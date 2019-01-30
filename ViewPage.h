#if !defined(AFX_VIEWPAGE_H__B2AC9C96_66EC_4C41_9C59_83C45C77F804__INCLUDED_)
#define AFX_VIEWPAGE_H__B2AC9C96_66EC_4C41_9C59_83C45C77F804__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewPage.h : header file
//
#include "ResizablePage.h"
#include "AnnotateDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CViewPage dialog
extern UINT CalculateEnvelopeLineAndSampleThread(LPVOID pParam);


class CViewPage : public CResizablePage
{
	DECLARE_DYNCREATE(CViewPage)

// Construction
public:
	void Vector2IntersectionPt(D3DXVECTOR2 Line1Point1, D3DXVECTOR2 Line1Point2, D3DXVECTOR2 Line2Point1, D3DXVECTOR2 Line2Point2, D3DXVECTOR2* Result);
	int SegmentIntersectionPt(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, CPoint* pt);
	void RefreshInclusions();
	bool m_bThreadEnabled;
	bool m_bThreadFinished;
	CWinThread* m_pThread;
	int m_nMoveCurrentCADElement;
	CRect m_rrZoomSamplesBtDn;
	CPoint m_ptCursorBtDn;
	void RenderTransparentParallelogram(CPaintDC* pDC,CPoint *pPt);
	void RefreshThicknessCrossSection(int nS0,int nL0, int nS1, int nL1);
	void RefreshHistogram();
	void CADElementSizeArrows();
	void RenderCADElementLine(CPaintDC* pDC, CRect rr,int nCADPtr,int nDevice);
	void RenderCADElementCentreLine(CPaintDC *pDC, CPoint *pPt,int nCADPtr,int nDevice);
	void RenderCADElementSizeArrows(CDC* pDC);
	void RenderTransparentEllipse(CDC* pDC,CRect *rr);
	void RenderTransparentRectangle(CDC* pDC,CRect *rr);
	void RenderCADElementDefectArea(CPaintDC* pDC, CRect rrArea,int nCADPtr);
	void RenderDropOut(CPaintDC* pDC);
	bool m_bRButtonDown;
	void LimitRulerToDropOutSize();
	void DisplayPolygonStats();
	BOOL MouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void GetValueStr();
	void TextBoxAtPt(CPaintDC *pDC, CString Buff,CPoint pt);
	void TextBoxAtCursor(CPaintDC* pDC,CString Buff);
	void PrintHeader(CPaintDC* pDC);
	void PrintWusiWug(float fScale);
	void DrawCADArrow(CPaintDC* pDC,CPoint pt0,CPoint pt1,int nSize, int nDir,int nDevice);
	void DrawCAD(CPaintDC *pDC,int nDevice);
	bool ClientToCoord(CPoint ptPixel,CCoord* pCp);
	void CADLButtonDown(CPoint pt);
	bool m_bLButtonDown;
	void WorldToLineSample(CCoord Cp,int *pSample, int *nLine);
	void GotoPoint(int nSide);
	void DrawProfileBraneLines(CPaintDC *pDC);
	float CalculateAreaOfPolygon();
	void DrawPolygon(CPaintDC *pDC);
	void DrawProfileEnvelopeLines(CPaintDC *pDC);
	void DrawProfileTaughtLines(CPaintDC *pDC);
	void DrawDiagDirChange(CPaintDC *pDC);
	void DrawDiagZeroCrossings(CPaintDC *pDC);
	void DrawProfileWaypoints(CPaintDC *pDC);
	void DrawInclusions(CPaintDC* pDC);
	void DrawCrossSectionLine(CPaintDC* pDC);
	int IsPtInAnnotation(CPoint pt);
	void PrintCallBack(CPaintDC *pDC,CRect* prr,int nDevice);
	void DrawAnnotation(CPaintDC* pDC);
	CAnnotateDlg* m_pAnnotateDlg;
	CPoint m_ptClient;
	void	ClientToSample(CPoint ptPixel,float *pfSample, float *pfLine);
	void	SampleToClient(float fSample, float fLine,CPoint* pPtPixel);
	void	SampleToClient(CPoint ptSample,CPoint* pPtPixel);
	void	ClientToSample(CPoint ptPixel,CPoint* pPtSample);
	void	ClientToWorld(CPoint pt,float *fX,float *fY,bool bPlusStartCoordinate = false);
	void	ComponentOffsetToClient(float fX, float fY,CPoint *pt);
	void	WorldToClient(CCoord Cp,CPoint *pt);
	void	DrawTessellation(CPaintDC* pDC);
	void	DrawRuler(CPaintDC* pDC);
	void	DrawValue(CPaintDC* pDC);
	void	DrawHistogram(CPaintDC* pDC);
	void	DrawZoomVariable(CPaintDC* pDC);
	void	GetGraticuleSize(float *fXSize, float *fYSize,float *fStep,int *nTextStep,float fUnits);
	void	DrawGraticule(CPaintDC* pDC);
	long	MinMax(long* pnV,int nMin,int nMax);
	int		MinMax(int* pnV,int nMin,int nMax);
	void	SetZoomSamples();
	void	RenderTimeImage(CPaintDC* pDC);
	void	RenderAmpImage(CPaintDC* pDC);
	void	InitializePalette(int nDevice);
	void	PresentRenderdView(CPaintDC* pDC);
	void	RenderView(CPaintDC* pDC);
	void	InitializeViewVariables(CPaintDC* pDC,CRect *prr);
	float	ComputeRulerStats(CString *pBuff);
	void	ClientToWorld(CPoint ptPixel,CCoord *pCp);
	void	RotoSampleToWorld(CPoint ptSample,CCoord *pCp);
	void	CADPopulateNodesWorldCoords(int nNode=-1);
	HCURSOR m_hcursorValue1;
	HCURSOR m_hcursorValue2;
	HCURSOR m_hcursorValue3;
	HCURSOR m_hcursorValue4;
	HCURSOR m_hcursorValue5;
	HCURSOR m_hcursorValue6;
	HCURSOR m_hcursorValue7;
	HCURSOR m_hcursorValue8;
	HCURSOR m_hcursorValue9;
	HCURSOR m_hcursorValue10;
	HCURSOR m_hcursorHand;
	COLORREF m_colorrefCrossHair[256];
	CPolyCoord m_LineSamplePos;
	CPoint	m_pointCrossHair;
	int		m_nPolyPtr;
	CString m_strValue;
	CString m_strRuler;
	float	m_fRulerLength;
	bool	m_bRejectWM_WHEEL;
	bool	m_bCrossVisible;
	bool	m_bHistogramPalette;
	bool	m_bPalette;
	bool	m_bInitialize;
	bool	m_bRenderView;
	int		m_nMinViewLine;
	int		m_nMaxViewLine;
	int*	m_nXpixelOdd;
	int*	m_nXpixelEven;
	int		m_nBytesPixel;
	char	m_cColor[256][4];
	char	*m_pArrayView;
	int		m_nMoveCursor;
	CRect	m_rrHistogramSample;
	CRect	m_rrCursor[10];
	CRect	m_rrRuler;
	CRect	m_rrView;
	CRect*	m_prrSamples;
	
