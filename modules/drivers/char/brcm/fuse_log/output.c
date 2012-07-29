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
#include <linux/brcm_console.h>
#include <linux/types.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include <linux/kbd_kern.h>
#include <linux/vt_kern.h>

#include "bcmlog.h"
#include "fifo.h"
#include "bcmmtt.h"
#include "output.h"

unsigned char *fifobuf ;
unsigned char *fifoend;
#define MTT_HEAD_LEN 15

/*
 *	extern declarations
 */
extern char brcm_netconsole_register_callbacks(struct brcm_netconsole_callbacks *_cb) ;
extern char obex_register_callbacks(struct obex_callbacks *_cb);	// for ACM logging

/*
 *	forward declarations
 */
static int netconsole_start_cb( void ) ;
static int netconsole_stop_cb( void ) ;
static int acm_start_cb( void ) ;
static int acm_stop_cb( void ) ;

#ifdef BCMLOG_DEBUG_FLAG
unsigned  int g_malloc_sig_buf = 0;
static int nospace = 0;
static int noheader = 0;
static int splitbuffer = 0;
static int writefail = 0;
static int n100=0;
static unsigned int totalbyte = 0;
#endif

/**
 *	local vars
 **/
static char g_netconsole_on = 0 ;				//	flow control state for RNDIS, set/reset by flow control callbacks
static char g_acm_on = 0 ;					//	flow control state for ACM, set/reset by flow control callbacks

/*		MTT log buffer setted to be 2MB in Samsung model 
#if defined(CONFIG_BOARD_TOTORO)
#define BCMLOG_OUTPUT_FIFO_MAX_BYTES (2*1024*1024)
#else
#define BCMLOG_OUTPUT_FIFO_MAX_BYTES  65536		
#endif
*/
#define BCMLOG_OUTPUT_FIFO_MAX_BYTES (2*1024*1024)

static BCMLOG_Fifo_t g_fifo ;					//	output fifo
	
static struct brcm_netconsole_callbacks _cb =	//	RNDIS flow control callbacks
{
	.start = netconsole_start_cb,
	.stop  = netconsole_stop_cb 
} ;

/**
 *	frame counter 
 **/
static unsigned char g_frame_counter = 0 ;

struct acm_callbacks 
{
	/** Start function for role change */
	int (*start) (void);
	/** Stop Function for role change */
	int (*stop) (void);	
};

static struct acm_callbacks _acm_cb =	//	ACM flow control callbacks
{
	.start = acm_start_cb,
	.stop  = acm_stop_cb 
} ;


typedef struct 
{
	struct work_struct	wq ;	
	int					outdev; 
	int					prev_outdev; 
	struct file			*file ;
	struct file                     *acm_file ;	
	u8					busy ;
}	WriteToLogDevParms_t ;

static WriteToLogDevParms_t g_devWrParms =		//	worker thread vars
{
	.file			= 0,
	.busy			= 0,
	.prev_outdev	= BCMLOG_OUTDEV_NONE,
	.outdev			= BCMLOG_OUTDEV_NONE,
} ;

/**
 *	flow control callback for RNDIS (start flow), called when RNDIS
 *	available to transport data
 **/
static int netconsole_start_cb( void )
{
	g_netconsole_on = 1 ;
	printk( "BCMLOG: RNDIS netconsole started\n" ) ;
	return 0 ;
}

/**
 *	flow control callback for RNDIS (stop flow), called when RNDIS
 *	available to transport data
 **/
static int netconsole_stop_cb( void )
{
	g_netconsole_on = 0 ;
	printk( "BCMLOG: RNDIS netconsole stopped\n" ) ;
	return 0 ;
}

/**
 *	flow control callback for ACM (start flow), called when ACM
 *	available to transport data
 **/
static int acm_start_cb( void )
{
	g_acm_on = 1 ;
	printk( "BCMLOG: ACM device started\n" ) ;
	return 0 ;
}

/**
 *	flow control callback for ACM (stop flow), called when ACM
 *	available to transport data
 **/
