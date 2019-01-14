// 3DToolsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "3DToolsSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DToolsSheet

IMPLEMENT_DYNAMIC(C3DToolsSheet, CPropertySheet)

C3DToolsSheet::C3DToolsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

C3DToolsSheet::C3DToolsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

C3DToolsSheet::~C3DToolsSheet()
{
}


BEGIN_MESSAGE_MAP(C3DToolsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(C3DToolsSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DToolsSheet message handlers
