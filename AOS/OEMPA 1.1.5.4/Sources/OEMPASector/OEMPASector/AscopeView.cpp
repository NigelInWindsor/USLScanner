// AscopeView.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "AscopeView.h"


// CAscopeView

IMPLEMENT_DYNCREATE(CAscopeView, CView)

CAscopeView::CAscopeView()
{
	g_pAscope = this;

	m_clrBackground[0] = RGB(198, 217, 241);
	m_clrBackground[1] = RGB(255, 255, 255);
	m_clrGrid = RGB(0, 0, 0);
	m_clrCurve = RGB(0, 0, 255);
	m_clrText = RGB(0, 0, 0);

	m_iCycle = 0;
	m_iProbe = 0;
	m_iDisplayMode = 0;

	m_cBrushBackground[0].CreateSolidBrush(m_clrBackground[0]);
	m_cBrushBackground[1].CreateSolidBrush(m_clrBackground[1]);
	m_cPenGrid[0].CreatePen(PS_SOLID, 1, m_clrGrid);
	m_cPenGrid[1].CreatePen(PS_DOT, 1, m_clrGrid);
	m_cPenCurve.CreatePen(PS_SOLID, 1, m_clrCurve);

	m_cFontText.CreateFont(12, 
						   0, 
						   0, 
						   0, 
						   FW_NORMAL, 
						   FALSE, 
						   FALSE, 
						   0, 
						   ANSI_CHARSET, 
						   OUT_DEFAULT_PRECIS, 
						   CLIP_DEFAULT_PRECIS, 
						   DEFAULT_QUALITY, 
						   DEFAULT_PITCH | FF_SWISS, 
						   _T(""));

	m_pAscan = NULL;
}

CAscopeView::~CAscopeView()
{
	g_pAscope = NULL;

	m_cBrushBackground[0].DeleteObject();
	m_cBrushBackground[1].DeleteObject();
	m_cPenGrid[0].DeleteObject();
	m_cPenGrid[1].DeleteObject();
	m_cPenCurve.DeleteObject();
	m_cFontText.DeleteObject();
}

BEGIN_MESSAGE_MAP(CAscopeView, CView)
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CAscopeView drawing

void CAscopeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	pDoc->SetTitle(_T("A-scan"));
}

// CAscopeView diagnostics

// CAscopeView message handlers

void CAscopeView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ASCOPE_VIEW)
		RefreshAscanDisplay();

	CView::OnTimer(nIDEvent);
}

CRect CAscopeView::GetAscanRect()
{
	CRect rect;
	GetClientRect(&rect);

	CRect rectangle;
	rectangle.left = rect.left + 80;
	rectangle.right = rect.right - 15;
	rectangle.top = rect.top + 50;
	rectangle.bottom = rect.bottom - 15;

	return rectangle;
}

void CAscopeView::RefreshAscanDisplay()
{
	EnterCriticalSection(&CRT_UPDATE_SETTING);
	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap memBit;
	CBitmap* pOldBit = NULL;
	memBit.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBit = memDC.SelectObject(&memBit);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);
	
	DrawBackground(&memDC);
	DrawGrid(&memDC);
	DrawScale(&memDC);
	DrawCurve(&memDC);
	
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBit);
	memBit.DeleteObject();
	memDC.DeleteDC();
	LeaveCriticalSection(&CRT_UPDATE_SETTING);
}

void CAscopeView::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush* pOldBrush = pDC->SelectObject(&m_cBrushBackground[0]);
	pDC->Rectangle(&rect);
	pDC->SelectObject(pOldBrush);
}

