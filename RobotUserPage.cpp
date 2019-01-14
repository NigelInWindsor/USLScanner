// RobotUserPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotUserPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotUserPage property page

IMPLEMENT_DYNCREATE(CRobotUserPage, CPropertyPage)

CRobotUserPage::CRobotUserPage() : CPropertyPage(CRobotUserPage::IDD)
{
	//{{AFX_DATA_INIT(CRobotUserPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_fJogZ[0] = 0.0f;
	m_fJogZ[1] = 0.0f;

}

CRobotUserPage::~CRobotUserPage()
{
}

void CRobotUserPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotUserPage)
	DDX_Control(pDX, IDC_SPIN_Z_1, m_editspinZ1);
	DDX_Control(pDX, IDC_SPIN_Z_0, m_editspinZ0);
	DDX_Control(pDX, IDC_SPIN_Y_1, m_editspinY1);
	DDX_Control(pDX, IDC_SPIN_Y_0, m_editspinY0);
	DDX_Control(pDX, IDC_SPIN_X_1, m_editspinX1);
	DDX_Control(pDX, IDC_SPIN_X_0, m_editspinX0);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_SLIDER_Z_AXIS2, m_sliderZAxis1);
	DDX_Control(pDX, IDC_SLIDER_Y_AXIS2, m_sliderYAxis1);
	DDX_Control(pDX, IDC_SLIDER_X_AXIS2, m_sliderXAxis1);
	DDX_Control(pDX, IDC_SLIDER_Z_AXIS, m_sliderZAxis0);
	DDX_Control(pDX, IDC_SLIDER_Y_AXIS, m_sliderYAxis0);
	DDX_Control(pDX, IDC_SLIDER_X_AXIS, m_sliderXAxis0);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SPIN_BALL_DIAMETER, m_editSpinBallDiameter);
	DDX_Control(pDX, IDC_BUTTON_0_EQUAL_1, m_btn0Equal1);
	DDX_Control(pDX, IDC_BUTTON_1_EQUAL_0, m_btn1Equal0);
	DDX_Control(pDX, IDC_SLIDER_ROT_Z_0, m_sliderJogZ0);
	DDX_Control(pDX, IDC_SLIDER_ROT_Z_1, m_sliderJogZ1);
}


BEGIN_MESSAGE_MAP(CRobotUserPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRobotUserPage)
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_CARTESIAN_0, OnButtonCartesian0)
	ON_BN_CLICKED(IDC_BUTTON_MIXED_0, OnButtonMixed0)
	ON_BN_CLICKED(IDC_BUTTON_ROBOT_0, OnButtonRobot0)
	ON_BN_CLICKED(IDC_BUTTON_CARTESIAN_1, OnButtonCartesian1)
	ON_BN_CLICKED(IDC_BUTTON_MIXED_1, OnButtonMixed1)
	ON_BN_CLICKED(IDC_BUTTON_ROBOT_1, OnButtonRobot1)
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_X_0, &CRobotUserPage::OnDeltaposSpinX0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Y_0, &CRobotUserPage::OnDeltaposSpinY0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Z_0, &CRobotUserPage::OnDeltaposSpinZ0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_X_1, &CRobotUserPage::OnDeltaposSpinX1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Y_1, &CRobotUserPage::OnDeltaposSpinY1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Z_1, &CRobotUserPage::OnDeltaposSpinZ1)
	ON_EN_CHANGE(IDC_EDIT_X_0, &CRobotUserPage::OnEnChangeEditX0)
	ON_EN_CHANGE(IDC_EDIT_Y_0, &CRobotUserPage::OnEnChangeEditY0)
	ON_EN_CHANGE(IDC_EDIT_Z_0, &CRobotUserPage::OnEnChangeEditZ0)
	ON_EN_CHANGE(IDC_EDIT_X_1, &CRobotUserPage::OnEnChangeEditX1)
	ON_EN_CHANGE(IDC_EDIT_Y_1, &CRobotUserPage::OnEnChangeEditY1)
	ON_EN_CHANGE(IDC_EDIT_Z_1, &CRobotUserPage::OnEnChangeEditZ1)
	ON_BN_CLICKED(IDC_BUTTON_ZERO_SIDE_0, &CRobotUserPage::OnBnClickedButtonZeroSide0)
	ON_BN_CLICKED(IDC_BUTTON_ZERO_SIDE_1, &CRobotUserPage::OnBnClickedButtonZeroSide1)
	ON_EN_CHANGE(IDC_EDIT_BALL_DIAMETER, &CRobotUserPage::OnEnChangeEditBallDiameter)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BALL_DIAMETER, &CRobotUserPage::OnDeltaposSpinBallDiameter)
	ON_BN_CLICKED(IDC_BUTTON_0_EQUAL_1, &CRobotUserPage::OnBnClickedButton0Equal1)
	ON_BN_CLICKED(IDC_BUTTON_1_EQUAL_0, &CRobotUserPage::OnBnClickedButton1Equal0)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotUserPage message handlers


