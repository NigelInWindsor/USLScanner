// DynamicNCView.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "DynamicNCView.h"
#include <math.h>

// CDynamicNCView

#define _PROFILING

IMPLEMENT_DYNCREATE(CDynamicNCView, CView)

CDynamicNCView::CDynamicNCView()
{
	g_pDynamicNCView = this;

	m_pBuffer[0] = NULL;
	m_pDataBuffer[0] = NULL;

	m_pBuffer[1] = NULL;
	m_pDataBuffer[1] = NULL;

	CreatePalette();
	m_bResetConfiguration = true;
}

CDynamicNCView::~CDynamicNCView()
{
	g_pDynamicNCView = NULL;

	if (m_pBuffer[0])
		delete[] m_pBuffer[0];

	if (m_pDataBuffer[0])
		delete[] m_pDataBuffer[0];

	if (m_pBuffer[1])
		delete[] m_pBuffer[1];

	if (m_pDataBuffer[1])
		delete[] m_pDataBuffer[1];
}

BEGIN_MESSAGE_MAP(CDynamicNCView, CView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CDynamicNCView drawing

void CDynamicNCView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	pDoc->SetTitle(_T("Dynamic View - Non Corrected"));
}

// CDynamicNCView message handlers

int CDynamicNCView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(TIMER_DYNAMIC_NON_CORRECTED_VIEW, 33, NULL);

	return 0;
}

void CDynamicNCView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_DYNAMIC_NON_CORRECTED_VIEW)
		Display();

	CView::OnTimer(nIDEvent);
}

void CDynamicNCView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_iViewWidth = cx;
	m_iViewHeight = cy;
}

void CDynamicNCView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CView::OnSizing(fwSide, pRect);
}

void CDynamicNCView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

void CDynamicNCView::CreatePalette()
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

void CDynamicNCView::DisplayProbe1()
{
	bool bError=false;//integration
	m_iBitmapWidth = g_uiCycleQuantity[0];
	m_iBitmapHeight = g_uiPointQuantity[0];

	char* TempAscanMemory = new char[g_uiPointQuantity[0] * g_uiCycleQuantity[0]];
	EnterCriticalSection(&CRT_MEMORY_1);
	if(AscanMemory1)
		memcpy(TempAscanMemory, AscanMemory1[g_LastAscanEncoderPosition], g_uiPointQuantity[0] * g_uiCycleQuantity[0]);
	else
		bError = true;
	LeaveCriticalSection(&CRT_MEMORY_1);
	if(bError)
		return;

	for (unsigned int i = 0 ; i < g_uiCycleQuantity[0] ; i++)
	{
		for (unsigned int j = 0 ; j < g_uiPointQuantity[0] ; j++)
		{
			m_pDataBuffer[0][j * m_iBitmapWidth + i] = (BYTE)max(0, abs(TempAscanMemory[i * m_iBitmapHeight + j]) * 2 - 1);
			m_pBuffer[0][j * m_iBitmapWidth + i] = m_cPalette[m_pDataBuffer[0][j * m_iBitmapWidth + i]];
		}
	}

	delete[] TempAscanMemory;
}

void CDynamicNCView::DisplayProbe2()
{
	m_iBitmapWidth = g_uiCycleQuantity[1];
	m_iBitmapHeight = g_uiPointQuantity[1];

	char* TempAscanMemory = new char[g_uiPointQuantity[1] * g_uiCycleQuantity[1]];
	EnterCriticalSection(&CRT_MEMORY_2);
	memcpy(TempAscanMemory, AscanMemory2[g_LastAscanEncoderPosition], g_uiPointQuantity[1] * g_uiCycleQuantity[1]);
	LeaveCriticalSection(&CRT_MEMORY_2);

	for (unsigned int i = 0 ; i < g_uiCycleQuantity[1] ; i++)
	{
		for (unsigned int j = 0 ; j < g_uiPointQuantity[1] ; j++)
		{
			m_pDataBuffer[1][j * m_iBitmapWidth + i] = (BYTE)max(0, abs(TempAscanMemory[i * m_iBitmapHeight + j]) * 2 - 1);
			m_pBuffer[1][j * m_iBitmapWidth + i] = m_cPalette[m_pDataBuffer[1][j * m_iBitmapWidth + i]];
		}
	}

	delete[] TempAscanMemory;
}

void CDynamicNCView::Display()
{
	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	EnterCriticalSection(&CRT_UPDATE_SETTING);

	if (m_bResetConfiguration)
	{
		if (m_pBuffer[0])
			delete[] m_pBuffer[0];

		if (m_pDataBuffer[0])
			delete[] m_pDataBuffer[0];

		if (m_pBuffer[1])
			delete[] m_pBuffer[1];

		if (m_pDataBuffer[1])
			delete[] m_pDataBuffer[1];

		m_pBuffer[0] = new COLORREF[g_uiCycleQuantity[0] * g_uiPointQuantity[0]];
		m_pDataBuffer[0] = new BYTE[g_uiCycleQuantity[0] * g_uiPointQuantity[0]];

		m_pBuffer[1] = new COLORREF[g_uiCycleQuantity[1] * g_uiPointQuantity[1]];
		m_pDataBuffer[1] = new BYTE[g_uiCycleQuantity[1] * g_uiPointQuantity[1]];
	}

	if (g_uiProbe == 0)
		DisplayProbe1();
	else if (g_uiProbe == 1)
		DisplayProbe2();

	if ((m_iBitmapWidth > 0) && (m_iBitmapHeight > 0))
	{
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

		if (g_uiProbe == 0)
			StretchDIBits(dc.m_hDC, 
						0, m_iViewHeight-1, m_iViewWidth, -(int)m_iViewHeight, 
						0, 0, m_iBitmapWidth, m_iBitmapHeight, 
						m_pBuffer[0], pBmpInfo, DIB_RGB_COLORS, SRCCOPY);
		else if (g_uiProbe == 1)
			StretchDIBits(dc.m_hDC, 
						0, m_iViewHeight-1, m_iViewWidth, -(int)m_iViewHeight, 
						0, 0, m_iBitmapWidth, m_iBitmapHeight, 
						m_pBuffer[1], pBmpInfo, DIB_RGB_COLORS, SRCCOPY);

		delete[] pBmpInfo;
	}

	LeaveCriticalSection(&CRT_UPDATE_SETTING);
}


void CDynamicNCView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);
}
