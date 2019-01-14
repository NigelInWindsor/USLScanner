// RemoteInterfacePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RemoteInterfacePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteInterfacePage property page

IMPLEMENT_DYNCREATE(CRemoteInterfacePage, CPropertyPage)

CRemoteInterfacePage::CRemoteInterfacePage() : CPropertyPage(CRemoteInterfacePage::IDD)
{
	//{{AFX_DATA_INIT(CRemoteInterfacePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nIndex = 0;
}

CRemoteInterfacePage::~CRemoteInterfacePage()
{
}

void CRemoteInterfacePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteInterfacePage)
	DDX_Control(pDX, IDC_COMBO_INTERFACE, m_comboInterface);
	DDX_Control(pDX, IDC_COMBO_BT_DEVICES, m_comboBluetoothDevice);
	DDX_Control(pDX, IDC_COMBO_BLUETOOTH_CLIENT_SERVER, m_comboBTClientServer);
	DDX_Control(pDX, IDC_EDIT_BLUETOOTH_STATUS, m_editBTStatus);
	DDX_Control(pDX, IDC_COMBO_CLIENT_SERVER, m_comboClientServer);
	DDX_Control(pDX, IDC_LIST_PACKETS, m_listPackets);
	DDX_Control(pDX, IDC_EDIT_TX, m_editTx);
	DDX_Control(pDX, IDC_IPADDRESS_REMOTE, m_IPAddress);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_editStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteInterfacePage, CPropertyPage)
	//{{AFX_MSG_MAP(CRemoteInterfacePage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_PACKETS, OnGetdispinfoListPackets)
	ON_CBN_SELCHANGE(IDC_COMBO_CLIENT_SERVER, OnSelchangeComboClientServer)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_REMOTE, OnFieldchangedIpaddressRemote)
	ON_CBN_SELCHANGE(IDC_COMBO_BLUETOOTH_CLIENT_SERVER, OnSelchangeComboBluetoothClientServer)
	ON_CBN_SELCHANGE(IDC_COMBO_BT_DEVICES, OnSelchangeComboBtDevices)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE, OnSelchangeComboInterface)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteInterfacePage message handlers

