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
*	@file		hal_adc.h
*
*	@brief	Global definitions of hardware abstraction layer functions for ADC 
*			driver.
*
****************************************************************************/
/**
*	@defgroup	AdcDrvGroup   HAL_ADC interface.
*	@ingroup		HALGroup
*	@brief		Device Independent API for ADC.

\section setup Setup
To initialize ADC driver and configure the ADC using the default configuration from the system data base:
\code
HAL_ADC_Init(void);
\endcode

\section actions Actions

****************************************************************************/
#if !defined( _HAL_ADC_H_ )
#define _HAL_ADC_H_


//! The ADC Channel values.
typedef enum
{
	ADC_CH_0,	///< PA_TEMP ADC channel.
	ADC_CH_1, 	///< BIAS_HI channel.
	ADC_CH_2, 	///< BIAS_LO channel.	
	ADC_CH_3, 	///< VBAT channel.
	ADC_CH_4, 	///< BAT_TEMP channel.
	ADC_CH_MAX	///< MAX ADC channel.	
} HAL_ADC_Ch_en_t;

//! The higher layer shall call the ADC hardware abstraction layer functions to perform the following actions.
typedef enum
{
	HAL_ADC_ACTION_GET_UNALIGN,				///<Element type {::HAL_ADC_ReadConfig_st_t}, Get Sync Read result.
	HAL_ADC_ACTION_GET_MULTI_UNALIGNED,  	///<Element type {::HAL_ADC_ReadConfig_st_t},  Get Real Time Measurement result.
	HAL_ADC_ACTION_GET_ALIGNED,  			///<Element type {::HAL_ADC_ReadConfig_st_t},  Get Real Time Measurement result.
	HAL_ADC_ACTION_GET_IMMED  				///<Element type {::HAL_ADC_ReadConfig_st_t},  Get Immediate Real Time Measurement result.	
}HAL_ADC_Action_en_t;


//! The trigger used to start the ADC sampling.
typedef enum
{
 	HAL_ADC_TRG_UN_ALGND, 			///< ADC sample start Trigger (Unaligned to the RF)
 	HAL_ADC_TRG_TX_ALGND, 			///< ADC sample start Trigger (Aligned to the TX)
 	HAL_ADC_TRG_RX_ALGND, 			///< ADC sample start Trigger (Aligned to the RX)
 	HAL_ADC_TRG_AVG_VAL,			///< ADC Average sample values
	HAL_ADC_TRG_MAX	 				///< Total ADC sample Triggers	
}HAL_ADC_Trg_en_t;

//! The ADC Config results.
typedef enum
{
	HAL_ADC_RESULT_OK,  			///< Get ADC Measurement result.
	HAL_ADC_RESULT_ERROR,		///< Error ADC read result invalid.
	HAL_ADC_RESULT_BUSY, 			///< ADC read aligned request in progress.
	HAL_ADC_RESULT_NOT_INIT,		///< Register the ADC RealTimeMeasurement read complete callback
	HAL_ADC_RESULT_NO_SUPPORT,	///< The HAL feature is not supported by the driver
}HAL_ADC_Result_en_t;

//! The ADC Callback results.
typedef enum
{
	HAL_ADC_CB_PASS,  		///< ADC read callback completed.
	HAL_ADC_CB_FAIL,		///< ADC read callabck result invalid.
	HAL_ADC_CB_TIMEOUT, 	///< ADC read callback timed out.
}HAL_ADC_Cb_Result_en_t;

//! The ADC control.
typedef struct
{
	HAL_ADC_Ch_en_t 		ch;			///< input ADC channel
	UInt16 					*pAdcData;	///< pointer to output ADC read data (Valid when cb is NULL)
	HAL_ADC_Cb_Result_en_t 	cb_res; 		///< Output ADC read completion result
	UInt32					cntxt;		///< input memory context
}HAL_ADC_BasicInfo_st_t;


typedef struct
{
	UInt16 					mrd_cnt;	///< multi-read only. Single read is 0, 2 ch read is 1, 3 ch read is 2 (total ch read -1)
}HAL_ADC_MultiInfo_st_t;

typedef struct
{
	HAL_ADC_Trg_en_t		trg;		///< input ADC sample trigger
	UInt16					dly;		///< input ADC delay time after sample
}HAL_ADC_RfAlignedInfo_st_t;

typedef struct
{
	HAL_ADC_BasicInfo_st_t 		adc_basic;		///< Basic information for the channel
	HAL_ADC_MultiInfo_st_t			adc_mult;		///< Multiple read unaligned specific
	HAL_ADC_RfAlignedInfo_st_t 	adc_rf_algn; 	///< RF Aligned specific
}HAL_ADC_ReadConfig_st_t;

//*********************************************************************
/**
*
*  This function will initialize the ADC driver.  There is no variable settings for the ADCinitialization.
*   @param    void (in)   void
*   @return   Result_t           Result of the HAL ADC action
*   @note      
*	
**********************************************************************/
HAL_ADC_Result_en_t HAL_ADC_Init(void);


//*********************************************************************
/**
*
*   The HAL_ADC_Ctrl provides user access to basic ADC read functions
* 
*   @param    action (in)   The type of read or control/status command
*   @param    data   (in)   Pointer to a static data structure input
*   @param    callback (in) Callback function
*   @return   Result_t           Result of the HAL ADC action
*   @note: IMPORTANT: that the output "data" variable used during the HAL_ADC reads needs to be from a static 
*	memory location. Since the callback is used to update this variable, it cannot be a stack value since the 
*	HAL_ADC_Ctrl function completes prior to the callback being issued. If it completes, the "data" updates, but a 
*	stack variable is no longer in the scope of the function, it must be static.
*
**********************************************************************/
HAL_ADC_Result_en_t HAL_ADC_Ctrl(
	HAL_ADC_Action_en_t action,		///< (in) Action request
	void *data,						///< (io) Input/Output parameters associated with the action
	void *callback					///< (in) Callback function associated with the action
	);	

//*********************************************************************
/**
*
*   The AdcCbResult_t is called when the ADC result is received from the ADC driver
* 
*   @param    fnc_result (in)   PASS, FAIL, TIMEOUT
*   @param    config_result   (in)   ADC Configuration and read value
*   @note - Returns the ADC value and the status of the read.      
*
**********************************************************************/
typedef void (*AdcCbResult_t)(	
	HAL_ADC_ReadConfig_st_t* config_result		///< (in) Result of ADC reading
	);



typedef void (*AdcGenericCbResult_t)(	
	UInt32 fnc_result,		///< (in) Return Function Status
	void* config_result		///< (in) Result of ADC reading
	);


	
#endif /*_HAL_ADC_H_*/


