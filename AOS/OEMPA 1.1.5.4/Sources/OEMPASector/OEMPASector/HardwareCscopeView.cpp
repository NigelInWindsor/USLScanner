// CscopeView.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "HardwareCscopeView.h"
#include <math.h>
#include "ExternalDefinition.h"

extern UINT g_uiUTEventMessage;//integration
extern int g_iGateId;
int g_iGateTimerId=-1;
bool g_bHWCriticalSection=false;
CRITICAL_SECTION g_HWCriticalSection;

// CHardwareCscopeView

IMPLEMENT_DYNCREATE(CHardwareCscopeView, CView)

CHardwareCscopeView::CHardwareCscopeView()
{
	if(!g_bHWCriticalSection)
		InitializeCriticalSection(&g_HWCriticalSection);
	m_iGateId = g_iGateId;
	m_iGateId4 = m_iGateId%4;
	g_pHardwareCscope[m_iGateId] = this;

	m_bOnSizing = false;
	m_bSetMapping = true;
	m_bResetConfiguration = true;

	m_iViewHeight = 100;
	m_iViewWidth = 100;
	m_iLastHeight = 0;
	m_iLastWidth = 0;

	m_pBuffer= NULL;
	m_pDataBuffer = NULL;

	//m_pCosTable[0] = NULL;
	//m_pSinTable[0] = NULL;
	//m_pCosTable[1] = NULL;
	//m_pSinTable[1] = NULL;

	//m_pPositionProbe_X[0] = NULL;
	//m_pPositionProbe_X[1] = NULL;
	//m_pPositionProbe_Y[0] = NULL;
	//m_pPositionProbe_Y[1] = NULL;

	CreatePalette();
}

CHardwareCscopeView::~CHardwareCscopeView()
{
	g_pHardwareCscope[m_iGateId] = NULL;

	if (m_pBuffer)
		delete[] m_pBuffer;

	if (m_pDataBuffer)
		delete[] m_pDataBuffer;

	//if (m_pCosTable[0])
	//	delete[] m_pCosTable[0];

	//if (m_pSinTable[0])
	//	delete[] m_pSinTable[0];

	//if (m_pCosTable[1])
	//	delete[] m_pCosTable[1];

	//if (m_pSinTable[1])
	//	delete[] m_pSinTable[1];

	//if (m_pPositionProbe_X[0])
	//	delete[] m_pPositionProbe_X[0];

	//if (m_pPositionProbe_X[1])
	//	delete[] m_pPositionProbe_X[1];

	//if (m_pPositionProbe_Y[0])
	//	delete[] m_pPositionProbe_Y[0];

	//if (m_pPositionProbe_Y[1])
	//	delete[] m_pPositionProbe_Y[1];
	EnterCriticalSection(&g_HWCriticalSection);
	if(g_iGateTimerId == m_iGateId)
	{
		g_iGateTimerId = -1;
		for(int iGate=0;iGate<4+4;iGate++)
		{
			if(g_pHardwareCscope[iGate])
				g_pHardwareCscope[iGate]->PostMessage(g_uiUTEventMessage,0,0);
		}
	}
	LeaveCriticalSection(&g_HWCriticalSection);
}

BEGIN_MESSAGE_MAP(CHardwareCscopeView, CView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
	ON_REGISTERED_MESSAGE(g_uiUTEventMessage, &CHardwareCscopeView::OnEventMessage)//integration
END_MESSAGE_MAP()

// CHardwareCscopeView drawing

void CHardwareCscopeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	pDoc->SetTitle(_T("Dynamic C-scan"));
}

// CHardwareCscopeView message handlers

int CHardwareCscopeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//SetTimer(TIMER_CSCOPE_VIEW, 33, NULL);
	EnterCriticalSection(&g_HWCriticalSection);
	if(g_iGateTimerId<0)
	{
		for(int iGate=0;iGate<4+4;iGate++)
		{
			if(g_pHardwareCscope[iGate])
				g_pHardwareCscope[iGate]->PostMessage(g_uiUTEventMessage,0,0);
		}
	}
	LeaveCriticalSection(&g_HWCriticalSection);

	return 0;
}

