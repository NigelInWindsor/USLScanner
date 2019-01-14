#if !defined(AFX_PUMPPAGE_H__8F63EA94_FBBA_418D_94E0_2965F5DDC7B9__INCLUDED_)
#define AFX_PUMPPAGE_H__8F63EA94_FBBA_418D_94E0_2965F5DDC7B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PumpPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPumpPage dialog

class CPumpPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPumpPage)

// Construction
public:
	HBRUSH m_hBr;
	int m_nOldArray[4];
	void SetAccessPrivelage();
	void UpdateAllControls();
	
	CPumpPage();
	~CPumpPage();

// Dialog Data
	//{{AFX_DATA(CPumpPage)
	enum { IDD = IDD_PUMP_PAGE };
	CButton	m_staticRight;
	CButton	m_staticLeft;
	CEdit	m_editVaneEncoder1;
	CEdit	m_editVaneEncoder0;
	CEdit	m_editPumpEncoder1;
	CEdit	m_editPumpEncoder0;
	CSpinButtonCtrl	m_spinRightSpeed;
	CSpinButtonCtrl	m_spinLeftSpeed;
	CEdit	m_editRightSpeed;
	CEdit	m_editLeftSpeed;
	CButton	m_checkRightOffOn;
	CButton	m_checkLeftOffOn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPumpPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPumpPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditLeftSpeed();
	afx_msg void OnChangeEditRightSpeed();
	afx_msg void OnCheckLeftOffon();
	afx_msg void OnButtonPurgeLeft();
	afx_msg void OnButtonPurgeRight();
	afx_msg void OnCheckRightOffon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PUMPPAGE_H__8F63EA94_FBBA_418D_94E0_2965F5DDC7B9__INCLUDED_)
