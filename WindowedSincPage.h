#if !defined(AFX_WINDOWEDSINCPAGE_H__1A381333_D206_4119_B531_E27CB0C0432F__INCLUDED_)
#define AFX_WINDOWEDSINCPAGE_H__1A381333_D206_4119_B531_E27CB0C0432F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindowedSincPage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CWindowedSincPage dialog

class CWindowedSincPage : public CResizablePage
{
	DECLARE_DYNCREATE(CWindowedSincPage)

// Construction
public:
	void RenderFrequency(CPaintDC* pDC,int freqPoints,double f[]);
	void RenderPhase(CPaintDC* pDC,int numTaps,double a[]);
	void CalculateFilter();
	
	void UpdateAllControls();
	float m_fMaxFrequency;
	CWindowedSincPage();
	~CWindowedSincPage();

// Dialog Data
	//{{AFX_DATA(CWindowedSincPage)
	enum { IDD = IDD_WINDOWED_SINC_PAGE };
	CStatic	m_staticPhase;
	CStatic	m_staticFrequency;
	CStatic	m_static2;
	CStatic	m_static1;
	CStatic	m_static0;
	CSliderCtrl	m_slider2;
	CSliderCtrl	m_slider1;
	CSliderCtrl	m_slider0;
	CEdit	m_edit2;
	CEdit	m_edit1;
	CEdit	m_edit0;
	CComboBox	m_comboWindowType;
	CComboBox	m_comboFilterType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWindowedSincPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWindowedSincPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit0();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboFilterType();
	afx_msg void OnSelchangeComboWindow();
	afx_msg void OnButtonCompute();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOWEDSINCPAGE_H__1A381333_D206_4119_B531_E27CB0C0432F__INCLUDED_)
