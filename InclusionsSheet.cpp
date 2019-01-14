// InclusionsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "InclusionsSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInclusionsSheet

IMPLEMENT_DYNAMIC(CInclusionsSheet, CPropertySheet)

inline void CInclusionsSheet::Construct(CWnd** pDlg,CInclusions* pInclusions, int nGlobalLocal, CData* pData)
{
	
	m_pDlg = pDlg;

	m_pData = pData;

	m_pInclusionsDetailsPage = NULL;
	m_pInclusionRulesPage = NULL;
	m_pInclusionsResultsPage = NULL;

	m_pInclusions = pInclusions;
	m_nGlobalLocal = nGlobalLocal;


	m_pInclusionsDetailsPage = new CInclusionsDetailsPage(m_pInclusions);
	AddPage(m_pInclusionsDetailsPage);

	m_pInclusionRulesPage = new CInclusionRulesPage(m_pInclusions,nGlobalLocal);
	AddPage(m_pInclusionRulesPage);

	if(nGlobalLocal == 1) {
		m_pInclusionsResultsPage = new CInclusionsResultsPage(m_pInclusions, m_pData);
		AddPage(m_pInclusionsResultsPage);
	}

}

CInclusionsSheet::CInclusionsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg,CInclusions* pInclusions, int nGlobalLocal, CData* pData)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg, pInclusions, nGlobalLocal, pData);
}

CInclusionsSheet::CInclusionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg,CInclusions* pInclusions, int nGlobalLocal, CData* pData)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg, pInclusions, nGlobalLocal, pData);
}

CInclusionsSheet::~CInclusionsSheet()
{
	SAFE_DELETE( m_pInclusionRulesPage );
	SAFE_DELETE( m_pInclusionsResultsPage );
	SAFE_DELETE( m_pInclusionsDetailsPage );
}


BEGIN_MESSAGE_MAP(CInclusionsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CInclusionsSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInclusionsSheet message handlers

void CInclusionsSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void CInclusionsSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_INCLUSIONS_SHEET % MAXIMUMWINDOWSIDNUMBER]=FALSE;
	
	CPropertySheet::OnClose();
}

void CInclusionsSheet::OnDestroy() 
{
	//Store Window Position
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_INCLUSIONS_SHEET % MAXIMUMWINDOWSIDNUMBER]);

	CPropertySheet::OnDestroy();
	
	*m_pDlg	= NULL;	
	// TODO: Add your message handler code here
	
}

BOOL CInclusionsSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_INCLUSIONS_SHEET % MAXIMUMWINDOWSIDNUMBER];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		theApp.ConfineToScreen(&rect);
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}


	
	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);

	
	return bResult;
}

void CInclusionsSheet::UpdateAllPages()
{
	CRect rr;

	if(m_pInclusionsDetailsPage) m_pInclusionsDetailsPage->UpdateAllControls();
	if(m_pInclusionRulesPage) m_pInclusionRulesPage->UpdateAllControls();
	if(m_pInclusionsResultsPage) {
		m_pInclusionsResultsPage->FillList();
		m_pInclusionsResultsPage->RenderView(NULL,rr);
	}


}
