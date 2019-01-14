//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
//	SERVO example
/////////////////////////////////////////////////////////////////////////////
//	dispatch.cpp:		I/O IRP handlers
/////////////////////////////////////////////////////////////////////////////
//	SERVOCreate			Handle Create/Open file IRP
//*	ClaimResources		Ask kernel to give is the resources we want
//*	UnclaimResources	Release our resources back to the system
//*	TranslateAndMapResources	Translate and map our resources
//*	FreeResources		Unmap memory and disconnect from our interrupt
//*	GetResourcesFromFilename	Extract required resources from filename string
//*	usStrCmpN			Compare section of Unicode string with a wide string
//*	usGetHex			Get hex value from section of Unicode string
//*	usGetDec			Get decimal value from section of Unicode string
//	SERVOClose			Handle Close file IRPs
//	SERVORead			Handle Read IRPs
//	SERVOWrite			Handle Write IRPs
//	SERVODeviceControl	Handle DeviceIoControl IRPs
//	SERVOSystemControl	Handle WMI IRPs
/////////////////////////////////////////////////////////////////////////////

#include "ServoDriver.h"
#include "ServoIoctl.h"
#include "C:\WindowsProjects\USLScanner\Si6Driver\sys\Si6Ioctl.h"
#include "C:\WindowsProjects\USLScanner\USLDriver\sys\USLIoctl.h"
#include "windef.h"

KSPIN_LOCK BufferLock;
PUCHAR	Buffer = NULL;
ULONG	BufferSize = 0;
char	Version[40];			//={"Servo V1.00 September24th 2002"};
int		nTable[10001];

/////////////////////////////////////////////////////////////////////////////

NTSTATUS GetResourcesFromFilename( IN PUNICODE_STRING usfilename, IN PSERVO_DEVICE_EXTENSION dx);
bool usStrCmpN( PUNICODE_STRING us, int& pos, int maxchars, PWSTR cmp, int cmpchars);
bool usGetHex( PUNICODE_STRING us, int& pos, int maxchars, ULONG& value);
bool usGetDec( PUNICODE_STRING us, int& pos, int maxchars, ULONG& value);

NTSTATUS ClaimResources( IN PDEVICE_OBJECT servoddo);
NTSTATUS TranslateAndMapResources( IN PDEVICE_OBJECT servoddo);
void UnclaimResources( IN PDEVICE_OBJECT servoddo);
void FreeUserStruct( IN PSERVO_DEVICE_EXTENSION dx);
void FreeTimeSlot( IN PSERVO_DEVICE_EXTENSION dx);
void FreeDataArray( IN PSERVO_DEVICE_EXTENSION dx);
void DeRegisterEvent( IN PSERVO_DEVICE_EXTENSION dx);

NTSTATUS PowerUpDevice( IN PDEVICE_OBJECT servoddo);
NTSTATUS FindParallelPort();
NTSTATUS  AbcConfigCallback(
	IN PVOID Context, IN PUNICODE_STRING PathName,
	IN INTERFACE_TYPE BusType, IN ULONG BusNumber, IN PKEY_VALUE_FULL_INFORMATION *BusInfo,
	IN CONFIGURATION_TYPE CtrlrType, IN ULONG CtrlrNumber, IN PKEY_VALUE_FULL_INFORMATION *CtrlrInfo,
	IN CONFIGURATION_TYPE PeripheralType, IN ULONG PeripheralNumber, IN PKEY_VALUE_FULL_INFORMATION *PeripheralInfo
	);
/////////////////////////////////////////////////////////////////////////////
//	SERVOCreate:
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

NTSTATUS SERVOCreate(	IN PDEVICE_OBJECT servoddo,
						IN PIRP Irp)
{
	int	ii;
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)servoddo->DeviceExtension;
	PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	DebugPrint( "Create File is %T", &(IrpStack->FileObject->FileName));

	dx->GotPort = false;
	dx->GotInterrupt = false;
	dx->PortNeedsMapping = false;
	dx->GotMemory = false;
	dx->MemoryNeedsMapping = false;
	dx->ConnectedToInterrupt = false;
	dx->ResourceOverride = FALSE;

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
	status = ClaimResources(servoddo);
	if( !NT_SUCCESS(status))
	{
		DebugPrintMsg("Could not ClaimResources");
		goto fail;
	}

	// Translate and map resources
	status = TranslateAndMapResources(servoddo);
	if( !NT_SUCCESS(status))
	{
		UnclaimResources(servoddo);
		goto fail;
	}


	// Complete
	return CompleteIrp(Irp,status);

	// On error, make sure everything's off
