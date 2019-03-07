#pragma once

#include "FocalLaw.h"

const int g_iOEMMCProbeCountMax = 4;
const int g_iOEMMCScanCountMax = 16;
#define	MAX_AOS_ASCAN_LENGTH	8192


class CAOSPhasedArray
{
public:
	WCHAR	m_wFileIdentifier[60] = L"USLScanner Phased Array";

	CStringArray m_Messages;

	CAOSPhasedArray();
	~CAOSPhasedArray();


	float m_fElementPitch;
	float m_fTxFocalLength;
	float m_fRxFocalLength[128];
	int	m_nScanMode;
	int	m_nAScanLength = 1024;

	bool IsConnected() { return m_bConnected; }
	bool m_bConnected = false;
	int	m_nFirstElement = 0;
	int m_nLastElement = 31;
	int m_nPRF = 2500;
	int m_nCycleCount=1;
	double m_dRange = 64.0e-6;
	double m_dDelay = 64.0;
	float m_fBeamCorrectionGain = 0.0f;
	int m_nDACCount = 0;
	double m_dDACTime = 0.0;
	float m_fDACSlope = 0.0f;
	bool m_bDACEnable = false;
	bool m_bMultiplexer;	//gets initialized in get Status;

	static DWORD WINAPI CallbackThreadToolbox(HWND hWnd, CRunToolbox *pRunToolbox);
	static UINT WINAPI ProcessAcquisitionAscan_0x00010103(void *pAcquisitionParameter, structAcqInfoEx &acqInfo, const CStream_0x0001 *pStreamHeader, const CSubStreamAscan_0x0103 *pAscanHeader, const void *pBufferMax, const void *pBufferMin, const void *pBufferSat);
	static UINT WINAPI ProcessAcquisitionCscan_0x00010X02(void *pAcquisitionParameter, structAcqInfoEx &acqInfo, const CStream_0x0001 *pStreamHeader, const CSubStreamCscan_0x0X02 *pCscanHeader, const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof);
	static UINT WINAPI ProcessAcquisitionIO_1x00010101(void *pAcquisitionParameter, structAcqInfoEx &acqInfo, const CStream_0x0001 *pStreamHeader, const CSubStreamIO_0x0101 *pIOHeader);
	static structCorrectionOEMPA* WINAPI CallbackCustomizedAPI(void *pAcquisitionParameter, const wchar_t *pFileName, enumStepCustomizedAPI eStep, structRoot *pRoot, structCycle *pCycle, CFocalLaw *pEmission, CFocalLaw *pReception);
	static UINT WINAPI sProcessAcquisitionStream(void *pAcquisitionParameter, void *pData, int iSize);
	UINT WINAPI ProcessAcquisitionStream(void *pData, int iSize);


protected:


	bool m_bAcquisitionWholeStream = false;//VERY SPECIAL PURPOSE in order to receive the whole FW stream instead of ascan, cscan and IO.
	bool m_bFifoAscanEnable = false;//can be changed to true to enable ascan fifo.
	bool m_bFifoCscanEnable = false;//can be changed to true to enable cscan fifo.
	bool m_bFifoIOEnable = false;//can be changed to true to enable cscan fifo.
	bool m_bFifoAscanDriverThread = true;//can be changed to false.
	bool m_bFifoCscanDriverThread = true;//can be changed to false.
	bool m_bFifoIODriverThread = true;//can be changed to false.
	UINT g_uiUTEventMessage = 0;
	bool g_bInitGain = false;
#ifdef _EVALUATION_
	BYTE m_cIP[4] = { 127,0,0,1 };
#else _EVALUATION_
	BYTE m_cIP[4] = { 192,168,1,11 };
#endif _EVALUATION_
	DWORD m_dwEditPort = 5001;
	bool m_bInitUsb = false;
	int m_iRadioEthernet = 0;
	bool m_bUnlockEnableShot = true;
	bool g_bFirstShotEnable = false;
	CString m_strEncoder;
	DWORD m_dwTimerID = 0;
	int m_iComboChannel = 0;
	CString m_strEditLostCount = L"";
	bool m_bCheckShotEnable;
	DWORD m_dwDigitalInputsAscan = 0;
	DWORD m_dwDigitalInputsCscan = 0;
	BOOL m_bCheckFMCCalibration = FALSE;
	bool g_bAdvanced = false;
	int m_iIOMaxTemperature = 0;

	int m_nMaxElementCount = 0;
	int	m_nMaxApertureSize = 0;
	int	m_nMaxCycleCount = 0;


	HANDLE m_hThread = NULL;
	DWORD m_dwThreadId = 0;
	DWORD WINAPI ThreadFifo(LPVOID pParam);


public:
	bool HWEnablePulser256(bool bEnable);
	bool HWEnablePulser(bool bEnable);
	bool Initialize();
	bool setAllHardwareVariables(PVOID  pParent);
	void getMaxNumberElements(CString *pBuff);
	void getMaxApertureSize(CString *pBuff);
	void getMaxCycleCount(CString *pBuff);
	bool isMultiplexer(CString * pBuff = NULL);
	void ResetCounters();
	bool Disconnect();
	bool Connect(int nIPAddress);
	bool isConnected();
	bool Close();
	void ProcessConnection();
	void getStatus();
	int getFrameCount();
	int getFrameRate();
	int getPRFRate();
	unsigned int getDataRate();
	bool setAllDacVariables(PVOID pParent);
	bool Enable(bool bStart);
	bool DriverRegistration();
	bool setAnalogueGain(float fGain);
	bool setDigitalGain(float fGain);
	bool setPulseWidth(PVOID pParent, float fPulseWidth);
	bool setTxFocalLaws(PVOID pParent);
	bool setRxFocalLaws(PVOID pParent);
	bool setWidthDelay(PVOID pParent);
	bool setRectify(PVOID pParent, int nFL = -1);
	bool setFilter(PVOID pParent, int nFL = -1);
	bool HWToFile();
	void EditFile(wchar_t * pFile, bool bCloseWaiting);
	ULONGLONG Save(CUSLFile* pFile);
	bool Retrieve(CUSLFile* pFile);
	bool SetFilter();
	bool setPRF(PVOID pParent);
};

