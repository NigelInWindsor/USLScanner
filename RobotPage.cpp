// RobotPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CRobotPage property page

IMPLEMENT_DYNCREATE(CRobotPage, CResizablePage)

CRobotPage::CRobotPage() : CResizablePage(CRobotPage::IDD)
{
	//{{AFX_DATA_INIT(CRobotPage)
	//}}AFX_DATA_INIT
	m_nRobot = 0;
	m_nCartesianCoordDisplay = 0;
	ZeroMemory(m_fJointDegrees,sizeof m_fJointDegrees);
}

CRobotPage::~CRobotPage()
{
}

void CRobotPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotPage)
	DDX_Control(pDX, IDC_SPIN_BASE_ROT_Z, m_spinBaseRotZ);
	DDX_Control(pDX, IDC_SPIN_BASE_ROT_Y, m_spinBaseRotY);
	DDX_Control(pDX, IDC_SPIN_BASE_ROT_X, m_spinBaseRotX);
	DDX_Control(pDX, IDC_EDIT_BASE_ROT_Z, m_editBaseRotZ);
	DDX_Control(pDX, IDC_EDIT_BASE_ROT_Y, m_editBaseRotY);
	DDX_Control(pDX, IDC_EDIT_BASE_ROT_X, m_editBaseRotX);
	DDX_Control(pDX, IDC_EDIT_ROLL, m_editRoll);
	DDX_Control(pDX, IDC_EDIT_PITCH, m_editPitch);
	DDX_Control(pDX, IDC_EDIT_YAW, m_editYaw);
	DDX_Control(pDX, IDC_EDIT_NORM_K, m_editNormK);
	DDX_Control(pDX, IDC_EDIT_NORM_J, m_editNormJ);
	DDX_Control(pDX, IDC_EDIT_NORM_I, m_editNormI);
	DDX_Control(pDX, IDC_CHECK_ROBOT_1, m_checkRobot1);
	DDX_Control(pDX, IDC_CHECK_ROBOT_0, m_checkRobot0);
	DDX_Control(pDX, IDC_EDIT_SERIAL_NUMBER, m_editSerialNumber);
	DDX_Control(pDX, IDC_EDIT_J6_TT, m_editJ6toTT);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_J6, m_editStepSizeJ6);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_J5, m_editStepSizeJ5);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_J4, m_editStepSizeJ4);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_J3, m_editStepSizeJ3);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_J2, m_editStepSizeJ2);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE_J1, m_editStepSizeJ1);
	DDX_Control(pDX, IDC_COMBO_VIRTUAL_ACTUAL, m_comboVirtualActual);
	DDX_Control(pDX, IDC_EDIT_J5_J6, m_editJ5J6);
	DDX_Control(pDX, IDC_EDIT_J4_J5, m_editJ4J5);
	DDX_Control(pDX, IDC_EDIT_J3_J4, m_editJ3J4);
	DDX_Control(pDX, IDC_EDIT_J2_J3, m_editJ2J3);
	DDX_Control(pDX, IDC_EDIT_J1_J2, m_editJ1J2);
	DDX_Control(pDX, IDC_EDIT_BASE_J1, m_editBaseJ1);
	DDX_Control(pDX, IDC_EDIT_BASE_COORD, m_editBaseCoord);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_SPIN_J6, m_spinJ6);
	DDX_Control(pDX, IDC_SPIN_J5, m_spinJ5);
	DDX_Control(pDX, IDC_SPIN_J4, m_spinJ4);
	DDX_Control(pDX, IDC_SPIN_J3, m_spinJ3);
	DDX_Control(pDX, IDC_SPIN_J2, m_spinJ2);
	DDX_Control(pDX, IDC_SPIN_J1, m_spinJ1);
	DDX_Control(pDX, IDC_COMBO_CARTESIAN_COORD, m_comboCartesianCoord);
	DDX_Control(pDX, IDC_EDIT_REV_J6, m_editInverseJ6);
	DDX_Control(pDX, IDC_EDIT_REV_J5, m_editInverseJ5);
	DDX_Control(pDX, IDC_EDIT_REV_J4, m_editInverseJ4);
	DDX_Control(pDX, IDC_EDIT_REV_J3, m_editInverseJ3);
	DDX_Control(pDX, IDC_EDIT_REV_J2, m_editInverseJ2);
	DDX_Control(pDX, IDC_EDIT_REV_J1, m_editInverseJ1);
	DDX_Control(pDX, IDC_EDIT_J6, m_editJ6);
	DDX_Control(pDX, IDC_EDIT_J5, m_editJ5);
	DDX_Control(pDX, IDC_EDIT_J4, m_editJ4);
	DDX_Control(pDX, IDC_EDIT_J3, m_editJ3);
	DDX_Control(pDX, IDC_EDIT_J2, m_editJ2);
	DDX_Control(pDX, IDC_EDIT_J1, m_editJ1);
	DDX_Control(pDX, IDC_EDIT_W, m_editW);
	DDX_Control(pDX, IDC_EDIT_Z, m_editZ);
	DDX_Control(pDX, IDC_EDIT_Y, m_editY);
	DDX_Control(pDX, IDC_EDIT_K, m_editK);
	DDX_Control(pDX, IDC_EDIT_J, m_editJ);
	DDX_Control(pDX, IDC_EDIT_I, m_editI);
	DDX_Control(pDX, IDC_EDIT_X, m_editX);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_FRAME_ZERO, m_editFrameZero);
}


