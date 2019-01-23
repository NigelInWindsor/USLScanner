#include "stdafx.h"
#include "USLScanner.h"
#include "AxisStatusPane.h"

IMPLEMENT_DYNAMIC(CAxisStatusPane, CDockablePane)

CAxisStatusPane::CAxisStatusPane()
{
	m_nToolOption = 0;
	m_strUnits[0] = L" mm";
	m_strUnits[1] = L"\x022";
	m_strUnits[2] = DEGREES;

}

CAxisStatusPane::~CAxisStatusPane()
{
}


BEGIN_MESSAGE_MAP(CAxisStatusPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_STATUS, OnGetdispinfoListStatus)
END_MESSAGE_MAP()



int CAxisStatusPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT;

	if (!m_listStatus.Create(dwViewStyle, rectDummy, this, IDC_LIST_STATUS))
	{
		TRACE0("Failed to create Static View in profiles import \n");
		return -1;      // fail to create
	}
	m_listStatus.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_STATUS_TOOLBAR);
	m_wndToolBar.LoadToolBar(IDR_STATUS_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);
	m_wndToolBar.SetRouteCommandsViaFrame(false);

//	CreateFont();
//	m_listStatus.SetFont(&m_Font);
	CreateColumns();
	FillList();

	AdjustLayout();

//	SetTimer(1, 500, NULL);

	return 0;
}


void CAxisStatusPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	CDockablePane::OnSize(nType, cx, cy);
	KillTimer(1);
	AdjustLayout();
	SetTimer(1, 500, NULL);
}

