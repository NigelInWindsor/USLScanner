// CoordArrayPane.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "CoordArrayPane.h"
#include "MainFrm.h"


#define DIVIDER_POSITION	140

IMPLEMENT_DYNAMIC(CCoordArrayPane, CDockablePane)

CCoordArrayPane::CCoordArrayPane()
{
	m_nIndex = 0;
	m_nColumn = 0;
	m_nLine = 0;
	m_nLineSize = 0;
	m_pListItemEdit = NULL;
	m_strRefName[0] = "CAD 1";
	m_strRefName[1] = "CAD 2";
	m_strRefName[2] = "CAD 3";
	m_strRefName[3] = "Measured 1";
	m_strRefName[4] = "Measured 2";
	m_strRefName[5] = "Measured 3";
	m_strRefType[0] = "Surface";
	m_strRefType[1] = "Ball";

	m_LineStyles[0] = L"Linear";
	m_LineStyles[1] = L"Spline";
	m_LineStyles[2] = L"Polynomial";
	m_LineStyles[3] = L"Quadratic";

}
/*
CCoordArrayPane::CPolyCoordPane(int nIDHighBitMap, bool bCombo, bool bList, int nIDDlg) : CUSLDockablePane(nIDHighBitMap, bCombo, bList, nIDDlg)
{
}
*/

CCoordArrayPane::~CCoordArrayPane()
{
}


BEGIN_MESSAGE_MAP(CCoordArrayPane, CDockablePane)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LINE_NUMBER, &CCoordArrayPane::OnDblclkListLines)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_LINE_NUMBER, &CCoordArrayPane::OnGetdispinfoListLines)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LINE_NUMBER, &CCoordArrayPane::OnClickListLines)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, &CCoordArrayPane::OnGetdispinfoListCoords)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COORDS, &CCoordArrayPane::OnDblclkListCoords)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COORDS, &CCoordArrayPane::OnClickListCoords)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_COORDS, &CCoordArrayPane::OnColumnclickListCoords)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_COORDS, OnKeydownListCoords)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_LINE_NUMBER, OnKeydownListLines)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LINE_NUMBER, OnRclickListLines)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COORDS, OnRclickListCoords)
	ON_COMMAND(ID_LINES_COPY, &CCoordArrayPane::OnLinesCopy)
	ON_COMMAND(ID_LINES_PASTE, &CCoordArrayPane::OnLinesPaste)
	ON_COMMAND(ID_LINES_CUT, &CCoordArrayPane::OnLinesCut)
	ON_COMMAND(ID_LIST3D_COPY, &CCoordArrayPane::OnCoordsCopy)
	ON_COMMAND(ID_LIST3D_PASTE, &CCoordArrayPane::OnCoordsPaste)
	ON_COMMAND(ID_LINES_ALLSPLINE, &CCoordArrayPane::OnLinesAllSpline)
	ON_COMMAND(ID_LINES_ALLLINEAR, &CCoordArrayPane::OnLinesAllLinear)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CPolyCoordPane message handlers




int CCoordArrayPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT;

	if (!m_listLines.Create(dwViewStyle, rectDummy, this, IDC_LIST_LINE_NUMBER))
	{
		TRACE0("Failed to create Static View in profiles import \n");
		return -1;      // fail to create
	}

	if (!m_listCoords.Create(dwViewStyle, rectDummy, this, IDC_LIST_COORDS))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	CreateFont();
	CreateColumns();
	FillList();
	FillLineList();

	m_listLines.SetFont(&m_Font);
	m_listCoords.SetFont(&m_Font);

	AdjustLayout();

	return 0;
}


void CCoordArrayPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}

