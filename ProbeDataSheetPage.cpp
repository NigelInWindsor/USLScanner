// ProbeDataSheetPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ProbeDataSheetPage.h"
#include "ProbeCharacterisationSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProbeDataSheetPage property page

IMPLEMENT_DYNCREATE(CProbeDataSheetPage, CResizablePage)

CProbeDataSheetPage::CProbeDataSheetPage(CProbe* pProbe, CProbeDataSheet* pDataSheet) : CResizablePage(CProbeDataSheetPage::IDD)
{
	//{{AFX_DATA_INIT(CProbeDataSheetPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pProbe = pProbe;
	m_pDataSheet = pDataSheet;
}

CProbeDataSheetPage::~CProbeDataSheetPage()
{
}

void CProbeDataSheetPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProbeDataSheetPage)
	DDX_Control(pDX, IDC_EDIT_TRANSDUCER_DESCRIPTION, m_editTransducerDescription);
	DDX_Control(pDX, IDC_SPIN_DB_DROP, m_spinAtAttenuation);
	DDX_Control(pDX, IDC_EDIT_DB_DROP, m_editAtAttenuation);
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
	DDX_Text(pDX, IDC_EDIT_PULSE_VOLTAGE, m_strPulseVoltage);
	DDX_Text(pDX, IDC_EDIT_PULSE_WIDTH, m_strPulseWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProbeDataSheetPage, CResizablePage)
	//{{AFX_MSG_MAP(CProbeDataSheetPage)
	ON_EN_CHANGE(IDC_EDIT0, OnChangeEdit0)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_LOCK, OnButtonLock)
	ON_EN_CHANGE(IDC_EDIT_DB_DROP, OnChangeEditDbDrop)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK_PULSE_DURATION, OnCheckPulseDuration)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProbeDataSheetPage message handlers

