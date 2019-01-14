#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CRunToolbox;

// CHWDlg dialog

class CHWDlg : public CDialog
{
	DECLARE_DYNAMIC(CHWDlg)

public:
	CHWDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHWDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_HW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	void UpdateDialog();
	bool SetTimer(bool bEnable);
	bool TemplateToOEMPAFile(const wchar_t *pOEMPAFile);
	bool TemplateToWizard();
	void OnBnClickedRadioChannel();

	DECLARE_MESSAGE_MAP()
public:
	static BOOL m_bCheckConnect;
	static const UINT_PTR g_kTimerID = 31;
	static const UINT g_kTimerDelay = 1000;
	static DWORD m_dwTimerID;
	static BYTE m_cIP[4];
	static BOOL m_iRadioSingleChannel;
	static bool m_bIsModified;
	static bool m_bIsChannelUpToDate;

	CButton m_CheckConnect;
	CIPAddressCtrl m_IPAddress;
	CButton m_ButtonOEMPAFileToHw;
	CButton m_ButtonHwToOEMPAFile;
	CButton m_ButtonTemplateToUpdateScan;
	CButton m_ButtonTemplateToWizard;
	CButton m_ButtonWizardToTemplate;
	CButton m_ButtonKernelFileToWizard;
	CButton m_ButtonWizardToKernelFile;
	CButton m_ButtonEdit;
	CButton m_ButtonUpdateScans;
	CButton m_ButtonWizardToHw;
	CButton m_ButtonWizardToOempaFile;
	CButton m_ButtonWizardToKernelFileFull;
	CButton m_ButtonHwToWizard;
	CButton m_ButtonOempaFileToWizard;

	static DWORD WINAPI CallbackThreadToolboxEdit(HWND hWnd,CRunToolbox *pRunToolbox);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnEventMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedCheckConnect();
	afx_msg void OnBnClickedButtonWizardToHw();
	afx_msg void OnBnClickedButtonOEMPAFileToHw();
	afx_msg void OnBnClickedButtonHwToOEMPAFile();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonTemplateToUpdateScan();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonWizardToKernelFile();
	afx_msg void OnBnClickedButtonKernelFileToWizard();
	afx_msg void OnBnClickedButtonTemplateToWizard();
	afx_msg void OnBnClickedButtonWizardToTemplate();
	afx_msg void OnBnClickedButtonUpdateScan();
	afx_msg void OnBnClickedRadioSingleChannel();
	afx_msg void OnBnClickedRadioMultiChannel();
	afx_msg void OnBnClickedButtonTemplateView();
	afx_msg void OnBnClickedButtonWizardToOempaFile();
	afx_msg void OnBnClickedButtonWizardToKernelFileFull();
	afx_msg void OnBnClickedButtonOempaFileOptions();
};
