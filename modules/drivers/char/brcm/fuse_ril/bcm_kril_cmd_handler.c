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
#include "bcm_kril_cmd_handler.h"
#include "bcm_kril_ioctl.h"
#include "bcm_kril_capi2_handler.h"
#include "bcm_cp_cmd_handler.h"
#include "brcm_urilc_cmd.h"

#ifdef OEM_RIL_ENABLE
#include "k_sril_handler.h"
#endif

static UInt8  g_ClientID = 0;
static UInt32 g_CAPI2TID = 0;

KRIL_CmdQueue_t gKrilCmdQueue;
KRIL_RespWq_t gKrilRespWq;
KRIL_NotifyWq_t gKrilNotifyWq;
KRIL_CmdList_t gKrilCmdList;
extern struct completion gCmdThreadExited;

extern KRIL_ResultQueue_t gKrilResultQueue;
extern KRIL_Param_t gKrilParam;

extern bcm_kril_dev_result_t bcm_dev_results[TOTAL_BCMDEVICE_NUM];
extern int g_kril_initialised;

// wake lock
#ifdef CONFIG_HAS_WAKELOCK
extern struct wake_lock kril_rsp_wake_lock;
extern struct wake_lock kril_notify_wake_lock;
extern struct wake_lock kril_result_wake_lock;
#endif
// for debug log switch flag
#ifdef CONFIG_BRCM_UNIFIED_LOGGING
static Boolean gEnable = TRUE;
#else
static Boolean gEnable = FALSE;
#endif

// for call
static UInt8 sIncomingCallIndex = INVALID_CALL;
static UInt8 sWaitingCallIndex = INVALID_CALL;
static int sCallType[BCM_MAX_CALLS_NO];
static BRIL_CallState sCallState[BCM_MAX_CALLS_NO];
static PresentationInd_t sCallNumPresent[BCM_MAX_CALLS_NO] ={CC_PRESENTATION_ALLOWED}; // default to set presentation allowed
static Boolean sInCallHandler = FALSE;
static Boolean sIsMakeCall = FALSE;
/* The last voice call failure cause. */
static BRIL_LastCallFailCause sLastCallFailCause = BCM_CALL_FAIL_NORMAL;
static UInt32  sMPTYTID = 0;

// for Network
static Boolean sInSetPrefNetworkTypeHandler = FALSE;
static int  sPreferredNetworkType = 0xFF;
static int  sBandMode = 0x0;           // default to auto
static int  sLocationUpdateStatus = 1; //+CREG: 2 enable by default
static int  sRestrictedState = 0xFF;
static UInt32 sServingCellTID = 0;
static Boolean sInNeighborCellHandler = FALSE;
static UInt32 sNetworkSelectTID = 0;
static Boolean sInNetworkSelectHandler = FALSE;
static Boolean sInSetupPDPHandler = FALSE;
static Boolean sNeedSetPreferredNetworkType = FALSE;

/*+20110418 BCOM PATCH FOR DebugScreen*/
static UInt32 sMeasureReportTID = 0;
static Boolean sInMeasureReportHandler = FALSE;
/*-20110418 BCOM PATCH FOR DebugScreen*/
static Boolean sHandling2GOnlyRequest = FALSE;
// for SS
static int sLastCLIP = 1;
static Boolean sInSsQueryHandler = FALSE;
static int sServiceClass = 0;

// for SIM
static SIM_APPL_TYPE_t  sSimAppType = SIM_APPL_INVALID;

// for SMS 
static SmsMti_t  sSmsMti;
static Boolean sIsClass2SMS = FALSE;
static Boolean sInSMSHandler = FALSE;
static Int8 sSendSMSNumber = 0;
static Int8 sUpdateSMSNumber = 0;
static UInt32 sWriteSMSToSIMTID = 0;

// for SMS info
static SIMSMSMesgStatus_t *sSMSArray;
static UInt8 sTotalSMSInSIM = 0;
static Boolean sIsSMSMEAvailable = TRUE;

kril_capi2_handler_fn_t g_kril_capi2_handler_array[]=
{
     {0, NULL}, //none
     {BRIL_REQUEST_KRIL_INIT, KRIL_InitCmdHandler, 0},
     {KRIL_REQUEST_QUERY_SMS_IN_SIM, KRIL_QuerySMSInSIMHandler, sizeof(UInt8)},
     {KRIL_REQUEST_QUERY_SIM_EMERGENCY_NUMBER, KRIL_QuerySimEmergencyNumberHandler, 0},
     {BRCM_RIL_REQUEST_GET_SIM_STATUS, KRIL_GetSimStatusHandler, 0},
     {BRCM_RIL_REQUEST_ENTER_SIM_PIN, KRIL_EnterSimPinHandler, 0},
     {BRCM_RIL_REQUEST_ENTER_SIM_PUK, KRIL_EnterSimPukHandler, 0},
     {BRCM_RIL_REQUEST_ENTER_SIM_PIN2, KRIL_EnterSimPinHandler, 0},
     {BRCM_RIL_REQUEST_ENTER_SIM_PUK2, KRIL_EnterSimPukHandler, 0},
     {BRCM_RIL_REQUEST_CHANGE_SIM_PIN, KRIL_ChangeSimPinHandler, 0},
     {BRCM_RIL_REQUEST_CHANGE_SIM_PIN2, KRIL_ChangeSimPinHandler, 0},
     {BRCM_RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION, KRIL_EnterNetworkDepersonHandler, 0},
     {BRCM_RIL_REQUEST_GET_CURRENT_CALLS, KRIL_GetCurrentCallHandler, 0},
     {BRCM_RIL_REQUEST_DIAL, KRIL_DialHandler, sizeof(KrilCallRetryInfo_t)},
     {BRCM_RIL_REQUEST_GET_IMSI, KRIL_GetIMSIHandler, 0},
     {BRCM_RIL_REQUEST_HANGUP, KRIL_HungupHandler, 0},
     {BRCM_RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND, KRIL_HungupWaitingOrBackgroundHandler, 0},
     {BRCM_RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND, KRIL_HungupForegroundResumeBackgroundHandler, 0},
     {BRCM_RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE, KRIL_SwitchWaitingOrHoldingAndActiveHandler, sizeof(KrilCallIndex_t)},
     {BRCM_RIL_REQUEST_CONFERENCE, KRIL_ConferenceHandler, 0},
     {BRCM_RIL_REQUEST_UDUB, KRIL_UDUBHandler, 0},
     {BRCM_RIL_REQUEST_LAST_CALL_FAIL_CAUSE, KRIL_LastCallFailCauseHandler, 0},
     {BRCM_RIL_REQUEST_SIGNAL_STRENGTH, KRIL_SignalStrengthHandler, 0},
     {BRCM_RIL_REQUEST_REGISTRATION_STATE, KRIL_RegistationStateHandler, 0},
     {BRCM_RIL_REQUEST_GPRS_REGISTRATION_STATE, KRIL_RegistationStateHandler, 0},
     {BRCM_RIL_REQUEST_OPERATOR, KRIL_OperatorHandler, 0},
     {BRCM_RIL_REQUEST_RADIO_POWER, KRIL_RadioPowerHandler, 0},
     {BRCM_RIL_REQUEST_DTMF, KRIL_SendDTMFRequestHandler, sizeof(KrilDTMFInfo_t)},
     {BRCM_RIL_REQUEST_SEND_SMS, KRIL_SendSMSHandler, 0},
     {BRCM_RIL_REQUEST_SEND_SMS_EXPECT_MORE, KRIL_SendSMSExpectMoreHandler, 0},
     {BRCM_RIL_REQUEST_SETUP_DATA_CALL, KRIL_SetupPdpHandler, 0},
     {BRCM_RIL_REQUEST_SIM_IO, KRIL_SimIOHandler, 0},
     {BRCM_RIL_REQUEST_SEND_USSD, KRIL_SendUSSDHandler, 0},
     {BRCM_RIL_REQUEST_CANCEL_USSD, KRIL_CancelUSSDHandler, 0},
     {BRCM_RIL_REQUEST_GET_CLIR, KRIL_GetCLIRHandler, 0},
     {BRCM_RIL_REQUEST_SET_CLIR, KRIL_SetCLIRHandler, 0},
     {BRCM_RIL_REQUEST_QUERY_CALL_FORWARD_STATUS, KRIL_QueryCallForwardStatusHandler, 0},
     {BRCM_RIL_REQUEST_SET_CALL_FORWARD, KRIL_SetCallForwardStatusHandler, 0},
     {BRCM_RIL_REQUEST_QUERY_CALL_WAITING, KRIL_QueryCallWaitingHandler, 0},
     {BRCM_RIL_REQUEST_SET_CALL_WAITING, KRIL_SetCallWaitingHandler, 0},
     {BRCM_RIL_REQUEST_SMS_ACKNOWLEDGE, KRIL_SMSAcknowledgeHandler, 0},
     {BRCM_RIL_REQUEST_GET_IMEI, KRIL_GetIMEIHandler, 0},
     {BRCM_RIL_REQUEST_GET_IMEISV, KRIL_GetIMEISVHandler, 0},
     {BRCM_RIL_REQUEST_ANSWER, KRIL_AnswerHandler, sizeof(UInt8)},
     {BRCM_RIL_REQUEST_DEACTIVATE_DATA_CALL, KRIL_DeactivatePdpHandler, 0},
     {BRCM_RIL_REQUEST_QUERY_FACILITY_LOCK, KRIL_QueryFacilityLockHandler, 0},
     {BRCM_RIL_REQUEST_SET_FACILITY_LOCK, KRIL_SetFacilityLockHandler, 0},
     {BRCM_RIL_REQUEST_CHANGE_BARRING_PASSWORD, KRIL_ChangeBarringPasswordHandler, 0},
     {BRCM_RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE, KRIL_QueryNetworkSelectionModeHandler, 0},
     {BRCM_RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC, KRIL_SetNetworkSelectionAutomaticHandler, 0},
     {BRCM_RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL, KRIL_SetNetworkSelectionManualHandler, 0},
     {BRCM_RIL_REQUEST_QUERY_AVAILABLE_NETWORKS , KRIL_QueryAvailableNetworksHandler, 0},
     {BRCM_RIL_REQUEST_DTMF_START, KRIL_SendDTMFStartHandler, sizeof(KrilDTMFInfo_t)},
     {BRCM_RIL_REQUEST_DTMF_STOP, KRIL_SendDTMFStopHandler,  sizeof(KrilDTMFInfo_t)},
     {BRCM_RIL_REQUEST_BASEBAND_VERSION, KRIL_BasebandVersionHandler, 0},
     {BRCM_RIL_REQUEST_SEPARATE_CONNECTION, KRIL_SeparateConnectionHandler, 0},
     {BRCM_RIL_REQUEST_SET_MUTE, NULL, 0},
     {BRCM_RIL_REQUEST_GET_MUTE, NULL, 0},
     {BRCM_RIL_REQUEST_QUERY_CLIP, KRIL_QueryCLIPHandler, 0},
     {BRCM_RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE, NULL, 0},
     {BRCM_RIL_REQUEST_DATA_CALL_LIST, NULL, 0},
     {BRCM_RIL_REQUEST_RESET_RADIO, NULL, 0},
     {BRCM_RIL_REQUEST_OEM_HOOK_RAW, KRIL_OemHookRawHandler, 0},
     {BRCM_RIL_REQUEST_OEM_HOOK_STRINGS, NULL, 0},
     {BRCM_RIL_REQUEST_SCREEN_STATE, NULL, 0},
     {BRCM_RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION, KRIL_SetSuppSvcNotificationHandler, 0},
     {BRCM_RIL_REQUEST_WRITE_SMS_TO_SIM, KRIL_WriteSMSToSIMHandler, 0},
     {BRCM_RIL_REQUEST_DELETE_SMS_ON_SIM, KRIL_DeleteSMSOnSIMHandler, 0},
     {BRCM_RIL_REQUEST_SET_BAND_MODE, KRIL_SetBandModeHandler, 0},
     {BRCM_RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE, KRIL_QueryAvailableBandModeHandler, 0},
     {BRCM_RIL_REQUEST_STK_GET_PROFILE, KRIL_StkGetProfile, 0},
     {BRCM_RIL_REQUEST_STK_SET_PROFILE, KRIL_StkSetProfile, 0}, 
     {BRCM_RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING, KRIL_StkService_Running, 0}, // gearn fix java sim card 
     {BRCM_RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND, KRIL_StkSendEnvelopeCmdHandler, 0},
     {BRCM_RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE, KRIL_StkSendTerminalRspHandler, 0},
     {BRCM_RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM, KRIL_StkHandleCallSetupRequestedHandler, 0},
     {BRCM_RIL_REQUEST_EXPLICIT_CALL_TRANSFER, KRIL_ExplicitCallTransferHandler, 0},
     {BRCM_RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE, KRIL_SetPreferredNetworkTypeHandler, sizeof(UInt8)},
     {BRCM_RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE, KRIL_GetPreferredNetworkTypeHandler, 0},
     {BRCM_RIL_REQUEST_GET_NEIGHBORING_CELL_IDS, KRIL_GetNeighboringCellIDsHandler, 0},
     {BRCM_RIL_REQUEST_SET_LOCATION_UPDATES, KRIL_SetLocationUpdatesHandler, 0},
     {BRCM_RIL_REQUEST_SET_TTY_MODE, KRIL_SetTTYModeHandler, 0},
     {BRCM_RIL_REQUEST_QUERY_TTY_MODE, KRIL_QueryTTYModeHandler, 0},
     {BRCM_RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG, KRIL_GetBroadcastSmsHandler, sizeof(UInt8)},
     {BRCM_RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG, KRIL_SetBroadcastSmsHandler, 0},
     {BRCM_RIL_REQUEST_GSM_SMS_BROADCAST_ACTIVATION, KRIL_SmsBroadcastActivationHandler, 0},
     {BRCM_RIL_REQUEST_DEVICE_IDENTITY, KRIL_GetDeviceIdentityHandler, 0},
     {BRCM_RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE, NULL, 0},
     {BRCM_RIL_REQUEST_GET_SMSC_ADDRESS, KRIL_GetSMSCAddressHandler, 0},
     {BRCM_RIL_REQUEST_SET_SMSC_ADDRESS, KRIL_SetSMSCAddressHandler, 0},
     {BRCM_RIL_REQUEST_REPORT_SMS_MEMORY_STATUS, KRIL_ReportSMSMemoryStatusHandler, 0},
     {BRCM_RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING, NULL, 0},
     {URILC_REQUEST_DATA_STATE, KRIL_DataStateHandler, 0},
     {URILC_REQUEST_SEND_DATA, KRIL_SendDataHandler, 0},
#ifdef OEM_RIL_ENABLE
	{RIL_REQUEST_SET_CELL_BROADCAST_CONFIG, KRIL_SRIL_SetCellBroadcastHandler, 0},
	{RIL_REQUEST_GET_CELL_BROADCAST_CONFIG, KRIL_SRIL_GetCellBroadcastHandler, 0},
	{RIL_REQUEST_SEND_ENCODED_USSD, KRIL_SRIL_SendEncodeUSSDHandler/*KRIL_SendUSSDHandler*/, 0},
	{RIL_REQUEST_GET_PHONEBOOK_STORAGE_INFO, KRIL_SRIL_PhoneBookStorageInfoHandler, 0},
	{RIL_REQUEST_GET_PHONEBOOK_ENTRY, KRIL_SRIL_GetPhoneBookEntryHandler, 0},
	{RIL_REQUEST_ACCESS_PHONEBOOK_ENTRY, KRIL_SRIL_AccessPhoneBookEnteryHandler, 0},
	{RIL_REQUEST_DIAL_VIDEO_CALL, KRIL_SRIL_DialVideoCallHandler, 0},
	{RIL_REQUEST_CALL_DEFLECTION, KRIL_SRIL_CallDeflectionHandler, 0},
	{RIL_REQUEST_READ_SMS_FROM_SIM, KRIL_SRIL_ReadSMSFromSIMHandler, 0},
	{RIL_REQUEST_USIM_PB_CAPA, KRIL_SRIL_USIM_PB_CAPAHandler, 0},
	{RIL_REQUEST_LOCK_INFO, KRIL_SRIL_LockInfoHandler, 0},
	{RIL_REQUEST_DIAL_EMERGENCY_CALL, KRIL_DialHandler/*KRIL_SRIL_DialEmergencyHandler*/, 
			 sizeof(KrilCallRetryInfo_t)},
	{RIL_REQUEST_GET_STOREAD_MSG_COUNT, KRIL_SRIL_GetStorageMSGCountHandler, 0},
#endif   
#ifdef VIDEO_TELEPHONY_ENABLE
     {BRCM_RIL_REQUEST_DIAL_VT, KRIL_DialHandler, sizeof(KrilCallRetryInfo_t)},
     {BRCM_RIL_REQUEST_ANSWER_VT, KRIL_AnswerHandler, sizeof(UInt8)},
     {BRCM_RIL_REQUEST_HANGUP_VT, KRIL_HungupHandler, 0},
#endif //VIDEO_TELEPHONY_ENABLE
#ifdef BRCM_AGPS_CONTROL_PLANE_ENABLE
	 {RIL_REQUEST_AGPS_SEND_UPLINK, KRIL_AgpsSendUpLinkHandler, 0},
	 {RIL_REQUEST_AGPS_RRC_MEAS_CTRL_FAILURE, KRIL_AgpsRrcMcFailureHandler, 0},
	 {RIL_REQUEST_AGPS_RRC_STATUS, KRIL_AgpsRrcStatusHandler, 0},
#endif
	 {URILC_REQUEST_GET_SMS_SIM_MAX_CAPACITY, KRIL_GetSmsSimMaxCapacityHandler, 0},
	 {URILC_REQUEST_READ_SMS_ON_SIM, KRIL_ReadSmsOnSimHandler, 0},
	 {URILC_REQUEST_LIST_SMS_ON_SIM, KRIL_ListSmsOnSimHandler, sizeof(UInt8)},
	 {URILC_REQUEST_SEND_STORED_SMS, KRIL_SendStoredSmsHandler, 0},
	 {URILC_REQUEST_GET_ELEM_CSCS, KRIL_GetElemCscsHandler, 0},
	 {URILC_REQUEST_GET_SMS_ELEM_MORE_MSG_TO_SEND, KRIL_GetElemMoreMsgToSendHandler, 0},
	 {URILC_REQUEST_SET_SMS_ELEM_MORE_MSG_TO_SEND, KRIL_SetElemMoreMsgToSendHandler, 0},
	 {URILC_REQUEST_START_MULTI_SMS_TX_REQ, KRIL_StartMultiSmsTxHandler, 0},
	 {URILC_REQUEST_STOP_MULTI_SMS_TX_REQ, KRIL_StopMultiSmsTxHandler, 0}
};

