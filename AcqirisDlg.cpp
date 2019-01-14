// AcqirisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AcqirisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqirisDlg dialog

IMPLEMENT_DYNCREATE(CAcqirisDlg, CPropertyPage)

CAcqirisDlg::CAcqirisDlg() : CPropertyPage(CAcqirisDlg::IDD)

{
	//{{AFX_DATA_INIT(CAcqirisDlg)
	//}}AFX_DATA_INIT
	
}


void CAcqirisDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqirisDlg)
	DDX_Control(pDX, IDC_STATIC_DC_OFFSET, m_staticDCOffset);
	DDX_Control(pDX, IDC_SPIN_MAX_SAMPLING_RATE, m_spinMaxSamplingRate);
	DDX_Control(pDX, IDC_EDIT_MAX_SAMPLING_RATE, m_editMaxSamplingRate);
	DDX_Control(pDX, IDC_COMBO_BITS, m_comboBits);
	DDX_Control(pDX, IDC_EDIT_TYPE, m_editType);
	DDX_Control(pDX, IDC_SPIN_LOG_MIN, m_spinLogMin);
	DDX_Control(pDX, IDC_SPIN_LOG_MAX, m_spinLogMax);
	DDX_Control(pDX, IDC_CHECK_CALIBRATE, m_checkCalibrate);
	DDX_Control(pDX, IDC_SPIN_DAC_MAX_THRESHOLD, m_spinDACMaxThreshold);
	DDX_Control(pDX, IDC_EDIT_DAC_MAX_THRESHOLD, m_editDACMaxThreshold);
	DDX_Control(pDX, IDC_SPIN_DC_OFFSET, m_spinDCOffset);
	DDX_Control(pDX, IDC_EDIT_DC_OFFSET, m_editDCOffset);
	DDX_Control(pDX, IDC_SPIN_LOG_GRATICULE, m_spinLogGraticuleScale);
	DDX_Control(pDX, IDC_EDIT_LOG_GRATICULE, m_editLogGraticuleScale);
	DDX_Control(pDX, IDC_EDIT_MAINBANG_CONSTANT, m_editMainBangConstant);
	DDX_Control(pDX, IDC_EDIT_LOG_MIN, m_editLogMin);
	DDX_Control(pDX, IDC_EDIT_LOG_MAX, m_editLogMax);
	DDX_Control(pDX, IDC_COMBO_COUPLING, m_comboCoupling);
	DDX_Control(pDX, IDC_EDIT_MAX_TIMEBASE, m_editMaxTimeBase);
	DDX_Control(pDX, IDC_SPIN_TRIGGER_THRESHOLD, m_spinTriggerThreshold);
	DDX_Control(pDX, IDC_EDIT_TRIGGER_THRESHOLD, m_editTriggerThreshold);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_SOURCE, m_comboTriggerSource);
	DDX_Control(pDX, IDC_COMBO_FULLSCALE_RANGE, m_comboFullScaleRange);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_SQUELCH_MAX, m_editSquelchMax);
	DDX_Control(pDX, IDC_EDIT_SQUELCH_MIN, m_editSquelchMin);
	DDX_Control(pDX, IDC_EDIT_SQUELCH_ATTENUATION, m_editSquelchAttenuation);
	DDX_Control(pDX, IDC_SPIN_SQUELCH_MAX, m_spinSquelchMax);
	DDX_Control(pDX, IDC_SPIN_SQUELCH_MIN, m_spinSquelchMin);
	DDX_Control(pDX, IDC_SPIN_SQUELCH_ATTENUATION, m_spinSquelchAttenuation);
}


