// LSAPage.cpp : implementation file
//

/****
	Values are saved in global read into local variables on startup 
	and written back in the destructor. Using local variables inbetween.

  ******/



#include "stdafx.h"
#include "USLScanner.h"
#include "LSAPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLSAPage dialog
//IMPLEMENT_DYNCREATE(CLSAPage, CPropertyPage)

CLSAPage::CLSAPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CLSAPage::IDD)
{
	//{{AFX_DATA_INIT(CLSAPage)
	//}}AFX_DATA_INIT
	m_nHighPassFilter = theApp.m_UtUser.m_Global.LSA.nHighPassFilter; 
	m_nLowPassFilter  = theApp.m_UtUser.m_Global.LSA.nLowPassFilter;

	m_pEditSpinItem = NULL;
	m_pPr30 = &theApp.m_UtUser.m_TS[m_nTS = 0].Pr30;
	m_pEditSpinItem=NULL;
	m_nDacIndex=0;

}


void CLSAPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLSAPage)
	DDX_Control(pDX, IDC_SPIN_PRF, m_spinPRF);
	DDX_Control(pDX, IDC_EDIT_PRF, m_editPRF);
	DDX_Control(pDX, IDC_STATIC_TEXT_2, m_text2);
	DDX_Control(pDX, IDC_STATIC_TEXT_0, m_text0);
	DDX_Control(pDX, IDC_STATIC_TEXT_1, m_text1);
	DDX_Control(pDX, IDC_STATIC_GROUP_0, m_buttonGroup);
	DDX_Control(pDX, IDC_COMBO_LF_POINT, m_comboLFPoint);
	DDX_Control(pDX, IDC_SPIN_DAC_THRESHOLD, m_spinDacTriggerThreshold);
	DDX_Control(pDX, IDC_EDIT_DAC_THRESHOLD, m_editDacTriggerThreshold);
	DDX_Control(pDX, IDC_SPIN_DAC_BLANKING, m_spinDacBlanking);
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
	DDX_Text(pDX, IDC_STATIC_BLANKING_TEXT, m_staticBlankingText);
	DDX_Control(pDX, IDC_LIST_DAC_LIST, m_listDac);
	DDX_Control(pDX, IDC_COMBO_GATE, m_comboGate);
	DDX_Control(pDX, IDC_BUTTON_CALIBRATE, m_buttonNormalize);
	DDX_Control(pDX, IDC_EDIT_FOCAL_LAW_GAIN, m_editFocalLawGain);
	DDX_Control(pDX, IDC_SPIN_FOCAL_LAW_GAIN, m_spinFocalLawGain);
	DDX_Control(pDX, IDC_COMBO_LOW_PASS, m_comboLowPass);
	DDX_Control(pDX, IDC_COMBO_HIGH_PASS, m_comboHighPass);
	DDX_Control(pDX, IDC_SPIN_VOLTAGE, m_spinVoltage);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE, m_editVoltage);
	DDX_Control(pDX, IDC_COMBO_PREAMPGAIN, m_comboPreAmpGain);
	DDX_Control(pDX, IDC_SPIN_VIEWLAW, m_spinViewLaw);
	DDX_Control(pDX, IDC_EDIT_VIEWLAW, m_editViewLaw);
	DDX_Control(pDX, IDC_SPIN_PULSEWIDTH, m_spinPulseWidth);
	DDX_Control(pDX, IDC_EDIT_PULSEWIDTH, m_editPulseWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLSAPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLSAPage)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_PULSEWIDTH, OnChangeEditPulsewidth)
	ON_EN_CHANGE(IDC_EDIT_VIEWLAW, OnChangeEditViewlaw)
	ON_CBN_SELCHANGE(IDC_COMBO_PREAMPGAIN, OnSelchangeComboPreampgain)
	ON_EN_CHANGE(IDC_EDIT_VOLTAGE, OnChangeEditVoltage)
	ON_CBN_SELCHANGE(IDC_COMBO_LOW_PASS, OnSelchangeComboLowPass)
	ON_CBN_SELCHANGE(IDC_COMBO_HIGH_PASS, OnSelchangeComboHighPass)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATE, OnButtonNormalize)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_FOCAL_LAW_GAIN, OnChangeEditFocalLawGain)
	ON_BN_CLICKED(IDC_BUTTON_ZERO_GAINS, OnButtonZeroGains)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FOCAL_LAW_GAIN, OnDeltaposSpinFocalLawGain)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE, OnSelchangeComboGate)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DAC_LIST, OnClickListDacList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DAC_LIST, OnDblclkListDacList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DAC_LIST, OnGetdispinfoListDacList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DAC_LIST, OnRclickListDacList)
	ON_EN_CHANGE(IDC_EDIT_LIN_GAIN, OnChangeEditLinGain)
	ON_CBN_SELCHANGE(IDC_COMBO_AMPLIFIER, OnSelchangeComboAmplifier)
	ON_EN_CHANGE(IDC_EDIT_DAC_BLANKING, OnChangeEditDacBlanking)
	ON_CBN_SELCHANGE(IDC_COMBO_DAC_TRIGGER_MODE, OnSelchangeComboDacTriggerMode)
	ON_CBN_SELCHANGE(IDC_COMBO_DAC_MODE, OnSelchangeComboDacMode)
	ON_CBN_SELCHANGE(IDC_COMBO_RECTIFIER_MODE, OnSelchangeComboRectifierMode)
	ON_EN_CHANGE(IDC_EDIT_RECTIFIER_DECAY, OnChangeEditRectifierDecay)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_RANGE, OnSelchangeComboFilterRange)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, OnSelchangeComboFilterType)
	ON_EN_CHANGE(IDC_EDIT_LOG_GAIN, OnChangeEditLogGain)
	ON_CBN_SELCHANGE(IDC_COMBO_POST_FILTER, OnSelchangeComboPostFilter)
	ON_CBN_SELCHANGE(IDC_COMBO_LF_POINT, OnSelchangeComboLfPoint)
	ON_COMMAND(ID_DAC_CLEARALLGAINS, OnDacClearallgains)
	ON_COMMAND(ID_DAC_CLEAREVERYTHING, OnDacCleareverything)
	ON_COMMAND(ID_DAC_DELETEPT, OnDacDeletept)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_PRF, OnChangeEditPrf)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,DacTableChanged)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLSAPage message handlers