//******************************************************************************
/**
*  Function to get the new TID
*
*  @param    None
*
*  @return   Valid TID number between BCM_TID_INIT and BCM_TID_MAX.
*
*  @note
*  Get a new TID number for the CAPI2 funciton in this handler
*  
*******************************************************************************/
UInt32 GetNewTID(void)
{
    if(g_CAPI2TID > BCM_TID_MAX) 
    {
        g_CAPI2TID = BCM_TID_INIT;
    }
    g_CAPI2TID++;
    KRIL_DEBUG(DBG_TRACE, "g_CAPI2TID:%lu\n", g_CAPI2TID);

    return g_CAPI2TID;
}

//******************************************************************************
/**
*  Function to get the TID
*
*  @param    None
*
*  @return   Valid TID number between BCM_TID_INIT and BCM_TID_MAX.
*
*  @note
*  Get the TID number for the CAPI2 funciton in this handler
*  
*******************************************************************************/
UInt32 GetTID(void)
{
    return g_CAPI2TID;
}

 //******************************************************************************
 /**
 *  Function to set the Client ID
 *
 *  @param    ClientID (in) Get client id from CAPI2 client
 *
 *  @return   None
 *
 *  @note
 *  Store the Client ID that from CAPI2
 *  
 *******************************************************************************/
void SetClientID(UInt8 ClientID)
{
    g_ClientID = ClientID;
}

//******************************************************************************
/**
*  Function to get the Client ID
*
*  @param    None
*
*  @return   Vaid the Client ID
*
*  @note
*  Get the Client ID
*  
*******************************************************************************/
UInt32 GetClientID(void)
{
    return g_ClientID;
}


//******************************************************************************
/**
*  Function to Initialition all parameter
*
*  @param    data (in) Does not to use
*
*  @return   0 for success 1 for failure
*
*  @note
*  Initialition all parameter.
*  
*******************************************************************************/
void KRIL_InitHandler(void)
{
    KRIL_DEBUG(DBG_TRACE, "Enter!\n");
    /* Init head list */
    INIT_LIST_HEAD(&gKrilCmdQueue.list);
    INIT_LIST_HEAD(&gKrilRespWq.capi2_head.list);
    INIT_LIST_HEAD(&gKrilNotifyWq.capi2_head.list);
    INIT_LIST_HEAD(&gKrilCmdList.list);

    /* Init work queue */
    INIT_WORK(&gKrilCmdQueue.commandq, KRIL_CommandThread);
    gKrilCmdQueue.cmd_wq = create_workqueue("cmd_wq");

    INIT_WORK(&gKrilRespWq.responseq, KRIL_ResponseHandler);
    gKrilRespWq.rsp_wq = create_workqueue("rsp_wq");
    INIT_WORK(&gKrilNotifyWq.notifyq, KRIL_NotifyHandler);
    gKrilNotifyWq.notify_wq = create_workqueue("notify_wq");
    
    /* Init lock section */
    mutex_init(&gKrilCmdList.mutex);
    mutex_init(&gKrilCmdQueue.mutex);
    spin_lock_init(&gKrilRespWq.lock);
    spin_lock_init(&gKrilNotifyWq.lock);

    g_kril_initialised = 1;

} // KRIL_InitHandler

//******************************************************************************
/**
*  Function to send the response data
*
*  @param    listentry (in)
*
*  @return   None
*
*  @note
*  Send response data in result queue.
*  
*******************************************************************************/
void KRIL_SendResponse(KRIL_CmdList_t *listentry)
{
    UInt32  flags;
    KRIL_ResultQueue_t *entry = kmalloc(sizeof(KRIL_ResultQueue_t), GFP_KERNEL);

    entry->result_info.client = listentry->ril_cmd->client;
    entry->result_info.t = listentry->ril_cmd->t;
    entry->result_info.CmdID = listentry->ril_cmd->CmdID;
    entry->result_info.datalen = listentry->rsp_len;
    if(BCM_ErrorCAPI2Cmd == listentry->handler_state)
    {
        entry->result_info.result = listentry->result; // Assign the error result
    }
    else
    {
        if(BCM_E_GENERIC_FAILURE == listentry->result) // If the result is default value, set the result is BCM_E_SUCCESS 
            entry->result_info.result = BCM_E_SUCCESS;
        else
            entry->result_info.result = listentry->result; // Assign the result
    }
    KRIL_DEBUG(DBG_INFO, "entry result:%d CmdID:%ld datalen:%d\n", entry->result_info.result, entry->result_info.CmdID, entry->result_info.datalen);

    if (0 == entry->result_info.datalen)
    {
        entry->result_info.data = NULL;
    }
    else
    {
        entry->result_info.data = kmalloc(entry->result_info.datalen, GFP_KERNEL);
        memcpy(entry->result_info.data, listentry->bcm_ril_rsp, entry->result_info.datalen);
    }
    #ifdef CONFIG_HAS_WAKELOCK
    wake_lock(&kril_result_wake_lock);
    #endif
    spin_lock_irqsave(&(gKrilParam.recv_lock), flags);
    list_add_tail(&entry->list, &(gKrilResultQueue.list));
    spin_unlock_irqrestore(&(gKrilParam.recv_lock), flags);
    wake_up_interruptible(&gKrilParam.read_wait);
}

