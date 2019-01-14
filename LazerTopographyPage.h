#if !defined(AFX_LAZERTOPOGRAPHYPAGE_H__CA951418_477E_435F_B760_0150454426D6__INCLUDED_)
#define AFX_LAZERTOPOGRAPHYPAGE_H__CA951418_477E_435F_B760_0150454426D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LazerTopographyPage.h : header file
//
#include "StaticPosManage.h"
#include "EditSpinItem.h"
#include "LaserViewDlg.h"
#include "LaserScanDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CLazerTopographyPage dialog

class CLazerTopographyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLazerTopographyPage)

// Construction
public:
	void RenderScreen();
	void DrawCross(CDC* pDC, CPoint pt, int nSize,COLORREF rgb);
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	void SetToolBarCheckedState();
	CCoord	m_StartCoord;
	CCoord	m_FinishCoord;
	CCoord	m_Cp;
	CPoint	m_ptClient;
	void DrawWindowPlot(CDC* pDC,CRect rr);
	void DrawPolarPlot(CDC* pDC,CRect rr);
	void DrawComponentPlot(CDC* pDC,CRect rr);
	void DrawMarker(CDC* pDC,CRect rr);
	COLORREF	*m_pArray;

	void CloseLazer();

	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticPosList;
	CEditSpinItem* m_pEditSpinItem;
	bool m_bDlgCreated;
	CLazerTopographyPage();
	~CLazerTopographyPage();

	void Move(int x,int y);
	void CloseAllChildren();
	void UpdateAllControls();
	float ClientToAngle(CPoint pt);
	CPoint WorldToClientPolar(float fAngle, float fRange);
	void ClientToPos(CPoint pt, float *fX, float *fY);

	CLaserViewDlg*	m_pLaserViewDlg;
	CLaserScanDlg*	m_pLaserScanDlg;
	CReBar			m_wndRebar;
	int				m_nView;
	CToolBar		m_wndLaserBar;

	float fTempJitterMax;
	float fTempJitterMin;

	int		m_nRadius;
	float m_fX;
	float m_fY;
	float m_fMarkerAngle;
	bool m_bLeftButtonDown;

// Dialog Data
	//{{AFX_DATA(CLazerTopographyPage)
	enum { IDD = IDD_LAZER_TOPOGRAPHY_PAGE };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLazerTopographyPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLazerTopographyPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void LaserToolBarView();
	afx_msg void LaserToolBarScan();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButton3dTools();
	afx_msg void OnButtonLaser360View();
	afx_msg void OnButtonLaserWindowView();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLaserAddtaughtpoint();
	afx_msg void OnLaserNewtaughtline();
	afx_msg void OnButtonLaserOffOn();
	afx_msg void OnLaserStyleLinear();
	afx_msg void OnLaserStyleSpline();
	afx_msg void OnLaserDelete();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAZERTOPOGRAPHYPAGE_H__CA951418_477E_435F_B760_0150454426D6__INCLUDED_)
