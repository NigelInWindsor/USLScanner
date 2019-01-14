//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
//	USL example
/////////////////////////////////////////////////////////////////////////////
//	dispatch.cpp:		I/O IRP handlers
/////////////////////////////////////////////////////////////////////////////
//	USLCreate			Handle Create/Open file IRP
//*	ClaimResources		Ask kernel to give is the resources we want
//*	UnclaimResources	Release our resources back to the system
//*	TranslateAndMapResources	Translate and map our resources
//*	FreeResources		Unmap memory and disconnect from our interrupt
//*	GetResourcesFromFilename	Extract required resources from filename string
//*	usStrCmpN			Compare section of Unicode string with a wide string
//*	usGetHex			Get hex value from section of Unicode string
//*	usGetDec			Get decimal value from section of Unicode string
//	USLClose			Handle Close file IRPs
//	USLRead			Handle Read IRPs
//	USLWrite			Handle Write IRPs
//	USLDeviceControl	Handle DeviceIoControl IRPs
//	USLSystemControl	Handle WMI IRPs
/////////////////////////////////////////////////////////////////////////////

#include "USLDriver.h"
#include "USLIoctl.h"
#include "C:\WindowsProjects\USLScanner\Si6Driver\sys\Si6Ioctl.h"
#include "C:\WindowsProjects\USLScanner\ServoDriver\sys\ServoIoctl.h"
#include "windef.h"

KSPIN_LOCK BufferLock;
PUCHAR	Buffer = NULL;
ULONG	BufferSize = 0;
char	Version[40];
extern	char	Trace[];

/////////////////////////////////////////////////////////////////////////////

NTSTATUS GetResourcesFromFilename( IN PUNICODE_STRING usfilename, IN PUSL_DEVICE_EXTENSION dx);
bool usStrCmpN( PUNICODE_STRING us, int& pos, int maxchars, PWSTR cmp, int cmpchars);
bool usGetHex( PUNICODE_STRING us, int& pos, int maxchars, ULONG& value);
bool usGetDec( PUNICODE_STRING us, int& pos, int maxchars, ULONG& value);

NTSTATUS ClaimResources( IN PDEVICE_OBJECT uslddo);
NTSTATUS TranslateAndMapResources( IN PDEVICE_OBJECT uslddo);
void UnclaimResources( IN PDEVICE_OBJECT uslddo);
void FreeTimeSlot( IN PUSL_DEVICE_EXTENSION dx);
void FreeSi6UserStruct( IN PUSL_DEVICE_EXTENSION dx);
void FreeServoUserStruct( IN PUSL_DEVICE_EXTENSION dx);
void FreeBscanArray( IN PUSL_DEVICE_EXTENSION dx);

NTSTATUS PowerUpDevice( IN PDEVICE_OBJECT uslddo);
NTSTATUS FindParallelPort();
NTSTATUS  AbcConfigCallback(
	IN PVOID Context, IN PUNICODE_STRING PathName,
	IN INTERFACE_TYPE BusType, IN ULONG BusNumber, IN PKEY_VALUE_FULL_INFORMATION *BusInfo,
	IN CONFIGURATION_TYPE CtrlrType, IN ULONG CtrlrNumber, IN PKEY_VALUE_FULL_INFORMATION *CtrlrInfo,
	IN CONFIGURATION_TYPE PeripheralType, IN ULONG PeripheralNumber, IN PKEY_VALUE_FULL_INFORMATION *PeripheralInfo
	);
/////////////////////////////////////////////////////////////////////////////
//	USLCreate:
//
//	Description:
//		Handle IRP_MJ_CREATE requests
//
//	Arguments:
//		Pointer to our PHD DO
//		Pointer to the IRP
//			IrpStack->Parameters.Create.xxx has create parameters
//			IrpStack->FileObject->FileName has file name of device
//
//	Return Value:
//		This function returns STATUS_XXX

