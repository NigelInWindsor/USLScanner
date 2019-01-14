#if !defined(AFX_POSITIONSINGLEBRIDGEPAGE_H__5019CDE5_38BE_477E_BC42_BC5EBD8A5209__INCLUDED_)
#define AFX_POSITIONSINGLEBRIDGEPAGE_H__5019CDE5_38BE_477E_BC42_BC5EBD8A5209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PositionSingleBridgePage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CPositionSingleBridgePage dialog

class CPositionSingleBridgePage : public CResizablePage
{
	DECLARE_DYNCREATE(CPositionSingleBridgePage)

// Construction
public:
	CCoord m_CpOld;
	inline void MinMax(int *nn,int nMin,int nMax);
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	void AddAnchor(CWnd* pCtrl);
	
	CPositionSingleBridgePage();
	~CPositionSingleBridgePage();

// Dialog Data
	//{{AFX_DATA(CPositionSingleBridgePage)
	enum { IDD = IDD_POSITION_SINGLE_BRIDGE_PAGE };
	CComboBox	m_comboRotateMode;
	CStatic	m_staticR;
	CEdit	m_editPosR;
	CButton	m_buttonZeroZt;
	CButton	m_buttonZeroZ;
	CButton	m_buttonZeroY;
	CButton	m_buttonZeroXt;
	CButton	m_buttonZeroX;
	CComboBox	m_comboPositionMode;
	CButton	m_staticLeft;
	CStatic	m_staticZtLeft;
	CStatic	m_staticZLeft;
	CStatic	m_staticYLeft;
	CStatic	m_staticXtLeft;
	CStatic	m_staticXLeft;
	CEdit	m_editPosZtLeft;
	CEdit	m_editPosZLeft;
	CEdit	m_editPosYLeft;
	CEdit	m_editPosXtLeft;
	CEdit	m_editPosXLeft;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPositionSingleBridgePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPositionSingleBridgePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboPositionMode();
	afx_msg void OnButtonZerox();
	afx_msg void OnButtonZeroy();
	afx_msg void OnButtonZeroz();
	afx_msg void OnBUTTONZEROXt();
	afx_msg void OnBUTTONZEROZt();
	afx_msg void OnCheckRotateSurfacePos();
	afx_msg void OnSelchangeComboRotateMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSITIONSINGLEBRIDGEPAGE_H__5019CDE5_38BE_477E_BC42_BC5EBD8A5209__INCLUDED_)
