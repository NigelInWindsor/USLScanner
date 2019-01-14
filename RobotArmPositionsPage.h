#if !defined(AFX_ROBOTARMPOSITIONSPAGE_H__2EC353B9_0F00_4F9A_A013_6C80A8D2668F__INCLUDED_)
#define AFX_ROBOTARMPOSITIONSPAGE_H__2EC353B9_0F00_4F9A_A013_6C80A8D2668F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotArmPositionsPage.h : header file
//
#include "EditSpinItem.h"


/////////////////////////////////////////////////////////////////////////////
// CRobotArmPositionsPage dialog

class CRobotArmPositionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRobotArmPositionsPage)

// Construction
public:
	void Untwist(int nSide);
	void SetToolBarCheckedState();
	void TowerGoto(int nSide);
	void ArmGoto(int nSide);
	void CreateColumns();
	void FillList();
	HRESULT ItemFinished(WPARAM, LPARAM);
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	CEditSpinItem* m_pEditSpinItem;

	int m_nIndex;
	int m_nWhichSet;

	HBRUSH		m_hBr;
	CToolBar	m_wndToolBar;
	CReBar		m_wndRebar;
	CPoint m_ptClient;
	void JointJog(int nDir);
	void SetButtonStates();
	int m_nJoystickMode;
	int	m_nSide;
	int	m_nJoint;
	bool m_bLbDown;
	int	m_nEditSide;
	void UpdateAllControls();
	
	CRobotArmPositionsPage();
	~CRobotArmPositionsPage();

// Dialog Data
	//{{AFX_DATA(CRobotArmPositionsPage)
	enum { IDD = IDD_ROBOT_ARM_POSITIONS_PAGE };
	CMFCListCtrl	m_listArm;
	CButton	m_buttonUnTwist;
	CComboBox	m_comboJoystickMode;
	CButton	m_checkRight;
	CButton	m_checkLeft;
	CButton	m_checkJoint6;
	CButton	m_checkJoint5;
	CButton	m_checkJoint4;
	CButton	m_checkJoint3;
	CButton	m_checkJoint2;
	CButton	m_checkJoint1;
	CSpinButtonCtrl	m_spinDirection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobotArmPositionsPage)
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
	//{{AFX_MSG(CRobotArmPositionsPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboJoystickMode();
	afx_msg void OnCheckLeft();
	afx_msg void OnCheckRight();
	afx_msg void OnCheckJoint1();
	afx_msg void OnCheckJoint2();
	afx_msg void OnCheckJoint3();
	afx_msg void OnCheckJoint4();
	afx_msg void OnCheckJoint5();
	afx_msg void OnCheckJoint6();
	afx_msg void OnArmsAdd();
	afx_msg void OnArmsDelete();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonUntwist();
	afx_msg void OnClickListArm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListArm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListArm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListArm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnArmsGotoleft();
	afx_msg void OnArmsGotoright();
	afx_msg void OnArmsModifyleft();
	afx_msg void OnArmsModifyright();
	afx_msg void OnArmsGotoboth();
	afx_msg void OnArmsMovejustlefttower();
	afx_msg void OnArmsMovejustrighttower();
	afx_msg void OnArmsMoveall();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonPowerMotors();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonUntwist0();
	afx_msg void OnButtonUntwist1();
	void FillArmEntry(int nIndex, int nSideMask);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_comboSet;
	afx_msg void OnCbnSelchangeComboSet();
	afx_msg HRESULT ItemChanged(WPARAM wp, LPARAM lp);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTARMPOSITIONSPAGE_H__2EC353B9_0F00_4F9A_A013_6C80A8D2668F__INCLUDED_)
