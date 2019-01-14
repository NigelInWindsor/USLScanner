// TabsPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "TabsPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabsPage property page

IMPLEMENT_DYNCREATE(CTabsPage, CPropertyPage)

CTabsPage::CTabsPage() : CPropertyPage(CTabsPage::IDD)
{
	//{{AFX_DATA_INIT(CTabsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTabType = 0;
}

CTabsPage::~CTabsPage()
{
}

void CTabsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabsPage)
	DDX_Control(pDX, IDC_COMBO_TABS, m_comboTabs);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_TABS, m_listTabs);
}


BEGIN_MESSAGE_MAP(CTabsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTabsPage)
	ON_CBN_SELCHANGE(IDC_COMBO_TABS, OnSelchangeComboTabs)
	//}}AFX_MSG_MAP
	ON_CLBN_CHKCHANGE(IDC_LIST_TABS, OnCheckChangeListTabs)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabsPage message handlers

BOOL CTabsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;
	
	Buff.LoadString(IDS_Ultrasonics);		m_comboTabs.AddString(Buff);
	Buff.LoadString(IDS_Motion);			m_comboTabs.AddString(Buff);
	Buff.LoadString(IDS_Palette);			m_comboTabs.AddString(Buff);
	Buff.LoadString(IDS_Profile);			m_comboTabs.AddString(Buff);
	Buff.LoadString(IDS_Position);			m_comboTabs.AddString(Buff);
	m_comboTabs.SetCurSel(m_nTabType);

	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabsPage::UpdateAllControls()
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_listTabs.ResetContent();
	m_listTabs.SetCheckStyle(BS_AUTOCHECKBOX);

	switch(m_nTabType) {
	case 0:
		Buff.LoadString(IDS_PR20);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_PR50);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_PR30);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Scope);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Global);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_TimeSlots);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Gates);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Self_Test);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Ext_Source);		m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Traffic_light);		m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Phased_Array);		m_listTabs.AddString(Buff);

		m_listTabs.SetCheck(0,App->m_LastSettings.bPr20Tab);
		m_listTabs.SetCheck(1,App->m_LastSettings.bPr50Tab);
		m_listTabs.SetCheck(2,App->m_LastSettings.bPr30Tab);
		m_listTabs.SetCheck(3,App->m_LastSettings.bScopeTab);
		m_listTabs.SetCheck(4,App->m_LastSettings.bGlobalTab);
		m_listTabs.SetCheck(5,App->m_LastSettings.bTimeSlotTabs);
		m_listTabs.SetCheck(6,App->m_LastSettings.bGatesTab);
		m_listTabs.SetCheck(7,App->m_LastSettings.bUltrasonicsSelfTestTab);
		m_listTabs.SetCheck(8,App->m_LastSettings.bUltrasonicsExtSourceTab);
		m_listTabs.SetCheck(9,App->m_LastSettings.bUltrasonicsTrafficLightTab);
		m_listTabs.SetCheck(10,App->m_LastSettings.bLinearScanningArrayTab);
		break;
	case 1:
		Buff.LoadString(IDS_Rectilinear);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Turntable);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Rollers);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Contour);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Tubes);						m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Disc);						m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Tip_Axes);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Optimize);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Pump);						m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Manipulator);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Details);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Origin);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Move_To);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Lift);						m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Operator_Contour_Scan);		m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Scan_Lines);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_RR_WIP);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Rotate_Theta_Lift);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Holding_Fixture);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_External_Trig_Scan);		m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Material_Velocity);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Jog);						m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Flat_Scan);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Roller_Scan);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Blade_Scan);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Vertical_Bore_scanner);		m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Roller_probe);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Arm_positions);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Trans_Ply);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Amplitude_palette);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Thickness_palette);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Robot);						m_listTabs.AddString(Buff);
		m_listTabs.SetCheck(0,App->m_LastSettings.bRectilinearScanTab);
		m_listTabs.SetCheck(1,App->m_LastSettings.bTurnTableScanTab);
		m_listTabs.SetCheck(2,App->m_LastSettings.bRollerScanTab);
		m_listTabs.SetCheck(3,App->m_LastSettings.bContourScanTab);
		m_listTabs.SetCheck(4,App->m_LastSettings.bTubesScanTab);
		m_listTabs.SetCheck(5,App->m_LastSettings.bDiscScanTab);
		m_listTabs.SetCheck(6,App->m_LastSettings.bMotionToolsTipAxesTab);
		m_listTabs.SetCheck(7,App->m_LastSettings.bNormalizeTab);
		m_listTabs.SetCheck(8,App->m_LastSettings.bPumpTab);
		m_listTabs.SetCheck(9,App->m_LastSettings.bManipulatorTab);
		m_listTabs.SetCheck(10,App->m_LastSettings.bDetailsTab);
		m_listTabs.SetCheck(11,App->m_LastSettings.bMotionToolsOriginTab);
		m_listTabs.SetCheck(12,App->m_LastSettings.bMotionToolsMoveToTab);
		m_listTabs.SetCheck(13,App->m_LastSettings.bMotionToolsLiftTab);
		m_listTabs.SetCheck(14,App->m_LastSettings.bMotionToolsOperatorContourTab);
		m_listTabs.SetCheck(15,App->m_LastSettings.bMotionToolsScanLinesTab);
		m_listTabs.SetCheck(16,App->m_LastSettings.bMotionToolsRRWIPTab);
		m_listTabs.SetCheck(17,App->m_LastSettings.bMotionToolsRotateThetaLiftTab);
		m_listTabs.SetCheck(18,App->m_LastSettings.bHoldingFixtureTab);
		m_listTabs.SetCheck(19,App->m_LastSettings.bExtTrigScanTab);
		m_listTabs.SetCheck(20,App->m_LastSettings.bMotionMaterialVelocityTab);
		m_listTabs.SetCheck(21,App->m_LastSettings.bMotionJogTab);
		m_listTabs.SetCheck(22,App->m_LastSettings.bMotionFlatScanTab);
		m_listTabs.SetCheck(23,App->m_LastSettings.bMotionRollerScanTab);
		m_listTabs.SetCheck(24,App->m_LastSettings.bMotionBladeScanTab);
		m_listTabs.SetCheck(25,App->m_LastSettings.bMotionVerticalBoreScanTab);
		m_listTabs.SetCheck(26,App->m_LastSettings.bMotionRollerProbeTab);
		m_listTabs.SetCheck(27,App->m_LastSettings.bMotionArmPositionsTab);
		m_listTabs.SetCheck(28,App->m_LastSettings.bMotionTransPlyTab);
		m_listTabs.SetCheck(29,App->m_LastSettings.bMotionAmplitudeTab);
		m_listTabs.SetCheck(30,App->m_LastSettings.bMotionThicknessTab);
		m_listTabs.SetCheck(31,App->m_LastSettings.bMotionRobotTab);
		break;
	case 2:
		Buff.LoadString(IDS_Amplitude);		m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Thickness);		m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Porosity);		m_listTabs.AddString(Buff);
		m_listTabs.SetCheck(0,App->m_LastSettings.bAmplitudeTab);
		m_listTabs.SetCheck(1,App->m_LastSettings.bThicknessTab);
		m_listTabs.SetCheck(2,App->m_LastSettings.bPorosityTab);
		break;
	case 3:
		Buff.LoadString(IDS_Cartesian);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Turntable);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Laser_Topography);	m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Design);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_3D_View);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Stiffeners);		m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Blade);		m_listTabs.AddString(Buff);
		m_listTabs.SetCheck(0,App->m_LastSettings.bProfileCartesianTab);
		m_listTabs.SetCheck(1,App->m_LastSettings.bProfileTurTableTab);
		m_listTabs.SetCheck(2,App->m_LastSettings.bProfileLaserTopographyTab);
		m_listTabs.SetCheck(3,App->m_LastSettings.bProfile3DDesignTab);
		m_listTabs.SetCheck(4,App->m_LastSettings.bProfile3DViewTab);
		m_listTabs.SetCheck(5,App->m_LastSettings.bProfile3DStiffenerTab);
		m_listTabs.SetCheck(6,App->m_LastSettings.bProfile3DBladeTab);
		
		break;
	case 4:
		Buff.LoadString(IDS_Dual_Sided);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Single_Bridge);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Single_Bridge_Dual_Sided);	m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Holding_Fixture);			m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Dual_Robot);				m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Graphic);					m_listTabs.AddString(Buff);
		Buff.LoadString(IDS_Swivel_Gimbal_Rotate);		m_listTabs.AddString(Buff);
		App->m_LastSettings.nPositionTabMask&0x01 ? m_listTabs.SetCheck(0,true) : m_listTabs.SetCheck(0,false);
		App->m_LastSettings.nPositionTabMask&0x02 ? m_listTabs.SetCheck(1,true) : m_listTabs.SetCheck(1,false);
		App->m_LastSettings.nPositionTabMask&0x04 ? m_listTabs.SetCheck(2,true) : m_listTabs.SetCheck(2,false);
		App->m_LastSettings.nPositionTabMask&0x08 ? m_listTabs.SetCheck(3,true) : m_listTabs.SetCheck(3,false);
		App->m_LastSettings.nPositionTabMask&0x10 ? m_listTabs.SetCheck(4,true) : m_listTabs.SetCheck(4,false);
		App->m_LastSettings.nPositionTabMask&0x20 ? m_listTabs.SetCheck(5,true) : m_listTabs.SetCheck(5,false);
		App->m_LastSettings.nPositionTabMask&0x40 ? m_listTabs.SetCheck(6,true) : m_listTabs.SetCheck(6,false);
		break;
	}
}

