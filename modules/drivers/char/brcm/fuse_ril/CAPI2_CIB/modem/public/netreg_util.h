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
*   @file   netreg_util.h
*
*   @brief  This file defines the interface for Network Registration related utility functions
*
****************************************************************************/
/**
*
*   @defgroup   NetRegUtilityGroup Network Registration Utility
*   @ingroup    TelephonyGroup				
*
*   @brief      This group contains the Netwrok Registration Utility functions..
*
****************************************************************************/

#ifndef _NETREG_UTIL_H_
#define _NETREG_UTIL_H_

/**
 * @addtogroup NetRegUtilityGroup
 * @{
 */

//**************************************************************************************
/**
	This function converts the MCC values: 
	For example: from 0x1300 to 0x310 for Pacific Bell.

	@param		mcc (in) UInt16 value used to pass the mcc value to be converted
	@return		UInt16
**/
UInt16	NetRegUtil_PlmnConvertRawMcc(UInt16 mcc);						 
UInt16	MS_PlmnConvertRawMcc(UInt16 mcc);						 

//**************************************************************************************
/**
	Description: This function converts the MNC values. 
	The third digit of MNC may be stored in the third nibble of the passed MCC.
	For example: the passed MCC is 0x1300 and passed MNC is 0x0071 for Pacific Bell.
	This function returns the converted MNC value as 0x170

	@param		mcc (in) UInt16 value used to pass the mcc value to be converted
	@param		mnc (in) UInt16 value used to pass the mnc to be converted
	@return		UInt16
**/

UInt16	NetRegUtil_PlmnConvertRawMnc(UInt16 mcc, UInt16 mnc);
UInt16	MS_PlmnConvertRawMnc(UInt16 mcc, UInt16 mnc);			 

//**************************************************************************************
/**
	This function converts the raw mnc, mcc and appends them to the plmn_str passed in as input 

	@param		mcc (in) UInt16 value used to pass the mcc value to be converted
	@param		mnc (in) UInt16 value used to pass the mnc to be converted
	@param		plmn_str (in) string passed to store the converted value.
	@param		plmn_str_sz (in) sizeof(plmn_str)
	@return		None
**/
void	NetRegUtil_ConvertRawPlmnToHer(UInt16 mcc, UInt16 mnc, UInt8 *plmn_str, UInt8 plmn_str_sz);
void	MS_ConvertRawPlmnToHer(UInt16 mcc, UInt16 mnc, UInt8 *plmn_str, UInt8 plmn_str_sz);

//**************************************************************************************
/**
	This function converts numberic string into plmn id and will return false if 
	the input is not correct.

	@param		oper (in) reference to the string to be converted (in)
	@param		plmn_id (in) reference to the plmnId structure to be filled in 
	@return		Boolean
**/
Boolean	NetRegUtil_ConvertStringToRawPlmnId(UInt8 *oper, PLMNId_t *plmn_id);
Boolean	MS_ConvertStringToRawPlmnId(UInt8 *oper, PLMNId_t *plmn_id);

//**************************************************************************************
/**
	This function converts the raw mnc, mcc to  plmn id passed in as input 

	@param		mcc (in) UInt16 value used to pass the mcc value to be converted
	@param		mnc (in) UInt16 value used to pass the mnc to be converted
	@param		plmn_id (in) reference to the plmnId structure to be filled in
	@return		None
**/
void	NetRegUtil_ConvertMccMncToRawPlmnId(UInt16 mcc, UInt16 mnc, PLMNId_t *plmn_id);
void	MS_ConvertMccMncToRawPlmnId(UInt16 mcc, UInt16 mnc, PLMNId_t *plmn_id);

//****************************************************************************
/**
 * This function converts the passed PLMN name to a string to be output to
 * ATC/MMI. 
 * 
 * -# If the PLMN name string is ASCII encoded, the returned string is ASCII 
 *	  encoded and NULL terminated. 
 *
 * -# If the PLMN name string is UCS2 encoded, the returned string is Hex 
 *	  string encoded. 
 *
 * @note the returned string is dynamically allocated. The caller of this 
 *		 function needs to de-allocate the memory if the returned string is 
 *		 not NULL.
 *
 *  @param		plmn_name Buffer to store the returned PLMN name. 
 *	@param		ucs2 (in)	encoding type of the PLMN name 
 *	@return		char* PLMN name string
 *
**/
char*	NetRegUtil_ConvertPLMNNameStr(Boolean ucs2, PLMN_NAME_t *plmn_name);
char*	MS_ConvertPLMNNameStr(Boolean ucs2, PLMN_NAME_t *plmn_name);


//******************************************************************************
/**
* This function decodes the Network Name 
* (long name or short name) IE defined in Section 
* 10.5.3.5a of GSM 04.08.
*
* @param ptr point to the length field of the Network Name IE TLV 
*				(the byte after Network Name IEI).
* @param mcc MCC of the registered PLMN in raw format (i.e. 13F0 for USA).
* @param plmn_name Buffer to store the returned PLMN name. 
* @return	None
*
*/
void	NetRegUtil_DecodeNetworkName(UInt8 *ptr, UInt16 mcc, PLMN_NAME_t *plmn_name); 
void	MS_DecodeNetworkName(UInt8 *ptr, UInt16 mcc, PLMN_NAME_t *plmn_name); 

