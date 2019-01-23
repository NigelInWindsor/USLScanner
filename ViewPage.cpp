// ViewPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ViewPage.h"
#include "ViewSheet.h"
#include "MainFrm.h"
#include "USLPrint.h"
#include "ProgressDlg.h"
#include "winuser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewPage property page

IMPLEMENT_DYNCREATE(CViewPage, CResizablePage)

CViewPage::CViewPage(CWnd* pParent,CWnd** pDlg,int nImageType,int nImageNumber,CData* pData,int nTS,int nGate,int nPageOrDlg,int nPageIndex)
 : CResizablePage(CViewPage::IDD)
{
	//{{AFX_DATA_INIT(CViewPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CViewSheet* pViewSheet = (CViewSheet*)pParent;
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_nPageOrDlg = nPageOrDlg;
	m_nPageIndex = nPageIndex;

	
	m_nImageType = nImageType;
	m_nImageNumber = nImageNumber;
	m_pData = pData;
	m_nTS = nTS;
	m_nPaletteNumber = nGate;
	m_nPaletteNumber = 0;
	m_pArrayView=NULL;
	m_nXpixelOdd=NULL;
	m_nXpixelEven=NULL;
	m_rrView.SetRectEmpty();
	m_prrSamples = &pViewSheet->m_rrSamples;
	m_bInitialize=FALSE;
	m_bHistogramPalette= false;
	m_bPalette=TRUE;
	m_bRenderView=FALSE;
	m_bCrossVisible=FALSE;
	m_bLButtonDown = false;
	m_bRButtonDown = false;
	m_nToolOption=0;
	m_nPolyPtr=0;
	m_nMoveCurrentCADElement = 0;
	m_bThreadEnabled = false;
	m_bThreadFinished = false;
	m_bRejectWM_WHEEL = false;


	m_hcursorValue1 = AfxGetApp()->LoadCursor(IDC_1_1_CURSOR);
	m_hcursorValue2 = AfxGetApp()->LoadCursor(IDC_2_2_CURSOR);
	m_hcursorValue3 = AfxGetApp()->LoadCursor(IDC_3_3_CURSOR);
	m_hcursorValue4 = AfxGetApp()->LoadCursor(IDC_4_4_CURSOR);
	m_hcursorValue5 = AfxGetApp()->LoadCursor(IDC_5_5_CURSOR);
	m_hcursorValue6 = AfxGetApp()->LoadCursor(IDC_6_6_CURSOR);
	m_hcursorValue7 = AfxGetApp()->LoadCursor(IDC_7_7_CURSOR);
	m_hcursorValue8 = AfxGetApp()->LoadCursor(IDC_8_8_CURSOR);
	m_hcursorValue9 = AfxGetApp()->LoadCursor(IDC_9_9_CURSOR);
	m_hcursorValue10 = AfxGetApp()->LoadCursor(IDC_10_10_CURSOR);


	m_pAnnotateDlg=NULL;


}

CViewPage::~CViewPage()
{
	SAFE_DELETE( m_pArrayView );
	SAFE_DELETE( m_nXpixelOdd );
	SAFE_DELETE( m_nXpixelEven );
}

void CViewPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewPage)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewPage, CResizablePage)
	//{{AFX_MSG_MAP(CViewPage)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_VIEW_PALETTE_1, OnViewPalette1)
	ON_COMMAND(ID_VIEW_PALETTE_2, OnViewPalette2)
	ON_COMMAND(ID_VIEW_PALETTE_3, OnViewPalette3)
	ON_COMMAND(ID_VIEW_PALETTE_4, OnViewPalette4)
	ON_COMMAND(ID_VIEW_PALETTE_5, OnViewPalette5)
	ON_COMMAND(ID_VIEW_PALETTE_6, OnViewPalette6)
	ON_COMMAND(ID_VIEW_PALETTE_7, OnViewPalette7)
	ON_COMMAND(ID_VIEW_PALETTE_8, OnViewPalette8)
	ON_COMMAND(ID_VIEW_ANNOTATE, OnViewAnnotate)
	ON_COMMAND(ID_VIEW_GOTOLEFT, OnViewGotoleft)
	ON_COMMAND(ID_VIEW_GOTORIGHT, OnViewGotoright)
	ON_COMMAND(ID_VIEW_GOTOBOTH, OnViewGotoboth)
	ON_COMMAND(ID_VIEW_COPYAREATOENVELOPE, OnViewCopyareatoenvelope)
	ON_COMMAND(ID_VIEW_DELETEANNOTATION, OnViewDeleteannotation)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_GOTOTAUGHTCOORD, OnViewGototaughtcoord)
	ON_COMMAND(ID_VIEW_UPDATEBRANECOORDINATE, OnViewUpdatebranecoordinate)
	ON_COMMAND(ID_VIEW_ANNOTATEHISTOGRAMDATA, OnViewAnnotatehistogramdata)
	ON_COMMAND(ID_POLYGON_NEW, OnPolygonNew)
	ON_COMMAND(ID_POLYGON_CLEAR, OnPolygonClear)
	ON_COMMAND(ID_POLYGON_CLEARALL, OnPolygonClearall)
	ON_COMMAND(ID_VIEW_DISPLAYAREA, OnViewDisplayarea)
	ON_COMMAND(ID_ZOOM_ENLARGE, OnZoomEnlarge)
	ON_COMMAND(ID_ZOOM_FULLSCAN, OnZoomFullscan)
	ON_COMMAND(ID_VIEW_3DLOOKATPOINT, OnView3dlookatpoint)
	ON_COMMAND(ID_POLYGON_AREA, OnPolygonArea)
	ON_COMMAND(ID_POLYGON_LENGTH, OnPolygonLength)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_CAD_SNAPTONEARESTNODE, OnCadSnaptonearestnode)
	ON_COMMAND(ID_CAD_SETHEIGHTWIDTH, OnCadSetheightwidth)
	ON_COMMAND(ID_CAD_SETWIDTHHEIGHT, OnCadSetwidthheight)
	ON_COMMAND(ID_CAD_PROPERTIES, OnCadProperties)
	ON_COMMAND(ID_VIEW_OPENINNEWWINDOW, OnViewOpeninnewwindow)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_GOTOWITH_LIFTOFF, OnViewGotowithLiftoff)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewPage message handlers
void CViewPage::OnClose() 
{
	SAFE_DELETE( m_pArrayView );
	SAFE_DELETE( m_nXpixelOdd );
	SAFE_DELETE( m_nXpixelEven );
	
	CResizablePage::OnClose();
	DestroyWindow();

}

void CViewPage::OnDestroy() 
{
	CResizablePage::OnDestroy();
	
	*m_pDlg	= NULL;
	delete this;
	
}

int CViewPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CResizablePage::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

BOOL CViewPage::OnInitDialog()
{
	CResizablePage::OnInitDialog();
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	AddAnchor(IDC_STATIC_VIEW, CSize(0,0), CSize(100,100));
	ShowScrollBar(SB_BOTH,TRUE);

	if(m_nPageOrDlg == 1) {
		CRect rectPos,rectSize;

		CPropertySheet *pSheet = (CPropertySheet*)m_pParent;
		CPropertyPage *pPage = pSheet->GetActivePage();
		pPage->GetWindowRect(&rectPos);

		m_pParent->GetClientRect(rectSize);

		rectPos.left += (m_nPageIndex *24);
		rectPos.right = rectPos.left + rectSize.Width();

		rectPos.top += (m_nPageIndex *24);
		rectPos.bottom = rectPos.top + rectSize.Height() - 16;
		theApp.ConfineToScreen(&rectPos);
		SetWindowPos( &wndTop , rectPos.left, rectPos.top, rectPos.Width(), rectPos.Height(), NULL);

		SetWindowText(pViewSheet->m_Name[m_nPageIndex]);
	}


	InitializePalette(SCREEN);
	SetZoomSamples();
	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);

	

	SetTimer(1,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	InitializePalette(SCREEN);
	SetZoomSamples();
	InitializeViewVariables(&dc,&rr);
	RenderView(&dc);

	PresentRenderdView(&dc);
	DrawCAD(&dc,SCREEN);

	DrawGraticule(&dc);
	DrawRuler(&dc);
	DrawValue(&dc);
	DrawPolygon(&dc);
	DrawHistogram(&dc);
	DrawInclusions(&dc);
	DrawZoomVariable(&dc);
	DrawAnnotation(&dc);
	DrawTessellation(&dc);
	DrawProfileTaughtLines(&dc);
	DrawProfileBraneLines(&dc);
	DrawProfileEnvelopeLines(&dc);
	DrawProfileWaypoints(&dc);
	DrawDiagDirChange(&dc);
	DrawDiagZeroCrossings(&dc);
	DrawCrossSectionLine(&dc);


	m_pointCrossHair.x=-1;
	m_pointCrossHair.y=-1;
	m_bCrossVisible=FALSE;

	m_bInitialize=FALSE;
	m_bPalette=FALSE;
	m_bRenderView=FALSE;


}

void CViewPage::OnSize(UINT nType, int cx, int cy)
{
	CResizablePage::OnSize(nType, cx, cy);

	if(GetSafeHwnd()) {
		CADElementSizeArrows();
		Invalidate(FALSE);
	}
}


void CViewPage::InitializeViewVariables(CPaintDC* pDC,CRect* prr)
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

		nXs1=m_prrSamples->right;
		nXs0=m_prrSamples->left;

		if(m_nXpixelOdd) delete m_nXpixelOdd;
		if(m_nXpixelEven) delete m_nXpixelEven;
		int nWidth = m_rrView.Width()+2;
		m_nXpixelOdd = new int[m_rrView.Width()+10];
		m_nXpixelEven = new int[m_rrView.Width()+10];
		for(xx=0;xx<=(m_rrView.Width()+1);xx++) {
			m_nXpixelOdd[xx]=RoundDownMulDiv(xx,(nXs1-nXs0),m_rrView.Width())+nXs0;
			m_nXpixelEven[xx]=RoundDownMulDiv(xx,(nXs1-nXs0),m_rrView.Width())+nXs0;
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

	}
}


void CViewPage::RenderView(CPaintDC* pDC)
{
	if(m_bRenderView != TRUE) return;
	CViewSheet* pParent = (CViewSheet*)m_pParent;

	switch(pParent->m_nRectPolarDisplayMode) {
	case 0:
		switch(m_nImageType) {
		case AMP_IMAGE:	m_pData->RenderAmpImage(pDC,m_rrView,m_prrSamples,m_nImageNumber,m_pArrayView,(char*)m_cColor,pParent->m_nScanLineDirection);
			RenderDropOut(pDC);
			break;
		case TIME_IMAGE: m_pData->RenderTimeImage(pDC,m_rrView,m_prrSamples,m_nImageNumber,m_pArrayView);
			break;
		case PHASE_IMAGE: m_pData->RenderPhaseImage(pDC,m_rrView,m_prrSamples,m_nImageNumber,m_pArrayView);
			break;
		case FILTER_IMAGE: m_pData->RenderFilterImage(pDC,m_rrView,m_prrSamples,m_nImageNumber,m_pArrayView,(char*)m_cColor,pParent->m_nScanLineDirection);
			break;
		case EXT_IMAGE: m_pData->RenderExtImage(pDC,m_rrView,m_prrSamples,m_nImageNumber,m_pArrayView,(char*)m_cColor);
			break;
		case PHASE_OVERLAY_IMAGE:	m_pData->RenderOverlayPhaseImage(pDC,m_rrView,m_prrSamples,theApp.m_LastSettings.nPhaseOverlayImageNumber,m_pArrayView,(char*)m_cColor);
			break;
		}
		break;
	case 1:
		switch(m_nImageType) {
		case AMP_IMAGE: m_pData->RenderAmpImagePolar(pDC,m_rrView,m_prrSamples,m_nImageNumber,m_pArrayView,(char*)m_cColor);
			break;
		case TIME_IMAGE: m_pData->RenderTimeImage(pDC,m_rrView,m_prrSamples,m_nImageNumber,m_pArrayView);
			break;
		case PHASE_IMAGE: m_pData->RenderPhaseImage(pDC,m_rrView,m_prrSamples,m_nImageNumber,m_pArrayView);
			break;
		}
		break;
	}
	theApp.m_p3DData = m_pData;
	theApp.m_n3DImageNumber = m_nImageNumber;
	theApp.m_n3DPaletteNumber = m_nPaletteNumber;
	theApp.m_n3DImageType = m_nImageType;
	theApp.m_b3DModified = TRUE;
}


void CViewPage::RenderDropOut(CPaintDC* pDC)
{
	CPoint ptSample,ptClient;
	CRect rrArea,rrSample;
	int xx,yy,nPtr,nValue,nTemp;

	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	if(pViewSheet->m_nToolOption != TOOL_HISTOGRAM) return;
	if(theApp.m_LastSettings.nHistogramDropOut != 1) return;



	rrArea.left		= m_rrRuler.left - m_rrView.left;
	rrArea.right	= m_rrRuler.right - m_rrView.left;
	rrArea.top		= m_rrRuler.top - m_rrView.top;
	rrArea.bottom	= m_rrRuler.bottom - m_rrView.top;

	if(rrArea.left>rrArea.right) {
		nTemp = rrArea.right;
		rrArea.right = rrArea.left;
		rrArea.left = nTemp;
	}
	if(rrArea.top>rrArea.bottom) {
		nTemp = rrArea.bottom;
		rrArea.bottom = rrArea.top;
		rrArea.top = nTemp;
	}

	ClientToSample(m_rrRuler.TopLeft(),&ptSample);
	rrSample.TopLeft() = ptSample;
	ClientToSample(m_rrRuler.BottomRight(),&ptSample);
	rrSample.BottomRight() = ptSample;

	MinMax(&rrArea.left,	0,m_rrView.Width()-1);
	MinMax(&rrArea.right,	0,m_rrView.Width()-1);
	MinMax(&rrArea.top,		0,m_rrView.Height()-1);
	MinMax(&rrArea.bottom,	0,m_rrView.Height()-1);

	float fPercentageBelow1 = (float)m_pData->m_Hist.nTotalBelowThreshold[1] * 100.0f / (float)m_pData->m_Hist.nTotalNumberSamples;

	int red = GetRValue(theApp.m_LastSettings.rgbDropOut);
	int green = GetGValue(theApp.m_LastSettings.rgbDropOut);
	int blue = GetBValue(theApp.m_LastSettings.rgbDropOut);
	COLORREF rgbOK(RGB(blue,green,red));

	COLORREF *rgb = (COLORREF *)m_pArrayView;

	for(yy=rrArea.top;yy<=rrArea.bottom;yy++) {
		for(xx=rrArea.left;xx<=rrArea.right;xx++) {
			nPtr = xx + yy * m_rrView.Width();

			ptClient.x = xx + m_rrView.left; 
			ptClient.y = yy + m_rrView.top;
			
			ClientToSample(ptClient,&ptSample);

			nValue = m_pData->GetFilteredAmp(ptSample.x,ptSample.y,m_nImageNumber);

			if(nValue <= m_pData->m_Hist.nSampleThreshold[1]) {
				if(fPercentageBelow1 >= theApp.m_LastSettings.fDropOutMaxPercentage) {
					rgb[nPtr] = RGB(0,0,255);
				} else {
					rgb[nPtr] = rgbOK;
				}
			}
		}
	}

}


void CViewPage::PresentRenderdView(CPaintDC* pDC)
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

void CViewPage::InitializePalette(int nDevice)
{
	int	ii;

	if(m_bPalette!=TRUE) return;

	if(nDevice==SCREEN) {
		for(ii=0;ii<127;ii++) {
			m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][256-ii]>>16)&0xff);
			m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][256-ii]>>8)&0xff);
			m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][256-ii]>>0)&0xff);
		}
		for(ii=128;ii<256;ii++) { //Fix for -ve threshold gate
			m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>16)&0xff);
			m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>8)&0xff);
			m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>0)&0xff);
		}
	}
	if(nDevice==PRINTER) {
		for(ii=0;ii<127;ii++) {
			if(theApp.m_LastSettings.bRedBlueReverse==FALSE) {
				m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][256-ii]>>16)&0xff);
				m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][256-ii]>>8)&0xff);
				m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][256-ii]>>0)&0xff);
			} else {
				m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][256-ii]>>0)&0xff);
				m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][256-ii]>>8)&0xff);
				m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][256-ii]>>16)&0xff);
			}
		}
		for(ii=128;ii<256;ii++) { //Fix for -ve threshold gate
			if(theApp.m_LastSettings.bRedBlueReverse==FALSE) {
				m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>16)&0xff);
				m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>8)&0xff);
				m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>0)&0xff);
			} else {
				m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>0)&0xff);
				m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>8)&0xff);
				m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>16)&0xff);
			}
		}
	}

	int nThresh0 = m_pData->m_Hist.nSampleThreshold[0];
	int nThresh1 = m_pData->m_Hist.nSampleThreshold[1];
	if(nThresh0>nThresh1) {
		ii=nThresh0;
		nThresh0=nThresh1;
		nThresh1=ii;
	}

/*
	float fAmp;
	for(int gg=0;gg<2;gg++) {
		switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nPaletteNumber]) {
		case 0:
		case 1:	fAmp = m_pData->m_Hist.fLatchAmp / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[gg]/-20.0f);
			break;
		case 2:	fAmp = m_pData->m_Hist.fLatchAmp + (theApp.m_LastSettings.fHistogramRelativedB[gg]*100.0f/(float)theApp.m_LastSettings.nLogGraticuleScale);
			break;
		}
		if(fAmp>100.0f) fAmp = 100.0f;
		if(fAmp<0.0f) fAmp = 0.0f;
		if(gg==0) {
			nThresh1= (int)((fAmp*(float)127)/100.0f) + 128;
		} else {
			nThresh0= (int)((fAmp*(float)127)/100.0f) + 128;
		}
	}
*/


	MinMax(&nThresh0,128,255);
	MinMax(&nThresh1,128,255);
	if((m_nToolOption==TOOL_HISTOGRAM) || (m_bHistogramPalette == true)) {
		if(m_pData->m_Hist.nAmpMode == HIST_ATTENUATION) {
			if(theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) {
				for(ii=nThresh0;ii<nThresh1;ii++) {
					if((theApp.m_LastSettings.bRedBlueReverse==TRUE) && (nDevice == PRINTER)) {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>0)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>16)&0xff);
					} else {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>16)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>0)&0xff);
					}
				}
			}
			if(theApp.m_LastSettings.bHistogramPaintView[1]==TRUE) {
				for(ii=128;ii<nThresh0;ii++) {
					if((theApp.m_LastSettings.bRedBlueReverse==TRUE) && (nDevice == PRINTER)) {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>0)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>16)&0xff);
					} else {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>16)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>0)&0xff);
					}
				}
			}
		} else {
			if(theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) {
				for(ii=nThresh0;ii<nThresh1;ii++) {
					if((theApp.m_LastSettings.bRedBlueReverse==TRUE) && (nDevice == PRINTER)) {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>0)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>16)&0xff);
					} else {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>16)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>0)&0xff);
					}
				}
			}
			if(theApp.m_LastSettings.bHistogramPaintView[1]==TRUE) {
				for(ii=nThresh1;ii<256;ii++) {
					if((theApp.m_LastSettings.bRedBlueReverse==TRUE) && (nDevice == PRINTER)) {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>0)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>16)&0xff);
					} else {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>16)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>0)&0xff);
					}
				}
			}
		}
	}
	m_bHistogramPalette = false;
}
/*
void CViewPage::RenderAmpImage(CPaintDC* pDC)
{
	int	yy,xx,nOffset;
	int	xs0=m_prrSamples->left;
	int	xs1=m_prrSamples->right;
	int	ys0=m_prrSamples->top;
	int ys1=m_prrSamples->bottom;
	int	nLine,s0,s1,c[2],color,ss,nVFilter,ff;
	int src,source[2];
	char* pBuffer;


	unsigned char* pData0 = m_pData->m_cAmpBuffer[m_nImageNumber];
	int nArrayLength=m_pData->m_nSamplesLine * m_pData->m_nNumberLines;

	for(yy=0;yy<m_rrView.Height();yy++) {
		if(theApp.m_LastSettings.nImageOrientation==0) {
			nOffset=yy*m_rrView.Width();
		} else {
			nOffset=(m_rrView.Height()-1-yy)*m_rrView.Width();
		};
		nLine=RoundDownMulDiv(yy,ys1-ys0,m_rrView.Height())+ys0;
		nVFilter=1;
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:
			break;
		case 1:nLine = nLine & 0xfffe;
			break;
		case 2:nLine = (nLine & 0xfffe) + 1;
			if(nLine>=m_pData->m_nNumberLines) nLine = m_pData->m_nNumberLines-1;
			break;
		case 3: nVFilter=2;
			break;
		}
		if((nLine>=m_nMinViewLine) && (nLine<m_nMaxViewLine)) {
			source[0] = nLine * m_pData->m_nSamplesLine;
			source[1] = (nLine + 1) * m_pData->m_nSamplesLine;
			if(source[1]>nArrayLength) source[1]=source[0];

			pBuffer=&m_pArrayView[nOffset * m_nBytesPixel];
			for(xx=0;xx<=m_rrView.Width();xx++) {
				if(nLine&1) {
					s0=m_nXpixelEven[xx];
					s1=m_nXpixelEven[xx+1];
				} else {
					s0=m_nXpixelOdd[xx];
					s1=m_nXpixelOdd[xx+1];
				}

				(theApp.m_LastSettings.nAmpPriority==1)?(c[0]=0):(c[0]=0xff);
				c[1]=c[0];
				for(ff=0;ff<nVFilter;ff++) {
					src=source[ff];
					if((s1-s0)>0) {
						for(ss=s0;ss<s1;ss++) {
							if((ss+src)<nArrayLength) {
								if(theApp.m_LastSettings.nAmpPriority==1) {
									if(pData0[ss+src]>c[ff]) c[ff]=pData0[ss+src];
								} else {
									if(pData0[ss+src]<c[ff]) c[ff]=pData0[ss+src];
								}
							} else {
								c[ff]=128;
							}
						};
					} else {
						if((s0+src)<nArrayLength) {
							c[ff]=pData0[s0+src];
						} else {
							c[ff]=128;
						};
					};
				}
				if(nVFilter==2) {
					color = ((c[0]+c[1])/2) & 0xff;
				} else {
					color = c[0]& 0xff;
				}
				*pBuffer++=m_cColor[color][0];
				*pBuffer++=m_cColor[color][1];
				*pBuffer++=m_cColor[color][2];
				if(m_nBytesPixel>3) pBuffer++;
			};
		};
	};

}

void CViewPage::RenderTimeImage(CPaintDC* pDC)
{
	int	yy,xx,nOffset;
	int	xs0=m_prrSamples->left;
	int	xs1=m_prrSamples->right;
	int	ys0=m_prrSamples->top;
	int ys1=m_prrSamples->bottom;
	int	nLine,s0,s1,c[2],nColor,ss,nVFilter,ff;
	int src,source[2];
	char* pBuffer;


	unsigned int* pData0 = m_pData->m_nTimeBuffer[m_nImageNumber];
	int nArrayLength=m_pData->m_nSamplesLine * m_pData->m_nNumberLines;

	for(yy=0;yy<m_rrView.Height();yy++) {
		if(theApp.m_LastSettings.nImageOrientation==0) {
			nOffset=yy*m_rrView.Width();
		} else {
			nOffset=(m_rrView.Height()-1-yy)*m_rrView.Width();
		};
		nLine=RoundDownMulDiv(yy,ys1-ys0,m_rrView.Height())+ys0;
		nVFilter=1;
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:
			break;
		case 1:nLine = nLine & 0xfffe;
			break;
		case 2:nLine = (nLine & 0xfffe) + 1;
			if(nLine>=m_pData->m_nNumberLines) nLine = m_pData->m_nNumberLines-1;
			break;
		case 3: nVFilter=2;
			break;
		}
		if((nLine>=m_nMinViewLine) && (nLine<m_nMaxViewLine)) {
			source[0] = nLine * m_pData->m_nSamplesLine;
			source[1] = (nLine + 1) * m_pData->m_nSamplesLine;
			if(source[1]>nArrayLength) source[1]=source[0];

			pBuffer=&m_pArrayView[nOffset * m_nBytesPixel];
			for(xx=0;xx<=m_rrView.Width();xx++) {
				if(nLine&1) {
					s0=m_nXpixelEven[xx];
					s1=m_nXpixelEven[xx+1];
				} else {
					s0=m_nXpixelOdd[xx];
					s1=m_nXpixelOdd[xx+1];
				}

				(theApp.m_LastSettings.nTimePriority==1)?(c[0]=0):(c[0]=1000000);
				c[1]=c[0];
				for(ff=0;ff<nVFilter;ff++) {
					src=source[ff];
					if((s1-s0)>0) {
						for(ss=s0;ss<s1;ss++) {
							if((ss+src)<nArrayLength) {
								if(theApp.m_LastSettings.nTimePriority==1) {
									if(pData0[ss+src]>c[ff]) c[ff]=pData0[ss+src];
								} else {
									if(pData0[ss+src]<c[ff]) c[ff]=pData0[ss+src];
								}
							} else {
								c[ff]=0;
							}
						};
					} else {
						if((s0+src)<nArrayLength) {
							c[ff]=pData0[s0+src];
						} else {
							c[ff]=0;
						};
					};
				}
				if(nVFilter==2) {
					nColor = (c[0]+c[1])/2;
				} else {
					nColor = c[0];
				}
				nColor=(nColor*theApp.m_LastSettings.nMaterialVelocity)/20000;
				if(nColor<0) nColor=1;
				if(nColor>=(theApp.m_Palette.m_nMaxThicknessNumber[0]-1)) nColor=theApp.m_Palette.m_nMaxThicknessNumber[0]-3;

				*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>0)&0xff);
				if(m_nBytesPixel>3) pBuffer++;
			};
		};
	};

}
*/
void CViewPage::SetZoomSamples()
{
	CViewSheet* pParent = (CViewSheet*)m_pParent;
	int	nWidth,nHeight,nSamples,nLines;
	float	dXSize,dYSize,fFastScanSize,fFastIncrement;
	float dScreenRatio = 1.0f;
	float dScanRatio = 1.0f;
	SCROLLINFO	info;
	CRect rr;
	m_staticView.GetWindowRect(&rr);

	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);

	if(rr.Height()>0) {
		if(pParent->m_nScanLineDirection==HORIZONTAL) {
			dScreenRatio=(float)rr.Width()/(float)rr.Height();
			dXSize=(fFastScanSize*(float)pParent->m_nZoomFactor)/1000.0f;
			dYSize=(m_pData->m_fSlowScanSize*(float)pParent->m_nZoomFactor)/1000.0f;
			dScanRatio=dXSize/dYSize;
			dYSize=dXSize / dScreenRatio;
			nWidth = (int)(dXSize / fFastIncrement);
			nHeight = (int)(dYSize / m_pData->m_fSlowIncrement);
			m_prrSamples->right=m_prrSamples->left + nWidth;
			m_prrSamples->bottom=m_prrSamples->top + nHeight;
			if(m_prrSamples->bottom<m_prrSamples->top) {
				ASSERT(TRUE);
			};

			if(m_prrSamples->right>(m_pData->m_nSamplesLine-1)) {
				m_prrSamples->right = m_pData->m_nSamplesLine-1;
				m_prrSamples->left = m_prrSamples->right - nWidth;
			};
			if(m_prrSamples->left<0) m_prrSamples->left=0;
			if(m_prrSamples->bottom>(m_pData->m_nNumberLines-1)) {
				m_prrSamples->bottom = m_pData->m_nNumberLines-1;
				m_prrSamples->top = m_prrSamples->bottom - nHeight;
			};

			MinMax(&m_prrSamples->top,0,100000);
			MinMax(&m_prrSamples->bottom,0,100000);
			MinMax(&m_prrSamples->left,0,100000);
			MinMax(&m_prrSamples->right,0,100000);

			SetScrollRange(SB_HORZ,0,m_pData->m_nSamplesLine-1,FALSE);
			SetScrollPos(SB_HORZ,m_prrSamples->left,FALSE);
			GetScrollInfo(SB_HORZ,&info,SIF_ALL);
			info.nPage = m_prrSamples->Width();
			SetScrollInfo(SB_HORZ,&info,TRUE);

			SetScrollRange(SB_VERT,0,m_pData->m_nNumberLines-1,FALSE);
			GetScrollInfo(SB_VERT,&info,SIF_ALL);
			info.nPage = m_prrSamples->Height();
			SetScrollInfo(SB_VERT,&info,TRUE);
			if( (theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2) ){
				SetScrollPos(SB_VERT,m_prrSamples->top,FALSE);
			} else {
				SetScrollPos(SB_VERT,m_pData->m_nNumberLines-1-(m_prrSamples->top+m_prrSamples->Height()),FALSE);
			};
			((CViewSheet*)m_pParent)->RefreshViewOptionsSheet();
		} else {
			dScreenRatio=(float)rr.Width()/(float)rr.Height();
			dXSize=(m_pData->m_fSlowScanSize*(float)pParent->m_nZoomFactor)/1000.0f;
			dYSize=(fFastScanSize*(float)pParent->m_nZoomFactor)/1000.0f;
			dScanRatio=dXSize/dYSize;
			dXSize=dYSize * dScreenRatio;
			nLines = (int)(dXSize / m_pData->m_fSlowIncrement);
			nSamples = (int)(dYSize / fFastIncrement);
			m_prrSamples->right=m_prrSamples->left + nSamples;
			m_prrSamples->bottom=m_prrSamples->top + nLines;

			if(m_prrSamples->bottom<m_prrSamples->top) {
				ASSERT(TRUE);
			};

			if(m_prrSamples->right>(m_pData->m_nSamplesLine-1)) {
				m_prrSamples->right = m_pData->m_nSamplesLine-1;
				m_prrSamples->left = m_prrSamples->right - nSamples;
			};
			if(m_prrSamples->left<0) m_prrSamples->left=0;
			if(m_prrSamples->bottom>(m_pData->m_nNumberLines-1)) {
				m_prrSamples->bottom = m_pData->m_nNumberLines-1;
				m_prrSamples->top = m_prrSamples->bottom - nLines;
			};
			MinMax(&m_prrSamples->top,0,100000);
			MinMax(&m_prrSamples->bottom,0,100000);
			MinMax(&m_prrSamples->left,0,100000);
			MinMax(&m_prrSamples->right,0,100000);

			SetScrollRange(SB_HORZ,0,m_pData->m_nNumberLines-1,FALSE);
			SetScrollPos(SB_HORZ,m_prrSamples->top,FALSE);
			GetScrollInfo(SB_HORZ,&info,SIF_ALL);
			info.nPage = m_prrSamples->Height();
			SetScrollInfo(SB_HORZ,&info,TRUE);

			SetScrollRange(SB_VERT,0,m_pData->m_nSamplesLine-1,FALSE);
			GetScrollInfo(SB_VERT,&info,SIF_ALL);
			info.nPage = m_prrSamples->Width();
			SetScrollInfo(SB_VERT,&info,TRUE);
			if( (theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2) ){
				SetScrollPos(SB_VERT,m_prrSamples->left,FALSE);
			} else {
				SetScrollPos(SB_VERT,m_pData->m_nSamplesLine-1-(m_prrSamples->left+m_prrSamples->Width()),FALSE);
			};
			((CViewSheet*)m_pParent)->RefreshViewOptionsSheet();
		}
	};
	CADElementSizeArrows();


}

