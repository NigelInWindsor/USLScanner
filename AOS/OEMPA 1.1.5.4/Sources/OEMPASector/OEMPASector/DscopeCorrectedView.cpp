// DscopeView.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "DscopeCorrectedView.h"
#include <math.h>
#include "ExternalDefinition.h"
#include "MainFrm.h"

const double g_bAngleFactor=1.0;//+1.0 or -1.0

long lround2(double x)
{
	long nx = (long)x;
	if(x>0)
	{
		if((x-nx)>0.5)
			return nx+1;
		else
			return nx;
	}else{
		if((x-nx)<-0.5)
			return nx-1;
		else
			return nx;
	}
}

int CDscopeCorrectedView::GetAcqLengthEncoderPointQuantity()
{
	double dUltrasoundStep = GetPointStepAnalysis();//digitizing distance (meter)
	double dUltrasoundMax = InfoProbe1Analysis.PointQuantity * dUltrasoundStep;
	unsigned int iCycle = GetAnalysisCycle();
	double dDepthMax = dUltrasoundMax*cos(g_bAngleFactor*InfoProbe1Analysis.pdAngle[iCycle]);
	double dEncoderStep = SystemAnalysis.dEncoderStep;
	int iLength = (int)lround2(abs(dDepthMax*sin(g_bAngleFactor*InfoProbe1Analysis.pdAngle[iCycle])/dEncoderStep));
	return GetAnalysisEncoderPositionQuantity()+iLength;
}
 
double CDscopeCorrectedView::GetAcqLengthEncoder()
{
	double dUltrasoundStep = GetPointStepAnalysis();//digitizing distance (meter)
	double dUltrasoundMax = InfoProbe1Analysis.PointQuantity * dUltrasoundStep;
	unsigned int iCycle = GetAnalysisCycle();
	double dEncoderStep = SystemAnalysis.dEncoderStep;
	double dLength = abs(dUltrasoundMax*sin(g_bAngleFactor*InfoProbe1Analysis.pdAngle[iCycle]));
	return g_dEncoderAxisLengthAnalysis+dLength;
}
 
double CDscopeCorrectedView::GetAcqDepth()
{
	double dUltrasoundStep = GetPointStepAnalysis();//digitizing distance (meter)
	double dEncoderStep = SystemAnalysis.dEncoderStep;
	double dUltrasoundMax = InfoProbe1Analysis.PointQuantity * dUltrasoundStep;
	unsigned int iCycle = GetAnalysisCycle();
	double dDepthMax = dUltrasoundMax*cos(g_bAngleFactor*InfoProbe1Analysis.pdAngle[iCycle]);
	return dDepthMax;
}
 
// CDscopeCorrectedView

IMPLEMENT_DYNCREATE(CDscopeCorrectedView, CView)

CDscopeCorrectedView::CDscopeCorrectedView()
{
	g_pDscopeCorrected = this;

	m_bOnSizing = false;
	m_uiEvent = 5;

	m_iViewHeight = 100;
	m_iViewWidth = 100;
	m_pBuffer= NULL;
	m_pDataBuffer = NULL;

	m_pCosTable = NULL;
	m_pSinTable = NULL;

	m_aiBitmapWidthIndex = NULL;
	m_aiBitmapHeightIndex = NULL;
	
	m_bCursor = false;
	m_ptPrev.x=0;m_ptPrev.y=0;

	CreatePalette();
}

CDscopeCorrectedView::~CDscopeCorrectedView()
{
	g_pDscopeCorrected = NULL;

	if (m_pBuffer)
		delete[] m_pBuffer;

	if (m_pDataBuffer)
		delete[] m_pDataBuffer;

	if (m_pCosTable)
		delete[] m_pCosTable;

	if (m_pSinTable)
		delete[] m_pSinTable;

	if (m_aiBitmapWidthIndex)
		delete[] m_aiBitmapWidthIndex;

	if (m_aiBitmapHeightIndex)
		delete[] m_aiBitmapHeightIndex;
}

BEGIN_MESSAGE_MAP(CDscopeCorrectedView, CView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()

// CDscopeCorrectedView drawing

void CDscopeCorrectedView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	pDoc->SetTitle(_T("D-scan2 Analysis"));
}

// CDscopeCorrectedView message handlers

int CDscopeCorrectedView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(TIMER_DSCOPE_VIEW, 33, NULL);

	return 0;
}

