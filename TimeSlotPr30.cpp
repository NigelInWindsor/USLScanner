// TimeSlotPr30.cpp : implementation file
//

#include "stdafx.h"
#include "Afxcmn.h"
#include "USLScanner.h"
#include "TimeSlotPr30.h"
#include "MainFrm.h"
#include "PromptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTimeSlotPr30 property page

IMPLEMENT_DYNCREATE(CTimeSlotPr30, CPropertyPage)

CTimeSlotPr30::CTimeSlotPr30(UINT nCap,CWnd* pParent,int nWhichTS,bool bSubset)
 : CPropertyPage(CTimeSlotPr30::IDD,nCap)
{
	//{{AFX_DATA_INIT(CTimeSlotPr30)
	m_staticBlankingText = _T("");
	//}}AFX_DATA_INIT
	m_nTS = MinMax(&nWhichTS, 0, 255);
	theApp.m_UtUser.SelectWriteTimeslot();
	m_pEditSpinItem=NULL;
	m_nDacIndex=0;
	m_bSubset = bSubset;
}

CTimeSlotPr30::~CTimeSlotPr30()
{
	DeleteObject(m_hBr);
}

void CTimeSlotPr30::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeSlotPr30)
	DDX_Control(pDX, IDC_STATIC_TEXT_2, m_text2);
	DDX_Control(pDX, IDC_STATIC_TEXT_0, m_text0);
	DDX_Control(pDX, IDC_STATIC_TEXT_1, m_text1);
	DDX_Control(pDX, IDC_STATIC_GROUP_0, m_buttonGroup);
	DDX_Control(pDX, IDC_COMBO_TX, m_comboTX);
	DDX_Control(pDX, IDC_COMBO_RX, m_comboRX);
	DDX_Control(pDX, IDC_COMBO_HT_OFF_ON, m_comboHTOffOn);
	DDX_Control(pDX, IDC_COMBO_LF_POINT, m_comboLFPoint);
	DDX_Control(pDX, IDC_SPIN_PRF, m_spinPRF);
	DDX_Control(pDX, IDC_EDIT_PRF, m_editPRF);
	DDX_Control(pDX, IDC_SPIN_DAC_THRESHOLD, m_spinDacTriggerThreshold);
	DDX_Control(pDX, IDC_EDIT_DAC_THRESHOLD, m_editDacTriggerThreshold);
	DDX_Control(pDX, IDC_SPIN_DAC_BLANKING, m_spinDacBlanking);
	DDX_Control(pDX, IDC_LIST_DAC_LIST, m_listDac);
	DDX_Control(pDX, IDC_EDIT_DAC_BLANKING, m_editDacBlanking);
	DDX_Control(pDX, IDC_COMBO_DAC_TRIGGER_MODE, m_comboDacTriggerMode);
	DDX_Control(pDX, IDC_COMBO_DAC_MODE, m_comboDacEnable);
	DDX_Control(pDX, IDC_SPIN_RECTIFIER_DECAY, m_spinRectifierDecay);
	DDX_Control(pDX, IDC_EDIT_RECTIFIER_DECAY, m_editRectifierDecay);
	DDX_Control(pDX, IDC_COMBO_RECTIFIER_MODE, m_comboRectifierMode);
	DDX_Control(pDX, IDC_SPIN_CENTRE_FREQ, m_spinCentreFrequency);
	DDX_Control(pDX, IDC_EDIT_CENTRE_FREQ, m_editCentreFrequency);
	DDX_Control(pDX, IDC_COMBO_FILTER_RANGE, m_comboFilterRange);
	DDX_Control(pDX, IDC_COMBO_FILTER_TYPE, m_comboFilterType);
	DDX_Control(pDX, IDC_SPIN_LIN_GAIN, m_spinLinGain);
	DDX_Control(pDX, IDC_EDIT_LIN_GAIN, m_editLinGain);
	DDX_Control(pDX, IDC_COMBO_POST_FILTER, m_comboLogPostFilter);
	DDX_Control(pDX, IDC_SPIN_LOG_GAIN, m_spinLogGain);
	DDX_Control(pDX, IDC_EDIT_LOG_GAIN, m_editLogGain);
	DDX_Control(pDX, IDC_COMBO_AMPLIFIER, m_comboAmplifier);
	DDX_Control(pDX, IDC_SPIN_PULSE_VOLTAGE, m_spinPulseVoltage);
	DDX_Control(pDX, IDC_EDIT_PULSE_VOLTAGE, m_editPulseVoltage);
	DDX_Control(pDX, IDC_SPIN_PULSE_WIDTH, m_spinPulseWidth);
	DDX_Control(pDX, IDC_EDIT_PULSE_WIDTH, m_editPulseWidth);
	DDX_Control(pDX, IDC_COMBO_DAMPING, m_comboDamping);
	DDX_Control(pDX, IDC_COMBO_PREAMP_MODE, m_comboPreampMode);
	DDX_Control(pDX, IDC_COMBO_PREAMP_GAIN, m_comboPreampGain);
	DDX_Text(pDX, IDC_STATIC_BLANKING_TEXT, m_staticBlankingText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeSlotPr30, CPropertyPage)
	//{{AFX_MSG_MAP(CTimeSlotPr30)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_PREAMP_GAIN, OnSelchangeComboPreampGain)
	ON_CBN_SELCHANGE(IDC_COMBO_PREAMP_MODE, OnSelchangeComboPreampMode)
	ON_CBN_SELCHANGE(IDC_COMBO_DAMPING, OnSelchangeComboDamping)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_PULSE_WIDTH, OnChangeEditPulseWidth)
	ON_EN_CHANGE(IDC_EDIT_PULSE_VOLTAGE, OnChangeEditPulseVoltage)
	ON_CBN_SELCHANGE(IDC_COMBO_AMPLIFIER, OnSelchangeComboAmplifier)
	ON_EN_CHANGE(IDC_EDIT_LOG_GAIN, OnChangeEditLogGain)
	ON_CBN_SELCHANGE(IDC_COMBO_POST_FILTER, OnSelchangeComboPostFilter)
	ON_EN_CHANGE(IDC_EDIT_LIN_GAIN, OnChangeEditLinGain)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, OnSelchangeComboFilterType)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_RANGE, OnSelchangeComboFilterRange)
	ON_EN_CHANGE(IDC_EDIT_CENTRE_FREQ, OnChangeEditCentreFreq)
	ON_EN_CHANGE(IDC_EDIT_RECTIFIER_DECAY, OnChangeEditRectifierDecay)
	ON_CBN_SELCHANGE(IDC_COMBO_RECTIFIER_MODE, OnSelchangeComboRectifierMode)
	ON_CBN_SELCHANGE(IDC_COMBO_DAC_MODE, OnSelchangeComboDacMode)
	ON_CBN_SELCHANGE(IDC_COMBO_DAC_TRIGGER_MODE, OnSelchangeComboDacTriggerMode)
	ON_EN_CHANGE(IDC_EDIT_DAC_THRESHOLD, OnChangeEditDacThreshold)
	ON_EN_CHANGE(IDC_EDIT_DAC_BLANKING, OnChangeEditDacBlanking)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DAC_LIST, OnGetdispinfoListDacList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DAC_LIST, OnDblclkListDacList)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DAC_LIST, OnClickListDacList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DAC_LIST, OnRclickListDacList)
	ON_COMMAND(ID_DAC_CLEARALLGAINS, OnRbdacmenuClearallgains)
	ON_COMMAND(ID_DAC_CLEAREVERYTHING, OnRbdacmenuCleareverything)
	ON_EN_CHANGE(IDC_EDIT_PRF, OnChangeEditPrf)
	ON_CBN_SELCHANGE(IDC_COMBO_LF_POINT, OnSelchangeComboLfPoint)
	ON_COMMAND(ID_DAC_DELETEPT, OnRbdacmenuDeletept)
	ON_CBN_SELCHANGE(IDC_COMBO_HT_OFF_ON, OnSelchangeComboHtOffOn)
	ON_CBN_SELCHANGE(IDC_COMBO_TX, OnSelchangeComboTx)
	ON_CBN_SELCHANGE(IDC_COMBO_RX, OnSelchangeComboRx)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_MESSAGE(UI_ITEMCHANGED,DacTableChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeSlotPr30 message handlers


BOOL CTimeSlotPr30::OnInitDialog() 
{
	CString	Buff;
	CPropertyPage::OnInitDialog();
	UDACCEL	Accel;
	Accel.nInc=5;
	Accel.nSec=1;

	RedrawLayout();

	m_comboPreampGain.AddString(_T("-20dB"));
	m_comboPreampGain.AddString(_T("-10dB"));
	m_comboPreampGain.AddString(_T("0dB"));
	m_comboPreampGain.AddString(_T("10dB"));
	m_comboPreampGain.AddString(_T("20dB"));

	Buff.LoadString(IDS_Pulse_Echo);			m_comboPreampMode.AddString(Buff);
	Buff.LoadString(IDS_Through_Transmission);	m_comboPreampMode.AddString(Buff);

	m_comboDamping.AddString(_T("33R"));
	m_comboDamping.AddString(_T("50R"));
	m_comboDamping.AddString(_T("100R"));
	m_comboDamping.AddString(_T("200R"));

	Buff.LoadString(IDS_RF);			m_comboAmplifier.AddString(Buff);
	Buff.LoadString(IDS_Rectified);		m_comboAmplifier.AddString(Buff);
	Buff.LoadString(IDS_Logorithmic);	m_comboAmplifier.AddString(Buff);

	m_spinPulseWidth.SetRange(0,0xfff);
	m_spinPulseVoltage.SetRange(0,400);
	m_spinLogGain.SetRange(0,299);
	m_spinLinGain.SetRange(0,1000);
	m_spinLogGain.SetAccel(1,&Accel);
	m_spinLinGain.SetAccel(1,&Accel);

	m_comboLogPostFilter.AddString(_T("500 KHz"));
	m_comboLogPostFilter.AddString(_T("1 MHz"));
	m_comboLogPostFilter.AddString(_T("2.5 MHz"));
	m_comboLogPostFilter.AddString(_T("5 MHz"));
	m_comboLogPostFilter.AddString(_T("10 MHz"));
	Buff.LoadString(IDS_WideBand);		m_comboLogPostFilter.AddString(Buff);

	Buff.LoadString(IDS_Low_Pass);			m_comboFilterType.AddString(Buff);
	Buff.LoadString(IDS_Band_Pass_Narrow);	m_comboFilterType.AddString(Buff);
	Buff.LoadString(IDS_Band_Pass_Wide);	m_comboFilterType.AddString(Buff);

	m_spinCentreFrequency.SetRange(3,63);

	Buff.LoadString(IDS_Positive);	m_comboRectifierMode.AddString(Buff);
	Buff.LoadString(IDS_Negative);	m_comboRectifierMode.AddString(Buff);
	Buff.LoadString(IDS_Full_Wave);	m_comboRectifierMode.AddString(Buff);

	m_spinRectifierDecay.SetRange(0,100);

	Buff.LoadString(IDS_Off); m_comboDacEnable.AddString(Buff);
	Buff.LoadString(IDS_On); m_comboDacEnable.AddString(Buff);

	Buff.LoadString(IDS_Mainbang); m_comboDacTriggerMode.AddString(Buff);
	Buff.LoadString(IDS_Interface); m_comboDacTriggerMode.AddString(Buff);

	m_comboLFPoint.AddString(_T("0.5 MHz"));
	m_comboLFPoint.AddString(_T("1.5 MHz"));

	m_spinDacBlanking.SetRange32(0,32767);
	m_spinDacTriggerThreshold.SetRange(0,200);

	m_spinPRF.SetRange(100,10000);

	Buff.LoadString(IDS_Off);	m_comboHTOffOn.AddString(Buff);
	Buff.LoadString(IDS_On);	m_comboHTOffOn.AddString(Buff);

	CreateColumnList();

	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	EnableToolTips(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CTimeSlotPr30::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CTimeSlotPr30::UpdateAllControls()
{
	CString Buff;

	if(GetSafeHwnd() == NULL) return;

	switch(theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputSelect) {
	case 0:	theApp.m_UtUser.m_TS[m_nTS].Adc.nRfType=RFTYPELINEAR;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputOffsetEnable=0;
		break;
	case 1:	theApp.m_UtUser.m_TS[m_nTS].Adc.nRfType=RFTYPERECTIFIED;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputOffsetEnable=0;
		break;
	case 2:	theApp.m_UtUser.m_TS[m_nTS].Adc.nRfType=RFTYPELOG;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputOffsetEnable=1;
		break;
	}

	m_comboTX.ResetContent();
	m_comboRX.ResetContent();
	for(int ii=0;ii<8;ii++) {
		Buff.Format(_T("%s"),theApp.m_UtUser.m_Global.cChannelName[ii]);
		m_comboTX.AddString(Buff);
		m_comboRX.AddString(Buff);
	}
	m_comboTX.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nMuxTx-1);
	m_comboRX.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nMuxRx-1);

	m_comboPreampGain.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nPreampGain);
	m_comboPreampMode.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nPreampMode);
	m_comboDamping.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nDamping);
	m_comboAmplifier.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputSelect);

	m_spinPulseWidth.SetPos(theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth);
	m_spinPulseVoltage.SetPos(theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage);

	Buff.Format(_T("%d"),theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth);
	m_editPulseWidth.SetWindowText(Buff);
	Buff.Format(_T("%d V"),theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage);
	m_editPulseVoltage.SetWindowText(Buff);

	m_spinLogGain.SetPos((int)(theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain*10.0));
	Buff.Format(_T("%.01f dB"),theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain);
	m_editLogGain.SetWindowText(Buff);

	m_spinLinGain.SetPos((int)(theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain*10.0));
	Buff.Format(_T("%.01f dB"),theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain);
	m_editLinGain.SetWindowText(Buff);

	m_comboLogPostFilter.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nLogPostDetectFilter);

	m_comboFilterType.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterType);
	SetFilterRangeCombo();

	m_spinCentreFrequency.SetPos(theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassCenterFrequency);
	theApp.m_UtUser.Pr30CalculateRealWorldCentreFrequency(&theApp.m_UtUser.m_TS[m_nTS].Pr30,&Buff);
	m_editCentreFrequency.SetWindowText(Buff);

	m_comboRectifierMode.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDetectionMode);
	Buff.Format(_T("%d %%"),theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDecayPercentage);
	m_editRectifierDecay.SetWindowText(Buff);
	m_spinRectifierDecay.SetPos(theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDecayPercentage);

	m_comboDacEnable.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacEnable&1);
	m_comboDacTriggerMode.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTriggerMode&1);

	m_comboLFPoint.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nLinTimeConstant);


	theApp.m_UtUser.Pr30DacBlankingString(&theApp.m_UtUser.m_TS[m_nTS].Pr30,&Buff);
	m_editDacBlanking.SetWindowText(Buff);

	m_spinDacBlanking.SetPos(theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacBlanking/10);
	m_spinDacTriggerThreshold.SetPos(MulDiv(theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTriggerThreshold-128,100,127)+100);
	Buff.Format(_T("%d %%"),MulDiv(theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTriggerThreshold-128,theApp.m_LastSettings.nDacMaxThreshold,127));
	m_editDacTriggerThreshold.SetWindowText(Buff);

	Buff.Format(_T("%d Hz"),theApp.m_UtUser.m_Global.nPrf);
	m_editPRF.SetWindowText(Buff);
	m_spinPRF.SetPos(theApp.m_UtUser.m_Global.nPrf);

	m_comboHTOffOn.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulserOffOn);

	switch(theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTriggerMode) {
	case 0:	m_staticBlankingText.LoadString(IDS_Delay);
		break;
	case 1: m_staticBlankingText.LoadString(IDS_Blanking);
		break;
	}
	UpdateData(FALSE);


	FillList();

	SetAccessPrivelage();
}


