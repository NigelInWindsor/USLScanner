#if !defined(AFX_LSAPAGE_H__2E348262_B8A6_4D5C_B170_1599B00E6A3D__INCLUDED_)
#define AFX_LSAPAGE_H__2E348262_B8A6_4D5C_B170_1599B00E6A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LSAPage.h : header file
//
#include "EditSpinItem.h"

/////////////////////////////////////////////////////////////////////////////
// CLSAPage dialog

class CLSAPage : public CPropertyPage
{
// Construction
public:
	void UpdateDacTable();
	void SetFilterRangeCombo();
	void SetAccessPrivelage();
	HBRUSH m_hBr;
	Pr30Data* m_pPr30;
	int	m_nTS;
	int	m_nDacIndex;
	CEditSpinItem* m_pEditSpinItem;
	void FillList();
	void CreateColumns();
	CLSAPage(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CLSAPage)
	enum { IDD = IDD_LSA_PAGE };
	CSpinButtonCtrl	m_spinPRF;
	CEdit	m_editPRF;
	CStatic	m_text3;
	CStatic	m_text2;
	CStatic	m_text0;
	CStatic	m_text1;
	CButton	m_buttonGroup;
	CEdit	m_editStr;
	CComboBox	m_comboLFPoint;
	CSpinButtonCtrl	m_spinDacTriggerThreshold;
	CEdit	m_editDacTriggerThreshold;
	CSpinButtonCtrl	m_spinDacBlanking;
	CEdit	m_editDacBlanking;
	CComboBox	m_comboDacTriggerMode;
	CComboBox	m_comboDacEnable;
	CSpinButtonCtrl	m_spinRectifierDecay;
	CEdit	m_editRectifierDecay;
	CComboBox	m_comboRectifierMode;
	CSpinButtonCtrl	m_spinCentreFrequency;
	CEdit	m_editCentreFrequency;
	CComboBox	m_comboFilterRange;
	CComboBox	m_comboFilterType;
	CSpinButtonCtrl	m_spinLinGain;
	CEdit	m_editLinGain;
	CComboBox	m_comboLogPostFilter;
	CSpinButtonCtrl	m_spinLogGain;
	CEdit	m_editLogGain;
	CComboBox	m_comboAmplifier;
	CString	m_staticBlankingText;
	CListCtrl	m_listDac;
	CComboBox	m_comboGate;
	CButton	m_buttonNormalize;
	CButton	m_checkReverseElementOrder;
	CSpinButtonCtrl	m_spinStartElement;
	CEdit	m_editStartElement;
	CEdit	m_editFocalLawGain;
	CSpinButtonCtrl	m_spinFocalLawGain;
	CComboBox	m_comboScanPitch;
	CEdit	m_editSweepWidth;
	CComboBox	m_comboProbeType;
	CSpinButtonCtrl	m_spinDOF;
	CComboBox	m_comboFocused;
	CEdit	m_editDOF;
	CComboBox	m_comboLowPass;
	CComboBox	m_comboHighPass;
	CSpinButtonCtrl	m_spinVoltage;
	CEdit	m_editVoltage;
	CComboBox	m_comboPreAmpGain;
	CSpinButtonCtrl	m_spinViewLaw;
	CEdit	m_editViewLaw;
	CSpinButtonCtrl	m_spinPulseWidth;
	CEdit	m_editPulseWidth;
	CStatic	m_staticView;
	CEdit	m_editStart;
	CSpinButtonCtrl	m_spinElements;
	CEdit	m_editElements;
	//}}AFX_DATA
	
	void UpdateUTGlobalSettings();
	void UpdateAllControls();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLSAPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLSAPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditPulsewidth();
	afx_msg void OnChangeEditViewlaw();
	afx_msg void OnSelchangeComboPreampgain();
	afx_msg void OnChangeEditVoltage();
	afx_msg void OnSelchangeComboLowPass();
	afx_msg void OnSelchangeComboHighPass();
	afx_msg void OnButtonNormalize();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditFocalLawGain();
	afx_msg void OnButtonZeroGains();
	afx_msg void OnDeltaposSpinFocalLawGain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboGate();
	afx_msg void OnClickListDacList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDacList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListDacList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HRESULT DacTableChanged(WPARAM, LPARAM);
	afx_msg void OnRclickListDacList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditLinGain();
	afx_msg void OnSelchangeComboAmplifier();
	afx_msg void OnChangeEditDacBlanking();
	afx_msg void OnSelchangeComboDacTriggerMode();
	afx_msg void OnSelchangeComboDacMode();
	afx_msg void OnSelchangeComboRectifierMode();
	afx_msg void OnChangeEditRectifierDecay();
	afx_msg void OnSelchangeComboFilterRange();
	afx_msg void OnSelchangeComboFilterType();
	afx_msg void OnChangeEditLogGain();
	afx_msg void OnSelchangeComboPostFilter();
	afx_msg void OnSelchangeComboLfPoint();
	afx_msg void OnDacClearallgains();
	afx_msg void OnDacCleareverything();
	afx_msg void OnDacDeletept();
	afx_msg void OnChangeEditPrf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	int m_nSweepLineHeight;

	int m_nFLS;
	int m_nHighPassFilter;
	int m_nLowPassFilter;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LSAPAGE_H__2E348262_B8A6_4D5C_B170_1599B00E6A3D__INCLUDED_)
