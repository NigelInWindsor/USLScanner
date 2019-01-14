// Robot9Plus9Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "Robot9Plus9Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobot9Plus9Dlg property page

IMPLEMENT_DYNCREATE(CRobot9Plus9Dlg, CPropertyPage)

CRobot9Plus9Dlg::CRobot9Plus9Dlg() : CPropertyPage(CRobot9Plus9Dlg::IDD)
{
	//{{AFX_DATA_INIT(CRobot9Plus9Dlg)
	//}}AFX_DATA_INIT
	

}

CRobot9Plus9Dlg::~CRobot9Plus9Dlg()
{
}

void CRobot9Plus9Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobot9Plus9Dlg)
	DDX_Control(pDX, IDC_CHECK_INCLUDE_Z_AXIS, m_checkIncludeZAxis);
	DDX_Control(pDX, IDC_CHECK_INCLUDE_Y_AXIS, m_checkIncludeYAxis);
	DDX_Control(pDX, IDC_CHECK_INCLUDE_X_AXIS, m_checkIncludeXAxis);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_13, m_editStepSize13);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_12, m_editStepSize12);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_11, m_editStepSize11);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_3, m_editStepSize3);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_2, m_editStepSize2);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_1, m_editStepSize1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRobot9Plus9Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CRobot9Plus9Dlg)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_1, OnChangeEditStepsize1)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_2, OnChangeEditStepsize2)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_3, OnChangeEditStepsize3)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_11, OnChangeEditStepsize11)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_12, OnChangeEditStepsize12)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_13, OnChangeEditStepsize13)
	ON_BN_CLICKED(IDC_BUTTON_HOME_1, OnButtonHome1)
	ON_BN_CLICKED(IDC_BUTTON_HOME_2, OnButtonHome2)
	ON_BN_CLICKED(IDC_BUTTON_HOME_3, OnButtonHome3)
	ON_BN_CLICKED(IDC_BUTTON_HOME_11, OnButtonHome11)
	ON_BN_CLICKED(IDC_BUTTON_HOME_12, OnButtonHome12)
	ON_BN_CLICKED(IDC_BUTTON_HOME_13, OnButtonHome13)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_ALL_LEFT, OnButtonAllLeft)
	ON_BN_CLICKED(IDC_BUTTON_ALL_RIGHT, OnButtonAllRight)
	ON_BN_CLICKED(IDC_CHECK_INCLUDE_X_AXIS, OnCheckIncludeXAxis)
	ON_BN_CLICKED(IDC_CHECK_INCLUDE_Y_AXIS, OnCheckIncludeYAxis)
	ON_BN_CLICKED(IDC_CHECK_INCLUDE_Z_AXIS, OnCheckIncludeZAxis)
	ON_BN_CLICKED(IDC_BUTTON_ZERO_LEFT, OnButtonZeroLeft)
	ON_BN_CLICKED(IDC_BUTTON_ZERO_RIGHT, OnButtonZeroRight)
	ON_BN_CLICKED(IDC_BUTTON_HOME_FAILED, OnButtonHomeFailed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobot9Plus9Dlg message handlers

BOOL CRobot9Plus9Dlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRobot9Plus9Dlg::UpdateAllControls()
{
	
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(L"%11.05e%s",theApp.m_Axes[0].fStepSize,DEGREES);	m_editStepSize1.SetWindowText(Buff);
	Buff.Format(L"%11.05e%s",theApp.m_Axes[1].fStepSize,DEGREES);	m_editStepSize2.SetWindowText(Buff);
	Buff.Format(L"%11.05e%s",theApp.m_Axes[2].fStepSize,DEGREES);	m_editStepSize3.SetWindowText(Buff);
	Buff.Format(L"%11.05e%s",theApp.m_Axes[10].fStepSize,DEGREES);	m_editStepSize11.SetWindowText(Buff);
	Buff.Format(L"%11.05e%s",theApp.m_Axes[11].fStepSize,DEGREES);	m_editStepSize12.SetWindowText(Buff);
	Buff.Format(L"%11.05e%s",theApp.m_Axes[12].fStepSize,DEGREES);	m_editStepSize13.SetWindowText(Buff);

	theApp.m_Kinematics.m_nAxesIncludedMoves & 1 ? m_checkIncludeXAxis.SetCheck(true) : m_checkIncludeXAxis.SetCheck(false);
	theApp.m_Kinematics.m_nAxesIncludedMoves & 2 ? m_checkIncludeYAxis.SetCheck(true) : m_checkIncludeYAxis.SetCheck(false);
	theApp.m_Kinematics.m_nAxesIncludedMoves & 4 ? m_checkIncludeZAxis.SetCheck(true) : m_checkIncludeZAxis.SetCheck(false);
}

BOOL CRobot9Plus9Dlg::OnSetActive() 
{
	
	theApp.m_LastSettings.nLastRobotTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();


	return CPropertyPage::OnSetActive();
}

BOOL CRobot9Plus9Dlg::OnKillActive() 
{
	
	return CPropertyPage::OnKillActive();
}

void CRobot9Plus9Dlg::OnChangeEditStepsize1() 
{
	
	CString Buff;
	float fTemp = theApp.m_Axes[0].fStepSize;

	m_editStepSize1.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Axes[0].fStepSize);
	if((fTemp - theApp.m_Axes[0].fStepSize) && (theApp.m_Axes[0].fStepSize != 0.0f)) {
		Buff.Format(L"Mtr1SF=%f",1.0f / theApp.m_Axes[0].fStepSize);
		theApp.m_FBCtrl.SendStr(Buff);
	}
}

