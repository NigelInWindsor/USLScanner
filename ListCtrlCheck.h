#if !defined(AFX_LISTCTRLCHECK_H__AB6ED04A_8882_4009_ACB4_F5AE0F52BE82__INCLUDED_)
#define AFX_LISTCTRLCHECK_H__AB6ED04A_8882_4009_ACB4_F5AE0F52BE82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlCheck.h : header file
//
#include "HeaderCtrlCheck.h"

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCheck window

class CListCtrlCheck : public CListCtrl
{
// Construction
public:
	CWnd* m_pParent;
	CListCtrlCheck();

// Operations
private:
	BOOL	m_blInited;
	CImageList	m_checkImgList;


public:
	BOOL Init();

protected:
	CHeaderCtrlCheck	m_checkHeadCtrl;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlCheck)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlCheck();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlCheck)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLCHECK_H__AB6ED04A_8882_4009_ACB4_F5AE0F52BE82__INCLUDED_)
