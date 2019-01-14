// Si10DlLL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Si10DlLL.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
SI10DLLL_API int nSi10DlLL=0;

// This is an example of an exported function.
SI10DLLL_API int fnSi10DlLL(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Si10DlLL.h for the class definition
CSi10DlLL::CSi10DlLL()
{ 
	m_bDevicePresent = false;
	return; 
}


bool CSi10DlLL::Open()
{
	PLX_PARAMS			IoBuffer;
	PLX_DEVICE_KEY		Key;
	unsigned long BytesReturned;
	int ReturnCode=0;

	if(&m_Device == NULL)    return false;

	if(IsObjectValid(&m_Device))
        return false;

	memset(&m_Device, 0, sizeof(PLX_DEVICE_OBJECT));

	ObjectInvalidate( &m_Device );

	m_Device.Key.ApiMode = PLX_API_MODE_PCI;

	m_Device.hDevice = CreateFile("\\\\.\\plx9030_v600-0",
										GENERIC_READ | GENERIC_WRITE,        // Desired access
										FILE_SHARE_READ | FILE_SHARE_WRITE,  // Shared mode
										NULL,                                // Security attributes
										OPEN_EXISTING,                       // Creation disposition
										FILE_ATTRIBUTE_NORMAL,               // File attributes
										NULL                                 // Template file
									);

	if(m_Device.hDevice == INVALID_HANDLE_VALUE)
		return false;

	memset(&Key,PCI_FIELD_IGNORE,sizeof(PLX_DEVICE_KEY));

	 // Set up fields to be used in search
	Key.DeviceId    = PCI_DEVICE_ID;
	Key.VendorId    = PCI_VENDOR_ID;

	IoBuffer.Key	= Key;
	IoBuffer.value[0] = 0;


	DeviceIoControl(m_Device.hDevice, PLX_IOCTL_PCI_DEVICE_FIND,
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Input
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Output
							&BytesReturned,NULL);

	if(IoBuffer.ReturnCode != SI10ApiSuccess) {
		CloseHandle(m_Device.hDevice);
		return false;
	}

	m_Device.Key = IoBuffer.Key;
	m_Device.Key.ApiIndex = 0;
	m_Device.Key.ApiMode = PLX_API_MODE_PCI;

	ObjectValidate( &m_Device.Key );

	ObjectValidate( &m_Device );

	m_bDevicePresent = true;

	return true;
}

void CSi10DlLL::Close()
{
	if(m_bDevicePresent == true) {
		//Make sure no clocks are enabled
//		U32 CounterEnabled = 0;
//		SI10PciBarSpaceWrite(0x1A8,4,&CounterEnabled);

		//Turn off interrupts
//		U32 Interrupt = 0x0;
//		SI10PciBarSpaceWrite(0x1AC,4,&Interrupt);

		//Turn off GPIO interrupts
//		SI10PciBarSpaceWrite(0x304,4,&Interrupt);

//		StopAllMotorsDead();

//		DeRegisterInterruptCallback();
	}

	CloseHandle(m_Device.hDevice);
	m_bDevicePresent = false;

}
