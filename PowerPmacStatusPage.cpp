// PowerPmacStatusPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PowerPmacStatusPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPowerPmacStatusPage property page

IMPLEMENT_DYNCREATE(CPowerPmacStatusPage, CPropertyPage)

CPowerPmacStatusPage::CPowerPmacStatusPage() : CPropertyPage(CPowerPmacStatusPage::IDD)
{
	//{{AFX_DATA_INIT(CPowerPmacStatusPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPowerPmacStatusPage::~CPowerPmacStatusPage()
{
}

void CPowerPmacStatusPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPowerPmacStatusPage)
	DDX_Control(pDX, IDC_EDIT_TS, m_editTS);
	DDX_Control(pDX, IDC_EDIT_TM, m_editTM);
	DDX_Control(pDX, IDC_EDIT_TA, m_editTA);
	DDX_Control(pDX, IDC_CHECK_PROG_ACTIVE_3, m_checkProgActive3);
	DDX_Control(pDX, IDC_CHECK_PROG_ACTIVE_2, m_checkProgActive2);
	DDX_Control(pDX, IDC_CHECK_PROG_ACTIVE_1, m_checkProgActive1);
	DDX_Control(pDX, IDC_CHECK_TIMERS_ENABLED_3, m_checkTimersEnabled3);
	DDX_Control(pDX, IDC_CHECK_TIMERS_ENABLED_2, m_checkTimersEnabled2);
	DDX_Control(pDX, IDC_CHECK_TIMERS_ENABLED_1, m_checkTimersEnabled1);
	DDX_Control(pDX, IDC_CHECK_MOVE_TIMER_3, m_checkMoveTimer3);
	DDX_Control(pDX, IDC_CHECK_MOVE_TIMER_2, m_checkMoveTimer2);
	DDX_Control(pDX, IDC_CHECK_MOVE_TIMER_1, m_checkMoveTimer1);
	DDX_Control(pDX, IDC_CHECK_LOOKAHEAD_3, m_checkLookAhead3);
	DDX_Control(pDX, IDC_CHECK_LOOKAHEAD_2, m_checkLookAhead2);
	DDX_Control(pDX, IDC_CHECK_LOOKAHEAD_1, m_checkLookAhead1);
	DDX_Control(pDX, IDC_CHECK_IN_POS_12, m_checkInPos3);
	DDX_Control(pDX, IDC_CHECK_IN_POS_11, m_checkInPos2);
	DDX_Control(pDX, IDC_CHECK_IN_POS_1, m_checkInPos1);
	DDX_Control(pDX, IDC_CHECK_DES_VEL_ZERO_3, m_checkDesVelZero3);
	DDX_Control(pDX, IDC_CHECK_DES_VEL_ZERO_2, m_checkDesVelZero2);
	DDX_Control(pDX, IDC_CHECK_DES_VEL_ZERO_1, m_checkDesVelZero1);
	DDX_Control(pDX, IDC_CHECK_AMP_ENABLED_3, m_checkAmpEnabled3);
	DDX_Control(pDX, IDC_CHECK_AMP_ENABLED_2, m_checkAmpEnabled2);
	DDX_Control(pDX, IDC_CHECK_AMP_ENABLED_1, m_checkAmpEnabled1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_SPEED_MOVE, m_editMoveSpeed);
	DDX_Control(pDX, IDC_EDIT_SPEED_NORMALIZATION, m_editNormalizationSpeed);
	DDX_Control(pDX, IDC_EDIT_SPEED_ORTHOGONAL, m_editOrthogonalSpeed);
	DDX_Control(pDX, IDC_SPIN_SPEED_MOVE, m_spinMoveSpeed);
	DDX_Control(pDX, IDC_SPIN_SPEED_NORMALIZATION, m_spinNormalizationSpeed);
	DDX_Control(pDX, IDC_SPIN_SPEED_ORTHOGONAL, m_spinOrthogonalSpeed);
	DDX_Control(pDX, IDC_EDIT_AXIS_ENABLE_MASK_1, m_editAxisEnableMask1);
	DDX_Control(pDX, IDC_EDIT_AXIS_ENABLE_MASK_2, m_editAxisEnableMask2);
}


