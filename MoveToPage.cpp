// MoveToPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "USLPropertyPage.h"
#include "MoveToPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LEFT	0
#define	RIGHT	1
#define	BOTH	2


/////////////////////////////////////////////////////////////////////////////
// CMoveToPage property page

IMPLEMENT_DYNCREATE(CMoveToPage, CPropertyPage)

CMoveToPage::CMoveToPage() : CPropertyPage(CMoveToPage::IDD)
{
	//{{AFX_DATA_INIT(CMoveToPage)
	//}}AFX_DATA_INIT
	m_nSide = BOTH;
	
	m_nIndex = 0;
}

CMoveToPage::~CMoveToPage()
{
}

void CMoveToPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveToPage)
	DDX_Control(pDX, IDC_SPIN_MATERIAL_VELOCITY, m_spinMaterialVelocity);
	DDX_Control(pDX, IDC_COMBO_MATERIAL_VELOCITY, m_comboMaterialVelocity);
	DDX_Control(pDX, IDC_SPIN_W_TEMP_VEL, m_spinWaterTempVelocity);
	DDX_Control(pDX, IDC_EDIT_W_TEMP_VEL, m_editWaterTempVelocity);
	DDX_Control(pDX, IDC_EDIT_THICKNESS, m_editThickness);
	DDX_Control(pDX, IDC_EDIT_MATERIAL_VELOCITY, m_editMaterialVelocity);
	DDX_Control(pDX, IDC_COMBO_TS, m_comboTimeSlot);
	DDX_Control(pDX, IDC_COMBO_SIDE, m_comboSide);
	DDX_Control(pDX, IDC_EDIT_DISTANCE, m_editDistance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveToPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMoveToPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_CHECK_BOTH, OnCheckBoth)
	ON_BN_CLICKED(IDC_CHECK_LEFT, OnCheckLeft)
	ON_BN_CLICKED(IDC_CHECK_RIGHT, OnCheckRight)
	ON_BN_CLICKED(IDC_BUTTON_STORE, OnButtonStore)
	ON_EN_CHANGE(IDC_EDIT_LEFT_POS, OnChangeEditLeftPos)
	ON_EN_CHANGE(IDC_EDIT_RIGHT_POS, OnChangeEditRightPos)
	ON_BN_CLICKED(IDC_BUTTON_WP_APPLY, OnButtonWpApply)
	ON_CBN_SELCHANGE(IDC_COMBO_SIDE, OnSelchangeComboSide)
	ON_CBN_SELCHANGE(IDC_COMBO_TS, OnSelchangeComboTs)
	ON_EN_CHANGE(IDC_EDIT_DISTANCE, OnChangeEditDistance)
	ON_BN_CLICKED(IDC_BUTTON_MATERIAL_VELOCITY, OnButtonMaterialVelocity)
	ON_EN_CHANGE(IDC_EDIT_MATERIAL_VELOCITY, OnChangeEditMaterialVelocity)
	ON_EN_CHANGE(IDC_EDIT_THICKNESS, OnChangeEditThickness)
	ON_LBN_SELCHANGE(IDC_LIST_MOVETO, OnSelchangeListMoveto)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_W_TEMP_VEL, OnDeltaposSpinWTempVel)
	ON_WM_CREATE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MATERIAL_VELOCITY, OnDeltaposSpinMaterialVelocity)
	ON_EN_CHANGE(IDC_EDIT_W_TEMP_VEL, OnChangeEditWTempVel)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoveToPage message handlers
int CMoveToPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndMoveToToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndMoveToToolBar.LoadToolBar(IDR_MOVETO_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndTipsToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndTipsToolBar.LoadToolBar(IDR_TIPS_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width(),30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndMoveToToolBar);
	m_wndRebar.AddBar(&m_wndTipsToolBar);
	
	return 0;
}


