// WindowedSincPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "WindowedSincPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	MAX_FREQUENCY	20.0f

/////////////////////////////////////////////////////////////////////////////
// CWindowedSincPage property page

IMPLEMENT_DYNCREATE(CWindowedSincPage, CResizablePage)

CWindowedSincPage::CWindowedSincPage() : CResizablePage(CWindowedSincPage::IDD)
{
	//{{AFX_DATA_INIT(CWindowedSincPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_fMaxFrequency = 100.0f;
}

CWindowedSincPage::~CWindowedSincPage()
{
}

void CWindowedSincPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindowedSincPage)
	DDX_Control(pDX, IDC_STATIC_PHASE, m_staticPhase);
	DDX_Control(pDX, IDC_STATIC_FREQUENCY, m_staticFrequency);
	DDX_Control(pDX, IDC_STATIC_2, m_static2);
	DDX_Control(pDX, IDC_STATIC_1, m_static1);
	DDX_Control(pDX, IDC_STATIC_0, m_static0);
	DDX_Control(pDX, IDC_SLIDER_2, m_slider2);
	DDX_Control(pDX, IDC_SLIDER_1, m_slider1);
	DDX_Control(pDX, IDC_SLIDER_0, m_slider0);
	DDX_Control(pDX, IDC_EDIT_2, m_edit2);
	DDX_Control(pDX, IDC_EDIT_1, m_edit1);
	DDX_Control(pDX, IDC_EDIT_0, m_edit0);
	DDX_Control(pDX, IDC_COMBO_WINDOW, m_comboWindowType);
	DDX_Control(pDX, IDC_COMBO_FILTER_TYPE, m_comboFilterType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindowedSincPage, CResizablePage)
	//{{AFX_MSG_MAP(CWindowedSincPage)
	ON_EN_CHANGE(IDC_EDIT_0, OnChangeEdit0)
	ON_EN_CHANGE(IDC_EDIT_1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT_2, OnChangeEdit2)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, OnSelchangeComboFilterType)
	ON_CBN_SELCHANGE(IDC_COMBO_WINDOW, OnSelchangeComboWindow)
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE, OnButtonCompute)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowedSincPage message handlers

BOOL CWindowedSincPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	CString Buff;

	Buff.LoadString(IDS_Lowpass);	m_comboFilterType.AddString(Buff);
	Buff.LoadString(IDS_Highpass);	m_comboFilterType.AddString(Buff);
	Buff.LoadString(IDS_Bandpass);	m_comboFilterType.AddString(Buff);

	m_comboWindowType.AddString(_T("Rectangular"));
	m_comboWindowType.AddString(_T("Hanning"));
	m_comboWindowType.AddString(_T("Hamming"));
	m_comboWindowType.AddString(_T("Blackman"));
	m_comboWindowType.AddString(_T("Welch"));

	m_slider0.SetRange(0,200);
	m_slider1.SetRange(0,200);
	m_slider2.SetRange(0,200);
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindowedSincPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;

	m_comboFilterType.SetCurSel(theApp.m_LastSettings.nFilterType);
	m_comboWindowType.SetCurSel(theApp.m_LastSettings.nFilterWindowType);

	Buff.LoadString(IDS_Lower_Cutoff_Frequency);	m_static0.SetWindowText(Buff);
	Buff.LoadString(IDS_Upper_Cutoff_Frequency);	m_static1.SetWindowText(Buff);
	Buff.LoadString(IDS_Number_of_Taps);			m_static2.SetWindowText(Buff);
	m_slider0.SetPos((int)(theApp.m_LastSettings.fLowCutOffFreq*200.0/MAX_FREQUENCY));
	m_slider1.SetPos((int)(theApp.m_LastSettings.fHighCutOffFreq*200.0/MAX_FREQUENCY));
	Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fLowCutOffFreq);
	m_edit0.SetWindowText(Buff);
	Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fHighCutOffFreq);
	m_edit1.SetWindowText(Buff);


	m_slider2.SetPos((int)(theApp.m_LastSettings.nNumberTaps));
	Buff.Format(_T("%d"),theApp.m_LastSettings.nNumberTaps);
	m_edit2.SetWindowText(Buff);
}

