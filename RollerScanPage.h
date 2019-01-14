#if !defined(AFX_ROLLERSCANPAGE_H__4C8490BC_6406_497B_96D2_D27E25EBC8A9__INCLUDED_)
#define AFX_ROLLERSCANPAGE_H__4C8490BC_6406_497B_96D2_D27E25EBC8A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RollerScanPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRollerScanPage dialog

class CRollerScanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRollerScanPage)

// Construction
public:
	HBRUSH m_hBr;
	int		m_nOldScanLine;
	int m_nOldThreadAction;
	void UpdateMainFrameTools();
	void GenerateScanLines();
	void Goto(int nWhich);
	void UpdateAllControls();
	
	CRollerScanPage();
	~CRollerScanPage();

// Dialog Data
	//{{AFX_DATA(CRollerScanPage)
	enum { IDD = IDD_ROLLER_SCAN_PAGE };
	CEdit	m_editScanSpeed;
	CButton	m_buttonStartPause;
	CEdit	m_editResumeLine;
	CEdit	m_editScanLine;
	CEdit	m_editCircumDistance;
	CSpinButtonCtrl	m_spinSlowIncrement;
	CSpinButtonCtrl	m_spinFastIncrement;
	CEdit	m_editFastIncrement;
	CEdit	m_editSlowIncrement;
	CEdit	m_editCurrentPos;
	CEdit	m_editStartCoord;
	CEdit	m_editFinishCoord;
	CEdit	m_editDiameter;
	CSpinButtonCtrl	m_spinDiameter;
	CComboBox	m_comboScanType;
	CEdit	m_editFilePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRollerScanPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRollerScanPage)
	afx_msg void OnChangeEditFilepath();
	afx_msg void OnButtonBrowse();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboScanType();
	afx_msg void OnChangeEditDiameter();
	afx_msg void OnDeltaposSpinDiameter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonStartCoord();
	afx_msg void OnButtonFinishCoord();
	afx_msg void OnChangeEditStartCoord();
	afx_msg void OnChangeEditFinishCoord();
	afx_msg void OnButtonZero();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonGotoStart();
	afx_msg void OnButtonGotoFinish();
	afx_msg void OnChangeEditFastIncrement();
	afx_msg void OnChangeEditSlowIncrement();
	afx_msg void OnDeltaposSpinFastIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonStart();
	afx_msg void OnChangeEditScanLine();
	afx_msg void OnChangeEditResumeLine();
	afx_msg void OnChangeEditScanSpeed();
	afx_msg void OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCheckDiameterFromCoords();
	CButton m_checkDiameterFromCoords;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROLLERSCANPAGE_H__4C8490BC_6406_497B_96D2_D27E25EBC8A9__INCLUDED_)
