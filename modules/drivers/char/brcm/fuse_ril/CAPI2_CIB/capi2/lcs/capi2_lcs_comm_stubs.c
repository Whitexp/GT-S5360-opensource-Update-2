/*********************************************************************
*
* Copyright 2010 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license agreement
* governing use of this software, this software is licensed to you under the
* terms of the GNU General Public License version 2, available at
* http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
***************************************************************************/
/**
*
*   @file   capi2_lcs_comm_stubs.c
*
*   @brief  This file implements the CAPI2 LCS API on the comm side.
*
****************************************************************************/
#include "mobcom_types.h"
#include "resultcode.h"
#include "taskmsgs.h"
#include "consts.h"

#include "xdr_porting_layer.h"
#include "xdr.h"

#include "common_defs.h"
#include "uelbs_api.h"
#include "ms_database_def.h"
#include "common_sim.h"
#include "sim_def.h"
#ifndef UNDER_LINUX
#include <string.h>
#endif
#include "assert.h"
#include "sysparm.h"
#include "engmode_api.h"
#include "sysparm.h"
///
#include "i2c_drv.h"
#include "ecdc.h"
#include "uelbs_api.h"
#include "common_sim.h"
#include "sim_def.h"
#include "stk_def.h"
#include "mti_trace.h"
#include "logapi.h"
#include "log.h"
#include "tones_def.h"
#include "phonebk_def.h"
#include "phonectrl_def.h"
#include "phonectrl_api.h"
#include "rtc.h"
#include "netreg_def.h"
#include "ms_database_old.h"
#include "ms_database_api.h"
#include "netreg_util_old.h"
#include "netreg_util.h"
#include "netreg_api.h"
#include "common_sim.h"
#include "sim_def.h"
#include "stk_def.h"
#include "ss_def.h"
#include "sim_api.h"
#include "phonectrl_def.h"
#include "isim_def.h"
#include "ostypes.h"
#include "pch_def.h"
#include "pchex_def.h"
#include "hal_em_battmgr.h"
#include "cc_def.h"
#include "rtem_events.h"
#include "rtc.h"
#include "engmode_def.h"
#include "sms_def.h"
#include "simlock_def.h"
#include "isim_def.h"
#include "pch_def.h"
#include "pchex_def.h"

#include "engmode_api.h"
#include "sysparm.h"
#include "ms_database_old.h"
#include "ms_database_api.h"
#include "ss_api.h"
#include "sms_api_atc.h"
#include "sms_api_old.h"
#include "sms_api.h"
#include "cc_api_old.h"
#include "sim_api_old.h"
#include "sim_api.h"
#include "phonebk_api_old.h"
#include "phonebk_api.h"
#include "phonectrl_api.h"
#include "isim_api_old.h"
#include "isim_api.h"

#include "util_api_old.h"
#include "util_api.h"
#include "dialstr_api.h"
#include "stk_api_old.h"
#include "stk_api.h"

#include "pch_api_old.h"
#include "pch_api.h"
#include "pchex_api.h"
#include "ss_api_old.h"
#include "lcs_cplane_rrlp_api.h"
#include "cc_api.h"
#include "netreg_api.h"
#include "lcs_ftt_api.h"
#include "lcs_cplane_rrc_api.h"
#include "lcs_cplane_shared_def.h"

#include "capi2_mstruct.h"
#include "capi2_sim_api.h"
#include "capi2_phonectrl_api.h"
#include "capi2_sms_api.h"
#include "capi2_cc_api.h"
#include "capi2_lcs_cplane_api.h"
#include "capi2_ss_api.h"
#include "capi2_phonebk_api.h"
#include "capi2_cmd_resp.h"
#include "capi2_phonectrl_api.h"
//#include "capi2_gen_api.h"	

#include "ipcproperties.h"
#include "rpc_global.h"
#include "rpc_ipc.h"
#include "xdr_porting_layer.h"
#include "xdr.h"
#include "rpc_api.h"

#include "capi2_global.h"
#include "capi2_mstruct.h"
#include "capi2_cc_ds.h"
#include "capi2_cc_msg.h"
#include "capi2_msnu.h"
#include "ss_api_old.h"
#include "ss_lcs_def.h"
#include "capi2_ss_msg.h"
#include "capi2_cp_socket.h"
#include "capi2_cp_msg.h"
#include "capi2_pch_msg.h"
#include "capi2_sms_msg.h"
#include "capi2_phonectrl_api.h"
#include "capi2_phonectrl_msg.h"
#include "capi2_isim_msg.h"
#include "capi2_sim_msg.h"
#include "capi2_ss_msg.h"
#include "capi2_stk_ds.h"
#include "capi2_stk_msg.h"
#include "lcs_cplane_api.h"
#include "capi2_lcs_msg.h"
#include "lcs_cplane_rrc_api.h"
#include "lcs_cplane_shared_def.h"
#include "lcs_ftt_api.h"
#include "capi2_phonebk_msg.h"
#include "capi2_lcs_cplane_msg.h"
#define MAX_CC_GET_ELEM_SIZE  64
#include "capi2_gen_msg.h"

#include "capi2_reqrep.h"

#include "capi2_comm.h"
#include "capi2_gen_api.h"


#ifndef GPS_INCLUDED 

Result_t LCS_FttSyncReq(ClientInfo_t* inClientInfoPtr)
{
	return RESULT_ERROR;
}

//*******************************************************************************
/**
*	Calculate the time difference in micro second between the provided two FTT parameters. 
*
*	@param inT1	(in) The first FTT parameter.
*	@param inT2	(in) The second FTT parameter.
*
*	@return The time difference in micro second between the provided two FTT parameters. 
*
*******************************************************************************/
UInt32 LCS_FttCalcDeltaTime( const LcsFttParams_t * inT1, const LcsFttParams_t* inT2)
{
	return 0;
}

Boolean LCS_L1_bb_isLocked( Boolean inStartend )
{
	return FALSE;
}

Result_t LcsApi_RrlpRegisterDataHandler(ClientInfo_t* inClientInfoPtr)
{
	return RESULT_ERROR;
}

Result_t LcsApi_RrlpSendDataToNetwork(ClientInfo_t* inClientInfoPtr, const UInt8 *inData, UInt32 inDataLen)
{
	return RESULT_ERROR;
}


Result_t LcsApi_RrcRegisterDataHandler(ClientInfo_t* inClientInfoPtr)
{
	return RESULT_ERROR;
}

Result_t LcsApi_RrcSendUlDcch(ClientInfo_t* inClientInfoPtr, UInt8* inData, UInt32 inDataLen)
{
	return RESULT_ERROR;
}

Result_t LcsApi_RrcMeasCtrlFailure(ClientInfo_t* inClientInfoPtr, UInt16 inTransactionId, 
								   LcsRrcMcFailure_t inMcFailure, UInt32 inErrorCode)
{
	return RESULT_ERROR;
}

Result_t LcsApi_RrcStatus(ClientInfo_t* inClientInfoPtr, LcsRrcMcStatus_t inStatus)
{
	return RESULT_ERROR;
}

#endif