static int acm_stop_cb( void )
{
	g_acm_on = 0 ;
	printk( "BCMLOG: ACM device stopped\n" ) ;
	return 0 ;
}

/*
 *	Create a file name for logging, based on system time.
 */
static void GetLogFileName( char *buf, char *rootdev, int size )
{

    struct timespec ts;
    struct rtc_time tm;

    getnstimeofday(&ts);
    rtc_time_to_tm(ts.tv_sec, &tm);

    snprintf(buf, size, 
            "%slog_mtt_%d_%02d_%02d_%02d_%02d_%02d.bin",
            rootdev, 
			tm.tm_year + 1900, 
			tm.tm_mon + 1, 
			tm.tm_mday,
            tm.tm_hour, 
			tm.tm_min, 
			tm.tm_sec);
}

#define MAX_FS_WRITE_SIZE 16384
#define MAX_LOG_PATHNAME     64

/*
 *	Write log to NAND file system
 */
int WriteToLogDev_NAND( void )
{
	mm_segment_t oldfs ;
	char		 fname[MAX_LOG_PATHNAME] ;
	struct file *pfile;

	pfile = 0;

	oldfs = get_fs( ) ;
	set_fs (KERNEL_DS);

	/*
	 *	Attempt to open log file, if not already open
	 */

		GetLogFileName( fname, "/data/log/", sizeof( fname ) ) ;		

		pfile = filp_open( fname, O_WRONLY|O_TRUNC|O_CREAT, 0666); 

		if( IS_ERR( pfile ) )
			pfile = 0 ;

	/*
	 *	If log file open start logging to it
	 */
	if( pfile )
	{
		u32 nFifo ;

		do
		{
			int nWrite ;

			nFifo = BCMLOG_FifoGetNumContig( &g_fifo ) ;
			
			if( nFifo > MAX_FS_WRITE_SIZE )
				nFifo = MAX_FS_WRITE_SIZE ;

			if( nFifo > 0 )
			{
				nWrite = pfile->f_op->write( pfile, 
					BCMLOG_FifoGetData( &g_fifo ), nFifo, &pfile->f_pos ) ;

				if( nWrite > 0 )
					BCMLOG_FifoRemove( &g_fifo, nWrite ) ;

				if( nWrite < nFifo )
				{
					nFifo = 0 ;
					filp_close( pfile ,NULL );
					pfile = 0 ;
					/*
					 * in the case of sdcard, redirect output
					 * to 'null' as the card is full or removed at
					 * this point
					 */
					 return -1;
				}
			}
		} while( nFifo > 0 );
	}
	filp_close( pfile ,NULL ); /* file close for next logging */
	pfile =0;
	set_fs( oldfs ) ;
	return 0;
}
/*
 *	Write log to SDCARD file system
 */
static void WriteToLogDev_SDCARD( void )
{
	mm_segment_t oldfs ;
	char		 fname[MAX_LOG_PATHNAME] ;

	oldfs = get_fs( ) ;
	set_fs (KERNEL_DS);

	/*
	 *	Attempt to open log file, if not already open
	 */
	if( !g_devWrParms.file )
	{
		GetLogFileName( fname, "/sdcard/", sizeof( fname ) ) ;

		g_devWrParms.file = filp_open( fname, O_WRONLY|O_TRUNC|O_CREAT, 0666); 

		if( IS_ERR( g_devWrParms.file ) )
			g_devWrParms.file = 0 ;
	}

	/*
	 *	If log file open start logging to it
	 */
	if( g_devWrParms.file )
	{
		u32 nFifo ;

		do
		{
			int nWrite ;

			nFifo = BCMLOG_FifoGetNumContig( &g_fifo ) ;
			
			if( nFifo > MAX_FS_WRITE_SIZE )
				nFifo = MAX_FS_WRITE_SIZE ;

			if( nFifo > 0 )
			{
				nWrite = g_devWrParms.file->f_op->write( g_devWrParms.file, 
					BCMLOG_FifoGetData( &g_fifo ), nFifo, &g_devWrParms.file->f_pos ) ;
				
				if( nWrite > 0 )
					BCMLOG_FifoRemove( &g_fifo, nWrite ) ;

				if( nWrite < nFifo )
				{
					nFifo = 0 ;
					filp_close( g_devWrParms.file ,NULL );
					g_devWrParms.file = 0 ;
					/*
					 * in the case of sdcard, redirect output
					 * to 'null' as the card is full or removed at
					 * this point
					 */
					 g_devWrParms.outdev = BCMLOG_OUTDEV_NONE ;
				}
			}
		} while( nFifo > 0 );
	}

	set_fs( oldfs ) ;
}

