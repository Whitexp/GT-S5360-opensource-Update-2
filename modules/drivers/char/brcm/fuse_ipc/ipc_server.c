/*****************************************************************************
*  Copyright 2003 - 2007 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/


#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/time.h>
#ifdef CONFIG_HAS_WAKELOCK
#include <linux/wakelock.h>
#endif
#include <linux/semaphore.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/broadcom/ipcinterface.h>
#include <linux/broadcom/ipc_server_ifc.h>
#include <linux/broadcom/ipc_server_ioctl.h>
#include <linux/broadcom/bcm_fuse_memmap.h>
#include <linux/broadcom/bcm_reset_utils.h>

#include "lnx_support.h"
#include "ipc_server_ccb.h"
#include "ipc_debug.h"
#include "bcmlog.h"

#define IPC_MAJOR (204)

typedef struct 
{
  dev_t devnum;
  int ipc_state;
  int ap_ipc_init_done;
  struct semaphore ipc_sem;
  struct cdev cdev;               
  struct work_struct cp_crash_dump_wq;
  struct work_struct intr_work;
  struct workqueue_struct *intr_workqueue;
  void __iomem *apcp_shmem;
}ipcs_info_t;

// flag used to track occurence of early CP interrupt; 
// CP interrupt before IPC configured indicates early CP
// crash, so we'll do crash dump as soon as IPC is set up
static int sEarlyCPInterrupt = 0;

static ipcs_info_t g_ipc_info = { 0 };
#ifdef CONFIG_HAS_WAKELOCK
struct wake_lock ipc_wake_lock;
#endif
static IPC_PlatformSpecificPowerSavingInfo_T ipc_ps;

extern int IpcCPCrashCheck(void);
extern void ProcessCPCrashedDump(struct work_struct *work);

#if defined(CONFIG_BCM215X_PM) && defined(CONFIG_ARCH_BCM2153)
extern void pm_ipc_power_saving_init(IPC_PlatformSpecificPowerSavingInfo_T* ipc_ps);
#endif


typedef struct
{
    wait_queue_head_t evt_wait;
    int evt;
}IPC_Evt_t;

static void * EventCreate (void)
{
    IPC_Evt_t* ipcEvt;

    ipcEvt = kmalloc(sizeof(IPC_Evt_t), GFP_KERNEL);
    if (!ipcEvt)
        return (void*)IPC_ERROR;
    
    init_waitqueue_head(&(ipcEvt->evt_wait));
    ipcEvt->evt = 0;
    
	return ipcEvt;
}


static IPC_ReturnCode_T EventSet (void * Event)
{
    IPC_Evt_t* ipcEvt = (IPC_Evt_t*)Event;
    
    bcm_disable_reentrancy();

    // **FIXME** need to protect access to this?
    ipcEvt->evt = 1;
    wake_up(&(ipcEvt->evt_wait));

    bcm_enable_reentrancy();

    return IPC_OK;   
}

static IPC_ReturnCode_T EventClear (void * Event)
{

    IPC_Evt_t* ipcEvt = (IPC_Evt_t*)Event;
    
    // **FIXME** need to protect access to this?
    ipcEvt->evt = 0;
    //wakeup(&(ipcEvt->evt_wait));
    return IPC_OK;   
}

static IPC_ReturnCode_T EventWait (void * Event, IPC_U32 MilliSeconds)
{
    IPC_Evt_t* ipcEvt = (IPC_Evt_t*)Event;
	IPC_ReturnCode_T  rtnCode = IPC_OK;

	if(MilliSeconds == IPC_WAIT_FOREVER)
	{
		wait_event( (ipcEvt->evt_wait), (ipcEvt->evt == 1) );
	}
	else
	{
	    int timeout = 0;
	    // timeout in "jiffies" (apparently 10ms/jiffie in android?)
		timeout = wait_event_timeout( (ipcEvt->evt_wait), (ipcEvt->evt == 1), MilliSeconds/10 );
		// returns 0 if we timed out, > 0 otherwise
		if ( timeout == 0 )
		{
		    rtnCode = IPC_TIMEOUT;
		}
	}
	
	return rtnCode;
}

static int ipcs_open(struct inode *inode, struct file *file)
{
  ipcs_info_t *info;

  info = container_of(inode->i_cdev, ipcs_info_t, cdev);
  file->private_data = info;

  return 0;
}

static int ipcs_release(struct inode *inode, struct file *file)
{
  return(0);
}

ssize_t ipcs_read(struct file *filep, char __user *buf, size_t size, loff_t *off)
{
  ssize_t rc = 0;

  //rc = ipc_server_read(filep, buf, size, off);
      
  return(rc);
}

ssize_t ipcs_write(struct file *filep, const char __user *buf, size_t size, loff_t *off)
{
  return -EPERM;
}

static long ipcs_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
  int rc = 0;

   //rc = ipc_server_ioctl(cmd, arg);

  return(rc);
}


static struct file_operations ipc_ops = 
{
  .owner = THIS_MODULE,
  .open  = ipcs_open,
  .read  = ipcs_read,
  .write = ipcs_write,
  .unlocked_ioctl = ipcs_ioctl,
  .mmap  = NULL,
  .release = ipcs_release,
};

/** 
   @fn void ipcs_ipc_initialised(void);
*/
void ipcs_ipc_initialised(void)
{
   IPC_DEBUG(DBG_INFO,"IPC Initialization completed\n");
   g_ipc_info.ipc_state = 1;

   return;
}

