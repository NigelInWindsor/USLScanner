#if !defined(AFX_PALETTESHEET_H__E53F1465_4455_462E_BE8B_254C8EF73588__INCLUDED_)
#define AFX_PALETTESHEET_H__E53F1465_4455_462E_BE8B_254C8EF73588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaletteSheet.h : header file
//
#include "ResizableSheet.h"

#include "AmplitudePalettePage.h"
#include "ThicknessPalettePage.h"
#include "PorosityPage.h"
#include "USLMFCToolBar.h"

/////////////////////////////////////////////////////////////////////////////
// CPaletteSheet

class CPaletteSheet : public CResizableSheet
{
	DECLARE_DYNAMIC(CPaletteSheet)

// Construction
public:
	void	SetWindowTitle();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CUSLMFCToolBar   m_wndPorosityToolBar;
	CReBar     m_wndRebar;
	CAmplitudePalettePage* m_pAmplitude;
	CThicknessPalettePage* m_pThickness;
	CPorosityPage* m_pPorosity;
	CWnd** m_pDlg;
	
	CPaletteSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPaletteSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateAllPages();
	virtual ~CPaletteSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaletteSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonPorosityCurves();
	afx_msg void OnButtonFixedPorosityCurve();
	afx_msg void OnFileSaveas();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenPorosity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTESHEET_H__E53F1465_4455_462E_BE8B_254C8EF73588__INCLUDED_)
