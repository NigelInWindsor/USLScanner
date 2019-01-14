#if !defined(AFX_SQUIRTERSPAGE_H__18E629E7_CCB9_4308_8EAB_99C8AF5DEDD9__INCLUDED_)
#define AFX_SQUIRTERSPAGE_H__18E629E7_CCB9_4308_8EAB_99C8AF5DEDD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SquirtersPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSquirtersPage dialog

class CSquirtersPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSquirtersPage)

// Construction
public:
	HBRUSH m_hBr;
	void SetAccessPrivelage();
	void UpdateAllControls();
	int m_nSide;
	CSquirtersPage();
	~CSquirtersPage();

// Dialog Data
	//{{AFX_DATA(CSquirtersPage)
	enum { IDD = IDD_SQUIRTERS_PAGE };
	CEdit	m_editLeftAngle;
	CEdit	m_editRightAngle;
	CEdit	m_editDwellTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSquirtersPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSquirtersPage)
	afx_msg void OnButtonPointDown();
	afx_msg void OnButtonZeroZero();
	afx_msg void OnButtonRemoveAir();
	afx_msg void OnChangeEditDewllTime();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnButton45();
	afx_msg void OnChangeEditLeftAngle();
	afx_msg void OnChangeEditRightAngle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SQUIRTERSPAGE_H__18E629E7_CCB9_4308_8EAB_99C8AF5DEDD9__INCLUDED_)
