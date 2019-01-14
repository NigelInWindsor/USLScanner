#if !defined(AFX_HOLESEDITDLG_H__8A0A0015_0643_4BA8_A146_D7F726126E3F__INCLUDED_)
#define AFX_HOLESEDITDLG_H__8A0A0015_0643_4BA8_A146_D7F726126E3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HolesEditDlg.h : header file
//
#include "ExclusionZonesDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CHolesEditDlg dialog

class CHolesEditDlg : public CDialog
{
// Construction
public:
	CExclusionZonesDlg* m_pExclusionZonesDlg;
	void UpdateParent(int nAction);
	void UpdateAllControls();
	CWnd* m_pParent;
	CWnd** m_pDlg;
	
	int		m_nLeft;
	int		m_nTop;
	CHolesEditDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nLeft = 0, int nTop = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHolesEditDlg)
	enum { IDD = IDD_EDITHOLES_DIALOG };
	CButton	m_checkVectorLengths;
	CButton	m_checkLockRefPts;
	CComboBox	m_comboComputeMode;
	CEdit	m_editOverallHeight;
	CEdit	m_editBallDiameter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHolesEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHolesEditDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditBallDiameter();
	afx_msg void OnChangeEditOverallheight();
	afx_msg void OnSelchangeCombocomputeMode();
	afx_msg void OnButtonCompute();
	afx_msg void OnButtonExclusion();
	afx_msg void OnButtonInvertJ();
	afx_msg void OnCheckLockRef();
	afx_msg void OnCheckVectorLengths();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLESEDITDLG_H__8A0A0015_0643_4BA8_A146_D7F726126E3F__INCLUDED_)
