#if !defined(AFX_BUTTONDERIVED_H__EFBBC738_11EE_4FE6_977B_9B23D326A5DE__INCLUDED_)
#define AFX_BUTTONDERIVED_H__EFBBC738_11EE_4FE6_977B_9B23D326A5DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonDerived.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonDerived window

class CButtonDerived : public CButton
{
// Construction
public:
	CButtonDerived();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonDerived)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonDerived();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonDerived)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONDERIVED_H__EFBBC738_11EE_4FE6_977B_9B23D326A5DE__INCLUDED_)