BOOL CMoveToPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_spinWaterTempVelocity.SetRange(0,40);
	m_spinMaterialVelocity.SetRange(1,12000);

	switch(theApp.m_nPrimarySide) {
	case 0:
		Buff.LoadString(IDS_Left);	m_comboSide.AddString(Buff);
		Buff.LoadString(IDS_Right);	m_comboSide.AddString(Buff);
		break;
	case 1:
		Buff.LoadString(IDS_Right);	m_comboSide.AddString(Buff);
		Buff.LoadString(IDS_Left);	m_comboSide.AddString(Buff);
		break;
	}


	m_comboMaterialVelocity.AddString(L"Acrylic (Perspex) 2730");
	m_comboMaterialVelocity.AddString(L"Aluminum 6320");
	m_comboMaterialVelocity.AddString(L"Beryllium 12900");
	m_comboMaterialVelocity.AddString(L"Brass 4430");
	m_comboMaterialVelocity.AddString(L"Composite graphite 3070");
	m_comboMaterialVelocity.AddString(L"Copper 4660");
	m_comboMaterialVelocity.AddString(L"Diamond 18000");
	m_comboMaterialVelocity.AddString(L"Fiberglass 2740");
	m_comboMaterialVelocity.AddString(L"Glycerin 1920");
	m_comboMaterialVelocity.AddString(L"Inconel 5820");
	m_comboMaterialVelocity.AddString(L"Iron, Cast (soft) 3500");
	m_comboMaterialVelocity.AddString(L"Iron, Cast (hard) 5600");
	m_comboMaterialVelocity.AddString(L"Iron oxide (magnetite)  5890");
	m_comboMaterialVelocity.AddString(L"Lead 2160");
	m_comboMaterialVelocity.AddString(L"Molybdenum 6250");
	m_comboMaterialVelocity.AddString(L"Motor oil  1740");
	m_comboMaterialVelocity.AddString(L"Nickel 5630");
	m_comboMaterialVelocity.AddString(L"Polyamide 2200");
	m_comboMaterialVelocity.AddString(L"Nylon 2600");
	m_comboMaterialVelocity.AddString(L"Polyethylene high density (HDPE) 2460");
	m_comboMaterialVelocity.AddString(L"Polyethylene low density (LDPE) 2080");
	m_comboMaterialVelocity.AddString(L"Polystyrene 2340");
	m_comboMaterialVelocity.AddString(L"Polyvinylchloride, (PVC) 2395");
	m_comboMaterialVelocity.AddString(L"Rubber 1610");
	m_comboMaterialVelocity.AddString(L"Silicon 9620");
	m_comboMaterialVelocity.AddString(L"Silicone 1485");
	m_comboMaterialVelocity.AddString(L"Steel, 1020 5890");
	m_comboMaterialVelocity.AddString(L"Steel, 4340 5850");
	m_comboMaterialVelocity.AddString(L"Steel, 302 austenitic stainless 5740");
	m_comboMaterialVelocity.AddString(L"Tantatalum 3400");
	m_comboMaterialVelocity.AddString(L"Tin 3320");
	m_comboMaterialVelocity.AddString(L"Titanium 6140");
	m_comboMaterialVelocity.AddString(L"Tungsten 5180");
	m_comboMaterialVelocity.AddString(L"Zinc 4170");
	m_comboMaterialVelocity.AddString(L"Zirconium 4650");
	m_comboMaterialVelocity.SetCurSel(0);

	
	UpdateAllControls();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CMoveToPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CMoveToPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_comboTimeSlot.ResetContent();
	for(int nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		m_comboTimeSlot.AddString(theApp.m_UtUser.m_TS[nTS].cName);
	}
	m_comboTimeSlot.SetCurSel(theApp.m_LastSettings.nWaterVelocityTS);
	m_comboSide.SetCurSel(theApp.m_LastSettings.nWaterVelocitySide);

	Buff.Format(_T("%.1f mm"),theApp.m_LastSettings.fWaterVelocityDistance);
	m_editDistance.SetWindowText(Buff);

	Buff.Format(_T("%d m/s"),theApp.m_LastSettings.nMaterialVelocity);
	m_editMaterialVelocity.SetWindowText(Buff);

	Buff.Format(_T("%.02fmm"),theApp.m_LastSettings.fMaterialVelocityThickness);
	m_editThickness.SetWindowText(Buff);

	Buff.Format(L"%d m/s  %d%s",theApp.m_LastSettings.nWaterVelocity,theApp.m_LastSettings.nWaterTemperature,DEGREES);
	m_editWaterTempVelocity.SetWindowText(Buff);

	m_spinMaterialVelocity.SetPos(theApp.m_LastSettings.nMaterialVelocity);

	SetAccessPrivelage();

}



