// AxisPowerPmacPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "AxisPowerPmacPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisPowerPmacPage property page

IMPLEMENT_DYNCREATE(CAxisPowerPmacPage, CPropertyPage)

CAxisPowerPmacPage::CAxisPowerPmacPage(int nAxis) : CPropertyPage(CAxisPowerPmacPage::IDD)
{
	//{{AFX_DATA_INIT(CAxisPowerPmacPage)
	//}}AFX_DATA_INIT

	m_nAxis = nAxis;
	m_fRelativeMove = 0.0f;

}

CAxisPowerPmacPage::~CAxisPowerPmacPage()
{
}

void CAxisPowerPmacPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxisPowerPmacPage)
	DDX_Control(pDX, IDC_EDIT_REL_MOVE, m_editRelativeMove);
	DDX_Control(pDX, IDC_SPIN_KINEMATIC_OFFSET, m_spinKinematicOffset);
	DDX_Control(pDX, IDC_EDIT_KINEMATIC_OFFSET, m_editKinematicOffset);
	DDX_Control(pDX, IDC_EDIT_HOME_POS, m_editHomePos);
	DDX_Control(pDX, IDC_EDIT_IN_POS_ERR, m_editInPosError);
	DDX_Control(pDX, IDC_EDIT_NUMBER_DP, m_editDisplayPrecision);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_MAX_SPEED, m_editMaxSpeed);
	DDX_Control(pDX, IDC_EDIT_JOYSTICK_NUMBER, m_editJoystickNumber);
	DDX_Control(pDX, IDC_EDIT_JOYSTICK_GAIN, m_editJogGain);
	DDX_Control(pDX, IDC_EDIT_JOG_ACCELERATION, m_editJogAcceleration);
	DDX_Control(pDX, IDC_EDIT_ACCELERATION, m_editAcceleration);
	DDX_Control(pDX, IDC_COMBO_TOGGLE_POS, m_comboTogglePos);
	DDX_Control(pDX, IDC_COMBO_PHYSICAL_AXIS, m_comboPhysicalAxis);
	DDX_Control(pDX, IDC_CHECK_JOYSTICK_REVERSE, m_checkJoystickReverse);
	DDX_Control(pDX, IDC_AXIS_UNITS_COMBO, m_comboUnits);
	DDX_Control(pDX, IDC_AXIS_STEPSIZE, m_editStepSize);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_ORIGIN_MODE, m_comboOriginMode);
}


