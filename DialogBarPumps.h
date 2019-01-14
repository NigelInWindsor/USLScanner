#if !defined(AFX_DIALOGBARPUMPS_H__D8DDBDBE_DC6B_4BFD_A58D_03D52BB8C942__INCLUDED_)
#define AFX_DIALOGBARPUMPS_H__D8DDBDBE_DC6B_4BFD_A58D_03D52BB8C942__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogBarPumps.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDialogBarPumps dialog

class CDialogBarPumps : public CPaneDialog
{
// Construction
public:
	CDialogBarPumps();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogBarPumps)
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBarPumps)
	public:
	virtual BOOL Create(CWnd* pParent, UINT nIDTemplate, UINT nStyle, UINT nID);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogBarPumps)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBARPUMPS_H__D8DDBDBE_DC6B_4BFD_A58D_03D52BB8C942__INCLUDED_)
