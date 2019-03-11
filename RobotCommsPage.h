#if !defined(AFX_ROBOTCOMMS_H__69D3EB83_3E6F_4306_B0A5_BE6F645CD9F6__INCLUDED_)
#define AFX_ROBOTCOMMS_H__69D3EB83_3E6F_4306_B0A5_BE6F645CD9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotComms.h : header file
//

#include "DoubleScrollBar.h"
#include "EditSpinItem.h"
#include "USLMFCListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CRobotCommsPage dialog

class CRobotCommsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRobotCommsPage)

// Construction
public:
	HBRUSH m_hBr;
	void CreateColumns();
	void FillList();
	CStringArray m_TxArray;
	int	m_nEcatDCClockDiff;
	int	m_nSysServoCount;
	int	m_nRowsL;
	int m_nTopRow;
	int m_nDisplayRows;
	int m_nTxIndex;
	int	m_nEthercatStatus;
	int m_nScanLine;
	int	m_nNumberLines;
	int	m_nIndex;
	CString m_editBuff;
	CEditSpinItem* m_pEditSpinItem;
	bool m_bCtrlButton;
	
	void UpdateAllControls();
	CRobotCommsPage();
	~CRobotCommsPage();

// Dialog Data
	//{{AFX_DATA(CRobotCommsPage)
	enum { IDD = IDD_ROBOT_COMMS_PAGE };
	CStatic	m_staticWatch;
	CEdit	m_editEthercatStatus;
	CButton	m_checkEchoData;
	CSpinButtonCtrl	m_spinDCPlus;
	CSpinButtonCtrl	m_spinDCMinus;
	CSpinButtonCtrl	m_spinDCBand;
	CEdit	m_editDCPlus;
	CEdit	m_editDCMinus;
	CEdit	m_editDCBand;
	CEdit	m_editResponce;
	CEdit	m_editInLineCommand;
	CButton	m_buttonConnect;
	CIPAddressCtrl	m_ipAddress;
	CEdit	m_editUser;
	CEdit	m_editPort;
	CEdit	m_editPassword;
	CComboBox	m_comboProtocol;
	CButton	m_checkSelectAtStartUp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobotCommsPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRobotCommsPage)
	afx_msg void OnFieldchangedIpaddressRobot(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnCheckSelectAtStartup();
	afx_msg void OnChangeEditUser();
	afx_msg void OnChangeEditPort();
	afx_msg void OnChangeEditPassword();
	afx_msg void OnSelchangeComboProtocol();
	afx_msg void OnButtonConnect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonEnable();
	afx_msg void OnButtonKill();
	afx_msg void OnRadioRobot0();
	afx_msg void OnRadioRobot1();
	afx_msg void OnRadioRobotBoth();
	afx_msg void OnButtonClear();
	afx_msg void OnCommsClearall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CUSLMFCListCtrl m_listMessages;
	afx_msg void OnLvnGetdispinfoListMessages(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListMessages(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCommsCopy();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTCOMMS_H__69D3EB83_3E6F_4306_B0A5_BE6F645CD9F6__INCLUDED_)
