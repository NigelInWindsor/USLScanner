#if !defined(AFX_NORMALSEDITDLG_H__FC7C0A11_3F8D_4397_A7CB_B78D09E86727__INCLUDED_)
#define AFX_NORMALSEDITDLG_H__FC7C0A11_3F8D_4397_A7CB_B78D09E86727__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NormalsEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNormalsEditDlg dialog

class CNormalsEditDlg : public CDialog
{
// Construction
public:
	CWnd* m_pParent;
	CWnd** m_pDlg;
	
	int		m_nLeft;
	int		m_nTop;
	CNormalsEditDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nLeft=0,int nTop=0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNormalsEditDlg)
	enum { IDD = IDD_EDITNORMALS_DIALOG };
	CButton	m_checkCAD;
	CButton	m_checkBrain;
	CButton	m_checkTaught;
	CButton	m_checkScan;
	CButton	m_checkKNormal;
	CButton	m_checkJNormal;
	CButton	m_checkINormal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNormalsEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNormalsEditDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckINormal();
	afx_msg void OnCheckJNormal();
	afx_msg void OnCheckKNormal();
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NORMALSEDITDLG_H__FC7C0A11_3F8D_4397_A7CB_B78D09E86727__INCLUDED_)
