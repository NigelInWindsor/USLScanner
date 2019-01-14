// DynamicView.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "DynamicView.h"
#include "MainFrm.h"
#include <math.h>

// CDynamicView
#ifdef _ENABLE_DISABLE_FILLING
__declspec(dllimport) bool GetFillingBetweenCycle();
__declspec(dllimport) bool GetFillingLast();
#endif _ENABLE_DISABLE_FILLING

#define _PROFILING

IMPLEMENT_DYNCREATE(CDynamicView, CView)

UINT ThreadRefreshDisplay(LPVOID lpParameter)
{
	while (1)
	{
		if (g_pDynamicView)
		{
			if (g_pDynamicView->m_bOnSizing == false)
				g_pDynamicView->Display();

			Sleep(33);
		}
	}

	return 0;
}

CDynamicView::CDynamicView()
{
	g_pDynamicView = this;

	m_bDisplayReady = true;
	m_bOnSizing = false;
	m_bSetMapping = true;
	m_bResetConfiguration = true;

	m_iViewHeight = 100;
	m_iViewWidth = 100;
	m_iBitmapHeight = 100;
	m_iBitmapWidth = 100;
	m_iLastHeight = 0;
	m_iLastWidth = 0;
	
	m_pBuffer = NULL;
	m_pDataBuffer = NULL;
	m_pUpdateBuffer = NULL;
	
	m_pCosTable[0] = NULL;
	m_pSinTable[0] = NULL;

	m_pCosTable[1] = NULL;
	m_pSinTable[1] = NULL;

	m_pBorneMin = NULL;
	m_pBorneMax = NULL;

	m_bCursor = false;
	m_ptPrev.x=0;m_ptPrev.y=0;
	m_bCycleUpdate = false;
	m_iCycle = -1;
	m_xCycle1=m_xCycle2=m_zCycle1=m_zCycle2=-1.0e6;

	CreatePalette();

	m_pThread = NULL;
	InitializeCriticalSection(&m_crtDisplay);
}

CDynamicView::~CDynamicView()
{
	g_pDynamicView = NULL;

	if (m_pThread)
	{
		EnterCriticalSection(&m_crtDisplay);
		TerminateThread(m_pThread->m_hThread, 0);
		LeaveCriticalSection(&m_crtDisplay);
		::DeleteCriticalSection(&m_crtDisplay);
	}

	if (m_pBuffer)
		delete[] m_pBuffer;

	if (m_pDataBuffer)
		delete[] m_pDataBuffer;

	if (m_pUpdateBuffer)
		delete[] m_pUpdateBuffer;

	if (m_pCosTable[0])
		delete[] m_pCosTable[0];

	if (m_pSinTable[0])
		delete[] m_pSinTable[0];

	if (m_pCosTable[1])
		delete[] m_pCosTable[1];

	if (m_pSinTable[1])
		delete[] m_pSinTable[1];

	if (m_pBorneMin)
		delete[] m_pBorneMin;

	if (m_pBorneMax)
		delete[] m_pBorneMax;
}

BEGIN_MESSAGE_MAP(CDynamicView, CView)
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

// CDynamicView drawing

void CDynamicView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	pDoc->SetTitle(_T("Dynamic View"));
}

// CDynamicView message handlers

int CDynamicView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pThread = AfxBeginThread(ThreadRefreshDisplay, NULL);
	//SetTimer(TIMER_DYNAMIC_CORRECTED_VIEW, 33, NULL);

	return 0;
}

void CDynamicView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_DYNAMIC_CORRECTED_VIEW)
	{
		if (m_bOnSizing == false)
			Display();
	}

	CView::OnTimer(nIDEvent);
}

void CDynamicView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	EnterCriticalSection(&CRT_UPDATE_SETTING);

	if (m_bOnSizing == false)
	{
		if (cx > 0)
		{
			m_iViewWidth = cx;
			m_iBitmapWidth = cx;
		}

		if (cy > 0)
		{
			m_iViewHeight = cy;
			m_iBitmapHeight = cy;
		}
	}

	LeaveCriticalSection(&CRT_UPDATE_SETTING);
}