NTSTATUS USLCreate(	IN PDEVICE_OBJECT USLddo,
						IN PIRP Irp)
{
	int	ii;
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)USLddo->DeviceExtension;
	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	DebugPrint( "Create File is %T", &(IrpStack->FileObject->FileName));

	dx->GotPort = false;
	dx->GotInterrupt = false;
	dx->PortNeedsMapping = false;
	dx->ConnectedToInterrupt = false;
	dx->ResourceOverride = FALSE;
	dx->mdlBscanArray = NULL;
	dx->pBscanArray = NULL;

	// Get resources from filename string
	PUNICODE_STRING usfilename = &(IrpStack->FileObject->FileName);
	NTSTATUS status = GetResourcesFromFilename(usfilename,dx);
	if( !NT_SUCCESS(status)) goto fail;

	// We must have IO port resource
	if( !dx->GotPort)
	{
		DebugPrintMsg("No IO Port resource in filename");
		status = STATUS_INVALID_PARAMETER;
		goto fail;
	}

	// Claim resources
	status = ClaimResources(USLddo);
	if( !NT_SUCCESS(status))
	{
		DebugPrintMsg("Could not ClaimResources");
		goto fail;
	}

	// Translate and map resources
	status = TranslateAndMapResources(USLddo);
	if( !NT_SUCCESS(status))
	{
		UnclaimResources(USLddo);
		goto fail;
	}
	//USL INITIALIZATION


	
	// Complete
	return CompleteIrp(Irp,status);

	// On error, make sure everything's off
fail:
	dx->GotPort = false;
	dx->GotInterrupt = false;
	dx->PortNeedsMapping = false;
	dx->ConnectedToInterrupt = false;
	return CompleteIrp(Irp,status);
}

/////////////////////////////////////////////////////////////////////////////
//	ClaimResources:	Ask kernel to give is the resources we want

NTSTATUS ClaimResources( IN PDEVICE_OBJECT USLddo)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)USLddo->DeviceExtension;

	// Get resource count: either 1 (IOport) or 2 (IOport&IRQ) or 3(MEM&IOPort&IRQ)
	ULONG PartialResourceCount = 1;
	if( dx->GotInterrupt) PartialResourceCount++;
	if( dx->GotMemory) PartialResourceCount++;

	// Get size of required CM_RESOURCE_LIST
	ULONG ListSize = FIELD_OFFSET( CM_RESOURCE_LIST, List[0]);

	ListSize += sizeof( CM_FULL_RESOURCE_DESCRIPTOR) +
				((PartialResourceCount-1) * sizeof(CM_PARTIAL_RESOURCE_DESCRIPTOR));

	// Allocate CM_RESOURCE_LIST
	PCM_RESOURCE_LIST ResourceList = (PCM_RESOURCE_LIST)ExAllocatePool( PagedPool, ListSize);
	if( ResourceList==NULL)
	{
		DebugPrintMsg("Cannot allocate memory for ResourceList");
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	RtlZeroMemory( ResourceList, ListSize);

	// Only one Full Resource Descriptor needed, for ISA
	ResourceList->Count = 1;

	// Initialise Full Resource Descriptor
	PCM_FULL_RESOURCE_DESCRIPTOR FullRD = &ResourceList->List[0];
	FullRD->InterfaceType = Isa;
	FullRD->BusNumber = 0;

	FullRD->PartialResourceList.Count = PartialResourceCount;

	// Initialise Partial Resource Descriptor for IO port
	PCM_PARTIAL_RESOURCE_DESCRIPTOR resource = &FullRD->PartialResourceList.PartialDescriptors[0];
	resource->Type = CmResourceTypePort;
	resource->ShareDisposition = CmResourceShareDriverExclusive;
	resource->Flags = CM_RESOURCE_PORT_IO;
	resource->u.Port.Start = dx->PortStartAddress;
	resource->u.Port.Length = dx->PortLength;

	if(dx->GotMemory)
	{
		resource++;
		resource->Type = CmResourceTypeMemory;
		resource->ShareDisposition = CmResourceShareDriverExclusive;
		resource->Flags = CM_RESOURCE_MEMORY_READ_WRITE;
		resource->u.Memory.Start = dx->MemoryStartAddress;
		resource->u.Memory.Length = dx->MemoryLength;
	}

	// Initialise Partial Resource Descriptor for Interrupt
	if( dx->GotInterrupt)
	{
		resource++;
		resource->Type = CmResourceTypeInterrupt;
		resource->ShareDisposition = CmResourceShareDriverExclusive;
		if( dx->Mode==Latched)
			resource->Flags = CM_RESOURCE_INTERRUPT_LATCHED;
		else
			resource->Flags = CM_RESOURCE_INTERRUPT_LEVEL_SENSITIVE;
		resource->u.Interrupt.Level = dx->Irql;
		resource->u.Interrupt.Vector = dx->Irql;
		resource->u.Interrupt.Affinity = 1;
	}

	// Ask for resources for the driver
	DebugPrint("Allocating %d resources",PartialResourceCount);
	DebugPrint("USLddo->DriverObject %x",USLddo->DriverObject);
	if( dx->ResourceOverride) {
		DebugPrintMsg("Resource override conflict");
	}
	BOOLEAN ConflictDetected;
	NTSTATUS status = IoReportResourceUsage( NULL,
						USLddo->DriverObject, ResourceList, ListSize,	// Driver resources
						NULL, NULL, 0,	// Device resources
						dx->ResourceOverride, &ConflictDetected);
	// Cope (or override) if resource conflict found
	if( ConflictDetected)
	{
		DebugPrintMsg("ConflictDetected");
		// NT4 returns STATUS_SUCCESS;  W2000 returns !NT_SUCCESS
		if( dx->ResourceOverride)
		{
			DebugPrintMsg("Conflict detected and overridden");
			status = STATUS_SUCCESS;
		}
	}
	// Free allocated memory
	ExFreePool(ResourceList);
	return status;
}