BEGIN_MESSAGE_MAP(CAcqirisDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CAcqirisDlg)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_FULLSCALE_RANGE, OnSelchangeComboFullscaleRange)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_SOURCE, OnSelchangeComboTriggerSource)
	ON_EN_CHANGE(IDC_EDIT_TRIGGER_THRESHOLD, OnChangeEditTriggerThreshold)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_MAX_TIMEBASE, OnChangeEditMaxTimebase)
	ON_CBN_SELCHANGE(IDC_COMBO_COUPLING, OnSelchangeComboCoupling)
	ON_EN_CHANGE(IDC_EDIT_LOG_MIN, OnChangeEditLogMin)
	ON_EN_CHANGE(IDC_EDIT_LOG_MAX, OnChangeEditLogMax)
	ON_EN_CHANGE(IDC_EDIT_MAINBANG_CONSTANT, OnChangeEditMainbangConstant)
	ON_EN_CHANGE(IDC_EDIT_LOG_GRATICULE, OnChangeEditLogGraticule)
	ON_EN_CHANGE(IDC_EDIT_DC_OFFSET, OnChangeEditDcOffset)
	ON_EN_CHANGE(IDC_EDIT_DAC_MAX_THRESHOLD, OnChangeEditDacMaxThreshold)
	ON_BN_CLICKED(IDC_CHECK_CALIBRATE, OnCheckCalibrate)
	ON_CBN_SELCHANGE(IDC_COMBO_BITS, OnSelchangeComboBits)
	ON_EN_CHANGE(IDC_EDIT_MAX_SAMPLING_RATE, OnChangeEditMaxSamplingRate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAX_SAMPLING_RATE, OnDeltaposSpinMaxSamplingRate)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DC_OFFSET, OnDeltaposSpinDcOffset)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_EN_CHANGE(IDC_EDIT_SQUELCH_MAX, &CAcqirisDlg::OnEnChangeEditSquelchMax)
	ON_EN_CHANGE(IDC_EDIT_SQUELCH_MIN, &CAcqirisDlg::OnEnChangeEditSquelchMin)
	ON_EN_CHANGE(IDC_EDIT_SQUELCH_ATTENUATION, &CAcqirisDlg::OnEnChangeEditSquelchAttenuation)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SQUELCH_MAX, &CAcqirisDlg::OnDeltaposSpinSquelchMax)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SQUELCH_MIN, &CAcqirisDlg::OnDeltaposSpinSquelchMin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SQUELCH_ATTENUATION, &CAcqirisDlg::OnDeltaposSpinSquelchAttenuation)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqirisDlg message handlers


