// ViewSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ViewSheet.h"
#include "MainFrm.h"
#include "USLScans.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSheetf

IMPLEMENT_DYNAMIC(CViewSheet, CResizableSheet)

CViewSheet::CViewSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CResizableSheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CViewSheet::CViewSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg,LPCTSTR FilePath)
	:CResizableSheet(pszCaption, pParentWnd, iSelectPage)
{
	CUSLFile File;
	CFileException e;
	CString Buff,Language;

	m_pToolPathPage = NULL;
	m_pDlg = pDlg;
	
	m_pViewOptionsSheet = NULL;
	m_pHistogramDlg=NULL;
	m_pReportPrintDlg=NULL;
	m_pVolumeView=NULL;
	m_pViewUltrasonicsDlg=NULL;
	m_pViewStatsDlg=NULL;
	m_pViewCommentsDlg=NULL;
	m_pPrintConesCylindersDlg=NULL;
	m_pCADTableDlg=NULL;
	m_pViewThicknessCrossSectionDlg = NULL;
	m_pDataExportRulesDlg = NULL;
	m_pDataPropertiesDlg = NULL;
	m_pInclusionsSheet = NULL;

	m_FilePath = FilePath;
	m_nZoomFactor = 1000;
	m_rrSamples.SetRectEmpty();
	m_nToolOption = 0;
	m_bRenderAnnotation = false;
	m_bRenderPolygons = false;
	m_bRenderProfile = false;
	m_bRenderBrane = false;
	m_bRenderEnvelope = false;
	m_bRenderWaypoints = false;
	m_bRenderDirChangeX = false;
	m_bRenderDirChangeY = false;
	m_bRenderDirChangeZ = false;
	m_bRenderDirChangeXtip = false;
	m_bRenderDirChangeZtip = false;
	m_bRenderZeroXtip = false;
	m_bRenderZeroZtip = false;
	m_pDetailsPage = NULL;
	m_pXMLRulesDlg = NULL;
	m_nRectPolarDisplayMode = 0;
	m_pMenu = NULL;
	m_nScanLineDirection = HORIZONTAL;
	m_pCADSheet = NULL;
	m_pMosaicDlg = NULL;
	m_bCompatibleFile = false;


	m_bCtrlButton = false;
	m_bShiftButton = false;


	for(int nn=0;nn<256;nn++) {
		m_pViewPage[nn]=NULL;
		m_pViewChildPage[nn]=NULL;
	}

	if(FilePath!=NULL) {
		if(!File.Open(FilePath, CFile::modeRead | CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			Language.LoadString(IDS_Error);
			MessageBox(Buff,Language,MB_ICONERROR);
		} else {
			m_bCompatibleFile = m_Data.Retrieve(&File);
			File.Close();
			if(m_bCompatibleFile == true) {
				AllocatePages(m_Data.m_nScanType);
			}
		}
	}


}

CViewSheet::~CViewSheet()
{
	for(int nn=0;nn<256;nn++) {
		SAFE_DELETE( m_pViewPage[nn] );
		SAFE_DELETE( m_pViewChildPage[nn] );
	}
	SAFE_DELETE( m_pToolPathPage );
	SAFE_DELETE( m_pMenu );
	SAFE_DELETE( m_pCADSheet );
	SAFE_DELETE( m_pDataPropertiesDlg );
	SAFE_DELETE( m_pInclusionsSheet );
	SAFE_DELETE( m_pwndEditGeneralButton );

}


BEGIN_MESSAGE_MAP(CViewSheet, CResizableSheet)
	//{{AFX_MSG_MAP(CViewSheet)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_ZOOM_100, OnZoom100)
	ON_COMMAND(ID_ZOOM_50, OnZoom50)
	ON_COMMAND(ID_ZOOM_25, OnZoom25)
	ON_COMMAND(ID_ZOOM_10, OnZoom10)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_COMMAND(ID_TOOLS_RULER, OnToolsRuler)
	ON_COMMAND(ID_TOOLS_HISTOGRAM, OnToolsHistogram)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_TOOLS_POINTER, OnToolsPointer)
	ON_COMMAND(ID_TOOLS_VALUE, OnToolsValue)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_BUTTON_ZOOM_IN, OnButtonZoomIn)
	ON_COMMAND(ID_BUTTON_ZOOM_OUT, OnButtonZoomOut)
	ON_COMMAND(ID_BUTTON_ANNOTATION, OnButtonAnnotation)
	ON_COMMAND(ID_FILE_SAVEPAGEINBMP, OnFileSavepageinbmp)
	ON_COMMAND(ID_FILE_EXPORT_CSV, OnFileSavePageinCSV)
	ON_COMMAND(ID_FILE_DETAILS, OnFileDetails)
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_ULTRASONICS, OnFileUltrasonics)
	ON_COMMAND(ID_FILE_STATS, OnFileStats)
	ON_COMMAND(ID_ZOOM_1PX1SAMPLE,OnZoom1Pxto1Sample)
	ON_COMMAND(ID_FILE_XML_SAVEPAGE, OnFileXmlSavepage)
	ON_EN_CHANGE(IDC_EDIT_TEXT, OnChangeEditEditText)
	ON_COMMAND(ID_BUTTON_OVERLAY_PROFILE, OnButtonOverlayProfile)
	ON_COMMAND(ID_BUTTON_HORIZONTAL_SCAN_LINES, OnButtonHorizontalScanLines)
	ON_COMMAND(ID_BUTTON_VERTICAL_SCANLINES, OnButtonVerticalScanlines)
	ON_COMMAND(ID_TOOLS_POLYGON, OnToolsPolygon)
	ON_COMMAND(ID_BUTTON_PALETTE, OnButtonPalette)
	ON_COMMAND(ID_BUTTON_OVERLAY_BRANE, OnButtonOverlayBrane)
	ON_COMMAND(ID_BUTTON_SENTENCE, OnButtonSentence)
	ON_COMMAND(ID_BUTTON_COMMENTS, OnButtonComments)
	ON_UPDATE_COMMAND_UI(ID_VOLUME_VIEW, OnUpdateVolumeView)
	ON_COMMAND(ID_WAYPOINTS_VIEW, OnWaypointsView)
	ON_COMMAND(ID_DIAGNOSTICS_DIRECTIONCHANGE_X, OnDiagDirChangeX)
	ON_COMMAND(ID_DIAGNOSTICS_DIRECTIONCHANGE_Y, OnDiagDirChangeY)
	ON_COMMAND(ID_DIAGNOSTICS_DIRECTIONCHANGE_Z, OnDiagDirChangeZ)
	ON_COMMAND(ID_DIAGNOSTICS_DIRECTIONCHANGE_XTIP, OnDiagDirChangeXtip)
	ON_COMMAND(ID_DIAGNOSTICS_DIRECTIONCHANGE_ZTIP, OnDiagDirChangeZtip)
	ON_COMMAND(ID_DIAGNOSTICS_PASSZERO_XTIP,OnDiagZeroXtip)
	ON_COMMAND(ID_DIAGNOSTICS_PASSZERO_ZTIP,OnDiagZeroZtip)
	ON_COMMAND(ID_PRINT_11, OnPrint11)
	ON_COMMAND(ID_PRINT_12, OnPrint12)
	ON_COMMAND(ID_PRINT_14, OnPrint14)
	ON_COMMAND(ID_FILE_EXPORT_CIP, OnFileExportCIP)
	ON_COMMAND(ID_ENVELOPE_VIEW, OnEnvelopeView)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_BUTTON_ZOOM_VARIABLE, OnButtonZoomVariable)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_FILE_EXPORT_CLIPBOARD, OnFileExportClipboard)
	ON_COMMAND(ID_VIEW_CONESANDCYLINDERS, OnViewConesandcylinders)
	ON_COMMAND(ID_VIEW_REPORT, OnViewReport)
	ON_COMMAND(ID_VIEW_VOLUME, OnViewVolume)
	ON_COMMAND(ID_BUTTON_1TO1, OnButton1to1)
	ON_COMMAND(ID_BUTTON_TOOLS_RECTANGLE, OnButtonToolsRectangle)
	ON_COMMAND(ID_BUTTON_TOOLS_ELLIPSE, OnButtonToolsEllipse)
	ON_COMMAND(ID_BUTTON_TOOLS_LINE, OnButtonToolsLine)
	ON_COMMAND(ID_TOOLS_CROSSHAIR, OnToolsCrosshair)
	ON_COMMAND(ID_TOOLS_CURSOR, OnToolsCursor)
	ON_COMMAND(ID_BUTTON_TOOLS_PARALLELOGRAM, OnButtonToolsParallelogram)
	ON_COMMAND(ID_FILE_APPENDTOAMULTIPLESCANFILE, OnFileAppendtoamultiplescanfile)
	ON_COMMAND(ID_FILE_EXPORT_APTSOURCE,OnFileExportAptsource)
	ON_COMMAND(ID_DIAGNOSTICS_CREATEPROFILE, OnDiagnosticsCreateProfile)
	ON_COMMAND(ID_BUTTON_TOOLS_CENTRELINE, OnButtonToolsCentreLine)
	ON_COMMAND(ID_TOOLS_CROSS_SECTION, OnToolsCrossSection)
	ON_COMMAND(ID_WINDOW_NEW, OnWindowNew)
	ON_COMMAND(ID_WINDOW_ALLNEW, OnWindowAllnew)
	ON_COMMAND(ID_WINDOW_TILEHORIZONTALLY, OnWindowTilehorizontally)
	ON_COMMAND(ID_WINDOW_CLOSEALL, OnWindowCloseall)
	ON_COMMAND(ID_WINDOW_TILEVERTICALLY, OnWindowTilevertically)
	ON_COMMAND(ID_FILE_EXPORT_RULES, OnFileExportRules)
	ON_COMMAND(ID_BUTTON_PAL0, OnButtonPal0)
	ON_COMMAND(ID_BUTTON_PAL1, OnButtonPal1)
	ON_COMMAND(ID_BUTTON_PAL2, OnButtonPal2)
	ON_COMMAND(ID_BUTTON_PAL3, OnButtonPal3)
	ON_COMMAND(ID_BUTTON_PAL4, OnButtonPal4)
	ON_COMMAND(ID_BUTTON_PAL6, OnButtonPal6)
	ON_COMMAND(ID_BUTTON_PAL7, OnButtonPal7)
	ON_COMMAND(ID_BUTTON_PAL5, OnButtonPal5)
	ON_COMMAND(ID_FILE_PROPERTIES, OnFileProperties)
	ON_COMMAND(ID_FILE_MOSAIC, OnFileMosaic)
	ON_COMMAND(ID_BUTTON_SQUASH, OnButtonSquash)
	ON_WM_CHILDACTIVATE()
	ON_COMMAND(ID_VIEW_INCLUSIONS, OnViewInclusions)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSheet message handlers

void CViewSheet::PostNcDestroy()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(m_pDetailsPage) m_pDetailsPage->SendMessage(WM_CLOSE);

	if(theApp.m_pViewData == &m_Data) theApp.m_pViewData = NULL;
	if(theApp.m_pViewSheet == this) theApp.m_pViewSheet = NULL;

	if(pFrame->m_pScanView==this) {
		theApp.m_bCollectComplexData = FALSE;
		Sleep(500);
		pFrame->m_pScanView=NULL;
		theApp.m_pScanData=NULL;
		pFrame->SetAccessPrivelages();
		if(pFrame->m_pMotionToolsSheet) {
			pFrame->m_pMotionToolsSheet->UpdateAllPages();
		}
	}

	CResizableSheet::PostNcDestroy();
	delete this;
}

