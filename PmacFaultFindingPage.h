#if !defined(AFX_PMACFAULTFINDINGPAGE_H__A46B37A3_80AF_4C9F_B3D6_3AEE985F72E7__INCLUDED_)
#define AFX_PMACFAULTFINDINGPAGE_H__A46B37A3_80AF_4C9F_B3D6_3AEE985F72E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacFaultFindingPage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CPmacFaultFindingPage dialog

class CPmacFaultFindingPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPmacFaultFindingPage)
// Construction
public:
	int m_nFinishPos;
	int m_nStartPos;
	
	int m_nMovePeriodInmS;
	int m_nTestType;
	int	m_nTestAxis; //0-15
	CPmacFaultFindingPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPmacFaultFindingPage)
	enum { IDD = IDD_PMAC_FAULT_FINDING_PAGE };
	CEdit	m_editSuggestions;
	CComboBox	m_comboAxis;
	CEdit	m_editInstructions;
	CEdit	m_editStartPos;
	CEdit	m_editFinishPos;
	CEdit	m_editMovePeriod;
	CComboBox	m_comboTest;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPmacFaultFindingPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPmacFaultFindingPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboTest();
	afx_msg void OnButtonProceed();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACFAULTFINDINGPAGE_H__A46B37A3_80AF_4C9F_B3D6_3AEE985F72E7__INCLUDED_)
