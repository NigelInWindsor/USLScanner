#if !defined(AFX_ROBOTJOGPAGE_H__E338C5ED_CF1C_4943_B152_E616DB7C3D5F__INCLUDED_)
#define AFX_ROBOTJOGPAGE_H__E338C5ED_CF1C_4943_B152_E616DB7C3D5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotJogPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRobotJogPage dialog

class CRobotJogPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRobotJogPage)

// Construction
public:
	float m_fReach;
	void JoystickEnableDisable();
	void FrameJog(bool bLButtonDn, CPoint pt);
	void JointJog(bool bLButtonDn, CPoint pt);
	void SetToolBarCheckedState();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	HBRUSH m_hBr;
	bool m_bLBDown;
	int m_nRobot;
	void UpdateAllControls();
	
	CRobotJogPage();
	~CRobotJogPage();

// Dialog Data
	//{{AFX_DATA(CRobotJogPage)
	enum { IDD = IDD_ROBOT_JOG_PAGE };
	CEdit	m_editOrthogonalSpeed;
	CSpinButtonCtrl	m_spinOrthogonalSpeed;
	CSpinButtonCtrl	m_spinScanSpeed;
	CSpinButtonCtrl	m_spinMoveToSpeed;
	CSpinButtonCtrl	m_spinJogSpeed;
	CEdit	m_editScanSpeed;
	CEdit	m_editMoveToSpeed;
	CEdit	m_editJogSpeed;
	CEdit	m_editZRight;
	CEdit	m_editZLeft;
	CEdit	m_editYRight;
	CEdit	m_editXRight;
	CEdit	m_editVRight;
	CEdit	m_editTLeft;
	CEdit	m_editTRight;
	CEdit	m_editVLeft;
	CEdit	m_editHRight;
	CEdit	m_editHLeft;
	CEdit	m_editYLeft;
	CEdit	m_editXLeft;
	CButton	m_checkReverseX;
	CButton	m_checkMakeVisible;
	CEdit	m_editScanSize;
	CSpinButtonCtrl	m_spinReach;
	CEdit	m_editReach;
	CSliderCtrl	m_sliderJ6;
	CSliderCtrl	m_sliderJ5;
	CSliderCtrl	m_sliderJ4;
	CSliderCtrl	m_sliderJ3;
	CSliderCtrl	m_sliderJ2;
	CSliderCtrl	m_sliderJ1;
	CButton	m_buttonPos5;
	CButton	m_buttonPos4;
	CButton	m_buttonPos3;
	CButton	m_buttonPos2;
	CButton	m_buttonPos1;
	CButton	m_buttonPos0;
	CButton	m_butttonNeg5;
	CButton	m_buttonNeg4;
	CButton	m_buttonNeg3;
	CButton	m_buttonNeg2;
	CButton	m_buttonNeg1;
	CButton	m_buttonNeg0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobotJogPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRobotJogPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonRobot0();
	afx_msg void OnButtonRobot1();
	afx_msg void OnButtonJoint();
	afx_msg void OnButtonTool();
	afx_msg void OnButtonFrame();
	afx_msg void OnButtonJoystick();
	afx_msg void OnDeltaposSpinReach(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditReach();
	afx_msg void OnCheckDisplay();
	afx_msg void OnButtonPark();
	afx_msg void OnCheckReverseX();
	afx_msg void OnChangeEditXLeft();
	afx_msg void OnChangeEditYLeft();
	afx_msg void OnChangeEditZLeft();
	afx_msg void OnChangeEditHLeft();
	afx_msg void OnChangeEditVLeft();
	afx_msg void OnChangeEditTLeft();
	afx_msg void OnChangeEditXRightJog();
	afx_msg void OnChangeEditYRightJog();
	afx_msg void OnChangeEditZRightJog();
	afx_msg void OnChangeEditHRightJog();
	afx_msg void OnChangeEditVRightJog();
	afx_msg void OnChangeEditTRightJog();
	afx_msg void OnButtonMovetoZero();
	afx_msg void OnChangeEditJogSpeed();
	afx_msg void OnDeltaposSpinJogSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditMovetoSpeed();
	afx_msg void OnDeltaposSpinMovetoSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditScanSpeed();
	afx_msg void OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditOrthogonalSpeed();
	afx_msg void OnDeltaposSpinOrthogonalSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonNeg5();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTJOGPAGE_H__E338C5ED_CF1C_4943_B152_E616DB7C3D5F__INCLUDED_)
