// RobotJointsPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotJointsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotJointsPage property page

IMPLEMENT_DYNCREATE(CRobotJointsPage, CPropertyPage)

CRobotJointsPage::CRobotJointsPage() : CPropertyPage(CRobotJointsPage::IDD)
{
	//{{AFX_DATA_INIT(CRobotJointsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nRobot = 0;
	m_nStartAxis = 0;
}

CRobotJointsPage::~CRobotJointsPage()
{
}

void CRobotJointsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotJointsPage)
	DDX_Control(pDX, IDC_EDIT_MIN_RANGE_1, m_editMinRange1);
	DDX_Control(pDX, IDC_EDIT_MIN_RANGE_2, m_editMinRange2);
	DDX_Control(pDX, IDC_EDIT_MIN_RANGE_3, m_editMinRange3);
	DDX_Control(pDX, IDC_EDIT_MIN_RANGE_4, m_editMinRange4);
	DDX_Control(pDX, IDC_EDIT_MIN_RANGE_5, m_editMinRange5);
	DDX_Control(pDX, IDC_EDIT_MIN_RANGE_6, m_editMinRange6);
	DDX_Control(pDX, IDC_EDIT_MAX_SPEED_1, m_editMaxVelocity1);
	DDX_Control(pDX, IDC_EDIT_MAX_SPEED_2, m_editMaxVelocity2);
	DDX_Control(pDX, IDC_EDIT_MAX_SPEED_3, m_editMaxVelocity3);
	DDX_Control(pDX, IDC_EDIT_MAX_SPEED_4, m_editMaxVelocity4);
	DDX_Control(pDX, IDC_EDIT_MAX_SPEED_5, m_editMaxVelocity5);
	DDX_Control(pDX, IDC_EDIT_MAX_SPEED_6, m_editMaxVelocity6);
	DDX_Control(pDX, IDC_EDIT_MAX_RANGE_1, m_editMaxRange1);
	DDX_Control(pDX, IDC_EDIT_MAX_RANGE_2, m_editMaxRange2);
	DDX_Control(pDX, IDC_EDIT_MAX_RANGE_3, m_editMaxRange3);
	DDX_Control(pDX, IDC_EDIT_MAX_RANGE_4, m_editMaxRange4);
	DDX_Control(pDX, IDC_EDIT_MAX_RANGE_5, m_editMaxRange5);
	DDX_Control(pDX, IDC_EDIT_MAX_RANGE_6, m_editMaxRange6);
	DDX_Control(pDX, IDC_EDIT_MAX_ACCEL_1, m_editAccel1);
	DDX_Control(pDX, IDC_EDIT_MAX_ACCEL_2, m_editAccel2);
	DDX_Control(pDX, IDC_EDIT_MAX_ACCEL_3, m_editAccel3);
	DDX_Control(pDX, IDC_EDIT_MAX_ACCEL_4, m_editAccel4);
	DDX_Control(pDX, IDC_EDIT_MAX_ACCEL_5, m_editAccel5);
	DDX_Control(pDX, IDC_EDIT_MAX_ACCEL_6, m_editAccel6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRobotJointsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRobotJointsPage)
	ON_BN_CLICKED(IDC_ACCEPT_BUTTON, OnAcceptButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotJointsPage message handlers

BOOL CRobotJointsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRobotJointsPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	
	float fStepSize;
	CEdit *peditMinRange,*peditMaxRange,*peditMaxVelocity,*peditAccel;

	for(int nAxis = 0;nAxis<6;nAxis++) {
		switch(nAxis) {
		default:
			peditMinRange = &m_editMinRange1;
			peditMaxRange = &m_editMaxRange1;
			peditMaxVelocity = &m_editMaxVelocity1;
			peditAccel = &m_editAccel1;
			break;
		case 1:
			peditMinRange = &m_editMinRange2;
			peditMaxRange = &m_editMaxRange2;
			peditMaxVelocity = &m_editMaxVelocity2;
			peditAccel = &m_editAccel2;
			break;
		case 2:
			peditMinRange = &m_editMinRange3;
			peditMaxRange = &m_editMaxRange3;
			peditMaxVelocity = &m_editMaxVelocity3;
			peditAccel = &m_editAccel3;
			break;
		case 3:
			peditMinRange = &m_editMinRange4;
			peditMaxRange = &m_editMaxRange4;
			peditMaxVelocity = &m_editMaxVelocity4;
			peditAccel = &m_editAccel4;
			break;
		case 4:
			peditMinRange = &m_editMinRange5;
			peditMaxRange = &m_editMaxRange5;
			peditMaxVelocity = &m_editMaxVelocity5;
			peditAccel = &m_editAccel5;
			break;
		case 5:
			peditMinRange = &m_editMinRange6;
			peditMaxRange = &m_editMaxRange6;
			peditMaxVelocity = &m_editMaxVelocity6;
			peditAccel = &m_editAccel6;
			break;
		}


		fStepSize = theApp.m_Axes[nAxis + m_nStartAxis].fStepSize;

		Buff.Format(L"%.01f%s",(float)theApp.m_Axes[nAxis + m_nStartAxis].nMinTravel * fStepSize,theApp.m_Degrees);
		peditMinRange->SetWindowText(Buff);

		Buff.Format(L"%.01f%s",(float)theApp.m_Axes[nAxis + m_nStartAxis].nMaxTravel * fStepSize,theApp.m_Degrees);
		peditMaxRange->SetWindowText(Buff);

		Buff.Format(L"%.01f%s/s",theApp.m_Axes[nAxis + m_nStartAxis].fMaxMoveSpeed,theApp.m_Degrees);
		peditMaxVelocity->SetWindowText(Buff);

		Buff.Format(_T("%.05f"),theApp.m_Axes[nAxis + m_nStartAxis].fAcceleration);
		peditAccel->SetWindowText(Buff);

	}

}

