// RRMESPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RRMESPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRRMESPage property page

IMPLEMENT_DYNCREATE(CRRMESPage, CPropertyPage)

CRRMESPage::CRRMESPage() : CPropertyPage(CRRMESPage::IDD)
{
	//{{AFX_DATA_INIT(CRRMESPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CRRMESPage::~CRRMESPage()
{
}

void CRRMESPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRRMESPage)
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_buttonApply);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editResourceName);
	DDX_Control(pDX, IDC_TREE_GAUGE_TO_MES, m_treeGaugeToMES);
	DDX_Control(pDX, IDC_TREE_MES_TO_GAUGE, m_treeMEStoGauge);
	DDX_Control(pDX, IDC_COMBO_ACTIVE, m_comboActive);
	DDX_Control(pDX, IDC_EDIT_CONTROL_PATH_TO_MES, m_editControlPathToMes);
	DDX_Control(pDX, IDC_EDIT_ASSOCIATED_PATH_TO_MES, m_editAssociatedPathToMes);
	DDX_Control(pDX, IDC_EDIT_ASSOCIATED_PATH_TO_GAUGE, m_editAssociatedPathToGauge);
	DDX_Control(pDX, IDC_EDIT_CONTROL_PATH_TO_GAUGE, m_editControlPathToGauge);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRRMESPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRRMESPage)
	ON_BN_CLICKED(IDC_BUTTON_CTRL_TO_GAUGE, OnButtonCtrlToGauge)
	ON_BN_CLICKED(IDC_BUTTON_ASSOCIATED_TO_GAUGE, OnButtonAssociatedToGauge)
	ON_BN_CLICKED(IDC_BUTTON_CTRL_TO_MES, OnButtonCtrlToMes)
	ON_BN_CLICKED(IDC_BUTTON_ASSOCIATED_TO_MES, OnButtonAssociatedToMes)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTIVE, OnSelchangeComboActive)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRRMESPage message handlers
BOOL CRRMESPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;
		
	Buff.LoadString(IDS_No);	m_comboActive.AddString(Buff);
	Buff.LoadString(IDS_Yes);	m_comboActive.AddString(Buff);

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRRMESPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	HTREEITEM handle;
	CStringArray Array;
	int	ii;

	m_editControlPathToGauge.SetWindowText(theApp.m_RRMES.m_ControlPathToGauge);
	m_editAssociatedPathToGauge.SetWindowText(theApp.m_RRMES.m_AssociatedPathToGauge);
	m_editControlPathToMes.SetWindowText(theApp.m_RRMES.m_MessagePathToMes);
	m_editAssociatedPathToMes.SetWindowText(theApp.m_RRMES.m_AssociatedPathToMes);

	m_comboActive.SetCurSel(theApp.m_RRMES.m_bEnabled);


	m_treeMEStoGauge.DeleteAllItems();
	HTREEITEM hControl = m_treeMEStoGauge.InsertItem(theApp.m_RRMES.m_ControlPathToGauge,TVI_ROOT,NULL);
	theApp.m_RRMES.FillFileNameList(MES_CONTROL_TO_GAUGE,&Array);
	for(ii=0;ii<Array.GetSize();ii++) {
		handle = m_treeMEStoGauge.InsertItem(Array.GetAt(ii),hControl,NULL); m_treeMEStoGauge.EnsureVisible(handle);
	}

	HTREEITEM hAssociated = m_treeMEStoGauge.InsertItem(theApp.m_RRMES.m_AssociatedPathToGauge,TVI_ROOT,NULL);
	theApp.m_RRMES.FillFileNameList(MES_ASSOCIATED_TO_GAUGE,&Array);
	for(ii=0;ii<Array.GetSize();ii++) {
		handle = m_treeMEStoGauge.InsertItem(Array.GetAt(ii),hAssociated,NULL); m_treeMEStoGauge.EnsureVisible(handle);
	}
	m_treeMEStoGauge.SetScrollPos(SB_HORZ,0,FALSE);

	m_treeGaugeToMES.DeleteAllItems();
	HTREEITEM hMessage = m_treeGaugeToMES.InsertItem(theApp.m_RRMES.m_MessagePathToMes,TVI_ROOT,NULL);
	theApp.m_RRMES.FillFileNameList(MES_MESSAGES_TO_MES,&Array);
	for(ii=0;ii<Array.GetSize();ii++) {
		handle = m_treeGaugeToMES.InsertItem(Array.GetAt(ii),hMessage,NULL); m_treeGaugeToMES.EnsureVisible(handle);
	}

	hAssociated = m_treeGaugeToMES.InsertItem(theApp.m_RRMES.m_AssociatedPathToMes,TVI_ROOT,NULL);
	theApp.m_RRMES.FillFileNameList(MES_ASSOCIATED_TO_MES,&Array);
	for(ii=0;ii<Array.GetSize();ii++) {
		handle = m_treeGaugeToMES.InsertItem(Array.GetAt(ii),hAssociated,NULL); m_treeGaugeToMES.EnsureVisible(handle);
	}
	m_treeGaugeToMES.SetScrollPos(SB_HORZ,0,FALSE);

	m_editResourceName.SetWindowText(theApp.m_RRMES.m_strResourceName);

}

void CRRMESPage::GetDirectory(CEdit *pEdit, CString *pInitDir)
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
	li=SHBrowseForFolder(&bi);
	if(SHGetPathFromIDList(li,DisplayName) ) {
		Buff.Format(_T("%s"),DisplayName);
		if(pEdit) pEdit->SetWindowText(Buff);
		if(pInitDir) pInitDir->Format(_T("%s"),Buff);
	};	

}

void CRRMESPage::OnButtonCtrlToGauge() 
{
	

	GetDirectory(&m_editControlPathToGauge,&theApp.m_RRMES.m_ControlPathToGauge);
	m_buttonApply.EnableWindow(TRUE);

}

void CRRMESPage::OnButtonAssociatedToGauge() 
{
	

	GetDirectory(&m_editAssociatedPathToGauge,&theApp.m_RRMES.m_AssociatedPathToGauge);
	m_buttonApply.EnableWindow(TRUE);
	
}

void CRRMESPage::OnButtonCtrlToMes() 
{
	

	GetDirectory(&m_editControlPathToMes,&theApp.m_RRMES.m_MessagePathToMes);
	m_buttonApply.EnableWindow(TRUE);
	
}

void CRRMESPage::OnButtonAssociatedToMes() 
{
	

	GetDirectory(&m_editAssociatedPathToMes,&theApp.m_RRMES.m_AssociatedPathToMes);
	m_buttonApply.EnableWindow(TRUE);

	
}



void CRRMESPage::OnButtonApply() 
{
	
	theApp.m_RRMES.SaveRegitryVariables();
	m_buttonApply.EnableWindow(FALSE);
}

void CRRMESPage::OnSelchangeComboActive() 
{
	
	
	theApp.m_RRMES.m_bEnabled = m_comboActive.GetCurSel() & 0x01;
	m_buttonApply.EnableWindow(TRUE);
}

void CRRMESPage::OnChangeEditName() 
{
	
	CString Temp = theApp.m_RRMES.m_strResourceName;

	m_editResourceName.GetWindowText(theApp.m_RRMES.m_strResourceName);
	if(Temp != theApp.m_RRMES.m_strResourceName) {
		m_buttonApply.EnableWindow(TRUE);
	}
}
