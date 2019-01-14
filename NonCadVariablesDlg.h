#if !defined(AFX_NONCADVARIABLESDLG_H__9AD0A8DB_34C2_4F43_A118_9CD211F04367__INCLUDED_)
#define AFX_NONCADVARIABLESDLG_H__9AD0A8DB_34C2_4F43_A118_9CD211F04367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NonCadVariablesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNonCadVariablesDlg dialog

class CNonCadVariablesDlg : public CDialog
{
// Construction
public:
	float m_fRightWp[2];
	float m_fLeftWp[2];
	int	m_nLeftWp;
	int	m_nRightWp;
	int	m_nStartLine;
	int	m_nFinishLine;
	void UpdateAllControls();
	
	CWnd** m_pDlg;
	CNonCadVariablesDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNonCadVariablesDlg)
	enum { IDD = IDD_NON_CAD_CHANGE_DLG };
	CButton	m_checkRightWp;
	CButton	m_checkLeftWp;
	CEdit	m_editWpRightFinish;
	CEdit	m_editWpRightStart;
	CEdit	m_editWpLeftFinish;
	CEdit	m_editWpLeftStart;
	CEdit	m_editFinishLine;
	CEdit	m_editStartLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNonCadVariablesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNonCadVariablesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NONCADVARIABLESDLG_H__9AD0A8DB_34C2_4F43_A118_9CD211F04367__INCLUDED_)
