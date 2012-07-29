/****************************************************************************
*
*     Copyright (c) 2009 Broadcom Corporation
*
*   Unless you and Broadcom execute a separate written software license 
*   agreement governing use of this software, this software is licensed to you 
*   under the terms of the GNU General Public License version 2, available 
*    at http://www.gnu.org/licenses/old-licenses/gpl-2.0.html (the "GPL"). 
*
*   Notwithstanding the above, under no circumstances may you combine this 
*   software in any way with any other Broadcom software provided under a license 
*   other than the GPL, without Broadcom's express prior written consent.
*
****************************************************************************/

#ifndef __BCMLOG_CONFIG_H__
#define __BCMLOG_CONFIG_H__

#include "bcmlog.h"

#define BCMLOG_CONFIG_PS_FILE		"/data/brcm/parm_log.bin"	///<	persistent storage configuration file
#define BCMLOG_CONFIG_PROC_FILE		"brcm_logcfg"				///<	/proc file to accept configuration changes

typedef struct {
	int				logcat_dump_dev ;							// must be at beginning of struct; see brcm/dumpd/dumpd.c
	int				runlog_dev ;
	int				cp_crashdump_dev ;
	unsigned long	id_enable[ 1 + BCMLOG_MAX_LOG_ID / BITS_PER_LONG ] ;
}	BCMLOG_Config_t ;

/**
 *	Enable or disable log ID
 *	@param		inLogId (in) log id
 *	@param		inEnable (in) nonzero to enable; zero to disable log ID
 **/
void BCMLOG_EnableLogId( unsigned short inLogId, int inEnable ) ;

/**
 *	Check if log id (sender) is enabled
 *	@param		inLogId (in) log id
 *	@return		1 if enabled; 0 if not enabled or invalid ID
 **/
int BCMLOG_LogIdIsEnabled( unsigned short inLogId ) ;

/**
 *	Save or reset configuration persistent storage
 *	@param		saveFlag [in] if nonzero save configuration else reset
 *				configuration
 *	@return		0 on success, -1 on error
 **/
int BCMLOG_SaveConfig( int saveFlag ) ;

/**
 *	Initialize logging configuration.  Schedules a work thread to
 *	load the configuration file once the file system is readable.
 **/
void BCMLOG_InitConfig( void ) ;

#endif // __BCMLOG_FIFO_H__