//******************************************************************************
/**
*  Function to Send notify data
*
*  @param    CmdID (in)
*  @param    rsp_data (in)
*  @param    rsp_len (in)
*
*  @return   None
*
*  @note
*  Send notify data in result queue
*  
*******************************************************************************/
void KRIL_SendNotify(int CmdID, void *rsp_data, UInt32 rsp_len)
{
    UInt32  flags;
    KRIL_ResultQueue_t *entry = kmalloc(sizeof(KRIL_ResultQueue_t), GFP_KERNEL);

    entry->result_info.client = 0xFF;
    entry->result_info.t = NULL;
    entry->result_info.CmdID = CmdID;
    entry->result_info.datalen = rsp_len;
    KRIL_DEBUG(DBG_INFO, "client:%d CmdID:%lu\n", entry->result_info.client, entry->result_info.CmdID);
    KRIL_DEBUG(DBG_INFO, "notify_len:%ld CmdID:%d\n", rsp_len, CmdID);
    if (0 == entry->result_info.datalen)
    {
        entry->result_info.data = NULL;
    }
    else
    {
        entry->result_info.data = kmalloc(entry->result_info.datalen, GFP_KERNEL);
        memcpy(entry->result_info.data, rsp_data, entry->result_info.datalen);
    }
    #ifdef CONFIG_HAS_WAKELOCK
    wake_lock(&kril_result_wake_lock);
    #endif
    spin_lock_irqsave(&(gKrilParam.recv_lock), flags);
    list_add_tail(&entry->list, &(gKrilResultQueue.list));
    spin_unlock_irqrestore(&(gKrilParam.recv_lock), flags);
    wake_up_interruptible(&gKrilParam.read_wait);
}

//******************************************************************************
/**
* Function Name: KRIL_CommandThread
*
* Description  : Worker thread to receive data from command queue.
*
* PARAMETERS   : void
*
* RETURN       : void
* Notes:
*
*******************************************************************************/
void KRIL_CommandThread(struct work_struct *data)
{
    struct list_head *listptr, *pos;
    KRIL_CmdQueue_t *entry = NULL;
    KRIL_CmdList_t *cmd_list = NULL;
    bool found = FALSE;
    int i = 0;

    KRIL_DEBUG(DBG_TRACE2, "Enter...!\n");
    mutex_lock(&gKrilCmdQueue.mutex);
    list_for_each_safe(listptr, pos, &gKrilCmdQueue.list)
    {
        entry = list_entry(listptr, KRIL_CmdQueue_t, list);
        KRIL_DEBUG(DBG_TRACE, "entry cmd:%ld list:%p next:%p prev:%p\n", entry->cmd, &entry->list, entry->list.next, entry->list.prev);
        mutex_unlock(&gKrilCmdQueue.mutex);

        if(TRUE == IsNeedToWait(entry->ril_cmd->CmdID))
        {
            continue;
        }
        // add cmd list
        cmd_list = kmalloc(sizeof(KRIL_CmdList_t), GFP_KERNEL);
        memset(cmd_list, 0, sizeof(KRIL_CmdList_t));
        cmd_list->cmd = entry->cmd;
        cmd_list->ril_cmd = entry->ril_cmd;
        cmd_list->bcm_ril_rsp = NULL;
        cmd_list->rsp_len = 0;
        cmd_list->cmdContext = NULL;
        cmd_list->result = BCM_E_GENERIC_FAILURE; // default error code is BCM_E_GENERIC_FAILURE
        cmd_list->handler_state = BCM_SendCAPI2Cmd;
        KRIL_DEBUG(DBG_TRACE2, "cmd list cmd:%ld list:%p next:%p prev:%p\n", cmd_list->cmd, &cmd_list->list, cmd_list->list.next, cmd_list->list.prev);
        mutex_lock(&gKrilCmdList.mutex);
        list_add_tail(&cmd_list->list, &gKrilCmdList.list);
        mutex_unlock(&gKrilCmdList.mutex);

        KRIL_DEBUG(DBG_TRACE, "client:%d CmdID:%ld RIL_Token:0x%lx\n", cmd_list->ril_cmd->client, cmd_list->ril_cmd->CmdID, (long unsigned int)cmd_list->ril_cmd->t);
        // search function
        for (i = 0; i < ARRAY_SIZE(g_kril_capi2_handler_array); ++i)
        {
            //KRIL_DEBUG(DBG_TRACE,"Current command is0x%lx",g_kril_capi2_handler_array[i].cmd);
            //KRIL_SendResponse(cmd_list);
            if (g_kril_capi2_handler_array[i].cmd == entry->ril_cmd->CmdID)
            {
                KRIL_DEBUG(DBG_TRACE, "match command success 0x%lx, msgid %d\n", entry->cmd, i);
                if(g_kril_capi2_handler_array[i].capi2_handler != NULL)
                {
                    if (g_kril_capi2_handler_array[i].contextSize != 0)
                    {
                        KRIL_DEBUG(DBG_INFO, "contextSize %d\n", g_kril_capi2_handler_array[i].contextSize);
                        cmd_list->cmdContext = kmalloc(g_kril_capi2_handler_array[i].contextSize, GFP_KERNEL);
                        memset(cmd_list->cmdContext, 0, g_kril_capi2_handler_array[i].contextSize);
                    }
                    found = TRUE;
                    break;
                }
                else
                {
                    KRIL_DEBUG(DBG_ERROR, "The function does not implement yet or not in Capi2 handler array ...!\n");
                }
            }
         }
        //KRIL_DEBUG(DBG_ERROR, "Iterations are over...!\n");
        //KRIL_SendResponse(cmd_list);
        if(TRUE == found)
        {
            mutex_lock(&gKrilCmdList.mutex);
            KRIL_DEBUG(DBG_INFO, "match command success 0x%lx, msgid %d\n", entry->cmd, i);
            cmd_list->capi2_handler = g_kril_capi2_handler_array[i].capi2_handler;
            g_kril_capi2_handler_array[i].capi2_handler((void *)cmd_list, NULL);
            if(BCM_ErrorCAPI2Cmd == cmd_list->handler_state || BCM_FinishCAPI2Cmd == cmd_list->handler_state)
            {
                KRIL_DEBUG(DBG_TRACE, "handler_state:%ld CmdID:%ld\n", cmd_list->handler_state, cmd_list->ril_cmd->CmdID);
                KRIL_SendResponse(cmd_list);
                list_del(&cmd_list->list); // delete command list
                if (cmd_list->ril_cmd->data != NULL && cmd_list->ril_cmd->datalen != 0)
                {
                    kfree(cmd_list->ril_cmd->data);
                }
                kfree(cmd_list->ril_cmd);

                if (cmd_list->bcm_ril_rsp != NULL && cmd_list->rsp_len != 0)
                {
                    kfree(cmd_list->bcm_ril_rsp);
                }
                if (cmd_list->cmdContext != NULL)
                {
                    kfree(cmd_list->cmdContext);
                    cmd_list->cmdContext = NULL;
                }
                kfree(cmd_list);
                cmd_list = NULL;
            }
            else
            {
                cmd_list->tid = GetTID(); // store next TID in command list
                KRIL_DEBUG(DBG_INFO, "other handler state:0x%lx tid:%ld\n", cmd_list->handler_state, cmd_list->tid);
            }
            mutex_unlock(&gKrilCmdList.mutex);
        }
        else
        {
            KRIL_DEBUG(DBG_ERROR, "Command not found...!0x%lx CmdID:%ld\n", cmd_list->cmd, entry->ril_cmd->CmdID);
            cmd_list->result = BCM_E_REQUEST_NOT_SUPPORTED;
            KRIL_SendResponse(cmd_list);
            mutex_lock(&gKrilCmdList.mutex);
            list_del(&cmd_list->list); // delete command list
            if (cmd_list->ril_cmd->data != NULL && cmd_list->ril_cmd->datalen != 0)
            {
                kfree(cmd_list->ril_cmd->data);
            }
            kfree(cmd_list->ril_cmd);
            
            if (cmd_list->bcm_ril_rsp != NULL && cmd_list->rsp_len != 0)
            {
                kfree(cmd_list->bcm_ril_rsp);
            }
            kfree(cmd_list);
            cmd_list = NULL;
            mutex_unlock(&gKrilCmdList.mutex);
        }
        KRIL_DEBUG(DBG_INFO, "list_empty(gKrilCmdQueue.list):%d...!\n", list_empty(&gKrilCmdQueue.list));
        mutex_lock(&gKrilCmdQueue.mutex);
        list_del(listptr);
        kfree(entry);
    }
    mutex_unlock(&gKrilCmdQueue.mutex);
} // KRIL_cmd_thread

//******************************************************************************
/**
* Function Name: KRIL_ResponseHandler
*
* Description  : When put the CAPI2 response in response queue, 
*                the handler will wake up and process the response data
*
* PARAMETERS   : void
*
* RETURN       : void
* Notes:
*
*******************************************************************************/
void KRIL_ResponseHandler(struct work_struct *data)
{
    struct list_head *ptr, *pos;
    struct list_head *listptr, *listpos;
    Kril_CAPI2Info_t *entry = NULL;
    KRIL_CmdList_t *listentry = NULL;
    bool found = FALSE;
    UInt32 irql;

    KRIL_DEBUG(DBG_TRACE2, "head tid:%ld list:%p next:%p prev:%p\n", gKrilRespWq.capi2_head.tid, &gKrilRespWq.capi2_head.list, gKrilRespWq.capi2_head.list.next, gKrilRespWq.capi2_head.list.prev);
    spin_lock_irqsave(&gKrilRespWq.lock, irql);
    list_for_each_safe(ptr, pos, &gKrilRespWq.capi2_head.list)
    {
        entry = list_entry(ptr, Kril_CAPI2Info_t, list);
        spin_unlock_irqrestore(&gKrilRespWq.lock, irql);
        KRIL_DEBUG(DBG_TRACE2, "entry tid:%ld msgType:%d list:%p next:%p prev:%p\n", entry->tid, entry->msgType, &entry->list, entry->list.next, entry->list.prev);

        // search in command list
        mutex_lock(&gKrilCmdList.mutex);
        list_for_each_safe(listptr, listpos, &gKrilCmdList.list)
        {
            listentry = list_entry(listptr, KRIL_CmdList_t, list);
            mutex_unlock(&gKrilCmdList.mutex);

            KRIL_DEBUG(DBG_TRACE, "listentry->tid:%ld entry->tid:%ld listentry->handler_state:%lX\n", listentry->tid, entry->tid,listentry->handler_state);
            mutex_lock(&gKrilCmdList.mutex);
            if(listentry->tid == entry->tid)
            {
                found = TRUE;
                break;
            }
            else
            {
                found = FALSE;
            }
        }
        mutex_unlock(&gKrilCmdList.mutex);

        if(TRUE == found)
        {
            KRIL_DEBUG(DBG_TRACE, "Run response CAPI2 handler\n");
            mutex_lock(&gKrilCmdList.mutex);
            listentry->capi2_handler((void *)listentry, entry);
            if(BCM_FinishCAPI2Cmd == listentry->handler_state || BCM_ErrorCAPI2Cmd == listentry->handler_state)
            {
                KRIL_DEBUG(DBG_INFO, "handler_state:0x%lx client:%d CmdID:%ld\n", listentry->handler_state, listentry->ril_cmd->client, listentry->ril_cmd->CmdID);
                if (BCM_URIL_CLIENT == listentry->ril_cmd->client ||
                    BCM_AT_URIL_CLIENT == listentry->ril_cmd->client 
#ifdef BCM_RIL_FOR_EAP_SIM         // BCM_EAP_SIM            
                    ||BCM_EAP_URIL_CLIENT == listentry->ril_cmd->client
#endif
			) 
                {
                    KRIL_DEBUG(DBG_TRACE, "BCMRIL_CLIENT...!\n");
                    KRIL_SendResponse(listentry);
                }
                else if(BCM_AUDIO_CLIENT == listentry->ril_cmd->client ||
                        BCM_POWER_CLIENT == listentry->ril_cmd->client ||
                        BCM_AT_CLIENT    == listentry->ril_cmd->client)
                {
                    // Call device driver callback function
                    if(bcm_dev_results[RIL_CLIENTID_INDEX(listentry->ril_cmd->client)].resultCb != NULL)
                    {
                        bcm_dev_results[RIL_CLIENTID_INDEX(listentry->ril_cmd->client)].resultCb(
                            listentry->ril_cmd->CmdID, (listentry->handler_state == BCM_FinishCAPI2Cmd ? 1 : 0), 
                            entry->dataBuf, entry->dataLength);
                    }
                    else
                    {
                        KRIL_DEBUG(DBG_TRACE,"The resultCb of clientID:%d is NULL\n",listentry->ril_cmd->client);
                    }
                }
                else if(BCM_KRIL_CLIENT == listentry->ril_cmd->client)
                {
                    KRIL_DEBUG(DBG_TRACE, "BCM_KRIL_CLIENT client:%d...!\n", listentry->ril_cmd->client);
                }
                else
                {
                    KRIL_DEBUG(DBG_ERROR, "CLIENT %d can't find...!\n", listentry->ril_cmd->client);
                }

                list_del(listptr); // delete command list
                if(listentry->ril_cmd->data != NULL && listentry->ril_cmd->datalen != 0)
                {
                    kfree(listentry->ril_cmd->data);
                }
                kfree(listentry->ril_cmd);

                if(listentry->bcm_ril_rsp != NULL && listentry->rsp_len != 0)
                {
                    kfree(listentry->bcm_ril_rsp);
                }
                if (listentry->cmdContext != NULL)
                {
                    kfree(listentry->cmdContext);
                    listentry->cmdContext = NULL;
                }
                kfree(listentry);
                listentry = NULL;
            }
            else
            {
                if (!(entry->msgType == MSG_SYS_ENABLE_CELL_INFO_RSP || // Skip to update the TID for enable cell information
                     entry->msgType == MSG_PBK_ENTRY_DATA_RSP ||  // Skip to update the TID for read phonebook
                     entry->msgType == MSG_PLMN_SELECT_RSP)) // Skip to update the TID for PLMN selection
                {
                    listentry->tid = GetTID(); // store next TID in command list
                }
                KRIL_DEBUG(DBG_INFO, "other handler state:0x%lx tid:%ld\n", listentry->handler_state, listentry->tid);
            }
            found = FALSE;
            mutex_unlock(&gKrilCmdList.mutex);
        }
        else
        {
            KRIL_DEBUG(DBG_ERROR, "TID doesn't match...!\nentry->tid:%ld entry->client=%d msgType:%ld\n",entry->tid, entry->clientID, (UInt32)entry->msgType);
        }
        RPC_SYSFreeResultDataBuffer(entry->dataBufHandle);
        spin_lock_irqsave(&gKrilRespWq.lock, irql);

        list_del(ptr); // delete response capi2
        kfree(entry);
        entry = NULL;
    }
    spin_unlock_irqrestore(&gKrilRespWq.lock, irql);
#ifdef CONFIG_HAS_WAKELOCK
    wake_unlock(&kril_rsp_wake_lock);
#endif
    if (FALSE == list_empty(&gKrilCmdQueue.list))
        KRIL_CmdQueueWork();
    return;
} // KRIL_ResponseHandler