void CDynamicView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CView::OnSizing(fwSide, pRect);
	m_bOnSizing = true;
}

void CDynamicView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

void CDynamicView::CreatePalette()
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

void CDynamicView::CreateAngleTable()
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

void CDynamicView::ResetBuffer()
{
	CMainFrame *pMainFrame=CMainFrame::GetMainFrame();

	if ((m_iLastHeight != m_iViewHeight) || 
		(m_iLastWidth != m_iViewWidth) || 
		(m_bResetConfiguration == true))
	{
		if(pMainFrame)
			pMainFrame->ResetCursor();
		if (m_pBuffer)
		{
			delete[] m_pBuffer;
			delete[] m_pDataBuffer;
			delete[] m_pUpdateBuffer;

			delete[] m_pBorneMin;
			delete[] m_pBorneMax;

			delete[] m_PositionProbe1_X;
			delete[] m_PositionProbe1_Y;
			delete[] m_PositionProbe2_X;
			delete[] m_PositionProbe2_Y;
		}

		CreateAngleTable();

		// Get the size of the Z axis to display
		m_ZMin = 0.0;
		m_ZMax = GetZAxisWidth();

		m_XMin = GetXAxisMin();
		m_XMax = GetXAxisMax();

		m_pBuffer = new DWORD[m_iBitmapHeight * m_iBitmapWidth];
		m_pDataBuffer = new BYTE[m_iBitmapHeight * m_iBitmapWidth];
		m_pUpdateBuffer = new bool[m_iBitmapHeight * m_iBitmapWidth];

		memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));

		m_pBorneMin = new int[m_iBitmapHeight];
		m_pBorneMax = new int[m_iBitmapHeight];

		m_PositionProbe1_X = new int[g_uiCycleQuantity[0] * g_uiPointQuantity[0]];
		m_PositionProbe1_Y = new int[g_uiCycleQuantity[0] * g_uiPointQuantity[0]];

		m_PositionProbe2_X = new int[g_uiCycleQuantity[1] * g_uiPointQuantity[1]];
		m_PositionProbe2_Y = new int[g_uiCycleQuantity[1] * g_uiPointQuantity[1]];

		memset(m_pBorneMin, 0, m_iBitmapHeight * sizeof(int));

		for (int i = 0 ; i < m_iBitmapHeight ; i++)
			m_pBorneMax[i] = m_iBitmapWidth;

		memset(m_PositionProbe1_X, -1, g_uiCycleQuantity[0] * g_uiPointQuantity[0] * sizeof(int));
		memset(m_PositionProbe1_Y, -1, g_uiCycleQuantity[0] * g_uiPointQuantity[0] * sizeof(int));

		memset(m_PositionProbe2_X, -1, g_uiCycleQuantity[1] * g_uiPointQuantity[1] * sizeof(int));
		memset(m_PositionProbe2_Y, -1, g_uiCycleQuantity[1] * g_uiPointQuantity[1] * sizeof(int));

		m_iLastHeight = m_iViewHeight;
		m_iLastWidth = m_iViewWidth;

		m_bSetMapping = true;
		m_bResetConfiguration = false;
	}

	memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
	memset(m_pUpdateBuffer, false, m_iBitmapHeight * m_iBitmapWidth * sizeof(bool));

	m_bDisplayReady = true;
}

void CDynamicView::SetBorneBuffers()
{
	for (int i = 0 ; i < m_iBitmapHeight ; i++)
	{
		for (int j = 0 ; j < m_iBitmapWidth ; j++)
		{
			if (m_pUpdateBuffer[i * m_iBitmapWidth + j] == true)
			{
				m_pBorneMin[i] = j;
				break;
			}
		}

		for (int j = m_iBitmapWidth - 1 ; j >= 0 ; j--)
		{
			if (m_pUpdateBuffer[i * m_iBitmapWidth + j] == true)
			{
				m_pBorneMax[i] = j;
				break;
			}
		}
	}
}

