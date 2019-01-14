#if !defined(AFX_FOKKERSCANDETAILSPAGE_H__7E9E4712_AC29_4E69_835E_D6C843879ABE__INCLUDED_)
#define AFX_FOKKERSCANDETAILSPAGE_H__7E9E4712_AC29_4E69_835E_D6C843879ABE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FokkerScanDetailsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFokkerScanDetailsPage dialog

class CFokkerScanDetailsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFokkerScanDetailsPage)

// Construction
public:
	void UpdateRegistry();
	void GetDirectory(CEdit *pEdit, CString lpstrInitDir);
	void UpdateAllControls();
	
	CData* m_pData;
	CFokkerScanDetailsPage(CData* pData = NULL);
	~CFokkerScanDetailsPage();

// Dialog Data
	//{{AFX_DATA(CFokkerScanDetailsPage)
	enum { IDD = IDD_FOKKER_SCAN_DETAILS_DIALOG };
	CEdit	m_editProject;
	CEdit	m_editProductNumber;
	CEdit	m_editProductDescription;
	CEdit	m_editOrderNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFokkerScanDetailsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFokkerScanDetailsPage)
	afx_msg void OnChangeEditproject();
	afx_msg void OnChangeEditOrderNumber();
	afx_msg void OnButtonDirectory();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOKKERSCANDETAILSPAGE_H__7E9E4712_AC29_4E69_835E_D6C843879ABE__INCLUDED_)
