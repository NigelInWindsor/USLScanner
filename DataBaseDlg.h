#if !defined(AFX_DATABASEDLG_H__2843354B_943F_4D5A_805D_67DC752527FA__INCLUDED_)
#define AFX_DATABASEDLG_H__2843354B_943F_4D5A_805D_67DC752527FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataBaseDlg.h : header file
//
#include "EditSpinItem.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CDataBaseDlg dialog

class CDataBaseDlg : public CDialog
{
// Construction
public:
	void Sort();
	int	m_nWhich;
	int m_nIndex;
	int m_nNumberColumns;
	int m_nDataL;
	CStringArray m_Column;
	CStringArray m_Data[100];
	bool RetrieveDataBase();
	CStaticPosManage m_StaticPosList;
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	
	CWnd** m_pDlg;
	CDataBaseDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataBaseDlg)
	enum { IDD = IDD_DATABASE_DIALOG };
	CComboBox	m_comboWhich;
	CListCtrl	m_listData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataBaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataBaseDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetdispinfoListData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboWhich();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATABASEDLG_H__2843354B_943F_4D5A_805D_67DC752527FA__INCLUDED_)