void CDynamicView::SetBuffer()
{
	ResetBuffer();

	if (m_bSetMapping == true)
		SetMapping();

#ifdef _ENABLE_DISABLE_FILLING
if(!GetFillingLast() || !GetFillingBetweenCycle())
{
	memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
	memset(m_pUpdateBuffer, false, m_iBitmapHeight * m_iBitmapWidth * sizeof(bool));
	memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));
}
#endif _ENABLE_DISABLE_FILLING
	if (g_uiProbe == 0)
		DisplayProbe1();
	else if (g_uiProbe == 1)
		DisplayProbe2();

	if (m_bSetMapping == true)
	{
		SetBorneBuffers();
		m_bSetMapping = false;
	}

#ifdef _ENABLE_DISABLE_FILLING
	if(GetFillingLast())
	{
#endif _ENABLE_DISABLE_FILLING
	if ((m_pBorneMin) && (m_pBorneMax))
	{
		for (int i = 0 ; i < m_iBitmapHeight ; i++)
		{
			for (int j = m_pBorneMin[i] ; j <= m_pBorneMax[i] ; j++)
			{
				if (m_pUpdateBuffer[i * m_iBitmapWidth + j] == false)
					FillPosition(j, i);
			}
		}
	}
#ifdef _ENABLE_DISABLE_FILLING
	}
#endif _ENABLE_DISABLE_FILLING
}

void CDynamicView::SetMapping()
{
	double Step = GetPointStep();
	double D, Lx, Lh;

	if(!m_bCycleUpdate)
	{
		m_bCursor = false;
		m_ptPrev.x=0;m_ptPrev.y=0;
	}
	m_bCycleUpdate = false;
	m_xMin = 1.0e6;m_xMax = -1.0e6;
	m_zMin = 1.0e6;m_zMax = -1.0e6;
	m_xCycle1=m_xCycle2=m_zCycle1=m_zCycle2=-1.0e6;
	for (unsigned int i = 0 ; i < g_uiCycleQuantity[0] ; i++)
	{
		for (unsigned int j = 0 ; j < g_uiPointQuantity[0] ; j++)
		{
			D = j * Step;
			Lx = InfoProbe1.pdX[i] + D * m_pSinTable[0][i];
			Lh = D * m_pCosTable[0][i];

			m_PositionProbe1_X[i * g_uiPointQuantity[0] + j] = (int)(m_iBitmapWidth * (Lx - m_XMin) / (m_XMax - m_XMin));
			m_PositionProbe1_Y[i * g_uiPointQuantity[0] + j] = (int)(m_iBitmapHeight * (Lh - m_ZMin) / (m_ZMax - m_ZMin));

			if((m_iCycle>=0) && (m_iCycle==(int)i))
			{
				m_xCycle1 = InfoProbe1.pdX[i];
				m_zCycle1 = 0;
				m_xCycle2 = Lx;
				m_zCycle2 = Lh;
			}
			if(m_xMin>Lx)
				m_xMin = Lx;
			if(m_xMax<Lx)
				m_xMax = Lx;
			if(m_zMin>Lh)
				m_zMin = Lh;
			if(m_zMax<Lh)
				m_zMax = Lh;
		}
	}

	for (unsigned int i = 0 ; i < g_uiCycleQuantity[1] ; i++)
	{
		for (unsigned int j = 0 ; j < g_uiPointQuantity[1] ; j++)
		{
			D = j * Step;
			Lx = InfoProbe2.pdX[i] + D * m_pSinTable[1][i];
			Lh = D * m_pCosTable[1][i];

			m_PositionProbe2_X[i * g_uiPointQuantity[1] + j] = (int)(m_iBitmapWidth * (Lx - m_XMin) / (m_XMax - m_XMin));
			m_PositionProbe2_Y[i * g_uiPointQuantity[1] + j] = (int)(m_iBitmapHeight * (Lh - m_ZMin) / (m_ZMax - m_ZMin));
		}
	}
}

