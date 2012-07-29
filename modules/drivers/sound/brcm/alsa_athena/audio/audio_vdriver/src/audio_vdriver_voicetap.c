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
*   @file   audio_vdriver_voicetap.c
*
*   @brief  DSP will handle the fifo and use HISR to notify ARM
*
****************************************************************************/

//=============================================================================
// Include directives
//=============================================================================

#include "mobcom_types.h"
#include "resultcode.h"
#include "ostask.h"
#include "osqueue.h"
#include "ossemaphore.h"
#include "osheap.h"

#include "msconsts.h"
#include "shared.h"
#include "audio_consts.h"
#include "auddrv_def.h"
//#include "brcm_rdb_sysmap.h"
//#include "brcm_rdb_dsp_audio.h"
#include "chal_types.h"
#include "chal_audiomisc.h"
#include "chal_audiomixertap.h"
#include "csl_aud_queue.h"
#include "audio_vdriver_voicetap.h"
#include "csl_aud_drv.h"
#include "audio_vdriver.h"
#include "dspcmd.h"
#include "log.h"


/**
*
* @addtogroup AudioDriverGroup
* @{
*/


#if defined (_ATHENA_) || defined (_RHEA_)

#define ENABLE_DEBUG
#ifdef  ENABLE_DEBUG
#define AUDDRV_DEBUG(a)		a
#else
#define AUDDRV_DEBUG(a)
#endif	

//=============================================================================
// Public Variable declarations
//=============================================================================


//=============================================================================
// Private Type and Constant declarations
//=============================================================================
#define BTNB_FRAME_SIZE		160*2 //20 msec in bytes

typedef enum VOICETAP_MSG_ID_t
{
	VOICETAP_MSG_CONFIG,
	VOICETAP_MSG_REGISTER_BUFDONE_CB,
	VOICETAP_MSG_START, 
	VOICETAP_MSG_STOP,
	VOICETAP_MSG_PAUSE,
	VOICETAP_MSG_RESUME,
	VOICETAP_MSG_ADD_BUFFER,
	VOICETAP_MSG_SHM_REQUEST
} VOICETAP_MSG_ID_t;


// create a new file, audQueue later.
typedef struct VOICETAP_MSG_t
{
	VOICETAP_MSG_ID_t	msgID;
	UInt32				parm1;
	UInt32				parm2;
} VOICETAP_MSG_t;


typedef struct VOICETAP_Configure_t
{
	AUDIO_SAMPLING_RATE_t   samplingRate;
	AUDIO_CHANNEL_NUM_t	numCh;
	//for BT-NB output configuration
	UInt16	channelSelect;				// select BT-NB [1/0] left/right channel 
	UInt16  enaStatus;				// ena STATUS_BT_NB_BUFFER_DONE for every 20 msec
	UInt16  overwriteDL;		// overwirte DL with BT-NB buffer 
	UInt16	mixingDL;		// mixing of BT-NB buffer with DL
	UInt16  overwriteUL;		// overwirte UL with BT-NB buffer
	UInt16	mixingUL;		// mixing of BT-NB buffer with UL
	UInt16	readLeftchannel;   	
	UInt16	readRightchannel;
	UInt16	enaUplink;   	
	UInt16	enaDownlink;   	

} VOICETAP_Configure_t;

typedef	struct VOICETAP_Drv_t
{
	Task_t					task;
	Queue_t					msgQueue;
	Semaphore_t				stopSema;
	Semaphore_t				addBufSema;

	UInt8					isRunning;
	
	VOICETAP_BufDoneCB_t		bufDoneCb;

	VOICETAP_TYPE_t			drvType;
	VOICETAP_Configure_t		config;

	UInt8					*ringBuffer;
	UInt32					bufferNum;
	UInt32					bufferSize;
	AUDQUE_Queue_t			*audQueue;
	
	// the destBuf to store the information of the buffer from application that can't be copied
	// fromthe queue due to the queue underflow
	UInt8					*destBuf;
	UInt32					destBufSize;
	UInt32					destBufCopied;
} VOICETAP_Drv_t;


//  zero inited
static VOICETAP_Drv_t	sBTTapNB_Drv = { 0 };


//=============================================================================
// Private function prototypes
//=============================================================================

static void BTNBCapture_TaskEntry (void);

static VOICETAP_Drv_t* GetDriverByType (VOICETAP_TYPE_t type);

