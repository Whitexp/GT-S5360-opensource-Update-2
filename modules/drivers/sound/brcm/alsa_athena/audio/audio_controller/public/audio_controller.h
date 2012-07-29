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
* @file   audio_controller.h
* @brief  Audio Controller interface
*
*****************************************************************************/
#ifndef __AUDIO_CONTROLLER_H__
#define __AUDIO_CONTROLLER_H__

/**
 * @addtogroup Audio_Controller 
 * @{
 */

typedef enum AUDIO_HW_ID_t
{
	AUDIO_HW_NONE,
	AUDIO_HW_MEM,
	AUDIO_HW_VOICE_OUT,			
	AUDIO_HW_AUDIO_OUT,
	AUDIO_HW_PLR_OUT,			
	AUDIO_HW_MONO_BT_OUT,	
	AUDIO_HW_STEREO_BT_OUT,		
	AUDIO_HW_USB_OUT,	
	AUDIO_HW_I2S_OUT,		
	AUDIO_HW_VOICE_IN,
	AUDIO_HW_AUDIO_IN,			
	AUDIO_HW_MONO_BT_IN,		
	AUDIO_HW_STEREO_BT_IN,		
	AUDIO_HW_USB_IN,	
	AUDIO_HW_I2S_IN,
	AUDIO_HW_TAP_VOICE,
	AUDIO_HW_TAP_AUDIO,
	AUDIO_HW_DSP_VOICE,
	AUDIO_HW_DSP_TONE,
    AUDIO_HW_EARPIECE_OUT,
    AUDIO_HW_HEADSET_OUT,
    AUDIO_HW_IHF_OUT,
    AUDIO_HW_SPEECH_IN,
    AUDIO_HW_NOISE_IN,
	AUDIO_HW_TOTAL_COUNT
} AUDIO_HW_ID_t;


typedef enum AUDCTRL_SPEAKER_t
{
	AUDCTRL_SPK_HANDSET,
	AUDCTRL_SPK_HEADSET,
	AUDCTRL_SPK_HANDSFREE,
	AUDCTRL_SPK_BTM,
	AUDCTRL_SPK_LOUDSPK,
	AUDCTRL_SPK_TTY,
	AUDCTRL_SPK_HAC,	
	AUDCTRL_SPK_USB,
	AUDCTRL_SPK_BTS,
	AUDCTRL_SPK_I2S,
	AUDCTRL_SPK_VIBRA,
	AUDCTRL_SPK_UNDEFINED,
	AUDCTRL_SPK_TOTAL_COUNT
} AUDCTRL_SPEAKER_t;

typedef enum AUDCTRL_MIC_Enum_t
{
	AUDCTRL_MIC_UNDEFINED,
	AUDCTRL_MIC_MAIN,
	AUDCTRL_MIC_AUX,
	AUDCTRL_MIC_DIGI1,
	AUDCTRL_MIC_DIGI2,
	AUDCTRL_DUAL_MIC_DIGI12,
	AUDCTRL_DUAL_MIC_DIGI21,
	AUDCTRL_DUAL_MIC_ANALOG_DIGI1,
	AUDCTRL_DUAL_MIC_DIGI1_ANALOG,
	AUDCTRL_MIC_BTM,  //Bluetooth Mono Headset Mic
	//AUDCTRL_MIC_BTS,  //not exist
	AUDCTRL_MIC_USB,  //USB headset Mic
	AUDCTRL_MIC_I2S,
    AUDCTRL_MIC_DIGI3, //Only for loopback path
	AUDCTRL_MIC_DIGI4, //Only for loopback path
	AUDCTRL_MIC_SPEECH_DIGI, //Digital Mic1/Mic2 in recording/Normal Quality Voice call.
	AUDCTRL_MIC_EANC_DIGI, //Digital Mic1/2/3/4 for Supreme Quality Voice Call.
    AUDCTRL_MIC_TOTAL_COUNT
} AUDCTRL_MIC_Enum_t;

#define AUDCTRL_MICROPHONE_t AUDCTRL_MIC_Enum_t  //need to merge with AUDDRV_MIC_Enum_t

typedef enum {
   TelephonyUseExtSpkr,
   VoiceUseExtSpkr,
   AudioUseExtSpkr,
   PolyUseExtSpkr
} ExtSpkrUsage_en_t;

/**
*  @brief  This function is the Init entry point for Audio Controller 
*
*  @param  none
*
*  @return none
*
****************************************************************************/
void AUDCTRL_Init (void);

