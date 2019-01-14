// MotionToolsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "MotionToolsSheet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMotionToolsSheet

IMPLEMENT_DYNAMIC(CMotionToolsSheet, CPropertySheet)

inline void CMotionToolsSheet::Construct(CWnd** pDlg)
{
	
	m_pDlg = pDlg;

	m_pNormalizePage = NULL;
	m_pPumpPage = NULL;
	m_pOriginPage = NULL;
	m_pManipulatorSelectPage = NULL;
	m_pHoldingPage = NULL;
	m_pContourScanPage = NULL;
	m_pRectilinearScanPage = NULL;
	m_pTurnTableScanPage = NULL;
	m_pDetailsPage = NULL;
	m_pOriginPage = NULL;
	m_pMoveToPage = NULL;
	m_pSquirtersPage = NULL;
	m_pLiftPage = NULL;
	m_pOriginForeplanePage = NULL;
	m_pOperatorContourScanPage = NULL;
	m_pScanLinesPage = NULL;
	m_pRRWIPPage = NULL;
	m_pRotateThetaLiftPage = NULL;
	m_pExtTrigScanPage = NULL;
	m_pTubeScanPage = NULL;
	m_pThicknessCalibrationPage = NULL;
	m_pJogPage = NULL;
	m_pFlatScanPage = NULL;
	m_pRollerScanPage = NULL;
	m_pRRBladeScanPage = NULL;
	m_pVerticalBoreScannerPage = NULL;
	m_pRollerProbePage = NULL;
	m_pRobotArmPositionsPage = NULL;
	m_pTransPlyPage = NULL;
	m_pAmplitude = NULL;
	m_pThickness = NULL;
	m_pRobotUserPage = NULL;
	m_pPhasedArrayLineScanPage = NULL;
	m_pDiscsShaftsPage = NULL;

	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] == 0)  theApp.m_LastSettings.nMotoionToolsTabMask[0] = 0x001;

	if(theApp.m_LastSettings.nMotoionToolsTabMask[0] & Optimize_TAB) {
		m_pNormalizePage = new CNormalizePage();
		AddPage(m_pNormalizePage);
	}
	if(theApp.m_LastSettings.nMotoionToolsTabMask[0] & Pump_TAB) {
		m_pPumpPage = new CPumpPage();
		AddPage(m_pPumpPage);
	}
	if(theApp.m_LastSettings.nMotoionToolsTabMask[0] & Origin_TAB){
		switch(theApp.m_nJobNumber) {
		default:
			m_pOriginPage = new COriginPage();
			AddPage(m_pOriginPage);
			break;
		case FOREPLANE:
			m_pOriginForeplanePage = new COriginForeplanePage();
			AddPage(m_pOriginForeplanePage);
			break;
		}
	}
	if(theApp.m_LastSettings.nMotoionToolsTabMask[0] & Move_To_TAB){
		m_pMoveToPage = new CMoveToPage();
		AddPage(m_pMoveToPage);
	}
	if(theApp.m_LastSettings.nMotoionToolsTabMask[0] & Tip_Axes_TAB) {
		m_pSquirtersPage = new CSquirtersPage();
		AddPage(m_pSquirtersPage);
	}
	if (theApp.m_LastSettings.bMotionToolsLiftTab) {
		m_pLiftPage = new CLiftPage();
		AddPage(m_pLiftPage);
	}
	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & Manipulator_TAB) {
		m_pManipulatorSelectPage = new CManipulatorSelectPage();
		AddPage(m_pManipulatorSelectPage);
	}
	if (theApp.m_LastSettings.bHoldingFixtureTab == TRUE) {
		m_pHoldingPage = new CHoldingPage();
		AddPage(m_pHoldingPage);
	}
	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & Contour_TAB) {
		m_pContourScanPage = new CContourScanPage();
		AddPage(m_pContourScanPage);
	}
	if (theApp.m_LastSettings.bDetailsTab) {
		m_pDetailsPage = new CDetailsPage((CWnd**)&m_pDetailsPage, &theApp.m_Data);
		AddPage(m_pDetailsPage);
	}

	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & Rectilinear_TAB) {
		m_pRectilinearScanPage = new CRectilinearScanPage();
		AddPage(m_pRectilinearScanPage);
	}

	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & Turntable_TAB) {
		m_pTurnTableScanPage = new CTurnTableScanPage();
		AddPage(m_pTurnTableScanPage);
	}
	if (theApp.m_LastSettings.bMotionToolsOperatorContourTab) {
		m_pOperatorContourScanPage = new COperatorContourScanPage();
		AddPage(m_pOperatorContourScanPage);
	}
	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & Scan_Lines_TAB) {
		m_pScanLinesPage = new CScanLinesPage();
		AddPage(m_pScanLinesPage);
	}
	if (theApp.m_LastSettings.bMotionToolsRRWIPTab) {
		m_pRRWIPPage = new CRRWIPPage();
		AddPage(m_pRRWIPPage);
	}
	if (theApp.m_LastSettings.bMotionToolsRotateThetaLiftTab) {
		m_pRotateThetaLiftPage = new CRotateThetaLiftPage();
		AddPage(m_pRotateThetaLiftPage);
	}
	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & Tubes_TAB) {
		m_pTubeScanPage = new CTubeScanPage();
		AddPage(m_pTubeScanPage);
	}
	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & TOF_CAL_TAB) {
		m_pThicknessCalibrationPage = new CThicknessCalibrationpage();
		AddPage(m_pThicknessCalibrationPage);
	}
	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & Rollers_TAB) {
		m_pRollerScanPage = new CRollerScanPage();
		AddPage(m_pRollerScanPage);
	}

	if (theApp.m_LastSettings.bExtTrigScanTab) {
		m_pExtTrigScanPage = new CExtTrigScan();
		AddPage(m_pExtTrigScanPage);
	}
	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & Jog_TAB) {
		switch (theApp.m_Tank.nScannerDescription) {
		default: m_pJogPage = new CJogPage();
			break;
		case RAILWAY_AXLE:m_pJogPage = new CJogPage(IDD_JOG_RAILWAY_AXLE_PAGE);
			break;
		}
		AddPage(m_pJogPage);
	}
	if (theApp.m_LastSettings.bMotionFlatScanTab) {
		m_pFlatScanPage = new CFlatScanPage();
		AddPage(m_pFlatScanPage);
	}

	if (theApp.m_LastSettings.bMotionBladeScanTab) {
		m_pRRBladeScanPage = new CRRBladeScanPage();
		AddPage(m_pRRBladeScanPage);
	}

	if (theApp.m_LastSettings.bMotionVerticalBoreScanTab) {
		m_pVerticalBoreScannerPage = new CVerticalBoreScannerPage();
		AddPage(m_pVerticalBoreScannerPage);
	}
	if (theApp.m_LastSettings.bMotionRollerProbeTab) {
		m_pRollerProbePage = new CRollerProbePage();
		AddPage(m_pRollerProbePage);
	}

	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & Robots_Position_TAB) {
		m_pRobotArmPositionsPage = new CRobotArmPositionsPage();
		AddPage(m_pRobotArmPositionsPage);
	}

	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & TRANSPLY_TAB) {
		m_pTransPlyPage = new CTransPlyPage();
		AddPage(m_pTransPlyPage);
	}
	if(theApp.m_LastSettings.nMotoionToolsTabMask[0] & PHASEDARRAY_LINE_SCAN_TAB) {
		m_pPhasedArrayLineScanPage = new CPhasedArrayLineScanPage();
		AddPage(m_pPhasedArrayLineScanPage);
	}
	if(theApp.m_LastSettings.bMotionAmplitudeTab) {
		m_pAmplitude = new CAmplitudePalettePage;
		AddPage(m_pAmplitude);
	}
	if(theApp.m_LastSettings.bMotionThicknessTab) {
		m_pThickness = new CThicknessPalettePage;
		AddPage(m_pThickness);
	}
	if(theApp.m_LastSettings.nMotoionToolsTabMask[0] & Robots_TAB) {
		m_pRobotUserPage = new CRobotUserPage;
		AddPage(m_pRobotUserPage);
	}
	if (theApp.m_LastSettings.nMotoionToolsTabMask[0] & DISCS_SHAFTS_TAB) {
		m_pDiscsShaftsPage = new CDiscsShaftsPage();
		AddPage(m_pDiscsShaftsPage);
	}


	SetActivePage(theApp.m_LastSettings.nLastMotionToolTab);

}

