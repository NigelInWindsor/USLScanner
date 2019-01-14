#if !defined(AFX_CTRLPROPERTIESDLG_H__EFF4DB7C_C5D2_48A4_8178_DBBA44A4A662__INCLUDED_)
#define AFX_CTRLPROPERTIESDLG_H__EFF4DB7C_C5D2_48A4_8178_DBBA44A4A662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CtrlPropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCtrlPropertiesDlg dialog

class CCtrlPropertiesDlg : public CDialog
{
// Construction
public:
	int m_nBorderStyle;
	void SetAccessPrivelages();
	void UpdateAllControls();
	
	int m_nIndex;
	CWnd* m_pParent;
	CWnd** m_pDlg;
	CCtrlPropertiesDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nIndex = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCtrlPropertiesDlg)
	enum { IDD = IDD_CTRL_PROPERTIES_DLG };
	CButton	m_checkFileName;
	CButton	m_checkFullPathName;
	CButton	m_checkPrint;
	CComboBox	m_comboAlignText;
	CButton	m_checkBorder;
	CButton	m_checkReadOnly;
	CButton	m_checkUpdateTime;
	CButton	m_checkUpdateOperator;
	CButton	m_checkUpdateDate;
	CButton	m_checkClearLevel3;
	CButton	m_checkClearLevel2;
	CButton	m_checkClearLevel1;
	CEdit	m_editCaption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtrlPropertiesDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCtrlPropertiesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditCaption();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnCheckClearLevel1();
	afx_msg void OnCheckClearLevel2();
	afx_msg void OnCheckClearLevel3();
	afx_msg void OnCheckUpdateDate();
	afx_msg void OnCheckUpdateOperator();
	afx_msg void OnCheckTime();
	afx_msg void OnCheckReadOnly();
	afx_msg void OnCheckBorder();
	afx_msg void OnSelchangeComboAlignText();
	afx_msg void OnCheckPrint();
	afx_msg void OnCheckFilename();
	afx_msg void OnCheckDisplayFullPath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTRLPROPERTIESDLG_H__EFF4DB7C_C5D2_48A4_8178_DBBA44A4A662__INCLUDED_)
