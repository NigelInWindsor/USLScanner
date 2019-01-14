// HistogramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "HistogramDlg.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CHistogramDlg dialog
#define _HIST_PEAK_TRACK	0x01
#define _HIST_MEAN_TRACK	0x02
#define _HIST_TROUGH_TRACK	0x04
#define	_HIST_VERBOSE		0x08


CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nImageType,int nImageNumber,int nPaletteNumber,CData* pData,int nTS)
	: CDialog(CHistogramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistogramDlg)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pData = pData;
	m_nImageNumber = nImageNumber;
	m_nImageType = nImageType;
	m_nTS = nTS;
	m_nPaletteNumber = nPaletteNumber;
	m_bLBDown = false;
	
	m_nLastThreshold = 0;
	m_pListItemEdit = NULL;

	m_pData->m_Hist.nAmplitudeScaleType = theApp.m_Palette.m_nAmplitudeScaleType[m_nPaletteNumber];
	switch(m_pData->m_TS[m_nTS].Pr30.nOutputSelect) {
	case 0:
	case 1:
		if((m_pData->m_Hist.nAmplitudeScaleType != SCALE_PERCENTAGE) || (m_pData->m_Hist.nAmplitudeScaleType != SCALE_LINEAR_DB)) {
			m_pData->m_Hist.nAmplitudeScaleType = SCALE_PERCENTAGE;
		}
		break;
	case 2:
		if(m_pData->m_Hist.nAmplitudeScaleType != SCALE_LOG) {
			m_pData->m_Hist.nAmplitudeScaleType = SCALE_LOG;
		}
		break;
	}


	m_pArray = NULL;
	m_nArrayWidth=-1;
	m_nArrayHeight=-1;



	switch(theApp.m_LastSettings.nHistogramMask & (_HIST_PEAK_TRACK + _HIST_MEAN_TRACK + _HIST_TROUGH_TRACK)) {
	default: m_bTrackPeak = FALSE;
		m_bTrackMean = FALSE;
		m_bTrackAtten = FALSE;
		break;
	case 1: m_bTrackPeak = TRUE;
		m_bTrackMean = FALSE;
		m_bTrackAtten = FALSE;
		break;
	case 2: m_bTrackPeak = FALSE;
		m_bTrackMean = TRUE;
		m_bTrackAtten = FALSE;
		break;
	case 4: m_bTrackPeak = FALSE;
		m_bTrackMean = FALSE;
		m_bTrackAtten = TRUE;
		break;
	}

	ZeroMemory(m_penHist, sizeof m_penHist);

}


void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistogramDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialog)
	//{{AFX_MSG_MAP(CHistogramDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_TRACK_PEAK, OnButtonTrackPeak)
	ON_COMMAND(ID_BUTTON_TRACK_MEAN, OnButtonTrackMean)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_BUTTON_LOG, OnButtonLog)
	ON_COMMAND(ID_BUTTON_LINEAR_DB, OnButtonLinearDb)
	ON_COMMAND(ID_BUTTON_PERCENTAGE, OnButtonPercentage)
	ON_COMMAND(ID_BUTTON_POROSITY, OnButtonPorosity)
	ON_COMMAND(ID_BUTTON_TRACK_MAX_ATTENUATION, OnButtonTrackMaxAttenuation)
	ON_COMMAND(ID_BUTTON_DISPLAY_HISTOGRAM, OnButtonDisplayHistogram)
	ON_COMMAND(ID_BUTTON_DISPLAY_DROPOUT, OnButtonDisplayDropout)
	ON_COMMAND(ID_BUTTON_VERBOSE, OnButtonVerbose)
	ON_COMMAND(ID_BUTTON_PAINT_2, OnButtonPaint2)
	ON_COMMAND(ID_BUTTON_PAINT_12, OnButtonPaint12)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(UI_ITEMCHANGED, ItemChanged)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistogramDlg message handlers

void CHistogramDlg::PostNcDestroy() 
{
	int ii;

	SAFE_DELETE( m_pArray );
	for(ii=0;ii<65536;ii++) {
		SAFE_DELETE( m_penHist[ii] );
	};

	CDialog::PostNcDestroy();
	delete this;
}

void CHistogramDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CHistogramDlg::IDD]=FALSE;

	CViewSheet* pViewSheet = (CViewSheet*)GetParent();
	CViewPage *pPage = (CViewPage*)pViewSheet->GetActivePage();
	pPage->m_nToolOption=0;
	pViewSheet->m_nToolOption=0;
	pViewSheet->SetToolBarCheckedState();


	
	CDialog::OnClose();
	DestroyWindow();
}

void CHistogramDlg::OnDestroy() 
{
	if (CHistogramDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CHistogramDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CHistogramDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);

	if (CHistogramDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CHistogramDlg::IDD];
		theApp.ConfineToScreen(&rect);
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
		}
	}
	m_StaticPosView.SetNewPos();
	AllocateScreenArray();
	
	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);
	SetToolBarCheckedState();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHistogramDlg::AllocateScreenArray()
{
	CRect rr;

	if (GetSafeHwnd() && m_staticView.GetSafeHwnd()) {
		m_staticView.GetWindowRect(&rr);
		ScreenToClient(&rr);

		SAFE_DELETE(m_pArray);
		m_pArray = new COLORREF[(rr.Width() + 2) * (rr.Height() + 2)];
		m_nArrayWidth = rr.Width();
		m_nArrayHeight = rr.Height();
	}
}


void CHistogramDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosView.SetNewPos();
	if(GetSafeHwnd()) {
		AllocateScreenArray();
		Invalidate(FALSE);
	}
	
}

void CHistogramDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC  dcMem;
	CBitmap bmp;
	CRect rr;

	m_staticView.GetWindowRect(&m_rrView);
	ScreenToClient(&m_rrView);
	m_rrView.DeflateRect(1,1);

	bmp.CreateCompatibleBitmap(&dc,m_rrView.Width(),m_rrView.Height());
	bmp.SetBitmapBits((DWORD)(m_rrView.Width()*m_rrView.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	
	rr=m_rrView;
	rr.OffsetRect(-m_rrView.left,-m_rrView.top);
	dcMem.FillRect(&rr,&CBrush(GetSysColor(COLOR_INFOBK)));

	switch(m_nImageType) {
	case AMP_IMAGE:
		DrawAmpScale(&dcMem);
		DrawAmpHistogram(&dcMem);
		DrawAmpStatistics(&dcMem);
		break;
	case TIME_IMAGE:
		DrawTimeScale(&dcMem);
		DrawTimeHistogram(&dcMem);
		DrawTimeStatistics(&dcMem);
		break;
	case EXT_IMAGE:
		DrawExtScale(&dcMem);
		DrawExtHistogram(&dcMem);
//		DrawExtStatistics(&dcMem);
		break;
	}

	dc.BitBlt(m_rrView.left,m_rrView.top,m_rrView.Width(),m_rrView.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

}

void CHistogramDlg::DrawAmpScale(CDC* pDC)
{
	CFont HorizontalFont;
	CFont VerticalFont;
	CSize size;
	CString Buff;
	int	ii,nPny;
	CRect rr;
	float fdB,fAmp;
	float fValue[256];

	if(theApp.m_LastSettings.fHistogramRelativedB[0] <= 0.0f) {
		m_pData->m_Hist.nAmpMode = HIST_ATTENUATION;
	} else {
		m_pData->m_Hist.nAmpMode = HIST_REFLECTION;
	}

	for(ii=0;ii<theApp.m_Palette.m_nAmplitudeEntriesl[m_nPaletteNumber];ii++) fValue[ii] = theApp.m_Palette.m_Amplitude[m_nPaletteNumber][ii].fValue;

//	theApp.m_LastSettings.lfHistogram = theApp.m_LastSettings.lfViewGraticule;

	theApp.m_LastSettings.lfHistogram.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfHistogram);
	theApp.m_LastSettings.lfHistogram.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&theApp.m_LastSettings.lfHistogram);

	CFont* pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);
	Buff = "-00.000-";
	size = pDC->GetTextExtent(Buff);

	m_rrGraticule = m_rrView;
	m_rrGraticule.OffsetRect(-m_rrView.left,-m_rrView.top);
	
	m_rrGraticule.left+=size.cx;
	m_rrGraticule.bottom-=size.cy;
	m_rrGraticule.right-=size.cx;
	if(theApp.m_LastSettings.nHistogramMask & _HIST_VERBOSE) {
		m_rrGraticule.top+=8*size.cy;
	} else {
		m_rrGraticule.top+=6*size.cy;
	}
	pDC->FillRect(&m_rrGraticule,&CBrush(theApp.m_LastSettings.rgbHistogramBkColor));


	pDC->MoveTo(m_rrGraticule.left-1,m_rrGraticule.top);
	pDC->LineTo(m_rrGraticule.left-1,m_rrGraticule.bottom+1);
	pDC->LineTo(m_rrGraticule.right,m_rrGraticule.bottom+1);



	if(m_pData->m_Hist.nAmpMode == HIST_ATTENUATION) {
		switch(m_pData->m_Hist.nAmplitudeScaleType) {
		case 0:
			for(ii=100;ii>0;ii-=10) {

				fAmp = ((float)ii * m_pData->m_Hist.fLatchAmp) / 100.0f;
				nPny=m_rrGraticule.bottom - (int)((fAmp*(float)m_rrGraticule.Height())/100.0f);

				pDC->MoveTo(m_rrGraticule.left,nPny);
				pDC->LineTo(m_rrGraticule.left-4,nPny);
				Buff.Format(_T("%d"),ii);
				size = pDC->GetTextExtent(Buff);
				pDC->TextOut(m_rrGraticule.left-size.cx-4,nPny-size.cy/2,Buff);
			};
			break;
		case 1:
			for(ii=0;ii<theApp.m_Palette.m_nAmplitudeEntriesl[m_nPaletteNumber];ii++) {

				if((fValue[0] / fValue[ii]) != 0.0f) {
					fAmp = m_pData->m_Hist.fLatchAmp / (fValue[0] / fValue[ii]);
				} else {
					fAmp = m_pData->m_Hist.fLatchAmp;
				}

				fdB = -20.0f * (float)log10(m_pData->m_Hist.fLatchAmp/fAmp);

				nPny=m_rrGraticule.bottom - (int)((fAmp*(float)m_rrGraticule.Height())/100.0f);

				pDC->MoveTo(m_rrGraticule.left,nPny);
				pDC->LineTo(m_rrGraticule.left-4,nPny);
				Buff.Format(_T("%.01f"),fdB);
				size = pDC->GetTextExtent(Buff);
				pDC->TextOut(m_rrGraticule.left-size.cx-4,nPny-size.cy/2,Buff);
			};
			break;
		case 2:
			for(ii=0;ii<m_pData->m_nLogRange;ii+=10) {
				nPny = m_rrGraticule.bottom - (int)(m_pData->m_Hist.fLatchAmp * (float)m_rrGraticule.Height() / 100.0f);
				nPny += MulDiv(ii,m_rrGraticule.Height(),m_pData->m_nLogRange);
				if(nPny <= m_rrGraticule.bottom) {
					pDC->MoveTo(m_rrGraticule.left,nPny);
					pDC->LineTo(m_rrGraticule.left-2,nPny);
					Buff.Format(_T("%d"),-ii);
					size = pDC->GetTextExtent(Buff);
					pDC->TextOut(m_rrGraticule.left-size.cx-4,nPny-size.cy/2,Buff);
				}
			};
			break;
		}
	} else { //HIST_REFLECTION
		switch(m_pData->m_Hist.nAmplitudeScaleType) {
		case 0:
			for(ii=0;ii<=100;ii+=10) {

				nPny=m_rrGraticule.bottom - (int)((float)ii*(float)m_rrGraticule.Height()/100.0f);

				pDC->MoveTo(m_rrGraticule.left,nPny);
				pDC->LineTo(m_rrGraticule.left-4,nPny);
				Buff.Format(_T("%d"),ii);
				size = pDC->GetTextExtent(Buff);
				pDC->TextOut(m_rrGraticule.left-size.cx-4,nPny-size.cy/2,Buff);
			};
			break;
		case 1:
			for(ii=0;ii<theApp.m_Palette.m_nAmplitudeEntriesl[m_nPaletteNumber];ii++) {

				if((fValue[0] / fValue[ii]) != 0.0f) {
					fAmp = 100.0f / (fValue[0] / fValue[ii]);
				} else {
					fAmp = 100.0f;
				}

				fdB = -20.0f * (float)log10(100.0f/fAmp);

				nPny=m_rrGraticule.bottom - (int)((fAmp*(float)m_rrGraticule.Height())/100.0f);

				pDC->MoveTo(m_rrGraticule.left,nPny);
				pDC->LineTo(m_rrGraticule.left-4,nPny);
				Buff.Format(_T("%.01f"),fdB);
				size = pDC->GetTextExtent(Buff);
				pDC->TextOut(m_rrGraticule.left-size.cx-4,nPny-size.cy/2,Buff);
			};
			break;
		case 2:
			for(ii=0;ii<m_pData->m_nLogRange;ii+=10) {
				nPny = m_rrGraticule.top + MulDiv(ii,m_rrGraticule.Height(),m_pData->m_nLogRange);
				if(nPny <= m_rrGraticule.bottom) {
					pDC->MoveTo(m_rrGraticule.left,nPny);
					pDC->LineTo(m_rrGraticule.left-2,nPny);
					Buff.Format(_T("%d"),-ii);
					size = pDC->GetTextExtent(Buff);
					pDC->TextOut(m_rrGraticule.left-size.cx-4,nPny-size.cy/2,Buff);
				}
			};
			break;
		}
	}
	pDC->SelectObject(pOldFont);
}