void CCoordArrayPane::AdjustLayout()
{
	CRect rr;
	int nX;

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	switch (theApp.m_nEditLineType) {
	case LineTypes::taught:
	case LineTypes::scan:
	case LineTypes::envelope:
		nX = DIVIDER_POSITION;
		m_listLines.SetWindowPos(NULL, rectClient.left, rectClient.top, nX, rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		m_listCoords.SetWindowPos(NULL, nX, rectClient.top, rectClient.Width() - nX, rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		break;
	case LineTypes::reference:
		nX = 0;
		m_listLines.SetWindowPos(NULL, rectClient.left, rectClient.top, nX, rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		m_listCoords.SetWindowPos(NULL, nX, rectClient.top, rectClient.Width() - nX, rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		break;
	};
	Invalidate(false);
}

void CCoordArrayPane::CreateFont()
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
	m_listCoords.SetFont(&m_Font);
	m_listLines.SetFont(&m_Font);
}

void CCoordArrayPane::ChangeFontSize()
{
	CreateFont();
//	CreateColumns();
//	FillLineList();
//	FillList();
}

void CCoordArrayPane::CreateColumns()
{
	int ColumnWidth[30];
	int	ColumnLineWidth[30];
	CString ColumnLineName[30] = { L"#", L"Type" };
	int nColumnCount;
	int ii=0;

	CString Buff = L"9999";
	CDC* pDC = m_listLines.GetDC();
	CSize size = pDC->GetTextExtent(Buff);
	ColumnLineWidth[0] = size.cx;
	ColumnLineWidth[1] = DIVIDER_POSITION - size.cx;

	m_listLines.ReleaseDC(pDC);

	switch (theApp.m_nEditLineType) {
	case LineTypes::taught:
	case LineTypes::scan:
		m_nColumnAxis[ii] = -1;			m_ColumnName[ii] = "#";			ColumnWidth[ii++] = 40;
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_SIDE_0) {
			m_nColumnAxis[ii] = _X1;		m_ColumnName[ii] = "X-1";		ColumnWidth[ii++] = 60;
			m_nColumnAxis[ii] = _Y1;		m_ColumnName[ii] = "Y";		ColumnWidth[ii++] = 60;
			m_nColumnAxis[ii] = _Z1;		m_ColumnName[ii] = "Z";		ColumnWidth[ii++] = 60;
			if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_NORM) {
				m_nColumnAxis[ii] = _i1;		m_ColumnName[ii] = "i";		ColumnWidth[ii++] = 60;
				m_nColumnAxis[ii] = _j1;		m_ColumnName[ii] = "j";		ColumnWidth[ii++] = 60;
				m_nColumnAxis[ii] = _k1;		m_ColumnName[ii] = "k";		ColumnWidth[ii++] = 60;
			}
			if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_ROT_NORM)
				m_nColumnAxis[ii] = _w1;		m_ColumnName[ii].Format(L"\x03b8");		ColumnWidth[ii++] = 50;
		}
		if (theApp.m_LastSettings.nPositionPaneMask & POSITION_SIDE_1) {
			m_nColumnAxis[ii] = _X2;		m_ColumnName[ii] = "X-2";		ColumnWidth[ii++] = 60;
			m_nColumnAxis[ii] = _Y2;		m_ColumnName[ii] = "Y";		ColumnWidth[ii++] = 60;
			m_nColumnAxis[ii] = _Z2;		m_ColumnName[ii] = "Z";		ColumnWidth[ii++] = 60;
			if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_NORM) {
				m_nColumnAxis[ii] = _i2;		m_ColumnName[ii] = "i";		ColumnWidth[ii++] = 60;
				m_nColumnAxis[ii] = _j2;		m_ColumnName[ii] = "j";		ColumnWidth[ii++] = 60;
				m_nColumnAxis[ii] = _k2;		m_ColumnName[ii] = "k";		ColumnWidth[ii++] = 60;
			}
			if (theApp.m_LastSettings.nPositionPaneMask & POSITION_DISPLAY_ROT_NORM)
				m_nColumnAxis[ii] = _w2;		m_ColumnName[ii].Format(L"\x03b8");		ColumnWidth[ii++] = 50;
		}
		m_nNumberColumns = ii;
		break;
	case LineTypes::reference:
		m_nColumnAxis[0] = -1;					m_ColumnName[0] = "#";			ColumnWidth[0] = 120;
		m_nColumnAxis[1] = _X1;					m_ColumnName[1] = "X";			ColumnWidth[1] = 70;
		m_nColumnAxis[2] = _Y1;					m_ColumnName[2] = "Y";			ColumnWidth[2] = 70;
		m_nColumnAxis[3] = _Z1;					m_ColumnName[3] = "Z";			ColumnWidth[3] = 70;
		m_nColumnAxis[4] = _i1;					m_ColumnName[4] = "i";			ColumnWidth[4] = 70;
		m_nColumnAxis[5] = _j1;					m_ColumnName[5] = "j";			ColumnWidth[5] = 70;
		m_nColumnAxis[6] = _k1;					m_ColumnName[6] = "k";			ColumnWidth[6] = 70;
		m_nColumnAxis[7] = _ref_type;			m_ColumnName[7] = "Type";		ColumnWidth[7] = 90;
		m_nColumnAxis[8] = _ref_vector_length;	m_ColumnName[8] = "Vect Length"; ColumnWidth[8] = 70;
		m_nNumberColumns = 9;
		break;
	}
	m_listCoords.DeleteAllItems();
	m_listLines.DeleteAllItems();

	switch (theApp.m_nEditLineType) {
	case LineTypes::taught:
	case LineTypes::scan:
	case LineTypes::envelope:
	case LineTypes::reference:
		nColumnCount = (&m_listCoords.GetHeaderCtrl())->GetItemCount();
		for (int ii = 0; ii < nColumnCount; ii++) m_listCoords.DeleteColumn(0);
		for (int ii = 0; ii < m_nNumberColumns; ii++) m_listCoords.InsertColumn(ii, m_ColumnName[ii], LVCFMT_CENTER, ColumnWidth[ii]);
		m_listCoords.SetExtendedStyle(LVS_EX_FULLROWSELECT);
		break;

	};

	nColumnCount = (&m_listLines.GetHeaderCtrl())->GetItemCount();
	for (int ii = 0; ii < nColumnCount; ii++) m_listLines.DeleteColumn(0);
	for (int ii = 0; ii<2; ii++) m_listLines.InsertColumn(ii, ColumnLineName[ii], LVCFMT_CENTER, ColumnLineWidth[ii]);
	m_listLines.SetExtendedStyle(LVS_EX_FULLROWSELECT);

}

