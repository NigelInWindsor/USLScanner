//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
//	USL example
/////////////////////////////////////////////////////////////////////////////
//	init.cpp:		Driver initialization code
/////////////////////////////////////////////////////////////////////////////
//	DriverEntry		Initialisation entry point
//	USLCreateDevice
//	USLUnload		Unload driver routine
//	USLDeleteDevice
//	LockDevice
//	UnlockDevice
/////////////////////////////////////////////////////////////////////////////

#include "USLDriver.h"

/////////////////////////////////////////////////////////////////////////////

PDEVICE_OBJECT uslddo = NULL;

NTSTATUS USLCreateDevice( IN PDRIVER_OBJECT DriverObject);
void USLDeleteDevice();

/////////////////////////////////////////////////////////////////////////////

#pragma code_seg("INIT") // start INIT section

/////////////////////////////////////////////////////////////////////////////
//	DriverEntry:
//
//	Description:
//		This function initializes the driver, and creates
//		any objects needed to process I/O requests.
//
//	Arguments:
//		Pointer to the Driver object
//		Registry path string for driver service key
//
//	Return Value:
//		This function returns STATUS_XXX

extern "C"
NTSTATUS DriverEntry(	IN PDRIVER_OBJECT DriverObject,
						IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS status = STATUS_SUCCESS;

#if DBG
	DebugPrintInit("USL checked");
#else
	DebugPrintInit("USL free");
#endif

	DebugPrint("RegistryPath is %T",RegistryPath);

	// Export other driver entry points...
// 	DriverObject->DriverExtension->AddDevice = USLAddDevice;
 	DriverObject->DriverStartIo = USLStartIo;
 	DriverObject->DriverUnload = USLUnload;

	DriverObject->MajorFunction[IRP_MJ_CREATE] = USLCreate;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = USLClose;

//	DriverObject->MajorFunction[IRP_MJ_PNP] = USLPnp;		No PnP handler...
//	DriverObject->MajorFunction[IRP_MJ_POWER] = USLPower;

	DriverObject->MajorFunction[IRP_MJ_READ] = USLRead;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = USLWrite;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = USLDeviceControl;

//	DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = USLSystemControl;

	DriverObject->MajorFunction[IRP_MJ_CLEANUP] = USLDispatchCleanup;

	status = USLCreateDevice(DriverObject);

	DebugPrintMsg("DriverEntry completed");

	return status;
}

//////////////////////////////////////////////////////////////////////////////

#define	NT_DEVICE_NAME	L"\\Device\\USL"
#define	SYM_LINK_NAME	L"\\DosDevices\\USL"

