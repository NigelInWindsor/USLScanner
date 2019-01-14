// DscopeView.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "DscopeView.h"
#include <math.h>
#include "ExternalDefinition.h"

// CDscopeView

IMPLEMENT_DYNCREATE(CDscopeView, CView)

CDscopeView::CDscopeView()
{
	g_pDscope = this;

	m_bOnSizing = false;
	m_uiEvent = 5;

	m_iViewHeight = 100;
	m_iViewWidth = 100;
	m_pBuffer= NULL;
	m_pDataBuffer = NULL;

	m_pCosTable[0] = NULL;
	m_pSinTable[0] = NULL;
	m_pCosTable[1] = NULL;
	m_pSinTable[1] = NULL;

	m_pPositionProbe_X[0] = NULL;
	m_pPositionProbe_X[1] = NULL;
	m_pPositionProbe_Z[0] = NULL;
	m_pPositionProbe_Z[1] = NULL;
	
	CreatePalette();
}

CDscopeView::~CDscopeView()
{
	g_pDscope = NULL;

	if (m_pBuffer)
		delete[] m_pBuffer;

	if (m_pDataBuffer)
		delete[] m_pDataBuffer;

	if (m_pCosTable[0])
		delete[] m_pCosTable[0];

	if (m_pSinTable[0])
		delete[] m_pSinTable[0];

	if (m_pCosTable[1])
		delete[] m_pCosTable[1];

	if (m_pSinTable[1])
		delete[] m_pSinTable[1];

	if (m_pPositionProbe_X[0])
		delete[] m_pPositionProbe_X[0];

	if (m_pPositionProbe_X[1])
		delete[] m_pPositionProbe_X[1];

	if (m_pPositionProbe_Z[0])
		delete[] m_pPositionProbe_Z[0];

	if (m_pPositionProbe_Z[1])
		delete[] m_pPositionProbe_Z[1];
}

BEGIN_MESSAGE_MAP(CDscopeView, CView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CDscopeView drawing

void CDscopeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	pDoc->SetTitle(_T("D-scan Analysis"));
}

// CDscopeView message handlers

int CDscopeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(TIMER_DSCOPE_VIEW, 33, NULL);

	return 0;
}

void CDscopeView::OnTimer(UINT_PTR nIDEvent)
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

void CDscopeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_iViewHeight = cy;
	m_iViewWidth = cx;
	m_iBitmapHeight = cy;
}

void CDscopeView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CView::OnSizing(fwSide, pRect);
	m_bOnSizing = true;
}

void CDscopeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

void CDscopeView::UpdateDisplayData()
{
	if (g_uiMode == 1)
	{
		ResetBuffer();
		CreateAngleTable();
		SetMapping();

		if (g_uiProbeAnalysis == 0)
			DisplayProbe1Gate();
		else if (g_uiProbeAnalysis == 1)
			DisplayProbe2Gate();
		else if (g_uiProbeAnalysis == 2)
		{
			DisplayProbe1Gate();
			DisplayProbe2Gate();
		}
	}
}

void CDscopeView::SetEvent(UINT Event)
{
	m_uiEvent = Event;

	if (m_uiEvent == 1)
		m_bOnSizing = false;
}

void CDscopeView::CreateAngleTable()
{
	if (m_pCosTable[0])
		delete[] m_pCosTable[0];

	if (m_pSinTable[0])
		delete[] m_pSinTable[0];

	if (m_pCosTable[1])
		delete[] m_pCosTable[1];

	if (m_pSinTable[1])
		delete[] m_pSinTable[1];

	m_pCosTable[0] = new double[InfoProbe1Analysis.iCycleCount];
	m_pSinTable[0] = new double[InfoProbe1Analysis.iCycleCount];

	m_pCosTable[1] = new double[InfoProbe2Analysis.iCycleCount];
	m_pSinTable[1] = new double[InfoProbe2Analysis.iCycleCount];

	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe1Analysis.iCycleCount ; i++)
	{
		m_pCosTable[0][i] = cos(InfoProbe1Analysis.pdAngle[i]);
		m_pSinTable[0][i] = sin(InfoProbe1Analysis.pdAngle[i]);
	}

	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2Analysis.iCycleCount ; i++)
	{
		m_pCosTable[1][i] = cos(InfoProbe2Analysis.pdAngle[i]);
		m_pSinTable[1][i] = sin(InfoProbe2Analysis.pdAngle[i]);
	}
}

void CDscopeView::CreatePalette()
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