LRESULT CHardwareCscopeView::OnEventMessage(WPARAM wParam,LPARAM lParam)//integration
{
	EnterCriticalSection(&g_HWCriticalSection);
	if(g_iGateTimerId<0)
	{
		g_iGateTimerId = m_iGateId;
		SetTimer(TIMER_CSCOPE_VIEW, 33, NULL);
	}
	LeaveCriticalSection(&g_HWCriticalSection);
	return 0;
}

extern int m_iOutOfRangeHW;
extern int g_bMechanicalOutOfRange;
void CHardwareCscopeView::OnTimer(UINT_PTR nIDEvent)
{
	bool bReset=false;

	if (nIDEvent == TIMER_CSCOPE_VIEW)
	{
		EnterCriticalSection(&CRT_MEMORY_1);
		if(m_iOutOfRangeHW<g_bMechanicalOutOfRange)
		{
			m_iOutOfRangeHW = g_bMechanicalOutOfRange;
			//ResetDisplayProbe1();
			//g_iCscanLastEncoderPosition[m_iGateId] = -1;
			bReset = true;
		}
		for(int iGate=0;iGate<4+4;iGate++)
		{
			if(g_pHardwareCscope[iGate])
				g_pHardwareCscope[iGate]->UpdateDisplayData(bReset);
		}
		LeaveCriticalSection(&CRT_MEMORY_1);
		for(int iGate=0;iGate<4+4;iGate++)
		{
			if(g_pHardwareCscope[iGate])
				g_pHardwareCscope[iGate]->Display();
		}
	}
	CView::OnTimer(nIDEvent);
}

void CHardwareCscopeView::Timer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_CSCOPE_VIEW)
	{
		//if (m_uiEvent != 0)
		//{
			UpdateDisplayData(false);
			//m_uiEvent = 0;
		//}

		//integration if (m_bOnSizing == false)
			Display();
	}
}

void CHardwareCscopeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_iViewHeight = cy;
	m_iViewWidth = cx;
	m_iBitmapWidth = cx;
}

void CHardwareCscopeView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CView::OnSizing(fwSide, pRect);
	m_bOnSizing = true;
}

void CHardwareCscopeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

void CHardwareCscopeView::UpdateDisplayData(bool bReset)
{
	ResetBuffer();
	//CreateAngleTable();
	
	if (m_bSetMapping == true)
		SetMapping();

	if (g_uiProbe == 0)
		DisplayProbe1(bReset);
	else if (g_uiProbe == 1)
		DisplayProbe2(bReset);
	else if (g_uiProbe == 2)
	{
		DisplayProbe1(bReset);
		DisplayProbe2(bReset);
	}
}

//void CHardwareCscopeView::CreateAngleTable()
//{
//	if (m_pCosTable[0])
//		delete[] m_pCosTable[0];
//
//	if (m_pSinTable[0])
//		delete[] m_pSinTable[0];
//
//	if (m_pCosTable[1])
//		delete[] m_pCosTable[1];
//
//	if (m_pSinTable[1])
//		delete[] m_pSinTable[1];
//
//	m_pCosTable[0] = new double[InfoProbe1.iCycleCount];
//	m_pSinTable[0] = new double[InfoProbe1.iCycleCount];
//
//	m_pCosTable[1] = new double[InfoProbe2.iCycleCount];
//	m_pSinTable[1] = new double[InfoProbe2.iCycleCount];
//
//	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe1.iCycleCount ; i++)
//	{
//		m_pCosTable[0][i] = cos(InfoProbe1.pdAngle[i]);
//		m_pSinTable[0][i] = sin(InfoProbe1.pdAngle[i]);
//	}
//
//	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2.iCycleCount ; i++)
//	{
//		m_pCosTable[1][i] = cos(InfoProbe2.pdAngle[i]);
//		m_pSinTable[1][i] = sin(InfoProbe2.pdAngle[i]);
//	}
//}

