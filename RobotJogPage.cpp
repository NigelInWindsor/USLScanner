// RobotJogPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotJogPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotJogPage property page

IMPLEMENT_DYNCREATE(CRobotJogPage, CPropertyPage)

CRobotJogPage::CRobotJogPage() : CPropertyPage(CRobotJogPage::IDD)
{
	//{{AFX_DATA_INIT(CRobotJogPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nRobot = 0;
	m_bLBDown = false;
	m_fReach = 1000.0f;
}

CRobotJogPage::~CRobotJogPage()
{
}

void CRobotJogPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotJogPage)
	DDX_Control(pDX, IDC_EDIT_ORTHOGONAL_SPEED, m_editOrthogonalSpeed);
	DDX_Control(pDX, IDC_SPIN_ORTHOGONAL_SPEED, m_spinOrthogonalSpeed);
	DDX_Control(pDX, IDC_SPIN_SCAN_SPEED, m_spinScanSpeed);
	DDX_Control(pDX, IDC_SPIN_MOVETO_SPEED, m_spinMoveToSpeed);
	DDX_Control(pDX, IDC_SPIN_JOG_SPEED, m_spinJogSpeed);
	DDX_Control(pDX, IDC_EDIT_SCAN_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_EDIT_MOVETO_SPEED, m_editMoveToSpeed);
	DDX_Control(pDX, IDC_EDIT_JOG_SPEED, m_editJogSpeed);
	DDX_Control(pDX, IDC_EDIT_Z_RIGHT_JOG, m_editZRight);
	DDX_Control(pDX, IDC_EDIT_Z_LEFT, m_editZLeft);
	DDX_Control(pDX, IDC_EDIT_Y_RIGHT_JOG, m_editYRight);
	DDX_Control(pDX, IDC_EDIT_X_RIGHT_JOG, m_editXRight);
	DDX_Control(pDX, IDC_EDIT_V_RIGHT_JOG, m_editVRight);
	DDX_Control(pDX, IDC_EDIT_T_RIGHT_JOG, m_editTRight);
	DDX_Control(pDX, IDC_EDIT_V_LEFT, m_editVLeft);
	DDX_Control(pDX, IDC_EDIT_T_LEFT, m_editTLeft);
	DDX_Control(pDX, IDC_EDIT_H_RIGHT_JOG, m_editHRight);
	DDX_Control(pDX, IDC_EDIT_H_LEFT, m_editHLeft);
	DDX_Control(pDX, IDC_EDIT_Y_LEFT, m_editYLeft);
	DDX_Control(pDX, IDC_EDIT_X_LEFT, m_editXLeft);
	DDX_Control(pDX, IDC_CHECK_DISPLAY, m_checkMakeVisible);
	DDX_Control(pDX, IDC_EDIT_SCAN_SIZE, m_editScanSize);
	DDX_Control(pDX, IDC_SPIN_REACH, m_spinReach);
	DDX_Control(pDX, IDC_EDIT_REACH, m_editReach);
	DDX_Control(pDX, IDC_SLIDER_J6, m_sliderJ6);
	DDX_Control(pDX, IDC_SLIDER_J5, m_sliderJ5);
	DDX_Control(pDX, IDC_SLIDER_J4, m_sliderJ4);
	DDX_Control(pDX, IDC_SLIDER_J3, m_sliderJ3);
	DDX_Control(pDX, IDC_SLIDER_J2, m_sliderJ2);
	DDX_Control(pDX, IDC_SLIDER_J1, m_sliderJ1);
	DDX_Control(pDX, IDC_BUTTON_POS_5, m_buttonPos5);
	DDX_Control(pDX, IDC_BUTTON_POS_4, m_buttonPos4);
	DDX_Control(pDX, IDC_BUTTON_POS_3, m_buttonPos3);
	DDX_Control(pDX, IDC_BUTTON_POS_2, m_buttonPos2);
	DDX_Control(pDX, IDC_BUTTON_POS_1, m_buttonPos1);
	DDX_Control(pDX, IDC_BUTTON_POS_0, m_buttonPos0);
	DDX_Control(pDX, IDC_BUTTON_NEG_5, m_butttonNeg5);
	DDX_Control(pDX, IDC_BUTTON_NEG_4, m_buttonNeg4);
	DDX_Control(pDX, IDC_BUTTON_NEG_3, m_buttonNeg3);
	DDX_Control(pDX, IDC_BUTTON_NEG_2, m_buttonNeg2);
	DDX_Control(pDX, IDC_BUTTON_NEG_1, m_buttonNeg1);
	DDX_Control(pDX, IDC_BUTTON_NEG_0, m_buttonNeg0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRobotJogPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRobotJogPage)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_ROBOT_0, OnButtonRobot0)
	ON_COMMAND(ID_BUTTON_ROBOT_1, OnButtonRobot1)
	ON_COMMAND(ID_BUTTON_JOINT, OnButtonJoint)
	ON_COMMAND(ID_BUTTON_TOOL, OnButtonTool)
	ON_COMMAND(ID_BUTTON_FRAME, OnButtonFrame)
	ON_COMMAND(ID_BUTTON_JOYSTICK, OnButtonJoystick)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_REACH, OnDeltaposSpinReach)
	ON_EN_CHANGE(IDC_EDIT_REACH, OnChangeEditReach)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY, OnCheckDisplay)
	ON_COMMAND(ID_BUTTON_PARK, OnButtonPark)
	ON_EN_CHANGE(IDC_EDIT_X_LEFT, OnChangeEditXLeft)
	ON_EN_CHANGE(IDC_EDIT_Y_LEFT, OnChangeEditYLeft)
	ON_EN_CHANGE(IDC_EDIT_Z_LEFT, OnChangeEditZLeft)
	ON_EN_CHANGE(IDC_EDIT_H_LEFT, OnChangeEditHLeft)
	ON_EN_CHANGE(IDC_EDIT_V_LEFT, OnChangeEditVLeft)
	ON_EN_CHANGE(IDC_EDIT_T_LEFT, OnChangeEditTLeft)
	ON_EN_CHANGE(IDC_EDIT_X_RIGHT_JOG, OnChangeEditXRightJog)
	ON_EN_CHANGE(IDC_EDIT_Y_RIGHT_JOG, OnChangeEditYRightJog)
	ON_EN_CHANGE(IDC_EDIT_Z_RIGHT_JOG, OnChangeEditZRightJog)
	ON_EN_CHANGE(IDC_EDIT_H_RIGHT_JOG, OnChangeEditHRightJog)
	ON_EN_CHANGE(IDC_EDIT_V_RIGHT_JOG, OnChangeEditVRightJog)
	ON_EN_CHANGE(IDC_EDIT_T_RIGHT_JOG, OnChangeEditTRightJog)
	ON_COMMAND(ID_BUTTON_MOVETO_ZERO, OnButtonMovetoZero)
	ON_EN_CHANGE(IDC_EDIT_JOG_SPEED, OnChangeEditJogSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_JOG_SPEED, OnDeltaposSpinJogSpeed)
	ON_EN_CHANGE(IDC_EDIT_MOVETO_SPEED, OnChangeEditMovetoSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVETO_SPEED, OnDeltaposSpinMovetoSpeed)
	ON_EN_CHANGE(IDC_EDIT_SCAN_SPEED, OnChangeEditScanSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCAN_SPEED, OnDeltaposSpinScanSpeed)
	ON_EN_CHANGE(IDC_EDIT_ORTHOGONAL_SPEED, OnChangeEditOrthogonalSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ORTHOGONAL_SPEED, OnDeltaposSpinOrthogonalSpeed)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotJogPage message handlers

