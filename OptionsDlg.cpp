// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "OptionsDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	//}}AFX_DATA_INIT
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Control(pDX, IDC_CHECK_HT_OFF_ON_ESTOP, m_checkHTOffEStop);
	DDX_Control(pDX, IDC_CHECK_ROBOT_WEIGHTINGS, m_checkLoadRobotWeightings);
	DDX_Control(pDX, IDC_EDIT_3POINT_NAME_2, m_edit3PointName2);
	DDX_Control(pDX, IDC_EDIT_3POINT_NAME_1, m_edit3PointName1);
	DDX_Control(pDX, IDC_EDIT_3POINT_NAME_0, m_edit3PointName0);
	DDX_Control(pDX, IDC_CHECK_HT_OFF_MACHINE_ZERO, m_checkHTOffMachineZero);
	DDX_Control(pDX, IDC_CHECK_HT_OFF_PUMPS, m_checkHTOffPumps);
	DDX_Control(pDX, IDC_CHECK_AUTO_1TO1, m_checkAuto1to1CScans);
	DDX_Control(pDX, IDC_EDIT_MAX_PULSE_VOLTAGE, m_editMaxPulseVoltage);
	DDX_Control(pDX, IDC_SPIN_MAX_PULSE_VOLTAGE, m_spinMaxPulseVoltage);
	DDX_Control(pDX, IDC_EDIT_ROLLER_STEPSIZE, m_editRollerStepSize);
	DDX_Control(pDX, IDC_CHECK_LOAD_UT_FILE, m_checkLoadUtFileAtStartOfScan);
	DDX_Control(pDX, IDC_CHECK_SAVE_INDIVIDUAL, m_checkSaveIndividual);
	DDX_Control(pDX, IDC_SPIN_ROLLER_DIAMETER, m_spinRollerDiameter);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_PRECISION, m_editStepSizePrecision);
	DDX_Control(pDX, IDC_COMBO_VOLUME_MONITOR, m_comboVolumeMonitor);
	DDX_Control(pDX, IDC_CHECK_ROLLER_PROBE_BACKOFF, m_checkRollerProbeBackoff);
	DDX_Control(pDX, IDC_CHECK_SCAN_LOCK_WP, m_checkLockWP);
	DDX_Control(pDX, IDC_CHECK_SCAN_LOCK_UT, m_checkLockUT);
	DDX_Control(pDX, IDC_CHECK_SCAN_START_VIA_DANGERPLANE, m_checkScanStartViaDangerPlane);
	DDX_Control(pDX, IDC_CHECK_LOCK_FILES, m_checkLockFile);
	DDX_Control(pDX, IDC_CHECK_PROPERTY_PAGE_COMPLETE, m_checkPropertyPageComplete);
	DDX_Control(pDX, IDC_CHECK_FORCE_PROPERTY, m_checkForcePropertyPage);
	DDX_Control(pDX, IDC_EDIT_CHANGE_IJK, m_editWayPointChangeIJK);
	DDX_Control(pDX, IDC_EDIT_CHANGE_XYZ, m_editWayPointChangeXYZ);
	DDX_Control(pDX, IDC_COMBO_WAYPOINTS_MODE, m_comboWaypointsMode);
	DDX_Control(pDX, IDC_COMBO_1TO1_MONITOR, m_combo1To1Monitor);
	DDX_Control(pDX, IDC_COMBO_1TO1_POSITION, m_combo1To1Position);
	DDX_Control(pDX, IDC_CHECK_AUTO_ENABLE_JOYSTICK, m_checkAutoEnableJoystick);
	DDX_Control(pDX, IDC_CHECK_PROFILE_NEW_TAUGHT_POINTS, m_checkProfileNewTaught);
	DDX_Control(pDX, IDC_EDIT_NUMBER_LINES, m_editNumberLines);
	DDX_Control(pDX, IDC_EDIT_SLOW_INCREMENT, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_FAST_INCREMENT, m_editFastIncrement);
	DDX_Control(pDX, IDC_IPADDRESS_LASER, m_ipaddressLaser);
	DDX_Control(pDX, IDC_COMBO_LASER, m_comboLaser);
	DDX_Control(pDX, IDC_CHECK_REDBLUE_REVERSE, m_buttonRedBlueReverse);
	DDX_Control(pDX, IDC_CHECK_DPRAM, m_checkDPRAM);
	DDX_Control(pDX, IDC_CHECK_SAVE_EVERY_LINE, m_buttonSaveScanEveryLine);
	DDX_Control(pDX, IDC_CHECK_LOAD_ORIGIN, m_checkLoadOriginFromProfile);
	DDX_Control(pDX, IDC_EDIT_MONITORWIDTH, m_editMonitorWidth);
	DDX_Control(pDX, IDC_CHECK_GOTO_VIA_DANGER, m_checkGotoViaDangerPlane);
	DDX_Control(pDX, IDC_CHECK_LOAD_WP, m_checkLoadWaterPaths);
	DDX_Control(pDX, IDC_COMBO_AREYOUSURE, m_comboAreYouSure);
	DDX_Control(pDX, IDC_CHECK_LOAD_UT, m_checkLoadUtFormProfile);
	DDX_Control(pDX, IDC_CHECK_IMPORTSAMPALETTE, m_checkImportPalettesFromSam);
	DDX_Control(pDX, IDC_CHECK_TIMESLOTNAME, m_checkTimeSlotName);
	DDX_Control(pDX, IDC_CHECK_ENTER_FILENAME, m_checkEnterFileName);
	DDX_Control(pDX, IDC_COMBO_GRATICULE_TYPE, m_comboGraticuleType);
	DDX_Control(pDX, IDC_EDIT_ROLLER_DIAMETER, m_editRollerDiameter);
	DDX_Control(pDX, IDC_COMBO_TEXT_EPARATOR, m_comboTextSeparator);
	DDX_Control(pDX, IDC_EDIT_MEMORY_AVAILABLE, m_editMemoryAvailable);
	DDX_Control(pDX, IDC_COMBO_JOYSTICK_SOURCE, m_comboJoystickSource);
	DDX_Control(pDX, IDC_COMBO_REMEMBER_DIALOGS, m_comboRememberDlgs);
	DDX_Control(pDX, IDC_COMBO_MOTOR_CONTROLLER, m_comboMotorController);
	DDX_Control(pDX, IDC_SPIN_MAX_SCAN_SPEED, m_spinMaxScanSpeed);
	DDX_Control(pDX, IDC_EDIT_MAX_SCAN_SPEED, m_editMaxScanSpeed);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_NUMBER_SEGMENTS, m_editNumberWayPoints);
	DDX_Control(pDX, IDC_EDIT_DE_GLITCH_THRESHOLD, m_editDeGlitchThreshold);
	DDX_Control(pDX, IDC_CHECK_DE_GLITCH, m_checkScanDeGlitch);
	DDX_Control(pDX, IDC_COMBO_DANGER_PLANE_DIR, m_comboDangerPlaneDir);
	DDX_Control(pDX, IDC_CHECK_DPC, m_checkScanDPCFill);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_MAX_SCAN_SPEED, OnChangeEditMaxScanSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_MOTOR_CONTROLLER, OnSelchangeComboMotorController)
	ON_CBN_SELCHANGE(IDC_COMBO_REMEMBER_DIALOGS, OnSelchangeComboRememberDialogs)
	ON_CBN_SELCHANGE(IDC_COMBO_JOYSTICK_SOURCE, OnSelchangeComboJoystickSource)
	ON_EN_CHANGE(IDC_EDIT_MEMORY_AVAILABLE, OnChangeEditMemoryAvailable)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXT_EPARATOR, OnSelchangeComboTextEparator)
	ON_EN_CHANGE(IDC_EDIT_ROLLER_STEPSIZE, OnChangeEditRollerStepsize)
	ON_EN_CHANGE(IDC_EDIT_ROLLER_DIAMETER, OnChangeEditRollerDiameter)
	ON_CBN_SELCHANGE(IDC_COMBO_GRATICULE_TYPE, OnSelchangeComboGraticuleType)
	ON_BN_CLICKED(IDC_CHECK_ENTER_FILENAME, OnCheckEnterFilename)
	ON_BN_CLICKED(IDC_CHECK_TIMESLOTNAME, OnCheckTimeslotname)
	ON_BN_CLICKED(IDC_CHECK_DPRAM, OnCheckDPRam)
	ON_BN_CLICKED(IDC_CHECK_AUTO_ENABLE_JOYSTICK, OnCheckAutoEnableJoystick)
	ON_BN_CLICKED(IDC_CHECK_PROPERTY_PAGE_COMPLETE, OnCheckPropertyPageComplete)
	ON_BN_CLICKED(IDC_CHECK_FORCE_PROPERTY, OnCheckForceProperty)
	ON_BN_CLICKED(IDC_CHECK_LOCK_FILES, OnCheckLockFiles)
	ON_BN_CLICKED(IDC_CHECK_SCAN_START_VIA_DANGERPLANE, OnCheckScanStartViaDangerplane)
	ON_BN_CLICKED(IDC_CHECK_SCAN_LOCK_UT, OnCheckScanLockUt)
	ON_BN_CLICKED(IDC_CHECK_SCAN_LOCK_WP, OnCheckScanLockWp)
	ON_BN_CLICKED(IDC_CHECK_ROLLER_PROBE_BACKOFF, OnCheckRollerProbeBackoff)
	ON_CBN_SELCHANGE(IDC_COMBO_VOLUME_MONITOR, OnSelchangeComboVolumeMonitor)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ROLLER_DIAMETER, OnDeltaposSpinRollerDiameter)
	ON_BN_CLICKED(IDC_CHECK_SAVE_INDIVIDUAL, OnCheckSaveIndividual)
	ON_BN_CLICKED(IDC_CHECK_LOAD_UT_FILE, OnCheckLoadUtFile)
	ON_BN_CLICKED(IDC_CHECK_AUTO_1TO1, OnCheckAuto1to1)
	ON_EN_CHANGE(IDC_EDIT_MAX_PULSE_VOLTAGE, OnChangeEditMaxPulseVoltage)
	ON_BN_CLICKED(IDC_CHECK_ROBOT_WEIGHTINGS, OnCheckRobotWeightings)
	ON_BN_CLICKED(IDC_CHECK_HT_OFF_ON_ESTOP, OnCheckHtOffOnEstop)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_DE_GLITCH, &COptionsDlg::OnBnClickedCheckDeGlitch)
	ON_EN_CHANGE(IDC_EDIT_DE_GLITCH_THRESHOLD, &COptionsDlg::OnEnChangeEditDeGlitchThreshold)
	ON_CBN_SELCHANGE(IDC_COMBO_DANGER_PLANE_DIR, &COptionsDlg::OnCbnSelchangeComboDangerPlaneDir)
	ON_BN_CLICKED(IDC_CHECK_DPC, &COptionsDlg::OnBnClickedCheckDpc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString	Buff;

	m_comboMotorController.AddString(_T("SI6"));
	m_comboMotorController.AddString(_T("PMAC Turbo"));
	m_comboMotorController.AddString(_T("PMAC 8 Axis"));
	m_comboMotorController.AddString(_T("SI10"));
	m_comboMotorController.AddString(_T("Field Bus"));
	m_comboMotorController.SetCurSel(theApp.m_nMotorType);

	m_comboRememberDlgs.AddString(_T("No"));
	m_comboRememberDlgs.AddString(_T("Yes"));
	m_comboRememberDlgs.SetCurSel(theApp.m_LastSettings.bWindowPos);

	m_comboJoystickSource.AddString(_T("MSC"));
	m_comboJoystickSource.AddString(_T("SI6"));
	m_comboJoystickSource.AddString(_T("Wired serial"));
	m_comboJoystickSource.AddString(_T("Wireless serial"));
	m_comboJoystickSource.AddString(_T("Si10"));
	m_comboJoystickSource.SetCurSel(theApp.m_Tank.nJoystickSource);


	m_comboTextSeparator.AddString(_T("<Space>"));
	m_comboTextSeparator.AddString(_T("Commar"));
	m_comboTextSeparator.AddString(_T("Tab"));
	m_comboTextSeparator.SetCurSel(theApp.m_LastSettings.nTextSeparator%3);


	Buff.Format(_T("%.07f%s"),theApp.m_LastSettings.fRollerStepSize,DEGREES);
	m_editRollerStepSize.SetWindowText(Buff);

	Buff.Format(_T("%.02f"),theApp.m_LastSettings.fRollerDiameter);
	m_editRollerDiameter.SetWindowText(Buff);
	m_spinRollerDiameter.SetRange(0, 1000);


	m_comboGraticuleType.AddString(_T("Lengh"));
	m_comboGraticuleType.AddString(_T("Position"));
	m_comboGraticuleType.SetCurSel(theApp.m_LastSettings.nGraticuleScrollMode);


	m_checkDPRAM.SetCheck(theApp.m_LastSettings.bDPRamWriteMemCheck&1);

	m_checkLoadUtFormProfile.SetCheck(theApp.m_LastSettings.nLoadUltrasonicsFromProfile&1);
	m_checkLoadWaterPaths.SetCheck(theApp.m_LastSettings.nLoadWaterPathsFromProfile&1);
	m_checkLoadOriginFromProfile.SetCheck(theApp.m_LastSettings.nLoadOriginFromProfile&1);

	m_comboAreYouSure.AddString(_T("No"));
	m_comboAreYouSure.AddString(_T("Yes"));
	m_comboAreYouSure.SetCurSel(theApp.m_LastSettings.nDisplayAreYouSure);

	m_checkGotoViaDangerPlane.SetCheck(theApp.m_LastSettings.bGotoViaDangerPlane&1);

	Buff.Format(_T("%d mm"),theApp.m_LastSettings.nScreenHorizontalSizeMM);
	m_editMonitorWidth.SetWindowText(Buff);

	m_buttonSaveScanEveryLine.SetCheck(theApp.m_LastSettings.nSaveScanEveryLine);
	m_buttonRedBlueReverse.SetCheck(theApp.m_LastSettings.bRedBlueReverse);

	m_comboLaser.InsertString(0,_T("None"));
	m_comboLaser.InsertString(1,_T("Local"));
	m_comboLaser.InsertString(2,_T("Remote"));
	m_comboLaser.SetCurSel(theApp.m_LastSettings.nLaserLocation);

	sockaddr_in Server;
	Server.sin_addr.s_addr = theApp.m_LastSettings.nLaserIPAddrs;
	m_ipaddressLaser.SetAddress(Server.sin_addr.s_addr); 

	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,theApp.m_LastSettings.fFastIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editFastIncrement.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,theApp.m_LastSettings.fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editSlowIncrement.SetWindowText(Buff);

	Buff.Format(L"%d",theApp.m_LastSettings.m_nNumberProbesPerImage);
	m_editNumberLines.SetWindowText(Buff);

	m_checkProfileNewTaught.SetCheck(theApp.m_LastSettings.nSelectTaughtButtonOnNew);

	m_checkAutoEnableJoystick.SetCheck(theApp.m_LastSettings.bAutoEnableJoystick);

	Buff.LoadString(IDS_Left);	m_combo1To1Monitor.AddString(Buff);
	Buff.LoadString(IDS_Right);	m_combo1To1Monitor.AddString(Buff);
	Buff.LoadString(IDS_Both);	m_combo1To1Monitor.AddString(Buff);
	m_combo1To1Monitor.SetCurSel(theApp.m_LastSettings.n1to1Monitor);

	Buff.LoadString(IDS_Top_Left);		m_combo1To1Position.AddString(Buff);
	Buff.LoadString(IDS_Top_Right);		m_combo1To1Position.AddString(Buff);
	Buff.LoadString(IDS_Bottom_Left);	m_combo1To1Position.AddString(Buff);
	Buff.LoadString(IDS_Bottom_Right);	m_combo1To1Position.AddString(Buff);
	Buff.LoadString(IDS_Centre);		m_combo1To1Position.AddString(Buff);
	m_combo1To1Position.SetCurSel(theApp.m_LastSettings.n1to1Position);

	Buff.LoadString(IDS_Left);	m_comboVolumeMonitor.AddString(Buff);
	Buff.LoadString(IDS_Right);	m_comboVolumeMonitor.AddString(Buff);
	Buff.LoadString(IDS_Both);	m_comboVolumeMonitor.AddString(Buff);
	m_comboVolumeMonitor.SetCurSel(theApp.m_LastSettings.nVolumeMonitor);

	Buff.LoadString(IDS_Even_Segments);		m_comboWaypointsMode.AddString(Buff);
	Buff.LoadString(IDS_Absolute_Change);	m_comboWaypointsMode.AddString(Buff);
	Buff.LoadString(IDS_Changed_Vector);	m_comboWaypointsMode.AddString(Buff);
	Buff.LoadString(IDS_All_Scan_Points);	m_comboWaypointsMode.AddString(Buff);
	m_comboWaypointsMode.SetCurSel(theApp.m_LastSettings.nWaypointMode);

	Buff.Format(_T("%.01fmm"),theApp.m_LastSettings.fWaypointXYZChange);
	m_editWayPointChangeXYZ.SetWindowText(Buff);

	Buff.Format(_T("%.01f%s"),theApp.m_LastSettings.fWaypointIJKChange,DEGREES);
	m_editWayPointChangeIJK.SetWindowText(Buff);

	Buff.Format(L"%d", theApp.m_LastSettings.nNumberOfWayPoints);
	m_editNumberWayPoints.SetWindowText(Buff);

	theApp.m_LastSettings.nScanMask & _ENTER_FILENAME_AT_START ?			m_checkEnterFileName.SetCheck(true)				: m_checkEnterFileName.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _FORCE_PROPERTY_PAGE ?				m_checkForcePropertyPage.SetCheck(true)			: m_checkForcePropertyPage.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _PROPERTY_PAGE_MUST_BE_COMPLETE ?		m_checkPropertyPageComplete.SetCheck(true)		: m_checkPropertyPageComplete.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _LOCK_FILE_ON_SENTENCE ?				m_checkLockFile.SetCheck(true)					: m_checkLockFile.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _SCAN_START_VIA_DANGER_PLANE ?		m_checkScanStartViaDangerPlane.SetCheck(true)	: m_checkScanStartViaDangerPlane.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _LOCK_UT ?							m_checkLockUT.SetCheck(true)					: m_checkLockUT.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _LOCK_WP ?							m_checkLockWP.SetCheck(true)					: m_checkLockWP.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _ROLLERPROBE_BACKOFF ?				m_checkRollerProbeBackoff.SetCheck(true)		: m_checkRollerProbeBackoff.SetCheck(false); 
	theApp.m_LastSettings.nScanMask & _SAVE_INDIVIDUAL_CSCANS ?				m_checkSaveIndividual.SetCheck(true)			: m_checkSaveIndividual.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _LOAD_UT_FILE_AT_SCAN_START ?			m_checkLoadUtFileAtStartOfScan.SetCheck(true)	: m_checkLoadUtFileAtStartOfScan.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _AUTO_1TO1	 ?						m_checkAuto1to1CScans.SetCheck(true)			: m_checkAuto1to1CScans.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _LOAD_ROBOT_WEIGHTINGS_FROM_PROFILE ? m_checkLoadRobotWeightings.SetCheck(true)		: m_checkLoadRobotWeightings.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _DEGLITCH_FILTER ?					m_checkScanDeGlitch.SetCheck(true)				: m_checkScanDeGlitch.SetCheck(false);
	theApp.m_LastSettings.nScanMask & _FILL_DPC_ERROR ?						m_checkScanDPCFill.SetCheck(true)				: m_checkScanDPCFill.SetCheck(false);

	MinMax(&theApp.m_LastSettings.nStepSizePrecision,0,3);
	Buff.Format(L"%d",theApp.m_LastSettings.nStepSizePrecision);
	
	m_editStepSizePrecision.SetWindowText(Buff);

	MinMax(&theApp.m_Tank.nMaxPulseVoltage,50,600);
	m_spinMaxPulseVoltage.SetRange(50, 600);
	m_spinMaxPulseVoltage.SetPos(theApp.m_Tank.nMaxPulseVoltage);

	Buff.Format(L"%d V",theApp.m_Tank.nMaxPulseVoltage);
	m_editMaxPulseVoltage.SetWindowText(Buff);

	theApp.m_LastSettings.nHTMask & _HT_OFF_PUMPS_OFF ?		m_checkHTOffPumps.SetCheck(true)		: m_checkHTOffPumps.SetCheck(false);
	theApp.m_LastSettings.nHTMask & _HT_OFF_MACHINE_ZERO ?	m_checkHTOffMachineZero.SetCheck(true)	: m_checkHTOffMachineZero.SetCheck(false);
	theApp.m_LastSettings.nHTMask & _HT_OFF_E_STOP ?			m_checkHTOffEStop.SetCheck(true)		: m_checkHTOffEStop.SetCheck(false);

	m_edit3PointName0.SetWindowText(theApp.m_3PtName[0]);
	m_edit3PointName1.SetWindowText(theApp.m_3PtName[1]);
	m_edit3PointName2.SetWindowText(theApp.m_3PtName[2]);

	Buff.Format(L"%d %%", theApp.m_LastSettings.nDeGitchThreshold);
	m_editDeGlitchThreshold.SetWindowTextW(Buff);

	m_comboDangerPlaneDir.AddString(L"i");
	m_comboDangerPlaneDir.AddString(L"j");
	m_comboDangerPlaneDir.AddString(L"k");
	m_comboDangerPlaneDir.SetCurSel(theApp.m_LastSettings.nDangerPlaneDir[0]);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void COptionsDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString Buff;
	int nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_MAX_PULSE_VOLTAGE:
		nTemp = theApp.m_Tank.nMaxPulseVoltage;
		theApp.m_Tank.nMaxPulseVoltage=pSpin->GetPos()&0xffff;
		if(	nTemp - theApp.m_Tank.nMaxPulseVoltage) {
			Buff.Format(L"%d V",theApp.m_Tank.nMaxPulseVoltage);
			m_editMaxPulseVoltage.SetWindowText(Buff);
		}
		break;

	};

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}