void CHistogramDlg::DrawAmpHistogram(CDC* pDC)
{
	int	ii,nX,nY,nY0,nY1,nX0,nX1,nColor;
	int	nSum=0,nThresholdSum=0;
	CString Buff;
	CString Units;
	CFont Font;
	CSize size;
	CRect	rr;
	float fThresh[2];
	float fAmp;

	if(m_pData->m_Hist.pnData==NULL) return;

	CPen penColor0(PS_SOLID,1,theApp.m_LastSettings.rgbHistogramColor[0]);
	CPen penColor1(PS_SOLID,1,theApp.m_LastSettings.rgbHistogramColor[1]);
	CPen* pen[256];

	for(int gg=0;gg<2;gg++) {
		switch(m_pData->m_Hist.nAmplitudeScaleType) {
		case 0:
		case 1:	fAmp = m_pData->m_Hist.fLatchAmp / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[gg]/-20.0f);
			break;
		case 2:	fAmp = m_pData->m_Hist.fLatchAmp + (theApp.m_LastSettings.fHistogramRelativedB[gg]*100.0f/(float)m_pData->m_nLogRange);
			break;
		}
		if(fAmp>100.0f) fAmp = 100.0f;
		if(fAmp<0.0f) fAmp = 0.0f;
		fThresh[gg]= fAmp*(float)(m_rrGraticule.Height()-1)/100.0f;
	}

	for(ii=0;ii<128;ii++) {
		pen[ii] = new CPen(PS_SOLID,1,theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii+128]);
	}

	CPen* pOldPen = pDC->SelectObject(pen[0]);
	//Fill the interior

	for(nColor=0;nColor<127;nColor++) {

		nY0=MulDiv(nColor, m_rrGraticule.Height()-1, 127);
		nY1=MulDiv(nColor+1, m_rrGraticule.Height()-1, 127);

		nX0 = m_rrGraticule.left + MulDiv(m_pData->m_Hist.pnData[nColor+128],m_rrGraticule.Width()-1,m_pData->m_Hist.nMaxAmp);
		nX1 = m_rrGraticule.left + MulDiv(m_pData->m_Hist.pnData[nColor+129],m_rrGraticule.Width()-1,m_pData->m_Hist.nMaxAmp);

		for(nY=nY0;nY<nY1;nY++) {
			if(nX0>0 && nX1>0) {
				nX = MulDiv(nX1-nX0,nY-nY0,nY1-nY0) + nX0;
			} else {
				nX = nX0;
			}
			fAmp = (float)nY * (float)(m_rrGraticule.Height()-1) / 127.0f;

			pDC->SelectObject(pen[nColor]);
			if(m_pData->m_Hist.nAmpMode == HIST_ATTENUATION) {
				if(theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) {
					if((nY>fThresh[1]) && (nY<fThresh[0])) {
						pDC->SelectObject(penColor0);
					}
				}
				if(theApp.m_LastSettings.bHistogramPaintView[1]==TRUE) {
					if(nY<fThresh[1]) {
						pDC->SelectObject(penColor1);
					}
				}
			} else {
				if(theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) {
					if((nY>=fThresh[0]) && (nY<fThresh[1])) {
						pDC->SelectObject(penColor0);
					}
				}
				if(theApp.m_LastSettings.bHistogramPaintView[1]==TRUE) {
					if(nY>=fThresh[1]) {
						pDC->SelectObject(penColor1);
					}
				}
			}
			pDC->MoveTo(m_rrGraticule.left,m_rrGraticule.bottom-nY);
			pDC->LineTo(nX,m_rrGraticule.bottom-nY);
		}
	}
	pDC->SelectObject(pOldPen);

	for(ii=0;ii<128;ii++) 
		delete pen[ii];

}
void CHistogramDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CRect rr, rrClient;
	int gg;
	CString Buff = L"-100dB";
	CDC *pDC = GetDC();
	CSize size = pDC->GetTextExtent(Buff);
	size.cy += 2;
	ReleaseDC(pDC);

	rr.left = m_rrGraticule.left + m_rrView.left;
	rr.right = m_rrView.right;

	switch(m_nImageType) {
	case AMP_IMAGE:

		rr.top = m_nLatchAmpY - 6 + m_rrView.top;
		rr.bottom = rr.top + 12;
		if(rr.PtInRect(point)) {
			m_nToolOption = 2;
			if(m_pData->m_Hist.fLatchAmp==100.0f) break;
		}

		for(gg=0;gg<2;gg++) {
			rr.top = m_nLatchThresholdY[gg] - 6 + m_rrView.top;
			rr.bottom = rr.top + 12;
			if(rr.PtInRect(point)) {
				m_nToolOption = 3 + gg;
				m_nLastThreshold = (m_nToolOption - 3);
				GetClientRect(&rrClient);
				m_staticView.GetWindowRect(&rr);
				rr.left = rr.right - (size.cx * 3) / 2;
				rr.top += (m_nLatchThresholdY[gg] - size.cy / 2);
				rr.bottom = rr.top + size.cy;
				m_pListItemEdit = new CListItemEdit(this, (CWnd**)&m_pListItemEdit, NULL, NULL, &rr);
				m_pListItemEdit->Create(CListItemEdit::IDD, this, &theApp.m_LastSettings.fHistogramRelativedB[gg], -100.0f, 100.0f, 1.0e-1f, L"%.01f dB");
			}
		}
		break;
	case TIME_IMAGE:
		for(gg=2;gg>=0;gg--) {
			rr.top = m_nLatchTimeY[gg] - 4 + m_rrView.top;
			rr.bottom = rr.top + 8;
			if(rr.PtInRect(point)) {
				m_nToolOption = 2 + gg;
				break;
			}
		}
		break;
	case EXT_IMAGE:
		break;
	}

	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CHistogramDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	int gg;

	if (m_pListItemEdit) {
		SAFE_DELETE(m_pListItemEdit);
		return;
	}

	rr.left = m_rrGraticule.left + m_rrView.left;
	rr.right = m_rrView.right;
	m_bLBDown = true;

	switch(m_nImageType) {
	case AMP_IMAGE:

		rr.top = m_nLatchAmpY - 6 + m_rrView.top;
		rr.bottom = rr.top + 12;
		if(rr.PtInRect(point)) {
			m_nToolOption = 2;
			if(m_pData->m_Hist.fLatchAmp==100.0f) break;
		}

		for(gg=0;gg<2;gg++) {
			rr.top = m_nLatchThresholdY[gg] - 6 + m_rrView.top;
			rr.bottom = rr.top + 12;
			if(rr.PtInRect(point)) {
				m_nToolOption = 3 + gg;
				m_nLastThreshold = (m_nToolOption - 3);
			}
		}
		break;
	case TIME_IMAGE:
		for(gg=2;gg>=0;gg--) {
			rr.top = m_nLatchTimeY[gg] - 4 + m_rrView.top;
			rr.bottom = rr.top + 8;
			if(rr.PtInRect(point)) {
				m_nToolOption = 2 + gg;
				break;
			}
		}
		break;
	case EXT_IMAGE:
		break;
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CHistogramDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_nToolOption = 0;
	m_bLBDown = false;
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CHistogramDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	float fAmp,fThickness,fTime;
	CRect rr;
	rr.SetRectEmpty();

	if (m_bLBDown == false) return;

	switch(m_nImageType) {
	case AMP_IMAGE:
		fAmp =  (float)MulDiv(m_rrGraticule.bottom-point.y+m_rrView.top,10000,m_rrGraticule.Height()-1) / 100.0f; //minimum resolvable 0.01%

		if(m_nToolOption >= 2) {
			switch(m_pData->m_Hist.nAmplitudeScaleType) {
			case 0:
			case 1:	
				switch(m_nToolOption) {
				case 2:m_pData->m_Hist.fLatchAmp = fAmp;
					break;
				case 3:
				case 4:
					theApp.m_LastSettings.fHistogramRelativedB[m_nLastThreshold = (m_nToolOption - 3)] = -20.0f * (float)log10(m_pData->m_Hist.fLatchAmp / fAmp);
					break;
				}
				break;
			case 2:
				switch(m_nToolOption) {
				case 2:m_pData->m_Hist.fLatchAmp = fAmp;
					if(m_pData->m_Hist.fLatchAmp>100.0f) m_pData->m_Hist.fLatchAmp=100.0f;
					break;
				case 3:
				case 4:
					theApp.m_LastSettings.fHistogramRelativedB[m_nLastThreshold = (m_nToolOption - 3)] = (fAmp - m_pData->m_Hist.fLatchAmp)*(float)m_pData->m_nLogRange/100.0f;
					break;
				}
				break;
			}
//			if((Signf(theApp.m_LastSettings.fHistogramRelativedB[0]) == POSITIVE) && (theApp.m_LastSettings.fHistogramRelativedB[1] < theApp.m_LastSettings.fHistogramRelativedB[0])) {
//				theApp.m_LastSettings.fHistogramRelativedB[1] = theApp.m_LastSettings.fHistogramRelativedB[0]+0.1f;
//			}
//			if((Signf(theApp.m_LastSettings.fHistogramRelativedB[0]) == NEGATIVE) && (theApp.m_LastSettings.fHistogramRelativedB[1] > theApp.m_LastSettings.fHistogramRelativedB[0])) {
//				theApp.m_LastSettings.fHistogramRelativedB[1] = theApp.m_LastSettings.fHistogramRelativedB[0]-0.1f;
//			}
			m_pData->ReBuildHistogramData();
			Invalidate(FALSE);
		}
		break;
	case TIME_IMAGE:

		fThickness =  (((float)(m_rrGraticule.bottom-point.y+m_rrView.top) * (float)theApp.m_Palette.m_dThicknessRange[0]) / (float)(m_rrGraticule.Height()-1))+(float)theApp.m_Palette.m_dMinThickness[0];
		fTime = (fThickness * 200000.0f) / (float)theApp.m_LastSettings.nMaterialVelocity;

		if(m_nToolOption >= 2) {
			switch(m_nToolOption) {
			case 2:m_pData->m_Hist.fLatchTime = fTime;
				m_pData->m_Hist.fLatchThickness = fThickness;
				break;
			case 3:
			case 4:
				theApp.m_LastSettings.fHistogramRelativeTime[m_nLastThreshold = (m_nToolOption - 3)] = m_pData->m_Hist.fLatchTime - fTime;
				theApp.m_LastSettings.fHistogramRelativeThickness[m_nToolOption - 3] = m_pData->m_Hist.fLatchThickness - fThickness;
				break;
			}
			m_pData->ReBuildHistogramData();
			Invalidate(FALSE);
		}

		break;
	case EXT_IMAGE:
		break;
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CHistogramDlg::DrawAmpStatistics(CDC *pDC)
{
	if(m_pData->m_Hist.pnData==NULL) return;

	int	nX,nY;
	float fAmp;
	float fPlies;
	CCoord Cp;
	CString Buff;
	CFont Font;
	CRect rr;
	CSize size;
	CString Peak,Mean,SD,TotalArea,Ref,Reference,BelowFSH,AreaBelowRef,AreaGreaterThanRef,Max,Min;
	Peak.LoadString(IDS_Peak);
	Mean.LoadString(IDS_Mean);
	TotalArea.LoadString(IDS_Total_Area);
	Ref.LoadString(IDS_Ref);
	Reference.LoadString(IDS_The_Reference_Amplitude_Is);
	BelowFSH.LoadString(IDS_Below_FSH);
	AreaBelowRef.LoadString(IDS_Area_Below_Ref);
	AreaGreaterThanRef.LoadString(IDS_Area_Greater_Than_Ref);
	SD = L"SD";
	Max.LoadString(IDS_Max);
	Min.LoadString(IDS_Min);

	LOGFONT lf;
	ZeroMemory(&lf,sizeof lf);
	lf.lfHeight = theApp.m_LastSettings.lfHistogram.lfHeight;
	theApp.SetFontLanguage(&lf);
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.OffsetRect(-rr.left,-rr.top);

	float fFastScanSize,fFastIncrement;
	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);


	switch(m_pData->m_TS[m_nTS].Pr30.nOutputSelect) {
	case 0:
	case 1:
		if((m_pData->m_Hist.nAmplitudeScaleType == SCALE_PERCENTAGE) || (m_pData->m_Hist.nAmplitudeScaleType == SCALE_LINEAR_DB)) {
			pDC->SetTextColor(RGB(0,0,0));
		} else {
			pDC->SetTextColor(RGB(255,0,0));
		}
		break;
	case 2:
		if(m_pData->m_Hist.nAmplitudeScaleType == SCALE_LOG) {
			pDC->SetTextColor(RGB(0,0,0));
		} else {
			pDC->SetTextColor(RGB(255,0,0));
		}
		break;
	}


	// Total Area
	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0: Buff.Format(_T("%s %.02f mm\x0b2"),TotalArea, (float)m_pData->m_Hist.nTotalNumberSamples*fFastIncrement*m_pData->m_fSlowIncrement);
		break;
	case 1: Buff.Format(_T("%s %.2f \x022\x0b2"),TotalArea, ((float)m_pData->m_Hist.nTotalNumberSamples*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f);
		break;
	}
	nX = rr.left +4;
	nY = rr.top + 4;
	pDC->TextOut(nX,nY,Buff);
	size = pDC->GetTextExtent(Buff);
	nY += size.cy;

	//if(theApp.m_LastSettings.nHistogramDropOut==0) {


	float fPercentageBelow0 = (float)m_pData->m_Hist.nTotalBelowThreshold[0] * 100.0f / (float)m_pData->m_Hist.nTotalNumberSamples;
	float fPercentageBelow1 = (float)m_pData->m_Hist.nTotalBelowThreshold[1] * 100.0f / (float)m_pData->m_Hist.nTotalNumberSamples;
	float fAverageBetween = 0.0f;

	// Reference amplitude
	if(m_pData->m_Hist.nAmpMode == HIST_ATTENUATION) {

		switch(m_pData->m_Hist.nAmplitudeScaleType) {
		case 0:
			Buff.Format(_T("%s%.01f %% %s"),Reference,m_pData->m_Hist.fRefAmp,BelowFSH);
			pDC->TextOut(nX,nY,Buff);
			nY += size.cy;


			switch(theApp.m_LastSettings.nMeasurementUnits) {
			case 0:
				Buff.Format(_T("%s%.01f %% %.02f mm\x0b2 == %%%.02f"),AreaBelowRef,-100.0 / pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[0]/-20.0f),(float)m_pData->m_Hist.nTotalBelowThreshold[0]*fFastIncrement*m_pData->m_fSlowIncrement,fPercentageBelow0);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				Buff.Format(_T("%s%.01f %% %.02f mm\x0b2 == %%%.02f"),AreaBelowRef,-100.0 / pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[1]/-20.0f),(float)m_pData->m_Hist.nTotalBelowThreshold[1]*fFastIncrement*m_pData->m_fSlowIncrement,fPercentageBelow1);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				break;
			case 1:
				Buff.Format(_T("%s%.01f %% %.02f \x022\x0b2 == %%%.02f"),AreaBelowRef,-100.0 / pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[0]/-20.0f),((float)m_pData->m_Hist.nTotalBelowThreshold[0]*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f,fPercentageBelow0);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				Buff.Format(_T("%s%.01f %% %.02f \x022\x0b2 == %%%.02f"),AreaBelowRef,-100.0 / pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[1]/-20.0f),((float)m_pData->m_Hist.nTotalBelowThreshold[1]*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f,fPercentageBelow1);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				break;
			}
			Buff.Format(_T("%s = %.02f"),SD,m_pData->m_Hist.fStd);
			pDC->TextOut(nX,nY,Buff);		nY += size.cy;
			break;
		case 1:
			Buff.Format(_T("%s%.01f dB %s"),Reference,m_pData->m_Hist.fRefAmp,BelowFSH);
			pDC->TextOut(nX,nY,Buff);		nY += size.cy;
			fAverageBetween = -20.0f * (float)log10(127.0f / (m_pData->m_Hist.fAverageBetween-128.0f));

			switch(theApp.m_LastSettings.nMeasurementUnits) {
			case 0:
				Buff.Format(_T("%s%.01f dB %.02f mm\x0b2 == %%%.02f"),AreaBelowRef,theApp.m_LastSettings.fHistogramRelativedB[0],(float)m_pData->m_Hist.nTotalBelowThreshold[0]*fFastIncrement*m_pData->m_fSlowIncrement,fPercentageBelow0);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				Buff.Format(_T("%s%.01f dB %.02f mm\x0b2 == %%%.02f"),AreaBelowRef,theApp.m_LastSettings.fHistogramRelativedB[1],(float)m_pData->m_Hist.nTotalBelowThreshold[1]*fFastIncrement*m_pData->m_fSlowIncrement,fPercentageBelow1);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				break;
			case 1:
				Buff.Format(_T("%s%.01f dB %.02f sq \x022 == %%%.02f"),AreaBelowRef,theApp.m_LastSettings.fHistogramRelativedB[0],((float)m_pData->m_Hist.nTotalBelowThreshold[0]*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f,fPercentageBelow0);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				Buff.Format(_T("%s%.01f dB %.02f sq \x022 == %%%.02f"),AreaBelowRef,theApp.m_LastSettings.fHistogramRelativedB[1],((float)m_pData->m_Hist.nTotalBelowThreshold[1]*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f,fPercentageBelow1);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				break;
			}
			Buff.Format(_T("%s = %.02f"),SD,-20.0f * log10((100 - m_pData->m_Hist.fStd) / 100.0f));
			pDC->TextOut(nX,nY,Buff); 		nY += size.cy;
			break;
		case 2:
			Buff.Format(_T("%s%.01f dB %s"),Reference,m_pData->m_Hist.fRefAmp,BelowFSH);
			pDC->TextOut(nX,nY,Buff);		nY += size.cy;
			fAverageBetween = (127.0f-(m_pData->m_Hist.fAverageBetween-128.0f)) * (float)m_pData->m_nLogRange / -127.0f;

			switch (theApp.m_LastSettings.nMeasurementUnits) {
			case 0:
				//				Buff.Format(_T("%s%.01f dB %.02f mm\x0b2 == %%%.02f"),AreaBelowRef,theApp.m_LastSettings.fHistogramRelativedB[0],(float)m_pData->m_Hist.nTotalBelowThreshold[0]*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement,fPercentageBelow0);
				Buff.Format(_T("%s%.01f dB %.02f mm\x0b2 == %%%.02f"), AreaBelowRef, theApp.m_LastSettings.fHistogramRelativedB[0], m_pData->m_Hist.fTotalAreaBelowThreshold[0], fPercentageBelow0);
				pDC->TextOut(nX, nY, Buff);		nY += size.cy;
				//				Buff.Format(_T("%s%.01f dB %.02f mm\x0b2 == %%%.02f"),AreaBelowRef,theApp.m_LastSettings.fHistogramRelativedB[1],(float)m_pData->m_Hist.nTotalBelowThreshold[1]*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement,fPercentageBelow1);
				Buff.Format(_T("%s%.01f dB %.02f mm\x0b2 == %%%.02f"), AreaBelowRef, theApp.m_LastSettings.fHistogramRelativedB[1], m_pData->m_Hist.fTotalAreaBelowThreshold[1], fPercentageBelow1);
				pDC->TextOut(nX, nY, Buff);		nY += size.cy;
				break;
			case 1:
				//				Buff.Format(_T("%s%.01f dB %.02f sq \x022 == %%%.02f"),AreaBelowRef,theApp.m_LastSettings.fHistogramRelativedB[0],((float)m_pData->m_Hist.nTotalBelowThreshold[0]*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f,fPercentageBelow0);
				Buff.Format(_T("%s%.01f dB %.02f sq \x022 == %%%.02f"), AreaBelowRef, theApp.m_LastSettings.fHistogramRelativedB[0], m_pData->m_Hist.fTotalAreaBelowThreshold[0] / 645.16f, fPercentageBelow0);
				pDC->TextOut(nX, nY, Buff);		nY += size.cy;
				//				Buff.Format(_T("%s%.01f dB %.02f sq \x022 == %%%.02f"),AreaBelowRef,theApp.m_LastSettings.fHistogramRelativedB[1],((float)m_pData->m_Hist.nTotalBelowThreshold[1]*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f,fPercentageBelow1);
				Buff.Format(_T("%s%.01f dB %.02f sq \x022 == %%%.02f"), AreaBelowRef, theApp.m_LastSettings.fHistogramRelativedB[1], m_pData->m_Hist.fTotalAreaBelowThreshold[1] / 645.16f, fPercentageBelow1);
				pDC->TextOut(nX, nY, Buff);		nY += size.cy;
				break;
			}
			fAmp = m_pData->m_Hist.fLatchAmp - 100.0f;
			Buff.Format(_T("%s = %.02f %s = %.01fdB %s = %.01fdB"),SD,m_pData->m_Hist.fStd,Max,m_pData->m_Hist.fMaxValue-fAmp,Min,m_pData->m_Hist.fMinValue-fAmp);
			pDC->TextOut(nX,nY,Buff);		nY += size.cy;

			break;
		}
	} else {	//HIST REFLECTION
		switch(m_pData->m_Hist.nAmplitudeScaleType) {
		case 0:
			Buff.Format(_T("%s%.01f %% %s"),Reference,m_pData->m_Hist.fRefAmp,BelowFSH);
			pDC->TextOut(nX,nY,Buff);		nY += size.cy;

			switch(theApp.m_LastSettings.nMeasurementUnits) {
			case 0:
				Buff.Format(_T("%s %.01f X Ref = %.02f mm\x0b2"),AreaGreaterThanRef,1.0/pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[0]/-20.0f),(float)m_pData->m_Hist.nTotalAboveThreshold[0]*fFastIncrement*m_pData->m_fSlowIncrement);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				Buff.Format(_T("%s %.01f X Ref = %.02f mm\x0b2"),AreaGreaterThanRef,1.0/pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[1]/-20.0f),(float)m_pData->m_Hist.nTotalAboveThreshold[1]*fFastIncrement*m_pData->m_fSlowIncrement);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				break;
			case 1:
				Buff.Format(_T("%s %.01f X Ref = %.02f sq \x022"),AreaGreaterThanRef,1.0/pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[0]/-20.0f),((float)m_pData->m_Hist.nTotalAboveThreshold[0]*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				Buff.Format(_T("%s %.01f X Ref = %.02f sq \x022"),AreaGreaterThanRef,1.0/pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[1]/-20.0f),((float)m_pData->m_Hist.nTotalAboveThreshold[1]*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				break;
			}

			Buff.Format(_T("%s = %.02f"),SD,m_pData->m_Hist.fStd);
			pDC->TextOut(nX,nY,Buff);		nY += size.cy;
			break;
		case 1:
			Buff.Format(_T("%s%.01f dB %s"),Reference,m_pData->m_Hist.fRefAmp,BelowFSH);
			pDC->TextOut(nX,nY,Buff);		nY += size.cy;

			switch(theApp.m_LastSettings.nMeasurementUnits) {
			case 0:
				Buff.Format(_T("%s + %.01f dB = %.02f mm\x0b2"),AreaGreaterThanRef,theApp.m_LastSettings.fHistogramRelativedB[0],(float)m_pData->m_Hist.nTotalAboveThreshold[0]*fFastIncrement*m_pData->m_fSlowIncrement);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				Buff.Format(_T("%s + %.01f dB = %.02f mm\x0b2"),AreaGreaterThanRef,theApp.m_LastSettings.fHistogramRelativedB[1],(float)m_pData->m_Hist.nTotalAboveThreshold[1]*fFastIncrement*m_pData->m_fSlowIncrement);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				break;
			case 1:
				Buff.Format(_T("%s + %.01f dB = %.02f sq \x022"),AreaGreaterThanRef,theApp.m_LastSettings.fHistogramRelativedB[0],((float)m_pData->m_Hist.nTotalAboveThreshold[0]*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				Buff.Format(_T("%s + %.01f dB = %.02f sq \x022"),AreaGreaterThanRef,theApp.m_LastSettings.fHistogramRelativedB[1],((float)m_pData->m_Hist.nTotalAboveThreshold[1]*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				break;
			}
			Buff.Format(_T("%s = %.02f"),SD,m_pData->m_Hist.fStd);
			pDC->TextOut(nX,nY,Buff);		nY += size.cy;
			break;
		case 2:
			Buff.Format(_T("%s%.01f dB %s"),Reference,m_pData->m_Hist.fRefAmp,BelowFSH);
			pDC->TextOut(nX,nY,Buff);		nY += size.cy;

			switch(theApp.m_LastSettings.nMeasurementUnits) {
			case 0:
				Buff.Format(_T("%s + %.01f dB = %.02f mm\x0b2"),AreaGreaterThanRef,theApp.m_LastSettings.fHistogramRelativedB[0],(float)m_pData->m_Hist.nTotalAboveThreshold[0]*fFastIncrement*m_pData->m_fSlowIncrement);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				Buff.Format(_T("%s + %.01f dB = %.02f mm\x0b2"),AreaGreaterThanRef,theApp.m_LastSettings.fHistogramRelativedB[1],(float)m_pData->m_Hist.nTotalAboveThreshold[1]*fFastIncrement*m_pData->m_fSlowIncrement);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				break;
			case 1:
				Buff.Format(_T("%s + %.01f dB = %.02f sq \x022"),AreaGreaterThanRef,theApp.m_LastSettings.fHistogramRelativedB[0],((float)m_pData->m_Hist.nTotalAboveThreshold[0]*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				Buff.Format(_T("%s + %.01f dB = %.02f sq \x022"),AreaGreaterThanRef,theApp.m_LastSettings.fHistogramRelativedB[1],((float)m_pData->m_Hist.nTotalAboveThreshold[1]*fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f);
				pDC->TextOut(nX,nY,Buff);		nY += size.cy;
				break;
			}

			Buff.Format(_T("%s = %.02f"),SD,m_pData->m_Hist.fStd);
			pDC->TextOut(nX,nY,Buff);		nY += size.cy;
			break;
		}
	}
	if(theApp.m_LastSettings.nHistogramMask & _HIST_VERBOSE) {
		pDC->SetTextColor(RGB(0,0,255));
		Buff.Format(L"The area between the two levels accounts for %.02f%%",fabs(fPercentageBelow1 - fPercentageBelow0));
		pDC->TextOut(nX,nY,Buff);		nY += size.cy;
		Buff.Format(L"Mean=%.01fdB Std' Dev'= %.02f",fAverageBetween,m_pData->m_Hist.fStdBetween);
		pDC->TextOut(nX,nY,Buff);		nY += size.cy;
		pDC->SetTextColor(RGB(0,0,0));
	}

	size = pDC->GetTextExtent(Buff);
	/////////////////////////////////////////////////////////////
	//Draw the threshold lines
	m_nLatchAmpY = m_rrGraticule.bottom - (int)((m_pData->m_Hist.fLatchAmp*(float)m_rrGraticule.Height())/100.0f);
	pDC->MoveTo(m_rrGraticule.left,m_nLatchAmpY);
	pDC->LineTo(m_rrGraticule.right,m_nLatchAmpY);
	FillCircle(pDC,m_rrGraticule.right+4,m_nLatchAmpY,4,RGB(128,128,128));
	pDC->TextOut(m_rrGraticule.right+8,m_nLatchAmpY-size.cy/2,Ref);

	for(int gg=0;gg<2;gg++) {
		theApp.m_Palette.m_PorosityThresholdCurve.SetPrimaryAxis(1);
		theApp.m_Palette.m_PorosityThresholdCurve.CalculateCoord(theApp.m_LastSettings.fHistogramRelativedB[gg]*-1.0f,Cp);
		fPlies = Cp.fPos[0];
		switch(m_pData->m_Hist.nAmplitudeScaleType) {
		case 0: fAmp = m_pData->m_Hist.fLatchAmp / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[gg]/-20.0f);
			if(m_pData->m_Hist.nAmpMode == HIST_ATTENUATION) {
				if(theApp.m_LastSettings.nHistogramPorosityStats==false) {
					Buff.Format(_T("%.01f"),100.0 / pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[gg]/-20.0f));
				} else {
					Buff.Format(_T("%d Ply"),(int)fPlies);
				}
			} else {
				Buff.Format(_T("%.01fX"),1.00 / pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[gg]/-20.0f));
			}
			break;
		case 1:	fAmp = m_pData->m_Hist.fLatchAmp / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[gg]/-20.0f);
			if(theApp.m_LastSettings.nHistogramPorosityStats==false) {
				Buff.Format(_T("%.01f"),theApp.m_LastSettings.fHistogramRelativedB[gg]);
			} else {
				Buff.Format(_T("%d Ply"),(int)fPlies);
			}
			break;
		case 2:
			fAmp = m_pData->m_Hist.fLatchAmp + (theApp.m_LastSettings.fHistogramRelativedB[gg]*100.0f/(float)m_pData->m_nLogRange);
			if(theApp.m_LastSettings.nHistogramPorosityStats==false) {
				Buff.Format(_T("%.01f"),theApp.m_LastSettings.fHistogramRelativedB[gg]);
			} else {
				Buff.Format(_T("%d Ply"),(int)fPlies);
			}
			break;
		}
		if(fAmp>100.0f) fAmp = 100.0f;
		if(fAmp<0.0f) fAmp = 0.0f;
		m_nLatchThresholdY[gg]= m_rrGraticule.bottom - (int)((fAmp*(float)m_rrGraticule.Height())/100.0f);
		pDC->MoveTo(m_rrGraticule.left,m_nLatchThresholdY[gg]);
		pDC->LineTo(m_rrGraticule.right,m_nLatchThresholdY[gg]);
		FillCircle(pDC,m_rrGraticule.right+4,m_nLatchThresholdY[gg],4,RGB(128,128,128));
		pDC->TextOut(m_rrGraticule.right+8,m_nLatchThresholdY[gg]-size.cy/2,Buff);
	}
	/////////////////////////////////////////////////////////////


	
	pDC->SelectObject(pOldFont);

	if (m_pListItemEdit) {
		CRect rr, rrView;
		m_staticView.GetWindowRect(&rrView);
		rr = m_pListItemEdit->m_rrWnd;
		m_pListItemEdit->m_rrWnd.top = rrView.top + (m_nLatchThresholdY[m_nToolOption-3] - rr.Height() / 2);
		m_pListItemEdit->m_rrWnd.bottom = m_pListItemEdit->m_rrWnd.top + rr.Height();
		m_pListItemEdit->SetWindowPosition(m_pListItemEdit->m_rrWnd);
		CPoint point;
		GetCursorPos(&point);
		SetCursorPos(point.x, point.y = m_pListItemEdit->m_rrWnd.CenterPoint().y);
	}
}



int CHistogramDlg::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

void CHistogramDlg::DrawTimeScale(CDC* pDC)
{
	CFont HorizontalFont;
	CFont VerticalFont;
	CSize size;
	CString Buff;
	int	nPny;
	CRect rr;
	double dStep;

	theApp.m_LastSettings.lfHistogram = theApp.m_LastSettings.lfViewGraticule;

	theApp.m_LastSettings.lfHistogram.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfHistogram);
	theApp.m_LastSettings.lfHistogram.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&theApp.m_LastSettings.lfHistogram);

	CFont* pOldFont = pDC->SelectObject(&VerticalFont);
	pDC->SetBkMode(TRANSPARENT);
	Buff = "mm\x0b2";
	size = pDC->GetTextExtent(Buff);
	m_rrGraticule = m_rrView;
	m_rrGraticule.OffsetRect(-m_rrView.left,-m_rrView.top);
	
	m_rrGraticule.left+=size.cx;
	m_rrGraticule.bottom-=size.cy;
	m_rrGraticule.right-=size.cx;
	m_rrGraticule.top+=5*size.cy;

	pDC->FillRect(&m_rrGraticule,&CBrush(theApp.m_LastSettings.rgbHistogramBkColor));


	pDC->MoveTo(m_rrGraticule.left-1,m_rrGraticule.top);
	pDC->LineTo(m_rrGraticule.left-1,m_rrGraticule.bottom+1);
	pDC->LineTo(m_rrGraticule.right,m_rrGraticule.bottom+1);

	for(dStep=0;dStep<=theApp.m_Palette.m_dThicknessRange[0];dStep+=theApp.m_Palette.m_dStepSize[0]) {
		nPny=m_rrGraticule.bottom-(int)((dStep*(double)m_rrGraticule.Height())/theApp.m_Palette.m_dThicknessRange[0]);
		pDC->MoveTo(m_rrGraticule.left+2,nPny);
		pDC->LineTo(m_rrGraticule.left+3,nPny);
		switch(theApp.m_LastSettings.nMeasurementUnits) {
		case 0:	Buff.Format(_T("%.01f"),dStep+theApp.m_Palette.m_dMinThickness[0]);
			break;
		case 1:	Buff.Format(_T("%.03f"),(dStep+theApp.m_Palette.m_dMinThickness[0]) / 25.4f);
			break;
		};
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGraticule.left-size.cy,nPny+size.cx/2,Buff);
	};

	pDC->SelectObject(&HorizontalFont);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0:
		switch(theApp.m_LastSettings.nHistogramAbsoluteCummulative) {
		case 0:
			Buff.Format(_T("%.02fmm\x0b2"),(float)m_pData->m_Hist.nMaxAmp*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement);
			break;
		case 1:
			Buff.Format(_T("%.02fmm\x0b2"),(float)m_pData->m_Hist.nTotalNumberSamples*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement);
			break;
		}
		break;
	case 1:
		switch(theApp.m_LastSettings.nHistogramAbsoluteCummulative) {
		case 0:
			Buff.Format(_T("%.03f\x022\x0b2"),((float)m_pData->m_Hist.nMaxAmp*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f);
			break;
		case 1:
			Buff.Format(_T("%.03f\x022\x0b2"),((float)m_pData->m_Hist.nTotalNumberSamples*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement) / 645.16f);
			break;
		}
		break;
	};
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.right-size.cx,m_rrGraticule.bottom,Buff);
	
	pDC->SelectObject(pOldFont);
}

