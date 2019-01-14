#if !defined(AFX_SCOPEOPTIONSPAGE_H__35E35006_67F4_457B_90F2_2F8B8BFF3873__INCLUDED_)
#define AFX_SCOPEOPTIONSPAGE_H__35E35006_67F4_457B_90F2_2F8B8BFF3873__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScopeOptionsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScopeOptionsPage dialog

class CScopeOptionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CScopeOptionsPage)

// Construction
public:
	HBRUSH m_hBr;
	void SetAccessPrivelage();
	void BlockFill(CPaintDC* pDC,CStatic* mStatic, COLORREF rgbColor);
	void UpdateAllControls();
	CScopeOptionsPage();
	~CScopeOptionsPage();

// Dialog Data
	//{{AFX_DATA(CScopeOptionsPage)
	enum { IDD = IDD_SCOPE_OPTIONS_DIALOG };
	CComboBox	m_comboTOF;
	CComboBox	m_comboAverages;
	CComboBox	m_comboDacDelayUnits;
	CComboBox	m_comboDacBlankingUnits;
	CButton m_checkTimeSlot;
	CComboBox	m_comboNumberTimeSlots;
	CStatic	m_staticGraticuleColor;
	CStatic	m_staticBKColor;
	CStatic	m_staticTraceColor;
	CButton	m_checkMetalPath;
	CStatic	m_staticTextColor;
	CSpinButtonCtrl	m_spinTextSize;
	CEdit	m_editTextSize;
	CButton	m_checkGateAmp;
	CButton	m_checkWaterPath;
	CComboBox	m_comboTraceWidthUnits;
	CComboBox	m_comboTraceDelayUnits;
	CButton	m_checkGraticuleOffOn;
	CComboBox	m_comboGateWidthUnits;
	CComboBox	m_comboGateDelayUnits;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CScopeOptionsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CScopeOptionsPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnCheckGraticuleOffOn();
	afx_msg void OnSelchangeComboGateDelayUnits();
	afx_msg void OnSelchangeComboGateWidthUnits();
	afx_msg void OnSelchangeComboTraceWidthUnits();
	afx_msg void OnSelchangeComboTraceDelayUnits();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckWaterpath();
	afx_msg void OnCheckGateAmp();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnCheckMetalpath();
	afx_msg void OnChangeEditTextSize();
	afx_msg void OnSelchangeComboNumberTimeslots();
	afx_msg void OnCheckTimeslot();
	afx_msg void OnSelchangeComboDacDelayUnits();
	afx_msg void OnSelchangeComboDacBlankingUnits();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSelchangeComboaverages();
	afx_msg void OnSelchangeComboTof();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCOPEOPTIONSPAGE_H__35E35006_67F4_457B_90F2_2F8B8BFF3873__INCLUDED_)
