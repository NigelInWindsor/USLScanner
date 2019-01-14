#if !defined(AFX_LINEPROPERTIESDLG_H__4B1919D7_BF06_4EC4_A5C4_92D2AD7AA185__INCLUDED_)
#define AFX_LINEPROPERTIESDLG_H__4B1919D7_BF06_4EC4_A5C4_92D2AD7AA185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinePropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLinePropertiesDlg dialog

class CLinePropertiesDlg : public CDialog
{
// Construction
public:
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void UpdateAllControls();
	
	FeatureStruct* m_pFeature;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CLinePropertiesDlg(CWnd* pParent=NULL,CWnd** pDlg=NULL, FeatureStruct* pFeature=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLinePropertiesDlg)
	enum { IDD = IDD_LINE_PROPERTIES_DIALOG };
	CComboBox	m_comboLineStyle;
	CStatic	m_staticColor;
	CSpinButtonCtrl	m_spinLineWidth;
	CEdit	m_editLineWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinePropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLinePropertiesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeComboLineStyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEPROPERTIESDLG_H__4B1919D7_BF06_4EC4_A5C4_92D2AD7AA185__INCLUDED_)
