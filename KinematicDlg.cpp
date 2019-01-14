// KinematicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "KinematicDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKinematicDlg dialog

IMPLEMENT_DYNCREATE(CKinematicDlg, CPropertyPage)

CKinematicDlg::CKinematicDlg() : CPropertyPage(CKinematicDlg::IDD)
{
	//{{AFX_DATA_INIT(CKinematicDlg)
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

CKinematicDlg::~CKinematicDlg()
{
}



void CKinematicDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKinematicDlg)
	DDX_Control(pDX, IDC_EDIT_MIRROR_NOZZLE_RIGHT, m_editMirrorNozzleRight);
	DDX_Control(pDX, IDC_EDIT_MIRROR_NOZZLE_LEFT, m_editMirrorNozzleLeft);
	DDX_Control(pDX, IDC_CHECK_YT_LIMITSWITCH_RIGHT, m_checkYTLswRight);
	DDX_Control(pDX, IDC_CHECK_YT_LIMITSWITCH_LEFT, m_checkYTLswLeft);
	DDX_Control(pDX, IDC_CHECK_YT_KINEMATIC_RIGHT, m_checkYTKinematicRight);
	DDX_Control(pDX, IDC_CHECK_YT_KINEMATIC_LEFT, m_checkYTKinematicLeft);
	DDX_Control(pDX, IDC_CHECK_XT_LIMITSWITCH_RIGHT, m_checkXTLswRight);
	DDX_Control(pDX, IDC_CHECK_XT_LIMITSWITCH_LEFT, m_checkXTLswLeft);
	DDX_Control(pDX, IDC_CHECK_XT_KINEMATIC_RIGHT, m_checkXTKinematicRight);
	DDX_Control(pDX, IDC_CHECK_XT_KINEMATIC_LEFT, m_checkXTKinematicLeft);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	DDX_Control(pDX, IDC_CHECK_INVERT_XTIP, m_checkInvertXTip);
	DDX_Control(pDX, IDC_CHECK_INVERT_YTIP, m_checkInvertYTip);
	DDX_Control(pDX, IDC_COMBO_YTIP_ROTATE_AXIS, m_comboYTipRotateAxis);
	DDX_Control(pDX, IDC_COMBO_XTIP_ROTATE_AXIS, m_comboXTipRotateAxis);
	DDX_Control(pDX, IDC_CHECK_ENABLE_LEFT, m_checkEnableLeft);
	DDX_Control(pDX, IDC_CHECK_ENABLE_RIGHT, m_checkEnableRight);
	DDX_Control(pDX, IDC_COMBO_RIGHT_MODEL, m_comboRightModel);
	DDX_Control(pDX, IDC_COMBO_LEFT_MODEL, m_comboLeftModel);
	DDX_Control(pDX, IDC_EDIT_RIGHT_ARM_LENGTH, m_editRightArmLength);
	DDX_Control(pDX, IDC_EDIT_LEFT_ARM_LENGTH, m_editLeftArmLength);
	DDX_Control(pDX, IDC_EDIT_Z_RIGHT_OFFSET, m_editRightZOffset);
	DDX_Control(pDX, IDC_EDIT_Z_LEFT_OFFSET, m_editLeftZOffset);
	DDX_Control(pDX, IDC_EDIT_Y_RIGHT_OFFSET, m_editRightYOffset);
	DDX_Control(pDX, IDC_EDIT_Y_LEFT_OFFSET, m_editLeftYOffset);
	DDX_Control(pDX, IDC_EDIT_X_RIGHT_OFFSET, m_editRightXOffset);
	DDX_Control(pDX, IDC_EDIT_X_LEFT_OFFSET, m_editLeftXOffset);
	DDX_Control(pDX, IDC_EDIT_RIGHT_PROBE_LENGTH, m_editRightProbeLength);
	DDX_Control(pDX, IDC_EDIT_LEFT_PROBE_LENGTH, m_editLeftProbeLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKinematicDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CKinematicDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_ENABLE_RIGHT, OnButtonEnableRight)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_LEFT, OnButtonEnableLeft)
	ON_EN_CHANGE(IDC_EDIT_LEFT_PROBE_LENGTH, OnChangeEditLeftProbeLength)
	ON_EN_CHANGE(IDC_EDIT_RIGHT_PROBE_LENGTH, OnChangeEditRightProbeLength)
	ON_EN_CHANGE(IDC_EDIT_X_LEFT_OFFSET, OnChangeEditXLeftOffset)
	ON_EN_CHANGE(IDC_EDIT_X_RIGHT_OFFSET, OnChangeEditXRightOffset)
	ON_EN_CHANGE(IDC_EDIT_Y_LEFT_OFFSET, OnChangeEditYLeftOffset)
	ON_EN_CHANGE(IDC_EDIT_Y_RIGHT_OFFSET, OnChangeEditYRightOffset)
	ON_EN_CHANGE(IDC_EDIT_Z_LEFT_OFFSET, OnChangeEditZLeftOffset)
	ON_EN_CHANGE(IDC_EDIT_Z_RIGHT_OFFSET, OnChangeEditZRightOffset)
	ON_EN_CHANGE(IDC_EDIT_RIGHT_ARM_LENGTH, OnChangeEditRightArmLength)
	ON_EN_CHANGE(IDC_EDIT_LEFT_ARM_LENGTH, OnChangeEditLeftArmLength)
	ON_CBN_SELCHANGE(IDC_COMBO_LEFT_MODEL, OnSelchangeComboLeftModel)
	ON_CBN_SELCHANGE(IDC_COMBO_RIGHT_MODEL, OnSelchangeComboRightModel)
	ON_CBN_SELCHANGE(IDC_COMBO_XTIP_ROTATE_AXIS, OnSelchangeComboXtipRotateAxis)
	ON_CBN_SELCHANGE(IDC_COMBO_YTIP_ROTATE_AXIS, OnSelchangeComboYtipRotateAxis)
	ON_BN_CLICKED(IDC_CHECK_INVERT_XTIP, OnCheckInvertXtip)
	ON_BN_CLICKED(IDC_CHECK_INVERT_YTIP, OnCheckInvertYtip)
	ON_BN_CLICKED(IDC_CHECK_XT_KINEMATIC_LEFT, OnCheckXtKinematicLeft)
	ON_BN_CLICKED(IDC_CHECK_XT_KINEMATIC_RIGHT, OnCheckXtKinematicRight)
	ON_BN_CLICKED(IDC_CHECK_XT_LIMITSWITCH_LEFT, OnCheckXtLimitswitchLeft)
	ON_BN_CLICKED(IDC_CHECK_XT_LIMITSWITCH_RIGHT, OnCheckXtLimitswitchRight)
	ON_BN_CLICKED(IDC_CHECK_YT_KINEMATIC_LEFT, OnCheckYtKinematicLeft)
	ON_BN_CLICKED(IDC_CHECK_YT_KINEMATIC_RIGHT, OnCheckYtKinematicRight)
	ON_BN_CLICKED(IDC_CHECK_YT_LIMITSWITCH_LEFT, OnCheckYtLimitswitchLeft)
	ON_BN_CLICKED(IDC_CHECK_YT_LIMITSWITCH_RIGHT, OnCheckYtLimitswitchRight)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LEFT, OnButtonSaveLeft)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_RIGHT, OnButtonSaveRight)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_LEFT, OnButtonOpenLeft)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RIGHT, OnButtonOpenRight)
	ON_EN_CHANGE(IDC_EDIT_MIRROR_NOZZLE_RIGHT, OnChangeEditMirrorNozzleRight)
	ON_EN_CHANGE(IDC_EDIT_MIRROR_NOZZLE_LEFT, OnChangeEditMirrorNozzleLeft)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKinematicDlg message handlers


