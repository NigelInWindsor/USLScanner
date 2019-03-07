#include "stdafx.h"
#include "..\..\SI10-64-Driver\Public.h"
#include "SI10x64.h"
#include "USLScanner.h"


#ifndef _X64_
#define _X64_

#define SPIN_COUNT_FOR_CS       0x4000
UINT	CSI10x64PendForInterrupt(LPVOID pParam);


CSI10x64::CSI10x64()
{
	m_hDevInfo = m_pDeviceInterfaceDetail = NULL;
	m_hDevice = INVALID_HANDLE_VALUE;
	CSInitialized = FALSE;
	m_bConnected = FALSE;
	m_nMailboxFailed = 0;
	m_nMaxMailboxWait = 0;
	m_nMailboxCount = 0;
	m_Status = STATUS_FAIL;
	m_bIntCallbackThreadFinished = false;
	m_bIntCallbackThreadEnabled = false;
	m_hEvent = NULL;
	m_pThread = NULL;
	m_nEventsCount = 0;
}


CSI10x64::~CSI10x64()
{
	CloseDevice();
	m_bConnected = false;
	if (CSInitialized) {
		DeleteCriticalSection(&CriticalSection);
	}
	if (m_hDevInfo) {
		SetupDiDestroyDeviceInfoList(m_hDevInfo);
	}

	if (m_pDeviceInterfaceDetail) {
		free(m_pDeviceInterfaceDetail);
	}


	if (m_hDevice != INVALID_HANDLE_VALUE) {
		CloseHandle(m_hDevice);
		m_hDevice = INVALID_HANDLE_VALUE;
	}

}

void CSI10x64::CloseDevice()
{
	if (IsConnected() == TRUE) {

		//Make sure no clocks are enabled
		int CounterEnabled = 0;
		OutpW(0x1A8, CounterEnabled);

		//Turn off interrupts
		int Interrupt = 0x0;
		OutpW(0x1AC, Interrupt);

		//Turn off GPIO interrupts
		OutpW(0x304, Interrupt);
		Sleep(2);
		DeRegisterInterruptCallback();
	}
}

BOOL CSI10x64::Initialize()
{
	ULONG nStatus;
	BOOL retValue = true;

	retValue = GetDevicePath();
	if (!retValue) {
		return retValue;
	}

	if (!CSInitialized) {
		retValue = (BOOL)InitializeCriticalSectionAndSpinCount(&CriticalSection, SPIN_COUNT_FOR_CS);
		if (!retValue) {
			m_Status = CRITICAL_SECTION_FAIL;
			nStatus = GetLastError();
			return retValue;
		}
		CSInitialized = TRUE;
	}

	if ((retValue = GetDeviceHandle()) == TRUE) {
		retValue = LoadFPGA();

		RegisterInterruptCallback();
	}

	return m_bConnected = retValue;
}

BOOL CSI10x64::IsConnected()
{
	return m_bConnected;
}

BOOL CSI10x64::IsHandle()
{
	if (m_hDevice != INVALID_HANDLE_VALUE) return true;
	return false;
}

static const GUID GuidDriver = { 0x78A1C341, 0x4539, 0x11d3,{ 0xB8, 0x8D, 0x00, 0xC0, 0x4F, 0xAD, 0x51, 0x71 } };
//DEFINE_GUID(GUID_SI10_INTERFACE,
//	0x78A1C341, 0x4539, 0x11d3, 0xB8, 0x8D, 0x00, 0xC0, 0x4F, 0xAD, 0x51, 0x71);

