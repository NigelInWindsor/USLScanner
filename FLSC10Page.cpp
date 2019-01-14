// FLSC10Page.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "FLSC10Page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFLSC10Page property page

HANDLE CFLSC10Page::m_comsSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

IMPLEMENT_DYNCREATE(CFLSC10Page, CPropertyPage)

CFLSC10Page::CFLSC10Page() : CPropertyPage(CFLSC10Page::IDD)
{
	//{{AFX_DATA_INIT(CFLSC10Page)
	//}}AFX_DATA_INIT
	m_pTimer = new CIntervalTimer();
	m_printModulo=1;
	m_nPrint=0;
	m_fDistance=-1;
	theApp.m_pFLSC10 = NULL;
}

CFLSC10Page::~CFLSC10Page()
{
	delete m_pTimer;
}

void CFLSC10Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFLSC10Page)
	DDX_Control(pDX, IDC_EDIT_MISC_RECEIVED, m_editMiscReceived);
	DDX_Control(pDX, IDC_EDIT_DMIN, m_editDMin);
	DDX_Control(pDX, IDC_EDIT_DMEAN, m_editDMean);
	DDX_Control(pDX, IDC_EDIT_DMAX, m_editDMax);
	DDX_Control(pDX, IDC_EDIT_DLAST, m_editDLast);
	DDX_Control(pDX, IDC_EDIT_DERR, m_editDErr);
	DDX_Control(pDX, IDC_COMBO_PMODULO, m_comboPrintModulo);
	DDX_Control(pDX, IDC_EDIT_TLAST, m_editTLast);
	DDX_Control(pDX, IDC_EDIT_TMIN, m_editTMin);
	DDX_Control(pDX, IDC_EDIT_TMEAN, m_editTMean);
	DDX_Control(pDX, IDC_EDIT_TMAX, m_editTMax);
	DDX_Control(pDX, IDC_EDIT_TERR, m_editTErr);
	DDX_Control(pDX, ID_BUTTON_STOPCLEAR, m_buttonStop);
	DDX_Control(pDX, ID_BUTTON_MEASURE_CONT, m_buttonMeasureCont);
	DDX_Control(pDX, ID_BUTTON_MEASURE, m_buttonMeasure);
	DDX_Control(pDX, ID_BUTTON_DISCONNECT, m_buttonDisconnect);
	DDX_Control(pDX, ID_BUTTON_CONNECT, m_buttonConnect);
	DDX_Control(pDX, IDC_EDIT_RECEIVED, m_editReceived);
	DDX_Control(pDX, IDC_EDIT_MEASURE, m_editMeasure);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFLSC10Page, CPropertyPage)
	//{{AFX_MSG_MAP(CFLSC10Page)
	ON_BN_CLICKED(ID_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(ID_BUTTON_DISCONNECT, OnButtonDisconnect)
	ON_BN_CLICKED(ID_BUTTON_MEASURE, OnButtonMeasure)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BUTTON_MEASURE_CONT, OnButtonMeasureCont)
	ON_BN_CLICKED(ID_BUTTON_STOPCLEAR, OnButtonStopclear)
	ON_CBN_EDITCHANGE(IDC_COMBO_PORT, OnEditchangeComboPort)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, OnSelchangeComboPort)
	ON_BN_CLICKED(ID_BUTTON_RESET_STATS, OnButtonResetStats)
	ON_CBN_EDITCHANGE(IDC_COMBO_PMODULO, OnEditchangeComboPmodulo)
	ON_CBN_SELCHANGE(IDC_COMBO_PMODULO, OnSelchangeComboPmodulo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(REMOTE_HANDLER, RemoteHandler)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFLSC10Page message handlers

void CFLSC10Page::OnButtonConnect() 
{
	if(theApp.m_pFLSC10->Start()){
		m_buttonDisconnect.EnableWindow(true);
		m_buttonMeasure.EnableWindow(true);
		m_buttonMeasureCont.EnableWindow(true);
		m_buttonConnect.EnableWindow(false);
	}	
}


void CFLSC10Page::OnButtonDisconnect() 
{
	if(theApp.m_pFLSC10->Close()){
		m_buttonConnect.EnableWindow(true);
		m_buttonDisconnect.EnableWindow(false);
		m_buttonMeasure.EnableWindow(false);
		m_buttonMeasureCont.EnableWindow(false);
		m_buttonStop.EnableWindow(false);
	}	
}

void CFLSC10Page::OnButtonMeasure() 
{
	m_nPrint=0;
	m_pTimer->Measure();
	theApp.m_pFLSC10->Measure();
}

BOOL CFLSC10Page::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	int iPort;
	CString sPort;
	for(int i=0; i<20; i++){
		iPort = i+1;
		sPort.Format(L"COM%d",iPort);
		m_comboPort.InsertString(-1,sPort);
	}
	CString defaultPort = L"COM8";
	m_comboPort.SelectString(0,defaultPort);
	if(theApp.m_pFLSC10) theApp.m_pFLSC10->SetPort(defaultPort);

	m_comboPrintModulo.InsertString(0,L"1");
	m_comboPrintModulo.InsertString(1,L"10");
	m_comboPrintModulo.InsertString(2,L"100");
	m_comboPrintModulo.InsertString(3,L"1000");
	m_comboPrintModulo.InsertString(4,L"10000");
	m_comboPrintModulo.InsertString(5,L"100000");
	m_comboPrintModulo.SetCurSel(0);
	OnEditchangeComboPmodulo();


	m_buttonDisconnect.EnableWindow(false);
	m_buttonMeasure.EnableWindow(false);
	m_buttonMeasureCont.EnableWindow(false);
	m_buttonStop.EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFLSC10Page::OnTimer(UINT nIDEvent) 
{
//	if(theApp.m_pFLSC10->Read()){
//		HandleMessage();
//	}
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CFLSC10Page::OnKillActive() 
{
	
	return CPropertyPage::OnKillActive();
}

BOOL CFLSC10Page::OnSetActive() 
{
	
	return CPropertyPage::OnSetActive();
}


void CFLSC10Page::UpdateErrorOutput(){
	CString buff;

	if (theApp.m_pFLSC10) buff = theApp.m_pFLSC10->GetReceived();
	if(!buff.IsEmpty()){
		m_editReceived.SetWindowText(buff);
	}
}

void CFLSC10Page::UpdateMiscOutput(){
	CString buff;
	if (theApp.m_pFLSC10) buff = theApp.m_pFLSC10->GetReceived();
	if(!buff.IsEmpty()){
		m_editMiscReceived.SetWindowText(buff);
	}
}

void CFLSC10Page::UpdateMeasOutput(){
	CString formatted;
	float newDist;

	if (theApp.m_pFLSC10) newDist = theApp.m_pFLSC10->GetLastDistance();
	if(newDist != m_fDistance){
		m_fDistance = newDist;
		formatted.Format(L"%.1f",m_fDistance);
		m_editMeasure.SetWindowText(formatted);
	}
	m_CStatsD.Add(m_fDistance);
	UpdateStats();
}

void CFLSC10Page::OnButtonMeasureCont() 
{
	m_nPrint=0;
	m_pTimer->Measure();
	if (theApp.m_pFLSC10) theApp.m_pFLSC10->MeasureCont();
	m_buttonMeasureCont.EnableWindow(false);
	m_buttonMeasure.EnableWindow(false);
	m_buttonStop.EnableWindow(true);
}

void CFLSC10Page::OnButtonStopclear() 
{
	if (theApp.m_pFLSC10) theApp.m_pFLSC10->StopClear();
	m_buttonMeasureCont.EnableWindow(true);
	m_buttonMeasure.EnableWindow(true);
	m_buttonStop.EnableWindow(false);
}

LRESULT CFLSC10Page::RemoteHandler(WPARAM Wp, LPARAM Lp){
	switch (Wp){
		case REMOTE_FLSC10_MEAS_REC:
			m_pTimer->Measure();
			//if(CheckPrint()){
				UpdateMeasOutput();
		//	}
			break;
		case REMOTE_FLSC10_ERR:
			//if(CheckPrint()){
				OnFLSC10Error();
		//	}
			break;
		case REMOTE_FLSC10_MISC:
			//if(CheckPrint()){
				UpdateMiscOutput();
		//	}
			break;
		default:
		//	if(CheckPrint()){
				UpdateMiscOutput();
		//	}
			break;
	}

	ReleaseSemaphore(m_comsSemaphore,1,NULL);
	return 0;
}

void CFLSC10Page::OnNoMeasurement() {
	m_editMeasure.SetWindowText(L"NM");
}

void CFLSC10Page::OnFLSC10Error(){
	if (theApp.m_pFLSC10) m_errorCode = theApp.m_pFLSC10->GetLastLaserError();

	UpdateErrorOutput();
	bool bWasCont = theApp.m_pFLSC10->IsRunningContinuously();

	int nRestartDelay_ms=100;

	switch(m_errorCode){
		case 330: //Distance jump too great in continuous measurement mode.Attempt another measurement.
			OnButtonStopclear(); //Will stop and clear if in continuous measurement mode.
			Sleep(nRestartDelay_ms);
			OnButtonMeasureCont();	
			break;
		case 260: //Unable to measure due to ambiguous targets. Try again if running continuously.
			if(bWasCont){
				OnButtonStopclear(); //Will stop and clear if in continuous measurement mode.
				Sleep(nRestartDelay_ms);	
				OnButtonMeasureCont();
			}
			break;
		default:
		
			break;		
	}
}

void CFLSC10Page::OnEditchangeComboPort() 
{
	CString sPort;
	m_comboPort.GetWindowText(sPort);
	if (theApp.m_pFLSC10) theApp.m_pFLSC10->SetPort(sPort);
}

void CFLSC10Page::OnSelchangeComboPort() 
{
	CString sPort;
	m_comboPort.GetLBText(m_comboPort.GetCurSel(),sPort);
	if(theApp.m_pFLSC10) theApp.m_pFLSC10->SetPort(sPort);
}

void CFLSC10Page::OnButtonResetStats() 
{
	m_pTimer->Reset();	
	m_CStatsD.Reset();
}

void CFLSC10Page::UpdateStats(){
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

bool CFLSC10Page::CheckPrint(){
	if(m_nPrint >= m_printModulo || m_nPrint==0){
		m_nPrint=1;
		return true;
	}
	m_nPrint++;
	return false;
}
void CFLSC10Page::OnEditchangeComboPmodulo() 
{
	CString Buff;
	m_comboPrintModulo.GetWindowText(Buff);
	m_printModulo = _ttoi(Buff);
}

void CFLSC10Page::OnSelchangeComboPmodulo() 
{
	OnEditchangeComboPmodulo();
	
}
