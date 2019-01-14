// ParksMcClellanPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ParksMcClellanPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	MAX_FREQUENCY	20.0f
#define	MAX_ATTENUATION	120.0f

/////////////////////////////////////////////////////////////////////////////
// CParksMcClellanPage property page

IMPLEMENT_DYNCREATE(CParksMcClellanPage, CResizablePage)

CParksMcClellanPage::CParksMcClellanPage() : CResizablePage(CParksMcClellanPage::IDD)
{
	//{{AFX_DATA_INIT(CParksMcClellanPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_fMaxFrequency = 50.0;
}

CParksMcClellanPage::~CParksMcClellanPage()
{
}

void CParksMcClellanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParksMcClellanPage)
	DDX_Control(pDX, IDC_EDIT_NUMBER_TAPS, m_editNumberTaps);
	DDX_Control(pDX, IDC_STATIC_FREQUENCY, m_staticFrequency);
	DDX_Control(pDX, IDC_STATIC_PHASE, m_staticPhase);
	DDX_Control(pDX, IDC_STATIC_4, m_static4);
	DDX_Control(pDX, IDC_SLIDER_4, m_slider4);
	DDX_Control(pDX, IDC_EDIT_4, m_edit4);
	DDX_Control(pDX, IDC_STATIC_3, m_static3);
	DDX_Control(pDX, IDC_STATIC_2, m_static2);
	DDX_Control(pDX, IDC_STATIC_1, m_static1);
	DDX_Control(pDX, IDC_STATIC_0, m_static0);
	DDX_Control(pDX, IDC_SLIDER_3, m_slider3);
	DDX_Control(pDX, IDC_SLIDER_2, m_slider2);
	DDX_Control(pDX, IDC_SLIDER_1, m_slider1);
	DDX_Control(pDX, IDC_SLIDER_0, m_slider0);
	DDX_Control(pDX, IDC_EDIT_3, m_edit3);
	DDX_Control(pDX, IDC_EDIT_2, m_edit2);
	DDX_Control(pDX, IDC_EDIT_1, m_edit1);
	DDX_Control(pDX, IDC_EDIT_0, m_edit0);
	DDX_Control(pDX, IDC_COMBO_FILTER_TYPE, m_comboFilterType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParksMcClellanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CParksMcClellanPage)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, OnSelchangeComboFilterType)
	ON_EN_CHANGE(IDC_EDIT_0, OnChangeEdit0)
	ON_EN_CHANGE(IDC_EDIT_1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT_2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT_3, OnChangeEdit3)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_4, OnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE, OnButtonCompute)
	ON_BN_CLICKED(IDC_BUTTON_ESTIMATE, OnButtonEstimate)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_TAPS, OnChangeEditNumberTaps)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_FILTER, OnButtonClearFilter)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParksMcClellanPage message handlers

BOOL CParksMcClellanPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.LoadString(IDS_Lowpass);	m_comboFilterType.AddString(Buff);
	Buff.LoadString(IDS_Highpass);	m_comboFilterType.AddString(Buff);
	Buff.LoadString(IDS_Bandpass);	m_comboFilterType.AddString(Buff);
	Buff.LoadString(IDS_Bandstop);	m_comboFilterType.AddString(Buff);

	m_slider0.SetRange(0,200);
	m_slider1.SetRange(0,200);
	m_slider2.SetRange(0,200);
	m_slider3.SetRange(0,200);
	m_slider4.SetRange(0,200);

	
