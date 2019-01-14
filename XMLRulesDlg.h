#if !defined(AFX_XMLRULESDLG_H__2544CC2F_6E6D_4302_8C4F_A1C294DE0166__INCLUDED_)
#define AFX_XMLRULESDLG_H__2544CC2F_6E6D_4302_8C4F_A1C294DE0166__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMLRulesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMLRulesDlg dialog

class CXMLRulesDlg : public CDialog
{
// Construction
public:
	void UpdateAllControls();
	int m_nImageType;
	int m_nImageNumber;
	CData* m_pData;
	CWnd** m_pDlg;
	CXMLRulesDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,CData* pData = NULL,int nImageType = 0,int nImageNumber = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXMLRulesDlg)
	enum { IDD = IDD_XML_RULES_DIALOG };
	CButton	m_checkTwist;
	CComboBox	m_comboCoordinateFrame;
	CComboBox	m_comboTOFRule;
	CEdit	m_editOrigional;
	CEdit	m_editPrompt;
	CButton	m_checkHeader;
	CButton	m_checkData;
	CButton	m_checkCoordinates;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLRulesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMLRulesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCoordinates();
	afx_msg void OnCheckData();
	afx_msg void OnCheckHeader();
	afx_msg void OnSelchangeComboTofRule();
	afx_msg void OnCheckTwist();
	afx_msg void OnSelchangeComboCoordinateFrame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLRULESDLG_H__2544CC2F_6E6D_4302_8C4F_A1C294DE0166__INCLUDED_)
