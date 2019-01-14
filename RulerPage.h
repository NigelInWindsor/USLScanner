#if !defined(AFX_RULERPAGE_H__8738FD9F_4654_41F2_A072_5CFEE34386DA__INCLUDED_)
#define AFX_RULERPAGE_H__8738FD9F_4654_41F2_A072_5CFEE34386DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RulerPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRulerPage dialog

class CRulerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRulerPage)

// Construction
public:
	HBRUSH m_hBr;
	
	void UpdateAllControls();
	int	m_nParentType;
	CWnd* m_pParent;
	CRulerPage(CWnd* pParent=NULL, int nParentType = 0);
	~CRulerPage();
	bool m_bScanUsesRotateAxis;

// Dialog Data
	//{{AFX_DATA(CRulerPage)
	enum { IDD = IDD_RULER_PAGE };
	CComboBox	m_comboFlagMode;
	CComboBox	m_comboUnits;
	CEdit	m_editScale;
	CStatic	m_staticWidth3;
	CStatic	m_staticStyleEllipse;
	CStatic	m_staticStyleBox;
	CStatic	m_staticStyleLine;
	CListBox	m_listRulerStyle;
	CListBox	m_listRulerText;
	CStatic	m_staticWidth2;
	CStatic	m_staticWidth1;
	CStatic	m_staticWidth0;
	CStatic	m_staticColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRulerPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRulerPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeListRulerText();
	afx_msg void OnChangeEditScale();
	afx_msg void OnSelchangeComboUnits();
	afx_msg void OnSelchangeComboFlagMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RULERPAGE_H__8738FD9F_4654_41F2_A072_5CFEE34386DA__INCLUDED_)
