/*******************************************************************************
* Copyright 2010 Broadcom Corporation.  All rights reserved.
*
*       @file   arch/arm/plat-bcmap/include/plat/osabstract/osqueue.h
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
*   @file   osqueue.h
*
*   @brief  This file contains the public interface to the OSAL Message Queue functionality.
*
****************************************************************************/
/**

*   @defgroup   RTOSQUEUEGroup   Message Queue Services
*   @ingroup    RTOSGroup
*
*	The Queue API provides interfaces to the send and recieve messages
	between tasks. A task can only pend on one queue and the messages will be handled
	by that task in the order they are placed in the queue. Messages can be posted to
	either the front or the back of the queue.
*
****************************************************************************/

/*
 * This file is taken from //depot/Sources/SystemIntegration/msp/soc/os/public/
 * on 6th Oct 2010
 */

/**
 * @addtogroup RTOSQUEUEGroup
 * @{
 */

#ifndef _OSQUEUE_H_
#define _OSQUEUE_H_

#include <plat/types.h>
#include <plat/osabstract/ostypes.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/completion.h>
#include <linux/hardirq.h>

#ifdef	__cplusplus
extern "C"
{
#endif

//******************************************************************************
// Global Typedefs
//******************************************************************************

/// Number of entries a Queue can hold.

typedef UInt32 QEntries_t;

/// Size of an entry within the Queue.

typedef UInt32 QMsgSize_t;

/// Queue Message.

typedef UInt8  QMsg_t;

typedef UInt32 QMem_t;


/*   */
struct QueueLLI_t {
	struct list_head head;
	UInt32 *msg_ptr;
};

/*   */
struct QueueStruct_t {
	UInt32 msg_size;
	UInt32 max_entries;
	struct mutex q_mutex;
	struct completion msg_queued;
	struct QueueLLI_t q_list;
};

//******************************************************************************
// Global Function Prototypes
//******************************************************************************

/**
	Create a queue.
	@param entries	(in) Number of entries in the Queue.
	@param msg_size	(in) Size of an entry in the Queue.
	@param mode		(in) Pending Method
	@return Queue_t	Queue object associated with created Queue.
**/

static inline Queue_t OSQUEUE_Create(					// returns newly-created queue
	QEntries_t entries,					// number of entries
	QMsgSize_t msg_size,				// fixed-size message size (in QMsg_t)
	OSSuspend_t mode					// pending method
	)
{
	struct QueueStruct_t *new_queue = kzalloc(sizeof(struct QueueStruct_t), GFP_KERNEL);
	if(new_queue)
	{
		new_queue->msg_size = msg_size;
		new_queue->max_entries = entries;
		mutex_init(&new_queue->q_mutex);
		init_completion(&new_queue->msg_queued);
		INIT_LIST_HEAD(&new_queue->q_list.head);
	}
	return (Queue_t *)new_queue;
}

/**
	Flush all entries in a Queue.
	@param	q	(in) Queue to flush.
**/

static inline void OSQUEUE_FlushQ(					// clear queue
	Queue_t q							// queue pointer
	)
{
	struct QueueStruct_t *queue = (struct QueueStruct_t *)q;
	struct QueueLLI_t *tmpLLI;
	struct list_head *pos;
	list_for_each(pos, &queue->q_list.head)
	{
		tmpLLI = list_entry(pos, struct QueueLLI_t, head);
		if(tmpLLI->msg_ptr) {
			kfree(tmpLLI->msg_ptr);
		}
		if (in_interrupt()) {
			if (!mutex_trylock(&queue->q_mutex))
				return;
		} else
			mutex_lock_interruptible(&queue->q_mutex);
		list_del_init(&tmpLLI->head);
		kfree(tmpLLI);
		mutex_unlock(&queue->q_mutex);
	}
}

/**
	Destroy the passed Queue and release all resources associated with the Queue.
	@param q	(in) Queue to destroy.
**/

static inline void OSQUEUE_Destroy(					// Destroys all resources associated
	Queue_t	q							// with the passed queue
	)
{
	OSQUEUE_FlushQ(q);
	kfree(q);
}

/**
	Pend on a Queue to receive a message.  If the timeout is set to TICKS_FOREVEVER
	and no message is available in the Queue the pending task will block until a
	message is available.
	@param	q		(in)	Queue to pend on.
	@param *msg		(in)	Pointer to where to put message.
	@param timeout	(in)	Timeout value
	@return OSSTATUS_t	Result of operation.
**/

static inline OSStatus_t OSQUEUE_Pend(				// get message from the queue
	Queue_t q,							// queue pointer
	QMsg_t *msg,						// pointer to message
	Ticks_t timeout						// timeout to pend failure
	)
{
	OSStatus_t status = OSSTATUS_FAILURE;
	struct QueueStruct_t *queue = (struct QueueStruct_t *)q;
	struct QueueLLI_t *temp_ptr;

	if (unlikely(!queue))
        return status;

	if (in_interrupt())
		return status;

	if(!wait_for_completion_interruptible_timeout(&queue->msg_queued, timeout)) {
		status = OSSTATUS_TIMEOUT;
		return status;
	}

	mutex_lock_interruptible(&queue->q_mutex);
	temp_ptr = list_first_entry(&(queue->q_list.head), struct QueueLLI_t, head);

	if (msg && temp_ptr && temp_ptr->msg_ptr && queue)
		memcpy(msg, temp_ptr->msg_ptr, queue->msg_size);
	else
		pr_info("Error: Node ptr=0x%p msg_ptr=0x%p\n", temp_ptr, temp_ptr->msg_ptr);
	kfree(temp_ptr->msg_ptr);
	list_del_init(&temp_ptr->head);
	kfree(temp_ptr);
	mutex_unlock(&queue->q_mutex);

	return status;
}

/**
	Post a message to a Queue.  If the Queue is full and the timeout is set to TICKS_FOREVER
	the posting task will block until space is available on the queue.
	@param	q		(in)	Queue to post message to.
	@param	*msg	(in)	Pointer to message to post.
	@param	timeout	(in)	TICKS_NO_WAIT or TICKS_FOREVER
	@param post_to_tail	(in)	TRUE for normal queuing; FALSE to queue at the beginning of the queue
	@return OSSTATUS_t		Result of operation.
**/

static inline OSStatus_t OSQUEUE_PostGeneric( 					// internal method to post msg
	Queue_t q,						// queue pointer
	QMsg_t *msg,						// pointer to empty message
	Ticks_t timeout,					// TICKS_NO_WAIT or TICKS_FOREVER
	bool post_to_tail					// set to TRUE for normal queuing; FALSE to queue at the beginning of the queue
	)
{
	OSStatus_t status = OSSTATUS_FAILURE;
	struct QueueStruct_t *queue = (struct QueueStruct_t *)q;
	struct QueueLLI_t *tmp = (struct QueueLLI_t *) kzalloc(sizeof(struct QueueLLI_t), GFP_KERNEL);
	if(tmp)
	{
		tmp->msg_ptr = (UInt32 *) kzalloc(queue->msg_size, GFP_KERNEL);
		if(tmp->msg_ptr) {
			if (in_interrupt()) {
				if (!mutex_trylock(&queue->q_mutex)) {
					kfree(tmp->msg_ptr);
					kfree(tmp);
					return status;
				}
			} else
				mutex_lock_interruptible(&queue->q_mutex);
			if(post_to_tail) {
				list_add_tail(&(tmp->head), &(queue->q_list.head));
			} else {
				list_add(&(tmp->head), &(queue->q_list.head));
			}
			memcpy((QMsg_t *)tmp->msg_ptr, msg, queue->msg_size);
			mutex_unlock(&queue->q_mutex);
			complete(&queue->msg_queued);
			status = OSSTATUS_SUCCESS;
		} else {
			pr_info("Couldn't allocate memory for msg\n");
            		kfree(tmp);
		}
	} else {
        pr_info("Couldn't allocate memory for queueLLI\n");
	}
	return status;
}

/**
	Post a message to a Queue.  If the Queue is full and the timeout is set to TICKS_FOREVER
	the posting task will block until space is available on the queue.
	@param	q		(in)	Queue to post message to.
	@param	*msg	(in)	Pointer to message to post.
	@param	timeout	(in)	TICKS_NO_WAIT or TICKS_FOREVER
	@return OSSTATUS_t		Result of operation.
**/

#define OSQUEUE_Post(q,msg,timeout) OSQUEUE_PostGeneric(q, msg, timeout, TRUE)

/**
	Post a message to the front of a Queue.  The message becomes the first message in the
	Queue.
	@param	q		(in)	Queue to post message to.
	@param	*msg	(in)	Pointer to message to post.
	@param	timeout	(in)	TICKS_NO_WAIT or TICKS_FOREVER
	@return OSSTATUS_t		Result of operation.
**/

#define OSQUEUE_Post_PostToFront(q,msg,timeout) OSQUEUE_PostGeneric(q, msg, timeout, FALSE)

/**
	Query if a Queue is Empty.
	@param q	(in)	Queue to query.
	@return Boolean	TRUE if Empty and FALSE otherwise.
**/

static inline Boolean OSQUEUE_IsEmpty(				// Check if there are any messages in queue
	Queue_t q							// queue pointer
	)
{
	return TRUE;
}

/**
	@cond internal
	Get number of messages currently queued. 
	@param	q	(in)	Queue object.
	@return UInt32	Number of messages queue.
**/

static inline UInt32 *OSQUEUE_GetCountAddr(			// Get number of messages currently queued. 
	Queue_t q
	)
{
	return 0;
}

/** @endcond */

/**
	Change the name of an existing queue.
	@param	queue	(in)	Queue object
	@param	name	(in)	New name of the queue
**/

static inline void OSQUEUE_ChangeName(
	Queue_t queue,
	const char* name
	)
{

}

/**
	Query if a Task is idlely pending at its input Queue.
	@param t	(in)	Task to query.
	@param q	(in)	Task's input Queue.
	@return Boolean	TRUE if Idle and FALSE otherwise.
**/

static inline Boolean OSQUEUE_IsTaskIdle(
	Task_t t,							// task (pointer)
	Queue_t q							// task's main queue (pointer)
	)
{
	return TRUE;
}

/**
	Get the name of a Queue.
	@param q		(in)	Queue object.
	@param p_name	(in)	Name storage (minimum 9 characters).
	@return OSSTATUS_t		Result of operation.
**/

static inline OSStatus_t OSQUEUE_GetName(		// get ASCII name of Queue
	Queue_t q,					// Queue pointer
	UInt8 *p_name				// location to store the ASCII name
	)
{
	return OSSTATUS_SUCCESS;
}

/** @} */

#ifdef	__cplusplus
}
#endif

#endif

