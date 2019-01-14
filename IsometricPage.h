#if !defined(AFX_ISOMETRICPAGE_H__1374A1E5_1467_405C_A974_079A12A2FFDE__INCLUDED_)
#define AFX_ISOMETRICPAGE_H__1374A1E5_1467_405C_A974_079A12A2FFDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsometricPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIsometricPage dialog

class CIsometricPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CIsometricPage)

// Construction
public:
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	
	int m_nNumberImages;
	CData* m_pData;
	CWnd* m_p3DViewEditDlg;
	CWnd* m_pParent;
	void UpdateAllControls();
	CIsometricPage(CWnd* pParent = NULL,CWnd* p3DViewEditDlg = NULL);
	~CIsometricPage();

// Dialog Data
	//{{AFX_DATA(CIsometricPage)
	enum { IDD = IDD_ISOMETRIC_PAGE };
	CSpinButtonCtrl	m_spinVerticalExaggeration;
	CEdit	m_editVerticalExaggeration;
	CEdit	m_editDecimateSamples;
	CEdit	m_editDecimateLines;
	CComboBox	m_comboDensity;
	CStatic	m_staticMeshColor;
	CButton	m_checkMesh;
	CComboBox	m_comboImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CIsometricPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CIsometricPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboImage();
	afx_msg void OnCheckMesh();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboDensity();
	afx_msg void OnChangeEditDecimateLines();
	afx_msg void OnChangeEditDecimateSamples();
	afx_msg void OnChangeEditVertical();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOMETRICPAGE_H__1374A1E5_1467_405C_A974_079A12A2FFDE__INCLUDED_)