void CDscopeCorrectedView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_DSCOPE_VIEW)
	{
		if (m_uiEvent != 0)
		{
			UpdateDisplayData();
			m_uiEvent = 0;
		}

		if ((g_uiMode == 1) && (m_bOnSizing == false))
			Display();
	}

	CView::OnTimer(nIDEvent);
}

void CDscopeCorrectedView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_iViewHeight = cy;
	m_iViewWidth = cx;
}

void CDscopeCorrectedView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CView::OnSizing(fwSide, pRect);
	m_bOnSizing = true;
}

void CDscopeCorrectedView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

void CDscopeCorrectedView::UpdateDisplayData()
{
	if (g_uiMode == 1)
	{
		ResetBuffer();
		CreateAngleTable();
		SetMapping();

		if (g_uiProbeAnalysis == 0)
			DisplayProbe1Gate();
	}
}

void CDscopeCorrectedView::SetEvent(UINT Event)
{
	m_uiEvent = Event;

	if (m_uiEvent == 1)
		m_bOnSizing = false;
}

void CDscopeCorrectedView::CreateAngleTable()
{
	if (m_pCosTable)
		delete[] m_pCosTable;

	if (m_pSinTable)
		delete[] m_pSinTable;

	m_pCosTable = new double[InfoProbe1Analysis.iCycleCount];
	m_pSinTable = new double[InfoProbe1Analysis.iCycleCount];

	for (unsigned int iCycle = 0 ; iCycle < (unsigned int)InfoProbe1Analysis.iCycleCount ; iCycle++)
	{
		m_pCosTable[iCycle] = cos(g_bAngleFactor*InfoProbe1Analysis.pdAngle[iCycle]);
		m_pSinTable[iCycle] = sin(g_bAngleFactor*InfoProbe1Analysis.pdAngle[iCycle]);
	}
}

void CDscopeCorrectedView::CreatePalette()
{
	COLORREF TempColor[5];
	TempColor[0] = RGB(255, 0, 0);
	TempColor[1] = RGB(255, 128, 0);
	TempColor[2] = RGB(0, 255, 255);
	TempColor[3] = RGB(0, 128, 255);
	TempColor[4] = RGB(0, 0, 255);

	double Percent[5];
	Percent[0] = 0.0;
	Percent[1] = 25.0;
	Percent[2] = 50.0;
	Percent[3] = 75.0;
	Percent[4] = 100.0;

	for (int i = 0 ; i < 4 ; i++)
	{
		int Red = GetRValue(TempColor[i + 1]) - GetRValue(TempColor[i]);
		int Green = GetGValue(TempColor[i + 1]) - GetGValue(TempColor[i]);
		int Blue = GetBValue(TempColor[i + 1]) - GetBValue(TempColor[i]);

		int Start = (int)(256.0 * Percent[i] / 100.0);
		int End = (int)(256.0 * Percent[i + 1] / 100.0);

		for (int j = Start ; j < End ; j++)
		{
			m_cPalette[j] = RGB(GetRValue(TempColor[i]) + Red * (j - Start) / (End - Start),
								GetGValue(TempColor[i]) + Green  * (j - Start) / (End - Start),
								GetBValue(TempColor[i]) + Blue * (j - Start) / (End - Start));
		}
	}
}

void CDscopeCorrectedView::ResetBuffer()
{
	CMainFrame *pMainFrame=CMainFrame::GetMainFrame();
	unsigned int iCycle = GetAnalysisCycle();
	unsigned int iAcqEncoderCount=GetAnalysisEncoderPositionQuantity();

	m_bCursor = false;
	m_ptPrev.x=0;m_ptPrev.y=0;
	if(pMainFrame)
		pMainFrame->ResetCursor();
	if (m_pBuffer)
	{
		delete[] m_pBuffer;
		delete[] m_pDataBuffer;

		delete[] m_aiBitmapWidthIndex;
		delete[] m_aiBitmapHeightIndex;
	}

	m_iBitmapHeight = (int)min(m_iViewHeight, (int)InfoProbe1Analysis.PointQuantity);
	m_iBitmapWidth = (int)min(m_iViewWidth, (int)GetAcqLengthEncoderPointQuantity());

	m_iBitmapSize = m_iBitmapHeight * m_iBitmapWidth;
	m_pBuffer = new DWORD[m_iBitmapHeight * m_iBitmapWidth];
	m_pDataBuffer = new BYTE[m_iBitmapHeight * m_iBitmapWidth];

	memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
	memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));

	m_aiBitmapWidthIndex = new int[iAcqEncoderCount * InfoProbe1Analysis.PointQuantity];
	m_aiBitmapHeightIndex = new int[iAcqEncoderCount * InfoProbe1Analysis.PointQuantity];

	memset(m_aiBitmapWidthIndex, -1, iAcqEncoderCount * InfoProbe1Analysis.PointQuantity * sizeof(int));
	memset(m_aiBitmapHeightIndex, -1, iAcqEncoderCount * InfoProbe1Analysis.PointQuantity * sizeof(int));
}

