// OriginPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "OriginPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COriginPage property page

IMPLEMENT_DYNCREATE(COriginPage, CPropertyPage)

COriginPage::COriginPage() : CPropertyPage(COriginPage::IDD)
{
	//{{AFX_DATA_INIT(COriginPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

COriginPage::~COriginPage()
{
}

void COriginPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COriginPage)
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	DDX_Control(pDX, IDC_BUTTON_PARK, m_buttonPark);
	DDX_Control(pDX, IDC_BUTTON_X_BRIDGE, m_buttonXBridge);
	//}}AFX_DATA_MAP
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_SAVE, m_checkSave);
	DDX_Control(pDX, IDC_STATIC_SAVE, m_staticSave);
}


BEGIN_MESSAGE_MAP(COriginPage, CPropertyPage)
	//{{AFX_MSG_MAP(COriginPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_X_LEFT, OnButtonXLeft)
	ON_BN_CLICKED(IDC_BUTTON_YLEFT, OnButtonYleft)
	ON_BN_CLICKED(IDC_BUTTON_ZLEFT, OnButtonZleft)
	ON_BN_CLICKED(IDC_BUTTON_XTLEFT, OnButtonXtleft)
	ON_BN_CLICKED(IDC_BUTTON_YTLEFT, OnButtonYtleft)
	ON_BN_CLICKED(IDC_BUTTON_X_RIGHT, OnButtonXRight)
	ON_BN_CLICKED(IDC_BUTTON_YRIGHT, OnButtonYright)
	ON_BN_CLICKED(IDC_BUTTON_ZRIGHT, OnButtonZright)
	ON_BN_CLICKED(IDC_BUTTON_XTRIGHT, OnButtonXtright)
	ON_BN_CLICKED(IDC_BUTTON_YTRIGHT, OnButtonYtright)
	ON_BN_CLICKED(IDC_BUTTON_ORIGINALL, OnButtonOriginall)
	ON_BN_CLICKED(IDC_BUTTON_XMOVING, OnButtonXmoving)
	ON_BN_CLICKED(IDC_BUTTON_ALL_LEFT, OnButtonAllLeft)
	ON_BN_CLICKED(IDC_BUTTON_ALL_RIGHT, OnButtonAllRight)
	ON_BN_CLICKED(IDC_BUTTON_HOLDING_Z_FIXED, OnButtonHoldingZFixed)
	ON_BN_CLICKED(IDC_BUTTON_HOLDING_Y_FIXED, OnButtonHoldingYFixed)
	ON_BN_CLICKED(IDC_BUTTON_HOLDING_X_FIXED, OnButtonHoldingXFixed)
	ON_BN_CLICKED(IDC_BUTTON_HOLDING_X, OnButtonHoldingX)
	ON_BN_CLICKED(IDC_BUTTON_HOLDING_Y_MOVING, OnButtonHoldingYMoving)
	ON_BN_CLICKED(IDC_BUTTON_HOLDING_Z_MOVING, OnButtonHoldingZMoving)
	ON_BN_CLICKED(IDC_BUTTON_X_BRIDGE, OnButtonXBridge)
	ON_BN_CLICKED(IDC_BUTTON_TURNTABLE, OnButtonTurntable)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BUTTON_PARK, OnButtonPark)
	ON_BN_CLICKED(IDC_BUTTON_R_LEFT, OnButtonRLeft)
	ON_BN_CLICKED(IDC_BUTTON_HOME_FAILED, OnButtonHomeFailed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COriginPage message handlers

BOOL COriginPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	if (theApp.m_nSide0Orientation == 0) {
		m_staticLeft.SetWindowText(theApp.m_DlgSideName[0]);
		m_staticRight.SetWindowText(theApp.m_DlgSideName[1]);
	}
	else {
		m_staticLeft.SetWindowText(theApp.m_DlgSideName[1]);
		m_staticRight.SetWindowText(theApp.m_DlgSideName[0]);
	}
	
	SetAccessPrivelage();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	switch (theApp.m_Tank.nScannerDescription) {
	//'Save' check box disabled and hidden by default.
	default:
		m_checkSave.ShowWindow(SW_HIDE);
		m_checkSave.EnableWindow(false);
		m_staticSave.ShowWindow(SW_HIDE);
		m_staticSave.EnableWindow(false);
		break;
	case 999:	//Shut the compiler up
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH COriginPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void COriginPage::OnButtonXBridge() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_X_BRIDGE;
	
}

void COriginPage::OnButtonXLeft() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_X_LEFT : theApp.m_Thread.m_nThreadAction = ORIGIN_X_RIGHT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_X_LEFT);
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x0002;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case SPX_ROBOT:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x0002;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case RAILWAY_AXLE:
		theApp.m_Motors.OriginAll(0,0x01);
		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		switch (theApp.m_Axes[theApp.m_Tank.nXLeft].nHomeMode) {
		case 0:
			break;
		case 1:	
			theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis - 1);
			break;
		case 2:
			break;
		}
		break;
	}
}