void CViewSheet::OnClose()
{
	
	CString Buff,Warning;

	if(m_Data.m_bModified==TRUE) {
		Buff.LoadString(IDS_Data_Not_Saved);
		Warning.LoadString(IDS_Warning);
		if (MessageBox(Buff, Warning, MB_YESNO) != IDYES) {
			return;
		}
	}

	for(int nn=0;nn<256;nn++) {
		SAFE_DELETE(m_pViewPage[nn]);
		SAFE_DELETE(m_pViewChildPage[nn]);
	}

	CResizableSheet::OnClose();
}
void CViewSheet::OnDestroy()
{
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_VIEW + m_nScanLineDirection]);

	CResizableSheet::OnDestroy();

	*m_pDlg	= NULL;
}

int CViewSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CRect rect;
	CMenu* pPopup = NULL;
	m_pMenu = new CMenu;
	m_pMenu->LoadMenu(IDR_VIEW_SHEET_MENU);

	m_pwndEditGeneralButton = new CMFCToolBarEditBoxButton(
		IDC_EDIT_GENERAL,
		GetCmdMgr()->GetCmdImage(IDC_EDIT_GENERAL, FALSE),
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 180);


	if (m_Data.m_nNumberVolumeImages == 0) {
		pPopup = m_pMenu->GetSubMenu(3);
		pPopup->EnableMenuItem(ID_VOLUME_VIEW, MF_GRAYED | MF_BYCOMMAND);
	}

	SetMenu(m_pMenu);

	if (CResizableSheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	CResizableSheet::m_bToolBar = TRUE;

	CMFCToolBar::EnableQuickCustomization();
	CList<UINT, UINT> lstBasicCommands;
	lstBasicCommands.AddTail(IDC_EDIT_GENERAL);
	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	m_wndArchiveToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_VIEW_ARCHIVE_TOOLBAR);
	m_wndArchiveToolBar.LoadToolBar(IDR_VIEW_ARCHIVE_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndArchiveToolBar.SetPaneStyle(m_wndArchiveToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndArchiveToolBar.SetPaneStyle(m_wndArchiveToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndArchiveToolBar.SetOwner(this);
	m_wndArchiveToolBar.SetRouteCommandsViaFrame(false);

	m_wndToolsToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_VIEW_TOOLS_TOOLBAR);
	m_wndToolsToolBar.LoadToolBar(IDR_VIEW_TOOLS_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndToolsToolBar.SetPaneStyle(m_wndToolsToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolsToolBar.SetPaneStyle(m_wndToolsToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolsToolBar.SetOwner(this);
	m_wndToolsToolBar.SetRouteCommandsViaFrame(false);


	CRect rrClient, rr, rectEdit;
	GetClientRect(rrClient);

	int cyTlb = m_wndArchiveToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndArchiveToolBar.SetWindowPos(NULL, rrClient.left, rrClient.top, m_wndArchiveToolBar.CalcFixedLayout(FALSE, TRUE).cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);			m_wndArchiveToolBar.GetWindowRect(rr);
	m_wndToolsToolBar.SetWindowPos(NULL, rr.right, rrClient.top, m_wndToolsToolBar.CalcFixedLayout(FALSE, TRUE).cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);	m_wndToolsToolBar.GetWindowRect(rr);


	EnableStackedTabs(FALSE);

	return 0;

}
/*
int CViewSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CRect rect;
	CMenu* pPopup = NULL;
	m_pMenu = new CMenu;
	m_pMenu->LoadMenu(IDR_VIEW_SHEET_MENU);

	if(m_Data.m_nNumberVolumeImages==0)  {
		pPopup = m_pMenu->GetSubMenu(3);
		pPopup->EnableMenuItem( ID_VOLUME_VIEW   , MF_GRAYED|MF_BYCOMMAND);
	}

	SetMenu(m_pMenu);

	if (CResizableSheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	CResizableSheet::m_bToolBar=TRUE;

	if (!m_wndArchiveToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndArchiveToolBar.LoadToolBar(IDR_VIEW_ARCHIVE_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolsToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndToolsToolBar.LoadToolBar(IDR_VIEW_TOOLS_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if(theApp.m_nJobNumber==RRFLATPACK) {
		if (!m_wndOrientationToolBar.CreateEx(this,
			TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
			WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
			CRect(2,2,0,0)) ||	!m_wndOrientationToolBar.LoadToolBar(IDR_VIEW_ORIENTATION_TOOLBAR)) {
				TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
	}
	if (!m_wndPaletteToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndPaletteToolBar.LoadToolBar(IDR_PALETTE_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndDialogBar.Create(this, IDD_VIEW_DIALOGBAR, CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_VIEW_DIALOGBAR);


	if (!m_wndRebar.Create(this,RBS_AUTOSIZE | RBS_VARHEIGHT,CBRS_ALIGN_TOP | AFX_IDW_DIALOGBAR)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	GetClientRect(rect);

	m_wndRebar.SetWindowPos(&wndTop   , 0,0,1040,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndArchiveToolBar);
	m_wndRebar.AddBar(&m_wndDialogBar);
	m_wndRebar.AddBar(&m_wndToolsToolBar);
	m_wndRebar.AddBar(&m_wndPaletteToolBar);
	if(m_wndOrientationToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndOrientationToolBar);

	EnableStackedTabs(FALSE);

	return 0;
}
*/

BOOL CViewSheet::OnInitDialog()
{
	
	CResizableSheet::OnInitDialog();

//	ModifyStyleEx(WS_EX_DLGMODALFRAME, WS_CAPTION);	// enable sys menu and icon

	// set minimal size
	CRect rc;
	GetWindowRect(&rc);

	SetMinTrackSize(CSize(GetMinWidth(), 170));

	if(theApp.m_LastSettings.nScanMask & _AUTO_1TO1) {
		One_to_One();
	} else {
		if (theApp.m_LastSettings.bWindowPos) {
			CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_VIEW + m_nScanLineDirection];
			if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
				theApp.ConfineToScreen(&rect);
				SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
			}
		}
	}

	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);
	SetTabNames();
	SetToolBarCheckedState();
	SetTimer(1,100,NULL);

	return TRUE;
}

void CViewSheet::AllocatePages(int nScanType)
{
	
	int	nn,nTS;
	int	nPage=0;
	int	nAmp=0;
	int	nTime=0;
	int	nPhase=0;
	int nExt=0;
	int gg,ii,jj=0;
	int nNumberTimeSlots;

	switch(nScanType) {
	default: nNumberTimeSlots = m_Data.m_nNumberTimeSlots;
		break;
	case MULTI_PROBE_SCAN:	nNumberTimeSlots = 1;
		break;
	}

	for(nTS=0;nTS<nNumberTimeSlots;nTS++) {
		for(nn=0;nn<m_Data.m_nNumberAmpImages[nTS];nn++,nPage++,nAmp++) {
			m_pViewPage[nPage] = new CViewPage(this,(CWnd**)&m_pViewPage[nPage],AMP_IMAGE,nAmp,&m_Data,nTS,nn,0);
			AddPage(m_pViewPage[nPage]);
			m_Name[nPage].Empty();

			m_Data.m_ImageName[nPage].Empty();

			if(m_Data.m_ImageName[nPage].IsEmpty()){
				if(theApp.m_LastSettings.nViewTabTSName) {
					m_Name[nPage] = m_Data.m_TS[nTS].cName;
				}
				if(theApp.m_LastSettings.nViewTabGateName) {
					for(gg=0,ii=0;gg<8;gg++) {
						if(m_Data.m_TS[nTS].Gate.nCollectAmp[gg]) {
							if(ii==nn) {
								m_Name[nPage] += " ";
								m_Name[nPage] += m_Data.m_TS[nTS].Gate.cName[gg];
							}
							ii++;
						}
					}
				}
				if(m_Name[nPage].IsEmpty())
					m_Name[nPage].Format(_T("Amp %d"),nAmp+1);
			}else{
				m_Name[nPage]=m_Data.m_ImageName[nPage];
			}
			m_Data.m_ImageName[nPage] = m_Name[nPage];

		}
		for(nn=0;nn<m_Data.m_nNumberTimeImages[nTS];nn++,nPage++,nTime++) {
			m_pViewPage[nPage] = new CViewPage(this,(CWnd**)&m_pViewPage[nPage],TIME_IMAGE,nTime,&m_Data,nTS,nn,0);
			AddPage(m_pViewPage[nPage]);
			m_Name[nPage].Format(_T("%s TOF:%d"),m_Data.m_TS[nTS].cName,nTime+1);
			m_Data.m_ImageName[nPage] = m_Name[nPage];
		}
		for(nn=0;nn<m_Data.m_nNumberPhaseImages[nTS];nn++,nPage++,nPhase++) {
			m_pViewPage[nPage] = new CViewPage(this,(CWnd**)&m_pViewPage[nPage],PHASE_IMAGE,nPhase,&m_Data,nTS,nn,0);
			AddPage(m_pViewPage[nPage]);
			m_Name[nPage].Format(_T("Phase %d"),nPhase+1);
			m_Data.m_ImageName[nPage] = m_Name[nPage];
		}
	}
	for(nn=0;nn<m_Data.m_nNumberExtImages;nn++,nPage++,nExt++) {
		m_pViewPage[nPage] = new CViewPage(this,(CWnd**)&m_pViewPage[nPage],EXT_IMAGE,nExt,&m_Data,nTS,nn,0);
		AddPage(m_pViewPage[nPage]);
		m_Name[nPage].Format(_T("Ext %d"),nExt + 1);
		m_Data.m_ImageName[nPage] = m_Name[nPage];
	}
//	m_pToolPathPage = new CToolPathPage();
//	AddPage(m_pToolPathPage);

}

void CViewSheet::SetTabNames()
{
	
	TCITEM TabCtrlItem;
	CTabCtrl* tab = GetTabControl();
	int	nn;

	for(int nPage = 0;nPage<256;nPage++) {
		if(m_pViewPage[nPage]) {
			nn=GetPageIndex(m_pViewPage[nPage]);
			ZeroMemory(&TabCtrlItem,sizeof TabCtrlItem);
			tab->GetItem(nn,&TabCtrlItem);
			TabCtrlItem.pszText = (WCHAR *)LPCTSTR(m_Name[nPage]);
			TabCtrlItem.mask=TCIF_TEXT;
			tab->SetItem(nn,&TabCtrlItem);
		} else {
			break;
		}
	}

}

void CViewSheet::InvalidateChild()
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;


	pPage->m_bPalette=TRUE;
	pPage->m_bRenderView=TRUE;
	pPage->m_bInitialize=TRUE;
	pPage->Invalidate(FALSE);

	if(m_pVolumeView!=NULL) {
		m_pVolumeView->m_bPalette=TRUE;
		m_pVolumeView->m_bRenderView=TRUE;
		m_pVolumeView->m_bInitialize=TRUE;
		m_pVolumeView->Invalidate(FALSE);
	}

	if(m_pPrintConesCylindersDlg) {
//		m_pPrintConesCylindersDlg->m_nPaletteNumber=pPage->m_nPaletteNumber;
//		m_pPrintConesCylindersDlg->Invalidate(FALSE);
	}

}
void CViewSheet::InvalidateDueToHistogramChange()
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;

	pPage->m_bPalette=TRUE;
	pPage->m_bRenderView=TRUE;
	pPage->m_bInitialize=TRUE;
	pPage->Invalidate(FALSE);

}

void CViewSheet::ChangePaletteOfActivePage(int nPaletteNumber)
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;

	pPage->m_nPaletteNumber = nPaletteNumber;
	pPage->m_bPalette=TRUE;
	pPage->m_bRenderView=TRUE;
	pPage->m_bInitialize=TRUE;
	pPage->Invalidate(FALSE);


}

