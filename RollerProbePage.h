#if !defined(AFX_ROLLERPROBEPAGE_H__464FAA66_90B5_462F_95E3_7DE50696FEAC__INCLUDED_)
#define AFX_ROLLERPROBEPAGE_H__464FAA66_90B5_462F_95E3_7DE50696FEAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RollerProbePage.h : header file
//
#include "EditSpinItem.h"

/////////////////////////////////////////////////////////////////////////////
// CRollerProbePage dialog

class CRollerProbePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRollerProbePage)

// Construction
public:
	void SetToolBarCheckedState();
	void	ClearEditSpin();
	CEditSpinItem* m_pEditSpinItem;
	int		m_nIndex;
	void FillList();
	void CreateColumns();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	void UpdateAllControls();
	bool m_bLBDown;
	
	CRollerProbePage();
	~CRollerProbePage();

// Dialog Data
	//{{AFX_DATA(CRollerProbePage)
	enum { IDD = IDD_ROLLER_PROBE_PAGE };
	CButton	m_checkUnidirectionalScan;
	CListCtrl	m_listRandomCoords;
	CSpinButtonCtrl	m_spinExtraArmLength;
	CEdit	m_editExtraArmLength;
	CButton	m_checkGotoWithStepOff;
	CSpinButtonCtrl	m_spinScanSpeed;
	CSpinButtonCtrl	m_spinMoveToSpeed;
	CSpinButtonCtrl	m_spinJogSpeed;
	CEdit	m_editScanSpeed;
	CEdit	m_editMoveToSpeed;
	CEdit	m_editJogSpeed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRollerProbePage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRollerProbePage)
	afx_msg void OnChangeEditJogSpeed();
	afx_msg void OnDeltaposSpinJogSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditMovetoSpeed();
	afx_msg void OnDeltaposSpinMovetoSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditScanSpeed();
	afx_msg void OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckStepOff();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonAddEnvelopePt();
	afx_msg void OnDeltaposSpinExtraArmLength(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditExtraArmLength();
	afx_msg void OnClickListRandomCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListRandomCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListRandomCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRollerAdd();
	afx_msg void OnRollerDelete();
	afx_msg void OnRollerGoto();
	afx_msg void OnRollerModify();
	afx_msg void OnCheckUnidirectionalScan();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROLLERPROBEPAGE_H__464FAA66_90B5_462F_95E3_7DE50696FEAC__INCLUDED_)
