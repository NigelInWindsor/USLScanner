#if !defined(AFX_OVERLAYPAGE_H__2C03A792_196A_4160_98F1_399A6B5C64D0__INCLUDED_)
#define AFX_OVERLAYPAGE_H__2C03A792_196A_4160_98F1_399A6B5C64D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COverlayPage dialog

class COverlayPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COverlayPage)

// Construction
public:
	void InvalidateView();
	void UpdateAllControls();
	int	m_nLineType;
	

	CData* m_pData;
	CWnd* m_pParent;
	COverlayPage(CWnd* pParent = NULL);
	~COverlayPage();

// Dialog Data
	//{{AFX_DATA(COverlayPage)
	enum { IDD = IDD_OVERLAY_PAGE };
	CComboBox	m_comboFlatten;
	CButton	m_checkDisplayCrosses;
	CStatic	m_staticColor;
	CComboBox	m_comboLineMode;
	CComboBox	m_comboLineType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COverlayPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COverlayPage)
	afx_msg void OnCheckDisplayCrosses();
	afx_msg void OnSelchangeComboLineType();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLineMode();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboFlatten();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYPAGE_H__2C03A792_196A_4160_98F1_399A6B5C64D0__INCLUDED_)