void COptionsDlg::OnChangeEditMaxScanSpeed()
{
}


void COptionsDlg::OnSelchangeComboMotorController()
{

	theApp.m_nMotorType=m_comboMotorController.GetCurSel();
}

void COptionsDlg::OnSelchangeComboRememberDialogs()
{

	theApp.m_LastSettings.bWindowPos=m_comboRememberDlgs.GetCurSel() & 0x01;
}

void COptionsDlg::OnSelchangeComboJoystickSource()
{

	theApp.m_Tank.nJoystickSource=(JoystickSource)m_comboJoystickSource.GetCurSel();

}

void COptionsDlg::OnChangeEditMemoryAvailable()
{
}

void COptionsDlg::OnSelchangeComboTextEparator()
{
	theApp.m_LastSettings.nTextSeparator = m_comboTextSeparator.GetCurSel();
}





void COptionsDlg::OnOK()
{
	CString Buff;

	theApp.m_LastSettings.nLoadUltrasonicsFromProfile = m_checkLoadUtFormProfile.GetCheck();
	theApp.m_LastSettings.nLoadWaterPathsFromProfile = m_checkLoadWaterPaths.GetCheck();
	theApp.m_LastSettings.nLoadOriginFromProfile = m_checkLoadOriginFromProfile.GetCheck();


	theApp.m_LastSettings.nDisplayAreYouSure=m_comboAreYouSure.GetCurSel();
	theApp.m_LastSettings.bGotoViaDangerPlane=m_checkGotoViaDangerPlane.GetCheck() & 0x01;

	m_editMonitorWidth.GetWindowText(Buff);
	theApp.m_LastSettings.nScreenHorizontalSizeMM = _ttoi(Buff);

	theApp.m_LastSettings.nSaveScanEveryLine = m_buttonSaveScanEveryLine.GetCheck();
	theApp.m_LastSettings.bRedBlueReverse = m_buttonRedBlueReverse.GetCheck() & 0x01;


	theApp.m_LastSettings.nLaserLocation = m_comboLaser.GetCurSel();

	sockaddr_in Server;
	m_ipaddressLaser.GetAddress(Server.sin_addr.s_addr); 
	theApp.m_LastSettings.nLaserIPAddrs = Server.sin_addr.s_addr;

	m_editSlowIncrement.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fSlowIncrement);
	theApp.m_LastSettings.fSlowIncrement /= theApp.m_fUnits;

	m_editFastIncrement.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fFastIncrement);
	theApp.m_LastSettings.fFastIncrement /= theApp.m_fUnits;

	m_editNumberLines.GetWindowText(Buff);
	theApp.m_LastSettings.m_nNumberProbesPerImage = _ttoi(Buff);

	theApp.m_LastSettings.nSelectTaughtButtonOnNew = m_checkProfileNewTaught.GetCheck();

	theApp.m_LastSettings.n1to1Monitor = m_combo1To1Monitor.GetCurSel();
	theApp.m_LastSettings.n1to1Position = m_combo1To1Position.GetCurSel();
	theApp.m_LastSettings.nVolumeMonitor = m_comboVolumeMonitor.GetCurSel();

	theApp.m_LastSettings.nWaypointMode = m_comboWaypointsMode.GetCurSel();
	m_editWayPointChangeXYZ.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fWaypointXYZChange);
	m_editWayPointChangeIJK.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fWaypointIJKChange);
	m_editNumberWayPoints.GetWindowText(Buff);
	theApp.m_LastSettings.nNumberOfWayPoints = _ttoi(Buff);
	MinMax(&theApp.m_LastSettings.nNumberOfWayPoints, 2, 120);

	m_editStepSizePrecision.GetWindowText(Buff);
	theApp.m_LastSettings.nStepSizePrecision = _ttoi(Buff);
	MinMax(&theApp.m_LastSettings.nStepSizePrecision,0,3);

	m_editMaxPulseVoltage.GetWindowText(Buff);
	theApp.m_Tank.nMaxPulseVoltage = _ttoi(Buff);

	m_checkHTOffPumps.GetCheck() == 1			? theApp.m_LastSettings.nHTMask |= _HT_OFF_PUMPS_OFF		: theApp.m_LastSettings.nHTMask &= ~_HT_OFF_PUMPS_OFF ;
	m_checkHTOffMachineZero.GetCheck() == 1		? theApp.m_LastSettings.nHTMask |= _HT_OFF_MACHINE_ZERO	: theApp.m_LastSettings.nHTMask &= ~_HT_OFF_MACHINE_ZERO ;

	m_edit3PointName0.GetWindowText(theApp.m_3PtName[0]);
	m_edit3PointName1.GetWindowText(theApp.m_3PtName[1]);
	m_edit3PointName2.GetWindowText(theApp.m_3PtName[2]);


	theApp.SaveTank();

	CDialog::OnOK();
}

