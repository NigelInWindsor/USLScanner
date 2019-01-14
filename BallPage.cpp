// BallPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "BallPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBallPage property page

IMPLEMENT_DYNCREATE(CBallPage, CPropertyPage)

CBallPage::CBallPage() : CPropertyPage(CBallPage::IDD)
{
	//{{AFX_DATA_INIT(CBallPage)
	//}}AFX_DATA_INIT
}

CBallPage::~CBallPage()
{
}

void CBallPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBallPage)
	DDX_Control(pDX, IDC_BUTTON_SIDE1_EQUAL_SIDE0, m_buttonSide1EqualSide0);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	DDX_Control(pDX, IDC_BUTTON_SET_ZTIP_RIGHT, m_buttonZtRight);
	DDX_Control(pDX, IDC_BUTTON_SET_ZTIP_LEFT, m_buttonZtLeft);
	DDX_Control(pDX, IDC_BUTTON_SET_XTIP_RIGHT, m_buttonXtRight);
	DDX_Control(pDX, IDC_BUTTON_SET_XTIP_LEFT, m_buttonXtLeft);
	DDX_Control(pDX, IDC_EDIT_Z_RIGHT, m_editZRight);
	DDX_Control(pDX, IDC_EDIT_Z_LEFT, m_editZLeft);
	DDX_Control(pDX, IDC_EDIT_Y_RIGHT, m_editYRight);
	DDX_Control(pDX, IDC_EDIT_Y_LEFT, m_editYLeft);
	DDX_Control(pDX, IDC_EDIT_X_RIGHT, m_editXRight);
	DDX_Control(pDX, IDC_EDIT_X_LEFT, m_editXLeft);
	DDX_Control(pDX, IDC_EDIT_XTIP_LEFT, m_editXTipLeft);
	DDX_Control(pDX, IDC_EDIT_ZTIP_RIGHT, m_editZTipRight);
	DDX_Control(pDX, IDC_EDIT_ZTIP_LEFT, m_editZTipLeft);
	DDX_Control(pDX, IDC_EDIT_XTIP_RIGHT, m_editXTipRight);
	DDX_Control(pDX, IDC_COMBO_RIGHT_TIMESLOT, m_comboRightTimeSlot);
	DDX_Control(pDX, IDC_COMBO_LEFT_TIMESLOT, m_comboLeftTimeSlot);
	DDX_Control(pDX, IDC_EDIT_RIGHT_POS, m_editRightPos);
	DDX_Control(pDX, IDC_EDIT_LEFT_POS, m_editLeftPos);
	DDX_Control(pDX, IDC_EDIT_DIAMETER, m_editDiameter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBallPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBallPage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_DIAMETER, OnChangeEditDiameter)
	ON_BN_CLICKED(IDC_BUTTON_STORE_LEFT, OnButtonStoreLeft)
	ON_BN_CLICKED(IDC_BUTTON_STORE_RIGHT, OnButtonStoreRight)
	ON_CBN_SELCHANGE(IDC_COMBO_RIGHT_TIMESLOT, OnSelchangeComboRightTimeslot)
	ON_CBN_SELCHANGE(IDC_COMBO_LEFT_TIMESLOT, OnSelchangeComboLeftTimeslot)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_RIGHT, OnButtonGotoRight)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_LEFT, OnButtonGotoLeft)
	ON_BN_CLICKED(IDC_BUTTON_XYZ_LEFT, OnButtonXyzLeft)
	ON_BN_CLICKED(IDC_BUTTON_XYZ_RIGHT, OnButtonXyzRight)
	ON_BN_CLICKED(IDC_BUTTON_SET_XTIP_LEFT, OnButtonSetXtipLeft)
	ON_BN_CLICKED(IDC_BUTTON_SET_ZTIP_LEFT, OnButtonSetZtipLeft)
	ON_BN_CLICKED(IDC_BUTTON_SET_XTIP_RIGHT, OnButtonSetXtipRight)
	ON_BN_CLICKED(IDC_BUTTON_SET_ZTIP_RIGHT, OnButtonSetZtipRight)
	ON_BN_CLICKED(IDC_BUTTON_ZERO_BRIDGE, OnButtonZeroBridge)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_XT_LEFT, OnDeltaposSpinXtLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_XT_RIGHT, OnDeltaposSpinXtRight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ZT_LEFT, OnDeltaposSpinZtLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ZT_RIGHT, OnDeltaposSpinZtRight)
	ON_EN_CHANGE(IDC_EDIT_LEFT_POS, OnChangeEditLeftPos)
	ON_EN_CHANGE(IDC_EDIT_RIGHT_POS, OnChangeEditRightPos)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_BOTH, OnButtonGotoBoth)
	ON_BN_CLICKED(IDC_BUTTON_ZERO_ZERO, OnButtonZeroZero)
	ON_BN_CLICKED(IDC_BUTTON_SIDE1_EQUAL_SIDE0, OnButtonSide1EqualSide0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBallPage message handlers

BOOL CBallPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;
	
	if (theApp.m_nSide0Orientation == 0) {
		m_staticLeft.SetWindowText(theApp.m_DlgSideName[0]);
		m_staticRight.SetWindowText(theApp.m_DlgSideName[1]);
	}
	else {
		m_staticLeft.SetWindowText(theApp.m_DlgSideName[1]);
		m_staticRight.SetWindowText(theApp.m_DlgSideName[0]);
	}

	Buff.Format(L"%s = %s",theApp.m_DlgSideName[1], theApp.m_DlgSideName[0]);
	m_buttonSide1EqualSide0.SetWindowText(Buff);

	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CBallPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CBallPage::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%.02f mm"),theApp.m_LastSettings.fZeroBallDiameter);
	m_editDiameter.SetWindowText(Buff);


	m_comboLeftTimeSlot.ResetContent();
	m_comboRightTimeSlot.ResetContent();
	for(int nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		Buff.Format(_T("(%d) %s"),nTS+1,theApp.m_UtUser.m_TS[nTS].cName);
		m_comboLeftTimeSlot.AddString(Buff);
		m_comboRightTimeSlot.AddString(Buff);
	}

	if(theApp.m_nSide0Orientation==0) {
		m_comboLeftTimeSlot.SetCurSel(theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		m_comboRightTimeSlot.SetCurSel(theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
		Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),theApp.m_CpBall.Side0.fX,theApp.m_CpBall.Side0.fY,theApp.m_CpBall.Side0.fZ);
		m_editLeftPos.SetWindowText(Buff);
		Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),theApp.m_CpBall.Side1.fX,theApp.m_CpBall.Side1.fY,theApp.m_CpBall.Side1.fZ);
		m_editRightPos.SetWindowText(Buff);
	} else {
		m_comboLeftTimeSlot.SetCurSel(theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
		m_comboRightTimeSlot.SetCurSel(theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),theApp.m_CpBall.Side1.fX,theApp.m_CpBall.Side1.fY,theApp.m_CpBall.Side1.fZ);
		m_editLeftPos.SetWindowText(Buff);
		Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),theApp.m_CpBall.Side0.fX,theApp.m_CpBall.Side0.fY,theApp.m_CpBall.Side0.fZ);
		m_editRightPos.SetWindowText(Buff);
	}
	Buff="0.0";
	m_editXLeft.SetWindowText(Buff);
	m_editYLeft.SetWindowText(Buff);
	m_editZLeft.SetWindowText(Buff);
	m_editXTipLeft.SetWindowText(Buff);
	m_editZTipLeft.SetWindowText(Buff);
	m_editXRight.SetWindowText(Buff);
	m_editYRight.SetWindowText(Buff);
	m_editZRight.SetWindowText(Buff);
	m_editXTipRight.SetWindowText(Buff);
	m_editZTipRight.SetWindowText(Buff);

	Buff.Format(L"%s",theApp.m_Axes[theApp.m_Tank.nXtLeft].cName);