void CViewSheet::InvalidateChildCommandedFromScanThread()
{
	
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;

	pPage->m_bPalette=FALSE;
	pPage->m_bRenderView=TRUE;
	pPage->m_bInitialize=FALSE;

	m_Data.m_nMinViewLine = theApp.m_Thread.m_nScanLine-1-PROFILE->m_nScanStartLine;
	m_Data.m_nMaxViewLine = theApp.m_Thread.m_nScanLine+1-PROFILE->m_nScanStartLine;
	pPage->Invalidate(FALSE);
	m_Data.m_nMinViewLine = 0;
	m_Data.m_nMaxViewLine = m_Data.m_nNumberLines;
}

void CViewSheet::OnZoom100()
{
	m_nZoomFactor=1000;
	InvalidateChild();
}

void CViewSheet::OnZoom50()
{
	m_nZoomFactor=500;
	InvalidateChild();
}

void CViewSheet::OnZoom25()
{
	m_nZoomFactor=250;
	InvalidateChild();
}

void CViewSheet::OnZoom10()
{
	m_nZoomFactor=100;
	InvalidateChild();
}

void CViewSheet::OnButtonZoomIn()
{
	m_nZoomFactor-=50;
	MinMax(&m_nZoomFactor,MIN_ZOOM_FACTOR,MAX_ZOOM_FACTOR);
	InvalidateChild();

}

void CViewSheet::OnButtonZoomOut()
{
	m_nZoomFactor+=50;
	MinMax(&m_nZoomFactor,MIN_ZOOM_FACTOR,MAX_ZOOM_FACTOR);

	InvalidateChild();
}

void CViewSheet::OnButtonZoomVariable() 
{
	m_nToolOption == TOOL_ZOOM_VARIABLE ? m_nToolOption = 0 : m_nToolOption = TOOL_ZOOM_VARIABLE;
	SetToolBarCheckedState();
	
}

