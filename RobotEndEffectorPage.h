#if !defined(AFX_ROBOTENDEFFECTORPAGE_H__9C58E8DC_2153_4EBB_A448_675856D92E29__INCLUDED_)
#define AFX_ROBOTENDEFFECTORPAGE_H__9C58E8DC_2153_4EBB_A448_675856D92E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotEndEffectorPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRobotEndEffectorPage dialog

class CRobotEndEffectorPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRobotEndEffectorPage)

// Construction
public:
	void CalculateKinematics();
	WCHAR m_wFileDescription[30];
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndFileToolBar;
	CReBar     m_wndRebar;
	HBRUSH m_hBr;
	int m_nRobot;
	
	void UpdateAllControls();
	void UpdateToolText();
	CRobotEndEffectorPage();
	~CRobotEndEffectorPage();

// Dialog Data
	//{{AFX_DATA(CRobotEndEffectorPage)
	enum { IDD = IDD_ROBOT_ENDEFFECTOR_PAGE };
	CEdit	m_editProbeVector;
	CEdit	m_editArmVector;
	CComboBox	m_comboEndEffectorType;
	CButton	m_checkRobot1;
	CButton	m_checkRobot0;
	CEdit	m_editJ6toTT;
	CSpinButtonCtrl	m_spinXRotation;
	CSpinButtonCtrl	m_spinYRotation;
	CSpinButtonCtrl	m_spinZRotation;
	CEdit	m_editXRotation;
	CEdit	m_editYRotation;
	CEdit	m_editZRotation;
	CEdit	m_editFilePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobotEndEffectorPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRobotEndEffectorPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnDeltaposSpinYRotation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditYRotation();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileSaveas();
	afx_msg void OnFileOpen();
	afx_msg void OnChangeEditProbeCoordinate();
	afx_msg void OnCheckRobot0();
	afx_msg void OnCheckRobot1();
	afx_msg void OnSelchangeComboEndEffectorType();
	afx_msg void OnDeltaposSpinArmLength(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinProbeLength(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditArmVector();
	afx_msg void OnChangeEditProbeVector();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTENDEFFECTORPAGE_H__9C58E8DC_2153_4EBB_A448_675856D92E29__INCLUDED_)