static Result_t ConfigAudDrv (VOICETAP_Drv_t *audDrv, VOICETAP_Configure_t    *config);
static UInt32	CopyBufferFromQueue (VOICETAP_Drv_t *audDrv, UInt8 *buf, UInt32 size);
static void ProcessSharedMemRequest (VOICETAP_Drv_t *audDrv, UInt16 bufIndex);
static void StartBTNBLink(VOICETAP_Configure_t *config);
static void StopBTNBLink(VOICETAP_Configure_t *config);
static void CheckBufDoneUponStop (VOICETAP_Drv_t	*audDrv);

//=============================================================================
// Functions
//=============================================================================


//============================================================================
//
// Function Name: AUDDRV_VoiceTap_Init
//
// Description:   Initialize driver internal variables and task queue
//
//============================================================================

Result_t AUDDRV_VoiceTap_Init( VOICETAP_TYPE_t type )
{
	VOICETAP_Drv_t	*audDrv = NULL;

	audDrv = GetDriverByType (type);

	if (audDrv == NULL)
		return RESULT_ERROR;

	if (audDrv->isRunning)
		return RESULT_OK;

	memset (audDrv, 0, sizeof(VOICETAP_Drv_t));

	audDrv->drvType = type;
	audDrv->stopSema = OSSEMAPHORE_Create(0, OSSUSPEND_PRIORITY);
	audDrv->addBufSema = OSSEMAPHORE_Create(0, OSSUSPEND_PRIORITY);

	switch (audDrv->drvType)
	{
		case VOICETAP_TYPE_BTNB:
			audDrv->msgQueue = OSQUEUE_Create( QUEUESIZE_AUDDRV_VOICETAP,
											sizeof(VOICETAP_MSG_t), OSSUSPEND_PRIORITY );
			OSQUEUE_ChangeName(audDrv->msgQueue, (const char *)"AUDDRV_VOICETAP_Q" );

			audDrv->task = OSTASK_Create( (TEntry_t) BTNBCapture_TaskEntry,
										(TName_t) TASKNAME_AUDDRV_VOICETAP,
										TASKPRI_AUDDRV_VOICETAP,
										STACKSIZE_AUDDRV_VOICETAP
										);
			break;

		default:
			AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"AUDDRV_VoiceTap_Init:: Doesn't support audio driver type drvType = 0x%x\n", audDrv->drvType);)
			return RESULT_ERROR;
//			break;
	}

	audDrv->isRunning = TRUE;
	AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"AUDDRV_VoiceTap_Init::Exit.\n");)

	return RESULT_OK;
}

//============================================================================
//
// Function Name: AUDDRV_VoiceTap_Shutdown
//
// Description:   Shut down driver internal variables and task queue
//
//============================================================================
 
Result_t AUDDRV_VoiceTap_Shutdown( VOICETAP_TYPE_t type )
{
	VOICETAP_Drv_t	*audDrv = NULL;

	audDrv = GetDriverByType (type);

	if (audDrv == NULL)
		return RESULT_ERROR;

	if (audDrv->isRunning == FALSE)
		return RESULT_OK;

	
	// destroy the audio ringbuffer queue
	AUDQUE_Destroy (audDrv->audQueue);
	OSHEAP_Delete(audDrv->ringBuffer); 
	OSTASK_Destroy(audDrv->task);
	OSQUEUE_Destroy(audDrv->msgQueue);
	OSSEMAPHORE_Destroy (audDrv->stopSema);
	OSSEMAPHORE_Destroy (audDrv->addBufSema);

	audDrv->isRunning = FALSE;
	AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"AUDDRV_VoiceTap_Shutdown::Exit.\n");)

	return RESULT_OK;
}

//============================================================================
//
// Function Name: AUDDRV_VoiceTap_SetConfig
//
// Description:   Set up feature related configuration parameters
//
//============================================================================

Result_t AUDDRV_VoiceTap_SetConfig(
						VOICETAP_TYPE_t type,
						VoiceTap_CFG_t *tapCfg
						) 