BOOL CKinematicDlg::OnInitDialog() 
{
	CString	Buff;
	CPropertyPage::OnInitDialog();

	switch (theApp.m_nSide0Orientation) {
	case 0:
		m_staticLeft.SetWindowText(theApp.m_DlgSideName[0]);
		m_staticRight.SetWindowText(theApp.m_DlgSideName[1]);
		break;
	case 1:
		m_staticLeft.SetWindowText(theApp.m_DlgSideName[1]);
		m_staticRight.SetWindowText(theApp.m_DlgSideName[0]);
		break;
	}

	if (CKinematicDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CKinematicDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}
	
	Buff.LoadString(IDS_Horizontal_Wrist);			m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Vertical_Wrist);			m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Vertical_Wrist_90);			m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Index_Finger);				m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_ForePlane_Scanner);			m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Index_Finger_2);			m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_GOODRICH_MACHINE_4);		m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Rotate_Diff_Theta);			m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_FOREPLANE_VERTICAL);		m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_DOWTY_MANIP);				m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Rotate_Theta_Horizontal);	m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Rotate_Theta_Vertical);		m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Arcadia_2);					m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_SWIVEL_Z_OFFSET_GIMBAL);	m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Swivel_Gimbal);				m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_TWI);						m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Rotate_Swivel_Gimbal);		m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Twin_tower_finger);			m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);
	Buff.LoadString(IDS_Vertical_wrist_PPMAC);		m_comboLeftModel.AddString(Buff);		m_comboRightModel.AddString(Buff);


	m_comboXTipRotateAxis.AddString(_T("Rotate about Z"));
	m_comboXTipRotateAxis.AddString(_T("Rotate about X"));
	m_comboXTipRotateAxis.AddString(_T("Rotate about Y"));

	m_comboYTipRotateAxis.AddString(_T("Rotate about Z"));
	m_comboYTipRotateAxis.AddString(_T("Rotate about X"));
	m_comboYTipRotateAxis.AddString(_T("Rotate about Y"));

	

	UpdateAllEditControls();

	SetAccessPrivelage();
	EnableToolTips(TRUE);

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CKinematicDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CKinematicDlg::UpdateAllEditControls()
{
	CString Buff;
	int nMask;

	if (GetSafeHwnd() == NULL) return;

	Buff.Format(_T("%.02f mm"),theApp.m_Kinematics.m_fXOffset[m_nLeft]);
	m_editLeftXOffset.SetWindowText(Buff);
	Buff.Format(_T("%.02f mm"),theApp.m_Kinematics.m_fYOffset[m_nLeft]);
	m_editLeftYOffset.SetWindowText(Buff);
	Buff.Format(_T("%.02f mm"),theApp.m_Kinematics.m_fZOffset[m_nLeft]);
	m_editLeftZOffset.SetWindowText(Buff);
	Buff.Format(_T("%.02f mm"),theApp.m_Kinematics.m_fArmLength[m_nLeft]);
	m_editLeftArmLength.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),theApp.m_Kinematics.m_fProbeMirror[m_nLeft]);
	m_editMirrorNozzleLeft.SetWindowText(Buff);


	Buff.Format(_T("%.02f mm"),theApp.m_Kinematics.m_fXOffset[m_nRight]);
	m_editRightXOffset.SetWindowText(Buff);
	Buff.Format(_T("%.02f mm"),theApp.m_Kinematics.m_fYOffset[m_nRight]);
	m_editRightYOffset.SetWindowText(Buff);
	Buff.Format(_T("%.02f mm"),theApp.m_Kinematics.m_fZOffset[m_nRight]);
	m_editRightZOffset.SetWindowText(Buff);
	Buff.Format(_T("%.02f mm"),theApp.m_Kinematics.m_fArmLength[m_nRight]);
	m_editRightArmLength.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),theApp.m_Kinematics.m_fProbeMirror[m_nRight]);
	m_editMirrorNozzleRight.SetWindowText(Buff);


	Buff.Format(_T("%.02f mm"),theApp.m_Kinematics.m_fProbeLength[m_nLeft]);
	m_editLeftProbeLength.SetWindowText(Buff);
	Buff.Format(_T("%.02f mm"),theApp.m_Kinematics.m_fProbeLength[m_nRight]);
	m_editRightProbeLength.SetWindowText(Buff);
	m_editLeftProbeLength.EnableWindow(TRUE);
	m_editRightProbeLength.EnableWindow(TRUE);

	m_comboLeftModel.SetCurSel(theApp.m_LastSettings.nManipulatorType[m_nLeft]);
	m_comboRightModel.SetCurSel(theApp.m_LastSettings.nManipulatorType[m_nRight]);

	m_checkEnableLeft.SetCheck(theApp.m_LastSettings.bEnableSide[m_nLeft] & 1);
	m_checkEnableRight.SetCheck(theApp.m_LastSettings.bEnableSide[m_nRight] & 1);

	m_comboXTipRotateAxis.SetCurSel(theApp.m_Kinematics.m_nXtipRotateAxis);
	m_comboYTipRotateAxis.SetCurSel(theApp.m_Kinematics.m_nYtipRotateAxis);

	m_checkInvertXTip.SetCheck(theApp.m_Kinematics.m_nInvertXtip);
	m_checkInvertYTip.SetCheck(theApp.m_Kinematics.m_nInvertYtip);

	switch(theApp.m_nSide0Orientation) {
	case 0:
		nMask = 1;
		nMask <<= theApp.m_Tank.nXtLeft;
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask & nMask ? m_checkXTLswLeft.SetCheck(true) : m_checkXTLswLeft.SetCheck(false);
		theApp.m_Kinematics.nKinematicOffsetLoadMask & nMask ? m_checkXTKinematicLeft.SetCheck(true) : m_checkXTKinematicLeft.SetCheck(false);
		nMask = 1;
		nMask <<= theApp.m_Tank.nXtRight;
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask & nMask ? m_checkXTLswRight.SetCheck(true) : m_checkXTLswRight.SetCheck(false);
		theApp.m_Kinematics.nKinematicOffsetLoadMask & nMask ? m_checkXTKinematicRight.SetCheck(true) : m_checkXTKinematicRight.SetCheck(false);
		nMask = 1;
		nMask <<= theApp.m_Tank.nYtLeft;
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask & nMask ? m_checkYTLswLeft.SetCheck(true) : m_checkYTLswLeft.SetCheck(false);
		theApp.m_Kinematics.nKinematicOffsetLoadMask & nMask ? m_checkYTKinematicLeft.SetCheck(true) : m_checkYTKinematicLeft.SetCheck(false);
		nMask = 1;
		nMask <<= theApp.m_Tank.nYtRight;
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask & nMask ? m_checkYTLswRight.SetCheck(true) : m_checkYTLswRight.SetCheck(false);
		theApp.m_Kinematics.nKinematicOffsetLoadMask & nMask ? m_checkYTKinematicRight.SetCheck(true) : m_checkYTKinematicRight.SetCheck(false);
		break;
	case 1:
		nMask = 1;
		nMask <<= theApp.m_Tank.nXtRight;
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask & nMask ? m_checkXTLswLeft.SetCheck(true) : m_checkXTLswLeft.SetCheck(false);
		theApp.m_Kinematics.nKinematicOffsetLoadMask & nMask ? m_checkXTKinematicLeft.SetCheck(true) : m_checkXTKinematicLeft.SetCheck(false);
		nMask = 1;
		nMask <<= theApp.m_Tank.nXLeft;
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask & nMask ? m_checkXTLswRight.SetCheck(true) : m_checkXTLswRight.SetCheck(false);
		theApp.m_Kinematics.nKinematicOffsetLoadMask & nMask ? m_checkXTKinematicRight.SetCheck(true) : m_checkXTKinematicRight.SetCheck(false);
		nMask = 1;
		nMask <<= theApp.m_Tank.nYtRight;
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask & nMask ? m_checkYTLswLeft.SetCheck(true) : m_checkYTLswLeft.SetCheck(false);
		theApp.m_Kinematics.nKinematicOffsetLoadMask & nMask ? m_checkYTKinematicLeft.SetCheck(true) : m_checkYTKinematicLeft.SetCheck(false);
		nMask = 1;
		nMask <<= theApp.m_Tank.nYtLeft;
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask & nMask ? m_checkYTLswRight.SetCheck(true) : m_checkYTLswRight.SetCheck(false);
		theApp.m_Kinematics.nKinematicOffsetLoadMask & nMask ? m_checkYTKinematicRight.SetCheck(true) : m_checkYTKinematicRight.SetCheck(false);
		break;
	}
}

