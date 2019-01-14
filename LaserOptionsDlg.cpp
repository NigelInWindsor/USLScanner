// LaserOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "LaserOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLaserOptionsDlg dialog


CLaserOptionsDlg::CLaserOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLaserOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaserOptionsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLaserOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaserOptionsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLaserOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CLaserOptionsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaserOptionsDlg message handlers
