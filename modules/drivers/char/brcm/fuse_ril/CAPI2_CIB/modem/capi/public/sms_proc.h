/****************************************************************************
*
*     Copyright (c) 2007-2008 Broadcom Corporation
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
/**
*
*   @file   sms_proc.h
*
*   @brief  This file defines some process interface for local SMS API.
*
****************************************************************************/

#ifndef _SMS_PROC_H_
#define _SMS_PROC_H_
   

#ifndef _WIN32
#ifndef max
#define max(x,y)   ((x) > (y) ? (x) : (y))
#endif
#endif

#define CB_DATA_PER_BLOCK_SZ	22		///< CB Data size from 2nd ~ 4th block
#define FILL_BYTE 0x2B					///< CB Fill BYTE

typedef enum {
	CBMID_CACHE_NOT_READY,
	CBMID_CACHE_READY,
	CBMID_NOT_EXIST
} CBMidsDataStatus_t;

/**
 * @addtogroup SMSAPIGroup
 * @{
 */
//**************************************************************************************
/**
	Initialize short message service module.
**/
void  SMS_APIInit(void);


//-------------------------------------------------
//  Function Prototype
//-------------------------------------------------

/** @} */


// default setting API
SmsTxParam_t* SMS_GetDefaultTxParamPtr( void );
SmsRxSettings_t* SMS_GetDefRxSettings(void);
smsBearerDb_t* SMS_GetBearerDbPtr(void);
SmsPrefProfile_t* SMS_GetSmsPrefProfilePtr( void );



void  SMS_APIInitWithFS(void);

// storage API
void SMS_BufferInit ( void );
//void SMS_SimInit ( void );

SmsStorageWaitState_t SMS_GetStorageWaitState(void);
void SMS_SetSMSAccessStorage(SmsStorage_t storageType);
SmsStorage_t SMS_GetSMSAccessStorage(void);

SmsMesgStatus_t SMS_GetNonSimSmsStoredState (SmsStorage_t storageType, UInt16 index);


Boolean SMS_IsMeStorageOverFlow (void);
Boolean SMS_IsSimStorageOverFlow (void);

void SMS_resetCachedDataReady(void);

void SMS_SetLastCmssSlotIndex(UInt16 index);


void SMS_SmsBufferStatus  (SmsStorage_t			storageType,
                           UInt16				*NbFree,
                           UInt16				*NbUsed );

void SMS_StoreSmsInBuffer (UInt8				clientId,
						   SmsStorage_t			storageType,
						   void					*sms,
						   UInt16				size,
						   UInt16				recNum,
						   SIMSMSMesgStatus_t	smsState,
						   UInt8				tp_pid);

void SMS_GetSmsFromBuffer (UInt8				clientId,
						   SmsStorage_t			storageType,
                           UInt16				index);

void SMS_RmvSmsFromBuffer (UInt8				clientId,
						   SmsStorage_t			storageType,
                           UInt16				index );

void SMS_SimSmsChangeStatusReq(UInt8			clientId,
						   SmsStorage_t			storageType,
                           UInt16				index,
                           SIMSMSMesgStatus_t	status);


// SMS utilities

UInt8 SMS_DecTime(SmsVpf_t VPf, SmsTime_t* Dst, Sms_340Time_t* Src);
UInt16 SMS_CodTime(SmsVpf_t *VPf, UInt8 Fo, Sms_340Time_t *Dst, SmsTime_t *Src);
void SMS_GetUserDataPtr (SmsTpdu_t *DstSms, UInt8 SrcUdl, UInt8 * SrcTpUd,      
                         UInt8 Udhi, Boolean Compression, SmsAlphabet_t Alphabet);

UInt8 SMS_FoToMnsmsVpf(UInt8 Fo);
UInt8 SMS_MnsmsVpfToVpf (UInt8 MnsmsVpf);

UInt8 SMS_ConvertSmsDeliverToStorageType(UInt8 * Dst, T_MN_TP_DELIVER_IND *DeliverInd);
UInt8 SMS_ConvertSmsSubmitToStorageType(UInt8 * Dst,T_MN_TP_SUBMIT_REQ  *SubmitReq);
UInt8 SMS_ConvertSmsSRToStorageType(UInt8 * Dst, T_MN_TP_STATUS_REPORT_IND *StatusReportInd);


// SMS processing
void SMS_HandleSmsStatusRptInd(InterTaskMsg_t* inMsg);
void SMS_HandleSmsDeliverInd(InterTaskMsg_t* inMsg);

Result_t SMS_SubmitSmsPduReq(ClientInfo_t* inClientInfoPtr, UInt8 length, UInt8* inSmsPdu, Sms_411Addr_t* sca);
Result_t SMS_SubmitSmsReq(ClientInfo_t* inClientInfoPtr, UInt8* inNum, UInt8* inSMS, SmsTxParam_t* params, UInt8* sca);
Boolean SMS_CheckSmsLength(UInt8 udl, SmsAlphabet_t msgAlphabet);

// SMS Command
Result_t SMS_SendSmsCommandFromTextMsg(ClientInfo_t* inClientInfoPtr, SmsCommand_t smsCmd, UInt8* inNum, UInt8* inCmdTxt, Sms_411Addr_t* inSca);
Result_t SMS_SendSmsCommandFromPduMsg(ClientInfo_t* inClientInfoPtr, UInt8 length, UInt8* inSmsPduCmd);

/**
 * @addtogroup SMSAPIGroup
 * @{
 */

