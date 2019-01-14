#if !defined(AFX_PROBEBEAMPROFILEPAGE_H__55F86D29_4471_4F0C_A718_31BA6088C010__INCLUDED_)
#define AFX_PROBEBEAMPROFILEPAGE_H__55F86D29_4471_4F0C_A718_31BA6088C010__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeBeamProfilePage.h : header file
//
#include "ResizablePage.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CProbeBeamProfilePage dialog

class CProbeBeamProfilePage : public CResizablePage
{
	DECLARE_DYNCREATE(CProbeBeamProfilePage)

// Construction
public:
	int	m_nMoveEnd;
	bool m_bLbDown;
	bool m_bRbDown;
	float m_fAxialX[2];
	void DrawBScanGraticule(CPaintDC* pDC,CProbeScanData *pScan);
	CRect *m_rr;
	CPoint m_ptClient;

	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticPosAxialPlot;
	CStaticPosManage m_StaticPosAxialGraph;

	int MinMax(int *pnV, int nMin, int nMax);
	void RenderAxialGraph(CPaintDC* pDC);
	void RenderBScan(CPaintDC* pDC);
	void RenderCScans(CPaintDC* pDC);
	CProbe* m_pProbe;
	CProbeBeamProfile* m_pBeamProfile;
	
	void UpdateAllControls();
	CProbeBeamProfilePage(CProbe* pProbe = NULL,CProbeBeamProfile* pBeamProfile = NULL);
	~CProbeBeamProfilePage();

// Dialog Data
	//{{AFX_DATA(CProbeBeamProfilePage)
	enum { IDD = IDD_PROBE_BEAMPROFILE_PAGE };
	CButton	m_checkFocalLength;
	CButton	m_checkSpotSizeFarZone;
	CButton	m_checkNearZone;
	CButton	m_checkSpotSize;
	CButton	m_checkAtAttenuation;
	CEdit	m_editAtAttenuation;
	CSpinButtonCtrl	m_spinAtAttenuation;
	CStatic	m_staticAxialPlot;
	CStatic	m_staticView;
	CStatic	m_staticAxialGraph;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProbeBeamProfilePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProbeBeamProfilePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBeamClearall();
	afx_msg void OnChangeEditAtDbDrop();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckAtAttenuation();
	afx_msg void OnCheckSpotSize();
	afx_msg void OnCheckNearZone();
	afx_msg void OnCheckSpotSizeFar();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnBeamGotohere();
	afx_msg void OnBeamUpdatescancoordinates();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBEBEAMPROFILEPAGE_H__55F86D29_4471_4F0C_A718_31BA6088C010__INCLUDED_)
