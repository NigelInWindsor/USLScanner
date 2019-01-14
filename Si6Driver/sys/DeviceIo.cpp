//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
//	SI6 example
/////////////////////////////////////////////////////////////////////////////
//	DeviceIo.cpp:	Routines that interact with a device
/////////////////////////////////////////////////////////////////////////////
//	StopDevice			Stop device
//*	WriteByte			Output a byte
//*	ReadByte			Input a byte
//	SI6StartIo		Process an IRP from the head of the device IRP queue.
//*	StoreCmds			Copy commands from IOCTL input buffer to new buffer (in dx)
//*	RunCmds				Run commands for IOCTL_SI6_RUN_CMDS
//*	RunCmdsSynch		RunCmds called as a Critical Section routine
//*	RunWriteCmdsSynch	Run stored write commands
//*	RunReadCmdsSynch	Run stored read commands
//*	RunStartReadCmdsSynch	Run stored start read commands
//*	ProcessCmds			Process commands in given buffer.
//*	IrqConnectRoutine	Work queue item to connect to an interrupt at PASSIVE_LEVEL
//*	InterruptHandler	Handle interrupts (during StartIo processing of R/W)
//	SI6DpcForIsr		Complete current IRP
//*	Timeout1sSynch		Timeout check 
//	Timeout1s			One second timer call
//	SI6CancelIrp		Cancel this IRP
//*	CancelCurrentIrpSynch	If a transfer is in progress, mark it for cancelling
//	SI6Cleanup		Handle IRP_MJ_CLEANUP requests
/////////////////////////////////////////////////////////////////////////////

#include "SI6Driver.h"
#include "Si6Ioctl.h"
#include "C:\WindowsProjects\USLScanner\USLDriver\sys\USLIoctl.h"
#include "windef.h"
#include <math.h>
#include <float.h>

/////////////////////////////////////////////////////////////////////////////

NTSTATUS StoreCmds( PUCHAR* pCmds, ULONG* pCmdsLen, ULONG len, PVOID Buffer);
void WriteByte( IN PSI6_DEVICE_EXTENSION dx, IN ULONG offset, IN UCHAR byte);
UCHAR ReadByte( IN PSI6_DEVICE_EXTENSION dx, IN ULONG offset);
WORD ReadWord( IN PSI6_DEVICE_EXTENSION dx, IN ULONG offset);
BOOLEAN InterruptHandler(IN PKINTERRUPT Interrupt, IN PSI6_DEVICE_EXTENSION dx);

BOOLEAN RunCmdsSynch( IN PDEVICE_OBJECT si6ddo);
BOOLEAN RunCmds( IN PDEVICE_OBJECT si6ddo, IN bool CanTrace);
bool ProcessCmds(	IN PSI6_DEVICE_EXTENSION dx,
					IN PUCHAR Buffer, IN ULONG len,
					OUT PUCHAR OutBuffer, IN ULONG outlen,
					IN bool CanTrace
				  );

BOOLEAN RunWriteCmdsSynch( IN PSI6_DEVICE_EXTENSION dx);
BOOLEAN RunReadCmdsSynch( IN PSI6_DEVICE_EXTENSION dx);
BOOLEAN RunStartReadCmdsSynch( IN PSI6_DEVICE_EXTENSION dx);

//Si6 Procedures

extern	int		nTable[10001];

BOOLEAN DspNonMuxIsr( IN PSI6_DEVICE_EXTENSION dx);
BOOLEAN DspMuxIsr( IN PSI6_DEVICE_EXTENSION dx);
BOOLEAN Si6Isr( IN PSI6_DEVICE_EXTENSION dx);
void Si6Joystick(IN PSI6_DEVICE_EXTENSION dx,int nWhich);
void Si6Moveto(IN PSI6_DEVICE_EXTENSION dx,int nWhich);
void Si6Hometo(IN PSI6_DEVICE_EXTENSION dx,int nWhich);
void Si6PulseMotorUpdateEncoder(IN PSI6_DEVICE_EXTENSION dx,struct AxisData *pAxis);
void Si6CounterStart(IN PSI6_DEVICE_EXTENSION dx,int Which,int Value);
//int	abs(int nn);
int MulDiv(int v0,int v1,int v2);
void CollectGateData(IN PSI6_DEVICE_EXTENSION dx,struct AxisData *pAxis);
int  CheckLimitSwitches(IN PSI6_DEVICE_EXTENSION dx);

BOOLEAN	FloatingPointTest( IN PSI6_DEVICE_EXTENSION dx,double dAngle, double *dResult);

/////////////////////////////////////////////////////////////////////////////
//	StopDevice:	Stop device

VOID StopDevice( IN PSI6_DEVICE_EXTENSION dx)
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

void WriteByte( IN PSI6_DEVICE_EXTENSION dx, IN ULONG offset, IN UCHAR byte)
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

UCHAR ReadByte( IN PSI6_DEVICE_EXTENSION dx, IN ULONG offset)
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

WORD ReadWord( IN PSI6_DEVICE_EXTENSION dx, IN ULONG offset)
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
//	SI6StartIo:	Process an IRP from the head of the device IRP queue.
//				1	Only IOCTL, Read and Write IRPs get here.
//				2	The IRP is either completed here, or completed once the
//					interrupt driven read or writes completes, times out
//					or is cancelled.
//				3	Note that IRP may be cancelled at any time during this
//					processing, so we check IRP's Cancel flag when appropriate.
//				4	The Irp parameter is equal to si6ddo->CurrentIrp until it
//					is completed and IoStartNextPacket called.
//
//	Runs at DISPATCH_LEVEL

