#if !defined(AFX_REMOTEINTERFACEPAGE_H__705A903B_F415_4384_B4E0_7ACAB944D8EA__INCLUDED_)
#define AFX_REMOTEINTERFACEPAGE_H__705A903B_F415_4384_B4E0_7ACAB944D8EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteInterfacePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRemoteInterfacePage dialog

class CRemoteInterfacePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRemoteInterfacePage)

// Construction
public:
	void SetAccessPrivelages();
	bool m_bBluetoothConnected;
	int m_nIndex;
	void FillList();
	void CreateColumns();
	bool m_bEthernetConnected;
	
	void UpdateAllControls();
	CRemoteInterfacePage();
	~CRemoteInterfacePage();

// Dialog Data
	//{{AFX_DATA(CRemoteInterfacePage)
	enum { IDD = IDD_REMOTE_INTERFACE_PAGE };
	CComboBox	m_comboInterface;
	CComboBox	m_comboBluetoothDevice;
	CComboBox	m_comboBTClientServer;
	CEdit	m_editBTStatus;
	CComboBox	m_comboClientServer;
	CListCtrl	m_listPackets;
	CEdit	m_editTx;
	CIPAddressCtrl	m_IPAddress;
	CEdit	m_editPortNumber;
	CEdit	m_editStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRemoteInterfacePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRemoteInterfacePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonSend();
	afx_msg void OnGetdispinfoListPackets(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboClientServer();
	afx_msg void OnFieldchangedIpaddressRemote(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboBluetoothClientServer();
	afx_msg void OnSelchangeComboBtDevices();
	afx_msg void OnSelchangeComboInterface();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEINTERFACEPAGE_H__705A903B_F415_4384_B4E0_7ACAB944D8EA__INCLUDED_)