BOOL CProbeDataSheetPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	CStatic* pStatic;

	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC0)) pStatic->SetWindowText(m_pDataSheet->m_Fields.GetAt(0));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC1)) pStatic->SetWindowText(m_pDataSheet->m_Fields.GetAt(1));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC2)) pStatic->SetWindowText(m_pDataSheet->m_Fields.GetAt(2));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC3)) pStatic->SetWindowText(m_pDataSheet->m_Fields.GetAt(3));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC4)) pStatic->SetWindowText(m_pDataSheet->m_Fields.GetAt(4));

	m_spinAtAttenuation.SetRange(0,45);

	
	CreateColumnList();
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProbeDataSheetPage::UpdateAllControls()
{
	
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	CEdit* pEdit;
	CButton* pButton;
	int nTabs[10] = {0,60,60,60,60,60,60,60,60,60};
	int ii;

	for(ii=1;ii<10;ii++) {
		nTabs[ii] = nTabs[ii-1] + 160;
	}
	m_editTransducerDescription.SetTabStops(10,nTabs);

	for(ii=0;ii<3;ii++) {
		Buff += m_pProbe->m_Fields.GetAt(ii);
		Buff += ": ";
		Buff += m_pProbe->m_strValues.GetAt(ii);
		Buff += "\t";
	}
	Buff += "\r\n";
	for(;ii<6;ii++) {
		Buff += m_pProbe->m_Fields.GetAt(ii);
		Buff += ": ";
		Buff += m_pProbe->m_strValues.GetAt(ii);
		Buff += "\t";
	}
	Buff += "\r\n";
	for(ii=7;ii<9;ii++) {
		Buff += m_pProbe->m_Fields.GetAt(ii);
		Buff += ": ";
		Buff += m_pProbe->m_strValues.GetAt(ii);
		Buff += "\t";
	}

	m_editTransducerDescription.SetWindowText(Buff);

	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT0)) pEdit->SetWindowText(m_pDataSheet->m_strValues.GetAt(0));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT1)) pEdit->SetWindowText(m_pDataSheet->m_strValues.GetAt(1));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT2)) pEdit->SetWindowText(m_pDataSheet->m_strValues.GetAt(2));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT3)) pEdit->SetWindowText(m_pDataSheet->m_strValues.GetAt(3));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT4)) pEdit->SetWindowText(m_pDataSheet->m_strValues.GetAt(4));

	m_strPreampGain.Format(_T("%ddB"),-20 + 10 * m_pDataSheet->m_TS.Pr30.nPreampGain);

	switch(m_pDataSheet->m_TS.Pr30.nPreampMode) {
	case 0: m_strPreampMode.LoadString(IDS_Pulse_Echo);
		break;
	case 1:	m_strPreampMode.LoadString(IDS_Pitch_Catch);
		break;
	}
		
	switch(m_pDataSheet->m_TS.Pr30.nDamping) {
	case 0: m_strPreampDamping = "33R";
		break;
	case 1: m_strPreampDamping = "50R";
		break;
	case 2: m_strPreampDamping = "100R";
		break;
	case 3: m_strPreampDamping = "200R";
		break;
	}
	switch(m_pDataSheet->m_TS.Pr30.nOutputSelect) {
	case 0: m_strAmplifier.LoadString(IDS_RF);
		break;
	case 1: m_strAmplifier.LoadString(IDS_Rectified);
		break;
	case 2: m_strAmplifier.LoadString(IDS_Logorithmic);
		break;
	}

	m_strPulseWidth.Format(_T("%d"),m_pDataSheet->m_TS.Pr30.nPulseWidth);
	m_strPulseVoltage.Format(_T("%dV"),m_pDataSheet->m_TS.Pr30.nPulseVoltage);
	Buff.Format(_T("%.01f dB"),m_pDataSheet->m_TS.Pr30.fLogGain); m_editLogGain.SetWindowText(Buff);
	Buff.Format(_T("%.01f dB"),m_pDataSheet->m_TS.Pr30.fLinGain); m_editLinGain.SetWindowText(Buff);

	switch(m_pDataSheet->m_TS.Pr30.nLogPostDetectFilter) {
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


	switch(m_pDataSheet->m_TS.Pr30.nFilterType) {
	case 0: m_strFilterType.LoadString(IDS_Low_Pass);
		break;
	case 1: m_strFilterType.LoadString(IDS_Band_Pass_Narrow);
		break;
	case 2:	m_strFilterType.LoadString(IDS_Band_Pass_Wide);
		break;
	}

	switch(m_pDataSheet->m_TS.Pr30.nFilterType) {
	case 0:
		switch(m_pDataSheet->m_TS.Pr30.nLowPassRange) {
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
		switch(m_pDataSheet->m_TS.Pr30.nBandPassRange) {
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

	theApp.m_UtUser.Pr30CalculateRealWorldCentreFrequency(&m_pDataSheet->m_TS.Pr30,&Buff);
	m_editCentreFrequency.SetWindowText(Buff);


	switch(m_pDataSheet->m_TS.Pr30.nRectifierDetectionMode) {
	case 0:	Buff.LoadString(IDS_Positive);
		break;
	case 1: Buff.LoadString(IDS_Negative);
		break;
	case 2: Buff.LoadString(IDS_Full_Wave);
		break;
	}
	m_editRectifierMode.SetWindowText(Buff);

	Buff.Format(_T("%d %%"),m_pDataSheet->m_TS.Pr30.nRectifierDecayPercentage);
	m_editRectifierDecay.SetWindowText(Buff);
	

	switch(m_pDataSheet->m_TS.Pr30.nDacEnable) {
	case 0: m_strDacMode.LoadString(IDS_Off);
		break;
	case 1: m_strDacMode.LoadString(IDS_On);
		break;
	}
	switch(m_pDataSheet->m_TS.Pr30.nDacTriggerMode) {
	case 0: m_strDacTrigger.LoadString(IDS_Main_Bang);
		break;
	case 1: m_strDacTrigger.LoadString(IDS_Interface);
		break;
	}

	switch(m_pDataSheet->m_TS.Pr30.nLinTimeConstant) {
	case 0: m_editCutOffPoint.SetWindowText(_T("0.5 MHz"));
		break;
	case 1: m_editCutOffPoint.SetWindowText(_T("1.5 MHz"));
		break;
	}

	theApp.m_UtUser.Pr30DacBlankingString(&m_pDataSheet->m_TS.Pr30,&m_strDacBlanking);
	m_strDacTriggerThreshold.Format(_T("%d%%"),MulDiv(m_pDataSheet->m_TS.Pr30.nDacTriggerThreshold-128,100,127));
//	m_strPRF.Format(_T("%d"),m_pData->m_Global.nPrf);
//	m_strTransmitter = 	m_pData->m_Global.cChannelName[m_pDataSheet->m_TS.Pr30.nMuxTx-1];
//	m_strReceiver = 	m_pData->m_Global.cChannelName[m_pDataSheet->m_TS.Pr30.nMuxRx-1];

	Buff.Format(_T("%.0f dB"),theApp.m_LastSettings.fProbeAtAttenuation);
	m_editAtAttenuation.SetWindowText(Buff);
	m_spinAtAttenuation.SetPos((int)fabs(theApp.m_LastSettings.fProbeAtAttenuation));

	m_pDataSheet->CalculateStats(theApp.m_LastSettings.fProbeAtAttenuation);

	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_PULSE_DURATION)) {
		m_pDataSheet->GetPulseDuration(Buff);
		pButton->SetWindowText(_T("Pulse duration : ") + Buff);
		pButton->SetCheck(theApp.m_LastSettings.bProbePulseDuration);
	}
	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_PEAK_FREQUENCY)) {
		m_pDataSheet->GetPeakFrequency(Buff);
		pButton->SetWindowText(_T("Peak Frequency : ") + Buff);
		pButton->SetCheck(theApp.m_LastSettings.bProbePeakFrequency);
	}
	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_CENTRAL_FREQUENCY)) {
		m_pDataSheet->GetCentralFrequency(Buff);
		pButton->SetWindowText(_T("Central Frequency : ") + Buff);
		pButton->SetCheck(theApp.m_LastSettings.bProbeCentralFrequency);
	}
	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_BANDWIDTH)) {
		m_pDataSheet->GetBandWidth(Buff);
		pButton->SetWindowText(_T("Bandwidth : ") + Buff);
		pButton->SetCheck(theApp.m_LastSettings.bProbeBandWidth);
	}
	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_RATIO_BW_CF)) {
		m_pDataSheet->GetRatioBWCF(Buff);
		pButton->SetWindowText(_T("Ratio BW/CF : ") + Buff);
		pButton->SetCheck(theApp.m_LastSettings.bProbeRatioBWCF);
	}
	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_LOWER_FREQUENCY)) {
		m_pDataSheet->GetLowerFrequency(Buff);
		pButton->SetWindowText(_T("Lower frequency : ") + Buff);
		pButton->SetCheck(theApp.m_LastSettings.bProbeLowerFrequency);
	}
	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_UPPER_FREQUENCY)) {
		m_pDataSheet->GetUpperFrequency(Buff);
		pButton->SetWindowText(_T("Upper frequency : ") + Buff);
		pButton->SetCheck(theApp.m_LastSettings.bProbeUpperFrequency);
	}


	
	
	UpdateData(FALSE);
	FillList();
	SetAccessPrivelage();

}

