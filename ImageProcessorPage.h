#if !defined(AFX_IMAGEPROCESSORPAGE_H__0818AB69_EEF8_4371_9498_6217E417A1D6__INCLUDED_)
#define AFX_IMAGEPROCESSORPAGE_H__0818AB69_EEF8_4371_9498_6217E417A1D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageProcessorPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageProcessorPage dialog

class CImageProcessorPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CImageProcessorPage)

// Construction
public:
	HBRUSH m_hBr;
	int	m_nImageType;
	int	m_nDest;
	int	m_nSrc;
	CWnd* m_pParent;
	CData*	m_pData;
	int	m_nFilter[10][5][5];
	void DrawGrid(CDC* pDC,CRect rr);
	void UpdateAllControls();
	COLORREF	*m_pArray;
	int	m_nRow;
	int	m_nColumn;
	
	bool m_bLButtonDblClk;
	CImageProcessorPage(CWnd* pParent = NULL);
	~CImageProcessorPage();

// Dialog Data
	//{{AFX_DATA(CImageProcessorPage)
	enum { IDD = IDD_IMAGE_PROCESSOR_PAGE };
	CComboBox	m_comboSrcType;
	CComboBox	m_comboSourceImage;
	CComboBox	m_comboDestination;
	CSliderCtrl	m_sliderThreshold;
	CComboBox	m_comboPreset;
	CEdit	m_edit;
	CStatic	m_staticFilter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessorPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CImageProcessorPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChangeEditValue();
	afx_msg void OnSelchangeComboPreset();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonDiferentiation();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonSobel();
	afx_msg void OnSelchangeComboSrcType();
	afx_msg void OnButton33Median();
	afx_msg void OnButton33Mean();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSORPAGE_H__0818AB69_EEF8_4371_9498_6217E417A1D6__INCLUDED_)
