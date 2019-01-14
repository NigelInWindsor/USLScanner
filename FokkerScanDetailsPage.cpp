// FokkerScanDetailsPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "FokkerScanDetailsPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFokkerScanDetailsPage property page

IMPLEMENT_DYNCREATE(CFokkerScanDetailsPage, CPropertyPage)

CFokkerScanDetailsPage::CFokkerScanDetailsPage(CData* pData) : CPropertyPage(CFokkerScanDetailsPage::IDD)
{
	//{{AFX_DATA_INIT(CFokkerScanDetailsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pData = pData;
}

CFokkerScanDetailsPage::~CFokkerScanDetailsPage()
{
}

void CFokkerScanDetailsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFokkerScanDetailsPage)
	DDX_Control(pDX, IDC_EDITPROJECT, m_editProject);
	DDX_Control(pDX, IDC_EDIT_PRODUCT_NUMBER, m_editProductNumber);
	DDX_Control(pDX, IDC_EDIT_PRODUCT, m_editProductDescription);
	DDX_Control(pDX, IDC_EDIT_ORDER_NUMBER, m_editOrderNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFokkerScanDetailsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFokkerScanDetailsPage)
	ON_EN_CHANGE(IDC_EDITPROJECT, OnChangeEditproject)
	ON_EN_CHANGE(IDC_EDIT_ORDER_NUMBER, OnChangeEditOrderNumber)
	ON_BN_CLICKED(IDC_BUTTON_DIRECTORY, OnButtonDirectory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFokkerScanDetailsPage message handlers

BOOL CFokkerScanDetailsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFokkerScanDetailsPage::UpdateAllControls()
{
	CString	Buff;
	int nPos;

	if (GetSafeHwnd() == NULL) return;


	Buff = m_pData->m_ProfileName;
	if((nPos=Buff.ReverseFind('.')) > -1) {
		Buff.GetBufferSetLength(nPos);
	}
	m_editProductNumber.SetWindowText(Buff);

	m_editProject.SetWindowText(theApp.m_StorePath[0]);
	m_editOrderNumber.SetWindowText(m_pData->m_FileName);
	m_editProductDescription.SetWindowText(m_pData->m_ProfileDescription);

}


BOOL CFokkerScanDetailsPage::OnSetActive() 
{
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}

void CFokkerScanDetailsPage::OnChangeEditproject() 
{
	m_editProject.GetWindowText(theApp.m_StorePath[0]);
	UpdateRegistry();
}

void CFokkerScanDetailsPage::OnChangeEditOrderNumber() 
{
	m_editOrderNumber.GetWindowText(m_pData->m_FileName);
	
}

void CFokkerScanDetailsPage::OnButtonDirectory() 
{
	GetDirectory(&m_editProject,theApp.m_StorePath[0]);
	UpdateRegistry();
	
}

static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	if(uMsg==BFFM_INITIALIZED) {
		::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);
	}
	return 0;


}

void CFokkerScanDetailsPage::GetDirectory(CEdit *pEdit, CString lpstrInitDir)
{
	BROWSEINFO bi;
	ITEMIDLIST* li;
	WCHAR DisplayName[MAX_PATH];
	WCHAR Title[MAX_PATH]={_T("Select new directory")};
	CString Buff;


	memset(&bi,0,sizeof bi);
	memset(DisplayName,0,sizeof DisplayName);
	bi.ulFlags = BIF_EDITBOX;
	bi.pszDisplayName = DisplayName;
	bi.lpszTitle = Title;
	bi.lpfn = BrowseCallbackProc;
//	bi.lParam = (LPARAM)(WCHAR*) lpstrInitDir;

	li=SHBrowseForFolder(&bi);
	if(SHGetPathFromIDList(li,DisplayName) ) {
		Buff.Format(_T("%s"),DisplayName);
		pEdit->SetWindowText(Buff);
	};	

}




void CFokkerScanDetailsPage::UpdateRegistry()
{
	theApp.SaveRegistryVariables();
}