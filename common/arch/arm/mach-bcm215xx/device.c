/*******************************************************************************
* Copyright 2010 Broadcom Corporation.  All rights reserved.
*
*	@file   arch/arm/mach-bcm215xx/device.c
*
* Unless you and Broadcom execute a separate written software license agreement
* governing use of this software, this software is licensed to you under the
* terms of the GNU General Public License version 2, available at
* http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
*******************************************************************************/

/*
 * device specific definitions
 */
#include <linux/types.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/serial_8250.h>
#include <linux/proc_fs.h>
#include <linux/sysctl.h>
#include <linux/cpufreq.h>

#include <mach/sdio.h>
#include <asm/mach/map.h>
#include <mach/hardware.h>
#include <mach/setup.h>
#include <linux/io.h>
#include <linux/delay.h>

#ifdef CONFIG_USB_GADGET_DWC_OTG
#include <mach/lm.h>
#include <mach/reg_usb.h>
#include <mach/usbctl.h>
#endif

#ifdef CONFIG_BCM_AVS
#include <plat/bcm_avs.h>
#endif

#if defined(CONFIG_BCM_CPU_FREQ)
#include <plat/bcm_cpufreq_drv.h>
#include <mach/reg_clkpwr.h>
#endif

#if defined(CONFIG_CPU_FREQ_GOV_BCM21553)
#include <mach/bcm21553_cpufreq_gov.h>
#endif

#include <linux/broadcom/types.h>
#include <linux/broadcom/bcmtypes.h>

#if defined(CONFIG_BRCM_FUSE_SYSPARM)
#include <linux/broadcom/bcm_fuse_sysparm.h>
#endif

#include <linux/mfd/max8986/max8986.h>

#if defined(CONFIG_SERIAL_8250) || defined(CONFIG_SERIAL_8250_MODULE)
/*!
 * The serial port definition structure.
 */
struct plat_serial8250_port serial_platform_data0[] = {
	{
	 .membase = (void __iomem *)IO_ADDRESS(BCM21553_UART_A_BASE),
	 .mapbase = (resource_size_t) (BCM21553_UART_A_BASE),
	 .irq = IRQ_UARTA,
	 .uartclk = 13000000,
	 .regshift = 2,
	 .iotype = UPIO_MEM,
	 .flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
	 .type = PORT_16550A,
	 .private_data =
	 (void __iomem *)(IO_ADDRESS(BCM21553_UART_A_BASE) + 0x100),
	 }, {
	     .flags = 0,
	     },
};

struct plat_serial8250_port serial_platform_data1[] = {
	{
	 .membase = (void __iomem *)IO_ADDRESS(BCM21553_UART_B_BASE),
	 .mapbase = (resource_size_t) (BCM21553_UART_B_BASE),
	 .irq = IRQ_UARTB,
	 .uartclk = 13000000,
	 .regshift = 2,
	 .iotype = UPIO_MEM,
	 .flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST |  UPF_LOW_LATENCY,
	 .type = PORT_16550A,
	 .private_data =
	 (void __iomem *)(IO_ADDRESS(BCM21553_UART_B_BASE) + 0x100),
	 }, {
	     .flags = 0,
	     },
};

struct plat_serial8250_port serial_platform_data2[] = {
	{
	 .membase = (void __iomem *)IO_ADDRESS(BCM21553_UART_C_BASE),
	 .mapbase = (resource_size_t) (BCM21553_UART_C_BASE),
	 .irq = IRQ_UARTC,
	 .uartclk = 13000000,
	 .regshift = 2,
	 .fifosize = 16,
	 .iotype = UPIO_MEM,
	 .flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
	 .type = PORT_16550A,
	 .private_data =
	 (void __iomem *)(IO_ADDRESS(BCM21553_UART_C_BASE) + 0x100),
	 }, {
	     .flags = 0,
	     },
};

struct platform_device bcm21553_serial_device0 = {
	.name = "serial8250",
	.id = PLAT8250_DEV_PLATFORM,
};

