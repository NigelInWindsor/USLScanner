// PropertiesPane.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PropertiesPane.h"
#include "USLPropertyGridProperty.h"
#include "MainFrm.h"

#define	ID_BASE							1000
#define	ID_LOAD_WINDOS_ON_LAST_EXIT		ID_BASE + 1
#define	ID_HEIGHT						ID_BASE + 2
#define	ID_SIZE							ID_BASE + 3
#define	ID_COLOUR						ID_BASE + 4
#define	ID_BORDER						ID_BASE + 5
#define	ID_ANNOTATE_FONT				ID_BASE + 6
#define	ID_ANNOTATE_TRANSPARENT			ID_BASE + 7
#define	ID_ANNOTATE_BKCOLOUR			ID_BASE + 8
#define	ID_LANGUAGE						ID_BASE + 9
#define	ID_UNITS						ID_BASE + 10
#define	ID_SIDE_1						ID_BASE + 11
#define	ID_SCOPE_FONT					ID_BASE + 12
#define	ID_GATE_DELAY_UNITS				ID_BASE + 13
#define	ID_GATE_WIDTH_UNITS				ID_BASE + 14
#define	ID_TRACE_DELAY_UNITS			ID_BASE + 15
#define	ID_TRACE_WIDTH_UNITS			ID_BASE + 16
#define	ID_DAC_BLANKING_UNITS			ID_BASE + 17
#define	ID_DAC_DELAY_UNITS				ID_BASE + 18
#define	ID_TRACE_COLOUR					ID_BASE + 19
#define	ID_USE_SYSTEM_FONT				ID_BASE + 20
#define	ID_MENU_FONT					ID_BASE + 21
#define	ID_FORCE_LOGON					ID_BASE + 22
#define	ID_LOG_OFF_AFTER				ID_BASE + 23
#define ID_MACHINE_SIDE_NAME_0			ID_BASE + 24
#define ID_MACHINE_SIDE_NAME_1			ID_BASE + 25
#define ID_MACHINE_SIDE_NAME_2			ID_BASE + 26
#define	ID_POSITION_MENU_FONT			ID_BASE + 27
#define ID_POSITION_TEXT_COLOUR			ID_BASE + 28
#define ID_POSITION_BK_COLOUR			ID_BASE + 29
#define ID_POSITION_SIDE_0				ID_BASE + 30
#define ID_POSITION_SIDE_1				ID_BASE + 31
#define	ID_POSITION_DISPLAY_NORM		ID_BASE + 32
#define	ID_POSITION_DISPLAY_A			ID_BASE + 33
#define	ID_POSITION_DISPLAY_B			ID_BASE + 34
#define	ID_POSITION_DISPLAY_ROT_NORM	ID_BASE + 35
#define ID_POSITION_DISPLAY_QUAT		ID_BASE + 36
#define ID_POSITION_DISPLAY_R0			ID_BASE + 37
#define ID_POSITION_DISPLAY_R1			ID_BASE + 38
#define ID_POSITION_UNITS				ID_BASE + 39
#define ID_REMOVE_AIR_0					ID_BASE + 40
#define ID_REMOVE_AIR_1					ID_BASE + 41
#define ID_AIR_DWELL_TIME				ID_BASE + 42
#define ID_DELTA_SCAN_SPEED				ID_BASE + 43
#define ID_DELTA_SCAN_INTERVAL			ID_BASE + 44
#define ID_TOF_UNITS					ID_BASE + 45

#define	ID_SCOPE_TEXT_COLOUR			ID_BASE + 46
#define	ID_SCOPE_BK_COLOUR				ID_BASE + 47
#define	ID_SCOPE_GRATICULE_COLOUR		ID_BASE + 48
#define	ID_GATE_COLOUR					ID_BASE + 50	//There are 10 of theses
#define	ID_DISPLAY_GRATICULE			ID_BASE + 60
#define ID_LOCK_ALL_TIMEBASES_TOGETHER	ID_BASE + 61
#define ID_LOCK_ALL_GATES_TOGETHER		ID_BASE + 62
#define ID_SCOPE_DAC_COLOUR				ID_BASE + 63
#define ID_INTERFACE_MARKER				ID_BASE + 64
#define ID_AMPLITUDE_MARKER				ID_BASE + 65
#define ID_TOF_MARKER					ID_BASE + 66
#define ID_RENDER_WATER_PATH			ID_BASE + 67
#define ID_RENDER_MATERIAL_PATH			ID_BASE + 68
#define ID_RENDER_COLLECT_TEXT			ID_BASE + 69
#define ID_LEFT_SCALE					ID_BASE + 71
#define ID_RIGHT_SCALE					ID_BASE + 72
#define ID_CONVERSION_RATE				ID_BASE + 73
#define ID_SAMPLING_RATE				ID_BASE + 74
#define ID_NUMBER_SAMPLES_PER_TRACE		ID_BASE + 75
#define ID_SCOPE_RENDER_TIME_ADC		ID_BASE + 76
#define ID_SCOPE_RENDER_TIME_ASCAN		ID_BASE + 77
#define ID_SCOPE_FONT_HEIGHT			ID_BASE + 78
#define ID_MACHINE_REF_NAME_0			ID_BASE + 79
#define ID_MACHINE_REF_NAME_1			ID_BASE + 80
#define ID_MACHINE_REF_NAME_2			ID_BASE + 81
#define ID_TAUGHT_LINES_COLOUR			ID_BASE + 82
#define ID_SCAN_LINES_COLOUR			ID_BASE + 83
#define ID_BRANE_LINES_COLOUR			ID_BASE + 84
#define ID_ENVELOPE_LINES_COLOUR		ID_BASE + 85
#define ID_INSIDE_LINES_COLOUR			ID_BASE + 86
#define ID_OUTSIDE_LINES_COLOUR			ID_BASE + 87
#define ID_THICKNESS_CALIBRATION_PERIOD	ID_BASE + 88

#define ID_DANGER_PLANE_DIR_0			ID_BASE + 100
#define ID_DANGER_PLANE_DIR_1			ID_BASE + 101
#define	ID_DANGER_PLANE_MARGIN_0		ID_BASE + 102
#define	ID_DANGER_PLANE_MARGIN_1		ID_BASE + 103
#define ID_PROFILE_NAME					ID_BASE + 104
#define ID_PROFILE_DESCRIPTION			ID_BASE + 105
#define	ID_PROFILE_FAST_INCREMENT		ID_BASE + 106
#define	ID_PROFILE_SLOW_INCREMENT		ID_BASE + 107
#define ID_PROFILE_ULTRASONICS			ID_BASE + 108

#define ID_MACHINE_TYPE					ID_BASE + 200
#define ID_MACHINE_SIDE0_PRESENT		ID_BASE + 201
#define ID_MACHINE_SIDE1_PRESENT		ID_BASE + 202
#define ID_MACHINE_TURNTABLE_PRESENT	ID_BASE + 203
#define ID_MACHINE_ROLLER_PRESENT		ID_BASE + 204
#define ID_MACHINE_HOLDING_PRESENT		ID_BASE + 205
#define ID_MACHINE_SQUIRTER_PRESENT		ID_BASE + 206
#define ID_MACHINE_TANK_PRESENT			ID_BASE + 207

#define ID_MACHINE_X_LENGTH				ID_BASE + 231
#define ID_MACHINE_Y_WIDTH				ID_BASE + 232
#define ID_MACHINE_Z_HEIGHT				ID_BASE + 233
#define ID_MACHINE_ZERO_X_OFFSET		ID_BASE + 234
#define ID_MACHINE_ZERO_Y_OFFSET		ID_BASE + 235
#define ID_MACHINE_ZERO_Z_OFFSET		ID_BASE + 236
#define ID_MACHINE_MOTION_CONTROLLER	ID_BASE + 237
#define	ID_TURNTABLE_CENTER				ID_BASE + 238
#define	ID_TURNTABLE_NORMAL				ID_BASE + 239
#define ID_TURNTABLE_ZERO_DEGREES		ID_BASE + 240
#define ID_JOG_SPEED_X_0				ID_BASE + 241
#define ID_JOG_SPEED_Y_0				ID_BASE + 242
#define ID_JOG_SPEED_Z_0				ID_BASE + 243
#define ID_JOG_SPEED_H_0				ID_BASE + 244
#define ID_JOG_SPEED_V_0				ID_BASE + 245
#define ID_JOG_SPEED_T_0				ID_BASE + 246
#define ID_JOG_SPEED_R_0				ID_BASE + 247
#define ID_JOG_SPEED_X_1				ID_BASE + 248
#define ID_JOG_SPEED_Y_1				ID_BASE + 249
#define ID_JOG_SPEED_Z_1				ID_BASE + 250
#define ID_JOG_SPEED_H_1				ID_BASE + 251
#define ID_JOG_SPEED_V_1				ID_BASE + 252
#define ID_JOG_SPEED_T_1				ID_BASE + 253
#define ID_JOG_SPEED_R_1				ID_BASE + 254
#define ID_MOVE_SPEED					ID_BASE + 255
#define ID_ORTHOGONAL_SPEED				ID_BASE + 256


#define ID_PR20_TAB					ID_BASE + 300
#define ID_PR50_TAB					ID_BASE + 301
#define ID_PR30_TAB					ID_BASE + 302
#define ID_PR40_TAB					ID_BASE + 303
#define ID_SCOPE_TAB				ID_BASE + 304
#define ID_GLOBAL_TAB				ID_BASE + 305
#define ID_TIMESLOTS_TAB			ID_BASE + 306
#define ID_GATES_TAB				ID_BASE + 307
#define ID_SELF_TEST_TAB			ID_BASE + 308
#define ID_EXT_SOURCE_TAB			ID_BASE + 309
#define ID_TRAFFIC_LIGHTS_TAB		ID_BASE + 310
#define ID_PHASED_ARRAY_TAB			ID_BASE + 311

#define ID_Rectilinear_TAB			ID_BASE + 320
#define ID_Turntable_TAB			ID_BASE + 321
#define ID_Rollers_TAB				ID_BASE + 322
#define ID_Contour_TAB				ID_BASE + 323
#define ID_Tubes_TAB				ID_BASE + 324
#define ID_Disc_TAB					ID_BASE + 325
#define ID_Tip_Axes_TAB				ID_BASE + 326
#define ID_Optimize_TAB				ID_BASE + 327
#define ID_Pump_TAB					ID_BASE + 328
#define ID_Manipulator_TAB			ID_BASE + 329
#define ID_Details_TAB				ID_BASE + 330
#define ID_Origin_TAB				ID_BASE + 331
#define ID_Move_To_TAB				ID_BASE + 332
#define ID_Lift_TAB					ID_BASE + 333
#define ID_Operator_Contour_Scan_TA		ID_BASE + 334
#define ID_Scan_Lines_TAB			ID_BASE + 335
#define ID_RR_WIP_TAB				ID_BASE + 336
#define ID_Rotate_Theta_Lift_TAB	ID_BASE + 337
#define ID_Holding_Fixture_TAB		ID_BASE + 338
#define ID_External_Trig_Scan_TAB	ID_BASE + 339
#define ID_Material_Velocity_TA		ID_BASE + 340
#define ID_Jog_TAB					ID_BASE + 341
#define ID_Flat_Scan_TAB			ID_BASE + 342
#define ID_Roller_Scan_TAB			ID_BASE + 343
#define ID_Blade_Scan_TAB			ID_BASE + 344
#define ID_Vertical_Bore_scanner_TAB	ID_BASE + 345
#define ID_Roller_probe_TAB	ID_BASE		ID_BASE + 346
#define ID_Arm_positions_TAB		ID_BASE + 347
#define	ID_TRANSPLY_TAB				ID_BASE + 348
#define	ID_PHASEDARRAY_LINE_SCAN_TAB ID_BASE + 349
#define ID_DISCS_SHAFTS_TAB			ID_BASE + 350

#define ID_Amplitude_palette_TAB	ID_BASE + 370
#define ID_Thickness_palette_TAB	ID_BASE + 371
#define ID_Robot_TAB				ID_BASE + 372
#define ID_Robot_Position_TAB		ID_BASE + 373
#define ID_TOF_CAL_TAB				ID_BASE + 374
#define ID_Amplitude_Palette_TAB	ID_BASE + 375
#define ID_Thickness_Palette_TAB	ID_BASE + 376
#define ID_Porosity_Palette_TAB		ID_BASE + 377
#define ID_ProfileCartesianTab		ID_BASE + 378
#define ID_ProfileTurTableTab		ID_BASE + 379
#define ID_Profile3DDesignTab		ID_BASE + 380
#define ID_Profile3DViewTab			ID_BASE + 381
#define ID_Profile3DStiffenerTab	ID_BASE + 382
#define ID_Profile3DBladeTab		ID_BASE + 383
#define ID_PositionPositionTab		ID_BASE + 384
#define ID_PositionSingleBridgeTab	ID_BASE + 385
#define ID_PositionHoldingTab		ID_BASE + 386
#define ID_PositionRobotDualTab		ID_BASE + 387
#define ID_PositionGraphicalTab		ID_BASE + 388
#define ID_PositionSwivGimTab		ID_BASE + 389

#define	ID_CSCAN_THICKNESS_ERROR	ID_BASE + 400

#define ID_PPMAC_ADDRESS			ID_BASE + 500
#define ID_PPMAC_USER_NAME			ID_BASE + 501
#define ID_PPMAC_PASSWORD			ID_BASE + 502
#define ID_PPMAC_PROTOCOL			ID_BASE + 503
#define ID_PPMAC_STARTUP			ID_BASE + 504
#define ID_ANDROID_IP_ADDRESS		ID_BASE + 505
#define ID_ANDROID_STARTUP			ID_BASE + 506
#define ID_ANDROID_COMMS_TYPE		ID_BASE + 507
#define ID_ANDROID_IP_PORT			ID_BASE + 508

#define	ID_TOOLBAR_MAIN				ID_BASE + 550

#define	ID_UT_PREAMP_GAIN			ID_BASE + 600
#define	ID_UT_PREAMP_MODE			ID_BASE + 601
#define	ID_UT_DAMPING				ID_BASE + 602
#define	ID_UT_PULSE_WIDTH			ID_BASE + 603
#define	ID_UT_PULSE_VOLTAGE			ID_BASE + 604
#define	ID_UT_PRF					ID_BASE + 605
#define	ID_UT_LINEAR_GAIN			ID_BASE + 606
#define	ID_UT_LOG_GAIN				ID_BASE + 607
#define	ID_UT_OUTPUT_MODE			ID_BASE + 608
#define	ID_UT_RECTIFY_MODE			ID_BASE + 609
#define	ID_UT_FILTER_TYPE			ID_BASE + 610
#define	ID_UT_FILTER_RANGE			ID_BASE + 611
#define	ID_UT_FILTER_CENTER			ID_BASE + 612
#define	ID_UT_FILTER_CUT_OFF		ID_BASE + 613
#define	ID_UT_DAC_MODE				ID_BASE + 614
#define	ID_UT_DAC_TRIGGER			ID_BASE + 615
#define	ID_UT_DAC_DELAY				ID_BASE + 616
#define	ID_UT_DAC_THRESHOLD			ID_BASE + 617
#define	ID_UT_DAC_TABLE				ID_BASE + 618
#define	ID_UT_SCOPE_DELAY			ID_BASE + 619
#define	ID_UT_SCOPE_WIDTH			ID_BASE + 620
#define	ID_UT_GATE_MODE				ID_BASE + 621
#define	ID_UT_GATE_DELAY			ID_BASE + 622
#define	ID_UT_GATE_WIDTH			ID_BASE + 623
#define	ID_UT_GATE_THRESHOLD		ID_BASE + 624
#define ID_UT_GATE_FAR_DEAD_ZONE	ID_BASE + 625
#define ID_UT_GATE_FAR_PEAK_WIDTH	ID_BASE + 626
#define ID_UT_AMPLITUDE_COLLECT		ID_BASE + 627
#define ID_UT_TOF_COLLECT			ID_BASE + 628
#define ID_UT_TOF_MODE				ID_BASE + 629
#define ID_UT_PULSER_OFF_ON			ID_BASE + 630
#define ID_UT_TX					ID_BASE + 631
#define ID_UT_RX					ID_BASE + 632

