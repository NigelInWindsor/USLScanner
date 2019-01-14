#if !defined(AFX_REPORTHEADERFOOTERDLG_H__273330DE_AC6B_43CE_857E_81630491CCD4__INCLUDED_)
#define AFX_REPORTHEADERFOOTERDLG_H__273330DE_AC6B_43CE_857E_81630491CCD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportHeaderFooterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportHeaderFooterDlg dialog

class CReportHeaderFooterDlg : public CDialog
{
// Construction
public:
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void UpdateAllControls();
	
	CWnd** m_pDlg;
	CWnd* m_pParent;
	FeatureStruct* m_pFeature;
	CReportHeaderFooterDlg(CWnd* pParent=NULL,CWnd** pDlg=NULL,FeatureStruct* pFeature=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReportHeaderFooterDlg)
	enum { IDD = IDD_REPORT_HEADER_FOOTER_DLG };
	CStatic	m_staticHeaderColour;
	CStatic	m_staticFooterColour;
	CButton	m_checkHeader;
	CButton	m_checkFooter;
	CSpinButtonCtrl	m_spinHeaderHeight;
	CSpinButtonCtrl	m_spinFooterHeight;
	CEdit	m_editHeaderHeight;
	CEdit	m_editFooterHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportHeaderFooterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportHeaderFooterDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditFooterHeight();
	afx_msg void OnChangeEditHeaderHeight();
	afx_msg void OnDeltaposSpinFooterHeight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinHeaderHeight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckHeader();
	afx_msg void OnCheckFooter();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTHEADERFOOTERDLG_H__273330DE_AC6B_43CE_857E_81630491CCD4__INCLUDED_)