void CProbeDataSheetPage::CreateColumnList()
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

void CProbeDataSheetPage::FillList()
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

void CProbeDataSheetPage::SetAccessPrivelage()
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

	switch(m_pDataSheet->m_TS.Pr30.nOutputSelect) {
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

	switch(m_pDataSheet->m_TS.Pr30.nFilterMode) {
	case 0:	m_editCentreFrequency.EnableWindow(FALSE);
		break;
	case 1:	m_editCentreFrequency.EnableWindow(TRUE);
		break;
	}




}
void CProbeDataSheetPage::OnChangeEdit0() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT0);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	m_pDataSheet->m_strValues.SetAt(0,Buff);
	m_pProbe->m_bModified = TRUE;
}

void CProbeDataSheetPage::OnChangeEdit1() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	m_pDataSheet->m_strValues.SetAt(1,Buff);
	m_pProbe->m_bModified = TRUE;
}

void CProbeDataSheetPage::OnChangeEdit2() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	m_pDataSheet->m_strValues.SetAt(2,Buff);
	m_pProbe->m_bModified = TRUE;
}

void CProbeDataSheetPage::OnChangeEdit3() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	m_pDataSheet->m_strValues.SetAt(3,Buff);
	m_pProbe->m_bModified = TRUE;
}

void CProbeDataSheetPage::OnChangeEdit4() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT4);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	m_pDataSheet->m_strValues.SetAt(4,Buff);
	m_pProbe->m_bModified = TRUE;
}



void CProbeDataSheetPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	RenderScope(&dc,SCREEN);
	RenderBode(&dc,SCREEN);
	
	// Do not call CResizablePage::OnPaint() for painting messages
}

void CProbeDataSheetPage::RenderScope(CPaintDC *pDC, int nWhich)
{	
	
	CString Buff;
	int ii,nX,nY;
	CFont font;
	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = 12;
	theApp.SetFontLanguage(&lf);
	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);

	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));

	CRect rr,rrGrat;
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_SCOPE);
	pStatic->GetWindowRect(rr);
	ScreenToClient(rr);
	rrGrat = rr;

	Buff = "100%";
	CSize size = pDC->GetTextExtent(Buff); 

	rrGrat.left += size.cx + 4 + 2;
	rrGrat.bottom -= (size.cy + 4 + 2);
	rrGrat.right -= (size.cx / 2 + 4);
	rrGrat.top += size.cy + 4;

	pDC->FillRect(rr,&CBrush(RGB(255,255,255)));


	CPen* pOldPen = pDC->SelectObject(&penGrey);
	for(ii=0;ii<11;ii++) {
		nX = MulDiv(ii,rrGrat.Width(),10) + rrGrat.left;
		pDC->MoveTo(nX,rrGrat.top);
		pDC->LineTo(nX,rrGrat.bottom);
		Buff.Format(_T("%.01f"),(float)m_pDataSheet->m_TS.Adc.nDelay/1000.0f + ((float)m_pDataSheet->m_TS.Adc.nWidth/100000) * (float)(ii*10));
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(nX-size.cx/2,rrGrat.bottom+4,Buff);
	}

	for(ii=0;ii<9;ii++) {
		nY = MulDiv(ii,rrGrat.Height(),8) + rrGrat.top;
		pDC->MoveTo(rrGrat.left,nY);
		pDC->LineTo(rrGrat.right,nY);
		Buff.Format(_T("%d%%"),abs(-100 + ii*25));
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rrGrat.left - 4 - size.cx,nY - (size.cy/2), Buff);
	}

	CString Degrees = L"\x0b0";
	CString Micro = L"\x0b5";

	Buff.Format(_T("Time Base = %.02f %ss / div"),(float)m_pDataSheet->m_TS.Adc.nWidth/10000,Micro);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut((rrGrat.right + rrGrat.left) / 2 - size.cx / 2,rr.top + 2 ,Buff);

	pDC->SelectObject(&penBlack);
	pDC->MoveTo(rrGrat.TopLeft());
	pDC->LineTo(rrGrat.right,rrGrat.top);
	pDC->LineTo(rrGrat.BottomRight());
	pDC->LineTo(rrGrat.left,rrGrat.bottom);
	pDC->LineTo(rrGrat.TopLeft());

	/////////////////////////////////////////////////////////////////////////
	// Draw Trace
	////////////////////////////////////////////////////////////////////////

	if((m_pDataSheet->m_nAcqLength>0) && (m_pDataSheet->m_nAcqLength<8192)) {
		int	nS0,nS1,nMin,nMax,xx,nn,vv,nPtr;
		CPoint* pPtTrace = new CPoint[rrGrat.Width() * 2];
		int	nMaxYSample = 127;
		char* pSrc;

		if(rrGrat.Width()<m_pDataSheet->m_nAcqLength) {
			for(xx=0,nn=0;xx<(rrGrat.Width()-1);xx++) {
				nS0=MulDiv(xx,m_pDataSheet->m_nAcqLength,rrGrat.Width());
				nS1=MulDiv(xx+1,m_pDataSheet->m_nAcqLength,rrGrat.Width());
				nMin=10000;
				nMax=0;
				pSrc = m_pDataSheet->m_cArray;
				for(nPtr=nS0;nPtr<nS1;nPtr++) {
					vv=MulDiv(pSrc[nPtr],rrGrat.Height()/2,- nMaxYSample)+rrGrat.Height()/2;
					if(vv<0) vv=0;
					if(vv>rrGrat.Height()-1) vv=rrGrat.Height()-1;
					if(vv<nMin) nMin=vv;
					if(vv>nMax) nMax=vv;
				}
				pPtTrace[nn].x = xx + rrGrat.left;
				pPtTrace[nn++].y = rrGrat.top + nMin;
				pPtTrace[nn].x = xx + rrGrat.left;
				pPtTrace[nn++].y = rrGrat.top + nMax;
			}
			pDC->Polyline(pPtTrace,nn);
		} else {
			for(xx=0,nn=0;xx<rrGrat.Width();xx++) {
				nPtr=MulDiv(xx,m_pDataSheet->m_nAcqLength,rrGrat.Width());
				vv=MulDiv(m_pDataSheet->m_cArray[nPtr],rrGrat.Height()/2,- nMaxYSample)+rrGrat.Height()/2;
				if(vv<0) vv=0;
				if(vv>rrGrat.Height()-1) vv=rrGrat.Height()-1;
				pPtTrace[nn].x = rrGrat.left + xx;
				pPtTrace[nn++].y = rrGrat.top + vv;
			}
			pDC->Polyline(pPtTrace,nn);
		}
		delete pPtTrace;
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}