/////////////////////////////////////////////////////////////////////////////
//	UnclaimResources:	Release our resources back to the system

void UnclaimResources( IN PDEVICE_OBJECT USLddo)
{
	DebugPrintMsg("Freeing all allocated resources");
	// Release all driver's resources by declaring we have none.
	CM_RESOURCE_LIST ResourceList;
	ResourceList.Count = 0;
	BOOLEAN ConflictDetected;
	IoReportResourceUsage( NULL,
					USLddo->DriverObject, &ResourceList, sizeof(ResourceList),	// Driver resources
					NULL, NULL, 0,	// Device resources
					FALSE, &ConflictDetected);
	// ignore return result
}

/////////////////////////////////////////////////////////////////////////////
//	TranslateAndMapResources:	Translate and map our resources

NTSTATUS TranslateAndMapResources( IN PDEVICE_OBJECT USLddo)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)USLddo->DeviceExtension;

//	FindParallelPort();
/*
    BUS_DATA_TYPE BusDataType;
    ULONG BusNumber;
    ULONG SlotNumber;
    UCHAR Buffer[40];
    ULONG Length=40;

	for(BusNumber=0;BusNumber<5;BusNumber++) {
		for(SlotNumber=0;SlotNumber<8;SlotNumber++) {
			RtlZeroMemory( &BusDataType, sizeof(BusDataType));
//			BusDataType=4;
			HalGetBusData(BusDataType,BusNumber,SlotNumber,Buffer,Length);
			DebugPrint( "Bus,%d Slot,%d Type,%d Buffer,%s",BusNumber,SlotNumber,BusDataType,Buffer);
		};
	};
*/

	// Translate IO port values
	ULONG AddressSpace = 1;	// IO space
	if( !HalTranslateBusAddress( Isa, 0, dx->PortStartAddress,
					&AddressSpace, &dx->PortStartAddress))
	{
		DebugPrint( "Create file: could not translate IO %x", dx->PortStartAddress.LowPart);
		return STATUS_INVALID_PARAMETER;
	}
	DebugPrint( "IO trans %x,%d", dx->PortStartAddress.LowPart, dx->PortLength);
	dx->PortNeedsMapping = (AddressSpace==0);
	dx->PortInIOSpace = (AddressSpace==1);

	// Translate Memory values
	AddressSpace=0;
	if( !HalTranslateBusAddress( Isa, 0, dx->MemoryStartAddress,
					&AddressSpace, &dx->MemoryStartAddress))
	{
		DebugPrint( "Create file: could not translate Memory %x", dx->MemoryStartAddress.LowPart);
		return STATUS_INVALID_PARAMETER;
	}
	DebugPrint( "Memory trans %x,%d", dx->MemoryStartAddress.LowPart, dx->MemoryLength);
	dx->MemoryNeedsMapping = (AddressSpace==0);
	dx->InMemorySpace = (AddressSpace==1);

	// Translate IRQ values
	if( dx->GotInterrupt)
	{
		ULONG irq = dx->Irql;
		dx->Vector = HalGetInterruptVector( Isa, 0, irq, irq, &dx->Irql, &dx->Affinity);
		if( dx->Vector==NULL)
		{
			DebugPrint( "Create filename: Could not get interrupt vector for IRQ %d", irq);
			return STATUS_INVALID_PARAMETER;
		}
		DebugPrint("Interrupt vector %x IRQL %d Affinity %d Mode %d",
						dx->Vector, dx->Irql, dx->Affinity, dx->Mode);
	}

	// Map io
	if( dx->PortNeedsMapping)
	{
		dx->PortBase = (PUCHAR)MmMapIoSpace( dx->PortStartAddress, dx->PortLength, MmNonCached);
		if( dx->PortBase==NULL)
		{
			DebugPrintMsg( "Cannot map IO port");
			return STATUS_NO_MEMORY;
		}
	}
	else
		dx->PortBase = (PUCHAR)dx->PortStartAddress.LowPart;

	DebugPrint("IO Base %x",dx->PortBase);

	// Map memory
	if( dx->MemoryNeedsMapping)
	{
		dx->MemoryBase = (PUCHAR)MmMapIoSpace( dx->MemoryStartAddress, dx->MemoryLength, MmNonCached);
		if( dx->MemoryBase==NULL)
		{
			DebugPrintMsg( "Cannot map Memory");
			return STATUS_NO_MEMORY;
		}
	}
	else
		dx->MemoryBase = (PUCHAR)dx->MemoryStartAddress.LowPart;

	DebugPrint("Memory Base (Device Virtual) %x",dx->MemoryBase);

	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////