//	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CParksMcClellanPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_comboFilterType.SetCurSel(theApp.m_LastSettings.nFilterType);

	switch(theApp.m_LastSettings.nFilterType) {
	case LOWPASS:
		Buff.LoadString(IDS_Passband_Upper_Frequency);	m_static0.SetWindowText(Buff);
		Buff.LoadString(IDS_Stopband_Lower_Frequency);	m_static1.SetWindowText(Buff);
		Buff.LoadString(IDS_Passband_Ripple);			m_static2.SetWindowText(Buff);
		Buff.LoadString(IDS_Stopband_Attenuation);		m_static3.SetWindowText(Buff);
		Buff.LoadString(IDS_Centre_Frequency);			m_static4.SetWindowText(Buff);
		m_slider0.SetPos((int)(theApp.m_LastSettings.fLowCutOffFreq*120.0/MAX_FREQUENCY));
		m_edit4.EnableWindow(FALSE);
		m_slider4.EnableWindow(FALSE);
		m_slider0.SetPos((int)(theApp.m_LastSettings.fLowCutOffFreq*200.0/MAX_FREQUENCY));
		m_slider1.SetPos((int)(theApp.m_LastSettings.fHighCutOffFreq*200.0/MAX_FREQUENCY));
		Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fLowCutOffFreq);
		m_edit0.SetWindowText(Buff);
		Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fHighCutOffFreq);
		m_edit1.SetWindowText(Buff);
		break;
	case HIGHPASS:
		Buff.LoadString(IDS_Stopband_Upper_Frequency);	m_static0.SetWindowText(Buff);
		Buff.LoadString(IDS_Passband_Lower_Frequency);	m_static1.SetWindowText(Buff);
		Buff.LoadString(IDS_Passband_Ripple);			m_static2.SetWindowText(Buff);
		Buff.LoadString(IDS_Stopband_Attenuation);		m_static3.SetWindowText(Buff);
		Buff.LoadString(IDS_Centre_Frequency);			m_static4.SetWindowText(Buff);
		m_edit4.EnableWindow(FALSE);
		m_slider4.EnableWindow(FALSE);
		m_slider0.SetPos((int)(theApp.m_LastSettings.fLowCutOffFreq*200.0/MAX_FREQUENCY));
		m_slider1.SetPos((int)(theApp.m_LastSettings.fHighCutOffFreq*200.0/MAX_FREQUENCY));
		Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fLowCutOffFreq);
		m_edit0.SetWindowText(Buff);
		Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fHighCutOffFreq);
		m_edit1.SetWindowText(Buff);
		break;
	case BANDPASS:
		Buff.LoadString(IDS_Passband_Bandwidth_at_Top);		m_static0.SetWindowText(Buff);
		Buff.LoadString(IDS_Passband_Bandwidth_at_Bottom);	m_static1.SetWindowText(Buff);
		Buff.LoadString(IDS_Passband_Ripple);				m_static2.SetWindowText(Buff);
		Buff.LoadString(IDS_Stopband_Attenuation);			m_static3.SetWindowText(Buff);
		Buff.LoadString(IDS_Centre_Frequency);				m_static4.SetWindowText(Buff);
		m_edit4.EnableWindow(TRUE);
		m_slider4.EnableWindow(TRUE);
		m_slider0.SetPos((int)(theApp.m_LastSettings.fTopBandWidth*200.0/MAX_FREQUENCY));
		m_slider1.SetPos((int)(theApp.m_LastSettings.fBottomBandWidth*200.0/MAX_FREQUENCY));
		Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fTopBandWidth);
		m_edit0.SetWindowText(Buff);
		Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fBottomBandWidth);
		m_edit1.SetWindowText(Buff);
		break;
	case BANDSTOP:
		Buff.LoadString(IDS_Stopband_Bandwidth_at_Top);		m_static0.SetWindowText(Buff);
		Buff.LoadString(IDS_Stopband_Bandwidth_at_Bottom);	m_static1.SetWindowText(Buff);
		Buff.LoadString(IDS_Passband_Ripple);				m_static2.SetWindowText(Buff);
		Buff.LoadString(IDS_Stopband_Attenuation);			m_static3.SetWindowText(Buff);
		Buff.LoadString(IDS_Centre_Frequency);				m_static4.SetWindowText(Buff);
		m_edit4.EnableWindow(TRUE);
		m_slider4.EnableWindow(TRUE);
		m_slider0.SetPos((int)(theApp.m_LastSettings.fTopBandWidth*200.0/MAX_FREQUENCY));
		m_slider1.SetPos((int)(theApp.m_LastSettings.fBottomBandWidth*200.0/MAX_FREQUENCY));
		Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fTopBandWidth);
		m_edit0.SetWindowText(Buff);
		Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fBottomBandWidth);
		m_edit1.SetWindowText(Buff);
		break;
	}


	m_slider2.SetPos((int)(theApp.m_LastSettings.fPassRipple*200.0/MAX_ATTENUATION));
	m_slider3.SetPos((int)(theApp.m_LastSettings.fStopAttenuation*200.0/MAX_ATTENUATION));
	m_slider4.SetPos((int)(theApp.m_LastSettings.fCentreFreq*200.0/MAX_FREQUENCY));

	Buff.Format(_T("%.01f dB"),theApp.m_LastSettings.fPassRipple);
	m_edit2.SetWindowText(Buff);
	Buff.Format(_T("%.01f dB"),theApp.m_LastSettings.fStopAttenuation);
	m_edit3.SetWindowText(Buff);
	Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fCentreFreq);
	m_edit4.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nNumberTaps);
	m_editNumberTaps.SetWindowText(Buff);


}

