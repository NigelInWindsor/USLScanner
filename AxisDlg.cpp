// Axes.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "Si6.h"
#include "AxisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisDlg property page

IMPLEMENT_DYNCREATE(CAxisDlg, CPropertyPage)

CAxisDlg::CAxisDlg(UINT nCap,CWnd* pParent,CWnd** DlgPtr,int nWhichAxis,int nTab) 
	: CPropertyPage(CAxisDlg::IDD,nCap)
{
	//{{AFX_DATA_INIT(CAxisDlg)
	m_stringPos = _T("");
	m_stringEncoderPos = _T("");
	m_stringCount = _T("");
	m_stringCurrentSpeed = _T("");
	m_stringJoyValue = _T("");
	//}}AFX_DATA_INIT

	m_pSi6User = &theApp.m_Si6User;
	m_pAxis = &theApp.m_Axes[nWhichAxis];
	m_nCap = nCap;
	m_nWhichAxis = nWhichAxis;
	m_DlgPtr = DlgPtr;
	m_nTab = nTab;
	
}

CAxisDlg::~CAxisDlg()
{
}


void CAxisDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxisDlg)
	DDX_Control(pDX, IDC_EDIT_KINEMATIC_OFFSET, m_editKinematicOffset);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_USE_FIDUCIAL, m_checkUseFiducial);
	DDX_Control(pDX, IDC_CHECK_HOME_SWITCH, m_checkHomeSwitch);
	DDX_Control(pDX, IDC_CHECK_MIN_SWITCH, m_checkMinSwitch);
	DDX_Control(pDX, IDC_CHECK_MAX_SWITCH, m_checkMaxSwitch);
	DDX_Control(pDX, IDC_EDIT_NUMBER_DP, m_editPrecision);
	DDX_Control(pDX, IDC_EDIT_PARK_POS, m_editParkPos);
	DDX_Control(pDX, IDC_CHECK_JOY_AVAILABLE, m_JoystickAvailable);
	DDX_Control(pDX, IDC_COMBO_TOGGLE_POS, m_comboTogglePos);
	DDX_Control(pDX, IDC_EDIT_JOY_MIN_DEV, m_editJoyMinDeviation);
	DDX_Control(pDX, IDC_EDIT_JOY_MAX_DEV, m_editJoyMaxDeviation);
	DDX_Control(pDX, IDC_EDIT_MOTOR_RATIO, m_editMotorRatio);
	DDX_Control(pDX, IDC_EDIT_IN_POS_ERR, m_editInPosError);
	DDX_Control(pDX, IDC_COMBO_ENCODER_MODE, m_comboEncoderMode);
	DDX_Control(pDX, IDC_EDIT_ENCODER_NUMBER, m_editEncoderNumber);
	DDX_Control(pDX, IDC_EDIT_HOME_OFFSET, m_editHomeOffset);
	DDX_Control(pDX, IDC_EDIT_STEPSREV, m_editStepsRev);
	DDX_Control(pDX, IDC_EDIT_MIN_TRAVEL, m_editMinTravel);
	DDX_Control(pDX, IDC_EDIT_MAX_TRAVEL, m_editMaxTravel);
	DDX_Control(pDX, IDC_CHECK_MOTOR_REVERSE, m_MotorReverse);
	DDX_Control(pDX, IDC_CHECK_ENCODER_REVERSE, m_EncoderReverse);
	DDX_Control(pDX, IDC_EDIT_MIN_SWITCH, m_editMinSwitch);
	DDX_Control(pDX, IDC_EDIT_MAX_SWITCH, m_editMaxSwitch);
	DDX_Control(pDX, IDC_EDIT_HOME_SWITCH, m_editHomeSwitch);
	DDX_Control(pDX, IDC_COMBO_HOME_MODE, m_comboHomeMode);
	DDX_Control(pDX, IDC_CHECK_JOYSTICK_REVERSE, m_JoystickReverse);
	DDX_Control(pDX, IDC_AXIS_AVAILABLE_CHECK, m_AxisAvailableCheck);
	DDX_Control(pDX, IDC_AXIS_JOYSTICK_MIN_SPEED, m_AxisJoystickMinSpeedEdit);
	DDX_Control(pDX, IDC_AXIS_UNITS_COMBO, m_AxisUnitsCombo);
	DDX_Control(pDX, IDC_AXIS_ACCELERATION, m_AxisAccelerationEdit);
	DDX_Control(pDX, IDC_AXIS_BIT, m_AxisBitEdit);
	DDX_Control(pDX, IDC_AXIS_COUNTER, m_AxisCounterEdit);
	DDX_Control(pDX, IDC_AXIS_DECELERATION, m_AxisDecelerationEdit);
	DDX_Control(pDX, IDC_AXIS_HOME_SPEED, m_AxisHomeSpeedEdit);
	DDX_Control(pDX, IDC_AXIS_JOYSTICK_GAIN, m_AxisJoystickGainEdit);
	DDX_Control(pDX, IDC_AXIS_JOYSTICK_MAX_SPEED, m_AxisJoystickMaxSpeedEdit);
	DDX_Control(pDX, IDC_AXIS_JOYSTICK_NUMBER, m_AxisJoystickNumberEdit);
	DDX_Control(pDX, IDC_AXIS_MAX_SPEED, m_AxisMaxSpeedEdit);
	DDX_Control(pDX, IDC_AXIS_MIN_SPEED, m_AxisMinSpeedEdit);
	DDX_Control(pDX, IDC_AXIS_STEPSIZE, m_AxisStepsizeEdit);
	DDX_Text(pDX, IDC_EDIT_POS, m_stringPos);
	DDX_Text(pDX, IDC_EDIT_ENCODER_POS, m_stringEncoderPos);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_stringCount);
	DDX_Text(pDX, IDC_EDIT_CURRENT_SPEED, m_stringCurrentSpeed);
	DDX_Text(pDX, IDC_EDIT_JOY_VALUE, m_stringJoyValue);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CAxisDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CAxisDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ACCEPT_BUTTON, OnAcceptButton)
	ON_BN_CLICKED(IDC_AXIS_AVAILABLE_CHECK, OnAxisAvailableCheck)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_BN_CLICKED(IDC_CHECK_JOYSTICK_REVERSE, OnCheckJoystickReverse)
	ON_BN_CLICKED(IDC_CHECK_MOTOR_REVERSE, OnCheckMotorReverse)
	ON_BN_CLICKED(IDC_CHECK_ENCODER_REVERSE, OnCheckEncoderReverse)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_ENCODER_MODE, OnSelchangeComboEncoderMode)
	ON_EN_CHANGE(IDC_EDIT_JOY_MIN_DEV, OnChangeEditJoyMinDev)
	ON_EN_CHANGE(IDC_EDIT_JOY_MAX_DEV, OnChangeEditJoyMaxDev)
	ON_BN_CLICKED(IDC_CHECK_JOY_AVAILABLE, OnCheckJoyAvailable)
	ON_BN_CLICKED(IDC_BUTTON_HMZ, OnButtonHmz)
	ON_BN_CLICKED(IDC_CHECK_USE_FIDUCIAL, OnCheckUseFiducial)
	ON_BN_CLICKED(IDC_BUTTON_LIMIT_SWITCH_OFFSET, OnButtonLimitSwitchOffset)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_EN_CHANGE(IDC_EDIT_KINEMATIC_OFFSET, OnChangeEditKinematicOffset)
	//}}AFX_MSG_MAP
   ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
   ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisDlg message handlers
