//{{AFX_INCLUDES()
#include "uslbar.h"
//}}AFX_INCLUDES
#if !defined(AFX_VOLUMEVIEWDLG_H__5A16A60A_286B_4F45_81A8_C8CD7182DF87__INCLUDED_)
#define AFX_VOLUMEVIEWDLG_H__5A16A60A_286B_4F45_81A8_C8CD7182DF87__INCLUDED_

#include "StaticPosManage.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolumeViewDlg.h : header file
//
#include "StdAfx.h"	// Added by ClassView

#include "Data.h"
#include "USLFile.h"
#include "VolumeOptionsSheet.h"
#include "ReportPrintDlg1.h"
#include "FIRDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CVolumeViewDlg dialog

class CVolumeViewDlg : public CDialog
{
// Construction
public:
	void SaveCurrentTraceAsText(int nSample, int nLine, char *pTrace);
	void SetWindowTitle();
	void RenderPositionDlg(int nSample, int nLine);
	void TextBoxAtCursor(CPaintDC *pDC, CString Buff);
	void ClientToWorld(CPoint ptPixel,CCoord *pCp);
	void RotoSampleToWorld(CPoint ptSample,CCoord *pCp);
	void ClientToSample(CPoint ptPixel,CPoint* pPtSample);
	void ClientToSample(CPoint ptPixel,float *pfSample, float *pfLine);
	float ComputeRulerStats(CString *pBuff);
	void SetRulerText(CString Buff);
	void	AdjustLayout();
	bool	m_bHistogramPalette;
	CFIRDlg* m_pFIRDlg;
	void DrawBScanScale(CPaintDC *pDC);
	void OnSelendokImage();
	void AddStringsImageCombo();
	void DrawCScanScale(CPaintDC *pDC);
	CReportPrintDlg* m_pReportPrintDlg;
	void DrawRuler(CPaintDC* pDC, int nWhich);
	CRect m_rrRuler;
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	void SetToolBarCheckedState();
	CVolumeOptionsSheet* m_pVolumeOptionsSheet;
	CToolBar    m_wndArchiveToolBar;
	CToolBar    m_wndToolsToolBar;
	CDialogBar	m_wndDialogBar;
	CDialogBar	m_wndImageDialogBar;
	CReBar		m_wndRebar;
	CMenu		*m_pMenu;
	CString		m_strRuler;
	float		m_fRulerLength;
	bool		m_bInCScan;
	bool		m_bInBScan;

	void GetGraticuleSize(float *fXSize, float *fYSize,float *fStep,int *nTextStep);
	void HBscanToWorld(CPoint pt, float *fX, float *fY);
	void ClientToWorld(CPoint pt, float *fX, float *fY);
	float MinMax(float *pfV, float fMin, float fMax);
	int MinMax(int *pnV, int nMin, int nMax);
	long MinMax(long *pnV, int nMin, int nMax);
	void SetZoomSamples();
	void InitializeViewVariables(CPaintDC* pDC,CRect* prr);
	void RenderView(CPaintDC* pDC);
	void PresentRenderdView(CPaintDC* pDC);
	void InitializePalette(int nDevice);
	void DrawText(CDC *pDC,CRect rr);
	void OnFileSaveasdatfileNormal();
	void OnFileSaveasdatfile90rotate();
	void DrawGraticule(CDC *pDC,CRect rr);
	void SetScrollBarParameters();
	void OnFileSaveasdatfile(int nOrientation);
	void SaveRectAsBmp(CDC* pDC,CRect rr);
	bool FindNearestGate(CPoint point, CRect rr, int *nGate, int *nEnd);
	void DrawStaticCscan(CDC *pDC, CRect *prr, int nAction);
	void DrawVerticalGates(CDC *pDC,CRect rr);
	void DrawHorizontalGates(CDC *pDC,CRect rr);
	void DrawGates(CDC *pDC,CRect rr);
	void DrawTrace(int nSample,int nLine);
	void DrawTrace(char *pTrace);
	void RenderCross(CDC * pDC, CPoint point, int nAction);
	void DrawTrace(CPoint point);
	void WriteTrace(int nSample, int nLine, char *pTrace);
	void GetTrace(int nSample,int nLine,char *pTrace,int nClearMode = 0);
	void DrawStaticHBscan(CDC *pDC, CRect *prr, int nAction);
	void DrawStaticVBscan(CDC *pDC, CRect *prr, int nAction);
	void DrawStaticView(CDC* pDC,CRect * prr,int nAction);
	void CalculateDragBox();
	int CreateAmpCscan(CString Name=L"");
	float GetGateThickness();
	float GetGateWaterPath();