void CParksMcClellanPage::OnSelchangeComboFilterType() 
{
	theApp.m_LastSettings.nFilterType = (FrequencyFilterType)m_comboFilterType.GetCurSel();
	UpdateAllControls();
}


void CParksMcClellanPage::OnChangeEdit0() 
{
	CString Buff;
	float fTemp;

	m_edit0.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	MinMax(0.0f,MAX_FREQUENCY,fTemp);

	switch(theApp.m_LastSettings.nFilterType) {
	case LOWPASS:
	case HIGHPASS:
		if(theApp.m_LastSettings.fLowCutOffFreq - fTemp) {
			theApp.m_LastSettings.fLowCutOffFreq = fTemp;
			m_slider0.SetPos((int)(theApp.m_LastSettings.fLowCutOffFreq*200.0/MAX_FREQUENCY));
		}
		break;
	case BANDPASS:
	case BANDSTOP:
		if(theApp.m_LastSettings.fTopBandWidth - fTemp) {
			theApp.m_LastSettings.fTopBandWidth = fTemp;
			m_slider0.SetPos((int)(theApp.m_LastSettings.fTopBandWidth*200.0/MAX_FREQUENCY));
		}
		break;
	}
}

void CParksMcClellanPage::OnChangeEdit1() 
{
	CString Buff;
	float fTemp;

	m_edit1.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	MinMax(0.0f,MAX_FREQUENCY,fTemp);

	switch(theApp.m_LastSettings.nFilterType) {
	case LOWPASS:
	case HIGHPASS:
		if(theApp.m_LastSettings.fHighCutOffFreq - fTemp) {
			theApp.m_LastSettings.fHighCutOffFreq = fTemp;
			m_slider1.SetPos((int)(theApp.m_LastSettings.fHighCutOffFreq*200.0/MAX_FREQUENCY));
		}
		break;
	case BANDPASS:
	case BANDSTOP:
		if(theApp.m_LastSettings.fBottomBandWidth - fTemp) {
			theApp.m_LastSettings.fBottomBandWidth = fTemp;
			m_slider1.SetPos((int)(theApp.m_LastSettings.fBottomBandWidth*200.0/MAX_FREQUENCY));
		}
		break;
	}
}

void CParksMcClellanPage::OnChangeEdit2() 
{
	CString Buff;
	float fTemp;

	m_edit2.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	MinMax(0.0f,MAX_ATTENUATION,fTemp);

	if(theApp.m_LastSettings.fPassRipple - fTemp) {
		theApp.m_LastSettings.fPassRipple = fTemp;
		m_slider2.SetPos((int)(theApp.m_LastSettings.fPassRipple*200.0/MAX_ATTENUATION));
	}
}

void CParksMcClellanPage::OnChangeEdit3() 
{
	CString Buff;
	float fTemp;

	m_edit3.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	MinMax(0.0f,MAX_ATTENUATION,fTemp);

	if(theApp.m_LastSettings.fStopAttenuation - fTemp) {
		theApp.m_LastSettings.fStopAttenuation = fTemp;
		m_slider3.SetPos((int)(theApp.m_LastSettings.fStopAttenuation*200.0/MAX_ATTENUATION));
	}
}

