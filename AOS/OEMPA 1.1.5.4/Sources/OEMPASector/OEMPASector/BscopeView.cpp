// BscopeView.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "BscopeView.h"
#include <math.h>
#include "ExternalDefinition.h"

// CBscopeView

IMPLEMENT_DYNCREATE(CBscopeView, CView)

CBscopeView::CBscopeView()
{
	g_pBscope = this;

	m_bOnSizing = false;
	m_uiEvent = 5;

	m_iViewHeight = 100;
	m_iViewWidth = 100;
	
	m_pBuffer = NULL;
	m_pDataBuffer = NULL;
	m_pUpdateBuffer[0] = NULL;
	m_pUpdateBuffer[1] = NULL;
	
	m_pCosTable[0] = NULL;
	m_pSinTable[0] = NULL;

	m_pCosTable[1] = NULL;
	m_pSinTable[1] = NULL;

	m_pBorneMin[0] = NULL;
	m_pBorneMin[1] = NULL;
	m_pBorneMax[0] = NULL;
	m_pBorneMax[1] = NULL;

	m_pPositionProbe_X[0] = NULL;
	m_pPositionProbe_Z[0] = NULL;
	m_pPositionProbe_X[1] = NULL;
	m_pPositionProbe_Z[1] = NULL;

	CreatePalette();
}

CBscopeView::~CBscopeView()
{
	g_pBscope = NULL;

	if (m_pBuffer)
		delete[] m_pBuffer;

	if (m_pDataBuffer)
		delete[] m_pDataBuffer;

	if (m_pUpdateBuffer[0])
		delete[] m_pUpdateBuffer[0];
	
	if (m_pUpdateBuffer[1])
		delete[] m_pUpdateBuffer[1];

	if (m_pCosTable[0])
		delete[] m_pCosTable[0];

	if (m_pSinTable[0])
		delete[] m_pSinTable[0];

	if (m_pCosTable[1])
		delete[] m_pCosTable[1];

	if (m_pSinTable[1])
		delete[] m_pSinTable[1];

	if (m_pBorneMin[0])
		delete[] m_pBorneMin[0];

	if (m_pBorneMin[1])
		delete[] m_pBorneMin[1];

	if (m_pBorneMax[0])
		delete[] m_pBorneMax[0];

	if (m_pBorneMax[1])
		delete[] m_pBorneMax[1];

	if (m_pPositionProbe_X[0])
		delete[] m_pPositionProbe_X[0];

	if (m_pPositionProbe_X[1])
		delete[] m_pPositionProbe_X[1];

	if (m_pPositionProbe_Z[0])
		delete[] m_pPositionProbe_Z[0];

	if (m_pPositionProbe_Z[1])
		delete[] m_pPositionProbe_Z[1];
}

BEGIN_MESSAGE_MAP(CBscopeView, CView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CBscopeView drawing

void CBscopeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	pDoc->SetTitle(_T("B-scan Analysis"));
}

// CBscopeView message handlers

int CBscopeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(TIMER_BSCOPE_VIEW, 33, NULL);

	return 0;
}

void CBscopeView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_BSCOPE_VIEW)
	{
		if (m_uiEvent != 0)
		{
			UpdateDisplayData();
			m_uiEvent = 0;
		}

		if (m_bOnSizing == false)
			Display();
	}

	CView::OnTimer(nIDEvent);
}

void CBscopeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_iViewWidth = cx;
	m_iViewHeight = cy;
	m_iBitmapWidth = cx;
	m_iBitmapHeight = cy;
}

void CBscopeView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CView::OnSizing(fwSide, pRect);
	m_bOnSizing = true;
}

void CBscopeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

