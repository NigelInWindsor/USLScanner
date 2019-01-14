#if !defined(AFX_MAINTENANCEPAGE_H__4F1E9A70_DA6A_40AE_AF2E_13657BC7B672__INCLUDED_)
#define AFX_MAINTENANCEPAGE_H__4F1E9A70_DA6A_40AE_AF2E_13657BC7B672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaintenancePage.h : header file
//
#include "StaticPosManage.h"
#include "EditSpinItem.h"

/////////////////////////////////////////////////////////////////////////////
// CMaintenancePage dialog

class CMaintenancePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMaintenancePage)

// Construction
public:
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	
	int	m_nIndex;
	CEditSpinItem* m_pEditSpinItem;
	CMaintenancePage();
	~CMaintenancePage();

// Dialog Data
	//{{AFX_DATA(CMaintenancePage)
	enum { IDD = IDD_MAINTENANCE_PAGE };
	CListCtrl	m_listSchedule;
	CEdit	m_editMachineName;
	CEdit	m_editMachineLocation;
	CEdit	m_editCustomer;
	CEdit	m_editInstalationDate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMaintenancePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMaintenancePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAccept();
	afx_msg void OnClickListSchedule(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListSchedule(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListSchedule(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListSchedule(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMaintenanceClearalltotaldistances();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINTENANCEPAGE_H__4F1E9A70_DA6A_40AE_AF2E_13657BC7B672__INCLUDED_)
