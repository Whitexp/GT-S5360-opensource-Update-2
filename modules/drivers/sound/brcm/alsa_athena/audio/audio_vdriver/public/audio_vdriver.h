/*******************************************************************************************
Copyright 2010 Broadcom Corporation.  All rights reserved.

Unless you and Broadcom execute a separate written software license agreement
governing use of this software, this software is licensed to you under the
terms of the GNU General Public License version 2, available at
http://www.gnu.org/copyleft/gpl.html (the "GPL").

Notwithstanding the above, under no circumstances may you combine this software
in any way with any other Broadcom software provided under a license other than
the GPL, without Broadcom's express prior written consent.
*******************************************************************************************/
/**
*
*   @file   audio_vdriver.h
*
*   @brief  common APIs for audio
*
****************************************************************************/
/**
*
* @defgroup Audio    Audio Component
*
* @brief    This group defines the common APIs for audio virtual driver
*
* @ingroup  Audio Component
*****************************************************************************/

/**
*
*  @file  audio_vdriver.h
*
*  @brief Audio Virtual Driver API
*
*  @note
*****************************************************************************/

#ifndef	__AUDIO_VDRIVER_H__
#define	__AUDIO_VDRIVER_H__

/**
*
* @addtogroup Audio
* @{
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	//AP->CP->DSP cmd to reuse the enum in dspcmd.h
	//AP->CP cmd to be listed here including filter loading.

	/**
	AUDDRV_DSP_FASTCMD,
	AUDDRV_DSP_VPUCMD,
	**/

	//CP:
	AUDDRV_CPCMD_SetBasebandVolume,
	AUDDRV_CPCMD_SetBasebandDownlinkMute,
	AUDDRV_CPCMD_SetBasebandDownlinkUnmute,
	AUDDRV_CPCMD_SetBasebandUplinkGain,
	AUDDRV_CPCMD_SET_DSP_AMR_VOLUME_UL,
	AUDDRV_CPCMD_SET_DSP_AMR_VOLUME_DL,
	AUDDRV_CPCMD_SetAudioMode,
	AUDDRV_CPCMD_READ_AUDVOC_AEQMODE,
	AUDDRV_CPCMD_WRITE_AUDVOC_AEQMODE,
	AUDDRV_CPCMD_GET_CP_AUDIO_MODE,
	AUDDRV_CPCMD_UPDATE_AUDIO_MODE,
	AUDDRV_CPCMD_ENABLE_DSP_DTX,
	AUDDRV_CPCMD_SET_VP_COMPANDER
} AUDDRV_CP_CMD_en_t;

typedef enum AUDDRV_REQUEST_ID_t
{
	AUDDRV_RATE_CHANGE_REQ,  //0x00
	AUDDRV_REQUEST_ID_TOTAL_COUNT
} AUDDRV_REQUEST_ID_t;

/// audio controller request message structure
typedef struct  AUDDRV_REQUEST_MSG_t
{
	AUDDRV_REQUEST_ID_t		    reqID;
	UInt32						param1;
	UInt32						param2;
	UInt32						param3;
} AUDDRV_REQUEST_MSG_t;

typedef enum {
	AUDDRV_USER_GET_SPKPROT,
	AUDDRV_USER_ENA_SPKPROT,
} AudioDrvUserParam_t;

UInt32 audio_control_generic(
				UInt32 param1,
				UInt32 param2,
				UInt32 param3,
				UInt32 param4,
				UInt32 param5,
				UInt32 param6);

UInt32 audio_control_dsp(
				UInt32 param1,
				UInt32 param2,
				UInt32 param3,
				UInt32 param4,
				UInt32 param5,
				UInt32 param6);


// Description:   Inititialize audio driver
//
void AUDDRV_Init( void );

// shut down audio driver
void AUDDRV_Shutdown(void);

//  the control sequence for starting telephony audio.
void AUDDRV_Telephony_Init (
				AUDDRV_MIC_Enum_t   mic,
				AUDDRV_SPKR_Enum_t  speaker
				);

// the control sequence for ratechange of voice call.
void AUDDRV_Telephony_RateChange ( UInt32 rate );

