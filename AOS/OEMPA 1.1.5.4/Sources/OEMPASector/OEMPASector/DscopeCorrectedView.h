#pragma once

// CDscopeCorrectedView view

class CDscopeCorrectedView : public CView
{
	DECLARE_DYNCREATE(CDscopeCorrectedView)

public:
	CDscopeCorrectedView();				// protected constructor used by dynamic creation
	virtual ~CDscopeCorrectedView();

	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);	// overriddent to draw this view
	virtual void OnActivateView(BOOL bActivate,CView* pActivateView,CView* pDeactiveView);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	void OnButtonDblClk(int iCursorIndex,UINT nFlags, CPoint point);
	void SetPoint(CPoint point);
	void SetEvent(UINT Event);

protected:

	DECLARE_MESSAGE_MAP()

	bool m_bCursor;
	CPoint	m_ptPrev;       // the last mouse pt in the stroke in progress
	double m_xMin,m_xMax,m_zMin,m_zMax;

	int m_iViewHeight;
	int m_iViewWidth;
	int m_iBitmapHeight;
	int m_iBitmapWidth;
	int m_iBitmapSize;

	COLORREF m_cPalette[256];

	DWORD* m_pBuffer;
	BYTE* m_pDataBuffer;

	int* m_aiBitmapWidthIndex;
	int* m_aiBitmapHeightIndex;

	double* m_pCosTable;
	double* m_pSinTable;

	bool m_bOnSizing;
	UINT m_uiEvent;

	int GetAcqLengthEncoderPointQuantity();
	double GetAcqLengthEncoder();
	double GetAcqDepth();

	void ResetBuffer();
	void CreatePalette();
	void CreateAngleTable();

	void DisplayProbe1Gate();
	void Display();
	void UpdateDisplayData();
	void SetMapping();
};