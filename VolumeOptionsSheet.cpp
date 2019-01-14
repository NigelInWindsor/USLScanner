// VolumeOptionsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "VolumeOptionsSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolumeOptionsSheet

IMPLEMENT_DYNAMIC(CVolumeOptionsSheet, CPropertySheet)

inline void CVolumeOptionsSheet::Construct(CWnd* pParent,CWnd** pDlg,GatesData* pGate,CData* pData,CWnd* pViewSheet)
{
	
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pGate = pGate;
	m_pData = pData;
	m_pViewSheet = pViewSheet;

	m_pVolumeAscanPage = NULL;
	m_pHistogramOptionsPage = NULL;
	m_pGraticulePage = NULL;
	m_pZoomPage = NULL;
	m_pRulerPage = NULL;
	m_pFilterPage = NULL;
	m_pValuePage = NULL;
	m_pCorrelationPage = NULL;
	m_pLibraryPage = NULL;


	m_pVolumeAscanPage = new CVolumeAscanPage(pParent,pGate,pData);
	m_pHistogramOptionsPage = new CHistogramOptionsPage(pParent);
	m_pGraticulePage = new CGraticulePage(pParent);
	m_pZoomPage = new CZoomPage(pParent,1);
	m_pRulerPage = new CRulerPage(pParent,1);
	m_pFilterPage = new CImageFilterPage(m_pViewSheet);
	m_pValuePage = new CViewValuePage(pParent);
	m_pCorrelationPage = new CCorrelationPage(pParent);
	m_pLibraryPage = new CVolumeLibraryPage(pParent,pGate);
	AddPage(m_pVolumeAscanPage);
	AddPage(m_pHistogramOptionsPage);
	AddPage(m_pGraticulePage);
	AddPage(m_pZoomPage);
	AddPage(m_pRulerPage);
	AddPage(m_pFilterPage);
	AddPage(m_pValuePage);
	AddPage(m_pCorrelationPage);
	AddPage(m_pLibraryPage);

	SetActivePage(theApp.m_LastSettings.nLastVolumeToolTab);

}

CVolumeOptionsSheet::CVolumeOptionsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg,GatesData* pGate,CData* pData,CWnd* pViewSheet)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pParentWnd,pDlg,pGate,pData, pViewSheet);
}

CVolumeOptionsSheet::CVolumeOptionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg,GatesData* pGate,CData* pData,CWnd* pViewSheet)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pParentWnd,pDlg,pGate,pData, pViewSheet);
}

CVolumeOptionsSheet::~CVolumeOptionsSheet()
{
	

	theApp.SaveLastSettings();

	SAFE_DELETE( m_pVolumeAscanPage);
	SAFE_DELETE( m_pHistogramOptionsPage);
	SAFE_DELETE( m_pGraticulePage);
	SAFE_DELETE( m_pZoomPage);
	SAFE_DELETE( m_pRulerPage);
	SAFE_DELETE( m_pFilterPage);
	SAFE_DELETE( m_pValuePage);
	SAFE_DELETE( m_pCorrelationPage);
	SAFE_DELETE( m_pLibraryPage);
}


BEGIN_MESSAGE_MAP(CVolumeOptionsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CVolumeOptionsSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_MESSAGE(UI_UPDATE_ALL_PAGES, &UpdateAllPages)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeOptionsSheet message handlers

void CVolumeOptionsSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void CVolumeOptionsSheet::OnClose() 
{
	
	CPropertySheet::OnClose();
}

void CVolumeOptionsSheet::OnDestroy() 
{
	

	if (IDD_VOLUME_OPTIONS_SHEET< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_VOLUME_OPTIONS_SHEET]);

	CPropertySheet::OnDestroy();
	*m_pDlg	= NULL;	
}

BOOL CVolumeOptionsSheet::OnInitDialog() 
{
	
	BOOL bResult = CPropertySheet::OnInitDialog();
	

	if (IDD_VOLUME_OPTIONS_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_VOLUME_OPTIONS_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	
	return bResult;
}


HRESULT CVolumeOptionsSheet::UpdateAllPages(WPARAM, LPARAM)
{
	if(m_pZoomPage) {
		if(m_pZoomPage->m_hWnd) {
			m_pZoomPage->UpdateAllControls();
			m_pZoomPage->m_bPalette=TRUE;
			m_pZoomPage->Invalidate(FALSE);
		}
	}
	if(m_pVolumeAscanPage) {
		if(m_pVolumeAscanPage->m_hWnd) {
			m_pVolumeAscanPage->UpdateAllControls();
		}
	}
	return NULL;
}
