#if !defined(AFX_LOGONDLG_H__A5C4AC7A_B2C3_4F1A_BAA9_8F536EA10E5E__INCLUDED_)
#define AFX_LOGONDLG_H__A5C4AC7A_B2C3_4F1A_BAA9_8F536EA10E5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg dialog

class CLogonDlg : public CDialog
{
// Construction
public:
	CLogonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogonDlg)
	enum { IDD = IDD_LOGON_DIALOG };
	CEdit	m_editPassword;
	CEdit	m_editName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogonDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGONDLG_H__A5C4AC7A_B2C3_4F1A_BAA9_8F536EA10E5E__INCLUDED_)
