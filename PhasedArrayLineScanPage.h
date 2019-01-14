#pragma once
#include "EditSpinCtrl.h"

class CPhasedArrayLineScanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPhasedArrayLineScanPage)

public:
	HBRUSH	m_hBr;
	CPhasedArrayLineScanPage();
	~CPhasedArrayLineScanPage();

// Dialog Data
	//{{AFX_DATA(CContourScanPage)
	enum { IDD = IDD_PHASEDARRAY_LINE_SCAN };
	//}}AFX_DATA

protected:
	int m_nLineNumber;
	CEdit m_editFilePath;
	CEditSpinCtrl m_editSpinLineNumber;

	DECLARE_MESSAGE_MAP()
	void UpdateAllControls();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	//}}AFX_MSG
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	afx_msg void OnEnChangeEditFilepath();
	afx_msg void OnEnChangeEditLineNumber();
	afx_msg void OnDeltaposSpinLineNumber(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	CComboBox m_comboScanSide;
	afx_msg void OnCbnSelchangeComboScanSide();
	CEdit m_editScanSpeed;
	CSpinButtonCtrl m_spinScanSpeed;
	afx_msg void OnEnChangeEditScanSpeed();
	afx_msg void OnDeltaposSpinScanSpeed(NMHDR *pNMHDR, LRESULT *pResult);
};