BOOL CSI10x64::GetDevicePath()
{
	SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
	SP_DEVINFO_DATA DeviceInfoData;

	ULONG size;
	int count, i, index;
	BOOL status = TRUE;
	TCHAR *DeviceName = NULL;
	TCHAR *DeviceLocation = NULL;

	//
	//  Retreive the device information for all PLX devices.
	//
	m_hDevInfo = SetupDiGetClassDevs(&GuidDriver,
		NULL,
		NULL,
		DIGCF_DEVICEINTERFACE |
		DIGCF_PRESENT);

	//
	//  Initialize the SP_DEVICE_INTERFACE_DATA Structure.
	//
	DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	//
	//  Determine how many devices are present.
	//
	count = 0;
	while (SetupDiEnumDeviceInterfaces(m_hDevInfo,
		NULL,
		&GuidDriver,
		count++,  //Cycle through the available devices.
		&DeviceInterfaceData)
		);

	//
	// Since the last call fails when all devices have been enumerated,
	// decrement the count to get the true device count.
	//
	count--;

	//
	//  If the count is zero then there are no devices present.
	//
	if (count == 0) {
		this->m_Status = NO_SI10_DEVICE_FOUND;
		return FALSE;
	}

	//
	//  Initialize the appropriate data structures in preparation for
	//  the SetupDi calls.
	//
	DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	//
	//  Loop through the device list to allow user to choose
	//  a device.  If there is only one device, select it
	//  by default.
	//
	i = 0;
	while (SetupDiEnumDeviceInterfaces(m_hDevInfo,
		NULL,
		(LPGUID)&GuidDriver,
		i,
		&DeviceInterfaceData)) {

		//
		// Determine the size required for the DeviceInterfaceData
		//
		SetupDiGetDeviceInterfaceDetail(m_hDevInfo,
			&DeviceInterfaceData,
			NULL,
			0,
			&size,
			NULL);

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			//			printf("SetupDiGetDeviceInterfaceDetail failed, Error: %u", GetLastError());
			this->m_Status = FAILED_TO_SETUP_DEVICE_INTERFACE;
			return FALSE;
		}

		if(m_pDeviceInterfaceDetail == NULL)
			m_pDeviceInterfaceDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(size);

		if (!m_pDeviceInterfaceDetail) {
			this->m_Status = INSUFFICIENT_MEMORY;
			return FALSE;
		}

		//
		// Initialize structure and retrieve data.
		//
		m_pDeviceInterfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		status = (BOOL)SetupDiGetDeviceInterfaceDetail(m_hDevInfo,
			&DeviceInterfaceData,
			m_pDeviceInterfaceDetail,
			size,
			NULL,
			&DeviceInfoData);

		free(m_pDeviceInterfaceDetail);
		m_pDeviceInterfaceDetail = NULL;

		if (!status) {
			this->m_Status = FAILED_TO_SETUP_DEVICE_INTERFACE;
			return status;
		}

		//
		//  Get the Device Name
		//  Calls to SetupDiGetDeviceRegistryProperty require two consecutive
		//  calls, first to get required buffer size and second to get
		//  the data.
		//
		SetupDiGetDeviceRegistryProperty(m_hDevInfo,
			&DeviceInfoData,
			SPDRP_DEVICEDESC,
			NULL,
			(PBYTE)DeviceName,
			0,
			&size);

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			this->m_Status = FAILED_TO_GET_REGISTRY_DETAILS;
			return FALSE;
		}

		DeviceName = (TCHAR*)malloc(size);
		if (!DeviceName) {
			this->m_Status = INSUFFICIENT_MEMORY_FOR_DEVICE_NAME;
			return FALSE;
		}

		status = (BOOL)SetupDiGetDeviceRegistryProperty(m_hDevInfo,
			&DeviceInfoData,
			SPDRP_DEVICEDESC,
			NULL,
			(PBYTE)DeviceName,
			size,
			NULL);
		if (!status) {
			free(DeviceName);
			this->m_Status = FAILED_TO_GET_REGISTRY_DETAILS;
			return status;
		}

		//
		//  Now retrieve the Device Location.
		//
		SetupDiGetDeviceRegistryProperty(m_hDevInfo,
			&DeviceInfoData,
			SPDRP_LOCATION_INFORMATION,
			NULL,
			(PBYTE)DeviceLocation,
			0,
			&size);

		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			DeviceLocation = (TCHAR*)malloc(size);

			if (DeviceLocation != NULL) {

				status = SetupDiGetDeviceRegistryProperty(m_hDevInfo,
					&DeviceInfoData,
					SPDRP_LOCATION_INFORMATION,
					NULL,
					(PBYTE)DeviceLocation,
					size,
					NULL);
				if (!status) {
					free(DeviceLocation);
					DeviceLocation = NULL;
				}
			}

		}
		else {
			DeviceLocation = NULL;
		}

		free(DeviceName);
		DeviceName = NULL;

		if (DeviceLocation) {
			free(DeviceLocation);
			DeviceLocation = NULL;
		}

		i++; // Cycle through the available devices.
	}

	//
	//  Select device.
	//
	index = 0;
	if (count > 1) {
		if (scanf_s("%d", &index) == 0) {
			this->m_Status = STATUS_FAIL;
			return ERROR_INVALID_DATA;
		}
	}

	//
	//  Get information for specific device.
	//
	status = SetupDiEnumDeviceInterfaces(m_hDevInfo,
		NULL,
		(LPGUID)&GuidDriver,
		index,
		&DeviceInterfaceData);

	if (!status) {
		this->m_Status = FAILED_TO_ENUMERATE;
		return status;
	}

	//
	// Determine the size required for the DeviceInterfaceData
	//
	SetupDiGetDeviceInterfaceDetail(m_hDevInfo,
		&DeviceInterfaceData,
		NULL,
		0,
		&size,
		NULL);

	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
		this->m_Status = FAILED_TO_GET_INTERFACE_DETAILS;
		return FALSE;
	}

	if(m_pDeviceInterfaceDetail == NULL)
		m_pDeviceInterfaceDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(size);

	if (!m_pDeviceInterfaceDetail) {
		this->m_Status = INSUFFICIENT_MEMORY_FOR_DETAILS;
		return FALSE;
	}

	//
	// Initialize structure and retrieve data.
	//
	m_pDeviceInterfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	status = SetupDiGetDeviceInterfaceDetail(m_hDevInfo,
		&DeviceInterfaceData,
		m_pDeviceInterfaceDetail,
		size,
		NULL,
		&DeviceInfoData);
	if (!status) {
		this->m_Status = FAILED_TO_GET_INTERFACE_DETAILS;
		return status;
	}

	this->m_Status = STATUS_SUCCESS;

	return status;
}

