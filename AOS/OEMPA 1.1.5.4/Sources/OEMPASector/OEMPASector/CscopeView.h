#pragma once

// CCscopeView view

class CCscopeView : public CView
{
	DECLARE_DYNCREATE(CCscopeView)

public:
	CCscopeView();				// protected constructor used by dynamic creation
	virtual ~CCscopeView();

	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);	// overriddent to draw this view

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	void SetEvent(UINT Event);

protected:

	DECLARE_MESSAGE_MAP()

	int m_iViewHeight;
	int m_iViewWidth;
	int m_iBitmapHeight;
	int m_iBitmapWidth;

	COLORREF m_cPalette[256];

	DWORD* m_pBuffer;
	BYTE* m_pDataBuffer;

	int* m_pPositionProbe_X[2];
	double* m_pPositionProbe_Z[2];

	double* m_pCosTable[2];
	double* m_pSinTable[2];

	bool m_bOnSizing;
	UINT m_uiEvent;

	void ResetBuffer();
	void CreatePalette();
	void CreateAngleTable();

	void DisplayProbe1Gate();
	void DisplayProbe2Gate();
	void Display();
	void UpdateDisplayData();
	void SetMapping();
};