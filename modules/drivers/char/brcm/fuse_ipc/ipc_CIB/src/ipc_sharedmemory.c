/*
	?007-2009 Broadcom Corporation

	Unless you and Broadcom execute a separate written software license
	agreement governing use of this software, this software is licensed to you
	under the terms of the GNU General Public License version 2, available
	at http://www.gnu.org/licenses/old-licenses/gpl-2.0.html (the "GPL").


   Notwithstanding the above, under no circumstances may you combine this
   software in any way with any other Broadcom software provided under a license
   other than the GPL, without Broadcom's express prior written consent.
*/

//============================================================
// IPC_SharedMemory.c
//
//============================================================
#ifdef UNDER_LINUX
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/broadcom/csl_types.h>
#include <linux/broadcom/ipcproperties.h>
#else
#include "mobcom_types.h"
#include "ipcproperties.h"
#endif // UNDER_LINUX

#include "ipc_buffer.h"
#include "ipc_bufferpool.h"
#include "ipc_sharedmemory.h"
#include "ipc_trace.h"

#include "ipc_cppowersaving.h"
#include "ipc_appowersaving.h"

//============================================================
// Static data
//============================================================

//**************************************************
const IPC_CPU_ID_T IPC_OTHER_CPU [IPC_CPU_ID_Count] =
{
	IPC_NO_CPU,
	IPC_AP_CPU,
	IPC_CP_CPU
};


#ifdef FUSE_IPC_CRASH_SUPPORT
//**************************************************
// Reason for CP crash used by AP.
// Should be kept in sync with IPC_CrashCode_T

char *const g_IpcAPtimeOutCode[IPC_CP_MAX_CRASH_CODE] =
{
	"CRUN",	//#define	IPC_CP_NOT_CRASHED              0
	"CACR",	//#define	IPC_CP_ANALYSING_CRASH          1
	"CUCR",	//#define	IPC_CP_UNKNOWN_CRASH            2
	"CAST",	//#define	IPC_CP_ASSERT                   3
	"CDAB",	//#define	IPC_CP_DATA_ABORT               4
	"CPFH",	//#define	IPC_CP_PREFETCH_FAILURE         5
	"CUDF",	//#define	IPC_CP_UNDEFINED_INSTRUCTION    6
	"CDBZ",	//#define	IPC_CP_DIVIDED_BY_ZERO          7
	"CNFP",	//#define	IPC_CP_NULL_FUNCTION_POINTER    8
	"CSKO",	//#define	IPC_CP_STACK_OVERFLOW			9
	"ARST",	//#define	IPC_AP_RESET                    10
	"CRAI",	//#define	IPC_CP_RAISE_CALLED             11
	"CEXT",	//#define	IPC_CP_EXIT_CALLED              12
	"AAST",	//#define	IPC_AP_ASSERT					13
	"CBAP"	//#define	IPC_CP_CRASHED_BY_AP			14
};

//**************************************************
IPC_Boolean g_bCpCrashed = IPC_FALSE;

#endif //FUSE_IPC_CRASH_SUPPORT

//**************************************************
// Control structure in local memory - CPU specific data

IPC_SmLocalControl_T SmLocalControl;

//**************************************************
// Used for converting Shared Memory offsets to real C pointers

char * SmBase;

//**************************************************
// For debug
volatile IPC_SmControl		SmView;


//============================================================
// Local Macros
//============================================================

//============================================================
// Local Functions
//============================================================

//**************************************************
#define RAISE_INTERRUPT (*SmLocalControl.RaiseInterrupt)();

//**************************************************
void IPC_SmFifoWrite (IPC_Fifo Fifo, IPC_Buffer Message)
{
	CRITICAL_REIGON_SETUP

	IPC_U32		OriginalWritePointer;

	IPC_TRACE (IPC_Channel_Sm, "IPC_SmFifoWrite", "Fifo %08X, Buffer %08X", Fifo, Message, 0, 0);

	IPC_TRACE (IPC_Channel_Debug, "IPC_SmFifoWrite", "Fifo %08X,  %d entries, read: %d, Write %d",
			Fifo,
			IPC_FIFOCOUNT (Fifo),
			Fifo->ReadIndex,
			Fifo->WriteIndex);

	CRITICAL_REIGON_ENTER

	OriginalWritePointer = Fifo->WriteIndex;

	Fifo->Buffer [OriginalWritePointer]	= Message;
	Fifo->WriteIndex					= IPC_FIFOINCREMENT (OriginalWritePointer);
	Fifo->WriteCount++;

	if (IPC_FIFOCOUNT (Fifo) > Fifo->HighWaterMark)
	{
		Fifo->HighWaterMark = IPC_FIFOCOUNT (Fifo);
	}

	CRITICAL_REIGON_LEAVE

	if (OriginalWritePointer == 0)
	{
		IPC_TRACE (IPC_Channel_General, "IPC_SmFifoWrite", "Fifo %08X, Count %d, HighWaterMark %d",
			Fifo, Fifo->WriteCount, Fifo->HighWaterMark, 0);
	}

	if (Fifo->ReadIndex == OriginalWritePointer)
	{
		// Remote end is not currently reading FIFO
		IPC_TRACE (IPC_Channel_Sm, "IPC_SmFifoWrite", "Interrupting other Cpu", 0, 0, 0, 0);
		RAISE_INTERRUPT;
	}
}

