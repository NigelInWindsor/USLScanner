#if !defined(AFX_3DBLADEPAGE_H__42E3CD82_BB57_4F18_8A47_4F529B9958E3__INCLUDED_)
#define AFX_3DBLADEPAGE_H__42E3CD82_BB57_4F18_8A47_4F529B9958E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DBladePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DBladePage dialog

class C3DBladePage : public CPropertyPage
{
	DECLARE_DYNCREATE(C3DBladePage)

// Construction
public:
	C3DBladePage();
	~C3DBladePage();

// Dialog Data
	//{{AFX_DATA(C3DBladePage)
	enum { IDD = IDD_3D_BLADE_PAGE };
	CEdit	m_edtitGash;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(C3DBladePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C3DBladePage)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChangeEdit1();
	afx_msg void OnErrspaceEdit1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DBLADEPAGE_H__42E3CD82_BB57_4F18_8A47_4F529B9958E3__INCLUDED_)