BEGIN_MESSAGE_MAP(CAxisPowerPmacPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAxisPowerPmacPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ACCEPT_BUTTON, OnAcceptButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_HOME_POS, OnButtonHomePos)
	ON_EN_CHANGE(IDC_EDIT_KINEMATIC_OFFSET, OnChangeEditKinematicOffset)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_KINEMATIC_OFFSET, OnDeltaposSpinKinematicOffset)
	ON_BN_CLICKED(IDC_BUTTON_KINEMATIC_ODDSET, OnButtonKinematicOddset)
	ON_EN_CHANGE(IDC_EDIT_REL_MOVE, OnChangeEditRelMove)
	ON_BN_CLICKED(IDC_BUTTON_REL_NEG_MOVE, OnButtonRelNegMove)
	ON_BN_CLICKED(IDC_BUTTON_REL_MOVE, OnButtonRelMove)
	ON_BN_CLICKED(IDC_BUTTON_ABS_MOVE, OnButtonAbsMove)
	ON_CBN_SELCHANGE(IDC_COMBO_PHYSICAL_AXIS, OnSelchangeComboPhysicalAxis)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	//}}AFX_MSG_MAP
   ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
   ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_CBN_SELCHANGE(IDC_COMBO_ORIGIN_MODE, &CAxisPowerPmacPage::OnCbnSelchangeComboOriginMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisPowerPmacPage message handlers

BOOL CAxisPowerPmacPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_comboUnits.AddString(_T("mm"));
	m_comboUnits.AddString(_T("\x022"));
	m_comboUnits.AddString(DEGREES);

	m_comboTogglePos.AddString(_T("Up"));
	m_comboTogglePos.AddString(_T("Middle"));
	m_comboTogglePos.AddString(_T("Bottom"));

	Buff.LoadStringW(IDS_None);					m_comboOriginMode.AddString(Buff);
	Buff.LoadStringW(IDS_Origin_here);			m_comboOriginMode.AddString(Buff);
	Buff.LoadStringW(IDS_Limit_switch_seek);	m_comboOriginMode.AddString(Buff);

	m_spinKinematicOffset.SetRange(-20000,20000);
	m_spinKinematicOffset.SetPos(0);

	for(int ii=1;ii<17;ii++) {
		Buff.Format(_T("%d"),ii);
		m_comboPhysicalAxis.AddString(Buff);
	}
	
	
	UpdateAllControls();
	
	EnableToolTips();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAxisPowerPmacPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CAxisPowerPmacPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	CString Units[3]={L" mm", L"\x022", L"\x0b0"};

	m_comboUnits.SetCurSel(theApp.m_Axes[m_nAxis].nUnitSelected);

	theApp.m_Axes[m_nAxis].bJoystickReverse==TRUE ? m_checkJoystickReverse.SetCheck(1) : m_checkJoystickReverse.SetCheck(0);

	Buff.Format(L"%.03f",theApp.m_Axes[m_nAxis].fAcceleration);
	m_editAcceleration.SetWindowText(Buff);

	Buff.Format(L"%.03f",theApp.m_Axes[m_nAxis].fJoystickAcceleration);
	m_editJogAcceleration.SetWindowText(Buff);

	Buff.Format(L"%.02f %s/s",theApp.m_Axes[m_nAxis].fMaxMoveSpeed * 1000.0f * theApp.m_Axes[m_nAxis].fStepSize,Units[theApp.m_Axes[m_nAxis].nUnitSelected]);
	m_editMaxSpeed.SetWindowText(Buff);


	if(theApp.m_Axes[m_nAxis].fStepSize >= 0.001f) {
		Buff.Format(L"%.07f",theApp.m_Axes[m_nAxis].fStepSize);
	} else {
		Buff.Format(L"%11.05e",theApp.m_Axes[m_nAxis].fStepSize);
	}
	m_editStepSize.SetWindowText(Buff);


	Buff.Format(_T("%.02f%s"),(float)theApp.m_Axes[m_nAxis].nPositionError * theApp.m_Axes[m_nAxis].fStepSize,Units[theApp.m_Axes[m_nAxis].nUnitSelected]);
	m_editInPosError.SetWindowText(Buff);


	Buff.Format(L"%d",theApp.m_Axes[m_nAxis].nJoystickGain);
	m_editJogGain.SetWindowText(Buff);

	Buff.Format(L"%d",theApp.m_Axes[m_nAxis].nJoystickNumber+1);
	m_editJoystickNumber.SetWindowText(Buff);

	m_comboPhysicalAxis.SetCurSel(theApp.m_Axes[m_nAxis].nPhysicalAxis - 1);

	m_comboTogglePos.SetCurSel(theApp.m_Axes[m_nAxis].nJoystickSwitchPos);


	Buff.Format(L"%d",theApp.m_Axes[m_nAxis].nPrecision);
	m_editDisplayPrecision.SetWindowText(Buff);


	Buff.Format(L"%s",theApp.m_Axes[m_nAxis].cName);
	m_editName.SetWindowText(Buff);

	Buff.Format(L"%d",theApp.m_Axes[m_nAxis].nHomePos);
	m_editHomePos.SetWindowText(Buff);

	Buff.Format(_T("%d"),theApp.m_Axes[m_nAxis].nOffset);
	m_editKinematicOffset.SetWindowText(Buff);

	Buff.Format(_T("%.02f"),m_fRelativeMove);
	m_editRelativeMove.SetWindowText(Buff);

	m_comboOriginMode.SetCurSel(theApp.m_Axes[m_nAxis].nHomeMode);
}

LPTSTR CAxisPowerPmacPage::getTabName()
{

	return theApp.m_Axes[m_nAxis].cName;

}

