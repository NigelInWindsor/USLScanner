#if !defined(AFX_KINEMATICDLG_H__397ED678_ACDB_43E8_A0F2_A5C531A70F41__INCLUDED_)
#define AFX_KINEMATICDLG_H__397ED678_ACDB_43E8_A0F2_A5C531A70F41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KinematicDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKinematicDlg dialog

class CKinematicDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CKinematicDlg)

// Construction
public:
	HBRUSH m_hBr;
	bool RetrieveManipulator(int nSide);
	bool SaveManipulator(int nSide);
	
	int	m_nLeft;
	int	m_nRight;
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	void SetAccessPrivelage();
	void UpdateAllEditControls();
	CKinematicDlg();   // standard constructor
	~CKinematicDlg();

// Dialog Data
	//{{AFX_DATA(CKinematicDlg)
	enum { IDD = IDD_KINEMATICS_DIALOG };
	CEdit	m_editInternalWpRight;
	CEdit	m_editInternalWpLeft;
	CEdit	m_editMirrorNozzleRight;
	CEdit	m_editMirrorNozzleLeft;
	CButton	m_checkYTLswRight;
	CButton	m_checkYTLswLeft;
	CButton	m_checkYTKinematicRight;
	CButton	m_checkYTKinematicLeft;
	CButton	m_checkXTLswRight;
	CButton	m_checkXTLswLeft;
	CButton	m_checkXTKinematicRight;
	CButton	m_checkXTKinematicLeft;
	CButton	m_staticRight;
	CButton	m_staticLeft;
	CButton	m_checkInvertXTip;
	CButton	m_checkInvertYTip;
	CComboBox	m_comboYTipRotateAxis;
	CComboBox	m_comboXTipRotateAxis;
	CEdit	m_editRightInitWp;
	CEdit	m_editLeftInitWp;
	CButton	m_checkEnableLeft;
	CButton	m_checkEnableRight;
	CComboBox	m_comboRightModel;
	CComboBox	m_comboLeftModel;
	CComboBox	m_comboRightTip;
	CComboBox	m_comboLeftTip;
	CEdit	m_editComponentThickness;
	CEdit	m_editRightArmLength;
	CEdit	m_editLeftArmLength;
	CComboBox	m_comboWaterPath;
	CEdit	m_editRightZOffset;
	CEdit	m_editLeftZOffset;
	CEdit	m_editRightYOffset;
	CEdit	m_editLeftYOffset;
	CEdit	m_editRightXOffset;
	CEdit	m_editLeftXOffset;
	CEdit	m_editRightWaterPath;
	CEdit	m_editRightProbeLength;
	CEdit	m_editLeftWaterPath;
	CEdit	m_editLeftProbeLength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKinematicDlg)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKinematicDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnButtonEnableRight();
	afx_msg void OnButtonEnableLeft();
	afx_msg void OnChangeEditLeftProbeLength();
	afx_msg void OnChangeEditLeftWaterPath();
	afx_msg void OnChangeEditRightProbeLength();
	afx_msg void OnChangeEditRightWaterPath();
	afx_msg void OnChangeEditXLeftOffset();
	afx_msg void OnChangeEditXRightOffset();
	afx_msg void OnChangeEditYLeftOffset();
	afx_msg void OnChangeEditYRightOffset();
	afx_msg void OnChangeEditZLeftOffset();
	afx_msg void OnChangeEditZRightOffset();
	afx_msg void OnSelchangeComboWaterPath();
	afx_msg void OnButtonZeroPos();
	afx_msg void OnButtonLeftTip();
	afx_msg void OnButtonZeroRightTip();
	afx_msg void OnChangeEditRightArmLength();
	afx_msg void OnChangeEditLeftArmLength();
	afx_msg void OnChangeEditComponentThickness();
	afx_msg void OnButtonFindLeft();
	afx_msg void OnButtonFindRight();
	afx_msg void OnSelchangeComboLeftTip();
	afx_msg void OnSelchangeComboRightTip();
	afx_msg void OnSelchangeComboLeftModel();
	afx_msg void OnSelchangeComboRightModel();
	afx_msg void OnChangeEditRightInitWp();
	afx_msg void OnChangeEditLeftInitWp();
	afx_msg void OnSelchangeComboXtipRotateAxis();
	afx_msg void OnSelchangeComboYtipRotateAxis();
	afx_msg void OnCheckInvertXtip();
	afx_msg void OnCheckInvertYtip();
	afx_msg void OnCheckXtKinematicLeft();
	afx_msg void OnCheckXtKinematicRight();
	afx_msg void OnCheckXtLimitswitchLeft();
	afx_msg void OnCheckXtLimitswitchRight();
	afx_msg void OnCheckYtKinematicLeft();
	afx_msg void OnCheckYtKinematicRight();
	afx_msg void OnCheckYtLimitswitchLeft();
	afx_msg void OnCheckYtLimitswitchRight();
	afx_msg void OnButtonSaveLeft();
	afx_msg void OnButtonSaveRight();
	afx_msg void OnButtonOpenLeft();
	afx_msg void OnButtonOpenRight();
	afx_msg void OnChangeEditMirrorNozzleRight();
	afx_msg void OnChangeEditMirrorNozzleLeft();
	afx_msg void OnChangeEditIntWpRight();
	afx_msg void OnChangeEditIntWpLeft();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KINEMATICDLG_H__397ED678_ACDB_43E8_A0F2_A5C531A70F41__INCLUDED_)