BEGIN_MESSAGE_MAP(CPowerPmacStatusPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPowerPmacStatusPage)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_SPEED_MOVE, &CPowerPmacStatusPage::OnEnChangeEditSpeedMove)
	ON_EN_CHANGE(IDC_EDIT_SPEED_NORMALIZATION, &CPowerPmacStatusPage::OnEnChangeEditSpeedNormalization)
	ON_EN_CHANGE(IDC_EDIT_SPEED_ORTHOGONAL, &CPowerPmacStatusPage::OnEnChangeEditSpeedOrthogonal)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED_MOVE, &CPowerPmacStatusPage::OnDeltaposSpinSpeedMove)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED_NORMALIZATION, &CPowerPmacStatusPage::OnDeltaposSpinSpeedNormalization)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED_ORTHOGONAL, &CPowerPmacStatusPage::OnDeltaposSpinSpeedOrthogonal)
	ON_EN_CHANGE(IDC_EDIT_AXIS_ENABLE_MASK_1, &CPowerPmacStatusPage::OnEnChangeEditAxisEnableMask1)
	ON_EN_CHANGE(IDC_EDIT_AXIS_ENABLE_MASK_2, &CPowerPmacStatusPage::OnEnChangeEditAxisEnableMask2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPowerPmacStatusPage message handlers

BOOL CPowerPmacStatusPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
/*
	m_spinSpeedJoyX.SetRange(-100, 100);
	m_spinSpeedJoyY.SetRange(-100, 100);
	m_spinSpeedJoyZ.SetRange(-100, 100);
	m_spinSpeedJoyXt.SetRange(-100, 100);
	m_spinSpeedJoyYt.SetRange(-100, 100);
	*/
	m_spinOrthogonalSpeed.SetRange(-100, 100);
	m_spinNormalizationSpeed.SetRange(-100, 100);
	m_spinMoveSpeed.SetRange(-100, 100);

	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPowerPmacStatusPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPowerPmacStatusPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	/*
	Buff.Format(L"%.02f mm/s", theApp.m_LastSettings.fJogDirSpeedX[0]);
	m_editSpeedJoyX.SetWindowText(Buff);
	Buff.Format(L"%.02f mm/s", theApp.m_LastSettings.fJogDirSpeedY[0]);
	m_editSpeedJoyY.SetWindowText(Buff);
	Buff.Format(L"%.02f mm/s", theApp.m_LastSettings.fJogDirSpeedZ[0]);
	m_editSpeedJoyZ.SetWindowText(Buff);
	Buff.Format(L"%.02f %s/s", theApp.m_LastSettings.fJogDirSpeedHTip[0], DEGREES);
	m_editSpeedJoyXt.SetWindowText(Buff);
	Buff.Format(L"%.02f %s/s", theApp.m_LastSettings.fJogDirSpeedVTip[0], DEGREES);
	m_editSpeedJoyYt.SetWindowText(Buff);
	*/
	Buff.Format(L"%.02f mm/s", theApp.m_LastSettings.fSpeedOrthogonal[0]);
	m_editOrthogonalSpeed.SetWindowText(Buff);
	Buff.Format(L"%.02f %s/s", theApp.m_LastSettings.fSpeedNormalize[0], DEGREES);
	m_editNormalizationSpeed.SetWindowText(Buff);
	Buff.Format(L"%.02f mm/s", theApp.m_LastSettings.fMoveSpeed);
	m_editMoveSpeed.SetWindowText(Buff);

	Buff.Format(L"%04X h", theApp.m_Tank.nAxisEnableMask[1]);
	m_editAxisEnableMask1.SetWindowTextW(Buff);
	Buff.Format(L"%04X h", theApp.m_Tank.nAxisEnableMask[2]);
	m_editAxisEnableMask2.SetWindowTextW(Buff);
	/*
	Buff.Format(L"X:%.01f Y:%.01f Z:%.01f i:%.03f j:%.03f k:%.03f", theApp.m_Tank.vParkPt[0].x, theApp.m_Tank.vParkPt[0].y, theApp.m_Tank.vParkPt[0].z,
		theApp.m_Tank.vParkNorm[0].x, theApp.m_Tank.vParkNorm[0].y, theApp.m_Tank.vParkNorm[0].z);
	m_editParkPos.SetWindowText(Buff);
	*/
}