/////////////////////////////////////////////////////////////////////////////


BOOL CAxisDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_AxisUnitsCombo.AddString(_T("mm"));
	m_AxisUnitsCombo.AddString(_T("\x022"));
	m_AxisUnitsCombo.AddString(DEGREES);

	m_comboTogglePos.AddString(_T("Up"));
	m_comboTogglePos.AddString(_T("Middle"));
	m_comboTogglePos.AddString(_T("Bottom"));
	m_comboTogglePos.AddString(_T("Any"));

	
	UpdateAllControls();
	UpdateData(FALSE);


	EnableToolTips();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAxisDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CAxisDlg::UpdateAllControls()
{
	CString Buff;
	if (GetSafeHwnd() == NULL) return;

	m_AxisAvailableCheck.SetCheck(m_pAxis->bAvailable);
	m_JoystickReverse.SetCheck(m_pAxis->bJoystickReverse);
	m_JoystickAvailable.SetCheck(m_pAxis->bJoystickAvailable);
	m_MotorReverse.SetCheck(m_pAxis->bDirectionReverse);
	m_EncoderReverse.SetCheck(m_pAxis->bEncoderReverse);
	m_checkUseFiducial.SetCheck(m_pAxis->bUseFiducial);


	m_comboTogglePos.SetCurSel(m_pAxis->nJoystickSwitchPos);
	m_comboHomeMode.SetCurSel(m_pAxis->nHomeMode);
	m_AxisUnitsCombo.SetCurSel(m_pAxis->nUnitSelected);

	Buff.Format(_T("%02xh"),m_pAxis->nBit);
	m_AxisBitEdit.SetWindowText(Buff);
	Buff.Format(_T("%05xh"),m_pAxis->nHomeSwitch);
	m_editHomeSwitch.SetWindowText(Buff);
	Buff.Format(_T("%05xh"),m_pAxis->nMinSwitch);
	m_editMinSwitch.SetWindowText(Buff);
	Buff.Format(_T("%05xh"),m_pAxis->nMaxSwitch);
	m_editMaxSwitch.SetWindowText(Buff);
	Buff.Format(_T("%.01f"),m_pAxis->fStepSize * (float)m_pAxis->nMaxTravel);
	m_editMaxTravel.SetWindowText(Buff);
	Buff.Format(_T("%.01f"),m_pAxis->fStepSize * (float)m_pAxis->nMinTravel);
	m_editMinTravel.SetWindowText(Buff);
	Buff.Format(_T("%d"),m_pAxis->nJoystickMinValue);
	m_editJoyMinDeviation.SetWindowText(Buff);
	Buff.Format(_T("%d"),m_pAxis->nJoystickMaxValue);
	m_editJoyMaxDeviation.SetWindowText(Buff);
	Buff.Format(_T("%.06f"),m_pAxis->fStepSize);
	m_AxisStepsizeEdit.SetWindowText(Buff);
	Buff.Format(_T("%d Steps/s"),m_pAxis->nMinSpeed);
	m_AxisMinSpeedEdit.SetWindowText(Buff);
	Buff.Format(_T("%d Steps/s"),m_pAxis->nMaxSpeed);
	m_AxisMaxSpeedEdit.SetWindowText(Buff);
	Buff.Format(_T("%d Steps/s"),m_pAxis->nHomeSpeed);
	m_AxisHomeSpeedEdit.SetWindowText(Buff);
	Buff.Format(_T("%d Steps/s"),m_pAxis->nJoystickMinSpeed);
	m_AxisJoystickMinSpeedEdit.SetWindowText(Buff);
	Buff.Format(_T("%d Steps/s"),m_pAxis->nJoystickMaxSpeed);
	m_AxisJoystickMaxSpeedEdit.SetWindowText(Buff);
	Buff.Format(_T("%.01f Steps"),m_pAxis->dStepsPerRev);
	m_editStepsRev.SetWindowText(Buff);
	Buff.Format(_T("%.03fmm"),(float)m_pAxis->nPositionError * m_pAxis->fStepSize);
	m_editInPosError.SetWindowText(Buff);
	Buff.Format(_T("%.05f"),m_pAxis->fEncMotorRatio);
	m_editMotorRatio.SetWindowText(Buff);
	Buff.Format(_T("%d"),m_pAxis->nCounter);
	m_AxisCounterEdit.SetWindowText(Buff);
	Buff.Format(_T("%d"),m_pAxis->nEncoderNumber);
	m_editEncoderNumber.SetWindowText(Buff);
	Buff.Format(_T("%d"),m_pAxis->nAcceleration);
	m_AxisAccelerationEdit.SetWindowText(Buff);
	Buff.Format(_T("%d"),m_pAxis->nDeceleration);
	m_AxisDecelerationEdit.SetWindowText(Buff);
	Buff.Format(_T("%%%d"),m_pAxis->nJoystickMinValue);
	m_AxisJoystickGainEdit.SetWindowText(Buff);
	Buff.Format(_T("%d"),m_pAxis->nJoystickNumber);
	m_AxisJoystickNumberEdit.SetWindowText(Buff);
	Buff.Format(_T("%d"),m_pAxis->nLimitSwitchOffset);
	m_editHomeOffset.SetWindowText(Buff);

	if(m_pAxis->nEncoderMode==0) {
		m_comboEncoderMode.SetCurSel(0);
	} else {
		if(m_pAxis->nEncoderResetMode==FALSE) {
			m_comboEncoderMode.SetCurSel(1);
		} else {
			m_comboEncoderMode.SetCurSel(2);
		};
	};

	Buff.Format(_T("%.1f mm"),m_pAxis->fParkPosition);
	m_editParkPos.SetWindowText(Buff);

	Buff.Format(_T("%d"),m_pAxis->nPrecision);
	m_editPrecision.SetWindowText(Buff);

	Buff.Format(L"%s",m_pAxis->cName);
	m_editName.SetWindowText(Buff);

	Buff.Format(_T("%d"),m_pAxis->nOffset);
	m_editKinematicOffset.SetWindowText(Buff);

}

