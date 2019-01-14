// MaintenanceDiagnosticsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "MaintenanceDiagnosticsSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceDiagnosticsSheet

IMPLEMENT_DYNAMIC(CMaintenanceDiagnosticsSheet, CPropertySheet)

inline void CMaintenanceDiagnosticsSheet::Construct(CWnd **pDlg)
{
	m_pDlg = pDlg;
	m_pActivityLogPage = NULL;
	m_pSerialJoystickDlg = NULL;
	m_pServoDrivesPage = NULL;
	m_pPrinterDiagnosticsPage = NULL;
	m_pEthernetPage = NULL;
	m_pAcqirisDlg = NULL;
	m_pFaroTrackerPage = NULL;
	m_pProtractorPage = NULL;
	m_pRemotePage=NULL;
	m_pMarkerPenPage = NULL;
	m_pMachineMonitorPage = NULL;
	m_pSI10Page=NULL;
	m_pEgismosPage = NULL;
	m_pFLSC10Page = NULL;
	m_pPM40Page = NULL;
#ifdef NPTRACKINGTOOLS
	m_pV120Page = NULL;
#endif
	m_pAOSPhasedArrayPage = NULL;


	m_pActivityLogPage = new CActivityLogPage();
	AddPage(m_pActivityLogPage);

	m_pSerialJoystickDlg = new CSerialJoystickDlg();
	AddPage(m_pSerialJoystickDlg);

	m_pServoDrivesPage = new CServoDrivesPage();
	AddPage(m_pServoDrivesPage);

/*	m_pPrinterDiagnosticsPage = new CPrinterDiagnosticsPage();
	AddPage(m_pPrinterDiagnosticsPage);
*/
	m_pEthernetPage = new CEthernetPage();
	AddPage(m_pEthernetPage);

	m_pMaintenancePage = new CMaintenancePage();
	AddPage(m_pMaintenancePage);

	m_pAcqirisDlg = new CAcqirisDlg();
	AddPage(m_pAcqirisDlg);

	m_pFaroTrackerPage = new CFaroTrackerPage();
	AddPage(m_pFaroTrackerPage);

	m_pProtractorPage = new CProtractorPage();
	AddPage(m_pProtractorPage);

	m_pRemotePage = new CRemotePage();
	AddPage(m_pRemotePage);

	m_pMarkerPenPage = new CMarkerPenPage();
	AddPage(m_pMarkerPenPage);

	m_pMachineMonitorPage = new CMachineMonitorPage();
	AddPage(m_pMachineMonitorPage);


	m_pSI10Page = new CSI10Page();
	AddPage(m_pSI10Page);

	
	m_pEgismosPage = new CEgismosRangefinderPage();
	AddPage(m_pEgismosPage);

	m_pFLSC10Page = new CFLSC10Page();
	AddPage(m_pFLSC10Page);


#ifdef NPTRACKINGTOOLS
	m_pV120Page = new CV120Page();
	AddPage(m_pV120Page);
#endif
	
	m_pAOSPhasedArrayPage = new CAOSPhasedArrayPage();
	AddPage(m_pAOSPhasedArrayPage);


	m_pPM40Page = new CPM40Page();
	AddPage(m_pPM40Page);

	SetActivePage(theApp.m_LastSettings.nLastMaintenanceTab);

}

CMaintenanceDiagnosticsSheet::CMaintenanceDiagnosticsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);

}

CMaintenanceDiagnosticsSheet::CMaintenanceDiagnosticsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CMaintenanceDiagnosticsSheet::~CMaintenanceDiagnosticsSheet()
{
	SAFE_DELETE( m_pActivityLogPage );
	SAFE_DELETE( m_pSerialJoystickDlg );
	SAFE_DELETE( m_pServoDrivesPage );
	SAFE_DELETE( m_pPrinterDiagnosticsPage );
	SAFE_DELETE( m_pEthernetPage );
	SAFE_DELETE( m_pMaintenancePage );
	SAFE_DELETE( m_pAcqirisDlg );
	SAFE_DELETE( m_pFaroTrackerPage );
	SAFE_DELETE( m_pProtractorPage );
	SAFE_DELETE( m_pRemotePage );
	SAFE_DELETE( m_pMarkerPenPage );
	SAFE_DELETE( m_pMachineMonitorPage );
	SAFE_DELETE( m_pSI10Page );
	SAFE_DELETE( m_pEgismosPage );
	SAFE_DELETE( m_pFLSC10Page );
#ifdef NPTRACKINGTOOLS
	SAFE_DELETE( m_pV120Page );
#endif
//	SAFE_DELETE(m_pAOSPhasedArrayPage);
	SAFE_DELETE(m_pPM40Page);
	SAFE_DELETE(m_pAOSPhasedArrayPage);
}


BEGIN_MESSAGE_MAP(CMaintenanceDiagnosticsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMaintenanceDiagnosticsSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(REMOTE_HANDLER_FLSC10, RemoteHandlerFLSC10)
	ON_MESSAGE(REMOTE_HANDLER_EGISMOSRF, RemoteHandlerEgismosRF)
	ON_MESSAGE(REMOTE_PAGE_UPDATE_CONTROLS, RemotePageUpdateControls)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceDiagnosticsSheet message handlers


void CMaintenanceDiagnosticsSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void CMaintenanceDiagnosticsSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_MAINTENANCE_DIAGNOSTICS_SHEET]=FALSE;
	
	CPropertySheet::OnClose();
}

void CMaintenanceDiagnosticsSheet::OnDestroy() 
{
	//Store Window Position
	if (IDD_MAINTENANCE_DIAGNOSTICS_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_MAINTENANCE_DIAGNOSTICS_SHEET]);

	CPropertySheet::OnDestroy();
	
	*m_pDlg	= NULL;	
}

BOOL CMaintenanceDiagnosticsSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	if (IDD_MAINTENANCE_DIAGNOSTICS_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_MAINTENANCE_DIAGNOSTICS_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}


	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);

	
	return bResult;
}


BOOL CMaintenanceDiagnosticsSheet::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	CPropertyPage* pPage = GetActivePage();


	if(m_pActivityLogPage == pPage) {
		m_pActivityLogPage->MouseWheel(nFlags, zDelta, pt);
		return true;
	}



	return CPropertySheet::OnMouseWheel(nFlags, zDelta, pt);
}

LRESULT CMaintenanceDiagnosticsSheet::RemoteHandlerFLSC10(WPARAM Wp, LPARAM Lp)
{
	if(m_pFLSC10Page != NULL) m_pFLSC10Page->PostMessage(REMOTE_HANDLER, Wp);
	return 0;
}

LRESULT CMaintenanceDiagnosticsSheet::RemoteHandlerEgismosRF(WPARAM Wp, LPARAM Lp)
{
	if(m_pEgismosPage != NULL) m_pEgismosPage->PostMessage(REMOTE_HANDLER, Wp);
	return 0;
}

LRESULT CMaintenanceDiagnosticsSheet::RemotePageUpdateControls(WPARAM Wp, LPARAM Lp)
{
	if (m_pRemotePage != NULL) {
		if (::IsWindow(m_pRemotePage->m_hWnd)) {
			m_pRemotePage->PostMessage(REMOTE_PAGE_UPDATE_CONTROLS, Wp);
		}
	}
	return 0;
}
