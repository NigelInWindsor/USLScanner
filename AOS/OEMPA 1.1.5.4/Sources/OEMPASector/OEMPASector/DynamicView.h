#pragma once

// CDynamicView view

class CDynamicView : public CView
{
	DECLARE_DYNCREATE(CDynamicView)

public:
	CDynamicView();				// protected constructor used by dynamic creation
	virtual ~CDynamicView();

	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);	// overriddent to draw this view
	virtual void OnActivateView(BOOL bActivate,CView* pActivateView,CView* pDeactiveView);

	bool m_bDisplayReady;		// Tell if we are ready to display
	bool m_bOnSizing;			// Tell if the window is being resized
	bool m_bSetMapping;			// Tell if it is needed to recalculate the Borne position
	bool m_bResetConfiguration;

	void ResetBuffer();			// Reset all the data buffers
	void SetBuffer();			// Calculate the data buffers (MAIN FUNCTION)

	void CreatePalette();		
	void CreateAngleTable();

	void DisplayProbe1();
	void DisplayProbe2();
	void Display();

	void FillPosition(int IndexX, int IndexZ);
	void SetBorneBuffers();
	void SetMapping();

	CWinThread* m_pThread;
	CRITICAL_SECTION m_crtDisplay;

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
	void SetCycle(int iCycle);

protected:

	DECLARE_MESSAGE_MAP()

	int m_iViewHeight;
	int m_iViewWidth;
	int m_iBitmapWidth;
	int m_iBitmapHeight;

	int m_iLastHeight;
	int m_iLastWidth;

	COLORREF m_cPalette[256];

	DWORD* m_pBuffer;
	BYTE* m_pDataBuffer;
	bool* m_pUpdateBuffer;

	int* m_PositionProbe1_X;
	int* m_PositionProbe1_Y;
	int* m_PositionProbe2_X;
	int* m_PositionProbe2_Y;

	int* m_pBorneMin;
	int* m_pBorneMax;

	double* m_pCosTable[2];
	double* m_pSinTable[2];

	double m_ZMin;
	double m_ZMax;

	double m_XMin;
	double m_XMax;

	bool m_bCursor;
	CPoint	m_ptPrev;       // the last mouse pt in the stroke in progress
	double m_xMin,m_xMax,m_zMin,m_zMax;
	bool m_bCycleUpdate;
	int m_iCycle;
	double m_xCycle1,m_xCycle2,m_zCycle1,m_zCycle2;
};