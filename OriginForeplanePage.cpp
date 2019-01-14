// OriginForeplanePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "OriginForeplanePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COriginForeplanePage property page

IMPLEMENT_DYNCREATE(COriginForeplanePage, CPropertyPage)

COriginForeplanePage::COriginForeplanePage() : CPropertyPage(COriginForeplanePage::IDD)
{
	//{{AFX_DATA_INIT(COriginForeplanePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

COriginForeplanePage::~COriginForeplanePage()
{
}

void COriginForeplanePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COriginForeplanePage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COriginForeplanePage, CPropertyPage)
	//{{AFX_MSG_MAP(COriginForeplanePage)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_X, OnButtonOriginX)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_Y, OnButtonOriginY)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_Z, OnButtonOriginZ)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_XTIP, OnButtonOriginXtip)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN_YTIP, OnButtonOriginYtip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COriginForeplanePage message handlers

BOOL COriginForeplanePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;
		
	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COriginForeplanePage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
}

void COriginForeplanePage::OnButtonAll() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_FOREPLANE;
}


void COriginForeplanePage::OnButtonOriginX() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_X_LEFT;
}

void COriginForeplanePage::OnButtonOriginY() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_Y_LEFT;
}

void COriginForeplanePage::OnButtonOriginZ() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_Z_LEFT;
}

void COriginForeplanePage::OnButtonOriginXtip() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset = 0;
		Sleep(100);
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos - (int)(0.0f / theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
	} else {
		theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset = 0;
		Sleep(100);
		theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset = theApp.m_Axes[theApp.m_Tank.nXtRight].nPos - (int)(0.0f / theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
	}

	theApp.SaveTank();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
	
}

void COriginForeplanePage::OnButtonOriginYtip() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = 0;
		Sleep(100);
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos - (int)(0.0f / theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
	} else {
		theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = 0;
		Sleep(100);
		theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos - (int)(0.0f / theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
	}
	theApp.SaveTank();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
	
}

void COriginForeplanePage::SetRegistry()
{

}