//**************************************************
// Only called by the SM HISR, so no critical reigon
//
static IPC_Buffer IPC_SmFifoRead (IPC_Fifo Fifo)
{
	IPC_Buffer	Buffer;
	IPC_U32		ReadIndex;

	ReadIndex = Fifo->ReadIndex;

	IPC_TRACE (IPC_Channel_Debug, "IPC_SmFifoRead", "Fifo %08X, %d entries, read: %d, Write %d",
			Fifo,
			IPC_FIFOCOUNT(Fifo),
			ReadIndex,
			Fifo->WriteIndex);

	if (ReadIndex == Fifo->WriteIndex)
	{
		IPC_TRACE (IPC_Channel_Sm, "IPC_SmFifoRead", "Fifo %08X, Empty", Fifo, 0, 0, 0);
		return (IPC_Buffer) 0;
	}

	Buffer = Fifo->Buffer[ReadIndex];

	Fifo->ReadIndex = IPC_FIFOINCREMENT (ReadIndex);

	IPC_TRACE (IPC_Channel_Sm, "IPC_SmFifoRead", "Fifo %08X, Read  %08X, %d entries",
			Fifo, Buffer, IPC_FIFOCOUNT(Fifo), 0);

	return Buffer;
}

//============================================================
// Visible Functions
//============================================================
//**************************************************
//
void IPC_EndpointRegister
(
	IPC_EndpointId_T			EndpointId,
	IPC_FlowCntrlFPtr_T			FlowControlFunction,
	IPC_BufferDeliveryFPtr_T	DeliveryFunction,
	IPC_U32						HeaderSize
)
{
	IPC_EP_T * EP = &SmLocalControl.SmControl->Endpoints [EndpointId];

	IPC_TRACE (IPC_Channel_Sm, "IPC_EndpointRegister",
		"Id %d, HdrMax %d FcFn %08X, DeliveryFn %08X",
		EndpointId,
		HeaderSize,
		(IPC_U32) FlowControlFunction,
		(IPC_U32) DeliveryFunction
		);

	EP->Cpu					= IPC_SM_CURRENT_CPU;
	EP->DeliveryFunction	= DeliveryFunction;
	EP->FlowControlFunction	= FlowControlFunction;
	EP->MaxHeaderSize		= ALIGN4 (HeaderSize);

}

//**************************************************
#ifdef IPC_DEBUG
IPC_Endpoint IPC_SmEndpointInfo (IPC_EndpointId_T EndpointId)
{
	if (   (EndpointId > IPC_EP_None)
		&& (EndpointId < IPC_EndpointId_Count))

	{
		IPC_Endpoint EndpointPtr = &SmLocalControl.SmControl->Endpoints [EndpointId];

		switch (EndpointPtr->Cpu)
		{
		case IPC_CP_CPU:
		case IPC_AP_CPU:
			// Valid Endpoint, do nothing
		break;

		default:
			EndpointPtr = 0;
		}
		return EndpointPtr;
	} else {
		return 0;
	}
}
#endif

//**************************************************
IPC_Boolean IPC_SmIsEndpointRegistered (IPC_EndpointId_T EndpointId)
{
    IPC_Endpoint EndpointPtr = IPC_SmEndpointInfo (EndpointId);

    if(EndpointPtr == 0)
    {
        return IPC_FALSE;
    } else {
	    return IPC_TRUE;
    }
}

//**************************************************
IPC_CPU_ID_T IPC_SmCurrentCpu (void)
{
#ifdef HOST_TEST
	// No separate memory for the 2 CPUs; part of test code, keep the "extern", no need to be cleaned here.
	extern IPC_CPU_ID_T IPCTest_CurrentCpu (void);
	return IPCTest_CurrentCpu ();
#else
	return SmLocalControl.CpuId;
#endif
}