#define ID_COPY_UT_PREAMP_GAIN		ID_BASE + 700
#define ID_COPY_UT_PREAMP_MODE		ID_BASE + 701
#define ID_COPY_UT_DAMPING			ID_BASE + 702
#define ID_COPY_UT_PULSE_WIDTH		ID_BASE + 703
#define ID_COPY_UT_PULSE_VOLTAGE	ID_BASE + 704
#define ID_COPY_UT_LINEAR_GAIN		ID_BASE + 705
#define ID_COPY_UT_LOG_GAIN			ID_BASE + 706
#define ID_COPY_UT_OUTPUT_MODE		ID_BASE + 707
#define ID_COPY_UT_RECTIFY_MODE		ID_BASE + 708
#define ID_COPY_UT_FILTER_TYPE		ID_BASE + 709
#define ID_COPY_UT_FILTER_RANGE		ID_BASE + 710
#define ID_COPY_UT_FILTER_CENTER	ID_BASE + 711
#define ID_COPY_UT_FILTER_CUT_OFF	ID_BASE + 712
#define ID_COPY_UT_DAC_MODE			ID_BASE + 713
#define ID_COPY_UT_DAC_TRIGGER		ID_BASE + 714
#define ID_COPY_UT_DAC_DELAY		ID_BASE + 715
#define ID_COPY_UT_DAC_THRESHOLD	ID_BASE + 716
#define ID_COPY_UT_DAC_TABLE		ID_BASE + 717
#define ID_COPY_UT_SCOPE_DELAY		ID_BASE + 718
#define ID_COPY_UT_SCOPE_WIDTH		ID_BASE + 719
#define ID_COPY_UT_GATE_MODE		ID_BASE + 720
#define ID_COPY_UT_GATE_DELAY		ID_BASE + 721
#define ID_COPY_UT_GATE_WIDTH		ID_BASE + 722
#define ID_COPY_UT_GATE_THRESHOLD	ID_BASE + 723
#define ID_COPY_UT_GATE_FAR_DEAD_ZONE	ID_BASE + 724
#define ID_COPY_UT_GATE_FAR_PEAK_WIDTH	ID_BASE + 725
#define ID_COPY_UT_AMPLITUDE_COLLECT	ID_BASE + 726
#define ID_COPY_UT_TOF_COLLECT		ID_BASE + 727
#define ID_COPY_UT_TOF_MODE			ID_BASE + 728
#define ID_COPY_UT_PRF				ID_BASE + 729
#define ID_COPY_UT_PULSER_OFF_ON	ID_BASE + 730
#define ID_COPY_UT_TX				ID_BASE + 731
#define ID_COPY_UT_RX				ID_BASE + 732




IMPLEMENT_DYNAMIC(CPropertiesPane, CDockablePane)

CPropertiesPane::CPropertiesPane()
{
	m_nWhichList = Application;
	m_nComboHeight = 0;
}

CPropertiesPane::~CPropertiesPane()
{
}


