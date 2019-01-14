#if !defined(AFX_PERMISSIONDLG_H__BAD78CF7_F57C_4CA1_9919_E812BDD4D6C3__INCLUDED_)
#define AFX_PERMISSIONDLG_H__BAD78CF7_F57C_4CA1_9919_E812BDD4D6C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PermissionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPermissionDlg dialog

class CPermissionDlg : public CDialog
{
// Construction
public:
	bool m_bAcceptable;
	CPermissionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPermissionDlg)
	enum { IDD = IDD_PERMISSION_DIALOG };
	CEdit	m_editCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPermissionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPermissionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERMISSIONDLG_H__BAD78CF7_F57C_4CA1_9919_E812BDD4D6C3__INCLUDED_)