void CDynamicView::DisplayProbe1()
{
	int DeltaX, DeltaZ;
	int IndexX, IndexZ;
	BYTE AMP1, AMP2;
	bool bError=false;//integration
#ifdef _ENABLE_DISABLE_FILLING
	bool bFillingBetweenCycle=GetFillingBetweenCycle();
#endif _ENABLE_DISABLE_FILLING

	char* TempAscanMemory = new char[g_uiPointQuantity[0] * g_uiCycleQuantity[0]];
	EnterCriticalSection(&CRT_MEMORY_1);
	if(AscanMemory1)
		memcpy(TempAscanMemory, AscanMemory1[g_LastAscanEncoderPosition], g_uiPointQuantity[0] * g_uiCycleQuantity[0]);
	else
		bError = true;
	LeaveCriticalSection(&CRT_MEMORY_1);
	if(bError)
		return;

	for (unsigned int i = 0 ; i < g_uiCycleQuantity[0] - 1 ; i++)
	{
		//DWORD dwT1 = GetTickCount();

		for (unsigned int j = 0 ; j < g_uiPointQuantity[0] ; j++)
		{
			int TempPosition1 = i * g_uiPointQuantity[0] + j;
			int TempPosition2 = (i + 1) * g_uiPointQuantity[0] + j;
			
			if ((m_PositionProbe1_Y[TempPosition1] < 0) && (m_PositionProbe1_X[TempPosition1] < 0))//integration
				return;//integration
			if ((m_PositionProbe1_Y[TempPosition1] < m_iBitmapHeight) && (m_PositionProbe1_X[TempPosition1] < m_iBitmapWidth))
			{
				int Position = m_PositionProbe1_Y[TempPosition1] * m_iBitmapWidth + m_PositionProbe1_X[TempPosition1];

				m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(TempAscanMemory[TempPosition1]) * 2 - 1);
				m_pUpdateBuffer[Position] = true;
				m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
			}

			if ((m_PositionProbe1_Y[TempPosition2] < 0 ) && (m_PositionProbe1_X[TempPosition2] < 0))//integration
				return;//integration
			if ((m_PositionProbe1_Y[TempPosition2] < m_iBitmapHeight) && (m_PositionProbe1_X[TempPosition2] < m_iBitmapWidth))
			{
				int Position = m_PositionProbe1_Y[TempPosition2] * m_iBitmapWidth + m_PositionProbe1_X[TempPosition2];

				m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(TempAscanMemory[TempPosition2]) * 2 - 1);
				m_pUpdateBuffer[Position] = true;
				m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
			}
#ifdef _ENABLE_DISABLE_FILLING
			if(!bFillingBetweenCycle)
				continue;
#endif _ENABLE_DISABLE_FILLING
			if ((m_PositionProbe1_Y[TempPosition1] < m_iBitmapHeight) && (m_PositionProbe1_X[TempPosition1] < m_iBitmapWidth) &&
				(m_PositionProbe1_Y[TempPosition2] < m_iBitmapHeight) && (m_PositionProbe1_X[TempPosition2] < m_iBitmapWidth))
			{
				AMP1 = m_pDataBuffer[m_PositionProbe1_Y[TempPosition1] * m_iBitmapWidth + m_PositionProbe1_X[TempPosition1]];
				AMP2 = m_pDataBuffer[m_PositionProbe1_Y[TempPosition2] * m_iBitmapWidth + m_PositionProbe1_X[TempPosition2]];

				DeltaX = m_PositionProbe1_X[TempPosition2] - m_PositionProbe1_X[TempPosition1];
				DeltaZ = m_PositionProbe1_Y[TempPosition2] - m_PositionProbe1_Y[TempPosition1];

				if (DeltaX > DeltaZ)
				{
					for (int k = 1 ; k < DeltaX ; k++)
					{
						IndexX = m_PositionProbe1_X[TempPosition1] + k;
						IndexZ = m_PositionProbe1_Y[TempPosition1] + k * DeltaZ / DeltaX;
                   
						if ((DeltaX > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaX));
							m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
							m_pUpdateBuffer[Position] = true;
						}
					}
				}
				else
				{
					for (int k = 1 ; k < DeltaZ ; k++)
					{
						IndexX = m_PositionProbe1_X[TempPosition1] + k * DeltaX / DeltaZ;
						IndexZ = m_PositionProbe1_Y[TempPosition1] + k;
                   
						if ((DeltaZ > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaZ));
							m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
							m_pUpdateBuffer[Position] = true;
						}
					}
				}
			}
		}

		//DWORD dwT2 = GetTickCount();

		//if (dwT2 != dwT1)
		//{
		//	CString csText;
		//	csText.Format(_T("Delai (%d) = %d\r\n"), i, dwT2 - dwT1);	//-576
		//	TRACE(csText);
		//}
	}

	delete[] TempAscanMemory;
}

