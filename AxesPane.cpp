#include "stdafx.h"
#include "USLScanner.h"
#include "AxesPane.h"

IMPLEMENT_DYNAMIC(CAxesPane, CDockablePane)


CAxesPane::CAxesPane()
{
	m_nIndex = 0;
	m_nColumn = 0;
	m_pListItemEdit = NULL;
	m_strHomeMode[0].LoadStringW(IDS_None);
	m_strHomeMode[1].LoadStringW(IDS_Origin_here);
	m_strHomeMode[2].LoadStringW(IDS_Limit_switch_seek);

	m_strAmplifier[0] = L"";
	m_strAmplifier[1] = L"Kollmorgen";
	m_strAmplifier[2] = L"Copley";
	m_strAmplifier[3] = L"Staubli";
	m_strAmplifier[4] = L"GeoBrick";

	m_strUnits[0] = L"mm";
	m_strUnits[1] = L"\x022";
	m_strUnits[2] = DEGREES;

}


CAxesPane::~CAxesPane()
{
	SAFE_DELETE(m_pListItemEdit);
}

BEGIN_MESSAGE_MAP(CAxesPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, &CAxesPane::OnGetdispinfoListAxes)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COORDS, &CAxesPane::OnDblclkListAxes)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COORDS, &CAxesPane::OnClickListAxes)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COORDS, &CAxesPane::OnRclickListUsers)
	ON_MESSAGE(UI_ITEMCHANGED, ItemChanged)
	ON_COMMAND(ID_AXES_SETHOMEVALUE, &CAxesPane::OnAxesSethomevalue)
	ON_COMMAND(ID_AXES_DOWNLOADTOPPMAC, &CAxesPane::OnAxesDownloadtoppmac)
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(UI_SET_ACCESS_PRIVELAGES, SetAccessPrivelages)
END_MESSAGE_MAP()

int CAxesPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT;

	if (!m_listAxes.Create(dwViewStyle, rectDummy, this, IDC_LIST_COORDS))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	CreateFont();
	CreateColumns();
	FillList();

	m_listAxes.SetFont(&m_Font);

	AdjustLayout();

	return 0;
}

void CAxesPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}

void CAxesPane::AdjustLayout()
{
	CRect rr;

	if (GetSafeHwnd() == NULL)	return;

	CRect rectClient;
	GetClientRect(rectClient);

	m_listAxes.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	Invalidate(false);
}

void CAxesPane::CreateFont()
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
	m_listAxes.SetFont(&m_Font);
}

void CAxesPane::ChangeFontSize()
{
	CreateFont();
	//	CreateColumns();
	//	FillLineList();
	//	FillList();
}

HRESULT CAxesPane::SetAccessPrivelages(WPARAM wp, LPARAM lp)
{
	bool bFlag;
	theApp.m_nLogonLevelMask&theApp.m_cAccess[m_nID] ? bFlag = true : bFlag = false;

//	int nStyle = m_listAxes.GetStyle();
//	m_listAxes.ModifyStyle(0, nStyle | EM_SETREADONLY);
//	m_listAxes.ModifyStyle(0, nStyle | EM_SETREADONLY | ES_READONLY);

	/*
	int nGroupCount = m_listAxes.GetPropertyCount();
	for (int ii = 0; ii < nGroupCount; ii++) {

		CMFCPropertyGridProperty* pGroup = m_wndPropList.GetProperty(ii);
		int nItemCount = pGroup->GetSubItemsCount();
		for (int nItem = 0; nItem < nItemCount; nItem++) {
			CMFCPropertyGridProperty* pProperty = pGroup->GetSubItem(nItem);
			pProperty->Enable(bFlag);
		}
	}
	*/
	return 0;
}


void CAxesPane::CreateColumns()
{
	int ColumnWidth[30] = { 50, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70 };
	CString ColumnName[30] = { L"#", L"Enable", L"Name", L"Physical #", L"Stepsize", L"In pos Err", L"Max speed", L"Max Accel'", L"Home pos", 
		L"Home Mode", L"Kin' Offset", L"Min pos", L"Max pos", L"Amplifier", L"Steps/Rev", L"Units"};

	CDC* pDC = m_listAxes.GetDC();
	m_nNumberColumns = 16;

	int nColumnCount = (&m_listAxes.GetHeaderCtrl())->GetItemCount();
	for (int ii = 0; ii < nColumnCount; ii++) m_listAxes.DeleteColumn(0);
	for (int ii = 0; ii < m_nNumberColumns; ii++) {
		CSize size = pDC->GetTextExtent(ColumnName[ii] + L"XX");
		m_listAxes.InsertColumn(ii, ColumnName[ii], LVCFMT_CENTER, size.cx);
	}
	m_listAxes.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listAxes.ReleaseDC(pDC);
}

