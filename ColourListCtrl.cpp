// ColourListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ColourListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TEXT_COLOR	1
#define	BK_COLOR	2

/////////////////////////////////////////////////////////////////////////////
// CColourListCtrl
IMPLEMENT_DYNCREATE(CColourListCtrl, CListCtrl)

CColourListCtrl::CColourListCtrl()
{
	m_nRows = 0;
	m_nColumns = 0;
	m_pRow = NULL;

}

CColourListCtrl::~CColourListCtrl()
{

	Zero();
}

void CColourListCtrl::Zero()
{
	if(m_pRow) {
		for(int ii=0;ii<m_nRows;ii++) {
			delete m_pRow[ii].nFlag;
			delete m_pRow[ii].rgbBk;
			delete m_pRow[ii].rgbText;
		}
		delete m_pRow;
		m_pRow = NULL;
	}

}

BEGIN_MESSAGE_MAP(CColourListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CColourListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, CustomDrawList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColourListCtrl message handlers

void CColourListCtrl::CustomDrawList(NMHDR *pNotifyStruct, LRESULT *result)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW) pNotifyStruct; // cast our generic structure to bigger/specialized strc

	int	nRow = lplvcd->nmcd.dwItemSpec;	
	int	nCol = lplvcd->iSubItem;
	
	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*result  = CDRF_NOTIFYITEMDRAW|CDRF_NOTIFYSUBITEMDRAW;	// request notifications for individual items
		break;

	case CDDS_ITEMPREPAINT:
		*result = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:	// drawing subitem
		*result = CDRF_NEWFONT;
		if(m_pRow) {
			if(m_pRow[nRow].nFlag[nCol] & BK_COLOR) {
				lplvcd->clrTextBk = m_pRow[nRow].rgbBk[nCol];
			}
			if(m_pRow[nRow].nFlag[nCol] & TEXT_COLOR) {
				lplvcd->clrText = m_pRow[nRow].rgbText[nCol];
			}
		}
		break;

	default:
		*result = CDRF_DODEFAULT;
	}

}

void CColourListCtrl::ResetColors()
{

	Zero();

	m_nRows = GetItemCount();
	m_nColumns = GetHeaderCtrl()->GetItemCount();


	if(m_nRows) {
		m_pRow = new RowFormat[m_nRows];
		for(int ii=0;ii<m_nRows;ii++) {
			m_pRow[ii].rgbBk = new COLORREF[m_nColumns];
			m_pRow[ii].rgbText = new COLORREF[m_nColumns];
			m_pRow[ii].nFlag = new int[m_nColumns];
			ZeroMemory(m_pRow[ii].nFlag, sizeof(int) * m_nColumns);
		}
	}

}

void CColourListCtrl::SetBkRowColor(int nRow, COLORREF rgb)
{
	int	ii;

	if(m_pRow == NULL) {
		ResetColors();
	} else {
		if(nRow >= m_nRows) {
			m_pRow = (RowFormat *)realloc(m_pRow, (nRow+1) * sizeof RowFormat);
			for(ii=m_nRows;ii<=nRow;ii++) {
				m_pRow[ii].nFlag = 0;
			}
			m_nRows = nRow;
		}
	}
	if(m_pRow) {
		for(int cc=0;cc<m_nColumns;cc++) {
			m_pRow[nRow].rgbBk[cc] = rgb;
			m_pRow[nRow].nFlag[cc] = BK_COLOR;
		}
	}
}

void CColourListCtrl::SetTextRowColor(int nRow, COLORREF rgb)
{
	int	ii;

	if(m_pRow == NULL) {
		ResetColors();
	} else {
		if(nRow >= m_nRows) {
			m_pRow = (RowFormat *)realloc(m_pRow, (nRow+1) * sizeof RowFormat);
			for(ii=m_nRows;ii<=nRow;ii++) {
				m_pRow[ii].nFlag = 0;
			}
			m_nRows = nRow;
		}
	}
	if(m_pRow) {
		for(int cc=0;cc<m_nColumns;cc++) {
			m_pRow[nRow].rgbText[cc] = rgb;
			m_pRow[nRow].nFlag[cc] = TEXT_COLOR;
		}
	}
}

void CColourListCtrl::PostNcDestroy() 
{
	Zero();
	
	CListCtrl::PostNcDestroy();
}