void CRobotJointsPage::OnAcceptButton() 
{
	CString Buff;
	
	float fStepSize,fTemp;
	CEdit *peditMinRange,*peditMaxRange,*peditMaxVelocity,*peditAccel;

	for(int nAxis = 0;nAxis<6;nAxis++) {
		switch(nAxis) {
		default:
			peditMinRange = &m_editMinRange1;
			peditMaxRange = &m_editMaxRange1;
			peditMaxVelocity = &m_editMaxVelocity1;
			peditAccel = &m_editAccel1;
			break;
		case 1:
			peditMinRange = &m_editMinRange2;
			peditMaxRange = &m_editMaxRange2;
			peditMaxVelocity = &m_editMaxVelocity2;
			peditAccel = &m_editAccel2;
			break;
		case 2:
			peditMinRange = &m_editMinRange3;
			peditMaxRange = &m_editMaxRange3;
			peditMaxVelocity = &m_editMaxVelocity3;
			peditAccel = &m_editAccel3;
			break;
		case 3:
			peditMinRange = &m_editMinRange4;
			peditMaxRange = &m_editMaxRange4;
			peditMaxVelocity = &m_editMaxVelocity4;
			peditAccel = &m_editAccel4;
			break;
		case 4:
			peditMinRange = &m_editMinRange5;
			peditMaxRange = &m_editMaxRange5;
			peditMaxVelocity = &m_editMaxVelocity5;
			peditAccel = &m_editAccel5;
			break;
		case 5:
			peditMinRange = &m_editMinRange6;
			peditMaxRange = &m_editMaxRange6;
			peditMaxVelocity = &m_editMaxVelocity6;
			peditAccel = &m_editAccel6;
			break;
		}

		fStepSize = theApp.m_Axes[nAxis + m_nStartAxis].fStepSize;

		peditMinRange->GetWindowText(Buff);
		_WTOF(Buff,fTemp);
		theApp.m_Axes[nAxis + m_nStartAxis].nMinTravel=(int)(fTemp / fStepSize);

		peditMaxRange->GetWindowText(Buff);
		_WTOF(Buff,fTemp);
		theApp.m_Axes[nAxis + m_nStartAxis].nMaxTravel=(int)(fTemp / fStepSize);

		peditMaxVelocity->GetWindowText(Buff);
		_WTOF(Buff,fTemp);
		theApp.m_Axes[nAxis + m_nStartAxis].fMaxMoveSpeed=fTemp;

		peditAccel->GetWindowText(Buff);
		_WTOF(Buff,fTemp);
		theApp.m_Axes[nAxis + m_nStartAxis].fAcceleration=fTemp;
	}


	theApp.m_FBCtrl.DownloadRobotJoints(m_nRobot);
	theApp.m_FBCtrl.DownloadRobotJoints(1);

}
