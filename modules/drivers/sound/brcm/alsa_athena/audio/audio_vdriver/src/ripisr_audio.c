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
* @file   ripisr_audio.c
* @brief  
*
******************************************************************************/

#include "mobcom_types.h"
#include "resultcode.h"
#include "chip_version.h"
//#include "memmap.h"
#include "ossemaphore.h"
#include "ostask.h"
#include "osqueue.h"
#include "audio_consts.h"
#include "auddrv_def.h"
#ifdef VPU_INCLUDED
//#include "mti_trace.h"
#endif
//#include "dbg.h"
#include "shared.h"
#include "vpu.h"
#include "audio_vdriver_voice_play.h"
#include "audio_vdriver_voice_record.h"
#include "audio_vdriver_voicetap.h"
//#include "audio_vdriver_voif.h"
#include "audio_vdriver_usb.h"
#include "csl_aud_drv.h"
#include "audio_vdriver.h"
#include "auddrv_audlog.h"
#include "msconsts.h"
#include "log.h"

#ifdef UNDER_LINUX
#include "ripisr_audio.h"
#endif

#define _DBG_(a)		a
//#define _DBG_(a)

extern int IPC_AudioControlSend(char *buff, UInt32 len);
void VPU_Capture_Request(VPStatQ_t reqMsg);
void VPU_Render_Request(VPStatQ_t reqMsg);

#if (defined(FUSE_DUAL_PROCESSOR_ARCHITECTURE) && defined(FUSE_COMMS_PROCESSOR) && defined(IPC_AUDIO))
// For CP msg queue to process VPU and Audio status
typedef enum
{
	TYPE_SEND_IPC_AUDIO_CTRL = 1,
	TYPE_SEND_IPC_VPU_CTRL
} AUDIO_VPU_t;

// msg data struct
typedef struct
{
	AUDIO_VPU_t type;
	StatQ_t	payload;
} ISRCMD_t;

static Queue_t	qAudioMsg = 0;
static Task_t 	taskAudioIsr = NULL;

//******************************************************************************
// Function Name:	CP_Audio_ISR_TaskEntry
//
// Description:		This is the task entry to process both audio and VPU related status 
//******************************************************************************
static void CP_Audio_ISR_TaskEntry( void ) 
{

	ISRCMD_t cmd;

	while(1)
	{
		OSQUEUE_Pend(qAudioMsg, (QMsg_t *)&cmd, TICKS_FOREVER );
		if (cmd.type == TYPE_SEND_IPC_AUDIO_CTRL)
		{
			StatQ_t msg;

			msg = cmd.payload;
			Log_DebugPrintf(LOGID_AUDIO, "CP_Audio_ISR_TaskEntry (AUDIO Type): 0x%x  : 0x%x : 0x%x :0x%x \r\n",msg.status,msg.arg0,msg.arg1,msg.arg2);
			IPC_AudioControlSend((char *)&msg, sizeof(StatQ_t));
		}
		else
			Log_DebugPrintf(LOGID_AUDIO, "CP_Audio_ISR_TaskEntry invalid status type \r\n");

	}

}

//******************************************************************************
// Function Name:	CP_Audio_ISR_Handler
//
// Description:		This function is called by  high level RIP interrupt service routine
//					to handles Audio status queue 
//******************************************************************************
void CP_Audio_ISR_Handler(StatQ_t status_msg)
{
	ISRCMD_t status;

	if(!qAudioMsg && !taskAudioIsr)
	{
		IPC_AudioControlSend((char *)&status_msg, sizeof(status_msg));

		if(!qAudioMsg)
			qAudioMsg = OSQUEUE_Create( QUEUESIZE_CP_ISRMSG,
							sizeof(ISRCMD_t), OSSUSPEND_PRIORITY);
		
		if(!taskAudioIsr)
			taskAudioIsr = 	OSTASK_Create( CP_Audio_ISR_TaskEntry, 
					TASKNAME_CP_Audio_ISR,
					TASKPRI_CP_Audio_ISR,
					STACKSIZE_CP_Audio_ISR
					);
	}
	else
	{
		status.type = TYPE_SEND_IPC_AUDIO_CTRL;
		status.payload = status_msg;
		OSQUEUE_Post(qAudioMsg, (QMsg_t *)&status, TICKS_FOREVER);	
	}

}