void CRobot9Plus9Dlg::OnChangeEditStepsize2() 
{
	
	CString Buff;
	float fTemp = theApp.m_Axes[1].fStepSize;

	m_editStepSize2.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Axes[1].fStepSize);
	if((fTemp - theApp.m_Axes[1].fStepSize) && (theApp.m_Axes[1].fStepSize != 0.0f)) {
		Buff.Format(L"Mtr2SF=%f",1.0f / theApp.m_Axes[1].fStepSize);
		theApp.m_FBCtrl.SendStr(Buff);
	}
}

void CRobot9Plus9Dlg::OnChangeEditStepsize3() 
{
	
	CString Buff;
	float fTemp = theApp.m_Axes[2].fStepSize;

	m_editStepSize3.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Axes[2].fStepSize);
	if((fTemp - theApp.m_Axes[2].fStepSize) && (theApp.m_Axes[2].fStepSize != 0.0f)) {
		Buff.Format(L"Mtr3SF=%f",1.0f / theApp.m_Axes[2].fStepSize);
		theApp.m_FBCtrl.SendStr(Buff);
	}
}

void CRobot9Plus9Dlg::OnChangeEditStepsize11() 
{
	
	CString Buff;
	float fTemp = theApp.m_Axes[10].fStepSize;

	m_editStepSize11.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Axes[10].fStepSize);
	if((fTemp - theApp.m_Axes[10].fStepSize) && (theApp.m_Axes[10].fStepSize != 0.0f)) {
		Buff.Format(L"Mtr11SF=%f",1.0f / theApp.m_Axes[10].fStepSize);
		theApp.m_FBCtrl.SendStr(Buff);
	}
}

void CRobot9Plus9Dlg::OnChangeEditStepsize12() 
{
	
	CString Buff;
	float fTemp = theApp.m_Axes[11].fStepSize;

	m_editStepSize12.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Axes[11].fStepSize);
	if((fTemp - theApp.m_Axes[11].fStepSize) && (theApp.m_Axes[11].fStepSize != 0.0f)) {
		Buff.Format(L"Mtr12SF=%f",1.0f / theApp.m_Axes[11].fStepSize);
		theApp.m_FBCtrl.SendStr(Buff);
	}
}