void CAxisStatusPane::AdjustLayout()
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

	switch (m_nToolOption)
	{
	default: m_wndToolBar.SetWindowPos(NULL, rrClient.left, rrClient.top, rrClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		break;
	case 999:
		break;
	}

	m_listStatus.SetWindowPos(NULL, rrClient.left, rrClient.top + cyTlb, rrClient.Width(), rrClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CAxisStatusPane::OnChangeVisualStyle()
{
}

void CAxisStatusPane::CreateFont()
{
	LOGFONT lf;
	::DeleteObject(m_Font.Detach());

	if (theApp.m_LastSettings.bUseAlernativeToSystemFont == false) {
		afxGlobalData.fontRegular.GetLogFont(&lf);

		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);

		afxGlobalData.GetNonClientMetrics(info);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;
	}
	else {
		lf = theApp.m_LastSettings.lfMenus;
	}
	lf.lfHeight = MulDiv(lf.lfHeight, theApp.nPercentageTextSize[theApp.m_LastSettings.nTextPercentageSize], 100);
	m_Font.CreateFontIndirect(&lf);
	m_listStatus.SetFont(&m_Font);
}

void CAxisStatusPane::ChangeFontSize()
{
	CreateFont();
	CreateColumns();
	FillList();
}


void CAxisStatusPane::CreateColumns()
{
	CString ColumnName[30] = { L"Axis", L"Enable", L"In Pos", L"Fault", L"Encoder", L" Pos ", L"Homed", L"Min", L"Max", L"Soft", L"Hard" };

	CDC* pDC = m_listStatus.GetDC();
	int nNumberColumns = 11;

	for (int ii = 0; ii < nNumberColumns; ii++) {
		CSize size = pDC->GetTextExtent(ColumnName[ii] + L"XX");
		if (ii == 5) {
			m_listStatus.InsertColumn(ii, ColumnName[ii], LVCFMT_RIGHT, size.cx * 2);
		}
		else {
			m_listStatus.InsertColumn(ii, ColumnName[ii], LVCFMT_CENTER, size.cx);

		}
	}
//	m_listStatus.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listStatus.ReleaseDC(pDC);

}

void CAxisStatusPane::FillList()
{
	CString Buff;

	ZeroMemory(m_nAxisList, sizeof m_nAxisList);

	int nColumnCount = (&m_listStatus.GetHeaderCtrl())->GetItemCount();

	m_listStatus.DeleteAllItems();
	m_listStatus.setCellRowColumns(32, nColumnCount);

	m_nNumberRows = 0;
	for (int ii = 0; ii < 32; ii++) {
		if (theApp.m_Axes[ii].bAvailable) {
			Buff.Format(L"%s", theApp.m_Axes[ii].cName);
			m_listStatus.InsertItem(ii, Buff.GetBuffer(255), ii);
			m_listStatus.m_pCell[m_nNumberRows * nColumnCount + 1].m_rgbText = 0xFF00FFFF;
			m_nAxisList[m_nNumberRows] = ii;
			m_nNumberRows++;
		}
	};
//	m_listStatus.SetItemState(m_nIndex, LVIS_SELECTED, LVIS_SELECTED);

}



//

void CAxisStatusPane::OnTimer(UINT_PTR nIDEvent)
{
	for (int nRow = 0; nRow < m_nNumberRows; nRow++) {

		AxisData* pAxis = &theApp.m_Axes[m_nAxisList[nRow]];
		if (pAxis->bAvailable && (pAxis->nStatus - m_nOldStatus[m_nAxisList[nRow]] || pAxis->nPos - m_nOldPos[m_nAxisList[nRow]])) {
			if (pAxis->nStatus & PMAC_AMPLIFIER_FAULT) {
				m_listStatus.setCellColor(nRow, 3, RGB(255, 255, 0), RGB(200, 0, 0));
			}
			else {
				m_listStatus.setCellColor(nRow, 3, RGB(0, 0, 0), RGB(255, 255, 255));
			}
			if (pAxis->nStatus & PMAC_HARD_LIMIT) {
				m_listStatus.setCellColor(nRow, 10, RGB(255, 255, 0), RGB(200, 0, 0));
			}
			else {
				m_listStatus.setCellColor(nRow, 10, RGB(0, 0, 0), RGB(255, 255, 255));
			}

			m_nOldStatus[m_nAxisList[nRow]] = theApp.m_Axes[m_nAxisList[nRow]].nStatus;
			m_nOldPos[m_nAxisList[nRow]] = theApp.m_Axes[m_nAxisList[nRow]].nPos;
			m_listStatus.RedrawItems(nRow, nRow);
		}
	}

	CDockablePane::OnTimer(nIDEvent);
}


void CAxisStatusPane::OnGetdispinfoListStatus(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	static	WCHAR str[100];
	AxisData* pAxis;
	int nColumn = pDispInfo->item.iSubItem;
	int	nRow = pDispInfo->item.iItem;
	const wchar_t Tick = 0x2714;

	str[0] = 0;

	if (nRow >= 0 && nRow < 32) {
		pAxis = &theApp.m_Axes[m_nAxisList[nRow]];

		switch (nColumn) {
		case 1:
			pAxis->nStatus & PMAC_AMPLIFIER_ENABLED ? swprintf_s(str, 100, L"%wc", Tick) : swprintf_s(str, 100, L"%s", L"-");
			break;
		case 2:
			pAxis->nStatus & PMAC_IN_POS ? swprintf_s(str, 100, L"%wc", Tick) : swprintf_s(str, 100, L"%s", L"-");
			break;
		case 3:
			pAxis->nStatus & PMAC_AMPLIFIER_FAULT ? swprintf_s(str, 100, L"X") : swprintf_s(str, 100, L"%s", L"-");
			break;
		case 4: swprintf_s(str, 100, L"%d", pAxis->nPos);
			break;
		case 5: swprintf_s(str, 100, L"%.*f%s", pAxis->nPrecision, (float)pAxis->nPos * pAxis->fStepSize, (LPCWSTR)m_strUnits[MinMax(&pAxis->nUnitSelected, 0, 2)]);
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			pAxis->nStatus & PMAC_HARD_LIMIT ? swprintf_s(str, 100, L"X") : swprintf_s(str, 100, L"%s", L"-");
			break;
		}
	}

	pDispInfo->item.pszText = str;
	*pResult = 0;
}
