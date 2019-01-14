#if !defined(AFX_REMOTEPAGE_H__52A5CC88_EF74_44F1_8E89_6D9544F24128__INCLUDED_)
#define AFX_REMOTEPAGE_H__52A5CC88_EF74_44F1_8E89_6D9544F24128__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemotePage.h : header file
//
#include <list>
#include "Remote.h"
#include "RMessageDefs.h"

/////////////////////////////////////////////////////////////////////////////
// CRemotePage dialog

class CRemotePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRemotePage)

// Construction
public:
	CRemotePage();
	~CRemotePage();

// Dialog Data
	//{{AFX_DATA(CRemotePage)
	enum { IDD = IDD_REMOTE_PAGE };
	CButton	m_startServerOnStartup;
	CEdit	m_editComputerName;
	CComboBox	m_comboAddress;
	CSpinButtonCtrl	m_spinPort;
	CEdit	m_editPort;
	CEdit	m_editStatus;
	CComboBox	m_comboBoxConType;
	CListBox	m_listBoxConType;
	CButton	m_buttonStopServer;
	CButton	m_buttonStartServer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRemotePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void SetNSent(int val);
	void SetConType();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRemotePage)
	afx_msg void OnButtonStartServer();
	afx_msg void OnButtonStopServer();
	BOOL StopServerUpdateControls();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditPort();
	afx_msg void OnSelchangeListContype();
	afx_msg void OnSelchangeComboConType();
	afx_msg void OnDeltaposSpinPort(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckStartserver();
	//}}AFX_MSG
	afx_msg LRESULT OnRMessageRemoteUpdate(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnRemoteUpdateConnectionStatus(WPARAM wparam, LPARAM lparam);
private:
	int m_iPort;
	CON_TYPE m_conType;
	void SetAddress();
	void SetAddress(const wchar_t * sAddress);
	void SetPort();
	DECLARE_MESSAGE_MAP()
    APP App;
	void UpdateConnectionStatus();
	void RestoreFromTank();
	void UpdateControls();
	void UpdateControls(BOOL bConnected);
	LRESULT OnRemotePageUpdateControls(WPARAM Wp, LPARAM Lp);
	bool m_bStartServerOnStartup;
	void SaveToTank();
	void StartServerUpdateControls();
	void StartServer();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEPAGE_H__52A5CC88_EF74_44F1_8E89_6D9544F24128__INCLUDED_)
