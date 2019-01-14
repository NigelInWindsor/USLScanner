#if !defined(AFX_PMACDPRAMTESTPAGE_H__685CD679_8B52_4658_B518_65E7108390B5__INCLUDED_)
#define AFX_PMACDPRAMTESTPAGE_H__685CD679_8B52_4658_B518_65E7108390B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacDPRAMTestPage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CPmacDPRAMTestPage dialog

class CPmacDPRAMTestPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPmacDPRAMTestPage)

// Construction
public:
	int m_nReadArray[1024];
	int m_nWriteArray[1024];
	int m_nFeederValue;
	int	m_nErrorCount;
	bool m_bTest;
	bool m_bTimerActive;
	CPmacDPRAMTestPage();
	~CPmacDPRAMTestPage();

// Dialog Data
	//{{AFX_DATA(CPmacDPRAMTestPage)
	enum { IDD = IDD_PMAC_DPRAM_TEST_PAGE };
	CEdit	m_editWritten;
	CEdit	m_editRead;
	CEdit	m_editErrors;
	CButton	m_checkTest;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPmacDPRAMTestPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPmacDPRAMTestPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckTest();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACDPRAMTESTPAGE_H__685CD679_8B52_4658_B518_65E7108390B5__INCLUDED_)