void CMoveToPage::OnCheckBoth() 
{
	m_nSide = BOTH;
	UpdateAllControls();
}

void CMoveToPage::OnCheckLeft() 
{
	theApp.m_nSide0Orientation==0 ? m_nSide = LEFT : m_nSide = RIGHT;
	UpdateAllControls();
}

void CMoveToPage::OnCheckRight() 
{
	theApp.m_nSide0Orientation==0 ? m_nSide = RIGHT : m_nSide = LEFT;
	UpdateAllControls();
}


void CMoveToPage::OnButtonApply() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		ApplyNonRobot();
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
		ApplyRobot();
		break;
	}
}




void CMoveToPage::ApplyNonRobot() 
{
	CString Buff;
	CCoord Cp,CpHead,CpSurface;

	theApp.m_Thread.m_nLadderExecuteSide=0;
	if(m_checkLeft.GetCheck() == TRUE) {
		theApp.m_nSide0Orientation==0 ? m_nSide = LEFT : m_nSide = RIGHT;
		theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nLadderExecuteSide |= 1 : theApp.m_Thread.m_nLadderExecuteSide |= 2;
	} else {
		if(m_checkRight.GetCheck() == TRUE) {
			theApp.m_nSide0Orientation==0 ? m_nSide = RIGHT : m_nSide = LEFT;
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nLadderExecuteSide |= 2 : theApp.m_Thread.m_nLadderExecuteSide |= 1;
		} else {
			if(m_checkBoth.GetCheck() == TRUE) {
				m_nSide = BOTH;
				theApp.m_Thread.m_nLadderExecuteSide |= 3;
			}
		}
	}


	m_nIndex = m_listMoveTo.GetCurSel();
	m_listMoveTo.GetText(m_nIndex,Buff);

	if(m_nIndex == 0) { // Safe
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nLadder = 0;
		theApp.m_Thread.m_nThreadAction=EXECUTE_LADDER;
	}
	
	if(m_nIndex == 1) { // All Zero
		theApp.m_Motors.GetHeadPos(&Cp);
		switch(m_nSide) {
		case LEFT:
			Cp.Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
			Cp.Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
			Cp.Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
			Cp.Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize * -1.0f;
			Cp.Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize * -1.0f;
			break;
		case RIGHT:
			Cp.Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize * -1.0f;
			Cp.Side1.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
			Cp.Side1.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nOffset * theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize * -1.0f;
			Cp.Side1.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize * -1.0f;
			Cp.Side1.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize * -1.0f;
			break;
		case BOTH:
			Cp.Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
			Cp.Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
			Cp.Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
			Cp.Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize * -1.0f;
			Cp.Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize * -1.0f;
			Cp.Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize * -1.0f;
			Cp.Side1.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
			Cp.Side1.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nOffset * theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize * -1.0f;
			Cp.Side1.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize * -1.0f;
			Cp.Side1.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize * -1.0f;
			break;
		}
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_CpMoveTo = Cp;
		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	}
	if(m_nIndex == 2) {  //Surface Zero
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		switch(m_nSide) {
		case LEFT:
			CpSurface.Side[PORTSIDE].fYt = CpSurface.Side[PORTSIDE].fXt =  CpSurface.Side[PORTSIDE].fZ = CpSurface.Side[PORTSIDE].fY = CpSurface.Side[PORTSIDE].fX = 0.0f;
			break;
		case RIGHT:
			CpSurface.Side[STARBOARD].fYt = CpSurface.Side[STARBOARD].fXt = CpSurface.Side[STARBOARD].fZ = CpSurface.Side[STARBOARD].fY = CpSurface.Side[STARBOARD].fX = 0.0f;
			break;
		case BOTH:
			CpSurface.Side[PORTSIDE].fYt = CpSurface.Side[PORTSIDE].fXt =  CpSurface.Side[PORTSIDE].fZ = CpSurface.Side[PORTSIDE].fY = CpSurface.Side[PORTSIDE].fX = 0.0f;
			CpSurface.Side[STARBOARD].fYt = CpSurface.Side[STARBOARD].fXt = CpSurface.Side[STARBOARD].fZ = CpSurface.Side[STARBOARD].fY = CpSurface.Side[STARBOARD].fX = 0.0f;
			break;
		}
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&theApp.m_Thread.m_CpMoveTo,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&theApp.m_Thread.m_CpMoveTo,STARBOARD,TRUE,TRUE);
		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	}
	if(m_nIndex == 3) { //Stored Pos
		theApp.m_Motors.GetHeadPos(&Cp);
		switch(m_nSide) {
		case LEFT:
			Cp.SetSide0(theApp.m_CpRandomPosition);
			break;
		case RIGHT:
			Cp.SetSide1(theApp.m_CpRandomPosition);
			break;
		case BOTH:
			Cp = theApp.m_CpRandomPosition;
			break;
		}
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_CpMoveTo = Cp;
		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	}
	if(m_nIndex == 4) { //Tips to Zero
		theApp.m_Motors.GetHeadPos(&Cp);
		switch(m_nSide) {
		case LEFT:
			Cp.Side0.fXt=0.0f;
			Cp.Side0.fYt=0.0f;
			break;
		case RIGHT:
			Cp.Side1.fXt=0.0f;
			Cp.Side1.fYt=0.0f;
			break;
		case BOTH:
			Cp.Side0.fXt=0.0f;
			Cp.Side0.fYt=0.0f;
			Cp.Side1.fXt=0.0f;
			Cp.Side1.fYt=0.0f;
			break;
		}
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_CpMoveTo = Cp;
		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	}
	if(m_nIndex == 5) { //Buff == "Danger Zone"
		if(theApp.m_nJobNumber!=GOODRICH_MACHINE_4) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.GetHeadPos(&CpHead);
			theApp.m_Thread.m_CpMoveTo = CpHead;
			CCoord CpDanger = PROFILE->m_CpDanger[DANGER_MINS];
			switch(m_nSide) {
			case LEFT:
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
				theApp.m_Thread.m_CpMoveTo.Side0.fY = Cp.Side0.fY;
				break;
			case RIGHT:
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
				theApp.m_Thread.m_CpMoveTo.Side1.fY = Cp.Side1.fY;
				break;
			case BOTH:
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
				theApp.m_Thread.m_CpMoveTo.Side0.fY = Cp.Side0.fY;
				theApp.m_Thread.m_CpMoveTo.Side1.fY = Cp.Side1.fY;
				break;
			}
			theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
		} else {
			MessageBox(_T("Not yet implemented"),_T("Error"),NULL);
		}
	}
	if(m_nIndex == 6) { //Right to Find Left
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Motors.GetHeadPos(&CpHead);
		switch(theApp.m_nJobNumber) {
		default:
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
			break;
		case FOKKER:
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
			theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			break;
		}
		theApp.m_Thread.m_CpMoveTo = CpHead;
		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	}
	if(m_nIndex == 7) { //Buff == "Left to Find Right"
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Motors.GetHeadPos(&CpHead);
		switch(theApp.m_nJobNumber) {
		default:
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
			theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			break;
		case FOKKER:
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
			break;
		}
		theApp.m_Thread.m_CpMoveTo = CpHead;
		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	}
}

