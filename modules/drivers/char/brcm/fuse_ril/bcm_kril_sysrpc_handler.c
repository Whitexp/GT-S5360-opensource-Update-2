/****************************************************************************
*
*     Copyright (c) 2009 Broadcom Corporation
*
*   Unless you and Broadcom execute a separate written software license 
*   agreement governing use of this software, this software is licensed to you 
*   under the terms of the GNU General Public License version 2, available 
*    at http://www.gnu.org/licenses/old-licenses/gpl-2.0.html (the "GPL"). 
*
*   Notwithstanding the above, under no circumstances may you combine this 
*   software in any way with any other Broadcom software provided under a license 
*   other than the GPL, without Broadcom's express prior written consent.
*
****************************************************************************/
#include "bcm_kril_common.h"
#include "bcm_kril_sysrpc_handler.h"
#include "bcm_cp_cmd_handler.h"
#include "bcm_kril_ioctl.h"

#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>

#include "pmu.h"
#include "hal_pmu.h"
#include "sys_usb_rpc.h"
#include "hal_adc.h"
#include "hal_em_battmgr.h"
#include "meas_mgr.h"

#include "sysparm.h"
#include "sys_api.h"
#include "sys_common_rpc.h"
#include "sys_gen_rpc.h"
#include "sys_rpc.h"
#include "sysrpc_init.h"


extern void KRIL_Capi2HandleFlowCtrl(RPC_FlowCtrlEvent_t event, UInt8 channel);
extern HAL_EM_BATTMGR_ErrorCode_en_t _HAL_EM_BATTMGR_RegisterEventCB(
	HAL_EM_BATTMGR_Events_en_t event,		///< (in) Event type
	void *callback							///< (in) Callback routine
	);

// temporary for passing battery level events back to PMU; sysrpc will soon be 
// a standalone module, with a different interface for PMU
extern void PoressDeviceNotifyCallbackFun(UInt32 msgType, void* dataBuf, UInt32 dataLength);

static void HandleBattMgrEmptyEvent( void );
static void HandleBattMgrLowEvent( void );
static void HandleBattMgrLevelEvent( UInt16 level, UInt16 adc_avg, UInt16 total_levels );
static void HandleBattMgrExtremeTempEvent( EM_BATTMGR_ExtTempState_en_t temp_state,	 Int16 temp_in_C );

#define THREE_VOLTS_IN_MICRO_VOLTS 3000000
#define THREE_PT_THREE_VOLTS_IN_MICRO_VOLTS 3300000
#define THREE_PT_ONE_VOLTS_IN_MICRO_VOLTS 3100000
#define TWO_PT_FIVE_VOLTS_IN_MICRO_VOLTS 2500000
#define ONE_PT_EIGHT_VOLTS_IN_MICRO_VOLTS 1800000

static struct regulator* sim_regulator = NULL;

typedef struct
{
    struct list_head list;
    RPC_Msg_t* pMsg; 
    ResultDataBufHandle_t dataBufHandle;
} Kril_SysRpcInfo_t;

typedef struct 
{
    spinlock_t lock;
    struct work_struct sysRpcQ;
    struct workqueue_struct* sysRpc_wq;
    Kril_SysRpcInfo_t sysRpc_head;
} KRIL_SysRpcWq_t;

// work queue used to process CP->AP commands passed thru SysRpc channel
KRIL_SysRpcWq_t sKrilSysRpcWq;
// wake lock for SysRpc channel
#ifdef CONFIG_HAS_WAKELOCK
struct wake_lock sKrilSysRpcWakeLock;
#endif

