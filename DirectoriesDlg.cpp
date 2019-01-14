// DirectoriesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "DirectoriesDlg.h"
#include "MainFrm.h"
#include <afxwin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirectoriesDlg dialog

#define LASTSETTINGS_DIR	0
#define	PROFILE_DIR			1
#define	IMAGES_DIR			2
#define	APTSOURCE_DIR		3

IMPLEMENT_DYNCREATE(CDirectoriesDlg, CPropertyPage)

CDirectoriesDlg::CDirectoriesDlg(CWnd* pParent /*=NULL*/) : CPropertyPage(CDirectoriesDlg::IDD)
{
	//{{AFX_DATA_INIT(CDirectoriesDlg)
	//}}AFX_DATA_INIT
	m_nFocus = -1;
}

CDirectoriesDlg::~CDirectoriesDlg()
{
}

void CDirectoriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirectoriesDlg)
	DDX_Control(pDX, IDC_EDIT_WORKSPACE_PATH, m_editWorkSpacePath);
	DDX_Control(pDX, IDC_EDIT_CALIBRATION_PATH, m_editCalibrationPath);
	DDX_Control(pDX, IDC_EDIT_WORDPAD_PATHNAME, m_editWordPadPathName);
	DDX_Control(pDX, IDC_EDIT_USLSCANNER_MANUAL_PATHNAME, m_editUSLScannerManualPathName);
	DDX_Control(pDX, IDC_EDIT_PMAC_MANUAL_PATHNAME, m_editPmacManualPathName);
	DDX_Control(pDX, IDC_EDIT_ACROBAT_PATHNAME, m_editAcrobatPathName);
	DDX_Control(pDX, IDC_EDIT_ANDON_PATHNAME, m_editAndonPathName);
	DDX_Control(pDX, IDC_EDIT_DATA_BASE_FAIL, m_editDataBaseFail);
	DDX_Control(pDX, IDC_EDIT_PROBES_PATH, m_editProbesPath);
	DDX_Control(pDX, IDC_EDIT_HELP_FILE, m_editHelpPath);
	DDX_Control(pDX, IDC_EDIT_DATA_BASE, m_editDataBase);
	DDX_Control(pDX, IDC_CHECK_LASTACCESSED, m_checkLastAccessed);
	DDX_Control(pDX, IDC_EDIT_ULTRASONICS_PATH, m_editUltrasonicsPath);
	DDX_Control(pDX, IDC_EDIT_SCAN_PATH, m_editScanPath);
	DDX_Control(pDX, IDC_EDIT_PALETTE_PATH, m_editPalettePath);
	DDX_Control(pDX, IDC_EDIT_APTSOURCE_PATH, m_editAptSourcePath);
	DDX_Control(pDX, IDC_EDIT_PROFILES_PATH, m_editProfilesPath);
	DDX_Control(pDX, IDC_EDIT_LASTSETTINGS_PATH, m_editLastSettingsPath);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_STORE, m_listStorePath);
	DDX_Control(pDX, IDC_EDIT_WORKING_DIRECTORY, m_editWorkingDirectory);
	DDX_Control(pDX, IDC_EDIT_REGISTRY_KEY, m_editRegistryKey);
}


