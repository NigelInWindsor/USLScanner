// RobotArmPositionsPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotArmPositionsPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotArmPositionsPage property page

IMPLEMENT_DYNCREATE(CRobotArmPositionsPage, CPropertyPage)

CRobotArmPositionsPage::CRobotArmPositionsPage() : CPropertyPage(CRobotArmPositionsPage::IDD)
{
	//{{AFX_DATA_INIT(CRobotArmPositionsPage)
	//}}AFX_DATA_INIT
	m_nJoystickMode = 0;
	m_nSide = 0;
	m_nJoint = 1;
	m_bLbDown = false;
	theApp.m_LastSettings.nRobotJogMode = 0;

	m_nIndex = 0;
	m_nWhichSet = 0;

	m_pEditSpinItem = NULL;

	MinMax(&theApp.m_nArmCount[0], 0, MAX_ARM_ENTRIES);
	MinMax(&theApp.m_nArmCount[1], 0, MAX_ARM_ENTRIES);
}

CRobotArmPositionsPage::~CRobotArmPositionsPage()
{
	SAFE_DELETE(m_pEditSpinItem);
}

void CRobotArmPositionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotArmPositionsPage)
	DDX_Control(pDX, IDC_LIST_ARM, m_listArm);
	DDX_Control(pDX, IDC_COMBO_JOYSTICK_MODE, m_comboJoystickMode);
	DDX_Control(pDX, IDC_CHECK_RIGHT, m_checkRight);
	DDX_Control(pDX, IDC_CHECK_LEFT, m_checkLeft);
	DDX_Control(pDX, IDC_CHECK_JOINT_6, m_checkJoint6);
	DDX_Control(pDX, IDC_CHECK_JOINT_5, m_checkJoint5);
	DDX_Control(pDX, IDC_CHECK_JOINT_4, m_checkJoint4);
	DDX_Control(pDX, IDC_CHECK_JOINT_3, m_checkJoint3);
	DDX_Control(pDX, IDC_CHECK_JOINT_2, m_checkJoint2);
	DDX_Control(pDX, IDC_CHECK_JOINT_1, m_checkJoint1);
	DDX_Control(pDX, IDC_SPIN_DIRECTION, m_spinDirection);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_SET, m_comboSet);
}


BEGIN_MESSAGE_MAP(CRobotArmPositionsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRobotArmPositionsPage)
	ON_CBN_SELCHANGE(IDC_COMBO_JOYSTICK_MODE, OnSelchangeComboJoystickMode)
	ON_BN_CLICKED(IDC_CHECK_LEFT, OnCheckLeft)
	ON_BN_CLICKED(IDC_CHECK_RIGHT, OnCheckRight)
	ON_BN_CLICKED(IDC_CHECK_JOINT_1, OnCheckJoint1)
	ON_BN_CLICKED(IDC_CHECK_JOINT_2, OnCheckJoint2)
	ON_BN_CLICKED(IDC_CHECK_JOINT_3, OnCheckJoint3)
	ON_BN_CLICKED(IDC_CHECK_JOINT_4, OnCheckJoint4)
	ON_BN_CLICKED(IDC_CHECK_JOINT_5, OnCheckJoint5)
	ON_BN_CLICKED(IDC_CHECK_JOINT_6, OnCheckJoint6)
	ON_COMMAND(ID_ARMS_ADD, OnArmsAdd)
	ON_COMMAND(ID_ARMS_DELETE, OnArmsDelete)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ARM, OnClickListArm)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ARM, OnDblclkListArm)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ARM, OnRclickListArm)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ARM, OnGetdispinfoListArm)
	ON_COMMAND(ID_ARMS_GOTOLEFT, OnArmsGotoleft)
	ON_COMMAND(ID_ARMS_GOTORIGHT, OnArmsGotoright)
	ON_COMMAND(ID_ARMS_MODIFYLEFT, OnArmsModifyleft)
	ON_COMMAND(ID_ARMS_MODIFYRIGHT, OnArmsModifyright)
	ON_COMMAND(ID_ARMS_GOTOBOTH, OnArmsGotoboth)
	ON_COMMAND(ID_ARMS_MOVEJUSTLEFTTOWER, OnArmsMovejustlefttower)
	ON_COMMAND(ID_ARMS_MOVEJUSTRIGHTTOWER, OnArmsMovejustrighttower)
	ON_COMMAND(ID_ARMS_MOVEALL, OnArmsMoveall)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_POWER_MOTORS, OnButtonPowerMotors)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_BUTTON_UNTWIST_0, OnButtonUntwist0)
	ON_COMMAND(ID_BUTTON_UNTWIST_1, OnButtonUntwist1)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_CBN_SELCHANGE(IDC_COMBO_SET, &CRobotArmPositionsPage::OnCbnSelchangeComboSet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotArmPositionsPage message handlers
int CRobotArmPositionsPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_ARM_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,270,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	
	return 0;
}