//	FreeResources:	Unmap memory and disconnect from our interrupt

void FreeResources( IN PDEVICE_OBJECT USLddo)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)USLddo->DeviceExtension;

	// Unmap mem
	if (dx->MemoryNeedsMapping)
	{
		DebugPrintMsg( "Unmapping memory");
		MmUnmapIoSpace( (PVOID)dx->MemoryBase, dx->MemoryLength);
		dx->MemoryNeedsMapping = false;
	}

	// Unmap io
	if (dx->PortNeedsMapping)
	{
		DebugPrintMsg( "Unmapping IO");
		MmUnmapIoSpace( (PVOID)dx->PortBase, dx->PortLength);
		dx->PortNeedsMapping = false;
	}

	// Disconnect from interrupt
	if( dx->ConnectedToInterrupt)
	{
		DebugPrintMsg( "Disconnecting from interrupt");
		IoDisconnectInterrupt( dx->InterruptObject);
		dx->ConnectedToInterrupt = false;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	GetResourcesFromFilename:	Extract required resources from filename string
//								All letters in filename must be lower case

NTSTATUS GetResourcesFromFilename( IN PUNICODE_STRING usfilename, IN PUSL_DEVICE_EXTENSION dx)
{
	NTSTATUS status = STATUS_SUCCESS;
	if( usfilename->Length==0)
		return STATUS_INVALID_PARAMETER;

	int chars = usfilename->Length>>1;

	// Check \isa given at start
	int uspos = 0;
	if( !usStrCmpN( usfilename, uspos, chars, L"\\isa", 4))
	{
		DebugPrintMsg( "Create filename does not start with '\\isa'");
		return STATUS_INVALID_PARAMETER;
	}

	/////////////////////////////////////////////////////////////////////////
	// Search for I/O space and IRQ resource strings and Memory space

	while(uspos!=chars)
	{
		/////////////////////////////////////////////////////////////////////
		// Get "\mem<base>,<length>" in hex
		if( usStrCmpN( usfilename, uspos, chars, L"\\mem", 4))
		{
			// Get <base>
			ULONG mem;
			if( !usGetHex( usfilename, uspos, chars, mem))
			{
				DebugPrintMsg( "Create filename '\\mem' not followed by base in hex");
				return STATUS_INVALID_PARAMETER;
			}
			// Get comma
			if( !usStrCmpN( usfilename, uspos, chars, L",", 1))
			{
				DebugPrintMsg( "Create filename '\\mem<base>' not followed by comma");
				return STATUS_INVALID_PARAMETER;
			}
			// Get <length>
			ULONG memlen;
			if( !usGetHex( usfilename, uspos, chars, memlen))
			{
				DebugPrintMsg( "Create filename '\\mem<base>,' not followed by length in hex");
				return STATUS_INVALID_PARAMETER;
			}

			// Save raw values
			DebugPrint( "MEM raw   %xh,%xh", mem, memlen);
			dx->MemoryStartAddress.QuadPart = mem;
			dx->MemoryLength = memlen;
			dx->GotMemory = true;
		}
		/////////////////////////////////////////////////////////////////////
		// Get "\io<base>,<length>" in hex
		if( usStrCmpN( usfilename, uspos, chars, L"\\io", 3))
		{
			// Get <base>
			ULONG io;
			if( !usGetHex( usfilename, uspos, chars, io))
			{
				DebugPrintMsg( "Create filename '\\io' not followed by base in hex");
				return STATUS_INVALID_PARAMETER;
			}
			// Get comma
			if( !usStrCmpN( usfilename, uspos, chars, L",", 1))
			{
				DebugPrintMsg( "Create filename '\\io<base>' not followed by comma");
				return STATUS_INVALID_PARAMETER;
			}
			// Get <length>
			ULONG iolen;
			if( !usGetHex( usfilename, uspos, chars, iolen))
			{
				DebugPrintMsg( "Create filename '\\io<base>,' not followed by length in hex");
				return STATUS_INVALID_PARAMETER;
			}

			// Save raw values
			DebugPrint( "IO raw   %xh,%xh", io, iolen);
			dx->PortStartAddress.QuadPart = io;
			dx->PortLength = iolen;
			dx->GotPort = true;
		}
		/////////////////////////////////////////////////////////////////////
		// Get "\irq<number>" in decimal
		else if( usStrCmpN( usfilename, uspos, chars, L"\\irq", 4))
		{
			// Get <number>
			ULONG irq;
			if( !usGetDec( usfilename, uspos, chars, irq) || irq>15)
			{
				DebugPrintMsg( "Create filename '\\irq' not followed by IRQ number in decimal");
				return STATUS_INVALID_PARAMETER;
			}
			DebugPrint( "IRQ %d", irq);
			dx->Irql = (KIRQL)irq;
			dx->Mode = Latched;
			dx->GotInterrupt = true;
		}
		else if( usStrCmpN( usfilename, uspos, chars, L"\\override", 9))
		{
			DebugPrintMsg("ResourceOverride");
			dx->ResourceOverride = TRUE;
		}
		else
		{
			DebugPrint( "Create filename:  Unrecognised resource %*S",
				chars-uspos, &usfilename->Buffer[uspos]);
			return STATUS_INVALID_PARAMETER;
		}
	}
	return status;
}

/////////////////////////////////////////////////////////////////////////////
//	usStrCmpN:	Compare section of Unicode string with a wide string

bool usStrCmpN( PUNICODE_STRING us, int& pos, int maxchars, PWSTR cmp, int cmpchars)
{
	if( pos+cmpchars>maxchars) return false;
	for( int cmpno=0; cmpno<cmpchars; cmpno++)
		if( us->Buffer[pos+cmpno] != cmp[cmpno])
			return false;
	pos += cmpchars;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	usGetHex:	Get hex value from section of Unicode string

bool usGetHex( PUNICODE_STRING us, int& pos, int maxchars, ULONG& value)
{
	if( pos==maxchars) return false;
	value = 0;
	while( pos<maxchars)
	{
		wchar_t ch = us->Buffer[pos];
		ULONG ThisDigit = 0;
		if( ch>=L'0' && ch<=L'9')
			ThisDigit = ch-L'0';
		else if( ch>=L'a' && ch<=L'f')
			ThisDigit = ch-L'a'+10;
		else
			break;
		value = (value<<4)+ThisDigit;
		pos++;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	usGetDec:	Get decimal value from section of Unicode string

bool usGetDec( PUNICODE_STRING us, int& pos, int maxchars, ULONG& value)
{
	if( pos==maxchars) return false;
	value = 0;
	while( pos<maxchars)
	{
		wchar_t ch = us->Buffer[pos];
		ULONG ThisDigit = 0;
		if( ch>=L'0' && ch<=L'9')
			ThisDigit = ch-L'0';
		else
			break;
		value = (value*10)+ThisDigit;
		pos++;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	USLClose:
//
//	Description:
//		Handle IRP_MJ_CLOSE requests
//		Allow closes to complete if device not started
//
//	Arguments:
//		Pointer to our PHD DO
//		Pointer to the IRP
//
//	Return Value:
//		This function returns STATUS_XXX

NTSTATUS USLClose(IN PDEVICE_OBJECT uslddo,
					IN PIRP Irp)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)uslddo->DeviceExtension;
	DebugPrintMsg("Close");

	FreeBscanArray(dx);
	FreeTimeSlot(dx);
	FreeSi6UserStruct(dx);
	FreeServoUserStruct(dx);
	FreeResources(uslddo);

	UnclaimResources(uslddo);

	// Complete successfully
	return CompleteIrp(Irp,STATUS_SUCCESS);
}
 
/////////////////////////////////////////////////////////////////////////////
//	USLRead:
//
//	Description:
//		Handle IRP_MJ_READ requests
//
//	Arguments:
//		Pointer to our PHD DO
//		Pointer to the IRP
//			IrpStack->Parameters.Read.xxx has read parameters
//			User buffer at:	AssociatedIrp.SystemBuffer	(buffered I/O)
//							MdlAddress					(direct I/O)
//
//	Return Value:
//		This function returns STATUS_XXX
NTSTATUS USLRead(	IN PDEVICE_OBJECT USLddo,
					IN PIRP Irp)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)USLddo->DeviceExtension;
	if( !dx->GotPort)
		return CompleteIrp( Irp, STATUS_INSUFFICIENT_RESOURCES);
	if (!LockDevice(dx))
		return CompleteIrp( Irp, STATUS_DELETE_PENDING);

	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	ULONG BytesTxd = 0;

	// Get call parameters
	LONGLONG FilePointer = IrpStack->Parameters.Read.ByteOffset.QuadPart;
	ULONG ReadLen = IrpStack->Parameters.Read.Length;
	DebugPrint("Read %d bytes from file pointer %d",(int)ReadLen,(int)FilePointer);

	// Check file pointer
	NTSTATUS status;
	if( FilePointer!=0)
		status = STATUS_INVALID_PARAMETER;
	else
	{
		IoMarkIrpPending(Irp);
		IoStartPacket( USLddo, Irp, 0, USLCancelIrp);
		return STATUS_PENDING;
	}
	DebugPrint("Read: %d bytes returned",(int)BytesTxd);

	// Complete IRP
	CompleteIrp(Irp,status,BytesTxd);
	UnlockDevice(dx);
	return status;
}

/////////////////////////////////////////////////////////////////////////////
//	USLWrite:
//
//	Description:
//		Handle IRP_MJ_WRITE requests
//
//	Arguments:
//		Pointer to our PHD DO
//		Pointer to the IRP
//			IrpStack->Parameters.Write.xxx has write parameters
//			User buffer at:	AssociatedIrp.SystemBuffer	(buffered I/O)
//							MdlAddress					(direct I/O)
//
//	Return Value:
//		This function returns STATUS_XXX

NTSTATUS USLWrite(	IN PDEVICE_OBJECT USLddo,
						IN PIRP Irp)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)USLddo->DeviceExtension;
	if( !dx->GotPort)
		return CompleteIrp( Irp, STATUS_INSUFFICIENT_RESOURCES);
	if (!LockDevice(dx))
		return CompleteIrp( Irp, STATUS_DELETE_PENDING);

	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	ULONG BytesTxd = 0;

	// Get call parameters
	LONGLONG FilePointer = IrpStack->Parameters.Write.ByteOffset.QuadPart;
	ULONG WriteLen = IrpStack->Parameters.Write.Length;
	DebugPrint("Write %d bytes from file pointer %d",(int)WriteLen,(int)FilePointer);

	NTSTATUS status;
	if( FilePointer!=0)
		status = STATUS_INVALID_PARAMETER;
	else
	{
		IoMarkIrpPending(Irp);
		IoStartPacket( USLddo, Irp, 0, USLCancelIrp);
		return STATUS_PENDING;
	}

	DebugPrint("Write: %d bytes written",(int)BytesTxd);

	// Complete IRP
	CompleteIrp(Irp,status,BytesTxd);
	UnlockDevice(dx);
	return status;
}

