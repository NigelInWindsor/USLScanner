// TubeScanPage1.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "TubeScanPage1.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTubeScanPage property page

IMPLEMENT_DYNCREATE(CTubeScanPage, CPropertyPage)

CTubeScanPage::CTubeScanPage() : CPropertyPage(CTubeScanPage::IDD)
{
	//{{AFX_DATA_INIT(CTubeScanPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nIndex=0;
	m_pEditSpinItem = NULL;

	switch(theApp.m_LastSettings.nScanType) {
	default: m_nScanType = CONTOUR_SCAN;
		break;
	case TURNTABLE_SCAN:
		m_nScanType = theApp.m_LastSettings.nScanType;
		break;
	}
}

CTubeScanPage::~CTubeScanPage()
{
}

void CTubeScanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTubeScanPage)
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_editFilePath);
	DDX_Control(pDX, IDC_EDIT_POSITION, m_editPosition);
	DDX_Control(pDX, IDC_SPIN_DIAMETER, m_spinDiameter);
	DDX_Control(pDX, IDC_EDIT_DIAMETER, m_editDiameter);
	DDX_Control(pDX, IDC_EDIT_RESUME_LINE, m_editResumeLine);
	DDX_Control(pDX, IDC_EDIT_SCAN_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_SPIN_SCAN_SPEED, m_spinScanSpeed);
	DDX_Control(pDX, IDC_EDIT_MAX_SPEED, m_editMaxSpeed);
	DDX_Control(pDX, IDC_EDIT_SCAN_LINE, m_editScanLine);
	DDX_Control(pDX, IDC_SPIN_SLOW_INCREMENT, m_spinSlowIncrement);
	DDX_Control(pDX, IDC_SPIN_FAST_INCREMENT, m_spinFastIncrement);
	DDX_Control(pDX, IDC_EDIT_SLOW_INCREMENT, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_FASY_INCREMENT, m_editFastIncrement);
	DDX_Control(pDX, IDC_LIST_COORDS, m_listCoords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTubeScanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTubeScanPage)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, OnGetdispinfoListCoords)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COORDS, OnClickListCoords)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COORDS, OnRclickListCoords)
	ON_COMMAND(ID_TUBES_ADD, OnTubesAdd)
	ON_COMMAND(ID_TUBES_DELETE, OnTubesDelete)
	ON_COMMAND(ID_TUBES_MODIFY, OnTubesModify)
	ON_COMMAND(ID_TUBES_GOTO, OnTubesGoto)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COORDS, OnDblclkListCoords)
	ON_COMMAND(ID_TUBES_CLEARALL, OnTubesClearall)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FAST_INCREMENT, OnDeltaposSpinFastIncrement)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOW_INCREMENT, OnDeltaposSpinSlowIncrement)
	ON_EN_CHANGE(IDC_EDIT_FASY_INCREMENT, OnChangeEditFasyIncrement)
	ON_EN_CHANGE(IDC_EDIT_SLOW_INCREMENT, OnChangeEditSlowIncrement)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_ABORT, OnButtonAbort)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_SCAN_SPEED, OnChangeEditScanSpeed)
	ON_EN_CHANGE(IDC_EDIT_RESUME_LINE, OnChangeEditResumeLine)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIAMETER, OnDeltaposSpinDiameter)
	ON_EN_CHANGE(IDC_EDIT_DIAMETER, OnChangeEditDiameter)
	ON_BN_CLICKED(IDC_BUTTON_ZERO, OnButtonZero)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCAN_SPEED, OnDeltaposSpinScanSpeed)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_ZERO_ROTATE, OnButtonZeroRotate)
	ON_COMMAND(ID_BUTTON_HELICAL_SCAN, OnButtonHelicalScan)
	ON_COMMAND(ID_BUTTON_RECTILINEAR_SCAN, OnButtonRectilinearScan)
	ON_EN_CHANGE(IDC_EDIT_FILEPATH, OnChangeEditFilepath)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_COMMAND(ID_TUBES_SORT_X, OnTubesSortX)
	ON_COMMAND(ID_TUBES_SORT_Y, OnTubesSortY)
	ON_COMMAND(ID_TUBES_SORT_Z, OnTubesSortZ)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTubeScanPage message handlers
int CTubeScanPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_ROLLER_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	
	return 0;
}

BOOL CTubeScanPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_spinFastIncrement.SetRange(0,200);
	m_spinSlowIncrement.SetRange(0,200);

	m_editFilePath.SetWindowText(theApp.m_Data.m_FileName);

	
	CreateColumns();
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTubeScanPage::CreateColumns()
{
	CString ColumnName[6] = { L"#",L"X",L"Y", L"Z", L"Style"};
	int ColumnWidth[6] = { 0,60,60,60,60};

	if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable == false) ColumnWidth[1] = 0;
	if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable == false) ColumnWidth[2] = 0;
	if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable == false) ColumnWidth[3] = 0;

	int nColumnCount = m_listCoords.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listCoords.DeleteColumn(0);
	}
	for (int ii=0;ii<5;ii++) {
		m_listCoords.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listCoords.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CTubeScanPage::FillList()
{
	CString Buff;

	m_listCoords.DeleteAllItems();
	if(PROFILE->m_TaughtLine) {
		for (int ii=0;ii<PROFILE->m_TaughtLine[0].GetSize();ii++) {
			Buff.Format(_T("%d"),ii+1);
			m_listCoords.InsertItem(ii, Buff.GetBuffer(255), ii);
		};

		m_listCoords.EnsureVisible(m_nIndex,FALSE);
	}
}

void CTubeScanPage::UpdateAllControls()
{
	CString Micro = L"\x0b5";
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	FillList();

	m_editFilePath.SetWindowText(theApp.m_Data.m_FileName);

	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fComponentDiameter[0]*theApp.m_fUnits,theApp.m_Units);
	m_editDiameter.SetWindowText(Buff);

	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fFastIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editFastIncrement.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editSlowIncrement.SetWindowText(Buff);

	switch(m_nScanType) {
	default:
		Buff.Format(_T("%.1f %s/s"),PROFILE->m_fScanSpeed,DEGREES);
		break;
	case CONTOUR_SCAN:
		Buff.Format(_T("%.1f mm/s"),PROFILE->m_fScanSpeed);
		break;
	}

	m_editScanSpeed.SetWindowText(Buff);

	Buff.Format(_T("%d"),PROFILE->m_nScanResumeLine+1);
	m_editResumeLine.SetWindowText(Buff);

	theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], true, true);

}

BOOL CTubeScanPage::OnSetActive() 
{
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1,300,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CTubeScanPage::OnKillActive() 
{
	KillTimer(1);	
	return CPropertyPage::OnKillActive();
}

void CTubeScanPage::OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	CCoord Cp;
	CString strAction[3] = {L"Continuous", L"Segment", L"Jump"};
	CString Style[4];
	Style[0].LoadString(IDS_Linear);
	Style[1].LoadString(IDS_Spline);

	str[0]=0;

	if(PROFILE->m_TaughtLine) {
		PROFILE->m_TaughtLine[0].GetCoord(pDispInfo->item.iItem,&Cp);

		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			swprintf_s(str,_T("%.1f"),Cp.Side0.fX);
			break;
		case 2:
			swprintf_s(str,_T("%.1f"),Cp.Side0.fY);
			break;
		case 3:
			swprintf_s(str,_T("%.1f"),Cp.Side0.fZ);
			break;
		case 4:
			if(Cp.nType & TYPE_SPLINE) {
				swprintf_s(str,_T("%s"),Style[1].GetBuffer());
			} else {
				swprintf_s(str,_T("%s"),Style[0].GetBuffer());
			}
			break;
		}
	}
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CTubeScanPage::OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearEditSpin();
	
	*pResult = 0;
}

void CTubeScanPage::OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	m_nIndex=m_listCoords.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_TUBES_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	
	*pResult = 0;
}