struct platform_device bcm21553_serial_device1 = {
	.name = "serial8250",
	.id = PLAT8250_DEV_PLATFORM1,
};

struct platform_device bcm21553_serial_device2 = {
	.name = "serial8250",
	.id = PLAT8250_DEV_PLATFORM2,
};
#endif

#if defined(CONFIG_I2C_BCM1160)
/*
 * The I2C definition structure.
 */

static struct resource bcm21553_i2c1_resources[] = {
	{
	 .start	= IO_ADDRESS(BCM21553_I2C1_BASE),
	 .end	= (IO_ADDRESS(BCM21553_I2C1_BASE) + SZ_4K - 1),
	 .flags	= IORESOURCE_MEM,
	},
	{
	 .start	= IRQ_I2C1,
	 .end	= IRQ_I2C1,
	 .flags	= IORESOURCE_IRQ,
	},
};

struct platform_device bcm21553_device_i2c1 = {
	.name		= "i2cbcm",
	.id		= 0,
	.resource	= bcm21553_i2c1_resources,
	.num_resources	= ARRAY_SIZE(bcm21553_i2c1_resources),
};

static struct resource bcm21553_i2c2_resources[] = {
	{
	 .start	= IO_ADDRESS(BCM21553_I2C2_BASE),
	 .end	= (IO_ADDRESS(BCM21553_I2C2_BASE + SZ_4K) - 1),
	 .flags	= IORESOURCE_MEM,
	},
	{
	 .start	= IRQ_I2C2,
	 .end	= IRQ_I2C2,
	 .flags	= IORESOURCE_IRQ,
	},
};

struct platform_device bcm21553_device_i2c2 = {
	.name		= "i2cbcm",
	.id		= 1,
	.resource	= bcm21553_i2c2_resources,
	.num_resources	= ARRAY_SIZE(bcm21553_i2c2_resources),
};

#if defined(CONFIG_ARCH_BCM21553_B0) || defined(CONFIG_ARCH_BCM21553_B1)
static struct resource bcm21553_i2c3_resources[] = {
	{
	 .start	= IO_ADDRESS(BCM21553_I2C3_BASE),
	 .end	= (IO_ADDRESS(BCM21553_I2C3_BASE + SZ_4K) - 1),
	 .flags	= IORESOURCE_MEM,
	},
	{
	 .start	= IRQ_I2C3,
	 .end	= IRQ_I2C3,
	 .flags	= IORESOURCE_IRQ,
	},
};

struct platform_device bcm21553_device_i2c3 = {
	.name		= "i2cbcm",
	.id		= 2,
	.resource	= bcm21553_i2c3_resources,
	.num_resources	= ARRAY_SIZE(bcm21553_i2c3_resources),
};
#endif
#endif

#if defined(CONFIG_BCM_WATCHDOG)
/*!
 * The watchdog  definition structure.
 */

static struct resource bcm_wdt_res[] = {
	{
	 .start = IO_ADDRESS(BCM21553_WDT_BASE1),
	 .end = IO_ADDRESS(BCM21553_WDT_BASE1) + SZ_16 - 1,
	 .flags = IORESOURCE_MEM,
	 },
	{
	 .start = IRQ_WDOG2,
	 .flags = IORESOURCE_IRQ,
	 },
};

struct platform_device bcm_watchdog_device = {
	.name = "watchdog",
	.id = 0,
	.resource = bcm_wdt_res,
	.num_resources = ARRAY_SIZE(bcm_wdt_res),
};
#endif

#if defined (CONFIG_MMC_BCM)

extern int bcmsdhc_ctrl_slot_is_invalid(u8 ctrl_slot);

int bcmsdhc_enable_int(void __iomem *ioaddr, u8 ctrl_slot)
{
	if (bcmsdhc_ctrl_slot_is_invalid(ctrl_slot))
		return -EINVAL;

	writel(SDIO_SDHOST_COREIMR_IP, ioaddr + SDHOST_COREIMR_SHIFT);

	return 0;
}

