// CardsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "CardsDlg.h"
#include "PermissionDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardsDlg dialog


CCardsDlg::CCardsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCardsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCardsDlg)
	//}}AFX_DATA_INIT
	m_bRestartADC=false;
	m_nWhichADC=0;
	m_nWhichPM30=0;
}


void CCardsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCardsDlg)
	DDX_Control(pDX, IDC_COMBO_UMBILICAL_CHANNEL, m_comboUmbilicalChannel);
	DDX_Control(pDX, IDC_COMBO_PR30_NUMBER, m_comboPM30Number);
	DDX_Control(pDX, IDC_COMBO_ADC_NUMBER, m_comboADCNumber);
	DDX_Control(pDX, IDC_BUTTON_RESET_MAILBOX, m_buttonResetMailbox);
	DDX_Control(pDX, IDC_CHECK_MAILBOX, m_checkMailBox);
	DDX_Control(pDX, IDC_CHECK_SI10, m_checkSi10);
	DDX_Control(pDX, IDC_EDIT_USBI2C, m_editUSBI2C);
	DDX_Control(pDX, IDC_CHECK_USBI2C, m_checkUSBI2C);
	DDX_Control(pDX, IDC_COMBO_ADC_INPUT2, m_comboADCInput2);
	DDX_Control(pDX, IDC_COMBO_ADC_INPUT1, m_comboADCInput1);
	DDX_Control(pDX, IDC_COMBO_MUX_VERSION, m_comboMuxVersion);
	DDX_Control(pDX, IDC_COMBO_PM30_ADDRESS, m_comboPM30Address_0);
	DDX_Control(pDX, IDC_COMBO_PR30_VERSION, m_comboPr30Version_0);
	DDX_Control(pDX, IDC_COMBO_PR30_PULSER, m_comboPr30Attached_0);
	DDX_Control(pDX, IDC_COMBO_PRF_SOURCE, m_comboPRFSource);
	DDX_Control(pDX, IDC_EDIT_DELAY_CLK, m_editDelayClkPeriod);
	DDX_Control(pDX, IDC_CHECK_PR30, m_checkPr30_0);
	DDX_Control(pDX, IDC_COMBO_ADC_TYPE, m_comboAdcCardType);
	DDX_Control(pDX, IDC_EDIT_ADC_MEMORY_ADDRS, m_editAdc200MemoryAddrs);
	DDX_Control(pDX, IDC_EDIT_DSP, m_editDspAddrs);
	DDX_Control(pDX, IDC_CHECK_DSP, m_checkDsp);
	DDX_Control(pDX, IDC_EDIT_MSC_ADDRS, m_editMscAddrs);
	DDX_Control(pDX, IDC_CHECK_MSC, m_checkMsc);
	DDX_Control(pDX, IDC_EDIT_PR50_ADDRS, m_editPr50Addrs);
	DDX_Control(pDX, IDC_EDIT_PR20_ADDRS, m_editPr20Addrs);
	DDX_Control(pDX, IDC_EDIT_MAILBOX_ADDRS, m_editMailBoxAddrs);
	DDX_Control(pDX, IDC_EDIT_ATP_ADDRS, m_editATPAddrs);
	DDX_Control(pDX, IDC_EDIT_ADC100_ADDRS, m_editAdc100Addrs);
	DDX_Control(pDX, IDC_CHECK_PR50, m_checkPr50);
	DDX_Control(pDX, IDC_CHECK_PR20, m_checkPr20);
	DDX_Control(pDX, IDC_CHECK_ATP_TIMS0, m_checkAtpTims0);
	DDX_Control(pDX, IDC_CHECK_ATP_GPD1, m_checkAtpGpd1);
	DDX_Control(pDX, IDC_CHECK_ATP_GPD0, m_checkAtpGpd0);
	DDX_Control(pDX, IDC_CHECK_ADC100, m_checkAdc100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCardsDlg, CDialog)
	//{{AFX_MSG_MAP(CCardsDlg)
	ON_BN_CLICKED(IDC_BUTTON_RESET_MAILBOX, OnButtonResetMailbox)
	ON_EN_CHANGE(IDC_EDIT_DELAY_CLK, OnChangeEditDelayClk)
	ON_CBN_SELCHANGE(IDC_COMBO_PM30_ADDRESS, OnSelchangeComboPm30Address)
	ON_CBN_SELCHANGE(IDC_COMBO_ADC_INPUT1, OnSelchangeComboAdcInput1)
	ON_CBN_SELCHANGE(IDC_COMBO_ADC_INPUT2, OnSelchangeComboAdcInput2)
	ON_CBN_SELCHANGE(IDC_COMBO_PR30_NUMBER, OnSelchangeComboPm30Number)
	ON_CBN_SELCHANGE(IDC_COMBO_ADC_NUMBER, OnSelchangeComboAdcNumber)
	ON_BN_CLICKED(IDC_CHECK_PR30, OnCheckPr30)
	ON_CBN_SELCHANGE(IDC_COMBO_PR30_VERSION, OnSelchangeComboPr30Version)
	ON_CBN_SELCHANGE(IDC_COMBO_PR30_PULSER, OnSelchangeComboPr30Pulser)
	ON_CBN_SELCHANGE(IDC_COMBO_UMBILICAL_CHANNEL, OnSelchangeComboUmbilicalChannel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCardsDlg message handlers

BOOL CCardsDlg::OnInitDialog() 
{
	CString	Buff;

	CDialog::OnInitDialog();
	
	m_checkPr30_0.SetCheck(theApp.m_LastSettings.bPr30Present[0]);
	m_checkPr20.SetCheck(theApp.m_LastSettings.bPr20Present);
	m_checkPr50.SetCheck(theApp.m_LastSettings.bPr50Present);
	m_checkAtpGpd0.SetCheck(theApp.m_LastSettings.bAtpGpd0Present);
	m_checkAtpGpd1.SetCheck(theApp.m_LastSettings.bAtpGpd1Present);
	m_checkAtpTims0.SetCheck(theApp.m_LastSettings.bAtpTims0Present);

	Buff.Format(_T("%02x h"),theApp.m_LastSettings.nPr20Addrs);
	m_editPr20Addrs.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_LastSettings.nPr50Addrs);
	m_editPr50Addrs.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_LastSettings.nAtpGpd0Addrs);
	m_editATPAddrs.SetWindowText(Buff);

	m_comboPM30Number.InsertString(0,_T("1"));
	m_comboPM30Number.InsertString(1,_T("2"));
	m_comboPM30Number.InsertString(2,_T("3"));
	m_comboPM30Number.InsertString(3,_T("4"));
	m_comboPM30Number.SetCurSel(m_nWhichPM30);

	m_comboPr30Attached_0.AddString(_T("PreAmp"));
	m_comboPr30Attached_0.AddString(_T("Multiplexer"));
	m_comboPr30Attached_0.AddString(_T("LSA"));
	m_comboPr30Attached_0.SetCurSel(theApp.m_LastSettings.nAttachedToPr30[0]);


	m_comboPr30Version_0.AddString(_T("1"));
	m_comboPr30Version_0.AddString(_T("2"));
	m_comboPr30Version_0.AddString(_T("3 High speed TVG"));
	m_comboPr30Version_0.SetCurSel(theApp.m_LastSettings.nPr30Version[0]);


	m_comboMuxVersion.AddString(_T("Version 1"));
	m_comboMuxVersion.AddString(_T("Version 2"));
	m_comboMuxVersion.AddString(_T("Version 3"));
	m_comboMuxVersion.SetCurSel(theApp.m_LastSettings.nMuxVersion);

	m_comboPRFSource.AddString(_T("PR20"));
	m_comboPRFSource.AddString(_T("PR30"));
	m_comboPRFSource.AddString(_T("ADC"));
	m_comboPRFSource.AddString(_T("DSP"));
	m_comboPRFSource.AddString(_T("UP15"));
	m_comboPRFSource.AddString(_T("SI10"));
	m_comboPRFSource.AddString(_T("PM40"));
	switch(theApp.m_UtUser.m_Global.nTriggerSource) {
	case USL_PR20: m_comboPRFSource.SetCurSel(0);
		break;
	case USL_PR30: m_comboPRFSource.SetCurSel(1);
		break;
	case USL_ADC200: m_comboPRFSource.SetCurSel(2);
		break;
	case USL_DSP200: m_comboPRFSource.SetCurSel(3);
		break;
	case USL_UP15: m_comboPRFSource.SetCurSel(4);
		break;
	case USL_SI10: m_comboPRFSource.SetCurSel(5);
		break;
	case USL_PR40: m_comboPRFSource.SetCurSel(6);
		break;
	}

	//PM30 Number 0

	m_comboPM30Address_0.InsertString(0,_T("0x8C"));
	m_comboPM30Address_0.InsertString(1,_T("0x8D (Default)"));
	m_comboPM30Address_0.InsertString(2,_T("0x8E"));
	m_comboPM30Address_0.InsertString(3,_T("0x8F"));
	switch(theApp.m_LastSettings.nPr30DestAddrs[0]) {
	case 0x8C:
		m_comboPM30Address_0.SetCurSel(0);
		break;
	default:
	case 0x8D:
		m_comboPM30Address_0.SetCurSel(1);
		break;
	case 0x8E:
		m_comboPM30Address_0.SetCurSel(2);
		break;
	case 0x8F:
		m_comboPM30Address_0.SetCurSel(3);
		break;
	}

	UpdateDataCollectionItems();
	UpdateCommsItems();
		
	m_comboUmbilicalChannel.AddString(L"1");
	m_comboUmbilicalChannel.AddString(L"2");
	m_comboUmbilicalChannel.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCardsDlg::UpdateDataCollectionItems()
{
	CString Buff;
	bool bADCPCI=false;
	theApp.m_LastSettings.bAdcPresent=false;
	int ii;

	/**********ADC STUFF***************/
	m_comboAdcCardType.AddString(_T("Adc100"));
	m_comboAdcCardType.AddString(_T("Adc200"));
	m_comboAdcCardType.AddString(_T("Acqiris DP211"));
	m_comboAdcCardType.AddString(_T("Acqiris U1071AHZ4"));
	m_comboAdcCardType.AddString(_T("ATS860"));
	m_comboAdcCardType.AddString(_T("ATS9462"));
	m_comboAdcCardType.AddString(_T("ATS9626"));
	switch(theApp.m_LastSettings.nAdcCardType) {
	case USL_ADC100:
		m_comboAdcCardType.SetCurSel(0);
		theApp.m_LastSettings.bAdcPresent=true;
		break;
	case USL_ADC200: 
		m_comboAdcCardType.SetCurSel(1);
		theApp.m_LastSettings.bAdcPresent=true;
		break;
	case Acqiris_DP210: 
		m_comboAdcCardType.SetCurSel(2);
		theApp.m_LastSettings.bAdcPresent=true;
		bADCPCI=true;
		break;
	case Acqiris_U1071AHZ4: 
		m_comboAdcCardType.SetCurSel(3);
		theApp.m_LastSettings.bAdcPresent=true;
		bADCPCI=true;
		break;
	case Alazar_860:
		m_comboAdcCardType.SetCurSel(4);
		theApp.m_LastSettings.bAdcPresent=true;
		bADCPCI=true;
		break;
	case Alazar_9462:
		m_comboAdcCardType.SetCurSel(5);
		theApp.m_LastSettings.bAdcPresent=true;
		bADCPCI=true;
		break;
	case Alazar_9626:
		m_comboAdcCardType.SetCurSel(6);
		theApp.m_LastSettings.bAdcPresent=true;
		bADCPCI=true;
		break;
	}

	//If its a pci card e have auto detected it
	if(bADCPCI) {
		m_comboAdcCardType.EnableWindow(false);
		m_editAdc100Addrs.EnableWindow(false);
		m_editAdc200MemoryAddrs.EnableWindow(false);
		m_checkAdc100.EnableWindow(false);

		m_comboADCInput1.EnableWindow(true);
		m_comboADCInput1.InsertString(0,_T("Unused"));
		m_comboADCInput1.InsertString(1,_T("PM30 PPA15"));
		m_comboADCInput1.InsertString(2,_T("PM30 MUX8"));
		m_comboADCInput1.InsertString(3,_T("PM30 LSA"));
		m_comboADCInput1.InsertString(4,_T("PM30 Aux Amp"));
		m_comboADCInput1.InsertString(5,_T("PR20/PR50"));
		m_comboADCInput1.SetCurSel(theApp.m_LastSettings.ADCInputDevice[0+(m_nWhichADC*2)]);

		m_comboADCInput2.EnableWindow(true);
		m_comboADCInput2.InsertString(0,_T("Unused"));
		m_comboADCInput2.InsertString(1,_T("PM30 PPA15"));
		m_comboADCInput2.InsertString(2,_T("PM30 MUX8"));
		m_comboADCInput2.InsertString(3,_T("PM30 LSA"));
		m_comboADCInput2.InsertString(4,_T("PM30 Aux Amp"));
		m_comboADCInput2.InsertString(5,_T("PR20/PR50"));
		m_comboADCInput2.SetCurSel(theApp.m_LastSettings.ADCInputDevice[1+(m_nWhichADC*2)]);
	}else{
		m_comboAdcCardType.EnableWindow(true);
		m_editAdc100Addrs.EnableWindow(true);
		m_editAdc200MemoryAddrs.EnableWindow(true);
		m_checkAdc100.EnableWindow(true);
		Buff.Format(_T("%02x h"),theApp.m_LastSettings.nAdcAddrs);
		m_editAdc100Addrs.SetWindowText(Buff);
		Buff.Format(_T("%02x h"),theApp.m_LastSettings.nAdcMemoryAddrs);
		m_editAdc200MemoryAddrs.SetWindowText(Buff);
		m_comboADCInput1.EnableWindow(false);
		m_comboADCInput2.EnableWindow(false);
	}
	m_checkAdc100.SetCheck(theApp.m_LastSettings.bAdcPresent);

	for(ii=0;ii<theApp.m_nNumADCs;ii++){
		Buff.Format(_T("%d"),ii+1);
		m_comboADCNumber.InsertString(ii,Buff);
	}

	if(theApp.m_nNumADCs<=1) {
		m_comboADCNumber.EnableWindow(false);
	}else{
		m_comboADCNumber.EnableWindow(true);
	}

	m_comboADCNumber.SetCurSel(m_nWhichADC);

	/*******************END ADC STUFF******************************/


	/********************DSP STUFF*********************************/
	//If we have a PCIADC then we dont have a DSP
	if(bADCPCI) {
		theApp.m_LastSettings.bDspPresent=false;
		m_checkDsp.EnableWindow(false);

		m_editDspAddrs.EnableWindow(false);

		m_editDelayClkPeriod.EnableWindow(false);

	}else{
		m_checkDsp.EnableWindow(true);

		m_editDspAddrs.EnableWindow(true);
		Buff.Format(_T("%02x h"),theApp.m_LastSettings.nDspAddrs);
		m_editDspAddrs.SetWindowText(Buff);

		m_editDelayClkPeriod.EnableWindow(true);
		Buff.Format(_T("%d ns"),theApp.m_LastSettings.nAdcDelayClkPeriod);
		m_editDelayClkPeriod.SetWindowText(Buff);
	}

	m_checkDsp.SetCheck(theApp.m_LastSettings.bDspPresent);

	/*******************END DSP STUFF *******************************/



}

