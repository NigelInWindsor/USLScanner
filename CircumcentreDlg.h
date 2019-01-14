#if !defined(AFX_CIRCUMCENTREDLG_H__B185F27F_0E98_4092_9F45_9FD97542BA80__INCLUDED_)
#define AFX_CIRCUMCENTREDLG_H__B185F27F_0E98_4092_9F45_9FD97542BA80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CircumcentreDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCircumcentreDlg dialog

class CCircumcentreDlg : public CDialog
{
// Construction
public:
	int m_nMode;
	int m_nNumberPts;
	float m_fRadius;
	void InvalidateParent();
	CPolyCoord m_InitLine;
	CPolyCoord m_ProcessedLine;
	void UpdateAllControls();
	CWnd** m_pDlg;
	CWnd* m_pParent;
	
	CCircumcentreDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCircumcentreDlg)
	enum { IDD = IDD_CIRCUMCENTRE_DLG };
	CComboBox	m_comboMode;
	CSpinButtonCtrl	m_spinRadius;
	CSpinButtonCtrl	m_spinExtraPoints;
	CEdit	m_editRadius;
	CEdit	m_editExtraPoints;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCircumcentreDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCircumcentreDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnChangeEditExtraPoints();
	afx_msg void OnChangeEditRadius();
	afx_msg void OnDeltaposSpinRadius(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIRCUMCENTREDLG_H__B185F27F_0E98_4092_9F45_9FD97542BA80__INCLUDED_)