void CTimeSlotPr30::SetFilterRangeCombo()
{
	CString Buff;

	m_comboFilterRange.ResetContent();
	switch(theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterType) {
	case 0:
		m_comboFilterRange.AddString(_T("2.5 MHz"));
		m_comboFilterRange.AddString(_T("5 MHz"));
		m_comboFilterRange.AddString(_T("10 MHz"));
		m_comboFilterRange.AddString(_T("15 MHz"));
		Buff.LoadString(IDS_Full); m_comboFilterRange.AddString(Buff);
		m_comboFilterRange.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nLowPassRange);
		break;
	case 1:
	case 2:
		m_comboFilterRange.AddString(_T("0.5-2 MHz"));
		m_comboFilterRange.AddString(_T("1-4 MHz"));
		m_comboFilterRange.AddString(_T("2-8 MHz"));
		m_comboFilterRange.AddString(_T("4-16 MHz"));
		m_comboFilterRange.AddString(_T("8-32 MHz"));
		m_comboFilterRange.SetCurSel(theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassRange);
		break;
	}
}


void CTimeSlotPr30::CheckPreAmpTSVariables()
{
	bool bTxWarning = FALSE;
	bool bRxWarning = FALSE;
	CString TxBuff = L"TxVariables Changed ";
	CString RxBuff = L"Rx Variables Changed";
	CString Buff;

	for(int nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		if(nTS - m_nTS) {

			if(theApp.m_UtUser.m_TS[m_nTS].Pr30.nMuxRx == theApp.m_UtUser.m_TS[nTS].Pr30.nMuxRx) {
				if(theApp.m_UtUser.m_TS[nTS].Pr30.nPreampGain - theApp.m_UtUser.m_TS[m_nTS].Pr30.nPreampGain) {
					bRxWarning=TRUE;
					theApp.m_UtUser.m_TS[nTS].Pr30.nPreampGain = theApp.m_UtUser.m_TS[m_nTS].Pr30.nPreampGain;
				}
			}

			if(theApp.m_UtUser.m_TS[m_nTS].Pr30.nMuxTx == theApp.m_UtUser.m_TS[nTS].Pr30.nMuxTx) {
				if(theApp.m_UtUser.m_TS[nTS].Pr30.nPulseVoltage - theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage) {
					bTxWarning=TRUE;
					theApp.m_UtUser.m_TS[nTS].Pr30.nPulseVoltage = theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage;
				}
				if(theApp.m_UtUser.m_TS[nTS].Pr30.nPulseWidth - theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth) {
					bTxWarning=TRUE;
					theApp.m_UtUser.m_TS[nTS].Pr30.nPulseWidth = theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth;
				}
				if(theApp.m_UtUser.m_TS[nTS].Pr30.nDamping - theApp.m_UtUser.m_TS[m_nTS].Pr30.nDamping) {
					bTxWarning=TRUE;
					theApp.m_UtUser.m_TS[nTS].Pr30.nDamping = theApp.m_UtUser.m_TS[m_nTS].Pr30.nDamping;
				}
			}
		}
	}

	Buff.Empty();
	if(bTxWarning == TRUE) Buff += TxBuff;
	if(bRxWarning == TRUE) {
		if(Buff.IsEmpty()==FALSE) Buff += " ";
		Buff += RxBuff;
	};
	/*
	if(Buff.IsEmpty() == FALSE) {

		CPromptDlg* pDlg = new CPromptDlg(this,Buff);
		pDlg->Create(CPromptDlg::IDD, this);	
		Sleep(1000);
		pDlg->SendMessage(WM_CLOSE);
		delete pDlg;

	}
	*/
}

