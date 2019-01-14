// RobotCalibrationPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotCalibrationPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotCalibrationPage property page

IMPLEMENT_DYNCREATE(CRobotCalibrationPage, CPropertyPage)

CRobotCalibrationPage::CRobotCalibrationPage() : CPropertyPage(CRobotCalibrationPage::IDD)
{
	//{{AFX_DATA_INIT(CRobotCalibrationPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nRobot = 0;

	ZeroMemory(m_vRobotPt, sizeof m_vRobotPt);
	m_vRobotNorm[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vRobotNorm[1] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	ZeroMemory(m_vTrackerPt, sizeof m_vTrackerPt);
}

CRobotCalibrationPage::~CRobotCalibrationPage()
{
}

void CRobotCalibrationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotCalibrationPage)
	DDX_Control(pDX, IDC_EDIT_TRACKER_DIFF, m_editTrackerDifference);
	DDX_Control(pDX, IDC_EDIT_PT_DIFF, m_editRobotDifference);
	DDX_Control(pDX, IDC_BUTTON_TRACKER_1, m_buttonTracker1);
	DDX_Control(pDX, IDC_BUTTON_TRACKER_0, m_buttonTracker0);
	DDX_Control(pDX, IDC_EDIT_PT_1, m_editPt1);
	DDX_Control(pDX, IDC_EDIT_PT_0, m_editPt0);
	DDX_Control(pDX, IDC_EDIT_J6_TT, m_editJ6toTT);
	DDX_Control(pDX, IDC_EDIT_J5_J6, m_editJ5J6);
	DDX_Control(pDX, IDC_EDIT_J4_J5, m_editJ4J5);
	DDX_Control(pDX, IDC_EDIT_J3_J4, m_editJ3J4);
	DDX_Control(pDX, IDC_EDIT_J2_J3, m_editJ2J3);
	DDX_Control(pDX, IDC_EDIT_J1_J2, m_editJ1J2);
	DDX_Control(pDX, IDC_EDIT_BASE_J1, m_editBaseJ1);
	DDX_Control(pDX, IDC_EDIT_BASE_COORD, m_editBaseCoord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRobotCalibrationPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRobotCalibrationPage)
	ON_EN_CHANGE(IDC_EDIT_J1_J2, OnChangeEditJ1J2)
	ON_EN_CHANGE(IDC_EDIT_J2_J3, OnChangeEditJ2J3)
	ON_EN_CHANGE(IDC_EDIT_J3_J4, OnChangeEditJ3J4)
	ON_EN_CHANGE(IDC_EDIT_J4_J5, OnChangeEditJ4J5)
	ON_EN_CHANGE(IDC_EDIT_J5_J6, OnChangeEditJ5J6)
	ON_EN_CHANGE(IDC_EDIT_J6_TT, OnChangeEditJ6Tt)
	ON_EN_CHANGE(IDC_EDIT_BASE_J1, OnChangeEditBaseJ1)
	ON_EN_CHANGE(IDC_EDIT_BASE_COORD, OnChangeEditBaseCoord)
	ON_EN_CHANGE(IDC_EDIT_PT_0, OnChangeEditPt0)
	ON_EN_CHANGE(IDC_EDIT_PT_1, OnChangeEditPt1)
	ON_BN_CLICKED(IDC_BUTTON_TRACKER_0, OnButtonTracker0)
	ON_BN_CLICKED(IDC_BUTTON_TRACKER_1, OnButtonTracker1)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_0, OnButtonGoto0)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_1, OnButtonGoto1)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_ALL_KINEMATICS, OnButtonDownloadAllKinematics)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotCalibrationPage message handlers