BOOL CRobotArmPositionsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_comboJoystickMode.AddString(L"Tool center joystick");
	m_comboJoystickMode.AddString(L"Joint jogs on buttons");

	m_comboSet.AddString(L"Global");
	m_comboSet.AddString(L"Profile");

	CreateColumns();
	FillList();

	UpdateAllControls();
	EnableToolTips(TRUE);
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRobotArmPositionsPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

BOOL CRobotArmPositionsPage::OnSetActive() 
{
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	SetTimer(1,500,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CRobotArmPositionsPage::OnKillActive() 
{
	
	SAFE_DELETE( m_pEditSpinItem );
	
	KillTimer(1);

	return CPropertyPage::OnKillActive();
}


void CRobotArmPositionsPage::CreateColumns()
{
	
	int ColumnWidth[] = { 0,100,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60};
	CString ColumnName[] = {L"",L"Name", L"J1 L",L"J2",L"J3",L"J4",L"J5",L"J6",L"J1 R",L"J2",L"J3",L"J4",L"J5",L"J6", L"T1 L", L"T2", L"T3", L"T1 R", L"T2", L"T3" };

	int nColumnCount = (&m_listArm.GetHeaderCtrl())->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listArm.DeleteColumn(0);
	}
	for (int ii=0;ii<20;ii++) {
		m_listArm.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listArm.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CRobotArmPositionsPage::FillList()
{
	CString Buff;

	m_listArm.DeleteAllItems();
	for (int ii=0;ii<theApp.m_nArmCount[m_nWhichSet];ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listArm.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listArm.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<theApp.m_nArmCount[m_nWhichSet];ii++)
		m_listArm.SetItemState(ii , 0,LVIS_SELECTED);
	m_listArm.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listArm.SetFocus();

}

void CRobotArmPositionsPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_comboJoystickMode.SetCurSel(m_nJoystickMode);
	m_comboSet.SetCurSel(m_nWhichSet);

	SetButtonStates();
}


void CRobotArmPositionsPage::SetButtonStates()
{

	m_checkLeft.EnableWindow(m_nJoystickMode);
	m_checkRight.EnableWindow(m_nJoystickMode);
	m_checkJoint1.EnableWindow(m_nJoystickMode);
	m_checkJoint2.EnableWindow(m_nJoystickMode);
	m_checkJoint3.EnableWindow(m_nJoystickMode);
	m_checkJoint4.EnableWindow(m_nJoystickMode);
	m_checkJoint5.EnableWindow(m_nJoystickMode);
	m_checkJoint6.EnableWindow(m_nJoystickMode);

	switch(m_nSide) {
	case 0: m_checkLeft.SetCheck(1);
		 m_checkRight.SetCheck(0);
		 break;
	case 1: m_checkLeft.SetCheck(0);
		 m_checkRight.SetCheck(1);
		 break;
	}
	m_nJoint == 1 ? m_checkJoint1.SetCheck(1) : m_checkJoint1.SetCheck(0);
	m_nJoint == 2 ? m_checkJoint2.SetCheck(1) : m_checkJoint2.SetCheck(0);
	m_nJoint == 3 ? m_checkJoint3.SetCheck(1) : m_checkJoint3.SetCheck(0);
	m_nJoint == 4 ? m_checkJoint4.SetCheck(1) : m_checkJoint4.SetCheck(0);
	m_nJoint == 5 ? m_checkJoint5.SetCheck(1) : m_checkJoint5.SetCheck(0);
	m_nJoint == 6 ? m_checkJoint6.SetCheck(1) : m_checkJoint6.SetCheck(0);
}


void CRobotArmPositionsPage::OnSelchangeComboJoystickMode() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	m_nJoystickMode = m_comboJoystickMode.GetCurSel();
	switch(m_nJoystickMode) {
	case 0:// pFrame->SendMessage(ID_BUTTON_JOYSTICK);
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		break;
	case 1: theApp.m_FBCtrl.KillAllMotionPrograms();
		break;
	}
	SetButtonStates();
}

void CRobotArmPositionsPage::OnCheckLeft() 
{
	m_nSide = 0;
	SetButtonStates();
	
}

void CRobotArmPositionsPage::OnCheckRight() 
{
	m_nSide = 1;
	SetButtonStates();
	
}

void CRobotArmPositionsPage::OnCheckJoint1() 
{
	m_nJoint = 1;
	SetButtonStates();	
}

void CRobotArmPositionsPage::OnCheckJoint2() 
{
	m_nJoint = 2;
	SetButtonStates();	
}

void CRobotArmPositionsPage::OnCheckJoint3() 
{
	m_nJoint = 3;
	SetButtonStates();	
}

void CRobotArmPositionsPage::OnCheckJoint4() 
{
	m_nJoint = 4;
	SetButtonStates();	
}

void CRobotArmPositionsPage::OnCheckJoint5() 
{
	m_nJoint = 5;
	SetButtonStates();	
}

void CRobotArmPositionsPage::OnCheckJoint6() 
{
	m_nJoint = 6;
	SetButtonStates();	
}

BOOL CRobotArmPositionsPage::PreTranslateMessage(MSG* pMsg) 
{
	
	CButton *pButton;
	CRect rr;
	int nDir;
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	switch(pMsg->message) {
	case 0x100:
		switch(pMsg->wParam) {
		case CURSOR_DOWN:
			break;
		case CURSOR_UP:
			break;
		}
		break;
	case 0x201://LBUTTONDOWN
		if(m_bLbDown == false) {
			pButton = (CButton*)GetDlgItem(IDC_SPIN_DIRECTION); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				if(point.x < rr.CenterPoint().x) {
					nDir = -1;
				} else {
					nDir = +1;
				};


				switch(theApp.m_LastSettings.nRobotJogMode) {
				case 0: JointJog(nDir);
					break;
				}
			}
			m_bLbDown = true;
		};
		break;

	case 0x202:	//LBUTTONUP
		if(m_bLbDown == true) {
			switch(theApp.m_LastSettings.nRobotJogMode) {
			case 0: JointJog(0);
				break;
			}
			m_bLbDown = false;
		};

		break;
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CRobotArmPositionsPage::JointJog(int nDir)
{
	
	CString Buff;

	switch(m_nSide) {
	case 0: Buff.Format(L"#%dJ",m_nJoint + 3);
		break;
	case 1: Buff.Format(L"#%dJ",m_nJoint + 13);
		break;
	}
	switch(nDir) {
	case -1: Buff += "-";
		break;
	case 0: Buff += "/";
		break;
	case 1: Buff += "+";
		break;
	};
	theApp.m_FBCtrl.SendStr(Buff);
}



void CRobotArmPositionsPage::OnArmsAdd() 
{
	SAFE_DELETE( m_pEditSpinItem );
	if (theApp.m_nArmCount[m_nWhichSet] < (MAX_ARM_ENTRIES - 1)) {
		m_nIndex = theApp.m_nArmCount[m_nWhichSet];
		theApp.m_nArmCount[m_nWhichSet]++;

		FillArmEntry(m_nIndex, 3);

		FillList();
	}
}

void CRobotArmPositionsPage::OnArmsDelete() 
{
	int	ii;

	for(ii=m_nIndex;ii<theApp.m_nArmCount[m_nWhichSet]; ii++) {
		CopyMemory(&theApp.m_Arm[m_nWhichSet][ii], &theApp.m_Arm[m_nWhichSet][ii+1], sizeof theApp.m_Arm[m_nWhichSet][0]);
	}
	theApp.m_nArmCount[m_nWhichSet]--;
	if (theApp.m_nArmCount[m_nWhichSet] < 0) theApp.m_nArmCount[m_nWhichSet] = 0;
	FillList();
}




BOOL CRobotArmPositionsPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
	
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case ID_BUTTON_POWER_MOTORS: strTipText = L"Power motors";
		break;
	case ID_BUTTON_UNTWIST_0: strTipText.Format(L"Un-twist %s", theApp.m_DlgSideName[0]);
		break;
	case ID_BUTTON_UNTWIST_1: strTipText.Format(L"Un-twist %s", theApp.m_DlgSideName[1]);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


void CRobotArmPositionsPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnLButtonDown(nFlags, point);
}


void CRobotArmPositionsPage::OnClickListArm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	SAFE_DELETE(m_pEditSpinItem);
	m_nIndex = pDispInfo->item.mask;

	*pResult = 0;
}

