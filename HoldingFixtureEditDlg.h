#if !defined(AFX_HOLDINGFIXTUREEDITDLG_H__332DFD9B_CFD8_41AF_B102_866B40968146__INCLUDED_)
#define AFX_HOLDINGFIXTUREEDITDLG_H__332DFD9B_CFD8_41AF_B102_866B40968146__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoldingFixtureEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHoldingFixtureEditDlg dialog

class CHoldingFixtureEditDlg : public CDialog
{
// Construction
public:
	CFont* m_pFont;
	
	int	m_nLeft;
	int	m_nTop;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	void UpdateAllControls();
	CHoldingFixtureEditDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nLeft=0,int nTop=0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHoldingFixtureEditDlg)
	enum { IDD = IDD_EDITHOLDINGFIXTURE_DIALOG };
	CEdit	m_editFixedX;
	CEdit	m_editZMoving;
	CEdit	m_editYMoving;
	CEdit	m_editXMoving;
	CEdit	m_editYFixed;
	CEdit	m_editZFixed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoldingFixtureEditDlg)
	public:
	virtual void OnSetFont(CFont* pFont);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHoldingFixtureEditDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStore();
	afx_msg void OnButtonGoto();
	afx_msg void OnChangeEditfixedY();
	afx_msg void OnChangeEditFixedZ();
	afx_msg void OnChangeEditmovingX();
	afx_msg void OnChangeEditmovingY();
	afx_msg void OnChangeEditmovingZ();
	afx_msg void OnButtonGotopark();
	afx_msg void OnChangeEditfixedX();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLDINGFIXTUREEDITDLG_H__332DFD9B_CFD8_41AF_B102_866B40968146__INCLUDED_)