BOOL CLSAPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;
	UDACCEL	Accel;
	Accel.nInc=5;
	Accel.nSec=1;
	
	m_spinPulseWidth.SetRange(0,63);
	m_spinPRF.SetRange(100,10000);

	
	m_editViewLaw.SetWindowText(_T("1"));

	theApp.m_LSA.CmdSetActiveFL(theApp.m_LSA.m_nScopeViewLaw);
	theApp.m_LSA.CmdSetPPGain(theApp.m_UtUser.m_Global.LSA.nPreAmpGain);

	m_spinViewLaw.SetRange(0,127);
	m_spinViewLaw.SetPos(theApp.m_LSA.m_nScopeViewLaw);

	m_comboPreAmpGain.InsertString(0,_T("0dB"));
	m_comboPreAmpGain.InsertString(1,_T("10dB"));
	m_comboPreAmpGain.InsertString(2,_T("20dB"));
	m_comboPreAmpGain.InsertString(3,_T("30dB"));


	theApp.m_LSA.CmdSetHTVoltage(theApp.m_UtUser.m_Global.LSA.nPulseVoltage); 
	m_spinVoltage.SetRange(1,255);
	
	m_comboLowPass.InsertString(0,_T("3.5MHz"));
	m_comboLowPass.InsertString(1,_T("5MHz"));
	m_comboLowPass.InsertString(2,_T("10MHz"));
	m_comboLowPass.InsertString(3,_T("WideBand"));

	m_comboHighPass.InsertString(0,_T("WideBand"));
	m_comboHighPass.InsertString(1,_T("1MHz"));
	m_comboHighPass.InsertString(2,_T("2MHz"));
	m_comboHighPass.InsertString(3,_T("5MHz"));

	m_spinFocalLawGain.SetRange(0,500);
	theApp.m_LSA.CmdSetFLCollectiveGain(theApp.m_LSA.m_nScopeViewLaw,theApp.m_LSA.GetFLGain(theApp.m_LSA.m_nScopeViewLaw));


	m_comboGate.LimitText(sizeof theApp.m_UtUser.m_TS[0].Gate.cName[0] - 1);
	for(int gg=0;gg<8;gg++) {
		m_comboGate.AddString(theApp.m_UtUser.m_TS[0].Gate.cName[gg]);
	}


	/////////////////// PR30 stuff
	Buff.LoadString(IDS_RF);			m_comboAmplifier.AddString(Buff);
	Buff.LoadString(IDS_Rectified);		m_comboAmplifier.AddString(Buff);
	Buff.LoadString(IDS_Logorithmic);	m_comboAmplifier.AddString(Buff);

	m_comboLogPostFilter.AddString(_T("500 KHz"));
	m_comboLogPostFilter.AddString(_T("1 MHz"));
	m_comboLogPostFilter.AddString(_T("2.5 MHz"));
	m_comboLogPostFilter.AddString(_T("5 MHz"));
	m_comboLogPostFilter.AddString(_T("10 MHz"));
	Buff.LoadString(IDS_WideBand); m_comboLogPostFilter.AddString(Buff);

	Buff.LoadString(IDS_Low_Pass); m_comboFilterType.AddString(Buff);
	Buff.LoadString(IDS_Band_Pass_Narrow); m_comboFilterType.AddString(Buff);
	Buff.LoadString(IDS_Band_Pass_Wide); m_comboFilterType.AddString(Buff);

	m_spinCentreFrequency.SetRange(3,63);

	Buff.LoadString(IDS_Positive); m_comboRectifierMode.AddString(Buff);
	Buff.LoadString(IDS_Negative); m_comboRectifierMode.AddString(Buff);
	Buff.LoadString(IDS_Full_Wave); m_comboRectifierMode.AddString(Buff);

	m_spinRectifierDecay.SetRange(0,100);

	Buff.LoadString(IDS_Off); m_comboDacEnable.AddString(Buff);
	Buff.LoadString(IDS_On); m_comboDacEnable.AddString(Buff);

	Buff.LoadString(IDS_Mainbang); m_comboDacTriggerMode.AddString(Buff);
	Buff.LoadString(IDS_Interface); m_comboDacTriggerMode.AddString(Buff);

	m_comboLFPoint.AddString(_T("0.5 MHz"));
	m_comboLFPoint.AddString(_T("1.5 MHz"));

	m_spinDacBlanking.SetRange32(0,32767);
	m_spinDacTriggerThreshold.SetRange(0,200);

	m_spinLogGain.SetRange(0,299);
	m_spinLinGain.SetRange(0,1000);
	m_spinLogGain.SetAccel(1,&Accel);
	m_spinLinGain.SetAccel(1,&Accel);

	CreateColumns();


	UpdateAllControls();


	//Timeslots TODO Is there a better way?
	theApp.m_LSA.MaintainUniformTSs(0);

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CLSAPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CLSAPage::OnDestroy() 
{
	CPropertyPage::OnDestroy();

	UpdateUTGlobalSettings();
	
}


