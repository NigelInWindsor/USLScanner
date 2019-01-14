#if !defined(AFX_VIEWTHICKNESSCROSSSECTIONDLG_H__C4722466_775C_4EBD_B0E6_63975F058783__INCLUDED_)
#define AFX_VIEWTHICKNESSCROSSSECTIONDLG_H__C4722466_775C_4EBD_B0E6_63975F058783__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewThicknessCrossSectionDlg.h : header file
//
#include "StaticPosManage.h"
#include "ImportCrossSectionOverlayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CViewThicknessCrossSectionDlg dialog

class CViewThicknessCrossSectionDlg : public CDialog
{
// Construction
public:
	void RenderView();
	void DrawOverlay(CDC* pDC);
	CImportCrossSectionOverlayDlg* m_pImportCrossSectionOverlayDlg;
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	void SetToolBarCheckedState();
	void SelectCurrentSampleLine(int nS0, int nL0, int nS1, int nL1);
	int	m_nCursorPtr;
	int m_nCursorL;
	bool m_bLbDown;
	void DrawSampleCursor(CDC* pDC);
	float *m_pfThickness;
	int* m_pnAmplitude;
	void DrawCrossSection(CDC* pDC, int nType);
	void GetGraticuleSize(float *fXSize, float *fYSize,float *fStep,int *nTextStep,float fUnits);
	void DrawGraticule(CDC* pDC);
	void DrawBarGraph(CDC* pDC);
	CPoint m_ptLine[2];
	CPoint m_ptPins[2];
	CRect m_rrView;
	CRect m_rrGraticule;
	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticSliderThickness;
	COLORREF *m_pArray;
	int m_nArrayWidth;
	int m_nArrayHeight;
	void AllocateScreenArray();
	int	m_nImageNumber;
	int	m_nImageType;
	int	m_nPaletteNumber;
	int m_nTS;
	double	m_dMinThickness;
	double m_dMaxThickness;
	int m_nMinAmplitude;
	int m_nMaxAmplitude;

	CRect *m_prrSamples;
	
	CToolBar    m_wndToolsToolBar;
	CToolBar	m_wndGateToolBar[16];
	CReBar		m_wndRebar;
	CData* m_pData;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CViewThicknessCrossSectionDlg(CWnd* pParent = NULL,CWnd** pDlg=NULL,int nImageType=0,int nImageNumber=0,int nPaletteNumber=0,CData* pData=NULL,int nTS = 0,CRect *prrSamples = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewThicknessCrossSectionDlg)
	enum { IDD = IDD_VIEW_THICKNESS_CROSSSECTION_DLG };
	CSliderCtrl	m_sliderThickness;
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewThicknessCrossSectionDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewThicknessCrossSectionDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonDoubleCursor();
	afx_msg void OnButtonSingleCursor();
	afx_msg void OnButtonHorizontalGraticule();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonBargraph();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCrosssectionColoursTrace();
	afx_msg void OnCrosssectionColoursBackground();
	afx_msg void OnCrosssectionColoursMaximum();
	afx_msg void OnCrosssectionColoursNominal();
	afx_msg void OnCrosssectionColoursMinimum();
	afx_msg void OnCrosssectionColoursGraticule();
	afx_msg void OnButtonAmp();
	afx_msg void OnButtonTof();
	afx_msg void OnCrosssectionColoursToftrace();
	afx_msg void OnButtonOverlay();
	afx_msg void OnButtonRelativeAbsolute();
	afx_msg void OnButtonLinearDb();
	afx_msg void OnButtonLog();
	afx_msg void OnButtonPercentage();
	afx_msg void OnButtonThickness();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWTHICKNESSCROSSSECTIONDLG_H__C4722466_775C_4EBD_B0E6_63975F058783__INCLUDED_)
