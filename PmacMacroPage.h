#if !defined(AFX_PMACMACROPAGE_H__5F2CD820_ACD3_4B39_999D_BA8F20682F6C__INCLUDED_)
#define AFX_PMACMACROPAGE_H__5F2CD820_ACD3_4B39_999D_BA8F20682F6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacMacroPage.h : header file
#include "ResizablePage.h"
//

/////////////////////////////////////////////////////////////////////////////
// CPmacMacroPage dialog

class CPmacMacroPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPmacMacroPage)

// Construction
public:
	void ConvertToDecimal(CString &Buff);
	int m_nNode;
	void SendStr(CString Buff);
	CStringArray m_Tx;
	CStringArray m_Rx;

	bool m_bMACROASCII;
	int	m_nLastTxPtr;
	int m_nStation;
	void UpdateAllControls();
	
	bool m_bCtrlButton;
	bool m_bTimerActive;
	CPmacMacroPage();
	~CPmacMacroPage();

// Dialog Data
	//{{AFX_DATA(CPmacMacroPage)
	enum { IDD = IDD_PMAC_MACRO_PAGE };
	CEdit	m_editMI990;
	CComboBox	m_comboNode;
	CEdit	m_editSent;
	CEdit	m_editMI998;
	CEdit	m_editMI997;
	CEdit	m_editMI996;
	CEdit	m_editMI995;
	CEdit	m_editMI992;
	CEdit	m_editMI10;
	CEdit	m_editMI9;
	CEdit	m_editMI8;
	CButton	m_buttonMACROASCII;
	CComboBox	m_comboStation;
	CEdit	m_edit73;
	CEdit	m_edit72;
	CEdit	m_edit6891;
	CEdit	m_edit6890;
	CEdit	m_edit6857;
	CEdit	m_edit6856;
	CEdit	m_edit6855;
	CEdit	m_edit6854;
	CEdit	m_edit6853;
	CEdit	m_edit6852;
	CEdit	m_edit6851;
	CEdit	m_edit6850;
	CEdit	m_edit82;
	CEdit	m_edit81;
	CEdit	m_edit80;
	CEdit	m_edit79;
	CEdit	m_edit78;
	CEdit	m_edit71;
	CEdit	m_edit70;
	CEdit	m_edit6841;
	CEdit	m_edit6840;
	CEdit	m_edit6807;
	CEdit	m_edit6806;
	CEdit	m_edit6805;
	CEdit	m_edit6804;
	CEdit	m_edit6803;
	CEdit	m_edit6802;
	CEdit	m_edit6801;
	CEdit	m_edit6800;
	CEdit	m_editResponce;
	CEdit	m_editCommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPmacMacroPage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPmacMacroPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRefresh();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboStation();
	afx_msg void OnButtonMacroAscii();
	afx_msg void OnButtonAllMi();
	afx_msg void OnSelchangeComboNode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACMACROPAGE_H__5F2CD820_ACD3_4B39_999D_BA8F20682F6C__INCLUDED_)