void CLSAPage::UpdateUTGlobalSettings()
{
	theApp.m_UtUser.m_Global.LSA.nHighPassFilter = m_nHighPassFilter; 
	theApp.m_UtUser.m_Global.LSA.nLowPassFilter = m_nLowPassFilter;
}

void CLSAPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	int	nTemp;
	float fTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_PULSEWIDTH:
		nTemp=(pSpin->GetPos()&0xffff);
		Buff.Format(_T("%dns"),nTemp*10);
		m_editPulseWidth.SetWindowText(Buff);
		theApp.m_LSA.CmdSetTXPulseWidth(nTemp*10);
		theApp.m_UtUser.m_Global.LSA.nPulseWidth = nTemp*10;
		break;
	case IDC_SPIN_VIEWLAW:
		nTemp=(pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_LSA.m_nScopeViewLaw) {
			theApp.m_LSA.m_nScopeViewLaw = MinMax(&nTemp,0,theApp.m_LSA.nFocalLawsL-1);
			Buff.Format(_T("%d"),theApp.m_LSA.m_nScopeViewLaw+1);
			m_editViewLaw.SetWindowText(Buff);
			m_spinViewLaw.SetPos(theApp.m_LSA.m_nScopeViewLaw);
			theApp.m_LSA.CmdSetWriteFL(theApp.m_LSA.m_nScopeViewLaw+1);

			Buff.Format(_T("%.01fdB"),theApp.m_LSA.GetFLGain(theApp.m_LSA.m_nScopeViewLaw));
			m_editFocalLawGain.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_VOLTAGE:
		nTemp=(pSpin->GetPos()&0xffff);
		Buff.Format(_T("%d"),nTemp);
		if(nTemp - theApp.m_UtUser.m_Global.LSA.nPulseVoltage) {
			theApp.m_UtUser.m_Global.LSA.nPulseVoltage = nTemp;
			theApp.m_LSA.CmdSetHTVoltage(theApp.m_UtUser.m_Global.LSA.nPulseVoltage); 
			m_editVoltage.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_LOG_GAIN:
		fTemp = m_pPr30->fLogGain;
		m_pPr30->fLogGain = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		if(fTemp - m_pPr30->fLogGain) {
			Buff.Format(_T("%.01f dB"),m_pPr30->fLogGain);
			m_editLogGain.SetWindowText(Buff);
			theApp.m_UtUser.Pr30LogFloatGain(m_pPr30);
//			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_LIN_GAIN:
		fTemp = m_pPr30->fLinGain;
		m_pPr30->fLinGain = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		if(fTemp - m_pPr30->fLinGain) {
			Buff.Format(_T("%.01f dB"),m_pPr30->fLinGain);
			m_editLinGain.SetWindowText(Buff);
			theApp.m_UtUser.Pr30LinFloatGain(m_pPr30);
//			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_CENTRE_FREQ:
		nTemp = m_pPr30->nBandPassCenterFrequency;
		m_pPr30->nBandPassCenterFrequency=(pSpin->GetPos()&0xffff);
		if(nTemp - m_pPr30->nBandPassCenterFrequency) {
			theApp.m_UtUser.Pr30CalculateRealWorldCentreFrequency(m_pPr30,&Buff);
			m_editCentreFrequency.SetWindowText(Buff);
			theApp.m_UtUser.Pr30CenterFrequency(m_pPr30);
//			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_RECTIFIER_DECAY:
		nTemp = m_pPr30->nRectifierDecayPercentage;
		m_pPr30->nRectifierDecayPercentage=(pSpin->GetPos()&0xffff);
		if(nTemp - m_pPr30->nRectifierDecayPercentage) {
			Buff.Format(_T("%d %%"),m_pPr30->nRectifierDecayPercentage);
			m_editRectifierDecay.SetWindowText(Buff);
			theApp.m_UtUser.Pr30RectifierDecay(m_pPr30);
//			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_DAC_BLANKING:
		nTemp=m_pPr30->nDacBlanking;
		m_pPr30->nDacBlanking=(pSpin->GetPos()&0xffff)*10;
		if(nTemp-m_pPr30->nDacBlanking) {
			theApp.m_UtUser.Pr30BlankingIntToFloat(m_pPr30);
			theApp.m_UtUser.Pr30DacBlankingString(m_pPr30,&Buff);
			m_editDacBlanking.SetWindowText(Buff);
//			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_DAC_THRESHOLD:
		nTemp = m_pPr30->nDacTriggerThreshold;
		m_pPr30->nDacTriggerThreshold=MulDiv((pSpin->GetPos()&0xffff)-100,127,100)+128;
		if(nTemp - m_pPr30->nDacTriggerThreshold) {
			Buff.Format(_T("%d %%"),MulDiv(m_pPr30->nDacTriggerThreshold-128,theApp.m_LastSettings.nDacMaxThreshold,127));
			m_editDacTriggerThreshold.SetWindowText(Buff);
			theApp.m_UtUser.Pr30DacThreshold(m_pPr30);
//			CopyTimeSlotVariables();
		}
		break;
	case IDC_SPIN_PRF:
		theApp.m_UtUser.m_Global.nPrf=(pSpin->GetPos()&0xffff);
		Buff.Format(_T("%d"),theApp.m_UtUser.m_Global.nPrf);
		m_editPRF.SetWindowText(Buff);
		theApp.m_UtUser.SetPrf();
		break;
	}//switch


	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CLSAPage::OnDeltaposSpinFocalLawGain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;

	float fFLGain = theApp.m_LSA.GetFLGain(theApp.m_LSA.m_nScopeViewLaw);
	fFLGain += ((float)pNMUpDown->iDelta * 0.1f);
	if(fFLGain<0.0f) fFLGain = 0.0f;
	theApp.m_LSA.CmdSetFLCollectiveGain(theApp.m_LSA.m_nScopeViewLaw,fFLGain);

	Buff.Format(_T("%.01fdB"),fFLGain);
	m_editFocalLawGain.SetWindowText(Buff);
	
	*pResult = 0;
}

//PreAmp Controls
void CLSAPage::OnSelchangeComboPreampgain() 
{
	theApp.m_UtUser.m_Global.LSA.nPreAmpGain = m_comboPreAmpGain.GetCurSel() * 10;
	theApp.m_LSA.CmdSetPPGain(theApp.m_UtUser.m_Global.LSA.nPreAmpGain);

	Invalidate(true);

	UpdateUTGlobalSettings();
}

void CLSAPage::OnChangeEditPulsewidth() 
{
	CString Buff;

	m_editPulseWidth.GetWindowText(Buff);
	theApp.m_UtUser.m_Global.LSA.nPulseWidth = _ttoi(Buff);
	m_spinPulseWidth.SetPos((theApp.m_UtUser.m_Global.LSA.nPulseWidth/10));
	theApp.m_LSA.CmdSetTXPulseWidth(theApp.m_UtUser.m_Global.LSA.nPulseWidth);

	Invalidate(true);

	UpdateUTGlobalSettings();
	
}



void CLSAPage::OnChangeEditVoltage() 
{
	CString Buff;

	m_editVoltage.GetWindowText(Buff);
	theApp.m_UtUser.m_Global.LSA.nPulseVoltage = _ttoi(Buff);
	m_spinVoltage.SetPos(theApp.m_UtUser.m_Global.LSA.nPulseVoltage);	
	theApp.m_LSA.CmdSetHTVoltage(theApp.m_UtUser.m_Global.LSA.nPulseVoltage); 
	


}


//Filter Controls
void CLSAPage::OnSelchangeComboLowPass() 
{
	float fLowPassMHz, fHighPassMHz;

	switch(m_comboLowPass.GetCurSel()){
	case 0: fLowPassMHz = 3.5;
		break;
	case 1: fLowPassMHz=5;
		break;
	case 2: fLowPassMHz=10;
		break;
	case 3: fLowPassMHz =0xFF;
		break;
	}//switch

	switch(m_comboHighPass.GetCurSel()){
	case 0: fHighPassMHz=0;
		break;
	case 1:	fHighPassMHz=1;
		break;
	case 2: fHighPassMHz=2;
		break;
	case 3: fHighPassMHz=5;
		break;
	}//switch

	
	m_nLowPassFilter=m_comboLowPass.GetCurSel();
	m_nHighPassFilter=m_comboHighPass.GetCurSel();
	theApp.m_LSA.CmdSetFilters(fLowPassMHz,fHighPassMHz); 

	UpdateUTGlobalSettings();
			
}

void CLSAPage::OnSelchangeComboHighPass() 
{
	float fLowPassMHz, fHighPassMHz;

	switch(m_comboLowPass.GetCurSel()){
	case 0: fLowPassMHz = 3.5;
		break;
	case 1: fLowPassMHz=5;
		break;
	case 2: fLowPassMHz=10;
		break;
	case 3: fLowPassMHz =0xFF;
		break;
	}//switch

	switch(m_comboHighPass.GetCurSel()){
	case 0: fHighPassMHz=0;
		break;
	case 1:	fHighPassMHz=1;
		break;
	case 2: fHighPassMHz=2;
		break;
	case 3: fHighPassMHz=5;
		break;
	}//switch



	m_nLowPassFilter=m_comboLowPass.GetCurSel();
	m_nHighPassFilter=m_comboHighPass.GetCurSel();
	theApp.m_LSA.CmdSetFilters(fLowPassMHz,fHighPassMHz); 

	UpdateUTGlobalSettings();
	
}



//Individual Focal Law Settings
void CLSAPage::OnChangeEditViewlaw() 
{
	CString Buff;
	int nTemp = theApp.m_LSA.m_nScopeViewLaw;

	m_editViewLaw.GetWindowText(Buff);
	nTemp = _ttoi(Buff) - 1;

	if(nTemp - theApp.m_LSA.m_nScopeViewLaw) {
		theApp.m_LSA.m_nScopeViewLaw = MinMax(&nTemp,0,theApp.m_LSA.nFocalLawsL-1);

		m_spinViewLaw.SetPos(theApp.m_LSA.m_nScopeViewLaw);

		Buff.Format(_T("%.01fdB"),theApp.m_LSA.GetFLGain(theApp.m_LSA.m_nScopeViewLaw));
		m_editFocalLawGain.SetWindowText(Buff);

		theApp.m_LSA.CmdSetWriteFL(theApp.m_LSA.m_nScopeViewLaw+1);

		Invalidate(true);
	}

}


void CLSAPage::OnChangeEditFocalLawGain() 
{
	CString Buff;
	float fFLGain;

	m_editFocalLawGain.GetWindowText(Buff);
	_WTOF(Buff,fFLGain);

	m_spinFocalLawGain.SetPos((int)(fFLGain*10.0f));

	theApp.m_LSA.CmdSetFLCollectiveGain(theApp.m_LSA.m_nScopeViewLaw,fFLGain);
	
}



void CLSAPage::OnButtonNormalize()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	double dGain;
	int nFL;
	double dDesiredAmp;
	double dAmp[128],dMin,dMax;
	int nMaxFL=0;
	int	nMinFL=0;
	int nAmp;

	float fInitLinearGain = m_pPr30->fLinGain;
	int	nInitPreAmpGain = theApp.m_UtUser.m_Global.LSA.nPreAmpGain;

	for(nFL=0;nFL<theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws();nFL++) {
		theApp.m_LSA.CmdSetFLCollectiveGain(nFL,0);
	}

again_label:
	Sleep(500);
	for(nFL=0;nFL<theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws();nFL++) {

		nAmp = theApp.m_UtUser.m_TS[nFL].Gate.nAmplitude[theApp.m_LastSettings.nLSACalibrationGate];
		theApp.m_UtUser.m_TS[nFL].Gate.nAmplitude[theApp.m_LastSettings.nLSACalibrationGate] = nAmp;

		if(theApp.m_UtUser.m_TS[nFL].Gate.nAmplitude[theApp.m_LastSettings.nLSACalibrationGate] >= 127) {
			if(m_pPr30->fLinGain >= 1.0f) {
				m_pPr30->fLinGain -= 1.0f;
				theApp.m_UtUser.Pr30LinFloatGain(m_pPr30);
			} else {
				m_pPr30->fLinGain += 9.0f;
				theApp.m_UtUser.Pr30LinFloatGain(m_pPr30);
				if(theApp.m_UtUser.m_Global.LSA.nPreAmpGain >= 10) {
					theApp.m_UtUser.m_Global.LSA.nPreAmpGain -= 10;
					theApp.m_LSA.CmdSetPPGain(theApp.m_UtUser.m_Global.LSA.nPreAmpGain);
				} else {
					MessageBox(L"Error",L"Unable to normalize because one element is saturated",MB_ICONERROR|MB_OK);
					goto finish_label;
				}
			}
			goto again_label;
		}
	}

	dMax = -100.0;
	dMin = 100.0;
	for(nFL=0;nFL<theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws();nFL++) {
		dAmp[nFL] = (double)theApp.m_UtUser.m_TS[nFL].Gate.nAmplitude[theApp.m_LastSettings.nLSACalibrationGate] * 100.0 / 128.0;
		if(dAmp[nFL] > dMax) {
			dMax = dAmp[nFL];
			nMaxFL = nFL;
		}
		if(dAmp[nFL]<dMin) {
			dMin = dAmp[nFL];
			nMinFL = nFL;
		}
	}

	dDesiredAmp = dMax;
	if(theApp.m_LastSettings.nLSACalibrationGate > 0) {
		if(theApp.m_UtUser.m_TS[0].Gate.nThreshold[theApp.m_LastSettings.nLSACalibrationGate] >= 0) {
			dDesiredAmp = dMax;
		} else {
			dDesiredAmp = fabs(dMin);
		}
	}

	for(nFL=0;nFL<theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws();nFL++) {

		if(theApp.m_UtUser.m_TS[0].Gate.nThreshold[theApp.m_LastSettings.nLSACalibrationGate] >= 0) {
			dGain =	log10(dDesiredAmp/dAmp[nFL]) * 20;
		} else {
			dGain =	log10(dDesiredAmp/fabs(dAmp[nFL])) * 20;
		}
		
		
		theApp.m_LSA.CmdSetFLCollectiveGain(nFL,(float)dGain);
	}

finish_label:

	theApp.m_LSA.CmdSetPPGain(theApp.m_UtUser.m_Global.LSA.nPreAmpGain = nInitPreAmpGain);
	m_pPr30->fLinGain = fInitLinearGain;
	theApp.m_UtUser.Pr30LinFloatGain(m_pPr30);

	UpdateAllControls();
	pFrame->SendMessage(UI_UPDATE_PHASED_ARRAY_DLG);
}



//currently only called if probe type is cahnged to enable or disable controls
void CLSAPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(_T("%d"),theApp.m_LSA.m_nScopeViewLaw+1);
	m_editViewLaw.SetWindowText(Buff);
	m_spinViewLaw.SetPos(theApp.m_LSA.m_nScopeViewLaw);

	//Update local settings 
	m_nHighPassFilter = theApp.m_UtUser.m_Global.LSA.nHighPassFilter; 
	m_nLowPassFilter  = theApp.m_UtUser.m_Global.LSA.nLowPassFilter;

	Buff.Format(_T("%dns"),theApp.m_UtUser.m_Global.LSA.nPulseWidth);
	m_editPulseWidth.SetWindowText(Buff);

	m_spinPulseWidth.SetPos(theApp.m_UtUser.m_Global.LSA.nPulseWidth/10);
	theApp.m_LSA.CmdSetTXPulseWidth(theApp.m_UtUser.m_Global.LSA.nPulseWidth);

	m_comboPreAmpGain.SetCurSel(theApp.m_UtUser.m_Global.LSA.nPreAmpGain / 10);

	Buff.Format(_T("%dV"),theApp.m_UtUser.m_Global.LSA.nPulseVoltage);
	m_editVoltage.SetWindowText(Buff);
	m_spinVoltage.SetPos(theApp.m_UtUser.m_Global.LSA.nPulseVoltage);

	m_comboLowPass.SetCurSel(m_nLowPassFilter);

	m_comboHighPass.SetCurSel(m_nHighPassFilter);

	
	float fLowPassMHz,fHighPassMHz;
	switch(m_comboLowPass.GetCurSel()){
	case 0: fLowPassMHz = 3.5;
		break;
	case 1: fLowPassMHz=5;
		break;
	case 2: fLowPassMHz=10;
		break;
	case 3: fLowPassMHz =0xFF;
		break;
	}//switch

	switch(m_comboHighPass.GetCurSel()){
	case 0: fHighPassMHz=0;
		break;
	case 1:	fHighPassMHz=1;
		break;
	case 2: fHighPassMHz=2;
		break;
	case 3: fHighPassMHz=5;
		break;
	}//switch
	theApp.m_LSA.CmdSetFilters(fLowPassMHz,fHighPassMHz); 

	Buff.Format(_T("%.01fdB"),theApp.m_LSA.GetFLGain(theApp.m_LSA.m_nScopeViewLaw));
	m_editFocalLawGain.SetWindowText(Buff);

	m_spinFocalLawGain.SetPos((int)(theApp.m_LSA.GetFLGain(theApp.m_LSA.m_nScopeViewLaw) * 10.0f));

	m_comboGate.SetCurSel(theApp.m_LastSettings.nLSACalibrationGate);

	////////////// PR30 Stuff
	m_comboAmplifier.SetCurSel(m_pPr30->nOutputSelect);


	m_spinLogGain.SetPos((int)(m_pPr30->fLogGain*10.0));
	Buff.Format(_T("%.01f dB"),m_pPr30->fLogGain);
	m_editLogGain.SetWindowText(Buff);

	m_spinLinGain.SetPos((int)(m_pPr30->fLinGain*10.0));
	Buff.Format(_T("%.01f dB"),m_pPr30->fLinGain);
	m_editLinGain.SetWindowText(Buff);

	m_comboLogPostFilter.SetCurSel(m_pPr30->nLogPostDetectFilter);

	m_comboFilterType.SetCurSel(m_pPr30->nFilterType);
	SetFilterRangeCombo();

	m_spinCentreFrequency.SetPos(m_pPr30->nBandPassCenterFrequency);
	theApp.m_UtUser.Pr30CalculateRealWorldCentreFrequency(m_pPr30,&Buff);
	m_editCentreFrequency.SetWindowText(Buff);

	m_comboRectifierMode.SetCurSel(m_pPr30->nRectifierDetectionMode);
	Buff.Format(_T("%d %%"),m_pPr30->nRectifierDecayPercentage);
	m_editRectifierDecay.SetWindowText(Buff);
	m_spinRectifierDecay.SetPos(m_pPr30->nRectifierDecayPercentage);

	m_comboDacEnable.SetCurSel(m_pPr30->nDacEnable&1);
	m_comboDacTriggerMode.SetCurSel(m_pPr30->nDacTriggerMode&1);

	m_comboLFPoint.SetCurSel(m_pPr30->nLinTimeConstant);


	theApp.m_UtUser.Pr30DacBlankingString(m_pPr30,&Buff);
	m_editDacBlanking.SetWindowText(Buff);

	m_spinDacBlanking.SetPos(m_pPr30->nDacBlanking/10);
	m_spinDacTriggerThreshold.SetPos(MulDiv(m_pPr30->nDacTriggerThreshold-128,100,127)+100);
	Buff.Format(_T("%d %%"),MulDiv(m_pPr30->nDacTriggerThreshold-128,theApp.m_LastSettings.nDacMaxThreshold,127));
	m_editDacTriggerThreshold.SetWindowText(Buff);

	Buff.Format(_T("%d Hz"),theApp.m_UtUser.m_Global.nPrf);
	m_editPRF.SetWindowText(Buff);
	m_spinPRF.SetPos(theApp.m_UtUser.m_Global.nPrf);


	switch(m_pPr30->nDacTriggerMode) {
	case 0:	m_staticBlankingText.LoadString(IDS_Delay);
		break;
	case 1: m_staticBlankingText.LoadString(IDS_Blanking);
		break;
	}
	UpdateData(FALSE);

	FillList();


	SetAccessPrivelage();

}



void CLSAPage::OnButtonZeroGains() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int ii;
	CString Buff;

	//reset all gains
	for(ii=0;ii<theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws();ii++) {
	
		theApp.m_LSA.CmdSetFLCollectiveGain(ii,0);
		theApp.m_LSA.CmdSetGainTrim(ii, 0);

		if(ii==theApp.m_LSA.m_nScopeViewLaw) {
			Buff.Format(_T("%.01fdB"),theApp.m_LSA.GetFLGain(theApp.m_LSA.m_nScopeViewLaw));
			m_editFocalLawGain.SetWindowText(Buff);
		}
	}
	UpdateAllControls();
	pFrame->SendMessage(UI_UPDATE_PHASED_ARRAY_DLG);
}


void CLSAPage::OnSelchangeComboGate() 
{
	theApp.m_LastSettings.nLSACalibrationGate = m_comboGate.GetCurSel();
}

void CLSAPage::CreateColumns()
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

void CLSAPage::FillList()
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


void CLSAPage::OnDblclkListDacList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	m_listDac.GetWindowRect(CtrlRect);
	m_listDac.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	SAFE_DELETE( m_pEditSpinItem );
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**)&m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 1:	m_pEditSpinItem->Initialize(&m_pPr30->fDacDelay[m_pPr30->nDacTable][pDispInfo->item.mask],0.0f,1400.0f,0.0999f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 2:	m_pEditSpinItem->Initialize(&m_pPr30->fDacGain[m_pPr30->nDacTable][pDispInfo->item.mask],0.0f,80.0f,0.0999f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	}
	*pResult = 0;
}

void CLSAPage::OnGetdispinfoListDacList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	m_pPr30->nDacTable%=3;
	str[0]=0;
	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str,_T("%.02f"),m_pPr30->fDacDelay[m_pPr30->nDacTable][pDispInfo->item.iItem]);
		break;
	case 2:	swprintf_s(str,_T("%.02f"),m_pPr30->fDacGain[m_pPr30->nDacTable][pDispInfo->item.iItem]);
		break;
	}
	pDispInfo->item.pszText=str;	
	*pResult = 0;
}