void CBscopeView::UpdateDisplayData()
{
	ResetBuffer();
	CreateAngleTable();
	SetMapping();

	if (g_uiMode == 0)
	{
		if (g_uiProbeAnalysis == 0)
			DisplayProbe1Slice();
		else if (g_uiProbeAnalysis == 1)
			DisplayProbe2Slice();
		else if (g_uiProbeAnalysis == 2)
		{
			DisplayProbe1Slice();
			DisplayProbe2Slice();
		}
	}
	else
	{
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

	SetBorneBuffers();
	Fill();
}

void CBscopeView::SetEvent(UINT Event)
{
	m_uiEvent = Event;

	if (m_uiEvent == 1)
		m_bOnSizing = false;
}

void CBscopeView::CreatePalette()
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

void CBscopeView::CreateAngleTable()
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

void CBscopeView::ResetBuffer()
{
	if (m_pBuffer)
	{
		delete[] m_pBuffer;
		delete[] m_pDataBuffer;
		delete[] m_pUpdateBuffer[0];
		delete[] m_pUpdateBuffer[1];

		delete[] m_pBorneMin[0];
		delete[] m_pBorneMin[1];
		delete[] m_pBorneMax[0];
		delete[] m_pBorneMax[1];

		delete[] m_pPositionProbe_X[0];
		delete[] m_pPositionProbe_Z[0];
		delete[] m_pPositionProbe_X[1];
		delete[] m_pPositionProbe_Z[1];
	}

	m_pBuffer = new DWORD[m_iBitmapHeight * m_iBitmapWidth];
	m_pDataBuffer = new BYTE[m_iBitmapHeight * m_iBitmapWidth];
	m_pUpdateBuffer[0] = new bool[m_iBitmapHeight * m_iBitmapWidth];
	m_pUpdateBuffer[1] = new bool[m_iBitmapHeight * m_iBitmapWidth];
	memset(m_pBuffer, m_cPalette[0], m_iBitmapHeight * m_iBitmapWidth * sizeof(DWORD));

	m_pBorneMin[0] = new int[m_iBitmapHeight];
	m_pBorneMax[0] = new int[m_iBitmapHeight];
	m_pBorneMin[1] = new int[m_iBitmapHeight];
	m_pBorneMax[1] = new int[m_iBitmapHeight];

	m_pPositionProbe_X[0] = new int[InfoProbe1Analysis.iCycleCount * InfoProbe1Analysis.PointQuantity];
	m_pPositionProbe_Z[0] = new int[InfoProbe1Analysis.iCycleCount * InfoProbe1Analysis.PointQuantity];

	m_pPositionProbe_X[1] = new int[InfoProbe2Analysis.iCycleCount * InfoProbe2Analysis.PointQuantity];
	m_pPositionProbe_Z[1] = new int[InfoProbe2Analysis.iCycleCount * InfoProbe2Analysis.PointQuantity];

	memset(m_pBorneMin[0], 0, m_iBitmapHeight * sizeof(int));
	memset(m_pBorneMin[1], 0, m_iBitmapHeight * sizeof(int));

	for (int i = 0 ; i < m_iBitmapHeight ; i++)
	{
		m_pBorneMax[0][i] = m_iBitmapWidth;
		m_pBorneMax[1][i] = m_iBitmapWidth;
	}

	memset(m_pPositionProbe_X[0], -1, InfoProbe1Analysis.iCycleCount * InfoProbe1Analysis.PointQuantity * sizeof(int));
	memset(m_pPositionProbe_Z[0], -1, InfoProbe1Analysis.iCycleCount * InfoProbe1Analysis.PointQuantity * sizeof(int));

	memset(m_pPositionProbe_X[1], -1, InfoProbe2Analysis.iCycleCount * InfoProbe2Analysis.PointQuantity * sizeof(int));
	memset(m_pPositionProbe_Z[1], -1, InfoProbe2Analysis.iCycleCount * InfoProbe2Analysis.PointQuantity * sizeof(int));

	memset(m_pDataBuffer, 0, m_iBitmapHeight * m_iBitmapWidth);
	memset(m_pUpdateBuffer[0], false, m_iBitmapHeight * m_iBitmapWidth * sizeof(bool));
	memset(m_pUpdateBuffer[1], false, m_iBitmapHeight * m_iBitmapWidth * sizeof(bool));
}

void CBscopeView::SetBorneBuffers()
{
	for (int i = 0 ; i < m_iBitmapHeight ; i++)
	{
		bool Found = false;

		for (int j = 0 ; j < m_iBitmapWidth ; j++)
		{
			if (m_pUpdateBuffer[0][i * m_iBitmapWidth + j] == true)
			{
				m_pBorneMin[0][i] = j;
				Found = true;
				break;
			}
		}

		if (Found == false)
			m_pBorneMin[0][i] = -1;
		else
		{
			for (int j = m_iBitmapWidth - 1 ; j >= 0 ; j--)
			{
				if (m_pUpdateBuffer[0][i * m_iBitmapWidth + j] == true)
				{
					m_pBorneMax[0][i] = j;
					break;
				}
			}
		}

		Found = false;

		for (int j = 0 ; j < m_iBitmapWidth ; j++)
		{
			if (m_pUpdateBuffer[1][i * m_iBitmapWidth + j] == true)
			{
				m_pBorneMin[1][i] = j;
				Found = true;
				break;
			}
		}

		if (Found == false)
			m_pBorneMin[1][i] = -1;
		else
		{
			for (int j = m_iBitmapWidth - 1 ; j >= 0 ; j--)
			{
				if (m_pUpdateBuffer[1][i * m_iBitmapWidth + j] == true)
				{
					m_pBorneMax[1][i] = j;
					break;
				}
			}
		}
	}
}

void CBscopeView::Fill()
{
	double XMin = GetAnalysisXAxisMin();
	double XMax = GetAnalysisXAxisMax();
	double ZMin = GetAnalysisZAxisMin();
	double ZMax = GetAnalysisZAxisMax();

	int IndexXMin = (int)(m_iBitmapWidth * (Analysis_XMIN - XMin) / (XMax - XMin));
	int IndexXMax = (int)(m_iBitmapWidth * (Analysis_XMAX - XMin) / (XMax - XMin));
	int IndexZMin = (int)(m_iBitmapHeight * (Analysis_ZMIN - ZMin) / (ZMax - ZMin));
	int IndexZMax = (int)(m_iBitmapHeight * (Analysis_ZMAX - ZMin) / (ZMax - ZMin));

	if (g_uiProbeAnalysis == 0)
	{
		if ((m_pBorneMin[0]) && (m_pBorneMax[0]))
		{
			for (int i = 0 ; i < m_iBitmapHeight ; i++)
			{
				if (m_pBorneMin[0][i] > -1)
				{
					for (int j = m_pBorneMin[0][i] ; j <= m_pBorneMax[0][i] ; j++)
					{
						if ((m_pUpdateBuffer[0][i * m_iBitmapWidth + j] == false) &&
							(j >= IndexXMin) &&
							(j <= IndexXMax) &&
							(i >= IndexZMin) &&
							(i <= IndexZMax))
							FillPosition(0, j, i);
					}
				}
			}
		}
	}
	else if (g_uiProbeAnalysis == 1)
	{
		if ((m_pBorneMin[1]) && (m_pBorneMax[1]))
		{
			for (int i = 0 ; i < m_iBitmapHeight ; i++)
			{
				if (m_pBorneMin[1][i] > -1)
				{
					for (int j = m_pBorneMin[1][i] ; j <= m_pBorneMax[1][i] ; j++)
					{
						if ((m_pUpdateBuffer[1][i * m_iBitmapWidth + j] == false) &&
							(j >= IndexXMin) &&
							(j <= IndexXMax) &&
							(i >= IndexZMin) &&
							(i <= IndexZMax))
							FillPosition(1, j, i);
					}
				}
			}
		}
	}
	else if (g_uiProbeAnalysis == 2)
	{
		if ((m_pBorneMin[0]) && (m_pBorneMax[0]))
		{
			for (int i = 0 ; i < m_iBitmapHeight ; i++)
			{
				if (m_pBorneMin[0][i] > -1)
				{
					for (int j = m_pBorneMin[0][i] ; j <= m_pBorneMax[0][i] ; j++)
					{
						if ((m_pUpdateBuffer[0][i * m_iBitmapWidth + j] == false) &&
							(j >= IndexXMin) &&
							(j <= IndexXMax) &&
							(i >= IndexZMin) &&
							(i <= IndexZMax))
							FillPosition(0, j, i);
					}
				}
			}
		}

		if ((m_pBorneMin[1]) && (m_pBorneMax[1]))
		{
			for (int i = 0 ; i < m_iBitmapHeight ; i++)
			{
				if (m_pBorneMin[1][i] > -1)
				{
					for (int j = m_pBorneMin[1][i] ; j <= m_pBorneMax[1][i] ; j++)
					{
						if ((m_pUpdateBuffer[1][i * m_iBitmapWidth + j] == false) &&
							(j >= IndexXMin) &&
							(j <= IndexXMax) &&
							(i >= IndexZMin) &&
							(i <= IndexZMax))
							FillPosition(1, j, i);
					}
				}
			}
		}
	}
}

void CBscopeView::SetMapping()
{
	double Step = GetPointStepAnalysis();
	double D, Lx, Lh;

	double XMin = GetAnalysisXAxisMin();
	double XMax = GetAnalysisXAxisMax();
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

				m_pPositionProbe_X[0][i * InfoProbe1Analysis.PointQuantity + j] = (int)(m_iBitmapWidth * (Lx - XMin) / (XMax - XMin));
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

				m_pPositionProbe_X[1][i * InfoProbe2Analysis.PointQuantity + j] = (int)(m_iBitmapWidth * (Lx - XMin) / (XMax - XMin));
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

				m_pPositionProbe_X[0][i * InfoProbe1Analysis.PointQuantity + j] = (int)(m_iBitmapWidth * (Lx - XMin) / (XMax - XMin));
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

				m_pPositionProbe_X[1][i * InfoProbe2Analysis.PointQuantity + j] = (int)(m_iBitmapWidth * (Lx - XMin) / (XMax - XMin));
				m_pPositionProbe_Z[1][i * InfoProbe2Analysis.PointQuantity + j] = (int)(m_iBitmapHeight * (Lh - ZMin) / (ZMax - ZMin));
			}
		}
	}
}

