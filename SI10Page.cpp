// SI10Page.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "SI10Page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSI10Page property page

IMPLEMENT_DYNCREATE(CSI10Page, CPropertyPage)

CSI10Page::CSI10Page() : CPropertyPage(CSI10Page::IDD)
{
	//{{AFX_DATA_INIT(CSI10Page)
	m_strVersion = _T("");
	m_nIRQCount0 = 0;
	m_nIRQCount1 = 0;
	m_nIRQCount2 = 0;
	m_nIRQCount3 = 0;
	m_nIRQCount4 = 0;
	m_nIRQCount5 = 0;
	m_nIRQCount6 = 0;
	m_nIRQCount7 = 0;
	m_nIRQCount8 = 0;
	m_nIRQCount9 = 0;
	m_nIRQCount = 0;
	m_nPRFIRQCount = 0;
	m_nEvents = 0;
	m_strProcessState = _T("");
	m_strLimitSwitches = _T("");
	m_strFirmwareVersion = _T("");
	//}}AFX_DATA_INIT
	
}

CSI10Page::~CSI10Page()
{
}

void CSI10Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSI10Page)
	DDX_Control(pDX, IDC_CHECK_FORCE_FIRMWARE_LOAD, m_checkForceFirmwareLoad);
	DDX_Control(pDX, IDC_COMBO_GPIO_IRQ_MODE, m_comboGPIOIrqMode);
	DDX_Control(pDX, IDC_EDIT_SWITCH_EOR, m_editSwitchEor);
	DDX_Control(pDX, IDC_EDIT_JOYSTICK_3, m_editJoystick3);
	DDX_Control(pDX, IDC_EDIT_JOYSTICK_2, m_editJoystick2);
	DDX_Control(pDX, IDC_EDIT_JOYSTICK_1, m_editJoystick1);
	DDX_Control(pDX, IDC_EDIT_JOYSTICK_0, m_editJoystick0);
	DDX_Text(pDX, IDC_EDIT_VERSION, m_strVersion);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT_0, m_nIRQCount0);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT_1, m_nIRQCount1);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT_2, m_nIRQCount2);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT_3, m_nIRQCount3);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT_4, m_nIRQCount4);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT_5, m_nIRQCount5);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT_6, m_nIRQCount6);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT_7, m_nIRQCount7);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT_8, m_nIRQCount8);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT_9, m_nIRQCount9);
	DDX_Text(pDX, IDC_EDIT_IRQ_COUNT, m_nIRQCount);
	DDX_Text(pDX, IDC_EDIT_PRF_IRQ_COUNT, m_nPRFIRQCount);
	DDX_Text(pDX, IDC_EDIT_EVENTS_COUNT, m_nEvents);
	DDX_Text(pDX, IDC_EDIT_PROCESS_STATE, m_strProcessState);
	DDX_Text(pDX, IDC_EDIT_LIMIT_SWITCHES, m_strLimitSwitches);
	DDX_Text(pDX, IDC_EDIT_FIRMWARE_VERSION, m_strFirmwareVersion);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_ISR_COUNT, m_editISRCount);
	DDX_Control(pDX, IDC_EDIT_DPC_EVENTS, m_editDPCEvents);
	DDX_Control(pDX, IDC_EDIT_GP_INPUT, m_editGPInput);
}


BEGIN_MESSAGE_MAP(CSI10Page, CPropertyPage)
	//{{AFX_MSG_MAP(CSI10Page)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ZERO_IRQ_COUNT, OnButtonZeroIrqCount)
	ON_EN_CHANGE(IDC_EDIT_SWITCH_EOR, OnChangeEditSwitchEor)
	ON_CBN_SELCHANGE(IDC_COMBO_GPIO_IRQ_MODE, OnSelchangeComboGpioIrqMode)
	ON_BN_CLICKED(IDC_CHECK_FORCE_FIRMWARE_LOAD, OnCheckForceFirmwareLoad)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CSI10Page::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_START_ISR, &CSI10Page::OnBnClickedButtonStartIsr)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_INTERRUPT_STATUS, &CSI10Page::OnBnClickedButtonClearInterruptStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSI10Page message handlers

BOOL CSI10Page::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.LoadString(IDS_Off);			m_comboGPIOIrqMode.AddString(Buff);
	Buff.LoadString(IDS_Rising_Edge);	m_comboGPIOIrqMode.AddString(Buff);
	Buff.LoadString(IDS_Falling_Edge);	m_comboGPIOIrqMode.AddString(Buff);
	Buff.LoadString(IDS_Level_change);	m_comboGPIOIrqMode.AddString(Buff);
	
	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSI10Page::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}


void CSI10Page::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;


	Buff.Format(_T("%05x h"),theApp.m_Si10User.m_Si10.nSwitchEor);
	m_editSwitchEor.SetWindowText(Buff);

	theApp.m_Si10User.GetDriverVersion(&m_strVersion);
	theApp.m_Si10User.GetFirmwareVersion(&m_strFirmwareVersion);

	m_comboGPIOIrqMode.SetCurSel( theApp.m_Tank.cSi10GPIOIrqMode[0] );

	m_checkForceFirmwareLoad.SetCheck( theApp.m_LastSettings.nForceSi10FirmwareLoad );

	UpdateData(FALSE);
}

