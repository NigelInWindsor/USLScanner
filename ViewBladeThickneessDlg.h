#if !defined(AFX_VIEWBLADETHICKNEESSDLG_H__FB5302A5_EAEE_42EB_8C74_4D75A8004658__INCLUDED_)
#define AFX_VIEWBLADETHICKNEESSDLG_H__FB5302A5_EAEE_42EB_8C74_4D75A8004658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewBladeThickneessDlg.h : header file
//
#include "ViewOptionsSheet.h"
#include "BladeStatisticsDlg.h"
#include "ReportPrintDlg1.h"

/////////////////////////////////////////////////////////////////////////////
// CViewBladeThickneessDlg dialog

class CViewBladeThickneessDlg : public CDialog
{
// Construction
public:
	void PrintCallBack(CPaintDC *pDC,CRect* prr,int nDevice);
	void SetProfileMinMaxThickness();
	void RefreshStatisticsDlg(int nAction);
	float m_fThicknessLbDown;
	bool m_bChangingMaxThickness;
	bool m_bLbDown;
	CPoint m_ptLbDown;
	CRect m_rrBorder;
	CRect m_rrClient;

	bool Save(CString PathName);
	void UpdateBladeTolerances();
	int m_nTrafficLight;
	CPoint m_ptClient;
	void SetWindowTitle();
	void InitializeFromProfile(int nScanType);
	void SetToolBarCheckedState();
	int m_nLButtonAction;
	void OnSelendokScanType();
	void OnChangeEditLine();
	void OnDeltaposSpinLine(NMHDR* pNMHDR, LRESULT* pResult);
	void OnChangeEditSpeed();
	void OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	void UpdateAllControls();
	int MinMax(int *pnV, int nMin, int nMax);
	void RenderPlan(CPaintDC *pDC,CRect *prr,int nDevice = 0);
	void RenderProfile(CPaintDC *pDC);
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);

	CToolBar    m_wndArchiveToolBar;
	CToolBar    m_wndToolsToolBar;
	CToolBar    m_wndScanToolBar;
	CDialogBar	m_wndDialogBar;
	CDialogBar	m_wndImageDialogBar;
	CReBar		m_wndRebar;
	CMenu		*m_pMenu;

	CReportPrintDlg* m_pReportPrintDlg;
	CBladeStatisticsDlg* m_pBladeStatisticsDlg;
	CViewOptionsSheet* m_pViewOptionsSheet;
	CData m_Data;
	bool  m_bLButtonDown;
	float m_fXCursorPos;
	CRect m_rrProfileGrat;
	CRect m_rrPlanGrat;
	int m_nScanType;
	int	m_nNumberLines;
	CWnd** m_pDlg;
	
	CViewBladeThickneessDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,LPCTSTR FilePath = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewBladeThickneessDlg)
	enum { IDD = IDD_VIEW_BLADE_THICKNESS_DLG };
	CStatic	m_staticProfile;
	CStatic	m_staticPlan;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewBladeThickneessDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewBladeThickneessDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonPointer();
	afx_msg void OnButtonVerticalRuler();
	afx_msg void OnFileSaveas();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBladeAddtrafficlight();
	afx_msg void OnButtonTrafficlights();
	afx_msg void OnBladeDeletetrafficlight();
	afx_msg void OnButtonDragLine();
	afx_msg void OnButtonTools();
	afx_msg void OnFileSave();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCrosssectionColoursTrace();
	afx_msg void OnCrosssectionColoursBackground();
	afx_msg void OnCrosssectionColoursGraticule();
	afx_msg void OnCrosssectionColoursMaximum();
	afx_msg void OnCrosssectionColoursMinimum();
	afx_msg void OnCrosssectionColoursNominal();
	afx_msg void OnCrosssectionColoursToftrace();
	afx_msg void OnFileOpen();
	afx_msg void OnCrosssectionGoto();
	afx_msg void OnButtonDisplayAmplitude();
	afx_msg void OnButtonDisplayTof();
	afx_msg void OnCrosssectionNormalize();
	afx_msg void OnCrosssectionAddbranept();
	afx_msg void OnFileStatistics();
	afx_msg void OnButtonScanAll();
	afx_msg void OnButtonScanCurrentLine();
	afx_msg void OnButtonScanRemainingLines();
	afx_msg void OnCrosssectionNormalizedac();
	afx_msg void OnPlanPrintcrosssection();
	afx_msg void OnPlanPrintallcrosssections();
	afx_msg void OnPlanSelecttoprint();
	afx_msg void OnPlanPrintselection();
	afx_msg void OnPlanClearallselections();
	afx_msg void OnPlanPrintcolourtrafficlightimage();
	afx_msg void OnCrosssectionEraseentirebrane();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWBLADETHICKNEESSDLG_H__FB5302A5_EAEE_42EB_8C74_4D75A8004658__INCLUDED_)
