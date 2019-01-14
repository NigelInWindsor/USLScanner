// MaintenancePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "MaintenancePage.h"
#include "PermissionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaintenancePage property page

IMPLEMENT_DYNCREATE(CMaintenancePage, CPropertyPage)

CMaintenancePage::CMaintenancePage() : CPropertyPage(CMaintenancePage::IDD)
{
	//{{AFX_DATA_INIT(CMaintenancePage)
	//}}AFX_DATA_INIT

	
	m_nIndex=0;
	m_pEditSpinItem = false;
}

CMaintenancePage::~CMaintenancePage()
{
}

void CMaintenancePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaintenancePage)
	DDX_Control(pDX, IDC_LIST_SCHEDULE, m_listSchedule);
	DDX_Control(pDX, IDC_EDIT_MACHINE_NAME, m_editMachineName);
	DDX_Control(pDX, IDC_EDIT_MACHINE_LOCATION, m_editMachineLocation);
	DDX_Control(pDX, IDC_EDIT_CUSTOMER, m_editCustomer);
	DDX_Control(pDX, IDC_EDIT_INSTALATION_DATE, m_editInstalationDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaintenancePage, CPropertyPage)
	//{{AFX_MSG_MAP(CMaintenancePage)
	ON_BN_CLICKED(IDC_BUTTON_ACCEPT, OnButtonAccept)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SCHEDULE, OnClickListSchedule)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SCHEDULE, OnDblclkListSchedule)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SCHEDULE, OnGetdispinfoListSchedule)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SCHEDULE, OnRclickListSchedule)
	ON_COMMAND(ID_MAINTENANCE_CLEARALLTOTALDISTANCES, OnMaintenanceClearalltotaldistances)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaintenancePage message handlers

BOOL CMaintenancePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	
	CreateColumns();
	FillList();

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMaintenancePage::CreateColumns()
{
	int ColumnWidth[10] = { 60,100,100,140,140,50,50,50,80,50};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Name);
	ColumnName[2].LoadString(IDS_Total_Distance);
	ColumnName[3].LoadString(IDS_Since_Last_Service);

	int nColumnCount = m_listSchedule.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listSchedule.DeleteColumn(0);
	}
	for (int ii=0;ii<5;ii++) {
		m_listSchedule.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listSchedule.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CMaintenancePage::FillList()
{
	CString Buff;

	m_listSchedule.DeleteAllItems();
	for (int ii=0;ii<32;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listSchedule.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listSchedule.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<32;ii++) 
		m_listSchedule.SetItemState(ii , 0,LVIS_SELECTED);
	m_listSchedule.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listSchedule.SetFocus();

}

void CMaintenancePage::UpdateAllControls()
{
	CString Buff;

	if (GetSafeHwnd() == NULL) return;

	Buff.Empty();
	Buff =	theApp.GetProfileString(L"Maintenance",L"Customer",Buff.GetBuffer(1));
	m_editCustomer.SetWindowText(Buff);
	m_editCustomer.SetReadOnly(!Buff.IsEmpty());

	Buff.Empty();
	Buff =	theApp.GetProfileString(L"Maintenance",L"Machine Name",Buff.GetBuffer(1));
	m_editMachineName.SetWindowText(Buff);
	m_editMachineName.SetReadOnly(!Buff.IsEmpty());

	Buff.Empty();
	Buff =	theApp.GetProfileString(L"Maintenance",L"Machine Location",Buff.GetBuffer(1));
	m_editMachineLocation.SetWindowText(Buff);
	m_editMachineLocation.SetReadOnly(!Buff.IsEmpty());

	Buff.Empty();
	Buff =	theApp.GetProfileString(L"Maintenance",L"Instalation Date",Buff.GetBuffer(1));
	m_editInstalationDate.SetWindowText(Buff);
	m_editInstalationDate.SetReadOnly(!Buff.IsEmpty());
}

void CMaintenancePage::OnButtonAccept() 
{
	CString Buff;

	Buff.Empty();
	Buff =	theApp.GetProfileString(L"Maintenance",L"Customer",Buff.GetBuffer(1));
	if(Buff.IsEmpty() == TRUE) {
		m_editCustomer.GetWindowText(Buff);
		theApp.WriteProfileString(L"Maintenance",L"Customer",Buff.GetBuffer(1));
	}

	Buff.Empty();
	Buff =	theApp.GetProfileString(L"Maintenance",L"Machine Name",Buff.GetBuffer(1));
	if(Buff.IsEmpty() == TRUE) {
		m_editMachineName.GetWindowText(Buff);
		theApp.WriteProfileString(L"Maintenance",L"Machine Name",Buff.GetBuffer(1));
	}

	Buff.Empty();
	Buff =	theApp.GetProfileString(L"Maintenance",L"Machine Location",Buff.GetBuffer(1));
	if(Buff.IsEmpty() == TRUE) {
		m_editMachineLocation.GetWindowText(Buff);
		theApp.WriteProfileString(L"Maintenance",L"Machine Location",Buff.GetBuffer(1));
	}

	Buff.Empty();
	Buff =	theApp.GetProfileString(L"Maintenance",L"Instalation Date",Buff.GetBuffer(1));
	if(Buff.IsEmpty() == TRUE) {
		m_editInstalationDate.GetWindowText(Buff);
		theApp.WriteProfileString(L"Maintenance",L"Instalation Date",Buff.GetBuffer(1));
	}
	
	
	
	UpdateAllControls();

}

void CMaintenancePage::OnClickListSchedule(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CMaintenancePage::OnDblclkListSchedule(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	SAFE_DELETE( m_pEditSpinItem );
	m_nIndex=pDispInfo->item.mask;

	if((m_nIndex>=0) && (m_nIndex<32)) {
		m_listSchedule.GetWindowRect(CtrlRect);
		m_listSchedule.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
		Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
		Rect.InflateRect(0,1,0,2);

		m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

		switch(pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:
			m_pEditSpinItem->Initialize(theApp.m_Axes[m_nIndex].cName);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
			break;
		}
	}	
	*pResult = 0;
}

void CMaintenancePage::OnGetdispinfoListSchedule(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	CString strTotalDistance;

	int nAxis = pDispInfo->item.iItem;
	str[0]=0;

	switch(theApp.m_Axes[nAxis].nUnitSelected) {
	case 0:	strTotalDistance.Format(L"%.02g km",theApp.m_Axes[nAxis].fTotalDistance / 1e6);
		break;
	case 1:	strTotalDistance.Format(L"%g/x022",theApp.m_Axes[nAxis].fTotalDistance / 304.8);
		break;
	case 2:	strTotalDistance.Format(L"%.0f%s",theApp.m_Axes[nAxis].fTotalDistance,DEGREES);
		break;
	}

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		swprintf_s(str,100,_T("%s"),theApp.m_Axes[nAxis].cName);
		break;
	case 2:
		swprintf_s(str,100,_T("%s"),(LPCWSTR)strTotalDistance);
		break;
	case 3:
		break;
	case 4:
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}



void CMaintenancePage::OnRclickListSchedule(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listSchedule.GetFirstSelectedItemPosition();
	m_nIndex=m_listSchedule.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_LIST_MAINTENANCE_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	*pResult = 0;
}

void CMaintenancePage::OnMaintenanceClearalltotaldistances() 
{
	CPermissionDlg dlg;
	if((dlg.DoModal() == IDOK) && (dlg.m_bAcceptable == TRUE)) {
		for(int nAxis=0;nAxis<32;nAxis++) {
			theApp.m_Axes[nAxis].fTotalDistance = 0.0f;
		};
		FillList();
		theApp.SaveTank();
	}

}
