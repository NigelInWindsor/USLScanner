// SpectrumAnalyzerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "SpectrumAnalyzerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpectrumAnalyzerDlg dialog


CSpectrumAnalyzerDlg::CSpectrumAnalyzerDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CSpectrumAnalyzerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpectrumAnalyzerDlg)
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_pScreenFFTArray = NULL;
	m_rectView.SetRectEmpty();
	
	theApp.m_LastSettings.fFFTMinFrequency = 0.0;
	theApp.m_LastSettings.fFFTMaxFrequency = 25.0;
	m_nSlot = theApp.m_UtUser.m_Global.nTimeSlot;
}


void CSpectrumAnalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpectrumAnalyzerDlg)
	DDX_Control(pDX, IDC_COMBO_WINDOW, m_comboWindow);
	DDX_Control(pDX, IDC_COMBO_TS, m_comboTimeSlot);
	DDX_Control(pDX, IDC_COMBO_RF_SOURCE, m_comboRFSource);
	DDX_Control(pDX, IDC_COMBO_FFT_RESOLVABLE, m_comboResolvableFrequency);
	DDX_Control(pDX, IDC_SPIN_FFT_GAIN, m_spinGain);
	DDX_Control(pDX, IDC_EDIT_FFT_GAIN, m_editGain);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_USLBARCTRL_FREQUENCY, m_scrollbarFrequency);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpectrumAnalyzerDlg, CDialog)
	//{{AFX_MSG_MAP(CSpectrumAnalyzerDlg)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_FFT_RESOLVABLE, OnSelchangeComboFftResolvable)
	ON_EN_CHANGE(IDC_EDIT_FFT_GAIN, OnChangeEditFftGain)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_RF_SOURCE, OnSelchangeComboRfSource)
	ON_CBN_SELCHANGE(IDC_COMBO_TS, OnSelchangeComboTs)
	ON_CBN_SELCHANGE(IDC_COMBO_WINDOW, OnSelchangeComboWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpectrumAnalyzerDlg message handlers

void CSpectrumAnalyzerDlg::PostNcDestroy() 
{
	DeAllocateScreenMemory();
	
	CDialog::PostNcDestroy();
	delete this;
}

void CSpectrumAnalyzerDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CSpectrumAnalyzerDlg::IDD]=FALSE;

	
	CDialog::OnClose();
	DestroyWindow();
}

void CSpectrumAnalyzerDlg::OnDestroy() 
{
	if (CSpectrumAnalyzerDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CSpectrumAnalyzerDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CSpectrumAnalyzerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	m_scrollbarFrequency.SetMax(250);
	m_scrollbarFrequency.SetMin(0);
	m_scrollbarFrequency.SetHorizontal(TRUE);
	m_scrollbarFrequency.SetWidthHighCursor(15);
	m_scrollbarFrequency.SetWidthLowCursor(15);
	m_scrollbarFrequency.SetMinimumScreenWidth(1);
	CalculateScrollBarFromFrequency();

	m_comboResolvableFrequency.AddString(_T("0.01MHz"));
	m_comboResolvableFrequency.AddString(_T("0.02MHz"));
	m_comboResolvableFrequency.AddString(_T("0.05MHz"));
	m_comboResolvableFrequency.AddString(_T("0.1MHz"));
	m_comboResolvableFrequency.AddString(_T("0.2MHz"));
	m_comboResolvableFrequency.AddString(_T("0.5MHz"));
	m_comboResolvableFrequency.AddString(_T("1.0MHz"));

	m_comboRFSource.AddString(_T("Entire Trace"));
	m_comboRFSource.AddString(_T("Interface Gate"));

	m_comboTimeSlot.AddString(_T("1"));
	m_comboTimeSlot.AddString(_T("2"));
	m_comboTimeSlot.AddString(_T("3"));
	m_comboTimeSlot.AddString(_T("4"));
	m_comboTimeSlot.AddString(_T("5"));
	m_comboTimeSlot.AddString(_T("6"));
	m_comboTimeSlot.AddString(_T("7"));
	m_comboTimeSlot.AddString(_T("8"));

	m_spinGain.SetRange(1,800);



	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosFrequency.Initialize(this, m_scrollbarFrequency, FALSE, TRUE, TRUE, TRUE);

	if (CSpectrumAnalyzerDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CSpectrumAnalyzerDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			if(rect.Width()>1024) rect.right=rect.left+1024;
			if(rect.Height()>512) rect.bottom=rect.top+512;
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
		};
	}
	
	m_StaticPosView.SetNewPos();
	m_StaticPosFrequency.SetNewPos();
	Buff.LoadString(IDS_Square); m_comboWindow.AddString(Buff);
	Buff.LoadString(IDS_Parzen); m_comboWindow.AddString(Buff);
	Buff.LoadString(IDS_Welch); m_comboWindow.AddString(Buff);
	Buff.LoadString(IDS_Hanning); m_comboWindow.AddString(Buff);

	UpdateAllControls();

	AllocateScreenMemory();
	
	
	SetTimer(1,250,NULL);
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSpectrumAnalyzerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CSpectrumAnalyzerDlg::UpdateAllControls()
{
	CString Buff;

	m_comboResolvableFrequency.SetCurSel(theApp.m_LastSettings.nFFTResolvableFrequency);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nFFTGain);
	m_editGain.SetWindowText(Buff);
	m_spinGain.SetPos(theApp.m_LastSettings.nFFTGain);
	m_comboRFSource.SetCurSel(theApp.m_LastSettings.nFFTRfSource);

	m_comboTimeSlot.SetCurSel(theApp.m_LastSettings.nFFTTS);
	m_comboWindow.SetCurSel(theApp.m_LastSettings.nFFTWindow);


}

void CSpectrumAnalyzerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect rect = theApp.m_LastSettings.RectWindowPos[CSpectrumAnalyzerDlg::IDD];
	if (CSpectrumAnalyzerDlg::IDD < MAXIMUMWINDOWSIDNUMBER && GetSafeHwnd()) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CSpectrumAnalyzerDlg::IDD]);
	}

	m_StaticPosView.SetNewPos();   
	m_StaticPosFrequency.SetNewPos();  

	if (GetSafeHwnd()) {
	
		AllocateScreenMemory();

		Invalidate(TRUE);
	}	
}


void CSpectrumAnalyzerDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CalculateGArray();
	DrawFFTTrace(&dc,&m_rectView,SCREEN);
	
}

void CSpectrumAnalyzerDlg::AllocateScreenMemory()
{
	CRect rr;

	if (m_staticView.GetSafeHwnd()) {
		m_staticView.GetWindowRect(&rr);
		ScreenToClient(&rr);
		if ((rr != m_rectView) || (m_pScreenFFTArray == NULL)) {
			if (m_pScreenFFTArray != NULL) delete m_pScreenFFTArray;
			m_pScreenFFTArray = new COLORREF[(rr.Width() + 2) * (rr.Height() + 2)];
			m_rectView = rr;
		}
	}
}

void CSpectrumAnalyzerDlg::DeAllocateScreenMemory()
{
	if(m_pScreenFFTArray!=NULL) delete m_pScreenFFTArray;
	m_pScreenFFTArray = NULL;
}

void CSpectrumAnalyzerDlg::DrawFFTTrace(CPaintDC *pDC, CRect *prr, int nAction)
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	CString BuffDelay,BuffWidth;
	CPen* pOldPen;
	int	jj;
	CDC  dcMem;
	CBitmap bmp;
	static int offset=0;
	CFont font;
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[theApp.m_LastSettings.nFFTTS].Adc;
	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = theApp.m_Scope.m_nTextSize;
	CFont Font;
	Font.CreateFontIndirect(&lf);
	
	bmp.CreateCompatibleBitmap(pDC,prr->Width(),prr->Height());
	bmp.SetBitmapBits((DWORD)(prr->Width()*prr->Height()*4),m_pScreenFFTArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CFont* pOldFont = dcMem.SelectObject(&Font);
	dcMem.SetTextColor(theApp.m_Scope.m_rgbTextColor);
	dcMem.SetBkMode(TRANSPARENT);

	CBrush brushBk(theApp.m_Scope.m_rgbBkColor);
	CBrush* pOldBrush = dcMem.SelectObject(&brushBk);
	dcMem.Rectangle(0,0,prr->right,prr->bottom);
	dcMem.SelectObject(pOldBrush);

	DrawGraticule(&dcMem,prr,nAction);

	//Draw Trace
	CPen OnPen(PS_SOLID,1,theApp.m_Scope.m_rgbTraceColor);
	pOldPen = dcMem.SelectObject(&OnPen);

	dcMem.MoveTo(0,nGArray[0]);
	for(jj=0;jj<prr->Width();jj++) {
		dcMem.LineTo(jj,nGArray[jj]);
	};


	pDC->BitBlt(prr->left,prr->top,prr->Width(),prr->Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBrush);
	dcMem.SelectObject(pOldBitmap);
	dcMem.SelectObject(pOldPen);
	dcMem.SelectObject(pOldFont);


}