void CKinematicDlg::OnChangeEditLeftProbeLength() 
{
	CString	Buff;
	float fTemp = theApp.m_Kinematics.m_fProbeLength[m_nLeft];

	m_editLeftProbeLength.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fProbeLength[m_nLeft]);
	if(fTemp - theApp.m_Kinematics.m_fProbeLength[m_nLeft]) {
		theApp.SaveTank();
	}

}


void CKinematicDlg::OnChangeEditRightProbeLength() 
{
	CString	Buff;
	float fTemp = theApp.m_Kinematics.m_fProbeLength[m_nRight];

	m_editRightProbeLength.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fProbeLength[m_nRight]);
	if(fTemp - theApp.m_Kinematics.m_fProbeLength[m_nRight]) {
		theApp.SaveTank();
	}
}


void CKinematicDlg::OnChangeEditXLeftOffset() 
{
	CString	Buff;
	float fTemp = theApp.m_Kinematics.m_fXOffset[m_nLeft];

	m_editLeftXOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fXOffset[m_nLeft]);
	if(fTemp - theApp.m_Kinematics.m_fXOffset[m_nLeft]) {
		theApp.SaveTank();
	}
}

void CKinematicDlg::OnChangeEditXRightOffset() 
{
	CString	Buff;
	float fTemp = theApp.m_Kinematics.m_fXOffset[m_nRight];

	m_editRightXOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fXOffset[m_nRight]);
	if(fTemp - theApp.m_Kinematics.m_fXOffset[m_nRight]) {
		theApp.SaveTank();
	}
}

