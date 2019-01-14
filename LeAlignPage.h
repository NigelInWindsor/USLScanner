#if !defined(AFX_LEALIGNPAGE_H__A53413BC_DFC7_4387_AB21_9BAA60AD1372__INCLUDED_)
#define AFX_LEALIGNPAGE_H__A53413BC_DFC7_4387_AB21_9BAA60AD1372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeAlignPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeAlignPage dialog

class CLeAlignPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLeAlignPage)

// Construction
public:
	void UpdateAllControls();
	
	CWnd* m_pParent;
	CLeAlignPage(CWnd* pParent = NULL);
	~CLeAlignPage();

// Dialog Data
	//{{AFX_DATA(CLeAlignPage)
	enum { IDD = IDD_LE_ALIGN_PAGE };
	CComboBox	m_comboLEAlgorithm;
	CSpinButtonCtrl	m_spinYOffset;
	CEdit	m_editYOffset;
	CEdit	m_editLeDrop;
	CListBox	m_listAlignWhich;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLeAlignPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLeAlignPage)
	afx_msg void OnButtonDoIt();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditLeDrop();
	afx_msg void OnDeltaposSpinYOffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditYOffset();
	afx_msg void OnSelchangeComboLeAlgorithm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEALIGNPAGE_H__A53413BC_DFC7_4387_AB21_9BAA60AD1372__INCLUDED_)