void CCoordArrayPane::FillList()
{
	CString Buff, Which;
	int nRow, nRowCount;

	switch (theApp.m_nEditLineType) {
	case LineTypes::taught:
	case LineTypes::scan:
	case LineTypes::envelope:
		m_pCpArray = PROFILE->GetLineElement((LineTypes)-1, theApp.m_nLine);

		if (m_pCpArray != NULL) {
			nRow = m_pCpArray->GetSize();
			while (nRow < (nRowCount = m_listCoords.GetItemCount())) {
				m_listCoords.DeleteItem(nRow);
			}

			for (int ii = nRowCount; ii < m_pCpArray->GetSize(); ii++) {
				Buff.Format(_T("%d"), ii + 1);
				m_listCoords.InsertItem(ii, Buff.GetBuffer(255), ii);
			};

			m_listCoords.EnsureVisible(m_nIndex = theApp.m_nIndex, FALSE);
		}
		else {
			m_listCoords.DeleteAllItems();
		}
		m_listCoords.SetItemState(m_nIndex, LVIS_SELECTED, LVIS_SELECTED);
		break;
	case LineTypes::reference:
		m_listCoords.DeleteAllItems();
		Which.LoadString(IDS_New);
		m_listCoords.InsertItem(0, Which + " " + theApp.m_3PtName[0], 0);
		m_listCoords.InsertItem(1, Which + " " + theApp.m_3PtName[1], 1);
		m_listCoords.InsertItem(2, Which + " " + theApp.m_3PtName[2], 2);

		Which.LoadString(IDS_Original);
		m_listCoords.InsertItem(3, Which + " " + theApp.m_3PtName[0], 3);
		m_listCoords.InsertItem(4, Which + " " + theApp.m_3PtName[1], 4);
		m_listCoords.InsertItem(5, Which + " " + theApp.m_3PtName[2], 5);
		m_listCoords.SetColumnWidth(0, 120);
		break;
	}
	m_listCoords.RedrawWindow();
	SetWindowTitle();
}