void CBscopeView::DisplayProbe1Slice()
{
	int DeltaX, DeltaZ;
	int IndexX, IndexZ;

	BYTE AMP1, AMP2;

	int EncoderPosition = (int)(Analysis_YPOS / SystemAnalysis.dEncoderStep);
	COLORREF BlankColor = RGB(255, 255, 255);

	EnterCriticalSection(&CRT_ANALYSIS_MEMORY_1);

	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe1Analysis.iCycleCount - 1 ; i++)
	{
		DWORD dwT1 = GetTickCount();

		for (unsigned int j = 0 ; j < (unsigned int)InfoProbe1Analysis.PointQuantity ; j++)
		{
			int TempPosition1 = i * InfoProbe1Analysis.PointQuantity + j;
			int TempPosition2 = (i + 1) * InfoProbe1Analysis.PointQuantity + j;
			
			if ((m_pPositionProbe_Z[0][TempPosition1] >= 0) &&
				(m_pPositionProbe_Z[0][TempPosition1] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[0][TempPosition1] >= 0) &&
				(m_pPositionProbe_X[0][TempPosition1] < m_iBitmapWidth))
			{
				int Position = m_pPositionProbe_Z[0][TempPosition1] * m_iBitmapWidth + m_pPositionProbe_X[0][TempPosition1];

				m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanAnalysisMemory1[EncoderPosition][TempPosition1]) * 2 - 1);
				m_pUpdateBuffer[0][Position] = true;
				m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
			}

			if ((m_pPositionProbe_Z[0][TempPosition2] >= 0) && 
				(m_pPositionProbe_Z[0][TempPosition2] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[0][TempPosition2] >= 0) &&
				(m_pPositionProbe_X[0][TempPosition2] < m_iBitmapWidth))
			{
				int Position = m_pPositionProbe_Z[0][TempPosition2] * m_iBitmapWidth + m_pPositionProbe_X[0][TempPosition2];

				m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanAnalysisMemory1[EncoderPosition][TempPosition2]) * 2 - 1);
				m_pUpdateBuffer[0][Position] = true;
				m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
			}

			if ((m_pPositionProbe_Z[0][TempPosition1] >= 0) &&
				(m_pPositionProbe_Z[0][TempPosition1] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[0][TempPosition1] >= 0) &&
				(m_pPositionProbe_X[0][TempPosition1] < m_iBitmapWidth) &&
				(m_pPositionProbe_Z[0][TempPosition2] >= 0) && 
				(m_pPositionProbe_Z[0][TempPosition2] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[0][TempPosition2] >= 0) &&
				(m_pPositionProbe_X[0][TempPosition2] < m_iBitmapWidth))
			{
				AMP1 = m_pDataBuffer[m_pPositionProbe_Z[0][TempPosition1] * m_iBitmapWidth + m_pPositionProbe_X[0][TempPosition1]];
				AMP2 = m_pDataBuffer[m_pPositionProbe_Z[0][TempPosition2] * m_iBitmapWidth + m_pPositionProbe_X[0][TempPosition2]];

				DeltaX = m_pPositionProbe_X[0][TempPosition2] - m_pPositionProbe_X[0][TempPosition1];
				DeltaZ = m_pPositionProbe_Z[0][TempPosition2] - m_pPositionProbe_Z[0][TempPosition1];

				if (DeltaX > DeltaZ)
				{
					for (int k = 1 ; k < DeltaX ; k++)
					{
						IndexX = m_pPositionProbe_X[0][TempPosition1] + k;
						IndexZ = m_pPositionProbe_Z[0][TempPosition1] + k * DeltaZ / DeltaX;
                   
						if ((DeltaX > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaX));
							m_pUpdateBuffer[0][Position] = true;
							m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
						}
					}
				}
				else
				{
					for (int k = 1 ; k < DeltaZ ; k++)
					{
						IndexX = m_pPositionProbe_X[0][TempPosition1] + k * DeltaX / DeltaZ;
						IndexZ = m_pPositionProbe_Z[0][TempPosition1] + k;
                   
						if ((DeltaZ > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaZ));
							m_pUpdateBuffer[0][Position] = true;
							m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
						}
					}
				}
			}
		}

		DWORD dwT2 = GetTickCount();

		if (dwT2 != dwT1)
		{
			CString csText;
			csText.Format(_T("Delai (%d) = %d\r\n"), i, dwT2 - dwT1);	//-576
			TRACE(csText);
		}
	}

	LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_1);
}