int CViewPage::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

long CViewPage::MinMax(long *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

BOOL CViewPage::OnSetActive()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_bRenderView=TRUE;

	pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
	if(pSheet->m_pViewUltrasonicsDlg!=NULL) {
		pSheet->m_pViewUltrasonicsDlg->m_nTS = m_nTS;
		pSheet->m_pViewUltrasonicsDlg->m_nImageType = m_nImageType;
		pSheet->m_pViewUltrasonicsDlg->m_nImageNumber = m_nImageNumber;
		pSheet->m_pViewUltrasonicsDlg->CalculateGateNumber();
		pSheet->m_pViewUltrasonicsDlg->UpdateAllControls();
	}

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

	return CPropertyPage::OnSetActive();
}

void CViewPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nPage;
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	if(pSheet->m_nScanLineDirection==HORIZONTAL) {
		nPage = RoundDownMulDiv(theApp.m_LastSettings.nViewPagePercent,m_prrSamples->Width(),100);
	} else {
		nPage = RoundDownMulDiv(theApp.m_LastSettings.nViewPagePercent,m_prrSamples->Height(),100);
	}

	int CurrentPos = GetScrollPos(SB_VERT);
	switch(nSBCode) {
	case SB_LINEDOWN:
		CurrentPos+=(nPage/10);
		break;
	case SB_LINEUP:
		 CurrentPos-=(nPage/10);
		break;
	case SB_PAGEDOWN:
		CurrentPos+=nPage;
		break;
	case SB_PAGEUP:
		CurrentPos-=nPage;
		break;
	case SB_THUMBTRACK:
		CurrentPos = nPos;
		break;
	}
	if(CurrentPos < 0) CurrentPos = 0;
	if(pSheet->m_nScanLineDirection==HORIZONTAL) {
		if(CurrentPos>=m_pData->m_nNumberLines) CurrentPos=m_pData->m_nNumberLines-1;
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2) ){
			m_prrSamples->top=CurrentPos;
		} else {
			m_prrSamples->top=m_pData->m_nNumberLines-1-(CurrentPos+m_prrSamples->Height());
		};
		MinMax(&m_prrSamples->top,0,m_pData->m_nNumberLines-1);
	} else {
		if(CurrentPos>=m_pData->m_nSamplesLine) CurrentPos=m_pData->m_nSamplesLine-1;
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2) ){
			m_prrSamples->left=CurrentPos;
		} else {
			m_prrSamples->left=m_pData->m_nSamplesLine-1-(CurrentPos+m_prrSamples->Width());
		};
		MinMax(&m_prrSamples->left,0,m_pData->m_nSamplesLine-1);
	}

	SetScrollPos(SB_VERT,CurrentPos,TRUE);


	m_bRenderView=TRUE;
	Invalidate(FALSE);

	pSheet->InvalidateChildren();

	CResizablePage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CViewPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nPage;
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	if(pSheet->m_nScanLineDirection==HORIZONTAL) {
		nPage = RoundDownMulDiv(theApp.m_LastSettings.nViewPagePercent,m_prrSamples->Width(),100);
	} else {
		nPage = RoundDownMulDiv(theApp.m_LastSettings.nViewPagePercent,m_prrSamples->Height(),100);
	}
	int CurrentPos = GetScrollPos(SB_HORZ);
	switch(nSBCode) {
	case SB_LINEDOWN:
		CurrentPos+=(nPage/10);
		break;
	case SB_LINEUP:
		 CurrentPos-=(nPage/10);
		break;
	case SB_PAGEDOWN:
		CurrentPos+=nPage;
		break;
	case SB_PAGEUP:
		CurrentPos-=nPage;
		break;
	case SB_THUMBTRACK:
		CurrentPos = nPos;
		break;
	}
	if (CurrentPos < 0) CurrentPos = 0;
	if(pSheet->m_nScanLineDirection==HORIZONTAL) {
		if(CurrentPos>=m_pData->m_nSamplesLine) CurrentPos=m_pData->m_nSamplesLine-1;
		m_prrSamples->left=CurrentPos;
	} else {
		if(CurrentPos>=m_pData->m_nNumberLines) CurrentPos=m_pData->m_nNumberLines-1;
		m_prrSamples->top=CurrentPos;
	}

	SetScrollPos(SB_HORZ,CurrentPos,TRUE);

	m_bRenderView=TRUE;
	m_bInitialize=TRUE;
	Invalidate(FALSE);

	pSheet->InvalidateChildren();

	CResizablePage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CViewPage::DrawGraticule(CPaintDC *pDC)
{
	float	fStep=100.0f;
	int	nTextStep=500;
	int ii;
	float xx,yy,fXSize,fYSize,fFastScanSize,fFastIncrement,fDivisor;
	int	pnx,pny;
	CString Buff;
	CSize TextSize;
	CFont HorizontalFont;
	CFont VerticalFont;
	CFont* pOldFont;
	CRect rrClient,rr;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));

	GetClientRect(&rrClient);
	rr=rrClient;
	rr.bottom=m_rrView.top-2;
	pDC->FillRect(&rr,&brushBK);
	rr=rrClient;
	rr.top=m_rrView.bottom+2;
	pDC->FillRect(&rr,&brushBK);
	rr=rrClient;
	rr.right=m_rrView.left-2;
	pDC->FillRect(&rr,&brushBK);
	rr=rrClient;
	rr.left=m_rrView.right+2;
	pDC->FillRect(&rr,&brushBK);

	if((theApp.m_LastSettings.nViewGraticuleStyle & (GRATICULE_BOTTOM | GRATICULE_RIGHT | GRATICULE_LEFT | GRATICULE_TOP)) == 0) return;
	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);
	theApp.m_LastSettings.nMeasurementUnits==0 ? fDivisor = 10.0f : fDivisor = 12.0f;

	GetGraticuleSize(&fXSize,&fYSize,&fStep,&nTextStep,theApp.m_fUnits);
	float xStart = (float)m_prrSamples->left * (float)fFastIncrement;
	float yStart = (float)m_prrSamples->top * (float)m_pData->m_fSlowIncrement;
	rr=m_rrView;
	rr.InflateRect(2,2);

	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);

	pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);

	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_BOTTOM) {
		for(xx=0,ii=0;xx<fXSize;xx+=fStep,ii++) {
			pnx=RoundDownMulDiv((int)xx,rr.Width()-1,(int)fXSize)+rr.left;
			pDC->MoveTo(pnx,rr.bottom);
			pDC->LineTo(pnx,rr.bottom+4);
			if(((int)xx%nTextStep) == 0) {
				if(theApp.m_LastSettings.nGraticuleScrollMode==0) {
					Buff.Format(_T("%.01f"),xx);
					if((m_pData->m_nFastAxis == 5) && (theApp.m_LastSettings.nRAxisGraticuleReverse == 1) && (theApp.m_LastSettings.nRAxisGraticuleMode==0)) {
						Buff.Format(_T("%.01f"),(float)(360-xx));
					};
				} else {
					Buff.Format(_T("%.01f"),xx+xStart);
				}
				TextSize=pDC->GetTextExtent(Buff);
				if(ii==0) {
					pDC->TextOut(pnx,rr.bottom+3,Buff);
				} else {
					pDC->TextOut(pnx-TextSize.cx/2,rr.bottom+3,Buff);
				}
			};
		}
		for(xx=0,ii=0;xx<fXSize;xx+=(fStep/fDivisor),ii++) {
			pnx=RoundDownMulDiv((int)xx,rr.Width(),(int)fXSize)+rr.left;
			if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_BOTTOM) {
				pDC->MoveTo(pnx,rr.bottom);
				pDC->LineTo(pnx,rr.bottom+2);
			}
		};
	};

	pDC->SelectObject(&VerticalFont);
	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_RIGHT) {
		for(yy=0,ii=0;yy<fYSize;yy+=fStep,ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0)||(theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			pDC->MoveTo(rr.right,pny);
			pDC->LineTo(rr.right+6,pny);
			if(((int)yy%nTextStep) == 0) {
				if(theApp.m_LastSettings.nGraticuleScrollMode==0) {
					Buff.Format(_T("%.01f"),yy);
				} else {
					Buff.Format(_T("%.01f"),yy+yStart);
				}
				TextSize=pDC->GetTextExtent(Buff);
				if(ii==0) {
					pDC->TextOut(rr.right+3,pny+TextSize.cx,Buff);
				} else {
					pDC->TextOut(rr.right+3,pny+TextSize.cx/2,Buff);
				}
			};
		}
		for(yy=0,ii=0;yy<fYSize;yy+=(fStep/fDivisor),ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0)||(theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			pDC->MoveTo(rr.right,pny);
			pDC->LineTo(rr.right+3,pny);
		}
	}
	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_LEFT) {
		for(yy=0,ii=0;yy<fYSize;yy+=fStep,ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0)||(theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			pDC->MoveTo(rr.left,pny);
			pDC->LineTo(rr.left-6,pny);
			if(((int)yy%nTextStep) == 0) {
				if(theApp.m_LastSettings.nGraticuleScrollMode==0) {
					Buff.Format(_T("%.01f"),yy);
				} else {
					Buff.Format(_T("%.01f"),yy+yStart);
				}
				TextSize=pDC->GetTextExtent(Buff);
				if(ii==0) {
					pDC->TextOut(rr.left-3-TextSize.cy,pny+TextSize.cx,Buff);
				} else {
					pDC->TextOut(rr.left-3-TextSize.cy,pny+TextSize.cx/2,Buff);
				}
			};
		}
		for(yy=0,ii=0;yy<fYSize;yy+=(fStep/fDivisor),ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0)||(theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			pDC->MoveTo(rr.left,pny);
			pDC->LineTo(rr.left-3,pny);
		}
	};

	pDC->SelectObject(pOldFont);

	fStep/=10;
	CPen penLine(PS_SOLID,1,theApp.m_LastSettings.rgbViewGraticuleColor);
	CPen* pOldPen = pDC->SelectObject(&penLine);

	if(theApp.m_LastSettings.nViewGraticuleSpacingMode == 0) {
		fStep = theApp.m_LastSettings.fViewGraticuleSpacing;
		if(fStep<=0.0f) fStep = 0.5f;
	}

	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_LINES) {
		for(xx=fStep;xx<fXSize;xx+=fStep,ii++) {
			pnx=RoundDownMulDiv((int)xx,rr.Width()-1,(int)fXSize)+rr.left;
			if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_BOTTOM) {
				pDC->MoveTo(pnx,m_rrView.bottom);
				pDC->LineTo(pnx,m_rrView.top);
			}
		};
		for(yy=fStep,ii=0;yy<fYSize;yy+=fStep,ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0)||(theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			pDC->MoveTo(m_rrView.left,pny);
			pDC->LineTo(m_rrView.right,pny);
		}
	}
	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_DOTS) {
		for(yy=fStep,ii=0;yy<fYSize;yy+=fStep,ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0)||(theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			for(xx=fStep,ii=0;xx<fXSize;xx+=fStep,ii++) {
				pnx=RoundDownMulDiv((int)xx,rr.Width()-1,(int)fXSize)+rr.left;
				pDC->SetPixel(pnx,pny,theApp.m_LastSettings.rgbViewGraticuleColor);
			};
		}
	}
	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_CROSSES) {
		for(yy=fStep,ii=0;yy<(fYSize-(fStep/2));yy+=fStep,ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0)||(theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			for(xx=fStep,ii=0;xx<(fXSize-(fStep/2));xx+=fStep,ii++) {
				pnx=RoundDownMulDiv((int)xx,rr.Width()-1,(int)fXSize)+rr.left;
				pDC->MoveTo(pnx-3,pny);
				pDC->LineTo(pnx+4,pny);
				pDC->MoveTo(pnx,pny-3);
				pDC->LineTo(pnx,pny+4);
			};
		}
	}


	pDC->SelectObject(pOldPen);
}


void CViewPage::GetGraticuleSize(float *fXSize, float *fYSize,float *fStep,int *nTextStep,float fUnits)
{
	float fTemp,fFastScanSize,fFastIncrement;

	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);

	*fXSize = fFastIncrement * (float)m_prrSamples->Width() * fUnits;
	*fYSize = m_pData->m_fSlowIncrement *  (float)m_prrSamples->Height() * fUnits;

	*fXSize>=*fYSize ? fTemp = *fXSize : fTemp = *fYSize;

	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0:
		*fStep = 500.0;
		*nTextStep=1000;
		if(fTemp<=1000) {
			*fStep = 100.0;
			*nTextStep=500;
		};
		if(fTemp<=500) {
			*fStep = 100.0;
			*nTextStep=100;
		};
		if(fTemp<=100) {
			*fStep = 10.0;
			*nTextStep=50;
		};
		if(fTemp<=50) {
			*fStep = 10.0;
			*nTextStep=10;
		};
		break;
	case 1:
		*fStep = 12.0;
		*nTextStep=12;
		if(fTemp<=120) {
			*fStep = 12.0;
			*nTextStep=12;
		};
		if(fTemp<=60.0f) {
			*fStep = 6.0f;
			*nTextStep=6;
		};
		if(fTemp<=12.0f) {
			*fStep = 1.0;
			*nTextStep=1;
		};
		break;
	}


}


void CViewPage::OnLButtonDown(UINT nFlags, CPoint point)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	CCoord Cp;
	CString Buff;
	CPoint ptSample;
	float fSample,fLine;

	m_bLButtonDown = true;

	if(pViewSheet->m_bRenderProfile){
		ClientToWorld(point,&Cp.Side0.fX,&Cp.Side0.fY,true);
		PROFILE->FindNearestCoordinate(TAUGHTLINES,Cp,&theApp.m_nLine,&theApp.m_nIndex,'Z');
		Invalidate(FALSE);
	}else if(pViewSheet->m_bRenderBrane){
		ClientToWorld(point,&Cp.Side0.fX,&Cp.Side0.fY,true);
		PROFILE->FindNearestCoordinate(BRAINLINES,Cp,&theApp.m_nLine,&theApp.m_nIndex,'Z');
		Invalidate(FALSE);
	}else{
		switch(pViewSheet->m_nToolOption) {
		case 0:
			if(m_rrView.PtInRect(point)) {
				m_nToolOption=pViewSheet->m_nToolOption;
				m_ptCursorBtDn = point;
				m_rrZoomSamplesBtDn = m_prrSamples;
			}
			break;
		case TOOL_RULER:
			if(m_rrView.PtInRect(point)) {
				m_nToolOption=pViewSheet->m_nToolOption;
				m_rrRuler.TopLeft()=point;
				m_rrRuler.BottomRight()=point;
				Invalidate(FALSE);
			}
			break;
		case TOOL_HISTOGRAM:
		case TOOL_INCLUSION:
			if(m_rrView.PtInRect(point)) {
				m_nToolOption=pViewSheet->m_nToolOption;
				m_rrRuler.TopLeft()=point;
				m_rrRuler.BottomRight()=point;
				Invalidate(FALSE);
			}
			break;
		case TOOL_ZOOM_VARIABLE:
			if(m_rrView.PtInRect(point)) {
				m_nToolOption=pViewSheet->m_nToolOption;
				m_rrRuler.TopLeft()=point;
				m_rrRuler.BottomRight()=point;
				Invalidate(FALSE);
			}
			break;
		case TOOL_CROSS_SECTION:
			if(m_rrView.PtInRect(point)) {
				m_nToolOption=pViewSheet->m_nToolOption;
				m_rrRuler.TopLeft()=point;
				m_rrRuler.BottomRight()=point;
			}
			break;
		case TOOL_VALUE:
			if(m_rrView.PtInRect(point)) {
				m_nToolOption=pViewSheet->m_nToolOption;
				m_rrRuler.BottomRight()=point;
				Invalidate(FALSE);
				GetValueStr();
				pViewSheet->EditTextSetWindowText(m_strValue);
			}
			break;
		case TOOL_POLYGON:
			if(m_rrView.PtInRect(point)) {
				m_nToolOption=pViewSheet->m_nToolOption;
				if(ClientToCoord(m_rrRuler.BottomRight(),&Cp) ) {
					m_rrRuler.BottomRight()=point;
					ClientToSample(m_rrRuler.BottomRight(),&point);
					Cp.Side1.fX = (float)point.x;
					Cp.Side1.fY = (float)point.y;
				//	m_pData->AddPolygonPt(&m_nPolyPtr,Cp);
					Invalidate(FALSE);
					DisplayPolygonStats();
				}
			}
			break;
		case TOOL_CAD:
			m_nToolOption=pViewSheet->m_nToolOption;
			CADLButtonDown(point);
			Invalidate(FALSE);
			break;
		case TOOL_RECTANGLE:
		case TOOL_LINE:
		case TOOL_ELLIPSE:
		case TOOL_PARALLELOGRAM:
		case TOOL_CENTRELINE:
			if(m_rrView.PtInRect(point)) {
				ClientToSample(point,&fSample,&fLine);
				if(m_nMoveCurrentCADElement == 0) {
					m_nToolOption = pViewSheet->m_nToolOption;
					if(m_nToolOption == TOOL_RECTANGLE)	pViewSheet->m_Data.AddCADElement(CAD_RECTANGLE,fSample,fLine,m_nImageType,m_nImageNumber,m_nPaletteNumber);
					if(m_nToolOption == TOOL_LINE)		pViewSheet->m_Data.AddCADElement(CAD_LINE,fSample,fLine,m_nImageType,m_nImageNumber,m_nPaletteNumber);
					if(m_nToolOption == TOOL_ELLIPSE)	pViewSheet->m_Data.AddCADElement(CAD_ELLIPSE,fSample,fLine,m_nImageType,m_nImageNumber,m_nPaletteNumber);
					if(m_nToolOption == TOOL_PARALLELOGRAM)	pViewSheet->m_Data.AddCADElement(CAD_PARALLELOGRAM,fSample,fLine,m_nImageType,m_nImageNumber,m_nPaletteNumber);
					if(m_nToolOption == TOOL_CENTRELINE)	pViewSheet->m_Data.AddCADElement(CAD_CENTRELINE,fSample,fLine,m_nImageType,m_nImageNumber,m_nPaletteNumber);
					m_nMoveCurrentCADElement = 1;
					m_nMoveCursor=4;
				}
				pViewSheet->m_nToolOption=0;
				pViewSheet->SetToolBarCheckedState();
				CADElementSizeArrows();
				Invalidate(FALSE);
				pViewSheet->RefreshCADTable();
			}
			break;
		}
	}


	CResizablePage::OnLButtonDown(nFlags, point);
}

void CViewPage::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLButtonDown = false;

	CResizablePage::OnLButtonUp(nFlags, point);
}

void CViewPage::OnRButtonDown(UINT nFlags, CPoint point) 
{

	m_bRButtonDown = true;
	m_ptClient = point;
	
	CResizablePage::OnRButtonDown(nFlags, point);
}

void CViewPage::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_bRButtonDown = false;
	
	CResizablePage::OnRButtonUp(nFlags, point);
}

