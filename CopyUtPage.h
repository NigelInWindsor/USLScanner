#if !defined(AFX_COPYUTPAGE_H__78C4ACBF_0765_43DE_8B4F_03282E404DB4__INCLUDED_)
#define AFX_COPYUTPAGE_H__78C4ACBF_0765_43DE_8B4F_03282E404DB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CopyUtPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCopyUtPage dialog

class CCopyUtPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCopyUtPage)

// Construction
public:
	
	CCopyUtPage();
	~CCopyUtPage();

// Dialog Data
	//{{AFX_DATA(CCopyUtPage)
	enum { IDD = IDD_COPY_UT_PAGE };
	CCheckListBox	m_listUtVariables;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCopyUtPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCopyUtPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListUtVariables();
	afx_msg void OnCheckChangeListUtVariables();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYUTPAGE_H__78C4ACBF_0765_43DE_8B4F_03282E404DB4__INCLUDED_)