void CDscopeCorrectedView::SetMapping()
{
	int iBufferIndex,iBitmapBufferIndex;
	double dEncoderStep = SystemAnalysis.dEncoderStep;
	unsigned int iCycle = GetAnalysisCycle();
	double dUltrasoundStep = GetPointStepAnalysis();//digitizing distance (meter)
	double D, Lx, Lh, LxRef;
	double dAcqLengthEncoder=GetAcqLengthEncoder();

	double dDepthMin = 0.0;//GetAnalysisZAxisMin();
	double dDepthMax = GetAcqDepth();//GetAnalysisZAxisMax();//thickness (meter)

	unsigned int iAcqEncoderIndexMin = (unsigned int)(Analysis_YMIN / SystemAnalysis.dEncoderStep);
	unsigned int iAcqEncoderIndexMax = (unsigned int)min(Analysis_YMAX / SystemAnalysis.dEncoderStep, GetAnalysisEncoderPositionQuantity() - 1);

	D = (InfoProbe1Analysis.PointQuantity-1) * dUltrasoundStep;
	if(m_pSinTable[iCycle]<0)
	{
		LxRef = dEncoderStep * iAcqEncoderIndexMin + D * m_pSinTable[iCycle];
		m_xMin = LxRef;
		m_xMax = dEncoderStep * iAcqEncoderIndexMax;
	}else{
		LxRef = 0.0;
		m_xMin = 0.0;
		m_xMax = dEncoderStep * iAcqEncoderIndexMax + D * m_pSinTable[iCycle];
	}
	m_zMin = 0.0;
	m_zMax = D * m_pCosTable[iCycle];
	for (unsigned int iAcqEncoderIndex = (unsigned int)iAcqEncoderIndexMin ; iAcqEncoderIndex <= (unsigned int)iAcqEncoderIndexMax ; iAcqEncoderIndex++)
	{
		if(iAcqEncoderIndex<0)
			continue;
		if(iAcqEncoderIndex>=GetAnalysisEncoderPositionQuantity())
			continue;
		for (unsigned int iAscanIndex = 0 ; iAscanIndex < (unsigned int)InfoProbe1Analysis.PointQuantity ; iAscanIndex++)
		{
			D = iAscanIndex * dUltrasoundStep;
			Lx = dEncoderStep * iAcqEncoderIndex + D * m_pSinTable[iCycle];
			Lh = D * m_pCosTable[iCycle];

			// Management of the rebound
			//if (g_bBackwallRebound && (Lh > g_dPieceThicknessAnalysis))
			//	Lh = 2 * g_dPieceThicknessAnalysis - Lh;

			iBufferIndex = iAcqEncoderIndex * InfoProbe1Analysis.PointQuantity + iAscanIndex;
			m_aiBitmapWidthIndex[iBufferIndex] = (int)lround2((Lx-LxRef)*m_iBitmapWidth/dAcqLengthEncoder);
			if(m_aiBitmapWidthIndex[iBufferIndex]>=m_iBitmapWidth)
				m_aiBitmapWidthIndex[iBufferIndex] = m_iBitmapWidth-1;
			m_aiBitmapHeightIndex[iBufferIndex] = (int)lround2(m_iBitmapHeight * iAscanIndex / InfoProbe1Analysis.PointQuantity);
			iBitmapBufferIndex = m_aiBitmapHeightIndex[iBufferIndex] * m_iBitmapWidth + m_aiBitmapWidthIndex[iBufferIndex];
			if ((iBitmapBufferIndex<0) || (iBitmapBufferIndex >= m_iBitmapSize))
				iBufferIndex = 0;
			if((m_aiBitmapHeightIndex[iBufferIndex]==m_iBitmapHeight-1) && !m_aiBitmapWidthIndex[iBufferIndex])
				iBufferIndex = 0;
		}
	}
}

