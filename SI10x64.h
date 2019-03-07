#pragma once
#include <SetupAPI.h>
#include <winioctl.h>

typedef enum _SI10_STATUS
{
	STATUS_FAIL = 0,
	STATUS_SUCCESS,
	NO_SI10_DEVICE_FOUND,
	FAILED_TO_SETUP_DEVICE_INTERFACE,
	INSUFFICIENT_MEMORY,
	FAILED_TO_GET_REGISTRY_DETAILS,
	INSUFFICIENT_MEMORY_FOR_DEVICE_NAME,
	FAILED_TO_ENUMERATE,
	FAILED_TO_GET_INTERFACE_DETAILS,
	INSUFFICIENT_MEMORY_FOR_DETAILS,
	FAILED_TO_ALLOCATE_HANDLE,
	BINARY_FILE_HAS_NO_LENGTH,
	CRITICAL_SECTION_FAIL,
	NO_BINARY_FILES_FOUND,

} SI10_STATUS, *PSI10_STATUS;

class CSI10x64
{
public:
	CSI10x64();
	~CSI10x64();
	void	CloseDevice();
	BOOL	Initialize();
	BOOL	IsConnected();
	BOOL	IsHandle();
	BOOL	MailboxTx(unsigned char * pArray, int nLength, unsigned int nMask = 0);
	BOOL	MailboxTx(int nValue);
	BOOL	MailboxTxCheckSum();
	BOOL	MailboxString(PUINT8 Array, int Length, unsigned int nMask = 0);
	int		ReadADC(int nWhich);
	int		InpW(int nOffset);
	void	OutpW(int nOffset, int nValue);
	int		PCIRegRead(int nOffset);
	void	PCIRegWrite(int nOffset, int nValue);

	SI10_STATUS m_Status;

private:

	BOOL	GetDeviceHandle();
	BOOL	GetDevicePath();
	BOOL	LoadFPGA();
	BOOL	FindLatestBinaryFile(CString* pFilePathName);
	BOOL	isFPGALoaded(CString * pFileName = NULL);

	BOOL	DoMem(UINT32 Rd_Wr_n, UINT32 BarNum, PUINT8 Buffer, UINT64 Offset, UINT64 CardOffset, UINT64 Length);

	BOOL		m_bConnected;
	HDEVINFO	m_hDevInfo;
	PSP_DEVICE_INTERFACE_DETAIL_DATA m_pDeviceInterfaceDetail;
	HANDLE		m_hDevice;
	CString		m_BinaryFileName;

	int			m_nMailboxFailed;
	int			m_nMaxMailboxWait;
	int			m_nCheckSum;
	int			m_nMailboxCount;

	CRITICAL_SECTION CriticalSection;
	BOOL CSInitialized;
public:
	int			m_nEventsCount;
	bool		m_bIntCallbackThreadFinished;
	bool		m_bIntCallbackThreadEnabled;
	HANDLE		m_hEvent;
	CWinThread*	m_pThread;
	CString & getFPGABinaryDate();
	CString getBinaryFileName();
	CString &getDriverVersion();
	CString &getSDKVersion();
	CString &GetLastErrorMsg();
	int ReadTS0();
	void SetPRF(int nPrf);
	void ArmPRF();
	bool IsTimeSlotSync();
	BOOL RegisterInterruptCallback();
	void DeRegisterInterruptCallback();
	ULONG & getISRCount();
	ULONG & getIRQSource();
};