#ifdef BCMLOG_DEBUG_FLAG
static unsigned int nTotalout = 0;

static void verifyRefCount(unsigned char *pBytes, unsigned long nBytes)
{
	static u32 i = 0;
	static unsigned char frame_counter =0;
	static unsigned char old_frame_counter =0 ;
	static unsigned char firstsync  = 1;

	unsigned char expect_counter;

	while (i < nBytes)
	{
		if (pBytes[i] == MTTLOG_FrameSync0)
		{
			if ((int)fifoend < (int)(&pBytes[i+MTT_HEAD_LEN]))
			{
				pr_info("too close to fifo end!!");
				memcpy(fifoend, fifobuf, MTT_HEAD_LEN);
			}
			
			if (pBytes[i+1] == MTTLOG_FrameSync1)
		{
			if (firstsync)
			{
				frame_counter = old_frame_counter = pBytes[i+2];
				firstsync  = 0;
			} else
			{
				frame_counter = pBytes[i+2];
				expect_counter = old_frame_counter+1;
				if (frame_counter != expect_counter)
				{
					pr_info("BCMLOG: RefCount Skip!!!Time %d: %d, expect %d", 
						(pBytes[i+4]<<24)|(pBytes[i+5]<<16)|(pBytes[i+6]<<8)|(pBytes[i+7]),
						frame_counter, expect_counter);
						pr_info("FIFO end@0x%x, pByte[%d]@ 0x%x, n %d", (int)fifoend, i, (int)(&pBytes[i]), nBytes);
	 					pr_info("%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x", 
							pBytes[i], pBytes[i+1], pBytes[i+2], pBytes[i+3],
							pBytes[i+4], pBytes[i+5], pBytes[i+6], pBytes[i+7],
							pBytes[i+8], pBytes[i+9], pBytes[i+10], pBytes[i+11],
							pBytes[i+12], pBytes[i+13], pBytes[i+14], pBytes[i+15]);

				}

				old_frame_counter = frame_counter;
			}
			
				i += 15 + ((pBytes[i+10]<<8)|pBytes[i+11]);
			} else
				i++;
			
		} else
		{
		 	i++;
		}
	}

	if (i>= nBytes)
		i-= nBytes;
}
#endif

static void WriteToLogDev_RNDIS( void )
{
	
	if( g_netconsole_on )
	{
		u32 nFifo ;

		do
		{
			int nWrite ;
			unsigned char* pData;

			nFifo = BCMLOG_FifoGetNumContig( &g_fifo ) ;
			
			if( nFifo > MAX_FS_WRITE_SIZE )
				nFifo = MAX_FS_WRITE_SIZE ;

			if( nFifo > 0 )
			{
				pData = BCMLOG_FifoGetData( &g_fifo );
				nWrite = brcm_klogging(pData, nFifo ) ;
				
				if( nWrite > 0 )
				{
#ifdef BCMLOG_DEBUG_FLAG
					verifyRefCount(pData, nWrite);
					nTotalout += nWrite;
#endif
					BCMLOG_FifoRemove( &g_fifo, nWrite ) ;
				}
				
				if( nWrite < nFifo )
				{
					nFifo = 0 ;
			}
			}
		} while( nFifo > 0 );
	}
}