//**************************************************
IPC_SmPtr	IPC_SmAlloc (IPC_U32 Size)
{
	IPC_CPU_ID_T	Cpu = IPC_SM_CURRENT_CPU;
	volatile IPC_U32 *	ApAllocated = &SmLocalControl.SmControl->Allocated [IPC_CPU_ID_INDEX(IPC_AP_CPU)];
	volatile IPC_U32 *	CpAllocated = &SmLocalControl.SmControl->Allocated [IPC_CPU_ID_INDEX(IPC_CP_CPU)];
	IPC_SmPtr		Allocated;

	if ( (*CpAllocated - *ApAllocated) 	< Size)
	{
		IPC_TRACE (IPC_Channel_Error, "IPC_SmAlloc", "Not Enough Space", 0, 0, 0, 0);
		return 0;
	} else {
		switch (Cpu)
		{
		case IPC_AP_CPU:
		{
			// Allocate from the bottom up for AP
			Allocated = *ApAllocated;
			*ApAllocated += Size;
		}
		break;

		case IPC_CP_CPU:
		{
			// Allocate from the top down for CP
			*CpAllocated -= Size;
			Allocated = *CpAllocated;
		}
		break;

		default:
		{
			IPC_TRACE (IPC_Channel_Error, "IPC_SmAlloc", "Unknown CPU %d", Cpu, 0, 0, 0);
			return 0;
		}
		}

		if (*CpAllocated < *ApAllocated)
		{
			IPC_TRACE (IPC_Channel_Error, "IPC_SmAlloc", "Memory overallocated", 0, 0, 0, 0);
			return 0;
		}

		IPC_TRACE (IPC_Channel_Sm, "IPC_SmAlloc", "%d Bytes Allocated at %08X", Size, Allocated, 0, 0);
		return Allocated;
	}
}

//**************************************************
void	IPC_SmAddPool (IPC_BufferPool Pool)
{
	// Add to Pool list
	if (SmLocalControl.SmControl->FirstPool == 0)
	{
		SmLocalControl.SmControl->FirstPool = Pool;
	}

	if (SmLocalControl.SmControl->LastPool == 0)
	{
		SmLocalControl.SmControl->LastPool = Pool;

	} else {

		IPC_PoolNextPoolSet (SmLocalControl.SmControl->LastPool, Pool);
		SmLocalControl.SmControl->LastPool	= Pool;
	}
}

//**************************************************
IPC_SmPtr	IPC_SmPoolAlloc
(
	IPC_U32 PoolOverhead,
	IPC_U32 HeaderSize,
	IPC_U32 DataSize,
	IPC_U32 BufferCount
)
{
	IPC_SmPtr	Pool;

	if (SmLocalControl.SmControl->CurrentBuffers [IPC_CPU_ID_INDEX(IPC_SM_CURRENT_CPU)] + BufferCount >= IPC_SM_MAX_BUFFERS)
	{
		IPC_TRACE (IPC_Channel_Error, "IPC_SmPoolAlloc", "Too Many Buffers", 0, 0, 0, 0);
		return 0;
	}

	Pool =  IPC_SmAlloc (
			PoolOverhead
			+ (IPC_BufferOverhead () + HeaderSize + DataSize) * BufferCount
		);

	if (Pool == 0)
	{
		IPC_TRACE (IPC_Channel_Error, "IPC_SmPoolAlloc", "Failed to Allocate Sm", 0, 0, 0, 0);
		return 0;
	}

	SmLocalControl.SmControl->CurrentBuffers [IPC_CPU_ID_INDEX (IPC_SM_CURRENT_CPU)] += BufferCount;

	IPC_SmAddPool (Pool);

	return Pool;
}

//**************************************************
void IPC_SmSendBuffer (IPC_Buffer Buffer)
{
	IPC_EndpointId_T	DestinationEpId;
	IPC_Endpoint		DestinationEpPtr;
	IPC_CPU_ID_T		DestinationCpu;

	DestinationEpId	= IPC_BufferDestinationEndpointId (Buffer);

#ifdef IPC_DEBUG
	if (DestinationEpId == IPC_EP_None)
	{
		IPC_TRACE (IPC_Channel_Error,
			"IPC_SmSendBuffer",
			"Invalid Destination Endpoint %08X for Buffer %08X",
			DestinationEpId, Buffer, 0, 0);
		return;
	}
#endif

#ifdef FUSE_IPC_CRASH_SUPPORT
    if(g_bCpCrashed == IPC_TRUE)
    {
        IPC_TRACE (IPC_Channel_Error,
			"IPC_SmSendBuffer",
			"Trying to send after CP has crashed for Buffer %08X",
			Buffer, 0, 0, 0);
		return;
    }
#endif

	DestinationEpPtr	= IPC_SmEndpointInfo (DestinationEpId);
	DestinationCpu		= DestinationEpPtr->Cpu;

	IPC_TRACE (IPC_Channel_Sm,
		"IPC_SmSendBuffer",
		"DestinationCpu %02X, Buffer %08X",
		DestinationCpu, Buffer, 0, 0);

	if (DestinationCpu == IPC_SM_CURRENT_CPU)
	{
		(*DestinationEpPtr->DeliveryFunction) (Buffer);
	} else {
		IPC_Fifo	SendFifo = &SmLocalControl.SmControl->Fifos [IPC_CPU_ID_INDEX (DestinationCpu)].SendFifo;
		IPC_SmFifoWrite (SendFifo, Buffer);
	}
}

