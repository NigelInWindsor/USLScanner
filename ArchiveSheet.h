#if !defined(AFX_ARCHIVESHEET_H__E99B4B2D_29C5_4D3F_9983_42AB80F4EE4A__INCLUDED_)
#define AFX_ARCHIVESHEET_H__E99B4B2D_29C5_4D3F_9983_42AB80F4EE4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArchiveSheet.h : header file
//

#include "DirectoriesDlg.h"
#include "RRMESPage.h"

/////////////////////////////////////////////////////////////////////////////
// CArchiveSheet

class CArchiveSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CArchiveSheet)

// Construction
public:
	CRRMESPage* m_pRRMESPage;
	CDirectoriesDlg* m_pDirectoriesPage;
	CWnd**	m_pDlg;
	

	CArchiveSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CArchiveSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArchiveSheet)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CArchiveSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CArchiveSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARCHIVESHEET_H__E99B4B2D_29C5_4D3F_9983_42AB80F4EE4A__INCLUDED_)