void CDynamicView::DisplayProbe2()
{
	int DeltaX, DeltaZ;
	int IndexX, IndexZ;
	BYTE AMP1, AMP2;

	char* TempAscanMemory = new char[g_uiPointQuantity[1] * g_uiCycleQuantity[1]];
	EnterCriticalSection(&CRT_MEMORY_2);
	memcpy(TempAscanMemory, AscanMemory2[g_LastAscanEncoderPosition], g_uiPointQuantity[1] * g_uiCycleQuantity[1]);
	LeaveCriticalSection(&CRT_MEMORY_2);

	for (unsigned int i = 0 ; i < g_uiCycleQuantity[1] - 1 ; i++)
	{
		//DWORD dwT1 = GetTickCount();

		for (unsigned int j = 0 ; j < g_uiPointQuantity[1] ; j++)
		{
			int TempPosition1 = i * g_uiPointQuantity[1] + j;
			int TempPosition2 = (i + 1) * g_uiPointQuantity[1] + j;
			
			if ((m_PositionProbe2_Y[TempPosition1] < m_iBitmapHeight) && (m_PositionProbe2_X[TempPosition1] < m_iBitmapWidth))
			{
				int Position = m_PositionProbe2_Y[TempPosition1] * m_iBitmapWidth + m_PositionProbe2_X[TempPosition1];

				m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(TempAscanMemory[TempPosition1]) * 2 - 1);
				m_pUpdateBuffer[Position] = true;
				m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
			}

			if ((m_PositionProbe2_Y[TempPosition2] < m_iBitmapHeight) && (m_PositionProbe2_X[TempPosition2] < m_iBitmapWidth))
			{
				int Position = m_PositionProbe2_Y[TempPosition2] * m_iBitmapWidth + m_PositionProbe2_X[TempPosition2];

				m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(TempAscanMemory[TempPosition2]) * 2 - 1);
				m_pUpdateBuffer[Position] = true;
				m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
			}

			if ((m_PositionProbe2_Y[TempPosition1] < m_iBitmapHeight) && (m_PositionProbe2_X[TempPosition1] < m_iBitmapWidth) &&
				(m_PositionProbe2_Y[TempPosition2] < m_iBitmapHeight) && (m_PositionProbe2_X[TempPosition2] < m_iBitmapWidth))
			{
				AMP1 = m_pDataBuffer[m_PositionProbe2_Y[TempPosition1] * m_iBitmapWidth + m_PositionProbe2_X[TempPosition1]];
				AMP2 = m_pDataBuffer[m_PositionProbe2_Y[TempPosition2] * m_iBitmapWidth + m_PositionProbe2_X[TempPosition2]];

				DeltaX = m_PositionProbe2_X[TempPosition2] - m_PositionProbe2_X[TempPosition1];
				DeltaZ = m_PositionProbe2_Y[TempPosition2] - m_PositionProbe2_Y[TempPosition1];

				if (DeltaX > DeltaZ)
				{
					for (int k = 1 ; k < DeltaX ; k++)
					{
						IndexX = m_PositionProbe2_X[TempPosition1] + k;
						IndexZ = m_PositionProbe2_Y[TempPosition1] + k * DeltaZ / DeltaX;
                   
						if ((DeltaX > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaX));
							m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
							m_pUpdateBuffer[Position] = true;
						}
					}
				}
				else
				{
					for (int k = 1 ; k < DeltaZ ; k++)
					{
						IndexX = m_PositionProbe2_X[TempPosition1] + k * DeltaX / DeltaZ;
						IndexZ = m_PositionProbe2_Y[TempPosition1] + k;
                   
						if ((DeltaZ > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaZ));
							m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
							m_pUpdateBuffer[Position] = true;
						}
					}
				}
			}
		}

		//DWORD dwT2 = GetTickCount();

		//if (dwT2 != dwT1)
		//{
		//	CString csText;
		//	csText.Format(_T("Delai (%d) = %d\r\n"), i, dwT2 - dwT1);	//-576
		//	TRACE(csText);
		//}
	}

	delete[] TempAscanMemory;
}