void CParksMcClellanPage::OnChangeEdit4() 
{
	CString Buff;
	float fTemp;

	m_edit4.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	MinMax(0.0f,MAX_FREQUENCY,fTemp);

	if(theApp.m_LastSettings.fCentreFreq - fTemp) {
		theApp.m_LastSettings.fCentreFreq = fTemp;
		m_slider4.SetPos((int)(theApp.m_LastSettings.fCentreFreq*200.0/MAX_FREQUENCY));
	}
}

void CParksMcClellanPage::OnChangeEditNumberTaps() 
{
	CString Buff;

	m_editNumberTaps.GetWindowText(Buff);
	theApp.m_LastSettings.nNumberTaps = _ttoi(Buff);
}

void CParksMcClellanPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;
	float fTemp;
	CString Buff;

	nTemp=pSlider->GetPos()&0xffff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SLIDER_0:
		fTemp = (float)nTemp * MAX_FREQUENCY / 200.0f;
		switch(theApp.m_LastSettings.nFilterType) {
		case LOWPASS:
		case HIGHPASS:
			if(theApp.m_LastSettings.fLowCutOffFreq - fTemp) {
				theApp.m_LastSettings.fLowCutOffFreq = fTemp;
				Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fLowCutOffFreq);
				m_edit0.SetWindowText(Buff);
				CalculateFilter();
			}
			break;
		case BANDPASS:
		case BANDSTOP:
			if(theApp.m_LastSettings.fTopBandWidth - fTemp) {
				theApp.m_LastSettings.fTopBandWidth = fTemp;
				Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fTopBandWidth);
				m_edit0.SetWindowText(Buff);
				CalculateFilter();
			}
			break;
		}
		break;

	case IDC_SLIDER_1:
		fTemp = (float)nTemp * MAX_FREQUENCY / 200.0f;
		switch(theApp.m_LastSettings.nFilterType) {
		case LOWPASS:
		case HIGHPASS:
			if(theApp.m_LastSettings.fHighCutOffFreq - fTemp) {
				theApp.m_LastSettings.fHighCutOffFreq = fTemp;
				Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fHighCutOffFreq);
				m_edit1.SetWindowText(Buff);
				CalculateFilter();
			}
			break;
		case BANDPASS:
		case BANDSTOP:
			if(theApp.m_LastSettings.fBottomBandWidth - fTemp) {
				theApp.m_LastSettings.fBottomBandWidth = fTemp;
				Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fBottomBandWidth);
				m_edit1.SetWindowText(Buff);
				CalculateFilter();
			}
			break;
		}
		break;

	case IDC_SLIDER_2:
		fTemp = (float)nTemp * MAX_ATTENUATION / 200.0f;
		if(theApp.m_LastSettings.fPassRipple - fTemp) {
			theApp.m_LastSettings.fPassRipple = fTemp;
			Buff.Format(_T("%.01f dB"),theApp.m_LastSettings.fPassRipple);
			m_edit2.SetWindowText(Buff);
			CalculateFilter();
		}
		break;

	case IDC_SLIDER_3:
		fTemp = (float)nTemp * MAX_ATTENUATION / 200.0f;
		if(theApp.m_LastSettings.fStopAttenuation - fTemp) {
			theApp.m_LastSettings.fStopAttenuation = fTemp;
			Buff.Format(_T("%.01f dB"),theApp.m_LastSettings.fStopAttenuation);
			m_edit3.SetWindowText(Buff);
			CalculateFilter();
		}
		break;

	case IDC_SLIDER_4:
		fTemp = (float)nTemp * MAX_FREQUENCY / 200.0f;
		if(theApp.m_LastSettings.fCentreFreq - fTemp) {
			theApp.m_LastSettings.fCentreFreq = fTemp;
			Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fCentreFreq);
			m_edit4.SetWindowText(Buff);
			CalculateFilter();
		}
		break;
	}

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}


