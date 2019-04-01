#include "afxwin.h"
#if !defined(AFX_HILSCHERPAGE_H__ECEC3938_6E12_4AF4_8228_8EA5CAD51640__INCLUDED_)
#define AFX_HILSCHERPAGE_H__ECEC3938_6E12_4AF4_8228_8EA5CAD51640__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HilscherPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHilscherPage dialog

class CHilscherPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CHilscherPage)

// Construction
public:
	int m_nDisplayRows;
	int m_nTopRow;
	int	m_nRowsL;
	void RenderView();
	void UpdateAllControls();
	
	CHilscherPage();
	~CHilscherPage();

// Dialog Data
	//{{AFX_DATA(CHilscherPage)
	enum { IDD = IDD_HILSCHER_PAGE };
	CEdit	m_editSyncShift1;
	CSpinButtonCtrl	m_spinSyncShift1;
	CEdit	m_editSyncShift2;
	CSpinButtonCtrl	m_spinSyncShift2;
	CEdit	m_editSyncShift3;
	CSpinButtonCtrl	m_spinSyncShift3;
	CEdit	m_editCallbackPeriod;
	CEdit	m_editCallbackCount;
	CComboBox	m_comboDisplayMode;
	CStatic	m_staticView;
	HBRUSH m_hBr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHilscherPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CHilscherPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeComboDisplayMode();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnHilscherClearall();
	afx_msg void OnChangeEditSyncShift1();
	afx_msg void OnDeltaposSpinSyncShift1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditSyncShift2();
	afx_msg void OnDeltaposSpinSyncShift2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditSyncShift3();
	afx_msg void OnDeltaposSpinSyncShift3(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_comboMode;
	afx_msg void OnCbnSelchangeComboMode();
	CButton m_buttonConnect;
	afx_msg void OnBnClickedButtonConnect();
	CEdit m_editSyncShift4;
	CSpinButtonCtrl m_spinSyncShift4;
	afx_msg void OnEnChangeEditSyncShift4();
	afx_msg void OnDeltaposSpinSyncShift4(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HILSCHERPAGE_H__ECEC3938_6E12_4AF4_8228_8EA5CAD51640__INCLUDED_)
