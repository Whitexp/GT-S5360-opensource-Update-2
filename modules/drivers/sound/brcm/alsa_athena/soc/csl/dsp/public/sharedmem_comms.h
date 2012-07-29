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
#ifndef	_INC_SHAREDMEM_COMMS_H_
#define	_INC_SHAREDMEM_COMMS_H_

#include "shared.h"
#include "chip_version.h"
#include "dsp_feature_def.h"
typedef enum
{
	BAND_SHARED_MEM_GSM	= 0,		// 900 for 900_DCS group
	BAND_SHARED_MEM_DCS,			// DCS for 900_DCS group
	BAND_SHARED_MEM_GSM850,			// 850 for 850_PCS group
	BAND_SHARED_MEM_PCS,			// PCS for 850_PCS group
	// NUM_OF_BANDS,				// Number of frequency bands in shared memory
	BAND_SHARED_MEM_ANY
} BandSharedMem_t;



void SHAREDMEM_Init( void );			// Initialize Shared Memory

//******************************************************************************
//
// Function Name:	SHAREDMEM_GetFrameIndexFromBuffer
//
// Description:		Read the frame index from the RX Buffer
//
// Notes:
//
//******************************************************************************

UInt8 SHAREDMEM_GetFrameIndexFromBuffer(				// Read the received message
	UInt8 buf_index,						// Rx buffer to use
	UInt8 buf_offset
	);

void SHAREDMEM_ReadBuffer16(			// Read the received message
	UInt8 index,						// Rx buffer to use
	UInt8 *bfi_flag,					// FALSE, message error
	UInt8 *frame_index,					// Last frame index of message
	UInt8 octets,						// message size (in octets)
	UInt16 *contents						// Pointer to received message storage
	);

void SHAREDMEM_ReadBufferIndexOTD(				// Read the frame index and OTD from received message
	UInt8 index,						// Rx buffer to use
	UInt8 *frame_index,					// Last frame index of message
	Int16 *otd							// Observed time difference, half-bits
);

void SHAREDMEM_ReadBuffer(				// Read the received message
	UInt8 index,						// Rx buffer to use
	UInt8 *bfi_flag,					// FALSE, message error
	UInt8 *bec,							// Bit error count
	UInt8 *frame_index,					// Last frame index of message
	Int8 *adj,							// Frame adjust (for SCH only)
	Int16 *otd,							// Observed time difference, half-bits (for SCH only)
	RxCode_t *code,						// Decoding mode
	UInt8 *octets,						// # of octets in message
	UInt8 *cn_db16,						// Cn (in dB/16)
	UInt8 *bl_var,						// BL_VAR
	UInt8 *contents						// Pointer to received message storage
	);

void SHAREDMEM_WriteBuffer(				// Post a message to send
	UInt8 index,						// Tx buffer to use
	TxCode_t code,						// Encoding mode
	UInt8 *contents						// Pointer to message to send
	);

void SHAREDMEM_WriteGPRSBuffer(			// Post a message to send
	UInt8 index,						// Tx buffer to use
	TxCode_t coding_scheme,				// tx data encoding mode
	UInt8 *contents						// Pointer to message to send
	);

UInt16 SHAREDMEM_ReadMonStatus(			// Read the neighbor power status
	UInt8	buff_id,					// Monitor Buffer Id
	UInt8	frame_index					// Frame index to get status
	);

UInt8 SHAREDMEM_ReadFrameIndex( void );	// Return the current frame index

void SHAREDMEM_PostCmdQ(				// Post an entry to the command queue
	CmdQ_t *cmd_msg						// Entry to post
	);
void SHAREDMEM_PostCmdQ_LISR(				// Post an entry to the command queue
	CmdQ_t *cmd_msg						// Entry to post
	);

#ifdef DSP_FAST_COMMAND
void SHAREDMEM_PostFastCmdQ(			// Post an entry to the fast command queue
	CmdQ_t *cmd_msg						// Entry to post
	);
void SHAREDMEM_PostFastCmdQ_LISR(			// Post an entry to the fast command queue
	CmdQ_t *cmd_msg						// Entry to post
	);
#endif

Boolean SHAREDMEM_ReadStatusQ(			// Read an entry from the command queue
	StatQ_t *status_msg					// Entry from queue
	);									// TRUE, if entry is available