float CParksMcClellanPage::MinMax(float fMin, float fMax, float &fValue)
{

	if(fValue<fMin) fValue = fMin;
	if(fValue>fMax) fValue = fMax;

	return fValue;
}

bool CParksMcClellanPage::CalculateFilter()
{
	float f1,f2,fc;
	int numBands,i;

	switch(theApp.m_LastSettings.nFilterType) {
	case LOWPASS:
	case HIGHPASS:	numBands = 2;
		break;
	case BANDPASS:
	case BANDSTOP:	numBands = 3;
		break;
	}
	double* desired = new double[numBands];
    double* bands   = new double[2*numBands];
    double* weights = new double[numBands];

	float deltaP = 0.5f*(1.0f - (float)pow(10.0f, -0.05f*theApp.m_LastSettings.fPassRipple));
	float deltaS = (float)pow(10.0f, -0.05f*theApp.m_LastSettings.fStopAttenuation);
    float rippleRatio = deltaP / deltaS;



	switch(theApp.m_LastSettings.nFilterType) {
	case LOWPASS:
		f1 = 0.5f * theApp.m_LastSettings.fLowCutOffFreq / m_fMaxFrequency;
		f2 = 0.5f * theApp.m_LastSettings.fHighCutOffFreq / m_fMaxFrequency;
		desired[0] = 1.0;
		desired[1] = 0.0;
		bands[0] = 0.0;
		bands[1] = f1;
		bands[2] = f2;
		bands[3] = 0.5;
		weights[0] = 1.0;
		weights[1] = rippleRatio;
		break;
	case HIGHPASS:
		f1 = 0.5f * theApp.m_LastSettings.fLowCutOffFreq / m_fMaxFrequency;
		f2 = 0.5f * theApp.m_LastSettings.fHighCutOffFreq / m_fMaxFrequency;
		desired[0] = 0.0;
		desired[1] = 1.0;
		bands[0] = 0.0;
		bands[1] = f1;
		bands[2] = f2;
		bands[3] = 0.5;
		weights[0] = rippleRatio;
		weights[1] = 1.0;
		break;
	case BANDPASS:
		fc = 0.5f * theApp.m_LastSettings.fCentreFreq / m_fMaxFrequency;
		f1 = (0.5f * theApp.m_LastSettings.fTopBandWidth / m_fMaxFrequency) / 2.0f;
		f2 = (0.5f * theApp.m_LastSettings.fBottomBandWidth / m_fMaxFrequency) / 2.0f;
		if(f1>=f2) goto finished;
		desired[0] = 0.0;
		desired[1] = 1.0;
		desired[2] = 0.0;
		bands[0] = 0.0;
		bands[1] = fc - f2;
		bands[2] = fc - f1;
		bands[3] = fc + f1;
		bands[4] = fc + f2;
		bands[5] = 0.5;
		weights[0] = rippleRatio;
		weights[1] = 1.0;
		weights[2] = rippleRatio;
		break;
	case BANDSTOP:
		fc = 0.5f * theApp.m_LastSettings.fCentreFreq / m_fMaxFrequency;
		f1 = (0.5f * theApp.m_LastSettings.fTopBandWidth / m_fMaxFrequency) / 2.0f;
		f2 = (0.5f * theApp.m_LastSettings.fBottomBandWidth / m_fMaxFrequency) / 2.0f;
		if(f2>=f1) goto finished;
		desired[0] = 1.0;
		desired[1] = 0.0;
		desired[2] = 1.0;
		bands[0] = 0.0;
		bands[1] = fc - f1;
		bands[2] = fc - f2;
		bands[3] = fc + f2;
		bands[4] = fc + f1;
		bands[5] = 0.5;
		weights[0] = 1;
		weights[1] = rippleRatio;
		weights[2] = 1;
		break;
	}

	for(i=0;i<(numBands+1);i++) {
		if(bands[i]<0.0f) goto finished;
		if(bands[i+1]<=bands[i]) goto finished;
		if(bands[i]>0.5) goto finished;
	}

	theApp.Fir.nNumTaps = theApp.m_LastSettings.nNumberTaps;
	theApp.Fir.remez( numBands, bands, desired, weights, FIR_BANDPASS);	//FIR_BANDPASS FIR_DIFFERENTIATOR FIR_POSITIVE

	Invalidate(FALSE);

finished:
	delete desired;
    delete bands;
    delete weights;

	return TRUE;
}

