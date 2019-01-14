
// OEMPACustomizedExampleDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// COEMPACustomizedExampleDlg dialog
class COEMPACustomizedExampleDlg : public CDialog
{
// Construction
public:
	COEMPACustomizedExampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OEMPACUSTOMIZEDEXAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	void UpdateDialog();
	void UpdateElementList(CFocalLaw *pFocalLaw,int aiElement[g_iOEMPAApertureElementCountMax]);
	void UpdateElementList(int aiElement[g_iOEMPAApertureElementCountMax],CFocalLaw *pFocalLaw);
	void UpdateEdit();
	void UpdateDataFormEdit();
	bool Load();
	bool Save();
	bool SetTimer(bool bEnable);
	static structCorrectionOEMPA* WINAPI CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
	static DWORD WINAPI ThreadLoad(LPVOID pParam);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	typedef enum enumWizardStep{eWizardDisconnected,eWizardConnected,eWizardFile,eWizardInput,eWizardOutput,eWizardFinish,eWizardLoad};

	CHWDeviceOEMPA m_HWDeviceOEMPA;
	CHWDeviceOEMPA *m_pHWDeviceOEMPA;
	enumWizardStep m_eWizardStep;
	static const UINT_PTR g_kTimerID = 31;
	static const UINT g_kTimerDelay = 1000;
	DWORD m_dwTimerID;
	BYTE m_cIP[4];
	HANDLE m_hThread;
	DWORD m_dwThreadId;
	CRITICAL_SECTION m_CriticalSection;
	enumStepCustomizedAPI m_eLoadStep;
	bool m_bReadFileWriteHW;
	structRoot *m_pRoot;
	structCycle *m_pCycle;
	CFocalLaw *m_pEmission,*m_pReception;
	CFocalLaw m_EmissionIn,m_ReceptionIn;
	CFocalLaw m_EmissionOut,m_ReceptionOut;
	float m_fBeamCorrectionIn,m_fBeamCorrectionOut;

	CIPAddressCtrl m_IPAddress;
	CButton m_CheckConnect;
	BOOL m_bCheckConnect;
	CButton m_ButtonWizardPrevious;
	CButton m_ButtonWizardNext;
	CButton m_RadioReception;
	CButton m_RadioEmission;
	BOOL m_iRadioReception;
	CString m_strStaticWizard1;
	CString m_strStaticWizard2;
	CString m_strStaticDisplay;
	CString m_strStaticAperture;
	CString m_strStaticApodisation;
	CEdit m_EditAperture;
	CEdit m_EditApodisation;
	CEdit m_EditBeamCorrection;
	CString m_strStaticBeamCorrection;
	CString m_strEditAperture;
	CString m_strEditApodisation;
	CString m_strEditBeamCorrection;
	CEdit m_EditFile;
	CString m_strEditFile;
	CStatic m_StaticData;

	afx_msg void OnClickedCheckConnect();
	afx_msg void OnClickedButtonWizardPrevious();
	afx_msg void OnClickedButtonWizardNext();
	afx_msg void OnRadioEmission();
	afx_msg void OnClickedRadioReception();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnKillfocusEditAperture();
	afx_msg void OnEnKillfocusEditApodisation();
	afx_msg void OnEnKillfocusEditBeamCorrection();
	afx_msg void OnBnClickedOk();
	float m_fEditBeamCorrection;
};
