// SquirtersPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "SquirtersPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LEFT	0
#define	RIGHT	1
#define	BOTH	2


/////////////////////////////////////////////////////////////////////////////
// CSquirtersPage property page

IMPLEMENT_DYNCREATE(CSquirtersPage, CPropertyPage)

CSquirtersPage::CSquirtersPage() : CPropertyPage(CSquirtersPage::IDD)
{
	//{{AFX_DATA_INIT(CSquirtersPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nSide = BOTH;
}

CSquirtersPage::~CSquirtersPage()
{
}

void CSquirtersPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSquirtersPage)
	DDX_Control(pDX, IDC_EDIT_LEFT_ANGLE, m_editLeftAngle);
	DDX_Control(pDX, IDC_EDIT_RIGHT_ANGLE, m_editRightAngle);
	DDX_Control(pDX, IDC_EDIT_DEWLL_TIME, m_editDwellTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSquirtersPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSquirtersPage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_DEWLL_TIME, OnChangeEditDewllTime)
	ON_EN_CHANGE(IDC_EDIT_LEFT_ANGLE, OnChangeEditLeftAngle)
	ON_EN_CHANGE(IDC_EDIT_RIGHT_ANGLE, OnChangeEditRightAngle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSquirtersPage message handlers
BOOL CSquirtersPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSquirtersPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CSquirtersPage::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(_T("%d"),theApp.m_LastSettings.nRemoveAirTime);
	m_editDwellTime.SetWindowText(Buff);

	Buff.Format(L"%.01f%s",theApp.m_LastSettings.fRemoveAirAngle[PORTSIDE],DEGREES);
	m_editLeftAngle.SetWindowText(Buff);
	Buff.Format(L"%.01f%s",theApp.m_LastSettings.fRemoveAirAngle[STARBOARD],DEGREES);
	m_editRightAngle.SetWindowText(Buff);


	SetAccessPrivelage();
}


void CSquirtersPage::OnButtonPointDown() 
{
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	switch(theApp.m_nJobNumber) {
	default:
		switch(m_nSide) {
		case LEFT:
			Cp.Side0.fYt=-90.0f;
			break;
		case RIGHT:
			Cp.Side1.fYt=90.0f;
			break;
		case BOTH:
			Cp.Side0.fYt=-90.0f;
			Cp.Side1.fYt=90.0f;
			break;
		}
		break;
	case GOODRICH_MACHINE_4:
	case TWI:
		switch(m_nSide) {
		case LEFT:
			Cp.Side0.fYt=90.0f;
			break;
		case RIGHT:
			Cp.Side1.fYt=-90.0f;
			break;
		case BOTH:
			Cp.Side0.fYt=90.0f;
			Cp.Side1.fYt=-90.0f;
			break;
		}
		break;
	}

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	
}

void CSquirtersPage::OnButtonZeroZero() 
{
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);
	switch(m_nSide) {
	case LEFT:
		Cp.Side0.fXt=0.0f;
		Cp.Side0.fYt=0.0f;
		break;
	case RIGHT:
		Cp.Side1.fXt=0.0f;
		Cp.Side1.fYt=0.0f;
		break;
	case BOTH:
		Cp.Side0.fXt=0.0f;
		Cp.Side0.fYt=0.0f;
		Cp.Side1.fXt=0.0f;
		Cp.Side1.fYt=0.0f;
		break;
	}
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	
}

BOOL CSquirtersPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}

void CSquirtersPage::OnButtonRemoveAir() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=TIPS_REMOVE_AIR;
	
}

void CSquirtersPage::OnChangeEditDewllTime() 
{
	CString Buff;

	m_editDwellTime.GetWindowText(Buff);

	theApp.m_LastSettings.nRemoveAirTime = _ttoi(Buff);

}



void CSquirtersPage::OnButton45() 
{
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	switch(theApp.m_nJobNumber) {
	default:
		switch(m_nSide) {
		case LEFT:
			Cp.Side0.fYt=-45.0f;
			break;
		case RIGHT:
			Cp.Side1.fYt=45.0f;
			break;
		case BOTH:
			Cp.Side0.fYt=-45.0f;
			Cp.Side1.fYt=45.0f;
			break;
		}
		break;
	case GOODRICH_MACHINE_4:
	case TWI:
		switch(m_nSide) {
		case LEFT:
			Cp.Side0.fYt=45.0f;
			break;
		case RIGHT:
			Cp.Side1.fYt=-45.0f;
			break;
		case BOTH:
			Cp.Side0.fYt=45.0f;
			Cp.Side1.fYt=-45.0f;
			break;
		}
		break;
	}

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	
}

void CSquirtersPage::SetAccessPrivelage()
{
	CButton* pButton;
	bool bFlagEnable;

	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_NORMALIZE_PAGE] ? bFlagEnable = TRUE : bFlagEnable = FALSE;

	m_editDwellTime.EnableWindow(bFlagEnable);

	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_POINT_DOWN)) pButton->EnableWindow(bFlagEnable);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_ZERO_ZERO)) pButton->EnableWindow(bFlagEnable);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_REMOVE_AIR)) pButton->EnableWindow(bFlagEnable);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_45)) pButton->EnableWindow(bFlagEnable);

}

void CSquirtersPage::OnChangeEditLeftAngle() 
{
	CString Buff;

	m_editLeftAngle.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fRemoveAirAngle[PORTSIDE]);
}

void CSquirtersPage::OnChangeEditRightAngle() 
{
	CString Buff;

	m_editRightAngle.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fRemoveAirAngle[STARBOARD]);
}