#else

//****************************************************************************
//                        G L O B A L   S E C T I O N
//****************************************************************************

//****************************************
// global variable definitions
//****************************************

//****************************************************************************
//                         L O C A L   S E C T I O N
//****************************************************************************

//****************************************
// local macro declarations
//****************************************

#define	INTERRUPT_STATUS_ON

//****************************************
// local typedef declarations
//****************************************


//****************************************
// local variable definitions
//****************************************

//****************************************
// local function declarations
//****************************************


//****************************************************************************
//                         FUNCTIONS
//****************************************************************************


#ifdef UNDER_LINUX


static capture_request_handler_t client_capture_request_handler = NULL;
static voif_request_handler_t client_voif_request_handler = NULL;


//******************************************************************************
// Function Name:	register_capture_request_handler
//
// Description:		
//                  
//******************************************************************************

void register_capture_request_handler( capture_request_handler_t capture_cb )
{
	Log_DebugPrintf(LOGID_AUDIO, "\n\r\t* AP register_capture_request_handler, 0x%x\n\r", capture_cb);
	client_capture_request_handler = capture_cb;
}

//******************************************************************************
// Function Name:	register_capture_request_handler
//
// Description:		
//                  
//******************************************************************************

void register_voif_request_handler( voif_request_handler_t voif_cb )
{
	Log_DebugPrintf(LOGID_AUDIO, "\n\r\t* AP register_voif_request_handler, 0x%x\n\r", voif_cb);
	client_voif_request_handler = voif_cb;
}


#ifdef VPU_INCLUDED
//******************************************************************************
// Function Name:	AP_VPU_ProcessStatus
//
// Description:		This function is called by audio driver at AP
//                  to handles VP status queue
//******************************************************************************
void AP_VPU_ProcessStatus( void )	// Process  VP status queue
{
	VPStatQ_t status_msg;

	while ( VPSHAREDMEM_ReadStatusQ( &status_msg ) )
	{    
		//Log_DebugPrintf(LOGID_AUDIO,"VPU_VPSHAREDMEM_ReadStatusQ= %d \r\n", status_msg.status);

        // In athena, we want to separate the vpu playback and record
		switch (status_msg.status)
		{
			case VP_STATUS_RECORDING_DATA_READY:
				//Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call VPU_Capture_Request\n");

                if(client_capture_request_handler != NULL)
				    client_capture_request_handler(status_msg);
				break;
            
            case VP_STATUS_VOIF_BUFFER_READY:
			{
				//Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call VOIF_ISR_Handler\n");
                if(client_voif_request_handler != NULL)
				    client_voif_request_handler (status_msg.arg0, status_msg.arg1);

				break;
			}

			case VP_STATUS_MAIN_AMR_DONE:
			{
				StatQ_t msg;

				msg.status = STATUS_MAIN_AMR_DONE;
				msg.arg0 = status_msg.arg0;
				msg.arg1 = status_msg.arg1;
				msg.arg2 = status_msg.arg2;

				//Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call VPU_ProcessStatusMainAMRDone\n");
				VPU_ProcessStatusMainAMRDone(msg);
				break;
			}

			default:
				break;

		}
	}
}
#endif  //#ifdef VPU_INCLUDED

// PCM logging dsp intr handler for IPC 
void AP_Audio_ISR_Handler(StatQ_t status_msg)
{
	int isr_status = status_msg.status;

	switch (isr_status)
	{
		case STATUS_AUDIO_STREAM_DATA_READY:
		{
			AUDLOG_ProcessLogChannel((StatQ_t*)&status_msg);
			break;		
		}

		default:
		{
			Log_DebugPrintf(LOGID_AUDIO, "Unknown Interrupt!\r\n");
			break;
		}
	}
}

#else // UNDER_LINUX