void CCoordArrayPane::FillLineList()
{
	CString Buff;

	m_listLines.DeleteAllItems();
	int nNumberLines =PROFILE->GetNumberLines(theApp.m_nEditLineType);
	for (int ii = 0; ii < nNumberLines; ii++) {
		Buff.Format(_T("%d"), ii + 1);
		m_listLines.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
	m_listLines.EnsureVisible(m_nLine = theApp.m_nLine, FALSE);
	for (int ii = 0; ii < nNumberLines; ii++)
		m_listLines.SetItemState(ii, 0, LVIS_SELECTED);

	m_listLines.SetItemState(m_nLine, LVIS_SELECTED, LVIS_SELECTED);
	m_listLines.SetFocus();
	SetWindowTitle();
}

void CCoordArrayPane::EnsureLineListVisible()
{
	m_listLines.EnsureVisible(theApp.m_nLine, FALSE);
/*
	int nNumberLines = PROFILE->GetNumberLines(theApp.m_nEditLineType);
	for (int ii = 0; ii < nNumberLines; ii++)
		m_listLines.SetItemState(ii, 0, LVIS_SELECTED);

	m_listLines.SetItemState(m_nLine, LVIS_SELECTED, LVIS_SELECTED);
	m_listLines.SetFocus();
*/
}


void CCoordArrayPane::OnGetdispinfoListCoords(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	static	WCHAR str[100];
	COORDDATA* pCp;
	int nColumn = pDispInfo->item.iSubItem;
	int	nRow = pDispInfo->item.iItem;

	if (theApp.m_nEditLineType == LineTypes::reference) {
		GetdispinfoListRef(pNMHDR, pResult);
		return;
	}

	str[0] = 0;

	m_pCpArray = PROFILE->GetLineElement((LineTypes)theApp.m_nEditLineType, theApp.m_nLine);

	if (m_pCpArray && m_pCpArray->GetSize() > 0) {
		if (theApp.m_nEditLineType == LineTypes::reference) {
			pCp = &PROFILE->GetRefCoord(nRow);
		}
		else {
			pCp = m_pCpArray->ElementAt(nRow);
		}

		if (pCp) {
			switch (m_nColumnAxis[nColumn]) {
			case -1:
				break;
			case _X1:
			case _X2:
			case _Y1:
			case _Y2:
			case _Z1:
			case _Z2:
				swprintf(str, 100, L"%.02f", pCp->fPos[m_nColumnAxis[nColumn]]);
				break;
			case _i1:
			case _i2:
			case _j1:
			case _j2:
			case _k1:
			case _k2:
				swprintf_s(str, 100, L"%.04f", pCp->fPos[m_nColumnAxis[nColumn]]);
				break;
			case _w1:
			case _w2:
				swprintf_s(str, 100, L"%.02f%s", pCp->fPos[m_nColumnAxis[nColumn]],(LPCWSTR)DEGREES);
				break;
			}
		}
	}

	pDispInfo->item.pszText = str;
	*pResult = 0;
}

void CCoordArrayPane::GetdispinfoListRef(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	static	WCHAR str[100];
	COORDDATA* pCp;
	int nColumn = pDispInfo->item.iSubItem;
	int	nRow = pDispInfo->item.iItem;

	str[0] = 0;

	pCp = &PROFILE->GetRefCoord(nRow);

	if (pCp) {
		switch (m_nColumnAxis[nColumn]) {
		case -1:
			break;
		case _X1:
		case _X2:
		case _Y1:
		case _Y2:
		case _Z1:
		case _Z2:
			swprintf(str, 100, L"%.02f", pCp->fPos[m_nColumnAxis[nColumn]]);
			break;
		case _i1:
		case _i2:
		case _j1:
		case _j2:
		case _k1:
		case _k2:
			swprintf_s(str, 100, L"%.04f", pCp->fPos[m_nColumnAxis[nColumn]]);
			break;
		case _ref_text:
			swprintf_s(str, 100, L"%s", m_strRefName[pDispInfo->item.iItem].GetBuffer());
			break;
		case _ref_type:
			if (pDispInfo->item.iItem >= 3) {
				swprintf_s(str, 100, L"%s", m_strRefType[pCp->nType &= 1].GetBuffer());
			}
			break;
		case _ref_vector_length:
			/*
				switch (nRow) {
				case 1: swprintf(str, 100, L"%.01f mm", m_WS.getProfileRefCADVectorLength(0, 1));
				break;
				case 2: swprintf(str, 100, L"%.01f mm", m_WS.getProfileRefCADVectorLength(0, 2));
				break;
				case 4: swprintf(str, 100, L"%.01f mm", m_WS.getProfileRefMeasuredVectorLength(0, 1));
				break;
				case 5: swprintf(str, 100, L"%.01f mm", m_WS.getProfileRefMeasuredVectorLength(0, 2));
				break;
				}
				*/
			break;
		}
	}
	

	pDispInfo->item.pszText = str;
	*pResult = 0;
}


void CCoordArrayPane::OnDblclkListCoords(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	COORDDATA* pCp;
	int nColumn = pNMItemActivate->iSubItem;
	int nRow = pNMItemActivate->iItem;

	if (theApp.m_nEditLineType == LineTypes::reference) {
		OnDblclkListRef(pNMHDR, pResult);
		return;
	}

	SAFE_DELETE(m_pListItemEdit);
	SetWindowTitle();

	if (m_pCpArray && m_pCpArray->GetSize() > 0) {

		pCp = m_pCpArray->ElementAt(nRow);
		m_pListItemEdit = new CListItemEdit(this, (CWnd**)&m_pListItemEdit, &m_listCoords, pNMItemActivate);

		switch (m_nColumnAxis[nColumn]) {
		case -1:
			break;
		case _X1:
		case _X2:
		case _Y1:
		case _Y2:
		case _Z1:
		case _Z2:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pCp->fPos[m_nColumnAxis[nColumn]], -100000.0f, 100000.0f, 1.0f, L"%.01f");
			break;
		case _i1:
		case _i2:
		case _j1:
		case _j2:
		case _k1:
		case _k2:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pCp->fPos[m_nColumnAxis[nColumn]], -1.0f, 1.0f, 0.0001f, L"%.04f");
			break;
		case _w1:
		case _w2:
			m_pListItemEdit->Create(CListItemEdit::IDD, this, &pCp->fPos[m_nColumnAxis[nColumn]], -180.0f, 180.0f, 0.1f, L"%.02f");
			break;
		case _ref_type:
			if (nRow < 3) {
				pCp->nType ^= 1;
				pCp->nType &= 1;
				m_listCoords.RedrawWindow();
			}
			break;
		}
	}


	*pResult = 0;
}

void CCoordArrayPane::OnDblclkListRef(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	COORDDATA* pCp;
	int nColumn = pNMItemActivate->iSubItem;
	int nRow = pNMItemActivate->iItem;

	SAFE_DELETE(m_pListItemEdit);
	SetWindowTitle();

	pCp = &PROFILE->GetRefCoord(nRow);


	switch (m_nColumnAxis[nColumn]) {
	case -1:
		break;
	case _X1:
	case _X2:
	case _Y1:
	case _Y2:
	case _Z1:
	case _Z2:
		m_pListItemEdit = new CListItemEdit(this, (CWnd**)&m_pListItemEdit, &m_listCoords, pNMItemActivate);
		m_pListItemEdit->Create(CListItemEdit::IDD, this, &pCp->fPos[m_nColumnAxis[nColumn]], -100000.0f, 100000.0f, 1.0f, L"%.01f");
		break;
	case _i1:
	case _i2:
	case _j1:
	case _j2:
	case _k1:
	case _k2:
		m_pListItemEdit = new CListItemEdit(this, (CWnd**)&m_pListItemEdit, &m_listCoords, pNMItemActivate);
		m_pListItemEdit->Create(CListItemEdit::IDD, this, &pCp->fPos[m_nColumnAxis[nColumn]], -1.0f, 1.0f, 0.0001f, L"%.04f");
		break;
	case _ref_type:
		if (nRow >= 3) {
			pCp->nType ^= 1;
			pCp->nType &= 1;
			m_listCoords.RedrawWindow();
		}
		break;
	}

	*pResult = 0;
}