void CCardsDlg::UpdateCommsItems() 
{
	CString Buff;


	if(theApp.m_Si10User.IsConnected()) {
		m_checkSi10.SetCheck(true);

		/*If Si10 Present system will assume that is only comms device so disable others*/
		m_checkUSBI2C.EnableWindow(false);
		m_checkUSBI2C.SetCheck(false);

		m_editUSBI2C.EnableWindow(false);

		m_checkMsc.EnableWindow(false);

		m_editMscAddrs.EnableWindow(false);

		m_checkMailBox.EnableWindow(false);

		m_editMailBoxAddrs.EnableWindow(false);

		m_buttonResetMailbox.EnableWindow(false);

	}else{
		m_checkSi10.SetCheck(false);

		m_checkUSBI2C.EnableWindow(true);
		m_checkUSBI2C.SetCheck(theApp.m_LastSettings.bUSBI2CPresent);
		
		m_editUSBI2C.EnableWindow(true);
		Buff.Format(_T("Com%d"),theApp.m_LastSettings.nUSBI2CComNum);
		m_editUSBI2C.SetWindowText(Buff);

		m_checkMsc.EnableWindow(true);
		m_checkMsc.SetCheck(theApp.m_LastSettings.bMscPresent);

		m_editMscAddrs.EnableWindow(true);
		Buff.Format(_T("%02x h"),theApp.m_LastSettings.nMscAddrs);
		m_editMscAddrs.SetWindowText(Buff);

		m_checkMailBox.EnableWindow(true);
		//m_checkMailBox.setCheck... //TODO

		m_editMailBoxAddrs.EnableWindow(true);
		Buff.Format(_T("%02x h"),theApp.m_LastSettings.nMailBoxAddrs);
		m_editMailBoxAddrs.SetWindowText(Buff);

		m_buttonResetMailbox.EnableWindow(true);
	}

	
	/*Always automatic*/
	m_checkSi10.EnableWindow(false);
		

}

