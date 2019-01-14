// PrintConesCylindersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PrintConesCylindersDlg.h"
#include "USLPrint.h"
#include "ProgressDlg.h"
#include "ViewSheet.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintConesCylindersDlg dialog


CPrintConesCylindersDlg::CPrintConesCylindersDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nImageType,int nImageNumber,int nPaletteNumber,CData *pData )
	: CDialog(CPrintConesCylindersDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrintConesCylindersDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CPolyCoord *pLine;
	CCoord Cp[2];
	float fX,fY;

	m_pParent = pParent;
	
	m_pDlg = pDlg;
	m_pData = pData;
	m_pPrintOptionsDlg = NULL;
	m_nImageType = nImageType;
	m_nImageNumber = nImageNumber;
	m_nPaletteNumber = nPaletteNumber;
	m_fHoriMag = 1.0f;
	m_fVertMag = 1.0f;
	m_bFullScalePrintOut = false;
	m_bHalfScalePrintOut = false;
	m_hcursorFinger = AfxGetApp()->LoadCursor(IDC_FINGER_CURSOR);
	m_pArrayView = NULL;
	m_fCrossHairAngle=0.0f;
	
	m_nMouseOption = 0;
	m_bDragSector = false;
	m_bLeftAngle = false;
	m_bRightAngle = false;
	m_bDragEnabled = false;
	m_bLbDown = false;
	m_bRenderGraticule = true;
	m_bRenderCrossHair = false;

	Cone.fR[CONE_TOP] = 750.0f;
	Cone.fR[CONE_BOT] = 1000.0f;
	Cone.fScanHeight = 1000.0;

	Donut.nSrcPtrArray = NULL;
	Donut.cArcAmpBuffer = NULL;
	Donut.fSector[0] = 0.0f;
	Donut.fSector[1] = 360.0f;

	m_fGratStartAngle = 0.0f;


	pLine = PROFILE->m_ScanLine;
	if(pLine != NULL) {
		pLine->GetCoord(0,&Cp[0]);
		pLine->GetLastCoord(&Cp[1]);
	} else {
		Cp[0] = m_pData->m_CpStart;
		Cp[1] = m_pData->m_CpFinish;
	}
	switch(m_pData->m_nSlowAxis) {
	case 0:
		break;
	case 1:
		break;
	case 2:
		if(Cp[0].Side0.fZ < Cp[1].Side0.fZ) {
			fX = Cp[0].Side0.fX - m_pData->m_CpTurnTableCentre.Side0.fX;
			fY = Cp[0].Side0.fY - m_pData->m_CpTurnTableCentre.Side0.fY;
			Cone.fR[CONE_BOT] = sqrtf(fX*fX + fY*fY);

			fX = Cp[1].Side0.fX - m_pData->m_CpTurnTableCentre.Side0.fX;
			fY = Cp[1].Side0.fY - m_pData->m_CpTurnTableCentre.Side0.fY;
			Cone.fR[CONE_TOP] = sqrtf((fX*fX) + (fY*fY));
			Cone.fScanHeight = Cp[1].Side0.fZ - Cp[0].Side0.fZ;
		} else {
			fX = Cp[0].Side0.fX - m_pData->m_CpTurnTableCentre.Side0.fX;
			fY = Cp[0].Side0.fY - m_pData->m_CpTurnTableCentre.Side0.fY;
			Cone.fR[CONE_TOP] = sqrtf((fX*fX) + (fY*fY));

			fX = Cp[1].Side0.fX - m_pData->m_CpTurnTableCentre.Side0.fX;
			fY = Cp[1].Side0.fY - m_pData->m_CpTurnTableCentre.Side0.fY;
			Cone.fR[CONE_BOT] = sqrtf((fX*fX) + (fY*fY));
			Cone.fScanHeight = Cp[0].Side0.fZ - Cp[1].Side0.fZ;
		}
		break;
	};

	CalculateConeGeometry();
}


void CPrintConesCylindersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintConesCylindersDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintConesCylindersDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintConesCylindersDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_CONE_ZOOM_IN, OnButtonConeZoomIn)
	ON_COMMAND(ID_BUTTON_CONE_ZOOM_OUT, OnButtonConeZoomOut)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_BUTTON_PRINT_OPTIONS, OnButtonPrintOptions)
	ON_COMMAND(ID_BUTTON_PRINT_CONE, OnButtonPrintCone)
	ON_COMMAND(ID_BUTTON_CROSS_HAIR, OnButtonCrossHair)
	ON_COMMAND(ID_BUTTON_PAPER_WIDTH, OnButtonPaperWidth)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_BUTTON_CONE_1_1, OnButtonCone11)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CONE_GRATICULEZEROPOS, OnConeGraticulezeropos)
	ON_COMMAND(ID_BUTTON_CONE_GRATICULE, OnButtonConeGraticule)
	ON_COMMAND(ID_CONE_PALETTENO_1, OnConePaletteno1)
	ON_COMMAND(ID_CONE_PALETTENO_2, OnConePaletteno2)
	ON_COMMAND(ID_CONE_PALETTENO_3, OnConePaletteno3)
	ON_COMMAND(ID_CONE_PALETTENO_4, OnConePaletteno4)
	ON_COMMAND(ID_CONE_PALETTENO_5, OnConePaletteno5)
	ON_COMMAND(ID_CONE_PALETTENO_6, OnConePaletteno6)
	ON_COMMAND(ID_CONE_PALETTENO_7, OnConePaletteno7)
	ON_COMMAND(ID_BUTTON_HALF_PAPER_WIDTH, OnButtonHalfPaperWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintConesCylindersDlg message handlers
int CPrintConesCylindersDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_PRINT_CONES_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}
	m_wndDialogBar.Create(this, IDD_VIEW_DIALOGBAR, CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_VIEW_DIALOGBAR);

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	m_wndRebar.AddBar(&m_wndDialogBar);
	
	return 0;
}

void CPrintConesCylindersDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CPrintConesCylindersDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[this->IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CPrintConesCylindersDlg::OnDestroy() 
{
	if (this->IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[this->IDD]);

	CDialog::OnDestroy();
	SAFE_DELETE(Donut.cArcAmpBuffer);
	SAFE_DELETE(Donut.nSrcPtrArray);
	SAFE_DELETE(m_pArrayView);
	*m_pDlg	= NULL;	
	
}

BOOL CPrintConesCylindersDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;
	

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	ShowScrollBar(SB_BOTH,TRUE);

	if (this->IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[this->IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	m_StaticPosView.SetNewPos();
	

	SetToolBarCheckedState();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrintConesCylindersDlg::SetZoomSamples()
{
	m_ZoomSamples.left = 0;
	m_ZoomSamples.right = Donut.nArcBufferWidth;

	if(m_fVertMag == 1.0f) {
		m_ZoomSamples.top = 0;
		m_ZoomSamples.bottom = Donut.nArcBufferHeight;
	} else {
		m_ZoomSamples.bottom = m_ZoomSamples.top + (int)((float)Donut.nArcBufferHeight * m_fVertMag);
		if(m_ZoomSamples.bottom >= Donut.nArcBufferHeight) {
			m_ZoomSamples.bottom = Donut.nArcBufferHeight;
			m_ZoomSamples.top = Donut.nArcBufferHeight - (int)(Donut.nArcBufferHeight * m_fVertMag);
		}

	}

	if(m_fHoriMag == 1.0f) {
		Donut.fSector[0]=0;
		Donut.fSector[1]=360;
	} else {
		Donut.fSector[1] = (360.0f / m_fHoriMag) + Donut.fSector[0];
		if(Donut.fSector[1] > 360.0f) {
			Donut.fSector[1] = 360.0f;
			Donut.fSector[0] = 360.0f - (360.0f / m_fHoriMag);
		}
	}
	Donut.nSample[0] = (int)((float)m_pData->m_nSamplesLine * Donut.fSector[0] / 360.0f);

}


void CPrintConesCylindersDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosView.SetNewPos();
	if(GetSafeHwnd()) {
		Invalidate(false);
	}
	
}

void CPrintConesCylindersDlg::DisplayStats(CPaintDC *pDC,CRect rr)
{
	CString Buff;
	CSize size;
	CString Degrees = L"\x0b0";


	Buff.Format(L"Top Radius:%.01f Circumference:%.01f Circle R:%.01f",Cone.fR[CONE_TOP],Cone.fC[CONE_TOP],Donut.fR[CONE_INNER]);
	pDC->TextOut(rr.left,rr.top,Buff);
	size = pDC->GetTextExtent(Buff);
	Buff.Format(L"Bot Radius:%.01f Circumference:%.01f Circle R:%.01f",Cone.fR[CONE_BOT],Cone.fC[CONE_BOT],Donut.fR[CONE_OUTER]);
	pDC->TextOut(rr.left,rr.top+size.cy,Buff);
	Buff.Format(L"Cone angle:%.02f%s Scan Height:%.01f Cone Height:%.01f",Cone.fAngle * RAD_TO_DEG,Degrees,Cone.fScanHeight,Cone.fHeight);
	pDC->TextOut(rr.left,rr.top+size.cy*2,Buff);

	Buff.Format(L"Paper Width: %.01f  Height: %.01f",Donut.fPaperWidth,Donut.fPaperHeight);
	pDC->TextOut(rr.left,rr.top+size.cy*3,Buff);



}
/*
void CPrintConesCylindersDlg::RenderArcs(CPaintDC *pDC,CRect rr)
{
	CPoint ptCentre = rr.CenterPoint();
	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penGreen(PS_SOLID,1,RGB(0,255,0));
	CPen *pOldPen = pDC->SelectObject(&penGrey);
	CRect rrRing[2];
	CPoint pt[2];

	rr.DeflateRect(20,20);
	if(rr.Width() > rr.Height()) {
		rr.left = ptCentre.x - rr.Height()/2;
		rr.right = ptCentre.x + rr.Height()/2;
	} else {
		rr.top = ptCentre.y - rr.Width()/2;
		rr.bottom = ptCentre.y + rr.Width()/2;
	};


	pDC->MoveTo(ptCentre.x,rr.top);
	pDC->LineTo(ptCentre.x,rr.bottom);
	pDC->MoveTo(rr.left,ptCentre.y);
	pDC->LineTo(rr.right,ptCentre.y);

	rrRing[0] = rr;
	pDC->SelectObject(&penBlue);
	pDC->SelectStockObject(HOLLOW_BRUSH);
	pDC->Ellipse(rrRing[0]);

	float fRatio = Donut.fR[CONE_INNER] / Donut.fR[CONE_OUTER];
	int nRadius = (int)((float)rr.Width()/2 * fRatio);

	rrRing[1].left = ptCentre.x - nRadius;
	rrRing[1].right = ptCentre.x + nRadius;
	rrRing[1].top = ptCentre.y - nRadius;
	rrRing[1].bottom = ptCentre.y + nRadius;

	pDC->SelectObject(&penRed);
	pDC->Ellipse(rrRing[1]);

	pt[0].x = ptCentre.x - (int)(((float)rr.Height()/2.0f) * sinf(Donut.fArcAngle / 2.0f));
	pt[1].x = ptCentre.x + (int)(((float)rr.Height()/2.0f) * sinf(Donut.fArcAngle / 2.0f));
	pt[0].y = pt[1].y = ptCentre.y + (int)(((float)rr.Height()/2.0f) * cosf(Donut.fArcAngle / 2.0f));
	pDC->SelectObject(&penGreen);
	pDC->MoveTo(pt[0]);
	pDC->LineTo(ptCentre);
	pDC->LineTo(pt[1]);

	pDC->SelectObject(pOldPen);



}
*/

void CPrintConesCylindersDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;
	SCROLLINFO	info;
	CString Buff;
	CRgn rgn;
	CRect rect;
	CDC	dcMem;
	CBitmap		bmp;
	int nBitsPixel = dc.GetDeviceCaps(BITSPIXEL);
	int	nBytesPixel = nBitsPixel/8;

	m_staticView.GetWindowRect(rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2);

	SetZoomSamples();

	SetScrollRange(SB_VERT,0,Donut.nArcBufferHeight,FALSE);
	GetScrollInfo(SB_VERT,&info,SIF_ALL);
	info.nPage = m_ZoomSamples.Height();
	SetScrollInfo(SB_VERT,&info,TRUE);
	SetScrollPos(SB_VERT,m_ZoomSamples.top,FALSE);

	SetScrollRange(SB_HORZ,0,360,FALSE);
	GetScrollInfo(SB_HORZ,&info,SIF_ALL);
	info.nPage = (int)(Donut.fSector[1] - Donut.fSector[0]);
	SetScrollInfo(SB_HORZ,&info,TRUE);
	SetScrollPos(SB_HORZ,(int)Donut.fSector[0],FALSE);

	//This is the bitblt that you want to replicate for future uses

	RenderArcCScan(&dc,rr);

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*nBytesPixel),m_pArrayView);

	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);


	CRect rrArea = rr;
	rrArea.OffsetRect(-rr.left,-rr.top);
	RenderGraticule((CPaintDC*)&dcMem,rrArea);
	RenderCrossHair((CPaintDC*)&dcMem,rrArea);


	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);


}


