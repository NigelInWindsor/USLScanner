// AxesPmacStatus.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AxesPmacStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxesPmacStatus property page

IMPLEMENT_DYNCREATE(CAxesPmacStatus, CPropertyPage)

CAxesPmacStatus::CAxesPmacStatus() : CPropertyPage(CAxesPmacStatus::IDD)
{
	//{{AFX_DATA_INIT(CAxesPmacStatus)
	//}}AFX_DATA_INIT
	for(int ii = 0;ii<32;ii++) {
		m_pAxisServoStatus[ii] = NULL;
	}
	
}

CAxesPmacStatus::~CAxesPmacStatus()
{
}

void CAxesPmacStatus::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxesPmacStatus)
	DDX_Control(pDX, IDC_CHECK_MANUAL_SCANNER_BRAKE, m_checkScannerBrakeOnOff);
	DDX_Control(pDX, IDC_CHECK_PLC_MANIPULATOR_SENSORS, m_checkManipulatorInterlocks);
	DDX_Control(pDX, IDC_CHECK_PLC_BRAKE_INTERLOCKS, m_checkBrakeInterlocks);
	DDX_Control(pDX, IDC_EDIT_HOLDING_BRAKE, m_editHoldingBrakes);
	DDX_Control(pDX, IDC_EDIT_SCANNER_BRAKES, m_editScannerBrakes);
	DDX_Control(pDX, IDC_EDIT_SCRATCH, m_editScratch);
	DDX_Control(pDX, IDC_COMBO_ZBRAKE_MODE2, m_comboBrakeMode1);
	DDX_Control(pDX, IDC_COMBO_ZBRAKE_MODE, m_comboBrakeMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxesPmacStatus, CPropertyPage)
	//{{AFX_MSG_MAP(CAxesPmacStatus)
	ON_WM_CTLCOLOR()
	ON_WM_MOVE()
	ON_CBN_SELCHANGE(IDC_COMBO_ZBRAKE_MODE, OnSelchangeComboZbrakeMode)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_ZBRAKE_MODE2, OnSelchangeComboZbrakeMode2)
	ON_BN_CLICKED(IDC_CHECK_PLC_BRAKE_INTERLOCKS, OnCheckPlcBrakeInterlocks)
	ON_BN_CLICKED(IDC_CHECK_PLC_MANIPULATOR_SENSORS, OnCheckPlcManipulatorSensors)
	ON_BN_CLICKED(IDC_CHECK_MANUAL_SCANNER_BRAKE, OnCheckManualScannerBrake)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxesPmacStatus message handlers

BOOL CAxesPmacStatus::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CRect rr;
	int nAxis,nDlg;

	GetWindowRect(&rr);
	rr.left+=90;
	rr.top+=0;
	for(nAxis=0,nDlg=0;(nAxis<32) && (nDlg<12);nAxis++) {
		if(nAxis>23) theApp.m_Axes[nAxis].bAvailable = false;
		if((theApp.m_Axes[nAxis].nMotorType == SERVO) && (nAxis!=theApp.m_Tank.nLeftPump) && (nAxis!=theApp.m_Tank.nRightPump) && (theApp.m_Axes[nAxis].bAvailable)) {
			m_pAxisServoStatus[nDlg] = new CAxisServoStatus(this,(CWnd**)&m_pAxisServoStatus[nDlg],rr.left,rr.top,nAxis);
			m_pAxisServoStatus[nDlg]->Create(CAxisServoStatus::IDD, this);	
			nDlg++;
			rr.left+=55;
		}
	}

	

	m_comboBrakeMode.AddString(_T("Manual"));
	m_comboBrakeMode.AddString(_T("Automatic"));

	m_comboBrakeMode1.AddString(_T("Manual"));
	m_comboBrakeMode1.AddString(_T("Automatic"));

/*	m_comboGateAction.AddString(_T("No Action"));
	m_comboGateAction.AddString(_T("Stop at end of line"));
	m_comboGateAction.AddString(_T("20% speed"));

	m_comboDoorAction.AddString(_T("No Action"));
	m_comboDoorAction.AddString(_T("Stop at end of line"));
	m_comboDoorAction.AddString(_T("20% speed"));
*/
	
	UpdateControls();
	SetTimer(1,500,NULL);
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAxesPmacStatus::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}


