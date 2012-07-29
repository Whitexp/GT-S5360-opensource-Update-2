/*******************************************************************************************
Copyright 2009, 2010 Broadcom Corporation.  All rights reserved.

Unless you and Broadcom execute a separate written software license agreement governing use 
of this software, this software is licensed to you under the terms of the GNU General Public 
License version 2, available at http://www.gnu.org/copyleft/gpl.html (the "GPL"). 

Notwithstanding the above, under no circumstances may you combine this software in any way 
with any other Broadcom software provided under a license other than the GPL, without 
Broadcom's express prior written consent.
*******************************************************************************************/

/**
*
*   @file   csl_audio_render.h
*
*   @brief  This file contains the definition for render CSL driver layer
*
****************************************************************************/


#ifndef _CSL_AUDIO_RENDER_
#define _CSL_AUDIO_RENDER_

typedef void (*CSL_AUDRENDER_CB)(UInt32 streamID);

/**
*
*  @brief  initialize the audio render block
*
*  @param  source (in) The source of the audio stream. 
*  @param  sink (in) The sink of the audio stream. 
*
*  @return UInt32 the obtained stream id.
*****************************************************************************/
UInt32 csl_audio_render_init(CSL_AUDIO_DEVICE_e source, CSL_AUDIO_DEVICE_e sink);


/**
*
*  @brief  deinitialize the audio render 
*
*  @param  streamID (in) Render stream ID
*
*  @return Result_t status
*****************************************************************************/
Result_t csl_audio_render_deinit(UInt32 streamID);


/**
*
*  @brief  configure a audio render for processing audio stream
*
*  @param   sampleRate (in) Sampling rate for this stream
*  @param   numChannels (in) number of channels
*  @param   bitsPerSample (in) bits/sample
*  @param   ringBuffer (in) start address of ring buffer
*  @param   numBlocks (in) number of blocks in ring buffer
*  @param   blockSize (in) size per each block
*  @param   csl_audio_render_cb (in) cb registered by render driver
*  @param   streamID (in) stream id of this stream
*
*  @return Result_t status
*****************************************************************************/
Result_t csl_audio_render_configure(AUDIO_SAMPLING_RATE_t    sampleRate, 
						AUDIO_CHANNEL_NUM_t    numChannels,
						AUDIO_BITS_PER_SAMPLE_t bitsPerSample,
						UInt8 *ringBuffer,
						UInt32 numBlocks,
						UInt32 blockSize,
						CSL_AUDRENDER_CB csl_audio_render_cb,
						UInt32 streamID);

/**
*
*  @brief  start the stream for audio render
*
*  @param   streamID  (in) render audio stream id
*
*  @return Result_t status
*****************************************************************************/
Result_t csl_audio_render_start (UInt32 streamID);

/**
*
*  @brief  stop the stream for audio render
*
*  @param   streamID  (in) render audio stream id
*
*  @return Result_t status
*****************************************************************************/
Result_t csl_audio_render_stop (UInt32 streamID);

/**
*
*  @brief  pause the stream for audio render
*
*  @param   streamID  (in) render audio stream id
*
*  @return Result_t status
*****************************************************************************/
Result_t csl_audio_render_pause (UInt32 streamID);

/**
*
*  @brief  resume the stream for audio render
*
*  @param   streamID  (in) render audio stream id
*
*  @return Result_t status
*****************************************************************************/
Result_t csl_audio_render_resume (UInt32 streamID);

/**
*
*  @brief  get the DMA parameters
*
*  @param   streamID  (in) render audio stream id
*  @param   dma_addr  (out) rdma src address
*  @param   xfer_size  (out) transfer size
*
*  @return Result_t status
*****************************************************************************/

Result_t csl_audio_render_get_dma_params( UInt32 streamID, UInt32* dma_addr, UInt32* xfer_size );


#endif // _CSL_AUDIO_RENDER_

