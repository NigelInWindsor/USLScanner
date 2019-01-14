#if !defined(AFX_COLOURSEDITDLG_H__70C42DDD_2295_4859_8205_A603DA26D993__INCLUDED_)
#define AFX_COLOURSEDITDLG_H__70C42DDD_2295_4859_8205_A603DA26D993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColoursEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColoursEditDlg dialog

class CColoursEditDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CColoursEditDlg)

// Construction
public:
	void RenderParent();
	CPropertySheet* m_pToolSheet;
	CPropertyPage* m_pViewPage;
	CColoursEditDlg(CPropertySheet* pToolSheet = NULL, CPropertyPage* pViewPage = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColoursEditDlg)
	enum { IDD = IDD_EDITCOLORS_DIALOG };
	CStatic	m_staticOutsideColor;
	CStatic	m_staticInsideColor;
	CStatic	m_staticVerticesColor;
	CStatic	m_staticRoomWallColor;
	CStatic	m_staticTaughtColor;
	CStatic	m_staticScanColor;
	CStatic	m_staticEnvelopeColor;
	CStatic	m_staticBrainColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColoursEditDlg)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColoursEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOURSEDITDLG_H__70C42DDD_2295_4859_8205_A603DA26D993__INCLUDED_)
