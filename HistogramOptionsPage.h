#if !defined(AFX_HISTOGRAMOPTIONSPAGE_H__45210A0B_0C02_4B50_9D49_64FB8012F971__INCLUDED_)
#define AFX_HISTOGRAMOPTIONSPAGE_H__45210A0B_0C02_4B50_9D49_64FB8012F971__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistogramOptionsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistogramOptionsPage dialog

class CHistogramOptionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CHistogramOptionsPage)

// Construction
public:
	HBRUSH m_hBr;
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void UpdateAllControls();
	CWnd* m_pParent;
	CHistogramOptionsPage(CWnd* pParent=NULL);
	~CHistogramOptionsPage();

// Dialog Data
	//{{AFX_DATA(CHistogramOptionsPage)
	enum { IDD = IDD_HISTOGRAM_OPTIONS_PAGE };
	CEdit	m_editMaxDropOut;
	CStatic	m_staticPaintDropOut;
	CEdit	m_editAspectRatio;
	CEdit	m_editArea;
	CEdit	m_editHistogramSize;
	CButton	m_checkEnterSize;
	CStatic	m_staticStyleCircle;
	CStatic	m_staticStyleBox;
	CSpinButtonCtrl	m_spinFontSize;
	CEdit	m_editFontSize;
	CButton	m_checkPaint1;
	CButton	m_checkPaint0;
	CStatic	m_staticPaint1;
	CStatic	m_staticPaint0;
	CComboBox	m_comboAmpScaleType;
	CComboBox	m_comboThresholdType;
	CSpinButtonCtrl	m_spinDBRange;
	CEdit	m_editDBRange;
	CEdit	m_editDBChange;
	CSpinButtonCtrl	m_spinDBChange;
	CComboBox	m_comboThresholdMode;
	CButton	m_checkMaxWidth;
	CButton	m_checkMaxHeight;
	CListBox	m_listAreaOption;
	CStatic	m_staticView1;
	CStatic	m_staticView0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHistogramOptionsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CHistogramOptionsPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeListAreaOption();
	afx_msg void OnCheckMaxWidth();
	afx_msg void OnCheckMaxHeight();
	afx_msg void OnSelchangeComboThrsholdMode();
	afx_msg void OnChangeEditDbChange();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckPaint0();
	afx_msg void OnChangeEditThreshRange();
	afx_msg void OnSelchangeComboThresholdType();
	afx_msg void OnSelchangeComboScaleType();
	afx_msg void OnCheckPaint1();
	afx_msg void OnChangeEditFontSize();
	afx_msg void OnCheckFixedsize();
	afx_msg void OnChangeEditHistSize();
	afx_msg void OnChangeEditAspectRatio();
	afx_msg void OnChangeEditArea();
	afx_msg void OnChangeEditMaxDropOut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTOGRAMOPTIONSPAGE_H__45210A0B_0C02_4B50_9D49_64FB8012F971__INCLUDED_)
