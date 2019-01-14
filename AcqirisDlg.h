#if !defined(AFX_ACQIRISDLG_H__3CFD988F_6446_4850_92B9_6E1F2BAE9AB1__INCLUDED_)
#define AFX_ACQIRISDLG_H__3CFD988F_6446_4850_92B9_6E1F2BAE9AB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AcqirisDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAcqirisDlg dialog

class CAcqirisDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CAcqirisDlg)

		// Construction
public:
	HBRUSH m_hBr;
	void SetAccessPrivelages();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	void UpdateHardware();
	void SetWindowTitle();
	void UpdateAllControls();
	
	CAcqirisDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAcqirisDlg)
	enum { IDD = IDD_ACQIRIS_DIALOG };
	CStatic	m_staticDCOffset;
	CSpinButtonCtrl	m_spinMaxSamplingRate;
	CEdit	m_editMaxSamplingRate;
	CComboBox	m_comboBits;
	CEdit	m_editType;
	CSpinButtonCtrl	m_spinLogMin;
	CSpinButtonCtrl	m_spinLogMax;
	CButton	m_checkCalibrate;
	CSpinButtonCtrl	m_spinDACMaxThreshold;
	CEdit	m_editDACMaxThreshold;
	CSpinButtonCtrl	m_spinDCOffset;
	CEdit	m_editDCOffset;
	CSpinButtonCtrl	m_spinLogGraticuleScale;
	CEdit	m_editLogGraticuleScale;
	CEdit	m_editMainBangConstant;
	CEdit	m_editLogMin;
	CEdit	m_editLogMax;
	CComboBox	m_comboCoupling;
	CEdit	m_editIrq;
	CEdit	m_editMaxTimeBase;
	CSpinButtonCtrl	m_spinTriggerThreshold;
	CEdit	m_editTriggerThreshold;
	CComboBox	m_comboTriggerSource;
	CComboBox	m_comboFullScaleRange;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAcqirisDlg)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAcqirisDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboFullscaleRange();
	afx_msg void OnSelchangeComboTriggerSource();
	afx_msg void OnChangeEditTriggerThreshold();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditMaxTimebase();
	afx_msg void OnSelchangeComboCoupling();
	afx_msg void OnChangeEditLogMin();
	afx_msg void OnChangeEditLogMax();
	afx_msg void OnChangeEditMainbangConstant();
	afx_msg void OnChangeEditLogGraticule();
	afx_msg void OnChangeEditDcOffset();
	afx_msg void OnChangeEditDacMaxThreshold();
	afx_msg void OnCheckCalibrate();
	afx_msg void OnSelchangeComboBits();
	afx_msg void OnChangeEditMaxSamplingRate();
	afx_msg void OnDeltaposSpinMaxSamplingRate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinDcOffset(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editSquelchMax;
	CEdit m_editSquelchMin;
	CEdit m_editSquelchAttenuation;
	CSpinButtonCtrl m_spinSquelchMax;
	CSpinButtonCtrl m_spinSquelchMin;
	CSpinButtonCtrl m_spinSquelchAttenuation;
	afx_msg void OnEnChangeEditSquelchMax();
	afx_msg void OnEnChangeEditSquelchMin();
	afx_msg void OnEnChangeEditSquelchAttenuation();
	afx_msg void OnDeltaposSpinSquelchMax(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSquelchMin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSquelchAttenuation(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACQIRISDLG_H__3CFD988F_6446_4850_92B9_6E1F2BAE9AB1__INCLUDED_)