void CBscopeView::DisplayProbe2Slice()
{
	int DeltaX, DeltaZ;
	int IndexX, IndexZ;

	BYTE AMP1, AMP2;

	int EncoderPosition = (int)(Analysis_YPOS / SystemAnalysis.dEncoderStep);
	COLORREF BlankColor = RGB(255, 255, 255);

	EnterCriticalSection(&CRT_ANALYSIS_MEMORY_2);

	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2Analysis.iCycleCount - 1 ; i++)
	{
		DWORD dwT1 = GetTickCount();

		for (unsigned int j = 0 ; j < (unsigned int)InfoProbe2Analysis.PointQuantity ; j++)
		{
			int TempPosition1 = i * InfoProbe2Analysis.PointQuantity + j;
			int TempPosition2 = (i + 1) * InfoProbe2Analysis.PointQuantity + j;
			
			if ((m_pPositionProbe_Z[1][TempPosition1] >= 0) &&
				(m_pPositionProbe_Z[1][TempPosition1] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[1][TempPosition1] >= 0) &&
				(m_pPositionProbe_X[1][TempPosition1] < m_iBitmapWidth))
			{
				int Position = m_pPositionProbe_Z[1][TempPosition1] * m_iBitmapWidth + m_pPositionProbe_X[1][TempPosition1];

				m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanAnalysisMemory2[EncoderPosition][TempPosition1]) * 2 - 1);
				m_pUpdateBuffer[1][Position] = true;
				m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
			}

			if ((m_pPositionProbe_Z[1][TempPosition2] >= 0) && 
				(m_pPositionProbe_Z[1][TempPosition2] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[1][TempPosition2] >= 0) &&
				(m_pPositionProbe_X[1][TempPosition2] < m_iBitmapWidth))
			{
				int Position = m_pPositionProbe_Z[1][TempPosition2] * m_iBitmapWidth + m_pPositionProbe_X[1][TempPosition2];

				m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanAnalysisMemory2[EncoderPosition][TempPosition2]) * 2 - 1);
				m_pUpdateBuffer[1][Position] = true;
				m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
			}

			if ((m_pPositionProbe_Z[1][TempPosition1] >= 0) &&
				(m_pPositionProbe_Z[1][TempPosition1] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[1][TempPosition1] >= 0) &&
				(m_pPositionProbe_X[1][TempPosition1] < m_iBitmapWidth) &&
				(m_pPositionProbe_Z[1][TempPosition2] >= 0) && 
				(m_pPositionProbe_Z[1][TempPosition2] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[1][TempPosition2] >= 0) &&
				(m_pPositionProbe_X[1][TempPosition2] < m_iBitmapWidth))
			{
				AMP1 = m_pDataBuffer[m_pPositionProbe_Z[1][TempPosition1] * m_iBitmapWidth + m_pPositionProbe_X[1][TempPosition1]];
				AMP2 = m_pDataBuffer[m_pPositionProbe_Z[1][TempPosition2] * m_iBitmapWidth + m_pPositionProbe_X[1][TempPosition2]];

				DeltaX = m_pPositionProbe_X[1][TempPosition2] - m_pPositionProbe_X[1][TempPosition1];
				DeltaZ = m_pPositionProbe_Z[1][TempPosition2] - m_pPositionProbe_Z[1][TempPosition1];

				if (DeltaX > DeltaZ)
				{
					for (int k = 1 ; k < DeltaX ; k++)
					{
						IndexX = m_pPositionProbe_X[1][TempPosition1] + k;
						IndexZ = m_pPositionProbe_Z[1][TempPosition1] + k * DeltaZ / DeltaX;
                   
						if ((DeltaX > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaX));
							m_pUpdateBuffer[1][Position] = true;
							m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
						}
					}
				}
				else
				{
					for (int k = 1 ; k < DeltaZ ; k++)
					{
						IndexX = m_pPositionProbe_X[1][TempPosition1] + k * DeltaX / DeltaZ;
						IndexZ = m_pPositionProbe_Z[1][TempPosition1] + k;
                   
						if ((DeltaZ > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaZ));
							m_pUpdateBuffer[1][Position] = true;
							m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
						}
					}
				}
			}
		}

		DWORD dwT2 = GetTickCount();

		if (dwT2 != dwT1)
		{
			CString csText;
			csText.Format(_T("Delai (%d) = %d\r\n"), i, dwT2 - dwT1);	//-576
			TRACE(csText);
		}
	}

	LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_2);
}

