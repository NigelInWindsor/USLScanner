// RRWIPPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RRWIPPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRRWIPPage property page

IMPLEMENT_DYNCREATE(CRRWIPPage, CPropertyPage)

CRRWIPPage::CRRWIPPage() : CPropertyPage(CRRWIPPage::IDD)
{
	//{{AFX_DATA_INIT(CRRWIPPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CRRWIPPage::~CRRWIPPage()
{
}

void CRRWIPPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRRWIPPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRRWIPPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRRWIPPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRRWIPPage message handlers

BOOL CRRWIPPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRRWIPPage::UpdateAllControls()
{
	
	CWnd* pWnd;
	CString Buff;

	if(pWnd = GetDlgItem(IDC_EDIT_BATCH)) {
		theApp.m_RRMES.GetWIPValue(_T("Batch"),Buff);
		pWnd->SetWindowText(Buff);
	}
	if(pWnd = GetDlgItem(IDC_EDIT_SERIAL_NUMBER)) {
		theApp.m_RRMES.GetWIPValue(_T("SerialNo"),Buff);
		pWnd->SetWindowText(Buff);
	}
	if(pWnd = GetDlgItem(IDC_EDIT_PART_NUMBER)) {
		theApp.m_RRMES.GetWIPValue(_T("PartNo"),Buff);
		pWnd->SetWindowText(Buff);
	}
	if(pWnd = GetDlgItem(IDC_EDIT_PART_TYPE)) {
		theApp.m_RRMES.GetWIPValue(_T("PartNoType"),Buff);
		pWnd->SetWindowText(Buff);
	}
	if(pWnd = GetDlgItem(IDC_EDIT_USER)) {
		theApp.m_RRMES.GetWIPValue(_T("User"),Buff);
		pWnd->SetWindowText(Buff);
	}
}