void CViewPage::OnMouseMove(UINT nFlags, CPoint point)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	CString Buff,Message,Width,Height,Length;
	float fX0,fY0,fX1,fY1,fZ0,fPos;
	CPoint pt,pt0,pt1,ptSample;
	CRect	rrTemp;
	CCoord Cp,Cp0;
	int nn;
	CPoint pt2, tempPt;
	int nY;
	float fSample,fLine;

	switch(m_nToolOption) {
	case 0:
		if(m_rrView.PtInRect(point) && (m_bLButtonDown == true)) {
			point -= m_ptCursorBtDn;
			ClientToSample(point,&tempPt);
			switch(theApp.m_LastSettings.nImageOrientation) {
			case 0:
				tempPt.x = RoundDownMulDiv(point.x,m_prrSamples->Width(),m_rrView.Width()-1);
				tempPt.y = RoundDownMulDiv(point.y,m_prrSamples->Height()-1,m_rrView.Height()-1);
				break;
			case 1:
				tempPt.x = RoundDownMulDiv(point.x,m_prrSamples->Width(),m_rrView.Width()-1);
				tempPt.y = -RoundDownMulDiv(point.y,m_prrSamples->Height()-1,m_rrView.Height()-1);
				break;
			case 2:
				tempPt.x = -RoundDownMulDiv(point.x,m_prrSamples->Width(),m_rrView.Width()-1);
				tempPt.y = RoundDownMulDiv(point.y,m_prrSamples->Height()-1,m_rrView.Height()-1);
				break;
			case 3:
				tempPt.x = -RoundDownMulDiv(point.x,m_prrSamples->Width(),m_rrView.Width()-1);
				tempPt.y = -RoundDownMulDiv(point.y,m_prrSamples->Height()-1,m_rrView.Height()-1);
				break;
			}

			m_prrSamples->left = m_rrZoomSamplesBtDn.left - tempPt.x;
			m_prrSamples->right = m_rrZoomSamplesBtDn.right - tempPt.x;
			m_prrSamples->top = m_rrZoomSamplesBtDn.top - tempPt.y;
			m_prrSamples->bottom = m_rrZoomSamplesBtDn.bottom - tempPt.y;
			m_bRenderView=TRUE;

			Invalidate(FALSE);
		}
		break;
	case TOOL_RULER:
		if(m_rrView.PtInRect(point) && (m_bLButtonDown == true)) {
			m_rrRuler.BottomRight()=point;
			Invalidate(FALSE);
			if(m_pData->m_nFastAxis == 5) {
				if(m_pData->m_ProfileName == PROFILE->m_FileName) {
					ComputeRulerStats(&m_strRuler);
				} else {
					m_strRuler = "Please load correct profile";
				}
			}else{
				ClientToWorld(m_rrRuler.TopLeft(),&fX0,&fY0);
				ClientToWorld(m_rrRuler.BottomRight(),&fX1,&fY1);
				switch(theApp.m_LastSettings.nRulerTextMode) {
				default:
					m_fRulerLength=(float)sqrt((fX0-fX1)*(fX0-fX1) + (fY0-fY1)*(fY0-fY1));
					Message.LoadString(IDS_Ruler_Length);
					theApp.FormatLength(m_fRulerLength,1,Length);
					m_strRuler.Format(_T("%s:%s"),Message,Length);
					break;
				case 1:
					if(m_pData->m_ProfileName == PROFILE->m_FileName) {
						ClientToSample(m_rrRuler.BottomRight(),&pt);
						pt.y *= m_pData->m_nLineIncrement;
						pt.y+=m_pData->m_nScanStartLine;
						fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fX;
						PROFILE->GetSurfaceCoordinate(Cp,pt,fPos,m_pData->m_nFastAxis);
						ClientToSample(m_rrRuler.TopLeft(),&pt);
						pt.y *= m_pData->m_nLineIncrement;
						pt.y+=m_pData->m_nScanStartLine;
						fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fX;
						PROFILE->GetSurfaceCoordinate(Cp0,pt,fPos,m_pData->m_nFastAxis);
						fX0 = (Cp0.Side0.fX - Cp.Side0.fX) * (Cp0.Side0.fX - Cp.Side0.fX);
						fY0 = (Cp0.Side0.fY - Cp.Side0.fY) * (Cp0.Side0.fY - Cp.Side0.fY);
						fZ0 = (Cp0.Side0.fZ - Cp.Side0.fZ) * (Cp0.Side0.fZ - Cp.Side0.fZ);
						m_fRulerLength=(float)sqrt(fX0 + fY0 + fZ0);
						Message.LoadString(IDS_Vector_Length);
						theApp.FormatLength(m_fRulerLength,1,Length);
						m_strRuler.Format(_T("%s:%s"),Message,Length);
					} else {
						m_strRuler = "Please load correct profile";
					}
					break;
				case 2:
					if(m_pData->m_ProfileName == PROFILE->m_FileName) {
						ClientToSample(m_rrRuler.BottomRight(),&pt0);
						ClientToSample(m_rrRuler.TopLeft(),&pt1);
						m_fRulerLength=0.0f;
						for(nn=0;nn<30;nn++) {
							pt.x=RoundDownMulDiv(nn,pt1.x-pt0.x,29) + pt0.x;
							pt.y=RoundDownMulDiv(nn,pt1.y-pt0.y,29) + pt0.y;
							pt.y *= m_pData->m_nLineIncrement;
							pt.y+=m_pData->m_nScanStartLine;
							fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fX;
							PROFILE->GetSurfaceCoordinate(Cp,pt,fPos,m_pData->m_nFastAxis);
							if(nn>0) {
								fX0 = (Cp0.Side0.fX - Cp.Side0.fX) * (Cp0.Side0.fX - Cp.Side0.fX);
								fY0 = (Cp0.Side0.fY - Cp.Side0.fY) * (Cp0.Side0.fY - Cp.Side0.fY);
								fZ0 = (Cp0.Side0.fZ - Cp.Side0.fZ) * (Cp0.Side0.fZ - Cp.Side0.fZ);
								m_fRulerLength+=(float)sqrt(fX0 + fY0 + fZ0);
							}
							Cp0 = Cp;
						}
						Message.LoadString(IDS_Surface_Length);
						theApp.FormatLength(m_fRulerLength,1,Length);
						m_strRuler.Format(_T("%s:%s"),Message,Length);
					} else {
						m_strRuler = "Please load correct profile";
					}
					break;
				case 3:
					Width.LoadString(IDS_Width);
					Height.LoadString(IDS_Height);
					if(theApp.m_LastSettings.nMeasurementUnits == 0) {
						m_strRuler.Format(_T("%s:%.01fmm %s:%.01fmm"),Width,fabs(fX0-fX1),Height,fabs(fY0-fY1));
					} else {
						m_strRuler.Format(_T("%s:%.03f\x022 %s:%.03f\x022"),Width,fabs(fX0-fX1)/25.4f,Height,fabs(fY0-fY1)/25.4f);
					}
					break;
				case 4:
					Message.LoadString(IDS_Diameter);
					if(theApp.m_LastSettings.nMeasurementUnits == 0) {
						m_strRuler.Format(_T("%s:%.01fmm"),Message,fabs(fY0-fY1));
					} else {
						m_strRuler.Format(_T("%s:%.03f\x022"),Message,fabs(fY0-fY1)/25.4f);
					}
					break;
				}
			}
			pViewSheet->EditTextSetWindowText(m_strRuler);

		}
		break;
	case TOOL_HISTOGRAM:
		if(m_rrView.PtInRect(point)) {
			if(m_bLButtonDown == true) {
				m_rrRuler.BottomRight()=point;
				if(theApp.m_LastSettings.nHistogramDropOut == 1) {	//drop out
					LimitRulerToDropOutSize();
					m_bRenderView = TRUE;
				}
			};
			if(m_bRButtonDown == true) {
				m_rrRuler.left += (point.x - m_ptClient.x);
				m_rrRuler.right += (point.x - m_ptClient.x);
				m_rrRuler.top += (point.y - m_ptClient.y);
				m_rrRuler.bottom += (point.y - m_ptClient.y);
				m_ptClient = point;
				m_bRenderView = TRUE;
			}
			if((theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) || (theApp.m_LastSettings.bHistogramPaintView[1]==TRUE)) {
				m_bPalette=TRUE;
				m_bRenderView=TRUE;
			}
			Invalidate(FALSE);

			if(pViewSheet->m_nRectPolarDisplayMode == 0) {
				switch(theApp.m_LastSettings.nHistogramStyle){
				case 0:
					if(theApp.m_LastSettings.bHistogramEnterSize==0){
						ClientToSample(m_rrRuler.TopLeft(),&pt);
						m_rrHistogramSample.TopLeft() = pt;
						ClientToSample(m_rrRuler.BottomRight(),&pt);
						m_rrHistogramSample.BottomRight() = pt;
					} else {
						m_rrRuler.left = m_rrRuler.right;
						ClientToSample(m_rrRuler.BottomRight(), &pt);
						if( (pt.x -= (int)( (theApp.m_LastSettings.fHistogramCircleSize/2.0f) / m_pData->m_fFastIncrement)) <0.0f) pt.x=0;
						if( (pt.y -= (int)( (theApp.m_LastSettings.fHistogramCircleSize/2.0f) / m_pData->m_fSlowIncrement)) <0.0f) pt.y=0;
						m_rrHistogramSample.TopLeft() = pt;
						ClientToSample(m_rrRuler.BottomRight(), &pt);
						if( (pt.x += (int)( (theApp.m_LastSettings.fHistogramCircleSize/2.0f) / m_pData->m_fFastIncrement)) >m_pData->m_nSamplesLine-1) pt.x = m_pData->m_nSamplesLine-1;
						if( (pt.y += (int)( (theApp.m_LastSettings.fHistogramCircleSize/2.0f) / m_pData->m_fSlowIncrement)) >m_pData->m_nNumberLines-1) pt.y = m_pData->m_nNumberLines-1;
						m_rrHistogramSample.BottomRight() = pt;

					}
					if(m_pData->BuildHistogramData(m_nImageType,m_nImageNumber,m_rrHistogramSample) == TRUE) {
						RefreshHistogram();
					}
					break;
				case 1:
					m_rrRuler.left = m_rrRuler.right;
					//Make circle with respect to image not screen
					//Draw a box around the circle

					if(theApp.m_LastSettings.bHistogramEnterSize==0){
						ClientToSample(m_rrRuler.TopLeft(), &pt);
						ClientToSample(m_rrRuler.BottomRight(), &pt2);
						nY = abs(pt.y-pt2.y);
						pt.x -= (int)((float)(nY/2)*(m_pData->m_fFastIncrement/m_pData->m_fSlowIncrement));
						pt2.x += (int)((float)(nY/2)*(m_pData->m_fFastIncrement/m_pData->m_fSlowIncrement));
						SampleToClient(pt,&tempPt);
						rrTemp.left = tempPt.x;
						SampleToClient(pt2,&tempPt);
						rrTemp.right = tempPt.x;
						rrTemp.top = m_rrRuler.top;
						rrTemp.bottom = m_rrRuler.bottom;
						ClientToSample(rrTemp.TopLeft(),&pt);
						m_rrHistogramSample.TopLeft() = pt;
						ClientToSample(rrTemp.BottomRight(),&pt);
						m_rrHistogramSample.BottomRight() = pt;
					}else{
						//Create Circle with entered Dimension with ruler bottom right centre
						ClientToSample(m_rrRuler.BottomRight(), &pt);
						if( (pt.x -= (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fFastIncrement)) <0) pt.x=0;
						if( (pt.y -= (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fSlowIncrement)) <0) pt.y=0;
						m_rrHistogramSample.TopLeft() = pt;
						ClientToSample(m_rrRuler.BottomRight(), &pt);
						if( (pt.x += (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fFastIncrement)) >m_pData->m_nSamplesLine-1) pt.x = m_pData->m_nSamplesLine-1;
						if( (pt.y += (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fSlowIncrement)) >m_pData->m_nNumberLines-1) pt.y = m_pData->m_nNumberLines-1;
						m_rrHistogramSample.BottomRight() = pt;
					}
					break;
				}
				if(m_pData->BuildHistogramData(m_nImageType,m_nImageNumber,m_rrHistogramSample) == TRUE) {
					RefreshHistogram();
				}
			} else {
				pt = m_rrRuler.TopLeft();
				ClientToSample(pt,&ptSample);
				m_rrHistogramSample.TopLeft() = ptSample;

				pt = m_rrRuler.BottomRight();
				ClientToSample(pt,&ptSample);
				m_rrHistogramSample.BottomRight() = ptSample;

				m_rrHistogramSample.NormalizeRect();

				Buff.Format(L"Line %d Sample %d",ptSample.y, ptSample.x);
				pViewSheet->EditTextSetWindowText(Buff);

				if(m_pData->BuildHistogramData(m_nImageType,m_nImageNumber,m_rrHistogramSample) == TRUE) {
					RefreshHistogram();
				}
			}
		}
		break;
	case TOOL_CROSS_SECTION:
		if(m_rrView.PtInRect(point) && (m_bLButtonDown == true)) {
			m_rrRuler.BottomRight()=point;
			m_bRenderView=TRUE;
			Invalidate(FALSE);
		}
		break;
	case TOOL_VALUE:
		if(m_rrView.PtInRect(point) && (m_bLButtonDown == true)) {
			m_rrRuler.BottomRight()=point;
			Invalidate(FALSE);
			GetValueStr();
			pViewSheet->EditTextSetWindowText(m_strValue);
		}
		break;
	case TOOL_POLYGON:
		if(m_rrView.PtInRect(point) && (m_bLButtonDown == true)) {
			m_rrRuler.BottomRight()=point;
			if(ClientToCoord(m_rrRuler.BottomRight(),&Cp) ) {
				ClientToSample(m_rrRuler.BottomRight(),&point);
				Cp.Side1.fX = (float)point.x;
				Cp.Side1.fY = (float)point.y;
				m_pData->AddPolygonPt(&m_nPolyPtr,Cp);
				Invalidate(FALSE);
				DisplayPolygonStats();
			}
		}
		break;
	case TOOL_CAD:
		if(pViewSheet->m_Data.m_nCADElementL>0) {

			switch(pViewSheet->m_Data.m_pCADElement[pViewSheet->m_Data.m_nCADPtr].m_Type) {
			case CAD_LINE:
				if(pViewSheet->m_Data.m_pCADElement[pViewSheet->m_Data.m_nCADPtr].m_Nodes.m_nCoordL<2) {
					Invalidate(FALSE);
				}
				break;
			}
		}
		break;
	case TOOL_ZOOM_VARIABLE:
		if(m_rrView.PtInRect(point) && (m_bLButtonDown == true)) {
			m_rrRuler.BottomRight()=point;
			Invalidate(FALSE);

		}
		break;
	case TOOL_RECTANGLE:
	case TOOL_LINE:
	case TOOL_ELLIPSE:
		if(m_rrView.PtInRect(point)) {
			ClientToSample(point,&fSample,&fLine);
			if(m_bLButtonDown == true) {
				switch(m_nMoveCursor) {
				case 0:	pViewSheet->m_Data.ModifyNodeCADElement(0,fSample,fLine);
					break;
				case 1:	pViewSheet->m_Data.ModifyTopCADElement(fSample,fLine);
					break;
				case 2:	pViewSheet->m_Data.ModifyTopCADElement(fSample,fLine);
						pViewSheet->m_Data.ModifyRightCADElement(fSample,fLine);
					break;
				case 3:	pViewSheet->m_Data.ModifyRightCADElement(fSample,fLine);
					break;
				case 4:	pViewSheet->m_Data.ModifyNodeCADElement(1,fSample,fLine);
					break;
				case 5:	pViewSheet->m_Data.ModifyBottomCADElement(fSample,fLine);
					break;
				case 6:	pViewSheet->m_Data.ModifyBottomCADElement(fSample,fLine);
						pViewSheet->m_Data.ModifyLeftCADElement(fSample,fLine);
					break;
				case 7:	pViewSheet->m_Data.ModifyLeftCADElement(fSample,fLine);
					break;
				case 8:	pViewSheet->m_Data.OffsetCADElement(fSample,fLine);
					break;
				}
				CADElementSizeArrows();
				pViewSheet->m_Data.ComputeAllElementStats(pViewSheet->m_Data.GetCADElement());
				pViewSheet->RefreshCADTable();
				if(m_nToolOption == TOOL_RECTANGLE) RefreshHistogram();
				Invalidate(FALSE);
			} else {
				pViewSheet->m_Data.TempStoreNodesCADElement(fSample,fLine);
			}

		}
		break;
	case TOOL_PARALLELOGRAM:
		if(m_rrView.PtInRect(point)) {
			ClientToSample(point,&fSample,&fLine);
			if(m_bLButtonDown == true) {
				switch(m_nMoveCursor) {
				case 0:	pViewSheet->m_Data.MoveCornerOfParallelogram(0,fSample,fLine);
					break;
				case 1:	pViewSheet->m_Data.MoveSideOfParallelogram(m_nMoveCursor,fSample,fLine);
					break;
				case 2:	pViewSheet->m_Data.MoveCornerOfParallelogram(1,fSample,fLine);
					break;
				case 3:	pViewSheet->m_Data.MoveSideOfParallelogram(m_nMoveCursor,fSample,fLine);
					break;
				case 4:	pViewSheet->m_Data.MoveCornerOfParallelogram(2,fSample,fLine);
					break;
				case 5:	pViewSheet->m_Data.MoveSideOfParallelogram(m_nMoveCursor,fSample,fLine);
					break;
				case 6:	pViewSheet->m_Data.MoveCornerOfParallelogram(3,fSample,fLine);
					break;
				case 7:	pViewSheet->m_Data.MoveSideOfParallelogram(m_nMoveCursor,fSample,fLine);
					break;
				case 8:	pViewSheet->m_Data.OffsetCADElement(fSample,fLine);
					break;
				}
				CADElementSizeArrows();
				pViewSheet->m_Data.ComputeAllElementStats(pViewSheet->m_Data.GetCADElement());
				pViewSheet->RefreshCADTable();
				if(m_nToolOption == TOOL_RECTANGLE) RefreshHistogram();
				Invalidate(FALSE);
			} else {
				pViewSheet->m_Data.TempStoreNodesCADElement(fSample,fLine);
			}
		}
		break;
	case TOOL_CENTRELINE:
		if(m_rrView.PtInRect(point)) {
			ClientToSample(point,&fSample,&fLine);
			if(m_bLButtonDown == true) {
				switch(m_nMoveCursor) {
				case 3:	pViewSheet->m_Data.MoveSideOfCentreLine(m_nMoveCursor,fSample,fLine);
					break;
				case 4:	pViewSheet->m_Data.MoveCornerOfParallelogram(2,fSample,fLine); //Used on first draw
					break;
				case 7:	pViewSheet->m_Data.MoveSideOfCentreLine(m_nMoveCursor,fSample,fLine);
					break;
				case 8:	pViewSheet->m_Data.OffsetCADElement(fSample,fLine);
					break;
				}
				CADElementSizeArrows();
				CADPopulateNodesWorldCoords();
				pViewSheet->m_Data.ComputeAllElementStats(pViewSheet->m_Data.GetCADElement());
				pViewSheet->RefreshCADTable();
				Invalidate(FALSE);
			} else {
				pViewSheet->m_Data.TempStoreNodesCADElement(fSample,fLine);
			}

		}
		break;
	case TOOL_INCLUSION:
		if(m_rrView.PtInRect(point)) {
			if(m_bLButtonDown == true) {
				m_rrRuler.BottomRight()=point;
			};
			if(m_bRButtonDown == true) {
				m_rrRuler.left += (point.x - m_ptClient.x);
				m_rrRuler.right += (point.x - m_ptClient.x);
				m_rrRuler.top += (point.y - m_ptClient.y);
				m_rrRuler.bottom += (point.y - m_ptClient.y);
				m_ptClient = point;
				m_bRenderView = TRUE;
			}
			ClientToSample(m_rrRuler.TopLeft(),&pt);
			m_rrHistogramSample.TopLeft() = pt;
			ClientToSample(m_rrRuler.BottomRight(),&pt);
			m_rrHistogramSample.BottomRight() = pt;

			if(m_bLButtonDown == true || m_bRButtonDown == true) {
				if(m_pData->BuildInclusionData(m_nImageType,m_nImageNumber,m_rrHistogramSample) == TRUE) {
					RefreshInclusions();
				}
				Invalidate(FALSE);
			}
		}
		break;
	}
	CResizablePage::OnMouseMove(nFlags, point);
}


void CViewPage::CADPopulateNodesWorldCoords(int nNode)
{
	//Populate Realworld positions for CAD nodes

	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	CPolyCoord* pNodes = &pViewSheet->m_Data.m_pCADElement[pViewSheet->m_Data.m_nCADPtr].m_Nodes;
	CPoint Pt,PtPixel;
	int ii;

	if(nNode==-1) {
		for(ii=0;ii<4;ii++){
			Pt.x=(int)pNodes->m_pCp[ii].fSample;
			Pt.y=(int)pNodes->m_pCp[ii].fLine;
			SampleToClient(Pt,&PtPixel);
			ClientToWorld(PtPixel, &pNodes->m_pCp[ii].Side0.fX, &pNodes->m_pCp[ii].Side0.fY);
		}
	}else{
		Pt.x=(int)pNodes->m_pCp[nNode].fSample;
		Pt.y=(int)pNodes->m_pCp[nNode].fLine;
		SampleToClient(Pt,&PtPixel);
		ClientToWorld(PtPixel, &pNodes->m_pCp[nNode].Side0.fX, &pNodes->m_pCp[nNode].Side0.fY);
	}


}

void CViewPage::DrawRuler(CPaintDC* pDC)
{
	CRect rr;
	int	nX,nY,nHypot0,nHypot1,nn,nTemp;
	CPoint pt[4];
	CPoint ptCursor;
	float fLength,fX0,fX1,fY0,fY1,fX;
	double dAngle0,dAngle1,dAngle,dHypot,dTemp;
	CFont Font;
	CString Buff,Length;
	CCoord Cp0,Cp;
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	D3DXVECTOR3 vSurface[2],vW;
	D3DXMATRIXA16 mWorld;

	if(m_nToolOption!=TOOL_RULER) return;

	CPen penBkRuler(PS_SOLID,1,GetSysColor(COLOR_INFOBK));
	CPen pen(PS_SOLID,theApp.m_LastSettings.nRulerWidth,theApp.m_LastSettings.rgbRuler);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush brushBkRuler(GetSysColor(COLOR_INFOBK));
	CBrush* pOldBrush = pDC->SelectObject(&brushBkRuler);

	int nLength=(int)sqrt((double)(m_rrRuler.Width()*m_rrRuler.Width() + m_rrRuler.Height()*m_rrRuler.Height()));

	if(pViewSheet->m_nRectPolarDisplayMode == 0) {
		switch(theApp.m_LastSettings.nRulerStyle) {
		case RULER_RECTANGLE_STYLE:
			pDC->MoveTo(m_rrRuler.TopLeft());
			pDC->LineTo(m_rrRuler.right,m_rrRuler.top);
			pDC->LineTo(m_rrRuler.BottomRight());
			pDC->LineTo(m_rrRuler.left,m_rrRuler.bottom);
			pDC->LineTo(m_rrRuler.TopLeft());
			break;
		case RULER_ELLIPSE_STYLE:
			pDC->SelectStockObject(HOLLOW_BRUSH);
			rr = m_rrRuler;
			rr.left=m_rrRuler.right-m_rrRuler.Height()/2;
			rr.right=m_rrRuler.right+m_rrRuler.Height()/2;
			pDC->Ellipse(&rr);
			break;
		default:
			switch(theApp.m_LastSettings.nRulerWidth) {
			default:
				pDC->MoveTo(m_rrRuler.TopLeft());
				pDC->LineTo(m_rrRuler.BottomRight());
				break;
			case 4:
				if(nLength) {
					theApp.m_LastSettings.lfViewGraticule.lfEscapement=(long)(atan2(m_rrRuler.Height(),m_rrRuler.Width()) * RAD_TO_DEG * -10.0);
					Font.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
					CFont* pOldFont = pDC->SelectObject(&Font);

					pt[0]=m_rrRuler.TopLeft();
					pt[1]=m_rrRuler.BottomRight();
					pt[2].x = m_rrRuler.right - RoundDownMulDiv(m_rrRuler.Height(),14,nLength);
					pt[2].y = m_rrRuler.bottom + RoundDownMulDiv(m_rrRuler.Width(),14,nLength);
					pt[3].x = m_rrRuler.left - RoundDownMulDiv(m_rrRuler.Height(),14,nLength);
					pt[3].y = m_rrRuler.top + RoundDownMulDiv(m_rrRuler.Width(),14,nLength);
					pDC->SelectObject(&penBkRuler);
					pDC->Polygon(pt,4);
					ClientToWorld(m_rrRuler.TopLeft(),&fX0,&fY0);
					ClientToWorld(m_rrRuler.BottomRight(),&fX1,&fY1);
					fLength=(float)sqrt((fX0-fX1)*(fX0-fX1) + (fY0-fY1)*(fY0-fY1));
					pDC->SelectObject(pOldPen);
					pDC->MoveTo(m_rrRuler.TopLeft());
					pDC->LineTo(m_rrRuler.BottomRight());
					float fStep = theApp.m_LastSettings.fRulerScale;
					if(fStep<0.1f) fStep=0.1f;

					fLength *= theApp.m_fUnits;
					for(fX=0.0f;fX<fLength;fX+=fStep) {
						nX=m_rrRuler.left + (int)(fX*(float)(m_rrRuler.Width())/fLength);
						nY=m_rrRuler.top + (int)(fX*(float)(m_rrRuler.Height())/fLength);
						pDC->MoveTo(nX,nY);
						Buff.Format(_T("%.0f %s"),fX,theApp.m_Units);
						pDC->TextOut(nX,nY,Buff);

						nX = nX - RoundDownMulDiv(m_rrRuler.Height(),5,nLength);
						nY = nY + RoundDownMulDiv(m_rrRuler.Width(),5,nLength);
						pDC->LineTo(nX,nY);

					}

					pDC->SelectObject(pOldFont);
				}
				break;
			} //switch
			break; //switch
		}
	} else {

		nX = m_rrRuler.left - m_rrView.CenterPoint().x;
		nY = m_rrRuler.top - m_rrView.CenterPoint().y;
		nHypot0 = (int)sqrt(nX*nX + nY*nY);
		dAngle0 = atan2((double)nY, (double)nX) * RAD_TO_DEG;
		if(dAngle0 < 0.0) dAngle0 += 360.0;

		nX = m_rrRuler.right - m_rrView.CenterPoint().x;
		nY = m_rrRuler.bottom - m_rrView.CenterPoint().y;
		nHypot1 = (int)sqrt(nX*nX + nY*nY);
		dAngle1 = atan2((double)nY, (double)nX) * RAD_TO_DEG;
		if(dAngle1 < 0.0) dAngle1 += 360.0;

		if(dAngle1==dAngle0) {
			dAngle1+=0.01;
		}

		if(dAngle1<dAngle0) {
			dTemp = dAngle1;
			dAngle1 = dAngle0;
			dAngle0 = dTemp;
			nTemp = nHypot1;
			nHypot1 = nHypot0;
			nHypot0 = nTemp;
		}
		if((dAngle1 - dAngle0) > 180.0) {
			dAngle0 += 360.0;
			dTemp = dAngle1;
			dAngle1 = dAngle0;
			dAngle0 = dTemp;
			nTemp = nHypot1;
			nHypot1 = nHypot0;
			nHypot0 = nTemp;
		}


		CPoint ptSample;
		m_fRulerLength = 0.0f;
		double dStep = (dAngle1 - dAngle0) / 100.0f;
		for(dAngle=dAngle0,nn=0;dAngle<=dAngle1;dAngle+=dStep,nn++) {
			dHypot = (double)(nHypot1-nHypot0) * (dAngle-dAngle0) / (dAngle1-dAngle0) + (double)nHypot0;
			pt[0].x = (int)(dHypot * cos(dAngle * DEG_TO_RAD)) + m_rrView.CenterPoint().x;
			pt[0].y = (int)(dHypot * sin(dAngle * DEG_TO_RAD)) + m_rrView.CenterPoint().y;
			//????
//			ClientToWorld(pt[0],&Cp);
			ClientToSample(pt[0], &ptSample);
			ptSample.y *= m_pData->m_nLineIncrement;
			ptSample.y += m_pData->m_nScanStartLine;

			m_pData->GetCoordFromLineSample(ptSample, &Cp);
			if(nn==0) {
				pDC->MoveTo(pt[0]);
			} else {
				pDC->LineTo(pt[0]);
			}
			vSurface[1] = Cp.Side0.pt - theApp.m_Tank.vTTCentre;
			D3DXMatrixRotationZ(&mWorld,Cp.Side0.fR * DEG_TO_RAD);		//Rotate about Z for horizontal turntable
			D3DXVec3TransformCoord(&vSurface[1],&vSurface[1],&mWorld);
			if(nn>0) {
				vW = vSurface[1] - vSurface[0];
				m_fRulerLength += D3DXVec3Length(&vW);
			}
			vSurface[0] = vSurface[1];
			m_fRulerLength = Cp.Side0.fY;
		}
	}

	CPoint ptCenter;
	theApp.FormatLength(m_fRulerLength,1,Length);
	switch(theApp.m_LastSettings.nRulerFlagMode) {
	case 0:
		break;
	case 1: TextBoxAtPt(pDC,m_strRuler,m_rrRuler.TopLeft());
		break;
	case 2: TextBoxAtCursor(pDC,m_strRuler);
		break;
	case 3:
		ptCenter.x = (m_rrRuler.left + m_rrRuler.right) / 2;
		ptCenter.y = (m_rrRuler.top + m_rrRuler.bottom) / 2;
		TextBoxAtPt(pDC,m_strRuler,ptCenter);
		break;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}


void CViewPage::DrawCrossSectionLine(CPaintDC* pDC)
{
	D3DXVECTOR2 ptBottom, ptTop, ptLeft, ptRight, ptD3DX0, ptD3DX1; 
	CPoint pt0, pt1, ptSample, ptPixel;

	if(m_nToolOption!=TOOL_CROSS_SECTION) return;

	CPen penBkRuler(PS_SOLID,1,GetSysColor(COLOR_INFOBK));
	CPen pen(PS_SOLID,theApp.m_LastSettings.nRulerWidth,theApp.m_LastSettings.rgbRuler);
	CPen* pOldPen = pDC->SelectObject(&pen);

	D3DXVECTOR2 Line1Pt1,Line1Pt2,Line2Pt1,Line2Pt2;

	ClientToSample(m_rrRuler.TopLeft(), &ptSample);
	Line1Pt1.x = (float)ptSample.x;
	Line1Pt1.y = (float)ptSample.y;
	ClientToSample(m_rrRuler.BottomRight(), &ptSample);
	Line1Pt2.x = (float)ptSample.x;
	Line1Pt2.y = (float)ptSample.y;

	Line2Pt1.x = 0.0f;
	Line2Pt1.y = 0.0f;
	Line2Pt2.x = (float)m_pData->m_nSamplesLine - 1.0f;
	Line2Pt2.y = 0.0f;
	Vector2IntersectionPt(Line1Pt1, Line1Pt2, Line2Pt1, Line2Pt2, &ptBottom);

	Line2Pt1.x = 0.0f;
	Line2Pt1.y = (float)m_pData->m_nNumberLines - 1.0f;
	Line2Pt2.x = (float)m_pData->m_nSamplesLine - 1.0f;
	Line2Pt2.y = (float)m_pData->m_nNumberLines - 1.0f;
	Vector2IntersectionPt(Line1Pt1, Line1Pt2, Line2Pt1, Line2Pt2, &ptTop);

	Line2Pt1.x = 0.0f;
	Line2Pt1.y = 0.0f;
	Line2Pt2.x = 0.0f;
	Line2Pt2.y = m_pData->m_nNumberLines - 1.0f;
	Vector2IntersectionPt(Line1Pt1, Line1Pt2, Line2Pt1, Line2Pt2, &ptLeft);


	Line2Pt1.x = m_pData->m_nSamplesLine - 1.0f;
	Line2Pt1.y = 0.0f;
	Line2Pt2.x = m_pData->m_nSamplesLine - 1.0f;
	Line2Pt2.y = m_pData->m_nNumberLines - 1.0f;
	Vector2IntersectionPt(Line1Pt1, Line1Pt2, Line2Pt1, Line2Pt2, &ptRight);

	bool bPt0Set = false;
	if (ptBottom.x >= 0.0f && ptBottom.x < m_pData->m_nSamplesLine) {
		ptD3DX0 = ptBottom;
		bPt0Set = true;
	}
	if (ptLeft.y >= 0.0f && ptLeft.y < m_pData->m_nNumberLines) {
		if (bPt0Set == false) {
			ptD3DX0 = ptLeft;
			bPt0Set = true;
		}
		else {
			ptD3DX1 = ptLeft;
		}
	}

	if (ptTop.x >= 0.0f && ptTop.x < m_pData->m_nSamplesLine) {
		if (bPt0Set == false) {
			ptD3DX0 = ptTop;
			bPt0Set = true;
		}
		else {
			ptD3DX1 = ptTop;
		}
	}
	if (ptRight.y >= 0.0f && ptRight.y < m_pData->m_nNumberLines) {
		if (bPt0Set == false) {
			ptD3DX0 = ptRight;
			bPt0Set = true;
		}
		else {
			ptD3DX1 = ptRight;
		}
	}

	pt0.x = (int)ptD3DX0.x;
	pt0.y = (int)ptD3DX0.y;
	pt1.x = (int)ptD3DX1.x;
	pt1.y = (int)ptD3DX1.y;
	SampleToClient(pt0,&ptPixel);
	pDC->MoveTo(ptPixel);
	SampleToClient(pt1,&ptPixel);
	pDC->LineTo(ptPixel);

	pDC->SelectObject(pOldPen);

	RefreshThicknessCrossSection(pt0.x,pt0.y, pt1.x, pt1.y);
}

void CViewPage::DrawValue(CPaintDC *pDC)
{
	CViewSheet *pViewSheet = (CViewSheet*)m_pParent;
	if(pViewSheet->m_nToolOption != TOOL_VALUE) return;

	TextBoxAtCursor(pDC,m_strValue);
}

void CViewPage::DrawPolygon(CPaintDC *pDC)
{
	CPoint pt,ptSample;
	CCoord Cp;
	CViewSheet *pViewSheet = (CViewSheet*)m_pParent;

	if(pViewSheet->m_bRenderPolygons != true) return;
	if(m_pData->m_nPolygonL <=0) return;
	if(m_pData->m_pPolygon==NULL) return;

	CPen pen(PS_SOLID,theApp.m_LastSettings.nRulerWidth,theApp.m_LastSettings.rgbRuler);
	CPen* pOldPen = pDC->SelectObject(&pen);

	for(int nP=0;nP<m_pData->m_nPolygonL;nP++) {
		for(int ii=0;ii<m_pData->m_pPolygon[nP].m_nCoordL;ii++) {
			m_pData->m_pPolygon[nP].GetCoord(ii,&Cp);
			ptSample.x = (int)Cp.Side1.fX;
			ptSample.y = (int)Cp.Side1.fY;
			SampleToClient(ptSample,&pt);
			if(ii==0) {
				pDC->MoveTo(pt);
			} else {
				pDC->LineTo(pt);
			}
		}
		if(m_pData->m_pPolygon[nP].m_nClosedEnds == 1) {
			m_pData->m_pPolygon[nP].GetCoord(0,&Cp);
			ptSample.x = (int)Cp.Side1.fX;
			ptSample.y = (int)Cp.Side1.fY;
			SampleToClient(ptSample,&pt);
			pDC->LineTo(pt);
		}
	}


	pDC->SelectObject(pOldPen);

}



void CViewPage::DrawHistogram(CPaintDC* pDC)
{
	if(m_nToolOption!=TOOL_HISTOGRAM) return;
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	CPen pen(PS_SOLID,theApp.m_LastSettings.nRulerWidth,theApp.m_LastSettings.rgbRuler);
	CPen pen1(PS_SOLID,1,theApp.m_LastSettings.rgbRuler);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush brushBkRuler(GetSysColor(COLOR_INFOBK));
	CBrush* pOldBrush = pDC->SelectObject(&brushBkRuler);
	CRect	rr, rrArc;
	CString Buff,Temp,Width,Height;
	CFont FontHorizontal,FontVertical;
	CFont* pOldFont;
	CSize size;
	float fX0,fX1,fY0,fY1,fLength,fX,fY;
	int	nX,nY;
	CPoint pt, pt2, tempPt, ptStart,ptFinish;
	int nWidth0,nHeight0,nHypot0;
	int nWidth1,nHeight1,nHypot1;
	float fFastScanSize,fFastIncrement;

	if(pViewSheet->m_nRectPolarDisplayMode == 0) {

		switch(theApp.m_LastSettings.nRulerWidth) {
		default:
			switch(theApp.m_LastSettings.nHistogramStyle){
			default:
				if(theApp.m_LastSettings.bHistogramEnterSize==0){
					pDC->MoveTo(m_rrRuler.TopLeft());
					pDC->LineTo(m_rrRuler.right,m_rrRuler.top);
					pDC->LineTo(m_rrRuler.BottomRight());
					pDC->LineTo(m_rrRuler.left,m_rrRuler.bottom);
					pDC->LineTo(m_rrRuler.TopLeft());
				} else {
					ClientToSample(m_rrRuler.BottomRight(), &pt);
					pt.x -= (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fFastIncrement);
					pt.y -= (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fSlowIncrement);
					SampleToClient(pt,&rr.TopLeft());

					ClientToSample(m_rrRuler.BottomRight(), &pt);
					pt.x += (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fFastIncrement);
					pt.y += (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fSlowIncrement);
					SampleToClient(pt,&rr.BottomRight());
					pDC->MoveTo(rr.TopLeft());
					pDC->LineTo(rr.right,rr.top);
					pDC->LineTo(rr.BottomRight());
					pDC->LineTo(rr.left,rr.bottom);
					pDC->LineTo(rr.TopLeft());
				}
				break;
			case 1:
				pDC->SelectStockObject(HOLLOW_BRUSH);
				rr = m_rrRuler;

				if(theApp.m_LastSettings.bHistogramEnterSize==0){
					//Make it a Circle on image not on screen
					ClientToSample(rr.TopLeft(), &pt);
					ClientToSample(rr.BottomRight(), &pt2);
					nY = abs(pt.y-pt2.y);
					pt.x -= (nY/2)*(int)(m_pData->m_fFastIncrement/m_pData->m_fSlowIncrement);
					pt2.x += (nY/2)*(int)(m_pData->m_fFastIncrement/m_pData->m_fSlowIncrement);
					SampleToClient(pt,&tempPt);
					rr.left = tempPt.x;
					SampleToClient(pt2,&tempPt);
					rr.right = tempPt.x;
				}else{
						//Create Circle with entered Dimension with ruler bottom right centre
					ClientToSample(m_rrRuler.BottomRight(), &pt);
					pt.x -= (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fFastIncrement);
					pt.y -= (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fSlowIncrement);
					SampleToClient(pt,&rr.TopLeft());

					ClientToSample(m_rrRuler.BottomRight(), &pt);
					pt.x += (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fFastIncrement);
					pt.y += (int)( (theApp.m_LastSettings.fHistogramCircleSize/2) / m_pData->m_fSlowIncrement);
					SampleToClient(pt,&rr.BottomRight());
				}

				//rr.left=m_rrRuler.right-m_rrRuler.Height()/2;
				//rr.right=m_rrRuler.right+m_rrRuler.Height()/2;

				pDC->Ellipse(&rr);
				break;
			}//switch
			break;
		case 4:
			theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
			FontHorizontal.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
			theApp.m_LastSettings.lfViewGraticule.lfEscapement=-900;
			FontVertical.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
			pOldFont = pDC->SelectObject(&FontHorizontal);
			Buff="100.0";
			size = pDC->GetTextExtent(Buff);
			pDC->SelectObject(pOldPen);

			rr=m_rrRuler;
			rr.NormalizeRect();
			rr.right=rr.left;
			rr.left=rr.right-size.cy;
			rr.bottom+=size.cy;
			pDC->FillRect(&rr,&brushBkRuler);
			rr=m_rrRuler;
			rr.NormalizeRect();
			rr.left=rr.left-size.cy;
			rr.top=rr.bottom;
			rr.bottom+=size.cy;
			pDC->FillRect(&rr,&brushBkRuler);

			rr=m_rrRuler;
			rr.NormalizeRect();
			pDC->MoveTo(rr.TopLeft());
			pDC->LineTo(rr.left,rr.bottom);
			pDC->LineTo(rr.BottomRight());

			rr=m_rrRuler;
			rr.NormalizeRect();
			ClientToWorld(m_rrRuler.TopLeft(),&fX0,&fY0);
			ClientToWorld(m_rrRuler.BottomRight(),&fX1,&fY1);
			fLength=(float)fabs((double)(fX1-fX0));
			for(fX=0.0f;fX<fLength;fX+=10.0f) {
				nX=rr.left + (int)(fX*(float)(rr.Width())/fLength);
				pDC->MoveTo(nX,rr.bottom);
				pDC->LineTo(nX,rr.bottom+3);
				Buff.Format(_T("%.0f"),fX);
				pDC->TextOut(nX,rr.bottom,Buff);
			}

			pDC->SelectObject(&FontVertical);
			fLength=(float)fabs(fY1-fY0);
			for(fY=0.0f;fY<fLength;fY+=10.0f) {
				nY=rr.top + (int)(fY*(float)(rr.Height())/fLength);
				pDC->MoveTo(rr.left,nY);
				pDC->LineTo(rr.left-3,nY);
				Buff.Format(_T("%.0f"),fY);
				pDC->TextOut(rr.left,nY,Buff);
			}

			pDC->SelectObject(pOldFont);
			break;
		}

		Buff.Empty();
		if(theApp.m_LastSettings.bHistogramMaxWidth) {
			pDC->SelectObject(&pen1);
			SampleToClient(m_pData->m_Hist.rrThreshWidth.TopLeft(),&pt);
			pDC->MoveTo(pt);
			SampleToClient(m_pData->m_Hist.rrThreshWidth.BottomRight(),&pt);
			pDC->LineTo(pt);

			m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);

			theApp.FormatLength((float)(m_pData->m_Hist.rrThreshWidth.Width()+1) * fFastIncrement,1,Width);
			Buff.Format(_T("Width:%s"),Width);
		}
		if(theApp.m_LastSettings.bHistogramMaxHeight) {
			pDC->SelectObject(&pen1);
			SampleToClient(m_pData->m_Hist.rrThreshHeight.TopLeft(),&pt);
			pDC->MoveTo(pt);
			SampleToClient(m_pData->m_Hist.rrThreshHeight.BottomRight(),&pt);
			pDC->LineTo(pt);
			theApp.FormatLength((float)(m_pData->m_Hist.rrThreshHeight.Height()+1) * m_pData->m_fSlowIncrement,1,Height);
			Temp.Format(_T(" Height:%s"),Height);
			Buff += Temp;
		}

		if(theApp.m_LastSettings.nHistogramStyle != HISTOGRAM_CIRCLE_STYLE)
			pViewSheet->EditTextSetWindowText(Buff);

	} else {

		m_staticView.GetWindowRect(rr);
		ScreenToClient(rr);

		pDC->MoveTo(m_rrRuler.TopLeft());

		nWidth0 = m_rrRuler.left - rr.CenterPoint().x;
		nHeight0 = m_rrRuler.top - rr.CenterPoint().y;
		nHypot0 = (int)sqrt(nWidth0*nWidth0 + nHeight0*nHeight0);
		rrArc.left = rr.CenterPoint().x - nHypot0;
		rrArc.right = rr.CenterPoint().x + nHypot0;
		rrArc.top = rr.CenterPoint().y - nHypot0;
		rrArc.bottom = rr.CenterPoint().y + nHypot0;
		ptStart=m_rrRuler.TopLeft();
		ptFinish=m_rrRuler.BottomRight();
		pDC->ArcTo(rrArc,ptStart,ptFinish);

		pDC->LineTo(m_rrRuler.BottomRight());
		nWidth1 = m_rrRuler.right - rr.CenterPoint().x;
		nHeight1 = m_rrRuler.bottom - rr.CenterPoint().y;
		nHypot1 = (int)sqrt(nWidth1*nWidth1 + nHeight1*nHeight1);
		rrArc.left = rr.CenterPoint().x - nHypot1;
		rrArc.right = rr.CenterPoint().x + nHypot1;
		rrArc.top = rr.CenterPoint().y - nHypot1;
		rrArc.bottom = rr.CenterPoint().y + nHypot1;
		ptStart=m_rrRuler.TopLeft();
		ptFinish=m_rrRuler.BottomRight();
		pDC->Arc(rrArc,ptStart,ptFinish);

		pDC->MoveTo(m_rrRuler.TopLeft());
		pt.x = MulDiv(nWidth0,nHypot1,nHypot0) + rr.CenterPoint().x;
		pt.y = MulDiv(nHeight0,nHypot1,nHypot0) + rr.CenterPoint().y;
		pDC->LineTo(pt);

		Buff.Empty();
	}


	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void CViewPage::DrawZoomVariable(CPaintDC* pDC)
{
	if(m_nToolOption!=TOOL_ZOOM_VARIABLE) return;
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	CPen pen(PS_SOLID,theApp.m_LastSettings.nRulerWidth,theApp.m_LastSettings.rgbRuler);
	CPen pen1(PS_SOLID,1,theApp.m_LastSettings.rgbRuler);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush brushBkRuler(GetSysColor(COLOR_INFOBK));
	CBrush* pOldBrush = pDC->SelectObject(&brushBkRuler);
	CRect	rr;
	CString Buff,Temp,Width,Height;
	CFont FontHorizontal,FontVertical;
	CSize size;
	CPoint pt, pt2, tempPt;

	pDC->MoveTo(m_rrRuler.TopLeft());
	pDC->LineTo(m_rrRuler.right,m_rrRuler.top);
	pDC->LineTo(m_rrRuler.BottomRight());
	pDC->LineTo(m_rrRuler.left,m_rrRuler.bottom);
	pDC->LineTo(m_rrRuler.TopLeft());

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void CViewPage::ClientToWorld(CPoint pt, float *fX, float *fY, bool bPlusStartCoordinate /* = false */)
{
	CViewSheet* pParent = (CViewSheet*)m_pParent;
	float fFastScanSize,fFastIncrement;

	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);

	*fX =			 fFastIncrement * (float)m_prrSamples->Width();
	*fY = m_pData->m_fSlowIncrement * (float)m_prrSamples->Height();

	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		if(pParent->m_nScanLineDirection==HORIZONTAL){
			*fX = *fX * (float)(pt.x - m_rrView.left) / (float)(m_rrView.Width()-1) + ((float)m_prrSamples->left * fFastIncrement);
			*fY = *fY * (float)(pt.y - m_rrView.top) / (float)(m_rrView.Height()-1) + ((float)m_prrSamples->top * m_pData->m_fSlowIncrement);
		}else{
			*fX = *fX * (float)(pt.y - m_rrView.left) / (float)(m_rrView.Height()-1) + ((float)m_prrSamples->left * m_pData->m_fSlowIncrement);
			*fY = *fY * (float)(pt.x - m_rrView.top) / (float)(m_rrView.Width()-1) + ((float)m_prrSamples->top * fFastIncrement);
		}
		break;
	case 1:
		*fX = *fX * (float)(pt.x - m_rrView.left) / (float)(m_rrView.Width()-1) + ((float)m_prrSamples->left * fFastIncrement);
		*fY = *fY * (float)(m_rrView.bottom - pt.y) / (float)(m_rrView.Height()-1) + ((float)m_prrSamples->top * m_pData->m_fSlowIncrement);
		break;
	case 2:
		*fX = *fX * (float)(m_rrView.right - pt.x) / (float)(m_rrView.Width()-1) + ((float)m_prrSamples->left * fFastIncrement);
		*fY = *fY * (float)(pt.y - m_rrView.top) / (float)(m_rrView.Height()-1) + ((float)m_prrSamples->top * m_pData->m_fSlowIncrement);
		break;
	case 3:
		*fX = *fX * (float)(m_rrView.right - pt.x) / (float)(m_rrView.Width()-1) + ((float)m_prrSamples->left * fFastIncrement);
		*fY = *fY * (float)(m_rrView.bottom - pt.y) / (float)(m_rrView.Height()-1) + ((float)m_prrSamples->top * m_pData->m_fSlowIncrement);
		break;
	}

	if(bPlusStartCoordinate == true) {
		switch(m_pData->m_nFastAxis) {
		case 0:	*fX += m_pData->m_CpStart.Side0.fX;
			break;
		case 1:	*fX += m_pData->m_CpStart.Side0.fY;
			break;
		case 2:	*fX += m_pData->m_CpStart.Side0.fZ;
			break;
		}
		switch(m_pData->m_nSlowAxis) {
		case 0:	*fY += m_pData->m_CpStart.Side0.fX;
			break;
		case 1:	*fY += m_pData->m_CpStart.Side0.fY;
			break;
		case 2:	*fY += m_pData->m_CpStart.Side0.fZ;
			break;
		}
	}


}

