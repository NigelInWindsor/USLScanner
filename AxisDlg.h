#if !defined(AFX_AXES_H__F9A9DE4E_3FFE_11D5_847E_60475CC10000__INCLUDED_)
#define AFX_AXES_H__F9A9DE4E_3FFE_11D5_847E_60475CC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Axes.h : header file
//
#include "Si6.h"

/////////////////////////////////////////////////////////////////////////////
// CAxisDlg dialog

class CAxisDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CAxisDlg)

// Construction
public:
	HBRUSH m_hBr;
	int m_nTab;
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	CAxisDlg(UINT nCap=0,CWnd* pParent=NULL,CWnd** DlgPtr=NULL,int nWhichAxis=0,int nTab=0);
	~CAxisDlg();

	// Dialog Data
	//{{AFX_DATA(CAxisDlg)
	enum { IDD = IDD_AXIS_DIALOG };
	CEdit	m_editKinematicOffset;
	CEdit	m_editName;
	CButton	m_checkUseFiducial;
	CButton	m_checkHomeSwitch;
	CButton	m_checkMinSwitch;
	CButton	m_checkMaxSwitch;
	CEdit	m_editPrecision;
	CEdit	m_editParkPos;
	CButton	m_JoystickAvailable;
	CComboBox	m_comboTogglePos;
	CEdit	m_editJoyMinDeviation;
	CEdit	m_editJoyMaxDeviation;
	CEdit	m_editMotorRatio;
	CEdit	m_editInPosError;
	CComboBox	m_comboEncoderMode;
	CEdit	m_editEncoderNumber;
	CEdit	m_editHomeOffset;
	CEdit	m_editStepsRev;
	CEdit	m_editMinTravel;
	CEdit	m_editMaxTravel;
	CButton	m_MotorReverse;
	CButton	m_EncoderReverse;
	CEdit	m_editMinSwitch;
	CEdit	m_editMaxSwitch;
	CEdit	m_editHomeSwitch;
	CComboBox	m_comboHomeMode;
	CButton	m_JoystickReverse;
	CButton	m_AxisAvailableCheck;
	CEdit	m_AxisJoystickMinSpeedEdit;
	CComboBox	m_AxisUnitsCombo;
	CEdit	m_AxisAccelerationEdit;
	CEdit	m_AxisBitEdit;
	CEdit	m_AxisCounterEdit;
	CEdit	m_AxisDecelerationEdit;
	CEdit	m_AxisHomeSpeedEdit;
	CEdit	m_AxisJoystickGainEdit;
	CEdit	m_AxisJoystickMaxSpeedEdit;
	CEdit	m_AxisJoystickNumberEdit;
	CEdit	m_AxisMaxSpeedEdit;
	CEdit	m_AxisMinSpeedEdit;
	CEdit	m_AxisStepsizeEdit;
	CString	m_stringPos;
	CString	m_stringEncoderPos;
	CString	m_stringCount;
	CString	m_stringCurrentSpeed;
	CString	m_stringJoyValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAxisDlg)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateAllControls();
	void	Defaults(int nWhich);

protected:
	void MinMax(int *v0,int v1,int v2);
	CWnd** m_DlgPtr;
	struct	AxisData	*m_pAxis;
	CSi6*	m_pSi6User;
	char	Caption[40];
	int		m_nCap;
	int		m_nWhichAxis;

	// Generated message map functions
	//{{AFX_MSG(CAxisDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnAcceptButton();
	afx_msg void OnAxisAvailableCheck();
	afx_msg void OnDefaultButton();
	afx_msg void OnCheckJoystickReverse();
	afx_msg void OnCheckMotorReverse();
	afx_msg void OnCheckEncoderReverse();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboEncoderMode();
	afx_msg void OnChangeEditJoyMinDev();
	afx_msg void OnChangeEditJoyMaxDev();
	afx_msg void OnCheckJoyAvailable();
	afx_msg void OnButtonHmz();
	afx_msg void OnCheckUseFiducial();
	afx_msg void OnButtonLimitSwitchOffset();
	afx_msg void OnChangeEditName();
	afx_msg void OnChangeEditKinematicOffset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXES_H__F9A9DE4E_3FFE_11D5_847E_60475CC10000__INCLUDED_)