void CBscopeView::DisplayProbe1Gate()
{
	int DeltaX, DeltaZ;
	int IndexX, IndexZ;
	BYTE AMP1, AMP2;

	double XMin = GetAnalysisXAxisMin();
	double XMax = GetAnalysisXAxisMax();
	double ZMin = GetAnalysisZAxisMin();
	double ZMax = GetAnalysisZAxisMax();

	int IndexXMin = (int)(m_iBitmapWidth * (Analysis_XMIN - XMin) / (XMax - XMin));
	int IndexXMax = (int)(m_iBitmapWidth * (Analysis_XMAX - XMin) / (XMax - XMin));
	int IndexZMin = (int)(m_iBitmapHeight * (Analysis_ZMIN - ZMin) / (ZMax - ZMin));
	int IndexZMax = (int)(m_iBitmapHeight * (Analysis_ZMAX - ZMin) / (ZMax - ZMin));
	COLORREF BlankColor = RGB(255, 255, 255);

	EnterCriticalSection(&CRT_ANALYSIS_MEMORY_1);

	unsigned int MinimumY_Index = (unsigned int)(Analysis_YMIN / SystemAnalysis.dEncoderStep);
	unsigned int MaximumY_Index = (unsigned int)(min(Analysis_YMAX / SystemAnalysis.dEncoderStep, GetAnalysisEncoderPositionQuantity() - 1));

	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe1Analysis.iCycleCount - 1 ; i++)
	{
		DWORD dwT1 = GetTickCount();

		for (unsigned int j = 0 ; j < (unsigned int)InfoProbe1Analysis.PointQuantity ; j++)
		{
			int TempPosition1 = i * InfoProbe1Analysis.PointQuantity + j;
			int TempPosition2 = (i + 1) * InfoProbe1Analysis.PointQuantity + j;
			
			if ((m_pPositionProbe_Z[0][TempPosition1] >= 0) &&
				(m_pPositionProbe_Z[0][TempPosition1] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[0][TempPosition1] >= 0) &&
				(m_pPositionProbe_X[0][TempPosition1] < m_iBitmapWidth))
			{
				int Position = m_pPositionProbe_Z[0][TempPosition1] * m_iBitmapWidth + m_pPositionProbe_X[0][TempPosition1];
				
				if ((Position >= 0) && 
					(Position < m_iBitmapWidth * m_iBitmapHeight))
				{
					for (unsigned int k = MinimumY_Index ; k <= MaximumY_Index ; k++)
						m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanAnalysisMemory1[k][TempPosition1]) * 2 - 1);

					m_pUpdateBuffer[0][Position] = true;

					if ((m_pPositionProbe_X[0][TempPosition1] < IndexXMin) ||
						(m_pPositionProbe_X[0][TempPosition1] > IndexXMax) ||
						(m_pPositionProbe_Z[0][TempPosition1] < IndexZMin) ||
						(m_pPositionProbe_Z[0][TempPosition1] > IndexZMax))
						m_pBuffer[Position] = BlankColor;
					else
						m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
				}
			}

			if ((m_pPositionProbe_Z[0][TempPosition2] >= 0) &&
				(m_pPositionProbe_Z[0][TempPosition2] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[0][TempPosition2] >= 0) &&
				(m_pPositionProbe_X[0][TempPosition2] < m_iBitmapWidth))
			{
				int Position = m_pPositionProbe_Z[0][TempPosition2] * m_iBitmapWidth + m_pPositionProbe_X[0][TempPosition2];

				if ((Position >= 0) && 
					(Position < m_iBitmapWidth * m_iBitmapHeight))
				{
					for (unsigned int k = MinimumY_Index ; k <= MaximumY_Index ; k++)
						m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanAnalysisMemory1[k][TempPosition2]) * 2 - 1);

					m_pUpdateBuffer[0][Position] = true;
				
					if ((m_pPositionProbe_X[0][TempPosition2] < IndexXMin) ||
						(m_pPositionProbe_X[0][TempPosition2] > IndexXMax) ||
						(m_pPositionProbe_Z[0][TempPosition2] < IndexZMin) ||
						(m_pPositionProbe_Z[0][TempPosition2] > IndexZMax))
						m_pBuffer[Position] = BlankColor;
					else
						m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
				}
			}

			if ((m_pPositionProbe_Z[0][TempPosition1] >= 0) &&
				(m_pPositionProbe_Z[0][TempPosition1] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[0][TempPosition1] >= 0) &&
				(m_pPositionProbe_X[0][TempPosition1] < m_iBitmapWidth) &&
				(m_pPositionProbe_Z[0][TempPosition2] >= 0) && 
				(m_pPositionProbe_Z[0][TempPosition2] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[0][TempPosition2] >= 0) &&
				(m_pPositionProbe_X[0][TempPosition2] < m_iBitmapWidth))
			{
				AMP1 = m_pDataBuffer[m_pPositionProbe_Z[0][TempPosition1] * m_iBitmapWidth + m_pPositionProbe_X[0][TempPosition1]];
				AMP2 = m_pDataBuffer[m_pPositionProbe_Z[0][TempPosition2] * m_iBitmapWidth + m_pPositionProbe_X[0][TempPosition2]];

				DeltaX = m_pPositionProbe_X[0][TempPosition2] - m_pPositionProbe_X[0][TempPosition1];
				DeltaZ = m_pPositionProbe_Z[0][TempPosition2] - m_pPositionProbe_Z[0][TempPosition1];

				if (DeltaX > DeltaZ)
				{
					for (int k = 1 ; k < DeltaX ; k++)
					{
						IndexX = m_pPositionProbe_X[0][TempPosition1] + k;
						IndexZ = m_pPositionProbe_Z[0][TempPosition1] + k * DeltaZ / DeltaX;
                   
						if ((DeltaX > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaX));
							m_pUpdateBuffer[0][Position] = true;

							if ((IndexX < IndexXMin) ||
								(IndexX > IndexXMax) ||
								(IndexZ < IndexZMin) ||
								(IndexZ > IndexZMax))
								m_pBuffer[Position] = BlankColor;
							else
								m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
						}
					}
				}
				else
				{
					for (int k = 1 ; k < DeltaZ ; k++)
					{
						IndexX = m_pPositionProbe_X[0][TempPosition1] + k * DeltaX / DeltaZ;
						IndexZ = m_pPositionProbe_Z[0][TempPosition1] + k;
                   
						if ((DeltaZ > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaZ));
							m_pUpdateBuffer[0][Position] = true;

							if ((IndexX < IndexXMin) ||
								(IndexX > IndexXMax) ||
								(IndexZ < IndexZMin) ||
								(IndexZ > IndexZMax))
								m_pBuffer[Position] = BlankColor;
							else
								m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
						}
					}
				}
			}
		}

		DWORD dwT2 = GetTickCount();

		if (dwT2 != dwT1)
		{
			CString csText;
			csText.Format(_T("Delai (%d) = %d\r\n"), i, dwT2 - dwT1);	//-576
			TRACE(csText);
		}
	}

	LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_1);
}

