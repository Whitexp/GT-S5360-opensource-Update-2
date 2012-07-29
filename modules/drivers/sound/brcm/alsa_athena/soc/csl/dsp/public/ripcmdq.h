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
#ifndef _RIPCMDQ_H_
#define _RIPCMDQ_H_

#include "mobcom_types.h"

typedef enum
{
	TCHLOOPMODE_A,
	TCHLOOPMODE_B,
	TCHLOOPMODE_C,
	TCHLOOPMODE_D,
	TCHLOOPMODE_E,
	TCHLOOPMODE_F
} TCHLoopMode_t;

extern Boolean is_TCHLOOP_C_active;

//******************************************************************************
// Function Prototypes
//******************************************************************************
void RIPCMDQ_ResetRIP( void );

void RIPCMDQ_StartDAITest(
	UInt8 device
	);

void RIPCMDQ_StopDAITest( void );

void RIPCMDQ_StopTone( void );

void RIPCMDQ_Connect(
	Boolean	Uplink,
	Boolean Downlink,
	Boolean sample_rate
	);

void RIPCMDQ_Connect_Uplink(
	Boolean	Uplink,
	Boolean sample_rate
	);

void RIPCMDQ_Connect_Downlink(
	Boolean Downlink,
	Boolean sample_rate
	);

void RIPCMDQ_Switch_AudioProfile(
	UInt8	Is_spkr_mode
	);


void RIPCMDQ_Read(
	UInt16 addr,
	UInt16 tag
	);

void RIPCMDQ_Write(
	UInt16 addr,
	UInt16 value
	);

#ifdef DSP_FAST_COMMAND
void RIPCMDQ_FastRead(
	UInt16 addr,
	UInt16 tag,
    UInt16 shared_page
	);

    
void RIPCMDQ_FastWrite(
	UInt16 addr,
	UInt16 value,
	UInt16 shared_page
	);

void RIPCMDQ_FastRead_DSPMem(
	UInt16 lowaddr,
	UInt16 tag,
    UInt16 highaddr
	);

#endif

void RIPCMDQ_WritePRAM(
	UInt16 addr,		
	UInt16 value		
	);

void RIPCMDQ_SetCal(					// Load the sleep parameters
	UInt16 cacmp_shift,					// # of bits to shift CASR for SFGIR, SFGFR
	UInt32 casr,						// Slow clock value
	UInt32 ratio						// CAFR/CASR ratio
	);

void RIPCMDQ_StartTone(
	UInt8	tone,
	UInt8	tone_duration
	);

void RIPCMDQ_SetupToneGeneric(
	UInt16 superimpose_flag,				//  
	UInt16 duration,						//  
	UInt16 tone_scale						//  
	);


void RIPCMDQ_EnableAudio(
	Boolean	audio_on,
	Boolean MMVPU_16KHz_sample_rate,  //WB-AMR
	Boolean voice_call_16KHz_sample_rate  //voice call
	);

void RIPCMDQ_StopTone( void );

void RIPCMDQ_Connect(
	Boolean	Uplink,
	Boolean Downlink,
	Boolean sample_rate
	);

void RIPCMDQ_Init(void);
/* Maja CL8877 to improve AFC performance, Xiaoxin, 10/23/02 */

void RIPCMDQ_GetVersionID( void );		// Get version ID


void RIPCMDQ_SwitchAudioChnl(
	Boolean	isAuxChnl
	);

void RIPCMDQ_SetDSPSidetoneEnable( 
	UInt16	aud_mode,
	UInt16	aud_app,
	Boolean audioTuningMode
	);

void RIPCMDQ_SetDSPSidetoneGain( 
	UInt16	aud_mode,
	UInt16	aud_app,
	Boolean audioTuningMode
	);


void RIPCMDQ_ConfigSidetoneFilter(  // Generate filter coefficients to command queue
	UInt16	aud_mode,
	UInt16	aud_app,
	Boolean audioTuningMode
	);


void RIPCMD_DSP_PATCH(UInt16 addr, UInt16 val1, UInt16 val2);
void RIPCMD_DSP_PATCH_ENABLE(UInt16 addr, UInt16 val1, UInt16 val2);

void RIPCMD_DSP_AMR_RUN(UInt16 type, Boolean amr_if2_enable, UInt16 mst_flag);
void RIPCMDQ_EnableAudioAlign(	Boolean enable);					

void RIPCMDQ_SetRIPAMCR( 
	UInt16 value
	);

void RIPCMDQ_WriteBitwise(
	UInt16 addr,
	UInt16 mask,
	UInt16 value
	);

void RIPCMDQ_WriteBitwiseSlow(
	UInt16 addr,
	UInt16 mask,
	UInt16 value
	);

void RIPCMDQ_MST_FLAG_EFR(
	UInt16 value
	);
void RIPCMDQ_MST_FLAG_AFS(
	UInt16 value
	);
void RIPCMDQ_MST_FLAG_AHS(
	UInt16 value
	);
void RIPCMDQ_MST_FLAG_CS1(
	UInt16 value
	);


void RIPCMDQ_FLAG_SAIC(
	UInt16 value
	);


void RIPCMDQ_LoadSysparm( void );

void RIPCMDQ_CheckSumTest(Int32 addr );

#ifdef DSP_COMMS_INCLUDED