void COriginPage::OnButtonYleft() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_Y_LEFT : theApp.m_Thread.m_nThreadAction = ORIGIN_Y_RIGHT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_Y_LEFT);
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x0001;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case SPX_ROBOT:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x0001;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case RAILWAY_AXLE:
		theApp.m_Motors.OriginAll(0,0x02);
		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		switch (theApp.m_Axes[theApp.m_Tank.nYLeft].nHomeMode) {
		case 0:
			break;
		case 1:	
			theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis - 1);
			break;
		case 2:
			break;
		}
		break;
	}
}

void COriginPage::OnButtonZleft() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_Z_LEFT : theApp.m_Thread.m_nThreadAction = ORIGIN_Z_RIGHT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_Z_LEFT);
			break;
		}
		break;
		case DUAL_ROBOT_9_PLUS_9:
		case SPX_ROBOT:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x0004;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case RAILWAY_AXLE:
		theApp.m_Motors.OriginAll(0,0x04);
		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		switch (theApp.m_Axes[theApp.m_Tank.nZLeft].nHomeMode) {
		case 0:
			break;
		case 1:	
			theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis - 1);
			break;
		case 2:
			break;
		}
		break;
	}
}

void COriginPage::OnButtonXtleft() 
{
	switch (theApp.m_Tank.nScannerDescription) {
	default:
		switch (theApp.m_nMotorType) {
		default:
			if(theApp.m_LastSettings.nManipulatorType[PORTSIDE] == ROTATE_SWIVEL_GIMBAL) {
				theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset = 0;
			}
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_XT_LEFT : theApp.m_Thread.m_nThreadAction = ORIGIN_XT_RIGHT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_XT_LEFT);
			break;
		}

		break;
	case RAILWAY_AXLE:
		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		switch (theApp.m_Axes[theApp.m_Tank.nXtLeft].nHomeMode) {
		case 0:
			break;
		case 1:	
			theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis - 1);
			break;
		case 2:
			break;
		}
		break;
	}
}

void COriginPage::OnButtonYtleft() 
{
	switch (theApp.m_Tank.nScannerDescription) {
	default:
		switch (theApp.m_nMotorType) {
		default:
			if(theApp.m_LastSettings.nManipulatorType[PORTSIDE] == ROTATE_SWIVEL_GIMBAL) {
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = 0;
			}
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_YT_LEFT : theApp.m_Thread.m_nThreadAction = ORIGIN_YT_RIGHT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_YT_LEFT);
			break;
		}
		break;
	case RAILWAY_AXLE:
		theApp.m_Motors.OriginAll(0,0x08);
		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		switch (theApp.m_Axes[theApp.m_Tank.nYtLeft].nHomeMode) {
		case 0:
			break;
		case 1:	
			theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis - 1);
			break;
		case 2:
			break;
		}
		break;
	}
}

