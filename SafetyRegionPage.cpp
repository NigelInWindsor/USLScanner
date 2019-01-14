// SafetyRegionPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "SafetyRegionPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSafetyRegionPage dialog

IMPLEMENT_DYNCREATE(CSafetyRegionPage, CPropertyPage)


CSafetyRegionPage::CSafetyRegionPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CSafetyRegionPage::IDD)
{
	//{{AFX_DATA_INIT(CSafetyRegionPage)
	//}}AFX_DATA_INIT
}


void CSafetyRegionPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSafetyRegionPage)
	DDX_Control(pDX, IDC_LIST_PERMISSABLE_FUNCTIONS, m_listPermissableFunctions);
	DDX_Control(pDX, IDC_LIST_AXES, m_listAxes);
	DDX_Control(pDX, IDC_COMBO_REGION, m_comboRegion);
	DDX_Control(pDX, IDC_CHECK_ENABLE_JOYSTICK, m_checkEnableJoystick);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSafetyRegionPage, CDialog)
	//{{AFX_MSG_MAP(CSafetyRegionPage)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_AXES, OnGetdispinfoListAxes)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_PERMISSABLE_FUNCTIONS, OnGetdispinfoListPermissableFunctions)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AXES, OnDblclkListAxes)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PERMISSABLE_FUNCTIONS, OnDblclkListPermissableFunctions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSafetyRegionPage message handlers

void CSafetyRegionPage::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnClose();
}

void CSafetyRegionPage::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CSafetyRegionPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CreateColumns();
	FillList();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSafetyRegionPage::CreateColumns()
{

	CString AxesName[4] = {_T("#"),_T("Axis"),_T("Dir"),_T("Position")};
	int AxesWidths[4] = {10,40,40,80};
	int i;

	for(i=0;i<4;i++)
	{
		m_listAxes.InsertColumn(i,AxesName[i],LVCFMT_CENTER,AxesWidths[i]);
	}
	m_listAxes.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	CString FuncName[2] = {_T("#"),_T("Function")};
	int FuncWidths[2] = {10,80};

	for(i=0;i<2;i++)
	{
		m_listPermissableFunctions.InsertColumn(i,FuncName[i],LVCFMT_CENTER,FuncWidths[i]);
	}
	m_listPermissableFunctions.SetExtendedStyle(LVS_EX_FULLROWSELECT);

}

void CSafetyRegionPage::FillList()
{
	APP App = (APP) AfxGetApp();
	CString Buff;
	int i;

	m_listAxes.DeleteAllItems();
	for(i=0;i<App->m_Tank.SafetyRegion[0].AxesL;i++)
	{
		Buff.Format(_T("%d"),i+1);
		m_listAxes.InsertItem(i,Buff.GetBuffer(255),i);
	}
	Buff=_T("Add New");
	m_listAxes.InsertItem(i,Buff.GetBuffer(255),i);

	m_listPermissableFunctions.DeleteAllItems();
	for(i=0;i<App->m_Tank.SafetyRegion[0].FunctionL;i++)
	{
		Buff.Format(_T("%d"),i+1);
		m_listPermissableFunctions.InsertItem(i,Buff.GetBuffer(255),i);
	}
	Buff=_T("Add New");
	m_listPermissableFunctions.InsertItem(i,Buff.GetBuffer(255),i);


}



void CSafetyRegionPage::OnGetdispinfoListAxes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	APP App = (APP)AfxGetApp();
	
	static WCHAR str[100];

	str[0]=0;
	switch(pDispInfo->item.iSubItem){
	case 1: swprintf(str, _T("%d"), App->m_Tank.SafetyRegion[0].Axis[pDispInfo->item.iItem]);
		break;
	case 2: 
		if(App->m_Tank.SafetyRegion[0].Direction[pDispInfo->item.iItem]) {
			swprintf(str, _T("<"));
		}else{
			swprintf(str, _T("<"));
		}
		break;
	case 3:
		swprintf(str, _T("%0.2f"), App->m_Tank.SafetyRegion[0].Position[pDispInfo->item.iItem]);
		break;
	}

	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CSafetyRegionPage::OnGetdispinfoListPermissableFunctions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	APP App = (APP)AfxGetApp();	
	static WCHAR str[100];

	str[0]=0;
	switch(pDispInfo->item.iSubItem){
	case 1: swprintf(str, _T("%s"), App->m_Tank.SafetyRegion[0].PermissableFunctions[pDispInfo->item.iItem]);
		break;
	}

	pDispInfo->item.pszText=str;

	
	*pResult = 0;
}

void CSafetyRegionPage::OnDblclkListAxes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	APP App = (APP) AfxGetApp();
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;
	CString Axes[12] = {_T("1"),_T("2"),_T("3"),_T("4"),_T("5"),_T("6"),_T("7"),_T("8"),_T("9"),_T("10"),_T("11"),_T("12")};
	CString Dir[2] = {_T("<"),_T(">")};
	WCHAR mm[10] = _T("mm");

	if(pDispInfo->item.mask==App->m_Tank.SafetyRegion[0].AxesL){
		//Add new function to list
		App->m_Tank.SafetyRegion[0].Axis[App->m_Tank.SafetyRegion[0].AxesL]=0;
		App->m_Tank.SafetyRegion[0].Direction[App->m_Tank.SafetyRegion[0].AxesL]=0;
		App->m_Tank.SafetyRegion[0].Position[App->m_Tank.SafetyRegion[0].AxesL]=0;
		App->m_Tank.SafetyRegion[0].AxesL++;
		CreateColumns();
		FillList();
		Invalidate();
		return;
	}


	m_listAxes.GetWindowRect(CtrlRect);
	m_listAxes.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);
	
	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:	m_pEditSpinItem->Initialize(&App->m_Tank.SafetyRegion[0].Axis[pDispInfo->item.mask],Axes,12);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 2: m_pEditSpinItem->Initialize(&App->m_Tank.SafetyRegion[0].Direction[pDispInfo->item.mask],Dir,2);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 3:	m_pEditSpinItem->Initialize(&App->m_Tank.SafetyRegion[0].Position[pDispInfo->item.mask],0,10000,1,mm);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	}
	
	*pResult = 0;
}

void CSafetyRegionPage::OnDblclkListPermissableFunctions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	APP App = (APP) AfxGetApp();
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;
	CString Functions[12] = {_T("1"),_T("2"),_T("3"),_T("4"),_T("5"),_T("6"),_T("7"),_T("8"),_T("9"),_T("10"),_T("11"),_T("12")};


	if(pDispInfo->item.mask==App->m_Tank.SafetyRegion[0].FunctionL){
		//Add new function to list
		App->m_Tank.SafetyRegion[0].PermissableFunctions[App->m_Tank.SafetyRegion[0].FunctionL]=WAIT_FOR_MOVE_FINISHED;
		App->m_Tank.SafetyRegion[0].FunctionL++;
		CreateColumns();
		FillList();
		Invalidate();
		return;
	}


	m_listAxes.GetWindowRect(CtrlRect);
	m_listAxes.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);
	
	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:	m_pEditSpinItem->Initialize((int*)&App->m_Tank.SafetyRegion[0].PermissableFunctions[pDispInfo->item.mask],Functions,12);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	}
	
	*pResult = 0;
}

void CSafetyRegionPage::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}
