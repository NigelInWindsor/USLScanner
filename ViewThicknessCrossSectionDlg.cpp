// ViewThicknessCrossSectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ViewThicknessCrossSectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewThicknessCrossSectionDlg dialog

#define	HORIZ_GRAT			1
#define	BAR_GRAPH			2
#define	RELATIVE_ABSOLUTE	4
#define	LINEAR_DB			0x0008
#define	LOG_DB				0x0010
#define	PERCENTAGE			0x0020
#define	THICKNESS			0x0040



CViewThicknessCrossSectionDlg::CViewThicknessCrossSectionDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nImageType,int nImageNumber,int nPaletteNumber,CData* pData,int nTS,CRect *prrSamples)
	: CDialog(CViewThicknessCrossSectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewThicknessCrossSectionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pData = pData;
	m_nImageNumber = nImageNumber;
	m_nImageType = nImageType;
	m_nTS = nTS;
	m_nPaletteNumber = nPaletteNumber;
	m_prrSamples = prrSamples;
	
	m_ptLine[0] = CPoint(0,m_pData->m_nNumberLines / 2);
	m_ptLine[1] = CPoint(m_pData->m_nSamplesLine-1,m_pData->m_nNumberLines / 2);
	m_nCursorL = 1;
	m_nCursorPtr = 0;
	m_pImportCrossSectionOverlayDlg = NULL;

	m_dMinThickness = 0.0f;
	m_dMaxThickness = theApp.m_Palette.m_dThicknessRange[0];

	m_nMinAmplitude = 0;
	m_nMaxAmplitude = 100;

	m_bLbDown = false;

	m_pfThickness = new float[m_pData->m_nSamplesLine];
	m_pnAmplitude = new int[m_pData->m_nSamplesLine];

	m_pArray = NULL;

}


void CViewThicknessCrossSectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewThicknessCrossSectionDlg)
	DDX_Control(pDX, IDC_SLIDER_THICKNESS, m_sliderThickness);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewThicknessCrossSectionDlg, CDialog)
	//{{AFX_MSG_MAP(CViewThicknessCrossSectionDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_DOUBLE_CURSOR, OnButtonDoubleCursor)
	ON_COMMAND(ID_BUTTON_SINGLE_CURSOR, OnButtonSingleCursor)
	ON_COMMAND(ID_BUTTON_HORIZONTAL_GRATICULE, OnButtonHorizontalGraticule)
	ON_WM_VSCROLL()
	ON_COMMAND(ID_BUTTON_BARGRAPH, OnButtonBargraph)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CROSSSECTION_COLOURS_TRACE, OnCrosssectionColoursTrace)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_BACKGROUND, OnCrosssectionColoursBackground)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_MAXIMUM, OnCrosssectionColoursMaximum)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_NOMINAL, OnCrosssectionColoursNominal)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_MINIMUM, OnCrosssectionColoursMinimum)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_GRATICULE, OnCrosssectionColoursGraticule)
	ON_COMMAND(ID_BUTTON_AMP, OnButtonAmp)
	ON_COMMAND(ID_BUTTON_TOF, OnButtonTof)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_TOFTRACE, OnCrosssectionColoursToftrace)
	ON_COMMAND(ID_BUTTON_OVERLAY, OnButtonOverlay)
	ON_COMMAND(ID_BUTTON_RELATIVE_ABSOLUTE, OnButtonRelativeAbsolute)
	ON_COMMAND(ID_BUTTON_LINEAR_DB, OnButtonLinearDb)
	ON_COMMAND(ID_BUTTON_LOG, OnButtonLog)
	ON_COMMAND(ID_BUTTON_PERCENTAGE, OnButtonPercentage)
	ON_COMMAND(ID_BUTTON_THICKNESS, OnButtonThickness)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewThicknessCrossSectionDlg message handlers

int CViewThicknessCrossSectionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;
	int nn;
	CRect rrItem,rrImage;
	CSize siItem,siImage;
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolsToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndToolsToolBar.LoadToolBar(IDR_VIEW_THICKNESS_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	} else {
		nn = m_wndToolsToolBar.GetCount();
//		m_wndToolsToolBar.GetToolBarCtrl().Set
	}

	int nButton = 0;
	for(int nTS=0;nTS<8;nTS++) {
		for(int gg=0;gg<m_pData->m_nNumberAmpImages[nTS];gg++) {
			if (!m_wndGateToolBar[nButton].CreateEx(this,
				TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
				WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
				CRect(2,2,0,0)) ||	!m_wndGateToolBar[nButton].LoadToolBar(IDR_AMP_TOOLBAR)) {
					TRACE0("Failed to create toolbar\n");
				return -1;      // fail to create
			} else {
				nButton++;
			}
		}
		for(int tt=0;tt<m_pData->m_nNumberTimeImages[nTS];tt++) {
			if (!m_wndGateToolBar[nButton].CreateEx(this,
				TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
				WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
				CRect(2,2,0,0)) ||	!m_wndGateToolBar[nButton].LoadToolBar(IDR_TOF_TOOLBAR)) {
					TRACE0("Failed to create toolbar\n");
				return -1;      // fail to create
			} else {
				nButton++;
			}
		}
	}


	GetClientRect(rect);

	if (!m_wndRebar.Create(this,NULL,CBRS_ALIGN_TOP | AFX_IDW_DIALOGBAR)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,400,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolsToolBar);
	for(int ii=0;ii<nButton;ii++) {
		m_wndRebar.AddBar(&m_wndGateToolBar[ii]);
	}

	return 0;
}

