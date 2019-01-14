#include "StdAfx.h"
#include "ToolBarLabel.h"

IMPLEMENT_SERIAL(CToolBarLabel, CMFCToolBarButton, 1)

CToolBarLabel::CToolBarLabel(UINT nID, LPCTSTR lpszText)
{
	if (lpszText != NULL) {
		m_strText = lpszText;
	}

	m_bText = TRUE;
	m_nID = nID;
	m_iImage = -1;
}

void CToolBarLabel::OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages,
	BOOL bHorz, BOOL /*bCustomizeMode*/, BOOL /*bHighlight*/,
	BOOL /*bDrawBorder*/, BOOL /*bGrayDisabledButtons*/)
{
	UINT nStyle = m_nStyle;
	m_nStyle &= ~TBBS_DISABLED;

	CMFCToolBarButton::OnDraw(pDC, rect, pImages, bHorz, FALSE,
		FALSE, FALSE, FALSE);

	m_nStyle = nStyle;
}

