//
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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <linux/errno.h>

#include "config.h"
#include "output.h"

static struct delayed_work g_load_config_wq ;			///< work queue for reading config file
static BCMLOG_Config_t g_config ;						///< log configuration
static struct proc_dir_entry *g_proc_dir_entry = NULL ;	///< procfs file

/**
 *	Set configuration defaults
 **/
static void SetConfigDefaults( void )
{
	g_config.logcat_dump_dev  = BCMLOG_OUTDEV_NONE ;	// logcat crash dump disabled
	g_config.runlog_dev       = BCMLOG_OUTDEV_NONE ;	// set default MTT log path to NONE
	g_config.cp_crashdump_dev = BCMLOG_CPCRASH_MTD ;	// CP dump to SD card
}

/**
 *	'safe' strncpy
 **/
static void safe_strncat( char *dst, const char *src, int len )
{
	int l_dst = strlen( dst ) ;
	int l_src = strlen( src ) ;
	if( l_dst + l_src < len )
	{
		strncat( dst, src, len - l_dst - l_src ) ;
	}
}

/**
 *	build device status string and append to 'buf'
 **/
static void bld_device_status_str( char *buf, int len, char *label, int device )
{
	safe_strncat( buf, label, len ) ;

	switch( device )
	{
	case BCMLOG_OUTDEV_NONE:
		safe_strncat( buf, "-> disabled\n", len ) ;
		break ;
	case BCMLOG_OUTDEV_PANIC:
		safe_strncat( buf, "-> flash\n", len ) ;
		break ;
	case BCMLOG_OUTDEV_RNDIS:
		safe_strncat( buf, "-> MTT\n", len ) ;
		break ;
	case BCMLOG_OUTDEV_SDCARD:
		safe_strncat( buf, "-> SD card\n", len ) ;
		break ;
	case BCMLOG_OUTDEV_UART:
		safe_strncat( buf, "-> UART\n", len ) ;
		break ;
	case BCMLOG_OUTDEV_ACM:
		safe_strncat( buf, "-> USB serial\n", len ) ;
		break ;
	case BCMLOG_OUTDEV_FFS:
		safe_strncat( buf, "-> FFS ,write on /data partition\n", len ) ;
		break ;
	case BCMLOG_CPCRASH_MTD:
		safe_strncat( buf, "-> FFS on /data/dontpanic \n", len ) ;
		break ;
	case BCMLOG_CPCRASH_SDCARD:
		safe_strncat( buf, "-> CP dump /sdcard\n", len ) ;
		break ;
	case BCMLOG_CPCRASH_BMTT:
		safe_strncat( buf, "-> CP dump MTT\n", len ) ;
		break ;

	default:
		safe_strncat( buf, "-> ERROR\n", len ) ;
		break ;
	}
}

/**
 *	proc read handler
 **/
static int proc_read(char *page, char **start, off_t offset, int count, int *eof, void *data)
{
	*page = 0 ;
	bld_device_status_str( page, count, "    BMTT logging", g_config.runlog_dev ) ;
	bld_device_status_str( page, count, "  APP crash dump", g_config.logcat_dump_dev ) ;
	bld_device_status_str( page, count, "  CP crash dump ", g_config.cp_crashdump_dev ) ;
	*eof = 1 ;
	return 1+strlen( page ) ;
}

/**
 *	proc write handler
 *
 *		a - BMTT logging   -> RNDIS
 *		b - BMTT logging   -> USB serial
 *		c - BMTT logging   -> UART
 *		d - BMTT logging   -> SD card
 *		e - APP crash dump -> flash
 *		f - APP crash dump -> SD card
 *		g - APP crash dump -> disabled
 *		h - Save for reboot
 *		i - Restore defaults
 **/
static ssize_t proc_write(struct file *file, const char *buffer, unsigned long count, void *data)
{	
	if( count > 0 )
	{
		switch( *buffer )
		{
		case 'a': 	
			g_config.runlog_dev = BCMLOG_OUTDEV_RNDIS ;
			break ;
		case 'b': 	
			g_config.runlog_dev = BCMLOG_OUTDEV_ACM ;
			break ;
		case 'c': 
			g_config.runlog_dev = BCMLOG_OUTDEV_UART ;
			break ;
		case 'd':	
			g_config.runlog_dev = BCMLOG_OUTDEV_SDCARD ;
			break ;
		case 'e':	
			g_config.logcat_dump_dev = BCMLOG_OUTDEV_PANIC ;
			break ;
		case 'f': 
			g_config.logcat_dump_dev = BCMLOG_OUTDEV_SDCARD ;
			break ;
		case 'g': 	
			g_config.logcat_dump_dev = BCMLOG_OUTDEV_NONE ;
			g_config.runlog_dev = BCMLOG_OUTDEV_NONE ;
			break ;
		case 'h': 
			BCMLOG_SaveConfig( 1 ) ;
			break ;
		case 'i': 
			SetConfigDefaults( ) ;
			BCMLOG_SaveConfig( 0 ) ;
			break ;
		case 'x' :
			WriteToLogDev_NAND();
			break;
		case 'j':
			g_config.runlog_dev = BCMLOG_OUTDEV_FFS ;
			break ;
		case 'k':
			g_config.cp_crashdump_dev = BCMLOG_CPCRASH_MTD ;
			break;
		case 'l': 
			g_config.cp_crashdump_dev = BCMLOG_CPCRASH_SDCARD ;
			break;
		case 'n': 
			g_config.cp_crashdump_dev = BCMLOG_CPCRASH_BMTT ;
			break;
			
		default:
			break ;
		}
	}

	return count ;
}