BOOL CRobotUserPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_staticLeft.SetWindowText(theApp.m_DlgSideName[0]);
	m_staticRight.SetWindowText(theApp.m_DlgSideName[1]);

	Buff.Format(L"%s = %s", theApp.m_DlgSideName[0], theApp.m_DlgSideName[1]);		m_btn0Equal1.SetWindowTextW(Buff);
	Buff.Format(L"%s = %s", theApp.m_DlgSideName[1], theApp.m_DlgSideName[0]);		m_btn1Equal0.SetWindowTextW(Buff);

	m_editspinX0.Init(theApp.m_Kinematics.m_fRobotWeightingX[0], 0.0f, 100.0f, 1.0f, 1.0f, L"%.0f");
	m_editspinY0.Init(theApp.m_Kinematics.m_fRobotWeightingY[0], 0.0f, 100.0f, 1.0f, 1.0f, L"%.0f");
	m_editspinZ0.Init(theApp.m_Kinematics.m_fRobotWeightingZ[0], 0.0f, 100.0f, 1.0f, 1.0f, L"%.0f");
	m_editspinX1.Init(theApp.m_Kinematics.m_fRobotWeightingX[1], 0.0f, 100.0f, 1.0f, 1.0f, L"%.0f");
	m_editspinY1.Init(theApp.m_Kinematics.m_fRobotWeightingY[1], 0.0f, 100.0f, 1.0f, 1.0f, L"%.0f");
	m_editspinZ1.Init(theApp.m_Kinematics.m_fRobotWeightingZ[1], 0.0f, 100.0f, 1.0f, 1.0f, L"%.0f");

	m_sliderXAxis0.SetRange(0,100,FALSE);
	m_sliderYAxis0.SetRange(0,100,FALSE);
	m_sliderZAxis0.SetRange(0,100,FALSE);
	m_sliderXAxis1.SetRange(0,100,FALSE);
	m_sliderYAxis1.SetRange(0,100,FALSE);
	m_sliderZAxis1.SetRange(0,100,FALSE);

	m_sliderJogZ0.SetRange(0, 360, FALSE);
	m_sliderJogZ1.SetRange(0, 360, FALSE);

	m_editSpinBallDiameter.Init(theApp.m_LastSettings.fZeroBallDiameter, 0.0f, 50.0f, 0.1f, 1.0f, L"%.1f mm");

	
	UpdateAllControls();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRobotUserPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}


void CRobotUserPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	MinMax(&theApp.m_Kinematics.m_fRobotWeightingX[0],0.0f,100.0f);
	MinMax(&theApp.m_Kinematics.m_fRobotWeightingY[0],0.0f,100.0f);
	MinMax(&theApp.m_Kinematics.m_fRobotWeightingZ[0],0.0f,100.0f);
	m_sliderXAxis0.SetPos((int)theApp.m_Kinematics.m_fRobotWeightingX[0]);
	m_sliderYAxis0.SetPos((int)theApp.m_Kinematics.m_fRobotWeightingY[0]);
	m_sliderZAxis0.SetPos((int)theApp.m_Kinematics.m_fRobotWeightingZ[0]);

	MinMax(&theApp.m_Kinematics.m_fRobotWeightingX[1],0.0f,100.0f);
	MinMax(&theApp.m_Kinematics.m_fRobotWeightingY[1],0.0f,100.0f);
	MinMax(&theApp.m_Kinematics.m_fRobotWeightingZ[1],0.0f,100.0f);
	m_sliderXAxis1.SetPos((int)theApp.m_Kinematics.m_fRobotWeightingX[1]);
	m_sliderYAxis1.SetPos((int)theApp.m_Kinematics.m_fRobotWeightingY[1]);
	m_sliderZAxis1.SetPos((int)theApp.m_Kinematics.m_fRobotWeightingZ[1]);

	m_sliderJogZ0.SetPos((int)(m_fJogZ[0] + 180.0f));
	m_sliderJogZ1.SetPos((int)(m_fJogZ[1] + 180.0f));

	theApp.m_FBCtrl.SendRobotWeightings(0, false);
}