void COriginPage::OnButtonXRight() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_X_RIGHT : theApp.m_Thread.m_nThreadAction = ORIGIN_X_LEFT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_X_RIGHT);
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x0800;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case RAILWAY_AXLE:
		theApp.m_Motors.OriginAll(0,0x01);
		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		switch (theApp.m_Axes[theApp.m_Tank.nXRight].nHomeMode) {
		case 0:
			break;
		case 1:	
			theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis - 1);
			break;
		case 2:
			break;
		}
		break;
	}
}

void COriginPage::OnButtonYright() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_Y_RIGHT : theApp.m_Thread.m_nThreadAction = ORIGIN_Y_LEFT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_Y_RIGHT);
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x0400;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case RAILWAY_AXLE:
		theApp.m_Motors.OriginAll(0,0x10);
		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		switch (theApp.m_Axes[theApp.m_Tank.nYRight].nHomeMode) {
		case 0:
			break;
		case 1:	
			theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis - 1);
			break;
		case 2:
			break;
		}
		break;
	}
}

void COriginPage::OnButtonZright() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_Z_RIGHT : theApp.m_Thread.m_nThreadAction = ORIGIN_Z_LEFT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_Z_RIGHT);
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x1000;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case RAILWAY_AXLE:
		theApp.m_Motors.OriginAll(0,0x20);
		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		switch (theApp.m_Axes[theApp.m_Tank.nZRight].nHomeMode) {
		case 0:
			break;
		case 1:	
			theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis - 1);
			break;
		case 2:
			break;
		}
		break;
	}
}

void COriginPage::OnButtonXtright() 
{
	switch (theApp.m_Tank.nScannerDescription) {
	default:
		switch (theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation == 0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_XT_RIGHT : theApp.m_Thread.m_nThreadAction = ORIGIN_XT_LEFT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_XT_RIGHT);
			break;
		}
		break;
	case RAILWAY_AXLE:
		theApp.m_Motors.OriginAll(0, 0x20);
		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		switch (theApp.m_Axes[theApp.m_Tank.nXtRight].nHomeMode) {
		case 0:
			break;
		case 1:
			theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis - 1);
			break;
		case 2:
			break;
		}
		break;
	}
}

void COriginPage::OnButtonYtright() 
{
	switch (theApp.m_Tank.nScannerDescription) {
	default:
		switch (theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_YT_RIGHT : theApp.m_Thread.m_nThreadAction = ORIGIN_YT_LEFT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_YT_RIGHT);
			break;
		}
		break;
	case RAILWAY_AXLE:
		theApp.m_Motors.OriginAll(0,0x40);
		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		switch (theApp.m_Axes[theApp.m_Tank.nYtRight].nHomeMode) {
		case 0:
			break;
		case 1:	
			theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis - 1);
			break;
		case 2:
			break;
		}
		break;
	}

}


void COriginPage::OnButtonOriginall() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nThreadAction = ORIGIN_ALL;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_ALL);
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x1c07;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case SPX_ROBOT:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x3006;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case RAILWAY_AXLE:
	case TANK_5AXIS:
		if (m_checkSave.GetCheck()) {
			for (int i = 0; i < 5; i++) {
				theApp.m_FBCtrl.SetHomePosToEncoderPos(1);
			}
		} else {
			theApp.m_Motors.OriginAll(0, 0xff);
		}
		break;
	}
}

void COriginPage::OnButtonXmoving() 
{
	CString Buff,Warning;
	
	Buff.LoadString(IDS_Is_It_Safe_To_Carry);
	Warning.LoadString(IDS_Warning_Fixture_Movement);
	if(MessageBox(Buff,Warning,MB_ICONWARNING|MB_YESNO)==IDYES) {
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_HOLDING_FIXTURE;
	}
}
/*
void COriginPage::OnButtonYmoving() 
{
	
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_Y_MOVING;
}

void COriginPage::OnButtonZmoving() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_Z_MOVING;
}

void COriginPage::OnButtonYfixed() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_Y_FIXED;
}

void COriginPage::OnButtonZfixed() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_Z_FIXED;
}
*/
BOOL COriginPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetAccessPrivelage();
	return CPropertyPage::OnSetActive();
}

