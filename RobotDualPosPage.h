#if !defined(AFX_ROBOTDUALPOSPAGE_H__72B1910A_8ED7_4DED_9390_F25F5A787C09__INCLUDED_)
#define AFX_ROBOTDUALPOSPAGE_H__72B1910A_8ED7_4DED_9390_F25F5A787C09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotDualPosPage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CRobotDualPosPage dialog

class CRobotDualPosPage : public CResizablePage
{
	DECLARE_DYNCREATE(CRobotDualPosPage)

// Construction
public:
	HBRUSH m_hBr;
	void SetStaticNames();
	int m_nOldMeasurementUnits;
	CCoord m_CpOld;
	inline void MinMax(int *nn,int nMin,int nMax);
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	void AddAnchor(CWnd* pCtrl);
	
	CRobotDualPosPage();
	~CRobotDualPosPage();

// Dialog Data
	//{{AFX_DATA(CRobotDualPosPage)
	enum { IDD = IDD_ROBOT_DUAL_POS_PAGE };
	CStatic	m_staticZRight;
	CStatic	m_staticZLeft;
	CStatic	m_staticYRight;
	CStatic	m_staticYLeft;
	CStatic	m_staticXRight;
	CStatic	m_staticXLeft;
	CButton	m_staticRight;
	CButton	m_staticLeft;
	CStatic	m_staticKRight;
	CStatic	m_staticKLeft;
	CStatic	m_staticJRight;
	CStatic	m_staticJLeft;
	CStatic	m_staticIRight;
	CStatic	m_staticILeft;
	CEdit	m_editZRight;
	CEdit	m_editZLeft;
	CEdit	m_editYRight;
	CEdit	m_editYLeft;
	CEdit	m_editJRight;
	CEdit	m_editIRight;
	CEdit	m_editXRight;
	CEdit	m_editXLeft;
	CEdit	m_editKRight;
	CEdit	m_editKLeft;
	CEdit	m_editJLeft;
	CEdit	m_editILeft;
	CComboBox	m_comboDisplayMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobotDualPosPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRobotDualPosPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboPositionMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTDUALPOSPAGE_H__72B1910A_8ED7_4DED_9390_F25F5A787C09__INCLUDED_)