void CRobotArmPositionsPage::OnDblclkListArm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	SAFE_DELETE( m_pEditSpinItem );
	m_nIndex=pDispInfo->item.mask;

	if((m_nIndex>=0) && (m_nIndex<MAX_ARM_ENTRIES)) {
		m_listArm.GetWindowRect(CtrlRect);
		m_listArm.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
		Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
		Rect.InflateRect(0,1,0,2);

		m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

		switch(pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:	m_pEditSpinItem->Initialize(theApp.m_Arm[m_nWhichSet][m_nIndex].wName);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
			break;
		}
	}	
	*pResult = 0;
}

void CRobotArmPositionsPage::OnRclickListArm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;
	CString Buff,Danger, Goto, Modify;

	Goto.LoadString(IDS_Goto);
	Modify.LoadString(IDS_Modify);

	POSITION pos = m_listArm.GetFirstSelectedItemPosition();
	m_nIndex=m_listArm.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_ARMS_MENU)) {

		pPopup = menu->GetSubMenu(0);

		Buff.Format(L"Move %s arm",theApp.m_DlgSideName[0]);
		pPopup->ModifyMenu(ID_ARMS_GOTOLEFT,MF_BYCOMMAND,ID_ARMS_GOTOLEFT,Buff);
		Buff.Format(L"Move %s arm",theApp.m_DlgSideName[1]);
		pPopup->ModifyMenu(ID_ARMS_GOTORIGHT,MF_BYCOMMAND,ID_ARMS_GOTORIGHT,Buff);

		Buff.Format(L"%s %s",Modify, theApp.m_DlgSideName[0]);
		pPopup->ModifyMenu(ID_ARMS_MODIFYLEFT,MF_BYCOMMAND,ID_ARMS_MODIFYLEFT,Buff);
		Buff.Format(L"%s %s",Modify, theApp.m_DlgSideName[1]);
		pPopup->ModifyMenu(ID_ARMS_MODIFYRIGHT,MF_BYCOMMAND,ID_ARMS_MODIFYRIGHT,Buff);
		
		Buff.Format(L"Move just %s tower",theApp.m_DlgSideName[0]);
		pPopup->ModifyMenu(ID_ARMS_MOVEJUSTLEFTTOWER,MF_BYCOMMAND,ID_ARMS_MOVEJUSTLEFTTOWER,Buff);
		Buff.Format(L"Move just %s tower",theApp.m_DlgSideName[1]);
		pPopup->ModifyMenu(ID_ARMS_MOVEJUSTRIGHTTOWER,MF_BYCOMMAND,ID_ARMS_MOVEJUSTRIGHTTOWER,Buff);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	*pResult = 0;
}

