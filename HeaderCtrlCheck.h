#if !defined(AFX_HEADERCTRLCHECK_H__64953D02_FE4B_4326_997D_75EE452CA2D8__INCLUDED_)
#define AFX_HEADERCTRLCHECK_H__64953D02_FE4B_4326_997D_75EE452CA2D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaderCtrlCheck.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlCheck window

class CHeaderCtrlCheck : public CHeaderCtrl
{
// Construction
public:
	CHeaderCtrlCheck();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaderCtrlCheck)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHeaderCtrlCheck();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeaderCtrlCheck)
	afx_msg void OnItemClicked(NMHDR* pNMHDR, LRESULT* pResult);
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADERCTRLCHECK_H__64953D02_FE4B_4326_997D_75EE452CA2D8__INCLUDED_)
