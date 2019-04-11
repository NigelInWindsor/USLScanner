// JogPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "JogPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJogPage property page

IMPLEMENT_DYNCREATE(CJogPage, CPropertyPage)

CJogPage::CJogPage(int nID) : CPropertyPage(nID)
{
	//{{AFX_DATA_INIT(CJogPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nSide = 2;
	m_nMode = 0;
	m_nMainTip = 0;
	m_bLBDown = false;
	m_nID = nID;
}

CJogPage::~CJogPage()
{
}

void CJogPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJogPage)
	if (m_nID == IDD_JOG_PAGE) {
		DDX_Control(pDX, IDC_COMBO_MAIN_TIPS, m_comboMainTips);
		DDX_Control(pDX, IDC_COMBO_SIDE, m_comboSide);
		DDX_Control(pDX, IDC_COMBO_JOG_MODE, m_comboJogMode);
	}
	DDX_Control(pDX, IDC_EDIT_MINIMUM_WATER_PATH, m_editMinimumWaterPath);
	DDX_Control(pDX, IDC_SPIN_Z_JOG_DISTANCE, m_spinZJogDistance);
	DDX_Control(pDX, IDC_EDIT_Z_JOG_DISTANCE, m_editZJogDistance);
	DDX_Control(pDX, IDC_SPIN_Y_JOG_DISTANCE, m_spinYJogDistance);
	DDX_Control(pDX, IDC_SPIN_X_JOG_DISTANCE, m_spinXJogDistance);
	DDX_Control(pDX, IDC_EDIT_Y_JOG_DISTANCE, m_editYJogDistance);
	DDX_Control(pDX, IDC_EDIT_X_JOG_DISTANCE, m_editXJogDistance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJogPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_SIDE, OnSelchangeComboSide)
	ON_CBN_SELCHANGE(IDC_COMBO_JOG_MODE, OnSelchangeComboJogMode)
	ON_EN_CHANGE(IDC_EDIT_Z_JOG_DISTANCE, OnChangeEditZJogDistance)
	ON_CBN_SELCHANGE(IDC_COMBO_MAIN_TIPS, OnSelchangeComboMainTips)
	ON_EN_CHANGE(IDC_EDIT_Y_JOG_DISTANCE, OnChangeEditYJogDistance)
	ON_EN_CHANGE(IDC_EDIT_X_JOG_DISTANCE, OnChangeEditXJogDistance)
	ON_EN_CHANGE(IDC_EDIT_MINIMUM_WATER_PATH, OnChangeEditMinimumWaterPath)
	ON_BN_CLICKED(IDC_BUTTON_I_NEG, OnButtonINeg)
	ON_BN_CLICKED(IDC_BUTTON_I_POS, OnButtonIPos)
	ON_BN_CLICKED(IDC_BUTTON_J_NEG, OnButtonJNeg)
	ON_BN_CLICKED(IDC_BUTTON_J_POS, OnButtonJPos)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_BN_CLICKED(ID_BUTTON_LEFT_WHEEL, OnButtonLeftWheel)
	ON_BN_CLICKED(ID_BUTTON_RIGHT_WHEEL, OnButtonRightWheel)
	ON_BN_CLICKED(ID_BUTTON_BOTH_WHEELS, OnButtonBothWheels)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJogPage message handlers


BOOL CJogPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_spinXJogDistance.SetRange(0,1000);
	m_spinYJogDistance.SetRange(0,1000);
	m_spinZJogDistance.SetRange(0,1000);

	if (m_comboSide.GetSafeHwnd()) {
		switch (theApp.m_nSide0Orientation) {
		case 0:
			Buff.LoadString(IDS_Left);	m_comboSide.AddString(theApp.m_DlgSideName[0]);
			Buff.LoadString(IDS_Right);	m_comboSide.AddString(theApp.m_DlgSideName[1]);
			break;
		case 1:
			Buff.LoadString(IDS_Right);	m_comboSide.AddString(theApp.m_DlgSideName[1]);
			Buff.LoadString(IDS_Left);	m_comboSide.AddString(theApp.m_DlgSideName[0]);
			break;
		};
	}
	if (m_comboSide.GetSafeHwnd()) {
		Buff.LoadString(IDS_Both);	m_comboSide.AddString(Buff);
	}

	if (m_comboJogMode.GetSafeHwnd()) {
		Buff.LoadString(IDS_Step);			m_comboJogMode.AddString(Buff);
		Buff.LoadString(IDS_Continuous);	m_comboJogMode.AddString(Buff);
	}
	if (m_comboMainTips.GetSafeHwnd()) {
		m_comboMainTips.AddString(L"X Y");
		m_comboMainTips.AddString(L"Tips");
	}

	
	UpdateAllControls();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CJogPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