BOOL CSI10x64::GetDeviceHandle()
{
	BOOL status = TRUE;
	CString Buff;

	if (m_pDeviceInterfaceDetail == NULL) {
		status = GetDevicePath();
	}
	if (m_pDeviceInterfaceDetail == NULL) {
		status = FALSE;
	}

	if (status) {

		m_hDevice = CreateFile(m_pDeviceInterfaceDetail->DevicePath,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
			INVALID_HANDLE_VALUE);

		if (m_hDevice == INVALID_HANDLE_VALUE) {
			status = FALSE;
			this->m_Status = FAILED_TO_ALLOCATE_HANDLE;
		}
	}

	return status;
}




BOOL CSI10x64::FindLatestBinaryFile(CString *pFilePathName)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = NULL;
	WCHAR *FilePath = new WCHAR[512];
	WCHAR *Directory = new WCHAR[512];
	WCHAR *Buff = new WCHAR[512];
	int nHighest = 0;
	BOOL bStatus = false;

	pFilePathName->Empty();

	GetSystemDirectory(Buff, 512);
	swprintf_s(Directory, 512, L"%s\\Drivers", Buff);
	swprintf_s(FilePath, 512, L"%s\\si10v1_fpgava1*.bin", Directory);
	hFind = FindFirstFile(FilePath, &ffd);

	if (hFind == INVALID_HANDLE_VALUE) {
		GetCurrentDirectory(512, Directory);
		//		swprintf_s(Directory, 512, L"%s", theApp.m_PathName.GetAt(CUSLFile::working));
		swprintf_s(FilePath, 512, L"%s\\si10v1_fpgava1*.bin", Directory);
		hFind = FindFirstFile(FilePath, &ffd);
	}

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if ((_ttoi(ffd.cFileName + 14)) >= nHighest) {
				nHighest = _ttoi(ffd.cFileName + 14);
				pFilePathName->Format(L"%s\\%s", Directory, ffd.cFileName);
				bStatus = TRUE;
			}

		} while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);

	}
	delete FilePath;
	delete Directory;
	delete Buff;

	return bStatus;


}

