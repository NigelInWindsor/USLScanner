// HoldingPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "HoldingPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoldingPage property page

IMPLEMENT_DYNCREATE(CHoldingPage, CPropertyPage)

CHoldingPage::CHoldingPage() : CPropertyPage(CHoldingPage::IDD)
{
	//{{AFX_DATA_INIT(CHoldingPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CHoldingPage::~CHoldingPage()
{
}

void CHoldingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHoldingPage)
	DDX_Control(pDX, IDC_EDIT_HOLDING_BALL, m_editHoldingBall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHoldingPage, CPropertyPage)
	//{{AFX_MSG_MAP(CHoldingPage)
	ON_BN_CLICKED(IDC_BUTTON_STORE_BALL_ZERO, OnButtonStoreBallZero)
	ON_BN_CLICKED(IDC_BUTTON_BALL_MOVETO, OnButtonBallMoveto)
	ON_BN_CLICKED(IDC_BUTTONMOVETO_ZERO, OnButtonmovetoZero)
	ON_BN_CLICKED(IDC_BUTTON_PARK, OnButtonPark)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoldingPage message handlers

BOOL CHoldingPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHoldingPage::UpdateAllControls()
{
	CString Buff;

	if (GetSafeHwnd() == NULL) return;

	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),theApp.m_ChBall.fXMoving,theApp.m_ChBall.fYMoving,theApp.m_ChBall.fZMoving);
	m_editHoldingBall.SetWindowText(Buff);
}

BOOL CHoldingPage::OnSetActive() 
{
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}

void CHoldingPage::OnButtonStoreBallZero() 
{
	if(MessageBox(_T("Are you Damned sure you want to change this?"),_T("Warning"),MB_YESNO) == IDYES) {
		theApp.m_Motors.GetHoldingCoord(&theApp.m_ChBall);
		UpdateAllControls();
	}
}

void CHoldingPage::OnButtonBallMoveto() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpHolding = theApp.m_ChBall;
	theApp.m_Thread.m_nThreadAction = MOVETO_HOLDING_FIXTURE;
	
}

void CHoldingPage::OnButtonmovetoZero() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	ZeroMemory(&theApp.m_Thread.m_CpHolding,sizeof theApp.m_Thread.m_CpHolding);
	theApp.m_Thread.m_nThreadAction = MOVETO_HOLDING_FIXTURE;
}

void CHoldingPage::OnButtonPark() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	ZeroMemory(&theApp.m_Thread.m_CpHolding,sizeof theApp.m_Thread.m_CpHolding);
	theApp.m_Thread.m_CpHolding.fXMoving = theApp.m_Axes[theApp.m_Tank.nXStep].fParkPosition;
	theApp.m_Thread.m_CpHolding.fYMoving = theApp.m_Axes[theApp.m_Tank.nYStep].fParkPosition;
	theApp.m_Thread.m_CpHolding.fZMoving = theApp.m_Axes[theApp.m_Tank.nZStep].fParkPosition;
	theApp.m_Thread.m_CpHolding.fYFixed = theApp.m_Axes[theApp.m_Tank.nXtStep].fParkPosition;
	theApp.m_Thread.m_CpHolding.fZFixed = theApp.m_Axes[theApp.m_Tank.nYtStep].fParkPosition;
	theApp.m_Thread.m_nThreadAction = MOVETO_HOLDING_FIXTURE;
	
}
