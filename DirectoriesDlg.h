#if !defined(AFX_DIRECTORIESDLG_H__329F3416_3202_49DB_A4C5_52F9624AF27A__INCLUDED_)
#define AFX_DIRECTORIESDLG_H__329F3416_3202_49DB_A4C5_52F9624AF27A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirectoriesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDirectoriesDlg dialog

class CDirectoriesDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CDirectoriesDlg)

// Construction
public:
	HBRUSH m_hBr;
	void GetDirectory(CEdit* pEdit,CString *pInitDir);
	int m_nFocus;
	CDirectoriesDlg(CWnd* pParent = NULL);   // standard constructor
	~CDirectoriesDlg();

// Dialog Data
	//{{AFX_DATA(CDirectoriesDlg)
	enum { IDD = IDD_DIRECTORIES_DIALOG };
	CEdit	m_editWorkSpacePath;
	CEdit	m_editCalibrationPath;
	CEdit	m_editWordPadPathName;
	CEdit	m_editUSLScannerManualPathName;
	CEdit	m_editPmacManualPathName;
	CEdit	m_editAcrobatPathName;
	CEdit	m_editAndonPathName;
	CEdit	m_editDataBaseFail;
	CEdit	m_editProbesPath;
	CEdit	m_editHelpPath;
	CEdit	m_editDataBase;
	CButton	m_checkLastAccessed;
	CEdit	m_editUltrasonicsPath;
	CEdit	m_editScanPath;
	CEdit	m_editPalettePath;
	CEdit	m_editAptSourcePath;
	CEdit	m_editImagespath;
	CEdit	m_editProfilesPath;
	CEdit	m_editLastSettingsPath;
	//}}AFX_DATA
	CCheckListBox	m_listStorePath;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirectoriesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDirectoriesDlg)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnButtonBrowse();
	afx_msg void OnSetfocusEditLastsettingsPath();
	afx_msg void OnSetfocusEditProfilesPath();
	afx_msg void OnChangeEditLastsettingsPath();
	afx_msg void OnChangeEditProfilesPath();
	afx_msg void OnSetfocusEditImagesPath();
	afx_msg void OnChangeEditImagesPath();
	afx_msg void OnChangeEditAptsourcePath();
	afx_msg void OnSetfocusEditAptsourcePath();
	afx_msg void OnButtonLastsettings();
	afx_msg void OnButtonProfile();
	afx_msg void OnChangeEditPalettePath();
	afx_msg void OnChangeEditScanPath();
	afx_msg void OnChangeEditUltrasonicsPath();
	afx_msg void OnButtonPalettes();
	afx_msg void OnButtonImages();
	afx_msg void OnButtonAptsource();
	afx_msg void OnButtonScan();
	afx_msg void OnButtonUltrasonics();
	afx_msg void OnCheckLastaccessed();
	afx_msg void OnButtonDataBase();
	afx_msg void OnChangeEditDataBase();
	afx_msg void OnChangeEditHelpFile();
	afx_msg void OnButtonHelpFile();
	afx_msg void OnChangeEditProbesPath();
	afx_msg void OnButtonProbes();
	afx_msg void OnCheckChangeListStorePath();
	afx_msg void OnButtonDataBaseFail();
	afx_msg void OnChangeEditDataBaseFail();
	afx_msg void OnButtonAndon();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonAcrobat();
	afx_msg void OnButtonUslscannermanual();
	afx_msg void OnButtonPmacmanual();
	afx_msg void OnChangeEditAcrobatPathname();
	afx_msg void OnChangeEditUslscannerManualPathname();
	afx_msg void OnChangeEditPmacManualPathname();
	afx_msg void OnChangeEditWordpadPathname();
	afx_msg void OnButtonWordpad();
	afx_msg void OnChangeEditCalibrationPath();
	afx_msg void OnButtonCalibration();
	afx_msg void OnButtonWorkspace();
	afx_msg void OnChangeEditWorkspacePath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editWorkingDirectory;
	CEdit m_editRegistryKey;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRECTORIESDLG_H__329F3416_3202_49DB_A4C5_52F9624AF27A__INCLUDED_)