BOOL CJogPage::OnSetActive() 
{
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	m_nAntiDriftStoreTime = 0;

	SetTimer(1,500,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CJogPage::OnKillActive() 
{

	KillTimer(1);
	return CPropertyPage::OnKillActive();
}

void CJogPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	if(m_comboSide.GetSafeHwnd()) m_comboSide.SetCurSel(m_nSide);
	if (m_comboJogMode.GetSafeHwnd()) m_comboJogMode.SetCurSel(m_nMode);
	if (m_comboMainTips.GetSafeHwnd()) m_comboMainTips.SetCurSel(m_nMainTip);

	if(m_nMainTip == 0) {

		Buff.Format(L"%.01f mm",theApp.m_LastSettings.fXJogDistance);
		m_editXJogDistance.SetWindowText(Buff);
		Buff.Format(L"%.01f mm",theApp.m_LastSettings.fYJogDistance);
		m_editYJogDistance.SetWindowText(Buff);

		m_spinXJogDistance.SetPos((int)(theApp.m_LastSettings.fXJogDistance * 10.0f));
		m_spinYJogDistance.SetPos((int)(theApp.m_LastSettings.fYJogDistance * 10.0f));
	} else {
		Buff.Format(L"%.02f%s",theApp.m_LastSettings.fXtJogDistance,DEGREES);
		m_editXJogDistance.SetWindowText(Buff);
		Buff.Format(L"%.02f%s",theApp.m_LastSettings.fYtJogDistance,DEGREES);
		m_editYJogDistance.SetWindowText(Buff);

		m_spinXJogDistance.SetPos((int)(theApp.m_LastSettings.fXtJogDistance * 100.0f));
		m_spinYJogDistance.SetPos((int)(theApp.m_LastSettings.fYtJogDistance * 100.0f));
	}

	Buff.Format(L"%.01f mm",theApp.m_LastSettings.fZJogDistance);
	m_editZJogDistance.SetWindowText(Buff);
	m_spinZJogDistance.SetPos((int)(theApp.m_LastSettings.fZJogDistance * 10.0f));


	Buff.Format(L"%.1f mm",theApp.m_LastSettings.fMinimumWaterPathJog);
	m_editMinimumWaterPath.SetWindowText(Buff);

	SetToolBarCheckedState();
}

void CJogPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rr[4];
	CButton *pButton;

	if (theApp.m_Tank.nScannerDescription != RAILWAY_AXLE) {
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_X_NEGATIVE); pButton->GetWindowRect(rr[0]);
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_X_POSITIVE); pButton->GetWindowRect(rr[1]);
		ScreenToClient(&rr[0]);
		ScreenToClient(&rr[1]);

		dc.MoveTo(rr[0].right + 2, rr[0].CenterPoint().y);
		dc.LineTo(rr[1].left - 2, rr[0].CenterPoint().y);

		pButton = (CButton*)GetDlgItem(IDC_BUTTON_Y_NEGATIVE); pButton->GetWindowRect(rr[0]);
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_Y_POSITIVE); pButton->GetWindowRect(rr[1]);
		ScreenToClient(&rr[0]);
		ScreenToClient(&rr[1]);

		dc.MoveTo(rr[0].CenterPoint().x, rr[0].top - 2);
		dc.LineTo(rr[1].CenterPoint().x, rr[1].bottom - 2);

		pButton = (CButton*)GetDlgItem(IDC_BUTTON_Z_NEGATIVE); pButton->GetWindowRect(rr[0]);
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_Z_POSITIVE); pButton->GetWindowRect(rr[1]);
		ScreenToClient(&rr[0]);
		ScreenToClient(&rr[1]);

		dc.MoveTo(rr[0].CenterPoint().x, rr[0].top - 2);
		dc.LineTo(rr[1].CenterPoint().x, rr[1].bottom - 2);
	}
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CJogPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	float fTemp;
	CString Buff;
	CString Micro = L"\x0b5";

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_Z_JOG_DISTANCE:
		fTemp = theApp.m_LastSettings.fZJogDistance;
		theApp.m_LastSettings.fZJogDistance = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		if(fTemp - theApp.m_LastSettings.fZJogDistance) {
			UpdateAllControls();
		}
		break;
	case IDC_SPIN_Y_JOG_DISTANCE:
		if(m_nMainTip==0) {
			fTemp = theApp.m_LastSettings.fYJogDistance;
			theApp.m_LastSettings.fYJogDistance = (float)(pSpin->GetPos()&0xffff) / 10.0f;
			if(fTemp - theApp.m_LastSettings.fYJogDistance) {
				UpdateAllControls();
			}
		} else {
			fTemp = theApp.m_LastSettings.fYtJogDistance;
			theApp.m_LastSettings.fYtJogDistance = (float)(pSpin->GetPos()&0xffff) / 100.0f;
			if(fTemp - theApp.m_LastSettings.fYtJogDistance) {
				UpdateAllControls();
			}
		}
		break;
	case IDC_SPIN_X_JOG_DISTANCE:
		if(m_nMainTip==0) {
			fTemp = theApp.m_LastSettings.fXJogDistance;
			theApp.m_LastSettings.fXJogDistance = (float)(pSpin->GetPos()&0xffff) / 10.0f;
			if(fTemp - theApp.m_LastSettings.fXJogDistance) {
				UpdateAllControls();
			}
		} else {
			fTemp = theApp.m_LastSettings.fXtJogDistance;
			theApp.m_LastSettings.fXtJogDistance = (float)(pSpin->GetPos()&0xffff) / 100.0f;
			if(fTemp - theApp.m_LastSettings.fXtJogDistance) {
				UpdateAllControls();
			}
		}
		break;
	}

	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