BEGIN_MESSAGE_MAP(CPropertiesPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_CBN_SELENDOK(ID_COMBO_PROPERTY, OnSelendokProperty)
	ON_NOTIFY(NM_RCLICK, IDC_PROPERTY_LIST, OnRclickPropertyList)
	ON_COMMAND(ID_UT_FILLALL, OnUtSubsetFillall)
	ON_COMMAND(ID_UT_CLEARALL, OnUtSubsetClearall)
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(UI_SET_ACCESS_PRIVELAGES, SetAccessPrivelages)
END_MESSAGE_MAP()



// CPropertiesPane message handlers




int CPropertiesPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_COMBO_PROPERTY))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}


	m_wndObjectCombo.AddString(LoadString(IDS_Application));
	m_wndObjectCombo.AddString(LoadString(IDS_Kinematics));
	m_wndObjectCombo.AddString(LoadString(IDS_Scope));
	m_wndObjectCombo.AddString(LoadString(IDS_Tabs));
	m_wndObjectCombo.AddString(LoadString(IDS_Tool_bars));
	m_wndObjectCombo.AddString(LoadString(IDS_Annotation));
	m_wndObjectCombo.AddString(_T("Path names"));
	m_wndObjectCombo.AddString(LoadString(IDS_Motion));
	m_wndObjectCombo.AddString(LoadString(IDS_Machine_Description));
	m_wndObjectCombo.AddString(LoadString(IDS_Profile));
	m_wndObjectCombo.AddString(LoadString(IDS_CScans));
	m_wndObjectCombo.AddString(LoadString(IDS_BScans));
	m_wndObjectCombo.AddString(LoadString(IDS_Palettes));
	m_wndObjectCombo.AddString(LoadString(IDS_Joystick));
	m_wndObjectCombo.AddString(LoadString(IDS_World_view));
	m_wndObjectCombo.AddString(LoadString(IDS_Network_Attached_Devices));
	m_wndObjectCombo.AddString(LoadString(IDS_Ut_subset));
	m_wndObjectCombo.AddString(LoadString(IDS_Ut_copy));
	m_wndObjectCombo.AddString(LoadString(IDS_Position_display));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_PROPERTY_LIST))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList(Application);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_STATUS_TOOLBAR);
	m_wndToolBar.LoadToolBar(IDR_STATUS_TOOLBAR, 0, 0, TRUE /* Is locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CreateFont();
	AdjustLayout();

	return 0;
}


void CPropertiesPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}


void CPropertiesPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect(&rectCombo);
	m_nComboHeight = rectCombo.Height() - 1;

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
//	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	cyTlb = 0;
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}


int CPropertiesPane::CreateFont()
{
	LOGFONT lf;
	::DeleteObject(m_Font.Detach());

	if (theApp.m_LastSettings.bUseAlernativeToSystemFont == false) {
		afxGlobalData.fontRegular.GetLogFont(&lf);

		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);

		afxGlobalData.GetNonClientMetrics(info);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;
	}
	else {
		lf = theApp.m_LastSettings.lfMenus;
	}
	lf.lfHeight = MulDiv(lf.lfHeight, theApp.nPercentageTextSize[theApp.m_LastSettings.nTextPercentageSize = 3], 100);
	m_Font.CreateFontIndirect(&lf);
	m_wndPropList.SetFont(&m_Font);
	m_wndObjectCombo.SetFont(&m_Font);

	return 0;
}

HRESULT CPropertiesPane::SetAccessPrivelages(WPARAM wp, LPARAM lp)
{
	bool bFlag;
	theApp.m_nLogonLevelMask&theApp.m_cAccess[m_nID] ? bFlag = true : bFlag = false;

	int nGroupCount = m_wndPropList.GetPropertyCount();
	for (int ii = 0; ii < nGroupCount; ii++) {

		CMFCPropertyGridProperty* pGroup = m_wndPropList.GetProperty(ii);
		int nItemCount = pGroup->GetSubItemsCount();
		for (int nItem = 0; nItem < nItemCount; nItem++) {
			CMFCPropertyGridProperty* pProperty = pGroup->GetSubItem(nItem);
			pProperty->Enable(bFlag);
		}
	}
	return 0;
}


int CPropertiesPane::ChangeFontSize()
{
	CreateFont();
	AdjustLayout();
	return 0;
}


int CPropertiesPane::MainFrameRedraw(bool bAction)
{
	if (GetSafeHwnd()) {
		ChangeFontSize();
		InitPropList(m_nWhichList);
	}
	return 0;
}

void CPropertiesPane::InitPropList(PropertiesList nWhichList)
{

	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	switch (m_nWhichList = nWhichList) {
	case Application:	InitPropListApplication();
		break;
	case Kinematics:	InitPropListKinematics();
		break;
	case Scope:	InitPropListScope();
		break;
	case Tabs:	InitPropListTabs();
		break;
	case Toolbars:	InitPropListToolbars();
		break;
	case CScan: InitPropListCScan();
		break;
	case Profile: InitPropListProfile();
		break;
	case NetworkAttachedDevices: InitPropListNAD();
		break;
	case Machine: InitPropListMachineDescription();
		break;
	case UtSubset: InitPropListUtSubset();
		break;
	case UtCopy: InitPropListUtCopy();
		break;
	case PositionDisplay: InitPropListPositionDisplay();
		break;
	}
	m_wndObjectCombo.SetCurSel(nWhichList);
	SetAccessPrivelages();
}

void CPropertiesPane::OnSelendokProperty()
{
	InitPropList((PropertiesList)m_wndObjectCombo.GetCurSel());
}

void CPropertiesPane::InitPropListApplication()
{
	CString Language[] = { L"English", L"Chinese", L"French", L"German", L"Russian", L"Spanish" };
	CString Units[] = { L"mm", L"\x022" };
	CString Side[] = { L"Left", L"Right" };
	CUSLPropertyGridProperty* pPropSpin;
	CMFCPropertyGridFileProperty* pPropName;
	CMFCPropertyGridColorProperty* pColorProp;
	CMFCPropertyGridProperty* pProp;
	bool		bFlag;

	MinMax(&theApp.m_nLanguage, 0, 5);
	MinMax(&theApp.m_LastSettings.nMeasurementUnits, 0, 1);
	MinMax(&theApp.m_nSide0Orientation, 0, 1);

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("System"));
	CMFCPropertyGridProperty* pPropLanguage = new CMFCPropertyGridProperty(_T("Language"), (_variant_t)Language[theApp.m_nLanguage], _T("Choose your language of choice "), ID_LANGUAGE);
	pPropLanguage->AddOption(Language[0]);
	pPropLanguage->AddOption(Language[1]);
	pPropLanguage->AddOption(Language[2]);
	pPropLanguage->AddOption(Language[3]);
	pPropLanguage->AddOption(Language[4]);
	pPropLanguage->AddOption(Language[5]);
	pPropLanguage->AllowEdit(FALSE);
	pGroup1->AddSubItem(pPropLanguage);
	CUSLPropertyGridProperty* pPropUnits = new CUSLPropertyGridProperty(_T("Units"), (_variant_t)Units[theApp.m_LastSettings.nMeasurementUnits], _T("If you're in the real world you'll choose mm\nAll those in the USA will probably choose inches "), ID_UNITS);
	pPropUnits->AddOption(Units[0]);
	pPropUnits->AddOption(Units[1]);
	pPropUnits->AllowEdit(FALSE);
	pGroup1->AddSubItem(pPropUnits);
	CUSLPropertyGridProperty* pPropSide = new CUSLPropertyGridProperty(_T("Side 1"), (_variant_t)Side[theApp.m_nSide0Orientation], _T("Which side of the window is side 1\nThis is to cope with scanners the wrong way round "), ID_SIDE_1);
	pPropSide->AddOption(Side[0]);
	pPropSide->AddOption(Side[1]);
	pPropSide->AllowEdit(FALSE);
	pGroup1->AddSubItem(pPropSide);
	theApp.m_LastSettings.bWindowPos == true ? bFlag = true : bFlag = false;
	pProp = new CMFCPropertyGridProperty(L"Load windows on last exit", (_variant_t)bFlag, L"Place the windows in the position they were last displayed", ID_LOAD_WINDOS_ON_LAST_EXIT);
	pGroup1->AddSubItem(pProp);

	m_wndPropList.AddProperty(pGroup1);


	pGroup1 = new CMFCPropertyGridProperty(_T("Font"));
	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("Arial"));

	pGroup1->AddSubItem(new CMFCPropertyGridFontProperty(_T("Alternative font"), theApp.m_LastSettings.lfMenus, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window"), ID_MENU_FONT));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Use alternative font"), (_variant_t)theApp.m_LastSettings.bUseAlernativeToSystemFont, _T("Specifies that the window uses MS Shell Dlg font"), ID_USE_SYSTEM_FONT));

	m_wndPropList.AddProperty(pGroup1);

	pGroup1 = new CMFCPropertyGridProperty(_T("Log on"));
	pProp = new CMFCPropertyGridProperty(_T("Force Log on"), (_variant_t)theApp.m_LastSettings.bForceLogon, _T("Force Log on at start of program "), ID_FORCE_LOGON);
	pGroup1->AddSubItem(pProp);
	pPropSpin = new CUSLPropertyGridProperty(_T("Log off after"), (_variant_t)(int)(0), _T("The system will automatialy log off after this period"), ID_LOG_OFF_AFTER, L"%.0f min", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, (float)theApp.m_LastSettings.nTimoutPeriodMinutes, 0.0f, 480.0f, 0);
	pGroup1->AddSubItem(pPropSpin);

	m_wndPropList.AddProperty(pGroup1);

	pGroup1 = new CMFCPropertyGridProperty(LoadString(IDS_Titles_and_Names));
	pPropName = new CMFCPropertyGridFileProperty(LoadString(IDS_Machine_Side_1), theApp.m_DlgSideName[0], ID_MACHINE_SIDE_NAME_0, L"What would you like side 1 of the machine to be called");
	pGroup1->AddSubItem(pPropName);
	pPropName = new CMFCPropertyGridFileProperty(LoadString(IDS_Machine_Side_2), theApp.m_DlgSideName[1], ID_MACHINE_SIDE_NAME_1, L"What would you like side 2 of the machine to be called");
	pGroup1->AddSubItem(pPropName);
	pPropName = new CMFCPropertyGridFileProperty(LoadString(IDS_Machine_both), theApp.m_DlgSideName[2], ID_MACHINE_SIDE_NAME_2, L"How would you like to refer to both sides of the machine");
	pGroup1->AddSubItem(pPropName);
	pPropName = new CMFCPropertyGridFileProperty(LoadString(IDS_Reference) + L"1", theApp.m_3PtName[0], ID_MACHINE_REF_NAME_0, L"How would you like to refer to reference 1");
	pGroup1->AddSubItem(pPropName);
	pPropName = new CMFCPropertyGridFileProperty(LoadString(IDS_Reference) + L"2", theApp.m_3PtName[1], ID_MACHINE_REF_NAME_1, L"How would you like to refer to reference 2");
	pGroup1->AddSubItem(pPropName);
	pPropName = new CMFCPropertyGridFileProperty(LoadString(IDS_Reference) + L"3", theApp.m_3PtName[2], ID_MACHINE_REF_NAME_2, L"How would you like to refer to reference 3");
	pGroup1->AddSubItem(pPropName);

	m_wndPropList.AddProperty(pGroup1);

	pGroup1 = new CMFCPropertyGridProperty(L"Profile edit");
	pColorProp = new CMFCPropertyGridColorProperty(_T("Taught lines"), (_variant_t)theApp.m_LastSettings.rgbDisplayTaughtColor, NULL, _T("Specifies the taught lines colour"), ID_TAUGHT_LINES_COLOUR);
	pGroup1->AddSubItem(pColorProp);
	pColorProp = new CMFCPropertyGridColorProperty(_T("Scan lines"), (_variant_t)theApp.m_LastSettings.rgbDisplayScanColor, NULL, _T("Specifies the scan lines colour"), ID_SCAN_LINES_COLOUR);
	pGroup1->AddSubItem(pColorProp);
	pColorProp = new CMFCPropertyGridColorProperty(_T("Brane lines"), (_variant_t)theApp.m_LastSettings.rgbDisplayBrainColor, NULL, _T("Specifies the brane lines colour"), ID_BRANE_LINES_COLOUR);
	pGroup1->AddSubItem(pColorProp);
	pColorProp = new CMFCPropertyGridColorProperty(_T("Envelope lines"), (_variant_t)theApp.m_LastSettings.rgbDisplayEnvelopeColor, NULL, _T("Specifies the envelope lines colour"), ID_ENVELOPE_LINES_COLOUR);
	pGroup1->AddSubItem(pColorProp);
	pColorProp = new CMFCPropertyGridColorProperty(_T("Inside lines"), (_variant_t)theApp.m_LastSettings.rgbBladeColor[INSIDE], NULL, _T("Specifies the inside lines colour"), ID_INSIDE_LINES_COLOUR);
	pGroup1->AddSubItem(pColorProp);
	pColorProp = new CMFCPropertyGridColorProperty(_T("Outside lines"), (_variant_t)theApp.m_LastSettings.rgbBladeColor[OUTSIDE], NULL, _T("Specifies the outside lines colour"), ID_OUTSIDE_LINES_COLOUR);
	pGroup1->AddSubItem(pColorProp);

	m_wndPropList.AddProperty(pGroup1);

	
	pGroup1 = new CMFCPropertyGridProperty(L"Calibration");
	pPropSpin = new CUSLPropertyGridProperty(_T("Thickness every"), (_variant_t)(int)(0), _T("Recalibrate the thickness"), ID_THICKNESS_CALIBRATION_PERIOD, L"%.0f min", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, (float)theApp.m_LastSettings.osThicknessReCalibrationPeriodMinutes, 0.0f, 1000.0f, 0);
	pGroup1->AddSubItem(pPropSpin);

	m_wndPropList.AddProperty(pGroup1);
}

void CPropertiesPane::InitPropListPositionDisplay()
{
	CMFCPropertyGridColorProperty* pColorProp;
	CMFCPropertyGridProperty* pGroup1;
	CMFCPropertyGridProperty* pProp;
	bool		bFlag;

	pGroup1 = new CMFCPropertyGridProperty(LoadString(IDS_Position));
	pGroup1->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), theApp.m_LastSettings.lfPositionPane, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the font for the position window"), ID_POSITION_MENU_FONT, theApp.m_LastSettings.rgbTextPositionPane));
	pColorProp = new CMFCPropertyGridColorProperty(_T("Text Color"), (_variant_t)theApp.m_LastSettings.rgbTextPositionPane, NULL, _T("Specifies the text color"), ID_POSITION_TEXT_COLOUR);
	pGroup1->AddSubItem(pColorProp);
	pColorProp = new CMFCPropertyGridColorProperty(_T("Background Color"), (_variant_t)theApp.m_LastSettings.rgbBKPositionPane, NULL, _T("Specifies the background color"), ID_POSITION_BK_COLOUR);
	pGroup1->AddSubItem(pColorProp);
	theApp.m_LastSettings.nPositionPaneMask & POSITION_SIDE_0 ? bFlag = true : bFlag = false;
	pProp = new CMFCPropertyGridProperty(L"Machine side 1", (_variant_t)bFlag, L"Render side 1 coordinates", ID_POSITION_SIDE_0);
	pGroup1->AddSubItem(pProp);
	theApp.m_LastSettings.nPositionPaneMask & POSITION_SIDE_1 ? bFlag = true : bFlag = false;
	pProp = new CMFCPropertyGridProperty(L"Machine side 2", (_variant_t)bFlag, L"Render side 2 coordinates", ID_POSITION_SIDE_1);
	pGroup1->AddSubItem(pProp);
	theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_NORM ? bFlag = true : bFlag = false;
	pProp = new CMFCPropertyGridProperty(L"Normals", (_variant_t)bFlag, L"Render normals", ID_POSITION_DISPLAY_NORM);
	pGroup1->AddSubItem(pProp);

	theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_A ? bFlag = true : bFlag = false;
	pProp = new CMFCPropertyGridProperty(L"Angle A", (_variant_t)bFlag, L"Render angle A / Xtip", ID_POSITION_DISPLAY_A);
	pGroup1->AddSubItem(pProp);
	theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_B ? bFlag = true : bFlag = false;
	pProp = new CMFCPropertyGridProperty(L"Angle B", (_variant_t)bFlag, L"Render angle B / Ytip", ID_POSITION_DISPLAY_B);
	pGroup1->AddSubItem(pProp);

	theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_ROT_NORM ? bFlag = true : bFlag = false;
	pProp = new CMFCPropertyGridProperty(L"Rotate about normal", (_variant_t)bFlag, L"Render rotate about normal", ID_POSITION_DISPLAY_ROT_NORM);
	pGroup1->AddSubItem(pProp);
	theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_QUAT ? bFlag = true : bFlag = false;
	pProp = new CMFCPropertyGridProperty(L"Quaternions", (_variant_t)bFlag, L"Render quaternions", ID_POSITION_DISPLAY_QUAT);
	pGroup1->AddSubItem(pProp);
	theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_R0 ? bFlag = true : bFlag = false;
	pProp = new CMFCPropertyGridProperty(L"Turntable", (_variant_t)bFlag, L"Render turntable", ID_POSITION_DISPLAY_R0);
	pGroup1->AddSubItem(pProp);
	theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_R1 ? bFlag = true : bFlag = false;
	pProp = new CMFCPropertyGridProperty(L"Rollers", (_variant_t)bFlag, L"Render rollers", ID_POSITION_DISPLAY_R1);
	pGroup1->AddSubItem(pProp);

	m_wndPropList.AddProperty(pGroup1);

}

void CPropertiesPane::InitPropListKinematics()
{
	CUSLPropertyGridProperty* pPropSpin;
	CMFCPropertyGridProperty* pGroup1;
	CUSLPropertyGridProperty* pPropCombo;
	CString		Buff;
	CString Controller[5] = { L"SI6", L"PMAC Turbo", L"PMAC 8 Axis", L"SI10", L"Power PMAC" };


	pGroup1 = new CMFCPropertyGridProperty(_T("Kinematics"));
	Buff.Format(L"Remove air %s", theApp.m_DlgSideName[0]);
	pPropSpin = new CUSLPropertyGridProperty(Buff, (_variant_t)(int)(0), _T("Angle probe tips to remove air"), ID_REMOVE_AIR_0, L"%.0f\x0b0", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_LastSettings.fRemoveAirAngle[PORTSIDE], -90.0f, 90.0f, 0);
	pGroup1->AddSubItem(pPropSpin);
	Buff.Format(L"Remove air %s", theApp.m_DlgSideName[1]);
	pPropSpin = new CUSLPropertyGridProperty(Buff, (_variant_t)(int)(0), _T("Angle probe tips to remove air"), ID_REMOVE_AIR_1, L"%.0f\x0b0", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_LastSettings.fRemoveAirAngle[STARBOARD], -90.0f, 90.0f, 0);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Dwell time", (_variant_t)(int)(0), _T("The time during which the air is allowed to escape"), ID_AIR_DWELL_TIME, L"%.0fs", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, (float)theApp.m_LastSettings.nRemoveAirTime, 0.0f, 20.0f, 0);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Delta scan speed", (_variant_t)(int)(0), _T("Step size when using the spin buttons or mouse wheel"), ID_DELTA_SCAN_SPEED, L"%.0f mm/s", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_LastSettings.fDeltaScanSpeed, 1.0f, 100.0f, 0);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Delta scan interval", (_variant_t)(int)(0), _T("Step size when using the spin buttons or mouse wheel"), ID_DELTA_SCAN_INTERVAL, L"%.02f mm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_LastSettings.fDeltaScanIncrement, 0.01f, 10.0f, 2);
	pGroup1->AddSubItem(pPropSpin);

	m_wndPropList.AddProperty(pGroup1);

	pGroup1 = new CMFCPropertyGridProperty(_T("Hardware"));
	pPropCombo = new CUSLPropertyGridProperty(_T("Motion controller"), (_variant_t)Controller[theApp.m_nMotorType], _T("Select motion controller"), ID_MACHINE_MOTION_CONTROLLER);
	for(int ii=0;ii<5;ii++)
		pPropCombo->AddOption(Controller[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup1->AddSubItem(pPropCombo);

	m_wndPropList.AddProperty(pGroup1);

	pGroup1 = new CMFCPropertyGridProperty(_T("Joystick"));
	pPropSpin = new CUSLPropertyGridProperty(L"Side 1 X", (_variant_t)(int)(0), L"Jog X speed", ID_JOG_SPEED_X_0, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[0].m_fJogDirSpeedX, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Y", (_variant_t)(int)(0), L"Jog Y speed", ID_JOG_SPEED_Y_0, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[0].m_fJogDirSpeedY, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Z", (_variant_t)(int)(0), L"Jog Z speed", ID_JOG_SPEED_Z_0, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[0].m_fJogDirSpeedZ, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Horizontal", (_variant_t)(int)(0), L"Jog Horizontal speed", ID_JOG_SPEED_H_0, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[0].m_fJogDirSpeedHTip, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Vertical", (_variant_t)(int)(0), L"Jog Vertical speed", ID_JOG_SPEED_V_0, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[0].m_fJogDirSpeedVTip, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Theta", (_variant_t)(int)(0), L"Jog Theta speed", ID_JOG_SPEED_T_0, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[0].m_fJogDirSpeedTTip, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Rotate", (_variant_t)(int)(0), L"Jog Rotate speed", ID_JOG_SPEED_R_0, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[0].m_fJogDirSpeedR, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Side 2 X", (_variant_t)(int)(0), L"Jog X speed", ID_JOG_SPEED_X_1, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[1].m_fJogDirSpeedX, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Y", (_variant_t)(int)(0), L"Jog Y speed", ID_JOG_SPEED_Y_1, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[1].m_fJogDirSpeedY, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Z", (_variant_t)(int)(0), L"Jog Z speed", ID_JOG_SPEED_Z_1, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[1].m_fJogDirSpeedZ, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Horizontal", (_variant_t)(int)(0), L"Jog Horizontal speed", ID_JOG_SPEED_H_1, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[1].m_fJogDirSpeedHTip, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Vertical", (_variant_t)(int)(0), L"Jog Vertical speed", ID_JOG_SPEED_V_1, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[1].m_fJogDirSpeedVTip, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Theta", (_variant_t)(int)(0), L"Jog Theta speed", ID_JOG_SPEED_T_1, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[1].m_fJogDirSpeedTTip, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Rotate", (_variant_t)(int)(0), L"Jog Rotate speed", ID_JOG_SPEED_R_1, L"%.03f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Robot[1].m_fJogDirSpeedR, 0.001f, 10.0f, 3);
	pGroup1->AddSubItem(pPropSpin);

	m_wndPropList.AddProperty(pGroup1);

	pGroup1 = new CMFCPropertyGridProperty(_T("Speeds"));
	pPropSpin = new CUSLPropertyGridProperty(L"Move", (_variant_t)(int)(0), L"Move speed", ID_MOVE_SPEED, L"%.01f mm/s", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_LastSettings.fMoveSpeed, 0.1f, 2000.0f, 1);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Orthogonal", (_variant_t)(int)(0), L"Orthogonal speed", ID_ORTHOGONAL_SPEED, L"%.01f mm/s", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_LastSettings.fPeekOrthogonalSpeed, 0.1f, 2000.0f, 1);
	pGroup1->AddSubItem(pPropSpin);

	m_wndPropList.AddProperty(pGroup1);

}

void CPropertiesPane::InitPropListProfile()
{
	CUSLPropertyGridProperty* pPropSpin;
	CMFCPropertyGridProperty* pGroup1;
	CUSLPropertyGridProperty* pPropCombo;
	CMFCPropertyGridFileProperty* pPropName;
	CMFCPropertyGridFileProperty* pPropFile;
	CString		Buff, Precision;
	int nDir = 0;
	CString Dir[] = { L"i", L"j", L"k" };
	CString Description[] = { L"Rectilinear", L"Open rotary", L"Closed rotary", L"Circular" };

	pGroup1 = new CMFCPropertyGridProperty(_T("Description"));
	pPropName = new CMFCPropertyGridFileProperty(LoadString(IDS_Name), PROFILE->m_strDescription, ID_PROFILE_NAME, L"Unique identifier for this component");
	pGroup1->AddSubItem(pPropName);
	Buff.Format(L"%s direction", theApp.m_DlgSideName[0]);
	pPropCombo = new CUSLPropertyGridProperty(L"Shape", (_variant_t)Description[PROFILE->m_nComponentDescription], _T("Choose the general shape of the component and how it will be scanned "), ID_PROFILE_DESCRIPTION);
	for (int ii = 0; ii < 3; ii++)
		pPropCombo->AddOption(Description[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup1->AddSubItem(pPropCombo);
	switch (theApp.m_LastSettings.nMeasurementUnits) {
	default: Precision.Format(L"%%.0%df mm", theApp.m_LastSettings.nStepSizePrecision);
		break;
	case 1:  Precision = L"%.03f\x022";
		break;
	}
	pPropSpin = new CUSLPropertyGridProperty(L"Fast increment", (_variant_t)(int)(0), _T("The sample separation along a scan line"), ID_PROFILE_FAST_INCREMENT, Precision, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, PROFILE->m_fFastIncrement * theApp.m_fUnits, 0.0f, 100.0f, theApp.m_LastSettings.nStepSizePrecision);
	pGroup1->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(L"Slow increment", (_variant_t)(int)(0), _T("The line separation"), ID_PROFILE_SLOW_INCREMENT, Precision, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, PROFILE->m_fSlowIncrement * theApp.m_fUnits, 0.0f, 100.0f, theApp.m_LastSettings.nStepSizePrecision);
	pGroup1->AddSubItem(pPropSpin);
	pPropFile = new CMFCPropertyGridFileProperty(L"Ultrasonics", TRUE, PROFILE->m_UltrasonicPathFileName, L"ul", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, L"Select ultrasonic file", ID_PROFILE_ULTRASONICS);
	pGroup1->AddSubItem(pPropFile);


	m_wndPropList.AddProperty(pGroup1);

	pGroup1 = new CMFCPropertyGridProperty(_T("Danger planes"));
	nDir = 0;
	if (PROFILE->m_vecToDangerPlane[0].x != 0.0f)	nDir = 0;
	if (PROFILE->m_vecToDangerPlane[0].y != 0.0f)	nDir = 1;
	if (PROFILE->m_vecToDangerPlane[0].z != 0.0f)	nDir = 2;
	Buff.Format(L"%s direction", theApp.m_DlgSideName[0]);
	pPropCombo = new CUSLPropertyGridProperty(Buff, (_variant_t)Dir[nDir], _T("Choose the direction to the danger plane "), ID_DANGER_PLANE_DIR_0);
	for (int ii = 0; ii < 3; ii++)
		pPropCombo->AddOption(Dir[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup1->AddSubItem(pPropCombo);
	nDir = 0;
	if (PROFILE->m_vecToDangerPlane[1].x != 0.0f)	nDir = 0;
	if (PROFILE->m_vecToDangerPlane[1].y != 0.0f)	nDir = 1;
	if (PROFILE->m_vecToDangerPlane[1].z != 0.0f)	nDir = 2;
	Buff.Format(L"%s direction", theApp.m_DlgSideName[1]);
	pPropCombo = new CUSLPropertyGridProperty(Buff, (_variant_t)Dir[nDir], _T("Choose the direction to the danger plane "), ID_DANGER_PLANE_DIR_1);
	for (int ii = 0; ii < 3; ii++)
		pPropCombo->AddOption(Dir[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup1->AddSubItem(pPropCombo);

	Buff.Format(L"%s margin", theApp.m_DlgSideName[0]);
	pPropSpin = new CUSLPropertyGridProperty(Buff, (_variant_t)(int)(0), _T("Danger plane margin"), ID_DANGER_PLANE_MARGIN_0, L"%.0fmm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_LastSettings.fDangerMargin[0], -300.0f, 300.0f, 0);
	pGroup1->AddSubItem(pPropSpin);
	Buff.Format(L"%s margin", theApp.m_DlgSideName[1]);
	pPropSpin = new CUSLPropertyGridProperty(Buff, (_variant_t)(int)(0), _T("Danger plane margin"), ID_DANGER_PLANE_MARGIN_1, L"%.0fmm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_LastSettings.fDangerMargin[1], -300.0f, 300.0f, 0);
	pGroup1->AddSubItem(pPropSpin);

	m_wndPropList.AddProperty(pGroup1);

}

void CPropertiesPane::InitPropListScope()
{
	CMFCPropertyGridColorProperty* pColor;
	CMFCPropertyGridFontProperty* pFont;
	CMFCPropertyGridProperty* pGroup;
	CUSLPropertyGridProperty* pPropCombo;
	CUSLPropertyGridProperty* pPropSpin;
	CString Buff;
	CString Units[] = { L"ns",L"\x0b5s", L"mm Water", L"mm Material", L"\x022 Water", L"\x022 Material" };
	CString InterfaceMarker[] = { L"None", L"Line", L"Arrow", L"Line & Arrow" };
	bool bFlag;
	int nSel;


	pGroup = new CMFCPropertyGridProperty(_T("Font && Colours"));
	pFont = new CMFCPropertyGridFontProperty(_T("Font"), theApp.m_LastSettings.lfScope, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the font for text "), ID_SCOPE_FONT, theApp.m_Scope.m_rgbTextColor);
	pGroup->AddSubItem(pFont);
	pPropSpin = new CUSLPropertyGridProperty(L"Font size", (_variant_t)(int)(0), _T("Size of font"), ID_SCOPE_FONT_HEIGHT, L"%.0f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, (float)theApp.m_Scope.m_nTextSize, 0.0f, 30.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	pColor = new CMFCPropertyGridColorProperty(_T("Trace"), (_variant_t)theApp.m_Scope.m_rgbTraceColor, NULL, _T("Specifies the trace color"), ID_TRACE_COLOUR);
	pGroup->AddSubItem(pColor);
	pColor = new CMFCPropertyGridColorProperty(_T("Background"), (_variant_t)theApp.m_Scope.m_rgbBkColor, NULL, _T("Specifies the background color"), ID_SCOPE_BK_COLOUR);
	pGroup->AddSubItem(pColor);
	pColor = new CMFCPropertyGridColorProperty(_T("Graticule"), (_variant_t)theApp.m_Scope.m_rgbGraticuleColor, NULL, _T("Specifies the graticule color"), ID_SCOPE_GRATICULE_COLOUR);
	pGroup->AddSubItem(pColor);
	pColor = new CMFCPropertyGridColorProperty(_T("DAC"), (_variant_t)theApp.m_Scope.m_rgbDACColor, NULL, _T("Specifies the DAC color"), ID_SCOPE_DAC_COLOUR);
	pGroup->AddSubItem(pColor);
	pColor = new CMFCPropertyGridColorProperty(_T("Text"), (_variant_t)theApp.m_Scope.m_rgbTextColor, NULL, _T("Specifies the Text color"), ID_SCOPE_TEXT_COLOUR);
	pGroup->AddSubItem(pColor);
	m_wndPropList.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Units"));
	pPropCombo = new CUSLPropertyGridProperty(_T("Gate delay"), (_variant_t)Units[theApp.m_Scope.m_nGateDelayUnits], _T("Choose the gate delay units "), ID_GATE_DELAY_UNITS);
	for (int ii = 0; ii < 6; ii++)
		pPropCombo->AddOption(Units[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup->AddSubItem(pPropCombo);
	pPropCombo = new CUSLPropertyGridProperty(_T("Gate width"), (_variant_t)Units[theApp.m_Scope.m_nGateWidthUnits], _T("Choose the gate width units "), ID_GATE_WIDTH_UNITS);
	for (int ii = 0; ii < 6; ii++)
		pPropCombo->AddOption(Units[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup->AddSubItem(pPropCombo);

	pPropCombo = new CUSLPropertyGridProperty(_T("Trace delay"), (_variant_t)Units[theApp.m_Scope.m_nTraceDelayUnits], _T("Choose the trace delay units "), ID_TRACE_DELAY_UNITS);
	for (int ii = 0; ii < 6; ii++)
		pPropCombo->AddOption(Units[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup->AddSubItem(pPropCombo);
	pPropCombo = new CUSLPropertyGridProperty(_T("Trace width"), (_variant_t)Units[theApp.m_Scope.m_nTraceWidthUnits], _T("Choose the trace width units "), ID_TRACE_WIDTH_UNITS);
	for (int ii = 0; ii < 6; ii++)
		pPropCombo->AddOption(Units[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup->AddSubItem(pPropCombo);

	pPropCombo = new CUSLPropertyGridProperty(_T("DAC blanking"), (_variant_t)Units[theApp.m_LastSettings.nDacBlankingUnits], _T("Choose the DAC blanking units "), ID_DAC_BLANKING_UNITS);
	for (int ii = 0; ii < 6; ii++)
		pPropCombo->AddOption(Units[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup->AddSubItem(pPropCombo);
	pPropCombo = new CUSLPropertyGridProperty(_T("DAC delay"), (_variant_t)Units[theApp.m_LastSettings.nDacDelayUnits], _T("Choose the DAC delay units "), ID_DAC_DELAY_UNITS);
	for (int ii = 0; ii < 6; ii++)
		pPropCombo->AddOption(Units[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup->AddSubItem(pPropCombo);

	pPropCombo = new CUSLPropertyGridProperty(_T("TOF"), (_variant_t)Units[theApp.m_Scope.m_nTOFText[1]], _T("Choose the Time Of Flight units "), ID_TOF_UNITS);
	for (int ii = 0; ii < 6; ii++)
		pPropCombo->AddOption(Units[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup->AddSubItem(pPropCombo);

	m_wndPropList.AddProperty(pGroup);


	pGroup = new CMFCPropertyGridProperty(_T("Gate Colours"));
	pColor = new CMFCPropertyGridColorProperty(_T("Interface"), (_variant_t)theApp.m_Scope.m_rgbGateColor[0], NULL, _T("Specifies the interface color"), ID_GATE_COLOUR);
	pGroup->AddSubItem(pColor);
	for (int nGate = 1; nGate < 8; nGate++) {
		Buff.Format(L"Gate %d", nGate);
		pColor = new CMFCPropertyGridColorProperty(Buff, (_variant_t)theApp.m_Scope.m_rgbGateColor[nGate], NULL, _T("Specifies the gate color"), ID_GATE_COLOUR + nGate);
		pGroup->AddSubItem(pColor);
	}
	m_wndPropList.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Markers"));
	nSel = 0;
	if (theApp.m_LastSettings.nScopeMask & SCOPE_INTERFACE_MARKER_LINE) nSel = 1;
	if (theApp.m_LastSettings.nScopeMask & SCOPE_INTERFACE_MARKER_ARROW) nSel = 2;
	if ((theApp.m_LastSettings.nScopeMask & (SCOPE_INTERFACE_MARKER_LINE | SCOPE_INTERFACE_MARKER_ARROW)) == (SCOPE_INTERFACE_MARKER_LINE | SCOPE_INTERFACE_MARKER_ARROW)) nSel = 3;
	pPropCombo = new CUSLPropertyGridProperty(_T("Interface"), (_variant_t)InterfaceMarker[nSel], _T("Style of marker used to point to the enterface position "), ID_INTERFACE_MARKER);
	pPropCombo->AddOption(InterfaceMarker[0]);
	pPropCombo->AddOption(InterfaceMarker[1]);
	pPropCombo->AddOption(InterfaceMarker[2]);
	pPropCombo->AddOption(InterfaceMarker[3]);
	pGroup->AddSubItem(pPropCombo);

	nSel = 0;
	if (theApp.m_LastSettings.nScopeMask & SCOPE_AMPLITUDE_MARKER_LINE) nSel = 1;
	if (theApp.m_LastSettings.nScopeMask & SCOPE_AMPLITUDE_MARKER_ARROW) nSel = 2;
	if ((theApp.m_LastSettings.nScopeMask & (SCOPE_AMPLITUDE_MARKER_LINE | SCOPE_AMPLITUDE_MARKER_ARROW)) == (SCOPE_AMPLITUDE_MARKER_LINE | SCOPE_AMPLITUDE_MARKER_ARROW)) nSel = 3;
	pPropCombo = new CUSLPropertyGridProperty(_T("Amplitude"), (_variant_t)InterfaceMarker[nSel], _T("Style of marker used to point to the amplitude within the gate"), ID_AMPLITUDE_MARKER);
	pPropCombo->AddOption(InterfaceMarker[0]);
	pPropCombo->AddOption(InterfaceMarker[1]);
	pPropCombo->AddOption(InterfaceMarker[2]);
	pPropCombo->AddOption(InterfaceMarker[3]);
	pGroup->AddSubItem(pPropCombo);

	nSel = 0;
	if (theApp.m_LastSettings.nScopeMask & SCOPE_TOF_MARKER_LINE) nSel = 1;
	if (theApp.m_LastSettings.nScopeMask & SCOPE_TOF_MARKER_ARROW) nSel = 2;
	if ((theApp.m_LastSettings.nScopeMask & (SCOPE_TOF_MARKER_LINE | SCOPE_TOF_MARKER_ARROW)) == (SCOPE_TOF_MARKER_LINE | SCOPE_TOF_MARKER_ARROW)) nSel = 3;
	pPropCombo = new CUSLPropertyGridProperty(_T("Time Of Flight"), (_variant_t)InterfaceMarker[nSel], _T("Style of marker used to point to the time of flight within the gate"), ID_TOF_MARKER);
	pPropCombo->AddOption(InterfaceMarker[0]);
	pPropCombo->AddOption(InterfaceMarker[1]);
	pPropCombo->AddOption(InterfaceMarker[2]);
	pPropCombo->AddOption(InterfaceMarker[3]);
	pGroup->AddSubItem(pPropCombo);

	m_wndPropList.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Render"));

	theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_WATER_PATH ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Water path"), (_variant_t)bFlag, _T("Display the water path distance between main bang and interface trigger"), ID_RENDER_WATER_PATH));

	theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_MATERIAL_PATH ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Material path"), (_variant_t)bFlag, _T("Display the distance between the interface and gate 1 trigger points"), ID_RENDER_MATERIAL_PATH));

	theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_COLLECT_TEXT ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Collect"), (_variant_t)bFlag, _T("Display the values of collected data"), ID_RENDER_COLLECT_TEXT));


	theApp.m_LastSettings.nScopeMask & SCOPE_GRATICULE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Graticule"), (_variant_t)bFlag, _T("Display the graticules in all the scopes"), ID_DISPLAY_GRATICULE));
	theApp.m_LastSettings.nScopeMask & SCOPE_LEFT_SCALE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Left scale"), (_variant_t)bFlag, _T("Display the left scale"), ID_LEFT_SCALE));
	theApp.m_LastSettings.nScopeMask & SCOPE_RIGHT_SCALE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Right scale"), (_variant_t)bFlag, _T("Display the right scale"), ID_RIGHT_SCALE));

	theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_CONVERSION_RATE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Conversion rate"), (_variant_t)bFlag, _T("Display the number of A-scan traces per second"), ID_CONVERSION_RATE));
	theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_SAMPLE_RATE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Sampling rate"), (_variant_t)bFlag, _T("Display the digitizer sampling rate"), ID_SAMPLING_RATE));
	theApp.m_LastSettings.nScopeMask & SCOPE_SAMPLES_PER_TRACE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Number samples"), (_variant_t)bFlag, _T("Display the number of samples per trace"), ID_NUMBER_SAMPLES_PER_TRACE));
	theApp.m_LastSettings.nScopeMask & SCOPE_TIME_TO_READ_ADC ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Time to read ADC"), (_variant_t)bFlag, NULL, ID_SCOPE_RENDER_TIME_ADC));
	theApp.m_LastSettings.nScopeMask & SCOPE_TIME_PROCESS_ASCAN ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Time to process A-scan"), (_variant_t)bFlag, NULL, ID_SCOPE_RENDER_TIME_ASCAN));

	m_wndPropList.AddProperty(pGroup);
}

void CPropertiesPane::InitPropListTabs()
{
	CMFCPropertyGridProperty* pGroup;
	bool bFlag;

	pGroup = new CMFCPropertyGridProperty(_T("Ultrasonics"));
	theApp.m_LastSettings.nUltrasonicsTabMask & PR20_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pr20", (_variant_t)bFlag, NULL, ID_PR20_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & PR50_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pr50", (_variant_t)bFlag, NULL, ID_PR50_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & PR30_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pr30", (_variant_t)bFlag, NULL, ID_PR30_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & PR40_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pr40", (_variant_t)bFlag, NULL, ID_PR40_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & SCOPE_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Scope", (_variant_t)bFlag, NULL, ID_SCOPE_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & GLOBAL_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Global", (_variant_t)bFlag, NULL, ID_GLOBAL_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & TIMESLOTS_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Timeslot", (_variant_t)bFlag, NULL, ID_TIMESLOTS_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & GATES_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gates", (_variant_t)bFlag, NULL, ID_GATES_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & SELF_TEST_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Self test", (_variant_t)bFlag, NULL, ID_SELF_TEST_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & EXT_SOURCE_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Ext source", (_variant_t)bFlag, NULL, ID_EXT_SOURCE_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & TRAFFIC_LIGHTS_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Traffic lights", (_variant_t)bFlag, NULL, ID_TRAFFIC_LIGHTS_TAB));
	theApp.m_LastSettings.nUltrasonicsTabMask & PHASED_ARRAY_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Phased array", (_variant_t)bFlag, NULL, ID_PHASED_ARRAY_TAB));

	m_wndPropList.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Motion tools"));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Rectilinear_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Rectilinear scan", (_variant_t)bFlag, NULL, ID_Rectilinear_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Turntable_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Turntable scan", (_variant_t)bFlag, NULL, ID_Turntable_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Rollers_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Rollers scan", (_variant_t)bFlag, NULL, ID_Rollers_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Contour_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Contour scan", (_variant_t)bFlag, NULL, ID_Contour_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Tubes_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Tubes scan", (_variant_t)bFlag, NULL, ID_Tubes_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Disc_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Disc scan", (_variant_t)bFlag, NULL, ID_Disc_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & PHASEDARRAY_LINE_SCAN_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Phased array line", (_variant_t)bFlag, NULL, ID_PHASEDARRAY_LINE_SCAN_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Tip_Axes_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Tip axes", (_variant_t)bFlag, NULL, ID_Tip_Axes_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Optimize_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Optimize", (_variant_t)bFlag, NULL, ID_Optimize_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Pump_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pumps", (_variant_t)bFlag, NULL, ID_Pump_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Manipulator_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Manipulator", (_variant_t)bFlag, NULL, ID_Manipulator_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Origin_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Origin", (_variant_t)bFlag, NULL, ID_Origin_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Move_To_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Move To", (_variant_t)bFlag, NULL, ID_Move_To_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Scan_Lines_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Jog", (_variant_t)bFlag, NULL, ID_Jog_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Jog_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Scan lines", (_variant_t)bFlag, NULL, ID_Scan_Lines_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Robots_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Robots", (_variant_t)bFlag, NULL, ID_Robot_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & Robots_Position_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Robot arm positions", (_variant_t)bFlag, NULL, ID_Robot_Position_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & TOF_CAL_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Thickness calibration", (_variant_t)bFlag, NULL, ID_TOF_CAL_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & TRANSPLY_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Transply", (_variant_t)bFlag, NULL, ID_TRANSPLY_TAB));
	theApp.m_LastSettings.nMotoionToolsTabMask[0] & DISCS_SHAFTS_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Discs and Shafts", (_variant_t)bFlag, NULL, ID_DISCS_SHAFTS_TAB));
	


	m_wndPropList.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Palette"));
	theApp.m_LastSettings.nPaletteTabMask & Amplitude_Palette_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Amplitude", (_variant_t)bFlag, NULL, ID_Amplitude_Palette_TAB));
	theApp.m_LastSettings.nPaletteTabMask & Thickness_Palette_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Thickness", (_variant_t)bFlag, NULL, ID_Thickness_Palette_TAB));
	theApp.m_LastSettings.nPaletteTabMask & Porosity_Palette_TAB ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Porosity", (_variant_t)bFlag, NULL, ID_Porosity_Palette_TAB));

	m_wndPropList.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Profile"));
	theApp.m_LastSettings.nProfileTabMask & ProfileCartesianTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Cartesian", (_variant_t)bFlag, NULL, ID_ProfileCartesianTab));
	theApp.m_LastSettings.nProfileTabMask & ProfileTurTableTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Turntable", (_variant_t)bFlag, NULL, ID_ProfileTurTableTab));
	theApp.m_LastSettings.nProfileTabMask & Profile3DDesignTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"3D Design", (_variant_t)bFlag, NULL, ID_Profile3DDesignTab));
	theApp.m_LastSettings.nProfileTabMask & Profile3DViewTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"3D View", (_variant_t)bFlag, NULL, ID_Profile3DViewTab));
	theApp.m_LastSettings.nProfileTabMask & Profile3DStiffenerTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Stiffeners and ribs", (_variant_t)bFlag, NULL, ID_Profile3DStiffenerTab));
	theApp.m_LastSettings.nProfileTabMask & Profile3DBladeTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Fan blades", (_variant_t)bFlag, NULL, ID_Profile3DBladeTab));

	m_wndPropList.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Position"));
	theApp.m_LastSettings.nPositionTabMask & PositionPositionTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"General", (_variant_t)bFlag, NULL, ID_PositionPositionTab));
	theApp.m_LastSettings.nPositionTabMask & PositionSingleBridgeTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Single", (_variant_t)bFlag, NULL, ID_PositionSingleBridgeTab));
	theApp.m_LastSettings.nPositionTabMask & PositionRobotDualTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Dual", (_variant_t)bFlag, NULL, ID_PositionRobotDualTab));
	theApp.m_LastSettings.nPositionTabMask & PositionHoldingTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Holding", (_variant_t)bFlag, NULL, ID_PositionHoldingTab));
	theApp.m_LastSettings.nPositionTabMask & PositionGraphicalTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Graphical", (_variant_t)bFlag, NULL, ID_PositionGraphicalTab));
	theApp.m_LastSettings.nPositionTabMask & PositionSwivGimTab ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Swival Gimbal", (_variant_t)bFlag, NULL, ID_PositionSwivGimTab));

	m_wndPropList.AddProperty(pGroup);
}


void CPropertiesPane::InitPropListToolbars()
{
	CMFCPropertyGridProperty* pGroup;
	bool bFlag;
	CString strBasePane, strMFCToolBar, strName;

	pGroup = new CMFCPropertyGridProperty(_T("Main frame"));
	for (int ii = 0; ii < sizeof m_nToolBarID / sizeof(int); ii++) {
		strBasePane.Format(L"BasePane-%d", m_nToolBarID[ii]);
		strMFCToolBar.Format(L"MFCToolBar-%d", m_nToolBarID[ii]);

		strName = theApp.GetSectionString(strMFCToolBar, L"Name", L"");
		theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? bFlag = true : bFlag = false;
		pGroup->AddSubItem(new CMFCPropertyGridProperty(strName, (_variant_t)bFlag, NULL, ID_TOOLBAR_MAIN + ii));
	}
	m_wndPropList.AddProperty(pGroup);


}

void CPropertiesPane::InitPropListCScan()
{
	CUSLPropertyGridProperty* pPropCombo;
	CString		Buff;
	CString		ThickErr[6];

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Data"));
	ThickErr[0].LoadString(IDS_Error);
	ThickErr[1].LoadString(IDS_Zero);
	ThickErr[2].LoadString(IDS_Minimum_thickness);
	ThickErr[3].LoadString(IDS_Maximum_thickness);

	Buff.Format(L"Thickness error");
	pPropCombo = new CUSLPropertyGridProperty(Buff, (_variant_t)ThickErr[theApp.m_LastSettings.nDefaultTOF], _T("What is displayed if either the start or finish criteria for TOF is not met"), ID_CSCAN_THICKNESS_ERROR);
	for (int ii = 0; ii < 4; ii++)
		pPropCombo->AddOption(ThickErr[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup->AddSubItem(pPropCombo);

	m_wndPropList.AddProperty(pGroup);
}

void CPropertiesPane::InitPropListNAD()
{
	CMFCPropertyGridProperty* pGroup;
	CMFCPropertyGridFileProperty* pPropFile;
	CUSLPropertyGridProperty* pPropCombo;
	CString Protocol[] = { L"SSH (22)", L"Telnet (23)" };
	CString AndroidCommsType[] = { L"WiFi", L"Bluetooth" };

	pGroup = new CMFCPropertyGridProperty(_T("Power PMAC"));
	pPropFile = new CMFCPropertyGridFileProperty(L"IP Address", theApp.m_Strings.GetAt(StringType::PPMAC_Address), ID_PPMAC_ADDRESS, L"This is generally the IP Address");
	pGroup->AddSubItem(pPropFile);
	pPropFile = new CMFCPropertyGridFileProperty(L"User name", theApp.m_Strings.GetAt(StringType::PPMAC_User), ID_PPMAC_USER_NAME, L"Unique identifier");
	pGroup->AddSubItem(pPropFile);
	pPropFile = new CMFCPropertyGridFileProperty(L"Password", theApp.m_Strings.GetAt(StringType::PPMAC_Password), ID_PPMAC_PASSWORD, L"Password");
	pGroup->AddSubItem(pPropFile);
	pPropCombo = new CUSLPropertyGridProperty(L"Protocol", (_variant_t)Protocol[theApp.m_FBCtrl.m_nProtocol], _T("Communication protocol"), ID_PPMAC_PROTOCOL);
	for (int ii = 0; ii < 2; ii++)
		pPropCombo->AddOption(Protocol[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup->AddSubItem(pPropCombo);
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Select device at startup", (_variant_t)theApp.m_FBCtrl.m_bEnableAtStartUp, _T("Enable communications at start of IDE"), ID_PPMAC_STARTUP));

	m_wndPropList.AddProperty(pGroup);
}

void CPropertiesPane::InitPropListMachineDescription()
{
	CMFCPropertyGridProperty* pGroup;
	CUSLPropertyGridProperty* pPropCombo;
	CUSLPropertyGridProperty* pPropSpin;
	CMFCPropertyGridFileProperty* pPropString;
	CString MachineType[30], Buff;
	CString Present[] = { L"-", L"Present" };
	int nIndex;
	int ii = 0;

	MachineType[ii++].LoadString(IDS_Dual_Sided_Squirter);
	MachineType[ii++].LoadString(IDS_Single_Bridge_Tank);
	MachineType[ii++].LoadString(IDS_Double_Bridge_Tank);
	MachineType[ii++].LoadString(IDS_Single_Bridge_Double_Sided);
	MachineType[ii++].LoadString(IDS_Dual_Bridge_Double_Sided);
	MachineType[ii++].LoadString(IDS_Dual_Sided_Squirter_Version_2);
	MachineType[ii++].LoadString(IDS_Dual_Squirter_less_9_axes);
	MachineType[ii++].LoadString(IDS_Single_Robot);
	MachineType[ii++].LoadString(IDS_Dual_Robot);
	MachineType[ii++].LoadString(IDS_Single_Bridge_Tank_With_Rot);
	MachineType[ii++].LoadString(IDS_Vertical_Bore_scanner);
	MachineType[ii++].LoadString(IDS_XYZ_Xt_Yt_R_Roller);
	MachineType[ii++].LoadString(IDS_9_plus_9_dual_robot);
	MachineType[ii++].LoadString(IDS_PPMAC_11_Axis);
	MachineType[ii++].LoadString(IDS_Twin_Tower);
	MachineType[ii++].LoadString(IDS_SPX_Robot);
	MachineType[ii++].LoadString(IDS_Railway_Wheels_Axle);
	MachineType[ii++].LoadString(IDS_Tracked_Probe);
	MachineType[ii++].LoadString(IDS_Tank_5_Axis);
	MachineType[ii++].Format(L"QuickStep Flat bed");
	MachineType[ii++].Format(L"Tank 6 axis");
	MachineType[ii++].Format(L"Twin tower kinematic");
	int nMaxMachineTypes = ii;


	pGroup = new CMFCPropertyGridProperty(LoadString(IDS_Machine_Description));
	pPropCombo = new CUSLPropertyGridProperty(LoadString(IDS_Type), (_variant_t)MachineType[theApp.m_Tank.nScannerDescription], _T("Choose the option which most acurately describes the machine"), ID_MACHINE_TYPE);
	for (int ii = 0; ii < nMaxMachineTypes; ii++)
		pPropCombo->AddOption(MachineType[ii]);
	pGroup->AddSubItem(pPropCombo);
	theApp.m_Tank.nMachineFeatures & _MACHINE_SIDE0 ? nIndex = 1 : nIndex = 0;
	pPropCombo = new CUSLPropertyGridProperty(L"Machine side 1", (_variant_t)Present[nIndex], L"", ID_MACHINE_SIDE0_PRESENT);
	for (int ii = 0; ii < 2; ii++) pPropCombo->AddOption(Present[ii]);
	pGroup->AddSubItem(pPropCombo);
	theApp.m_Tank.nMachineFeatures & _MACHINE_SIDE1 ? nIndex = 1 : nIndex = 0;
	pPropCombo = new CUSLPropertyGridProperty(L"Machine side 2", (_variant_t)Present[nIndex], L"", ID_MACHINE_SIDE1_PRESENT);
	for (int ii = 0; ii < 2; ii++) pPropCombo->AddOption(Present[ii]);
	pGroup->AddSubItem(pPropCombo);
	theApp.m_Tank.nMachineFeatures & _MACHINE_TURNTABLE ? nIndex = 1 : nIndex = 0;
	pPropCombo = new CUSLPropertyGridProperty(L"Turntable", (_variant_t)Present[nIndex], L"", ID_MACHINE_TURNTABLE_PRESENT);
	for (int ii = 0; ii < 2; ii++) pPropCombo->AddOption(Present[ii]);
	pGroup->AddSubItem(pPropCombo);
	theApp.m_Tank.nMachineFeatures & _MACHINE_ROLLER ? nIndex = 1 : nIndex = 0;
	pPropCombo = new CUSLPropertyGridProperty(L"Rollers", (_variant_t)Present[nIndex], L"", ID_MACHINE_ROLLER_PRESENT);
	for (int ii = 0; ii < 2; ii++) pPropCombo->AddOption(Present[ii]);
	pGroup->AddSubItem(pPropCombo);
	theApp.m_Tank.nMachineFeatures & _MACHINE_HOLDING ? nIndex = 1 : nIndex = 0;
	pPropCombo = new CUSLPropertyGridProperty(L"Holding fixture", (_variant_t)Present[nIndex], L"", ID_MACHINE_HOLDING_PRESENT);
	for (int ii = 0; ii < 2; ii++) pPropCombo->AddOption(Present[ii]);
	pGroup->AddSubItem(pPropCombo);
	theApp.m_Tank.nMachineFeatures & _MACHINE_SQUIRTER ? nIndex = 1 : nIndex = 0;
	pPropCombo = new CUSLPropertyGridProperty(L"Squirter system", (_variant_t)Present[nIndex], L"", ID_MACHINE_SQUIRTER_PRESENT);
	for (int ii = 0; ii < 2; ii++) pPropCombo->AddOption(Present[ii]);
	pGroup->AddSubItem(pPropCombo);
	theApp.m_Tank.nMachineFeatures & _MACHINE_TANK ? nIndex = 1 : nIndex = 0;
	pPropCombo = new CUSLPropertyGridProperty(L"Tank of water", (_variant_t)Present[nIndex], L"", ID_MACHINE_TANK_PRESENT);
	for (int ii = 0; ii < 2; ii++) pPropCombo->AddOption(Present[ii]);
	pGroup->AddSubItem(pPropCombo);


	m_wndPropList.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(L"Dimesions");
	pPropSpin = new CUSLPropertyGridProperty(_T("Length"), (_variant_t)(int)(0), _T("Specifies the scanners  (x) length"), ID_MACHINE_X_LENGTH, L"%.0f mm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Tank.fXLength, 0.0f, 30000.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(_T("Width"), (_variant_t)(int)(0), _T("Specifies the scanners  (y) width"), ID_MACHINE_Y_WIDTH, L"%.0f mm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Tank.fYLength, 0.0f, 30000.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(_T("Height"), (_variant_t)(int)(0), _T("Specifies the scanners  (z) height"), ID_MACHINE_Z_HEIGHT, L"%.0f mm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Tank.fZLength, 0.0f, 30000.0f, 0);
	pGroup->AddSubItem(pPropSpin);

	pPropSpin = new CUSLPropertyGridProperty(_T("X Zero offset"), (_variant_t)(int)(0), _T("Machine to zero X offset"), ID_MACHINE_ZERO_X_OFFSET, L"%.0f mm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Tank.fXSurfaceZero, 0.0f, 30000.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(_T("Y Zero offset"), (_variant_t)(int)(0), _T("Machine to zero Y offset"), ID_MACHINE_ZERO_Y_OFFSET, L"%.0f mm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Tank.fYSurfaceZero, 0.0f, 30000.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(_T("Z zero offset"), (_variant_t)(int)(0), _T("Machine to zero Z offset"), ID_MACHINE_ZERO_Z_OFFSET, L"%.0f mm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Tank.fZSurfaceZero, 0.0f, 30000.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	m_wndPropList.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(LoadString(IDS_Turntable));
	Buff.Format(L"X:%.01f Y:%.01f Z:%.01f", theApp.m_Tank.vTTCentre.x, theApp.m_Tank.vTTCentre.y, theApp.m_Tank.vTTCentre.z);
	pPropString = new CMFCPropertyGridFileProperty(LoadString(IDS_Centre), Buff, ID_TURNTABLE_CENTER, L"The centre of the turntable in surface coordinates");
	pGroup->AddSubItem(pPropString);
	Buff.Format(L"i:%.03f j:%.03f k:%.03f", theApp.m_Tank.vTTNorm.x, theApp.m_Tank.vTTNorm.y, theApp.m_Tank.vTTNorm.z);
	pPropString = new CMFCPropertyGridFileProperty(LoadString(IDS_Normal), Buff, ID_TURNTABLE_NORMAL, L"The turntable normal");
	pGroup->AddSubItem(pPropString);
	Buff.Format(L"i:%.03f j:%.03f k:%.03f", theApp.m_Tank.vTTZeroDegrees.x, theApp.m_Tank.vTTZeroDegrees.y, theApp.m_Tank.vTTZeroDegrees.z);
	pPropString = new CMFCPropertyGridFileProperty(LoadString(IDS_Zero_degrees), Buff, ID_TURNTABLE_ZERO_DEGREES, L"The turntable zero degrees");
	pGroup->AddSubItem(pPropString);
	m_wndPropList.AddProperty(pGroup);




	/*
	pGroup = new CMFCPropertyGridProperty(LoadString(IDS_Danger_plane));
	pPropSpin = new CUSLPropertyGridProperty(theApp.m_Strings[MachineSide0], (_variant_t)(int)(0), _T("Specifies the danger plane clearance on side 1 of the machine"), ID_DANGER_PLANE_0, L"%.0f mm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, g_LS.fDangerPlaneClearance[0], -300.0f, 300.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(theApp.m_Strings[MachineSide1], (_variant_t)(int)(0), _T("Specifies the danger plane clearance on side 2 of the machine"), ID_DANGER_PLANE_1, L"%.0f mm", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, g_LS.fDangerPlaneClearance[1], -300.0f, 300.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	m_wndPropList.AddProperty(pGroup);

	COLORREF rgbColour;
	pGroup = new CMFCPropertyGridProperty(LoadString(IDS_Colours));
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbDangerPlane[0]);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_Danger_plane) + L" " + theApp.m_Strings[ComponentSide0], (_variant_t)rgbColour, NULL, _T("Specifies the color for the danger plane on master side of component"), ID_DANGER_PLANE_COLOUR_0);
	pGroup->AddSubItem(pColor);
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbDangerPlane[1]);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_Danger_plane) + L" " + theApp.m_Strings[ComponentSide1], (_variant_t)rgbColour, NULL, _T("Specifies the color for the danger plane on slave side of component"), ID_DANGER_PLANE_COLOUR_1);
	pGroup->AddSubItem(pColor);
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbFloodFill[0]);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_Flood_fill) + L" " + theApp.m_Strings[ComponentSide0], (_variant_t)rgbColour, NULL, _T("Specifies the color for the flood fill on master side of component"), ID_FLOOD_FILL_COLOUR_0);
	pGroup->AddSubItem(pColor);
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbFloodFill[1]);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_Flood_fill) + L" " + theApp.m_Strings[ComponentSide1], (_variant_t)rgbColour, NULL, _T("Specifies the color for the flood fill on slave side of component"), ID_FLOOD_FILL_COLOUR_1);
	pGroup->AddSubItem(pColor);
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbCADShade);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_CAD_Shade), (_variant_t)rgbColour, NULL, _T("Specifies the color for the CAD surface shader"), ID_CAD_SHADE_COLOR);
	pGroup->AddSubItem(pColor);
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbCADWireFrame);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_CAD_Wire_frame), (_variant_t)rgbColour, NULL, _T("Specifies the color for the CAD wire frame"), ID_CAD_WIREFRAME_COLOR);
	pGroup->AddSubItem(pColor);
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbCADSurfaceSelect);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_CAD_Shade_highlighted), (_variant_t)rgbColour, NULL, _T("Specifies the color for the CAD surface shader when highlighted"), ID_CAD_SHADE_HIGHLIGHTED_COLOR);
	pGroup->AddSubItem(pColor);
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbCADWireFrameSelect);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_CAD_Wire_frame_highlighted), (_variant_t)rgbColour, NULL, _T("Specifies the color for the CAD wire frame when highlighted"), ID_CAD_WIREFRAME_HIGHLIGHTED_COLOR);
	pGroup->AddSubItem(pColor);
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbBackGroundWorld);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_Background_view), (_variant_t)rgbColour, NULL, _T("Specifies the color of the background when moving around"), ID_BACKGROUND_WORLD_COLOR);
	pGroup->AddSubItem(pColor);
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbBackGroundProfile);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_Background_profile), (_variant_t)rgbColour, NULL, _T("Specifies the color of the background when moving the profile"), ID_BACKGROUND_PROFILE_COLOR);
	pGroup->AddSubItem(pColor);
	rgbColour = XMVECTORF32_to_COLORREF(&g_LS.rgbSlice);
	pColor = new CMFCPropertyGridColorProperty(LoadString(IDS_Slice_plane), (_variant_t)rgbColour, NULL, _T("Specifies the color of the plane that slices the profile"), ID_SLICE_COLOR);
	pGroup->AddSubItem(pColor);
	
	m_wndPropList.AddProperty(pGroup);
	*/

}

