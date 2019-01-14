#if !defined(AFX_JOGPAGE_H__809B094C_476F_41D6_9D22_1C563DF0E3A4__INCLUDED_)
#define AFX_JOGPAGE_H__809B094C_476F_41D6_9D22_1C563DF0E3A4__INCLUDED_

#include "Coord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JogPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJogPage dialog

class CJogPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CJogPage)

// Construction
public:
	void		SetToolBarCheckedState();
	void OnButtonLeftWheel();
	void OnButtonRightWheel();
	void OnButtonBothWheels();
	CToolBar	m_wndToolBar;
	CReBar		m_wndRebar;
	HBRUSH m_hBr;
	int m_nAntiDriftStoreTime;
	CCoord m_CpAntiDrift;
	void GoniometricMove(char cVec,float fDist);
	int	m_nMainTip;
	int	m_nMode;
	int	m_nSide;
	int	m_nID;
	
	bool m_bLBDown;
	void UpdateAllControls();
	CJogPage(int nID = IDD);
	~CJogPage();

// Dialog Data
	//{{AFX_DATA(CJogPage)
	enum { IDD = IDD_JOG_PAGE };
	CEdit	m_editMinimumWaterPath;
	CComboBox	m_comboMainTips;
	CSpinButtonCtrl	m_spinZJogDistance;
	CEdit	m_editZJogDistance;
	CComboBox	m_comboSide;
	CSpinButtonCtrl	m_spinYJogDistance;
	CSpinButtonCtrl	m_spinXJogDistance;
	CEdit	m_editYJogDistance;
	CEdit	m_editXJogDistance;
	CComboBox	m_comboJogMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CJogPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CJogPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboSide();
	afx_msg void OnSelchangeComboJogMode();
	afx_msg void OnChangeEditZJogDistance();
	afx_msg void OnSelchangeComboMainTips();
	afx_msg void OnChangeEditYJogDistance();
	afx_msg void OnChangeEditXJogDistance();
	afx_msg void OnChangeEditMinimumWaterPath();
	afx_msg void OnButtonINeg();
	afx_msg void OnButtonIPos();
	afx_msg void OnButtonJNeg();
	afx_msg void OnButtonJPos();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOGPAGE_H__809B094C_476F_41D6_9D22_1C563DF0E3A4__INCLUDED_)
