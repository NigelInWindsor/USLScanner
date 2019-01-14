// WristFingerPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "WristFingerPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWristFingerPage property page

IMPLEMENT_DYNCREATE(CWristFingerPage, CPropertyPage)

CWristFingerPage::CWristFingerPage() : CPropertyPage(CWristFingerPage::IDD)
{
	//{{AFX_DATA_INIT(CWristFingerPage)
	//}}AFX_DATA_INIT
	APP App = (APP) AfxGetApp();

	switch(App->m_nSide0Orientation) {
	case 0: m_nLeft = WRISTFINGERPORTSIDE;
		m_nRight = WRISTFINGERSTARBOARD;
		break;
	case 1: m_nLeft = WRISTFINGERSTARBOARD;
		m_nRight = WRISTFINGERPORTSIDE;
		break;
	}
}

CWristFingerPage::~CWristFingerPage()
{
}

void CWristFingerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWristFingerPage)
	DDX_Control(pDX, IDC_EDIT_Y_OFFSET_RIGHT, m_editYOffsetRight);
	DDX_Control(pDX, IDC_EDIT_Y_OFFSET_LEFT, m_editYOffsetLeft);
	DDX_Control(pDX, IDC_EDIT_X_RIGHT_OFFSET, m_editRightXOffset);
	DDX_Control(pDX, IDC_EDIT_MIRROR_NOZZLE_RIGHT, m_editMirrorNozzleRight);
	DDX_Control(pDX, IDC_EDIT_INT_WP_RIGHT, m_editInternalWaterPathRight);
	DDX_Control(pDX, IDC_EDIT_MIRROR_NOZZLE_LEFT, m_editMirrorNozzleLeft);
	DDX_Control(pDX, IDC_EDIT_INT_WP_LEFT, m_editInternalWaterPathLeft);
	DDX_Control(pDX, IDC_EDIT_X_LEFT_OFFSET, m_editLeftXOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWristFingerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CWristFingerPage)
	ON_EN_CHANGE(IDC_EDIT_X_LEFT_OFFSET, OnChangeEditXLeftOffset)
	ON_EN_CHANGE(IDC_EDIT_INT_WP_LEFT, OnChangeEditIntWpLeft)
	ON_EN_CHANGE(IDC_EDIT_MIRROR_NOZZLE_LEFT, OnChangeEditMirrorNozzleLeft)
	ON_EN_CHANGE(IDC_EDIT_X_RIGHT_OFFSET, OnChangeEditXRightOffset)
	ON_EN_CHANGE(IDC_EDIT_MIRROR_NOZZLE_RIGHT, OnChangeEditMirrorNozzleRight)
	ON_EN_CHANGE(IDC_EDIT_INT_WP_RIGHT, OnChangeEditIntWpRight)
	ON_EN_CHANGE(IDC_EDIT_Y_OFFSET_LEFT, OnChangeEditYOffsetLeft)
	ON_EN_CHANGE(IDC_EDIT_Y_OFFSET_RIGHT, OnChangeEditYOffsetRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWristFingerPage message handlers

BOOL CWristFingerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CWristFingerPage::OnSetActive() 
{
	APP App = (APP) AfxGetApp();
	
	App->m_LastSettings.nLastCompensationToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();

	return CPropertyPage::OnSetActive();
}

void CWristFingerPage::UpdateAllControls()
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	Buff.Format(_T("%.02f mm"),App->m_Kinematics.m_fXOffset[m_nLeft]);
	m_editLeftXOffset.SetWindowText(Buff);
	Buff.Format(_T("%.02f mm"),App->m_Kinematics.m_fYOffset[m_nLeft]);
	m_editYOffsetLeft.SetWindowText(Buff);

	Buff.Format(_T("%.02f mm"),App->m_Kinematics.m_fInternalWaterPath[m_nLeft]);
	m_editInternalWaterPathLeft.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),App->m_Kinematics.m_fMirrorNozzle[m_nLeft]);
	m_editMirrorNozzleLeft.SetWindowText(Buff);

	Buff.Format(_T("%.02f mm"),App->m_Kinematics.m_fXOffset[m_nRight]);
	m_editRightXOffset.SetWindowText(Buff);
	Buff.Format(_T("%.02f mm"),App->m_Kinematics.m_fYOffset[m_nRight]);
	m_editYOffsetRight.SetWindowText(Buff);

	Buff.Format(_T("%.02f mm"),App->m_Kinematics.m_fInternalWaterPath[m_nRight]);
	m_editInternalWaterPathRight.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),App->m_Kinematics.m_fMirrorNozzle[m_nRight]);
	m_editMirrorNozzleRight.SetWindowText(Buff);
}

