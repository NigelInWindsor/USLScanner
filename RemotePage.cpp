// RemotePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RemotePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemotePage property page

IMPLEMENT_DYNCREATE(CRemotePage, CPropertyPage)

CRemotePage::CRemotePage() : CPropertyPage(CRemotePage::IDD)
{
	//{{AFX_DATA_INIT(CRemotePage)
	//}}AFX_DATA_INIT
	
	RestoreFromTank();	
}
	

CRemotePage::~CRemotePage()
{
}

void CRemotePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemotePage)
	DDX_Control(pDX, IDC_CHECK_STARTSERVER, m_startServerOnStartup);
	DDX_Control(pDX, IDC_EDIT_COMPUTERNAME, m_editComputerName);
	DDX_Control(pDX, IDC_SPIN_PORT, m_spinPort);
	DDX_Control(pDX, IDC_EDIT_PORT, m_editPort);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_editStatus);
	DDX_Control(pDX, IDC_COMBO_CON_TYPE, m_comboBoxConType);
	DDX_Control(pDX, IDC_BUTTON_STOP_SERVER, m_buttonStopServer);
	DDX_Control(pDX, IDC_BUTTON_START_SERVER, m_buttonStartServer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemotePage, CPropertyPage)
	//{{AFX_MSG_MAP(CRemotePage)
	ON_BN_CLICKED(IDC_BUTTON_START_SERVER, OnButtonStartServer)
	ON_BN_CLICKED(IDC_BUTTON_STOP_SERVER, OnButtonStopServer)
	ON_EN_CHANGE(IDC_EDIT_PORT, OnChangeEditPort)
	ON_CBN_SELCHANGE(IDC_COMBO_CON_TYPE, OnSelchangeComboConType)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PORT, OnDeltaposSpinPort)
	ON_BN_CLICKED(IDC_CHECK_STARTSERVER, OnCheckStartserver)
	//}}AFX_MSG_MAP
	ON_MESSAGE(RMESSAGE_WM_REMOTEUPDATE, OnRMessageRemoteUpdate)
	ON_MESSAGE(REMOTE_WM_UPDATECONNECTIONSTATUS, OnRemoteUpdateConnectionStatus)
	ON_MESSAGE(REMOTE_PAGE_UPDATE_CONTROLS, OnRemotePageUpdateControls)
END_MESSAGE_MAP()


void SetNSent(int val){
	//m_ListNSentString=CString(val);
}

/////////////////////////////////////////////////////////////////////////////
// CRemotePage message handlers


void CRemotePage::OnButtonStartServer() 
{
	StartServer();
}

void CRemotePage::StartServer(){
	if(theApp.m_pRemote == NULL){
		return;
	}
	//Choose the typoe of connection (e.g. bluetooth, tcpip).
	theApp.m_pRemote->SetConType(m_conType);
	//Choose the port number for the connection and start the server.
	theApp.m_pRemote->SetPort(m_iPort);
	theApp.m_pRemote->SetRestart(true); //Keep connection open if client disconnects.

//	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
//	pFrame->SendMessage(REMOTE_START_SERVER);
	AfxGetApp()->m_pMainWnd->SendMessage(REMOTE_START_SERVER);
}

void CRemotePage::StartServerUpdateControls(){

	m_editPort.EnableWindow(false);
	m_spinPort.EnableWindow(false);
//	m_comboBoxConType.EnableWindow(false); Only allow bluetooth.
	m_buttonStartServer.EnableWindow(false);

	m_buttonStopServer.EnableWindow(true);
	SetAddress();


	/*
	if(theApp.m_pRemote->StartServer()){ //Check for success starting server
		m_buttonStopServer.EnableWindow(true);
	} else {
		m_buttonStartServer.EnableWindow(true);
	}
	*/
}

void CRemotePage::OnButtonStopServer() 
{
	m_buttonStopServer.EnableWindow(false);
	theApp.m_pRemote->SetRestart(false); //Dont keep connection open when client disconnects.
	if(theApp.m_pRemote->SuspendServer()){ //Check for success closing the server
		m_buttonStartServer.EnableWindow(true);
//	m_comboBoxConType.EnableWindow(true); Only allow bluetooth.
		m_editPort.EnableWindow(true);
		m_spinPort.EnableWindow(true);
		SetAddress(L"");
	} else {
		m_buttonStopServer.EnableWindow(true);
		SetAddress();
	}
	
}


BOOL CRemotePage::OnSetActive() 
{

	return CPropertyPage::OnSetActive();
}


BOOL CRemotePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	
	//Get the settings from the remote object
	CRemote* rem = theApp.m_pRemote;
	if(rem==NULL){
		rem = new CRemote();
	}
	
	
	UpdateConnectionStatus();