void CMoveToPage::ApplyRobot() 
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	switch(m_nIndex) {
	default:	MessageBox(L"Not implemented on this robot system\r\nSuggest you talk to Nigel if you definetly need it!",L"Error",MB_ICONERROR);
		break;
	case 0:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nLadderExecuteSide=3;
		theApp.m_Thread.m_nLadder = 0;
		theApp.m_Thread.m_nThreadAction=EXECUTE_LADDER;
		break;
	case 2:		// Surface zero
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		switch(m_nSide) {
		case LEFT:
			CpSurface.Side[PORTSIDE].fX = 0.0f;
			CpSurface.Side[PORTSIDE].fY = 0.0f;
			CpSurface.Side[PORTSIDE].fZ = 0.0f;
			CpSurface.Side[PORTSIDE].fI = -1.0f;
			CpSurface.Side[PORTSIDE].fJ = 0.0f;
			CpSurface.Side[PORTSIDE].fK = 0.0f;
			break;
		case RIGHT:
			CpSurface.Side[STARBOARD].fX = 0.0f;
			CpSurface.Side[STARBOARD].fY = 0.0f;
			CpSurface.Side[STARBOARD].fZ = 0.0f;
			CpSurface.Side[STARBOARD].fI = -1.0f;
			CpSurface.Side[STARBOARD].fJ = 0.0f;
			CpSurface.Side[STARBOARD].fK = 0.0f;
			break;
		case BOTH:
			CpSurface.Side[PORTSIDE].fX = 0.0f;
			CpSurface.Side[PORTSIDE].fY = 0.0f;
			CpSurface.Side[PORTSIDE].fZ = 0.0f;
			CpSurface.Side[PORTSIDE].fI = -1.0f;
			CpSurface.Side[PORTSIDE].fJ = 0.0f;
			CpSurface.Side[PORTSIDE].fK = 0.0f;
			CpSurface.Side[STARBOARD].fX = 0.0f;
			CpSurface.Side[STARBOARD].fY = 0.0f;
			CpSurface.Side[STARBOARD].fZ = 0.0f;
			CpSurface.Side[STARBOARD].fI = -1.0f;
			CpSurface.Side[STARBOARD].fJ = 0.0f;
			CpSurface.Side[STARBOARD].fK = 0.0f;
			break;
		}

		theApp.m_Thread.m_CpMoveTo = CpSurface;
		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
		break;
	}


}

