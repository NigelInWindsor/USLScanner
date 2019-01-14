#if !defined(AFX_ACCESSLEVELSDLG_H__2388BEEA_0838_4459_A7D3_C5411D47690A__INCLUDED_)
#define AFX_ACCESSLEVELSDLG_H__2388BEEA_0838_4459_A7D3_C5411D47690A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccessLevelsDlg.h : header file
//
#include "EditSpinItem.h"
#include "StaticPosManage.h"
/////////////////////////////////////////////////////////////////////////////
// CAccessLevelsDlg dialog

class CAccessLevelsDlg : public CDialog
{
// Construction
public:
	HBRUSH m_hBr;
	int m_nColumn;
	int m_nIndex;
	CString m_Menu[100];
	int m_nID[100];
	int m_nL;
	void	ClearEditSpin();
	void	CreateColumns();
	void	FillList();
	void	UpdateAllControls();
//	CString m_AccessLevel[6];
	CWnd** m_pDlg;
	
	CEditSpinItem* m_pEditSpinItem;
	CStaticPosManage m_StaticPosList;
	CAccessLevelsDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAccessLevelsDlg)
	enum { IDD = IDD_ACCESS_LEVEL_DIALOG };
	CEdit	m_editCurrentLogonLevel;
	CMFCListCtrl	m_listID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccessLevelsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccessLevelsDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickListId(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListId(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListId(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonError();
	afx_msg void OnRclickListId(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAccessClearrow();
	afx_msg void OnAccessSetrow();
	afx_msg void OnAccessSetall();
	afx_msg void OnAccessClearall();
	afx_msg void OnAccessSetcolumn();
	afx_msg void OnAccessClearcolumn();
	afx_msg void OnChangeEditCurrentlogonlevel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCESSLEVELSDLG_H__2388BEEA_0838_4459_A7D3_C5411D47690A__INCLUDED_)
