// RotateThetaLiftPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RotateThetaLiftPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotateThetaLiftPage property page

IMPLEMENT_DYNCREATE(CRotateThetaLiftPage, CPropertyPage)

CRotateThetaLiftPage::CRotateThetaLiftPage() : CPropertyPage(CRotateThetaLiftPage::IDD)
{
	//{{AFX_DATA_INIT(CRotateThetaLiftPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CRotateThetaLiftPage::~CRotateThetaLiftPage()
{
}

void CRotateThetaLiftPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotateThetaLiftPage)
	DDX_Control(pDX, IDC_CHECK_R_AXIS, m_checkRAxis);
	DDX_Control(pDX, IDC_EDIT_DELTA_THETA_ANGLE, m_editDeltaThetaAngle);
	DDX_Control(pDX, IDC_BUTTON_TANK_ZERO, m_buttonTankZero);
	DDX_Control(pDX, IDC_BUTTON_LIFT_DOWN, m_buttonLiftDown);
	DDX_Control(pDX, IDC_BUTTON_LIFT_UP, m_buttonLiftUp);
	DDX_Control(pDX, IDC_BUTTON_SIDE_0, m_buttonSide0);
	DDX_Control(pDX, IDC_BUTTON_SIDE_1, m_buttonSide1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRotateThetaLiftPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRotateThetaLiftPage)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_ALL, OnButtonOriginAll)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_X, OnButtonOriginX)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_Y, OnButtonOriginY)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_Z, OnButtonOriginZ)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_ROTATE, OnButtonOriginRotate)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_THETA, OnButtonOriginTheta)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SIDE_0, OnButtonSide0)
	ON_BN_CLICKED(IDC_BUTTON_SIDE_1, OnButtonSide1)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_UP, OnButtonLiftUp)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_DOWN, OnButtonLiftDown)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_STOP, OnButtonLiftStop)
	ON_BN_CLICKED(IDC_BUTTON_TANK_ZERO, OnButtonTankZero)
	ON_BN_CLICKED(IDC_BUTTON_R_ZERO, OnButtonRZero)
	ON_BN_CLICKED(IDC_BUTTON_R_180, OnButtonR180)
	ON_BN_CLICKED(IDC_BUTTON_THETA_PLUS, OnButtonThetaPlus)
	ON_BN_CLICKED(IDC_BUTTON_THETA_MINUS, OnButtonThetaMinus)
	ON_EN_CHANGE(IDC_EDIT_DELTA_THETA_ANGLE, OnChangeEditDeltaThetaAngle)
	ON_BN_CLICKED(IDC_CHECK_R_AXIS, OnCheckRAxis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotateThetaLiftPage message handlers

BOOL CRotateThetaLiftPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRotateThetaLiftPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;

	CString Degrees = L"\x0b0";
	CString Micro = L"\x0b5";
	Buff.Format(_T("%.02f%s"),theApp.m_LastSettings.fDeltaThetaAngle,Degrees);

	m_editDeltaThetaAngle.SetWindowText(Buff);
	m_checkRAxis.SetCheck(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable);

}

void CRotateThetaLiftPage::SetAccessPrivelages()
{
	
	if (GetSafeHwnd() == NULL) return;

	if(theApp.m_PmacUser.WhereIsLift() == 1) {		//Up
		theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable = true;
		m_checkRAxis.SetCheck(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable);
		m_buttonSide0.EnableWindow(TRUE);
		if(theApp.m_Axes[theApp.m_Tank.nRLeft].nStatus & PMAC_HOMED) {
			m_buttonSide1.EnableWindow(TRUE);
		} else {
			m_buttonSide1.EnableWindow(FALSE);
		}
	} else {
		theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable = false;
		m_checkRAxis.SetCheck(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable);
		m_buttonSide0.EnableWindow(FALSE);
		m_buttonSide1.EnableWindow(FALSE);
	}
	bool bFlag = false;
	if(theApp.m_Axes[theApp.m_Tank.nXLeft].nStatus & PMAC_HOMED) {
		if(theApp.m_Axes[theApp.m_Tank.nYLeft].nStatus & PMAC_HOMED) {
			if(theApp.m_Axes[theApp.m_Tank.nZLeft].nStatus & PMAC_HOMED) {
//				if(theApp.m_Axes[theApp.m_Tank.nXtLeft].nStatus & PMAC_HOMED) {
//					if(theApp.m_Axes[theApp.m_Tank.nYtLeft].nStatus & PMAC_HOMED) {
						bFlag = true;
//					}
//				}
			}
		}
	}
	m_buttonTankZero.EnableWindow( bFlag );

}

void CRotateThetaLiftPage::OnButtonOriginAll() 
{
	
	CString Buff;

	Buff.LoadString(IDS_Will_the_Z_axis_avoid_the_lift_mechanism);
	if(MessageBox(Buff,_T("Warning"),MB_YESNO) !=IDYES) return;

	
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_X_Y_Z_R_THETA_LEFT;

}

void CRotateThetaLiftPage::OnButtonOriginX() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_X_LEFT;
}

void CRotateThetaLiftPage::OnButtonOriginY() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_Y_LEFT;
}