#define YPRSpeed 1.0f

BOOL CJogPage::PreTranslateMessage(MSG* pMsg) 
{
	CString Buff;
	CButton *pButton;
	CRect rr;
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	int nLeftAxis = -1;
	int nRightAxis = -1;
	float fDistance = 0.0f;
	float fXDistance = 0.0f;
	float fYDistance = 0.0f;
	float fZDistance = 0.0f;
	float fXtDistance = 0.0f;
	float fYtDistance = 0.0f; 
	float fYawDistance = 0.0f;
	float fPitchDistance = 0.0f;
	float fRollDistance = 0.0f;

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
		switch(pMsg->wParam) {
		case 1:
			break;
		};
		break;
	case 0x201://LBUTTONDOWN
		if(m_bLBDown == false) {
			m_bLBDown = true;
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_X_NEGATIVE); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				if(m_nMainTip==0) {
					nLeftAxis = theApp.m_Tank.nXLeft;
					nRightAxis = theApp.m_Tank.nXRight;
					fXDistance = fDistance = theApp.m_LastSettings.fXJogDistance * -1.0f;
				} else {
					nLeftAxis = theApp.m_Tank.nXtLeft;
					nRightAxis = theApp.m_Tank.nXtRight;
					fXtDistance = fDistance = theApp.m_LastSettings.fXtJogDistance * -1.0f;
				};
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_X_POSITIVE); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				if(m_nMainTip==0) {
					nLeftAxis = theApp.m_Tank.nXLeft;
					nRightAxis = theApp.m_Tank.nXRight;
					fXDistance = fDistance = theApp.m_LastSettings.fXJogDistance;
				} else {
					nLeftAxis = theApp.m_Tank.nXtLeft;
					nRightAxis = theApp.m_Tank.nXtRight;
					fXtDistance = fDistance = theApp.m_LastSettings.fXtJogDistance;
				};
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_Y_NEGATIVE); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				if(m_nMainTip==0) {
					nLeftAxis = theApp.m_Tank.nYLeft;
					nRightAxis = theApp.m_Tank.nYRight;
					fYDistance = fDistance = theApp.m_LastSettings.fYJogDistance * -1.0f;
				} else {
					nLeftAxis = theApp.m_Tank.nYtLeft;
					nRightAxis = theApp.m_Tank.nYtRight;
					fYtDistance = fDistance = theApp.m_LastSettings.fYtJogDistance * -1.0f;
				};
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_Y_POSITIVE); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				if(m_nMainTip==0) {
					nLeftAxis = theApp.m_Tank.nYLeft;
					nRightAxis = theApp.m_Tank.nYRight;
					fYDistance = fDistance = theApp.m_LastSettings.fYJogDistance;
				} else {
					nLeftAxis = theApp.m_Tank.nYtLeft;
					nRightAxis = theApp.m_Tank.nYtRight;
					fYtDistance = fDistance = theApp.m_LastSettings.fYtJogDistance;
				};
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_Z_NEGATIVE); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				nLeftAxis = theApp.m_Tank.nZLeft;
				nRightAxis = theApp.m_Tank.nZRight;
				fZDistance = fDistance = theApp.m_LastSettings.fZJogDistance * -1.0f;
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_Z_POSITIVE); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				nLeftAxis = theApp.m_Tank.nZLeft;
				nRightAxis = theApp.m_Tank.nZRight;
				fZDistance = fDistance = theApp.m_LastSettings.fZJogDistance;
			}
		//TIP AXES
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_I_NEG); 
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fXtDistance = fDistance = -0.5f;
				}
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_I_POS);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fXtDistance = fDistance = 0.5f;
				}
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_J_NEG);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fYtDistance = fDistance = -0.5f;
				}
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_J_POS);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fYtDistance = fDistance = 0.5f;
				}
			}

			pButton = (CButton*)GetDlgItem(IDC_BUTTON_ROT_X_POS);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fXtDistance = fDistance = -0.5f;
				}
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_ROT_X_NEG);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fXtDistance = fDistance = 0.5f;
				}
			}

			pButton = (CButton*)GetDlgItem(IDC_BUTTON_YAW_NEGATIVE);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fYawDistance = fDistance = -YPRSpeed;
				}
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_YAW_POSITIVE);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fYawDistance = fDistance = YPRSpeed;
				}
			}

			pButton = (CButton*)GetDlgItem(IDC_BUTTON_PITCH_NEGATIVE);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fPitchDistance = fDistance = -YPRSpeed;
				}
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_PITCH_POSITIVE);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fPitchDistance = fDistance = YPRSpeed;
				}
			}

			pButton = (CButton*)GetDlgItem(IDC_BUTTON_ROLL_NEGATIVE);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fRollDistance = fDistance = -YPRSpeed;
				}
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_ROLL_POSITIVE);
			if (pButton && pButton->GetSafeHwnd()) {
				pButton->GetWindowRect(rr);	ScreenToClient(rr);
				if (rr.PtInRect(point)) {
					fRollDistance = fDistance = YPRSpeed;
				}
			}


			switch(theApp.m_Tank.nScannerDescription) {
			default:
				if((nLeftAxis != -1) && (nRightAxis != -1)) {
					switch(m_nSide) {
					case 0: theApp.m_PmacUser.JogMove(&theApp.m_Axes[nLeftAxis],fDistance,m_nMode);
						break;
					case 1: theApp.m_PmacUser.JogMove(&theApp.m_Axes[nRightAxis],fDistance,m_nMode);
						break;
					case 2: theApp.m_PmacUser.JogMove(&theApp.m_Axes[nLeftAxis],fDistance,m_nMode);
							theApp.m_PmacUser.JogMove(&theApp.m_Axes[nRightAxis],fDistance,m_nMode);
						break;
					};
				}
				break;
			case SINGLE_ROBOT:
			case DUAL_ROBOT:
			case DUAL_ROBOT_9_PLUS_9:
			case RAILWAY_AXLE:
			case TANK_2AXIS:
				if (m_nSide == 0 || m_nSide == 2) {
					Buff.Format(L"XJogMove=%.02f", fXDistance);				theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
					Buff.Format(L"YJogMove=%.02f", fYDistance);				theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
					Buff.Format(L"ZJogMove=%.02f", fZDistance);				theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
					Buff.Format(L"YawJogMove=%.05f", fYawDistance);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
					Buff.Format(L"PitchJogMove=%.05f", fPitchDistance);		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
					Buff.Format(L"RollJogMove=%.05f", fRollDistance);		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				}
				if (m_nSide == 1 || m_nSide == 2) {
					Buff.Format(L"B_XJogMove=%.02f", fXDistance);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
					Buff.Format(L"B_YJogMove=%.02f", fYDistance);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
					Buff.Format(L"B_ZJogMove=%.02f", fZDistance);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
					Buff.Format(L"B_YawJogMove=%.05f", fYawDistance);		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
					Buff.Format(L"B_PitchJogMove=%.05f", fPitchDistance);	theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
					Buff.Format(L"B_RollJogMove=%.05f", fRollDistance);		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				}
				break;
			}

		};
		break;

	case 0x202:	//LBUTTONUP
		if(m_bLBDown == true) {
			m_bLBDown = false;
			switch(theApp.m_Tank.nScannerDescription) {
			default:
				theApp.m_PmacUser.JogMove(&theApp.m_Axes[theApp.m_Tank.nXLeft],0.0f,m_nMode);
				theApp.m_PmacUser.JogMove(&theApp.m_Axes[theApp.m_Tank.nXRight],0.0f,m_nMode);
				theApp.m_PmacUser.JogMove(&theApp.m_Axes[theApp.m_Tank.nYLeft],0.0f,m_nMode);
				theApp.m_PmacUser.JogMove(&theApp.m_Axes[theApp.m_Tank.nYRight],0.0f,m_nMode);
				theApp.m_PmacUser.JogMove(&theApp.m_Axes[theApp.m_Tank.nZLeft],0.0f,m_nMode);
				theApp.m_PmacUser.JogMove(&theApp.m_Axes[theApp.m_Tank.nZRight],0.0f,m_nMode);
				theApp.m_PmacUser.JogMove(&theApp.m_Axes[theApp.m_Tank.nXtLeft],0.0f,m_nMode);
				theApp.m_PmacUser.JogMove(&theApp.m_Axes[theApp.m_Tank.nXtRight],0.0f,m_nMode);
				theApp.m_PmacUser.JogMove(&theApp.m_Axes[theApp.m_Tank.nYtLeft],0.0f,m_nMode);
				theApp.m_PmacUser.JogMove(&theApp.m_Axes[theApp.m_Tank.nYtRight],0.0f,m_nMode);
			case SINGLE_ROBOT:
			case DUAL_ROBOT:
			case DUAL_ROBOT_9_PLUS_9:
			case RAILWAY_AXLE:
				Buff.Format(L"XJogMove=%.0f", 0.0f);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"YJogMove=%.0f", 0.0f);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"ZJogMove=%.0f", 0.0f);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"YawJogMove=%.05f", 0.0f);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"PitchJogMove=%.05f", 0.0f);		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"RollJogMove=%.05f", 0.0f);		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"B_XJogMove=%.0f", 0.0f);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"B_YJogMove=%.0f", 0.0f);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"B_ZJogMove=%.0f", 0.0f);			theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"B_YawJogMove=%.05f", 0.0f);		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"B_PitchJogMove=%.05f", 0.0f);		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				Buff.Format(L"B_RollJogMove=%.05f", 0.0f);		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
				break;
			}
		};

		break;
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CJogPage::OnSelchangeComboSide() 
{

	m_nSide = m_comboSide.GetCurSel();
}

