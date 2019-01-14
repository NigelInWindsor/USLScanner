#if !defined(AFX_RRBLADESCANPAGE_H__57C97B5F_81CA_4FC8_8DDA_5A2799BF8BE8__INCLUDED_)
#define AFX_RRBLADESCANPAGE_H__57C97B5F_81CA_4FC8_8DDA_5A2799BF8BE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RRBladeScanPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRRBladeScanPage dialog

class CRRBladeScanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRRBladeScanPage)

// Construction
public:
	void UpdateAllControls();
	
	CRRBladeScanPage();
	~CRRBladeScanPage();

// Dialog Data
	//{{AFX_DATA(CRRBladeScanPage)
	enum { IDD = IDD_RR_BLADE_SCAN_PAGE };
	CEdit	m_editProfileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRRBladeScanPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRRBladeScanPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOutside();
	afx_msg void OnButtonInside();
	afx_msg void OnButtonOneLine();
	afx_msg void OnButtonXfer();
	afx_msg void OnButtonPreviousLine();
	afx_msg void OnButtonNextLine();
	afx_msg void OnButtonBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RRBLADESCANPAGE_H__57C97B5F_81CA_4FC8_8DDA_5A2799BF8BE8__INCLUDED_)
