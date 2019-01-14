#if !defined(AFX_SAFETYINTERLOCKSDLG_H__BF99FA8F_641C_47A2_8F86_C37A3795C1E7__INCLUDED_)
#define AFX_SAFETYINTERLOCKSDLG_H__BF99FA8F_641C_47A2_8F86_C37A3795C1E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SafetyInterlocksDlg.h : header file
//
#include "EditSpinItem.h"
#include "StaticPosManage.h"

#define	MAX_ENTRYS	1024
/////////////////////////////////////////////////////////////////////////////
// CSafetyInterlocksDlg dialog

class CSafetyInterlocksDlg : public CDialog
{
// Construction
public:
	int	m_nIndex;
	int	m_nL;
	CString	m_Menu[MAX_ENTRYS];
	CString m_Logic[3];

	void ClearEditSpin();
	void FillList();
	void CreateColumns();
	CEditSpinItem* m_pEditSpinItem;
	CStaticPosManage m_StaticPosList;
	
	void UpdateAllControls();
	CWnd** m_pDlg;
	CSafetyInterlocksDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSafetyInterlocksDlg)
	enum { IDD = IDD_SAFETY_INTERLOCKS_DLG };
	CComboBox	m_comboIOController;
	CEdit	m_editInPort;
	CListCtrl	m_listSafety;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSafetyInterlocksDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSafetyInterlocksDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnClickListSafety(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListSafety(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListSafety(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboIocontroller();
	afx_msg void OnRclickListSafety(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSafetyPastecurrentstatus();
	afx_msg void OnSafetyClearall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAFETYINTERLOCKSDLG_H__BF99FA8F_641C_47A2_8F86_C37A3795C1E7__INCLUDED_)
