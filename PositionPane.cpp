// PositionPane.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PositionPane.h"
#include "MainFrm.h"

// CPositionPane

IMPLEMENT_DYNAMIC(CPositionPane, CDockablePane)

CPositionPane::CPositionPane()
{
	m_pCell = NULL;
	m_nRows = 0;
	m_nColumns = 0;
	m_nToolOption = 0;
	m_pFont = NULL;
}

CPositionPane::~CPositionPane()
{
	SAFE_DELETE_ARRAY(m_pCell);
	SAFE_DELETE(m_pFont);
}


BEGIN_MESSAGE_MAP(CPositionPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POSITION_PROPERTIES, &CPositionPane::OnPositionProperties)
END_MESSAGE_MAP()

// CPositionPane message handlers

int CPositionPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
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


void CPositionPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	KillTimer(1);
	AdjustLayout();
	SetTimer(1, 400, NULL);
}

void CPositionPane::AdjustLayout()
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

void CPositionPane::OnChangeVisualStyle()
{
}

void CPositionPane::OnTimer(UINT_PTR nIDEvent)
{
	Invalidate(false);

	CDockablePane::OnTimer(nIDEvent);
}


void CPositionPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
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

	switch (m_nToolOption) {
	default:	RenderDualXYZijk(&dcMem, rr);
		break;
	case 999:
		break;
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

void CPositionPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDockablePane::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE) {
		SetTimer(1, 400, NULL);
	}
	else {
		KillTimer(1);
	}
}


CFont* CPositionPane::SetRowsColumnsFonts(CDC* pDC, int nRows, int nColumns, CString strLargestStr, float *fWidths)
{
	CFont *pOldFont = NULL;

	SetRowsColumns(nRows, nColumns, fWidths);

	//Calculate size of font
	LOGFONT lf = theApp.m_LastSettings.lfPositionPane;
	for (lf.lfHeight = 100; lf.lfHeight > 8; lf.lfHeight--) {
		SAFE_DELETE(m_pFont);
		m_pFont = new CFont();
		m_pFont->CreateFontIndirectW(&lf);
		pOldFont = pDC->SelectObject(m_pFont);
		CSize sizeAxis = pDC->GetTextExtent(strLargestStr);
		m_nBorder = sizeAxis.cy / 2;

		if (((sizeAxis.cx + m_nBorder) < m_pCell[0].rect.Width()) && (sizeAxis.cy < m_pCell[0].rect.Height())) {
			break;
		}
	}

	return pOldFont;

}

void CPositionPane::SetRowsColumns(int nRows, int nColumns,float *fWidths)
{
	CRect rr;
	m_staticView.GetWindowRect(rr);
	rr -= rr.TopLeft();

	SAFE_DELETE_ARRAY(m_pCell);
	m_nRows = nRows;
	m_nColumns = nColumns;

	m_pCell = new CCellStruct[m_nRows * m_nColumns];

	int nIndex;
	int nColumnWidth = MulDiv(1, rr.Width(), m_nColumns);
	int nColumnHeight = MulDiv(1, rr.Height(), m_nRows);
	for (int nRow = 0; nRow < m_nRows; nRow++) {
		int nPnY = MulDiv(nRow, rr.Height(), m_nRows);
		int nPnX = 0;
		float fLeft = 0.0f;
		float fRight = 0.0f;
		for (int nColumn = 0; nColumn < m_nColumns; nColumn++) {
			nIndex = nRow * m_nColumns + nColumn;

			m_pCell[nIndex].setColours(theApp.m_LastSettings.rgbTextPositionPane, theApp.m_LastSettings.rgbBKPositionPane);

			if (fWidths == NULL) {
				nPnX = MulDiv(nColumn, rr.Width(), m_nColumns);
				m_pCell[nIndex].rect.TopLeft() = CPoint(nPnX, nPnY);
				m_pCell[nIndex].rect.BottomRight() = m_pCell[nIndex].rect.TopLeft() + CPoint(nColumnWidth, nColumnHeight);
			}
			else {
				m_pCell[nIndex].rect.TopLeft() = CPoint(nPnX, nPnY);
				fRight += (float)rr.Width() * fWidths[nColumn];
				nPnX = (int)fRight;
				m_pCell[nIndex].rect.BottomRight() = CPoint(nPnX, nPnY + nColumnHeight);
			}
		}
	}
}

