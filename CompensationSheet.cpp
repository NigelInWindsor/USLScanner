// CompensationSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "CompensationSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompensationSheet

IMPLEMENT_DYNAMIC(CCompensationSheet, CPropertySheet)

inline void CCompensationSheet::Construct(CWnd** pDlg)
{
	m_pDlg = pDlg;
	
	m_pKinematicsPage = NULL;
	m_pAxesCompensationPage = NULL;
	m_pBallPage = NULL;
	m_pTurntablePage=NULL;
	m_pIJKPage=NULL;
	m_pRThetaPage=NULL;
	m_pRobotPage=NULL;
	m_pRotateSwivelGimbalPage=NULL;
	m_pQuaternionPage=NULL;
	m_pToolChangerPage=NULL;


	m_pKinematicsPage = new CKinematicDlg();
	AddPage(m_pKinematicsPage);

//	m_pAxesCompensationPage = new CAxesCompensationPage();
//	AddPage(m_pAxesCompensationPage);

	m_pBallPage = new CBallPage();
	AddPage(m_pBallPage);

	m_pTurntablePage = new CTurnTablePage();
	AddPage(m_pTurntablePage);

	m_pIJKPage = new CIJKPage();
	AddPage(m_pIJKPage);

	m_pRThetaPage = new CRThetaPage();
	AddPage(m_pRThetaPage);

//	m_pRobotPage = new CRobotPage();
//	AddPage(m_pRobotPage);

	m_pRotateSwivelGimbalPage = new CRotateSwivelGimbalPage();
	AddPage(m_pRotateSwivelGimbalPage);

	m_pToolChangerPage = new CToolChangerPage();
	AddPage(m_pToolChangerPage);


	SetActivePage(theApp.m_LastSettings.nLastCompensationToolTab);



}

CCompensationSheet::CCompensationSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CCompensationSheet::CCompensationSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CCompensationSheet::~CCompensationSheet()
{
	SAFE_DELETE( m_pRobotPage );
	SAFE_DELETE( m_pIJKPage );
	SAFE_DELETE( m_pTurntablePage );
	SAFE_DELETE( m_pKinematicsPage );
	SAFE_DELETE( m_pAxesCompensationPage );
	SAFE_DELETE( m_pBallPage );
	SAFE_DELETE( m_pRThetaPage );
	SAFE_DELETE( m_pRotateSwivelGimbalPage );
	SAFE_DELETE( m_pToolChangerPage );
}


BEGIN_MESSAGE_MAP(CCompensationSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CCompensationSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompensationSheet message handlers

void CCompensationSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void CCompensationSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_COMPENSATION_SHEET]=FALSE;
	
	CPropertySheet::OnClose();
}

void CCompensationSheet::OnDestroy() 
{
	//Store Window Position
	if (IDD_COMPENSATION_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_COMPENSATION_SHEET]);

	CPropertySheet::OnDestroy();
	
	*m_pDlg	= NULL;	
}

BOOL CCompensationSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	if (IDD_COMPENSATION_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_COMPENSATION_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);
	
	return bResult;
}
