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
*   @file   csl_bt_tap.h
*
*   @brief  This file contains CSL (DSP) API to BT TAP interface
*
****************************************************************************/
#ifndef _CSL_BT_TAP_H_
#define _CSL_BT_TAP_H_

// ---- Include Files -------------------------------------------------------
#include "mobcom_types.h"


// ---- Function Declarations -----------------------------------------
//*********************************************************************
/**
*
*   CSL_BTTAP_Init initializes  BT TAP output buffer.
*
* 
**********************************************************************/
void CSL_BTTAP_Init(void);

//*********************************************************************
/**
*
*   CSL_BTTAP_Read reads PCM data from shared memory for BT TAP voice record.
* 
*   @param    outBuf		(out)	destination buffer
*   @param    outSize		(in)	data size to read
*   @param    readIndex		(in)	index of ping-pong buffer
*   @param    channelSelect	(in)	channel (L or R)
*   @return   UInt32				number of bytes read to the buffer
*
**********************************************************************/
UInt32 CSL_BTTAP_Read(UInt8 *outBuf, UInt32 outSize, UInt16 readIndex, UInt16 channelSelect);


#endif //_CSL_BT_TAP_H_
