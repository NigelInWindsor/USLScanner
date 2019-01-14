#pragma once

// CDynamicCscopeView view

class CDynamicCscopeView : public CView
{
	DECLARE_DYNCREATE(CDynamicCscopeView)

public:
	CDynamicCscopeView();				// protected constructor used by dynamic creation
	virtual ~CDynamicCscopeView();

	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);	// overriddent to draw this view

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	void ResetBufferEx();//integration
	void ResetDisplayProbe1();//integration
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

	bool m_bOnSizing;			// Tell if the window is being resized
	bool m_bSetMapping;			// Tell if it is needed to recalculate the Borne position
	bool m_bResetConfiguration;

	void ResetBuffer();
	void CreatePalette();
	void CreateAngleTable();

	void Display();
	void DisplayProbe1();
	void DisplayProbe2();
	void UpdateDisplayData();
	void SetMapping();

	int m_iLastHeight;
	int m_iLastWidth;
};