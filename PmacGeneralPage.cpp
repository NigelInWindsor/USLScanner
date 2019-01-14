// PmacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PmacGeneralPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacGeneralPage dialog


IMPLEMENT_DYNCREATE(CPmacGeneralPage, CResizablePage)

CPmacGeneralPage::CPmacGeneralPage() : CResizablePage(CPmacGeneralPage::IDD)
{
	//{{AFX_DATA_INIT(CPmacGeneralPage)
	m_nIrqCount = 0;
	//}}AFX_DATA_INIT
	
	m_bCtrlButton = false;
	m_pPromptDlg = NULL;
}


void CPmacGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmacGeneralPage)
	DDX_Control(pDX, IDC_EDIT_SERVO_UPDATE_RATE, m_editServoUpdateRate);
	DDX_Control(pDX, IDC_EDIT_SERVO_RATE, m_editServoRate);
	DDX_Control(pDX, IDC_EDIT_ERROR_COUNT, m_editErrorCount);
	DDX_Control(pDX, IDC_CHECK_HARDWAREIRQ, m_checkHardwareIRQ);
	DDX_Control(pDX, IDC_EDIT_RESPONCE, m_editResponce);
	DDX_Control(pDX, IDC_EDIT_INLINECOMMAND, m_editInLineCommand);
	DDX_Control(pDX, IDC_COMBO_ENABLE_DEVICE, m_comboEnableDevice);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT, m_nIrqCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmacGeneralPage, CDialog)
	//{{AFX_MSG_MAP(CPmacGeneralPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_ENABLE_DEVICE, OnSelchangeComboEnableDevice)
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_CHECK_HARDWAREIRQ, OnCheckHardwareirq)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_SERVO_UPDATE_RATE, OnChangeEditServoUpdateRate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacGeneralPage message handlers


BOOL CPmacGeneralPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	CString	Buff;
	
	m_StaticPosResponce.Initialize(this, m_editResponce, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosResponce.SetNewPos();
	
	Buff.LoadString(IDS_On);	m_comboEnableDevice.AddString(Buff);
	Buff.LoadString(IDS_Off);	m_comboEnableDevice.AddString(Buff);

	m_checkHardwareIRQ.SetCheck(theApp.m_LastSettings.bPMACUsesHardwareIRQ);

	
	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPmacGeneralPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPmacGeneralPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd()) {
		m_StaticPosResponce.SetNewPos();
	}
		
}


void CPmacGeneralPage::UpdateAllControls()
{
	CString Buff;
	m_comboEnableDevice.SetCurSel(theApp.m_bServoHardware);

	Buff.Format(L"%.1fHz",theApp.m_Tank.fServoUpdateRate);
	m_editServoUpdateRate.SetWindowText(Buff);

}

void CPmacGeneralPage::OnSelchangeComboEnableDevice() 
{
	theApp.m_bServoHardware = m_comboEnableDevice.GetCurSel() & 0x01;
}

BOOL CPmacGeneralPage::PreTranslateMessage(MSG* pMsg) 
{
	CString Buff;

	switch(pMsg->message) {
	case WM_KEYUP:
		m_bCtrlButton = false;
		break;
	case WM_KEYDOWN:
		switch(pMsg->wParam) {
		case 17: m_bCtrlButton=true;//Ctrl button down
			break;
		case 'T':
			if(m_bCtrlButton==true) {
				Buff.Format(L"%c",0x14);
				m_editInLineCommand.SetWindowText(Buff);
			};
			break;
		case 'V':
			if(m_bCtrlButton==true) {
				Buff.Format(L"%c",0x16);
				m_editInLineCommand.SetWindowText(Buff);
			};
			break;
		case 13:
			if(&m_editInLineCommand == FromHandle(pMsg->hwnd)) {
				Buff.Empty();
				m_editResponce.SetWindowText(Buff);
				m_editInLineCommand.GetWindowText(Buff);
				if(Buff == "save") {
					m_pPromptDlg = new CPromptDlg(this,L"All variables being saved to non volatile memory",L"Save");
					m_pPromptDlg->Create(CPromptDlg::IDD, this);
				}

				theApp.m_PmacUser.SendStr(Buff);
				Buff.Empty();
				m_editInLineCommand.SetWindowText(Buff);
				return TRUE;
			}
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CPmacGeneralPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;

	if(theApp.m_PmacUser.m_nWhichDLL == PMACISAUSL)
		theApp.m_PmacUser.nIrqCount = theApp.m_PmacUser.m_ServoUser.m_Pmac.nIrqCount;

	c_f = clock();
	int nCount = theApp.m_PmacUser.nIrqCount - m_nOldIrqCount;
//	int nCount = theApp.m_PmacUser.m_nDAC[0] - m_nOldIrqCount;
	double dElapsedTime = (double)(c_f-c_i)/CLOCKS_PER_SEC;
	if(dElapsedTime > 1.0) {
		c_i = c_f;
		m_nOldIrqCount = theApp.m_PmacUser.nIrqCount;
//		m_nOldIrqCount = theApp.m_PmacUser.m_nDAC[0];
		Buff.Format(L"%.01f Hz",(double)nCount/dElapsedTime);
		m_editServoRate.SetWindowText(Buff);
	}


	if(theApp.m_PmacUser.GetStr(Buff)) {
		Buff.Replace(_T("\r"),_T("\r\n"));
		m_editResponce.SetWindowText(Buff);

		if(m_pPromptDlg) {
			m_pPromptDlg->SendMessage(WM_CLOSE);
			SAFE_DELETE( m_pPromptDlg );

		}
	}
	m_nIrqCount = theApp.m_PmacUser.nIrqCount;
//	m_nIrqCount = theApp.m_PmacUser.m_nDAC[0];


	UpdateData(FALSE);



	if(m_nOldErrorCount != theApp.m_PmacUser.m_nDPRAMError) {
//		Buff.Format(_T("%d"),(theApp.m_PmacUser.nIrqCount-m_nOldErrorCount)*5);
		Buff.Format(_T("%d"),m_nOldErrorCount = theApp.m_PmacUser.m_nDPRAMError);
		m_editErrorCount.SetWindowText(Buff);
	}

	
	m_nOldErrorCount = theApp.m_PmacUser.m_nDPRAMError;


	CDialog::OnTimer(nIDEvent);
}

void CPmacGeneralPage::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
}

void CPmacGeneralPage::OnButtonReset() 
{
	theApp.m_PmacUser.DisableInterrupt();

	Sleep(100);

	theApp.m_PmacUser.EnableInterrupt();
}

BOOL CPmacGeneralPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	if(m_bTimerActive != TRUE) SetTimer(1,200,NULL);
	m_bTimerActive = TRUE;
	
	return CResizablePage::OnSetActive();
}

BOOL CPmacGeneralPage::OnKillActive() 
{
	if(m_bTimerActive == TRUE) KillTimer(1);
	m_bTimerActive = FALSE;
	
	return CResizablePage::OnKillActive();
}

void CPmacGeneralPage::OnCheckHardwareirq() 
{
	theApp.m_LastSettings.bPMACUsesHardwareIRQ = m_checkHardwareIRQ.GetCheck();
	MessageBox(_T("Requires Restart"),_T("Warning"),NULL);
}


void CPmacGeneralPage::OnChangeEditServoUpdateRate() 
{
	CString Buff;

	m_editServoUpdateRate.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fServoUpdateRate);
}
