// ViewZoomPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ViewZoomPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewZoomPage property page

IMPLEMENT_DYNCREATE(CViewZoomPage, CPropertyPage)

CViewZoomPage::CViewZoomPage(CWnd* pViewSheet) : CPropertyPage(CViewZoomPage::IDD)
{
	//{{AFX_DATA_INIT(CViewZoomPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pViewSheet = pViewSheet;
}

CViewZoomPage::~CViewZoomPage()
{
}

void CViewZoomPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewZoomPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewZoomPage, CPropertyPage)
	//{{AFX_MSG_MAP(CViewZoomPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewZoomPage message handlers

BOOL CViewZoomPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
