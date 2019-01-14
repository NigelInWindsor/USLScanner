#if !defined(AFX_TIMESLOTPR30_H__44F140D1_B9B3_423D_9084_C9550AFD8512__INCLUDED_)
#define AFX_TIMESLOTPR30_H__44F140D1_B9B3_423D_9084_C9550AFD8512__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeSlotPr30.h : header file
//
#include "EditSpinItem.h"

/////////////////////////////////////////////////////////////////////////////
// CTimeSlotPr30 dialog

class CTimeSlotPr30 : public CPropertyPage
{
	DECLARE_DYNCREATE(CTimeSlotPr30)

// Construction
public:
	void RedrawControl(CWnd* pStatic, CWnd* pCtrl, CSpinButtonCtrl* pSpin, __int64  n64Mask, CRect *prr);
	void RedrawLayout();
	void CopyTimeSlotVariables();
	void CheckPreAmpTSVariables();
	void UpdateDacTable();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	int m_nDacIndex;
	void CloseEditSpin();
	CEditSpinItem* m_pEditSpinItem;
	void FillList();
	void CreateColumnList();
	void SetAccessPrivelage();
	void SetFilterRangeCombo();
	void UpdateAllControls();
	int	m_nTS;
	bool m_bSubset;
	CTimeSlotPr30(UINT nCap=0,CWnd* pParent=NULL,int nWhichTS=0,bool bSubset = false);
	~CTimeSlotPr30();

// Dialog Data
	//{{AFX_DATA(CTimeSlotPr30)
	enum { IDD = IDD_PR30_TIMESLOT_DIALOG };
	HBRUSH m_hBr;
	CSliderCtrl	m_sliderReject;
	CStatic	m_text3;
	CStatic	m_text2;
	CStatic	m_text0;
	CStatic	m_text1;
	CButton	m_buttonGroup;
	CEdit	m_editStr;
	CComboBox	m_comboTX;
	CComboBox	m_comboRX;
	CComboBox	m_comboHTOffOn;
	CComboBox	m_comboLFPoint;
	CSpinButtonCtrl	m_spinPRF;
	CEdit	m_editPRF;
	CSpinButtonCtrl	m_spinDacTriggerThreshold;
	CEdit	m_editDacTriggerThreshold;
	CSpinButtonCtrl	m_spinDacBlanking;
	CListCtrl	m_listDac;
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
	CSpinButtonCtrl	m_spinPulseVoltage;
	CEdit	m_editPulseVoltage;
	CSpinButtonCtrl	m_spinPulseWidth;
	CEdit	m_editPulseWidth;
	CComboBox	m_comboDamping;
	CComboBox	m_comboPreampMode;
	CComboBox	m_comboPreampGain;
	CString	m_staticBlankingText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTimeSlotPr30)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTimeSlotPr30)
	afx_msg void OnSelchangeComboPreampGain();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboPreampMode();
	afx_msg void OnSelchangeComboDamping();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditPulseWidth();
	afx_msg void OnChangeEditPulseVoltage();
	afx_msg void OnSelchangeComboAmplifier();
	afx_msg void OnChangeEditLogGain();
	afx_msg void OnSelchangeComboPostFilter();
	afx_msg void OnChangeEditLinGain();
	afx_msg void OnSelchangeComboFilterType();
	afx_msg void OnSelchangeComboFilterRange();
	afx_msg void OnChangeEditCentreFreq();
	afx_msg void OnChangeEditRectifierDecay();
	afx_msg void OnSelchangeComboRectifierMode();
	afx_msg void OnSelchangeComboDacMode();
	afx_msg void OnSelchangeComboDacTriggerMode();
	afx_msg void OnChangeEditDacThreshold();
	afx_msg void OnChangeEditDacBlanking();
	afx_msg void OnGetdispinfoListDacList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDacList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HRESULT DacTableChanged(WPARAM, LPARAM);
	afx_msg void OnClickListDacList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListDacList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRbdacmenuClearallgains();
	afx_msg void OnRbdacmenuCleareverything();
	afx_msg void OnChangeEditPrf();
	afx_msg void OnSelchangeComboLfPoint();
	afx_msg void OnRbdacmenuDeletept();
	afx_msg void OnSelchangeComboHtOffOn();
	afx_msg void OnSelchangeComboTx();
	afx_msg void OnSelchangeComboRx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMESLOTPR30_H__44F140D1_B9B3_423D_9084_C9550AFD8512__INCLUDED_)
