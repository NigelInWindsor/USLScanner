#if !defined(AFX_TOOLCHANGERPAGE_H__0B6552E3_DB9C_4E9A_A3F7_5254E1E5E5D1__INCLUDED_)
#define AFX_TOOLCHANGERPAGE_H__0B6552E3_DB9C_4E9A_A3F7_5254E1E5E5D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolChangerPage.h : header file
//
#include "EditSpinItem.h"
#include "EditColour.h"

/////////////////////////////////////////////////////////////////////////////
// CToolChangerPage dialog

class CToolChangerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CToolChangerPage)

// Construction
public:
	int			m_nSortDir;
	int			m_nSortColumn;
	float		m_fEdit;
	int			m_nDoorStatus[2];
	int			m_nToolID[2];
	int			m_nToolHolderStatus[2];
	HRESULT		ItemChanged(WPARAM wParam, LPARAM lParam);
	BOOL		OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	void		SetToolBarCheckedState();
	CToolBar	m_wndToolBar;
	CToolBar	m_wndOriginFingerToolBar;
	CReBar		m_wndRebar;
	CString			m_strID[16];
	CEditSpinItem*	m_pEditSpinItem;
	int m_nColumn;
	int m_nIndex;
	
	HBRUSH m_hBr;

	void SetAccessPrivelages();
	void UpdateAllControls();
	void CreateColumns();
	void FillList();
	CToolChangerPage();
	~CToolChangerPage();

// Dialog Data
	//{{AFX_DATA(CToolChangerPage)
	enum { IDD = IDD_TOOL_CHANGER_PAGE };
	CEdit	m_editZLatchDistance;
	CEdit	m_editLatchStandoffDistance;
	CEditColour	m_editStatus1;
	CEditColour	m_editStatus0;
	CMFCListCtrl	m_listTools;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CToolChangerPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CToolChangerPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnGetdispinfoListTools(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickListTools(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListTools(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListTools(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolchangerStorecurrentposition();
	afx_msg void OnToolchangerDelete();
	afx_msg void OnChangeEditZLatchDistance();
	afx_msg void OnChangeEditLatchStandoffDistance();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonLockTool0();
	afx_msg void OnButtonLockTool1();
	afx_msg void OnButtonUnlockTool0();
	afx_msg void OnButtonUnlockTool1();
	afx_msg void OnButtonRetrieveTool0();
	afx_msg void OnButtonRetrieveTool1();
	afx_msg void OnButtonReturnTool0();
	afx_msg void OnButtonReturnTool1();
	afx_msg void OnItemclickListTools(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListTools(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolchangerRotatehead();
	afx_msg void OnButtonOriginFingerManipulator();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLCHANGERPAGE_H__0B6552E3_DB9C_4E9A_A3F7_5254E1E5E5D1__INCLUDED_)
