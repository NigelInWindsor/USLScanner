#if !defined(AFX_ROBOTCALIBRATIONPAGE_H__69E8FC9C_934A_41AE_8AFD_2EEE96D5A89D__INCLUDED_)
#define AFX_ROBOTCALIBRATIONPAGE_H__69E8FC9C_934A_41AE_8AFD_2EEE96D5A89D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotCalibrationPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRobotCalibrationPage dialog

class CRobotCalibrationPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRobotCalibrationPage)

// Construction
public:
	void RenderTrackerPts();
	bool ExtractVector(CEdit *pEdit, D3DXVECTOR3 *pVec, D3DXVECTOR3 *pVecNorm = NULL,float *pfAngle = NULL);


	D3DXVECTOR3 m_vTrackerPt[2];
	D3DXVECTOR3 m_vRobotPt[2];
	D3DXVECTOR3 m_vRobotNorm[2];
	int m_nRobot;
	
	void UpdateAllControls();
	CRobotCalibrationPage();
	~CRobotCalibrationPage();

// Dialog Data
	//{{AFX_DATA(CRobotCalibrationPage)
	enum { IDD = IDD_ROBOT_CALIBRATION_PAGE };
	CEdit	m_editTrackerDifference;
	CEdit	m_editRobotDifference;
	CButton	m_buttonTracker1;
	CButton	m_buttonTracker0;
	CEdit	m_editPt1;
	CEdit	m_editPt0;
	CEdit	m_editJ6toTT;
	CEdit	m_editJ5J6;
	CEdit	m_editJ4J5;
	CEdit	m_editJ3J4;
	CEdit	m_editJ2J3;
	CEdit	m_editJ1J2;
	CEdit	m_editBaseJ1;
	CEdit	m_editBaseCoord;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobotCalibrationPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRobotCalibrationPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditJ1J2();
	afx_msg void OnChangeEditJ2J3();
	afx_msg void OnChangeEditJ3J4();
	afx_msg void OnChangeEditJ4J5();
	afx_msg void OnChangeEditJ5J6();
	afx_msg void OnChangeEditJ6Tt();
	afx_msg void OnChangeEditBaseJ1();
	afx_msg void OnChangeEditBaseCoord();
	afx_msg void OnChangeEditPt0();
	afx_msg void OnChangeEditPt1();
	afx_msg void OnButtonTracker0();
	afx_msg void OnButtonTracker1();
	afx_msg void OnButtonGoto0();
	afx_msg void OnButtonGoto1();
	afx_msg void OnButtonDownloadAllKinematics();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTCALIBRATIONPAGE_H__69E8FC9C_934A_41AE_8AFD_2EEE96D5A89D__INCLUDED_)
