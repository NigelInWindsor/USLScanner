#if !defined(AFX_PMACTRAJECTORYPAGE_H__E987C672_2EB1_4B93_AAA4_7359F50C91D4__INCLUDED_)
#define AFX_PMACTRAJECTORYPAGE_H__E987C672_2EB1_4B93_AAA4_7359F50C91D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacTrajectoryPage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CPmacTrajectoryPage dialog

class CPmacTrajectoryPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPmacTrajectoryPage)

// Construction
public:
	HBRUSH m_hBr;
	int m_nTableOrder;
	bool InitializeToolPathPtr();
	int m_nHeadSurface;
	void RefreshFromWayPoints();
	void RenderTipPlot(CPaintDC* pDC,CRect rr);
	void RenderVectorPlot(CPaintDC* pDC,CRect rr);
	void SetToolBarCheckedState();
	void Goto(int nIndex);
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	void DrawTrajectory(CPaintDC* pDC,CRect rr,int nAxis);
	void DrawGraticule(CPaintDC* pDC,CRect rr,int nAxis);

	CCoord m_CpSurface;
	CRect m_rrBorder;
	float m_fBorderPos;
	CPoint m_ptLbDown;
	bool m_bLbDown;
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	CPolyCoord* m_pToolPath;
	CRect m_rrClient;
	
	bool m_bGotoIfHighlighted;
	int	m_nSide;
	int	m_nIndex;
	int	m_nColumn;
	int	m_nRenderType;
	void AddAnchor(CWnd *pCtrl);
	CPmacTrajectoryPage();
	~CPmacTrajectoryPage();

// Dialog Data
	//{{AFX_DATA(CPmacTrajectoryPage)
	enum { IDD = IDD_PMAC_TRAJECTORY_PAGE };
	CListCtrl	m_listCoord;
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPmacTrajectoryPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPmacTrajectoryPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnListGoto();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonGotoIfHighlighted();
	afx_msg void OnButtonLastToolpath();
	afx_msg void OnButtonNextToolpath();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnButtonVectorPlot();
	afx_msg void OnButtonSurfaceWaypointLine();
	afx_msg void OnButtonHeadWaypointLine();
	afx_msg void OnButtonLeftCoordinates();
	afx_msg void OnButtonRightCoordinates();
	afx_msg void OnButtonReverseTableOrder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACTRAJECTORYPAGE_H__E987C672_2EB1_4B93_AAA4_7359F50C91D4__INCLUDED_)
