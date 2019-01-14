#if !defined(AFX_BLADECSVPAGE_H__CEBD8959_A0BB_4C4E_9FEA_E0DCE55DED28__INCLUDED_)
#define AFX_BLADECSVPAGE_H__CEBD8959_A0BB_4C4E_9FEA_E0DCE55DED28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BladeCSVPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBladeCSVPage dialog

class CBladeCSVPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBladeCSVPage)

// Construction
public:
	void UpdateAllControls();
	
	CWnd* m_pParent;
	CData * m_pData;
	CBladeCSVPage(CWnd* pParent = NULL, CData *pData = NULL);
	~CBladeCSVPage();

// Dialog Data
	//{{AFX_DATA(CBladeCSVPage)
	enum { IDD = IDD_BLADE_CSV_PAGE };
	CComboBox	m_comboWhat;
	CComboBox	m_comboWhichLines;
	CComboBox	m_comboSaveTo;
	CComboBox	m_comboAutoIncrementer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBladeCSVPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBladeCSVPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnSelchangeComboAutoIncrementer();
	afx_msg void OnSelchangeComboSaveTo();
	afx_msg void OnSelchangeComboWhichLines();
	afx_msg void OnSelchangeComboWhat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLADECSVPAGE_H__CEBD8959_A0BB_4C4E_9FEA_E0DCE55DED28__INCLUDED_)
