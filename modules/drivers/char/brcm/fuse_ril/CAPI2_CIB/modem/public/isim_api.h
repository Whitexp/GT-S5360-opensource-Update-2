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
*   @file   isim_api.h
*
*   @brief  This file defines the ISIM-specifc API funtion prototypes.
*
****************************************************************************/
/**

	@defgroup   ISIMAPIGroup   IP Multi-media Service SIM (ISIM) API
    @ingroup    SIMSTK
 
 	Typically the IMS (IP Multimedia Systems) client shall call functions in this file
	in the following order to access ISIM features:

  1. After receiving the #MSG_SIM_CACHED_DATA_READY_IND indication, the client can 
     call ISIM_IsIsimSupported() function to check whether ISIM is supported 
	   in the SysParm and USIM. 

  2. If ISIM_IsIsimSupported() indicates ISIM is supported, the client can call ISIM_ActivateIsimAppli()
     to activate ISIM application in USIM. The response message #MSG_ISIM_ACTIVATE_RSP will return
	   a Socket_ID associated with the ISIM application if ISIM is successfully activated. 

  3. After ISIM application is successfully activated, ISIM_IsIsimActivated() can be called 
     to confirm ISIM application is activated and also to get the Socket_ID associated with the ISIM application. 

  4. The client can then use the ISIM Socket_ID to call the following Generic SIM Access fucntions to 
	   read the data in ISIM EF-IMPI, EF-IMPU, EF-DOMAIN, EF-IST and EF-PCSCF:\n
		SIM_SubmitDFileInfoReq();\n
		SIM_SubmitEFileInfoReq();\n
		SIM_SubmitWholeBinaryEFileReadReq();\n
		SIM_SubmitBinaryEFileReadReq();\n
		SIM_SubmitRecordEFileReadReq();\n
		SIM_SubmitBinaryEFileUpdateReq();\n
		SIM_SubmitLinearEFileUpdateReq();\n
		SIM_SubmitCyclicEFileUpdateReq();\n

  5. The client can also call one of the following functions to perform ISIM authentication:\n
	    ISIM_SendAuthenAkaReq();\n
	    ISIM_SendAuthenHttpReq();\n
	    ISIM_SendAuthenGbaBootReq();\n
	    ISIM_SendAuthenGbaNafReq();\n

  6. Before and after ISIM application is activated, the client can use the following Generic SIM Access functions 
     to read/write USIM files:\n
	    SIM_SendDFileInfoReq() - equivalent to SIM_SubmitDFileInfoReq() with Socket_ID = USIM_BASIC_CHANNEL_SOCKET_ID\n
		SIM_SendEFileInfoReq() - equivalent to SIM_SubmitEFileInfoReq() with Socket_ID = USIM_BASIC_CHANNEL_SOCKET_ID\n
		SIM_SendWholeBinaryEFileReadReq() - equivalent to SIM_SubmitWholeBinaryEFileReadReq() with Socket_ID = USIM_BASIC_CHANNEL_SOCKET_ID\n
		SIM_SendBinaryEFileReadReq() - equivalent to SIM_SubmitBinaryEFileReadReq() with Socket_ID = USIM_BASIC_CHANNEL_SOCKET_ID\n
		SIM_SendRecordEFileReadReq() - equivalent to SIM_SubmitRecordEFileReadReq() with Socket_ID = USIM_BASIC_CHANNEL_SOCKET_ID\n
		SIM_SendBinaryEFileUpdateReq() - equivalent to SIM_SubmitBinaryEFileUpdateReq() with Socket_ID = USIM_BASIC_CHANNEL_SOCKET_ID\n
		SIM_SendLinearEFileUpdateReq() - equivalent to SIM_SubmitLinearEFileUpdateReq() with Socket_ID = USIM_BASIC_CHANNEL_SOCKET_ID\n
		SIM_SendCyclicEFileUpdateReq() - equivalent to SIM_SubmitCyclicEFileUpdateReq() with Socket_ID = USIM_BASIC_CHANNEL_SOCKET_ID\n

  7. See below for examples on how to use the ISIM API functions.
  \dontinclude at_sim_test.c
  \skip // Function Name:	testIsim
  \until return at_status;
 
*/
#ifndef _ISIM_API_H_
#define _ISIM_API_H_
   

/**
 * @addtogroup ISIMAPIGroup
 * @{
*/

//***************************************************************************************
/**
    This function returns TRUE if the inserted SIM/USIM supports ISIM feature
	and Sys Parm indicates we support ISIM. Note that ISIM can be supported only on USIM.

	@param 		inClientInfoPtr (in) pointer to ClientInfo_t data structure.

	@return		TRUE if ISIM supported; FALSE otherwise. 
**/	
Boolean ISimApi_IsIsimSupported(ClientInfo_t* inClientInfoPtr);