{
	VOICETAP_Drv_t	*audDrv = NULL;
	VOICETAP_Configure_t	*config;
	VOICETAP_MSG_t	msg;
	
	audDrv = GetDriverByType (type);

	if (audDrv == NULL)
	{
		return RESULT_ERROR;
	}

	//allocated here and will be deleted later in ConfigAudDrv
	config = (VOICETAP_Configure_t *)OSHEAP_Alloc(sizeof(VOICETAP_Configure_t));
	config->samplingRate = tapCfg->sample_rate;
	config->numCh = tapCfg->numCh;

	//temp set voice Tap control for DSP here
	//later needs to pass down from upper layer
	AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"  AUDDRV_VoiceTap_SetConfig L/R = %d, enastatus=%d, link=%d, mixing=%d\n", tapCfg->chSelect, tapCfg->statusSend, tapCfg->vlink, tapCfg->mixMode);)	
	config->channelSelect = tapCfg->chSelect; //select channel; 1=R and 0=L
	if (config->channelSelect)
	{
		config->readLeftchannel = 1; //0;
		config->readRightchannel = 1; 
	}
	else
	{
		config->readLeftchannel = 1; 
		config->readRightchannel = 1; //0;
	}
	if (TRUE == tapCfg->statusSend) config->enaStatus = 1; //enable STATUS int to ARM
	else	config->enaStatus = 0;

	if (tapCfg->vlink == AUDIO_SPEECH_LINK_FAR)
	{
		config->enaUplink = 1; // UL
		config->enaDownlink = 0; //DL
	}
	else if (tapCfg->vlink == AUDIO_SPEECH_LINK_BOTH)
	{
		config->enaUplink = 1; // UL
		config->enaDownlink = 1; //DL
	} 
	else //AUDIO_SPEECH_LINK_NEAR or else
	{
		config->enaUplink = 0; // UL
		config->enaDownlink = 1; //DL
	}

	config->overwriteDL = 0;
	config->overwriteUL = 0;
	config->mixingDL = tapCfg->mixMode;
	config->mixingUL = tapCfg->mixMode;

	memset (&msg, 0, sizeof(VOICETAP_MSG_t));
	msg.msgID = VOICETAP_MSG_CONFIG;
	msg.parm1 = (UInt32)config;

	OSQUEUE_Post(audDrv->msgQueue, (QMsg_t*)&msg, TICKS_FOREVER);
	return RESULT_OK;
}



//============================================================================
//
// Function Name: AUDDRV_VoiceTap_SetBufDoneCB
//
// Description:   Setup callback to caller when buffer is filled
//
//============================================================================
Result_t AUDDRV_VoiceTap_SetBufDoneCB ( 
                     VOICETAP_TYPE_t			type,
                     VOICETAP_BufDoneCB_t	bufDone_cb )
{
	VOICETAP_Drv_t	*audDrv = NULL;
	VOICETAP_MSG_t	msg;

	audDrv = GetDriverByType (type);

	if (audDrv == NULL)
		return RESULT_ERROR;
	memset (&msg, 0, sizeof(VOICETAP_MSG_t));

	msg.msgID = VOICETAP_MSG_REGISTER_BUFDONE_CB;
	msg.parm1 = (UInt32)bufDone_cb;

	OSQUEUE_Post(audDrv->msgQueue, (QMsg_t*)&msg, TICKS_FOREVER);

	return RESULT_OK;
}

//============================================================================
//
// Function Name: AUDDRV_VoiceTap_Read
//
// Description:   API to Capture data from the BT NB Tap
//
//============================================================================
 
UInt32 AUDDRV_VoiceTap_Read( 
                    VOICETAP_TYPE_t		type,
                    UInt8*				pBuf,
                    UInt32				nSize )
{
	VOICETAP_Drv_t	*audDrv = NULL;
	VOICETAP_MSG_t	msg;

	audDrv = GetDriverByType (type);

	if (audDrv == NULL)
		return RESULT_ERROR;
	memset (&msg, 0, sizeof(VOICETAP_MSG_t));

	msg.msgID = VOICETAP_MSG_ADD_BUFFER;
	msg.parm1 = (UInt32)pBuf;
	msg.parm2 = nSize;	//in bytes

	OSQUEUE_Post(audDrv->msgQueue, (QMsg_t*)&msg, TICKS_FOREVER);

	// wait for the data copy finished.
	OSSEMAPHORE_Obtain (audDrv->addBufSema, TICKS_FOREVER);

	AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO," AUDDRV_VoiceTap_ReadBuffer obtain semaphore :: destBufCopied = 0x%x\n", audDrv->destBufCopied);)

	return audDrv->destBufCopied;
}

//============================================================================
//
// Function Name: AUDDRV_VoiceTap_Start
//
// Description:   API to Start the BT NB Tap
//
//============================================================================

