#if !defined(AFX_STATICSCROLL_H__4E0CD245_8914_4D7A_96BA_32BBB3D1F304__INCLUDED_)
#define AFX_STATICSCROLL_H__4E0CD245_8914_4D7A_96BA_32BBB3D1F304__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticScroll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticScroll window

class CStaticScroll : public CStatic
{
// Construction
public:
	CStaticScroll();
	DECLARE_DYNCREATE(CStaticScroll)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticScroll)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticScroll();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticScroll)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICSCROLL_H__4E0CD245_8914_4D7A_96BA_32BBB3D1F304__INCLUDED_)
