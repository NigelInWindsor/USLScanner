#if !defined(AFX_DEVICERESOUCESDLG_H__4B3B684D_4C24_4188_99DE_564E23E0FAB8__INCLUDED_)
#define AFX_DEVICERESOUCESDLG_H__4B3B684D_4C24_4188_99DE_564E23E0FAB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceResoucesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeviceResoucesDlg dialog

class CDeviceResoucesDlg : public CDialog
{
// Construction
public:
	void SetAccessPrivelage();
	void CopyControlsToVariables();
	CDeviceResoucesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeviceResoucesDlg)
	enum { IDD = IDD_DEVICE_RESOURCES_DIALOG };
	CComboBox	m_comboSi6Present;
	CComboBox	m_comboPmacPresent;
	CButton	m_buttonLookUp;
	CButton	m_buttonBrowse;
	CButton	m_buttonInstall;
	CEdit	m_editLastSettings;
	CButton	m_buttonStop;
	CButton	m_buttonStart;
	CComboBox	m_comboHardware;
	CEdit	m_editDriverName;
	CEdit	m_editPmacMemRange;
	CEdit	m_editPmacMemAddrs;
	CEdit	m_editPmacIRQ;
	CEdit	m_editPmacIORange;
	CEdit	m_editPmacIOAddrs;
	CEdit	m_editUtMemRange;
	CEdit	m_editUtMemAddrs;
	CEdit	m_editUtIRQ;
	CEdit	m_editUtIORange;
	CEdit	m_editUtIOAddrs;
	CEdit	m_editSi6MemRange;
	CEdit	m_editSi6MemAddrs;
	CEdit	m_editSi6IRQ;
	CEdit	m_editSi6IORange;
	CEdit	m_editSi6IOAddrs;
	CString	m_stringPmacVersion;
	CString	m_stringSi6Version;
	CString	m_stringUltrasonicsVersion;
	CString	m_Status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceResoucesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeviceResoucesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLookup();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnSelchangeComboHardware();
	virtual void OnOK();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonInstall();
	afx_msg void OnSelchangeComboPmacPresent();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICERESOUCESDLG_H__4B3B684D_4C24_4188_99DE_564E23E0FAB8__INCLUDED_)