BOOL CRobotUserPage::OnSetActive() 
{
	UpdateAllControls();

	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	return CPropertyPage::OnSetActive();
}


void CRobotUserPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int nTemp = pSlider->GetPos()&0xffff;
	CString Buff;


	switch(pSlider->GetDlgCtrlID()) {
	case IDC_SLIDER_X_AXIS:
		theApp.m_Kinematics.m_fRobotWeightingX[0] = (float)nTemp;
		theApp.m_LastSettings.vRobotWeighting[0].x = theApp.m_Kinematics.m_fRobotWeightingX[0];
		m_editspinX0.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[0]);
		break;
	case IDC_SLIDER_Y_AXIS:
		theApp.m_Kinematics.m_fRobotWeightingY[0] = (float)nTemp;
		theApp.m_LastSettings.vRobotWeighting[0].y = theApp.m_Kinematics.m_fRobotWeightingY[0];
		m_editspinY0.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[0]);
		break;
	case IDC_SLIDER_Z_AXIS:
		theApp.m_Kinematics.m_fRobotWeightingZ[0] = (float)nTemp;
		theApp.m_LastSettings.vRobotWeighting[0].z = theApp.m_Kinematics.m_fRobotWeightingZ[0];
		m_editspinZ0.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[0]);
		break;
	case IDC_SLIDER_X_AXIS2:
		theApp.m_Kinematics.m_fRobotWeightingX[1] = (float)nTemp;
		theApp.m_LastSettings.vRobotWeighting[1].x = theApp.m_Kinematics.m_fRobotWeightingX[1];
		m_editspinX1.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[1]);
		break;
	case IDC_SLIDER_Y_AXIS2:
		theApp.m_Kinematics.m_fRobotWeightingY[1] = (float)nTemp;
		theApp.m_LastSettings.vRobotWeighting[1].y = theApp.m_Kinematics.m_fRobotWeightingY[1];
		m_editspinY1.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[1]);
		break;
	case IDC_SLIDER_Z_AXIS2:
		theApp.m_Kinematics.m_fRobotWeightingZ[1] = (float)nTemp;
		theApp.m_LastSettings.vRobotWeighting[1].z = theApp.m_Kinematics.m_fRobotWeightingZ[1];
		m_editspinZ1.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[1]);
		break;
	case IDC_SLIDER_ROT_Z_0:
		switch (nSBCode) {
		case 5:
			m_fJogZ[0] = (float)(nTemp - 180) * 0.02f;
			break;
		default:
			m_fJogZ[0] = 0.0f;
			m_sliderJogZ0.SetPos(180);
			break;
		}
		Buff.Format(L"RZJogMove=%.02f", m_fJogZ[0]);
		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
		break;
	case IDC_SLIDER_ROT_Z_1:
		switch (nSBCode) {
		case 5:
			m_fJogZ[1] = (float)(nTemp - 180) * 0.02f;
			break;
		default:
			m_fJogZ[1] = 0.0f;
			m_sliderJogZ1.SetPos(180);
			break;
		}
		Buff.Format(L"B_RZJogMove=%.02f", m_fJogZ[1]);
		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
		break;

	};

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

//Side 0 //

void CRobotUserPage::OnButtonCartesian0() 
{
	theApp.m_Kinematics.m_fRobotWeightingX[0] = 0.0f;
	theApp.m_Kinematics.m_fRobotWeightingY[0] = 0.0f;
	theApp.m_Kinematics.m_fRobotWeightingZ[0] = 0.0f;
	UpdateAllControls();	
	m_editspinX0.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[0]);
	m_editspinY0.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[0]);
	m_editspinZ0.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[0]);
	m_editspinX1.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[1]);
	m_editspinY1.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[1]);
	m_editspinZ1.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[1]);
}

