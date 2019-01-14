#if !defined(AFX_TABSPAGE_H__ED702ED0_994C_4E08_8AF0_B044B1D9A1FF__INCLUDED_)
#define AFX_TABSPAGE_H__ED702ED0_994C_4E08_8AF0_B044B1D9A1FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabsPage dialog

class CTabsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabsPage)

// Construction
public:
	void UpdateAllControls();
	int m_nTabType;
	CTabsPage();
	~CTabsPage();

// Dialog Data
	//{{AFX_DATA(CTabsPage)
	enum { IDD = IDD_TABS_PAGE };
	CComboBox	m_comboTabs;
	//}}AFX_DATA
	CCheckListBox	m_listTabs;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboTabs();
	afx_msg void OnCheckChangeListTabs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSPAGE_H__ED702ED0_994C_4E08_8AF0_B044B1D9A1FF__INCLUDED_)
