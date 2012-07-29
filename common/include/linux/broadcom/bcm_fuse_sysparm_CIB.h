/*******************************************************************************
* Copyright 2010 Broadcom Corporation.  All rights reserved.
*
* 	@file	include/linux/broadcom/bcm_fuse_sysparm_CIB.h
*
* Unless you and Broadcom execute a separate written software license agreement
* governing use of this software, this software is licensed to you under the
* terms of the GNU General Public License version 2, available at
* http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
*******************************************************************************/

#ifndef _BCM_FUSE_SYSPARM_CIB_H_
#define _BCM_FUSE_SYSPARM_CIB_H_

#include <linux/broadcom/chip_version.h>

/* **FIXME** MAG - Athena sysparm addrs/sizes; may be different for other */
/* CIB platforms */
#define PARM_IND_BASE               0x80050000
#define PARM_IND_SIZE               0x40000
#define PARM_DEP_BASE               0x80090000
#define PARM_DEP_SIZE		        0x10000

#define SYSPARM_INDEX_READY_INDICATOR   0x5059504D	/* SYSP */
#define MAX_SYSPARM_NAME_SIZE   128

/** Number of byte of Terminal Profile data defined in Sysparm. If the number of bytes is larger
 * than the "MAX_PROFILE_ARRAY_SIZE" defined in USIMAP/SIMAP, the extra bytes are truncated
 * and are not sent to the USIM/SIM.
 */
#define MAX_TERMINAL_PROFILE_ARRAY_SIZE  30

#define IMEI_SIZE 9		/* /< size of IMEI array in dep parms */

/** TX FREQ SECTION PER BAND */
#define N_FREQ_SECTIONS       8

/* PMU related defines */
#ifdef CONFIG_MFD_MAX8986
#define PARM_PMU_REG_TOTAL    0x7c
#endif

/** audio related defines */
#define AUDIO_MAGIC_SIZE    16

/* from audio_consts.h */
#define AUDIO_MODE_NUMBER   9	/* /< Up to 10 Audio Profiles (modes) after 213x1 */
#define AUDIO_MODE_NUMBER_VOICE	(AUDIO_MODE_NUMBER*2)
#define	AUDIO_APP_NUMBER		8	/*  /< 6 applications, can be extended */
#define NUM_OF_ENTRY_IN_DSP_VOICE_VOLUME_TABLE		15 
#define NUM_OF_ENTRY_IN_FM_RADIO_DIGITAL_VOLUME		15

#define AUDIO_5BAND_EQ_MODE_NUMBER    6	/* /< Up to 6 Audio EQ Profiles (modes) */

#define PR_DAC_IIR_SIZE				25

 /* path 0-5 in EQ. */
#define EQPATH_SIZE					6

#define NUM_OF_VOICE_COEFF      35

#define COEF_PER_GROUP      5	/* /< VOICE_DAC/VOICE_ADC */
#define DSP_SUBBAND_COMPANDER_FIR_TAP 11	/* /<11 taps */
#define DSP_SUBBAND_NLP_FREQ_BINS_WB 24	/* /<24 freq bins */

#define NUM_OF_SIDETONE_FILTER_COEFF 10
#define NUM_OF_ECHO_FAR_IN_FILTER_COEFF 10
#define NUM_OF_ECHO_NLP_CNG_FILTER  10
#define NUM_OF_BIQUAD_FILTER_COEF 10
#define NUM_OF_ECHO_STABLE_COEF_THRESH 2
#define NUM_OF_ECHO_NLP_GAIN    6

#define GPIO_INIT_FIELD_NUM 5
#define GPIO_INIT_REC_NUM 64