fail:
	dx->GotPort = false;
	dx->GotMemory = false;
	dx->GotInterrupt = false;
	dx->PortNeedsMapping = false;
	dx->ConnectedToInterrupt = false;
	return CompleteIrp(Irp,status);
}

/////////////////////////////////////////////////////////////////////////////
//	ClaimResources:	Ask kernel to give is the resources we want

NTSTATUS ClaimResources( IN PDEVICE_OBJECT servoddo)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)servoddo->DeviceExtension;

	// Get resource count: either 1 (IOport) or 2 (IOport&IRQ)
	ULONG PartialResourceCount = 1;
	if( dx->GotInterrupt) PartialResourceCount++;

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
	DebugPrint("servoddo->DriverObject %x",servoddo->DriverObject);
	if( dx->ResourceOverride) {
		DebugPrintMsg("Resource override conflict");
	}
	BOOLEAN ConflictDetected;
	NTSTATUS status = IoReportResourceUsage( NULL,
						servoddo->DriverObject, ResourceList, ListSize,	// Driver resources
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

void UnclaimResources( IN PDEVICE_OBJECT servoddo)
{
	DebugPrintMsg("Freeing all allocated resources");
	// Release all driver's resources by declaring we have none.
	CM_RESOURCE_LIST ResourceList;
	ResourceList.Count = 0;
	BOOLEAN ConflictDetected;
	IoReportResourceUsage( NULL,
					servoddo->DriverObject, &ResourceList, sizeof(ResourceList),	// Driver resources
					NULL, NULL, 0,	// Device resources
					FALSE, &ConflictDetected);
	// ignore return result
}

/////////////////////////////////////////////////////////////////////////////
//	TranslateAndMapResources:	Translate and map our resources
NTSTATUS TranslateAndMapResources( IN PDEVICE_OBJECT servoddo)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)servoddo->DeviceExtension;

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

void FreeResources( IN PDEVICE_OBJECT SERVOddo)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)SERVOddo->DeviceExtension;

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

NTSTATUS GetResourcesFromFilename( IN PUNICODE_STRING usfilename, IN PSERVO_DEVICE_EXTENSION dx)
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
//	SERVOClose:
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

NTSTATUS SERVOClose(IN PDEVICE_OBJECT servoddo,
					IN PIRP Irp)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)servoddo->DeviceExtension;
	DebugPrintMsg("Close");

	FreeDataArray(dx);
	FreeUserStruct(dx);
	FreeTimeSlot(dx);
	FreeResources(servoddo);
	DeRegisterEvent(dx);


	UnclaimResources(servoddo);

	// Complete successfully
	return CompleteIrp(Irp,STATUS_SUCCESS);
}
 
void DeRegisterEvent( PSERVO_DEVICE_EXTENSION dx)
{

	if (dx->pIntEvent)
	{						// event already registered
		ObDereferenceObject(dx->pIntEvent);
		dx->pIntEvent = NULL;
	}

}
/////////////////////////////////////////////////////////////////////////////
//	SERVORead:
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

NTSTATUS SERVORead(	IN PDEVICE_OBJECT SERVOddo,
					IN PIRP Irp)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)SERVOddo->DeviceExtension;
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
		IoStartPacket( SERVOddo, Irp, 0, SERVOCancelIrp);
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

NTSTATUS SERVOWrite(	IN PDEVICE_OBJECT SERVOddo,
						IN PIRP Irp)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)SERVOddo->DeviceExtension;
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
		IoStartPacket( SERVOddo, Irp, 0, SERVOCancelIrp);
		return STATUS_PENDING;
	}

	DebugPrint("Write: %d bytes written",(int)BytesTxd);

	// Complete IRP
	CompleteIrp(Irp,status,BytesTxd);
	UnlockDevice(dx);
	return status;
}
/////////////////////////////////////////////////////////////////////////////
//	SERVODeviceControl:
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

