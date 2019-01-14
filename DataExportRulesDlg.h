#if !defined(AFX_DATAEXPORTRULESDLG_H__2730804D_2BBE_4BC5_BE11_0E158FB887F3__INCLUDED_)
#define AFX_DATAEXPORTRULESDLG_H__2730804D_2BBE_4BC5_BE11_0E158FB887F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataExportRulesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataExportRulesDlg dialog

class CDataExportRulesDlg : public CDialog
{
// Construction
public:
	void UpdateAllControls();
	CWnd **m_pDlg;
	CDataExportRulesDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataExportRulesDlg)
	enum { IDD = IDD_DATA_EXPORT_RULES_DLG };
	CButton	m_checkTwist;
	CComboBox	m_comboCoordinateFrame;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataExportRulesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataExportRulesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboCoordinateFrame();
	afx_msg void OnCheckTwist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAEXPORTRULESDLG_H__2730804D_2BBE_4BC5_BE11_0E158FB887F3__INCLUDED_)