//***************************************************************************************
/**
    This function returns TRUE if the ISIM application is activated in the SIM/USIM. 
	If the ISIM application is activated, the socket ID is returned to "socket_id", 
	otherwise 0 is returned to "socket_id".

	@param 		inClientInfoPtr (in) pointer to ClientInfo_t data structure.
	@param 		socket_id (in) Socket ID for the ISIM application; 0 if ISIM is not activated.

	@return		TRUE if ISIM application is activated; FALSE otherwise. 
**/	
Boolean ISimApi_IsIsimActivated(ClientInfo_t* inClientInfoPtr, UInt8 *socket_id);


//***************************************************************************************
/**
    This function activates the ISIM application in the SIM/USIM. 
	This function is applicable only if ISIM_IsIsimSupported() returns TRUE. 

	@param 		inClientInfoPtr (in) pointer to ClientInfo_t data structure.
	@param 		sim_access_cb (in) Callback function
	
	@return		Result_t

	@note		A MSG_ISIM_ACTIVATE_RSP message will be returned.
**/	
Result_t ISimApi_ActivateIsimAppli(ClientInfo_t* inClientInfoPtr, CallbackFunc_t* sim_access_cb);


//***************************************************************************************
/**
    This function sends the Authenticate command for IMS AKA Security Context (see Section
	7.1.2.1 of 3GPP 31.103). A MSG_ISIM_AUTHEN_AKA_RSP message will be returned to the passed
	callback function. 
	This function is applicable only if ISIM_IsIsimSupported() returns TRUE and ISIM application is activated.

	@param 		inClientInfoPtr (in) pointer to ClientInfo_t data structure.	
	@param		rand_data (in) RAND data
	@param		rand_len (in) Number of bytes in RAND data
	@param		autn_data (in) AUTN data
	@param		autn_len (in) Number of bytes in AUTN data
	@param		sim_access_cb (in) Callback function

	@return		Result_t
**/	
Result_t ISimApi_SendAuthenAkaReq( ClientInfo_t* inClientInfoPtr, const UInt8 *rand_data, UInt16 rand_len,
							const UInt8 *autn_data, UInt16 autn_len, CallbackFunc_t* sim_access_cb );


//***************************************************************************************
/**
    This function sends the Authenticate command for HTTP Digest Security Context (see Section
	7.1.2.2 of 3GPP 31.103). A MSG_ISIM_AUTHEN_HTTP_RSP message will be returned to the passed
	callback function. 
	This function is applicable only if ISIM_IsIsimSupported() returns TRUE and ISIM application is activated.

	@param 		inClientInfoPtr (in) pointer to ClientInfo_t data structure.
	@param		realm_data (in) REALM data
	@param		realm_len (in) Number of bytes in REALM data
	@param		nonce_data (in) NONCE data
	@param		nonce_len (in) Number of bytes in NONCE data
	@param		cnonce_data (in) CNONCE data
	@param		cnonce_len (in) Number of bytes in CNONCE data
	@param		sim_access_cb (in) Callback function

	@return		Result_t
**/	
Result_t ISimApi_SendAuthenHttpReq( ClientInfo_t* inClientInfoPtr, const UInt8 *realm_data, UInt16 realm_len,
                                          const UInt8 *nonce_data, UInt16 nonce_len,
                                          const UInt8 *cnonce_data, UInt16 cnonce_len, CallbackFunc_t* sim_access_cb );


//***************************************************************************************
/**
    This function sends the Authenticate command for GBA Security Context in Bootstrapping Mode (see Section
	7.1.2.3 of 3GPP 31.103) to ISIM. A MSG_ISIM_AUTHEN_GBA_BOOT_RSP message will be returned to the passed
	callback function. 
	This function is applicable only if ISIM_IsIsimSupported() returns TRUE. 

	@param 		inClientInfoPtr (in) pointer to ClientInfo_t data structure.
	@param		rand_data (in) RAND data
	@param		rand_len (in) Number of bytes in RAND data
	@param		autn_data (in) AUTN data
	@param		autn_len (in) Number of bytes in AUTN data
	@param		sim_access_cb (in) Callback function

	@return		Result_t
**/	
Result_t ISimApi_SendAuthenGbaBootReq( ClientInfo_t* inClientInfoPtr, const UInt8 *rand_data, UInt16 rand_len,
						const UInt8 *autn_data, UInt16 autn_len, CallbackFunc_t* sim_access_cb );


//***************************************************************************************
/**
    This function sends the Authenticate command for GBA Security Context in NAF Derivation Mode (see Section
	7.1.2.4 of 3GPP 31.103) to ISIM. A MSG_ISIM_AUTHEN_GBA_NAF_RSP message will be returned to the passed
	callback function. 
	This function is applicable only if ISIM_IsIsimSupported() returns TRUE and ISIM application is activated.

		@param 		inClientInfoPtr (in) pointer to ClientInfo_t data structure.
	@param		naf_id_data (in) NAF ID data
	@param		naf_id_len (in) Number of bytes in NAF ID data
	@param		sim_access_cb (in) Callback function

	@return		Result_t
**/	
Result_t ISimApi_SendAuthenGbaNafReq(ClientInfo_t* inClientInfoPtr, const UInt8 *naf_id_data, UInt16 naf_id_len, CallbackFunc_t* sim_access_cb);



/** @} */

#endif  