/**
*  @brief  This function controls the power on/off of external
*          amplifier
*
*  @param  speaker	    (in)  speaker selection 
*  @param  usage_flag	(in)  external speaker usage
*  @param  use		    (in)  on/off
*
*  @return none
*
****************************************************************************/
void powerOnExternalAmp( AUDCTRL_SPEAKER_t speaker, ExtSpkrUsage_en_t usage_flag, Boolean use );


/**
*  @brief  This function controls the gain setting of external
*          amplifier
*
*  @param  gain	        (in)  gain value as per PMU setting register value
*  @return none
*
****************************************************************************/

void setExternalAmpGain(Int16 gain);

/**
*  @brief  This function controls the gain setting of external
*          pre mplifier
*
*  @param  gain	        (in)  gain value as per PMU setting register value
*  @return none
*
****************************************************************************/

void setExternalPreAmpGain(Int16 gain);

/**
*  @brief  Enable telephony audio path in HW and DSP
*
*  @param  ulSrc	(in)  uplink source 
*  @param  dlSink	(in)  downlink sink
*  @param  mic		(in)  microphone selection
*  @param  speaker	(in)  speaker selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_EnableTelephony(
				AUDIO_HW_ID_t			ulSrc,
				AUDIO_HW_ID_t			dlSink,
				AUDCTRL_MICROPHONE_t	mic,
				AUDCTRL_SPEAKER_t		speaker
				);

/**
*  @brief  Rate change telephony audio for DSP
*
*
*  @return none
*
****************************************************************************/
void AUDCTRL_RateChangeTelephony(void);

/**
*  @brief  Disable telephony audio path in HW and DSP
*
*  @param  ulSrc	(in)  uplink source 
*  @param  dlSink	(in)  downlink sink
*  @param  mic		(in)  microphone selection
*  @param  speaker	(in)  speaker selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_DisableTelephony(
				AUDIO_HW_ID_t			ulSrc,
				AUDIO_HW_ID_t			dlSink,
				AUDCTRL_MICROPHONE_t	mic,
				AUDCTRL_SPEAKER_t		speaker
				);

/**
*  @brief  Change telephony audio path in HW and DSP
*
*  @param  ulSrc	(in)  uplink source 
*  @param  dlSink	(in)  downlink sink
*  @param  mic		(in)  microphone selection
*  @param  speaker	(in)  speaker selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetTelephonyMicSpkr(
				AUDIO_HW_ID_t			ulSrc,
				AUDIO_HW_ID_t			dlSink,
				AUDCTRL_MICROPHONE_t	mic,
				AUDCTRL_SPEAKER_t		speaker
				);

/**
*  @brief  Get telephony speaker (downlink) volume
*
*
*  @return volume in dB
*
****************************************************************************/
UInt16 AUDCTRL_GetTelephonySpkrVolume(void);
/**
*  @brief  Set telephony speaker (downlink) volume
*
*  @param  dlSink	(in)  downlink sink
*  @param  speaker	(in)  speaker selection
*  @param  volume	(in)  downlink volume
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetTelephonySpkrVolume(
				AUDIO_HW_ID_t			dlSink,
				AUDCTRL_SPEAKER_t		speaker,
				UInt32					volume,
				AUDIO_GAIN_FORMAT_t		gain_format
				);

/**
*  @brief  Set telephony speaker (downlink) mute / un-mute
*
*  @param  dlSink	(in)  downlink sink
*  @param  speaker	(in)  speaker selection
*  @param  mute		(in)  TRUE: mute;    FALSE: un-mute
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetTelephonySpkrMute(
				AUDIO_HW_ID_t			dlSink,
				AUDCTRL_SPEAKER_t		speaker,
				Boolean					mute
				);

/**
*  @brief  Set telephony microphone (uplink) gain
*
*  @param  dlSink	(in)  downlink sink
*  @param  mic		(in)  microphone selection
*  @param  speaker	(in)  speaker selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetTelephonyMicGain(
				AUDIO_HW_ID_t			ulSrc,
				AUDCTRL_MICROPHONE_t	mic,
				Int16					gain
				);

/**
*  @brief  Set telephony mic (uplink) mute / un-mute
*
*  @param  ulSrc	(in)  uplink source 
*  @param  mic		(in)  microphone selection
*  @param  mute		(in)  TRUE: mute;    FALSE: un-mute
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetTelephonyMicMute(
				AUDIO_HW_ID_t			ulSrc,
				AUDCTRL_MICROPHONE_t	mic,
				Boolean					mute
				);

/**
*  @brief  Check whether in voice call mode.
*
*  @param  none
*
*  @return TRUE or FALSE
*
****************************************************************************/
Boolean AUDCTRL_InVoiceCall( void );