void CTubeScanPage::OnDblclkListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;
	CString strAction[3] = {L"Continuous", L"Segment", L"Jump"};
	CCoord* pCp = NULL;
	CString Style[4];
	Style[0].LoadString(IDS_Linear);
	Style[1].LoadString(IDS_Spline);

	m_listCoords.GetWindowRect(CtrlRect);
	m_listCoords.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**)&m_pEditSpinItem,&Rect);

	theApp.m_nIndex = pDispInfo->item.mask;
	if(PROFILE->m_TaughtLine==NULL) return;
	pCp = (CCoord*)&PROFILE->m_TaughtLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];

	switch(m_nColumn=pDispInfo->item.iItem) {
	case 1:
		m_pEditSpinItem->Initialize(&pCp->Side0.fX,-10000.0f,10000.0f,1.0f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 2:
		m_pEditSpinItem->Initialize(&pCp->Side0.fY,-10000.0f,10000.0f,1.0f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 3:
		m_pEditSpinItem->Initialize(&pCp->Side0.fZ,-10000.0f,10000.0f,1.0f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 4:
		pCp->nType & TYPE_SPLINE ? m_nEditType = 1 : m_nEditType = 0;
		m_pEditSpinItem->Initialize(&m_nEditType,Style,2);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	}
	
	*pResult = 0;
}


void CTubeScanPage::OnTubesAdd() 
{
	CCoord CpSurface;

	CpSurface.Zero();
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable == false) CpSurface.Side0.fX = 0.0f;
	if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable == false) CpSurface.Side0.fY = 0.0f;
	if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable == false) CpSurface.Side0.fZ = 0.0f;

	PROFILE->AddCoordToLine(TAUGHTLINES,0,CpSurface);
	FillList();
	GenerateScanLines();

}

void CTubeScanPage::OnTubesDelete() 
{
	PROFILE->DeleteCoordFromLine(TAUGHTLINES,0,m_nIndex);
	FillList();
	GenerateScanLines();
}

void CTubeScanPage::OnTubesClearall() 
{
	CCoord CpSurface;

	CpSurface.Zero();
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable == false) CpSurface.Side0.fX = 0.0f;
	if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable == false) CpSurface.Side0.fY = 0.0f;
	if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable == false) CpSurface.Side0.fZ = 0.0f;
	PROFILE->New();
	GenerateScanLines();

	UpdateAllControls();
}

void CTubeScanPage::OnTubesModify() 
{
	CCoord CpSurface;

	CpSurface.Zero();
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable == false) CpSurface.Side0.fX = 0.0f;
	if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable == false) CpSurface.Side0.fY = 0.0f;
	if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable == false) CpSurface.Side0.fZ = 0.0f;
	PROFILE->m_TaughtLine[0].m_pCp[m_nIndex] = CpSurface;
	FillList();
	GenerateScanLines();
}

void CTubeScanPage::OnTubesGoto() 
{
	CCoord CpSurface,CpComponent;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_Motors.SetRevCount(0);
	PROFILE->m_TaughtLine[0].GetCoord(m_nIndex,&CpComponent);
	CpComponent.Side0.fR = CpSurface.Side0.fR;
	theApp.m_Motors.GotoSurfaceCoord(&CpComponent,-1);
	
}


void CTubeScanPage::ClearEditSpin()
{
	SAFE_DELETE( m_pEditSpinItem );
}




void CTubeScanPage::OnChangeEditFasyIncrement() 
{
	CString Buff;
	float fTemp;

	m_editFastIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fFastIncrement - fTemp) {
		PROFILE->m_fFastIncrement = fTemp;
		UpdateAllControls();
		GenerateScanLines();
	}
	
}

void CTubeScanPage::OnChangeEditSlowIncrement() 
{
	CString Buff;
	float fTemp;

	m_editSlowIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fSlowIncrement - fTemp) {
		PROFILE->m_fSlowIncrement = fTemp;
		UpdateAllControls();
		GenerateScanLines();
	}
	
}




