// AxisPmacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AxisPmacDlg.h"
#include "AxesSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisPmacDlg dialog

CAxisPmacDlg::CAxisPmacDlg(UINT nCap,CWnd* pParent,CWnd** DlgPtr,int nWhichAxis) 
	: CPropertyPage(CAxisPmacDlg::IDD,nCap)
{
	//{{AFX_DATA_INIT(CAxisPmacDlg)
	m_stringEncoderPos = _T("");
	m_stringStatus = _T("");
	m_nPos = 0;
	//}}AFX_DATA_INIT

	m_pSi6User = &theApp.m_Si6User;
	m_pAxis = &theApp.m_Axes[nWhichAxis];
	m_nCap = nCap;
	m_nWhichAxis = nWhichAxis;
	m_DlgPtr = DlgPtr;
	m_pParent = pParent;
	m_fRelativeMove = 0.0f;
	
}

CAxisPmacDlg::~CAxisPmacDlg()
{
}

void CAxisPmacDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxisPmacDlg)
	DDX_Control(pDX, IDC_COMBO_AMPLIFIER_TYPE, m_comboAmplifierType);
	DDX_Control(pDX, IDC_COMBO_MACRO_NODE, m_comboMacroNode);
	DDX_Control(pDX, IDC_EDIT_TOTAL_DISTANCE, m_editTotalDistance);
	DDX_Control(pDX, IDC_EDIT_JOG_ACCELERATION, m_editJoystickAcceleration);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_LIMIT_SWITCHES, m_checkLimitSwitches);
	DDX_Control(pDX, IDC_CHECK_FIDUCIAL, m_checkUseFiducial);
	DDX_Control(pDX, IDC_EDIT_MAX_TRAVEL_JOY, m_editMaxTravelJoy);
	DDX_Control(pDX, IDC_EDIT_MIN_TRAVEL_JOY, m_editMinTravelJoy);
	DDX_Control(pDX, IDC_SPIN_KINEMATIC_OFFSET, m_spinKinematicOffset);
	DDX_Control(pDX, IDC_EDIT_REL_MOVE, m_editRelativeMove);
	DDX_Control(pDX, IDC_COMBO_PHYSICAL_AXIS, m_comboPhysicalAxis);
	DDX_Control(pDX, IDC_EDIT_NUMBER_DP, m_editNumberDP);
	DDX_Control(pDX, IDC_EDIT_KINEMATIC_OFFSET, m_editKinematicOffset);
	DDX_Control(pDX, IDC_EDIT_STEPSREV, m_editStepsRev);
	DDX_Control(pDX, IDC_COMBO_TOGGLE_POS, m_comboTogglePos);
	DDX_Control(pDX, IDC_CHECK_MUST_HOME, m_checkMustHome);
	DDX_Control(pDX, IDC_AXIS_STEPSIZE, m_editStepSize);
	DDX_Control(pDX, IDC_CHECK_JOYSTICK_REVERSE, m_checkJoystickReverse);
	DDX_Control(pDX, IDC_AXIS_AVAILABLE_CHECK, m_checkAxisAvailable);
	DDX_Control(pDX, IDC_AXIS_UNITS_COMBO, m_comboAxisUnits);
	DDX_Control(pDX, IDC_EDIT_ACCELERATION, m_editAcceleration);
	DDX_Control(pDX, IDC_EDIT_HOME_OFFSET, m_editHomeOffset);
	DDX_Control(pDX, IDC_EDIT_HOME_SPEED, m_editHomeSpeed);
	DDX_Control(pDX, IDC_EDIT_IN_POS_ERR, m_editInPosErr);
	DDX_Control(pDX, IDC_EDIT_JOYSTICK_GAIN, m_editJoystickGain);
	DDX_Control(pDX, IDC_EDIT_JOYSTICK_NUMBER, m_editJoystickNumber);
	DDX_Control(pDX, IDC_EDIT_MAX_SPEED, m_editMaxSpeed);
	DDX_Control(pDX, IDC_EDIT_MAX_TRAVEL, m_editMaxTravel);
	DDX_Control(pDX, IDC_EDIT_MIN_TRAVEL, m_editMinTravel);
	DDX_Text(pDX, IDC_EDIT_ENCODER_POS, m_stringEncoderPos);
	DDX_Text(pDX, IDC_EDIT_CURRENT_STATUS, m_stringStatus);
	DDX_Text(pDX, IDC_EDIT_POS, m_nPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxisPmacDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CAxisPmacDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ACCEPT_BUTTON, OnAcceptButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_BN_CLICKED(IDC_BUTTON_HOME_OFFSET, OnButtonHomeOffset)
	ON_BN_CLICKED(IDC_BUTTON_HMZ, OnButtonHmz)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_TOGGLE_POS, OnSelchangeComboTogglePos)
	ON_CBN_SELCHANGE(IDC_COMBO_PHYSICAL_AXIS, OnSelchangeComboPhysicalAxis)
	ON_BN_CLICKED(IDC_BUTTON_REL_MOVE, OnButtonRelMove)
	ON_EN_CHANGE(IDC_EDIT_REL_MOVE, OnChangeEditRelMove)
	ON_EN_CHANGE(IDC_EDIT_MIN_TRAVEL, OnChangeEditMinTravel)
	ON_EN_CHANGE(IDC_EDIT_MAX_TRAVEL, OnChangeEditMaxTravel)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE_LS, OnButtonDisableLs)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_KINEMATIC_OFFSET, OnDeltaposSpinKinematicOffset)
	ON_EN_CHANGE(IDC_EDIT_KINEMATIC_OFFSET, OnChangeEditKinematicOffset)
	ON_BN_CLICKED(IDC_BUTTON_KINEMATIC_ODDSET, OnButtonKinematicOddset)
	ON_BN_CLICKED(IDC_CHECK_LIMIT_SWITCHES, OnCheckLimitSwitches)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_CBN_SELCHANGE(IDC_COMBO_AMPLIFIER_TYPE, OnSelchangeComboAmplifierType)
	ON_EN_CHANGE(IDC_EDIT_JOYSTICK_GAIN, OnChangeEditJoystickGain)
	ON_EN_CHANGE(IDC_EDIT_STEPSREV, OnChangeEditStepsrev)
	ON_BN_CLICKED(IDC_BUTTON_REL_NEG_MOVE, OnButtonRelNegMove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisPmacDlg message handlers

BOOL CAxisPmacDlg::OnInitDialog() 
{
	CString Buff;
	CPropertyPage::OnInitDialog();

	m_comboAxisUnits.AddString(_T("mm"));
	m_comboAxisUnits.AddString(_T("\x022"));
	m_comboAxisUnits.AddString(DEGREES);

	m_comboTogglePos.AddString(_T("Up"));
	m_comboTogglePos.AddString(_T("Middle"));
	m_comboTogglePos.AddString(_T("Bottom"));

	m_spinKinematicOffset.SetRange(-20000,20000);
	m_spinKinematicOffset.SetPos(0);

	for(int ii=1;ii<17;ii++) {
		Buff.Format(_T("%d"),ii);
		m_comboPhysicalAxis.AddString(Buff);
	}

	m_comboMacroNode.AddString(L"N/A");
	for(int ii=0;ii<32;ii++) {
		Buff.Format(L"%d",ii);
		m_comboMacroNode.AddString(Buff);
	}

	m_comboAmplifierType.AddString(L"");
	m_comboAmplifierType.AddString(L"Kollmorgen");
	m_comboAmplifierType.AddString(L"Copley");
	m_comboAmplifierType.AddString(L"Staubli");
	m_comboAmplifierType.AddString(L"Oriental");
	m_comboAmplifierType.AddString(L"GeoBrick");

	
	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;
}

HBRUSH CAxisPmacDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CAxisPmacDlg::OnAcceptButton() 
{
	CString	Buff;
	CAxesSheet* pSheet = (CAxesSheet*)m_pParent;
	float fTemp;

	m_pAxis->nMacroNode = m_comboMacroNode.GetCurSel() - 1;

	m_editKinematicOffset.GetWindowText(Buff);
	m_pAxis->nOffset = _ttoi(Buff);

	m_editStepSize.GetWindowText(Buff);
	_WTOF(Buff,m_pAxis->fStepSize);
	if(m_pAxis->fStepSize == 0.0) {
		m_pAxis->fStepSize = 0.0001f;
		Buff.Format(_T("%.07f"),m_pAxis->fStepSize);
		m_editStepSize.SetWindowText(Buff);
	};

	if(m_checkAxisAvailable.GetCheck()&0x0001) {
		m_pAxis->bAvailable=TRUE;
	} else {
		m_pAxis->bAvailable=FALSE;
	}
	if(m_checkJoystickReverse.GetCheck()&1) {
		m_pAxis->bJoystickReverse=TRUE;
	} else {
		m_pAxis->bJoystickReverse=FALSE;
	};
	if(m_checkMustHome.GetCheck()&1) {
		m_pAxis->bMustHome=TRUE;
	} else {
		m_pAxis->bMustHome=FALSE;
	};


	m_editMaxSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	m_pAxis->fMaxMoveSpeed=m_pAxis->fMaxJogSpeed = (fTemp / 1000.0f) / m_pAxis->fStepSize;

	m_editHomeSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	m_pAxis->fHomeSpeed = (fTemp / 1000.0f) / m_pAxis->fStepSize;

	m_editJoystickAcceleration.GetWindowText(Buff);
	_WTOF(Buff,m_pAxis->fJoystickAcceleration);

	m_editAcceleration.GetWindowText(Buff);
	_WTOF(Buff,m_pAxis->fAcceleration);

	m_editHomeOffset.GetWindowText(Buff);
	m_pAxis->nLimitSwitchOffset=_ttoi(Buff);

	m_editInPosErr.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	switch(theApp.m_nMotorType) {
	default:	m_pAxis->nPositionError=(int)(fTemp / m_pAxis->fStepSize) * 16;
		break;
	case FIELD_BUS: m_pAxis->nPositionError=(int)(fTemp / m_pAxis->fStepSize);
		break;
	}


	m_editJoystickNumber.GetWindowText(Buff);
	m_pAxis->nJoystickNumber=_ttoi(Buff)-1;

	m_editJoystickGain.GetWindowText(Buff);
	m_pAxis->nJoystickGain=_ttoi(Buff);

	m_editMaxTravel.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	m_pAxis->nMaxTravel=(int)(fTemp / m_pAxis->fStepSize);

	m_editMinTravel.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	m_pAxis->nMinTravel=(int)(fTemp / m_pAxis->fStepSize);


	m_pAxis->nUnitSelected=m_comboAxisUnits.GetCurSel();

	m_editStepsRev.GetWindowText(Buff);
	_WTOD(Buff,m_pAxis->dStepsPerRev);
	m_pAxis->nStepsPerRev = (int)m_pAxis->dStepsPerRev;

	m_editNumberDP.GetWindowText(Buff);
	m_pAxis->nPrecision = _ttoi(Buff);


	m_editMinTravelJoy.GetWindowText(Buff);
	_WTOF(Buff,m_pAxis->fMinKin);
	m_editMaxTravelJoy.GetWindowText(Buff);
	_WTOF(Buff,m_pAxis->fMaxKin);

	m_checkUseFiducial.GetCheck() & 1 ?	m_pAxis->bUseFiducial=true : 	m_pAxis->bUseFiducial=false;

	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: 
		theApp.m_PmacUser.SetVariable(_T("I"),m_pAxis->nPhysicalAxis*100+22,m_pAxis->fMaxMoveSpeed);
		theApp.m_PmacUser.SetVariable(_T("I"),m_pAxis->nPhysicalAxis*100+16,m_pAxis->fMaxMoveSpeed);
		theApp.m_PmacUser.SetVariable(_T("I"),m_pAxis->nPhysicalAxis*100+23,m_pAxis->fHomeSpeed);
		theApp.m_PmacUser.SetVariable(_T("I"),m_pAxis->nPhysicalAxis*100+19,m_pAxis->fJoystickAcceleration);
		theApp.m_PmacUser.SetVariable(_T("I"),m_pAxis->nPhysicalAxis*100+17,m_pAxis->fAcceleration);
		theApp.m_PmacUser.SetVariable(_T("I"),m_pAxis->nPhysicalAxis*100+26,m_pAxis->nLimitSwitchOffset);
		theApp.m_PmacUser.SetVariable(_T("I"),m_pAxis->nPhysicalAxis*100+28,m_pAxis->nPositionError);
		theApp.m_PmacUser.SetVariable(_T("I"),m_pAxis->nPhysicalAxis*100+13,m_pAxis->nMaxTravel);
		theApp.m_PmacUser.SetVariable(_T("I"),m_pAxis->nPhysicalAxis*100+14,m_pAxis->nMinTravel);

		theApp.m_PmacUser.UpdateVirtualAxes();
		break;
	case FIELD_BUS:
		
		Buff.Format(L"Motor[%d].InvAmax=%f",m_pAxis->nPhysicalAxis,1.0f / m_pAxis->fAcceleration);
		theApp.m_FBCtrl.SendStr(Buff);

		Buff.Format(L"Motor[%d].InvDmax=%f",m_pAxis->nPhysicalAxis,1.0f / m_pAxis->fAcceleration);
		theApp.m_FBCtrl.SendStr(Buff);

		Buff.Format(L"Motor[%d].JogTa=%f",m_pAxis->nPhysicalAxis,-1.0f / m_pAxis->fJoystickAcceleration);
		theApp.m_FBCtrl.SendStr(Buff);


		Buff.Format(L"Motor[%d].MaxSpeed=%f",m_pAxis->nPhysicalAxis,m_pAxis->fMaxMoveSpeed);
		theApp.m_FBCtrl.SendStr(Buff);

		Buff.Format(L"Motor[%d].InPosBand=%d",m_pAxis->nPhysicalAxis,m_pAxis->nPositionError);
		theApp.m_FBCtrl.SendStr(Buff);
		break;
	}

	pSheet->UpdateStatusSheet();


	theApp.SaveTank();

}