void CAxesPane::FillList()
{
	CString Buff;

	m_listAxes.DeleteAllItems();

	for (int ii = 0; ii < 32; ii++) {
		Buff.Format(_T("%d"), ii + 1);
		m_listAxes.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
	m_listAxes.SetItemState(m_nIndex, LVIS_SELECTED, LVIS_SELECTED);
//	SetWindowTitle();
}

void CAxesPane::OnGetdispinfoListAxes(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	static	WCHAR str[100];
	AxisData* pAxis;
	int nColumn = pDispInfo->item.iSubItem;
	int	nRow = pDispInfo->item.iItem;
	const wchar_t Tick = 0x2714;


	str[0] = 0;

	if (nRow >= 0 && nRow < 32) {
		pAxis = &theApp.m_Axes[nRow];

		switch (nColumn) {
		case 1:
			if(pAxis->bAvailable) swprintf_s(str, 100, L"%wc", Tick);
			break;
		case 2:
			swprintf_s(str, 100, L"%s", pAxis->cName);
			break;
		case 3:
			swprintf_s(str, 100, L"%d", pAxis->nPhysicalAxis);
			break;
		case 4:
			if (pAxis->fStepSize == 0.0f) {
				swprintf_s(str, 100, L"%.01f", pAxis->fStepSize);
			}
			else {
				if (pAxis->fStepSize >= 0.001f) {
					swprintf_s(str, 100, L"%.07f", pAxis->fStepSize);
				}
				else {
					swprintf_s(str, 100, L"%11.05e", pAxis->fStepSize);
				}
			}
			break;
		case 5:
			swprintf_s(str, 100, L"%.02f", (float)pAxis->nPositionError * pAxis->fStepSize);
			break;
		case 6:
			swprintf_s(str, 100, L"%.01f", pAxis->fMaxMoveSpeed * 1000.0f * pAxis->fStepSize);
			break;
		case 7:
			swprintf_s(str, 100, L"%.03f", pAxis->fAcceleration);
			break;
		case 8:
			swprintf_s(str, 100, L"%d", pAxis->nHomePos);
			break;
		case 9:
			swprintf_s(str, 100, L"%s", (LPCWSTR)m_strHomeMode[pAxis->nHomeMode%3]);
			break;
		case 10:
			swprintf_s(str, 100, L"%d", pAxis->nOffset);
			break;
		case 11:
			swprintf_s(str, 100, L"%.1f", pAxis->fMinKin);
			break;
		case 12:
			swprintf_s(str, 100, L"%.1f", pAxis->fMaxKin);
			break;
		case 13:
			swprintf_s(str, 100, L"%s", (LPCWSTR)m_strAmplifier[pAxis->eAmplifierType % 4]);
			break;
		case 14:
			if (pAxis->dStepsPerRev == 0.0f) {
				swprintf_s(str, 100, L"%.01f", pAxis->dStepsPerRev);
			}
			else {
				swprintf_s(str, 100, L"%.02f", pAxis->dStepsPerRev);
			}
			break;
		case 15: swprintf(str, 100, L"%s", (LPCWSTR)m_strUnits[MinMax(&pAxis->nUnitSelected, 0, 2)]);
			break;
		}
	}

	pDispInfo->item.pszText = str;
	*pResult = 0;
}

void CAxesPane::OnDblclkListAxes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nColumn = pNMItemActivate->iSubItem;
	m_nRow = pNMItemActivate->iItem;

	if (!(theApp.m_nLogonLevelMask&theApp.m_cAccess[m_nID])) return;

	SAFE_DELETE(m_pListItemEdit);

	if(m_nRow >=0 && m_nRow<32) {
		AxisData* pAxis = &theApp.m_Axes[m_nRow];

		m_pListItemEdit = new CListItemEdit(this, (CWnd**)&m_pListItemEdit, &m_listAxes, pNMItemActivate);

		switch (m_nColumn) {
		case 1: pAxis->bAvailable ^= 1;
			SendMessage(UI_ITEMCHANGED, WM_ITEMFINISHED, 0);
			break;
		case 2:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, pAxis->cName, 10, L"%s");
			break;
		case 3:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->nPhysicalAxis, 0, 32, 1, L"%d");
			break;
		case 4:
			if (pAxis->fStepSize >= 0.001f) {
				m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->fStepSize, 0.0f, 100000.0f, 1.0e-5f, L"%.07f");
			}
			else {
				m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->fStepSize, 0.0f, 100000.0f, 1.0e-5f, L"%11.05e");
			}
			break;
		case 5:
			m_fEditValue = (float)pAxis->nPositionError * pAxis->fStepSize;
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &m_fEditValue, 0.0f, 20.0f, 1.0e-2f, L"%.02f");
			break;
		case 6:
			m_fEditValue = pAxis->fMaxMoveSpeed * 1000.0f * pAxis->fStepSize;
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &m_fEditValue, 0.0f, 2000.0f, 10.0f, L"%.01f");
			break;
		case 7:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->fAcceleration, -100.0f, 100.0f, 0.001f, L"%.03f");
			break;
		case 8:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->nHomePos, -100000000, 100000000, 1, L"%d");
			break;
		case 9:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->nHomeMode, m_strHomeMode, 3);
			break;
		case 10:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->nOffset, -100000000, 100000000, 1, L"%d");
			break;
		case 11:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->fMinKin, -20000.0f, 20000.0f, 1.0, L"%.01f");
			break;
		case 12:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->fMaxKin, -20000.0f, 20000.0f, 1.0, L"%.01f");
			break;
		case 13:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, (int*)&pAxis->eAmplifierType, m_strAmplifier, 5);
			break;
		case 14:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->dStepsPerRev, 0.0f, 10000000.0f, 0.01f, L"%.02f");
			break;
		case 15:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pAxis->nUnitSelected, m_strUnits, 3);
			break;
		}
	}


	*pResult = 0;
}

