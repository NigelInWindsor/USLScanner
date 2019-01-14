#if !defined(AFX_AXISPMACDLG_H__16726E10_120A_47D2_BF2E_FD7FF8A89069__INCLUDED_)
#define AFX_AXISPMACDLG_H__16726E10_120A_47D2_BF2E_FD7FF8A89069__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxisPmacDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxisPmacDlg dialog

class CAxisPmacDlg : public CPropertyPage
{
// Construction
public:
	int	nUpdateLoopCounter;
	float m_fLastDisplayedDistance;
	int	m_nFlagModeControl;
	
	float m_fRelativeMove;
	CWnd* m_pParent;
	int m_nOldStatus;
	void ReadPmac();
	void MinMax(int *v0, int v1, int v2);
	void UpdateAllControls();
	void Defaults(int nWhich);
	CAxisPmacDlg(UINT nCap=0,CWnd* pParent=NULL,CWnd** DlgPtr=NULL,int nWhichAxis=0);
	~CAxisPmacDlg();

// Dialog Data
	//{{AFX_DATA(CAxisPmacDlg)
	enum { IDD = IDD_AXIS_PMAC_DIALOG };
	CComboBox	m_comboAmplifierType;
	CComboBox	m_comboMacroNode;
	CEdit	m_editTotalDistance;
	CEdit	m_editJoystickAcceleration;
	CEdit	m_editName;
	CButton	m_checkLimitSwitches;
	CButton	m_checkUseFiducial;
	CEdit	m_editMaxTravelJoy;
	CEdit	m_editMinTravelJoy;
	CSpinButtonCtrl	m_spinKinematicOffset;
	CEdit	m_editRelativeMove;
	CComboBox	m_comboPhysicalAxis;
	CEdit	m_editNumberDP;
	CEdit	m_editKinematicOffset;
	CEdit	m_editStepsRev;
	CComboBox	m_comboTogglePos;
	CEdit	m_editTankZeroPos;
	CButton	m_checkMustHome;
	CEdit	m_editStepSize;
	CButton	m_checkJoystickReverse;
	CButton	m_checkAxisAvailable;
	CComboBox	m_comboAxisUnits;
	CEdit	m_editAcceleration;
	CEdit	m_editHomeOffset;
	CEdit	m_editHomeSpeed;
	CEdit	m_editInPosErr;
	CEdit	m_editJoystickGain;
	CEdit	m_editJoystickNumber;
	CEdit	m_editMaxSpeed;
	CEdit	m_editMaxTravel;
	CEdit	m_editMinTravel;
	CEdit	m_editMemOffset;
	CString	m_stringEncoderPos;
	CString	m_stringStatus;
	int		m_nPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisPmacDlg)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd** m_DlgPtr;
	struct	AxisData	*m_pAxis;
	CSi6*	m_pSi6User;
	char	Caption[40];
	int		m_nCap;
	int		m_nWhichAxis;
	HBRUSH m_hBr;

	// Generated message map functions
	//{{AFX_MSG(CAxisPmacDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnAcceptButton();
	afx_msg void OnDefaultButton();
	afx_msg void OnButtonHomeOffset();
	afx_msg void OnButtonHmz();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboTogglePos();
	afx_msg void OnSelchangeComboPhysicalAxis();
	afx_msg void OnButtonRelMove();
	afx_msg void OnChangeEditRelMove();
	afx_msg void OnChangeEditMinTravel();
	afx_msg void OnChangeEditMaxTravel();
	afx_msg void OnButtonDisableLs();
	afx_msg void OnDeltaposSpinKinematicOffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditKinematicOffset();
	afx_msg void OnButtonKinematicOddset();
	afx_msg void OnCheckLimitSwitches();
	afx_msg void OnChangeEditName();
	afx_msg void OnSelchangeComboAmplifierType();
	afx_msg void OnChangeEditJoystickGain();
	afx_msg void OnChangeEditStepsrev();
	afx_msg void OnButtonRelNegMove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISPMACDLG_H__16726E10_120A_47D2_BF2E_FD7FF8A89069__INCLUDED_)
