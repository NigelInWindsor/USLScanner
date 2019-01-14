#if !defined(AFX_ROBOTHANDSHAKINGPAGE_H__EAC517E5_684C_47D0_B817_472104FCEF00__INCLUDED_)
#define AFX_ROBOTHANDSHAKINGPAGE_H__EAC517E5_684C_47D0_B817_472104FCEF00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotHandShakingPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRobotHandShakingPage dialog

class CRobotHandShakingPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRobotHandShakingPage)

// Construction
public:
	int m_nTableMode;
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	
	CRobotHandShakingPage();
	~CRobotHandShakingPage();

// Dialog Data
	//{{AFX_DATA(CRobotHandShakingPage)
	enum { IDD = IDD_ROBOT_HANDSHAKING_PAGE };
	CEdit	m_editCoordSystem3;
	CEdit	m_editScanLine;
	CListCtrl	m_listGeneral;
	CEdit	m_editUTCollectionPtr;
	CEdit	m_editInputRegister;
	CEdit	m_editOutputToPort;
	CEdit	m_editInSyncCount;
	CComboBox	m_comboOutSynchPulseBit;
	CComboBox	m_comboOutCollectDataBit;
	CComboBox	m_comboInSynchPulseBit;
	CComboBox	m_comboInCollectDataBit;
	CComboBox	m_comboInputPort;
	CComboBox	m_comboOutputPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobotHandShakingPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRobotHandShakingPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboInCollectDataBit();
	afx_msg void OnSelchangeComboInSynchPulse();
	afx_msg void OnSelchangeComboInputPort();
	afx_msg void OnSelchangeComboOutCollectDataBit();
	afx_msg void OnSelchangeComboOutSynchPulse();
	afx_msg void OnSelchangeComboOutputPort();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditOutputToPort();
	afx_msg void OnGetdispinfoListGeneral(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTHANDSHAKINGPAGE_H__EAC517E5_684C_47D0_B817_472104FCEF00__INCLUDED_)