/**
   @fn void ipcs_get_ipc_state(int *state);
 */
void ipcs_get_ipc_state(int *state)
{
   if (state != NULL)
      *state = g_ipc_info.ipc_state;

   return;
}


/**
   @fn void ipcs_ipc_reset(void);
*/
void ipcs_ipc_reset(void)
{
  IPC_DEBUG(DBG_INFO, ">> %s\n", __FUNCTION__);
  IPC_DEBUG(DBG_INFO, "<< %s\n", __FUNCTION__);

   return;
}

#if defined(CONFIG_SEC_DEBUG)
extern void cp_abort(void);
#else
extern void abort(void);
#endif

int cp_crashed = 0;
void ipcs_intr_workqueue_process(struct work_struct *work)
{
    static int first = 1;
    if (first)
    {
        first = 0;
        set_user_nice(current, -16);
    }


   if(IpcCPCrashCheck())
   {
		cp_crashed  = 1;
		if( BCMLOG_CPCRASH_MTD == BCMLOG_GetCpCrashDumpDevice() )
		{
		/* we kill AP when CP crashes */
			IPC_DEBUG(DBG_INFO, "Crashing AP now...\n\n");
#if defined(CONFIG_SEC_DEBUG)
    			cp_abort();
#else
    			abort();
#endif
		} else 
		{
		schedule_work(&g_ipc_info.cp_crash_dump_wq);
	}

		IPC_ProcessEvents();
	}else
	{
       IPC_ProcessEvents();  
#ifdef CONFIG_HAS_WAKELOCK 
       wake_unlock(&ipc_wake_lock);
#endif // CONFIG_HAS_WAKELOCK
   }
}


static irqreturn_t ipcs_interrupt(int irq, void *dev_id)
{
   //IPC_DEBUG(DBG_INFO, "[ipc]: ipcs_interrupt\n");
   // check for early CP interrupt
   if( 1 == g_ipc_info.ap_ipc_init_done )
   {
#ifdef CONFIG_HAS_WAKELOCK
       wake_lock(&ipc_wake_lock);
#endif
       queue_work(g_ipc_info.intr_workqueue, &g_ipc_info.intr_work);
   }

   else
   {
        // if we're interrupted before IPC is setup, that 
        // means CP has had an early crash....
        IPC_DEBUG(DBG_INFO, "[ipc]:  abnormal CP interrupt\n");
        sEarlyCPInterrupt = 1;
   }

  return IRQ_HANDLED;
}


