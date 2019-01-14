#if !defined(AFX_LASERTOOLSPAGE_H__6C1BDEFB_424E_4F59_9208_E43011DB406B__INCLUDED_)
#define AFX_LASERTOOLSPAGE_H__6C1BDEFB_424E_4F59_9208_E43011DB406B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LaserToolsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLaserToolsPage dialog

class CLaserToolsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLaserToolsPage)

// Construction
public:
	CPropertySheet* m_pToolSheet;
	CPropertyPage* m_pViewPage;
	CLaserToolsPage(CPropertySheet* pToolSheet = NULL, CPropertyPage* pViewPage = NULL);   // standard constructor
	~CLaserToolsPage();

// Dialog Data
	//{{AFX_DATA(CLaserToolsPage)
	enum { IDD = IDD_LASER_TOOLS_PAGE };
	CEdit	m_editZRange;
	CEdit	m_editYRange;
	CEdit	m_editTranslation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLaserToolsPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLaserToolsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditTranslation();
	afx_msg void OnChangeEditZRange();
	afx_msg void OnChangeEditYRange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LASERTOOLSPAGE_H__6C1BDEFB_424E_4F59_9208_E43011DB406B__INCLUDED_)
