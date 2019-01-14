#if !defined(AFX_RTHETAPAGE_H__359E93B6_10AA_4A99_8A81_F6CF98087B62__INCLUDED_)
#define AFX_RTHETAPAGE_H__359E93B6_10AA_4A99_8A81_F6CF98087B62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RThetaPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRThetaPage dialog

class CRThetaPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRThetaPage)

// Construction
public:
	int m_nRight;
	int m_nLeft;
	void UpdateAllControls();
	
	CRThetaPage();
	~CRThetaPage();

// Dialog Data
	//{{AFX_DATA(CRThetaPage)
	enum { IDD = IDD_R_THETA_PAGE };
	CButton	m_checkW;
	CButton	m_check90;
	CButton	m_check180;
	CButton	m_checkLookBack;
	CComboBox	m_comboPeakTimeslot;
	CButton	m_staticRight;
	CButton	m_staticLeft;
	CEdit	m_editRLeftOffset;
	CEdit	m_editTLeftOffset;
	int		m_nRLeftAbsEncoder;
	int		m_nTLeftAbsEncoder;
	int		m_nRLeftPmacCount;
	int		m_nTLeftPmacCount;
	CString	m_strRLeftAngle;
	CString	m_strTLeftAngle;
	CString	m_RLeftStatus;
	CString	m_TLeftStatus;
	CString	m_strFloatRevNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRThetaPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRThetaPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditRLeftOffset();
	afx_msg void OnChangeEditTLeftOffset();
	afx_msg void OnButtonRLeftOffset();
	afx_msg void OnButtonTLeftOffset();
	afx_msg void OnButtonTest();
	afx_msg void OnSelchangeComboLeftTimeslot();
	afx_msg void OnSelchangeComboRightTimeslot();
	afx_msg void OnSelchangeComboPeakTimeslot();
	afx_msg void OnButtonZeroThetaLeft();
	afx_msg void OnButtonZeroThetaRight();
	afx_msg void OnButtonAcceptRight();
	afx_msg void OnButtonAcceptLeft();
	afx_msg void OnCheckQuadrant();
	afx_msg void OnCheckLookback();
	afx_msg void OnCheck180();
	afx_msg void OnCheck90();
	afx_msg void OnCheckW();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTHETAPAGE_H__359E93B6_10AA_4A99_8A81_F6CF98087B62__INCLUDED_)
