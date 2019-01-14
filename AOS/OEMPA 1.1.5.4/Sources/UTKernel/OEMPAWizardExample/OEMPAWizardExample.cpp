// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// OEMPAWizardExample.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "OEMPAWizardExample.h"
#include "MainFrm.h"

#include "OEMPAWizardExampleDoc.h"
#include "OEMPAWizardExampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool WizardUpdateScan(int *piErrorChannel,int *piErrorScan);
const wchar_t *GetOEMPAWizardExamplePath();
bool ReadWizardWriteTemplate(bool bOverwrite);
bool ReadTemplateWriteWizard();
wchar_t g_wcFolderData[MAX_PATH];
wchar_t g_wcFolderKernel[MAX_PATH];
wchar_t g_wcFolderEmuMon[MAX_PATH];

// COEMPAWizardExampleApp

BEGIN_MESSAGE_MAP(COEMPAWizardExampleApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &COEMPAWizardExampleApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// COEMPAWizardExampleApp construction

COEMPAWizardExampleApp::COEMPAWizardExampleApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("OEMPAWizardExample.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only COEMPAWizardExampleApp object

COEMPAWizardExampleApp theApp;


// COEMPAWizardExampleApp initialization

BOOL COEMPAWizardExampleApp::InitInstance()
{
	wchar_t wcFolderExe[MAX_PATH];
	CString strFileName;
	DWORD dwAttributes;
	int iErrorChannel,iErrorScan;
	CString string;

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COEMPAWizardExampleDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(COEMPAWizardExampleView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	wcscpy_s(wcFolderExe,MAX_PATH,UTKernel_GetFolderExe());
	wcscat_s(wcFolderExe,MAX_PATH,L"TemplateFlashSWSection.txt");
	wcscpy_s(g_wcFolderData,MAX_PATH,UTKernel_GetFolderData());
	wcscat_s(g_wcFolderData,MAX_PATH,L"Files\\OEMPA\\");
	wcscpy_s(g_wcFolderKernel,MAX_PATH,UTKernel_GetFolderData());
	wcscat_s(g_wcFolderKernel,MAX_PATH,L"Files\\Kernel\\");
	wcscpy_s(g_wcFolderEmuMon,MAX_PATH,UTKernel_GetFolderData());
	wcscat_s(g_wcFolderEmuMon,MAX_PATH,L"Files\\EmuMon\\");
	strFileName = g_wcFolderEmuMon;
	strFileName += L"WizardTemplateFlashFile.txt";
	dwAttributes = GetFileAttributes(strFileName);
	if(dwAttributes==INVALID_FILE_ATTRIBUTES)
	{
		dwAttributes = GetFileAttributes(wcFolderExe);
		if(dwAttributes==INVALID_FILE_ATTRIBUTES)
		{
			AfxMessageBox(L"Eror : \"TemplateFlashSWSection.txt\" is missing!");
		}else{
			CopyFile(wcFolderExe,strFileName,FALSE);
			CHWDeviceOEMPA::SetFlashItem(strFileName,L"RXBoardCount",L"4");
			CHWDeviceOEMPA::SetFlashItem(strFileName,L"BoardIO",L"false");
			CHWDeviceOEMPA::SetFlashItem(strFileName,L"Multiplexer",L"0:0");
		}
	}

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

    // No file name passed on command line
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
    {
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
		//if (GetProfileInt(_T("Config"), _T("OpenAtStart")))
		{
            //cmdInfo.m_strFileName = GetProfileString(
            //    _T("Config"), _T("InitialFile"));
			strFileName = g_wcFolderKernel;
			strFileName += GetOEMPAWizardExamplePath();
			cmdInfo.m_strFileName = strFileName;//L"c:\\users\\public\\Files\\test_readback.txt";
            if (!cmdInfo.m_strFileName.IsEmpty())
                cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
        }
    }

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	CMainFrame *pMainFrame=dynamic_cast<CMainFrame*>(m_pMainWnd);
	COEMPAWizardExampleView *pView = NULL;
	if(pMainFrame)
		pView = (COEMPAWizardExampleView*)pMainFrame->GetActiveView();
	if(pView)
	{//to complete initialisation => update the wizard and scans
		pView->EditTemplate();
		ReadTemplateWriteWizard();//update the wizard
		if(!WizardUpdateScan(&iErrorChannel,&iErrorScan))//update scans
		{
			string.Format(L"Error to update channel index %d scan index %d!",iErrorChannel,iErrorScan);
			AfxMessageBox(string);
		}
	}
	return TRUE;
}

int COEMPAWizardExampleApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// COEMPAWizardExampleApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void COEMPAWizardExampleApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// COEMPAWizardExampleApp customization load/save methods

void COEMPAWizardExampleApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void COEMPAWizardExampleApp::LoadCustomState()
{
}

void COEMPAWizardExampleApp::SaveCustomState()
{
}

// COEMPAWizardExampleApp message handlers