void CPositionPane::SetRowColumnWidth(int nRow, int nColumn, float fWidth)
{
	CRect rr;
	m_staticView.GetWindowRect(rr);
	rr -= rr.TopLeft();

	int nIndex = nRow * m_nColumns;
	int nPnX = 0;
	float fRight = 0.0f;
	if (nColumn > 0) fRight = (float)m_pCell[nIndex + (nColumn - 1)].rect.right;
	fRight += (float)rr.Width() * fWidth;
	m_pCell[nIndex + nColumn].rect.right = (int)fRight;
	for(++nColumn; nColumn<m_nColumns;nColumn++) {
		if (m_pCell[nIndex + nColumn].rect.left < (int)fRight) m_pCell[nIndex + nColumn].rect.left = (int)fRight;
	}
}

void CPositionPane::TextAt(CDC* pDC, int nRow, int nColumn, CString Buff, int nJustify)
{
	int nPnX, nPnY;
	CSize size;
	CBrush bruskBk(m_pCell[nRow * m_nColumns + nColumn].rgbBk);
	CBrush* pOldBrush = pDC->SelectObject(&bruskBk);
	CRect rrClip,rr;
	CRgn rgn;

	int nIndex = nRow * m_nColumns + nColumn;

	pDC->Rectangle(m_pCell[nIndex].rect);

	size = pDC->GetTextExtent(Buff);

	switch (nJustify) {
	default:		nPnX = m_pCell[nIndex].rect.left + m_nBorder;
		break;
	case ES_RIGHT:	nPnX = m_pCell[nIndex].rect.right - m_nBorder - size.cx;
		break;
	case ES_CENTER:	nPnX = m_pCell[nIndex].rect.CenterPoint().x - size.cx / 2;
		break;
	}
	nPnY = m_pCell[nIndex].rect.CenterPoint().y - size.cy / 2;

	pDC->GetClipBox(&rrClip);
	rgn.CreateRectRgn(m_pCell[nIndex].rect.left, m_pCell[nIndex].rect.top, m_pCell[nIndex].rect.right, m_pCell[nIndex].rect.bottom);
	pDC->SelectClipRgn(&rgn);

	pDC->TextOutW(nPnX, nPnY, Buff);

	rgn.SetRectRgn(&rrClip);
	pDC->SelectClipRgn(&rgn);

	pDC->SelectObject(pOldBrush);
}

void CPositionPane::TextAt(CDC* pDC, int nRow, int nColumn, int nValue, int nJustify)
{
	CString Buff;

	Buff.Format(L"%d", nValue);
	TextAt(pDC, nRow, nColumn, Buff, nJustify);
}

void CPositionPane::TextAt(CDC* pDC, int nRow, int nColumn, float fValue, int nJustify)
{
	CString Buff;

	Buff.Format(L"%f", fValue);
	TextAt(pDC, nRow, nColumn, Buff, nJustify);
}

void CPositionPane::RenderGrid(CDC* pDC, COLORREF rgb)
{
	CRect rr;
	CPen* pPen = new CPen(PS_SOLID, 1, rgb);
	CPen* pOldPen = pDC->SelectObject(pPen);

	m_staticView.GetWindowRect(rr);
	rr -= rr.TopLeft();

	for (int nColumn = 1; nColumn < m_nColumns; nColumn++) {
		pDC->MoveTo(m_pCell[nColumn].rect.left, rr.top);
		pDC->LineTo(m_pCell[nColumn].rect.left, rr.bottom);
	}
	for (int nRow = 1; nRow < m_nRows; nRow++) {
		pDC->MoveTo(rr.left, m_pCell[nRow * m_nColumns].rect.top);
		pDC->LineTo(rr.right, m_pCell[nRow * m_nColumns].rect.top);
	}

	pDC->SelectObject(pOldPen);
	delete pPen;
}