void CViewThicknessCrossSectionDlg::PostNcDestroy() 
{
	delete m_pArray;
	delete m_pfThickness;
	delete m_pnAmplitude;

	CDialog::PostNcDestroy();
	delete this;
}

void CViewThicknessCrossSectionDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CViewThicknessCrossSectionDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=FALSE;

	
	CDialog::OnClose();
	DestroyWindow();
}

void CViewThicknessCrossSectionDlg::OnDestroy() 
{
	

	GetWindowRect(theApp.m_LastSettings.RectWindowPos[(CViewThicknessCrossSectionDlg::IDD) % MAXIMUMWINDOWSIDNUMBER]);
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CViewThicknessCrossSectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;
	

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	m_StaticSliderThickness.Initialize(this,m_sliderThickness,TRUE,TRUE,TRUE,FALSE);

	CRect rect = theApp.m_LastSettings.RectWindowPos[(CViewThicknessCrossSectionDlg::IDD) % MAXIMUMWINDOWSIDNUMBER];
	theApp.ConfineToScreen(&rect);
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);

	m_StaticPosView.SetNewPos();
	m_StaticSliderThickness.SetNewPos();

	m_sliderThickness.SetRange(0,(int)(m_dMaxThickness * 20.0));
	m_sliderThickness.SetPos((int)(m_dMaxThickness * 10.0));

	AllocateScreenArray();
	
	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);

	SetToolBarCheckedState();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewThicknessCrossSectionDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosView.SetNewPos();
	m_StaticSliderThickness.SetNewPos();

	if(GetSafeHwnd()) {
		AllocateScreenArray();
		Invalidate(FALSE);
	}
	
}

void CViewThicknessCrossSectionDlg::AllocateScreenArray()
{
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	SAFE_DELETE(m_pArray);
	m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
	m_nArrayWidth = rr.Width();
	m_nArrayHeight = rr.Height();

}

void CViewThicknessCrossSectionDlg::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting

	RenderView();
}


