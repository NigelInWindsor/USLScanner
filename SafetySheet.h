#if !defined(AFX_SAFETYSHEET_H__6D7A49FB_3647_440A_9591_770F8D16AC09__INCLUDED_)
#define AFX_SAFETYSHEET_H__6D7A49FB_3647_440A_9591_770F8D16AC09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SafetySheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSafetySheet

class CSafetySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSafetySheet)

// Construction
public:
	CSafetySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSafetySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSafetySheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSafetySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSafetySheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAFETYSHEET_H__6D7A49FB_3647_440A_9591_770F8D16AC09__INCLUDED_)
