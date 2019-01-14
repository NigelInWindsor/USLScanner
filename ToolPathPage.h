#if !defined(AFX_TOOLPATHPAGE_H__ABE23117_273A_4D92_A20B_C21DCDF5FF28__INCLUDED_)
#define AFX_TOOLPATHPAGE_H__ABE23117_273A_4D92_A20B_C21DCDF5FF28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolPathPage.h : header file
//
#include "ResizablePage.h"
#include "PolyCoord.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CToolPathPage dialog

class CToolPathPage : public CResizablePage
{
	DECLARE_DYNCREATE(CToolPathPage)

// Construction
public:
	void Cross(CDC *pDC, int nX, int nY,int nSize);
	void DrawPlots(CDC *pDC, CRect rr);
	COLORREF	*m_pArray;
	CPolyCoord m_Line;
	void FetchToolPath();
	CRect m_rrGrat;
	int	m_nSample;
	
	bool m_bLButtonDown;
	CToolPathPage();
	~CToolPathPage();

// Dialog Data
	//{{AFX_DATA(CToolPathPage)
	enum { IDD = IDD_TOOLPATH_PAGE };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CToolPathPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CToolPathPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLPATHPAGE_H__ABE23117_273A_4D92_A20B_C21DCDF5FF28__INCLUDED_)