void CDscopeCorrectedView::DisplayProbe1Gate()
{
	unsigned int iCycle = GetAnalysisCycle();

	double dDepthMin = 0.0;//GetAnalysisZAxisMin();
	double dDepthMax = GetAcqDepth();//GetAnalysisZAxisMax();//thickness (meter)

	unsigned int iAcqEncoderIndexMin = (unsigned int)(Analysis_YMIN / SystemAnalysis.dEncoderStep);
	unsigned int iAcqEncoderIndexMax = (unsigned int)min(Analysis_YMAX / SystemAnalysis.dEncoderStep, GetAnalysisEncoderPositionQuantity() - 1);

	COLORREF BlankColor = RGB(255, 255, 255);

	EnterCriticalSection(&CRT_ANALYSIS_MEMORY_1);

	for (unsigned int iAcqEncoderIndex = (unsigned int)iAcqEncoderIndexMin ; iAcqEncoderIndex <= (unsigned int)iAcqEncoderIndexMax ; iAcqEncoderIndex++)
	{
		if(iAcqEncoderIndex<0)
			continue;
		if(iAcqEncoderIndex>=GetAnalysisEncoderPositionQuantity())
			continue;
		for (unsigned int iAscanIndex = 0 ; iAscanIndex < (unsigned int)InfoProbe1Analysis.PointQuantity ; iAscanIndex++)
		{
			int iBufferIndex = iAcqEncoderIndex * InfoProbe1Analysis.PointQuantity + iAscanIndex;
			int iAcqBufferIndex =  iCycle * InfoProbe1Analysis.PointQuantity + iAscanIndex;
			int iBitmapBufferIndex = m_aiBitmapHeightIndex[iBufferIndex] * m_iBitmapWidth + m_aiBitmapWidthIndex[iBufferIndex];
			if ((iBitmapBufferIndex<0) || (iBitmapBufferIndex >= m_iBitmapSize))
				continue;
			m_pDataBuffer[iBitmapBufferIndex] = (BYTE)max(m_pDataBuffer[iBitmapBufferIndex], abs(AscanAnalysisMemory1[iAcqEncoderIndex][iAcqBufferIndex]) * 2 - 1);
			m_pBuffer[iBitmapBufferIndex] = m_cPalette[m_pDataBuffer[iBitmapBufferIndex]];
			//to test the filling if (g_bBackwallRebound)
			if(iAcqEncoderIndex)
			{
				int iPrevBufferIndex = (iAcqEncoderIndex-1) * InfoProbe1Analysis.PointQuantity + iAscanIndex;
				int iPrevBitmapBufferIndex = m_aiBitmapHeightIndex[iPrevBufferIndex] * m_iBitmapWidth + m_aiBitmapWidthIndex[iPrevBufferIndex];
				if ((iPrevBitmapBufferIndex<0) || (iPrevBitmapBufferIndex >= m_iBitmapSize))
					continue;
				if(m_aiBitmapHeightIndex[iPrevBufferIndex]!=m_aiBitmapHeightIndex[iBufferIndex])
					continue;
				int iDelta=m_aiBitmapWidthIndex[iBufferIndex]-m_aiBitmapWidthIndex[iPrevBufferIndex];
				if(iDelta==1)
					continue;
				char prevAmp=AscanAnalysisMemory1[iAcqEncoderIndex-1][iAcqBufferIndex];
				char Amp=AscanAnalysisMemory1[iAcqEncoderIndex][iAcqBufferIndex];
				double dAmp;
				char amp;
				//for(int i=m_aiBitmapWidthIndex[iPrevBufferIndex]+1;i<m_aiBitmapWidthIndex[iBufferIndex];i++)
				//{
				//	int iBitmapBufferIndex2 = m_aiBitmapHeightIndex[iBufferIndex] * m_iBitmapWidth + i;
				//	m_pDataBuffer[iBitmapBufferIndex2] = (BYTE)max(m_pDataBuffer[iBitmapBufferIndex2], abs(AscanAnalysisMemory1[iAcqEncoderIndex][iAcqBufferIndex]) * 2 - 1);
				//	m_pBuffer[iBitmapBufferIndex2] = m_cPalette[m_pDataBuffer[iBitmapBufferIndex2]];
				//}
				for(int i=1;i<iDelta;i++)
				{
					int iBitmapBufferIndex2 = m_aiBitmapHeightIndex[iBufferIndex] * m_iBitmapWidth + i+m_aiBitmapWidthIndex[iPrevBufferIndex];
					dAmp = (prevAmp*i+Amp*(iDelta-i))/(double)iDelta;
					amp = (char)dAmp;
					m_pDataBuffer[iBitmapBufferIndex2] = (BYTE)max(m_pDataBuffer[iBitmapBufferIndex2], abs(amp) * 2 - 1);
					m_pBuffer[iBitmapBufferIndex2] = m_cPalette[m_pDataBuffer[iBitmapBufferIndex2]];
				}
			}
		}
	}

	LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_1);
}

