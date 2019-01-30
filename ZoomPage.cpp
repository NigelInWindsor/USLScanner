// ZoomPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ZoomPage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZoomPage property page

IMPLEMENT_DYNCREATE(CZoomPage, CPropertyPage)

CZoomPage::CZoomPage(CWnd* pParent,int nParentType) : CPropertyPage(CZoomPage::IDD)
{
	//{{AFX_DATA_INIT(CZoomPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_nParentType = nParentType;
	
	if(m_nParentType == 0) {
		CViewSheet* pSheet = (CViewSheet*)m_pParent;
		m_pData = &pSheet->m_Data;
		m_pnZoomFactor = &pSheet->m_nZoomFactor;
		m_prrSamples = &pSheet->m_rrSamples;
	} else {
		CVolumeViewDlg* pDlg = (CVolumeViewDlg*)m_pParent;
		m_pData = pDlg->m_pData;
		m_pnZoomFactor = &pDlg->m_nZoomFactor;
		m_prrSamples = pDlg->m_prrSamples;
	}

	m_pArrayView = NULL;
	m_nXpixelOdd = NULL;
	m_nXpixelEven = NULL;

	m_bInitialize=FALSE;
	m_bPalette=TRUE;
	m_bRenderView=FALSE;
	
	m_bLBDown=false;

	m_nArea=0;
	m_nAreaL=1;
	m_nXArea=1;
	m_nYArea=1;
}

CZoomPage::~CZoomPage()
{
	SAFE_DELETE(m_pArrayView);
	SAFE_DELETE(m_nXpixelOdd);
	SAFE_DELETE(m_nXpixelEven);
}

void CZoomPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomPage)
	DDX_Control(pDX, IDC_STATIC_MAGNIFICATION, m_staticMagnification);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_SLIDER_ZOOM, m_sliderZoom);
	DDX_Control(pDX, IDC_EDIT_ZOOM, m_editZoom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoomPage, CPropertyPage)
	//{{AFX_MSG_MAP(CZoomPage)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_ZOOM, OnChangeEditZoom)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_ZOOM, OnReleasedcaptureSliderZoom)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomPage message handlers

BOOL CZoomPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_sliderZoom.SetRange(0,990,FALSE);

	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CZoomPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CZoomPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL) return;

	float fFastScanSize,fFastIncrement,fSlowScanSize;
	CRect rrView;
	CRect rrClient;
	CPoint pt;

	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);
	fSlowScanSize = m_pData->m_fSlowScanSize;

	if (m_staticView.GetSafeHwnd()) {
		m_staticView.GetWindowRect(&rrView);
		GetWindowRect(rrClient);

		rrView.right = rrClient.right - 8;
		rrView.bottom = rrClient.bottom - 8;
		pt = rrView.CenterPoint();

		if ((fFastScanSize / fSlowScanSize) >= ((float)rrView.Width() / (float)rrView.Height())) {
			rrView.top = pt.y - (int)(((float)rrView.Width() * fSlowScanSize) / fFastScanSize) / 2;
			rrView.bottom = (pt.y - rrView.top) + pt.y;
		}
		else {
			rrView.left = pt.x - (int)(((float)rrView.Height() * fFastScanSize) / fSlowScanSize) / 2;
			rrView.right = (pt.x - rrView.left) + pt.x;
		}

		ScreenToClient(&rrView);
		m_staticView.SetWindowPos(NULL, rrView.left, rrView.top, rrView.Width(), rrView.Height(), NULL);
	}
}

void CZoomPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff,Language;
	CRect rr;
	float fFastScanSize,fFastIncrement;
	int nWidth;

	if(m_nParentType == 0) {
		CViewSheet* pSheet = (CViewSheet*)m_pParent;
		CViewPage* pPage = (CViewPage*)pSheet->GetActivePage();
		nWidth = pPage->m_rrView.Width();
	} else {
		CVolumeViewDlg* pDlg = (CVolumeViewDlg*)m_pParent;
		nWidth = pDlg->m_rrCscan.Width();
	}


	m_sliderZoom.SetPos(MAX_ZOOM_FACTOR - *m_pnZoomFactor);
	Buff.Format(_T("%d%%"),*m_pnZoomFactor/10);
	m_editZoom.SetWindowText(Buff);

	if(theApp.m_LastSettings.nScreenWidthPixels<=0) {
		CDC* pDC = GetDC();
		theApp.m_LastSettings.nScreenWidthPixels = pDC->GetDeviceCaps(HORZRES);
		ReleaseDC(pDC);
	}

	float fViewWidth = (float)MulDiv(nWidth , theApp.m_LastSettings.nScreenHorizontalSizeMM , theApp.m_LastSettings.nScreenWidthPixels);
	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);
	float fDisplayedWidth = (float)m_prrSamples->Width() * fFastIncrement;

	Language.LoadString(IDS_Magnification);
	Buff.Format(_T("%s %.02f"),Language,fViewWidth/fDisplayedWidth);
	m_staticMagnification.SetWindowText(Buff);


}

void CZoomPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SLIDER_ZOOM:
		*m_pnZoomFactor = MAX_ZOOM_FACTOR - pSlider->GetPos()&0xffff;
		MinMax(m_pnZoomFactor, MIN_ZOOM_FACTOR, MAX_ZOOM_FACTOR);
		InvalidateParent();
		UpdateAllControls();
		Invalidate(FALSE);
		break;
	};
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CZoomPage::OnChangeEditZoom() 
{
	CString Buff;
	int nTemp;
	
	m_editZoom.GetWindowText(Buff);
	nTemp = _ttoi(Buff)*10;
	MinMax(&nTemp,MIN_ZOOM_FACTOR, MAX_ZOOM_FACTOR);
	if(nTemp - *m_pnZoomFactor) {
		*m_pnZoomFactor = nTemp;
		m_sliderZoom.SetPos(MAX_ZOOM_FACTOR - *m_pnZoomFactor);
		InvalidateParent();
	}

}

void CZoomPage::OnReleasedcaptureSliderZoom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CZoomPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rrView;

	m_staticView.GetWindowRect(&rrView);
	ScreenToClient(&rrView);
	rrView.DeflateRect(1,1);

	InitializePalette();
	InitializeViewVariables(&dc,&rrView);
	RenderView(&dc);

	PresentRenderdView(&dc);
	DrawActiveArea(&dc);

	m_bInitialize=FALSE;
	m_bPalette=FALSE;
	m_bRenderView=FALSE;

	// Do not call CPropertyPage::OnPaint() for painting messages
}

BOOL CZoomPage::OnSetActive() 
{
	
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}


void CZoomPage::RenderView(CPaintDC* pDC)
{
	static int nOldImageType,nOldImageNumber;
	CRect rrSamples(0,0,m_pData->m_nSamplesLine-1,m_pData->m_nNumberLines-1);
	int nImageType,nImageNumber;

	if(m_nParentType == 0) {
		CViewSheet* pSheet = (CViewSheet*)m_pParent;
		CViewPage* pPage = (CViewPage*)pSheet->GetActivePage();
		nImageType = pPage->m_nImageType;
		nImageNumber = pPage->m_nImageNumber;
	} else {
		CVolumeViewDlg* pDlg = (CVolumeViewDlg*)m_pParent;
		nImageType = pDlg->m_nImageType;
		nImageNumber = pDlg->m_nCscanNumber;
	}

	if(nOldImageType - nImageType) m_bRenderView = TRUE;
	if(nOldImageNumber - nImageNumber) m_bRenderView = TRUE;

	if(m_bRenderView != TRUE) return;

	switch(nImageType) {
	case AMP_IMAGE: m_pData->RenderAmpImage(pDC,m_rrView,&rrSamples,nImageNumber,m_pArrayView,(char*)m_cColor,0);
		break;
	case TIME_IMAGE: m_pData->RenderTimeImage(pDC,m_rrView,&rrSamples,nImageNumber,m_pArrayView);
		break;
	}

	nOldImageType = nImageType;
	nOldImageNumber = nImageNumber;

}