void CHistogramDlg::DrawTimeHistogram(CDC* pDC)
{
	int	ii,nX,nY,nColor;
	int	nSum=0,nThresholdSum=0;
	int	nXThreshold,nYThreshold;
	CString Buff;
	CString Units;
	CFont Font;
	CSize size;
	CRect	rr;
	int nThresh0,nThresh1;

	if(m_pData->m_Hist.pnData==NULL) return;

	CPen* pen[10000];
	if(theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) {
		nThresh0 = m_pData->m_Hist.nSampleThreshold[0]-128;
		nThresh1 = m_pData->m_Hist.nPaintThreshold-128;
		if(nThresh0>nThresh1) {
			ii=nThresh0;
			nThresh0=nThresh1;
			nThresh1=ii;
		}
		MinMax(&nThresh0,0,127);
		MinMax(&nThresh1,0,127);
	}

	for(ii=0;ii<theApp.m_Palette.m_nMaxThicknessNumber[0];ii++) {
		if(theApp.m_LastSettings.bHistogramPaintView[0]!=TRUE) {
			pen[ii] = new CPen(PS_SOLID,1,theApp.m_Palette.m_rgbThickness[0][ii]);
		} else {
			if((ii<nThresh0) || (ii>nThresh1)) {
				pen[ii] = new CPen(PS_SOLID,1,theApp.m_Palette.m_rgbThickness[0][ii]);
			} else {
				pen[ii] = new CPen(PS_SOLID,1,RGB(255,0,0));
			}
		}
	}

	CPen* pOldPen = pDC->SelectObject(pen[0]);
	//Fill the interior
	if(m_rrGraticule.Height() > theApp.m_Palette.m_nMaxThicknessNumber[0]) {
		for(nY=0;nY<m_rrGraticule.Height();nY++) {
			nColor = MulDiv(nY,theApp.m_Palette.m_nMaxThicknessNumber[0]-1,m_rrGraticule.Height()-1);
			switch(theApp.m_LastSettings.nHistogramAbsoluteCummulative) {
			case 0:
				nX = m_rrGraticule.left + MulDiv(m_pData->m_Hist.pnData[nColor],m_rrGraticule.Width()-1,m_pData->m_Hist.nMaxThickness);
				break;
			case 1:
				nX = m_rrGraticule.left + MulDiv(nSum,m_rrGraticule.Width()-1,m_pData->m_Hist.nTotalNumberSamples);
				break;
			}
			pDC->SelectObject(pen[nColor]);
			pDC->MoveTo(m_rrGraticule.left,m_rrGraticule.bottom-nY);
			pDC->LineTo(nX,m_rrGraticule.bottom-nY);
		}
	} else {
		for(nColor=theApp.m_Palette.m_nMinThicknessNumber[0];nColor<theApp.m_Palette.m_nMaxThicknessNumber[0];nColor++) {
			nY = MulDiv(nColor-theApp.m_Palette.m_nMinThicknessNumber[0],m_rrGraticule.Height(),theApp.m_Palette.m_nMaxThicknessNumber[0]-theApp.m_Palette.m_nMinThicknessNumber[0]-1);
			switch(theApp.m_LastSettings.nHistogramAbsoluteCummulative) {
			case 0:
				nX = m_rrGraticule.left + MulDiv(m_pData->m_Hist.pnData[nColor],m_rrGraticule.Width()-1,m_pData->m_Hist.nMaxThickness);
				break;
			case 1:
				nX = m_rrGraticule.left + MulDiv(nSum,m_rrGraticule.Width()-1,m_pData->m_Hist.nTotalNumberSamples);
				break;
			}
			pDC->SelectObject(pen[nColor]);
			pDC->MoveTo(m_rrGraticule.left,m_rrGraticule.bottom-nY);
			pDC->LineTo(nX,m_rrGraticule.bottom-nY);
		}
	}
	pDC->SelectObject(pOldPen);

	//Draw the black line round the edge
	nSum = 0;
	pDC->MoveTo(m_rrGraticule.TopLeft());
	for(ii=255;ii>=128;ii--) {
		nY = m_rrGraticule.bottom - MulDiv(ii-128,m_rrGraticule.Height()-1,127);
		nSum+=m_pData->m_Hist.pnData[ii];
		switch(theApp.m_LastSettings.nHistogramAbsoluteCummulative) {
		case 0:
			nX = m_rrGraticule.left + MulDiv(m_pData->m_Hist.pnData[ii],m_rrGraticule.Width()-1,m_pData->m_Hist.nMaxAmp);
			break;
		case 1:
			nX = m_rrGraticule.left + MulDiv(nSum,m_rrGraticule.Width()-1,m_pData->m_Hist.nTotalNumberSamples);
			break;
		}
		pDC->LineTo(nX,nY);
		if(ii==m_pData->m_Hist.nSampleThreshold[0]) {
			nXThreshold=nX;
			nYThreshold=nY;
			nThresholdSum=nSum;
		}
	}
	pDC->MoveTo(m_rrGraticule.left,nYThreshold);
	pDC->LineTo(nXThreshold,nYThreshold);
	pDC->LineTo(nXThreshold,m_rrGraticule.bottom);

	float fTotalArea = (float)m_pData->m_Hist.nTotalNumberSamples*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement;
	float fSubArea = (float)nThresholdSum*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement;
	float fAmp;

	switch(m_pData->m_Hist.nAmplitudeScaleType) {
	case 0: fAmp = theApp.m_LastSettings.fHistogramAmpThreshold;
		Units = "%";
		break;
	case 1:
		fAmp = theApp.m_LastSettings.fHistogramAmpThreshold;
		Units = "dB";
		break;
	case 2:
		fAmp = theApp.m_LastSettings.fHistogramAmpThreshold;
		Units = "dB";
		break;
	}

	switch(theApp.m_LastSettings.nHistogramAreamode) {
	default: Buff.Format(_T("%.1fmm\x0b2 < %.01f%s"),fTotalArea-fSubArea,fAmp,Units);
		break;
	case 1: Buff.Format(_T("%.1fmm\x0b2 > %.01f%s"),fSubArea,fAmp,Units);
		break;
	}

	theApp.m_LastSettings.lfHistogram.lfEscapement=0;
	Font.CreateFontIndirect(&theApp.m_LastSettings.lfHistogram);
	CFont* pOldFont = pDC->SelectObject(&Font);
	size = pDC->GetTextExtent(Buff);
	size.cx+=4;
	switch(theApp.m_LastSettings.nHistogramAbsoluteCummulative) {
	case 0:
		nX=m_rrGraticule.CenterPoint().x-size.cx/2;
		break;
	case 1:
		nX=nXThreshold - size.cx/2;
		nYThreshold-=2;
		if(nX<m_rrGraticule.left) nX=m_rrGraticule.left;
		if((nX+size.cx)>m_rrGraticule.right) nX=m_rrGraticule.right-size.cx;
		break;
	}

	CBrush brush(GetSysColor(COLOR_INFOBK));
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(nX,nYThreshold-size.cy,nX+size.cx,nYThreshold);
	pDC->TextOut(nX+2,nYThreshold-size.cy,Buff);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldBrush);
	for(ii=0;ii<theApp.m_Palette.m_nMaxThicknessNumber[0];ii++) {
		if(pen[ii]) delete pen[ii];
	}

}