void CKinematicDlg::OnChangeEditYLeftOffset() 
{
	CString	Buff;
	m_editLeftYOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fYOffset[m_nLeft]);
}

void CKinematicDlg::OnChangeEditYRightOffset() 
{
	CString	Buff;
	m_editRightYOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fYOffset[m_nRight]);
}

void CKinematicDlg::OnChangeEditZLeftOffset() 
{
	CString	Buff;
	m_editLeftZOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fZOffset[m_nLeft]);
}

void CKinematicDlg::OnChangeEditZRightOffset() 
{
	CString	Buff;
	m_editRightZOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fZOffset[m_nRight]);
}

void CKinematicDlg::OnChangeEditRightArmLength() 
{
	CString	Buff;
	float fTemp = theApp.m_Kinematics.m_fArmLength[m_nRight];

	m_editRightArmLength.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fArmLength[m_nRight]);
	if(fTemp - theApp.m_Kinematics.m_fArmLength[m_nRight]) {
		theApp.SaveTank();
	}
}

void CKinematicDlg::OnChangeEditLeftArmLength() 
{
	CString	Buff;
	float fTemp = theApp.m_Kinematics.m_fArmLength[m_nLeft];

	m_editLeftArmLength.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fArmLength[m_nLeft]);
	if(fTemp - theApp.m_Kinematics.m_fArmLength[m_nLeft]) {
		theApp.SaveTank();
	}
}


