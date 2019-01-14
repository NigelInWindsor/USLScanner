#if !defined(AFX_ORIENTATIONDLG_H__8EA5EE2A_BE9C_4E9A_A58B_D4935683B3F3__INCLUDED_)
#define AFX_ORIENTATIONDLG_H__8EA5EE2A_BE9C_4E9A_A58B_D4935683B3F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OrientationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COrientationDlg dialog

class COrientationDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(COrientationDlg)

// Construction
public:
	void 	SetRegistry();
	COrientationDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(COrientationDlg)
	enum { IDD = IDD_ORIENTATION_DIALOG };
	CEdit	m_editRightName;
	CEdit	m_editLeftName;
	CComboBox	m_comboUnits;
	CComboBox	m_comboOrientation;
	CComboBox	m_comboPrimarySide;
	CComboBox	m_comboLanguage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrientationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrientationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLanguage();
	afx_msg void OnSelchangeComboPrimarySide();
	afx_msg void OnSelchangeComboSide0Orientation();
	afx_msg void OnSelchangeComboUnits();
	afx_msg void OnChangeEditRightName();
	afx_msg void OnChangeEditLeftName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORIENTATIONDLG_H__8EA5EE2A_BE9C_4E9A_A58B_D4935683B3F3__INCLUDED_)
