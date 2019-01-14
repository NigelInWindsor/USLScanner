#pragma once

// CHardwareCscopeView view

class CHardwareCscopeView : public CView
{
	DECLARE_DYNCREATE(CHardwareCscopeView)

public:
	CHardwareCscopeView();				// protected constructor used by dynamic creation
	virtual ~CHardwareCscopeView();

	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);	// overriddent to draw this view

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void Timer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnEventMessage(WPARAM wParam,LPARAM lParam);//integration

	void ResetBufferEx();//integration
	void ResetDisplayProbe1();//integration
	int GetGateId();

protected:

	DECLARE_MESSAGE_MAP()

	int m_iViewHeight;
	int m_iViewWidth;
	int m_iBitmapHeight;
	int m_iBitmapWidth;

	COLORREF m_cPalette[256];

	DWORD* m_pBuffer;
	BYTE* m_pDataBuffer;

	int m_iAcquisitionLength;
	int* m_pPositionProbe_X[2];
	int* m_pPositionProbe_Y[2];

	//double* m_pCosTable[2];
	//double* m_pSinTable[2];

	bool m_bOnSizing;			// Tell if the window is being resized
	bool m_bSetMapping;			// Tell if it is needed to recalculate the Borne position
	bool m_bResetConfiguration;

	void ResetBuffer();
	void CreatePalette();
	//void CreateAngleTable();

	bool IsProcessRequired();
	void Display();
	void DisplayProbe1(bool bReset);
	void DisplayProbe2(bool bReset);
	void UpdateDisplayData(bool bReset);
	void SetMapping();

	int m_iLastHeight;
	int m_iLastWidth;
	int m_iGateId;
	int m_iGateId4;
};