//**************************************************
#ifdef IPC_DEBUG
void IPC_SmFreeBuffer (IPC_Buffer Buffer, IPC_CPU_ID_T OwningCpu)
{
	IPC_Fifo FreeFifo = &SmLocalControl.SmControl->Fifos [IPC_CPU_ID_INDEX (OwningCpu)].FreeFifo;

	IPC_TRACE (IPC_Channel_Sm,
		"IPC_SmFreeBuffer",
		"Buffer %08X",
		Buffer, (IPC_U32) 0, 0, 0);

	IPC_SmFifoWrite (FreeFifo, Buffer);
}
#endif

#ifdef FUSE_IPC_CRASH_SUPPORT
//**************************************************
void IPCCP_SetCPCrashedStatus(IPC_CrashCode_T CrashCode)
{
	SmLocalControl.SmControl->CrashCode = CrashCode;

	// Just for the info that CP is crashed
	SmLocalControl.RaiseInterrupt();
}

//**************************************************
void IPCCP_SignalCrashToAP(IPC_CrashCode_T CrashCode,  void *Dump)
{
	SmLocalControl.SmControl->CrashCode			= CrashCode;
	SmLocalControl.SmControl->CrashDump         = Dump;

	// Interrupt AP so that it can start taking the crash dump out
	SmLocalControl.RaiseInterrupt();
}

//**************************************************
void IPCAP_GetCrashData(IPC_CrashCode_T *CrashCode, void **Dump)
{
	*CrashCode 		= SmLocalControl.SmControl->CrashCode;
	*Dump         	= (void **)SmLocalControl.SmControl->CrashDump;
}

//**************************************************
void IPCAP_ClearCrashData(void)
{
	SmLocalControl.SmControl->CrashCode = 0;
	SmLocalControl.SmControl->CrashDump = (void *)0;
	g_bCpCrashed = IPC_FALSE;

}

#endif //FUSE_IPC_CRASH_SUPPORT
//**************************************************

void IPC_ProcessEvents	(void)
{
	IPC_Buffer		Buffer;
	IPC_CPU_ID_T	Cpu = IPC_SM_CURRENT_CPU;

	IPC_TRACE (IPC_Channel_Hisr, "IPC_ProcessEvents", "CPU %02X", Cpu, 0, 0, 0);

	if (!SmLocalControl.ConfiguredReported)
	{

		if (  (SmLocalControl.SmControl->Initialised [IPC_CPU_ID_INDEX(Cpu)]					== IPC_SmConfigured)
		   && (SmLocalControl.SmControl->Initialised [IPC_CPU_ID_INDEX(IPC_OTHER_CPU [Cpu])]	== IPC_SmConfigured))
		{
			SmLocalControl.ConfiguredReported = IPC_TRUE;
			(*SmLocalControl.IPCInitialisedFunction) ();
		} else {
			IPC_TRACE (IPC_Channel_Error, "IPC_ProcessEvents", "CPU %02X, Not Initialised", Cpu, 0, 0, 0);
			return;
		}
	}

#ifdef FUSE_IPC_CRASH_SUPPORT
	if ((Cpu == IPC_AP_CPU) && (g_bCpCrashed == IPC_FALSE) && (SmLocalControl.SmControl->CrashCode != IPC_CP_NOT_CRASHED))
	{
		// CP has crashed so set the crash status
		g_bCpCrashed = IPC_TRUE;

		// CP should be analysing crach here as this is the first indication - but call crash function in case finished.
		if(SmLocalControl.IPCCPCrashed)
				SmLocalControl.IPCCPCrashed(SmLocalControl.SmControl->CrashCode);

	}

	if(g_bCpCrashed == IPC_TRUE)
	{
		if((SmLocalControl.SmControl->CrashCode != IPC_CP_ANALYSING_CRASH) && (SmLocalControl.SmControl->CrashCode != IPC_CP_NOT_CRASHED))
		{

			//if CP is not analysing any more than we should have crash dump now
			//call the function to handle the crash dump
			if(SmLocalControl.IPCCPCrashed)
				SmLocalControl.IPCCPCrashed(SmLocalControl.SmControl->CrashCode);
			//This function can call  IPCAP_GetCrashData to get crash location.
		}
		else
		{
			g_bCpCrashed = IPC_FALSE;
		}
	}
	else
#endif //FUSE_IPC_CRASH_SUPPORT
	{
		{
			IPC_Fifo SendFifo = SmLocalControl.SendFifo;
			while (0 != (Buffer = IPC_SmFifoRead (SendFifo)))
			{
				IPC_EndpointId_T	DestinationEpId		= IPC_PoolDestinationEndpointId (IPC_BufferOwningPool (Buffer));
				IPC_Endpoint		DestinationEpPtr	= IPC_SmEndpointInfo (DestinationEpId);

#ifdef IPC_DEBUG
				if (		DestinationEpPtr
						&&	DestinationEpPtr->DeliveryFunction
						&&	DestinationEpPtr->Cpu == IPC_SM_CURRENT_CPU
					)
				{
					IPC_TRACE (IPC_Channel_Hisr, "IPC_ProcessEvents",
						"Buffer %08X, size %d, EP %d, Fn %08X",
						Buffer,
						IPC_BufferDataSize (Buffer),
						DestinationEpId,
						(IPC_U32) DestinationEpPtr->DeliveryFunction);

					(DestinationEpPtr->DeliveryFunction) (Buffer);

				} else {
					IPC_TRACE (IPC_Channel_Error, "IPC_ProcessEvents",
						"No Delivery Function for EP %d, Function %08X, CpuId %d",
						DestinationEpId,
						(IPC_U32) DestinationEpPtr->DeliveryFunction,
						DestinationEpPtr->Cpu, 0);

					IPC_FreeBuffer (Buffer);
				}
#else
				(DestinationEpPtr->DeliveryFunction) (Buffer);
#endif
			}
		}
		{

			IPC_Fifo FreeFifo = SmLocalControl.FreeFifo;
			while (0 != (Buffer = IPC_SmFifoRead (FreeFifo)))
			{
				IPC_BufferReturn (Buffer, IPC_BufferOwningPool (Buffer));
			}
		}
	}
}

