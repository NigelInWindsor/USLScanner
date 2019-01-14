#if !defined(AFX_FILTERSHEET_H__6D681954_966B_4C2C_9D49_72A31E48C79B__INCLUDED_)
#define AFX_FILTERSHEET_H__6D681954_966B_4C2C_9D49_72A31E48C79B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterSheet.h : header file
//
#include "ResizableSheet.h"
#include "ParksMcClellanPage.h"
#include "WindowedSincPage.h"
#include "ArtificialSignalPage.h"

/////////////////////////////////////////////////////////////////////////////
// CFilterSheet

class CFilterSheet : public CResizableSheet
{
	DECLARE_DYNAMIC(CFilterSheet)

// Construction
public:
	CWnd** m_pDlg;
	CFilterSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CFilterSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	CArtificialSignalPage* m_pArtificialSignalPage;
	CWindowedSincPage* m_pWindowedSincPage;
	CParksMcClellanPage* m_pParksMcClellanPage;
	void AllocatePages();
	virtual ~CFilterSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFilterSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERSHEET_H__6D681954_966B_4C2C_9D49_72A31E48C79B__INCLUDED_)
