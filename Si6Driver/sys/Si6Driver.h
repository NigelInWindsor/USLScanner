//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
//	SI6 example
/////////////////////////////////////////////////////////////////////////////
//	SI6.h			Common header
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//	Include NT DDK standard header with C linkage

#ifdef __cplusplus
extern "C"
{
#endif
#include "ntddk.h"
#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	DebugPrint and Guid headers

#include "DebugPrint.h"

//	#include "..\..\GUIDs.h"

/////////////////////////////////////////////////////////////////////////////
//	Our device extension

typedef struct _SI6_DEVICE_EXTENSION
{
	PDEVICE_OBJECT	si6ddo;

	LONG UsageCount;		// Pending I/O Count
	bool Stopping;			// In process of stopping
	KEVENT StoppingEvent;	// Set when all pending I/O complete


	// Resource allocations
	BOOLEAN ResourceOverride;

	bool GotPort;
	bool PortInIOSpace;
	bool PortNeedsMapping;
	PUCHAR PortBase;
	PHYSICAL_ADDRESS PortStartAddress;
	ULONG PortLength;

	bool GotMemory;
	bool InMemorySpace;
	bool MemoryNeedsMapping;
	PUCHAR MemoryBase;
	PHYSICAL_ADDRESS MemoryStartAddress;
	ULONG MemoryLength;

	bool GotInterrupt;
	ULONG Vector;
	KIRQL Irql;
	KINTERRUPT_MODE Mode;
	KAFFINITY Affinity;
	PKINTERRUPT InterruptObject;

	// Interrupt handling support
	bool ConnectedToInterrupt;
	UCHAR InterruptReg;
	UCHAR InterruptRegMask;
	UCHAR InterruptRegValue;
	WORK_QUEUE_ITEM ConnectIntWQI;	// Work queue item for connecting to interrupt 
	bool ConnectIntQueued;			// Work queue item queued: StartIo doesn't complete IRP

	ULONG TxTotal;		// R/W total transfer size in bytes
	ULONG TxLeft;		// R/W bytes left to transfer
	PUCHAR TxBuffer;	// R/W buffer.  Moves through current IRP SystemBuffer
	bool TxIsWrite;		// R/W direction
	NTSTATUS TxStatus;	// R/W status return
	UCHAR TxResult[5];	// R/W output buffer (2 Failcode, 2 Offset, 1 user)
	UCHAR TxLastIntReg;	// R/W last interrupt register value
	ULONG TxCmdOutputCount;	// R/W Copy of last CmdOutputCount

	PUSHORT RunCmdsOutBuffer;// Output buffer during RUN_CMDS
	ULONG CmdOutputCount;	// Count of bytes output from commands

	UCHAR SetTimeout;	// Timeout stored from script
	int Timeout;		// Seconds left to go.  -1 if not in force
	bool StopTimer;		// Set to stop timer

	PUCHAR WriteCmds;	// Stored commands for write IRP
	ULONG WriteCmdsLen;	//							 length
	PUCHAR StartReadCmds;	// Stored commands for start read IRP
	ULONG StartReadCmdsLen;	//						 		  length
	PUCHAR ReadCmds;	// Stored commands for read IRP
	ULONG ReadCmdsLen;	//							length

	PMDL	mdl;
	struct SI6Data *pVirtualUserStruct;
	struct SI6Data *pSi6User;			//System Addrs
	int		nIrqCount;

	PMDL	mdlAxis[8];
	struct	AxisData	*pAxis[8];

	PMDL	mdlDataArray;
	char	*pDataArray;

	struct TimeSlotData*	pVirtualTimeSlot;
	PMDL	mdlTimeSlot;
	struct TimeSlotData*	pTimeSlotUser;

	bool	bCollectData;



} SI6_DEVICE_EXTENSION, *PSI6_DEVICE_EXTENSION;

/////////////////////////////////////////////////////////////////////////////
// Forward declarations of global functions

VOID SI6Unload(IN PDRIVER_OBJECT DriverObject);

NTSTATUS SI6Create(IN PDEVICE_OBJECT si6ddo,
					 IN PIRP Irp);

NTSTATUS SI6Close(	IN PDEVICE_OBJECT si6ddo,
						IN PIRP Irp);
 
NTSTATUS SI6Write(	IN PDEVICE_OBJECT si6ddo,
						IN PIRP Irp);

NTSTATUS SI6Read(	IN PDEVICE_OBJECT si6ddo,
					IN PIRP Irp);

NTSTATUS SI6DeviceControl(	IN PDEVICE_OBJECT si6ddo,
								IN PIRP Irp);

VOID SI6StartIo(	IN PDEVICE_OBJECT si6ddo,
					IN PIRP Irp);

VOID SI6DpcForIsr(IN PKDPC Dpc, IN PDEVICE_OBJECT si6ddo, 
					IN PIRP Irp, IN PSI6_DEVICE_EXTENSION dx);

VOID Timeout1s( IN PDEVICE_OBJECT si6ddo, IN PSI6_DEVICE_EXTENSION dx);

VOID SI6CancelIrp( IN PDEVICE_OBJECT si6ddo, IN PIRP Irp);

NTSTATUS SI6DispatchCleanup( IN PDEVICE_OBJECT si6ddo, IN PIRP Irp);

VOID IrqConnectRoutine( IN PVOID Context);

/////////////////////////////////////////////////////////////////////////////

bool LockDevice( IN PSI6_DEVICE_EXTENSION dx);
void UnlockDevice( IN PSI6_DEVICE_EXTENSION dx);

VOID StopDevice(	IN PSI6_DEVICE_EXTENSION dx);

#define FreeIfAllocated(x) if( (x)!=NULL) { ExFreePool(x); (x) = NULL; }

/////////////////////////////////////////////////////////////////////////////

NTSTATUS CompleteIrp( IN PIRP Irp, IN NTSTATUS status, IN ULONG info=0, IN CCHAR PriorityBoost=IO_NO_INCREMENT);

/////////////////////////////////////////////////////////////////////////////