void CHistogramDlg::DrawExtScale(CDC* pDC)
{
	CFont HorizontalFont;
	CFont VerticalFont;
	CSize size;
	CString Buff;
	int	nPny;
	CRect rr;
	double dStep;
	double dGain[4] = {1.0, 2.0, 4.0, 8.0};

	theApp.m_LastSettings.lfHistogram = theApp.m_LastSettings.lfViewGraticule;

	theApp.m_LastSettings.lfHistogram.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfHistogram);
	theApp.m_LastSettings.lfHistogram.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&theApp.m_LastSettings.lfHistogram);

	CFont* pOldFont = pDC->SelectObject(&VerticalFont);
	pDC->SetBkMode(TRANSPARENT);
	Buff = "mm\x0b2";
	size = pDC->GetTextExtent(Buff);
	m_rrGraticule = m_rrView;
	m_rrGraticule.OffsetRect(-m_rrView.left,-m_rrView.top);
	
	m_rrGraticule.left+=size.cx;
	m_rrGraticule.bottom-=size.cy;
	m_rrGraticule.right-=size.cx;
	m_rrGraticule.top+=5*size.cy;

	pDC->FillRect(&m_rrGraticule,&CBrush(theApp.m_LastSettings.rgbHistogramBkColor));


	pDC->MoveTo(m_rrGraticule.left-1,m_rrGraticule.top);
	pDC->LineTo(m_rrGraticule.left-1,m_rrGraticule.bottom+1);
	pDC->LineTo(m_rrGraticule.right,m_rrGraticule.bottom+1);

	double dMaxExt,dMinExt,dStepSize,dStart;
	int nPrecision;

	switch(theApp.m_LastSettings.nExtSourceDisplayMode) {
	case 0:
		dMaxExt = 65535.0;
		dStart = dMinExt = 0.0;
		dStepSize = 10000.0;
		nPrecision = 0;
		break;
	case 1:
		dMaxExt = 32767.0;
		dMinExt = -32768.0;
		dStart = -30000;
		dStepSize = 10000.0;
		nPrecision = 0;
		break;
	case 2:
		dMaxExt = 10.0 / dGain[0];
		dStart = dMinExt = -10.0 / dGain[0];
		dStepSize = 1.0;
		nPrecision = 1;
		break;
	case 3:
		dMaxExt = 100.0;
		dStart = dMinExt = -100.0;
		dStepSize = 10.0;
		nPrecision = 1;
		break;
	}
	double dRange = dMaxExt - dMinExt;

	for(dStep=dStart;dStep<=dMaxExt;dStep+=dStepSize) {
		nPny=m_rrGraticule.bottom-(int)(((dStep-dMinExt)*(double)m_rrGraticule.Height())/dRange);
		pDC->MoveTo(m_rrGraticule.left+2,nPny);
		pDC->LineTo(m_rrGraticule.left+3,nPny);
		Buff.Format(_T("%.*f"),nPrecision,dStep);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGraticule.left-size.cy,nPny+size.cx/2,Buff);
	};

	pDC->SelectObject(&HorizontalFont);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	switch(theApp.m_LastSettings.nHistogramAbsoluteCummulative) {
	case 0:
		Buff.Format(_T("%.02fmm\x0b2"),(float)m_pData->m_Hist.nMaxAmp*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement);
		break;
	case 1:
		Buff.Format(_T("%.02fmm\x0b2"),(float)m_pData->m_Hist.nTotalNumberSamples*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement);
		break;
	}

	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.right-size.cx,m_rrGraticule.bottom,Buff);
	
	pDC->SelectObject(pOldFont);
}


