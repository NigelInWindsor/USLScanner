#if !defined(AFX_CUSTOMIZESHEET_H__295B02FC_B81D_40C6_BC0C_28C236346723__INCLUDED_)
#define AFX_CUSTOMIZESHEET_H__295B02FC_B81D_40C6_BC0C_28C236346723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomizeSheet.h : header file
//
#include "MachineDescriptionDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CCustomizeSheet

class CCustomizeSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCustomizeSheet)

// Construction
public:
	CMachineDescriptionDlg* m_pMachineDescriptionDlg;
	inline void Construct(CWnd** pDlg);
	
	CWnd** m_pDlg;
	CCustomizeSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CCustomizeSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomizeSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomizeSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomizeSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMIZESHEET_H__295B02FC_B81D_40C6_BC0C_28C236346723__INCLUDED_)
