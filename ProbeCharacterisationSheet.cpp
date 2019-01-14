// ProbeCharacterisationSheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ProbeCharacterisationSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProbeCharacterisationSheet

IMPLEMENT_DYNAMIC(CProbeCharacterisationSheet, CResizableSheet)

inline void CProbeCharacterisationSheet::Construct(CWnd** pDlg)
{
	
	int	ii;

	m_pDlg = pDlg;
	m_pMenu = NULL;
	
	m_pProbeToolsSheet = NULL;

	m_pProbeSpecificationPage = NULL;
	for(ii=0;ii<256;ii++) {
		m_pProbeDataSheetPage[ii] = NULL;
	}
	for(ii=0;ii<256;ii++) {
		m_pProbeBeamProfilePage[ii] = NULL;
	}

	CFileException e;
	CUSLFile File;
	CString FilePath,Buff;

	FilePath = theApp.m_LastSettingsPath + L"\\Last.probe";

	if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(Buff,_T("Error"),MB_ICONERROR);
	} else {
		m_Probe.Retrieve(&File);
		File.Close();
	}


	AddAllPages(TRUE);


}

CProbeCharacterisationSheet::CProbeCharacterisationSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CProbeCharacterisationSheet::CProbeCharacterisationSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CProbeCharacterisationSheet::~CProbeCharacterisationSheet()
{
	
	CFileException e;
	CUSLFile File;
	CString FilePath,Buff;

	FilePath = theApp.m_LastSettingsPath + L"\\Last.probe";

	if(!File.Open(FilePath,CFile::modeCreate|CFile::typeBinary|CFile::modeWrite,&e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(Buff,_T("Error"),MB_ICONERROR);
	} else {
		m_Probe.Save(&File);
		File.Close();
	}

	SAFE_DELETE(m_pMenu);
	RemoveAllPages(TRUE);

}

void CProbeCharacterisationSheet::AddAllPages(bool bIncludingSpecification)
{
	int ii;

	if(bIncludingSpecification == TRUE) {
		m_pProbeSpecificationPage = new CProbeSpecificationPage(&m_Probe);
		AddPage(m_pProbeSpecificationPage);
	}

	for(ii=0;ii<m_Probe.m_nNumberDataSheets;ii++) {
		m_pProbeDataSheetPage[ii] = new CProbeDataSheetPage(&m_Probe,m_Probe.m_pProbeDataSheet[ii]);
		AddPage(m_pProbeDataSheetPage[ii]);
	}

	for(ii=0;ii<m_Probe.m_nNumberBeamProfiles;ii++) {
		m_pProbeBeamProfilePage[ii] = new CProbeBeamProfilePage(&m_Probe,m_Probe.m_pProbeBeamProfile[ii]);
		AddPage(m_pProbeBeamProfilePage[ii]);
	}

}

void CProbeCharacterisationSheet::RemoveAllPages(bool bIncludingSpecification)
{
	int ii;

	if(bIncludingSpecification == TRUE) {
		if( m_pProbeSpecificationPage) RemovePage(m_pProbeSpecificationPage);
		SAFE_DELETE(m_pProbeSpecificationPage);
	}

	for(ii=0;ii<256;ii++) {
		if( m_pProbeDataSheetPage[ii]) RemovePage(m_pProbeDataSheetPage[ii]);
		SAFE_DELETE(m_pProbeDataSheetPage[ii]);
	}
	for(ii=0;ii<256;ii++) {
		if( m_pProbeBeamProfilePage[ii]) RemovePage(m_pProbeBeamProfilePage[ii]);
		SAFE_DELETE(m_pProbeBeamProfilePage[ii]);
	}


}


BEGIN_MESSAGE_MAP(CProbeCharacterisationSheet, CResizableSheet)
	//{{AFX_MSG_MAP(CProbeCharacterisationSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_DATASHEETS_ADD, OnDatasheetsAdd)
	ON_COMMAND(ID_BEAMPROFILES_ADD, OnBeamprofilesAdd)
	ON_COMMAND(ID_BUTTON_REFRESH, OnButtonRefresh)
	ON_COMMAND(ID_BUTTON_LOCK, OnButtonLock)
	ON_COMMAND(ID_BUTTON_DELETE, OnButtonDelete)
	ON_COMMAND(ID_BUTTON_TOOLS, OnButtonTools)
	ON_COMMAND(ID_BUTTON_EXPORT_CLIPBOARD, OnButtonExportClipboard)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProbeCharacterisationSheet message handlers

void CProbeCharacterisationSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CResizableSheet::PostNcDestroy();
	delete this;
}
void CProbeCharacterisationSheet::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PROBE_CHARACTER_SHEET]=FALSE;
	
	CResizableSheet::OnClose();
}

