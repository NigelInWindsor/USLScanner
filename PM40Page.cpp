#include "stdafx.h"
#include "USLScanner.h"
#include "PM40Page.h"



IMPLEMENT_DYNCREATE(CPM40Page, CPropertyPage)

CPM40Page::CPM40Page() : CPropertyPage(CPM40Page::IDD)
{
	//{{AFX_DATA_INIT(CPM40Page)
	//}}AFX_DATA_INIT

}

CPM40Page::~CPM40Page()
{
}

void CPM40Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPM40Page)
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_buttonConnect);
	DDX_Control(pDX, IDC_EDIT_DLL_VERSION, m_editDLLVersion);
	DDX_Control(pDX, IDC_EDIT_DRIVER_VERSION, m_editDriverVersion);
	DDX_Control(pDX, IDC_EDIT_CONVERSION_COUNT, m_editConversionCount);
	DDX_Control(pDX, IDC_EDIT_TIMED_OUT_COUNT, m_editTimedOutCount);
	DDX_Control(pDX, IDC_EDIT_ELAPSED_TIME, m_editElapsedTime);
	DDX_Control(pDX, IDC_EDIT_IRQ_COUNTS, m_editIrqCounts);
	DDX_Control(pDX, IDC_EDIT_MAILBOX_MAX_WAIT, m_editMailboxMaxWait);
}


BEGIN_MESSAGE_MAP(CPM40Page, CPropertyPage)
	//{{AFX_MSG_MAP(CPM40Page)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CPM40Page::OnBnClickedButtonConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CPM40Page::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_DMA_CAP, &CPM40Page::OnBnClickedButtonDmaCap)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PATTERN, &CPM40Page::OnBnClickedButtonTestPattern)
	ON_BN_CLICKED(IDC_CHECK_CONNECT_AT_STARTUP, &CPM40Page::OnBnClickedCheckConnectAtStartup)
	ON_BN_CLICKED(IDC_BUTTON_IRQ_COUNT, &CPM40Page::OnBnClickedButtonIrqCount)
	ON_BN_CLICKED(IDC_BUTTON_RESET_ALL_TS, &CPM40Page::OnBnClickedButtonResetAllTs)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPM40Page message handlers
BOOL CPM40Page::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPM40Page::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPM40Page::UpdateAllControls()
{
	CString Buff;

	m_editDLLVersion.SetWindowTextW( theApp.m_PM40User.getDllVersion() );
	m_editDriverVersion.SetWindowTextW(theApp.m_PM40User.getDriverVersion());

	if (theApp.m_PM40User.IsCardFound() == true) {
		m_buttonConnect.SetWindowText(L"Connected");
	}
	else {
		m_buttonConnect.SetWindowText(L"Connect to board");
	}
}

BOOL CPM40Page::OnSetActive()
{
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1, 100, NULL);

	return CPropertyPage::OnSetActive();
}


BOOL CPM40Page::OnKillActive()
{
	KillTimer(1);

	return CPropertyPage::OnKillActive();
}


void CPM40Page::OnBnClickedButtonConnect()
{
	if (theApp.m_PM40User.IsCardFound() == false) {
		if (theApp.m_PM40User.FindDevice() == STATUS_SUCCESSFUL) {
			UpdateAllControls();
		}
		else {
//			m_editMessage.SetWindowText(theApp.m_PM40.getLastErrorMessage());
		}
	}
}


void CPM40Page::OnTimer(UINT_PTR nIDEvent)
{
	CString Buff;
	static int nOldConversionCount = -1;
	static int nOldTimedOutCount = -1;
	static double dOldElapsedTime = -1.0;
	static int nOldMaxMailboxWait = -1;
	static int nOldTotalCommandsSent = -1;

	if (nOldConversionCount - theApp.m_PM40User.getConversionCount()) {
		Buff.Format(L"%d", nOldConversionCount = theApp.m_PM40User.getConversionCount());
		m_editConversionCount.SetWindowTextW(Buff);
	}

	if (nOldTimedOutCount - theApp.m_PM40User.getTimedOutCount()) {
		Buff.Format(L"%d", nOldTimedOutCount = theApp.m_PM40User.getTimedOutCount());
		m_editTimedOutCount.SetWindowTextW(Buff);
	}
	if (dOldElapsedTime - theApp.m_PM40User.getElapsedTime()) {
		Buff.Format(L"%.03fms", (dOldElapsedTime = theApp.m_PM40User.getElapsedTime()) * 1e3f);
		m_editElapsedTime.SetWindowTextW(Buff);
	}
//	if (nOldMaxMailboxWait - theApp.m_PM40User.m_nMaxMailboxWait) {
		Buff.Format(L"%d %d", nOldMaxMailboxWait = theApp.m_PM40User.m_nMaxMailboxWait, nOldTotalCommandsSent = theApp.m_PM40User.m_nTotalCommandsSent);
		m_editMailboxMaxWait.SetWindowTextW(Buff);
//	}
	if (nOldTotalCommandsSent - theApp.m_PM40User.m_nTotalCommandsSent) {
//		Buff.Format(L"%d", nOldTotalCommandsSent = theApp.m_PM40User.m_nTotalCommandsSent);
//		m_editMailboxMaxWait.SetWindowTextW(Buff);
	}


	CPropertyPage::OnTimer(nIDEvent);
}


void CPM40Page::OnBnClickedButton1()
{
	theApp.m_PM40User.ReadDMA();
}


void CPM40Page::OnBnClickedButtonDmaCap()
{
	theApp.m_PM40User.getDMACap();
}


void CPM40Page::OnBnClickedButtonTestPattern()
{
	theApp.m_UtUser.Pr30Command69h(&theApp.m_UtUser.m_TS[0], ADC_RAMP);
}


void CPM40Page::OnBnClickedCheckConnectAtStartup()
{
	// TODO: Add your control notification handler code here
}


void CPM40Page::OnBnClickedButtonIrqCount()
{
	static WCHAR wArray[100];
	CString Buff;

	GetAllInterruptCounts(0, wArray, sizeof wArray);
	Buff.Format(L"%s", wArray);
	m_editIrqCounts.SetWindowTextW(Buff);
}


void CPM40Page::OnBnClickedButtonResetAllTs()
{
	Pr30Data Pr;

	Pr.nSrcAddrs = theApp.m_UtUser.m_TS[0].Pr30.nSrcAddrs;
	Pr.nDestAddrs = theApp.m_UtUser.m_TS[0].Pr30.nDestAddrs;
	Pr.nWriteTimeSlot = 0;

	theApp.m_UtUser.Pr30Command4(&Pr);		//Set write timeslot to 0 == all timeslots
	theApp.m_UtUser.Pr30Command8(&Pr,0);		//Reset all timeslot content
}
