#include "stdafx.h"
#include "USLMFCListCtrl.h"

IMPLEMENT_DYNAMIC(CUSLMFCListCtrl, CMFCListCtrl)

BEGIN_MESSAGE_MAP(CUSLMFCListCtrl, CMFCListCtrl)
	ON_NOTIFY_REFLECT_EX(NM_CUSTOMDRAW, &CUSLMFCListCtrl::OnNMCustomdraw)
//	ON_MESSAGE(WM_DRAWITEM, &CUSLMFCListCtrl::OnDrawItem)
END_MESSAGE_MAP()
CUSLMFCListCtrl::CUSLMFCListCtrl()
{
}


CUSLMFCListCtrl::~CUSLMFCListCtrl()
{
	if (m_pCell)  delete[](m_pCell);
	if (m_pColumn) delete[](m_pColumn);
}

void CUSLMFCListCtrl::PreSubclassWindow()
{
	
	CMFCHeaderCtrl* pHeader = NULL;
	pHeader = &GetHeaderCtrl();
	if (pHeader != NULL)
	{
		m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd);
	}

	m_clrDefBack = GetTextBkColor();
	m_clrDefText = GetTextColor();
	m_clrDefBack = 0xFFFFFFFF;
	m_clrDefText = 0xFF000000;

	CMFCListCtrl::PreSubclassWindow();
//	SetExtendedStyle(LVS_EX_FULLROWSELECT);
//	ModifyStyle(0, LVS_REPORT);
}

CMFCHeaderCtrl& CUSLMFCListCtrl::GetHeaderCtrl()
{
	return m_HeaderCtrl;
}

void CUSLMFCListCtrl::InitHeader()
{
	// Initialize header control:
	m_HeaderCtrl.SubclassDlgItem(0, this);
	GetFont()->GetLogFont(&m_lf);
}


void CUSLMFCListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
	if (m_HeaderCtrl.GetSafeHwnd() != NULL)
	{
		m_HeaderCtrl.RedrawWindow();
	}
}

/*
if (iRow < m_nRows && iCol < m_nColumns) {
	clrBack = m_pCell[iRow * m_nColumns + iCol].m_rgbBack;
	clrText = m_pCell[iRow * m_nColumns + iCol].m_rgbText;
}
_tcsncpy_s(m_lf.lfFaceName, LF_FACESIZE, _T("Symbol"), 7);
m_lf.lfWeight = 0;
Font.CreateFontIndirect(&m_lf);
SelectObject(lplvcd->nmcd.hdc, &Font);
*/

BOOL CUSLMFCListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	CFont Font;
	int iRow = lplvcd->nmcd.dwItemSpec;
	int iCol = lplvcd->iSubItem;
	*pResult = 0;


	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;          // ask for subitem notifications.
		break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		lplvcd->clrTextBk = m_clrDefBack;
		lplvcd->clrText = m_clrDefText;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
	{
		*pResult = CDRF_DODEFAULT;
		if (iRow < m_nRows && iCol < m_nColumns) {
			if (m_clrDefText != m_pCell[iRow * m_nColumns + iCol].m_rgbText) *pResult = CDRF_NEWFONT;
			if (m_clrDefBack != m_pCell[iRow * m_nColumns + iCol].m_rgbBack) *pResult = CDRF_NEWFONT;
			m_clrDefBack = lplvcd->clrTextBk = m_pCell[iRow * m_nColumns + iCol].m_rgbBack;
			m_clrDefText = lplvcd->clrText = m_pCell[iRow * m_nColumns + iCol].m_rgbText;
		}
	}
	break;
	default:
		*pResult = CDRF_DODEFAULT;
	}
	if (*pResult == 0 || *pResult == CDRF_DODEFAULT)
		return FALSE;
	else
		return TRUE;
}


void CUSLMFCListCtrl::setCellRowColumns(int nRows, int nColumns)
{
	int nCount;

	if (m_pCell)  delete[](m_pCell);
	m_pCell = NULL;
	if (m_pColumn)  delete[](m_pColumn);
	m_pColumn = NULL;
	m_nRows = 0;
	m_nColumns = 0;

	if ((nCount = nRows * nColumns) > 0) {
		m_pCell = new CellInfo[nCount];
		m_nRows = nRows;
		m_nColumns = nColumns;
		for (int i = 0; i < nCount; i++) {
			m_pCell[i].m_rgbBack = 0xFFFFFFFF;
			m_pCell[i].m_rgbText = 0xFF000000;
		}
	}

	if (nColumns > 0) {
		m_pColumn = new ColumnInfo[nColumns];
		for (int i = 0; i < nColumns; i++) {
			m_pColumn[i].m_rgbBack = 0xFFFFFFFF;
			m_pColumn[i].m_rgbText = 0xFF000000;
		}
	}
}

void CUSLMFCListCtrl::setCellColor(int nRow, int nColumn, COLORREF rgbText, COLORREF rgbBack)
{
	if (nRow < m_nRows && nColumn < m_nColumns) {
		int nIndex = nRow * m_nColumns + nColumn;
		m_pCell[nIndex].m_rgbBack = rgbBack;
		m_pCell[nIndex].m_rgbText = rgbText;
	}
}

void CUSLMFCListCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CMFCListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CUSLMFCListCtrl::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{

	// TODO:  Add your code to draw the specified item
}