void AP_Audio_ISR_Handler(StatQ_t status_msg)
{
	int isr_status = status_msg.status;

#ifdef	INTERRUPT_STATUS_ON
	_DBG_(Log_DebugPrintf(LOGID_AUDIO, "DSP Interrupt status_msg.status : 0x%x 0x%x 0x%x 0x%x ",status_msg.status, status_msg.arg0, status_msg.arg1, status_msg.arg2));

	if(isr_status == STATUS_NEWAUDFIFO_SW_FIFO_LOW)			_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_NEWAUDFIFO_SW_FIFO_LOW\r\n"));
	else if (isr_status == STATUS_NEWAUDFIFO_SW_FIFO_EMPTY)	_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_NEWAUDFIFO_SW_FIFO_EMPTY\r\n"));
	else if (isr_status == STATUS_NEWAUDFIFO_DONEPLAY)		_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_NEWAUDFIFO_DONEPLAY\r\n"));
	else if (isr_status == STATUS_NEWAUDFIFO_CANCELPLAY)	_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_NEWAUDFIFO_CANCELPLAY\r\n"));
#if 0 // the following status is moved to AP status queue (VPU)
	else if (isr_status == STATUS_PRAM_CODEC_INPUT_EMPTY)	_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_PRAM_CODEC_INPUT_EMPTY\r\n"));
	else if (isr_status == STATUS_PRAM_CODEC_INPUT_LOW)		_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_PRAM_CODEC_INPUT_LOW\r\n"));
	else if (isr_status == STATUS_PRAM_CODEC_OUTPUT_FULL)	_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_PRAM_CODEC_OUTPUT_FULL\r\n"));
	else if (isr_status == STATUS_PRAM_CODEC_OUTPUT_LOW)	_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_PRAM_CODEC_OUTPUT_LOW\r\n"));
	else if (isr_status == STATUS_PRAM_CODEC_DONEPLAY)		_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_PRAM_CODEC_DONEPLAY\r\n"));
//	else if (isr_status == STATUS_PRAM_CODEC_CANCELPLAY)	_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_PRAM_CODEC_CANCELPLAY\r\n"));
	else if (isr_status == STATUS_USB_HEADSET_BUFFER)		_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_USB_HEADSET_BUFFER\r\n"));
#if	(defined(_ATHENA_))
	else if (isr_status == STATUS_BT_NB_BUFFER_DONE)		_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_BT_NB_BUFFER_DONE\r\n"));
#endif
#endif // move to AP
	else if (isr_status == STATUS_ASK_START_DDMA)			_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_ASK_START_DDMA\r\n"));
	else if (isr_status == VPU_STATUS_RECORDING_DATA_READY)	_DBG_(Log_DebugPrintf(LOGID_AUDIO, "VPU_STATUS_RECORDING_DATA_READY\r\n"));
	else ; //if (isr_status == STATUS_SP)		_DBG_(Log_DebugPrintf(LOGID_AUDIO, "STATUS_SP\r\n"));
#endif  //#ifdef	INTERRUPT_STATUS_ON

	switch (isr_status)
	{
		case STATUS_NEWAUDFIFO_SW_FIFO_LOW:
		{
			break;
		}
		case STATUS_NEWAUDFIFO_SW_FIFO_EMPTY:
		{
			break;
		}
		
		case STATUS_TEST_PDMA:
		{
			break;
		}

#ifdef VPU_INCLUDED

		case VPU_STATUS_RECORDING_DATA_READY:
		{
			break;
		}

#endif

			
#ifdef AUDIO_DRIVER_HQ_ENABLED
		case STATUS_HQ_ADC_PAGE_DONE:
		{
			block_info.buffer = NULL;
			block_info.length = status_msg.arg0; // use length to tell which page is used. 0 = page35, 1 = page36.
			msg.block_info = block_info;
			msg.type = HQ_RECORDING_PAGE_DONE;
			if ((p_Device_Channel_HQ_Input) && (p_Device_Channel_HQ_Input->queuAudioMsg))
			{
				OSQUEUE_Post(p_Device_Channel_HQ_Input->queuAudioMsg, (QMsg_t*)&msg, TICKS_FOREVER);
			}
			break;
		}
#endif // #ifdef AUDIO_DRIVER_HQ_ENABLED


#if (defined(FUSE_DUAL_PROCESSOR_ARCHITECTURE) && defined(FUSE_APPS_PROCESSOR))

		case STATUS_AUDIO_STREAM_DATA_READY:
		{
			//AUDLOG_ProcessLogChannel((StatQ_t*)&status_msg);
			break;		
		}
#endif


		default:
			_DBG_(Log_DebugPrintf(LOGID_AUDIO, "Unknown Interrupt!\r\n"));
			break;
	}

}