void CViewSheet::OnToolsOptions()
{

	if (m_pViewOptionsSheet == NULL) {
		m_pViewOptionsSheet = new CViewOptionsSheet(_T("Options"),this,0,(CWnd**)&m_pViewOptionsSheet,&m_Data);
		m_pViewOptionsSheet->Create( this);
	} else {
		m_pViewOptionsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}

void CViewSheet::OnToolsCrosshair() 
{
	m_nToolOption == TOOL_RULER ? m_nToolOption = 0 : m_nToolOption = TOOL_RULER;
	SetToolBarCheckedState();
}

void CViewSheet::OnToolsRuler()
{
	m_nToolOption == TOOL_RULER ? m_nToolOption = 0 : m_nToolOption = TOOL_RULER;
	SetToolBarCheckedState();
}

void CViewSheet::OnButtonToolsRectangle() 
{
	m_nToolOption == TOOL_RECTANGLE ? m_nToolOption = 0 : m_nToolOption = TOOL_RECTANGLE;
	SetToolBarCheckedState();

	if (m_pCADTableDlg == NULL) {
		m_pCADTableDlg = new CCADTableDlg(this,(CWnd**)&m_pCADTableDlg,&m_Data);
		m_pCADTableDlg->Create(CCADTableDlg::IDD,this);
	} else {
		m_pCADTableDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewSheet::OnButtonToolsEllipse() 
{
	m_nToolOption == TOOL_ELLIPSE ? m_nToolOption = 0 : m_nToolOption = TOOL_ELLIPSE;
	SetToolBarCheckedState();

	if (m_pCADTableDlg == NULL) {
		m_pCADTableDlg = new CCADTableDlg(this,(CWnd**)&m_pCADTableDlg,&m_Data);
		m_pCADTableDlg->Create(CCADTableDlg::IDD,this);
	} else {
		m_pCADTableDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewSheet::OnButtonToolsLine() 
{
	m_nToolOption == TOOL_LINE ? m_nToolOption = 0 : m_nToolOption = TOOL_LINE;
	SetToolBarCheckedState();

	if (m_pCADTableDlg == NULL) {
		m_pCADTableDlg = new CCADTableDlg(this,(CWnd**)&m_pCADTableDlg,&m_Data);
		m_pCADTableDlg->Create(CCADTableDlg::IDD,this);
	} else {
		m_pCADTableDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}

void CViewSheet::OnButtonToolsParallelogram() 
{
	m_nToolOption == TOOL_PARALLELOGRAM ? m_nToolOption = 0 : m_nToolOption = TOOL_PARALLELOGRAM;
	SetToolBarCheckedState();

	if (m_pCADTableDlg == NULL) {
		m_pCADTableDlg = new CCADTableDlg(this,(CWnd**)&m_pCADTableDlg,&m_Data);
		m_pCADTableDlg->Create(CCADTableDlg::IDD,this);
	} else {
		m_pCADTableDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewSheet::OnButtonToolsCentreLine()
{
	m_nToolOption == TOOL_CENTRELINE ? m_nToolOption = 0 : m_nToolOption = TOOL_CENTRELINE;
	SetToolBarCheckedState();

	if (m_pCADTableDlg == NULL) {
		m_pCADTableDlg = new CCADTableDlg(this,(CWnd**)&m_pCADTableDlg,&m_Data);
		m_pCADTableDlg->Create(CCADTableDlg::IDD,this);
	} else {
		m_pCADTableDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}



}



void CViewSheet::OnToolsPolygon()
{
	CViewPage *pPage = (CViewPage*) GetActivePage();
	m_nToolOption = TOOL_POLYGON;
	m_bRenderPolygons = true;
	pPage->m_nPolyPtr = m_Data.m_nPolygonL;
	SetToolBarCheckedState();
}

void CViewSheet::OnButtonAnnotation()
{
	m_bRenderAnnotation ^= 1;
	SetToolBarCheckedState();
	InvalidateChild();

}

void CViewSheet::OnToolsHistogram()
{
	
	m_nToolOption = TOOL_HISTOGRAM;
	SetToolBarCheckedState();
	CViewPage* pPage=(CViewPage*)GetActivePage();
	int m_nImageType = pPage->m_nImageType;
	int	m_nImageNumber = pPage->m_nImageNumber;
	int	nPaletteNumber = pPage->m_nPaletteNumber;
	int nTS = pPage->m_nTS;
	CString Buff;

	if( theApp.m_LastSettings.nHistogramStyle == HISTOGRAM_CIRCLE_STYLE ) {
		Buff.Format(_T("%0.1fmm"), theApp.m_LastSettings.fHistogramCircleSize);
		EditTextSetWindowText(Buff);
	}



	if (m_pHistogramDlg == NULL) {
		m_pHistogramDlg = new CHistogramDlg(this,(CWnd**)&m_pHistogramDlg,m_nImageType,m_nImageNumber,nPaletteNumber,&m_Data,nTS);
		m_pHistogramDlg->Create(CHistogramDlg::IDD,this);
	} else {
		m_pHistogramDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		m_pHistogramDlg->m_nImageType = m_nImageType;
		m_pHistogramDlg->m_nImageNumber = m_nImageNumber;
		m_pHistogramDlg->m_nPaletteNumber = nPaletteNumber;
		m_pHistogramDlg->m_nTS = nTS;
	}

}

void CViewSheet::OnToolsValue()
{
	m_nToolOption = TOOL_VALUE;
	SetToolBarCheckedState();

}



void CViewSheet::InitializeFromProfile(int nScanType)
{
	
	CTime CurrentTime = CTime::GetCurrentTime();
	int nLine;
	CString Buff;

	if(PROFILE->m_nLineIncrementMode == VARIABLE_INC) {
		PROFILE->m_nLineIncrement = 1;
	}

	m_Data.m_fFastIncrement = PROFILE->m_fFastIncrement;
	m_Data.m_fSlowIncrement = PROFILE->m_fSlowIncrement * PROFILE->m_nLineIncrement;
	m_Data.m_nScanStartLine = PROFILE->m_nScanStartLine;
	m_Data.m_nScanFinishLine = PROFILE->m_nScanFinishLine;
	m_Data.m_nLineIncrement = PROFILE->m_nLineIncrement;
	m_Data.m_nLineIncrementMode = PROFILE->m_nLineIncrementMode;
	m_Data.m_nComponentDescription = PROFILE->m_nComponentDescription;
	m_Data.m_nScanType = PROFILE->m_nScanType = nScanType;
	m_Data.m_nCollectAxis = PROFILE->m_nCollectAxis;
	m_Data.m_nFlattenMode = PROFILE->m_nFlattenMode;
	m_Data.m_matCadToScan = PROFILE->m_matCadToScan;


	m_Data.m_CpTurnTableCentre.Side0.pt = theApp.m_Tank.vTTCentre;
	m_Data.vTTCentre = theApp.m_Tank.vTTCentre;
	if(nScanType==TURNTABLE_SCAN) {
		PROFILE->GetDiameters(m_Data.m_fComponentDiameter);
		m_Data.m_nFastAxis = 5;
	} else {
		m_Data.m_nFastAxis = PROFILE->m_nFastAxis;
		m_Data.m_fComponentDiameter[0] = PROFILE->m_fComponentDiameter[0];
		m_Data.m_fComponentDiameter[1] = PROFILE->m_fComponentDiameter[1];
	}
	m_Data.m_nSlowAxis = PROFILE->m_nSlowAxis;

	m_Data.m_nCollectScanLines = PROFILE->m_nCollectScanLines = FALSE;	//No longer used
	m_Data.m_nCollectSurface = PROFILE->m_nCollectSurface = FALSE;		//No longer used

	m_Data.m_nDetailsL=0;
	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		if(theApp.m_pDescription[nn].nCtrlType == 0) {
			m_Data.m_Details[m_Data.m_nDetailsL] = theApp.m_Data.m_Details[nn];
			m_Data.m_Caption[m_Data.m_nDetailsL] = *theApp.m_pDescription[nn].pCaption;
			m_Data.m_nDetailsL++;
		}
	}

	if(PROFILE->m_nEnvelopeLineL) {
		m_Data.m_pEnvelopeLine = new CPolyCoord[m_Data.m_nEnvelopeLineL = PROFILE->m_nEnvelopeLineL];
		for(nLine=0;nLine<m_Data.m_nEnvelopeLineL;nLine++) {
			m_Data.m_pEnvelopeLine[nLine] = &PROFILE->m_EnvelopeLine[nLine];
			m_Data.m_pEnvelopeLine[nLine].m_nOffOn =1;
			m_Data.m_pEnvelopeLine[nLine].m_rgb = 0;
		}
	}

	PROFILE->FindMaximumSamplesLine(&m_Data.m_nSamplesLine);
	PROFILE->FindNumberScanLines(&m_Data.m_nNumberLines);
	PROFILE->FindScanSizes(&m_Data.m_fFastScanSize,&m_Data.m_fSlowScanSize,&m_Data.m_CpStart,&m_Data.m_CpFinish);
	if(PROFILE->m_nCollectSurface ) PROFILE->AllocateSurfaceLine(m_Data.m_nSamplesLine / 10);
	m_Data.GenerateOneTimeOnlyLineLength();



	m_Data.m_TestDateTime = CurrentTime.Format(_T("%d %b %Y %Hh%M"));
	m_Data.m_osTestDateTime=(int)CurrentTime.GetTime();
	m_Data.m_ProfileName = PROFILE->m_FileName;
	m_Data.m_Operator = theApp.m_Operator;
	Buff.Empty();
	Buff =	theApp.GetProfileString(L"Maintenance",L"Machine Name",Buff.GetBuffer(1));
	m_Data.m_MachineName = Buff;
	Buff.Empty();
	Buff =	theApp.GetProfileString(L"Maintenance",L"Machine Location",Buff.GetBuffer(1));
	m_Data.m_MachineLocation = Buff;
	m_Data.m_nLogRange = theApp.m_LastSettings.nLogRange;

	if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
		m_Data.m_nNumberProbesPerImage = theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws();
	} else {
		if(PROFILE->m_nNumberProbesPerImage<=0) PROFILE->m_nNumberProbesPerImage=1;
		m_Data.m_nNumberProbesPerImage = PROFILE->m_nNumberProbesPerImage;
		m_Data.m_nFirstProbe = PROFILE->m_nFirstProbe;
	}

}

bool CViewSheet::InitializeFromUt(int nScanType)
{
	

	PROFILE->CopyGlobalUtToLocal();

	CopyMemory(m_Data.m_TS,theApp.m_UtUser.m_TS,sizeof theApp.m_UtUser.m_TS);
	CopyMemory(&m_Data.m_Global,&theApp.m_UtUser.m_Global,sizeof theApp.m_UtUser.m_Global);
	m_Data.CalculateNumberOfImages();
	if(m_Data.m_nNumberImages<=0) {
		MessageBox(L"No gates set to collect",L"Error", MB_ICONERROR);
		return false;
	}
	m_Data.AllocateImageBuffers();
	m_Data.AllocateVolumeBuffersAndOffsets();
	return true;
}

void CViewSheet::OnFileSaveas()
{

	if(m_Data.m_nNumberVolumeImages>0) {
		if(m_Data.m_bVolScansSaved==true) {
			if(m_Data.m_bCScansSaved == true) {
				MessageBox(L"File has volume data in it.\r\n It has already been saved automatically",L"Saved",MB_OK);
				return;
			}
		}
	}

	FileSaveas();

}


void CViewSheet::FileSaveas()
{
	
	CUSLFile File;
	CFileException e;
	CString Buff,FilePath;
	int nPos;

	if(m_Data.m_nNumberVolumeImages>0) {
		if(m_Data.m_bVolScansSaved==true) {
			if(m_Data.m_bCScansSaved != TRUE) {
				if(!File.Open(m_Data.m_FilePath, CFile::modeReadWrite | CFile::typeBinary, &e)) {
					File.DecodeFileException(&e,&Buff,m_Data.m_FilePath);
					MessageBox(Buff,_T("Error"),MB_ICONERROR);
					return;
				} else {
					m_Data.Save(&File);
					File.Close();
					m_Data.MESSaveOEM();
					Buff.Format(L"%s saved",m_Data.m_FileName);
					MessageBox(Buff,L"Saved",MB_OK);
				}
			}
			return;
		}
		return;
	}

	CString InitialFile = m_Data.m_FileName;
	if((nPos = m_Data.m_FileName.ReverseFind('.')) >0) {
		InitialFile = m_Data.m_FileName.Left(nPos);
	}


	CFileDialog FileDlg(FALSE,_T(".dat"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Image (*.dat)|*.dat|CIP (*.cip)|*.cip|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				CString path = FilePath.Left(nPos);
				theApp.m_StorePath[0] = path;
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate | CFile::typeBinary | CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			Buff = FileDlg.GetFileExt();
			if(Buff == _T("cip")) {
				m_Data.SaveCIPFile(&File);
			}else{
				m_Data.Save(&File);
			}
			File.Close();
			m_Data.MESSaveOEM();
		}
	}
	SetWindowTitle();
	SetToolBarCheckedState();

}

void CViewSheet::OnSave()
{

	OnFileSave();

}

void CViewSheet::OnFileSave()
{
	
	CString Buff;
	CUSLFile File;
	CFileException e;
	int nPos,ii;
	int nSaveCount=0;

	if(m_Data.m_FileName.IsEmpty()) {
		OnFileSaveas();
	} else {


		for(ii=0;ii<10;ii++) {
			if(theApp.m_LastSettings.nUseStorePath[ii] == 1) {

				Buff = m_Data.m_FileName;
				if((nPos = m_Data.m_FileName.ReverseFind('.')) >0) {
					Buff = m_Data.m_FileName.Left(nPos);
				}
				m_Data.m_FileName = Buff;
				m_Data.m_FilePath = theApp.m_StorePath[ii] + L"\\" + m_Data.m_FileName + L".dat";

				if(!File.Open(m_Data.m_FilePath, CFile::modeReadWrite | CFile::typeBinary, &e)) {
					File.DecodeFileException(&e,&Buff,m_Data.m_FilePath);
					MessageBox(Buff,_T("Error"),MB_ICONERROR);
					m_Data.AppendDataBase(false);
				} else {
					m_Data.Save(&File);
					File.Close();
					nSaveCount++;
				}
			}
		}
		m_Data.MESSaveOEM();

		if(nSaveCount==0) {
			MessageBox(L"The file has not been saved!\r\nEnsure the store paths are checked on",L"Error",MB_ICONERROR);
		} else {
			Buff.Format(L"File: %s has been saved",m_Data.m_FilePath);
			MessageBox(Buff,L"File saved",MB_OK);
		}

	}

}

void CViewSheet::OnToolsCursor() 
{
	CViewPage* pPage = (CViewPage*)GetActivePage();

	pPage->m_nToolOption = m_nToolOption = TOOL_CURSOR;
	SetToolBarCheckedState();
}

void CViewSheet::OnToolsPointer()
{
	m_nToolOption == TOOL_POINTER ? m_nToolOption = TOOL_CURSOR : m_nToolOption = TOOL_POINTER;
	SetToolBarCheckedState();
}

BOOL CViewSheet::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

   switch(nID) {
   case ID_TOOLS_RULER: strTipText.LoadString(IDS_Ruler);
	   break;
   case ID_TOOLS_POLYGON: strTipText.LoadString(IDS_Polygon);
	   break;
   case ID_TOOLS_HISTOGRAM: strTipText.LoadString(IDS_Histogram);
	   break;
   case ID_TOOLS_VALUE: strTipText.LoadString(IDS_Value);
	   break;
   case ID_TOOLS_OPTIONS: strTipText.LoadString(IDS_Options);
	   break;
   case ID_BUTTON_ZOOM_IN: strTipText.LoadString(IDS_Zoom_In_10);
	   break;
   case ID_BUTTON_ZOOM_OUT: strTipText.LoadString(IDS_Zoom_Out_10);
	   break;
   case ID_BUTTON_ZOOM_VARIABLE: strTipText.LoadString(IDS_Zoom_Variable);
	   break;
   case ID_FILE_SAVE: strTipText.LoadString(IDS_Save);
	   break;
   case ID_BUTTON_ANNOTATION: strTipText.LoadString(IDS_Annotation);
	   break;
   case ID_BUTTON_SENTENCE: strTipText.LoadString(IDS_Sentence);
	   break;
   case ID_BUTTON_PALETTE: strTipText.LoadString(IDS_Palette);
	   break;
   case ID_BUTTON_COMMENTS: strTipText.LoadString(IDS_Comments);
	   break;
   case ID_VIEW_REPORT: strTipText.LoadString(IDS_Print_Report);
	   break;
   case ID_VIEW_VOLUME: strTipText.LoadString(IDS_Volume);
	   break;
   case ID_BUTTON_TOOLS_LINE: strTipText.LoadString(IDS_Line);
	   break;
   case ID_BUTTON_TOOLS_RECTANGLE: strTipText.LoadString(IDS_Rectangle);
	   break;
   case ID_BUTTON_TOOLS_ELLIPSE: strTipText.LoadString(IDS_Ellipse);
	   break;
   case ID_BUTTON_TOOLS_PARALLELOGRAM: strTipText.LoadString(IDS_Parallelogram);
	   break;
	case ID_BUTTON_TOOLS_CENTRELINE: strTipText.LoadString(IDS_Centre_Line);
	   break;
	   
   }
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


void CViewSheet::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here

}


void CViewSheet::OnFileExportCIP(){

	
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = m_Data.m_FileName;
	if((nPos = m_Data.m_FileName.ReverseFind('.')) >0) {
		InitialFile = m_Data.m_FileName.Left(nPos);
	}

//	theApp.SuspendThreads();
	CFileDialog FileDlg(FALSE,_T(".cip"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("CIP (*.cip)|*.cip|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			m_Data.SaveCIPFile(&File);
			File.Close();
		}
	}
//	theApp.ResumeThreads();
	SetWindowTitle();
	SetToolBarCheckedState();


}



void CViewSheet::OnFileSavepageinbmp()
{
	#define NUMBERENTRIESPALETTE 0
	CDC* pDC = GetDC();
	CRect	rr;
	CDC  dcMem;
	CBitmap bmp;
	COLORREF *pArray;

//	CDialog* pDlg = (CDialog* )GetActiveFrame();
	GetClientRect(&rr);
	rr.top+=30;
	if(rr.Width()%8) rr.right=(((rr.Width()/8)+1)*8) + rr.left;

	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	dcMem.BitBlt(0,0,rr.Width(),rr.Height(),pDC,rr.left,rr.top,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
	bmp.GetBitmapBits(rr.Width()*rr.Height()*4,pArray);

	int m_SizePixels = rr.Width()*rr.Height()*(sizeof COLORREF);
	BITMAPFILEHEADER BmpFileHeader;

	char *Ptr = (char*)&BmpFileHeader.bfType;
	strcpy_s(Ptr,2,"BM");
	BmpFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) +
		                        sizeof(BITMAPINFOHEADER) +
								4*NUMBERENTRIESPALETTE     +
								m_SizePixels;

	BmpFileHeader.bfReserved1 = 0;
	BmpFileHeader.bfReserved2 = 0;
	BmpFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) +
		                        sizeof(BITMAPINFOHEADER) +
								4*NUMBERENTRIESPALETTE  ;


	BITMAPINFOHEADER BmpHeader;
	BmpHeader.biSize          = sizeof(BITMAPINFOHEADER);
	BmpHeader.biWidth         = rr.Width();
	BmpHeader.biHeight        = rr.Height();
	BmpHeader.biPlanes        = 1;
	BmpHeader.biBitCount      = 32;
	BmpHeader.biCompression   = 0;
	BmpHeader.biSizeImage     = m_SizePixels;
	BmpHeader.biXPelsPerMeter = 0;
	BmpHeader.biYPelsPerMeter = 0;
	BmpHeader.biClrUsed       = 0;
	BmpHeader.biClrImportant  = 0;

	CFileDialog FileDlg(FALSE,_T(".bmp"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("BitMap (*.bmp)|*.bmp|All Files (*.*)|*.*||"),NULL);
	if(FileDlg.DoModal()==IDOK) {

		CFile File;
		CFileException ExceptionInstance;
		int flag = File.Open(FileDlg.GetPathName(), CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &ExceptionInstance);

		File.Write(&BmpFileHeader, sizeof(BITMAPFILEHEADER));
		File.Write(&BmpHeader, sizeof(BITMAPINFOHEADER));

		char* Buff;
		int	yy;

		for(yy=rr.Height()-1;yy>=0;yy--) {
			Buff = (char *) &(pArray[yy*rr.Width()]);
			File.Write(Buff, rr.Width() * (sizeof COLORREF));
		};
		File.Close();

		ReleaseDC(pDC);
	}

	delete pArray;

}


void CViewSheet::OnFileExportClipboard() 
{
	CRect rr;
	CDC  dcMem;
	CBitmap bmpScreen;

	GetClientRect(rr);
	rr.top+=30;
	rr.right-=25;
	rr.bottom-=17;

	rr.DeflateRect(1,1);
	CDC *pDCScreen = GetDC();

	bmpScreen.CreateCompatibleBitmap(pDCScreen,rr.Width(),rr.Height());
	dcMem.CreateCompatibleDC(pDCScreen);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmpScreen);

	dcMem.BitBlt(0,0,rr.Width(),rr.Height(),pDCScreen,rr.left,rr.top,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);


	if ( OpenClipboard() )
	{
		EmptyClipboard();
		SetClipboardData(CF_BITMAP,bmpScreen.m_hObject);
		CloseClipboard();
	}

	ReleaseDC(pDCScreen);
	
}

void CViewSheet::SetToolBarCheckedState()
{
	
	int	nIndex;

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_ZOOM_VARIABLE))>=0)
		m_nToolOption==TOOL_ZOOM_VARIABLE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_TOOLS_POLYGON))>=0)
		m_nToolOption==TOOL_POLYGON ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_TOOLS_RULER))>=0)
		m_nToolOption==TOOL_RULER ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_TOOLS_HISTOGRAM))>=0)
		m_nToolOption==TOOL_HISTOGRAM ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_TOOLS_VALUE))>=0)
		m_nToolOption==TOOL_VALUE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_ANNOTATION))>=0)
		m_bRenderAnnotation==TRUE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_TOOLS_RECTANGLE))>=0)
		m_nToolOption==TOOL_RECTANGLE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_TOOLS_ELLIPSE))>=0)
		m_nToolOption==TOOL_ELLIPSE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_TOOLS_LINE))>=0)
		m_nToolOption==TOOL_LINE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_TOOLS_PARALLELOGRAM))>=0)
		m_nToolOption==TOOL_PARALLELOGRAM ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_TOOLS_CENTRELINE))>=0)
		m_nToolOption==TOOL_CENTRELINE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_TOOLS_CROSS_SECTION))>=0)
		m_nToolOption==TOOL_CROSS_SECTION ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);


	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_VIEW_VOLUME))>=0) {
		((m_pVolumeView!=NULL) &&  (m_Data.m_bHeaderForVolumeIsValid==TRUE)) ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
		if(m_Data.m_nNumberVolumeImages==0)  {
			m_wndToolsToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
//			m_wndToolsToolBar.GetToolBarCtrl().SetState(ID_VOLUME_VIEW,0);
		}
	}
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_WAYPOINTS_VIEW))>=0)
		m_bRenderWaypoints==TRUE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_ENVELOPE_VIEW))>=0)
		m_bRenderEnvelope==TRUE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

	if(m_wndOrientationToolBar.m_hWnd) {
		if((nIndex=m_wndOrientationToolBar.CommandToIndex(ID_BUTTON_OVERLAY_PROFILE))>=0)
			m_bRenderProfile==TRUE ? m_wndOrientationToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndOrientationToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndOrientationToolBar.CommandToIndex(ID_BUTTON_OVERLAY_BRANE))>=0)
			m_bRenderBrane==TRUE ? m_wndOrientationToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndOrientationToolBar.SetButtonStyle(nIndex,0);
	}
	if(m_wndPaletteToolBar.m_hWnd) {
		if((nIndex=m_wndPaletteToolBar.CommandToIndex(ID_BUTTON_SQUASH))>=0)
			theApp.m_LastSettings.nThresholdMode > 0 ? m_wndPaletteToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPaletteToolBar.SetButtonStyle(nIndex,0);
	}

}