	int		m_nToolOption;
	int		m_nImageType;
	int		m_nImageNumber;
	int		m_nTS;
	int		m_nPaletteNumber;
	int		m_nPageOrDlg;
	int		m_nPageIndex;
	CData*	m_pData;
	CWnd*	m_pParent;
	CWnd**	m_pDlg;
	CViewPage(CWnd* pParent = NULL,CWnd** pDlg = NULL, int nImageType=0,int nImageNumber=0,CData* pData = NULL,int nTS=0,int nGate=0,int nPageOrDlg =0,int nPageindex = 0);
	~CViewPage();

// Dialog Data
	//{{AFX_DATA(CViewPage)
	enum { IDD = IDD_VIEW_PAGE };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CViewPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CViewPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnViewPalette1();
	afx_msg void OnViewPalette2();
	afx_msg void OnViewPalette3();
	afx_msg void OnViewPalette4();
	afx_msg void OnViewPalette5();
	afx_msg void OnViewPalette6();
	afx_msg void OnViewPalette7();
	afx_msg void OnViewPalette8();
	afx_msg void OnViewAnnotate();
	afx_msg void OnViewGotoleft();
	afx_msg void OnViewGotoright();
	afx_msg void OnViewGotoboth();
	afx_msg void OnViewCopyareatoenvelope();
	afx_msg void OnViewDeleteannotation();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewGototaughtcoord();
	afx_msg void OnViewUpdatebranecoordinate();
	afx_msg void OnViewAnnotatehistogramdata();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPolygonNew();
	afx_msg void OnPolygonClear();
	afx_msg void OnPolygonClearall();
	afx_msg void OnViewDisplayarea();
	afx_msg void OnZoomEnlarge();
	afx_msg void OnZoomFullscan();
	afx_msg void OnView3dlookatpoint();
	afx_msg void OnPolygonArea();
	afx_msg void OnPolygonLength();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCadSnaptonearestnode();
	afx_msg void OnCadSetheightwidth();
	afx_msg void OnCadSetwidthheight();
	afx_msg void OnCadProperties();
	afx_msg void OnViewOpeninnewwindow();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnViewGotowithLiftoff();
	afx_msg void OnViewProperties();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPAGE_H__B2AC9C96_66EC_4C41_9C59_83C45C77F804__INCLUDED_)