void CViewPage::WorldToClient(CCoord Cp,CPoint *pt)
{
	float fFastScanSize,fFastIncrement;
	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);

	float fRTotal;
	float fX=0.0f,fY=0.0f;
	float f0,f1,f2;
	float fWidth =			   fFastIncrement * (float)m_prrSamples->Width();
	float fHeight = /*m_pData->m_fSlowIncrement */ (float)(m_pData->m_CpFinish.Side0.fZ - m_pData->m_CpStart.Side0.fZ) * (float)((float)m_prrSamples->Height()/(float)m_pData->m_nNumberLines); //(float)m_prrSamples->Height();
	CViewSheet* pParent = (CViewSheet*)m_pParent;
	int nLine,nNearestLine;
	float fDistance,fMinDistance;
	CCoord CpScan;
	float fRadius, fAngle, fXScreen, fYScreen, fMaxRadius;


	switch(m_pData->m_nFastAxis) {
	default:
		fX = Cp.Side0.fX - m_pData->m_CpStart.Side0.fX;
		f0 = Cp.Side0.fY - m_pData->m_CpStart.Side0.fY;
		f1 = Cp.Side0.fZ - m_pData->m_CpStart.Side0.fZ;
		switch(m_pData->m_nSlowAxis) {
		case 1:
			fY = f0;//(float)hypot(f0,f1);
			break;
		default:
		case 2:
			fY = f1;//(float)hypot(f0,f1); //FY is distance in Z, we need surface distance of that point			
			break;
		}
		break;
	case 1: fX = Cp.Side0.fY - m_pData->m_CpStart.Side0.fY;
		f0 = Cp.Side0.fX - m_pData->m_CpStart.Side0.fX;
		f1 = Cp.Side0.fZ - m_pData->m_CpStart.Side0.fZ;
		fY = (float)hypot(f0,f1);
		break;
	case 2: fX = Cp.Side0.fZ - m_pData->m_CpStart.Side0.fZ;
		f0 = Cp.Side0.fX - m_pData->m_CpStart.Side0.fX;
		f1 = Cp.Side0.fY - m_pData->m_CpStart.Side0.fY;
		fY = (float)hypot(f0,f1);
		break;
	case 5:
		switch(theApp.m_nUseTurntableOrRollers) {
		default: fRTotal = m_pData->m_CpFinish.Side0.fR - m_pData->m_CpStart.Side0.fR;
			fX = ((Cp.Side0.fR-m_pData->m_CpStart.Side0.fR) * fFastScanSize) / fRTotal;
			break;
		case 1: fRTotal = m_pData->m_CpFinish.Side0.fR - m_pData->m_CpStart.Side0.fR;
			fX = ((Cp.Side1.fR-m_pData->m_CpStart.Side0.fR) * fFastScanSize) / fRTotal;
			break;
		};
		f0 = Cp.Side0.fX - m_pData->m_CpStart.Side0.fX;
		f1 = Cp.Side0.fY - m_pData->m_CpStart.Side0.fY;
		f2 = m_pData->m_CpFinish.Side0.fZ - Cp.Side0.fZ;
		fY = sqrtf(f0*f0 + f1*f1 + f2*f2);

		fMinDistance=1000000.0f;
		nNearestLine = 0;
		if(PROFILE->m_ScanLine) {
			for(nLine=0;nLine<PROFILE->m_ScanLine[0].m_nCoordL;nLine++) {
				PROFILE->m_ScanLine[0].GetCoord(nLine,&CpScan);
				fDistance = CCoord::Length(Cp, CpScan,0);
				if(fDistance < fMinDistance) {
					fMinDistance = fDistance;
					nNearestLine = nLine;
				}
			}
			fHeight = PROFILE->m_ScanLine[0].m_nCoordL * m_pData->m_fSlowIncrement;
		}
		fY = nNearestLine * m_pData->m_fSlowIncrement;

		break;
	}

	switch(m_pData->m_nFastAxis) {
	default:
		switch(theApp.m_LastSettings.nImageOrientation) {
		case 0:
			if(pParent->m_nScanLineDirection==HORIZONTAL){
			}else{
				pt->x = m_rrView.left + (int)((fY - ((float)m_prrSamples->left * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Width()-1) / fHeight);
				pt->y = m_rrView.top  + (int)((fX - ((float)m_prrSamples->top *				fFastIncrement)) * (float)(m_rrView.Height()-1) / fWidth);
			}

			break;
		case 1:
			pt->x = m_rrView.left + (int)((fX - ((float)m_prrSamples->left *			fFastIncrement)) * (float)(m_rrView.Width()-1) / fWidth);
			pt->y = m_rrView.bottom - (int)((fY - ((float)m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Height()-1) / fHeight);
			break;
		case 2:
			pt->x = m_rrView.right - (int)((fX - ((float)m_prrSamples->left *			fFastIncrement)) * (float)(m_rrView.Width()-1) / fWidth);
			pt->y = m_rrView.top  + (int)((fY - ((float)m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Height()-1) / fHeight);
			break;
		case 3:
			pt->x = m_rrView.right - (int)((fX - ((float)m_prrSamples->left *			fFastIncrement)) * (float)(m_rrView.Width()-1) / fWidth);
			pt->y =m_rrView.bottom - (int)((fY - ((float)m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Height()-1) / fHeight);
			break;
		}
		break;
	case 5:
		//??????
		if (pParent->m_nRectPolarDisplayMode == 0) {	//Rectilinear

			switch (theApp.m_LastSettings.nImageOrientation) {
			default:
				pt->x = m_rrView.left + (int)((fX - ((float)m_prrSamples->left *			fFastIncrement)) * (float)(m_rrView.Width() - 1) / fWidth);
				pt->y = m_rrView.top + (int)((fY - ((float)m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Height() - 1) / fHeight);
				break;
			case 1:
				pt->x = m_rrView.left + (int)((fX - ((float)m_prrSamples->left *			fFastIncrement)) * (float)(m_rrView.Width() - 1) / fWidth);
				pt->y = m_rrView.bottom - (int)((fY - ((float)m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Height() - 1) / fHeight);
				break;
			case 3:
				pt->x = m_rrView.left + (int)((fX - ((float)m_prrSamples->left *			fFastIncrement)) * (float)(m_rrView.Width() - 1) / fWidth);
				pt->y = m_rrView.bottom - (int)((fY - ((float)m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Height() - 1) / fHeight);
				break;
			};
		}
		else {
			if (m_pData->m_fComponentDiameter[1] >= m_pData->m_fComponentDiameter[0]) {
				fMaxRadius = m_pData->m_fComponentDiameter[1] / 2.0f;
				fRadius = fY + m_pData->m_fComponentDiameter[0] / 2.0f;
			}
			else {
				fMaxRadius = m_pData->m_fComponentDiameter[0] / 2.0f;
				fRadius = m_pData->m_fComponentDiameter[0] / 2.0f - fY;
			}
			fAngle = fX * DEG_TO_RAD;
			fXScreen = fRadius * cosf(fAngle) / fMaxRadius;
			fYScreen = fRadius * sinf(fAngle) / fMaxRadius;
			pt->x = m_rrView.CenterPoint().x - (int)((float)(m_rrView.Height() / 2.0f) * fXScreen);
			pt->y = m_rrView.CenterPoint().y - (int)((float)(m_rrView.Height() / 2.0f) * fYScreen);
		}
		break;
	}

}

void CViewPage::ComponentOffsetToClient(float fX, float fY,CPoint *pt)
{
	float fFastScanSize,fFastIncrement;

	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);

	float fWidth =			   fFastIncrement * (float)m_prrSamples->Width();
	float fHeight = m_pData->m_fSlowIncrement * (float)m_prrSamples->Height();

	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		pt->x = m_rrView.left + (int)((fX - ((float)m_prrSamples->left *		   fFastIncrement)) * (float)(m_rrView.Width()-1) / fWidth);
		pt->y = m_rrView.top  + (int)((fY - ((float)m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Height()-1) / fHeight);
		break;
	case 1:
		pt->x = m_rrView.left + (int)((fX - ((float)m_prrSamples->left *			fFastIncrement)) * (float)(m_rrView.Width()-1) / fWidth);
		pt->y =m_rrView.bottom - (int)((fY - ((float)m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Height()-1) / fHeight);
		break;
	case 2:
		pt->x = m_rrView.right - (int)((fX - ((float)m_prrSamples->left *			fFastIncrement)) * (float)(m_rrView.Width()-1) / fWidth);
		pt->y = m_rrView.top  + (int)((fY - ((float)m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Height()-1) / fHeight);
		break;
	case 3:
		pt->x = m_rrView.right - (int)((fX - ((float)m_prrSamples->left *			fFastIncrement)) * (float)(m_rrView.Width()-1) / fWidth);
		pt->y =m_rrView.bottom - (int)((fY - ((float)m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(m_rrView.Height()-1) / fHeight);
		break;
	}
}

void CViewPage::ClientToSample(CPoint ptPixel,CPoint* pPtSample)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	int nPixelRadius, nX, nY, nMinPixelRadius;
	float fRadius, fAngle, fMaxRadius,fMinRadius;

	if(pViewSheet->m_nRectPolarDisplayMode == 0) {

		switch(theApp.m_LastSettings.nImageOrientation) {
		case 0:
			pPtSample->x = RoundDownMulDiv(ptPixel.x - m_rrView.left,m_prrSamples->Width(),m_rrView.Width()-1) + m_prrSamples->left;
			pPtSample->y = RoundDownMulDiv(ptPixel.y - m_rrView.top,m_prrSamples->Height(),m_rrView.Height()-1) + m_prrSamples->top;
			break;
		case 1:
			pPtSample->x = RoundDownMulDiv(ptPixel.x - m_rrView.left,m_prrSamples->Width(),m_rrView.Width()-1) + m_prrSamples->left;
			pPtSample->y = RoundDownMulDiv(m_rrView.bottom - ptPixel.y,m_prrSamples->Height(),m_rrView.Height()-1) + m_prrSamples->top;
			break;
		case 2:
			pPtSample->x = RoundDownMulDiv(m_rrView.right - ptPixel.x,m_prrSamples->Width(),m_rrView.Width()-1) + m_prrSamples->left;
			pPtSample->y = RoundDownMulDiv(ptPixel.y - m_rrView.top,m_prrSamples->Height(),m_rrView.Height()-1) + m_prrSamples->top;
			break;
		case 3:
			pPtSample->x = RoundDownMulDiv(m_rrView.right - ptPixel.x,m_prrSamples->Width(),m_rrView.Width()-1) + m_prrSamples->left;
			pPtSample->y = RoundDownMulDiv(m_rrView.bottom - ptPixel.y,m_prrSamples->Height(),m_rrView.Height()-1) + m_prrSamples->top;
			break;
		}
	} else {
		nX = m_rrView.CenterPoint().x - ptPixel.x;
		nY = m_rrView.CenterPoint().y - ptPixel.y;
		fRadius = sqrtf((float)(nX*nX + nY*nY));

		m_rrView.Height() < m_rrView.Width() ? nPixelRadius = m_rrView.Height() / 2 : nPixelRadius = m_rrView.Width() / 2;
		if (m_pData->m_fComponentDiameter[0] < m_pData->m_fComponentDiameter[1]) {
			fMaxRadius = m_pData->m_fComponentDiameter[1] / 2.0f;
			fMinRadius = m_pData->m_fComponentDiameter[0] / 2.0f;
			nMinPixelRadius = (int)(fMinRadius * (float)nPixelRadius / fMaxRadius);
			pPtSample->y = (int)(((float)nPixelRadius - fRadius) * (float)m_pData->m_nNumberLines / (float)(nPixelRadius - nMinPixelRadius));
			pPtSample->y = m_pData->m_nNumberLines - pPtSample->y;
		}
		else {
			fMaxRadius = m_pData->m_fComponentDiameter[0] / 2.0f;
			fMinRadius = m_pData->m_fComponentDiameter[1] / 2.0f;
			nMinPixelRadius = (int)(fMinRadius * (float)nPixelRadius / fMaxRadius);
			pPtSample->y = (int)(((float)nPixelRadius - fRadius) * (float)m_pData->m_nNumberLines / (float)(nPixelRadius - nMinPixelRadius));
		}
		MinMax(&pPtSample->y, 0, m_pData->m_nNumberLines-1);

		fAngle = atan2f((float)nY, (float)nX) * RAD_TO_DEG;
		if(fAngle < 0.0) fAngle += 360.0;

		pPtSample->x = (int)(fAngle * (float)(m_pData->m_nSamplesLine - 1) / 360.0f);
		MinMax(&pPtSample->x, 0, m_pData->m_nSamplesLine-1);
	}

}

void CViewPage::SampleToClient(CPoint ptSample,CPoint* pPtPixel)
{
	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		pPtPixel->x = m_rrView.left + RoundDownMulDiv(ptSample.x-m_prrSamples->left,m_rrView.Width()-1,m_prrSamples->Width()-1);
		pPtPixel->y = m_rrView.top + RoundDownMulDiv(ptSample.y-m_prrSamples->top,m_rrView.Height()-1,m_prrSamples->Height()-1);
		break;
	case 1:
		pPtPixel->x = m_rrView.left + RoundDownMulDiv(ptSample.x-m_prrSamples->left,m_rrView.Width()-1,m_prrSamples->Width()-1);
		pPtPixel->y = m_rrView.bottom - RoundDownMulDiv(ptSample.y-m_prrSamples->top,m_rrView.Height()-1,m_prrSamples->Height()-1);
		break;
	case 2:
		pPtPixel->x = m_rrView.right - RoundDownMulDiv(ptSample.x-m_prrSamples->left,m_rrView.Width()-1,m_prrSamples->Width()-1);
		pPtPixel->y = m_rrView.top + RoundDownMulDiv(ptSample.y-m_prrSamples->top,m_rrView.Height()-1,m_prrSamples->Height()-1);
		break;
	case 3:
		pPtPixel->x = m_rrView.right - RoundDownMulDiv(ptSample.x-m_prrSamples->left,m_rrView.Width()-1,m_prrSamples->Width()-1);
		pPtPixel->y = m_rrView.bottom - RoundDownMulDiv(ptSample.y-m_prrSamples->top,m_rrView.Height()-1,m_prrSamples->Height()-1);
		break;
	}
}

void CViewPage::ClientToSample(CPoint ptPixel,float *pfSample, float *pfLine)
{
	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		*pfSample =	(float)(ptPixel.x - m_rrView.left) * (float)m_prrSamples->Width() / (float)(m_rrView.Width()-1) + (float)m_prrSamples->left;
		*pfLine =	(float)(ptPixel.y - m_rrView.top) * (float)(m_prrSamples->Height()-1) / (float)(m_rrView.Height()-1) + (float)m_prrSamples->top;
		break;
	case 1:
		*pfSample = (float)(ptPixel.x - m_rrView.left) * (float)m_prrSamples->Width() / (float)(m_rrView.Width()-1) + (float)m_prrSamples->left;
		*pfLine = (float)(m_rrView.bottom - ptPixel.y) * (float)(m_prrSamples->Height()-1) / (float)(m_rrView.Height()-1) + (float)m_prrSamples->top;
		break;
	case 2:
		*pfSample = (float)(m_rrView.right - ptPixel.x) * (float)m_prrSamples->Width() / (float)(m_rrView.Width()-1) + (float)m_prrSamples->left;
		*pfLine = (float)(ptPixel.y - m_rrView.top) * (float)(m_prrSamples->Height()-1) / (float)(m_rrView.Height()-1) + (float)m_prrSamples->top;
		break;
	case 3:
		*pfSample = (float)(m_rrView.right - ptPixel.x) * (float)m_prrSamples->Width() / (float)(m_rrView.Width()-1) + (float)m_prrSamples->left;
		*pfLine = (float)(m_rrView.bottom - ptPixel.y) * (float)(m_prrSamples->Height()-1) / (float)(m_rrView.Height()-1) + (float)m_prrSamples->top;
		break;
	}
}

void CViewPage::SampleToClient(float fSample, float fLine,CPoint* pPtPixel)
{
	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		pPtPixel->x = m_rrView.left + (int)((fSample-(float)m_prrSamples->left) * (float)(m_rrView.Width()-1) / (float)(m_prrSamples->Width()-1));
		pPtPixel->y = m_rrView.top + (int)((fLine-(float)m_prrSamples->top) * (float)(m_rrView.Height()-1) / (float)(m_prrSamples->Height()-1));
		break;
	case 1:
		pPtPixel->x = m_rrView.left + (int)((fSample-(float)m_prrSamples->left) * (float)(m_rrView.Width()-1) / (float)(m_prrSamples->Width()-1));
		pPtPixel->y = m_rrView.bottom - (int)((fLine-(float)m_prrSamples->top) * (float)(m_rrView.Height()-1) / (float)(m_prrSamples->Height()-1));
		break;
	case 2:
		pPtPixel->x = m_rrView.right - (int)((fSample-(float)m_prrSamples->left) * (float)(m_rrView.Width()-1) / (float)(m_prrSamples->Width()-1));
		pPtPixel->y = m_rrView.top + (int)((fLine-(float)m_prrSamples->top) * (float)(m_rrView.Height()-1) / (float)(m_prrSamples->Height()-1));
		break;
	case 3:
		pPtPixel->x = m_rrView.right - (int)((fSample-(float)m_prrSamples->left) * (float)(m_rrView.Width()-1) / (float)(m_prrSamples->Width()-1));
		pPtPixel->y = m_rrView.bottom - (int)((fLine-(float)m_prrSamples->top) * (float)(m_rrView.Height()-1) / (float)(m_prrSamples->Height()-1));
		break;
	}
}

BOOL CViewPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	CPoint	point;
	float fPixelSize;
	int nCursorSize;
	GetCursorPos(&point);
	ScreenToClient(&point);

	switch(m_nToolOption) {
	case 0:
		if((m_bLButtonDown == true) && (m_nMoveCurrentCADElement == 0)){
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			return true;
		}
		break;
	case TOOL_RULER:
		break;
	case TOOL_HISTOGRAM:
		break;
	case TOOL_VALUE:
		if(m_rrView.PtInRect(point)) {
			fPixelSize = (float)m_prrSamples->Width() / (float)m_rrView.Width()  * m_pData->m_fFastIncrement;
			nCursorSize = (int)(theApp.m_LastSettings.fValueCursorSize / fPixelSize);
			if(nCursorSize<=0) nCursorSize = 1;
			if(nCursorSize>10) nCursorSize = 10;

			switch(nCursorSize) {
			default: SetCursor(m_hcursorValue1);
				break;
			case 2: SetCursor(m_hcursorValue2);
				break;
			case 3: SetCursor(m_hcursorValue3);
				break;
			case 4: SetCursor(m_hcursorValue4);
				break;
			case 5: SetCursor(m_hcursorValue5);
				break;
			case 6: SetCursor(m_hcursorValue6);
				break;
			case 7: SetCursor(m_hcursorValue7);
				break;
			case 8: SetCursor(m_hcursorValue8);
				break;
			case 9: SetCursor(m_hcursorValue9);
				break;
			case 10: SetCursor(m_hcursorValue10);
				break;
			}
			return TRUE;
		}
		break;
	case TOOL_CAD:
		if(m_rrView.PtInRect(point)) {
			switch(theApp.m_nCADTool) {
			case CAD_MOUSE:
				break;
			case CAD_LINE: SetCursor(m_hcursorValue1);
				break;
			case CAD_ELLIPSE: SetCursor(m_hcursorValue1);
				break;
			case CAD_MULTI_LINE: SetCursor(m_hcursorValue1);
				break;
			}
			return TRUE;
		}
		break;
	case TOOL_ELLIPSE:
	case TOOL_RECTANGLE:
	case TOOL_LINE:
	case TOOL_CENTRELINE:
	case TOOL_PARALLELOGRAM:
		if(m_bLButtonDown == false) {
			if(m_rrCursor[0].PtInRect(point)) {
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
				 m_nMoveCursor=0;
				 m_nMoveCurrentCADElement = 1;
				return TRUE;
			}
			if(m_rrCursor[1].PtInRect(point)) {
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				m_nMoveCursor=1;
				 m_nMoveCurrentCADElement = 1;
				return TRUE;
			}
			if(m_rrCursor[2].PtInRect(point)) {
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
				m_nMoveCursor=2;
				 m_nMoveCurrentCADElement = 1;
				return TRUE;
			}
			if(m_rrCursor[3].PtInRect(point)) {
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				m_nMoveCursor=3;
				 m_nMoveCurrentCADElement = 1;
				return TRUE;
			}
			if(m_rrCursor[4].PtInRect(point)) {
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
				m_nMoveCursor=4;
				 m_nMoveCurrentCADElement = 1;
				return TRUE;
			}
			if(m_rrCursor[5].PtInRect(point)) {
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				m_nMoveCursor=5;
				 m_nMoveCurrentCADElement = 1;
				return TRUE;
			}
			if(m_rrCursor[6].PtInRect(point)) {
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
				m_nMoveCursor=6;
				 m_nMoveCurrentCADElement = 1;
				return TRUE;
			}
			if(m_rrCursor[7].PtInRect(point)) {
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				m_nMoveCursor=7;
				 m_nMoveCurrentCADElement = 1;
				return TRUE;
			}
			if(m_rrCursor[9].PtInRect(point)) {
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
				m_nMoveCursor=8;
				 m_nMoveCurrentCADElement = 1;
				return TRUE;
			}
			m_nMoveCurrentCADElement = 0;
		} else {
			switch(m_nMoveCursor) {
			case 0:
			case 4: SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
				return TRUE;
				break;
			case 1:
			case 5: SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				return TRUE;
				break;
			case 2:
			case 6: SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
				return TRUE;
				break;
			case 3:
			case 7: SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				return TRUE;
				break;
			case 8: SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
				return TRUE;
				break;
			}
		}
		break;
	}

	return CResizablePage::OnSetCursor(pWnd, nHitTest, message);
}

void CViewPage::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu* menu = NULL;
	CMenu* pPopup = NULL;
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	CPoint pt = point;
	CRect rr;
	UINT id;
	CString Buff,Language, strGoto;
	menu = (CMenu *) new CMenu;

	ScreenToClient(&point);
	m_ptClient = point;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);


	switch(m_nToolOption) {
	default:
		switch(theApp.m_Tank.nScannerDescription) {
		default:
			menu->LoadMenu(IDR_RB_VIEW_PAGE_MENU);
			pPopup = menu->GetSubMenu(0);
			if(theApp.m_LastSettings.bUseBrainCompensation==TRUE) {
				id = pPopup->GetMenuItemID(5);
				Language.LoadString(IDS_Goto_Left);
				Buff.LoadString(IDS_Brain);
				pPopup->ModifyMenu(id,MF_BYCOMMAND,id,Language + _T(" + ") + Buff);

				id = pPopup->GetMenuItemID(6);
				Language.LoadString(IDS_Goto_Right);
				Buff.LoadString(IDS_Brain);
				pPopup->ModifyMenu(id,MF_BYCOMMAND,id,Language + _T(" + ") + Buff);

				id = pPopup->GetMenuItemID(7);
				Language.LoadString(IDS_Goto_Both);
				Buff.LoadString(IDS_Brain);
				pPopup->ModifyMenu(id,MF_BYCOMMAND,id,Language + _T(" + ") + Buff);
			}
			if (!(theApp.m_Tank.nMachineFeatures & _MACHINE_SIDE1)) {
				pPopup->EnableMenuItem(ID_VIEW_GOTORIGHT, MF_GRAYED | MF_BYCOMMAND);
				pPopup->EnableMenuItem(ID_VIEW_GOTOBOTH, MF_GRAYED | MF_BYCOMMAND);
			}

			pPopup->EnableMenuItem( ID_VIEW_COPYAREATOENVELOPE   , MF_GRAYED|MF_BYCOMMAND);
			if(IsPtInAnnotation(m_ptClient) == -1) pPopup->EnableMenuItem( ID_VIEW_DELETEANNOTATION   , MF_GRAYED|MF_BYCOMMAND);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
			break;

		case SINGLE_BRIDGE_TANK_WITH_R:
		case SINGLE_BRIDGE_TANK:
			menu->LoadMenu(IDR_RB_VIEW_PAGE_SINGLE_BRIDGE_MENU);
			pPopup = menu->GetSubMenu(0);
			if(theApp.m_LastSettings.bUseBrainCompensation==TRUE) {
				id = pPopup->GetMenuItemID(0);
				Language.LoadString(IDS_Goto_Left);
				Buff.LoadString(IDS_Brain);
				pPopup->ModifyMenu(id,MF_BYCOMMAND,id,Language + _T(" + ") + Buff);
			}
			if(pViewSheet->m_nRectPolarDisplayMode == 1) {
				pPopup->EnableMenuItem( ID_VIEW_GOTOLEFT   , MF_GRAYED|MF_BYCOMMAND);
				pPopup->EnableMenuItem( ID_VIEW_GOTOTAUGHTCOORD   , MF_GRAYED|MF_BYCOMMAND);
				pPopup->EnableMenuItem( ID_VIEW_ANNOTATE   , MF_GRAYED|MF_BYCOMMAND);
				pPopup->EnableMenuItem( ID_VIEW_UPDATEBRANECOORDINATE   , MF_GRAYED|MF_BYCOMMAND);
				pPopup->EnableMenuItem( ID_VIEW_ANNOTATEHISTOGRAMDATA   , MF_GRAYED|MF_BYCOMMAND);
				pPopup->EnableMenuItem( ID_VIEW_3DLOOKATPOINT   , MF_GRAYED|MF_BYCOMMAND);
			}

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
			break;
		case SINGLE_ROBOT:
			menu->LoadMenu(IDR_RB_VIEW_PAGE_SINGLE_ROBOT_MENU);
			pPopup = menu->GetSubMenu(0);
			if(theApp.m_LastSettings.nGotoWithStepOff) {
				id = pPopup->GetMenuItemID(1);
				pPopup->ModifyMenu(id,MF_BYCOMMAND,id,L"Goto here with step off");
			};
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
			break;
		case DUAL_ROBOT:
		case DUAL_ROBOT_9_PLUS_9:
		case SPX_ROBOT:
			menu->LoadMenu(IDR_RB_VIEW_PAGE_MENU);
			pPopup = menu->GetSubMenu(0);

			strGoto.LoadString(IDS_Goto);
			Buff.Format(L"%s %s", strGoto, theApp.m_DlgSideName[0]);
			pPopup->ModifyMenu(ID_VIEW_GOTOLEFT,MF_BYCOMMAND,ID_VIEW_GOTOLEFT,Buff);
			Buff.Format(L"%s %s", strGoto, theApp.m_DlgSideName[1]);
			pPopup->ModifyMenu(ID_VIEW_GOTORIGHT,MF_BYCOMMAND,ID_VIEW_GOTORIGHT,Buff);
			Buff.Format(L"%s %s + %s", strGoto, theApp.m_DlgSideName[0], theApp.m_DlgSideName[1]);
			pPopup->ModifyMenu(ID_VIEW_GOTOBOTH,MF_BYCOMMAND,ID_VIEW_GOTOBOTH,Buff);

			pPopup->EnableMenuItem( ID_VIEW_COPYAREATOENVELOPE   , MF_GRAYED|MF_BYCOMMAND);
			if(IsPtInAnnotation(m_ptClient) == -1) pPopup->EnableMenuItem( ID_VIEW_DELETEANNOTATION   , MF_GRAYED|MF_BYCOMMAND);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
			break;
		case TRACKED_PROBE:
			menu->LoadMenu(IDR_RB_VIEW_PAGE_MENU);
			pPopup = menu->GetSubMenu(0);
			pPopup->EnableMenuItem(ID_VIEW_COPYAREATOENVELOPE, MF_GRAYED | MF_BYCOMMAND);
			if (IsPtInAnnotation(m_ptClient) == -1) pPopup->EnableMenuItem(ID_VIEW_DELETEANNOTATION, MF_GRAYED | MF_BYCOMMAND);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
			break;
		}
		break;
	case TOOL_ZOOM_VARIABLE:
		menu->LoadMenu(IDR_RB_ZOOM_MENU);
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
		break;
	case TOOL_POLYGON:
		menu->LoadMenu(IDR_RB_VIEW_POLYGON_MENU);
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
		break;
	case TOOL_RULER:
	case TOOL_HISTOGRAM:
		break;
	case TOOL_CENTRELINE:
	case TOOL_RECTANGLE:
	case TOOL_ELLIPSE:
	case TOOL_LINE:
	case TOOL_PARALLELOGRAM:
		menu->LoadMenu(IDR_RB_CAD_MENU);
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
		break;
	}

	SAFE_DELETE( menu );

}

void CViewPage::OnViewPalette1()
{
	m_nPaletteNumber=0;
	m_bPalette=TRUE;
	m_bRenderView=TRUE;
	m_bInitialize=TRUE;
	Invalidate(FALSE);

	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);

}

void CViewPage::OnViewPalette2()
{
	m_nPaletteNumber=1;
	m_bPalette=TRUE;
	m_bRenderView=TRUE;
	m_bInitialize=TRUE;
	Invalidate(FALSE);
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
}

void CViewPage::OnViewPalette3()
{
	m_nPaletteNumber=2;
	m_bPalette=TRUE;
	m_bRenderView=TRUE;
	m_bInitialize=TRUE;
	Invalidate(FALSE);
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
}

void CViewPage::OnViewPalette4()
{
	m_nPaletteNumber=3;
	m_bPalette=TRUE;
	m_bRenderView=TRUE;
	m_bInitialize=TRUE;
	Invalidate(FALSE);
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
}

void CViewPage::OnViewPalette5()
{
	m_nPaletteNumber=4;
	m_bPalette=TRUE;
	m_bRenderView=TRUE;
	m_bInitialize=TRUE;
	Invalidate(FALSE);
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
}

void CViewPage::OnViewPalette6()
{
	m_nPaletteNumber=5;
	m_bPalette=TRUE;
	m_bRenderView=TRUE;
	m_bInitialize=TRUE;
	Invalidate(FALSE);
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
}

void CViewPage::OnViewPalette7()
{
	m_nPaletteNumber=6;
	m_bPalette=TRUE;
	m_bRenderView=TRUE;
	m_bInitialize=TRUE;
	Invalidate(FALSE);
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
}

void CViewPage::OnViewPalette8()
{
	m_nPaletteNumber=7;
	m_bPalette=TRUE;
	m_bRenderView=TRUE;
	m_bInitialize=TRUE;
	Invalidate(FALSE);
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
}

void CViewPage::OnViewAnnotate()
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	float fX,fY;
	CPoint pt;

	int	nEditAnnotate=IsPtInAnnotation(m_ptClient);

	ClientToWorld(m_ptClient, &fX,&fY);

	CAnnotateDlg dlg(this,m_pData,fX,fY,&m_ptClient,nEditAnnotate,m_nImageType,m_nImageNumber);
	pViewSheet->m_bRenderAnnotation=TRUE;
	pViewSheet->SetToolBarCheckedState();
	dlg.DoModal();

}