void COptionsDlg::OnSelchangeComboGraticuleType()
{
	theApp.m_LastSettings.nGraticuleScrollMode = m_comboGraticuleType.GetCurSel();
}

void COptionsDlg::OnCheckEnterFilename()
{
	m_checkEnterFileName.GetCheck() ? theApp.m_LastSettings.nScanMask |= _ENTER_FILENAME_AT_START : theApp.m_LastSettings.nScanMask &= ~_ENTER_FILENAME_AT_START;
}

void COptionsDlg::OnCheckTimeslotname()
{

}

void COptionsDlg::OnCheckDPRam()
{
	theApp.m_LastSettings.bDPRamWriteMemCheck = m_checkDPRAM.GetCheck()&1;
}



void COptionsDlg::SetRegistry()
{

}

void COptionsDlg::OnCheckAutoEnableJoystick() 
{
	theApp.m_LastSettings.bAutoEnableJoystick = m_checkAutoEnableJoystick.GetCheck() & 1;
}

void COptionsDlg::OnCheckPropertyPageComplete() 
{
	m_checkPropertyPageComplete.GetCheck() ? theApp.m_LastSettings.nScanMask |= _PROPERTY_PAGE_MUST_BE_COMPLETE : theApp.m_LastSettings.nScanMask &= ~_PROPERTY_PAGE_MUST_BE_COMPLETE;
}

