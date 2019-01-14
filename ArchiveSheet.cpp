// ArchiveSheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ArchiveSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArchiveSheet

IMPLEMENT_DYNAMIC(CArchiveSheet, CPropertySheet)

CArchiveSheet::CArchiveSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CArchiveSheet::CArchiveSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	m_pDlg = pDlg;
	
	m_pDirectoriesPage = NULL;
	m_pRRMESPage = NULL;


	m_pDirectoriesPage = new CDirectoriesDlg;
	AddPage(m_pDirectoriesPage);
	m_pRRMESPage = new CRRMESPage;
	AddPage(m_pRRMESPage);

}

CArchiveSheet::~CArchiveSheet()
{
	SAFE_DELETE( m_pDirectoriesPage );
	SAFE_DELETE( m_pRRMESPage );

}


BEGIN_MESSAGE_MAP(CArchiveSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CArchiveSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArchiveSheet message handlers

void CArchiveSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}

void CArchiveSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_ARCHIVE_SHEET]=FALSE;
	
	CPropertySheet::OnClose();
}
void CArchiveSheet::OnDestroy() 
{
	//Store Window Position
	if (IDD_AXES_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_ARCHIVE_SHEET]);

	CPropertySheet::OnDestroy();
	
	*m_pDlg	= NULL;		
}
	