BEGIN_MESSAGE_MAP(CRobotPage, CResizablePage)
	//{{AFX_MSG_MAP(CRobotPage)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ZERO, OnButtonZero)
	ON_CBN_SELCHANGE(IDC_COMBO_CARTESIAN_COORD, OnSelchangeComboCartesianCoord)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_EN_CHANGE(IDC_EDIT_BASE_COORD, OnChangeEditBaseCoord)
	ON_EN_CHANGE(IDC_EDIT_BASE_J1, OnChangeEditBaseJ1)
	ON_EN_CHANGE(IDC_EDIT_J1_J2, OnChangeEditJ1J2)
	ON_EN_CHANGE(IDC_EDIT_J2_J3, OnChangeEditJ2J3)
	ON_EN_CHANGE(IDC_EDIT_J3_J4, OnChangeEditJ3J4)
	ON_EN_CHANGE(IDC_EDIT_J4_J5, OnChangeEditJ4J5)
	ON_EN_CHANGE(IDC_EDIT_J5_J6, OnChangeEditJ5J6)
	ON_CBN_SELCHANGE(IDC_COMBO_VIRTUAL_ACTUAL, OnSelchangeComboVirtualActual)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_J1, OnChangeEditStepsizeJ1)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_J2, OnChangeEditStepsizeJ2)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_J3, OnChangeEditStepsizeJ3)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_J4, OnChangeEditStepsizeJ4)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_J5, OnChangeEditStepsizeJ5)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE_J6, OnChangeEditStepsizeJ6)
	ON_EN_CHANGE(IDC_EDIT_J6_TT, OnChangeEditJ6Tt)
	ON_EN_CHANGE(IDC_EDIT_SERIAL_NUMBER, OnChangeEditSerialNumber)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_END_EFFECTOR, OnButtonDownloadEndEffector)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_ALL_KINEMATICS, OnButtonDownloadAllKinematics)
	ON_BN_CLICKED(IDC_BUTTON_BOTH_ROBOTS, OnButtonBothRobots)
	ON_BN_CLICKED(IDC_CHECK_ROBOT_0, OnCheckRobot0)
	ON_BN_CLICKED(IDC_CHECK_ROBOT_1, OnCheckRobot1)
	ON_EN_CHANGE(IDC_EDIT_BASE_ROT_X, OnChangeEditBaseRotX)
	ON_EN_CHANGE(IDC_EDIT_BASE_ROT_Y, OnChangeEditBaseRotY)
	ON_EN_CHANGE(IDC_EDIT_BASE_ROT_Z, OnChangeEditBaseRotZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BASE_ROT_X, OnDeltaposSpinBaseRotX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BASE_ROT_Y, OnDeltaposSpinBaseRotY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BASE_ROT_Z, OnDeltaposSpinBaseRotZ)
	ON_BN_CLICKED(IDC_BUTTON_BASE, OnButtonBase)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_FRAME_ZERO, &CRobotPage::OnEnChangeEditFrameZero)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotPage message handlers

