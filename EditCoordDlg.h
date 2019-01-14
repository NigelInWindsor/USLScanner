#if !defined(AFX_EDITCOORDDLG_H__752C435A_0FE0_4367_9923_C528BB3E0D84__INCLUDED_)
#define AFX_EDITCOORDDLG_H__752C435A_0FE0_4367_9923_C528BB3E0D84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditCoordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditCoordDlg dialog

class CEditCoordDlg : public CDialog
{
// Construction
public:
	CString m_Text;
	CString m_Action;
	void SetText(CString Text,CString Action);
	float m_fValue;
	int	m_nAxis;
	CWnd* m_pParent;
	CEditCoordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditCoordDlg)
	enum { IDD = IDD_EDIT_COORD_DIALOG };
	CButton	m_buttonTranslate;
	CStatic	m_editAction;
	CStatic	m_editText;
	CEdit	m_editValue;
	CComboBox	m_comboWhichAxis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditCoordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditCoordDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonTranslate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCOORDDLG_H__752C435A_0FE0_4367_9923_C528BB3E0D84__INCLUDED_)