void CHardwareCscopeView::CreatePalette()
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

void CHardwareCscopeView::ResetBufferEx()
{
	m_bResetConfiguration = true;
}

extern double g_dEncoderAxisLength;
long iround(double x)
{
	int nx = (int)x;
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

void CHardwareCscopeView::ResetBuffer()
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
			delete[] m_pPositionProbe_Y[0];
			delete[] m_pPositionProbe_X[1];
			delete[] m_pPositionProbe_Y[1];
		}

		m_iBitmapHeight = (int)min(m_iViewHeight, (int)GetEncoderPositionQuantity());
		m_iBitmapWidth = (int)min(m_iViewWidth, (int)InfoProbe1.iCycleCount);//integration

		m_pBuffer = new DWORD[m_iBitmapHeight * m_iBitmapWidth];
		m_pDataBuffer = new BYTE[m_iBitmapHeight * m_iBitmapWidth];

		memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
		memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));

		m_iAcquisitionLength = iround(g_dEncoderAxisLength/System.dEncoderStep);
		m_pPositionProbe_X[0] = new int[InfoProbe1.iCycleCount];
		m_pPositionProbe_Y[0] = new int[m_iAcquisitionLength];

		m_pPositionProbe_X[1] = new int[InfoProbe2.iCycleCount];
		m_pPositionProbe_Y[1] = new int[m_iAcquisitionLength];

		memset(m_pPositionProbe_X[0], -1, InfoProbe1.iCycleCount * sizeof(int));
		memset(m_pPositionProbe_Y[0], -1, m_iAcquisitionLength * sizeof(int));

		memset(m_pPositionProbe_X[1], -1, InfoProbe2.iCycleCount * sizeof(int));
		memset(m_pPositionProbe_Y[1], -1, m_iAcquisitionLength * sizeof(int));

		memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
		memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));

		m_iLastHeight = m_iViewHeight;
		m_iLastWidth = m_iViewWidth;

		m_bSetMapping = true;
		m_bResetConfiguration = false;
	}
}

void CHardwareCscopeView::SetMapping()
{
	double Step = GetPointStep();
	//double D, Lx, Lh;

	double XMin = GetXAxisMin();
	double XMax = GetXAxisMax();

	if (g_uiProbe == 0)
	{
		for (unsigned int iCycle = 0 ; iCycle < (unsigned int)InfoProbe1.iCycleCount ; iCycle++)
		{
			//m_pPositionProbe_X[0][iCycle] = (int)(m_iBitmapWidth * iCycle / InfoProbe1.iCycleCount);
			m_pPositionProbe_X[0][iCycle] = (int)(iCycle);//before 1:1:3:0
		}
		for (unsigned int iPoint = 0 ; iPoint < (unsigned int)m_iAcquisitionLength ; iPoint++)
		{
			m_pPositionProbe_Y[0][iPoint] = (int)(m_iBitmapHeight * iPoint / (m_iAcquisitionLength-1));//before 1:1:3:0
		}
	}
	else if (g_uiProbe == 1)
	{
		for (unsigned int iCycle = 0 ; iCycle < (unsigned int)InfoProbe1.iCycleCount ; iCycle++)
		{
			//m_pPositionProbe_X[1][iCycle] = (int)(m_iBitmapWidth * iCycle / InfoProbe1.iCycleCount);
			m_pPositionProbe_X[1][iCycle] = (int)(iCycle);
		}
		for (unsigned int iPoint = 0 ; iPoint < (unsigned int)m_iAcquisitionLength ; iPoint++)
		{
			m_pPositionProbe_Y[1][iPoint] = (int)(m_iBitmapHeight * iPoint / (m_iAcquisitionLength-1));
		}
	}
	else if (g_uiProbe == 2)
	{
		for (unsigned int iCycle = 0 ; iCycle < (unsigned int)InfoProbe1.iCycleCount ; iCycle++)
		{
			//m_pPositionProbe_X[0][iCycle] = (int)(m_iBitmapWidth * iCycle / InfoProbe1.iCycleCount);
			m_pPositionProbe_X[0][iCycle] = (int)(iCycle);
		}
		for (unsigned int iPoint = 0 ; iPoint < (unsigned int)m_iAcquisitionLength ; iPoint++)
		{
			m_pPositionProbe_Y[0][iPoint] = (int)(m_iBitmapHeight * iPoint / (m_iAcquisitionLength-1));
		}
		for (unsigned int iCycle = 0 ; iCycle < (unsigned int)InfoProbe1.iCycleCount ; iCycle++)
		{
			//m_pPositionProbe_X[1][iCycle] = (int)(m_iBitmapWidth * iCycle / InfoProbe1.iCycleCount);
			m_pPositionProbe_X[1][iCycle] = (int)(iCycle);
		}
		for (unsigned int iPoint = 0 ; iPoint < (unsigned int)m_iAcquisitionLength ; iPoint++)
		{
			m_pPositionProbe_Y[1][iPoint] = (int)(m_iBitmapHeight * iPoint / (m_iAcquisitionLength-1));
		}
	}
}