BEGIN_MESSAGE_MAP(CDirectoriesDlg, CDialog)
	//{{AFX_MSG_MAP(CDirectoriesDlg)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDBLCLK()
	ON_EN_CHANGE(IDC_EDIT_LASTSETTINGS_PATH, OnChangeEditLastsettingsPath)
	ON_EN_CHANGE(IDC_EDIT_PROFILES_PATH, OnChangeEditProfilesPath)
	ON_EN_CHANGE(IDC_EDIT_APTSOURCE_PATH, OnChangeEditAptsourcePath)
	ON_BN_CLICKED(IDC_BUTTON_LASTSETTINGS, OnButtonLastsettings)
	ON_BN_CLICKED(IDC_BUTTON_PROFILE, OnButtonProfile)
	ON_EN_CHANGE(IDC_EDIT_PALETTE_PATH, OnChangeEditPalettePath)
	ON_EN_CHANGE(IDC_EDIT_SCAN_PATH, OnChangeEditScanPath)
	ON_EN_CHANGE(IDC_EDIT_ULTRASONICS_PATH, OnChangeEditUltrasonicsPath)
	ON_BN_CLICKED(IDC_BUTTON_PALETTES, OnButtonPalettes)
	ON_BN_CLICKED(IDC_BUTTON_IMAGES, OnButtonImages)
	ON_BN_CLICKED(IDC_BUTTON_APTSOURCE, OnButtonAptsource)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_ULTRASONICS, OnButtonUltrasonics)
	ON_BN_CLICKED(IDC_CHECK_LASTACCESSED, OnCheckLastaccessed)
	ON_BN_CLICKED(IDC_BUTTON_DATA_BASE, OnButtonDataBase)
	ON_EN_CHANGE(IDC_EDIT_DATA_BASE, OnChangeEditDataBase)
	ON_EN_CHANGE(IDC_EDIT_HELP_FILE, OnChangeEditHelpFile)
	ON_BN_CLICKED(IDC_BUTTON_HELP_FILE, OnButtonHelpFile)
	ON_EN_CHANGE(IDC_EDIT_PROBES_PATH, OnChangeEditProbesPath)
	ON_BN_CLICKED(IDC_BUTTON_PROBES, OnButtonProbes)
	ON_BN_CLICKED(IDC_BUTTON_DATA_BASE_FAIL, OnButtonDataBaseFail)
	ON_EN_CHANGE(IDC_EDIT_DATA_BASE_FAIL, OnChangeEditDataBaseFail)
	ON_BN_CLICKED(IDC_BUTTON_ANDON, OnButtonAndon)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_ACROBAT, OnButtonAcrobat)
	ON_BN_CLICKED(IDC_BUTTON_USLSCANNERMANUAL, OnButtonUslscannermanual)
	ON_BN_CLICKED(IDC_BUTTON_PMACMANUAL, OnButtonPmacmanual)
	ON_EN_CHANGE(IDC_EDIT_ACROBAT_PATHNAME, OnChangeEditAcrobatPathname)
	ON_EN_CHANGE(IDC_EDIT_USLSCANNER_MANUAL_PATHNAME, OnChangeEditUslscannerManualPathname)
	ON_EN_CHANGE(IDC_EDIT_PMAC_MANUAL_PATHNAME, OnChangeEditPmacManualPathname)
	ON_EN_CHANGE(IDC_EDIT_WORDPAD_PATHNAME, OnChangeEditWordpadPathname)
	ON_BN_CLICKED(IDC_BUTTON_WORDPAD, OnButtonWordpad)
	ON_EN_CHANGE(IDC_EDIT_CALIBRATION_PATH, OnChangeEditCalibrationPath)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION, OnButtonCalibration)
	ON_BN_CLICKED(IDC_BUTTON_WORKSPACE, OnButtonWorkspace)
	ON_EN_CHANGE(IDC_EDIT_WORKSPACE_PATH, OnChangeEditWorkspacePath)
	//}}AFX_MSG_MAP
	ON_CLBN_CHKCHANGE(IDC_LIST_STORE, OnCheckChangeListStorePath)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirectoriesDlg message handlers

BOOL CDirectoriesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_editLastSettingsPath.SetWindowText(theApp.m_LastSettingsPath);
	m_editProfilesPath.SetWindowText(theApp.m_ProfilePath);
	m_editWorkSpacePath.SetWindowText(theApp.m_WorkspacePathName);
	m_editProbesPath.SetWindowText(theApp.m_ProbesPath);
	m_editAptSourcePath.SetWindowText(theApp.m_AptSourcePath);
	m_editUltrasonicsPath.SetWindowText(theApp.m_UltrasonicsPath);
	m_editPalettePath.SetWindowText(theApp.m_PalettePath);
	m_editScanPath.SetWindowText(theApp.m_ScanPath);
	m_editDataBase.SetWindowText(theApp.m_DataBasePathName);
	m_editDataBaseFail.SetWindowText(theApp.m_DataBaseFailedToSavePathName);
	m_editHelpPath.SetWindowText(theApp.m_HelpPathName);

	m_checkLastAccessed.SetCheck(theApp.m_LastSettings.bUpdateDirectoryPath);

	m_listStorePath.ResetContent();
	m_listStorePath.SetCheckStyle(BS_AUTOCHECKBOX);
	for(int ii=0;ii<10;ii++) {
		m_listStorePath.AddString(theApp.m_StorePath[ii]);
		m_listStorePath.SetCheck(ii, theApp.m_LastSettings.nUseStorePath[ii]);
	}
	m_editAndonPathName.SetWindowText(theApp.m_AndonPathName);

	m_editAcrobatPathName.SetWindowText(theApp.m_AcrobatPathName);
	m_editWordPadPathName.SetWindowText(theApp.m_WordPadPathName);
	m_editUSLScannerManualPathName.SetWindowText(theApp.m_USLScannerManualPathName);
	m_editPmacManualPathName.SetWindowText(theApp.m_PMACSRMPathName);
	m_editCalibrationPath.SetWindowText(theApp.m_CalibrationPathName);

	m_editWorkingDirectory.SetWindowTextW(theApp.m_WorkingDirectory);
	CAtlTransactionManager TM;
	HKEY Key = theApp.GetAppRegistryKey(&TM);
//	if (Key) {
//		m_editRegistryKey.SetWindowTextW(Key->unused);
//	}

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDirectoriesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CDirectoriesDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDblClk(nFlags, point);
}