void CViewThicknessCrossSectionDlg::RenderView()
{
	
	CDC* pDC = GetDC();

	CDC  dcMem;
	CBitmap bmp;
	CRect rr;
	CString Buff;

	m_staticView.GetWindowRect(&m_rrView);
	ScreenToClient(&m_rrView);
	m_rrView.DeflateRect(1,1);

	bmp.CreateCompatibleBitmap(pDC,m_rrView.Width(),m_rrView.Height());
	bmp.SetBitmapBits((DWORD)(m_rrView.Width()*m_rrView.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	
	rr=m_rrView;
	rr.OffsetRect(-m_rrView.left,-m_rrView.top);
	dcMem.FillRect(&rr,&CBrush(theApp.m_LastSettings.rgbCrossSection[RGB_BKGROUND]));

	rr=m_rrView;
	rr.OffsetRect(-m_rrView.left,-m_rrView.top);

	Buff.Format(_T("%.01f"),-99.99);
	CSize size = dcMem.GetTextExtent(Buff);
	m_rrGraticule=rr;
	m_rrGraticule.left += size.cx;
	m_rrGraticule.right -= size.cx;
	m_rrGraticule.top += (2 * size.cy);
	m_rrGraticule.bottom -= ((m_nCursorL +1) * size.cy);

	DrawGraticule(&dcMem);

	int nMask = 1;
	int nAmp = 0;
	int nTOF = 0;
	for(int nTS=0;nTS<8;nTS++) {
		for(int gg=0;gg<m_pData->m_nNumberAmpImages[nTS];gg++,nMask<<=1,nAmp++) {
			if(theApp.m_LastSettings.nCrossSectionMask & nMask) {
				m_nImageType = AMP_IMAGE;
				m_nImageNumber = nAmp;
				DrawCrossSection(&dcMem, AMP_IMAGE);
			}
		}
		for(int tt=0;tt<m_pData->m_nNumberTimeImages[nTS];tt++,nMask<<=1,nTOF++) {
			if(theApp.m_LastSettings.nCrossSectionMask & nMask) {
				m_nImageType = TIME_IMAGE;
				m_nImageNumber = nTOF;
				DrawCrossSection(&dcMem, TIME_IMAGE);
			}
		}
	}
	DrawBarGraph(&dcMem);
	DrawSampleCursor(&dcMem);

	
	if(m_pImportCrossSectionOverlayDlg != NULL) {
		DrawOverlay(&dcMem);
	}


	pDC->BitBlt(m_rrView.left,m_rrView.top,m_rrView.Width(),m_rrView.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);



	ReleaseDC(pDC);

}


void CViewThicknessCrossSectionDlg::DrawGraticule(CDC *pDC)
{
	
	CString Buff;
	CSize size;
	CRect rr;
	CPen penWhite(PS_SOLID, 1, RGB(255,255,255));
	CPen penHorizontal(PS_SOLID, 1, theApp.m_LastSettings.rgbCrossSection[RGB_GRATICULE]);
	CPen* pOldPen = pDC->SelectObject(&penWhite);
	CFont HorizontalFont;
	CFont VerticalFont;
	CFont* pOldFont;
	float fFastScanSize,fFastIncrement,fDivisor,xx;
	float fXSize,fStep;
	int	nTextStep,ii,pnx,pny;
	double dStep;
	int nRow;
	CPoint pt;
	CCoord Cp;
	float fdB,fPercentage;

	rr=m_rrView;
	rr.OffsetRect(-m_rrView.left,-m_rrView.top);

	Buff.Format(_T("%.01f"),-99.99);
	size = pDC->GetTextExtent(Buff);
//	m_rrGraticule=rr;
//	m_rrGraticule.left += size.cx;
//	m_rrGraticule.right -= size.cx;
//	m_rrGraticule.top += (2 * size.cy);
//	m_rrGraticule.bottom -= ((m_nCursorL +1) * size.cy);


	pDC->MoveTo(m_rrGraticule.TopLeft());
	pDC->LineTo(m_rrGraticule.left,m_rrGraticule.bottom);
	pDC->LineTo(m_rrGraticule.BottomRight());
	pDC->LineTo(m_rrGraticule.right,m_rrGraticule.top);

	fXSize = hypot(m_pData->m_fFastIncrement * (m_ptLine[0].x-m_ptLine[1].x), m_pData->m_fSlowIncrement * (m_ptLine[0].y-m_ptLine[1].y));

	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);
	theApp.m_LastSettings.nMeasurementUnits==0 ? fDivisor = 10.0f : fDivisor = 12.0f;

	float xStart = 0.0f;
	fStep = 10.0f;
	nTextStep = 50;

	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);

	pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));

	//******** Horizontal graticule
	if(!(theApp.m_LastSettings.nThicknessDisplayOptions & RELATIVE_ABSOLUTE)) {
		for(xx=0,ii=0;xx<fXSize;xx+=fStep,ii++) {
			pnx=MulDiv((int)xx,m_rrGraticule.Width()-1,(int)fXSize)+m_rrGraticule.left;
			pDC->MoveTo(pnx,m_rrGraticule.bottom);
			pDC->LineTo(pnx,m_rrGraticule.bottom+4);
			if(((int)xx%nTextStep) == 0) {
				if(theApp.m_LastSettings.nGraticuleScrollMode==0) {
					Buff.Format(_T("%.01f"),xx);
					if((m_pData->m_nFastAxis == 5) && (theApp.m_LastSettings.nRAxisGraticuleReverse == 1) && (theApp.m_LastSettings.nRAxisGraticuleMode==0)) {
						Buff.Format(_T("%.01f"),(float)(360-xx));
					};
				} else {
					Buff.Format(_T("%.01f"),xx+xStart);
				}
				size=pDC->GetTextExtent(Buff);
				if(ii==0) {
					pDC->TextOut(pnx,m_rrGraticule.bottom+3,Buff);
				} else {
					pDC->TextOut(pnx-size.cx/2,m_rrGraticule.bottom+3,Buff);
				}
			};
		}
		for(xx=0,ii=0;xx<fXSize;xx+=(fStep/fDivisor),ii++) {
			pnx=MulDiv((int)xx,m_rrGraticule.Width(),(int)fXSize)+m_rrGraticule.left;
			if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_BOTTOM) {
				pDC->MoveTo(pnx,m_rrGraticule.bottom);
				pDC->LineTo(pnx,m_rrGraticule.bottom+2);
			}
		};
	} else {
		if(m_pData->m_vLineSample) {
			/*
			for(nSample = m_pData->m_vLineSample[m_nLine].m_nFirstSample;nSample < m_pData->m_nSamplesLine;nSample+=60) {
				pt.x = nSample;
				pt.y = m_nLine;
				if(m_pData->GetCoordFromLineSample(pt,&Cp) == true) {
					Buff.Format(L"%.01f",Cp.Side0.fX);
					pnx = MulDiv(nSample,m_rrGraticule.Width()-1,m_pData->m_nSamplesLine-1) +m_rrGraticule.left;
					pDC->MoveTo(pnx,m_rrGraticule.bottom);
					pDC->LineTo(pnx,m_rrGraticule.bottom+4);
					size = pDC->GetTextExtent(Buff);
					pDC->TextOut(pnx - size.cx / 2,m_rrGraticule.bottom+3,Buff);
				}
			}
			*/
		}
	}


	//******** Vertical graticule
	int nMaxRows = m_pData->m_nLogRange / 10;
	if(nMaxRows<=0) nMaxRows=1;
	int oldpnY = 0;

	switch(	theApp.m_LastSettings.nThicknessDisplayOptions & (LINEAR_DB | LOG_DB | PERCENTAGE | THICKNESS)) {
	case LINEAR_DB:
		for(fdB = 0.0f; fdB<40.0f; fdB+=1.0f) {
			fPercentage = powf(10.0f,fdB / -20.0f);
			pny=m_rrGraticule.bottom-(int)((float)m_rrGraticule.Height() * fPercentage);
			Buff.Format(_T("%.0fdB"),-fdB);
			size = pDC->GetTextExtent(Buff);
			if(oldpnY + size.cy < pny) {
				oldpnY = pny;
				pDC->SelectObject(&penWhite);
				pDC->MoveTo(m_rrGraticule.left,pny);
				pDC->LineTo(m_rrGraticule.left-3,pny);
				pDC->TextOut(m_rrGraticule.left-3-size.cx,pny-size.cy/2,Buff);
				if(theApp.m_LastSettings.nThicknessDisplayOptions & HORIZ_GRAT) {
					pDC->SelectObject(&penHorizontal);
					pDC->MoveTo(m_rrGraticule.left+1,pny);
					pDC->LineTo(m_rrGraticule.right,pny);
				}
			}
		}
		break;
	case LOG_DB:
		for(nRow=0;nRow<=nMaxRows;nRow++) {
			pny=m_rrGraticule.top+MulDiv(nRow*10,m_rrGraticule.Height(),m_pData->m_nLogRange);
			pDC->SelectObject(&penWhite);
			pDC->MoveTo(m_rrGraticule.left,pny);
			pDC->LineTo(m_rrGraticule.left-3,pny);
			Buff.Format(_T("%ddB"),(nRow)*10);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(m_rrGraticule.left-3-size.cx,pny-size.cy/2,Buff);

			if(theApp.m_LastSettings.nThicknessDisplayOptions & HORIZ_GRAT) {
				pDC->SelectObject(&penHorizontal);
				pDC->MoveTo(m_rrGraticule.left+1,pny);
				pDC->LineTo(m_rrGraticule.right,pny);
			}
		};
		break;
	case PERCENTAGE:
		for(nRow=0;nRow<=100;nRow+=10) {
			pny=m_rrGraticule.top+MulDiv(nRow,m_rrGraticule.Height(),100);
			pDC->SelectObject(&penWhite);
			pDC->MoveTo(m_rrGraticule.left,pny);
			pDC->LineTo(m_rrGraticule.left-3,pny);
			Buff.Format(_T("%d%%"),100-nRow);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(m_rrGraticule.left-3-size.cx,pny-size.cy/2,Buff);

			if(theApp.m_LastSettings.nThicknessDisplayOptions & HORIZ_GRAT && nRow != 100) {
				pDC->SelectObject(&penHorizontal);
				pDC->MoveTo(m_rrGraticule.left+1,pny);
				pDC->LineTo(m_rrGraticule.right,pny);
			}
		};
		break;
	case THICKNESS:
		if(theApp.m_Palette.m_dStepSize[0]<=0.0) theApp.m_Palette.m_dStepSize[0] = m_dMaxThickness / 10.0;
		for(dStep=0;dStep<=m_dMaxThickness;dStep+=theApp.m_Palette.m_dStepSize[0]) {
			pny=m_rrGraticule.bottom-(int)((dStep*(double)m_rrGraticule.Height())/m_dMaxThickness);
			pDC->SelectObject(&penWhite);
			pDC->MoveTo(m_rrGraticule.left,pny);
			pDC->LineTo(m_rrGraticule.left-3,pny);
			Buff.Format(_T("%.02f"),(dStep+theApp.m_Palette.m_dMinThickness[0]) * theApp.m_fUnits);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(m_rrGraticule.left-3-size.cx,pny-size.cy/2,Buff);

			if((theApp.m_LastSettings.nThicknessDisplayOptions & HORIZ_GRAT) && (dStep)) {
				pDC->SelectObject(&penHorizontal);
				pDC->MoveTo(m_rrGraticule.left+1,pny);
				pDC->LineTo(m_rrGraticule.right,pny);
			}
		};
		break;
	}


	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
}

