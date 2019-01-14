#if !defined(AFX_BLADETOLERANCEPAGE_H__CBDA710F_D9E8_47F8_A4EE_648ECC9C3BB8__INCLUDED_)
#define AFX_BLADETOLERANCEPAGE_H__CBDA710F_D9E8_47F8_A4EE_648ECC9C3BB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BladeTolerancePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBladeTolerancePage dialog

class CBladeTolerancePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBladeTolerancePage)

// Construction
public:
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void RefreshParent();
	
	CWnd* m_pParent;
	void UpdateAllControls();
	CBladeTolerancePage(CWnd* pParent = NULL);
	~CBladeTolerancePage();

// Dialog Data
	//{{AFX_DATA(CBladeTolerancePage)
	enum { IDD = IDD_BLADE_TOLERANCE_PAGE };
	CButton	m_checkAutoVerticalScale;
	CStatic	m_staticFontColor;
	CStatic	m_staticBkColor;
	CSpinButtonCtrl	m_spinFontSize;
	CEdit	m_editFontSize;
	CStatic	m_staticNom;
	CStatic	m_staticAct;
	CButton	m_checkDisplayNom;
	CStatic	m_staticMin;
	CStatic	m_staticMax;
	CSpinButtonCtrl	m_spinMin;
	CSpinButtonCtrl	m_spinMax;
	CEdit	m_editMin;
	CEdit	m_editMax;
	CButton	m_checkDisplayMin;
	CButton	m_checkDisplayMax;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBladeTolerancePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBladeTolerancePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditMaxTol();
	afx_msg void OnChangeEditMinTol();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnCheckDisplayMin();
	afx_msg void OnCheckDisplayMax();
	afx_msg void OnCheckDisplayNom();
	afx_msg void OnChangeEditFontSize();
	afx_msg void OnCheckAutoVerticalScale();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLADETOLERANCEPAGE_H__CBDA710F_D9E8_47F8_A4EE_648ECC9C3BB8__INCLUDED_)