void CAscopeView::DrawGrid(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->SetBkColor(m_clrBackground[1]);

	CRect rectangle;
	rectangle.left = rect.left + 80;
	rectangle.right = rect.right - 15;
	rectangle.top = rect.top + 50;
	rectangle.bottom = rect.bottom - 15;

	CPen* pOldPen = pDC->SelectObject(&m_cPenGrid[0]);
	pDC->Rectangle(rectangle);

	pDC->SelectObject(&m_cPenGrid[1]);

	for (int i = 0 ; i < 9 ; i++)
	{
		POINT pt;
		
		pt.x = rectangle.left + (i + 1) * (rectangle.right - rectangle.left) / 10;
		pt.y = rectangle.top;
		pDC->MoveTo(pt);
		pt.y = rectangle.bottom;
		pDC->LineTo(pt);

		if (i < 7)
		{
			pt.x = rectangle.left;
			pt.y = rectangle.top + (i + 1) * (rectangle.bottom - rectangle.top) / 8;
			pDC->MoveTo(pt);
			pt.x = rectangle.right;
			pDC->LineTo(pt);
		}
	}

	pDC->SelectObject(pOldPen);
}

void CAscopeView::DrawScale(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CPen* pOldPen = pDC->SelectObject(&m_cPenGrid[0]);
	CFont* pOldFont = pDC->SelectObject(&m_cFontText);
	COLORREF clrText = pDC->SetTextColor(m_clrText);
	pDC->SetBkColor(m_clrBackground[0]);

	CRect rectangle;
	rectangle.left = rect.left + 80;
	rectangle.right = rect.right - 15;
	rectangle.top = rect.top + 50;
	rectangle.bottom = rect.bottom - 15;

	POINT pt;

	CString csText;
	double dRange=0.0;

	if (m_iProbe == 0)
		dRange = InfoProbe1.PointQuantity * System.dDigitizingPeriod * 1000000;
	else if (m_iProbe == 1)
		dRange = InfoProbe2.PointQuantity * System.dDigitizingPeriod * 1000000;

	// Vertical bar
	pt.x = rect.left + 20;
	pt.y = rectangle.top;
	pDC->MoveTo(pt);
	pt.y = rectangle.bottom;
	pDC->LineTo(pt);

	// Horizontal bar
	pt.x = rectangle.left;
	pt.y = rect.top + 5;
	pDC->MoveTo(pt);
	pt.x = rectangle.right;
	pDC->LineTo(pt);

	// Horizontal scale
	for (int i = 0 ; i < 10 ; i++)
	{
		int Start = rectangle.left + i * (rectangle.right - rectangle.left) / 10;
		pt.x = Start;
		pt.y = rect.top + 5;
		pDC->MoveTo(pt);
		pt.y += 15;
		pDC->LineTo(pt);

		csText.Format(_T("%.1f us"), (double)i * dRange / 10.0);

		if (i == 0)
			pDC->TextOutW(pt.x, pt.y + 5, csText);
		else
			pDC->TextOutW(pt.x - pDC->GetTextExtent(csText).cx / 2, pt.y + 5, csText);

		int End = rectangle.left + (i + 1) * (rectangle.right - rectangle.left) / 10;
		
		if (i == 9)
		{
			pt.x = End;
			pt.y = rect.top + 5;
			pDC->MoveTo(pt);
			pt.y += 15;
			pDC->LineTo(pt);

			csText.Format(_T("%.1f us"), (double)(i + 1) * dRange / 10.0);
			pDC->TextOutW(pt.x - pDC->GetTextExtent(csText).cx, pt.y + 5, csText);
		}

		for (int j = 0 ; j < 9 ; j++)
		{
			pt.x = Start + (j + 1) * (End - Start) /  10;
			pt.y = rect.top + 5;
			pDC->MoveTo(pt);
			pt.y += 8;
			pDC->LineTo(pt);
		}
	}

	// Vertical scale
	for (int i = 0 ; i < 8 ; i++)
	{
		int Start = rectangle.top + i * (rectangle.bottom - rectangle.top) / 8;
		pt.x = rect.left + 20;
		pt.y = Start;
		pDC->MoveTo(pt);
		pt.x += 15;
		pDC->LineTo(pt);

		csText.Format(_T("%.1f"), 100.0 - (double)i * 25.0);
		csText += _T(" %");
		pDC->TextOutW(pt.x + 5, pt.y - pDC->GetTextExtent(csText).cy / 2, csText);

		int End = rectangle.top + (i + 1) * (rectangle.bottom - rectangle.top) / 8;
		
		if (i == 7)
		{
			pt.x = rect.left + 20;
			pt.y = End;
			pDC->MoveTo(pt);
			pt.x += 15;
			pDC->LineTo(pt);

			csText = _T("-100.0");
			csText += _T(" %");

			pDC->TextOutW(pt.x + 5, pt.y - pDC->GetTextExtent(csText).cy / 2, csText);
		}

		for (int j = 0 ; j < 7 ; j++)
		{
			pt.y = Start + (j + 1) * (End - Start) /  8;
			pt.x = rect.left + 20;
			pDC->MoveTo(pt);
			pt.x += 8;
			pDC->LineTo(pt);
		}
	}

	pDC->SetTextColor(clrText);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}

