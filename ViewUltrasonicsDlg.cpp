// ViewUltrasonicsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ViewUltrasonicsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewUltrasonicsDlg dialog


CViewUltrasonicsDlg::CViewUltrasonicsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,CData* pData,int nTS,int nImageType,int nImageNumber)
	: CDialog(CViewUltrasonicsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewUltrasonicsDlg)
	m_strAmplifier = _T("");
	m_strDacBlanking = _T("");
	m_strDacMode = _T("");
	m_strDacTriggerThreshold = _T("");
	m_strDacTrigger = _T("");
	m_strFilterRange = _T("");
	m_strFilterType = _T("");
	m_strPreampDamping = _T("");
	m_strPreampGain = _T("");
	m_strPreampMode = _T("");
	m_strPRF = _T("");
	m_strPulseVoltage = _T("");
	m_strPulseWidth = _T("");
	m_strReceiver = _T("");
	m_strTransmitter = _T("");
	m_staticBlankingText = _T("");
	m_strGateDelay = _T("");
	m_strGateThreshold = _T("");
	m_strGateWidth = _T("");
	m_strTOFMode = _T("");
	m_strGateMode = _T("");
	//}}AFX_DATA_INIT
	m_nTS = nTS;
	m_nImageType = nImageType;
	m_nImageNumber = nImageNumber;
	m_pData = pData;
	m_pDlg = pDlg;
	

	m_Units[0]="\x0b5s";
	m_Units[1]="mm";

}


void CViewUltrasonicsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewUltrasonicsDlg)
	DDX_Control(pDX, IDC_COMBO_GATE_NUMBER, m_comboGateNumber);
	DDX_Control(pDX, IDC_EDIT_THICKNESS_MODE, m_editThicknessMode);
	DDX_Control(pDX, IDC_EDIT_CENTRE_FREQ, m_editCentreFrequency);
	DDX_Control(pDX, IDC_EDIT_RECTIFIER_DECAY, m_editRectifierDecay);
	DDX_Control(pDX, IDC_EDIT_RECTIFIER_MODE, m_editRectifierMode);
	DDX_Control(pDX, IDC_EDIT1_POST_FILTER, m_editLogPostFilter);
	DDX_Control(pDX, IDC_EDIT_FILTER_CUTOFF, m_editCutOffPoint);
	DDX_Control(pDX, IDC_EDIT_LOG_GAIN, m_editLogGain);
	DDX_Control(pDX, IDC_EDIT_LIN_GAIN, m_editLinGain);
	DDX_Control(pDX, IDC_STATIC_GROUP_0, m_buttonGroup);
	DDX_Control(pDX, IDC_STATIC_TEXT_2, m_text2);
	DDX_Control(pDX, IDC_STATIC_TEXT_1, m_text1);
	DDX_Control(pDX, IDC_STATIC_TEXT_0, m_text0);
	DDX_Control(pDX, IDC_LIST_DAC_LIST, m_listDac);
	DDX_Text(pDX, IDC_EDIT_AMPLIFIER, m_strAmplifier);
	DDX_Text(pDX, IDC_EDIT_DAC_BLANKING, m_strDacBlanking);
	DDX_Text(pDX, IDC_EDIT_DAC_MODE, m_strDacMode);
	DDX_Text(pDX, IDC_EDIT_DAC_THRESHOLD, m_strDacTriggerThreshold);
	DDX_Text(pDX, IDC_EDIT_DAC_TRIGGER, m_strDacTrigger);
	DDX_Text(pDX, IDC_EDIT_FILTER_RANGE, m_strFilterRange);
	DDX_Text(pDX, IDC_EDIT_FILTER_TYPE, m_strFilterType);
	DDX_Text(pDX, IDC_EDIT_PREAMP_DAMPING, m_strPreampDamping);
	DDX_Text(pDX, IDC_EDIT_PREAMP_GAIN, m_strPreampGain);
	DDX_Text(pDX, IDC_EDIT_PREAMP_MODE, m_strPreampMode);
	DDX_Text(pDX, IDC_EDIT_PRF, m_strPRF);
	DDX_Text(pDX, IDC_EDIT_PULSE_VOLTAGE, m_strPulseVoltage);
	DDX_Text(pDX, IDC_EDIT_PULSE_WIDTH, m_strPulseWidth);
	DDX_Text(pDX, IDC_EDIT_RECEIVER, m_strReceiver);
	DDX_Text(pDX, IDC_EDIT_TRANSMITTER, m_strTransmitter);
	DDX_Text(pDX, IDC_STATIC_BLANKING_TEXT, m_staticBlankingText);
	DDX_Text(pDX, IDC_EDIT_GATE_DELAY, m_strGateDelay);
	DDX_Text(pDX, IDC_EDIT_GATE_THRESHOLD, m_strGateThreshold);
	DDX_Text(pDX, IDC_EDIT_GATE_WIDTH, m_strGateWidth);
	DDX_Text(pDX, IDC_EDIT1_TOF_MODE, m_strTOFMode);
	DDX_Text(pDX, IDC_EDIT1_GATE_MODE, m_strGateMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewUltrasonicsDlg, CDialog)
	//{{AFX_MSG_MAP(CViewUltrasonicsDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DAC_LIST, OnGetdispinfoListDacList)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE_NUMBER, OnSelchangeComboGateNumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewUltrasonicsDlg message handlers

void CViewUltrasonicsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CViewUltrasonicsDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CViewUltrasonicsDlg::IDD]=FALSE;
		
	CDialog::OnClose();
	DestroyWindow();
}

void CViewUltrasonicsDlg::OnDestroy() 
{
	

	if (CViewUltrasonicsDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CViewUltrasonicsDlg::IDD]);
	}

	CDialog::OnDestroy();
	*m_pDlg = NULL;			
}

BOOL CViewUltrasonicsDlg::OnInitDialog() 
{
	
	CString	Buff;
	CDialog::OnInitDialog();
	
	if (CViewUltrasonicsDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CViewUltrasonicsDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	CalculateGateNumber();

	CreateColumnList();

	
	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewUltrasonicsDlg::UpdateAllControls()
{
	
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_strPreampGain.Format(_T("%ddB"),-20 + 10 * m_pData->m_TS[m_nTS].Pr30.nPreampGain);

	switch(m_pData->m_TS[m_nTS].Pr30.nPreampMode) {
	case 0: m_strPreampMode.LoadString(IDS_Pulse_Echo);
		break;
	case 1:	m_strPreampMode.LoadString(IDS_Pitch_Catch);
		break;
	}
		
	switch(m_pData->m_TS[m_nTS].Pr30.nDamping) {
	case 0: m_strPreampDamping = "33R";
		break;
	case 1: m_strPreampDamping = "50R";
		break;
	case 2: m_strPreampDamping = "100R";
		break;
	case 3: m_strPreampDamping = "200R";
		break;
	}
	switch(m_pData->m_TS[m_nTS].Pr30.nOutputSelect) {
	case 0: m_strAmplifier.LoadString(IDS_RF);
		break;
	case 1: m_strAmplifier.LoadString(IDS_Rectified);
		break;
	case 2: m_strAmplifier.LoadString(IDS_Logorithmic);
		break;
	}

	m_strPulseWidth.Format(_T("%d"),m_pData->m_TS[m_nTS].Pr30.nPulseWidth);
	m_strPulseVoltage.Format(_T("%dV"),m_pData->m_TS[m_nTS].Pr30.nPulseVoltage);
	Buff.Format(_T("%.01f dB"),m_pData->m_TS[m_nTS].Pr30.fLogGain); m_editLogGain.SetWindowText(Buff);
	Buff.Format(_T("%.01f dB"),m_pData->m_TS[m_nTS].Pr30.fLinGain); m_editLinGain.SetWindowText(Buff);

	switch(m_pData->m_TS[m_nTS].Pr30.nLogPostDetectFilter) {
	case 0: m_editLogPostFilter.SetWindowText(_T("500KHz"));
		break;
	case 1: m_editLogPostFilter.SetWindowText(_T("1 MHz"));
		break;
	case 2: m_editLogPostFilter.SetWindowText(_T("2.5 MHz"));
		break;
	case 3: m_editLogPostFilter.SetWindowText(_T("5 MHz"));
		break;
	case 4: m_editLogPostFilter.SetWindowText(_T("10 MHz"));
		break;
	case 5: Buff.LoadString(IDS_WideBand); m_editLogPostFilter.SetWindowText(Buff);
		break;
	}


	switch(m_pData->m_TS[m_nTS].Pr30.nFilterType) {
	case 0: m_strFilterType.LoadString(IDS_Low_Pass);
		break;
	case 1: m_strFilterType.LoadString(IDS_Band_Pass_Narrow);
		break;
	case 2:	m_strFilterType.LoadString(IDS_Band_Pass_Wide);
		break;
	}

	switch(m_pData->m_TS[m_nTS].Pr30.nFilterType) {
	case 0:
		switch(m_pData->m_TS[m_nTS].Pr30.nLowPassRange) {
		case 0: m_strFilterRange=_T("2.5 MHz");
			break;
		case 1: m_strFilterRange = _T("5 MHz");
			break;
		case 2: m_strFilterRange = _T("10 MHz");
			break;
		case 3: m_strFilterRange = _T("15 MHz");
			break;
		case 4: m_strFilterRange.LoadString(IDS_Full);
			break;
		}
		break;
	case 1:
	case 2:
		switch(m_pData->m_TS[m_nTS].Pr30.nBandPassRange) {
		case 0: m_strFilterRange = _T("0.5-2 MHz");
			break;
		case 1: m_strFilterRange = _T("1-4 MHz");
			break;
		case 2: m_strFilterRange = _T("2-8 MHz");
			break;
		case 3: m_strFilterRange = _T("4-16 MHz");
			break;
		case 4: m_strFilterRange = _T("8-32 MHz");
			break;
		}
	}

	theApp.m_UtUser.Pr30CalculateRealWorldCentreFrequency(&m_pData->m_TS[m_nTS].Pr30,&Buff);
	m_editCentreFrequency.SetWindowText(Buff);


	switch(m_pData->m_TS[m_nTS].Pr30.nRectifierDetectionMode) {
	case 0:	Buff.LoadString(IDS_Positive);
		break;
	case 1: Buff.LoadString(IDS_Negative);
		break;
	case 2: Buff.LoadString(IDS_Full_Wave);
		break;
	}
	m_editRectifierMode.SetWindowText(Buff);

	Buff.Format(_T("%d %%"),m_pData->m_TS[m_nTS].Pr30.nRectifierDecayPercentage);
	m_editRectifierDecay.SetWindowText(Buff);
	

	switch(m_pData->m_TS[m_nTS].Pr30.nDacEnable) {
	case 0: m_strDacMode.LoadString(IDS_Off);
		break;
	case 1: m_strDacMode.LoadString(IDS_On);
		break;
	}
	switch(m_pData->m_TS[m_nTS].Pr30.nDacTriggerMode) {
	case 0: m_strDacTrigger.LoadString(IDS_Main_Bang);
		break;
	case 1: m_strDacTrigger.LoadString(IDS_Interface);
		break;
	}

	switch(m_pData->m_TS[m_nTS].Pr30.nLinTimeConstant) {
	case 0: m_editCutOffPoint.SetWindowText(_T("0.5 MHz"));
		break;
	case 1: m_editCutOffPoint.SetWindowText(_T("1.5 MHz"));
		break;
	}

	theApp.m_UtUser.Pr30DacBlankingString(&m_pData->m_TS[m_nTS].Pr30,&m_strDacBlanking);
	m_strDacTriggerThreshold.Format(_T("%d%%"),MulDiv(m_pData->m_TS[m_nTS].Pr30.nDacTriggerThreshold-128,100,127));
	m_strPRF.Format(_T("%d"),m_pData->m_Global.nPrf);


	m_strTransmitter = 	m_pData->m_Global.cChannelName[m_pData->m_TS[m_nTS].Pr30.nMuxTx-1];
	m_strReceiver = 	m_pData->m_Global.cChannelName[m_pData->m_TS[m_nTS].Pr30.nMuxRx-1];

	switch(m_pData->m_TS[m_nTS].Pr30.nDacTriggerMode) {
	case 0:	m_staticBlankingText.LoadString(IDS_Delay);
		break;
	case 1: m_staticBlankingText.LoadString(IDS_Blanking);
		break;
	}


	m_comboGateNumber.ResetContent();
	m_comboGateNumber.LimitText(sizeof m_pData->m_TS[m_nTS].Gate.cName[0] - 1);
	for(int gg=0;gg<8;gg++) {
		m_comboGateNumber.AddString(m_pData->m_TS[m_nTS].Gate.cName[gg]);
	}
	m_comboGateNumber.SetCurSel(m_nGate);

	switch(m_pData->m_TS[m_nTS].Gate.nGateMode[m_nGate]) {
	case 0: m_strGateMode.LoadString(IDS_Off);
		break;
	case 1: m_strGateMode.LoadString(IDS_Main_Bang);
		break;
	case 2: m_strGateMode.LoadString(IDS_Interface);
		break;
	}

	switch(theApp.m_Scope.m_nGateDelayUnits) {
	case 0: m_strGateDelay.Format(_T("%.02f %s"),(float)m_pData->m_TS[m_nTS].Gate.nNsDelay[m_nGate]/1000,m_Units[theApp.m_Scope.m_nGateDelayUnits]);
		break;
	case 1: m_strGateDelay.Format(_T("%.02f %s"),(float)(m_pData->m_TS[m_nTS].Gate.nNsDelay[m_nGate]*theApp.m_LastSettings.nMaterialVelocity)/2e6,m_Units[theApp.m_Scope.m_nGateDelayUnits]);
		break;
	}
	switch(theApp.m_Scope.m_nGateWidthUnits) {
	case 0: m_strGateWidth.Format(_T("%.02f %s"),(float)m_pData->m_TS[m_nTS].Gate.nNsWidth[m_nGate]/1000,m_Units[theApp.m_Scope.m_nGateWidthUnits]);
		break;
	case 1: m_strGateWidth.Format(_T("%.02f %s"),(float)(m_pData->m_TS[m_nTS].Gate.nNsWidth[m_nGate]*theApp.m_LastSettings.nMaterialVelocity)/2e6,m_Units[theApp.m_Scope.m_nGateWidthUnits]);
		break;
	}

	m_strGateThreshold.Format(_T("%d"),m_pData->m_TS[m_nTS].Gate.nThreshold[m_nGate]);
	switch(m_pData->m_TS[m_nTS].Gate.nTimsDetectionMode[m_nGate]) {
	case 0:	m_strTOFMode.LoadString(IDS_Off);
		break;
	case 1:	m_strTOFMode.LoadString(IDS_Rising_Edge);
		break;
	case 2:	m_strTOFMode.LoadString(IDS_Pos_Peak);
		break;
	case 3:	m_strTOFMode.LoadString(IDS_Last_Edge);
		break;
	case 4:	m_strTOFMode.LoadString(IDS_Last_Peak);
		break;
	}

	CString StartBuff,StopBuff;

	if(m_nTOF == 0) {
		Buff.LoadString(IDS_Water_Path);
	} else {
		switch(m_pData->m_TS[m_nTS].Gate.nTOFStartGate[m_nTOF]) {
		case 0: StartBuff.LoadString(IDS_Main_Bang);
			break;
		case 1: StartBuff.LoadString(IDS_Interface);
			break;
		default: StartBuff.Format(_T("%s"),m_pData->m_TS[m_nTS].Gate.cName[m_pData->m_TS[m_nTS].Gate.nTOFStartGate[m_nTOF] - 2]);
			break;
		}
		switch(m_pData->m_TS[m_nTS].Gate.nTOFStopGate[m_nTOF]) {
		case 0: StopBuff.LoadString(IDS_Interface);
			break;
		default: StopBuff.Format(_T("%s"),m_pData->m_TS[m_nTS].Gate.cName[m_pData->m_TS[m_nTS].Gate.nTOFStopGate[m_nTOF]]);
			break;
		}
		Buff.Format(_T("Thickness: %s -> %s"),StartBuff,StopBuff);
	}
	m_editThicknessMode.SetWindowText(Buff);



	UpdateData(FALSE);

	FillList();
	SetAccessPrivelage();

}

void CViewUltrasonicsDlg::CreateColumnList()
{
	
	CString ColumnName[3] = { L"#",L"Delay mm", L"Gain"};
	int ColumnWidth[3] = { 0,60,50 };
	CRect rr;

	ColumnName[1].LoadString(IDS_Delay_mm);
	ColumnName[2].LoadString(IDS_Gain);

	m_listDac.GetWindowRect(&rr);
	rr.right-=22;

	// Delete all of the columns.
	int nColumnCount = m_listDac.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listDac.DeleteColumn(0);
	}

	m_listDac.InsertColumn(0,ColumnName[0], LVCFMT_LEFT,0);
	m_listDac.InsertColumn(1,ColumnName[1], LVCFMT_LEFT,rr.Width()/2-1);
	m_listDac.InsertColumn(2,ColumnName[2], LVCFMT_LEFT,rr.Width()/2-2);

	m_listDac.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CViewUltrasonicsDlg::FillList()
{
	CString	Buff;

	m_listDac.DeleteAllItems();
	for (int ii=0;ii<16;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listDac.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
	m_listDac.EnsureVisible(0,FALSE);
	m_listDac.SetItemState(0 , LVIS_SELECTED,LVIS_SELECTED);
	m_listDac.SetFocus();

}

void CViewUltrasonicsDlg::OnGetdispinfoListDacList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	str[0]=0;
	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str,_T("%.02f"),m_pData->m_TS[m_nTS].Pr30.fDacDelay[m_pData->m_TS[m_nTS].Pr30.nDacTable][pDispInfo->item.iItem]);
		break;
	case 2:	swprintf_s(str,_T("%.02f"),m_pData->m_TS[m_nTS].Pr30.fDacGain[m_pData->m_TS[m_nTS].Pr30.nDacTable][pDispInfo->item.iItem]);
		break;
	}
	pDispInfo->item.pszText=str;	
	*pResult = 0;
}

