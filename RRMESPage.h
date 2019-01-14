#if !defined(AFX_RRMESPAGE_H__73AD3133_7CC5_4074_AA02_20EC48442015__INCLUDED_)
#define AFX_RRMESPAGE_H__73AD3133_7CC5_4074_AA02_20EC48442015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RRMESPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRRMESPage dialog

class CRRMESPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRRMESPage)

// Construction
public:
	void UpdateAllControls();
	void GetDirectory(CEdit *pEdit, CString *pInitDir);
	
	CRRMESPage();
	~CRRMESPage();

// Dialog Data
	//{{AFX_DATA(CRRMESPage)
	enum { IDD = IDD_RR_MES_PAGE };
	CButton	m_buttonApply;
	CEdit	m_editResourceName;
	CTreeCtrl	m_treeGaugeToMES;
	CTreeCtrl	m_treeMEStoGauge;
	CComboBox	m_comboActive;
	CEdit	m_editControlPathToMes;
	CEdit	m_editAssociatedPathToMes;
	CEdit	m_editAssociatedPathToGauge;
	CEdit	m_editControlPathToGauge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRRMESPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRRMESPage)
	afx_msg void OnButtonCtrlToGauge();
	afx_msg void OnButtonAssociatedToGauge();
	afx_msg void OnButtonCtrlToMes();
	afx_msg void OnButtonAssociatedToMes();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApply();
	afx_msg void OnSelchangeComboActive();
	afx_msg void OnChangeEditName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RRMESPAGE_H__73AD3133_7CC5_4074_AA02_20EC48442015__INCLUDED_)