void COptionsDlg::OnCheckForceProperty() 
{
	m_checkForcePropertyPage.GetCheck() ? theApp.m_LastSettings.nScanMask |= _FORCE_PROPERTY_PAGE : theApp.m_LastSettings.nScanMask &= ~_FORCE_PROPERTY_PAGE;
}

void COptionsDlg::OnCheckLockFiles() 
{
	m_checkLockFile.GetCheck() ? theApp.m_LastSettings.nScanMask |= _LOCK_FILE_ON_SENTENCE : theApp.m_LastSettings.nScanMask &= ~_LOCK_FILE_ON_SENTENCE;
}

void COptionsDlg::OnCheckScanStartViaDangerplane() 
{
	m_checkScanStartViaDangerPlane.GetCheck() ? theApp.m_LastSettings.nScanMask |= _SCAN_START_VIA_DANGER_PLANE : theApp.m_LastSettings.nScanMask &= ~_SCAN_START_VIA_DANGER_PLANE;
}

void COptionsDlg::OnCheckScanLockUt() 
{
	m_checkLockUT.GetCheck() ? theApp.m_LastSettings.nScanMask |= _LOCK_UT : theApp.m_LastSettings.nScanMask &= ~_LOCK_UT;
}

void COptionsDlg::OnCheckScanLockWp() 
{
	m_checkLockWP.GetCheck() ? theApp.m_LastSettings.nScanMask |= _LOCK_WP : theApp.m_LastSettings.nScanMask &= ~_LOCK_WP;
}