void CParksMcClellanPage::OnButtonCompute() 
{
	CalculateFilter();
}

void CParksMcClellanPage::RenderPhase(CPaintDC* pDC,int numTaps, double a[])
{
//	CDC* pDC = GetDC();
	CRect rr;
	CBrush Brush(RGB(255,255,255));
	CPen* pOldPen;
	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penGrey(PS_SOLID,1,RGB(50,50,50));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen penDotGrey(PS_DOT,1,RGB(180,180,180));
	CSize size;
	CString Buff;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));
	double dY,dX;
	int nPnY,nPnX;

	LOGFONT lf;
	ZeroMemory(&lf,sizeof lf);
	CFont FontHorizontal;
	CFont FontVertical;
	CFont* pOldFont;
	theApp.SetFontLanguage(&lf);
	lf.lfHeight = 12;
	FontHorizontal.CreateFontIndirect(&lf);
	lf.lfEscapement = 900;
	FontVertical.CreateFontIndirect(&lf);


	Buff = "HelloAll";
	size = pDC->GetTextExtent(Buff);

	pOldPen = pDC->SelectObject(&penRed);
	pOldFont = pDC->SelectObject(&FontHorizontal);
	pDC->SetBkMode(TRANSPARENT);



	m_staticPhase.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.top=rr.bottom;
	rr.bottom=rr.top+3 + size.cy;
	rr.left-=size.cx;
	rr.right+=size.cx;
	pDC->FillRect(rr,&brushBK);

	m_staticPhase.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2);
	pDC->FillRect(&rr,&Brush);

	//Find Min and Max
	double dMax = -1.0;
	double dMin = 1.0;
	double dStep = 0.01;
	int nPrecision = 1;
	int ii;

	if(a != NULL) {
		for(ii=0;ii<numTaps;ii++) {
			if(a[ii]>dMax) dMax = a[ii];
			if(a[ii]<dMin) dMin = a[ii];
		}


		if((dMax - dMin) < 0.3) {
			dMax = (double)((int)(dMax*100.0) + 1) / 100.0;
			dMin = (double)((int)(dMin*100.0) - 1) / 100.0;
			dStep = 0.01;
			nPrecision = 2;
		} else {
			dMax = (double)((int)(dMax*10.0) + 1) / 10.0;
			dMin = (double)((int)(dMin*10.0) - 1) / 10.0;
			dStep = 0.1;
			nPrecision = 1;
		}
		if(dMax<=dMin) dMax = dMin + 0.1;

		//Draw graticule & text
		rr.DeflateRect(size.cx,2 * size.cy);
		rr.right += (size.cx*2/3);

		pDC->SelectObject(&penDotGrey);
		for(dY = dMin;dY<=(dMax+0.001);dY+=dStep) {
			nPnY = rr.bottom - (int)((dY-dMin) * (double)rr.Height() / (dMax - dMin));
			pDC->MoveTo(rr.left+1,nPnY);
			pDC->LineTo(rr.right+1,nPnY);
			Buff.Format(_T("%.*f"),nPrecision,dY);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(rr.left - size.cx-5,nPnY-size.cy/2,Buff);
		}

		double dMaxTime = 1.0 / (m_fMaxFrequency * 2.0) * (double)theApp.Fir.nNumTaps;
		dStep = 0.2;
		if(dMaxTime <= 1.0) dStep = 0.1;
		for(dX = 0.0;dX <= dMaxTime; dX += dStep) {
			nPnX = (int)(dX * (double)rr.Width() / dMaxTime) + rr.left;
			pDC->MoveTo(nPnX,rr.bottom-1);
			pDC->LineTo(nPnX,rr.top);
			Buff.Format(_T("%.01f"),dX);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(nPnX-size.cx/2,rr.bottom+2,Buff);
		}

		pDC->SelectObject(&penBlack);
		pDC->MoveTo(rr.TopLeft());
		pDC->LineTo(rr.left,rr.bottom);
		pDC->LineTo(rr.BottomRight());

		Buff.LoadString(IDS_Impulse_Responce);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.top-(size.cy*3/2),Buff);

		Buff.LoadString(IDS_Time_in_us);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.bottom+(size.cy*3/2),Buff);

		pDC->SelectObject(&FontVertical);
		Buff.LoadString(IDS_Inphase_Amplitude);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.left-size.cy*3-4,rr.CenterPoint().y+(size.cx/2),Buff);



		//Draw Trace
		rr.DeflateRect(1,1);
		pDC->SelectObject(&penRed);
		ii=0;
		nPnY = rr.bottom - (int)((a[ii++]-dMin) * (double)rr.Height() / (dMax-dMin));
		pDC->MoveTo(rr.left,nPnY);
		for(ii;ii<numTaps;ii++) {
			nPnX = MulDiv(ii,rr.Width()-1,numTaps-1) + rr.left;
			nPnY = rr.bottom - (int)((a[ii]-dMin) * (double)rr.Height() / (dMax-dMin));
			pDC->LineTo(nPnX,nPnY);
		}

	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

