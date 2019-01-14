#if !defined(AFX_NORMALIZEPAGE_H__7C5C9714_8CF4_4707_B88E_93EC167935CC__INCLUDED_)
#define AFX_NORMALIZEPAGE_H__7C5C9714_8CF4_4707_B88E_93EC167935CC__INCLUDED_

#include "PolyCoord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NormalizePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNormalizePage dialog

class CNormalizePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CNormalizePage)

// Construction
public:
	HBRUSH m_hBr;
	CPoint m_ptClient;
	int m_nImageOrientation;
	COLORREF m_colorrefCrossHair[256];
	CPoint	m_pointCrossHair;
	bool	m_bCrossVisible;

	long MinMax(long *pnV, int nMin, int nMax);
	int MinMax(int *pnV, int nMin, int nMax);
	void WorldToClient(CCoord Cp,int nSide,CPoint *pt);
	void FindRectilinearCentre(int *pnSample,int *pnLine);
	void RenderRectilinear(CPaintDC *pDC);
	void RenderCrissCross(CPaintDC* pDC);
	char *m_pRectilinearArray;
	CRect m_RectilinearRect;
	void TransferRectilinearData(CPolyCoord *pLine,int nNumberLines,int nLine);
	void ShowAndEnableWindow(CWnd* pWnd,bool bShowFlag,bool bEnableFlag);
	void SetAccessPrivelages();
	CCoord m_CpHorizontalCentre;
	CCoord m_CpVerticalCentre;
	CPolyCoord m_CpHorizontal;
	CPolyCoord m_CpVertical;
	void TransferData(int nAxis,CPolyCoord* pLine,CCoord Cp);
	void UpdateAllControls();
	int	m_nPaintMode;
	CNormalizePage();
	~CNormalizePage();

// Dialog Data
	//{{AFX_DATA(CNormalizePage)
	enum { IDD = IDD_NORMALIZE_PAGE };
	CButton	m_buttonMove1To2;
	CButton	m_buttonMove2To1;
	CButton	m_staticRight;
	CButton	m_staticLeft;
	CComboBox	m_comboRightWpTS;
	CComboBox	m_comboLeftWpTS;
	CComboBox	m_comboPeakTimeSlot;
	CComboBox	m_comboNormTimeSlot;
	CComboBox	m_comboNormSide;
	CComboBox	m_comboPeakSide;
	CComboBox	m_comboPeakOrthogonalMode;
	CComboBox	m_comboPeakMode;
	CButton	m_checkYTip;
	CButton	m_checkXTip;
	CEdit	m_editDistance;
	CSpinButtonCtrl	m_spinDistance;
	CComboBox	m_comboGatePeak;
	CStatic	m_staticView;
	CComboBox	m_comboGate;
	CComboBox	m_comboFindMode;
	CSpinButtonCtrl	m_spinAngle;
	CEdit	m_editAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNormalizePage)
	public:
	virtual void OnSetFont(CFont* pFont);
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNormalizePage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnButtonNormalize();
	afx_msg void OnButtonPeakOpposite();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditAngle();
	afx_msg void OnSelchangeComboGate();
	afx_msg void OnSelchangeComboTimeslot();
	afx_msg void OnPaint();
	afx_msg void OnChangeEditDistance();
	afx_msg void OnSelchangeComboGate2();
	afx_msg void OnButtonWaterpathLeft();
	afx_msg void OnButtonWaterpathRight();
	afx_msg void OnButtonMovetoLeft();
	afx_msg void OnButtonMovetoRight();
	afx_msg void OnSelchangeComboPeakSide();
	afx_msg void OnCheckXtip();
	afx_msg void OnCheckYtip();
	afx_msg void OnSelchangeComboPeakMode();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSelchangeComboInspectionMode();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboNormSide();
	afx_msg void OnSelchangeComboPeakTimeslot();
	afx_msg void OnDeltaposSpinDistance(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboLeftWpTs();
	afx_msg void OnSelchangeComboRightWpTs();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnNormalizeGoto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NORMALIZEPAGE_H__7C5C9714_8CF4_4707_B88E_93EC167935CC__INCLUDED_)