BOOL CAcqirisDlg::OnInitDialog() 
{
	BOOL bResult = CPropertyPage::OnInitDialog();

	if (CAcqirisDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CAcqirisDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	switch(theApp.m_LastSettings.nAdcCardType) {
	case USL_PR40:
		m_comboFullScaleRange.AddString(_T("4.0 V P-P"));
		break;
	case SpectrumM3i_2120:
	case SpectrumM3i_2122:
	case SpectrumM3i_2130:
	case SpectrumM3i_2132:
		m_comboFullScaleRange.AddString(_T("20.0 V"));
		m_comboFullScaleRange.AddString(_T("10.0 V"));
		m_comboFullScaleRange.AddString(_T("4.0 V"));
		m_comboFullScaleRange.AddString(_T("2.0 V"));
		m_comboFullScaleRange.AddString(_T("1.0 V"));
		m_comboFullScaleRange.AddString(_T("0.5 V"));
		m_comboFullScaleRange.AddString(_T("0.2 V"));
		m_comboFullScaleRange.AddString(_T("0.1 V"));
		break;
	case Alazar_9626:
		m_comboFullScaleRange.AddString(_T("2.5 V P-P"));
		break;
	default:
		m_comboFullScaleRange.AddString(_T("5.0 V"));
		m_comboFullScaleRange.AddString(_T("4.0 V"));
		m_comboFullScaleRange.AddString(_T("3.0 V"));
		m_comboFullScaleRange.AddString(_T("2.0 V"));
		m_comboFullScaleRange.AddString(_T("1.0 V"));
		m_comboFullScaleRange.AddString(_T("0.5 V"));
		m_comboFullScaleRange.AddString(_T("0.1 V"));
		m_comboFullScaleRange.AddString(_T("0.05 V"));
		break;
	}

	m_comboTriggerSource.AddString(_T("Channel 1"));
	m_comboTriggerSource.AddString(_T("External"));


	m_comboCoupling.AddString(_T("Disallowed"));
	m_comboCoupling.AddString(_T("DC 1 MOhm"));
	m_comboCoupling.AddString(_T("AC 1 MOhm"));
	m_comboCoupling.AddString(_T("DC 50 MOhm"));
	m_comboCoupling.AddString(_T("AC 50 MOhm"));

	m_spinDCOffset.SetRange(0,4096);
	m_spinDCOffset.SetPos(2048);
	m_spinTriggerThreshold.SetRange(0,5000);
	m_spinLogGraticuleScale.SetRange(10,170);
	m_spinDACMaxThreshold.SetRange(0,200); //0 to 200%

	m_spinLogMin.SetRange(0,1000);
	m_spinLogMax.SetRange(0,1000);

	m_spinMaxSamplingRate.SetRange(0,2000);


	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_LastSettings.nAdcCardType) {
		case USL_ADC100: m_editType.SetWindowText(L"Adc100");
			m_comboBits.EnableWindow(false);
			break;
		case USL_ADC200:  m_editType.SetWindowText(L"Adc200");
			m_comboBits.EnableWindow(false);
			break;
		case Acqiris_DP210:  m_editType.SetWindowText(L"Acqiris DP211");
			m_comboBits.EnableWindow(false);
			break;
		case Acqiris_U1071AHZ4:  m_editType.SetWindowText(L"Acqiris U1071AHZ4");
			m_comboBits.EnableWindow(false);
			break;
		case Alazar_860: m_editType.SetWindowText(L"ATS860");
			m_comboBits.EnableWindow(false);
			break;
		case Alazar_9462: m_editType.SetWindowText(L"ATS9462");
			m_comboBits.EnableWindow(true);
			break;
		case Alazar_9626: m_editType.SetWindowText(L"ATS9626");
			m_comboBits.EnableWindow(true);
			break;
		case SpectrumM3i_2122:	m_editType.SetWindowText(theApp.m_Spectrum.getName());
			m_comboBits.EnableWindow(false);
			break;
		case SpectrumM3i_2132:	m_editType.SetWindowText(theApp.m_Spectrum.getName());
			m_comboBits.EnableWindow(false);
			break;
		case USL_PR40:	m_editType.SetWindowText(L"USL PR 40");
			m_comboBits.EnableWindow(true);
			break;
		}
		m_comboBits.AddString(L"8");
		m_comboBits.AddString(L"16");
		break;
	case FB_TWIN_TOWER:
		m_editType.SetWindowText(L"ATS9462");
		m_comboBits.EnableWindow(false);
		m_comboBits.AddString(L"16");
		break;
	}

	m_spinSquelchMax.SetRange(0,100);
	m_spinSquelchMin.SetRange(0, 100);
	m_spinSquelchAttenuation.SetRange(0, 100);

	UpdateAllControls();
	

	SetWindowTitle();

	theApp.m_UtUser.m_TS[0].Up15.nCount=0;

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAcqirisDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CAcqirisDlg::UpdateAllControls()
{
	CString Buff;
	float fTemp;

	m_comboTriggerSource.SetCurSel(theApp.m_UtUser.m_TS[0].Adc.nTriggerSource);

	for(int nIndex = 0;nIndex<m_comboFullScaleRange.GetCount();nIndex++) {
		m_comboFullScaleRange.GetLBText(nIndex,Buff);
		_WTOF(Buff, fTemp);
		if(theApp.m_Scope.m_fVoltageRange == fTemp) m_comboFullScaleRange.SetCurSel(nIndex);
	}


	Buff.Format(_T("%.01f us"),theApp.m_Scope.m_fMaxRange * 1e6f);
	m_editMaxTimeBase.SetWindowText(Buff);

	Buff.Format(_T("%.0f mV"),theApp.m_Tank.fADCTriggerLevel);
	m_editTriggerThreshold.SetWindowText(Buff);
	m_spinTriggerThreshold.SetPos((int)theApp.m_Tank.fADCTriggerLevel);


	Buff.Format(_T("%.01f %%"),theApp.m_Scope.m_fDCOffset);
	m_editDCOffset.SetWindowText(Buff);


	m_comboCoupling.SetCurSel(theApp.m_Scope.m_nCoupling);

	Buff.Format(_T("%.03f us"),theApp.m_Scope.m_fMainBangConstant * 1e6f);
	m_editMainBangConstant.SetWindowText(Buff);

	Buff.Format(L"%.01f%% FSH",(float)theApp.m_Scope.m_nLogMin *100.0f / 2047.0f);
	m_editLogMin.SetWindowText(Buff);
	Buff.Format(L"%.01f%% FSH",(float)theApp.m_Scope.m_nLogMax *100.0f / 2047.0f);
	m_editLogMax.SetWindowText(Buff);

	m_spinLogMin.SetPos(MulDiv(theApp.m_Scope.m_nLogMin,1000,2047));
	m_spinLogMax.SetPos(MulDiv(theApp.m_Scope.m_nLogMax,1000,2047));

	Buff.Format(_T("%d dB"),theApp.m_LastSettings.nLogRange);
	m_editLogGraticuleScale.SetWindowText(Buff);

	Buff.Format(_T("%d dB"),theApp.m_LastSettings.nLogRange);
	m_editLogGraticuleScale.SetWindowText(Buff);
	m_spinLogGraticuleScale.SetPos(theApp.m_LastSettings.nLogRange);

	Buff.Format(_T("%d%%"),theApp.m_LastSettings.nDacMaxThreshold);
	m_editDACMaxThreshold.SetWindowText(Buff);
	m_spinDACMaxThreshold.SetPos(theApp.m_LastSettings.nDacMaxThreshold);

	m_checkCalibrate.SetCheck(theApp.m_Tank.nCalibrateAdc);

	switch(theApp.m_Scope.m_nBits) {
	default: m_comboBits.SetCurSel(0);
		break;
	case 16: m_comboBits.SetCurSel(1);
		break;
	}

	Buff.Format(L"%.01f MSPS",(float)theApp.m_Tank.nMaxADCConversionRate / 1e6);
	m_editMaxSamplingRate.SetWindowText(Buff);

	Buff.Format(L"%.0f%%", theApp.m_LastSettings.fSquelchMax);
	m_editSquelchMax.SetWindowTextW(Buff);
	Buff.Format(L"%.0f%%", theApp.m_LastSettings.fSquelchMin);
	m_editSquelchMin.SetWindowTextW(Buff);
	Buff.Format(L"%.0f dB", theApp.m_LastSettings.fSquelchAttenuation);
	m_editSquelchAttenuation.SetWindowTextW(Buff);


	SetAccessPrivelages();

}

