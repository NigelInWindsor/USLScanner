#if !defined(AFX_ROBOTSHEET_H__1E6BB734_1192_423D_8341_E91DD1A5B0DC__INCLUDED_)
#define AFX_ROBOTSHEET_H__1E6BB734_1192_423D_8341_E91DD1A5B0DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotSheet.h : header file
//
#include "RobotPage.h"
#include "RobotCommsPage.h"
#include "RobotJointsPage.h"
#include "RobotEndEffectorPage.h"
#include "RobotHandShakingPage.h"
#include "EthercatPage.h"
#include "MoveLadderPage.h"

#include "ResizableSheet.h"


/////////////////////////////////////////////////////////////////////////////
// CRobotSheet

class CRobotSheet : public CResizableSheet
{
	DECLARE_DYNAMIC(CRobotSheet)

// Construction
public:
	CMoveLadderPage*		m_pMoveLadderPage;
	CEthercatPage*			m_pEthercatPage;
	CRobotHandShakingPage*	m_pRobotHandShakingPage;
	CRobotEndEffectorPage*	m_pRobotEndEffectorPage;
	CRobotJointsPage*		m_pRobotJointsPage;
	CRobotCommsPage*		m_pRobotCommsPage;
	CRobotPage*				m_pRobotPage;
	
	CWnd** m_pDlg;
	void Construct(CWnd** pDlg);
	CRobotSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CRobotSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateAllPages();
	virtual ~CRobotSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRobotSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTSHEET_H__1E6BB734_1192_423D_8341_E91DD1A5B0DC__INCLUDED_)