int CViewPage::IsPtInAnnotation(CPoint pt)
{
	CString Buff;
	CSize size;
	CRect rr;
	CDC* pDC = GetDC();
	CPoint ptText;
	int	nEditAnnotate=-1;

	for(int ii=0;ii<m_pData->m_nAnnotationL;ii++) {
		CFont* pFont = (CFont *) new CFont;
		pFont->CreateFontIndirect(&m_pData->m_pAnnotation[ii].lf);
		CFont* pOldFont = pDC->SelectObject(pFont);

		ComponentOffsetToClient(m_pData->m_pAnnotation[ii].fXp,m_pData->m_pAnnotation[ii].fYp, &ptText);

		Buff.Format(_T("%s "),m_pData->m_pAnnotation[ii].Text);
		size = pDC->GetTextExtent(Buff);
		rr.TopLeft()=ptText;
		rr.right=rr.left+size.cx;
		rr.bottom=rr.top+size.cy;
		if(rr.PtInRect(pt)) {
			nEditAnnotate = ii;
		}
		pDC->SelectObject(pOldFont);
		delete pFont;
	}
	ReleaseDC(pDC);

	return nEditAnnotate;
}

void CViewPage::DrawAnnotation(CPaintDC* pDC)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	if(pViewSheet->m_bRenderAnnotation!=TRUE) return;

	CString	Buff,Temp;
	CPoint pt;
	int nn;
	CSize size;

	for(int ii=0;ii<m_pData->m_nAnnotationL;ii++) {
		if((m_pData->m_pAnnotation[ii].nApplyToAllImage==1) || ((m_pData->m_pAnnotation[ii].nImageType == m_nImageType) && (m_pData->m_pAnnotation[ii].nImageNumber ==m_nImageNumber))) {
			CFont* pFont = (CFont *) new CFont;
			pFont->CreateFontIndirect(&m_pData->m_pAnnotation[ii].lf);
			CFont* pOldFont = pDC->SelectObject(pFont);
			pDC->SetTextColor(m_pData->m_pAnnotation[ii].rgbTextColor);
			pDC->SetBkMode(m_pData->m_pAnnotation[ii].nBkMode);
			pDC->SetBkColor(m_pData->m_pAnnotation[ii].rgbBkColor);

			ComponentOffsetToClient(m_pData->m_pAnnotation[ii].fXp,m_pData->m_pAnnotation[ii].fYp, &pt);
			Temp.Format(L"%s",m_pData->m_pAnnotation[ii].Text);
			Buff.Empty();
			for(nn=0;nn<Temp.GetLength(); nn++) {
				if(Temp.GetAt(nn) != '\r') {
					Buff += Temp.GetAt(nn);
				} else {
					size = pDC->GetTextExtent(Buff);
					pDC->TextOut(pt.x,pt.y,Buff);
					pt.y += size.cy;
					Buff.Empty();
				}
			}
			if(Buff.GetLength()>0) {
				size = pDC->GetTextExtent(Buff);
				pDC->TextOut(pt.x,pt.y,Buff);
			}
			
			pDC->SelectObject(pOldFont);
			delete pFont;
		}
	}

}

void CViewPage::DrawDiagZeroCrossings(CPaintDC *pDC)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	if(! (pViewSheet->m_bRenderZeroXtip || pViewSheet->m_bRenderZeroZtip) )
		return;
	
	CPolyCoord *pLines = PROFILE->m_ScanLine;
	int nLineL = PROFILE->m_nScanLineL;
	CFont Font;
	int nLine=0;
	int nn=0;
	CCoord Cp;
	double dOldValue=0;
	CPoint pt;
	int nCoordL;
	CPen pen(PS_SOLID,1,theApp.m_LastSettings.rgbRuler);
	CRgn rgn;
	CRect rect,rr;

	CPen *pOldPen = pDC->SelectObject(&pen);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	Font.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(theApp.m_LastSettings.rgbRuler);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	pDC->GetClipBox(&rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);


	if(pViewSheet->m_bRenderZeroXtip) {
		for(nLine = 0;nLine<nLineL;nLine++) {
			nCoordL = pLines[nLine].m_nCoordL;
			for(nn=0;nn<nCoordL;nn++) {
				pLines[nLine].GetCoord(nn,&Cp);
				if( ((Cp.Side0.fI>=0) && (dOldValue<0)) ||
					((Cp.Side0.fI<0) && (dOldValue>=0)) ) 
				{
					WorldToClient(Cp,&pt);
					if(rr.PtInRect(pt)) {
						pDC->SetPixel(pt,RGB(255,0,255));
					}
				}
				dOldValue = Cp.Side0.fI;
			}//for point
		}//for line
	}

	if(pViewSheet->m_bRenderZeroZtip) {
		for(nLine = 0;nLine<nLineL;nLine++) {
			nCoordL = pLines[nLine].m_nCoordL;
			for(nn=0;nn<nCoordL;nn++) {
				pLines[nLine].GetCoord(nn,&Cp);
				if( ((Cp.Side0.fK>=0) && (dOldValue<0)) ||
					((Cp.Side0.fK<0) && (dOldValue>=0)) ) 
				{
					WorldToClient(Cp,&pt);
					if(rr.PtInRect(pt)) {
						pDC->SetPixel(pt,RGB(128,255,128));
					}
				}
				dOldValue = Cp.Side0.fK;
			}//for point
		}//for line
	}


	rgn.SetRectRgn(&rect);
	pDC->SelectClipRgn(&rgn);


}

void CViewPage::DrawDiagDirChange(CPaintDC *pDC)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	if(! (pViewSheet->m_bRenderDirChangeX || pViewSheet->m_bRenderDirChangeY || pViewSheet->m_bRenderDirChangeZ || pViewSheet->m_bRenderDirChangeXtip || pViewSheet->m_bRenderDirChangeZtip) )
		return;

	CPolyCoord *pLines = PROFILE->m_ScanLine;
	int nLineL = PROFILE->m_nScanLineL;
	CFont Font;
	int nn=0;
	CCoord CpSurface,CpHead;
	double dOldValue=0;
	CPoint pt;
	int nCoordL;
	double dir;

	CPen pen(PS_SOLID,1,theApp.m_LastSettings.rgbRuler);
	CRgn rgn;
	CRect rect,rr;

	CPen *pOldPen = pDC->SelectObject(&pen);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	Font.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(theApp.m_LastSettings.rgbRuler);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	pDC->GetClipBox(&rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);

		//for each scanline check scan point see if at zero
	if(pViewSheet->m_bRenderDirChangeX) {
		for(int nLine = 0;nLine<nLineL;nLine++) {
			nCoordL = pLines[nLine].m_nCoordL;
			for(nn=0;nn<nCoordL;nn++) {
				pLines[nLine].GetCoord(nn,&CpSurface);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,0,TRUE,TRUE, NULL, CURRENT_FL);
				if( ((CpHead.Side0.fX>dOldValue) && (dir<0)) ||
					((CpHead.Side0.fX<dOldValue) && (dir>0)) ) 
				{
					WorldToClient(CpHead,&pt);
					if(rr.PtInRect(pt)) {
						pDC->SetPixel(pt,RGB(255,0,0));
					}
				}
				dir=CpHead.Side0.fX-dOldValue;
				dOldValue = CpHead.Side0.fX;			
			}//for point
		}//for line
	}//if

	if(pViewSheet->m_bRenderDirChangeY) {
		for(int nLine = 0;nLine<nLineL;nLine++) {
			nCoordL = pLines[nLine].m_nCoordL;
			for(nn=0;nn<nCoordL;nn++) {
				pLines[nLine].GetCoord(nn,&CpSurface);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,0,TRUE,TRUE, NULL, CURRENT_FL);
				if( ((CpHead.Side0.fY>dOldValue) && (dir<0)) ||
					((CpHead.Side0.fY<dOldValue) && (dir>0)) ) 
				{
					WorldToClient(CpHead,&pt);
					if(rr.PtInRect(pt)) {
						pDC->SetPixel(pt,RGB(0,255,0));
					}
				}
				dir=CpHead.Side0.fY-dOldValue;
				dOldValue = CpHead.Side0.fY;			
			}//for point
		}//for line
	}//if


	if(pViewSheet->m_bRenderDirChangeZ) {
		for(int nLine = 0;nLine<nLineL;nLine++) {
			nCoordL = pLines[nLine].m_nCoordL;
			for(nn=0;nn<nCoordL;nn++) {
				pLines[nLine].GetCoord(nn,&CpSurface);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,0,TRUE,TRUE, NULL, CURRENT_FL);
				if( ((CpHead.Side0.fZ>dOldValue) && (dir<0)) ||
					((CpHead.Side0.fZ<dOldValue) && (dir>0)) ) 
				{
					WorldToClient(CpHead,&pt);
					if(rr.PtInRect(pt)) {
						pDC->SetPixel(pt,RGB(0,0,255));
					}
				}
				dir=CpHead.Side0.fZ-dOldValue;
				dOldValue = CpHead.Side0.fZ;			
			}//for point
		}//for line
	}//if


	if(pViewSheet->m_bRenderDirChangeXtip) {
		for(int nLine = 0;nLine<nLineL;nLine++) {
			nCoordL = pLines[nLine].m_nCoordL;
			for(nn=0;nn<nCoordL;nn++) {
				pLines[nLine].GetCoord(nn,&CpSurface);
				if(nLine==92)
					int gg=100;
				if( ((CpSurface.Side0.fI>dOldValue) && (dir<0)) ||
					((CpSurface.Side0.fI<dOldValue) && (dir>0)) ) 
				{
					WorldToClient(CpSurface,&pt);
					if(rr.PtInRect(pt)) {
						pDC->SetPixel(pt,RGB(255,255,0));
					}
				}
				dir=CpSurface.Side0.fI-dOldValue;
				dOldValue = CpSurface.Side0.fI;			
			}//for point
		}//for line
	}//if

	if(pViewSheet->m_bRenderDirChangeZtip) {
		for(int nLine = 0;nLine<nLineL;nLine++) {
			nCoordL = pLines[nLine].m_nCoordL;
			for(nn=0;nn<nCoordL;nn++) {
				pLines[nLine].GetCoord(nn,&CpSurface);
				if( ((CpSurface.Side0.fK>dOldValue) && (dir<0)) ||
					((CpSurface.Side0.fK<dOldValue) && (dir>0)) ) 
				{
					WorldToClient(CpSurface,&pt);
					if(rr.PtInRect(pt)) {
						pDC->SetPixel(pt,RGB(0,255,255));
					}
				}
				dir=CpSurface.Side0.fK-dOldValue;
				dOldValue = CpSurface.Side0.fK;			
			}//for point
		}//for line
	}//if

	rgn.SetRectRgn(&rect);
	pDC->SelectClipRgn(&rgn);

}

void CViewPage::DrawProfileWaypoints(CPaintDC *pDC)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	CPolyCoord *pLines = PROFILE->m_ScanLine;
	int nLineL = PROFILE->m_nScanLineL;
	CFont Font;
	int nn=0;
	CCoord Cp;
	double dOldValue=0;
	CPoint pt;

	CPen pen(PS_SOLID,1,theApp.m_LastSettings.rgbRuler);
	CRgn rgn;
	CRect rect,rr;

	if(pViewSheet->m_bRenderWaypoints!=TRUE) return;

	CPen *pOldPen = pDC->SelectObject(&pen);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	Font.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(theApp.m_LastSettings.rgbRuler);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	pDC->GetClipBox(&rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);



	for(int nLine = 0;nLine<PROFILE->m_nWayPointsL;nLine++) {
		for(int ss=0;ss<PROFILE->m_WayPoints[nLine].m_nCoordL;ss++) {
			PROFILE->m_WayPoints[nLine].GetCoord(ss,&Cp);
			WorldToClient(Cp,&pt);
			if(rr.PtInRect(pt)) {
				pDC->SetPixel(pt,RGB(255,0,0));
			}
		}
	}


	rgn.SetRectRgn(&rect);
	pDC->SelectClipRgn(&rgn);






}



void CViewPage::DrawProfileTaughtLines(CPaintDC *pDC)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	if(pViewSheet->m_bRenderProfile!=TRUE) return;
	CPolyCoord *pLines = PROFILE->m_TaughtLine;
	int nLineL = PROFILE->m_nTaughtLineL;
	int nCoordL,nn;
	CCoord Cp;
	CFont Font;
	CPoint pt;
	CString Buff;
	CPen pen(PS_SOLID,1,theApp.m_LastSettings.rgbRuler);
	CRgn rgn;
	CRect rect,rr;


	if(pLines==NULL) return;
	if(nLineL==0) return;

	CPen *pOldPen = pDC->SelectObject(&pen);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	Font.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(theApp.m_LastSettings.rgbRuler);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	pDC->GetClipBox(&rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);


	//Draw lines
	for(int nLine = 0;nLine<nLineL;nLine++) {
		nCoordL = pLines[nLine].m_nCoordL;
		for(nn=0;nn<nCoordL;nn++) {
			pLines[nLine].GetCoord(nn,&Cp);
			WorldToClient(Cp,&pt);
			if(nn==0) {
				pDC->MoveTo(pt);
			} else {
				pDC->LineTo(pt);
			}
		}
	}
	//Draw crosses
	for(int nLine = 0;nLine<nLineL;nLine++) {
		nCoordL = pLines[nLine].m_nCoordL;
		for(nn=0;nn<nCoordL;nn++) {
			pLines[nLine].GetCoord(nn,&Cp);
			WorldToClient(Cp,&pt);
			pDC->MoveTo(pt.x-3,pt.y-3);
			pDC->LineTo(pt.x+4,pt.y+4);
			pDC->MoveTo(pt.x-3,pt.y+3);
			pDC->LineTo(pt.x+4,pt.y-4);
		}
	}

	pLines[theApp.m_nLine].GetCoord(theApp.m_nIndex,&Cp);
	WorldToClient(Cp,&pt);
	Buff.Format(_T("Line %d Coord %d"),theApp.m_nLine+1,theApp.m_nIndex+1);
	pDC->TextOut(pt.x,pt.y,Buff);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(RGB(0,0,0));

	rgn.SetRectRgn(&rect);
	pDC->SelectClipRgn(&rgn);



}