//**************************************************
void IPC_Initialise
(
	void *					Sm_BaseAddress,
	IPC_U32					Sm_Size,
	IPC_CPU_ID_T			CPUID,
	IPC_ControlInfo_T *		ControlInfo,
	IPCConfiguredFPtr_T		IPCConfigured,
	IPCResetFPtr_T			IPCReset
#ifdef FUSE_IPC_CRASH_SUPPORT
	,IPCCPCrashCbFptr_T		IPCCPCrashedCb
#endif //FUSE_IPC_CRASH_SUPPORT
)
{
	IPC_U32			CpuIndex = IPC_CPU_ID_INDEX(CPUID);
	IPC_SmControl	SmControl;

	IPC_TRACE (IPC_Channel_Sm, "IPC_Initialise", "Cpu %d, Sm_Base %08X, Sm_Size %d", CPUID, Sm_BaseAddress, Sm_Size, 0);


	SmView 									= (IPC_SmControl) Sm_BaseAddress; // For debug only
	SmBase									= Sm_BaseAddress;

	// Initialise CPU local control structure
	SmLocalControl.CpuId					= CPUID;
	SmLocalControl.SmControl				= (IPC_SmControl) Sm_BaseAddress;
	SmLocalControl.RaiseInterrupt			= ControlInfo->RaiseEventFptr;
	SmLocalControl.DisableReentrancy		= ControlInfo->DisableReEntrancyFPtr;
	SmLocalControl.EnableReentrancy			= ControlInfo->EnableReEntrancyFPtr;
	SmLocalControl.OsToPhyAddress			= ControlInfo->OSToPhyAddrFPtr;
	SmLocalControl.PhyToOSAddress			= ControlInfo->PhyToOSAddrFPtr;
	SmLocalControl.IPCInitialisedFunction	= IPCConfigured;
	SmLocalControl.IPCReset					= IPCReset;
	SmLocalControl.SendFifo					= &SmLocalControl.SmControl->Fifos [IPC_CPU_ID_INDEX (CPUID)].SendFifo;
	SmLocalControl.FreeFifo					= &SmLocalControl.SmControl->Fifos [IPC_CPU_ID_INDEX (CPUID)].FreeFifo;
	SmLocalControl.Event.Create				= ControlInfo->EventFunctions.Create;
	SmLocalControl.Event.Set				= ControlInfo->EventFunctions.Set;
	SmLocalControl.Event.Clear				= ControlInfo->EventFunctions.Clear;
	SmLocalControl.Event.Wait				= ControlInfo->EventFunctions.Wait;

	if (
		(SmLocalControl.Event.Create	== 0) ||
		(SmLocalControl.Event.Set		== 0) ||
		(SmLocalControl.Event.Clear		== 0) ||
		(SmLocalControl.Event.Wait		== 0)
		)
	{
		// Need all four to be useful
		SmLocalControl.Event.Create 			= 0;
		SmLocalControl.Event.Set				= 0;
		SmLocalControl.Event.Clear				= 0;
		SmLocalControl.Event.Wait				= 0;
	}

#ifdef FUSE_IPC_CRASH_SUPPORT
	SmLocalControl.IPCCPCrashed				= IPCCPCrashedCb;
    SmLocalControl.SmControl->CrashCode 	= IPC_CP_NOT_CRASHED;
    g_bCpCrashed = IPC_FALSE;
#endif

	// Initialise Shared Memory
	SmControl = SmLocalControl.SmControl;

	// Basic Sm Initialisation only done once
	if (SmControl->Initialised [IPC_CPU_ID_INDEX(IPC_OTHER_CPU [CPUID])] == 0)
	{
		SmControl->Size											= Sm_Size;
		SmControl->Allocated [IPC_CPU_ID_INDEX(IPC_AP_CPU)]		= sizeof (IPC_SmControl_T);
		SmControl->Allocated [IPC_CPU_ID_INDEX(IPC_CP_CPU)]		= Sm_Size;
		SmControl->FirstPool									= 0;
		SmControl->LastPool										= 0;

		/* Sleep Functionailty */
		/* This may have to be changed for Wi-Mo bootup, JPG */
		SmLocalControl.SmControl->PS.ApDeepSleepEnabled = IPC_FALSE;
		SmLocalControl.SmControl->PS.ApAccessSharedPowerDWORD = IPC_FALSE;

		SmLocalControl.SmControl->PS.CpDeepSleepEnabled = IPC_FALSE;
		SmLocalControl.SmControl->PS.CpAccessSharedPowerDWORD = IPC_FALSE;
	}

#ifdef FUSE_COMMS_PROCESSOR
	{
		IPC_CpPSInitialise((IPC_PowerSavingInfo_T *)&SmLocalControl.SmControl->PS, (IPC_PlatformSpecificPowerSavingInfo_T *)ControlInfo->PowerSavingStruct);
	}
#elif defined(FUSE_APPS_PROCESSOR)
	{
		IPC_ApPSInitialise((IPC_PowerSavingInfo_T *)&SmLocalControl.SmControl->PS, (IPC_PlatformSpecificPowerSavingInfo_T *)ControlInfo->PowerSavingStruct);
	}
#endif

	SmControl->CurrentBuffers	[CpuIndex]							= 0;

	SmControl->Fifos 			[CpuIndex].FreeFifo.ReadIndex		= 0;
	SmControl->Fifos			[CpuIndex].FreeFifo.WriteIndex		= 0;
	SmControl->Fifos			[CpuIndex].FreeFifo.WriteCount		= 0;
	SmControl->Fifos			[CpuIndex].FreeFifo.HighWaterMark	= 0;

	SmControl->Fifos			[CpuIndex].SendFifo.ReadIndex		= 0;
	SmControl->Fifos			[CpuIndex].SendFifo.WriteIndex		= 0;
	SmControl->Fifos			[CpuIndex].SendFifo.WriteCount		= 0;
	SmControl->Fifos			[CpuIndex].SendFifo.HighWaterMark	= 0;

	SmControl->Initialised		[CpuIndex] = IPC_SmInitialised;

}