//	theApp.CharToCString(theApp.m_Axes[theApp.m_Tank.nXtLeft].cName, Buff);
	if(Buff.GetLength()>0)
		m_buttonXtLeft.SetWindowText(Buff);

	Buff.Format(L"%s",theApp.m_Axes[theApp.m_Tank.nYtLeft].cName);
//	theApp.CharToCString(theApp.m_Axes[theApp.m_Tank.nYtLeft].cName, Buff);
	if(Buff.GetLength()>0)
		m_buttonZtLeft.SetWindowText(Buff);

	Buff.Format(L"%s",theApp.m_Axes[theApp.m_Tank.nXtRight].cName);
//	theApp.CharToCString(theApp.m_Axes[theApp.m_Tank.nXtRight].cName, Buff);
	if(Buff.GetLength()>0)
		m_buttonXtRight.SetWindowText(Buff);

	Buff.Format(L"%s",theApp.m_Axes[theApp.m_Tank.nYtRight].cName);
//	theApp.CharToCString(theApp.m_Axes[theApp.m_Tank.nYtRight].cName, Buff);
	if(Buff.GetLength()>0)
		m_buttonZtRight.SetWindowText(Buff);

	SetAccessPrivelages();

}

BOOL CBallPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastCompensationToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}

void CBallPage::OnChangeEditDiameter() 
{
	CString Buff;

	m_editDiameter.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fZeroBallDiameter);

}

