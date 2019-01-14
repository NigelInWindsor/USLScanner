#if !defined(AFX_ROBOTPAGE_H__05A25386_A7FB_4C92_83A5_D87B425360B3__INCLUDED_)
#define AFX_ROBOTPAGE_H__05A25386_A7FB_4C92_83A5_D87B425360B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotPage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CRobotPage dialog

class CRobotPage : public CResizablePage
{
	DECLARE_DYNCREATE(CRobotPage)

// Construction
public:
	void SetAccessPrivelage();
	bool ExtractVector(CEdit *pEdit, D3DXVECTOR3 *pVec, D3DXVECTOR3 *pVecNorm = NULL,float *pfAngle = NULL);

	int m_nRobot;
	float m_fJointDegrees[6];
	int m_nCartesianCoordDisplay;
	D3DXVECTOR3 m_vOldPos;
	D3DXQUATERNION m_quatOldPos;
	
	HBRUSH m_hBr;

	void UpdateAllControls();
	void InvalidateProfileSheet();
	CRobotPage();
	~CRobotPage();

// Dialog Data
	//{{AFX_DATA(CRobotPage)
	enum { IDD = IDD_ROBOT_KINEMATICS_PAGE };
	CSpinButtonCtrl	m_spinBaseRotZ;
	CSpinButtonCtrl	m_spinBaseRotY;
	CSpinButtonCtrl	m_spinBaseRotX;
	CEdit	m_editBaseRotZ;
	CEdit	m_editBaseRotY;
	CEdit	m_editBaseRotX;
	CEdit	m_editNormR;
	CEdit	m_editNormK;
	CEdit	m_editNormJ;
	CEdit	m_editNormI;
	CButton	m_checkRobot1;
	CButton	m_checkRobot0;
	CEdit	m_editEndEffectorType;
	CEdit	m_editEndEffectorSerialNumber;
	CEdit	m_editSerialNumber;
	CEdit	m_editJ6toTT;
	CEdit	m_editStepSizeJ6;
	CEdit	m_editStepSizeJ5;
	CEdit	m_editStepSizeJ4;
	CEdit	m_editStepSizeJ3;
	CEdit	m_editStepSizeJ2;
	CEdit	m_editStepSizeJ1;
	CComboBox	m_comboVirtualActual;
	CEdit	m_editJ5J6;
	CEdit	m_editJ4J5;
	CEdit	m_editJ3J4;
	CEdit	m_editJ2J3;
	CEdit	m_editJ1J2;
	CEdit	m_editBaseJ1;
	CEdit	m_editBaseCoord;
	CEdit	m_editName;
	CSpinButtonCtrl	m_spinJ6;
	CSpinButtonCtrl	m_spinJ5;
	CSpinButtonCtrl	m_spinJ4;
	CSpinButtonCtrl	m_spinJ3;
	CSpinButtonCtrl	m_spinJ2;
	CSpinButtonCtrl	m_spinJ1;
	CComboBox	m_comboCartesianCoord;
	CEdit	m_editInverseJ6;
	CEdit	m_editInverseJ5;
	CEdit	m_editInverseJ4;
	CEdit	m_editInverseJ3;
	CEdit	m_editInverseJ2;
	CEdit	m_editInverseJ1;
	CEdit	m_editJ6;
	CEdit	m_editJ5;
	CEdit	m_editJ4;
	CEdit	m_editJ3;
	CEdit	m_editJ2;
	CEdit	m_editJ1;
	CEdit	m_editW;
	CEdit	m_editZ;
	CEdit	m_editY;
	CEdit	m_editK;
	CEdit	m_editJ;
	CEdit	m_editI;
	CEdit	m_editX;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobotPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRobotPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonZero();
	afx_msg void OnSelchangeComboCartesianCoord();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditName();
	afx_msg void OnChangeEditBaseCoord();
	afx_msg void OnChangeEditBaseJ1();
	afx_msg void OnChangeEditJ1J2();
	afx_msg void OnChangeEditJ2J3();
	afx_msg void OnChangeEditJ3J4();
	afx_msg void OnChangeEditJ4J5();
	afx_msg void OnChangeEditJ5J6();
	afx_msg void OnSelchangeComboVirtualActual();
	afx_msg void OnChangeEditStepsizeJ1();
	afx_msg void OnChangeEditStepsizeJ2();
	afx_msg void OnChangeEditStepsizeJ3();
	afx_msg void OnChangeEditStepsizeJ4();
	afx_msg void OnChangeEditStepsizeJ5();
	afx_msg void OnChangeEditStepsizeJ6();
	afx_msg void OnChangeEditJ6Tt();
	afx_msg void OnSelchangeComboRobotNumber();
	afx_msg void OnChangeEditSerialNumber();
	afx_msg void OnChangeEditEndEffectorType();
	afx_msg void OnChangeEditEndEffectorSerialnumber();
	afx_msg void OnButtonDownloadEndEffector();
	afx_msg void OnButtonDownloadAllKinematics();
	afx_msg void OnButtonBothRobots();
	afx_msg void OnCheckRobot0();
	afx_msg void OnCheckRobot1();
	afx_msg void OnChangeEditBaseRotX();
	afx_msg void OnChangeEditBaseRotY();
	afx_msg void OnChangeEditBaseRotZ();
	afx_msg void OnDeltaposSpinBaseRotX(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinBaseRotY(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinBaseRotZ(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonBase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTPAGE_H__05A25386_A7FB_4C92_83A5_D87B425360B3__INCLUDED_)