BOOL CPowerPmacStatusPage::OnSetActive() 
{
	m_fTMMove[0] = -11111111111.0f;
	m_fTAMove[0] = -11111111111.0f;
	m_fTSMove[0] = -11111111111.0f;

	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1,200,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CPowerPmacStatusPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CPowerPmacStatusPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;

	if(GetSafeHwnd() != NULL) {
		if(m_n64CoordStatus[0] - theApp.m_FBCtrl.m_n64CoordStatus[0]) {
			m_n64CoordStatus[0] = theApp.m_FBCtrl.m_n64CoordStatus[0];
			m_n64CoordStatus[0] & _PP_AMP_ENABLED ?		m_checkAmpEnabled1.SetCheck(true) :		m_checkAmpEnabled1.SetCheck(false);
			m_n64CoordStatus[0] & _PP_MOVETIMER ?		m_checkMoveTimer1.SetCheck(true) :		m_checkMoveTimer1.SetCheck(false);
			m_n64CoordStatus[0] & _PP_TIMERSENABLED ?	m_checkTimersEnabled1.SetCheck(true) :	m_checkTimersEnabled1.SetCheck(false);
			m_n64CoordStatus[0] & _PP_LOOKAHEAD ?		m_checkLookAhead1.SetCheck(true) :		m_checkLookAhead1.SetCheck(false);
			m_n64CoordStatus[0] & _PP_INPOS ?			m_checkInPos1.SetCheck(true) :			m_checkInPos1.SetCheck(false);
			m_n64CoordStatus[0] & _PP_DESVELZERO ?		m_checkDesVelZero1.SetCheck(true) :		m_checkDesVelZero1.SetCheck(false);
			theApp.m_FBCtrl.m_nProgActive[0] & 1 ?		m_checkProgActive1.SetCheck(true) :		m_checkProgActive1.SetCheck(false);
		}
		if(m_n64CoordStatus[1] - theApp.m_FBCtrl.m_n64CoordStatus[1]) {
			m_n64CoordStatus[1] = theApp.m_FBCtrl.m_n64CoordStatus[1];
			m_n64CoordStatus[1] & _PP_AMP_ENABLED ?		m_checkAmpEnabled2.SetCheck(true) :		m_checkAmpEnabled2.SetCheck(false);
			m_n64CoordStatus[1] & _PP_MOVETIMER ?		m_checkMoveTimer2.SetCheck(true) :		m_checkMoveTimer2.SetCheck(false);
			m_n64CoordStatus[1] & _PP_TIMERSENABLED ?	m_checkTimersEnabled2.SetCheck(true) :	m_checkTimersEnabled2.SetCheck(false);
			m_n64CoordStatus[1] & _PP_LOOKAHEAD ?		m_checkLookAhead2.SetCheck(true) :		m_checkLookAhead2.SetCheck(false);
			m_n64CoordStatus[1] & _PP_INPOS ?			m_checkInPos2.SetCheck(true) :			m_checkInPos2.SetCheck(false);
			m_n64CoordStatus[1] & _PP_DESVELZERO ?		m_checkDesVelZero2.SetCheck(true) :		m_checkDesVelZero2.SetCheck(false);
			theApp.m_FBCtrl.m_nProgActive[1] & 1 ?		m_checkProgActive2.SetCheck(true) :		m_checkProgActive2.SetCheck(false);
		}
		if(m_n64CoordStatus[2] - theApp.m_FBCtrl.m_n64CoordStatus[2]) {
			m_n64CoordStatus[2] = theApp.m_FBCtrl.m_n64CoordStatus[2];
			m_n64CoordStatus[2] & _PP_AMP_ENABLED ?		m_checkAmpEnabled3.SetCheck(true) :		m_checkAmpEnabled3.SetCheck(false);
			m_n64CoordStatus[2] & _PP_MOVETIMER ?		m_checkMoveTimer3.SetCheck(true) :		m_checkMoveTimer3.SetCheck(false);
			m_n64CoordStatus[2] & _PP_TIMERSENABLED ?	m_checkTimersEnabled3.SetCheck(true) :	m_checkTimersEnabled3.SetCheck(false);
			m_n64CoordStatus[2] & _PP_LOOKAHEAD ?		m_checkLookAhead3.SetCheck(true) :		m_checkLookAhead3.SetCheck(false);
			m_n64CoordStatus[2] & _PP_INPOS ?			m_checkInPos3.SetCheck(true) :			m_checkInPos3.SetCheck(false);
			m_n64CoordStatus[2] & _PP_DESVELZERO ?		m_checkDesVelZero3.SetCheck(true) :		m_checkDesVelZero3.SetCheck(false);
			theApp.m_FBCtrl.m_nProgActive[2] & 1 ?		m_checkProgActive3.SetCheck(true) :		m_checkProgActive3.SetCheck(false);
		}
		if(m_fTMMove[0] - theApp.m_FBCtrl.m_fTMMove[0]) {
			m_fTMMove[0] = theApp.m_FBCtrl.m_fTMMove[0];
			Buff.Format(L"%.03f s", m_fTMMove[0] / 1000.0f);
			m_editTM.SetWindowText(Buff);
		}
		if(m_fTAMove[0] - theApp.m_FBCtrl.m_fTAMove[0]) {
			m_fTAMove[0] = theApp.m_FBCtrl.m_fTAMove[0];
			Buff.Format(L"%.03f s", m_fTAMove[0] / 1000.0f);
			m_editTA.SetWindowText(Buff);
		}
		if(m_fTSMove[0] - theApp.m_FBCtrl.m_fTSMove[0]) {
			m_fTSMove[0] = theApp.m_FBCtrl.m_fTSMove[0];
			Buff.Format(L"%.03f s", m_fTSMove[0] / 1000.0f);
			m_editTS.SetWindowText(Buff);
		}

	}
	
	CPropertyPage::OnTimer(nIDEvent);
}


