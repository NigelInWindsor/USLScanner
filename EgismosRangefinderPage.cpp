// EgismosRangefinderPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "EgismosRangefinderPage.h"
#include <sstream>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HANDLE CEgismosRangefinderPage::m_comsSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

/////////////////////////////////////////////////////////////////////////////
// CEgismosRangefinderPage property page

IMPLEMENT_DYNCREATE(CEgismosRangefinderPage, CPropertyPage)

CEgismosRangefinderPage::CEgismosRangefinderPage() : CPropertyPage(CEgismosRangefinderPage::IDD)
{
	//{{AFX_DATA_INIT(CEgismosRangefinderPage)
	//}}AFX_DATA_INIT

	
	m_pTimer = new CIntervalTimer();
}

CEgismosRangefinderPage::~CEgismosRangefinderPage()
{
	delete m_pTimer;
}

void CEgismosRangefinderPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEgismosRangefinderPage)
	DDX_Control(pDX, IDC_EDIT_TMIN, m_editTMin);
	DDX_Control(pDX, IDC_EDIT_TMEAN, m_editTMean);
	DDX_Control(pDX, IDC_EDIT_TMAX, m_editTMax);
	DDX_Control(pDX, IDC_EDIT_TLAST, m_editTLast);
	DDX_Control(pDX, IDC_EDIT_TERR, m_editTErr);
	DDX_Control(pDX, IDC_EDIT_DMIN, m_editDMin);
	DDX_Control(pDX, IDC_EDIT_DMEAN, m_editDMean);
	DDX_Control(pDX, IDC_EDIT_DMAX, m_editDMax);
	DDX_Control(pDX, IDC_EDIT_DLAST, m_editDLast);
	DDX_Control(pDX, IDC_EDIT_DERR, m_editDErr);
	DDX_Control(pDX, ID_BUTTON_DISCONNECT, m_buttonDisconnect);
	DDX_Control(pDX, ID_BUTTON_STOP, m_buttonStop);
	DDX_Control(pDX, ID_BUTTON_READSLAVEADDRESS, m_buttonReadSlaveAddress);
	DDX_Control(pDX, ID_BUTTON_READDEVICETYPE, m_buttonReadDeviceType);
	DDX_Control(pDX, ID_BUTTON_READDEVICESTATUS, m_buttonReadDeviceStatus);
	DDX_Control(pDX, ID_BUTTON_MEAS_ONCE, m_buttonMeasOnce);
	DDX_Control(pDX, ID_BUTTON_MEAS_CONT, m_buttonMeasCont);
	DDX_Control(pDX, ID_BUTTON_LASERON, m_buttonLaserOn);
	DDX_Control(pDX, ID_BUTTON_LASEROFF, m_buttonLaserOff);
	DDX_Control(pDX, ID_BUTTON_CONNECT, m_buttonConnect);
	DDX_Control(pDX, ID_BUTTON_BUZZERON, m_buttonBuzzerOn);
	DDX_Control(pDX, ID_BUTTON_BUZZEROFF, m_buttonBuzzerOff);
	DDX_Control(pDX, IDC_EDIT_RECEIVED, m_editReceived);
	DDX_Control(pDX, IDC_EDIT_INTERP, m_editInterp);
	DDX_Control(pDX, IDC_EDIT_SENT, m_editSent);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	DDX_Control(pDX, IDC_EDIT_RANGEMM, m_editRange_mm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEgismosRangefinderPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEgismosRangefinderPage)
	ON_BN_CLICKED(ID_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(ID_BUTTON_BUZZEROFF, OnButtonBuzzeroff)
	ON_BN_CLICKED(ID_BUTTON_BUZZERON, OnButtonBuzzeron)
	ON_BN_CLICKED(ID_BUTTON_LASEROFF, OnButtonLaseroff)
	ON_BN_CLICKED(ID_BUTTON_LASERON, OnButtonLaseron)
	ON_BN_CLICKED(ID_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(ID_BUTTON_READDEVICETYPE, OnButtonReaddevicetype)
	ON_BN_CLICKED(ID_BUTTON_READDEVICESTATUS, OnButtonReaddevicestatus)
	ON_BN_CLICKED(ID_BUTTON_READSLAVEADDRESS, OnButtonReadslaveaddress)
	ON_BN_CLICKED(ID_BUTTON_MEAS_CONT, OnButtonMeasCont)
	ON_BN_CLICKED(ID_BUTTON_MEAS_ONCE, OnButtonMeasOnce)
	ON_BN_CLICKED(ID_BUTTON_DISCONNECT2, OnButtonDisconnect)
	ON_CBN_EDITCHANGE(IDC_COMBO_PORT, OnEditchangeComboPort)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, OnSelchangeComboPort)
	ON_BN_CLICKED(ID_BUTTON_RESET_STATS, OnButtonResetStats)
	ON_BN_CLICKED(ID_BUTTON_DISCONNECT, OnButtonDisconnect)
	//}}AFX_MSG_MAP
	ON_MESSAGE(REMOTE_HANDLER, RemoteHandler)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEgismosRangefinderPage message handlers

void CEgismosRangefinderPage::OnButtonConnect() 
{
	if(m_pRF->Start()){
		m_buttonDisconnect.EnableWindow(true);
		m_buttonReadSlaveAddress.EnableWindow(true);
		m_buttonReadDeviceType.EnableWindow(true);
		m_buttonReadDeviceStatus.EnableWindow(true);
		m_buttonMeasOnce.EnableWindow(true);
		m_buttonMeasCont.EnableWindow(true);
		m_buttonLaserOn.EnableWindow(true);
		m_buttonLaserOff.EnableWindow(true);
		m_buttonConnect.EnableWindow(false);
		m_buttonBuzzerOn.EnableWindow(true);
		m_buttonBuzzerOff.EnableWindow(true);
	}
	

}

void CEgismosRangefinderPage::OnButtonBuzzeroff() 
{
	m_pRF->BuzzerOff();
	UpdateWriteOutput();
}

void CEgismosRangefinderPage::OnButtonBuzzeron() 
{
	m_pRF->BuzzerOn();
	UpdateWriteOutput();
}

void CEgismosRangefinderPage::OnButtonLaseroff() 
{
	m_pRF->LaserOff();
	UpdateWriteOutput();
}

void CEgismosRangefinderPage::OnButtonLaseron() 
{
	m_pRF->LaserOn();
	UpdateWriteOutput();
}

BOOL CEgismosRangefinderPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_pRF = theApp.m_pEgismosRangefinder;

	int iPort;
	CString sPort;
	for(int i=0; i<20; i++){
		iPort = i+1;
		sPort.Format(L"COM%d",iPort);
		m_comboPort.InsertString(-1,sPort);
	}
	CString defaultPort = L"COM4";
	m_comboPort.SelectString(0,defaultPort);
	if(m_pRF) m_pRF->SetPort(defaultPort);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEgismosRangefinderPage::OnKillActive() 
{
	return CPropertyPage::OnKillActive();
}


void CEgismosRangefinderPage::UpdateReadOutput(){
	CString s = m_pRF->GetReceived();
	int len = s.GetLength();
	LPTSTR received = s.GetBuffer(len);
	CString outString;
	CString s2 = TEXT("");
	std::stringstream ss;
	ss << std::hex << std::showbase;
	for(int i=0; i<len; i++){
		//ss << (int)bytes[i];
		//ss << " ";
		s2.Format(L"%X",received[i]);
		ss << s2;
		outString += s2;
		outString += " ";
	}
//	m_editReceived.SetWindowText(m_pRF->GetReceived());
	m_editReceived.SetWindowText(outString);
}

void CEgismosRangefinderPage::UpdateWriteOutput(){

	std::stringstream ss;
	ss << std::hex << std::showbase;
	int nBytes;
	char* bytes = m_pRF->GetBytesSent(&nBytes);
	CString outString;
	CString s = TEXT("");
	for(int i=0; i<nBytes; i++){
		//ss << (int)bytes[i];
		//ss << " ";
		s.Format(L"%X",bytes[i]);
	//	ss << s << " ";
		outString += s;
		outString += " ";
	}
	//ss << "\n\r";
//	m_editSent.SetWindowText(CString(ss.str().c_str()));
	m_editSent.SetWindowText(outString);
}

BOOL CEgismosRangefinderPage::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

void CEgismosRangefinderPage::OnButtonStop() 
{
	m_buttonReadSlaveAddress.EnableWindow(true);
	m_buttonReadDeviceType.EnableWindow(true);
	m_buttonReadDeviceStatus.EnableWindow(true);
	m_buttonMeasOnce.EnableWindow(true);
	m_buttonMeasCont.EnableWindow(true);
	m_buttonLaserOn.EnableWindow(true);
	m_buttonLaserOff.EnableWindow(true);
	m_buttonBuzzerOn.EnableWindow(true);
	m_buttonBuzzerOff.EnableWindow(true);
	m_buttonStop.EnableWindow(false);
	m_pRF->StopClear();
	UpdateWriteOutput();	
}

void CEgismosRangefinderPage::OnButtonReaddevicetype() 
{
	m_pRF->ReadDeviceType();
	UpdateWriteOutput();
}

void CEgismosRangefinderPage::OnButtonReaddevicestatus() 
{
	m_pRF->ReadDeviceStatus();
	UpdateWriteOutput();
	
}

void CEgismosRangefinderPage::OnButtonReadslaveaddress() 
{
	m_pRF->ReadSlaveAddress();
	UpdateWriteOutput();	
}

void CEgismosRangefinderPage::OnButtonMeasCont() 
{
	m_buttonReadSlaveAddress.EnableWindow(false);
	m_buttonReadDeviceType.EnableWindow(false);
	m_buttonReadDeviceStatus.EnableWindow(false);
	m_buttonMeasOnce.EnableWindow(false);
	m_buttonMeasCont.EnableWindow(false);
	m_buttonLaserOn.EnableWindow(false);
	m_buttonLaserOff.EnableWindow(false);
	m_buttonBuzzerOn.EnableWindow(false);
	m_buttonBuzzerOff.EnableWindow(false);
	m_buttonStop.EnableWindow(true);
	m_pTimer->Measure();
	m_pRF->MeasureCont();
	UpdateWriteOutput();
}

void CEgismosRangefinderPage::OnButtonMeasOnce() 
{
	m_pTimer->Measure();
	m_pRF->Measure();
	UpdateWriteOutput();
	
}

void CEgismosRangefinderPage::OnButtonDisconnect() 
{	
	if(m_pRF->Close()){
		m_buttonDisconnect.EnableWindow(false);
		m_buttonReadSlaveAddress.EnableWindow(false);
		m_buttonReadDeviceType.EnableWindow(false);
		m_buttonReadDeviceStatus.EnableWindow(false);
		m_buttonMeasOnce.EnableWindow(false);
		m_buttonMeasCont.EnableWindow(false);
		m_buttonLaserOn.EnableWindow(false);
		m_buttonLaserOff.EnableWindow(false);
		m_buttonConnect.EnableWindow(true);
		m_buttonBuzzerOn.EnableWindow(false);
		m_buttonBuzzerOff.EnableWindow(false);
	}
}

void CEgismosRangefinderPage::OnNoMeasurement() {
	m_editRange_mm.SetWindowText(L"NM");
}

LRESULT CEgismosRangefinderPage::RemoteHandler(WPARAM Wp, LPARAM Lp){
	
	int poll_timeout_ms = 0;
	

	switch (Wp){
		case REMOTE_EGISMOSRF_MEAS_REC:
			m_pTimer->Measure();
			//if(CheckPrint()){
			//	UpdateMeasOutput();
			UpdateReadOutput();
			UpdateMeasOutput();
		//	}
			break;
		case REMOTE_EGISMOSRF_ERR:
			//if(CheckPrint()){
			//	OnLaserError();
		//	}
			UpdateReadOutput();
			break;
		case REMOTE_EGISMOSRF_MISC:
			//if(CheckPrint()){
			//	UpdateMiscOutput();
			UpdateReadOutput();
		//	}
			break;
		default:
		//	if(CheckPrint()){
			//	UpdateMiscOutput();
		//	}
			UpdateReadOutput();
			break;
	}
	ReleaseSemaphore(m_comsSemaphore, 1, NULL);
	return 0;
}

void CEgismosRangefinderPage::UpdateMeasOutput(){
	CString formatted;
	float newDist = m_pRF->GetLastDistance();
	if(newDist != m_fDistance){
		m_fDistance = newDist;
		formatted.Format(L"%.0f",m_fDistance);
		m_editRange_mm.SetWindowText(formatted);
	}
	m_CStatsD.Add(m_fDistance);
	UpdateStats();
}

void CEgismosRangefinderPage::OnEditchangeComboPort() 
{
	CString sPort;
	m_comboPort.GetWindowText(sPort);
	m_pRF->SetPort(sPort);	
}

void CEgismosRangefinderPage::OnSelchangeComboPort() 
{
	CString sPort;
	m_comboPort.GetLBText(m_comboPort.GetCurSel(),sPort);
	m_pRF->SetPort(sPort);
}

void CEgismosRangefinderPage::OnButtonResetStats() 
{
	m_pTimer->Reset();	
	m_CStatsD.Reset();
}

void CEgismosRangefinderPage::UpdateStats(){
	CString Buff;
	Buff.Format(L"%f",m_pTimer->Stats()->Mean());
	m_editTMean.SetWindowText(Buff);
	Buff.Format(L"%f",m_pTimer->Stats()->STDev());
	m_editTErr.SetWindowText(Buff);
	Buff.Format(L"%f",m_pTimer->Stats()->Last());
	m_editTLast.SetWindowText(Buff);
	Buff.Format(L"%f",m_pTimer->Stats()->Min());
	m_editTMin.SetWindowText(Buff);
	Buff.Format(L"%f",m_pTimer->Stats()->Max());
	m_editTMax.SetWindowText(Buff);

	Buff.Format(L"%f",m_CStatsD.Mean());
	m_editDMean.SetWindowText(Buff);
	Buff.Format(L"%f",m_CStatsD.STDev());
	m_editDErr.SetWindowText(Buff);
	Buff.Format(L"%f",m_CStatsD.Last());
	m_editDLast.SetWindowText(Buff);
	Buff.Format(L"%f",m_CStatsD.Min());
	m_editDMin.SetWindowText(Buff);
	Buff.Format(L"%f",m_CStatsD.Max());
	m_editDMax.SetWindowText(Buff);
}