/**
*  @brief  Check whether in WB voice call mode.
*
*  @param  none
*
*  @return TRUE or FALSE
*
****************************************************************************/
Boolean AUDCTRL_InVoiceCallWB( void );

/**
*   Get current (voice call) audio mode 
*
*	@param		none
*
*	@return		AudioMode_t		(voice call) audio mode 
*
*   @note      
****************************************************************************/
AudioMode_t AUDCTRL_GetAudioMode( void );

/**
*   Get current (voice call) audio application 
*
*	@param		none
*
*	@return		AudioApp_t		(voice call) audio mode 
*
*   @note      
****************************************************************************/
AudioApp_t AUDCTRL_GetAudioApp( void );

//*********************************************************************
//  Save audio mode before call AUDCTRL_SaveAudioModeFlag( )
//	@param		mode		(voice call) audio mode 
//	@param		app			(voice call) audio app 
//	@return		none
//**********************************************************************/
void AUDCTRL_SaveAudioModeFlag( AudioMode_t mode, AudioApp_t app );


//*********************************************************************
//   Set (voice call) audio mode 
//	@param		mode		(voice call) audio mode 
//	@param		app		(voice call) audio app 
//	@return		none
//**********************************************************************/
void AUDCTRL_SetAudioMode( AudioMode_t mode, AudioApp_t app);


/**
*  @brief  Enable a playback path
*
*  @param  src	(in)  playback source 
*  @param  sink	(in)  playback sink
*  @param  spk	(in)  speaker selection
*  @param  numCh	(in)  stereo, momo
*  @param  sr	(in)  sample rate
*
*  @return none
*
****************************************************************************/
void AUDCTRL_EnablePlay(
				AUDIO_HW_ID_t			src,
				AUDIO_HW_ID_t			sink,
				AUDIO_HW_ID_t			tap,
				AUDCTRL_SPEAKER_t		spk,
				AUDIO_CHANNEL_NUM_t		numCh,
				AUDIO_SAMPLING_RATE_t	sr
				);

/********************************************************************
*  @brief  Disable a playback path
*
*  @param  src	(in)  playback source 
*  @param  sink	(in)  playback sink
*  @param  speaker	(in)  speaker selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_DisablePlay(
				AUDIO_HW_ID_t			src,
				AUDIO_HW_ID_t			sink,
				AUDCTRL_SPEAKER_t		spk
				);

/********************************************************************
*  @brief  Set playback volume
*
*  @param  sink	(in)  playback sink
*  @param  speaker	(in)  speaker selection
*  @param  vol	(in)  volume to set
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetPlayVolume(
				AUDIO_HW_ID_t			sink,
				AUDCTRL_SPEAKER_t		spk,
				AUDIO_GAIN_FORMAT_t     gainF,
				UInt32					vol_left,
				UInt32					vol_right
				);

/********************************************************************
*  @brief  mute/unmute playback 
*
*  @param  sink	(in)  playback sink 
*  @param  speaker	(in)  speaker selection
*  @param  vol	(in)  TRUE: mute, FALSE: unmute
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetPlayMute(
				AUDIO_HW_ID_t			sink,
				AUDCTRL_SPEAKER_t		spk,
				Boolean					mute
				);

/********************************************************************
*  @brief  Add a speaker to a playback path
*
*  @param  sink	(in)  playback sink
*  @param  speaker	(in)  speaker selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_AddPlaySpk(
				AUDIO_HW_ID_t			sink,
				AUDCTRL_SPEAKER_t		spk
				);

/********************************************************************
*  @brief  Remove a speaker to a playback path
*
*  @param  sink	(in)  playback sink  
*  @param  speaker	(in)  speaker selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_RemovePlaySpk(
				AUDIO_HW_ID_t			sink,
				AUDCTRL_SPEAKER_t		spk
				);


/********************************************************************
*  @brief  enable a record path
*
*  @param  src	(in)  record source
*  @param  sink	(in)  record sink
*  @param  speaker	(in)  speaker selection
*  @param  numCh	(in)  stereo, momo
*  @param  sr	(in)  sample rate
*
*  @return none
*
****************************************************************************/
void AUDCTRL_EnableRecord(
				AUDIO_HW_ID_t			src,
				AUDIO_HW_ID_t			sink,
				AUDCTRL_MICROPHONE_t	mic,
				AUDIO_CHANNEL_NUM_t		numCh,
				AUDIO_SAMPLING_RATE_t	sr
				);

