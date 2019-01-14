// 3DBladePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "3DBladePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DBladePage property page

IMPLEMENT_DYNCREATE(C3DBladePage, CPropertyPage)

C3DBladePage::C3DBladePage() : CPropertyPage(C3DBladePage::IDD)
{
	//{{AFX_DATA_INIT(C3DBladePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

C3DBladePage::~C3DBladePage()
{
}

void C3DBladePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DBladePage)
	DDX_Control(pDX, IDC_EDIT1, m_edtitGash);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3DBladePage, CPropertyPage)
	//{{AFX_MSG_MAP(C3DBladePage)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_ERRSPACE(IDC_EDIT1, OnErrspaceEdit1)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DBladePage message handlers

BOOL C3DBladePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL C3DBladePage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}

int C3DBladePage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void C3DBladePage::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void C3DBladePage::OnErrspaceEdit1() 
{
	// TODO: Add your control notification handler code here
	
}

void C3DBladePage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
}

void C3DBladePage::OnTimer(UINT nIDEvent) 
{
	
	CPropertyPage::OnTimer(nIDEvent);
}
