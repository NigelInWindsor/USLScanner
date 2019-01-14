#if !defined(AFX_TURNTABLESCANPAGE_H__B7A1E4C9_2B2E_494D_BF81_71308DDE8B90__INCLUDED_)
#define AFX_TURNTABLESCANPAGE_H__B7A1E4C9_2B2E_494D_BF81_71308DDE8B90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TurnTableScanPage.h : header file
//
#include "EditColour.h"

/////////////////////////////////////////////////////////////////////////////
// CTurnTableScanPage dialog

class CTurnTableScanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTurnTableScanPage)

// Construction
public:
	HBRUSH m_hBr;
	float m_fMaxSpeed;
	void UpdateFastIndexEquivelance();
	CCoord m_CpSurface;
	int	m_nOldScanLine;
	
	void SetAccessPrivelage();
	void UpdateAllControls();
	CTurnTableScanPage();
	~CTurnTableScanPage();

// Dialog Data
	//{{AFX_DATA(CTurnTableScanPage)
	enum { IDD = IDD_TURNTABLE_SCAN_PAGE };
	CEditColour	m_editMaxSpeed;
	CComboBox	m_comboLineIndex;
	CButton	m_buttonEccentricCompensation;
	CEdit	m_editFastIndexEquivelance;
	CSpinButtonCtrl	m_spinFastIncrement;
	CEdit	m_editFastIncrement;
	CEdit	m_editRPos;
	CEdit	m_editIndex;
	CSpinButtonCtrl	m_spinIndex;
	CSpinButtonCtrl	m_spinScanSpeed;
	CEdit	m_editFilePath;
	CComboBox	m_comboScanMode;
	CComboBox	m_comboScanType;
	CEdit	m_editResumeLine;
	CEdit	m_editStartLine;
	CEdit	m_editScanSpeed;
	CEdit	m_editScanLine;
	CEdit	m_editFinishLine;
	CEdit	m_editFinishAngle;
	CEdit	m_editStartAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTurnTableScanPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTurnTableScanPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditStartLine();
	afx_msg void OnChangeEditFinishLine();
	afx_msg void OnChangeEditIndex();
	afx_msg void OnChangeEditStartAngle();
	afx_msg void OnChangeEditFinishAngle();
	afx_msg void OnChangeEditScanSpeed();
	afx_msg void OnChangeEditResumeLine();
	afx_msg void OnChangeEditFilepath();
	afx_msg void OnSelchangeComboScanMode();
	afx_msg void OnSelchangeComboScanType();
	afx_msg void OnButtonStartAngle();
	afx_msg void OnButtonFinishAngle();
	afx_msg void OnButtonStartLine();
	afx_msg void OnButtonFinishLine();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonAbort();
	afx_msg void OnButtonBrowse();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditFastIndex();
	afx_msg void OnCheckUseEccentric();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeComboSlowIndex();
	afx_msg void OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnKillActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TURNTABLESCANPAGE_H__B7A1E4C9_2B2E_494D_BF81_71308DDE8B90__INCLUDED_)
