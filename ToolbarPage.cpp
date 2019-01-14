// ToolbarPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ToolbarPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolbarPage property page

IMPLEMENT_DYNCREATE(CToolbarPage, CPropertyPage)

CToolbarPage::CToolbarPage() : CPropertyPage(CToolbarPage::IDD)
{
	//{{AFX_DATA_INIT(CToolbarPage)
	//}}AFX_DATA_INIT
}

CToolbarPage::~CToolbarPage()
{
}

void CToolbarPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolbarPage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_TOOLBARS, m_listToolBar);
}


BEGIN_MESSAGE_MAP(CToolbarPage, CPropertyPage)
	//{{AFX_MSG_MAP(CToolbarPage)
	//}}AFX_MSG_MAP
	ON_CLBN_CHKCHANGE(IDC_LIST_TOOLBARS, OnCheckChangeListToolbars)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolbarPage message handlers

BOOL CToolbarPage::OnInitDialog() 
{
	APP App = (APP) AfxGetApp();
	CPropertyPage::OnInitDialog();
	CString Buff;
	CString strMFCToolBar, strBasePane;

	m_listToolBar.ResetContent();
	m_listToolBar.SetCheckStyle(BS_AUTOCHECKBOX);


	for (int ii = 0; ii < sizeof m_nID / sizeof (int); ii++) {
		strBasePane.Format(L"BasePane-%d", m_nID[ii]);
		strMFCToolBar.Format(L"MFCToolBar-%d", m_nID[ii]);

		m_listToolBar.AddString(theApp.GetSectionString(strMFCToolBar, L"Name", L""));
		m_listToolBar.SetCheck(ii, theApp.GetSectionInt(strBasePane, L"IsVisible") );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CToolbarPage::OnCheckChangeListToolbars()
{
	CString strBasePane;

	int nIndex = m_listToolBar.GetCurSel();
	strBasePane.Format(L"BasePane-%d", m_nID[nIndex]);

	if (m_listToolBar.GetCheck(nIndex) != 0) {
		theApp.WriteSectionInt(strBasePane, L"IsVisible", 1);
	}
	else {
		theApp.WriteSectionInt(strBasePane, L"IsVisible", 0);
	}

	CMainFrame*  pFrame = (CMainFrame*)theApp.m_pMainWnd;
	pFrame->SendMessage(UI_INVALIDATE_TOOLBARS, NULL, NULL);

}

BOOL CToolbarPage::OnSetActive() 
{
	APP App = (APP)AfxGetApp();
	App->m_LastSettings.nLastCustomizeTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}