BOOL CSI10x64::isFPGALoaded(CString *pFileName)
{
	int Date;
	CString strDate;

	//GetDate
	DoMem(READ_FROM_CARD, 2, (PUINT8)&Date, 0, SI10_FPGA_BINARY_DATE, 4);		//View the date in hexadecimal

	int nDay = (((Date >> 4) & 0x0f) * 10) + (Date & 0x0f);			//view in decimal
	int nMonth = (((Date >> 12) & 0x0f) * 10) + ((Date >> 8) & 0x0f);	//view in decimal
	int nYear = (((Date >> 20) & 0x0f) * 10) + ((Date >> 16) & 0x0f);	//view in decimal

	strDate.Format(L"%02d%02d%02d", nYear, nMonth, nDay);

	if (pFileName) {
		if (pFileName->Find(strDate) > 0) return true;
	}
	return false;
}

int CSI10x64::InpW(int nOffset)
{
	int nValue;
	if (IsHandle() == false) return -1;

	DoMem(READ_FROM_CARD, 2, (PUINT8)&nValue, 0, nOffset, sizeof nValue);
	return nValue;
}

void CSI10x64::OutpW(int nOffset, int nValue)
{
	if (IsHandle() == false) return;

	DoMem(WRITE_TO_CARD, 2, (PUINT8)&nValue, 0, nOffset, 4);
}

int CSI10x64::PCIRegRead(int nOffset)
{
	int nValue;
	if (IsHandle() == false) return -1;

	DoMem(READ_FROM_CARD, 0, (PUINT8)&nValue, 0, nOffset, 4);
	return nValue;
}

void CSI10x64::PCIRegWrite(int nOffset, int nValue)
{
	if (IsHandle() == false) return;

	DoMem(WRITE_TO_CARD, 0, (PUINT8)&nValue, 0, nOffset, 4);
}


BOOL CSI10x64::DoMem(
	UINT32          Rd_Wr_n,
	UINT32          BarNum,
	PUINT8          Buffer,
	UINT64          Offset,
	UINT64          CardOffset,
	UINT64          Length
)
{
	SI10_DO_MEM_STRUCT   doMemStruct;
	OVERLAPPED      os;         // OVERLAPPED structure for the operation
	DWORD           bytesReturned = 0;
	DWORD           ioctlCode;
	DWORD           LastErrorStatus = 0;
	int				nStatus = 0;
	BOOL          bStatus = true;

	os.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	doMemStruct.BarNum = BarNum;
	doMemStruct.Offset = Offset;
	doMemStruct.CardOffset = CardOffset;
	doMemStruct.Length = Length;

	if (Rd_Wr_n == READ_FROM_CARD) {
		ioctlCode = SI10_IOCTL_PCI_BAR_SPACE_READ;
	}
	else {
		ioctlCode = SI10_IOCTL_PCI_BAR_SPACE_WRITE;
	}

	if (!DeviceIoControl(m_hDevice, ioctlCode,
		(LPVOID)&doMemStruct, sizeof(SI10_DO_MEM_STRUCT),
		(LPVOID)Buffer, (DWORD)Length,
		&bytesReturned, &os))
	{
		LastErrorStatus = GetLastError();
		if (LastErrorStatus == ERROR_IO_PENDING)
		{
			// Wait here (forever) for the Overlapped I/O to complete
			if (!GetOverlappedResult(m_hDevice, &os, &bytesReturned, TRUE))
			{
				nStatus = LastErrorStatus = GetLastError();
			}
		}
		else
		{
			nStatus = LastErrorStatus;
		}
	}

	if ((bytesReturned != Length) && (bStatus == TRUE))
	{
		bStatus = false;
	}

	CloseHandle(os.hEvent);
	return bStatus;
}


