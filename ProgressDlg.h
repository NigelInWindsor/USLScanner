#if !defined(AFX_PROGRESSDLG_H__AD58A72D_F225_451D_9FF1_92E2F37874CC__INCLUDED_)
#define AFX_PROGRESSDLG_H__AD58A72D_F225_451D_9FF1_92E2F37874CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

class CProgressDlg : public CDialog
{
// Construction
public:
	void SetProgressText(CString String);
	void SetProgressRange(int nMin,int nMax);
	void SetProgressPos(int nPos);
	int m_nMax;
	int m_nMin;
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressDlg)
	enum { IDD = IDD_PROGRESS_DIALOG };
	CProgressCtrl	m_progressBar;
	CString	m_stringText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__AD58A72D_F225_451D_9FF1_92E2F37874CC__INCLUDED_)