void CTubeScanPage::OnDeltaposSpinFastIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * 0.0254f);
	}
	UpdateAllControls();
	GenerateScanLines();
	
	*pResult = 0;
}

void CTubeScanPage::OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * 0.0254f);
	}
	UpdateAllControls();
	GenerateScanLines();
	
	*pResult = 0;
}

HRESULT CTubeScanPage::ItemChanged(WPARAM wp, LPARAM lp)
{
	CCoord*	pCp = (CCoord*)&PROFILE->m_TaughtLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];

	switch (wp) {
	default:
		switch (m_nColumn) {
		case 4:
			m_nEditType & 1 ? pCp->nType |= TYPE_SPLINE : pCp->nType &= ~TYPE_SPLINE;
			break;
		}
		break;
	case WM_ITEMFINISHED:
		FillList();
		GenerateScanLines();
		break;
	}

	return NULL;
}


void CTubeScanPage::GenerateScanLines()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(PROFILE->m_TaughtLine!=NULL) {
		if(PROFILE->m_TaughtLine[0].m_nCoordL>1) {

			switch(m_nScanType) {
			case TURNTABLE_SCAN:	PROFILE->GenerateRotoSymetricScanLineV2();
				break;
			}
		}
	}

	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG);
}


void CTubeScanPage::OnButtonStart() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	CString Buff,Error,Buff1;

	if(pFrame->IsScanImageStillOpen() == FALSE) {
		if(theApp.m_Data.IsNameRequired() == TRUE) {
			Buff.LoadString(IDS_Please_enter_a_scan_name);
			Error.LoadString(IDS_Error);
			MessageBox(Buff,Error,MB_ICONERROR);
			return;
		}
		if((theApp.m_Data.m_FileName.IsEmpty()) && (theApp.m_LastSettings.nSaveScanEveryLine!=0)) {
			Buff.LoadString(IDS_Save_every_line_is_selected);
			Buff1.LoadString(IDS_please_enter_file_name);
			Error.LoadString(IDS_Error);
			MessageBox(Buff + "\r\n" + Buff1,Error,MB_ICONERROR);
			return;
		}
		theApp.m_nScanWhat = 0;
		PROFILE->setRUsed(true);
		PROFILE->m_nCollectAxis = theApp.m_Tank.nRLeft + 1; //Collect axis uses 1 incremental whereas Tank uses 0 hence +1
		theApp.m_nUseTurntableOrRollers = 0;
	}


	if(pFrame->m_pViewSheet[0]==NULL) {
		GenerateScanLines();
		PROFILE->m_nScanStartLine = 0;
		if((PROFILE->m_nScanLineL>=1) && (PROFILE->m_ScanLine!=NULL)) {
			PROFILE->m_nScanFinishLine = PROFILE->m_ScanLine[0].m_nCoordL-1;
		}
	}
	pFrame->StartScan(TURNTABLE_SCAN);
	
}

void CTubeScanPage::OnButtonAbort() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);

	theApp.m_Thread.m_bAbortFlag = TRUE;
}

void CTubeScanPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	CCoord CpSurface,Cp;
	float	fMaxSpeed;
	static float fScanSpeed = -0.1f;
	CString Micro = L"\x0b5";

	if(m_nOldScanLine-theApp.m_Thread.m_nScanLine) {
		Buff.Format(_T("%d"),(m_nOldScanLine=theApp.m_Thread.m_nScanLine) + 1);
		m_editScanLine.SetWindowText(Buff);
	}

	theApp.m_Motors.GetSurfacePos(&CpSurface);
