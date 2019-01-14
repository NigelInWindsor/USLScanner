#if !defined(AFX_ZOOMPAGE_H__9A85530F_27C1_4EF8_AFF7_AB587FE16734__INCLUDED_)
#define AFX_ZOOMPAGE_H__9A85530F_27C1_4EF8_AFF7_AB587FE16734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZoomPage dialog

class CZoomPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CZoomPage)

// Construction
public:
	int MinMax(int *pV,int nMin,int nMax);
	void InvalidateParent();
	void DrawActiveArea(CPaintDC *pDC);
	int m_nMaxViewLine;
	int m_nMinViewLine;
	int m_bInitialize;
	int m_nBytesPixel;
	void InitializeViewVariables(CPaintDC* pDC,CRect* prr);
	bool m_bRenderView;
	void InitializePalette();
	CData* m_pData;
	void PresentRenderdView(CPaintDC* pDC);
	void RenderView(CPaintDC* pDC);
	char m_cColor[256][4];
	char *m_pArrayView;
	int* m_nXpixelOdd;
	int* m_nXpixelEven;
	int	m_nArea;
	int	m_nAreaL;
	int	m_nXArea;
	int	m_nYArea;
	int* m_pnZoomFactor;
	CRect* m_prrSamples;
	CRect m_rrView;
	CPoint	m_ptClient;
	
	bool m_bPalette;
	bool m_bLBDown;
	void UpdateAllControls();
	int m_nParentType;
	CWnd* m_pParent;
	HBRUSH m_hBr;
	CZoomPage(CWnd* pParent = NULL,int nParentType = 0);
	~CZoomPage();

// Dialog Data
	//{{AFX_DATA(CZoomPage)
	enum { IDD = IDD_ZOOM_PAGE };
	CStatic	m_staticMagnification;
	CStatic	m_staticView;
	CSliderCtrl	m_sliderZoom;
	CEdit	m_editZoom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CZoomPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CZoomPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditZoom();
	afx_msg void OnReleasedcaptureSliderZoom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMPAGE_H__9A85530F_27C1_4EF8_AFF7_AB587FE16734__INCLUDED_)