#define   DSP_FEATURE_AHS_SID_UPDATE_BEC_CHECK	/* Additional BEC check to determine SID updaet frame in Rxqual_s calculation */
#define   DSP_FEATURE_SUBBAND_NLP	/* Enable the compilation of ARM code specific to the subband_nlp feature in the DSP */
#define   DSP_FEATURE_SUBBAND_INF_COMP	/* Enable Infinite compression subband compressor */
#define   DSP_FEATURE_SUBBAND_INF_COMP_UL	/* Enable Infinite compression subband compressor sysparm/sheredmem init; not all the chips have ul and dl inf comp */
#define   DSP_FEATURE_EC_DYN_RNG_ENHANCEMENT	/* Enable EC dynamic range enhancements in the DSP */
#define   DSP_FEATURE_SUBBAND_NLP_MARGIN_VECTOR	/* Change single variable subband_nlp_UL_margin and subband_nlp_noise_margin thresholds into vector */
#define   DSP_FEATURE_CLASS_33	/* Enable class 33 feature in the DSP */
#define   DSP_FEATURE_NEW_FACCH_MUTING
#define   DSP_FEATURE_NULLPAGE_AUTOTRACK
#define   DSP_FEATURE_BT_PCM
#define   DSP_FEATURE_AFC_FB
#define   DSP_FEATURE_BB_RX_ADV	/* DSP BB advanced RX feature */
#define   DSP_FEATURE_STACK_DEPTH_CHECKING

#define   DSP_FEATURE_NULLPAGE_AUTOTRACK
#define   DSP_FEATURE_GAIN_DL_UL_NON_JUNO
#define   DSP_FEATURE_USB_HEADSET_GAIN
#define   DSP_FEATURE_AAC_LC_ENCODE_MIC_INPUT
#define   DSP_FEATURE_OTD_SNR_CHECK	/* OTD reported in SCH is updated when passing SNR threshold. */
#define   DSP_FEATURE_NORX0	/* Disable 1st RX slot so the previous frame can increase by one slot for search */
#define   DSP_FEATURE_AAC_ENCODER_DOWNLOADABLE
#define   DSP_FEATURE_OMEGA_VOICE
#define   DSP_FEATURE_FR_MUTE_FRAME

#define   _ATHENA_

#define   NUM_OMEGA_VOICE_BANDS 3
#define   NUM_OMEGA_VOICE_MAX_VOLUME_LEVELS 11
#define   NUM_OMEGA_VOICE_PARMS (NUM_OMEGA_VOICE_BANDS*NUM_OMEGA_VOICE_MAX_VOLUME_LEVELS)

typedef struct {
	Int16 omega_voice_thres_dB[NUM_OMEGA_VOICE_BANDS];
	Int16 omega_voice_max_gain_dB[NUM_OMEGA_VOICE_BANDS];
	Int16 omega_voice_gain_step_up_dB256;
	Int16 omega_voice_gain_step_dn_dB256;
	Int16 omega_voice_max_gain_dB_scale_factor;
} OmegaVoice_Sysparm_t;

typedef struct {
	UInt16 alpha_voice_enable;
	UInt16 alpha_voice_avc_target_dB;
	UInt16 alpha_voice_avc_max_gain_dB;
	UInt16 alpha_voice_avc_step_up_dB256;
	UInt16 alpha_voice_avc_step_down_dB256;
} AlphaVoice_Sysparm_t;

typedef struct {
	UInt16 kappa_voice_enable;
	UInt16 kappa_voice_high_pitched_tone_target_dB;
	Int16 kappa_voice_fast_detector_loudness_target_dB;
	UInt16 kappa_voice_fast_detector_max_supp_dB;
	Int16 kappa_voice_slow_detector_loudness_target_dB;
	UInt16 kappa_voice_slow_detector_max_supp_dB;
} KappaVoice_Sysparm_t;

typedef struct {
	char *name;
	void *ptr;
	unsigned int size;
	unsigned int flag;
} SysparmIndex_t;