void CBallPage::OnButtonStoreLeft() 
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_Kinematics.CalculateCoordAlongNorm(CpSurface,&theApp.m_CpBall,PORTSIDE,theApp.m_LastSettings.fZeroBallDiameter / 2.0f);
	} else {
		theApp.m_Kinematics.CalculateCoordAlongNorm(CpSurface,&theApp.m_CpBall,STARBOARD,theApp.m_LastSettings.fZeroBallDiameter / 2.0f);
	}
		
	UpdateAllControls();
	
}

void CBallPage::OnButtonStoreRight() 
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_Kinematics.CalculateCoordAlongNorm(CpSurface,&theApp.m_CpBall,STARBOARD,theApp.m_LastSettings.fZeroBallDiameter / 2.0f);
	} else {
		theApp.m_Kinematics.CalculateCoordAlongNorm(CpSurface,&theApp.m_CpBall,PORTSIDE,theApp.m_LastSettings.fZeroBallDiameter / -2.0f);
	}

	UpdateAllControls();
}


void CBallPage::OnSelchangeComboRightTimeslot() 
{
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_LastSettings.nWaterPathTS[STARBOARD] = m_comboRightTimeSlot.GetCurSel();
	} else {
		theApp.m_LastSettings.nWaterPathTS[PORTSIDE] = m_comboRightTimeSlot.GetCurSel();
	}
}

void CBallPage::OnSelchangeComboLeftTimeslot() 
{
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_LastSettings.nWaterPathTS[PORTSIDE] = m_comboLeftTimeSlot.GetCurSel();
	} else {
		theApp.m_LastSettings.nWaterPathTS[STARBOARD] = m_comboLeftTimeSlot.GetCurSel();
	}
}

void CBallPage::OnButtonGotoRight() 
{
	CString Buff;

	theApp.m_Thread.StopCurrentThreadAction();
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_Thread.m_nNormSide = STARBOARD;
	} else {
		theApp.m_Thread.m_nNormSide = PORTSIDE;
	}
	m_editXTipRight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Thread.m_fXtMove);
	m_editZTipRight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Thread.m_fYtMove);
	theApp.m_Thread.m_nThreadAction=MOVE_ROUND_BALL;

}

void CBallPage::OnButtonGotoLeft() 
{
	CString Buff;

	theApp.m_Thread.StopCurrentThreadAction();
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_Thread.m_nNormSide = PORTSIDE;
	} else {
		theApp.m_Thread.m_nNormSide = STARBOARD;
	}
	m_editXTipLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Thread.m_fXtMove);
	m_editZTipLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Thread.m_fYtMove);
	theApp.m_Thread.m_nThreadAction=MOVE_ROUND_BALL;
}


void CBallPage::OnButtonSetXtipLeft() 
{
	CCoord CpHead;
	CString Buff;
	float fTip;
	int nOffset;
	int nAxis;

	theApp.m_nSide0Orientation == 0 ? nAxis = theApp.m_Tank.nXtLeft : nAxis = theApp.m_Tank.nXtRight;

	m_editXTipLeft.GetWindowText(Buff);
	_WTOF(Buff,fTip);

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	switch (theApp.m_Tank.nScannerDescription) {
	default:

		if (theApp.m_Axes[theApp.m_Tank.nXtLeft].nMotorType == STEPPER) {
			theApp.m_Thread.m_nThreadAction = JOYSTICK_START; //required so we have irqs to keep npos up to date
			Sleep(200);
		}
		else {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
		}

		theApp.m_Axes[nAxis].nOffset = 0;
		Sleep(100);
		theApp.m_Ethercat.InvalidateRobotJoints();
		theApp.m_Axes[nAxis].nOffset = theApp.m_Axes[nAxis].nPos - (int)(fTip / theApp.m_Axes[nAxis].fStepSize);
		UpdateAxesSheet();
		break;
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		if (theApp.m_Motors.WaitMotionFinished(1.0) == true) {
			nOffset = theApp.m_Axes[nAxis].nPos - (int)(fTip / theApp.m_Axes[nAxis].fStepSize);
			theApp.m_FBCtrl.SetHomePos(nAxis, theApp.m_Axes[nAxis].nHomePos + nOffset);
		}
		break;
	}
	theApp.SaveTank();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;

}

