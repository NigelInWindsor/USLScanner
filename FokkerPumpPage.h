#if !defined(AFX_FOKKERPUMPPAGE_H__CF05ADDF_BA44_4791_8503_0F268E65CD52__INCLUDED_)
#define AFX_FOKKERPUMPPAGE_H__CF05ADDF_BA44_4791_8503_0F268E65CD52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FokkerPumpPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFokkerPumpPage dialog

class CFokkerPumpPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFokkerPumpPage)

	// Construction
public:
	HBRUSH	m_hBr;
	int m_nRight;
	int m_nLeft;
	int	m_nOldArray[1024];
	
	void UpdateAllControls();
	CFokkerPumpPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFokkerPumpPage)
	enum { IDD = IDD_FOKKER_PUMPS_PAGE };
	CComboBox	m_comboPumpMode1;
	CComboBox	m_comboPumpMode0;
	CEdit	m_editVerticalMultiplier1;
	CEdit	m_editVerticalMultiplier0;
	CButton	m_staticRight;
	CButton	m_staticLeft;
	CEdit	m_editVaneEncoder1;
	CEdit	m_editVaneEncoder0;
	CEdit	m_editPumpEncoder1;
	CEdit	m_editPumpEncoder0;
	CEdit	m_editScaleFactor1;
	CEdit	m_editScaleFactor0;
	CEdit	m_editRightPhysicalAxis;
	CEdit	m_editLeftPhysicalAxis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFokkerPumpPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFokkerPumpPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnButtonApply();
	afx_msg void OnChangeEditScaleFactor();
	afx_msg void OnChangeEditScaleFactor2();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditVerticalMultiplier0();
	afx_msg void OnChangeEditVerticalMultiplier1();
	afx_msg void OnSelchangeComboPump1Mode();
	afx_msg void OnSelchangeComboPump0Mode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOKKERPUMPPAGE_H__CF05ADDF_BA44_4791_8503_0F268E65CD52__INCLUDED_)
