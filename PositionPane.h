#pragma once

class CCellStruct {

public:
	CCellStruct() {};
	virtual ~CCellStruct() {};

	void setColours(COLORREF rgbTextColor, COLORREF rgbBkColor) {
		rgbText = rgbTextColor;
		rgbBk = rgbBkColor;
	};

	CRect rect;
	COLORREF	rgbText;
	COLORREF	rgbBk;
	int			nBorderMask;
};


// CPositionPane

class CPositionPane : public CDockablePane
{
	DECLARE_DYNAMIC(CPositionPane)

public:
	int	m_nRows;
	int m_nColumns;
	int	m_nToolOption;
	int m_nBorder;
	CFont *m_pFont;
	CCellStruct* m_pCell;
	void RenderDualXYZijk(CDC *pDC, CRect rr);
	afx_msg void OnContextMenu(CWnd * pWnd, CPoint point);
	CStatic m_staticView;

	CPositionPane();
	virtual ~CPositionPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void AdjustLayout();
	void OnChangeVisualStyle();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CFont * SetRowsColumnsFonts(CDC * pDC, int nRows, int nColumns, CString strLargestStr, float * fWidths = NULL);
	void SetRowsColumns(int nRows, int nColumns, float * fWidths = NULL);
	void SetRowColumnWidth(int nRow, int nColumn, float fWidth);
	void TextAt(CDC * pDC, int nRow, int nColumn, CString Buff, int nJustify);
	void TextAt(CDC * pDC, int nRow, int nColumn, int nValue, int nJustify);
	void TextAt(CDC * pDC, int nRow, int nColumn, float fValue, int nJustify);
	void RenderGrid(CDC * pDC, COLORREF rgb);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPositionProperties();
};


