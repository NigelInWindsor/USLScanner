#if !defined(AFX_MOTIONTOOLSSHEET_H__1BD19CAE_BA93_4D32_A8B3_98A9245020B1__INCLUDED_)
#define AFX_MOTIONTOOLSSHEET_H__1BD19CAE_BA93_4D32_A8B3_98A9245020B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionToolsSheet.h : header file
//
#include "NormalizePage.h"
#include "PumpPage.h"
#include "OriginPage.h"
#include "MoveToPage.h"
#include "SquirtersPage.h"
#include "ManipulatorSelectPage.h"
#include "HoldingPage.h"
#include "ContourScanDlg.h"
#include "DetailsPage.h"
#include "RectilinearScanPage.h"
#include "TurnTableScanPage.h"
#include "LiftPage.h"
#include "OriginForeplanePage.h"
#include "OperatorContourScanPage.h"
#include "ScanLinesPage.h"
#include "RRWIPPage.h"
#include "RotateThetaLiftPage.h"
#include "ExtTrigScan.h"
#include "TubeScanPage1.h"
#include "ThicknessCalibrationpage.h"
#include "JogPage.h"
#include "FlatScanPage.h"
#include "RollerScanPage.h"
#include "RRBladeScanPage.h"
#include "VerticalBoreScannerPage.h"
#include "RollerProbePage.h"
#include "RobotArmPositionsPage.h"
#include "TransPlyPage.h"
#include "AmplitudePalettePage.h"
#include "ThicknessPalettePage.h"
#include "RobotUserPage.h"
#include "PhasedArrayLineScanPage.h"
#include "DiscsShaftsPage.h"

/////////////////////////////////////////////////////////////////////////////
// CMotionToolsSheet

class CMotionToolsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMotionToolsSheet)

// Construction
public:
	void UpdateAllPages();
	
	CWnd** m_pDlg;
	void Construct(CWnd** pDlg);
	CMotionToolsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CMotionToolsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotionToolsSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void KillAllTimers();
	CDiscsShaftsPage* m_pDiscsShaftsPage;
	CRobotUserPage* m_pRobotUserPage;
	CAmplitudePalettePage* m_pAmplitude;
	CThicknessPalettePage* m_pThickness;
	CTransPlyPage*	m_pTransPlyPage;
	CRobotArmPositionsPage* m_pRobotArmPositionsPage;
	CRollerProbePage* m_pRollerProbePage;
	CVerticalBoreScannerPage* m_pVerticalBoreScannerPage;
	CJogPage* m_pJogPage;
	CThicknessCalibrationpage* m_pThicknessCalibrationPage;
	CTubeScanPage* m_pTubeScanPage;
	CExtTrigScan* m_pExtTrigScanPage;
	CRotateThetaLiftPage* m_pRotateThetaLiftPage;
	CRRWIPPage* m_pRRWIPPage;
	CScanLinesPage*	m_pScanLinesPage;
	COperatorContourScanPage* m_pOperatorContourScanPage;
	COriginForeplanePage* m_pOriginForeplanePage;
	CLiftPage* m_pLiftPage;
	CTurnTableScanPage* m_pTurnTableScanPage;
	CRectilinearScanPage* m_pRectilinearScanPage;
	CDetailsPage* m_pDetailsPage;
	CContourScanPage* m_pContourScanPage;
	CHoldingPage* m_pHoldingPage;
	CManipulatorSelectPage* m_pManipulatorSelectPage;
	CSquirtersPage* m_pSquirtersPage;
	CMoveToPage* m_pMoveToPage;
	COriginPage* m_pOriginPage;
	CPumpPage* m_pPumpPage;
	CNormalizePage* m_pNormalizePage;
	CFlatScanPage* m_pFlatScanPage;
	CRollerScanPage* m_pRollerScanPage;
	CRRBladeScanPage* m_pRRBladeScanPage;
	CPhasedArrayLineScanPage* m_pPhasedArrayLineScanPage;

	void SetTabNames();
	virtual ~CMotionToolsSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMotionToolsSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////
//	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONTOOLSSHEET_H__1BD19CAE_BA93_4D32_A8B3_98A9245020B1__INCLUDED_)
