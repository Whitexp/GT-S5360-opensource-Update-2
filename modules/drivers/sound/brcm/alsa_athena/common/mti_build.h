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
*   @file   mti_build.h
*
*   @brief  This file contains target build option defines. 
*
****************************************************************************/ 

#ifndef _INC_MTI_BUILD_H_
#define _INC_MTI_BUILD_H_

// platform flag
#define  OS_NUCLEUS			1
#define  OS_WIN32			2
#define  OS_LINUX			3


// determine target platform
#ifdef UNDER_LINUX
#define  TARGET_OS				OS_LINUX
#elif defined(WIN32)	
#define  TARGET_OS				OS_WIN32
#else		// default to NECLEUS
#define  TARGET_OS				OS_NUCLEUS
#endif

#endif // _INC_MTI_BUILD_H_