void CMoveToPage::OnButtonStore() 
{
	theApp.m_Motors.GetHeadPos(&theApp.m_CpRandomPosition);
	UpdateAllControls();
}

void CMoveToPage::OnChangeEditLeftPos() 
{
	CString Buff;

	m_editLeftPos.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f Z:%f Xt:%f Yt:%f"),
		&theApp.m_CpRandomPosition.Side0.fX,&theApp.m_CpRandomPosition.Side0.fY,&theApp.m_CpRandomPosition.Side0.fZ,
		&theApp.m_CpRandomPosition.Side0.fXt,&theApp.m_CpRandomPosition.Side0.fYt);

}

void CMoveToPage::OnChangeEditRightPos() 
{
	CString Buff;

	m_editRightPos.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f Z:%f Xt:%f Yt:%f"),
		&theApp.m_CpRandomPosition.Side1.fX,&theApp.m_CpRandomPosition.Side1.fY,&theApp.m_CpRandomPosition.Side1.fZ,
		&theApp.m_CpRandomPosition.Side1.fXt,&theApp.m_CpRandomPosition.Side1.fYt);
}


BOOL CMoveToPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}

void CMoveToPage::OnButtonWpApply() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=MEASURE_WATER_VELOCITY;	
}

void CMoveToPage::OnSelchangeComboSide() 
{
	theApp.m_LastSettings.nWaterVelocitySide = m_comboSide.GetCurSel();
}

void CMoveToPage::OnSelchangeComboTs() 
{
	theApp.m_LastSettings.nWaterVelocityTS = m_comboTimeSlot.GetCurSel();
}

void CMoveToPage::OnChangeEditDistance() 
{
	CString Buff;
	m_editDistance.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fWaterVelocityDistance);
}

void CMoveToPage::OnChangeEditWaterVelocity() 
{
	CString Buff;
	m_editWaterVelocity.GetWindowText(Buff);
	theApp.m_LastSettings.nWaterVelocity = _ttoi(Buff);
}

void CMoveToPage::OnButtonMaterialVelocity() 
{
	CString Buff;

	int nTimeNs=theApp.m_UtUser.GetGateMaterialTimeNs(theApp.m_LastSettings.nWaterVelocityTS,1);
	theApp.m_LastSettings.nMaterialVelocity = (int)(theApp.m_LastSettings.fMaterialVelocityThickness / ((float)nTimeNs * 0.5e-6f));

	Buff.Format(_T("%d m/s"),theApp.m_LastSettings.nMaterialVelocity);
	m_editMaterialVelocity.SetWindowText(Buff);
	
}

void CMoveToPage::OnChangeEditMaterialVelocity() 
{
	CString Buff;

	m_editMaterialVelocity.GetWindowText(Buff);
	theApp.m_LastSettings.nMaterialVelocity = _ttoi(Buff);
}

void CMoveToPage::OnChangeEditThickness() 
{
	CString Buff;

	m_editThickness.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fMaterialVelocityThickness);
}