void CPropertiesPane::InitPropListUtSubset()
{
	CMFCPropertyGridProperty* pGroup;
	bool bFlag;

	pGroup = new CMFCPropertyGridProperty(LoadString(IDS_Ut_subset));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PREAMP_GAIN ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Preamp Gain", (_variant_t)bFlag, NULL, ID_UT_PREAMP_GAIN));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PREAMP_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Preamp Mode", (_variant_t)bFlag, NULL, ID_UT_PREAMP_MODE));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAMPING ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Damping", (_variant_t)bFlag, NULL, ID_UT_DAMPING));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PULSE_WIDTH ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pulse width", (_variant_t)bFlag, NULL, ID_UT_PULSE_WIDTH));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PULSE_VOLTAGE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pulse voltage", (_variant_t)bFlag, NULL, ID_UT_PULSE_VOLTAGE));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PRF ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"PRF", (_variant_t)bFlag, NULL, ID_UT_PRF));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PULSER_OFF_ON ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pulser Off/On", (_variant_t)bFlag, NULL, ID_UT_PULSER_OFF_ON));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_OUTPUT_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Outout mode", (_variant_t)bFlag, NULL, ID_UT_OUTPUT_MODE));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_TX ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Transmitter", (_variant_t)bFlag, NULL, ID_UT_TX));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_RX ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Receiver", (_variant_t)bFlag, NULL, ID_UT_RX));

	theApp.m_LastSettings.n64SubsetUtVariable & _UT_LINEAR_GAIN ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Linear gain", (_variant_t)bFlag, NULL, ID_UT_LINEAR_GAIN));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_LOG_GAIN ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Log gain", (_variant_t)bFlag, NULL, ID_UT_LOG_GAIN));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_RECTIFY_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Rectifier mode", (_variant_t)bFlag, NULL, ID_UT_RECTIFY_MODE));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_FILTER_TYPE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Filter type", (_variant_t)bFlag, NULL, ID_UT_FILTER_TYPE));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_FILTER_RANGE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Filter range", (_variant_t)bFlag, NULL, ID_UT_FILTER_RANGE));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_FILTER_CENTER ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Filter frequncy", (_variant_t)bFlag, NULL, ID_UT_FILTER_CENTER));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_FILTER_CUT_OFF ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Filter cut off", (_variant_t)bFlag, NULL, ID_UT_FILTER_CUT_OFF));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAC_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"DAC mode", (_variant_t)bFlag, NULL, ID_UT_DAC_MODE));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAC_TRIGGER ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"DAC trigger mode", (_variant_t)bFlag, NULL, ID_UT_DAC_TRIGGER));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAC_DELAY ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"DAC Delay", (_variant_t)bFlag, NULL, ID_UT_DAC_DELAY));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAC_THRESHOLD ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"DAC trigger threshold", (_variant_t)bFlag, NULL, ID_UT_DAC_THRESHOLD));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAC_TABLE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"DAC table", (_variant_t)bFlag, NULL, ID_UT_DAC_TABLE));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_SCOPE_DELAY ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Scope delay", (_variant_t)bFlag, NULL, ID_UT_SCOPE_DELAY));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_SCOPE_WIDTH ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Scope width", (_variant_t)bFlag, NULL, ID_UT_SCOPE_WIDTH));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate mode", (_variant_t)bFlag, NULL, ID_UT_GATE_MODE));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_DELAY ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate delay", (_variant_t)bFlag, NULL, ID_UT_GATE_DELAY));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_WIDTH ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate width", (_variant_t)bFlag, NULL, ID_UT_GATE_WIDTH));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_THRESHOLD ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate threshold", (_variant_t)bFlag, NULL, ID_UT_GATE_THRESHOLD));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_FAR_DEAD_ZONE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate far dead zone", (_variant_t)bFlag, NULL, ID_UT_GATE_FAR_DEAD_ZONE));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_FAR_PEAK_WIDTH ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate peak width", (_variant_t)bFlag, NULL, ID_UT_GATE_FAR_PEAK_WIDTH));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_AMPLITUDE_COLLECT ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Amplitude collect", (_variant_t)bFlag, NULL, ID_UT_AMPLITUDE_COLLECT));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_TOF_COLLECT ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"TOF collect", (_variant_t)bFlag, NULL, ID_UT_TOF_COLLECT));
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_TOF_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"TOF mode", (_variant_t)bFlag, NULL, ID_UT_TOF_MODE));

	m_wndPropList.AddProperty(pGroup);

}