BOOL CRobotJogPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_sliderJ1.SetRange(0,600);
	m_sliderJ1.SetPos(300);
	m_sliderJ2.SetRange(0,600);
	m_sliderJ2.SetPos(300);
	m_sliderJ3.SetRange(0,600);
	m_sliderJ3.SetPos(300);
	m_sliderJ4.SetRange(0,600);
	m_sliderJ4.SetPos(300);
	m_sliderJ5.SetRange(0,600);
	m_sliderJ5.SetPos(300);
	m_sliderJ6.SetRange(0,600);
	m_sliderJ6.SetPos(300);

	m_spinScanSpeed.SetRange(0,10000);
	m_spinMoveToSpeed.SetRange(0,10000);
	m_spinJogSpeed.SetRange(0,10000);
	m_spinOrthogonalSpeed.SetRange(0,10000);

	UpdateAllControls();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRobotJogPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

BOOL CRobotJogPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastRobotTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();
	
	return CPropertyPage::OnSetActive();
}

BOOL CRobotJogPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

void CRobotJogPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	CString Buff;

	Buff.Format(L"%.1f mm",m_fReach);
	m_editReach.SetWindowText(Buff);
	
	Buff.Format(L"%0.01f",theApp.m_Robot[0].m_fJogDirSpeedX);		m_editXLeft.SetWindowText(Buff);
	Buff.Format(L"%0.01f",theApp.m_Robot[0].m_fJogDirSpeedY);		m_editYLeft.SetWindowText(Buff);
	Buff.Format(L"%0.01f",theApp.m_Robot[0].m_fJogDirSpeedZ);		m_editZLeft.SetWindowText(Buff);
	Buff.Format(L"%0.03f",theApp.m_Robot[0].m_fJogDirSpeedHTip);	m_editHLeft.SetWindowText(Buff);
	Buff.Format(L"%0.03f", theApp.m_Robot[0].m_fJogDirSpeedVTip);	m_editVLeft.SetWindowText(Buff);
	Buff.Format(L"%0.03f", theApp.m_Robot[0].m_fJogDirSpeedTTip);	m_editTLeft.SetWindowText(Buff);

	Buff.Format(L"%0.01f",theApp.m_Robot[1].m_fJogDirSpeedX);		m_editXRight.SetWindowText(Buff);
	Buff.Format(L"%0.01f",theApp.m_Robot[1].m_fJogDirSpeedY);		m_editYRight.SetWindowText(Buff);
	Buff.Format(L"%0.01f",theApp.m_Robot[1].m_fJogDirSpeedZ);		m_editZRight.SetWindowText(Buff);
	Buff.Format(L"%0.03f",theApp.m_Robot[1].m_fJogDirSpeedHTip);	m_editHRight.SetWindowText(Buff);
	Buff.Format(L"%0.03f", theApp.m_Robot[1].m_fJogDirSpeedVTip);	m_editVRight.SetWindowText(Buff);
	Buff.Format(L"%0.03f", theApp.m_Robot[1].m_fJogDirSpeedTTip);	m_editTRight.SetWindowText(Buff);

	if(theApp.m_nUnitsPrecision<1) theApp.m_nUnitsPrecision = 1;
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,PROFILE->m_fScanSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editScanSpeed.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,theApp.m_LastSettings.fJogSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editJogSpeed.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,theApp.m_LastSettings.fMoveSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editMoveToSpeed.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,theApp.m_LastSettings.fPeekOrthogonalSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editOrthogonalSpeed.SetWindowText(Buff);


	SetToolBarCheckedState();
}