void CCardsDlg::OnOK() 
{
	CString	Buff;
	CMainFrame *pFrame =   (CMainFrame*)AfxGetApp()->m_pMainWnd;


	theApp.m_LastSettings.bPr20Present=m_checkPr20.GetCheck()&0x0001;
	theApp.m_LastSettings.bPr50Present=m_checkPr50.GetCheck()&0x0001;
	theApp.m_LastSettings.bAtpGpd0Present=m_checkAtpGpd0.GetCheck()&0x0001;
	theApp.m_LastSettings.bAtpGpd1Present=m_checkAtpGpd1.GetCheck()&0x0001;
	theApp.m_LastSettings.bAtpTims0Present=m_checkAtpTims0.GetCheck()&0x0001;
	theApp.m_LastSettings.bAdcPresent=m_checkAdc100.GetCheck()&0x0001;
	theApp.m_LastSettings.bDspPresent=m_checkDsp.GetCheck()&0x0001;
	theApp.m_LastSettings.bMscPresent=m_checkMsc.GetCheck()&0x0001;
	theApp.m_LastSettings.bUSBI2CPresent=m_checkUSBI2C.GetCheck()&0x0001;



/*
	if((theApp.m_LastSettings.nAttachedToPr30[0]==LSA) || (theApp.m_LastSettings.nAttachedToPr30[1]==LSA)){
		if(!theApp.m_LSA.FindDevice()) {
			Buff.Format(_T("Error No Comms to LSA"));
			MessageBox(Buff,_T("Error"), MB_ICONERROR);
		}
	}
*/

	
	m_editAdc100Addrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_LastSettings.nAdcAddrs);
	m_editAdc200MemoryAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_LastSettings.nAdcMemoryAddrs);
	m_editPr20Addrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_LastSettings.nPr20Addrs);
	m_editPr50Addrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_LastSettings.nPr50Addrs);
	m_editMailBoxAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_LastSettings.nMailBoxAddrs);
	m_editMscAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_LastSettings.nMscAddrs);
	m_editDspAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_LastSettings.nDspAddrs);
	m_editATPAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_LastSettings.nAtpGpd0Addrs);
	m_editUSBI2C.GetWindowText(Buff);
	swscanf_s(Buff,_T("Com%d"),&theApp.m_LastSettings.nUSBI2CComNum);

	
	theApp.m_LastSettings.bAtpGpd1Present=FALSE;
	theApp.m_LastSettings.bAtpTims1Present=FALSE;

	int	nAddrs=theApp.m_LastSettings.nAtpGpd0Addrs;
	if(theApp.m_LastSettings.bAtpGpd1Present==TRUE) {
		nAddrs+=0x10;
		theApp.m_LastSettings.nAtpGpd1Addrs=nAddrs;
	}
	if(theApp.m_LastSettings.bAtpTims0Present==TRUE) {
		nAddrs+=0x10;
		theApp.m_LastSettings.nAtpTims0Addrs=nAddrs;
	}
	if(theApp.m_LastSettings.bAtpTims1Present==TRUE) {
		nAddrs+=0x10;
		theApp.m_LastSettings.nAtpTims1Addrs=nAddrs;
	}

	m_editDelayClkPeriod.GetWindowText(Buff);
	theApp.m_LastSettings.nAdcDelayClkPeriod = _ttoi(Buff);

	switch(m_comboAdcCardType.GetCurSel()) {
	case 0:theApp.m_LastSettings.nAdcCardType=USL_ADC100;
		break;
	case 1: theApp.m_LastSettings.nAdcCardType=USL_ADC200;
		break;
	case 2: theApp.m_LastSettings.nAdcCardType=Acqiris_DP210;
		break;
	}

	switch(m_comboPRFSource.GetCurSel()) {
	case 0:	theApp.m_UtUser.m_Global.nTriggerSource = USL_PR20;
		break;
	case 1:	theApp.m_UtUser.m_Global.nTriggerSource = USL_PR30;
		break;
	case 2:	theApp.m_UtUser.m_Global.nTriggerSource = USL_ADC200;
		break;
	case 3:	theApp.m_UtUser.m_Global.nTriggerSource = USL_DSP200;
		break;
	case 4:	theApp.m_UtUser.m_Global.nTriggerSource = USL_UP15;
		break;
	case 5: theApp.m_UtUser.m_Global.nTriggerSource = USL_SI10;
		break;
	case 6: theApp.m_UtUser.m_Global.nTriggerSource = USL_PR40;
		break;
	}

	for(int ii=0;ii<8;ii++) {
		if(m_comboPRFSource.GetCurSel()==USL_PR30) {
			theApp.m_UtUser.m_TS[ii].Pr30.nPrfSrc = 0;
			theApp.m_UtUser.Pr30Command1(&theApp.m_UtUser.m_TS[ii].Pr30);
		}else{
			theApp.m_UtUser.m_TS[ii].Pr30.nPrfSrc = 1;
			theApp.m_UtUser.Pr30Command1(&theApp.m_UtUser.m_TS[ii].Pr30);
		}
	}

	theApp.m_LastSettings.nMuxVersion=m_comboMuxVersion.GetCurSel();


	CPermissionDlg dlg;

	if((dlg.DoModal() == IDOK) && (dlg.m_bAcceptable == TRUE)) {
		theApp.SaveCardEntries();
		theApp.SaveRegistryVariables();
	}

	if(m_bRestartADC){
		theApp.m_UtUser.ResetMaxTimeSlots();
		theApp.m_UtUser.RefreshAllVariables(0);
		switch(theApp.m_LastSettings.nAdcCardType) {
		case Acqiris_DP210: 
		case Acqiris_U1071AHZ4: 
			theApp.m_Acqiris.SuspendThread();
			Sleep(2000);
			theApp.m_Acqiris.StartThread();
			break;
		case Alazar_860:
		case Alazar_9462:
		case Alazar_9626:
			theApp.m_Alazar.SuspendThread();
			Sleep(2000);
			theApp.m_Alazar.Configure(&theApp.m_UtUser.m_TS[0].Adc); //LSA has different output range to be looked at
			theApp.m_Alazar.StartThread();
			break;
		}
		theApp.m_UtUser.SetupOutputDeviceSettings();
	}

	//Need to update ultrasonics sheet
	if (pFrame->m_pUltrasonicsSheet != NULL) {
		pFrame->m_pUltrasonicsSheet->SendMessage(UPDATE_TIMESLOTS);
	}


	CDialog::OnOK();
}