/////////////////////////////////////////////////////////////////////////////
//	USLDeviceControl:
//
//	Description:
//		Handle IRP_MJ_DEVICE_CONTROL requests
//
//	Arguments:
//		Pointer to our PHD DO
//		Pointer to the IRP
//			Buffered:	AssociatedIrp.SystemBuffer (and IrpStack->Parameters.DeviceIoControl.Type3InputBuffer)
//			Direct:		MdlAddress
//
//			IrpStack->Parameters.DeviceIoControl.InputBufferLength
//			IrpStack->Parameters.DeviceIoControl.OutputBufferLength 
//
//	Return Value:
//		This function returns STATUS_XXX

NTSTATUS USLDeviceControl(	IN PDEVICE_OBJECT uslddo,
								IN PIRP Irp)
{
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)uslddo->DeviceExtension;
	int	ii;

	if( !dx->GotPort)
		return CompleteIrp( Irp, STATUS_INSUFFICIENT_RESOURCES);
//	if( dx->IODisabled)
//		return CompleteIrp( Irp, STATUS_DEVICE_NOT_CONNECTED);
	if (!LockDevice(dx))
		return CompleteIrp( Irp, STATUS_DELETE_PENDING);

	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	NTSTATUS status = STATUS_SUCCESS;
	ULONG BytesTxd = 0;

	ULONG ControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
	ULONG InputLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG OutputLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	DebugPrint("DeviceIoControl: Control code %x InputLength %d OutputLength %d",
				ControlCode, InputLength, OutputLength);

	// Get access to the shared buffer
	char *Dummy;
	char Addrs[4];