BOOL CRobotJogPage::PreTranslateMessage(MSG* pMsg) 
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	int nLeftAxis = -1;
	int nRightAxis = -1;
	float fDistance = 0.0f;

	switch(pMsg->message) {
	case 0x100:
		switch(pMsg->wParam) {
		case CURSOR_DOWN:
			break;
		case CURSOR_UP:
			break;
		}
		break;
	case 0x200: // Mouse move
		if(m_bLBDown == true) {
			switch(theApp.m_LastSettings.nRobotJogMode) {
			case 0: JointJog(m_bLBDown, point);
				break;
			case 1: FrameJog(m_bLBDown, point);
				break;
			case 2: FrameJog(m_bLBDown, point);
				break;
			}
		};
		break;
	case 0x201://LBUTTONDOWN
		if(m_bLBDown == false) {
			switch(theApp.m_LastSettings.nRobotJogMode) {
			case 0: JointJog(m_bLBDown = true, point);
				break;
			case 1: FrameJog(m_bLBDown = true, point);
				break;
			case 2: FrameJog(m_bLBDown = true, point);
				break;
			}
		};
		break;

	case 0x202:	//LBUTTONUP
		if(m_bLBDown == true) {
			switch(theApp.m_LastSettings.nRobotJogMode) {
			case 0: JointJog(m_bLBDown = false, point);
				break;
			case 1: FrameJog(m_bLBDown = false, point);
				break;
			case 2: FrameJog(m_bLBDown = false, point);
				break;
			}
		};

		break;
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

int CRobotJogPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_ROBOT_JOG_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	
	return 0;
}