void CProbeCharacterisationSheet::OnDestroy() 
{

	


	//Store Window Position
	if (IDD_PROBE_CHARACTER_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_PROBE_CHARACTER_SHEET]);

	CResizableSheet::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CProbeCharacterisationSheet::OnInitDialog() 
{
	
	BOOL bResult = CResizableSheet::OnInitDialog();
	

	if (IDD_PROBE_CHARACTER_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_PROBE_CHARACTER_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}


	
	SetToolBarCheckedState();
	SetTabNames();

	return bResult;

}

int CProbeCharacterisationSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;
	if (CResizableSheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	CResizableSheet::m_bToolBar=TRUE;

	m_pMenu = new CMenu; 
	m_pMenu->LoadMenu(IDR_PROBE_SHEET_MENU);
	SetMenu(m_pMenu);

	if (!m_wndToolsToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndToolsToolBar.LoadToolBar(IDR_PROBE_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	if (!m_wndRebar.Create(this,NULL,CBRS_ALIGN_TOP | AFX_IDW_DIALOGBAR)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	GetClientRect(rect);

	m_wndRebar.SetWindowPos(&wndTop   , 0,0,200,30,SWP_SHOWWINDOW);
	m_wndRebar.AddBar(&m_wndToolsToolBar);

	EnableStackedTabs(FALSE);
	
	
	return 0;
}

void CProbeCharacterisationSheet::SetTabNames()
{
	
	TCITEM TabCtrlItem;
	CTabCtrl* tab = GetTabControl();
	int	nPage,nn;
	CString Buff;

	for(nPage=0;nPage<m_Probe.m_nNumberDataSheets;nPage++) {
		if(m_pProbeDataSheetPage[nPage]) {
			nn=GetPageIndex(m_pProbeDataSheetPage[nPage]);
			ZeroMemory(&TabCtrlItem,sizeof TabCtrlItem);
			tab->GetItem(nn,&TabCtrlItem);
			Buff = L"Data : " + m_Probe.m_pProbeDataSheet[nPage]->m_strValues.GetAt(0);
			TabCtrlItem.pszText = (WCHAR *)LPCTSTR(Buff);
			TabCtrlItem.mask=TCIF_TEXT;
			tab->SetItem(nn,&TabCtrlItem);
		}
	}
	for(nPage=0;nPage<m_Probe.m_nNumberBeamProfiles;nPage++) {
		if(m_pProbeBeamProfilePage[nPage]) {
			nn=GetPageIndex(m_pProbeBeamProfilePage[nPage]);
			ZeroMemory(&TabCtrlItem,sizeof TabCtrlItem);
			tab->GetItem(nn,&TabCtrlItem);
			Buff = L"Beam : " + m_Probe.m_pProbeBeamProfile[nPage]->m_strValues.GetAt(0);
			TabCtrlItem.pszText = (WCHAR *)LPCTSTR(Buff);
			TabCtrlItem.mask=TCIF_TEXT;
			tab->SetItem(nn,&TabCtrlItem);
		}
	}


}

void CProbeCharacterisationSheet::OnFileNew() 
{

	if(m_Probe.CheckNewProfileAllowed()!=TRUE) return;

	m_Probe.Zero();
	RemoveAllPages(FALSE);

	if(m_pProbeSpecificationPage) m_pProbeSpecificationPage->UpdateAllControls();

	m_Probe.m_bModified=FALSE;
}

void CProbeCharacterisationSheet::OnFileOpen() 
{
	
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	if(m_Probe.CheckNewProfileAllowed()!=TRUE) return;

	CFileDialog FileDlg(TRUE,_T(".probe"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Probe (*.probe)|*.probe|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProbesPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProbesPath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			m_Probe.Retrieve(&File);
			File.Close();
			m_Probe.m_bModified=FALSE;

			RemoveAllPages(FALSE);
			AddAllPages(FALSE);
			SetTabNames();
			if(m_pProbeSpecificationPage) m_pProbeSpecificationPage->UpdateAllControls();
			SetWindowTitle();
		}
	}

}

void CProbeCharacterisationSheet::OnFileSaveas() 
{
	
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = m_Probe.m_FileName;



	if((nPos = m_Probe.m_FileName.ReverseFind('.')) >0) {
		InitialFile = m_Probe.m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".probe"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Probe (*.probe)|*.probe|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProbesPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProbesPath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			m_Probe.Save(&File);
			File.Close();
			SetWindowTitle();
		}
	}
	
}


void CProbeCharacterisationSheet::OnDatasheetsAdd() 
{
	int	ii;

	for(ii=0;ii<256;ii++) {
		if(m_pProbeDataSheetPage[ii] == NULL) {
			m_Probe.m_pProbeDataSheet[ii] = new CProbeDataSheet();
			m_pProbeDataSheetPage[ii] = new CProbeDataSheetPage(&m_Probe,m_Probe.m_pProbeDataSheet[ii]);
			AddPage(m_pProbeDataSheetPage[ii]);
			SetActivePage(m_pProbeDataSheetPage[ii]);
			m_Probe.m_nNumberDataSheets = ii+1;
			SetTabNames();
			break;
		}
	}
}


void CProbeCharacterisationSheet::OnBeamprofilesAdd() 
{
	int	ii;

	for(ii=0;ii<256;ii++) {
		if(m_pProbeBeamProfilePage[ii] == NULL) {
			m_Probe.m_pProbeBeamProfile[ii] = new CProbeBeamProfile();
			m_pProbeBeamProfilePage[ii] = new CProbeBeamProfilePage(&m_Probe,m_Probe.m_pProbeBeamProfile[ii]);
			AddPage(m_pProbeBeamProfilePage[ii]);
			SetActivePage(m_pProbeBeamProfilePage[ii]);
			m_Probe.m_nNumberBeamProfiles = ii+1;
			SetTabNames();
			break;
		}
	}
}


void CProbeCharacterisationSheet::OnButtonRefresh() 
{
	int ii;
	CPropertyPage* pPage;

	pPage = GetActivePage();
	for(ii=0;ii<256;ii++) {
		if(pPage == (CPropertyPage*)m_pProbeDataSheetPage[ii]) {
			m_Probe.m_pProbeDataSheet[ii]->Refresh();
			m_pProbeDataSheetPage[ii]->UpdateAllControls();
			m_pProbeDataSheetPage[ii]->Invalidate(FALSE);
			break;
		}
		if(pPage == (CPropertyPage*)m_pProbeBeamProfilePage[ii]) {
			m_pProbeBeamProfilePage[ii]->UpdateAllControls();
			break;
		}
	}
	SetTabNames();

}

void CProbeCharacterisationSheet::OnButtonLock() 
{
	int ii;
	CPropertyPage* pPage;
	CString Buff;
	CString Warning;
	Buff.LoadString(IDS_Are_You_Sure_Lock);
	Warning.LoadString(IDS_Warning);

	if(MessageBox(Buff,Warning,MB_YESNO) == IDYES) {
		pPage = GetActivePage();
		for(ii=0;ii<256;ii++) {
			if(pPage == (CPropertyPage*)m_pProbeDataSheetPage[ii]) {
				m_Probe.m_pProbeDataSheet[ii]->m_bLocked = TRUE;
				m_pProbeDataSheetPage[ii]->UpdateAllControls();
				break;
			}
			if(pPage == (CPropertyPage*)m_pProbeBeamProfilePage[ii]) {
				m_Probe.m_pProbeBeamProfile[ii]->m_bLocked = TRUE;
				m_pProbeBeamProfilePage[ii]->UpdateAllControls();
				break;
			}
		}
		SetToolBarCheckedState();
	}
}

void CProbeCharacterisationSheet::OnButtonDelete() 
{
	int ii,nRemoveDataSheet,nRemoveBeamProfile;
	CPropertyPage* pPage;

	pPage = GetActivePage();
	nRemoveDataSheet = -1;
	nRemoveBeamProfile = -1;
	for(ii=0;ii<256;ii++) {
		if(pPage == (CPropertyPage*)m_pProbeDataSheetPage[ii]) {
			nRemoveDataSheet = ii;
			break;
		}
		if(pPage == (CPropertyPage*)m_pProbeBeamProfilePage[ii]) {
			nRemoveBeamProfile = ii;
			break;
		}
	}
	RemoveAllPages(FALSE);
	if(nRemoveDataSheet>=0) m_Probe.DeleteProbeDataSheet(nRemoveDataSheet);
//	if(nRemoveBeamProfile>=0) m_Probe.DeleteBeamProfile(nRemoveBeamProfile);
	AddAllPages(FALSE);	
	SetTabNames();
}

void CProbeCharacterisationSheet::SetToolBarCheckedState()
{
	bool bLocked = FALSE;
	int ii,nIndex;
	CPropertyPage* pPage;

	pPage = GetPage(GetActiveIndex());

	for(ii=0;ii<256;ii++) {
		if(pPage == (CPropertyPage*)m_pProbeDataSheetPage[ii]) {
			if(m_Probe.m_pProbeDataSheet[ii]) bLocked = m_Probe.m_pProbeDataSheet[ii]->m_bLocked;
			break;
		}
		if(pPage == (CPropertyPage*)m_pProbeBeamProfilePage[ii]) {
			if(m_Probe.m_pProbeBeamProfile[ii]) bLocked = m_Probe.m_pProbeBeamProfile[ii]->m_bLocked;
			break;
		}
	}

	if(bLocked == TRUE) {
		if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_LOCK))>=0) m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED);
		if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_REFRESH))>=0) m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_DISABLED);
		if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_DELETE))>=0) m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_DISABLED);
	} else {
		if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_LOCK))>=0) m_wndToolsToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_REFRESH))>=0) m_wndToolsToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_DELETE))>=0) m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	}
}