//extern int g_iCscanPreviousEncoder;//integration
extern int g_iCscanPreviousEncoder;
extern int g_iCscanLastEncoderPosition[4+4];//integration
extern int g_iCscanInspectionCount;//integration
extern int g_aiCscanInspectionCount[4+4];//integration
extern bool g_bMechanicalDirectionForward;//integration
extern int g_bMechanicalOutOfRange;//integration
int m_iOutOfRangeHW=0;
void CHardwareCscopeView::ResetDisplayProbe1()//integration
{
	if(m_pDataBuffer)
		memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
	if(m_pBuffer)
		memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));
}
bool CHardwareCscopeView::IsProcessRequired()
{
	if(System.bInternal)
	{
		if(g_aiCscanInspectionCount[m_iGateId]<g_iCscanInspectionCount)
			return true;
		if(g_iCscanLastEncoderPosition[m_iGateId]<g_iCscanPreviousEncoder)
			return true;
		else
			return false;
	}else{
		if(	(g_bMechanicalDirectionForward && (g_iCscanLastEncoderPosition[m_iGateId]<g_iCscanPreviousEncoder)) ||
				(!g_bMechanicalDirectionForward && (g_iCscanLastEncoderPosition[m_iGateId]>g_iCscanPreviousEncoder)))//integration
			return true;
		else
			return false;
	}
}