/* *************************************************************************************************** */
/* Function Name: bcmsdhc_external_reset */
/* Description: external reset SD host */
/* *************************************************************************************************** */

int bcmsdhc_external_reset(void __iomem *ioaddr, u8 ctrl_slot)
{
	u32 result = 0;

	if (bcmsdhc_ctrl_slot_is_invalid(ctrl_slot))
		return -EINVAL;

	writel(SDHOST_CORECTRL_RESET, ioaddr + SDHOST_CORECTRL_SHIFT);
	mdelay(5);
	writel(result, ioaddr + SDHOST_CORECTRL_SHIFT);

	return 0;
}

int bcmsdhc_cfg_card_detect(void __iomem *ioaddr, u8 ctrl_slot)
{
	u32 value = 0;

	if (bcmsdhc_ctrl_slot_is_invalid(ctrl_slot))
		return -EINVAL;

	if (ctrl_slot == 1) {
		/* slot 1 is for Wifi ,Disable WP and CD
		   Disable WP and CD */
		value &= ~(SDIO_SDHOST_CORESTAT_CD | SDIO_SDHOST_CORESTAT_WP);
		writel(value, ioaddr + SDHOST_CORESTAT_SHIFT);
	} else if (ctrl_slot == 2) {
		/* For  SD/MMC card
		   Disable WP,Enable CD */
		value = SDIO_SDHOST_CORESTAT_CD;
		writel(value, ioaddr + SDHOST_CORESTAT_SHIFT);
		/* For power saving,do not enable AHB clock at here
		   writel(SDIO_SDHOST_CORECTRL_EN, ioaddr + SDHOST_CORECTRL_SHIFT); */
	} else if (ctrl_slot == 3) {
		/* Disable WP,Enable CD */
		value = SDIO_SDHOST_CORESTAT_CD;
		writel(value, ioaddr + SDHOST_CORESTAT_SHIFT);
	}

	return 0;
}

/*!
 * The MMC definition structure.
 */

static struct resource sdhc1_resources[] = {
	{
	 .start = IO_ADDRESS(BCM21553_SDIO1_BASE),
	 .end = IO_ADDRESS(BCM21553_SDIO1_BASE) + SZ_64K - 1,
	 .flags = IORESOURCE_MEM,
	 .name = "sdhc1_phy_addr",
	 },
	{
	 .start = IRQ_SDIO1,
	 .flags = IORESOURCE_IRQ,
	 .name = "sdhc1_irq",
	 },
};

struct platform_device bcm21553_sdhc_slot1 = {
	.name = "bcm_sdhc",
	.id = 1,
	.num_resources = ARRAY_SIZE(sdhc1_resources),
	.resource = sdhc1_resources,
};
EXPORT_SYMBOL(bcm21553_sdhc_slot1);
#if !defined(CONFIG_MTD_ONENAND)
/*
 * SDHC2 is used for eMMC and SDHC2 shares pin mux with FLASH(OneNAND)
 * So both OneNAND and SDHC2 cannot co-exist
 */
static struct resource sdhc2_resources[] = {
	{
	 .start = IO_ADDRESS(BCM21553_SDIO2_BASE),
	 .end = IO_ADDRESS(BCM21553_SDIO2_BASE) + SZ_64K - 1,
	 .flags = IORESOURCE_MEM,
	 .name = "sdhc2_phy_addr",
	 },
	{
	 .start = IRQ_SDIO2,
	 .flags = IORESOURCE_IRQ,
	 .name = "sdhc2_irq",
	 },
};

struct platform_device bcm21553_sdhc_slot2 = {
	.name = "bcm_sdhc",
	.id = 2,
	.num_resources = ARRAY_SIZE(sdhc2_resources),
	.resource = sdhc2_resources,
};
#endif