void CWristFingerPage::OnChangeEditLeftArmLength() 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	float fTemp = App->m_Kinematics.m_fArmLength[m_nLeft];

	m_editLeftArmLength.GetWindowText(Buff);
	_WTOF(Buff,App->m_Kinematics.m_fArmLength[m_nLeft]);
	if(fTemp - App->m_Kinematics.m_fArmLength[m_nLeft]) {
		App->SaveTank();
	}

}


void CWristFingerPage::OnChangeEditXLeftOffset() 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	float fTemp = App->m_Kinematics.m_fXOffset[m_nLeft];

	m_editLeftXOffset.GetWindowText(Buff);
	_WTOF(Buff,App->m_Kinematics.m_fXOffset[m_nLeft]);
	if(fTemp - App->m_Kinematics.m_fXOffset[m_nLeft]) {
		App->SaveTank();
	}
}

void CWristFingerPage::OnChangeEditIntWpLeft() 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	float fTemp = App->m_Kinematics.m_fInternalWaterPath[m_nLeft];

	m_editInternalWaterPathLeft.GetWindowText(Buff);
	_WTOF(Buff,App->m_Kinematics.m_fInternalWaterPath[m_nLeft]);
	if(fTemp - App->m_Kinematics.m_fInternalWaterPath[m_nLeft]) {
		App->SaveTank();
	}
}

void CWristFingerPage::OnChangeEditMirrorNozzleLeft() 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	float fTemp = App->m_Kinematics.m_fMirrorNozzle[m_nLeft];

	m_editMirrorNozzleLeft.GetWindowText(Buff);
	_WTOF(Buff,App->m_Kinematics.m_fMirrorNozzle[m_nLeft]);
	if(fTemp - App->m_Kinematics.m_fMirrorNozzle[m_nLeft]) {
		App->SaveTank();
	}
}

void CWristFingerPage::OnChangeEditXRightOffset() 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	float fTemp = App->m_Kinematics.m_fXOffset[m_nRight];

	m_editRightXOffset.GetWindowText(Buff);
	_WTOF(Buff,App->m_Kinematics.m_fXOffset[m_nRight]);
	if(fTemp - App->m_Kinematics.m_fXOffset[m_nRight]) {
		App->SaveTank();
	}
}

void CWristFingerPage::OnChangeEditRightArmLength() 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	float fTemp = App->m_Kinematics.m_fArmLength[m_nRight];

	m_editRightArmLength.GetWindowText(Buff);
	_WTOF(Buff,App->m_Kinematics.m_fArmLength[m_nRight]);
	if(fTemp - App->m_Kinematics.m_fArmLength[m_nRight]) {
		App->SaveTank();
	}
}

void CWristFingerPage::OnChangeEditMirrorNozzleRight() 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	float fTemp = App->m_Kinematics.m_fMirrorNozzle[m_nRight];

	m_editMirrorNozzleRight.GetWindowText(Buff);
	_WTOF(Buff,App->m_Kinematics.m_fMirrorNozzle[m_nRight]);
	if(fTemp - App->m_Kinematics.m_fMirrorNozzle[m_nRight]) {
		App->SaveTank();
	}
}

void CWristFingerPage::OnChangeEditIntWpRight() 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	float fTemp = App->m_Kinematics.m_fInternalWaterPath[m_nRight];

	m_editInternalWaterPathRight.GetWindowText(Buff);
	_WTOF(Buff,App->m_Kinematics.m_fInternalWaterPath[m_nRight]);
	if(fTemp - App->m_Kinematics.m_fInternalWaterPath[m_nRight]) {
		App->SaveTank();
	}
}

void CWristFingerPage::OnChangeEditYOffsetLeft() 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	float fTemp = App->m_Kinematics.m_fYOffset[m_nLeft];

	m_editYOffsetLeft.GetWindowText(Buff);
	_WTOF(Buff,App->m_Kinematics.m_fYOffset[m_nLeft]);
	if(fTemp - App->m_Kinematics.m_fYOffset[m_nLeft]) {
		App->SaveTank();
	}
}

void CWristFingerPage::OnChangeEditYOffsetRight() 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	float fTemp = App->m_Kinematics.m_fYOffset[m_nRight];

	m_editYOffsetRight.GetWindowText(Buff);
	_WTOF(Buff,App->m_Kinematics.m_fYOffset[m_nRight]);
	if(fTemp - App->m_Kinematics.m_fYOffset[m_nRight]) {
		App->SaveTank();
	}
	
}
