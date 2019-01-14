// CscopeView.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "DynamicCscopeView.h"
#include <math.h>
#include "ExternalDefinition.h"

// CDynamicCscopeView

IMPLEMENT_DYNCREATE(CDynamicCscopeView, CView)

CDynamicCscopeView::CDynamicCscopeView()
{
	g_pDynamicCscope = this;

	m_bOnSizing = false;
	m_bSetMapping = true;
	m_bResetConfiguration = true;

	m_iViewHeight = 100;
	m_iViewWidth = 100;
	m_iLastHeight = 0;
	m_iLastWidth = 0;

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

CDynamicCscopeView::~CDynamicCscopeView()
{
	g_pDynamicCscope = NULL;

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

BEGIN_MESSAGE_MAP(CDynamicCscopeView, CView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CDynamicCscopeView drawing

void CDynamicCscopeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	pDoc->SetTitle(_T("Dynamic C-scan"));
}

// CDynamicCscopeView message handlers

int CDynamicCscopeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(TIMER_CSCOPE_VIEW, 33, NULL);

	return 0;
}

void CDynamicCscopeView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_CSCOPE_VIEW)
	{
		//if (m_uiEvent != 0)
		//{
			UpdateDisplayData();
			//m_uiEvent = 0;
		//}

		//integration if (m_bOnSizing == false)
			Display();
	}

	CView::OnTimer(nIDEvent);
}

extern bool g_bMechanicalDirectionForward;
void CDynamicCscopeView::OnSize(UINT nType, int cx, int cy)
{//The framework calls this member function after the window's size has changed.
	CView::OnSize(nType, cx, cy);

	m_iViewHeight = cy;
	m_iViewWidth = cx;
	m_iBitmapWidth = cx;
#ifdef NEW
	if(g_bMechanicalDirectionForward)
#endif NEW
}

void CDynamicCscopeView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CView::OnSizing(fwSide, pRect);
	m_bOnSizing = true;
}

void CDynamicCscopeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

void CDynamicCscopeView::UpdateDisplayData()
{
	ResetBuffer();
	CreateAngleTable();
	
	if (m_bSetMapping == true)
		SetMapping();

	if (g_uiProbe == 0)
		DisplayProbe1();
	else if (g_uiProbe == 1)
		DisplayProbe2();
	else if (g_uiProbe == 2)
	{
		DisplayProbe1();
		DisplayProbe2();
	}
}

void CDynamicCscopeView::CreateAngleTable()
{
	if (m_pCosTable[0])
		delete[] m_pCosTable[0];

	if (m_pSinTable[0])
		delete[] m_pSinTable[0];

	if (m_pCosTable[1])
		delete[] m_pCosTable[1];

	if (m_pSinTable[1])
		delete[] m_pSinTable[1];

	m_pCosTable[0] = new double[InfoProbe1.iCycleCount];
	m_pSinTable[0] = new double[InfoProbe1.iCycleCount];

	m_pCosTable[1] = new double[InfoProbe2.iCycleCount];
	m_pSinTable[1] = new double[InfoProbe2.iCycleCount];

	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe1.iCycleCount ; i++)
	{
		m_pCosTable[0][i] = cos(InfoProbe1.pdAngle[i]);
		m_pSinTable[0][i] = sin(InfoProbe1.pdAngle[i]);
	}

	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2.iCycleCount ; i++)
	{
		m_pCosTable[1][i] = cos(InfoProbe2.pdAngle[i]);
		m_pSinTable[1][i] = sin(InfoProbe2.pdAngle[i]);
	}
}

void CDynamicCscopeView::CreatePalette()
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

void CDynamicCscopeView::ResetBufferEx()
{
	m_bResetConfiguration = true;
}

void CDynamicCscopeView::ResetBuffer()
{
	if ((m_iLastHeight != m_iViewHeight) || 
		(m_iLastWidth != m_iViewWidth) || 
		(m_bResetConfiguration == true))
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

		m_iBitmapHeight = (int)min(m_iViewHeight, (int)GetEncoderPositionQuantity());

		m_pBuffer = new DWORD[m_iBitmapHeight * m_iBitmapWidth];
		m_pDataBuffer = new BYTE[m_iBitmapHeight * m_iBitmapWidth];

		memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
		memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));

		m_pPositionProbe_X[0] = new int[InfoProbe1.iCycleCount * InfoProbe1.PointQuantity];
		m_pPositionProbe_Z[0] = new double[InfoProbe1.iCycleCount * InfoProbe1.PointQuantity];

		m_pPositionProbe_X[1] = new int[InfoProbe2.iCycleCount * InfoProbe2.PointQuantity];
		m_pPositionProbe_Z[1] = new double[InfoProbe2.iCycleCount * InfoProbe2.PointQuantity];

		memset(m_pPositionProbe_X[0], -1, InfoProbe1.iCycleCount * InfoProbe1.PointQuantity * sizeof(int));
		memset(m_pPositionProbe_Z[0], -1, InfoProbe1.iCycleCount * InfoProbe1.PointQuantity * sizeof(double));

		memset(m_pPositionProbe_X[1], -1, InfoProbe2.iCycleCount * InfoProbe2.PointQuantity * sizeof(int));
		memset(m_pPositionProbe_Z[1], -1, InfoProbe2.iCycleCount * InfoProbe2.PointQuantity * sizeof(double));

		memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
		memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));

		m_iLastHeight = m_iViewHeight;
		m_iLastWidth = m_iViewWidth;

		m_bSetMapping = true;
		m_bResetConfiguration = false;
	}
}