void CCoordArrayPane::OnClickListCoords(NMHDR *pNMHDR, LRESULT *pResult)
{
	FRAME;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	theApp.m_nIndex = pNMItemActivate->iItem;

	SAFE_DELETE(m_pListItemEdit);
	SetWindowTitle();
	*pResult = 0;
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG);
}


void CCoordArrayPane::OnColumnclickListCoords(NMHDR *pNMHDR, LRESULT *pResult)
{
	FRAME;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nColumn = pNMLV->iSubItem;

	if (m_pCpArray == NULL) return;
	m_pCpArray->Sort((LineSort) m_nColumnAxis[nColumn], 0);
	m_listCoords.Invalidate(false);

	*pResult = 0;
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG);
}

void CCoordArrayPane::OnDblclkListLines(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nColumn = pNMItemActivate->iSubItem;
	int nRow = pNMItemActivate->iItem;

	SAFE_DELETE(m_pListItemEdit);
	SetWindowTitle();

	if ((LineTypes)theApp.m_nEditLineType == taught) {
		CPolyCoord* pCpArray = PROFILE->GetLineElement((LineTypes)theApp.m_nEditLineType, nRow);
		if (pCpArray) {
			m_pListItemEdit = new CListItemEdit(this, (CWnd**)&m_pListItemEdit, &m_listLines, pNMItemActivate);

			switch (nColumn) {
			case 1:
				m_pListItemEdit->Create(CListItemEdit::IDD, this, &pCpArray->m_nStyle, m_LineStyles, 4);
				break;
			}
		}
	}

	*pResult = 0;
}


void CCoordArrayPane::OnClickListLines(NMHDR *pNMHDR, LRESULT *pResult)
{
	FRAME;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	SAFE_DELETE(m_pListItemEdit);

	theApp.m_nIndex = pNMItemActivate->iItem;
	FillList();
	SetWindowTitle();
	*pResult = 0;
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG);

}

void CCoordArrayPane::OnGetdispinfoListLines(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	static	WCHAR str[100];
	int nColumn = pDispInfo->item.iSubItem;
	int	nRow = pDispInfo->item.iItem;
	const WCHAR LineStyles[4][12] = { {L"Linear"}, {L"Spline"}, {L"Polynomial"}, {L"Quadratic"} };

	str[0] = 0;

	CPolyCoord* pCpArray = PROFILE->GetLineElement((LineTypes)theApp.m_nEditLineType, nRow);

	if (pCpArray) {

		switch (nColumn) {
		case 0:
			break;
		case 1: swprintf_s(str, 100, L"%s",LineStyles[(int)pCpArray->m_nStyle]);
			break;
		}
	}

	pDispInfo->item.pszText = str;
	*pResult = 0;
}

void CCoordArrayPane::OnKeydownListCoords(NMHDR* pNMHDR, LRESULT* pResult)
{
	FRAME;
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	if (pos) {
		m_nIndex = m_listCoords.GetNextSelectedItem(pos);
	}

	switch (pLVKeyDow->wVKey) {
	case 40: m_nIndex++;
		theApp.m_nIndex = m_nIndex;
		break;
	case 38: m_nIndex--;
		theApp.m_nIndex = m_nIndex;
		break;
	}
	m_nIndex = theApp.m_nIndex;
	SetWindowTitle();
	*pResult = 0;
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG);

}


void CCoordArrayPane::OnKeydownListLines(NMHDR* pNMHDR, LRESULT* pResult)
{
	FRAME;
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	POSITION pos = m_listLines.GetFirstSelectedItemPosition();
	if (pos) {
		m_nLine = m_listLines.GetNextSelectedItem(pos);
	}

	switch (pLVKeyDow->wVKey) {
	case 40: m_nLine++;
		theApp.m_nLine = m_nLine;
		break;
	case 38: m_nLine--;
		theApp.m_nLine = m_nLine;
		break;
	}
	m_nLine = theApp.m_nLine;
	SetWindowTitle();
	FillList();
	*pResult = 0;
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG);

}