void CBallPage::OnButtonSetZtipLeft() 
{
	CCoord CpHead;
	CString Buff;
	float fTip;
	int nOffset;

	m_editZTipLeft.GetWindowText(Buff);
	_WTOF(Buff,fTip);

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	switch (theApp.m_Tank.nScannerDescription) {
	default:

		if (theApp.m_Axes[theApp.m_Tank.nXtLeft].nMotorType == STEPPER) {
			theApp.m_Thread.m_nThreadAction = JOYSTICK_START; //required so we have irqs to keep npos up to date
			Sleep(200);
		}
		else {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
		}


		if (theApp.m_nSide0Orientation == 0) {
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = 0;
			Sleep(100);
			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
		}
		else {
			theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = 0;
			Sleep(100);
			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
		}
		UpdateAxesSheet();
		break;
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
	case TANK_5AXIS:
		if (theApp.m_Motors.WaitMotionFinished(1.0) == true) {
			if (theApp.m_nSide0Orientation == 0) {
				nOffset = theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
				theApp.m_FBCtrl.SetHomePos(theApp.m_Tank.nYtLeft, theApp.m_Axes[theApp.m_Tank.nYtLeft].nHomePos + nOffset);
			}
			else {
				nOffset = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
				theApp.m_FBCtrl.SetHomePos(theApp.m_Tank.nYtRight, theApp.m_Axes[theApp.m_Tank.nYtRight].nHomePos + nOffset);
			}
		}
		break;
	}
	theApp.SaveTank();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;

}

void CBallPage::OnButtonSetXtipRight() 
{
	CCoord CpHead;
	CString Buff;
	float fTip;
	int nOffset;

	m_editXTipRight.GetWindowText(Buff);
	_WTOF(Buff,fTip);

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	switch (theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
		if(theApp.m_nSide0Orientation==0) {
			theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset = 0;
			Sleep(100);
			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset = theApp.m_Axes[theApp.m_Tank.nXtRight].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		} else {
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset = 0;
			Sleep(100);
			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
		}
		UpdateAxesSheet();
		break;
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		if (theApp.m_Motors.WaitMotionFinished(1.0) == true) {
			if (theApp.m_nSide0Orientation == 0) {
				nOffset = theApp.m_Axes[theApp.m_Tank.nXtRight].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
				theApp.m_FBCtrl.SetHomePos(theApp.m_Tank.nXtRight, theApp.m_Axes[theApp.m_Tank.nXtRight].nHomePos + nOffset);
			}
			else {
				nOffset = theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
				theApp.m_FBCtrl.SetHomePos(theApp.m_Tank.nXtLeft, theApp.m_Axes[theApp.m_Tank.nXtLeft].nHomePos + nOffset);
			}
		}
		break;
	}
	theApp.SaveTank();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
}