void CAcqirisDlg::OnSelchangeComboFullscaleRange() 
{
	CString Buff;
	
	int nIndex = m_comboFullScaleRange.GetCurSel();
	m_comboFullScaleRange.GetLBText(nIndex,Buff);
	_WTOF(Buff, theApp.m_Scope.m_fVoltageRange);

	UpdateHardware();
}

void CAcqirisDlg::OnSelchangeComboTriggerSource() 
{
	for(int nTS=0;nTS<8;nTS++)
		theApp.m_UtUser.m_TS[nTS].Adc.nTriggerSource = m_comboTriggerSource.GetCurSel();
	
}

void CAcqirisDlg::OnChangeEditTriggerThreshold() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fADCTriggerLevel;

	m_editTriggerThreshold.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fADCTriggerLevel);

	if(fTemp - theApp.m_Tank.fADCTriggerLevel) {
		m_spinTriggerThreshold.SetPos((int)theApp.m_Tank.fADCTriggerLevel);
		UpdateHardware();
	}


}

void CAcqirisDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString Buff;
	int nTemp;
	float fTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_TRIGGER_THRESHOLD:
		fTemp = theApp.m_Tank.fADCTriggerLevel;
		theApp.m_Tank.fADCTriggerLevel = (float)(pSpin->GetPos()&0xffff);
		if(fTemp - theApp.m_Tank.fADCTriggerLevel) {

			Buff.Format(_T("%.0f mV"),theApp.m_Tank.fADCTriggerLevel);
			m_editTriggerThreshold.SetWindowText(Buff);

			UpdateHardware();
		}
		break;
	case IDC_SPIN_LOG_GRATICULE:
		nTemp = theApp.m_LastSettings.nLogRange;
		theApp.m_LastSettings.nLogRange = pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_LastSettings.nLogRange) {
			Buff.Format(_T("%d dB"),theApp.m_LastSettings.nLogRange);
			m_editLogGraticuleScale.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_DAC_TRIG_COEFF:
		nTemp = theApp.m_LastSettings.nDacMaxThreshold;
		theApp.m_LastSettings.nDacMaxThreshold = (pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_LastSettings.nDacMaxThreshold) {
			Buff.Format(_T("%d%%"),theApp.m_LastSettings.nDacMaxThreshold);
			m_editDACMaxThreshold.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_LOG_MIN:
		nTemp = (int)((float)(pSpin->GetPos()&0xffff) * 2.047f);
		if(nTemp - theApp.m_Scope.m_nLogMin) {
			theApp.m_Scope.m_nLogMin = nTemp;
			Buff.Format(L"%.01f%% FSH",(float)theApp.m_Scope.m_nLogMin *100.0f / 2047.0f);
			m_editLogMin.SetWindowText(Buff);
			theApp.m_Alazar.CalculateLogConversionTable();
			theApp.m_Scope.CalculateLogConversionTable();
		}
		break;
	case IDC_SPIN_LOG_MAX:
		nTemp = (int)((float)(pSpin->GetPos()&0xffff) * 2.047f);
		if(nTemp - theApp.m_Scope.m_nLogMax) {
			theApp.m_Scope.m_nLogMax = nTemp;
			Buff.Format(L"%.01f%% FSH",(float)theApp.m_Scope.m_nLogMax *100.0f / 2047.0f);
			m_editLogMax.SetWindowText(Buff);
			theApp.m_Alazar.CalculateLogConversionTable();
			theApp.m_Scope.CalculateLogConversionTable();
		}
		break;

	}
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CAcqirisDlg::OnChangeEditMaxTimebase() 
{
	CString	Buff;

	m_editMaxTimeBase.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Scope.m_fMaxRange);
	theApp.m_Scope.m_fMaxRange /= 1e6;
}