/********************************************************************
*  @brief  disable a record path
*
*  @param  src	(in)  record source
*  @param  sink	(in)  record sink
*  @param  mic	(in)  microphone selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_DisableRecord(
				AUDIO_HW_ID_t			src,
				AUDIO_HW_ID_t			sink,
				AUDCTRL_MICROPHONE_t	mic
				);

/********************************************************************
*  @brief  Set gain of a record path
*
*  @param  src	(in)  record source
*  @param  mic	(in)  microphone selection
*  @param  gainL	(in)  the left channel gain to set
*  @param  gainR	(in)  the right channel gain to set
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetRecordGain(
				AUDIO_HW_ID_t			src,
				AUDCTRL_MICROPHONE_t	mic,
				UInt32					gainL,
				UInt32					gainR
				);

/**
*  @brief  Ensable Tap (wideband or voice)
*
*  @param  src	(in)  
*  @param  sink	(in)  
*  @param  spk	(in)  speaker selection
*  @param  numCh	(in)  stereo, momo
*  @param  sr	(in)  sample rate
*
*  @return none
*
****************************************************************************/
void AUDCTRL_EnableTap(
				AUDIO_HW_ID_t			tap,
				AUDCTRL_SPEAKER_t		spk,
				AUDIO_SAMPLING_RATE_t	sr
				);

/**
*  @brief  Disable Tap (wideband or voice)
*
*  @param  src	(in)  
*  @param  sink	(in)  
*  @param  spr	(in)  speaker selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_DisableTap( AUDIO_HW_ID_t tap);

/**
*  @brief  Set Tap gain (only wideband)
*
*  @param  src	(in)  
*  @param  spr	(in)  speaker selection
*  @param  gain	(in)  
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetTapGain(
				AUDIO_HW_ID_t			tap,
				UInt32					gain
				);

/********************************************************************
*  @brief  mute/unmute a record path
*
*  @param  src	(in)  record source
*  @param  mic	(in)  microphone selection
*  @param  mute	(in)  TRUE: mute, FALSE: unmute
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetRecordMute(
				AUDIO_HW_ID_t			src,
				AUDCTRL_MICROPHONE_t	mic,
				Boolean					mute
				);

/********************************************************************
*  @brief  add a micophone to a record path
*
*  @param  src	(in)  record source
*  @param  mic	(in)  microphone selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_AddRecordMic(
				AUDIO_HW_ID_t			src,
				AUDCTRL_MICROPHONE_t	mic
				);

/********************************************************************
*  @brief  remove a micophone from a record path
*
*  @param  src	(in)  record source
*  @param  mic	(in)  microphone selection
*
*  @return none
*
****************************************************************************/
void AUDCTRL_RemoveRecordMic(
				AUDIO_HW_ID_t			src,
				AUDCTRL_MICROPHONE_t	mic
				);


/********************************************************************
*  @brief  enable or disable audio HW loopback
*
*  @param  enable_lpbk (in)  the audio mode
*  @param  mic         (in)  the input to loopback
*  @param  speaker     (in)  the output from loopback
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetAudioLoopback( 
							Boolean					enable_lpbk,
							AUDCTRL_MICROPHONE_t	mic,
							AUDCTRL_SPEAKER_t		speaker
							);

/********************************************************************
*  @brief  enable or disable audio HW loopback
*
*  @param  enable_lpbk (in)  the audio mode
*  @param  mic         (in)  the input to loopback
*  @param  speaker     (in)  the output from loopback
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetEQ( 
				AUDIO_HW_ID_t	audioPath,
				AudioEqualizer_en_t  equType
				);

/********************************************************************
*  @brief  set gain on the VPU playback path on the downlink path
*
*  @param  uVol (in)  amr volume
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetAMRVolume_DL(UInt16 uVol);

/********************************************************************
*  @brief  set gain on the VPU playback path on the uplink path
*
*  @param  uVol (in)  amr volume
*
*  @return none
*
****************************************************************************/
void AUDCTRL_SetAMRVolume_UL(UInt16 uVol);

void AUDCTRL_EC(Boolean enable, UInt32 arg);
void AUDCTRL_NS(Boolean enable);
void AUDCTRL_ECreset_NLPoff(Boolean ECenable);

#endif //#define __AUDIO_CONTROLLER_H__
