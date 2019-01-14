// ViewOptionsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ViewOptionsSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewOptionsSheet

IMPLEMENT_DYNAMIC(CViewOptionsSheet, CPropertySheet)

inline void CViewOptionsSheet::Construct(CWnd* pParent,CWnd** pDlg,CData* pData,int nOptions)
{
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pData = pData;

	m_pHistogramOptionsPage = NULL;
	m_pGraticulePage = NULL;
	m_pZoomPage = NULL;
	m_pRulerPage = NULL;
	m_pFilterPage = NULL;
	m_pValuePage = NULL;
	m_pEdgePage = NULL;
	m_pPAEdgeEqualizerPage = NULL;
	m_pImageProcessorPage = NULL;
	m_pOverlayPage = NULL;
//	m_pRotaryOptionsPage = new CRotaryOptionsPage(pParent,pData);
	m_pEnvelopePage = NULL;
	m_pLeAlignPage = NULL;
	m_pBladeTolerancePage = NULL;
	m_pBladeMasterPage = NULL;
	m_pBladeCSVPage = NULL;
	m_pPhasePage = NULL;


	switch(nOptions) {
	case 0:
		m_pHistogramOptionsPage = new CHistogramOptionsPage(pParent);
		m_pGraticulePage = new CGraticulePage(pParent);
		m_pZoomPage = new CZoomPage(pParent);
		m_pRulerPage = new CRulerPage(pParent);
		m_pFilterPage = new CImageFilterPage(pParent);
		m_pValuePage = new CViewValuePage(pParent);
		m_pEdgePage = new CEdgeEqualizerPage(pParent);
		m_pPAEdgeEqualizerPage = new CPAEdgeEqualizerPage(pParent);
		m_pImageProcessorPage = new CImageProcessorPage(pParent);
		m_pOverlayPage = new COverlayPage(pParent);
	//	m_pRotaryOptionsPage = new CRotaryOptionsPage(pParent,pData);
		m_pEnvelopePage = new CEnvelopePage(pParent, pData);

		AddPage(m_pHistogramOptionsPage);
		AddPage(m_pGraticulePage);
		AddPage(m_pZoomPage);
		AddPage(m_pRulerPage);
		AddPage(m_pFilterPage);
		AddPage(m_pValuePage);
		if((m_pData->m_nNumberProbesPerImage<=1) || (m_pData->m_nScanType == MULTI_PROBE_SCAN)) {
			AddPage(m_pEdgePage);
		} else {
			AddPage(m_pPAEdgeEqualizerPage);
		}
		AddPage(m_pImageProcessorPage);
		AddPage(m_pOverlayPage);
		AddPage(m_pEnvelopePage);
		if((m_pData->m_nNumberPhaseImages[0] +m_pData->m_nNumberPhaseImages[1] + m_pData->m_nNumberPhaseImages[2]) >0) {
			m_pPhasePage = new CPhasePage(pParent, pData);
			AddPage(m_pPhasePage);
		}
		break;
	case 1:
		m_pLeAlignPage = new CLeAlignPage(pParent);
		m_pBladeTolerancePage = new CBladeTolerancePage(pParent);
		m_pBladeMasterPage = new CBladeMasterPage(pParent,m_pData);
		m_pBladeCSVPage = new CBladeCSVPage(pParent, m_pData);
		AddPage(m_pBladeTolerancePage);
		AddPage(m_pLeAlignPage);
		AddPage(m_pBladeMasterPage);
		AddPage(m_pBladeCSVPage);
		break;
	}


	SetActivePage(theApp.m_LastSettings.nLastViewOptionsTab);

}

CViewOptionsSheet::CViewOptionsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg,CData* pData,int nOptions)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pParentWnd,pDlg,pData,nOptions);
}

CViewOptionsSheet::CViewOptionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg,CData* pData,int nOptions)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pParentWnd,pDlg,pData,nOptions);
}

CViewOptionsSheet::~CViewOptionsSheet()
{
	theApp.SaveLastSettings();

	SAFE_DELETE(m_pHistogramOptionsPage);
	SAFE_DELETE(m_pGraticulePage);
	SAFE_DELETE(m_pZoomPage);
	SAFE_DELETE(m_pRulerPage);
	SAFE_DELETE(m_pFilterPage);
	SAFE_DELETE(m_pValuePage);
	SAFE_DELETE(m_pEdgePage);
	SAFE_DELETE(m_pPAEdgeEqualizerPage);
	SAFE_DELETE(m_pImageProcessorPage);
	SAFE_DELETE(m_pOverlayPage);
	SAFE_DELETE(m_pEnvelopePage);
//	delete m_pRotaryOptionsPage;
	SAFE_DELETE(m_pBladeTolerancePage);
	SAFE_DELETE(m_pLeAlignPage);
	SAFE_DELETE(m_pBladeMasterPage);
	SAFE_DELETE(m_pBladeCSVPage);
	SAFE_DELETE(m_pPhasePage);
}


BEGIN_MESSAGE_MAP(CViewOptionsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CViewOptionsSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewOptionsSheet message handlers

void CViewOptionsSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void CViewOptionsSheet::OnClose() 
{
	
	CPropertySheet::OnClose();
}

void CViewOptionsSheet::OnDestroy() 
{
	if (IDD_VIEW_OPTIONS_SHEET< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_VIEW_OPTIONS_SHEET]);

	CPropertySheet::OnDestroy();
	*m_pDlg	= NULL;	
}

BOOL CViewOptionsSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	if (IDD_VIEW_OPTIONS_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_VIEW_OPTIONS_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}


	
	return bResult;
}

void CViewOptionsSheet::UpdateAllPages()
{
	if(m_pZoomPage) {
		if(m_pZoomPage->m_hWnd != NULL) {
			m_pZoomPage->UpdateAllControls();
			m_pZoomPage->m_bPalette=TRUE;
			m_pZoomPage->Invalidate(FALSE);
		}
	}

}