Result_t AUDDRV_VoiceTap_Start ( VOICETAP_TYPE_t	type )
{
	VOICETAP_Drv_t	*audDrv = NULL;
	VOICETAP_MSG_t	msg;

	audDrv = GetDriverByType (type);

	if (audDrv == NULL)
		return RESULT_ERROR;
	memset (&msg, 0, sizeof(VOICETAP_MSG_t));

	msg.msgID = VOICETAP_MSG_START;

	OSQUEUE_Post(audDrv->msgQueue, (QMsg_t*)&msg, TICKS_FOREVER);

	return RESULT_OK;
}

//============================================================================
//
// Function Name: AUDDRV_VoiceTap_Pause
//
// Description:   API to Pause the BT NB Tap
//
//============================================================================
Result_t AUDDRV_VoiceTap_Pause ( VOICETAP_TYPE_t	type )
{
	VOICETAP_Drv_t	*audDrv = NULL;
	VOICETAP_MSG_t	msg;

	audDrv = GetDriverByType (type);

	if (audDrv == NULL)
		return RESULT_ERROR;
	memset (&msg, 0, sizeof(VOICETAP_MSG_t));

	msg.msgID = VOICETAP_MSG_PAUSE;

	OSQUEUE_Post(audDrv->msgQueue, (QMsg_t*)&msg, TICKS_FOREVER);
	return RESULT_OK;
}

//============================================================================
//
// Function Name: AUDDRV_VoiceTap_Resume
//
// Description:   API to Resume the BT NB Tap
//
//============================================================================
Result_t AUDDRV_VoiceTap_Resume( VOICETAP_TYPE_t	type )
{
	VOICETAP_Drv_t	*audDrv = NULL;
	VOICETAP_MSG_t	msg;

	audDrv = GetDriverByType (type);

	if (audDrv == NULL)
		return RESULT_ERROR;
	memset (&msg, 0, sizeof(VOICETAP_MSG_t));

	msg.msgID = VOICETAP_MSG_RESUME;

	OSQUEUE_Post(audDrv->msgQueue, (QMsg_t*)&msg, TICKS_FOREVER);

	return RESULT_OK;
}

//============================================================================
//
// Function Name: AUDDRV_VoiceTap_Stop
//
// Description:   API to Stop the BT NB Tap
//
//============================================================================

Result_t AUDDRV_VoiceTap_Stop( 
		VOICETAP_TYPE_t	type,
		Boolean			immediately )
{
	VOICETAP_Drv_t	*audDrv = NULL;
	VOICETAP_MSG_t	msg;

	audDrv = GetDriverByType (type);

	if (audDrv == NULL)
		return RESULT_ERROR;
	memset (&msg, 0, sizeof(VOICETAP_MSG_t));

	msg.msgID = VOICETAP_MSG_STOP;

	OSQUEUE_Post(audDrv->msgQueue, (QMsg_t*)&msg, TICKS_FOREVER);

	// make sure the task is stopped.
	OSSEMAPHORE_Obtain (audDrv->stopSema, TICKS_FOREVER);

	AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"  AUDDRV_VoiceTap_Stop::semaphore obtained. type = 0x%x, audDrv->type = 0x%x\n", type, audDrv->drvType);)

	return RESULT_OK;
}