void CAxesPane::OnClickListAxes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	SAFE_DELETE(m_pListItemEdit);
	if (!(theApp.m_nLogonLevelMask&theApp.m_cAccess[m_nID])) return;

	*pResult = 0;
}


LRESULT CAxesPane::ItemChanged(WPARAM wp, LPARAM lp)
{
	if (m_nRow >= 0 && m_nRow < 32) {
		AxisData* pAxis = &theApp.m_Axes[m_nRow];

		switch (m_nColumn) {
		case 5:
			pAxis->nPositionError = (int)(m_fEditValue / pAxis->fStepSize);
			break;
		case 6:
			pAxis->fMaxMoveSpeed = m_fEditValue / (1000.0f * pAxis->fStepSize);
			break;
		case 14:
			pAxis->nStepsPerRev = (int)pAxis->dStepsPerRev;
			break;
		}
	}
	if (wp == WM_ITEMFINISHED) {
		m_listAxes.RedrawWindow();
	}

	return true;
}

void CAxesPane::OnRclickListUsers(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listAxes.GetFirstSelectedItemPosition();
	m_nIndex = m_listAxes.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_AXES_PANE_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this);

	};
	delete menu;

	*pResult = 0;
}


void CAxesPane::OnAxesSethomevalue()
{
	POSITION pos = m_listAxes.GetFirstSelectedItemPosition();
	while (pos) {
		theApp.m_FBCtrl.SetHomePosToEncoderPos((int)pos - 1);
		m_listAxes.GetNextSelectedItem(pos);
	}
	m_listAxes.RedrawWindow();
	theApp.SaveTank();
}


void CAxesPane::OnAxesDownloadtoppmac()
{
	POSITION pos = m_listAxes.GetFirstSelectedItemPosition();
	while (pos) {
		int nAxis = (int)pos - 1;
		theApp.m_FBCtrl.DownloadAxisParameters((int)pos - 1);
		m_listAxes.GetNextSelectedItem(pos);
	}
	theApp.SaveTank();
}


void CAxesPane::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}
