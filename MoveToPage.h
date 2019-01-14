#if !defined(AFX_MOVETOPAGE_H__A094DC39_BF29_40A1_B69C_F832EDAAF7D2__INCLUDED_)
#define AFX_MOVETOPAGE_H__A094DC39_BF29_40A1_B69C_F832EDAAF7D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoveToPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMoveToPage dialog

class CMoveToPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMoveToPage)

// Construction
public:
	HBRUSH		m_hBr;
	void		CalculateVelocityAndTemperature(int nCalcVelTemp);
	BOOL		OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	void		SetToolBarCheckedState();
	CToolBar	m_wndTipsToolBar;
	CToolBar	m_wndMoveToToolBar;
	CReBar		m_wndRebar;
	void	ApplyNonRobot();
	void	ApplyRobot();
	void SetAccessPrivelage();
	
	int m_nSide;
	int	m_nIndex;
	void UpdateAllControls();
	CMoveToPage();
	~CMoveToPage();

// Dialog Data
	//{{AFX_DATA(CMoveToPage)
	enum { IDD = IDD_MOVETO_PAGE };
	CSpinButtonCtrl	m_spinMaterialVelocity;
	CComboBox	m_comboMaterialVelocity;
	CSpinButtonCtrl	m_spinWaterTempVelocity;
	CEdit	m_editWaterTempVelocity;
	CEdit	m_editThickness;
	CEdit	m_editMaterialVelocity;
	CEdit	m_editWaterVelocity;
	CComboBox	m_comboTimeSlot;
	CComboBox	m_comboSide;
	CEdit	m_editDistance;
	CEdit	m_editRightPos;
	CEdit	m_editLeftPos;
	CButton	m_checkBoth;
	CButton	m_checkRight;
	CButton	m_checkLeft;
	CListBox	m_listMoveTo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMoveToPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMoveToPage)
	afx_msg void OnButtonApply();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnCheckBoth();
	afx_msg void OnCheckLeft();
	afx_msg void OnCheckRight();
	afx_msg void OnButtonStore();
	afx_msg void OnChangeEditLeftPos();
	afx_msg void OnChangeEditRightPos();
	afx_msg void OnButtonWpApply();
	afx_msg void OnSelchangeComboSide();
	afx_msg void OnSelchangeComboTs();
	afx_msg void OnChangeEditDistance();
	afx_msg void OnChangeEditWaterVelocity();
	afx_msg void OnButtonMaterialVelocity();
	afx_msg void OnChangeEditMaterialVelocity();
	afx_msg void OnChangeEditThickness();
	afx_msg void OnSelchangeListMoveto();
	afx_msg void OnDeltaposSpinWTempVel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeltaposSpinMaterialVelocity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditWTempVel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVETOPAGE_H__A094DC39_BF29_40A1_B69C_F832EDAAF7D2__INCLUDED_)