void CPositionPane::RenderDualXYZijk(CDC *pDC, CRect rect)
{
	float fWidths[4] = { 0.125f, 0.375f, 0.125f, 0.375f };
	int nRowRot, nSide;
	CCoord CpSurface;
	CString Buff,Theta;
	D3DXQUATERNION qNormal;

	Theta.Format(L"\x03b8");

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	int nColumns = 0;
	int nRows = 4;
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_SIDE_0) {
		nColumns += 2;
	}
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_SIDE_1) {
		nColumns += 2;
	}
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_NORM) nRows += 3;
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_A) nRows += 1;
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_B) nRows += 1;
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_ROT_NORM) nRows += 1;
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_QUAT) nRows += 4;
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_R0 || theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_R1) {
		nRowRot = nRows;
		nRows += 1;
	}
	if (nColumns == 2) {
		fWidths[0] = 0.25f;
		fWidths[1] = 0.75f;
	}

	CFont *pOldFont = SetRowsColumnsFonts(pDC, nRows, nColumns, L"XXXX", fWidths);

	int nColumn = 0;
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_SIDE_0) {
		theApp.m_nSide0Orientation == 0 ? nSide = 0 : nSide = 1;
		int nRow=0;
		TextAt(pDC, nRow, nColumn, theApp.m_DlgSideName[nSide], ES_CENTER);
		TextAt(pDC, nRow, nColumn+1, L" ", ES_CENTER);
		nRow++;
		TextAt(pDC, nRow++, nColumn, L"X", ES_CENTER);
		TextAt(pDC, nRow++, nColumn, L"Y", ES_CENTER);
		TextAt(pDC, nRow++, nColumn, L"Z", ES_CENTER);
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_NORM) {
			TextAt(pDC, nRow++, nColumn, L"i", ES_CENTER);
			TextAt(pDC, nRow++, nColumn, L"j", ES_CENTER);
			TextAt(pDC, nRow++, nColumn, L"k", ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_A) {
			TextAt(pDC, nRow++, nColumn, L"A", ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_B) {
			TextAt(pDC, nRow++, nColumn, L"B", ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_ROT_NORM) {
			TextAt(pDC, nRow++, nColumn, Theta, ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_QUAT) {
			TextAt(pDC, nRow++, nColumn, L"A", ES_CENTER);
			TextAt(pDC, nRow++, nColumn, L"B", ES_CENTER);
			TextAt(pDC, nRow++, nColumn, L"C", ES_CENTER);
			TextAt(pDC, nRow++, nColumn, L"U", ES_CENTER);
		}
		nColumn++;
		nRow = 1;
		Buff.Format(L"%.02f %s", CpSurface.Side[nSide].fX * theApp.m_fUnits, theApp.m_Units);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		Buff.Format(L"%.02f %s", CpSurface.Side[nSide].fY * theApp.m_fUnits, theApp.m_Units);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		Buff.Format(L"%.02f %s", CpSurface.Side[nSide].fZ * theApp.m_fUnits, theApp.m_Units);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_NORM) {
			Buff.Format(L"%.04f", CpSurface.Side[nSide].fI);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
			Buff.Format(L"%.04f", CpSurface.Side[nSide].fJ);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
			Buff.Format(L"%.04f", CpSurface.Side[nSide].fK);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_A) {
			Buff.Format(L"%.02f%s", CpSurface.Side[nSide].fXt, DEGREES);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_B) {
			Buff.Format(L"%.02f%s", CpSurface.Side[nSide].fYt, DEGREES);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_ROT_NORM) {
			Buff.Format(L"%.02f%s", CpSurface.Side[nSide].fW, DEGREES);		TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_QUAT) {
			CpSurface.QuaternionFromNormal(0, &qNormal);
			Buff.Format(L"%.04f", qNormal.x);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
			Buff.Format(L"%.04f", qNormal.y);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
			Buff.Format(L"%.04f", qNormal.z);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
			Buff.Format(L"%.04f", qNormal.w);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		}
		nColumn++;
	}
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_SIDE_1) {
		theApp.m_nSide0Orientation == 0 ? nSide = 1 : nSide = 0;
		int nRow = 0;
		TextAt(pDC, nRow, nColumn, theApp.m_DlgSideName[nSide], ES_CENTER);
		TextAt(pDC, nRow, nColumn + 1, L" ", ES_CENTER);
		nRow++;
		TextAt(pDC, nRow++, nColumn, L"X", ES_CENTER);
		TextAt(pDC, nRow++, nColumn, L"Y", ES_CENTER);
		TextAt(pDC, nRow++, nColumn, L"Z", ES_CENTER);
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_NORM) {
			TextAt(pDC, nRow++, nColumn, L"i", ES_CENTER);
			TextAt(pDC, nRow++, nColumn, L"j", ES_CENTER);
			TextAt(pDC, nRow++, nColumn, L"k", ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_A) {
			TextAt(pDC, nRow++, nColumn, L"A", ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_B) {
			TextAt(pDC, nRow++, nColumn, L"B", ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_ROT_NORM) {
			TextAt(pDC, nRow++, nColumn, Theta, ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_QUAT) {
			TextAt(pDC, nRow++, nColumn, L"A", ES_CENTER);
			TextAt(pDC, nRow++, nColumn, L"B", ES_CENTER);
			TextAt(pDC, nRow++, nColumn, L"C", ES_CENTER);
			TextAt(pDC, nRow++, nColumn, L"U", ES_CENTER);
		}
		nColumn++;
		nRow = 1;
		Buff.Format(L"%.02f %s", CpSurface.Side[nSide].fX * theApp.m_fUnits, theApp.m_Units);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		Buff.Format(L"%.02f %s", CpSurface.Side[nSide].fY * theApp.m_fUnits, theApp.m_Units);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		Buff.Format(L"%.02f %s", CpSurface.Side[nSide].fZ * theApp.m_fUnits, theApp.m_Units);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_NORM) {
			Buff.Format(L"%.04f", CpSurface.Side[nSide].fI);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
			Buff.Format(L"%.04f", CpSurface.Side[nSide].fJ);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
			Buff.Format(L"%.04f", CpSurface.Side[nSide].fK);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_A) {
			Buff.Format(L"%.02f%S", CpSurface.Side[nSide].fXt, DEGREES);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_B) {
			Buff.Format(L"%.02f%S", CpSurface.Side[nSide].fYt, DEGREES);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_ROT_NORM) {
			Buff.Format(L"%.02f%S", CpSurface.Side[nSide].fW, DEGREES);		TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_QUAT) {
			CpSurface.QuaternionFromNormal(1, &qNormal);
			Buff.Format(L"%.04f", qNormal.x);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
			Buff.Format(L"%.04f", qNormal.y);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
			Buff.Format(L"%.04f", qNormal.z);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
			Buff.Format(L"%.04f", qNormal.w);	TextAt(pDC, nRow++, nColumn, Buff, ES_CENTER);
		}
		nColumn++;
	}
	nColumn = 0;
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_R0) {
		TextAt(pDC, nRowRot, nColumn++, L"Tur'", ES_CENTER);
		Buff.Format(L"%.02f%s", CpSurface.Side0.fR, DEGREES);	TextAt(pDC, nRowRot, nColumn++, Buff, ES_CENTER);
	}
	if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_R1) {
		TextAt(pDC, nRowRot, nColumn++, L"Rol'", ES_CENTER);
		Buff.Format(L"%.02f%s", CpSurface.Side1.fR, DEGREES);	TextAt(pDC, nRowRot, nColumn++, Buff, ES_CENTER);
	}


	RenderGrid(pDC, GetSysColor(COLOR_MENU));

	pDC->SelectObject(pOldFont);
}

void CPositionPane::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CRect rr;

	m_staticView.GetWindowRect(&rr);

	if (rr.PtInRect(point)) {

		CMenu* menu = (CMenu *) new CMenu;
		menu->LoadMenu(IDR_RB_POSITION_MENU);
		menu->GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		delete menu;

	}
}


void CPositionPane::OnPositionProperties()
{
	FRAME;

	pFrame->SendMessage(UI_SHOW_PROPERTIES_PANE, NULL, PositionDisplay);
}
