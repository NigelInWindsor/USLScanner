#if !defined(AFX_PMACFAULTSPAGE_H__5FEDA31D_96FF_4440_83F3_6A439CA55996__INCLUDED_)
#define AFX_PMACFAULTSPAGE_H__5FEDA31D_96FF_4440_83F3_6A439CA55996__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacFaultsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPmacFaultsPage dialog

class CPmacFaultsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPmacFaultsPage)

// Construction
public:
	void SetABBOutput(bool bFlag);
	CStringArray m_ErrorCodes;
	
	int m_nRepairAxis;
	void ConvertToDecimal(CString &Buff);
	CPmacFaultsPage();
	~CPmacFaultsPage();

// Dialog Data
	//{{AFX_DATA(CPmacFaultsPage)
	enum { IDD = IDD_PMAC_FAULTS_PAGE };
	CComboBox	m_comboRepairAxis;
	CEdit	m_editFaults;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPmacFaultsPage)
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
	//{{AFX_MSG(CPmacFaultsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonReset();
	afx_msg void OnButtonPmacReset();
	afx_msg void OnButtonAttempt();
	afx_msg void OnSelchangeComboRepairAxis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACFAULTSPAGE_H__5FEDA31D_96FF_4440_83F3_6A439CA55996__INCLUDED_)
