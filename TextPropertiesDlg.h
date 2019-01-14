#if !defined(AFX_TEXTPROPERTIESDLG_H__7A4E3689_307D_44E9_8D3A_A6A114D1632E__INCLUDED_)
#define AFX_TEXTPROPERTIESDLG_H__7A4E3689_307D_44E9_8D3A_A6A114D1632E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextPropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextPropertiesDlg dialog

class CTextPropertiesDlg : public CDialog
{
// Construction
public:
	void UpdateAllControls();
	FeatureStruct* m_pFeature;
//	COLORREF m_rgbText;
//	LOGFONT		m_lf;
//	CString m_stringCaption;
	
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CTextPropertiesDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL, FeatureStruct* pFeature = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextPropertiesDlg)
	enum { IDD = IDD_TEXT_PROPERTIES_DIALOG };
	CEdit	m_editCaption;
	CEdit	m_editFont;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextPropertiesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFont();
	afx_msg void OnChangeEditCaption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPROPERTIESDLG_H__7A4E3689_307D_44E9_8D3A_A6A114D1632E__INCLUDED_)
