// PmacSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PmacSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacSheet

IMPLEMENT_DYNAMIC(CPmacSheet, CResizableSheet)
inline void CPmacSheet::Construct(CWnd** pDlg)
{
	m_pDlg = pDlg;
	

	m_pPmacGeneralPage = NULL;
	m_pIVariables = NULL;
	m_pMVariables = NULL;
	m_pDownLoad = NULL;
	m_pPmacArchivePage = NULL;
	m_pPmacTuningPage = NULL;
	m_pPmacFaultFindingPage = NULL;
	m_pPmacDPRAMTestPage = NULL;
	m_pLeadScrewCompensationPage = NULL;
	m_pPmacTrajectoryPage = NULL;
	m_pPmacMacroPage = NULL;
	m_pPmacFaultsPage = NULL;
	m_pRobotCommsPage = NULL;
	m_pPowerPmacStatusPage = NULL;
	m_pHilscherPage = NULL;


	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: 
		m_pPmacGeneralPage = new CPmacGeneralPage();
		m_pIVariables = new CPmacVariablesPage('I');
		m_pMVariables = new CPmacVariablesPage('M');
		m_pDownLoad = new CPmacDownLoadPage();
		m_pPmacArchivePage = new CPmacArchivePage();
		m_pPmacTuningPage = new CPmacTuningPage();
		m_pPmacFaultFindingPage = new CPmacFaultFindingPage();
		m_pPmacDPRAMTestPage = new CPmacDPRAMTestPage();
		m_pLeadScrewCompensationPage = new CLeadScrewCompensationPage();
		m_pPmacTrajectoryPage = new CPmacTrajectoryPage();
		m_pPmacMacroPage = new CPmacMacroPage;
		m_pPmacFaultsPage = new CPmacFaultsPage;
		AddPage(m_pPmacGeneralPage);
		AddPage(m_pIVariables);
		AddPage(m_pMVariables);
		AddPage(m_pDownLoad);
		AddPage(m_pPmacArchivePage);
		AddPage(m_pPmacTuningPage);
		AddPage(m_pPmacFaultFindingPage);
		AddPage(m_pPmacDPRAMTestPage);
		AddPage(m_pLeadScrewCompensationPage);
		AddPage(m_pPmacTrajectoryPage);
		AddPage(m_pPmacMacroPage);
		AddPage(m_pPmacFaultsPage);
		break;
	default:
		m_pRobotCommsPage = new CRobotCommsPage;
		m_pLeadScrewCompensationPage = new CLeadScrewCompensationPage();
		m_pHilscherPage = new CHilscherPage();
		m_pPowerPmacStatusPage = new CPowerPmacStatusPage();
		AddPage(m_pRobotCommsPage);
		AddPage(m_pLeadScrewCompensationPage);
		AddPage(m_pHilscherPage);
		AddPage(m_pPowerPmacStatusPage);
		break;
	}


//	SetActivePage(theApp.m_LastSettings.nLastPmacTab);

}

CPmacSheet::CPmacSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CPmacSheet::CPmacSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CPmacSheet::~CPmacSheet()
{
	SAFE_DELETE( m_pPmacGeneralPage );
	SAFE_DELETE( m_pIVariables);
	SAFE_DELETE( m_pMVariables );
	SAFE_DELETE( m_pDownLoad );
	SAFE_DELETE( m_pPmacArchivePage );
	SAFE_DELETE( m_pPmacTuningPage );
	SAFE_DELETE( m_pPmacFaultFindingPage );
	SAFE_DELETE( m_pPmacDPRAMTestPage );
	SAFE_DELETE( m_pLeadScrewCompensationPage );
	SAFE_DELETE( m_pPmacTrajectoryPage );
	SAFE_DELETE( m_pPmacMacroPage );
	SAFE_DELETE( m_pPmacFaultsPage );
	SAFE_DELETE( m_pRobotCommsPage );
	SAFE_DELETE( m_pHilscherPage );
	SAFE_DELETE( m_pPowerPmacStatusPage );
}


BEGIN_MESSAGE_MAP(CPmacSheet, CResizableSheet)
	//{{AFX_MSG_MAP(CPmacSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacSheet message handlers

void CPmacSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CResizableSheet::PostNcDestroy();
	delete this;
}

void CPmacSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PMAC_SHEET] = FALSE;
	
	CResizableSheet::OnClose();
}

void CPmacSheet::OnDestroy() 
{
	if (IDD_PMAC_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_PMAC_SHEET]);

	CResizableSheet::OnDestroy();
	
	*m_pDlg	= NULL;	
}

BOOL CPmacSheet::OnInitDialog() 
{
	BOOL bResult = CResizableSheet::OnInitDialog();
	
	

	CRect rc;
	GetWindowRect(&rc);

	SetMinTrackSize(CSize(GetMinWidth(), rc.Height()));

	EnableSaveRestore(_T("PmacSheet"), TRUE, TRUE);

	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);

	switch(theApp.m_nMotorType) {
	case SERVO_TURBO: 	SetWindowText(L"PMAC Turbo");
		break;
	case SERVO:  	SetWindowText(L"PMAC");
		break;
	case FIELD_BUS: SetWindowText(L"Power PMAC");
		break;
	}




	SetTabNames();

	return bResult;
}

void CPmacSheet::SetTabNames()
{
	TCITEM TabCtrlItem;
	CTabCtrl* tab = GetTabControl();
	int	nn;

	if(m_pIVariables) {
		nn=GetPageIndex(m_pIVariables);
		ZeroMemory(&TabCtrlItem,sizeof TabCtrlItem);
		tab->GetItem(nn,&TabCtrlItem);
		TabCtrlItem.pszText = _T("I Variables");
		TabCtrlItem.mask=TCIF_TEXT;
		tab->SetItem(nn,&TabCtrlItem);
	}
	if(m_pMVariables) {
		nn=GetPageIndex(m_pMVariables);
		ZeroMemory(&TabCtrlItem,sizeof TabCtrlItem);
		tab->GetItem(nn,&TabCtrlItem);
		TabCtrlItem.pszText = _T("M Variables");
		TabCtrlItem.mask=TCIF_TEXT;
		tab->SetItem(nn,&TabCtrlItem);
	}

}

void CPmacSheet::UpdateAllPages()
{
	if(m_pLeadScrewCompensationPage) m_pLeadScrewCompensationPage->UpdateAllControls();
}