void COriginPage::OnButtonAllLeft() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_nMotorType) {
		default:
			if(theApp.m_LastSettings.nManipulatorType[PORTSIDE] == ROTATE_SWIVEL_GIMBAL) {
				theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset = 0;
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = 0;
				theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset = 0;
			}
			
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_LEFT : theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_RIGHT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_ALL_LEFT);
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x0007;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case SPX_ROBOT:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x0006;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case TANK_5AXIS:
		theApp.m_Motors.OriginAll(0, 0x1f);
		break;
	}
}

void COriginPage::OnButtonAllRight() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_RIGHT : theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_LEFT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_ALL_RIGHT);
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x1c00;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	case SPX_ROBOT:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x1000;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	}
}

void COriginPage::OnButtonHoldingZFixed() 
{
	CString Buff,Warning;
	
	Buff.LoadString(IDS_Is_It_Safe_To_Carry);
	Warning.LoadString(IDS_Warning_Fixture_Movement);
	if(MessageBox(Buff,Warning,MB_ICONWARNING|MB_YESNO)==IDYES) {
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_Z_FIXED;
	}
}

void COriginPage::OnButtonHoldingYFixed() 
{
	CString Buff,Warning;
	
	Buff.LoadString(IDS_Is_It_Safe_To_Carry);
	Warning.LoadString(IDS_Warning_Fixture_Movement);
	if(MessageBox(Buff,Warning,MB_ICONWARNING|MB_YESNO)==IDYES) {
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_Y_FIXED;
	}
	
}

void COriginPage::OnButtonHoldingXFixed() 
{
	CString Buff,Warning;
	
	Buff.LoadString(IDS_Is_It_Safe_To_Carry);
	Warning.LoadString(IDS_Warning_Fixture_Movement);
	if(MessageBox(Buff,Warning,MB_ICONWARNING|MB_YESNO)==IDYES) {
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_X_FIXED;
	}
}

void COriginPage::OnButtonHoldingX() 
{
	CString Buff,Warning;
	
	Buff.LoadString(IDS_Is_It_Safe_To_Carry);
	Warning.LoadString(IDS_Warning_Fixture_Movement);
	if(MessageBox(Buff,Warning,MB_ICONWARNING|MB_YESNO)==IDYES) {
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_X_MOVING;
	}
}

void COriginPage::OnButtonHoldingYMoving() 
{
	CString Buff,Warning;
	
	Buff.LoadString(IDS_Is_It_Safe_To_Carry);
	Warning.LoadString(IDS_Warning_Fixture_Movement);
	if(MessageBox(Buff,Warning,MB_ICONWARNING|MB_YESNO)==IDYES) {
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_Y_MOVING;
	}
}

void COriginPage::OnButtonHoldingZMoving() 
{
	CString Buff,Warning;
	
	Buff.LoadString(IDS_Is_It_Safe_To_Carry);
	Warning.LoadString(IDS_Warning_Fixture_Movement);
	if(MessageBox(Buff,Warning,MB_ICONWARNING|MB_YESNO)==IDYES) {
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_Z_MOVING;
	}
}


void COriginPage::OnButtonTurntable() 
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_nMotorType) {
		default:
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nThreadAction = ORIGIN_R_LEFT;
			break;
		case FIELD_BUS: theApp.m_FBCtrl.Origin(ORIGIN_R_LEFT);
			break;
		}
		break;
	case SPX_ROBOT:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nRoboAxisMask = 0x2000;
		theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
		break;
	}

}


BOOL COriginPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.m_Help.Open(_T("MotionTools_Origin.html"),NULL);
	
	return CPropertyPage::OnHelpInfo(pHelpInfo);
}