/*	if(m_CpSurface.Side1.fR - CpSurface.Side1.fR) {
		Buff.Format(_T("%.*f%s"),theApp.m_Axes[theApp.m_Tank.nRRight].nPrecision,CpSurface.Side1.fR,DEGREES);
		m_editPosition.SetWindowText(Buff);
	}*/
	if(m_CpSurface.Side0.fR - CpSurface.Side0.fR) {
		Buff.Format(_T("%.*f%s"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPrecision,CpSurface.Side0.fR,DEGREES);
		m_editPosition.SetWindowText(Buff);
	}
	m_CpSurface = CpSurface;

	if(theApp.m_nRealPRFPerTS > 0) {
		fMaxSpeed = (float)theApp.m_nRealPRFPerTS * PROFILE->m_fFastIncrement;
	} else {
		fMaxSpeed = (float)theApp.m_UtUser.m_Global.nPrf * PROFILE->m_fFastIncrement;
	}
	if((m_fMaxSpeed != fMaxSpeed) || (fScanSpeed - PROFILE->m_fScanSpeed)) {
		fScanSpeed = PROFILE->m_fScanSpeed;
		Buff.Format(_T("%.02f%s/s"),m_fMaxSpeed = fMaxSpeed,DEGREES);
		if(m_fMaxSpeed < PROFILE->m_fScanSpeed) {
			m_editMaxSpeed.SetTextColour(RGB(255,0,0));
		} else {
			m_editMaxSpeed.SetTextColour(RGB(0,0,0));
		}
		m_editMaxSpeed.SetWindowText(Buff);
	}


	SetToolBarCheckedState();
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CTubeScanPage::OnChangeEditScanSpeed() 
{
	CString Buff;
	float fTemp = PROFILE->m_fScanSpeed;

	m_editScanSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	if(fTemp<1.0f) fTemp = 1.0f;
	if(fTemp>1000.0f) fTemp = 1000.0f;
	if(PROFILE->m_fScanSpeed - fTemp) {
		PROFILE->m_fScanSpeed = fTemp;
//		UpdateInformation();
	}
}

void CTubeScanPage::OnChangeEditResumeLine() 
{
	CString	Buff;

	m_editResumeLine.GetWindowText(Buff);
	PROFILE->m_nScanResumeLine = _ttoi(Buff) -1;
	if(PROFILE->m_nScanResumeLine<PROFILE->m_nScanStartLine) {
		PROFILE->m_nScanResumeLine = PROFILE->m_nScanStartLine;
	}
	if(PROFILE->m_nScanResumeLine>=PROFILE->m_nScanFinishLine) {
		PROFILE->m_nScanResumeLine = PROFILE->m_nScanFinishLine;
	}
}

void CTubeScanPage::OnDeltaposSpinDiameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fComponentDiameter[0] -= ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fComponentDiameter[0] -= ((float)pNMUpDown->iDelta * 0.254f);
	}
	UpdateAllControls();
	pFrame->SendMessage(UI_UPDATE_AXES);
	
	*pResult = 0;
}

void CTubeScanPage::OnChangeEditDiameter() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString Buff;
	float fTemp;

	m_editDiameter.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;

	if(PROFILE->m_fComponentDiameter[0]-fTemp) {
		PROFILE->m_fComponentDiameter[0]=fTemp;
		theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], true, true);
		pFrame->SendMessage(UI_UPDATE_AXES);

//		UpdateSurfaceSpeed();
	};
}

void CTubeScanPage::OnButtonZero() 
{
	theApp.m_Motors.ZeroAxis(theApp.m_Tank.nRLeft);
	theApp.m_Motors.ZeroAxis(theApp.m_Tank.nRRight);
	theApp.m_Axes[31].nOffset = 0;
	theApp.m_PmacUser.HomeAxisWithoutMoving(32);
	
}

void CTubeScanPage::OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	PROFILE->m_fScanSpeed -= ((float)pNMUpDown->iDelta * 0.1f);
	if(PROFILE->m_fScanSpeed<0.0f) PROFILE->m_fScanSpeed = 0.0f;
	if(PROFILE->m_fScanSpeed>1080.0f) PROFILE->m_fScanSpeed = 1080.0f;
	UpdateAllControls();

	*pResult = 0;
}

