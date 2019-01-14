#if !defined(AFX_TRANSLATDLG_H__01A26472_76DB_4054_9A74_09BF016512C0__INCLUDED_)
#define AFX_TRANSLATDLG_H__01A26472_76DB_4054_9A74_09BF016512C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TranslatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTranslatDlg dialog

class CTranslatDlg : public CDialog
{
// Construction
public:
	float m_fXDev;
	float m_fYDev;
	float m_fZDev;
	CTranslatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTranslatDlg)
	enum { IDD = IDD_TRANSLATE_DIALOG };
	CEdit	m_editZDev;
	CEdit	m_editYDev;
	CEdit	m_editXDev;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTranslatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTranslatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSLATDLG_H__01A26472_76DB_4054_9A74_09BF016512C0__INCLUDED_)
