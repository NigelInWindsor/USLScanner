#include "afxwin.h"
#if !defined(AFX_ORIGINPAGE_H__9F0E06BB_9E61_4E93_8F45_7293F6131186__INCLUDED_)
#define AFX_ORIGINPAGE_H__9F0E06BB_9E61_4E93_8F45_7293F6131186__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OriginPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COriginPage dialog

class COriginPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COriginPage)

// Construction
public:
	HBRUSH m_hBr;
	void UpdateAllControls();
	void ShowAndEnableWindow(CWnd* pWnd,bool bShowFlag,bool bEnableFlag,WCHAR *cName = NULL);
	void SetAccessPrivelage();
	
	COriginPage();
	~COriginPage();

// Dialog Data
	//{{AFX_DATA(COriginPage)
	enum { IDD = IDD_ORIGIN_PAGE };
	CButton	m_staticRight;
	CButton	m_staticLeft;
	CButton	m_buttonPark;
	CButton	m_buttonXBridge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COriginPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COriginPage)
	afx_msg void OnButtonXLeft();
	afx_msg void OnButtonYleft();
	afx_msg void OnButtonZleft();
	afx_msg void OnButtonXtleft();
	afx_msg void OnButtonYtleft();
	afx_msg void OnButtonXRight();
	afx_msg void OnButtonYright();
	afx_msg void OnButtonZright();
	afx_msg void OnButtonXtright();
	afx_msg void OnButtonYtright();
	afx_msg void OnButtonOriginall();
	afx_msg void OnButtonXmoving();
	afx_msg void OnButtonAllLeft();
	afx_msg void OnButtonAllRight();
	afx_msg void OnButtonHoldingZFixed();
	afx_msg void OnButtonHoldingYFixed();
	afx_msg void OnButtonHoldingX();
	afx_msg void OnButtonHoldingYMoving();
	afx_msg void OnButtonHoldingZMoving();
	afx_msg void OnButtonXBridge();
	afx_msg void OnButtonTurntable();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnButtonPark();
	afx_msg void OnButtonRLeft();
	afx_msg void OnButtonHomeFailed();
	afx_msg void OnButtonHoldingXFixed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CButton m_checkSave;
	CStatic m_staticSave;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORIGINPAGE_H__9F0E06BB_9E61_4E93_8F45_7293F6131186__INCLUDED_)
