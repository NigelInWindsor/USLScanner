// RThetaPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RThetaPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRThetaPage property page

IMPLEMENT_DYNCREATE(CRThetaPage, CPropertyPage)

CRThetaPage::CRThetaPage() : CPropertyPage(CRThetaPage::IDD)
{
	//{{AFX_DATA_INIT(CRThetaPage)
	m_nRLeftAbsEncoder = 0;
	m_nTLeftAbsEncoder = 0;
	m_nRLeftPmacCount = 0;
	m_nTLeftPmacCount = 0;
	m_strRLeftAngle = _T("");
	m_strTLeftAngle = _T("");
	m_RLeftStatus = _T("");
	m_TLeftStatus = _T("");
	m_strFloatRevNumber = _T("");
	//}}AFX_DATA_INIT
	
	

	

	switch(theApp.m_nSide0Orientation) {
	case 0: m_nLeft = PORTSIDE;
		m_nRight = STARBOARD;
		break;
	case 1: m_nLeft = STARBOARD;
		m_nRight = PORTSIDE;
		break;
	}

}

CRThetaPage::~CRThetaPage()
{
}

void CRThetaPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRThetaPage)
	DDX_Control(pDX, IDC_CHECK_W, m_checkW);
	DDX_Control(pDX, IDC_CHECK_90, m_check90);
	DDX_Control(pDX, IDC_CHECK_180, m_check180);
	DDX_Control(pDX, IDC_CHECK_LOOKBACK, m_checkLookBack);
	DDX_Control(pDX, IDC_COMBO_PEAK_TIMESLOT, m_comboPeakTimeslot);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	DDX_Control(pDX, IDC_EDIT_R_LEFT_OFFSET, m_editRLeftOffset);
	DDX_Control(pDX, IDC_EDIT_T_LEFT_OFFSET, m_editTLeftOffset);
	DDX_Text(pDX, IDC_EDIT_R_LEFT_ABS_ENC, m_nRLeftAbsEncoder);
	DDX_Text(pDX, IDC_EDIT_T_LEFT_ABS_ENC, m_nTLeftAbsEncoder);
	DDX_Text(pDX, IDC_EDIT_R_LEFT_PMAC_COUNT, m_nRLeftPmacCount);
	DDX_Text(pDX, IDC_EDIT_T_LEFT_PMAC_COUNT, m_nTLeftPmacCount);
	DDX_Text(pDX, IDC_EDIT_R_LEFT_ANGLE, m_strRLeftAngle);
	DDX_Text(pDX, IDC_EDIT_T_LEFT_ANGLE, m_strTLeftAngle);
	DDX_Text(pDX, IDC_EDIT_R_LEFT_STATUS, m_RLeftStatus);
	DDX_Text(pDX, IDC_EDIT_T_LEFT_STATUS, m_TLeftStatus);
	DDX_Text(pDX, IDC_EDIT_REV_NUMBER, m_strFloatRevNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRThetaPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRThetaPage)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_R_LEFT_OFFSET, OnChangeEditRLeftOffset)
	ON_EN_CHANGE(IDC_EDIT_T_LEFT_OFFSET, OnChangeEditTLeftOffset)
	ON_BN_CLICKED(IDC_BUTTON_R_LEFT_OFFSET, OnButtonRLeftOffset)
	ON_BN_CLICKED(IDC_BUTTON_T_LEFT_OFFSET, OnButtonTLeftOffset)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_CBN_SELCHANGE(IDC_COMBO_PEAK_TIMESLOT, OnSelchangeComboPeakTimeslot)
	ON_BN_CLICKED(IDC_BUTTON_ZERO_THETA_LEFT, OnButtonZeroThetaLeft)
	ON_BN_CLICKED(IDC_BUTTON_ZERO_THETA_RIGHT, OnButtonZeroThetaRight)
	ON_BN_CLICKED(IDC_BUTTON_ACCEPT_RIGHT, OnButtonAcceptRight)
	ON_BN_CLICKED(IDC_BUTTON_ACCEPT_LEFT, OnButtonAcceptLeft)
	ON_BN_CLICKED(IDC_CHECK_LOOKBACK, OnCheckLookback)
	ON_BN_CLICKED(IDC_CHECK_180, OnCheck180)
	ON_BN_CLICKED(IDC_CHECK_90, OnCheck90)
	ON_BN_CLICKED(IDC_CHECK_W, OnCheckW)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRThetaPage message handlers

BOOL CRThetaPage::OnInitDialog() 
{
	
	CPropertyPage::OnInitDialog();

	m_staticLeft.SetWindowText(theApp.m_DlgSideName[0]);
	m_staticRight.SetWindowText(theApp.m_DlgSideName[1]);
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRThetaPage::OnSetActive() 
{
	
	
	UpdateAllControls();

	SetTimer(1,200,NULL);

	theApp.m_LastSettings.nLastCompensationToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	return CPropertyPage::OnSetActive();
}

BOOL CRThetaPage::OnKillActive() 
{
	
	KillTimer(1);
	return CPropertyPage::OnKillActive();
}

void CRThetaPage::UpdateAllControls()
{
	
	CString Buff;

	Buff.Format(_T("%d"),theApp.m_Axes[3].nOffset);
	m_editRLeftOffset.SetWindowText(Buff);

	Buff.Format(_T("%d"),theApp.m_Axes[4].nOffset);
	m_editTLeftOffset.SetWindowText(Buff);

	m_comboPeakTimeslot.ResetContent();
	for(int nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		m_comboPeakTimeslot.AddString(theApp.m_UtUser.m_TS[nTS].cName);
	}
	
	m_comboPeakTimeslot.SetCurSel(theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]);

	m_check90.SetCheck(theApp.m_LastSettings.nRTheta90Check);
	m_check180.SetCheck(theApp.m_LastSettings.nRTheta180Check);
	m_checkLookBack.SetCheck(theApp.m_LastSettings.nRThetaLookBack);
	m_checkW.SetCheck(theApp.m_LastSettings.nRThetaUseW);

}

