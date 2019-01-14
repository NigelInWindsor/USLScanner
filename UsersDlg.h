#if !defined(AFX_USERSDLG_H__05798284_51BD_4461_BBF8_7614CFE60414__INCLUDED_)
#define AFX_USERSDLG_H__05798284_51BD_4461_BBF8_7614CFE60414__INCLUDED_

#include "Users.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UsersDlg.h : header file
//
#include "EditSpinItem.h"
#include "StaticPosManage.h"
/////////////////////////////////////////////////////////////////////////////
// CUsersDlg dialog

class CUsersDlg : public CDialogEx
{
// Construction
public:
	CStaticPosManage m_StaticPosList;
	int m_nIndex;
	CString m_AccessLevel[10];
	void ClearEditSpin();
	CUsers m_Users;
	void UpdateAllControls();
	void FillList();
	void CreateColumns();

	CEditSpinItem* m_pEditSpinItem;
	CWnd** m_pDlg;
	
	CUsersDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUsersDlg)
	enum { IDD = IDD_USERS_DIALOG };
	CEdit	m_editLogOffTime;
	CComboBox	m_comboLogonLevel;
	CComboBox	m_comboForceLogon;
	CListCtrl	m_listUsers;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsersDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUsersDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual void SetBackgroundColor(COLORREF color, BOOL bRepaint = 1);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickListUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditAdd();
	afx_msg void OnSelchangeComboLogonLevel();
	afx_msg void OnChangeEditLogoffTime();
	afx_msg void OnSelchangeComboForceLogon();
	afx_msg void OnTrackListUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnddragListUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegintrackListUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERSDLG_H__05798284_51BD_4461_BBF8_7614CFE60414__INCLUDED_)
