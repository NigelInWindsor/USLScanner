#if !defined(AFX_PMACSHEET_H__11FD717D_0B50_4D9D_B5F6_1B9F3EA6F64B__INCLUDED_)
#define AFX_PMACSHEET_H__11FD717D_0B50_4D9D_B5F6_1B9F3EA6F64B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacSheet.h : header file
//
#include "PmacGeneralPage.h"
#include "PmacVariablesPage.h"
#include "PmacDownloadPage.h"
#include "PmacArchivePage.h"
#include "PmacTuningPage.h"
#include "PmacFaultFindingPage.h"
#include "PmacDPRAMTestPage.h"
#include "LeadScrewCompensationPage.h"
#include "PmacTrajectoryPage.h"
#include "PmacMacroPage.h"
#include "PmacFaultsPage.h"
#include "RobotCommsPage.h"
#include "HilscherPage.h"
#include "PowerPmacStatusPage.h"

#include "ResizableSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CPmacSheet

class CPmacSheet : public CResizableSheet
{
	DECLARE_DYNAMIC(CPmacSheet)

// Construction
public:
	CPowerPmacStatusPage* m_pPowerPmacStatusPage;
	CHilscherPage* m_pHilscherPage;
	CRobotCommsPage* m_pRobotCommsPage;
	CPmacFaultsPage* m_pPmacFaultsPage;
	CPmacMacroPage* m_pPmacMacroPage;
	CPmacTrajectoryPage* m_pPmacTrajectoryPage;
	CLeadScrewCompensationPage* m_pLeadScrewCompensationPage;
	CPmacDPRAMTestPage* m_pPmacDPRAMTestPage;
	CPmacTuningPage* m_pPmacTuningPage;
	CPmacArchivePage* m_pPmacArchivePage;
	CPmacDownLoadPage* m_pDownLoad;
	CPmacVariablesPage* m_pIVariables;
	CPmacVariablesPage* m_pMVariables;
	CPmacGeneralPage* m_pPmacGeneralPage;
	CPmacFaultFindingPage* m_pPmacFaultFindingPage;
	
	CWnd** m_pDlg;
	void Construct(CWnd** pDlg);
	CPmacSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CPmacSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:
	void UpdateAllPages();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPmacSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTabNames();
	virtual ~CPmacSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPmacSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACSHEET_H__11FD717D_0B50_4D9D_B5F6_1B9F3EA6F64B__INCLUDED_)