BOOL CSI10x64::LoadFPGA()
{

	CFile File;
	CFileException ExceptionInstance;
	CString FilePathName;

	OVERLAPPED      os;         // OVERLAPPED structure for the operation
	int				ReturnBuffer = 0;	//Not technically needed, just thought I'd add it for fun
	DWORD       bytesReturned = 0;
	DWORD           LastErrorStatus = 0;
	int				nStatus = 0;
	BOOL			bStatus = true;
	DWORD		nLength = 0;
	unsigned char *FPGAArray = NULL;

	if ((bStatus = FindLatestBinaryFile(&FilePathName)) == TRUE) {

		if (File.Open(FilePathName, CFile::typeBinary | CFile::modeRead, &ExceptionInstance)) {
			m_BinaryFileName = File.GetFileName();
			nLength = (DWORD)File.GetLength();
			if (nLength > 0) {
				FPGAArray = new unsigned char[(unsigned int)nLength];
				unsigned int nBytesRead = (unsigned int)File.Read(FPGAArray, (unsigned int)nLength);
				bStatus = true;
			}
			else {
				m_Status = BINARY_FILE_HAS_NO_LENGTH;
				bStatus = false;
			}
			File.Close();
		}
	}
	else {
		this->m_Status = NO_BINARY_FILES_FOUND;
		MessageBox(NULL, L"Please install the SI10 binary files", L"Error", MB_ICONERROR);
		return false;
	}

	if ((bStatus == TRUE) && (isFPGALoaded(&m_BinaryFileName) == FALSE || theApp.m_LastSettings.nForceSi10FirmwareLoad == 1)) {

		os.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		if (!DeviceIoControl(m_hDevice, IOCTL_SI10_DOWNLOAD_FPGA_CODE,
			(LPVOID)FPGAArray, nLength,
			(LPVOID)ReturnBuffer, sizeof ReturnBuffer,
			&bytesReturned, &os)) {

			LastErrorStatus = GetLastError();
			if (LastErrorStatus == ERROR_IO_PENDING)
			{
				// Wait here (forever) for the Overlapped I/O to complete
				if (!GetOverlappedResult(m_hDevice, &os, &bytesReturned, TRUE))
				{
					nStatus = LastErrorStatus = GetLastError();
				}
			}
			else {
				nStatus = LastErrorStatus;
			}
		}

		CloseHandle(os.hEvent);

		//Check to make sure it's loaded correctly
		bStatus = isFPGALoaded(&m_BinaryFileName);
	}


	if (FPGAArray) delete FPGAArray;

	return bStatus;

}


int CSI10x64::ReadADC(int nWhich)
{
	if (IsConnected() != TRUE) return -1;

	if (nWhich < 0) nWhich = 0;
	if (nWhich > 3) nWhich = 3;

	int nAllValues = InpW(SI10_JOYSTICK);

	return ((nAllValues >> (nWhich * 8)) & 0x000000ff);
}

BOOL CSI10x64::MailboxTx(unsigned char *pArray, int nLength, unsigned int nMask)
{

	for (int ii = 0; ii < nLength; ii++) {
		MailboxTx(pArray[ii]);
	}
	if (nMask & MAILBOX_CS) MailboxTxCheckSum();
	if (nMask & MAILBOX_EOS) MailboxTx(0xff);
	return TRUE;
}

BOOL CSI10x64::MailboxTx(int	nValue)
{
	if (IsConnected() != TRUE) return FALSE;
	if (m_nMailboxFailed) return FALSE;

	int nTimeOut = 0;
	while (InpW(SI10_MAILBOX_STATUS) & 0x03) {
		nTimeOut++;
		if (nTimeOut>m_nMaxMailboxWait) m_nMaxMailboxWait = nTimeOut;
		if (nTimeOut >= 32000) {
			m_nMailboxFailed++;
			MessageBox(NULL, _T("Mailbox has failed"), _T("Error"), NULL);
			return FALSE;
		};
	};
	m_nMailboxCount++;

	OutpW(SI10_MAILBOX_PORT, nValue);

	nValue & 0x80 ? m_nCheckSum = nValue : m_nCheckSum += nValue;

	return TRUE;
}

BOOL CSI10x64::MailboxTxCheckSum()
{
	if (IsConnected() != TRUE) return FALSE;

	MailboxTx(m_nCheckSum & 0x7f);

	return TRUE;
}

