#if !defined(AFX_ROBOTUSERPAGE_H__C311893C_115A_4A08_B0F1_D4CBFF0A3DC3__INCLUDED_)
#define AFX_ROBOTUSERPAGE_H__C311893C_115A_4A08_B0F1_D4CBFF0A3DC3__INCLUDED_

#include "EditSpinCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotUserPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRobotUserPage dialog

class CRobotUserPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRobotUserPage)

// Construction
public:
	float m_fJogZ[2];
	HBRUSH m_hBr;
	void 	UpdateAllControls();
	
	CRobotUserPage();
	~CRobotUserPage();

// Dialog Data
	//{{AFX_DATA(CRobotUserPage)
	enum { IDD = IDD_ROBOT_USER_PAGE };
	CEditSpinCtrl	m_editspinZ1;
	CEditSpinCtrl	m_editspinZ0;
	CEditSpinCtrl	m_editspinY1;
	CEditSpinCtrl	m_editspinY0;
	CEditSpinCtrl	m_editspinX1;
	CEditSpinCtrl	m_editspinX0;
	CButton	m_staticLeft;
	CButton	m_staticRight;
	CSliderCtrl	m_sliderZAxis1;
	CSliderCtrl	m_sliderYAxis1;
	CSliderCtrl	m_sliderXAxis1;
	CSliderCtrl	m_sliderZAxis0;
	CSliderCtrl	m_sliderYAxis0;
	CSliderCtrl	m_sliderXAxis0;
	CButton	m_checkIncludeZAxis;
	CButton	m_checkIncludeYAxis;
	CButton	m_checkIncludeXAxis;
	CEditSpinCtrl m_editSpinBallDiameter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobotUserPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRobotUserPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonCartesian0();
	afx_msg void OnButtonMixed0();
	afx_msg void OnButtonRobot0();
	afx_msg void OnButtonCartesian1();
	afx_msg void OnButtonMixed1();
	afx_msg void OnButtonRobot1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDeltaposSpinX0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinY0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinZ0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinX1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinY1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinZ1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditX0();
	afx_msg void OnEnChangeEditY0();
	afx_msg void OnEnChangeEditZ0();
	afx_msg void OnEnChangeEditX1();
	afx_msg void OnEnChangeEditY1();
	afx_msg void OnEnChangeEditZ1();
	afx_msg void OnBnClickedButtonZeroSide0();
	afx_msg void OnBnClickedButtonZeroSide1();
	afx_msg void OnEnChangeEditBallDiameter();
	afx_msg void OnDeltaposSpinBallDiameter(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton0Equal1();
	afx_msg void OnBnClickedButton1Equal0();
	CButton m_btn0Equal1;
	CButton m_btn1Equal0;
	CSliderCtrl m_sliderJogZ0;
	CSliderCtrl m_sliderJogZ1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTUSERPAGE_H__C311893C_115A_4A08_B0F1_D4CBFF0A3DC3__INCLUDED_)