//	ReleaseDC(pDC);
}


void CParksMcClellanPage::RenderFrequency(CPaintDC* pDC,int freqPoints, double f[])
{
//	CDC* pDC = GetDC();
	CRect rr;
	CBrush Brush(RGB(255,255,255));
	CPen* pOldPen;
	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penGrey(PS_SOLID,1,RGB(50,50,50));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen penDotGrey(PS_DOT,1,RGB(180,180,180));
	CSize size;
	CString Buff;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));
	double dY,dX;
	int nPnY,nPnX,ii;

	LOGFONT lf;
	ZeroMemory(&lf,sizeof lf);
	CFont FontHorizontal;
	CFont FontVertical;
	CFont* pOldFont;
	theApp.SetFontLanguage(&lf);
	lf.lfHeight = 12;
	FontHorizontal.CreateFontIndirect(&lf);
	lf.lfEscapement = 900;
	FontVertical.CreateFontIndirect(&lf);


	Buff = "Hello";
	size = pDC->GetTextExtent(Buff);

	pOldPen = pDC->SelectObject(&penRed);
	pOldFont = pDC->SelectObject(&FontHorizontal);
	pDC->SetBkMode(TRANSPARENT);



	m_staticFrequency.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.top=rr.bottom;
	rr.bottom=rr.top+3 + size.cy;
	rr.left-=size.cx;
	rr.right+=size.cx;
	pDC->FillRect(rr,&brushBK);

	m_staticFrequency.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2);
	pDC->FillRect(&rr,&Brush);

	//Find Min and Max
	double dMax = -120.0;
	double dMin = 120.0;

	for(ii=0;ii<freqPoints;ii++) {
		if(f[ii]>dMax) dMax = f[ii];
		if(f[ii]<dMin) dMin = f[ii];
	}

	dMin = -120.0;
	dMax = (double)((int)dMax/10 + 1) * 10.0;
	dMin = (double)((int)dMin/10 - 1) * 10.0;

	//Draw graticule & text
	rr.DeflateRect(size.cx,2 * size.cy);
	rr.right += (size.cx/2);

	pDC->SelectObject(&penDotGrey);
	for(dY = dMin;dY<=dMax;dY+=10.0) {
		nPnY = rr.bottom - (int)((dY-dMin) * (double)rr.Height() / (dMax - dMin));
		pDC->MoveTo(rr.left+1,nPnY);
		pDC->LineTo(rr.right+1,nPnY);
		Buff.Format(_T("%.0f"),dY);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.left - size.cx-5,nPnY-size.cy/2,Buff);
	}

	for(dX = 0.0;dX <= m_fMaxFrequency; dX += (m_fMaxFrequency/10)) {
		nPnX = (int)(dX * (double)rr.Width() / m_fMaxFrequency) + rr.left;
		pDC->MoveTo(nPnX,rr.bottom-1);
		pDC->LineTo(nPnX,rr.top);
		Buff.Format(_T("%.0f"),dX);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(nPnX-size.cx/2,rr.bottom+2,Buff);
	}

	pDC->SelectObject(&penBlack);
	pDC->MoveTo(rr.TopLeft());
	pDC->LineTo(rr.left,rr.bottom);
	pDC->LineTo(rr.BottomRight());

	Buff.LoadString(IDS_Inphase_Filter_Frequency_Responce);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.top-(size.cy*3/2),Buff);

	Buff.LoadString(IDS_Frequency_In_MHz);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.bottom+(size.cy*3/2),Buff);

	pDC->SelectObject(&FontVertical);
	Buff.LoadString(IDS_Magnitude_In_dB);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.left-size.cy*2-5,rr.CenterPoint().y+(size.cx/2),Buff);

	//Draw Trace
	rr.DeflateRect(1,1);
	pDC->SelectObject(&penRed);
	ii=0;
	nPnY = rr.bottom - (int)((f[ii++]-dMin) * (double)rr.Height() / (dMax-dMin));
	pDC->MoveTo(rr.left,nPnY);
	for(ii;ii<freqPoints;ii++) {
		nPnX = MulDiv(ii,rr.Width()-1,freqPoints-1) + rr.left;
		nPnY = rr.bottom - (int)((f[ii]-dMin) * (double)rr.Height() / (dMax-dMin));
		pDC->LineTo(nPnX,nPnY);
	}




	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

