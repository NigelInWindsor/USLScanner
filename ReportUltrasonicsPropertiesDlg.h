#if !defined(AFX_REPORTULTRASONICSPROPERTIESDLG_H__51579324_7E75_425A_8790_D9E05037B85C__INCLUDED_)
#define AFX_REPORTULTRASONICSPROPERTIESDLG_H__51579324_7E75_425A_8790_D9E05037B85C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportUltrasonicsPropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportUltrasonicsPropertiesDlg dialog

class CReportUltrasonicsPropertiesDlg : public CDialog
{
// Construction
public:
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void UpdateParent();
	void UpdateAllControls();
	
	FeatureStruct* m_pFeature;
	CData* m_pData;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CReportUltrasonicsPropertiesDlg(CWnd* pParent = NULL,CWnd** pDlg=NULL, FeatureStruct* pFeature=NULL,CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReportUltrasonicsPropertiesDlg)
	enum { IDD = IDD_REPORT_ULTRASONICS_PROPERTIES_DLG };
	CComboBox	m_comboLanguage;
	CStatic	m_staticTitleBk;
	CStatic	m_staticTitleText;
	CButton	m_checkTitleBar;
	CEdit	m_editTitle;
	//}}AFX_DATA
	CCheckListBox	m_listUtVariables;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportUltrasonicsPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportUltrasonicsPropertiesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditTitle();
	afx_msg void OnCheckTitleBar();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnCheckChangeListUtVariables();
	afx_msg void OnSelchangeComboLanguage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTULTRASONICSPROPERTIESDLG_H__51579324_7E75_425A_8790_D9E05037B85C__INCLUDED_)
