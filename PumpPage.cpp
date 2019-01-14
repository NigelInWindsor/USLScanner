// PumpPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PumpPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPumpPage property page

IMPLEMENT_DYNCREATE(CPumpPage, CPropertyPage)

CPumpPage::CPumpPage() : CPropertyPage(CPumpPage::IDD)
{
	//{{AFX_DATA_INIT(CPumpPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CPumpPage::~CPumpPage()
{
}

void CPumpPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPumpPage)
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	DDX_Control(pDX, IDC_SPIN_RIGHT_SPEED, m_spinRightSpeed);
	DDX_Control(pDX, IDC_SPIN_LEFT_SPEED, m_spinLeftSpeed);
	DDX_Control(pDX, IDC_EDIT_RIGHT_SPEED, m_editRightSpeed);
	DDX_Control(pDX, IDC_EDIT_LEFT_SPEED, m_editLeftSpeed);
	DDX_Control(pDX, IDC_CHECK_RIGHT_OFFON, m_checkRightOffOn);
	DDX_Control(pDX, IDC_CHECK_LEFT_OFFON, m_checkLeftOffOn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPumpPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPumpPage)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_LEFT_SPEED, OnChangeEditLeftSpeed)
	ON_EN_CHANGE(IDC_EDIT_RIGHT_SPEED, OnChangeEditRightSpeed)
	ON_BN_CLICKED(IDC_CHECK_LEFT_OFFON, OnCheckLeftOffon)
	ON_BN_CLICKED(IDC_BUTTON_PURGE_LEFT, OnButtonPurgeLeft)
	ON_BN_CLICKED(IDC_BUTTON_PURGE_RIGHT, OnButtonPurgeRight)
	ON_BN_CLICKED(IDC_CHECK_RIGHT_OFFON, OnCheckRightOffon)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPumpPage message handlers

BOOL CPumpPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_spinLeftSpeed.SetRange(0,300);
	m_spinRightSpeed.SetRange(0,300);


	

	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPumpPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPumpPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	bool bFlag;
	CString Buff;

	m_staticLeft.SetWindowText(theApp.m_DlgSideName[0]);
	m_staticRight.SetWindowText(theApp.m_DlgSideName[1]);

	if(theApp.m_nSide0Orientation==0) {
		Buff.Format(_T("%d"),theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);
		m_editLeftSpeed.SetWindowText(Buff);
		m_spinLeftSpeed.SetPos(theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);

		Buff.Format(_T("%d"),theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);
		m_editRightSpeed.SetWindowText(Buff);
		m_spinRightSpeed.SetPos(theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);

		bFlag = theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nLeftPump, 0);
		m_checkLeftOffOn.SetCheck(bFlag);
		bFlag = theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nRightPump, 1);
		m_checkRightOffOn.SetCheck(bFlag);
	} else {
		Buff.Format(_T("%d"),theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);
		m_editLeftSpeed.SetWindowText(Buff);
		m_spinLeftSpeed.SetPos(theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);

		Buff.Format(_T("%d"),theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);
		m_editRightSpeed.SetWindowText(Buff);
		m_spinRightSpeed.SetPos(theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);

		bFlag = theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nRightPump, 0);
		m_checkLeftOffOn.SetCheck(bFlag);
		bFlag = theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nLeftPump, 1);
		m_checkRightOffOn.SetCheck(bFlag);
	}

	SetAccessPrivelage();
}

void CPumpPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CString Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;


	if(theApp.m_nSide0Orientation==0) {
		switch(pSpin->GetDlgCtrlID()) {
		case IDC_SPIN_LEFT_SPEED:
			theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed = pSpin->GetPos()&0xffff;
			Buff.Format(_T("%d"),theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);
			m_editLeftSpeed.SetWindowText(Buff);
			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nLeftPump,0);
			break;
		case IDC_SPIN_RIGHT_SPEED:
			theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed = pSpin->GetPos()&0xffff;
			Buff.Format(_T("%d"),theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);
			m_editRightSpeed.SetWindowText(Buff);
			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nRightPump, 1);
			break;
		}
	} else {
		switch(pSpin->GetDlgCtrlID()) {
		case IDC_SPIN_LEFT_SPEED:
			theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed = pSpin->GetPos()&0xffff;
			Buff.Format(_T("%d"),theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);
			m_editLeftSpeed.SetWindowText(Buff);
			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nRightPump, 1);
			break;
		case IDC_SPIN_RIGHT_SPEED:
			theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed = pSpin->GetPos()&0xffff;
			Buff.Format(_T("%d"),theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);
			m_editRightSpeed.SetWindowText(Buff);
			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nLeftPump, 0);
			break;
		}
	}
	pFrame->SendMessage(UI_UPDATE_PUMPS_DIALOGBAR);

	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPumpPage::OnChangeEditLeftSpeed() 
{
	
	CString Buff;
	int nTemp;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(theApp.m_nSide0Orientation==0) {
		nTemp = theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed;

		m_editLeftSpeed.GetWindowText(Buff);
		theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed = _ttoi(Buff);
		if(theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed<0) theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed=0;
		if(theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed>300) theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed=300;

		if(nTemp - theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed) {
			m_spinLeftSpeed.SetPos(theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);

			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nLeftPump, 0);
			pFrame->SendMessage(UI_UPDATE_PUMPS_DIALOGBAR);
		}
	} else {
		nTemp = theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed;

		m_editLeftSpeed.GetWindowText(Buff);
		theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed = _ttoi(Buff);
		if(theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed<0) theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed=0;
		if(theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed>300) theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed=300;

		if(nTemp - theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed) {
			m_spinLeftSpeed.SetPos(theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);

			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nRightPump , 0);
			pFrame->SendMessage(UI_UPDATE_PUMPS_DIALOGBAR);
		}
	}
}