BOOL CRobotPage::OnInitDialog() 
{
	CString Buff;
	CResizablePage::OnInitDialog();
	UDACCEL	Accel;
	Accel.nInc=5;
	Accel.nSec=1;
	
	m_comboVirtualActual.AddString(L"Virtual");
	m_comboVirtualActual.AddString(L"Actual");

	m_spinJ1.SetRange(0,360);
	m_spinJ2.SetRange(0,360);
	m_spinJ3.SetRange(0,360);
	m_spinJ4.SetRange(0,360);
	m_spinJ5.SetRange(0,360);
	m_spinJ6.SetRange(0,360);
	m_spinJ1.SetAccel(1,&Accel);
	m_spinJ2.SetAccel(1,&Accel);
	m_spinJ3.SetAccel(1,&Accel);
	m_spinJ4.SetAccel(1,&Accel);
	m_spinJ5.SetAccel(1,&Accel);
	m_spinJ6.SetAccel(1,&Accel);


	Buff.LoadString(IDS_Tool_Tip);		m_comboCartesianCoord.AddString(Buff);
	Buff.LoadString(IDS_J6);			m_comboCartesianCoord.AddString(Buff);
	Buff.LoadString(IDS_Inverse_J6);	m_comboCartesianCoord.AddString(Buff);
	Buff.LoadString(IDS_Wrist);			m_comboCartesianCoord.AddString(Buff);
	Buff.LoadString(IDS_Inverse_Wrist);	m_comboCartesianCoord.AddString(Buff);

	
	UpdateAllControls();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRobotPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

BOOL CRobotPage::OnSetActive()
{
	theApp.m_LastSettings.nLastRobotTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();

	SetTimer(1, 200, NULL);

	return CPropertyPage::OnSetActive();
}

void CRobotPage::UpdateAllControls()
{
	CString Buff;
	if (GetSafeHwnd() == NULL) return;

	switch(m_nRobot) {
	case 0: m_checkRobot0.SetCheck(true);
		 m_checkRobot1.SetCheck(false);
		break;
	case 1: m_checkRobot0.SetCheck(false);
		 m_checkRobot1.SetCheck(true);
		break;
	}
	m_checkRobot0.SetWindowText(theApp.m_DlgSideName[0]);
	m_checkRobot1.SetWindowText(theApp.m_DlgSideName[1]);

	m_comboVirtualActual.SetCurSel(theApp.m_LastSettings.nVirtualActualRobot);

	m_editName.SetWindowText(theApp.m_Robot[m_nRobot].m_strName);
	m_editSerialNumber.SetWindowText(theApp.m_Robot[m_nRobot].m_strSerialNumber);



	m_spinJ1.SetPos((int)(m_fJointDegrees[0] + 180.0f));
	m_spinJ2.SetPos((int)(m_fJointDegrees[1] + 180.0f));
	m_spinJ3.SetPos((int)(m_fJointDegrees[2] + 180.0f));
	m_spinJ4.SetPos((int)(m_fJointDegrees[3] + 180.0f));
	m_spinJ5.SetPos((int)(m_fJointDegrees[4] + 180.0f));
	m_spinJ6.SetPos((int)(m_fJointDegrees[5] + 180.0f));

	Buff.Format(L"%.01f",m_fJointDegrees[0]);
	m_editJ1.SetWindowText(Buff);
	Buff.Format(L"%.01f",m_fJointDegrees[1]);
	m_editJ2.SetWindowText(Buff);
	Buff.Format(L"%.01f",m_fJointDegrees[2]);
	m_editJ3.SetWindowText(Buff);
	Buff.Format(L"%.01f",m_fJointDegrees[3]);
	m_editJ4.SetWindowText(Buff);
	Buff.Format(L"%.01f",m_fJointDegrees[4]);
	m_editJ5.SetWindowText(Buff);
	Buff.Format(L"%.01f",m_fJointDegrees[5]);
	m_editJ6.SetWindowText(Buff);

	m_comboCartesianCoord.SetCurSel(m_nCartesianCoordDisplay);

	for(int jj=0;jj<6;jj++) {
		theApp.m_Robot[m_nRobot].m_fJointAngle[jj] = m_fJointDegrees[jj] * DEG_TO_RAD;
	};
	theApp.m_Robot[m_nRobot].InitializeJointMatrices();

	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f",theApp.m_Robot[m_nRobot].m_vBase.x,theApp.m_Robot[m_nRobot].m_vBase.y,theApp.m_Robot[m_nRobot].m_vBase.z);
	m_editBaseCoord.SetWindowText(Buff);
	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f",theApp.m_Robot[m_nRobot].m_vBaseJ1.x,theApp.m_Robot[m_nRobot].m_vBaseJ1.y,theApp.m_Robot[m_nRobot].m_vBaseJ1.z);
	m_editBaseJ1.SetWindowText(Buff);
	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f",theApp.m_Robot[m_nRobot].m_vJ1J2.x,theApp.m_Robot[m_nRobot].m_vJ1J2.y,theApp.m_Robot[m_nRobot].m_vJ1J2.z);
	m_editJ1J2.SetWindowText(Buff);
	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f",theApp.m_Robot[m_nRobot].m_vJ2J3.x,theApp.m_Robot[m_nRobot].m_vJ2J3.y,theApp.m_Robot[m_nRobot].m_vJ2J3.z);
	m_editJ2J3.SetWindowText(Buff);
	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f",theApp.m_Robot[m_nRobot].m_vJ3J4.x,theApp.m_Robot[m_nRobot].m_vJ3J4.y,theApp.m_Robot[m_nRobot].m_vJ3J4.z);
	m_editJ3J4.SetWindowText(Buff);
	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f",theApp.m_Robot[m_nRobot].m_vJ4J5.x,theApp.m_Robot[m_nRobot].m_vJ4J5.y,theApp.m_Robot[m_nRobot].m_vJ4J5.z);
	m_editJ4J5.SetWindowText(Buff);
	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f",theApp.m_Robot[m_nRobot].m_vJ5J6.x,theApp.m_Robot[m_nRobot].m_vJ5J6.y,theApp.m_Robot[m_nRobot].m_vJ5J6.z);
	m_editJ5J6.SetWindowText(Buff);

	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f\tI %.05f\tJ %.05f\tK %.05f",theApp.m_Robot[m_nRobot].m_vJ6ToolTip.x,theApp.m_Robot[m_nRobot].m_vJ6ToolTip.y,theApp.m_Robot[m_nRobot].m_vJ6ToolTip.z,
										theApp.m_Robot[m_nRobot].m_vAtToolTip.x, theApp.m_Robot[m_nRobot].m_vAtToolTip.y, theApp.m_Robot[m_nRobot].m_vAtToolTip.z);
	m_editJ6toTT.SetWindowText(Buff);

	int nAxis = m_nRobot * 6;
	if(theApp.m_Robot[m_nRobot].m_fStepSize[0] >= 0.0001f) {
		Buff.Format(_T("%.07f"),theApp.m_Robot[m_nRobot].m_fStepSize[0]);
	} else {
		Buff.Format(_T("%11.05g"),theApp.m_Robot[m_nRobot].m_fStepSize[0]);
	}
	m_editStepSizeJ1.SetWindowText(Buff);

	if(theApp.m_Robot[m_nRobot].m_fStepSize[1] >= 0.0001f) {
		Buff.Format(_T("%.07f"),theApp.m_Robot[m_nRobot].m_fStepSize[1]);
	} else {
		Buff.Format(_T("%11.05g"),theApp.m_Robot[m_nRobot].m_fStepSize[1]);
	}
	m_editStepSizeJ2.SetWindowText(Buff);

	if(theApp.m_Robot[m_nRobot].m_fStepSize[2] >= 0.0001f) {
		Buff.Format(_T("%.07f"),theApp.m_Robot[m_nRobot].m_fStepSize[2]);
	} else {
		Buff.Format(_T("%11.05g"),theApp.m_Robot[m_nRobot].m_fStepSize[2]);
	}
	m_editStepSizeJ3.SetWindowText(Buff);

	if(theApp.m_Robot[m_nRobot].m_fStepSize[3] >= 0.0001f) {
		Buff.Format(_T("%.07f"),theApp.m_Robot[m_nRobot].m_fStepSize[3]);
	} else {
		Buff.Format(_T("%11.05g"),theApp.m_Robot[m_nRobot].m_fStepSize[3]);
	}
	m_editStepSizeJ4.SetWindowText(Buff);

	if(theApp.m_Robot[m_nRobot].m_fStepSize[4] >= 0.0001f) {
		Buff.Format(_T("%.07f"),theApp.m_Robot[m_nRobot].m_fStepSize[4]);
	} else {
		Buff.Format(_T("%11.05g"),theApp.m_Robot[m_nRobot].m_fStepSize[4]);
	}
	m_editStepSizeJ5.SetWindowText(Buff);

	if(theApp.m_Robot[m_nRobot].m_fStepSize[5] >= 0.0001f) {
		Buff.Format(_T("%.07f"),theApp.m_Robot[m_nRobot].m_fStepSize[5]);
	} else {
		Buff.Format(_T("%11.05g"),theApp.m_Robot[m_nRobot].m_fStepSize[5]);
	}
	m_editStepSizeJ6.SetWindowText(Buff);

	Buff.Format(L"%.02f%s",theApp.m_Robot[m_nRobot].m_fBaseRotX,DEGREES);
	m_editBaseRotX.SetWindowText(Buff);

	Buff.Format(L"%.02f%s",theApp.m_Robot[m_nRobot].m_fBaseRotY,DEGREES);
	m_editBaseRotY.SetWindowText(Buff);
	
	Buff.Format(L"%.02f%s",theApp.m_Robot[m_nRobot].m_fBaseRotZ,DEGREES);
	m_editBaseRotZ.SetWindowText(Buff);

	Buff.Format(L"X:%.01f Y:%.01f Z:%.01f", theApp.m_Tank.vFrameZero[m_nRobot].x, theApp.m_Tank.vFrameZero[m_nRobot].y, theApp.m_Tank.vFrameZero[m_nRobot].z);
	m_editFrameZero.SetWindowTextW(Buff);

	SetAccessPrivelage();

}

void CRobotPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;

	int	nTemp=pSpin->GetPos()&0xffff;
	float fTemp = (float)(nTemp - 180.0f);

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_J1:	m_fJointDegrees[0] = fTemp;
		break;
	case IDC_SPIN_J2:	m_fJointDegrees[1] = fTemp;
		break;
	case IDC_SPIN_J3:	m_fJointDegrees[2] = fTemp;
		break;
	case IDC_SPIN_J4:	m_fJointDegrees[3] = fTemp;
		break;
	case IDC_SPIN_J5:	m_fJointDegrees[4] = fTemp;
		break;
	case IDC_SPIN_J6:	m_fJointDegrees[5] = fTemp;
		break;
	}
	UpdateAllControls();
	InvalidateProfileSheet();

	CResizablePage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRobotPage::InvalidateProfileSheet()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;


	pFrame->Invalidate3DImages();

}

void CRobotPage::OnTimer(UINT nIDEvent) 
{
	if (GetSafeHwnd() == NULL) return;
	bool bDoInverse = false;
	CString Buff,Temp;
	D3DXVECTOR3 vPos;
	D3DXQUATERNION quatPos;
	D3DXMATRIXA16 matWrist;
	D3DXMATRIXA16 matQuatRot;
	CCoord Cp;
	D3DXQUATERNION quatTemp;

	if(theApp.m_LastSettings.nVirtualActualRobot == 1) {

		theApp.m_Ethercat.InvalidateRobotJoints();

		if(m_fJointDegrees[0] - (theApp.m_Robot[m_nRobot].m_fJointAngle[0] * RAD_TO_DEG)) {
			m_fJointDegrees[0] = theApp.m_Robot[m_nRobot].m_fJointAngle[0] * RAD_TO_DEG;
			m_spinJ1.SetPos((int)(m_fJointDegrees[0] + 180.0f));
			Buff.Format(L"%.02f",m_fJointDegrees[0]);
			m_editJ1.SetWindowText(Buff);
		}
		if(m_fJointDegrees[1] - (theApp.m_Robot[m_nRobot].m_fJointAngle[1] * RAD_TO_DEG)) {
			m_fJointDegrees[1] = theApp.m_Robot[m_nRobot].m_fJointAngle[1] * RAD_TO_DEG;
			m_spinJ2.SetPos((int)(m_fJointDegrees[1] + 180.0f));
			Buff.Format(L"%.02f",m_fJointDegrees[1]);
			m_editJ2.SetWindowText(Buff);
		}
		if(m_fJointDegrees[2] - (theApp.m_Robot[m_nRobot].m_fJointAngle[2] * RAD_TO_DEG)) {
			m_fJointDegrees[2] = theApp.m_Robot[m_nRobot].m_fJointAngle[2] * RAD_TO_DEG;
			m_spinJ3.SetPos((int)(m_fJointDegrees[2] + 180.0f));
			Buff.Format(L"%.02f",m_fJointDegrees[2]);
			m_editJ3.SetWindowText(Buff);
		}
		if(m_fJointDegrees[3] - (theApp.m_Robot[m_nRobot].m_fJointAngle[3] * RAD_TO_DEG)) {
			m_fJointDegrees[3] = theApp.m_Robot[m_nRobot].m_fJointAngle[3] * RAD_TO_DEG;
			m_spinJ4.SetPos((int)(m_fJointDegrees[3] + 180.0f));
			Buff.Format(L"%.02f",m_fJointDegrees[3]);
			m_editJ4.SetWindowText(Buff);
		}
		if(m_fJointDegrees[4] - (theApp.m_Robot[m_nRobot].m_fJointAngle[4] * RAD_TO_DEG)) {
			m_fJointDegrees[4] = theApp.m_Robot[m_nRobot].m_fJointAngle[4] * RAD_TO_DEG;
			m_spinJ5.SetPos((int)(m_fJointDegrees[4] + 180.0f));
			Buff.Format(L"%.02f",m_fJointDegrees[4]);
			m_editJ5.SetWindowText(Buff);
		}
		if(m_fJointDegrees[5] - (theApp.m_Robot[m_nRobot].m_fJointAngle[5] * RAD_TO_DEG)) {
			m_fJointDegrees[5] = theApp.m_Robot[m_nRobot].m_fJointAngle[5] * RAD_TO_DEG;
			m_spinJ6.SetPos((int)(m_fJointDegrees[5] + 180.0f));
			Buff.Format(L"%.02f",m_fJointDegrees[5]);
			m_editJ6.SetWindowText(Buff);
		}
		InvalidateProfileSheet();
	}


	switch(m_nCartesianCoordDisplay) {
	case 0:
		theApp.m_Robot[m_nRobot].GetToolTipPos(&vPos, &quatPos);
		break;
	case 1:
		theApp.m_Robot[m_nRobot].GetRobotJ6(&vPos, &quatPos);
		break;
	case 2:
		theApp.m_Robot[m_nRobot].GetInverseJ6(&vPos, &quatPos);
		break;
	case 3:
		vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		theApp.m_Robot[m_nRobot].TransformVectorFromJ5(&vPos);
		break;
	case 4:
		theApp.m_Robot[m_nRobot].GetInverseWrist(&vPos, &quatPos);
		break;
	}

	if((m_vOldPos != vPos) || (m_quatOldPos != quatPos)) {
		Buff.Format(L"%.02f",vPos.x);
		m_editX.SetWindowText(Buff);
		Buff.Format(L"%.02f",vPos.y);
		m_editY.SetWindowText(Buff);
		Buff.Format(L"%.02f",vPos.z);
		m_editZ.SetWindowText(Buff);
		Buff.Format(L"%.04f",quatPos.x);
		m_editI.SetWindowText(Buff);
		Buff.Format(L"%.04f",quatPos.y);
		m_editJ.SetWindowText(Buff);
		Buff.Format(L"%.04f",quatPos.z);
		m_editK.SetWindowText(Buff);
		Buff.Format(L"%.04f",quatPos.w);
		m_editW.SetWindowText(Buff);

		//Normal from quaternion
		float fCosAngle = quatPos.w;
		float fAngle = 2.0f * acosf(fCosAngle) * RAD_TO_DEG;
		float fSinAngle = sqrt(1.0f - fCosAngle * fCosAngle);

		Cp.Side0.norm.x = quatPos.x / fSinAngle;
		Cp.Side0.norm.y = quatPos.y / fSinAngle;
		Cp.Side0.norm.z = quatPos.z / fSinAngle;

		D3DXMatrixRotationQuaternion(&matQuatRot,&quatPos);
		Buff.Format(L"%.04f",matQuatRot._31);	m_editNormI.SetWindowText(Buff);
		Buff.Format(L"%.04f",matQuatRot._32);	m_editNormJ.SetWindowText(Buff);
		Buff.Format(L"%.04f",matQuatRot._33);	m_editNormK.SetWindowText(Buff);

		float fYaw = atan2f(matQuatRot._32,  matQuatRot._31) * RAD_TO_DEG;
		float fPitch = acosf(matQuatRot._33) * RAD_TO_DEG;
		float fRoll = atan2f(-matQuatRot._23,-matQuatRot._13) * -RAD_TO_DEG;

		Buff.Format(L"%.01f", fYaw);	m_editYaw.SetWindowText(Buff);
		Buff.Format(L"%.01f", fPitch);	m_editPitch.SetWindowText(Buff);
		Buff.Format(L"%.01f", fRoll);	m_editRoll.SetWindowText(Buff);


		Cp.Side0.fI = -matQuatRot._31;
		Cp.Side0.fJ = -matQuatRot._32;
		Cp.Side0.fK = -matQuatRot._33;
		Cp.QuaternionFromNormal(0,&quatTemp);

		bDoInverse = true;
	
	}
	m_vOldPos = vPos;
	m_quatOldPos = quatPos;

//	if(bDoInverse == true) {
		theApp.m_Robot[m_nRobot].InitializeJointMatrices();
		theApp.m_Robot[m_nRobot].InitializeLimbMatrices();
		theApp.m_Robot[m_nRobot].GetToolTipPos(&vPos, &quatPos);
		theApp.m_Robot[m_nRobot].InverseKinematic(&vPos, &quatPos, &matWrist);


		Buff.Format(L"%.02f",theApp.m_Robot[m_nRobot].m_fInverseJointAngle[0] * RAD_TO_DEG);
		m_editInverseJ1.SetWindowText(Buff);
		Buff.Format(L"%.02f",theApp.m_Robot[m_nRobot].m_fInverseJointAngle[1] * RAD_TO_DEG);
		m_editInverseJ2.SetWindowText(Buff);
		Buff.Format(L"%.02f",theApp.m_Robot[m_nRobot].m_fInverseJointAngle[2] * RAD_TO_DEG);
		m_editInverseJ3.SetWindowText(Buff);
		Buff.Format(L"%.02f",theApp.m_Robot[m_nRobot].m_fInverseJointAngle[3] * RAD_TO_DEG);
		m_editInverseJ4.SetWindowText(Buff);
		Buff.Format(L"%.02f",theApp.m_Robot[m_nRobot].m_fInverseJointAngle[4] * RAD_TO_DEG);
		m_editInverseJ5.SetWindowText(Buff);
		Buff.Format(L"%.02f",theApp.m_Robot[m_nRobot].m_fInverseJointAngle[5] * RAD_TO_DEG);
		m_editInverseJ6.SetWindowText(Buff);
//	}


	CResizablePage::OnTimer(nIDEvent);
}