void CJogPage::OnSelchangeComboJogMode() 
{

	m_nMode = m_comboJogMode.GetCurSel();
}


void CJogPage::OnSelchangeComboMainTips() 
{

	m_nMainTip = m_comboMainTips.GetCurSel();
	UpdateAllControls();
}

void CJogPage::OnChangeEditZJogDistance() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fZJogDistance;

	m_editZJogDistance.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fZJogDistance);
	m_spinZJogDistance.SetPos((int)(theApp.m_LastSettings.fZJogDistance * 10.0f));
}

void CJogPage::OnChangeEditYJogDistance() 
{
	CString Buff;
	float fTemp;

	m_editYJogDistance.GetWindowText(Buff);
	if(m_nMainTip==0) {
		fTemp = theApp.m_LastSettings.fYJogDistance;
		_WTOF(Buff,theApp.m_LastSettings.fYJogDistance);
		m_spinYJogDistance.SetPos((int)(theApp.m_LastSettings.fYJogDistance * 10.0f));
	} else {
		fTemp = theApp.m_LastSettings.fYtJogDistance;
		_WTOF(Buff,theApp.m_LastSettings.fYtJogDistance);
		m_spinYJogDistance.SetPos((int)(theApp.m_LastSettings.fYtJogDistance * 100.0f));
	}
}

