#if !defined(AFX_PMACVARIABLESPAGE_H__BC4CD2CF_53B9_4BD4_89E0_19FAD9D34652__INCLUDED_)
#define AFX_PMACVARIABLESPAGE_H__BC4CD2CF_53B9_4BD4_89E0_19FAD9D34652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacVariablesPage.h : header file
//
#include "EditSpinItem.h"
#include "ResizablePage.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CPmacVariablesPage dialog

class CPmacVariablesPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPmacVariablesPage)

// Construction
public:
	HBRUSH m_hBr;
	CStaticPosManage m_StaticPosVariables;
	int m_nIndex;
	CEditSpinItem* m_pEditSpinItem;
	CString m_strDescription[100];
	CString m_strValue[100];
	CString m_strRange0[100];
	CString m_strRange1[100];
	CString	m_Variable;
	int m_nTop;

	long UpdatePmacFromList(unsigned int nTemp,long nlTemp);
	void ClearEditSpin();
	void InitializeStrings();
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	int	m_nVariableType;
	CPmacVariablesPage(int nVariableType='I');
	~CPmacVariablesPage();

// Dialog Data
	//{{AFX_DATA(CPmacVariablesPage)
	enum { IDD = IDD_PMAC_VARIABLES_DIALOG };
	CEdit	m_editSwap;
	CEdit	m_editJumpTo;
	CListCtrl	m_listVariables;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPmacVariablesPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPmacVariablesPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnGetdispinfoListVariables(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonPlus100();
	afx_msg void OnButtonMinus100();
	afx_msg void OnDblclkListVariables(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListVariables(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonSwap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACVARIABLESPAGE_H__BC4CD2CF_53B9_4BD4_89E0_19FAD9D34652__INCLUDED_)
