#if !defined(AFX_INCLUSIONSRESULTSPAGE_H__FEC8BDD5_A171_44C8_8035_F645DDB18460__INCLUDED_)
#define AFX_INCLUSIONSRESULTSPAGE_H__FEC8BDD5_A171_44C8_8035_F645DDB18460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InclusionsResultsPage.h : header file
//
#include "EditSpinItem.h"

/////////////////////////////////////////////////////////////////////////////
// CInclusionsResultsPage dialog

class CInclusionsResultsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CInclusionsResultsPage)

// Construction
public:
	void RenderView(CDC* pDC, CRect rrView);
	void UpdateAllControls();
	void FillList();
	void CreateColumns();
	CEditSpinItem*	m_pEditSpinItem;
	int m_nIndex;
	
	CData* m_pData;
	CInclusions* m_pInclusions;
	CInclusionsResultsPage(CInclusions* pInclusions = NULL, CData* pData = NULL);
	~CInclusionsResultsPage();

// Dialog Data
	//{{AFX_DATA(CInclusionsResultsPage)
	enum { IDD = IDD_INCLUSION_RESULTS_PAGE };
	CEdit	m_editResults;
	CStatic	m_staticView;
	CListCtrl	m_listResults;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CInclusionsResultsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInclusionsResultsPage)
	afx_msg void OnClickListResults(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListResults(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListResults(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListResults(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnButtonTextFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INCLUSIONSRESULTSPAGE_H__FEC8BDD5_A171_44C8_8035_F645DDB18460__INCLUDED_)
