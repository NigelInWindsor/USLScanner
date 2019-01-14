// UsersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "UsersDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUsersDlg dialog


CUsersDlg::CUsersDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialogEx(CUsersDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsersDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_pEditSpinItem = NULL;

	m_AccessLevel[0].LoadString(IDS_Anyone);
	m_AccessLevel[1].LoadString(IDS_Operator);
	m_AccessLevel[2].LoadString(IDS_Level_1);
	m_AccessLevel[3].LoadString(IDS_Level_2);
	m_AccessLevel[4].LoadString(IDS_Level_3);
	m_AccessLevel[5].LoadString(IDS_Supervisor);
	m_AccessLevel[6].LoadString(IDS_Management);
	m_AccessLevel[7].LoadString(IDS_Maintenance);
	m_AccessLevel[8].LoadString(IDS_Manufacturer);


}


void CUsersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsersDlg)
	DDX_Control(pDX, IDC_COMBO_FORCE_LOGON, m_comboForceLogon);
	DDX_Control(pDX, IDC_EDIT_LOGOFF_TIME, m_editLogOffTime);
	DDX_Control(pDX, IDC_COMBO_LOGON_LEVEL, m_comboLogonLevel);
	DDX_Control(pDX, IDC_LIST_USERS, m_listUsers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUsersDlg, CDialog)
	//{{AFX_MSG_MAP(CUsersDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_USERS, OnClickListUsers)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USERS, OnDblclkListUsers)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_USERS, OnRclickListUsers)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_USERS, OnGetdispinfoListUsers)
	ON_COMMAND(ID_EDIT_ADD, OnEditAdd)
	ON_CBN_SELCHANGE(IDC_COMBO_LOGON_LEVEL, OnSelchangeComboLogonLevel)
	ON_EN_CHANGE(IDC_EDIT_LOGOFF_TIME, OnChangeEditLogoffTime)
	ON_CBN_SELCHANGE(IDC_COMBO_FORCE_LOGON, OnSelchangeComboForceLogon)
	ON_NOTIFY(HDN_TRACK, IDC_LIST_USERS, OnTrackListUsers)
	ON_NOTIFY(HDN_ENDDRAG, IDC_LIST_USERS, OnEnddragListUsers)
	ON_NOTIFY(HDN_BEGINTRACK, IDC_LIST_USERS, OnBegintrackListUsers)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsersDlg message handlers

void CUsersDlg::PostNcDestroy() 
{
	m_Users.Save();
	
	CDialog::PostNcDestroy();
	delete this;
}

void CUsersDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CUsersDlg::IDD & MAXIMUMWINDOWSIDNUMBER] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CUsersDlg::OnDestroy() 
{
	

	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CUsersDlg::IDD & MAXIMUMWINDOWSIDNUMBER]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CUsersDlg::OnInitDialog() 
{
	
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;
	

	m_StaticPosList.Initialize(this, m_listUsers, TRUE, TRUE, TRUE, TRUE);

	CRect rect = theApp.m_LastSettings.RectWindowPos[CUsersDlg::IDD & MAXIMUMWINDOWSIDNUMBER];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		theApp.ConfineToScreen(&rect);
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	m_StaticPosList.SetNewPos();

	for(int ii=0;ii<9;ii++) {
		m_comboLogonLevel.AddString(m_AccessLevel[ii]);
	}

	Buff.LoadString(IDS_No);	m_comboForceLogon.AddString(Buff);
	Buff.LoadString(IDS_Yes);	m_comboForceLogon.AddString(Buff);
	m_comboForceLogon.SetCurSel(theApp.m_LastSettings.bForceLogon);

	theApp.RetrieveRegistryVariables();
	m_comboLogonLevel.SetCurSel(theApp.m_nInitialLogonLevel);

	Buff.Format(_T("%d min"),theApp.m_LastSettings.nTimoutPeriodMinutes);
	m_editLogOffTime.SetWindowText(Buff);

	

	CreateColumns();
	FillList();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUsersDlg::SetBackgroundColor(COLORREF color, BOOL bRepaint)
{
	SetBackgroundColor(RGB(255, 255, 0), bRepaint);
}

void CUsersDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosList.SetNewPos();
	
}

