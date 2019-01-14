#if !defined(AFX_SCANSHEET_H__0947B973_C53A_4E19_90DA_BD5C707BC20C__INCLUDED_)
#define AFX_SCANSHEET_H__0947B973_C53A_4E19_90DA_BD5C707BC20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanSheet.h : header file
//
#include "ContourScanDlg.h"
#include "FokkerScanDetailsPage.h"
/////////////////////////////////////////////////////////////////////////////
// CScanSheet

class CScanSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CScanSheet)

// Construction
public:
	CContourScanPage* m_pContourScanPage;
	CFokkerScanDetailsPage* m_pFokkerScanDetailsPage;

	void UpdateAllPages();
	
	CWnd** m_pDlg;
	void Construct(CWnd** pDlg);
	CScanSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CScanSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScanSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScanSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANSHEET_H__0947B973_C53A_4E19_90DA_BD5C707BC20C__INCLUDED_)