/****************************************************************************
*
*  KRIL_NotifyHandler
*
*  Worker thread to receive data from notify queue.
*
****************************************************************************/
void KRIL_NotifyHandler(struct work_struct *data)
{
    struct list_head *listptr, *pos;
    Kril_CAPI2Info_t *entry = NULL;
    UInt32 irql;

    KRIL_DEBUG(DBG_INFO, "head tid:%ld list:%p next:%p prev:%p\n", gKrilNotifyWq.capi2_head.tid, &gKrilNotifyWq.capi2_head.list, gKrilNotifyWq.capi2_head.list.next, gKrilNotifyWq.capi2_head.list.prev);

    spin_lock_irqsave(&gKrilNotifyWq.lock, irql);
    list_for_each_safe(listptr, pos, &gKrilNotifyWq.capi2_head.list)
    {
        entry = list_entry(listptr, Kril_CAPI2Info_t, list);
        spin_unlock_irqrestore(&gKrilNotifyWq.lock, irql);

        ProcessNotification(entry);

        KRIL_DEBUG(DBG_INFO, "entry tid:%ld msgType:%d list:%p next:%p prev:%p list_empty:%d\n", entry->tid, entry->msgType, &entry->list, entry->list.next, entry->list.prev, list_empty(&gKrilNotifyWq.capi2_head.list));
        RPC_SYSFreeResultDataBuffer(entry->dataBufHandle);
        spin_lock_irqsave(&gKrilNotifyWq.lock, irql);

        list_del(listptr);
        kfree(entry);
        entry = NULL;
    }
    spin_unlock_irqrestore(&gKrilNotifyWq.lock, irql);
#ifdef CONFIG_HAS_WAKELOCK
    wake_unlock(&kril_notify_wake_lock);
#endif
} // KRIL_NotifyHandler

//******************************************************************************
//
// Function Name: CAPI2CauseTransform
//
// Description:   CAPI2 error cause transform.
//
// Notes:
//          BCM_E_SUCCESS                          = 0
//          BCM_E_RADIO_NOT_AVAILABLE              = 1  /* If radio did not start or is resetting */
//          BCM_E_GENERIC_FAILURE                  = 2
//          BCM_E_PASSWORD_INCORRECT               = 3  /* for PIN/PIN2 methods only! */
//          BCM_E_SIM_PIN2                         = 4  /* Operation requires SIM PIN2 to be entered */
//          BCM_E_SIM_PUK2                         = 5  /* Operation requires SIM PIN2 to be entered */
//          BCM_E_REQUEST_NOT_SUPPORTED            = 6
//          BCM_E_CANCELLED                        = 7
//          BCM_E_OP_NOT_ALLOWED_DURING_VOICE_CALL = 8  /* data ops are not allowed during voice call on a Class C GPRS device */
//          BCM_E_OP_NOT_ALLOWED_BEFORE_REG_TO_NW  = 9  /* data ops are not allowed before device registers in network */
//          BCM_E_SMS_SEND_FAIL_RETRY              = 10 /* fail to send sms and need retry */
//          BCM_E_SIM_ABSENT                       = 11 /* fail to set the location where CDMA subscription card absent */
//          BCM_E_SUBSCRIPTION_NOT_AVAILABLE       = 12 /* fail to find CDMA subscription from specified location */
//          BCM_E_MODE_NOT_SUPPORTED               = 13 /* HW does not support preferred network type */
//          BCM_E_FDN_CHECK_FAILURE                = 14 /* command failed because recipient is not on FDN list */
//
//******************************************************************************
BRIL_Errno RILErrorResult(Result_t err)
{
    BRIL_Errno ret;

    switch (err)
    {
         case RESULT_OK:
             ret = BCM_E_SUCCESS;
         break;

         case SMS_NO_SERVICE:
             ret = BCM_E_OP_NOT_ALLOWED_BEFORE_REG_TO_NW;
         break;

         case SS_INVALID_PASSWORD_LENGTH:
             ret = BCM_E_PASSWORD_INCORRECT;
         break;

        case SS_FDN_BLOCK_SS_REQUEST:
        case PDP_ACTIVATION_BLOCKED_BY_SIM:
        case SMS_FDN_NOT_ALLOWED:
        case CC_FDN_BLOCK_MAKE_CALL:
            ret = BCM_E_FDN_CHECK_FAILURE;
        break;

        case SMS_SIM_BUSY:
            ret = BCM_E_SMS_SEND_FAIL_RETRY;
        break;

        default:
            ret = BCM_E_GENERIC_FAILURE;
        break;
    }
    return ret;
}

BRIL_Errno RILErrorSIMResult(SIMAccess_t err)
{
    BRIL_Errno ret;

    switch (err)
    {
         case SIMACCESS_SUCCESS:
             ret = BCM_E_SUCCESS;
         break;

         case SIMACCESS_INCORRECT_CHV:
         case SIMACCESS_INCORRECT_PUK:
             ret = BCM_E_PASSWORD_INCORRECT;
         break;

         case SIMACCESS_NEED_CHV2:
             ret = BCM_E_SIM_PIN2;
         break;

        case SIMACCESS_BLOCKED_CHV2:
            ret = BCM_E_SIM_PUK2;
        break;

        default:
            ret = BCM_E_GENERIC_FAILURE;
        break;
    }
    return ret;
}