void CPropertiesPane::InitPropListUtCopy()
{
	CMFCPropertyGridProperty* pGroup;
	bool bFlag;

	pGroup = new CMFCPropertyGridProperty(LoadString(IDS_Ut_copy));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_PREAMP_GAIN ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Preamp Gain", (_variant_t)bFlag, NULL, ID_COPY_UT_PREAMP_GAIN));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_PREAMP_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Preamp Mode", (_variant_t)bFlag, NULL, ID_COPY_UT_PREAMP_MODE));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAMPING ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Damping", (_variant_t)bFlag, NULL, ID_COPY_UT_DAMPING));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_PULSE_WIDTH ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pulse width", (_variant_t)bFlag, NULL, ID_COPY_UT_PULSE_WIDTH));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_PULSE_VOLTAGE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pulse voltage", (_variant_t)bFlag, NULL, ID_COPY_UT_PULSE_VOLTAGE));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_PRF ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"PRF", (_variant_t)bFlag, NULL, ID_COPY_UT_PRF));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_PULSER_OFF_ON ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Pulser Off/On", (_variant_t)bFlag, NULL, ID_COPY_UT_PULSER_OFF_ON));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_OUTPUT_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Outout mode", (_variant_t)bFlag, NULL, ID_COPY_UT_OUTPUT_MODE));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_TX ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Transmitter", (_variant_t)bFlag, NULL, ID_COPY_UT_TX));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_RX ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Receiver", (_variant_t)bFlag, NULL, ID_COPY_UT_RX));

	theApp.m_LastSettings.n64CopyUtVariable & _UT_LINEAR_GAIN ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Linear gain", (_variant_t)bFlag, NULL, ID_COPY_UT_LINEAR_GAIN));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_LOG_GAIN ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Log gain", (_variant_t)bFlag, NULL, ID_COPY_UT_LOG_GAIN));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_RECTIFY_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Rectifier mode", (_variant_t)bFlag, NULL, ID_COPY_UT_RECTIFY_MODE));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_TYPE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Filter type", (_variant_t)bFlag, NULL, ID_COPY_UT_FILTER_TYPE));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_RANGE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Filter range", (_variant_t)bFlag, NULL, ID_COPY_UT_FILTER_RANGE));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_CENTER ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Filter frequncy", (_variant_t)bFlag, NULL, ID_COPY_UT_FILTER_CENTER));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_CUT_OFF ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Filter cut off", (_variant_t)bFlag, NULL, ID_COPY_UT_FILTER_CUT_OFF));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAC_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"DAC mode", (_variant_t)bFlag, NULL, ID_COPY_UT_DAC_MODE));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAC_TRIGGER ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"DAC trigger mode", (_variant_t)bFlag, NULL, ID_COPY_UT_DAC_TRIGGER));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAC_DELAY ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"DAC Delay", (_variant_t)bFlag, NULL, ID_COPY_UT_DAC_DELAY));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAC_THRESHOLD ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"DAC trigger threshold", (_variant_t)bFlag, NULL, ID_COPY_UT_DAC_THRESHOLD));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAC_TABLE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"DAC table", (_variant_t)bFlag, NULL, ID_COPY_UT_DAC_TABLE));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_SCOPE_DELAY ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Scope delay", (_variant_t)bFlag, NULL, ID_COPY_UT_SCOPE_DELAY));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_SCOPE_WIDTH ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Scope width", (_variant_t)bFlag, NULL, ID_COPY_UT_SCOPE_WIDTH));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate mode", (_variant_t)bFlag, NULL, ID_COPY_UT_GATE_MODE));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_DELAY ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate delay", (_variant_t)bFlag, NULL, ID_COPY_UT_GATE_DELAY));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_WIDTH ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate width", (_variant_t)bFlag, NULL, ID_COPY_UT_GATE_WIDTH));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_THRESHOLD ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate threshold", (_variant_t)bFlag, NULL, ID_COPY_UT_GATE_THRESHOLD));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_FAR_DEAD_ZONE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate far dead zone", (_variant_t)bFlag, NULL, ID_COPY_UT_GATE_FAR_DEAD_ZONE));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_FAR_PEAK_WIDTH ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Gate peak width", (_variant_t)bFlag, NULL, ID_COPY_UT_GATE_FAR_PEAK_WIDTH));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_AMPLITUDE_COLLECT ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"Amplitude collect", (_variant_t)bFlag, NULL, ID_COPY_UT_AMPLITUDE_COLLECT));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_TOF_COLLECT ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"TOF collect", (_variant_t)bFlag, NULL, ID_COPY_UT_TOF_COLLECT));
	theApp.m_LastSettings.n64CopyUtVariable & _UT_TOF_MODE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CMFCPropertyGridProperty(L"TOF mode", (_variant_t)bFlag, NULL, ID_COPY_UT_TOF_MODE));

	m_wndPropList.AddProperty(pGroup);

}