void CCoordArrayPane::SetWindowTitle()
{
	CString Buff;
	CString Type[] = { L"Taught", L"Scan", L"Brane", L"Envelope", L"Reference", L"", L"", L"", L"", L"", L"", L"" };

	switch (theApp.m_nEditLineType) {
	default:
		Buff.Format(L"Type %s %-20.20s Line %d %-20.20s Index %d", Type[theApp.m_nEditLineType], L" ", theApp.m_nLine + 1, L" ", theApp.m_nIndex + 1);
		break;
	case LineTypes::reference:
		Buff.Format(L"Type %s", Type[theApp.m_nEditLineType]);
		break;
	}
	SetWindowText(Buff);
}

void CCoordArrayPane::OnRclickListLines(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu *pPopup;
	CPoint	Point;
	CString Buff, Language;


	POSITION pos = m_listLines.GetFirstSelectedItemPosition();
	while (pos) {
		int nIndex = m_listLines.GetNextSelectedItem(pos);
	}

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_LIST_LINES_MENU)) {
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this);
	}
	delete menu;

	*pResult = 0;
}

void CCoordArrayPane::OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu *pPopup, *pPopup1;
	CPoint	pt;
	CString Buff, Danger, strGoto, Language;
	COORDDATA* pCp;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	int nIndex = m_listCoords.GetNextSelectedItem(pos);

	GetCursorPos(&pt);

	switch (theApp.m_nEditLineType) {
	default:
		switch (theApp.m_Tank.nScannerDescription) {
		case DUAL_SIDED_7_AXES:
		case DUAL_SIDED_SQUIRTER:
		case SINGLE_BRIDGE_DUAL_SIDED:
		case DUAL_SIDED_SQUIRTER_2:
		case DUAL_ROBOT:
		case DUAL_ROBOT_9_PLUS_9:
		case FB_11_AXIS:
		case SPX_ROBOT:
			if (menu->LoadMenu(IDR_RB_3D_LIST_MENU)) {
				pPopup = menu->GetSubMenu(0);
				if (m_pCpArray && m_pCpArray->GetSize() > 0) {

					pCp = m_pCpArray->ElementAt(nIndex);

					if (theApp.m_nIndex < m_pCpArray->GetSize() - 1) {
						Buff.Format(_T("Bisect %d && %d"), theApp.m_nIndex + 1, theApp.m_nIndex + 2);
						pPopup->ModifyMenu(ID_LIST3D_BISECT, MF_BYCOMMAND, ID_LIST3D_BISECT, Buff);
						Buff.Format(_T("Circumcenter between %d && %d"), theApp.m_nIndex + 1, theApp.m_nIndex + 2);
						pPopup->ModifyMenu(ID_LIST3D_CIRCUMCENTERBETWEEN, MF_BYCOMMAND, ID_LIST3D_CIRCUMCENTERBETWEEN, Buff);
					}
					else {
						pPopup->EnableMenuItem(ID_LIST3D_BISECT, MF_GRAYED | MF_BYCOMMAND);
						pPopup->EnableMenuItem(ID_LIST3D_CIRCUMCENTERBETWEEN, MF_GRAYED | MF_BYCOMMAND);
					}
					if (theApp.m_LastSettings.bGotoViaDangerPlane == TRUE) {
						pPopup1 = pPopup->GetSubMenu(0);
						Buff.LoadString(IDS_Via_Danger_Plane);
						Language.LoadString(IDS_Goto_Left);
						pPopup1->ModifyMenu(ID_LIST3D_GOTO_LEFT, MF_BYCOMMAND, ID_LIST3D_GOTO_LEFT, Language + _T(" ") + Buff);
						Language.LoadString(IDS_Goto_Right);
						pPopup1->ModifyMenu(ID_LIST3D_GOTO_RIGHT, MF_BYCOMMAND, ID_LIST3D_GOTO_RIGHT, Language + _T(" ") + Buff);
						Language.LoadString(IDS_Goto_Both);
						pPopup1->ModifyMenu(ID_LIST3D_GOTO_BOTH, MF_BYCOMMAND, ID_LIST3D_GOTO_BOTH, Language + _T(" ") + Buff);
					}
					pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
				}
			}
			break;
		case SINGLE_ROBOT:
			if (menu->LoadMenu(IDR_RB_3D_LIST_ROBOT_MENU)) {
				pPopup = menu->GetSubMenu(0);
				if (m_pCpArray && m_pCpArray->GetSize() > 0) {

					pCp = m_pCpArray->ElementAt(nIndex);

					if (theApp.m_nIndex < m_pCpArray->GetSize() - 1) {
						Buff.Format(_T("Bisect %d && %d"), theApp.m_nIndex + 1, theApp.m_nIndex + 2);
						pPopup->ModifyMenu(ID_LIST3D_BISECT, MF_BYCOMMAND, ID_LIST3D_BISECT, Buff);
					}
					else {
						pPopup->EnableMenuItem(ID_LIST3D_BISECT, MF_GRAYED | MF_BYCOMMAND);
						pPopup->EnableMenuItem(ID_LIST3D_CIRCUMCENTERBETWEEN, MF_GRAYED | MF_BYCOMMAND);
					}
					pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
				}
			};
			break;
		case SINGLE_BRIDGE_TT_ROLLERS:
		case SINGLE_BRIDGE_TANK_WITH_R:
		case SINGLE_BRIDGE_TANK:
		case DOUBLE_BRIDGE_TANK:
			if (menu->LoadMenu(IDR_RB_3D_LIST_TANK_MENU)) {
				pPopup = menu->GetSubMenu(0);
				if (m_pCpArray && m_pCpArray->GetSize() > 0) {

					pCp = m_pCpArray->ElementAt(nIndex);

					if (theApp.m_nIndex < m_pCpArray->GetSize() - 1) {
						Buff.Format(_T("Bisect %d && %d"), theApp.m_nIndex + 1, theApp.m_nIndex + 2);
						pPopup->ModifyMenu(ID_LIST3D_BISECT, MF_BYCOMMAND, ID_LIST3D_BISECT, Buff);
					}
					else {
						pPopup->EnableMenuItem(ID_LIST3D_BISECT, MF_GRAYED | MF_BYCOMMAND);
						pPopup->EnableMenuItem(ID_LIST3D_CIRCUMCENTERBETWEEN, MF_GRAYED | MF_BYCOMMAND);
					}
					pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
				}
			};
			break;
		case TRACKED_PROBE:
			if (menu->LoadMenu(IDR_RB_3D_LIST_MENU)) {
				pPopup = menu->GetSubMenu(0);
				if (m_pCpArray && m_pCpArray->GetSize() > 0) {

					pCp = m_pCpArray->ElementAt(nIndex);

					if (theApp.m_nIndex < m_pCpArray->GetSize() - 1) {
						Buff.Format(_T("Bisect %d && %d"), theApp.m_nIndex + 1, theApp.m_nIndex + 2);
						pPopup->ModifyMenu(ID_LIST3D_BISECT, MF_BYCOMMAND, ID_LIST3D_BISECT, Buff);
					}
					else {
						pPopup->EnableMenuItem(ID_LIST3D_BISECT, MF_GRAYED | MF_BYCOMMAND);
						pPopup->EnableMenuItem(ID_LIST3D_CIRCUMCENTERBETWEEN, MF_GRAYED | MF_BYCOMMAND);
					}
					pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
				}
			}
			break;
		}

		break;
	case BRAINLINES:
		if (menu->LoadMenu(IDR_RB_3D_BRAIN_MENU)) {

			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

		};
		break;
	case ENVELOPELINES:
		if (menu->LoadMenu(IDR_RB_3D_ENVELOPE_MENU)) {

			pPopup = menu->GetSubMenu(0);
			if (m_pCpArray && m_pCpArray->GetSize() > 0) {

				pCp = m_pCpArray->ElementAt(nIndex);

				if (theApp.m_nIndex < m_pCpArray->GetSize() - 1) {
					Buff.Format(_T("Bisect %d && %d"), theApp.m_nIndex + 1, theApp.m_nIndex + 2);
					pPopup->ModifyMenu(ID_LIST3D_BISECT, MF_BYCOMMAND, ID_LIST3D_BISECT, Buff);
				}
				else {
					pPopup->EnableMenuItem(ID_LIST3D_BISECT, MF_GRAYED | MF_BYCOMMAND);
					pPopup->EnableMenuItem(ID_LIST3D_CIRCUMCENTERBETWEEN, MF_GRAYED | MF_BYCOMMAND);
				}
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
			}

		};
		break;
	case HOLES:
		if (menu->LoadMenu(IDR_RB_3D_HOLES_MENU)) {

			pPopup = menu->GetSubMenu(0);
			switch (theApp.m_nIndex) {
			case 0:
			case 1:
			case 2:
				pPopup->EnableMenuItem(ID_HOLES_SWAP0, MF_GRAYED | MF_BYCOMMAND);
				pPopup->EnableMenuItem(ID_HOLES_SWAP1, MF_GRAYED | MF_BYCOMMAND);
				pPopup->EnableMenuItem(ID_HOLES_INVERTNORMAL, MF_GRAYED | MF_BYCOMMAND);
				break;
			case 3:
				pPopup->ModifyMenu(ID_HOLES_SWAP0, MF_BYCOMMAND, ID_HOLES_SWAP0, _T("Swap with Floating"));
				pPopup->ModifyMenu(ID_HOLES_SWAP1, MF_BYCOMMAND, ID_HOLES_SWAP1, _T("Swap with Third"));
				break;
			case 4:
				pPopup->ModifyMenu(ID_HOLES_SWAP0, MF_BYCOMMAND, ID_HOLES_SWAP0, _T("Swap with Fixed"));
				pPopup->ModifyMenu(ID_HOLES_SWAP1, MF_BYCOMMAND, ID_HOLES_SWAP1, _T("Swap with Third"));
				break;
			case 5:
				pPopup->ModifyMenu(ID_HOLES_SWAP0, MF_BYCOMMAND, ID_HOLES_SWAP0, _T("Swap with Fixed"));
				pPopup->ModifyMenu(ID_HOLES_SWAP1, MF_BYCOMMAND, ID_HOLES_SWAP1, _T("Swap with Floating"));
				break;
			}
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
		};
		break;
	case SURFACELINES:
		if (menu->LoadMenu(IDR_RB_LIST_SURFACE_MENU)) {

			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

		};
		break;
	}
	delete menu;

	*pResult = 0;
}