void CRobotUserPage::OnButtonMixed0() 
{
	theApp.m_Kinematics.m_fRobotWeightingX[0] = theApp.m_LastSettings.vRobotWeighting[0].x;
	theApp.m_Kinematics.m_fRobotWeightingY[0] = theApp.m_LastSettings.vRobotWeighting[0].y;
	theApp.m_Kinematics.m_fRobotWeightingZ[0] = theApp.m_LastSettings.vRobotWeighting[0].z;
	UpdateAllControls();
	m_editspinX0.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[0]);
	m_editspinY0.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[0]);
	m_editspinZ0.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[0]);
	m_editspinX1.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[1]);
	m_editspinY1.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[1]);
	m_editspinZ1.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[1]);
}

void CRobotUserPage::OnButtonRobot0() 
{
	theApp.m_Kinematics.m_fRobotWeightingX[0] = 100.0f;
	theApp.m_Kinematics.m_fRobotWeightingY[0] = 100.0f;
	theApp.m_Kinematics.m_fRobotWeightingZ[0] = 100.0f;
	UpdateAllControls();	
	m_editspinX0.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[0]);
	m_editspinY0.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[0]);
	m_editspinZ0.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[0]);
	m_editspinX1.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[1]);
	m_editspinY1.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[1]);
	m_editspinZ1.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[1]);
}
//// Side 1 //////
void CRobotUserPage::OnButtonCartesian1() 
{
	theApp.m_Kinematics.m_fRobotWeightingX[1] = 0.0f;
	theApp.m_Kinematics.m_fRobotWeightingY[1] = 0.0f;
	theApp.m_Kinematics.m_fRobotWeightingZ[1] = 0.0f;
	UpdateAllControls();	
	m_editspinX0.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[0]);
	m_editspinY0.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[0]);
	m_editspinZ0.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[0]);
	m_editspinX1.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[1]);
	m_editspinY1.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[1]);
	m_editspinZ1.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[1]);
}

void CRobotUserPage::OnButtonMixed1() 
{
	theApp.m_Kinematics.m_fRobotWeightingX[1] = theApp.m_LastSettings.vRobotWeighting[1].x;
	theApp.m_Kinematics.m_fRobotWeightingY[1] = theApp.m_LastSettings.vRobotWeighting[1].y;
	theApp.m_Kinematics.m_fRobotWeightingZ[1] = theApp.m_LastSettings.vRobotWeighting[1].z;
	UpdateAllControls();
	m_editspinX0.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[0]);
	m_editspinY0.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[0]);
	m_editspinZ0.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[0]);
	m_editspinX1.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[1]);
	m_editspinY1.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[1]);
	m_editspinZ1.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[1]);
}

void CRobotUserPage::OnButtonRobot1() 
{
	theApp.m_Kinematics.m_fRobotWeightingX[1] = 100.0f;
	theApp.m_Kinematics.m_fRobotWeightingY[1] = 100.0f;
	theApp.m_Kinematics.m_fRobotWeightingZ[1] = 100.0f;
	UpdateAllControls();	
	m_editspinX0.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[0]);
	m_editspinY0.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[0]);
	m_editspinZ0.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[0]);
	m_editspinX1.SetPos(theApp.m_Kinematics.m_fRobotWeightingX[1]);
	m_editspinY1.SetPos(theApp.m_Kinematics.m_fRobotWeightingY[1]);
	m_editspinZ1.SetPos(theApp.m_Kinematics.m_fRobotWeightingZ[1]);
}


void CRobotUserPage::OnDeltaposSpinX0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	theApp.m_LastSettings.vRobotWeighting[0].x = theApp.m_Kinematics.m_fRobotWeightingX[0] = m_editspinX0.GetValue();
	UpdateAllControls();
	*pResult = 0;
}


void CRobotUserPage::OnDeltaposSpinY0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	theApp.m_LastSettings.vRobotWeighting[0].y = theApp.m_Kinematics.m_fRobotWeightingY[0] = m_editspinY0.GetValue();
	UpdateAllControls();
	*pResult = 0;
}


void CRobotUserPage::OnDeltaposSpinZ0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	theApp.m_LastSettings.vRobotWeighting[0].z = theApp.m_Kinematics.m_fRobotWeightingZ[0] = m_editspinZ0.GetValue();
	UpdateAllControls();
	*pResult = 0;
}