void CDynamicView::FillPosition(int IndexX, int IndexZ)
{
	unsigned int Value = 0;
	unsigned int Quantity = 0;

	if ((IndexX - 1 >= 0) && 
		(IndexZ - 1 >= 0) && 
		(m_pUpdateBuffer[(IndexZ - 1) * m_iBitmapWidth + IndexX - 1] == true))
	{
		Value += m_pDataBuffer[(IndexZ - 1) * m_iBitmapWidth + IndexX - 1];
		Quantity++;
	}

	if ((IndexZ - 1 >= 0) && 
		(m_pUpdateBuffer[(IndexZ - 1) * m_iBitmapWidth + IndexX] == true))
	{
		Value += m_pDataBuffer[(IndexZ - 1) * m_iBitmapWidth + IndexX];
		Quantity++;
	}

	if ((IndexX + 1 < m_iBitmapWidth) && 
		(IndexZ - 1 >= 0) && 
		(m_pUpdateBuffer[(IndexZ - 1) * m_iBitmapWidth + IndexX + 1] == true))
	{
		Value += m_pDataBuffer[(IndexZ - 1) * m_iBitmapWidth + IndexX + 1];
		Quantity++;
	}

	if ((IndexX - 1 >= 0) &&
		(IndexZ < m_iBitmapHeight) &&
		(m_pUpdateBuffer[IndexZ * m_iBitmapWidth + IndexX - 1] == true))
	{
		Value += m_pDataBuffer[IndexZ * m_iBitmapWidth + IndexX - 1];
		Quantity++;
	}

	if ((IndexX + 1 < m_iBitmapWidth) && 
		(IndexZ < m_iBitmapHeight) &&
		(m_pUpdateBuffer[IndexZ * m_iBitmapWidth + IndexX + 1] == true))
	{
		Value += m_pDataBuffer[IndexZ * m_iBitmapWidth + IndexX + 1];
		Quantity++;
	}

	if ((IndexX - 1 >= 0) && 
		(IndexZ + 1 < m_iBitmapHeight) && 
		((IndexZ + 1) * m_iBitmapWidth + IndexX - 1 < m_iBitmapHeight * m_iBitmapWidth) &&
		(m_pUpdateBuffer[(IndexZ + 1) * m_iBitmapWidth + IndexX - 1] == true))
	{
		Value += m_pDataBuffer[(IndexZ + 1) * m_iBitmapWidth + IndexX - 1];
		Quantity++;
	}

	if ((IndexZ + 1 < m_iBitmapHeight) && 
		((IndexZ + 1) * m_iBitmapWidth + IndexX < m_iBitmapWidth * m_iBitmapHeight) &&
		(m_pUpdateBuffer[(IndexZ + 1) * m_iBitmapWidth + IndexX] == true))
	{
		Value += m_pDataBuffer[(IndexZ + 1) * m_iBitmapWidth + IndexX];
		Quantity++;
	}

	if ((IndexX + 1 < m_iBitmapWidth) && 
		(IndexZ + 1 < m_iBitmapHeight) && 
		(m_pUpdateBuffer[(IndexZ + 1) * m_iBitmapWidth + IndexX + 1] == true))
	{
		Value += m_pDataBuffer[(IndexZ + 1) * m_iBitmapWidth + IndexX + 1];
		Quantity++;
	}

	if ((Quantity > 0) && (IndexX < m_iBitmapWidth) && (IndexZ < m_iBitmapHeight))
	{
		m_pDataBuffer[IndexZ * m_iBitmapWidth + IndexX] = (BYTE)(Value / Quantity);
		m_pBuffer[IndexZ * m_iBitmapWidth + IndexX] = m_cPalette[m_pDataBuffer[IndexZ * m_iBitmapWidth + IndexX]];
		m_pUpdateBuffer[IndexZ * m_iBitmapWidth + IndexX] = true;
	}
}