void COptionsDlg::OnCheckRollerProbeBackoff() 
{
	m_checkRollerProbeBackoff.GetCheck() ? theApp.m_LastSettings.nScanMask |= _ROLLERPROBE_BACKOFF : theApp.m_LastSettings.nScanMask &= ~_ROLLERPROBE_BACKOFF;
}

void COptionsDlg::OnCheckSaveIndividual() 
{
	m_checkSaveIndividual.GetCheck() ? theApp.m_LastSettings.nScanMask |= _SAVE_INDIVIDUAL_CSCANS : theApp.m_LastSettings.nScanMask &= ~_SAVE_INDIVIDUAL_CSCANS;
}

void COptionsDlg::OnSelchangeComboVolumeMonitor() 
{
	// TODO: Add your control notification handler code here
	
}
void COptionsDlg::OnChangeEditRollerStepsize()
{
	CString Buff;

	m_editRollerStepSize.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fRollerStepSize);

}

void COptionsDlg::OnChangeEditRollerDiameter()
{
	CString Buff;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	float fTemp;

	m_editRollerDiameter.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	if(fTemp - theApp.m_LastSettings.fRollerDiameter) {
		theApp.m_LastSettings.fRollerDiameter = fTemp;
		theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], true, true);
		pFrame->SendMessage(UI_UPDATE_AXES);
	}
}

