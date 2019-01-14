#if !defined(AFX_TURNTABLEPAGE_H__E0B5E182_7D00_4854_AA90_8A7B2964F845__INCLUDED_)
#define AFX_TURNTABLEPAGE_H__E0B5E182_7D00_4854_AA90_8A7B2964F845__INCLUDED_

#include "VertexArray.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TurnTablePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTurnTablePage dialog

class CTurnTablePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTurnTablePage)

// Construction
public:
	HBRUSH m_hBr;
	CCoord m_CpSurface0;
	CCoord m_CpSurface1;
	void UpdateCircumcentre();
	CVertexArray m_Circumcentre;
	void UpdateAllControls();
	
	CTurnTablePage();
	~CTurnTablePage();

// Dialog Data
	//{{AFX_DATA(CTurnTablePage)
	enum { IDD = IDD_TURNTABLE_PAGE };
	CEdit	m_editStore1;
	CEdit	m_editStore0;
	CEdit	m_editCircumcentre;
	CEdit	m_editCC2;
	CEdit	m_editCC1;
	CEdit	m_editCC0;
	CButton	m_buttonCC2;
	CButton	m_buttonCC1;
	CButton	m_buttonCC0;
	CEdit	m_editRight;
	CEdit	m_editLeft;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTurnTablePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTurnTablePage)
	afx_msg void OnChangeEditCc0();
	afx_msg void OnChangeEditCc1();
	afx_msg void OnChangeEditCc2();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnButtonCc0();
	afx_msg void OnButtonCc1();
	afx_msg void OnButtonCc2();
	afx_msg void OnButtonStore();
	afx_msg void OnButtonStore0();
	afx_msg void OnButtonStore1();
	afx_msg void OnButtonGoto0();
	afx_msg void OnButtonGoto1();
	afx_msg void OnButtonGotoCenter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TURNTABLEPAGE_H__E0B5E182_7D00_4854_AA90_8A7B2964F845__INCLUDED_)
