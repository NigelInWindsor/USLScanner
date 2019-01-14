#if !defined(AFX_RECTILINEARSCANPAGE_H__5D89135A_3C84_416E_BCD9_B3F27D21180E__INCLUDED_)
#define AFX_RECTILINEARSCANPAGE_H__5D89135A_3C84_416E_BCD9_B3F27D21180E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RectilinearScanPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRectilinearScanPage dialog

class CRectilinearScanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRectilinearScanPage)

// Construction
public:
	HBRUSH m_hBr;
	int	m_nOldScanLine;
	void SetAccessPrivelage();
	
	void UpdateAllControls();
	CRectilinearScanPage();
	~CRectilinearScanPage();

// Dialog Data
	//{{AFX_DATA(CRectilinearScanPage)
	enum { IDD = IDD_RECTILINEAR_SCAN_PAGE };
	CComboBox	m_comboBiUniDirectional;
	CButton	m_buttonRightToLeft;
	CEdit	m_editCurrentLine;
	CEdit	m_editResumeLine;
	CEdit	m_editFilePath;
	CSpinButtonCtrl	m_spinScanSpeed;
	CEdit	m_editScanSpeed;
	CComboBox	m_comboScanSide;
	CSpinButtonCtrl	m_spinSlowLength;
	CSpinButtonCtrl	m_spinFastLength;
	CSpinButtonCtrl	m_spinSlowIncrement;
	CSpinButtonCtrl	m_spinFastIncrement;
	CEdit	m_editFinishPt;
	CEdit	m_editStartPt;
	CEdit	m_editSlowLength;
	CEdit	m_editFastLength;
	CEdit	m_editSlowIncrement;
	CEdit	m_editFastIncrement;
	CComboBox	m_comboSlowAxis;
	CComboBox	m_comboFastAxis;
	CComboBox	m_comboMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRectilinearScanPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRectilinearScanPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnButtonMoveRightToLeft();
	afx_msg void OnChangeEditPt0();
	afx_msg void OnChangeEditPt1();
	afx_msg void OnChangeEditIncrement0();
	afx_msg void OnChangeEditIncrement1();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditLength0();
	afx_msg void OnChangeEditLength1();
	afx_msg void OnSelchangeComboPrimaryAxis();
	afx_msg void OnSelchangeComboSecondaryAxis();
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnButtonStore0();
	afx_msg void OnButtonStore1();
	afx_msg void OnButtonGoto0();
	afx_msg void OnButtonGoto1();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonAbort();
	afx_msg void OnSelchangeComboScanSide();
	afx_msg void OnChangeEditSpeed();
	afx_msg void OnButtonBrowse();
	afx_msg void OnChangeEditFilepath();
	afx_msg void OnButtonMoveLeftToRight();
	afx_msg void OnChangeEditResumeLine();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDeltaposSpinIncrement0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinIncrement1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinLength0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinLength1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboBiUni();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECTILINEARSCANPAGE_H__5D89135A_3C84_416E_BCD9_B3F27D21180E__INCLUDED_)