BOOL CRobotJogPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
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
	case ID_BUTTON_ROBOT_0: strTipText.LoadString(IDS_Master_robot);
	   break;
	case ID_BUTTON_ROBOT_1: strTipText.LoadString(IDS_Slave_robot);
		break;
	case ID_BUTTON_JOINT: strTipText.LoadString(IDS_Joint);
		break;
	case ID_BUTTON_TOOL: strTipText.LoadString(IDS_Tool);
		break;
	case ID_BUTTON_FRAME: strTipText.LoadString(IDS_Frame);
		break;
	case ID_BUTTON_JOYSTICK: strTipText.LoadString(IDS_Enable_Disable);
		break;
	}
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void CRobotJogPage::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_ROBOT_0))>=0)
		m_nRobot==0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_ROBOT_1))>=0)
		m_nRobot==1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_JOINT))>=0)
		theApp.m_LastSettings.nRobotJogMode==0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_TOOL))>=0)
		theApp.m_LastSettings.nRobotJogMode==1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_FRAME))>=0)
		theApp.m_LastSettings.nRobotJogMode==2 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_JOYSTICK))>=0)
		theApp.m_FBCtrl.bJoystickEnabled==true ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

}

void CRobotJogPage::OnButtonRobot0() 
{
	m_nRobot = 0;
	UpdateAllControls();
}

void CRobotJogPage::OnButtonRobot1() 
{
	m_nRobot = 1;
	UpdateAllControls();
}

void CRobotJogPage::OnButtonJoint() 
{
	theApp.m_LastSettings.nRobotJogMode = 0;
	UpdateAllControls();
	JoystickEnableDisable();
}

void CRobotJogPage::OnButtonTool() 
{
	theApp.m_LastSettings.nRobotJogMode = 1;
	UpdateAllControls();
	JoystickEnableDisable();
}

void CRobotJogPage::OnButtonFrame() 
{
	theApp.m_LastSettings.nRobotJogMode = 2;
	UpdateAllControls();
	JoystickEnableDisable();
}

void CRobotJogPage::OnButtonJoystick() 
{
	theApp.m_FBCtrl.bJoystickEnabled == false ? theApp.m_FBCtrl.bJoystickEnabled = true : theApp.m_FBCtrl.bJoystickEnabled = false;
	UpdateAllControls();
	JoystickEnableDisable();
}

void CRobotJogPage::JoystickEnableDisable()
{
	switch(theApp.m_FBCtrl.bJoystickEnabled) {
	case false:	theApp.m_FBCtrl.SendStr(L"qa");
		theApp.m_FBCtrl.SendStr(L"#1k#2k#3k#4k#5k#6k");
		break;
	case true:
		switch(theApp.m_LastSettings.nRobotJogMode) {
		case 0:
			theApp.m_FBCtrl.EnableJoystick();
			break;
		case 1:
			break;
		case 2:
			break;
		}
		break;
	}
}