void SHAREDMEM_ReadMeasReport(			// Read the measurement report
	UInt16 *rxqual_full,				// Pointer to array of four elements
	UInt16 *rxqual_sub,					// Pointer to array of four elements
	UInt16 *rxlev_full,					// Pointer to array of four elements
	UInt16 *rxlev_sub					// Pointer to array of four elements
	);

void SHAREDMEM_WriteTestPoint(			// Write to a test point
	UInt8	tp,							// Test Point
	UInt16	value						// Test Point Value
	);

SharedMem_t *SHAREDMEM_GetSharedMemPtr( void );
										// Return pointer to shared memory
Int16 *SHAREDMEM_ReadSNR( void);

UInt16 *SHAREDMEM_ReadMetric( void);

UInt16 *SHAREDMEM_ReadBFI_BEC( void);
void SHAREDMEM_RefreshAMREntry( UInt8 cmr);

void SHAREDMEM_GetEdgeRxBufPtr(UInt8 index, EDGE_RxBuf_t **SharedMemRxBuf);

void SHAREDMEM_GetEdgeTxBufPtr(UInt8 index, EDGE_TxBuf_t **SharedMemTxBuf);

#ifdef	ARM_HS_VOCODER
UInt16 *SHAREDMEM_GetHsDecBuffer_inptr(	void );		// Return the HS decoder input ptr
Int16 *SHAREDMEM_GetHsDecBuffer_outptr( void );			// Return the HS decoder output ptr
Int16 *SHAREDMEM_GetHsEncBuffer_inptr(	void );		// Return the HS encoder input ptr
UInt16 *SHAREDMEM_GetHsEncBuffer_outptr( void );			// Return the HS encoder output ptr
#endif

void SHAREDMEM_SetAMRParms( 
	UInt16 icm,
	UInt16 acs,
	UInt16 direction
	);

void SHAREDMEM_WriteDL_AMR_Speech(
	UInt8 rx_type,
	UInt8 frame_type,
	UInt32 *data,
	UInt8 length,
	Boolean amr_if2_enable,
	UInt16 *softA,
	UInt32 *deciphbit
);

void SHAREDMEM_ReadUL_AMR_Speech(
	UInt32 **speech_data,
	Boolean amr_if2_enable
);

void SHAREDMEM_WriteDL_VoIP_Data(UInt16 codec_type, UInt16 *pSrc);
UInt8 SHAREDMEM_ReadUL_VoIP_Data(UInt16 codec_type, UInt16 *pDst);

UInt16 SHAREDMEM_ReadUL_AMR_TxType(void);
UInt16 SHAREDMEM_ReadUL_AMR_Mode(void);
void SHAREDMEM_DLAMR_Speech_Init(void);
void SHAREDMEM_ULAMR_Speech_Init(void);
void SHAREDMEM_UMTSSetOffset(UInt16	qbc);
void SHAREDMEM_WriteBufferRach(				// Post a message to send
	UInt8 index,						// Tx buffer to use
	TxCode_t code,						// Encoding mode
	UInt8 *contents						// Pointer to message to send
	);

#define CLEAR_ENTRY_UMTS(entry)				{ (entry).entry4 &= ~0x4000; }
#define CLEAR_ENTRY_UMTS_NO_FN_INC(entry)	{ (entry).entry4 &= ~0x2000; }

void SHAREDMEM_InitComms(void);


#if defined(DEVELOPMENT_ONLY)

void  SHAREDMEM_set_audio_logging_flag(UInt16 input);
UInt16  SHAREDMEM_get_audio_logging_flag(void);
Boolean  SHAREDMEM_is_audio_logging_set(UInt16 mask);
void  SHAREDMEM_set_track_logging_flag(UInt16 input);
UInt16  SHAREDMEM_get_track_logging_flag(void);
void  SHAREDMEM_set_ncell_logging_flag(UInt16 input);
UInt16  SHAREDMEM_get_ncell_logging_flag(void);
void  SHAREDMEM_set_fer_logging_flag(UInt16 input);
UInt16  SHAREDMEM_get_fer_logging_flag(void);
void  SHAREDMEM_set_frameentry_logging_flag(Boolean input);
Boolean  SHAREDMEM_get_frameentry_logging_flag(void);
Boolean  SHAREDMEM_get_fsc_logging_flag(void);
Boolean  SHAREDMEM_get_event_logging_flag(void);

#endif	//  DEVELOPMENT_ONLY

#endif	// _INC_SHAREDMEM_COMMS_H_