void CDynamicCscopeView::SetMapping()
{
	double Step = GetPointStep();
	double D, Lx, Lh;

	double XMin = GetXAxisMin();
	double XMax = GetXAxisMax();

	if (g_uiProbe == 0)
	{
		for (unsigned int iCycle = 0 ; iCycle < (unsigned int)InfoProbe1.iCycleCount ; iCycle++)
		{
			for (unsigned int iPoint = 0 ; iPoint < (unsigned int)InfoProbe1.PointQuantity ; iPoint++)
			{
				D = iPoint * Step;
				Lx = InfoProbe1.pdX[iCycle] + D * m_pSinTable[0][iCycle];
				Lh = D * m_pCosTable[0][iCycle];

				// Management of the rebound
				if (Lh > g_dPieceThickness)
					Lh = 2 * g_dPieceThickness - Lh;

				m_pPositionProbe_X[0][iCycle * InfoProbe1.PointQuantity + iPoint] = (int)(m_iBitmapWidth * (Lx - XMin) / (XMax - XMin));
				m_pPositionProbe_Z[0][iCycle * InfoProbe1.PointQuantity + iPoint] = Lh;
			}
		}
	}
	else if (g_uiProbe == 1)
	{
		for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2.iCycleCount ; i++)
		{
			for (unsigned int j = 0 ; j < (unsigned int)InfoProbe2.PointQuantity ; j++)
			{
				D = j * Step;
				Lx = InfoProbe2.pdX[i] + D * m_pSinTable[1][i];
				Lh = D * m_pCosTable[1][i];

				// Management of the rebound
				if (Lh > g_dPieceThickness)
					Lh = 2 * g_dPieceThickness - Lh;

				m_pPositionProbe_X[1][i * InfoProbe2.PointQuantity + j] = (int)(m_iBitmapWidth * (Lx - XMin) / (XMax - XMin));
				m_pPositionProbe_Z[1][i * InfoProbe2.PointQuantity + j] = Lh;
			}
		}
	}
	else if (g_uiProbe == 2)
	{
		for (unsigned int i = 0 ; i < (unsigned int)InfoProbe1.iCycleCount ; i++)
		{
			for (unsigned int j = 0 ; j < (unsigned int)InfoProbe1.PointQuantity ; j++)
			{
				D = j * Step;
				Lx = InfoProbe1.pdX[i] + D * m_pSinTable[0][i];
				Lh = D * m_pCosTable[0][i];

				// Management of the rebound
				if (Lh > g_dPieceThickness)
					Lh = 2 * g_dPieceThickness - Lh;

				m_pPositionProbe_X[0][i * InfoProbe1.PointQuantity + j] = (int)(m_iBitmapWidth * (Lx - XMin) / (XMax - XMin));
				m_pPositionProbe_Z[0][i * InfoProbe1.PointQuantity + j] = Lh;
			}
		}

		for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2.iCycleCount ; i++)
		{
			for (unsigned int j = 0 ; j < (unsigned int)InfoProbe2.PointQuantity ; j++)
			{
				D = j * Step;
				Lx = InfoProbe2.pdX[i] + D * m_pSinTable[1][i];
				Lh = D * m_pCosTable[1][i];

				// Management of the rebound
				if (Lh > g_dPieceThickness)
					Lh = 2 * g_dPieceThickness - Lh;

				m_pPositionProbe_X[1][i * InfoProbe2.PointQuantity + j] = (int)(m_iBitmapWidth * (Lx - XMin) / (XMax - XMin));
				m_pPositionProbe_Z[1][i * InfoProbe2.PointQuantity + j] = Lh;
			}
		}
	}
}

