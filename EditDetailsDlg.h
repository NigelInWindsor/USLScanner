#if !defined(AFX_EDITDETAILSDLG_H__C6FCB9D6_C8A1_4E9F_B729_2F6805A099FD__INCLUDED_)
#define AFX_EDITDETAILSDLG_H__C6FCB9D6_C8A1_4E9F_B729_2F6805A099FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDetailsDlg.h : header file
//
#include "EditSpinItem.h"
#include "StaticPosManage.h"
#include "DetailsPage.h"

/////////////////////////////////////////////////////////////////////////////
// CEditDetailsDlg dialog

class CEditDetailsDlg : public CDialog
{
// Construction
public:
	CStaticPosManage m_StaticPosList;
	CString m_Automatic[10];
	CString m_Ctrl[10];
	void RefreshDetailsPage();
	HRESULT ItemChanged(WPARAM, LPARAM);
	bool DeleteDescription(int nIndex);
	CDetailsPage* m_pDetailsPage;
	void ClearEditSpin();
	void UpdateAllControls();
	void FillList();
	void CreateColumns();
	CEditSpinItem* m_pEditSpinItem;
	int m_nIndex;
	CWnd** m_pDlg;
	
	CEditDetailsDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditDetailsDlg)
	enum { IDD = IDD_EDIT_DETAILS_DIALOG };
	CListCtrl	m_listDetails;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditDetailsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditDetailsDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListDetails(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDetails(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListDetails(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListDetails(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditAdd();
	afx_msg void OnEditDelete();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITDETAILSDLG_H__C6FCB9D6_C8A1_4E9F_B729_2F6805A099FD__INCLUDED_)
