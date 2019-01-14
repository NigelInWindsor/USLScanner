#if !defined(AFX_IMPORTRULESDLG_H__62D325CD_42C8_44F0_800D_7697F2C18AA8__INCLUDED_)
#define AFX_IMPORTRULESDLG_H__62D325CD_42C8_44F0_800D_7697F2C18AA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportRulesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportRulesDlg dialog

class CImportRulesDlg : public CDialog
{
// Construction
public:
	bool CheckItsUnicode(CString* pFileName);
	CImportRulesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImportRulesDlg)
	enum { IDD = IDD_IMPORT_RULES_DIALOG };
	CComboBox	m_comboSimpleDest;
	CComboBox	m_comboReverselineOrder;
	CComboBox	m_comboComponentInScanner;
	CProgressCtrl	m_progressBar;
	CButton	m_buttonReadingHoles;
	CButton	m_buttonReadCoordinates;
	CButton	m_buttonHowManyLines;
	CButton	m_editConvertToUnicode;
	CComboBox	m_comboFileType;
	CComboBox	m_comboDirection;
	CComboBox	m_comboUnits;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportRulesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportRulesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboAptFiletype();
	afx_msg void OnButtonImport();
	afx_msg void OnSelchangeComboCompInScanner();
	afx_msg void OnSelchangeCombo1ReverseLineOrder();
	afx_msg void OnSelchangeComboSimpleDest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTRULESDLG_H__62D325CD_42C8_44F0_800D_7697F2C18AA8__INCLUDED_)