void COriginPage::OnButtonPark() 
{
	for(int ii=0;ii<12;ii++){
		if(!(theApp.m_Axes[ii].nStatus&0x400l)){
			MessageBox(_T("All axes must be homed to perform this function"),_T("Error"),NULL);
			return;
		}
	}

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=MOVETO_PARK;
	

}

void COriginPage::SetAccessPrivelage()
{
	if (GetSafeHwnd() == NULL) return;
	bool bFlagShow,bFlagEnable;
	CWnd* pWnd;

	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_ORIGIN_PAGE] ? bFlagEnable = TRUE : bFlagEnable = FALSE;

	switch(theApp.m_Tank.nScannerDescription) {
	default:

		theApp.m_Tank.bHoldingFixture != TRUE ? bFlagShow = false : bFlagShow = true;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_HOLDING),			bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XMOVING),			bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Z_FIXED),	bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Y_FIXED),	bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_X_FIXED),	bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_X),		bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Y_MOVING),	bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Z_MOVING),	bFlagShow,bFlagEnable);


		theApp.m_Tank.bTurnTable != TRUE ? bFlagShow = false : bFlagShow = true;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_TURNTABLE),		bFlagShow,bFlagEnable);

		theApp.m_Tank.bBridge != TRUE ? bFlagShow = false : bFlagShow = true;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_BRIDGE),			bFlagShow,bFlagEnable);

		theApp.m_Tank.nScannerDescription == SINGLE_BRIDGE_TANK ? bFlagShow = false : bFlagShow = true;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ORIGINALL),	true,bFlagEnable);

		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_LEFT),			true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ALL_LEFT),		true,bFlagEnable);

		theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_LEFT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YLEFT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ZLEFT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XTLEFT),		bFlagShow,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nXtLeft].cName);
		theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YTLEFT),		bFlagShow,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nYtLeft].cName);
		theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_R_LEFT),		bFlagShow,bFlagEnable);


		theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_RIGHT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YRIGHT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ZRIGHT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XTRIGHT),		bFlagShow,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nXtRight].cName);
		theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YTRIGHT),		bFlagShow,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nYtRight].cName);

		bFlagShow = false;
		if( theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable || theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable ||	theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable
			|| theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable || 	theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable ) {
			bFlagShow=true;
		}
		
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_RIGHT),		bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ALL_RIGHT),	bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ORIGINALL),	bFlagShow,bFlagEnable);

		if(theApp.m_nJobNumber != GKN) {
			ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_PARK),			FALSE,bFlagEnable);
		} else {
			ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ORIGINALL),	FALSE,bFlagEnable);
		}
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOME_FAILED),	false,false);

		break;
	case DUAL_ROBOT_9_PLUS_9:

		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ORIGINALL),	true,bFlagEnable);

		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_LEFT),			true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ALL_LEFT),		true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_LEFT),		true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YLEFT),		true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ZLEFT),		true,bFlagEnable);


		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_RIGHT),		true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ALL_RIGHT),	true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_RIGHT),		true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YRIGHT),		true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ZRIGHT),		true,bFlagEnable);

		
		
		
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_HOLDING),			false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XMOVING),			false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Z_FIXED),	false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Y_FIXED),	false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_X_FIXED),	false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_X),		false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Y_MOVING),	false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Z_MOVING),	false,bFlagEnable);


		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_TURNTABLE),		false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_BRIDGE),			false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XTLEFT),		false,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nXtLeft].cName);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YTLEFT),		false,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nYtLeft].cName);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_R_LEFT),		false,bFlagEnable);


		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XTRIGHT),		false,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nXtRight].cName);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YTRIGHT),		false,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nYtRight].cName);

		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_PARK),			FALSE,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOME_FAILED),	true,bFlagEnable);

		break;
	case SPX_ROBOT:

		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ORIGINALL),	true,bFlagEnable);

		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_LEFT),			true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ALL_LEFT),		true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_LEFT),		true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YLEFT),		false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ZLEFT),		true,bFlagEnable);


		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_RIGHT),		true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ALL_RIGHT),	true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_RIGHT),		false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YRIGHT),		false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ZRIGHT),		true,bFlagEnable);

		
		
		
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_HOLDING),			false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XMOVING),			false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Z_FIXED),	false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Y_FIXED),	false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_X_FIXED),	false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_X),		false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Y_MOVING),	false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Z_MOVING),	false,bFlagEnable);


		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_TURNTABLE),		true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_BRIDGE),			false,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XTLEFT),		false,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nXtLeft].cName);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YTLEFT),		false,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nYtLeft].cName);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_R_LEFT),		false,bFlagEnable);


		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XTRIGHT),		false,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nXtRight].cName);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YTRIGHT),		false,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nYtRight].cName);

		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_PARK),			FALSE,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOME_FAILED),	true,bFlagEnable);

		break;
	case FB_11_AXIS:

		theApp.m_Tank.bHoldingFixture != TRUE ? bFlagShow = false : bFlagShow = true;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_HOLDING),			bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XMOVING),			bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Z_FIXED),	bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Y_FIXED),	bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_X_FIXED),	bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_X),		bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Y_MOVING),	bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOLDING_Z_MOVING),	bFlagShow,bFlagEnable);


		theApp.m_Tank.bTurnTable != TRUE ? bFlagShow = false : bFlagShow = true;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_TURNTABLE),		bFlagShow,bFlagEnable);

		theApp.m_Tank.bBridge != TRUE ? bFlagShow = false : bFlagShow = true;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_BRIDGE),			bFlagShow,bFlagEnable);

		theApp.m_Tank.nScannerDescription == SINGLE_BRIDGE_TANK ? bFlagShow = false : bFlagShow = true;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ORIGINALL),	true,bFlagEnable);

		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_LEFT),			true,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ALL_LEFT),		true,bFlagEnable);

		theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_LEFT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YLEFT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ZLEFT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XTLEFT),		bFlagShow,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nXtLeft].cName);
		theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YTLEFT),		bFlagShow,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nYtLeft].cName);
		theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_R_LEFT),		bFlagShow,bFlagEnable);


		theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_X_RIGHT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YRIGHT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ZRIGHT),		bFlagShow,bFlagEnable);
		theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_XTRIGHT),		bFlagShow,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nXtRight].cName);
		theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable ? bFlagShow = true : bFlagShow = false;
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_YTRIGHT),		bFlagShow,bFlagEnable,	theApp.m_Axes[theApp.m_Tank.nYtRight].cName);

		bFlagShow = false;
		if( theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable || theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable ||	theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable
			|| theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable || 	theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable ) {
			bFlagShow=true;
		}
		
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_RIGHT),		bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ALL_RIGHT),	bFlagShow,bFlagEnable);
		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ORIGINALL),	bFlagShow,bFlagEnable);

		if(theApp.m_nJobNumber != GKN) {
			ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_PARK),			FALSE,bFlagEnable);
		} else {
			ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_ORIGINALL),	FALSE,bFlagEnable);
		}

		ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_HOME_FAILED),	true,true);
		break;
	}
}

void COriginPage::ShowAndEnableWindow(CWnd* pWnd,bool bShowFlag,bool bEnableFlag,WCHAR *cName)
{
	CString Buff;

	pWnd->ShowWindow(bShowFlag);
	if(bShowFlag == TRUE) pWnd->EnableWindow(bEnableFlag);
	if(cName != NULL) {
		Buff.Format(L"%s",cName);
		pWnd->SetWindowText(Buff);
	}

}

void COriginPage::UpdateAllControls()
{
	SetAccessPrivelage();
}

void COriginPage::OnButtonRLeft() 
{
	if(theApp.m_LastSettings.nManipulatorType[PORTSIDE] == ROTATE_SWIVEL_GIMBAL) {
		theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset = 0;
	}

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_R_LEFT : theApp.m_Thread.m_nThreadAction = ORIGIN_R_LEFT;
}

void COriginPage::OnButtonHomeFailed() 
{
	theApp.m_FBCtrl.ResetKarlMorganOperationMode();
}