void CCoordArrayPane::OnLinesCopy()
{
	POSITION pos = m_listLines.GetFirstSelectedItemPosition();
	int nTotal = 0;
	while (pos) {
		nTotal++;
		m_listLines.GetNextSelectedItem(pos);
	}
	SAFE_DELETE_ARRAY(theApp.m_pCopyManyLine);
	if (nTotal > 0) {
		theApp.m_pCopyManyLine = new CPolyCoord[theApp.m_nManyLineL = nTotal];
		pos = m_listLines.GetFirstSelectedItemPosition();
		int nDestLine = 0;
		while (pos) {
			int nSrcLine = m_listLines.GetNextSelectedItem(pos);
			CPolyCoord *pArray = PROFILE->GetLineElement((LineTypes)-1,nSrcLine);
			theApp.m_pCopyManyLine[nDestLine] = pArray;
			nDestLine++;
		}
	}
}



void CCoordArrayPane::OnLinesPaste()
{
	int nInsertIndex = PROFILE->GetNumberLines(theApp.m_nEditLineType);
	POSITION pos = m_listLines.GetFirstSelectedItemPosition();
	if (pos) nInsertIndex = m_listLines.GetNextSelectedItem(pos);

	if ((theApp.m_nManyLineL > 0) && (theApp.m_pCopyManyLine != NULL)) {
		PROFILE->PasteAllLinesFromGlobal(theApp.m_nEditLineType, nInsertIndex);
	}
	FillList();
	FillLineList();
}




