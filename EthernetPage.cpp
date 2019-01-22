// EthernetPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "EthernetPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEthernetPage property page

long EthernetCallbackLocal(CWnd* pParent, int nDataSize, char * pData)
{
	CEthernetPage* pDlg = (CEthernetPage*)pParent;
	CString Buff,Temp;

	pDlg->m_editRxStr.GetWindowText(Buff);

	Temp.GetBufferSetLength(strlen(pData));
	for(size_t ii=0;ii<strlen(pData);ii++) {
		Temp.SetAt(ii,pData[ii]);
	}
	if(Buff.GetLength()) Temp += "\r\n";
	Temp += Buff;
	pDlg->m_editRxStr.SetWindowText(Temp);

	return 0;
}

IMPLEMENT_DYNCREATE(CEthernetPage, CPropertyPage)

CEthernetPage::CEthernetPage() : CPropertyPage(CEthernetPage::IDD)
{
	//{{AFX_DATA_INIT(CEthernetPage)
	//}}AFX_DATA_INIT
	
	m_pEthernet = &m_Ethernet;
	m_nIPAddrs = theApp.m_LastSettings.nLaserIPAddrs;
	m_nWhich = 0;
	m_bClientServer = false;
	m_nPort = 20347;

//	m_pSocketAddrs = new CSocketAddr();
//	m_pAddrInfo = m_pSocketAddrs->GetAddrInfoList();

}

CEthernetPage::~CEthernetPage()
{
//	SAFE_DELETE(m_pSocketAddrs);
}

void CEthernetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEthernetPage)
	DDX_Control(pDX, IDC_EDIT_SEND, m_editSend);
	DDX_Control(pDX, IDC_EDIT_PORT, m_editPort);
	DDX_Control(pDX, IDC_COMBO_WHICH_ATTACHMENT, m_comboWhichAttachment);
	DDX_Control(pDX, IDC_COMBO_SERVER_CLIENT, m_comboClientServer);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_buttonConnect);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_ipAddress);
	DDX_Control(pDX, IDC_EDIT_RX, m_editRxStr);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_ADDRS, m_listAddrs);
}


BEGIN_MESSAGE_MAP(CEthernetPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEthernetPage)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVER_CLIENT, OnSelchangeComboServerClient)
	ON_CBN_SELCHANGE(IDC_COMBO_WHICH_ATTACHMENT, OnSelchangeComboWhichAttachment)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEthernetPage message handlers

BOOL CEthernetPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_comboWhichAttachment.AddString(L"Local");
	m_comboWhichAttachment.AddString(L"Ultrasonics");
	
	m_comboClientServer.AddString(L"Client");
	m_comboClientServer.AddString(L"Server");

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEthernetPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1,200,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CEthernetPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CEthernetPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	sockaddr_in Server;
	Server.sin_addr.s_addr = m_nIPAddrs;
	m_ipAddress.SetAddress(Server.sin_addr.s_addr); 

	m_comboWhichAttachment.SetCurSel(m_nWhich);
	m_comboClientServer.SetCurSel(m_bClientServer);

	Buff.Format(L"%d",m_nPort);
	m_editPort.SetWindowText(Buff);

	if(m_bClientServer == false) {
		m_ipAddress.EnableWindow(true);
	} else {
		m_ipAddress.EnableWindow(false);
	};


}

void CEthernetPage::OnButtonConnect() 
{
	sockaddr_in Server;
	CString Buff;

	m_ipAddress.GetAddress(Server.sin_addr.s_addr); 
	theApp.m_LastSettings.nLaserIPAddrs = m_nIPAddrs = Server.sin_addr.s_addr;

	m_editPort.GetWindowText(Buff);
	m_nPort = _ttoi(Buff);

	if(m_pEthernet->Connected() == false) {
		m_pEthernet->Connect(this,m_bClientServer,EthernetCallbackLocal,m_nIPAddrs,m_nPort);
	} else {
		m_pEthernet->Disconnect();
	}

	UpdateAllControls();
}


void CEthernetPage::OnSelchangeComboServerClient() 
{

	if(m_pEthernet->Disconnect() == true) {

		m_bClientServer = m_comboClientServer.GetCurSel() & 0x01;
		UpdateAllControls();
	}
	
}

void CEthernetPage::OnSelchangeComboWhichAttachment() 
{
	m_nWhich = m_comboWhichAttachment.GetCurSel();
	switch(m_nWhich) {
	case 0: m_pEthernet = &m_Ethernet;
		break;
	case 1: m_pEthernet = &theApp.m_UtUser.m_Ethernet;
		break;
	}

	m_bClientServer = m_pEthernet->m_bServer;
	m_nPort = m_pEthernet->m_nPort;
	m_nIPAddrs = m_pEthernet->m_nRemoteIP;

	UpdateAllControls();
	
}


void CEthernetPage::OnButtonSend() 
{
	CString Buff;
	char str[500];
	int ii;
	
	m_editSend.GetWindowText(Buff);

	for(ii=0;ii<Buff.GetLength();ii++) {
		str[ii]=(char)Buff.GetAt(ii);
	}
	str[ii++]=0;
	m_pEthernet->SendMessage(ii,str);
	m_editSend.SetWindowText(L"");
}

void CEthernetPage::OnTimer(UINT nIDEvent) 
{
	static bool m_bConnected   = false;
	
	if(m_bConnected != m_pEthernet->Connected()) {
		if((m_bConnected = m_pEthernet->Connected()) == true) {
			m_buttonConnect.SetWindowText(_T("Connected / Disconnect"));
		} else {
			m_buttonConnect.SetWindowText(_T("Connect"));
		}
	}

	CPropertyPage::OnTimer(nIDEvent);
}

void CEthernetPage::OnButtonClear() 
{

	m_editRxStr.SetWindowText(L"");
}

BOOL CEthernetPage::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case 13: OnButtonSend();
			return TRUE;
			break;
		}
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}