void CKinematicDlg::OnChangeEditComponentThickness() 
{
	CString	Buff;
	m_editComponentThickness.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fComponentThickness);
	
}


void CKinematicDlg::SetAccessPrivelage()
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case FB_TWIN_TOWER:
		m_editLeftProbeLength.SetReadOnly(true);
		m_editRightProbeLength.SetReadOnly(true);
		m_comboLeftModel.EnableWindow(false);
		break;
	}

}


BOOL CKinematicDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case IDC_EDIT_RIGHT_ARM_LENGTH:
	case IDC_EDIT_LEFT_ARM_LENGTH:
	case IDC_EDIT_RIGHT_PROBE_LENGTH:
	case IDC_EDIT_LEFT_PROBE_LENGTH:  strTipText = "Always positive, whether left or right";
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


BOOL CKinematicDlg::OnSetActive() 
{
	theApp.m_LastSettings.nLastCompensationToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllEditControls();
	return CPropertyPage::OnSetActive();
}

void CKinematicDlg::OnSelchangeComboLeftModel()
{
	theApp.m_LastSettings.nManipulatorType[m_nLeft] = m_comboLeftModel.GetCurSel();
}

void CKinematicDlg::OnSelchangeComboRightModel()
{
	theApp.m_LastSettings.nManipulatorType[m_nRight] = m_comboRightModel.GetCurSel();
}

