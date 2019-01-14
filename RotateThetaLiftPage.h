#if !defined(AFX_ROTATETHETALIFTPAGE_H__F22EEC26_10B1_4D23_8EB2_9B91FE63D522__INCLUDED_)
#define AFX_ROTATETHETALIFTPAGE_H__F22EEC26_10B1_4D23_8EB2_9B91FE63D522__INCLUDED_

#include "Coord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotateThetaLiftPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRotateThetaLiftPage dialog

class CRotateThetaLiftPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRotateThetaLiftPage)

// Construction
public:
	void UpdateAllControls();
	CCoord m_CpHead;
	void SetAccessPrivelages();
	
	CRotateThetaLiftPage();
	~CRotateThetaLiftPage();

// Dialog Data
	//{{AFX_DATA(CRotateThetaLiftPage)
	enum { IDD = IDD_R_THETA_LIFT_PAGE };
	CButton	m_checkRAxis;
	CEdit	m_editDeltaThetaAngle;
	CButton	m_buttonTankZero;
	CButton	m_buttonLiftDown;
	CButton	m_buttonLiftUp;
	CButton	m_buttonSide0;
	CButton	m_buttonSide1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRotateThetaLiftPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRotateThetaLiftPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOriginAll();
	afx_msg void OnButtonOriginX();
	afx_msg void OnButtonOriginY();
	afx_msg void OnButtonOriginZ();
	afx_msg void OnButtonOriginRotate();
	afx_msg void OnButtonOriginTheta();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonSide0();
	afx_msg void OnButtonSide1();
	afx_msg void OnButtonLiftUp();
	afx_msg void OnButtonLiftDown();
	afx_msg void OnButtonLiftStop();
	afx_msg void OnButtonTankZero();
	afx_msg void OnButtonRZero();
	afx_msg void OnButtonR180();
	afx_msg void OnButtonThetaPlus();
	afx_msg void OnButtonThetaMinus();
	afx_msg void OnChangeEditDeltaThetaAngle();
	afx_msg void OnCheckRAxis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATETHETALIFTPAGE_H__F22EEC26_10B1_4D23_8EB2_9B91FE63D522__INCLUDED_)