// initialize sysrpc interface
void KRIL_SysRpc_Init( void )
{
    static int inited = 0;

    if ( !inited )
    {
        int ret;
        
        HAL_EM_BATTMGR_ErrorCode_en_t errCode;
        
        wake_lock_init(&sKrilSysRpcWakeLock, WAKE_LOCK_SUSPEND, "kril_sysrpc_wake_lock");

        // **FIXME** add this to actual module init? Need a way to shut it down as well...
        KRIL_DEBUG(DBG_ERROR,"calling regulator_get...\n");
        sim_regulator = regulator_get(NULL,"sim_vcc");
        if ( sim_regulator > 0 )
        {
            KRIL_DEBUG(DBG_ERROR," regulator_get OK\n");
        }
        else
        {
            KRIL_DEBUG(DBG_ERROR," **regulator_get FAILED %d\n", sim_regulator);
        }
        
        // **FIXME** workaround for SIM LDO being enabled on startup by the driver; this 
        // just bumps up our ref count in the sim_regulator struct, so that the 
        // regulator_disable() call below doesn't fail
        KRIL_DEBUG(DBG_INFO," calling regulator_enable...\n");
        ret = regulator_enable(sim_regulator);
        KRIL_DEBUG(DBG_INFO," regulator_enable returned %d\n",ret);

        KRIL_DEBUG(DBG_INFO," calling SYS_InitRpc\n");
        SYS_InitRpc();


        errCode = _HAL_EM_BATTMGR_RegisterEventCB( BATTMGR_EMPTY_BATT_EVENT, HandleBattMgrEmptyEvent );
        KRIL_DEBUG(DBG_INFO," reg empty rtnd %d\n", (int)errCode );

        errCode = _HAL_EM_BATTMGR_RegisterEventCB( BATTMGR_LOW_BATT_EVENT, HandleBattMgrLowEvent );
        KRIL_DEBUG(DBG_INFO," reg low rtnd %d\n", (int)errCode );

        errCode = _HAL_EM_BATTMGR_RegisterEventCB( BATTMGR_BATTLEVEL_CHANGE_EVENT, HandleBattMgrLevelEvent );
        KRIL_DEBUG(DBG_INFO," reg change rtnd %d\n", (int)errCode );

        errCode = _HAL_EM_BATTMGR_RegisterEventCB( BATTMGR_BATT_EXTREME_TEMP_EVENT , HandleBattMgrExtremeTempEvent ) ;
        KRIL_DEBUG(DBG_INFO," reg extreme temp rtnd %d\n", (int)errCode );

    }
}

// ******************************************************************************
/**	
*   Utility function to dispatch result message for FFS control requests from CP
*   back to CP.
*
*	@param inTid        (in) transaction id of original request from CP
*	@param inClientId	(in) client id of original request from CP
*	@param inFFSCtrlResult	(in) result of FFS control operation
*
********************************************************************************/
void KRIL_SysRpc_SendFFSControlRsp( UInt32 inTid, UInt8 inClientId, UInt32 inFFSCtrlResult )
{
    SYS_ReqRep_t data;
    RPC_Msg_t rsp;

    KRIL_DEBUG(DBG_INFO, "KRIL_SysRpc_SendFFSControlRsp: result 0x%x\r\n", inFFSCtrlResult );
    memset(&data, 0, sizeof(SYS_ReqRep_t));
    data.req_rep_u.CAPI2_CPPS_Control_Rsp.val = inFFSCtrlResult;
    data.result = RESULT_OK;

    rsp.msgId = MSG_CPPS_CONTROL_RSP;
    rsp.tid = inTid;
    rsp.clientID = inClientId;
    rsp.dataBuf = (void*)&data;
    rsp.dataLen = 0;

    RPC_SerializeRsp(&rsp);
}

// 
// Handlers for CP to AP requests
//
Result_t Handle_CAPI2_CPPS_Control(RPC_Msg_t* pReqMsg, UInt32 cmd, UInt32 address, UInt32 offset, UInt32 size)
{
    CAPI2_FFS_Control(pReqMsg->tid, pReqMsg->clientID, cmd, address, offset, size);
    
    return RESULT_OK;
}

static UInt32 PedestalModeAllowedByCP = 0;

Result_t Handle_CAPI2_CP2AP_PedestalMode_Control(RPC_Msg_t* pReqMsg, UInt32 enable)
{
    Result_t result = RESULT_OK;
	SYS_ReqRep_t data;
    KRIL_DEBUG(DBG_INFO," enter Handle_CAPI2_CP2AP_PedestalMode_Control\n");
    
	memset(&data, 0, sizeof(SYS_ReqRep_t));
	PedestalModeAllowedByCP = enable;

	data.result = result;
	Send_SYS_RspForRequest(pReqMsg, MSG_CP2AP_PEDESTALMODE_CONTROL_RSP, &data);
    return result;
}

static Boolean gSimInited = FALSE;

Result_t Handle_CAPI2_PMU_IsSIMReady(RPC_Msg_t* pReqMsg, PMU_SIMLDO_t simldo)
{
    Result_t result = RESULT_OK;
	SYS_ReqRep_t data;
    int ret=0;
    
    KRIL_DEBUG(DBG_ERROR," enter Handle_CAPI2_PMU_IsSIMReady\n");
    ret = regulator_is_enabled(sim_regulator);
    KRIL_DEBUG(DBG_ERROR," regulator_is_enabled return %d\n", ret);
    
	memset(&data, 0, sizeof(SYS_ReqRep_t));
	data.req_rep_u.CAPI2_PMU_IsSIMReady_Rsp.val = gSimInited; //(Boolean)(regulator_is_enabled > 0);

	data.result = result;
	Send_SYS_RspForRequest(pReqMsg, MSG_PMU_IS_SIM_READY_RSP, &data);

    KRIL_DEBUG(DBG_ERROR," Handle_CAPI2_PMU_IsSIMReady DONE %d\n", (int)gSimInited);
    return result;
}