void CSpectrumAnalyzerDlg::CalculateGArray()
{
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[theApp.m_LastSettings.nFFTTS].Adc;
	DSP200Data* pDsp = &theApp.m_UtUser.m_TS[theApp.m_LastSettings.nFFTTS].Dsp;
	int xx,vv,ptr,nY;
	float fMaxFrequency;
	char Array[8192];
	int nDelay,nWidth;
	double dLogAmp;


	switch(theApp.m_LastSettings.nFFTRfSource) {
	case 0:
		m_FFT.DoFastFourierTransform(theApp.m_Scope.m_RFTrace[theApp.m_LastSettings.nFFTTS],pAdc->nAcqLength,pAdc->nAcqLength);
/*
		ZeroMemory(nRealArray,sizeof nRealArray);
		for(int gg=0;gg<8;gg++) {
			for(int ii=0;ii<pAdc->nAcqLength;ii++) {
				nRealArray[ii] += theApp.m_Scope.m_RFTrace[theApp.m_LastSettings.nFFTTS][ii];
			}
			Sleep(2);
		}
		m_FFT.DoFastFourierTransform(nRealArray,pAdc->nAcqLength,pAdc->nAcqLength,8);
		*/
		break;
	case 1:
		nDelay = pDsp->Delay[0];
		nWidth = pDsp->Width[0];
		if(nDelay > (int)pAdc->nAcqLength) nDelay = 0;
		if(nDelay < 0) nDelay = 0;
		if(nWidth<0) nWidth = 10;
		if(nWidth > (int)pAdc->nAcqLength) nWidth = 10;
		memset(Array,0,sizeof Array);
		for(int ii=0,jj=nDelay;ii<nWidth;ii++,jj++) {
			Array[ii]=theApp.m_Scope.m_RFTrace[theApp.m_LastSettings.nFFTTS][jj];
		}
		m_FFT.DoFastFourierTransform(Array,pAdc->nAcqLength,nWidth);
		break;
	}

	fMaxFrequency = 1.0f / (pAdc->fSamplePeriod * 2e-3f);

	int nS0 = (int)(((float)(m_FFT.m_nRFLength/2) * theApp.m_LastSettings.fFFTMinFrequency) / fMaxFrequency);
	int nS1 = (int)(((float)(m_FFT.m_nRFLength/2) * theApp.m_LastSettings.fFFTMaxFrequency) / fMaxFrequency);
	for(xx=0;xx<m_rectView.Width();xx++) {
		ptr=MulDiv(xx,nS1-nS0,m_rectView.Width()) + nS0;
		if(ptr<0) ptr=0;
		if(ptr<(m_FFT.m_nRFLength/2)) {
			nY = MulDiv(m_FFT.peak[ptr]*theApp.m_LastSettings.nFFTGain,m_rectView.Height(),0x7fff);
			if (m_FFT.peak[ptr] != 0) {
				dLogAmp = 20.0 * log10((double)(m_FFT.peak[ptr] * theApp.m_LastSettings.nFFTGain) / 32767.0);
				nY = (int)((FFT_MAX_RANGE + dLogAmp) * (double)m_rectView.Height() / FFT_MAX_RANGE);
				vv = m_rectView.Height() - nY;
			} else {
				vv = m_rectView.Height() - 1;
			}
		}
		nGArray[xx] = MinMax(&vv, 0, m_rectView.Height() - 1);
	};

}

void CSpectrumAnalyzerDlg::DrawGraticule(CDC* pDC,CRect* prr,int nAction)
{
	CPen* pPen;
	CPen *pOldPen;
	int	nPnx,nPny,nRow,nCol;
	CString Buff;
	CSize size;

	if(theApp.m_Scope.m_bGraticule!=TRUE) return;
	pPen = new CPen;
	pPen->CreatePen(PS_SOLID,0,theApp.m_Scope.m_rgbGraticuleColor);
	pOldPen=pDC->SelectObject(pPen);

	for(nRow=1;nRow<10;nRow++) {
		nPny=MulDiv(nRow,prr->Height(),10);
		pDC->MoveTo(0,nPny);
		pDC->LineTo(prr->Width(),nPny);
		Buff.Format(_T("%.01fdB"),(float)(FFT_MAX_RANGE * nRow) / -10.0f);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(2,nPny-size.cy/2,Buff);
	};
	for(nCol=1;nCol<10;nCol++) {
		nPnx=MulDiv(nCol,prr->Width(),10);
		pDC->MoveTo(nPnx,0);
		pDC->LineTo(nPnx,prr->Height());
		Buff.Format(_T("%.01f"),(((theApp.m_LastSettings.fFFTMaxFrequency-theApp.m_LastSettings.fFFTMinFrequency) * (float)nCol) / 10.0) +theApp.m_LastSettings.fFFTMinFrequency);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(nPnx-size.cx/2,0,Buff);
	};

	pDC->SelectObject(pOldPen);
	delete pPen;


}