void CAxisPmacDlg::OnDefaultButton() 
{
	Defaults(m_nWhichAxis);
	
}

void CAxisPmacDlg::UpdateAllControls()
{
	int	nn;
	CString Buff;
	CString Units[3]={L" mm", L"\x022", L"\x0b0"};
	if (GetSafeHwnd() == NULL) return;

	m_comboAxisUnits.SetCurSel(m_pAxis->nUnitSelected);

	m_comboMacroNode.SetCurSel(m_pAxis->nMacroNode + 1);


	if(m_pAxis->bAvailable==TRUE) {
		m_checkAxisAvailable.SetCheck(0x0001);
	} else {
		m_checkAxisAvailable.SetCheck(0x0000);
	}

	if(m_pAxis->bJoystickReverse==TRUE) {
		m_checkJoystickReverse.SetCheck(1);
	} else {
		m_checkJoystickReverse.SetCheck(0);
	};

	if(m_pAxis->bMustHome==TRUE) {
		m_checkMustHome.SetCheck(1);
	} else {
		m_checkMustHome.SetCheck(0);
	};

	Buff.Format(_T("%.03f"),m_pAxis->fAcceleration);
	m_editAcceleration.SetWindowText(Buff);

	Buff.Format(_T("%.03f"),m_pAxis->fJoystickAcceleration);
	m_editJoystickAcceleration.SetWindowText(Buff);

	Buff.Format(_T("%.02f %s/s"),m_pAxis->fMaxMoveSpeed * 1000.0f * m_pAxis->fStepSize,Units[m_pAxis->nUnitSelected]);
	m_editMaxSpeed.SetWindowText(Buff);

	Buff.Format(_T("%.01f  %s/s"),m_pAxis->fHomeSpeed * 1000.0f * m_pAxis->fStepSize,Units[m_pAxis->nUnitSelected]);
	m_editHomeSpeed.SetWindowText(Buff);

	Buff.Format(_T("%d Steps"),m_pAxis->nLimitSwitchOffset);
	m_editHomeOffset.SetWindowText(Buff);


	if(m_pAxis->fStepSize >= 0.0001f) {
		Buff.Format(_T("%.07f"),m_pAxis->fStepSize);
	} else {
		Buff.Format(_T("%11.05e"),m_pAxis->fStepSize);
	}
	m_editStepSize.SetWindowText(Buff);




//	Buff.Format(_T("%d Steps/s"),m_pAxis->nJoystickMaxSpeed);
//	m_editJoystickMaxSpeed.SetWindowText(str);


	switch(theApp.m_nMotorType) {
	default:	Buff.Format(_T("%.02f%s"),(float)m_pAxis->nPositionError * m_pAxis->fStepSize / 16.0f,Units[m_pAxis->nUnitSelected]);
		break;
	case FIELD_BUS: Buff.Format(_T("%.02f%s"),(float)m_pAxis->nPositionError * m_pAxis->fStepSize,Units[m_pAxis->nUnitSelected]);
		break;
	}
	m_editInPosErr.SetWindowText(Buff);


	Buff.Format(_T("%d"),m_pAxis->nJoystickGain);
	m_editJoystickGain.SetWindowText(Buff);

	nn=m_pAxis->nJoystickNumber+1;
	Buff.Format(_T("%d"),nn);
	m_editJoystickNumber.SetWindowText(Buff);

	Buff.Format(_T("%.01f"),m_pAxis->fStepSize * (float)m_pAxis->nMaxTravel);
	m_editMaxTravel.SetWindowText(Buff);

	Buff.Format(_T("%.01f"),m_pAxis->fStepSize * (float)m_pAxis->nMinTravel);
	m_editMinTravel.SetWindowText(Buff);


	m_comboPhysicalAxis.SetCurSel(m_pAxis->nPhysicalAxis - 1);

	m_comboTogglePos.SetCurSel(m_pAxis->nJoystickSwitchPos);

	Buff.Format(_T("%.01f"),m_pAxis->dStepsPerRev);
	m_editStepsRev.SetWindowText(Buff);

	Buff.Format(_T("%d"),m_pAxis->nOffset);
	m_editKinematicOffset.SetWindowText(Buff);

	Buff.Format(_T("%d"),m_pAxis->nPrecision);
	m_editNumberDP.SetWindowText(Buff);

	Buff.Format(_T("%.02f"),m_fRelativeMove);
	m_editRelativeMove.SetWindowText(Buff);

	Buff.Format(_T("%.01f"),m_pAxis->fMinKin);
	m_editMinTravelJoy.SetWindowText(Buff);
	Buff.Format(_T("%.01f"),m_pAxis->fMaxKin);
	m_editMaxTravelJoy.SetWindowText(Buff);

	m_checkUseFiducial.SetCheck(m_pAxis->bUseFiducial);

	if(m_nFlagModeControl & 0x20000) {
		m_checkLimitSwitches.SetCheck(0);	//Limit switches are disabled
	} else {
		m_checkLimitSwitches.SetCheck(1);
	}


	Buff.Format(L"%s",m_pAxis->cName);
	m_editName.SetWindowText(Buff);


	m_comboAmplifierType.SetCurSel(m_pAxis->eAmplifierType);	

	UpdateData(FALSE);
}