//============================================================================
//
// Function Name: BTNBCapture_TaskEntry
//
// Description:   Task entry function for BT NB Tap driver
//
//============================================================================
static void BTNBCapture_TaskEntry (void)
{
	VOICETAP_MSG_t	msg;

	OSStatus_t		status;

	VOICETAP_Drv_t	*audDrv = &sBTTapNB_Drv;
	

//	AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"BTNBCapture_TaskEntry: BTNBCapture_TaskEntry is running \r\n");)

	while(TRUE)
	{
		status = OSQUEUE_Pend( audDrv->msgQueue, (QMsg_t *)&msg, TICKS_FOREVER );
		if (status == OSSTATUS_SUCCESS)
		{
			AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"  BTNBCapture_TaskEntry::msgID = 0x%x.\n", msg.msgID);)

			switch (msg.msgID)
			{
				case VOICETAP_MSG_CONFIG:
					AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"  BTNBCapture_TaskEntry::Configure.\n");)
					ConfigAudDrv (audDrv, (VOICETAP_Configure_t *)msg.parm1);
					break;

				case VOICETAP_MSG_REGISTER_BUFDONE_CB:
					audDrv->bufDoneCb = (VOICETAP_BufDoneCB_t)msg.parm1; //register app callback
					break;

				case VOICETAP_MSG_START:
					AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"  BTNBCapture_TaskEntry::Start capture.\n");)
					StartBTNBLink((VOICETAP_Configure_t *)&audDrv->config);				
					break;

				case VOICETAP_MSG_STOP:
					AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"  BTNBCapture_TaskEntry::Stop capture.\n");)
					StopBTNBLink((VOICETAP_Configure_t *)&audDrv->config);
					CheckBufDoneUponStop(audDrv);
					OSSEMAPHORE_Release (audDrv->stopSema);
					break;

				case VOICETAP_MSG_PAUSE:
					StopBTNBLink((VOICETAP_Configure_t *)&audDrv->config);
					break;

				case VOICETAP_MSG_RESUME:
					StartBTNBLink((VOICETAP_Configure_t *)&audDrv->config);
					break;

				case VOICETAP_MSG_ADD_BUFFER:
					CopyBufferFromQueue (audDrv, (UInt8 *)msg.parm1, msg.parm2);
					OSSEMAPHORE_Release (audDrv->addBufSema);
					break;

				case VOICETAP_MSG_SHM_REQUEST:
					ProcessSharedMemRequest (audDrv, (UInt16)msg.parm1);
					break;

				default:
					AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"BTNBCapture_TaskEntry: Unsupported msg, msgID = 0x%x \r\n", msg.msgID);)
					break;
			}
		}
	}
}

//============================================================================
//
// Function Name: GetDriverByType
//
// Description:   get driver structure head according to type
//
//============================================================================
static VOICETAP_Drv_t* GetDriverByType (VOICETAP_TYPE_t type)
{
	VOICETAP_Drv_t	*audDrv = NULL;

	switch (type)
	{
		case VOICETAP_TYPE_BTNB:
			audDrv = &sBTTapNB_Drv;
			break;

		default:
			AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"%s VOICETAP_Drv_t GetDriverByType:: Doesn't support audio driver type type = 0x%x\n", __FILE__, type);)
			break;
	}
	return audDrv;
}


//============================================================================
//
// Function Name: CopyBufferFromQueue
//
// Description:   local func to copy data from driver queue to *buf with size
//
//				return the size of bytes has been copied.
//============================================================================

static UInt32	CopyBufferFromQueue (VOICETAP_Drv_t *audDrv, UInt8 *buf, UInt32 size)
{
	UInt32 copied = 0;
	AUDQUE_Queue_t	*aq = audDrv->audQueue;
	
	// write to queue
	copied = AUDQUE_Read (aq, buf, size);
	
	if (copied == size)
	{
		// only callback if all data is copied
		audDrv->bufDoneCb (buf, size, audDrv->drvType);
	}
	
	
	// if we haven't copied all data, and will copy the left when 
	// we get the next callback. Save the break point.
	audDrv->destBuf = buf;
	audDrv->destBufSize = size;
	audDrv->destBufCopied = copied;

	AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO," AUDFIFO CopyBufferFromQueue :: destBufCopied = 0x%x, readPtr = 0x%x, writePtr = 0x%x\n", audDrv->destBufCopied, aq->readPtr, aq->writePtr);)

	return copied;
}