void CCoordArrayPane::OnLinesCut()
{
	/*
	POSITION pos = m_listLines.GetFirstSelectedItemPosition();
	while (pos) {
		int nLine = m_listLines.GetNextSelectedItem(pos);
		m_WS.setProfileMarkLineForDeletion(nLine);
	}
	m_WS.DeleteMarkedLines();
	FillList();
	FillLineList();
	*/
}





void CCoordArrayPane::OnCoordsCut()
{
	CPolyCoord* pCpArray = PROFILE->GetLineElement((LineTypes)theApp.m_nEditLineType, theApp.m_nLine);
	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	/*
	while (pos) {
		int nIndex = m_listCoords.GetNextSelectedItem(pos);
		m_WS.setProfileMarkCoordForDeletion(nIndex);
	}
	m_WS.DeleteMarkedCoords();
	FillList();
	FillLineList();
	*/
}


void CCoordArrayPane::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
}

void CCoordArrayPane::OnLinesAllSpline()
{
	for (int nLine = 0; nLine < PROFILE->m_nTaughtLineL; nLine++) {
		PROFILE->m_TaughtLine[nLine].m_nStyle = 1;
	}
	m_listLines.Invalidate(false);
}

void CCoordArrayPane::OnLinesAllLinear()
{
	for (int nLine = 0; nLine < PROFILE->m_nTaughtLineL; nLine++) {
		PROFILE->m_TaughtLine[nLine].m_nStyle = 0;
	}
	m_listLines.Invalidate(false);
}


void CCoordArrayPane::OnCoordsCopy()
{
	CCoord Cp;

	theApp.m_CopyCoords.RemoveAll();
	CPolyCoord* pCpArray = PROFILE->GetLineElement((LineTypes)theApp.m_nEditLineType, theApp.m_nLine);
	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	while (pos) {
		int nIndex = m_listCoords.GetNextSelectedItem(pos);
		switch (theApp.m_nEditLineType) {
		default:
			pCpArray->GetCoord(nIndex, &Cp);
			theApp.m_CopyCoords.Add(Cp);
			break;
		case HOLES:
			if (nIndex >= 0 && nIndex<6) {
				Cp = PROFILE->GetRefCoord(nIndex);
				theApp.m_CopyCoords.Add(Cp);
			}
			break;
		}
	}
}


void CCoordArrayPane::OnCoordsPaste()
{
	CCoord Cp;
	int nInsertIndex = 0;
	CPolyCoord* pCpArray = PROFILE->GetLineElement((LineTypes)theApp.m_nEditLineType, theApp.m_nLine);
	if (pCpArray != NULL) nInsertIndex = pCpArray->GetSize();
	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	if (pos) nInsertIndex = m_listCoords.GetNextSelectedItem(pos);
	
	if (theApp.m_CopyCoords.GetSize() > 0) {
		switch (theApp.m_nEditLineType) {
		default:
			break;
		case HOLES:
			theApp.m_CopyCoords.GetCoord(0, &Cp);
			PROFILE->SetRefCoord(nInsertIndex, Cp);
			break;
		}
	}
	FillList();
}
