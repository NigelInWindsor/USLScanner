#include "stdafx.h"
#include "USLScanner.h"
#include "StatusPane.h"


// CStatusPane

IMPLEMENT_DYNAMIC(CStatusPane, CDockablePane)

CStatusPane::CStatusPane()
{
	m_pRect = NULL;
	m_nRows = 0;
	m_nColumns = 0;
	m_nToolOption = _THREADS;
	m_pFont = NULL;
}

CStatusPane::~CStatusPane()
{
	SAFE_DELETE(m_pRect);
	SAFE_DELETE(m_pFont);
}


BEGIN_MESSAGE_MAP(CStatusPane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_STATUS_ETHERCAT, &CStatusPane::OnUpdateButtonStatusEcat)
	ON_COMMAND(ID_BUTTON_STATUS_ETHERCAT, &CStatusPane::OnButtonStatusEcat)
	ON_COMMAND(ID_BUTTON_ETHERCAT_RESTART, &CStatusPane::OnButtonEthercatRestart)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_STATUS_JOYSTICK, &CStatusPane::OnUpdateButtonStatusJoystick)
	ON_COMMAND(ID_BUTTON_STATUS_JOYSTICK, &CStatusPane::OnButtonStatusJoystick)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_STATUS_THREADS, &CStatusPane::OnUpdateButtonStatusThreads)
	ON_COMMAND(ID_BUTTON_STATUS_THREADS, &CStatusPane::OnButtonStatusThreads)
END_MESSAGE_MAP()



// CStatusPane message handlers


int CStatusPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_STATUS_TOOLBAR);
	m_wndToolBar.LoadToolBar(IDR_STATUS_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);
	m_wndToolBar.SetRouteCommandsViaFrame(false);

	m_wndEcatToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_STATUS_ECAT_TOOLBAR);
	m_wndEcatToolBar.LoadToolBar(IDR_STATUS_ECAT_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndEcatToolBar.SetPaneStyle(m_wndEcatToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndEcatToolBar.SetPaneStyle(m_wndEcatToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndEcatToolBar.SetOwner(this);
	m_wndEcatToolBar.SetRouteCommandsViaFrame(false);


	AdjustLayout();


	return 0;
}

void CStatusPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	KillTimer(1);
	AdjustLayout();
	SetTimer(1, 400, NULL);
}

void CStatusPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL) return;

	CRect rectClient;
	GetClientRect(rectClient);

	int nTopBorder, nBottomBorder, nLeftBorder, nRightBorder;
	int nBorderDepth = 20;
	CRect rrClient;
	GetClientRect(rrClient);

	nTopBorder = nBottomBorder = nLeftBorder = nRightBorder = 0;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	int cxTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cx;
	int	nX = m_wndEcatToolBar.CalcFixedLayout(FALSE, TRUE).cx;

	m_wndEcatToolBar.ShowWindow(false);

	switch (m_nToolOption)
	{
	default: m_wndToolBar.SetWindowPos(NULL, rrClient.left, rrClient.top, rrClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		break;
	case _ECAT:
		m_wndToolBar.SetWindowPos(NULL, rrClient.left, rrClient.top, cxTlb, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		m_wndEcatToolBar.SetWindowPos(NULL, rrClient.left + cxTlb, rrClient.top, rrClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);
		break;
	case _JOYSTICK:
	case _THREADS:
		m_wndToolBar.SetWindowPos(NULL, rrClient.left, rrClient.top, rrClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		//		m_wndJoystickToolBar.SetWindowPos(NULL, rrClient.left + cxTlb, rrClient.top, rrClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);
		break;
	}


	m_staticView.SetWindowPos(NULL, rrClient.left, rrClient.top + cyTlb, rrClient.Width(), rrClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

}


void CStatusPane::OnTimer(UINT_PTR nIDEvent)
{
	Invalidate(false);

	CDockablePane::OnTimer(nIDEvent);
}


void CStatusPane::OnPaint()
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
	theApp.m_LastSettings.rgbBKPositionPane = RGB(255, 255, 255);
	CBrush brush(theApp.m_LastSettings.rgbBKPositionPane);
	CBrush* pOldBrush = dcMem.SelectObject(&brush);
	CPen pen(PS_SOLID, 1, GetSysColor(COLOR_MENU));
	CPen* pOldPen = dcMem.SelectObject(&pen);
	dcMem.Rectangle(rr);
	dcMem.SetTextColor(theApp.m_LastSettings.rgbTextPositionPane);


	switch (m_nToolOption) {
	case _ECAT:	RenderEthercat(&dcMem, rr);
		break;
	case _JOYSTICK:	RenderJoystick(&dcMem, rr);
		break;
	case _THREADS: RenderThreads(&dcMem, rr);
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



void CStatusPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDockablePane::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE) {
		SetTimer(1, 400, NULL);
	}
	else {
		KillTimer(1);
	}
}


CFont* CStatusPane::SetRowsColumnsFonts(CDC* pDC, int nRows, int nColumns, CString strLargestStr)
{
	CFont *pOldFont = NULL;

	SetRowsColumns(nRows, nColumns);

	//Calculate size of font
	LOGFONT lf = theApp.m_LastSettings.lfPositionPane;
	for (lf.lfHeight = 100; lf.lfHeight > 8; lf.lfHeight--) {
		SAFE_DELETE(m_pFont);
		m_pFont = new CFont();
		m_pFont->CreateFontIndirectW(&lf);
		pOldFont = pDC->SelectObject(m_pFont);
		CSize sizeAxis = pDC->GetTextExtent(strLargestStr);
		m_nBorder = sizeAxis.cy / 2;

		if (((sizeAxis.cx + m_nBorder) < m_pRect[0].Width()) && (sizeAxis.cy < m_pRect[0].Height())) {
			break;
		}
	}

	return pOldFont;

}

void CStatusPane::SetRowsColumns(int nRows, int nColumns)
{
	CRect rr;
	m_staticView.GetWindowRect(rr);
	rr -= rr.TopLeft();

	SAFE_DELETE(m_pRect);
	m_nRows = nRows;
	m_nColumns = nColumns;

	m_pRect = new CRect[m_nRows * m_nColumns];

	int nPnX;
	int nPnY;
	int nColumnWidth = MulDiv(1, rr.Width(), m_nColumns);
	int nColumnHeight = MulDiv(1, rr.Height(), m_nRows);
	for (int nRow = 0; nRow < m_nRows; nRow++) {
		nPnX = MulDiv(nRow, rr.Height(), m_nRows);
		for (int nColumn = 0; nColumn < m_nColumns; nColumn++) {
			nPnY = MulDiv(nColumn, rr.Width(), m_nColumns);
			m_pRect[nRow * m_nColumns + nColumn].TopLeft() = CPoint(nPnY, nPnX);
			m_pRect[nRow * m_nColumns + nColumn].BottomRight() = m_pRect[nRow * m_nColumns + nColumn].TopLeft() + CPoint(nColumnWidth, nColumnHeight);
		}
	}

}

void CStatusPane::TextAt(CDC* pDC, int nRow, int nColumn, CString Buff, int nJustify)
{
	int nPnX, nPnY;
	CSize size;

	size = pDC->GetTextExtent(Buff);

	switch (nJustify) {
	default:		nPnX = m_pRect[nRow * m_nColumns + nColumn].left + m_nBorder;
		break;
	case ES_RIGHT:	nPnX = m_pRect[nRow * m_nColumns + nColumn].right - m_nBorder - size.cx;
		break;
	case ES_CENTER:	nPnX = m_pRect[nRow * m_nColumns + nColumn].CenterPoint().x - size.cx / 2;
		break;
	}
	nPnY = m_pRect[nRow * m_nColumns + nColumn].CenterPoint().y - size.cy / 2;
	pDC->TextOutW(nPnX, nPnY, Buff);
}

void CStatusPane::TextAt(CDC* pDC, int nRow, int nColumn, int nValue, int nJustify)
{
	CString Buff;

	Buff.Format(L"%d", nValue);
	TextAt(pDC, nRow, nColumn, Buff, nJustify);
}

void CStatusPane::TextAt(CDC* pDC, int nRow, int nColumn, float fValue, int nJustify)
{
	CString Buff;

	Buff.Format(L"%f", fValue);
	TextAt(pDC, nRow, nColumn, Buff, nJustify);
}

void CStatusPane::RenderGrid(CDC* pDC, COLORREF rgb)
{
	CRect rr;
	CPen* pPen = new CPen(PS_SOLID, 1, rgb);
	CPen* pOldPen = pDC->SelectObject(pPen);

	m_staticView.GetWindowRect(rr);
	rr -= rr.TopLeft();

	for (int nColumn = 1; nColumn < m_nColumns; nColumn++) {
		pDC->MoveTo(m_pRect[nColumn].left, rr.top);
		pDC->LineTo(m_pRect[nColumn].left, rr.bottom);
	}
	for (int nRow = 1; nRow < m_nRows; nRow++) {
		pDC->MoveTo(rr.left, m_pRect[nRow * m_nColumns].top);
		pDC->LineTo(rr.right, m_pRect[nRow * m_nColumns].top);
	}

	pDC->SelectObject(pOldPen);
	delete pPen;
}


void CStatusPane::RenderEthercat(CDC *pDC, CRect rect)
{
	CPoint pt;
	CString Buff;	CString Row[] = { L"X:", L"Y:", L"Z:", L"i:", L"j:", L"k:" };

	CFont *pOldFont = SetRowsColumnsFonts(pDC, 10, 2, L"Ecat[0].DCClockDiff");

//	theApp.m_FBCtrl.sendStr(L"Ecat[0].DCClockDiff\r\n", 1);

	TextAt(pDC, 0, 0, L"Ethercat count:", ES_CENTER); 	TextAt(pDC, 0, 1, theApp.m_Ethercat.m_nCallBackCount, ES_RIGHT);
	TextAt(pDC, 1, 0, L"Ethercat period:", ES_CENTER); //	TextAt(pDC, 1, 1, (float)theApp.m_Ethercat.getDCPeriod(), ES_RIGHT);
	TextAt(pDC, 2, 0, L"Life count:", ES_CENTER); 		//TextAt(pDC, 2, 1, theApp.m_Ethercat.m_EthercatIO[1].cLifeCount, ES_RIGHT);

	for (int nRow = 0; nRow < 6; nRow++) {
		TextAt(pDC, 3 + nRow, 0, Row[nRow], ES_CENTER); 		//TextAt(pDC, 3 + nRow, 1, theApp.m_Ethercat.m_EthercatIO[1].fArray[nRow], ES_RIGHT);
	}
	TextAt(pDC, 9, 0, L"Ecat[0].DCClockDiff:", ES_CENTER); 		//TextAt(pDC, 9, 1, theApp.m_PPMAC.m_nDCClockDiff, ES_RIGHT);

	RenderGrid(pDC, GetSysColor(COLOR_MENU));

	pDC->SelectObject(pOldFont);
}

void CStatusPane::RenderJoystick(CDC *pDC, CRect rect)
{
	int nRow;
	CPoint pt;
	CString Buff;	CString Row[] = { L"Left Horizontal:", L"Left Vertical:", L"Right Horizontal:", L"Right Vertical:", L"Digital Input:", L"Life count:" };

	CFont *pOldFont = SetRowsColumnsFonts(pDC, 10, 2, L"Ecat[0].DCClockDiff");

	for (nRow = 0; nRow < 4; nRow++) {
		TextAt(pDC, nRow, 0, Row[nRow], ES_CENTER); 		//TextAt(pDC, nRow, 1, theApp.m_Joystick.getPosition(nRow), ES_RIGHT);
	}
	TextAt(pDC, nRow, 0, Row[nRow], ES_CENTER); 		//TextAt(pDC, nRow++, 1, theApp.m_Joystick.getInputPort(0), ES_RIGHT);

	RenderGrid(pDC, GetSysColor(COLOR_MENU));

	pDC->SelectObject(pOldFont);
}

void CStatusPane::RenderThreads(CDC *pDC, CRect rect)
{
	int nRow, nRows;
	CPoint pt;
	CString Buff;


	for (nRows = 0, nRow = 0; nRow < MAX_THREAD_COUNT; nRow++) {
		if (theApp.m_USLThread[nRow].IsActive()) nRows++;
	}
	if (nRows < 10) nRows = 10;

	CFont *pOldFont = SetRowsColumnsFonts(pDC, nRows, 4, L"XXXXXXXXXXXXXXXXX");
	nRow = 0;
	TextAt(pDC, nRow, 0, L"Name", ES_CENTER);
	TextAt(pDC, nRow, 1, L"Rate", ES_CENTER);
	TextAt(pDC, nRow, 2, L"Process", ES_CENTER);
	TextAt(pDC, nRow, 3, L"Dead", ES_CENTER);
	nRow++;

	for (int nThread = 0; nThread < MAX_THREAD_COUNT; nThread++) {
		if (theApp.m_USLThread[nThread].IsActive()) {
			TextAt(pDC, nRow, 0, theApp.m_USLThread[nThread].getName(), ES_CENTER);
			TextAt(pDC, nRow, 1, theApp.m_USLThread[nThread].getRefreshRate(), ES_CENTER);
			TextAt(pDC, nRow, 2, theApp.m_USLThread[nThread].getProcessTime(), ES_CENTER);
			TextAt(pDC, nRow, 3, theApp.m_USLThread[nThread].getDeadTime(), ES_CENTER);
			nRow++;
		}
	}

	RenderGrid(pDC, GetSysColor(COLOR_MENU));

	pDC->SelectObject(pOldFont);
}




void CStatusPane::OnButtonStatusEcat()
{
	if (m_nToolOption == _ECAT) {
		m_nToolOption = 0;
	}
	else {
		m_nToolOption = _ECAT;
		AdjustLayout();
	}
}


void CStatusPane::OnUpdateButtonStatusEcat(CCmdUI *pCmdUI)
{
	m_nToolOption == _ECAT ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
}

void CStatusPane::OnButtonEthercatRestart()
{
//	theApp.m_Ethercat.Restart();
}

void CStatusPane::OnButtonStatusJoystick()
{
	if (m_nToolOption == _JOYSTICK) {
		m_nToolOption = 0;
	}
	else {
		m_nToolOption = _JOYSTICK;
		AdjustLayout();
	}
}


void CStatusPane::OnUpdateButtonStatusJoystick(CCmdUI *pCmdUI)
{
	m_nToolOption == _JOYSTICK ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
}

void CStatusPane::OnButtonStatusThreads()
{
	if (m_nToolOption == _THREADS) {
		m_nToolOption = 0;
	}
	else {
		m_nToolOption = _THREADS;
		AdjustLayout();
	}
}


void CStatusPane::OnUpdateButtonStatusThreads(CCmdUI *pCmdUI)
{
	m_nToolOption == _THREADS ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
}
