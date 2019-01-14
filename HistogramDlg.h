#if !defined(AFX_HISTOGRAMDLG_H__4C4A73AC_D6E3_478A_9302_DF74A9CE52CF__INCLUDED_)
#define AFX_HISTOGRAMDLG_H__4C4A73AC_D6E3_478A_9302_DF74A9CE52CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistogramDlg.h : header file
//
#include "StaticPosManage.h"
#include "ListItemEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CHistogramDlg dialog

class CHistogramDlg : public CDialog
{
// Construction
public:
	CPen* m_penHist[65536];
	CListItemEdit *m_pListItemEdit;
	void InvalidateViewSheet();
	void FillCircle(CDC *pDC,int nX,int nY,int nR,COLORREF rgb);
	bool m_bTrackMean;
	bool m_bTrackPeak;
	bool m_bTrackAtten;
	void SetToolBarCheckedState();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	int MinMax(int *pnV, int nMin, int nMax);
	bool m_bLBDown;

	COLORREF *m_pArray;
	int m_nArrayWidth;
	int m_nArrayHeight;
	void AllocateScreenArray();

	int	m_nLastThreshold;
	int m_nLatchTimeY[3];
	int	m_nLatchAmpY;
	int	m_nLatchThresholdY[2];
	int	m_nToolOption;
	CRect m_rrView;
	CRect m_rrGraticule;

	void DrawExtStatistics(CDC* pDC);
	void DrawExtHistogram(CDC* pDC);
	void DrawExtScale(CDC* pDC);

	void DrawTimeStatistics(CDC* pDC);
	void DrawTimeHistogram(CDC* pDC);
	void DrawTimeScale(CDC* pDC);
	void DrawDropOutStatistics(CDC* pDC);
	void DrawAmpStatistics(CDC* pDC);
	void DrawAmpHistogram(CDC* pDC);
	void DrawAmpScale(CDC* pDC);
	CStaticPosManage m_StaticPosView;
	int	m_nImageNumber;
	int	m_nImageType;
	int	m_nPaletteNumber;
	int m_nTS;
	CRect m_rrSamples;
	
	CToolBar    m_wndToolsToolBar;
	CReBar		m_wndRebar;
	CData* m_pData;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CHistogramDlg(CWnd* pParent = NULL,CWnd** pDlg=NULL,int nImageType=0,int nImageNumber=0,int nPaletteNumber=0,CData* pData=NULL,int nTS = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHistogramDlg)
	enum { IDD = IDD_HISTOGRAM_DIALOG };
	CButton	m_checkLatchMean;
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistogramDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistogramDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonTrackPeak();
	afx_msg void OnButtonTrackMean();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnButtonLog();
	afx_msg void OnButtonLinearDb();
	afx_msg void OnButtonPercentage();
	afx_msg void OnButtonPorosity();
	afx_msg void OnButtonTrackMaxAttenuation();
	afx_msg void OnButtonDisplayHistogram();
	afx_msg void OnButtonDisplayDropout();
	afx_msg void OnButtonVerbose();
	afx_msg void OnButtonPaint2();
	afx_msg void OnButtonPaint12();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT ItemChanged(WPARAM wp, LPARAM lp);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTOGRAMDLG_H__4C4A73AC_D6E3_478A_9302_DF74A9CE52CF__INCLUDED_)