void CAxisDlg::Defaults(int nWhich)
{
	int	mask=1;

	memset(m_pAxis,0,sizeof AxisData);
	m_pAxis->fStepSize = 0.1f;
	m_pAxis->nBit = mask<<nWhich;
	m_pAxis->nCounter=nWhich;
	m_pAxis->nMinSpeed = 200;
	m_pAxis->nMaxSpeed = 2000;
	m_pAxis->nAcceleration=2;
	m_pAxis->nDeceleration=2;
	m_pAxis->nHomeSpeed=-400;
	m_pAxis->nJoystickMinSpeed=50;
	m_pAxis->nJoystickMaxSpeed=1000;
	m_pAxis->nJoystickMinValue=100;
	m_pAxis->nJoystickMaxValue=150;
	m_pAxis->nJoystickNumber=nWhich%3;
	m_pAxis->nHomeMode=0;
	m_pAxis->nHomeSwitch=0;
	m_pAxis->nMinSwitch=0;
	m_pAxis->nMaxSwitch=0;
	m_pAxis->nMinTravel=0;
	m_pAxis->nMaxTravel=0;
	m_pAxis->bAvailable=TRUE;
	m_pAxis->nPrecision=1;

	
}

void CAxisDlg::OnAcceptButton() 
{
	CString Buff;
	int	nn;
	float fTemp;
	int nMask = 1;

	m_AxisStepsizeEdit.GetWindowText(Buff);
	_WTOF(Buff,m_pAxis->fStepSize);

	m_AxisMinSpeedEdit.GetWindowText(Buff);
	m_pAxis->nMinSpeed=_ttoi(Buff);
	MinMax(&m_pAxis->nMinSpeed,0,10000);

	m_AxisMaxSpeedEdit.GetWindowText(Buff);
	m_pAxis->nMaxSpeed=_ttoi(Buff);
	MinMax(&m_pAxis->nMaxSpeed,0,10000);

	m_AxisJoystickMinSpeedEdit.GetWindowText(Buff);
	m_pAxis->nJoystickMinSpeed=_ttoi(Buff);
	MinMax(&m_pAxis->nJoystickMinSpeed,0,10000);

	m_AxisHomeSpeedEdit.GetWindowText(Buff);
	m_pAxis->nHomeSpeed=_ttoi(Buff);
	MinMax(&m_pAxis->nHomeSpeed,-10000,10000);

	m_AxisJoystickMaxSpeedEdit.GetWindowText(Buff);
	m_pAxis->nJoystickMaxSpeed=_ttoi(Buff);
	MinMax(&m_pAxis->nJoystickMaxSpeed,0,10000);

	m_editInPosError.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	m_pAxis->nPositionError=(int)(fTemp / m_pAxis->fStepSize);


	m_editMotorRatio.GetWindowText(Buff);
	_WTOF(Buff,m_pAxis->fEncMotorRatio);
	m_pAxis->nEncMotorRatioNumerator=100;
	m_pAxis->nEncMotorRatioDenominator=(int)(m_pAxis->fEncMotorRatio * 100.0);

	m_AxisAccelerationEdit.GetWindowText(Buff);
	m_pAxis->nAcceleration=_ttoi(Buff);

	m_AxisDecelerationEdit.GetWindowText(Buff);
	m_pAxis->nDeceleration=_ttoi(Buff);


	m_AxisCounterEdit.GetWindowText(Buff);
	m_pAxis->nCounter=_ttoi(Buff);
	m_pAxis->nIrqMask=nMask<<m_pAxis->nCounter;

	m_editEncoderNumber.GetWindowText(Buff);
	m_pAxis->nEncoderNumber=_ttoi(Buff);

	m_editStepsRev.GetWindowText(Buff);
	_WTOD(Buff,m_pAxis->dStepsPerRev);

	m_AxisJoystickNumberEdit.GetWindowText(Buff);
	nn=_ttoi(Buff);
	nn%=4;
	m_pAxis->nJoystickNumber=nn;

	m_AxisBitEdit.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&m_pAxis->nBit);


	m_editHomeSwitch.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&m_pAxis->nHomeSwitch);

	m_editMinSwitch.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&m_pAxis->nMinSwitch);

	m_editMaxSwitch.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&m_pAxis->nMaxSwitch);

	if(m_pAxis->fStepSize > 0.0001) {
		m_editMaxTravel.GetWindowText(Buff);
		_WTOF(Buff,fTemp);
		m_pAxis->nMaxTravel=(int)(fTemp / m_pAxis->fStepSize);

		m_editMinTravel.GetWindowText(Buff);
		_WTOF(Buff,fTemp);
		m_pAxis->nMinTravel=(int)(fTemp / m_pAxis->fStepSize);
	};

	m_pAxis->nHomeMode=m_comboHomeMode.GetCurSel();

	m_editJoyMinDeviation.GetWindowText(Buff);
	m_pAxis->nJoystickMinValue=_ttoi(Buff);

	m_editJoyMaxDeviation.GetWindowText(Buff);
	m_pAxis->nJoystickMaxValue=_ttoi(Buff);

	m_pAxis->nJoystickSwitchPos = m_comboTogglePos.GetCurSel();	

	m_pAxis->nUnitSelected = m_AxisUnitsCombo.GetCurSel();

	m_pAxis->fMaxMoveSpeed = m_pAxis->fStepSize * (float)m_pAxis->nMaxSpeed;
	m_pAxis->fMaxJogSpeed = m_pAxis->fStepSize * (float)m_pAxis->nMaxSpeed;

	m_editParkPos.GetWindowText(Buff);
	_WTOF(Buff,m_pAxis->fParkPosition);

	m_editPrecision.GetWindowText(Buff);
	m_pAxis->nPrecision = _ttoi(Buff);

	m_editHomeOffset.GetWindowText(Buff);
	m_pAxis->nLimitSwitchOffset=_ttoi(Buff);


	theApp.SaveTank();
}


