#if !defined(AFX_CORRELATIONPAGE_H__60B940BD_9A77_4907_B2F3_F644FD693E81__INCLUDED_)
#define AFX_CORRELATIONPAGE_H__60B940BD_9A77_4907_B2F3_F644FD693E81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CorrelationPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCorrelationPage dialog

class CCorrelationPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCorrelationPage)

// Construction
public:
	void RenderScope(CPaintDC* pDC);
	void UpdateAllControls();
	
	CWnd* m_pParent;
	CCorrelationPage(CWnd* pParent = NULL);
	~CCorrelationPage();

// Dialog Data
	//{{AFX_DATA(CCorrelationPage)
	enum { IDD = IDD_CORRELATION_PAGE };
	CStatic	m_staticScope;
	CSpinButtonCtrl	m_spinPreTrigger;
	CSpinButtonCtrl	m_spinPostTrigger;
	CEdit	m_editPreTrigger;
	CEdit	m_editPostTrigger;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCorrelationPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCorrelationPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditPostTrigger();
	afx_msg void OnChangeEditPreTrigger();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CORRELATIONPAGE_H__60B940BD_9A77_4907_B2F3_F644FD693E81__INCLUDED_)