BOOL CRobotCalibrationPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	UpdateAllControls();
	RenderTrackerPts();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRobotCalibrationPage::UpdateAllControls()
{
	
	CString Buff;
	if (GetSafeHwnd() == NULL) return;

//	m_comboRobotNumber.SetCurSel(m_nRobot);

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

	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f\tI %.03f\tJ %.03f\tK %.03f",theApp.m_Robot[m_nRobot].m_vJ6ToolTip.x,theApp.m_Robot[m_nRobot].m_vJ6ToolTip.y,theApp.m_Robot[m_nRobot].m_vJ6ToolTip.z,
										theApp.m_Robot[m_nRobot].m_vAtToolTip.x, theApp.m_Robot[m_nRobot].m_vAtToolTip.y, theApp.m_Robot[m_nRobot].m_vAtToolTip.z);
	m_editJ6toTT.SetWindowText(Buff);

	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f",m_vRobotPt[0].x,m_vRobotPt[0].y,m_vRobotPt[0].z);
	m_editPt0.SetWindowText(Buff);

	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f",m_vRobotPt[1].x,m_vRobotPt[1].y,m_vRobotPt[1].z);
	m_editPt1.SetWindowText(Buff);


}

BOOL CRobotCalibrationPage::OnSetActive() 
{
	
	
	theApp.m_LastSettings.nLastRobotTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();

	return CPropertyPage::OnSetActive();
}

void CRobotCalibrationPage::OnChangeEditBaseCoord() 
{
	

	ExtractVector(&m_editBaseCoord,&theApp.m_Robot[m_nRobot].m_vBase,&theApp.m_Robot[m_nRobot].m_vBaseNorm, &theApp.m_Robot[m_nRobot].m_fBaseRotX);
	theApp.m_Robot[m_nRobot].InitializeMatrices();

}