Result_t Handle_CAPI2_PMU_ActivateSIM(RPC_Msg_t* pReqMsg, PMU_SIMLDO_t simldo, PMU_SIMVolt_t volt)
{
    Result_t result = RESULT_OK;
	SYS_ReqRep_t data;
    int simMicroVolts = 0;
    
    KRIL_DEBUG(DBG_ERROR," enter Handle_CAPI2_PMU_ActivateSIM\n");
    
    switch (volt)
    {
        case PMU_SIM3P0Volt:
        {
            KRIL_DEBUG(DBG_ERROR," PMU_SIM3P0Volt\n");
            // 3.0 V SIM
            simMicroVolts = THREE_VOLTS_IN_MICRO_VOLTS;
            break;
        }

#ifdef PMU_BCM59038
        case PMU_SIM3P3Volt:
        {
            KRIL_DEBUG(DBG_ERROR," PMU_SIM3P3Volt\n");
            // 3.3 V SIM
            simMicroVolts = THREE_PT_THREE_VOLTS_IN_MICRO_VOLTS;
            break;
        }
#endif // PMU_BCM59038

#ifdef PMU_MAX8986
		 case PMU_SIM3P1Volt:
		 {
			 KRIL_DEBUG(DBG_ERROR," PMU_SIM3P1Volt\n");
			 // 3.1 V SIM
			 simMicroVolts = THREE_PT_ONE_VOLTS_IN_MICRO_VOLTS;
			 break;
		 }
 #endif // PMU_MAX8986

        case PMU_SIM2P5Volt:
        {
            KRIL_DEBUG(DBG_ERROR," PMU_SIM2P5Volt\n");
            // 2.5 V SIM
            simMicroVolts = TWO_PT_FIVE_VOLTS_IN_MICRO_VOLTS;
            break;
        }
        
        case PMU_SIM1P8Volt:
        {
            KRIL_DEBUG(DBG_ERROR," PMU_SIM1P8Volt\n");
            // 1.8 V SIM
            simMicroVolts = ONE_PT_EIGHT_VOLTS_IN_MICRO_VOLTS;
            break;
        }
        
        case PMU_SIM0P0Volt:
        {
            int enabled = regulator_is_enabled(sim_regulator);
            KRIL_DEBUG(DBG_ERROR," ** PMU_SIM0P0Volt - regulator_is_enabled returned %d\n", enabled);
            
            simMicroVolts = 0;
            gSimInited = FALSE;
            if ( enabled > 0 )
            {
                int ret;
                KRIL_DEBUG(DBG_ERROR," ** PMU_SIM0P0Volt - turn off regulator (FORCE)\n");
                ret = regulator_disable(sim_regulator);
                KRIL_DEBUG(DBG_ERROR," regulator_disable returned 0x%x\n", ret);
 
            }
            else
            {
                //int ret;
                KRIL_DEBUG(DBG_ERROR," ** PMU_SIM0P0Volt - regulator not enabled, do nothing...\n");
                //ret = regulator_enable(sim_regulator);
                //KRIL_DEBUG(DBG_ERROR," regulator_enable returned 0x%x\n", ret);
                //ret = regulator_disable(sim_regulator);
                //KRIL_DEBUG(DBG_ERROR," regulator_disable returned 0x%x\n", ret);
            }
           break;
        }
        
        default:
        {
            KRIL_DEBUG(DBG_ERROR," unrecognized value for volt %d\n", (int)volt);
            break;
        }
            
    }
    
	memset(&data, 0, sizeof(SYS_ReqRep_t));
	
	if ( simMicroVolts > 0 )
	{
        int ret = regulator_set_voltage(sim_regulator,simMicroVolts,simMicroVolts);
        KRIL_DEBUG(DBG_ERROR," regulator_set_voltage returned %d\n", ret);
        if ( regulator_is_enabled(sim_regulator) > 0 )
        {
            KRIL_DEBUG(DBG_ERROR," regulator already enabled \n");
        }
        else
        {
            ret = regulator_enable(sim_regulator);
            KRIL_DEBUG(DBG_ERROR," regulator_enable returned %d\n", ret);
        }
        gSimInited = TRUE;
	}
	//PMU_ActivateSIM(volt);

	data.result = result;
	Send_SYS_RspForRequest(pReqMsg, MSG_PMU_ACTIVATE_SIM_RSP, &data);

    return result;
}

Result_t Handle_CAPI2_PMU_ClientPowerDown(RPC_Msg_t* pReqMsg)
{
	Result_t result = RESULT_OK;
	SYS_ReqRep_t data;
    KRIL_DEBUG(DBG_ERROR," enter Handle_CAPI2_PMU_ClientPowerDown\n");

	memset(&data, 0, sizeof(SYS_ReqRep_t));
	// **FIXME** check with Shameer for equivalent in Android
	//PMU_ClientPowerDown();

	data.result = result;
	Send_SYS_RspForRequest(pReqMsg, MSG_PMU_ClientPowerDown_RSP, &data);
	return result;
}