//The Checksum and EOS will be added automatically
BOOL CSI10x64::MailboxString(
	PUINT8          Array,
	int	          Length,
	unsigned int		nMask
)
{
	MAILBOX_STRING_STRUCT   doStringStruct;
	OVERLAPPED      os;         // OVERLAPPED structure for the operation
	DWORD           bytesReturned = 0;
	DWORD           ioctlCode;
	DWORD           LastErrorStatus = 0;
	int				nStatus = 0;
	BOOL          bStatus = true;

	if (IsConnected() != TRUE) return FALSE;
	if (m_nMailboxFailed) return FALSE;

	os.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	doStringStruct.nLength = Length;
	doStringStruct.nMask = nMask;

	ioctlCode = SI10_IOCTL_MAILBOX_STRING_WRITE;

	if (!DeviceIoControl(m_hDevice, ioctlCode,
		(LPVOID)&doStringStruct, sizeof(MAILBOX_STRING_STRUCT),
		(LPVOID)Array, (DWORD)Length,
		&bytesReturned, &os))
	{
		LastErrorStatus = GetLastError();
		if (LastErrorStatus == ERROR_IO_PENDING)
		{
			// Wait here (forever) for the Overlapped I/O to complete
			if (!GetOverlappedResult(m_hDevice, &os, &bytesReturned, TRUE))
			{
				nStatus = LastErrorStatus = GetLastError();
			}
		}
		else
		{
			nStatus = LastErrorStatus;
		}
	}
	CloseHandle(os.hEvent);

	if (nStatus == MAILBOX_TIMED_OUT) {
		m_nMailboxFailed = 1;
		MessageBox(NULL, _T("Mailbox has failed"), _T("Error"), NULL);
		bStatus = false;
	}

	return bStatus;
}

CString &CSI10x64::getFPGABinaryDate()
{
	static CString strDate;

	if (IsConnected()) {
		int nDate = InpW(SI10_FPGA_BINARY_DATE);

		int nDay = (((nDate >> 4) & 0x0f) * 10) + (nDate & 0x0f);			//view in decimal
		int nMonth = (((nDate >> 12) & 0x0f) * 10) + ((nDate >> 8) & 0x0f);	//view in decimal
		int nYear = (((nDate >> 20) & 0x0f) * 10) + ((nDate >> 16) & 0x0f);	//view in decimal

		strDate.Format(L"%d-%d-%d (%02d%02d%02d)", nDay, nMonth, nYear + 2000, nYear, nMonth, nDay);
	}
	else {
		strDate.Empty();
	}

	return strDate;
}

CString CSI10x64::getBinaryFileName()
{
	return m_BinaryFileName;
}

CString &CSI10x64::getDriverVersion()
{
	static CString Buff;

	SI10_DEVICE_DRIVER_VERSION   Version;
	OVERLAPPED      os;         // OVERLAPPED structure for the operation
	DWORD           bytesReturned = 0;
	DWORD           ioctlCode;
	DWORD           LastErrorStatus = 0;
	int				nStatus = 0;
	BOOL			bStatus = true;
	int				nLength = sizeof(SI10_DEVICE_DRIVER_VERSION);

	os.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	ZeroMemory(&Version, sizeof Version);

	ioctlCode = SI10_IOCTL_DRIVER_VERSION;

	if (!DeviceIoControl(m_hDevice, ioctlCode,
		NULL, 0,
		(LPVOID)&Version, nLength,
		&bytesReturned, &os))
	{
		LastErrorStatus = GetLastError();
		if (LastErrorStatus == ERROR_IO_PENDING)
		{
			// Wait here (forever) for the Overlapped I/O to complete
			if (!GetOverlappedResult(m_hDevice, &os, &bytesReturned, TRUE))
			{
				nStatus = LastErrorStatus = GetLastError();
			}
		}
		else
		{
			nStatus = LastErrorStatus;
		}
	}
	CloseHandle(os.hEvent);

	Buff.Empty();
	if ((bytesReturned == nLength) && (bStatus == TRUE))
	{
		Buff.Format(L"%d.%d.%d  %s", Version.nMajorNumber, Version.nMinorNumber, Version.nSubMinorNumber, (CString)Version.BuildNumber);
	}


	return Buff;
}

CString &CSI10x64::getSDKVersion()
{
	static CString Buff;

	Buff.Format(L"%d.%d.%d  %s", SI10_MAJOR_NUM, SI10_MINOR_NUM, SI10_SUBMINOR_NUM, (CString)SI10_BUILD_NUM);

	return Buff;
}

