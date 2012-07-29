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
*   @file   sim_3g.h
*
*   @brief  This file contains definitions for USIM-specific prototypes.
*
****************************************************************************/

#ifndef _SIM_3G_H_
#define _SIM_3G_H_

   


//-------------------------------------------------
// Constant Definitions
//-------------------------------------------------

/* Definitions used in FCP (File Control Parameter) TLV. See Section 11.1.1.4 of 31.101 */
#define FCP_TEMPLATE_TAG		0x62
#define FILE_SIZE_TAG			0x80
#define FILE_DESCRIPTOR_TAG		0x82
#define FILE_IDENTIFICATION_TAG 0x83
#define LIFE_CYCLE_STATUS_TAG	0x8A

//-------------------------------------------------
// Data Structure
//-------------------------------------------------

/* USIM EF-EST data: USIM Enabled Service Table */
#define MAX_EST_LEN	1

typedef struct {

    UInt8 Est_Data_Len;
    UInt8 Est_Data[MAX_EST_LEN];

    /* We need to cache the EF-ARR record data so that we can return the EF Access Conditions
    * stored in these records in the response to SIM_SendEFileInfoReq() API function if
    * the access conditions are stored in EF-ARR using Tag = 0x8B (Reference to Expanded Format).
    * See Section 9.2 and Section 11.1.1.4.7 of 3GPP 31.101.
    * 
    * There are three EF-ARR files in USIM. 
    * EF-ARR under MF; EF-ARR under MF/ADF-USIM; EF-ARR under MF/DF-Telecom. 
    */

	/* Security status error, e.g. SW1/SW2=0x69/0x82 received when reading EF-ARR records for some Orange SIM's */ 
	Boolean Arr_Security_Status_Error; 


    /* Number of records in EF-ARR */
    UInt8 Arr_Under_Mf_Rec_Num;
    UInt8 Arr_Under_Adf_Rec_Num;
    UInt8 Arr_Under_Telecom_Rec_Num;

    /* Record length in EF-ARR */
    UInt8 Arr_Under_Mf_Rec_Len;
    UInt8 Arr_Under_Adf_Rec_Len;
    UInt8 Arr_Under_Telecom_Rec_Len;

    /* The following memory will be malloc-ed to store EF-ARR records */
    UInt8 *Arr_Under_Mf_Rec_Data;
    UInt8 *Arr_Under_Adf_Rec_Data;
    UInt8 *Arr_Under_Telecom_Rec_Data;

    /* APN Control List (ACL) Data */
    UInt8 *Acl_Data;
    UInt16 Acl_Data_Len;

    /* EF-RAT data for AT&T RAT Balancing feature */
    UInt8 Rat_Mode_Data;

    /* USIM application list obtained from EF-DIR records */
    T_USIMAP_APP_LIST Usim_Appli_List;

} SIM_3G_STRUCT_t;

//---------------------------------------------------------------
// enum
//---------------------------------------------------------------


//-------------------------------------------------
// Function Prototype
//-------------------------------------------------

SIM_3G_STRUCT_t* get3gStructPtr(void);

//***************************************************************************************
/**
    This function decodes the information returned for Selecting an USIM EF according to 
	Section 11.1.1.3.2 of 3GPP 31.101. 

    @param socket_id (in) Channel ID on which the Select response is received
	@param efile_data (in) Response for the file selection
	@param efile_info (out) EF configuration structure
	@param file_path (in) Selection path for the file
	@param path_len (in) Length of the selection path for the file
**/	
void USIM_DecodeEfInfo(UInt8 socket_id, UInt8 *efile_data, SIM_EFILE_INFO_t *efile_info, UInt16 *file_path, UInt8 path_len);


//***************************************************************************************
/**
    This function reads the EF-ARR and EF-UST data in USIM to SRAM. It shall be called 
	before any other USIM files are read. 
**/	
void USIM_InitBasicSimData(void);


//***************************************************************************************
/**
    This function reads the USIM-specific SIM files into RAM. It shall be called only if 
	a USIM is inserted and after USIM_InitBasicSimData() is called. 

	@return		TRUE if ISIM supported; FALSE otherwise. 
**/	
void USIM_InitCachedSimData(void);


//***************************************************************************************
/**
    This function processes the request to activate/deactivate a service in USIM EF-EST. 
	It is to be called inside the SIM Process task. 

    @param msg (in) Intertask message for the request to activate/deactivate a service 
					in USIM EF-EST
**/	
void USIM_ProcSetEstServReq(const InterTaskMsg_t *msg);


//***************************************************************************************
/**
    This function processes the request to update one APN name in EF-ACL in USIM.  

    @param msg (in) Intertask message to update one APN name in EF-ACL in USIM.
**/	
void USIM_ProcUpdateOneApnReq(const InterTaskMsg_t *msg);


//***************************************************************************************
/**
    This function processes the request to delete all APN names in EF-ACL in USIM. 

	 @param msg (in) Intertask message to delete all APN names in EF-ACL in USIM. 
**/	
void USIM_ProcDeleteAllApnReq(const InterTaskMsg_t *msg);


//***************************************************************************************
/**
    This function processes the responses from the USIM for updating the EF-EST service bits. 

    @param sim_access (in) SIM access result for updating the EF-EST.
**/	
void USIM_HandleEstUpdateStatus(SIMAccess_t sim_access);


//***************************************************************************************
/**
    This function processes the responses from the USIM for updating one APN name in EF-ACL. 

	@param sim_access (in) SIM access result for updating the EF-ACL. 
**/	
void USIM_HandleUpdateOneApnStatus(SIMAccess_t sim_access);


//***************************************************************************************
/**
    This function returns TRUE if the inserted SIM/USIM supports ISIM feature
	and Sys Parm indicates we support ISIM. Note that ISIM can be supported only on USIM.

	@return		TRUE if ISIM supported; FALSE otherwise. 
**/	
void USIM_HandleDeleteAllApnStatus(SIMAccess_t sim_access);


//***************************************************************************************
/**
    This function initializes the EF-RAT data for AT&T RAT balancing feature and caches
	the RAT mode data in SRAM.
**/	
void USIM_InitRat(void);


//***************************************************************************************
/**
     This function processes the USIM Application list which is obtained from EF-DIR records 
	 and stores it in cache.

    @param appli_list (in) EF-DIR record data which stores the application data
**/	
void USIM_HandleSupportAppli(const T_USIMAP_APP_LIST *appli_list);


//***************************************************************************************
/**
    This function returns the AID data for an application in USIM. The returned AID data 
	can be used to activate the application. 

    @param appli_type (in) Application type, i.e. USIM, ISIM or WSIM.
	@param aid_data (out)  - Pointer to return the AID data pointer. 
	@param aid_len (out) - The length of the returned AID data.
**/	
void USIM_GetAppliAidData(USIM_APPLICATION_TYPE appli_type, const UInt8 **aid_data, UInt8 *aid_len);


//***************************************************************************************
/**
    This function returns application type corresponding to the passed AID data.  

	@param aid_data (in) AID data
	@param aid_len (in) Length of the AID data

    @return Application type the AID data indicates.
**/	
USIM_APPLICATION_TYPE USIM_GetAppliType(const UInt8 *aid_data, UInt8 aid_len);


//***************************************************************************************
/**
    This function returns the USIM Service Table contents and its size in EF-UST.
	
	@param size (out) Size of EF-UST data
	
	@return Poiner to the EF-UST data (no need for client to delete memory).
**/	
const UInt8 *USIM_GetServiceTable(UInt16 *size);


#endif  