void CBscopeView::DisplayProbe2Gate()
{
	int DeltaX, DeltaZ;
	int IndexX, IndexZ;
	BYTE AMP1, AMP2;

	double XMin = GetAnalysisXAxisMin();
	double XMax = GetAnalysisXAxisMax();
	double ZMin = GetAnalysisZAxisMin();
	double ZMax = GetAnalysisZAxisMax();

	int IndexXMin = (int)(m_iBitmapWidth * (Analysis_XMIN - XMin) / (XMax - XMin));
	int IndexXMax = (int)(m_iBitmapWidth * (Analysis_XMAX - XMin) / (XMax - XMin));
	int IndexZMin = (int)(m_iBitmapHeight * (Analysis_ZMIN - ZMin) / (ZMax - ZMin));
	int IndexZMax = (int)(m_iBitmapHeight * (Analysis_ZMAX - ZMin) / (ZMax - ZMin));
	COLORREF BlankColor = RGB(255, 255, 255);

	EnterCriticalSection(&CRT_ANALYSIS_MEMORY_2);

	unsigned int MinimumY_Index = (unsigned int)(Analysis_YMIN / SystemAnalysis.dEncoderStep);
	unsigned int MaximumY_Index = (unsigned int)min(Analysis_YMAX / SystemAnalysis.dEncoderStep, GetAnalysisEncoderPositionQuantity() - 1);

	for (unsigned int i = 0 ; i < (unsigned int)InfoProbe2Analysis.iCycleCount - 1 ; i++)
	{
		DWORD dwT1 = GetTickCount();

		for (unsigned int j = 0 ; j < (unsigned int)InfoProbe2Analysis.PointQuantity ; j++)
		{
			int TempPosition1 = i * InfoProbe2Analysis.PointQuantity + j;
			int TempPosition2 = (i + 1) * InfoProbe2Analysis.PointQuantity + j;
			
			if ((m_pPositionProbe_Z[1][TempPosition1] >= 0) &&
				(m_pPositionProbe_Z[1][TempPosition1] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[1][TempPosition1] >= 0) &&
				(m_pPositionProbe_X[1][TempPosition1] < m_iBitmapWidth))
			{
				int Position = m_pPositionProbe_Z[1][TempPosition1] * m_iBitmapWidth + m_pPositionProbe_X[1][TempPosition1];
				
				for (unsigned int k = MinimumY_Index ; k <= MaximumY_Index ; k++)
					m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanAnalysisMemory2[k][TempPosition1]) * 2 - 1);

				m_pUpdateBuffer[1][Position] = true;

				if ((m_pPositionProbe_X[1][TempPosition1] < IndexXMin) ||
					(m_pPositionProbe_X[1][TempPosition1] > IndexXMax) ||
					(m_pPositionProbe_Z[1][TempPosition1] < IndexZMin) ||
					(m_pPositionProbe_Z[1][TempPosition1] > IndexZMax))
					m_pBuffer[Position] = BlankColor;
				else
					m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
			}

			if ((m_pPositionProbe_Z[1][TempPosition2] >= 0) &&
				(m_pPositionProbe_Z[1][TempPosition2] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[1][TempPosition2] >= 0) &&
				(m_pPositionProbe_X[1][TempPosition2] < m_iBitmapWidth))
			{
				int Position = m_pPositionProbe_Z[1][TempPosition2] * m_iBitmapWidth + m_pPositionProbe_X[1][TempPosition2];

				for (unsigned int k = MinimumY_Index ; k <= MaximumY_Index ; k++)
					m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], abs(AscanAnalysisMemory2[k][TempPosition2]) * 2 - 1);

				m_pUpdateBuffer[1][Position] = true;
				
				if ((m_pPositionProbe_X[1][TempPosition2] < IndexXMin) ||
					(m_pPositionProbe_X[1][TempPosition2] > IndexXMax) ||
					(m_pPositionProbe_Z[1][TempPosition2] < IndexZMin) ||
					(m_pPositionProbe_Z[1][TempPosition2] > IndexZMax))
					m_pBuffer[Position] = BlankColor;
				else
					m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
			}

			if ((m_pPositionProbe_Z[1][TempPosition1] >= 0) &&
				(m_pPositionProbe_Z[1][TempPosition1] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[1][TempPosition1] >= 0) &&
				(m_pPositionProbe_X[1][TempPosition1] < m_iBitmapWidth) &&
				(m_pPositionProbe_Z[1][TempPosition2] >= 0) && 
				(m_pPositionProbe_Z[1][TempPosition2] < m_iBitmapHeight) && 
				(m_pPositionProbe_X[1][TempPosition2] >= 0) &&
				(m_pPositionProbe_X[1][TempPosition2] < m_iBitmapWidth))
			{
				AMP1 = m_pDataBuffer[m_pPositionProbe_Z[1][TempPosition1] * m_iBitmapWidth + m_pPositionProbe_X[1][TempPosition1]];
				AMP2 = m_pDataBuffer[m_pPositionProbe_Z[1][TempPosition2] * m_iBitmapWidth + m_pPositionProbe_X[1][TempPosition2]];

				DeltaX = m_pPositionProbe_X[1][TempPosition2] - m_pPositionProbe_X[1][TempPosition1];
				DeltaZ = m_pPositionProbe_Z[1][TempPosition2] - m_pPositionProbe_Z[1][TempPosition1];

				if (DeltaX > DeltaZ)
				{
					for (int k = 1 ; k < DeltaX ; k++)
					{
						IndexX = m_pPositionProbe_X[1][TempPosition1] + k;
						IndexZ = m_pPositionProbe_Z[1][TempPosition1] + k * DeltaZ / DeltaX;
                   
						if ((DeltaX > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaX));
							m_pUpdateBuffer[1][Position] = true;

							if ((IndexX < IndexXMin) ||
								(IndexX > IndexXMax) ||
								(IndexZ < IndexZMin) ||
								(IndexZ > IndexZMax))
								m_pBuffer[Position] = BlankColor;
							else
								m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
						}
					}
				}
				else
				{
					for (int k = 1 ; k < DeltaZ ; k++)
					{
						IndexX = m_pPositionProbe_X[1][TempPosition1] + k * DeltaX / DeltaZ;
						IndexZ = m_pPositionProbe_Z[1][TempPosition1] + k;
                   
						if ((DeltaZ > 0) && 
							(IndexX < m_iBitmapWidth) &&
							(IndexZ < m_iBitmapHeight))
						{
							int Position = IndexZ * m_iBitmapWidth + IndexX;

							m_pDataBuffer[Position] = (BYTE)max(m_pDataBuffer[Position], AMP1 + k * (AMP2 - AMP1) / (DeltaZ));
							m_pUpdateBuffer[1][Position] = true;

							if ((IndexX < IndexXMin) ||
								(IndexX > IndexXMax) ||
								(IndexZ < IndexZMin) ||
								(IndexZ > IndexZMax))
								m_pBuffer[Position] = BlankColor;
							else
								m_pBuffer[Position] = m_cPalette[m_pDataBuffer[Position]];
						}
					}
				}
			}
		}

		DWORD dwT2 = GetTickCount();

		if (dwT2 != dwT1)
		{
			CString csText;
			csText.Format(_T("Delai (%d) = %d\r\n"), i, dwT2 - dwT1);	//-576
			TRACE(csText);
		}
	}

	LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_2);
}