void CPowerPmacStatusPage::OnEnChangeEditSpeedMove()
{
	CString Buff;

	m_editMoveSpeed.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_LastSettings.fMoveSpeed);
}


void CPowerPmacStatusPage::OnEnChangeEditSpeedNormalization()
{
	CString Buff;

	m_editNormalizationSpeed.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_LastSettings.fSpeedNormalize[0]);
}


void CPowerPmacStatusPage::OnEnChangeEditSpeedOrthogonal()
{
	CString Buff;

	m_editOrthogonalSpeed.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_LastSettings.fSpeedOrthogonal[0]);
}


void CPowerPmacStatusPage::OnDeltaposSpinSpeedMove(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_LastSettings.fMoveSpeed += (float)pNMUpDown->iDelta * 0.01f;
	UpdateAllControls();

	*pResult = 0;
}


void CPowerPmacStatusPage::OnDeltaposSpinSpeedNormalization(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_LastSettings.fSpeedNormalize[0] += (float)pNMUpDown->iDelta * 0.01f;
	UpdateAllControls();

	*pResult = 0;
}


void CPowerPmacStatusPage::OnDeltaposSpinSpeedOrthogonal(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_LastSettings.fSpeedOrthogonal[0] += (float)pNMUpDown->iDelta * 0.01f;
	UpdateAllControls();

	*pResult = 0;
}


void CPowerPmacStatusPage::OnEnChangeEditAxisEnableMask1()
{
	CString Buff;

	m_editAxisEnableMask1.GetWindowTextW(Buff);
	swscanf_s(Buff, L"%x", &theApp.m_Tank.nAxisEnableMask[1]);
}


void CPowerPmacStatusPage::OnEnChangeEditAxisEnableMask2()
{
	CString Buff;

	m_editAxisEnableMask2.GetWindowTextW(Buff);
	swscanf_s(Buff, L"%x", &theApp.m_Tank.nAxisEnableMask[2]);
}