void CDynamicView::SetCycle(int iCycle)
{
	m_iCycle = iCycle;
	m_bResetConfiguration = true;
	m_bCycleUpdate = true;
}

void CDynamicView::Display()
{
	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CPen pen;
	CPen penAscan;

	if (!pen.CreatePen(/*PS_DASHDOTDOT*/PS_SOLID, 1, RGB(255,0,0)))//PS_DASH
		return;
	if (!penAscan.CreatePen(/*PS_DASHDOTDOT*/PS_SOLID, 1, RGB(0,0,0)))//PS_DASH
		return;

	EnterCriticalSection(&CRT_UPDATE_SETTING);
	SetBuffer();
	LeaveCriticalSection(&CRT_UPDATE_SETTING);

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
					0, m_iViewHeight-1, m_iViewWidth, -(int)m_iViewHeight, 
					0, 0, m_iBitmapWidth, m_iBitmapHeight, 
					m_pBuffer, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	delete[] pBmpInfo;

	if(m_bCursor)
	{
		dc.SelectObject(&pen);
		dc.MoveTo(m_ptPrev.x,0);
		dc.LineTo(m_ptPrev.x,m_iViewHeight-1);
		dc.MoveTo(0,m_ptPrev.y);
		dc.LineTo(m_iViewWidth-1,m_ptPrev.y);
	}
	if(m_xCycle1>-1.0e6)
	{
		dc.SelectObject(&penAscan);
		dc.MoveTo(	(int)(m_iViewWidth*(m_xCycle1-m_xMin)/(m_xMax-m_xMin)),
					(int)(m_iViewHeight*(m_zCycle1-m_zMin)/(m_zMax-m_zMin)));
		dc.LineTo(	(int)(m_iViewWidth*(m_xCycle2-m_xMin)/(m_xMax-m_xMin)),
					(int)(m_iViewHeight*(m_zCycle2-m_zMin)/(m_zMax-m_zMin)));
	}
}

void CDynamicView::OnActivateView(BOOL bActivate,CView* pActivateView,CView* pDeactiveView)
{
	if(m_bCursor && (pActivateView==dynamic_cast<CView*>(this)))
	{
		SetPoint(m_ptPrev);
	}
}

void CDynamicView::SetPoint(CPoint point)
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

void CDynamicView::OnLButtonDown(UINT, CPoint point) 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	SetCapture();       // Capture the mouse until button up.
	SetPoint(point);
	return;
}

void CDynamicView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bOnSizing)
		m_bOnSizing = false;

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

void CDynamicView::OnMouseMove(UINT, CPoint point) 
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

void CDynamicView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	m_bCursor = false;
}

void CDynamicView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//OnButtonDblClk(0,nFlags,point);
	m_bCursor = false;
}

void CDynamicView::OnButtonDblClk(int iCursorIndex,UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	// CScrollView changes the viewport origin and mapping mode.
	// It's necessary to convert the point from device coordinates
	// to logical coordinates, such as are stored in the document.
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	SetPoint(point);
}
