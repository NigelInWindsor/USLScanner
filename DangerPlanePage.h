#if !defined(AFX_DANGERPLANEPAGE_H__C5696ED7_82DC_4266_8FAA_1670B2D2493C__INCLUDED_)
#define AFX_DANGERPLANEPAGE_H__C5696ED7_82DC_4266_8FAA_1670B2D2493C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DangerPlanePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDangerPlanePage dialog

class CDangerPlanePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDangerPlanePage)

// Construction
public:
	void Invalidate3DView();
	
	void UpdateAllControls();
	CPropertyPage* m_pPage;
	CPropertySheet *m_pSheet;
	CDangerPlanePage(CPropertySheet* pToolSheet = NULL, CPropertyPage* pViewPage = NULL);
	~CDangerPlanePage();

// Dialog Data
	//{{AFX_DATA(CDangerPlanePage)
	enum { IDD = IDD_DANGER_PLANE_PAGE };
	CSpinButtonCtrl	m_spinClearance1;
	CSpinButtonCtrl	m_spinClearance0;
	CEdit	m_editClearance1;
	CEdit	m_editClearance0;
	CButton	m_static1;
	CButton	m_static0;
	CComboBox	m_comboVecToDangerPlane1;
	CComboBox	m_comboVecToDangerPlane0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDangerPlanePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDangerPlanePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboVecToDangerplane();
	afx_msg void OnSelchangeComboVecToDangerplane2();
	afx_msg void OnChangeEditClearance0();
	afx_msg void OnChangeEditClearance1();
	afx_msg void OnDeltaposSpinClearance0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinClearance1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DANGERPLANEPAGE_H__C5696ED7_82DC_4266_8FAA_1670B2D2493C__INCLUDED_)
