#if !defined(AFX_VIEWVALUEPAGE_H__63F5EA7F_AECC_4EF0_95E7_DA594ACCE8B1__INCLUDED_)
#define AFX_VIEWVALUEPAGE_H__63F5EA7F_AECC_4EF0_95E7_DA594ACCE8B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewValuePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewValuePage dialog

class CViewValuePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CViewValuePage)

// Construction
public:
	HBRUSH m_hBr;
	void UpdateAllControls();
	
	CWnd* m_pViewSheet;
	CViewValuePage(CWnd* pViewSheet = NULL);
	~CViewValuePage();

// Dialog Data
	//{{AFX_DATA(CViewValuePage)
	enum { IDD = IDD_VIEW_VALUE_PAGE };
	CSpinButtonCtrl	m_spinLogRange;
	CEdit	m_editLogRange;
	CButton	m_checkPositionCross;
	CButton	m_checkTSName;
	CButton	m_checkGateName;
	CComboBox	m_comboDataType;
	CSpinButtonCtrl	m_spinSize;
	CEdit	m_editSize;
	CComboBox	m_comboAlgorithm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CViewValuePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CViewValuePage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnChangeEditSize();
	afx_msg void OnSelchangeComboAlgorithm();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboValueData();
	afx_msg void OnCheckTsName();
	afx_msg void OnCheckGateName();
	afx_msg void OnCheckPositionCross();
	afx_msg void OnChangeEditLogRange();
	afx_msg void OnDeltaposSpinLogRange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWVALUEPAGE_H__63F5EA7F_AECC_4EF0_95E7_DA594ACCE8B1__INCLUDED_)
