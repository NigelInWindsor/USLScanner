#pragma once
#include "afxwin.h"
#include "afxcmn.h"

typedef struct structInfo
{
	int iCycleCount;			// Cycle quantity for one Ascan
	unsigned int PointQuantity;	// Point quantity for one Ascan
	double *pdX;				// Tableau des points d��mergence de chaque cycle (m�tre).
	double *pdAngle;			// Tableau des angles d�attaque de chaque cycle (radian).
}structInfo;

typedef struct structSystem
{
	double dVelocity;			// C�l�rit� dans la pi�ce.
	double dDigitizingPeriod;	// P�riode de la fr�quence de num�risation.
	double dEncoderStep;		// Pas d�acquisition m�canique.
	double dEncoderLength;		//mechanical acquisition length.
	bool bInternal;				//internal synchronisation for the encoder.
	bool bCscanTofEnable;		//fw time of flight ON.
	double dTofMax;				//maximum time of flight (unit digitizing period).
	bool bGateEnable[4];		//gate.
	double dTofStart[4];		//gate start (unit digitizing period).
	double dTofRange[4];		//gate range (unit digitizing period).
}structSystem;

typedef bool (WINAPI* pTypeCallbackAscanOld)(int iSystem, int iCycle, const char* pData, int iSize);
typedef bool (WINAPI* pTypeCallbackAscan)(int iSystem, int iCycle, double dEncoder, const char* pData, int iSize);
typedef bool (WINAPI* pTypeCallbackCscan)(int iSystem, int iCycle, double dEncoder, const structCscanAmp_0x0102 *pAmp, const structCscanAmpTof_0x0202 *pAmpTof, int iSize);
typedef bool (WINAPI* pDemoCallbackInit)(int iSystem, int iCycleCount, int iPointCount);
typedef void (*pAllocDisplayCallback)(bool bAlloc);

class CRunToolbox;

// CHWDlg dialog

class CHWDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHWDlg)

public:
	CHWDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHWDlg();
	static void Display(CWnd *pWnd);
	static void Destroy();

// Dialog Data
	enum { IDD = IDD_DIALOG_HW };

	void UpdateDialog();
	void RefreshConnect();
	static CHWDlg *GetHWDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	bool RegisterObjects();
	bool UnregisterObjects();
	void OnTimer(UINT_PTR nIDEvent);
	bool SetTimer(BOOL bEnable);
	void UpdateGain();

	static CHWDlg *m_pHWDlg;
	static DWORD WINAPI CallbackThreadToolbox(HWND hWnd,CRunToolbox *pRunToolbox);

	DECLARE_MESSAGE_MAP()
public:
	// Add a CBrush* to store the new background brush for edit controls.
	CBrush* m_pEditBkBrush;
	static int m_iComboSystemCount;
	static BOOL m_bCheckToolboxInit;
	static int m_iSystemIndex;
	static int m_iCycleCount[2];
	static BOOL m_bCheckConnect;
	static double m_dEditGain;
	static double m_dGain[2];
	static BOOL m_bCheckEnable;
	static const UINT_PTR g_kTimerID = 31;
	static const UINT g_kTimerDelay = 50;//20 Hz
	static DWORD m_dwTimerID;
	static int m_iIndexLayoutFile;
	static bool m_bUpdateDialog;//to avoid crash at exit
	static BOOL m_bEnableEncoder;
	static bool m_bEnableInternal;
	static int m_iCommunicationCountError;
	static long m_lEditResolution;
	static double m_dEditLength;
	static double m_dEditThickness;
	static double m_dEditStep;
	static BOOL m_bCheckAutoResetAcq;
	static BOOL m_bCheckInternal;
	static CString m_strEditDataLostCount;
	static DWORD m_dwEditPort1;
	static BYTE m_cIP[4];

	CSpinButtonCtrl m_SpinGain;
	CComboBox m_EditGain;
	CComboBox m_ComboSystemCount;
	CButton m_ButtonConnect;
	CButton m_ButtonHWUpdate;
	CButton m_CheckToolboxInit;
	CButton m_CheckWizard;
	CButton m_ButtonLoad;
	BOOL m_bCheckWizard;
	CButton m_RadioProbe1;
	CButton m_RadioProbe2;
	CButton m_ButtonResetTime;
	CButton m_ButtonStatus;
	CButton m_CheckEnable;
	CEdit m_EditEncoder;
	CEdit m_EditDataLostCount;
	CButton m_ButtonHwSave;
	CButton m_ButtonWizardSave;
	CButton m_ButtonWizardLoad;
	CButton m_ButtonWizardSectorial;
	CButton m_EnableEncoder;
	CButton m_ButtonConfig;
	CEdit m_EditResolution;
	CEdit m_EditLength;
	CEdit m_EditThickness;
	CEdit m_EditStep;
	CButton m_CheckAutoResetAcq;
	CButton m_CheckInternal;
	CIPAddressCtrl m_IPAddress1;
	CEdit m_EditPort1;
	CButton m_ButtonHwLink;
	CButton m_ButtonToolbox;

	static structCorrectionOEMPA* WINAPI CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
	static UINT WINAPI AcquisitionAscan1_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
	static UINT WINAPI AcquisitionAscan2_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
	static void DumpFileData(CStdioFile &file);
	static int m_iAcquisitionDataCount[g_iOEMPACycleCountMax];
	static int m_iAcquisitionDataSize[g_iOEMPACycleCountMax];
	static enumBitSize m_eAcquisitionDataBitSize[g_iOEMPACycleCountMax];
	static bool m_bAcquisitionDataSigned[g_iOEMPACycleCountMax];
	static int m_iAcquisitionCycleCount;
	static WORD *m_pAcquisitionDataMax[g_iOEMPACycleCountMax];
	static CRITICAL_SECTION m_CriticalSection;
	static bool m_bInitCriticalSection;

	afx_msg LRESULT OnEventMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonHwUpdate();
	afx_msg void OnBnClickedCheckToolboxInit();
	afx_msg void OnBnClickedCheckConnect();
	afx_msg void OnBnClickedButtonHwLoad();
	afx_msg void OnDeltaposSpinGain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnKillfocusEditGain();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonHwSave();
	afx_msg void OnBnClickedRadioProbe1();
	afx_msg void OnBnClickedRadioProbe2();
	afx_msg void OnBnClickedButtonResetTimestamp();
	afx_msg void OnBnClickedButtonStatus();
	afx_msg void OnBnClickedCheckEnable();
	afx_msg void OnBnClickedButtonToolbox();
	afx_msg void OnBnClickedButtonScanSectorial();
	afx_msg void OnBnClickedButtonSystemSave();
	afx_msg void OnBnClickedButtonSystemLoad();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedCheckEncoder();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonConfig();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCheckAutoResetacq();
	afx_msg void OnBnClickedCheckInternal();
	afx_msg void OnBnClickedButtonCalibration();
	afx_msg void OnBnClickedCheckHwLink();
	afx_msg void OnBnClickedCheckFillingBetween();
	afx_msg void OnBnClickedCheckFillingLast();
//#ifdef _ENABLE_DISABLE_FILLING
	CButton m_CheckFilingBetween;
	CButton m_CheckFilingLast;
//#endif _ENABLE_DISABLE_FILLING
};
