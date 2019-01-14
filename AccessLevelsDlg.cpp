// AccessLevelsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AccessLevelsDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessLevelsDlg dialog


CAccessLevelsDlg::CAccessLevelsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CAccessLevelsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessLevelsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_pEditSpinItem = NULL;
	m_nIndex = 0;
	m_nColumn = 2;

//	m_AccessLevel[0].LoadString(IDS_Any_One);
//	m_AccessLevel[1].LoadString(IDS_Operator);
//	m_AccessLevel[2].LoadString(IDS_Maintenance);
//	m_AccessLevel[3].LoadString(IDS_Supervisor);
//	m_AccessLevel[4].LoadString(IDS_Manufacturer);

	m_nL=0;
	m_nID[m_nL] = ID_ULTRASONICS_SAVEAS;			m_Menu[m_nL++] = "Ultrasonics Save as";
	m_nID[m_nL] = ID_ULTRASONICS_OPEN;				m_Menu[m_nL++] = "Ultrasonics Open";
	m_nID[m_nL] = IDD_PR30_TIMESLOT_DIALOG;			m_Menu[m_nL++] = "Ultrasonics PR30";
	m_nID[m_nL] = ID_ULTRASONICS_PULSERRECEIVERS;	m_Menu[m_nL++] = "Ultrasonics Pulser Receiver";
	m_nID[m_nL] = ID_ULTRASONICS_OSCILLOSCOPE;		m_Menu[m_nL++] = "Ultrasonics Oscilloscope";
	m_nID[m_nL] = ID_ULTRASONICS_SPECTRUMANALYZER;	m_Menu[m_nL++] = "Ultrasonics Spectrum Analyzer";
	m_nID[m_nL] = IDD_SCOPE_DIALOG1;				m_Menu[m_nL++] = "Ultrasonics Oscilloscope Page";
	m_nID[m_nL] = IDD_GATES_PAGE;					m_Menu[m_nL++] = "Ultrasonics Gates Page";
	m_nID[m_nL] = ID_ULTRASONICS_FILTERS;			m_Menu[m_nL++] = "Ultrasonics Filters";
	m_nID[m_nL] = ID_ULTRASONICS_LSABSCAN;			m_Menu[m_nL++] = "Ultrasonics LSA-BScan";
	m_nID[m_nL] = ID_ULTRASONICS_FOCALLAWS;			m_Menu[m_nL++] = "Ultrasonics Focal Laws";
	m_nID[m_nL] = ID_ULTRASONICS_PROBECHARACTERSATION;				m_Menu[m_nL++] = "Ultrasonics Probe Characterisation";
	m_nID[m_nL] = ID_ULTRASONICS_PULSERRECEIVERS_PR30;				m_Menu[m_nL++] = "Ultrasonics PR30";
	m_nID[m_nL] = ID_ULTRASONICS_PULSERRECEIVERS_PHASEDARRAY;		m_Menu[m_nL++] = "Ultrasonics Phased Array";
	m_nID[m_nL] = 0;								m_Menu[m_nL++] = "";
	m_nID[m_nL] = ID_SYSTEM_OPTIONS;				m_Menu[m_nL++] = "System Options";
	m_nID[m_nL] = ID_SYSTEM_CARDS;					m_Menu[m_nL++] = "System Cards";
	m_nID[m_nL] = ID_SYSTEM_DEVICERESOURCES;		m_Menu[m_nL++] = "System Device Resources";
	m_nID[m_nL] = ID_SYSTEM_AXES;					m_Menu[m_nL++] = "System Axes";
	m_nID[m_nL] = ID_SYSTEM_PMAC;					m_Menu[m_nL++] = "System Pmac";
	m_nID[m_nL] = ID_SYSTEM_SERIALJOYSTICK;			m_Menu[m_nL++] = "System Joystick";
	m_nID[m_nL] = ID_SYSTEM_DIRECTORIES;			m_Menu[m_nL++] = "System Directories";
	m_nID[m_nL] = ID_SYSTEM_ACQIRISDIGITIZER;		m_Menu[m_nL++] = "System Digitizer";
	m_nID[m_nL] = ID_SYSTEM_ACCESSLEVELS;			m_Menu[m_nL++] = "System Access Levels";
	m_nID[m_nL] = ID_SYSTEM_USERS;					m_Menu[m_nL++] = "System Users";
	m_nID[m_nL] = ID_SYSTEM_ASSIGNAXES;				m_Menu[m_nL++] = "System Assign Axes";
	m_nID[m_nL] = ID_SYSTEM_HOLDINGFIXTURE;			m_Menu[m_nL++] = "System Holding Fixture";
	m_nID[m_nL] = ID_SYSTEM_KINEMATICS;				m_Menu[m_nL++] = "System Kinematics";
	m_nID[m_nL] = ID_SYSTEM_SAFETYINTERLOCKS;		m_Menu[m_nL++] = "System Safety Interlocks";
	m_nID[m_nL] = ID_SYSTEM_CUSTOMIZE;				m_Menu[m_nL++] = "System Customise";
	m_nID[m_nL] = ID_SYSTEM_ARCHIVE;				m_Menu[m_nL++] = "System Archive";
	m_nID[m_nL] = ID_SYSTEM_EDITSCANDETAILS;		m_Menu[m_nL++] = "System Scan Details";
	m_nID[m_nL] = 0;								m_Menu[m_nL++] = "";
	m_nID[m_nL] = ID_PROFILES_OPEN;					m_Menu[m_nL++] = "Profiles Open";
	m_nID[m_nL] = ID_PROFILES_SAVEAS;				m_Menu[m_nL++] = "Profiles Save As";
	m_nID[m_nL] = ID_PROFILES_EDIT;					m_Menu[m_nL++] = "Profiles Edit";
	m_nID[m_nL] = ID_PROFILES_IMPORT;				m_Menu[m_nL++] = "Profiles Import";
	m_nID[m_nL] = ID_PROFILES_IMPORTDESIGNER;		m_Menu[m_nL++] = "Profiles Import Designer";
	m_nID[m_nL] = ID_PROFILES_IGESFILEIMPORT;		m_Menu[m_nL++] = "Profiles IGES Import";
	m_nID[m_nL] = 0;								m_Menu[m_nL++] = "";
	m_nID[m_nL] = IDD_NORMALIZE_PAGE;				m_Menu[m_nL++] = "Motion Optimize Page";
	m_nID[m_nL] = IDD_CONTOUR_SCAN_DIALOG;			m_Menu[m_nL++] = "Motion Scan Page";
	m_nID[m_nL] = IDD_TURNTABLE_SCAN_PAGE;			m_Menu[m_nL++] = "Motion Turntable Scan Page";
	m_nID[m_nL] = IDD_ORIGIN_PAGE;					m_Menu[m_nL++] = "Motion Origin Page";
	m_nID[m_nL] = IDD_PUMP_PAGE;					m_Menu[m_nL++] = "Motion Pumps Page";
	m_nID[m_nL] = IDD_MOVETO_PAGE;					m_Menu[m_nL++] = "Motion Move To Page";
	m_nID[m_nL] = ID_MOTION_KINEMATICS;				m_Menu[m_nL++] = "Motion Kinematics";
	m_nID[m_nL] = ID_MOTION_BISECT;					m_Menu[m_nL++] = "Motion Bisect";
	m_nID[m_nL] = IDD_DISCS_SHAFTS_PAGE;			m_Menu[m_nL++] = "Motion Disc & Shafts";
	m_nID[m_nL] = 0;								m_Menu[m_nL++] = "";
	m_nID[m_nL] = IDD_PROFILE_SHEET;				m_Menu[m_nL++] = "Profile Edit";
	m_nID[m_nL] = IDD_3D_EDIT_DIALOG;				m_Menu[m_nL++] = "Profile Cartesian Page";
	m_nID[m_nL] = IDD_3D_TURNTABLE_PAGE;			m_Menu[m_nL++] = "Profile Turntable Page";
	m_nID[m_nL] = 0;								m_Menu[m_nL++] = "";
	m_nID[m_nL] = ID_PALETTE_OPEN;					m_Menu[m_nL++] = "Palette Open";
	m_nID[m_nL] = ID_PALETTE_SAVEAS;				m_Menu[m_nL++] = "Palette Save As";
	m_nID[m_nL] = ID_PALETTE_EDIT;					m_Menu[m_nL++] = "Palette Edit";
	m_nID[m_nL] = IDD_PALETTE_AMPLITUDE_DIALOG;		m_Menu[m_nL++] = "Palette Amplitude";
	m_nID[m_nL] = IDD_PALETTE_THICK_DIALOG1;		m_Menu[m_nL++] = "Palette Thickness";
	m_nID[m_nL] = IDD_POROSITY_PAGE;				m_Menu[m_nL++] = "Palette Porosity";
	m_nID[m_nL] = 0;								m_Menu[m_nL++] = "";
	m_nID[m_nL] = ID_BUTTON_JOYSTICK;				m_Menu[m_nL++] = "Button Joystick";
	m_nID[m_nL] = ID_BUTTON_SAFE;					m_Menu[m_nL++] = "Button Safe";
	m_nID[m_nL] = ID_BUTTON_LEFT_PUMP;				m_Menu[m_nL++] = "Button Left Pump";
	m_nID[m_nL] = ID_BUTTON_RIGHT_PUMP;				m_Menu[m_nL++] = "Button Right Pump";
	m_nID[m_nL] = ID_BUTTON_LEFT_AIR;				m_Menu[m_nL++] = "Button Left Air";
	m_nID[m_nL] = ID_BUTTON_RIGHT_AIR;				m_Menu[m_nL++] = "Button Right Air";
	m_nID[m_nL] = 0;								m_Menu[m_nL++] = "";

	m_nID[m_nL] = IDC_LIST_CAL;						m_Menu[m_nL++] = "Thickness calibration table";
	m_nID[m_nL] = IDD_BLADE_MASTER_PAGE;			m_Menu[m_nL++] = "Blade master";
	m_nID[m_nL] = IDD_WORKSPACE_DLG;				m_Menu[m_nL++] = "Work space";


	m_nID[m_nL] = IDC_EDIT_PRF;						m_Menu[m_nL++] = "PRF";
	m_nID[m_nL] = IDC_EDIT_DAC_THRESHOLD;			m_Menu[m_nL++] = "DAC";
	m_nID[m_nL] = IDC_COMBO_FILTER_TYPE;			m_Menu[m_nL++] = "Filter";
	m_nID[m_nL] = IDC_COMBO_AMPLIFIER;				m_Menu[m_nL++] = "Amplifier type";
	m_nID[m_nL] = IDC_COMBO_PREAMP_GAIN;			m_Menu[m_nL++] = "Preamp";
	m_nID[m_nL] = IDC_EDIT_LIN_GAIN;				m_Menu[m_nL++] = "Linear gain";
	m_nID[m_nL] = IDC_EDIT_LOG_GAIN;				m_Menu[m_nL++] = "Log gain";

	m_nID[m_nL] = UI_CHANGE_INTERFACE_GATE;			m_Menu[m_nL++] = "Interface gate";
	m_nID[m_nL] = UI_CHANGE_OTHER_GATES;			m_Menu[m_nL++] = "Other gates";
	m_nID[m_nL] = IDD_TOOL_CHANGER_PAGE;			m_Menu[m_nL++] = "Tool changer";
	m_nID[m_nL] = 0;								m_Menu[m_nL++] = "";
	m_nID[m_nL] = ID_AXIS_PANEWND;					m_Menu[m_nL++] = "Pane: Axis";
	m_nID[m_nL] = ID_AXES_PANEWND;					m_Menu[m_nL++] = "Pane: Axes";
	m_nID[m_nL] = ID_PROPERTIES_PANEWND;			m_Menu[m_nL++] = "Pane: Properties";
}

void CAccessLevelsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessLevelsDlg)
	DDX_Control(pDX, IDC_EDIT_CURRENTLOGONLEVEL, m_editCurrentLogonLevel);
	DDX_Control(pDX, IDC_LIST_ID, m_listID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessLevelsDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessLevelsDlg)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ID, OnClickListId)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ID, OnDblclkListId)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ID, OnGetdispinfoListId)
	ON_BN_CLICKED(IDC_BUTTON_ERROR, OnButtonError)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ID, OnRclickListId)
	ON_COMMAND(ID_ACCESS_CLEARROW, OnAccessClearrow)
	ON_COMMAND(ID_ACCESS_SETROW, OnAccessSetrow)
	ON_COMMAND(ID_ACCESS_SETALL, OnAccessSetall)
	ON_COMMAND(ID_ACCESS_CLEARALL, OnAccessClearall)
	ON_COMMAND(ID_ACCESS_SETCOLUMN, OnAccessSetcolumn)
	ON_COMMAND(ID_ACCESS_CLEARCOLUMN, OnAccessClearcolumn)
	ON_EN_CHANGE(IDC_EDIT_CURRENTLOGONLEVEL, OnChangeEditCurrentlogonlevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessLevelsDlg message handlers

void CAccessLevelsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CAccessLevelsDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CAccessLevelsDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CAccessLevelsDlg::OnDestroy() 
{
	if (CAccessLevelsDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CAccessLevelsDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
}

BOOL CAccessLevelsDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	

	m_StaticPosList.Initialize(this, m_listID, TRUE, TRUE, TRUE, TRUE);

	if (CAccessLevelsDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CAccessLevelsDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
		}
	}

	m_StaticPosList.SetNewPos();

	CreateColumns();
	FillList();

	UpdateAllControls();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAccessLevelsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CAccessLevelsDlg::OnButtonError() 
{
	char *pMem = NULL;
	
	*pMem = 0;
}

void CAccessLevelsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosList.SetNewPos();
}

