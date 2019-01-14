#if !defined(AFX_POROSITYPAGE_H__09366FEC_7164_4C1E_B9CB_BDDCD486CC6A__INCLUDED_)
#define AFX_POROSITYPAGE_H__09366FEC_7164_4C1E_B9CB_BDDCD486CC6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PorosityPage.h : header file
//
#include "ResizablePage.h"
#include "EditSpinItem.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CPorosityPage dialog

class CPorosityPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPorosityPage)

// Construction
public:
	CPoint m_ptClient;
	CPoint m_ptLbDown;
	int m_nMinPlies;
	int	m_nMaxPlies;
	int	m_nMaxPorosity;
	int m_nIndex;
	float m_fBorderPos;
	float m_fMaxAttenuation;
	float m_fMaxPorosity;
	bool m_bLBDown;
	
	bool m_bMoveBorder;
	bool m_bMoveThreshold;
	CRect m_rrBorder;
	CRect m_rrClient;
	CRect m_rrGrat;
	CStaticPosManage m_StaticPosList;

	void RenderFixedPorosityCurve(CPaintDC *pDC, CRect rr);
	void RenderPorosityVsAttenuation(CPaintDC *pDC,CRect rr);
	long PorosityItemChanged(unsigned int nn,long mm);
	CEditSpinItem* m_pEditSpinItem;
	void ClearEditSpin();
	void AddAnchor(CWnd *pCtrl,BOOL LockTL = FALSE,BOOL LockBR = FALSE);
	void UpdateAllControls();
	void FillList();
	void CreateColumns();
	HBRUSH m_hBr;
	CPorosityPage();
	~CPorosityPage();

// Dialog Data
	//{{AFX_DATA(CPorosityPage)
	enum { IDD = IDD_POROSITY_PAGE };
	CEdit	m_editStandardID;
	CEdit	m_editRefTarget;
	CEdit	m_editRefAttenuation;
	CEdit	m_editSquirterDiameter;
	CEdit	m_editProbeDiameter;
	CEdit	m_editFrequency;
	CStatic	m_staticView;
	CMFCListCtrl	m_listPts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPorosityPage)
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
	//{{AFX_MSG(CPorosityPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnDblclkListPoints(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListPoints(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListPoints(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPorosityDelete();
	afx_msg void OnPorositySort();
	afx_msg void OnRclickListPoints(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPorosityCurve1Colour();
	afx_msg void OnPorosityCurve2Colour();
	afx_msg void OnPorosityCurve3Colour();
	afx_msg void OnPorosityCurve4Colour();
	afx_msg void OnPorosityCurve5Colour();
	afx_msg void OnChangeEditFrequency();
	afx_msg void OnChangeEditProbeDiameter();
	afx_msg void OnChangeEditSquirterDiameter();
	afx_msg void OnPorosityDeleteall();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnChangeEditRefTarget();
	afx_msg void OnChangeEditRefAttenuation();
	afx_msg void OnChangeEditStandardId();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POROSITYPAGE_H__09366FEC_7164_4C1E_B9CB_BDDCD486CC6A__INCLUDED_)