//******************************************************************************
//
// Function Name: IsNeedToWait
//
// Description:   Judge the command whether is need to wait until some command has processed.
//
// Notes:
//
//******************************************************************************
Boolean IsNeedToWait(unsigned long CmdID)
{
    // can't handle more than 1 RIL_REQUEST_GET_NEIGHBORING_CELL_IDS at a time, so 
    // check if we're already handling a request...
    
    if ( (BRCM_RIL_REQUEST_OEM_HOOK_RAW == CmdID) && (KRIL_GetInMeasureReportHandler()) )
    {
        KRIL_DEBUG(DBG_ERROR, "MeasureReport MATCH::Cmd queue:0x%02lx\n", CmdID);
        return TRUE;
    }
    else if ( (BRCM_RIL_REQUEST_GET_NEIGHBORING_CELL_IDS == CmdID) && (KRIL_GetInNeighborCellHandler()) )
    {
        KRIL_DEBUG(DBG_INFO, "NeighborCellHandler MATCH::Cmd queue:0x%02lx\n", CmdID);
        return TRUE;
    }
    else if ( (BRCM_RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC == CmdID || 
         BRCM_RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL == CmdID) && 
         (KRIL_GetInNetworkSelectHandler()) )
    {
        // can't handle more than one call to CAPI2_NetRegApi_PlmnSelect() at a time, as we must 
        // wait for both solicited response MSG_PLMN_SELECT_RSP and potentially unsolicited 
        // notification MSG_PLMN_SELECT_CNF (which needs to have tid of original request mapped
        // to it so it can be routed to appropriate KRIL handler)
        KRIL_DEBUG(DBG_INFO, "NetworkSelectHandler MATCH::Cmd queue:0x%02lx\n", CmdID);
        return TRUE;
    }
    else if ((BRCM_RIL_REQUEST_DIAL == CmdID || RIL_REQUEST_DIAL_EMERGENCY_CALL == CmdID) && KRIL_GetInHoldCallHandler() == TRUE)
    {
        KRIL_DEBUG(DBG_INFO, "Dial Handler MATCH::Cmd queue:0x%02lx\n", CmdID);
        KRIL_SetIsNeedMakeCall(TRUE);
        return TRUE;
    }
    else if ((BRCM_RIL_REQUEST_SEND_SMS == CmdID || BRCM_RIL_REQUEST_SEND_SMS_EXPECT_MORE == CmdID) && KRIL_GetInSendSMSHandler() == TRUE)
    {
        KRIL_DEBUG(DBG_INFO, "Send SMS handler MATCH::Cmd queue:0x%02lx\n", CmdID);
        KRIL_IncrementSendSMSNumber();
        return TRUE;
    }
    else if ((BRCM_RIL_REQUEST_WRITE_SMS_TO_SIM == CmdID || BRCM_RIL_REQUEST_DELETE_SMS_ON_SIM == CmdID) && KRIL_GetInUpdateSMSInSIMHandler() == TRUE)
    {
        KRIL_DEBUG(DBG_INFO, "Send SMS handler MATCH::Cmd queue:0x%02lx\n", CmdID);
        KRIL_IncrementUpdateSMSNumber();
        return TRUE;
    }
    else if((BRCM_RIL_REQUEST_SETUP_DATA_CALL == CmdID) && (KRIL_GetInSetupPDPHandler() || KRIL_GetInSetPrefNetworkTypeHandler()) )
	{
		KRIL_DEBUG(DBG_INFO, "SetupPdpHandler MATCH::Cmd queue:0x%02lx\n", CmdID);
		return TRUE; 
    }
    else if (BRCM_RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE == CmdID && KRIL_GetInSetPrefNetworkTypeHandler() == TRUE)
    {
        KRIL_DEBUG(DBG_INFO, "Set Preferred Network handler MATCH::Cmd queue:0x%02lx\n", CmdID);
        KRIL_SetIsNeedSetPreferNetworkType(TRUE); 
        return TRUE;
    }
    else if(BRCM_RIL_REQUEST_SIM_IO == CmdID)
    {
        struct list_head *listptr, *listpos;
        KRIL_CmdList_t *listentry = NULL;
        Boolean found = FALSE;
        int i = 0;
        mutex_lock(&gKrilCmdList.mutex);
        list_for_each_safe(listptr, listpos, &gKrilCmdList.list)
        {
            listentry = list_entry(listptr, KRIL_CmdList_t, list);
            KRIL_DEBUG(DBG_TRACE, "command list:: i:%d CmdID:%ld\n", i, listentry->ril_cmd->CmdID);
            if(CmdID == listentry->ril_cmd->CmdID)
            {
                KRIL_DEBUG(DBG_INFO, "command list::find tid:%d\n", listentry->tid);
                found = TRUE;
                break;
            }
            i++;
        }
        mutex_unlock(&gKrilCmdList.mutex);
        return found;
    }
    else if(BRCM_RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND == CmdID ||
            BRCM_RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND == CmdID ||
            BRCM_RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE == CmdID ||
            BRCM_RIL_REQUEST_SEPARATE_CONNECTION == CmdID ||
            BRCM_RIL_REQUEST_CONFERENCE == CmdID) // Just only send one MPTY call request to CP to avoid call state error
    {
        struct list_head *listptr, *listpos;
        KRIL_CmdList_t *listentry = NULL;
        Boolean found = FALSE;
        int i = 0;
        mutex_lock(&gKrilCmdList.mutex);
        list_for_each_safe(listptr, listpos, &gKrilCmdList.list)
        {
            listentry = list_entry(listptr, KRIL_CmdList_t, list);
            KRIL_DEBUG(DBG_TRACE, "command list:: i:%d CmdID:%ld\n", i, listentry->ril_cmd->CmdID);
            if(BRCM_RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND == listentry->ril_cmd->CmdID ||
               BRCM_RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND == listentry->ril_cmd->CmdID ||
               BRCM_RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE == listentry->ril_cmd->CmdID ||
               BRCM_RIL_REQUEST_SEPARATE_CONNECTION == listentry->ril_cmd->CmdID ||
               BRCM_RIL_REQUEST_CONFERENCE == listentry->ril_cmd->CmdID)
            {
                KRIL_DEBUG(DBG_INFO, "command list::CmdID:%d find CmdID:%d tid:%d\n", CmdID, listentry->ril_cmd->CmdID, listentry->tid);
                found = TRUE;
                break;
            }
            i++;
        }
        mutex_unlock(&gKrilCmdList.mutex);
        return found;
    }
    else if(BRCM_RIL_REQUEST_SEND_USSD == CmdID ||
            BRCM_RIL_REQUEST_CANCEL_USSD == CmdID ||
            BRCM_RIL_REQUEST_SET_CLIR == CmdID ||
            BRCM_RIL_REQUEST_SET_CALL_FORWARD == CmdID ||
            BRCM_RIL_REQUEST_SET_CALL_WAITING == CmdID ||
            BRCM_RIL_REQUEST_CHANGE_BARRING_PASSWORD == CmdID) // Just only send one SS request to CP to avoid re-entrant requests
    {
        struct list_head *listptr, *listpos;
        KRIL_CmdList_t *listentry = NULL;
        Boolean found = FALSE;
        int i = 0;
        mutex_lock(&gKrilCmdList.mutex);
        list_for_each_safe(listptr, listpos, &gKrilCmdList.list)
        {
            listentry = list_entry(listptr, KRIL_CmdList_t, list);
            KRIL_DEBUG(DBG_TRACE, "command list:: i:%d CmdID:%ld\n", i, listentry->ril_cmd->CmdID);
            if(BRCM_RIL_REQUEST_SEND_USSD == listentry->ril_cmd->CmdID ||
               BRCM_RIL_REQUEST_CANCEL_USSD == listentry->ril_cmd->CmdID ||
               BRCM_RIL_REQUEST_SET_CLIR == listentry->ril_cmd->CmdID ||
               BRCM_RIL_REQUEST_SET_CALL_FORWARD == listentry->ril_cmd->CmdID ||
               BRCM_RIL_REQUEST_SET_CALL_WAITING == listentry->ril_cmd->CmdID ||
               BRCM_RIL_REQUEST_CHANGE_BARRING_PASSWORD == listentry->ril_cmd->CmdID)
            {
                KRIL_DEBUG(DBG_INFO, "command list::CmdID:%ld find CmdID:%ld tid:%ld\n", CmdID, listentry->ril_cmd->CmdID, listentry->tid);
                found = TRUE;
                break;
            }
            i++;
        }
        mutex_unlock(&gKrilCmdList.mutex);
        return found;
    }
    else if ((BRCM_RIL_REQUEST_QUERY_CALL_FORWARD_STATUS == CmdID ||
             BRCM_RIL_REQUEST_GET_CLIR == CmdID ||
             BRCM_RIL_REQUEST_QUERY_CALL_WAITING == CmdID) &&
                KRIL_GetInSsQueryHandler() == TRUE)
    {
        // can't handle more than 1 SS request at a time,
        // so check if we're already handling a request...
        KRIL_DEBUG(DBG_INFO, "SSHandler MATCH::Cmd queue:0x%02lx\n", CmdID);
        return TRUE;
    }

    return FALSE;
}

//******************************************************************************
//
// Function Name: KRIL_CmdQueueWork
//
// Description:   Trigger Command Queue work
//
// Notes:
//
//******************************************************************************
void KRIL_CmdQueueWork(void)
{
    queue_work(gKrilCmdQueue.cmd_wq, &gKrilCmdQueue.commandq);
}

//******************************************************************************
//
// Function Name: SetBasicLoggingEnable
//
// Description:   Set logging Enable/Disable.
//
// Notes:
//
//******************************************************************************
void SetBasicLoggingEnable(Boolean Enable)
{
    gEnable = Enable;
}

//******************************************************************************
//
// Function Name: IsBasicLoggingEnable
//
// Description:   Get is logging Enable.
//
// Notes:
//
//******************************************************************************
Boolean IsBasicCapi2LoggingEnable(void)
{
    return gEnable;
}

//******************************************************************************
//
// Function Name: KRIL_SetSimAppType
//
// Description:   Set SIM Application type.
//
// Notes:
//
//******************************************************************************
void KRIL_SetSimAppType(SIM_APPL_TYPE_t simAppType)
{
    sSimAppType = simAppType;
    
    if (sSimAppType != SIM_APPL_INVALID)
    {
        KRIL_DEBUG(DBG_INFO,"SIM APP TYPE:%s\n",((SIM_APPL_2G == sSimAppType)? "2G":"3G"));
    }
}

//******************************************************************************
//
// Function Name: KRIL_GetSimAppType
//
// Description:   Get SIM Application type.
//
// Notes:
//
//******************************************************************************
SIM_APPL_TYPE_t KRIL_GetSimAppType()
{
    if (sSimAppType != SIM_APPL_INVALID)
    {
        KRIL_DEBUG(DBG_INFO,"SIM APP TYPE:%s\n",((SIM_APPL_2G == sSimAppType)? "2G":"3G"));
    }
    return sSimAppType;
}

//******************************************************************************
//
// Function Name: KRIL_SetIncomingCallIndex
//
// Description:   Set Incoming Call Index.
//
// Notes:
//
//******************************************************************************
void KRIL_SetIncomingCallIndex(UInt8 IncomingCallIndex)
{
    KRIL_DEBUG(DBG_INFO,"IncomingCallIndex:%d\n", IncomingCallIndex);
    sIncomingCallIndex = IncomingCallIndex;
}

//******************************************************************************
//
// Function Name: KRIL_GetIncomingCallIndex
//
// Description:   Get Incoming Call Index.
//
// Notes:
//
//******************************************************************************
UInt8 KRIL_GetIncomingCallIndex()
{
    KRIL_DEBUG(DBG_INFO,"sIncomingCallIndex:%d\n", sIncomingCallIndex);
    return sIncomingCallIndex;
}

//******************************************************************************
//
// Function Name: KRIL_SetWaitingCallIndex
//
// Description:   Set Waiting Call Index.
//
// Notes:
//
//******************************************************************************
void KRIL_SetWaitingCallIndex(UInt8 WaitingCallIndex)
{
    KRIL_DEBUG(DBG_INFO,"WaitingCallIndex:%d\n", WaitingCallIndex);
    sWaitingCallIndex = WaitingCallIndex;
}

//******************************************************************************
//
// Function Name: KRIL_GetWaitingCallIndex
//
// Description:   Get Waiting Call Index.
//
// Notes:
//
//******************************************************************************
UInt8 KRIL_GetWaitingCallIndex()
{
    KRIL_DEBUG(DBG_INFO,"sWaitingCallIndex:%d\n", sWaitingCallIndex);
    return sWaitingCallIndex;
}

//******************************************************************************
//
// Function Name: KRIL_SetCallType
//
// Description:   Set specific call type.
//
// Notes:
//
//******************************************************************************
void KRIL_SetCallType(int index, CCallType_t theCallType)
{
    KRIL_DEBUG(DBG_INFO,"index:%d theCallType:%d\n", index, theCallType);
    sCallType[index] = theCallType;
}

//******************************************************************************
//
// Function Name: KRIL_GetCallType
//
// Description:   Get specific call type.
//
// Notes:
//
//******************************************************************************
CCallType_t KRIL_GetCallType(int index)
{
    KRIL_DEBUG(DBG_INFO,"sCallType[%d]:%d\n", index, sCallType[index]);
    return sCallType[index];
}

//******************************************************************************
//
// Function Name: KRIL_SetCallState
//
// Description:   Set specific call state.
//
// Notes:
//
//******************************************************************************
void KRIL_SetCallState(int index, BRIL_CallState theCallState)
{
    KRIL_DEBUG(DBG_INFO,"index:%d theCallType:%d\n", index, theCallState);
    sCallState[index] = theCallState;
}

//******************************************************************************
//
// Function Name: KRIL_GetCallState
//
// Description:   Get specific call state.
//
// Notes:
//
//******************************************************************************
BRIL_CallState KRIL_GetCallState(int index)
{
    KRIL_DEBUG(DBG_INFO,"sCallState[%d]:%d\n", index, sCallState[index]);
    return sCallState[index];
}

//******************************************************************************
//
// Function Name: KRIL_ClearCallNumPresent
//
// Description:   Clear Call Number Present
//
// Notes:   Set all call number present status to CC_PRESENTATION_ALLOWED
//
//******************************************************************************
void KRIL_ClearCallNumPresent(void)
{
    memset(sCallNumPresent, CC_PRESENTATION_ALLOWED, sizeof(PresentationInd_t)*BCM_MAX_CALLS_NO);
}

//******************************************************************************
//
// Function Name: KRIL_SetCallNumPresent
//
// Description:   Set Call Number Present
//
// Notes:
//
//******************************************************************************
void KRIL_SetCallNumPresent(int index, PresentationInd_t present, UInt8 c_num)
{
    KRIL_DEBUG(DBG_INFO,"index:%d theCallType:%d NumberLength:%d\n", index, present,c_num);
    if( c_num > 0)
	sCallNumPresent[index] = 0; //CC_PRESENTATION_ALLOWED
    else
	sCallNumPresent[index] = present;
}

//******************************************************************************
//
// Function Name: KRIL_GetCallNumPresent
//
// Description:   Get Call Number Present
//
// Notes:
//
//******************************************************************************
PresentationInd_t KRIL_GetCallNumPresent(int index)
{
    KRIL_DEBUG(DBG_INFO,"sCallNumPresent[%d]:%d\n", index, sCallNumPresent[index]);
    return sCallNumPresent[index];
}

