#if !defined(AFX_3DSTIFFENERPAGE_H__091591D6_B686_4317_8CE4_29B82B03CD01__INCLUDED_)
#define AFX_3DSTIFFENERPAGE_H__091591D6_B686_4317_8CE4_29B82B03CD01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DStiffenerPage.h : header file
//
#include "EditSpinItem.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// C3DStiffenerPage dialog

class C3DStiffenerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(C3DStiffenerPage)

// Construction
public:
	CEditSpinItem* m_pEditSpinItem;
	CString m_strAction[5];
	int m_nIndex;
	void FillDepartList();
	void FillApproachList();
	void CreateColumns();
	
	void UpdateAllControls();
	C3DStiffenerPage();
	~C3DStiffenerPage();

// Dialog Data
	//{{AFX_DATA(C3DStiffenerPage)
	enum { IDD = IDD_3D_STIFFENER_PAGE };
	CListCtrl	m_listDepart;
	CListCtrl	m_listApproach;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(C3DStiffenerPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C3DStiffenerPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoListApproach(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListApproach(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListApproach(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListDepart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListDepart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDepart(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DSTIFFENERPAGE_H__091591D6_B686_4317_8CE4_29B82B03CD01__INCLUDED_)
