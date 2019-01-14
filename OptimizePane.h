#pragma once
//#include <afxdockablepane.h>
class COptimizePane :
	public CDockablePane
{
	DECLARE_DYNAMIC(COptimizePane)

public:
	CStatic m_staticView;
	CCoord m_CpHorizontalCentre;
	CCoord m_CpVerticalCentre;
	CPolyCoord m_CpHorizontal;
	CPolyCoord m_CpVertical;
	int	m_nPaintMode = 0;

	COptimizePane();
	~COptimizePane();

public:
	void TransferAndRender(OptimizePaneMessage * pMsg);
	void RenderCrissCross(CDC * pDC);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void AdjustLayout();
	void OnChangeVisualStyle();
	afx_msg void OnPaint();
};

