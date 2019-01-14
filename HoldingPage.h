#if !defined(AFX_HOLDINGPAGE_H__BE7C4890_6232_463A_BF75_DA71EE4E4ABC__INCLUDED_)
#define AFX_HOLDINGPAGE_H__BE7C4890_6232_463A_BF75_DA71EE4E4ABC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoldingPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHoldingPage dialog

class CHoldingPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CHoldingPage)

// Construction
public:
	
	void UpdateAllControls();
	CHoldingPage();
	~CHoldingPage();

// Dialog Data
	//{{AFX_DATA(CHoldingPage)
	enum { IDD = IDD_HOLDING_PAGE };
	CEdit	m_editHoldingBall;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHoldingPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CHoldingPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStoreBallZero();
	afx_msg void OnButtonBallMoveto();
	afx_msg void OnButtonmovetoZero();
	afx_msg void OnButtonPark();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLDINGPAGE_H__BE7C4890_6232_463A_BF75_DA71EE4E4ABC__INCLUDED_)
