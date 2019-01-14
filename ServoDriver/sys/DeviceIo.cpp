//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
//	SERVO example
/////////////////////////////////////////////////////////////////////////////
//	DeviceIo.cpp:	Routines that interact with a device
/////////////////////////////////////////////////////////////////////////////
//	StopDevice			Stop device
//*	WriteByte			Output a byte
//*	ReadByte			Input a byte
//	SERVOStartIo		Process an IRP from the head of the device IRP queue.
//*	StoreCmds			Copy commands from IOCTL input buffer to new buffer (in dx)
//*	RunCmds				Run commands for IOCTL_SERVO_RUN_CMDS
//*	RunCmdsSynch		RunCmds called as a Critical Section routine
//*	RunWriteCmdsSynch	Run stored write commands
//*	RunReadCmdsSynch	Run stored read commands
//*	RunStartReadCmdsSynch	Run stored start read commands
//*	ProcessCmds			Process commands in given buffer.
//*	IrqConnectRoutine	Work queue item to connect to an interrupt at PASSIVE_LEVEL
//*	InterruptHandler	Handle interrupts (during StartIo processing of R/W)
//	SERVODpcForIsr		Complete current IRP
//*	Timeout1sSynch		Timeout check 
//	Timeout1s			One second timer call
//	SERVOCancelIrp		Cancel this IRP
//*	CancelCurrentIrpSynch	If a transfer is in progress, mark it for cancelling
//	SERVOCleanup		Handle IRP_MJ_CLEANUP requests
/////////////////////////////////////////////////////////////////////////////

#include "ServoDriver.h"
#include "ServoIoctl.h"
#include "C:\WindowsProjects\USLScanner\Si6Driver\sys\Si6Ioctl.h"
#include "C:\WindowsProjects\USLScanner\USLDriver\sys\USLIoctl.h"
#include "windef.h"

/////////////////////////////////////////////////////////////////////////////

NTSTATUS StoreCmds( PUCHAR* pCmds, ULONG* pCmdsLen, ULONG len, PVOID Buffer);
void WriteByte( IN PSERVO_DEVICE_EXTENSION dx, IN ULONG offset, IN UCHAR byte);
UCHAR ReadByte( IN PSERVO_DEVICE_EXTENSION dx, IN ULONG offset);
WORD ReadWord( IN PSERVO_DEVICE_EXTENSION dx, IN ULONG offset);
DWORD ReadMemInt( IN PSERVO_DEVICE_EXTENSION dx, IN ULONG offset);
void WriteMemInt( IN PSERVO_DEVICE_EXTENSION dx, IN ULONG offset, IN DWORD nValue);

BOOLEAN InterruptHandler(IN PKINTERRUPT Interrupt, IN PSERVO_DEVICE_EXTENSION dx);

BOOLEAN RunCmdsSynch( IN PDEVICE_OBJECT servoddo);
BOOLEAN RunCmds( IN PDEVICE_OBJECT servoddo, IN bool CanTrace);
bool ProcessCmds(	IN PSERVO_DEVICE_EXTENSION dx,
					IN PUCHAR Buffer, IN ULONG len,
					OUT PUCHAR OutBuffer, IN ULONG outlen,
					IN bool CanTrace
				  );

BOOLEAN RunWriteCmdsSynch( IN PSERVO_DEVICE_EXTENSION dx);
BOOLEAN RunReadCmdsSynch( IN PSERVO_DEVICE_EXTENSION dx);
BOOLEAN RunStartReadCmdsSynch( IN PSERVO_DEVICE_EXTENSION dx);

//Si6 Procedures

extern	int		nTable[10001];

BOOLEAN ServoIsr( IN PSERVO_DEVICE_EXTENSION dx);
int	abs(int nn);
int MulDiv(int v0,int v1,int v2);
int  CheckLimitSwitches(IN PSERVO_DEVICE_EXTENSION dx);
void CollectGateData(IN PSERVO_DEVICE_EXTENSION dx);
void CalculateJoystickVariables(IN PSERVO_DEVICE_EXTENSION dx);

/////////////////////////////////////////////////////////////////////////////
//	StopDevice:	Stop device

VOID StopDevice( IN PSERVO_DEVICE_EXTENSION dx)
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

void WriteByte( IN PSERVO_DEVICE_EXTENSION dx, IN ULONG offset, IN UCHAR byte)
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

UCHAR ReadByte( IN PSERVO_DEVICE_EXTENSION dx, IN ULONG offset)
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

