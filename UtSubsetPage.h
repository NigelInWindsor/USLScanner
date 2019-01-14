#if !defined(AFX_UTSUBSETPAGE_H__1FB7E7D7_73B2_4B62_A09F_064ADA3CB738__INCLUDED_)
#define AFX_UTSUBSETPAGE_H__1FB7E7D7_73B2_4B62_A09F_064ADA3CB738__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UtSubsetPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUtSubsetPage dialog

class CUtSubsetPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CUtSubsetPage)

// Construction
public:
	void UpdateAllControls();
	void OnCheckChangeListUtVariables();
	
	CUtSubsetPage();
	~CUtSubsetPage();

// Dialog Data
	//{{AFX_DATA(CUtSubsetPage)
	enum { IDD = IDD_UT_SUBSET_PAGE };
	CCheckListBox	m_listVariables;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUtSubsetPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUtSubsetPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListUtVariables();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUtFillall();
	afx_msg void OnUtClearall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UTSUBSETPAGE_H__1FB7E7D7_73B2_4B62_A09F_064ADA3CB738__INCLUDED_)