//	struct	USLData	usl;
//	Dummy=(char *)(&usl);

	switch( ControlCode)
	{
	///////	Pass to StartIo
	case IOCTL_USL_RUN_CMDS:
	case IOCTL_USL_CMDS_FOR_READ:
	case IOCTL_USL_CMDS_FOR_READ_START:
	case IOCTL_USL_CMDS_FOR_WRITE:
	case IOCTL_USL_GET_RW_RESULTS:
//		DebugPrintMsg("DeviceIoControl: StartIo called");
		IoMarkIrpPending(Irp);
		IoStartPacket( uslddo, Irp, 0, USLCancelIrp);
		return STATUS_PENDING;
		break;
	case IOCTL_USL_GET_VERSION:
		if( (sizeof Version)>OutputLength)
			status = STATUS_INVALID_PARAMETER;
		else
		{
			strcpy(Version,USL_VERSION);
			BytesTxd = sizeof Version;
			RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,Version,BytesTxd);
		}
		break;
	case IOCTL_USL_SHARE_BSCAN_ARRAY:
		if((InputLength!=0)||(dx->pTimeSlotUser==NULL)) {
			status = STATUS_INVALID_PARAMETER;
		}
		else
		{
			FreeBscanArray(dx);
			DebugPrint("Virtual Bscan Array Address = %x",dx->pTimeSlotUser->Adc.pBscanArray);
			dx->mdlBscanArray = IoAllocateMdl(dx->pTimeSlotUser->Adc.pBscanArray,dx->pTimeSlotUser->Adc.nBscanArraySize,FALSE,FALSE,NULL);
			if(dx->mdlBscanArray!=NULL) {
				MmProbeAndLockPages(dx->mdlBscanArray,KernelMode,IoModifyAccess);
				dx->pBscanArray=(char *)MmGetSystemAddressForMdl(dx->mdlBscanArray);
				DebugPrint("system Addrs = %x %d",dx->pBscanArray,dx->pTimeSlotUser->Adc.nBscanArraySize);
			};
		};
		break;
	case IOCTL_USL_FREE_BSCAN_ARRAY:	FreeBscanArray(dx);
		break;
	case IOCTL_USL_SHARE_TIMESLOT_STRUCT:
		if(InputLength!=0) {
			status = STATUS_INVALID_PARAMETER;
		}
		else
		{
			FreeTimeSlot(dx);
			dx->pVirtualTimeSlot=(struct TimeSlotData *)IrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
			DebugPrint("Virtual Tiemslot Address = %x",dx->pVirtualTimeSlot);
			
			dx->mdlTimeSlot = IoAllocateMdl(dx->pVirtualTimeSlot,(sizeof (struct TimeSlotData))*8,FALSE,FALSE,NULL);
			if(dx->mdlTimeSlot) {
				MmProbeAndLockPages(dx->mdlTimeSlot,KernelMode,IoModifyAccess);
				dx->pTimeSlotUser=(struct TimeSlotData *)MmGetSystemAddressForMdl(dx->mdlTimeSlot);
				DebugPrint("system Addrs = %x",dx->pTimeSlotUser);
			};
		};

		break;
	case IOCTL_USL_FREE_TIMESLOT_STRUCT:	FreeTimeSlot(dx);
		break;
	case IOCTL_USL_READ_ADC200_MEMORY:
		if(OutputLength<=0)
			status = STATUS_INVALID_PARAMETER;
		else
		{
			BytesTxd = OutputLength;
			switch(dx->pTimeSlotUser[0].Adc.nCardType) {
			case USL_ADC200:
				RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,dx->MemoryBase,BytesTxd);
				break;
			case USL_ADC100:
				if(dx->pTimeSlotUser[0].Adc.bBscanCollectData!=TRUE) {
					RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,dx->MemoryBase,BytesTxd);
				} else {
					RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,Trace,BytesTxd);
				}
				break;
			}
		}
		break;
	case IOCTL_USL_SHARE_SI6_USER_STRUCT:
		if(InputLength!=0)
			status = STATUS_INVALID_PARAMETER;
		else
		{
			FreeSi6UserStruct(dx);
			dx->pVirtualSi6UserStruct=(struct SI6Data *)IrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
			DebugPrint("Virtual Si6 Address = %x",dx->pVirtualSi6UserStruct);
			
			dx->si6mdl = IoAllocateMdl(dx->pVirtualSi6UserStruct,sizeof (struct SI6Data),FALSE,FALSE,NULL);
			if(dx->si6mdl) {
				MmProbeAndLockPages(dx->si6mdl,KernelMode,IoModifyAccess);
				dx->pSi6User=(struct SI6Data *)MmGetSystemAddressForMdl(dx->si6mdl);
				DebugPrint("system Si6 Addrs = %x",dx->pSi6User);
				dx->bCollectData=TRUE;
			}
		}
		break;
	case IOCTL_USL_FREE_SI6_USER_STRUCT:	FreeSi6UserStruct(dx);
		break;
	case IOCTL_USL_SHARE_SERVO_USER_STRUCT:
		if(InputLength!=0)
			status = STATUS_INVALID_PARAMETER;
		else
		{
			FreeServoUserStruct(dx);
			dx->pVirtualServoUserStruct=(struct SERVOData *)IrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
			DebugPrint("Virtual Servo Address = %x",dx->pVirtualSi6UserStruct);
			
			dx->servomdl = IoAllocateMdl(dx->pVirtualServoUserStruct,sizeof (struct SERVOData),FALSE,FALSE,NULL);
			if(dx->servomdl) {
				MmProbeAndLockPages(dx->servomdl,KernelMode,IoModifyAccess);
				dx->pServoUser=(struct SERVOData *)MmGetSystemAddressForMdl(dx->servomdl);
				DebugPrint("system Servo Addrs = %x",dx->pServoUser);
				dx->bCollectData=TRUE;
			}
		}
		break;
	case IOCTL_USL_FREE_SERVO_USER_STRUCT:	FreeServoUserStruct(dx);
		break;
	///////	Invalid request
	default:
		status = STATUS_INVALID_DEVICE_REQUEST;
		DebugPrintMsg("DeviceIoControl: invalid request code");
	}


	// Complete IRP
	UnlockDevice(dx);
	return CompleteIrp(Irp,status,BytesTxd);
}