void CPropertiesPane::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	CMenu* menu = (CMenu *) new CMenu;
	CMenu *pPopup;
	CPoint pt;
	GetCursorPos(&pt);

	switch (m_nWhichList) {
	case Application:
		break;
	case Kinematics:
		break;
	case Scope:
		break;
	case Tabs:
		break;
	case Toolbars:
		break;
	case CScan:
		break;
	case Profile:
		break;
	case NetworkAttachedDevices:
		break;
	case Machine:
		break;
	case UtSubset:
		if (menu->LoadMenu(IDR_RB_UT_SUBSET)) {
			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
		}
		break;
	}
	delete menu;

}

void CPropertiesPane::OnRclickPropertyList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	*pResult = 0;
}

void CPropertiesPane::OnUtSubsetFillall()
{
	FRAME;
	theApp.m_LastSettings.n64SubsetUtVariable = 0xffffffffffff;
	pFrame->SendMessage(UI_UPDATE_ULTRASONICS_SHEET);
	InitPropList(m_nWhichList);
}

void CPropertiesPane::OnUtSubsetClearall()
{
	FRAME;
	theApp.m_LastSettings.n64SubsetUtVariable = 0;
	pFrame->SendMessage(UI_UPDATE_ULTRASONICS_SHEET);
	InitPropList(m_nWhichList);
}