void CAxisDlg::OnAxisAvailableCheck() 
{
	int nn;
	if((nn=m_AxisAvailableCheck.GetState()) & 0x0001) {
		m_pAxis->bAvailable=TRUE;
	} else {
		m_pAxis->bAvailable=FALSE;

	};

}

void CAxisDlg::OnDefaultButton() 
{

	Defaults(m_nTab);
	UpdateAllControls();

}

void CAxisDlg::MinMax(int *v0, int v1, int v2)
{

	if(*v0<v1) *v0=v1;
	if(*v0>v2) *v0=v2;
}

void CAxisDlg::OnCheckJoystickReverse() 
{
	int nn;
	if((nn=m_JoystickReverse.GetState()) & 0x0001) {
		m_pAxis->bJoystickReverse=TRUE;
	} else {
		m_pAxis->bJoystickReverse=FALSE;

	};
	
}

void CAxisDlg::OnCheckMotorReverse() 
{
	int nn;
	if((nn=m_MotorReverse.GetState()) & 0x0001) {
		m_pAxis->bDirectionReverse = TRUE;
	} else {
		m_pAxis->bDirectionReverse = FALSE;
	};
	
}

void CAxisDlg::OnCheckEncoderReverse() 
{
	int nn;
	if((nn=m_EncoderReverse.GetState()) & 0x0001) {
		m_pAxis->bEncoderReverse=TRUE;
	} else {
		m_pAxis->bEncoderReverse=FALSE;
	};
	
}