void CAccessLevelsDlg::CreateColumns()
{
	int ColumnWidth[12] = { 0,180,70,70,70,70,70,70,70,70,70,70};
	CString ColumnName[12];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Name);
	ColumnName[2].LoadString(IDS_Operator);
	ColumnName[3].LoadString(IDS_Level_1);
	ColumnName[4].LoadString(IDS_Level_2);
	ColumnName[5].LoadString(IDS_Level_3);
	ColumnName[6].LoadString(IDS_Supervisor);
	ColumnName[7].LoadString(IDS_Management);
	ColumnName[8].LoadString(IDS_Maintenance);
	ColumnName[9].LoadString(IDS_Manufacturer);
	ColumnName[10].LoadString(IDS_Help_Entry_Point);


	int nColumnCount = (&m_listID.GetHeaderCtrl())->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listID.DeleteColumn(0);
	}
	for (int ii=0;ii<10;ii++) {
		if (ii < 2) {
			m_listID.InsertColumn(ii, ColumnName[ii], LVCFMT_LEFT, ColumnWidth[ii]);
		}
		else {
			m_listID.InsertColumn(ii, ColumnName[ii], LVCFMT_CENTER, ColumnWidth[ii]);
		}
	}
	m_listID.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CAccessLevelsDlg::FillList()
{
	CString Buff;

	m_listID.DeleteAllItems();
	for (int ii=0;ii<m_nL;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listID.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listID.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<m_nL;ii++) 
		m_listID.SetItemState(ii , 0,LVIS_SELECTED);
	m_listID.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listID.SetFocus();

}