LRESULT CPropertiesPane::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMainFrame*  pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMFCPropertyGridProperty * pProperty = (CMFCPropertyGridProperty *)lParam;
	CMFCPropertyGridColorProperty* pPropertyColour = (CMFCPropertyGridColorProperty *)lParam;
	CUSLPropertyGridProperty * pUSLProperty = (CUSLPropertyGridProperty *)lParam;
	CMFCPropertyGridFontProperty* pFontProperty = (CMFCPropertyGridFontProperty *)lParam;
	CMFCPropertyGridFileProperty* pFileProperty = (CMFCPropertyGridFileProperty *)lParam;
	_variant_t value;
	CString Buff, strBasePane;
	COLORREF rgb;
	long nValue;
	int nID, nGate, nIndex;
	int nMotoionToolsTabMask[2];

	CopyMemory((PVOID)nMotoionToolsTabMask, (PVOID)theApp.m_LastSettings.nMotoionToolsTabMask, sizeof theApp.m_LastSettings.nMotoionToolsTabMask);
	int nUltrasonicsTabMask = theApp.m_LastSettings.nUltrasonicsTabMask;
	int nPaletteTabMask = theApp.m_LastSettings.nPaletteTabMask;
	__int64 n64SubsetUtVariable = theApp.m_LastSettings.n64SubsetUtVariable;

	bool bFlag = pProperty->GetValue().boolVal;
	int nSpin = (int)pUSLProperty->GetFloatSpinValue();


	switch (nID = pProperty->GetData()) {
	case ID_SIDE_1: theApp.m_nSide0Orientation = pUSLProperty->GetCurSel();
		break;
	case ID_UNITS: theApp.m_LastSettings.nMeasurementUnits = pUSLProperty->GetCurSel();
		theApp.ChangeGlobalUnits();
		break;
	case ID_USE_SYSTEM_FONT: theApp.m_LastSettings.bUseAlernativeToSystemFont = (bool)pProperty->GetValue().boolVal;
		break;
	case ID_MENU_FONT: CopyMemory(&theApp.m_LastSettings.lfMenus, pFontProperty->GetLogFont(), sizeof LOGFONT);
		break;
	case ID_BORDER:	Buff.Format(L"%s", pProperty->GetValue().bstrVal);
		nValue = pUSLProperty->GetCurSel();
		break;
	case ID_COLOUR:	rgb = pPropertyColour->GetColor();
		break;
	case ID_LOAD_WINDOS_ON_LAST_EXIT:	theApp.m_LastSettings.bWindowPos = bFlag;
		break;
	case ID_HEIGHT:	nValue = pProperty->GetValue().lVal;
		break;
	case ID_MACHINE_SIDE_NAME_0:  theApp.m_DlgSideName[0] = pFileProperty->GetValue().bstrVal;
		break;
	case ID_MACHINE_SIDE_NAME_1:  theApp.m_DlgSideName[1] = pFileProperty->GetValue().bstrVal;
		break;
	case ID_MACHINE_SIDE_NAME_2:  theApp.m_DlgSideName[2] = pFileProperty->GetValue().bstrVal;
		break;
	case ID_MACHINE_REF_NAME_0:  theApp.m_3PtName[0] = pFileProperty->GetValue().bstrVal;
		break;
	case ID_MACHINE_REF_NAME_1:  theApp.m_3PtName[1] = pFileProperty->GetValue().bstrVal;
		break;
	case ID_MACHINE_REF_NAME_2:  theApp.m_3PtName[2] = pFileProperty->GetValue().bstrVal;
		break;
	case ID_POSITION_MENU_FONT: CopyMemory(&theApp.m_LastSettings.lfPositionPane, pFontProperty->GetLogFont(), sizeof LOGFONT);
		break;
	case ID_POSITION_TEXT_COLOUR: theApp.m_LastSettings.rgbTextPositionPane = pPropertyColour->GetColor();
		break;
	case ID_POSITION_BK_COLOUR: theApp.m_LastSettings.rgbBKPositionPane = pPropertyColour->GetColor();
		break;
	case ID_POSITION_SIDE_0: bFlag == true ? theApp.m_LastSettings.nPositionPaneMask |= POSITION_SIDE_0 : theApp.m_LastSettings.nPositionPaneMask &= ~POSITION_SIDE_0;
		break;
	case ID_POSITION_SIDE_1: bFlag == true ? theApp.m_LastSettings.nPositionPaneMask |= POSITION_SIDE_1 : theApp.m_LastSettings.nPositionPaneMask &= ~POSITION_SIDE_1;
		break;
	case ID_POSITION_DISPLAY_NORM: bFlag == true ? theApp.m_LastSettings.nPositionPaneMask |= POSITION_DISPLAY_NORM : theApp.m_LastSettings.nPositionPaneMask &= ~POSITION_DISPLAY_NORM;
		break;
	case ID_POSITION_DISPLAY_A: bFlag == true ? theApp.m_LastSettings.nPositionPaneMask |= POSITION_DISPLAY_A : theApp.m_LastSettings.nPositionPaneMask &= ~POSITION_DISPLAY_A;
		break;
	case ID_POSITION_DISPLAY_B: bFlag == true ? theApp.m_LastSettings.nPositionPaneMask |= POSITION_DISPLAY_B : theApp.m_LastSettings.nPositionPaneMask &= ~POSITION_DISPLAY_B;
		break;
	case ID_POSITION_DISPLAY_ROT_NORM: bFlag == true ? theApp.m_LastSettings.nPositionPaneMask |= POSITION_DISPLAY_ROT_NORM : theApp.m_LastSettings.nPositionPaneMask &= ~POSITION_DISPLAY_ROT_NORM;
		break;
	case ID_POSITION_DISPLAY_QUAT: bFlag == true ? theApp.m_LastSettings.nPositionPaneMask |= POSITION_DISPLAY_QUAT : theApp.m_LastSettings.nPositionPaneMask &= ~POSITION_DISPLAY_QUAT;
		break;
	case ID_POSITION_DISPLAY_R0: bFlag == true ? theApp.m_LastSettings.nPositionPaneMask |= POSITION_DISPLAY_R0 : theApp.m_LastSettings.nPositionPaneMask &= ~POSITION_DISPLAY_R0;
		break;
	case ID_POSITION_DISPLAY_R1: bFlag == true ? theApp.m_LastSettings.nPositionPaneMask |= POSITION_DISPLAY_R1 : theApp.m_LastSettings.nPositionPaneMask &= ~POSITION_DISPLAY_R1;
		break;
	case ID_TAUGHT_LINES_COLOUR: theApp.m_LastSettings.rgbDisplayTaughtColor = pPropertyColour->GetColor();
		break;
	case ID_SCAN_LINES_COLOUR: theApp.m_LastSettings.rgbDisplayScanColor = pPropertyColour->GetColor();
		break;
	case ID_BRANE_LINES_COLOUR: theApp.m_LastSettings.rgbDisplayBrainColor = pPropertyColour->GetColor();
		break;
	case ID_ENVELOPE_LINES_COLOUR: theApp.m_LastSettings.rgbDisplayEnvelopeColor = pPropertyColour->GetColor();
		break;
	case ID_INSIDE_LINES_COLOUR: theApp.m_LastSettings.rgbBladeColor[INSIDE] = pPropertyColour->GetColor();
		break;
	case ID_OUTSIDE_LINES_COLOUR: theApp.m_LastSettings.rgbBladeColor[OUTSIDE] = pPropertyColour->GetColor();
		break;
	case ID_THICKNESS_CALIBRATION_PERIOD: theApp.m_LastSettings.osThicknessReCalibrationPeriodMinutes = nSpin;
		break;


		/////////////////////////////////////////////////////////////////
		// Kinematic 
		/////////////////////////////////////////////////////////////////
	case ID_REMOVE_AIR_0: theApp.m_LastSettings.fRemoveAirAngle[PORTSIDE] = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_REMOVE_AIR_1: theApp.m_LastSettings.fRemoveAirAngle[STARBOARD] = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_AIR_DWELL_TIME: theApp.m_LastSettings.nRemoveAirTime = nSpin;
		break;
	case ID_DELTA_SCAN_SPEED: theApp.m_LastSettings.fDeltaScanSpeed = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_DELTA_SCAN_INTERVAL: theApp.m_LastSettings.fDeltaScanIncrement = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_MACHINE_MOTION_CONTROLLER: theApp.m_nMotorType = pUSLProperty->GetCurSel();
		break;
		////////////////////////////////////////////////////////////////
		// Profile
		//////////////////////////////////////////////////////////////
	case ID_PROFILE_NAME: PROFILE->m_strDescription = pFileProperty->GetValue().bstrVal;
		break;
	case ID_PROFILE_DESCRIPTION: PROFILE->m_nComponentDescription = (ComponentDescription)pUSLProperty->GetCurSel();
		break;
	case ID_PROFILE_FAST_INCREMENT: PROFILE->m_fFastIncrement = pUSLProperty->GetFloatSpinValue() / theApp.m_fUnits;
		break;
	case ID_PROFILE_SLOW_INCREMENT: PROFILE->m_fSlowIncrement = pUSLProperty->GetFloatSpinValue() / theApp.m_fUnits;
		break;
	case ID_PROFILE_ULTRASONICS: PROFILE->m_UltrasonicPathFileName = pFileProperty->GetValue().bstrVal;
		break;
	case ID_DANGER_PLANE_MARGIN_0: theApp.m_LastSettings.fDangerMargin[0] = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_DANGER_PLANE_MARGIN_1: theApp.m_LastSettings.fDangerMargin[1] = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_DANGER_PLANE_DIR_0:
		ZeroMemory(&PROFILE->m_vecToDangerPlane[0], sizeof PROFILE->m_vecToDangerPlane[0]);
		if ((int)pUSLProperty->GetCurSel() == 0) PROFILE->m_vecToDangerPlane[0].x = 1.0f;
		if ((int)pUSLProperty->GetCurSel() == 1) PROFILE->m_vecToDangerPlane[0].y = 1.0f;
		if ((int)pUSLProperty->GetCurSel() == 2) PROFILE->m_vecToDangerPlane[0].z = 1.0f;
		break;
	case ID_DANGER_PLANE_DIR_1:
		ZeroMemory(&PROFILE->m_vecToDangerPlane[1], sizeof PROFILE->m_vecToDangerPlane[1]);
		if ((int)pUSLProperty->GetCurSel() == 0) PROFILE->m_vecToDangerPlane[1].x = 1.0f;
		if ((int)pUSLProperty->GetCurSel() == 1) PROFILE->m_vecToDangerPlane[1].y = 1.0f;
		if ((int)pUSLProperty->GetCurSel() == 2) PROFILE->m_vecToDangerPlane[1].z = 1.0f;
		break;

		///////////////////////////////////////////////////////////////////////
		// Scope
		///////////////////////////////////////////////////////////////////////
	case ID_GATE_COLOUR + 0:
	case ID_GATE_COLOUR + 1:
	case ID_GATE_COLOUR + 2:
	case ID_GATE_COLOUR + 3:
	case ID_GATE_COLOUR + 4:
	case ID_GATE_COLOUR + 5:
	case ID_GATE_COLOUR + 6:
	case ID_GATE_COLOUR + 7:
		nGate = nID - (ID_GATE_COLOUR);
		theApp.m_Scope.m_rgbGateColor[nGate] = pPropertyColour->GetColor();
		break;
	case ID_TRACE_COLOUR: theApp.m_Scope.m_rgbTraceColor = pPropertyColour->GetColor();
		break;
	case ID_SCOPE_BK_COLOUR: theApp.m_Scope.m_rgbBkColor = pPropertyColour->GetColor();
		break;
	case ID_SCOPE_GRATICULE_COLOUR: theApp.m_Scope.m_rgbGraticuleColor = pPropertyColour->GetColor();
		break;
	case ID_SCOPE_DAC_COLOUR: theApp.m_Scope.m_rgbDACColor = pPropertyColour->GetColor();
		break;
	case ID_SCOPE_TEXT_COLOUR: theApp.m_Scope.m_rgbTextColor = pPropertyColour->GetColor();
		break;
	case ID_SCOPE_FONT: CopyMemory(&theApp.m_LastSettings.lfScope, pFontProperty->GetLogFont(), sizeof LOGFONT);
		theApp.m_Scope.m_rgbTextColor = pFontProperty->GetColor();
		break;
	case ID_GATE_DELAY_UNITS: theApp.m_Scope.m_nGateDelayUnits = (int)pUSLProperty->GetCurSel();
		pFrame->SendMessage(UI_UPDATE_GATES_UT_PAGE);
		break;
	case ID_GATE_WIDTH_UNITS: theApp.m_Scope.m_nGateWidthUnits = (int)pUSLProperty->GetCurSel();
		pFrame->SendMessage(UI_UPDATE_GATES_UT_PAGE);
		break;
	case ID_TRACE_DELAY_UNITS: theApp.m_Scope.m_nTraceDelayUnits = (int)pUSLProperty->GetCurSel();
		break;
	case ID_TRACE_WIDTH_UNITS: theApp.m_Scope.m_nTraceWidthUnits = (int)pUSLProperty->GetCurSel();
		break;
	case ID_DAC_BLANKING_UNITS: theApp.m_LastSettings.nDacBlankingUnits = (int)pUSLProperty->GetCurSel();
		break;
	case ID_DAC_DELAY_UNITS: theApp.m_LastSettings.nDacDelayUnits = (int)pUSLProperty->GetCurSel();
		break;
	case ID_TOF_UNITS: theApp.m_Scope.m_nTOFText[1] = (int)pUSLProperty->GetCurSel();
		break;
	case ID_DISPLAY_GRATICULE: bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_GRATICULE : theApp.m_LastSettings.nScopeMask &= ~SCOPE_GRATICULE;
		break;
	case ID_LEFT_SCALE: bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_LEFT_SCALE : theApp.m_LastSettings.nScopeMask &= ~SCOPE_LEFT_SCALE;
		break;
	case ID_RIGHT_SCALE:  bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_RIGHT_SCALE : theApp.m_LastSettings.nScopeMask &= ~SCOPE_RIGHT_SCALE;
		break;
	case ID_SCOPE_FONT_HEIGHT: theApp.m_Scope.m_nTextSize = nSpin;
		break;
	case ID_RENDER_WATER_PATH: bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_RENDER_WATER_PATH : theApp.m_LastSettings.nScopeMask &= ~SCOPE_RENDER_WATER_PATH;
		break;
	case ID_RENDER_MATERIAL_PATH: bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_RENDER_MATERIAL_PATH : theApp.m_LastSettings.nScopeMask &= ~SCOPE_RENDER_MATERIAL_PATH;
		break;
	case ID_RENDER_COLLECT_TEXT: bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_RENDER_COLLECT_TEXT : theApp.m_LastSettings.nScopeMask &= ~SCOPE_RENDER_COLLECT_TEXT;
		break;
	case ID_CONVERSION_RATE:  bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_RENDER_CONVERSION_RATE : theApp.m_LastSettings.nScopeMask &= ~SCOPE_RENDER_CONVERSION_RATE;
		break;
	case ID_SAMPLING_RATE:  bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_RENDER_SAMPLE_RATE : theApp.m_LastSettings.nScopeMask &= ~SCOPE_RENDER_SAMPLE_RATE;
		break;
	case ID_NUMBER_SAMPLES_PER_TRACE:  bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_SAMPLES_PER_TRACE : theApp.m_LastSettings.nScopeMask &= ~SCOPE_SAMPLES_PER_TRACE;
		break;
	case ID_SCOPE_RENDER_TIME_ADC:  bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_TIME_TO_READ_ADC : theApp.m_LastSettings.nScopeMask &= ~SCOPE_TIME_TO_READ_ADC;
		break;
	case ID_SCOPE_RENDER_TIME_ASCAN:  bFlag == true ? theApp.m_LastSettings.nScopeMask |= SCOPE_TIME_PROCESS_ASCAN : theApp.m_LastSettings.nScopeMask &= ~SCOPE_TIME_PROCESS_ASCAN;
		break;
		//////////////////////////////////////////////////////////////
		// Tabs
		///////////////////////////////////////////////////////////////
	case ID_PR20_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= PR20_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~PR20_TAB;
		break;
	case ID_PR50_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= PR50_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~PR50_TAB;
		break;
	case ID_PR30_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= PR30_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~PR30_TAB;
		break;
	case ID_PR40_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= PR40_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~PR40_TAB;
		break;
	case ID_SCOPE_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= SCOPE_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~SCOPE_TAB;
		break;
	case ID_GLOBAL_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= GLOBAL_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~GLOBAL_TAB;
		break;
	case ID_TIMESLOTS_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= TIMESLOTS_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~TIMESLOTS_TAB;
		break;
	case ID_GATES_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= GATES_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~GATES_TAB;
		break;
	case ID_SELF_TEST_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= SELF_TEST_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~SELF_TEST_TAB;
		break;
	case ID_EXT_SOURCE_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= EXT_SOURCE_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~EXT_SOURCE_TAB;
		break;
	case ID_TRAFFIC_LIGHTS_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= TRAFFIC_LIGHTS_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~TRAFFIC_LIGHTS_TAB;
		break;
	case ID_PHASED_ARRAY_TAB: bFlag == true ? theApp.m_LastSettings.nUltrasonicsTabMask |= PHASED_ARRAY_TAB : theApp.m_LastSettings.nUltrasonicsTabMask &= ~PHASED_ARRAY_TAB;
		break;
	case ID_Rectilinear_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Rectilinear_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Rectilinear_TAB;
		break;
	case ID_Turntable_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Turntable_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Turntable_TAB;
		break;
	case ID_Rollers_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Rollers_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Rollers_TAB;
		break;
	case ID_Contour_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Contour_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Contour_TAB;
		break;
	case ID_Tubes_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Tubes_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Tubes_TAB;
		break;
	case ID_Disc_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Disc_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Disc_TAB;
		break;
	case ID_Tip_Axes_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Tip_Axes_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Tip_Axes_TAB;
		break;
	case ID_Optimize_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Optimize_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Optimize_TAB;
		break;
	case ID_Pump_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Pump_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Pump_TAB;
		break;
	case ID_Manipulator_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Manipulator_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Manipulator_TAB;
		break;
	case ID_Origin_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Origin_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Origin_TAB;
		break;
	case ID_Move_To_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Move_To_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Move_To_TAB;
		break;
	case ID_Jog_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Jog_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Jog_TAB;
		break;
	case ID_TOF_CAL_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= TOF_CAL_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~TOF_CAL_TAB;
		break;
	case ID_TRANSPLY_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= TRANSPLY_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~TRANSPLY_TAB;
		break;
	case ID_DISCS_SHAFTS_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= DISCS_SHAFTS_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~DISCS_SHAFTS_TAB;
		break;
	case ID_Robot_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Robots_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Robots_TAB;
		break;
	case ID_PHASEDARRAY_LINE_SCAN_TAB:  bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= PHASEDARRAY_LINE_SCAN_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~PHASEDARRAY_LINE_SCAN_TAB;
		break;
	case ID_Robot_Position_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Robots_Position_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Robots_Position_TAB;
		break;
	case ID_Scan_Lines_TAB: bFlag == true ? theApp.m_LastSettings.nMotoionToolsTabMask[0] |= Scan_Lines_TAB : theApp.m_LastSettings.nMotoionToolsTabMask[0] &= ~Scan_Lines_TAB;
		break;
	case ID_Amplitude_Palette_TAB: bFlag == true ? theApp.m_LastSettings.nPaletteTabMask |= Amplitude_Palette_TAB : theApp.m_LastSettings.nPaletteTabMask &= ~Amplitude_Palette_TAB;
		break;
	case ID_Thickness_Palette_TAB: bFlag == true ? theApp.m_LastSettings.nPaletteTabMask |= Thickness_Palette_TAB : theApp.m_LastSettings.nPaletteTabMask &= ~Thickness_Palette_TAB;
		break;
	case ID_Porosity_Palette_TAB: bFlag == true ? theApp.m_LastSettings.nPaletteTabMask |= Porosity_Palette_TAB : theApp.m_LastSettings.nPaletteTabMask &= ~Porosity_Palette_TAB;
		break;
	case ID_ProfileCartesianTab: bFlag == true ? theApp.m_LastSettings.nProfileTabMask |= ProfileCartesianTab : theApp.m_LastSettings.nProfileTabMask &= ~ProfileCartesianTab;
		break;
	case ID_ProfileTurTableTab: bFlag == true ? theApp.m_LastSettings.nProfileTabMask |= ProfileTurTableTab : theApp.m_LastSettings.nProfileTabMask &= ~ProfileTurTableTab;
		break;
	case ID_Profile3DDesignTab: bFlag == true ? theApp.m_LastSettings.nProfileTabMask |= Profile3DDesignTab : theApp.m_LastSettings.nProfileTabMask &= ~Profile3DDesignTab;
		break;
	case ID_Profile3DViewTab: bFlag == true ? theApp.m_LastSettings.nProfileTabMask |= Profile3DViewTab : theApp.m_LastSettings.nProfileTabMask &= ~Profile3DViewTab;
		break;
	case ID_Profile3DStiffenerTab: bFlag == true ? theApp.m_LastSettings.nProfileTabMask |= Profile3DStiffenerTab : theApp.m_LastSettings.nProfileTabMask &= ~Profile3DStiffenerTab;
		break;
	case ID_Profile3DBladeTab: bFlag == true ? theApp.m_LastSettings.nProfileTabMask |= Profile3DBladeTab : theApp.m_LastSettings.nProfileTabMask &= ~Profile3DBladeTab;
		break;
	case ID_PositionPositionTab: bFlag == true ? theApp.m_LastSettings.nPositionTabMask |= PositionPositionTab : theApp.m_LastSettings.nProfileTabMask &= ~PositionPositionTab;
		break;
	case ID_PositionSingleBridgeTab: bFlag == true ? theApp.m_LastSettings.nPositionTabMask |= PositionSingleBridgeTab : theApp.m_LastSettings.nProfileTabMask &= ~PositionSingleBridgeTab;
		break;
	case ID_PositionRobotDualTab: bFlag == true ? theApp.m_LastSettings.nPositionTabMask |= PositionRobotDualTab : theApp.m_LastSettings.nProfileTabMask &= ~PositionRobotDualTab;
		break;
	case ID_PositionGraphicalTab: bFlag == true ? theApp.m_LastSettings.nPositionTabMask |= PositionGraphicalTab : theApp.m_LastSettings.nProfileTabMask &= ~PositionGraphicalTab;
		break;
	case ID_PositionSwivGimTab: bFlag == true ? theApp.m_LastSettings.nPositionTabMask |= PositionSwivGimTab : theApp.m_LastSettings.nProfileTabMask &= ~PositionSwivGimTab;
		break;

		/////////////////////////////////////////////////////////////
		// C-Scan
		/////////////////////////////////////////////////////////////
	case ID_CSCAN_THICKNESS_ERROR: theApp.m_LastSettings.nDefaultTOF = (int)pUSLProperty->GetCurSel();
		break;

		/////////////////////////////////////////////////////////////
		// Network attached
		////////////////////////////////////////////////////////////
	case ID_PPMAC_ADDRESS: theApp.m_Strings.SetAt(StringType::PPMAC_Address, pFileProperty->GetValue().bstrVal);
		break;
	case ID_PPMAC_USER_NAME: theApp.m_Strings.SetAt(StringType::PPMAC_User, pFileProperty->GetValue().bstrVal);
		break;
	case ID_PPMAC_PASSWORD: theApp.m_Strings.SetAt(StringType::PPMAC_Password, pFileProperty->GetValue().bstrVal);
		break;
	case ID_PPMAC_STARTUP: theApp.m_FBCtrl.m_bEnableAtStartUp = pProperty->GetValue().boolVal;
		break;
	case ID_PPMAC_PROTOCOL: theApp.m_FBCtrl.m_nProtocol = pUSLProperty->GetCurSel();
		break;
		/////////////////////////////////////////////////////////////
		// Machine description
		////////////////////////////////////////////////////////////
	case ID_MACHINE_TYPE: theApp.m_Tank.nScannerDescription = (ScannerDescription)pUSLProperty->GetCurSel();
		break;
	case ID_MACHINE_SIDE0_PRESENT:
		pUSLProperty->GetCurSel() == 1 ? theApp.m_Tank.nMachineFeatures |= _MACHINE_SIDE0 : theApp.m_Tank.nMachineFeatures &= ~_MACHINE_SIDE0;
		break;
	case ID_MACHINE_SIDE1_PRESENT:
		pUSLProperty->GetCurSel() == 1 ? theApp.m_Tank.nMachineFeatures |= _MACHINE_SIDE1 : theApp.m_Tank.nMachineFeatures &= ~_MACHINE_SIDE1;
		break;
	case ID_MACHINE_TURNTABLE_PRESENT:
		pUSLProperty->GetCurSel() == 1 ? theApp.m_Tank.nMachineFeatures |= _MACHINE_TURNTABLE : theApp.m_Tank.nMachineFeatures &= ~_MACHINE_TURNTABLE;
		break;
	case ID_MACHINE_ROLLER_PRESENT:
		pUSLProperty->GetCurSel() == 1 ? theApp.m_Tank.nMachineFeatures |= _MACHINE_ROLLER : theApp.m_Tank.nMachineFeatures &= ~_MACHINE_ROLLER;
		break;
	case ID_MACHINE_HOLDING_PRESENT:
		pUSLProperty->GetCurSel() == 1 ? theApp.m_Tank.nMachineFeatures |= _MACHINE_HOLDING : theApp.m_Tank.nMachineFeatures &= ~_MACHINE_HOLDING;
		break;
	case ID_MACHINE_SQUIRTER_PRESENT:
		pUSLProperty->GetCurSel() == 1 ? theApp.m_Tank.nMachineFeatures |= _MACHINE_SQUIRTER : theApp.m_Tank.nMachineFeatures &= ~_MACHINE_SQUIRTER;
		break;
	case ID_MACHINE_TANK_PRESENT:
		pUSLProperty->GetCurSel() == 1 ? theApp.m_Tank.nMachineFeatures |= _MACHINE_TANK : theApp.m_Tank.nMachineFeatures &= ~_MACHINE_TANK;
		break;


	case ID_MACHINE_X_LENGTH: theApp.m_Tank.fXLength = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_MACHINE_Y_WIDTH: theApp.m_Tank.fYLength = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_MACHINE_Z_HEIGHT: theApp.m_Tank.fZLength = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_MACHINE_ZERO_X_OFFSET: theApp.m_Tank.fXSurfaceZero = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_MACHINE_ZERO_Y_OFFSET: theApp.m_Tank.fYSurfaceZero = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_MACHINE_ZERO_Z_OFFSET: theApp.m_Tank.fZSurfaceZero = pUSLProperty->GetFloatSpinValue();
		break;

	case ID_TOOLBAR_MAIN + 0:
	case ID_TOOLBAR_MAIN + 1:
	case ID_TOOLBAR_MAIN + 2:
	case ID_TOOLBAR_MAIN + 3:
	case ID_TOOLBAR_MAIN + 4:
	case ID_TOOLBAR_MAIN + 5:
	case ID_TOOLBAR_MAIN + 6:
	case ID_TOOLBAR_MAIN + 7:
	case ID_TOOLBAR_MAIN + 8:
	case ID_TOOLBAR_MAIN + 9:
	case ID_TOOLBAR_MAIN + 10:
	case ID_TOOLBAR_MAIN + 11:
	case ID_TOOLBAR_MAIN + 12:
	case ID_TOOLBAR_MAIN + 13:
	case ID_TOOLBAR_MAIN + 14:
	case ID_TOOLBAR_MAIN + 15:
		nIndex = nID - (ID_TOOLBAR_MAIN);
		strBasePane.Format(L"BasePane-%d", m_nToolBarID[nIndex]);
		if (bFlag == true) {
			theApp.WriteSectionInt(strBasePane, L"IsVisible", 1);
		}
		else {
			theApp.WriteSectionInt(strBasePane, L"IsVisible", 0);
		}

		pFrame->SendMessage(UI_INVALIDATE_TOOLBARS, NULL, NULL);
		break;

		//UT Subset
	case ID_UT_PREAMP_GAIN: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PREAMP_GAIN : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_PREAMP_GAIN;
		break;
	case ID_UT_PREAMP_MODE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PREAMP_MODE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_PREAMP_MODE;
		break;
	case ID_UT_DAMPING: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_DAMPING : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_DAMPING;
		break;
	case ID_UT_PULSE_WIDTH: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PULSE_WIDTH : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_PULSE_WIDTH;
		break;
	case ID_UT_PULSE_VOLTAGE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PULSE_VOLTAGE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_PULSE_VOLTAGE;
		break;
	case ID_UT_PRF: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PRF : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_PRF;
		break;
	case ID_UT_LINEAR_GAIN: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_LINEAR_GAIN : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_LINEAR_GAIN;
		break;
	case ID_UT_LOG_GAIN: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_LOG_GAIN : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_LOG_GAIN;
		break;
	case ID_UT_OUTPUT_MODE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_OUTPUT_MODE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_OUTPUT_MODE;
		break;
	case ID_UT_RECTIFY_MODE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_RECTIFY_MODE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_RECTIFY_MODE;
		break;
	case ID_UT_FILTER_TYPE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_FILTER_TYPE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_FILTER_TYPE;
		break;
	case ID_UT_FILTER_RANGE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_FILTER_RANGE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_FILTER_RANGE;
		break;
	case ID_UT_FILTER_CENTER: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_FILTER_CENTER : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_FILTER_CENTER;
		break;
	case ID_UT_FILTER_CUT_OFF: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_FILTER_CUT_OFF : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_FILTER_CUT_OFF;
		break;
	case ID_UT_DAC_MODE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_DAC_MODE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_DAC_MODE;
		break;
	case ID_UT_DAC_TRIGGER: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_DAC_TRIGGER : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_DAC_TRIGGER;
		break;
	case ID_UT_DAC_DELAY: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_DAC_DELAY : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_DAC_DELAY;
		break;
	case ID_UT_DAC_THRESHOLD: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_DAC_THRESHOLD : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_DAC_THRESHOLD;
		break;
	case ID_UT_DAC_TABLE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_DAC_TABLE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_DAC_TABLE;
		break;
	case ID_UT_SCOPE_DELAY: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_SCOPE_DELAY : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_SCOPE_DELAY;
		break;
	case ID_UT_SCOPE_WIDTH: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_SCOPE_WIDTH : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_SCOPE_WIDTH;
		break;
	case ID_UT_GATE_MODE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_GATE_MODE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_GATE_MODE;
		break;
	case ID_UT_GATE_DELAY: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_GATE_DELAY : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_GATE_DELAY;
		break;
	case ID_UT_GATE_WIDTH: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_GATE_WIDTH : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_GATE_WIDTH;
		break;
	case ID_UT_GATE_THRESHOLD: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_GATE_THRESHOLD : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_GATE_THRESHOLD;
		break;
	case ID_UT_GATE_FAR_DEAD_ZONE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_GATE_FAR_DEAD_ZONE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_GATE_FAR_DEAD_ZONE;
		break;
	case ID_UT_GATE_FAR_PEAK_WIDTH: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_GATE_FAR_PEAK_WIDTH : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_GATE_FAR_PEAK_WIDTH;
		break;
	case ID_UT_AMPLITUDE_COLLECT: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_AMPLITUDE_COLLECT : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_AMPLITUDE_COLLECT;
		break;
	case ID_UT_TOF_COLLECT: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_TOF_COLLECT : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_TOF_COLLECT;
		break;
	case ID_UT_TOF_MODE: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_TOF_MODE : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_TOF_MODE;
		break;
	case ID_UT_PULSER_OFF_ON: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PULSER_OFF_ON : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_PULSER_OFF_ON;
		break;
	case ID_UT_TX: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_TX : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_TX;
		break;
	case ID_UT_RX: bFlag == true ? theApp.m_LastSettings.n64SubsetUtVariable |= _UT_RX : theApp.m_LastSettings.n64SubsetUtVariable &= ~_UT_RX;
		break;
		//UT Copy
	case ID_COPY_UT_PREAMP_GAIN: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_PREAMP_GAIN : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_PREAMP_GAIN;
		break;
	case ID_COPY_UT_PREAMP_MODE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_PREAMP_MODE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_PREAMP_MODE;
		break;
	case ID_COPY_UT_DAMPING: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_DAMPING : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_DAMPING;
		break;
	case ID_COPY_UT_PULSE_WIDTH: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_PULSE_WIDTH : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_PULSE_WIDTH;
		break;
	case ID_COPY_UT_PULSE_VOLTAGE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_PULSE_VOLTAGE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_PULSE_VOLTAGE;
		break;
	case ID_COPY_UT_PRF: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_PRF : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_PRF;
		break;
	case ID_COPY_UT_LINEAR_GAIN: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_LINEAR_GAIN : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_LINEAR_GAIN;
		break;
	case ID_COPY_UT_LOG_GAIN: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_LOG_GAIN : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_LOG_GAIN;
		break;
	case ID_COPY_UT_OUTPUT_MODE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_OUTPUT_MODE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_OUTPUT_MODE;
		break;
	case ID_COPY_UT_RECTIFY_MODE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_RECTIFY_MODE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_RECTIFY_MODE;
		break;
	case ID_COPY_UT_FILTER_TYPE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_FILTER_TYPE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_FILTER_TYPE;
		break;
	case ID_COPY_UT_FILTER_RANGE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_FILTER_RANGE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_FILTER_RANGE;
		break;
	case ID_COPY_UT_FILTER_CENTER: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_FILTER_CENTER : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_FILTER_CENTER;
		break;
	case ID_COPY_UT_FILTER_CUT_OFF: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_FILTER_CUT_OFF : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_FILTER_CUT_OFF;
		break;
	case ID_COPY_UT_DAC_MODE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_DAC_MODE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_DAC_MODE;
		break;
	case ID_COPY_UT_DAC_TRIGGER: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_DAC_TRIGGER : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_DAC_TRIGGER;
		break;
	case ID_COPY_UT_DAC_DELAY: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_DAC_DELAY : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_DAC_DELAY;
		break;
	case ID_COPY_UT_DAC_THRESHOLD: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_DAC_THRESHOLD : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_DAC_THRESHOLD;
		break;
	case ID_COPY_UT_DAC_TABLE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_DAC_TABLE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_DAC_TABLE;
		break;
	case ID_COPY_UT_SCOPE_DELAY: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_SCOPE_DELAY : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_SCOPE_DELAY;
		break;
	case ID_COPY_UT_SCOPE_WIDTH: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_SCOPE_WIDTH : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_SCOPE_WIDTH;
		break;
	case ID_COPY_UT_GATE_MODE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_GATE_MODE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_GATE_MODE;
		break;
	case ID_COPY_UT_GATE_DELAY: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_GATE_DELAY : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_GATE_DELAY;
		break;
	case ID_COPY_UT_GATE_WIDTH: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_GATE_WIDTH : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_GATE_WIDTH;
		break;
	case ID_COPY_UT_GATE_THRESHOLD: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_GATE_THRESHOLD : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_GATE_THRESHOLD;
		break;
	case ID_COPY_UT_GATE_FAR_DEAD_ZONE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_GATE_FAR_DEAD_ZONE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_GATE_FAR_DEAD_ZONE;
		break;
	case ID_COPY_UT_GATE_FAR_PEAK_WIDTH: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_GATE_FAR_PEAK_WIDTH : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_GATE_FAR_PEAK_WIDTH;
		break;
	case ID_COPY_UT_AMPLITUDE_COLLECT: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_AMPLITUDE_COLLECT : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_AMPLITUDE_COLLECT;
		break;
	case ID_COPY_UT_TOF_COLLECT: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_TOF_COLLECT : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_TOF_COLLECT;
		break;
	case ID_COPY_UT_TOF_MODE: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_TOF_MODE : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_TOF_MODE;
		break;
	case ID_COPY_UT_PULSER_OFF_ON: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_PULSER_OFF_ON : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_PULSER_OFF_ON;
		break;
	case ID_COPY_UT_TX: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_TX : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_TX;
		break;
	case ID_COPY_UT_RX: bFlag == true ? theApp.m_LastSettings.n64CopyUtVariable |= _UT_RX : theApp.m_LastSettings.n64CopyUtVariable &= ~_UT_RX;
		break;
	case ID_TURNTABLE_CENTER:
		Buff = pFileProperty->GetValue().bstrVal;
		ExtractVector(Buff, &theApp.m_Tank.vTTCentre);
		break;
	case ID_TURNTABLE_NORMAL:
		Buff = pFileProperty->GetValue().bstrVal;
		ExtractVector(Buff, NULL, &theApp.m_Tank.vTTNorm);
		break;
	case ID_TURNTABLE_ZERO_DEGREES:
		Buff = pFileProperty->GetValue().bstrVal;
		ExtractVector(Buff, NULL, &theApp.m_Tank.vTTZeroDegrees);
		break;
	case ID_JOG_SPEED_X_0: theApp.m_Robot[0].m_fJogDirSpeedX = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_Y_0: theApp.m_Robot[0].m_fJogDirSpeedY = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_Z_0: theApp.m_Robot[0].m_fJogDirSpeedZ = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_H_0: theApp.m_Robot[0].m_fJogDirSpeedHTip = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_V_0: theApp.m_Robot[0].m_fJogDirSpeedVTip = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_T_0: theApp.m_Robot[0].m_fJogDirSpeedTTip = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_R_0: theApp.m_Robot[0].m_fJogDirSpeedR = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_X_1: theApp.m_Robot[1].m_fJogDirSpeedX = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_Y_1: theApp.m_Robot[1].m_fJogDirSpeedY = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_Z_1: theApp.m_Robot[1].m_fJogDirSpeedZ = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_H_1: theApp.m_Robot[1].m_fJogDirSpeedHTip = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_V_1: theApp.m_Robot[1].m_fJogDirSpeedVTip = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_T_1: theApp.m_Robot[1].m_fJogDirSpeedTTip = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_SPEED_R_1: theApp.m_Robot[1].m_fJogDirSpeedR = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_MOVE_SPEED: theApp.m_LastSettings.fMoveSpeed = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_ORTHOGONAL_SPEED: theApp.m_LastSettings.fPeekOrthogonalSpeed = pUSLProperty->GetFloatSpinValue();
		break;
	}


	if (nMotoionToolsTabMask[0] - theApp.m_LastSettings.nMotoionToolsTabMask[0] || nMotoionToolsTabMask[1] - theApp.m_LastSettings.nMotoionToolsTabMask[1]) {
		pFrame->SendMessage(UI_UPDATE_MOTION_TOOLS);
	}
	if (nUltrasonicsTabMask - theApp.m_LastSettings.nUltrasonicsTabMask) {
		pFrame->SendMessage(UI_UPDATE_ULTRASONICS_SHEET);
	}
	if (nPaletteTabMask - theApp.m_LastSettings.nPaletteTabMask) {
		pFrame->SendMessage(UI_UPDATE_PALETTE_SHEET,1);
	}
	if (n64SubsetUtVariable - theApp.m_LastSettings.n64SubsetUtVariable) {
		pFrame->SendMessage(UI_UPDATE_ULTRASONICS_SHEET);
	}


	return 0;
}

