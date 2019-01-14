// AOSPhasedArrayPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "AOSPhasedArrayPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAOSPhasedArrayPage property page

IMPLEMENT_DYNCREATE(CAOSPhasedArrayPage, CPropertyPage)

CAOSPhasedArrayPage::CAOSPhasedArrayPage() : CPropertyPage(CAOSPhasedArrayPage::IDD)
{
	//{{AFX_DATA_INIT(CAOSPhasedArrayPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAOSPhasedArrayPage::~CAOSPhasedArrayPage()
{
}

void CAOSPhasedArrayPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAOSPhasedArrayPage)
	DDX_Control(pDX, IDC_AOS_IPADDRESS, m_ipAddress);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_CONNECT_AT_STARTUP, m_checkConnectAtStartUp);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_buttonConnect);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_editStatus);
	DDX_Control(pDX, IDC_EDIT_FRAME_COUNT, m_editFrameCount);
	DDX_Control(pDX, IDC_EDIT_FRAME_RATE, m_editFrameRate);
	DDX_Control(pDX, IDC_EDIT_PRF_RATE, m_editPRFRate);
	DDX_Control(pDX, IDC_EDIT_DATA_RATE, m_editDataRate);
	DDX_Control(pDX, IDC_SPIN_ASCAN_LENGTH, m_editspinAScanLength);
}


BEGIN_MESSAGE_MAP(CAOSPhasedArrayPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAOSPhasedArrayPage)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_AOS_IPADDRESS, OnFieldchangedAosIpaddress)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CAOSPhasedArrayPage::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_CHECK_CONNECT_AT_STARTUP, &CAOSPhasedArrayPage::OnBnClickedCheckConnectAtStartup)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_HW_FILE, &CAOSPhasedArrayPage::OnBnClickedButtonHwFile)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ASCAN_LENGTH, &CAOSPhasedArrayPage::OnDeltaposSpinAscanLength)
	ON_EN_CHANGE(IDC_EDIT_ASCAN_LENGTH, &CAOSPhasedArrayPage::OnEnChangeEditAscanLength)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CAOSPhasedArrayPage::OnBnClickedButtonDefault)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAOSPhasedArrayPage message handlers

BOOL CAOSPhasedArrayPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	if (theApp.m_AOSPhasedArray.isConnected() == true) 
		theApp.m_AOSPhasedArray.getStatus();

	m_editspinAScanLength.Init((float)theApp.m_AOSPhasedArray.m_nAScanLength, 512.0f, 8192.0f, 16.0f, 1.0f, L"%.0f");

	UpdateAllControls();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAOSPhasedArrayPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CAOSPhasedArrayPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	CString Buff, Temp;
	sockaddr_in Server;

	Server.sin_addr.s_addr = theApp.m_Tank.nAOSIPAddrs[0];
	m_ipAddress.SetAddress(Server.sin_addr.s_addr); 

	m_checkConnectAtStartUp.SetCheck( theApp.m_Tank.bConnectAOSAtStartUp[0]);

	if (theApp.m_AOSPhasedArray.isConnected() == true) {
		m_buttonConnect.SetWindowText(L"Disconnect");
	}
	else {
		m_buttonConnect.SetWindowText(L"Connect");
	}

	Buff.Empty();
	theApp.m_AOSPhasedArray.getMaxNumberElements(&Temp);	Buff += Temp + L"\r\n";
	theApp.m_AOSPhasedArray.getMaxApertureSize(&Temp);	Buff += Temp + L"\r\n";
	theApp.m_AOSPhasedArray.getMaxCycleCount(&Temp);		Buff += Temp + L"\r\n";
	theApp.m_AOSPhasedArray.isMultiplexer(&Temp);			Buff += Temp + L"\r\n";

	m_editStatus.SetWindowTextW(Buff);


}

BOOL CAOSPhasedArrayPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	SetTimer(1, 200, NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CAOSPhasedArrayPage::OnKillActive() 
{

	KillTimer(1);

	return CPropertyPage::OnKillActive();
}

void CAOSPhasedArrayPage::OnFieldchangedAosIpaddress(NMHDR* pNMHDR, LRESULT* pResult) 
{
	sockaddr_in Server;

	m_ipAddress.GetAddress(Server.sin_addr.s_addr); 
	theApp.m_Tank.nAOSIPAddrs[0] = Server.sin_addr.s_addr;
	
	*pResult = 0;
}


void CAOSPhasedArrayPage::OnBnClickedButtonConnect()
{
	sockaddr_in Server;

	if (theApp.m_Tank.nAOSIPAddrs[0] == 0) {
		ZeroMemory(&Server, sizeof Server);
		Server.sin_addr.S_un.S_un_b.s_b4 = 192;
		Server.sin_addr.S_un.S_un_b.s_b3 = 168;
		Server.sin_addr.S_un.S_un_b.s_b2 = 1;
		Server.sin_addr.S_un.S_un_b.s_b1 = 11;
		theApp.m_Tank.nAOSIPAddrs[0] = Server.sin_addr.s_addr;
	}

	if (theApp.m_AOSPhasedArray.isConnected() == false) {
		if (theApp.m_AOSPhasedArray.Connect(theApp.m_Tank.nAOSIPAddrs[0]) == true) {
			theApp.m_AOSPhasedArray.getStatus();
		}
	}
	else {
		theApp.m_AOSPhasedArray.Disconnect();
	}
	UpdateAllControls();


}


void CAOSPhasedArrayPage::OnBnClickedCheckConnectAtStartup()
{
	theApp.m_Tank.bConnectAOSAtStartUp[0] = m_checkConnectAtStartUp.GetCheck();
}


void CAOSPhasedArrayPage::OnTimer(UINT_PTR nIDEvent)
{
	CString Buff;

	if (theApp.m_AOSPhasedArray.isConnected()) {
		Buff.Format(L"%d", theApp.m_AOSPhasedArray.getFrameCount());
		m_editFrameCount.SetWindowTextW(Buff);
		Buff.Format(L"%d /s", theApp.m_AOSPhasedArray.getFrameRate());
		m_editFrameRate.SetWindowTextW(Buff);
		Buff.Format(L"%d /s", theApp.m_AOSPhasedArray.getPRFRate());
		m_editPRFRate.SetWindowTextW(Buff);
		Buff.Format(L"%.02f Mb/s", (float)theApp.m_AOSPhasedArray.getDataRate()/1e6f);
		m_editDataRate.SetWindowTextW(Buff);
	}

	CPropertyPage::OnTimer(nIDEvent);
}


void CAOSPhasedArrayPage::OnBnClickedButtonHwFile()
{
	theApp.m_AOSPhasedArray.HWToFile();
}



void CAOSPhasedArrayPage::OnDeltaposSpinAscanLength(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_AOSPhasedArray.m_nAScanLength = (int)m_editspinAScanLength.GetValue();
	*pResult = 0;
}


void CAOSPhasedArrayPage::OnEnChangeEditAscanLength()
{
	theApp.m_AOSPhasedArray.m_nAScanLength = (int)m_editspinAScanLength.GetValue();
}


void CAOSPhasedArrayPage::OnBnClickedButtonDefault()
{
	sockaddr_in Server;

	Server.sin_addr.S_un.S_un_b.s_b4 = 192;
	Server.sin_addr.S_un.S_un_b.s_b3 = 168;
	Server.sin_addr.S_un.S_un_b.s_b2 = 1;
	Server.sin_addr.S_un.S_un_b.s_b1 = 11;
	theApp.m_Tank.nAOSIPAddrs[0] = Server.sin_addr.s_addr;

	UpdateAllControls();

}
