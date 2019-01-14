#if !defined(AFX_CADLINEPAGE_H__D65ACDD9_0EB8_4E20_AB25_443E8B01DEE3__INCLUDED_)
#define AFX_CADLINEPAGE_H__D65ACDD9_0EB8_4E20_AB25_443E8B01DEE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CADLinePage.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCADLinePage dialog

class CCADLinePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCADLinePage)

// Construction
public:
	void BlockFill();
	void Invalidate();
	CWnd* m_pViewSheet;
	
	void UpdateAllControls();
	CCADLinePage(CWnd* pWnd = NULL);
	~CCADLinePage();

// Dialog Data
	//{{AFX_DATA(CCADLinePage)
	enum { IDD = IDD_CAD_LINE_PAGE };
	CButton	m_checkLength;
	CStatic	m_staticFontSize;
	CStatic	m_staticFontName;
	CStatic	m_staticColor;
	CSpinButtonCtrl	m_spinWidth;
	CEdit	m_editWidth;
	CButton	m_checkShowLine;
	CButton	m_checkNode1ArrowOutside;
	CButton	m_checkNode0ArrowOutside;
	CButton	m_checkNode1ArrowInside;
	CButton	m_checkNode0ArrowInside;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCADLinePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCADLinePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckNode0Arrow();
	afx_msg void OnCheckNode1Arrow();
	afx_msg void OnCheckNode0ArrowOutside();
	afx_msg void OnCheckNode1ArrowOutside();
	afx_msg void OnCheckShow();
	afx_msg void OnChangeEditThickness();
	afx_msg void OnButtonFont();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnCheckLength();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CADLINEPAGE_H__D65ACDD9_0EB8_4E20_AB25_443E8B01DEE3__INCLUDED_)