void CViewThicknessCrossSectionDlg::DrawBarGraph(CDC *pDC)
{
	
	if(!(theApp.m_LastSettings.nThicknessDisplayOptions & BAR_GRAPH)) return;

	CRect	bar;
	CBrush* pOldBrush;
	CBrush* pBrush;
	int		xx,yy,nColor;
	CString Buff;
	int	nRange=(int)(m_dMaxThickness * 100.0);
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;
	if(nBytesPixel<0) nBytesPixel=1;

	bar.top = m_rrGraticule.top;
	bar.bottom = m_rrGraticule.bottom;
	bar.left = m_rrGraticule.right + 4;
	bar.right = m_rrGraticule.right + 12;

	pBrush = new CBrush;
	pBrush->CreateSolidBrush(RGB(255,255,255));
	pOldBrush=pDC->SelectObject(pBrush);

	CDC  dcMem;
	CBitmap bmp;
	char	*pArray,*pBuffer;
	pArray = new char[(bar.Width()+2) * (bar.Height()+2) * 4];

	switch(m_nImageType) {
	case AMP_IMAGE:
		for(yy=0;yy<(bar.Height()-1);yy++) {
			pBuffer=&pArray[((bar.Height()-1-yy)*bar.Width()) * nBytesPixel];

			nColor=MulDiv(yy,127,(bar.Height()-1))+128;
			for(xx=0;xx<bar.Width();xx++) {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>0)&0xff);
				if(nBytesPixel>3) pBuffer++;
			};
		};
		break;
	case TIME_IMAGE:
		for(yy=0;yy<bar.Height();yy++) {
			pBuffer=&pArray[((bar.Height()-1-yy)*bar.Width()) * nBytesPixel];

			nColor=MulDiv(yy,nRange,bar.Height())+theApp.m_Palette.m_nMinThicknessNumber[0];
			if(nColor>theApp.m_Palette.m_nMaxThicknessNumber[0]) nColor=theApp.m_Palette.m_nMaxThicknessNumber[0];
			for(xx=0;xx<bar.Width();xx++) {
				*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>0)&0xff);
				if(nBytesPixel>3) pBuffer++;
			};
		};
		break;
	}
	bmp.CreateCompatibleBitmap(pDC,bar.Width(),bar.Height());
	bmp.SetBitmapBits((DWORD)(bar.Width()*bar.Height()*nBytesPixel),pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	pDC->BitBlt(bar.left,bar.top,bar.Width(),bar.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	delete pArray;

	pDC->SelectObject(pOldBrush);
	delete pBrush;


}

void CViewThicknessCrossSectionDlg::GetGraticuleSize(float *fXSize, float *fYSize,float *fStep,int *nTextStep,float fUnits)
{
	
	float fTemp,fFastScanSize,fFastIncrement;

	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);

	*fXSize = fFastIncrement * (float)m_prrSamples->Width() * fUnits;
	if((m_pData->m_nFastAxis == 5) && (theApp.m_LastSettings.nRAxisGraticuleMode == 0)) {
		*fXSize = 360.0f;
	}
	*fYSize = m_pData->m_fSlowIncrement *  (float)m_prrSamples->Height() * fUnits;

	*fXSize<=*fYSize ? fTemp = *fXSize : fTemp = *fYSize;

	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0:
		*fStep = 250.0;
		*nTextStep=250;
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

void CViewThicknessCrossSectionDlg::DrawCrossSection(CDC *pDC, int nType)
{
	
	int nPnX,nPnY;
	CPen penAmp(PS_SOLID, 1, theApp.m_LastSettings.rgbCrossSection[RGB_AMP_TRACE]);
	CPen penThick(PS_SOLID, 1, theApp.m_LastSettings.rgbCrossSection[RGB_TOF_TRACE]);
	CPen *pOldPen = pDC->SelectObject(&penAmp);
	CPoint *pt = new CPoint[m_rrGraticule.Width()];
	int nCount = 0;
	int nAmp;
	float fThickness;


	for (nPnX = 0; nPnX < m_rrGraticule.Width(); nPnX++) {
		int nSample = MulDiv(nPnX, m_ptLine[1].x-m_ptLine[0].x, m_rrGraticule.Width() - 1) + m_ptLine[0].x;
		int nLine = MulDiv(nPnX, m_ptLine[1].y-m_ptLine[0].y, m_rrGraticule.Width() - 1) + m_ptLine[0].y;

		switch(nType) {
		case 0:
			nAmp = m_pData->GetFilteredAmp(nSample,nLine,m_nImageNumber,m_nImageType);
			nPnY = m_rrGraticule.bottom - (int)((double)(abs(nAmp - 128)*(double)m_rrGraticule.Height())/127);
			nPnY= MinMax(&nPnY,m_rrGraticule.top,m_rrGraticule.bottom);
			break;
		case 1:
			fThickness = m_pData->GetFilteredTimeThickness(nSample,nLine,m_nImageNumber, 1) / 100.0f;
			nPnY = m_rrGraticule.bottom - (int)((double)(fThickness*(double)m_rrGraticule.Height())/m_dMaxThickness);
			nPnY= MinMax(&nPnY,m_rrGraticule.top,m_rrGraticule.bottom);
			break;
		}
			
		pt[nCount++] = CPoint(nPnX + m_rrGraticule.left, nPnY);
	}
	switch(nType) {
	case 0: pOldPen = pDC->SelectObject(&penAmp);
		break;
	case 1: pOldPen = pDC->SelectObject(&penThick);
		break;
	}
	pDC->Polyline(pt, nCount);
	pDC->SelectObject(pOldPen);
	delete pt;

}

void CViewThicknessCrossSectionDlg::DrawOverlay(CDC *pDC)
{
	
	int	pnx,pny;
	float fHeader[11];
	CString Buff,Temp;
	CSize size;
	CFont HorizontalFont;

	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);

	CFont *pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,0));

	if(m_pImportCrossSectionOverlayDlg != NULL) {

		float *pfOverlay = new float[m_pData->m_nSamplesLine];
		ZeroMemory(pfOverlay,m_pData->m_nSamplesLine * sizeof (float));

		if(m_pImportCrossSectionOverlayDlg->GetLine(m_pData->m_nSamplesLine,pfOverlay) == true) {

			CPen pen(PS_SOLID, 1, RGB(0,255,0));
			CPen *pOldPen = pDC->SelectObject(&pen);

			for(int nSample = 0;nSample<m_pData->m_nSamplesLine;nSample++) {

				pnx = m_rrGraticule.left + MulDiv(nSample,m_rrGraticule.Width(),m_pData->m_nSamplesLine-1);

				pny = m_rrGraticule.bottom - (int)((double)(pfOverlay[nSample]*(double)m_rrGraticule.Height())/m_dMaxThickness);
				pny= MinMax(&pny,m_rrGraticule.top,m_rrGraticule.bottom);

				if(nSample == 0) {
					pDC->MoveTo(pnx,pny);
				} else {
					pDC->LineTo(pnx,pny);
				}
			}
			pDC->SelectObject(pOldPen);
		}
		delete pfOverlay;

		if(m_pImportCrossSectionOverlayDlg->GetHeader(fHeader) == true) {
			Buff.Format(L"%.0f %.02f %.02f %.02f %.02f %.02f %.02f %.02f",fHeader[2],fHeader[3],fHeader[4],fHeader[5],fHeader[6],fHeader[7],fHeader[8],fHeader[9]);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(m_rrView.Width() - size.cx -2, 2, Buff);
		}
	}
	pDC->SelectObject(pOldFont);
}