void CViewSheet::OnUpdateVolumeView(CCmdUI* pCmdUI)
{
	
/*
	if(m_Data.m_nNumberVolumeImages==0)  {
		pCmdUI->Enable(FALSE);
	}
*/
}

void CViewSheet::OnFileDetails()
{

	if(m_pDetailsPage) delete m_pDetailsPage;

	m_pDetailsPage = new CDetailsPage((CWnd**)&m_pDetailsPage,&m_Data);
	m_pDetailsPage->Create(CDetailsPage::IDD);

}

BOOL CViewSheet::PreTranslateMessage(MSG* pMsg)
{
	double dTemp;
	

	if(pMsg->message==WM_KEYDOWN) {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		
		CViewPage* pPage=(CViewPage*)GetActivePage();
		int m_nImageType = pPage->m_nImageType;
		int	m_nPaletteNumber = pPage->m_nPaletteNumber;
		int m_nImageNumber = pPage->m_nImageNumber;


		switch(m_nImageType) {
		case AMP_IMAGE:
			switch(pMsg->wParam) {
			case 'w':
			case 'W':
				switch(m_nToolOption) {
				case TOOL_HISTOGRAM:
					if(m_Data.RemoveWaterSplash(m_nImageType,m_nImageNumber,pPage->m_rrHistogramSample) == TRUE) {
						InvalidateChild();
						return true;
					}
					break;
				}
				break;
			case CURSOR_LEFT:
				dTemp=theApp.m_Palette.m_ddBRange[m_nPaletteNumber]/theApp.m_Palette.m_ddBStepSize[m_nPaletteNumber];
				theApp.m_Palette.m_ddBStepSize[m_nPaletteNumber]--;
				if(theApp.m_Palette.m_ddBStepSize[m_nPaletteNumber]<1.0) theApp.m_Palette.m_ddBStepSize[m_nPaletteNumber] = 1.0;
				theApp.m_Palette.m_ddBRange[m_nPaletteNumber]=dTemp*theApp.m_Palette.m_ddBStepSize[m_nPaletteNumber];
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
				InvalidateChild();
				return TRUE;
				break;
			case CURSOR_RIGHT:
				dTemp=theApp.m_Palette.m_ddBRange[m_nPaletteNumber]/theApp.m_Palette.m_ddBStepSize[m_nPaletteNumber];
				theApp.m_Palette.m_ddBStepSize[m_nPaletteNumber]++;
				if(theApp.m_Palette.m_ddBStepSize[m_nPaletteNumber]>10.0) theApp.m_Palette.m_ddBStepSize[m_nPaletteNumber] = 10.0;
				theApp.m_Palette.m_ddBRange[m_nPaletteNumber]=dTemp*theApp.m_Palette.m_ddBStepSize[m_nPaletteNumber];
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
				InvalidateChild();
				return TRUE;
				break;
			case CURSOR_UP:
				theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber]++;
				if(theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber]>m_Data.m_nLogRange) theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber] = m_Data.m_nLogRange;
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
				InvalidateChild();
				return TRUE;
				break;
			case CURSOR_DOWN:
				theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber]--;
				if(theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber]<-90.0) theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber] = -90.0;
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
				InvalidateChild();
				return TRUE;
				break;
			case PGUP:
				theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber]+=10;
				if(theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber]>m_Data.m_nLogRange) theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber] = m_Data.m_nLogRange;
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
				InvalidateChild();
				return TRUE;
				break;
			case PGDOWN:
				theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber]-=10;
				if(theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber]<-90.0) theApp.m_Palette.m_ddBTopLevel[m_nPaletteNumber] = -90.0;
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nPaletteNumber);
				InvalidateChild();
				return TRUE;
				break;
			};
			break;
		case TIME_IMAGE:
			double dStep = theApp.m_Palette.m_dStepSize[0];
			switch(pMsg->wParam) {
			case CURSOR_LEFT:
				theApp.m_Palette.m_dThicknessRange[0]-=dStep;
				if(theApp.m_Palette.m_dThicknessRange[0]<(dStep*10)) theApp.m_Palette.m_dThicknessRange[0] = (dStep*10.0);
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nImageNumber);
				InvalidateChild();
				return TRUE;
				break;
			case CURSOR_RIGHT:
				theApp.m_Palette.m_dThicknessRange[0]+=dStep;
		//		if(theApp.m_Palette.m_dThicknessRange>theApp.m_Palette.m_dMaxThickness) theApp.m_Palette.m_dThicknessRange = theApp.m_Palette.m_dMaxThickness;
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nImageNumber);
				InvalidateChild();
				return TRUE;
				break;
			case CURSOR_UP:
				theApp.m_Palette.m_dMinThickness[0]+=dStep;
	//			if(theApp.m_Palette.m_dMinThickness>dStep) theApp.m_Palette.m_dMinThickness=dStep;
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nImageNumber);
				InvalidateChild();
				return TRUE;
				break;
			case CURSOR_DOWN:
				theApp.m_Palette.m_dMinThickness[0]-=dStep;
				if(theApp.m_Palette.m_dMinThickness[0]<0.0) theApp.m_Palette.m_dMinThickness[0]=0.0;
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nImageNumber);
				InvalidateChild();
				return TRUE;
				break;
			case PGUP:
				theApp.m_Palette.m_dMinThickness[0]+=(dStep*10.0);
				if(theApp.m_Palette.m_dMinThickness[0]>theApp.m_Palette.m_dMaxThickness[0]) theApp.m_Palette.m_dMinThickness[0]=theApp.m_Palette.m_dMaxThickness[0];
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nImageNumber);
				InvalidateChild();
				return TRUE;
				break;
			case PGDOWN:
				theApp.m_Palette.m_dMinThickness[0]-=(dStep*10.0);
				if(theApp.m_Palette.m_dMinThickness[0]<0.0) theApp.m_Palette.m_dMinThickness[0]=0.0;
				theApp.m_Palette.CalculatePalette();
				pFrame->ChangePalletteNumber(m_nImageType,m_nImageNumber);
				InvalidateChild();
				return TRUE;
				break;
			};
			break;
		};



	};

	return CResizableSheet::PreTranslateMessage(pMsg);
}



void CViewSheet::SetWindowTitle()
{
	CString	Buff,Temp;
	int nPos;

	Buff = m_Data.m_FileName;
	if((nPos=Buff.ReverseFind('.')) > -1) {
		Buff.GetBufferSetLength(nPos);
	}

	if(m_Data.m_bModified) {
		Buff += " (Not Saved)";
	}

	Temp.Format(_T(" Profile: %s"),m_Data.m_ProfileName);

	Buff += Temp;
	SetWindowText(Buff);
}

void CViewSheet::OnTimer(UINT nIDEvent)
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;

	if(m_Data.m_bThreadFinished == true) {

		pPage->m_bPalette=TRUE;
		pPage->m_bRenderView=TRUE;
		pPage->m_bInitialize=TRUE;
		pPage->Invalidate(FALSE);
	}


	SetWindowTitle();
	CResizableSheet::OnTimer(nIDEvent);
}

void CViewSheet::RefreshViewOptionsSheet()
{
	if(m_pViewOptionsSheet)
		m_pViewOptionsSheet->UpdateAllPages();
}


void CViewSheet::OnFileUltrasonics()
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;

	if (m_pViewUltrasonicsDlg == NULL) {
		m_pViewUltrasonicsDlg = new CViewUltrasonicsDlg(this, (CWnd**)&m_pViewUltrasonicsDlg,&m_Data,pPage->m_nTS,pPage->m_nImageType,pPage->m_nImageNumber);
		m_pViewUltrasonicsDlg->Create(CViewUltrasonicsDlg::IDD,this);
	} else {
		m_pViewUltrasonicsDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}


void CViewSheet::OnFileStats()
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;

	if (m_pViewStatsDlg == NULL) {
		m_pViewStatsDlg = new CViewStatsDlg(this, (CWnd**)&m_pViewStatsDlg,&m_Data);
		m_pViewStatsDlg->Create(CViewStatsDlg::IDD,this);
	} else {
		m_pViewStatsDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}

