#pragma once

class CToolBarLabel : public CMFCToolBarButton
{
	DECLARE_SERIAL(CToolBarLabel)

public:
	CToolBarLabel(UINT nID = 0, LPCTSTR lpszText = NULL);

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages,
		BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
		BOOL bHighlight = FALSE,
		BOOL bDrawBorder = TRUE,
		BOOL bGrayDisabledButtons = TRUE);

};