NTSTATUS SERVODeviceControl(	IN PDEVICE_OBJECT servoddo,
								IN PIRP Irp)
{
	PSERVO_DEVICE_EXTENSION dx = (PSERVO_DEVICE_EXTENSION)servoddo->DeviceExtension;
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

	switch( ControlCode)
	{
	///////	Pass to StartIo
	case IOCTL_SERVO_RUN_CMDS:
	case IOCTL_SERVO_CMDS_FOR_READ:
	case IOCTL_SERVO_CMDS_FOR_READ_START:
	case IOCTL_SERVO_CMDS_FOR_WRITE:
	case IOCTL_SERVO_GET_RW_RESULTS:
//		DebugPrintMsg("DeviceIoControl: StartIo called");
		IoMarkIrpPending(Irp);
		IoStartPacket( servoddo, Irp, 0, SERVOCancelIrp);
		return STATUS_PENDING;
		break;
	case IOCTL_SERVO_GET_VERSION:
		if( (sizeof Version)>OutputLength)
			status = STATUS_INVALID_PARAMETER;
		else
		{
			strcpy(Version,SERVO_VERSION);
			BytesTxd = sizeof Version;
			RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,Version,BytesTxd);
		}
		break;
/*
	case IOCTL_SERVO_GET_PTR_SERVO:
		if( (sizeof (struct SERVOData))>OutputLength)
			status = STATUS_INVALID_PARAMETER;
		else
		{
			Addrs[0]=(int)Dummy & 0xff;
			Addrs[1]=((int)Dummy>>8) & 0xff;
			Addrs[2]=((int)Dummy>>16) & 0xff;
			Addrs[3]=((int)Dummy>>24) & 0xff;
			BytesTxd = sizeof (&servo);
			RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,Addrs,BytesTxd);
		}
		break;
	case IOCTL_SERVO_DRIVER_SERVOSTRUCT_TO_USER:
		if( (sizeof )>OutputLength)
			status = STATUS_INVALID_PARAMETER;
		else
		{
			BytesTxd = sizeof servo;
			RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,Dummy,BytesTxd);
		}
		break;
	case IOCTL_SERVO_USER_SERVOSTRUCT_TO_DRIVER:
		if( (sizeof servo)<InputLength)
			status = STATUS_INVALID_PARAMETER;
		else
		{
			RtlCopyMemory(&servo,Irp->AssociatedIrp.SystemBuffer,InputLength);
		}
		break;
	case IOCTL_SERVO_SEND_PTR_USER_SERVO:
		if(InputLength!=0)
			status = STATUS_INVALID_PARAMETER;
		else
		{
//			pservo=(struct SERVOData *)IrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
//			pservo->nEcho=0x123;
		}
		break;
*/
	case IOCTL_SERVO_SHARE_USER_STRUCT:
		if(InputLength!=0)
			status = STATUS_INVALID_PARAMETER;
		else
		{
			FreeUserStruct(dx);
			dx->pVirtualUserStruct=(struct SERVOData *)IrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
			DebugPrint("Virtual Address = %x",dx->pVirtualUserStruct);
			
			dx->mdlServo = IoAllocateMdl(dx->pVirtualUserStruct,sizeof (struct SERVOData),FALSE,FALSE,NULL);
			if(dx->mdlServo) {
				MmProbeAndLockPages(dx->mdlServo,KernelMode,IoModifyAccess);
				dx->pServoUser=(struct SERVOData *)MmGetSystemAddressForMdl(dx->mdlServo);
				DebugPrint("system Addrs = %x",dx->pServoUser);
				int ii;
				for(ii=0;ii<16;ii++) {
					DebugPrint("Axis Addrs %d = %x",ii+1,dx->pServoUser->pAxis[ii]);
					dx->mdlAxis[ii] = IoAllocateMdl(dx->pServoUser->pAxis[ii],sizeof (struct AxisData),FALSE,FALSE,NULL);
					if(dx->mdlAxis[ii]) {
						MmProbeAndLockPages(dx->mdlAxis[ii],KernelMode,IoModifyAccess);
						dx->pAxis[ii]=(struct AxisData *)MmGetSystemAddressForMdl(dx->mdlAxis[ii]);
						DebugPrint("system Axis Addrs = %x",dx->pAxis[ii]);
					}
				}
			}
		}
		break;
	case IOCTL_SERVO_FREE_USER_STRUCT:	FreeUserStruct(dx);
		break;
	case IOCTL_SERVO_SHARE_DATA_ARRAY:
		if((InputLength!=0)||(dx->pServoUser->pData==NULL)) {
			status = STATUS_INVALID_PARAMETER;
		}
		else
		{
			FreeDataArray(dx);
			DebugPrint("Virtual Data Array Address = %x",dx->pServoUser->pData);
			dx->mdlDataArray = IoAllocateMdl(dx->pServoUser->pData,dx->pServoUser->nDataSize,FALSE,FALSE,NULL);
			if(dx->mdlDataArray!=NULL) {
				MmProbeAndLockPages(dx->mdlDataArray,KernelMode,IoModifyAccess);
				dx->pDataArray=(char *)MmGetSystemAddressForMdl(dx->mdlDataArray);
				DebugPrint("system Addrs = %x %d",dx->pDataArray,dx->pServoUser->nDataSize);
			};
		};
		break;
	case IOCTL_SERVO_FREE_DATA_ARRAY:	FreeDataArray(dx);
		break;
	case IOCTL_SERVO_SHARE_TIMESLOT_STRUCT:
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
	case IOCTL_SERVO_FREE_TIMESLOT_STRUCT:	FreeTimeSlot(dx);
		break;
	case IOCTL_SERVO_GET_IRQ_COUNT:
		if( OutputLength<4)
			status = STATUS_INVALID_PARAMETER;
		else
		{
			char Addrs[4];
			Addrs[0]=(int)dx->nIrqCount & 0xff;
			Addrs[1]=((int)dx->nIrqCount>>8) & 0xff;
			Addrs[2]=((int)dx->nIrqCount>>16) & 0xff;
			Addrs[3]=((int)dx->nIrqCount>>24) & 0xff;
			BytesTxd = 4;
			RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,Addrs,BytesTxd);
		}
		break;
	case IOCTL_SERVO_ENABLE_INTERRUPT_CALLBACK:

		if (InputLength < sizeof(HANDLE))
		{
			DebugPrint("IOCTL_SI10_ENABLE_INTERRUPT_CALLBACK - STATUS_INVALID_PARAMETER");
			break;
		}
		
		if (dx->pIntEvent)
		{						// event already registered
			ObDereferenceObject(dx->pIntEvent);
			dx->pIntEvent = NULL;
		}						// event already registered

		status = ObReferenceObjectByHandle(* (PHANDLE) IrpStack->Parameters.DeviceIoControl.Type3InputBuffer, EVENT_MODIFY_STATE, *ExEventObjectType, Irp->RequestorMode, (PVOID*) &dx->pIntEvent, NULL);

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



