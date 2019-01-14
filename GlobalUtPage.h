#if !defined(AFX_GLOBALUTPAGE_H__53C88D72_D1CD_407F_81F8_AB092FF09E09__INCLUDED_)
#define AFX_GLOBALUTPAGE_H__53C88D72_D1CD_407F_81F8_AB092FF09E09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GlobalUtPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGlobalUtPage dialog

class CGlobalUtPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGlobalUtPage)

// Construction
public:
	
	CPropertySheet* m_pSheet;
	void UpdateAllControls();
	void UpdateRelatedPages();
	CGlobalUtPage(CPropertySheet* pSheet = NULL);
	~CGlobalUtPage();

// Dialog Data
	//{{AFX_DATA(CGlobalUtPage)
	enum { IDD = IDD_GLOBAL_UT_DIALOG };
	CComboBox	m_comboTimeSlots;
	CComboBox	m_comboChannel2;
	CComboBox	m_comboHtSource;
	CComboBox	m_comboTx;
	CComboBox	m_comboRx;
	CProgressCtrl	m_progressAmplitude;
	CButton	m_checkUseAnaloguePort;
	CComboBox	m_comboProcess;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGlobalUtPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGlobalUtPage)
	afx_msg void OnSelchangeComboProcess();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCollectAnalogue();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboRx();
	afx_msg void OnSelchangeComboTx();
	afx_msg void OnSelchangeComboHtSource();
	afx_msg void OnSelchangeComboChannel2();
	afx_msg void OnSelchangeComboTimeslots();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBALUTPAGE_H__53C88D72_D1CD_407F_81F8_AB092FF09E09__INCLUDED_)