WORD ReadWord( IN PSERVO_DEVICE_EXTENSION dx, IN ULONG offset)
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
//	ReadMemInt:	Input a word
//				Silently ignores request if register out of range
//	Don't call DebugPrint as may be called at DIRQL

DWORD ReadMemInt( IN PSERVO_DEVICE_EXTENSION dx, IN ULONG offset)
{
	if( (offset+4)>=dx->MemoryLength) return 0;
	PUCHAR Port = dx->MemoryBase+offset;
	DWORD b = ((DWORD)READ_REGISTER_UCHAR(Port+3)<<24)|
			((DWORD)READ_REGISTER_UCHAR(Port+2)<<16)|
			((DWORD)READ_REGISTER_UCHAR(Port+1)<<8)|
			READ_REGISTER_UCHAR(Port);
	return b;
}
/////////////////////////////////////////////////////////////////////////////
//	WriteMemInt:	Output a byte
//				Silently ignores request if register out of range
//	Don't call DebugPrint as may be called at DIRQL

void WriteMemInt( IN PSERVO_DEVICE_EXTENSION dx, IN ULONG offset, IN DWORD nValue)
{
	if( (offset+4)>=dx->MemoryLength) return;
	PUCHAR Port = dx->MemoryBase+offset;

	WRITE_REGISTER_UCHAR( Port, (UCHAR)nValue&0xff);
	WRITE_REGISTER_UCHAR( Port+1, (UCHAR)(nValue>>8)&0xff);
	WRITE_REGISTER_UCHAR( Port+2, (UCHAR)(nValue>>16)&0xff);
	WRITE_REGISTER_UCHAR( Port+3, (UCHAR)(nValue>>24)&0xff);

}
/////////////////////////////////////////////////////////////////////////////
//	SERVOStartIo:	Process an IRP from the head of the device IRP queue.
//				1	Only IOCTL, Read and Write IRPs get here.
//				2	The IRP is either completed here, or completed once the
//					interrupt driven read or writes completes, times out
//					or is cancelled.
//				3	Note that IRP may be cancelled at any time during this
//					processing, so we check IRP's Cancel flag when appropriate.
//				4	The Irp parameter is equal to servoddo->CurrentIrp until it
//					is completed and IoStartNextPacket called.
//
//	Runs at DISPATCH_LEVEL

