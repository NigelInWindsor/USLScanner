#if !defined(AFX_3DTOOLSSHEET_H__4F51646D_B7E6_4345_BA77_82D6CFEAEF2D__INCLUDED_)
#define AFX_3DTOOLSSHEET_H__4F51646D_B7E6_4345_BA77_82D6CFEAEF2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DToolsSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DToolsSheet

class C3DToolsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(C3DToolsSheet)

// Construction
public:
	C3DToolsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	C3DToolsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DToolsSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~C3DToolsSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(C3DToolsSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DTOOLSSHEET_H__4F51646D_B7E6_4345_BA77_82D6CFEAEF2D__INCLUDED_)
