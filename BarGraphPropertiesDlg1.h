#if !defined(AFX_BARGRAPHPROPERTIESDLG_H__9E3656CE_647D_457F_A851_C9EE96D00620__INCLUDED_)
#define AFX_BARGRAPHPROPERTIESDLG_H__9E3656CE_647D_457F_A851_C9EE96D00620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarGraphPropertiesDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBarGraphPropertiesDlg dialog

class CBarGraphPropertiesDlg : public CDialog
{
// Construction
public:
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void UpdateAllControls();
	CData* m_pData;
	
	FeatureStruct* m_pFeature;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CBarGraphPropertiesDlg(CWnd* pParent = NULL,CWnd** pDlg=NULL, FeatureStruct* pFeature=NULL,CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBarGraphPropertiesDlg)
	enum { IDD = IDD_BARGRAPH_PROPERTIES_DIALOG21 };
	CComboBox	m_comboGate;
	CStatic	m_staticBorderColor;
	CSpinButtonCtrl	m_spinBorderWidth;
	CEdit	m_editBorderWidth;
	CComboBox	m_comboBorder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarGraphPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarGraphPropertiesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditBorderWidth();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboBorder();
	afx_msg void OnSelchangeComboGate();
	afx_msg void OnButtonGraticuleFont();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARGRAPHPROPERTIESDLG_H__9E3656CE_647D_457F_A851_C9EE96D00620__INCLUDED_)