NTSTATUS USLCreateDevice( IN PDRIVER_OBJECT DriverObject)
{
	NTSTATUS status = STATUS_SUCCESS;

	// Initialise NT and Symbolic link names
	UNICODE_STRING deviceName, linkName;
	RtlInitUnicodeString( &deviceName, NT_DEVICE_NAME);
	RtlInitUnicodeString( &linkName, SYM_LINK_NAME);

	// Create our device
	DebugPrint("Creating device %T",&deviceName);
	status = IoCreateDevice(
				DriverObject,
				sizeof(USL_DEVICE_EXTENSION),
				&deviceName,
				FILE_DEVICE_UNKNOWN,
				0,
				TRUE,	// Exclusive
				&uslddo);
	if( !NT_SUCCESS(status))
	{
		DebugPrintMsg("Could not create device");
		return status;
	}

	uslddo->Flags |= DO_BUFFERED_IO;
	uslddo->Flags |= DO_DIRECT_IO;

	// Initialise device extension
	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)uslddo->DeviceExtension;
	dx->uslddo = uslddo;
	dx->UsageCount = 1;
	KeInitializeEvent( &dx->StoppingEvent, NotificationEvent, FALSE);
	dx->Stopping = false;
	dx->GotPort = false;
	dx->GotMemory = false;
	dx->GotInterrupt = false;
	dx->ConnectedToInterrupt = false;
	dx->SetTimeout = 10;
	dx->Timeout = -1;
	dx->StopTimer = false;
	dx->WriteCmds = NULL;
	dx->ReadCmds = NULL;
	dx->StartReadCmds = NULL;

	// Initialise "connect to interrupt" work queue item
	ExInitializeWorkItem( &dx->ConnectIntWQI, IrqConnectRoutine, dx);
	dx->ConnectIntQueued = false;

	// Initialise timer for this device (but do not start)
	status = IoInitializeTimer( uslddo, (PIO_TIMER_ROUTINE)Timeout1s, dx);
	if( !NT_SUCCESS(status))
	{
		DebugPrintMsg("Could not initialise timer");
		IoDeleteDevice(uslddo);
		return status;
	}

	// Create a symbolic link so our device is visible to Win32...
	DebugPrint("Creating symbolic link %T",&linkName);
	status = IoCreateSymbolicLink( &linkName, &deviceName);
	if( !NT_SUCCESS(status)) 
	{
		DebugPrintMsg("Could not create symbolic link");
		IoDeleteDevice(uslddo);
		return status;
	}
	
	// Initialise our DPC for IRQ completion processing
	IoInitializeDpcRequest( uslddo, USLDpcForIsr);

	return status;
}

#pragma code_seg() // end INIT section

//////////////////////////////////////////////////////////////////////////////
//	USLUnload
//
//	Description:
//		Unload the driver by removing any remaining objects, etc.
//
//	Arguments:
//		Pointer to the Driver object
//
//	Return Value:
//		None

#pragma code_seg("PAGE") // start PAGE section

VOID USLUnload(IN PDRIVER_OBJECT DriverObject)
{
	DebugPrintMsg("USLUnload");
	USLDeleteDevice();
	DebugPrintClose();
}

//////////////////////////////////////////////////////////////////////////////

void USLDeleteDevice()
{
	if( uslddo==NULL) return;

	PUSL_DEVICE_EXTENSION dx = (PUSL_DEVICE_EXTENSION)uslddo->DeviceExtension;
	StopDevice(dx);

	// Stop timer (not sure if this is necessary)
	if( dx->StopTimer)
		IoStopTimer(uslddo);

	// Remove any stored command buffers
	FreeIfAllocated(dx->WriteCmds);
	FreeIfAllocated(dx->StartReadCmds);
	FreeIfAllocated(dx->ReadCmds);

	// Initialise Symbolic link name
	UNICODE_STRING linkName;
	RtlInitUnicodeString( &linkName, SYM_LINK_NAME);

	// Remove symbolic link
	DebugPrint("Deleting symbolic link %T",&linkName);
	IoDeleteSymbolicLink( &linkName);

	// Delete device
	DebugPrintMsg("Deleting uslddo device object");
	IoDeleteDevice(uslddo);
}

//////////////////////////////////////////////////////////////////////////////
#pragma code_seg() // end PAGE section

/////////////////////////////////////////////////////////////////////////////
//	LockDevice:	Lock out PnP remove request

bool LockDevice( IN PUSL_DEVICE_EXTENSION dx)
{
//	DebugPrintMsg("LockDevice");
	InterlockedIncrement(&dx->UsageCount);

	if( dx->Stopping)
	{
		if( InterlockedDecrement(&dx->UsageCount)==0)
			KeSetEvent( &dx->StoppingEvent, 0, FALSE);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	UnlockDevice:	Unlock device allow PnP remove request

void UnlockDevice( IN PUSL_DEVICE_EXTENSION dx)
{
//	DebugPrintMsg("UnlockDevice");
	LONG UsageCount = InterlockedDecrement(&dx->UsageCount);
	if( UsageCount==0)
	{
		DebugPrintMsg("UnlockDevice: setting StoppingEvent flag");
		KeSetEvent( &dx->StoppingEvent, 0, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