void CRobotJogPage::JointJog(bool bLButtonDn, CPoint point)
{
	CButton *pButton;
	CRect rr;

	if(bLButtonDn == true) {
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_0); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#1j-");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_0); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#1j+");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_1); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#2j-");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_1); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#2j+");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_2); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#3j-");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_2); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#3j+");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_3); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#4j-");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_3); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#4j+");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_4); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#5j-");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_4); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#5j+");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_5); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#6j-");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_5); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"#6j+");
		}
	} else {
		theApp.m_FBCtrl.SendStr(L"#1j/#2j/#3j/#4j/#5j/#6j/");
	}
}

void CRobotJogPage::FrameJog(bool bLButtonDn, CPoint point)
{
	CButton *pButton;
	CSliderCtrl *pSlider;
	CRect rr;
	int nPos;
	CString Buff;

	if(bLButtonDn == true) {
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_0); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"XJogMove=-100");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_0); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"XJogMove=+100");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_1); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"YJogMove=-100");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_1); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"YJogMove=100");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_2); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"ZJogMove=-100");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_2); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_FBCtrl.SendStr(L"ZJogMove=100");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_3); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
//			theApp.m_FBCtrl.SendStr(L"#4j-");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_3); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
//			theApp.m_FBCtrl.SendStr(L"#4j+");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_4); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
//			theApp.m_FBCtrl.SendStr(L"#5j-");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_4); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
//			theApp.m_FBCtrl.SendStr(L"#5j+");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEG_5); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
//			theApp.m_FBCtrl.SendStr(L"#6j-");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_POS_5); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
//			theApp.m_FBCtrl.SendStr(L"#6j+");
		}

		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_J1); pSlider->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			nPos = MulDiv(100,point.x-rr.CenterPoint().x,rr.Width()/2);
			Buff.Format(L"XJogMove=%d",nPos);
			theApp.m_FBCtrl.SendStr(Buff);
		}
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_J2); pSlider->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			nPos = MulDiv(100,point.x-rr.CenterPoint().x,rr.Width()/2);
			Buff.Format(L"YJogMove=%d",nPos);
			theApp.m_FBCtrl.SendStr(Buff);
		}
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_J3); pSlider->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			nPos = MulDiv(100,point.x-rr.CenterPoint().x,rr.Width()/2);
			Buff.Format(L"ZJogMove=%d",nPos);
			theApp.m_FBCtrl.SendStr(Buff);
		}
	
	
	} else {
		theApp.m_FBCtrl.SendStr(L"XJogMove=0");
		theApp.m_FBCtrl.SendStr(L"YJogMove=0");
		theApp.m_FBCtrl.SendStr(L"ZJogMove=0");
		theApp.m_FBCtrl.SendStr(L"RXJogMove=0");
		theApp.m_FBCtrl.SendStr(L"RYJogMove=0");
		theApp.m_FBCtrl.SendStr(L"RZJogMove=0");
		theApp.m_FBCtrl.SendStr(L"AJogMove=0");
		theApp.m_FBCtrl.SendStr(L"BJogMove=0");
		theApp.m_FBCtrl.SendStr(L"CJogMove=0");
		theApp.m_FBCtrl.SendStr(L"UJogMove=0");
		m_sliderJ1.SetPos(300);
		m_sliderJ2.SetPos(300);
		m_sliderJ3.SetPos(300);
		m_sliderJ4.SetPos(300);
		m_sliderJ5.SetPos(300);
		m_sliderJ6.SetPos(300);
	}
}

void CRobotJogPage::OnDeltaposSpinReach(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;

	m_fReach -= ((float)pNMUpDown->iDelta * 20.0f);
	if(m_fReach<0.0f) m_fReach = 0.0f;
	if(m_fReach>4000.0f) m_fReach = 4000.0f;
	UpdateAllControls();
	theApp.m_Robot[0].GetScanSize(&m_fReach,&Buff);
	m_editScanSize.SetWindowText(Buff);
	
	*pResult = 0;
}

void CRobotJogPage::OnChangeEditReach() 
{
	CString Buff;
	float fTemp = m_fReach;

	m_editReach.GetWindowText(Buff);
	_WTOF(Buff,m_fReach);
	if(fTemp - m_fReach) {
		theApp.m_Robot[0].GetScanSize(&m_fReach,&Buff);
		m_editScanSize.SetWindowText(Buff);
	}
}

