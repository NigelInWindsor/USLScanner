#if !defined(AFX_BLADEMASTERPAGE_H__E4F5A08D_FE0B_46DD_A62A_93D4B09C185D__INCLUDED_)
#define AFX_BLADEMASTERPAGE_H__E4F5A08D_FE0B_46DD_A62A_93D4B09C185D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BladeMasterPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBladeMasterPage dialog

class CBladeMasterPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBladeMasterPage)

// Construction
public:
	void SetAccessPrivelages();
	
	CWnd* m_pParent;
	CData * m_pData;
	CBladeMasterPage(CWnd* pParent = NULL, CData *pData = NULL);
	~CBladeMasterPage();

// Dialog Data
	//{{AFX_DATA(CBladeMasterPage)
	enum { IDD = IDD_BLADE_MASTER_PAGE };
	CButton	m_buttonSaveAs;
	CButton	m_buttonTrafficLights;
	CButton	m_buttonBrane;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBladeMasterPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBladeMasterPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTrafficLights();
	afx_msg void OnButtonBrane();
	afx_msg void OnButtonFileSaveas();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLADEMASTERPAGE_H__E4F5A08D_FE0B_46DD_A62A_93D4B09C185D__INCLUDED_)