void CWindowedSincPage::OnChangeEdit0() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CResizablePage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CWindowedSincPage::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CResizablePage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CWindowedSincPage::OnChangeEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CResizablePage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CWindowedSincPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
		if(theApp.m_LastSettings.fLowCutOffFreq - fTemp) {
			theApp.m_LastSettings.fLowCutOffFreq = fTemp;
			Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fLowCutOffFreq);
			m_edit0.SetWindowText(Buff);
			CalculateFilter();
		}
		break;

	case IDC_SLIDER_1:
		fTemp = (float)nTemp * MAX_FREQUENCY / 200.0f;
		if(theApp.m_LastSettings.fHighCutOffFreq - fTemp) {
			theApp.m_LastSettings.fHighCutOffFreq = fTemp;
			Buff.Format(_T("%.01f MHz"),theApp.m_LastSettings.fHighCutOffFreq);
			m_edit1.SetWindowText(Buff);
			CalculateFilter();
		}
		break;

	case IDC_SLIDER_2:
		if(theApp.m_LastSettings.nNumberTaps - nTemp) {
			theApp.m_LastSettings.nNumberTaps = nTemp;
			Buff.Format(_T("%d"),theApp.m_LastSettings.nNumberTaps);
			m_edit2.SetWindowText(Buff);
			CalculateFilter();
		}
		break;
	}
	
	CResizablePage::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CWindowedSincPage::OnSelchangeComboFilterType() 
{
	
	
	theApp.m_LastSettings.nFilterType = (FrequencyFilterType)m_comboFilterType.GetCurSel();
	UpdateAllControls();
	CalculateFilter();
}

void CWindowedSincPage::OnSelchangeComboWindow() 
{
	
	
	theApp.m_LastSettings.nFilterWindowType = (WindowedType)m_comboWindowType.GetCurSel();
	UpdateAllControls();
	CalculateFilter();
}

BOOL CWindowedSincPage::OnSetActive() 
{
	
	
	UpdateAllControls();
	theApp.m_LastSettings.nLastFilterToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CResizablePage::OnSetActive();
}

void CWindowedSincPage::CalculateFilter()
{
	


	theApp.Fir.nNumTaps = theApp.m_LastSettings.nNumberTaps;
	theApp.Fir.WindowedSinc(theApp.m_LastSettings.nFilterType, theApp.m_LastSettings.nFilterWindowType,theApp.m_LastSettings.fLowCutOffFreq,theApp.m_LastSettings.fHighCutOffFreq,m_fMaxFrequency);


	Invalidate(FALSE);
}

void CWindowedSincPage::OnButtonCompute() 
{
	CalculateFilter();	
}

void CWindowedSincPage::RenderPhase(CPaintDC* pDC,int numTaps, double a[])
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

	if(a !=NULL) {
		for(int ii=0;ii<numTaps;ii++) {
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
		int ii=0;
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


void CWindowedSincPage::RenderFrequency(CPaintDC* pDC,int freqPoints, double f[])
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

	for(int ii=0;ii<freqPoints;ii++) {
		if(f[ii]>dMax) dMax = f[ii];
		if(f[ii]<dMin) dMin = f[ii];
	}
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
	int ii=0;
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

void CWindowedSincPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	int freqPoints = 500;
	double* f = new double[freqPoints+1];


	RenderPhase(&dc,theApp.Fir.nNumTaps,theApp.Fir.m_dAmpFilter);
	theApp.Fir.FilterGain(theApp.Fir.nNumTaps,theApp.Fir.m_dAmpFilter,freqPoints,f);
	RenderFrequency(&dc,freqPoints,f);

	delete f;
}