CString &CSI10x64::GetLastErrorMsg()
{
	static CString Buff;


	switch (m_Status) {
	case STATUS_FAIL: Buff = "General failure";
		break;
	case STATUS_SUCCESS: Buff = "Success";
		break;
	case NO_SI10_DEVICE_FOUND: Buff = "No Si10 detected in system";
		break;
	case FAILED_TO_SETUP_DEVICE_INTERFACE: "Failed to setup device interface";
		break;
	case INSUFFICIENT_MEMORY: Buff = "Insufficient memory";
		break;
	case FAILED_TO_GET_REGISTRY_DETAILS: Buff = "Get Registry details failed";
		break;
	case INSUFFICIENT_MEMORY_FOR_DEVICE_NAME: Buff = "Insufficient memory for device name";
		break;
	case FAILED_TO_ENUMERATE: Buff = "Failed to enumerate";
		break;
	case FAILED_TO_GET_INTERFACE_DETAILS: Buff = "Failed to get interface details";
		break;
	case INSUFFICIENT_MEMORY_FOR_DETAILS: Buff = "Insufficient memory for interface details";
		break;
	case FAILED_TO_ALLOCATE_HANDLE: Buff = "Failed to allocaye handle. Try running application as Administrator";
		break;
	case BINARY_FILE_HAS_NO_LENGTH: Buff = "Binary file has no length";
		break;
	case CRITICAL_SECTION_FAIL: Buff = "Critical section failed";
		break;
	case NO_BINARY_FILES_FOUND: Buff = "No Binary file found";
		break;
	default: Buff = "Error not recognised";
		break;
	}
	return Buff;
}

int CSI10x64::ReadTS0()
{
	int nResult = 0;

	nResult = InpW(SI10_MAILBOX_STATUS);

	if (nResult & 0x00000008) //bit 3
		return 1;
	else
		return 0;

}

#define	SI10_INTERNAL_PRF			0x01
#define	SI10_INTERNAL_SYNC			0x02
#define	SI10_DELAY_PRF				0x04
#define	SI10_CONTINUOUS_PRF			0x08
#define	SI10_SEND_SOFTWARE_TRIGGER	0x10
#define	SI10_ENABLE_INTERRUPTS		0x20
#define	SI10_LOW_PERIOD_10			0x00
#define	SI10_LOW_PERIOD_25			0x40
#define	SI10_MAILBOX_INTERRUPT		0x80

void CSI10x64::SetPRF(int nPrf)
{
	int nValue;

	nValue = SI10_INTERNAL_PRF | SI10_LOW_PERIOD_25 | SI10_CONTINUOUS_PRF;
	OutpW(SI10_MAILBOX_STATUS, nValue);

	if (nPrf <= 0)	nPrf = 100;

	nValue = 0xf4240 / nPrf;		 //1 unit = 1us

	if (nValue<20) nValue = 20;
	if (nValue>0x3b2f) nValue = 0x3b2f;
	OutpW(SI10_PRF_PERIOD, nValue);

	return;
}


void CSI10x64::ArmPRF()
{
	int nValue;

	nValue = SI10_INTERNAL_PRF | SI10_LOW_PERIOD_25 | SI10_CONTINUOUS_PRF;
	OutpW(SI10_MAILBOX_STATUS, nValue);

	return;
}

bool CSI10x64::IsTimeSlotSync()
{

	if (InpW(SI10_MAILBOX_STATUS) & 0x00000008)
		return true;
	else
		return false;

}


ULONG &CSI10x64::getISRCount()
{
	static ULONG ulISRCount;

	OVERLAPPED      os;         // OVERLAPPED structure for the operation
	DWORD           bytesReturned = 0;
	DWORD           ioctlCode;
	DWORD           LastErrorStatus = 0;
	int				nStatus = 0;
	BOOL			bStatus = true;
	int				nLength = sizeof(ulISRCount);

	os.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	ioctlCode = SI10_IOCTL_GET_ISR_COUNT;
	ulISRCount = -1;

	if (!DeviceIoControl(m_hDevice, ioctlCode,
		NULL, 0,
		(LPVOID)&ulISRCount, nLength,
		&bytesReturned, &os))
	{
		LastErrorStatus = GetLastError();
		if (LastErrorStatus == ERROR_IO_PENDING)
		{
			// Wait here (forever) for the Overlapped I/O to complete
			if (!GetOverlappedResult(m_hDevice, &os, &bytesReturned, TRUE))
			{
				nStatus = LastErrorStatus = GetLastError();
			}
		}
		else
		{
			nStatus = LastErrorStatus;
		}
	}
	CloseHandle(os.hEvent);

	return ulISRCount;
}