BOOL CAxisPowerPmacPage::OnSetActive() 
{
	ReadPOWERPmac();
	m_nUpdateLoopCounter= 0;
	SetTimer(1,250, NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CAxisPowerPmacPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CAxisPowerPmacPage::OnAcceptButton() 
{
	CString	Buff;
	float fTemp;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	Sleep(10);

	m_editStepSize.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Axes[m_nAxis].fStepSize);
	if(theApp.m_Axes[m_nAxis].fStepSize == 0.0f) {
		theApp.m_Axes[m_nAxis].fStepSize = 0.0001f;
		m_editStepSize.SetWindowText(Buff);
	};

	if(m_checkJoystickReverse.GetCheck()&1) {
		theApp.m_Axes[m_nAxis].bJoystickReverse=TRUE;
	} else {
		theApp.m_Axes[m_nAxis].bJoystickReverse=FALSE;
	};


	m_editMaxSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	theApp.m_Axes[m_nAxis].fMaxMoveSpeed=theApp.m_Axes[m_nAxis].fMaxJogSpeed = (fTemp / 1000.0f) / theApp.m_Axes[m_nAxis].fStepSize;


	m_editJogAcceleration.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Axes[m_nAxis].fJoystickAcceleration);

	m_editAcceleration.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Axes[m_nAxis].fAcceleration);


	m_editInPosError.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	theApp.m_Axes[m_nAxis].nPositionError=(int)(fTemp / theApp.m_Axes[m_nAxis].fStepSize);


	m_editJoystickNumber.GetWindowText(Buff);
	theApp.m_Axes[m_nAxis].nJoystickNumber=_ttoi(Buff)-1;

	m_editJogGain.GetWindowText(Buff);
	theApp.m_Axes[m_nAxis].nJoystickGain=_ttoi(Buff);

	theApp.m_Axes[m_nAxis].nUnitSelected=m_comboUnits.GetCurSel();

	m_editDisplayPrecision.GetWindowText(Buff);
	theApp.m_Axes[m_nAxis].nPrecision = _ttoi(Buff);


	m_editHomePos.GetWindowText(Buff);
	theApp.m_FBCtrl.SetHomePos(m_nAxis, _ttoi(Buff));

	m_editKinematicOffset.GetWindowText(Buff);
	theApp.m_Axes[m_nAxis].nOffset = _ttoi(Buff);

	theApp.m_FBCtrl.DownloadAxisParameters(m_nAxis);


//	pSheet->UpdateStatusSheet();


	theApp.SaveTank();

	theApp.m_Thread.m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
}

void CAxisPowerPmacPage::ReadPOWERPmac() 
{
	CString RxStr,Buff;

	Buff.Format(L"Motor[%d].InvAmax",theApp.m_Axes[m_nAxis].nPhysicalAxis);
	theApp.m_FBCtrl.SendStr(Buff,_BACKGROUND);

	Buff.Format(L"Motor[%d].JogTa",theApp.m_Axes[m_nAxis].nPhysicalAxis);
	theApp.m_FBCtrl.SendStr(Buff,_BACKGROUND);

	Buff.Format(L"Motor[%d].MaxSpeed",theApp.m_Axes[m_nAxis].nPhysicalAxis);
	theApp.m_FBCtrl.SendStr(Buff,_BACKGROUND);

	Buff.Format(L"Motor[%d].InPosBand",theApp.m_Axes[m_nAxis].nPhysicalAxis);
	theApp.m_FBCtrl.SendStr(Buff,_BACKGROUND);

	Buff.Format(L"Motor[%d].HomePos",theApp.m_Axes[m_nAxis].nPhysicalAxis);
	theApp.m_FBCtrl.SendStr(Buff,_BACKGROUND);

}

