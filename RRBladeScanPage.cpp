// RRBladeScanPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RRBladeScanPage.h"
#include "MainFrm.h"
#include "FileThumbNailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRRBladeScanPage property page

IMPLEMENT_DYNCREATE(CRRBladeScanPage, CPropertyPage)

CRRBladeScanPage::CRRBladeScanPage() : CPropertyPage(CRRBladeScanPage::IDD)
{
	//{{AFX_DATA_INIT(CRRBladeScanPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CRRBladeScanPage::~CRRBladeScanPage()
{
}

void CRRBladeScanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRRBladeScanPage)
	DDX_Control(pDX, IDC_EDIT_PROFILENAME, m_editProfileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRRBladeScanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRRBladeScanPage)
	ON_BN_CLICKED(IDC_BUTTON_OUTSIDE, OnButtonOutside)
	ON_BN_CLICKED(IDC_BUTTON_INSIDE, OnButtonInside)
	ON_BN_CLICKED(IDC_BUTTON_ONE_LINE, OnButtonOneLine)
	ON_BN_CLICKED(IDC_BUTTON_XFER, OnButtonXfer)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS_LINE, OnButtonPreviousLine)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_LINE, OnButtonNextLine)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRRBladeScanPage message handlers

BOOL CRRBladeScanPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRRBladeScanPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;

	m_editProfileName.SetWindowText(PROFILE->m_FileName);

}

BOOL CRRBladeScanPage::OnSetActive() 
{
	
	
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	SetTimer(1,500,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CRRBladeScanPage::OnKillActive() 
{

	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CRRBladeScanPage::OnButtonOutside() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->StartBladeScan(BLADE_SCAN, OUTSIDE, SCAN_ALL);
}

void CRRBladeScanPage::OnButtonInside() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->StartBladeScan(BLADE_SCAN, INSIDE, SCAN_ALL);
}

void CRRBladeScanPage::OnButtonOneLine() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->StartBladeScan(BLADE_SCAN, theApp.m_Thread.m_nBladeSide, SCAN_CURRENT_LINE);
}

void CRRBladeScanPage::OnButtonXfer() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(pFrame->m_pViewBladeThicknessDlg[0]) pFrame->m_pViewBladeThicknessDlg[0]->SendMessage(WM_CLOSE);
}

void CRRBladeScanPage::OnButtonPreviousLine() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->StartBladeScan(BLADE_SCAN, theApp.m_Thread.m_nBladeSide, SCAN_PREVIOUS_LINE);
}

void CRRBladeScanPage::OnButtonNextLine() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->StartBladeScan(BLADE_SCAN, theApp.m_Thread.m_nBladeSide, SCAN_REMAINDER);
}

void CRRBladeScanPage::OnButtonBrowse() 
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
//			SetWindowTitle();
//			pFrame->SendMessage(UI_PROFILE_OPENED);
			pFrame->SendMessage(UI_PROFILE_OPENED,(WPARAM)this, NULL);
			theApp.ActivityLog(L"Profile loaded : " + FilePath);

			UpdateAllControls();


			pFrame->m_pScanViewBladeThicknessDlg = NULL;
			pFrame->StartBladeScan(0, theApp.m_Thread.m_nBladeSide, SCAN_ALL);	//first variable set to zero so nothing happens
		}
	}
	
}