void CPrintConesCylindersDlg::RenderArcCScan(CPaintDC* pDC,CRect rr)
{
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int	nBytesPixel = nBitsPixel/8;
	CBitmap bmp;

	SAFE_DELETE( m_pArrayView );
	m_pArrayView = new char[(rr.Width()+2) * (rr.Height()+2) * nBytesPixel];

	char	cColor[256][4];
	for(int ii=0;ii<256;ii++) {
		cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>16)&0xff);
		cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>8)&0xff);
		cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>0)&0xff);
	}

	RenderArcAmpBuffer(pDC,&rr,m_pArrayView,(char*)cColor,m_ZoomSamples);



}



void CPrintConesCylindersDlg::OnButtonConeZoomIn() 
{
	m_fHoriMag *= 2.0f;
	if(m_fHoriMag > 10.0f) m_fHoriMag = 10.0f;
	Donut.fSector[1] = (360.0f / m_fHoriMag) + Donut.fSector[0];
	CalculateConeGeometry();

	Invalidate(true);
	
}

void CPrintConesCylindersDlg::OnButtonConeZoomOut() 
{
	m_fHoriMag /= 2.0f;
	if(m_fHoriMag < 1.0f) m_fHoriMag = 1.0f;
	Donut.fSector[1] = (360.0f / m_fHoriMag) + Donut.fSector[0];
	CalculateConeGeometry();

	Invalidate(false);
}

BOOL CPrintConesCylindersDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	point;
	CRect rr;

	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CPrintConesCylindersDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(rr.PtInRect(point)) {
		m_bLbDown = true;
		m_ptDown = point;
		ClientToWorld(point,rr,&m_fCrossHairAngle,&m_fCrossHairDonutRadius);
		Invalidate(false);
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CPrintConesCylindersDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{

	m_bLbDown = false;
	CDialog::OnLButtonUp(nFlags, point);
}

void CPrintConesCylindersDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CString Buff;
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(rr.PtInRect(point) && (m_bLbDown == true)) {

		ClientToWorld(point,rr,&m_fCrossHairAngle,&m_fCrossHairDonutRadius);
		Invalidate(false);
	};

	CDialog::OnMouseMove(nFlags, point);
}

void CPrintConesCylindersDlg::OnButtonPrintOptions() 
{
	if(m_pPrintOptionsDlg == NULL) {
		m_pPrintOptionsDlg = new CPrintOptionsDlg(this,(CWnd**)&m_pPrintOptionsDlg, m_pData);
		m_pPrintOptionsDlg->Create(CPrintOptionsDlg::IDD,this);
	} else {
		m_pPrintOptionsDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}


void CPrintConesCylindersDlg::SetToolBarCheckedState() 
{
	int	nIndex;

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_CROSS_HAIR))>=0)
		m_bRenderCrossHair==true ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_PAPER_WIDTH))>=0)
		m_bFullScalePrintOut == true ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_HALF_PAPER_WIDTH))>=0)
		m_bHalfScalePrintOut==true ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_CONE_GRATICULE))>=0)
		m_bRenderGraticule==true ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
}


void CPrintConesCylindersDlg::OnButtonPrintCone() 
{
	PrintWusiWug(1.0f);
}


void CPrintConesCylindersDlg::PrintWusiWug(float fScale)
{
	CUSLPrint m_Print;
	int	nPrintRow=0;
	CRect Rect,rr,rrSampleSlice;
	CPaintDC *pDC = (CPaintDC *) &m_Print.m_dc;
	CString Buff;
	int cc;
	float fXSize,fYSize;
	CProgressDlg Progress;

	CWaitCursor Wait;
	m_Print.StartDoc(L"USLScanner Image");
	m_Print.StartPage();
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;
	if(nBytesPixel<0) nBytesPixel=1;

	int	nPrintingAreaWidthMM = pDC->GetDeviceCaps(HORZSIZE);	//908			//287mm
	int	nPrintingAreaHeightMM = pDC->GetDeviceCaps(VERTSIZE);	//1213			//405mm
	int nXRes = pDC->GetDeviceCaps(LOGPIXELSX);					//600			//300 pixels / inch
	int nYRes = pDC->GetDeviceCaps(LOGPIXELSY);					//600			//300 pixels / inch
	float fXResMM = (float)nXRes / 25.4f;
	float fYResMM = (float)nYRes / 25.4f;
	int nPrintingAreaWidth	= pDC->GetDeviceCaps(HORZRES);		//21460			//3387 Number of dots
	int nPrintingAreaHeight  = pDC->GetDeviceCaps(VERTRES);		//28660			//4786 Number of dots

	int	nPhysicalPageWidth   = pDC->GetDeviceCaps(PHYSICALWIDTH);	//21600			//3507
	int	nPhysicalPageHeight  = pDC->GetDeviceCaps(PHYSICALHEIGHT);	//28800			//4960

	int	nPrintingAreaOffsetLeft	= pDC->GetDeviceCaps(PHYSICALOFFSETX);	//70		//60
	int	nPrintingAreaOffsetTop	= pDC->GetDeviceCaps(PHYSICALOFFSETY);	//70		//36

	fXSize = Donut.fPaperWidth;
	fYSize = Donut.fPaperHeight * m_fVertMag;


	if(m_bHalfScalePrintOut == true) {
		fScale = 0.5f;
	}

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

//	unsigned char* pData = Donut.cArcAmpBuffer;
	unsigned char* pData = m_pData->m_cAmpBuffer[m_nImageNumber];


	char *pArrayView = new char[(Rect.Width()+2) * nLineSlices * nBytesPixel];
	CDC  dcMem;
	CBitmap bmp;
	int nSrcPtr,nDestPtr,nY,nX,nLength;
	int nDataSample,nDataLine,nPos,nPtr;
	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,Rect.Width(),nLineSlices);

	int nPage = 0;
	int nPageLineNumber = 0;

	CProgressDlg dlg(NULL);
	dlg.Create(CProgressDlg::IDD, NULL);
	dlg.SetProgressRange(0,100);
	dlg.SetProgressText(L"Printing USLScanner");

	char	cColor[256][4];
	for(int ii=0;ii<256;ii++) {
		cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>16)&0xff);
		cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>8)&0xff);
		cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nPaletteNumber][ii]>>0)&0xff);
	}

	int nSize = Donut.nArcBufferWidth * Donut.nArcBufferHeight;
	int nSrcSize = m_pData->m_nSamplesLine * m_pData->m_nNumberLines;