void CAxisPowerPmacPage::OnTimer(UINT nIDEvent) 
{
	m_nUpdateLoopCounter++;
	if(m_nUpdateLoopCounter == 2) {
		UpdateAllControls();
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CAxisPowerPmacPage::OnButtonHomePos() 
{
	theApp.m_Axes[m_nAxis].nHomePos += theApp.m_Ethercat.EncoderPos(theApp.m_Axes[m_nAxis].nPhysicalAxis - 1);
	CString Buff;
	Buff.Format(L"%d", theApp.m_Axes[m_nAxis].nHomePos);
	m_editHomePos.SetWindowText(Buff);

	switch (theApp.m_Axes[m_nAxis].nHomeMode) {
	case 0:
		break;
	case 1:	theApp.m_FBCtrl.SetHomePosToEncoderPos(theApp.m_Axes[m_nAxis].nPhysicalAxis - 1);
		break;
	case 2:
		break;
	}

}

void CAxisPowerPmacPage::OnButtonKinematicOddset() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
	theApp.m_Axes[m_nAxis].nOffset = 0;
	Sleep(100);
	theApp.m_Axes[m_nAxis].nOffset = theApp.m_Axes[m_nAxis].nPos;

	UpdateAllControls();
	theApp.SaveTank();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
	
}

void CAxisPowerPmacPage::OnDeltaposSpinKinematicOffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	
	int nStep = pNMUpDown->iDelta * (int)(0.05f / theApp.m_Axes[m_nAxis].fStepSize);
	Buff.Format(_T("%d"),theApp.m_Axes[m_nAxis].nOffset += nStep);
	m_editKinematicOffset.SetWindowText(Buff);

	*pResult = 0;
}

void CAxisPowerPmacPage::OnChangeEditKinematicOffset() 
{
	CString Buff;
	
	m_editKinematicOffset.GetWindowText(Buff);
	theApp.m_Axes[m_nAxis].nOffset = _ttoi(Buff);
	
}

BOOL CAxisPowerPmacPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;

   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) || pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND)) {
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case IDC_EDIT_ACCELERATION: strTipText = "The larger the number the faster it accelerates";
	   break;
	}

	
	lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
	*pResult = 0;

   return TRUE;    // message was handled
}

void CAxisPowerPmacPage::OnChangeEditRelMove() 
{
	CString Buff;
	
	m_editRelativeMove.GetWindowText(Buff);
	_WTOF(Buff,m_fRelativeMove);
}

void CAxisPowerPmacPage::OnButtonRelNegMove() 
{
	int nRelativeMove = (int)(m_fRelativeMove / theApp.m_Axes[m_nAxis].fStepSize);
	theApp.m_Motors.RelativeMove(&theApp.m_Axes[m_nAxis],-nRelativeMove);
}

void CAxisPowerPmacPage::OnButtonRelMove() 
{
	int nRelativeMove = (int)(m_fRelativeMove / theApp.m_Axes[m_nAxis].fStepSize);
	theApp.m_Motors.RelativeMove(&theApp.m_Axes[m_nAxis],nRelativeMove);
}

void CAxisPowerPmacPage::OnButtonAbsMove() 
{
	int nAbsoluteMove = (int)(m_fRelativeMove / theApp.m_Axes[m_nAxis].fStepSize);
	theApp.m_Motors.AbsoluteMove(&theApp.m_Axes[m_nAxis],nAbsoluteMove);
}

void CAxisPowerPmacPage::OnSelchangeComboPhysicalAxis() 
{
	int nAxis = theApp.m_Axes[m_nAxis].nPhysicalAxis;

	theApp.m_Axes[m_nAxis].nPhysicalAxis=m_comboPhysicalAxis.GetCurSel() + 1;
	if(nAxis - theApp.m_Axes[m_nAxis].nPhysicalAxis) {
		UpdateAllControls();
	}
}

void CAxisPowerPmacPage::OnChangeEditName() 
{
	CString Buff;
	
	m_editName.GetWindowText(Buff);
	if(Buff.GetLength() > (AXISNAMELENGHT-1)) {
		Buff.GetBufferSetLength(AXISNAMELENGHT-1);
	}
	swprintf_s(theApp.m_Axes[m_nAxis].cName,10,L"%s",(LPCWSTR)Buff);
	
}


void CAxisPowerPmacPage::OnCbnSelchangeComboOriginMode()
{
	theApp.m_Axes[m_nAxis].nHomeMode = m_comboOriginMode.GetCurSel();
}