CMotionToolsSheet::CMotionToolsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CMotionToolsSheet::CMotionToolsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CMotionToolsSheet::~CMotionToolsSheet()
{
	SAFE_DELETE( m_pTurnTableScanPage);
	SAFE_DELETE( m_pNormalizePage);
	SAFE_DELETE( m_pPumpPage);
	SAFE_DELETE( m_pOriginPage);
	SAFE_DELETE( m_pMoveToPage);
	SAFE_DELETE( m_pSquirtersPage);
	SAFE_DELETE( m_pManipulatorSelectPage);
	SAFE_DELETE( m_pHoldingPage);
	SAFE_DELETE( m_pContourScanPage);
	SAFE_DELETE( m_pDetailsPage);
	SAFE_DELETE( m_pRectilinearScanPage);
	SAFE_DELETE( m_pLiftPage );
	SAFE_DELETE( m_pOriginForeplanePage );
	SAFE_DELETE( m_pOperatorContourScanPage );
	SAFE_DELETE( m_pScanLinesPage );
	SAFE_DELETE( m_pRRWIPPage );
	SAFE_DELETE( m_pRotateThetaLiftPage );
	SAFE_DELETE( m_pExtTrigScanPage );
	SAFE_DELETE( m_pTubeScanPage );
	SAFE_DELETE( m_pThicknessCalibrationPage );
	SAFE_DELETE( m_pJogPage );
	SAFE_DELETE( m_pFlatScanPage );
	SAFE_DELETE( m_pRollerScanPage );
	SAFE_DELETE( m_pRRBladeScanPage );
	SAFE_DELETE( m_pVerticalBoreScannerPage );
	SAFE_DELETE( m_pRollerProbePage );
	SAFE_DELETE( m_pRobotArmPositionsPage );
	SAFE_DELETE( m_pTransPlyPage );
	SAFE_DELETE( m_pAmplitude );
	SAFE_DELETE( m_pThickness );
	SAFE_DELETE( m_pRobotUserPage );
	SAFE_DELETE( m_pPhasedArrayLineScanPage );
	SAFE_DELETE( m_pDiscsShaftsPage);
}


