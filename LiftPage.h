#if !defined(AFX_LIFTPAGE_H__8AB2B181_4AE7_4A37_9F1B_446BA07D297F__INCLUDED_)
#define AFX_LIFTPAGE_H__8AB2B181_4AE7_4A37_9F1B_446BA07D297F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LiftPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLiftPage dialog

class CLiftPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLiftPage)

// Construction
public:
	int m_nOldStatus;
	bool m_bTimerActive;
	
	void UpdateAllControls();
	CLiftPage();
	~CLiftPage();

// Dialog Data
	//{{AFX_DATA(CLiftPage)
	enum { IDD = IDD_LIFT_PAGE };
	CEdit	m_editLiftStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLiftPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLiftPage)
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonGotoZero();
	afx_msg void OnButtonGoto180();
	afx_msg void OnButtonOrigin();
	afx_msg void OnButtonLiftStop();
	afx_msg void OnButtonMovetoZero();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIFTPAGE_H__8AB2B181_4AE7_4A37_9F1B_446BA07D297F__INCLUDED_)