void CBallPage::OnButtonSetZtipRight() 
{
	CCoord CpHead;
	CString Buff;
	float fTip;
	int nOffset;

	m_editZTipRight.GetWindowText(Buff);
	_WTOF(Buff,fTip);

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	switch (theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
		if(theApp.m_nSide0Orientation==0) {
			theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = 0;
			Sleep(100);
			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
		} else {
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = 0;
			Sleep(100);
			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
		}
		UpdateAxesSheet();
		break;
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		if (theApp.m_Motors.WaitMotionFinished(1.0) == true) {
			if (theApp.m_nSide0Orientation == 0) {
				nOffset = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
				theApp.m_FBCtrl.SetHomePos(theApp.m_Tank.nYtRight, theApp.m_Axes[theApp.m_Tank.nYtRight].nHomePos + nOffset);
			}
			else {
				nOffset = theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos - (int)(fTip / theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
				theApp.m_FBCtrl.SetHomePos(theApp.m_Tank.nYtLeft, theApp.m_Axes[theApp.m_Tank.nYtLeft].nHomePos + nOffset);
			}
		}
		break;
	}
	theApp.SaveTank();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;

}


void CBallPage::OnButtonXyzLeft() 
{
	CCoord CpSurface,CpHead;
	CString Buff,Msg;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		if(theApp.m_Axes[theApp.m_Tank.nXLeft].nMotorType==STEPPER){
			theApp.m_Thread.m_nThreadAction = JOYSTICK_START; //required so we have irqs to keep npos up to date
			Sleep(200);
		}else{
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
		}
		
		theApp.m_Motors.GetSurfacePos(&CpSurface);

		if(theApp.m_nSide0Orientation==0) {
			m_editXLeft.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side0.fX);
			m_editYLeft.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side0.fY);
			m_editZLeft.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side0.fZ);

			theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset = 0;
			theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset = 0;
			theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset = 0;
			Sleep(100);
			theApp.m_Ethercat.InvalidateRobotJoints();
			switch(theApp.m_LastSettings.nPositionDisplayMode) {
			default:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
				break;
			case 3:
				theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,FALSE);
				break;
			}
			if(theApp.m_LastSettings.fZeroBallDiameter>0.0f) {
				Buff.LoadString(IDS_Is_the_ball_bearing_in_place);
				Msg.LoadString(IDS_Question);
				if(MessageBox(Buff,Msg,MB_YESNO)==IDYES) {
					CpHead.Side0.fX += ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[PORTSIDE].fI);
					CpHead.Side0.fY += ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[PORTSIDE].fJ);
					CpHead.Side0.fZ += ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[PORTSIDE].fK);
				}
			}
			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nXLeft].nPos - (int)(CpHead.Side0.fX / theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nYLeft].nPos - (int)(CpHead.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nZLeft].nPos - (int)(CpHead.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
		} else {
			m_editXLeft.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side1.fX);
			m_editYLeft.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side1.fY);
			m_editZLeft.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side1.fZ);

			theApp.m_Axes[theApp.m_Tank.nXRight].nOffset = 0;
			theApp.m_Axes[theApp.m_Tank.nYRight].nOffset = 0;
			theApp.m_Axes[theApp.m_Tank.nZRight].nOffset = 0;
			Sleep(100);
			theApp.m_Ethercat.InvalidateRobotJoints();
			switch(theApp.m_LastSettings.nPositionDisplayMode) {
			default:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
				break;
			case 3:
				theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,FALSE);
				break;
			}
			if(theApp.m_LastSettings.fZeroBallDiameter>0.0f) {
				Buff.LoadString(IDS_Is_the_ball_bearing_in_place);
				Msg.LoadString(IDS_Question);
				if(MessageBox(Buff,Msg,MB_YESNO)==IDYES) {
					CpHead.Side1.fX -= ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[STARBOARD].fI);
					CpHead.Side1.fY -= ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[STARBOARD].fJ);
					CpHead.Side1.fZ -= ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[STARBOARD].fK);
				}
			}
			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Axes[theApp.m_Tank.nXRight].nOffset = theApp.m_Axes[theApp.m_Tank.nXRight].nPos - (int)(CpHead.Side1.fX / theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYRight].nOffset = theApp.m_Axes[theApp.m_Tank.nYRight].nPos - (int)(CpHead.Side1.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nZRight].nOffset = theApp.m_Axes[theApp.m_Tank.nZRight].nPos - (int)(CpHead.Side1.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		}
		UpdateAxesSheet();
		theApp.SaveTank();
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		theApp.m_nSide0Orientation == 0 ? theApp.m_Kinematics.ZeroFrame(0) : theApp.m_Kinematics.ZeroFrame(1);
		theApp.SaveTank();
		break;
	}


}

