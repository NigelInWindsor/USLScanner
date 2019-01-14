#if !defined(AFX_LEADSCREWCOMPENSATIONPAGE_H__44AECD4A_1ED9_43B2_8F06_068963DC468C__INCLUDED_)
#define AFX_LEADSCREWCOMPENSATIONPAGE_H__44AECD4A_1ED9_43B2_8F06_068963DC468C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeadScrewCompensationPage.h : header file
//
#include "EditSpinItem.h"
/////////////////////////////////////////////////////////////////////////////
// CLeadScrewCompensationPage dialog

class CLeadScrewCompensationPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLeadScrewCompensationPage)

// Construction
public:
	void Goto(float fPos);
	void GenerateCompensationTable(int nAxis, CString *pBuff,bool bVerbose);
	void CloseEditSpin();
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	CEditSpinItem* m_pEditSpinItem;
	
	bool	m_bTimerActive;
	int		m_nAxis;
	int		m_nIndex;
	int		m_nCompensationOffOn;
	int		m_nOldPos;
	float	m_fStepSize;
	float	m_fRelativeMoveDistance;
	float	m_fPos[3][16];
	HBRUSH	m_hBr;
	CLeadScrewCompensationPage();
	~CLeadScrewCompensationPage();

// Dialog Data
	//{{AFX_DATA(CLeadScrewCompensationPage)
	enum { IDD = IDD_LEADSCREW_PAGE };
	CComboBox	m_comboLaserAxis;
	CEdit	m_editPos2;
	CEdit	m_editPos1;
	CEdit	m_editPos0;
	CEdit	m_editEncoderPosition;
	CEdit	m_editCurrentPos;
	CComboBox	m_comboOffOn;
	CListCtrl	m_listCoords;
	CEdit	m_editDownLoad;
	CEdit	m_editRelativeDistance;
	CEdit	m_editStepSize;
	CComboBox	m_comboCompensationAxis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLeadScrewCompensationPage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLeadScrewCompensationPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboAxisNumber();
	afx_msg void OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLeadscrewAdd();
	afx_msg void OnLeadscrewDelete();
	afx_msg void OnLeadscrewDeleteall();
	afx_msg void OnLeadscrewGoto();
	afx_msg void OnSelchangeComboOffOn();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg bool OnButtonDownload();
	afx_msg
	afx_msg	HRESULT ListChanged(WPARAM, LPARAM);
	void OnButtonStepForward();
	afx_msg void OnButtonStepBack();
	afx_msg void OnLeadscrewSort();
	afx_msg void OnLeadscrewCopyveve();
	afx_msg void OnLeadscrewCopyvetove();
	afx_msg void OnChangeEditStepsize();
	afx_msg void OnButtonSave();
	afx_msg void OnLeadscrewCopymastertoveve();
	afx_msg void OnLeadscrewGotocompensationmaster();
	afx_msg void OnChangeEditPos0();
	afx_msg void OnChangeEditPos1();
	afx_msg void OnChangeEditPos2();
	afx_msg void OnButtonGoto0();
	afx_msg void OnButtonGoto1();
	afx_msg void OnButtonGoto2();
	afx_msg void OnButtonStore();
	afx_msg void OnLeadscrewModifywithrespecttomaster();
	afx_msg void OnLeadscrewFilltable();
	afx_msg void OnButtonMakeItSo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEADSCREWCOMPENSATIONPAGE_H__44AECD4A_1ED9_43B2_8F06_068963DC468C__INCLUDED_)
