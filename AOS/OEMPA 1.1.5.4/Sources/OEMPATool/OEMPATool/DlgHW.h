#pragma once
#include "afxwin.h"
#include "afxcmn.h"

const int g_iOEMMCProbeCountMax=4;
const int g_iOEMMCScanCountMax=16;

class CHWDevice;
class CUTKernelCom;
class CRunToolbox;

// CDlgHW dialog

class CDlgHW : public CDialog
{
	DECLARE_DYNAMIC(CDlgHW)

public:
	CDlgHW(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHW();
	static void Display();
	static int GetDeviceId();
	static void Destroy();
	static void DisplayEncoder();
	static void UpdateDialog();
	static CHWDeviceOEMPA *GetKernelDriverOEMPA();

// Dialog Data
	enum { IDD = IDD_DIALOG_HW };

protected:

	static CDlgHW *m_pDlgHW;
	void UpdateDialog_();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	static CHWDeviceOEMPA *m_pHWDeviceOEMPA;
	static HANDLE m_hThread;
	static DWORD m_dwThreadId;
	static DWORD WINAPI ThreadFifo(LPVOID pParam);
public:
	static bool m_bAcquisitionWholeStream;
protected:
	static bool m_bFifoAscanEnable;
	static bool m_bFifoCscanEnable;
	static bool m_bFifoIOEnable;
	static bool m_bFifoAscanDriverThread;
	static bool m_bFifoCscanDriverThread;
	static bool m_bFifoIODriverThread;

	bool DriverRegistration();
	bool UpdateDriver();
	void UpdateFMC(int iCycleCount);

	bool HWReadCycleCount(int &iCycleCount);
	bool HWReadFMC();
	bool HWWriteGain(int iCycle,double &dGain);
	bool HWWriteGain256(int iCycle,double &dGain);
	bool HWReadGain(int iCycle,double &dGain);
	bool HWWriteGainAll(double adGain[g_iOEMPACycleCountMax]);
	bool HWWriteGainAll256(double adGain[g_iOEMPACycleCountMax]);
	bool HWReadGainAll(double adGain[g_iOEMPACycleCountMax],int &iCycleCount);
	bool HWEnablePulser(bool bEnable);
	bool HWEnablePulser256(bool bEnable);
	bool UpdateCycle();
	bool UpdateGain();
	bool IsGainModified();
	void UpdateGain_();
	void WriteGainHW();
	void FileSamples(bool bWizard);
	bool IsKernelObject(wchar_t *wcObjectName,enumRootType eInRootID,int &iKernelObjectId);
	bool SetTriggerMode(enumOEMPATrigger eTrig);
	bool SetTimer(bool bEnable);
	void UpdateComboChannel();
	bool ProcessMultiChannel(structAcqChannelInfo &chInfo,structAcqInfoEx &acqInfo,int &iCycle);
	void ResetCounters();
	void StatusExtra(CHWDeviceOempa *pHWDeviceOempa,CString &string);
	void ProcessConnection();
	bool Loadfile(CString strFileName);
	void TestOEMMC1();
	void TestOEMMC2();

	DECLARE_MESSAGE_MAP()
public:
	static DWORD WINAPI CallbackThreadToolbox(HWND hWnd,CRunToolbox *pRunToolbox);
	static UINT WINAPI ProcessAcquisitionAscan_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
	static UINT WINAPI ProcessAcquisitionCscan_0x00010X02(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof);
	static UINT WINAPI ProcessAcquisitionIO_1x00010101(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader);
	static structCorrectionOEMPA* WINAPI CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
	static UINT WINAPI sProcessAcquisitionStream(void *pAcquisitionParameter,void *pData,int iSize);
	UINT WINAPI ProcessAcquisitionStream(void *pData,int iSize);
	static void UpdateEncoderDebugUSB3(unsigned int &iSize1,unsigned int &iSize2);
	static const UINT_PTR g_kTimerID = 31;
	static const UINT g_kTimerDelay = 1000;
	static DWORD m_dwTimerID;
	static double m_adGain[g_iOEMPACycleCountMax];
	static double m_dEditGain;
	static int m_iComboCycle;
	static int m_iCycleCount;
	static BOOL m_bCheckShotEnable;
	static double m_dEditAxisStartRead;
	static double m_dEditAxisStartWrite;
	static BYTE m_cIP[4];
	static DWORD m_dwIPAddress;
	static DWORD m_dwEditPort;
	static bool m_bInitUsb;
	static int m_iRadioEthernet;
	static BOOL m_bCheckDisplayImage;
	static BOOL m_bCheckDisplayInvert;
	static BOOL m_bEnableCorrection;
	static BOOL m_bCheckWizardEnable;
	static int m_iIndexLayoutFile;
	static bool m_bUnlockEnableShot;
	static bool g_bFirstShotEnable;//the user need to set the shot enable by himself.
	static CString m_strEncoder;
	static BOOL m_bCheckDisplayMax;
	static int m_iComboChannel;
	static int m_iChannelProbe,m_iChannelScan;
	static CRITICAL_SECTION m_ChannelCriticalSection;
	static int m_iChannelCount,m_iChannelProbeCount;
	static int m_iChannelScanCount[g_iOEMMCProbeCountMax];
	static int m_iChannelCycleCount[g_iOEMMCProbeCountMax][g_iOEMMCScanCountMax];//first index Probe, second index Scan.
	static CString m_strEditLostCount;
	static CRITICAL_SECTION m_CriticSectionCounters;
	static ULONGLONG m_ullDataSizeAscan,m_ullDisplayDataSizeAscan;
	static ULONGLONG m_ullDataSizeCscan,m_ullDisplayDataSizeCscan;
	static ULONGLONG m_ullAscanCount;
	static double m_dEncoderAscan[g_iDriverEncoderCountMax];
	static double m_dEncoderCscan[g_iDriverEncoderCountMax];
	static DWORD m_dwDigitalInputsAscan;
	static DWORD m_dwDigitalInputsCscan;
	static DWORD m_dwEncoderAscanTickCount;
	static DWORD m_dwEncoderCscanTickCount;
	static LARGE_INTEGER m_llEncoderAscanTickCount;
	static LARGE_INTEGER m_llEncoderCscanTickCount;
	static DWORD m_dwDisplayTickCount;
	static bool m_bFullMatrixCapture;
	static bool m_bMultiHWChannel;
	static BYTE m_abyMultiChannelDecimation[g_iOEMPACycleCountMax];
	static float m_afMultiChannelStart[g_iOEMPACycleCountMax];
	static float m_fMultiChannelPeriod;
	static int m_iFMCElementStart;
	static int m_iFMCElementStop;
	static int m_iFMCElementStep;
	static int m_iFMCElementCount;
	static BOOL m_bCheckFMCCalibration;
	static BOOL m_bCheckFileFormatAddressData;
	static int m_iIOMaxTemperature;
	bool m_bGainKillfocusEnable;
	bool m_bInitDialog;
	CString m_strLayoutTitle;
	COLORREF m_bkColor;
	bool m_bkColorInit;
	BOOL m_bCheckTest;

	CIPAddressCtrl m_IPAddress;
	CEdit m_EditPort;
	BOOL m_bCheckConnect;
	DWORD m_dwEditError;
	CComboBox m_ComboUsb;
	CString m_strComboUsb;
	CButton m_RadioEthernet;
	CButton m_RadioUsb;
	CButton m_CheckShotEnable;
	CButton m_ButtonAPISamples;
	CButton m_ButtonFileToHW;
	CButton m_ButtonHWToFile;
	CButton m_CheckDisplayImage;
	CButton m_CheckDisplayInvert;
	CButton m_ButtonWizardToHW;
	CButton m_CheckDisplayMax;
	CButton m_EditButtonOutput;
	CButton m_CheckTest;
	CSpinButtonCtrl m_SpinGain;
	CEdit m_EditGain;
	CComboBox m_ComboCycle;
	CButton m_ButtonColor;
	CButton m_ButtonReset;
	CButton m_ButtonStatus;
	CRichEditCtrl m_EditEncoder;
	CComboBox m_ComboChannel;
	CStatic m_StaticChannel;
	CStatic m_StaticCycle;
	CEdit m_EditDataLost;
	CBrush* m_pEditBkBrush;
	CButton m_ButtonToolbox;
	CButton m_EditCalibration;
	CStatic m_StaticGainUnit;
	CButton m_CheckFMCCalibration;
	CButton m_CheckFileFormatAddressData;
	CEdit m_EditError;

	afx_msg LRESULT OnEventMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose( );
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnKillfocusEditGain();
	afx_msg void OnEnKillfocusEditA();
	afx_msg void OnBnClickedCheckPulserEnable();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedCheckConnect();
	afx_msg void OnBnClickedCheckConnectionCallback();
	afx_msg void OnBnClickedRadioEthernet();
	afx_msg void OnBnClickedRadioUsb();
	afx_msg void OnDeltaposSpinGain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonFileOempaLoad();
	afx_msg void OnBnClickedButtonFileOempaSave();
	afx_msg void OnCbnSelchangeComboDisplayCycle();
	afx_msg void OnBnClickedCheckDisplayImage();
	afx_msg void OnBnClickedCheckDisplayInvert();
	afx_msg void OnBnClickedButtonWizardToHw();
	afx_msg void OnBnClickedButtonDisplayGrid();
	afx_msg void OnBnClickedButtonDisplayColor();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonStatus();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnEnKillfocusIPAddress();
	afx_msg void OnEnKillfocusEditPort();
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedCheckDisplayMax();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboChannel();
	afx_msg void OnBnClickedButtonMulti();
	afx_msg void OnBnClickedButtonOutput();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonCalibration();
	afx_msg void OnBnClickedCheckTest();
	afx_msg void OnBnClickedButtonFileOempaLoad2();
	BOOL m_bCheckUSB3Simulator;
	afx_msg void OnBnClickedCheckUsb3Simulator();
	CButton m_CheckUSB3Simulator;
	afx_msg void OnBnClickedCheckFileAddDataFormat();
	CButton m_ButtonResetUSB3;
	afx_msg void OnBnClickedButtonResetUsb3();
};
