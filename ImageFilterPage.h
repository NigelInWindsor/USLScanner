#if !defined(AFX_IMAGEFILTERPAGE_H__2BDC746A_CF18_4430_AB76_52392221F5C1__INCLUDED_)
#define AFX_IMAGEFILTERPAGE_H__2BDC746A_CF18_4430_AB76_52392221F5C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageFilterPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageFilterPage dialog

class CImageFilterPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CImageFilterPage)

// Construction
public:
	HBRUSH m_hBr;
	
	void UpdateAllControls();
	CWnd* m_pViewSheet;
	CImageFilterPage(CWnd* pViewSheet = NULL);
	~CImageFilterPage();

// Dialog Data
	//{{AFX_DATA(CImageFilterPage)
	enum { IDD = IDD_IMAGE_FILTER_PAGE };
	CComboBox	m_comboDisplayMode;
	CProgressCtrl	m_progressBar;
	CComboBox	m_comboTimePriority;
	CSpinButtonCtrl	m_spinOD;
	CSpinButtonCtrl	m_spinID;
	CEdit	m_editOD;
	CEdit	m_editID;
	CButton	m_checkRectilinear;
	CButton	m_checkPolar;
	CComboBox	m_comboOrientation;
	CComboBox	m_comboAmpPriority;
	CComboBox	m_comboWhichLines;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CImageFilterPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CImageFilterPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboWhichLines();
	afx_msg void OnSelchangeComboAmpPriorrity();
	afx_msg void OnSelchangeComboOrientation();
	afx_msg void OnCheckPolar();
	afx_msg void OnCheckRectilinear();
	afx_msg void OnChangeEditId();
	afx_msg void OnChangeEditOd();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboTimePriorrity();
	afx_msg void OnButtonRotate90();
	afx_msg void OnSelchangeComboDisplayMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CButton m_radioRectilinear;
	CButton m_radioPolar;
	afx_msg void OnBnClickedRadioRectilinear();
	afx_msg void OnBnClickedRadioPolar();
	CComboBox m_comboPolarOrientation;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEFILTERPAGE_H__2BDC746A_CF18_4430_AB76_52392221F5C1__INCLUDED_)
