// PositionSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PositionSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPositionSheet

IMPLEMENT_DYNAMIC(CPositionSheet, CResizableSheet)

inline void CPositionSheet::Construct(CWnd** pDlg)
{
	m_pDlg = pDlg;
	
	m_pPositionPage=NULL;
	m_pHoldingFokkerPage=NULL;
	m_pPositionSingleBridePage=NULL;
	m_pRobotDualPositionPage=NULL;
	m_pPositionGraphicalPage=NULL;
	m_pPositionSwivGimRotPage=NULL;

	if(theApp.m_LastSettings.nPositionTabMask & PositionPositionTab) {
		m_pPositionPage = new CPositionPage();
		AddPage(m_pPositionPage);
	}
	if(theApp.m_LastSettings.nPositionTabMask & PositionSingleBridgeTab) {
		m_pPositionSingleBridePage = new CPositionSingleBridgePage();
		AddPage(m_pPositionSingleBridePage);
	}

	if(theApp.m_LastSettings.nPositionTabMask & PositionHoldingTab) {
		m_pHoldingFokkerPage = new CPositionHoldingFokkerPage();
		AddPage(m_pHoldingFokkerPage);
	}
	if(theApp.m_LastSettings.nPositionTabMask & PositionRobotDualTab) {
		m_pRobotDualPositionPage = new CRobotDualPosPage();
		AddPage(m_pRobotDualPositionPage);
	}
	if(theApp.m_LastSettings.nPositionTabMask & PositionGraphicalTab) {
		m_pPositionGraphicalPage = new CPositionGraphicalPage();
		AddPage(m_pPositionGraphicalPage);
	}
	if(theApp.m_LastSettings.nPositionTabMask & PositionSwivGimTab) {
		m_pPositionSwivGimRotPage = new CPositionSwivGimRotPage();
		AddPage(m_pPositionSwivGimRotPage);
	}

}

CPositionSheet::CPositionSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CPositionSheet::CPositionSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CPositionSheet::~CPositionSheet()
{
	SAFE_DELETE(m_pPositionPage);
	SAFE_DELETE(m_pHoldingFokkerPage);
	SAFE_DELETE(m_pPositionSingleBridePage);
	SAFE_DELETE(m_pRobotDualPositionPage);
	SAFE_DELETE(m_pPositionGraphicalPage);
	SAFE_DELETE(m_pPositionSwivGimRotPage);
}


BEGIN_MESSAGE_MAP(CPositionSheet, CResizableSheet)
	//{{AFX_MSG_MAP(CPositionSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPositionSheet message handlers
void CPositionSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CResizableSheet::PostNcDestroy();
	delete this;
}

void CPositionSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_POSITION_SHEET]=FALSE;
	
	CResizableSheet::OnClose();
}

void CPositionSheet::OnDestroy() 
{
	if (IDD_ULTRASONICS_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_POSITION_SHEET]);
	
	CResizableSheet::OnDestroy();
	
	*m_pDlg	= NULL;		
	
}

BOOL CPositionSheet::OnInitDialog() 
{
	CResizableSheet::OnInitDialog();

//	ModifyStyleEx(WS_EX_DLGMODALFRAME, WS_CAPTION);	// enable sys menu and icon
	
	// set minimal size
	CRect rc;
	GetWindowRect(&rc);

	SetMinTrackSize(CSize(GetMinWidth(), rc.Height()));

	EnableSaveRestore(_T("PositionSheet"), TRUE, TRUE);
	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);


	return TRUE;
}