void CProbeDataSheetPage::RenderBode(CPaintDC *pDC, int nWhich)
{	
	
	CString Buff;
	int ii,nX,nY,ptr,vv;
	double dLogAmp;
	CFont font;
	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = 12;
	theApp.SetFontLanguage(&lf);
	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);

	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));

	CRect rr,rrGrat;
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_BODE);
	pStatic->GetWindowRect(rr);
	ScreenToClient(rr);
	rrGrat = rr;

	Buff = "100%";
	CSize size = pDC->GetTextExtent(Buff); 

	rrGrat.left += size.cx + 4 + 2;
	rrGrat.bottom -= (size.cy + 4 + 2);
	rrGrat.right -= (size.cx / 2 + 4);
	rrGrat.top += size.cy + 4;

	pDC->FillRect(rr,&CBrush(RGB(255,255,255)));


	CPen* pOldPen = pDC->SelectObject(&penGrey);
	for(ii=0;ii<11;ii++) {
		nX = MulDiv(ii,rrGrat.Width(),10) + rrGrat.left;
		pDC->MoveTo(nX,rrGrat.top);
		pDC->LineTo(nX,rrGrat.bottom);
		Buff.Format(_T("%.01f"),((m_pDataSheet->m_fFFTMaxFrequency - m_pDataSheet->m_fFFTMinFrequency) * (float)ii / 10.0f) + m_pDataSheet->m_fFFTMinFrequency);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(nX-size.cx/2,rrGrat.bottom+4,Buff);
	}

	for(ii=0;ii<FFT_MAX_RANGE;ii+=3) {
		nY = MulDiv(ii,rrGrat.Height(),FFT_MAX_RANGE) + rrGrat.top;
		pDC->MoveTo(rrGrat.left,nY);
		pDC->LineTo(rrGrat.right,nY);
		Buff.Format(_T("%d"),ii* -1);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rrGrat.left - 4 - size.cx,nY - (size.cy/2), Buff);
	}

	Buff.Format(_T("dB / MHz"));
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut((rrGrat.right + rrGrat.left) / 2 - size.cx / 2,rr.top + 2 ,Buff);

	pDC->SelectObject(&penBlack);
	pDC->MoveTo(rrGrat.TopLeft());
	pDC->LineTo(rrGrat.right,rrGrat.top);
	pDC->LineTo(rrGrat.BottomRight());
	pDC->LineTo(rrGrat.left,rrGrat.bottom);
	pDC->LineTo(rrGrat.TopLeft());


	//Draw trace
	if(m_pDataSheet->m_fSamplePeriod<=0.0f) m_pDataSheet->m_fSamplePeriod = 10.0f;
	float fMaxFrequency = 1.0f / (m_pDataSheet->m_fSamplePeriod * 2e-3f);

	int nS0 = (int)(((float)m_pDataSheet->m_nFFTLength * m_pDataSheet->m_fFFTMinFrequency) / fMaxFrequency);
	int nS1 = (int)(((float)m_pDataSheet->m_nFFTLength * m_pDataSheet->m_fFFTMaxFrequency) / fMaxFrequency);
	for(int xx=0;xx<rrGrat.Width();xx++) {
		ptr=MulDiv(xx,nS1-nS0,rrGrat.Width()) + nS0;
		if(ptr<0) ptr=0;
		if(ptr<m_pDataSheet->m_nFFTLength) {
			vv=MulDiv(m_pDataSheet->m_nAmpFrequency[ptr],rrGrat.Height(),0x7fff);
			dLogAmp = 20.0 * log10((double)(m_pDataSheet->m_nAmpFrequency[ptr]) / 32767.0);
			vv = (int)((FFT_MAX_RANGE + dLogAmp) * (double)rrGrat.Height() / FFT_MAX_RANGE);
		} else {
			vv=0;
		}
		if(vv<0) vv=0;
		if(vv>rrGrat.Height()-1) vv=rrGrat.Height()-1;
		if(xx==0) {
			pDC->MoveTo(xx+rrGrat.left,rrGrat.bottom - vv);
		} else {
			pDC->LineTo(xx+rrGrat.left,rrGrat.bottom - vv);
		}
	};




	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}