//******************************************************************************
//
// Function Name: KRIL_SetInHoldCallHandler
//
// Description:   Set the flag in Call Handler has not processed yet.
//
// Notes:
//
//******************************************************************************
void KRIL_SetInHoldCallHandler(Boolean CallHandler)
{
    KRIL_DEBUG(DBG_INFO,"CallHandler:%d\n", CallHandler);
    sInCallHandler = CallHandler;
    if (TRUE == KRIL_GetIsNeedMakeCall())
    {
        KRIL_SetIsNeedMakeCall(FALSE);
        KRIL_CmdQueueWork();
    }
}

//******************************************************************************
//
// Function Name: KRIL_GetInHoldCallHandler
//
// Description:   Get the flag in Call handler has not processed yet.
//
// Notes:
//
//******************************************************************************
Boolean KRIL_GetInHoldCallHandler(void)
{
    KRIL_DEBUG(DBG_INFO,"isMakeCall:%d\n", sInCallHandler);
    return sInCallHandler;
}

//******************************************************************************
//
// Function Name: KRIL_SetIsNeedMakeCall
//
// Description:   Set the flag for make call handler is running.
//
// Notes:
//
//******************************************************************************
void KRIL_SetIsNeedMakeCall(Boolean MakeCall)
{
    KRIL_DEBUG(DBG_INFO,"IsMakeCall:%d\n", MakeCall);
    sIsMakeCall = MakeCall;
}

//******************************************************************************
//
// Function Name: KRIL_GetIsNeedMakeCall
//
// Description:   Get the flag to check whether the make call handler is running.
//
// Notes:
//
//******************************************************************************
Boolean KRIL_GetIsNeedMakeCall(void)
{
    KRIL_DEBUG(DBG_INFO,"sIsMakeCall:%d\n", sIsMakeCall);
    return sIsMakeCall;
}

//******************************************************************************
//
// Function Name: KRIL_SetHungupForegroundResumeBackgroundEndMPTY
//
// Description:   Set HungupForegroundResumeBackground TID when End MPTY.
//
// Notes:
//
//******************************************************************************
void KRIL_SetHungupForegroundResumeBackgroundEndMPTY(UInt32 tid)
{
    KRIL_DEBUG(DBG_INFO,"KRIL_SetHungupForegroundResumeBackgroundEndMPTY:%ld\n", tid);
    sMPTYTID = tid;
}

//******************************************************************************
//
// Function Name: KRIL_GetHungupForegroundResumeBackgroundEndMPTY
//
// Description:   Get HungupForegroundResumeBackground TID when End MPTY.
//
// Notes:
//
//******************************************************************************
UInt32 KRIL_GetHungupForegroundResumeBackgroundEndMPTY(void)
{
    KRIL_DEBUG(DBG_INFO,"sMPTYTID:%ld\n", sMPTYTID);
    return sMPTYTID;
}

//******************************************************************************
//
// Function Name: KRIL_SetLastCallFailCause
//
// Description:   Set the last call failure cause.
//
// Notes:
//
//******************************************************************************
void KRIL_SetLastCallFailCause(BRIL_LastCallFailCause inCause)
{
    KRIL_DEBUG(DBG_INFO,"SetLastCallFailCause:%d\n", inCause);
    sLastCallFailCause = inCause;
}

//******************************************************************************
//
// Function Name: KRIL_GetLastCallFailCause
//
// Description:   Get the last call failure cause.
//
// Notes:
//
//******************************************************************************
BRIL_LastCallFailCause KRIL_GetLastCallFailCause(void)
{
    KRIL_DEBUG(DBG_INFO,"sLastCallFailCause:%d\n", sLastCallFailCause);
    return sLastCallFailCause;
}

//******************************************************************************
//
// Function Name: KRIL_MNCauseToRilError
//
// Description:   Converts the MN failure cause to a RIL last call failure cause.
//
// Notes:
//
//******************************************************************************
BRIL_LastCallFailCause KRIL_MNCauseToRilError(Cause_t inMNCause)
{
    BRIL_LastCallFailCause failCause;

    switch (inMNCause)
    {
        case MNCAUSE_NORMAL_CALL_CLEARING:
            failCause = BCM_CALL_FAIL_NORMAL;
            break;

        case MNCAUSE_USER_BUSY:
            failCause = BCM_CALL_FAIL_BUSY;
            break;

        case MNCAUSE_NO_CIRCUIT_AVAILABLE:
        case MNCAUSE_SWITCH_CONGESTION:
            failCause = BCM_CALL_FAIL_CONGESTION;
            break;

        case MNCAUSE_ACM_GREATER_OR_EQUAL_TO_ACMMAX:
            failCause = BCM_CALL_FAIL_ACM_LIMIT_EXCEEDED;
            break;

        case MNCAUSE_OPERATOR_BARRING:
            failCause = BCM_CALL_FAIL_CALL_BARRED;
            break;
            
        default:
            failCause = BCM_CALL_FAIL_ERROR_UNSPECIFIED;
            break;
    }

    KRIL_DEBUG(DBG_INFO, "MN Cause:0x%x failCause:0x%x\n", inMNCause, failCause);
    return failCause;
}

//******************************************************************************
//
// Function Name: KRIL_SetSmsMti
//
// Description:   Set SMS Message indicator type.
//
// Notes:
//
//******************************************************************************
void KRIL_SetSmsMti(SmsMti_t SmsMti)
{
    KRIL_DEBUG(DBG_INFO,"SmsMti:%d\n", SmsMti);
    sSmsMti = SmsMti;
}

//******************************************************************************
//
// Function Name: KRIL_GetSmsMti
//
// Description:   Get SMS Message indicator type.
//
// Notes:
//
//******************************************************************************
SmsMti_t KRIL_GetSmsMti(void)
{
    KRIL_DEBUG(DBG_INFO,"sSmsMti:%d\n", sSmsMti);
    return sSmsMti;
}

//******************************************************************************
//
// Function Name: SetIsRevClass2SMS
//
// Description:   Set Receive Class2 SMS state.
//
// Notes:
//
//******************************************************************************
void SetIsRevClass2SMS(Boolean value)
{
    KRIL_DEBUG(DBG_INFO,"IsClass2SMS:%d\n", value);
    sIsClass2SMS = value;
}

//******************************************************************************
//
// Function Name: GetIsRevClass2SMS
//
// Description:   Get Receive Class2 SMS state.
//
// Notes:
//
//******************************************************************************
Boolean GetIsRevClass2SMS(void)
{
    KRIL_DEBUG(DBG_INFO,"sIsClass2SMS:%d\n", sIsClass2SMS);
    return sIsClass2SMS;
}

//******************************************************************************
//
// Function Name: KRIL_SetInSendSMSHandler
//
// Description:   Set the flag for send SMS handler is running.
//
// Notes:
//
//******************************************************************************
void KRIL_SetInSendSMSHandler(Boolean SMSHandler)
{
    KRIL_DEBUG(DBG_INFO,"InSMSHandler:%d\n", SMSHandler);
    sInSMSHandler = SMSHandler;
}

//******************************************************************************
//
// Function Name: KRIL_GetInSendSMSHandler
//
// Description:   Get the flag to check whether the send SMS handler is running.
//
// Notes:
//
//******************************************************************************
Boolean KRIL_GetInSendSMSHandler(void)
{
    KRIL_DEBUG(DBG_INFO,"sInSMSHandler:%d\n", sInSMSHandler);
    return sInSMSHandler;
}

//******************************************************************************
//
// Function Name: KRIL_SetMESMSAvailable
//
// Description:   Set SMS in ME is available
//
// Notes:
//******************************************************************************
void KRIL_SetMESMSAvailable(Boolean IsSMSMEAvailable)
{
    sIsSMSMEAvailable = IsSMSMEAvailable;
    KRIL_DEBUG(DBG_INFO,"sIsSMSMEAvailable:%d\n", sIsSMSMEAvailable);
}

//******************************************************************************
//
// Function Name: KRIL_GetMESMSAvailable
//
// Description:   Get SMS in ME is available
//
// Notes:
//******************************************************************************
Boolean KRIL_GetMESMSAvailable(void)
{
    KRIL_DEBUG(DBG_INFO,"sIsSMSMEAvailable:%d\n", sIsSMSMEAvailable);
    return sIsSMSMEAvailable;
}

//******************************************************************************
//
// Function Name: KRIL_SetTotalSMSInSIM
//
// Description:   Set Total SMS Number In SIM and allocate SMS array to keep the SMS status.
//
// Notes:
//******************************************************************************
void KRIL_SetTotalSMSInSIM(UInt8 TotalSMSInSIM)
{
    sTotalSMSInSIM = TotalSMSInSIM;
    if(sSMSArray != NULL)
    {
        kfree(sSMSArray);
    }
    sSMSArray = (SIMSMSMesgStatus_t *)kmalloc(sTotalSMSInSIM * sizeof(SIMSMSMesgStatus_t), GFP_KERNEL);
    memset(sSMSArray, SIMSMSMESGSTATUS_FREE, sTotalSMSInSIM * sizeof(SIMSMSMesgStatus_t));
    KRIL_DEBUG(DBG_INFO,"sTotalSMSInSIM:%d\n", sTotalSMSInSIM);
}

//******************************************************************************
//
// Function Name: KRIL_GetTotalSMSInSIM
//
// Description:   Get Total SMS Number of SIM.
//
// Notes:
//******************************************************************************
UInt8 KRIL_GetTotalSMSInSIM(void)
{
    KRIL_DEBUG(DBG_TRACE,"sTotalSMSInSIM:%d\n", sTotalSMSInSIM);
    return sTotalSMSInSIM;
}

//******************************************************************************
//
// Function Name: CheckFreeSMSIndex
//
// Description:   Check Free SMS index
//
// Notes:  return index value is '1' base, SMS array in KRIL is '0' base
//******************************************************************************
UInt8 CheckFreeSMSIndex(void)
{
    UInt8 i;
    for(i = 0 ; i < KRIL_GetTotalSMSInSIM() ; i++)
    {
        KRIL_DEBUG(DBG_INFO, "sSMSArray[%d]:%d\n", i, sSMSArray[i]);
        if(SIMSMSMESGSTATUS_FREE == sSMSArray[i])
        {
            i++;
            return i;
    }
    }
    return SMS_FULL;
}

//******************************************************************************
//
// Function Name: SetSMSMesgStatus
//
// Description:   Set SMS Message Status
//
// Notes:  Input index is '1' base, SMS array in KRIL is '0' base
//******************************************************************************
void SetSMSMesgStatus(UInt8 Index, SIMSMSMesgStatus_t status)
{
    Index--;
    sSMSArray[Index] = status;
    KRIL_DEBUG(DBG_INFO,"status[%d]:%d\n", Index, status);
}

//******************************************************************************
//
// Function Name: GetSMSMesgStatus
//
// Description:   Get SMS Message Status
//
// Notes:  Input index is '1' base, SMS array in KRIL is '0' base
//******************************************************************************
SIMSMSMesgStatus_t GetSMSMesgStatus(UInt8 Index)
{
    Index--;
    KRIL_DEBUG(DBG_INFO,"sSMSArray[%d]:%d\n", Index, sSMSArray[Index]);
    return sSMSArray[Index];
}

//******************************************************************************
//
// Function Name: KRIL_IncrementSendSMSNumber
//
// Description:   Increase Sending SMS Number.
//
// Notes:
//******************************************************************************
void KRIL_IncrementSendSMSNumber(void)
{
    KRIL_DEBUG(DBG_INFO,"sSendSMSNumber:%d\n", sSendSMSNumber);
    sSendSMSNumber++;
}

//******************************************************************************
//
// Function Name: KRIL_DecrementSendSMSNumber
//
// Description:   Decrease Sending SMS Number.
//
// Notes:
//******************************************************************************
void KRIL_DecrementSendSMSNumber(void)
{
    KRIL_DEBUG(DBG_INFO,"sSendSMSNumber:%d\n", sSendSMSNumber);
    if (sSendSMSNumber > 0)
        sSendSMSNumber--;
    else
         sSendSMSNumber = 0;
}