// **FIXME** implement this
Result_t Handle_CAPI2_FLASH_SaveImage(RPC_Msg_t* pReqMsg, UInt32 flash_addr, UInt32 length, UInt32 shared_mem_addr)
{
	Result_t result = RESULT_OK;
	SYS_ReqRep_t data;
    KRIL_DEBUG(DBG_ERROR," enter Handle_CAPI2_FLASH_SaveImage\n");

	memset(&data, 0, sizeof(SYS_ReqRep_t));
	data.req_rep_u.CAPI2_FLASH_SaveImage_Rsp.val = TRUE; //(Boolean)FlashSaveData(flash_addr,length,(UInt8*)shared_mem_addr);

	data.result = result;
	Send_SYS_RspForRequest(pReqMsg, MSG_FLASH_SAVEIMAGE_RSP, &data);
	return result;
}

Result_t Handle_CAPI2_USB_IpcMsg(RPC_Msg_t* pReqMsg, USBPayload_t *val)
{
    Result_t result = RESULT_OK;
    KRIL_DEBUG(DBG_ERROR," enter Handle_CAPI2_USB_IpcMsg\n");
   
    return result;
}

Result_t SysUsbRpc_SendReq( UInt8 clientID, UInt32 tid, InterTaskMsg_t *InMsg)
{
    return RESULT_OK;
}

static void HandleBattMgrEmptyEvent( void )
{
    HAL_EM_BatteryLevel_t battLevel;

    KRIL_DEBUG(DBG_INFO," *** HandleBattMgrEmptyEvent\n");
    battLevel.eventType = BATTMGR_EMPTY_BATT_EVENT;
    battLevel.inLevel = 0;
    battLevel.inAdc_avg = 0;
    battLevel.inTotal_levels = 0;
    // dispatch back to PMU if it is registered
    PoressDeviceNotifyCallbackFun(RIL_NOTIFY_DEVSPECIFIC_BATT_LEVEL, &battLevel, sizeof(HAL_EM_BatteryLevel_t));
}
static void HandleBattMgrLowEvent( void )
{
    HAL_EM_BatteryLevel_t battLevel;

    KRIL_DEBUG(DBG_INFO," *** HandleBattMgrLowEvent\n");
    battLevel.eventType = BATTMGR_LOW_BATT_EVENT;
    battLevel.inLevel = 0;
    battLevel.inAdc_avg = 0;
    battLevel.inTotal_levels = 0;
    // dispatch back to PMU if it is registered
    PoressDeviceNotifyCallbackFun(RIL_NOTIFY_DEVSPECIFIC_BATT_LEVEL, &battLevel, sizeof(HAL_EM_BatteryLevel_t));
}

static void HandleBattMgrLevelEvent( UInt16 level, UInt16 adc_avg, UInt16 total_levels )
{
    HAL_EM_BatteryLevel_t battLevel;

    KRIL_DEBUG(DBG_INFO," *** HandleBattMgrLevelEvent lvl:%d adc:%d total_levels:%d\n", level,adc_avg,total_levels);
    battLevel.eventType = BATTMGR_BATTLEVEL_CHANGE_EVENT;
    battLevel.inLevel = level;
    battLevel.inAdc_avg = adc_avg;
    battLevel.inTotal_levels = total_levels;
    // dispatch back to PMU if it is registered
    PoressDeviceNotifyCallbackFun(RIL_NOTIFY_DEVSPECIFIC_BATT_LEVEL, &battLevel, sizeof(HAL_EM_BatteryLevel_t));
}

static void HandleBattMgrExtremeTempEvent( EM_BATTMGR_ExtTempState_en_t temp_state, Int16 temp_in_C )
{
    HAL_EM_BatteryLevel_t battLevel;

    KRIL_DEBUG(DBG_INFO," *** HandleBattMgrExtremeTempEvent state:%d temp(C):%d\n", temp_state, temp_in_C );
    battLevel.eventType = BATTMGR_BATT_EXTREME_TEMP_EVENT;
    battLevel.inLevel = temp_state;
    battLevel.inAdc_avg = temp_in_C;
    // dispatch back to PMU if it is registered
    PoressDeviceNotifyCallbackFun(RIL_NOTIFY_DEVSPECIFIC_BATT_LEVEL, &battLevel, sizeof(HAL_EM_BatteryLevel_t));
}


// stub for sysrpc
void SYS_SyncTaskMsg( void )
{
//    Boolean ret = RPC_SetProperty(RPC_PROP_AP_TASKMSGS_READY, 1);
    KRIL_DEBUG(DBG_INFO," SYS_SyncTaskMsg - ignoring\n");
}