void CViewPage::PrintCallBack(CPaintDC *pDC,CRect* prr,int nDevice)
{

	m_bPalette = TRUE;
	InitializePalette(nDevice);
//	SetZoomSamples();
	if(prr->Width()>0 && prr->Height()>0) {
		InitializeViewVariables(pDC,prr);
		RenderView(pDC);

		PresentRenderdView(pDC);
		DrawCAD(pDC,nDevice);
	//	DrawGraticule(pDC);
	//	DrawRuler(pDC);
	//	DrawHistogram(pDC);
	//	DrawAnnotation(pDC);
	}

	m_bInitialize=FALSE;
	m_bPalette=TRUE;
	m_bRenderView=FALSE;


}

void CViewPage::OnViewGotoleft()
{


	GotoPoint(1);
}

void CViewPage::OnViewGotoright()
{
	GotoPoint(2);
}

void CViewPage::OnViewGotoboth()
{
	GotoPoint(3);
}


void CViewPage::GotoPoint(int nSide)
{
	CCoord	Cp,CpSurface,CpHead;
	CPoint pt;
	CString Buff,Warning;

	if(theApp.m_LastSettings.nDisplayAreYouSure==TRUE) {
		if(MessageBox(_T("Is the correct profile loaded?"),_T("Warning"),MB_YESNO)!=IDYES) return;
	}

	theApp.m_Motors.SetRevCount(0);
	theApp.m_Motors.GetHeadPos(&CpHead);


	ClientToSample(m_ptClient,&pt);
	pt.y *= m_pData->m_nLineIncrement;
	pt.y+=m_pData->m_nScanStartLine;
	float fSize = m_pData->m_fFastScanSize;
	fSize = m_pData->m_CpFinish.Side0.fPos[m_pData->m_nFastAxis] - m_pData->m_CpStart.Side0.fPos[m_pData->m_nFastAxis];

	if((m_pData->m_nCollectAxis==5) || (m_pData->m_nCollectAxis==6)) {
			if(fSize==0) fSize=360;
	}


	float fPos = ((float)pt.x * fSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fPos[m_pData->m_nCollectAxis];
	switch(theApp.m_LastSettings.nImageOrientation) {
	default:
		break;
	case 2:
	case 3:
		fPos = 360.0f - fPos;
		break;
	}

	ClientToSample(m_ptClient,&pt);
	pt.y *= m_pData->m_nLineIncrement;
	pt.y+=m_pData->m_nScanStartLine;

	if(m_pData->m_ProfileName == PROFILE->m_FileName) {

		theApp.m_Thread.StopCurrentThreadAction();

		if(m_pData->m_nComponentDescription == RECTILINEAR) {
			ClientToCoord(m_ptClient,&CpSurface);
//			m_pData->GenerateSamplePositionLine(pt.y,&m_LineSamplePos);
//			m_LineSamplePos.GetCoord(pt.x,&CpSurface);
			PROFILE->GetSurfaceCoordinate(CpSurface,pt,CpSurface.fPos[m_pData->m_nFastAxis],m_pData->m_nFastAxis);

			if(m_pData->m_nScanType == MULTI_PROBE_SCAN) {
				int nProbeNumber = theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Pr30.nMuxTx - 1;
				CpSurface.Side0.fY -= ((float)nProbeNumber * theApp.m_LastSettings.fProbeSeparation);
			}

			if((theApp.m_LastSettings.bUseBrainCompensation == TRUE) && (PROFILE->m_nBrainLineL>0)){
				PROFILE->CalculateBrainCoord(PROFILE->m_nFastAxis,PROFILE->m_nSlowAxis,CpSurface);
			}


			switch(theApp.m_Tank.nScannerDescription) {
			default:
				switch(nSide) {
				case 1: theApp.m_PmacUser.m_nScanSide = 0;
					break;
				case 2: theApp.m_PmacUser.m_nScanSide = 1;
					break;
				case 3: theApp.m_PmacUser.m_nScanSide = 2;
					break;
				};
				theApp.m_Thread.m_nMoveSide = nSide;
				theApp.m_Thread.m_CpMoveTo = CpSurface;
				theApp.m_Thread.m_nDangerPlaneThreadAction = 0;
				theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
				break;
			case SINGLE_ROBOT:
			case DUAL_ROBOT:
			case DUAL_ROBOT_9_PLUS_9:
			case SPX_ROBOT:
			case TANK_5AXIS:
			case TANK_6AXIS:
			case QUICKSTEP_FLATBED:
				theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpSurface, nSide, theApp.m_LastSettings.nDangerPlaneMoveMask);
				break;
			case TRACKED_PROBE:
				break;
			};

		} else {

			PROFILE->GetSurfaceCoordinate(CpSurface,pt,fPos,m_pData->m_nCollectAxis);

			if(theApp.m_LastSettings.bUseBrainCompensation==TRUE) 
				PROFILE->CalculateBrainCoord(0,2,CpSurface);

			theApp.m_Thread.m_nMoveSide = nSide;

			switch(theApp.m_Tank.nScannerDescription) {
			default:
				theApp.m_Thread.m_CpMoveTo = CpSurface;
				theApp.m_Thread.m_nDangerPlaneThreadAction = 0;
				theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
				break;
			case SPX_ROBOT:
			case FB_11_AXIS:
			case TANK_5AXIS:
			case QUICKSTEP_FLATBED:
				theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpSurface, nSide, theApp.m_LastSettings.nDangerPlaneMoveMask);
				theApp.m_FBCtrl.JogJustR(CpSurface.Side0.fR, PROFILE->m_fScanSpeed);
				break;
			case TRACKED_PROBE:
				break;
			case TANK_6AXIS:
				theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpSurface, nSide, theApp.m_LastSettings.nDangerPlaneMoveMask);
				break;
			}
		}


//		}

	} else {
		Buff.LoadString(IDS_Please_Load_Appropriate_Profile);
		Warning.LoadString(IDS_Error);
		MessageBox(Buff,Warning,MB_ICONERROR);
	}
}



void CViewPage::OnViewCopyareatoenvelope()
{
	CPoint pt,ptCorner;
	float fPos;
	CCoord Cp,Cp0;


	PROFILE->DeleteLine(ENVELOPELINES,0);


	ClientToSample(m_rrRuler.TopLeft(),&pt);
	pt.y *= m_pData->m_nLineIncrement;
	pt.y+=m_pData->m_nScanStartLine;
	fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fX;
	PROFILE->GetSurfaceCoordinate(Cp,pt,fPos,m_pData->m_nFastAxis);
	if(PROFILE->m_EnvelopeLine==NULL) PROFILE->m_EnvelopeLine = new CPolyCoord[PROFILE->m_nEnvelopeLineL=1];
	PROFILE->m_EnvelopeLine[theApp.m_nLine=0].Add(Cp);

	ptCorner.x = m_rrRuler.right;
	ptCorner.y = m_rrRuler.top;
	ClientToSample(ptCorner,&pt);
	pt.y *= m_pData->m_nLineIncrement;
	pt.y+=m_pData->m_nScanStartLine;
	fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fX;
	PROFILE->GetSurfaceCoordinate(Cp,pt,fPos,m_pData->m_nFastAxis);
	PROFILE->m_EnvelopeLine[theApp.m_nLine=0].Add(Cp);

	ptCorner.x = m_rrRuler.right;
	ptCorner.y = m_rrRuler.bottom;
	ClientToSample(ptCorner,&pt);
	pt.y *= m_pData->m_nLineIncrement;
	pt.y+=m_pData->m_nScanStartLine;
	fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fX;
	PROFILE->GetSurfaceCoordinate(Cp,pt,fPos,m_pData->m_nFastAxis);
	PROFILE->m_EnvelopeLine[theApp.m_nLine=0].Add(Cp);

	ptCorner.x = m_rrRuler.left;
	ptCorner.y = m_rrRuler.bottom;
	ClientToSample(ptCorner,&pt);
	pt.y *= m_pData->m_nLineIncrement;
	pt.y+=m_pData->m_nScanStartLine;
	fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fX;
	PROFILE->GetSurfaceCoordinate(Cp,pt,fPos,m_pData->m_nFastAxis);
	PROFILE->m_EnvelopeLine[theApp.m_nLine=0].Add(Cp);
}


void CViewPage::OnViewDeleteannotation()
{
	int nIndex = IsPtInAnnotation(m_ptClient);

	if(nIndex>=0) m_pData->DeleteAnnotation(nIndex);
	Invalidate(FALSE);
}

void CViewPage::DrawTessellation(CPaintDC* pDC)
{
	CRect rr;
	CPoint pt[4];
	/*
	if(theApp.m_nTessL<=0) return;

	CPen pen(PS_SOLID,1,theApp.m_LastSettings.rgbRuler);
	CPen* pOldPen = pDC->SelectObject(&pen);

	for(int nT=0;nT<theApp.m_nTessL;nT++) {
		SampleToClient(theApp.m_pTessellation[nT].v[0].pt,&pt[0]);
		SampleToClient(theApp.m_pTessellation[nT].v[1].pt,&pt[1]);
		SampleToClient(theApp.m_pTessellation[nT].v[2].pt,&pt[2]);
		SampleToClient(theApp.m_pTessellation[nT].v[3].pt,&pt[3]);

		pDC->MoveTo(pt[0]);
		pDC->LineTo(pt[1]);
		pDC->LineTo(pt[2]);
		pDC->LineTo(pt[3]);
		pDC->LineTo(pt[0]);
		pDC->LineTo(pt[2]);

	}

	pDC->SelectObject(pOldPen);
	*/
}


void CViewPage::OnTimer(UINT nIDEvent)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	if(theApp.m_LastSettings.nDisplayPositionCross!=TRUE) return;

	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CDC  dcMem;
	CRgn rgn;
	CRect rr,rect;
	CCoord	CpSurface,CpHead;
	CPoint	point(-1,-1);
	CDC* pDC = GetDC();
	CPen pen(PS_SOLID,1,theApp.m_LastSettings.rgbRuler);
	CPen* pOldPen=pDC->SelectObject(&pen);
	CBitmap *pOldBitmap;
	CBitmap	bmp,bmpCrossHair;
	CPoint ptSample;
	int	nLine,nSample;

	static int nCount=0;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	if(m_pData->m_nScanType == MULTI_PROBE_SCAN) {
		int nProbeNumber = theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Pr30.nMuxTx - 1;
		CpSurface.Side0.fY += (float)nProbeNumber * theApp.m_LastSettings.fProbeSeparation;
	}

	if((m_pData->m_nCollectAxis != 5) &&  (m_pData->m_nCollectAxis != 6)) {
		WorldToLineSample(CpSurface, &nSample, &nLine);
		ptSample = CPoint(nSample,(nLine-m_pData->m_nScanStartLine)/m_pData->m_nLineIncrement);
		SampleToClient(ptSample,&point);
	} else {
		WorldToClient(CpSurface,&point);
	}

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	point.x-=8;
	point.y-=8;

//	point.x=nCount%rr.Width();
//	point.y=nCount/rr.Width();
//	nCount++;

	MinMax(&point.x,-8,rr.right);
	MinMax(&point.y,-8,rr.bottom);
	int m_nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = m_nBitsPixel/8;

	bmp.CreateCompatibleBitmap(pDC,16,16);
	bmp.SetBitmapBits((DWORD)(256*nBytesPixel),m_colorrefCrossHair);
	dcMem.CreateCompatibleDC(pDC);

	if(m_pointCrossHair!=point){
		pDC->GetClipBox(&rect);
		rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
		pDC->SelectClipRgn(&rgn);
		if(m_bCrossVisible==TRUE) {
			pOldBitmap = dcMem.SelectObject(&bmp);
			pDC->BitBlt(m_pointCrossHair.x,m_pointCrossHair.y,16,16,&dcMem,0,0,SRCCOPY);
			dcMem.SelectObject(pOldBitmap);
			m_bCrossVisible=FALSE;
		}
		bmpCrossHair.CreateCompatibleBitmap(pDC,16,16);
		pOldBitmap = dcMem.SelectObject(&bmpCrossHair);
		dcMem.BitBlt(0,0,16,16,pDC,point.x,point.y,SRCCOPY);
		dcMem.SelectObject(pOldBitmap);
		bmpCrossHair.GetBitmapBits(256*nBytesPixel,m_colorrefCrossHair);

		pDC->MoveTo(point.x+7,point.y);
		pDC->LineTo(point.x+7,point.y+6);
		pDC->MoveTo(point.x+8,point.y);
		pDC->LineTo(point.x+8,point.y+8);
		pDC->MoveTo(point.x+9,point.y);
		pDC->LineTo(point.x+9,point.y+6);

		pDC->MoveTo(point.x+7,point.y+15);
		pDC->LineTo(point.x+7,point.y+10);
		pDC->MoveTo(point.x+8,point.y+15);
		pDC->LineTo(point.x+8,point.y+8);
		pDC->MoveTo(point.x+9,point.y+15);
		pDC->LineTo(point.x+9,point.y+10);

		pDC->MoveTo(point.x,point.y+7);
		pDC->LineTo(point.x+6,point.y+7);
		pDC->MoveTo(point.x,point.y+8);
		pDC->LineTo(point.x+8,point.y+8);
		pDC->MoveTo(point.x,point.y+9);
		pDC->LineTo(point.x+6,point.y+9);

		pDC->MoveTo(point.x+15,point.y+7);
		pDC->LineTo(point.x+10,point.y+7);
		pDC->MoveTo(point.x+15,point.y+8);
		pDC->LineTo(point.x+8,point.y+8);
		pDC->MoveTo(point.x+15,point.y+9);
		pDC->LineTo(point.x+10,point.y+9);

//		pDC->LineTo(point.x+8,point.y+8);
//		pDC->LineTo(point.x+9,point.y+6);
//		pDC->LineTo(point.x+9,point.y-1);
//		pDC->MoveTo(point.x,point.y+8);
//		pDC->LineTo(point.x+16,point.y+8);

		m_pointCrossHair=point;
		m_bCrossVisible=TRUE;
		rgn.SetRectRgn(&rect);
		pDC->SelectClipRgn(&rgn);

	};

	pDC->SelectObject(pOldPen);
//	pDC->SetROP2(nOldMode);

	ReleaseDC(pDC);

	CResizablePage::OnTimer(nIDEvent);
}


void CViewPage::OnViewGototaughtcoord()
{
	CCoord	Cp,CpSurface,CpHead;

	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	if(pViewSheet->m_bRenderProfile){
		theApp.m_Thread.StopCurrentThreadAction();
		PROFILE->m_TaughtLine[theApp.m_nLine].GetCoord(theApp.m_nIndex,&CpSurface);
		theApp.m_UtUser.SetAllUt(CpSurface,PROFILE->m_nHeader);
	}else if (pViewSheet->m_bRenderBrane){
		theApp.m_Thread.StopCurrentThreadAction();
		PROFILE->m_BrainLine[theApp.m_nLine].GetCoord(theApp.m_nIndex,&CpSurface);
		theApp.m_UtUser.SetAllUt(CpSurface,PROFILE->m_nHeader);
	}else{
		return;
	}

	if(theApp.m_LastSettings.bGotoViaDangerPlane == FALSE) {

		theApp.m_Motors.GetHeadPos(&CpHead);

		theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,STARBOARD,TRUE,TRUE, NULL, CURRENT_FL);

		//Keep right side where it is
		switch(theApp.m_nSide0Orientation) {
		case 0:	Cp.CopySide(STARBOARD,CpHead);
			break;
		case 1:	Cp.CopySide(PORTSIDE,CpHead);
			break;
		}

		theApp.m_Thread.m_CpMoveTo = Cp;
		theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
	} else {

		//Keep right side where it is
		switch(theApp.m_nSide0Orientation) {
		case 0:	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 0;
			break;
		case 1:	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 1;
			break;
		}
		theApp.m_Thread.m_CpMoveTo = CpSurface;
		theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
	}


}


float CViewPage::CalculateAreaOfPolygon()
{
	CCoord Cp;

	if(m_pData->m_pPolygon == NULL) return 0.0f;
	if(m_pData->m_pPolygon[m_nPolyPtr].m_nCoordL<3) return 0.0f;

	m_pData->m_pPolygon[m_nPolyPtr].GetCoord(0,&Cp);
	float fX0 = Cp.Side0.fX;
	float fY0 = Cp.Side0.fY;
	float fZ0 = Cp.Side0.fZ;
	float fX1,fY1,fZ1;
	float fTotalArea=0.0f;
	float fH;
	int jj;

	for(int ii=1;ii<=m_pData->m_pPolygon[m_nPolyPtr].m_nCoordL;ii++) {
		jj=ii%m_pData->m_pPolygon[m_nPolyPtr].m_nCoordL;
		m_pData->m_pPolygon[m_nPolyPtr].GetCoord(jj,&Cp);
		fX1 = Cp.Side0.fX;
		fY1 = Cp.Side0.fY;
		fZ1 = Cp.Side0.fZ;
		fH = (float)hypot( ((fY1+fY0)/2), ((fZ1+fZ0)/2) );
		fTotalArea += (fH * (fX1-fX0));
		fX0 = fX1;
		fY0 = fY1;
		fZ0 = fZ1;
	};
	return fabs(fTotalArea);

}

void CViewPage::DrawProfileBraneLines(CPaintDC *pDC)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	if(pViewSheet->m_bRenderBrane!=TRUE) return;
	CPolyCoord *pLines = PROFILE->m_BrainLine;
	int nLineL = PROFILE->m_nBrainLineL;
	int nCoordL,nn;
	CCoord Cp;
	CFont Font;
	CPoint pt;
	CString Buff;
	CPen pen(PS_SOLID,1,theApp.m_LastSettings.rgbDisplayBrainColor);
	CRgn rgn;
	CRect rect,rr;


	if(pLines==NULL) return;
	if(nLineL==0) return;

	CPen *pOldPen = pDC->SelectObject(&pen);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	Font.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(theApp.m_LastSettings.rgbRuler);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	pDC->GetClipBox(&rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);


	//Draw lines
	for(int nLine = 0;nLine<nLineL;nLine++) {
		nCoordL = pLines[nLine].m_nCoordL;
		for(nn=0;nn<nCoordL;nn++) {
			pLines[nLine].GetCoord(nn,&Cp);
			WorldToClient(Cp,&pt);
			if(nn==0) {
				pDC->MoveTo(pt);
			} else {
				pDC->LineTo(pt);
			}
		}
	}
	//Draw crosses
	for(int nLine = 0;nLine<nLineL;nLine++) {
		nCoordL = pLines[nLine].m_nCoordL;
		for(nn=0;nn<nCoordL;nn++) {
			pLines[nLine].GetCoord(nn,&Cp);
			WorldToClient(Cp,&pt);
			pDC->MoveTo(pt.x-3,pt.y-3);
			pDC->LineTo(pt.x+4,pt.y+4);
			pDC->MoveTo(pt.x-3,pt.y+3);
			pDC->LineTo(pt.x+4,pt.y-4);
		}
	}

	pLines[theApp.m_nLine].GetCoord(theApp.m_nIndex,&Cp);
	WorldToClient(Cp,&pt);
	Buff.Format(_T("Line %d Coord %d"),theApp.m_nLine+1,theApp.m_nIndex+1);
	pDC->TextOut(pt.x,pt.y,Buff);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(RGB(0,0,0));

	rgn.SetRectRgn(&rect);
	pDC->SelectClipRgn(&rgn);


}

void CViewPage::OnViewUpdatebranecoordinate()
{
	CCoord	Cp,CpSurface,CpHead;
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(!pViewSheet->m_bRenderBrane) return;

	theApp.m_Thread.StopCurrentThreadAction();
	PROFILE->m_TaughtLine[theApp.m_nLine].GetCoord(theApp.m_nIndex,&CpSurface);
	theApp.m_UtUser.SetAllUt(CpSurface,PROFILE->m_nHeader);

	int nLine;

	for(nLine=0;nLine<PROFILE->m_nBrainLineL;nLine++) {
		PROFILE->m_BrainLine[nLine].SetAllModifiy(FALSE);
	}

	PROFILE->m_BrainLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].bModified = TRUE;

	pFrame->StopCurrentTimerAction();
	pFrame->m_nTimerAction = RELEARN_BRAIN;


}


void CViewPage::OnViewAnnotatehistogramdata()
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	CString ID;
	float fX, fY;

	ID.Format(_T("%.1f%%"),((float)m_pData->m_Hist.nTotalBelowThreshold[0]*100)/(float)m_pData->m_Hist.nTotalNumberSamples);
	ClientToWorld(m_ptClient, &fX,&fY);
	m_pData->AddAnnotation(fX,fY,ID,theApp.m_LastSettings.lfAnnotation,theApp.m_LastSettings.rgbAnnotation,m_nImageType,m_nImageNumber,theApp.m_LastSettings.nAnnotateApplyToAllImages, theApp.m_LastSettings.nAnnotateBkMode, theApp.m_LastSettings.rgbAnnotateBkColor);
	pViewSheet->m_bRenderAnnotation=TRUE;
	Invalidate(FALSE);
}

void CViewPage::WorldToLineSample(CCoord Cp, int *pnSample, int *pnLine)
{
	PROFILE->FindScanLineSample(Cp, pnSample, pnLine);

	int nAxis = m_pData->m_nCollectAxis;

	//Calculate in the old fashion way incase the two if statements fail
	*pnSample = (int)((Cp.fPos[nAxis]-m_pData->m_CpStart.fPos[nAxis]) * (float)(m_pData->m_nSamplesLine-1) / (m_pData->m_CpFinish.fPos[nAxis] - m_pData->m_CpStart.fPos[nAxis]));

	int nVertexLine  = (*pnLine - m_pData->m_nScanStartLine) / m_pData->m_nLineIncrement;
	if(nVertexLine<0) nVertexLine = 0;
	if(nVertexLine>=m_pData->m_nNumberLines) nVertexLine=m_pData->m_nNumberLines-1;
	if(m_pData->m_vLineSample) {
		if(m_pData->m_vLineSample[nVertexLine].m_nCoordL>2) {
			m_pData->m_vLineSample[nVertexLine].FindNearestSegment(Cp,PORTSIDE,pnSample);
			*pnSample += m_pData->m_vLineSample[nVertexLine].m_nFirstSample;
		}
	}

	if(*pnSample<0) *pnSample = 0;
	if(*pnSample>=m_pData->m_nSamplesLine) *pnSample = m_pData->m_nSamplesLine-1;

}

void CViewPage::CADLButtonDown(CPoint pt)
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;
	CCoord Cp;

	ClientToCoord(pt,&Cp);

	switch(theApp.m_nCADTool)
	{
	case CAD_MOUSE:
		pSheet->m_Data.m_nCADPtr = pSheet->m_Data.FindNearestCADNode(Cp);
		if((pSheet->m_Data.m_nCADPtr >=0 ) && (pSheet->m_Data.m_pCADElement)) {
			pSheet->m_Data.m_pCADElement[pSheet->m_Data.m_nCADPtr].SetLastSettings();
			if(pSheet->m_pCADSheet) pSheet->m_pCADSheet->UpdateAllPages();
		}
		break;
	case CAD_LINE:	pSheet->m_Data.m_nCADPtr = pSheet->m_Data.AddCADNode(pSheet->m_Data.m_nCADPtr, CAD_LINE, Cp);
		break;
	case CAD_ELLIPSE:
		break;
	case CAD_MULTI_LINE:
		break;
	case CAD_MOVE_NODE:
		pSheet->m_Data.m_nCADPtr = pSheet->m_Data.FindNearestCADNode(Cp);
		if((pSheet->m_Data.m_nCADPtr >=0 ) && (pSheet->m_Data.m_pCADElement)) {
			pSheet->m_Data.m_pCADElement[pSheet->m_Data.m_nCADPtr].m_Nodes.DeleteCoord(pSheet->m_Data.m_nCADNode);
			theApp.m_nCADTool = pSheet->m_Data.m_pCADElement[pSheet->m_Data.m_nCADPtr].m_Type;
			pSheet->m_Data.m_pCADElement[pSheet->m_Data.m_nCADPtr].SetLastSettings();
			if(pSheet->m_pCADSheet) pSheet->m_pCADSheet->UpdateAllPages();
		}
		break;
	}


}


bool CViewPage::ClientToCoord(CPoint ptPixel, CCoord *pCp)
{
	CPoint	ptSample;
	CString Buff;
	float fPos,fSize;

	if(m_pData->IsLineSample() ) {
		ClientToSample(ptPixel,&ptSample);
		m_pData->GetCoordFromLineSample(ptSample,pCp);
		return true;
	} else {

		if(m_pData->m_ProfileName == PROFILE->m_FileName) {
			ClientToSample(ptPixel,&ptSample);
			ptSample.y *= m_pData->m_nLineIncrement;
			ptSample.y+=m_pData->m_nScanStartLine;

			fSize = m_pData->m_fFastScanSize;
			fSize = m_pData->m_CpFinish.Side0.fPos[m_pData->m_nFastAxis] - m_pData->m_CpStart.Side0.fPos[m_pData->m_nFastAxis];
			fPos = ((float)ptSample.x * fSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fPos[m_pData->m_nFastAxis];

			PROFILE->GetSurfaceCoordinate(*pCp,ptSample,fPos,m_pData->m_nFastAxis);
			pCp->nSample = ptSample.x;
			pCp->nLine = ptSample.y;

			return true;
		} else {
			Buff = "Please load correct profile";
			MessageBox(Buff,_T("Error"),NULL);
			return false;
		}
	}
}

void CViewPage::DrawCAD(CPaintDC *pDC,int nDevice)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	int nn;
	CCoord Cp;
	CPoint pt[4],ptSample,ptCursor;
	CString Buff;
	CPen *pPen = NULL, *pPenDashed = NULL, *pOldPen = NULL;
	CRgn rgn;
	CRect rect,rr;
	CSize size;

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(theApp.m_LastSettings.rgbRuler);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	pDC->GetClipBox(&rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);

	for(nn=0; nn<pViewSheet->m_Data.m_nCADElementL; nn++) {

		pPen = new CPen(PS_SOLID,pViewSheet->m_Data.m_pCADElement[nn].m_nLineWidth,pViewSheet->m_Data.m_pCADElement[nn].m_rgbColor);
		pPenDashed = new CPen(PS_DOT,pViewSheet->m_Data.m_pCADElement[nn].m_nLineWidth,pViewSheet->m_Data.m_pCADElement[nn].m_rgbColor);
		pOldPen=pDC->SelectObject(pPen);

		CFont* pFont = (CFont *) new CFont;
		pFont->CreateFontIndirect(&pViewSheet->m_Data.m_pCADElement[nn].m_lf);
		CFont* pOldFont = pDC->SelectObject(pFont);
		pDC->SetTextColor(pViewSheet->m_Data.m_pCADElement[nn].m_rgbText);

		pViewSheet->m_Data.m_pCADElement[nn].m_Nodes.GetCoord(0,&Cp);
		SampleToClient(Cp.fSample,Cp.fLine,&pt[0]);
		pViewSheet->m_Data.m_pCADElement[nn].m_Nodes.GetCoord(1,&Cp);
		SampleToClient(Cp.fSample,Cp.fLine,&pt[1]);
		rr.TopLeft() = pt[0];
		rr.BottomRight() = pt[1];

		switch(pViewSheet->m_Data.m_pCADElement[nn].m_Type) {
		case CAD_LINE:
			RenderCADElementLine(pDC, &rr, nn, nDevice);
			break;
		case CAD_RECTANGLE:
			RenderCADElementDefectArea(pDC, &rr, nn);
			RenderTransparentRectangle(pDC, &rr);
			break;
		case CAD_ELLIPSE:
			RenderTransparentEllipse(pDC, &rr);
			break;
		case CAD_PARALLELOGRAM:
			pViewSheet->m_Data.m_pCADElement[nn].m_Nodes.GetCoord(2,&Cp);
			SampleToClient(Cp.fSample,Cp.fLine,&pt[2]);
			pViewSheet->m_Data.m_pCADElement[nn].m_Nodes.GetCoord(3,&Cp);
			SampleToClient(Cp.fSample,Cp.fLine,&pt[3]);
			RenderTransparentParallelogram(pDC,pt);
			break;
		case CAD_CENTRELINE:
			pViewSheet->m_Data.m_pCADElement[nn].m_Nodes.GetCoord(2,&Cp);
			SampleToClient(Cp.fSample,Cp.fLine,&pt[2]);
			pViewSheet->m_Data.m_pCADElement[nn].m_Nodes.GetCoord(3,&Cp);
			SampleToClient(Cp.fSample,Cp.fLine,&pt[3]);
			RenderCADElementCentreLine(pDC, pt,nn, nDevice);
			break;
		}
		if(nn == pViewSheet->m_Data.GetCADElement() && 
			(m_nToolOption == TOOL_LINE || m_nToolOption == TOOL_RECTANGLE || m_nToolOption == TOOL_ELLIPSE || TOOL_PARALLELOGRAM || TOOL_CENTRELINE) && 
			(pViewSheet->m_pCADTableDlg != NULL) ) {
			RenderCADElementSizeArrows(pDC);
		}


		size = pDC->GetTextExtent(pViewSheet->m_Data.m_pCADElement[nn].m_wText);
		pDC->TextOut(pt[0].x,pt[0].y-size.cy,pViewSheet->m_Data.m_pCADElement[nn].m_wText);


		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldFont);
		SAFE_DELETE( pFont );
		SAFE_DELETE( pPen );
		SAFE_DELETE( pPenDashed );
	}


	pDC->SetTextColor(RGB(0,0,0));

	rgn.SetRectRgn(&rect);
	pDC->SelectClipRgn(&rgn);
}