void FreeUserStruct( IN PSERVO_DEVICE_EXTENSION dx)
{
	int ii;
	for(ii=0;ii<16;ii++) {
		if(dx->mdlAxis[ii]) {
			MmUnlockPages(dx->mdlAxis[ii]);
			IoFreeMdl(dx->mdlAxis[ii]);
			dx->mdlAxis[ii]=NULL;
			dx->pAxis[ii]=NULL;
			DebugPrint("Free AxisMdl%d",ii);
		}
	}
	if(dx->mdlServo) {
		MmUnlockPages(dx->mdlServo);
		IoFreeMdl(dx->mdlServo);
		dx->mdlServo=NULL;
		dx->pServoUser=NULL;
		DebugPrintMsg("Free Struct Mdl");

	}

}
void FreeTimeSlot( IN PSERVO_DEVICE_EXTENSION dx)
{
	if(dx->mdlTimeSlot) {
		MmUnlockPages(dx->mdlTimeSlot);
		IoFreeMdl(dx->mdlTimeSlot);
		dx->mdlTimeSlot=NULL;
		dx->pTimeSlotUser=NULL;
		DebugPrintMsg("Free mdlTimeSlot");
	}
}

void FreeDataArray( IN PSERVO_DEVICE_EXTENSION dx)
{
	if(dx->mdlDataArray) {
		MmUnlockPages(dx->mdlDataArray);
		IoFreeMdl(dx->mdlDataArray);
		dx->mdlDataArray=NULL;
		dx->pDataArray=NULL;
		DebugPrintMsg("Free Data Array Mdl");
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

/////////////////////////////////////////////////////////////////////////////
NTSTATUS  AbcConfigCallback(
	IN PVOID Context, IN PUNICODE_STRING PathName,
	IN INTERFACE_TYPE BusType, IN ULONG BusNumber, IN PKEY_VALUE_FULL_INFORMATION *BusInfo,
	IN CONFIGURATION_TYPE CtrlrType, IN ULONG CtrlrNumber, IN PKEY_VALUE_FULL_INFORMATION *CtrlrInfo,
	IN CONFIGURATION_TYPE PeripheralType, IN ULONG PeripheralNumber, IN PKEY_VALUE_FULL_INFORMATION *PeripheralInfo
	)
{
	DebugPrint( "ConfigCallback: Bus: %d,%d",BusType,BusNumber);
	DebugPrint( "ConfigCallback: Controller: %d,%d",CtrlrType,CtrlrNumber);
	DebugPrint( "ConfigCallback: Peripheral: %d,%d",PeripheralType,PeripheralNumber);

	if( CtrlrInfo!=NULL)
	{
		PCM_FULL_RESOURCE_DESCRIPTOR frd = (PCM_FULL_RESOURCE_DESCRIPTOR)
			(((PUCHAR)CtrlrInfo[IoQueryDeviceConfigurationData])
			+CtrlrInfo[IoQueryDeviceConfigurationData]->DataOffset);
		for( ULONG i=0; i<frd->PartialResourceList.Count; i++)
		{
			PCM_PARTIAL_RESOURCE_DESCRIPTOR resource = &frd->PartialResourceList.PartialDescriptors[i];
			switch( resource->Type)
			{
			case CmResourceTypePort:
				DebugPrint( "ConfigCallback: I/O port %x,%d",
						resource->u.Port.Start.LowPart, resource->u.Port.Length);
				break;
			case CmResourceTypeInterrupt:
				DebugPrint( "ConfigCallback: Interrupt level %d vector %d", resource->u.Interrupt.Level, resource->u.Interrupt.Vector);
				break;
			default:
				DebugPrint( "ConfigCallback: Resource type %d",resource->Type);
			}
		}
	}
	return STATUS_SUCCESS;
}

NTSTATUS FindParallelPort()
{
	NTSTATUS status;
	for( int BusType=0; BusType<MaximumInterfaceType; BusType++)
	{
		INTERFACE_TYPE iBusType = (INTERFACE_TYPE)BusType;
		CONFIGURATION_TYPE iCtrlType;
		ULONG iBusNumber,BusNumber = 0;

		for(BusNumber=0;BusNumber<8;BusNumber++) 
		{
			// See if this bus instance exists
			iBusNumber=BusNumber;
			status = IoQueryDeviceDescription(
					&iBusType, &iBusNumber,
					NULL, NULL,
					NULL, NULL,
					AbcConfigCallback, NULL);
			if( !NT_SUCCESS(status))
			{
				if( status != STATUS_OBJECT_NAME_NOT_FOUND)
					return status;
				break;
			}

			// See what printers exist on this bus instance
			for(int CtrlType=0;CtrlType<MaximumType;CtrlType++) {
				iCtrlType=(CONFIGURATION_TYPE)CtrlType;
				iBusNumber=BusNumber;
				status = IoQueryDeviceDescription(
						&iBusType, &iBusNumber,
						&iCtrlType, NULL,
						NULL, NULL,
						AbcConfigCallback, NULL);
//				if( !NT_SUCCESS(status) && (status != STATUS_OBJECT_NAME_NOT_FOUND)) {
//					return status;
//				}
			}
			BusNumber++;
		}
	}
	return status;
}