BOOL CTubeScanPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
   case ID_PROFILES_SAVEAS: strTipText.LoadString(IDS_Save);
	   break;
   case ID_PROFILES_OPEN: strTipText.LoadString(IDS_Open);
	   break;
   case ID_BUTTON_CALIBRATE: strTipText.LoadString(IDS_Run_calibration);
	   break;
   case ID_BUTTON_CHECK_GAUGE: strTipText.LoadString(IDS_Check_gauge);
	   break;
   case ID_BUTTON_ZERO_ROTATE: strTipText.LoadString(IDS_Zero_rollers);
	   break;
   case ID_BUTTON_HELICAL_SCAN: strTipText.LoadString(IDS_Helical_scan);
	   break;
   case ID_BUTTON_RECTILINEAR_SCAN: strTipText.LoadString(IDS_Linear_step_rotate);
	   break;
   case IDC_BUTTON_START: strTipText.LoadString(IDS_Start_Scan);
	   break;
   case IDC_BUTTON_ABORT: strTipText.LoadString(IDS_Abort_scan);
	   break;
   }
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


void CTubeScanPage::OnButtonZeroRotate() 
{
	theApp.m_Motors.ZeroAxis(theApp.m_Tank.nRLeft);
	theApp.m_Motors.ZeroAxis(theApp.m_Tank.nRRight);
	theApp.m_Axes[31].nOffset = 0;
	theApp.m_PmacUser.HomeAxisWithoutMoving(32);
	
}

void CTubeScanPage::SetToolBarCheckedState()
{
	int	nIndex;

	if(m_nOldScanType - m_nScanType) {
		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_HELICAL_SCAN))>=0)
			m_nScanType==TURNTABLE_SCAN ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_RECTILINEAR_SCAN))>=0)
			m_nScanType==CONTOUR_SCAN ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	}

	if(m_nOldThreadAction - theApp.m_Thread.m_nThreadAction) {
		switch(m_nOldThreadAction = theApp.m_Thread.m_nThreadAction) {
		case CONTOUR_SCAN:
		case TURNTABLE_SCAN:
			m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(IDC_BUTTON_START),TBBS_DISABLED );
			m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(IDC_BUTTON_ABORT),0 );
			break;
		default:
			m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(IDC_BUTTON_START),0 );
			m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(IDC_BUTTON_ABORT),TBBS_DISABLED );
			break;
		}
	}

}

void CTubeScanPage::OnButtonHelicalScan() 
{
	theApp.m_LastSettings.nScanType = m_nScanType = TURNTABLE_SCAN;
	UpdateAllControls();
}

void CTubeScanPage::OnButtonRectilinearScan() 
{
	theApp.m_LastSettings.nScanType = m_nScanType = CONTOUR_SCAN;
	UpdateAllControls();
}

void CTubeScanPage::OnChangeEditFilepath() 
{
	m_editFilePath.GetWindowText(theApp.m_Data.m_FileName);
	theApp.m_Data.m_FilePath = theApp.m_StorePath[0] + L"\\" + theApp.m_Data.m_FileName + L".dat";
}


void CTubeScanPage::OnButtonBrowse() 
{
	int nPos;

	CFileDialog File(TRUE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data (*.dat)|*.dat|All Files (*.*)|*.*||"),NULL);
	File.m_ofn.lpstrInitialDir = theApp.m_StorePath[0];
	if(File.DoModal()==IDOK) {
		theApp.m_Data.m_FilePath.Format(_T("%s"),File.GetPathName());
		theApp.m_Data.m_FileName.Format(_T("%s"),File.GetFileTitle());

		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = theApp.m_Data.m_FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = theApp.m_Data.m_FilePath.Left(nPos);
			}
		}

		m_editFilePath.SetWindowText(File.GetFileTitle());
	};	
}

void CTubeScanPage::OnTubesSortX() 
{
	PROFILE->SortEachLine(theApp.m_nEditLineType,0);
	FillList();
	GenerateScanLines();
	
}

void CTubeScanPage::OnTubesSortY() 
{
	PROFILE->SortEachLine(theApp.m_nEditLineType,1);
	FillList();
	GenerateScanLines();
}

void CTubeScanPage::OnTubesSortZ() 
{
	PROFILE->SortEachLine(theApp.m_nEditLineType,2);
	FillList();
	GenerateScanLines();
}
