#if !defined(AFX_PMACDLG_H__C71C1340_DCB8_448B_9EF9_91C020417543__INCLUDED_)
#define AFX_PMACDLG_H__C71C1340_DCB8_448B_9EF9_91C020417543__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacGeneralPage.h : header file
//

#include "ResizablePage.h"
#include "StaticPosManage.h"
#include "PromptDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CPmacGeneralPage dialog

class CPmacGeneralPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPmacGeneralPage)

	// Construction
public:
	HBRUSH m_hBr;
	int	m_nOldIrqCount;
	clock_t c_f;
	clock_t c_i;
	CPromptDlg* m_pPromptDlg;
	CStaticPosManage m_StaticPosResponce;
	int m_nOldErrorCount;
	bool m_bTimerActive;
	bool m_bCtrlButton;
	void UpdateAllControls();
	
	CPmacGeneralPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPmacGeneralPage)
	enum { IDD = IDD_PMAC_DIALOG };
	CEdit	m_editServoUpdateRate;
	CEdit	m_editServoRate;
	CEdit	m_editErrorCount;
	CButton	m_checkHardwareIRQ;
	CEdit	m_editResponce;
	CEdit	m_editInLineCommand;
	CComboBox	m_comboEnableDevice;
	int		m_nIrqCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPmacGeneralPage)
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
	//{{AFX_MSG(CPmacGeneralPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboEnableDevice();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnButtonReset();
	afx_msg void OnCheckHardwareirq();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeEditServoUpdateRate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACDLG_H__C71C1340_DCB8_448B_9EF9_91C020417543__INCLUDED_)