VOID SERVOStartIo( IN PDEVICE_OBJECT servoddo, IN PIRP Irp)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)servoddo->DeviceExtension;
	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	PUCHAR Buffer = (PUCHAR)Irp->AssociatedIrp.SystemBuffer;

	// Zero the output count
	dx->CmdOutputCount = 0;
	dx->ConnectIntQueued = false;

	DebugPrint( "SERVOStartIo: %x %I",Irp,Irp);

	// Stop the 1 second timer if necessary
	if( dx->StopTimer)
	{
		IoStopTimer(servoddo);
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
		case IOCTL_SERVO_RUN_CMDS:
			DebugPrint( "SERVOStartIo: Run Cmds %s", dx->ConnectedToInterrupt?"(synchronised)":"");
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
				if( !KeSynchronizeExecution( dx->InterruptObject, (PKSYNCHRONIZE_ROUTINE)RunCmdsSynch, (PVOID)servoddo))
					status = STATUS_UNSUCCESSFUL;
			}
			else
			{
				if( !RunCmds(servoddo,true))
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
		case IOCTL_SERVO_CMDS_FOR_READ:
			DebugPrintMsg( "SERVOStartIo: Store cmds for read");
			status = StoreCmds( &dx->ReadCmds, &dx->ReadCmdsLen, InputLength, Buffer);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_SERVO_CMDS_FOR_READ_START:
			DebugPrintMsg( "SERVOStartIo: Store cmds for read start");
			status = StoreCmds( &dx->StartReadCmds, &dx->StartReadCmdsLen, InputLength, Buffer);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_SERVO_CMDS_FOR_WRITE:
			DebugPrintMsg( "SERVOStartIo: Store cmds for write");
			status = StoreCmds( &dx->WriteCmds, &dx->WriteCmdsLen, InputLength, Buffer);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case IOCTL_SERVO_GET_RW_RESULTS:
#if DBG
			if( dx->TxCmdOutputCount>sizeof(dx->TxResult))
			{
				DebugPrint( "SERVOStartIo: Get RW Results: dx->TxCmdOutputCount too big at %d",dx->CmdOutputCount);
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

			DebugPrint( "SERVOStartIo: Get RW Results: %d bytes",dx->CmdOutputCount);
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
		DebugPrint( "SERVOStartIo: Write %d bytes: %*s",dx->TxTotal,dx->TxTotal,dx->TxBuffer);

		// Start timeout timer
		dx->Timeout = dx->SetTimeout+1;
		IoStartTimer(servoddo);

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
		DebugPrint( "SERVOStartIo: Read %d bytes: %*s",dx->TxTotal,dx->TxTotal,dx->TxBuffer);

		// Start timeout timer
		dx->Timeout = dx->SetTimeout;
		if( dx->Timeout<=0) dx->Timeout = 10;
		IoStartTimer(servoddo);

		// Run StartReadCmds if available
		if( dx->StartReadCmds!=NULL)
		{
			DebugPrintMsg( "SERVOStartIo: Read: Running start read commands");
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
	DebugPrint( "SERVOStartIo: CmdOutputCount %d", dx->CmdOutputCount);
	CompleteIrp(Irp, status, dx->CmdOutputCount);
	IoStartNextPacket( servoddo, TRUE);
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
//	RunCmds:		Run commands for IOCTL_SERVO_RUN_CMDS
//	RunCmdsSynch:	RunCmds called as a Critical Section routine at DIRQL
//
//	Only do trace output if not run as a Critical Section routine.
//
//	Runs at DISPATCH_LEVEL or DIRQL
//	Return	TRUE if commands ran (successfully or not)

BOOLEAN RunCmdsSynch( IN PDEVICE_OBJECT servoddo)
{
	return RunCmds( servoddo, true);
}

BOOLEAN RunCmds( IN PDEVICE_OBJECT servoddo, IN bool CanTrace)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)servoddo->DeviceExtension;
	PIRP Irp = servoddo->CurrentIrp;
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

BOOLEAN RunWriteCmdsSynch( IN PSERVO_DEVICE_EXTENSION dx)
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

BOOLEAN RunReadCmdsSynch( IN PSERVO_DEVICE_EXTENSION dx)
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

BOOLEAN RunStartReadCmdsSynch( IN PSERVO_DEVICE_EXTENSION dx)
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
	"SERVO_OR",
	"SERVO_AND",
	"SERVO_XOR",

	"SERVO_WRITE",
	"SERVO_READ",

	"SERVO_DELAY",

	"SERVO_WRITES",
	"SERVO_READS",

	"SERVO_IRQ_CONNECT",

	"SERVO_TIMEOUT",
	"SERVO_WRITE_NEXT",
	"SERVO_READ_NEXT",
};
static int NUM_PHD_IO_CMDS = sizeof(PHD_IO_CMDS)/sizeof(char*);
#endif

/////////////////////////////////////////////////////////////////////////////
//	Useful macros for ProcessCmds
//	GetUChar:			Get next UCHAR in newly declared variable (if available)
//	GetUCharNoDeclare:	Get next UCHAR in existing variable (if available)
//	SetUChar:			Store UCHAR in output buffer (if there's room)

#define GetUChar(var) if( ByteNo>=len) { FailCode=SERVO_NO_CMD_PARAMS; goto fail; } UCHAR var = *Buffer++; ByteNo++
#define GetUCharNoDeclare(var) if( ByteNo>=len) { FailCode=SERVO_NO_CMD_PARAMS; goto fail; }; var = *Buffer++; ByteNo++
#define SetUChar(b) if( OutByteNo>=outlen) { FailCode=SERVO_NO_OUTPUT_ROOM; goto fail; } *OutBuffer++ = (b); OutByteNo++; dx->CmdOutputCount++
#define GetInt(var) if( (ByteNo+4)>=len) { FailCode=SERVO_NO_CMD_PARAMS; goto fail; } int var = (int *)Buffer++; ByteNo+=4

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

bool ProcessCmds(	IN PSERVO_DEVICE_EXTENSION dx,
					IN PUCHAR Buffer, IN ULONG len,
					OUT PUCHAR OutBuffer, IN ULONG outlen,
					IN bool CanTrace
				  )
{
	if( CanTrace) { DebugPrint( "ProcessCmds. input:%d output:%d", len, outlen); }

	PIRP Irp = dx->servoddo->CurrentIrp;

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
		*Result = SERVO_OK;
		*(Result+1) = 0;
		dx->CmdOutputCount += ResultLen;
	}
	USHORT FailCode = SERVO_OK;

	USHORT ByteNo=0;
	USHORT OutByteNo=0;
	// Loop through all commands
	while( ByteNo<len)
	{
		// See if we've been cancelled
		if( Irp->Cancel)
		{
			FailCode = SERVO_CANCELLED;
			goto fail;
		}
		// Get next command
		UCHAR Cmd = *Buffer++;
		ByteNo++;
		UCHAR Size = Cmd&0xC0;
		if( Size!=0)
		{
			FailCode = SERVO_BYTE_CMDS_ONLY;
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
		case SERVO_OR:
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
		case SERVO_AND:
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
		case SERVO_XOR:
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
		case SERVO_WRITE:
		{
			GetUChar(reg);
			GetUChar(bvalue);
			if( CanTrace) { DebugPrint( "Write %xh %2xh", reg, bvalue); }
			WriteByte( dx, reg, bvalue);
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SERVO_READ:
		{
			GetUChar(reg);
			UCHAR bvalue = ReadByte( dx, reg);
			SetUChar(bvalue);
			if( CanTrace) { DebugPrint( "Read %xh %2xh", reg, bvalue); }
			break;
		}
				// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SERVO_WRITE_DPRAM:
		{
			ByteNo+=3;
			Buffer+=3;
			PDWORD pnBuffer = (PDWORD)Buffer;
			if( (ByteNo+4)>(USHORT)len) { 
				FailCode=SERVO_NO_CMD_PARAMS;
				goto fail; 
			};
			int reg = *pnBuffer++;
			Buffer+=4;
			ByteNo+=4;
			if( (ByteNo+4)>(USHORT)len) { 
				FailCode=SERVO_NO_CMD_PARAMS;
				goto fail; 
			};
			int nValue = *pnBuffer++;
			Buffer+=4;
			ByteNo+=4;
			if( CanTrace) { DebugPrint( "Write %xh %xh", reg, nValue); }
			WriteMemInt( dx, reg*4, nValue);
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SERVO_READ_DPRAM:
		{
			ByteNo+=3;
			Buffer+=3;
			PDWORD pnBuffer = (PDWORD)Buffer;
			if( (ByteNo+4)>(USHORT)len) { 
				FailCode=SERVO_NO_CMD_PARAMS;
				goto fail; 
			};
			int reg = *pnBuffer++;
			Buffer+=4;
			ByteNo+=4;
			int nValue = ReadMemInt( dx, reg*4);
			if( (OutByteNo+4)>(USHORT)outlen) { 
				FailCode=SERVO_NO_OUTPUT_ROOM; 
				goto fail;
			}
			PDWORD npOutBuffer = (PDWORD)OutBuffer;
			*npOutBuffer = nValue;
			OutBuffer+=4;
			OutByteNo+=4;
			dx->CmdOutputCount+=4;

			if( CanTrace) { DebugPrint( "Read %xh %xh", reg, nValue); }
			break;
		}

		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SERVO_DELAY:
		{
			GetUChar(delay);
			if( CanTrace) { DebugPrint( "Delay %dus", delay); }
			if( delay>60) { FailCode = SERVO_DELAY_TOO_LONG; goto fail; }
			KeStallExecutionProcessor(delay);
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SERVO_WRITES:
		{
			GetUChar(reg);
			GetUChar(count);
			GetUChar(delay);
			if( CanTrace) { DebugPrint( "Write %d values to %d, delay %dus", count, reg, delay); }
			if( delay>60) { FailCode = SERVO_DELAY_TOO_LONG; goto fail; }
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
		case SERVO_READS:
		{
			GetUChar(reg);
			GetUChar(count);
			GetUChar(delay);
			if( CanTrace) { DebugPrint( "Read %d values from %d, delay %dus", count, reg, delay); }
			if( delay>60) { FailCode = SERVO_DELAY_TOO_LONG; goto fail; }
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
		case SERVO_IRQ_CONNECT:
		{
			if( !dx->GotInterrupt) { FailCode = SERVO_NO_INTERRUPT; goto fail; }
			if( KeGetCurrentIrql()>DISPATCH_LEVEL) { FailCode = SERVO_ALREADY_CONNECTED_TO_INT; goto fail; }
			GetUCharNoDeclare(dx->InterruptReg);
			if( dx->InterruptReg>=dx->PortLength) { FailCode = SERVO_NOT_IN_RANGE; goto fail; }
			GetUCharNoDeclare(dx->InterruptRegMask);
			GetUCharNoDeclare(dx->InterruptRegValue);
			if( (dx->InterruptRegValue&dx->InterruptRegMask) != dx->InterruptRegValue)
				{ FailCode = SERVO_BAD_INTERRUPT_VALUE; goto fail; }

			if( CanTrace) { DebugPrint( "Connect.  Reg %d  Mask %2x Value %2x",
				dx->InterruptReg, dx->InterruptRegMask, dx->InterruptRegValue); }

			dx->ConnectIntQueued = true;
			ExQueueWorkItem( &dx->ConnectIntWQI, DelayedWorkQueue);

			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SERVO_TIMEOUT:
			GetUCharNoDeclare(dx->SetTimeout);
			break;
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SERVO_WRITE_NEXT:
		{
			if( dx->Timeout==-1) { FailCode = SERVO_CANNOT_RW_NEXT; goto fail; }
			if( dx->TxLeft==0) { FailCode = SERVO_NO_DATA_LEFT_TO_TRANSFER; goto fail; }
			GetUChar(reg);
			WriteByte( dx, reg, *dx->TxBuffer++);
			dx->TxLeft--;
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		case SERVO_READ_NEXT:
		{
			if( dx->Timeout==-1) { FailCode = SERVO_CANNOT_RW_NEXT; goto fail; }
			if( dx->TxLeft==0) { FailCode = SERVO_NO_DATA_LEFT_TO_TRANSFER; goto fail; }
			GetUChar(reg);
			*dx->TxBuffer++ = ReadByte( dx, reg);
			dx->TxLeft--;
			break;
		}
		// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / //
		default:
			FailCode = SERVO_UNRECOGNISED_CMD;
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
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)Context;

	DebugPrint( "IrqConnectRoutine");
	dx->ConnectIntQueued = false;

	// Get the current IRP that we are working on
	PIRP Irp = dx->servoddo->CurrentIrp;
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
				*dx->RunCmdsOutBuffer = SERVO_CANNOT_CONNECT_TO_INTERRUPT;
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
	IoStartNextPacket( dx->servoddo, TRUE);
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

BOOLEAN InterruptHandler(IN PKINTERRUPT Interrupt, IN PSERVO_DEVICE_EXTENSION dx)
{

	unsigned char OldIrql;

	KeRaiseIrql(31,&OldIrql);

	BOOLEAN flag=ServoIsr(dx);

	KeLowerIrql(OldIrql);

	if(flag==FALSE) return FALSE;



	// See if interrupt is ours
	dx->TxLastIntReg = ReadByte( dx, dx->InterruptReg);
	if( (dx->TxLastIntReg&dx->InterruptRegMask) != dx->InterruptRegValue)
		return FALSE;

	// If no transfer in progress then no further processing required
	if( dx->Timeout==-1) return TRUE;

	// See if current IRP being cancelled

	PDEVICE_OBJECT servoddo = dx->servoddo;
	PIRP Irp = servoddo->CurrentIrp;
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
		IoRequestDpc( servoddo, Irp, dx);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	SERVODpcForIsr:	Complete current IRP

VOID SERVODpcForIsr(IN PKDPC Dpc, IN PDEVICE_OBJECT servoddo, 
					IN PIRP Irp, IN PSERVO_DEVICE_EXTENSION dx)
{

//	if((dx->pIntEvent)&&(dx->pServoUser->bCollectData==TRUE)){
//		KeSetEvent(dx->pIntEvent, 0, FALSE);
//	}

	dx->Timeout = -1;
	ULONG BytesTxd = dx->TxTotal-dx->TxLeft;
	if( Irp->Cancel) dx->TxStatus = STATUS_CANCELLED;

	DebugPrint("SERVODpcForIsr: Status %x Info %d", dx->TxStatus, BytesTxd);

	// Remove cancel routine
	KIRQL OldIrql;
	IoAcquireCancelSpinLock( &OldIrql);
	IoSetCancelRoutine( Irp, NULL);
	IoReleaseCancelSpinLock(OldIrql);

	// Unlock device and complete IRP
	UnlockDevice(dx);
	CompleteIrp(Irp, dx->TxStatus, BytesTxd);
	IoStartNextPacket( servoddo, TRUE);

	// Stop timer calls
	dx->StopTimer = true;
	
}

/////////////////////////////////////////////////////////////////////////////
//	Timeout1sSynch:	Timeout check 
//	Return TRUE if operation has timed out
//	Called as a Critical Section routine

static BOOLEAN Timeout1sSynch( IN PSERVO_DEVICE_EXTENSION dx)
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

VOID Timeout1s( IN PDEVICE_OBJECT servoddo, IN PSERVO_DEVICE_EXTENSION dx)
{
	if( dx->Timeout==-1) return;

	DebugPrint("Timeout1s: Timeout is %d",dx->Timeout);
	PIRP Irp = servoddo->CurrentIrp;
#if DBG
	if( Irp==NULL) return;
#endif
	if( Irp->Cancel || KeSynchronizeExecution( dx->InterruptObject, (PKSYNCHRONIZE_ROUTINE)Timeout1sSynch, dx))
		SERVODpcForIsr( NULL, servoddo, servoddo->CurrentIrp, dx);
}

/////////////////////////////////////////////////////////////////////////////
//	SERVOCancelIrp:
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

VOID SERVOCancelIrp( IN PDEVICE_OBJECT servoddo, IN PIRP Irp)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)servoddo->DeviceExtension;
	DebugPrint("SERVOCancelIrp: Cancelling %x %I", Irp, Irp);
	if( Irp==servoddo->CurrentIrp)
	{
		DebugPrintMsg("SERVOCancelIrp: IRP running in StartIo");
		// IRP is being processed by SERVOStartIo.
		// Irp->Cancel flag already set.
		// SERVOStartIo or timeout will detect Cancel flag and cancel IRP in due course
		IoReleaseCancelSpinLock(Irp->CancelIrql);
	}
	else
	{
		DebugPrintMsg("SERVOCancelIrp: IRP in StartIo queue");
		// IRP is still in StartIo device queue.
		// Just dequeue and cancel it.  No need to start next IRP.
		BOOLEAN dequeued = KeRemoveEntryDeviceQueue(
								&servoddo->DeviceQueue,
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

static BOOLEAN CancelCurrentIrpSynch( IN PSERVO_DEVICE_EXTENSION dx)
{
	if( dx->Timeout==-1)
		return FALSE;
	dx->Timeout = -1;
	dx->TxStatus = STATUS_CANCELLED;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//	SERVOCleanup:
//
//	Description:
//		Handle IRP_MJ_CLEANUP requests
//		Cancel queued IRPs which match given FileObject
//
//		SERVO cancels *all* queued IRPs and the current Irp
//
//	Arguments:
//		Pointer to our PHD DO
//		Pointer to the IRP
//			IrpStack->FileObject has handle to file
//
//	Return Value:
//		This function returns STATUS_XXX

NTSTATUS SERVODispatchCleanup( IN PDEVICE_OBJECT servoddo, IN PIRP Irp)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)servoddo->DeviceExtension;
	DebugPrintMsg("SERVODispatchCleanup");
	KIRQL OldIrql;
	IoAcquireCancelSpinLock(&OldIrql);

	// Cancel all IRPs in the I/O Manager maintained queue in device object
	PKDEVICE_QUEUE_ENTRY QueueEntry;
	while( (QueueEntry=KeRemoveDeviceQueue(&servoddo->DeviceQueue)) != NULL)
	{
		PIRP CancelIrp = CONTAINING_RECORD( QueueEntry, IRP, Tail.Overlay.DeviceQueueEntry);
		CancelIrp->Cancel = TRUE;
		CancelIrp->CancelIrql = OldIrql;
		CancelIrp->CancelRoutine = NULL;

		IoReleaseCancelSpinLock(OldIrql);
		DebugPrint("SERVODispatchCleanup: Cancelling %x %I",CancelIrp,CancelIrp);
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
			if( servoddo->CurrentIrp!=NULL)
			{
				DebugPrint("SERVODispatchCleanup: Cancelled in-progress IRP %x %I",servoddo->CurrentIrp,servoddo->CurrentIrp);
				SERVODpcForIsr( NULL, servoddo, servoddo->CurrentIrp, dx);
			}
		}
	}

	return CompleteIrp( Irp, STATUS_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////////
BOOLEAN ServoIsr( IN PSERVO_DEVICE_EXTENSION dx)
{
	bool flag = FALSE;
	int	ii,nPos,nOldPos;
	int	*pAddrs = (int *)dx->MemoryBase;
	static	int nLastPos[8];
	static	bool bFlag=FALSE;

	dx->pServoUser->nIrqCount++;

	for(ii=0;ii<8;ii++) {
		dx->pAxis[ii]->nPos = pAddrs[dx->pAxis[ii]->nPhysicalAxis-1+PMAC_POS]-dx->pAxis[ii]->nOffset;

		if(dx->pAxis[ii]->nStepsPerRev>0) {
			nPos = dx->pAxis[ii]->nPos % dx->pAxis[ii]->nStepsPerRev;
			if(bFlag==FALSE) nLastPos[ii]=nPos;
			if(nPos<0) nPos+=dx->pAxis[ii]->nStepsPerRev;
			if(abs(nLastPos[ii] - nPos)>1000) dx->pServoUser->nRevCounter++;
			nLastPos[ii]=nPos;
			bFlag=TRUE;
		};
		dx->pAxis[ii]->nStatus = pAddrs[dx->pAxis[ii]->nPhysicalAxis-1+PMAC_MOTOR_STATUS];
	};
	bFlag=TRUE;
	flag=TRUE;
	if(dx->pServoUser->bCollectData==TRUE) {
//		CollectGateData(dx);
	};
	if(dx->pServoUser->bJoystickEnabled==TRUE) {
//		CalculateJoystickVariables(dx);
	};

	if((dx->pIntEvent)&&(dx->pServoUser->bCollectData==TRUE)){
		KeSetEvent(dx->pIntEvent, 0, FALSE);
	}

	return flag;

}


void CollectGateData(IN PSERVO_DEVICE_EXTENSION dx)
{
	int	ptr,gg,ts,nTS;
	char	*pData = (char *)dx->pDataArray;
	int		*pnData;
	int kk,nTemp;
	struct	AxisData	*pAxis = dx->pAxis[dx->pServoUser->nWhichAxis];
	static	int	nOldTemp[2];
	int	nPos;

	if(pAxis==NULL) return;
	if(dx->pDataArray==NULL) return;
	if((pAxis->nFinishPos-pAxis->nStartPos) == 0) return;

	if(dx->pServoUser->bContinuousRotate==FALSE) {
		ptr=MulDiv(pAxis->nPos-pAxis->nStartPos, dx->pServoUser->nSamplesLine-1, pAxis->nFinishPos-pAxis->nStartPos);
	} else {
		if(pAxis->nStepsPerRev>0) {
			nPos = pAxis->nPos % pAxis->nStepsPerRev;
		} else {
			nPos = pAxis->nPos;
		}
		if(nPos<0) nPos+=pAxis->nStepsPerRev;
		ptr=MulDiv(nPos-pAxis->nStartPos, dx->pServoUser->nSamplesLine-1, pAxis->nFinishPos-pAxis->nStartPos);
	}

	if(ptr<0) ptr=0;
	if(ptr>=dx->pServoUser->nSamplesLine) ptr = dx->pServoUser->nSamplesLine-1;
	dx->pServoUser->nDataPtr=ptr;
	dx->pTimeSlotUser[0].Adc.nBscanNumber=ptr;
/*
	for(ts=0,kk=1;ts<dx->pServoUser->nNumberTimeSlots;ts++) {
		for(gg=0;gg<dx->pServoUser->nNumberAmpGates[ts];gg++) {
			if(dx->pTimeSlotUser[ts].Gate.nCollectAmp[gg]==TRUE) {
				pData[ptr]=dx->pTimeSlotUser[ts].Gate.nAmplitude[gg]+128;
			}
			pData+=dx->pServoUser->nSamplesLine;
		};
	
		pnData=(int *)pData;
		for(gg=0;gg<dx->pServoUser->nNumberTimeGates[ts];gg++) {
			nTemp=dx->pTimeSlotUser[ts].Dsp.TimsStopPos[0]-dx->pTimeSlotUser[ts].Dsp.TimsStartPos[0];
			if(nTemp>=0) {
				pnData[ptr]=nTemp;
				nOldTemp[gg]=nTemp;
			} else {
				pnData[ptr]=nOldTemp[gg];
			};
	//		pnData[ptr]=ptr*kk;
	//		kk++;
			pnData+=dx->pServoUser->nSamplesLine;
		};
		pData=(char *)pnData;

	};
*/

	for(nTS=0;nTS<dx->pServoUser->nNumberTimeSlots;nTS++) {

		for(gg=0;gg<8;gg++) {
			if(dx->pTimeSlotUser[nTS].Gate.nCollectAmp[gg]==1) {
				pData[ptr]=dx->pTimeSlotUser[nTS].Gate.nAmplitude[gg]+128;
				pData+=dx->pServoUser->nSamplesLine;
			}
		};
		pnData=(int *)pData;

		if(dx->pTimeSlotUser[nTS].Gate.nCollectTime[1]==1) {
			nTemp=dx->pTimeSlotUser[nTS].Dsp.TimsStopPos[0]-dx->pTimeSlotUser[nTS].Dsp.TimsStartPos[0];
			pnData[ptr]=nTemp;
			pnData+=dx->pServoUser->nSamplesLine;
		};
		pData=(char *)pnData;
	}

}

#define	MARGIN 3

void CalculateJoystickVariables(IN PSERVO_DEVICE_EXTENSION dx)
{
	int	nXTemp,nYTemp,nZTemp,nXtTemp,nSTemp,nSide,nTemp,nAxis,nSign,nReqPos;
	static	int	nWhichAxisLoop=0,nOldSide=0;
	int	*pDpram = (int *)dx->MemoryBase;

	nXTemp=dx->pServoUser->nXJoyValue;
	nYTemp=dx->pServoUser->nYJoyValue;
	nZTemp=dx->pServoUser->nZJoyValue;
	nXtTemp=dx->pServoUser->nSJoyValue;
	nSTemp=dx->pServoUser->nTJoyValue;

	nWhichAxisLoop++;
	nWhichAxisLoop%=4;

	if(nSTemp<30) nSide=0;	//LEFT
	if(nSTemp>240) nSide=1;	//RIGHT
	if((nSTemp>=30)&&(nSTemp<=240)) nSide=2;

	switch(nWhichAxisLoop) {
	case 0:	nTemp=nXTemp;
		nAxis=X0AXIS;
		break;
	case 1:	nTemp=nYTemp;
		(nSide==0)?(nAxis=Y0AXIS):(nAxis=Y1AXIS);
		break;
	case 2:	nTemp=nZTemp;
		(nSide==0)?(nAxis=Z0AXIS):(nAxis=Z1AXIS);
		break;
	case 3:	nTemp=nXtTemp;
		(nSide==0)?(nAxis=R0AXIS):(nAxis=R1AXIS);
		break;
	};

	if(nOldSide-nSide) {
		dx->pAxis[X0AXIS]->nRLdiff=dx->pAxis[X1AXIS]->nRLdiff=dx->pAxis[X0AXIS]->nPos-dx->pAxis[X1AXIS]->nPos;	
		dx->pAxis[Y0AXIS]->nRLdiff=dx->pAxis[Y1AXIS]->nRLdiff=dx->pAxis[Y0AXIS]->nPos-dx->pAxis[Y1AXIS]->nPos;	
		dx->pAxis[Z0AXIS]->nRLdiff=dx->pAxis[Z1AXIS]->nRLdiff=dx->pAxis[Z0AXIS]->nPos-dx->pAxis[Z1AXIS]->nPos;	
		dx->pAxis[R0AXIS]->nRLdiff=dx->pAxis[R1AXIS]->nRLdiff=dx->pAxis[R0AXIS]->nPos-dx->pAxis[R1AXIS]->nPos;	
		nOldSide=nSide;
	};
	
	if(nAxis<0) return;
	nTemp -= dx->pAxis[nAxis]->nJoystickZero;
	(dx->pAxis[nAxis]->bJoystickReverse)?(nTemp *= -1):(nTemp);
	if(dx->pAxis[nAxis]->nJoystickGain==0)	dx->pAxis[nAxis]->nJoystickGain = 100;
	if((abs(nTemp) > 40) || (abs(nTemp)<=MARGIN) || (dx->pAxis[nAxis]->bAvailable==FALSE)) {
		nTemp = 0;
		nReqPos=0;
//		dx->pAxis[nAxis]->nDesirePos=dx->pAxis[nAxis]->nDesirePos+nReqPos;
	} else {
		nSign = nTemp;
		nReqPos=(abs(nTemp)-MARGIN)*dx->pAxis[nAxis]->nJoystickGain;
		if(nSign<0) nReqPos*= -1;
		dx->pAxis[nAxis]->nDesirePos=dx->pAxis[nAxis]->nPos+nReqPos;
	};
	if(dx->pAxis[nAxis]->bAvailable==TRUE) {
		pDpram[dx->pAxis[nAxis]->nPhysicalAxis-1+PMAC_DESIRE_POS]=dx->pAxis[nAxis]->nDesirePos+dx->pAxis[nAxis]->nOffset;
	};
	if((nSide==2)&&(nAxis>0)) {
		dx->pAxis[nAxis-1]->nDesirePos=dx->pAxis[nAxis]->nDesirePos+dx->pAxis[nAxis]->nRLdiff;
		if(dx->pAxis[nAxis-1]->bAvailable==TRUE) {
			pDpram[dx->pAxis[nAxis-1]->nPhysicalAxis-1+PMAC_DESIRE_POS]=dx->pAxis[nAxis-1]->nDesirePos+dx->pAxis[nAxis-1]->nOffset;
		};
	};

}


int	abs(int nn)
{
	if(nn<0) nn*=-1;
	return nn;
}

int MulDiv(int v0,int v1,int v2)
{
	v0=v0*v1;
	v0=v0/v2;
	return v0;

}
