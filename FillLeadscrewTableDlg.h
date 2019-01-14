#if !defined(AFX_FILLLEADSCREWTABLEDLG_H__8693D3A1_505C_4C95_9846_60B932FCBBDE__INCLUDED_)
#define AFX_FILLLEADSCREWTABLEDLG_H__8693D3A1_505C_4C95_9846_60B932FCBBDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FillLeadscrewTableDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFillLeadscrewTableDlg dialog

class CFillLeadscrewTableDlg : public CDialog
{
// Construction
public:
	int m_nAxis;
	CWnd* m_pParent;
	CFillLeadscrewTableDlg(CWnd* pParent = NULL, int nAxis = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFillLeadscrewTableDlg)
	enum { IDD = IDD_FILL_LEADSCREW_TABLE_DLG };
	CEdit	m_editStepsize;
	CEdit	m_editMaxPos;
	CEdit	m_editMinPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFillLeadscrewTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFillLeadscrewTableDlg)
	afx_msg void OnButtonApply();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILLLEADSCREWTABLEDLG_H__8693D3A1_505C_4C95_9846_60B932FCBBDE__INCLUDED_)
