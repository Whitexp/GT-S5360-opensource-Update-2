/*******************************************************************************
Copyright 2010 Broadcom Corporation.  All rights reserved.

Unless you and Broadcom execute a separate written software license agreement
governing use of this software, this software is licensed to you under the
terms of the GNU General Public License version 2, available at
http://www.gnu.org/copyleft/gpl.html (the "GPL").

Notwithstanding the above, under no circumstances may you combine this software
in any way with any other Broadcom software provided under a license other than
the GPL, without Broadcom's express prior written consent.
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <mach/hardware.h>
#include <mach/irqs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/clk.h>
#include <mach/clkmgr.h>
#include <plat/syscfg.h>
#include <linux/platform_device.h>
#include <linux/broadcom/v3d.h>
#include "reg_v3d.h"

#if !defined (CONFIG_BRCM_V3D_OPT)

#ifndef V3D_DEV_NAME
#define V3D_DEV_NAME	"v3d"
#endif

#ifndef V3D_DEV_MAJOR
#define V3D_DEV_MAJOR	0
#endif

#ifndef BCM_CLK_V3D_POWER_STR_ID
#define BCM_CLK_V3D_POWER_STR_ID	""
#endif

#ifndef BCM21553_V3D_BASE
#define BCM21553_V3D_BASE	0
#endif

#ifndef IRQ_GRAPHICS
#define IRQ_GRAPHICS	0
#endif

#define FLAG0	0x0
#define FLAG1	0x4
#define TURN	0x8

#define V3D_SERVER_ID	1	// 0 is treated as invalid id


static int v3d_major = V3D_DEV_MAJOR;
static struct class *v3d_class;
static void __iomem *v3d_base = NULL;
static struct clk *gClkPower, *gClkAHB;
#ifdef CONFIG_CPU_FREQ_GOV_BCM21553
#include <mach/bcm21553_cpufreq_gov.h>
static struct cpufreq_client_desc *cpufreq_client;
#endif
static spinlock_t v3d_id_lock = SPIN_LOCK_UNLOCKED;
static u32 v3d_id = V3D_SERVER_ID;
static int v3d_in_use = 0;
#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
extern void __iomem *sync_flag_base;
#endif
extern void *v3d_mempool_base;
extern unsigned long v3d_mempool_size;

/* Static variable to check if the V3D power island is ON or OFF */
static int v3d_is_on = 0;

typedef struct {
	mem_t mempool;
	struct semaphore irq_sem;
	spinlock_t lock;
	gl_irq_flags_t irq_flags;
	u32 id;
} v3d_t;


#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
// Athena B0 V3D APB read back bug workaround
static inline void v3d_clean(void)
{
	iowrite32(0, v3d_base + SCRATCH);
	if (ioread32(v3d_base + SCRATCH))
		iowrite32(0, v3d_base + SCRATCH);
}

void sync_mutex_down(volatile void __iomem *base)
{
	iowrite32(1, base + FLAG0);
	iowrite32(1, base + TURN);

	while ((ioread32(base + FLAG1) == 1) && (ioread32(base + TURN) == 1));
}

void sync_mutex_up(volatile void __iomem *base)
{
	iowrite32(0, base + FLAG0);
}
#endif

static inline u32 v3d_read(u32 reg)
{
	u32 flags;
#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
	sync_mutex_down(sync_flag_base);
#endif
	flags = ioread32(v3d_base + reg);
#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
	v3d_clean();
	sync_mutex_up(sync_flag_base);
#endif
	return flags;
}