void CHistogramDlg::DrawExtHistogram(CDC* pDC)
{
	int	ii,nX,nY,nColor;
	int	nSum=0,nThresholdSum=0;
	int	nXThreshold,nYThreshold;
	CString Buff;
	CString Units;
	CFont Font;
	CSize size;
	CRect	rr;
	int nThresh[2];
	float fAmp;

	if(m_pData->m_Hist.pnData==NULL) return;

	CPen penColor0(PS_SOLID,1,theApp.m_LastSettings.rgbHistogramColor[0]);
	CPen penColor1(PS_SOLID,1,theApp.m_LastSettings.rgbHistogramColor[1]);

	for(int gg=0;gg<2;gg++) {
		switch(m_pData->m_Hist.nAmplitudeScaleType) {
		case 0:
		case 1:	fAmp = m_pData->m_Hist.fLatchAmp / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[gg]/-20.0f);
			break;
		case 2:	fAmp = m_pData->m_Hist.fLatchAmp + (theApp.m_LastSettings.fHistogramRelativedB[gg]*100.0f/(float)m_pData->m_nLogRange);
			break;
		}
		if(fAmp>100.0f) fAmp = 100.0f;
		if(fAmp<0.0f) fAmp = 0.0f;
		nThresh[gg]= (int)((fAmp*(float)m_rrGraticule.Height())/100.0f);
	}

	for(ii=0;ii<65536;ii++) {
		if(m_penHist[ii] == NULL)
			m_penHist[ii] = new CPen(PS_SOLID,1,theApp.m_Palette.m_rgb16[m_nPaletteNumber][ii]);
	}

	int nPtr0,nPtr1,nPtr,nMax;
	CPen* pOldPen = pDC->SelectObject(m_penHist[0]);
	//Fill the interior
	for(nY=0;nY<m_rrGraticule.Height()-1;nY++) {
		nPtr0 = MulDiv(nY,65535,m_rrGraticule.Height()-1);
		nPtr1 = MulDiv(nY+1,65535,m_rrGraticule.Height()-1);
		nMax = 0;
		nColor = nPtr0;
		for(nPtr=nPtr0;nPtr<nPtr1;nPtr++) {
			if(nMax < m_pData->m_Hist.pnData[nPtr]) {
				nMax = m_pData->m_Hist.pnData[nPtr];
				nColor = nPtr;
			}
		}
		switch(theApp.m_LastSettings.nHistogramAbsoluteCummulative) {
		case 0:
			nX = m_rrGraticule.left + MulDiv(m_pData->m_Hist.pnData[nColor],m_rrGraticule.Width()-1,m_pData->m_Hist.nMaxThickness);
			break;
		case 1:
			nX = m_rrGraticule.left + MulDiv(nSum,m_rrGraticule.Width()-1,m_pData->m_Hist.nTotalNumberSamples);
			break;
		}
	
		pDC->SelectObject(m_penHist[nColor]);
		/*
		if(m_pData->m_Hist.nAmpMode == HIST_ATTENUATION) {
			if(theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) {
				if((nY>=nThresh[1]) && (nY<nThresh[0])) {
					pDC->SelectObject(penColor0);
				}
			}
			if(theApp.m_LastSettings.bHistogramPaintView[1]==TRUE) {
				if(nY<nThresh[1]) {
					pDC->SelectObject(penColor1);
				}
			}
		} else {
			if(theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) {
				if((nY>=nThresh[0]) && (nY<nThresh[1])) {
					pDC->SelectObject(penColor0);
				}
			}
			if(theApp.m_LastSettings.bHistogramPaintView[1]==TRUE) {
				if(nY>=nThresh[1]) {
					pDC->SelectObject(penColor1);
				}
			}
		}
		*/
		pDC->MoveTo(m_rrGraticule.left,m_rrGraticule.bottom-nY);
		pDC->LineTo(nX,m_rrGraticule.bottom-nY);
	}
	pDC->SelectObject(pOldPen);