void CViewPage::DrawCADArrow(CPaintDC *pDC, CPoint pt0, CPoint pt1, int nSize,int nDir,int nDevice)
{
	int ii;
	int	xx,yy;
	double dAngle = atan2(pt1.y-pt0.y,pt1.x-pt0.x);
	double dPointAngle = (2.0 * PI * 30.0) / 360.0;
	double dTemp;
	double dSize = (double)abs(nSize) * (double)nDir;

	for(ii=1;ii<=abs(nSize);ii++) {

		dTemp = (dPointAngle * (double)ii) / (double)abs(nSize);
		xx=(int)(dSize * cos(dAngle + dTemp));
		yy=(int)(dSize * sin(dAngle + dTemp));
		pDC->MoveTo(pt0.x,pt0.y);
		pDC->LineTo(pt0.x+xx,pt0.y+yy);

		xx=(int)(dSize * cos(dAngle - dTemp));
		yy=(int)(dSize * sin(dAngle - dTemp));
		pDC->MoveTo(pt0.x,pt0.y);
		pDC->LineTo(pt0.x+xx,pt0.y+yy);

	}
}

void CViewPage::RenderCADElementDefectArea(CPaintDC* pDC, CRect rrArea,int nCADPtr)
{
	CPoint ptSample,ptClient;
	CRect rrSample,rrSrc,rrDest;
	int xx,yy,nSrc,nDest,nH,nW,nValue;

	if((m_pData->m_pCADElement[nCADPtr].m_nFlags & CAD_DEFECT_AREA) == 0) return;

	if(rrArea.top < 0) return;
	if(rrArea.bottom < 0) return;
	if(rrArea.left < 0) return;
	if(rrArea.right < 0) return;

	if(m_pData->m_pCADElement[nCADPtr].m_nCADDefectPaintMode == 1) {
		rrSrc.SetRect(0,0,m_rrView.Width(),m_rrView.Height());
		rrDest = rrArea;
		rrDest.OffsetRect(m_rrView.left,m_rrView.top);

		COLORREF *rgbSrc = (COLORREF *)m_pArrayView;
		COLORREF *rgbDest = new COLORREF[(rrArea.Width()+2) * (rrArea.Height()+2)];

		for(yy=rrArea.top,nH=0;yy<=rrArea.bottom;yy++,nH++) {

			nDest = nH * rrArea.Width();

			for(xx=rrArea.left,nW=0;xx<=rrArea.right;xx++,nW++,nDest++) {
		
				nSrc = (xx - m_rrView.left) + ((yy - m_rrView.top) * m_rrView.Width());

				ptClient.x = xx;
				ptClient.y = yy;
						
				ClientToSample(ptClient,&ptSample);

				nValue = m_pData->GetFilteredAmp(ptSample.x,ptSample.y,m_nImageNumber);

				if(nValue <= m_pData->m_pCADElement[nCADPtr].m_Hist.nSampleThreshold[0]) {
					rgbDest[nDest] = RGB(0,0,255);
				} else {
					if(nSrc>=0) {
						rgbDest[nDest] = rgbSrc[nSrc];
					}
				}
			}
		}

		CDC  dcMem;
		CBitmap bmp;

		bmp.CreateCompatibleBitmap(pDC,rrArea.Width(),rrArea.Height());
		bmp.SetBitmapBits((DWORD)(rrArea.Width()*rrArea.Height()*m_nBytesPixel),rgbDest);

		dcMem.CreateCompatibleDC(pDC);

		CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
		pDC->BitBlt(rrArea.left,rrArea.top,rrArea.Width(),rrArea.Height(),&dcMem,0,0,SRCCOPY);
		dcMem.SelectObject(pOldBitmap);

		delete rgbDest;
	}
	if(m_pData->m_pCADElement[nCADPtr].m_nCADDefectPaintMode == 2) {
		rrSample.left		= m_pData->m_pCADElement[nCADPtr].m_Hist.rrThreshWidth.left;
		rrSample.right	= m_pData->m_pCADElement[nCADPtr].m_Hist.rrThreshWidth.right;
		rrSample.top		= m_pData->m_pCADElement[nCADPtr].m_Hist.rrThreshHeight.top;
		rrSample.bottom	= m_pData->m_pCADElement[nCADPtr].m_Hist.rrThreshHeight.bottom;
		SampleToClient(rrSample.TopLeft(),&ptClient);		rrDest.TopLeft() = ptClient;
		SampleToClient(rrSample.BottomRight(),&ptClient);	rrDest.BottomRight() = ptClient;
		rrDest.top -= 1;
		RenderTransparentRectangle(pDC,&rrDest);
	}

}


/*
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
			nOffset=yy*rr.Width();
		} else {
			nOffset=(rr.Height()-1-yy)*rr.Width();
		};
*/

void CViewPage::PrintWusiWug(float fScale)
{
	CUSLPrint m_Print;
	int	nPrintRow=0;
	CRect Rect,rr,rrSampleSlice;
	CPaintDC *pDC = (CPaintDC *) &m_Print.m_dc;
	CString Buff;
	int	nTextStep;
	int cc;
	float fXSize,fYSize,fStep;
	CProgressDlg Progress;

	CWaitCursor Wait;
	m_Print.StartDoc(L"USLScanner Image");
	m_Print.StartPage();
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;
	if(nBytesPixel<0) nBytesPixel=1;

	int	nPrintingAreaWidthMM = pDC->GetDeviceCaps(HORZSIZE);		//908 //287mm
	int	nPrintingAreaHeightMM = pDC->GetDeviceCaps(VERTSIZE);	//1213 //405mm
	int nXRes = pDC->GetDeviceCaps(LOGPIXELSX);					//600 //300 pixels / inch
	int nYRes = pDC->GetDeviceCaps(LOGPIXELSY);					//600 //300 pixels / inch
	float fXResMM = (float)nXRes / 25.4f;
	float fYResMM = (float)nYRes / 25.4f;
	int nPrintingAreaWidth	= pDC->GetDeviceCaps(HORZRES);		//21460 //3387 Number of dots
	int nPrintingAreaHeight  = pDC->GetDeviceCaps(VERTRES);		//28860 //4786 Number of dots

	int	nPhysicalPageWidth   = pDC->GetDeviceCaps(PHYSICALWIDTH);	//21600 //3507
	int	nPhysicalPageHeight  = pDC->GetDeviceCaps(PHYSICALHEIGHT);	//28800 //4960

	int	nPrintingAreaOffsetLeft	= pDC->GetDeviceCaps(PHYSICALOFFSETX);	//70 //60
	int	nPrintingAreaOffsetTop	= pDC->GetDeviceCaps(PHYSICALOFFSETY);	//70 //36

	GetGraticuleSize(&fXSize, &fYSize,&fStep,&nTextStep,1.0f);	//315.4		579.95


	Rect.left=0;
	Rect.top=0;
	Rect.right = (int)((fYSize * fScale) * fXResMM);
	Rect.bottom = (int)((fXSize * fScale) * fYResMM);

	Rect.right = ((Rect.right / 8) + 1) * 8;

	if(Rect.Width() > nPrintingAreaWidth) {
		MessageBox(_T("The height of the image is greater than the width of the paper\r\nPrint aborted"),_T("Error"),MB_ICONERROR);
		m_Print.StopPage();
		m_Print.EndDoc();
		Wait.Restore();
		return;
	}

	int nLineSlices = 1000;

	unsigned char* pData = m_pData->m_cAmpBuffer[m_nImageNumber];

	char *pArrayView = new char[(Rect.Width()+2) * nLineSlices * nBytesPixel];
	CDC  dcMem;
	CBitmap bmp;
	int nSrcPtr,nDestPtr,nY,nX,nLength;
	int nDataSample,nDataLine,nPos;
	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,Rect.Width(),nLineSlices);

	int nPage = 0;
	int nPageLineNumber = 0;

	CProgressDlg dlg(NULL);
	dlg.Create(CProgressDlg::IDD, NULL);
	dlg.SetProgressRange(0,100);
	dlg.SetProgressText(L"Printing USLScanner");

	m_bPalette = true;
	InitializePalette(PRINTER);

	for(int nPrinterLine=Rect.top;nPrinterLine<Rect.bottom;nPrinterLine+=nLineSlices,nPageLineNumber+=nLineSlices) {


		nPos = RoundDownMulDiv(nPrinterLine,100,Rect.bottom);
		dlg.SetProgressPos(nPos);

		for(nY=0;nY<nLineSlices;nY++) {
			nDestPtr=nY * Rect.Width() * nBytesPixel;

			nDataSample = RoundDownMulDiv(nY+nPrinterLine,m_prrSamples->Width(),Rect.Height()) + m_prrSamples->left;


			for(nX=0;nX<Rect.Width();nX++) {

				if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
					nDataLine = m_prrSamples->Height() - RoundDownMulDiv(nX,m_prrSamples->Height(),Rect.Width()) + m_prrSamples->top;
				} else {
					nDataLine = m_prrSamples->Height() - RoundDownMulDiv(Rect.Width()-nX-1,m_prrSamples->Height(),Rect.Width()) + m_prrSamples->top;
				}

				nSrcPtr = nDataLine * m_pData->m_nSamplesLine + nDataSample;

				if((nY+nPrinterLine) < Rect.bottom) {
					cc = pData[nSrcPtr];
					pArrayView[nDestPtr++]=m_cColor[cc][0];
					pArrayView[nDestPtr++]=m_cColor[cc][1];
					pArrayView[nDestPtr++]=m_cColor[cc][2];
				} else {
					pArrayView[nDestPtr++]=(char) 255;
					pArrayView[nDestPtr++]=(char) 255;
					pArrayView[nDestPtr++]=(char) 255;
				}
				if(nBytesPixel>3) nDestPtr++;
			}
		}
		nLength = nLineSlices;
		if((nPageLineNumber + nLineSlices) > nPrintingAreaHeight) {
			nLength = nPrintingAreaHeight - nPageLineNumber;
		}

		bmp.SetBitmapBits((DWORD)(Rect.Width()*nLength*nBytesPixel),pArrayView);
		CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
		pDC->BitBlt(Rect.left,nPageLineNumber,Rect.Width(),nLength,&dcMem,0,0,SRCCOPY);
		dcMem.SelectObject(pOldBitmap);

		if(nPage==0) {
			PrintHeader(pDC);
		}

		if((nPageLineNumber+nLineSlices) > nPrintingAreaHeight) {
			m_Print.StopPage();
			nPage++;
			nPageLineNumber = 0 - nLineSlices;
			nPrinterLine = (nPrintingAreaHeight * nPage) - nLineSlices;
			Buff.Format(_T("Page %d"),nPage+1);
			m_Print.StartPage();
		}


	}

	m_Print.StopPage();
	m_Print.EndDoc();

	Wait.Restore();

	delete pArrayView;
	InitializePalette(SCREEN);

}

float CViewPage::ComputeRulerStats(CString *pBuff)
{
	CCoord Cp0,Cp1,Cp;
	CPoint pt,pt0,pt1;
	CString Message,Width,Height,Length;;
	float fLength,fCircumferance,fAngle,fStep;
	int nTemp,nn;
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	CString Buff;
	float fFastScanSize,fFastIncrement,fRadialLength,fTangentalLength;

	ClientToWorld(m_rrRuler.TopLeft(),&Cp0);
	ClientToWorld(m_rrRuler.BottomRight(),&Cp1);

	if(pViewSheet->m_nRectPolarDisplayMode == 0) {

		switch(theApp.m_LastSettings.nRulerTextMode) {
		default:
			break;
		case 1:
			fCircumferance = 2.0f * PIf * Cp0.Side0.fY;
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			fLength = fAngle * fCircumferance / 360.0f;
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Circumferential Length:%s"),Length);
			break;

		case 2:
			ClientToSample(m_rrRuler.BottomRight(),&pt0);
			ClientToSample(m_rrRuler.TopLeft(),&pt1);
			if(pt0.y>pt1.y) {
				nTemp = pt0.y;
				pt0.y=pt1.y;
				pt1.y=nTemp;
			}
			fLength = 0.0f;
			for(nn=pt0.y;nn<pt1.y;nn++) {
				Cp = PROFILE->m_ScanLine[0].m_pCp[nn];
				fStep = Cp.Length(PROFILE->m_ScanLine[0].m_pCp[nn+1]);
				fLength += fStep;
			}
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Tangental Length:%s"),Length);
			break;
		case 3:
			ClientToSample(m_rrRuler.BottomRight(),&pt0);
			ClientToSample(m_rrRuler.TopLeft(),&pt1);
			if(pt0.y>pt1.y) {
				nTemp = pt0.y;
				pt0.y=pt1.y;
				pt1.y=nTemp;
			}
			fLength = 0.0f;
			for(nn=pt0.y;nn<pt1.y;nn++) {
				Cp = PROFILE->m_ScanLine[0].m_pCp[nn];
				fStep = Cp.Length(PROFILE->m_ScanLine[0].m_pCp[nn+1]);
				fLength += fStep;
			}
			fCircumferance = 2.0f * PIf * Cp0.Side0.fY;
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			fCircumferance = fAngle * fCircumferance / 360.0f;
			fLength = sqrt(fCircumferance*fCircumferance + fLength*fLength);
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Surface Length:%s"),Length);
			break;
		case 5:	//Circumferential angle
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			pBuff->Format(_T("Angle:%.02f%s"),fAngle,DEGREES);
			break;
		case 6://Circumferential distance
			m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			fLength = fFastScanSize * fAngle / 360.0f;
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Length:%s"),Length);
			break;
		case 7:
			ClientToSample(m_rrRuler.BottomRight(),&pt0);
			pt0.y *= m_pData->m_nLineIncrement;
			pt0.y+=m_pData->m_nScanStartLine;
			ClientToSample(m_rrRuler.TopLeft(),&pt1);
			pt1.y *= m_pData->m_nLineIncrement;
			pt1.y+=m_pData->m_nScanStartLine;
			fLength = m_pData->m_fSlowScanSize / (float)m_pData->m_nNumberLines * (float)abs(pt0.y-pt1.y);
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Tangental Length:%s"),Length);
			break;
		case 8:
			m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			fRadialLength = fFastScanSize * fAngle / 360.0f;

			ClientToSample(m_rrRuler.BottomRight(),&pt0);
			pt0.y *= m_pData->m_nLineIncrement;
			pt0.y+=m_pData->m_nScanStartLine;
			ClientToSample(m_rrRuler.TopLeft(),&pt1);
			pt1.y *= m_pData->m_nLineIncrement;
			pt1.y+=m_pData->m_nScanStartLine;
			fTangentalLength = m_pData->m_fSlowScanSize / (float)m_pData->m_nNumberLines * (float)abs(pt0.y-pt1.y);

			fLength = sqrtf(fTangentalLength * fTangentalLength + fRadialLength * fRadialLength);

			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(L"Length:%s",Length);
			break;
		}
	}else {
		//m_fRulerLength calculated in DrawRuler
		fLength = m_fRulerLength;
		theApp.FormatLength(m_fRulerLength,1,Length);
		pBuff->Format(L"Surface Length:%s",Length);
	}


	return fLength;
}

void CViewPage::ClientToWorld(CPoint ptPixel,CCoord *pCp)
{
	CPoint ptSample;
	CCoord Cp;

	ClientToSample(ptPixel,&ptSample);
	RotoSampleToWorld(ptSample,pCp);

}

void CViewPage::RotoSampleToWorld(CPoint ptSample,CCoord *pCp)
{
	CCoord Cp;

	MinMax(&ptSample.x,0,m_pData->m_nSamplesLine-1);
	MinMax(&ptSample.y,0,m_pData->m_nNumberLines-1);

	if((m_pData->m_ProfileName.GetLength() > 0) && PROFILE->IsCorrectOneLoaded(&m_pData->m_ProfileName)) {
		Cp = PROFILE->m_ScanLine[0].m_pCp[ptSample.y];
	} else {
		Cp.Zero();
	}

	Cp.Side0.fR = (float)ptSample.x * 360.0f / (float)(m_pData->m_nSamplesLine - 1);

	CopyMemory(pCp,&Cp,sizeof Cp);

}


void CViewPage::DrawProfileEnvelopeLines(CPaintDC *pDC)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	if(pViewSheet->m_bRenderEnvelope!=TRUE) return;

	CPolyCoord *pLines = m_pData->m_pEnvelopeLine;
	int nLineL = m_pData->m_nEnvelopeLineL;
	int nCoordL,nn;
	CCoord Cp;
	CFont Font;
	CPoint pt0;
	CPoint *pt = NULL;
	BYTE *pByte = NULL;
	CPoint ptSample;
	CString Buff;
	CRgn rgn;
	CRect rect,rr;
	CPen *pOldPen=NULL;
	CPen *pPen=NULL;

	if(pLines==NULL) return;
	if(nLineL==0) return;


	if(pLines[0].m_nCoordL>0) {;
		pLines[0].GetCoord(0,&Cp);
		if((Cp.nSample == 0) && (Cp.nLine == 0) && (m_bThreadEnabled == false)) {
			m_bThreadEnabled=TRUE;
			m_bThreadFinished = false;
			m_pThread=AfxBeginThread(&CalculateEnvelopeLineAndSampleThread,this,THREAD_PRIORITY_NORMAL ,0,NULL);

		}
	}

	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	Font.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(theApp.m_LastSettings.rgbRuler);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	pDC->GetClipBox(&rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);


	//Draw lines
	for(int nLine = 0;nLine<nLineL;nLine++) {
		
		if(pLines[nLine].m_nOffOn > 0) {
			nCoordL = pLines[nLine].m_nCoordL;
			pt = new CPoint[nCoordL + 1];
			pByte = new BYTE[nCoordL + 1];

			for(nn=0;nn<nCoordL;nn++) {
				pLines[nLine].GetCoord(nn,&Cp);
/*
				if((Cp.nSample == 0) && (Cp.nLine == 0)) {
					WorldToLineSample(Cp, &nS, &nL);			//This bit is horrifically slow
					Cp.nSample = nS;
					Cp.nLine = nL;
					pLines[nLine].m_pCp[nn].nSample = nS;		// so the answer is storred for use next time
					pLines[nLine].m_pCp[nn].nLine = nL;
				}
*/
				ptSample = CPoint(Cp.nSample,(Cp.nLine-m_pData->m_nScanStartLine)/m_pData->m_nLineIncrement);
				SampleToClient(ptSample,&pt[nn]);
				nn==0 ? pByte[nn]=PT_MOVETO : pByte[nn]=PT_LINETO;
			}
			pt[nn] = pt[0];	pByte[nn++] = PT_LINETO;


			pPen = new CPen;
			pPen->CreatePen(PS_SOLID,0,pLines[nLine].m_rgb);
			

			pOldPen=pDC->SelectObject(pPen);
			pDC->PolyDraw(pt,pByte,nn);

			Buff.Format(L"%d",nLine + 1);
			pDC->TextOut(pt[0].x,pt[0].y,Buff);

			pDC->SelectObject(pOldPen);
			SAFE_DELETE( pPen );
			SAFE_DELETE( pt );
			SAFE_DELETE( pByte );
		}
	}


	rgn.SetRectRgn(&rect);
	pDC->SelectClipRgn(&rgn);
}



UINT CalculateEnvelopeLineAndSampleThread(LPVOID pParam)
{
	CViewPage* pPage = (CViewPage *)pParam;
	CData* pData = pPage->m_pData;
	pData->m_bThreadFinished=FALSE;
	CPolyCoord *pLines = pData->m_pEnvelopeLine;
	int nLineL = pData->m_nEnvelopeLineL;
	int nCoordL,nn;
	CCoord Cp;
	CFont Font;
	CPoint pt0;
	CPoint *pt = NULL;
	BYTE *pByte = NULL;
	CPoint ptSample;
	CString Buff;
	CRgn rgn;
	CRect rect,rr;
	CPen *pOldPen=NULL;
	CPen *pPen=NULL;
	int nS,nL;

	if(pLines==NULL) return -1;
	if(nLineL==0) return -1;


	//Draw lines
	for(int nLine = 0;nLine<nLineL;nLine++) {
		
		nCoordL = pLines[nLine].m_nCoordL;
		for(nn=0;nn<nCoordL;nn++) {
			pLines[nLine].GetCoord(nn,&Cp);

			if((Cp.nSample == 0) && (Cp.nLine == 0)) {
				pPage->WorldToLineSample(Cp, &nS, &nL);			//This bit is horrifically slow
				Cp.nSample = nS;
				Cp.nLine = nL;
				pLines[nLine].m_pCp[nn].nSample = nS;		// so the answer is storred for use next time
				pLines[nLine].m_pCp[nn].nLine = nL;
			}

		}
	}


	pData->m_bThreadEnabled=false;
	pData->m_bThreadFinished = true;

	return 0;
}

void CViewPage::PrintHeader(CPaintDC *pDC)
{
	CString Buff,Temp;
	CTime time(m_pData->m_osTestDateTime);
	int nPos;
	LOGFONT lf;

	CopyMemory(&lf,&theApp.m_LastSettings.lfViewGraticule,sizeof lf);
	CFont VerticalFont;
	lf.lfEscapement=-900;
	lf.lfHeight *= 12;
	VerticalFont.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&VerticalFont);

	int nMask = 1;
	Buff.Empty();
	for(int nn=0;nn<5;nn++, nMask<<=1) {
		if(theApp.m_LastSettings.nPrintOptionsMask & nMask) {

			switch(nn) {
			case 0:
				Temp.Format(L"File Path: %s",m_pData->m_FilePath);
				break;
			case 1:
				if((nPos = m_pData->m_FileName.ReverseFind('.')) >0) {
					Temp.Format(L"File Name: %s", m_pData->m_FileName.Left(nPos));
				} else {
					Temp.Format(L"File Name: %s",m_pData->m_FileName);
				}
				break;
			case 2:
				Temp.Format(L"Profile: %s",m_pData->m_ProfileName);
				break;
			case 3:
				Temp.Format(L"Operator: %s",m_pData->m_Operator);
				break;
			case 4:
				Temp.Format(L"Date: %s", time.Format(_T("%d/%m/%Y %Hh%M")));
				break;
			}

			Buff += Temp;
			Buff += " ";
		}

	}
	CSize size = pDC->GetTextExtent(Buff);
	pDC->TextOut(size.cy,50,Buff);

	pDC->SelectObject(pOldFont);



}

void CViewPage::TextBoxAtCursor(CPaintDC *pDC, CString Buff)
{
	CRect rr,rrStatic;
	CPoint ptCursor;
	CPaintDC *pdc = pDC;

	if(pDC==NULL) {
		pdc = (CPaintDC*)GetDC();
	}

	m_staticView.GetWindowRect(rrStatic);
	ScreenToClient(rrStatic);

	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	pdc->SetBkColor(GetSysColor(COLOR_INFOBK));
	pdc->SetBkMode(OPAQUE);
	CSize size = pdc->GetTextExtent(Buff);

	if((ptCursor.x + size.cx) < rrStatic.right) {
		rr.left = ptCursor.x;
	} else {
		rr.left = ptCursor.x - size.cx;
	};

	if((ptCursor.y + size.cy) < rrStatic.bottom) {
		rr.top = ptCursor.y;
	} else {
		rr.top = ptCursor.y - size.cy;
	};

	rr.bottom = rr.top + size.cy+2;
	rr.right = rr.left + size.cx+2;

	pdc->Rectangle(rr);
	pdc->TextOut(rr.left+1,rr.top+1,Buff);

	if(pDC == NULL) {
		ReleaseDC(pdc);
	}

}

void CViewPage::TextBoxAtPt(CPaintDC *pDC, CString Buff,CPoint pt)
{
	CRect rr,rrStatic;
	CPaintDC *pdc = pDC;

	if(pDC==NULL) {
		pdc = (CPaintDC*)GetDC();
	}

	m_staticView.GetWindowRect(rrStatic);
	ScreenToClient(rrStatic);

	pdc->SetBkColor(GetSysColor(COLOR_INFOBK));
	pdc->SetBkMode(OPAQUE);
	CSize size = pdc->GetTextExtent(Buff);

	if((pt.x + size.cx) < rrStatic.right) {
		rr.left = pt.x;
	} else {
		rr.left = pt.x - size.cx;
	};

	if((pt.y + size.cy) < rrStatic.bottom) {
		rr.top = pt.y;
	} else {
		rr.top = pt.y - size.cy;
	};

	rr.bottom = rr.top + size.cy+2;
	rr.right = rr.left + size.cx+2;

	pdc->Rectangle(rr);
	pdc->TextOut(rr.left+1,rr.top+1,Buff);

	if(pDC == NULL) {
		ReleaseDC(pdc);
	}

}

BOOL CViewPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	switch(m_nToolOption) {
	case TOOL_VALUE:
		zDelta>0 ? theApp.m_LastSettings.nValueDataType++ : theApp.m_LastSettings.nValueDataType--;
		MinMax(&theApp.m_LastSettings.nValueDataType, 0, 2);
		GetValueStr();
		Invalidate(false);
		m_bRejectWM_WHEEL = true;
		break;
	}
	
	return CResizablePage::OnMouseWheel(nFlags, zDelta, pt);
}

//This is here because the WM_MOUSEWHEEL doesn't get here
BOOL CViewPage::MouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CViewSheet *pViewSheet = (CViewSheet*)m_pParent;
	if (m_bRejectWM_WHEEL == true) return false;

	switch(pViewSheet->m_nToolOption) {
	case TOOL_VALUE:
		zDelta>0 ? theApp.m_LastSettings.nValueDataType++ : theApp.m_LastSettings.nValueDataType--;
		MinMax(&theApp.m_LastSettings.nValueDataType, 0, 2);
		GetValueStr();
		Invalidate(false);
		break;
	case TOOL_RULER:
		zDelta>0 ? theApp.m_LastSettings.nRulerWidth++ : theApp.m_LastSettings.nRulerWidth--;
		MinMax(&theApp.m_LastSettings.nRulerWidth,1,4);
		Invalidate(false);
	}

	return false;

}

void CViewPage::GetValueStr()
{
	CPoint pt;
	float fPos,fZ;
	CCoord Cp;
	
	switch(theApp.m_LastSettings.nValueDataType) {
	case 0:	ClientToSample(m_rrRuler.BottomRight(),&pt);
		m_strValue.Format(_T("Sample %d Line %d"),pt.x,pt.y);
		break;
	case 1:// ClientToWorld(m_rrRuler.BottomRight(),&fX1,&fY1);
		ClientToSample(m_rrRuler.BottomRight(),&pt);
		pt.y *= m_pData->m_nLineIncrement;
		pt.y+=m_pData->m_nScanStartLine;
		switch(m_pData->m_nCollectAxis) {
		default:
			Cp.Zero();
			ClientToCoord(m_rrRuler.BottomRight(),&Cp);
			if(theApp.m_LastSettings.nMeasurementUnits == 0) {
				m_strValue.Format(_T("X %.1f Y: %.1f Z %.1f"),Cp.Side0.fX,Cp.Side0.fY,Cp.Side0.fZ);
			} else {
				m_strValue.Format(_T("X %.03f Y: %.03f Z %.03f"),Cp.Side0.fX/25.4f,Cp.Side0.fY/25.4f,Cp.Side0.fZ/25.4f);
			}
			break;
		case 5:
			fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fR;
			fZ = m_pData->m_fSlowScanSize / (float)m_pData->m_nNumberLines * (float)pt.y;
			m_pData->GetCoordFromLineSample(pt, &Cp);
			if(theApp.m_LastSettings.nMeasurementUnits == 0) {
				m_strValue.Format(L"X: %.1f%s Y: %.1f%s Z: %.1f%s R %.1f%s",Cp.Side0.fX*theApp.m_fUnits,theApp.m_Units,
															Cp.Side0.fY*theApp.m_fUnits, theApp.m_Units, 
																Cp.Side0.fZ*theApp.m_fUnits, theApp.m_Units, fPos,DEGREES);
			} else {
				m_strValue.Format(_T("Z: %.03f%s R %.1f%s"),fZ*theApp.m_fUnits,theApp.m_Units,fPos,DEGREES);
			}
//			Buff.Format(_T("Z: %d R %.1f"),pt.y,fPos);
			break;
		}
		break;
	case 2:// ClientToWorld(m_rrRuler.BottomRight(),&fX1,&fY1);
		ClientToSample(m_rrRuler.BottomRight(),&pt);
		m_pData->GetStringValue(m_nImageType,m_nImageNumber,m_nPaletteNumber,m_nTS,pt,&m_strValue);
		break;
	}

}


