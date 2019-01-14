//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
//	USL example
/////////////////////////////////////////////////////////////////////////////
//	DeviceIo.cpp:	Routines that interact with a device
/////////////////////////////////////////////////////////////////////////////
//	StopDevice			Stop device
//*	WriteByte			Output a byte
//*	ReadByte			Input a byte
//	USLStartIo		Process an IRP from the head of the device IRP queue.
//*	StoreCmds			Copy commands from IOCTL input buffer to new buffer (in dx)
//*	RunCmds				Run commands for IOCTL_USL_RUN_CMDS
//*	RunCmdsSynch		RunCmds called as a Critical Section routine
//*	RunWriteCmdsSynch	Run stored write commands
//*	RunReadCmdsSynch	Run stored read commands
//*	RunStartReadCmdsSynch	Run stored start read commands
//*	ProcessCmds			Process commands in given buffer.
//*	IrqConnectRoutine	Work queue item to connect to an interrupt at PASSIVE_LEVEL
//*	InterruptHandler	Handle interrupts (during StartIo processing of R/W)
//	USLDpcForIsr		Complete current IRP
//*	Timeout1sSynch		Timeout check 
//	Timeout1s			One second timer call
//	USLCancelIrp		Cancel this IRP
//*	CancelCurrentIrpSynch	If a transfer is in progress, mark it for cancelling
//	USLCleanup		Handle IRP_MJ_CLEANUP requests
/////////////////////////////////////////////////////////////////////////////

#include "USLDriver.h"
//#include "USLIoctl.h"
#include "C:\WindowsProjects\USLScanner\USLDriver\sys\USLIoctl.h"
#include "C:\WindowsProjects\USLScanner\Si6Driver\sys\Si6Ioctl.h"
#include "C:\WindowsProjects\USLScanner\ServoDriver\sys\ServoIoctl.h"
#include "windef.h"

/////////////////////////////////////////////////////////////////////////////

NTSTATUS StoreCmds( PUCHAR* pCmds, ULONG* pCmdsLen, ULONG len, PVOID Buffer);
void WriteByte( IN PUSL_DEVICE_EXTENSION dx, IN ULONG offset, IN UCHAR byte);
UCHAR ReadByte( IN PUSL_DEVICE_EXTENSION dx, IN ULONG offset);
WORD ReadWord( IN PUSL_DEVICE_EXTENSION dx, IN ULONG offset);
BOOLEAN InterruptHandler(IN PKINTERRUPT Interrupt, IN PUSL_DEVICE_EXTENSION dx);

BOOLEAN RunCmdsSynch( IN PDEVICE_OBJECT uslddo);
BOOLEAN RunCmds( IN PDEVICE_OBJECT uslddo, IN bool CanTrace);
bool ProcessCmds(	IN PUSL_DEVICE_EXTENSION dx,
					IN PUCHAR Buffer, IN ULONG len,
					OUT PUCHAR OutBuffer, IN ULONG outlen,
					IN bool CanTrace
				  );

BOOLEAN RunWriteCmdsSynch( IN PUSL_DEVICE_EXTENSION dx);
BOOLEAN RunReadCmdsSynch( IN PUSL_DEVICE_EXTENSION dx);
BOOLEAN RunStartReadCmdsSynch( IN PUSL_DEVICE_EXTENSION dx);

//USL Procedures
BOOLEAN DspNonMuxIsr( IN PUSL_DEVICE_EXTENSION dx);
BOOLEAN DspMuxIsr( IN PUSL_DEVICE_EXTENSION dx);
BOOLEAN CollectBscanData( IN PUSL_DEVICE_EXTENSION dx);
BOOLEAN ProcessTrace( IN PUSL_DEVICE_EXTENSION dx);

char Trace[8192];
char ImageTrace[8192];


/////////////////////////////////////////////////////////////////////////////
//	StopDevice:	Stop device

