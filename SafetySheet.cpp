// SafetySheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "SafetySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSafetySheet

IMPLEMENT_DYNAMIC(CSafetySheet, CPropertySheet)

CSafetySheet::CSafetySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CSafetySheet::CSafetySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CSafetySheet::~CSafetySheet()
{
}


BEGIN_MESSAGE_MAP(CSafetySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSafetySheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSafetySheet message handlers
