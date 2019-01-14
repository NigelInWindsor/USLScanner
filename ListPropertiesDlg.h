#if !defined(AFX_LISTPROPERTIESDLG_H__F724B5C9_D4DF_45AB_9025_9CF44C11A86C__INCLUDED_)
#define AFX_LISTPROPERTIESDLG_H__F724B5C9_D4DF_45AB_9025_9CF44C11A86C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListPropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListPropertiesDlg dialog

class CListPropertiesDlg : public CDialog
{
// Construction
public:
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void UpdateAllControls();
	CData* m_pData;
	
	FeatureStruct* m_pFeature;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CListPropertiesDlg(CWnd* pParent=NULL,CWnd** pDlg=NULL, FeatureStruct* pFeature=NULL,CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CListPropertiesDlg)
	enum { IDD = IDD_LIST_PROPERTIES_DIALOG };
	CComboBox	m_comboLanguage;
	CStatic	m_staticBorderColor;
	CStatic	m_staticBkColor;
	CEdit	m_editCaption;
	CComboBox	m_comboListType;
	CSpinButtonCtrl	m_spinBorderWidth;
	CEdit	m_editBorderWidth;
	CComboBox	m_comboBorder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CListPropertiesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonFont();
	afx_msg void OnSelchangeComboWhichList();
	afx_msg void OnSelchangeComboBorder();
	afx_msg void OnChangeEditCaption();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeComboLanguage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTPROPERTIESDLG_H__F724B5C9_D4DF_45AB_9025_9CF44C11A86C__INCLUDED_)
