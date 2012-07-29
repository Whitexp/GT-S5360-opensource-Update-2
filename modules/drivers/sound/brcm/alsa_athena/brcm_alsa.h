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


#ifndef __BRCM_ALSA_H__
#define __BRCM_ALSA_H__


#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>

#include <sound/core.h>
#include <sound/control.h>
#include <sound/pcm.h>
#include <sound/rawmidi.h>
#include <sound/initval.h>
#include <linux/wakelock.h>

#include <linux/broadcom/hw_cfg.h>

#define DEBUG_ON

#if defined(DEBUG_ON)
//#define DEBUG(args...)  if (debug) snd_printk(args) KERN_ALERT
#define DEBUG(args...)  if (debug) printk(args)
#else
#define DEBUG(args...) 
#endif


#define AUDIO_MAX_OUTPUT_VOLUME 40

typedef enum 
{
	BRCM_CTL_Speaker_Playback_Volume=0,
	BRCM_CTL_Speaker_Playback_Mute,
	
	BRCM_CTL_Mic_Capture_Volume,
	BRCM_CTL_Mic_Capture_Mute,

	BRCM_CTL_EAR_Playback_Volume,
	BRCM_CTL_EAR_Playback_Mute,
	
	BRCM_CTL_HEADSET_Playback_Volume,
	BRCM_CTL_HEADSET_Playback_Mute,
	
    BRCM_CTL_BTHeadset_Volume,
	BRCM_CTL_BTHeadset_Mute,
	
	BRCM_CTL_ROUTE,
	
	BRCM_CTL_TOTAL,
	BRCM_CTL_RESERVED

} BRCM_CTRL_ENU;


typedef enum
{
	NOCALL_IDLE=0,
	INCALL_EARPIECE,
	INCALL_SPEAKER,
	CALLSTATE_TOTAL
}CallStateEnu_t;


typedef enum 
{
	PCM_TYPE_PLAYBACK=0,
	PCM_TYPE_CAPTURE,
	PCM_TYPE_TOTAL,
	
	//tag it along
	PCM_TYPE_PLAYBACK_MUTE
} PCM_TYPE;

//event code
#if 0
#define PCM_EVENT_PLAY_NONE  0x00000000
#define PCM_EVENT_PLAY_START (1<<PCM_TYPE_PLAYBACK)
#define PCM_EVENT_RECD_START (1<<PCM_TYPE_CAPTURE)

#define PCM_EVENT_PLAY_VOLM  (1<<PCM_TYPE_PLAYBACK)
#define PCM_EVENT_RECD_VOLM  (1<<PCM_TYPE_CAPTURE)
#define PCM_EVENT_PLAY_MUTE  (1<<PCM_TYPE_PLAYBACK_MUTE)
#endif

typedef struct brcm_alsa_chip
{
	struct snd_card *card;
	//for playback[0] and record[1]
	struct snd_pcm_substream *substream[PCM_TYPE_TOTAL];   //current   
	int rate[PCM_TYPE_TOTAL];
	int buffer_bytes[PCM_TYPE_TOTAL];
	int period_bytes[PCM_TYPE_TOTAL];
	int pcm_ptr[PCM_TYPE_TOTAL];	
	int pcm_write_ptr[PCM_TYPE_TOTAL];	
	snd_pcm_uframes_t pcm_read_ptr[PCM_TYPE_TOTAL];	
	snd_pcm_uframes_t last_pcm_rdptr[PCM_TYPE_TOTAL];	
	int    xrun_occured[PCM_TYPE_TOTAL];
	
	/* workqueue */
	struct work_struct work_play;
    struct work_struct work_capt;


	
} brcm_alsa_chip_t;




//variables
extern int debug;
extern brcm_alsa_chip_t *g_brcm_alsa_chip;


//functions
extern int __devinit PcmDeviceNew(struct snd_card *card);
extern int __devinit ControlDeviceNew(struct snd_card *card);

extern void IsrHandler(int index);




#endif