static void WriteToLogDev_UART( void )
{
	mm_segment_t oldfs ;

	oldfs = get_fs( ) ;
	set_fs (KERNEL_DS);

	/*
	 *	Attempt to open log file, if not already open
	 */
	if( !g_devWrParms.file )
	{
		g_devWrParms.file = filp_open( "/dev/ttyS0", O_WRONLY|O_TRUNC|O_CREAT, 0666); 

		if( IS_ERR( g_devWrParms.file ) )
			g_devWrParms.file = 0 ;
	}

	/*
	 *	If log file open start logging to it
	 */
	if( g_devWrParms.file )
	{
		u32 nFifo ;

		do
		{
			int nWrite ;

			nFifo = BCMLOG_FifoGetNumContig( &g_fifo ) ;
			
			if( nFifo > MAX_FS_WRITE_SIZE )
				nFifo = MAX_FS_WRITE_SIZE ;

			if( nFifo > 0 )
			{
				nWrite = g_devWrParms.file->f_op->write( g_devWrParms.file, 
					BCMLOG_FifoGetData( &g_fifo ), nFifo, &g_devWrParms.file->f_pos ) ;
				
				if( nWrite > 0 )
					BCMLOG_FifoRemove( &g_fifo, nWrite ) ;

				if( nWrite < nFifo )
				{
					nFifo = 0 ;
					filp_close( g_devWrParms.file ,NULL );
					g_devWrParms.file = 0 ;
				}
			}
		} while( nFifo > 0 );
	}

	set_fs( oldfs ) ;
}


/* For printing Factory String oootha (woosik.youm@samsung.com)*/
void put_factory_string(const char *s, unsigned int count)
{


	struct file *fp      = NULL;
	int nWrite ;

	fp = filp_open( "/dev/ttyS0", O_WRONLY|O_TRUNC|O_CREAT, 0666); 

	if( IS_ERR( fp ) )
		printk( "put_factory_string: filp_open failed.\n" ) ;
	else
	{
		fp->f_op->write( fp, s, count, 0 ) ;
		filp_close(fp, NULL);
	}

}


static void WriteToLogDev_ACM( void )
{
	mm_segment_t oldfs ;

	oldfs = get_fs( ) ;
	set_fs (KERNEL_DS);

	/*
	 *	Attempt to open log file, if not already open
	 */
	if( !g_devWrParms.file )
	{
		struct tty_struct *tty = NULL;
		
		g_devWrParms.file = filp_open( "/dev/ttyGS1", O_WRONLY|O_TRUNC|O_CREAT, 0666); 

		tty = g_devWrParms.file->private_data;
		tty->termios->c_iflag |= IGNBRK | ISTRIP | IGNPAR;
		tty->termios->c_oflag = 0;
		tty->termios->c_lflag = 0;
		tty->termios->c_cc[VERASE] =
		tty->termios->c_cc[VKILL]  = 0;
		tty->termios->c_cc[VMIN]   = 1;
		tty->termios->c_cc[VTIME]  = 0;
		
		if( IS_ERR( g_devWrParms.file ) )
			g_devWrParms.file = 0 ;

		g_devWrParms.acm_file = g_devWrParms.file;
	}

	/*
	 *	If log file open start logging to it
	 */
	if( g_devWrParms.file )
	{
		if (g_acm_on) {
			u32 nFifo ;
			unsigned char* pData;

			do
			{
				int nWrite ;

				nFifo = BCMLOG_FifoGetNumContig( &g_fifo ) ;
				pData = BCMLOG_FifoGetData( &g_fifo );
				
				if( nFifo > MAX_FS_WRITE_SIZE )
					nFifo = MAX_FS_WRITE_SIZE ;

				if( nFifo > 0 )
				{
					nWrite = g_devWrParms.file->f_op->write( g_devWrParms.file, 
						pData, nFifo, &g_devWrParms.file->f_pos ) ;
					
					if( nWrite > 0 )
						BCMLOG_FifoRemove( &g_fifo, nWrite ) ;

					if( nWrite < nFifo )
					{
						nFifo = 0 ;
						filp_close( g_devWrParms.file ,NULL );
						g_devWrParms.file = 0 ;
					}
				}
			} while( nFifo > 0 );	
		}
	}

	set_fs( oldfs ) ;
}