void CTimeSlotPr30::OnSelchangeComboPreampGain() 
{
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nPreampGain = m_comboPreampGain.GetCurSel();
	theApp.m_UtUser.Pr30PreampGain(&theApp.m_UtUser.m_TS[m_nTS].Pr30);

	CheckPreAmpTSVariables();
	CopyTimeSlotVariables();
}



void CTimeSlotPr30::OnSelchangeComboPreampMode() 
{
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nPreampMode = m_comboPreampMode.GetCurSel();
	theApp.m_UtUser.Pr30PreampMode(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	CopyTimeSlotVariables();
}

void CTimeSlotPr30::OnSelchangeComboDamping() 
{
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nDamping = m_comboDamping.GetCurSel();
	theApp.m_UtUser.Pr30Damping(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	CheckPreAmpTSVariables();
	CopyTimeSlotVariables();
	
}

void CTimeSlotPr30::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;
	float fTemp;

	switch(pSpin->GetDlgCtrlID()) {
/*	case IDC_SPIN_MAIN_GAIN:
		m_pTS->Pr15.nMainGain=(pSpin->GetPos()&0xffff);
		Buff.Format(_T("%.01fdB"),((double)m_pTS->Pr15.nMainGain-56)/2.0);
		m_editMainGain.SetWindowText(Buff);
		theApp.m_UtUser.Pr15Gain(&m_pTS->Pr15);
		break;
		*/
	case IDC_SPIN_PULSE_VOLTAGE:
		nTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage=(pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage) {
			MinMax(&theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage, 0, theApp.m_Tank.nMaxPulseVoltage);
			Buff.Format(_T("%dV"),theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage);
			m_editPulseVoltage.SetWindowText(Buff);
			theApp.m_UtUser.Pr30PulseVoltage(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
			theApp.m_UtUser.Pr30PulseVoltageMSC(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
			theApp.m_UtUser.MailboxHt(TRUE,theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage);
			CheckPreAmpTSVariables();
			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_PULSE_WIDTH:
		nTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth=(pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth) {
			Buff.Format(_T("%d"),theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth);
			m_editPulseWidth.SetWindowText(Buff);
			theApp.m_UtUser.Pr30PulseWidth(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
			CheckPreAmpTSVariables();
			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_LOG_GAIN:
		fTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		if(fTemp - theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain) {
			Buff.Format(_T("%.01f dB"),theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain);
			m_editLogGain.SetWindowText(Buff);
			theApp.m_UtUser.Pr30LogFloatGain(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_LIN_GAIN:
		fTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		if(fTemp - theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain) {
			Buff.Format(_T("%.01f dB"),theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain);
			m_editLinGain.SetWindowText(Buff);
			theApp.m_UtUser.Pr30LinFloatGain(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_CENTRE_FREQ:
		nTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassCenterFrequency;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassCenterFrequency=(pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassCenterFrequency) {
			theApp.m_UtUser.Pr30CalculateRealWorldCentreFrequency(&theApp.m_UtUser.m_TS[m_nTS].Pr30,&Buff);
			m_editCentreFrequency.SetWindowText(Buff);
			theApp.m_UtUser.Pr30CenterFrequency(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_RECTIFIER_DECAY:
		nTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDecayPercentage;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDecayPercentage=(pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDecayPercentage) {
			Buff.Format(_T("%d %%"),theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDecayPercentage);
			m_editRectifierDecay.SetWindowText(Buff);
			theApp.m_UtUser.Pr30RectifierDecay(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_DAC_BLANKING:
		nTemp=theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacBlanking;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacBlanking=(pSpin->GetPos()&0xffff)*10;
		if(nTemp-theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacBlanking) {
			theApp.m_UtUser.Pr30BlankingIntToFloat(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
			theApp.m_UtUser.Pr30DacBlankingString(&theApp.m_UtUser.m_TS[m_nTS].Pr30,&Buff);
			m_editDacBlanking.SetWindowText(Buff);
			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_DAC_THRESHOLD:
		nTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTriggerThreshold;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTriggerThreshold=MulDiv((pSpin->GetPos()&0xffff)-100,127,100)+128;
		if(nTemp - theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTriggerThreshold) {
			Buff.Format(_T("%d %%"),MulDiv(theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTriggerThreshold-128,theApp.m_LastSettings.nDacMaxThreshold,127));
			m_editDacTriggerThreshold.SetWindowText(Buff);
			theApp.m_UtUser.Pr30DacThreshold(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_PRF:
		theApp.m_UtUser.m_Global.nPrf=(pSpin->GetPos()&0xffff);
		Buff.Format(_T("%d"),theApp.m_UtUser.m_Global.nPrf);
		m_editPRF.SetWindowText(Buff);
		theApp.m_UtUser.SetPrf();
		break;
	}	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTimeSlotPr30::OnChangeEditPulseWidth() 
{
	int	nWidth=theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth;
	CString	Buff;

	m_editPulseWidth.GetWindowText(Buff);
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth=_ttoi(Buff);
	m_spinPulseWidth.SetPos(theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth);
	if(nWidth-theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth) {
		theApp.m_UtUser.Pr30PulseWidth(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
		CheckPreAmpTSVariables();
		CopyTimeSlotVariables();
	}
}

void CTimeSlotPr30::OnChangeEditPulseVoltage() 
{
	int	nValue=theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage;
	CString	Buff;

	m_editPulseVoltage.GetWindowText(Buff);
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage=_ttoi(Buff);
	MinMax(&theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage, 0, theApp.m_Tank.nMaxPulseVoltage);
	m_spinPulseVoltage.SetPos(theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage);

	if(nValue-theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage) {
		theApp.m_UtUser.Pr30PulseVoltage(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
		theApp.m_UtUser.MailboxHt(TRUE,theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage);
		CheckPreAmpTSVariables();
		CopyTimeSlotVariables();
	}
	
}

void CTimeSlotPr30::OnSelchangeComboAmplifier() 
{
	switch(theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputSelect=m_comboAmplifier.GetCurSel()) {
	case 0:	theApp.m_UtUser.m_TS[m_nTS].Adc.nRfType=RFTYPELINEAR;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputOffsetEnable=0;
		break;
	case 1:	theApp.m_UtUser.m_TS[m_nTS].Adc.nRfType=RFTYPERECTIFIED;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputOffsetEnable=0;
		break;
	case 2:	theApp.m_UtUser.m_TS[m_nTS].Adc.nRfType=RFTYPELOG;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputOffsetEnable=1;
		break;
	}
	theApp.m_UtUser.Pr30OutputSelect(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	theApp.m_UtUser.Pr30OutputOffsetEnable(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	switch(theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputSelect) {
	case 0:	theApp.m_UtUser.Pr30LinFloatGain(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
		break;
	case 1:	theApp.m_UtUser.Pr30LinFloatGain(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
		break;
	case 2:	theApp.m_UtUser.Pr30LogFloatGain(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
		break;
	}
	CopyTimeSlotVariables();
	SetAccessPrivelage();
}

void CTimeSlotPr30::OnChangeEditLogGain() 
{
	CString Buff;
	float fTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain;

	m_editLogGain.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain);
	if(fTemp != theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain) {
		theApp.m_UtUser.Pr30LogFloatGain(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
		m_spinLogGain.SetPos((int)(theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain*10.0));
		CopyTimeSlotVariables();
	}

}

void CTimeSlotPr30::OnSelchangeComboPostFilter() 
{
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nLogPostDetectFilter=m_comboLogPostFilter.GetCurSel();
	theApp.m_UtUser.Pr30LogPostDetectFilter(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	CopyTimeSlotVariables();
}

void CTimeSlotPr30::OnChangeEditLinGain() 
{
	CString Buff;
	float fTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain;

	m_editLinGain.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain);
	if(fTemp != theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain) {
		theApp.m_UtUser.Pr30LinFloatGain(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
		m_spinLinGain.SetPos((int)(theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain*10.0));
		CopyTimeSlotVariables();
	}
	
}

void CTimeSlotPr30::OnSelchangeComboFilterType() 
{
	switch(theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterType = m_comboFilterType.GetCurSel()) {
	case 0: theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterMode = 0;
		break;
	case 1: theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterMode = 1;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassWidth=0;
		break;
	case 2: theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterMode = 1;
		theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassWidth=1;
		break;
	}
	theApp.m_UtUser.Pr30FilterMode(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	theApp.m_UtUser.Pr30BandPassWidth(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	SetFilterRangeCombo();
	CopyTimeSlotVariables();
	SetAccessPrivelage();
}

void CTimeSlotPr30::OnSelchangeComboFilterRange() 
{
	CString Buff;

	switch(theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterType) {
	case 0:	theApp.m_UtUser.m_TS[m_nTS].Pr30.nLowPassRange = m_comboFilterRange.GetCurSel();
		break;
	case 1:
	case 2:	theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassRange = m_comboFilterRange.GetCurSel();
		theApp.m_UtUser.Pr30CalculateRealWorldCentreFrequency(&theApp.m_UtUser.m_TS[m_nTS].Pr30,&Buff);
		m_editCentreFrequency.SetWindowText(Buff);
		break;
	}
	theApp.m_UtUser.Pr30FilterRange(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	CopyTimeSlotVariables();

}


void CTimeSlotPr30::OnChangeEditCentreFreq() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}


void CTimeSlotPr30::OnChangeEditRectifierDecay() 
{
	CString Buff;
	int nTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDecayPercentage;

	m_editRectifierDecay.GetWindowText(Buff);
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDecayPercentage = _ttoi(Buff);
	if(nTemp - theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDecayPercentage) {
		m_spinRectifierDecay.SetPos(theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDecayPercentage);
		theApp.m_UtUser.Pr30RectifierDecay(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
		CopyTimeSlotVariables();
	}

}

void CTimeSlotPr30::OnSelchangeComboRectifierMode() 
{
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nRectifierDetectionMode=m_comboRectifierMode.GetCurSel();
	theApp.m_UtUser.Pr30RectifierMode(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	CopyTimeSlotVariables();
}

void CTimeSlotPr30::OnSelchangeComboDacMode() 
{
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacEnable = m_comboDacEnable.GetCurSel();
	theApp.m_UtUser.Pr30DacEnable(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	CopyTimeSlotVariables();
}


void CTimeSlotPr30::OnChangeEditDacThreshold() 
{
	
}

void CTimeSlotPr30::OnSelchangeComboDacTriggerMode() 
{
	CString Buff;

	theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTriggerMode = m_comboDacTriggerMode.GetCurSel();

	theApp.m_UtUser.Pr30DacTriggerMode(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	theApp.m_UtUser.Pr30DacBlanking(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	CopyTimeSlotVariables();
	UpdateAllControls();
	
}


void CTimeSlotPr30::OnChangeEditDacBlanking() 
{
	CString Buff;
	int	nTemp = theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacBlanking;

	m_editDacBlanking.GetWindowText(Buff);
	theApp.m_UtUser.Pr30DacBlankingFromString(&theApp.m_UtUser.m_TS[m_nTS].Pr30,Buff);
	if(nTemp - theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacBlanking) {
		m_spinDacBlanking.SetPos(theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacBlanking/10);
		CopyTimeSlotVariables();
	}
	
}



void CTimeSlotPr30::CreateColumnList()
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

void CTimeSlotPr30::FillList()
{
	CString	Buff;

	m_listDac.DeleteAllItems();
	for (int ii=0;ii<16;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listDac.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
	m_listDac.EnsureVisible(m_nDacIndex,FALSE);
	m_listDac.SetItemState(m_nDacIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listDac.SetFocus();
}

void CTimeSlotPr30::OnGetdispinfoListDacList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTable%=3;
	str[0]=0;
	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str,_T("%.02f"),theApp.m_UtUser.m_TS[m_nTS].Pr30.fDacDelay[theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTable][pDispInfo->item.iItem]);
		break;
	case 2:	swprintf_s(str,_T("%.02f"),theApp.m_UtUser.m_TS[m_nTS].Pr30.fDacGain[theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTable][pDispInfo->item.iItem]);
		break;
	}
	pDispInfo->item.pszText=str;	
	*pResult = 0;}

void CTimeSlotPr30::OnDblclkListDacList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	m_listDac.GetWindowRect(CtrlRect);
	m_listDac.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	CloseEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**)&m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 1:	m_pEditSpinItem->Initialize(&theApp.m_UtUser.m_TS[m_nTS].Pr30.fDacDelay[theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTable][pDispInfo->item.mask],0.0f,1400.0f,0.0999f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 2:	m_pEditSpinItem->Initialize(&theApp.m_UtUser.m_TS[m_nTS].Pr30.fDacGain[theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTable][pDispInfo->item.mask],0.0f,80.0f,0.0999f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	}
	*pResult = 0;
}

HRESULT CTimeSlotPr30::DacTableChanged(WPARAM, LPARAM)
{
	theApp.m_UtUser.Pr30CalculateDacData(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	CopyTimeSlotVariables();
	return NULL;
}

void CTimeSlotPr30::OnClickListDacList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CloseEditSpin();
//	m_nIndex=pDispInfo->item.mask;
	
	*pResult = 0;
}


void CTimeSlotPr30::OnRclickListDacList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	CloseEditSpin();

	m_nDacIndex=pDispInfo->item.mask;

	CMenu* menu = (CMenu *) new CMenu;
	CPoint	Point;

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_DAC_MENU)) {
		CMenu* pPopup = menu->GetSubMenu(0);
		ASSERT(pPopup != NULL);
				
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;

	*pResult = 0;
}

void CTimeSlotPr30::CloseEditSpin()
{
	SAFE_DELETE(m_pEditSpinItem);
}

void CTimeSlotPr30::OnRbdacmenuClearallgains() 
{
	theApp.m_UtUser.Pr30ZeroCurrentDacGains(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	CopyTimeSlotVariables();
	Invalidate(FALSE);
	
}

void CTimeSlotPr30::OnRbdacmenuCleareverything() 
{
	theApp.m_UtUser.Pr30ZeroCurrentDacDelaysAndGains(&theApp.m_UtUser.m_TS[m_nTS].Pr30);

	theApp.m_UtUser.Pr30CalculateDacData(&theApp.m_UtUser.m_TS[m_nTS].Pr30);

	CopyTimeSlotVariables();

	Invalidate(FALSE);
}

void CTimeSlotPr30::OnChangeEditPrf() 
{
	CString	Buff;
	int	nPrf=theApp.m_UtUser.m_Global.nPrf;

	m_editPRF.GetWindowText(Buff);
	theApp.m_UtUser.m_Global.nPrf=_ttoi(Buff);
	m_spinPRF.SetPos(theApp.m_UtUser.m_Global.nPrf);
	if(nPrf-theApp.m_UtUser.m_Global.nPrf) theApp.m_UtUser.SetPrf();
	
}


void CTimeSlotPr30::OnSelchangeComboLfPoint() 
{
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nLinTimeConstant = m_comboLFPoint.GetCurSel();
	theApp.m_UtUser.Pr30LinearTimeConstant(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	CopyTimeSlotVariables();
	
	
}


void CTimeSlotPr30::OnRbdacmenuDeletept() 
{
	for(int ii=m_nDacIndex;ii<63;ii++) {
		theApp.m_UtUser.m_TS[m_nTS].Pr30.fDacDelay[theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTable][ii]=theApp.m_UtUser.m_TS[m_nTS].Pr30.fDacDelay[theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTable][ii+1];
		theApp.m_UtUser.m_TS[m_nTS].Pr30.fDacGain[theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTable][ii]=theApp.m_UtUser.m_TS[m_nTS].Pr30.fDacGain[theApp.m_UtUser.m_TS[m_nTS].Pr30.nDacTable][ii+1];
	}
	FillList();
	DacTableChanged(NULL,NULL);
}

void CTimeSlotPr30::OnSelchangeComboHtOffOn() 
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulserOffOn=m_comboHTOffOn.GetCurSel();

	theApp.m_UtUser.Pr30PulserOffOn(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	pFrame->SetToolBarCheckedState(FALSE);

}

void CTimeSlotPr30::OnSelchangeComboTx() 
{
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nMuxTx=m_comboTX.GetCurSel()+1;
	theApp.m_UtUser.Pr30SetTx(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
}

void CTimeSlotPr30::OnSelchangeComboRx() 
{
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nMuxRx=m_comboRX.GetCurSel()+1;
	theApp.m_UtUser.Pr30SetRx(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	
}



BOOL CTimeSlotPr30::PreTranslateMessage(MSG* pMsg) 
{
	CString Buff;
	int	nn;

	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case 13:
			nn=theApp.m_UtUser.m_TS[m_nTS].Pr30.nMuxTx-1;
			m_comboTX.GetWindowText(Buff);
			m_comboTX.DeleteString(nn);
			m_comboTX.InsertString(nn,Buff);
			m_comboTX.SetCurSel(nn);
			swprintf_s(theApp.m_UtUser.m_Global.cChannelName[nn], sizeof theApp.m_UtUser.m_Global.cChannelName[0], L"%s", (LPCWSTR)Buff);

			return TRUE;
			break;
		}
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL CTimeSlotPr30::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case IDC_COMBO_TX:  strTipText.LoadString(IDS_This_field_is_editable);
		break;
	case IDC_LIST_DAC_LIST: strTipText.LoadString(IDS_There_is_a_context_menu);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

BOOL CTimeSlotPr30::OnKillActive() 
{
//	CloseEditSpin();	
	return CPropertyPage::OnKillActive();
}

void CTimeSlotPr30::UpdateDacTable()
{
	if (GetSafeHwnd() == NULL) return;

	m_listDac.Invalidate(FALSE);
}


void CTimeSlotPr30::SetAccessPrivelage()
{
	if (GetSafeHwnd() == NULL) return;

	bool bFlag = false;
	if(theApp.m_nLogonLevelMask &  theApp.m_cAccess[IDD_PR30_TIMESLOT_DIALOG]) {
		bFlag = true;
	}
	if((theApp.m_LastSettings.nScanMask & _LOCK_UT) && (theApp.m_pScanData != NULL)) {
		bFlag = false;
	}
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
	m_spinLinGain.ShowWindow(FALSE);
	m_spinLogGain.ShowWindow(FALSE);
	m_editLogGain.ShowWindow(FALSE);
	m_comboLFPoint.EnableWindow(FALSE);
	m_comboLogPostFilter.ShowWindow(FALSE);
	m_comboRectifierMode.ShowWindow(FALSE);
	m_spinRectifierDecay.ShowWindow(FALSE);
	m_editRectifierDecay.ShowWindow(FALSE);
	m_text0.ShowWindow(FALSE);
	m_text1.ShowWindow(FALSE);
	m_text2.ShowWindow(FALSE);

	switch(theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputSelect) {
	case 0:
		Buff.LoadString(IDS_Linear); m_buttonGroup.SetWindowText(Buff);
		Buff.LoadString(IDS_Gain); m_text0.SetWindowText(Buff);
		m_text0.ShowWindow(TRUE);

		m_editLinGain.ShowWindow(TRUE);
		m_editLinGain.GetWindowRect(&rrEdit);
		m_editLinGain.SetWindowPos(NULL,nEditLeft,nTop,63,23,NULL);
		m_spinLinGain.SetBuddy(&m_editLinGain);

		m_editLinGain.GetWindowRect(&rrEdit);
		ScreenToClient(&rrEdit);
		m_buttonGroup.SetWindowPos(NULL,rrGroup.left,rrGroup.top,rrGroup.Width(),rrEdit.bottom-rrGroup.top+4,NULL);
		m_comboLFPoint.EnableWindow(bFlag);
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
		m_spinLinGain.SetBuddy(&m_editLinGain);

		m_comboRectifierMode.SetWindowPos(NULL,nEditLeft,nTop+nHeight,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);

		m_editRectifierDecay.ShowWindow(TRUE);
		m_editRectifierDecay.GetWindowRect(&rrEdit);
		m_editRectifierDecay.SetWindowPos(NULL,nEditLeft,nTop+nHeight*2,63,23,NULL);
		m_spinRectifierDecay.SetBuddy(&m_editRectifierDecay);

		m_editRectifierDecay.GetWindowRect(&rrEdit);
		ScreenToClient(&rrEdit);
		m_buttonGroup.SetWindowPos(NULL,rrGroup.left,rrGroup.top,rrGroup.Width(),rrEdit.bottom-rrGroup.top+4,NULL);
		m_comboLFPoint.EnableWindow(bFlag);
		break;
	case 2:
		Buff.LoadString(IDS_Log); m_buttonGroup.SetWindowText(Buff);
		Buff.LoadString(IDS_Log_Gain);		m_text0.SetWindowText(Buff);
		Buff.LoadString(IDS_Post_Filter);	m_text1.SetWindowText(Buff);
		Buff.LoadString(IDS_RF_Gain);		m_text2.SetWindowText(Buff);
		m_text1.SetWindowPos(NULL,nTextLeft,nTop+nHeight,0,0,SWP_NOSIZE);
		m_text2.SetWindowPos(NULL,nTextLeft,nTop+nHeight*2,0,0,SWP_NOSIZE);
		m_text0.ShowWindow(TRUE);
		m_text1.ShowWindow(TRUE);

		m_editLogGain.ShowWindow(TRUE);
		m_editLogGain.GetWindowRect(&rrEdit);
		m_editLogGain.SetWindowPos(NULL,nEditLeft,nTop,63,23,NULL);
		m_spinLogGain.SetBuddy(&m_editLogGain);

		m_comboLogPostFilter.SetWindowPos(NULL,nEditLeft,nTop+nHeight,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);

		m_comboLogPostFilter.GetWindowRect(&rrEdit);
		ScreenToClient(&rrEdit);

		m_buttonGroup.SetWindowPos(NULL,rrGroup.left,rrGroup.top,rrGroup.Width(),rrEdit.bottom-rrGroup.top+4,NULL);
		break;
	}

	//????????
	switch(theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterMode) {
	case 0: m_spinCentreFrequency.EnableWindow(FALSE);
		m_editCentreFrequency.EnableWindow(FALSE);
		break;
	case 1: m_spinCentreFrequency.EnableWindow(bFlag);
		m_editCentreFrequency.EnableWindow(bFlag);
		break;
	}

	switch(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]) {
	default:
		m_comboTX.EnableWindow(bFlag);
		m_comboRX.EnableWindow(bFlag);
		m_comboHTOffOn.EnableWindow(bFlag);
		m_comboLFPoint.EnableWindow(bFlag);
		m_spinPRF.EnableWindow(bFlag);
		m_editPRF.EnableWindow(bFlag);
		m_spinDacTriggerThreshold.EnableWindow(bFlag);
		m_editDacTriggerThreshold.EnableWindow(bFlag);
		m_spinDacBlanking.EnableWindow(bFlag);
		m_listDac.EnableWindow(bFlag);
		m_editDacBlanking.EnableWindow(bFlag);
		m_comboDacTriggerMode.EnableWindow(bFlag);
		m_comboDacEnable.EnableWindow(bFlag);
		m_spinRectifierDecay.EnableWindow(bFlag);
		m_editRectifierDecay.EnableWindow(bFlag);
		m_comboRectifierMode.EnableWindow(bFlag);
		m_comboFilterRange.EnableWindow(bFlag);
		m_comboFilterType.EnableWindow(bFlag);
		m_spinLinGain.EnableWindow(bFlag);
		m_editLinGain.EnableWindow(bFlag);
		m_comboLogPostFilter.EnableWindow(bFlag);
		m_spinLogGain.EnableWindow(bFlag);
		m_editLogGain.EnableWindow(bFlag);
		m_comboAmplifier.EnableWindow(bFlag);
		m_spinPulseVoltage.EnableWindow(bFlag);
		m_editPulseVoltage.EnableWindow(bFlag);
		m_spinPulseWidth.EnableWindow(bFlag);
		m_editPulseWidth.EnableWindow(bFlag);
		m_comboDamping.EnableWindow(bFlag);
		m_comboPreampMode.EnableWindow(bFlag);
		m_comboPreampGain.EnableWindow(bFlag);
	break;
	case LSA:
		m_comboTX.EnableWindow(false);
		m_comboRX.EnableWindow(false);
		m_comboHTOffOn.EnableWindow(false);
		m_comboLFPoint.EnableWindow(bFlag);
		m_spinPRF.EnableWindow(bFlag);
		m_editPRF.EnableWindow(bFlag);
		m_spinDacTriggerThreshold.EnableWindow(bFlag);
		m_editDacTriggerThreshold.EnableWindow(bFlag);
		m_spinDacBlanking.EnableWindow(bFlag);
		m_listDac.EnableWindow(bFlag);
		m_editDacBlanking.EnableWindow(bFlag);
		m_comboDacTriggerMode.EnableWindow(bFlag);
		m_comboDacEnable.EnableWindow(bFlag);
		m_spinRectifierDecay.EnableWindow(bFlag);
		m_editRectifierDecay.EnableWindow(bFlag);
		m_comboRectifierMode.EnableWindow(bFlag);
		m_comboFilterRange.EnableWindow(bFlag);
		m_comboFilterType.EnableWindow(bFlag);
		m_spinLinGain.EnableWindow(bFlag);
		m_editLinGain.EnableWindow(bFlag);
		m_comboLogPostFilter.EnableWindow(bFlag);
		m_spinLogGain.EnableWindow(bFlag);
		m_editLogGain.EnableWindow(bFlag);
		m_comboAmplifier.EnableWindow(bFlag);
		m_spinPulseVoltage.EnableWindow(false);
		m_editPulseVoltage.EnableWindow(false);
		m_spinPulseWidth.EnableWindow(false);
		m_editPulseWidth.EnableWindow(false);
		m_comboDamping.EnableWindow(false);
		m_comboPreampMode.EnableWindow(false);
		m_comboPreampGain.EnableWindow(false);
		break;
	}


	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDC_EDIT_PRF] ? m_editPRF.EnableWindow(true) : m_editPRF.EnableWindow(false);
	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDC_EDIT_DAC_THRESHOLD] ? bFlag = true : bFlag = false;
	m_spinDacTriggerThreshold.EnableWindow(bFlag);
	m_editDacTriggerThreshold.EnableWindow(bFlag);
	m_spinDacBlanking.EnableWindow(bFlag);
	m_listDac.EnableWindow(bFlag);
	m_editDacBlanking.EnableWindow(bFlag);
	m_comboDacTriggerMode.EnableWindow(bFlag);
	m_comboDacEnable.EnableWindow(bFlag);
	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDC_COMBO_FILTER_TYPE] ? bFlag = true : bFlag = false;
	m_spinRectifierDecay.EnableWindow(bFlag);
	m_editRectifierDecay.EnableWindow(bFlag);
	m_comboRectifierMode.EnableWindow(bFlag);
	m_comboFilterRange.EnableWindow(bFlag);
	m_comboFilterType.EnableWindow(bFlag);
	m_comboLFPoint.EnableWindow(bFlag);
	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDC_COMBO_AMPLIFIER] ? bFlag = true : bFlag = false;
	m_comboTX.EnableWindow(bFlag);
	m_comboRX.EnableWindow(bFlag);
	m_comboAmplifier.EnableWindow(bFlag);
	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDC_COMBO_PREAMP_GAIN] ? bFlag = true : bFlag = false;
	m_comboHTOffOn.EnableWindow(bFlag);
	m_spinPulseVoltage.EnableWindow(bFlag);
	m_editPulseVoltage.EnableWindow(bFlag);
	m_spinPulseWidth.EnableWindow(bFlag);
	m_editPulseWidth.EnableWindow(bFlag);
	m_comboDamping.EnableWindow(bFlag);
	m_comboPreampMode.EnableWindow(bFlag);
	m_comboPreampGain.EnableWindow(bFlag);
	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDC_EDIT_LIN_GAIN] ? bFlag = true : bFlag = false;
	m_editLinGain.EnableWindow(bFlag);
	m_spinLogGain.EnableWindow(bFlag);
	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDC_EDIT_LOG_GAIN] ? bFlag = true : bFlag = false;
	m_spinLogGain.EnableWindow(bFlag);
	m_editLogGain.EnableWindow(bFlag);
}



void CTimeSlotPr30::CopyTimeSlotVariables()
{
	int nEntryTimeSlot, nCurrentTimeSlot;

	nEntryTimeSlot = nCurrentTimeSlot = theApp.m_UtUser.m_Global.nTimeSlot;

	GatesData* pGate = &theApp.m_UtUser.m_TS[nEntryTimeSlot].Gate;
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[nEntryTimeSlot].Adc;
	DSP200Data* pDsp = &theApp.m_UtUser.m_TS[nEntryTimeSlot].Dsp;

	for(int nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {

		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_PREAMP_GAIN) && (theApp.m_UtUser.m_TS[nTS].Pr30.nPreampGain - theApp.m_UtUser.m_TS[m_nTS].Pr30.nPreampGain)) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.nPreampGain = theApp.m_UtUser.m_TS[m_nTS].Pr30.nPreampGain;
			theApp.m_UtUser.Pr30PreampGain(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_PREAMP_MODE) && (theApp.m_UtUser.m_TS[nTS].Pr30.nPreampMode - theApp.m_UtUser.m_TS[m_nTS].Pr30.nPreampMode)) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.nPreampMode =  theApp.m_UtUser.m_TS[m_nTS].Pr30.nPreampMode;
			theApp.m_UtUser.Pr30PreampMode(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_DAMPING) && (theApp.m_UtUser.m_TS[nTS].Pr30.nDamping - theApp.m_UtUser.m_TS[m_nTS].Pr30.nDamping)) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.nDamping = theApp.m_UtUser.m_TS[m_nTS].Pr30.nDamping;
			theApp.m_UtUser.Pr30Damping(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_PULSE_VOLTAGE) && (theApp.m_UtUser.m_TS[nTS].Pr30.nPulseVoltage - theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage)) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.nPulseVoltage = theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseVoltage;
			theApp.m_UtUser.Pr30PulseVoltage(&theApp.m_UtUser.m_TS[nTS].Pr30);
			theApp.m_UtUser.Pr30PulseVoltageMSC(&theApp.m_UtUser.m_TS[nTS].Pr30);
			theApp.m_UtUser.MailboxHt(TRUE,theApp.m_UtUser.m_TS[nTS].Pr30.nPulseVoltage);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_PULSE_WIDTH) && (theApp.m_UtUser.m_TS[nTS].Pr30.nPulseWidth - theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth)) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.nPulseWidth = theApp.m_UtUser.m_TS[m_nTS].Pr30.nPulseWidth;
			theApp.m_UtUser.Pr30PulseWidth(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_LINEAR_GAIN) && (theApp.m_UtUser.m_TS[nTS].Pr30.fLinGain - theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain)) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.fLinGain = theApp.m_UtUser.m_TS[m_nTS].Pr30.fLinGain;
			theApp.m_UtUser.Pr30LinFloatGain(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_LOG_GAIN) && (theApp.m_UtUser.m_TS[nTS].Pr30.fLogGain - theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain)) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.fLogGain = theApp.m_UtUser.m_TS[m_nTS].Pr30.fLogGain;
			theApp.m_UtUser.Pr30LogFloatGain(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_OUTPUT_MODE) && (theApp.m_UtUser.m_TS[nTS].Pr30.nOutputSelect - theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputSelect)) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.nOutputSelect = theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputSelect;
			theApp.m_UtUser.m_TS[nTS].Pr30.nOutputOffsetEnable = theApp.m_UtUser.m_TS[m_nTS].Pr30.nOutputOffsetEnable;
			theApp.m_UtUser.m_TS[nTS].Adc.nRfType = pAdc->nRfType;
			theApp.m_UtUser.Pr30OutputSelect(&theApp.m_UtUser.m_TS[nTS].Pr30);
			theApp.m_UtUser.Pr30OutputOffsetEnable(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_TYPE) && ((theApp.m_UtUser.m_TS[nTS].Pr30.nFilterType - theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterType) || (theApp.m_UtUser.m_TS[nTS].Pr30.nFilterMode - theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterMode) || (theApp.m_UtUser.m_TS[nTS].Pr30.nBandPassWidth - theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassWidth))) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.nFilterType = theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterType;
			theApp.m_UtUser.m_TS[nTS].Pr30.nFilterMode = theApp.m_UtUser.m_TS[m_nTS].Pr30.nFilterMode;
			theApp.m_UtUser.m_TS[nTS].Pr30.nBandPassWidth = theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassWidth;
			theApp.m_UtUser.Pr30FilterMode(&theApp.m_UtUser.m_TS[nTS].Pr30);
			theApp.m_UtUser.Pr30BandPassWidth(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_CENTER) && (theApp.m_UtUser.m_TS[nTS].Pr30.nBandPassCenterFrequency - theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassCenterFrequency)) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.nBandPassCenterFrequency = theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassCenterFrequency;
			theApp.m_UtUser.Pr30CenterFrequency(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_RANGE) && ((theApp.m_UtUser.m_TS[nTS].Pr30.nLowPassRange - theApp.m_UtUser.m_TS[m_nTS].Pr30.nLowPassRange) || (theApp.m_UtUser.m_TS[nTS].Pr30.nBandPassRange - theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassRange))) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.nLowPassRange = theApp.m_UtUser.m_TS[m_nTS].Pr30.nLowPassRange;
			theApp.m_UtUser.m_TS[nTS].Pr30.nBandPassRange = theApp.m_UtUser.m_TS[m_nTS].Pr30.nBandPassRange;
			theApp.m_UtUser.Pr30FilterRange(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}
		if((theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_CUT_OFF) && (theApp.m_UtUser.m_TS[nTS].Pr30.nLinTimeConstant - theApp.m_UtUser.m_TS[m_nTS].Pr30.nLinTimeConstant)) {
			if(nTS - nCurrentTimeSlot) theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot = nTS].Pr30);
			theApp.m_UtUser.m_TS[nTS].Pr30.nLinTimeConstant = theApp.m_UtUser.m_TS[m_nTS].Pr30.nLinTimeConstant;
			theApp.m_UtUser.Pr30LinearTimeConstant(&theApp.m_UtUser.m_TS[nTS].Pr30);
		}

	}
	theApp.m_UtUser.m_Global.nTimeSlot = nEntryTimeSlot;
	if(nEntryTimeSlot - nCurrentTimeSlot) {
		theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[m_nTS].Pr30);
	}
}

void CTimeSlotPr30::RedrawLayout()
{
	if (GetSafeHwnd() == NULL) return;
	if(m_bSubset != true) return;

	CRect rr,rrBox;
	CWnd *pWnd;


	pWnd = GetDlgItem(IDC_STATIC_PREAMP);
	pWnd->GetWindowRect(rr);
	ScreenToClient(rr);
	rrBox = rr;
	rr.top += 14;
	RedrawControl(GetDlgItem(IDC_STATIC_PREAMP_GAIN),		&m_comboPreampGain,	NULL, theApp.m_LastSettings.n64SubsetUtVariable & _UT_PREAMP_GAIN,&rr);
	RedrawControl(GetDlgItem(IDC_STATIC_PREAMP_MODE),		&m_comboPreampMode,	NULL, theApp.m_LastSettings.n64SubsetUtVariable & _UT_PREAMP_MODE,&rr);
	RedrawControl(GetDlgItem(IDC_STATIC_PREAMP_DAMPING),	&m_comboDamping,	NULL, theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAMPING,&rr);
	RedrawControl(GetDlgItem(IDC_STATIC_PULSE_WIDTH),		&m_editPulseWidth,	&m_spinPulseWidth, theApp.m_LastSettings.n64SubsetUtVariable & _UT_PULSE_WIDTH,&rr);
	RedrawControl(GetDlgItem(IDC_STATIC_PULSE_VOLTAGE),		&m_editPulseVoltage,&m_spinPulseVoltage, theApp.m_LastSettings.n64SubsetUtVariable & _UT_PULSE_VOLTAGE,&rr);
	RedrawControl(GetDlgItem(IDC_STATIC_PRF),				&m_editPRF,			&m_spinPRF, theApp.m_LastSettings.n64SubsetUtVariable & _UT_PRF,&rr);
	rrBox.bottom = rr.top;
	pWnd->SetWindowPos(NULL,rrBox.left,rrBox.top,rrBox.Width(),rrBox.Height(),NULL);

	pWnd = GetDlgItem(IDC_STATIC_PREAMP);
	pWnd->GetWindowRect(rr);
	ScreenToClient(rr);
	rr.top = rr.bottom + 10;
	RedrawControl(GetDlgItem(IDC_STATIC_PULSER),			&m_comboHTOffOn,	NULL, theApp.m_LastSettings.n64SubsetUtVariable & _UT_PULSER_OFF_ON,&rr);
	RedrawControl(GetDlgItem(IDC_STATIC_AMPLIFIER),			&m_comboAmplifier,	NULL, theApp.m_LastSettings.n64SubsetUtVariable & _UT_OUTPUT_MODE,&rr);
	RedrawControl(GetDlgItem(IDC_STATIC_TRANSMITTER),		&m_comboTX,	NULL,	theApp.m_LastSettings.n64SubsetUtVariable & _UT_TX,&rr);
	RedrawControl(GetDlgItem(IDC_STATIC_RECEIVER),			&m_comboRX,	NULL,	theApp.m_LastSettings.n64SubsetUtVariable & _UT_RX,&rr);
}

void CTimeSlotPr30::RedrawControl(CWnd *pStatic, CWnd *pCtrl,CSpinButtonCtrl* pSpin, __int64 n64Mask, CRect *prr)
{

	bool bFlag = false;
	if(n64Mask) bFlag = true;
	CRect rrCtrl,rrClient,rrSpin;
	int nHeight = 13;

	rrSpin = CRect(0,0,0,0);
	if(pSpin) pSpin->GetWindowRect(&rrSpin);

	if(pStatic) {
		pStatic->GetWindowRect(&rrCtrl);
		ScreenToClient(rrCtrl);
		pStatic->SetWindowPos(NULL,rrCtrl.left,prr->top,rrCtrl.Width(),rrCtrl.Height(),NULL);
		pStatic->ShowWindow(bFlag);
	}
	if(pCtrl) {
		pCtrl->ShowWindow(bFlag);
		pCtrl->GetWindowRect(&rrCtrl);
		ScreenToClient(rrCtrl);
		pCtrl->SetWindowPos(NULL,rrCtrl.left,prr->top,rrCtrl.Width()+ rrSpin.Width(),rrCtrl.Height(),NULL);
		if(pSpin) {
			if(bFlag == true) {
				pSpin->SetBuddy(pCtrl);
			} else {
				pSpin->ShowWindow(bFlag);
			}
		}
		nHeight = rrCtrl.Height() + 3;
	}

	if(bFlag == true) {
		prr->top += nHeight;
		prr->bottom += nHeight;
	};
}


BOOL CTimeSlotPr30::OnSetActive() 
{
	if(theApp.m_LastSettings.nDesiredDevice!=NOT_PHASED_ARRAY) {
		theApp.m_LastSettings.nDesiredDevice=NOT_PHASED_ARRAY;
		theApp.m_UtUser.Pr30UmbilicalSelect(0);
		theApp.m_UtUser.ResetMaxTimeSlots();
		theApp.m_UtUser.Initialize(theApp.m_nJobNumber);
	}

	theApp.m_UtUser.m_Global.nTimeSlot = m_nTS;
	theApp.m_UtUser.SelectWriteTimeslot();
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nSyncPulseTimeSlot = m_nTS+1;
	theApp.m_UtUser.Pr30SyncPulseTimeSlot(&theApp.m_UtUser.m_TS[m_nTS].Pr30);

	UpdateAllControls();

	theApp.m_UtUser.m_TS[0].nAmplifier=USL_PR30;

	return CPropertyPage::OnSetActive();
}