//	this->SendMessage(RMESSAGE_WM_REMOTEUPDATE);
	

	//Add the options to the connection types drop down list
	m_comboBoxConType.AddString(L"Bluetooth");
	m_comboBoxConType.SetItemData(0, CT_BLUETOOTH);
	m_comboBoxConType.AddString(L"TCP/IP");
	m_comboBoxConType.SetItemData(1, CT_TCPIP);
	m_conType=rem->GetConType();
	SetConType();

	//Initialise the computer name
	SetAddress();

	//Initialise the port number edit box
	m_spinPort.SetRange(0,1000);
	m_iPort=rem->GetPort();
	SetPort();

	if(m_bStartServerOnStartup){
		m_startServerOnStartup.SetCheck(BST_CHECKED);
//		StartServerUpdateControls();
	} else {
		m_startServerOnStartup.SetCheck(BST_UNCHECKED);
	}

	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CRemotePage::OnRMessageRemoteUpdate(WPARAM wparam, LPARAM lparam){
	WaitForSingleObject(CRemote::m_updateDlgSemaphore, INFINITE);
	ReleaseSemaphore(CRemote::m_updateDlgSemaphore,1,NULL); 
	return 0;
}

LRESULT CRemotePage::OnRemoteUpdateConnectionStatus(WPARAM wparam, LPARAM lparam){
	UpdateConnectionStatus();
	return 0;
}

void CRemotePage::UpdateConnectionStatus(){
	
	//Get the settings from the remote object
	CRemote* rem = theApp.m_pRemote;
	
//Set the state of the window according to the connection status.
	bool bOpen = false;
	if(rem != NULL) bOpen = rem->GetOpen();
	m_buttonStartServer.EnableWindow(!bOpen);
	m_buttonStopServer.EnableWindow(bOpen);

	if(bOpen){
		bool bConnected = false;
		if(rem != NULL) rem->GetConnected();
		if(bConnected) {m_editStatus.SetWindowText(L"Connected.");
		} else {
			m_editStatus.SetWindowText(L"Waiting for client...");
		}
	} else {
		m_editStatus.SetWindowText(L"Closed.");
	}

	SetAddress();
}

void CRemotePage::OnChangeEditPort() 
{
	CString strPort;
	int iLength = m_editPort.GetWindowTextLength();
	LPTSTR buf = strPort.GetBufferSetLength(iLength);
	int iLengthWin=m_editPort.GetWindowText(buf, iLength+1);
//	m_iPort = _wtoi((const unsigned short*)buf);
	m_iPort = _wtoi(buf);
	theApp.m_pRemote->SetPort(m_iPort);
}

//Sets the connection type member variable accoring to the list box.
void CRemotePage::SetConType(){
	if(m_conType == CT_BLUETOOTH){
		m_comboBoxConType.SetCurSel(0);
	} else {
		m_comboBoxConType.SetCurSel(1);
	}
}

void CRemotePage::OnSelchangeComboConType() 
{
	m_conType = (CON_TYPE)m_comboBoxConType.GetItemData(m_comboBoxConType.GetCurSel());
	theApp.m_pRemote->SetConType(m_conType);
}

void CRemotePage::OnDeltaposSpinPort(NMHDR* pNMHDR, LRESULT* pResult) {
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(pNMUpDown->iDelta>0){
		m_iPort++;
	} else {
		m_iPort--;
	}
	int lower, upper;
	m_spinPort.GetRange(lower, upper);
	m_spinPort.SetPos((lower+upper)/2);
	SetPort();
	*pResult = 0;
}

void CRemotePage::SetAddress(){
	CRemote* pRemote = theApp.m_pRemote;
	if(pRemote == NULL){
		theApp.m_pRemote = new CRemote();
		pRemote = theApp.m_pRemote;
	}
	SetAddress(pRemote->GetThisComputerName());
}

void CRemotePage::SetAddress(const wchar_t* sAddress) {
	m_editComputerName.SetWindowText(sAddress);
}

void CRemotePage::SetPort(){
	CString strPort;
	strPort.Format(L"%d",m_iPort);
	m_editPort.SetWindowText(strPort);
}

void CRemotePage::OnCheckStartserver() 
{
	m_bStartServerOnStartup = (m_startServerOnStartup.GetCheck() == BST_CHECKED);
	SaveToTank();
}

void CRemotePage::SaveToTank(){
	theApp.m_Tank.bStartRemoteServerOnStartup = m_bStartServerOnStartup;
}

void CRemotePage::RestoreFromTank(){
	m_bStartServerOnStartup = theApp.m_Tank.bStartRemoteServerOnStartup;
}

void CRemotePage::UpdateControls() {
	UpdateControls(theApp.m_pRemote->GetOpen());
}

void CRemotePage::UpdateControls(BOOL bOpen) {
	if (bOpen) {
		m_editPort.EnableWindow(false);
		m_spinPort.EnableWindow(false);
		//	m_comboBoxConType.EnableWindow(false); Only allow bluetooth.
		m_buttonStartServer.EnableWindow(false);

		m_buttonStopServer.EnableWindow(true);
		SetAddress();
	}
	else {
		m_editPort.EnableWindow(true);
		m_spinPort.EnableWindow(true);
		//	m_comboBoxConType.EnableWindow(false); Only allow bluetooth.
		m_buttonStartServer.EnableWindow(true);
		m_buttonStopServer.EnableWindow(false);
		SetAddress(L"");
	}
}

LRESULT CRemotePage::OnRemotePageUpdateControls(WPARAM Wp, LPARAM Lp)
{
	UpdateControls();
	return 0;
}