void CAxisPmacDlg::Defaults(int nWhich)
{
	int	mask=1;

	memset(m_pAxis,0,sizeof AxisData);
	m_pAxis->bAvailable = true;
	m_pAxis->fStepSize = 0.1f;
	m_pAxis->nMinSpeed = 200;
	m_pAxis->nMaxSpeed = 2000;
	m_pAxis->nAcceleration=2;
	m_pAxis->nDeceleration=2;
	m_pAxis->nHomeSpeed=-400;
	m_pAxis->nJoystickMinSpeed=50;
	m_pAxis->nJoystickMaxSpeed=1000;
	m_pAxis->nJoystickMinValue=0;
	m_pAxis->nJoystickMaxValue=255;
	m_pAxis->nJoystickNumber=nWhich%3;
	m_pAxis->nMinTravel=0;
	m_pAxis->nMaxTravel=2000;
	m_pAxis->nJoystickGain=100;
	m_pAxis->nPhysicalAxis=nWhich+1;
	m_pAxis->nPositionError =  (int)(16.0f * (m_pAxis->fStepSize * 5.0f));
	m_pAxis->nMotorType = SERVO;

	ReadPmac();

	UpdateAllControls();	
}


void CAxisPmacDlg::MinMax(int *v0, int v1, int v2)
{

	if(*v0<v1) *v0=v1;
	if(*v0>v2) *v0=v2;
}


