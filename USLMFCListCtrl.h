#pragma once
#include "USLMFCHeaderCtrl.h"
typedef struct CellInfo
{
	COLORREF m_rgbBack;
	COLORREF m_rgbText;
	DWORD_PTR	m_dwUserData;
	int m_nColumn;
	int m_nRow;
//	CellInfo(int nRow = 0, int nColumn = 0, COLORREF rgbText = 0x00000000, COLORREF rgbBack = 0xFFFFFFFF, DWORD_PTR dwUserData = NULL);
}CellInfo;
typedef struct ColumnInfo
{
	int m_nColumn;
	COLORREF m_rgbBack;
	COLORREF m_rgbText;
	PFNLVCOMPARE m_fnCompare;
}ColumnInfo;

class CUSLMFCListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CUSLMFCListCtrl)

public:
	CUSLMFCListCtrl();
	~CUSLMFCListCtrl();
public:
	CUSLMFCHeaderCtrl m_HeaderCtrl;
	CellInfo* m_pCell = NULL;
	ColumnInfo* m_pColumn = NULL;
	int	m_nRows = 0;
	int m_nColumns = 0;
	COLORREF m_clrDefBack;
	COLORREF m_clrDefText;
	LOGFONT m_lf;


protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg virtual void PreSubclassWindow();
	afx_msg CMFCHeaderCtrl & GetHeaderCtrl();
	afx_msg void InitHeader();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNMCustomdraw(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void setCellRowColumns(int nRows, int nColumns);
	void setCellColor(int nRow, int nColumn, COLORREF rgbText, COLORREF rgbBack);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};