void CRobotArmPositionsPage::OnGetdispinfoListArm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nRow, nColumn;

	nRow = pDispInfo->item.iItem;

	switch(nColumn = pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		swprintf_s(str,_T("%s"),theApp.m_Arm[m_nWhichSet][nRow].wName);
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		swprintf_s(str,_T("%.02f\x0b0"),theApp.m_Arm[m_nWhichSet][nRow].fAngle[nColumn - 2]);
		break;
	case 14: swprintf_s(str, _T("%.02f\x0b0"), theApp.m_Arm[m_nWhichSet][nRow].vCartesian[0].x);
		break;
	case 15: swprintf_s(str, _T("%.02f\x0b0"), theApp.m_Arm[m_nWhichSet][nRow].vCartesian[0].y);
		break;
	case 16: swprintf_s(str, _T("%.02f\x0b0"), theApp.m_Arm[m_nWhichSet][nRow].vCartesian[0].z);
		break;
	case 17: swprintf_s(str, _T("%.02f\x0b0"), theApp.m_Arm[m_nWhichSet][nRow].vCartesian[1].x);
		break;
	case 18: swprintf_s(str, _T("%.02f\x0b0"), theApp.m_Arm[m_nWhichSet][nRow].vCartesian[1].y);
		break;
	case 19: swprintf_s(str, _T("%.02f\x0b0"), theApp.m_Arm[m_nWhichSet][nRow].vCartesian[1].z);
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}