//?????????????
	for(int nPrinterLine=Rect.top;nPrinterLine<Rect.bottom;nPrinterLine+=nLineSlices,nPageLineNumber+=nLineSlices) {

		nPos = MulDiv(nPrinterLine,100,Rect.bottom);
		dlg.SetProgressPos(nPos);

		for(nY=0;nY<nLineSlices;nY++) {
			nDestPtr=nY * Rect.Width() * nBytesPixel;

			nDataSample = MulDiv(nY+nPrinterLine,m_ZoomSamples.Width(),Rect.Height()) + m_ZoomSamples.left;

			for(nX=0;nX<Rect.Width();nX++) {
				nDataLine = m_ZoomSamples.Height() - MulDiv(nX,m_ZoomSamples.Height(),Rect.Width()) + m_ZoomSamples.top;

				nSrcPtr = nDataLine * Donut.nArcBufferWidth + nDataSample;
				if(nSrcPtr<0) nSrcPtr=0;
				if(nSrcPtr>=nSize) nSrcPtr=nSize-1;

				if((nY+nPrinterLine) < Rect.bottom) {
//					cc = pData[nSrcPtr];
					nPtr = Donut.nSrcPtrArray[nSrcPtr] + Donut.nSample[0];
					if(nPtr<0) nPtr=0;
					if(nPtr>=nSrcSize) nPtr = nSrcSize-1;
//					if(Donut.nSrcPtrArray[nSrcPtr]!=0) {
						cc = pData[nPtr];
//					} else {
//						cc = 0;
//					}

					pArrayView[nDestPtr++]=cColor[cc][0];
					pArrayView[nDestPtr++]=cColor[cc][1];
					pArrayView[nDestPtr++]=cColor[cc][2];
				} else {
					pArrayView[nDestPtr++]=(char)255;
					pArrayView[nDestPtr++]=(char)255;
					pArrayView[nDestPtr++]=(char)255;
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

}


void CPrintConesCylindersDlg::CalculateConeGeometry()
{

	Cone.fAngle = atanf((Cone.fR[CONE_BOT] - Cone.fR[CONE_TOP]) / Cone.fScanHeight);
	Cone.fTriangleAngle = Cone.fAngle / 2.0f;
	Cone.fHeight = Cone.fR[CONE_BOT] / tanf(Cone.fAngle);

	Cone.fC[CONE_TOP] = Cone.fR[CONE_TOP] * 2.0f *PIf;
	Cone.fC[CONE_BOT] = Cone.fR[CONE_BOT] * 2.0f *PIf;

	Donut.fR[CONE_INNER] = (Cone.fHeight - Cone.fScanHeight) / cosf(Cone.fAngle);
	Donut.fR[CONE_OUTER] = Cone.fHeight / cosf(Cone.fAngle);

	Donut.fC[CONE_INNER] = Donut.fR[CONE_INNER] * 2.0f *PIf;
	Donut.fC[CONE_OUTER] = Donut.fR[CONE_OUTER] * 2.0f *PIf;


	if(Donut.fSector[0]>Donut.fSector[1]) {
		float fTemp = Donut.fSector[0];
		Donut.fSector[0] = Donut.fSector[1];
		Donut.fSector[1] = fTemp;
	}

	float fSectorAngle = (Donut.fSector[1] - Donut.fSector[0]) * 2.0f * PIf / 360.0f;
	//These should give exactly the same answer and is therefore superfolous
	Donut.fArcAngle = (Cone.fC[CONE_BOT] / Donut.fC[CONE_OUTER]) * fSectorAngle;
	Donut.fArcAngle = (Cone.fC[CONE_TOP] / Donut.fC[CONE_INNER]) * fSectorAngle;
	Donut.fTriangleAngle = Donut.fArcAngle / 2.0f;

	Donut.fPaperWidth = Donut.fR[CONE_OUTER] * sinf(Donut.fTriangleAngle) * 2.0f;
	Donut.fPaperHeight = Donut.fR[CONE_OUTER] - ( Donut.fR[CONE_INNER] * cosf(Donut.fTriangleAngle) );

	CRect rrSamples;
	Donut.nArcBufferWidth = (int)((float)m_pData->m_nSamplesLine * Donut.fPaperWidth / Cone.fC[CONE_BOT]);
	Donut.nArcBufferHeight = (int)((float)m_pData->m_nNumberLines * Donut.fPaperHeight / Cone.fScanHeight);
	m_dComputationTime = m_pData->GenerateArcAmpBuffer(m_nImageNumber,&Cone,&Donut,m_fGratStartAngle);

}

void CPrintConesCylindersDlg::RenderArcAmpBuffer(CPaintDC* pDC,CRect rr,char *pScreenBuffer,char* cColor, CRect rrSrc)
{
	if(Donut.cArcAmpBuffer == NULL) return;

	char* pBuffer;
	int	nX,nY,nOffset;
	int	nLine,nSample,color;
	int nSrcPtr,nPtr;

	int nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;
	int nWidth = rr.Width()+2;
	int nArrayLength=Donut.nArcBufferWidth * Donut.nArcBufferHeight;

	unsigned char *pSrcBuffer = m_pData->m_cAmpBuffer[m_nImageNumber];
	int nSize = m_pData->m_nSamplesLine * m_pData->m_nNumberLines;

	for(nY=0;nY<rr.Height();nY++) {
		nOffset=nY*rr.Width();
		nLine=MulDiv(nY,rrSrc.Height(),rr.Height()) + rrSrc.top;
		nSrcPtr = nLine * Donut.nArcBufferWidth;

		pBuffer=&pScreenBuffer[nOffset * nBytesPixel];
		for(nX=0;nX<=rr.Width();nX++) {
				
			nSample = MulDiv(nX,rrSrc.Width(),rr.Width()-1) + rrSrc.left;

			if(Donut.nSrcPtrArray[nSrcPtr + nSample]!=0) {
				nPtr = Donut.nSrcPtrArray[nSrcPtr + nSample] + Donut.nSample[0];
				if(nPtr<0) nPtr = 0;
				if(nPtr >= nSize) nPtr = nSize - 1;

				color = pSrcBuffer[nPtr];
			} else {
				color = 0;
			}
			color*=4;
			*pBuffer++=cColor[color];
			*pBuffer++=cColor[color+1];
			*pBuffer++=cColor[color+2];
			if(nBytesPixel>3) pBuffer++;
		};
	};

}

void CPrintConesCylindersDlg::OnButtonCrossHair() 
{

	m_bRenderCrossHair == false ? m_bRenderCrossHair = true : m_bRenderCrossHair = false;
	SetToolBarCheckedState();

	CRect rr;
	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);
	rr.DeflateRect(2,2);

	m_ptDown = rr.CenterPoint();
	Invalidate(false);
}



void CPrintConesCylindersDlg::OnButtonPaperWidth() 
{
	if(m_bFullScalePrintOut == false) {
		m_fVertMag = theApp.m_LastSettings.fPrintPaperWidth / Donut.fPaperHeight;
		if(m_fVertMag>1.0f) m_fVertMag=1.0f;
		m_bFullScalePrintOut = true;
		m_bHalfScalePrintOut = false;
	} else {
		m_fVertMag = 1.0f;
		m_bHalfScalePrintOut = false;
		m_bFullScalePrintOut = false;
	}
	SetToolBarCheckedState();
	Invalidate(false);

}

void CPrintConesCylindersDlg::OnButtonHalfPaperWidth() 
{
	m_fPrintOutScale = 0.5f;
	if(m_bHalfScalePrintOut == false) {
		m_fVertMag = theApp.m_LastSettings.fPrintPaperWidth / (Donut.fPaperHeight / 2.0f);
		if(m_fVertMag>1.0f) m_fVertMag=1.0f;
		m_bFullScalePrintOut = false;
		m_bHalfScalePrintOut = true;
	} else {
		m_fVertMag = 1.0f;
		m_bHalfScalePrintOut = false;
		m_bFullScalePrintOut = false;
	}
	SetToolBarCheckedState();
	Invalidate(false);
}


void CPrintConesCylindersDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nPage;

	nPage = (int)(360.0f * m_fHoriMag);

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
	if(CurrentPos < 0) CurrentPos = 0;

	if(CurrentPos>=360) CurrentPos=360;
	Donut.fSector[0]=(float)CurrentPos;
	if(Donut.fSector[0]<0.0f) Donut.fSector[0] = 0.0f;
	if(Donut.fSector[0]>=360.0f) Donut.fSector[0] = 360.0f;

	SetScrollPos(SB_HORZ,CurrentPos,TRUE);
	Invalidate(FALSE);

	
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPrintConesCylindersDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nPage;

	nPage = (int)(Donut.nArcBufferHeight * m_fVertMag);

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

	if(CurrentPos>=Donut.nArcBufferHeight) CurrentPos=Donut.nArcBufferHeight-1;
	m_ZoomSamples.top=CurrentPos;
	MinMax(&m_ZoomSamples.top,0,Donut.nArcBufferHeight-1);

	SetScrollPos(SB_VERT,CurrentPos,TRUE);
	Invalidate(FALSE);

	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CPrintConesCylindersDlg::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

long CPrintConesCylindersDlg::MinMax(long *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

void CPrintConesCylindersDlg::PrintHeader(CPaintDC *pDC)
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



void CPrintConesCylindersDlg::OnButtonCone11() 
{
	CRect rrView,rrWindow;

	GetWindowRect(rrWindow);
	m_staticView.GetWindowRect(rrView);
	
	int nTopMargin = rrView.top - rrWindow.top;
	int nBotMargin = rrView.bottom - rrWindow.bottom;

	rrView.bottom = rrView.top + MulDiv(rrView.Width(), (int)Donut.fPaperHeight, (int)Donut.fPaperWidth);
	rrWindow.bottom = rrView.bottom + nBotMargin;

	SetWindowPos( &wndTop , rrWindow.left, rrWindow.top, rrWindow.Width(), rrWindow.Height(), NULL);
}

void CPrintConesCylindersDlg::RenderGraticule(CPaintDC* pDC, CRect rr)
{
	if(m_bRenderGraticule != true) return;
	if(m_fVertMag != 1.0f) return;
	CRect rrArc;
	CPen pen(PS_SOLID,2,theApp.m_LastSettings.rgbConeTextColor);
	CPen penWhite(PS_SOLID,2,RGB(255,255,255));
	CPen* pOldPen = pDC->SelectObject(&pen);
	int	nScreenWidth,nScreenHeight,nTemp;
	float fX,fY;
	CPoint pt[2];
	float fRadius;
	CFont *pOldFont;
	CString Buff;
	CString Degrees = L"\x0b0";
	CSize size;
	int ndx,ndy;
	float fAngle;
	float fIndex,fStep;
	float fLengthPerDegree;
	int nOldMode = pDC->SetBkMode(TRANSPARENT);

	CFont *pFont = NULL;
	LOGFONT lf;
	CopyMemory(&lf,&theApp.m_LastSettings.lfViewGraticule,sizeof lf);
	
	for(int aa=0;aa<2;aa++) {
		fRadius = Donut.fR[aa];

		fX = fRadius * sinf(Donut.fTriangleAngle);
		fY = fRadius * cosf(Donut.fTriangleAngle);

		pt[0].y = pt[1].y = (int)((fY - (Donut.fR[CONE_OUTER] - Donut.fPaperHeight)) * (float)rr.Height() / Donut.fPaperHeight);
		pt[0].x = rr.CenterPoint().x - (int)(fX * (float)(rr.Width()/2) / (Donut.fPaperWidth / 2.0f));
		pt[1].x = rr.CenterPoint().x + (int)(fX * (float)(rr.Width()/2) / (Donut.fPaperWidth / 2.0f));
		
		nScreenWidth = (int)(fRadius * (float)rr.Width() / Donut.fPaperWidth);
		nScreenHeight = (int)(fRadius * (float)rr.Height() / Donut.fPaperHeight);
		rrArc.bottom = rr.bottom - (int)((Donut.fR[CONE_OUTER]-fRadius) * (float)rr.Height() / Donut.fPaperHeight);
		rrArc.left = rr.CenterPoint().x - nScreenWidth;
		rrArc.right = rr.CenterPoint().x + nScreenWidth;
		rrArc.top = rrArc.bottom - (2 * nScreenHeight);
		if(theApp.m_LastSettings.nConeTextTransparent == 0) {
			pFont = new CFont;
			lf.lfEscapement=0;
			lf.lfHeight = theApp.m_LastSettings.nConeFontSize;
			pFont->CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(pFont);
			Buff.Format(L"8888");
			size = pDC->GetTextExtent(Buff);
			if(aa==0) {
				rrArc.InflateRect(size.cy,size.cy);
				pDC->SelectObject(&penWhite);
				for(int nn=size.cy;nn>0;nn--) {
					pDC->Arc(rrArc,pt[0],pt[1]);
					rrArc.DeflateRect(1,1);
				}
			} else {
				rrArc.DeflateRect(size.cy,size.cy);
				pDC->SelectObject(&penWhite);
				for(int nn=size.cy;nn>0;nn--) {
					pDC->Arc(rrArc,pt[0],pt[1]);
					rrArc.InflateRect(1,1);
				}
			}
			pDC->SelectObject(&pen);
			pDC->SelectObject(pFont);
			SAFE_DELETE( pFont );
		}

		pDC->Arc(rrArc,pt[0],pt[1]);

		float fS[2];
		fS[0] =  Donut.fSector[0] - m_fGratStartAngle;
		fS[1] =  Donut.fSector[1] - m_fGratStartAngle;

		switch(theApp.m_LastSettings.nRAxisGraticuleMode) {
		default:
			fStep = 10.0f;
			fIndex = (float) (((int)fS[0] / (int)fStep) * (int)fStep);
			break;
		case 1:
			fLengthPerDegree = (Cone.fC[aa] / 25.4f) / 360.0f;
			fS[0] *= fLengthPerDegree;
			fS[1] *= fLengthPerDegree;
			fStep = 6.0f;
			nTemp = (int)(fS[0] / fStep);
			fIndex = (float)nTemp * fStep;
			break;
		};

		
		for(fIndex; fIndex <= fS[1]; fIndex+=fStep) {

			float fTemp = (fIndex - fS[0]) / (fS[1]-fS[0]);

			fAngle = (fTemp * Donut.fArcAngle) -  Donut.fTriangleAngle;
			fX = fRadius * sinf(fAngle);
			fY = fRadius * cosf(fAngle);
			pt[0].y = pt[1].y = (int)((fY - (Donut.fR[CONE_OUTER] - Donut.fPaperHeight)) * (float)rr.Height() / Donut.fPaperHeight) + rr.top;
			pt[0].x = rr.CenterPoint().x + (int)(fX * (float)(rr.Width()/2) / (Donut.fPaperWidth / 2.0f));

			pFont = new CFont;
			lf.lfEscapement=(long)(tanf(PIf - fAngle) * RAD_TO_DEG * -10.0);
			pFont->CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(pFont);
			pDC->SetTextColor(theApp.m_LastSettings.rgbConeTextColor);

			switch(theApp.m_LastSettings.nRAxisGraticuleMode) {
			default:
				if(theApp.m_LastSettings.nRAxisGraticuleReverse == 0) {
					Buff.Format(L"%.0f%s",fmodf((fIndex+360.0f),360.0f),Degrees);
				} else {
					Buff.Format(L"%.0f%s",360.0f - fmodf((fIndex+360.0f),360.0f),Degrees);
				}
				break;
			case 1:	Buff.Format(L"%.0f%s",fIndex,theApp.m_Units);
				break;
			};
			size = pDC->GetTextExtent(Buff);

			if(aa==0) {
				ndx = (int)(6.0f * sinf(fAngle));
				ndy = (int)(6.0f * cosf(fAngle));
				pDC->TextOut(pt[0].x+ndx,pt[0].y,Buff);
			} else {
				ndx = (int)(- (float)size.cy * sinf(fAngle));
				ndy = (int)(- (float)size.cy * cosf(fAngle));
				pDC->TextOut(pt[0].x+ndx,pt[0].y+ndy,Buff);
				ndx = (int)(-6.0f * sinf(fAngle));
				ndy = (int)(-6.0f * cosf(fAngle));
			}
			pDC->MoveTo(pt[0]);
			pDC->LineTo(pt[0].x+ndx,pt[0].y+ndy);

			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldFont);
			delete pFont;

		}

	}

	pDC->SetBkMode(nOldMode);
	pDC->SelectObject(pOldPen);

}


void CPrintConesCylindersDlg::RenderCrossHair(CPaintDC* pDC, CRect rr)
{
	if(m_bRenderCrossHair != true) return;
	if(m_fVertMag != 1.0f) return;

	CRect rrArc;
	CPen pen(PS_SOLID,2,theApp.m_LastSettings.rgbConeTextColor);
	CPen penWhite(PS_SOLID,2,RGB(255,255,255));
	CPen* pOldPen = pDC->SelectObject(&pen);
	int	nScreenWidth,nScreenHeight;
	float fX,fY;
	CPoint pt[2];
	float fRadius,fCircumference;
	CString Buff;
	CString Degrees = L"\x0b0";
	CSize size;
	float fAngle,fStep,fIndex,fLengthPerDegree;
	float fS[2];
	int nTemp,ndx,ndy;
	int nOldMode = pDC->SetBkMode(TRANSPARENT);

	if(m_fCrossHairDonutRadius<0.0f) m_fCrossHairDonutRadius=0.0f;
	if(m_fCrossHairDonutRadius>Donut.fR[CONE_OUTER]) m_fCrossHairDonutRadius=Donut.fR[CONE_OUTER];

	CFont *pFont = new CFont;
	LOGFONT lf;
	CopyMemory(&lf,&theApp.m_LastSettings.lfViewGraticule,sizeof lf);
	lf.lfEscapement=-900;
	lf.lfHeight = theApp.m_LastSettings.nConeFontSize;
	pFont->CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(pFont);

	//Vertical line
	fS[0] =  Donut.fSector[0];
	fS[1] =  Donut.fSector[1];
	float fTemp = (m_fCrossHairAngle - fS[0]) / (fS[1]-fS[0]);
	fAngle = (fTemp * Donut.fArcAngle) -  Donut.fTriangleAngle;

	for(int aa=0;aa<2;aa++) {
		fRadius = Donut.fR[aa];

		fX = fRadius * sinf(fAngle);
		fY = fRadius * cosf(fAngle);
		pt[aa].y = pt[1].y = (int)((fY - (Donut.fR[CONE_OUTER] - Donut.fPaperHeight)) * (float)rr.Height() / Donut.fPaperHeight) + rr.top;
		pt[aa].x = rr.CenterPoint().x + (int)(fX * (float)(rr.Width()/2) / (Donut.fPaperWidth / 2.0f));
	
	}
	if(theApp.m_LastSettings.nConeTextTransparent == 0) {
		Buff.Format(L"8888");
		size = pDC->GetTextExtent(Buff);
		pDC->SelectObject(&penWhite);
		for(int nn=size.cy;nn>0;nn--) {
			pDC->MoveTo(pt[0].x-nn,pt[0].y);
			pDC->LineTo(pt[1].x-nn,pt[1].y);
		}
		pDC->SelectObject(&pen);
	}
	pDC->MoveTo(pt[0]);
	pDC->LineTo(pt[1]);
	SAFE_DELETE( pFont);

	for(fRadius = m_fCrossHairDonutRadius; fRadius > Donut.fR[CONE_INNER]; fRadius -= 50.8f) {
		fX = fRadius * sinf(fAngle);
		fY = fRadius * cosf(fAngle);
		pt[0].y = pt[1].y = (int)((fY - (Donut.fR[CONE_OUTER] - Donut.fPaperHeight)) * (float)rr.Height() / Donut.fPaperHeight) + rr.top;
		pt[0].x = rr.CenterPoint().x + (int)(fX * (float)(rr.Width()/2) / (Donut.fPaperWidth / 2.0f));
		ndx = (int)(6.0f * cosf(fAngle));
		ndy = (int)(6.0f * sinf(fAngle));
		pDC->MoveTo(pt[0]);
		pDC->LineTo(pt[0].x-ndx,pt[0].y+ndy);
		Buff.Format(L"%.0f%s",fabs(fRadius - m_fCrossHairDonutRadius) / 25.4,theApp.m_Units);

		pFont = new CFont;
		lf.lfEscapement=(long)((90-tanf(fAngle-PIf) * RAD_TO_DEG) * -10.0);
		pFont->CreateFontIndirect(&lf);
		pDC->SelectObject(pFont);

		pDC->TextOut(pt[0].x,pt[0].y,Buff);
	
		SAFE_DELETE( pFont );
	}

	for(fRadius = m_fCrossHairDonutRadius+50.8f; fRadius < Donut.fR[CONE_OUTER]; fRadius += 50.8f) {
		fX = fRadius * sinf(fAngle);
		fY = fRadius * cosf(fAngle);
		pt[0].y = pt[1].y = (int)((fY - (Donut.fR[CONE_OUTER] - Donut.fPaperHeight)) * (float)rr.Height() / Donut.fPaperHeight) + rr.top;
		pt[0].x = rr.CenterPoint().x + (int)(fX * (float)(rr.Width()/2) / (Donut.fPaperWidth / 2.0f));
		ndx = (int)(6.0f * cosf(fAngle));
		ndy = (int)(6.0f * sinf(fAngle));
		pDC->MoveTo(pt[0]);
		pDC->LineTo(pt[0].x-ndx,pt[0].y+ndy);
		Buff.Format(L"%.0f%s",fabs(fRadius - m_fCrossHairDonutRadius) / 25.4,theApp.m_Units);

		pFont = new CFont;
		lf.lfEscapement=(long)((90-tanf(fAngle-PIf) * RAD_TO_DEG) * -10.0);
		pFont->CreateFontIndirect(&lf);
		pDC->SelectObject(pFont);

		pDC->TextOut(pt[0].x,pt[0].y,Buff);
	
		SAFE_DELETE( pFont );
	}


	//Circumferential line
	fX = m_fCrossHairDonutRadius * sinf(Donut.fTriangleAngle);
	fY = m_fCrossHairDonutRadius * cosf(Donut.fTriangleAngle);

	pt[0].y = pt[1].y = (int)((fY - (Donut.fR[CONE_OUTER] - Donut.fPaperHeight)) * (float)rr.Height() / Donut.fPaperHeight) + rr.top;
	pt[0].x = rr.CenterPoint().x - (int)(fX * (float)(rr.Width()/2) / (Donut.fPaperWidth / 2.0f));
	pt[1].x = rr.CenterPoint().x + (int)(fX * (float)(rr.Width()/2) / (Donut.fPaperWidth / 2.0f));
		
	nScreenWidth = (int)(m_fCrossHairDonutRadius * (float)rr.Width() / Donut.fPaperWidth);
	nScreenHeight = (int)(m_fCrossHairDonutRadius * (float)rr.Height() / Donut.fPaperHeight);
	rrArc.bottom = rr.bottom - (int)((Donut.fR[CONE_OUTER]-m_fCrossHairDonutRadius) * (float)rr.Height() / Donut.fPaperHeight);
	rrArc.left = rr.CenterPoint().x - nScreenWidth;
	rrArc.right = rr.CenterPoint().x + nScreenWidth;
	rrArc.top = rrArc.bottom - (2 * nScreenHeight);
	if(theApp.m_LastSettings.nConeTextTransparent == 0) {
		Buff.Format(L"8888");
		size = pDC->GetTextExtent(Buff);
		rrArc.InflateRect(size.cy,size.cy);
		pDC->SelectObject(&penWhite);
		for(int nn=size.cy;nn>0;nn--) {
			pDC->Arc(rrArc,pt[0],pt[1]);
			rrArc.DeflateRect(1,1);
		}
		pDC->SelectObject(&pen);
	}
	pDC->Arc(rrArc,pt[0],pt[1]);

	fS[0] =  Donut.fSector[0] - m_fCrossHairAngle;
	fS[1] =  Donut.fSector[1] - m_fCrossHairAngle;

	switch(theApp.m_LastSettings.nRAxisGraticuleMode) {
	default:
		fStep = 10.0f;
		fIndex = (float) (((int)fS[0] / (int)fStep) * (int)fStep);
		break;
	case 1:
		fCircumference = m_fCrossHairDonutRadius * 2.0f * PIf;
		fCircumference = fCircumference * Donut.fArcAngle / ((fS[1]-fS[0]) * DEG_TO_RAD);
		fLengthPerDegree = (fCircumference / 25.4f) / 360.0f;
		fS[0] *= fLengthPerDegree;
		fS[1] *= fLengthPerDegree;
		fStep = 2.0f;
		nTemp = (int)(fS[0] / fStep);
		fIndex = (float)nTemp * fStep;
		break;
	};

	pDC->SelectObject(pOldFont);
	SAFE_DELETE( pFont);
		
	for(fIndex; fIndex <= fS[1]; fIndex+=fStep) {

		float fTemp = (fIndex - fS[0]) / (fS[1]-fS[0]);

		fAngle = (fTemp * Donut.fArcAngle) -  Donut.fTriangleAngle;
		fX = m_fCrossHairDonutRadius * sinf(fAngle);
		fY = m_fCrossHairDonutRadius * cosf(fAngle);
		pt[0].y = pt[1].y = (int)((fY - (Donut.fR[CONE_OUTER] - Donut.fPaperHeight)) * (float)rr.Height() / Donut.fPaperHeight) + rr.top;
		pt[0].x = rr.CenterPoint().x + (int)(fX * (float)(rr.Width()/2) / (Donut.fPaperWidth / 2.0f));

		pFont = new CFont;
		lf.lfEscapement=(long)(tanf(PIf - fAngle) * RAD_TO_DEG * -10.0);
		pFont->CreateFontIndirect(&lf);
		pOldFont = pDC->SelectObject(pFont);
		pDC->SetTextColor(theApp.m_LastSettings.rgbConeTextColor);

		switch(theApp.m_LastSettings.nRAxisGraticuleMode) {
		default:
			if(theApp.m_LastSettings.nRAxisGraticuleReverse == 0) {
				Buff.Format(L"%.0f%s",fmodf((fIndex+360.0f),360.0f),Degrees);
			} else {
				Buff.Format(L"%.0f%s",360.0f-fmodf((fIndex+360.0f),360.0f),Degrees);
			}
			break;
		case 1:	Buff.Format(L"%.0f%s",fIndex,theApp.m_Units);
			break;
		};
		size = pDC->GetTextExtent(Buff);

		ndx = (int)(6.0f * sinf(fAngle));
		ndy = (int)(6.0f * cosf(fAngle));
		pDC->MoveTo(pt[0]);
		pDC->LineTo(pt[0].x+ndx,pt[0].y+ndy);
		pDC->TextOut(pt[0].x+ndx,pt[0].y,Buff);

		pDC->SelectObject(pOldFont);
		SAFE_DELETE( pFont );

	}

	pDC->SelectObject(pOldPen);
	pDC->SetBkMode(nOldMode);
}

bool CPrintConesCylindersDlg::ClientToWorld(CPoint pt, CRect rr, float *fAngle, float *fHeight)
{
	float fX,fY,fHalfSectorAngle,fPtAngle;
	CPoint ptCentre;
	CString Buff;


	ptCentre.x = rr.CenterPoint().x;
	ptCentre.y = (int)(Donut.fR[CONE_OUTER] * (float)rr.Height() / Donut.fPaperHeight);

	fX = (float)(pt.x - ptCentre.x) * (Donut.fPaperWidth/2.0f) / (float)(rr.Width()/2);
	fY = Donut.fR[CONE_OUTER] - (float)(rr.bottom - pt.y) * (Donut.fPaperHeight) / (float)rr.Height();

	fPtAngle = atan(fX/fY);
	fHalfSectorAngle = (Donut.fSector[1] - Donut.fSector[0]) * DEG_TO_RAD / 2.0f;

	*fAngle = Donut.fSector[0] + (fHalfSectorAngle + (fPtAngle * fHalfSectorAngle / Donut.fTriangleAngle)) * RAD_TO_DEG;
	*fHeight = sqrtf(fX*fX+fY*fY);

	Buff.Format(L"Radius %.0f\x022 Angle %.1f",*fHeight / 25.4, *fAngle );

	CEdit* pEdit = (CEdit* ) m_wndDialogBar.GetDlgItem(IDC_EDIT_TEXT);
	pEdit->SetWindowText(Buff);

	return true;
}

void CPrintConesCylindersDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CPoint pt = point;
	CRect rr;

	ScreenToClient(&point);
	m_ptClient = point;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	if(rr.PtInRect(point)) {
		if (menu->LoadMenu(IDR_RB_CONE_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
		};
	}

	delete menu;

	
}

void CPrintConesCylindersDlg::OnConeGraticulezeropos() 
{
	float fHeight;
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	ClientToWorld(m_ptClient,rr,&m_fGratStartAngle,&fHeight);

	Invalidate(false);
}

void CPrintConesCylindersDlg::OnButtonConeGraticule() 
{

	m_bRenderGraticule == false ? m_bRenderGraticule = true : m_bRenderGraticule = false;
	SetToolBarCheckedState();
	Invalidate(false);
}

void CPrintConesCylindersDlg::OnConePaletteno1() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nPaletteNumber = 0;
	pSheet->ChangePaletteOfActivePage(m_nPaletteNumber);
	Invalidate(false);
}

void CPrintConesCylindersDlg::OnConePaletteno2() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nPaletteNumber = 1;
	pSheet->ChangePaletteOfActivePage(m_nPaletteNumber);
	Invalidate(false);
}

void CPrintConesCylindersDlg::OnConePaletteno3() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nPaletteNumber = 2;
	pSheet->ChangePaletteOfActivePage(m_nPaletteNumber);
	Invalidate(false);
}

void CPrintConesCylindersDlg::OnConePaletteno4() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nPaletteNumber = 3;
	pSheet->ChangePaletteOfActivePage(m_nPaletteNumber);
	Invalidate(false);
}

void CPrintConesCylindersDlg::OnConePaletteno5() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nPaletteNumber = 4;
	pSheet->ChangePaletteOfActivePage(m_nPaletteNumber);
	Invalidate(false);
}

void CPrintConesCylindersDlg::OnConePaletteno6() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nPaletteNumber = 5;
	pSheet->ChangePaletteOfActivePage(m_nPaletteNumber);
	Invalidate(false);
}

void CPrintConesCylindersDlg::OnConePaletteno7() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nPaletteNumber = 6;
	pSheet->ChangePaletteOfActivePage(m_nPaletteNumber);
	Invalidate(false);
}