void CJogPage::OnChangeEditXJogDistance() 
{
	CString Buff;
	float fTemp;

	m_editXJogDistance.GetWindowText(Buff);
	if(m_nMainTip==0) {
		fTemp = theApp.m_LastSettings.fXJogDistance;
		_WTOF(Buff,theApp.m_LastSettings.fXJogDistance);
		m_spinXJogDistance.SetPos((int)(theApp.m_LastSettings.fXJogDistance * 10.0f));
	} else {
		fTemp = theApp.m_LastSettings.fXtJogDistance;
		_WTOF(Buff,theApp.m_LastSettings.fXtJogDistance);
		m_spinXJogDistance.SetPos((int)(theApp.m_LastSettings.fXtJogDistance * 100.0f));
	}
}

void CJogPage::OnChangeEditMinimumWaterPath() 
{
	CString Buff;

	m_editMinimumWaterPath.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fMinimumWaterPathJog);
}

void CJogPage::OnButtonINeg() 
{

	GoniometricMove('i',-0.1f);
}

void CJogPage::OnButtonIPos() 
{
	GoniometricMove('i',0.1f);
	
}

void CJogPage::OnButtonJNeg() 
{
	GoniometricMove('j',-0.1f);
}

void CJogPage::OnButtonJPos() 
{
	GoniometricMove('j',0.1f);
	
}