#ifdef VPU_INCLUDED
//******************************************************************************
// Function Name:	AP_VPU_ProcessStatus
//
// Description:		This function is called by audio driver at AP
//                  to handles VP status queue
//******************************************************************************
void AP_VPU_ProcessStatus( void )	// Process  VP status queue
{
	VPStatQ_t status_msg;

	while ( VPSHAREDMEM_ReadStatusQ( &status_msg ) )
	{    
		MSG_LOGV("VPU_VPSHAREDMEM_ReadStatusQ= %d \r\n", status_msg.status);

#if defined(_ATHENA_)
		// In athena, we want to separate the vpu playback and record
		switch (status_msg.status)
		{
			case VP_STATUS_RECORDING_DATA_READY:
				Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call VPU_Capture_Request\n");
				VPU_Capture_Request (status_msg);
				break;

			case VP_STATUS_PLAYBACK_DATA_EMPTY:
				Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call VPU_Render_Request\n");
				VPU_Render_Request (status_msg);
				break;
			// move to AP VPU status
			case VP_STATUS_PRAM_CODEC_INPUT_EMPTY:
			case VP_STATUS_PRAM_CODEC_INPUT_LOW:
//			case VP_STATUS_PRAM_CODEC_CANCELPLAY: //not supported by DSP since Athena
			case VP_STATUS_PRAM_CODEC_DONEPLAY:
			{
				Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call AMRWB_Render_Request, status = %d\n", status_msg.status);
				AMRWB_Render_Request(status_msg);
				break;
			}

			case VP_STATUS_USB_HEADSET_BUFFER:
			{
				Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call AUDDRV_USB_HandleDSPInt arg0=%x, arg1=%x, arg2=%x,\n",status_msg.arg0, status_msg.arg1, status_msg.arg2);
				AUDDRV_USB_HandleDSPInt(status_msg.arg0, status_msg.arg1, status_msg.arg2);
				break;
			}

			case VP_STATUS_BT_NB_BUFFER_DONE:
			{
				Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call BTTapNB_Capture_Request\n");
				BTTapNB_Capture_Request(status_msg);
				break;
			}


			case VP_STATUS_VOIF_BUFFER_READY:
			{
				Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call VOIF_ISR_Handler\n");
				VOIF_ISR_Handler (status_msg.arg0, status_msg.arg1);
				break;
			}

			case VP_STATUS_ARM2SP_EMPTY:
			{
				Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call ARM2SP_Render_Request\n");
				ARM2SP_Render_Request(status_msg);
				break;
			}

			case VP_STATUS_ARM2SP2_EMPTY:
			{
				Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call ARM2SP2_Render_Request\n");
				ARM2SP2_Render_Request(status_msg);
				break;
			}
			
			// move to AP VPU status
			case VP_STATUS_PRAM_CODEC_DONE_RECORD:
			{
				Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call AMRWB_Capture_Request\n");
				AMRWB_Capture_Request(status_msg);
				break;
			}

			case VP_STATUS_MAIN_AMR_DONE:
			{
				StatQ_t msg;

				msg.status = STATUS_MAIN_AMR_DONE;
				msg.arg0 = status_msg.arg0;
				msg.arg1 = status_msg.arg1;
				msg.arg2 = status_msg.arg2;

				//Log_DebugPrintf(LOGID_AUDIO, "AP_VPU_ProcessStatus  to call VPU_ProcessStatusMainAMRDone\n");
				VPU_ProcessStatusMainAMRDone(msg);
				break;
			}

#if defined(ENABLE_SPKPROT)

		case VP_STATUS_SP:
		{
			AUDDRV_User_HandleDSPInt((UInt32)status_msg.arg0, (UInt32)status_msg.arg1, (UInt32)status_msg.arg2);
			break;
		}
#endif

			default:
				break;

		}
#else
		Log_DebugPrintf(LOGID_AUDIO, "IPC_process_dsp_interrupt  to call AP_Process_VPU_Status\n");

		//now this is not called.
		//audio_ipc_ap.c and ripisr_audio.c can consolidate.
		AP_Process_VPU_Status(status_msg);
#endif // #if defined(_ATHENA_)


	}
}
#endif  //#ifdef VPU_INCLUDED


#endif //UNDER_LINUX

#endif