void CHardwareCscopeView::DisplayProbe1(bool bReset)
{
	//double XMin = (int)(m_iBitmapWidth * (Analysis_XMIN - GetXAxisMin()) / (GetXAxisMax() - GetXAxisMin()));
	//double XMax = (int)(m_iBitmapWidth * (Analysis_XMAX - GetXAxisMin()) / (GetXAxisMax() - GetXAxisMin()));
	//double ZMin = Analysis_ZMIN;
	//double ZMax = Analysis_ZMAX;
	bool bLock=false;//integration
	BYTE byData;
	double dData;

	COLORREF BlankColor = RGB(255, 255, 255);

	//if(m_iOutOfRangeHW<g_bMechanicalOutOfRange)
	//{
	//	m_iOutOfRangeHW = g_bMechanicalOutOfRange;
	//	ResetDisplayProbe1();
	//	g_iCscanLastEncoderPosition[m_iGateId] = -1;
	//}
	if(bReset)
	{
		ResetDisplayProbe1();
		g_iCscanLastEncoderPosition[m_iGateId] = -1;
	}
	if(g_iCscanPreviousEncoder<0)
		return;
	if(g_iCscanLastEncoderPosition[m_iGateId]<0)
		g_iCscanLastEncoderPosition[m_iGateId] = g_iCscanPreviousEncoder;
	while(IsProcessRequired())
	{
		if((0<=g_iCscanLastEncoderPosition[m_iGateId]) && (g_iCscanLastEncoderPosition[m_iGateId]<m_iAcquisitionLength))
		for (unsigned int iCycle = 0 ; iCycle < (unsigned int)InfoProbe1.iCycleCount ; iCycle++)
		{
			int Position = m_pPositionProbe_Y[0][g_iCscanLastEncoderPosition[m_iGateId]] * InfoProbe1.iCycleCount + /*m_pPositionProbe_X[0][*/iCycle/*]*/;//before 1:1:3:0

			if ((Position >= 0) && 
				(Position < InfoProbe1.iCycleCount * m_iBitmapHeight))
			{
				if(m_iGateId<4)
				{
					byData = (BYTE)(abs(CscanAmpMemory1[m_iGateId4][g_iCscanLastEncoderPosition[m_iGateId]*InfoProbe1.iCycleCount+iCycle]));// * 2 - 1);
				}else{
					dData = (double)(short)(abs(CscanTofMemory1[m_iGateId4][g_iCscanLastEncoderPosition[m_iGateId]*InfoProbe1.iCycleCount+iCycle]));
					//if(dData>System.dTofMax)
					//	dData = System.dTofMax;
					//else if(dData<0)
					//	dData = 0;
					//byData = (BYTE)(dData*255/System.dTofMax);
					dData -= System.dTofStart[m_iGateId4];
					if(dData<0)
						dData = 0;
					if(dData>System.dTofRange[m_iGateId4])
						dData = System.dTofRange[m_iGateId4];
					byData = (BYTE)(dData*255/System.dTofRange[m_iGateId4]);
				}
				m_pDataBuffer[Position] = byData;
				m_pBuffer[Position] = m_cPalette[byData];
			}
		}
		if(System.bInternal)
		{
			g_iCscanLastEncoderPosition[m_iGateId]++;
			if(g_iCscanLastEncoderPosition[m_iGateId]>=m_iAcquisitionLength)
			{
				g_iCscanLastEncoderPosition[m_iGateId] = 0;
				g_aiCscanInspectionCount[m_iGateId] = g_iCscanInspectionCount;
			}
		}else{
			if(g_bMechanicalDirectionForward)
				g_iCscanLastEncoderPosition[m_iGateId]++;
			else
				g_iCscanLastEncoderPosition[m_iGateId]--;
		}
	}
}

void CHardwareCscopeView::DisplayProbe2(bool bReset)
{
	//double XMin = (int)(m_iBitmapWidth * (Analysis_XMIN - GetXAxisMin()) / (GetXAxisMax() - GetXAxisMin()));
	//double XMax = (int)(m_iBitmapWidth * (Analysis_XMAX - GetXAxisMin()) / (GetXAxisMax() - GetXAxisMin()));
	//double ZMin = Analysis_ZMIN;
	//double ZMax = Analysis_ZMAX;
	COLORREF BlankColor = RGB(255, 255, 255);

	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2.iCycleCount ; i++)
	{
#ifdef NEW
		int Position = g_iCscanLastEncoderPosition[m_iGateId4] * m_iBitmapWidth + m_pPositionProbe_X[1][TempPosition];

		if ((Position >= 0) && 
			(Position < m_iBitmapWidth * m_iBitmapHeight))
		{
			EnterCriticalSection(&CRT_MEMORY_2);
			m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanMemory2[g_iCscanLastEncoderPosition[m_iGateId4]][i/*TempPosition*/]) * 2 - 1);
			LeaveCriticalSection(&CRT_MEMORY_2);
			m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
		}
#endif NEW
	}
}

void CHardwareCscopeView::Display()
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

void CHardwareCscopeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);
}