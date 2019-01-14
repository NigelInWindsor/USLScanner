#if !defined(AFX_GRATICULEPAGE_H__6F8DEAFC_8B8D_48D0_9CA6_0630E9BBF0FF__INCLUDED_)
#define AFX_GRATICULEPAGE_H__6F8DEAFC_8B8D_48D0_9CA6_0630E9BBF0FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraticulePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraticulePage dialog

class CGraticulePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGraticulePage)

// Construction
public:
	HBRUSH m_hBr;
	
	void UpdateAllControls();
	CWnd* m_pViewSheet;
	CGraticulePage(CWnd* pViewSheet = NULL);
	~CGraticulePage();

// Dialog Data
	//{{AFX_DATA(CGraticulePage)
	enum { IDD = IDD_GRATICULE_PAGE };
	CSpinButtonCtrl	m_spinSpacing;
	CButton	m_checkAutoSpacing;
	CEdit	m_editSpacing;
	CButton	m_checkRAxisGraticuleReverse;
	CComboBox	m_comboRaxisMode;
	CStatic	m_staticExample;
	CStatic	m_staticStyleColor;
	CStatic	m_static7;
	CStatic	m_static6;
	CStatic	m_static5;
	CStatic	m_static4;
	CStatic	m_static3;
	CStatic	m_static2;
	CStatic	m_static1;
	CStatic	m_static0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGraticulePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGraticulePage)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnButtonFont();
	afx_msg void OnSelchangeComboRaxisMode();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnCheckRAxisReverse();
	afx_msg void OnChangeEditSpacing();
	afx_msg void OnDeltaposSpinSpacing(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckSpacingAuto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRATICULEPAGE_H__6F8DEAFC_8B8D_48D0_9CA6_0630E9BBF0FF__INCLUDED_)