/*
 *	Write log to file on FFS 
*	On /data partition of NAND
 */
static void WriteToLogDev_FFS( void )
{
	mm_segment_t oldfs ;
	char		 fname[MAX_LOG_PATHNAME] ;
		
	oldfs = get_fs( ) ;
	set_fs (KERNEL_DS);
	/*
	 *	Attempt to open log file, if not already open
	 */
	if( !g_devWrParms.file )
	{
		GetLogFileName( fname, "/data/", sizeof( fname ) ) ;

		g_devWrParms.file = filp_open( fname, O_WRONLY|O_TRUNC|O_CREAT, 0666); 

		if( IS_ERR( g_devWrParms.file ) )
			g_devWrParms.file = 0 ;
	}

	/*
	 *	If log file open start logging to it
	 */
	if( g_devWrParms.file )
	{
		u32 nFifo ;

		do
		{
			int nWrite ;

			nFifo = BCMLOG_FifoGetNumContig( &g_fifo ) ;
			
			if( nFifo > MAX_FS_WRITE_SIZE )
				nFifo = MAX_FS_WRITE_SIZE ;

			if( nFifo > 0 )
			{
				nWrite = g_devWrParms.file->f_op->write( g_devWrParms.file, 
					BCMLOG_FifoGetData( &g_fifo ), nFifo, &g_devWrParms.file->f_pos ) ;
				
				if( nWrite > 0 )
					BCMLOG_FifoRemove( &g_fifo, nWrite ) ;

				if( nWrite < nFifo )
				{
					nFifo = 0 ;
					filp_close( g_devWrParms.file ,NULL );
					g_devWrParms.file = 0 ;
					/*
					 * in the case of sdcard, redirect output
					 * to 'null' as the card is full or removed at
					 * this point
					 */
					 g_devWrParms.outdev = BCMLOG_OUTDEV_NONE ;
				}
			}
		} while( nFifo > 0 );
	}

	set_fs( oldfs ) ;
}

extern unsigned long AcquireOutputLock( void );
extern void ReleaseOutputLock( unsigned long irql );
static void WriteToLogDev( struct work_struct *work )
{
	unsigned long irql ;

	if( g_devWrParms.prev_outdev != g_devWrParms.outdev )
	{
		if( g_devWrParms.file && g_devWrParms.file!=g_devWrParms.acm_file )
		{
			filp_close( g_devWrParms.file ,NULL );
			g_devWrParms.file = 0 ;
		}
	}
				
	switch( g_devWrParms.outdev )
	{
	case BCMLOG_OUTDEV_SDCARD:
		WriteToLogDev_SDCARD( ) ;
		break ;
	
	case BCMLOG_OUTDEV_RNDIS:
		irql = AcquireOutputLock( ) ;

		WriteToLogDev_RNDIS( ) ;
		ReleaseOutputLock( irql ) ;

		break ;

	case BCMLOG_OUTDEV_UART:
		WriteToLogDev_UART( ) ;
		break ;

	case BCMLOG_OUTDEV_ACM:
		WriteToLogDev_ACM( ) ;
		break ;
	case BCMLOG_OUTDEV_FFS:
		WriteToLogDev_FFS( ) ;
		break ;


	default:
		break ;
	}

	g_devWrParms.busy = 0 ;

}

unsigned long BCMLOG_GetFreeSize( void ) 
{
	return BCMLOG_FifoGetFreeSize(&g_fifo);
}

/**
 *	Output bytes to host
 *	@param  pBytes				(in)	pointer to user buffer
 *	@param	nBytes				(in)	number of bytes
 **/
