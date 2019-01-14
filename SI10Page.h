#include "afxwin.h"
#if !defined(AFX_SI10PAGE_H__133631C0_D8F9_49EF_ABF4_12866E58365E__INCLUDED_)
#define AFX_SI10PAGE_H__133631C0_D8F9_49EF_ABF4_12866E58365E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SI10Page.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSI10Page dialog

class CSI10Page : public CPropertyPage
{
	DECLARE_DYNCREATE(CSI10Page)

// Construction
public:
	HBRUSH m_hBr;
	int m_nOldJoyValue[4];
	
	void UpdateAllControls();
	CSI10Page();
	~CSI10Page();

// Dialog Data
	//{{AFX_DATA(CSI10Page)
	enum { IDD = IDD_SI10_PAGE };
	CButton	m_checkForceFirmwareLoad;
	CComboBox	m_comboGPIOIrqMode;
	CEdit	m_editSwitchEor;
	CEdit	m_editJoystick3;
	CEdit	m_editJoystick2;
	CEdit	m_editJoystick1;
	CEdit	m_editJoystick0;
	CString	m_strVersion;
	int		m_nIRQCount0;
	int		m_nIRQCount1;
	int		m_nIRQCount2;
	int		m_nIRQCount3;
	int		m_nIRQCount4;
	int		m_nIRQCount5;
	int		m_nIRQCount6;
	int		m_nIRQCount7;
	int		m_nIRQCount8;
	int		m_nIRQCount9;
	int		m_nIRQCount;
	int		m_nPRFIRQCount;
	int		m_nEvents;
	CString	m_strProcessState;
	CString	m_strLimitSwitches;
	CString	m_strFirmwareVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSI10Page)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSI10Page)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonZeroIrqCount();
	afx_msg void OnChangeEditSwitchEor();
	afx_msg void OnSelchangeComboGpioIrqMode();
	afx_msg void OnCheckForceFirmwareLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonReset();
	CEdit m_editISRCount;
	afx_msg void OnBnClickedButtonStartIsr();
	afx_msg void OnBnClickedButtonClearInterruptStatus();
	CEdit m_editDPCEvents;
	CEdit m_editGPInput;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SI10PAGE_H__133631C0_D8F9_49EF_ABF4_12866E58365E__INCLUDED_)
