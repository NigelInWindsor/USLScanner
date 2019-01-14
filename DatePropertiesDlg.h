#if !defined(AFX_DATEPROPERTIESDLG_H__512E3C8E_6E99_4C8B_AD63_4BFA6414AD21__INCLUDED_)
#define AFX_DATEPROPERTIESDLG_H__512E3C8E_6E99_4C8B_AD63_4BFA6414AD21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DatePropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDatePropertiesDlg dialog

class CDatePropertiesDlg : public CDialog
{
// Construction
public:
	void UpdateAllControls();
	
	CData* m_pData;
	FeatureStruct* m_pFeature;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CDatePropertiesDlg(CWnd* pParent = NULL,CWnd** pDlg=NULL,FeatureStruct* pFeature=NULL,CData* pData=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDatePropertiesDlg)
	enum { IDD = IDD_DATE_PROPERTIES_DIALOG };
	CEdit	m_editCaption;
	CButton	m_checkCaptionRight;
	CButton	m_checkCaptionLeft;
	CComboBox	m_comboWhichDate;
	CComboBox	m_comboDateFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDatePropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDatePropertiesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboDateFormat();
	afx_msg void OnSelchangeComboWhichDate();
	afx_msg void OnButtonFont();
	afx_msg void OnCheckCaptionLeft();
	afx_msg void OnCheckCaptionRight();
	afx_msg void OnChangeEditCaption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATEPROPERTIESDLG_H__512E3C8E_6E99_4C8B_AD63_4BFA6414AD21__INCLUDED_)
