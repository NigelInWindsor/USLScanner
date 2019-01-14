#if !defined(AFX_POSITIONFOKKERPAGE_H__9877E0D9_C977_414B_A8C2_5B9AAD0C997E__INCLUDED_)
#define AFX_POSITIONFOKKERPAGE_H__9877E0D9_C977_414B_A8C2_5B9AAD0C997E__INCLUDED_

#include "Coord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PositionFokkerPage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CPositionPage dialog

class CPositionPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPositionPage)

// Construction
public:
	HBRUSH m_hBr;
	int m_nOldMeasurementUnits;
	CCoord m_CpOld;
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	void AddAnchor(CWnd* pCtrl);
	
	CPositionPage();
	~CPositionPage();

// Dialog Data
	//{{AFX_DATA(CPositionPage)
	enum { IDD = IDD_POSITION_DUAL_SQUIRTER_PAGE };
	CStatic	m_staticR;
	CStatic	m_staticBridge;
	CEdit	m_editPosR;
	CEdit	m_editPosBridge;
	CComboBox	m_comboPositionMode;
	CEdit	m_editPosXRight;
	CEdit	m_editPosYRight;
	CEdit	m_editPosZRight;
	CEdit	m_editPosXtRight;
	CEdit	m_editPosZtRight;
	CEdit	m_editPosXLeft;
	CEdit	m_editPosYLeft;
	CEdit	m_editPosZLeft;
	CEdit	m_editPosXtLeft;
	CEdit	m_editPosZtLeft;
	CStatic	m_staticXRight;
	CStatic	m_staticYRight;
	CStatic	m_staticZRight;
	CStatic	m_staticXtRight;
	CStatic	m_staticZtRight;
	CStatic	m_staticXLeft;
	CStatic	m_staticYLeft;
	CStatic	m_staticZLeft;
	CStatic	m_staticXtLeft;
	CStatic	m_staticZtLeft;
	CButton	m_staticRight;
	CButton	m_staticLeft;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPositionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPositionPage)
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

#endif // !defined(AFX_POSITIONFOKKERPAGE_H__9877E0D9_C977_414B_A8C2_5B9AAD0C997E__INCLUDED_)
