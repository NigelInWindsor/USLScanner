#if !defined(AFX_TUBESCANPAGE_H__AF90F3D6_47ED_4C19_AFF5_E1002EE321BA__INCLUDED_)
#define AFX_TUBESCANPAGE_H__AF90F3D6_47ED_4C19_AFF5_E1002EE321BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TubeScanPage1.h : header file
//
#include "EditColour.h"
#include "EditSpinItem.h"

/////////////////////////////////////////////////////////////////////////////
// CTubeScanPage dialog

class CTubeScanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTubeScanPage)

// Construction
public:
	int		m_nOldScanType;
	int		m_nOldThreadAction;
	void SetToolBarCheckedState();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	CCoord	m_CpSurface;
	int		m_nOldScanLine;
	int		m_nScanType;
	float m_fMaxSpeed;
	HRESULT ItemFinished(WPARAM, LPARAM);
	void GenerateScanLines();
	int m_nColumn;
	int	m_nEditType;
	HRESULT ItemChanged(WPARAM, LPARAM);
	CString m_strEdit;
	CEditSpinItem* m_pEditSpinItem;
	void ClearEditSpin();
	int m_nIndex;
	
	void UpdateAllControls();
	void FillList();
	void CreateColumns();
	CTubeScanPage();
	~CTubeScanPage();

// Dialog Data
	//{{AFX_DATA(CTubeScanPage)
	enum { IDD = IDD_TUBE_SCAN_PAGE };
	CEdit	m_editFilePath;
	CEdit	m_editPosition;
	CSpinButtonCtrl	m_spinDiameter;
	CEdit	m_editDiameter;
	CEdit	m_editResumeLine;
	CEdit	m_editScanSpeed;
	CSpinButtonCtrl	m_spinScanSpeed;
	CEditColour	m_editMaxSpeed;
	CEdit	m_editScanLine;
	CSpinButtonCtrl	m_spinSlowIncrement;
	CSpinButtonCtrl	m_spinFastIncrement;
	CEdit	m_editSlowIncrement;
	CEdit	m_editFastIncrement;
	CComboBox	m_comboScanType;
	CListCtrl	m_listCoords;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTubeScanPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTubeScanPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTubesAdd();
	afx_msg void OnTubesDelete();
	afx_msg void OnTubesModify();
	afx_msg void OnTubesGoto();
	afx_msg void OnDblclkListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTubesClearall();
	afx_msg void OnDeltaposSpinFastIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditFasyIncrement();
	afx_msg void OnChangeEditSlowIncrement();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonAbort();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditScanSpeed();
	afx_msg void OnChangeEditResumeLine();
	afx_msg void OnDeltaposSpinDiameter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditDiameter();
	afx_msg void OnButtonZero();
	afx_msg void OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonZeroRotate();
	afx_msg void OnButtonHelicalScan();
	afx_msg void OnButtonRectilinearScan();
	afx_msg void OnChangeEditFilepath();
	afx_msg void OnButtonBrowse();
	afx_msg void OnTubesSortX();
	afx_msg void OnTubesSortY();
	afx_msg void OnTubesSortZ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TUBESCANPAGE_H__AF90F3D6_47ED_4C19_AFF5_E1002EE321BA__INCLUDED_)