void CDirectoriesDlg::OnChangeEditLastsettingsPath() 
{
	m_editLastSettingsPath.GetWindowText(theApp.m_LastSettingsPath);
}

void CDirectoriesDlg::OnChangeEditProfilesPath() 
{
	m_editProfilesPath.GetWindowText(theApp.m_ProfilePath);
}



void CDirectoriesDlg::OnChangeEditAptsourcePath() 
{
	m_editAptSourcePath.GetWindowText(theApp.m_AptSourcePath);
}

void CDirectoriesDlg::OnChangeEditPalettePath() 
{
	m_editPalettePath.GetWindowText(theApp.m_PalettePath);
}

void CDirectoriesDlg::OnChangeEditScanPath() 
{
	m_editScanPath.GetWindowText(theApp.m_ScanPath);
}

void CDirectoriesDlg::OnChangeEditUltrasonicsPath() 
{
	m_editUltrasonicsPath.GetWindowText(theApp.m_UltrasonicsPath);
}

void CDirectoriesDlg::OnButtonApply() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(pFrame) pFrame->StatusBarPrompt(L"Saving directories",500);
	theApp.SaveRegistryVariables();
	if(pFrame) pFrame->StatusBarPrompt(L"Ready",500);
}




void CDirectoriesDlg::GetDirectory(CEdit *pEdit, CString *pInitDir)
{
	BROWSEINFO bi;
	ITEMIDLIST* li;
	WCHAR DisplayName[MAX_PATH];
	WCHAR Title[MAX_PATH]={_T("Select new directory")};
	CString Buff;

	memset(&bi,0,sizeof bi);
	memset(DisplayName,0,sizeof DisplayName);
	bi.ulFlags = BIF_EDITBOX;
	bi.pszDisplayName = DisplayName;
	bi.lpszTitle = Title;
	li=SHBrowseForFolder(&bi);
	if(SHGetPathFromIDList(li,DisplayName) ) {
		Buff.Format(_T("%s"),DisplayName);
		if(pEdit) pEdit->SetWindowText(Buff);
		if(pInitDir) pInitDir->Format(_T("%s"),Buff);
	};	

}
void CDirectoriesDlg::OnButtonLastsettings() 
{
	GetDirectory(&m_editLastSettingsPath,&theApp.m_LastSettingsPath);
}

void CDirectoriesDlg::OnButtonProfile() 
{
	GetDirectory(&m_editProfilesPath,&theApp.m_ProfilePath);
}


void CDirectoriesDlg::OnButtonPalettes() 
{
	GetDirectory(&m_editPalettePath,&theApp.m_PalettePath);
}


void CDirectoriesDlg::OnButtonAptsource() 
{
	GetDirectory(&m_editAptSourcePath,&theApp.m_AptSourcePath);
}

void CDirectoriesDlg::OnButtonScan() 
{
	GetDirectory(&m_editScanPath,&theApp.m_ScanPath);
}

void CDirectoriesDlg::OnButtonUltrasonics() 
{
	GetDirectory(&m_editUltrasonicsPath,&theApp.m_UltrasonicsPath);
}

void CDirectoriesDlg::OnCheckLastaccessed() 
{
	theApp.m_LastSettings.bUpdateDirectoryPath = m_checkLastAccessed.GetCheck() & 0x01;
}

void CDirectoriesDlg::OnButtonDataBase() 
{
	CFileDialog FileDlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data Base (*.txt)|*.txt|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_DataBasePathName;
	if(FileDlg.DoModal()==IDOK) {
		theApp.m_DataBasePathName=FileDlg.GetPathName();
		m_editDataBase.SetWindowText(theApp.m_DataBasePathName);
	}
	
}

void CDirectoriesDlg::OnChangeEditDataBase() 
{
	m_editDataBase.GetWindowText(theApp.m_DataBasePathName);
}

void CDirectoriesDlg::OnChangeEditHelpFile() 
{
	m_editHelpPath.GetWindowText(theApp.m_HelpPathName);
}