void CTabsPage::OnSelchangeComboTabs() 
{
	m_nTabType = m_comboTabs.GetCurSel();
	UpdateAllControls();
	
}

void CTabsPage::OnCheckChangeListTabs()
{
	APP App = (APP) AfxGetApp();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	switch(m_nTabType) {
	case 0:
		App->m_LastSettings.bPr20Tab = m_listTabs.GetCheck(0) & 0x01;
		App->m_LastSettings.bPr50Tab = m_listTabs.GetCheck(1) & 0x01;
		App->m_LastSettings.bPr30Tab = m_listTabs.GetCheck(2) & 0x01;
		App->m_LastSettings.bScopeTab = m_listTabs.GetCheck(3) & 0x01;
		App->m_LastSettings.bGlobalTab = m_listTabs.GetCheck(4) & 0x01;
		App->m_LastSettings.bTimeSlotTabs = m_listTabs.GetCheck(5) & 0x01;
		App->m_LastSettings.bGatesTab = m_listTabs.GetCheck(6) & 0x01;
		App->m_LastSettings.bUltrasonicsSelfTestTab = m_listTabs.GetCheck(7) & 0x01;
		App->m_LastSettings.bUltrasonicsExtSourceTab = m_listTabs.GetCheck(8) & 0x01;
		App->m_LastSettings.bUltrasonicsTrafficLightTab = m_listTabs.GetCheck(9) & 0x01;
		App->m_LastSettings.bLinearScanningArrayTab = m_listTabs.GetCheck(10) & 0x01;
		break;
	case 1:
		App->m_LastSettings.bRectilinearScanTab = m_listTabs.GetCheck(0) & 0x01;
		App->m_LastSettings.bTurnTableScanTab = m_listTabs.GetCheck(1) & 0x01;
		App->m_LastSettings.bRollerScanTab = m_listTabs.GetCheck(2) & 0x01;
		App->m_LastSettings.bContourScanTab = m_listTabs.GetCheck(3) & 0x01;
		App->m_LastSettings.bTubesScanTab = m_listTabs.GetCheck(4) & 0x01;
		App->m_LastSettings.bDiscScanTab = m_listTabs.GetCheck(5) & 0x01;
		App->m_LastSettings.bMotionToolsTipAxesTab = m_listTabs.GetCheck(6) & 0x01;
		App->m_LastSettings.bNormalizeTab = m_listTabs.GetCheck(7) & 0x01;
		App->m_LastSettings.bPumpTab = m_listTabs.GetCheck(8) & 0x01;
		App->m_LastSettings.bManipulatorTab = m_listTabs.GetCheck(9) & 0x01;
		App->m_LastSettings.bDetailsTab = m_listTabs.GetCheck(10) & 0x01;
		App->m_LastSettings.bMotionToolsOriginTab = m_listTabs.GetCheck(11) & 0x01;
		App->m_LastSettings.bMotionToolsMoveToTab = m_listTabs.GetCheck(12) & 0x01;
		App->m_LastSettings.bMotionToolsLiftTab = m_listTabs.GetCheck(13) & 0x01;
		App->m_LastSettings.bMotionToolsOperatorContourTab = m_listTabs.GetCheck(14) & 0x01;
		App->m_LastSettings.bMotionToolsScanLinesTab = m_listTabs.GetCheck(15) & 0x01;
		App->m_LastSettings.bMotionToolsRRWIPTab = m_listTabs.GetCheck(16) & 0x01;
		App->m_LastSettings.bMotionToolsRotateThetaLiftTab = m_listTabs.GetCheck(17) & 0x01;
		App->m_LastSettings.bHoldingFixtureTab = m_listTabs.GetCheck(18) & 0x01;
		App->m_LastSettings.bExtTrigScanTab = m_listTabs.GetCheck(19) & 0x01;
		App->m_LastSettings.bMotionMaterialVelocityTab = m_listTabs.GetCheck(20) & 0x01;
		App->m_LastSettings.bMotionJogTab = m_listTabs.GetCheck(21) & 0x01;
		App->m_LastSettings.bMotionFlatScanTab = m_listTabs.GetCheck(22) & 0x01;
		App->m_LastSettings.bMotionRollerScanTab = m_listTabs.GetCheck(23) & 0x01;
		App->m_LastSettings.bMotionBladeScanTab = m_listTabs.GetCheck(24) & 0x01;
		App->m_LastSettings.bMotionVerticalBoreScanTab = m_listTabs.GetCheck(25) & 0x01;
		App->m_LastSettings.bMotionRollerProbeTab = m_listTabs.GetCheck(26) & 0x01;
		App->m_LastSettings.bMotionArmPositionsTab = m_listTabs.GetCheck(27) & 0x01;
		App->m_LastSettings.bMotionTransPlyTab = m_listTabs.GetCheck(28) & 0x01;
		App->m_LastSettings.bMotionAmplitudeTab = m_listTabs.GetCheck(29) & 0x01;
		App->m_LastSettings.bMotionThicknessTab = m_listTabs.GetCheck(30) & 0x01;
		App->m_LastSettings.bMotionRobotTab = m_listTabs.GetCheck(31) & 0x01;
		pFrame->SendMessage(UI_UPDATE_MOTION_TOOLS);
		break;
	case 2:
		App->m_LastSettings.bAmplitudeTab = m_listTabs.GetCheck(0) & 0x01;
		App->m_LastSettings.bThicknessTab = m_listTabs.GetCheck(1) & 0x01;
		App->m_LastSettings.bPorosityTab = m_listTabs.GetCheck(2) & 0x01;
		break;
	case 3:
		App->m_LastSettings.bProfileCartesianTab = m_listTabs.GetCheck(0) & 0x01;
		App->m_LastSettings.bProfileTurTableTab = m_listTabs.GetCheck(1) & 0x01;
		App->m_LastSettings.bProfileLaserTopographyTab = m_listTabs.GetCheck(2) & 0x01;
		App->m_LastSettings.bProfile3DDesignTab = m_listTabs.GetCheck(3) & 0x01;
		App->m_LastSettings.bProfile3DViewTab = m_listTabs.GetCheck(4) & 0x01;
		App->m_LastSettings.bProfile3DStiffenerTab = m_listTabs.GetCheck(5) & 0x01;
		App->m_LastSettings.bProfile3DBladeTab = m_listTabs.GetCheck(6) & 0x01;
		break;
	case 4:
		App->m_LastSettings.nPositionTabMask=0;
		if(m_listTabs.GetCheck(0)) App->m_LastSettings.nPositionTabMask |= 0x01;
		if(m_listTabs.GetCheck(1)) App->m_LastSettings.nPositionTabMask |= 0x02;
		if(m_listTabs.GetCheck(2)) App->m_LastSettings.nPositionTabMask |= 0x04;
		if(m_listTabs.GetCheck(3)) App->m_LastSettings.nPositionTabMask |= 0x08;
		if(m_listTabs.GetCheck(4)) App->m_LastSettings.nPositionTabMask |= 0x10;
		if(m_listTabs.GetCheck(5)) App->m_LastSettings.nPositionTabMask |= 0x20;
		if(m_listTabs.GetCheck(6)) App->m_LastSettings.nPositionTabMask |= 0x40;
		break;
	}
}
