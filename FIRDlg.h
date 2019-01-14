#if !defined(AFX_FIRDLG_H__9FAC5867_F370_4831_AD08_9BA063DD02CE__INCLUDED_)
#define AFX_FIRDLG_H__9FAC5867_F370_4831_AD08_9BA063DD02CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FIRDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFIRDlg dialog

class CFIRDlg : public CDialog
{
// Construction
public:
	CWnd** m_pDlg;
	
	CFIRDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFIRDlg)
	enum { IDD = IDD_FIR_DIALOG };
	CStatic	m_staticPhase;
	CStatic	m_staticFrequency;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFIRDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFIRDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIRDLG_H__9FAC5867_F370_4831_AD08_9BA063DD02CE__INCLUDED_)
