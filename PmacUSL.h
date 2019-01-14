// PmacUSL.h: interface for the CPmacUSL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMACUSL_H__E6DB855F_1551_42FD_B66A_376AC9EE7056__INCLUDED_)
#define AFX_PMACUSL_H__E6DB855F_1551_42FD_B66A_376AC9EE7056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "C:\WindowsProjects\DeltaTau\INCLUDE\MIOCTL.H"
#include "HRTimer.h"


//#define MAX_INT_LEVELS	8				// levels of interrupts

typedef enum _PMAC_API_MODE
{
    PMAC_API_MODE_PCI,                   // Device accessed via PLX driver over PCI/PCIe
    PMAC_API_MODE_I2C_AARDVARK,          // Device accessed via Aardvark I2C USB
    PMAC_API_MODE_TCP                    // Device accessed via TCP/IP
} PMAC_API_MODE;

// PCI Device Key Identifier
typedef struct _PMAC_DEVICE_KEY
{
    U32          IsValidTag;         // Magic number to determine validity
    U8           bus;                // Physical device location
    U8           slot;
    U8           function;
    U16          VendorId;           // Device Identifier
    U16          DeviceId;
    U16          SubVendorId;
    U16          SubDeviceId;
    U8           Revision;
    U8           ApiIndex;           // Used internally by the API
    U8           DeviceNumber;       // Used internally by device drivers
    PMAC_API_MODE ApiMode;            // Mode API uses to access device
    U8           PlxPort;            // PLX port number of device
    U8           NTPortType;         // Stores if NT port & whether link or virtual
    U32          Reserved[8];        // Reserved for future use
} PMAC_DEVICE_KEY;

typedef struct _PMAC_PHYSICAL_MEM
{
    U64 UserAddr;                    // User-mode virtual address
    U64 PhysicalAddr;                // Bus physical address
    U64 CpuPhysical;                 // CPU physical address
    U32 Size;                        // Size of the buffer
} PMAC_PHYSICAL_MEM;

typedef struct _PMAC_PCI_BAR_PROP
{
    U32      BarValue;               // Actual value in BAR
    U64      Physical;               // BAR Physical Address
    U64      Size;                   // Size of BAR space
    BOOLEAN  bIoSpace;               // Memory or I/O space?
    BOOLEAN  bPrefetchable;          // Is space pre-fetchable?
    BOOLEAN  b64bit;                 // Is PCI BAR 64-bit?
} PMAC_PCI_BAR_PROP;


// PLX Device Object Structure
typedef struct _PMAC_DEVICE_OBJECT
{
    U32					IsValidTag;   // Magic number to determine validity
    PMAC_DEVICE_KEY     Key;          // Device location key identifier
    HANDLE				hDevice;      // Handle to driver
    PMAC_PCI_BAR_PROP   PciBar[6];    // PCI BAR properties
    U64					PciBarVa[6];  // For PCI BAR user-mode BAR mappings
    U8					BarMapRef[6]; // BAR map count used by API
    U32					PmacChipType;  // PLX chip type
    U8					PmacRevision;  // PLX chip revision
    PMAC_PHYSICAL_MEM	CommonBuffer; // Used to store common buffer information
    U32					Reserved[8];  // Reserved for future use
} PMAC_DEVICE_OBJECT;
/*
typedef struct
{
  HANDLE hEvent[MAX_INT_LEVELS + 1];
} DEVICEINTARRAY;
*/
UINT PmacUSLThread(LPVOID pParam);
UINT PendForPmacInterrupt(LPVOID pParam);

class CPmacUSL  
{
public:
	bool RequestSlaveVariable(CString VariableName, int nNode, int nOffset, CString &RxBuff);
	bool GetPmacType(PVOID pMem);
	void InterruptServiceRoutine();
	void DisableInterrupt();
	void EnableInterrupt();
	int ReadDpram(int nOffset);
	void WriteDpram(int nOffset, int nValue, bool bDoMemCheck = false);
	int DpramGetMem(int nOffset,int nLength,PVOID pMem);
	int DpramSetMem(int nOffset,int nLength,PVOID pMem, bool bDoMemCheck);
	bool RequestVariable(CString VariableName, int nOffset, CString &RxBuff);
	int RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff);
	int RequestMVariableDefinitions(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff);
	void FlushStr();
	bool IsStrReadyRead();
	int SendStr(CString TxBuff,bool bWaitForResponce = false);
	bool GetStr(CString &RxBuff);
	bool Initialize();
	bool StartDriver(CString &strPmacType,int *nPmacType);
	bool SuspendDriver();

	int		m_nOldPos[16];
//	int		m_nNewPos[16];
	int		m_nLowerBlock[0x100];
	int		m_nUpperBlock[0x100];
	int		m_nVirtualBlock[0x100];

	bool		m_bIntCallbackThreadEnabled;
	bool		m_bIntCallbackThreadFinished;
	bool		m_bInterruptEnabled;
	int			m_nEventsCount;
	CWinThread*	m_pInterruptThread;
	PMAC_DEVICE_OBJECT	m_Device;
	DEVICEINTARRAY		m_DeviceInterrupt;

	CHRTimer m_HRTimer;

	CPmacUSL();
	virtual ~CPmacUSL();

};

#endif // !defined(AFX_PMACUSL_H__E6DB855F_1551_42FD_B66A_376AC9EE7056__INCLUDED_)
