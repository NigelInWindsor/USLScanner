// ExampleAscanDisplay.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OEMPATool.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern UINT g_uiUTEventMessage;

// COEMPATool

BEGIN_MESSAGE_MAP(COEMPATool, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &COEMPATool::OnAppAbout)
END_MESSAGE_MAP()


// COEMPATool construction

COEMPATool::COEMPATool()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only COEMPATool object

COEMPATool theApp;


// COEMPATool initialization

wchar_t g_wcFolderData[MAX_PATH];
wchar_t g_wcFolderExe[MAX_PATH];
bool CheckPrivateMemory();
void SetWindowTime(SYSTEMTIME &time,CString &strWindowTime)
{
	strWindowTime.Format(L"%04d/%02d/%02d %02d:%02d:%02d.%03d",
		time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
}

bool GetWindowTime(CString &strWindowTime,SYSTEMTIME &time)
{
	int iYear=0, iMonth=0, iDay=0;
	int iHour=0, iMinute=0, iSecond=0, iMilliseconds=0;
	const wchar_t *pValue=strWindowTime;

	if(swscanf_s(pValue,L"%d", &iYear, sizeof(iYear))!=1)
		return false;
	pValue = wcsstr(pValue,L"/");
	if(!pValue)
		return false;
	pValue++;
	if(swscanf_s(pValue,L"%d", &iMonth, sizeof(iMonth))!=1)
		return false;
	pValue = wcsstr(pValue,L"/");
	if(!pValue)
		return false;
	pValue++;
	if(swscanf_s(pValue,L"%d", &iDay, sizeof(iDay))!=1)
		return false;
	pValue = wcsstr(pValue,L" ");
	if(!pValue)
		return false;
	pValue++;
	if(swscanf_s(pValue,L"%d", &iHour, sizeof(iHour))!=1)
		return false;
	pValue = wcsstr(pValue,L":");
	if(!pValue)
		return false;
	pValue++;
	if(swscanf_s(pValue,L"%d", &iMinute, sizeof(iMinute))!=1)
		return false;
	pValue = wcsstr(pValue,L":");
	if(!pValue)
		return false;
	pValue++;
	if(swscanf_s(pValue,L"%d", &iSecond, sizeof(iSecond))!=1)
		return false;
	pValue = wcsstr(pValue,L".");
	if(!pValue)
		return false;
	pValue++;
	if(swscanf_s(pValue,L"%d", &iMilliseconds, sizeof(iMilliseconds))!=1)
		return false;
	time.wYear = (WORD)iYear; time.wMonth = (WORD)iMonth; time.wDay = (WORD)iDay;
	time.wHour = (WORD)iHour; time.wMinute = (WORD)iMinute; time.wSecond = (WORD)iSecond;
	time.wMilliseconds = (WORD)iMilliseconds;
	return true;
}

void strExtract(const char *pcStart,const char *pcStop,wchar_t *pValue,int iSize)
{
	char cData[MAX_PATH];
	int iIndex;

	iIndex = 0;
	while(&pcStart[iIndex]!=pcStop)
	{
		cData[iIndex] = pcStart[iIndex];
		iIndex++;
	}
	cData[iIndex] = 0;
	if(!MultiByteToWideChar( CP_ACP , 0, cData, -1, pValue, iSize ))
		pValue[0] = 0;
}

bool GetFlashSWSectionItem(char *pBuffer,int iBufferSize,int &iIndexNext,wchar_t *pName,int iNameSize,wchar_t *pType,int iTypeSize,wchar_t *pValue,int iValueSize)
{
	const char *pcName,*pcType,*pcValue,*pcEnd,*pcCst;
	int iIndex=iIndexNext;

	pcCst = "Name=\"";
	pcName = strstr(&pBuffer[iIndex],pcCst);
	if(!pcName)
		return false;
	pcName += strlen(pcCst);
	pcCst = "\";Type=\"";
	pcType = strstr(pcName,pcCst);
	if(!pcType)
		return false;
	strExtract(pcName,pcType,pName,iNameSize);
	pcType += strlen(pcCst);
	pcCst = "\";Value=\"";
	pcValue = strstr(pcType,pcCst);
	if(!pcValue)
		return false;
	strExtract(pcType,pcValue,pType,iTypeSize);
	pcValue += strlen(pcCst);
	pcCst = "\";";
	pcEnd = strstr(pcValue,pcCst);
	if(!pcEnd)
		return false;
	strExtract(pcValue,pcEnd,pValue,iValueSize);
	pcEnd += strlen(pcCst);
	iIndex = (int)(pcEnd - &pBuffer[iIndex]);
	iIndexNext += iIndex;
	return true;
}

#ifdef _DEBUG
CMemoryState g_oldMemState, g_newMemState, g_diffMemState;
#endif _DEBUG
BOOL COEMPATool::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
//AfxMessageBox(L"iCycle/*pAscanHeader->cycle*/");
	wcscpy_s(g_wcFolderData,MAX_PATH,UTKernel_GetFolderData());
	CString string=AfxGetApp()->m_pszExeName; // Get the "MyExe" portion of "MyExe.exe". Or, "MyDll" if RunDll32 is used.
	string += L".exe";
	HMODULE hmod = GetModuleHandle(string);  
	GetModuleFileName(hmod,g_wcFolderExe,MAX_PATH);
	wchar_t *pSlash,*pSlashOld;
	pSlash = wcsstr(g_wcFolderExe,L"\\");
	pSlashOld = pSlash;
	while(pSlash)
	{
		pSlashOld = pSlash;
		pSlash = wcsstr(pSlash+1,L"\\");
	}
	if(pSlashOld)
		pSlashOld[1] = 0;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
#ifdef _EVALUATION_
	CUTPreference *pPref=CUTPreference::GetCurrentPreference();
	wchar_t wcComp[MAX_PATH];
	if(pPref && pPref->GetCompilationOption(MAX_PATH,wcComp) && !wcsstr(wcComp,L"Evaluation"))
	{
		PostQuitMessage( 0 );
		return FALSE;
	}
	pFrame->PostMessage(WM_COMMAND,MAKELONG(ID_EDIT_DIALOG_HARDWARE,0),0);
#endif _EVALUATION_
	return TRUE;
}

int COEMPATool::ExitInstance()
{
	UTKernel_Close();//clean the kernel
	return CWinApp::ExitInstance();
}

// COEMPATool message handlers




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void COEMPATool::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// COEMPATool message handlers

