#if !defined(AFX_ETHERCATPAGE_H__BB280E7A_E4AD_4F82_8DBE_F3EE3F951DB2__INCLUDED_)
#define AFX_ETHERCATPAGE_H__BB280E7A_E4AD_4F82_8DBE_F3EE3F951DB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EthercatPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEthercatPage dialog

class CEthercatPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CEthercatPage)

// Construction
public:
	void UpdateAllControls();
	
	CEthercatPage();
	~CEthercatPage();

// Dialog Data
	//{{AFX_DATA(CEthercatPage)
	enum { IDD = IDD_ETHERCAT_PAGE };
	CEdit	m_editMessage;
	CComboBox	m_comboMode;
	CEdit	m_editPositionDifference;
	CEdit	m_editTimeDiff;
	CEdit	m_editTime1;
	CEdit	m_editTime0;
	CEdit	m_editPosition1;
	CEdit	m_editPosition0;
	CEdit	m_editEthercatCount;
	CEdit	m_editRx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEthercatPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEthercatPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETHERCATPAGE_H__BB280E7A_E4AD_4F82_8DBE_F3EE3F951DB2__INCLUDED_)
