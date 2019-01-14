#if !defined(AFX_PRINTCONESCYLINDERSDLG_H__1AF8311C_DDDB_4055_B9E1_FEFFD3A0F9BE__INCLUDED_)
#define AFX_PRINTCONESCYLINDERSDLG_H__1AF8311C_DDDB_4055_B9E1_FEFFD3A0F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintConesCylindersDlg.h : header file
//
#include "StaticPosManage.h"
#include "PrintOptionsDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CPrintConesCylindersDlg dialog

class CPrintConesCylindersDlg : public CDialog
{
// Construction
public:
	bool m_bHalfScalePrintOut;
	bool m_bFullScalePrintOut;
	float m_fPrintOutScale;
	bool ClientToWorld(CPoint pt, CRect rr, float *fAngle, float *fHeight);
	void SetZoomSamples();
	void PrintHeader(CPaintDC* pDC);
	ConeData Cone;
	DonutData Donut;

	long MinMax(long *pnV, int nMin, int nMax);
	int  MinMax(int *pnV, int nMin, int nMax);
	void RenderCrossHair(CPaintDC* pDC, CRect rr);
	void RenderGraticule(CPaintDC* pDC, CRect rr);
	void RenderArcAmpBuffer(CPaintDC* pDC,CRect rr,char *pScreenBuffer,char* cColor, CRect rrSrc);
	void RenderArcCScan(CPaintDC* pDC,CRect rr);
	void PrintWusiWug(float fScale);
	void SetToolBarCheckedState();
	void CalculateConeGeometry();
	void DisplayStats(CPaintDC* pDC,CRect rr);
	CStaticPosManage m_StaticPosView;
	
	HCURSOR	m_hcursorFinger;

	char	*m_pArrayView;
	float	m_fCrossHairAngle;
	float	m_fCrossHairDonutRadius;
	bool	m_bRenderCrossHair;
	bool	m_bRenderGraticule;
	bool	m_bDragSector;
	bool	m_bLeftAngle;
	bool	m_bRightAngle;
	bool	 m_bLbDown;
	bool	m_bDragEnabled;
	int		m_nMouseOption;

	CPoint m_ptClient;
	CPoint m_ptDown;
	CPoint m_ptSampleLineDown;
	CWnd** m_pDlg;
	CWnd*  m_pParent;
	CData* m_pData;
	int m_nImageType;
	int m_nImageNumber;
	int m_nPaletteNumber;
	float m_fVertMag;
	float m_fHoriMag;
	float m_fGratStartAngle;
	double m_dComputationTime;
	CRect m_ZoomSamples;
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	CDialogBar	m_wndDialogBar;
	CPrintOptionsDlg* m_pPrintOptionsDlg;
	CPrintConesCylindersDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL,int nImageType = 0,int nImageNumber = 0,int nPaletteNumber = 0,CData *pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrintConesCylindersDlg)
	enum { IDD = IDD_PRINT_CONES_CYLINDERS_DLG };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintConesCylindersDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintConesCylindersDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonConeZoomIn();
	afx_msg void OnButtonConeZoomOut();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonPrintOptions();
	afx_msg void OnButtonPrintCone();
	afx_msg void OnButtonCrossHair();
	afx_msg void OnButtonPaperWidth();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonCone11();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnConeGraticulezeropos();
	afx_msg void OnButtonConeGraticule();
	afx_msg void OnConePaletteno1();
	afx_msg void OnConePaletteno2();
	afx_msg void OnConePaletteno3();
	afx_msg void OnConePaletteno4();
	afx_msg void OnConePaletteno5();
	afx_msg void OnConePaletteno6();
	afx_msg void OnConePaletteno7();
	afx_msg void OnButtonHalfPaperWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTCONESCYLINDERSDLG_H__1AF8311C_DDDB_4055_B9E1_FEFFD3A0F9BE__INCLUDED_)
