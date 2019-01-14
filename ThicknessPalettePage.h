#if !defined(AFX_THICKNESSPALETTEPAGE_H__262EA484_7700_4FCD_BE42_5248842C974A__INCLUDED_)
#define AFX_THICKNESSPALETTEPAGE_H__262EA484_7700_4FCD_BE42_5248842C974A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThicknessPalettePage1.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CThicknessPalettePage dialog

class CThicknessPalettePage : public CResizablePage
{
	DECLARE_DYNCREATE(CThicknessPalettePage)

// Construction
public:
	void SetAccessPrivelage();
	int m_nWhich;
	void UpdateAllControls();
	void UpdateImages();
	void AddAnchor(CWnd* pCtrl);

	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	
	HBRUSH m_hBr;

	void RefreshBarGraph(CPaintDC *pDC,CRect rr);
	void RefreshEntries(CPaintDC *pDC);

	CThicknessPalettePage();
	~CThicknessPalettePage();

// Dialog Data
	//{{AFX_DATA(CThicknessPalettePage)
	enum { IDD = IDD_PALETTE_THICK_DIALOG1 };
	CStatic	m_static4;
	CStatic	m_static3;
	CStatic	m_static2;
	CStatic	m_static1;
	CStatic	m_static0;
	CSpinButtonCtrl	m_spinNumberOfEntries;
	CEdit	m_editNumberOfEntries;
	CSpinButtonCtrl	m_spinRange;
	CEdit	m_editRange;
	CEdit	m_editMax;
	CComboBox	m_comboMode;
	CEdit	m_editMin;
	CSpinButtonCtrl	m_spinStepSize;
	CSpinButtonCtrl	m_spinMin;
	CSpinButtonCtrl	m_spinMax;
	CEdit	m_editStepSize;
	CStatic	m_staticBarGraph;
	CStatic	m_staticEntries;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CThicknessPalettePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CThicknessPalettePage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnChangeEditStepsize();
	afx_msg void OnChangeEditMin();
	afx_msg void OnChangeEditRange();
	afx_msg void OnChangeEditNumberOfEntries();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnDeltaposSpinRange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinStepsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinMin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THICKNESSPALETTEPAGE_H__262EA484_7700_4FCD_BE42_5248842C974A__INCLUDED_)