void CBallPage::OnButtonXyzRight() 
{
	CCoord CpSurface,CpHead;
	CString Buff,Msg;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);

		theApp.m_Motors.GetSurfacePos(&CpSurface);


		if(theApp.m_nSide0Orientation==0) {
			m_editXRight.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side1.fX);
			m_editYRight.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side1.fY);
			m_editZRight.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side1.fZ);

			theApp.m_Axes[theApp.m_Tank.nXRight].nOffset = 0;
			theApp.m_Axes[theApp.m_Tank.nYRight].nOffset = 0;
			theApp.m_Axes[theApp.m_Tank.nZRight].nOffset = 0;
			Sleep(100);
			theApp.m_Ethercat.InvalidateRobotJoints();
			switch(theApp.m_LastSettings.nPositionDisplayMode) {
			default:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
				break;
			case 3:
				theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,FALSE);
				break;
			}
			if(theApp.m_LastSettings.fZeroBallDiameter>0.0f) {
				Buff.LoadString(IDS_Is_the_ball_bearing_in_place);
				Msg.LoadString(IDS_Question);
				if(MessageBox(Buff,Msg,MB_YESNO)==IDYES) {
					CpHead.Side1.fX -= ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[STARBOARD].fI);
					CpHead.Side1.fY -= ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[STARBOARD].fJ);
					CpHead.Side1.fZ -= ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[STARBOARD].fK);
				}
			}
			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Axes[theApp.m_Tank.nXRight].nOffset = theApp.m_Axes[theApp.m_Tank.nXRight].nPos - (int)(CpHead.Side1.fX / theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYRight].nOffset = theApp.m_Axes[theApp.m_Tank.nYRight].nPos - (int)(CpHead.Side1.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nZRight].nOffset = theApp.m_Axes[theApp.m_Tank.nZRight].nPos - (int)(CpHead.Side1.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		} else {
			m_editXRight.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side0.fX);
			m_editYRight.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side0.fY);
			m_editZRight.GetWindowText(Buff); _WTOF(Buff,CpSurface.Side0.fZ);

			theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset = 0;
			theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset = 0;
			theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset = 0;
			Sleep(100);
			theApp.m_Ethercat.InvalidateRobotJoints();
			switch(theApp.m_LastSettings.nPositionDisplayMode) {
			default:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
				break;
			case 3:
				theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,FALSE);
				break;
			}
			if(theApp.m_LastSettings.fZeroBallDiameter>0.0f) {
				Buff.LoadString(IDS_Is_the_ball_bearing_in_place);
				Msg.LoadString(IDS_Question);
				if(MessageBox(Buff,Msg,MB_YESNO)==IDYES) {
					CpHead.Side0.fX += ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[PORTSIDE].fI);
					CpHead.Side0.fY += ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[PORTSIDE].fJ);
					CpHead.Side0.fZ += ((theApp.m_LastSettings.fZeroBallDiameter/2.0f) * CpSurface.Side[PORTSIDE].fK);
				}
			}
			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nXLeft].nPos - (int)(CpHead.Side0.fX / theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nYLeft].nPos - (int)(CpHead.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nZLeft].nPos - (int)(CpHead.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
		}

		UpdateAxesSheet();
		theApp.SaveTank();
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		theApp.m_nSide0Orientation == 0 ? theApp.m_Kinematics.ZeroFrame(1) : theApp.m_Kinematics.ZeroFrame(0);
		theApp.SaveTank();
		break;
	}
}

void CBallPage::OnButtonZeroBridge() 
{
	CCoord CpSurface,CpHead;
	CString Buff,Msg;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
	CpSurface.Zero();
	int nLeft, nRight;
	float KinematicOffsetDiff;

	theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset = 0;
	Sleep(100);
	theApp.m_Ethercat.InvalidateRobotJoints();
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	default:
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
		break;
	case 3:
		theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,FALSE);
		break;
	}

	switch(theApp.m_nSide0Orientation) {
	case 0: nLeft = PORTSIDE;
		nRight = STARBOARD;
		break;
	case 1: nLeft = STARBOARD;
		nRight = PORTSIDE;
		break;
	}


	//KinematicOffsetDiff is for NLR type systems where X offsets differnet (namely GKN) 
	theApp.m_Ethercat.InvalidateRobotJoints();
	KinematicOffsetDiff = abs((theApp.m_Kinematics.m_fXOffset[nLeft]-theApp.m_Kinematics.m_fXOffset[nRight])/2);
	theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset = theApp.m_Axes[theApp.m_Tank.nXBridge].nPos - (int)(KinematicOffsetDiff / theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset  += (int)(KinematicOffsetDiff / theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nXRight].nOffset  += (int)(KinematicOffsetDiff / theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);

	UpdateAxesSheet();
	theApp.SaveTank();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;

}


void CBallPage::OnButtonOpen() 
{
	CString FilePath,Buff;
	float fVersion;
	CUSLFile File;
	CFileException e;
	int nKinematicOffset[32],nLimitSwitchOffset[32];
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	CFileDialog FileDlg(TRUE,_T(".kin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Kinematics (*.kin)|*.kin|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			if(theApp.m_fVersion<4.35f) {
				File.Read(nKinematicOffset,sizeof nKinematicOffset);
				File.Read(nLimitSwitchOffset,sizeof nLimitSwitchOffset);
				File.Read(theApp.m_LastSettings.nManipulatorType, sizeof theApp.m_LastSettings.nManipulatorType);
				theApp.m_Kinematics.RetrieveManipulator(&File);
			} else {
				File.Read(&fVersion,sizeof fVersion);
				File.Read(nKinematicOffset,sizeof nKinematicOffset);
				File.Read(nLimitSwitchOffset,sizeof nLimitSwitchOffset);
				File.Read(theApp.m_LastSettings.nManipulatorType, sizeof theApp.m_LastSettings.nManipulatorType);
				File.Read(&theApp.m_Kinematics.nLimitSwitchOffsetLoadMask, sizeof theApp.m_Kinematics.nLimitSwitchOffsetLoadMask);
				File.Read(&theApp.m_Kinematics.nKinematicOffsetLoadMask, sizeof theApp.m_Kinematics.nKinematicOffsetLoadMask);
				theApp.m_Kinematics.RetrieveManipulator(&File);
			}


			File.Close();

			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.SuspendMotors();

			for(int ii=0;ii<32;ii++) {
				theApp.m_Axes[ii].nOffset = nKinematicOffset[ii];
				theApp.m_Axes[ii].nLimitSwitchOffset = nLimitSwitchOffset[ii];
			}

			theApp.m_PmacUser.DownloadLimitSwitchOffsets();

			theApp.m_LimitAndKinematicOffset = FileDlg.GetFileTitle();
			UpdateAllControls();
			pFrame->InvalidateWaterPaths();
			UpdateAxesSheet();
			theApp.SaveTank();
			theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		}
	}
	
}

