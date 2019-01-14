
// SectorWeld.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SectorApp.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "SectorAppDoc.h"
#include "SectorAppView.h"

#include "DynamicView.h"
#include "DynamicWnd.h"
#include "DynamicNCView.h"
#include "DynamicNCWnd.h"
#include "DynamicCscopeView.h"
#include "DynamicCscopeWnd.h"
#include "HardwareCscopeView.h"
#include "HardwareCscopeWnd.h"
#include "AscopeView.h"
#include "AscopeWnd.h"
#include "BscopeView.h"
#include "BscopeWnd.h"
#include "CscopeView.h"
#include "CscopeWnd.h"
#include "DscopeView.h"
#include "DscopeWnd.h"
#include "DscopeCorrectedView.h"
#include "DscopeCorrectedWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _TEST_DATA

#ifdef _TEST_DATA
int receptionCount = 0;

bool WINAPI CallbackAscan(int iSystem, int iCycle, double Encoder, const char* pData, int iSize)
{
	acq_data_ascan(iSystem, iCycle, Encoder, iSize, pData);

	return true;
}
bool WINAPI CallbackCscan(int iSystem, int iCycle, double Encoder, const structCscanAmp_0x0102 *pAmp, const structCscanAmpTof_0x0202 *pAmpTof, int iSize)
{
	acq_data_cscan(iSystem, iCycle, Encoder, iSize, pAmp, pAmpTof);

	return true;
}
#endif

CDynamicView* g_pDynamicView = NULL;
CDynamicNCView* g_pDynamicNCView = NULL;
CDynamicCscopeView* g_pDynamicCscope = NULL;
CHardwareCscopeView* g_pHardwareCscope[4+4] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
CAscopeView* g_pAscope = NULL;
CBscopeView* g_pBscope = NULL;
CCscopeView* g_pCscope = NULL;
CDscopeView* g_pDscope = NULL;
CDscopeCorrectedView* g_pDscopeCorrected = NULL;
CAscopeToolbar* g_pAscopeToolbar = NULL;

// CSectorAppApp

