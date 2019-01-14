#if !defined(AFX_POSITIONSHEET_H__4A47B144_8433_4611_B978_F0AD08FD7F29__INCLUDED_)
#define AFX_POSITIONSHEET_H__4A47B144_8433_4611_B978_F0AD08FD7F29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PositionSheet.h : header file
//
#include "ResizableSheet.h"

#include "PositionPage.h"
#include "PositionHoldingFokkerPage.h"
#include "PositionSingleBridgePage.h"
#include "RobotDualPosPage.h"
#include "PositionGraphicalPage1.h"
#include "PositionSwivGimRotPage.h"
/////////////////////////////////////////////////////////////////////////////
// CPositionSheet

class CPositionSheet : public CResizableSheet
{
	DECLARE_DYNAMIC(CPositionSheet)

// Construction
public:
	CPositionSwivGimRotPage* m_pPositionSwivGimRotPage;
	CPositionGraphicalPage* m_pPositionGraphicalPage;
	
	CWnd** m_pDlg;
	CRobotDualPosPage *m_pRobotDualPositionPage;
	CPositionHoldingFokkerPage *m_pHoldingFokkerPage;
	CPositionPage *m_pPositionPage;
	CPositionSingleBridgePage *m_pPositionSingleBridePage;
	void Construct(CWnd** pDlg);
	CPositionSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CPositionSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPositionSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPositionSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPositionSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSITIONSHEET_H__4A47B144_8433_4611_B978_F0AD08FD7F29__INCLUDED_)