goto finish_label;

	//Draw the black line round the edge
	nSum = 0;
	pDC->MoveTo(m_rrGraticule.TopLeft());
	for(ii=65535;ii>=0;ii--) {
		nY = m_rrGraticule.bottom - MulDiv(ii,m_rrGraticule.Height()-1,65535);
		nSum+=m_pData->m_Hist.pnData[ii];
		switch(theApp.m_LastSettings.nHistogramAbsoluteCummulative) {
		case 0:
			nX = m_rrGraticule.left + MulDiv(m_pData->m_Hist.pnData[ii],m_rrGraticule.Width()-1,m_pData->m_Hist.nMaxAmp);
			break;
		case 1:
			nX = m_rrGraticule.left + MulDiv(nSum,m_rrGraticule.Width()-1,m_pData->m_Hist.nTotalNumberSamples);
			break;
		}
		pDC->LineTo(nX,nY);
		if(ii==m_pData->m_Hist.nSampleThreshold[0]) {
			nXThreshold=nX;
			nYThreshold=nY;
			nThresholdSum=nSum;
		}
	}

finish_label:
;
}


int CHistogramDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolsToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndToolsToolBar.LoadToolBar(IDR_HISTOGRAM_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	GetClientRect(rect);

	if (!m_wndRebar.Create(this,RBS_AUTOSIZE,CBRS_ALIGN_TOP | AFX_IDW_DIALOGBAR)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,500,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolsToolBar);

//	CReBarCtrl& ReBarCtrl = m_wndRebar.GetReBarCtrl();
//	CToolTipCtrl *pToolTipCtrl = ReBarCtrl.GetToolTips();
//	int nDelay = pToolTipCtrl->GetDelayTime(20);
//	pToolTipCtrl->SetDelayTime(50);


	return 0;
}