void CViewThicknessCrossSectionDlg::DrawSampleCursor(CDC *pDC)
{
	
	CFont HorizontalFont;
	CPen penYellow(PS_SOLID,1,RGB(255,255,0));
	CPen *pOldPen = pDC->SelectObject(&penYellow);
	CString Buff;
	CSize size;
	CPoint pt[2];
	CCoord Cp[2];

	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);

	CFont *pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,0));

/*
	for(int nCursor=0;nCursor<m_nCursorL;nCursor++) {

		if(m_nSample[nCursor]<0) m_nSample[nCursor] = 0;
		if(m_nSample[nCursor]>=m_pData->m_nSamplesLine) m_nSample[nCursor] = m_pData->m_nSamplesLine - 1;


		pnx = m_rrGraticule.left + MulDiv(m_nSample[nCursor],m_rrGraticule.Width(),m_pData->m_nSamplesLine-1);

		switch(	theApp.m_LastSettings.nThicknessDisplayOptions & (LINEAR_DB | LOG_DB | PERCENTAGE | THICKNESS)) {
		case LINEAR_DB:
		case LOG_DB:
		case PERCENTAGE:
			pny = m_rrGraticule.bottom - (int)((double)(m_pnAmplitude[m_nSample[nCursor]]*(double)m_rrGraticule.Height())/m_nMaxAmplitude);
			break;
		case THICKNESS:
			pny = m_rrGraticule.bottom - (int)((double)(m_pfThickness[m_nSample[nCursor]]*(double)m_rrGraticule.Height())/m_dMaxThickness);
			break;
		}
		switch(	theApp.m_LastSettings.nThicknessDisplayOptions & (LINEAR_DB | LOG_DB | PERCENTAGE | THICKNESS)) {
		case LINEAR_DB:		Buff.Format(L"%.01fdB",20.0 * log10((float)m_pnAmplitude[m_nSample[nCursor]] * 0.01));
			break;
		case LOG_DB:		Buff.Format(L"%ddB",m_pData->m_nLogRange - m_pnAmplitude[m_nSample[nCursor]]);
			break;
		case PERCENTAGE:	Buff.Format(L"%d%%",m_pnAmplitude[m_nSample[nCursor]]);
			break;
		case THICKNESS:		Buff.Format(L"%.02fmm",m_pfThickness[m_nSample[nCursor]]);
			break;
		}

		pDC->MoveTo(pnx,m_rrGraticule.bottom-1);
		pDC->LineTo(pnx,pny);

		size = pDC->GetTextExtent(Buff);

		pDC->TextOut(pnx-size.cx/2,m_rrGraticule.bottom + (size.cy * (nCursor+1)),Buff);

		pt[nCursor].x = m_nSample[nCursor];
		pt[nCursor].y = m_nLine;
		m_pData->GetCoordFromLineSample(pt[nCursor],&Cp[nCursor]);
	}

	if(m_nCursorL>1) {
		fLength = CCoord::Length(Cp[0],Cp[1],0);
		Buff.Format(L"Cursor separation in XYZ coordinates = %.01fmm",fLength);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGraticule.CenterPoint().x-size.cx/2,2,Buff);
	}
	*/
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}


void CViewThicknessCrossSectionDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int	nSS = 0;
	int	nSSDelta = 100000;
	int nPtr = 0;
/*
	point.x -= m_rrView.left;
	point.y -= m_rrView.top;
	if(m_rrGraticule.PtInRect(point)) {
		m_bLbDown = true;

		nSS = MulDiv(point.x-m_rrGraticule.left,m_pData->m_nSamplesLine-1,m_rrGraticule.Width());
		for(int ii=0; ii<m_nCursorL;ii++) {
			if(abs(m_nSample[ii] - nSS) < nSSDelta) {
				nSSDelta = abs(m_nSample[ii] - nSS);
				nPtr = ii;
			}
		}
		m_nSample[m_nCursorPtr = nPtr] = nSS;
		Invalidate(false);
	}
*/	
	CDialog::OnLButtonDown(nFlags, point);
}

void CViewThicknessCrossSectionDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLbDown = false;
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CViewThicknessCrossSectionDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	point.x -= m_rrView.left;
	point.y -= m_rrView.top;
/*
	if(m_rrGraticule.PtInRect(point) && (m_bLbDown==true)) {

		m_nSample[m_nCursorPtr] = MulDiv(point.x-m_rrGraticule.left,m_pData->m_nSamplesLine-1,m_rrGraticule.Width());
		RenderView();
	}
*/	
	CDialog::OnMouseMove(nFlags, point);
}