//**************************************************
void IPC_Configured (void)
{
	IPC_CPU_ID_T Cpu = SmLocalControl.CpuId;

	if (Cpu == IPC_NO_CPU)
	{
		IPC_TRACE (IPC_Channel_Error, "IPC_Configured", "Not Initialised", 0, 0, 0, 0);
		return;
	}

	IPC_TRACE (IPC_Channel_Sm, "IPC_Configured", "Cpu %d", Cpu, 0, 0, 0);

	SmLocalControl.SmControl->Initialised [IPC_CPU_ID_INDEX(Cpu)] = IPC_SmConfigured;

	if (SmLocalControl.SmControl->Initialised [IPC_CPU_ID_INDEX(IPC_OTHER_CPU [Cpu])] == IPC_SmConfigured)
	{
		SmLocalControl.ConfiguredReported = IPC_TRUE;
		(*SmLocalControl.IPCInitialisedFunction) ();
		RAISE_INTERRUPT;
	}
}

//============================================================
// Sleep Mode
//============================================================

//**************************************************
void IPC_ApSleepModeSet (IPC_Boolean Setting)
{
	IPC_TRACE (IPC_Channel_Error, "IPC_ApSleepModeSet", "DONOT USE THIS FUNCTION, USE DEEPSLEEP API INSTEAD", 0, 0, 0, 0);
}