BOOL CHistogramDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText,strCtrlKey;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

   strCtrlKey.LoadString(IDS_Ctrl_Key);
   switch(nID) {
   case ID_BUTTON_TRACK_PEAK: strTipText.LoadString(IDS_Track_Peak);
	   strTipText += (L" Hold " + strCtrlKey);
	   break;
   case ID_BUTTON_TRACK_MEAN: strTipText.LoadString(IDS_Track_Mean);
	   strTipText += (L" Hold " + strCtrlKey);
	   break;
   case ID_BUTTON_PERCENTAGE: strTipText.LoadString(IDS_Percentage);
	   strTipText += (L" " + strCtrlKey);
	   break;
   case ID_BUTTON_LINEAR_DB: strTipText.LoadString(IDS_dB_Linear);
	   break;
   case ID_BUTTON_LOG: strTipText.LoadString(IDS_dB_Log);
	   break;
   case ID_BUTTON_POROSITY: strTipText.LoadString(IDS_Porosity);
	   break;
   case ID_BUTTON_DISPLAY_HISTOGRAM: strTipText.LoadString(IDS_Histogram);
	   break;
   case ID_BUTTON_DISPLAY_DROPOUT: strTipText.LoadString(IDS_Drop_Out);
	   break;
//   case ID_BUTTON_VERBOSE: strTipText.LoadString(IDS_Verbose);
//	   break;
   }
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


void CHistogramDlg::OnButtonTrackPeak() 
{
	theApp.m_LastSettings.nHistogramMask &= ~(_HIST_PEAK_TRACK + _HIST_MEAN_TRACK + _HIST_TROUGH_TRACK);

	m_bTrackAtten = FALSE;
	m_bTrackPeak = TRUE;
	m_bTrackMean = FALSE;
	theApp.m_LastSettings.nHistogramMask |= _HIST_PEAK_TRACK;

	SetToolBarCheckedState();
	m_pData->BuildHistogramData(m_nImageType,m_nImageNumber,m_rrSamples);
	Invalidate(false);
	CViewSheet* pViewSheet = (CViewSheet*)GetParent();
	pViewSheet->InvalidateDueToHistogramChange();
}

void CHistogramDlg::OnButtonTrackMean() 
{
	theApp.m_LastSettings.nHistogramMask &= ~(_HIST_PEAK_TRACK + _HIST_MEAN_TRACK + _HIST_TROUGH_TRACK);

	m_bTrackAtten = FALSE;
	m_bTrackPeak = FALSE;
	m_bTrackMean = TRUE;
	theApp.m_LastSettings.nHistogramMask |= _HIST_MEAN_TRACK;

	SetToolBarCheckedState();
	m_pData->BuildHistogramData(m_nImageType,m_nImageNumber,m_rrSamples);
	Invalidate(false);
	CViewSheet* pViewSheet = (CViewSheet*)GetParent();
	pViewSheet->InvalidateDueToHistogramChange();

}

void CHistogramDlg::OnButtonTrackMaxAttenuation() 
{
	theApp.m_LastSettings.nHistogramMask &= ~(_HIST_PEAK_TRACK + _HIST_MEAN_TRACK + _HIST_TROUGH_TRACK);

	m_bTrackAtten = TRUE;
	m_bTrackPeak = FALSE;
	m_bTrackMean = FALSE;
	theApp.m_LastSettings.nHistogramMask |= _HIST_TROUGH_TRACK;

	SetToolBarCheckedState();
	m_pData->BuildHistogramData(m_nImageType,m_nImageNumber,m_rrSamples);
	Invalidate(false);
	InvalidateViewSheet();
}


void CHistogramDlg::FillCircle(CDC *pDC, int nX, int nY, int nR, COLORREF rgb)
{

	pDC->Ellipse(nX-nR,nY-nR,nX+nR,nY+nR);

}

