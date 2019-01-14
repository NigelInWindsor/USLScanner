#if !defined(AFX_BRAINEDITDLG_H__240F4061_48DA_4B5C_BA4C_173B868D0F53__INCLUDED_)
#define AFX_BRAINEDITDLG_H__240F4061_48DA_4B5C_BA4C_173B868D0F53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrainEditDlg.h : header file
//
#include "BrainToolsDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CBrainEditDlg dialog

class CBrainEditDlg : public CDialog
{
// Construction
public:
	void SetAccessPrivelages();
	CBrainToolsDlg* m_pBrainTools;
	void UpdateParent(int nAction);
	void UpdateAllControls();
	CWnd* m_pParent;
	CWnd** m_pDlg;
	
	int		m_nLeft;
	int		m_nTop;
	CBrainEditDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nLeft = 0, int nTop = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBrainEditDlg)
	enum { IDD = IDD_EDITBRAIN_DIALOG };
	CButton	m_checkViaDangerPlane;
	CButton	m_checkWaterPath;
	CButton	m_checkMoveRight;
	CButton	m_checkNormalize;
	CButton	m_checkPeakOpposite;
	CComboBox	m_comboOptomizeWhat;
	CComboBox	m_comboMovetoUsing;
	CSpinButtonCtrl	m_spinLineNumber;
	CEdit	m_editLineNumber;
	CButton	m_checkOn;
	CButton	m_checkLines;
	CButton	m_checkCrosses;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrainEditDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBrainEditDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckLines();
	afx_msg void OnCheckCrosses();
	afx_msg void OnCheckOn();
	afx_msg void OnChangeEditLineNumber();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonTools();
	afx_msg void OnSelchangeComboOptomizeWhat();
	afx_msg void OnSelchangeComboMovetoUsing();
	afx_msg void OnCheckPeakOpposite();
	afx_msg void OnButtonMakeItSo();
	afx_msg void OnCheckWp0();
	afx_msg void OnCheckNormalize();
	afx_msg void OnCheckMoveRight();
	afx_msg void OnCheckViaDangerPlane();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRAINEDITDLG_H__240F4061_48DA_4B5C_BA4C_173B868D0F53__INCLUDED_)