void CDscopeView::ResetBuffer()
{
	if (m_pBuffer)
	{
		delete[] m_pBuffer;
		delete[] m_pDataBuffer;

		delete[] m_pPositionProbe_X[0];
		delete[] m_pPositionProbe_Z[0];
		delete[] m_pPositionProbe_X[1];
		delete[] m_pPositionProbe_Z[1];
	}

	m_iBitmapWidth = (int)min(m_iViewWidth, (int)GetAnalysisEncoderPositionQuantity());

	m_pBuffer = new DWORD[m_iBitmapHeight * m_iBitmapWidth];
	m_pDataBuffer = new BYTE[m_iBitmapHeight * m_iBitmapWidth];

	memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
	memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));

	m_pPositionProbe_X[0] = new double[InfoProbe1Analysis.iCycleCount * InfoProbe1Analysis.PointQuantity];
	m_pPositionProbe_Z[0] = new int[InfoProbe1Analysis.iCycleCount * InfoProbe1Analysis.PointQuantity];

	m_pPositionProbe_X[1] = new double[InfoProbe2Analysis.iCycleCount * InfoProbe2Analysis.PointQuantity];
	m_pPositionProbe_Z[1] = new int[InfoProbe2Analysis.iCycleCount * InfoProbe2Analysis.PointQuantity];

	memset(m_pPositionProbe_X[0], -1, InfoProbe1Analysis.iCycleCount * InfoProbe1Analysis.PointQuantity * sizeof(double));
	memset(m_pPositionProbe_Z[0], -1, InfoProbe1Analysis.iCycleCount * InfoProbe1Analysis.PointQuantity * sizeof(int));

	memset(m_pPositionProbe_X[1], -1, InfoProbe2Analysis.iCycleCount * InfoProbe2Analysis.PointQuantity * sizeof(double));
	memset(m_pPositionProbe_Z[1], -1, InfoProbe2Analysis.iCycleCount * InfoProbe2Analysis.PointQuantity * sizeof(int));
}

void CDscopeView::SetMapping()
{
	double Step = GetPointStepAnalysis();
	double D, Lx, Lh;

	double ZMin = GetAnalysisZAxisMin();
	double ZMax = GetAnalysisZAxisMax();

	if (g_uiProbeAnalysis == 0)
	{
		for (unsigned int i = 0 ; i < (unsigned int)InfoProbe1Analysis.iCycleCount ; i++)
		{
			for (unsigned int j = 0 ; j < (unsigned int)InfoProbe1Analysis.PointQuantity ; j++)
			{
				D = j * Step;
				Lx = InfoProbe1Analysis.pdX[i] + D * m_pSinTable[0][i];
				Lh = D * m_pCosTable[0][i];

				// Management of the rebound
				if (g_bBackwallRebound && (Lh > g_dPieceThicknessAnalysis))
					Lh = 2 * g_dPieceThicknessAnalysis - Lh;

				m_pPositionProbe_X[0][i * InfoProbe1Analysis.PointQuantity + j] = Lx;
				m_pPositionProbe_Z[0][i * InfoProbe1Analysis.PointQuantity + j] = (int)(m_iBitmapHeight * (Lh - ZMin) / (ZMax - ZMin));
			}
		}
	}
	else if (g_uiProbeAnalysis == 1)
	{
		for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2Analysis.iCycleCount ; i++)
		{
			for (unsigned int j = 0 ; j < (unsigned int)InfoProbe2Analysis.PointQuantity ; j++)
			{
				D = j * Step;
				Lx = InfoProbe2Analysis.pdX[i] + D * m_pSinTable[1][i];
				Lh = D * m_pCosTable[1][i];

				// Management of the rebound
				if (g_bBackwallRebound && (Lh > g_dPieceThicknessAnalysis))
					Lh = 2 * g_dPieceThicknessAnalysis - Lh;

				m_pPositionProbe_X[1][i * InfoProbe2Analysis.PointQuantity + j] = Lx;
				m_pPositionProbe_Z[1][i * InfoProbe2Analysis.PointQuantity + j] = (int)(m_iBitmapHeight * (Lh - ZMin) / (ZMax - ZMin));
			}
		}
	}
	else if (g_uiProbeAnalysis == 2)
	{
		for (unsigned int i = 0 ; i < (unsigned int)InfoProbe1Analysis.iCycleCount ; i++)
		{
			for (unsigned int j = 0 ; j < (unsigned int)InfoProbe1Analysis.PointQuantity ; j++)
			{
				D = j * Step;
				Lx = InfoProbe1Analysis.pdX[i] + D * m_pSinTable[0][i];
				Lh = D * m_pCosTable[0][i];

				// Management of the rebound
				if (g_bBackwallRebound && (Lh > g_dPieceThicknessAnalysis))
					Lh = 2 * g_dPieceThicknessAnalysis - Lh;

				m_pPositionProbe_X[0][i * InfoProbe1Analysis.PointQuantity + j] = Lx;
				m_pPositionProbe_Z[0][i * InfoProbe1Analysis.PointQuantity + j] = (int)(m_iBitmapHeight * (Lh - ZMin) / (ZMax - ZMin));
			}
		}

		for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2Analysis.iCycleCount ; i++)
		{
			for (unsigned int j = 0 ; j < (unsigned int)InfoProbe2Analysis.PointQuantity ; j++)
			{
				D = j * Step;
				Lx = InfoProbe2Analysis.pdX[i] + D * m_pSinTable[1][i];
				Lh = D * m_pCosTable[1][i];

				// Management of the rebound
				if (g_bBackwallRebound && (Lh > g_dPieceThicknessAnalysis))
					Lh = 2 * g_dPieceThicknessAnalysis - Lh;

				m_pPositionProbe_X[1][i * InfoProbe2Analysis.PointQuantity + j] = Lx;
				m_pPositionProbe_Z[1][i * InfoProbe2Analysis.PointQuantity + j] = (int)(m_iBitmapHeight * (Lh - ZMin) / (ZMax - ZMin));
			}
		}
	}
}

