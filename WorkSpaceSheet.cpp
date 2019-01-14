// WorkSpaceSheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "WorkSpaceSheet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceSheet

IMPLEMENT_DYNAMIC(CWorkSpaceSheet, CResizableSheet)
inline void CWorkSpaceSheet::Construct(CWnd** pDlg)
{
	

	m_pDlg = pDlg;
	m_pMenu = NULL;

	
	m_pWorkSpacePage = NULL;
	m_pMultipleScansPage = NULL;

	m_pWorkSpacePage = new CWorkSpaceDlg();
	AddPage(m_pWorkSpacePage);

//	m_pMultipleScansPage = new CMultipleScansPage();
//	AddPage(m_pMultipleScansPage);

	SetActivePage(theApp.m_LastSettings.nLastWorkspaceTab);

}

CWorkSpaceSheet::CWorkSpaceSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CWorkSpaceSheet::CWorkSpaceSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CWorkSpaceSheet::~CWorkSpaceSheet()
{
	SAFE_DELETE( m_pWorkSpacePage );
	SAFE_DELETE( m_pMultipleScansPage );
	SAFE_DELETE( m_pMenu );

}


BEGIN_MESSAGE_MAP(CWorkSpaceSheet, CResizableSheet)
	//{{AFX_MSG_MAP(CWorkSpaceSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEWWORKSPACE, OnFileNewworkspace)
	ON_COMMAND(ID_FILE_OPENWORKSPACE, OnFileOpenworkspace)
	ON_COMMAND(ID_FILE_SAVEWORKSPACE, OnFileSaveworkspace)
	ON_COMMAND(ID_BUTTON_START_SCAN, OnButtonStartScan)
	ON_COMMAND(ID_BUTTON_PAUSE_SCAN, OnButtonPauseScan)
	ON_WM_TIMER()
	ON_COMMAND(ID_BUTTON_ERASE_ALL_FILENAMES, OnButtonEraseAllFilenames)
	ON_COMMAND(ID_BUTTON_PARTITION, OnButtonPartition)
	ON_COMMAND(ID_BUTTON_SERIAL, OnButtonSerial)
	ON_COMMAND(ID_BUTTON_SINGLE_SCAN, OnButtonSingleScan)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceSheet message handlers
int CWorkSpaceSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	
	m_pMenu = new CMenu;
	m_pMenu->LoadMenu(IDR_WORKSPACE_MENU);

	SetMenu(m_pMenu);

	if (CResizableSheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	CResizableSheet::m_bToolBar=TRUE;


	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_WORKSPACE_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create porosity ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);

	return 0;
}

void CWorkSpaceSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CResizableSheet::PostNcDestroy();
	delete this;
}

void CWorkSpaceSheet::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_WORKSPACE_SHEET] = FALSE;
	
	CResizableSheet::OnClose();
}

void CWorkSpaceSheet::OnDestroy() 
{
	

	if (IDD_WORKSPACE_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_WORKSPACE_SHEET]);

	CResizableSheet::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CWorkSpaceSheet::OnInitDialog() 
{
	
	BOOL bResult = CResizableSheet::OnInitDialog();
	
	

	CRect rc;
	GetWindowRect(&rc);

	SetMinTrackSize(CSize(GetMinWidth(), rc.Height()));

	EnableSaveRestore(_T("WorkSpaceSheet"), TRUE, TRUE);

	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);
//	SetTabNames();
	
	SetTimer(1,500,NULL);

	return bResult;
}


void CWorkSpaceSheet::OnFileNewworkspace() 
{
	m_pWorkSpacePage->NewWorkspace();
	
}

void CWorkSpaceSheet::OnFileOpenworkspace() 
{
	m_pWorkSpacePage->OpenWorkspace();
	SetWindowText();
	
}

void CWorkSpaceSheet::OnFileSaveworkspace() 
{
	m_pWorkSpacePage->SaveAsWorkSpace();
	
}

void CWorkSpaceSheet::OnButtonEraseAllFilenames() 
{
	m_pWorkSpacePage->EraseAllFileNames();
}


