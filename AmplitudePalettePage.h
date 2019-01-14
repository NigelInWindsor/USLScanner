//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_AMPLITUDEPALETTEPAGE_H__BFF5CFF4_5F30_48B4_940F_4B8EA65190AF__INCLUDED_)
#define AFX_AMPLITUDEPALETTEPAGE_H__BFF5CFF4_5F30_48B4_940F_4B8EA65190AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AmplitudePalettePage1.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CAmplitudePalettePage dialog

class CAmplitudePalettePage : public CResizablePage
{
	DECLARE_DYNCREATE(CAmplitudePalettePage)

// Construction
public:
	void SetTopThresholdText();
	bool m_bMoveSlider;
	void SetAccessPrivelage();
	int m_nSlideEntry;
	int m_nWhichAmp;
	void UpdateAllControls();
	void UpdateRelatedDialogs();
	int m_nScaleType;
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void RefreshSlider(CPaintDC *pDC,CRect rr);
	void RefreshBarGraph(CPaintDC *pDC,CRect rr,int nWhichAmp = -1,LOGFONT *plf = NULL,int ndevice = SCREEN);
	void RefreshBarGraphTotal(CPaintDC *pDC,CRect rr,LOGFONT *plf = NULL,int ndevice = SCREEN);
	void RefreshBarGraphWindow(CPaintDC *pDC,CRect rr,int ndevice = SCREEN);
	void SlowRefreshBarGraph(CPaintDC *pDC,CRect rr);
	void RefreshEntries(CPaintDC *pDC);
	
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	void AddAnchor(CWnd* pCtrl);
	CAmplitudePalettePage(int nWhichAmp = 0);
	~CAmplitudePalettePage();

// Dialog Data
	//{{AFX_DATA(CAmplitudePalettePage)
	enum { IDD = IDD_PALETTE_AMPLITUDE_DIALOG };
	CButton	m_staticThresholds;
	CEdit	m_editTopThreshold;
	CSpinButtonCtrl	m_spinTopThreshold;
	CComboBox	m_comboThresholdMode;
	CEdit	m_editThresholdSpan;
	CSpinButtonCtrl	m_spinThresholdSpan;
	CStatic	m_staticBottomColour;
	CStatic	m_staticTopColour;
	CSliderCtrl	m_sliderThreshold;
	CButton	m_buttonMakeItSo;
	CStatic	m_static8;
	CStatic	m_static7;
	CStatic	m_static6;
	CStatic	m_static5;
	CStatic	m_static4;
	CStatic	m_static3;
	CStatic	m_static2;
	CStatic	m_static1;
	CStatic	m_static0;
	CComboBox	m_comboStepMode;
	CStatic	m_staticSlider;
	CComboBox	m_comboGraphType;
	CSpinButtonCtrl	m_spinNumberOfEntries;
	CEdit	m_editNumberOfEntries;
	CComboBox	m_comboCopyTo;
	CComboBox	m_comboWhichGate;
	CSliderCtrl	m_sliderBrightness;
	CSliderCtrl	m_sliderZeroLevel;
	CSpinButtonCtrl	m_spinTopLevel;
	CEdit	m_editTopLevel;
	CSpinButtonCtrl	m_spinRange;
	CEdit	m_editRange;
	CComboBox	m_comboScaleType;
	CStatic	m_staticEntries;
	CStatic	m_staticBarGraph;
	CSpinButtonCtrl	m_spinStepSize;
	CEdit	m_editStepSize;
	CComboBox	m_comboMode;
	HBRUSH m_hBr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAmplitudePalettePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAmplitudePalettePage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnChangeEditStepsize();
	afx_msg void OnSelchangeComboScaleType();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnChangeEditTopLevel();
	afx_msg void OnSelchangeComboWhichGate();
	afx_msg void OnButtonMakeItSo();
	afx_msg void OnChangeEditNumberOfEntries();
	afx_msg void OnSelchangeComboGraphType();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboStepMode();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnChangeEditThresholdSpan();
	afx_msg void OnDeltaposSpinThresholdSpan(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckEnableThreshold();
	afx_msg void OnSelchangeComboThresholdMode();
	afx_msg void OnChangeEditTopThreshold();
	afx_msg void OnDeltaposSpinTopThreshold(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDeltaposSpinStepsize(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMPLITUDEPALETTEPAGE_H__BFF5CFF4_5F30_48B4_940F_4B8EA65190AF__INCLUDED_)