void CHistogramDlg::DrawTimeStatistics(CDC *pDC)
{
	if(m_pData->m_Hist.pnData==NULL) return;

	int	nX,nY;
	CString Buff;
	CFont Font, HorizontalFont, VerticalFont;
	CRect rr;
	CSize size;
	CString Peak,Mean,Std,TotalArea,Ref,Reference,BelowFSH,AreaBetween;
	Peak.LoadString(IDS_Peak);
	Mean.LoadString(IDS_Mean);
	Std.LoadString(IDS_Std);
	TotalArea.LoadString(IDS_Total_Area);
	Ref.LoadString(IDS_Ref);
	Reference.LoadString(IDS_The_Reference_Thickness_Is);
	BelowFSH.LoadString(IDS_Below_FSH);
	AreaBetween.LoadString(IDS_Area_Between);

	LOGFONT lf;
	ZeroMemory(&lf,sizeof lf);
	lf.lfHeight = theApp.m_LastSettings.lfHistogram.lfHeight;
	theApp.SetFontLanguage(&lf);
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);

	theApp.m_LastSettings.lfHistogram.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfHistogram);
	theApp.m_LastSettings.lfHistogram.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&theApp.m_LastSettings.lfHistogram);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.OffsetRect(-rr.left,-rr.top);

	// Total Area
	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0:	Buff.Format(_T("%s %.02f mm\x0b2"),TotalArea, (float)m_pData->m_Hist.nTotalNumberSamples*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement);
		break;
	case 1:	Buff.Format(_T("%s %.02f \x022\x0b2"),TotalArea, (float)m_pData->m_Hist.nTotalNumberSamples*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement  / 645.16f);
		break;
	};
	nX = rr.left +4;
	nY = rr.top + 4;
	pDC->TextOut(nX,nY,Buff);
	size = pDC->GetTextExtent(Buff);


	// Reference amplitude
	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0:	Buff.Format(_T("%s %.02f mm"),Reference,m_pData->m_Hist.fLatchThickness);
		break;
	case 1:	Buff.Format(_T("%s %.03f \x022"),Reference,m_pData->m_Hist.fLatchThickness / 25.4f);
		break;
	};
	pDC->TextOut(nX,nY+size.cy,Buff);

	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0:
		Buff.Format(_T("%s %.02f & %.02f mm = %.1fmm"),AreaBetween,m_pData->m_Hist.fLatchThickness,m_pData->m_Hist.fLatchThickness - theApp.m_LastSettings.fHistogramRelativeThickness[0],(float)m_pData->m_Hist.nTotalBelowThreshold[0]*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement);
		pDC->TextOut(nX,nY+size.cy*2,Buff);
		Buff.Format(_T("%s %.02f & %.02f mm = %.1fmm"),AreaBetween,m_pData->m_Hist.fLatchThickness - theApp.m_LastSettings.fHistogramRelativeThickness[0],m_pData->m_Hist.fLatchThickness - theApp.m_LastSettings.fHistogramRelativeThickness[1],(float)m_pData->m_Hist.nTotalBelowThreshold[1]*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement);
		pDC->TextOut(nX,nY+size.cy*3,Buff);
		break;
	case 1:
		Buff.Format(_T("%s %.03f & %.03f \x022 = %.1f\x022"),AreaBetween,m_pData->m_Hist.fLatchThickness/25.4f,(m_pData->m_Hist.fLatchThickness - theApp.m_LastSettings.fHistogramRelativeThickness[0])/25.4f,((float)m_pData->m_Hist.nTotalBelowThreshold[0]*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement)/645.16f);
		pDC->TextOut(nX,nY+size.cy*2,Buff);
		Buff.Format(_T("%s %.03f & %.03f \x022 = %.1f\x022"),AreaBetween,(m_pData->m_Hist.fLatchThickness - theApp.m_LastSettings.fHistogramRelativeThickness[0])/25.4f,(m_pData->m_Hist.fLatchThickness - theApp.m_LastSettings.fHistogramRelativeThickness[1])/25.4f,((float)m_pData->m_Hist.nTotalBelowThreshold[1]*m_pData->m_fFastIncrement*m_pData->m_fSlowIncrement)/645.16f);
		pDC->TextOut(nX,nY+size.cy*3,Buff);
		break;
	}		
	size = pDC->GetTextExtent(Buff);
	/////////////////////////////////////////////////////////////
	//Draw the threshold lines
	pDC->SelectObject(&VerticalFont);
	int gg=0;
	m_nLatchTimeY[gg] = m_rrGraticule.bottom - (int)(((m_pData->m_Hist.fLatchThickness - (float)theApp.m_Palette.m_dMinThickness[0])*(float)m_rrGraticule.Height())/(float)theApp.m_Palette.m_dThicknessRange[0]);
	pDC->MoveTo(m_rrGraticule.left,m_nLatchTimeY[gg]);
	pDC->LineTo(m_rrGraticule.right,m_nLatchTimeY[gg]);
	FillCircle(pDC,m_rrGraticule.right+4,m_nLatchTimeY[gg],4,RGB(128,128,128));
	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0:	Buff.Format(_T("%.02f"),m_pData->m_Hist.fLatchThickness);
		break;
	case 1:	Buff.Format(_T("%.03f"),m_pData->m_Hist.fLatchThickness/25.4f);
		break;
	};
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(m_rrGraticule.right+8,m_nLatchTimeY[gg]+size.cx/2,Buff);

	for(gg=1;gg<3;gg++) {
		m_nLatchTimeY[gg] = m_rrGraticule.bottom - (int)((((m_pData->m_Hist.fLatchThickness - theApp.m_LastSettings.fHistogramRelativeThickness[gg-1]) - (float)theApp.m_Palette.m_dMinThickness[0])*(float)m_rrGraticule.Height())/(float)theApp.m_Palette.m_dThicknessRange[0]);
		pDC->MoveTo(m_rrGraticule.left,m_nLatchTimeY[gg]);
		pDC->LineTo(m_rrGraticule.right,m_nLatchTimeY[gg]);
		FillCircle(pDC,m_rrGraticule.right+4,m_nLatchTimeY[gg],4,RGB(128,128,128));
		switch(theApp.m_LastSettings.nMeasurementUnits) {
		case 0:	Buff.Format(_T("%.02f"),m_pData->m_Hist.fLatchThickness - theApp.m_LastSettings.fHistogramRelativeThickness[gg-1]);
			break;
		case 1:	Buff.Format(_T("%.03f"),(m_pData->m_Hist.fLatchThickness - theApp.m_LastSettings.fHistogramRelativeThickness[gg-1]) / 25.4f);
			break;
		}
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGraticule.right+8,m_nLatchTimeY[gg]+size.cx/2,Buff);
	}

	
	pDC->SelectObject(pOldFont);


}


void CHistogramDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMFCColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbHistogramBkColor=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CHistogramDlg::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_TRACK_PEAK))>=0)
		m_bTrackPeak==TRUE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_TRACK_MEAN))>=0)
		m_bTrackMean==TRUE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_TRACK_MAX_ATTENUATION))>=0)
		m_bTrackAtten==TRUE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_PERCENTAGE))>=0)
		m_pData->m_Hist.nAmplitudeScaleType==SCALE_PERCENTAGE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_LINEAR_DB))>=0)
		m_pData->m_Hist.nAmplitudeScaleType==SCALE_LINEAR_DB ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_LOG))>=0)
		m_pData->m_Hist.nAmplitudeScaleType==SCALE_LOG ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_POROSITY))>=0)
		theApp.m_LastSettings.nHistogramPorosityStats==1 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_DISPLAY_HISTOGRAM))>=0)
		theApp.m_LastSettings.nHistogramDropOut==0 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_DISPLAY_DROPOUT))>=0)
		theApp.m_LastSettings.nHistogramDropOut==1 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_VERBOSE))>=0)
		theApp.m_LastSettings.nHistogramMask & _HIST_VERBOSE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_PAINT_12))>=0)
		theApp.m_LastSettings.bHistogramPaintView[0] == 1 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_PAINT_2))>=0)
		theApp.m_LastSettings.bHistogramPaintView[1] == 1 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	
}

void CHistogramDlg::OnButtonLog() 
{
	m_pData->m_Hist.nAmplitudeScaleType = SCALE_LOG;
	SetToolBarCheckedState();
	m_pData->ReBuildHistogramData();
	Invalidate(false);

}

void CHistogramDlg::OnButtonLinearDb() 
{
	m_pData->m_Hist.nAmplitudeScaleType = SCALE_LINEAR_DB;
	SetToolBarCheckedState();
	m_pData->ReBuildHistogramData();
	Invalidate(false);
}

void CHistogramDlg::OnButtonPercentage() 
{
	m_pData->m_Hist.nAmplitudeScaleType = SCALE_PERCENTAGE;
	SetToolBarCheckedState();
	m_pData->ReBuildHistogramData();
	Invalidate(false);
}

void CHistogramDlg::OnButtonPorosity() 
{
	theApp.m_LastSettings.nHistogramPorosityStats==1 ? theApp.m_LastSettings.nHistogramPorosityStats=false : theApp.m_LastSettings.nHistogramPorosityStats=true;
	SetToolBarCheckedState();
}


void CHistogramDlg::OnButtonDisplayHistogram() 
{
	theApp.m_LastSettings.nHistogramDropOut = 0;
	SetToolBarCheckedState();
}

void CHistogramDlg::OnButtonDisplayDropout() 
{
	theApp.m_LastSettings.nHistogramDropOut = 1;
	SetToolBarCheckedState();
}

void CHistogramDlg::OnButtonVerbose() 
{
	theApp.m_LastSettings.nHistogramMask ^= _HIST_VERBOSE;
	SetToolBarCheckedState();
	Invalidate(false);
	
}

void CHistogramDlg::OnButtonPaint2() 
{
	theApp.m_LastSettings.bHistogramPaintView[1]==1 ? theApp.m_LastSettings.bHistogramPaintView[1] = 0 : theApp.m_LastSettings.bHistogramPaintView[1] = 1;
	SetToolBarCheckedState();
	Invalidate(false);
	InvalidateViewSheet();
}

void CHistogramDlg::OnButtonPaint12() 
{
	theApp.m_LastSettings.bHistogramPaintView[0]==1 ? theApp.m_LastSettings.bHistogramPaintView[0] = 0 : theApp.m_LastSettings.bHistogramPaintView[0] = 1;
	SetToolBarCheckedState();
	Invalidate(false);
	InvalidateViewSheet();
}

void CHistogramDlg::InvalidateViewSheet()
{
	CViewSheet* pViewSheet = (CViewSheet*)GetParent();
	pViewSheet->InvalidateDueToHistogramChange();

}

BOOL CHistogramDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	theApp.m_LastSettings.fHistogramRelativedB[m_nLastThreshold] += ((float)(zDelta/120) * 0.1f);
	m_pData->ReBuildHistogramData();
	Invalidate(FALSE);
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

LRESULT CHistogramDlg::ItemChanged(WPARAM wp, LPARAM lp)
{

	switch ((int)wp) {
	case WM_ITEMCHANGED:
		Invalidate(false);
		break;
	case WM_ITEMFINISHED:
		break;
	}

	return true;
}
