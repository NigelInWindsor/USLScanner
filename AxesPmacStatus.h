#if !defined(AFX_AXESPMACSTATUS_H__8EAA5445_E78C_487B_B1A1_116FBFD8CB1E__INCLUDED_)
#define AFX_AXESPMACSTATUS_H__8EAA5445_E78C_487B_B1A1_116FBFD8CB1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxesPmacStatus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxesPmacStatus dialog
#include "AxisServoStatus.h"

class CAxesPmacStatus : public CPropertyPage
{
	DECLARE_DYNCREATE(CAxesPmacStatus)

// Construction
public:
	CAxisServoStatus* m_pAxisServoStatus[32];

	HBRUSH m_hBr;
	
	void UpdateControls();
	int	m_nBrakeStatus;
	int m_nGateStatus;
	int	m_nDoorStatus;
	int m_nEmergencyStatus;
	CAxesPmacStatus();
	~CAxesPmacStatus();

// Dialog Data
	//{{AFX_DATA(CAxesPmacStatus)
	enum { IDD = IDD_AXES_STATUS_DIALOG };
	CButton	m_checkScannerBrakeOnOff;
	CButton	m_checkManipulatorInterlocks;
	CButton	m_checkBrakeInterlocks;
	CEdit	m_editHoldingBrakes;
	CEdit	m_editScannerBrakes;
	CEdit	m_editScratch;
	CComboBox	m_comboBrakeMode1;
	CComboBox	m_comboBrake1;
	CComboBox	m_comboDoorAction;
	CButton	m_checkDoorStatus;
	CComboBox	m_comboGateAction;
	CButton	m_checkESwitchStatus;
	CButton	m_checkGateStatus;
	CComboBox	m_comboBrakeMode;
	CComboBox	m_comboBrake;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAxesPmacStatus)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAxesPmacStatus)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboBrake();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSelchangeComboZbrakeMode();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboGateAction();
	afx_msg void OnSelchangeComboDoorAction();
	afx_msg void OnSelchangeComboZbrakeMode2();
	afx_msg void OnSelchangeComboBrake2();
	afx_msg void OnCheckPlcBrakeInterlocks();
	afx_msg void OnCheckPlcManipulatorSensors();
	afx_msg void OnCheckManualScannerBrake();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXESPMACSTATUS_H__8EAA5445_E78C_487B_B1A1_116FBFD8CB1E__INCLUDED_)