/**
 *	Load configuration from persistent storage
 **/
static int LoadConfigFromPersistentStorage( void )
{
	mm_segment_t oldfs ;
	struct file* config_file ;
	int rc = 0 ;

	oldfs = get_fs( ) ;
	set_fs (KERNEL_DS);

	config_file = filp_open( BCMLOG_CONFIG_PS_FILE, O_RDONLY, 0 ); 

	if( IS_ERR( config_file ) )
	{
		rc = -1 ;
	}

	else
	{
		if( sizeof(BCMLOG_Config_t)!= config_file->f_op->read( config_file, (void*)&g_config, 
				sizeof(BCMLOG_Config_t), &config_file->f_pos ) )
		{
			rc = -1 ;
		}

		filp_close( config_file ,NULL );
	}
		
	set_fs( oldfs ) ;

	return rc ;
}

/**
 *	Work thread to read configuration file.  Called at driver
 *	initialization, will reschedule itself periodically until
 *	configuration is successfully loaded, up to predetermined
 *	number of attempts.  Delay is necessary to wait until
 *	file system is available.
 *   
 *	@param	(in)	ptr to work structure
 *	@note
 *		Function prototype as required by INIT_DELAYED_WORK macro.
 *	@note
 *		There is a one-second delay between each attemt.  It was 
 *		determined emperically that 5 - 6 seconds is about
 *		the time required for FS to come up, so we'll give 
 *		it up to 10 seconds.
 **/
static void try_load_config( struct work_struct *work )
{
	static int tries = 10 ;

	if( --tries > 0 )
		if( LoadConfigFromPersistentStorage( ) < 0 )
			schedule_delayed_work( &g_load_config_wq, 1*HZ ) ;
}

/**
 *	Save or reset configuration persistent storage
 *	@param		saveFlag [in] if nonzero save configuration else reset
 *				configuration
 *	@return		0 on success, -1 on error
 **/
int BCMLOG_SaveConfig( int saveFlag )
{
	mm_segment_t oldfs ;
	struct file* config_file ;
	int rc = 0 ;

	oldfs = get_fs( ) ;
	set_fs (KERNEL_DS);

	config_file = filp_open( BCMLOG_CONFIG_PS_FILE, O_WRONLY|O_TRUNC|O_CREAT, 0666); 

	if( IS_ERR( config_file ) )
		rc = -1 ;

	else
	{
		if( saveFlag )
			if( sizeof(BCMLOG_Config_t) != config_file->f_op->write( config_file, (void*)&g_config, 
				sizeof(BCMLOG_Config_t), &config_file->f_pos ) )
				rc = -1 ;

		// if !saveFlag the file is truncated to zero bytes, invalidating the configuration
		filp_close( config_file ,NULL );
	}
		
	set_fs( oldfs ) ;

	return rc ;
}

/**
 *	Enable or disable log ID
 *	@param		inLogId (in) log id
 *	@param		inEnable (in) nonzero to enable; zero to disable log ID
 **/
void BCMLOG_EnableLogId( unsigned short inLogId, int inEnable )
{
	unsigned long index ;
	unsigned long bit ;
	
	if( inLogId < BCMLOG_MAX_LOG_ID )
	{
		index = inLogId / BITS_PER_LONG ;
		bit   = inLogId % BITS_PER_LONG ;
						
		if( inEnable )
			g_config.id_enable[index] |= ( 1 << bit ) ;
		else
			g_config.id_enable[index] &= ~( 1 << bit ) ;
	}
}

/**
 *	Check if log id (sender) is enabled
 *	@param		inLogId (in) log id
 *	@return		1 if enabled; 0 if not enabled or invalid ID
 **/
int BCMLOG_LogIdIsEnabled( unsigned short inLogId )
{
	unsigned long index ;
	unsigned long bit ;
	
	if( inLogId >= BCMLOG_MAX_LOG_ID )
	{
		return 0 ;
	}
	
	index = inLogId / BITS_PER_LONG ;
	bit   = inLogId % BITS_PER_LONG ;
					
	return g_config.id_enable[index] & ( 1 << bit ) ? 1 : 0 ;
}

/**
 *	Initialize logging configuration.  Schedules a work thread to
 *	load the configuration file once the file system is readable.
 **/
void BCMLOG_InitConfig( void ) 
{
	/*
	 *	disable all AP logging (CP logging is handled by CP) [MobC00126731]
	 */
	memset( &g_config, 0x00, sizeof( g_config ) ) ;
	
	/*
	 *	set default configuration
	 */
	SetConfigDefaults( ) ;
	
	/*
	 *	create the procfs entry
	 */
	g_proc_dir_entry = create_proc_entry( BCMLOG_CONFIG_PROC_FILE, S_IRWXU | S_IRWXG | S_IRWXO, NULL );

	if (g_proc_dir_entry == NULL) 
	{
		remove_proc_entry( BCMLOG_CONFIG_PROC_FILE, NULL); 
	}

	else
	{
		g_proc_dir_entry->read_proc  = proc_read;
		g_proc_dir_entry->write_proc = proc_write;
	}

	/*
	 *	start a thread to attempt to load configuration when filesystem ready
	 */
	INIT_DELAYED_WORK( &g_load_config_wq, try_load_config ) ;
	
	schedule_delayed_work( &g_load_config_wq, 1*HZ ) ;
}

int BCMLOG_GetRunlogDevice( void ) 
{
	return g_config.runlog_dev ;
}

int BCMLOG_GetCpCrashDumpDevice( void )
{
	return g_config.cp_crashdump_dev ;
}