BEGIN_MESSAGE_MAP(CSectorAppApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CSectorAppApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CSectorAppApp construction

CSectorAppApp::CSectorAppApp()
{
	m_bHiColorIcons = TRUE;
	SetAppID(_T("OEMPASector.AppID.NoVersion"));
}

// The one and only CSectorAppApp object

CSectorAppApp theApp;

#ifndef _TEST_DATA
UINT ThreadFunc(LPVOID pvParam)
{
	int CountQty = 3;

	while (1)
	{
		if ((InfoProbe1.iCycleCount != 0) &&
			(InfoProbe1.PointQuantity != 0) &&
			(InfoProbe2.iCycleCount != 0) &&
			(InfoProbe2.PointQuantity != 0))
		{
			char* AcqData1 = new char[InfoProbe1.PointQuantity];
			char* AcqData2 = new char[InfoProbe2.PointQuantity];

			for (unsigned int i = 0 ; i < InfoProbe1.PointQuantity ; i++)
			{
				// values range from -100% (-128) to +100% (127)
				// All values are take rand(1) excepth some  (every CountQty)
				for (int j = 1 ; j < CountQty ; j++)
				{
					if (i == j * InfoProbe1.PointQuantity / CountQty - 1)
						AcqData1[i] = -128;
					else if (i == j * InfoProbe1.PointQuantity / CountQty)
						AcqData1[i] = 0;
					else if (i == j * InfoProbe1.PointQuantity / CountQty + 1)
						AcqData1[i] = 127;
					else if (i == j * InfoProbe1.PointQuantity / CountQty + 2)
						AcqData1[i] = 0;
					else
						AcqData1[i] = rand() % 2;
				}
			}

			for (unsigned int i = 0 ; i < InfoProbe2.PointQuantity ; i++)
			{
				// values range from -100% (-128) to +100% (127)
				// All values take to rand(1) excepth some  (every CountQty)
				for (int j = 1 ; j < CountQty ; j++)
				{
					if (i == j * InfoProbe2.PointQuantity / CountQty - 1)
						AcqData2[i] = -128;
					else if (i == j * InfoProbe2.PointQuantity / CountQty)
						AcqData2[i] = 0;
					else if (i == j * InfoProbe2.PointQuantity / CountQty + 1)
						AcqData2[i] = 127;
					else if (i == j * InfoProbe2.PointQuantity / CountQty + 2)
						AcqData2[i] = 0;
					else
						AcqData2[i] = rand() % 2;
				}
			}

			for (unsigned int i = 0 ; i < InfoProbe1.iCycleCount ; i++)
				acq_data_ascan(0, i, 0.002, InfoProbe1.PointQuantity, AcqData1);

			for (unsigned int i = 0 ; i < InfoProbe2.iCycleCount ; i++)
				acq_data_ascan(1, i, 0.002, InfoProbe2.PointQuantity, AcqData2);

			delete[] AcqData1;
			delete[] AcqData2;
		}

		Sleep(100);

		CountQty++;

		if (CountQty > 10)
			CountQty = 3;
	}

	return 0;
}
#endif

// CSectorAppApp initialization
void AllocDisplay(bool bAlloc);//integration

BOOL CSectorAppApp::InitInstance()
{
	CWinAppEx::InitInstance();

	EnableTaskbarInteraction(FALSE);
	SetRegistryKey(_T("OEMPASector"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Initialize the test data
	InitProgram();

#ifndef _TEST_DATA
	// Start the sending of dumb Ascan data for both probes
	m_pThread = AfxBeginThread(ThreadFunc, NULL);
#endif

	m_pDynamicViewTemplate = new CMultiDocTemplate(IDR_DYNAMIC,
												  RUNTIME_CLASS(CSectorAppDoc),
												  RUNTIME_CLASS(CDynamicWnd),
												  RUNTIME_CLASS(CDynamicView));

	if (m_pDynamicViewTemplate == NULL)
		return FALSE;

	AddDocTemplate(m_pDynamicViewTemplate);

	m_pDynamicNCViewTemplate = new CMultiDocTemplate(IDR_DYNAMIC_NC,
												  RUNTIME_CLASS(CSectorAppDoc),
												  RUNTIME_CLASS(CDynamicNCWnd),
												  RUNTIME_CLASS(CDynamicNCView));

	if (m_pDynamicNCViewTemplate == NULL)
		return FALSE;

	AddDocTemplate(m_pDynamicNCViewTemplate);

	m_pDynamicCscopeViewTemplate = new CMultiDocTemplate(IDR_DYNAMIC_CSCOPE,
														 RUNTIME_CLASS(CSectorAppDoc),
														 RUNTIME_CLASS(CDynamicCscopeWnd),
														 RUNTIME_CLASS(CDynamicCscopeView));

	if (m_pDynamicCscopeViewTemplate == NULL)
		return FALSE;

	AddDocTemplate(m_pDynamicCscopeViewTemplate);

	m_pHardwareCscopeViewTemplate = new CMultiDocTemplate(IDR_HARDWARE_CSCOPE,
														 RUNTIME_CLASS(CSectorAppDoc),
														 RUNTIME_CLASS(CHardwareCscopeWnd),
														 RUNTIME_CLASS(CHardwareCscopeView));

	if (m_pHardwareCscopeViewTemplate == NULL)
		return FALSE;

	AddDocTemplate(m_pHardwareCscopeViewTemplate);

	m_pAscopeViewTemplate = new CMultiDocTemplate(IDR_ASCOPE,
												  RUNTIME_CLASS(CSectorAppDoc),
												  RUNTIME_CLASS(CAscopeWnd),
												  RUNTIME_CLASS(CAscopeView));

	if (m_pAscopeViewTemplate == NULL)
		return FALSE;

	AddDocTemplate(m_pAscopeViewTemplate);

	m_pBscopeViewTemplate = new CMultiDocTemplate(IDR_BSCOPE,
												  RUNTIME_CLASS(CSectorAppDoc),
												  RUNTIME_CLASS(CBscopeWnd),
												  RUNTIME_CLASS(CBscopeView));

	if (m_pBscopeViewTemplate == NULL)
		return FALSE;

	AddDocTemplate(m_pBscopeViewTemplate);


	m_pCscopeViewTemplate = new CMultiDocTemplate(IDR_CSCOPE,
												  RUNTIME_CLASS(CSectorAppDoc),
												  RUNTIME_CLASS(CCscopeWnd),
												  RUNTIME_CLASS(CCscopeView));

	if (m_pCscopeViewTemplate == NULL)
		return FALSE;

	AddDocTemplate(m_pCscopeViewTemplate);


	m_pDscopeViewTemplate = new CMultiDocTemplate(IDR_DSCOPE,
												  RUNTIME_CLASS(CSectorAppDoc),
												  RUNTIME_CLASS(CDscopeWnd),
												  RUNTIME_CLASS(CDscopeView));

	if (m_pDscopeViewTemplate == NULL)
		return FALSE;

	AddDocTemplate(m_pDscopeViewTemplate);

	m_pDscopeCorrectedViewTemplate = new CMultiDocTemplate(IDR_DSCOPE_CORRECTED,
												  RUNTIME_CLASS(CSectorAppDoc),
												  RUNTIME_CLASS(CDscopeCorrectedWnd),
												  RUNTIME_CLASS(CDscopeCorrectedView));
	if (m_pDscopeCorrectedViewTemplate == NULL)
		return FALSE;
	AddDocTemplate(m_pDscopeCorrectedViewTemplate);

	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	if(pMainFrame)
		pMainFrame->InitPlacement();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;	// Not open view
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

#ifdef _TEST_DATA
	bool Error = DemoInit(1);

	if (Error == false)
		return FALSE;

	SetDemoCallback(CallbackAscan,CallbackCscan);
	SetAllocDisplay(AllocDisplay,pMainFrame);//integration
#endif

	return TRUE;
}

int CSectorAppApp::ExitInstance()
{

#ifdef _TEST_DATA
	DemoRun(false, 0.0, NULL);
	DemoExit();
#else
	if (m_pThread)
		::TerminateThread(m_pThread->m_hThread, 0);
#endif

	CloseProgram();
	return CWinAppEx::ExitInstance();
}

// CSectorAppApp message handlers
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
void CSectorAppApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSectorAppApp customization load/save methods

void CSectorAppApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
}

void CSectorAppApp::LoadCustomState()
{
}

void CSectorAppApp::SaveCustomState()
{
}