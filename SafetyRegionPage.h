#if !defined(AFX_SAFETYREGIONPAGE_H__DEB0CB29_45F6_4BDB_BBBF_9FDAF7EFBF50__INCLUDED_)
#define AFX_SAFETYREGIONPAGE_H__DEB0CB29_45F6_4BDB_BBBF_9FDAF7EFBF50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SafetyRegionPage.h : header file
//
#include "EditSpinItem.h"

/////////////////////////////////////////////////////////////////////////////
// CSafetyRegionPage dialog

class CSafetyRegionPage : public CPropertyPage
{

	DECLARE_DYNCREATE(CSafetyRegionPage)

// Construction
public:
	CSafetyRegionPage(CWnd* pParent = NULL);   // standard constructor
	void CreateColumns();
	void FillList();
	void ClearEditSpin();



	CEditSpinItem* m_pEditSpinItem;

// Dialog Data
	//{{AFX_DATA(CSafetyRegionPage)
	enum { IDD = IDD_SAFETY_REGIONS_PAGE };
	CListCtrl	m_listPermissableFunctions;
	CListCtrl	m_listAxes;
	CComboBox	m_comboRegion;
	CButton	m_checkEnableJoystick;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSafetyRegionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSafetyRegionPage)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoListAxes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListPermissableFunctions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListAxes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListPermissableFunctions(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAFETYREGIONPAGE_H__DEB0CB29_45F6_4BDB_BBBF_9FDAF7EFBF50__INCLUDED_)