void BCMLOG_Output( unsigned char *pBytes, unsigned long nBytes , unsigned int might_has_mtthead)
{
	/*
	 *	Add to FIFO.  If unable to add (FIFO full) then discard the message; there's nothing
	 *	that can be done to save it.
	 */
	unsigned int i = 0;

	unsigned int wrotebyte;
#if BCMLOG_LATEST_LOGS
       /*
        *      Before adding to FIFO , check if FIFO is full, remove logs from fifo ( early logs)
               and make space for latest logs in fifo so we can keep latest logs.
               When this option enabled, we would loose early logs and would get latest logs as much as possible
        */
                
       if (BCMLOG_FifoGetFreeSize(&g_fifo) < nBytes && (nBytes > 0) )
       {
               BCMLOG_FifoRemove(&g_fifo, nBytes);
       }       
#endif 
	
	if (nBytes <= BCMLOG_FifoGetFreeSize(&g_fifo) && (nBytes > 0))
	{
		if (might_has_mtthead)
		{
		while (i+12 < nBytes)
		{
			if (pBytes[i] == MTTLOG_FrameSync0 && pBytes[i+1] == MTTLOG_FrameSync1)
			{
				pBytes[i+2] = g_frame_counter++;
				pBytes[i+12] = (unsigned char)MTTLOG_Checksum16(&pBytes[i], 12);
					i += (15 + (((unsigned short)(pBytes[i+10]))<<8) + (unsigned short)(pBytes[i+11]));
				} else
				{
#ifdef BCMLOG_DEBUG_FLAG
					noheader ++;
#endif
					break;
				}
			}
			}

		wrotebyte = BCMLOG_FifoAdd( &g_fifo, pBytes, nBytes );

#ifdef BCMLOG_DEBUG_FLAG
		if (wrotebyte != nBytes)
		{
			writefail ++;
		}
		totalbyte += wrotebyte;
#endif		
		}
#ifdef BCMLOG_DEBUG_FLAG

	else if (g_netconsole_on)
	{
		nospace ++;
	}
	
	n100++; 
	if (n100 == 100)
	{
		n100 = 0;
		pr_info("nospc %d nohdr %d splt %d wrtfl %d nosigbuf %d", 
			nospace, noheader, splitbuffer, writefail, g_malloc_sig_buf);
		pr_info("IN %d OUT %d dif %d inFIFO %d", 
			totalbyte, nTotalout, totalbyte-nTotalout, BCMLOG_FifoGetDataSize(&g_fifo));
	}
#endif	
	/*
	 *	If output worker thread is not busy then set up arguments and schedule it; otherwise
	 *	we'll retry on the next output to run the thread.
	 */
	if( !g_devWrParms.busy )
	{
		g_devWrParms.busy   = 1; 
		g_devWrParms.prev_outdev = g_devWrParms.outdev ;
		g_devWrParms.outdev = BCMLOG_GetRunlogDevice( ) ;
		schedule_work( &g_devWrParms.wq ) ;
	}
}

/**
 *	Initialize output module
 **/
int BCMLOG_OutputInit( void )
{

#ifdef BCMLOG_DEBUG_FLAG
	fifobuf = kmalloc( BCMLOG_OUTPUT_FIFO_MAX_BYTES+MTT_HEAD_LEN, GFP_KERNEL ) ;

	if( !fifobuf )
        return -1 ;

	fifoend = &(fifobuf[BCMLOG_OUTPUT_FIFO_MAX_BYTES]);
	memset(fifoend, 0xff, MTT_HEAD_LEN);
#else
	fifobuf = kmalloc( BCMLOG_OUTPUT_FIFO_MAX_BYTES, GFP_KERNEL ) ;

	if( !fifobuf )
        return -1 ;

#endif

	BCMLOG_FifoInit( &g_fifo, fifobuf, BCMLOG_OUTPUT_FIFO_MAX_BYTES ) ;

	g_devWrParms.busy = 0 ;
	g_devWrParms.file = 0 ;


	INIT_WORK( &g_devWrParms.wq, WriteToLogDev ) ;

	/*
	 *	register flow control callback functions
	 */
	g_netconsole_on = brcm_netconsole_register_callbacks( &_cb ) ;
	g_acm_on = obex_register_callbacks( &_acm_cb);

	return 0 ;

}
