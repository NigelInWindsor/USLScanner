// ProfileSheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ProfileSheet.h"
#include "FileThumbNailDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfileSheet

IMPLEMENT_DYNAMIC(CProfileSheet, CResizableSheet)

CProfileSheet::CProfileSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CResizableSheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CProfileSheet::CProfileSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage, CWnd** pDlg)
	:CResizableSheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pDlg = pDlg;

	m_pMenu = NULL;
	m_p3DViewEditPage = NULL;
	m_p3DEditTurnTablePage = NULL;
	m_p3DDesignPage = NULL;
	m_p3DViewPage = NULL;
	m_p3DToolsSheet = NULL;
	m_p3DStiffenerPage = NULL;

	if (theApp.m_LastSettings.nProfileTabMask == 0) theApp.m_LastSettings.nProfileTabMask = 0x001;

	if(theApp.m_LastSettings.nProfileTabMask & ProfileCartesianTab) {
		m_p3DViewEditPage = new C3DViewEditDlg(0);
		AddPage(m_p3DViewEditPage);
	}

	if(theApp.m_LastSettings.nProfileTabMask & ProfileTurTableTab) {
		m_p3DEditTurnTablePage = new C3DEditTurnTablePage;
		AddPage(m_p3DEditTurnTablePage);
	}

	if(theApp.m_LastSettings.nProfileTabMask & Profile3DDesignTab) {
		m_p3DDesignPage = new C3DDesignPage;
		AddPage(m_p3DDesignPage);
	}
	if(theApp.m_LastSettings.nProfileTabMask & Profile3DViewTab) {
		m_p3DViewPage = new C3DViewPage;
		AddPage(m_p3DViewPage);
	};

	if(theApp.m_LastSettings.nProfileTabMask & Profile3DStiffenerTab) {
		m_p3DStiffenerPage = new C3DStiffenerPage;
		AddPage(m_p3DStiffenerPage);
	};
}

CProfileSheet::~CProfileSheet()
{
	SAFE_DELETE( m_p3DViewPage );
	SAFE_DELETE( m_p3DDesignPage );
	SAFE_DELETE( m_p3DViewEditPage );
	SAFE_DELETE( m_p3DEditTurnTablePage );
	SAFE_DELETE( m_p3DStiffenerPage );
	SAFE_DELETE( m_pMenu );

}


BEGIN_MESSAGE_MAP(CProfileSheet, CResizableSheet)
	//{{AFX_MSG_MAP(CProfileSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_TTLIST_ADD, OnTtlistAdd)
	ON_COMMAND(ID_TTLIST_DELETE, OnTtlistDelete)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_TOOLS_DRAWINGOPTIONS, OnToolsDrawingoptions)
	ON_COMMAND(ID_COPY_ALLSCANLINESTAUGHTLINES, OnCopyAllscanlinestaughtlines)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfileSheet message handlers

void CProfileSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CResizableSheet::PostNcDestroy();
	delete this;
}
void CProfileSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PROFILE_SHEET]=FALSE;
	
	CResizableSheet::OnClose();
}

void CProfileSheet::OnDestroy() 
{
	//Store Window Position
	if (IDD_PROFILE_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_PROFILE_SHEET]);

	CResizableSheet::OnDestroy();
	
	*m_pDlg	= NULL;	
}


BOOL CProfileSheet::OnInitDialog() 
{
	BOOL bResult = CResizableSheet::OnInitDialog();

	CRect rc;
	GetWindowRect(&rc);

	SetMinTrackSize(CSize(GetMinWidth(), rc.Height()));

	EnableSaveRestore(_T("ProfileSheet"), TRUE, TRUE);

	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);
	
	SetAccessPrivelage();

	return bResult;
}

void CProfileSheet::OnMove(int x, int y) 
{
	CResizableSheet::OnMove(x, y);
	
	if(m_p3DViewEditPage) m_p3DViewEditPage->Move(x,y);
	
}

int CProfileSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;

	/* Disabled because it takes up space
	m_pMenu = new CMenu; 
	m_pMenu->LoadMenu(IDR_3D_VIEW_MENU);
	SetMenu(m_pMenu);
	*/

	if (CResizableSheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	CResizableSheet::m_bToolBar=TRUE;
	
	return 0;
}

void CProfileSheet::OnFileOpen() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	if(PROFILE->CheckNewProfileAllowed()!=TRUE) return;

	CFileThumbNailDlg FileDlg(TRUE,_T(".pro"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Profile (*.pro)|*.pro|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			CWaitCursor Wait;
			PROFILE->Retrieve(&File);
			if(theApp.m_LastSettings.nLoadUltrasonicsFromProfile==1) {
				PROFILE->CopyLocalUtToGlobal();
			}
			File.Close();
			Wait.Restore();
			SetWindowTitle();
//			pFrame->SendMessage(UI_PROFILE_OPENED);
			pFrame->SendMessage(UI_PROFILE_OPENED, (WPARAM)this, NULL);
			theApp.ActivityLog(L"Profile loaded : " + FilePath);

			UpdateAllPages();
		}
	}
	
}

void CProfileSheet::OnFileSaveAs() 
{
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = PROFILE->m_FileName;


	if((nPos = PROFILE->m_FileName.ReverseFind('.')) >0) {
		InitialFile = PROFILE->m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".pro"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Profile (*.pro)|*.pro|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			PROFILE->Save(&File);
			File.Close();
			SetWindowTitle();
			theApp.ActivityLog(L"Profile saved : " + FilePath);
		}
	}
	
}

