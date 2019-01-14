//{{AFX_INCLUDES()
#include "uslbar.h"
//}}AFX_INCLUDES
#if !defined(AFX_EXTTRIGSCAN_H__ACBB50B0_E3BD_43BA_A36A_F8410369237A__INCLUDED_)
#define AFX_EXTTRIGSCAN_H__ACBB50B0_E3BD_43BA_A36A_F8410369237A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtTrigScan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExtTrigScan dialog

class CExtTrigScan : public CPropertyPage
{
	DECLARE_DYNCREATE(CExtTrigScan)

// Construction
public:
	int m_nBytesPixel;
	void PresentRenderdView(CPaintDC* pDC);
	CRect m_rrView;
	char* m_pArrayView;
	void InitializeViewVariables(CPaintDC* pDC,CRect* prr);
	
	void UpdateAllControls();
	CExtTrigScan();
	~CExtTrigScan();

// Dialog Data
	//{{AFX_DATA(CExtTrigScan)
	enum { IDD = IDD_EXT_TRIG_SCAN };
	CStatic	m_staticView;
	CComboBox	m_comboTrigSource;
	CUSLBar	m_scrollbarDelayWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CExtTrigScan)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExtTrigScan)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboTrigSource();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnScrollHasMovedUslbarctrl(long LowPosition, long HighPosition, long Width);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTTRIGSCAN_H__ACBB50B0_E3BD_43BA_A36A_F8410369237A__INCLUDED_)