//**************************************************
IPC_Boolean IPC_ApSleepModeGet (void)
{
	return SmLocalControl.SmControl->PS.ApDeepSleepEnabled;
}

//**************************************************
IPC_PowerSavingInfo_T * IPC_ApGetAddrPSStruct(void)
{
	return (IPC_PowerSavingInfo_T *)&SmLocalControl.SmControl->PS;
}

//============================================================
//Persistent Data Store Functions
//============================================================

//**************************************************
void  IPC_GetPersistentData(IPC_PersistentDataStore_t *thePersistentData)
{
	thePersistentData->DataLength	= IPC_PERSISTENT_DATA_SIZE * sizeof(IPC_U32);
	thePersistentData->DataPtr		= (void *)&SmLocalControl.SmControl->PersistentData;
}

//============================================================
//Property Functions
//============================================================

//**************************************************
IPC_Boolean IPC_SetProperty(IPC_PropertyID_E PropertyId, IPC_U32 value)
{
	IPC_Boolean retValue = IPC_TRUE;

	if(IPC_SM_CURRENT_CPU == IPC_AP_CPU)
	{
		if((PropertyId - IPC_PROPERTY_START_AP <= IPC_PROPERTY_END_AP - IPC_PROPERTY_START_AP))
		{
			SmLocalControl.SmControl->Properties[PropertyId] = value;
		}
		else
		{
			retValue = IPC_FALSE;
		}
	}
	else //Must be CP
	{
		if((PropertyId >= IPC_PROPERTY_START_CP) && (PropertyId <= IPC_PROPERTY_END_CP))
		{
			SmLocalControl.SmControl->Properties[PropertyId] = value;
		}
		else
		{
			retValue = IPC_FALSE;
		}
	}
	return retValue;
}

//**************************************************
IPC_Boolean IPC_GetProperty(IPC_PropertyID_E PropertyId, IPC_U32 *value)
{
	IPC_Boolean retValue = IPC_TRUE;

	if(PropertyId < IPC_NUM_OF_PROPERTIES )
	{
		*value = SmLocalControl.SmControl->Properties[PropertyId];
	}
	else
	{
		retValue = IPC_FALSE;
	}
	return retValue;
}



//============================================================
// Debug Functions
//============================================================

//**************************************************
#if defined(FUSE_COMMS_PROCESSOR) || (defined(FUSE_APPS_PROCESSOR) && !defined(UNDER_CE) && !defined(UNDER_LINUX))
extern char* GetFuncNameByAddr(IPC_U32, IPC_U32);
#endif