void FreeTimeSlot( IN PUSL_DEVICE_EXTENSION dx)
{
	if(dx->mdlTimeSlot) {
		MmUnlockPages(dx->mdlTimeSlot);
		IoFreeMdl(dx->mdlTimeSlot);
		dx->mdlTimeSlot=NULL;
		dx->pTimeSlotUser=NULL;
		DebugPrintMsg("Free mdlTimeSlot");
	}
}

void FreeSi6UserStruct( IN PUSL_DEVICE_EXTENSION dx)
{
	if(dx->si6mdl) {
		dx->bCollectData=FALSE;
		MmUnlockPages(dx->si6mdl);
		IoFreeMdl(dx->si6mdl);
		dx->si6mdl=NULL;
		dx->pSi6User=NULL;
		DebugPrintMsg("Free Struct Si6Mdl");

	}

}
void FreeServoUserStruct( IN PUSL_DEVICE_EXTENSION dx)
{
	if(dx->servomdl) {
		dx->bCollectData=FALSE;
		MmUnlockPages(dx->servomdl);
		IoFreeMdl(dx->servomdl);
		dx->servomdl=NULL;
		dx->pServoUser=NULL;
		DebugPrintMsg("Free Struct ServoMdl");

	}

}

void FreeBscanArray( IN PUSL_DEVICE_EXTENSION dx)
{
	if(dx->mdlBscanArray) {
		MmUnlockPages(dx->mdlBscanArray);
		IoFreeMdl(dx->mdlBscanArray);
		dx->mdlBscanArray=NULL;
		dx->pBscanArray=NULL;
		DebugPrintMsg("Free Bscan Array Mdl");
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CompleteIrp:	Sets IoStatus and completes the IRP

NTSTATUS CompleteIrp( IN PIRP Irp, IN NTSTATUS status, IN ULONG info/*=0*/, IN CCHAR PriorityBoost/*=IO_NO_INCREMENT*/)
{
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = info;
	IoCompleteRequest(Irp,PriorityBoost);
	return status;
}