void CRobotArmPositionsPage::OnArmsModifyleft() 
{
	

	if(m_nIndex>=0 && m_nIndex<theApp.m_nArmCount[m_nWhichSet]) {
		FillArmEntry(m_nIndex, 1);
		FillList();
	}
}

void CRobotArmPositionsPage::OnArmsModifyright() 
{
	

	if(m_nIndex>=0 && m_nIndex<theApp.m_nArmCount[m_nWhichSet]) {
		FillArmEntry(m_nIndex, 2);
		FillList();
	}
}

void CRobotArmPositionsPage::OnArmsGotoleft() 
{
	ArmGoto(1);
}

void CRobotArmPositionsPage::OnArmsGotoright() 
{
	ArmGoto(2);
}

void CRobotArmPositionsPage::OnArmsGotoboth() 
{
	ArmGoto(3);
}

void CRobotArmPositionsPage::OnArmsMovejustlefttower() 
{
	TowerGoto(1);
	
}

void CRobotArmPositionsPage::OnArmsMovejustrighttower() 
{
	TowerGoto(2);
	
}

void CRobotArmPositionsPage::TowerGoto(int nSide) 
{
	
	int	ii;
	
	if(m_nIndex < 0) return;
	if(m_nIndex >= theApp.m_nArmCount[m_nWhichSet]) return;
	for(ii=0;ii<6;ii++) 
		theApp.m_Thread.m_CpMoveTo.Side0.fPos[ii+6] = theApp.m_Arm[m_nWhichSet][m_nIndex].fCartesian[ii];
	for(ii=0;ii<6;ii++) 
		theApp.m_Thread.m_CpMoveTo.Side1.fPos[ii+6] = theApp.m_Arm[m_nWhichSet][m_nIndex].fCartesian[ii + 3];

	theApp.m_Thread.m_nJogRobotSide = nSide;
	theApp.m_Thread.m_nJogWhatToPosition = _JOG_TOWER;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = JOG_TO_POSITION;
}

void CRobotArmPositionsPage::ArmGoto(int nSide) 
{
	
	int	ii;
	
	if(m_nIndex < 0) return;
	if(m_nIndex >= theApp.m_nArmCount[m_nWhichSet]) return;
	for(ii=0;ii<6;ii++) 
		theApp.m_Thread.m_CpMoveTo.Side0.fPos[ii] = theApp.m_Arm[m_nWhichSet][m_nIndex].fAngle[ii];
	for(ii=0;ii<6;ii++) 
		theApp.m_Thread.m_CpMoveTo.Side1.fPos[ii] = theApp.m_Arm[m_nWhichSet][m_nIndex].fAngle[ii + 6];

	theApp.m_Thread.m_nJogRobotSide = nSide;
	theApp.m_Thread.m_nJogWhatToPosition = _JOG_ROBOT;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = JOG_TO_POSITION;
}


