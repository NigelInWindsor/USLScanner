#if !defined(AFX_CADSHEET_H__984FE9DC_310D_46C0_A7EC_921EBFD6D982__INCLUDED_)
#define AFX_CADSHEET_H__984FE9DC_310D_46C0_A7EC_921EBFD6D982__INCLUDED_

#include "CADLinePage.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CADSheet.h : header file
//

#include "CADLinePage.h"
/////////////////////////////////////////////////////////////////////////////
// CCADSheet

class CCADSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCADSheet)

// Construction
public:
	CWnd* m_pViewSheet;
	CWnd** m_pDlg;
	
	void Construct(CWnd* pParentWnd, CWnd** pDlg);
	CCADSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CCADSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCADSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateAllPages();
	CCADLinePage *m_pCADLinePage;
	CWnd** pDlg;
	virtual ~CCADSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCADSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CADSHEET_H__984FE9DC_310D_46C0_A7EC_921EBFD6D982__INCLUDED_)