void CCardsDlg::OnButtonResetMailbox() 
{
	theApp.m_UtUser.ResetMailbox();
//	Sleep(3000);
//	theApp.m_UtUser.Initialize(theApp.m_nJobNumber);
}

void CCardsDlg::OnChangeEditDelayClk() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}




void CCardsDlg::OnSelchangeComboAdcInput1() 
{
	switch(m_comboADCInput1.GetCurSel()) {
	case 0: theApp.m_LastSettings.ADCInputDevice[0+(m_nWhichADC*2)] = UNUSED;
		break;
	case 1: theApp.m_LastSettings.ADCInputDevice[0+(m_nWhichADC*2)] = PM30_PPA15;
		break;
	case 2: theApp.m_LastSettings.ADCInputDevice[0+(m_nWhichADC*2)] = PM30_MUX8;
		break;
	case 3: theApp.m_LastSettings.ADCInputDevice[0+(m_nWhichADC*2)] = PM30_LSA;
		break;
	case 4: theApp.m_LastSettings.ADCInputDevice[0+(m_nWhichADC*2)] = PM30_AA;
		break;
	case 5: theApp.m_LastSettings.ADCInputDevice[0+(m_nWhichADC*2)] = PR2050;
		break;
	}

	m_bRestartADC=true;
	MessageBox(_T("You need to restart the software for this change to take effect"),_T("Warning"),NULL);

}