void CSI10Page::OnTimer(UINT nIDEvent) 
{
	int nValue,nAxis,nWhich;
	CString Buff;

	Buff.Format(L"%d", theApp.m_Si10Userx64.m_nEventsCount);
	m_editDPCEvents.SetWindowTextW(Buff);

	Buff.Format(L"%u", theApp.m_Si10Userx64.getISRCount());
	m_editISRCount.SetWindowTextW(Buff);
	
	if(m_nOldJoyValue[0] - (nValue = theApp.m_Si10User.ReadJoystick(0))) {
		Buff.Format(_T("%d"),m_nOldJoyValue[0] = nValue);
		m_editJoystick0.SetWindowText(Buff);
	}

	if(m_nOldJoyValue[1] - (nValue = theApp.m_Si10User.ReadJoystick(1))) {
		Buff.Format(_T("%d"),m_nOldJoyValue[1] = nValue);
		m_editJoystick1.SetWindowText(Buff);
	}

	if(m_nOldJoyValue[2] - (nValue = theApp.m_Si10User.ReadJoystick(2))) {
		Buff.Format(_T("%d"),m_nOldJoyValue[2] = nValue);
		m_editJoystick2.SetWindowText(Buff);
	}

	if(m_nOldJoyValue[3] - (nValue = theApp.m_Si10User.ReadJoystick(3))) {
		Buff.Format(_T("%d"),m_nOldJoyValue[3] = nValue);
		m_editJoystick3.SetWindowText(Buff);
	}

	m_nIRQCount = theApp.m_Axes[4].nDesireRevCount;// theApp.m_Si10User.m_Si10.nIrqCount;
	m_nPRFIRQCount = theApp.m_Axes[4].nDesirePos; //theApp.m_Si10User.m_Si10.nIrqPRFCount;
	m_nEvents = theApp.m_Si10User.m_nEventsCount;// theApp.m_Si10User.m_Si10.nIrqCount;//m_nEventsCount;

	for(nAxis = 0, nWhich = 0;nAxis<24;nAxis++) {
		if((theApp.m_Axes[nAxis].bAvailable==TRUE) && (theApp.m_Axes[nAxis].nMotorType==STEPPER)) {
			switch(nWhich) {
			case 0:	m_nIRQCount0 = theApp.m_Axes[nAxis].nIrqCount;
				break;
			case 1:	m_nIRQCount1 = theApp.m_Axes[nAxis].nIrqCount;
				break;
			case 2:	m_nIRQCount2 = theApp.m_Axes[nAxis].nIrqCount;
				break;
			case 3:	m_nIRQCount3 = theApp.m_Axes[nAxis].nIrqCount;
				break;
			case 4:	m_nIRQCount4 = theApp.m_Axes[nAxis].nIrqCount;
				break;
			case 5:	m_nIRQCount5 = theApp.m_Axes[nAxis].nIrqCount;
				break;
			case 6:	m_nIRQCount6 = theApp.m_Axes[nAxis].nIrqCount;
				break;
			case 7:	m_nIRQCount7 = theApp.m_Axes[nAxis].nIrqCount;
				break;
			case 8:	m_nIRQCount8 = theApp.m_Axes[nAxis].nIrqCount;
				break;
			case 9:	m_nIRQCount9 = theApp.m_Axes[nAxis].nIrqCount;
				break;
			}
			nWhich++;
		}
	}
//	m_strProcessState.Format(_T("%02x"),theApp.m_Si10User.m_Si10.nProcessState);
	m_strProcessState.Format(_T("%02x"),theApp.m_Si10User.m_Si10.nDirEor);
	m_strLimitSwitches.Format(_T("%05x"),theApp.m_Si10User.ReadLimitSwitches());
	Buff.Format(L"%02xh", theApp.m_Si10User.GetInputPort());
	m_editGPInput.SetWindowTextW(Buff);

	UpdateData(FALSE);

	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CSI10Page::OnSetActive()
{
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	SetTimer(1, 100, NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CSI10Page::OnKillActive()
{
	KillTimer(1);

	return CPropertyPage::OnSetActive();
}

void CSI10Page::OnButtonZeroIrqCount() 
{
	theApp.m_Si10User.m_Si10.nIrqCount = 0;
	theApp.m_Si10User.m_Si10.nIrqPRFCount = 0;
	theApp.m_Si10User.m_nEventsCount = 0;
	for(int ii=0;ii<24;ii++) {
		theApp.m_Axes[ii].nIrqCount = 0;
	}
	
}

void CSI10Page::OnChangeEditSwitchEor() 
{
	CString Buff;

	m_editSwitchEor.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Si10User.m_Si10.nSwitchEor);
	theApp.m_Tank.nSi10SwitchEor = theApp.m_Si10User.m_Si10.nSwitchEor;
	
}

void CSI10Page::OnSelchangeComboGpioIrqMode() 
{
	theApp.m_Tank.cSi10GPIOIrqMode[0] = m_comboGPIOIrqMode.GetCurSel();
	theApp.m_Si10User.SetGPIOIRQMode();
}

void CSI10Page::OnCheckForceFirmwareLoad() 
{
	theApp.m_LastSettings.nForceSi10FirmwareLoad = m_checkForceFirmwareLoad.GetCheck();
}


void CSI10Page::OnBnClickedButtonReset()
{
	theApp.m_Si10User.Reset();
}


void CSI10Page::OnBnClickedButtonStartIsr()
{
	if (theApp.m_Si10Userx64.IsConnected()) {

		U32 CounterEnabled = 1;
		theApp.m_Si10User.OutpW(0x1A8, CounterEnabled);

		theApp.m_Si10User.CounterStart(0, 1000000 / 1000);

		//Select pulse source
		U32 PulseSource = 1;
		theApp.m_Si10User.OutpW(0x200, PulseSource);

		//Turn on interrupts 
		U32 Interrupt = 0x0001;
		theApp.m_Si10User.OutpW(0x1AC, Interrupt);

	}
}


void CSI10Page::OnBnClickedButtonClearInterruptStatus()
{
	int nn = theApp.m_Si10Userx64.InpW(0x4c);
}