BOOL CRemoteInterfacePage::OnInitDialog() 
{
	
	CPropertyPage::OnInitDialog();
	CString Buff;
	int ii;
	
	// TODO: Add extra initialization here
	

	Buff.Format(L"This is a client contacting port %s",theApp.m_RemoteInterface.getPortNumber());
	m_comboClientServer.AddString(Buff);
	Buff.Format(L"This is a Server with address %s",theApp.m_RemoteInterface.getHostAddress());
	m_comboClientServer.AddString(Buff);

	Buff.Format(L"Client");	m_comboBTClientServer.AddString(Buff);
	Buff.Format(L"Server");	m_comboBTClientServer.AddString(Buff);

	theApp.m_RemoteInterface.BTRefreshAvailableDevices();
	for(ii=0;ii<theApp.m_RemoteInterface.getBTAvailableDevicesSize();ii++) {
		Buff = theApp.m_RemoteInterface.getBTDeviceName(ii);
		m_comboBluetoothDevice.AddString(Buff);
	};

	Buff.LoadString(IDS_Disabled);	m_comboInterface.AddString(Buff);
	Buff.LoadString(IDS_Ethernet);	m_comboInterface.AddString(Buff);
	Buff.LoadString(IDS_Bluetooth);	m_comboInterface.AddString(Buff);

	CreateColumns();

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRemoteInterfacePage::OnSetActive() 
{
	

	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	m_bEthernetConnected = !theApp.m_RemoteInterface.isEthernetConnected();
	UpdateAllControls();
	SetTimer(1,100,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CRemoteInterfacePage::OnKillActive() 
{

	KillTimer(1);
	return CPropertyPage::OnKillActive();
}

void CRemoteInterfacePage::OnTimer(UINT nIDEvent) 
{
	
	
	switch(theApp.m_LastSettings.bEthernetRemoteClientServer) {
	case false:
		if(m_bEthernetConnected != theApp.m_RemoteInterface.isEthernetConnected()) {
			if((m_bEthernetConnected = theApp.m_RemoteInterface.isEthernetConnected()) == true) {
				m_editStatus.SetWindowText(_T("Connected to a server"));
			} else {
				m_editStatus.SetWindowText(_T("Not Connected to a server"));
			}
		}
		break;
	case true:
		if(m_bEthernetConnected != theApp.m_RemoteInterface.isEthernetConnected()) {
			if((m_bEthernetConnected = theApp.m_RemoteInterface.isEthernetConnected()) == true) {
				m_editStatus.SetWindowText(_T("Connected to a client"));
			} else {
				m_editStatus.SetWindowText(_T("Not Connected to a client"));
			}
		}
		break;
	}

	switch(theApp.m_LastSettings.bBluetoothRemoteClientServer) {
	case false:
		if(m_bBluetoothConnected != theApp.m_RemoteInterface.isBluetoothConnected()) {
			if((m_bBluetoothConnected = theApp.m_RemoteInterface.isBluetoothConnected()) == true) {
				m_editBTStatus.SetWindowText(_T("Connected to a server"));
			} else {
				m_editBTStatus.SetWindowText(_T("Not Connected to a server"));
			}
		}
		break;
	case true:
		if(m_bBluetoothConnected != theApp.m_RemoteInterface.isBluetoothConnected()) {
			if((m_bBluetoothConnected = theApp.m_RemoteInterface.isBluetoothConnected()) == true) {
				m_editBTStatus.SetWindowText(_T("Connected to a client"));
			} else {
				m_editBTStatus.SetWindowText(_T("Not Connected to a client"));
			}
		}
		break;
	}
	FillList();
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CRemoteInterfacePage::UpdateAllControls()
{
	
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	sockaddr_in Server;

	m_comboInterface.SetCurSel(	theApp.m_LastSettings.nRemoteInterfaceEthernetBluetooth );


	m_editStatus.SetReadOnly(TRUE);

	if(theApp.m_LastSettings.bEthernetRemoteClientServer == true) {
		m_IPAddress.EnableWindow(false);
		Server.sin_addr.s_addr = 0;
		m_IPAddress.SetAddress(Server.sin_addr.s_addr); 
	} else {
		m_IPAddress.EnableWindow(true);
		Server.sin_addr.s_addr = theApp.m_LastSettings.nRemoteServerIPAddrs;
		m_IPAddress.SetAddress(Server.sin_addr.s_addr); 
	}


	m_comboClientServer.SetCurSel(theApp.m_LastSettings.bEthernetRemoteClientServer);
	m_comboBTClientServer.SetCurSel(theApp.m_LastSettings.bBluetoothRemoteClientServer);
	m_comboBluetoothDevice.SetCurSel(theApp.m_LastSettings.nBluetoothDeviceNumber);


	SetAccessPrivelages();
}


void CRemoteInterfacePage::OnButtonSend() 
{
	

	CString Buff;

	m_editTx.GetWindowText(Buff);

	theApp.m_RemoteInterface.SendMessage(Buff + L"\r\n");

}


void CRemoteInterfacePage::CreateColumns()
{
	
	int ColumnWidth[10] = { 0,40,120,50,500,50,50,50,80,50};
	int ii;
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Dir);
	ColumnName[2].LoadString(IDS_Time);
	ColumnName[3].LoadString(IDS_Size);
	ColumnName[4].LoadString(IDS_Data);

	int nColumnCount = m_listPackets.GetHeaderCtrl()->GetItemCount();
	for (ii=0;ii < nColumnCount;ii++) {
		m_listPackets.DeleteColumn(0);
	}
	for (ii=0;ii<4;ii++) {
		m_listPackets.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listPackets.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth[ii]);
	m_listPackets.SetExtendedStyle( LVS_EX_FULLROWSELECT   );


}

void CRemoteInterfacePage::FillList()
{
	
	CString Buff;
	int ii;

	int nCount = m_listPackets.GetItemCount();
	int nPktL = theApp.m_RemoteInterface.GetPktCount();

	if((nCount - nPktL) && (nPktL>=0)) {
		if(nCount<nPktL) {
			for (int ii=nCount;ii<nPktL;ii++) {
				Buff.Format(_T("%d"),ii+1);
				m_listPackets.InsertItem(ii, Buff.GetBuffer(255), ii);
			};
		} else {
			for(ii=nCount-1;ii>=nPktL;ii--) {
				m_listPackets.DeleteItem(ii);
			}
		}
		m_listPackets.EnsureVisible(nPktL-1,FALSE);
		for(ii=0;ii<nPktL;ii++)
			m_listPackets.SetItemState(ii , 0,LVIS_SELECTED);
		m_listPackets.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
		m_listPackets.SetFocus();
	}
}

void CRemoteInterfacePage::OnGetdispinfoListPackets(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn;

	str[0]=0;

	if(theApp.m_RemoteInterface.GetPktCount() <= 0) return;

	nn = pDispInfo->item.iItem;

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:	swprintf_s(str,100,L"%s",(LPCWSTR)theApp.m_RemoteInterface.GetPktTxRx(nn));
		break;
	case 2:	swprintf_s(str,100,L"%s", (LPCWSTR)theApp.m_RemoteInterface.FormatPktTime(nn));
		break;
	case 3:	swprintf_s(str,100,L"%s", (LPCWSTR)theApp.m_RemoteInterface.GetPktDataSize(nn));
		break;
	case 4:	swprintf_s(str,100,L"%s", (LPCWSTR)theApp.m_RemoteInterface.FormatPktData(nn));
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CRemoteInterfacePage::OnSelchangeComboClientServer() 
{
	

	theApp.m_LastSettings.bEthernetRemoteClientServer = m_comboClientServer.GetCurSel() & 0x01;

	theApp.m_RemoteInterface.Close();
	theApp.m_RemoteInterface.Start();

	UpdateAllControls();

}

void CRemoteInterfacePage::OnFieldchangedIpaddressRemote(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	sockaddr_in Server;

	m_IPAddress.GetAddress(Server.sin_addr.s_addr); 
	theApp.m_LastSettings.nRemoteServerIPAddrs = Server.sin_addr.s_addr;
	
	*pResult = 0;
}

void CRemoteInterfacePage::OnSelchangeComboBluetoothClientServer() 
{
	

	theApp.m_LastSettings.bBluetoothRemoteClientServer = m_comboBTClientServer.GetCurSel() & 0x01;

	theApp.m_RemoteInterface.Close();
	theApp.m_RemoteInterface.Start();

	UpdateAllControls();
}

void CRemoteInterfacePage::OnSelchangeComboBtDevices() 
{
	

	theApp.m_RemoteInterface.Close();
	theApp.m_LastSettings.nBluetoothDeviceNumber= m_comboBluetoothDevice.GetCurSel();
	theApp.m_RemoteInterface.Start();
	
}


void CRemoteInterfacePage::OnSelchangeComboInterface() 
{
	

	theApp.m_RemoteInterface.Close();
	theApp.m_LastSettings.nRemoteInterfaceEthernetBluetooth = m_comboInterface.GetCurSel();
	SetAccessPrivelages();
	theApp.m_RemoteInterface.Start();

}

void CRemoteInterfacePage::SetAccessPrivelages()
{
	if (GetSafeHwnd() == NULL) return;
	
}