HRESULT CLSAPage::DacTableChanged(WPARAM,LPARAM)
{
	theApp.m_UtUser.Pr30CalculateDacData(m_pPr30);
	return NULL;
}

void CLSAPage::OnClickListDacList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SAFE_DELETE( m_pEditSpinItem );
	
	*pResult = 0;
}


void CLSAPage::OnRclickListDacList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	SAFE_DELETE( m_pEditSpinItem );

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

void CLSAPage::SetAccessPrivelage()
{
	if (GetSafeHwnd() == NULL) return;

	bool bFlag = false;
	if(theApp.m_nLogonLevelMask &  theApp.m_cAccess[IDD_PR30_TIMESLOT_DIALOG]) {
		bFlag = true;
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

	switch(m_pPr30->nOutputSelect) {
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
	switch(m_pPr30->nFilterMode) {
	case 0: m_spinCentreFrequency.EnableWindow(FALSE);
		m_editCentreFrequency.EnableWindow(FALSE);
		break;
	case 1: m_spinCentreFrequency.EnableWindow(bFlag);
		m_editCentreFrequency.EnableWindow(bFlag);
		break;
	}

	m_comboLFPoint.EnableWindow(bFlag);
//	m_spinPRF.EnableWindow(bFlag);
//	m_editPRF.EnableWindow(bFlag);
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

}

void CLSAPage::SetFilterRangeCombo()
{
	CString Buff;

	m_comboFilterRange.ResetContent();
	switch(m_pPr30->nFilterType) {
	case 0:
		m_comboFilterRange.AddString(_T("2.5 MHz"));
		m_comboFilterRange.AddString(_T("5 MHz"));
		m_comboFilterRange.AddString(_T("10 MHz"));
		m_comboFilterRange.AddString(_T("15 MHz"));
		Buff.LoadString(IDS_Full); m_comboFilterRange.AddString(Buff);
		m_comboFilterRange.SetCurSel(m_pPr30->nLowPassRange);
		break;
	case 1:
	case 2:
		m_comboFilterRange.AddString(_T("0.5-2 MHz"));
		m_comboFilterRange.AddString(_T("1-4 MHz"));
		m_comboFilterRange.AddString(_T("2-8 MHz"));
		m_comboFilterRange.AddString(_T("4-16 MHz"));
		m_comboFilterRange.AddString(_T("8-32 MHz"));
		m_comboFilterRange.SetCurSel(m_pPr30->nBandPassRange);
		break;
	}

}

void CLSAPage::OnChangeEditLinGain() 
{
	CString Buff;
	float fTemp = m_pPr30->fLinGain;

	m_editLinGain.GetWindowText(Buff);
	_WTOF(Buff,m_pPr30->fLinGain);
	if(fTemp != m_pPr30->fLinGain) {
		theApp.m_UtUser.Pr30LinFloatGain(m_pPr30);
		m_spinLinGain.SetPos((int)(m_pPr30->fLinGain*10.0));
//		CopyTimeSlotVariables();
	}
	
}

void CLSAPage::OnSelchangeComboAmplifier() 
{
	switch(m_pPr30->nOutputSelect=m_comboAmplifier.GetCurSel()) {
	case 0:	theApp.m_UtUser.m_TS[m_nTS].Adc.nRfType=RFTYPELINEAR;
		m_pPr30->nOutputOffsetEnable=0;
		break;
	case 1:	theApp.m_UtUser.m_TS[m_nTS].Adc.nRfType=RFTYPERECTIFIED;
		m_pPr30->nOutputOffsetEnable=0;
		break;
	case 2:	theApp.m_UtUser.m_TS[m_nTS].Adc.nRfType=RFTYPELOG;
		m_pPr30->nOutputOffsetEnable=1;
		break;
	}
	theApp.m_UtUser.Pr30OutputSelect(m_pPr30);
	theApp.m_UtUser.Pr30OutputOffsetEnable(m_pPr30);
	switch(m_pPr30->nOutputSelect) {
	case 0:	theApp.m_UtUser.Pr30LinFloatGain(m_pPr30);
		break;
	case 1:	theApp.m_UtUser.Pr30LinFloatGain(m_pPr30);
		break;
	case 2:	theApp.m_UtUser.Pr30LogFloatGain(m_pPr30);
		break;
	}
//	CopyTimeSlotVariables();
	SetAccessPrivelage();
}

void CLSAPage::OnChangeEditLogGain() 
{
	CString Buff;
	float fTemp = m_pPr30->fLogGain;

	m_editLogGain.GetWindowText(Buff);
	_WTOF(Buff,m_pPr30->fLogGain);
	if(fTemp != m_pPr30->fLogGain) {
		theApp.m_UtUser.Pr30LogFloatGain(m_pPr30);
		m_spinLogGain.SetPos((int)(m_pPr30->fLogGain*10.0));
//		CopyTimeSlotVariables();
	}

}

void CLSAPage::OnSelchangeComboPostFilter() 
{
	m_pPr30->nLogPostDetectFilter=m_comboLogPostFilter.GetCurSel();
	theApp.m_UtUser.Pr30LogPostDetectFilter(m_pPr30);
//	CopyTimeSlotVariables();
}

void CLSAPage::OnSelchangeComboFilterType() 
{
	switch(m_pPr30->nFilterType = m_comboFilterType.GetCurSel()) {
	case 0: m_pPr30->nFilterMode = 0;
		break;
	case 1: m_pPr30->nFilterMode = 1;
		m_pPr30->nBandPassWidth=0;
		break;
	case 2: m_pPr30->nFilterMode = 1;
		m_pPr30->nBandPassWidth=1;
		break;
	}
	theApp.m_UtUser.Pr30FilterMode(m_pPr30);
	theApp.m_UtUser.Pr30BandPassWidth(m_pPr30);
	SetFilterRangeCombo();
//	CopyTimeSlotVariables();
	SetAccessPrivelage();
}

void CLSAPage::OnSelchangeComboFilterRange() 
{
	CString Buff;

	switch(m_pPr30->nFilterType) {
	case 0:	m_pPr30->nLowPassRange = m_comboFilterRange.GetCurSel();
		break;
	case 1:
	case 2:	m_pPr30->nBandPassRange = m_comboFilterRange.GetCurSel();
		theApp.m_UtUser.Pr30CalculateRealWorldCentreFrequency(m_pPr30,&Buff);
		m_editCentreFrequency.SetWindowText(Buff);
		break;
	}
	theApp.m_UtUser.Pr30FilterRange(m_pPr30);
//	CopyTimeSlotVariables();

}




void CLSAPage::OnChangeEditRectifierDecay() 
{
	CString Buff;
	int nTemp = m_pPr30->nRectifierDecayPercentage;

	m_editRectifierDecay.GetWindowText(Buff);
	m_pPr30->nRectifierDecayPercentage = _ttoi(Buff);
	if(nTemp - m_pPr30->nRectifierDecayPercentage) {
		m_spinRectifierDecay.SetPos(m_pPr30->nRectifierDecayPercentage);
		theApp.m_UtUser.Pr30RectifierDecay(m_pPr30);
//		CopyTimeSlotVariables();
	}

}

void CLSAPage::OnSelchangeComboRectifierMode() 
{
	m_pPr30->nRectifierDetectionMode=m_comboRectifierMode.GetCurSel();
	theApp.m_UtUser.Pr30RectifierMode(m_pPr30);
//	CopyTimeSlotVariables();
}

void CLSAPage::OnSelchangeComboDacMode() 
{
	m_pPr30->nDacEnable = m_comboDacEnable.GetCurSel();
	theApp.m_UtUser.Pr30DacEnable(m_pPr30);
//	CopyTimeSlotVariables();
}



void CLSAPage::OnSelchangeComboDacTriggerMode() 
{
	CString Buff;

	m_pPr30->nDacTriggerMode = m_comboDacTriggerMode.GetCurSel();

	theApp.m_UtUser.Pr30DacTriggerMode(m_pPr30);
	theApp.m_UtUser.Pr30DacBlanking(m_pPr30);
//	CopyTimeSlotVariables();
	UpdateAllControls();
	
}


void CLSAPage::OnChangeEditDacBlanking() 
{
	CString Buff;
	int	nTemp = m_pPr30->nDacBlanking;

	m_editDacBlanking.GetWindowText(Buff);
	theApp.m_UtUser.Pr30DacBlankingFromString(m_pPr30,Buff);
	if(nTemp - m_pPr30->nDacBlanking) {
		m_spinDacBlanking.SetPos(m_pPr30->nDacBlanking/10);
//		CopyTimeSlotVariables();
	}
	
}

void CLSAPage::OnSelchangeComboLfPoint() 
{
	m_pPr30->nLinTimeConstant = m_comboLFPoint.GetCurSel();
	theApp.m_UtUser.Pr30LinearTimeConstant(m_pPr30);
//	CopyTimeSlotVariables();
	
	
}

void CLSAPage::OnDacClearallgains() 
{
	theApp.m_UtUser.Pr30ZeroCurrentDacGains(m_pPr30);
//	CopyTimeSlotVariables();
	Invalidate(FALSE);
}

void CLSAPage::OnDacCleareverything() 
{
	theApp.m_UtUser.Pr30ZeroCurrentDacDelaysAndGains(m_pPr30);

	theApp.m_UtUser.Pr30CalculateDacData(m_pPr30);

//	CopyTimeSlotVariables();

	Invalidate(FALSE);
}

void CLSAPage::OnDacDeletept() 
{
	// TODO: Add your command handler code here
	
}

void CLSAPage::UpdateDacTable()
{
	if (GetSafeHwnd() == NULL) return;
	m_listDac.Invalidate(FALSE);
}

BOOL CLSAPage::OnSetActive() 
{
	theApp.m_LastSettings.nDesiredDevice=PHASED_ARRAY;
	theApp.m_UtUser.Pr30ResetMaxTimeSlots(&theApp.m_UtUser.m_TS[0].Pr30, false);
	theApp.m_UtUser.Pr30UmbilicalSelect(1);

	theApp.m_UtUser.m_Global.nTimeSlot = 0;
	theApp.m_UtUser.SelectWriteTimeslot();
	theApp.m_UtUser.m_TS[m_nTS].Pr30.nSyncPulseTimeSlot = 1;
	theApp.m_UtUser.Pr30SyncPulseTimeSlot(&theApp.m_UtUser.m_TS[0].Pr30);

	theApp.m_UtUser.Pr30Initialize(&theApp.m_UtUser.m_TS[0].Pr30);

	UpdateAllControls();
	
	return CPropertyPage::OnSetActive();
}

void CLSAPage::OnChangeEditPrf() 
{
	CString	Buff;
	int	nPrf=theApp.m_UtUser.m_Global.nPrf;

	m_editPRF.GetWindowText(Buff);
	theApp.m_UtUser.m_Global.nPrf=_ttoi(Buff);
	m_spinPRF.SetPos(theApp.m_UtUser.m_Global.nPrf);
	if(nPrf-theApp.m_UtUser.m_Global.nPrf) theApp.m_UtUser.SetPrf();
}
