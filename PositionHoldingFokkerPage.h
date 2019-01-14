#if !defined(AFX_POSITIONHOLDINGFOKKERPAGE_H__F021320A_A1CB_4FC7_B408_C1142288E460__INCLUDED_)
#define AFX_POSITIONHOLDINGFOKKERPAGE_H__F021320A_A1CB_4FC7_B408_C1142288E460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PositionHoldingFokkerPage.h : header file
//
#include "ResizablePage.h"
/////////////////////////////////////////////////////////////////////////////
// CPositionHoldingFokkerPage dialog

class CPositionHoldingFokkerPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPositionHoldingFokkerPage)

// Construction
public:
	inline void MinMax(int *nn,int nMin,int nMax);
	int nOldPos[32];
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	void AddAnchor(CWnd* pCtrl);
	
	CPositionHoldingFokkerPage();
	~CPositionHoldingFokkerPage();

// Dialog Data
	//{{AFX_DATA(CPositionHoldingFokkerPage)
	enum { IDD = IDD_POSITION_HOLDINGFOKKER };
	CEdit	m_editXFixed;
	CStatic	m_staticXFixed;
	CButton	m_staticMoving;
	CButton	m_staticFixed;
	CStatic	m_staticXMoving;
	CStatic	m_staticZMoving;
	CStatic	m_staticZFixed;
	CStatic	m_staticYMoving;
	CStatic	m_staticYFixed;
	CEdit	m_editZMoving;
	CEdit	m_editZFixed;
	CEdit	m_editYMoving;
	CEdit	m_editYFixed;
	CEdit	m_editXMoving;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPositionHoldingFokkerPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPositionHoldingFokkerPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSITIONHOLDINGFOKKERPAGE_H__F021320A_A1CB_4FC7_B408_C1142288E460__INCLUDED_)