ULONG &CSI10x64::getIRQSource()
{
	static ULONG ulIRQSource;

	OVERLAPPED      os;         // OVERLAPPED structure for the operation
	DWORD           bytesReturned = 0;
	DWORD           ioctlCode;
	DWORD           LastErrorStatus = 0;
	int				nStatus = 0;
	BOOL			bStatus = true;
	int				nLength = sizeof(ulIRQSource);

	os.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	ioctlCode = SI10_IOCTL_GET_IRQ_SOURCE;
	ulIRQSource = -1;

	if (!DeviceIoControl(m_hDevice, ioctlCode,
		NULL, 0,
		(LPVOID)&ulIRQSource, nLength,
		&bytesReturned, &os))
	{
		LastErrorStatus = GetLastError();
		if (LastErrorStatus == ERROR_IO_PENDING)
		{
			// Wait here (forever) for the Overlapped I/O to complete
			if (!GetOverlappedResult(m_hDevice, &os, &bytesReturned, TRUE))
			{
				nStatus = LastErrorStatus = GetLastError();
			}
		}
		else
		{
			nStatus = LastErrorStatus;
		}
	}
	CloseHandle(os.hEvent);

	return ulIRQSource;
}

BOOL CSI10x64::RegisterInterruptCallback()
{
	OVERLAPPED      os;         // OVERLAPPED structure for the operation
	int				ReturnBuffer = 0;
	DWORD           bytesReturned = 0;
	DWORD           ioctlCode;
	DWORD           LastErrorStatus = 0;
	int				nStatus = 0;
	BOOL			bStatus = true;

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	LPVOID pHandle = &m_hEvent;

	os.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	ioctlCode = SI10_IOCTL_ENABLE_INTERRUPT_CALLBACK;

//	InterruptCallback.phEvent = &m_hEvent;

	if (!DeviceIoControl(m_hDevice, ioctlCode,
		(LPVOID)&m_hEvent, sizeof m_hEvent,
		(LPVOID)ReturnBuffer, sizeof ReturnBuffer,
		&bytesReturned, &os))
	{
		LastErrorStatus = GetLastError();
		if (LastErrorStatus == ERROR_IO_PENDING)
		{
			// Wait here (forever) for the Overlapped I/O to complete
			if (!GetOverlappedResult(m_hDevice, &os, &bytesReturned, TRUE))
			{
				nStatus = LastErrorStatus = GetLastError();
			}
		}
		else
		{
			nStatus = LastErrorStatus;
		}
	}
	CloseHandle(os.hEvent);

	m_bIntCallbackThreadEnabled = true;
	m_pThread = AfxBeginThread(&CSI10x64PendForInterrupt, this, THREAD_PRIORITY_TIME_CRITICAL, 0, NULL);

	return true;
}


void CSI10x64::DeRegisterInterruptCallback()
{

	if (m_bIntCallbackThreadEnabled) {
		TRACE0("Wait for Si10x64 Thread to finish\n");
		m_bIntCallbackThreadEnabled = false;
		WaitForSingleObject(m_pThread, INFINITE);
		TRACE0("SI10x64 Object finished\n");
	}

}

UINT CSI10x64PendForInterrupt(LPVOID pParam)
{
	CSI10x64* pParent = (CSI10x64 *)pParam;
	int nIrqMask;

	pParent->m_bIntCallbackThreadFinished = FALSE;
	while (pParent->m_bIntCallbackThreadEnabled == TRUE) {
		if (WaitForSingleObject(pParent->m_hEvent, 100) == WAIT_OBJECT_0) {
			pParent->m_nEventsCount++;
			nIrqMask = pParent->getIRQSource();

			ResetEvent(pParent->m_hEvent);
		}
	}
	pParent->m_bIntCallbackThreadFinished = TRUE;
	TRACE0("SI10x64 Interrupt callback finished\n");

	return 0;
}

#endif