void CViewSheet::OnFileExportRules() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;

	if (m_pXMLRulesDlg == NULL) {
		m_pXMLRulesDlg = new CXMLRulesDlg(this, (CWnd**)&m_pXMLRulesDlg,&m_Data,pPage->m_nImageType,pPage->m_nImageNumber);
		m_pXMLRulesDlg->Create(CXMLRulesDlg::IDD,this);
	} else {
		m_pXMLRulesDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}



void CViewSheet::OnFileXmlSavepage()
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	int nImageType = pPage->m_nImageType;
	int	nImageNumber = pPage->m_nImageNumber;
	int	nPaletteNumber = pPage->m_nPaletteNumber;
	int	nTS = pPage->m_nTS;
	int nAttemptNo = 1;

	CFileDialog FileDlg(FALSE,_T(".xml"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("XML (*.xml)|*.xml|All Files (*.*)|*.*||"),NULL);
	if(FileDlg.DoModal()==IDOK) {

		CUSLFile File;
		CFileException ExceptionInstance;
		int flag = File.Open(FileDlg.GetPathName(), CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate, &ExceptionInstance);
		File.SetLength(0);

		switch(nAttemptNo) {
		case 0:	m_Data.SaveXMLAttempt0(&File,nTS,nImageType,nImageNumber);
			break;
		case 1:	m_Data.SaveXMLAttempt1(&File,nTS,nImageType,nImageNumber);
			break;
		}


		File.Close();
	}
}


void CViewSheet::OnFileSavePageinCSV()
{

	CViewPage* pPage=(CViewPage*)GetActivePage();
	int nImageType = pPage->m_nImageType;
	int	nImageNumber = pPage->m_nImageNumber;
	int	nTS = pPage->m_nTS;
	int nAttemptNo = 1;

	CFileDialog FileDlg(FALSE,_T(".csv"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("CSV (*.csv)|*.csv|All Files (*.*)|*.*||"),NULL);
	if(FileDlg.DoModal()==IDOK) {

		CUSLFile File;
		CFileException ExceptionInstance;
		int flag = File.Open(FileDlg.GetPathName(), CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate, &ExceptionInstance);
		File.SetLength(0);
		m_Data.SaveCSV(&File,nTS,nImageType,nImageNumber);

		File.Close();
	}

}




/***
Currently this just imports the Vertexes at Scan Lines
***/

void CViewSheet::OnDiagnosticsCreateProfile()
{

	
	int nLine;
	CString Buff,Warning;

	Buff.LoadString(IDS_Any_unsaved_changes_to_the_current_profile_will_be_lost_Continue);
	Warning.LoadString(IDS_Warning);
	
	if(MessageBox(Buff,Warning,MB_YESNO) != IDYES)
		return;

	if(m_Data.m_nNumberVertexLines<=0)
		return;

	if(PROFILE->CheckNewProfileAllowed()!=TRUE) 
		return;

	PROFILE->New();

	PROFILE->m_nScanLineL = m_Data.m_nNumberVertexLines;

	PROFILE->m_ScanLine = new CPolyCoord[PROFILE->m_nScanLineL];

	for(nLine = 0;nLine < PROFILE->m_nScanLineL; nLine++) {
		m_Data.m_vLineSample[nLine].GetVertices(PROFILE->m_ScanLine[nLine],400);
	}

}



void CViewSheet::OnZoom1Pxto1Sample()
{
	
	CRect rrSheet,rrStatic, rrBorders;
	CDC* pDC = GetDC();
	double fPixelsX, fPixelsY;
	double  PixelsWide,PixelsHigh;
	int cx,cy;

	GetWindowRect(&rrSheet);
	if(m_pViewPage[0]) {
		m_pViewPage[0]->m_staticView.GetWindowRect(rrStatic);
		rrBorders.top = rrStatic.top - rrSheet.top;
		rrBorders.bottom = rrSheet.bottom - rrStatic.bottom;
		rrBorders.left = rrStatic.left - rrSheet.left;
		rrBorders.right = rrSheet.right - rrStatic.right;
	}


	PixelsWide = (double)m_Data.m_nSamplesLine;// * m_Data.m_fFastIncrement;//*fPixelsmmX;
	PixelsHigh = (double)m_Data.m_nNumberLines;// * m_Data.m_fSlowIncrement;//*fPixelsmmY;


	cx = (int)PixelsWide + rrBorders.left + rrBorders.right;
	cy = (int)PixelsHigh + rrBorders.top + rrBorders.bottom;

	fPixelsX	= ((double)pDC->GetDeviceCaps(HORZRES)); //480 is the screen width in cm
	fPixelsY	= ((double)pDC->GetDeviceCaps(VERTRES)); //300 is the screen height in cm

	if((cx>fPixelsX)){
		MessageBox(_T("Error"),_T("Horizontal Screen resolution great enough"),NULL);
		return;
	}

	if((cy>fPixelsY)){
		MessageBox(_T("Vertical Screen resolution great enough"),_T("Error"),NULL);
		return;
	}

	m_nZoomFactor=1000;

	this->SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE);

	ReleaseDC(pDC);
	InvalidateChild();


}




void CViewSheet::OnButtonHorizontalScanLines()
{
	
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_VIEW + m_nScanLineDirection]);
	m_nScanLineDirection = HORIZONTAL;
	CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_VIEW + m_nScanLineDirection];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
}

void CViewSheet::OnButtonVerticalScanlines()
{
	
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_VIEW + m_nScanLineDirection]);
	m_nScanLineDirection = VERTICAL;
	CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_VIEW + m_nScanLineDirection];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
}


void CViewSheet::OnButtonPalette()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->CallProtectedPaletteEdit();

}

void CViewSheet::OnButtonOverlayProfile()
{
	m_bRenderProfile == false ? m_bRenderProfile = true : m_bRenderProfile = false;
	if(m_bRenderProfile)
		m_bRenderBrane=false;
	SetToolBarCheckedState();
	InvalidateChild();
}

void CViewSheet::OnButtonOverlayBrane()
{
	m_bRenderBrane == false ? m_bRenderBrane = true : m_bRenderBrane = false;
	if(m_bRenderBrane)
		m_bRenderProfile=false;
	SetToolBarCheckedState();
	InvalidateChild();

}

void CViewSheet::OnButtonSentence()
{
	CSentenceDlg dlg(this,&m_Data);

	dlg.DoModal();
}

void CViewSheet::OnButtonComments()
{

	if (m_pViewCommentsDlg == NULL) {
		m_pViewCommentsDlg = new CViewCommentsDlg(this, (CWnd**)&m_pViewCommentsDlg,&m_Data);
		m_pViewCommentsDlg->Create(CViewCommentsDlg::IDD,this);
	} else {
		m_pViewCommentsDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewSheet::CreateImageProcessPage(int nAction,int nSrcType, int nSrc, int nDest)
{
	int nPage;

	switch(nAction) {
	case 0:	m_Data.FilterAmpBufferWithConvolutionFilter(nSrcType, nSrc,nDest);
		break;
	case 1:	m_Data.DifferentiateAmpBuffer(nSrcType, nSrc,nDest);
		break;
	case 2:	m_Data.SobelEdgeFilter(nSrcType, nSrc,nDest);
		break;
	case 3:	m_Data.MedianFilter3X3(nSrcType, nSrc,nDest);
		break;
	case 4:	m_Data.MeanFilter3X3(nSrcType, nSrc,nDest);
		break;
	}

	for(nPage=0;nPage < 256;nPage++) {
		if(m_pViewPage[nPage]) {
			if(m_pViewPage[nPage]->m_nImageType == FILTER_IMAGE) {
				if(m_pViewPage[nPage]->m_nImageNumber == nDest) {
					m_pViewPage[nPage]->m_bRenderView=true;
					SetActivePage(nPage);
					m_pViewPage[nPage]->Invalidate(false);
					goto page_found;
				}
			}
		} else {
			m_pViewPage[nPage] = new CViewPage(this,(CWnd**)&m_pViewPage[nPage],FILTER_IMAGE,nDest,&m_Data,0,0,0);

			AddPage(m_pViewPage[nPage]);
			SetActivePage(nPage);
			goto page_found;
		}

	}
page_found:

	m_Name[nPage].Format(_T("(Filtered) %s"),	m_Name[nSrc]);
	SetTabNames();

}




void CViewSheet::OnWaypointsView()
{

	m_bRenderWaypoints == false ? m_bRenderWaypoints = true : m_bRenderWaypoints = false;
	SetToolBarCheckedState();
	InvalidateChild();

}

void CViewSheet::OnEnvelopeView() 
{
	m_bRenderEnvelope == false ? m_bRenderEnvelope = true : m_bRenderEnvelope = false;
	SetToolBarCheckedState();
	InvalidateChild();
}

void CViewSheet::OnDiagDirChangeX()
{
	m_bRenderDirChangeX == false ? m_bRenderDirChangeX = true : m_bRenderDirChangeX = false;
	SetToolBarCheckedState();
	InvalidateChild();

}

void CViewSheet::OnDiagDirChangeY()
{
	m_bRenderDirChangeY == false ? m_bRenderDirChangeY = true : m_bRenderDirChangeY = false;
	SetToolBarCheckedState();
	InvalidateChild();

}

void CViewSheet::OnDiagDirChangeZ()
{
	m_bRenderDirChangeZ == false ? m_bRenderDirChangeZ = true : m_bRenderDirChangeZ = false;
	SetToolBarCheckedState();
	InvalidateChild();

}

void CViewSheet::OnDiagDirChangeXtip()
{
	m_bRenderDirChangeXtip == false ? m_bRenderDirChangeXtip = true : m_bRenderDirChangeXtip = false;
	SetToolBarCheckedState();
	InvalidateChild();

}

void CViewSheet::OnDiagDirChangeZtip()
{
	m_bRenderDirChangeZtip == false ? m_bRenderDirChangeZtip = true : m_bRenderDirChangeZtip = false;
	SetToolBarCheckedState();
	InvalidateChild();

}

void CViewSheet::OnDiagZeroXtip()
{
	m_bRenderZeroXtip == false ? m_bRenderZeroXtip = true : m_bRenderZeroXtip = false;
	SetToolBarCheckedState();
	InvalidateChild();
}

void CViewSheet::OnDiagZeroZtip()
{
	m_bRenderZeroZtip == false ? m_bRenderZeroZtip = true : m_bRenderZeroZtip = false;
	SetToolBarCheckedState();
	InvalidateChild();
}



void CViewSheet::OnPrint11()
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;

	pPage->PrintWusiWug(1.0f);
}

void CViewSheet::OnPrint12()
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;

	pPage->PrintWusiWug(0.5f);

}

void CViewSheet::PrintWusiWug(float fScale)
{

}

void CViewSheet::OnPrint14()
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	if(pPage==NULL) return;

	pPage->PrintWusiWug(0.25f);
}




BOOL CViewSheet::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	
	CViewPage* pPage = (CViewPage*)GetActivePage();

	pPage->MouseWheel(nFlags, zDelta, pt);
	
	return CResizableSheet::OnMouseWheel(nFlags, zDelta, pt);
}


void CViewSheet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CResizableSheet::OnActivate(nState, pWndOther, bMinimized);
	
	
	switch(nState) {
	case 0:
		break;
	default:
		theApp.m_pViewData = &m_Data;
		theApp.m_pViewSheet = this;
		break;
	}
	
}


void CViewSheet::OnViewConesandcylinders() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	int m_nImageType = pPage->m_nImageType;
	int	m_nImageNumber = pPage->m_nImageNumber;
	int	m_nPaletteNumber = pPage->m_nPaletteNumber;


	if (m_pPrintConesCylindersDlg == NULL) {
		m_pPrintConesCylindersDlg = new CPrintConesCylindersDlg(this,(CWnd**)&m_pPrintConesCylindersDlg,m_nImageType,m_nImageNumber,m_nPaletteNumber,&m_Data);
		m_pPrintConesCylindersDlg->Create(CPrintConesCylindersDlg::IDD,this);
	} else {
		m_pPrintConesCylindersDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		m_pPrintConesCylindersDlg->m_nImageType = m_nImageType;
		m_pPrintConesCylindersDlg->m_nImageNumber = m_nImageNumber;
	}

}