void CRobotJogPage::OnCheckDisplay() 
{
	theApp.m_LastSettings.nDisplayRobotScanEnvelope = m_checkMakeVisible.GetCheck();
}


void CRobotJogPage::OnCheckReverseX() 
{
	// TODO: Add your control notification handler code here
	
}

void CRobotJogPage::OnChangeEditXLeft() 
{
	CString Buff;

	m_editXLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[0].m_fJogDirSpeedX);
}

void CRobotJogPage::OnChangeEditYLeft() 
{
	CString Buff;

	m_editYLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[0].m_fJogDirSpeedY);
}

void CRobotJogPage::OnChangeEditZLeft() 
{
	CString Buff;

	m_editZLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[0].m_fJogDirSpeedZ);
}

void CRobotJogPage::OnChangeEditHLeft() 
{
	CString Buff;

	m_editHLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[0].m_fJogDirSpeedHTip);
}

void CRobotJogPage::OnChangeEditVLeft()
{
	CString Buff;

	m_editVLeft.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_Robot[0].m_fJogDirSpeedVTip);
}

void CRobotJogPage::OnChangeEditTLeft()
{
	CString Buff;

	m_editTLeft.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_Robot[0].m_fJogDirSpeedTTip);
}

void CRobotJogPage::OnChangeEditXRightJog() 
{
	CString Buff;

	m_editXRight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[1].m_fJogDirSpeedX);
}

void CRobotJogPage::OnChangeEditYRightJog() 
{
	CString Buff;

	m_editYRight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[1].m_fJogDirSpeedY);
}

void CRobotJogPage::OnChangeEditZRightJog() 
{
	CString Buff;

	m_editZRight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[1].m_fJogDirSpeedZ);
}

void CRobotJogPage::OnChangeEditHRightJog() 
{
	CString Buff;

	m_editHRight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Robot[1].m_fJogDirSpeedHTip);
}

void CRobotJogPage::OnChangeEditVRightJog()
{
	CString Buff;

	m_editVRight.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_Robot[1].m_fJogDirSpeedVTip);
}

void CRobotJogPage::OnChangeEditTRightJog()
{
	CString Buff;

	m_editTRight.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_Robot[1].m_fJogDirSpeedTTip);
}

void CRobotJogPage::OnButtonPark() 
{
	CCoord	CpSurface;
	CPolyCoord Line;

	theApp.m_Thread.StopCurrentThreadAction();

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		CpSurface.Side0.pt = D3DXVECTOR3(-100.0f,0.0f,0.0f);
		CpSurface.Side1.pt = D3DXVECTOR3(100.0f,0.0f,0.0f);
		CpSurface.Side0.norm = D3DXVECTOR4(-1.0f,0.0f,0.0f,0.0f);
		CpSurface.Side1.norm = D3DXVECTOR4(1.0f,0.0f,0.0f,0.0f);
		break;
	case DUAL_ROBOT_9_PLUS_9:
		CpSurface.Side0.pt = D3DXVECTOR3(-100.0f,0.0f,0.0f);
		CpSurface.Side1.pt = D3DXVECTOR3(100.0f,0.0f,0.0f);
		CpSurface.Side0.norm = D3DXVECTOR4(-1.0f,0.0f,0.0f,0.0f);
		CpSurface.Side1.norm = D3DXVECTOR4(1.0f,0.0f,0.0f,0.0f);
		break;
	}
	Line.Add(CpSurface);
	theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
	
}

void CRobotJogPage::OnButtonMovetoZero() 
{
	CCoord	CpSurface;
	CPolyCoord Line;

	CpSurface.Side0.pt = D3DXVECTOR3(0.0f,0.0f,0.0f);
	CpSurface.Side1.pt = D3DXVECTOR3(0.0f,0.0f,0.0f);
	CpSurface.Side0.norm = D3DXVECTOR4(-1.0f,0.0f,0.0f,1.0f);
	CpSurface.Side1.norm = D3DXVECTOR4(-1.0f,0.0f,0.0f,1.0f);

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = CpSurface;
	theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;

//	Line.AddCoord(CpSurface);
//	theApp.m_FBCtrl.DoPolyLine(&Line);
	
}