VOID StopDevice( IN PUSL_DEVICE_EXTENSION dx)
{
	DebugPrintMsg("StopDevice");

	// Unmap memory
	if (dx->PortNeedsMapping)
	{
		MmUnmapIoSpace( (PVOID)dx->PortBase, dx->PortLength);
		dx->PortNeedsMapping = false;
	}

	// Disconnect from interrupt
	if( dx->ConnectedToInterrupt)
	{
		IoDisconnectInterrupt( dx->InterruptObject);
		dx->ConnectedToInterrupt = false;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	WriteByte:	Output a byte
//				Silently ignores request if register out of range
//	Don't call DebugPrint as may be called at DIRQL

void WriteByte( IN PUSL_DEVICE_EXTENSION dx, IN ULONG offset, IN UCHAR byte)
{
	if( offset>=dx->PortLength) return;
	PUCHAR Port = dx->PortBase+offset;
	if( dx->PortInIOSpace)
		WRITE_PORT_UCHAR( Port, byte);
	else
		WRITE_REGISTER_UCHAR( Port, byte);
}

/////////////////////////////////////////////////////////////////////////////
//	ReadByte:	Input a byte
//				Silently ignores request if register out of range
//	Don't call DebugPrint as may be called at DIRQL

UCHAR ReadByte( IN PUSL_DEVICE_EXTENSION dx, IN ULONG offset)
{
	if( offset>=dx->PortLength) return 0;
	PUCHAR Port = dx->PortBase+offset;
	UCHAR b;
	if( dx->PortInIOSpace)
		b = READ_PORT_UCHAR(Port);
	else
		b = READ_REGISTER_UCHAR(Port);
	return b;
}
/////////////////////////////////////////////////////////////////////////////
//	ReadWord:	Input a word
//				Silently ignores request if register out of range
//	Don't call DebugPrint as may be called at DIRQL

WORD ReadWord( IN PUSL_DEVICE_EXTENSION dx, IN ULONG offset)
{
	if( offset>=dx->PortLength) return 0;
	PUCHAR Port = dx->PortBase+offset;
	WORD b;
	if( dx->PortInIOSpace)
		b = ((WORD)READ_PORT_UCHAR(Port+1)<<8)|READ_PORT_UCHAR(Port);
	else
		b = ((WORD)READ_REGISTER_UCHAR(Port+1)<<8)|READ_REGISTER_UCHAR(Port);
	return b;
}
/////////////////////////////////////////////////////////////////////////////
//	USLStartIo:	Process an IRP from the head of the device IRP queue.
//				1	Only IOCTL, Read and Write IRPs get here.
//				2	The IRP is either completed here, or completed once the
//					interrupt driven read or writes completes, times out
//					or is cancelled.
//				3	Note that IRP may be cancelled at any time during this
//					processing, so we check IRP's Cancel flag when appropriate.
//				4	The Irp parameter is equal to uslddo->CurrentIrp until it
//					is completed and IoStartNextPacket called.
//
//	Runs at DISPATCH_LEVEL

VOID USLStartIo( IN PDEVICE_OBJECT uslddo, IN PIRP Irp)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)uslddo->DeviceExtension;
	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	PUCHAR Buffer = (PUCHAR)Irp->AssociatedIrp.SystemBuffer;

	// Zero the output count
	dx->CmdOutputCount = 0;
	dx->ConnectIntQueued = false;

	DebugPrint( "USLStartIo: %x %I",Irp,Irp);

	// Stop the 1 second timer if necessary
	if( dx->StopTimer)
	{
		IoStopTimer(uslddo);
		dx->StopTimer = false;
	}

	NTSTATUS status = STATUS_SUCCESS;

	// Switch on the IRP major function code
	switch( IrpStack->MajorFunction)
	{
	/////////////////////////////////////////////////////////////////////////
	case IRP_MJ_DEVICE_CONTROL:
	{
		ULONG ControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
		ULONG InputLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
		ULONG OutputLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;
		switch( ControlCode)
		{
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_USL_RUN_CMDS:
			DebugPrint( "USLStartIo: Run Cmds %s", dx->ConnectedToInterrupt?"(synchronised)":"");
			// If necessary make a temp buffer for the output data
			dx->RunCmdsOutBuffer = NULL;
			if( OutputLength>0)
			{
				dx->RunCmdsOutBuffer = (PUSHORT)ExAllocatePool(NonPagedPool,OutputLength);
				if( dx->RunCmdsOutBuffer==NULL)
				{
					status = STATUS_UNSUCCESSFUL;
					break;
				}
			}
			// Run the commands, synchronized with interrupt if necessary
			if( dx->ConnectedToInterrupt)
			{
				if( !KeSynchronizeExecution( dx->InterruptObject, (PKSYNCHRONIZE_ROUTINE)RunCmdsSynch, (PVOID)uslddo))
					status = STATUS_UNSUCCESSFUL;
			}
			else
			{
				if( !RunCmds(uslddo,true))
					status = STATUS_UNSUCCESSFUL;

				// Return straightaway if ConnectIntWQI queued
				if( dx->ConnectIntQueued) return;
			}

			// Copy temp output buffer back into shared IOCTL buffer
			if( dx->RunCmdsOutBuffer!=NULL)
			{
				RtlCopyMemory( Buffer, dx->RunCmdsOutBuffer, OutputLength);
				ExFreePool(dx->RunCmdsOutBuffer);
				dx->RunCmdsOutBuffer = NULL;
			}
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_USL_CMDS_FOR_READ:
			DebugPrintMsg( "USLStartIo: Store cmds for read");
			status = StoreCmds( &dx->ReadCmds, &dx->ReadCmdsLen, InputLength, Buffer);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_USL_CMDS_FOR_READ_START:
			DebugPrintMsg( "USLStartIo: Store cmds for read start");
			status = StoreCmds( &dx->StartReadCmds, &dx->StartReadCmdsLen, InputLength, Buffer);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_USL_CMDS_FOR_WRITE:
			DebugPrintMsg( "USLStartIo: Store cmds for write");
			status = StoreCmds( &dx->WriteCmds, &dx->WriteCmdsLen, InputLength, Buffer);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_USL_GET_RW_RESULTS:
#if DBG
			if( dx->TxCmdOutputCount>sizeof(dx->TxResult))
			{
				DebugPrint( "USLStartIo: Get RW Results: dx->TxCmdOutputCount too big at %d",dx->CmdOutputCount);
				dx->CmdOutputCount = sizeof(dx->TxResult);
			}
#endif
			// Copy cmd output first
			dx->CmdOutputCount = dx->TxCmdOutputCount;
			if( dx->CmdOutputCount>OutputLength)
				 dx->CmdOutputCount = OutputLength;
			RtlCopyMemory( Buffer, dx->TxResult, dx->CmdOutputCount);
			// Then add on last interrupt reg value
			if( dx->CmdOutputCount+1<=OutputLength)
				Buffer[dx->CmdOutputCount++] = dx->TxLastIntReg;

			DebugPrint( "USLStartIo: Get RW Results: %d bytes",dx->CmdOutputCount);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		default:
			status = STATUS_NOT_SUPPORTED;
		}
		break;
	}

	/////////////////////////////////////////////////////////////////////////
	case IRP_MJ_WRITE:
		if( dx->WriteCmds==NULL || !dx->ConnectedToInterrupt)
		{
			status = STATUS_INVALID_DEVICE_REQUEST;
			break;
		}

		// Store transfer details
		dx->TxTotal = IrpStack->Parameters.Write.Length;
		dx->TxLeft = dx->TxTotal;
		dx->TxBuffer = (PUCHAR)Buffer;
		dx->TxStatus = STATUS_SUCCESS;
		dx->TxIsWrite = true;
		RtlZeroMemory( dx->TxResult, sizeof(dx->TxResult));
		DebugPrint( "USLStartIo: Write %d bytes: %*s",dx->TxTotal,dx->TxTotal,dx->TxBuffer);

		// Start timeout timer
		dx->Timeout = dx->SetTimeout+1;
		IoStartTimer(uslddo);

		// Send first value
		if( KeSynchronizeExecution( dx->InterruptObject, (PKSYNCHRONIZE_ROUTINE)RunWriteCmdsSynch, (PVOID)dx))
		{
			status = STATUS_UNSUCCESSFUL;
			break;
		}
		return;

	/////////////////////////////////////////////////////////////////////////
	case IRP_MJ_READ:
		if( dx->ReadCmds==NULL || !dx->ConnectedToInterrupt)
		{
			status = STATUS_INVALID_DEVICE_REQUEST;
			break;
		}

		// Store transfer details
		dx->TxTotal = IrpStack->Parameters.Read.Length;
		dx->TxLeft = dx->TxTotal;
		dx->TxBuffer = (PUCHAR)Buffer;
		dx->TxStatus = STATUS_SUCCESS;
		dx->TxIsWrite = false;
		RtlZeroMemory( dx->TxResult, sizeof(dx->TxResult));
		DebugPrint( "USLStartIo: Read %d bytes: %*s",dx->TxTotal,dx->TxTotal,dx->TxBuffer);

		// Start timeout timer
		dx->Timeout = dx->SetTimeout;
		if( dx->Timeout<=0) dx->Timeout = 10;
		IoStartTimer(uslddo);

		// Run StartReadCmds if available
		if( dx->StartReadCmds!=NULL)
		{
			DebugPrintMsg( "USLStartIo: Read: Running start read commands");
			if( KeSynchronizeExecution( dx->InterruptObject, (PKSYNCHRONIZE_ROUTINE)RunStartReadCmdsSynch, (PVOID)dx))
			{
				status = STATUS_UNSUCCESSFUL;
				break;
			}
		}
		return;
	/////////////////////////////////////////////////////////////////////////
	default:
		status = STATUS_NOT_SUPPORTED;
		break;
	}

	/////////////////////////////////////////////////////////////////////////
	// Complete this IRP

	if( Irp->Cancel) status = STATUS_CANCELLED;

	// Remove cancel routine
	KIRQL OldIrql;
	IoAcquireCancelSpinLock( &OldIrql);
	IoSetCancelRoutine( Irp, NULL);
	IoReleaseCancelSpinLock(OldIrql);

	// Unlock device, complete IRP and start next
	UnlockDevice(dx);
	DebugPrint( "USLStartIo: CmdOutputCount %d", dx->CmdOutputCount);
	CompleteIrp(Irp, status, dx->CmdOutputCount);
	IoStartNextPacket( uslddo, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//	StoreCmds:	Copy commands from IOCTL input buffer to new buffer (in dx)

NTSTATUS StoreCmds( PUCHAR* pCmds, ULONG* pCmdsLen, ULONG len, PVOID Buffer)
{
	// Save commands for later processing
	if( len==0) return STATUS_INVALID_PARAMETER;
	FreeIfAllocated(*pCmds);
	*pCmds = (PUCHAR)ExAllocatePool( NonPagedPool, len);
	if( *pCmds==NULL)
		return STATUS_NO_MEMORY;
	RtlCopyMemory( *pCmds, Buffer, len);
	*pCmdsLen = len;
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////
//	RunCmds:		Run commands for IOCTL_USL_RUN_CMDS
//	RunCmdsSynch:	RunCmds called as a Critical Section routine at DIRQL
//
//	Only do trace output if not run as a Critical Section routine.
//
//	Runs at DISPATCH_LEVEL or DIRQL
//	Return	TRUE if commands ran (successfully or not)

BOOLEAN RunCmdsSynch( IN PDEVICE_OBJECT uslddo)
{
	return RunCmds( uslddo, true);
}

BOOLEAN RunCmds( IN PDEVICE_OBJECT uslddo, IN bool CanTrace)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)uslddo->DeviceExtension;
	PIRP Irp = uslddo->CurrentIrp;
	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	ULONG InputLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG OutputLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;
	PUCHAR Buffer = (PUCHAR)Irp->AssociatedIrp.SystemBuffer;

	ProcessCmds( dx, Buffer, InputLength, (PUCHAR)dx->RunCmdsOutBuffer, OutputLength, CanTrace);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	RunWriteCmdsSynch:	Run stored write commands
//						ProcessCmds output sent to dx->TxResult
//	Runs at DIRQL
//	Return TRUE if bytes all transferred (or in error)

BOOLEAN RunWriteCmdsSynch( IN PUSL_DEVICE_EXTENSION dx)
{
	if( dx->TxLeft==0) return TRUE;

	dx->CmdOutputCount = 0;
	BOOLEAN rv = ProcessCmds( dx, dx->WriteCmds, dx->WriteCmdsLen, dx->TxResult, sizeof(dx->TxResult), false);
	dx->TxCmdOutputCount = dx->CmdOutputCount;
	if( !rv)
	{
		dx->TxStatus = STATUS_UNSUCCESSFUL;
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	RunReadCmdsSynch:	Run stored read commands
//						ProcessCmds output sent to dx->TxResult
//	Runs at DIRQL
//	Return TRUE if bytes all transferred (or in error)

BOOLEAN RunReadCmdsSynch( IN PUSL_DEVICE_EXTENSION dx)
{
	if( dx->TxLeft==0) return TRUE;

	dx->CmdOutputCount = 0;
	BOOLEAN rv = ProcessCmds( dx, dx->ReadCmds, dx->ReadCmdsLen, dx->TxResult, sizeof(dx->TxResult), false);
	dx->TxCmdOutputCount = dx->CmdOutputCount;
	if( !rv)
	{
		dx->TxStatus = STATUS_UNSUCCESSFUL;
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	RunStartReadCmdsSynch:	Run stored start read commands
//							ProcessCmds output sent to dx->TxResult
//	Runs at DIRQL
//	Return TRUE if bytes all transferred (or in error)

BOOLEAN RunStartReadCmdsSynch( IN PUSL_DEVICE_EXTENSION dx)
{
	if( dx->TxLeft==0) return TRUE;

	dx->CmdOutputCount = 0;
	BOOLEAN rv = ProcessCmds( dx, dx->StartReadCmds, dx->StartReadCmdsLen, dx->TxResult, sizeof(dx->TxResult), false);
	dx->TxCmdOutputCount = dx->CmdOutputCount;
	if( !rv)
	{
		dx->TxStatus = STATUS_UNSUCCESSFUL;
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	Command names for DebugPrint

#if DODEBUGPRINT
static char* PHD_IO_CMDS[] =
{
	"USL_OR",
	"USL_AND",
	"USL_XOR",

	"USL_WRITE",
	"USL_READ",

	"USL_DELAY",

	"USL_WRITES",
	"USL_READS",

	"USL_IRQ_CONNECT",

	"USL_TIMEOUT",
	"USL_WRITE_NEXT",
	"USL_READ_NEXT",
};
static int NUM_PHD_IO_CMDS = sizeof(PHD_IO_CMDS)/sizeof(char*);
#endif

/////////////////////////////////////////////////////////////////////////////
//	Useful macros for ProcessCmds
//	GetUChar:			Get next UCHAR in newly declared variable (if available)
//	GetUCharNoDeclare:	Get next UCHAR in existing variable (if available)
//	SetUChar:			Store UCHAR in output buffer (if there's room)

#define GetUChar(var) if( ByteNo>=len) { FailCode=USL_NO_CMD_PARAMS; goto fail; } UCHAR var = *Buffer++; ByteNo++
#define GetUCharNoDeclare(var) if( ByteNo>=len) { FailCode=USL_NO_CMD_PARAMS; goto fail; }; var = *Buffer++; ByteNo++
#define SetUChar(b) if( OutByteNo>=outlen) { FailCode=USL_NO_OUTPUT_ROOM; goto fail; } *OutBuffer++ = (b); OutByteNo++; dx->CmdOutputCount++

/////////////////////////////////////////////////////////////////////////////
//	ProcessCmds:	Process commands in given buffer.
//					If output buffer given, first word has result code and
//					second word has index into input buffer of problem cmd.
//					Actual output values are stored after this.
//
//	Currently can only process UCHAR size commands.
//	Only produce DebugPrint output if CanTrace is true
//
//	return false if there's a problem

bool ProcessCmds(	IN PUSL_DEVICE_EXTENSION dx,
					IN PUCHAR Buffer, IN ULONG len,
					OUT PUCHAR OutBuffer, IN ULONG outlen,
					IN bool CanTrace
				  )
{
	if( CanTrace) { DebugPrint( "ProcessCmds. input:%d output:%d", len, outlen); }

	PIRP Irp = dx->uslddo->CurrentIrp;

	// Zero first 2 words of output buffer (if available)
	PUSHORT Result = (PUSHORT)OutBuffer;
	const int ResultLen = 2*sizeof(USHORT);
	if( outlen<ResultLen)		// 1.02..
	{
		Result = NULL;
		outlen = 0;
	}
	else
	{
		OutBuffer += ResultLen;
		outlen -= ResultLen;	// ..1.02
		*Result = USL_OK;
		*(Result+1) = 0;
		dx->CmdOutputCount += ResultLen;
	}
	USHORT FailCode = USL_OK;

	USHORT ByteNo=0;
	USHORT OutByteNo=0;
	// Loop through all commands
	while( ByteNo<len)
	{
		// See if we've been cancelled
		if( Irp->Cancel)
		{
			FailCode = USL_CANCELLED;
			goto fail;
		}
		// Get next command
		UCHAR Cmd = *Buffer++;
		ByteNo++;
		UCHAR Size = Cmd&0xC0;
		if( Size!=0)
		{
			FailCode = USL_BYTE_CMDS_ONLY;
			goto fail;	// Replace with following once uwords and ulongs supported
		}
/*
		bool isUCHAR = true;
		bool isUWORD = false;
		bool isULONG = false;
		if( Size==0x40) { isUCHAR = false; isUWORD = true; }
		if( Size==0x80) { isUCHAR = false; isULONG = true; }
		Cmd &= 0x3F;
*/
#if DODEBUGPRINT
		if( CanTrace)
			if( Cmd<NUM_PHD_IO_CMDS)
				DebugPrint( "Cmd: %s", PHD_IO_CMDS[Cmd]);
#endif
		/////////////////////////////////////////////////////////////////////
		switch( Cmd)
		{
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_OR:
		{
			GetUChar(reg);
			GetUChar(orvalue);
			UCHAR bvalue = ReadByte( dx, reg);
			UCHAR oredvalue = bvalue|orvalue;
			if( CanTrace) { DebugPrint( "Or %d %2x.  %2x->%2x", reg, orvalue, bvalue, oredvalue); }
			WriteByte( dx, reg, oredvalue);
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_AND:
		{
			GetUChar(reg);
			GetUChar(andvalue);
			UCHAR bvalue = ReadByte( dx, reg);
			UCHAR andedvalue = bvalue&andvalue;
			if( CanTrace) { DebugPrint( "And %d %2x.  %2x->%2x", reg, andvalue, bvalue, andedvalue); }
			WriteByte( dx, reg, andedvalue);
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_XOR:
		{
			GetUChar(reg);
			GetUChar(xorvalue);
			UCHAR bvalue = ReadByte( dx, reg);
			UCHAR xoredvalue = bvalue^xorvalue;
			if( CanTrace) { DebugPrint( "Xor %d %2x.  %2x->%2x", reg, xorvalue, bvalue, xoredvalue); }
			WriteByte( dx, reg, xoredvalue);
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_WRITE:
		{
			GetUChar(reg);
			GetUChar(bvalue);
			if( CanTrace) { DebugPrint( "Write %xh %2xh", reg, bvalue); }
			WriteByte( dx, reg, bvalue);
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_READ:
		{
			GetUChar(reg);
			UCHAR bvalue = ReadByte( dx, reg);
			SetUChar(bvalue);
			if( CanTrace) { DebugPrint( "Read %xh %2xh", reg, bvalue); }
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_DELAY:
		{
			GetUChar(delay);
			if( CanTrace) { DebugPrint( "Delay %dus", delay); }
			if( delay>60) { FailCode = USL_DELAY_TOO_LONG; goto fail; }
			KeStallExecutionProcessor(delay);
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_WRITES:
		{
			GetUChar(reg);
			GetUChar(count);
			GetUChar(delay);
			if( CanTrace) { DebugPrint( "Write %d values to %d, delay %dus", count, reg, delay); }
			if( delay>60) { FailCode = USL_DELAY_TOO_LONG; goto fail; }
			for( UCHAR vno=0; vno<count; vno++)
			{
				GetUChar(bvalue);
				if( CanTrace) { DebugPrint( "Writing %d %2x", reg, bvalue); }
				WriteByte( dx, reg, bvalue);
				KeStallExecutionProcessor(delay);
			}
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_READS:
		{
			GetUChar(reg);
			GetUChar(count);
			GetUChar(delay);
			if( CanTrace) { DebugPrint( "Read %d values from %d, delay %dus", count, reg, delay); }
			if( delay>60) { FailCode = USL_DELAY_TOO_LONG; goto fail; }
			for( UCHAR vno=0; vno<count; vno++)
			{
				UCHAR bvalue = ReadByte( dx, reg);
				KeStallExecutionProcessor(delay);
				SetUChar(bvalue);
				if( CanTrace) { DebugPrint( "Read %d %2x", reg, bvalue); }
			}
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_IRQ_CONNECT:
		{
			if( !dx->GotInterrupt) { FailCode = USL_NO_INTERRUPT; goto fail; }
			if( KeGetCurrentIrql()>DISPATCH_LEVEL) { FailCode = USL_ALREADY_CONNECTED_TO_INT; goto fail; }
			GetUCharNoDeclare(dx->InterruptReg);
			if( dx->InterruptReg>=dx->PortLength) { FailCode = USL_NOT_IN_RANGE; goto fail; }
			GetUCharNoDeclare(dx->InterruptRegMask);
			GetUCharNoDeclare(dx->InterruptRegValue);
			if( (dx->InterruptRegValue&dx->InterruptRegMask) != dx->InterruptRegValue)
				{ FailCode = USL_BAD_INTERRUPT_VALUE; goto fail; }

			if( CanTrace) { DebugPrint( "Connect.  Reg %d  Mask %2x Value %2x",
				dx->InterruptReg, dx->InterruptRegMask, dx->InterruptRegValue); }

			dx->ConnectIntQueued = true;
			ExQueueWorkItem( &dx->ConnectIntWQI, DelayedWorkQueue);

			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_TIMEOUT:
			GetUCharNoDeclare(dx->SetTimeout);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_WRITE_NEXT:
		{
			if( dx->Timeout==-1) { FailCode = USL_CANNOT_RW_NEXT; goto fail; }
			if( dx->TxLeft==0) { FailCode = USL_NO_DATA_LEFT_TO_TRANSFER; goto fail; }
			GetUChar(reg);
			WriteByte( dx, reg, *dx->TxBuffer++);
			dx->TxLeft--;
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case USL_READ_NEXT:
		{
			if( dx->Timeout==-1) { FailCode = USL_CANNOT_RW_NEXT; goto fail; }
			if( dx->TxLeft==0) { FailCode = USL_NO_DATA_LEFT_TO_TRANSFER; goto fail; }
			GetUChar(reg);
			*dx->TxBuffer++ = ReadByte( dx, reg);
			dx->TxLeft--;
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		default:
			FailCode = USL_UNRECOGNISED_CMD;
			goto fail;
		}
	}
	return true;

	/////////////////////////////////////////////////////////////////////////
	// Store failure code and location
fail:
	if( CanTrace) { DebugPrint( "ProcessCmds. FailCode %d at input:%d output:%d", FailCode, ByteNo-1, OutByteNo); }
	if( Result!=NULL)
	{
		*Result++ = FailCode;
		*Result = ByteNo-1;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//	IrqConnectRoutine:	Work queue item to connect to an interrupt at PASSIVE_LEVEL

VOID IrqConnectRoutine( IN PVOID Context)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)Context;

	DebugPrint( "IrqConnectRoutine");
	dx->ConnectIntQueued = false;

	// Get the current IRP that we are working on
	PIRP Irp = dx->uslddo->CurrentIrp;
	if( Irp==NULL) return;

	// Cancel if necessary
	NTSTATUS status;
	if( Irp->Cancel)
		status = STATUS_CANCELLED;
	else
	{
		// Try to connect to interrupt
		status = IoConnectInterrupt( &dx->InterruptObject, (PKSERVICE_ROUTINE)InterruptHandler,
							dx, NULL, dx->Vector, dx->Irql, dx->Irql, dx->Mode, FALSE, dx->Affinity, FALSE);

		if( NT_SUCCESS(status)) {
			dx->ConnectedToInterrupt = true;
		} 
		else
		{
			// Store FailCode in output 
			if( dx->RunCmdsOutBuffer!=NULL)
				*dx->RunCmdsOutBuffer = USL_CANNOT_CONNECT_TO_INTERRUPT;
		}
	}

	// Copy commands output to RUN_CMDS buffer
	if( dx->RunCmdsOutBuffer!=NULL)
	{
		PUCHAR Buffer = (PUCHAR)Irp->AssociatedIrp.SystemBuffer;
		PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
		ULONG OutputLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

		RtlCopyMemory( Buffer, dx->RunCmdsOutBuffer, OutputLength);
		ExFreePool(dx->RunCmdsOutBuffer);
		dx->RunCmdsOutBuffer = NULL;
	}

	// Remove cancel routine
	KIRQL OldIrql;
	IoAcquireCancelSpinLock( &OldIrql);
	IoSetCancelRoutine( Irp, NULL);
	IoReleaseCancelSpinLock(OldIrql);

	// Unlock device, complete IRP and start next
	UnlockDevice(dx);
	DebugPrint( "IrqConnectRoutine: CmdOutputCount %d", dx->CmdOutputCount);
	CompleteIrp(Irp, status, dx->CmdOutputCount);
	KeRaiseIrql( DISPATCH_LEVEL, &OldIrql);
	IoStartNextPacket( dx->uslddo, TRUE);
	KeLowerIrql(OldIrql);
}

/////////////////////////////////////////////////////////////////////////////
//	InterruptHandler:	Handle interrupts (during StartIo processing of R/W)
//					1	Always read the relevant status regsiter.
//					2	Only proceed if it has the right value,
//						ie to signal that our device caused the interrupt.
//					3	If IRP being cancelled then just call DPC to complete IRP.
//					4	Normally run write or read cmds to do whatever is necessary
//						to output or input next byte.
//					5	If all buffer txd (or error) then call DPC to complete IRP
//	Do not call DebugPrint here
//	Return TRUE if interrupt handled

BOOLEAN InterruptHandler(IN PKINTERRUPT Interrupt, IN PUSL_DEVICE_EXTENSION dx)
{

	unsigned char OldIrql;
	BOOL	flag=FALSE;

	KeRaiseIrql(31,&OldIrql);

	if(dx->pTimeSlotUser->Up15.bIsrFlag==FALSE) {	//cleared by user level
//		WriteByte(dx,3,1);	//Rearm the prf generator
//		WriteByte(dx,3,0);	
//		WriteByte(dx,2,0);	//Clear Trigger // Yes this is the correct order. there has to be a 10us trigger width
//		WriteByte(dx,2,0);	//Clear Trigger // Yes this is the correct order. there has to be a 10us trigger width
//		WriteByte(dx,2,0);	//Clear Trigger // Yes this is the correct order. there has to be a 10us trigger width
//		WriteByte(dx,2,0);	//Clear Trigger // Yes this is the correct order. there has to be a 10us trigger width
//		WriteByte(dx,2,0);	//Clear Trigger // Yes this is the correct order. there has to be a 10us trigger width
//		WriteByte(dx,2,0);	//Clear Trigger // Yes this is the correct order. there has to be a 10us trigger width
//		WriteByte(dx,2,0);	//Clear Trigger // Yes this is the correct order. there has to be a 10us trigger width
//		WriteByte(dx,2,0);	//Clear Trigger // Yes this is the correct order. there has to be a 10us trigger width
//		WriteByte(dx,2,1);	
	} else {
		dx->pTimeSlotUser->Up15.bIsrFlag=TRUE;
	}
	dx->pTimeSlotUser->Up15.nCount++;

//	if(dx->pTimeSlotUser->Dsp.nCardType==0) {
//		flag=DspNonMuxIsr(dx);
//	} else {
//		flag=DspMuxIsr(dx);
//	};


	KeLowerIrql(OldIrql);
	if(flag==FALSE) return FALSE;

	// See if interrupt is ours
	dx->TxLastIntReg = ReadByte( dx, dx->InterruptReg);
	if( (dx->TxLastIntReg&dx->InterruptRegMask) != dx->InterruptRegValue)
		return FALSE;

	// If no transfer in progress then no further processing required
	if( dx->Timeout==-1) return TRUE;

	// See if current IRP being cancelled
	PDEVICE_OBJECT uslddo = dx->uslddo;
	PIRP Irp = uslddo->CurrentIrp;
	if( Irp==NULL) return TRUE;
	BOOLEAN TxComplete = Irp->Cancel;
	if( !TxComplete)
	{
		// Run relevant set of commands
		if( dx->TxIsWrite)
			TxComplete = RunWriteCmdsSynch(dx);
		else
			TxComplete = RunReadCmdsSynch(dx);
	}
	// If all done, in error or being cancelled then call DPC to complete IRP
	if( TxComplete)
	{
		dx->Timeout = -1;
		IoRequestDpc( uslddo, Irp, dx);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	USLDpcForIsr:	Complete current IRP

VOID USLDpcForIsr(IN PKDPC Dpc, IN PDEVICE_OBJECT uslddo, 
					IN PIRP Irp, IN PUSL_DEVICE_EXTENSION dx)
{
	dx->Timeout = -1;
	ULONG BytesTxd = dx->TxTotal-dx->TxLeft;
	if( Irp->Cancel) dx->TxStatus = STATUS_CANCELLED;

	DebugPrint("USLDpcForIsr: Status %x Info %d", dx->TxStatus, BytesTxd);

	// Remove cancel routine
	KIRQL OldIrql;
	IoAcquireCancelSpinLock( &OldIrql);
	IoSetCancelRoutine( Irp, NULL);
	IoReleaseCancelSpinLock(OldIrql);

	// Unlock device and complete IRP
	UnlockDevice(dx);
	CompleteIrp(Irp, dx->TxStatus, BytesTxd);
	IoStartNextPacket( uslddo, TRUE);

	// Stop timer calls
	dx->StopTimer = true;
}

/////////////////////////////////////////////////////////////////////////////
//	Timeout1sSynch:	Timeout check 
//	Return TRUE if operation has timed out
//	Called as a Critical Section routine

static BOOLEAN Timeout1sSynch( IN PUSL_DEVICE_EXTENSION dx)
{
	if( dx->Timeout==-1 || --dx->Timeout>0)
		return FALSE;
	dx->Timeout = -1;
	dx->TxStatus = STATUS_NO_MEDIA_IN_DEVICE;	// Win32: ERROR_NOT_READY
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	Timeout1s:	One second timer call
//				Call Timeout1sSynch and then DPC if time out

VOID Timeout1s( IN PDEVICE_OBJECT uslddo, IN PUSL_DEVICE_EXTENSION dx)
{
	if( dx->Timeout==-1) return;

	DebugPrint("Timeout1s: Timeout is %d",dx->Timeout);
	PIRP Irp = uslddo->CurrentIrp;
#if DBG
	if( Irp==NULL) return;
#endif
	if( Irp->Cancel || KeSynchronizeExecution( dx->InterruptObject, (PKSYNCHRONIZE_ROUTINE)Timeout1sSynch, dx))
		USLDpcForIsr( NULL, uslddo, uslddo->CurrentIrp, dx);
}

/////////////////////////////////////////////////////////////////////////////
//	USLCancelIrp:
//
//	Description:
//		Cancel this IRP.
//			Called to cancel a Irp.
//			Called when CancelIo called or process finished without closing handle.
//			IRP must have set this as its cancel routine.
//
//		1	If IRP currently being processed in StartIo or interrupt handler
//			then just quit without completing IRP.  The IRP Cancel flag will 
//			be detected in due course and the IRP completed (as cancelled) then.
//		2	If IRP still in StartIo queue then remove it and complete it as cancelled.

VOID USLCancelIrp( IN PDEVICE_OBJECT uslddo, IN PIRP Irp)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)uslddo->DeviceExtension;
	DebugPrint("USLCancelIrp: Cancelling %x %I", Irp, Irp);
	if( Irp==uslddo->CurrentIrp)
	{
		DebugPrintMsg("USLCancelIrp: IRP running in StartIo");
		// IRP is being processed by USLStartIo.
		// Irp->Cancel flag already set.
		// USLStartIo or timeout will detect Cancel flag and cancel IRP in due course
		IoReleaseCancelSpinLock(Irp->CancelIrql);
	}
	else
	{
		DebugPrintMsg("USLCancelIrp: IRP in StartIo queue");
		// IRP is still in StartIo device queue.
		// Just dequeue and cancel it.  No need to start next IRP.
		BOOLEAN dequeued = KeRemoveEntryDeviceQueue(
								&uslddo->DeviceQueue,
								&Irp->Tail.Overlay.DeviceQueueEntry);

		IoReleaseCancelSpinLock(Irp->CancelIrql);

		if( dequeued)
		{
			UnlockDevice(dx);
			CompleteIrp( Irp, STATUS_CANCELLED);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CancelCurrentIrpSynch:	If a transfer is in progress, mark it for cancelling
//	Return TRUE if operation can been cancelled
//	Runs at DIRQL

static BOOLEAN CancelCurrentIrpSynch( IN PUSL_DEVICE_EXTENSION dx)
{
	if( dx->Timeout==-1)
		return FALSE;
	dx->Timeout = -1;
	dx->TxStatus = STATUS_CANCELLED;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//	USLCleanup:
//
//	Description:
//		Handle IRP_MJ_CLEANUP requests
//		Cancel queued IRPs which match given FileObject
//
//		USL cancels *all* queued IRPs and the current Irp
//
//	Arguments:
//		Pointer to our PHD DO
//		Pointer to the IRP
//			IrpStack->FileObject has handle to file
//
//	Return Value:
//		This function returns STATUS_XXX

NTSTATUS USLDispatchCleanup( IN PDEVICE_OBJECT uslddo, IN PIRP Irp)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)uslddo->DeviceExtension;
	DebugPrintMsg("USLDispatchCleanup");
	KIRQL OldIrql;
	IoAcquireCancelSpinLock(&OldIrql);

	// Cancel all IRPs in the I/O Manager maintained queue in device object
	PKDEVICE_QUEUE_ENTRY QueueEntry;
	while( (QueueEntry=KeRemoveDeviceQueue(&uslddo->DeviceQueue)) != NULL)
	{
		PIRP CancelIrp = CONTAINING_RECORD( QueueEntry, IRP, Tail.Overlay.DeviceQueueEntry);
		CancelIrp->Cancel = TRUE;
		CancelIrp->CancelIrql = OldIrql;
		CancelIrp->CancelRoutine = NULL;

		IoReleaseCancelSpinLock(OldIrql);
		DebugPrint("USLDispatchCleanup: Cancelling %x %I",CancelIrp,CancelIrp);
		UnlockDevice(dx);
		CompleteIrp( CancelIrp, STATUS_CANCELLED);
		IoAcquireCancelSpinLock(&OldIrql);
	}
	IoReleaseCancelSpinLock(OldIrql);

	// Forcibly cancel any in-progress IRP
	if( dx->Timeout!=-1)
	{
		if( KeSynchronizeExecution( dx->InterruptObject, (PKSYNCHRONIZE_ROUTINE)CancelCurrentIrpSynch, dx))
		{
			if( uslddo->CurrentIrp!=NULL)
			{
				DebugPrint("USLDispatchCleanup: Cancelled in-progress IRP %x %I",uslddo->CurrentIrp,uslddo->CurrentIrp);
				USLDpcForIsr( NULL, uslddo, uslddo->CurrentIrp, dx);
			}
		}
	}

	return CompleteIrp( Irp, STATUS_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////////

BOOLEAN DspNonMuxIsr( IN PUSL_DEVICE_EXTENSION dx)
{
	BOOLEAN	flag=FALSE;
	int	nAddrs,ii,nSlot;
	unsigned char	nVv[30];
	nSlot=0;

	dx->nIrqCount++;
	if(dx->pTimeSlotUser!=NULL) {
		dx->pTimeSlotUser->nIrqCount++;

		nAddrs=dx->pTimeSlotUser->Dsp.nAddrs;
		for(ii=0;(ii<22)&&(ReadByte(dx,nAddrs+2) & 0x02);ii++) {
			nVv[ii]=ReadByte(dx,nAddrs);
		};
		WriteByte(dx,nAddrs+1,1);//Clear Interrupt

		while(ReadByte(dx,nAddrs+2) & 0x02) {
			ReadByte(dx,nAddrs); // CLEAR OUT FIFO
			dx->pTimeSlotUser->Dsp.ErrorCount++;
		};

		if(dx->pTimeSlotUser->Dsp.bPlayBack==TRUE) {
			goto function_finish;
		}
		if((nVv[0]==0x80) && (nVv[21]==0xff))  {
			nSlot=0;
			for(ii=0;ii<22;ii++) dx->pTimeSlotUser[nSlot].Dsp.Vv[ii]=nVv[ii];
		} else {
			dx->pTimeSlotUser->Dsp.ErrorCount++;
			goto function_finish;
		};

		for(ii=0;ii<8;ii++) {
			dx->pTimeSlotUser[nSlot].Gate.nAmplitude[ii]=nVv[ii+1];
		};

		dx->pTimeSlotUser[nSlot].Dsp.InterfacePos=dx->pTimeSlotUser[nSlot].Dsp.Vv[12]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[11]&0x1f)<<7);
		if(dx->pTimeSlotUser[nSlot].Dsp.Vv[11]&0x40) {
			dx->pTimeSlotUser[nSlot].Dsp.bInterfaceStatus=TRUE;
		} else {
			dx->pTimeSlotUser[nSlot].Dsp.InterfacePos=4095;
			dx->pTimeSlotUser[nSlot].Dsp.bInterfaceStatus=FALSE;
		};
		dx->pTimeSlotUser[nSlot].Dsp.TimsStartPos[0]=dx->pTimeSlotUser[nSlot].Dsp.Vv[14]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[13]&0x1f)<<7);
		dx->pTimeSlotUser[nSlot].Dsp.TimsStopPos[0]=dx->pTimeSlotUser[nSlot].Dsp.Vv[16]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[15]&0x1f)<<7);

		if((dx->pSi6User!=NULL)  && (dx->pSi6User->bCollectData==TRUE)) {
			for(ii=0;ii<8;ii++) {
				dx->pSi6User->Amp[ii]=dx->pTimeSlotUser->Dsp.Vv[1+ii];
			};
		};
		nVv[13]&0x40 ? dx->pTimeSlotUser[nSlot].Dsp.nPhase = NEGATIVE_PHASE : dx->pTimeSlotUser[nSlot].Dsp.nPhase = POSITIVE_PHASE;

/*		if((dx->pServoUser!=NULL)  && (dx->bCollectData==TRUE)) {
			for(ii=0;ii<8;ii++) {
				dx->pServoUser->Amp[nSlot][ii]=dx->pTimeSlotUser[nSlot].Dsp.Vv[2+ii];
			};
			dx->pServoUser->Time[nSlot][0]=dx->pTimeSlotUser[nSlot].Dsp.InterfacePos;
			dx->pServoUser->Time[nSlot][1]=dx->pTimeSlotUser[nSlot].Dsp.TimsStopPos[0]-dx->pTimeSlotUser[nSlot].Dsp.TimsStartPos[0];
		};
*/
	};

function_finish:

	CollectBscanData(dx);

	return flag;

}


BOOLEAN DspMuxIsr( IN PUSL_DEVICE_EXTENSION dx)
{
	BOOLEAN	flag=FALSE;
	int	nAddrs,ii,nSlot;
	unsigned char	nVv[30];


	dx->nIrqCount++;
	if(dx->pTimeSlotUser!=NULL) {

//	WriteByte(dx,0x17,0x80);
//	WriteByte(dx,0x15,0x00);
		dx->pTimeSlotUser->nIrqCount++;

		nAddrs=dx->pTimeSlotUser->Dsp.nAddrs;
		if((ReadByte(dx,nAddrs+3) & 1)==0) return FALSE;	//Card not present

		for(ii=0;(ii<25)&&(ReadByte(dx,nAddrs+2) & 0x02);ii++) {
			nVv[ii]=ReadByte(dx,nAddrs);
		};
		ReadByte(dx,nAddrs+1);//Clear Interrupt this is output in non mux version

		while(ReadByte(dx,nAddrs+2) & 0x02) {
			ReadByte(dx,nAddrs); // CLEAR OUT FIFO
			dx->pTimeSlotUser->Dsp.ErrorCount++;
		};
//	WriteByte(dx,0x17,0x80);

		if(dx->pTimeSlotUser->Dsp.bPlayBack==TRUE) {
			goto function_finish;
		}
		if((nVv[0]==0x80) && (nVv[24]==0xff) && (nVv[1]>=0) && (nVv[1]<8))  {
			nSlot=nVv[1];
			for(ii=0;ii<25;ii++) dx->pTimeSlotUser[nSlot].Dsp.Vv[ii]=nVv[ii];
		} else {
			dx->pTimeSlotUser->Dsp.ErrorCount++;
			goto function_finish;
		};
		dx->pTimeSlotUser[nSlot].Dsp.Vv[29]=ReadByte(dx,dx->pTimeSlotUser->Adc.nAddrs+2); //ADC Timeslot
		dx->pTimeSlotUser[nSlot].Dsp.nIrqCount++;

		for(ii=0;ii<8;ii++) {
			dx->pTimeSlotUser[nSlot].Gate.nAmplitude[ii]=nVv[ii+2];
		};

		dx->pTimeSlotUser[nSlot].Dsp.InterfacePos=dx->pTimeSlotUser[nSlot].Dsp.Vv[13]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[12]&0x1f)<<7);
		if(dx->pTimeSlotUser[nSlot].Dsp.Vv[12]&0x40) {
			dx->pTimeSlotUser[nSlot].Dsp.bInterfaceStatus=TRUE;
		} else {
			dx->pTimeSlotUser[nSlot].Dsp.InterfacePos=4095;
			dx->pTimeSlotUser[nSlot].Dsp.bInterfaceStatus=FALSE;
		};
		if(dx->pTimeSlotUser[nSlot].Dsp.Vv[14]& 0x20) {
			dx->pTimeSlotUser[nSlot].Dsp.TimsStartPos[0]=dx->pTimeSlotUser[nSlot].Dsp.Vv[15]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[14]&0x1f)<<7);
		}
		if(dx->pTimeSlotUser[nSlot].Dsp.Vv[16]& 0x20) {
			dx->pTimeSlotUser[nSlot].Dsp.TimsStopPos[0]=dx->pTimeSlotUser[nSlot].Dsp.Vv[17]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[16]&0x1f)<<7);
		} else {
			dx->pTimeSlotUser[nSlot].Dsp.TimsStopPos[0]=dx->pTimeSlotUser[nSlot].Dsp.TimsStartPos[0];
		}


		if((dx->pSi6User!=NULL)  && (dx->pSi6User->bCollectData==TRUE)) {
			for(ii=0;ii<8;ii++) {
				dx->pSi6User->Amp[ii]=dx->pTimeSlotUser->Dsp.Vv[2+ii];
			};
		};

		if((dx->pServoUser!=NULL)  && (dx->bCollectData==TRUE)) {
			for(ii=0;ii<8;ii++) {
				dx->pServoUser->Amp[nSlot][ii]=dx->pTimeSlotUser[nSlot].Dsp.Vv[2+ii];
			};
			dx->pServoUser->Time[nSlot][0]=dx->pTimeSlotUser[nSlot].Dsp.InterfacePos;
			dx->pServoUser->Time[nSlot][1]=dx->pTimeSlotUser[nSlot].Dsp.TimsStopPos[0]-dx->pTimeSlotUser[nSlot].Dsp.TimsStartPos[0];
		};

	};

function_finish:

	CollectBscanData(dx);


	return flag;

}

/*

BOOLEAN DspNonMuxIsr( IN PUSL_DEVICE_EXTENSION dx)
{
	BOOLEAN	flag=FALSE;
	int	nAddrs,ii;

	dx->nIrqCount++;
	if(dx->pTimeSlotUser!=NULL) {
		dx->pTimeSlotUser->nIrqCount++;

		nAddrs=dx->pTimeSlotUser->Dsp.nAddrs;
		for(ii=0;(ii<22)&&(ReadByte(dx,nAddrs+2) & 0x02);ii++) {
			dx->pTimeSlotUser->Dsp.Vv[ii]=ReadByte(dx,nAddrs);
		};
		while(ReadByte(dx,nAddrs+2) & 0x02) {
			ReadByte(dx,nAddrs); // CLEAR OUT FIFO
			dx->pTimeSlotUser->Dsp.ErrorCount++;
		};
		WriteByte(dx,nAddrs+1,0);

		dx->pTimeSlotUser->Dsp.InterfacePos=dx->pTimeSlotUser->Dsp.Vv[12]|((dx->pTimeSlotUser->Dsp.Vv[11]&0x1f)<<7);
		if(dx->pTimeSlotUser->Dsp.Vv[11]&0x40) {
			dx->pTimeSlotUser->Dsp.bInterfaceStatus=TRUE;
		} else {
			dx->pTimeSlotUser->Dsp.InterfacePos=4095;
			dx->pTimeSlotUser->Dsp.bInterfaceStatus=FALSE;
		};
		dx->pTimeSlotUser->Dsp.TimsStartPos[0]=dx->pTimeSlotUser->Dsp.Vv[14]|((dx->pTimeSlotUser->Dsp.Vv[13]&0x1f)<<7);
		dx->pTimeSlotUser->Dsp.TimsStopPos[0]=dx->pTimeSlotUser->Dsp.Vv[16]|((dx->pTimeSlotUser->Dsp.Vv[15]&0x1f)<<7);

	};
	if((dx->pSi6User!=NULL)  && (dx->bCollectData==TRUE)) {
		for(ii=0;ii<8;ii++) {
			dx->pSi6User->Amp[ii]=dx->pTimeSlotUser->Dsp.Vv[1+ii];
		};
	};


	return flag;

}


BOOLEAN DspMuxIsr( IN PUSL_DEVICE_EXTENSION dx)
{
	BOOLEAN	flag=FALSE;
	int	nAddrs,ii,nSlot;
	unsigned char	nVv[30];

	dx->nIrqCount++;
	if(dx->pTimeSlotUser!=NULL) {
		dx->pTimeSlotUser->nIrqCount++;

		nAddrs=dx->pTimeSlotUser->Dsp.nAddrs;
		for(ii=0;(ii<25)&&(ReadByte(dx,nAddrs+2) & 0x02);ii++) {
			nVv[ii]=ReadByte(dx,nAddrs);
		};
		while(ReadByte(dx,nAddrs+2) & 0x02) {
			ReadByte(dx,nAddrs); // CLEAR OUT FIFO
			dx->pTimeSlotUser->Dsp.ErrorCount++;
		};
		ReadByte(dx,nAddrs+1);//Clear Interrupt this is output in non mux version

		if((nVv[0]==0x80) && (nVv[24]==0xff) && (nVv[1]>=0) && (nVv[1]<8))  {
			nSlot=nVv[1];
			for(ii=0;ii<25;ii++) dx->pTimeSlotUser[nSlot].Dsp.Vv[ii]=nVv[ii];
		} else {
			dx->pTimeSlotUser->Dsp.ErrorCount++;
			goto function_finish;
		};	

		dx->pTimeSlotUser[nSlot].Dsp.InterfacePos=dx->pTimeSlotUser[nSlot].Dsp.Vv[13]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[12]&0x1f)<<7);
		if(dx->pTimeSlotUser[nSlot].Dsp.Vv[12]&0x40) {
			dx->pTimeSlotUser[nSlot].Dsp.bInterfaceStatus=TRUE;
		} else {
			dx->pTimeSlotUser[nSlot].Dsp.InterfacePos=4095;
			dx->pTimeSlotUser[nSlot].Dsp.bInterfaceStatus=FALSE;
		};
		dx->pTimeSlotUser[nSlot].Dsp.TimsStartPos[0]=dx->pTimeSlotUser[nSlot].Dsp.Vv[15]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[14]&0x1f)<<7);
		dx->pTimeSlotUser[nSlot].Dsp.TimsStopPos[0]=dx->pTimeSlotUser[nSlot].Dsp.Vv[17]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[16]&0x1f)<<7);

		if((dx->pServoUser!=NULL)  && (dx->bCollectData==TRUE)) {
			for(ii=0;ii<8;ii++) {
				dx->pServoUser->Amp[nSlot][ii]=dx->pTimeSlotUser[nSlot].Dsp.Vv[2+ii];
			};
			dx->pServoUser->Time[nSlot][0]=dx->pTimeSlotUser[nSlot].Dsp.InterfacePos;
			dx->pServoUser->Time[nSlot][1]=dx->pTimeSlotUser[nSlot].Dsp.TimsStopPos[0]-dx->pTimeSlotUser[nSlot].Dsp.TimsStartPos[0];
		};
	};

function_finish:

	return flag;

}


  */

BOOLEAN CollectBscanData( IN PUSL_DEVICE_EXTENSION dx)
{
	int	nOffset,timeout=0;
	static int nOldNumber=-1;
	int	nTrigPos=0;
	int	nTemp,ii;

	if(dx->pTimeSlotUser!=NULL) {
		if(dx->pTimeSlotUser[0].Adc.bBscanCollectData!=TRUE) {
			return FALSE;
		}
		if(dx->pBscanArray==NULL) return FALSE;
		struct ADC200Data* pAdc = &dx->pTimeSlotUser[0].Adc;

		if(nOldNumber-pAdc->nBscanNumber) {

			switch(pAdc->nCardType) {
			case USL_ADC200:
				nOffset=pAdc->nBscanNumber * pAdc->nBscanLength;
				
				pAdc->control_reg&=0xf3;		//Request ADC access
				pAdc->control_reg|=0x02;
				WriteByte(dx,pAdc->nAddrs+1,(char)pAdc->control_reg);

				while(timeout<1000) {
					if((ReadByte(dx,pAdc->nAddrs+1)  & 0x01) == 0 ) {
						timeout++;
					} else {
						switch(pAdc->nBscanMode) {
						case 0:
							break;
						case 1:	
							RtlCopyMemory(&(dx->pBscanArray[nOffset]),&nTrigPos,2);
							RtlCopyMemory(&(dx->pBscanArray[nOffset+2]),dx->MemoryBase,pAdc->nBscanLength-2);
							break;
						case 2:
							nTrigPos=dx->pTimeSlotUser[0].Dsp.InterfacePos;
							int	nStart=pAdc->nBscanPreTrigger/pAdc->nSamplePeriod;
							int	nFinish=pAdc->nBscanPostTrigger/pAdc->nSamplePeriod;
							nStart=nTrigPos-nStart;
							nFinish+=nTrigPos;
							if(nStart<0) nStart=0;
							if(nStart>=(int)pAdc->nAcqLength) nStart=(int)pAdc->nAcqLength-1;
							if(nFinish<nStart) nFinish=nStart;
							if(nFinish>=(int)pAdc->nAcqLength) nFinish=(int)pAdc->nAcqLength-1;
							RtlCopyMemory(&(dx->pBscanArray[nOffset]),&nStart,2);
							RtlCopyMemory(&(dx->pBscanArray[nOffset+2]),dx->MemoryBase+nStart,nFinish-nStart);
							break;
						}
						timeout=1000;
					};
				}

				pAdc->control_reg&=0xf5;	//Clear ADC access
				pAdc->control_reg|=0x04;
				WriteByte(dx,pAdc->nAddrs+1,(char)pAdc->control_reg);
				
				break;
			case USL_ADC100:
				nTemp = pAdc->control_reg;
				nTemp &= 0xef;	/* set bit 4 low */
				nTemp|=0x20;
				WriteByte(dx,pAdc->nAddrs+2,(char)nTemp);
				for(ii=0;ii<10000;ii++) {
					if((ReadByte(dx,pAdc->nAddrs) & 0x10) == 0){	/* wait for adcacgrant*/
						ii=30000;
					};
				};
				if(ii<30000) {
					goto relinquish_Adc;
				};
				nOffset=pAdc->nBscanNumber * pAdc->nBscanLength;
				RtlCopyMemory(Trace,dx->MemoryBase,1024);
				switch(pAdc->nBscanMode) {
				case 0:
					break;
				case 1:	
					RtlCopyMemory(&(dx->pBscanArray[nOffset]),&nTrigPos,2);
					RtlCopyMemory(&(dx->pBscanArray[nOffset+2]),Trace,pAdc->nBscanLength-2);
					break;
				case 2:
					nTrigPos=dx->pTimeSlotUser[0].Dsp.InterfacePos;
					int	nStart=pAdc->nBscanPreTrigger/pAdc->nSamplePeriod;
					int	nFinish=pAdc->nBscanPostTrigger/pAdc->nSamplePeriod;
					nStart=nTrigPos-nStart;
					nFinish+=nTrigPos;
					if(nStart<0) nStart=0;
					if(nStart>=(int)pAdc->nAcqLength) nStart=(int)pAdc->nAcqLength-1;
					if(nFinish<nStart) nFinish=nStart;
					if(nFinish>=(int)pAdc->nAcqLength) nFinish=(int)pAdc->nAcqLength-1;
					RtlCopyMemory(&(dx->pBscanArray[nOffset]),&nStart,2);
					RtlCopyMemory(&(dx->pBscanArray[nOffset+2]),Trace+nStart,nFinish-nStart);
					break;
				}
relinquish_Adc:
				pAdc->control_reg |= 0x10;		/* relinquish adc */
				WriteByte(dx,pAdc->nAddrs+2,(char)pAdc->control_reg); 
				break;
			}
			nOldNumber=dx->pTimeSlotUser[0].Adc.nBscanNumber;
			return TRUE;
		}
	}

	return FALSE;

}

BOOLEAN ProcessTrace( IN PUSL_DEVICE_EXTENSION dx)
{
	DSP200Data* pDsp = &dx->pTimeSlotUser->Dsp;

	int nInterfacePos = -1;
	int	ii,jj,gg;
	char cThreshold;
	char cMax[8]={0,0,0,0,0,0,0,0};
	int	nDelay,nFinish;

	WriteByte(dx,0x17,0x80);
	WriteByte(dx,0x15,0);

//	RtlCopyMemory(Trace,ImageTrace,8192);
/*	
	for(ii=0;ii<8192;ii++) {
		Trace[ii]=ImageTrace[ii];
	}
*/
	cThreshold=(char)pDsp->InterfaceThreshold[0];
	if(pDsp->SignDetect[0]&1) cThreshold*=-1;
	for(ii=pDsp->Delay[0],jj=0;jj<pDsp->Width[0];ii++,jj++) {
		if(nInterfacePos==-1) {	//Find interface pos
			if(!pDsp->SignDetect[0]) {
				if(Trace[ii]>=cThreshold) nInterfacePos = ii;
			} else {
				if(Trace[ii]<=cThreshold) nInterfacePos = ii;
			}
		}
		if(!pDsp->SignDetect) {
			if(Trace[ii]>=cMax[0]) cMax[0] = Trace[ii];;
		} else {
			if(Trace[ii]<=cMax[0]) cMax[0] = Trace[ii];
		}
	}

	
	for(gg=1;gg<8;gg++) {
		switch(pDsp->TriggerMode[gg]) {
		case 0:
			break;
		case 1:
			nDelay=pDsp->Delay[gg];
			if(nDelay<0) nDelay=0;
			if(nDelay>4095) nDelay = 4095;
			break;
		case 2:
			nDelay=pDsp->Delay[gg] + nInterfacePos;
			if(nDelay<0) nDelay=0;
			if(nDelay>4095) nDelay = 4095;
			break;
		}
		if(pDsp->TriggerMode[gg]>0) {
			nFinish = pDsp->Width[gg] + nDelay;
			if(nFinish>4095) nFinish = 4095;
			if(!pDsp->SignDetect[gg]) {
				for(ii=nDelay;ii<=nFinish;ii++) {
					if(Trace[ii]>=cMax[gg]) cMax[gg] = Trace[ii];
				}
			} else {
				for(ii=nDelay;ii<=nFinish;ii++) {
					if(Trace[ii]<=cMax[gg]) cMax[gg] = Trace[ii];
				}
			}
		}
	}
	
	WriteByte(dx,0x17,0x80);


	return TRUE;
}