BOOL CProbeCharacterisationSheet::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
	case ID_DATASHEETS_ADD:  strTipText.LoadString(IDS_Add_Data_Sheet);
		break;
	case ID_BEAMPROFILES_ADD:  strTipText.LoadString(IDS_Add_Beam_Profile);
		break;
	case ID_BUTTON_REFRESH:  strTipText.LoadString(IDS_Refresh);
		break;
	case ID_BUTTON_LOCK:  strTipText.LoadString(IDS_Lock);
		break;
	case ID_BUTTON_DELETE:  strTipText.LoadString(IDS_Delete_Page);
		break;
	case ID_BUTTON_TOOLS:  strTipText.LoadString(IDS_Tools);
		break;
	case ID_BUTTON_EXPORT_CLIPBOARD:  strTipText.LoadString(IDS_Send_to_clipboard);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}





void CProbeCharacterisationSheet::OnButtonTools() 
{
	
	
	if (m_pProbeToolsSheet == NULL) {
		m_pProbeToolsSheet = new CProbeToolsSheet(_T("Tools"), this, 0, (CWnd**)&m_pProbeToolsSheet,&m_Probe);
		m_pProbeToolsSheet->Create(this);
	} else {
		m_pProbeToolsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}

void CProbeCharacterisationSheet::UpdateAllPages()
{
	int ii;

	for(ii=0;ii<256;ii++) {
		if(m_pCurrentPage == (CPropertyPage*)m_pProbeDataSheetPage[ii]) {
			m_pProbeDataSheetPage[ii]->UpdateAllControls();
			break;
		}
		if(m_pCurrentPage == (CPropertyPage*)m_pProbeBeamProfilePage[ii]) {
			m_pProbeBeamProfilePage[ii]->UpdateAllControls();
			m_pProbeBeamProfilePage[ii]->Invalidate(FALSE);
			break;
		}
	}


}

void CProbeCharacterisationSheet::SetWindowTitle()
{
	
	CString	Buff,Temp;
	int nPos;

	Buff = m_Probe.m_FileName;
	if((nPos=Buff.ReverseFind('.')) > -1) {
		Buff.GetBufferSetLength(nPos);
	}

	if(m_Probe.m_bModified) {
		Buff += " (Not Saved)";
	}


	SetWindowText(Buff);

}

void CProbeCharacterisationSheet::OnButtonExportClipboard() 
{
	CRect rr;
	CDC  dcMem;
	CBitmap bmpScreen;

	GetClientRect(rr);
	rr.top+=56;
	rr.left+=4;
	rr.right-=15;
	rr.bottom-=15;

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