static struct resource sdhc3_resources[] = {
	{
	 .start = IO_ADDRESS(BCM21553_SDIO3_BASE),
	 .end = IO_ADDRESS(BCM21553_SDIO3_BASE) + SZ_64K - 1,
	 .flags = IORESOURCE_MEM,
	 .name = "sdhc3_phy_addr",
	 },
	{
	 .start = IRQ_SDIO3,
	 .flags = IORESOURCE_IRQ,
	 .name = "sdhc3_irq",
	 },
};

struct platform_device bcm21553_sdhc_slot3 = {
	.name = "bcm_sdhc",
	.id = 3,
	.num_resources = ARRAY_SIZE(sdhc3_resources),
	.resource = sdhc3_resources,
};
#endif

#ifdef CONFIG_USB_GADGET_DWC_OTG
int __init usb_devices_init(void)
{
	struct lm_device *lmdev;
	int rc = -1;

	/* Power Up the 48MHz PLL */
	/* REG_CLKPWR_USBPLL_ENABLE |= 1; */

	/* Turn on the output gate of the 48MHz PLL */
	/* REG_CLKPWR_USBPLL_OEN |= 1; */

	/* Turn on the HS USB in the usb control register */
	/* REG_USB_CONTROL_REG |= REG_USB_CONTROL_USB_ON; */

	lmdev = kmalloc(sizeof(struct lm_device), GFP_KERNEL);
	if (lmdev) {
		memset(lmdev, 0, sizeof(struct lm_device));

		lmdev->resource.start = HW_USBOTG_BASE;
		lmdev->resource.end = lmdev->resource.start + SZ_256K - 1;
		lmdev->resource.flags = IORESOURCE_MEM;

		lmdev->irq = IRQ_USBHSOTG;
		lmdev->id = -2;

		rc = lm_device_register(lmdev);
	}

	return rc;
}

subsys_initcall(usb_devices_init);

#endif // CONFIG_USB_GADGET_DWC_OTG

#if defined(CONFIG_KEYBOARD_BCM)
/*!
 * The keyboard definition structure.
 */

struct platform_device bcm215xx_kp_device = {
	.name = "bcm_keypad",
	.id = -1,
};
#endif

#ifdef CONFIG_BCM_PWM
static struct resource bcm_pwm_resource[] = {
	{
	 .start = IO_ADDRESS(BCM21553_PWM_TOP_BASE),
	 .end = IO_ADDRESS(BCM21553_PWM_TOP_BASE) + SZ_4K - 1,
	 .flags = IORESOURCE_MEM,
	 },
};

struct platform_device bcm_pwm_device = {
	.name = "bcm_pwm",
	.id = -1,
	.resource = bcm_pwm_resource,
	.num_resources = ARRAY_SIZE(bcm_pwm_resource),
};

#endif

#if defined(CONFIG_BCM_AUXADC)
/*!
 * The auxadc definition structure.
 */

static struct resource bcm_auxadc_resource[] = {
        {
                .start  = IO_ADDRESS(BCM21553_AUXADC_BASE),
                .end    = IO_ADDRESS(BCM21553_AUXADC_BASE) + SZ_16 - 1,
                .flags  = IORESOURCE_MEM,
        },
};

struct platform_device auxadc_device={
        .name = "bcm_auxadc",
        .id   = -1,
	.resource       = bcm_auxadc_resource,
        .num_resources  = ARRAY_SIZE(bcm_auxadc_resource),
};
#endif

#ifdef CONFIG_BCM_I2SDAI
static struct resource bcm_i2sdai_resource[] = {
	{
	 .start = IO_ADDRESS(BCM21553_I2S_BASE),
	 .end = IO_ADDRESS(BCM21553_I2S_BASE) + SZ_4K - 1,
	 .flags = IORESOURCE_MEM,
	 },
};

struct platform_device i2sdai_device = {
	.name = "bcm_i2sdai",
	.id = -1,
	.resource = bcm_i2sdai_resource,
	.num_resources = ARRAY_SIZE(bcm_i2sdai_resource),
};
#endif