LRESULT CViewPage::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	
	return CPropertyPage::DefWindowProc(message, wParam, lParam);
}

void CViewPage::OnPolygonNew() 
{
	m_nPolyPtr = m_pData->m_nPolygonL;
}

void CViewPage::OnPolygonClear() 
{

	m_pData->ClearPolygon(m_nPolyPtr);
	Invalidate(FALSE);
}

void CViewPage::OnPolygonClearall() 
{

	m_pData->ClearAllPolygons();
	m_nPolyPtr=0;
	Invalidate(FALSE);

}


void CViewPage::OnPolygonArea() 
{
	m_pData->SetPloygonLengthAreaMode(m_nPolyPtr, theApp.m_LastSettings.nPloygonLengthAreaMode = 1);
	Invalidate(FALSE);
	DisplayPolygonStats();

	
}

void CViewPage::OnPolygonLength() 
{
	m_pData->SetPloygonLengthAreaMode(m_nPolyPtr, theApp.m_LastSettings.nPloygonLengthAreaMode = 0);
	Invalidate(FALSE);
	DisplayPolygonStats();


}

void CViewPage::OnViewDisplayarea() 
{
	CViewSheet *pViewSheet = (CViewSheet*)m_pParent;

	pViewSheet->m_bRenderPolygons == true ? pViewSheet->m_bRenderPolygons = false : pViewSheet->m_bRenderPolygons = true;
	Invalidate(false);
}



void CViewPage::OnView3dlookatpoint() 
{
	CPoint pt;
	CCoord CpSurface;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CProfileSheet* pSheet = pFrame->m_pProfileSheet;
	float fPos,fZ;
	

	ClientToSample(m_ptClient,&pt);
	pt.y *= m_pData->m_nLineIncrement;
	pt.y+=m_pData->m_nScanStartLine;
	switch(m_pData->m_nCollectAxis) {
	default:
		CpSurface.Zero();
		ClientToCoord(m_ptClient,&CpSurface);
		break;
	case 5:
		fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fR;
		fZ = m_pData->m_fSlowScanSize / (float)m_pData->m_nNumberLines * (float)pt.y;
		break;
	}

	if(pSheet->m_p3DViewPage) {
		pSheet->m_p3DViewPage->LookAtPoint(CpSurface,false);
	}
}


void CViewPage::DisplayPolygonStats()
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	float fArea, fLength;
	CString Buff;

	if(m_pData->m_pPolygon != NULL) {
		if(m_pData->m_pPolygon[m_nPolyPtr].m_nClosedEnds == 1) {
			fArea = CalculateAreaOfPolygon();
			Buff.Format(_T("Area = %.1f mm\x0b2"),fArea);
		} else {
			fLength = m_pData->m_pPolygon[m_nPolyPtr].GetLength();
			Buff.Format(_T("Length = %.1f mm"),fLength);
		}
		pViewSheet->EditTextSetWindowText(Buff);
	}
}


void CViewPage::LimitRulerToDropOutSize()
{	
	float fArea,fAspectRatio,fWidth,fHeight;
	CPoint pt;
	CRect rrSample;

	ClientToSample(m_rrRuler.TopLeft(),&pt);
	rrSample.TopLeft() = pt;
	ClientToSample(m_rrRuler.BottomRight(),&pt);
	rrSample.BottomRight() = pt;

	//Check the Aspect ratio
	fWidth = fabs((float)rrSample.Width() * m_pData->m_fFastIncrement);
	fHeight = fabs((float)rrSample.Height() * m_pData->m_fSlowIncrement);

	fAspectRatio = fWidth / fHeight;
	if(fAspectRatio > theApp.m_LastSettings.fDropOutAspectRatio) {
		fHeight = fWidth / theApp.m_LastSettings.fDropOutAspectRatio;
		if(m_rrRuler.bottom>m_rrRuler.top) {
			rrSample.bottom = rrSample.top + (int)(fHeight / m_pData->m_fSlowIncrement);
		} else {
			rrSample.bottom = rrSample.top - (int)(fHeight / m_pData->m_fSlowIncrement);
		}
		SampleToClient(rrSample.BottomRight(),&pt);
		m_rrRuler.BottomRight() = pt;
	} else {
		fAspectRatio = fHeight / fWidth;
		if(fAspectRatio > theApp.m_LastSettings.fDropOutAspectRatio) {
			fWidth = fHeight / theApp.m_LastSettings.fDropOutAspectRatio;
			if(m_rrRuler.right>m_rrRuler.left) {
				rrSample.right = rrSample.left + (int)(fWidth / m_pData->m_fFastIncrement);
			} else {
				rrSample.right = rrSample.left - (int)(fWidth / m_pData->m_fFastIncrement);
			}
			SampleToClient(rrSample.BottomRight(),&pt);
			m_rrRuler.BottomRight() = pt;
		}
	}
	//Check the area;
	fWidth = fabs((float)rrSample.Width() * m_pData->m_fFastIncrement);
	fHeight = fabs((float)rrSample.Height() * m_pData->m_fSlowIncrement);
	fArea = fWidth * fHeight;
	fAspectRatio = fWidth / fHeight;
	if(fArea > theApp.m_LastSettings.fDropOutArea) {
		fWidth = sqrtf(theApp.m_LastSettings.fDropOutArea) * fAspectRatio;
		fHeight = sqrtf(theApp.m_LastSettings.fDropOutArea) / fAspectRatio;
		if(rrSample.bottom > rrSample.top) {
			rrSample.bottom = rrSample.top + (int)(fHeight / m_pData->m_fSlowIncrement);
		} else {
			rrSample.bottom = rrSample.top - (int)(fHeight / m_pData->m_fSlowIncrement);
		}
		if(rrSample.right > rrSample.left) {
			rrSample.right = rrSample.left + (int)(fWidth / m_pData->m_fFastIncrement);
		} else {
			rrSample.right = rrSample.left - (int)(fWidth / m_pData->m_fFastIncrement);
		}
		SampleToClient(rrSample.BottomRight(),&pt);
		m_rrRuler.BottomRight() = pt;
	}
				
}


void CViewPage::RenderTransparentRectangle(CDC *pDC, CRect *rr)
{
	CPoint pt[5];

	pt[4] = pt[0] = rr->TopLeft();
	pt[1].x = rr->right;
	pt[1].y = rr->top;
	pt[2] = rr->BottomRight();
	pt[3].x = rr->left;
	pt[3].y = rr->bottom;

	pDC->Polyline(pt,5);
}

void CViewPage::RenderTransparentEllipse(CDC *pDC, CRect *rr)
{
	pDC->SelectStockObject(HOLLOW_BRUSH);
	pDC->Ellipse(rr);
	pDC->SelectStockObject(WHITE_BRUSH);

}

void CViewPage::RenderTransparentParallelogram(CPaintDC *pDC, CPoint *pPt)
{
	CPoint ptArray[5];

	CopyMemory(ptArray, pPt, (sizeof CPoint) * 4);
	ptArray[4]=ptArray[0];
	pDC->Polyline(ptArray,5);
}

#define	BSIZE	6
void CViewPage::CADElementSizeArrows()
{
	CRect rr;
	CCoord Cp;
	CPoint pt[4];
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	int ii;


	if(pViewSheet->m_Data.m_nCADElementL>0) {
		switch(pViewSheet->m_Data.m_pCADElement[pViewSheet->m_Data.m_nCADPtr].m_Type) {
		default:
			for(ii=0;ii<2;ii++) {
				pViewSheet->m_Data.m_pCADElement[pViewSheet->m_Data.m_nCADPtr].m_Nodes.GetCoord(ii,&Cp);
				SampleToClient(Cp.fSample,Cp.fLine,&pt[ii]);
			};
			rr.TopLeft() = pt[0];
			rr.BottomRight() = pt[1];

			m_rrCursor[0].SetRect(rr.left-BSIZE,rr.top-BSIZE,rr.left,rr.top);

			m_rrCursor[1].SetRect(rr.CenterPoint().x-BSIZE/2,rr.top-BSIZE,rr.CenterPoint().x+BSIZE/2,rr.top);

			m_rrCursor[2].SetRect(rr.right,rr.top-BSIZE,rr.right+BSIZE,rr.top);

			m_rrCursor[3].SetRect(rr.right,rr.CenterPoint().y-BSIZE/2,rr.right+BSIZE,rr.CenterPoint().y+BSIZE/2);

			m_rrCursor[4].SetRect(rr.right,rr.bottom,rr.right+BSIZE,rr.bottom+BSIZE);

			m_rrCursor[5].SetRect(rr.CenterPoint().x-BSIZE/2,rr.bottom,rr.CenterPoint().x+BSIZE/2,rr.bottom+BSIZE);

			m_rrCursor[6].SetRect(rr.left-BSIZE,rr.bottom,rr.left,rr.bottom+BSIZE);

			m_rrCursor[7].SetRect(rr.left-BSIZE,rr.CenterPoint().y-BSIZE/2,rr.left,rr.CenterPoint().y+BSIZE/2);

			m_rrCursor[8] = rr;
			m_rrCursor[9].SetRect(rr.left-BSIZE,rr.top-BSIZE,rr.right+BSIZE,rr.bottom+BSIZE);
			break;
		case CAD_CENTRELINE:
		case CAD_PARALLELOGRAM:
			for(ii=0;ii<4;ii++) {
				pViewSheet->m_Data.m_pCADElement[pViewSheet->m_Data.m_nCADPtr].m_Nodes.GetCoord(ii,&Cp);
				SampleToClient(Cp.fSample,Cp.fLine,&pt[ii]);
			}

			m_rrCursor[0].SetRect(pt[0].x-BSIZE,pt[0].y-BSIZE,pt[0].x,pt[0].y);

			m_rrCursor[1].SetRect((pt[0].x+pt[1].x)/2-BSIZE/2,(pt[0].y+pt[1].y)/2-BSIZE,(pt[0].x+pt[1].x)/2+BSIZE/2,(pt[0].y+pt[1].y)/2);

			m_rrCursor[2].SetRect(pt[1].x,pt[1].y-BSIZE,pt[1].x+BSIZE,pt[1].y);

			m_rrCursor[3].SetRect((pt[1].x+pt[2].x)/2,(pt[1].y+pt[2].y)/2-BSIZE/2,(pt[1].x+pt[2].x)/2+BSIZE,(pt[1].y+pt[2].y)/2+BSIZE/2);

			m_rrCursor[4].SetRect(pt[2].x,pt[2].y,pt[2].x+BSIZE,pt[2].y+BSIZE);

			m_rrCursor[5].SetRect((pt[2].x+pt[3].x)/2-BSIZE/2,(pt[2].y+pt[3].y)/2,(pt[2].x+pt[3].x)/2+BSIZE/2,(pt[2].y+pt[3].y)/2+BSIZE);

			m_rrCursor[6].SetRect(pt[3].x-BSIZE,pt[3].y,pt[3].x,pt[3].y+BSIZE);

			m_rrCursor[7].SetRect((pt[3].x+pt[0].x)/2-BSIZE,(pt[3].y+pt[0].y)/2-BSIZE/2,(pt[3].x+pt[0].x)/2,(pt[3].y+pt[0].y)/2+BSIZE/2);

			pt[0].x<pt[3].x ? m_rrCursor[8].left = pt[0].x : m_rrCursor[8].left = pt[3].x;
			pt[0].y<pt[1].y ? m_rrCursor[8].top = pt[0].y : m_rrCursor[8].top = pt[1].y;

			pt[1].x>pt[2].x ? m_rrCursor[8].right = pt[1].x : m_rrCursor[8].right = pt[2].x;
			pt[2].y>pt[3].y ? m_rrCursor[8].bottom = pt[2].y : m_rrCursor[8].bottom = pt[3].y;
			
			m_rrCursor[9].SetRect(m_rrCursor[8].left-BSIZE,m_rrCursor[8].top-BSIZE,m_rrCursor[8].right+BSIZE,m_rrCursor[8].bottom+BSIZE);
			break;
		}
	}

}

void CViewPage::RenderCADElementSizeArrows(CDC *pDC)
{
	CBrush brushBlue(RGB(0,0,255));
	CPen pen(PS_SOLID,1,RGB(0,0,255));
	CPen *pOldPen = pDC->SelectObject(&pen);
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	int ii;

	if(pViewSheet->m_Data.m_nCADElementL>0) {
		switch(pViewSheet->m_Data.m_pCADElement[pViewSheet->m_Data.m_nCADPtr].m_Type) {
		default:
			for(ii=0;ii<8;ii++) {
				pDC->Rectangle(m_rrCursor[ii]);
			}
		case CAD_CENTRELINE:
			int pts[2] = {3,7};
			for(ii=0;ii<2;ii++) {
				pDC->Rectangle(m_rrCursor[pts[ii]]);
			}
			break;
		}
	}
	pDC->SelectObject(pOldPen);
}

void CViewPage::OnCadSnaptonearestnode() 
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	m_pData->SnapToNearestCADElement(m_pData->m_nCADPtr,m_nMoveCursor);
	CADElementSizeArrows();
	pViewSheet->RefreshCADTable();
	Invalidate(false);
}


void CViewPage::OnCadSetheightwidth() 
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	m_pData->SetHeightSameWidthCADElement(m_pData->m_nCADPtr);
	CADElementSizeArrows();
	pViewSheet->RefreshCADTable();
	Invalidate(false);
}

void CViewPage::OnCadSetwidthheight() 
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	m_pData->SetWidthSameHeightCADElement(m_pData->m_nCADPtr);
	CADElementSizeArrows();
	pViewSheet->RefreshCADTable();
	Invalidate(false);
}

void CViewPage::RefreshHistogram()
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	if (pViewSheet->m_pHistogramDlg == NULL) {
		pViewSheet->m_pHistogramDlg = new CHistogramDlg(pViewSheet,(CWnd**)&pViewSheet->m_pHistogramDlg,m_nImageType,m_nImageNumber,m_nPaletteNumber,m_pData,m_nTS);
		pViewSheet->m_pHistogramDlg->Create(CHistogramDlg::IDD,pViewSheet);
	}
	pViewSheet->m_pHistogramDlg->m_nImageNumber = m_nImageNumber;
	pViewSheet->m_pHistogramDlg->m_nImageType = m_nImageType;
	pViewSheet->m_pHistogramDlg->m_nTS = m_nTS;
	pViewSheet->m_pHistogramDlg->Invalidate(FALSE);

}

void CViewPage::RefreshThicknessCrossSection(int nS0,int nL0, int nS1, int nL1)
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	if (pViewSheet->m_pViewThicknessCrossSectionDlg) {
		pViewSheet->m_pViewThicknessCrossSectionDlg->m_nImageNumber = m_nImageNumber;
		pViewSheet->m_pViewThicknessCrossSectionDlg->m_nImageType = m_nImageType;
		pViewSheet->m_pViewThicknessCrossSectionDlg->m_nTS = m_nTS;
		pViewSheet->m_pViewThicknessCrossSectionDlg->SelectCurrentSampleLine(nS0, nL0, nS1, nL1);
		pViewSheet->m_pViewThicknessCrossSectionDlg->RenderView();
	}

}


void CViewPage::OnCadProperties() 
{
}


void CViewPage::RenderCADElementLine(CPaintDC* pDC, CRect rr,int nCADPtr,int nDevice)
{

	CSize size;
	int nFlags;
	CPoint pt;
	float fLength = 12;
	LOGFONT lf;
	CString Buff;
	CFont Font;

	nFlags = m_pData->m_pCADElement[nCADPtr].m_nFlags;
	size = pDC->GetTextExtent(m_pData->m_pCADElement[nCADPtr].m_wText);

	pDC->MoveTo(rr.TopLeft());
	pDC->LineTo(rr.BottomRight());

	float fAngle = atan2f((float)rr.Height(),(float)rr.Width());
	float f45Degrees = PIf / 4.0f;

	if(nFlags & CAD_START_ARROW) {
		pt.x = rr.left + (int)(fLength * cosf(fAngle + f45Degrees));
		pt.y = rr.top + (int)(fLength * sinf(fAngle + f45Degrees));
		pDC->MoveTo(rr.TopLeft());
		pDC->LineTo(pt);
		pt.x = rr.left + (int)(fLength * cosf(fAngle - f45Degrees));
		pt.y = rr.top + (int)(fLength * sinf(fAngle - f45Degrees));
		pDC->MoveTo(rr.TopLeft());
		pDC->LineTo(pt);
	};

	if(nFlags & CAD_FINISH_ARROW) {
		pt.x = rr.right - (int)(fLength * cosf(fAngle + f45Degrees));
		pt.y = rr.bottom - (int)(fLength * sinf(fAngle + f45Degrees));
		pDC->MoveTo(rr.BottomRight());
		pDC->LineTo(pt);
		pt.x = rr.right - (int)(fLength * cosf(fAngle - f45Degrees));
		pt.y = rr.bottom - (int)(fLength * sinf(fAngle - f45Degrees));
		pDC->MoveTo(rr.BottomRight());
		pDC->LineTo(pt);
	};

	if(nFlags & CAD_LINE_TEXT) {
		CopyMemory(&lf, &m_pData->m_pCADElement[nCADPtr].m_lf, sizeof lf);
		lf.lfEscapement=(long)(fAngle * RAD_TO_DEG * -10.0);

//	CRect rrClient;
//	GetClientRect(rrClient);
//	lf.lfHeight = RoundDownMulDiv(rrClient.Height(),12,m_rrClient.Height());

		Font.CreateFontIndirect(&lf);
		CFont* pOldFont = pDC->SelectObject(&Font);

		m_pData->GetCADElementLength(nCADPtr,&Buff,false);
		size = pDC->GetTextExtent(Buff);

		pt.x = rr.CenterPoint().x - (int)((float)(size.cx/2) * cosf(fAngle));
		pt.y = rr.CenterPoint().y - (int)((float)(size.cx/2) * sinf(fAngle));

		pDC->TextOut(pt.x,pt.y,Buff);

		pDC->SelectObject(pOldFont);

	}


}

void CViewPage::RenderCADElementCentreLine(CPaintDC *pDC, CPoint *pPt,int nCADPtr,int nDevice)
{

	CSize size;
	CPoint ptArray[5];	
	CRect rr;	
	LOGFONT lf;
	CString Buff;
	CFont Font;
	CPoint pt;


	CopyMemory(ptArray, pPt, (sizeof CPoint) * 4);
	
	ptArray[4]=ptArray[0];
	
	rr.SetRect(ptArray[1],ptArray[0]);
	CPoint ptLeftCentre = rr.CenterPoint();
	
	rr.SetRect(ptArray[2],ptArray[3]);
	CPoint ptRightCentre = rr.CenterPoint();


	pDC->MoveTo(ptArray[1]);
	pDC->LineTo(ptArray[2]);
	pDC->MoveTo(ptArray[3]);
	pDC->LineTo(ptArray[0]);

	pDC->MoveTo(ptLeftCentre);
	pDC->LineTo(ptRightCentre);

	rr.SetRect(ptLeftCentre,ptRightCentre);
	float fAngle = atan2f((float)rr.Height(),(float)rr.Width());
	float f45Degrees = PIf / 4.0f;

	int nFlags = m_pData->m_pCADElement[nCADPtr].m_nFlags;
	size = pDC->GetTextExtent(m_pData->m_pCADElement[nCADPtr].m_wText);


	if(nFlags & CAD_LINE_TEXT) {
		CopyMemory(&lf, &m_pData->m_pCADElement[nCADPtr].m_lf, sizeof lf);
		lf.lfEscapement=(long)(((fAngle * RAD_TO_DEG)-90) * -10.0);

		Font.CreateFontIndirect(&lf);
		CFont* pOldFont = pDC->SelectObject(&Font);

		m_pData->GetCADElementWidth(nCADPtr,&Buff,false);
		size = pDC->GetTextExtent(Buff);

		rr.SetRect(ptLeftCentre,ptRightCentre);

		pt.x = rr.CenterPoint().x - (size.cx/2);// * sinf(fAngle));
		pt.y = rr.CenterPoint().y;// - ((size.cx/2) * cosf(fAngle));

		pDC->TextOut(pt.x,pt.y,Buff);

		pDC->SelectObject(pOldFont);
	}




}


void CViewPage::OnViewOpeninnewwindow() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CViewSheet* pParent = (CViewSheet*)m_pParent;

	pParent->OpenChildView();

}



void CViewPage::OnViewGotowithLiftoff() 
{
	CCoord	Cp,CpSurface,CpHead;
	CPoint pt;
	CString Buff,Warning;

	ClientToSample(m_ptClient,&pt);
	pt.y *= m_pData->m_nLineIncrement;
	pt.y+=m_pData->m_nScanStartLine;
	float fSize = m_pData->m_fFastScanSize;
	fSize = m_pData->m_CpFinish.Side0.fPos[m_pData->m_nFastAxis] - m_pData->m_CpStart.Side0.fPos[m_pData->m_nFastAxis];

	float fPos = ((float)pt.x * fSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fPos[m_pData->m_nCollectAxis];

	ClientToSample(m_ptClient,&pt);
	pt.y *= m_pData->m_nLineIncrement;
	pt.y+=m_pData->m_nScanStartLine;

	if(m_pData->m_ProfileName == PROFILE->m_FileName) {

		if(m_pData->m_nComponentDescription == RECTILINEAR) {
			ClientToCoord(m_ptClient,&CpSurface);
			PROFILE->GetSurfaceCoordinate(CpSurface,pt,CpSurface.fPos[m_pData->m_nFastAxis],m_pData->m_nFastAxis);
			theApp.m_Motors.GotoSurfaceCoord(&CpSurface,MOVE_LEFT);
		}

	} else {
		Buff.LoadString(IDS_Please_Load_Appropriate_Profile);
		Warning.LoadString(IDS_Error);
		MessageBox(Buff,Warning,MB_ICONERROR);
	}
	
}


void CViewPage::OnZoomEnlarge() 
{
	CViewSheet* pParent = (CViewSheet*)m_pParent;
	CRect rrSample;
	CPoint pt;
	float fZoomFactor;

	if( (theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2) ){
		ClientToSample(m_rrRuler.TopLeft(),&pt);
		rrSample.TopLeft() = pt;
		ClientToSample(m_rrRuler.BottomRight(),&pt);
		rrSample.BottomRight() = pt;
	} else {
		ClientToSample(m_rrRuler.TopLeft(),&pt);
		rrSample.left = pt.x;
		rrSample.bottom = pt.y;
		ClientToSample(m_rrRuler.BottomRight(),&pt);
		rrSample.right = pt.x;
		rrSample.top = pt.y;
	}

	m_prrSamples->TopLeft()		= rrSample.TopLeft();
	m_prrSamples->BottomRight() = rrSample.BottomRight();

	fZoomFactor = (float)m_prrSamples->Width() / (float)m_pData->m_nSamplesLine;
	pParent->m_nZoomFactor = (int)(fZoomFactor * 1000.0f);
	m_bRenderView=TRUE;
	Invalidate(false);
	SetZoomSamples();
	m_rrRuler = CRect(0,0,0,0);
}

void CViewPage::OnZoomFullscan() 
{
	CViewSheet* pParent = (CViewSheet*)m_pParent;

	pParent->m_nZoomFactor = 1000;
	m_bRenderView=TRUE;
	Invalidate(false);
}

void CViewPage::RefreshInclusions()
{
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	if (pViewSheet->m_pInclusionsSheet == NULL) {
//		pViewSheet->m_pInclusionsSheet = new CHistogramDlg(pViewSheet,(CWnd**)&pViewSheet->m_pHistogramDlg,m_nImageType,m_nImageNumber,m_nPaletteNumber,m_pData,m_nTS);
//		pViewSheet->m_pInclusionsSheet->Create(CHistogramDlg::IDD,pViewSheet);
	} else {
	
		pViewSheet->m_pInclusionsSheet->UpdateAllPages();
	}
}

void CViewPage::DrawInclusions(CPaintDC* pDC)
{
	CPoint ptClient[2],ptSample;

	if(m_nToolOption!=TOOL_INCLUSION) return;
	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;

	CPen pen(PS_SOLID,theApp.m_LastSettings.nRulerWidth,theApp.m_LastSettings.rgbRuler);

	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(m_rrRuler.TopLeft());
	pDC->LineTo(m_rrRuler.right,m_rrRuler.top);
	pDC->LineTo(m_rrRuler.BottomRight());
	pDC->LineTo(m_rrRuler.left,m_rrRuler.bottom);
	pDC->LineTo(m_rrRuler.TopLeft());

	for(int ii=0;ii<m_pData->m_Inclusions.GetDefectSize();ii++) {
		DefectStruct* pDef = m_pData->m_Inclusions.GetPtrDefect(ii);
		ptSample.x = pDef->nX[0];
		ptSample.y = pDef->nY[0];
		SampleToClient(ptSample,&ptClient[0]);
		ptSample.x = pDef->nX[1];
		ptSample.y = pDef->nY[1];
		SampleToClient(ptSample,&ptClient[1]);

		pDC->MoveTo(ptClient[0].x,ptClient[0].y);
		pDC->LineTo(ptClient[1].x,ptClient[1].y);
		pDC->MoveTo(ptClient[1].x,ptClient[0].y);
		pDC->LineTo(ptClient[0].x,ptClient[1].y);
	}


	pDC->SelectObject(pOldPen);
}

int CViewPage::SegmentIntersectionPt(float p0_x, float p0_y, float p1_x, float p1_y, 
    float p2_x, float p2_y, float p3_x, float p3_y, CPoint* pt)
{
    float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
	float i_x, i_y;
    s10_x = p1_x - p0_x;
    s10_y = p1_y - p0_y;
    s32_x = p3_x - p2_x;
    s32_y = p3_y - p2_y;

	pt->x = pt->y = 0;

    denom = s10_x * s32_y - s32_x * s10_y;
    if (denom == 0)
        return 0; // Collinear
    bool denomPositive = denom > 0;

    s02_x = p0_x - p2_x;
    s02_y = p0_y - p2_y;
    s_numer = s10_x * s02_y - s10_y * s02_x;
    if ((s_numer < 0) == denomPositive)
        return 0; // No collision

    t_numer = s32_x * s02_y - s32_y * s02_x;
    if ((t_numer < 0) == denomPositive)
        return 0; // No collision

    if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
        return 0; // No collision
    // Collision detected
    t = t_numer / denom;
    i_x = p0_x + (t * s10_x);
    i_y = p0_y + (t * s10_y);

	pt->x = (int)i_x;
	pt->y = (int)i_y;

    return 1;
}

void CViewPage::Vector2IntersectionPt(D3DXVECTOR2 Line1Point1, D3DXVECTOR2 Line1Point2, D3DXVECTOR2 Line2Point1, D3DXVECTOR2 Line2Point2, D3DXVECTOR2* Result)
{
	
    D3DXVECTOR2 V1, V2, V3;
	D3DXVec2Subtract(&V1, &Line1Point2, &Line1Point1);
    D3DXVec2Subtract(&V2, &Line2Point2, &Line2Point1);
    D3DXVec2Subtract(&V3, &Line1Point1, &Line2Point1);

    float C1,C2;
	C1 = D3DXVec2CCW( &V1, &V2);
    C2 = D3DXVec2CCW( &V2, &V3);


	if(C1 == 0.0f) {
		if(C2==0.0f) { //Coincident
			Result->x = 1e32f;
			Result->y = 1e32f;
		} else {	//Parallel
			Result->x = sqrtf(-1.0f);
			Result->y = sqrtf(-1.0f);
		}
	} else {


		float fScale = C2 / C1;
		V1.x *= fScale;
		V1.y *= fScale;


		Result->x = Line1Point1.x + V1.x;
		Result->y = Line1Point1.y + V1.y;
	}

}


