// DialogBarPumps.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "DialogBarPumps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogBarPumps dialog


CDialogBarPumps::CDialogBarPumps()
{
	//{{AFX_DATA_INIT(CDialogBarPumps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogBarPumps::DoDataExchange(CDataExchange* pDX)
{
	CPaneDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBarPumps)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogBarPumps, CPaneDialog)
	//{{AFX_MSG_MAP(CDialogBarPumps)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBarPumps message handlers


BOOL CDialogBarPumps::Create(CWnd* pParent, UINT nIDTemplate, UINT nStyle, UINT nID) 
{
	BOOL bReturn = CPaneDialog::Create(pParent, nIDTemplate, nStyle, nID);

	return bReturn;
}

int CDialogBarPumps::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPaneDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}