void CAxisPmacDlg::ReadPmac() 
{
	CString RxStr,Buff;

	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: 
		theApp.m_PmacUser.RequestVariable(_T("I"),m_pAxis->nPhysicalAxis*100+17,&RxStr);
		_WTOF(RxStr,m_pAxis->fAcceleration);

		theApp.m_PmacUser.RequestVariable(_T("I"),m_pAxis->nPhysicalAxis*100+19,&RxStr);
		_WTOF(RxStr,m_pAxis->fJoystickAcceleration);

		theApp.m_PmacUser.RequestVariable(_T("I"),m_pAxis->nPhysicalAxis*100+16,&RxStr);
		_WTOF(RxStr,m_pAxis->fMaxMoveSpeed);

		theApp.m_PmacUser.RequestVariable(_T("I"),m_pAxis->nPhysicalAxis*100+28,&RxStr);
		if(RxStr.GetLength()) m_pAxis->nPositionError=_ttoi(RxStr);

		theApp.m_PmacUser.RequestVariable(_T("I"),m_pAxis->nPhysicalAxis*100+24,&RxStr);
		if(RxStr.GetLength()) {
			RxStr.Replace(L"$",L"0x");
			swscanf_s(RxStr,_T("%x"),&m_nFlagModeControl);
		}
		break;
	case FIELD_BUS:
		Buff.Format(L"Motor[%d].InvAmax",m_pAxis->nPhysicalAxis);
		theApp.m_FBCtrl.SendStr(Buff,_BACKGROUND);

		Buff.Format(L"Motor[%d].JogTa",m_pAxis->nPhysicalAxis);
		theApp.m_FBCtrl.SendStr(Buff,_BACKGROUND);

		Buff.Format(L"Motor[%d].MaxSpeed",m_pAxis->nPhysicalAxis);
		theApp.m_FBCtrl.SendStr(Buff,_BACKGROUND);

		Buff.Format(L"Motor[%d].InPosBand",m_pAxis->nPhysicalAxis);
		theApp.m_FBCtrl.SendStr(Buff,_BACKGROUND);
		break;
	}



}