void CAcqirisDlg::OnSelchangeComboCoupling() 
{
	theApp.m_Scope.m_nCoupling = m_comboCoupling.GetCurSel();
	UpdateHardware();
}

void CAcqirisDlg::OnChangeEditLogMin() 
{
	CString Buff;
	float fTemp;
	int nTemp;

	m_editLogMin.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	nTemp = (int)(2047.0f * fTemp / 100.0f);
	if(nTemp - theApp.m_Scope.m_nLogMin) {
		theApp.m_Scope.m_nLogMin = nTemp;

		Buff.Format(L"%.01f%% FSH",(float)theApp.m_Scope.m_nLogMin *100.0f / 2047.0f);
		m_editLogMin.SetWindowText(Buff);

		m_spinLogMin.SetPos(MulDiv(theApp.m_Scope.m_nLogMin,1000,2047));
		theApp.m_Alazar.CalculateLogConversionTable();
		theApp.m_Scope.CalculateLogConversionTable();
	};
	
}

void CAcqirisDlg::OnChangeEditLogMax() 
{
	CString Buff;
	float fTemp;
	int nTemp;

	m_editLogMax.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	nTemp = (int)(2047.0f * fTemp / 100.0f);
	if(nTemp - theApp.m_Scope.m_nLogMax) {
		theApp.m_Scope.m_nLogMax = nTemp;

		Buff.Format(L"%.01f%% FSH",(float)theApp.m_Scope.m_nLogMax *100.0f / 2047.0f);
		m_editLogMax.SetWindowText(Buff);

		m_spinLogMax.SetPos(MulDiv(theApp.m_Scope.m_nLogMax,1000,2047));
		theApp.m_Alazar.CalculateLogConversionTable();
		theApp.m_Scope.CalculateLogConversionTable();

	};
}

void CAcqirisDlg::OnChangeEditMainbangConstant() 
{
	CString Buff;
	float fTemp = theApp.m_Scope.m_fMainBangConstant;

	m_editMainBangConstant.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Scope.m_fMainBangConstant);
	theApp.m_Scope.m_fMainBangConstant /= 1e6;
	if(fTemp - theApp.m_Scope.m_fMainBangConstant) {
		UpdateHardware();
	}
		
}

void CAcqirisDlg::SetWindowTitle()
{
	CString Buff;

	switch(theApp.m_LastSettings.nAdcCardType) {
	case Alazar_860: Buff = "Alazar 860";
		break;
	case Alazar_9462: Buff = "Alazar 9462";
		break;
	case Alazar_9626: Buff = "Alazar 9626";
		break;
	case Acqiris_DP210: Buff = "Acqiris DP210";
		break;
	case Acqiris_U1071AHZ4: Buff = "Acqiris U1071AHZ4 (1GSps)";
		break;
	case Acqiris_U1071AHZ2: Buff = "Acqiris U1071AHZ2 (2GSps)";
		break;
	case USL_PR40: Buff = "USL PR40";
		break;
	}
	SetWindowText(Buff);
}