void CZoomPage::PresentRenderdView(CPaintDC* pDC)
{
	CDC  dcMem;
	CBitmap bmp;

	bmp.CreateCompatibleBitmap(pDC,m_rrView.Width(),m_rrView.Height());
	bmp.SetBitmapBits((DWORD)(m_rrView.Width()*m_rrView.Height()*m_nBytesPixel),m_pArrayView);

	dcMem.CreateCompatibleDC(pDC);

	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	pDC->BitBlt(m_rrView.left,m_rrView.top,m_rrView.Width(),m_rrView.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);


}

void CZoomPage::InitializePalette()
{
	int	ii;
	int nPaletteNumber;

	if(m_bPalette!=TRUE) return;
	if(m_nParentType == 0) {
		CViewSheet* pSheet = (CViewSheet*)m_pParent;
		CViewPage* pPage = (CViewPage*)pSheet->GetActivePage();
		nPaletteNumber = pPage->m_nPaletteNumber;
	} else {
		CVolumeViewDlg* pDlg = (CVolumeViewDlg*)m_pParent;
		nPaletteNumber = pDlg->m_nCScanPalette;
	}


	for(ii=0;ii<256;ii++) {
		m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[nPaletteNumber][ii]>>16)&0xff);
		m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[nPaletteNumber][ii]>>8)&0xff);
		m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[nPaletteNumber][ii]>>0)&0xff);
	}
	m_bPalette = TRUE;
}

void CZoomPage::InitializeViewVariables(CPaintDC* pDC,CRect* prr)
{
	int	nXs0,nXs1,xx;

	if((prr->Width() - m_rrView.Width()) || (prr->Height() - m_rrView.Height()) || (m_bInitialize==TRUE)){
		m_rrView = prr;

		int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
		m_nBytesPixel = nBitsPixel/8;

		if(m_pArrayView) delete m_pArrayView;
		m_pArrayView = new char[(m_rrView.Width()+2) * (m_rrView.Height()+2) * m_nBytesPixel];

		m_nMinViewLine=0;
		m_nMaxViewLine=m_pData->m_nNumberLines-1;
		m_bRenderView=TRUE;

		nXs0=0;
		nXs1=m_pData->m_nSamplesLine-1;

		if(m_nXpixelOdd) delete m_nXpixelOdd;
		if(m_nXpixelEven) delete m_nXpixelEven;
		int nWidth = m_rrView.Width()+2;
		m_nXpixelOdd = new int[m_rrView.Width()+10];
		m_nXpixelEven = new int[m_rrView.Width()+10];
		for(xx=0;xx<=(m_rrView.Width()+1);xx++) {
			m_nXpixelOdd[xx]=MulDiv(xx,(nXs1-nXs0),m_rrView.Width())+nXs0;
			m_nXpixelEven[xx]=MulDiv(xx,(nXs1-nXs0),m_rrView.Width())+nXs0;
		};

		CPolyCoord lineEdge;
		CCoord Cp;
		for(int ii=0;ii<8;ii++) {
			Cp.Side0.fX = (float)((ii*nWidth)/7);
			Cp.Side0.fY = (float)theApp.m_LastSettings.nImageEdge[ii];
			lineEdge.Add(Cp);
		}
		int nEdge;
		for(xx=1;xx<nWidth;xx++) {
			lineEdge.CalculateCoord((float)xx,Cp);
			nEdge=(int)Cp.Side0.fY;
			m_nXpixelEven[xx] += nEdge;
			if(nWidth>m_pData->m_nSamplesLine) {
				if((m_nXpixelEven[xx]<m_nXpixelEven[xx-1])) {
					m_nXpixelEven[xx]=m_nXpixelEven[xx-1];
				}
			}
		}

		m_bRenderView=TRUE;
		m_bInitialize = FALSE;

	}
}