void CDirectoriesDlg::OnButtonHelpFile() 
{
	CFileDialog FileDlg(TRUE,_T(".chm"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Compiled HTML (*.chm)|*.chm|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_HelpPathName;
	if(FileDlg.DoModal()==IDOK) {
		theApp.m_HelpPathName=FileDlg.GetPathName();
		m_editHelpPath.SetWindowText(theApp.m_HelpPathName);
	}
}

void CDirectoriesDlg::OnChangeEditProbesPath() 
{
	m_editProbesPath.GetWindowText(theApp.m_ProbesPath);
}

void CDirectoriesDlg::OnButtonProbes() 
{
	GetDirectory(&m_editProbesPath,&theApp.m_ProbesPath);
}

void CDirectoriesDlg::OnButtonImages() 
{
	int nn = m_listStorePath.GetCurSel();
	MinMax(&nn, 0, 9);
	GetDirectory(NULL,&theApp.m_StorePath[nn]);

	m_listStorePath.ResetContent();
	m_listStorePath.SetCheckStyle(BS_AUTOCHECKBOX);
	for(int ii=0;ii<10;ii++) {
		m_listStorePath.AddString(theApp.m_StorePath[ii]);
	}

}

void CDirectoriesDlg::OnCheckChangeListStorePath()
{
	for(int ii=0;ii<10;ii++) {
		theApp.m_LastSettings.nUseStorePath[ii] = m_listStorePath.GetCheck(ii);
	}

}

void CDirectoriesDlg::OnButtonDataBaseFail() 
{
	CFileDialog FileDlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data Base (*.txt)|*.txt|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_DataBaseFailedToSavePathName;
	if(FileDlg.DoModal()==IDOK) {
		theApp.m_DataBaseFailedToSavePathName=FileDlg.GetPathName();
		m_editDataBaseFail.SetWindowText(theApp.m_DataBaseFailedToSavePathName);
	}
}

void CDirectoriesDlg::OnChangeEditDataBaseFail() 
{
	m_editDataBaseFail.GetWindowText(theApp.m_DataBaseFailedToSavePathName);
}


void CDirectoriesDlg::OnButtonAndon() 
{
	CFileDialog FileDlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Andon (*.txt)|*.txt|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_AndonPathName;
	if(FileDlg.DoModal()==IDOK) {
		theApp.m_AndonPathName=FileDlg.GetPathName();
		m_editAndonPathName.SetWindowText(theApp.m_AndonPathName);
	}
}


void CDirectoriesDlg::OnButtonAcrobat() 
{
	CFileDialog FileDlg(TRUE,_T(".exe"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Application (*.exe)|*.exe|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_AcrobatPathName;
	if(FileDlg.DoModal()==IDOK) {
		theApp.m_AcrobatPathName=FileDlg.GetPathName();
		m_editAcrobatPathName.SetWindowText(theApp.m_AcrobatPathName);
	}
}

void CDirectoriesDlg::OnButtonUslscannermanual() 
{
	CFileDialog FileDlg(TRUE,_T(".pdf"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Acrobat (*.pdf)|*.pdf|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_USLScannerManualPathName;
	if(FileDlg.DoModal()==IDOK) {
		theApp.m_USLScannerManualPathName=FileDlg.GetPathName();
		m_editUSLScannerManualPathName.SetWindowText(theApp.m_USLScannerManualPathName);
	}
}

void CDirectoriesDlg::OnButtonPmacmanual() 
{
	CFileDialog FileDlg(TRUE,_T(".pdf"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Acrobat (*.pdf)|*.pdf|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_PMACSRMPathName;
	if(FileDlg.DoModal()==IDOK) {
		theApp.m_PMACSRMPathName=FileDlg.GetPathName();
		m_editPmacManualPathName.SetWindowText(theApp.m_PMACSRMPathName);
	}
}

void CDirectoriesDlg::OnChangeEditAcrobatPathname() 
{
	m_editAcrobatPathName.GetWindowText(theApp.m_AcrobatPathName);
}

void CDirectoriesDlg::OnChangeEditUslscannerManualPathname() 
{
	m_editUSLScannerManualPathName.GetWindowText(theApp.m_USLScannerManualPathName);
}

void CDirectoriesDlg::OnChangeEditPmacManualPathname() 
{
	m_editPmacManualPathName.GetWindowText(theApp.m_PMACSRMPathName);
}

void CDirectoriesDlg::OnChangeEditWordpadPathname() 
{
	m_editWordPadPathName.GetWindowText(theApp.m_WordPadPathName);
}

void CDirectoriesDlg::OnButtonWordpad() 
{
	CFileDialog FileDlg(TRUE,_T(".exe"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Application (*.exe)|*.exe|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_WordPadPathName;
	if(FileDlg.DoModal()==IDOK) {
		theApp.m_WordPadPathName=FileDlg.GetPathName();
		m_editWordPadPathName.SetWindowText(theApp.m_WordPadPathName);
	}
	
}

void CDirectoriesDlg::OnChangeEditCalibrationPath() 
{
	m_editCalibrationPath.GetWindowText(theApp.m_CalibrationPathName);
}

void CDirectoriesDlg::OnButtonCalibration() 
{
	GetDirectory(&m_editCalibrationPath,&theApp.m_CalibrationPathName);
}

void CDirectoriesDlg::OnButtonWorkspace() 
{
	GetDirectory(&m_editWorkSpacePath,&theApp.m_WorkspacePathName);
}

void CDirectoriesDlg::OnChangeEditWorkspacePath() 
{
	m_editWorkSpacePath.GetWindowText(theApp.m_WorkspacePathName);
}