/**
    API function for MMI to move a SMS from ME to SIM
	@param		clientID (in) client ID
	@param		size (in) msg PDU size
	@param		*Sms (in) msg data
	@param		smsState (in) msg status
**/	
void SMS_WriteSmsPduToSim(UInt8 clientID, UInt16 size, void* Sms, SIMSMSMesgStatus_t smsState);

/** @} */

void SMS_HandleSendMesgToStorage(InterTaskMsg_t* inMsg);

UInt16 SMS_FindNextMsg(SmsStorage_t storeType, SmsMesgStatus_t msgBox, UInt16 lastIndex);
Boolean SMS_FormatSmsForClient(SmsSimMsg_t *smsMsg, SIM_SMS_DATA_t *sms_data, SrType_t sr_type);

Boolean SMS_GetSimCachedDataReadyStatus(void);

void SMS_GetVoiceMailInfo(SmsVoicemailInd_t *vm_ind);

Boolean SMS_IsVoiceMailIndEnabled(void);

void SMS_SetVoiceMailIndFlag(Boolean enabled);

Boolean SMS_IsVoiceMailFowarded(void);

void SMS_SetVoiceMailForwardedFlag(Boolean forwarded);

CBMidsDataStatus_t GetCBMidsDataReadyStatus(void);
SMS_CB_MSG_IDS_t* SMS_GetCBMIdb(void);
MS_T_MN_CB_LANGUAGES* SMS_GetCbLanguageDb(void);
void SMS_ReadCBMIReq(void);
Result_t SMS_UpdateCBMIReq(UInt8* pCBMI, UInt8 cbmiLen);
Result_t SMS_RemoveCBMIReq(UInt8* newCBMI);

void SMS_StartAutoCBReq(UInt16 msgId);
void SMS_StopAutoCBReq(void);

Boolean SMS_IsCBMsgIdStarted(UInt16 msgId);

void SMS_HandleStartCBReq(void);
void SMS_HandleWriteCBMIReq(void);

Result_t SMS_StartCBReq(UInt8* chnlIDs, UInt8* codings);
Result_t SMS_StopCBReq(void);
void MNSMS_SuspendCBFilteringReq(Boolean suspend_filering);
Result_t SMS_AddCBLangReq(UInt8 newLang);
Result_t SMS_RemoveCBLangReq(UInt8* newLang);
void SMS_ResetCbLanguageDb(void);

void SMS_HandleSimSmsTpMrRsp(InterTaskMsg_t* inMsg);
void SMS_HandleSmsSubmitRsp(InterTaskMsg_t* inMsg);
void SMS_HandleMemAvailRsp(InterTaskMsg_t* inMsg);
void SMS_HandleSmsRetryReq(ClientInfo_t* inClientInfoPtr);
void SMS_HandleSmsDeleteResp(InterTaskMsg_t *inMsg);
void SMS_HandleMeSmsDeleteInd(InterTaskMsg_t *inMsg);
void SMS_HandleSmsReadResp(InterTaskMsg_t *inMsg);
void SMS_HandleSimSmsStatusUpdateRsp(InterTaskMsg_t* inMsg);
void SMS_HandleSimSmsMemAvailInd(Boolean mem_avail);
void SMS_HandleIncomingSmsStoredRes(InterTaskMsg_t* inMsg);
void SMS_HandleSmsWriteResp(InterTaskMsg_t* inMsg);
void SMS_HandleSimSmsDataResp(InterTaskMsg_t* inMsg);
void SMS_HandleSmsSendFromSlot(InterTaskMsg_t* inMsg);

void SMS_HandleSimEfileDataResp(InterTaskMsg_t* inMsg);

void SMS_HandleSimScaUpdResp(void);

void SMS_HandleCBDataInd(InterTaskMsg_t* inMsg);
void SMS_HandleCBStartStopRsp(InterTaskMsg_t* inMsg);
UInt8 SMS_GetCBMIRecLength(void);

Boolean SMS_IsMemAvailRspPending(void);
void SMS_SetPendingflagForMemAvailRsp(Boolean flag);

// SATK SMS control
void SMS_HandleMOSMS_ControlRsp(InterTaskMsg_t* inMsg);


void SMS_HandleMsgWaitingIndReadRsp(InterTaskMsg_t *inMsg);



void SMS_HandleSmsCommandRsp(InterTaskMsg_t* inMsg);

// transaction management prototype
UInt8 SMS_GetClientIDFromTransaction(SmsTransactionType_t smsTrans, ClientInfo_t* inClientInfoPtr);

void  SMS_ResetTransactionClientID(UInt8 clientID);

SmsTransactionType_t SMS_GetTransactionFromDialogId(UInt32 dialogId, SmsMesgStatus_t* category, 
													SmsStorage_t* storageType, ClientInfo_t* inClientInfoPtr);

SmsTransactionType_t SMS_GetTransactionFromClientID(UInt8 clientID, SmsMesgStatus_t* category, 
													SmsStorage_t* storageType, ClientInfo_t* inClientInfoPtr);

SMS_BEARER_PREFERENCE_t  SMS_GetSMSPrefBearer(void);
void  SMS_SetSMSPrefBearer(SMS_BEARER_PREFERENCE_t inPref);

void SMS_SetAckTestCase(Boolean flag);

void SMS_HandleSmsSubmitVccTimeOutRsp(InterTaskMsg_t* inMsg);

#endif	// _SMS_PROC_H_