//******************************************************************************
//
// Function Name: KRIL_GetSendSMSNumber
//
// Description:   Get Sending SMS Number.
//
// Notes:
//
//******************************************************************************
Int8 KRIL_GetSendSMSNumber(void)
{
    KRIL_DEBUG(DBG_INFO,"sSendSMSNumber:%d\n", sSendSMSNumber);
    return sSendSMSNumber;
}

//******************************************************************************
//
// Function Name: KRIL_SetInUpdateSMSInSIMHandler
//
// Description:   Set the flag for write/delete SMS handler is running.
//
// Notes:
//
//******************************************************************************
void KRIL_SetInUpdateSMSInSIMHandler(Boolean SMSHandler)
{
    KRIL_DEBUG(DBG_INFO,"InSMSHandler:%d\n", SMSHandler);
    sInSMSHandler = SMSHandler;
}

//******************************************************************************
//
// Function Name: KRIL_GetInUpdateSMSInSIMHandler
//
// Description:   Get the flag to check whether the write/delete SMS handler is running.
//
// Notes:
//
//******************************************************************************
Boolean KRIL_GetInUpdateSMSInSIMHandler(void)
{
    KRIL_DEBUG(DBG_INFO,"sInSMSHandler:%d\n", sInSMSHandler);
    return sInSMSHandler;
}

//******************************************************************************
//
// Function Name: KRIL_IncrementUpdateSMSNumber
//
// Description:   Increase Update SMS Number.
//
// Notes:
//******************************************************************************
void KRIL_IncrementUpdateSMSNumber(void)
{
    KRIL_DEBUG(DBG_INFO,"sUpdateSMSNumber:%d\n", sUpdateSMSNumber);
    sUpdateSMSNumber++;
}

//******************************************************************************
//
// Function Name: KRIL_DecrementUpdateSMSNumber
//
// Description:   Change Update SMS Number.
//
// Notes:
//******************************************************************************
void KRIL_DecrementUpdateSMSNumber(void)
{
    KRIL_DEBUG(DBG_INFO,"sUpdateSMSNumber:%d\n", sUpdateSMSNumber);
    if (sUpdateSMSNumber > 0)
        sUpdateSMSNumber--;
    else
        sUpdateSMSNumber = 0;
}

//******************************************************************************
//
// Function Name: KRIL_GetUpdateSMSNumber
//
// Description:   Get Update SMS Number.
//
// Notes:
//
//******************************************************************************
Int8 KRIL_GetUpdateSMSNumber(void)
{
    KRIL_DEBUG(DBG_INFO,"sUpdateSMSNumber:%d\n", sUpdateSMSNumber);
    if(sUpdateSMSNumber > 0)
    {
        KRIL_DecrementUpdateSMSNumber();
        KRIL_CmdQueueWork();
    }
    return sUpdateSMSNumber;
}

//******************************************************************************
//
// Function Name: KRIL_SetSMSToSIMTID
//
// Description:   Store the SMS message TID that store in SIM.
//
// Notes:
//
//******************************************************************************
void KRIL_SetSMSToSIMTID(UInt32 tid)
{
    KRIL_DEBUG(DBG_INFO,"SMSToSIMTID:%ld\n", tid);
    sWriteSMSToSIMTID = tid;
}

//******************************************************************************
//
// Function Name: KRIL_GetSMSToSIMTID
//
// Description:   Get the SMS message TID that store in SIM.
//
// Notes:
//
//******************************************************************************
UInt32 KRIL_GetSMSToSIMTID(void)
{
    KRIL_DEBUG(DBG_INFO,"sWriteSMSToSIMTID:%ld\n", sWriteSMSToSIMTID);
    return sWriteSMSToSIMTID;
}

//******************************************************************************
//
// Function Name: KRIL_SetServingCellTID
//
// Description:   Store the TID for Serving Cell information.
//
// Notes:
//
//******************************************************************************
void KRIL_SetServingCellTID(UInt32 tid)
{
    KRIL_DEBUG(DBG_INFO,"ServingCellTID:%ld\n", tid);
    sServingCellTID = tid;
}

//******************************************************************************
//
// Function Name: KRIL_GetServingCellTID
//
// Description:   Get the TID for Serving Cell information.
//
// Notes:
//
//******************************************************************************
UInt32 KRIL_GetServingCellTID(void)
{
    KRIL_DEBUG(DBG_INFO,"sServingCellTID:%ld\n", sServingCellTID);
    return sServingCellTID;
}

//******************************************************************************
//
// Function Name: KRIL_SetInNeighborCellHandler
//
// Description:   Sets whether or not we are in process of handling 
//                RIL_REQUEST_GET_NEIGHBORING_CELL_IDS. Used to serialize
//                these requests as the KRIL handler is not re-entrant.
//
// Notes:
//
//******************************************************************************
void KRIL_SetInNeighborCellHandler( Boolean inHandler )
{
    KRIL_DEBUG(DBG_INFO,"inHandler:%s\n", (inHandler?"TRUE":"FALSE") );
    sInNeighborCellHandler = inHandler;
}

//******************************************************************************
//
// Function Name: KRIL_GetInNeighborCellHandler
//
// Description:   TRUE if currently handling RIL_REQUEST_GET_NEIGHBORING_CELL_IDS
//                request, FALSE otherwise.
//
// Notes:
//
//******************************************************************************
Boolean KRIL_GetInNeighborCellHandler()
{
    return sInNeighborCellHandler;
}

//******************************************************************************
//
// Function Name: KRIL_SetInSetupPDPHandler
//
// Description:   Sets whether or not we are in process of handling 
//                RIL_REQUEST_SETUP_DATA_CALL . Used to serialize
//                these requests as the KRIL handler is not re-entrant.
//
// Notes:
//
//******************************************************************************
void KRIL_SetInSetupPDPHandler( Boolean inHandler )
{
    KRIL_DEBUG(DBG_INFO,"inHandler:%s\n", (inHandler?"TRUE":"FALSE") );
    sInSetupPDPHandler=inHandler;
}

//******************************************************************************
//
// Function Name: KRIL_GetInSetupPDPHandler
//
// Description:   TRUE if currently handling RIL_REQUEST_SETUP_DATA_CALL 
//                request, FALSE otherwise.
//
// Notes:
//
//******************************************************************************
Boolean KRIL_GetInSetupPDPHandler()
{
    return sInSetupPDPHandler;
}

/*+20110418 BCOM PATCH FOR DebugScreen*/
//******************************************************************************
//
// Function Name: KRIL_SetServingCellTID
//
// Description:   Store the TID for Serving Cell information.
//
// Notes:
//
//******************************************************************************
void KRIL_SetMeasureReportTID(UInt32 tid)
{
    KRIL_DEBUG(DBG_ERROR,"KRIL_SetMeasureReportTID:%ld\n", tid);
    sMeasureReportTID = tid;
}

//******************************************************************************
//
// Function Name: KRIL_GetServingCellTID
//
// Description:   Get the TID for Serving Cell information.
//
// Notes:
//
//******************************************************************************
UInt32 KRIL_GetMeasureReportTID(void)
{
    KRIL_DEBUG(DBG_ERROR,"KRIL_GetMeasureReportTID:%ld\n", sMeasureReportTID);
    return sMeasureReportTID;
}
/*-20110418 BCOM PATCH FOR DebugScreen*/

//******************************************************************************
//
// Function Name: KRIL_SetNetworkSelectTID
//
// Description:   Store the TID for CAPI2_NetRegApi_PlmnSelect requests.
//
// Notes:
//
//******************************************************************************
void KRIL_SetNetworkSelectTID(UInt32 tid)
{
    KRIL_DEBUG(DBG_INFO,"NetworkSelectTID:%ld\n", tid);
    sNetworkSelectTID = tid;
}

//******************************************************************************
//
// Function Name: KRIL_GetNetworkSelectTID
//
// Description:   Get the TID for CAPI2_NetRegApi_PlmnSelect requests.
//
// Notes:
//
//******************************************************************************
UInt32 KRIL_GetNetworkSelectTID(void)
{
    KRIL_DEBUG(DBG_INFO,"sNetworkSelectTID:%ld\n", sNetworkSelectTID);
    return sNetworkSelectTID;
}

//******************************************************************************
//
// Function Name: KRIL_SetInNetworkSelectHandler
//
// Description:   Sets whether or not we are in process of handling 
//                RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC or
//                RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL. Used to serialize
//                these requests as the KRIL handlers are not re-entrant.
//
// Notes:
//
//******************************************************************************
void KRIL_SetInNetworkSelectHandler( Boolean inHandler )
{
    KRIL_DEBUG(DBG_INFO,"inHandler:%s\n", (inHandler?"TRUE":"FALSE") );
    sInNetworkSelectHandler = inHandler;
}

//******************************************************************************
//
// Function Name: KRIL_GetInNetworkSelectHandler
//
// Description:   TRUE if currently handling RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC
//                or RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL requests, 
//                FALSE otherwise.
//
// Notes:
//
//******************************************************************************
Boolean KRIL_GetInNetworkSelectHandler()
{
    return sInNetworkSelectHandler;
}


//******************************************************************************
//
// Function Name: KRIL_SetHandling2GOnlyRequest
//
//                a change to "2G only". Used to determine whether or
//                not to pass network search indications to MMI 
//                (customer requested feature).
//
// Notes:
//
//******************************************************************************
 void KRIL_SetHandling2GOnlyRequest( Boolean inHandler )
 {
    KRIL_DEBUG(DBG_INFO,"inHandler:%s\n", (inHandler?"TRUE":"FALSE") );
    sHandling2GOnlyRequest = inHandler;
 }

//******************************************************************************
//
// Function Name: KRIL_GetHandling2GOnlyRequest
//
// Description:   TRUE if in process of handling 
//                a change to "2G only" network setting, FALSE otherwise.
//
// Notes:
//
//******************************************************************************
Boolean KRIL_GetHandling2GOnlyRequest()
{
    return sHandling2GOnlyRequest;
}


void KRIL_SetInMeasureReportHandler( Boolean inHandler )
{
    KRIL_DEBUG(DBG_ERROR,"inHandler:%s\n", (inHandler?"TRUE":"FALSE") );
    sInMeasureReportHandler= inHandler;
}

Boolean KRIL_GetInMeasureReportHandler()
{
    return sInMeasureReportHandler;
}
/*-20110418 BCOM PATCH FOR DebugScreen*/

//******************************************************************************
//
// Function Name: KRIL_SetCLIPValue
//
// Description:   Set CLIP Value.
//
// Notes:
//
//******************************************************************************
void KRIL_SetCLIPValue(int LastCLIP)
{
    KRIL_DEBUG(DBG_INFO,"LastCLIP:%d\n", LastCLIP);
    sLastCLIP = LastCLIP;
}

//******************************************************************************
//
// Function Name: KRIL_GetWaitingCallIndex
//
// Description:   Get CLIP Value.
//
// Notes:
//
//******************************************************************************
int KRIL_GetCLIPValue()
{
    KRIL_DEBUG(DBG_INFO,"sLastCLIP:%d\n", sLastCLIP);
    return sLastCLIP;
}

//******************************************************************************
//
// Function Name: KRIL_SetInSsQueryHandler
//
// Description:   TRUE if currently handling BRCM_RIL_REQUEST_QUERY_CALL_FORWARD_STATUS,
//                BRCM_RIL_REQUEST_GET_CLIR, or BRCM_RIL_REQUEST_QUERY_CALL_WAITING
//                request; FALSE otherwise.
//
// Notes:
//
//******************************************************************************
void KRIL_SetInSsQueryHandler(Boolean inHandler)
{
    KRIL_DEBUG(DBG_INFO,"SetInSsQueryHandler:%d\n", sInSsQueryHandler);
    sInSsQueryHandler = inHandler;
}

//******************************************************************************
//
// Function Name: KRIL_GetInSsQueryHandler
//
// Description:   Get the flag to check if handling a
//                BRCM_RIL_REQUEST_QUERY_CALL_FORWARD_STATUS,
//                BRCM_RIL_REQUEST_GET_CLIR, or BRCM_RIL_REQUEST_QUERY_CALL_WAITING
//                request.
//
// Notes:
//
//******************************************************************************
Boolean KRIL_GetInSsQueryHandler()
{
    KRIL_DEBUG(DBG_INFO,"InSsQueryHandler:%d\n", sInSsQueryHandler);
    return sInSsQueryHandler;
}

