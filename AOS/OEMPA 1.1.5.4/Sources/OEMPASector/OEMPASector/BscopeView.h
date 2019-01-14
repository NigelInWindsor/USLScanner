#pragma once

// CBscopeView view

class CBscopeView : public CView
{
	DECLARE_DYNCREATE(CBscopeView)

public:
	CBscopeView();				// protected constructor used by dynamic creation
	virtual ~CBscopeView();

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
	int m_iBitmapWidth;
	int m_iBitmapHeight;

	COLORREF m_cPalette[256];

	DWORD* m_pBuffer;
	BYTE* m_pDataBuffer;
	bool* m_pUpdateBuffer[2];

	int* m_pPositionProbe_X[2];
	int* m_pPositionProbe_Z[2];

	int* m_pBorneMin[2];
	int* m_pBorneMax[2];

	double* m_pCosTable[2];
	double* m_pSinTable[2];

	bool m_bOnSizing;
	UINT m_uiEvent;				// 0: Nothing
								// 1: Resizing
								// 2: Probe selection has changed
								// 3: Mode selection has changed
								// 4: Modification of the gates
								// 5: New file has been loaded
								// 6: Modification of the slice position
	
	void ResetBuffer();
	void CreatePalette();
	void CreateAngleTable();
	
	void DisplayProbe1Slice();
	void DisplayProbe2Slice();
	void DisplayProbe1Gate();
	void DisplayProbe2Gate();
	void Display();
	void UpdateDisplayData();
	void Fill();
	void FillPosition(int Probe, int IndexX, int IndexZ);
	void SetBorneBuffers();
	void SetMapping();
};