BOOL CRobotPage::OnKillActive() 
{
	KillTimer(1);
	
	return CResizablePage::OnKillActive();
}

void CRobotPage::OnButtonZero() 
{
	ZeroMemory(m_fJointDegrees,sizeof m_fJointDegrees);

	for(int jj=0;jj<6;jj++) {
		theApp.m_Robot[m_nRobot].m_fJointAngle[jj] = 0.0f;
		theApp.m_Robot[m_nRobot].m_fInverseJointAngle[jj] = 0.0f;
		theApp.m_Robot[m_nRobot].m_fLastJointAngle[jj] = 0.0f;
	};
	theApp.m_Robot[m_nRobot].InitializeJointMatrices();
	UpdateAllControls();
}


void CRobotPage::OnSelchangeComboCartesianCoord() 
{
	m_nCartesianCoordDisplay = m_comboCartesianCoord.GetCurSel();
	UpdateAllControls();
	
}


void CRobotPage::OnChangeEditName() 
{
	m_editName.GetWindowText(theApp.m_Robot[m_nRobot].m_strName);

}

void CRobotPage::OnChangeEditSerialNumber() 
{
	m_editSerialNumber.GetWindowText(theApp.m_Robot[m_nRobot].m_strSerialNumber);
}

bool CRobotPage::ExtractVector(CEdit *pEdit, D3DXVECTOR3 *pVec, D3DXVECTOR3 *pVecNorm,float *pfAngle)
{
	CString Buff;
	int ii,nStart;

	pEdit->GetWindowText(Buff);
	Buff.MakeUpper();

	for(ii=0;ii<Buff.GetLength();ii++) {
		if(!(isalnum(Buff.GetAt(ii)) || Buff.GetAt(ii) == '.' || Buff.GetAt(ii) == '-')) {
			Buff.Delete(ii);
			ii=-1;
		}
	}

	if((nStart=Buff.Find(L"X",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"X%f",&pVec->x);
	if((nStart=Buff.Find(L"Y",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Y%f",&pVec->y);
	if((nStart=Buff.Find(L"Z",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Z%f",&pVec->z);

	if(pVecNorm != NULL) {
		if((nStart=Buff.Find(L"I",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"I%f",&pVecNorm->x);
		if((nStart=Buff.Find(L"J",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"J%f",&pVecNorm->y);
		if((nStart=Buff.Find(L"K",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"K%f",&pVecNorm->z);
	}
	if(pfAngle != NULL) {
		if((nStart=Buff.Find(L"U",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"U%f",pfAngle);
	}

	return true;
}

void CRobotPage::OnChangeEditBaseCoord() 
{
	ExtractVector(&m_editBaseCoord,&theApp.m_Robot[m_nRobot].m_vBase);
	theApp.m_Robot[m_nRobot].InitializeMatrices();

}


void CRobotPage::OnChangeEditBaseJ1() 
{
	ExtractVector(&m_editBaseJ1,&theApp.m_Robot[m_nRobot].m_vBaseJ1);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotPage::OnChangeEditJ1J2() 
{
	ExtractVector(&m_editJ1J2,&theApp.m_Robot[m_nRobot].m_vJ1J2);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotPage::OnChangeEditJ2J3() 
{
	ExtractVector(&m_editJ2J3,&theApp.m_Robot[m_nRobot].m_vJ2J3);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotPage::OnChangeEditJ3J4() 
{
	ExtractVector(&m_editJ3J4,&theApp.m_Robot[m_nRobot].m_vJ3J4);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotPage::OnChangeEditJ4J5() 
{
	ExtractVector(&m_editJ4J5,&theApp.m_Robot[m_nRobot].m_vJ4J5);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotPage::OnChangeEditJ5J6() 
{
	ExtractVector(&m_editJ5J6,&theApp.m_Robot[m_nRobot].m_vJ5J6);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotPage::OnChangeEditJ6Tt() 
{
	ExtractVector(&m_editJ6toTT,&theApp.m_Robot[m_nRobot].m_vJ6ToolTip,&theApp.m_Robot[m_nRobot].m_vAtToolTip);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotPage::OnSelchangeComboVirtualActual() 
{
	theApp.m_LastSettings.nVirtualActualRobot = m_comboVirtualActual.GetCurSel() & 0x01;
	UpdateAllControls();
}

void CRobotPage::SetAccessPrivelage()
{
	if (GetSafeHwnd() == NULL) return;
	if(m_hWnd == NULL) return;

	bool bFlag = false;

	if(theApp.m_LastSettings.nVirtualActualRobot == 0) bFlag = true;

	m_editJ1.EnableWindow(bFlag);
	m_editJ2.EnableWindow(bFlag);
	m_editJ3.EnableWindow(bFlag);
	m_editJ4.EnableWindow(bFlag);
	m_editJ5.EnableWindow(bFlag);
	m_editJ6.EnableWindow(bFlag);
	m_spinJ1.EnableWindow(bFlag);
	m_spinJ2.EnableWindow(bFlag);
	m_spinJ3.EnableWindow(bFlag);
	m_spinJ4.EnableWindow(bFlag);
	m_spinJ5.EnableWindow(bFlag);
	m_spinJ6.EnableWindow(bFlag);
}

void CRobotPage::OnChangeEditStepsizeJ1() 
{
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fStepSize[0];

	m_editStepSizeJ1.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[m_nRobot].m_fStepSize[0]);
	if((fTemp - theApp.m_Robot[m_nRobot].m_fStepSize[0]) && (theApp.m_Robot[m_nRobot].m_fStepSize[0] > 0.0f)) {

	}
}

void CRobotPage::OnChangeEditStepsizeJ2() 
{
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fStepSize[1];

	m_editStepSizeJ2.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[m_nRobot].m_fStepSize[1]);
	if((fTemp - theApp.m_Robot[m_nRobot].m_fStepSize[1]) && (theApp.m_Robot[m_nRobot].m_fStepSize[1] > 0.0f)) {
	}
}

void CRobotPage::OnChangeEditStepsizeJ3() 
{
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fStepSize[2];

	m_editStepSizeJ3.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[m_nRobot].m_fStepSize[2]);
	if((fTemp - theApp.m_Robot[m_nRobot].m_fStepSize[2]) && (theApp.m_Robot[m_nRobot].m_fStepSize[2] > 0.0f)) {
	}
}

void CRobotPage::OnChangeEditStepsizeJ4() 
{
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fStepSize[3];

	m_editStepSizeJ4.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[m_nRobot].m_fStepSize[3]);
	if((fTemp - theApp.m_Robot[m_nRobot].m_fStepSize[3]) && (theApp.m_Robot[m_nRobot].m_fStepSize[3] > 0.0f)) {
	}
}

void CRobotPage::OnChangeEditStepsizeJ5() 
{
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fStepSize[4];

	m_editStepSizeJ5.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[m_nRobot].m_fStepSize[4]);
	if((fTemp - theApp.m_Robot[m_nRobot].m_fStepSize[4]) && (theApp.m_Robot[m_nRobot].m_fStepSize[4] > 0.0f)) {
	}
}

void CRobotPage::OnChangeEditStepsizeJ6() 
{
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fStepSize[5];

	m_editStepSizeJ6.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[m_nRobot].m_fStepSize[5]);
	if((fTemp - theApp.m_Robot[m_nRobot].m_fStepSize[5]) && (theApp.m_Robot[m_nRobot].m_fStepSize[5] > 0.0f)) {
	}
}



void CRobotPage::OnButtonDownloadEndEffector() 
{
	CString Buff;
	CString TX[2] = {L"LengthTX", L"B_LengthTX"};
	CString TY[2] = {L"LengthTY", L"B_LengthTY"};
	CString TZ[2] = {L"LengthTZ", L"B_LengthTZ"};
	CString rotY[2] = {L"ToolRY", L"B_ToolRY"};
	float fAngle;

	Buff.Format(L"%s=%f",TX[m_nRobot], theApp.m_Robot[m_nRobot].m_vJ6ToolTip.x);
	theApp.m_FBCtrl.SendStr(Buff);
	Buff.Format(L"%s=%f",TY[m_nRobot], theApp.m_Robot[m_nRobot].m_vJ6ToolTip.y);
	theApp.m_FBCtrl.SendStr(Buff);
	Buff.Format(L"%s=%f",TZ[m_nRobot], theApp.m_Robot[m_nRobot].m_vJ6ToolTip.z);
	theApp.m_FBCtrl.SendStr(Buff);

	fAngle = atan2f(theApp.m_Robot[m_nRobot].m_vAtToolTip.y,theApp.m_Robot[m_nRobot].m_vAtToolTip.z) * RAD_TO_DEG;
	Buff.Format(L"%s=%f",rotY[m_nRobot], fAngle);
	theApp.m_FBCtrl.SendStr(Buff);

}

void CRobotPage::OnButtonDownloadAllKinematics() 
{
	theApp.m_FBCtrl.DownloadRobotKinematics(m_nRobot);
}

void CRobotPage::OnButtonBothRobots() 
{
	for(int ii=0;ii<2;ii++) {
		theApp.m_FBCtrl.DownloadRobotKinematics(ii);
		theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(ii,true);
	}		
}

void CRobotPage::OnCheckRobot0() 
{
	m_nRobot = 0;

	for(int jj=0;jj<6;jj++) {
		m_fJointDegrees[jj] = theApp.m_Robot[m_nRobot].m_fJointAngle[jj] / DEG_TO_RAD;
	};

	UpdateAllControls();
}

void CRobotPage::OnCheckRobot1() 
{
	m_nRobot = 1;

	for(int jj=0;jj<6;jj++) {
		m_fJointDegrees[jj] = theApp.m_Robot[m_nRobot].m_fJointAngle[jj] / DEG_TO_RAD;
	};

	UpdateAllControls();
}


void CRobotPage::OnChangeEditBaseRotX() 
{
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fBaseRotX;

	m_editBaseRotX.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[m_nRobot].m_fBaseRotX);
	if(fTemp - theApp.m_Robot[m_nRobot].m_fBaseRotX) {
		theApp.m_Robot[m_nRobot].InitializeJointMatrices();

	}

}

void CRobotPage::OnChangeEditBaseRotY() 
{
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fBaseRotY;

	m_editBaseRotY.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[m_nRobot].m_fBaseRotY);
	if(fTemp - theApp.m_Robot[m_nRobot].m_fBaseRotY) {
		theApp.m_Robot[m_nRobot].InitializeJointMatrices();

	}
}

void CRobotPage::OnChangeEditBaseRotZ() 
{
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fBaseRotZ;

	m_editBaseRotZ.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[m_nRobot].m_fBaseRotZ);
	if(fTemp - theApp.m_Robot[m_nRobot].m_fBaseRotZ) {
		theApp.m_Robot[m_nRobot].InitializeJointMatrices();

	}
}

void CRobotPage::OnDeltaposSpinBaseRotX(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fBaseRotX;

	theApp.m_Robot[m_nRobot].m_fBaseRotX += ((float)pNMUpDown->iDelta * 0.01f);
	MinMax(&theApp.m_Robot[m_nRobot].m_fBaseRotX,-180.0f,180.0f);

	if(fTemp - theApp.m_Robot[m_nRobot].m_fBaseRotX) {
		theApp.m_Robot[m_nRobot].InitializeJointMatrices();
		UpdateAllControls();
	}
	
	*pResult = 0;
}

void CRobotPage::OnDeltaposSpinBaseRotY(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fBaseRotX;

	theApp.m_Robot[m_nRobot].m_fBaseRotY += ((float)pNMUpDown->iDelta * 0.01f);
	MinMax(&theApp.m_Robot[m_nRobot].m_fBaseRotY,-180.0f,180.0f);

	if(fTemp - theApp.m_Robot[m_nRobot].m_fBaseRotY) {
		theApp.m_Robot[m_nRobot].InitializeJointMatrices();
		UpdateAllControls();
	}
	
	*pResult = 0;
}

void CRobotPage::OnDeltaposSpinBaseRotZ(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fTemp = theApp.m_Robot[m_nRobot].m_fBaseRotZ;

	theApp.m_Robot[m_nRobot].m_fBaseRotZ += ((float)pNMUpDown->iDelta * 0.01f);
	MinMax(&theApp.m_Robot[m_nRobot].m_fBaseRotZ,-180.0f,180.0f);

	if(fTemp - theApp.m_Robot[m_nRobot].m_fBaseRotZ) {
		theApp.m_Robot[m_nRobot].InitializeJointMatrices();
		UpdateAllControls();
	}
	
	*pResult = 0;
}

void CRobotPage::OnButtonBase() 
{
	CCoord CpSurface;
	theApp.m_Motors.GetSurfacePos(&CpSurface);

	if(m_nRobot == 0) {
		theApp.m_Robot[m_nRobot].m_vBase.x -= CpSurface.Side[0].fX;
		theApp.m_Robot[m_nRobot].m_vBase.y -= CpSurface.Side[0].fY;
		theApp.m_Robot[m_nRobot].m_vBase.z -= CpSurface.Side[0].fZ;
	} else {
		theApp.m_Robot[m_nRobot].m_vBase.x -= CpSurface.Side[1].fX;
		theApp.m_Robot[m_nRobot].m_vBase.y -= CpSurface.Side[1].fY;
		theApp.m_Robot[m_nRobot].m_vBase.z -= CpSurface.Side[1].fZ;
	};
	
	UpdateAllControls();
}


void CRobotPage::OnEnChangeEditFrameZero()
{
	ExtractVector(&m_editFrameZero, &theApp.m_Tank.vFrameZero[m_nRobot]);
}
