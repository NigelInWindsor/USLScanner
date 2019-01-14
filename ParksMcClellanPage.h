#if !defined(AFX_PARKSMCCLELLANPAGE_H__4D74E4E5_A76E_445B_9651_4D57D72E8217__INCLUDED_)
#define AFX_PARKSMCCLELLANPAGE_H__4D74E4E5_A76E_445B_9651_4D57D72E8217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParksMcClellanPage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CParksMcClellanPage dialog

class CParksMcClellanPage : public CResizablePage
{
	DECLARE_DYNCREATE(CParksMcClellanPage)

// Construction
public:
	float m_fMaxFrequency;
	void RenderFrequency(CPaintDC* pDC,int freqPoints,double f[]);
	void RenderPhase(CPaintDC* pDC,int numTaps,double a[]);
	bool CalculateFilter();
	float MinMax(float fMin,float fMax,float &fValue);
	
	void UpdateAllControls();
	CParksMcClellanPage();
	~CParksMcClellanPage();

// Dialog Data
	//{{AFX_DATA(CParksMcClellanPage)
	enum { IDD = IDD_PARKSMCCLELLAN_PAGE };
	CEdit	m_editNumberTaps;
	CStatic	m_staticFrequency;
	CStatic	m_staticPhase;
	CStatic	m_static4;
	CSliderCtrl	m_slider4;
	CEdit	m_edit4;
	CStatic	m_static3;
	CStatic	m_static2;
	CStatic	m_static1;
	CStatic	m_static0;
	CSliderCtrl	m_slider3;
	CSliderCtrl	m_slider2;
	CSliderCtrl	m_slider1;
	CSliderCtrl	m_slider0;
	CEdit	m_edit3;
	CEdit	m_edit2;
	CEdit	m_edit1;
	CEdit	m_edit0;
	CComboBox	m_comboFilterType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CParksMcClellanPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CParksMcClellanPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboFilterType();
	afx_msg void OnChangeEdit0();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEdit4();
	afx_msg void OnButtonCompute();
	afx_msg void OnButtonEstimate();
	afx_msg void OnChangeEditNumberTaps();
	afx_msg void OnButtonClearFilter();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARKSMCCLELLANPAGE_H__4D74E4E5_A76E_445B_9651_4D57D72E8217__INCLUDED_)