void CRobot9Plus9Dlg::OnChangeEditStepsize13() 
{
	
	CString Buff;
	float fTemp = theApp.m_Axes[12].fStepSize;

	m_editStepSize13.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Axes[12].fStepSize);
	if((fTemp - theApp.m_Axes[12].fStepSize) && (theApp.m_Axes[12].fStepSize != 0.0f)) {
		Buff.Format(L"Mtr13SF=%f",1.0f / theApp.m_Axes[12].fStepSize);
		theApp.m_FBCtrl.SendStr(Buff);
	}
}




void CRobot9Plus9Dlg::OnButtonHome1() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nRoboAxisMask = 0x0001;
	theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;

}

void CRobot9Plus9Dlg::OnButtonHome2() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nRoboAxisMask = 0x0002;
	theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
}

void CRobot9Plus9Dlg::OnButtonHome3() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nRoboAxisMask = 0x0004;
	theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
}

void CRobot9Plus9Dlg::OnButtonHome11() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nRoboAxisMask = 0x0400;
	theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
}

void CRobot9Plus9Dlg::OnButtonHome12() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nRoboAxisMask = 0x0800;
	theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
}

void CRobot9Plus9Dlg::OnButtonHome13() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nRoboAxisMask = 0x1000;
	theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
}

void CRobot9Plus9Dlg::OnButtonAll() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nRoboAxisMask = 0x1c07;
	theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
	
}

void CRobot9Plus9Dlg::OnButtonAllLeft() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nRoboAxisMask = 0x0007;
	theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
}

void CRobot9Plus9Dlg::OnButtonAllRight() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nRoboAxisMask = 0x1c00;
	theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_ROBO_9_PLUS_9;
}

void CRobot9Plus9Dlg::OnCheckIncludeXAxis() 
{
	

	m_checkIncludeXAxis.GetCheck() == 1 ? theApp.m_Kinematics.m_nAxesIncludedMoves |= 1 : theApp.m_Kinematics.m_nAxesIncludedMoves &= 0xfffe;
	if(theApp.m_Kinematics.m_nAxesIncludedMoves & 1) {
		theApp.m_FBCtrl.SendStr(L"IncludeXAxis=1");
	} else {
		theApp.m_FBCtrl.SendStr(L"IncludeXAxis=0");
	}
	
}

void CRobot9Plus9Dlg::OnCheckIncludeYAxis() 
{
	

	m_checkIncludeYAxis.GetCheck() == 1 ? theApp.m_Kinematics.m_nAxesIncludedMoves |= 2 : theApp.m_Kinematics.m_nAxesIncludedMoves &= 0xfffd;
	if(theApp.m_Kinematics.m_nAxesIncludedMoves & 2) {
		theApp.m_FBCtrl.SendStr(L"IncludeYAxis=1");
	} else {
		theApp.m_FBCtrl.SendStr(L"IncludeYAxis=0");
	}
}

void CRobot9Plus9Dlg::OnCheckIncludeZAxis() 
{
	

	m_checkIncludeZAxis.GetCheck() == 1 ? theApp.m_Kinematics.m_nAxesIncludedMoves |= 4 : theApp.m_Kinematics.m_nAxesIncludedMoves &= 0xfffb;
	if(theApp.m_Kinematics.m_nAxesIncludedMoves & 4) {
		theApp.m_FBCtrl.SendStr(L"IncludeZAxis=1");
	} else {
		theApp.m_FBCtrl.SendStr(L"IncludeZAxis=0");
	}
}


void CRobot9Plus9Dlg::OnButtonZeroLeft() 
{
	
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_Robot[0].m_vBase += CpSurface.Side0.pt;
}

void CRobot9Plus9Dlg::OnButtonZeroRight() 
{
	
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_Robot[1].m_vBase += CpSurface.Side1.pt;
}

void CRobot9Plus9Dlg::OnButtonHomeFailed() 
{
	
	
	theApp.m_FBCtrl.ResetKarlMorganOperationMode();
}