void CRobotArmPositionsPage::OnArmsMoveall() 
{
	
	int	ii;
	
	if(m_nIndex < 0) return;
	if(m_nIndex >= theApp.m_nArmCount[m_nWhichSet]) return;

	for(ii=0;ii<6;ii++) 
		theApp.m_Thread.m_CpMoveTo.Side0.fPos[ii+6] = theApp.m_Arm[m_nWhichSet][m_nIndex].fCartesian[ii];
	for(ii=0;ii<6;ii++) 
		theApp.m_Thread.m_CpMoveTo.Side1.fPos[ii+6] = theApp.m_Arm[m_nWhichSet][m_nIndex].fCartesian[ii + 3];

	for(ii=0;ii<6;ii++) 
		theApp.m_Thread.m_CpMoveTo.Side0.fPos[ii] = theApp.m_Arm[m_nWhichSet][m_nIndex].fAngle[ii];
	for(ii=0;ii<6;ii++) 
		theApp.m_Thread.m_CpMoveTo.Side1.fPos[ii] = theApp.m_Arm[m_nWhichSet][m_nIndex].fAngle[ii + 6];
	
	theApp.m_Thread.m_nJogRobotSide = 3;
	theApp.m_Thread.m_nJogWhatToPosition = _JOG_ROBOT | _JOG_TOWER;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = JOG_TO_POSITION;
}


void CRobotArmPositionsPage::OnButtonPowerMotors() 
{
	if(theApp.m_FBCtrl.IsMotorsEnabled(theApp.m_LastSettings.nCoordMask) == true) {
		theApp.m_FBCtrl.KillMotors();
	} else {
		theApp.m_FBCtrl.EnableMotors();
	};
	
}


void CRobotArmPositionsPage::SetToolBarCheckedState()
{
	int nStyle;

	if(theApp.m_FBCtrl.IsMotorsEnabled(theApp.m_LastSettings.nCoordMask) == true) {
		nStyle = TBBS_CHECKED;
	} else {
		nStyle = 0;
	}
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_POWER_MOTORS),nStyle );
}

