// RobotSheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotSheet

IMPLEMENT_DYNAMIC(CRobotSheet, CResizableSheet)

inline void CRobotSheet::Construct(CWnd** pDlg)
{
	

	m_pDlg = pDlg;
	
	m_pRobotCommsPage = new CRobotCommsPage;
	m_pRobotPage = new CRobotPage;
	m_pRobotJointsPage = new CRobotJointsPage;
	m_pRobotEndEffectorPage = new CRobotEndEffectorPage;
	m_pRobotHandShakingPage = new CRobotHandShakingPage;
	m_pEthercatPage = new CEthercatPage;
	m_pMoveLadderPage = new CMoveLadderPage();

	AddPage(m_pRobotCommsPage);
	AddPage(m_pRobotPage);
	AddPage(m_pRobotJointsPage);
	AddPage(m_pRobotEndEffectorPage);
	AddPage(m_pRobotHandShakingPage);
	AddPage(m_pEthercatPage);
	AddPage(m_pMoveLadderPage );

	SetActivePage(theApp.m_LastSettings.nLastRobotTab);

}

CRobotSheet::CRobotSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CRobotSheet::CRobotSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CRobotSheet::~CRobotSheet()
{
	SAFE_DELETE( m_pRobotCommsPage );
	SAFE_DELETE( m_pRobotPage );
	SAFE_DELETE( m_pRobotJointsPage );
	SAFE_DELETE( m_pRobotEndEffectorPage );
	SAFE_DELETE( m_pRobotHandShakingPage );
	SAFE_DELETE( m_pEthercatPage );
	SAFE_DELETE( m_pMoveLadderPage );

}


BEGIN_MESSAGE_MAP(CRobotSheet, CResizableSheet)
	//{{AFX_MSG_MAP(CRobotSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotSheet message handlers

void CRobotSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CResizableSheet::PostNcDestroy();
	delete this;
}

void CRobotSheet::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_ROBOT_SHEET] = FALSE;
	
	CResizableSheet::OnClose();
}

void CRobotSheet::OnDestroy() 
{
	

	if (IDD_ROBOT_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_ROBOT_SHEET]);

	CResizableSheet::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CRobotSheet::OnInitDialog() 
{
	
	BOOL bResult = CResizableSheet::OnInitDialog();
	
	

	CRect rc;
	GetWindowRect(&rc);

	SetMinTrackSize(CSize(GetMinWidth(), rc.Height()));

	EnableSaveRestore(_T("RobotSheet"), TRUE, TRUE);

	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);

	return bResult;
}

void CRobotSheet::UpdateAllPages()
{
	if( m_pRobotCommsPage ) m_pRobotCommsPage->UpdateAllControls();
	if( m_pRobotPage ) m_pRobotPage->UpdateAllControls();
	if( m_pRobotJointsPage ) m_pRobotJointsPage->UpdateAllControls();
	if( m_pRobotEndEffectorPage ) m_pRobotEndEffectorPage->UpdateAllControls();
	if( m_pRobotHandShakingPage ) m_pRobotHandShakingPage->UpdateAllControls();

}
