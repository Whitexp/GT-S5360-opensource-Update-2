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
*  @file  auddrv_audlog.h
*  @brief Audio Driver API for audio logging
*
*****************************************************************************/
/**
*
* @defgroup AudioLoggingDriver    Audio Logging Driver
* @brief    This group defines the common APIs for the audio logging driver
*
*****************************************************************************/


#ifndef	__AUDDRV_AUDLOG_H__
#define	__AUDDRV_AUDLOG_H__

/**
*
* @addtogroup AudioLoggingDriver
* @{
*/

#include "shared.h"

#define	DBG_MSG_ON_ATCMD

#define	LOG_FRAME_NUMBER						(20)
#define	LOG_FRAME_NUMBER_FOR_FLASH_MEMORY		(100)
#define	LOG_STREAM_NUMBER						(4)

#define	LOG_BUFFER_NUMBER	40
#define	LOG_FRAME_SIZE		162
#define	LOG_FILE_SIZE		(2*1024*1024)

/**
	Log message consumer type
**/
typedef enum
{
	LOG_TO_PC = 0,		//< Log message to PC/MTT 
	LOG_TO_FLASH,		//< Save log message to local flash
} AUDLOG_DEST_en_t;

/**
	Audvoc driver call back for logging message save to file system
**/
typedef struct audlog_cb_info_t
{
  UInt16 *p_LogRead;	//< shared memory read pointer
  UInt32 size_to_read;	//< read size
  UInt32 capture_point;	//< the capture point value in the stream
  UInt32 stream_index;	//< the stream number
} AUDLOG_CB_INFO;

typedef void (*AUDLOG_CB_FUNC)( AUDLOG_CB_INFO *);


/**
	Log data source defintion with DSP sharedmem interface
**/
typedef struct audvoc_log_t
{
	AUDLOG_CB_FUNC	LogMessageCallback;	//< call back function to save stream frame to flash
	AUDLOG_DEST_en_t	log_consumer[4];	//< log message consumer 0 : MTT  1 : file system 
} AUDLOG_INFO;


typedef struct audvoc_audio_buffer_t
{
	UInt16	*buffer;			///< buffer pointer
	UInt16	*current_ptr;		///< current position of buffer
	Int32	length;				///< buffer length
	UInt32	bits_per_sample;	///< number of bits per audio sample
	Int32	flag;				///< flag = 0, buffer this block, flag = 1, start to play immediately.
	Int32	buffer_type;		///< buffer_type = 0,	buffer mode one buffer, buffer_type = 1, stream mode, use queues. 
	UInt32	buffer_serial_index; ///< buffer serial ID
} AUDVOC_BUFFER_INFO;

typedef struct
{
	UInt32				type;
	AUDVOC_BUFFER_INFO	block_info;
} AUDIO_PLAY_MSG_t;

typedef	struct 
{
	UInt16	stream_index;
	UInt16	log_capture_control;
	UInt16	log_msg[LOG_FRAME_SIZE/2];	// VR_Lin_PCM_t log_msg;
} LOG_FRAME_t;	

typedef struct
{
	UInt16*	p_LogMsg;
	UInt32	i_LogMsgSize;
} LOG_MSG_SAVE_t;

typedef struct log_msg_info_t
{

       UInt32 log_link;                ///< log message link path ( the stream number 0, 1, 2, 3)
    UInt32 log_capture_point;       ///< log message capture point value for stream
    UInt16 log_consumer;            ///< log message consumer 0 : MTT  1 : file system

} AUDVOC_CFG_LOG_INFO;


#ifdef __cplusplus
extern "C" {
#endif


void BCMLOG_LogSignal( unsigned int inSigCode, void* inSigBuf, unsigned int inSigBufSize,unsigned short inState,unsigned short inSender );

//Initialize driver internal variables and task queue. 
Int16 AUDDRV_AudLog_Init( void );

//Shut down driver internal variables and task queue. 
Int16 AUDDRV_AudLog_Shutdown( void );

Int16 AUDDRV_AudLog_Start ( 
							  UInt32 log_stream,
							  UInt32 log_capture_point,
							  AUDLOG_DEST_en_t log_consumer
							  
							  );

Int16 AUDDRV_AudLog_Stop( UInt32 log_stream );

void AUDLOG_ProcessLogChannel( StatQ_t *msg);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // __AUDDRV_AUDLOG_H__