//============================================================================
//
// Function Name: ProcessSharedMemRequest
//
// Description:   Process data from DSP from the shared mem for BT_NB feature
//
//============================================================================
//bufIndex = 0 or 1 points to ring buffer in the shared mem
static void ProcessSharedMemRequest (VOICETAP_Drv_t *audDrv, UInt16 bufIndex)
{
	UInt32 copied = 0;
	AUDQUE_Queue_t	*aq = audDrv->audQueue;

	SharedMem_t *sh_mem = SHAREDMEM_GetDsp_SharedMemPtr();
	UInt8 *buffer = NULL;

	switch (audDrv->drvType)
	{
	//read from the FIFO here
		case VOICETAP_TYPE_BTNB:
// shared_BTnbdinLR only exists in CP.			
#if defined (FUSE_APPS_PROCESSOR)			
			if (!bufIndex)
				buffer = (UInt8 *)&sh_mem->shared_BTnbdinLR[0][0];        //Left channel, one of ping-pong buffer
			else
				buffer = (UInt8 *)&sh_mem->shared_BTnbdinLR[0][320];    //Left channel, one of ping-pong buffer

			if (0 == audDrv->config.channelSelect)
				buffer += 640*2;		//read from R channel of BT NB buffer
#endif
			break;

		default:
			AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"ProcessSharedMemRequest:: Voice Tap type not supported drvType = 0x%x\n", audDrv->drvType);)
			break;	
	}

	if (buffer != NULL)
	{	
		AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"ProcessSharedMemRequest:: bufIndex = 0x%x, readL_R = %d, buf_addr = 0x%x\n", bufIndex, audDrv->config.channelSelect, buffer);)

		// just copy it from shared memeory to queue
		AUDQUE_WriteNoCheck(aq, buffer, audDrv->bufferSize);

		AUDQUE_UpdateWritePtr (aq);
		AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"ProcessShareMemRequest callback, readPtr = 0x%x, writePtr = 0x%x\n", aq->readPtr, aq->writePtr);)
	}


	// check if we have left to copy 
	if (audDrv->destBufSize > audDrv->destBufCopied)
	{
		copied = AUDQUE_Read (aq, audDrv->destBuf + audDrv->destBufCopied, audDrv->destBufSize - audDrv->destBufCopied);
		
		if (copied == audDrv->destBufSize - audDrv->destBufCopied)
		{
			// only callback if all data is copied, call back with the whole buffer size
			audDrv->bufDoneCb (audDrv->destBuf, audDrv->destBufSize, audDrv->drvType);	
		}
		else
		{
			// we haven't copied all data, and will copy the left when 
			// we get the next dsp callback.
			AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO,"	AUDFIFO ProcessSharedMemRequest:: request size not meet? destBufSize = 0x%x, copied = 0x%x\n", audDrv->destBufSize, copied);)
		}	
		audDrv->destBufCopied += copied;
	}

}

//============================================================================
//
// Function Name: BTTapNB_Capture_Request
//
// Description:   called from STATUS_BT_NB_BUFFER_DONE status notification
//
//============================================================================
void BTTapNB_Capture_Request(VPStatQ_t status)
{
	VOICETAP_MSG_t	msg;

	AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO," BTTapNB_Capture_Request:: capture interrupt callback.\n");)

	memset (&msg, 0, sizeof(VOICETAP_MSG_t));
	msg.msgID = VOICETAP_MSG_SHM_REQUEST;
	msg.parm1 = (UInt32) status.arg1; // for ping pong buffer 0/1 from DSP

	OSQUEUE_Post(sBTTapNB_Drv.msgQueue, (QMsg_t*)&msg, TICKS_FOREVER);	
	
}

//============================================================================
//
// Function Name: ConfigAudDrv
//
// Description:   Configure the driver with ring buffer and R/W pointers
//
//============================================================================
static Result_t ConfigAudDrv (
				VOICETAP_Drv_t *audDrv, 
				VOICETAP_Configure_t    *config)
{
	memcpy (&audDrv->config, config, sizeof(VOICETAP_Configure_t));

	OSHEAP_Delete(config);

	switch (audDrv->drvType)
	{
		case VOICETAP_TYPE_BTNB:
			if (AUDIO_SAMPLING_RATE_8000 == audDrv->config.samplingRate)
			{
				// totally to queue about 25 frames 
				audDrv->bufferSize = BTNB_FRAME_SIZE; 
				audDrv->bufferNum = 25; 
			}
			else
			{
				// totally to queue about 1 sec of 50 frames 
				audDrv->bufferSize = BTNB_FRAME_SIZE*2; 
				audDrv->bufferNum = 25; 
			}


			audDrv->ringBuffer = (UInt8 *)OSHEAP_Alloc (audDrv->bufferNum*audDrv->bufferSize);
			audDrv->audQueue = AUDQUE_Create (audDrv->ringBuffer, audDrv->bufferNum, audDrv->bufferSize);
			AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO," BTNB ConfigAudDrv::\n");)

			break;

		default:
			AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO," BTNB ConfigAudDrv:: Doesn't support audio driver type drvType = 0x%x\n", audDrv->drvType);)
			break;
	}
	AUDDRV_DEBUG(Log_DebugPrintf(LOGID_AUDIO," Voice Tap ConfigAudDrv Done. \n");)

	return RESULT_OK;
}

