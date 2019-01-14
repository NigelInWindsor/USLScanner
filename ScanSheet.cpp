// ScanSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ScanSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScanSheet

IMPLEMENT_DYNAMIC(CScanSheet, CPropertySheet)


inline void CScanSheet::Construct(CWnd** pDlg)
{
	

	
	m_pDlg = pDlg;
	m_pContourScanPage = NULL;
	m_pFokkerScanDetailsPage = NULL;

	if(theApp.m_LastSettings.bContourScanTab) {
		m_pContourScanPage = new CContourScanPage();
		AddPage(m_pContourScanPage);
	}
		m_pFokkerScanDetailsPage = new CFokkerScanDetailsPage(&theApp.m_Data);
		AddPage(m_pFokkerScanDetailsPage);
}

CScanSheet::CScanSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CScanSheet::CScanSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CScanSheet::~CScanSheet()
{
	delete m_pContourScanPage;
	delete m_pFokkerScanDetailsPage;
}


BEGIN_MESSAGE_MAP(CScanSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CScanSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanSheet message handlers

void CScanSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void CScanSheet::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_SCAN_SHEET]=FALSE;
	
	CPropertySheet::OnClose();
}

void CScanSheet::OnDestroy() 
{

	


	//Store Window Position
	if (IDD_SCAN_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_SCAN_SHEET]);

	CPropertySheet::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CScanSheet::OnInitDialog() 
{
	
	BOOL bResult = CPropertySheet::OnInitDialog();
	

	if (IDD_SCAN_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_SCAN_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()){
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}


	

	
	return bResult;
}

void CScanSheet::UpdateAllPages()
{

	if(m_pContourScanPage) m_pContourScanPage->UpdateAllControls();
	if(m_pFokkerScanDetailsPage) m_pFokkerScanDetailsPage->UpdateAllControls();


}