void CBscopeView::FillPosition(int Probe, int IndexX, int IndexZ)
{
	unsigned int Value = 0;
	unsigned int Quantity = 0;

	if ((IndexX - 1 >= 0) && 
		(IndexZ - 1 >= 0) && 
		((m_pUpdateBuffer[0][(IndexZ - 1) * m_iBitmapWidth + IndexX - 1] == true) ||
		(m_pUpdateBuffer[1][(IndexZ - 1) * m_iBitmapWidth + IndexX - 1] == true)))
	{
		Value += m_pDataBuffer[(IndexZ - 1) * m_iBitmapWidth + IndexX - 1];
		Quantity++;
	}

	if ((IndexZ - 1 >= 0) && 
		((m_pUpdateBuffer[0][(IndexZ - 1) * m_iBitmapWidth + IndexX] == true) ||
		(m_pUpdateBuffer[1][(IndexZ - 1) * m_iBitmapWidth + IndexX] == true)))
	{
		Value += m_pDataBuffer[(IndexZ - 1) * m_iBitmapWidth + IndexX];
		Quantity++;
	}

	if ((IndexX + 1 < m_iBitmapWidth) && 
		(IndexZ - 1 >= 0) && 
		((m_pUpdateBuffer[0][(IndexZ - 1) * m_iBitmapWidth + IndexX + 1] == true) ||
		(m_pUpdateBuffer[1][(IndexZ - 1) * m_iBitmapWidth + IndexX + 1] == true)))
	{
		Value += m_pDataBuffer[(IndexZ - 1) * m_iBitmapWidth + IndexX + 1];
		Quantity++;
	}

	if ((IndexX - 1 >= 0) &&
		(IndexZ < m_iBitmapHeight) &&
		((m_pUpdateBuffer[0][IndexZ * m_iBitmapWidth + IndexX - 1] == true) ||
		(m_pUpdateBuffer[1][IndexZ * m_iBitmapWidth + IndexX - 1] == true)))
	{
		Value += m_pDataBuffer[IndexZ * m_iBitmapWidth + IndexX - 1];
		Quantity++;
	}

	if ((IndexX + 1 < m_iBitmapWidth) && 
		(IndexZ < m_iBitmapHeight) &&
		((m_pUpdateBuffer[0][IndexZ * m_iBitmapWidth + IndexX + 1] == true) ||
		(m_pUpdateBuffer[1][IndexZ * m_iBitmapWidth + IndexX + 1] == true)))
	{
		Value += m_pDataBuffer[IndexZ * m_iBitmapWidth + IndexX + 1];
		Quantity++;
	}

	if ((IndexX - 1 >= 0) && 
		(IndexZ + 1 < m_iBitmapHeight) && 
		((IndexZ + 1) * m_iBitmapWidth + IndexX - 1 < m_iBitmapHeight * m_iBitmapWidth) &&
		((m_pUpdateBuffer[0][(IndexZ + 1) * m_iBitmapWidth + IndexX - 1] == true) ||
		(m_pUpdateBuffer[1][(IndexZ + 1) * m_iBitmapWidth + IndexX - 1] == true)))
	{
		Value += m_pDataBuffer[(IndexZ + 1) * m_iBitmapWidth + IndexX - 1];
		Quantity++;
	}

	if ((IndexZ + 1 < m_iBitmapHeight) && 
		((IndexZ + 1) * m_iBitmapWidth + IndexX < m_iBitmapWidth * m_iBitmapHeight) &&
		((m_pUpdateBuffer[0][(IndexZ + 1) * m_iBitmapWidth + IndexX] == true) ||
		(m_pUpdateBuffer[1][(IndexZ + 1) * m_iBitmapWidth + IndexX] == true)))
	{
		Value += m_pDataBuffer[(IndexZ + 1) * m_iBitmapWidth + IndexX];
		Quantity++;
	}

	if ((IndexX + 1 < m_iBitmapWidth) && 
		(IndexZ + 1 < m_iBitmapHeight) && 
		((m_pUpdateBuffer[0][(IndexZ + 1) * m_iBitmapWidth + IndexX + 1] == true) ||
		(m_pUpdateBuffer[1][(IndexZ + 1) * m_iBitmapWidth + IndexX + 1] == true)))
	{
		Value += m_pDataBuffer[(IndexZ + 1) * m_iBitmapWidth + IndexX + 1];
		Quantity++;
	}

	if ((Quantity > 0) && (IndexX < m_iBitmapWidth) && (IndexZ < m_iBitmapHeight))
	{
		m_pDataBuffer[IndexZ * m_iBitmapWidth + IndexX] = max(m_pDataBuffer[IndexZ * m_iBitmapWidth + IndexX], (BYTE)(Value / Quantity));
		m_pBuffer[IndexZ * m_iBitmapWidth + IndexX] = m_cPalette[m_pDataBuffer[IndexZ * m_iBitmapWidth + IndexX]];
		
		m_pUpdateBuffer[Probe][IndexZ * m_iBitmapWidth + IndexX] = true;
	}
}

void CBscopeView::Display()
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

void CBscopeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);
}