/**
   @fn int ipc_ipc_init(void *smbase, unsigned int size);
*/
int __init ipc_ipc_init(void *smbase, unsigned int size)
{
  IPC_ControlInfo_T ipc_control;

  memset(&ipc_control, 0, sizeof(IPC_ControlInfo_T));

  ipc_control.RaiseEventFptr  = bcm_raise_cp_int;
  ipc_control.EnableReEntrancyFPtr = bcm_enable_reentrancy;
  ipc_control.DisableReEntrancyFPtr = bcm_disable_reentrancy;
  ipc_control.PhyToOSAddrFPtr = bcm_map_phys_to_virt;
  ipc_control.OSToPhyAddrFPtr = bcm_map_virt_to_phys;
  ipc_control.EventFunctions.Create = EventCreate;
  ipc_control.EventFunctions.Set = EventSet;
  ipc_control.EventFunctions.Clear = EventClear;
  ipc_control.EventFunctions.Wait = EventWait;
  ipc_control.PowerSavingStruct = &ipc_ps;

#if defined(CONFIG_BCM215X_PM) && defined(CONFIG_ARCH_BCM2153)
  pm_ipc_power_saving_init(&ipc_ps);
#endif

  IPC_Initialise(smbase, size, IPC_AP_CPU, &ipc_control,
                 ipcs_ipc_initialised, ipcs_ipc_reset, NULL);

   return(0);
}

/**
   static int ipcs_init(void *smbase, unsigned int size)
 */
static int __init ipcs_init(void *smbase, unsigned int size)
{
   int rc = 0;

   //Initialize OS specific callbacks with the IPC lib
  rc = ipc_ipc_init(smbase, size);
  if (rc) 
  {
    IPC_DEBUG(DBG_ERROR, "ipc_ipc_init() failed, ret[%d]\n", rc);
      return(rc);
  }

  //Register Endpoints 
  rc = ipcs_ccb_init();
  if (rc) 
  {
    IPC_DEBUG(DBG_ERROR, "ipcs_ccb_init() failed, ret[%d]\n", rc);
      return(rc);
  }

  // AP ipc init is done
  g_ipc_info.ap_ipc_init_done = 1;

  //Let CP know that we are done registering endpoints
  IPC_Configured();

  //Wait for IPC initialized
  IPC_DEBUG(DBG_INFO, "IPC_Configured() invoked\n");

  return(0);
}