//============================================================================
//
// Function Name: StartBTNBLink
//
// Description:   Send DSP command to start BT_NB
//
//============================================================================
static void StartBTNBLink(VOICETAP_Configure_t *config)
{

	UInt16 arg0 = 0;

	if(config->channelSelect > 0)  arg0 |= BIT15_MASK;     //  select left channel, otherwise select right channel
	if (config->enaStatus > 0)	arg0 |= BIT14_MASK; //enable STUTUS_BT_NB_BUFFER_DONE every 20ms to ARM

	if(config->readRightchannel > 0) arg0 |= BIT02_MASK;
	if(config->readLeftchannel > 0)  arg0 |= BIT03_MASK;
	if(config->mixingUL > 0)
	{
		arg0 |= BIT04_MASK;
	}
	else
	{
		if(config->overwriteUL > 0)   arg0 |= BIT05_MASK;
		else    arg0 |= BIT04_MASK;
	}

	if(config->mixingDL > 0)
	{
		arg0 |= BIT08_MASK;
	}
	else
	{
		if(config->overwriteDL > 0)   arg0 |= BIT09_MASK;
		else    arg0 |= BIT08_MASK;
	}

	//  [1/0] = [enable/disable] UL
	if(config->enaUplink > 0)	arg0 |= BIT01_MASK;         
	//  [1/0] = [enable/disable] DL
	if(config->enaDownlink > 0)	arg0 |= BIT00_MASK;         

	if ( AUDDRV_GetVCflag() )
	{
		Log_DebugPrintf(LOGID_AUDIO," StartBTNBLink arg0 = 0x%x. VC, No Need to init telephony\n", arg0);
	}
	else
	{
		AUDDRV_Telephony_Init( AUDDRV_MIC_PCM_IF, AUDDRV_SPKR_PCM_IF );
		Log_DebugPrintf(LOGID_AUDIO," StartBTNBLink arg0 = 0x%x. and init telephony\n", arg0);
	}

	Log_DebugPrintf(LOGID_AUDIO, " use audio_control_dsp to start BT-NB Tap, arg0=0x%x\n", arg0);
	audio_control_dsp(DSPCMD_TYPE_COMMAND_SET_BT_NB, arg0, 0, 0, 0, 0);

	Log_DebugPrintf(LOGID_AUDIO," StartBTNBLink arg0 = 0x%x. \n", arg0);

	chal_audiomixertap_EnableNb (NULL, TRUE); //should use AUDDRV_Enable_MixerTap( )
	//OEMIoControl(IOCTL_BTM_ENABLE, &btm_cfg, sizeof(AUDVOC_CFG_BTM_INFO), NULL, 0, NULL);

}


//============================================================================
//
// Function Name: StopBTNBLink
//
// Description:   Send DSP command to stop BT_NB
//
//============================================================================
static void StopBTNBLink(VOICETAP_Configure_t *config)
{

		audio_control_dsp(DSPCMD_TYPE_COMMAND_SET_BT_NB, 0, 0, 0, 0, 0);

		Log_DebugPrintf(LOGID_AUDIO," StopBTNBLink . \n");
		if ( AUDDRV_GetVCflag() )
		{
			chal_audiomixertap_EnableNb (NULL, FALSE); //should use AUDDRV_Enable_MixerTap( )
			Log_DebugPrintf(LOGID_AUDIO," StopBTNBLink and only disabel tap. \n");
		}
		else
		{
			AUDDRV_Telephony_Deinit();
			chal_audiomixertap_EnableNb (NULL, FALSE); //should use AUDDRV_Disable_MixerTap( )
			Log_DebugPrintf(LOGID_AUDIO," StopBTNBLink and disabel DSP audio. \n");
		}
}



// ==========================================================================
//
// Function Name: CheckBufDoneUponStop
//
// Description: Check if there is a pending buffer done CB when we are stopping 
//
// =========================================================================
static void CheckBufDoneUponStop (VOICETAP_Drv_t	*audDrv)
{
	// If a buffer has been copied to driver queue and bufDoneCB is not called, 
	// we need to call the buffer done
	if (audDrv->destBufCopied < audDrv->destBufSize)
	{
		Log_DebugPrintf(LOGID_SOC_AUDIO, "%s Catch a pending bufDoneCB! total buffer size 0x%x, copied buffer size 0x%x.", __FUNCTION__, audDrv->destBufSize, audDrv->destBufCopied);
		audDrv->bufDoneCb (audDrv->destBuf, audDrv->destBufCopied, audDrv->drvType);
	}
}

#endif