void CViewSheet::OnViewReport() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	int m_nImageType = pPage->m_nImageType;
	int	m_nImageNumber = pPage->m_nImageNumber;

	if (m_pReportPrintDlg == NULL) {
		m_pReportPrintDlg = new CReportPrintDlg(this,(CWnd**)&m_pReportPrintDlg,pPage,&m_Data);
		m_pReportPrintDlg->Create(CReportPrintDlg::IDD,this);
	} else {
		m_pReportPrintDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewSheet::OnViewVolume() 
{
	int lRet;

	if(m_Data.m_nNumberVolumeImages<=0) {
		MessageBox(_T("There is no volume data in the file"),_T("Error"),MB_ICONERROR);
		return;
	}

	FileSaveas();

	if (m_pVolumeView == NULL) {
		m_pVolumeView = new CVolumeViewDlg(this,(CWnd**)&m_pVolumeView,&m_Data,0,0);
		lRet = m_pVolumeView->Create(CVolumeViewDlg::IDD,this);
	} else {
		m_pVolumeView->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
//	SetToolBarCheckedState();
}

void CViewSheet::OnButton1to1() 
{

	One_to_One();

}

void CViewSheet::One_to_One() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRect rrMainFrm,rr;
	float fFastScanSize,fFastIncrement;
	int nX[2];
	int nXTranslate,nYTranslate,nY;
	float fScanRatio;

	int nHPixel = GetSystemMetrics(78);
	int nVPixel = GetSystemMetrics(79);
	int	nNumberMonitors = GetSystemMetrics(80);

	pFrame->GetClientRect(rrMainFrm);
	if(nNumberMonitors>1) {
		switch(theApp.m_LastSettings.n1to1Monitor) {
		case 0: 
		case 2:
			if(pFrame->m_pwndToolBar[0]) {
				pFrame->m_pwndToolBar[0]->GetWindowRect(&rr);
				rrMainFrm.top = rr.bottom;
				rrMainFrm.bottom += rr.Height();
			}
			if(pFrame->m_wndStatusBar) {
				pFrame->m_wndStatusBar.GetWindowRect(&rr);
				rrMainFrm.bottom = rr.top;
			}
			break;
		case 1:
			rrMainFrm.top = 0;
			rrMainFrm.bottom = nVPixel;
			break;
		}
	} else {
		if(pFrame->m_pwndToolBar[0]) {
			pFrame->m_pwndToolBar[0]->GetWindowRect(&rr);
			rrMainFrm.top = rr.bottom;
			rrMainFrm.bottom += rr.Height();
		}
	}

	CRect rrWindow,rrView;
	GetWindowRect(&rrWindow);
	CViewPage *pPage = (CViewPage*)GetActivePage();
	pPage->m_staticView.GetWindowRect(&rrView);

	int nLeftBorder = abs(rrView.left - rrWindow.left);
	int	nRightBorder = abs(rrView.right - rrWindow.right);
	int nTopBorder = abs(rrView.top - rrWindow.top);
	int nBottomBorder = abs(rrView.bottom - rrWindow.bottom);

	rrWindow = rrMainFrm;

	nX[0]=0;
	nX[1]=nHPixel;
	nY=rrWindow.bottom;
	if(nNumberMonitors>1) {
		switch(theApp.m_LastSettings.n1to1Monitor) {
		case 0: 
			nX[0] = rrWindow.left = 0;
			nX[1] = rrWindow.right = nHPixel / 2;
			break;
		case 1:
			nX[0] = rrWindow.left = nHPixel / 2;
			nX[1] = rrWindow.right = nHPixel-1;
			break;
		case 2:
			nX[0] = rrWindow.left = 0;
			nX[1] = rrWindow.right = nHPixel-1;
			break;
		}
	}
	rrView.left =	rrWindow.left + nLeftBorder;
	rrView.right =	rrWindow.right - nRightBorder;
	rrView.top =	rrWindow.top + nTopBorder;
	rrView.bottom =	rrWindow.bottom - nBottomBorder;
	float fViewRatio = (float)rrView.Width() / (float)rrView.Height();

	if (m_nRectPolarDisplayMode == 0) {	//Rectilinear
		m_Data.GetFastScanSizeIncrement(&fFastScanSize, &fFastIncrement, 0);
		float fXSize = (fFastScanSize*(float)m_nZoomFactor) / 1000.0f;
		float fYSize = ((m_Data.m_fSlowIncrement*(float)m_Data.m_nNumberLines)*(float)m_nZoomFactor) / 1000.0f;
		fScanRatio = fXSize / fYSize;
	}
	else {
		fScanRatio = 1.0f;
	}

	if (fScanRatio > fViewRatio) {
		rrView.bottom = rrView.top + (int)((float)rrView.Width() / fScanRatio);
		rrWindow.bottom = rrView.bottom + nBottomBorder;
	}
	else {
		rrView.right = rrView.left + (int)((float)rrView.Height() * fScanRatio);
		rrWindow.right = rrView.right + nRightBorder;
	}

	nXTranslate = nYTranslate = 0;
	switch (theApp.m_LastSettings.n1to1Position) {
	case 0:	//Top Left
		break;
	case 1: //Top Right
		nXTranslate = nX[1] - rrWindow.right;
		break;
	case 2: //Bottom Left
		nYTranslate = nY - rrWindow.bottom;
		break;
	case 3://Bottom right
		nXTranslate = nX[1] - rrWindow.right;
		nYTranslate = nY - rrWindow.bottom;
		break;
	case 4://Centre
		nXTranslate = (nX[1] - rrWindow.right) / 2;
		nYTranslate = (nY - rrWindow.bottom) / 2;
		break;
	}

	rrWindow.OffsetRect(nXTranslate, nYTranslate);

	SetWindowPos( &wndTop , rrWindow.left, rrWindow.top, rrWindow.Width(), rrWindow.Height(), NULL);
}


void CViewSheet::RefreshCADTable()
{
	if (m_pCADTableDlg == NULL) {
		m_pCADTableDlg = new CCADTableDlg(this,(CWnd**)&m_pCADTableDlg,&m_Data);
		m_pCADTableDlg->Create(CCADTableDlg::IDD,this);
	} else {
		m_pCADTableDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		m_pCADTableDlg->FillList();
		m_pCADTableDlg->Invalidate(FALSE);

	}

}




void CViewSheet::OnFileAppendtoamultiplescanfile() 
{
	
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = m_Data.m_FileName;
	CUSLScans USLScans;

	if((nPos = m_Data.m_FileName.ReverseFind('.')) >0) {
		InitialFile = m_Data.m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".USLScans"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Multiple scans (*.USLScans)|*.USLScans|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate | CFile::modeNoTruncate, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			USLScans.Save(&File, &m_Data);
			File.Close();
		}
	}
	SetWindowTitle();
	SetToolBarCheckedState();

	
}

void CViewSheet::OnToolsCrossSection() 
{
	
	m_nToolOption = TOOL_CROSS_SECTION;
	SetToolBarCheckedState();
	CViewPage* pPage=(CViewPage*)GetActivePage();
	int m_nImageType = pPage->m_nImageType;
	int	m_nImageNumber = pPage->m_nImageNumber;
	int	nPaletteNumber = pPage->m_nPaletteNumber;
	int nTS = pPage->m_nTS;
	CString Buff;


	if (m_pViewThicknessCrossSectionDlg == NULL) {
		m_pViewThicknessCrossSectionDlg = new CViewThicknessCrossSectionDlg(this,(CWnd**)&m_pViewThicknessCrossSectionDlg,m_nImageType,m_nImageNumber,nPaletteNumber,&m_Data,nTS,&m_rrSamples);
		m_pViewThicknessCrossSectionDlg->Create(CViewThicknessCrossSectionDlg::IDD,this);
	} else {
		m_pViewThicknessCrossSectionDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		m_pViewThicknessCrossSectionDlg->m_nImageType = m_nImageType;
		m_pViewThicknessCrossSectionDlg->m_nImageNumber = m_nImageNumber;
		m_pViewThicknessCrossSectionDlg->m_nPaletteNumber = nPaletteNumber;
		m_pViewThicknessCrossSectionDlg->m_nTS = nTS;
	}
}




void CViewSheet::OpenWordPad(CString FileName) 
{
	
	CString CommandLine;
	int ii;

	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
	STARTUPINFO StartupInfo; //This is an [in] parameter
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof StartupInfo ; //Only compulsory field
	WCHAR strFileName[100];

	for(ii=0;ii<FileName.GetLength();ii++) {
		strFileName[ii]=FileName.GetAt(ii);
	};
	strFileName[ii]=0;
	CommandLine = theApp.m_WordPadPathName + " " + FileName;

	if(CreateProcess(NULL, CommandLine.GetBuffer(200), 
		NULL,NULL,FALSE,0,NULL,
		NULL,&StartupInfo,&ProcessInfo))
	{ 
		WaitForSingleObject(ProcessInfo.hProcess,INFINITE);
		CloseHandle(ProcessInfo.hThread);
		CloseHandle(ProcessInfo.hProcess);
	}  
	else
	{
		MessageBox(L"The process could not be started...");
	}
}


void CViewSheet::OpenChildView()
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	int m_nImageType = pPage->m_nImageType;
	int	m_nImageNumber = pPage->m_nImageNumber;
	int	nPaletteNumber = pPage->m_nPaletteNumber;
	int nTS = pPage->m_nTS;
	int	nPage = GetPageIndex(pPage);


	for(int nn=0;nn<256;nn++) {
		if(m_pViewChildPage[nn]==NULL) {
			m_pViewChildPage[nn] = new CViewPage(this,(CWnd**)&m_pViewChildPage[nn],m_nImageType,m_nImageNumber,&m_Data,nTS,nPaletteNumber,1,nPage);
			m_pViewChildPage[nn]->Create(CViewPage::IDD);
			return;
		}
	}

}

void CViewSheet::OnWindowNew() 
{
	OpenChildView();
	
}

void CViewSheet::OnWindowAllnew() 
{
	int nChild;

	for(int nPage = 0;nPage<256;nPage++) {
		if(m_pViewPage[nPage]) {
			for(nChild=0;nChild<256;nChild++) {
				if(m_pViewChildPage[nChild]!=NULL) {
					if(m_pViewChildPage[nChild]->m_nPageIndex == nPage) {
						nChild = 256;
					}
				} else {
					int m_nImageType = m_pViewPage[nPage]->m_nImageType;
					int	m_nImageNumber = m_pViewPage[nPage]->m_nImageNumber;
					int	nPaletteNumber = m_pViewPage[nPage]->m_nPaletteNumber;
					int nTS = m_pViewPage[nPage]->m_nTS;

					m_pViewChildPage[nChild] = new CViewPage(this,(CWnd**)&m_pViewChildPage[nChild],m_nImageType,m_nImageNumber,&m_Data,nTS,nPaletteNumber,1,nPage);
					m_pViewChildPage[nChild]->Create(CViewPage::IDD);
					nChild = 256;
				}
			}
		} else {
			nPage = 256;
		}
	}

}