BEGIN_MESSAGE_MAP(CMotionToolsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMotionToolsSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotionToolsSheet message handlers

void CMotionToolsSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void CMotionToolsSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_MOTIONTOOLS_SHEET]=FALSE;
	
	CPropertySheet::OnClose();
}

void CMotionToolsSheet::OnDestroy() 
{
	//Store Window Position
	if (IDD_MOTIONTOOLS_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_MOTIONTOOLS_SHEET]);

	CPropertySheet::OnDestroy();
	
	*m_pDlg	= NULL;	
}

BOOL CMotionToolsSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	if (IDD_MOTIONTOOLS_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_MOTIONTOOLS_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	
	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);

	SetTabNames();

	
	return bResult;
}

void CMotionToolsSheet::UpdateAllPages()
{

	if(m_pContourScanPage) m_pContourScanPage->UpdateAllControls();
	if(m_pRectilinearScanPage) m_pRectilinearScanPage->UpdateAllControls();
	if(m_pMoveToPage) {
		m_pMoveToPage->CalculateVelocityAndTemperature(1);
		m_pMoveToPage->UpdateAllControls();
	}
	if(m_pTurnTableScanPage) m_pTurnTableScanPage->UpdateAllControls();
	if(m_pNormalizePage) m_pNormalizePage->UpdateAllControls();
	if(m_pOperatorContourScanPage) m_pOperatorContourScanPage->UpdateAllControls();
	if(m_pScanLinesPage) m_pScanLinesPage->UpdateAllControls();
	if(m_pOriginPage) m_pOriginPage->UpdateAllControls();
	if(m_pSquirtersPage) m_pSquirtersPage->UpdateAllControls();
	if(m_pPumpPage) m_pPumpPage->UpdateAllControls();
	if(m_pTubeScanPage) m_pTubeScanPage->UpdateAllControls();
	if(m_pThicknessCalibrationPage) m_pThicknessCalibrationPage->UpdateAllControls();
	if(m_pFlatScanPage) m_pFlatScanPage->UpdateAllControls();
	if(m_pVerticalBoreScannerPage) m_pVerticalBoreScannerPage->UpdateAllControls();
	if(m_pRollerScanPage) m_pRollerScanPage->UpdateAllControls();
	if(m_pRobotArmPositionsPage) m_pRobotArmPositionsPage->UpdateAllControls();
	if(m_pTransPlyPage) m_pTransPlyPage->UpdateAllControls();
	if(m_pThickness) m_pThickness->UpdateAllControls();
	if(m_pAmplitude) m_pAmplitude->UpdateAllControls();
	if(m_pRobotUserPage) m_pRobotUserPage->UpdateAllControls();
	if(m_pDiscsShaftsPage) m_pDiscsShaftsPage->UpdateAllControls();


}

void CMotionToolsSheet::SetTabNames()
{
	if(m_pDetailsPage) {
		TCITEM TabCtrlItem;
		CTabCtrl* tab = GetTabControl();

		int nn=GetPageIndex(m_pDetailsPage);

		ZeroMemory(&TabCtrlItem,sizeof TabCtrlItem);
		tab->GetItem(nn,&TabCtrlItem);

		TabCtrlItem.pszText = (WCHAR *)LPCTSTR(theApp.m_DetailsWindowName);
		TabCtrlItem.mask=TCIF_TEXT;
		tab->SetItem(nn,&TabCtrlItem);
	}
}

void CMotionToolsSheet::KillAllTimers()
{
	CPropertyPage *pPage = GetActivePage();

	pPage->KillTimer(1);
}



void CMotionToolsSheet::OnRButtonDown(UINT nFlags, CPoint point)
{
	FRAME;

	pFrame->SendMessage(UI_SHOW_PROPERTIES_PANE,NULL, Tabs);

	CPropertySheet::OnRButtonDown(nFlags, point);
}
