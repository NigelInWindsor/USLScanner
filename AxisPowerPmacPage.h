#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxisPowerPmacPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxisPowerPmacPage dialog

class CAxisPowerPmacPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAxisPowerPmacPage)

// Construction
public:
	HBRUSH	m_hBr;
	float	m_fRelativeMove;
	int		m_nUpdateLoopCounter;
	LPTSTR	getTabName();
	void	ReadPOWERPmac();
	void	UpdateAllControls();
	int		m_nAxis;
	BOOL	OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	CAxisPowerPmacPage(int nAxis = 0);
	~CAxisPowerPmacPage();

// Dialog Data
	//{{AFX_DATA(CAxisPowerPmacPage)
	enum { IDD = IDD_AXIS_POWER_PMAC_PAGE };
	CEdit	m_editRelativeMove;
	CSpinButtonCtrl	m_spinKinematicOffset;
	CEdit	m_editKinematicOffset;
	CEdit	m_editHomePos;
	CEdit	m_editInPosError;
	CEdit	m_editDisplayPrecision;
	CEdit	m_editName;
	CEdit	m_editMaxSpeed;
	CEdit	m_editJoystickNumber;
	CEdit	m_editJogGain;
	CEdit	m_editJogAcceleration;
	CEdit	m_editAcceleration;
	CComboBox	m_comboTogglePos;
	CComboBox	m_comboPhysicalAxis;
	CButton	m_checkJoystickReverse;
	CComboBox	m_comboUnits;
	CEdit	m_editStepSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAxisPowerPmacPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAxisPowerPmacPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnAcceptButton();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonHomePos();
	afx_msg void OnChangeEditKinematicOffset();
	afx_msg void OnDeltaposSpinKinematicOffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonKinematicOddset();
	afx_msg void OnChangeEditRelMove();
	afx_msg void OnButtonRelNegMove();
	afx_msg void OnButtonRelMove();
	afx_msg void OnButtonAbsMove();
	afx_msg void OnSelchangeComboPhysicalAxis();
	afx_msg void OnChangeEditName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_editJogAccelerationTs;
	CComboBox m_comboOriginMode;
	afx_msg void OnCbnSelchangeComboOriginMode();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