//****************************************************************************
/**
 * This function determines if the passed MCC-MNC matches the MCC-MNC returned
 * from the network. 
 *
 * @note the passed MCC-MNC supports the use of "wild-carding" in MCC and MNC: 
 *       if a digit is set to Hex 'D', it matches any digit value in network
 *		 MCC-MNC.
 *
 *	@param		net_mcc (in) UInt16 network mcc value to be compared
 *	@param		net_mnc (in) UInt16 network mnc value to be compared
 *	@param		mcc (in) UInt16 value used to pass the mcc value to be compared
 *	@param		mnc (in) UInt16 value used to pass the mnc value to be compared
 *	@return		Boolean match or not-match
 */
Boolean				NetRegUtil_IsMatchedPLMN(UInt16 net_mcc, UInt8 net_mnc, UInt16 mcc, UInt8 mnc);
Boolean				MS_IsMatchedPLMN(UInt16 net_mcc, UInt8 net_mnc, UInt16 mcc, UInt8 mnc);


/**
	Get country name by MCC

	@param		mcc	mobile country code
	@return		char*	country name corresponding to the passed Mobile Country Code.
**/
const char* NetRegUtil_GetCountryByMcc(UInt16 mcc);
const char* PLMN_GetCountryByMcc(UInt16 mcc);

/**
	Get PLMN list size

	@return		UInt16	the number of entries in the PLMN lookup table.
**/
UInt16 NetRegUtil_GetPLMNListSize(void);
UInt16 MS_GetPLMNListSize(void);

/**
	Get PLMN by index

	@param		index (in)	index in the PLMN table
	@param		ucs2 (in)	encoding type of the PLMN name 
	@param		plmn_mcc (out)	plmn country code
	@param		plmn_mnc (out)	plmn network code
	@param		long_name (out)	long name
	@param		short_name (out)	short name
	@return		Boolean TRUE=input index is valid; FALSE= input index is out of range.
**/
Boolean NetRegUtil_GetPLMNEntryByIndex(
					UInt16	index,
					Boolean	ucs2,
					UInt16	*plmn_mcc,
					UInt16	*plmn_mnc,
				 	PLMN_NAME_t *long_name, 
					PLMN_NAME_t *short_name );
Boolean MS_GetPLMNEntryByIndex(
					UInt16	index,
					Boolean	ucs2,
					UInt16	*plmn_mcc,
					UInt16	*plmn_mnc,
				 	PLMN_NAME_t *long_name, 
					PLMN_NAME_t *short_name );

/**
	Get country based on mcc

	@param		mcc (in)	plmn country code
	@param		country_initial (out)	country initial
	@return		Boolean TRUE=initial found; FALSE= initial not found.
**/
Boolean	NetRegUtil_GetCountryInitialByMcc(UInt16 mcc, const char** country_initial);
Boolean	MS_GetCountryInitialByMcc(UInt16 mcc, const char** country_initial);

/**
	Get PLMN by code

	@param		ucs2 (in)	encoding type of the PLMN name 
	@param		plmn_mcc (in)	plmn country code
	@param		plmn_mnc (in)	plmn network code
	@param		long_name (out)	long name
	@param		short_name (out)	short name
	@param		country_name (out)	country name
	@return		Boolean TRUE=input index is valid; FALSE= input index is out of range.
**/
Boolean NetRegUtil_GetPLMNByCode(
					Boolean ucs2,
					UInt16	plmn_mcc,
					UInt16	plmn_mnc,
					PLMN_NAME_t *long_name, 
					PLMN_NAME_t *short_name,
					const char	**country_name ); 
Boolean MS_GetPLMNByCode(
					Boolean ucs2,
					UInt16	plmn_mcc,
					UInt16	plmn_mnc,
					PLMN_NAME_t *long_name, 
					PLMN_NAME_t *short_name,
					const char	**country_name ); 

/**
	Get PLMN code by name

	@param		ucs2 (in)	encoding type of the PLMN name 
	@param		long_format (in)	the plmn name given is a: TRUE = long name; FALSE = short name
	@param		*plmn_name (in)	plmn name
	@param		plmn_mcc (out)	plmn country code
	@param		plmn_mnc (out)	plmn network code
	@return		Boolean TRUE = found a name; FALSE = no name found.
**/
Boolean NetRegUtil_GetPLMNCodeByName(
					Boolean		ucs2,
					Boolean		long_format,
					const char	*plmn_name,
				 	UInt16		*plmn_mcc,
					UInt16		*plmn_mnc );
Boolean MS_GetPLMNCodeByName(
					Boolean		ucs2,
					Boolean		long_format,
					const char	*plmn_name,
				 	UInt16		*plmn_mcc,
					UInt16		*plmn_mnc );



/** @} */

#endif //_NETREG_UTIL_H_