void CAscopeView::DrawCurve(CDC* pDC)
{
	CRect rect = GetAscanRect();
	CPen* pOldPen = pDC->SelectObject(&m_cPenCurve);
	double dWidth = static_cast<double>(rect.Width());
	double dHeight = static_cast<double>(rect.Height());

	unsigned int PointQuantity = 0;
	unsigned int CycleQuantity = 0;

	if(!AscanMemory1)//integration
		return;
	if (m_iProbe == 0)
	{
		PointQuantity = InfoProbe1.PointQuantity;
		CycleQuantity = InfoProbe1.iCycleCount;
	}
	else if (m_iProbe == 1)
	{
		PointQuantity = InfoProbe2.PointQuantity;
		CycleQuantity = InfoProbe2.iCycleCount;
	}

	POINT* pPoint = new POINT[PointQuantity];

	if (m_pAscan)
		delete[] m_pAscan;

	m_pAscan = new char[PointQuantity];
	memset(m_pAscan, 0, PointQuantity);

	for (unsigned int i = 0 ; i < PointQuantity ; i++)
	{
		pPoint[i].x = rect.left + (LONG)(i * dWidth / PointQuantity);

		if (m_iDisplayMode == 0)		// We display only one cycle
		{
			if (m_iProbe == 0)
				m_pAscan[i] = AscanMemory1[g_LastAscanEncoderPosition][m_iCycle * PointQuantity + i];
			else if (m_iProbe == 1)
				m_pAscan[i] = AscanMemory2[g_LastAscanEncoderPosition][m_iCycle * PointQuantity + i];

			if (m_pAscan[i] == 127)
				pPoint[i].y = rect.top;
			else
				pPoint[i].y = rect.top + (LONG)(dHeight * 0.5 * (1.0 - m_pAscan[i] / 128.0));
		}
		else if (m_iDisplayMode == 1)	// We do a max process over all cycles of the probe
		{
			for (int j = 0 ; j < (int)CycleQuantity ; j++)
			{
				char data[2] = {0, 0};

				if (m_iProbe == 0)
					data[0] = AscanMemory1[g_LastAscanEncoderPosition][j * PointQuantity + i];
				else if (m_iProbe == 1)
					data[0] = AscanMemory2[g_LastAscanEncoderPosition][j * PointQuantity + i];

				data[1] = m_pAscan[i];

				if (abs(data[0]) > abs(data[1]))
					m_pAscan[i] = data[0];
				else
					m_pAscan[i] = data[1];

				if (m_pAscan[i] == 127)
					pPoint[i].y = rect.top;
				else
					pPoint[i].y = rect.top + (LONG)(dHeight * 0.5 * (1.0 - m_pAscan[i] / 128.0));
			}
		}
	}

	pDC ->Polyline(pPoint, PointQuantity);
	
	delete[] pPoint;
	pDC->SelectObject(pOldPen);
}

int CAscopeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(TIMER_ASCOPE_VIEW, 33, NULL);

	return 0;
}