void CUsersDlg::CreateColumns()
{
	
	int ColumnWidth[10] = { 0,170,100,100,100,50,50,50,80,50};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Name);
	ColumnName[2].LoadString(IDS_Access_Level);
	ColumnName[3].LoadString(IDS_ID);
	ColumnName[4].LoadString(IDS_Password);

	int nColumnCount = m_listUsers.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listUsers.DeleteColumn(0);
	}
	for (int ii=0;ii<5;ii++) {
		m_listUsers.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listUsers.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CUsersDlg::FillList()
{
	

	CString Buff;

	m_listUsers.DeleteAllItems();
	for (int ii=0;ii<m_Users.m_nIDL;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listUsers.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listUsers.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<m_Users.m_nIDL;ii++) 
		m_listUsers.SetItemState(ii , 0,LVIS_SELECTED);
	m_listUsers.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listUsers.SetFocus();

}

void CUsersDlg::UpdateAllControls()
{
	
	if (GetSafeHwnd() == NULL) return;

	m_comboLogonLevel.SetCurSel(theApp.m_nInitialLogonLevel);

}

void CUsersDlg::OnClickListUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearEditSpin();
	
	*pResult = 0;
}

void CUsersDlg::OnDblclkListUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;


	ClearEditSpin();
	m_nIndex=pDispInfo->item.mask;

	if(m_nIndex<0) {
		m_Users.AddEmpty();
		FillList();
		m_nIndex = m_Users.m_nIDL - 1;
	}

	if((m_nIndex>=0) && (m_nIndex<m_Users.m_nIDL)) {
		m_listUsers.GetWindowRect(CtrlRect);
		m_listUsers.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
		Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
		Rect.InflateRect(0,1,0,2);

		m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

		switch(pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:	m_pEditSpinItem->Initialize(m_Users.m_pID[m_nIndex].wName);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
			break;
		case 2: m_pEditSpinItem->Initialize(&m_Users.m_pID[m_nIndex].nAccessLevel,m_AccessLevel,9);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 3:	m_pEditSpinItem->Initialize(m_Users.m_pID[m_nIndex].wID);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
			break;
		case 4:	m_pEditSpinItem->Initialize(m_Users.m_pID[m_nIndex].wPassword);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
			break;
		}
	}	
	*pResult = 0;
}

void CUsersDlg::OnRclickListUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listUsers.GetFirstSelectedItemPosition();
	m_nIndex=m_listUsers.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_EDIT_DESCRIPTION_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	*pResult = 0;
}

void CUsersDlg::OnGetdispinfoListUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn;

	if(m_Users.m_nIDL<=0) return;
	m_nIndex = pDispInfo->item.iItem;
	if(m_Users.m_pID[m_nIndex].nAccessLevel<0) m_Users.m_pID[m_nIndex].nAccessLevel=0;
	if(m_Users.m_pID[m_nIndex].nAccessLevel>8) m_Users.m_pID[m_nIndex].nAccessLevel=8;
	int nLogonLevel = m_Users.m_pID[m_nIndex].nAccessLevel;

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		swprintf_s(str,_T("%s"),m_Users.m_pID[m_nIndex].wName);
		break;
	case 2:
		swprintf_s(str,_T("%s"),m_AccessLevel[nLogonLevel].GetBuffer());
		break;
	case 3:
		swprintf_s(str,_T("%s"),m_Users.m_pID[m_nIndex].wID);
		break;
	case 4:
		nn=wcslen(m_Users.m_pID[m_nIndex].wPassword);
		swprintf_s(str,_T("%-*.*s"),nn,nn,_T("**********************"));
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}



void CUsersDlg::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}

void CUsersDlg::OnEditAdd() 
{
	m_Users.AddEmpty();
	FillList();
}

void CUsersDlg::OnSelchangeComboLogonLevel() 
{
	

	theApp.m_nInitialLogonLevel = m_comboLogonLevel.GetCurSel();
	theApp.SaveRegistryVariables();	
}

void CUsersDlg::OnChangeEditLogoffTime() 
{
	
	CString Buff;

	m_editLogOffTime.GetWindowText(Buff);
	theApp.m_LastSettings.nTimoutPeriodMinutes = _ttoi(Buff);
}

void CUsersDlg::OnSelchangeComboForceLogon() 
{
	
	theApp.m_LastSettings.bForceLogon = m_comboForceLogon.GetCurSel() & 0x01;
	
}


void CUsersDlg::OnTrackListUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CUsersDlg::OnEnddragListUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY * phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CUsersDlg::OnBegintrackListUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CUsersDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}