void CRobotUserPage::OnDeltaposSpinX1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	theApp.m_LastSettings.vRobotWeighting[1].x = theApp.m_Kinematics.m_fRobotWeightingX[1] = m_editspinX1.GetValue();
	UpdateAllControls();
	*pResult = 0;
}


void CRobotUserPage::OnDeltaposSpinY1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	theApp.m_LastSettings.vRobotWeighting[1].y = theApp.m_Kinematics.m_fRobotWeightingY[1] = m_editspinY1.GetValue();
	UpdateAllControls();
	*pResult = 0;
}


void CRobotUserPage::OnDeltaposSpinZ1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_LastSettings.vRobotWeighting[1].z = theApp.m_Kinematics.m_fRobotWeightingZ[1] = m_editspinZ1.GetValue();
	UpdateAllControls();

	*pResult = 0;
}


void CRobotUserPage::OnEnChangeEditX0()
{
	if (theApp.m_Kinematics.m_fRobotWeightingX[0] - m_editspinX0.GetValue()) {
		theApp.m_LastSettings.vRobotWeighting[0].x = theApp.m_Kinematics.m_fRobotWeightingX[0] = m_editspinX0.GetValue();
	}
	UpdateAllControls();
}


void CRobotUserPage::OnEnChangeEditY0()
{
	if (theApp.m_Kinematics.m_fRobotWeightingY[0] - m_editspinY0.GetValue()) {
		theApp.m_LastSettings.vRobotWeighting[0].y = theApp.m_Kinematics.m_fRobotWeightingY[0] = m_editspinY0.GetValue();
	}
	UpdateAllControls();
}


void CRobotUserPage::OnEnChangeEditZ0()
{
	if (theApp.m_Kinematics.m_fRobotWeightingZ[0] - m_editspinZ0.GetValue()) {
		theApp.m_LastSettings.vRobotWeighting[0].z = theApp.m_Kinematics.m_fRobotWeightingZ[0] = m_editspinZ0.GetValue();
	}
	UpdateAllControls();
}


void CRobotUserPage::OnEnChangeEditX1()
{
	if (theApp.m_Kinematics.m_fRobotWeightingX[1] - m_editspinX1.GetValue()) {
		theApp.m_LastSettings.vRobotWeighting[1].x = theApp.m_Kinematics.m_fRobotWeightingX[1] = m_editspinX1.GetValue();
	}
	UpdateAllControls();
}


void CRobotUserPage::OnEnChangeEditY1()
{
	if (theApp.m_Kinematics.m_fRobotWeightingY[1] - m_editspinY1.GetValue()) {
		theApp.m_LastSettings.vRobotWeighting[1].y = theApp.m_Kinematics.m_fRobotWeightingY[1] = m_editspinY1.GetValue();
	}
	UpdateAllControls();
}


void CRobotUserPage::OnEnChangeEditZ1()
{
	if (theApp.m_Kinematics.m_fRobotWeightingZ[1] - m_editspinZ1.GetValue()) {
		theApp.m_LastSettings.vRobotWeighting[1].z = theApp.m_Kinematics.m_fRobotWeightingZ[1] = m_editspinZ1.GetValue();
	}
	UpdateAllControls();
}


void CRobotUserPage::OnBnClickedButtonZeroSide0()
{
	theApp.m_nSide0Orientation == 0 ? theApp.m_Kinematics.ZeroFrame(0) : theApp.m_Kinematics.ZeroFrame(1);
}


void CRobotUserPage::OnBnClickedButtonZeroSide1()
{
	theApp.m_nSide0Orientation == 0 ? theApp.m_Kinematics.ZeroFrame(1) : theApp.m_Kinematics.ZeroFrame(0);
}


void CRobotUserPage::OnEnChangeEditBallDiameter()
{
	theApp.m_LastSettings.fZeroBallDiameter = m_editSpinBallDiameter.GetValue();
}


void CRobotUserPage::OnDeltaposSpinBallDiameter(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	theApp.m_LastSettings.fZeroBallDiameter = m_editSpinBallDiameter.GetValue();
	*pResult = 0;
}


void CRobotUserPage::OnBnClickedButton0Equal1()
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_Tank.vFrameZero[0] += (CpSurface.Side1.pt - CpSurface.Side0.pt);
}


void CRobotUserPage::OnBnClickedButton1Equal0()
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_Tank.vFrameZero[1] -= (CpSurface.Side1.pt - CpSurface.Side0.pt);
}