static irqreturn_t v3d_isr(int irq, void *dev_id)
{
// DEBUG_V3D needs to be enabled in both kernel and userspace v3d.c file
#ifdef DEBUG_V3D
	static short interrupt_count = 0;
#endif
	v3d_t *dev;
	u32 flags, flags_qpu;
	int skip;
#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
	unsigned long flags2;
#endif

	dev = (v3d_t *)dev_id;

	if ((dev->id != V3D_SERVER_ID) && v3d_in_use) {
		return IRQ_RETVAL(1);
	}

	// see v3d.c :: v3d_lisr
#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
	spin_lock_irqsave(&dev->lock, flags2);
#endif
	flags = v3d_read(INTCTL);
	flags &= v3d_read(INTENA);
	flags &= v3d_read(INTCTL);
	flags_qpu = v3d_read(DBQITC);
#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
	spin_unlock_irqrestore(&dev->lock, flags2);
#endif

	// see khrn_prod_4.c :: khrn_hw_isr
	iowrite32(flags, v3d_base + INTCTL);
	/* this interrupt will be forced high until we supply some memory... */
	if (flags & (1 << 2))
		iowrite32(1 << 2, v3d_base + INTDIS);

	if (flags_qpu)
		iowrite32(flags_qpu, v3d_base + DBQITC);

	if (!v3d_in_use) {
		pr_err("\nv3d residual interrupt caught by handler with id = %d !\n", dev->id);
		return IRQ_RETVAL(1);
	}

	// skip one interrupt notification if interrupt happens before notification to userspace
	if (dev->irq_flags.v3d_irq_flags || dev->irq_flags.qpu_irq_flags)
		skip = 1;
	else
		skip = 0;

	// pass flags up to user space
	dev->irq_flags.v3d_irq_flags |= flags;
	dev->irq_flags.qpu_irq_flags |= flags_qpu;

#ifdef DEBUG_V3D
	dev->irq_flags &= 0xffff;
	dev->irq_flags |= (++interrupt_count << 16);
	pr_debug("V3D interrupt 0x%04x, irq_flags = 0x%04x, flags = 0x%04x, previous interrupt skip = %d\n", \
		(dev->irq_flags >> 16), (dev->irq_flags & 0xffff), flags, skip);
#endif

	if (!skip)
		up(&dev->irq_sem);

	return IRQ_RETVAL(1);
}

static void v3d_power(int flag)
{
	if (flag) {
		/* Enable V3D island power */
		clk_enable(gClkPower);
	} else {
		/* Disable V3D island power */
		clk_disable(gClkPower);
	}
}

static void v3d_turn_all_on(void)
{
#ifdef CONFIG_CPU_FREQ_GOV_BCM21553
	cpufreq_bcm_dvfs_disable(cpufreq_client);
#endif
	/* Check if the V3D power island is already ON. If not ON, switch it on */
	if (!v3d_is_on) {
		v3d_power(1);
		v3d_is_on = 1;
	}

	clk_enable(gClkAHB);
}

static void v3d_turn_all_off(void)
{
	clk_disable(gClkAHB);
#ifdef CONFIG_CPU_FREQ_GOV_BCM21553
	cpufreq_bcm_dvfs_enable(cpufreq_client);
#endif
}