void CAxisDlg::OnTimer(UINT nIDEvent) 
{
	m_stringPos.Format(_T("%d"),m_pAxis->nPos);
	m_stringCount.Format(_T("%d"),m_pAxis->nCount);
	m_stringEncoderPos.Format(_T("%d"),m_pAxis->nEncoderValue);
	m_stringCurrentSpeed.Format(_T("%d"),m_pAxis->nCurrentSpeed);
	m_stringJoyValue.Format(_T("%d"),m_pAxis->nJoystickValueRead);
//	m_stringJoyValue.Format(_T("%d"),theApp.m_Si10User.InpW(SI10_COUNTER_LATCH_0+m_pAxis->nEncoderNumber*4));

	m_checkMinSwitch.SetCheck((m_pAxis->nFlagMinMax & MIN_SWITCH)>0);
	m_checkMaxSwitch.SetCheck((m_pAxis->nFlagMinMax & MAX_SWITCH)>0);
	m_checkHomeSwitch.SetCheck((m_pAxis->nFlagMinMax & HOME_SWITCH)>0);

	UpdateData(FALSE);
	CPropertyPage::OnTimer(nIDEvent);
}

void CAxisDlg::OnSelchangeComboEncoderMode() 
{
	switch(m_comboEncoderMode.GetCurSel()) {
	case 0:	m_pAxis->nEncoderMode=0;
		break;
	case 1:	m_pAxis->nEncoderMode=1;
			m_pAxis->nEncoderResetMode=FALSE;
			m_pSi6User->EncoderResetMode(m_pAxis);
		break;
	case 2:	m_pAxis->nEncoderMode=1;
			m_pAxis->nEncoderResetMode=TRUE;
			m_pSi6User->EncoderResetMode(m_pAxis);
		break;
	};
	
}

