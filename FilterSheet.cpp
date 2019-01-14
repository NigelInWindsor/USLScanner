// FilterSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "FilterSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterSheet

IMPLEMENT_DYNAMIC(CFilterSheet, CResizableSheet)

CFilterSheet::CFilterSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CResizableSheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CFilterSheet::CFilterSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pDlg = pDlg;
	m_pParksMcClellanPage = NULL;
	m_pWindowedSincPage = NULL;
	m_pArtificialSignalPage = NULL;

	m_pParksMcClellanPage = new CParksMcClellanPage();
	AddPage(m_pParksMcClellanPage);

//	m_pWindowedSincPage = new CWindowedSincPage();
//	AddPage(m_pWindowedSincPage);

//	m_pArtificialSignalPage = new CArtificialSignalPage();
//	AddPage(m_pArtificialSignalPage);

//	SetActivePage(theApp.m_LastSettings.nLastFilterToolTab);
}

CFilterSheet::~CFilterSheet()
{

	SAFE_DELETE(m_pParksMcClellanPage);
	SAFE_DELETE(m_pWindowedSincPage);
	SAFE_DELETE(m_pArtificialSignalPage);
}


BEGIN_MESSAGE_MAP(CFilterSheet, CResizableSheet)
	//{{AFX_MSG_MAP(CFilterSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterSheet message handlers

void CFilterSheet::PostNcDestroy() 
{

	CResizableSheet::PostNcDestroy();
	delete this;
}

void CFilterSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_FILTER_SHEET]=FALSE;
	
	CResizableSheet::OnClose();
}
void CFilterSheet::OnDestroy() 
{
	//Store Window Position
	if (IDD_FILTER_SHEET< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_FILTER_SHEET]);

	CResizableSheet::OnDestroy();
	
	*m_pDlg	= NULL;		
}

BOOL CFilterSheet::OnInitDialog() 
{
	CResizableSheet::OnInitDialog();

	return true;

//	ModifyStyleEx(WS_EX_DLGMODALFRAME, WS_CAPTION);	// enable sys menu and icon
	
	// set minimal size
	CRect rc;
	GetWindowRect(&rc);

//	HWND hWnd = m_pParksMcClellanPage->GetSafeHwnd();
//	AddAnchor(hWnd,CSize(10,10),CSize(50,50),CSize(60,60));

	SetMinTrackSize(CSize(GetMinWidth(), rc.Height()));

	ArrangeLayout();

	EnableSaveRestore(_T("FilterSheet"), TRUE, TRUE);


	return TRUE;
}

