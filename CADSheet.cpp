// CADSheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "CADSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCADSheet

IMPLEMENT_DYNAMIC(CCADSheet, CPropertySheet)
inline void CCADSheet::Construct(CWnd* pParentWnd, CWnd** pDlg)
{
	m_pViewSheet = pParentWnd;
	m_pDlg = pDlg;
//	m_pMenu = NULL;
	

	m_pCADLinePage = NULL;

	m_pCADLinePage = new CCADLinePage(m_pViewSheet);
	AddPage(m_pCADLinePage);

}

CCADSheet::CCADSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct( pParentWnd, pDlg );
}

CCADSheet::CCADSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct( pParentWnd, pDlg );
}

CCADSheet::~CCADSheet()
{
	SAFE_DELETE( m_pCADLinePage );
}


BEGIN_MESSAGE_MAP(CCADSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CCADSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCADSheet message handlers

void CCADSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void CCADSheet::OnClose() 
{
	
	CPropertySheet::OnClose();
}

void CCADSheet::OnDestroy() 
{
	if (IDD_CAD_SHEET< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_CAD_SHEET]);

	CPropertySheet::OnDestroy();
	*m_pDlg	= NULL;	
}

BOOL CCADSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	if (IDD_CAD_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_CAD_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}
	
	return bResult;
}

void CCADSheet::UpdateAllPages()
{
	if(m_pCADLinePage) m_pCADLinePage->UpdateAllControls();
}