static int v3d_open(struct inode *inode, struct file *filp)
{
	int ret;
	unsigned long flags;

	v3d_t *dev = kmalloc(sizeof(v3d_t), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	filp->private_data = dev;

	dev->mempool.ptr = v3d_mempool_base;
	dev->mempool.addr = virt_to_phys(dev->mempool.ptr);
	dev->mempool.size = v3d_mempool_size;

	sema_init(&dev->irq_sem, 0);
	dev->lock = SPIN_LOCK_UNLOCKED;
	dev->irq_flags.v3d_irq_flags = 0;
	dev->irq_flags.qpu_irq_flags = 0;
	dev->id = 0;

	ret = request_irq(IRQ_GRAPHICS, v3d_isr,
			IRQF_DISABLED | IRQF_SHARED, V3D_DEV_NAME, dev);
	if (ret)
		goto err;

	spin_lock_irqsave(&v3d_id_lock, flags);
	if (v3d_id == 0)
		v3d_id = 1;
	dev->id = v3d_id++;
	if (dev->id == V3D_SERVER_ID)
		v3d_in_use = 1;
	spin_unlock_irqrestore(&v3d_id_lock, flags);

	return ret;

err:
	if (dev)
		kfree(dev);
	return ret;
}

static int v3d_release(struct inode *inode, struct file *filp)
{
	unsigned long flags;
	v3d_t *dev = (v3d_t *)filp->private_data;

	spin_lock_irqsave(&v3d_id_lock, flags);
	if(v3d_id > V3D_SERVER_ID)
		v3d_id--;
	if (dev->id == V3D_SERVER_ID) {
		v3d_in_use = 0;
	}
	spin_unlock_irqrestore(&v3d_id_lock, flags);

	free_irq(IRQ_GRAPHICS, dev);
	if (dev)
		kfree(dev);

	return 0;
}

#define pgprot_cached(prot) \
__pgprot((pgprot_val(prot) & ~L_PTE_MT_MASK) | L_PTE_MT_WRITEBACK)

static int v3d_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long vma_size = vma->vm_end - vma->vm_start;
	v3d_t *dev = (v3d_t *)(filp->private_data);

	if (vma_size & (~PAGE_MASK)) {
		pr_err(KERN_ERR "v3d_mmap: mmaps must be aligned to a multiple of pages_size.\n");
		return -EINVAL;
	}

	if (!vma->vm_pgoff) {
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	}
	else
	{
		vma->vm_page_prot = pgprot_cached(vma->vm_page_prot);
	}
	if (!vma->vm_pgoff) {
		vma->vm_pgoff = BCM21553_V3D_BASE >> PAGE_SHIFT;
	} else if (vma->vm_pgoff != (dev->mempool.addr >> PAGE_SHIFT)) {
		return -EINVAL;
	}


	/* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
	if (remap_pfn_range(vma,
			vma->vm_start,
			vma->vm_pgoff,
			vma_size,
			vma->vm_page_prot)) {
		pr_err("%s(): remap_pfn_range() failed\n", __FUNCTION__);
		return -EINVAL;
	}

	return 0;
}

static int v3d_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	v3d_t *dev;
	int ret = 0;

	if(_IOC_TYPE(cmd) != BCM_V3D_MAGIC)
		return -ENOTTY;

	if(_IOC_NR(cmd) > V3D_CMD_LAST)
		return -ENOTTY;

	if(_IOC_DIR(cmd) & _IOC_READ)
		ret = !access_ok(VERIFY_WRITE, (void *) arg, _IOC_SIZE(cmd));

	if(_IOC_DIR(cmd) & _IOC_WRITE)
		ret |= !access_ok(VERIFY_READ, (void *) arg, _IOC_SIZE(cmd));

	if(ret)
		return -EFAULT;

	dev = (v3d_t *)(filp->private_data);

	switch (cmd) {
	case V3D_IOCTL_GET_MEMPOOL:
		if (copy_to_user((mem_t *)arg, &(dev->mempool), sizeof(mem_t)))
			ret = -EPERM;
		break;

	case V3D_IOCTL_WAIT_IRQ:
		{
		gl_irq_flags_t usr_irq_flags;
		unsigned long flags;

		if (down_interruptible(&dev->irq_sem))
			return -ERESTARTSYS;

		spin_lock_irqsave(&dev->lock, flags);
		usr_irq_flags.v3d_irq_flags = dev->irq_flags.v3d_irq_flags;
		dev->irq_flags.v3d_irq_flags = 0;
		usr_irq_flags.qpu_irq_flags = dev->irq_flags.qpu_irq_flags;
		dev->irq_flags.qpu_irq_flags = 0;
		usr_irq_flags.early_suspend = 0;
		spin_unlock_irqrestore(&dev->lock, flags);

		if (copy_to_user((u32 *)arg, &usr_irq_flags, sizeof(usr_irq_flags))) {
			pr_err("V3D_IOCTL_WAIT_IRQ copy_to_user failed\n");
			ret = -EFAULT;
		}

		break;
		}

	case V3D_IOCTL_READ_REG:
		{
		u32 reg_addr, reg_value;
#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
		unsigned long flags;
#endif
		if (copy_from_user(&reg_addr, (u32 *)arg, sizeof(reg_addr))) {
			pr_err("V3D_IOCTL_READ_REG copy_from_user failed\n");
			ret = -EFAULT;
			break;
		}

#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
		spin_lock_irqsave(&dev->lock, flags);
#endif
		reg_value = v3d_read(reg_addr);
#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
		spin_unlock_irqrestore(&dev->lock, flags);
#endif

		if (copy_to_user((u32 *)arg, &reg_value, sizeof(reg_value))) {
			pr_err("V3D_IOCTL_READ_REG copy_to_user failed\n");
			ret = -EFAULT;
		}

		break;
		}

	case V3D_IOCTL_SOFT_RESET:
		board_sysconfig(SYSCFG_V3D, SYSCFG_INIT);
		break;

	case V3D_IOCTL_TURN_ON:
		v3d_turn_all_on();
		break;

	case V3D_IOCTL_TURN_OFF:
		v3d_turn_all_off();
		break;

	default:
		break;
	}

	return ret;
}

static struct file_operations v3d_fops =
{
	.open		= v3d_open,
	.release	= v3d_release,
	.mmap		= v3d_mmap,
	.ioctl		= v3d_ioctl,
};


static int v3d_suspend (struct platform_device *pdev)
{
    /* If the V3D power island is On, switch is OFF*/
    if (v3d_is_on) {
        v3d_power(0);
        v3d_is_on = 0;
    }

    return 0;
}