#ifdef CONFIG_SPI
static struct resource bcm21xx_spi_resources[] = {
	{
	 .start = IO_ADDRESS(BCM21553_SPI1_BASE),
	 .end = IO_ADDRESS(BCM21553_SPI1_BASE) + SZ_4K - 1,
	 .flags = IORESOURCE_MEM,
	 }, {
	     .start = IRQ_SPI,
	     .end = IRQ_SPI,
	     .flags = IORESOURCE_IRQ,
	     },
};

struct platform_device bcm21xx_device_spi = {
	.name = "bcm_spi",
	.id = 0,
	.resource = bcm21xx_spi_resources,
	.num_resources = ARRAY_SIZE(bcm21xx_spi_resources),
};
#endif

#if defined(CONFIG_BRCM_HEADSET) || defined(CONFIG_BRCM_HEADSET_MODULE)
struct platform_device bcm_headset_device = {
	.name = "bcmheadset",
	.id = -1,
};
#endif

#if defined(CONFIG_BCM_OTP)
static struct resource bcm_otp[] = {
	{
	 .start = IO_ADDRESS(BCM21553_OTP_BASE),
	 .end = IO_ADDRESS(BCM21553_OTP_BASE) + SZ_4K - 1,
	 .flags = IORESOURCE_MEM,
	 },
};

struct platform_device bcm_otp_device = {
	.name = "otp",
	.id = -1,
	.resource = bcm_otp,
	.num_resources = ARRAY_SIZE(bcm_otp),
};
#endif