void CRobotCalibrationPage::OnChangeEditBaseJ1() 
{
	

	ExtractVector(&m_editBaseJ1,&theApp.m_Robot[m_nRobot].m_vBaseJ1);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotCalibrationPage::OnChangeEditJ1J2() 
{
	

	ExtractVector(&m_editJ1J2,&theApp.m_Robot[m_nRobot].m_vJ1J2);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotCalibrationPage::OnChangeEditJ2J3() 
{
	

	ExtractVector(&m_editJ2J3,&theApp.m_Robot[m_nRobot].m_vJ2J3);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotCalibrationPage::OnChangeEditJ3J4() 
{
	

	ExtractVector(&m_editJ3J4,&theApp.m_Robot[m_nRobot].m_vJ3J4);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotCalibrationPage::OnChangeEditJ4J5() 
{
	

	ExtractVector(&m_editJ4J5,&theApp.m_Robot[m_nRobot].m_vJ4J5);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotCalibrationPage::OnChangeEditJ5J6() 
{
	

	ExtractVector(&m_editJ5J6,&theApp.m_Robot[m_nRobot].m_vJ5J6);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

void CRobotCalibrationPage::OnChangeEditJ6Tt() 
{
	

	ExtractVector(&m_editJ6toTT,&theApp.m_Robot[m_nRobot].m_vJ6ToolTip,&theApp.m_Robot[m_nRobot].m_vAtToolTip);
	theApp.m_Robot[m_nRobot].InitializeMatrices();
}

bool CRobotCalibrationPage::ExtractVector(CEdit *pEdit, D3DXVECTOR3 *pVec, D3DXVECTOR3 *pVecNorm,float *pfAngle)
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


void CRobotCalibrationPage::OnChangeEditPt0() 
{
	CString Buff;

	ExtractVector(&m_editPt0,&m_vRobotPt[0]);
	Buff.Format(L"X %.01f  Y %.01f  Z %.01f",m_vRobotPt[1].x-m_vRobotPt[0].x,  m_vRobotPt[1].y-m_vRobotPt[0].y,  m_vRobotPt[1].z-m_vRobotPt[0].z);
	m_editRobotDifference.SetWindowText(Buff);
}

void CRobotCalibrationPage::OnChangeEditPt1() 
{
	CString Buff;

	ExtractVector(&m_editPt1,&m_vRobotPt[1]);
	Buff.Format(L"X %.01f  Y %.01f  Z %.01f",m_vRobotPt[1].x-m_vRobotPt[0].x,  m_vRobotPt[1].y-m_vRobotPt[0].y,  m_vRobotPt[1].z-m_vRobotPt[0].z);
	m_editRobotDifference.SetWindowText(Buff);
}

void CRobotCalibrationPage::OnButtonTracker0() 
{
	
	D3DXVECTOR3 vPos;

	int nStatus = theApp.m_FaroWorkerThread.GetRawXYZCoordinate(&vPos);

	switch(	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode ) {
	default:
		break;
	case 1:	theApp.m_FaroWorkerThread.GetFrameCoordinate(&vPos, &vPos);
		break;
	case 2:	theApp.m_FaroWorkerThread.GetToolCoordinate(&vPos, &vPos);
		break;
	}
	m_vTrackerPt[0] = vPos;
	RenderTrackerPts();
	
}

void CRobotCalibrationPage::OnButtonTracker1() 
{
	
	D3DXVECTOR3 vPos;

	int nStatus = theApp.m_FaroWorkerThread.GetRawXYZCoordinate(&vPos);

	switch(	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode ) {
	default:
		break;
	case 1:	theApp.m_FaroWorkerThread.GetFrameCoordinate(&vPos, &vPos);
		break;
	case 2:	theApp.m_FaroWorkerThread.GetToolCoordinate(&vPos, &vPos);
		break;
	}
	m_vTrackerPt[1] = vPos;
	RenderTrackerPts();
}

void CRobotCalibrationPage::RenderTrackerPts()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(L"X %.02f  Y %.02f  Z %.02f",m_vTrackerPt[0].x,m_vTrackerPt[0].y,m_vTrackerPt[0].z);
	m_buttonTracker0.SetWindowText(Buff);
	Buff.Format(L"X %.02f  Y %.02f  Z %.02f",m_vTrackerPt[1].x,m_vTrackerPt[1].y,m_vTrackerPt[1].z);
	m_buttonTracker1.SetWindowText(Buff);

	Buff.Format(L"X %.02f  Y %.02f  Z %.02f",m_vTrackerPt[1].x-m_vTrackerPt[0].x,  m_vTrackerPt[1].y-m_vTrackerPt[0].y,  m_vTrackerPt[1].z-m_vTrackerPt[0].z);
	m_editTrackerDifference.SetWindowText(Buff);

}

void CRobotCalibrationPage::OnButtonGoto0() 
{
	
	CCoord	CpSurface;
	CPolyCoord Line;

	theApp.m_Thread.StopCurrentThreadAction();
	CpSurface.Side0.pt = m_vRobotPt[0];
	CpSurface.Side1.pt = m_vRobotPt[0];
	CpSurface.Side0.norm = D3DXVECTOR4(-1.0f,0.0f,0.0f,1.0f);
	CpSurface.Side1.norm = D3DXVECTOR4(-1.0f,0.0f,0.0f,1.0f);

	Line.Add(CpSurface);
	theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
}

void CRobotCalibrationPage::OnButtonGoto1() 
{
	
	CCoord	CpSurface;
	CPolyCoord Line;

	theApp.m_Thread.StopCurrentThreadAction();
	CpSurface.Side0.pt = m_vRobotPt[1];
	CpSurface.Side1.pt = m_vRobotPt[1];
	CpSurface.Side0.norm = D3DXVECTOR4(-1.0f,0.0f,0.0f,1.0f);
	CpSurface.Side1.norm = D3DXVECTOR4(-1.0f,0.0f,0.0f,1.0f);

	Line.Add(CpSurface);
	theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
	
}

void CRobotCalibrationPage::OnButtonDownloadAllKinematics() 
{
	

	theApp.m_FBCtrl.DownloadRobotKinematics(m_nRobot);
}
