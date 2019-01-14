// LiftPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "LiftPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLiftPage property page

IMPLEMENT_DYNCREATE(CLiftPage, CPropertyPage)

CLiftPage::CLiftPage() : CPropertyPage(CLiftPage::IDD)
{
	//{{AFX_DATA_INIT(CLiftPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_bTimerActive = false;
}

CLiftPage::~CLiftPage()
{
}

void CLiftPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLiftPage)
	DDX_Control(pDX, IDC_EDIT_LIFT_STATUS, m_editLiftStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLiftPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLiftPage)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_GOTO_ZERO, OnButtonGotoZero)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_180, OnButtonGoto180)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN, OnButtonOrigin)
	ON_BN_CLICKED(IDC_BUTTON_LIFT_STOP, OnButtonLiftStop)
	ON_BN_CLICKED(IDC_BUTTON_MOVETO_ZERO, OnButtonMovetoZero)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLiftPage message handlers
BOOL CLiftPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	UpdateAllControls();

	SetTimer(1,500,NULL);
	m_bTimerActive = true;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLiftPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
}


void CLiftPage::OnButtonUp() 
{
	switch(theApp.m_nJobNumber) {
	case FOREPLANE:	theApp.m_UtUser.LiftCommandUp();
		break;
	default: theApp.m_PmacUser.LiftCommandUp();
		break;
	}
}

void CLiftPage::OnButtonDown() 
{
	switch(theApp.m_nJobNumber) {
	case FOREPLANE:	theApp.m_UtUser.LiftCommandDown();
		break;
	default: theApp.m_PmacUser.LiftCommandDown();
		break;
	}
}

void CLiftPage::OnButtonLiftStop() 
{
	switch(theApp.m_nJobNumber) {
	case FOREPLANE:	theApp.m_UtUser.LiftCommandStop();
		break;
	default: theApp.m_PmacUser.LiftCommandStop();
		break;
	}
}

BOOL CLiftPage::OnSetActive() 
{
	UpdateAllControls();
	if(m_bTimerActive != true) {
		SetTimer(1,500,NULL);
		m_bTimerActive = true;
	}

	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CLiftPage::OnKillActive() 
{
	if(m_bTimerActive == true) {
		KillTimer(1);
	}
	m_bTimerActive = false;

	
	return CPropertyPage::OnKillActive();
}

void CLiftPage::OnTimer(UINT nIDEvent) 
{
	int nStatus;

	switch(theApp.m_nJobNumber) {
	case FOREPLANE:	nStatus = theApp.m_UtUser.WhereIsLift();
		break;
	default: nStatus = theApp.m_PmacUser.WhereIsLift();
		 break;
	}

	if(m_nOldStatus - nStatus) {
		switch(m_nOldStatus = nStatus) {
		case 1: m_editLiftStatus.SetWindowText(_T("Lift is up"));
			break;
		case 2: m_editLiftStatus.SetWindowText(_T("Lift is down"));
			break;
		default: m_editLiftStatus.SetWindowText(_T("Ask All the King's men"));
			break;
		}
	}
	CPropertyPage::OnTimer(nIDEvent);
}


void CLiftPage::OnButtonGotoZero() 
{
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	Cp.Side0.fR = 0.0f;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVE_JUST_R;
}

void CLiftPage::OnButtonGoto180() 
{
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	Cp.Side0.fR = 180.0f;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVE_JUST_R;
}

void CLiftPage::OnButtonOrigin() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = ORIGIN_R_LEFT;
}


void CLiftPage::OnButtonMovetoZero() 
{
	if(theApp.m_nJobNumber==FOREPLANE) {
	/*	for(int ii=0;ii<12;ii++){
			if(theApp.m_Axes[ii].bAvailable==TRUE) {
				if(!(theApp.m_Axes[ii].nStatus&0x400l)){
					MessageBox(_T("All axes must be homed to perform this function"),_T("Error"),NULL);
					return;
				}
			}
		}
	*/
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction=MOVETO_PARK;
	}

	
}