void CSpectrumAnalyzerDlg::OnTimer(UINT nIDEvent) 
{
	Invalidate(FALSE);
	CDialog::OnTimer(nIDEvent);
}


void CSpectrumAnalyzerDlg::CalculateScrollBarFromFrequency()
{
	int	nMinFrequency = (int)theApp.m_LastSettings.fFFTMinFrequency;
	int	nMaxFrequency = (int)theApp.m_LastSettings.fFFTMaxFrequency;

	m_scrollbarFrequency.SetLowPosition(nMinFrequency);
	m_scrollbarFrequency.SetHighPosition(nMaxFrequency);

}

BEGIN_EVENTSINK_MAP(CSpectrumAnalyzerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSpectrumAnalyzerDlg)
//	ON_EVENT(CSpectrumAnalyzerDlg, IDC_USLBARCTRL_FREQ, 1 /* ScrollHasMoved */, OnScrollHasMovedUslbarctrlFreq, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CSpectrumAnalyzerDlg, IDC_USLBARCTRL_FREQUENCY, 1 /* ScrollHasMoved */, OnScrollHasMovedUslbarctrlFrequency, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSpectrumAnalyzerDlg::OnScrollHasMovedUslbarctrlFrequency(long LowPosition, long HighPosition, long Width) 
{
	theApp.m_LastSettings.fFFTMinFrequency = (float)m_scrollbarFrequency.GetLowPosition();
	theApp.m_LastSettings.fFFTMaxFrequency = (float)m_scrollbarFrequency.GetHighPosition();
}

void CSpectrumAnalyzerDlg::OnSelchangeComboFftResolvable() 
{
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[theApp.m_LastSettings.nFFTTS].Adc;

	KillTimer(1);
	switch(theApp.m_LastSettings.nFFTResolvableFrequency = m_comboResolvableFrequency.GetCurSel()) {
	case 0: m_FFT.SetRFLength(pAdc->fSamplePeriod , 0.01f);
		break;
	case 1: m_FFT.SetRFLength(pAdc->fSamplePeriod , 0.02f);
		break;
	case 2: m_FFT.SetRFLength(pAdc->fSamplePeriod , 0.05f);
		break;
	case 3: m_FFT.SetRFLength(pAdc->fSamplePeriod , 0.1f);
		break;
	case 4: m_FFT.SetRFLength(pAdc->fSamplePeriod , 0.2f);
		break;
	case 5: m_FFT.SetRFLength(pAdc->fSamplePeriod , 0.5f);
		break;
	case 6: m_FFT.SetRFLength(pAdc->fSamplePeriod , 1.0f);
		break;
	}
	SetTimer(1,250,NULL);


	
}

void CSpectrumAnalyzerDlg::OnChangeEditFftGain() 
{
	CString Buff;
	int nGain = theApp.m_LastSettings.nFFTGain;

	m_editGain.GetWindowText(Buff);
	theApp.m_LastSettings.nFFTGain=_ttoi(Buff);
	if(nGain - theApp.m_LastSettings.nFFTGain) {
		m_spinGain.SetPos(theApp.m_LastSettings.nFFTGain);
	}

}

void CSpectrumAnalyzerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;
	int nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_FFT_GAIN:
		nTemp=theApp.m_LastSettings.nFFTGain;
		theApp.m_LastSettings.nFFTGain=(pSpin->GetPos()&0xffff);
		if(nTemp-theApp.m_LastSettings.nFFTGain) {
			Buff.Format(_T("%d"),theApp.m_LastSettings.nFFTGain);
			m_editGain.SetWindowText(Buff);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CSpectrumAnalyzerDlg::OnSelchangeComboRfSource() 
{
	theApp.m_LastSettings.nFFTRfSource=m_comboRFSource.GetCurSel();
}


void CSpectrumAnalyzerDlg::OnSelchangeComboTs() 
{
	theApp.m_LastSettings.nFFTTS = m_comboTimeSlot.GetCurSel();
}

void CSpectrumAnalyzerDlg::OnSelchangeComboWindow() 
{
	theApp.m_LastSettings.nFFTWindow=m_comboWindow.GetCurSel();
}