extern int g_iAscanPreviousEncoder;//integration
extern int g_iAscanLastEncoderPosition;//integration
extern bool g_bMechanicalDirectionForward;//integration
extern int g_bMechanicalOutOfRange;//integration
int m_iOutOfRange=0;
void CDynamicCscopeView::ResetDisplayProbe1()//integration
{
	if(m_pDataBuffer)
		memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
	if(m_pBuffer)
		memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));
}
void CDynamicCscopeView::DisplayProbe1()
{
	double XMin = (int)(m_iBitmapWidth * (Analysis_XMIN - GetXAxisMin()) / (GetXAxisMax() - GetXAxisMin()));
	double XMax = (int)(m_iBitmapWidth * (Analysis_XMAX - GetXAxisMin()) / (GetXAxisMax() - GetXAxisMin()));
	double ZMin = Analysis_ZMIN;
	double ZMax = Analysis_ZMAX;
	bool bLock=false;//integration

	COLORREF BlankColor = RGB(255, 255, 255);

	if(m_iOutOfRange<g_bMechanicalOutOfRange)
	{
		m_iOutOfRange = g_bMechanicalOutOfRange;
		ResetDisplayProbe1();
		g_iAscanLastEncoderPosition = -1;
	}
	if(g_iAscanPreviousEncoder<0)
		return;
	if(g_iAscanLastEncoderPosition<0)
		g_iAscanLastEncoderPosition = g_iAscanPreviousEncoder;
	while(	(g_bMechanicalDirectionForward && (g_iAscanLastEncoderPosition<g_iAscanPreviousEncoder)) ||
			(!g_bMechanicalDirectionForward && (g_iAscanLastEncoderPosition>g_iAscanPreviousEncoder)))//integration
	{
		for (unsigned int iCycle = 0 ; iCycle < (unsigned int)InfoProbe1.iCycleCount ; iCycle++)
		{
			for (unsigned int iPoint = 0 ; iPoint < (unsigned int)InfoProbe1.PointQuantity ; iPoint++)
			{
				int TempPosition = iCycle * InfoProbe1.PointQuantity + iPoint;

				//integration
				//if ((m_pPositionProbe_Z[0][TempPosition] >= ZMin) &&
				//	(m_pPositionProbe_Z[0][TempPosition] <= ZMax) &&
				//	(m_pPositionProbe_X[0][TempPosition] >= XMin) &&
				//	(m_pPositionProbe_X[0][TempPosition] <= XMax))
				{
					int Position = g_iAscanLastEncoderPosition * m_iBitmapWidth + m_pPositionProbe_X[0][TempPosition];

					if ((Position >= 0) && 
						(Position < m_iBitmapWidth * m_iBitmapHeight))
					{
						if(!bLock)
						{
							bLock=true;//integration
							EnterCriticalSection(&CRT_MEMORY_1);
						}
						m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanMemory1[g_iAscanLastEncoderPosition][TempPosition]) * 2 - 1);
						//integration LeaveCriticalSection(&CRT_MEMORY_1);

						m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
					}
				}
			}
		}
		if(g_bMechanicalDirectionForward)
			g_iAscanLastEncoderPosition++;
		else
			g_iAscanLastEncoderPosition--;
	}
	if(bLock)//integration
		LeaveCriticalSection(&CRT_MEMORY_1);
}

void CDynamicCscopeView::DisplayProbe2()
{
	double XMin = (int)(m_iBitmapWidth * (Analysis_XMIN - GetXAxisMin()) / (GetXAxisMax() - GetXAxisMin()));
	double XMax = (int)(m_iBitmapWidth * (Analysis_XMAX - GetXAxisMin()) / (GetXAxisMax() - GetXAxisMin()));
	double ZMin = Analysis_ZMIN;
	double ZMax = Analysis_ZMAX;
	COLORREF BlankColor = RGB(255, 255, 255);

	for (unsigned int iCycle = 0 ; iCycle < (unsigned int)InfoProbe2.iCycleCount ; iCycle++)
	{
		for (unsigned int iPoint = 0 ; iPoint < (unsigned int)InfoProbe2.PointQuantity ; iPoint++)
		{
			int TempPosition = iCycle * InfoProbe2.PointQuantity + iPoint;

			if ((m_pPositionProbe_Z[1][TempPosition] >= ZMin) &&
				(m_pPositionProbe_Z[1][TempPosition] <= ZMax) &&
				(m_pPositionProbe_X[1][TempPosition] >= XMin) &&
				(m_pPositionProbe_X[1][TempPosition] <= XMax))
			{
				int Position = g_iAscanLastEncoderPosition * m_iBitmapWidth + m_pPositionProbe_X[1][TempPosition];

				if ((Position >= 0) && 
					(Position < m_iBitmapWidth * m_iBitmapHeight))
				{
					EnterCriticalSection(&CRT_MEMORY_2);
					m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanMemory2[g_iAscanLastEncoderPosition][TempPosition]) * 2 - 1);
					LeaveCriticalSection(&CRT_MEMORY_2);
					m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
				}
			}
		}
	}
}

void CDynamicCscopeView::Display()
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

void CDynamicCscopeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);
}