// the rate change request function 
void AUDDRV_RequestRateChange(UInt8 codecID);

// the control sequence for ending telephony audio.
//this func let DSP to turn off voice path, if need to resume apps operation on voice, controller needs to reenable voice path after phonce call ends.
void AUDDRV_Telephony_Deinit (void );


void AUDDRV_Telephony_SelectMicSpkr  (
				AUDDRV_MIC_Enum_t   mic,
				AUDDRV_SPKR_Enum_t  speaker
				);
// Enable DSP output processing.
void AUDDRV_EnableDSPOutput (
				AUDDRV_SPKR_Enum_t      mixer_speaker_selection,
				AUDIO_SAMPLING_RATE_t   sample_rate
				);
// Enable DSP input processing.
void AUDDRV_EnableDSPInput (
				AUDDRV_MIC_Enum_t		 mic_selection,
				AUDIO_SAMPLING_RATE_t	 sample_rate
				);

// Enable audio output path and audio processing.
void AUDDRV_Enable_Output (
				AUDDRV_InOut_Enum_t     input_path_to_mixer,
				AUDDRV_SPKR_Enum_t      mixer_speaker_selection,
				Boolean                 enable_speaker,
				AUDIO_SAMPLING_RATE_t   sample_rate,
				AUDIO_CHANNEL_NUM_t     input_to_mixer
				);

// Disable audio output path. 
void AUDDRV_Disable_Output ( AUDDRV_InOut_Enum_t  path );

// Enable audio input and digital processing.
void AUDDRV_Enable_Input (
				AUDDRV_InOut_Enum_t 	 input_path,
				AUDDRV_MIC_Enum_t		 mic_selection,
				AUDIO_SAMPLING_RATE_t	 sample_rate
				);

// Disable audio input and digital processing.
void AUDDRV_Disable_Input (  AUDDRV_InOut_Enum_t  path );

Boolean AUDDRV_IsVoiceCallWB(AudioMode_t audio_mode);
Boolean AUDDRV_IsCall16K(AudioMode_t voiceMode);
Boolean AUDDRV_InVoiceCall( void );

// mode and app profile related APIs
AudioApp_t AUDDRV_GetAudioApp( void );
void AUDDRV_SaveAudioMode( AudioMode_t audio_mode, AudioApp_t audio_app);
void AUDDRV_SetAudioMode( AudioMode_t audio_mode, AudioApp_t audio_app);
void AUDDRV_SetMusicMode ( AudioMode_t  audio_mode, AudioApp_t audio_app);
AudioMode_t AUDDRV_GetAudioMode( void );


void AUDDRV_SetVCflag( Boolean inVoiceCall );
Boolean AUDDRV_GetVCflag( void );

void AUDDRV_User_CtrlDSP (
				AudioDrvUserParam_t	audioDrvUserParam,
				void			*user_CB,
				UInt32			param1,
				UInt32			param2
				);

void AUDDRV_User_HandleDSPInt ( UInt32 param1, UInt32 param2, UInt32 param3 );
void AUDDRV_SetPCMOnOff(Boolean	on_off);
AudioEqualizer_en_t AUDDRV_GetEquType( AUDDRV_TYPE_Enum_t path );
void AUDDRV_SetEquType( AUDDRV_TYPE_Enum_t path, AudioEqualizer_en_t equ_id );

void AUDDRV_Telephony_InitHW (AUDDRV_MIC_Enum_t mic,
			  AUDDRV_SPKR_Enum_t speaker,
			  AUDIO_SAMPLING_RATE_t	sample_rate);

void AUDDRV_Telephony_DeinitHW(void);

void AUDDRV_SetTelephonyMicMute(Boolean mute);

void AUDDRV_ControlFlagFor_CustomGain( Boolean on_off );

void AUDDRV_EC(Boolean enable, UInt32 arg);
void AUDDRV_NS(Boolean enable);
void AUDDRV_ECreset_NLPoff(Boolean ECenable);

#ifdef __cplusplus
}
#endif

#endif // __AUDIO_VDRIVER_H__