void CZoomPage::DrawActiveArea(CPaintDC *pDC)
{
	CPen PenActive(PS_SOLID,1,theApp.m_LastSettings.rgbRulerLine);
	CPen PenGrey(PS_SOLID,1,RGB(120,120,120));
	CPen* pOldPen = pDC->SelectObject(&PenGrey);
	CRect rr;
	int nX,nY;

	int nXStep = MulDiv(m_prrSamples->Width(),m_rrView.Width(),m_pData->m_nSamplesLine-1);
	int nYStep = MulDiv(m_prrSamples->Height(),m_rrView.Height(),m_pData->m_nNumberLines-1);

	for(nX=nXStep,m_nXArea=1;nX<m_rrView.Width();nX+=nXStep,m_nXArea++) {
		if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
			pDC->MoveTo(nX-m_rrView.right,m_rrView.top);
			pDC->LineTo(nX-m_rrView.right,m_rrView.bottom);
		}else{
			pDC->MoveTo(nX+m_rrView.left,m_rrView.top);
			pDC->LineTo(nX+m_rrView.left,m_rrView.bottom);
		}
	}
	for(nY=nYStep,m_nYArea=1;nY<m_rrView.Height();nY+=nYStep,m_nYArea++) {
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
			pDC->MoveTo(m_rrView.left,m_rrView.top+nY);
			pDC->LineTo(m_rrView.right,m_rrView.top+nY);
		} else {
			pDC->MoveTo(m_rrView.left,m_rrView.bottom-nY);
			pDC->LineTo(m_rrView.right,m_rrView.bottom-nY);
		}
	}
	m_nAreaL = m_nXArea * m_nYArea;
	if(m_nArea>= m_nAreaL) m_nArea = m_nAreaL-1;

	
	
	pDC->SelectObject(&PenActive);
	if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
		rr.left = MulDiv(m_prrSamples->left,m_rrView.Width(),m_pData->m_nSamplesLine-1) - m_rrView.right;
		rr.right = MulDiv(m_prrSamples->right,m_rrView.Width(),m_pData->m_nSamplesLine-1) - m_rrView.right;
	}else{
		rr.left = MulDiv(m_prrSamples->left,m_rrView.Width(),m_pData->m_nSamplesLine-1) + m_rrView.left;
		rr.right = MulDiv(m_prrSamples->right,m_rrView.Width(),m_pData->m_nSamplesLine-1) + m_rrView.left;
	}

	if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
		rr.top = m_rrView.top + MulDiv(m_prrSamples->top,m_rrView.Height(),m_pData->m_nNumberLines-1);
		rr.bottom = m_rrView.top + MulDiv(m_prrSamples->bottom,m_rrView.Height(),m_pData->m_nNumberLines-1);
	} else {
		rr.top = m_rrView.bottom - MulDiv(m_prrSamples->top,m_rrView.Height(),m_pData->m_nNumberLines-1);
		rr.bottom = m_rrView.bottom - MulDiv(m_prrSamples->bottom,m_rrView.Height(),m_pData->m_nNumberLines-1);
	}

	pDC->MoveTo(rr.TopLeft());
	pDC->LineTo(rr.right,rr.top);
	pDC->LineTo(rr.BottomRight());
	pDC->LineTo(rr.left,rr.bottom);
	pDC->LineTo(rr.TopLeft());

	pDC->SelectObject(pOldPen);

}

void CZoomPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int nXSample,nYSample;

	m_bLBDown = true;
	m_ptClient = point;

	if(m_rrView.PtInRect(point)) {
		int nXStep = m_prrSamples->Width();
		int nYStep = m_prrSamples->Height();

		if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
			nXSample = MulDiv(m_rrView.right - point.x,m_pData->m_nSamplesLine,m_rrView.Width());
			m_prrSamples->left = (nXSample/nXStep) * nXStep;
		}else{
			nXSample = MulDiv(point.x - m_rrView.left,m_pData->m_nSamplesLine,m_rrView.Width());
			m_prrSamples->left = (nXSample/nXStep) * nXStep;
		}

		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
			nYSample = MulDiv(point.y - m_rrView.top,m_pData->m_nNumberLines,m_rrView.Height());
			m_prrSamples->top = (nYSample/nYStep) * nYStep;
		} else {
			nYSample = MulDiv(m_rrView.bottom - point.y,m_pData->m_nNumberLines,m_rrView.Height());
			m_prrSamples->top = (nYSample/nYStep) * nYStep;
		}

		InvalidateParent();
		m_nArea = (nYSample/nYStep) * m_nXArea + (nXSample/nXStep);
	}


	CPropertyPage::OnLButtonDown(nFlags, point);
}

void CZoomPage::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bLBDown != true) return;

	if(m_ptClient != point) {
		if(m_rrView.PtInRect(point)) {
			int nXStep = m_prrSamples->Width();
			int nYStep = m_prrSamples->Height();

			if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
				m_prrSamples->left = (m_prrSamples->Width() / 2) - MulDiv(m_rrView.right - point.x,m_pData->m_nSamplesLine,m_rrView.Width());
			}else{
				m_prrSamples->left = MulDiv(point.x - m_rrView.left,m_pData->m_nSamplesLine,m_rrView.Width()) - (m_prrSamples->Width() / 2);
			}
			MinMax((int*)&m_prrSamples->left,0,m_pData->m_nSamplesLine - 1 - nXStep);
			m_prrSamples->right = m_prrSamples->left + nXStep;

			if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
				m_prrSamples->top = MulDiv(point.y - m_rrView.top,m_pData->m_nNumberLines,m_rrView.Height()) - (m_prrSamples->Height() / 2);
			} else {
				m_prrSamples->top = MulDiv(m_rrView.bottom - point.y,m_pData->m_nNumberLines,m_rrView.Height()) - (m_prrSamples->Height() / 2);
			}
			MinMax((int*)&m_prrSamples->top,0,m_pData->m_nNumberLines - 1 - nYStep);
			m_prrSamples->bottom = m_prrSamples->top + nYStep;

			InvalidateParent();
			m_ptClient = point;

		}
	}
	CPropertyPage::OnMouseMove(nFlags, point);
}

void CZoomPage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBDown = false;

	CPropertyPage::OnLButtonUp(nFlags, point);
}

void CZoomPage::OnButtonPrevious() 
{
	if(m_nArea) m_nArea--;
	int x = m_nArea % m_nXArea;
	int y = m_nArea / m_nXArea;

	m_prrSamples->left = m_prrSamples->Width() * x;
	m_prrSamples->top = m_prrSamples->Height() * y;
	InvalidateParent();

}

void CZoomPage::OnButtonNext() 
{
	if(m_nArea < (m_nAreaL-1)) m_nArea++;
	int x = m_nArea % m_nXArea;
	int y = m_nArea / m_nXArea;

	m_prrSamples->left = m_prrSamples->Width() * x;
	m_prrSamples->top = m_prrSamples->Height() * y;
	InvalidateParent();
}


void CZoomPage::InvalidateParent()
{
	if(m_nParentType == 0) {
		CViewSheet* pSheet = (CViewSheet*)m_pParent;
		pSheet->InvalidateChild();
		pSheet->SetFocus();
	} else {
		CVolumeViewDlg* pDlg = (CVolumeViewDlg*)m_pParent;
		pDlg->m_bRenderView=TRUE;
		pDlg->Invalidate(FALSE);
		pDlg->SetFocus();
	}

}



int CZoomPage::MinMax(int *pV, int nMin, int nMax)
{
	if(*pV < nMin) *pV=nMin;
	if(*pV > nMax) *pV=nMax;
	return *pV;
}