//	ReleaseDC(pDC);
}

void CParksMcClellanPage::OnButtonEstimate() 
{
	float deltaP = 0.5f*(1.0f - (float)pow(10.0f, -0.05f*theApp.m_LastSettings.fPassRipple));
	float deltaS = (float)pow(10.0f, -0.05f*theApp.m_LastSettings.fStopAttenuation);
	float f1,f2,fTrans;

	switch(theApp.m_LastSettings.nFilterType) {
	case LOWPASS:
	case HIGHPASS:
		f1 = 0.5f * theApp.m_LastSettings.fLowCutOffFreq / m_fMaxFrequency;
		f2 = 0.5f * theApp.m_LastSettings.fHighCutOffFreq / m_fMaxFrequency;
		break;
	case BANDPASS:
	case BANDSTOP:
		f1 = (0.5f * theApp.m_LastSettings.fTopBandWidth / m_fMaxFrequency) / 2.0f;
		f2 = (0.5f * theApp.m_LastSettings.fBottomBandWidth / m_fMaxFrequency) / 2.0f;
		break;
	}
	fTrans = (float)fabs(f2-f1);
	theApp.m_LastSettings.nNumberTaps = (int)((-10.0f*log10(deltaP*deltaS) - 13)/(14.6f*fTrans));

	switch(theApp.m_LastSettings.nFilterType) {
	case LOWPASS:
	case HIGHPASS:
	case BANDPASS:
		if(theApp.m_LastSettings.nNumberTaps % 2) {
			theApp.m_LastSettings.nNumberTaps += 1;
		}
		break;
	case BANDSTOP:
		if(!(theApp.m_LastSettings.nNumberTaps % 2)) {
			theApp.m_LastSettings.nNumberTaps += 1;
		}
		break;
	}
	UpdateAllControls();
	
}


void CParksMcClellanPage::OnButtonClearFilter() 
{

	theApp.Fir.nNumTaps = 0;
}

BOOL CParksMcClellanPage::OnSetActive() 
{
	
	UpdateAllControls();
	theApp.m_LastSettings.nLastFilterToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CParksMcClellanPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	int freqPoints = 200;
	double* f = new double[freqPoints+1];


	RenderPhase(&dc,theApp.Fir.nNumTaps,theApp.Fir.m_dAmpFilter);
	theApp.Fir.FilterGain(theApp.Fir.nNumTaps,theApp.Fir.m_dAmpFilter,freqPoints,f);
	RenderFrequency(&dc,freqPoints,f);

	delete f;

}
