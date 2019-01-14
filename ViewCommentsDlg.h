#if !defined(AFX_VIEWCOMMENTSDLG_H__D014DC32_B783_45C7_B937_C666A529696D__INCLUDED_)
#define AFX_VIEWCOMMENTSDLG_H__D014DC32_B783_45C7_B937_C666A529696D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewCommentsDlg.h : header file
//
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCommentsDlg dialog

class CViewCommentsDlg : public CDialog
{
// Construction
public:
	CStaticPosManage m_StaticPosText;
	
	CWnd**	m_pDlg;
	CData*	m_pData;
	CViewCommentsDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL, CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewCommentsDlg)
	enum { IDD = IDD_VIEW_COMMENTS_DLG };
	CEdit	m_editText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewCommentsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewCommentsDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeEditText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCOMMENTSDLG_H__D014DC32_B783_45C7_B937_C666A529696D__INCLUDED_)