void COptionsDlg::OnChangeEditRollerStepsperrev() 
{
	CString Buff;

	m_editRollerStepsPerRev.GetWindowText(Buff);

//	theApp.m_LastSettings.nRollerStepsPerRev = _ttoi(Buff);
}

void COptionsDlg::OnDeltaposSpinRollerDiameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	float fTemp = theApp.m_LastSettings.fRollerDiameter;
	
	theApp.m_LastSettings.fRollerDiameter += (float)pNMUpDown->iDelta * 0.01f;

	if(fTemp - theApp.m_LastSettings.fRollerDiameter) {
		Buff.Format(L"%.02f",theApp.m_LastSettings.fRollerDiameter);
		m_editRollerDiameter.SetWindowText(Buff);

		theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], true, true);
		pFrame->SendMessage(UI_UPDATE_AXES);
	}

	*pResult = 0;
}


void COptionsDlg::OnCheckLoadUtFile() 
{
	m_checkLoadUtFileAtStartOfScan.GetCheck() ? theApp.m_LastSettings.nScanMask |= _LOAD_UT_FILE_AT_SCAN_START : theApp.m_LastSettings.nScanMask &= ~_LOAD_UT_FILE_AT_SCAN_START;
}

void COptionsDlg::OnCheckAuto1to1() 
{
	m_checkAuto1to1CScans.GetCheck() ? theApp.m_LastSettings.nScanMask |= _AUTO_1TO1 : theApp.m_LastSettings.nScanMask &= ~_AUTO_1TO1;
}