void CAccessLevelsDlg::UpdateAllControls()
{
	CString Buff;
	if (GetSafeHwnd() == NULL) return;

	switch(theApp.m_nLogonLevel) {
	case 0: Buff.LoadString(IDS_Anyone);
		break;
	case 1:	Buff.LoadString(IDS_Operator);
		break;
	case 2:	Buff.LoadString(IDS_Level_1);
		break;
	case 3: Buff.LoadString(IDS_Level_2);
		break;
	case 4: Buff.LoadString(IDS_Level_3);
		break;
	case 5: Buff.LoadString(IDS_Supervisor);
		break;
	case 6: Buff.LoadString(IDS_Management);
		break;
	case 7: Buff.LoadString(IDS_Maintenance);
		break;
	case 8: Buff.LoadString(IDS_Manufacturer);
		break;
	}
	m_editCurrentLogonLevel.SetWindowText(Buff);

}

void CAccessLevelsDlg::OnClickListId(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	m_nIndex=pDispInfo->item.mask;
	m_nColumn=pDispInfo->item.iItem;

	ClearEditSpin();
	
	*pResult = 0;
}

void CAccessLevelsDlg::OnDblclkListId(NMHDR* pNMHDR, LRESULT* pResult) 
{
	FRAME;
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	m_listID.GetWindowRect(CtrlRect);
	m_listID.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();

	m_nIndex=pDispInfo->item.mask;
	unsigned char *cAccess = &theApp.m_cAccess[m_nID[m_nIndex]];

	switch(m_nColumn=pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:
		break;
	case 2:*cAccess & OPERATOR ? *cAccess &= ~OPERATOR : *cAccess |= OPERATOR;
		break;
	case 3:*cAccess & LEVEL_1 ? *cAccess &= ~LEVEL_1 : *cAccess |= LEVEL_1;
		break;
	case 4:*cAccess & LEVEL_2 ? *cAccess &= ~LEVEL_2 : *cAccess |= LEVEL_2;
		break;
	case 5:*cAccess & LEVEL_3 ? *cAccess &= ~LEVEL_3 : *cAccess |= LEVEL_3;
		break;
	case 6:*cAccess & SUPERVISOR ? *cAccess &= ~SUPERVISOR : *cAccess |= SUPERVISOR;
		break;
	case 7:*cAccess & MANAGEMENT ? *cAccess &= ~MANAGEMENT : *cAccess |= MANAGEMENT;
		break;
	case 8:*cAccess & MAINTENANCE ? *cAccess &= ~MAINTENANCE : *cAccess |= MAINTENANCE;
		break;
	case 9:*cAccess & MANUFACTURER ? *cAccess &= ~MANUFACTURER : *cAccess |= MANUFACTURER;
		break;
	}
	Invalidate(FALSE);

	pFrame->SendMessage(UI_SET_ACCESS_PRIVELAGES, NULL, NULL);
	
	*pResult = 0;
}