//******************************************************************************
//
// Function Name: KRIL_SetServiceClassValue
//
// Description:   Set Service Class Value temporarily.
//
// Notes:
//
//******************************************************************************
void KRIL_SetServiceClassValue(int ss_class)
{
    KRIL_DEBUG(DBG_INFO,"ss_class:%d\n", ss_class);
    sServiceClass = ss_class;
}

//******************************************************************************
//
// Function Name: KRIL_GetServiceClassValue
//
// Description:   Get temporary service class Value.
//
// Notes:
//
//******************************************************************************
int KRIL_GetServiceClassValue()
{
    KRIL_DEBUG(DBG_INFO,"sServiceClass:%d\n", sServiceClass);
    return sServiceClass;
}

//******************************************************************************
//
// Function Name: KRIL_SetInSetPrefNetworkTypeHandler
//
//******************************************************************************
void KRIL_SetInSetPrefNetworkTypeHandler( Boolean inHandler )
{
    KRIL_DEBUG(DBG_INFO,"inHandler:%s\n", (inHandler?"TRUE":"FALSE") );
    sInSetPrefNetworkTypeHandler = inHandler;
    //when leave the SetPrferredNetworkTypeHandler(), need to check whether need to run the handler in work_queue.
    if (FALSE == inHandler && TRUE == KRIL_GetIsNeedSetPreferNetworkType()) 
    {
        KRIL_SetIsNeedSetPreferNetworkType(FALSE);
        KRIL_CmdQueueWork();
    }
}

//******************************************************************************
//
// Function Name: KRIL_GetInSetPrefNetworkTypeHandler
//
//******************************************************************************
Boolean KRIL_GetInSetPrefNetworkTypeHandler()
{
    return sInSetPrefNetworkTypeHandler;
}

//******************************************************************************
//
// Function Name: KRIL_SetIsNeedSetPreferNetworkType
//
// Description:   If need to trigger work_queue to run the preferred netwrok Type handler, set the state TRUE.
//
// Notes:
//
//******************************************************************************
void KRIL_SetIsNeedSetPreferNetworkType(Boolean state)
{
    KRIL_DEBUG(DBG_INFO,"sNeedSetPreferredNetworkType:%d state:%d\n", sNeedSetPreferredNetworkType, state);
    sNeedSetPreferredNetworkType = state;
}

//******************************************************************************
//
// Function Name: KRIL_GetIsNeedSetPreferNetworkType
//
// Description:   Get is need to trigger work_queue to run the preferred netwrok Type handler
//
// Notes:
//
//******************************************************************************
Boolean KRIL_GetIsNeedSetPreferNetworkType(void)
{
    KRIL_DEBUG(DBG_INFO,"sNeedSetPreferredNetworkType:%d\n", sNeedSetPreferredNetworkType);
    return sNeedSetPreferredNetworkType;
}


//******************************************************************************
//
// Function Name: KRIL_SetPreferredNetworkType
//
// Description:   Set Preferred Network Type.
//
// Notes:  int PreferredNetworkType
//            0 for GSM/WCDMA (WCDMA preferred)
//            1 for GSM only
//            2 for WCDMA only
//            3 for GSM/WCDMA (auto mode, according to PRL)
//            4 for CDMA and EvDo (auto mode, according to PRL)
//            5 for CDMA only
//            6 for EvDo only
//            7 for GSM/WCDMA, CDMA, and EvDo (auto mode, according to PRL)
//******************************************************************************
Boolean KRIL_SetPreferredNetworkType(int PreferredNetworkType)
{
    Boolean state = FALSE;
    KRIL_DEBUG(DBG_INFO,"PreferredNetworkType:%d\n", PreferredNetworkType);
    if (PreferredNetworkType >= 0 && PreferredNetworkType <= 2)
    {
        sPreferredNetworkType = PreferredNetworkType;
        state = TRUE;
    }
    return state;
}

//******************************************************************************
//
// Function Name: KRIL_GetPreferredNetworkType
//
// Description:   Get Preferred Network Type.
//
// Notes:
//
//******************************************************************************
int KRIL_GetPreferredNetworkType(void)
{
    KRIL_DEBUG(DBG_INFO,"sPreferredNetworkType:%d\n", sPreferredNetworkType);
    return sPreferredNetworkType;
}

//******************************************************************************
//
// Function Name: KRIL_SetBandMode
//
// Description:   Save last band mode requested by Android.
//
//******************************************************************************
Boolean KRIL_SetBandMode(int BandMode)
{
    KRIL_DEBUG(DBG_INFO,"KRIL_SetBandMode:%d\n", BandMode);
    sBandMode = BandMode;
    return TRUE;
}

//******************************************************************************
//
// Function Name: KRIL_GetBandMode
//
// Description:   Get current band mode.
//
// Notes:
//
//******************************************************************************
int KRIL_GetBandMode(void)
{
    KRIL_DEBUG(DBG_INFO,"KRIL_GetBandMode:%d\n", sBandMode);
    return sBandMode;
}

//******************************************************************************
//
// Function Name: KRIL_SetLocationUpdateStatus
//
// Description:   Set Location Update Status.
//
// Notes:  1:enable ; Creg = 2
//         0:disable ; Creg = 1
//
//******************************************************************************
void KRIL_SetLocationUpdateStatus(int LocationUpdateStatus)
{
    KRIL_DEBUG(DBG_INFO,"LocationUpdateStatus:%d\n", LocationUpdateStatus);
    sLocationUpdateStatus = LocationUpdateStatus;
}

//******************************************************************************
//
// Function Name: KRIL_GetLocationUpdateStatus
//
// Description:   Get Location Update Status.
//
// Notes:
//
//******************************************************************************
int KRIL_GetLocationUpdateStatus(void)
{
    KRIL_DEBUG(DBG_INFO,"sLocationUpdateStatus:%d\n", sLocationUpdateStatus);
    return sLocationUpdateStatus;
}

//******************************************************************************
//
// Function Name: KRIL_SetRestrictedState
//
// Description:   Set Restricted State.
//
// Notes:
//
//******************************************************************************
void KRIL_SetRestrictedState(int RestrictedState)
{
    KRIL_DEBUG(DBG_INFO,"RestrictedState:%d\n", RestrictedState);
    sRestrictedState = RestrictedState;
}

//******************************************************************************
//
// Function Name: KRIL_GetRestrictedState
//
// Description:   Get Restricted State.
//
// Notes:
//
//******************************************************************************
int KRIL_GetRestrictedState(void)
{
    KRIL_DEBUG(DBG_INFO,"sRestrictedState:%d\n", sRestrictedState);
    return sRestrictedState;
}

//******************************************************************************
//
// Function Name:      HexDataToHexStr
//
// Description:   Convert hex data array (two hex digits occupy one byte) to
//          hex string (double size).
//******************************************************************************
static const char HexDataToHexStrTab[] = 
  {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void HexDataToHexStr(char *HexString, const UInt8 *HexData, UInt16 length)
{
    UInt16 i = 0;

    while (i < length)
    {
        HexString[2*i]     = HexDataToHexStrTab[HexData[i] >> 4];
        HexString[2*i + 1] = HexDataToHexStrTab[HexData[i] & 0x0F];
        i++;
    }

    HexString[2*length] = '\0';
}

//******************************************************************************
//
// Function Name:      RawDataPrintfun
//
// Description:   Print Raw data in hex format. This is just for debug.
//                
//******************************************************************************
void RawDataPrintfun(UInt8* rawdata, UInt16 datalen, char* showstr)
{
    UInt8  *pdata = rawdata;
    UInt8  array[32];
    
    KRIL_DEBUG(DBG_INFO,"%s: datalen:%d\n", showstr, datalen);
    memset(array, 0, 32);
    
    while (datalen > 0)
    {
        if (datalen >= 32)
        {
            KRIL_DEBUG(DBG_INFO,"%s: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",showstr,pdata[0],pdata[1],pdata[2],pdata[3],
            pdata[4],pdata[5],pdata[6],pdata[7],pdata[8],pdata[9],pdata[10],pdata[11],pdata[12],pdata[13],pdata[14],pdata[15],pdata[16],pdata[17],pdata[18],pdata[19],pdata[20],pdata[21],pdata[22],pdata[23],pdata[24],pdata[25],pdata[26],
            pdata[27],pdata[28],pdata[29],pdata[30],pdata[31]);
            
            pdata += 32;
            datalen -= 32;
        }
        else
        {
            memcpy(array, pdata, datalen);
            KRIL_DEBUG(DBG_INFO,"%s: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",showstr,array[0],array[1],array[2],array[3],
            array[4],array[5],array[6],array[7],array[8],array[9],array[10],array[11],array[12],array[13],array[14],array[15],array[16],array[17],array[18],array[19],array[20],array[21],array[22],array[23],array[24],array[25],array[26],
            array[27],array[28],array[29],array[30],array[31]);
            
            datalen = 0;
        }
    }
}


//******************************************************************************
//
// Function Name: KRIL_OemHookRawHandler
//
// Description:  Switch OEM Hook message.
//
// Notes:
//
//******************************************************************************
void KRIL_OemHookRawHandler(void *ril_cmd, Kril_CAPI2Info_t *capi2_rsp)
{

#ifndef BCM_RIL_FOR_EAP_SIM  
	KRIL_SRIL_OemHookRawHandler(ril_cmd, capi2_rsp);
#else
    KRIL_CmdList_t *pdata = (KRIL_CmdList_t*)ril_cmd;
    char* rawdata = NULL;
    
    rawdata = (char*)pdata->ril_cmd->data;
    
    KRIL_DEBUG(DBG_ERROR,"KRIL_OemHookRawHandler%d %d %d %d msgid:%d\n",rawdata[0],rawdata[1],rawdata[2],rawdata[3], rawdata[4]);
	
    if (rawdata[0] == 'B' &&
        rawdata[1] == 'R' &&
        rawdata[2] == 'C' &&
        rawdata[3] == 'M')
    {
        UInt8 msgid;
        
        msgid = (UInt8)rawdata[4];
        KRIL_DEBUG(DBG_INFO,"msgid:%d\n", msgid);
        
        switch (msgid)
        {
#if 0 
            //case BRIL_HOOK_SET_PREFDATA:
            //   break;
               
            case BRIL_HOOK_QUERY_SIM_PIN_REMAINING:
               KRIL_QuerySimPinRemainingHandler(ril_cmd, capi2_rsp);
               break;
           
            case BRIL_HOOK_GET_SIM_VOLTAGE:
                KRIL_GetCurrentSimVoltageHandler(ril_cmd, capi2_rsp);
                break;
#endif           
            case BRIL_HOOK_GENERIC_SIM_ACCESS:
                KRIL_GenericSimAccessHandler(ril_cmd, capi2_rsp);
                break;
           
	// BCM_EAP_SIM
            case BRIL_HOOK_EAP_SIM_AUTHENTICATION:
	           KRIL_DEBUG(DBG_ERROR,"BRIL_HOOK_EAP_SIM_AUTHENTICATION\n");//For test	
   	           KRIL_GsmSimAuthenticationHandler(ril_cmd, capi2_rsp);
            break;
			
            case BRIL_HOOK_EAP_AKA_AUTHENTICATION:
    	           KRIL_DEBUG(DBG_ERROR,"BRIL_HOOK_EAP_AKA_AUTHENTICATION\n");//For test
     		   KRIL_USimAuthenticationHandler(ril_cmd, capi2_rsp);
            break;         
				
            default:
               KRIL_DEBUG(DBG_ERROR,"Unsupported msgtype:%d Error!!!\n", msgid);
               pdata->handler_state = BCM_ErrorCAPI2Cmd;
               break;
        }
    }
    else
    {
		KRIL_SRIL_OemHookRawHandler(ril_cmd, capi2_rsp);
    }    
	
#endif    
}