void CAxisPmacDlg::OnButtonHmz() 
{
	if(MessageBox(_T("Are you damn sure you want to set this encoder to zero"),_T("Warning"),MB_YESNO)==IDYES) {
		theApp.m_PmacUser.HomeAxisWithoutMoving(m_pAxis->nPhysicalAxis);
	};
}

void CAxisPmacDlg::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	CString Units[3]={L" mm", L"\x022", L"\x0b0"};

	if(nUpdateLoopCounter < 5) {
		UpdateAllControls();
		nUpdateLoopCounter++;
	};

	if(m_nOldStatus - m_pAxis->nStatus) {
		m_stringStatus.Format(_T("%06x"),m_pAxis->nStatus);
		m_nOldStatus =m_pAxis->nStatus;
		UpdateData(FALSE);
	};

	m_nPos = m_pAxis->nPos;
	UpdateData(FALSE);

	if(m_fLastDisplayedDistance - m_pAxis->fTotalDistance) {
		switch(m_pAxis->nUnitSelected) {
		case 0:
			Buff.Format(L"%.02gkm",m_pAxis->fTotalDistance / 1e6);
			break;
		case 1:	Buff.Format(L"%g/x022",m_pAxis->fTotalDistance / 304.8);
			break;
		case 2:	Buff.Format(L"%.01g%s",m_pAxis->fTotalDistance,DEGREES);
			break;
		}
		m_editTotalDistance.SetWindowText(Buff);
		m_fLastDisplayedDistance = m_pAxis->fTotalDistance;
	}

	CPropertyPage::OnTimer(nIDEvent);
}