VOID SI6StartIo( IN PDEVICE_OBJECT si6ddo, IN PIRP Irp)
{
	PSI6_DEVICE_EXTENSION dx = (PSI6_DEVICE_EXTENSION)si6ddo->DeviceExtension;
	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	PUCHAR Buffer = (PUCHAR)Irp->AssociatedIrp.SystemBuffer;

	// Zero the output count
	dx->CmdOutputCount = 0;
	dx->ConnectIntQueued = false;

	DebugPrint( "SI6StartIo: %x %I",Irp,Irp);

	// Stop the 1 second timer if necessary
	if( dx->StopTimer)
	{
		IoStopTimer(si6ddo);
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
		case IOCTL_SI6_RUN_CMDS:
			DebugPrint( "SI6StartIo: Run Cmds %s", dx->ConnectedToInterrupt?"(synchronised)":"");
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
				if( !KeSynchronizeExecution( dx->InterruptObject, (PKSYNCHRONIZE_ROUTINE)RunCmdsSynch, (PVOID)si6ddo))
					status = STATUS_UNSUCCESSFUL;
			}
			else
			{
				if( !RunCmds(si6ddo,true))
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
		case IOCTL_SI6_CMDS_FOR_READ:
			DebugPrintMsg( "SI6StartIo: Store cmds for read");
			status = StoreCmds( &dx->ReadCmds, &dx->ReadCmdsLen, InputLength, Buffer);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_SI6_CMDS_FOR_READ_START:
			DebugPrintMsg( "SI6StartIo: Store cmds for read start");
			status = StoreCmds( &dx->StartReadCmds, &dx->StartReadCmdsLen, InputLength, Buffer);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_SI6_CMDS_FOR_WRITE:
			DebugPrintMsg( "SI6StartIo: Store cmds for write");
			status = StoreCmds( &dx->WriteCmds, &dx->WriteCmdsLen, InputLength, Buffer);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_SI6_GET_RW_RESULTS:
#if DBG
			if( dx->TxCmdOutputCount>sizeof(dx->TxResult))
			{
				DebugPrint( "SI6StartIo: Get RW Results: dx->TxCmdOutputCount too big at %d",dx->CmdOutputCount);
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

			DebugPrint( "SI6StartIo: Get RW Results: %d bytes",dx->CmdOutputCount);
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
		DebugPrint( "SI6StartIo: Write %d bytes: %*s",dx->TxTotal,dx->TxTotal,dx->TxBuffer);

		// Start timeout timer
		dx->Timeout = dx->SetTimeout+1;
		IoStartTimer(si6ddo);

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
		DebugPrint( "SI6StartIo: Read %d bytes: %*s",dx->TxTotal,dx->TxTotal,dx->TxBuffer);

		// Start timeout timer
		dx->Timeout = dx->SetTimeout;
		if( dx->Timeout<=0) dx->Timeout = 10;
		IoStartTimer(si6ddo);

		// Run StartReadCmds if available
		if( dx->StartReadCmds!=NULL)
		{
			DebugPrintMsg( "SI6StartIo: Read: Running start read commands");
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
	DebugPrint( "SI6StartIo: CmdOutputCount %d", dx->CmdOutputCount);
	CompleteIrp(Irp, status, dx->CmdOutputCount);
	IoStartNextPacket( si6ddo, TRUE);
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
//	RunCmds:		Run commands for IOCTL_SI6_RUN_CMDS
//	RunCmdsSynch:	RunCmds called as a Critical Section routine at DIRQL
//
//	Only do trace output if not run as a Critical Section routine.
//
//	Runs at DISPATCH_LEVEL or DIRQL
//	Return	TRUE if commands ran (successfully or not)

BOOLEAN RunCmdsSynch( IN PDEVICE_OBJECT si6ddo)
{
	return RunCmds( si6ddo, true);
}

BOOLEAN RunCmds( IN PDEVICE_OBJECT si6ddo, IN bool CanTrace)
{
	PSI6_DEVICE_EXTENSION dx = (PSI6_DEVICE_EXTENSION)si6ddo->DeviceExtension;
	PIRP Irp = si6ddo->CurrentIrp;
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

BOOLEAN RunWriteCmdsSynch( IN PSI6_DEVICE_EXTENSION dx)
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

BOOLEAN RunReadCmdsSynch( IN PSI6_DEVICE_EXTENSION dx)
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

BOOLEAN RunStartReadCmdsSynch( IN PSI6_DEVICE_EXTENSION dx)
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
	"SI6_OR",
	"SI6_AND",
	"SI6_XOR",

	"SI6_WRITE",
	"SI6_READ",

	"SI6_DELAY",

	"SI6_WRITES",
	"SI6_READS",

	"SI6_IRQ_CONNECT",

	"SI6_TIMEOUT",
	"SI6_WRITE_NEXT",
	"SI6_READ_NEXT",
};
static int NUM_PHD_IO_CMDS = sizeof(PHD_IO_CMDS)/sizeof(char*);
#endif

/////////////////////////////////////////////////////////////////////////////
//	Useful macros for ProcessCmds
//	GetUChar:			Get next UCHAR in newly declared variable (if available)
//	GetUCharNoDeclare:	Get next UCHAR in existing variable (if available)
//	SetUChar:			Store UCHAR in output buffer (if there's room)

#define GetUChar(var) if( ByteNo>=len) { FailCode=SI6_NO_CMD_PARAMS; goto fail; } UCHAR var = *Buffer++; ByteNo++
#define GetUCharNoDeclare(var) if( ByteNo>=len) { FailCode=SI6_NO_CMD_PARAMS; goto fail; }; var = *Buffer++; ByteNo++
#define SetUChar(b) if( OutByteNo>=outlen) { FailCode=SI6_NO_OUTPUT_ROOM; goto fail; } *OutBuffer++ = (b); OutByteNo++; dx->CmdOutputCount++

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

bool ProcessCmds(	IN PSI6_DEVICE_EXTENSION dx,
					IN PUCHAR Buffer, IN ULONG len,
					OUT PUCHAR OutBuffer, IN ULONG outlen,
					IN bool CanTrace
				  )
{
	if( CanTrace) { DebugPrint( "ProcessCmds. input:%d output:%d", len, outlen); }

	PIRP Irp = dx->si6ddo->CurrentIrp;

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
		*Result = SI6_OK;
		*(Result+1) = 0;
		dx->CmdOutputCount += ResultLen;
	}
	USHORT FailCode = SI6_OK;

	USHORT ByteNo=0;
	USHORT OutByteNo=0;
	// Loop through all commands
	while( ByteNo<len)
	{
		// See if we've been cancelled
		if( Irp->Cancel)
		{
			FailCode = SI6_CANCELLED;
			goto fail;
		}
		// Get next command
		UCHAR Cmd = *Buffer++;
		ByteNo++;
		UCHAR Size = Cmd&0xC0;
		if( Size!=0)
		{
			FailCode = SI6_BYTE_CMDS_ONLY;
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
		case SI6_OR:
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
		case SI6_AND:
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
		case SI6_XOR:
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
		case SI6_WRITE:
		{
			GetUChar(reg);
			GetUChar(bvalue);
			if( CanTrace) { DebugPrint( "Write %xh %2xh", reg, bvalue); }
			WriteByte( dx, reg, bvalue);
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SI6_READ:
		{
			GetUChar(reg);
			UCHAR bvalue = ReadByte( dx, reg);
			SetUChar(bvalue);
			if( CanTrace) { DebugPrint( "Read %xh %2xh", reg, bvalue); }
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SI6_DELAY:
		{
			GetUChar(delay);
			if( CanTrace) { DebugPrint( "Delay %dus", delay); }
			if( delay>60) { FailCode = SI6_DELAY_TOO_LONG; goto fail; }
			KeStallExecutionProcessor(delay);
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SI6_WRITES:
		{
			GetUChar(reg);
			GetUChar(count);
			GetUChar(delay);
			if( CanTrace) { DebugPrint( "Write %d values to %d, delay %dus", count, reg, delay); }
			if( delay>60) { FailCode = SI6_DELAY_TOO_LONG; goto fail; }
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
		case SI6_READS:
		{
			GetUChar(reg);
			GetUChar(count);
			GetUChar(delay);
			if( CanTrace) { DebugPrint( "Read %d values from %d, delay %dus", count, reg, delay); }
			if( delay>60) { FailCode = SI6_DELAY_TOO_LONG; goto fail; }
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
		case SI6_IRQ_CONNECT:
		{
			if( !dx->GotInterrupt) { FailCode = SI6_NO_INTERRUPT; goto fail; }
			if( KeGetCurrentIrql()>DISPATCH_LEVEL) { FailCode = SI6_ALREADY_CONNECTED_TO_INT; goto fail; }
			GetUCharNoDeclare(dx->InterruptReg);
			if( dx->InterruptReg>=dx->PortLength) { FailCode = SI6_NOT_IN_RANGE; goto fail; }
			GetUCharNoDeclare(dx->InterruptRegMask);
			GetUCharNoDeclare(dx->InterruptRegValue);
			if( (dx->InterruptRegValue&dx->InterruptRegMask) != dx->InterruptRegValue)
				{ FailCode = SI6_BAD_INTERRUPT_VALUE; goto fail; }

			if( CanTrace) { DebugPrint( "Connect.  Reg %d  Mask %2x Value %2x",
				dx->InterruptReg, dx->InterruptRegMask, dx->InterruptRegValue); }

			dx->ConnectIntQueued = true;
			ExQueueWorkItem( &dx->ConnectIntWQI, DelayedWorkQueue);

			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SI6_TIMEOUT:
			GetUCharNoDeclare(dx->SetTimeout);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SI6_WRITE_NEXT:
		{
			if( dx->Timeout==-1) { FailCode = SI6_CANNOT_RW_NEXT; goto fail; }
			if( dx->TxLeft==0) { FailCode = SI6_NO_DATA_LEFT_TO_TRANSFER; goto fail; }
			GetUChar(reg);
			WriteByte( dx, reg, *dx->TxBuffer++);
			dx->TxLeft--;
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SI6_READ_NEXT:
		{
			if( dx->Timeout==-1) { FailCode = SI6_CANNOT_RW_NEXT; goto fail; }
			if( dx->TxLeft==0) { FailCode = SI6_NO_DATA_LEFT_TO_TRANSFER; goto fail; }
			GetUChar(reg);
			*dx->TxBuffer++ = ReadByte( dx, reg);
			dx->TxLeft--;
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		default:
			FailCode = SI6_UNRECOGNISED_CMD;
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
	PSI6_DEVICE_EXTENSION dx = (PSI6_DEVICE_EXTENSION)Context;

	DebugPrint( "IrqConnectRoutine");
	dx->ConnectIntQueued = false;

	// Get the current IRP that we are working on
	PIRP Irp = dx->si6ddo->CurrentIrp;
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
				*dx->RunCmdsOutBuffer = SI6_CANNOT_CONNECT_TO_INTERRUPT;
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
	IoStartNextPacket( dx->si6ddo, TRUE);
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

BOOLEAN InterruptHandler(IN PKINTERRUPT Interrupt, IN PSI6_DEVICE_EXTENSION dx)
{

	unsigned char OldIrql;

	KeRaiseIrql(31,&OldIrql);

	BOOLEAN flag=Si6Isr(dx);

	KeLowerIrql(OldIrql);
	if(flag==FALSE) return FALSE;

	// See if interrupt is ours
	dx->TxLastIntReg = ReadByte( dx, dx->InterruptReg);
	if( (dx->TxLastIntReg&dx->InterruptRegMask) != dx->InterruptRegValue)
		return FALSE;

	// If no transfer in progress then no further processing required
	if( dx->Timeout==-1) return TRUE;

	// See if current IRP being cancelled
	PDEVICE_OBJECT si6ddo = dx->si6ddo;
	PIRP Irp = si6ddo->CurrentIrp;
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
		IoRequestDpc( si6ddo, Irp, dx);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	SI6DpcForIsr:	Complete current IRP

VOID SI6DpcForIsr(IN PKDPC Dpc, IN PDEVICE_OBJECT si6ddo, 
					IN PIRP Irp, IN PSI6_DEVICE_EXTENSION dx)
{
	dx->Timeout = -1;
	ULONG BytesTxd = dx->TxTotal-dx->TxLeft;
	if( Irp->Cancel) dx->TxStatus = STATUS_CANCELLED;

	DebugPrint("SI6DpcForIsr: Status %x Info %d", dx->TxStatus, BytesTxd);

	// Remove cancel routine
	KIRQL OldIrql;
	IoAcquireCancelSpinLock( &OldIrql);
	IoSetCancelRoutine( Irp, NULL);
	IoReleaseCancelSpinLock(OldIrql);

	// Unlock device and complete IRP
	UnlockDevice(dx);
	CompleteIrp(Irp, dx->TxStatus, BytesTxd);
	IoStartNextPacket( si6ddo, TRUE);

	// Stop timer calls
	dx->StopTimer = true;
}

/////////////////////////////////////////////////////////////////////////////
//	Timeout1sSynch:	Timeout check 
//	Return TRUE if operation has timed out
//	Called as a Critical Section routine

static BOOLEAN Timeout1sSynch( IN PSI6_DEVICE_EXTENSION dx)
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

VOID Timeout1s( IN PDEVICE_OBJECT si6ddo, IN PSI6_DEVICE_EXTENSION dx)
{
	if( dx->Timeout==-1) return;

	DebugPrint("Timeout1s: Timeout is %d",dx->Timeout);
	PIRP Irp = si6ddo->CurrentIrp;
#if DBG
	if( Irp==NULL) return;
#endif
	if( Irp->Cancel || KeSynchronizeExecution( dx->InterruptObject, (PKSYNCHRONIZE_ROUTINE)Timeout1sSynch, dx))
		SI6DpcForIsr( NULL, si6ddo, si6ddo->CurrentIrp, dx);
}

/////////////////////////////////////////////////////////////////////////////
//	SI6CancelIrp:
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

VOID SI6CancelIrp( IN PDEVICE_OBJECT si6ddo, IN PIRP Irp)
{
	PSI6_DEVICE_EXTENSION dx = (PSI6_DEVICE_EXTENSION)si6ddo->DeviceExtension;
	DebugPrint("SI6CancelIrp: Cancelling %x %I", Irp, Irp);
	if( Irp==si6ddo->CurrentIrp)
	{
		DebugPrintMsg("SI6CancelIrp: IRP running in StartIo");
		// IRP is being processed by SI6StartIo.
		// Irp->Cancel flag already set.
		// SI6StartIo or timeout will detect Cancel flag and cancel IRP in due course
		IoReleaseCancelSpinLock(Irp->CancelIrql);
	}
	else
	{
		DebugPrintMsg("SI6CancelIrp: IRP in StartIo queue");
		// IRP is still in StartIo device queue.
		// Just dequeue and cancel it.  No need to start next IRP.
		BOOLEAN dequeued = KeRemoveEntryDeviceQueue(
								&si6ddo->DeviceQueue,
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

static BOOLEAN CancelCurrentIrpSynch( IN PSI6_DEVICE_EXTENSION dx)
{
	if( dx->Timeout==-1)
		return FALSE;
	dx->Timeout = -1;
	dx->TxStatus = STATUS_CANCELLED;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//	SI6Cleanup:
//
//	Description:
//		Handle IRP_MJ_CLEANUP requests
//		Cancel queued IRPs which match given FileObject
//
//		SI6 cancels *all* queued IRPs and the current Irp
//
//	Arguments:
//		Pointer to our PHD DO
//		Pointer to the IRP
//			IrpStack->FileObject has handle to file
//
//	Return Value:
//		This function returns STATUS_XXX

NTSTATUS SI6DispatchCleanup( IN PDEVICE_OBJECT si6ddo, IN PIRP Irp)
{
	PSI6_DEVICE_EXTENSION dx = (PSI6_DEVICE_EXTENSION)si6ddo->DeviceExtension;
	DebugPrintMsg("SI6DispatchCleanup");
	KIRQL OldIrql;
	IoAcquireCancelSpinLock(&OldIrql);

	// Cancel all IRPs in the I/O Manager maintained queue in device object
	PKDEVICE_QUEUE_ENTRY QueueEntry;
	while( (QueueEntry=KeRemoveDeviceQueue(&si6ddo->DeviceQueue)) != NULL)
	{
		PIRP CancelIrp = CONTAINING_RECORD( QueueEntry, IRP, Tail.Overlay.DeviceQueueEntry);
		CancelIrp->Cancel = TRUE;
		CancelIrp->CancelIrql = OldIrql;
		CancelIrp->CancelRoutine = NULL;

		IoReleaseCancelSpinLock(OldIrql);
		DebugPrint("SI6DispatchCleanup: Cancelling %x %I",CancelIrp,CancelIrp);
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
			if( si6ddo->CurrentIrp!=NULL)
			{
				DebugPrint("SI6DispatchCleanup: Cancelled in-progress IRP %x %I",si6ddo->CurrentIrp,si6ddo->CurrentIrp);
				SI6DpcForIsr( NULL, si6ddo, si6ddo->CurrentIrp, dx);
			}
		}
	}

	return CompleteIrp( Irp, STATUS_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////////
BOOLEAN Si6Isr( IN PSI6_DEVICE_EXTENSION dx)
{
	int	nIrq;

	BOOLEAN	flag=FALSE;


	dx->pSi6User->nIrqCount++;

	WriteByte(dx,dx->pSi6User->nAddrs+SI6_PIC+0,PIC_NOP);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_PIC+0,0x0f);
	
	nIrq=ReadByte(dx,dx->pSi6User->nAddrs+SI6_PIC+0)&0x07;

	if((nIrq)<6) {
		switch(dx->pSi6User->nIrqAction[nIrq]) {
		case MOVETO: Si6Moveto(dx,nIrq);
			break;
		case HOMETO: Si6Hometo(dx,nIrq);
			break;
		case JOYSTICK: Si6Joystick(dx,nIrq);
			break;
		};
		flag=FALSE;
	};

	return flag;

}
#define DEADMAN_SWITCH		4
#define FIXTURE_SELECTED	1

void Si6Joystick(IN PSI6_DEVICE_EXTENSION dx,int nWhich)
{
	struct	AxisData *pAxis = dx->pAxis[nWhich];
	int	nSideTemp,nTemp,nMinTemp,nMaxTemp,nLimitSwitch;
	#define	MARGIN	8


	if((pAxis->nJoystickMaxValue-pAxis->nJoystickZero)==0) pAxis->nJoystickMaxValue=pAxis->nJoystickZero+35;
	if((pAxis->nJoystickMinValue-pAxis->nJoystickZero)==0) pAxis->nJoystickMinValue=pAxis->nJoystickZero-35;
	if(dx->pSi6User->nJoystickPercent<=0) dx->pSi6User->nJoystickPercent=100;

	dx->pSi6User->nJoystickPercent=100;

	(pAxis->bDirectionReverse)?
	(dx->pSi6User->nDirEor|=pAxis->nBit):(dx->pSi6User->nDirEor&=(~pAxis->nBit));
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_DIR_PORT,dx->pSi6User->nDirReg ^ dx->pSi6User->nDirEor);

	Si6PulseMotorUpdateEncoder(dx,pAxis);

	nMinTemp=MulDiv(pAxis->nJoystickMinSpeed,dx->pSi6User->nJoystickPercent,100);
	nMaxTemp=MulDiv(pAxis->nJoystickMaxSpeed,dx->pSi6User->nJoystickPercent,100);

	switch(dx->pSi6User->nJoystickSource) {
	case 0:		//Si6 ADC ports
		nTemp=ReadByte(dx,dx->pSi6User->nAddrs+SI6_JOYSTICK+pAxis->nJoystickNumber);
		pAxis->nJoystickValueRead=nTemp;
		nSideTemp=ReadByte(dx,dx->pSi6User->nAddrs+SI6_JOYSTICK+3);
		break;
	case 1:		//Comms Joystick
		pAxis->nJoystickValueRead = dx->pSi6User->nCommsJoystickValue[pAxis->nJoystickNumber%4];
		nSideTemp = dx->pSi6User->nCommsJoystickValue[4];
		break;
	};

//	if(dx->pSi6User->nCommsSwitchValue & DEADMAN_SWITCH) {
//		pAxis->nJoystickZero = pAxis->nJoystickValueRead;
//	}
	if((dx->pSi6User->nCommsSwitchValue & FIXTURE_SELECTED) == 0) {
		pAxis->nJoystickZero = pAxis->nJoystickValueRead;
		goto finish_interrupt;
	}


	nTemp=pAxis->nJoystickValueRead-pAxis->nJoystickZero;
	if(nTemp>=0) {
		nTemp=MulDiv(100,nTemp,pAxis->nJoystickMaxValue-pAxis->nJoystickZero);
	} else {
		nTemp=MulDiv(100,nTemp,pAxis->nJoystickZero-pAxis->nJoystickMinValue);
	}

	switch(dx->pSi6User->nJobNumber) {
	default:
		switch(pAxis->nJoystickSwitchPos) {
		case 0: if(nSideTemp>30) nTemp=0;
			break;
		case 1: if((nSideTemp<30) || (nSideTemp>200)) nTemp=0;
			break;
		case 2: if(nSideTemp<200) nTemp=0;
			break;
		}
		break;
	case 484:
		break;
	case 485:	//VTT HELSINKY
		nSideTemp=0;
		break;
	}


	if(pAxis->bJoystickAvailable==0) nTemp=0;
	if(pAxis->bJoystickReverse) nTemp *= -1;

	if(abs(nTemp) > 150) {
		pAxis->nCurrentSpeed=nTemp;
		dx->pSi6User->nProcessState &= (~pAxis->nBit);
		goto finish_interrupt;
	};

	if((abs(nTemp)<MARGIN)&&(pAxis->nCurrentSpeed<=nMinTemp)) {
		pAxis->nCurrentSpeed = nMinTemp;
		dx->pSi6User->nProcessState &= (~pAxis->nBit);
		goto finish_interrupt;
	};
	if((nTemp>0) && (pAxis->nCurrentSpeed <= nMinTemp)) {
		dx->pSi6User->nDirReg |= pAxis->nBit;
	};
	if((nTemp<0) && (pAxis->nCurrentSpeed <= nMinTemp)) {
		dx->pSi6User->nDirReg &= (~pAxis->nBit);
	};

	if(((nTemp>0) && !(dx->pSi6User->nDirReg & pAxis->nBit)) || ((nTemp<0) && (dx->pSi6User->nDirReg & pAxis->nBit))) {
		pAxis->nReqSpeed = nMinTemp;
	} else {
		nTemp = abs(nTemp)-MARGIN;
		pAxis->nReqSpeed = MulDiv(nMaxTemp-nMinTemp,nTemp,100)+nMinTemp;
		if(pAxis->nReqSpeed>nMaxTemp) 
			pAxis->nReqSpeed = nMaxTemp;
	};
	pAxis->nReqSpeed=MulDiv(pAxis->nReqSpeed,dx->pSi6User->nJoystickPercent,100);
	if(pAxis->nCurrentSpeed < pAxis->nReqSpeed) {
		pAxis->nCurrentSpeed += pAxis->nAcceleration;
		if(pAxis->nCurrentSpeed > pAxis->nReqSpeed) pAxis->nCurrentSpeed = pAxis->nReqSpeed;
	};
	if(pAxis->nCurrentSpeed > pAxis->nReqSpeed) {
		pAxis->nCurrentSpeed -= pAxis->nDeceleration;
		if(pAxis->nCurrentSpeed < pAxis->nReqSpeed) pAxis->nCurrentSpeed = pAxis->nReqSpeed;
	};
	dx->pSi6User->nProcessState |= pAxis->nBit;

	//CHECK LIMIT SWITCHES
	nLimitSwitch=CheckLimitSwitches(dx);
	if((nLimitSwitch&pAxis->nMaxSwitch)&&!(dx->pSi6User->nDirReg&pAxis->nBit)){
		dx->pSi6User->nProcessState &= (~pAxis->nBit);
	};
	if((nLimitSwitch&pAxis->nMinSwitch)&&(dx->pSi6User->nDirReg&pAxis->nBit)){
		dx->pSi6User->nProcessState &= (~pAxis->nBit);
	};

	//CHECK FOR OVER TRAVEL
	if((pAxis->nMinTravel!=pAxis->nMaxTravel)&&(dx->pSi6User->nProcessState&pAxis->nBit)) {
		if((pAxis->nPos>=pAxis->nMaxTravel)&&!(dx->pSi6User->nDirReg&pAxis->nBit))
															dx->pSi6User->nProcessState &=(~pAxis->nBit);
		if((pAxis->nPos<=pAxis->nMinTravel)&&(dx->pSi6User->nDirReg&pAxis->nBit)) 
														dx->pSi6User->nProcessState &=(~pAxis->nBit);
		(dx->pSi6User->nDirReg&pAxis->nBit)?(nTemp=pAxis->nPos-pAxis->nMinTravel):
										(nTemp=pAxis->nMaxTravel-pAxis->nPos);
		if(pAxis->nDeceleration==0) pAxis->nDeceleration=1;
	
		if(nTemp>5000l) nTemp=5000l;
		if(nTemp<((pAxis->nCurrentSpeed-pAxis->nMinSpeed)/pAxis->nDeceleration)) {
			nTemp = (nTemp*pAxis->nDeceleration)+pAxis->nMinSpeed;
			if(nTemp<pAxis->nCurrentSpeed) pAxis->nCurrentSpeed = nTemp;
			if(pAxis->nCurrentSpeed<nMinTemp) pAxis->nCurrentSpeed=nMinTemp;
			if(pAxis->nCurrentSpeed>nMaxTemp) pAxis->nCurrentSpeed=nMaxTemp;
		};
	};
	
/*
if((pAxis->stall_err & pAxis->nBit) && 
	((abs((int)(pAxis->count-pAxis->nPos))) > pAxis->stall_err)) {
	dx->pSi6User->nProcessState &= (~pAxis->nBit);
	stalled |= pAxis->nBit;
};
*/
finish_interrupt:
if(pAxis->nCurrentSpeed<0) pAxis->nCurrentSpeed=0;
if(pAxis->nCurrentSpeed>10000) pAxis->nCurrentSpeed=9999;
Si6CounterStart(dx,pAxis->nCounter,nTable[pAxis->nCurrentSpeed]);

}



void Si6Moveto(IN PSI6_DEVICE_EXTENSION dx,int nWhich)
{
	struct	AxisData *pAxis = dx->pAxis[nWhich];
	int	nTemp,nAcceleration;
	static int nLoopCounter=1;

	(pAxis->bDirectionReverse)?(dx->pSi6User->nDirEor|=pAxis->nBit):(dx->pSi6User->nDirEor&=(~pAxis->nBit));
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_DIR_PORT,dx->pSi6User->nDirReg ^ dx->pSi6User->nDirEor);


	Si6PulseMotorUpdateEncoder(dx,pAxis);


	nTemp = pAxis->nPos - pAxis->nDesirePos;

	if(abs(nTemp)<=pAxis->nPositionError){
		dx->pSi6User->nProcessState &= (~pAxis->nBit);
		goto finish_interrupt;
	};

	dx->pSi6User->nProcessState |= pAxis->nBit;

	if((nTemp > 0) && !(dx->pSi6User->nDirReg & pAxis->nBit)) {
		pAxis->nCurrentSpeed = pAxis->nMinSpeed;
		dx->pSi6User->nDirReg |= pAxis->nBit;
		goto direction_set;
	};
	if((nTemp < 0) && (dx->pSi6User->nDirReg & pAxis->nBit)) {
		pAxis->nCurrentSpeed = pAxis->nMinSpeed;
		dx->pSi6User->nDirReg &= (~pAxis->nBit);
	};
	direction_set:

	nTemp = abs(nTemp);
	if(pAxis->nDeceleration==0) pAxis->nDeceleration=1;

	if(nTemp > ((pAxis->nCurrentSpeed-pAxis->nMinSpeed)/pAxis->nDeceleration)) {
		nLoopCounter--;
		nAcceleration=0;
		if((pAxis->nCurrentSpeed>5000) && (nLoopCounter<=0)) { 
			nAcceleration = 1;
			nLoopCounter=(pAxis->nCurrentSpeed-5000)/50;
		} else {
			nAcceleration = pAxis->nAcceleration;
			nLoopCounter=1;
		}
		pAxis->nCurrentSpeed += nAcceleration;
		if(pAxis->nCurrentSpeed > pAxis->nMaxSpeed) {
			pAxis->nCurrentSpeed = pAxis->nMaxSpeed;
		};
	} else {
		pAxis->nCurrentSpeed -= pAxis->nDeceleration;
		if(pAxis->nCurrentSpeed < pAxis->nMinSpeed) {
			pAxis->nCurrentSpeed = pAxis->nMinSpeed;
		};
	};

	finish_interrupt:

	if((nWhich==dx->pSi6User->nWhichAxis) && (dx->pSi6User->bCollectData==TRUE)) {
		CollectGateData(dx,pAxis);
	};
	
	Si6CounterStart(dx,pAxis->nCounter,nTable[pAxis->nCurrentSpeed]);
}



void Si6Hometo(IN PSI6_DEVICE_EXTENSION dx,int nWhich)
{
	struct	AxisData *pAxis = dx->pAxis[nWhich];
	int	nLimitSwitch;

	(pAxis->bDirectionReverse)?(dx->pSi6User->nDirEor|=pAxis->nBit):(dx->pSi6User->nDirEor&=(~pAxis->nBit));
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_DIR_PORT,dx->pSi6User->nDirReg ^ dx->pSi6User->nDirEor);

	Si6PulseMotorUpdateEncoder(dx,pAxis);

	nLimitSwitch=CheckLimitSwitches(dx);

	switch(pAxis->nHomeMode) {
	case 0:
		if(nLimitSwitch&pAxis->nMinSwitch) {
			pAxis->bHomeFlag=TRUE;
			pAxis->nHomeFoundAt=pAxis->nPos;
			dx->pSi6User->nProcessState &= (~pAxis->nBit);
			goto finish_interrupt;
		};
		break;
	case 1:
		if(nLimitSwitch&pAxis->nMaxSwitch) {
			pAxis->bHomeFlag=TRUE;
			pAxis->nHomeFoundAt=pAxis->nPos;
			dx->pSi6User->nProcessState &= (~pAxis->nBit);
			goto finish_interrupt;
		};
		break;
	case 2:
		if(nLimitSwitch&pAxis->nHomeSwitch) {
			pAxis->bHomeFlag=TRUE;
			pAxis->nHomeFoundAt=pAxis->nPos;
			dx->pSi6User->nProcessState &= (~pAxis->nBit);
			goto finish_interrupt;
		};
		break;
	case 3:
		if(pAxis->bHomeFlag==FALSE) {
			if(abs(pAxis->nCount)>=((abs(pAxis->nHomePos)*3)/2)) {
				pAxis->bHomeFlag=TRUE;
				pAxis->nHomeFoundAt=pAxis->nPos;
				pAxis->nCurrentSpeed = pAxis->nMinSpeed;
				dx->pSi6User->nProcessState &= (~pAxis->nBit);
				goto finish_interrupt;
			};
		};
		break;
	case 4:
		if(pAxis->bHomeFlag==FALSE) {
			if(abs(pAxis->nCount)>=((abs(pAxis->nStepsPerRev)*3)/2)) {
				pAxis->bHomeFlag=TRUE;
				pAxis->nHomeFoundAt=pAxis->nPos;
				pAxis->nCurrentSpeed = pAxis->nMinSpeed;
				dx->pSi6User->nProcessState &= (~pAxis->nBit);
				goto finish_interrupt;
			};
		};
		break;
	};



	if(pAxis->nCurrentSpeed < abs(pAxis->nHomeSpeed)) {
		pAxis->nCurrentSpeed += pAxis->nAcceleration;
	};

	finish_interrupt:
	Si6CounterStart(dx,pAxis->nCounter,nTable[pAxis->nCurrentSpeed]);

}


void Si6CounterStart(IN PSI6_DEVICE_EXTENSION dx,int number,int value)
{

switch(number) {
case 0:
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_0,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_0,value>>8);
	break;
case 1:
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_1,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_1,value>>8);
	break;
case 2:
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_2,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_2,value>>8);
	break;
case 3:
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_3,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_3,value>>8);
	break;
case 4:
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_4,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_4,value>>8);
	break;
case 5:
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_5,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_5,value>>8);
	break;
};
/*
switch(number) {
case 0:	WriteByte(dx,dx->pSi6User->nAddrs+SI6_CNTRL_012,0x34);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_0,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_0,value>>8);
	break;
case 1:	WriteByte(dx,dx->pSi6User->nAddrs+SI6_CNTRL_012,0x74);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_1,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_1,value>>8);
	break;
case 2:	WriteByte(dx,dx->pSi6User->nAddrs+SI6_CNTRL_012,0xb4);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_2,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_2,value>>8);
	break;
case 3:	WriteByte(dx,dx->pSi6User->nAddrs+SI6_CNTRL_345,0x34);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_3,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_3,value>>8);
	break;
case 4:	WriteByte(dx,dx->pSi6User->nAddrs+SI6_CNTRL_345,0x74);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_4,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_4,value>>8);
	break;
case 5:	WriteByte(dx,dx->pSi6User->nAddrs+SI6_CNTRL_345,0xb4);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_5,value&0xff);
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_COUNT_5,value>>8);
	break;
};
*/
}

void Si6PulseMotorUpdateEncoder(IN PSI6_DEVICE_EXTENSION dx,struct AxisData *pAxis)
{
static int nLastSwitchState,nLastRevPos;
int	nLimitSwitch,nPos;


nLimitSwitch=CheckLimitSwitches(dx);

if(dx->pSi6User->nProcessState & pAxis->nBit) {
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_CLK_PORT,(UCHAR)pAxis->nBit);
	if(pAxis->nEncoderMode==0) {
		(dx->pSi6User->nDirReg & pAxis->nBit)?(pAxis->nPos--):(pAxis->nPos++);
		if(pAxis->nStepsPerRev) pAxis->nPos%=pAxis->nStepsPerRev;
	};
	(dx->pSi6User->nDirReg & pAxis->nBit)?(pAxis->nCount--):(pAxis->nCount++);
	if(pAxis->nStepsPerRev) pAxis->nCount%=pAxis->nStepsPerRev;
};
if(pAxis->nEncoderMode) {
	WriteByte(dx,dx->pSi6User->nAddrs+SI6_ENCODER_SELECT,(UCHAR)pAxis->nEncoderNumber);
	pAxis->nEncoderValue = ReadWord(dx,dx->pSi6User->nAddrs+0x18);
	pAxis->nEncoderValue |= (ReadWord(dx,dx->pSi6User->nAddrs+0x1a)<<16);
	if(pAxis->bEncoderReverse==TRUE) pAxis->nEncoderValue *= -1;

	nPos=pAxis->nEncoderValue;
	if(pAxis->nEncMotorRatioDenominator!=0) {
		nPos*=pAxis->nEncMotorRatioNumerator;
		nPos/=pAxis->nEncMotorRatioDenominator;
	}
	pAxis->nPos = nPos;


	if(pAxis->nStepsPerRev && !pAxis->nEncoderResetMode) {
		pAxis->nCount%=pAxis->nStepsPerRev;
		pAxis->nPos%=pAxis->nStepsPerRev;
	};

};
if(pAxis==dx->pAxis[R0AXIS]) {
	if(pAxis->nHomeSwitch && !(dx->pSi6User->nDirReg & pAxis->nBit)) {
		if(!(nLastSwitchState&pAxis->nHomeSwitch)&&(nLimitSwitch&pAxis->nHomeSwitch)){
			pAxis->nPos=0;
		};
		nLastSwitchState=nLimitSwitch;
	};

	if(abs(nLastRevPos-pAxis->nPos)>(pAxis->nStepsPerRev/2)) {
		dx->pSi6User->nRevCounter++;
	};
	nLastRevPos=pAxis->nPos;
};
}

/*
int	abs(int nn)
{
	if(nn<0) nn*=-1;
	return nn;
}
*/
int MulDiv(int v0,int v1,int v2)
{
	v0=v0*v1;
	v0=v0/v2;
	return v0;

}


void CollectGateData(IN PSI6_DEVICE_EXTENSION dx,struct AxisData *pAxis)
{
	int	ptr,gg,offset;
	char	*pData = (char *)dx->pDataArray;
	int		*pIntData,*pnData;
	int	vv,ts,nTS;
	char	GpdAmp[16]={0,8,17,25,34,42,51,59,68,76,85,93,101,110,118,127};
	int	nTemp;
	static	int	nOldTemp[2];


	if(dx->pDataArray==NULL) return;
	if((pAxis->nFinishPos-pAxis->nStartPos) == 0) return;

	ptr=MulDiv(pAxis->nPos-pAxis->nStartPos, dx->pSi6User->nSamplesLine-1, pAxis->nFinishPos-pAxis->nStartPos);
	if(ptr<0) ptr=0;
	if(ptr>=(dx->pSi6User->nSamplesLine-1)) ptr = dx->pSi6User->nSamplesLine-1;
	dx->pSi6User->nDataPtr=ptr;
	dx->pTimeSlotUser[0].Adc.nBscanNumber=ptr;

	switch(dx->pSi6User->nJobNumber) {
	default:	//DSP
		for(nTS=0;nTS<dx->pSi6User->nNumberTimeSlots;nTS++) {

			for(gg=0;gg<8;gg++) {
				if(dx->pTimeSlotUser[nTS].Gate.nCollectAmp[gg]==1) {
					pData[ptr]=dx->pTimeSlotUser[nTS].Gate.nAmplitude[gg]+128;
					pData+=dx->pSi6User->nSamplesLine;
				}
			};
			pnData=(int *)pData;

			if(dx->pTimeSlotUser[nTS].Gate.nCollectTime[1]==1) {
				nTemp=dx->pTimeSlotUser[nTS].Dsp.TimsStopPos[0]-dx->pTimeSlotUser[nTS].Dsp.TimsStartPos[0];
				pnData[ptr]=nTemp;
				pnData+=dx->pSi6User->nSamplesLine;
			};
			pData=(char *)pnData;

		}
		break;
	case 1000:	//ATP GPD TIMS
		for(gg=0,offset=0;gg<dx->pSi6User->nNumberAmpGates;gg++) {
			pData[ptr+offset]=dx->pSi6User->Amp[gg];
			offset+=dx->pSi6User->nSamplesLine;
		};
	
		pIntData=(int *)pData;
		for(gg=0,offset=0;gg<dx->pSi6User->nNumberTimeGates;gg++) {
			pIntData[ptr+offset]=dx->pSi6User->Time[gg];
			offset+=dx->pSi6User->nSamplesLine;
		};
		break;
	case 325:
		if(dx->pTimeSlotUser!=NULL) {
			vv=0;
			vv=ReadByte(dx,dx->pTimeSlotUser->Tims[0].nAddrs);
			vv|=((int)ReadByte(dx,dx->pTimeSlotUser->Tims[0].nAddrs+1)<<8);
			vv|=((int)ReadByte(dx,dx->pTimeSlotUser->Tims[0].nAddrs+2)<<16);
			vv=vv & 0x00ffffffl;
			pIntData=(int *)pData;
			pIntData[ptr]=vv;
		};
		break;
	case 485:	//VTT_HELSINKY
		pIntData=(int *)pData;
		if(dx->pTimeSlotUser!=NULL) {
			switch(dx->pSi6User->nProcessCard) {
			case USL_ATP_GPD0:	vv=ReadByte(dx,dx->pTimeSlotUser->Gpd[0].nAddrs)&0x0f;
				pData[ptr]=GpdAmp[vv]+128;
				break;
			case USL_ATP_GPD1:	vv=ReadByte(dx,dx->pTimeSlotUser->Gpd[1].nAddrs)&0x0f;
				pData[ptr]=GpdAmp[vv]+128;
				break;
			case USL_ATP_TIMS0:	
				vv=0;
				vv=ReadByte(dx,dx->pTimeSlotUser->Tims[0].nAddrs);
				vv|=((int)ReadByte(dx,dx->pTimeSlotUser->Tims[0].nAddrs+1)<<8);
				vv|=((int)ReadByte(dx,dx->pTimeSlotUser->Tims[0].nAddrs+2)<<16);
				vv=vv & 0x00ffffffl;
				pIntData[ptr]=vv;
				break;
			case USL_SI6:	vv=ReadByte(dx,dx->pSi6User->nAddrs+SI6_JOYSTICK+3);
				pData[ptr]=(vv/2) + 128;
				break;
			}
			break;
		}
		break;
	case 300: // EXHIBITION
		for(gg=0;gg<dx->pSi6User->nNumberAmpGates;gg++) {
			if(ptr<dx->pSi6User->nDataSize) pData[ptr]=dx->pTimeSlotUser[0].Gate.nAmplitude[gg+1]+128;
			pData+=dx->pSi6User->nSamplesLine;
		};
	
		pIntData=(int *)pData;
		ts=0;
		for(gg=0;gg<dx->pSi6User->nNumberTimeGates;gg++) {
			nTemp=dx->pTimeSlotUser[ts].Dsp.TimsStopPos[0]-dx->pTimeSlotUser[ts].Dsp.TimsStartPos[0];
//			if(nTemp>=0) {
				pIntData[ptr]=nTemp;
				nOldTemp[gg]=nTemp;
//			} else {
//				pIntData[ptr]=nOldTemp[gg];
//			};
	//		pnData[ptr]=ptr*kk;
	//		kk++;
			pIntData+=dx->pSi6User->nSamplesLine;
		};
		pData=(char *)pIntData;
		break;
	case 536: //corus
		for(gg=0,offset=0;gg<8;gg++) {
			if(dx->pTimeSlotUser[0].Gate.nCollectAmp[gg]==1) {
				if(ptr+offset<dx->pSi6User->nDataSize) pData[ptr+offset]=dx->pTimeSlotUser[0].Gate.nAmplitude[gg]+128;
				offset+=dx->pSi6User->nSamplesLine;
			};
		};
		break;
	case 560: //CTG_COMPOSITES
		for(nTS=0;nTS<dx->pSi6User->nNumberTimeSlots;nTS++) {

			for(gg=0;gg<8;gg++) {
				if(dx->pTimeSlotUser[nTS].Gate.nCollectAmp[gg]==1) {
					pData[ptr]=dx->pTimeSlotUser[nTS].Gate.nAmplitude[gg]+128;
					pData+=dx->pSi6User->nSamplesLine;
				}
			};
			pnData=(int *)pData;

			if(dx->pTimeSlotUser[nTS].Gate.nCollectTime[1]==1) {
				nTemp=dx->pTimeSlotUser[nTS].Dsp.TimsStopPos[0]-dx->pTimeSlotUser[nTS].Dsp.TimsStartPos[0];
				pnData[ptr]=nTemp;
				pnData+=dx->pSi6User->nSamplesLine;
			};
			pData=(char *)pnData;

		}
		break;
	case 570: //BAE_5METER
		for(nTS=0;nTS<dx->pSi6User->nNumberTimeSlots;nTS++) {

			for(gg=0;gg<8;gg++) {
				if(dx->pTimeSlotUser[nTS].Gate.nCollectAmp[gg]==1) {
					pData[ptr]=dx->pTimeSlotUser[nTS].Gate.nAmplitude[gg]+128;
					pData+=dx->pSi6User->nSamplesLine;
				}
			};
			pnData=(int *)pData;

			if(dx->pTimeSlotUser[nTS].Gate.nCollectTime[1]==1) {
				nTemp=dx->pTimeSlotUser[nTS].Dsp.TimsStopPos[0]-dx->pTimeSlotUser[nTS].Dsp.TimsStartPos[0];
				pnData[ptr]=nTemp;
				pnData+=dx->pSi6User->nSamplesLine;
			};
			pData=(char *)pnData;

		}
		break;
	case 567:	//RavensCourt
		if(dx->pSi6User->nExternalInputNumber<0) dx->pSi6User->nExternalInputNumber=0;
		if(dx->pSi6User->nExternalInputNumber>6) dx->pSi6User->nExternalInputNumber=6;
		nTemp = ReadByte(dx,dx->pSi6User->nAddrs+SI6_JOYSTICK + dx->pSi6User->nExternalInputNumber);

		if(dx->pSi6User->nMinExternalValue < dx->pSi6User->nMaxExternalValue) {
			nTemp = MulDiv(nTemp-dx->pSi6User->nMinExternalValue,127,dx->pSi6User->nMaxExternalValue - dx->pSi6User->nMinExternalValue) + 128;
		}
		if(nTemp<128)nTemp=128;
		if(nTemp>255)nTemp=255;
		pData[ptr] = nTemp & 0xff;

		break;
	};
}


int  CheckLimitSwitches(IN PSI6_DEVICE_EXTENSION dx)
{
	int	nLimitSwitch;

//	dx->pSi6User->switch_eor|=r_axis.home_switch;
	nLimitSwitch=dx->pSi6User->nLimitSwitch=ReadWord(dx,dx->pSi6User->nAddrs+SI6_PIA+0)^dx->pSi6User->nSwitchEor;
	//if(limit_switch&r_axis.home_switch) r_axis.home_flag=1;
	return nLimitSwitch;

}

BOOLEAN DspNonMuxIsr( IN PSI6_DEVICE_EXTENSION dx)
{
	BOOLEAN	flag=FALSE;
	int	nAddrs,ii,nSlot;
	unsigned char	nVv[30];
	nSlot=0;

//	dx->nIrqCount++;
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

	return flag;

}


BOOLEAN DspMuxIsr( IN PSI6_DEVICE_EXTENSION dx)
{
	BOOLEAN	flag=FALSE;
	int	nAddrs,ii,nSlot;
	unsigned char	nVv[30];

//	dx->nIrqCount++;
	if(dx->pTimeSlotUser!=NULL) {
		dx->pTimeSlotUser->nIrqCount++;

		nAddrs=dx->pTimeSlotUser->Dsp.nAddrs;
		if((ReadByte(dx,nAddrs+3) & 1)==0) return FALSE;

		for(ii=0;(ii<25)&&(ReadByte(dx,nAddrs+2) & 0x02);ii++) {
			nVv[ii]=ReadByte(dx,nAddrs);
		};
		ReadByte(dx,nAddrs+1);//Clear Interrupt this is output in non mux version

		while(ReadByte(dx,nAddrs+2) & 0x02) {
			ReadByte(dx,nAddrs); // CLEAR OUT FIFO
			dx->pTimeSlotUser->Dsp.ErrorCount++;
		};

		if((nVv[0]==0x80) && (nVv[24]==0xff) && (nVv[1]>=0) && (nVv[1]<8))  {
			nSlot=nVv[1];
			for(ii=0;ii<25;ii++) dx->pTimeSlotUser[nSlot].Dsp.Vv[ii]=nVv[ii];
		} else {
			dx->pTimeSlotUser->Dsp.ErrorCount++;
			goto function_finish;
		};

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
		dx->pTimeSlotUser[nSlot].Dsp.TimsStartPos[0]=dx->pTimeSlotUser[nSlot].Dsp.Vv[15]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[14]&0x1f)<<7);
		dx->pTimeSlotUser[nSlot].Dsp.TimsStopPos[0]=dx->pTimeSlotUser[nSlot].Dsp.Vv[17]|((dx->pTimeSlotUser[nSlot].Dsp.Vv[16]&0x1f)<<7);

		if((dx->pSi6User!=NULL)  && (dx->pSi6User->bCollectData==TRUE)) {
			for(ii=0;ii<8;ii++) {
				dx->pSi6User->Amp[ii]=dx->pTimeSlotUser->Dsp.Vv[2+ii];
			};
		};

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

	return flag;

}

