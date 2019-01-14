#pragma once

#include "ExternalDefinition.h"

// CAscopeView view

class CAscopeView : public CView
{
	DECLARE_DYNCREATE(CAscopeView)

protected:
	CAscopeView();           // protected constructor used by dynamic creation
	virtual ~CAscopeView();

public:

	COLORREF m_clrBackground[2];
	COLORREF m_clrGrid;
	COLORREF m_clrCurve;
	COLORREF m_clrText;

	char* m_pAscan;

	int m_iCycle;
	int m_iProbe;
	int m_iDisplayMode;

	CBrush m_cBrushBackground[2];
	CPen m_cPenGrid[2];
	CPen m_cPenCurve;
	CFont m_cFontText;

	CRect GetAscanRect();

	void RefreshAscanDisplay();
	void DrawBackground(CDC* pDC);
	void DrawGrid(CDC* pDC);
	void DrawScale(CDC* pDC);
	void DrawCurve(CDC* pDC);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