void CAccessLevelsDlg::OnGetdispinfoListId(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	const wchar_t Tick = 0x2714;

	str[0]=0;

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:	swprintf_s(str,100,_T("%s"),(LPCWSTR)m_Menu[pDispInfo->item.iItem]);
		break;
	case 2:	theApp.m_cAccess[m_nID[pDispInfo->item.iItem]] & OPERATOR ? swprintf_s(str, 100, L"%wc", Tick) : swprintf_s(str, 100, L"%s", L"-");
		break;
	case 3:	theApp.m_cAccess[m_nID[pDispInfo->item.iItem]] & LEVEL_1 ? swprintf_s(str, 100, L"%wc", Tick) : swprintf_s(str, 100, L"%s", L"-");
		break;
	case 4:	theApp.m_cAccess[m_nID[pDispInfo->item.iItem]] & LEVEL_2 ? swprintf_s(str, 100, L"%wc", Tick) : swprintf_s(str, 100, L"%s", L"-");
		break;
	case 5:	theApp.m_cAccess[m_nID[pDispInfo->item.iItem]] & LEVEL_3 ? swprintf_s(str, 100, L"%wc", Tick) : swprintf_s(str, 100, L"%s", L"-");
		break;
	case 6:	theApp.m_cAccess[m_nID[pDispInfo->item.iItem]] & SUPERVISOR ? swprintf_s(str, 100, L"%wc", Tick) : swprintf_s(str, 100, L"%s", L"-");
		break;
	case 7:	theApp.m_cAccess[m_nID[pDispInfo->item.iItem]] & MANAGEMENT ? swprintf_s(str, 100, L"%wc", Tick) : swprintf_s(str, 100, L"%s", L"-");
		break;
	case 8:	theApp.m_cAccess[m_nID[pDispInfo->item.iItem]] & MAINTENANCE ? swprintf_s(str, 100, L"%wc", Tick) : swprintf_s(str, 100, L"%s", L"-");
		break;
	case 9:	theApp.m_cAccess[m_nID[pDispInfo->item.iItem]] & MANUFACTURER ? swprintf_s(str, 100, L"%wc", Tick) : swprintf_s(str, 100, L"%s", L"-");
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CAccessLevelsDlg::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}


void CAccessLevelsDlg::OnRclickListId(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	m_nIndex=pDispInfo->item.mask;

	CMenu* menu = (CMenu *) new CMenu;
	CPoint	Point;

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_ACCESS_LEVELS_MENU)) {
		CMenu* pPopup = menu->GetSubMenu(0);
		ASSERT(pPopup != NULL);
				
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;

	*pResult = 0;
}

void CAccessLevelsDlg::OnAccessClearrow() 
{
	theApp.m_cAccess[m_nID[m_nIndex]] = 0;
	Invalidate(FALSE);
}

void CAccessLevelsDlg::OnAccessSetrow() 
{
	theApp.m_cAccess[m_nID[m_nIndex]] = 0xff;
	Invalidate(FALSE);
}

void CAccessLevelsDlg::OnAccessSetall() 
{
	FillMemory(&theApp.m_cAccess,sizeof theApp.m_cAccess,0xff);
	Invalidate(FALSE);
}

void CAccessLevelsDlg::OnAccessClearall() 
{
	FillMemory(&theApp.m_cAccess,sizeof theApp.m_cAccess,0x00);
	Invalidate(FALSE);
}

void CAccessLevelsDlg::OnAccessSetcolumn() 
{
	int nMask = 1;

	nMask <<= (m_nColumn - 2);
	for(int ii=0;ii<(sizeof theApp.m_cAccess);ii++) {
		theApp.m_cAccess[ii] |= nMask;
	}
	Invalidate(FALSE);
	
}

void CAccessLevelsDlg::OnAccessClearcolumn() 
{
	int nMask = 1;

	nMask <<= (m_nColumn - 2);
	for(int ii=0;ii<(sizeof theApp.m_cAccess);ii++) {
		theApp.m_cAccess[ii] &= ~nMask;
	}
	Invalidate(FALSE);
	
}

void CAccessLevelsDlg::OnChangeEditCurrentlogonlevel() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