void CWorkSpaceSheet::SetToolBarCheckedState()
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int	nIndex;
	int nStyle;

	if(pFrame->m_nTimerAction == WORKSHEET_SCAN || pFrame->m_nTimerAction == WORKSHEET_SINGLE_SCAN) {
		nStyle = TBBS_DISABLED;
	} else {
		nStyle = 0;
	}
	
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_FILE_OPENWORKSPACE),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_FILE_SAVEWORKSPACE),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_START_SCAN),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_SINGLE_SCAN),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_PAUSE_SCAN),nStyle ^ TBBS_DISABLED);
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_ERASE_ALL_FILENAMES),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_INSPECTION_CRITERIA),nStyle );

	
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_SERIAL))>=0)
		theApp.m_WS.m_nScanMethod==_SERIAL ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED | nStyle) : m_wndToolBar.SetButtonStyle(nIndex,nStyle);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_PARTITION))>=0)
		theApp.m_WS.m_nScanMethod==_PARTITION ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED | nStyle) : m_wndToolBar.SetButtonStyle(nIndex,nStyle);


}

void CWorkSpaceSheet::OnTimer(UINT nIDEvent) 
{

	SetToolBarCheckedState();
	CResizableSheet::OnTimer(nIDEvent);
}


void CWorkSpaceSheet::OnButtonStartScan() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString Buff,Error;

	Error.LoadString(IDS_Error);
	if(theApp.m_WS.IsCalibrationExpired() == true && theApp.m_WS.IsPartitionScan()==true) {
		Buff.LoadString(IDS_Calibration_expired);
		MessageBox(Buff, Error, MB_ICONERROR);
		return;
	}



	pFrame->StopCurrentTimerAction();
	pFrame->m_bPauseWorksheetScan = false;
	pFrame->m_nTimerAction = WORKSHEET_SCAN;
}

void CWorkSpaceSheet::OnButtonPauseScan() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->m_bPauseWorksheetScan = true;
}

void CWorkSpaceSheet::OnButtonSingleScan() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;



	pFrame->StopCurrentTimerAction();
	pFrame->m_bPauseWorksheetScan = false;
	pFrame->m_nTimerAction = WORKSHEET_SINGLE_SCAN;
}


BOOL CWorkSpaceSheet::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
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
   case ID_FILE_OPENWORKSPACE: strTipText.LoadString(IDS_Open_workspace);
	   break;
   case ID_FILE_SAVEWORKSPACE: strTipText.LoadString(IDS_Save_workspace);
	   break;
   case ID_BUTTON_START_SCAN: strTipText.LoadString(IDS_Start_Scan);
	   break;
   case ID_BUTTON_SINGLE_SCAN: strTipText.LoadString(IDS_Single_Scan);
	   break;
   case ID_BUTTON_PAUSE_SCAN: strTipText.LoadString(IDS_Pause_Scan);
	   break;
   case ID_BUTTON_ERASE_ALL_FILENAMES: strTipText = L"Erase and start new inspection";
	   break;
   case ID_BUTTON_SERIAL: strTipText.LoadString(IDS_Serial_scan);
	   break;
   case ID_BUTTON_PARTITION: strTipText.LoadString(IDS_Partition_scan);
	   break;
   case ID_BUTTON_INSPECTION_CRITERIA: strTipText.LoadString(IDS_Import_ref_criteria);
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


void CWorkSpaceSheet::UpdateAllPages()
{
	if(m_pWorkSpacePage ) {
		m_pWorkSpacePage->UpdateAllControls();
		m_pWorkSpacePage->Invalidate(false);
	}
//	if(m_pMultipleScansPage ) m_pMultipleScansPage->UpdateAllControls();

}


void CWorkSpaceSheet::OnButtonPartition() 
{
	
	theApp.m_WS.m_nScanMethod=1;
	SetToolBarCheckedState();
	if(m_pWorkSpacePage ) m_pWorkSpacePage->SetAccessPrivelage();
	SetWindowText();
	
}

void CWorkSpaceSheet::OnButtonSerial() 
{
	
	theApp.m_WS.m_nScanMethod=0;
	SetToolBarCheckedState();
	if(m_pWorkSpacePage ) m_pWorkSpacePage->SetAccessPrivelage();
	SetWindowText();
}

void CWorkSpaceSheet::SetWindowText()
{
	
	CString Buff;

	switch(theApp.m_WS.m_nScanMethod) {
	case 0: Buff.Format(L"WorkSpace: Serial:  %s",theApp.m_WS.m_FileName);
		break;
	case 1: Buff.Format(L"WorkSpace: Partition:  %s",theApp.m_WS.m_FileName);
		break;
	}
	SetTitle(Buff);

}