void CPumpPage::OnChangeEditRightSpeed() 
{
	
	CString Buff;
	int nTemp;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(theApp.m_nSide0Orientation==0) {
		nTemp = theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed;

		m_editRightSpeed.GetWindowText(Buff);
		theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed = _ttoi(Buff);
		if(theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed<0) theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed=0;
		if(theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed>300) theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed=300;

		if(nTemp - theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed) {
			m_spinRightSpeed.SetPos(theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);

			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nRightPump, 1);
			pFrame->SendMessage(UI_UPDATE_PUMPS_DIALOGBAR);
		}
	} else {
		nTemp = theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed;

		m_editRightSpeed.GetWindowText(Buff);
		theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed = _ttoi(Buff);
		if(theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed<0) theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed=0;
		if(theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed>300) theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed=300;

		if(nTemp - theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed) {
			m_spinRightSpeed.SetPos(theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);

			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nLeftPump, 0);
			pFrame->SendMessage(UI_UPDATE_PUMPS_DIALOGBAR);
		}
	}
}

void CPumpPage::OnCheckLeftOffon() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	bool bFlag;

	if(theApp.m_nSide0Orientation==0) {
		bFlag = theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nLeftPump, 0);
		bFlag = m_checkLeftOffOn.GetCheck()&0x0001;
		theApp.m_PmacUser.PumpStopStart(theApp.m_Tank.nLeftPump,bFlag);
	} else {
		bFlag = theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nRightPump, 1);
		bFlag = m_checkLeftOffOn.GetCheck()&0x0001;
		theApp.m_PmacUser.PumpStopStart(theApp.m_Tank.nRightPump,bFlag);
	}
	pFrame->SetToolBarCheckedState(TRUE);
	
}

void CPumpPage::OnCheckRightOffon() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	bool bFlag;

	if(theApp.m_nSide0Orientation==0) {
		bFlag = theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nRightPump, 1);
		bFlag = m_checkRightOffOn.GetCheck()&0x0001;
		theApp.m_PmacUser.PumpStopStart(theApp.m_Tank.nRightPump,bFlag);
	} else {
		bFlag = theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nLeftPump, 0);
		bFlag = m_checkRightOffOn.GetCheck()&0x0001;
		theApp.m_PmacUser.PumpStopStart(theApp.m_Tank.nLeftPump,bFlag);
	}
	pFrame->SetToolBarCheckedState(TRUE);
}


BOOL CPumpPage::OnSetActive() 
{
	
	
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}

BOOL CPumpPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CPumpPage::OnButtonPurgeLeft() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_PmacUser.PurgePump(theApp.m_Tank.nLeftPump,TRUE);
	} else {
		theApp.m_PmacUser.PurgePump(theApp.m_Tank.nRightPump,TRUE);
	}
	pFrame->SetToolBarCheckedState(TRUE);
	UpdateAllControls();
}

void CPumpPage::OnButtonPurgeRight() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_PmacUser.PurgePump(theApp.m_Tank.nRightPump,TRUE);
	} else {
		theApp.m_PmacUser.PurgePump(theApp.m_Tank.nLeftPump,TRUE);
	}

	pFrame->SetToolBarCheckedState(TRUE);
	UpdateAllControls();

}


void CPumpPage::SetAccessPrivelage()
{
	
	bool bFlagEnable;
	CButton* pButton;

	theApp.m_nLogonLevelMask &  theApp.m_cAccess[IDD_NORMALIZE_PAGE] ? bFlagEnable = TRUE : bFlagEnable = FALSE;

	m_spinRightSpeed.EnableWindow(bFlagEnable);
	m_spinLeftSpeed.EnableWindow(bFlagEnable);
	m_editRightSpeed.EnableWindow(bFlagEnable);
	m_editLeftSpeed.EnableWindow(bFlagEnable);
	m_checkRightOffOn.EnableWindow(bFlagEnable);
	m_checkLeftOffOn.EnableWindow(bFlagEnable);

	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_PURGE_LEFT)) pButton->EnableWindow(bFlagEnable);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_PURGE_RIGHT)) pButton->EnableWindow(bFlagEnable);
}

void CPumpPage::OnTimer(UINT nIDEvent) 
{

	CPropertyPage::OnTimer(nIDEvent);
}