void CRotateThetaLiftPage::OnButtonOriginZ() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_Z_LEFT;
}

void CRotateThetaLiftPage::OnButtonOriginRotate() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_XT_LEFT;
}

void CRotateThetaLiftPage::OnButtonOriginTheta() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_YT_LEFT;
}


BOOL CRotateThetaLiftPage::OnSetActive() 
{
	
	SetTimer(1,500,NULL);

	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CRotateThetaLiftPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CRotateThetaLiftPage::OnTimer(UINT nIDEvent) 
{

	SetAccessPrivelages();

	CPropertyPage::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void CRotateThetaLiftPage::OnButtonSide0() 
{
	
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	theApp.m_Thread.StopCurrentThreadAction();
/*
	if(theApp.m_Axes[theApp.m_Tank.nRLeft].nStatus & PMAC_HOMED) {
		Cp.Side0.fR = 0.0f;
		theApp.m_Thread.m_CpMoveTo = Cp;
		theApp.m_Thread.m_nThreadAction=MOVE_JUST_R;
	} else {
		theApp.m_Thread.m_nThreadAction = ORIGIN_R_LEFT;
	}
*/

	theApp.m_Thread.m_nThreadAction = ORIGIN_R_LEFT;
	
}

void CRotateThetaLiftPage::OnButtonSide1() 
{
	
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	Cp.Side0.fR = 180.0f;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVE_JUST_R;
}


//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void CRotateThetaLiftPage::OnButtonLiftUp() 
{
	

	switch(theApp.m_nJobNumber) {
	case FOREPLANE:	theApp.m_UtUser.LiftCommandUp();
		break;
	default: theApp.m_PmacUser.LiftCommandUp();
		break;
	}
}

void CRotateThetaLiftPage::OnButtonLiftDown() 
{
	

	switch(theApp.m_nJobNumber) {
	case FOREPLANE:	theApp.m_UtUser.LiftCommandDown();
		break;
	default: theApp.m_PmacUser.LiftCommandDown();
		break;
	}
}

void CRotateThetaLiftPage::OnButtonLiftStop() 
{
	

	switch(theApp.m_nJobNumber) {
	case FOREPLANE:	theApp.m_UtUser.LiftCommandStop();
		break;
	default: theApp.m_PmacUser.LiftCommandStop();
		break;
	}
}

void CRotateThetaLiftPage::OnButtonTankZero() 
{
	
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);
	Cp.Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
	Cp.Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
	Cp.Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
	Cp.Side0.fXt=0.0f;
	Cp.Side0.fYt=0.0f;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	
}

void CRotateThetaLiftPage::OnButtonRZero() 
{
	
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);
	if((fabsf(Cp.Side0.fX-m_CpHead.Side0.fX) > 0.2f) ||
		(fabsf(Cp.Side0.fY-m_CpHead.Side0.fY) > 0.2f) ||
		(fabsf(Cp.Side0.fZ-m_CpHead.Side0.fZ) > 0.2f)) {
		m_CpHead = Cp;
	}
	m_CpHead.Side0.fXt=0.0f;
	m_CpHead.Side0.fYt=Cp.Side0.fYt;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = m_CpHead;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
}

void CRotateThetaLiftPage::OnButtonR180() 
{
	
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);
	if((fabsf(Cp.Side0.fX-m_CpHead.Side0.fX) > 0.2f) ||
		(fabsf(Cp.Side0.fY-m_CpHead.Side0.fY) > 0.2f) ||
		(fabsf(Cp.Side0.fZ-m_CpHead.Side0.fZ) > 0.2f)) {
		m_CpHead = Cp;
	}
	m_CpHead.Side0.fXt=180.0f;
	m_CpHead.Side0.fYt=Cp.Side0.fYt;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = m_CpHead;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
}

void CRotateThetaLiftPage::OnButtonThetaPlus() 
{
	
	CCoord Cp,CpSurface;
/*
	theApp.m_Motors.GetHeadPos(&Cp);
	if((fabsf(Cp.Side0.fX-m_CpHead.Side0.fX) > 1.0f) ||
		(fabsf(Cp.Side0.fY-m_CpHead.Side0.fY) > 1.0f) ||
		(fabsf(Cp.Side0.fZ-m_CpHead.Side0.fZ) > 1.0f)) {
		m_CpHead = Cp;
	}
	theApp.m_Kinematics.SurfaceFromHead(&m_CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
	theApp.m_Kinematics.SurfaceFromHead(&m_CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);

	CpSurface.Side0.fI
	m_CpHead.Side0.fXt=180.0f;
	m_CpHead.Side0.fYt=Cp.Side0.fYt;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = m_CpHead;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	*/
}

void CRotateThetaLiftPage::OnButtonThetaMinus() 
{
	// TODO: Add your control notification handler code here
	
}

void CRotateThetaLiftPage::OnChangeEditDeltaThetaAngle() 
{
	
	CString Buff;

	m_editDeltaThetaAngle.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fDeltaThetaAngle);
}


void CRotateThetaLiftPage::OnCheckRAxis() 
{
	

	theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==TRUE ? theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable = false : theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable = true;
	m_checkRAxis.SetCheck(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable);
}
