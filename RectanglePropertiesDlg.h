#if !defined(AFX_RECTANGLEPROPERTIESDLG_H__3FBEE45D_9F8F_4B6B_B9EF_3E992936CC61__INCLUDED_)
#define AFX_RECTANGLEPROPERTIESDLG_H__3FBEE45D_9F8F_4B6B_B9EF_3E992936CC61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RectanglePropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRectanglePropertiesDlg dialog

class CRectanglePropertiesDlg : public CDialog
{
// Construction
public:
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void UpdateAllControls();
	
	FeatureStruct* m_pFeature;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CRectanglePropertiesDlg(CWnd* pParent = NULL,CWnd** pDlg=NULL, FeatureStruct* pFeature=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRectanglePropertiesDlg)
	enum { IDD = IDD_RECTANGLE_PROPERTIES_DIALOG };
	CComboBox	m_comboBorder;
	CStatic	m_staticColor;
	CSpinButtonCtrl	m_spinLineWidth;
	CEdit	m_editLineWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRectanglePropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRectanglePropertiesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeComboLineStyle();
	afx_msg void OnSelchangeComboBorder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECTANGLEPROPERTIESDLG_H__3FBEE45D_9F8F_4B6B_B9EF_3E992936CC61__INCLUDED_)