void IPC_Dump (void)
{
	IPC_SmControl	SmControl	= SmLocalControl.SmControl;
	IPC_U32			EpIndex;
	IPC_U32			CpuIndex;

	IPC_TRACE (IPC_Channel_General, "\n===== IPC Dump =====\n", "", 0, 0, 0, 0);

	CpuIndex = IPC_CPU_ID_INDEX (IPC_AP_CPU);
	IPC_TRACE (IPC_Channel_General, "IPC_AP_Dump",
			"CpuId %d, Init %08X, Alloc %d, Buffers %d",
			IPC_AP_CPU,
			SmControl->Initialised [CpuIndex],
			SmControl->Allocated [CpuIndex],
			SmControl->CurrentBuffers [CpuIndex]);
	IPC_TRACE (IPC_Channel_General, "           ",
			"Send: RdIx %d, WrIx %d; Free: RdIx %d, WrIx %d;",
			SmControl->Fifos[CpuIndex].SendFifo.ReadIndex,
			SmControl->Fifos[CpuIndex].SendFifo.WriteIndex,
			SmControl->Fifos[CpuIndex].FreeFifo.ReadIndex,
			SmControl->Fifos[CpuIndex].FreeFifo.WriteIndex);

	IPC_TRACE (IPC_Channel_General, "           ",
			"Send: WaterMark %d, Free: WaterMark %d; Send: WriteCount %d, Free: WriteCount %d",
			SmControl->Fifos[CpuIndex].SendFifo.HighWaterMark,
			SmControl->Fifos[CpuIndex].FreeFifo.HighWaterMark,
			SmControl->Fifos[CpuIndex].SendFifo.WriteCount,
			SmControl->Fifos[CpuIndex].FreeFifo.WriteCount);

	CpuIndex = IPC_CPU_ID_INDEX (IPC_CP_CPU);
	IPC_TRACE (IPC_Channel_General, "IPC_CP_Dump",
			"CpuId %d, Init %08X, Alloc %d, Buffers %d",
			IPC_CP_CPU,
			SmControl->Initialised [CpuIndex],
			SmControl->Allocated [CpuIndex],
			SmControl->CurrentBuffers [CpuIndex]);
	IPC_TRACE (IPC_Channel_General, "           ",
			"Send: RdIx %d, WrIx %d; Free: RdIx %d, WrIx %d",
			SmControl->Fifos[CpuIndex].SendFifo.ReadIndex,
			SmControl->Fifos[CpuIndex].SendFifo.WriteIndex,
			SmControl->Fifos[CpuIndex].FreeFifo.ReadIndex,
			SmControl->Fifos[CpuIndex].FreeFifo.WriteIndex);

	IPC_TRACE (IPC_Channel_General, "           ",
			"Send: WaterMark %d, Free: WaterMark %d; Send: WriteCount %d, Free: WriteCount %d",
			SmControl->Fifos[CpuIndex].SendFifo.HighWaterMark,
			SmControl->Fifos[CpuIndex].FreeFifo.HighWaterMark,
			SmControl->Fifos[CpuIndex].SendFifo.WriteCount,
			SmControl->Fifos[CpuIndex].FreeFifo.WriteCount);

	for (EpIndex = IPC_EP_None + 1; EpIndex < IPC_EndpointId_Count; EpIndex++)
	{
		IPC_Endpoint EpPtr = &SmLocalControl.SmControl->Endpoints [EpIndex];

		EpPtr = EpPtr; // Compiler warning

		IPC_TRACE (IPC_Channel_General, "IPC_Ep_Dump",
			"Ep Name %-12.12s, Id %d, CPU %s, HeaderSize %d",
			(IPC_U32)IPC_GetEndPointName(EpIndex),
			EpIndex,
			(IPC_U32)IPC_GetCpuName(EpPtr->Cpu),
			EpPtr->MaxHeaderSize);
#if defined(FUSE_COMMS_PROCESSOR)
		if (EpPtr->DeliveryFunction && EpPtr->Cpu == IPC_CP_CPU)
		{
			IPC_TRACE (IPC_Channel_General, "           ",
				"DeliveryFunction %08X %s",
				(IPC_U32) EpPtr->DeliveryFunction,
				(IPC_U32) GetFuncNameByAddr((IPC_U32)EpPtr->DeliveryFunction, 0x4000),
				0, 0);
		}
		else
		{
			IPC_TRACE (IPC_Channel_General, "           ",
				"DeliveryFunction %08X",
				(IPC_U32) EpPtr->DeliveryFunction,
				0, 0, 0);
		}
		if (EpPtr->FlowControlFunction && EpPtr->Cpu == IPC_CP_CPU)
		{
			IPC_TRACE (IPC_Channel_General, "           ",
				"FlowControlFunction %08X %s",
				(IPC_U32) EpPtr->FlowControlFunction,
				(IPC_U32) GetFuncNameByAddr((IPC_U32)EpPtr->FlowControlFunction, 0x4000),
				0, 0);
		}
		else
		{
			IPC_TRACE (IPC_Channel_General, "           ",
				"FlowControlFunction %08X",
				(IPC_U32) EpPtr->FlowControlFunction,
				0, 0, 0);
		}
#elif defined(FUSE_APPS_PROCESSOR) && !defined(UNDER_CE) && !defined(UNDER_LINUX)
		if (EpPtr->DeliveryFunction && EpPtr->Cpu == IPC_AP_CPU)
		{
			IPC_TRACE (IPC_Channel_General, "           ",
				"DeliveryFunction %08X %s",
				(IPC_U32) EpPtr->DeliveryFunction,
				(IPC_U32) GetFuncNameByAddr((IPC_U32)EpPtr->DeliveryFunction, 0x4000),
				0, 0);
		}
		else
		{
			IPC_TRACE (IPC_Channel_General, "           ",
				"DeliveryFunction %08X",
				(IPC_U32) EpPtr->DeliveryFunction,
				0, 0, 0);
		}
		if (EpPtr->FlowControlFunction && EpPtr->Cpu == IPC_AP_CPU)
		{
			IPC_TRACE (IPC_Channel_General, "           ",
				"FlowControlFunction %08X %s",
				(IPC_U32) EpPtr->FlowControlFunction,
				(IPC_U32) GetFuncNameByAddr((IPC_U32)EpPtr->FlowControlFunction, 0x4000),
				0, 0);
		}
		else
		{
			IPC_TRACE (IPC_Channel_General, "           ",
				"FlowControlFunction %08X",
				(IPC_U32) EpPtr->FlowControlFunction,
				0, 0, 0);
		}
#else
		IPC_TRACE (IPC_Channel_General, "           ",
			"DeliveryFunction %08X, FcFunction %08X",
			(IPC_U32) EpPtr->DeliveryFunction,
			(IPC_U32) EpPtr->FlowControlFunction,
			0, 0);
#endif
	}

	IPC_PoolDumpAll (SmControl->FirstPool);
}