void CAxesPmacStatus::UpdateControls()
{
	CString Buff;

	if(GetSafeHwnd() == NULL) return;

	m_comboBrakeMode.SetCurSel(theApp.m_LastSettings.nBrakeScannerMode);
	m_comboBrakeMode1.SetCurSel(theApp.m_LastSettings.nBrakeHoldingMode);

	m_checkBrakeInterlocks.SetCheck(theApp.m_Tank.nBrakeInterlockPLCEnable);
	m_checkManipulatorInterlocks.SetCheck(theApp.m_Tank.nManipulatorInterlockPLCEnable);

//	m_comboGateAction.SetCurSel(theApp.m_LastSettings.nGateAction);
//	m_comboDoorAction.SetCurSel(theApp.m_LastSettings.nDoorAction);
}


void CAxesPmacStatus::OnMove(int x, int y) 
{
	CPropertyPage::OnMove(x, y);
	CRect rr;
	
	GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.left+=90;
	rr.top+=0;

	if(GetSafeHwnd()) {
		for(int ii = 0;ii<32;ii++) {
			if(m_pAxisServoStatus[ii]) m_pAxisServoStatus[ii]->SetWindowPos(NULL,rr.left,rr.top,0,0,SWP_NOSIZE);
			rr.left+=55;
		}
	}
}


void CAxesPmacStatus::OnTimer(UINT nIDEvent) 
{
	CString Buff;

	Buff.Format(_T("Desired coordinates\r\nX:%d\r\nY:%d\r\nZ:%d\r\nXt:%d\r\nYt:%d"),
		theApp.m_Axes[0].nDesirePos,theApp.m_Axes[1].nDesirePos,theApp.m_Axes[2].nDesirePos,theApp.m_Axes[3].nDesirePos,theApp.m_Axes[4].nDesirePos);
	m_editScratch.SetWindowText(Buff);

	if(theApp.m_Motors.GetBrakeStatus(0) == 0) {
		Buff.Format(_T("Brake is unpowered and locked"));
	} else {
		Buff.Format(_T("Brake is powered and unlocked"));
	}
	m_editScannerBrakes.SetWindowText(Buff);

	if(theApp.m_Motors.GetBrakeStatus(1) == 0) {
		Buff.Format(_T("Brake is unpowered and locked"));
	} else {
		Buff.Format(_T("Brake is powered and unlocked"));
	}
	m_editHoldingBrakes.SetWindowText(Buff);
	
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CAxesPmacStatus::OnSetActive() 
{
	m_nGateStatus=-1;
	
	return CPropertyPage::OnSetActive();
}

void CAxesPmacStatus::OnSelchangeComboGateAction() 
{
	theApp.m_LastSettings.nGateAction = m_comboGateAction.GetCurSel();
}

void CAxesPmacStatus::OnSelchangeComboDoorAction() 
{
	theApp.m_LastSettings.nDoorAction = m_comboDoorAction.GetCurSel();
}

void CAxesPmacStatus::OnSelchangeComboZbrakeMode() 
{
	theApp.m_LastSettings.nBrakeScannerMode = m_comboBrakeMode.GetCurSel();

	if(theApp.m_LastSettings.nBrakeScannerMode==0){ //manual
		m_checkScannerBrakeOnOff.EnableWindow(true);
	}else{
		m_checkScannerBrakeOnOff.EnableWindow(false);
	}
}

void CAxesPmacStatus::OnSelchangeComboZbrakeMode2() 
{
	theApp.m_LastSettings.nBrakeHoldingMode = m_comboBrakeMode1.GetCurSel();
}


void CAxesPmacStatus::OnCheckPlcBrakeInterlocks() 
{
	theApp.m_Tank.nBrakeInterlockPLCEnable = m_checkBrakeInterlocks.GetCheck();
	theApp.m_PmacUser.SetPLCInterlocks();
	
}

void CAxesPmacStatus::OnCheckPlcManipulatorSensors() 
{
	theApp.m_Tank.nManipulatorInterlockPLCEnable = m_checkManipulatorInterlocks.GetCheck();
	theApp.m_PmacUser.SetPLCInterlocks();
}



void CAxesPmacStatus::OnCheckManualScannerBrake() 
{
	int n = m_checkScannerBrakeOnOff.GetCheck();

	switch(n){
	case 0:
		theApp.m_Motors.ZBrakesEnable(TRUE,0);
		break;
	case 1:
		theApp.m_Motors.ZBrakesEnable(FALSE,0);
		break;
	}
	
}