static int __init ipcs_module_init(void)
{
  int rc = 0;
  int readyChkCnt = 0;
  struct timespec startTime, endTime;
  
  IPC_DEBUG(DBG_INFO,"[ipc]: ipcs_module_init start..\n");
  
  init_MUTEX_LOCKED(&g_ipc_info.ipc_sem);

  g_ipc_info.ipc_state = 0;

  g_ipc_info.devnum = MKDEV(IPC_MAJOR, 0);
  
  rc = register_chrdev_region(g_ipc_info.devnum, 1, "bcm_fuse_ipc");
  if (rc < 0) 
  {
    IPC_DEBUG(DBG_ERROR,"Error registering the IPC device\n");
    goto out;
  }

  cdev_init(&g_ipc_info.cdev, &ipc_ops);
  
  g_ipc_info.cdev.owner = THIS_MODULE;

  rc = cdev_add(&g_ipc_info.cdev, g_ipc_info.devnum, 1);
  if (rc) 
  {
    IPC_DEBUG(DBG_ERROR,"[ipc]: cdev_add errpr\n");
    goto out_unregister;
  }

  IPC_DEBUG(DBG_INFO, "[ipc]: create_workqueue\n");
 
  INIT_WORK(&g_ipc_info.cp_crash_dump_wq, ProcessCPCrashedDump);
  INIT_WORK(&g_ipc_info.intr_work, ipcs_intr_workqueue_process);

  g_ipc_info.intr_workqueue = create_workqueue("ipc-wq");
  if (!g_ipc_info.intr_workqueue)
  {
    IPC_DEBUG(DBG_ERROR,"[ipc]: cannot create workqueue\n");
    goto out_unregister;
  } 


  IPC_DEBUG(DBG_INFO, "[ipc]: request_irq\n");
  rc = request_irq(IRQ_IPC_C2A, ipcs_interrupt, IRQF_NO_SUSPEND, "ipc-intr", &g_ipc_info);
  if (rc) 
  {
    IPC_DEBUG(DBG_ERROR,"[ipc]: request_irq error\n");
    goto out_del;
  }
  
  /**
     Make sure this is not cache'd because CP has to know about any changes
     we write to this memory immediately.
   */
  IPC_DEBUG(DBG_INFO, "[ipc]: ioremap_nocache IPC_BASE\n");
  g_ipc_info.apcp_shmem = ioremap_nocache(IPC_BASE, IPC_SIZE);
  if (!g_ipc_info.apcp_shmem) 
  {
    rc = -ENOMEM;
    IPC_DEBUG(DBG_ERROR,"[ipc]: Could not map shmem\n");
    goto out_del;
  }
#ifdef CONFIG_HAS_WAKELOCK
  wake_lock_init(&ipc_wake_lock, WAKE_LOCK_SUSPEND, "ipc_wake_lock");
#endif

  IPC_DEBUG(DBG_INFO, "[ipc]: ipcs_init\n");
  if (ipcs_init((void *)g_ipc_info.apcp_shmem, IPC_SIZE))
  {
    rc = -1;
    IPC_DEBUG(DBG_ERROR,"[ipc]: ipcs_init() failed\n");
    goto out_del;
  }
  
  if ( sEarlyCPInterrupt )
  {
    IPC_DEBUG(DBG_INFO,"[ipc]: early CP interrupt - doing crash dump...\n");
#ifdef CONFIG_HAS_WAKELOCK
    wake_lock(&ipc_wake_lock);
#endif
    schedule_work(&g_ipc_info.cp_crash_dump_wq);
  }
  
  // check for AP only boot mode
  if ( AP_ONLY_BOOT == get_ap_boot_mode() )
  {
      IPC_DEBUG(DBG_INFO,"[ipc]: AP only boot - not waiting for CP\n");
  }
  else
  {
  // wait for CP to have IPC setup as well; if we exit module init
  // before IPC is ready, RPC module will likely crash during its 
  // own init
  startTime = current_kernel_time();
  while ( !g_ipc_info.ipc_state )
  {
    IPC_DEBUG(DBG_INFO, "[ipc]: CP IPC not ready, sleeping...\n");
    msleep(20);
    readyChkCnt++;
    if ( readyChkCnt > 100 )
    {
        IPC_DEBUG(DBG_ERROR, "[ipc]: IPC init timeout - no response from CP\n");
        rc = -1;
        goto out_del;
    }
  }
  endTime = current_kernel_time();
  IPC_DEBUG(DBG_INFO,"readyChkCnt=%d time=%ldus\n", readyChkCnt,
        ((endTime.tv_sec - startTime.tv_sec)*1000000L+(endTime.tv_nsec - startTime.tv_nsec)/1000L));

  IPC_DEBUG(DBG_INFO,"[ipc]: ipcs_module_init ok\n");
  }
    
  return 0;

out_del:
  cdev_del(&g_ipc_info.cdev);
out_unregister:
  unregister_chrdev_region(g_ipc_info.devnum, 1);
out:
  IPC_DEBUG(DBG_ERROR,"IPC Driver Failed to initialise!\n");
  return rc;
}

static void __exit ipcs_module_exit(void)
{
  flush_workqueue(g_ipc_info.intr_workqueue);
  destroy_workqueue(g_ipc_info.intr_workqueue);

  iounmap(g_ipc_info.apcp_shmem);

  free_irq(IRQ_IPC_C2A, &g_ipc_info);

  cdev_del(&g_ipc_info.cdev);

  unregister_chrdev_region(g_ipc_info.devnum, 1);
#ifdef CONFIG_HAS_WAKELOCK
  wake_lock_destroy(&ipc_wake_lock);
#endif

  return;
}

module_init(ipcs_module_init);
module_exit(ipcs_module_exit);
