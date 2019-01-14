#if !defined(AFX_REPORTTABLEPROPERTIESDLG_H__1DACFE39_BBA7_40B6_9235_E8732FA2F039__INCLUDED_)
#define AFX_REPORTTABLEPROPERTIESDLG_H__1DACFE39_BBA7_40B6_9235_E8732FA2F039__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportTablePropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportTablePropertiesDlg dialog

class CReportTablePropertiesDlg : public CDialog
{
// Construction
public:
	int m_nColumn;
	int m_nRow;
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void UpdateParent();
	void UpdateAllControls();
	
	FeatureStruct* m_pFeature;
	CData* m_pData;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CReportTablePropertiesDlg(CWnd* pParent = NULL,CWnd** pDlg=NULL, FeatureStruct* pFeature=NULL,CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReportTablePropertiesDlg)
	enum { IDD = IDD_REPORT_TABLE_PROPERTIES_DLG };
	CComboBox	m_comboJustify;
	CButton	m_checkBothLanguagesOneColumn;
	CComboBox	m_comboIDItem;
	CEdit	m_editRowColumn;
	CEdit	m_editTitle;
	CStatic	m_staticTitleText;
	CStatic	m_staticTitleBk;
	CSpinButtonCtrl	m_spinNumberRows;
	CSpinButtonCtrl	m_spinNumberColumns;
	CButton	m_checkTitleBar;
	CEdit	m_editNumberColumns;
	CEdit	m_editNumberRows;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportTablePropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportTablePropertiesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditColumns();
	afx_msg void OnChangeEditRows();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckTitleBar();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnChangeEditTitle();
	afx_msg void OnChangeEditRowColumn();
	afx_msg void OnSelchangeComboIdItem();
	afx_msg void OnButtonAutomatic();
	afx_msg void OnCheckBothLanguages();
	afx_msg void OnSelchangeComboJustify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTTABLEPROPERTIESDLG_H__1DACFE39_BBA7_40B6_9235_E8732FA2F039__INCLUDED_)