void CProfileSheet::SetWindowTitle()
{
	CString	Buff;
	int nPos;

	if(PROFILE->m_strDescription.IsEmpty()) {
		Buff = PROFILE->m_FileName;
		if((nPos=Buff.ReverseFind('.')) > -1) {
			Buff.GetBufferSetLength(nPos);
		}
	} else {
		Buff = PROFILE->m_strDescription;
	}

	if(PROFILE->m_bModified) {
		Buff += " (Not Saved)";
	}

	SetWindowText(Buff);

}

void CProfileSheet::UpdateAllPages()
{
	if( m_p3DViewEditPage ) {
		if(m_p3DViewEditPage->m_hWnd != NULL) {
			m_p3DViewEditPage->CreateColumns();
			m_p3DViewEditPage->FillList();
			m_p3DViewEditPage->UpdateAllControls();
			m_p3DViewEditPage->Invalidate(FALSE);
		}
	}

	if( m_p3DEditTurnTablePage ) {
		if(m_p3DEditTurnTablePage->m_hWnd!=NULL) {
			m_p3DEditTurnTablePage->CreateColumns();
			m_p3DEditTurnTablePage->FillList();
			m_p3DEditTurnTablePage->UpdateAllControls();
			m_p3DEditTurnTablePage->Invalidate(FALSE);
		}
	}
	if( m_p3DViewPage ) {
		if( m_p3DViewPage->m_hWnd!=NULL) {
			m_p3DViewPage->InitializeComponent();
			m_p3DViewPage->Invalidate(false);
			m_p3DViewPage->SetToolBarCheckedState();
		}
	}
	if( m_p3DStiffenerPage ) {
		if( m_p3DViewPage->m_hWnd!=NULL) {
			m_p3DViewPage->Invalidate(false);
		}
	}

	SetWindowTitle();
}

void CProfileSheet::OnFileNew() 
{
	if(PROFILE->CheckNewProfileAllowed()!=TRUE) return;

	PROFILE->New();
	if(theApp.m_LastSettings.nSelectTaughtButtonOnNew) 	theApp.m_nEditLineType = TAUGHTLINES;

	C3DViewPage* pPage = (C3DViewPage*)GetActivePage();

	if(pPage == (C3DViewPage*)m_p3DEditTurnTablePage) {
		PROFILE->m_nComponentDescription = CIRCULAR;
	}

	UpdateAllPages();
}

void CProfileSheet::OnTtlistAdd() 
{
	// TODO: Add your command handler code here
	
}

void CProfileSheet::OnTtlistDelete() 
{
	// TODO: Add your command handler code here
	
}

void CProfileSheet::SetAccessPrivelage()
{
	if(GetSafeHwnd() == NULL) return;
	int m_nEnableFlag;

	if( m_p3DViewEditPage) m_p3DViewEditPage->SetAccessPrivelage();
	if( m_p3DEditTurnTablePage) m_p3DEditTurnTablePage->SetAccessPrivelage();

	CMenu* pPopup = NULL;

	if(m_pMenu) {
		pPopup = m_pMenu->GetSubMenu(0);
	//	theApp.m_nLogonLevelMask & theApp.m_cAccess[ID_PROFILES_NEW] ? m_nEnableFlag = 0  : m_nEnableFlag = MF_GRAYED|MF_BYCOMMAND;
	//	pPopup->EnableMenuItem( ID_FILE_NEW   , m_nEnableFlag);

		theApp.m_nLogonLevelMask & theApp.m_cAccess[ID_PROFILES_SAVEAS] ? m_nEnableFlag = 0  : m_nEnableFlag = MF_GRAYED|MF_BYCOMMAND;
		pPopup->EnableMenuItem( ID_FILE_SAVE_AS   , m_nEnableFlag);

		theApp.m_nLogonLevelMask & theApp.m_cAccess[ID_PROFILES_OPEN] ? m_nEnableFlag = 0 : m_nEnableFlag = MF_GRAYED|MF_BYCOMMAND;
		pPopup->EnableMenuItem( ID_FILE_OPEN   , m_nEnableFlag);
	}

}

BOOL CProfileSheet::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	C3DViewPage* pPage = (C3DViewPage*)GetActivePage();

	if(pPage == m_p3DViewPage) {
		pPage->MouseWheel(nFlags, zDelta, pt);
	}
	
	return CResizableSheet::OnMouseWheel(nFlags, zDelta, pt);
}

void CProfileSheet::ToolsDrawingoptions() 
{
	OnToolsDrawingoptions();
}

void CProfileSheet::OnToolsDrawingoptions() 
{
	if (m_p3DToolsSheet == NULL) {
		m_p3DToolsSheet = new C3DToolsSheet(_T("3D Tools"),this,0,(CWnd**)&m_p3DToolsSheet);
		m_p3DToolsSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_3D_TOOLS_SHEET]=TRUE;
	} else {
		m_p3DToolsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}	




void CProfileSheet::Invalidate3DImages()
{
	if(m_p3DViewPage != NULL && m_p3DViewPage->GetSafeHwnd()) {
		m_p3DViewPage->InitializeObjects();
		m_p3DViewPage->Invalidate(false);
	}
}

void CProfileSheet::OnCopyAllscanlinestaughtlines() 
{
	PROFILE->CopyAllLines(TAUGHTLINES , SCANLINES );
	UpdateAllPages();
}