void CDscopeView::DisplayProbe1Gate()
{
	double ZMin = GetAnalysisZAxisMin();
	double ZMax = GetAnalysisZAxisMax();

	unsigned int MinimumY_Index = (unsigned int)(Analysis_YMIN / SystemAnalysis.dEncoderStep);
	unsigned int MaximumY_Index = (unsigned int)min(Analysis_YMAX / SystemAnalysis.dEncoderStep, GetAnalysisEncoderPositionQuantity() - 1);

	int IndexZMin = (int)(m_iBitmapHeight * (Analysis_ZMIN - ZMin) / (ZMax - ZMin));
	int IndexZMax = (int)(m_iBitmapHeight * (Analysis_ZMAX - ZMin) / (ZMax - ZMin));

	int IndexYMin = (int)(m_iBitmapWidth * (MinimumY_Index) / GetAnalysisEncoderPositionQuantity());
	int IndexYMax = (int)(m_iBitmapWidth * (MaximumY_Index) / GetAnalysisEncoderPositionQuantity());

	COLORREF BlankColor = RGB(255, 255, 255);

	EnterCriticalSection(&CRT_ANALYSIS_MEMORY_1);

	for (unsigned int h = (unsigned int)IndexYMin ; h <= (unsigned int)IndexYMax ; h++)//bug ? IndexYMin instead of MinimumY_Index ?
	{
		for (unsigned int i = 0 ; i < (unsigned int)InfoProbe1Analysis.iCycleCount ; i++)
		{
			for (unsigned int j = 0 ; j < (unsigned int)InfoProbe1Analysis.PointQuantity ; j++)
			{
				int TempPosition = i * InfoProbe1Analysis.PointQuantity + j;

				if ((m_pPositionProbe_X[0][TempPosition] >= Analysis_XMIN) &&
					(m_pPositionProbe_X[0][TempPosition] <= Analysis_XMAX) &&
					(m_pPositionProbe_Z[0][TempPosition] >= IndexZMin) &&
					(m_pPositionProbe_Z[0][TempPosition] <= IndexZMax))
				{
					int Position = m_pPositionProbe_Z[0][TempPosition] * m_iBitmapWidth + h;

					if (m_pPositionProbe_Z[0][TempPosition] < m_iBitmapHeight)
					{
						m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanAnalysisMemory1[h][TempPosition]) * 2 - 1);
						m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
					}	
				}
			}
		}
	}

	LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_1);
}

void CDscopeView::DisplayProbe2Gate()
{
	double ZMin = GetAnalysisZAxisMin();
	double ZMax = GetAnalysisZAxisMax();

	unsigned int MinimumY_Index = (unsigned int)(Analysis_YMIN / SystemAnalysis.dEncoderStep);
	unsigned int MaximumY_Index = (unsigned int)min(Analysis_YMAX / SystemAnalysis.dEncoderStep, GetAnalysisEncoderPositionQuantity() - 1);

	int IndexZMin = (int)(m_iBitmapHeight * (Analysis_ZMIN - ZMin) / (ZMax - ZMin));
	int IndexZMax = (int)(m_iBitmapHeight * (Analysis_ZMAX - ZMin) / (ZMax - ZMin));

	int IndexYMin = (int)(m_iBitmapWidth * (MinimumY_Index) / GetAnalysisEncoderPositionQuantity());
	int IndexYMax = (int)(m_iBitmapWidth * (MaximumY_Index) / GetAnalysisEncoderPositionQuantity());

	COLORREF BlankColor = RGB(255, 255, 255);

	EnterCriticalSection(&CRT_ANALYSIS_MEMORY_2);

	for (unsigned int h = (unsigned int)IndexYMin ; h <= (unsigned int)IndexYMax ; h++)
	{
		for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2Analysis.iCycleCount ; i++)
		{
			for (unsigned int j = 0 ; j < (unsigned int)InfoProbe2Analysis.PointQuantity ; j++)
			{
				int TempPosition = i * InfoProbe2Analysis.PointQuantity + j;

				if ((m_pPositionProbe_X[1][TempPosition] >= Analysis_XMIN) &&
					(m_pPositionProbe_X[1][TempPosition] <= Analysis_XMAX) &&
					(m_pPositionProbe_Z[1][TempPosition] >= IndexZMin) &&
					(m_pPositionProbe_Z[1][TempPosition] <= IndexZMax))
				{
					int Position = m_pPositionProbe_Z[1][TempPosition] * m_iBitmapWidth + h;

					if (m_pPositionProbe_Z[1][TempPosition] < m_iBitmapHeight)
					{
						m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanAnalysisMemory2[h][TempPosition]) * 2 - 1);
						m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
					}	
				}
			}
		}
	}

	LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_2);
}

void CDscopeView::Display()
{
	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);

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
}

void CDscopeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);
}