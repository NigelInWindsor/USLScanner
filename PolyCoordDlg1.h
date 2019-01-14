#if !defined(AFX_POLYCOORDDLG_H__0CF1304F_E028_412A_B1AE_3BD561362FFE__INCLUDED_)
#define AFX_POLYCOORDDLG_H__0CF1304F_E028_412A_B1AE_3BD561362FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PolyCoordDlg1.h : header file
//
#include "StaticPosManage.h"
/////////////////////////////////////////////////////////////////////////////
// CPolyCoordDlg dialog

class CPolyCoordDlg : public CDialog
{
// Construction
public:
	void SetToolBarCheckedState();
	int	m_nSurfaceJoint;
	void InitializeLinePtr();
	int	m_nLineL;
	CPolyCoord* m_pLine;
	int m_nLine;
	int m_nLineType;
	void UpdateAllControls();
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	CCoord m_Cp[2];
	CCoord CPolyCoordDlg::ClientToWorld(CPoint pt, CCoord *Cp);
	CPoint WorldToClient(CCoord Cp,CPoint *pt,int nIndex, int nAxis);
	void RenderLine(CPaintDC *pDC, CRect rrView, CPolyCoord *pLine);
	void RenderGraticule(CPaintDC *pDC, CRect rrView);
	void RenderPerspectiveArrows(CPaintDC *pDC, CRect rrView);
	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticPosTree;
	void FillList();
	void CreateColumns();
	int		m_nIndex;
	CPolyCoord	m_NewLine;
	CPolyCoord	m_Line;
	CPolyCoord* m_pMasterLine;
	CRect	m_rrGrat;
	CPoint	m_ptClient;
	CWnd** m_pDlg;
	CPolyCoordDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL, CPolyCoord *pLine = NULL, int nLineType = 0, int nLine = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPolyCoordDlg)
	enum { IDD = IDD_POLY_COORD_DLG };
	CEdit	m_editPrompt;
	CSpinButtonCtrl	m_spinLineNumber;
	CEdit	m_editLineNumber;
	CComboBox	m_comboLineType;
	CStatic	m_staticView;
	CTreeCtrl	m_treeCoords;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPolyCoordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPolyCoordDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCoordExpand();
	afx_msg void OnCoordExpandnorm();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnVertexPerspectiveZy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonExport();
	afx_msg void OnVertexPerspective();
	afx_msg void OnVertexAxesX();
	afx_msg void OnVertexAxesY();
	afx_msg void OnVertexAxesZ();
	afx_msg void OnVertexAxesXtip();
	afx_msg void OnVertexAxesYtip();
	afx_msg void OnVertexAxesXright();
	afx_msg void OnVertexAxesYRight();
	afx_msg void OnVertexAxesZRight();
	afx_msg void OnVertexAxesXtipRight();
	afx_msg void OnVertexAxesYtipRight();
	afx_msg void OnChangeEditLineNumber();
	afx_msg void OnSelchangeComboLineType();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonJoint();
	afx_msg void OnButtonSurface();
	afx_msg void OnButtonXLeft();
	afx_msg void OnButtonIjkLeft();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POLYCOORDDLG_H__0CF1304F_E028_412A_B1AE_3BD561362FFE__INCLUDED_)
