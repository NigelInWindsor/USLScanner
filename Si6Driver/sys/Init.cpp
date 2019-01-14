//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
//	SI6 example
/////////////////////////////////////////////////////////////////////////////
//	init.cpp:		Driver initialization code
/////////////////////////////////////////////////////////////////////////////
//	DriverEntry		Initialisation entry point
//	SI6CreateDevice
//	SI6Unload		Unload driver routine
//	SI6DeleteDevice
//	LockDevice
//	UnlockDevice
/////////////////////////////////////////////////////////////////////////////

#include "SI6Driver.h"

/////////////////////////////////////////////////////////////////////////////

PDEVICE_OBJECT si6ddo = NULL;

NTSTATUS SI6CreateDevice( IN PDRIVER_OBJECT DriverObject);
void SI6DeleteDevice();

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
	DebugPrintInit("SI6 checked");
#else
	DebugPrintInit("SI6 free");
#endif

	DebugPrint("RegistryPath is %T",RegistryPath);

	// Export other driver entry points...
// 	DriverObject->DriverExtension->AddDevice = SI6AddDevice;
 	DriverObject->DriverStartIo = SI6StartIo;
 	DriverObject->DriverUnload = SI6Unload;

	DriverObject->MajorFunction[IRP_MJ_CREATE] = SI6Create;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = SI6Close;

//	DriverObject->MajorFunction[IRP_MJ_PNP] = SI6Pnp;		No PnP handler...
//	DriverObject->MajorFunction[IRP_MJ_POWER] = SI6Power;

	DriverObject->MajorFunction[IRP_MJ_READ] = SI6Read;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = SI6Write;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = SI6DeviceControl;

//	DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = SI6SystemControl;

	DriverObject->MajorFunction[IRP_MJ_CLEANUP] = SI6DispatchCleanup;

	status = SI6CreateDevice(DriverObject);

	DebugPrintMsg("DriverEntry completed");

	return status;
}

//////////////////////////////////////////////////////////////////////////////

#define	NT_DEVICE_NAME	L"\\Device\\SI6"
#define	SYM_LINK_NAME	L"\\DosDevices\\SI6"

NTSTATUS SI6CreateDevice( IN PDRIVER_OBJECT DriverObject)
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
				sizeof(SI6_DEVICE_EXTENSION),
				&deviceName,
				FILE_DEVICE_UNKNOWN,
				0,
				TRUE,	// Exclusive
				&si6ddo);
	if( !NT_SUCCESS(status))
	{
		DebugPrintMsg("Could not create device");
		return status;
	}

	si6ddo->Flags |= DO_BUFFERED_IO;
	si6ddo->Flags |= DO_DIRECT_IO;

	// Initialise device extension
	PSI6_DEVICE_EXTENSION dx = (PSI6_DEVICE_EXTENSION)si6ddo->DeviceExtension;
	dx->si6ddo = si6ddo;
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
	status = IoInitializeTimer( si6ddo, (PIO_TIMER_ROUTINE)Timeout1s, dx);
	if( !NT_SUCCESS(status))
	{
		DebugPrintMsg("Could not initialise timer");
		IoDeleteDevice(si6ddo);
		return status;
	}

	// Create a symbolic link so our device is visible to Win32...
	DebugPrint("Creating symbolic link %T",&linkName);
	status = IoCreateSymbolicLink( &linkName, &deviceName);
	if( !NT_SUCCESS(status)) 
	{
		DebugPrintMsg("Could not create symbolic link");
		IoDeleteDevice(si6ddo);
		return status;
	}
	
	// Initialise our DPC for IRQ completion processing
	IoInitializeDpcRequest( si6ddo, SI6DpcForIsr);

	return status;
}

#pragma code_seg() // end INIT section

//////////////////////////////////////////////////////////////////////////////
//	SI6Unload
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

VOID SI6Unload(IN PDRIVER_OBJECT DriverObject)
{
	DebugPrintMsg("SI6Unload");
	SI6DeleteDevice();
	DebugPrintClose();
}

//////////////////////////////////////////////////////////////////////////////

void SI6DeleteDevice()
{
	if( si6ddo==NULL) return;

	PSI6_DEVICE_EXTENSION dx = (PSI6_DEVICE_EXTENSION)si6ddo->DeviceExtension;
	StopDevice(dx);

	// Stop timer (not sure if this is necessary)
	if( dx->StopTimer)
		IoStopTimer(si6ddo);

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
	DebugPrintMsg("Deleting si6ddo device object");
	IoDeleteDevice(si6ddo);
}

//////////////////////////////////////////////////////////////////////////////
#pragma code_seg() // end PAGE section

/////////////////////////////////////////////////////////////////////////////
//	LockDevice:	Lock out PnP remove request

bool LockDevice( IN PSI6_DEVICE_EXTENSION dx)
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

void UnlockDevice( IN PSI6_DEVICE_EXTENSION dx)
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