void CBallPage::OnButtonSave() 
{
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	int nKinematicOffset[32],nLimitSwitchOffset[32];


	CFileDialog FileDlg(FALSE,_T(".kin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Kinematics (*.kin)|*.kin|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {

			for(int ii=0;ii<32;ii++) {
				nKinematicOffset[ii] = theApp.m_Axes[ii].nOffset;
				nLimitSwitchOffset[ii] = theApp.m_Axes[ii].nLimitSwitchOffset;
			}
			File.Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);
			File.Write(nKinematicOffset,sizeof nKinematicOffset);
			File.Write(nLimitSwitchOffset,sizeof nLimitSwitchOffset);
			File.Write(theApp.m_LastSettings.nManipulatorType, sizeof theApp.m_LastSettings.nManipulatorType);
			File.Write(&theApp.m_Kinematics.nLimitSwitchOffsetLoadMask, sizeof theApp.m_Kinematics.nLimitSwitchOffsetLoadMask);
			File.Write(&theApp.m_Kinematics.nKinematicOffsetLoadMask, sizeof theApp.m_Kinematics.nKinematicOffsetLoadMask);
			theApp.m_Kinematics.SaveManipulator(&File);
			File.Close();

			theApp.m_LimitAndKinematicOffset = FileDlg.GetFileTitle();
			UpdateAllControls();
		}
	}
	
}

void CBallPage::OnDeltaposSpinXtLeft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	
	m_editXTipLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Thread.m_fXtMove);
	theApp.m_Thread.m_fXtMove -= ((float)pNMUpDown->iDelta * 45.0f);

	if(theApp.m_Thread.m_fXtMove < -90.0f) theApp.m_Thread.m_fXtMove = -90.0f;
	if(theApp.m_Thread.m_fXtMove >  90.0f) theApp.m_Thread.m_fXtMove =  90.0f;

	Buff.Format(_T("%.01f"),theApp.m_Thread.m_fXtMove );
	m_editXTipLeft.SetWindowText(Buff);

	*pResult = 0;
}

void CBallPage::OnDeltaposSpinXtRight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	
	m_editXTipRight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Thread.m_fXtMove);
	theApp.m_Thread.m_fXtMove -= ((float)pNMUpDown->iDelta * 45.0f);

	if(theApp.m_Thread.m_fXtMove < -90.0f) theApp.m_Thread.m_fXtMove = -90.0f;
	if(theApp.m_Thread.m_fXtMove >  90.0f) theApp.m_Thread.m_fXtMove =  90.0f;

	Buff.Format(_T("%.01f"),theApp.m_Thread.m_fXtMove );
	m_editXTipRight.SetWindowText(Buff);

	*pResult = 0;
}

void CBallPage::OnDeltaposSpinZtLeft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	
	m_editZTipLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Thread.m_fYtMove);
	theApp.m_Thread.m_fYtMove -= ((float)pNMUpDown->iDelta * 45.0f);

	if(theApp.m_Thread.m_fYtMove < -90.0f) theApp.m_Thread.m_fYtMove = -90.0f;
	if(theApp.m_Thread.m_fYtMove >  90.0f) theApp.m_Thread.m_fYtMove =  90.0f;

	Buff.Format(_T("%.01f"),theApp.m_Thread.m_fYtMove );
	m_editZTipLeft.SetWindowText(Buff);

	*pResult = 0;
}

void CBallPage::OnDeltaposSpinZtRight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	
	m_editZTipRight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Thread.m_fYtMove);
	theApp.m_Thread.m_fYtMove -= ((float)pNMUpDown->iDelta * 45.0f);

	if(theApp.m_Thread.m_fYtMove < -90.0f) theApp.m_Thread.m_fYtMove = -90.0f;
	if(theApp.m_Thread.m_fYtMove >  90.0f) theApp.m_Thread.m_fYtMove =  90.0f;

	Buff.Format(_T("%.01f"),theApp.m_Thread.m_fYtMove );
	m_editZTipRight.SetWindowText(Buff);

	*pResult = 0;
}