void CJogPage::GoniometricMove(char cVec, float fDist)
{
	CCoord CpSurface,CpHead;
	CTime CurrentTime = CTime::GetCurrentTime();

	m_nAntiDriftStoreTime=(int)CurrentTime.GetTime();

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	CpSurface.Side0.pt = m_CpAntiDrift.Side0.pt;

	switch(cVec) {
	case 'i':
		CpSurface.Side0.fI += (fDist /30.0f);
		break;
	case 'j':
		CpSurface.Side0.fJ += (fDist /30.0f);
		break;
	};
	CpSurface.UnityVector();

	theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);
	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
	theApp.m_Motors.SuspendMotors();
	theApp.m_Motors.EnableAxes(BOTH_MOVETO);
	theApp.m_Motors.DesirePosEqualsCoord(CpHead);
	theApp.m_Motors.MoveTo(DONT_WAIT);
}

void CJogPage::OnTimer(UINT nIDEvent) 
{
	CTime CurrentTime = CTime::GetCurrentTime();

	if((CurrentTime.GetTime() - m_nAntiDriftStoreTime) > 4) {
		theApp.m_Motors.GetSurfacePos(&m_CpAntiDrift);
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}



int CJogPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY,
		CRect(0, 2, 0, 0)) || !m_wndToolBar.LoadToolBar(IDR_RAILWAY_WHEEL_TOOLBAR)) {
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this, RBS_BANDBORDERS, CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop, 0, 0, rect.Width(), 30, SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);

	return 0;
}

BOOL CJogPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	int nLength;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	switch (nID) {
	case ID_BUTTON_LEFT_WHEEL: strTipText.LoadString(ID_BUTTON_LEFT_WHEEL);
		break;
	case ID_BUTTON_RIGHT_WHEEL: strTipText.LoadString(ID_BUTTON_RIGHT_WHEEL);
		break;
	case ID_BUTTON_BOTH_WHEELS: strTipText.LoadString(ID_BUTTON_BOTH_WHEELS);
		break;
		break;
	}

	if ((nLength = strTipText.Find('\n', 0)) > 0) {
		strTipText.GetBufferSetLength(nLength);
	}
	lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));

	*pResult = 0;

	return TRUE;    // message was handled
}

void CJogPage::SetToolBarCheckedState()
{
	int nIndex;

	if ((nIndex = m_wndToolBar.CommandToIndex(ID_BUTTON_LEFT_WHEEL)) >= 0)
		m_nSide == 0 ? m_wndToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex, 0);
	if ((nIndex = m_wndToolBar.CommandToIndex(ID_BUTTON_RIGHT_WHEEL)) >= 0)
		m_nSide == 1 ? m_wndToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex, 0);
	if ((nIndex = m_wndToolBar.CommandToIndex(ID_BUTTON_BOTH_WHEELS)) >= 0)
		m_nSide == 2 ? m_wndToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex, 0);

}

void CJogPage::OnButtonLeftWheel()
{

	m_nSide = 0;
	SetToolBarCheckedState();
}

void CJogPage::OnButtonRightWheel()
{

	m_nSide = 1;
	SetToolBarCheckedState();
}

void CJogPage::OnButtonBothWheels()
{

	m_nSide = 2;
	SetToolBarCheckedState();
}