void CDscopeCorrectedView::Display()
{
	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CPen pen;

	if (!pen.CreatePen(/*PS_DASHDOTDOT*/PS_SOLID, 1, RGB(255,0,0)))//PS_DASH
		return;

	LPBITMAPINFO pBmpInfo = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];

	pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBmpInfo->bmiHeader.biWidth = m_iBitmapWidth;
	pBmpInfo->bmiHeader.biHeight = m_iBitmapHeight;
	pBmpInfo->bmiHeader.biPlanes = 1;
	pBmpInfo->bmiHeader.biBitCount = 32;
	pBmpInfo->bmiHeader.biCompression = BI_RGB;
	pBmpInfo->bmiHeader.biSizeImage = m_iBitmapWidth * m_iBitmapHeight;
	pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	pBmpInfo->bmiHeader.biClrUsed = 1;
	pBmpInfo->bmiHeader.biClrImportant = 0;

	SetStretchBltMode(dc.m_hDC, STRETCH_DELETESCANS);
	StretchDIBits(dc.m_hDC, 
				  0, 
				  m_iViewHeight-1, 
				  m_iViewWidth, 
				  -(int)m_iViewHeight, 
				  0, 
				  0, 
				  m_iBitmapWidth, 
				  m_iBitmapHeight, 
				  m_pBuffer, 
				  pBmpInfo, 
				  DIB_RGB_COLORS, 
				  SRCCOPY);

	delete[] pBmpInfo;

	if(m_bCursor)
	{
		dc.SelectObject(&pen);
		dc.MoveTo(m_ptPrev.x,0);
		dc.LineTo(m_ptPrev.x,m_iViewHeight-1);
		dc.MoveTo(0,m_ptPrev.y);
		dc.LineTo(m_iViewWidth-1,m_ptPrev.y);
	}
}

void CDscopeCorrectedView::OnActivateView(BOOL bActivate,CView* pActivateView,CView* pDeactiveView)
{
	if(m_bCursor && (pActivateView==dynamic_cast<CView*>(this)))
	{
		SetPoint(m_ptPrev);
	}
}

void CDscopeCorrectedView::SetPoint(CPoint point)
{
	CMainFrame *pMainFrame=CMainFrame::GetMainFrame();

	m_ptPrev = point;   // Serves as the MoveTo() anchor point for the
						// LineTo() the next point, as the user drags the
						// mouse.
	m_bCursor = true;
	if(pMainFrame)
		pMainFrame->SetCursor(	(m_xMax-m_xMin)*point.x/m_iViewWidth+m_xMin,
								(m_zMax-m_zMin)*point.y/m_iViewHeight+m_zMin);
}

void CDscopeCorrectedView::OnLButtonDown(UINT, CPoint point) 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	SetCapture();       // Capture the mouse until button up.
	SetPoint(point);
	return;
}

void CDscopeCorrectedView::OnLButtonUp(UINT, CPoint point) 
{
	if (GetCapture() != this)
		return; // If this window (view) didn't capture the mouse,
				// then the user isn't drawing in this window.

	CClientDC dc(this);
	OnPrepareDC(&dc);  // set up mapping mode and viewport origin
	dc.DPtoLP(&point);

	ReleaseCapture();   // Release the mouse capture established at
						// the beginning of the mouse drag.
	return;
}

void CDscopeCorrectedView::OnMouseMove(UINT, CPoint point) 
{
	// Mouse movement is interesting in the Scribble application
	// only if the user is currently drawing a new stroke by dragging
	// the captured mouse.

	if (GetCapture() != this)
		return; // If this window (view) didn't capture the mouse,
				// then the user isn't drawing in this window.

	CClientDC dc(this);
	// CScrollView changes the viewport origin and mapping mode.
	// It's necessary to convert the point from device coordinates
	// to logical coordinates, such as are stored in the document.
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	if((0<=point.x) && (point.x<m_iViewWidth) && (0<=point.y) && (point.y<m_iViewHeight))
		SetPoint(point);
	return;
}

void CDscopeCorrectedView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	m_bCursor = false;
}

void CDscopeCorrectedView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//OnButtonDblClk(0,nFlags,point);
	m_bCursor = false;
}

void CDscopeCorrectedView::OnButtonDblClk(int iCursorIndex,UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	// CScrollView changes the viewport origin and mapping mode.
	// It's necessary to convert the point from device coordinates
	// to logical coordinates, such as are stored in the document.
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	SetPoint(point);
}
