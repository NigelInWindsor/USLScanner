// 3DToolsSheet1.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "3DToolsSheet1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DToolsSheet

IMPLEMENT_DYNAMIC(C3DToolsSheet, CPropertySheet)
inline void C3DToolsSheet::Construct(CWnd* pParentWnd,CWnd** pDlg, int nPageOption)
{
	
	m_pDlg = pDlg;
	CPropertySheet *pSheet = (CPropertySheet*)pParentWnd;
	CPropertyPage *pPage = pSheet->GetActivePage();

	m_p3DLightsPage=NULL;
	m_pIsometricPage=NULL;
	m_pDrawingToolsPage=NULL;
	m_p3DEyePointPage=NULL;
	m_pLaserToolsPage = NULL;
	m_p3DWorldViewLightsPage = NULL;

	switch (nPageOption) {
	case 0:
		m_p3DEyePointPage = new C3DEyePointPage(this, pPage);
		AddPage(m_p3DEyePointPage);

		m_pDrawingToolsPage = new CDrawingToollsDlg(this, pParentWnd);
		AddPage(m_pDrawingToolsPage);

		m_pIsometricPage = new CIsometricPage(this, pParentWnd);
		AddPage(m_pIsometricPage);

		m_p3DLightsPage = new C3DLightsPage(this, pPage);
		AddPage(m_p3DLightsPage);

		m_pLaserToolsPage = new CLaserToolsPage(this, pPage);
		AddPage(m_pLaserToolsPage);

		m_pTessellationPage = new CTessellationPage(this, pPage);
		AddPage(m_pTessellationPage);
		break;
	case 1:

		m_p3DWorldViewLightsPage = new C3DWorldViewLightsPage(this, pPage);
		AddPage(m_p3DWorldViewLightsPage);
		break;
	}
	SetActivePage(theApp.m_LastSettings.nLast3DDrawingToolTab);

}

C3DToolsSheet::C3DToolsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg, int nPageOption)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pParentWnd,pDlg, nPageOption);
}

C3DToolsSheet::C3DToolsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg, int nPageOption)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pParentWnd,pDlg, nPageOption);
}

C3DToolsSheet::~C3DToolsSheet()
{
	SAFE_DELETE( m_pDrawingToolsPage );
	SAFE_DELETE( m_p3DEyePointPage );
	SAFE_DELETE( m_pIsometricPage );
	SAFE_DELETE( m_p3DLightsPage );
	SAFE_DELETE( m_pLaserToolsPage );
	SAFE_DELETE( m_pTessellationPage );
	SAFE_DELETE(m_p3DWorldViewLightsPage);
}


BEGIN_MESSAGE_MAP(C3DToolsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(C3DToolsSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DToolsSheet message handlers

void C3DToolsSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void C3DToolsSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_3D_TOOLS_SHEET]=FALSE;
	
	CPropertySheet::OnClose();
}

void C3DToolsSheet::OnDestroy() 
{
	//Store Window Position
	if (IDD_3D_TOOLS_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_3D_TOOLS_SHEET]);

	CPropertySheet::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}


BOOL C3DToolsSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	if (IDD_3D_TOOLS_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_3D_TOOLS_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}


	

	
	return bResult;



}

void C3DToolsSheet::UpdateAllPages()
{
	if(m_p3DEyePointPage) m_p3DEyePointPage->UpdateAllControls();
	if(m_pDrawingToolsPage) m_pDrawingToolsPage->UpdateAllControls();
	if(m_pIsometricPage) m_pIsometricPage->UpdateAllControls();
}