static int v3d_resume (struct platform_device *pdev)
{
    return 0;
}

struct platform_driver v3d_driver = {
    .driver =   {
        .name   =   "v3d_dev",
        .owner  =   THIS_MODULE,
    },
    .suspend    =   v3d_suspend,
    .resume     =   v3d_resume,
};

#endif /* #if !defined (CONFIG_BRCM_V3D_OPT) */

int __init v3d_init(void)
{
#if !defined (CONFIG_BRCM_V3D_OPT)
	int ret;

	ret = register_chrdev(0, V3D_DEV_NAME, &v3d_fops);
	if (ret < 0)
		return -EINVAL;
	else
		v3d_major = ret;

	v3d_class = class_create(THIS_MODULE, V3D_DEV_NAME);
	if (IS_ERR(v3d_class)) {
		unregister_chrdev(v3d_major, V3D_DEV_NAME);
		return PTR_ERR(v3d_class);
	}

	device_create(v3d_class, NULL, MKDEV(v3d_major, 0), NULL, V3D_DEV_NAME);

	gClkAHB = clk_get(NULL, BCM_CLK_V3D_STR_ID);
	gClkPower = clk_get(NULL, BCM_CLK_V3D_POWER_STR_ID);
#ifdef CONFIG_CPU_FREQ_GOV_BCM21553
	cpufreq_client = cpufreq_bcm_client_get("v3d");
#endif
	v3d_turn_all_on();

	v3d_base = (void __iomem *)ioremap_nocache(BCM21553_V3D_BASE, SZ_64K);
	if (v3d_base == NULL)
		goto err;

#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
	if (sync_flag_base == NULL) {
		iounmap(v3d_base);
		goto err;
	}
#endif

#ifdef CONFIG_BCM21553_V3D_SYNC_ENABLE
	{
	unsigned long flags;
	spin_lock_irqsave(&v3d_id_lock, flags);
	ret = v3d_read(IDENT0);
	spin_unlock_irqrestore(&v3d_id_lock, flags);
	}
#else
	ret = v3d_read(IDENT0);
#endif
	pr_info("v3d id = 0x%04x\n", ret);

	platform_driver_register(&v3d_driver);
	v3d_turn_all_off();

	return 0;

err:
	v3d_turn_all_off();
#ifdef CONFIG_CPU_FREQ_GOV_BCM21553
	cpufreq_bcm_client_put(cpufreq_client);
#endif
	unregister_chrdev(v3d_major, V3D_DEV_NAME);

    platform_driver_unregister(&v3d_driver);

    return ret;
#else
	return 0;
#endif
}

void __exit v3d_exit(void)
{
#if !defined (CONFIG_BRCM_V3D_OPT)

#ifdef CONFIG_CPU_FREQ_GOV_BCM21553
	cpufreq_bcm_client_put(cpufreq_client);
#endif
	if (v3d_base)
		iounmap(v3d_base);
	device_destroy(v3d_class, MKDEV(v3d_major, 0));
	class_destroy(v3d_class);
	unregister_chrdev(v3d_major, V3D_DEV_NAME);
    platform_driver_unregister(&v3d_driver);
#endif
}

module_init(v3d_init);
module_exit(v3d_exit);

MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("V3D device driver");
MODULE_LICENSE("GPL");
