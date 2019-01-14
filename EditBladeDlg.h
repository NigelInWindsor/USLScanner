#if !defined(AFX_EDITBLADEDLG_H__A28DBA73_757C_4487_82D1_1B567D74868F__INCLUDED_)
#define AFX_EDITBLADEDLG_H__A28DBA73_757C_4487_82D1_1B567D74868F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditBladeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditBladeDlg dialog

class CEditBladeDlg : public CDialog
{
// Construction
public:
	void SetAccessPrivelage();
	void ImportOverlay(int nOutsideInside,int nWhichThickness);
	void UpdateParent(int nAction);
	
	void UpdateAllControls();
	CWnd* m_pParent;
	CWnd** m_pDlg;
	int		m_nLeft;
	int		m_nTop;
	CEditBladeDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nLeft = 0, int nTop = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditBladeDlg)
	enum { IDD = IDD_EDITBLADE_DIALOG };
	CButton	m_checkRotate;
	CButton	m_checkDecimate;
	CEdit	m_editFastIncrement;
	CButton	m_buttonMin;
	CButton	m_buttonMax;
	CEdit	m_editLineNumber;
	CSpinButtonCtrl	m_spinLineNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditBladeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditBladeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditLineNumber();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonImportMax();
	afx_msg void OnButtonImportNom();
	afx_msg void OnButtonImportMin();
	afx_msg void OnButtonAdd180Degrees();
	afx_msg void OnButtonDecimate();
	afx_msg void OnButtonCopyToScanlines();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITBLADEDLG_H__A28DBA73_757C_4487_82D1_1B567D74868F__INCLUDED_)