void CProbeDataSheetPage::OnButtonLock() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CProbeDataSheetPage::OnSetActive() 
{
	((CProbeCharacterisationSheet*)GetParent())->SetToolBarCheckedState();	//SetToolBarCheckedState();;	
	((CProbeCharacterisationSheet*)GetParent())->m_pCurrentPage = (CPropertyPage*)this;

	return CResizablePage::OnSetActive();
}

void CProbeDataSheetPage::OnRadioPulseDuration() 
{
	
}

void CProbeDataSheetPage::OnChangeEditDbDrop() 
{
	
	CString Buff;
	float fTemp = theApp.m_LastSettings.fProbeAtAttenuation;

	m_editAtAttenuation.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fProbeAtAttenuation);
	if(fTemp - theApp.m_LastSettings.fProbeAtAttenuation) {
		m_spinAtAttenuation.SetPos((int)fabs(theApp.m_LastSettings.fProbeAtAttenuation));
	}
}

void CProbeDataSheetPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	float fTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_DB_DROP:
		fTemp = theApp.m_LastSettings.fProbeAtAttenuation;
		theApp.m_LastSettings.fProbeAtAttenuation = (float)(pSpin->GetPos()&0xffff) * -1.0f;
		if(fTemp - theApp.m_LastSettings.fProbeAtAttenuation) {
			UpdateAllControls();
		}
		break;
	}	
	
	CResizablePage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CProbeDataSheetPage::OnCheckPulseDuration() 
{
	

	theApp.m_LastSettings.bProbePulseDuration == FALSE ? theApp.m_LastSettings.bProbePulseDuration = TRUE : theApp.m_LastSettings.bProbePulseDuration = FALSE;
	UpdateAllControls();
}
