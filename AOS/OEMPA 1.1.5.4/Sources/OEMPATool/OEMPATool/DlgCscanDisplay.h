#pragma once
#include "afxwin.h"


// CDlgCscanDisplay dialog

class CDlgCscanDisplay : public CDialog
{
	DECLARE_DYNAMIC(CDlgCscanDisplay)

public:
	CDlgCscanDisplay(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCscanDisplay();

	static void Display();
	static bool IsDisplayed();
	void UpdateDialog(bool bForce=true);
	static void _UpdateDialog();
	static UINT WINAPI ProcessAcquisitionAscan_0x00010103(int iAscanCycle,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
	static UINT WINAPI ProcessAcquisitionCscan_0x00010X02(int iCscanCycle,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof);
	static structCorrectionOEMPA* WINAPI CallbackCustomizedAPI(CHWDeviceOEMPA *pOEMPA,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
	static void OnTimer();

	static bool IsDisplayEnabled(){if(!m_pDlg) return false; return (m_pDlg->m_bCheckEnableCscan && m_bCheckDisplay)?true:false;};
	static bool IsHwGate(){return !m_iRadioHw?true:false;};

// Dialog Data
	enum { IDD = IDD_DIALOG_CSCAN_DISPLAY };

protected:
	static CDlgCscanDisplay *m_pDlg;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	static bool m_bNewSetup;
	static bool m_bUpdate;
	static double m_dAscanStart[g_iOEMPACycleCountMax];
	static double m_dAscanRange[g_iOEMPACycleCountMax];
	static int m_iCycleCount;
	static double m_dDigitizingPeriod;
	static bool m_bCallbackCalled;
	static bool m_bEnableAscan;
	static bool m_bEnableCscanTof;
	static int m_iIndexLayoutFile;
	CString m_strLayoutTitle;

	//setting to compute software cscan.
	static BOOL m_bCheckDisplay;
	static int m_iRadioHw;
	static int m_iComboGate;
	static int m_iEditPointCount;
	static int m_aiPointCount[g_iOEMPACycleCountMax];
	static bool m_abGateEnableSW[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static bool m_abGateEnableHW[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static bool m_abGateSign[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static enumGateModeAmp m_aeGateModeAmp[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static double m_adGateStart[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static double m_adGateStop[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static int m_aiGateStart[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static int m_aiGateStop[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static int m_aiAcquisitionSWAmpCount[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static CRITICAL_SECTION m_CriticSection;

	static ULONGLONG m_ullUpdateSWCount;
	static LONGLONG m_llAcquisitionSWAmpSequenceIndex0;
	static BYTE *m_apAcquisitionSWAmp[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static int m_aiAcquisitionSWAmpLastIndex[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static LONGLONG m_llAcquisitionHWAmpSequenceIndex0;

	static ULONGLONG m_ullUpdateHWCount;
	static int m_aiAcquisitionHWAmpCount[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static BYTE *m_apAcquisitionHWAmp[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	static int m_aiAcquisitionHWAmpLastIndex[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];

	static bool m_bEncoderEnable;
	static double m_dEncoderStep;
	static bool m_bEnableCscan;
	
	BOOL m_bCheckEnableCscan;
	CButton m_CheckEnableCscan;
	CButton m_CheckDisplay;
	CButton m_RadioSW;
	CButton m_RadioHW;
	CEdit m_EditPointCount;
	CComboBox m_ComboGate;
	CButton m_ButtonReset;

	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckDisplay();
	afx_msg void OnCbnSelchangeComboGate();
	afx_msg void OnEnKillfocusEditPointCount();
	afx_msg void OnBnClickedRadioHw();
	afx_msg void OnBnClickedRadioSw();
	afx_msg void OnBnClickedCheckEnableCscan();
	afx_msg void OnBnClickedButtonReset();
};