	GatesData m_Gate;
	float m_fTimeBaseScaleDelay;
	float m_fTimeBaseMM;
	char* m_pArrayView;
	char* m_pTrace;
	char* m_pHBscanArray;
	char* m_pVBscanArray;
	int* m_nXpixelOdd;
	int* m_nXpixelEven;
	bool m_bInitialize;
	bool m_bPalette;
	bool m_bRenderView;
	bool m_bScopeDir;
	bool bModifyGatePosition;
	int	m_nImageType;
	int	m_nMinViewLine;
	int	m_nMaxViewLine;
	int m_nBytesPixel;
	int m_nLine;
	int m_nSample;
	int m_nEnd;
	int m_nGate;
	int	m_nCScanPalette;
	int	m_nBScanPalette;
	int	m_nToolOption;
	int	m_nIntPos[8];
	int m_nTimeSlot;
	int m_nHZoomScale;
	int m_nVZoomScale;
	int	m_nZoomFactor;
	int m_nWidthSamples;
	int m_nDelaySamples;
	int m_nWidth;
	int m_nDelay;
	int m_nOldWidth;
	int m_nOldHeight;
	UINT64 m_n64FileLength;
	int m_nTopGap;
	int m_nBottomGap;
	int m_nLeftGap;
	int m_nRightGap;
	int m_nMouseAction;
	int	m_nVolumeNumber;
	int	m_nCscanNumber;
	int	m_nEncoding;
	char m_cColor[256][4];
	CRect* m_prrSamples;
	CRect m_rectZoom;
	CData* m_pData;
	CPoint m_pointScreen;
	CRect m_rectHBscan;
	CRect m_rectVBscan;
	CRect m_rrCscan;
	COLORREF	*m_pArray;
	CUSLFile m_File;
	CString m_FileName;
	CPoint m_pointOld;
	CPoint m_ptDragFrom;
	CStaticPosManage m_StaticPosHCscan;
	CStaticPosManage m_StaticPosVCscan;
	CStaticPosManage m_StaticPosProgress;
	CStaticPosManage m_StaticPosScope;
	CStaticPosManage m_StaticPosHBscan;
	CStaticPosManage m_StaticPosVBscan;
	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticPosDelayWidth;
	CRect m_rectDragBox;
	HCURSOR m_hcursorVMove;
	HCURSOR m_hcursorHMove;
	HCURSOR m_hcursorAllMove;
	bool m_bModifiedFlag;
	
	bool m_bFileOpenSuccessfully;
	CWnd* m_pViewSheet;
	CWnd** m_pDlg;
	~CVolumeViewDlg();
	CVolumeViewDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL, CData* pData = NULL,int nVolumeNumber=0,int nCscanNumber=0);   // standard constructor
	CString m_ImageNames[100];
	void WorldToClient(CPoint *pt, int nSample, int nLine);

// Dialog Data
	//{{AFX_DATA(CVolumeViewDlg)
	enum { IDD = IDD_VOLUME_VIEW_DIALOG };
	CScrollBar	m_scrollVCscanBar;
	CScrollBar	m_scrollHCscanBar;
	CProgressCtrl	m_progressBar;
	CStatic	m_staticVBscan;
	CStatic	m_staticScope;
	CStatic	m_staticHBscan;
	CStatic	m_staticView;
	CUSLBar	m_scrollbarDelayWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolumeViewDlg)
		public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVolumeViewDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateOscilloscopeRectifierOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOscilloscopeRectifierHalfwave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOscilloscopeRectifierFullwave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOscilloscopeGraticuleOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOscilloscopeGraticuleLog(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOscilloscopeGraticule8line(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOscilloscopeDirectionVertical(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOscilloscopeDirectionHorizontal(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnOscilloscopeDirectionHorizontal();
	afx_msg void OnOscilloscopeDirectionVertical();
	afx_msg void OnFileSaveasbmpAscan();
	afx_msg void OnFileSaveasbmpCompletedialog();
	afx_msg void OnFileSaveasbmpHorizontalbscan();
	afx_msg void OnFileSaveasbmpVerticalbscan();
	afx_msg void OnFileSaveasbmpCscan();
	afx_msg void OnScrollHasMovedUslbarctrlScope(long LowPosition, long HighPosition, long Width);
	afx_msg void OnCbscanZoom10();
	afx_msg void OnCbscanZoom100();
	afx_msg void OnCbscanZoom25();
	afx_msg void OnCbscanZoom50();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToolsOptions();
	afx_msg void OnToolsCrosshair();
	afx_msg void OnToolsHBscan();
	afx_msg void OnToolsVBscan();
	afx_msg void OnToolsThickCscan();
	afx_msg void OnToolsAmpCscan();
	afx_msg void OnToolsRuler();
	afx_msg void OnFileReport();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonZoomIn();
	afx_msg void OnButtonZoomOut();
	afx_msg void OnFileImagesaveas();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnProcessFirfilter();
	afx_msg void OnFileSavevolumedataasxml();
	afx_msg void OnButtonFullScreen();
	afx_msg void OnVolumePalette1();
	afx_msg void OnVolumePalette2();
	afx_msg void OnVolumePalette3();
	afx_msg void OnVolumePalette4();
	afx_msg void OnVolumePalette5();
	afx_msg void OnVolumePalette6();
	afx_msg void OnVolumePalette7();
	afx_msg void OnVolumePalette8();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMEVIEWDLG_H__5A16A60A_286B_4F45_81A8_C8CD7182DF87__INCLUDED_)