void RIPCMDQ_SetSCellSlot(
	UInt16	cell_id,
	UInt16 slot,
	Boolean dtx_enable,
	UInt8 bandSharedMem
	);
UInt16 RIPCMDQ_GetDSP_SCell( void);

void RIPCMDQ_SetKc(
	Boolean A5_3_flag,
	UInt8 *kc
	);
void RIPCMDQ_Cell4BitID(		// enable/disable 4 bit cell id
	Boolean set4bit						// True = 4 bit id enabled
	);

void RIPCMDQ_CloseTCHLoop(
	UInt8 subchan
	);

void RIPCMDQ_OpenTCHLoop( void );

void RIPCMDQ_CloseMSLoop(
	UInt16 loop_mode,
	UInt16 rxtx0,
	UInt16 rxtx1
	);

void RIPCMDQ_OpenMSLoop( void );

#ifdef DSP_FAST_COMMAND
void RIPCMDQ_FastTxFull(
	UInt8 index,
	TxOpt_t option,
	UInt16 delay
	);

void RIPCMDQ_Fast_FlushBufSimple( void );
#endif

void RIPCMDQ_SyncCmd(
	UInt16 sync_val
	);

void RIPCMDQ_TxFull(
	UInt8 index,
	TxOpt_t option,
	UInt16 delay
	);

void RIPCMDQ_RxEmpty(
	UInt8 index
	);

void RIPCMDQ_FlushBuffers( void );

void RIPCMDQ_FlushBufSimple( void );

void RIPCMDQ_FlushFACCH_RATSCCH( void );

void RIPCMDQ_SetTrackingIdle( void );

void RIPCMDQ_SetTrackingDch( void );

void RIPCMDQ_SetTrackingTch( void );

void RIPCMDQ_SetTrackingTBF( void );

void RIPCMDQ_SetTrackingTBF1( void );

void RIPCMDQ_SetTrackingTch_h( void );

void RIPCMDQ_SetGPIOActiveFrame(		// Enable active frame GPIO
	UInt16 addr,						// GPWR address
	UInt16 value,						// Active frame value
	UInt16 mask							// Active frame mask
	);

void RIPCMDQ_EnableDTX(					// Enable DTX mode
	Boolean enable						// TRUE, enable DTX
	);

/* Maja CL8877 to improve AFC performance, Xiaoxin, 10/23/02 */
/* void RIPCMDQ_ResetAFCDAC( void );		// Reset AFC DAC*/
void RIPCMDQ_ResetAFCDAC(UInt16 AFCAdjed);		// Reset AFC DAC
//adjust AFCDAC based on freq offset(in term of 900) calculated from UMTS AFC.
void RIPCMDQ_AdjustAFCDAC(Int16 freq_off_900);

void RIPCMDQ_AbortTx(					// Clear transmit burst buffer
	UInt16 index						// Transmit buffer to clear
	);

void RIPCMDQ_SetGPRSPwrCtrlIdle(
	Boolean reset_cn,
	UInt16	forget_factor,		// MIN(n, MAX(5, T_avg_w/Tdrx))
	UInt8	Pb_db
	);

void RIPCMDQ_SetGPRSPwrCtrlTx(
	Boolean use_bcch,
	UInt16	forget_factor		// SPEC 5.08 10.2.3
	);

void RIPCMDQ_SetGPRSAlpha(
	UInt8	alpha
	);
											  
void RIPCMDQ_SetGPRSPmax(
	UInt8	Pmax
	);

void RIPCMDQ_SetGPRSGamma(
	UInt8	slot_nu,
	UInt8	gamma
	);

void RIPCMDQ_SetGPRSPwrCtrlStart( void );

void RIPCMDQ_ForceRIPUpdate( void );

void RIPCMDQ_SetNoiseSuppress( 
	UInt16 noise_sup
	);

void RIPCMDQ_ConfigNoiseSuppression( 
    Boolean ns_on
    );

void RIPCMDQ_CLearAMRFlag( void	);
void RIPCMDQ_SetAMRParms( 
	UInt16 icm,
	UInt16 acs,
	UInt16 direction
	);

void RIPCMDQ_WakeupDSP(UInt16 threshold, UInt16 fcrnew);

void RIPCMDQ_EnhMeasEnable( 
	UInt16 enable
	);

void RIPCMDQ_SetFreqDone(void);

void RIPCMDQ_ForceNullPgTemplateUpdate( UInt8 index );

void RIPCMDQ_SetNullPage( Boolean enable);

#ifdef EPC
void RIPCMDQ_EPC( UInt16 mode, UInt16 flag );
#endif

void RIPCMDQ_AdjScellTimeOffset( Int16 offset );
void RIPCMDQ_EnableRFRxTest( UInt16 mode );
void RIPCMDQ_EnableRFTxTest( UInt16 mode );
#ifdef	ARM_HS_VOCODER
void RIPCMDQ_HS_VOCODER_Done(UInt16 dummy);
#endif

void RIPCMDQ_ControlEchoCancellation(void);

#endif //#ifdef DSP_COMMS_INCLUDED

void RIPCMDQ_Set_Was_Speech_Idle(void);

Boolean RIPCMDQ_IsAudioOn( void );
void PHYFRAME_GetDCH_OutputTrackLoopData( void);

void RIPCMDQ_SetVoiceDtx(Boolean enable);


#endif	//_RIPCMDQ_H_