void CMoveToPage::SetAccessPrivelage()
{
	bool bFlagEnable;
	CButton *pButton;

	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_MOVETO_PAGE] ? bFlagEnable = TRUE : bFlagEnable = FALSE;

	m_editThickness.EnableWindow(bFlagEnable);
	m_editMaterialVelocity.EnableWindow(bFlagEnable);
	m_comboTimeSlot.EnableWindow(bFlagEnable);
	m_comboSide.EnableWindow(bFlagEnable);
	m_editDistance.EnableWindow(bFlagEnable);

	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_WP_APPLY)) pButton->EnableWindow(bFlagEnable);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_MATERIAL_VELOCITY)) pButton->EnableWindow(bFlagEnable);
}

void CMoveToPage::OnSelchangeListMoveto() 
{
	m_nIndex = m_listMoveTo.GetCurSel();	
}

BOOL CMoveToPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   int nLength;

   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case ID_BUTTON_MOVETO_MACHINE_ZERO: strTipText.LoadString(IDS_Move_to_machine_zero);
	   break;
	case ID_BUTTON_MOVETO_SURFACE_ZERO: strTipText.LoadString(IDS_Move_to_surface_zero);
		break;
	case ID_BUTTON_TIPS_REMOVE_AIR:
	case ID_BUTTON_TIPS_TO_45:
	case ID_BUTTON_TIPS_TO_ZERO:
	case ID_BUTTON_TIPS_POINT_DOWN:	strTipText.LoadString(nID);
		break;
	}

	if((nLength = strTipText.Find('\n',0)) > 0) {
		strTipText.GetBufferSetLength(nLength);
	}
	lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));

	*pResult = 0;

	return TRUE;    // message was handled
}

void CMoveToPage::OnDeltaposSpinMaterialVelocity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_LastSettings.nMaterialVelocity += pNMUpDown->iDelta;
	UpdateAllControls();
	
	*pResult = 0;
}

void CMoveToPage::OnDeltaposSpinWTempVel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	int nTemperature = theApp.m_LastSettings.nWaterTemperature;
	
	theApp.m_LastSettings.nWaterTemperature += pNMUpDown->iDelta;
	if(nTemperature - theApp.m_LastSettings.nWaterTemperature) {
		CalculateVelocityAndTemperature(0);
		UpdateAllControls();
	}

	*pResult = 0;
}


void CMoveToPage::OnChangeEditWTempVel() 
{
	CString Buff;
	int nVelocity = theApp.m_LastSettings.nWaterVelocity;

	m_editWaterTempVelocity.GetWindowText(Buff);
	swscanf_s(Buff,L"%d",&theApp.m_LastSettings.nWaterVelocity);
	if(nVelocity - theApp.m_LastSettings.nWaterVelocity) {
		CalculateVelocityAndTemperature(1);
		UpdateAllControls();
	}
}



void CMoveToPage::CalculateVelocityAndTemperature(int nCalcVelTemp)
{
	CPolyCoord Curve;
	CCoord Cp;
	float fTemp;

	Curve.Zero();
	//fX Temp fY Velocity
	Cp.Side0.fX = 0.0f;		Cp.Side0.fY = 1403.0f;	Curve.Add(Cp);
	Cp.Side0.fX = 5.0f;		Cp.Side0.fY = 1427.0f;	Curve.Add(Cp);
	Cp.Side0.fX = 10.0f;	Cp.Side0.fY = 1447.0f;	Curve.Add(Cp);
	Cp.Side0.fX = 20.0f;	Cp.Side0.fY = 1481.0f;	Curve.Add(Cp);
	Cp.Side0.fX = 30.0f;	Cp.Side0.fY = 1507.0f;	Curve.Add(Cp);
	Cp.Side0.fX = 40.0f;	Cp.Side0.fY = 1526.0f;	Curve.Add(Cp);
	Cp.Side0.fX = 50.0f;	Cp.Side0.fY = 1541.0f;	Curve.Add(Cp);

	Curve.SetStyle(1);

	switch(nCalcVelTemp) {
	case 0:
		Curve.SetPrimaryAxis(0);
		fTemp = (float)theApp.m_LastSettings.nWaterTemperature;
		Curve.CalculateCoord(fTemp,Cp);
		theApp.m_LastSettings.nWaterVelocity = (int)Cp.Side0.fY;
		break;
	case 1:
		Curve.SetPrimaryAxis(1);
		fTemp = (float)theApp.m_LastSettings.nWaterVelocity;
		Curve.CalculateCoord(fTemp,Cp);
		theApp.m_LastSettings.nWaterTemperature = (int)Cp.Side0.fX;
		break;
	}

}