void CBallPage::OnChangeEditLeftPos() 
{
	CString Buff;
	int nStart;

	m_editLeftPos.GetWindowText(Buff);
	Buff.Remove(' ');
	Buff.MakeUpper();

	if(theApp.m_nSide0Orientation==0) {
		if((nStart=Buff.Find(L"X:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"X:%f",&theApp.m_CpBall.Side0.fX);
		if((nStart=Buff.Find(L"Y:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Y:%f",&theApp.m_CpBall.Side0.fY);
		if((nStart=Buff.Find(L"Z:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Z:%f",&theApp.m_CpBall.Side0.fZ);
	} else {
		if((nStart=Buff.Find(L"X:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"X:%f",&theApp.m_CpBall.Side1.fX);
		if((nStart=Buff.Find(L"Y:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Y:%f",&theApp.m_CpBall.Side1.fY);
		if((nStart=Buff.Find(L"Z:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Z:%f",&theApp.m_CpBall.Side1.fZ);
	};
}

void CBallPage::OnChangeEditRightPos() 
{
	CString Buff;
	int nStart;

	m_editRightPos.GetWindowText(Buff);
	Buff.Remove(' ');
	Buff.MakeUpper();

	if(theApp.m_nSide0Orientation==0) {
		if((nStart=Buff.Find(L"X:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"X:%f",&theApp.m_CpBall.Side1.fX);
		if((nStart=Buff.Find(L"Y:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Y:%f",&theApp.m_CpBall.Side1.fY);
		if((nStart=Buff.Find(L"Z:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Z:%f",&theApp.m_CpBall.Side1.fZ);
	} else {
		if((nStart=Buff.Find(L"X:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"X:%f",&theApp.m_CpBall.Side0.fX);
		if((nStart=Buff.Find(L"Y:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Y:%f",&theApp.m_CpBall.Side0.fY);
		if((nStart=Buff.Find(L"Z:",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Z:%f",&theApp.m_CpBall.Side0.fZ);
	};
}

void CBallPage::OnButtonGotoBoth() 
{
	CString Buff;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nNormSide = 2;

	m_editXTipLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Thread.m_fXtMove);
	m_editZTipLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Thread.m_fYtMove);
	theApp.m_Thread.m_nThreadAction=MOVE_ROUND_BALL;
	
}

void CBallPage::OnButtonZeroZero() 
{
	CString Buff;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nNormSide = 2;

	theApp.m_Thread.m_fXtMove = 0.0f;
	theApp.m_Thread.m_fYtMove = 0.0f;
	theApp.m_Thread.m_nThreadAction=MOVE_ROUND_BALL;
}


void CBallPage::UpdateAxesSheet()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(UI_UPDATE_AXES,NULL,NULL);
}

void CBallPage::SetAccessPrivelages()
{
	CButton* pButton;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
		if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_SET_XTIP_LEFT)) pButton->EnableWindow(false);
		if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_SET_ZTIP_LEFT)) pButton->EnableWindow(false);
		if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_SET_XTIP_RIGHT)) pButton->EnableWindow(false);
		if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_SET_ZTIP_RIGHT)) pButton->EnableWindow(false);
		break;
	}
}

void CBallPage::OnButtonSide1EqualSide0() 
{
	CCoord CpSurface;
	float fX, fY, fZ;
	int nRobot = 1;

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	switch (theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:

		fX = CpSurface.Side[1].fX - CpSurface.Side[0].fX;
		fY = CpSurface.Side[1].fY - CpSurface.Side[0].fY;
		fZ = CpSurface.Side[1].fZ - CpSurface.Side[0].fZ;

		if (nRobot == 0) {
			//this hasn't been checked
			theApp.m_Robot[nRobot].m_vBase.x += fX;
			theApp.m_Robot[nRobot].m_vBase.y += fY;
			theApp.m_Robot[nRobot].m_vBase.z += fZ;
		}
		else {
			//this has been checked
			theApp.m_Robot[nRobot].m_vBase.x -= fX;
			theApp.m_Robot[nRobot].m_vBase.y -= fY;
			theApp.m_Robot[nRobot].m_vBase.z -= fZ;
		};

		break;
	case TWIN_TOWER_KINEMATIC:
		theApp.m_Tank.vFrameZero[1] -= (CpSurface.Side1.pt - CpSurface.Side0.pt);

		break;
	};

}