void CAcqirisDlg::UpdateHardware()
{
	CString Buff;

	switch(theApp.m_LastSettings.nAdcCardType) {
	case Alazar_860: theApp.m_Alazar.Configure(&theApp.m_UtUser.m_TS[0].Adc);
		theApp.m_Alazar.ConfigureWidthDelay(&theApp.m_UtUser.m_TS[0].Adc, 0);
		theApp.m_Alazar.CalculateLogConversionTable();
		break;
	case Alazar_9462: theApp.m_Alazar.Configure(&theApp.m_UtUser.m_TS[0].Adc);
		theApp.m_Alazar.ConfigureWidthDelay(&theApp.m_UtUser.m_TS[0].Adc, 0);
		theApp.m_Alazar.CalculateLogConversionTable();
		break;
	case Alazar_9626: theApp.m_Alazar.Configure(&theApp.m_UtUser.m_TS[0].Adc);
		theApp.m_Alazar.ConfigureWidthDelay(&theApp.m_UtUser.m_TS[0].Adc, 0);
		theApp.m_Alazar.CalculateLogConversionTable();
		break;
	case Acqiris_U1071AHZ4:
	case Acqiris_DP1400:
	case Acqiris_DP210: theApp.m_Acqiris.Configure();
		break;
	case SpectrumM3i_2120:
	case SpectrumM3i_2122:
	case SpectrumM3i_2130:
	case SpectrumM3i_2132:
		theApp.m_Spectrum.m_bConfigureHW = true;
		break;
	case USL_PR40:
		break;
	}

	theApp.m_Scope.CalculateLogConversionTable();

	Buff.Format(L"%.01f",(float)theApp.m_Tank.nMaxADCConversionRate / 1e6);
	m_editMaxSamplingRate.SetWindowText(Buff);
	m_spinMaxSamplingRate.SetPos(theApp.m_Tank.nMaxADCConversionRate / 100000);
			
}

void CAcqirisDlg::OnChangeEditLogGraticule() 
{
	CString Buff;
	int nTemp = theApp.m_LastSettings.nLogRange;

	m_editLogGraticuleScale.GetWindowText(Buff);
	theApp.m_LastSettings.nLogRange = _ttoi(Buff);
	if(nTemp - theApp.m_LastSettings.nLogRange) {
		m_spinLogGraticuleScale.SetPos(theApp.m_LastSettings.nLogRange);
	}
}

void CAcqirisDlg::OnChangeEditDcOffset() 
{
	CString Buff;
	float fTemp = theApp.m_Scope.m_fDCOffset;

	m_editDCOffset.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_Scope.m_fDCOffset);
	if(fTemp - theApp.m_Scope.m_fDCOffset) {
		UpdateHardware();
	}
}

void CAcqirisDlg::OnChangeEditDacMaxThreshold() 
{
	CString Buff;
	float nTemp = (float)theApp.m_LastSettings.nDacMaxThreshold;

	m_editDACMaxThreshold.GetWindowText(Buff);
	theApp.m_LastSettings.nDacMaxThreshold = _ttoi(Buff);
	if(nTemp - theApp.m_LastSettings.nDacMaxThreshold) 
		m_spinDACMaxThreshold.SetPos(theApp.m_LastSettings.nDacMaxThreshold);
	
}

void CAcqirisDlg::OnCheckCalibrate() 
{
	theApp.m_Tank.nCalibrateAdc = m_checkCalibrate.GetCheck();
}
/*
int CAcqirisDlg::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}
*/

BOOL CAcqirisDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
	case IDC_EDIT_LOG_MIN:  strTipText.Format(L"%d",theApp.m_Scope.m_nLogMin);
		break;
	case IDC_EDIT_LOG_MAX: strTipText.Format(L"%d",theApp.m_Scope.m_nLogMax);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

BOOL CAcqirisDlg::OnSetActive() 
{
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CAcqirisDlg::OnKillActive() 
{
	
	return CPropertyPage::OnKillActive();
}

void CAcqirisDlg::OnSelchangeComboBits() 
{
	switch(m_comboBits.GetCurSel()) {
	case 0: theApp.m_Scope.m_nBits = 8;
		break;
	case 1: theApp.m_Scope.m_nBits = 16;
		break;
	}

}

void CAcqirisDlg::OnChangeEditMaxSamplingRate() 
{
	CString Buff;

	m_editMaxSamplingRate.GetWindowText(Buff);
	theApp.m_Tank.nMaxADCConversionRate = _ttoi(Buff) * 1000000;
}

void CAcqirisDlg::OnDeltaposSpinMaxSamplingRate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nTemp = theApp.m_Tank.nMaxADCConversionRate;

	theApp.m_Tank.nMaxADCConversionRate += ((int)pNMUpDown->iDelta * 10000000);
	MinMax(&theApp.m_Tank.nMaxADCConversionRate,10000000,2000000000);

	if(nTemp - theApp.m_Tank.nMaxADCConversionRate) {
		Buff.Format(L"%.0f MSPS",(float)theApp.m_Tank.nMaxADCConversionRate / 1e6);
		m_editMaxSamplingRate.SetWindowText(Buff);
		theApp.m_Acqiris.SetConversionRates();
	}
	*pResult = 0;
}