void CRobotArmPositionsPage::OnTimer(UINT nIDEvent) 
{
	SetToolBarCheckedState();
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CRobotArmPositionsPage::Untwist(int nSide) 
{
	CCoord Cp;

	theApp.m_Ethercat.InvalidateRobotJoints();

	theApp.m_Thread.m_nJogRobotSide = nSide;

	if(nSide & 1) {
		theApp.m_Thread.m_CpMoveTo.Side0.fPos[0] = theApp.m_Robot[0].m_fJointAngle[0] * RAD_TO_DEG;
		theApp.m_Thread.m_CpMoveTo.Side0.fPos[1] = theApp.m_Robot[0].m_fJointAngle[1] * RAD_TO_DEG;
		theApp.m_Thread.m_CpMoveTo.Side0.fPos[2] = theApp.m_Robot[0].m_fJointAngle[2] * RAD_TO_DEG;

		if(theApp.m_Robot[0].m_fJointAngle[3] >= 0.0f) {
			theApp.m_Thread.m_CpMoveTo.Side0.fPos[3] = theApp.m_Robot[0].m_fJointAngle[3] * RAD_TO_DEG - 180.0f;
		} else {
			theApp.m_Thread.m_CpMoveTo.Side0.fPos[3] = theApp.m_Robot[0].m_fJointAngle[3] * RAD_TO_DEG + 180.0f;
		}

		theApp.m_Thread.m_CpMoveTo.Side0.fPos[4] = theApp.m_Robot[0].m_fJointAngle[4] * RAD_TO_DEG * -1.0f;

		if(theApp.m_Robot[0].m_fJointAngle[5] >= 0.0f) {
			theApp.m_Thread.m_CpMoveTo.Side0.fPos[5] = theApp.m_Robot[0].m_fJointAngle[5] * RAD_TO_DEG - 180.0f;
		} else {
			theApp.m_Thread.m_CpMoveTo.Side0.fPos[5] = theApp.m_Robot[0].m_fJointAngle[5] * RAD_TO_DEG + 180.0f;
		}
	}

	if(nSide & 2) {
		theApp.m_Thread.m_CpMoveTo.Side1.fPos[0] = theApp.m_Robot[1].m_fJointAngle[0] * RAD_TO_DEG;
		theApp.m_Thread.m_CpMoveTo.Side1.fPos[1] = theApp.m_Robot[1].m_fJointAngle[1] * RAD_TO_DEG;
		theApp.m_Thread.m_CpMoveTo.Side1.fPos[2] = theApp.m_Robot[1].m_fJointAngle[2] * RAD_TO_DEG;

		if(theApp.m_Robot[1].m_fJointAngle[3] >= 0.0f) {
			theApp.m_Thread.m_CpMoveTo.Side1.fPos[3] = theApp.m_Robot[1].m_fJointAngle[3] * RAD_TO_DEG - 180.0f;
		} else {
			theApp.m_Thread.m_CpMoveTo.Side1.fPos[3] = theApp.m_Robot[1].m_fJointAngle[3] * RAD_TO_DEG + 180.0f;
		}

		theApp.m_Thread.m_CpMoveTo.Side1.fPos[4] = theApp.m_Robot[1].m_fJointAngle[4] * RAD_TO_DEG * -1.0f;

		if(theApp.m_Robot[1].m_fJointAngle[5] >= 0.0f) {
			theApp.m_Thread.m_CpMoveTo.Side1.fPos[5] = theApp.m_Robot[1].m_fJointAngle[5] * RAD_TO_DEG - 180.0f;
		} else {
			theApp.m_Thread.m_CpMoveTo.Side1.fPos[5] = theApp.m_Robot[1].m_fJointAngle[5] * RAD_TO_DEG + 180.0f;
		}
	}

	theApp.m_Thread.m_nJogWhatToPosition = _JOG_ROBOT;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = JOG_TO_POSITION;

}

void CRobotArmPositionsPage::OnButtonUntwist0() 
{
	Untwist(1);
}

void CRobotArmPositionsPage::OnButtonUntwist1() 
{
	Untwist(2);
}


void CRobotArmPositionsPage::FillArmEntry(int nIndex, int nSideMask)
{
	int ii;

	theApp.m_Ethercat.InvalidateRobotJoints();

	if (nSideMask & 1) {
		for (ii = 0; ii < 6; ii++)
			theApp.m_Arm[m_nWhichSet][nIndex].fAngle[ii] = theApp.m_Robot[0].m_fJointAngle[ii] * RAD_TO_DEG;

		theApp.m_Arm[m_nWhichSet][nIndex].vCartesian[0].x = (float)theApp.m_Axes[0].nPos * theApp.m_Axes[0].fStepSize;
		theApp.m_Arm[m_nWhichSet][nIndex].vCartesian[0].y = (float)theApp.m_Axes[1].nPos * theApp.m_Axes[1].fStepSize;
		theApp.m_Arm[m_nWhichSet][nIndex].vCartesian[0].z = (float)theApp.m_Axes[2].nPos * theApp.m_Axes[2].fStepSize;
	}
	if (nSideMask & 2) {
		for (ii = 0; ii < 6; ii++)
			theApp.m_Arm[m_nWhichSet][nIndex].fAngle[ii + 6] = theApp.m_Robot[1].m_fJointAngle[ii] * RAD_TO_DEG;

		theApp.m_Arm[m_nWhichSet][nIndex].vCartesian[1].x = (float)theApp.m_Axes[10].nPos * theApp.m_Axes[10].fStepSize;
		theApp.m_Arm[m_nWhichSet][nIndex].vCartesian[1].y = (float)theApp.m_Axes[11].nPos * theApp.m_Axes[11].fStepSize;
		theApp.m_Arm[m_nWhichSet][nIndex].vCartesian[1].z = (float)theApp.m_Axes[12].nPos * theApp.m_Axes[12].fStepSize;
	}
}


void CRobotArmPositionsPage::OnCbnSelchangeComboSet()
{
	m_nWhichSet = m_comboSet.GetCurSel();
	FillList();
}


HRESULT CRobotArmPositionsPage::ItemChanged(WPARAM wp, LPARAM lp)
{
	switch (wp) {
	case WM_ITEMFINISHED:	UpdateAllControls();
		break;
	}
	return NULL;
}