void CViewThicknessCrossSectionDlg::SelectCurrentSampleLine(int nS0, int nL0, int nS1, int nL1)
{

	if(nS0<=nS1) {
		m_ptLine[0] = CPoint(nS0,nL0);
		m_ptLine[1] = CPoint(nS1,nL1);
	} else {
		m_ptLine[1] = CPoint(nS0,nL0);
		m_ptLine[0] = CPoint(nS1,nL1);
	}

}

void CViewThicknessCrossSectionDlg::OnButtonDoubleCursor() 
{
	m_nCursorL = 2;
	SetToolBarCheckedState();
	Invalidate(false);
}

void CViewThicknessCrossSectionDlg::OnButtonSingleCursor() 
{
	m_nCursorL = 1;
	SetToolBarCheckedState();
	Invalidate(false);
}

void CViewThicknessCrossSectionDlg::SetToolBarCheckedState()
{
	
	int	nIndex,ii,nMask=1;

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_SINGLE_CURSOR))>=0)
		m_nCursorL==1 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_DOUBLE_CURSOR))>=0)
		m_nCursorL==2 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_HORIZONTAL_GRATICULE))>=0)
		theApp.m_LastSettings.nThicknessDisplayOptions & HORIZ_GRAT ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_BARGRAPH))>=0)
		theApp.m_LastSettings.nThicknessDisplayOptions & BAR_GRAPH ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_RELATIVE_ABSOLUTE))>=0)
		theApp.m_LastSettings.nThicknessDisplayOptions & RELATIVE_ABSOLUTE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

	for(ii=0;ii<m_pData->m_nNumberImages;ii++,nMask<<=1) {
		if((nIndex=m_wndGateToolBar[ii].CommandToIndex(ID_BUTTON_AMP))>=0) {
			theApp.m_LastSettings.nCrossSectionMask & nMask ? m_wndGateToolBar[ii].SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndGateToolBar[ii].SetButtonStyle(nIndex,0);
		}
		if((nIndex=m_wndGateToolBar[ii].CommandToIndex(ID_BUTTON_TOF))>=0) {
			theApp.m_LastSettings.nCrossSectionMask & nMask ? m_wndGateToolBar[ii].SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndGateToolBar[ii].SetButtonStyle(nIndex,0);
		}
	}

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_LINEAR_DB))>=0)
		theApp.m_LastSettings.nThicknessDisplayOptions & LINEAR_DB ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_LOG))>=0)
		theApp.m_LastSettings.nThicknessDisplayOptions & LOG_DB ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_PERCENTAGE))>=0)
		theApp.m_LastSettings.nThicknessDisplayOptions & PERCENTAGE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_THICKNESS))>=0)
		theApp.m_LastSettings.nThicknessDisplayOptions & THICKNESS ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);


}

BOOL CViewThicknessCrossSectionDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
   case ID_BUTTON_SINGLE_CURSOR: strTipText.LoadString(IDS_Single_cursor);
	   break;
   case ID_BUTTON_DOUBLE_CURSOR: strTipText.LoadString(IDS_Double_cursor);
	   break;
   case ID_BUTTON_HORIZONTAL_GRATICULE: strTipText.LoadString(IDS_Graticule);
	   break;
   case ID_BUTTON_BARGRAPH: strTipText.LoadString(IDS_Bar_Graph);
	   break;
   case ID_BUTTON_OVERLAY: strTipText.LoadString(IDS_Overlay);
	   break;
   case ID_BUTTON_RELATIVE_ABSOLUTE: strTipText.LoadString(IDS_Relative_or_absolute_graticule);
	   break;
   case ID_BUTTON_LINEAR_DB: strTipText.LoadString(IDS_Linear_dB_graticule);
	   break;
   case ID_BUTTON_LOG: strTipText.LoadString(IDS_Log_dB_graticule);
	   break;
   case ID_BUTTON_PERCENTAGE: strTipText.LoadString(IDS_Percentage_FSH_graticule);
	   break;
   case ID_BUTTON_THICKNESS: strTipText.LoadString(IDS_Thickness_graticule);
	   break;

   }
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


void CViewThicknessCrossSectionDlg::OnButtonHorizontalGraticule() 
{
	
	
	theApp.m_LastSettings.nThicknessDisplayOptions & HORIZ_GRAT ? theApp.m_LastSettings.nThicknessDisplayOptions &= ~HORIZ_GRAT : theApp.m_LastSettings.nThicknessDisplayOptions |= HORIZ_GRAT;
	SetToolBarCheckedState();
	Invalidate(false);
}

void CViewThicknessCrossSectionDlg::OnButtonBargraph() 
{
	
	
	theApp.m_LastSettings.nThicknessDisplayOptions & BAR_GRAPH ? theApp.m_LastSettings.nThicknessDisplayOptions &= ~BAR_GRAPH : theApp.m_LastSettings.nThicknessDisplayOptions |= BAR_GRAPH;
	SetToolBarCheckedState();
	Invalidate(false);
}

void CViewThicknessCrossSectionDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SLIDER_THICKNESS:
		m_dMaxThickness = ((theApp.m_Palette.m_dThicknessRange[0] * 20.0) - (double)(pSlider->GetPos()&0xffff)) /10.0;
		Invalidate(FALSE);
		break;
	};
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CViewThicknessCrossSectionDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	
	CMenu* menu = (CMenu *) new CMenu;
	CPoint pt = point;
	CRect rr;

	ScreenToClient(&point);
