#if !defined(AFX_DETAILSEDITDLG_H__72EB27A2_342B_4011_B57D_AA888E6F80BB__INCLUDED_)
#define AFX_DETAILSEDITDLG_H__72EB27A2_342B_4011_B57D_AA888E6F80BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetailsEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetailsEditDlg dialog

class CDetailsEditDlg : public CDialog
{
// Construction
public:
	void UpdateMainFrameTools();
	void UpdateParent(int nAction);
	void UpdateAllControls();
	CWnd* m_pParent;
	CWnd** m_pDlg;
	
	int		m_nLeft;
	int		m_nTop;
	CDetailsEditDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nLeft=0,int nTop=0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetailsEditDlg)
	enum { IDD = IDD_EDITDETAILS_DIALOG };
	CEdit	m_editUtFile;
	CComboBox	m_comboDescription;
	CSpinButtonCtrl	m_spinSlowIncrement;
	CSpinButtonCtrl	m_spinFastIncrement;
	CEdit	m_editSlowIncrement;
	CEdit	m_editFastIncrement;
	CEdit	m_editDescription;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetailsEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetailsEditDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditDescription();
	afx_msg void OnChangeEditFasyIncrement();
	afx_msg void OnChangeEditSlowIncrement();
	afx_msg void OnSelchangeComboManipulatorType();
	afx_msg void OnDeltaposSpinFastIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboDescription();
	afx_msg void OnButtonBrowse();
	afx_msg void OnChangeEditUtFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETAILSEDITDLG_H__72EB27A2_342B_4011_B57D_AA888E6F80BB__INCLUDED_)
