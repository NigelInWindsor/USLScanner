#if !defined(AFX_INCLUSIONSSHEET_H__CD6867C2_FD3B_4C2D_8FCF_99070CB79AF0__INCLUDED_)
#define AFX_INCLUSIONSSHEET_H__CD6867C2_FD3B_4C2D_8FCF_99070CB79AF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InclusionsSheet.h : header file
//
#include "InclusionRulesPage.h"
#include "InclusionsResultsPage.h"
#include "InclusionsDetailsPage.h"

/////////////////////////////////////////////////////////////////////////////
// CInclusionsSheet

class CInclusionsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CInclusionsSheet)

// Construction
public:
	CData* m_pData;
	int	m_nGlobalLocal;
	CInclusions* m_pInclusions;
	void UpdateAllPages();
	
	CWnd** m_pDlg;
	void Construct(CWnd** pDlg,CInclusions* pInclusions = NULL, int nGlobalLocal = 0, CData* pData = NULL);
	CInclusionsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL,CInclusions* pInclusions = NULL, int nGlobalLocal = 0, CData* pData = NULL);
	CInclusionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL,CInclusions* pInclusions = NULL, int nGlobalLocal = 0, CData* pData = NULL);

// Attributes
public:

// Operations
public:
	CInclusionsDetailsPage* m_pInclusionsDetailsPage;
	CInclusionRulesPage* m_pInclusionRulesPage;
	CInclusionsResultsPage* m_pInclusionsResultsPage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInclusionsSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInclusionsSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInclusionsSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INCLUSIONSSHEET_H__CD6867C2_FD3B_4C2D_8FCF_99070CB79AF0__INCLUDED_)