void CKinematicDlg::OnButtonEnableLeft()
{
	theApp.m_LastSettings.bEnableSide[m_nLeft] = m_checkEnableLeft.GetCheck() & 1;
}

void CKinematicDlg::OnButtonEnableRight()
{
	theApp.m_LastSettings.bEnableSide[m_nRight] = m_checkEnableRight.GetCheck() & 1;
}

void CKinematicDlg::OnSelchangeComboXtipRotateAxis() 
{
	theApp.m_Kinematics.m_nXtipRotateAxis = m_comboXTipRotateAxis.GetCurSel();
}

void CKinematicDlg::OnSelchangeComboYtipRotateAxis() 
{
	theApp.m_Kinematics.m_nYtipRotateAxis = m_comboYTipRotateAxis.GetCurSel();
}

void CKinematicDlg::OnCheckInvertXtip() 
{
	theApp.m_Kinematics.m_nInvertXtip = m_checkInvertXTip.GetCheck();
}

void CKinematicDlg::OnCheckInvertYtip() 
{
	theApp.m_Kinematics.m_nInvertYtip = m_checkInvertYTip.GetCheck();
}

void CKinematicDlg::OnCheckXtKinematicLeft() 
{
	int nMask = 1;
	switch(theApp.m_nSide0Orientation) {
	case 0:	nMask <<= theApp.m_Tank.nXtLeft;
		break;
	case 1:	nMask <<= theApp.m_Tank.nXtRight;
		break;
	}

	if(m_checkXTKinematicLeft.GetCheck() & 1) {
		theApp.m_Kinematics.nKinematicOffsetLoadMask |= nMask;
	} else {
		theApp.m_Kinematics.nKinematicOffsetLoadMask &= ~nMask;
	}
}

void CKinematicDlg::OnCheckXtKinematicRight() 
{
	int nMask = 1;
	switch(theApp.m_nSide0Orientation) {
	case 0:	nMask <<= theApp.m_Tank.nXtRight;
		break;
	case 1:	nMask <<= theApp.m_Tank.nXtLeft;
		break;
	}

	if(m_checkXTKinematicRight.GetCheck() & 1) {
		theApp.m_Kinematics.nKinematicOffsetLoadMask |= nMask;
	} else {
		theApp.m_Kinematics.nKinematicOffsetLoadMask &= ~nMask;
	}
}

void CKinematicDlg::OnCheckXtLimitswitchLeft() 
{
	int nMask = 1;
	switch(theApp.m_nSide0Orientation) {
	case 0:	nMask <<= theApp.m_Tank.nXtLeft;
		break;
	case 1:	nMask <<= theApp.m_Tank.nXtRight;
		break;
	}


	if(m_checkXTLswLeft.GetCheck() & 1) {
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask |= nMask;
	} else {
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask &= ~nMask;
	}
	
}

void CKinematicDlg::OnCheckXtLimitswitchRight() 
{
	int nMask = 1;
	switch(theApp.m_nSide0Orientation) {
	case 0:	nMask <<= theApp.m_Tank.nXtRight;
		break;
	case 1:	nMask <<= theApp.m_Tank.nXtLeft;
		break;
	}


	if(m_checkXTLswRight.GetCheck() & 1) {
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask |= nMask;
	} else {
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask &= ~nMask;
	}
}

void CKinematicDlg::OnCheckYtKinematicLeft() 
{
	int nMask = 1;
	switch(theApp.m_nSide0Orientation) {
	case 0:	nMask <<= theApp.m_Tank.nYtLeft;
		break;
	case 1:	nMask <<= theApp.m_Tank.nYtRight;
		break;
	}

	if(m_checkYTKinematicLeft.GetCheck() & 1) {
		theApp.m_Kinematics.nKinematicOffsetLoadMask |= nMask;
	} else {
		theApp.m_Kinematics.nKinematicOffsetLoadMask &= ~nMask;
	}
}

