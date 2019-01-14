#if !defined(AFX_COMPENSATIONSHEET_H__5BBE7130_4DDE_4C27_92CE_EA34A24F77CE__INCLUDED_)
#define AFX_COMPENSATIONSHEET_H__5BBE7130_4DDE_4C27_92CE_EA34A24F77CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CompensationSheet.h : header file
//
#include "KinematicDlg.h"
#include "AxesCompensationPage.h"
#include "BallPage.h"
#include "TurntablePage.h"
#include "IJKPage.h"
#include "RThetaPage.h"
#include "RobotPage.h"
#include "RotateSwivelGimbalPage.h"
#include "QuaternionPage.h"
#include "ToolChangerPage.h"

/////////////////////////////////////////////////////////////////////////////
// CCompensationSheet

class CCompensationSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCompensationSheet)

// Construction
public:
	CToolChangerPage* m_pToolChangerPage;
	CQuaternionPage* m_pQuaternionPage;
	CRotateSwivelGimbalPage* m_pRotateSwivelGimbalPage;
	CRobotPage*	m_pRobotPage;
	CRThetaPage* m_pRThetaPage;
	CIJKPage* m_pIJKPage;
	CTurnTablePage* m_pTurntablePage;
	CBallPage*	m_pBallPage;
	CAxesCompensationPage* m_pAxesCompensationPage;
	CKinematicDlg* m_pKinematicsPage;
	CWnd** m_pDlg;
	
	void Construct(CWnd** pDlg);
	CCompensationSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CCompensationSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompensationSheet)
	public:
	BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCompensationSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCompensationSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPENSATIONSHEET_H__5BBE7130_4DDE_4C27_92CE_EA34A24F77CE__INCLUDED_)