void CRobotJogPage::OnChangeEditJogSpeed() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fJogSpeed;

	m_editJogSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	fTemp/=theApp.m_fUnits;
	MinMax(&fTemp, 1.0f, 1000.0f);

	if(theApp.m_LastSettings.fJogSpeed - fTemp) {
		theApp.m_LastSettings.fJogSpeed = fTemp;
	}
}

void CRobotJogPage::OnDeltaposSpinJogSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_LastSettings.fJogSpeed += ((float)pNMUpDown->iDelta);
		MinMax(&theApp.m_LastSettings.fJogSpeed, 1.0f, 1000.0f);
	} else {
		theApp.m_LastSettings.fJogSpeed += ((float)pNMUpDown->iDelta * 1.016f);
		MinMax(&theApp.m_LastSettings.fJogSpeed, 25.4f, 1016.0f);
	}
	UpdateAllControls();

	*pResult = 0;
}

void CRobotJogPage::OnChangeEditMovetoSpeed() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fMoveSpeed;

	m_editMoveToSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	fTemp/=theApp.m_fUnits;
	MinMax(&fTemp, 1.0f, 1000.0f);

	if(theApp.m_LastSettings.fMoveSpeed - fTemp) {
		theApp.m_LastSettings.fMoveSpeed = fTemp;
	}

}

void CRobotJogPage::OnDeltaposSpinMovetoSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_LastSettings.fMoveSpeed += ((float)pNMUpDown->iDelta);
		MinMax(&theApp.m_LastSettings.fMoveSpeed, 1.0f, 1000.0f);
	} else {
		theApp.m_LastSettings.fMoveSpeed += ((float)pNMUpDown->iDelta * 1.016f);
		MinMax(&theApp.m_LastSettings.fMoveSpeed, 25.4f, 1016.0f);
	}
	UpdateAllControls();

	*pResult = 0;
}

void CRobotJogPage::OnChangeEditScanSpeed() 
{
	CString Buff;
	float fTemp = PROFILE->m_fScanSpeed;

	m_editScanSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	fTemp/=theApp.m_fUnits;
	MinMax(&fTemp, 1.0f, 1000.0f);

	if(PROFILE->m_fScanSpeed - fTemp) {
		PROFILE->m_fScanSpeed = fTemp;
	}

}

void CRobotJogPage::OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta);
		MinMax(&PROFILE->m_fScanSpeed, 1.0f, 1000.0f);
	} else {
		PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * 1.016f);
		MinMax(&PROFILE->m_fScanSpeed, 25.4f, 1016.0f);
	}
	UpdateAllControls();

	*pResult = 0;
}


void CRobotJogPage::OnChangeEditOrthogonalSpeed() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fMoveSpeed;

	m_editOrthogonalSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	fTemp/=theApp.m_fUnits;
	MinMax(&fTemp, 1.0f, 1000.0f);

	if(theApp.m_LastSettings.fPeekOrthogonalSpeed - fTemp) {
		theApp.m_LastSettings.fPeekOrthogonalSpeed = fTemp;
	}
}

void CRobotJogPage::OnDeltaposSpinOrthogonalSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_LastSettings.fPeekOrthogonalSpeed += ((float)pNMUpDown->iDelta);
		MinMax(&theApp.m_LastSettings.fPeekOrthogonalSpeed, 1.0f, 1000.0f);
	} else {
		theApp.m_LastSettings.fPeekOrthogonalSpeed += ((float)pNMUpDown->iDelta * 1.016f);
		MinMax(&theApp.m_LastSettings.fPeekOrthogonalSpeed, 25.4f, 1016.0f);
	}
	UpdateAllControls();

	*pResult = 0;
}