void CCardsDlg::OnSelchangeComboAdcInput2() 
{
	switch(m_comboADCInput2.GetCurSel()) {
	case 0: theApp.m_LastSettings.ADCInputDevice[1+(m_nWhichADC*2)] = UNUSED;
		break;
	case 1: theApp.m_LastSettings.ADCInputDevice[1+(m_nWhichADC*2)] = PM30_PPA15;
		break;
	case 2: theApp.m_LastSettings.ADCInputDevice[1+(m_nWhichADC*2)] = PM30_MUX8;
		break;
	case 3: theApp.m_LastSettings.ADCInputDevice[1+(m_nWhichADC*2)] = PM30_LSA;
		break;
	case 4: theApp.m_LastSettings.ADCInputDevice[1+(m_nWhichADC*2)] = PM30_AA;
		break;
	case 5: theApp.m_LastSettings.ADCInputDevice[1+(m_nWhichADC*2)] = PR2050;
		break;
	}

	m_bRestartADC=true;
	MessageBox(_T("You need to restart the software for this change to take effect"),_T("Warning"),NULL);
	
}


void CCardsDlg::OnSelchangeComboPm30Number() 
{
	CString Buff;

	m_nWhichPM30 = m_comboPM30Number.GetCurSel();
	
	m_checkPr30_0.SetCheck(theApp.m_LastSettings.bPr30Present[m_nWhichPM30]);
	m_comboPr30Version_0.SetCurSel(theApp.m_LastSettings.nPr30Version[m_nWhichPM30]);
	m_comboPr30Attached_0.SetCurSel(theApp.m_LastSettings.nAttachedToPr30[m_nWhichPM30]);
	switch(theApp.m_LastSettings.nPr30DestAddrs[m_nWhichPM30]) {
	case 0x8C:
		m_comboPM30Address_0.SetCurSel(0);
		break;
	default:
	case 0x8D:
		m_comboPM30Address_0.SetCurSel(1);
		break;
	case 0x8E:
		m_comboPM30Address_0.SetCurSel(2);
		break;
	case 0x8F:
		m_comboPM30Address_0.SetCurSel(3);
		break;
	}
}


