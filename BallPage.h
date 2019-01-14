#if !defined(AFX_BALLPAGE_H__B6E8E780_3082_4068_B9B1_D98DD809F330__INCLUDED_)
#define AFX_BALLPAGE_H__B6E8E780_3082_4068_B9B1_D98DD809F330__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BallPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBallPage dialog

class CBallPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBallPage)

// Construction
public:
	HBRUSH m_hBr;
	void SetAccessPrivelages();
	void UpdateAxesSheet();
	void UpdateAllControls();
	CBallPage();
	~CBallPage();

// Dialog Data
	//{{AFX_DATA(CBallPage)
	enum { IDD = IDD_BALL_PAGE };
	CButton	m_buttonSide1EqualSide0;
	CButton	m_staticRight;
	CButton	m_staticLeft;
	CButton	m_buttonZtRight;
	CButton	m_buttonZtLeft;
	CButton	m_buttonXtRight;
	CButton	m_buttonXtLeft;
	CEdit	m_editOffsetName;
	CEdit	m_editZRight;
	CEdit	m_editZLeft;
	CEdit	m_editYRight;
	CEdit	m_editYLeft;
	CEdit	m_editXRight;
	CEdit	m_editXLeft;
	CEdit	m_editXTipLeft;
	CEdit	m_editZTipRight;
	CEdit	m_editZTipLeft;
	CEdit	m_editXTipRight;
	CComboBox	m_comboRightTimeSlot;
	CComboBox	m_comboLeftTimeSlot;
	CEdit	m_editRightPos;
	CEdit	m_editLeftPos;
	CEdit	m_editDiameter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBallPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBallPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnChangeEditDiameter();
	afx_msg void OnButtonStoreLeft();
	afx_msg void OnButtonStoreRight();
	afx_msg void OnSelchangeComboRightTimeslot();
	afx_msg void OnSelchangeComboLeftTimeslot();
	afx_msg void OnButtonGotoRight();
	afx_msg void OnButtonGotoLeft();
	afx_msg void OnButtonXyzLeft();
	afx_msg void OnButtonXyzRight();
	afx_msg void OnButtonSetXtipLeft();
	afx_msg void OnButtonSetZtipLeft();
	afx_msg void OnButtonSetXtipRight();
	afx_msg void OnButtonSetZtipRight();
	afx_msg void OnButtonZeroBridge();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonSave();
	afx_msg void OnDeltaposSpinXtLeft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinXtRight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinZtLeft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinZtRight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditLeftPos();
	afx_msg void OnChangeEditRightPos();
	afx_msg void OnButtonGotoBoth();
	afx_msg void OnButtonZeroZero();
	afx_msg void OnButtonSide1EqualSide0();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BALLPAGE_H__B6E8E780_3082_4068_B9B1_D98DD809F330__INCLUDED_)