//	m_ptClient = point;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	if(rr.PtInRect(point)) {
		if (menu->LoadMenu(IDR_RB_CROSSSECTION_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
		};
	}

	delete menu;
}

void CViewThicknessCrossSectionDlg::OnCrosssectionColoursTrace() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_AMP_TRACE]=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CViewThicknessCrossSectionDlg::OnCrosssectionColoursToftrace() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_TOF_TRACE]=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CViewThicknessCrossSectionDlg::OnCrosssectionColoursBackground() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_BKGROUND]=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CViewThicknessCrossSectionDlg::OnCrosssectionColoursMaximum() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_MAXIMUM]=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CViewThicknessCrossSectionDlg::OnCrosssectionColoursNominal() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_NOMINAL]=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CViewThicknessCrossSectionDlg::OnCrosssectionColoursMinimum() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_MINIMUM]=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CViewThicknessCrossSectionDlg::OnCrosssectionColoursGraticule() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_GRATICULE]=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CViewThicknessCrossSectionDlg::OnButtonAmp() 
{

	for(int ii=0;ii<m_pData->m_nNumberImages;ii++) {

	}
}

void CViewThicknessCrossSectionDlg::OnButtonTof() 
{
	
}

BOOL CViewThicknessCrossSectionDlg::PreTranslateMessage(MSG* pMsg) 
{
	
	HANDLE hWnd;
	int ii,nState;
	int nMask = 1;

	switch(pMsg->message) {
	case 0x202:
		hWnd = ::GetFocus();
		for(ii=0;ii<16;ii++,nMask<<=1) {
			if(m_wndGateToolBar[ii].m_hWnd) {
				nState = m_wndGateToolBar[ii].GetToolBarCtrl().GetState(ID_BUTTON_AMP);
				switch(nState) {
				case 6:	theApp.m_LastSettings.nCrossSectionMask |= nMask;
					SetToolBarCheckedState();
					Invalidate(false);
					break;
				case 7:	theApp.m_LastSettings.nCrossSectionMask &= ~nMask;
					SetToolBarCheckedState();
					Invalidate(false);
					break;
				};
				nState = m_wndGateToolBar[ii].GetToolBarCtrl().GetState(ID_BUTTON_TOF);
				switch(nState) {
				case 6:	theApp.m_LastSettings.nCrossSectionMask |= nMask;
					SetToolBarCheckedState();
					Invalidate(false);
					break;
				case 7:	theApp.m_LastSettings.nCrossSectionMask &= ~nMask;
					SetToolBarCheckedState();
					Invalidate(false);
					break;
				};
			}
		}
		break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CViewThicknessCrossSectionDlg::OnButtonOverlay() 
{
		
	if (m_pImportCrossSectionOverlayDlg == NULL) {
		m_pImportCrossSectionOverlayDlg = new CImportCrossSectionOverlayDlg(this,(CWnd**)&m_pImportCrossSectionOverlayDlg);
		m_pImportCrossSectionOverlayDlg->Create(CImportCrossSectionOverlayDlg::IDD,this);
	} else {
		m_pImportCrossSectionOverlayDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}


void CViewThicknessCrossSectionDlg::OnButtonRelativeAbsolute() 
{
	

	theApp.m_LastSettings.nThicknessDisplayOptions & RELATIVE_ABSOLUTE ? theApp.m_LastSettings.nThicknessDisplayOptions &= ~RELATIVE_ABSOLUTE : theApp.m_LastSettings.nThicknessDisplayOptions |= RELATIVE_ABSOLUTE;
	SetToolBarCheckedState();
	Invalidate(false);

}

void CViewThicknessCrossSectionDlg::OnButtonLinearDb() 
{
	

	theApp.m_LastSettings.nThicknessDisplayOptions &= ~(LINEAR_DB | LOG_DB | PERCENTAGE | THICKNESS);
	theApp.m_LastSettings.nThicknessDisplayOptions |= LINEAR_DB;
	SetToolBarCheckedState();
	Invalidate(false);
	
}

void CViewThicknessCrossSectionDlg::OnButtonLog() 
{
	

	theApp.m_LastSettings.nThicknessDisplayOptions &= ~(LINEAR_DB | LOG_DB | PERCENTAGE | THICKNESS);
	theApp.m_LastSettings.nThicknessDisplayOptions |= LOG_DB;
	SetToolBarCheckedState();
	Invalidate(false);
}

void CViewThicknessCrossSectionDlg::OnButtonPercentage() 
{
	

	theApp.m_LastSettings.nThicknessDisplayOptions &= ~(LINEAR_DB | LOG_DB | PERCENTAGE | THICKNESS);
	theApp.m_LastSettings.nThicknessDisplayOptions |= PERCENTAGE;
	SetToolBarCheckedState();
	Invalidate(false);
}

void CViewThicknessCrossSectionDlg::OnButtonThickness() 
{
	

	theApp.m_LastSettings.nThicknessDisplayOptions &= ~(LINEAR_DB | LOG_DB | PERCENTAGE | THICKNESS);
	theApp.m_LastSettings.nThicknessDisplayOptions |= THICKNESS;
	SetToolBarCheckedState();
	Invalidate(false);

}