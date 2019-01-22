#if !defined(AFX_ETHERNETPAGE_H__771F7451_65E2_46F5_BA62_B9611F333B73__INCLUDED_)
#define AFX_ETHERNETPAGE_H__771F7451_65E2_46F5_BA62_B9611F333B73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EthernetPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEthernetPage dialog

class CEthernetPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CEthernetPage)

// Construction
public:
	bool m_bClientServer;
	int	m_nWhich;
	int	m_nIPAddrs;
	int	m_nPort;
	CEthernet m_Ethernet;
	CEthernet* m_pEthernet;
	CSocketAddr* m_pSocketAddrs = NULL;
	ADDRINFOT* m_pAddrInfo = NULL;

	
	void UpdateAllControls();
	CEthernetPage();
	~CEthernetPage();

// Dialog Data
	//{{AFX_DATA(CEthernetPage)
	enum { IDD = IDD_ETHERNET_PAGE };
	CEdit	m_editSend;
	CEdit	m_editPort;
	CComboBox	m_comboWhichAttachment;
	CComboBox	m_comboClientServer;
	CButton	m_buttonConnect;
	CIPAddressCtrl	m_ipAddress;
	CEdit	m_editRxStr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEthernetPage)
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
	//{{AFX_MSG(CEthernetPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonConnect();
	afx_msg void OnSelchangeComboServerClient();
	afx_msg void OnSelchangeComboWhichAttachment();
	afx_msg void OnButtonSend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_listAddrs;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETHERNETPAGE_H__771F7451_65E2_46F5_BA62_B9611F333B73__INCLUDED_)