void CAcqirisDlg::SetAccessPrivelages()
{
	switch(theApp.m_LastSettings.nAdcCardType) {
	case Acqiris_DP210:
	case Acqiris_U1071AHZ4:
		break;
	case Alazar_860:
	case Alazar_9462:
	case Alazar_9626:
		m_staticDCOffset.ShowWindow(false);
		m_editDCOffset.ShowWindow(false);
		m_spinDCOffset.ShowWindow(false);
		break;
	}


}

void CAcqirisDlg::OnDeltaposSpinDcOffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_Scope.m_fDCOffset += ((float)pNMUpDown->iDelta * 0.1f);
	if(theApp.m_Scope.m_fDCOffset < -20.0f) theApp.m_Scope.m_fDCOffset = -20.0f;
	if(theApp.m_Scope.m_fDCOffset > 20.0f) theApp.m_Scope.m_fDCOffset = 20.0f;
	UpdateHardware();
	UpdateAllControls();

	*pResult = 0;
}


void CAcqirisDlg::OnEnChangeEditSquelchMax()
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fSquelchMax;

	m_editSquelchMax.GetWindowTextW(Buff);
	_WTOF(Buff, theApp.m_LastSettings.fSquelchMax);
	if (fTemp - theApp.m_LastSettings.fSquelchMax) {
//		UpdateHardware();
	}
}


void CAcqirisDlg::OnEnChangeEditSquelchMin()
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fSquelchMin;

	m_editSquelchMin.GetWindowTextW(Buff);
	_WTOF(Buff, theApp.m_LastSettings.fSquelchMin);
	if (fTemp - theApp.m_LastSettings.fSquelchMin) {
//		UpdateHardware();
	}
}


void CAcqirisDlg::OnEnChangeEditSquelchAttenuation()
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fSquelchAttenuation;

	m_editSquelchAttenuation.GetWindowTextW(Buff);
	_WTOF(Buff, theApp.m_LastSettings.fSquelchAttenuation);
	if (fTemp - theApp.m_LastSettings.fSquelchAttenuation) {
//		UpdateHardware();
	}
}


void CAcqirisDlg::OnDeltaposSpinSquelchMax(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString Buff;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_LastSettings.fSquelchMax += (float)pNMUpDown->iDelta;
	MinMax(&theApp.m_LastSettings.fSquelchMax, 0.0f, 100.0f);
	Buff.Format(L"%.0f%%", theApp.m_LastSettings.fSquelchMax);
	m_editSquelchMax.SetWindowTextW(Buff);
	theApp.m_Scope.CalculateLogConversionTable();

	*pResult = 0;
}


void CAcqirisDlg::OnDeltaposSpinSquelchMin(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString Buff;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_LastSettings.fSquelchMin += (float)pNMUpDown->iDelta;
	MinMax(&theApp.m_LastSettings.fSquelchMin, 0.0f, 100.0f);
	Buff.Format(L"%.0f%%", theApp.m_LastSettings.fSquelchMin);
	m_editSquelchMin.SetWindowTextW(Buff);
	theApp.m_Scope.CalculateLogConversionTable();

	*pResult = 0;
}


void CAcqirisDlg::OnDeltaposSpinSquelchAttenuation(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString Buff;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_LastSettings.fSquelchAttenuation += (float)pNMUpDown->iDelta;
	MinMax(&theApp.m_LastSettings.fSquelchAttenuation, 0.0f, 100.0f);
	Buff.Format(L"%.0f dB", theApp.m_LastSettings.fSquelchAttenuation);
	m_editSquelchAttenuation.SetWindowTextW(Buff);
	theApp.m_Scope.CalculateLogConversionTable();

	*pResult = 0;
}
