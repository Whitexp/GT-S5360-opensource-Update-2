/*******************************************************************************
* Copyright 2010 Broadcom Corporation.  All rights reserved.
*
*       @file   arch/arm/plat-bcmap/include/plat/osabstract/ossemaphore.h
*
* Unless you and Broadcom execute a separate written software license agreement
* governing use of this software, this software is licensed to you under the
* terms of the GNU General Public License version 2, available at
* http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
*******************************************************************************/
/**
*
*   @file   ossemaphore.h
*
*   @brief  This file contains the public interface to the OSAL Semaphore functionality.
*
****************************************************************************/
/**

*   @defgroup   RTOSSemaphoreGroup   Semaphore Services
*   @ingroup    RTOSGroup
*
*
*	The Semaphore API provides interfaces to create/destroy and control
	counting semaphores and semaphore based mutexes.

*
****************************************************************************/
/*
 * This file is taken from //depot/Sources/SystemIntegration/msp/soc/os/public/
 * on 6th Oct 2010
 */

/**
 * @addtogroup RTOSSemaphoreGroup
 * @{
 */


#ifndef _OSSEMAPHORE_H_
#define _OSSEMAPHORE_H_

#include <plat/types.h>
#include <plat/osabstract/ostypes.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>
#include <linux/slab.h>

//******************************************************************************
// Global Typedefs
//******************************************************************************

/// Semaphore creation index.

typedef UInt32 SCount_t;

//******************************************************************************
// Global Function Prototypes
//******************************************************************************

/**
	Create a counting Semphore.
	@param count	(in)	Initial count for semaphore.  A value of 0 indicates
							the semaphore is already taken.
	@param mode		(in)	Suspension Mode.
	@return	Sempahore_t	A sempahore object.
**/

static inline Semaphore_t OSSEMAPHORE_Create(			// returns newly-created semaphore
	SCount_t count,						// number of available semaphores
	OSSuspend_t mode					// suspending task mode
	)
{
	Semaphore_t *sem_ptr = (Semaphore_t *) kzalloc(sizeof(struct semaphore), GFP_KERNEL);
	sema_init((struct semaphore *)sem_ptr, count);
	return sem_ptr;
}

static inline void OSSEMAPHORE_Reset(Semaphore_t s, SCount_t count)
{
	sema_init((struct semaphore *)s, count);
}
/**
	Deallocate and destroy a sempahore.
	@param	s	(in)	Semaphore object to destory.
**/

static inline void OSSEMAPHORE_Destroy( 				// Destroy a semaphore
	Semaphore_t s						// Semaphore to destroy
	)
{
	kfree(s);
}

/**
	Obtain a semaphore.  If the semaphore count is 0 the calling task will
	block until the semphore is released.
	@param s	(in)	Semaphore Object.
	@param timeout	(in)	Ticks to wait for semaphore.
	@return OSStatus_t	Status of operation.
**/

static inline OSStatus_t OSSEMAPHORE_Obtain(			// wait to obtain a semaphore
	Semaphore_t s,						// semaphore pointer
	Ticks_t timeout						// timeout to failed semaphore obtain
	)
{
	int status = down_timeout((struct semaphore *)s, timeout);
	if(!status)
	{
		return OSSTATUS_SUCCESS;
	}
	else if(status == -ETIME)
	{
		pr_info("OSSEMAPHORE_Obtain timeout\n");
		return OSSTATUS_TIMEOUT;
	}
	else
	{
		pr_info("OSSEMAPHORE_Obtain error:%d\n", status);
		return OSSTATUS_FAILURE;
	}
}

/**
	Release a semphore that was obtained via a call to OSSEMPAHORE_Obtain.
	@param	s	(in)	Sempahore Object.
	@return OSStatus_t	Status of operation.
**/

static inline OSStatus_t OSSEMAPHORE_Release(			// wait to release a semaphore
	Semaphore_t s						// semaphore pointer
	)
{
	up((struct semaphore *)s);
	return OSSTATUS_SUCCESS;
}

/**
	Create a Mutex.
	@param	mode	(in)	Suspension Mode
	@return Semaphore_t		Semaphore Object.
**/
 
static inline Semaphore_t OSSEMAPHORE_createMutex(	// returns newly-created "mutex"
	OSSuspend_t mode					// suspending task mode
	)
{
	Semaphore_t *mutex_ptr = (Semaphore_t) kzalloc(sizeof(struct mutex), GFP_KERNEL);
	mutex_init((struct mutex *)mutex_ptr);
	return mutex_ptr;
}

/**
	Deallocate and destroy resources associated with a Mutex.
	@param	s	(in)	Sempahore Object
**/

static inline void OSSEMAPHORE_destroyMutex( 			// Destroy a "mutex"
	Semaphore_t s						// "Mutex" to destroy
	)
{
	kfree(s);
}

/**
	Obtain a mutex.  If the mutex is already "locked" the calling task
	will block until the mutex is "unlocked".
	@param s (in)	Sempahore Object.
	@param timeout (in) Ticks to wait for mutex.
	@return OSSTATUS_t	Status of operation.
**/

static inline OSStatus_t OSSEMAPHORE_obtainMutex(		// wait to obtain a "mutex"
	Semaphore_t s,						// "Mutex" pointer
	Ticks_t timeout						// timeout to failed "mutex" obtain
	)
{
	return (mutex_lock_interruptible((struct mutex *)s)? OSSTATUS_FAILURE : OSSTATUS_SUCCESS);
}

/**
	Release a mutex that was obtained via a call to OSSEMPAHORE_obtainMutex.
	@param	s	(in)	Sempahore Object.
	@return OSStatus_t	Status of operation.
**/


static inline OSStatus_t OSSEMAPHORE_releaseMutex(	// wait to release a "mutex"
	Semaphore_t s						// "mutex" pointer
	)
{
	mutex_unlock((struct mutex *)s);
	return OSSTATUS_SUCCESS;
}

	
/**
	Get current count of a counting semaphore.
	@param passed_in_s	(in)	Semaphore Object.
	@return UInt32	Semaphore count.
**/

static inline UInt32 OSSEMAPHORE_GetCnt(				// return count of semaphore,
	Semaphore_t passed_in_s				// if count <= 0, return -1
	)
{
	/* Not supported*/
	return 0;
}


/**
	Reset the count of a semaphore to zero.
	This function has high risk. Use it with extreme caution
	@param passed_in_s	(in)	Semaphore Object.
	@return UInt32	Semaphore count.
**/

static inline UInt32 OSSEMAPHORE_ResetCnt(
	Semaphore_t passed_in_s
	)
{
	/* Not supported*/
	return 0;
}


/**
	Change the name of an existing semaphore.
	@param s	(in)	Semaphore Object.
	@param name	(in)	New name of the semaphore.
**/

static inline void OSSEMAPHORE_ChangeName(
	Semaphore_t s,
	const char* name
	)
{
	/* We should define a separate structure in case this needs to be implemented*/
}

/**
	Get the name of an existing semaphore.
	@param s		(in)	Semaphore Object.
	@param p_name	(in)	Name storage (minimum 9 characters).
	@return OSStatus_t	Status of operation.
**/

static inline OSStatus_t OSSEMAPHORE_GetName(		// get ASCII name of semaphore
	Semaphore_t s,					// semaphore pointer
	UInt8 *p_name					// location to store the ASCII name
	)
{
	/* We should define a separate structure in case this needs to be implemented */
	return OSSTATUS_FAILURE;
}

/** @} */

#endif