void CKinematicDlg::OnCheckYtKinematicRight() 
{
	int nMask = 1;
	switch(theApp.m_nSide0Orientation) {
	case 0:	nMask <<= theApp.m_Tank.nYtRight;
		break;
	case 1:	nMask <<= theApp.m_Tank.nYtLeft;
		break;
	}

	if(m_checkYTKinematicRight.GetCheck() & 1) {
		theApp.m_Kinematics.nKinematicOffsetLoadMask |= nMask;
	} else {
		theApp.m_Kinematics.nKinematicOffsetLoadMask &= ~nMask;
	}
}

void CKinematicDlg::OnCheckYtLimitswitchLeft() 
{
	int nMask = 1;
	switch(theApp.m_nSide0Orientation) {
	case 0:	nMask <<= theApp.m_Tank.nYtLeft;
		break;
	case 1:	nMask <<= theApp.m_Tank.nYtRight;
		break;
	}

	if(m_checkYTLswLeft.GetCheck() & 1) {
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask |= nMask;
	} else {
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask &= ~nMask;
	}
	
}

void CKinematicDlg::OnCheckYtLimitswitchRight() 
{
	int nMask = 1;
	switch(theApp.m_nSide0Orientation) {
	case 0:	nMask <<= theApp.m_Tank.nYtRight;
		break;
	case 1:	nMask <<= theApp.m_Tank.nYtLeft;
		break;
	}

	if(m_checkYTLswRight.GetCheck() & 1) {
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask |= nMask;
	} else {
		theApp.m_Kinematics.nLimitSwitchOffsetLoadMask &= ~nMask;
	}
}

void CKinematicDlg::OnButtonSaveLeft() 
{

	SaveManipulator(m_nLeft);

}

void CKinematicDlg::OnButtonSaveRight() 
{
	SaveManipulator(m_nRight);
}


bool CKinematicDlg::SaveManipulator(int nSide)
{
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;

	CFileDialog FileDlg(FALSE,_T(".manipulator"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Manipulator (*.manipulator)|*.manipulator|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
			return false;
		} else {
			theApp.m_Kinematics.SaveManipulator(&File, nSide);

			File.Close();
			return true;
		}
	}

	return false;
	
}




void CKinematicDlg::OnButtonOpenLeft() 
{
	RetrieveManipulator(m_nLeft);
	
}

void CKinematicDlg::OnButtonOpenRight() 
{
	RetrieveManipulator(m_nRight);
	
}

bool CKinematicDlg::RetrieveManipulator(int nSide)
{
	CString FilePath,Buff;
	CUSLFile File;
	CFileException e;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	CFileDialog FileDlg(TRUE,_T(".manipulator"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Manipulator (*.manipulator)|*.manipulator|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {

			theApp.m_Kinematics.RetrieveManipulator(&File, nSide);

			File.Close();

			theApp.m_LimitAndKinematicOffset = FileDlg.GetFileTitle();
			UpdateAllEditControls();
			pFrame->SendMessage(UI_UPDATE_AXES, NULL, NULL);
			theApp.SaveTank();
			theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		}
	}
	return true;
	

}

void CKinematicDlg::OnChangeEditMirrorNozzleLeft() 
{
	CString	Buff;
	float fTemp = theApp.m_Kinematics.m_fProbeMirror[m_nLeft];

	m_editMirrorNozzleLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fProbeMirror[m_nLeft]);
	if(fTemp - theApp.m_Kinematics.m_fProbeMirror[m_nLeft]) {
		theApp.SaveTank();
	}
}

void CKinematicDlg::OnChangeEditMirrorNozzleRight() 
{
	CString	Buff;
	float fTemp = theApp.m_Kinematics.m_fProbeMirror[m_nRight];

	m_editMirrorNozzleRight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Kinematics.m_fProbeMirror[m_nRight]);
	if(fTemp - theApp.m_Kinematics.m_fProbeMirror[m_nRight]) {
		theApp.SaveTank();
	}
}