void CAxisDlg::OnChangeEditJoyMinDev() 
{
	
}

void CAxisDlg::OnChangeEditJoyMaxDev() 
{

}

void CAxisDlg::OnCheckJoyAvailable() 
{
	int nn;
	if((nn=m_JoystickAvailable.GetState()) & 0x0001) {
		m_pAxis->bJoystickAvailable=TRUE;
	} else {
		m_pAxis->bJoystickAvailable=FALSE;
	};
	
}

BOOL CAxisDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
   case IDC_EDIT_ENCODER_NUMBER: strTipText = "Range 0 - 5";
	   break;
   case IDC_AXIS_COUNTER: strTipText = "Range 0 - 5";
	   break;
   case IDC_AXIS_BIT: strTipText = "Hex Values only";
	   break;
   case IDC_AXIS_JOYSTICK_NUMBER: strTipText = "Range 0 - 4";
	   break;
   case IDC_EDIT_MIN_TRAVEL:
   case IDC_EDIT_MAX_TRAVEL: strTipText = "Min && Max = 0 Disabled";
	   break;
   }
//      strTipText.Format(_T("Control ID = %d"), nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


void CAxisDlg::OnButtonHmz() 
{
	if(MessageBox(_T("Are you damn sure you want to set this encoder to zero"),_T("Warning"),MB_YESNO)==IDYES) {
		theApp.m_Motors.HoldingFixtureZeroPosition(m_nWhichAxis);	
	}
}

void CAxisDlg::OnCheckUseFiducial() 
{
	int nn;
	if((nn=m_checkUseFiducial.GetState()) & 0x0001) {
		m_pAxis->bUseFiducial = TRUE;
	} else {
		m_pAxis->bUseFiducial = FALSE;
	};
	
}

void CAxisDlg::OnButtonLimitSwitchOffset() 
{
	CString Buff;

	m_editHomeOffset.GetWindowText(Buff);
	m_pAxis->nLimitSwitchOffset=_ttoi(Buff) + m_pAxis->nPos;
	UpdateAllControls();	
}

void CAxisDlg::OnChangeEditName() 
{
	CString Buff;
	
	m_editName.GetWindowText(Buff);
	if(Buff.GetLength() > (AXISNAMELENGHT-1)) {
		Buff.GetBufferSetLength(AXISNAMELENGHT-1);
	}
	swprintf_s(m_pAxis->cName,10,L"%s",(LPCWSTR)Buff);
}

void CAxisDlg::OnChangeEditKinematicOffset() 
{
	CString Buff;
	
	m_editKinematicOffset.GetWindowText(Buff);
	m_pAxis->nOffset = _ttoi(Buff);

}

BOOL CAxisDlg::OnSetActive() 
{
	UpdateAllControls();
	SetTimer(1,100,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CAxisDlg::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}