void CViewSheet::OnWindowTilehorizontally() 
{
	CRect rrFirst,rrChild;
	int nNumberChildren;

	for(nNumberChildren=0;m_pViewChildPage[nNumberChildren];nNumberChildren++) {};
	if(nNumberChildren<=0) return;

	m_pViewChildPage[0]->GetWindowRect(rrFirst);

	for(int nChild = 1;nChild < nNumberChildren;nChild++) {
		rrChild = rrFirst;
		rrChild.left = rrFirst.left + rrFirst.Width() * nChild;
		rrChild.right = rrFirst.Width() + rrChild.left;

		m_pViewChildPage[nChild]->SetWindowPos( NULL , rrChild.left, rrChild.top, rrChild.Width(), rrChild.Height(), NULL);


	}

}

void CViewSheet::InvalidateChildren()
{
	for(int nChild=0;m_pViewChildPage[nChild];nChild++) {
	
		m_pViewChildPage[nChild]->m_bRenderView=TRUE;
		m_pViewChildPage[nChild]->Invalidate(FALSE);

	
	};

}

void CViewSheet::OnWindowCloseall() 
{
	for(int nChild=0;m_pViewChildPage[nChild];nChild++) {
	
		SAFE_DELETE( m_pViewChildPage[nChild] );
	
	};
}

void CViewSheet::OnWindowTilevertically() 
{
	CRect rrFirst,rrChild;
	int nNumberChildren;

	for(nNumberChildren=0;m_pViewChildPage[nNumberChildren];nNumberChildren++) {};
	if(nNumberChildren<=0) return;

	m_pViewChildPage[0]->GetWindowRect(rrFirst);

	for(int nChild = 1;nChild < nNumberChildren;nChild++) {
		rrChild = rrFirst;
		rrChild.top = rrFirst.top + rrFirst.Height() * nChild;
		rrChild.bottom = rrFirst.Height() + rrChild.top;

		m_pViewChildPage[nChild]->SetWindowPos( NULL , rrChild.left, rrChild.top, rrChild.Width(), rrChild.Height(), NULL);


	}

}


bool CViewSheet::IsContinueScanAllowable()
{
	

	return true;
	if(m_Data.m_fFastIncrement - PROFILE->m_fFastIncrement) return false;
	if(m_Data.m_fSlowIncrement - (PROFILE->m_fSlowIncrement * PROFILE->m_nLineIncrement)) return false;
	if(m_Data.m_nScanStartLine - PROFILE->m_nScanStartLine) return false;
	if(m_Data.m_nScanFinishLine - PROFILE->m_nScanFinishLine) return false;
	if(m_Data.m_nLineIncrement - PROFILE->m_nLineIncrement) return false;
	if(m_Data.m_nLineIncrementMode - PROFILE->m_nLineIncrementMode) return false;
	if(m_Data.m_nComponentDescription - PROFILE->m_nComponentDescription) return false;
	if(m_Data.m_nScanType - PROFILE->m_nScanType) return false;
	if(m_Data.m_nCollectAxis - PROFILE->m_nCollectAxis) return false;
	if(m_Data.m_nFlattenMode - PROFILE->m_nFlattenMode) return false;

	return true;
}

void CViewSheet::OnButtonPal0() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	pPage->m_nPaletteNumber = 0;
	InvalidateChild();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(pPage->m_nImageType,pPage->m_nPaletteNumber);
	
}

void CViewSheet::OnButtonPal1() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	pPage->m_nPaletteNumber = 1;
	InvalidateChild();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(pPage->m_nImageType,pPage->m_nPaletteNumber);
}

void CViewSheet::OnButtonPal2() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	pPage->m_nPaletteNumber = 2;
	InvalidateChild();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(pPage->m_nImageType,pPage->m_nPaletteNumber);
}

void CViewSheet::OnButtonPal3() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	pPage->m_nPaletteNumber = 3;
	InvalidateChild();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(pPage->m_nImageType,pPage->m_nPaletteNumber);
}

void CViewSheet::OnButtonPal4() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	pPage->m_nPaletteNumber = 4;
	InvalidateChild();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(pPage->m_nImageType,pPage->m_nPaletteNumber);
}

void CViewSheet::OnButtonPal5() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	pPage->m_nPaletteNumber = 5;
	InvalidateChild();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(pPage->m_nImageType,pPage->m_nPaletteNumber);
}

void CViewSheet::OnButtonPal6() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	pPage->m_nPaletteNumber = 6;
	InvalidateChild();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(pPage->m_nImageType,pPage->m_nPaletteNumber);
}

void CViewSheet::OnButtonPal7() 
{
	CViewPage* pPage=(CViewPage*)GetActivePage();
	pPage->m_nPaletteNumber = 7;
	InvalidateChild();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->ChangePalletteNumber(pPage->m_nImageType,pPage->m_nPaletteNumber);
}

void CViewSheet::OnFileExportAptsource()
{
	
	CStringArray StringArray;
	CString Buff,FileName;
	int nLine,ii;
	CProgressDlg dlg(NULL);
	dlg.Create(IDD_PROGRESS_DIALOG, NULL);	

	CFileDialog FileDlg(FALSE,_T(".aptsource"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("APTSOURCE (*.aptsource)|*.aptsource|All Files (*.*)|*.*||"),NULL);
	if(FileDlg.DoModal()==IDOK) {

		CUSLFile File;
		CFileException ExceptionInstance;
		int flag = File.Open(FileName =FileDlg.GetPathName(), CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate, &ExceptionInstance);

		File.SetLength(0);

		CArchive *pArch;
		pArch = new CArchive(&File,CArchive::store);

		Buff.Format(_T("USL APTSOURCE\nGenerated from %s\n"),m_Data.m_FileName);
		pArch->WriteString(Buff);
		
		if(m_Data.m_nNumberLines>0) {
			theApp.m_p3DData->PutvLineSampleInCADCoordinates();

			for(nLine=0;nLine<m_Data.m_nNumberLines;nLine++) {
				dlg.SetProgressPos(MulDiv(nLine, 100, m_Data.m_nNumberVertexLines));
				Buff.Format(_T("Saving coordinates %d of %d"),nLine,m_Data.m_nNumberVertexLines);
				dlg.SetProgressText(Buff);

				m_Data.GetVerticesAndResultsForALine(nLine, StringArray);

				for(ii=0;ii<StringArray.GetSize();ii++){
					Buff.Format(_T("GOTO \\ %s\n"),StringArray.GetAt(ii));
					pArch->WriteString(Buff);
				}
				Buff.Format(_T("RAPID\nRAPID\n"));
				pArch->WriteString(Buff);
			}
			theApp.m_p3DData->PutvLineSamplesBackInttoScanCoordinates();
		}	
		
		Buff.Format(_T("End Of File\n"));
		pArch->WriteString(Buff);

		pArch->Close();
		delete pArch;
		File.Close();
//		ShellExecute(NULL,L"Open",theApp.m_WordPadPathName,FileName,NULL,SW_SHOW);
//		OpenWordPad(FileName);
	}
}

void CViewSheet::OnFileProperties() 
{
	if (m_pDataPropertiesDlg == NULL) {
		m_pDataPropertiesDlg = new CDataPropertiesDlg(this,(CWnd**)&m_pDataPropertiesDlg,&m_Data);
		m_pDataPropertiesDlg->Create(CDataPropertiesDlg::IDD,this);
	} else {
		m_pDataPropertiesDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}

void CViewSheet::FileProperties()
{
	OnFileProperties();
}

void CViewSheet::OnFileMosaic() 
{
	if (m_pMosaicDlg == NULL) {
		m_pMosaicDlg = new CMosaicDlg(this,(CWnd**)&m_pMosaicDlg,&m_Data);
		m_pMosaicDlg->Create(CMosaicDlg::IDD,this);
	} else {
		m_pMosaicDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewSheet::OnButtonSquash() 
{
	

	theApp.m_LastSettings.nThresholdMode > 0 ? theApp.m_LastSettings.nThresholdMode = 0 : theApp.m_LastSettings.nThresholdMode = 2;
	theApp.m_Palette.CalculatePalette();
	SetToolBarCheckedState();
	InvalidateChild();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(UI_UPDATE_PALETTE_SHEET);

}

void CViewSheet::OnViewInclusions() 
{
	
	CString Buff;

	m_nToolOption = TOOL_INCLUSION;

	if (m_pInclusionsSheet == NULL) {
		m_pInclusionsSheet = new CInclusionsSheet(L"Inclusions",this,0,(CWnd**)&m_pInclusionsSheet, &m_Data.m_Inclusions,1,&m_Data);
		m_pInclusionsSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_INCLUSIONS_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pInclusionsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewSheet::CreatePhaseOverlayPage(int nAction)
{
	
	int nDest = 0;
	int nPage;
	int nSrc = theApp.m_LastSettings.nPhaseOverlayImageNumber;

	for(nPage=0;nPage < 256;nPage++) {
		if(m_pViewPage[nPage]) {
			if(m_pViewPage[nPage]->m_nImageType == PHASE_OVERLAY_IMAGE) {
				if(m_pViewPage[nPage]->m_nImageNumber == nDest) {
					m_pViewPage[nPage]->m_bRenderView=true;
					SetActivePage(nPage);
					m_pViewPage[nPage]->Invalidate(false);
					goto page_found;
				}
			}
		} else {
			m_pViewPage[nPage] = new CViewPage(this,(CWnd**)&m_pViewPage[nPage],PHASE_OVERLAY_IMAGE,nDest,&m_Data,0,0,0);

			AddPage(m_pViewPage[nPage]);
			SetActivePage(nPage);
			goto page_found;
		}

	}
page_found:

	m_Name[nPage].Format(_T("(Phase overlay) %s"),	m_Name[nSrc]);
	SetTabNames();

}

afx_msg LRESULT CViewSheet::OnToolbarReset(WPARAM wp, LPARAM)
{
	UINT uiToolBarId = (UINT)wp;

	int nReplaced;

	m_pwndEditGeneralButton->SetContextMenuID(IDC_EDIT_GENERAL);
	m_pwndEditGeneralButton->CanBeStretched();
	m_pwndEditGeneralButton->HaveHotBorder();
	m_pwndEditGeneralButton->SetFlatMode(true);
	m_pwndEditGeneralButton->SetStyle(TBBS_PRESSED);
	m_pwndEditGeneralButton->SetContents(L"Hello world");

	nReplaced = m_wndArchiveToolBar.ReplaceButton(IDC_EDIT_GENERAL, *m_pwndEditGeneralButton);

	return 0;
}

void CViewSheet::EditTextSetWindowText(CString Buff)
{
	CMFCToolBarEditBoxButton::SetContentsAll(IDC_EDIT_GENERAL, Buff);
}

void CViewSheet::EditTextGetWindowText(CString &pBuff)
{
	CEdit *pEdit = m_pwndEditGeneralButton->GetEditBox();
	pEdit->GetWindowTextW(pBuff);
}

void CViewSheet::OnChangeEditEditText()
{
	CString Buff;

	if ((m_nToolOption == TOOL_HISTOGRAM) && (theApp.m_LastSettings.nHistogramStyle == HISTOGRAM_CIRCLE_STYLE)) {
		EditTextGetWindowText(Buff);
		_WTOF(Buff, theApp.m_LastSettings.fHistogramCircleSize);
	}
}


void CViewSheet::OnSize(UINT nType, int cx, int cy)
{
	CResizableSheet::OnSize(nType, cx, cy);

	CMFCToolBarButton *pButton = m_wndArchiveToolBar.GetButton(m_wndArchiveToolBar.CommandToIndex(IDC_EDIT_GENERAL));
	if (pButton && pButton->GetHwnd()) {
//		CRect rr = pButton->Rect();
//		rr.top -= 4;
//		rr.bottom += 4;
//		pButton->SetRect(rr);
	}
}