#if defined(CONFIG_DMADEVICES) && defined(CONFIG_OSDAL_SUPPORT)
static struct resource	bcm21xx_dma_resources[] = {
	{
		.start	= IO_ADDRESS(BCM21553_DMAC_BASE),
		.end	= IO_ADDRESS(BCM21553_DMAC_BASE) + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= IRQ_DMA,
		.end	= IRQ_DMA,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device	bcm21xx_dma_device = {
	.name		= "bcm21xx-dma",
	.id		= -1,
	.resource	= bcm21xx_dma_resources,
	.num_resources	= ARRAY_SIZE(bcm21xx_dma_resources),
};
#endif

#ifdef CONFIG_BCM215XX_DSS
static struct resource lcdc_resources[] = {
	{
	 .start = IO_ADDRESS(BCM21553_LCDC_BASE),
	 .end = IO_ADDRESS(BCM21553_LCDC_BASE) + SZ_256 - 1,
	 .flags = IORESOURCE_MEM,
	 },
};

struct platform_device bcm215xx_lcdc_device = {
	.name		= "LCDC",
	.id		= -1,
	.resource	= lcdc_resources,
	.num_resources	= ARRAY_SIZE(lcdc_resources),
};
#endif

#define BCM_CORECLK_TURBO	BCM21553_CORECLK_KHZ_832
#define BCM_CORE_CLK_NORMAL	BCM21553_CORECLK_KHZ_312

#if defined(CONFIG_BCM_CPU_FREQ)
/*********************************************************************
 *                  DATA FOR BCM CPUFREQ PLAT DRIVER                 *
 *********************************************************************/

/* Indices for the voltage to frequency mapping table */
enum {
	BCM_NORMAL_MODE,
	BCM_TURBO_MODE,
};

/* Voltage-Frequency mapping for BCM21553 CPU0 */
static struct bcm_freq_tbl bcm215xx_cpu0_freq_tbl[] = {
	FTBL_INIT(BCM_CORE_CLK_NORMAL / 1000, 1200000),
	FTBL_INIT(BCM_CORECLK_TURBO / 1000, 1360000),
};
/* BCM21553 CPU info */
static struct bcm_cpu_info bcm215xx_cpu_info[] = {
	[0] = {
		.freq_tbl = bcm215xx_cpu0_freq_tbl,
		.num_freqs = ARRAY_SIZE(bcm215xx_cpu0_freq_tbl),
		.cpu_latency = CPUFREQ_ETERNAL,
		.cpu_clk = "arm11",
		.appspll_en_clk = "appspll_en",
		.cpu_regulator = "csr_nm2",
		.index_turbo = BCM_TURBO_MODE,
	},
};
/* Platform data for BCM CPU freq driver */
static struct bcm_cpufreq_drv_plat bcm21553_cpufreq_drv_plat = {
	.info = bcm215xx_cpu_info,
	.nr_cpus = ARRAY_SIZE(bcm215xx_cpu_info),
};

struct platform_device bcm21553_cpufreq_drv = {
	.name   = "bcm21553-cpufreq-drv",
	.id     = -1,
	.dev    = {
		.platform_data = &bcm21553_cpufreq_drv_plat,
	},
};
#endif /* CONFIG_BCM_CPU_FREQ */

#if defined(CONFIG_CPU_FREQ_GOV_BCM21553)
/*********************************************************************
 *                DATA FOR BCM21553 CPUFREQ GOVERNOR                 *
 *********************************************************************/

static struct bcm21553_cpufreq_gov_plat bcm21553_cpufreq_gov_plat = {
	.freq_turbo = BCM_CORECLK_TURBO,
	.freq_normal = BCM_CORE_CLK_NORMAL,
};

struct platform_device bcm21553_cpufreq_gov = {
	.name   = "bcm21553-cpufreq-gov",
	.id     = -1,
	.dev    = {
		.platform_data = &bcm21553_cpufreq_gov_plat,
	},
};
#endif /* CONFIG_CPU_FREQ_GOV_BCM21553 */

#if defined(CONFIG_BCM_AVS)
/*********************************************************************
 *                        DATA FOR AVS DRIVER                        *
 *********************************************************************/

#define NM2_FF_VOLTAGE_NORMAL	1180000
#define NM2_TT_VOLTAGE_NORMAL	1240000
#define NM2_SS_VOLTAGE_NORMAL	1300000

#define NM2_FF_VOLTAGE_TURBO	1220000
#define NM2_TT_VOLTAGE_TURBO	1300000
#define NM2_SS_VOLTAGE_TURBO	1360000

#define NM_FF_VOLTAGE		1320000
#define NM_TT_VOLTAGE		1320000
#define NM_SS_VOLTAGE		1360000

#define FF_THRESHOLD 445
#define SS_THRESHOLD 395

static struct silicon_type_info part_type_ss = {
	.lpm_voltage = -1, /* Pass -1 if no update needed */
	.nm_voltage = NM_SS_VOLTAGE,
	.nm2_normal_voltage = NM2_SS_VOLTAGE_NORMAL,
	.nm2_turbo_voltage = NM2_SS_VOLTAGE_TURBO,
};

static struct silicon_type_info part_type_tt = {
	.lpm_voltage = -1, /* Pass -1 if no update needed */
	.nm_voltage = NM_TT_VOLTAGE,
	.nm2_normal_voltage = NM2_TT_VOLTAGE_NORMAL,
	.nm2_turbo_voltage = NM2_TT_VOLTAGE_TURBO,
};

static struct silicon_type_info part_type_ff = {
	.lpm_voltage = -1, /* Pass -1 if no update needed */
	.nm_voltage = NM_FF_VOLTAGE,
	.nm2_normal_voltage = NM2_FF_VOLTAGE_NORMAL,
	.nm2_turbo_voltage = NM2_FF_VOLTAGE_TURBO,
};

#ifdef CONFIG_BCM_CPU_FREQ
/* If cpufreq is enabled, this callback updates the voltages
 * for the normal and turbo modes in the cpufreq driver
 * voltage-to-frequency mapping table.
 */
static void bcm215xx_avs_notify(int silicon_type)
{
	int normal;
	int turbo;

	pr_info("%s: silicon_type : %d\n", __func__, silicon_type);

	switch(silicon_type)
	{
	case SILICON_TYPE_SLOW:
		normal = part_type_ss.nm2_normal_voltage;
		turbo = part_type_ss.nm2_turbo_voltage;
		break;

	case SILICON_TYPE_TYPICAL:
		normal = part_type_tt.nm2_normal_voltage;
		turbo = part_type_tt.nm2_turbo_voltage;
		break;

	case SILICON_TYPE_FAST:
		normal = part_type_ff.nm2_normal_voltage;
		turbo = part_type_ff.nm2_turbo_voltage;
		break;

	default:
		normal = part_type_ss.nm2_normal_voltage;
		turbo = part_type_ss.nm2_turbo_voltage;
		break;
	}

	if (normal >= 0)
		bcm215xx_cpu0_freq_tbl[BCM_NORMAL_MODE].cpu_voltage =
			(u32)normal;
	if (turbo >= 0)
		bcm215xx_cpu0_freq_tbl[BCM_TURBO_MODE].cpu_voltage =
			(u32)turbo;
}
#else
#define bcm215xx_avs_notify NULL
#endif

static struct bcm_avs_platform_data_t bcm_avs_pdata = {
	/* Pass NULL if not supported/need not update */
	.core_lpm_regl = NULL,
	.core_nml_regl = "csr_nm1",
	.core_turbo_regl = "csr_nm2",

