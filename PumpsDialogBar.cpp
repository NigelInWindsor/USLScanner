// PumpsDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PumpsDialogBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPumpsDialogBar dialog


CPumpsDialogBar::CPumpsDialogBar(CWnd* pParent /*=NULL*/)
	: CDialog(CPumpsDialogBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPumpsDialogBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPumpsDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPumpsDialogBar)
	DDX_Control(pDX, ID_BUTTON_RIGHT_PUMP, m_buttonRightPump);
	DDX_Control(pDX, ID_BUTTON_LEFT_PUMP, m_buttonLeftPump);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPumpsDialogBar, CDialog)
	//{{AFX_MSG_MAP(CPumpsDialogBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPumpsDialogBar message handlers


BOOL CPumpsDialogBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
