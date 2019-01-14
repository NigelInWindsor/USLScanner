#pragma once

// CDynamicNCView view

class CDynamicNCView : public CView
{
	DECLARE_DYNCREATE(CDynamicNCView)

public:
	CDynamicNCView();				// protected constructor used by dynamic creation
	virtual ~CDynamicNCView();

	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);	// overriddent to draw this view

	void CreatePalette();		

	void DisplayProbe1();
	void DisplayProbe2();
	void Display();

	bool m_bResetConfiguration;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

protected:

	DECLARE_MESSAGE_MAP()

	int m_iBitmapWidth;
	int m_iBitmapHeight;

	int m_iViewWidth;
	int m_iViewHeight;

	COLORREF m_cPalette[256];

	DWORD* m_pBuffer[2];
	BYTE* m_pDataBuffer[2];

public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};