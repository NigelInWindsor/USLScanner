#if !defined(AFX_PROFILESHEET_H__B7133A9F_58B3_46FF_88BB_7EF3E3B517F4__INCLUDED_)
#define AFX_PROFILESHEET_H__B7133A9F_58B3_46FF_88BB_7EF3E3B517F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfileSheet.h : header file
//
#include "ResizableSheet.h"
#include "3DViewEditDlg.h"
#include "3DEditTurnTablePage.h"
#include "3DDesignPage.h"
#include "3DViewPage.h"
#include "3DToolsSheet1.h"
#include "3DStiffenerPage.h"

/////////////////////////////////////////////////////////////////////////////
// CProfileSheet

class CProfileSheet : public CResizableSheet
{
	DECLARE_DYNAMIC(CProfileSheet)

// Construction
public:
	void		ToolsDrawingoptions();
	CToolBar	m_wndOrientationToolBar;
	CToolBar    m_wndArchiveToolBar;
	CToolBar    m_wndToolsToolBar;
	CDialogBar	m_wndDialogBar;
	CReBar		m_wndRebar;
	CMenu		*m_pMenu;
	C3DStiffenerPage* m_p3DStiffenerPage;
	C3DViewEditDlg*	m_p3DViewEditPage;
	C3DEditTurnTablePage* m_p3DEditTurnTablePage;
	C3DDesignPage*	m_p3DDesignPage;
	C3DViewPage* m_p3DViewPage;
	C3DToolsSheet* m_p3DToolsSheet;
	CWnd**	m_pDlg;

	CProfileSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CProfileSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfileSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void Invalidate3DImages();
	void SetAccessPrivelage();
	void UpdateAllPages();
	void SetWindowTitle();
	virtual ~CProfileSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProfileSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileNew();
	afx_msg void OnTtlistAdd();
	afx_msg void OnTtlistDelete();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnToolsDrawingoptions();
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnCopyAllscanlinestaughtlines();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILESHEET_H__B7133A9F_58B3_46FF_88BB_7EF3E3B517F4__INCLUDED_)