/* Echo Canceller Comfort noise and NLP sysparms RON 4/28/2005 */
typedef struct {
	Int16			echo_cng_bias;
	UInt16			echo_cng_enable;
	UInt16			echo_nlp_max_supp;
	UInt16			echo_nlp_enable;
	UInt16			echo_subband_nlp_enable;
	UInt16			echo_nlp_gain;
	UInt16			echo_nlp_ul_brk_in_thresh;
	UInt16			echo_nlp_min_ul_brk_in_thresh;
	UInt16			echo_nlp_ul_gain_table[NUM_OF_ECHO_NLP_GAIN];
	UInt16			echo_nlp_dl_gain_table[NUM_OF_ECHO_NLP_GAIN];
	UInt16			nlp_ul_energy_window;
	UInt16			nlp_dl_energy_window;
	UInt16			echo_nlp_dtalk_ul_gain;
	UInt16			echo_nlp_dtalk_dl_agc_idx;// not used yet
	UInt16			echo_nlp_idle_ul_gain;// not used yet
	UInt16			echo_nlp_idle_dl_agc_idx;// not used yet
	UInt16			echo_nlp_ul_active_dl_agc_idx;// not used yet
	Int16			echo_nlp_relative_offset;
	UInt16			echo_nlp_relative_dl_w_thresh;
	UInt16			echo_nlp_relative_dl_energy_window;
	UInt16			echo_nlp_relative_dl_energy_decay;
	UInt16			echo_nlp_min_dl_pwr;
	UInt16			echo_nlp_min_ul_pwr;
	UInt16			echo_nlp_dtalk_dl_gain;
	UInt16			echo_nlp_dl_idle_ul_gain;
	UInt16			echo_nlp_ul_active_dl_gain;
	UInt16			echo_nlp_ul_idle_dl_gain;
	UInt16			echo_nlp_ul_idle_ul_gain;
	Int16			echo_nlp_relative_offset_dtalk;
	Int16			echo_nlp_relative_offset_dl_active;
	Int16			echo_nlp_relative_offset_ul_active;
	UInt16			echo_nlp_dtalk_hang_count;
	UInt16			echo_nlp_ul_active_hang_count;
	UInt16			echo_nlp_relative_ul_active_dl_loss;
	UInt16			echo_nlp_relative_ul_idle_dl_loss;
	UInt16			echo_nlp_relative_dl_idle_ul_loss;
	UInt16			echo_nlp_dl_persistence_thresh;
	UInt16			echo_nlp_dl_persistence_leak;
	UInt16			echo_nlp_dl_loss_adjust_thld;
	UInt16			echo_nlp_ul_active_dl_loss_adjust_step;
	UInt16			echo_nlp_ul_idle_dl_loss_adjust_step;
	UInt16			echo_nlp_ul_active_dl_loss_min_val;
	UInt16			echo_nlp_ul_idle_dl_loss_min_val;
	UInt16			echo_nlp_dl_idle_dl_gain;
#ifdef DSP_FEATURE_SUBBAND_NLP
	UInt16          echo_subband_nlp_distortion_thresh[DSP_SUBBAND_NLP_FREQ_BINS_WB];
	UInt16 			echo_subband_nlp_dl_delay_adj;
	UInt16 			echo_subband_nlp_ul_margin[DSP_SUBBAND_NLP_FREQ_BINS_WB];
	UInt16          echo_subband_nlp_noise_margin[DSP_SUBBAND_NLP_FREQ_BINS_WB];
	UInt16			echo_subband_nlp_erl_erle_adj_wb[DSP_SUBBAND_NLP_FREQ_BINS_WB];
	UInt16			echo_subband_nlp_ul_margin_wb[DSP_SUBBAND_NLP_FREQ_BINS_WB];
	UInt16			echo_subband_nlp_noise_margin_wb[DSP_SUBBAND_NLP_FREQ_BINS_WB]; 
	//UInt16 			echo_subband_nlp_filtered_gain_alpha;
	UInt16 			echo_subband_nlp_hpf_coef_a;
	UInt16 			echo_subband_nlp_hpf_coef_b;
	UInt16			echo_subband_nlp_dt_fine_control;
	UInt16			nlp_distortion_coupling;		 //Used in NLP distortion coupling estimate
	UInt16			reverb_time_constant;			 //Used in reverb control
	Int16			reverb_level;					 //Used in reverb control
#endif
} EchoNlp_t;

typedef struct {
	UInt16 audio_agc_enable;	/* /< Enable/Disable uplink and downlink audio agc */
	UInt16 max_thresh;
	UInt16 hi_thresh;
	UInt16 low_thresh;
	UInt16 decay;
	UInt16 max_idx;
	UInt16 min_idx;
	UInt16 step_down;
	UInt16 max_step_down;
	UInt16 step_up;
} AudioAGC_t;