void COptionsDlg::OnChangeEditMaxPulseVoltage() 
{
	CString Buff;
	int nTemp = theApp.m_Tank.nMaxPulseVoltage;

	m_editMaxPulseVoltage.GetWindowText(Buff);
	theApp.m_Tank.nMaxPulseVoltage = _ttoi(Buff);
	MinMax(&theApp.m_Tank.nMaxPulseVoltage,50,600);
	if(nTemp - theApp.m_Tank.nMaxPulseVoltage) {
		m_spinMaxPulseVoltage.SetPos(theApp.m_Tank.nMaxPulseVoltage);
	}


}

void COptionsDlg::OnCheckRobotWeightings() 
{
	m_checkLoadRobotWeightings.GetCheck() ? theApp.m_LastSettings.nScanMask |= _LOAD_ROBOT_WEIGHTINGS_FROM_PROFILE : theApp.m_LastSettings.nScanMask &= ~_LOAD_ROBOT_WEIGHTINGS_FROM_PROFILE;
}

void COptionsDlg::OnCheckHtOffOnEstop() 
{
	m_checkHTOffEStop.GetCheck() ? theApp.m_LastSettings.nHTMask |= _HT_OFF_E_STOP : theApp.m_LastSettings.nHTMask &= ~_HT_OFF_E_STOP;
}


void COptionsDlg::OnBnClickedCheckDeGlitch()
{
	m_checkScanDeGlitch.GetCheck() ? theApp.m_LastSettings.nScanMask |= _DEGLITCH_FILTER : theApp.m_LastSettings.nScanMask &= ~_DEGLITCH_FILTER;
}


void COptionsDlg::OnEnChangeEditDeGlitchThreshold()
{
	CString Buff;
	int nTemp;

	m_editDeGlitchThreshold.GetWindowText(Buff);
	nTemp = _ttoi(Buff);
	theApp.m_LastSettings.nDeGitchThreshold = MinMax(&nTemp, 0, 100);
}


void COptionsDlg::OnCbnSelchangeComboDangerPlaneDir()
{
	theApp.m_LastSettings.nDangerPlaneDir[0] = theApp.m_LastSettings.nDangerPlaneDir[1] = m_comboDangerPlaneDir.GetCurSel();
}


void COptionsDlg::OnBnClickedCheckDpc()
{
	m_checkScanDPCFill.GetCheck() ? theApp.m_LastSettings.nScanMask |= _FILL_DPC_ERROR : theApp.m_LastSettings.nScanMask &= ~_FILL_DPC_ERROR;
}
