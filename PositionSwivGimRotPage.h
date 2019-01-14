#if !defined(AFX_POSITIONSWIVGIMROTPAGE_H__C7BA0291_ABEE_4D8E_9F98_81F386CC3F59__INCLUDED_)
#define AFX_POSITIONSWIVGIMROTPAGE_H__C7BA0291_ABEE_4D8E_9F98_81F386CC3F59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PositionSwivGimRotPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPositionSwivGimRotPage dialog
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CPositionPage dialog

class CPositionSwivGimRotPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPositionSwivGimRotPage)

public:
	void UpdateAllControls();
	int m_nOldMeasurementUnits;
	CCoord m_CpOld;
	CCoord m_CpAxesOld;
	inline void MinMax(int *nn,int nMin,int nMax);
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	void AddAnchor(CWnd* pCtrl);
	
	CPositionSwivGimRotPage();   // standard constructor
	~CPositionSwivGimRotPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPositionSwivGimRotPage)
	enum { IDD = IDD_POS_SWIV_GIM_ROT_PAGE };
	CButton	m_buttonAxis;
	CStatic	m_staticZ;
	CStatic	m_staticY;
	CStatic	m_staticX;
	CStatic	m_staticSwival;
	CStatic	m_staticR;
	CStatic	m_staticGimbal;
	CEdit	m_editAxis5;
	CEdit	m_editAxis4;
	CEdit	m_editAxis3;
	CEdit	m_editAxis2;
	CEdit	m_editAxis1;
	CEdit	m_editAxis0;
	CComboBox	m_comboPositionMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPositionSwivGimRotPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPositionSwivGimRotPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboPositionMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSITIONSWIVGIMROTPAGE_H__C7BA0291_ABEE_4D8E_9F98_81F386CC3F59__INCLUDED_)