#ifdef DSP_FEATURE_SUBBAND_INF_COMP
typedef struct {
	UInt16 t2lin;
	UInt16 g2t2;
	UInt16 g3t3;
	UInt16 g4t4;
	UInt16 alpha;
	UInt16 beta;
	UInt16 env;
	Int16 g1lin;
	Int16 step2;
	Int16 step3;
	Int16 step4;
} Subband_Compander_Band_t;

typedef	struct
{

	UInt16			g2t2;
	UInt16          env;
	Int16			g1lin;
} Smart_Compressor_t;
#endif

typedef struct {
	UInt8 audio_parm_magic[AUDIO_MAGIC_SIZE];
	//UInt16 audio_channel;
	UInt16 speaker_pga;	/* level, index */
	Int16 ext_speaker_pga;	/* dB */
	Int16 ext_speaker_preamp_pga;	/* dB */
	UInt16 mic_pga;		/* level, index */
	UInt16 max_mic_gain;
	//UInt32 audvoc_anacr0;

	UInt16 audvoc_vslopgain;
	UInt16 audvoc_mixergain;
	UInt16 audvoc_vcfgr;
#if defined(_ATHENA_) || defined(_HERA_) || defined(_RHEA_)
	UInt16 MPM_Dga_G;
	UInt16 MPM_Niir_Coefficient;
	UInt16 MPM_Gain_Attack_Step;
	UInt16 MPM_Gain_Attack_Threshold;
	UInt32 MPM_Gain_Attack_Slope;
	UInt32 MPM_Gain_Decay_Slope;
	UInt16 ihf_protection_enable;
	UInt16 GE_Center_Freq_1;
	UInt16 GE_Center_Freq_2;
	UInt16 GE_Center_Freq_3;
	UInt16 GE_Center_Freq_4;
	UInt16 GE_Center_Freq_5;
	UInt16 GE_Bandwidth_1;
	UInt16 GE_Bandwidth_2;
	UInt16 GE_Bandwidth_3;
	UInt16 GE_Bandwidth_4;
	UInt16 GE_Bandwidth_5;
	UInt16 GE_Crossover_Freq_1;
	UInt16 GE_Crossover_Freq_2;
	UInt16 GE_Crossover_Freq_3;
	UInt16 GE_Crossover_Freq_4;
	UInt16 audvoc_mixer_iir_hpf_enable;
	UInt16 audvoc_mixer_iir_hpf_cutoff_freq;
	UInt16 MPMbiquad_cfg;
#endif
	UInt16 sidetone;
	UInt16 audio_dsp_sidetone;	/* means dsp_sidetone_enable. */
	UInt16 audio_dl_idle_pga_adj;
	UInt16 audio_ns_ul_idle_adj;
	//UInt16 dac_filter_scale_factor;
	UInt16 ty_mic_gain;
	UInt16 sidetone_tty;
	AudioAGC_t ul_agc;
	AudioAGC_t dl_agc;
	EchoNlp_t echoNlp_parms;
	UInt16 echo_cancelling_enable;
	UInt16 echo_dual_filter_mode;
	UInt16 echo_nlp_cng_filter[NUM_OF_ECHO_NLP_CNG_FILTER];
	UInt16 echo_far_in_filter[NUM_OF_ECHO_FAR_IN_FILTER_COEFF];
	UInt16 comp_filter_coef[NUM_OF_BIQUAD_FILTER_COEF];
	UInt16 comp_biquad_gain;
	UInt16 echo_adapt_norm_factor;
	UInt16 echo_stable_coef_thresh[NUM_OF_ECHO_STABLE_COEF_THRESH];
	UInt16 echo_cancel_dtd_hang;
	UInt16 echo_cancel_dtd_thresh;
	UInt16 echo_cancel_hsec_step;
	UInt16 echo_cancel_hsec_mfact;
	UInt16 echo_cancel_hsec_loop;
	UInt16 echo_cancel_max_hsec;
	UInt16 echo_cancel_input_gain;
	UInt16 echo_cancel_output_gain;
	UInt16 echo_cancel_frame_samples;
	UInt16 echo_cancel_update_delay;
	UInt16 echo_feed_forward_gain;
	UInt16 echo_digital_input_clip_level;
	UInt16 echo_spkr_phone_input_gain;
	UInt16 echo_coupling_delay;
	UInt16 echo_en_near_scale_factor;
	UInt16 echo_en_far_scale_factor;
	UInt16 echo_nlp_downlink_volume_ctrl;
	UInt16 echo_nlp_timeout_val;
	UInt16 noise_suppression_enable;
	UInt16 noise_supp_input_gain;
	UInt16 noise_supp_output_gain;
	UInt16 voice_dac[NUM_OF_VOICE_COEFF];	/* should be NUM_OF_DAC_VOICE_COEFF */
	UInt16 voice_adc[NUM_OF_VOICE_COEFF];	/* should be NUM_OF_ADC_VOICE_COEFF */
	//UInt16 voice_adc_16khz[NUM_OF_VOICE_COEFF];	/* should be NUM_OF_ADC_VOICE_COEFF */
	UInt16 voice_dac_hpf_enable;
	UInt16 voice_dac_hpf_cutoff_freq;
	UInt16 ecLen;
	Int16 DT_TH_ERL_dB;
	UInt16 echo_step_size_gain;

	UInt16 compander_flag;
	UInt16 expander_alpha;
	UInt16 expander_beta;
	UInt16 expander_upper_limit;
	UInt16 expander_b;
	UInt16 expander_c;
	UInt16 expander_c_div_b;
	UInt16 expander_inv_b;
	UInt16 expander_flag_sidetone;
	UInt16 expander_alpha_sidetone;
	UInt16 expander_beta_sidetone;
	UInt16 expander_b_sidetone;
	UInt16 expander_c_sidetone;
	UInt16 expander_c_div_b_sidetone;
	UInt16 expander_inv_b_sidetone;
	UInt16 compander_flag_ul;
	UInt16 expander_alpha_ul;
	UInt16 expander_beta_ul;
	UInt16 expander_upper_limit_ul;
	UInt16 expander_b_ul;
	UInt16 expander_c_ul;
	UInt16 expander_c_div_b_ul;
	UInt16 expander_inv_b_ul;
	UInt16 compressor_gain_ul;
	UInt16 compressor_alpha_ul;
	UInt16 compressor_beta_ul;
	UInt16 ul_audio_clip_level;

	UInt16 polyringer_out_gain_dl;
	UInt16 polyringer_out_gain_ul;

	UInt16 second_amr_out_gain;
	Int16 noise_supp_min;
	Int16 noise_supp_max;
	/* 2133A5, 2124A2,2152A4, 213x1/2153A3 and beyond */
	UInt16 arm2speech_call_gain;
	UInt16 volume_step_size;
	UInt16 num_supported_volume_levels;

	UInt16 voice_volume_max;	/* in dB. */
	UInt16 voice_volume_init;	/* in dB. */
    UInt16 dsp_voice_vol_tbl[NUM_OF_ENTRY_IN_DSP_VOICE_VOLUME_TABLE]; /* in dB. */
	
	UInt16 sidetone_output_gain;
	UInt16 sidetone_biquad_scale_factor;
	UInt16 sidetone_biquad_sys_gain;
	UInt16 sidetone_filter[NUM_OF_SIDETONE_FILTER_COEFF];

	Int16 ec_de_emp_filt_coef;
	Int16 ec_pre_emp_filt_coef;

	UInt16 audio_hpf_enable;
	UInt16 audio_ul_hpf_coef_b;
	UInt16 audio_ul_hpf_coef_a;
	UInt16 audio_dl_hpf_coef_b;
	UInt16 audio_dl_hpf_coef_a;

	UInt16	pcm_filter_enable;
	UInt16	pcm_dl_filter[NUM_OF_BIQUAD_FILTER_COEF];
	UInt16	pcm_dl_biquad_gain;
	UInt16	pcm_dl_biquad_output_gain;
	UInt16	pcm_ul_filter[NUM_OF_BIQUAD_FILTER_COEF];
	UInt16	pcm_ul_biquad_gain;
	UInt16	pcm_ul_biquad_output_gain;

#ifdef DSP_FEATURE_SUBBAND_INF_COMP
	UInt16 dl_subband_compander_flag;
	Subband_Compander_Band_t fir1;	/* midband */
	Subband_Compander_Band_t fir2;	/* lowband */
	Subband_Compander_Band_t stream3;	/* highband */
	Subband_Compander_Band_t fullband;	/* fullband */
	Int16 compress_coef_fir1[DSP_SUBBAND_COMPANDER_FIR_TAP];
	Int16 compress_coef_fir2[DSP_SUBBAND_COMPANDER_FIR_TAP];
#endif

	UInt32 AUDVOC_MIXER_IIR[60];	/* Mixer output IIR */

	UInt16 omega_voice_enable;
	UInt16 omega_voice_max_allowed_gain_spread_dB;
	OmegaVoice_Sysparm_t
	    omega_voice_parms[NUM_OMEGA_VOICE_MAX_VOLUME_LEVELS];
	AlphaVoice_Sysparm_t alpha_voice_parms;
	KappaVoice_Sysparm_t kappa_voice_parms;

	UInt16 dual_mic_enable;
	UInt16 dual_mic_anc_enable;
	UInt16 dual_mic_nlp_enable;
	UInt16 dual_mic_pre_wnr_enable;
	UInt16 dual_mic_post_wnr_enable;
	UInt16 dual_mic_ns_level;

	UInt16 echo_cancel_mic2_input_gain; 
	UInt16 echo_cancel_mic2_output_gain;
	UInt16 echo_mic2_feed_forward_gain;

	UInt16 echo_path_change_detection_threshold;				//Used in echo path change detection
	Smart_Compressor_t smart_compressor;    				//smart compressor
} SysAudioParm_t;

