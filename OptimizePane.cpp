#include "stdafx.h"
#include "USLScanner.h"
#include "OptimizePane.h"


IMPLEMENT_DYNAMIC(COptimizePane, CDockablePane)

COptimizePane::COptimizePane()
{
}


COptimizePane::~COptimizePane()
{
}
BEGIN_MESSAGE_MAP(COptimizePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


int COptimizePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	DWORD dwViewStyle = WS_CHILD | WS_VISIBLE;
	dwViewStyle += 7;

	if (!m_staticView.Create(NULL, dwViewStyle, rectDummy, this, ID_STATIC_VIEW))
	{
		TRACE0("Failed to create Static View in profiles import \n");
		return -1;      // fail to create
	}

	AdjustLayout();

	return 0;
}


void COptimizePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
	Invalidate(false);

}

void COptimizePane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL) return;

	CRect rectClient;
	GetClientRect(rectClient);

	int nTopBorder, nBottomBorder, nLeftBorder, nRightBorder;
	int nBorderDepth = 20;
	CRect rrClient;
	GetClientRect(rrClient);

	nTopBorder = nBottomBorder = nLeftBorder = nRightBorder = 0;
	int cyTlb = 0;

	m_staticView.SetWindowPos(NULL, rrClient.left, rrClient.top + cyTlb, rrClient.Width(), rrClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

void COptimizePane::OnChangeVisualStyle()
{
}


void COptimizePane::OnPaint()
{
	CPaintDC dc(this);
	CRect rrClip, rr;
	CDC  dcMem;
	CBitmap bmp;
	CRgn rgn;

	if (GetSafeHwnd() == NULL) return;
	if (m_staticView.GetSafeHwnd() == NULL) return;

	CPaintDC* pDC = (CPaintDC*)m_staticView.GetDC();

	m_staticView.GetWindowRect(rr);
	rr -= rr.TopLeft();

	pDC->GetClipBox(&rrClip);
	rgn.CreateRectRgn(rr.left, rr.top, rr.right, rr.bottom);
	pDC->SelectClipRgn(&rgn);

	dcMem.CreateCompatibleDC(pDC);

	dcMem.SetBkMode(TRANSPARENT);

	bmp.CreateCompatibleBitmap(pDC, rr.Width(), rr.Height());
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CBrush brush(theApp.m_LastSettings.rgbBKPositionPane);
	CBrush* pOldBrush = dcMem.SelectObject(&brush);
	CPen pen(PS_SOLID, 1, GetSysColor(COLOR_MENU));
	CPen* pOldPen = dcMem.SelectObject(&pen);
	dcMem.Rectangle(rr);
	dcMem.SetTextColor(theApp.m_LastSettings.rgbTextPositionPane);

	if (m_nPaintMode == 0) {
		RenderCrissCross(&dcMem);
	}
	else {
		switch (theApp.m_LastSettings.nPeakOrthogonalMode) {
		case 0: RenderCrissCross(&dcMem);
			break;
		case 1: //RenderRectilinear(&dcMem);
			break;
		}
	}

	pDC->BitBlt(rr.left, rr.top, rr.Width(), rr.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	rgn.SetRectRgn(&rrClip);
	dc.SelectClipRgn(&rgn);

	dcMem.SelectObject(pOldPen);
	dcMem.SetBkMode(OPAQUE);
	dcMem.SelectObject(pOldBrush);

	m_staticView.ReleaseDC(pDC);

}


void COptimizePane::TransferAndRender(OptimizePaneMessage *pMsg)
{
	switch (pMsg->nDirection) {
	case HORIZONTAL: m_CpHorizontal.Zero();
		m_CpHorizontal = pMsg->pLine;
		m_CpHorizontalCentre = *pMsg->pCp;
		break;
	case VERTICAL: m_CpVertical.Zero();
		m_CpVertical = pMsg->pLine;
		m_CpVerticalCentre = *pMsg->pCp;
		break;
	}
	Invalidate(false);
}


void COptimizePane::RenderCrissCross(CDC* pDC)
{
	CString m_Axis[] = { L"X Left",L"Y Left",L"Z Left",L"Xt Left",L"Zt Left",L"R Left",L"I Left",L"J Left",L"K Left",
		L"X Right",L"Y Right",L"Z Right",L"Xt Right",L"Zt Right",L"R Right",L"I Right",L"J Right",L"K Right" };

	CRect rr;
	CBrush Brush(RGB(255, 255, 255));
	CPen* pOldPen;
	CPen penRed(PS_SOLID, 1, RGB(255, 0, 0));
	CPen penBlue(PS_SOLID, 1, RGB(0, 0, 255));
	CPen penGrey(PS_SOLID, 1, RGB(50, 50, 50));
	int	pnx, pny, ii, nMin, nMax;
	float f0, f1;
	CSize size;
	CString Buff;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));

	LOGFONT lf;
	memset((void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 10;
	CFont Font;
	CFont* pOldFont;
	Font.CreateFontIndirect(&lf);

	Buff = "Hello";
	size = pDC->GetTextExtent(Buff);

	pOldPen = pDC->SelectObject(&penRed);
	pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.top = rr.bottom;
	rr.bottom = rr.top + 3 + size.cy;
	rr.left -= size.cx;
	rr.right += size.cx;
	pDC->FillRect(rr, &brushBK);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2, 2);
	pDC->FillRect(&rr, &Brush);

	//Horizontal
	if (m_CpHorizontal.m_nCoordL > 0) {
		pOldPen = pDC->SelectObject(&penRed);
		for (ii = 0; ii < m_CpHorizontal.m_nCoordL; ii++) {
			pnx = MulDiv(ii, rr.Width(), m_CpHorizontal.m_nCoordL - 1) + rr.left;
			pny = rr.bottom - MulDiv(m_CpHorizontal.m_pCp[ii].nAmp, rr.Height(), 127);
			if (ii == 0) {
				pDC->MoveTo(pnx, pny);
			}
			else {
				pDC->LineTo(pnx, pny);
			}
		}

		f0 = m_CpHorizontal.m_pCp[0].fPos[m_CpHorizontal.m_nPrimaryAxis];
		f1 = m_CpHorizontal.m_pCp[m_CpHorizontal.m_nCoordL - 1].fPos[m_CpHorizontal.m_nPrimaryAxis];
		nMin = (int)f0;
		nMax = (int)f1;
		/*
				pDC->SelectObject(penGrey);
				Buff=m_Axis[m_CpHorizontal.m_nPrimaryAxis];
				size = pDC->GetTextExtent(Buff);
				pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.bottom-size.cy,Buff);

				fMin = (float)nMin;
				fMax = (float)nMax;
				for(fStep=fMin;fStep<=fMax;fStep+=5.0) {
					pnx = (int)((fStep-f0) * (float)rr.Width() / (f1-f0)) + rr.left;
					pDC->MoveTo(pnx,rr.bottom+2);
					pDC->LineTo(pnx,rr.bottom+4);
					Buff.Format(_T("%.1f"),fStep);
					size = pDC->GetTextExtent(Buff);
					pDC->TextOut(pnx-size.cx/2,rr.bottom+3,Buff);
				}
				*/
		pDC->SelectObject(&penRed);
		pnx = (int)((m_CpHorizontalCentre.fPos[m_CpHorizontal.m_nPrimaryAxis] - f0) * (float)rr.Width() / (f1 - f0)) + rr.left;
		pnx = rr.left + MulDiv(m_CpHorizontal.m_nMaxPtr, rr.Width(), m_CpHorizontal.m_nCoordL - 1);
		pDC->MoveTo(pnx, rr.top);
		pDC->LineTo(pnx, rr.bottom);
	}

	//Vertical
	if (m_CpVertical.m_nCoordL > 0) {
		pOldPen = pDC->SelectObject(&penBlue);
		for (ii = 0; ii < m_CpVertical.m_nCoordL; ii++) {
			pny = rr.bottom - MulDiv(ii, rr.Height(), m_CpVertical.m_nCoordL - 1);
			pnx = rr.left + MulDiv(m_CpVertical.m_pCp[ii].nAmp, rr.Width(), 127);
			if (ii == 0) {
				pDC->MoveTo(pnx, pny);
			}
			else {
				pDC->LineTo(pnx, pny);
			}
		}

		f0 = m_CpVertical.m_pCp[0].fPos[m_CpVertical.m_nPrimaryAxis];
		f1 = m_CpVertical.m_pCp[m_CpVertical.m_nCoordL - 1].fPos[m_CpVertical.m_nPrimaryAxis];
		nMin = (int)f0;
		nMax = (int)f1;
		/*
		pDC->SelectObject(penGrey);
		Buff=m_Axis[m_CpVertical.m_nPrimaryAxis];
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.bottom-size.cy,Buff);

		fMin = (float)nMin;
		fMax = (float)nMax;
		for(fStep=fMin;fStep<=fMax;fStep+=5.0) {
			pnx = (int)((fStep-f0) * (float)rr.Width() / (f1-f0)) + rr.left;
			pDC->MoveTo(pnx,rr.bottom+2);
			pDC->LineTo(pnx,rr.bottom+4);
			Buff.Format(_T("%.1f"),fStep);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(pnx-size.cx/2,rr.bottom+3,Buff);
		}
		*/

		pDC->SelectObject(&penBlue);
		//		pny= rr.bottom - (int)((m_CpVerticalCentre.fPos[m_CpVertical.m_nPrimaryAxis] - f0) * (float)rr.Height() / (f1-f0));
		pny = rr.bottom - MulDiv(m_CpVertical.m_nMaxPtr, rr.Height(), m_CpVertical.m_nCoordL - 1);
		pDC->MoveTo(rr.left, pny);
		pDC->LineTo(rr.right, pny);
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

}