void CRThetaPage::OnTimer(UINT nIDEvent) 
{
	
	float	fRAngle,fTAngle;

	m_nRLeftAbsEncoder = theApp.m_Si10User.ReadAbsEncoder(0);
	m_nTLeftAbsEncoder = theApp.m_Si10User.ReadAbsEncoder(1);

	m_nRLeftPmacCount = theApp.m_Axes[3].nPos;
	m_nTLeftPmacCount = theApp.m_Axes[4].nPos;

	CString Degrees = L"\x0b0";
	CString Micro = L"\x0b5";

	fRAngle = (float)theApp.m_Axes[3].nPos * theApp.m_Axes[3].fStepSize;
	m_strRLeftAngle.Format(_T("%.02f%s"),fRAngle,Degrees);

	fTAngle = (float)theApp.m_Axes[4].nPos * theApp.m_Axes[4].fStepSize;
	m_strTLeftAngle.Format(_T("%.02f%s"),fTAngle,Degrees);

	theApp.m_Si10User.ReadAbsStatus(0,m_RLeftStatus);
	theApp.m_Si10User.ReadAbsStatus(1,m_TLeftStatus);

	m_strFloatRevNumber.Format(_T("%.02f"),theApp.m_fRotateThetaRevNumber[PORTSIDE]);

	UpdateData(FALSE);
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CRThetaPage::OnChangeEditRLeftOffset() 
{
	
	CString Buff;

	m_editRLeftOffset.GetWindowText(Buff);
	theApp.m_Axes[3].nOffset = _ttoi(Buff);
}

void CRThetaPage::OnChangeEditTLeftOffset() 
{
	
	CString Buff;

	m_editTLeftOffset.GetWindowText(Buff);
	theApp.m_Axes[4].nOffset = _ttoi(Buff);
}

void CRThetaPage::OnButtonRLeftOffset() 
{
	

	theApp.m_Axes[3].nOffset = m_nRLeftAbsEncoder;
	UpdateAllControls();
	
}

void CRThetaPage::OnButtonTLeftOffset() 
{
	

	theApp.m_Axes[4].nOffset = m_nTLeftAbsEncoder;
	UpdateAllControls();
}

void CRThetaPage::OnButtonTest() 
{
	
	CCoord CpSurface;
	D3DXMATRIXA16 mat;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_Kinematics.D3DXRotateMatrixFromKNormal(mat, CpSurface.Side0.norm);
}



void CRThetaPage::OnSelchangeComboPeakTimeslot() 
{
	

	theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide] = m_comboPeakTimeslot.GetCurSel();
}

void CRThetaPage::OnButtonZeroThetaLeft() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->StopCurrentTimerAction();
	pFrame->m_nRThetaZeroSide = m_nLeft;
	pFrame->m_nTimerAction = ZERO_THETA;
	
}

void CRThetaPage::OnButtonZeroThetaRight() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->StopCurrentTimerAction();
	pFrame->m_nRThetaZeroSide = m_nRight;
	pFrame->m_nTimerAction = ZERO_THETA;
}

void CRThetaPage::OnButtonAcceptRight() 
{
	

	switch(theApp.m_nSide0Orientation) {
	case 0:
		theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = 0;
		Sleep(100);
		theApp.m_Axes[theApp.m_Tank.nYtRight].nLimitSwitchOffset+=((theApp.m_Axes[theApp.m_Tank.nYtRight].nPos+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset)*16);
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_YT_RIGHT;
		break;
	case 1:
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = 0;
		Sleep(100);
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nLimitSwitchOffset+=((theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset)*16);
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_YT_LEFT;
		break;
	};
	
}

void CRThetaPage::OnButtonAcceptLeft() 
{
	

	switch(theApp.m_nSide0Orientation) {
	case 0:
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = 0;
		Sleep(100);
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nLimitSwitchOffset+=((theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset)*16);
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_YT_LEFT;
		break;
	case 1:
		theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = 0;
		Sleep(100);
		theApp.m_Axes[theApp.m_Tank.nYtRight].nLimitSwitchOffset+=((theApp.m_Axes[theApp.m_Tank.nYtRight].nPos+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset)*16);
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = ORIGIN_YT_RIGHT;
		break;
	};
	
}


void CRThetaPage::OnCheckLookback() 
{
	
	
	theApp.m_LastSettings.nRThetaLookBack = m_checkLookBack.GetCheck();
}

void CRThetaPage::OnCheck180() 
{
	
	
	theApp.m_LastSettings.nRTheta180Check = m_check180.GetCheck();
}

void CRThetaPage::OnCheck90() 
{
	
	
	theApp.m_LastSettings.nRTheta90Check = m_check90.GetCheck();
}

void CRThetaPage::OnCheckW() 
{
	
	
	theApp.m_LastSettings.nRThetaUseW = m_checkW.GetCheck();
}