typedef struct {
	UInt16 audvoc_pslopgain;
	UInt16 audvoc_aslopgain;
	UInt16 PR_DAC_IIR[25];	/*  Polyringer DAC IIR coefficients */
	UInt16 AUDVOC_ADAC_IIR[25];
	UInt16 AUDVOC_ADAC_IIR_hpf_enable;
	UInt16 AUDVOC_ADAC_IIR_hpf_cutoff_freq;
	UInt16 fm_radio_digital_vol[NUM_OF_ENTRY_IN_FM_RADIO_DIGITAL_VOLUME]; //in dB.
} SysIndMultimediaAudioParm_t;

UInt16 SYSPARM_GetLogFormat(void);

SysAudioParm_t* APSYSPARM_GetAudioParmAccessPtr(void);
SysIndMultimediaAudioParm_t* APSYSPARM_GetMultimediaAudioParmAccessPtr(void);
SysAudioParm_t* SYSPARM_GetExtAudioParmAccessPtr(UInt8 AudioApp);

UInt16 *SYSPARM_Get_AUDVOC_ADAC_FIR_Ptr(void);
UInt16 *SYSPARM_Get_AUDVOC_PEQPATHGAIN_Ptr(void);
UInt16 *SYSPARM_Get_AUDVOC_PEQCOF_Ptr(void);

UInt16 SYSPARM_GetDefault4p2VoltReading(void);
UInt16 SYSPARM_GetActual4p2VoltReading(void);
UInt16 SYSPARM_GetBattLowThresh(void);
UInt16 SYSPARM_GetActualLowVoltReading(void);
UInt16 SYSPARM_GetBattEmptyThresh( void );

UInt8 *SYSPARM_GetGPIO_Default_Value(UInt8 gpio_index);

/* retrieves IMEI string from sysparms only (doesn't check MS database */
/* value, as is done in CP sysparm.c) */
Boolean SYSPARM_GetImeiStr(UInt8 * inImeiStrPtr);

#endif /* _BCM_FUSE_SYSPARM_CIB_H_ */