void CViewUltrasonicsDlg::SetAccessPrivelage()
{
	CRect rrGroup,rrText,rrSpin,rrEdit;
	CSize size;
	CString Buff;

	m_buttonGroup.GetWindowRect(&rrGroup);
	ScreenToClient(&rrGroup);
	m_text0.GetWindowRect(&rrText);
	ScreenToClient(&rrText);
	int nTextLeft = rrText.left;
	int	nEditLeft = (rrGroup.left+rrGroup.right)/2-10;
	int	nTop = rrText.top;
	int	nHeight = 24;

	m_editLinGain.ShowWindow(FALSE);
	m_editLogGain.ShowWindow(FALSE);
	m_editCutOffPoint.EnableWindow(FALSE);
	m_editLogPostFilter.ShowWindow(FALSE);
	m_editRectifierMode.ShowWindow(FALSE);
	m_editRectifierDecay.ShowWindow(FALSE);
	m_text0.ShowWindow(FALSE);
	m_text1.ShowWindow(FALSE);
	m_text2.ShowWindow(FALSE);

	switch(m_pData->m_TS[m_nTS].Pr30.nOutputSelect) {
	case 0:
		Buff.LoadString(IDS_Linear); m_buttonGroup.SetWindowText(Buff);
		Buff.LoadString(IDS_Gain); m_text0.SetWindowText(Buff);
		m_text0.ShowWindow(TRUE);

		m_editLinGain.ShowWindow(TRUE);
		m_editLinGain.GetWindowRect(&rrEdit);
		m_editLinGain.SetWindowPos(NULL,nEditLeft,nTop,63,23,NULL);

		m_editLinGain.GetWindowRect(&rrEdit);
		ScreenToClient(&rrEdit);
		m_buttonGroup.SetWindowPos(NULL,rrGroup.left,rrGroup.top,rrGroup.Width(),rrEdit.bottom-rrGroup.top+4,NULL);
		m_editCutOffPoint.EnableWindow(TRUE);
		break;
	case 1:
		Buff.LoadString(IDS_Linear); m_buttonGroup.SetWindowText(Buff);
		Buff.LoadString(IDS_Gain); m_text0.SetWindowText(Buff);
		Buff.LoadString(IDS_Rectifier); m_text1.SetWindowText(Buff);
		m_text1.SetWindowPos(NULL,nTextLeft,nTop+nHeight,0,0,SWP_NOSIZE);
		Buff.LoadString(IDS_Envelope); m_text2.SetWindowText(Buff);
		m_text2.SetWindowPos(NULL,nTextLeft,nTop+nHeight*2,0,0,SWP_NOSIZE);
		m_text0.ShowWindow(TRUE);
		m_text1.ShowWindow(TRUE);
		m_text2.ShowWindow(TRUE);

		m_editLinGain.ShowWindow(TRUE);
		m_editLinGain.GetWindowRect(&rrEdit);
		m_editLinGain.SetWindowPos(NULL,nEditLeft,nTop,63,23,NULL);

		m_editRectifierMode.SetWindowPos(NULL,nEditLeft,nTop+nHeight,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);

		m_editRectifierDecay.ShowWindow(TRUE);
		m_editRectifierDecay.GetWindowRect(&rrEdit);
		m_editRectifierDecay.SetWindowPos(NULL,nEditLeft,nTop+nHeight*2,63,23,NULL);

		m_editRectifierDecay.GetWindowRect(&rrEdit);
		ScreenToClient(&rrEdit);
		m_buttonGroup.SetWindowPos(NULL,rrGroup.left,rrGroup.top,rrGroup.Width(),rrEdit.bottom-rrGroup.top+4,NULL);
		m_editCutOffPoint.EnableWindow(TRUE);
		break;
	case 2:
		Buff.LoadString(IDS_Log); m_buttonGroup.SetWindowText(Buff);
		Buff.LoadString(IDS_Gain); m_text0.SetWindowText(Buff);
		Buff.LoadString(IDS_Post_Filter); m_text1.SetWindowText(Buff);
		m_text1.SetWindowPos(NULL,nTextLeft,nTop+nHeight,0,0,SWP_NOSIZE);
		m_text0.ShowWindow(TRUE);
		m_text1.ShowWindow(TRUE);

		m_editLogGain.ShowWindow(TRUE);
		m_editLogGain.GetWindowRect(&rrEdit);
		m_editLogGain.SetWindowPos(NULL,nEditLeft,nTop,63,23,NULL);

		m_editLogPostFilter.SetWindowPos(NULL,nEditLeft,nTop+nHeight,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);

		m_editLogPostFilter.GetWindowRect(&rrEdit);
		ScreenToClient(&rrEdit);
		m_buttonGroup.SetWindowPos(NULL,rrGroup.left,rrGroup.top,rrGroup.Width(),rrEdit.bottom-rrGroup.top+4,NULL);
		break;
	}

	switch(m_pData->m_TS[m_nTS].Pr30.nFilterMode) {
	case 0:	m_editCentreFrequency.EnableWindow(FALSE);
		break;
	case 1:	m_editCentreFrequency.EnableWindow(TRUE);
		break;
	}

	if(m_nImageType == 0) {
		m_editThicknessMode.ShowWindow(FALSE);
	} else {
		m_editThicknessMode.ShowWindow(TRUE);
	}



}

void CViewUltrasonicsDlg::OnSelchangeComboGateNumber() 
{
	int nTemp = m_nGate;

	m_nGate = m_comboGateNumber.GetCurSel();
	if(nTemp - m_nGate) UpdateAllControls();
}

void CViewUltrasonicsDlg::CalculateGateNumber()
{
	int	nn,ii;
	m_nGate = 0;
	m_nTOF = 0;

	switch(m_nImageType) {
	case 0:
		for(nn=0,ii=0;nn<8;nn++) {
			if(m_pData->m_TS[m_nTS].Gate.nCollectAmp[nn]) {
				if(ii==m_nImageNumber) m_nGate = nn;
				ii++;
			}
		}
		break;
	case 1:
		for(nn=0,ii=0;nn<8;nn++) {
			if(m_pData->m_TS[m_nTS].Gate.nCollectTime[nn]) {
				if(ii==m_nImageNumber) {
					m_nGate = nn;
					m_nTOF = nn;
				}
				ii++;
			}
		}
		break;
	}


}