void CCardsDlg::OnSelchangeComboAdcNumber() 
{
	m_nWhichADC = m_comboADCNumber.GetCurSel();
	m_comboADCInput1.SetCurSel(theApp.m_LastSettings.ADCInputDevice[0+(m_nWhichADC*2)]);
	m_comboADCInput2.SetCurSel(theApp.m_LastSettings.ADCInputDevice[1+(m_nWhichADC*2)]);
}

void CCardsDlg::OnCheckPr30() 
{	
	theApp.m_LastSettings.bPr30Present[m_nWhichPM30]=m_checkPr30_0.GetCheck()&0x0001;
}

void CCardsDlg::OnSelchangeComboPr30Version() 
{
	theApp.m_LastSettings.nPr30Version[m_nWhichPM30]=m_comboPr30Version_0.GetCurSel();
}

void CCardsDlg::OnSelchangeComboPr30Pulser() 
{
	switch(m_comboPr30Attached_0.GetCurSel()) {
	default:
	case 0: theApp.m_LastSettings.nAttachedToPr30[m_nWhichPM30] = PPA15;
		break;
	case 1: theApp.m_LastSettings.nAttachedToPr30[m_nWhichPM30] = MUX8;
		break;
	case 2: theApp.m_LastSettings.nAttachedToPr30[m_nWhichPM30] = LSA;
		break;
	}	
}



void CCardsDlg::OnSelchangeComboPm30Address() 
{
	switch(m_comboPM30Address_0.GetCurSel()) {
	case 0:
		theApp.m_LastSettings.nPr30DestAddrs[m_nWhichPM30] = 0x8C;
		break;
	default:
	case 1:
		theApp.m_LastSettings.nPr30DestAddrs[m_nWhichPM30] = 0x8D;
		break;
	case 2:
		theApp.m_LastSettings.nPr30DestAddrs[m_nWhichPM30] = 0x8E;
		break;
	case 3:
		theApp.m_LastSettings.nPr30DestAddrs[m_nWhichPM30] = 0x8F;
		break;
	}
		
	m_bRestartADC=true;

}

void CCardsDlg::OnSelchangeComboUmbilicalChannel() 
{
	theApp.m_UtUser.Pr30UmbilicalSelect(&theApp.m_UtUser.m_TS[0].Pr30, m_comboUmbilicalChannel.GetCurSel());	
}
