#if !defined(AFX_3DTOOLSSHEET1_H__38B8F079_87E0_441D_AEAA_25384BA1EB03__INCLUDED_)
#define AFX_3DTOOLSSHEET1_H__38B8F079_87E0_441D_AEAA_25384BA1EB03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DToolsSheet1.h : header file
//
#include "DrawingToollsDlg.h"
#include "3DEyePointPage.h"
#include "IsometricPage.h"
#include "3DLightsPage.h"
#include "LaserToolsPage.h"
#include "TessellationPage.h"
#include "3DWorldViewLightsPage.h"

/////////////////////////////////////////////////////////////////////////////
// C3DToolsSheet

class C3DToolsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(C3DToolsSheet)

// Construction
public:
	C3DWorldViewLightsPage* m_p3DWorldViewLightsPage;
	CTessellationPage* m_pTessellationPage;
	CLaserToolsPage* m_pLaserToolsPage;
	C3DLightsPage* m_p3DLightsPage;
	CIsometricPage* m_pIsometricPage;
	C3DEyePointPage* m_p3DEyePointPage;
	CDrawingToollsDlg* m_pDrawingToolsPage;
	
	CWnd** m_pDlg;
	void Construct(CWnd* pParentWnd,CWnd** pDlg, int nPageOption);
	C3DToolsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL, int nPageOption = 0);
	C3DToolsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL, int nPageOption = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DToolsSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateAllPages();
	virtual ~C3DToolsSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(C3DToolsSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DTOOLSSHEET1_H__38B8F079_87E0_441D_AEAA_25384BA1EB03__INCLUDED_)