void CAxisPmacDlg::OnSelchangeComboTogglePos() 
{
	m_pAxis->nJoystickSwitchPos = m_comboTogglePos.GetCurSel();	
}

BOOL CAxisPmacDlg::OnSetActive() 
{

	ReadPmac();
	m_fLastDisplayedDistance = -1.0f;
	UpdateAllControls();
	nUpdateLoopCounter=0;
	SetTimer(1,200,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CAxisPmacDlg::OnKillActive() 
{

	KillTimer(1);

	return CPropertyPage::OnKillActive();
}

void CAxisPmacDlg::OnSelchangeComboPhysicalAxis() 
{
	int nAxis = m_pAxis->nPhysicalAxis;

	m_pAxis->nPhysicalAxis=m_comboPhysicalAxis.GetCurSel() + 1;
	if(nAxis - m_pAxis->nPhysicalAxis) {
		ReadPmac();
		UpdateAllControls();
	}

}


void CAxisPmacDlg::OnChangeEditRelMove() 
{
	CString Buff;
	
	m_editRelativeMove.GetWindowText(Buff);
	_WTOF(Buff,m_fRelativeMove);
}

void CAxisPmacDlg::OnChangeEditMinTravel() 
{
//done in on accept
}

void CAxisPmacDlg::OnChangeEditMaxTravel() 
{
//done in on accept	
}

void CAxisPmacDlg::OnButtonDisableLs() 
{
	CString Buff;

	theApp.m_PmacUser.DisableLimitSwitch(m_pAxis);
	theApp.m_PmacUser.m_bLS_Disabled_Reenable_On_Exit[m_pAxis->nPhysicalAxis] = true;

	Buff.LoadString(IDS_Limit_switch_disabled);
	MessageBox(Buff,_T("Warning"),NULL);


}

void CAxisPmacDlg::OnDeltaposSpinKinematicOffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	
	int nStep = pNMUpDown->iDelta * (int)(0.05f / m_pAxis->fStepSize);
	Buff.Format(_T("%d"),m_pAxis->nOffset += nStep);
	m_editKinematicOffset.SetWindowText(Buff);

	*pResult = 0;
}

void CAxisPmacDlg::OnChangeEditKinematicOffset() 
{
	CString Buff;
	
	m_editKinematicOffset.GetWindowText(Buff);
	m_pAxis->nOffset = _ttoi(Buff);
}

void CAxisPmacDlg::OnButtonKinematicOddset() 
{
	CCoord CpHead;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
	m_pAxis->nOffset = 0;
	Sleep(100);
	m_pAxis->nOffset = m_pAxis->nPos;

	UpdateAllControls();
	theApp.SaveTank();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
	
}

void CAxisPmacDlg::OnButtonHomeOffset() 
{

	if(theApp.m_LastSettings.nManipulatorType[PORTSIDE] == ROTATE_DIF_THETA) {
		if(m_nWhichAxis == theApp.m_Tank.nXtLeft) {
			m_pAxis->nLimitSwitchOffset = theApp.m_Si10User.ReadAbsEncoder(0);
			UpdateAllControls();
			return;
		}
		if(m_nWhichAxis == theApp.m_Tank.nYtLeft) {
			m_pAxis->nLimitSwitchOffset = theApp.m_Si10User.ReadAbsEncoder(1);
			UpdateAllControls();
			return;
		}
	};
	if(theApp.m_LastSettings.nManipulatorType[STARBOARD] == ROTATE_DIF_THETA) {
		if(m_nWhichAxis == theApp.m_Tank.nXtRight) {
			m_pAxis->nLimitSwitchOffset = theApp.m_Si10User.ReadAbsEncoder(2);
			UpdateAllControls();
			return;
		}
		if(m_nWhichAxis == theApp.m_Tank.nYtRight) {
			m_pAxis->nLimitSwitchOffset = theApp.m_Si10User.ReadAbsEncoder(3);
			UpdateAllControls();
			return;
		}
	};

	m_pAxis->nLimitSwitchOffset+=((m_pAxis->nPos+m_pAxis->nOffset)*16);
	UpdateAllControls();
}


void CAxisPmacDlg::OnCheckLimitSwitches() 
{
	if(m_checkLimitSwitches.GetCheck() == 0) {
		theApp.m_PmacUser.DisableLimitSwitch(m_pAxis);
	} else {
		theApp.m_PmacUser.EnableLimitSwitch(m_pAxis);
	}	
}

void CAxisPmacDlg::OnChangeEditName() 
{
	CString Buff;
	
	m_editName.GetWindowText(Buff);
	if(Buff.GetLength() > (AXISNAMELENGHT-1)) {
		Buff.GetBufferSetLength(AXISNAMELENGHT-1);
	}
	swprintf_s(m_pAxis->cName,10,L"%s",(LPCWSTR)Buff);
}

void CAxisPmacDlg::OnSelchangeComboAmplifierType() 
{
	m_pAxis->eAmplifierType = (AmplifierType)m_comboAmplifierType.GetCurSel();	
}

void CAxisPmacDlg::OnChangeEditJoystickGain() 
{
	CString Buff;

	m_editJoystickGain.GetWindowText(Buff);
	m_pAxis->nJoystickGain=_ttoi(Buff);
}


void CAxisPmacDlg::OnChangeEditStepsrev() 
{
	CString Buff;

	m_editStepsRev.GetWindowText(Buff);
	_WTOD(Buff,m_pAxis->dStepsPerRev);
	m_pAxis->nStepsPerRev = (int)m_pAxis->dStepsPerRev;
}

void CAxisPmacDlg::OnButtonRelMove() 
{
	int nRelativeMove = (int)(m_fRelativeMove / m_pAxis->fStepSize);
	theApp.m_Motors.RelativeMove(m_pAxis,nRelativeMove);
}

void CAxisPmacDlg::OnButtonRelNegMove() 
{
	int nRelativeMove = (int)(m_fRelativeMove / m_pAxis->fStepSize);
	theApp.m_Motors.RelativeMove(m_pAxis,-nRelativeMove);
}