	.otp_bit_lsb = 169,
	.otp_bit_msb = 188,

	.fast_silicon_thold = FF_THRESHOLD,
	.slow_silicon_thold = SS_THRESHOLD,

	.fast_silicon = &part_type_ff,
	.typical_silicon = &part_type_tt,
	.slow_silicon = &part_type_ss,
	.notify_silicon_type = bcm215xx_avs_notify,
};

struct platform_device bcm215xx_avs_device = {
	.name   = "bcm-avs-drv",
	.id     = -1,
	.dev    = {
		.platform_data = &bcm_avs_pdata,
	},
};

#if defined(CONFIG_BRCM_FUSE_SYSPARM)

#define SYSPARM_VOLT(name, dst)						\
	do {								\
		int parm;						\
		int ret = SYSPARM_GetParmU32ByName(name, &parm);	\
		if (ret == 0) {						\
			pr_info("sysparm: %s: %x\n", name, parm);	\
			parm = max8986_csr_reg2volt(parm);		\
			if (parm >= 0)					\
				dst = (u32)parm;			\
		}							\
	} while (0)

#define SYSPARM_READ32(name, dst)					\
	do {								\
		unsigned int parm;					\
		int ret = SYSPARM_GetParmU32ByName(name, &parm);	\
		if (ret == 0) {						\
			pr_info("sysparm: %s: %x\n", name, parm);	\
			dst = parm;					\
		}							\
	} while (0)

void __init update_avs_sysparm(void)
{
	SYSPARM_VOLT("nm2_ff_voltage_turbo", part_type_ff.nm2_turbo_voltage);
	SYSPARM_VOLT("nm2_tt_voltage_turbo", part_type_tt.nm2_turbo_voltage);
	SYSPARM_VOLT("nm2_ss_voltage_turbo", part_type_ss.nm2_turbo_voltage);

	SYSPARM_VOLT("nm2_ff_voltage_normal", part_type_ff.nm2_normal_voltage);
	SYSPARM_VOLT("nm2_tt_voltage_normal", part_type_tt.nm2_normal_voltage);
	SYSPARM_VOLT("nm2_ss_voltage_normal", part_type_ss.nm2_normal_voltage);

	SYSPARM_VOLT("nm_ff_voltage", part_type_ff.nm_voltage);
	SYSPARM_VOLT("nm_tt_voltage", part_type_tt.nm_voltage);
	SYSPARM_VOLT("nm_ss_voltage", part_type_ss.nm_voltage);

	SYSPARM_READ32("avs_otp_threshold_sstt",
		     bcm_avs_pdata.slow_silicon_thold);
	SYSPARM_READ32("avs_otp_threshold_ttff",
		     bcm_avs_pdata.fast_silicon_thold);
}
#endif /* CONFIG_BRCM_FUSE_SYSPARM */
#endif /* CONFIG_BCM_AVS */

#if defined (CONFIG_BRCM_V3D)
struct platform_device v3d_device = {
    .name   =   "v3d_dev",
    .id     =   -1,
};
#endif

