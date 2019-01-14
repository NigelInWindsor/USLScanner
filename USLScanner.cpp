// USLScanner.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "USLScanner.h"
#include "RegisterActiveX.h"
#include "Winbase.h"
#include "EvaluationDlg.h"
#include "Token.h"
#include "direct.h"
#include <afxsock.h>
//#include ".\USLDLL\USLDLL.h"
//#pragma comment(lib,"USLDLL.lib")



#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CUSLScannerApp

BEGIN_MESSAGE_MAP(CUSLScannerApp, CWinAppEx)
	//{{AFX_MSG_MAP(CUSLScannerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSLScannerApp construction


CUSLScannerApp::CUSLScannerApp()
{
	CheckQuaternionMaths();
	//Set up memory leak detection.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#ifndef NO_REGISTRY
	SetAppID(_T("USLScannner.AppID.500"));
#endif // !NO_REGISTRY

	m_Strings.SetSize(40);
}

void CUSLScannerApp::SuspendThreads()
{
	m_Thread.StopCurrentThreadAction();
	m_SerialJoystick.Close();
	m_Acqiris.SuspendThread();
	m_Alazar.SuspendThread();
	m_PmacUser.DisableInterrupt();
	Sleep(500);

}

void CUSLScannerApp::ResumeThreads()
{
	m_PmacUser.EnableInterrupt();
	if( (m_LastSettings.nAdcCardType==Alazar_9626) || (m_LastSettings.nAdcCardType==Alazar_9462) || (m_LastSettings.nAdcCardType==Alazar_860) ) {
		m_Alazar.StartThread();
	}

	if(m_LastSettings.nAdcCardType == Acqiris_DP210) {
		m_Acqiris.StartThread();
	}

	m_SerialJoystick.Start();
	m_Thread.m_nThreadAction = JOYSTICK_START;

}


CUSLScannerApp::~CUSLScannerApp()
{
#ifdef NPTRACKINGTOOLS
//	SAFE_DELETE(m_pTracker);
//	SAFE_DELETE(m_pV120Trio);
#endif
}

int CUSLScannerApp::ExitInstance()
{
	ActivityLog(L"Exit program");

	for (int ii = 0; ii < MAX_THREAD_COUNT; ii++) {
		if (m_USLThread[ii].IsActive()) {
			AfxMessageBox(L"Thread Running");
		};
	}

	m_AOSPhasedArray.Disconnect();
	m_Palette.Destroy();
	m_FaroWorkerThread.OnDisconnect();
	SAFE_DELETE(m_pFaroTracker);

	m_DataTranslationHW.Close();
	m_Si10User.DeRegisterInterruptCallback();
	m_Motors.ZBrakesEnable(TRUE, 0);
	m_Motors.ZBrakesEnable(TRUE, 1);
	m_PmacUser.ReEnableAllLimitSwitches();

	m_FBCtrl.KillMotors();

	if (m_nExitMode == 0) {
		CheckLastSettingsPathName();
		m_Scope.Save(m_LastSettingsPath + L"\\Last.sco");
		m_Palette.Save(m_LastSettingsPath + L"\\Last.pal");
		m_UtUser.Save(m_LastSettingsPath + L"\\Last.ul");
		m_Kinematics.SaveManipulator(m_LastSettingsPath + L"\\Last.man");
		m_PhasedArray[PORTSIDE].Save(m_LastSettingsPath + L"\\Last_PhasedArray_Portside.pa");
		m_PhasedArray[STARBOARD].Save(m_LastSettingsPath + L"\\Last_PhasedArray_Starboard.pa");
		m_ToolChangers.Save(m_LastSettingsPath + L"\\ToolChangers.tools");

		SaveLastSettings();
		SaveTank();
		SaveRegistryVariables();
		SaveMaintenanceVariables();
		SaveRestore();
	}
	m_RemoteInterface.Close();
	m_SafetySerial.SuspendThread();
	m_SerialJoystick.Close();
	m_Acqiris.SuspendThread();
	m_Alazar.SuspendThread();
	m_Spectrum.SuspendThread();

	if (m_pDescription) {
		for (int nn = 0; nn<m_nDescriptionL; nn++) {
			SAFE_DELETE(m_pDescription[nn].pCaption);
		}
		SAFE_DELETE(m_pDescription);
	}

	SAFE_DELETE_ARRAY(m_pLadder);
	SAFE_DELETE_ARRAY(m_pCopyManyLine);
	SAFE_DELETE(m_pFLSC10);
	SAFE_DELETE(m_pEgismosRangefinder);
	SAFE_DELETE(m_pRemote);

	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

void CUSLScannerApp::SaveRestore()
{
	CUSLFile File;
	CFileException e;
	CString Path = m_WorkingDirectory + "\\Restore";
	CString TypBuff,Buff;
	CTime CurrentTime = CTime::GetCurrentTime();
	CString Date = L"Restore " + m_ProgramName + L" "+ CurrentTime.Format(L"%d, %b, %Y");

	::CreateDirectory((LPTSTR)Path.GetBuffer(200), (LPSECURITY_ATTRIBUTES)NULL);

	if(!File.Open(Path + "\\" + Date + ".res", CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
		File.DecodeFileException(&e,&Buff,File.GetFilePath());
		TypBuff.LoadString(IDS_Warning);
		MessageBox(NULL,Buff, TypBuff, MB_ICONERROR);
	} else {
		SaveLastSettings(&File);
		SaveTank(&File);
		m_Palette.Save(&File);
		m_Scope.Save(&File);
		m_Kinematics.SaveManipulator(&File);
		m_ToolChangers.Save(&File);

		File.Close();
	}

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUSLScannerApp object

CUSLScannerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUSLScannerApp initialization

BOOL CUSLScannerApp::InitInstance()
{
///////////////////////////////////////////////////////////////////////////////
	CString Buff,FilePath;
	char Identifier[60]={"Ultrasonic Sciences Ltd USLScanner"};
	char str[60];
	float fVersion;
	bool bRestore=FALSE;

	// create and load the frame with its resources
	//	CleanState();
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxOleInit())
	{
		AfxMessageBox(L"Failed to initialize Ole()");
		return FALSE;
	}

	AfxEnableControlContainer();

	SetRegistryKey(_T("USL"));

	m_fVersion = 5.02f; //WHEN INCREMENTING REMEMBER TO ADD VERISON EVALUATIONDLG::GETVERSIONUMBER!!!!!
	m_strVersion.Format(_T("USLScanner Version %.02f %s"),m_fVersion,(CString)__DATE__);

	m_nLicenseMask = _MINIMAL;
	m_nLicenseMask = _ALL;

	if(!FirstInstance(m_strVersion)) {
		AfxMessageBox(L"You may only have one instance of USLScanner\r\nThe previous version is open now.");
		exit(m_nExitMode=1);
	}

	if(!AfxSocketInit()) {
		AfxMessageBox(L"Sockets initialization failed");
	}

	m_nLine = 0;
	m_nIndex = 0;
	m_nElementIndex = 0;
	m_nLanguage = 0;
	m_nPrimarySide = PORTSIDE;
	m_nSecondarySide = STARBOARD;
	m_nSide0Orientation = 0;
	m_pDescription = NULL;
	m_nDescriptionL = 0;
	memset(m_cAccess,0xff,sizeof m_cAccess);
	m_n3DImageType = 0;
	m_n3DImageNumber = 0;
	m_n3DPaletteNumber = 0;
	m_p3DData = NULL;
	m_b3DModified = FALSE;
	m_bRThetaJoystick = false;
	m_pScanData = NULL;
	m_pViewData = NULL;
	m_pViewSheet = NULL;
	m_bCtrlButton = false;
	m_bShiftButton = false;
	CStringArray Params;
	int ParamsL;
	CString RestoreFilter;
	m_nCADTool = CAD_MOUSE;
	m_nOldAndonAction = MachineFault;
	m_pFaroTracker = NULL;
	m_Degrees = "\x0b0";
	m_nNumADCs = 0;
	m_nExitMode = 0;
	m_fXScanOffset = 0.0f;
	m_fYScanOffset = 0.0f;
	m_fZScanOffset = 0.0f;
	m_fRotateAboutXDuringScan = 0.0f;
	m_fRotateAboutYDuringScan = 0.0f;
	m_fRotateAboutZDuringScan = 0.0f;
	m_pLoggingAr = NULL;
	m_nSuspendProcessingCount = 0;
	m_pFLSC10 = NULL;


/*
	if(m_usbKey.GetNumDevices()<1) {
		MessageBox(NULL,_T("Unable to Find USB Dongle"),_T("Error"),NULL);
		return NULL;
	}

	if(!m_usbKey.IsValid(0)) { //Version
		MessageBox(NULL,_T("USB Dongle Invalid"),_T("Error"), NULL);
		return NULL;		
	}
*/



	m_ProgramName = "USLScanner"; //default

	ParamsL = SplitString(AfxGetApp()->m_lpCmdLine, &Params, 10, true);

	//First pass check for Program name
	for(int ii=0;ii<ParamsL;ii++){
		Buff = Params.GetAt(ii);
		if(Buff.Find(L"ProgramName") == 0) {
			m_ProgramName = Params.GetAt(ii+1);
			m_strVersion += L"   PROGRAM -- " + m_ProgramName;
		}
	}

	GlobalDefaults();
	GetWorkingDirectory();

#ifndef NO_REGISTRY
	CRegisterActiveX RegisterInstance;
	RegisterInstance.Register(L"uslbar.ocx");	// Regsvr32 /u USLBar.ocx followed by Regsvr32 USLBar.ocx
	m_RRMES.RetrieveRegistryVariables();
#endif // !NO_REGISTRY

	m_LastSettingsPath.Format(L"%s\\Config\\Ver%.02f", m_WorkingDirectory, m_fVersion);
	m_StorePath[0]=m_WorkingDirectory+L"\\Config";
	m_ArchivePath=m_WorkingDirectory+L"\\Config";
	m_ProfilePath=m_WorkingDirectory+L"\\Config";
	m_AptSourcePath=m_WorkingDirectory+L"\\Config";
	m_PalettePath=m_WorkingDirectory+L"\\Config";
	m_UltrasonicsPath=m_WorkingDirectory+L"\\Config";
	m_ScanPath=m_WorkingDirectory+L"\\Config";
	m_PmacPath=m_WorkingDirectory+L"\\Config";
	m_ProbesPath=m_WorkingDirectory+L"\\Config";
	m_PmacProgramPath=m_WorkingDirectory+L"\\Config";
	m_LimitAndKinematicOffset.Empty();
	m_DlgSideName[0] = "Left";
	m_DlgSideName[1] = "Right";
	m_AcrobatPathName = "C:\\Program Files\\Adobe\\Acrobat 7.0\\Reader\\AcroRd32.exe";
	m_WordPadPathName = "C:\\Program Files\\Windows NT\\Accessories\\wordpad.exe";
	m_USLScannerManualPathName = "C:\\USLScanner.pdf";
	m_PMACSRMPathName = "C:\\TURBO SRM.pdf";
	m_TrackerPathName = m_WorkingDirectory+L"\\Config";
	m_WorkspacePathName = m_WorkingDirectory+L"\\Config";
	m_3PtName[0] = "Red";
	m_3PtName[1] = "Green";
	m_3PtName[2] = "Blue";

	m_DataBasePathName=m_WorkingDirectory+L"\\Config\\DataBase.txt";
	m_DataBaseFailedToSavePathName=m_WorkingDirectory+L"\\Config\\DataBaseFail.txt";
	m_ViewReportTemplateName = "LastReport.tem";
	m_DetailsWindowName = "Details";

	m_nSecondarySide = m_nPrimarySide ^ 1;

	m_nLadderL = 0;
	m_pLadder = NULL;
	m_pCopyManyLine = NULL;
	m_nManyLineL = 0;

	QueryPerformanceFrequency(&m_CPUFrequency);

	RetrieveRegistryVariables();
	RetrieveCardEntries();

	SetLanguage();
	CString RestoreDir = m_WorkingDirectory + "\\Restore";

	m_pBootUpDlg = new CBootUpDlg((CWnd*)this);
	m_pBootUpDlg->Create(CBootUpDlg::IDD,(CWnd*)this);

	m_pBootUpDlg->UpdatePrompt(L"Check Screen Settings");
	CheckScreenSettings();

	//Second Pass look for Restore
	for(int ii=0;ii<ParamsL;ii++){
		if(Params.GetAt(ii)=="restore") {
			bRestore=TRUE;
			RestoreFilter.Format(_T("Restore %s (*.res)|*%s*.res|All Restore (*.res)|*.res|All Files (*.*)|*.*||"),m_ProgramName,m_ProgramName);
			CFileDialog FileDlg(TRUE,(CString)".res",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,RestoreFilter,NULL);
			FileDlg.m_ofn.lpstrInitialDir=RestoreDir;
			if(FileDlg.DoModal()==IDOK) {
				FilePath=FileDlg.GetPathName();
				CUSLFile File;
				CFileException e;
				CString Buff, TypBuff;

				if(!File.Open(FilePath, CFile::typeBinary|CFile::modeRead, &e)) {
					File.DecodeFileException(&e,&Buff,FileDlg.GetFileName());
					TypBuff.LoadString(IDS_Warning);
					MessageBox(NULL,Buff, TypBuff, MB_ICONERROR);
				} else {

					ZeroMemory(str,sizeof str);
					File.Read(str,strlen(Identifier));
					if(strcmp(str,Identifier)) {
						File.Close();
						MessageBox(NULL,_T("Restore file is of an incompatible file type"),_T("Error"),MB_ICONERROR|MB_OK);
						m_pBootUpDlg->SendMessage(WM_CLOSE);
						delete m_pBootUpDlg;
						exit(m_nExitMode=1);
					}
					File.Read(&fVersion,sizeof fVersion);
					if(fVersion != m_fVersion) {
						File.Close();
						Buff.Format(L"Restore file is version %.02f\r\nThis is incompatible with this version of USLScanner version %.02f",fVersion, m_fVersion);
						MessageBox(NULL,Buff,_T("Error"),MB_ICONERROR|MB_OK);
						m_pBootUpDlg->SendMessage(WM_CLOSE);
						delete m_pBootUpDlg;
						exit(m_nExitMode=1);
					};

					File.Seek(0,CFile::begin);

					RetrieveLastSettings(&File);
					RetrieveTank(&File);
					m_Palette.Retrieve(&File);
					m_Scope.Retrieve(&File);
					m_Kinematics.RetrieveManipulator(&File);
					m_ToolChangers.Retrieve(&File);
					File.Close();
				}
			} else {
				bRestore=FALSE;
			}
		}//if
	}

	m_pBootUpDlg->UpdatePrompt(L"Retrieving Files");
	if(bRestore==FALSE) {
		CheckLastSettingsPathName();
		RetrieveLastSettings();
		RetrieveTank();
		m_Palette.Retrieve(m_LastSettingsPath + L"\\Last.pal");
		m_Scope.Retrieve(m_LastSettingsPath + L"\\Last.sco");
		m_UtUser.Retrieve(m_LastSettingsPath + L"\\Last.ul");
		m_Kinematics.RetrieveManipulator(m_LastSettingsPath + L"\\Last.man");
		m_PhasedArray[PORTSIDE].Retrieve(m_LastSettingsPath + L"\\Last_PhasedArray_Portside.pa");
		m_PhasedArray[STARBOARD].Retrieve(m_LastSettingsPath + L"\\Last_PhasedArray_Starboard.pa");
		m_ToolChangers.Retrieve(m_LastSettingsPath + L"\\ToolChangers.tools");
	}

	RetrieveMaintenanceVariables();	//Must be done after retrieving tank

	ChangeGlobalUnits();


	m_Profile.Zero();

	m_Ethercat.Initialize();
	m_FBCtrl.Initialize();

	if (m_Si10Userx64.Initialize()) {

	}
	else {

		if (m_Si10User.FindDevice()) {
			m_Si10User.StartDriver();
			m_UtUser.StartDriver();
			m_Si10User.CloseLaserDoor();
		}
		else if (m_LastSettings.bUSBI2CPresent) { //Would only have USBtoI2C if didn't have Si10
			m_USBtoI2C.StartDriver();
			m_UtUser.StartDriver();
		}
	}

	for(int ii=0;ii<4;ii++){
		m_LastSettings.bUseWhichPM30[ii]=false;
		if(m_LastSettings.bPr30Present[ii]){
			if(m_LastSettings.nAttachedToPr30[ii]!=LSA){
				m_LastSettings.bUseWhichPM30[ii]=true; //So we know which PM30s to talk to
				m_LastSettings.nUseWhichPR30 = ii;
			}
		}
	}
	if (theApp.m_PM40User.FindDevice() == STATUS_SUCCESSFUL) {

	}


	if(m_LSA.FindDevice()) {
		int nScanPitch = m_PhasedArray[0].getIntTxFocalLawPitch();
		int nNumberFocalLaws = m_PhasedArray[PORTSIDE].getNumberFocalLaws();
		m_LSA.ApplyFocalLaws(nNumberFocalLaws, m_PhasedArray[PORTSIDE].getTxAperture(), m_PhasedArray[PORTSIDE].getTxFirstElement(), false, 0, nScanPitch, m_PhasedArray[PORTSIDE].getElementPitch());
	}

	m_bUtHardware = true;
	if(m_bUtHardware==TRUE) {
		Buff.LoadString(IDS_Start_Ultrasonics); m_pBootUpDlg->UpdatePrompt(Buff);
		if (m_PM40User.IsConnected()) {
			m_UtUser.StartDriver(false, false);
		}
		else {
			if(m_Si10User.IsConnected() == false) {
				m_UtUser.StartDriver(true,true);
			}
			else {
				m_UtUser.StartDriver(true, false);
			}
		}
	}

	if(m_bSi6Hardware==TRUE) {
		Buff.LoadString(IDS_Start_Holding_Fixture); m_pBootUpDlg->UpdatePrompt(Buff);
		m_Si6User.StartDriver();
	}

	m_bServoHardware = TRUE;
	if(m_bServoHardware==TRUE) {
		Buff.LoadString(IDS_Start_Servo_Motors); m_pBootUpDlg->UpdatePrompt(Buff);
		m_PmacUser.StartDriver();
	}


	if(m_LastSettings.bAdcPresent)
		m_nNumADCs=1; //At least. Up to PCI ADCs to increase

	if(m_Tank.nCalibrateAdc == 1) {
		Buff.Format(L"Calibrating ADC. This will take some time, Please wait");
	} else {
		Buff.LoadString(IDS_Start_Digitizer);
	}
	m_pBootUpDlg->UpdatePrompt(Buff);

	if(	m_Alazar.FindDevices() > 0) {
		m_Alazar.Configure(&m_UtUser.m_TS[0].Adc);
		m_Alazar.StartThread();
	} else {
		if(m_Acqiris.FindDevices() > 0) {
			m_Acqiris.Configure();
			m_Acqiris.StartThread();
		} else {
			if(m_Spectrum.FindDevices() > 0) {
				m_Spectrum.Configure();
				m_Spectrum.StartThread();
			}
			else {
				if (m_PM40User.IsConnected()) {
					m_PM40User.ConfigureADC();
				}
			}
		}
	}

	
//	m_RemoteInterface.Start();

	Buff.LoadString(IDS_Start_Joystick); m_pBootUpDlg->UpdatePrompt(Buff);
	m_SerialJoystick.Start();

	if(m_LastSettings.nIOController==SAFETY_UNIT) {
		if(!m_SafetySerial.FindDevice()) {
			AfxMessageBox(_T("No Communication to Safety Unit"), MB_ICONEXCLAMATION);
		}
	}

	m_DataTranslationHW.Start();

	if (m_Tank.bConnectAOSAtStartUp[0]) {
		m_pBootUpDlg->UpdatePrompt(L"Starting AOS Phased Array");
		if (m_AOSPhasedArray.Connect(m_Tank.nAOSIPAddrs[0])) {
			m_AOSPhasedArray.Initialize();
		}
	}

	
	m_pBootUpDlg->SendMessage(WM_CLOSE);
	delete m_pBootUpDlg;

	for(int ii=0;ii<ParamsL;ii++){
		if(Params.GetAt(ii)=="logon") {
			m_cAccess[ID_SYSTEM_ACCESSLEVELS] = 0xff;
		}
	}

	if(m_nLicenseMask & _MINIMAL) {
		ZeroMemory(m_cAccess, sizeof m_cAccess);
	}


	m_nLogonLevel = m_nInitialLogonLevel;
	m_nLogonLevelMask = 1;
	m_nLogonLevelMask <<= (m_nLogonLevel - 1);

	m_nLogonLevelMask = OPERATOR;
	m_nLogonLevelMask = MANUFACTURER;

/////////////////////////////////////////////////////////////////////

	EnableTaskbarInteraction(FALSE);

	LoadStdProfileSettings(0);
	InitContextMenuManager();
	InitShellManager();
	InitKeyboardManager();
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
			RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);
	
	if(m_pRemote == NULL){
		m_pRemote = new CRemote();
	}

	/*
	if(m_pEgismosRangefinder == NULL){
		m_pEgismosRangefinder = new CEgismosRangefinder();
	}

	if(m_pFLSC10 == NULL){
		m_pFLSC10 = new CFLSC10Rangefinder();
	}
#ifdef NPTRACKINGTOOLS
	if (m_pV120Trio == NULL) {
		m_pV120Trio = new CV120Trio();
	}
	//if (m_pTracker == NULL) {
	//	m_pTracker = new CRigidBody();
//	}
#endif
*/
	/*
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
*/
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

//	LoadState();

	if(m_nLicenseMask & _MINIMAL ) {
		pFrame->LoadFrame(IDR_MAINFRAME_MINIMAL,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	} else {
		pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	}


	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();
	pFrame->OpenOnLastExit();

//	pFrame->SendMessage(UI_RESETTOOLBAR, (WPARAM)IDR_AIR_WATER_SIDE_0_TOOLBAR, NULL);
	if(pFrame->ForceLogon()==TRUE) {
		m_Thread.StartThread();

//		m_ServoUser.IsHomeRequired();
	}

	if(m_Tank.bStartRemoteServerOnStartup){
		m_pRemote->SetRestart(true); //Keep connection open if client disconnects.
		pFrame->PostMessage(REMOTE_START_SERVER);
	}
	GetAvailableMemory();

	ActivityLog(L"");
	ActivityLog(L"Start program");
	AndonEvent(MachineOkay, L"Succesully started");
	

return true;
/*
	CEvaluationDlg dlg;
	if(dlg.GetHowManyDaysLeft()<=0) {
		dlg.DoModal();
		if(dlg.GetHowManyDaysLeft()<=0) {
			pFrame->SendMessage(WM_CLOSE);
		}
	} else if(dlg.GetHowManyDaysLeft()<=30) {
		Buff.Format(_T("You only have %d days left of your license\nRenew your license using File >> Update License"),dlg.GetHowManyDaysLeft());
		MessageBox(NULL,Buff,_T("Warning"), NULL);
	}
*/
	return TRUE;
}
/*
BOOL CUSLScannerApp::LoadState(CFrameWndEx* pFrame,	LPCTSTR lpszSectionName)
{

	return FALSE;
}

BOOL CUSLScannerApp::LoadState(LPCTSTR lpszSectionName, CFrameImpl* pFrameImpl)
{
	CWinAppEx::LoadState(lpszSectionName, pFrameImpl);

//	m_pMainWnd->SendMessage(UI_RESETTOOLBAR, (WPARAM)IDR_AIR_WATER_SIDE_0_TOOLBAR, NULL);

	return FALSE;
}
*/
/////////////////////////////////////////////////////////////////////////////
// CUSLScannerApp message handlers





/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strVersion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CUSLScannerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.m_strVersion = m_strVersion;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CUSLScannerApp message handlers

#include <afxinet.h>
void CUSLScannerApp::GetWorkingDirectory()
{
	WCHAR str[256];

	int nLength = GetCurrentDirectory(0,NULL);
	GetCurrentDirectory(sizeof str,str);
	m_WorkingDirectory.Format((CString)"%s",str);

}

void CUSLScannerApp::GlobalDefaults(PropertiesList nWhichList)
{
	LOGFONT lf;

	m_Operator = "Any One";
	m_nLogonLevelMask = MANUFACTURER;

	ZeroMemory(&lf, sizeof lf);
	lf.lfHeight = -24;
	lf.lfWeight = 400;
	lf.lfQuality = 5;
	wcscpy_s(lf.lfFaceName, L"Calibri");

	if (nWhichList == All || nWhichList == Application) {
		theApp.m_LastSettings.bUseAlernativeToSystemFont = false;
		theApp.m_LastSettings.nTextPercentageSize = 3;

		theApp.m_LastSettings.lfMenus = lf;

		theApp.m_LastSettings.lfPositionPane = lf;
		theApp.m_LastSettings.rgbBKPositionPane = COLORREF(RGB(255, 255, 255));
		theApp.m_LastSettings.rgbTextPositionPane = COLORREF(RGB(0, 0, 0));

		m_Strings.SetAt(MachineSide0, L"Left");
		m_Strings.SetAt(MachineSide1, L"Right");
		m_Strings.SetAt(ComponentSide0, L"Tool");
		m_Strings.SetAt(ComponentSide1, L"Bag");
	}

	if (nWhichList == All || nWhichList == Palettes) {
		theApp.m_LastSettings.lfAmplitudePalette = lf;
		theApp.m_LastSettings.lfThicknessPalette = lf;
		theApp.m_LastSettings.lfPorosity = lf;
	}

	if (nWhichList == All || nWhichList == NetworkAttachedDevices) {
		theApp.m_Strings.SetAt(StringType::PPMAC_Address, L"192.168.0.200");
		theApp.m_Strings.SetAt(StringType::PPMAC_User, L"user");
		theApp.m_Strings.SetAt(StringType::PPMAC_Password, L"deltatau");
	}
}

void CUSLScannerApp::CheckLastSettingsPathName()
{
	int nn;
	CString Buff;

	if ((nn=m_LastSettingsPath.Find(L"\\Ver", 0)) > 0) {
		m_LastSettingsPath.GetBufferSetLength(nn);
	}
	Buff.Format(L"\\Ver%.02f", m_fVersion);
	m_LastSettingsPath += Buff;
	::CreateDirectory(m_LastSettingsPath, NULL);
}

bool CUSLScannerApp::SaveLastSettings()
{
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff ;
	CMainFrame* pFrame = (CMainFrame*)m_pMainWnd;

	CheckLastSettingsPathName();
	FilePath = m_LastSettingsPath + L"\\LastSettings.set";

	if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(NULL,Buff,_T("Warning"), MB_ICONERROR);
		return FALSE;
	} else {
		if(pFrame) pFrame->StatusBarPrompt(L"Last Settings Saved",500);
		SaveLastSettings(&File);
		File.Close();
		if(pFrame) pFrame->StatusBarPrompt(L"Ready",500);
		return TRUE;
	}
	return FALSE;

}

void CUSLScannerApp::SaveLastSettings(CUSLFile* pFile)
{
	CString Buff;
	int	nTS, nLastSettingsOffset, nTimeSlotOffset, nGlobalOffset, nRandomPositionOffset, nDescriptionsOffset, nAccessOffset, nAnnotateOffset, nPrCalOffset;
	int nHelpFileOffset, nToolPathRulesOffset, nThickCalOffset, nLadderOffset, nInclusionsOffset, nStringsOffset, nArmPositionOffset;
	int nOffsetArray[0x100];
	ZeroMemory(nOffsetArray,sizeof nOffsetArray);
	char Identifier[60]={"Ultrasonic Sciences Ltd USLScanner"};
	int nStartOffset = (int)(int)pFile->GetPosition();
	int nCount,nn;
	CArchive* pAr;

	m_LastSettings.fFastScanSize			= m_Profile.m_fFastScanSize;
	m_LastSettings.fSlowScanSize			= m_Profile.m_fSlowScanSize;
	m_LastSettings.fFastIncrement			= m_Profile.m_fFastIncrement;
	m_LastSettings.fSlowIncrement			= m_Profile.m_fSlowIncrement;
	m_LastSettings.nScanSide				= m_Profile.m_nScanSide;
	m_LastSettings.fScanSpeed				= m_Profile.m_fScanSpeed;
	m_LastSettings.m_nRectilinearScanMode	= m_Profile.m_nRectilinearScanMode;
	m_LastSettings.nFastAxis				= m_Profile.m_nFastAxis;
	m_LastSettings.nSlowAxis				= m_Profile.m_nSlowAxis;

	pFile->Write(Identifier,strlen(Identifier));
	pFile->Write(&m_fVersion,sizeof m_fVersion);

	pFile->Seek(nLastSettingsOffset=(int)(int)pFile->GetPosition()+0x200,CFile::begin);
	pFile->WriteStruct(&m_LastSettings, m_LastSettings.nStructSize=sizeof m_LastSettings);

	pFile->Seek(nGlobalOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	pFile->WriteStruct(&m_UtUser.m_Global, m_UtUser.m_Global.nStructSize=sizeof m_UtUser.m_Global);

	pFile->Seek(nTimeSlotOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	for(nTS=0;nTS<256;nTS++) {
		pFile->WriteStruct(&m_UtUser.m_TS[nTS], m_UtUser.m_TS[nTS].nStructSize=sizeof m_UtUser.m_TS[0]);
	}

	pFile->Seek(nRandomPositionOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	m_CpRandomPosition.Save(pFile);
	m_CpBall.Save(pFile);
	pFile->WriteStruct(&m_ChBall,m_ChBall.nStructSize = sizeof m_ChBall);

	pFile->Seek(nDescriptionsOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	if(m_fVersion<=4.38f) {
		pFile->Write(&m_nDescriptionL,sizeof m_nDescriptionL);
		for(nn=0;nn<m_nDescriptionL;nn++) {
			pFile->Seek(nDescriptionsOffset+nn*1024+4,CFile::begin);
			pFile->Write(&m_pDescription[nn],sizeof DescriptionStruct);

			pFile->Seek(nDescriptionsOffset+nn*1024+4+512,CFile::begin);
			Buff.Format(_T("%s"),*m_pDescription[nn].pCaption);
			pAr = new CArchive(pFile,CArchive::store);
			pAr->WriteString(Buff + _T("\r\n"));
			pAr->Close();
			SAFE_DELETE(pAr);
		}
	} else {
		m_Data.m_DataProperties.Save(pFile);
	}

	pFile->Seek(nAccessOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(m_cAccess,sizeof m_cAccess);

	pFile->Seek(nAnnotateOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	nCount=m_AnnotateReasons.GetSize();
	if(nCount<0) nCount=0;
	pFile->Write(&nCount,sizeof nCount);
	pAr = new CArchive(pFile,CArchive::store);
	for(nn=0;nn<nCount;nn++) {
		Buff=m_AnnotateReasons.GetAt(nn);
		pAr->WriteString(Buff + _T("\r\n"));
	}
	pAr->Close();
	SAFE_DELETE(pAr);

	pFile->Seek(nPrCalOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	for(nTS=0;nTS<256;nTS++) {
		pFile->WriteStruct(&m_UtUser.m_Cal[nTS], m_UtUser.m_Cal[nTS].nStructSize=sizeof m_UtUser.m_Cal[0]);
		if((m_UtUser.m_Cal[nTS].nArrayL>0) && (m_UtUser.m_pnDefaultCalTrace[nTS]!=NULL) && (m_UtUser.m_Cal[nTS].bDefaultTraceTrue==TRUE)) {
			pFile->Seek((int)pFile->GetPosition()+0x20,CFile::begin);
			pFile->Write(m_UtUser.m_pnDefaultCalTrace[nTS],m_UtUser.m_Cal[nTS].nArrayL * sizeof (int));
		}
	}


	pFile->Seek(nToolPathRulesOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	for(nn=0;nn<10;nn++) {
		nCount = m_strToolPath[nn].GetSize();
		pFile->Write(&nCount,sizeof nCount);
	}
	pAr = new CArchive(pFile,CArchive::store);
	for(nn=0;nn<10;nn++) {
		for(nCount=0;nCount<m_strToolPath[nn].GetSize();nCount++) {
			pAr->WriteString(m_strToolPath[nn].GetAt(nCount) + _T("\r\n"));
		}
	}
	pAr->Close();
	SAFE_DELETE(pAr);

	pFile->Seek(nThickCalOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	m_ThickCal.Save(pFile);

	pFile->Seek(nLadderOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&m_nLadderL, sizeof m_nLadderL);
	for(nn=0;nn<m_nLadderL;nn++) {
		m_pLadder[nn].Save(pFile);
	}

	nInclusionsOffset = (int)m_Inclusions.Save(pFile);

	pFile->Seek(nStringsOffset = (int)pFile->GetPosition() + 0x1000, CFile::begin);
	int nSize = m_Strings.GetSize();
	pFile->Write(&nSize, sizeof nSize);
	pAr = new CArchive(pFile, CArchive::store);
	for (int nIndex = 0; nIndex < nSize; nIndex++) {
		pAr->WriteString(m_Strings.GetAt(nIndex) + L"\r\n");
	}
	SAFE_DELETE( pAr );

	pFile->Seek(nArmPositionOffset = (int)pFile->GetPosition() + 0x200, CFile::begin);
	pFile->Write(m_nArmCount, sizeof m_nArmCount);
	pFile->Write(m_Arm, sizeof m_Arm);

	int nFinishOffset = (int)pFile->GetPosition();

	//Write the offsets
	pFile->Seek(0x100+nStartOffset,CFile::begin);
	pFile->Write(nOffsetArray,0x100);
	pFile->Seek(0x100+nStartOffset,CFile::begin);
	pFile->Write(&nLastSettingsOffset,sizeof nLastSettingsOffset);
	pFile->Write(&nGlobalOffset,sizeof nGlobalOffset);
	pFile->Write(&nTimeSlotOffset,sizeof nTimeSlotOffset);
	pFile->Write(&nRandomPositionOffset,sizeof nRandomPositionOffset);
	pFile->Write(&nDescriptionsOffset,sizeof nDescriptionsOffset);
	pFile->Write(&nAccessOffset,sizeof nAccessOffset);
	pFile->Write(&nAnnotateOffset,sizeof nAnnotateOffset);
	pFile->Write(&nPrCalOffset,sizeof nPrCalOffset);
	pFile->Write(&nHelpFileOffset,sizeof nHelpFileOffset);
	pFile->Write(&nToolPathRulesOffset,sizeof nToolPathRulesOffset);
	pFile->Write(&nThickCalOffset,sizeof nThickCalOffset);
	pFile->Write(&nLadderOffset,sizeof nLadderOffset);
	pFile->Write(&nInclusionsOffset, sizeof nInclusionsOffset);
	pFile->Write(&nStringsOffset, sizeof nStringsOffset);
	pFile->Write(&nArmPositionOffset, sizeof nArmPositionOffset);

	pFile->Seek(nFinishOffset,CFile::begin);
}



bool CUSLScannerApp::RetrieveLastSettings()
{
	CFileException e;
	CUSLFile File;
	CString FilePath,Buff;

	FilePath = m_LastSettingsPath + L"\\LastSettings.set";

	if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(NULL,Buff,_T("Error"),MB_ICONERROR);
		return FALSE;
	} else {
		RetrieveLastSettings(&File);
		File.Close();
		return TRUE;
	}
	return FALSE;
}

void CUSLScannerApp::RetrieveLastSettings(CUSLFile* pFile)
{
	int	nTS, nLastSettingsOffset, nTimeSlotOffset, nGlobalOffset, nRandomPositionOffset, nDescriptionsOffset, nAccessOffset, nAnnotateOffset, nPrCalOffset;
	int nHelpFileOffset, nToolPathRulesOffset, nThickCalOffset, nLadderOffset, nInclusionsOffset, nStringsOffset, nArmPositionOffset;
	float	fVersion;
	CString Buff;
	char Identifier[60]={"Ultrasonic Sciences Ltd USLScanner"};
	char str[60];
	int nStartOffset = (int)pFile->GetPosition();
	int nn,nCount,ii;
	int	nTPCount[10];

	memset(str,0,sizeof str);
	pFile->Read(str,strlen(Identifier));
	if(strcmp(str,Identifier)) {
		MessageBox(NULL,_T("LastSettings.set Incompatible file type"),_T("Error"),MB_ICONERROR);
	}
	pFile->Read(&fVersion,sizeof fVersion);
	pFile->m_fVersion = fVersion;

	if (fVersion > 4.0f && fVersion <= theApp.m_fVersion) {
		pFile->Seek(0x100 + nStartOffset, CFile::begin);
		pFile->Read(&nLastSettingsOffset, sizeof nLastSettingsOffset);
		pFile->Read(&nGlobalOffset, sizeof nGlobalOffset);
		pFile->Read(&nTimeSlotOffset, sizeof nTimeSlotOffset);
		pFile->Read(&nRandomPositionOffset, sizeof nRandomPositionOffset);
		pFile->Read(&nDescriptionsOffset, sizeof nDescriptionsOffset);
		pFile->Read(&nAccessOffset, sizeof nAccessOffset);
		pFile->Read(&nAnnotateOffset, sizeof nAnnotateOffset);
		pFile->Read(&nPrCalOffset, sizeof nPrCalOffset);
		pFile->Read(&nHelpFileOffset, sizeof nHelpFileOffset);
		pFile->Read(&nToolPathRulesOffset, sizeof nToolPathRulesOffset);
		pFile->Read(&nThickCalOffset, sizeof nThickCalOffset);
		pFile->Read(&nLadderOffset, sizeof nLadderOffset);
		pFile->Read(&nInclusionsOffset, sizeof nInclusionsOffset);
		pFile->Read(&nStringsOffset, sizeof nStringsOffset);
		pFile->Read(&nArmPositionOffset, sizeof nArmPositionOffset);


		if (nLastSettingsOffset) {
			pFile->Seek(nLastSettingsOffset, CFile::begin);
			pFile->ReadStruct(&m_LastSettings, sizeof m_LastSettings);
		}

		if (nGlobalOffset) {
			pFile->Seek(nGlobalOffset, CFile::begin);
			pFile->ReadStruct(&m_UtUser.m_Global, sizeof m_UtUser.m_Global);
		}
		if (nTimeSlotOffset) {
			pFile->Seek(nTimeSlotOffset, CFile::begin);
			for (nTS = 0; nTS < 256; nTS++) {
				pFile->ReadStruct(&m_UtUser.m_TS[nTS], sizeof m_UtUser.m_TS[0]);
			}
		}
		if (nRandomPositionOffset) {
			pFile->Seek(nRandomPositionOffset, CFile::begin);
			m_CpRandomPosition.Retrieve(pFile);
			m_CpBall.Retrieve(pFile);
			pFile->ReadStruct(&m_ChBall, sizeof m_ChBall);
		}
		if (nDescriptionsOffset) {
			if (fVersion <= 3.04) {

			}
			else {
				if (fVersion <= 3.48f) {
					if (m_pDescription) {
						for (int nn = 0; nn < m_nDescriptionL; nn++) {
							delete m_pDescription[nn].pCaption;
						}
						delete m_pDescription;
					}
					m_pDescription = NULL;
					m_nDescriptionL = 0;
					pFile->Seek(nDescriptionsOffset, CFile::begin);
					pFile->Read(&m_nDescriptionL, sizeof m_nDescriptionL);

					if (m_nDescriptionL) m_pDescription = (DescriptionStruct *) new DescriptionStruct[m_nDescriptionL];

					for (int nn = 0; nn < m_nDescriptionL; nn++) {
						pFile->Seek(nDescriptionsOffset + nn * 1024 + 4, CFile::begin);
						pFile->Read(&m_pDescription[nn], sizeof DescriptionStruct);

						pFile->Seek(nDescriptionsOffset + nn * 1024 + 4 + 512, CFile::begin);
						m_pDescription[nn].pCaption = new CString;
						CArchive ar(pFile, CArchive::load);
						ar.ReadString(Buff);
						ar.Close();
						m_pDescription[nn].pCaption->Format(_T("%s"), Buff);
					}
				}
				else {
					pFile->Seek(nDescriptionsOffset, CFile::begin);
					m_Data.m_DataProperties.Retrieve(pFile);
				}
			}

		}

		if (nAccessOffset) {
			pFile->Seek(nAccessOffset, CFile::begin);
			pFile->Read(m_cAccess, sizeof m_cAccess);
		}


		if (nAnnotateOffset) {
			pFile->Seek(nAnnotateOffset, CFile::begin);
			pFile->Read(&nCount, sizeof nCount);
			CArchive ar(pFile, CArchive::load);
			for (nn = 0; nn < nCount; nn++) {
				ar.ReadString(Buff);
				m_AnnotateReasons.Add(Buff);
			}
			ar.Close();
		}

		if (nPrCalOffset) {
			pFile->Seek(nPrCalOffset, CFile::begin);
			for (nTS = 0; nTS < 256; nTS++) {
				SAFE_DELETE(m_UtUser.m_pnCalTrace[nTS]);
				SAFE_DELETE(m_UtUser.m_pnDefaultCalTrace[nTS]);
				/*
				pFile->ReadStruct(&m_UtUser.m_Cal[nTS]);
				if((m_UtUser.m_Cal[nTS].nArrayL>0) && (m_UtUser.m_Cal[nTS].bDefaultTraceTrue==TRUE)) {
					pFile->Seek((int)pFile->GetPosition()+0x20,CFile::begin);
					m_UtUser.m_pnDefaultCalTrace[nTS] = new int[m_UtUser.m_Cal[nTS].nArrayL];
					pFile->Read(m_UtUser.m_pnDefaultCalTrace[nTS],m_UtUser.m_Cal[nTS].nArrayL * sizeof (int));
				}
				*/
			}
		}


		if (nToolPathRulesOffset) {
			pFile->Seek(nToolPathRulesOffset, CFile::begin);
			for (nn = 0; nn < 10; nn++) {
				pFile->Read(&nTPCount[nn], sizeof(int));
			}
			CArchive ar(pFile, CArchive::load);
			for (nn = 0; nn < 10; nn++) {
				m_strToolPath[nn].RemoveAll();
				for (nCount = 0; nCount < nTPCount[nn]; nCount++) {
					ar.ReadString(Buff);
					m_strToolPath[nn].Add(Buff);
				}
			}
			ar.Close();
		}

		if (nThickCalOffset && pFile->m_fVersion >= 5.0f) {
			//		pFile->Seek(nThickCalOffset,CFile::begin);
			//		m_ThickCal.Retrieve(pFile);
			//		ComputeVelocityTable();
		}

		if (nLadderOffset > 0 && nLadderOffset < pFile->GetLength() && pFile->m_fVersion >= 5.00f) {
			SAFE_DELETE_ARRAY(m_pLadder);
			m_nLadderL = 0;
			pFile->Seek(nLadderOffset, CFile::begin);
			pFile->Read(&m_nLadderL, sizeof m_nLadderL);
			if (m_nLadderL > 0) {
				m_pLadder = new CPolyCoord[m_nLadderL];
				for (nn = 0; nn < m_nLadderL; nn++) {
					m_pLadder[nn].Retrieve(pFile);
				}
			}
		}

		if (nInclusionsOffset > 0) {
			pFile->Seek(nInclusionsOffset, CFile::begin);
			m_Inclusions.Retrieve(pFile);
		};
	}

	if (nStringsOffset > 0 && nStringsOffset < pFile->GetLength()) {
		pFile->Seek(nStringsOffset, CFile::begin);
		int nSize;
		CString Buff;
		pFile->Read(&nSize, sizeof nSize);
		if (nSize > 0 && nSize < 2000) {
			m_Strings.RemoveAll();
			CArchive* pAr = new CArchive(pFile, CArchive::load);
			for (int nIndex = 0; nIndex < nSize; nIndex++) {
				pAr->ReadString(Buff);
				m_Strings.SetAtGrow(nIndex, Buff);
			}
			delete pAr;
		}
	}

	if (nArmPositionOffset > 0 && nArmPositionOffset < pFile->GetLength()) {
		pFile->Seek(nArmPositionOffset, CFile::begin);
		pFile->Read(m_nArmCount, sizeof m_nArmCount);
		pFile->Read(m_Arm, sizeof m_Arm);
	}

	//The next section of code is to initialise new variables, If the software is upgraded and there are new variables in the 
	//Last settings structure we should initalise them to a default value

	if( (m_LastSettings.nDacMaxThreshold==0)||(m_LastSettings.nDacMaxThreshold<0)||(m_LastSettings.nDacMaxThreshold>200) )
		m_LastSettings.nDacMaxThreshold=168;

	if( (m_LastSettings.nIOController!=SERVO_TURBO)&&(m_LastSettings.nIOController!=SERVO)&&(m_LastSettings.nIOController!=SI10)&&(m_LastSettings.nIOController!=SAFETY_UNIT)&&(m_LastSettings.nIOController!=FIELD_BUS)  )
		m_LastSettings.nIOController = SERVO_TURBO;
	
	if( (m_LastSettings.ADCInputDevice[0]>5) || (m_LastSettings.ADCInputDevice[0]<=0) ) {
		m_LastSettings.ADCInputDevice[0]=PM30_MUX8;
		m_LastSettings.ADCInputDevice[1]=UNUSED;
		m_LastSettings.ADCInputDevice[2]=UNUSED;
		m_LastSettings.ADCInputDevice[3]=UNUSED;
		m_LastSettings.ADCInputDevice[4]=UNUSED;
		m_LastSettings.ADCInputDevice[5]=UNUSED;
		m_LastSettings.ADCInputDevice[6]=UNUSED;
		m_LastSettings.ADCInputDevice[7]=UNUSED;
	}

	if((m_LastSettings.nUseWhichPR30<0) || (m_LastSettings.nUseWhichPR30>7)) 
		m_LastSettings.nUseWhichPR30 = 0;
	
	for(ii=0;ii<8;ii++) {
		if( (m_LastSettings.nAttachedToPr30[ii]>2) || (m_LastSettings.nAttachedToPr30[ii]<0) )
			m_LastSettings.nAttachedToPr30[ii] = MUX8;
	}
	

}

bool CUSLScannerApp::SaveToolChanger()
{
	CMainFrame* pFrame = (CMainFrame*)m_pMainWnd;

	if(pFrame) pFrame->StatusBarPrompt(L"Toolchanger Saved",200);
	CheckLastSettingsPathName();
	m_ToolChangers.Save(m_LastSettingsPath + L"\\ToolChangers.tools");
	if(pFrame) pFrame->StatusBarPrompt(L"Ready",0);

	return TRUE;
}

bool CUSLScannerApp::SaveTank()
{
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff ;
	CMainFrame* pFrame = (CMainFrame*)m_pMainWnd;

	CheckLastSettingsPathName();
	FilePath = m_LastSettingsPath + L"\\Tank.set";

	if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(NULL,Buff,_T("Warning"), MB_ICONERROR);
		return FALSE;
	} else {
		if(pFrame) pFrame->StatusBarPrompt(L"Tank Saved",200);
		SaveTank(&File);
		File.Close();
		if(pFrame) pFrame->StatusBarPrompt(L"Ready",0);
		return TRUE;
	}
	return FALSE;

}

void CUSLScannerApp::SaveTank(CUSLFile* pFile)
{
	int	ii,nAxesOffset,nSi6Offset,nTankOffset,nSafetyOffset,nFBCtrlOffset,nRobotOffset[2];
	int nOffsetArray[0x100];
	ZeroMemory(nOffsetArray,sizeof nOffsetArray);
	char Identifier[60]={"Ultrasonic Sciences Ltd USLScanner Tank"};
	int nStartOffset = (int)pFile->GetPosition();

	//Write the identifier
	pFile->Write(Identifier,strlen(Identifier));
	pFile->Write(&m_fVersion,sizeof m_fVersion);

	pFile->Seek(nAxesOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	for(ii=0;ii<32;ii++) {
		pFile->WriteStruct(&m_Axes[ii], m_Axes[ii].nStructSize=sizeof m_Axes[0]);
		pFile->Seek(0x100,CFile::current);
	}

	nSi6Offset = m_Si6User.Save(pFile,0x200);

	pFile->Seek(nTankOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	pFile->WriteStruct(&m_Tank,m_Tank.nStructSize = sizeof m_Tank);

	pFile->Seek(nSafetyOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	CArchive ar(pFile,CArchive::store);
	for(ii=0;ii<16;ii++) {
		ar.WriteString(m_SafetyBitDescription[ii]);
		ar.WriteString(_T("\r\n"));
	}
	ar.Close();
	pFile->Write(m_nSafetyLogic,sizeof m_nSafetyLogic);

	pFile->Seek(0x200,CFile::current);
	nFBCtrlOffset = m_FBCtrl.Save(pFile);

	for(ii=0;ii<2;ii++) {
		pFile->Seek(0x200,CFile::current);
		nRobotOffset[ii]=m_Robot[ii].Save(pFile);
	};

	int nFinishOffset = (int)pFile->GetPosition();

	//Write the offsets
	pFile->Seek(0x100+nStartOffset,CFile::begin);
	pFile->Write(nOffsetArray,0x100);
	pFile->Seek(0x100+nStartOffset,CFile::begin);
	pFile->Write(&nAxesOffset,sizeof nAxesOffset);
	pFile->Write(&nSi6Offset,sizeof nSi6Offset);
	pFile->Write(&nTankOffset,sizeof nTankOffset);
	pFile->Write(&nSafetyOffset,sizeof nSafetyOffset);
	pFile->Write(&nFBCtrlOffset,sizeof nFBCtrlOffset);
	pFile->Write(nRobotOffset,sizeof nRobotOffset);

	pFile->Seek(nFinishOffset,CFile::begin);
}

bool CUSLScannerApp::RetrieveTank()
{
	CFileException e;
	CUSLFile File;
	CString FilePath,Buff;

	FilePath = m_LastSettingsPath + L"\\Tank.set";

	if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(NULL,Buff,_T("Error"),MB_ICONERROR);
		return FALSE;
	} else {
		RetrieveTank(&File);
		File.Close();
		return TRUE;
	}


	return FALSE;


}

void CUSLScannerApp::RetrieveTank(CUSLFile* pFile)
{
	int	ii,nAxesOffset,nSi6Offset,nTankOffset,nSafetyOffset,nFBCtrlOffset,nRobotOffset[2];
	float	fVersion;

	char Identifier[60]={"Ultrasonic Sciences Ltd USLScanner Tank"};
	char str[60];
	int nStartOffset = (int)pFile->GetPosition();

	memset(str,0,sizeof str);
	pFile->Read(str,strlen(Identifier));
	if(strcmp(str,Identifier)) {
		MessageBox(NULL,_T("Incompatible Tank file type"),_T("Error"),MB_ICONERROR);
	}
	pFile->Read(&fVersion,sizeof fVersion);
	if (fVersion > 4.0f && fVersion <= theApp.m_fVersion) {

		pFile->Seek(0x100 + nStartOffset, CFile::begin);
		pFile->Read(&nAxesOffset, sizeof nAxesOffset);
		pFile->Read(&nSi6Offset, sizeof nSi6Offset);
		pFile->Read(&nTankOffset, sizeof nTankOffset);
		pFile->Read(&nSafetyOffset, sizeof nSafetyOffset);
		pFile->Read(&nFBCtrlOffset, sizeof nFBCtrlOffset);
		pFile->Read(nRobotOffset, sizeof nRobotOffset);

		if (nAxesOffset) {
			pFile->Seek(nAxesOffset, CFile::begin);
			for (ii = 0; ii < 32; ii++) {
				pFile->ReadStruct(&m_Axes[ii], sizeof m_Axes[0]);
				pFile->Seek(0x100, CFile::current);
				if (fVersion < 4.55f) {
					m_Axes[ii].dStepsPerRev = (double)m_Axes[ii].nStepsPerRev;
				}
			}
		}

		if (nSi6Offset) {
			pFile->Seek(nSi6Offset, CFile::begin);
			m_Si6User.Retrieve(pFile);
		}

		if (nTankOffset) {
			pFile->Seek(nTankOffset, CFile::begin);
			pFile->ReadStruct(&m_Tank, sizeof m_Tank);
		}
		if (nSafetyOffset) {
			pFile->Seek(nSafetyOffset, CFile::begin);
			CArchive ar(pFile, CArchive::load);
			for (ii = 0; ii < 16; ii++) {
				ar.ReadString(m_SafetyBitDescription[ii]);
			}
			ar.Close();
			pFile->Read(m_nSafetyLogic, sizeof m_nSafetyLogic);
		}
		else {
			for (ii = 0; ii < 16; ii++) {
				m_SafetyBitDescription[ii].Format(_T("%d"), ii);
			}
			ZeroMemory(m_nSafetyLogic, sizeof m_nSafetyLogic);
		}

		if (fVersion >= 4.30f) {
			if (nFBCtrlOffset) {
				pFile->Seek(nFBCtrlOffset, CFile::begin);
				m_FBCtrl.Retrieve(pFile);
			}
		}

		if (fVersion >= 4.31f) {
			for (ii = 0; ii < 2; ii++) {
				if (nRobotOffset[ii]) {
					pFile->Seek(nRobotOffset[ii], CFile::begin);
					m_Robot[ii].Retrieve(pFile, ii);
				};
			}
		}
	}

	m_Si10User.m_Si10.nSwitchEor = m_Tank.nSi10SwitchEor;

}

bool CUSLScannerApp::CheckScreenSettings()
{
	CDC Dc;
	Dc.CreateCompatibleDC(NULL);
	int nBitsPixel = Dc.GetDeviceCaps(BITSPIXEL);
//	m_LastSettings.nScreenHorizontalSizeMM = Dc.GetDeviceCaps(HORZSIZE);	//This should work but often doesn't so I've commented it out
	m_LastSettings.nScreenWidthPixels = Dc.GetDeviceCaps(HORZRES);

	if ((nBitsPixel!=32)&&(nBitsPixel!=24))
	{
		AfxMessageBox(_T("You need to change the setting of your screen before using this Software"), MB_OK|MB_ICONEXCLAMATION);
		ExitInstance( );
		PostQuitMessage(0);
		return FALSE;
	}

	return TRUE;
}

void CUSLScannerApp::SetLanguage()
{
	HINSTANCE hInst;

	switch(m_nLanguage) {
	default:
		hInst = LoadLibrary((CString)"USLScanner.exe");
		break;
	case 1:
		hInst  = LoadLibrary((CString)"Russianresources.dll");
		break;
	case 2:
		hInst  = LoadLibrary((CString)"ChineseResources.dll");
		break;
	case 3:
		hInst  = LoadLibrary((CString)"GermanResources.dll");
		break;
	case 4:
		hInst  = LoadLibrary((CString)"SpanishResources.dll");
		break;
	case 5:
		hInst  = LoadLibrary((CString)"JapaneseResources.dll");
		break;
	case 6:
		hInst  = LoadLibrary((CString)"TaiwaneseResources.dll");
		break;
	}
	if (hInst  != NULL)	{
		AfxSetResourceHandle(hInst);
	} else {
		AfxMessageBox((CString)"Unable to load Language setting. Defaulting to English");
		m_nLanguage=0;
	}

}

void CUSLScannerApp::SetFontLanguage(LOGFONT *plf)
{
	switch (m_nLanguage) {
	default:
		_tcsncpy_s(plf->lfFaceName, LF_FACESIZE, _T("Times"), 7);
		break;
	case 2:
	case 6:
		_tcsncpy_s(plf->lfFaceName, LF_FACESIZE, _T("SimSun"), 7);
		break;
	}
}

void CUSLScannerApp::ChangeGlobalUnits()
{
	switch (m_LastSettings.nMeasurementUnits) {
	default: m_Units = "mm";
		m_fUnits = 1.0f;
		m_nUnitsPrecision = 1;
		m_nWpPrecision = 1;
		break;
	case 1:	m_Units = "\x022";
		m_fUnits = 1.0f / 25.4f;
		m_nUnitsPrecision = 3;
		m_nWpPrecision = 2;
		break;
	}

}

/*
typedef struct _MEMORYSTATUSEX {
  DWORD     dwLength;
  DWORD     dwMemoryLoad;
  DWORDLONG ullTotalPhys;
  DWORDLONG ullAvailPhys;
  DWORDLONG ullTotalPageFile;
  DWORDLONG ullAvailPageFile;
  DWORDLONG ullTotalVirtual;
  DWORDLONG ullAvailVirtual;
  DWORDLONG ullAvailExtendedVirtual;
}MEMORYSTATUSEX, *LPMEMORYSTATUSEX;

WINBASEAPI
VOID
WINAPI
GlobalMemoryStatusEx(
    MEMORYSTATUSEX lpBuffer
    );
*/

void CUSLScannerApp::GetAvailableMemory()
{
	CString	Buff;

	MEMORYSTATUS mem;
	mem.dwLength = sizeof MEMORYSTATUS;
	GlobalMemoryStatus(&mem);

	Buff.Format(_T("Total = %ldMByte Available=%ldMByte"),mem.dwTotalPhys / 1048576,mem.dwAvailVirtual / 1048576);

/*
	MEMORYSTATUSEX memvlm;
	memvlm.dwLength = sizeof MEMORYSTATUSEX;
	GlobalMemoryStatusEx(memvlm);
*/
}


void CUSLScannerApp::FormatLength(float fValue, int nPresicion, CString &Buff)
{
	switch(m_LastSettings.nMeasurementUnits) {
	default: Buff.Format(_T("%.*f %s"),nPresicion,fValue,m_Units);
		break;
	case 1: fValue /= 25.4f;
		switch(nPresicion) {
		case 0:  nPresicion=2;
			break;
		case 1: nPresicion=3;
			break;
		case 2: nPresicion=4;
			break;
		case 3: nPresicion=5;
			break;
		}
		Buff.Format(_T("%.*f%s"),nPresicion,fValue,m_Units);
		break;
	}
}




BOOL CUSLScannerApp::PreTranslateMessage(MSG* pMsg)
{
	int m_nTS = m_UtUser.m_Global.nTimeSlot;

	switch(pMsg->message) {
	case WM_KEYUP:
		m_bCtrlButton = false;
		m_bShiftButton = false;
		break;
	case WM_KEYDOWN:
		switch(pMsg->wParam) {
		case 16: m_bShiftButton=true;
			break;
		case 17: m_bCtrlButton=true;//Ctrl button down
			break;
		case 't':
		case 'T':
			if((m_bCtrlButton==true) && (m_bShiftButton==true)){
				for(m_nTS=0;m_nTS<m_UtUser.m_Global.Mux.nMaxTimeslots;m_nTS++) {
					if(m_UtUser.m_TS[m_nTS].Pr30.nOutputSelect == 2) break;
				}
				m_nTS = m_UtUser.m_Global.nTimeSlot;
				if(m_bTracking != TRUE) {
					m_nTrackingTS = m_nTS;
					m_nTrackingAmp[m_nTS] =		m_UtUser.m_TS[m_nTS].Gate.nAmplitude[0];
					m_fTrackingLogGain =	m_UtUser.m_TS[m_nTS].Pr30.fLogGain;
					m_fTrackingLinGain =	m_UtUser.m_TS[m_nTS].Pr30.fLinGain;
					m_bTracking=TRUE;
				} else {
					m_bTracking=FALSE;
					m_UtUser.m_TS[m_nTS].Pr30.fLogGain = m_fTrackingLogGain;
					m_UtUser.Pr30LogFloatGain(&m_UtUser.m_TS[m_nTS].Pr30);
				}
				return true;
			};
			break;
		case 'z':
		case 'Z':
			if((m_bCtrlButton==true) && (m_bShiftButton==true)){
				for(int nAxis=0;nAxis<8;nAxis++) {
					m_Motors.HoldingFixtureZeroPosition(nAxis);
				}
				return true;
			}
			break;
		case 0x52:
			m_PmacUser.m_bDriveAwayRequired[0] = false;
			m_PmacUser.m_bDriveAwayRequired[1] = false;
			break;
		case 27:
			m_bCollectComplexData = false;
			m_bCollectBladeData = false;
			m_Thread.m_nThreadAction = 0;
			return TRUE;
			break;
		};
		break;
	case WM_MOUSEMOVE:
		m_bResetTimeOutTimer = TRUE;
		break;
	};

	return CWinApp::PreTranslateMessage(pMsg);
}


void CUSLScannerApp::ActivityLog(CString Activity)
{
	CString FileName = m_LastSettingsPath + "\\ActivityLog.txt";
	CUSLFile File;
	CFileException e;
	CTime CurrentTime = CTime::GetCurrentTime();
	CString Date = CurrentTime.Format("%d %b %Y  %Hh%M : ");


	if(!File.Open(FileName, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite|CFile::modeNoTruncate, &e)) {
//		File.DecodeFileException(&e,&Buff,File.GetFilePath());
//		TypBuff.LoadString(IDS_Warning);
//		MessageBox(NULL,Buff, TypBuff, MB_ICONERROR);
	} else {
		File.SeekToEnd();

		CArchive* pAr = new CArchive(&File,CArchive::store);
		pAr->WriteString(Date + Activity + _T("\r\n"));
		pAr->Close();
		delete pAr;

		File.Close();
	}


}


void CUSLScannerApp::AndonEvent(AndonEventType nAction, CString Message)
{
	CUSLFile File;
	CString Buff;
	int nMask;

	if(m_nOldAndonAction != nAction) {
		switch(nAction) {
		case MachineOkay: nMask = m_Tank.nAndonOkay;
			Buff = "Machine Okay: ";
			break;
		case CycleRunning: nMask = m_Tank.nAndonCycleRunning;
			Buff = "Cycle Running: ";
			break;
		case MachineWarning: nMask = m_Tank.nAndonWarning;
			Buff = "Machine Warning: ";
			break;
		case MachineFault: nMask = m_Tank.nAndonFault;
			Buff = "Machine Fault: ";
			break;
		}

		switch(m_Tank.nAndonOutputPort) {
		case 0: m_PmacUser.EnableLights(nMask);
			break;
		case 1: m_Si10User.EnableLights(nMask);
			break;
		case 2:
			break;
		}

		m_FBCtrl.AndonEvent(nAction);

		if(File.Open(m_LastSettingsPath + "\\AndonEvent.txt", CFile::modeCreate|CFile::typeBinary|CFile::modeReadWrite|CFile::modeNoTruncate, NULL)) {
			File.SeekToEnd();

			CArchive* pAr = new CArchive(&File,CArchive::store);
			pAr->WriteString(Buff);
			if(Message.GetLength() > 0) {
				pAr->WriteString(Message);
			};
			pAr->WriteString(L"\r\n");
			pAr->Close();
			delete pAr;

			File.Close();
		}

	}
	m_nOldAndonAction = nAction;
}

int CUSLScannerApp::SplitString(CString InitString, CStringArray *OuputStrings, int nMaxStrings, bool ToLowerCase)
{

		int nOuputStringsL;
		CString newtok;
		CToken tok(InitString);
		tok.SetToken(L" ");

		
		nOuputStringsL=0;
		while((tok.MoreTokens())&&(nOuputStringsL<nMaxStrings))
		{
			nOuputStringsL++;
			newtok = tok.GetNextToken();
			if(ToLowerCase)
				newtok.MakeLower();
			OuputStrings->Add(newtok);
		}

		if((nOuputStringsL==0)&&(InitString.GetLength()>0)){
			//we only have 1 Parameter and so no token
			nOuputStringsL=1;
			if(ToLowerCase)
				InitString.MakeLower();
			OuputStrings->Add(InitString);
		}


		return nOuputStringsL;

}

void CUSLScannerApp::CStringToChar(CString str, char* chr)
{
	int i=0;

	for(i=0;i<str.GetLength();i++)
	{
		chr[i]= (char)str.GetAt(i);
	}

	chr[i] = '\0';
}

void CUSLScannerApp::CharToCString(char* chr, CString &str)
{
	size_t i=0;
	str.Empty();
	for(i=0;i<strlen(chr);i++)
	{
		str += chr[i];
	}
}

void CUSLScannerApp::ComputeVelocityTable()
{
	int nPtr;
	int nPtr0=0;
	int nPtr1;
	float fTemp,fThick0,fThick1;

	fThick0 = 0.0;

	for(int ii=0;ii<m_ThickCal.m_nCoordL;ii++) {
		nPtr1 = m_ThickCal.m_pCp[ii].nTimens;
		fThick1 = m_ThickCal.m_pCp[ii].fNomThickness * 2.0f;		//Yes * 2.0. I've thought long and hard this is correct


		for(nPtr=nPtr0;(nPtr<nPtr1) && (nPtr < 300000);nPtr++) {
			fTemp = (float)(nPtr - nPtr0) * (fThick1 - fThick0);
			fTemp /= (float)((nPtr1-1) - nPtr0);
			fTemp += fThick0;
			m_fPath[nPtr] = fTemp;
		}

		if(ii<(m_ThickCal.m_nCoordL - 1) ) {
			fThick0 = fThick1;
			nPtr0 = nPtr1;
		} else {
			for(nPtr=nPtr0;nPtr < 300000;nPtr++) {
				fTemp = (float)(nPtr - nPtr0) * (fThick1 - fThick0);
				fTemp /= (float)((nPtr1-1) - nPtr0);
				fTemp += fThick0;
				m_fPath[nPtr] = fTemp;
			}
		}
	}
}


float CUSLScannerApp::TimeToThickness(int nTimens,float *pfThickness,int nUnits, CString *pBuff)
{

	float fPath = 0.0f;

	if(m_LastSettings.nMaterialVelocityMode==0) {
		fPath = (float)((nTimens + m_LastSettings.nMaterialConstant) * m_LastSettings.nMaterialVelocity) / 1.0e6f;
	} else {
		if((nTimens>=0) && (nTimens<300000)) {
			fPath = m_fPath[nTimens];
		}
	}

	if(pfThickness != NULL) *pfThickness = fPath;
	if (pBuff != NULL) {
		switch (nUnits) {
		case 0:	pBuff->Format(_T("%.0f ns"), (float)nTimens);
			break;
		case 1:	pBuff->Format(_T("%.03f \x0b5s"), (float)nTimens / 1000);
			break;
		case 2:	pBuff->Format(_T("%.01f mm"), (float)(nTimens*theApp.m_LastSettings.nWaterVelocity) / 1e6);
			break;
		case 3:	pBuff->Format(_T("%.01f mm"), (float)(nTimens*theApp.m_LastSettings.nMaterialVelocity) / 1e6);
			break;
		case 4:	pBuff->Format(_T("%.03f\x022"), (float)(nTimens*theApp.m_LastSettings.nWaterVelocity) / 254e5);
			break;
		case 5:	pBuff->Format(_T("%.03f\x022"), (float)(nTimens*theApp.m_LastSettings.nMaterialVelocity) / 254e5);
			break;
		}
	}
	return fPath;
}

void CUSLScannerApp::RetrieveRegistryVariables()
{
	CString Buff;

#ifndef NO_REGISTRY

	m_LastSettingsPath =				GetProfileString(L"GeneralVariables",L"LastSettings",m_LastSettingsPath.GetBuffer(1));
	m_ProbesPath =						GetProfileString(L"GeneralVariables",L"ProbesPath",m_ProbesPath.GetBuffer(1));
	m_ArchivePath =						GetProfileString(L"GeneralVariables",L"ArchivePath",m_ArchivePath.GetBuffer(1));
	m_ProfilePath =						GetProfileString(L"GeneralVariables",L"ProfilePath",m_ProfilePath.GetBuffer(1));
	m_AptSourcePath =					GetProfileString(L"GeneralVariables",L"AptSourcePath",m_AptSourcePath.GetBuffer(1));
	m_UltrasonicsPath =					GetProfileString(L"GeneralVariables",L"UltrasonicsPath",m_UltrasonicsPath.GetBuffer(1));
	m_PalettePath =						GetProfileString(L"GeneralVariables",L"PalettePath",m_PalettePath.GetBuffer(1));
	m_ScanPath =						GetProfileString(L"GeneralVariables",L"ScanPath",m_ScanPath.GetBuffer(1));
	m_DataBasePathName =				GetProfileString(L"GeneralVariables",L"DataBasePath",m_DataBasePathName.GetBuffer(1));
	m_DataBaseFailedToSavePathName =	GetProfileString(L"GeneralVariables",L"DataBaseFailPath",m_DataBaseFailedToSavePathName.GetBuffer(1));
	m_ViewReportTemplateName =			GetProfileString(L"GeneralVariables",L"ViewTemplateName",m_ViewReportTemplateName.GetBuffer(1));
	m_WallImagePathName =				GetProfileString(L"GeneralVariables",L"WallImagePath",m_WallImagePathName.GetBuffer(1));
	m_HelpPathName =					GetProfileString(L"GeneralVariables",L"HelpPath",m_HelpPathName.GetBuffer(1));
	m_DetailsWindowName =				GetProfileString(L"GeneralVariables",L"DetailsWindowName",m_DetailsWindowName.GetBuffer(1));
	m_AcrobatPathName =					GetProfileString(L"GeneralVariables",L"AcrobatPath",m_AcrobatPathName);
	m_WordPadPathName =					GetProfileString(L"GeneralVariables",L"WordPadPath",m_WordPadPathName);
	m_USLScannerManualPathName =		GetProfileString(L"GeneralVariables",L"USLScannerManualPath",m_USLScannerManualPathName);
	m_PMACSRMPathName =					GetProfileString(L"GeneralVariables",L"PMACSRMPath",m_PMACSRMPathName);
	m_AndonPathName =					GetProfileString(L"GeneralVariables",L"AndonPathName",m_AndonPathName.GetBuffer(1));
	m_PmacProgramPath =					GetProfileString(L"GeneralVariables",L"PmacProgramPath",m_PmacProgramPath.GetBuffer(1));
	m_LimitAndKinematicOffset =			GetProfileString(L"GeneralVariables",L"LimitAndKinematicName",m_LimitAndKinematicOffset.GetBuffer(1));
	m_ThicknessCalibrationPathName =	GetProfileString(L"GeneralVariables",L"ThicknessCalibrationPathName",m_ThicknessCalibrationPathName.GetBuffer(1));
	m_TrackerPathName =					GetProfileString(L"GeneralVariables",L"TrackerPath",m_TrackerPathName.GetBuffer(1));
	m_WorkspacePathName =				GetProfileString(L"GeneralVariables",L"WorkspacePath",m_WorkspacePathName.GetBuffer(1));
	m_DlgSideName[0] =					GetProfileString(L"GeneralVariables",L"LeftSideOfDialogueName",m_DlgSideName[0].GetBuffer(1));
	m_DlgSideName[1] =					GetProfileString(L"GeneralVariables",L"RightSideOfDialogueName",m_DlgSideName[1].GetBuffer(1));
	m_EndEffectorPathName[0] =			GetProfileString(L"GeneralVariables",L"Robot_1_EndEffector",m_EndEffectorPathName[0].GetBuffer(1));
	m_EndEffectorPathName[1] =			GetProfileString(L"GeneralVariables",L"Robot_2_EndEffector",m_EndEffectorPathName[1].GetBuffer(1));
	m_ProbeSerialNumber[0] =			GetProfileString(L"GeneralVariables",L"Probe_1_Serial_Number",m_ProbeSerialNumber[0].GetBuffer(1));
	m_ProbeSerialNumber[1] =			GetProfileString(L"GeneralVariables",L"Probe_2_Serial_Number",m_ProbeSerialNumber[1].GetBuffer(1));
	m_CalibrationComponent[0] =			GetProfileString(L"GeneralVariables",L"Calibration_component_1",m_CalibrationComponent[0].GetBuffer(1));
	m_CalibrationComponent[1] =			GetProfileString(L"GeneralVariables",L"Calibration_component_2",m_CalibrationComponent[1].GetBuffer(1));
	m_CalibrationPathName =				GetProfileString(L"GeneralVariables",L"CalibrationPath",m_CalibrationPathName.GetBuffer(1));
	m_3PtName[0] =						GetProfileString(L"GeneralVariables",L"3PtName_1",m_3PtName[0].GetBuffer(1));
	m_3PtName[1] =						GetProfileString(L"GeneralVariables",L"3PtName_2",m_3PtName[1].GetBuffer(1));
	m_3PtName[2] =						GetProfileString(L"GeneralVariables",L"3PtName_3",m_3PtName[2].GetBuffer(1));

	for(int ii=0;ii<10;ii++) {
		Buff.Format(_T("StorePath %d"),ii);
		m_StorePath[ii] = GetProfileString(L"GeneralVariables",Buff,m_StorePath[ii].GetBuffer(1));
	}


	unsigned int nSize;
	unsigned char *pUtData = NULL;
	unsigned char *pServoData = NULL;
	unsigned char *pSi6Data = NULL;
	unsigned char *pUtEthernetData = NULL;
	GetProfileBinary(L"GeneralVariables",_T("UltrasonicHardware"),(LPBYTE*)&pUtData,&nSize);
	GetProfileBinary(L"GeneralVariables",_T("ServoHardware"),(LPBYTE*)&pServoData,&nSize);
	GetProfileBinary(L"GeneralVariables",_T("Si6Hardware"),(LPBYTE*)&pSi6Data,&nSize);
	GetProfileBinary(L"GeneralVariables",_T("UtEthernet"),(LPBYTE*)&pUtEthernetData,&nSize);
	CopyMemory(&m_bUtHardware,pUtData,nSize);
	CopyMemory(&m_bServoHardware,pServoData,nSize);
	CopyMemory(&m_bSi6Hardware,pSi6Data,nSize);
	CopyMemory(&m_bUTEthernet,pUtEthernetData,nSize);
	SAFE_DELETE_ARRAY( pUtData );
	SAFE_DELETE_ARRAY( pServoData );
	SAFE_DELETE_ARRAY( pSi6Data );
	SAFE_DELETE_ARRAY( pUtEthernetData );

	m_nJobNumber = GetProfileInt(L"GeneralVariables",L"Customer",m_nJobNumber);
	m_nMotorType = GetProfileInt(L"GeneralVariables",L"Motor Controller",m_nMotorType);
	m_nLanguage = GetProfileInt(L"GeneralVariables",L"Language",m_nLanguage);
	m_nPrimarySide = GetProfileInt(L"GeneralVariables",L"Primary Side",m_nPrimarySide);
	m_nSide0Orientation = GetProfileInt(L"GeneralVariables",L"Side 0 Orientation",m_nSide0Orientation);
	m_nInitialLogonLevel = GetProfileInt(L"GeneralVariables",_T("LogonLevel"),m_nInitialLogonLevel);
#endif // !NO_REGISTRY


}

void CUSLScannerApp::SaveRegistryVariables()
{
	CString Buff;
#ifndef NO_REGISTRY

	WriteProfileString(L"GeneralVariables",L"LastSettings",m_LastSettingsPath.GetBuffer(1));
	WriteProfileString(L"GeneralVariables",_T("ProbesPath"),m_ProbesPath);
	WriteProfileString(L"GeneralVariables",_T("ArchivePath"),m_ArchivePath);
	WriteProfileString(L"GeneralVariables",_T("ProfilePath"),m_ProfilePath);
	WriteProfileString(L"GeneralVariables",_T("AptSourcePath"),m_AptSourcePath);
	WriteProfileString(L"GeneralVariables",_T("UltrasonicsPath"),m_UltrasonicsPath);
	WriteProfileString(L"GeneralVariables",_T("PalettePath"),m_PalettePath);
	WriteProfileString(L"GeneralVariables",_T("ScanPath"),m_ScanPath);
	WriteProfileString(L"GeneralVariables",_T("DataBasePath"),m_DataBasePathName);
	WriteProfileString(L"GeneralVariables",_T("DataBaseFailPath"),m_DataBaseFailedToSavePathName);
	WriteProfileString(L"GeneralVariables",_T("ViewTemplateName"),m_ViewReportTemplateName);
	WriteProfileString(L"GeneralVariables",_T("WallImagePath"),m_WallImagePathName);
	WriteProfileString(L"GeneralVariables",_T("HelpPath"),m_HelpPathName);
	WriteProfileString(L"GeneralVariables",_T("DetailsWindowName"),m_DetailsWindowName);
	WriteProfileString(L"GeneralVariables",_T("AcrobatPath"),m_AcrobatPathName);
	WriteProfileString(L"GeneralVariables",_T("USLScannerManualPath"),m_USLScannerManualPathName);
	WriteProfileString(L"GeneralVariables",_T("PMACSRMPath"),m_PMACSRMPathName);
	WriteProfileString(L"GeneralVariables",_T("TrackerPath"),m_TrackerPathName);
	WriteProfileString(L"GeneralVariables",_T("WorkspacePath"),m_WorkspacePathName);
	WriteProfileString(L"GeneralVariables",_T("WordPadPath"),m_WordPadPathName);
	WriteProfileString(L"GeneralVariables",_T("CalibrationPath"),m_CalibrationPathName);

	for(int ii=0;ii<10;ii++) {
		Buff.Format(_T("StorePath %d"),ii);
		WriteProfileString(L"GeneralVariables",Buff,m_StorePath[ii]);
	}
	WriteProfileString(L"GeneralVariables",L"AndonPathName",m_AndonPathName);
	WriteProfileString(L"GeneralVariables",L"PmacProgramPath",m_PmacProgramPath);
	WriteProfileString(L"GeneralVariables",L"LimitAndKinematicName",m_LimitAndKinematicOffset);
	WriteProfileString(L"GeneralVariables",L"ThicknessCalibrationPathName",m_ThicknessCalibrationPathName);
	WriteProfileString(L"GeneralVariables",L"LeftSideOfDialogueName",m_DlgSideName[0]);
	WriteProfileString(L"GeneralVariables",L"RightSideOfDialogueName",m_DlgSideName[1]);
	WriteProfileString(L"GeneralVariables",L"Robot_1_EndEffector",m_EndEffectorPathName[0]);
	WriteProfileString(L"GeneralVariables",L"Robot_2_EndEffector",m_EndEffectorPathName[1]);
	WriteProfileString(L"GeneralVariables",L"Probe_1_Serial_Number",m_ProbeSerialNumber[0]);
	WriteProfileString(L"GeneralVariables",L"Probe_2_Serial_Number",m_ProbeSerialNumber[1]);
	WriteProfileString(L"GeneralVariables",L"Calibration_component_1",m_CalibrationComponent[0]);
	WriteProfileString(L"GeneralVariables",L"Calibration_component_2",m_CalibrationComponent[1]);
	WriteProfileString(L"GeneralVariables",L"3PtName_1",m_3PtName[0]);
	WriteProfileString(L"GeneralVariables",L"3PtName_2",m_3PtName[1]);
	WriteProfileString(L"GeneralVariables",L"3PtName_3",m_3PtName[2]);

	WriteProfileBinary(L"GeneralVariables",_T("UltrasonicHardware"),(unsigned char*)&m_bUtHardware,sizeof m_bUtHardware);
	WriteProfileBinary(L"GeneralVariables",_T("ServoHardware"),(unsigned char*)&m_bServoHardware,sizeof m_bServoHardware);
	WriteProfileBinary(L"GeneralVariables",_T("Si6Hardware"),(unsigned char*)&m_bSi6Hardware,sizeof m_bSi6Hardware);
	WriteProfileBinary(L"GeneralVariables",_T("UtEthernet"),(unsigned char*)&m_bUTEthernet,sizeof m_bUTEthernet);

	WriteProfileInt(L"GeneralVariables",_T("Customer"),m_nJobNumber);
	WriteProfileInt(L"GeneralVariables",_T("Motor Controller"),m_nMotorType);

	WriteProfileInt(L"GeneralVariables",L"Language",m_nLanguage);
	WriteProfileInt(L"GeneralVariables",L"Primary Side",m_nPrimarySide);
	WriteProfileInt(L"GeneralVariables",L"Side 0 Orientation",m_nSide0Orientation);

	WriteProfileInt(L"GeneralVariables",_T("LogonLevel"),m_nInitialLogonLevel);

#endif // !NO_REGISTRY

}

void CUSLScannerApp::RetrieveCardEntries()
{
	m_Si6Res.nIOAddrs = GetProfileInt(L"Cards",_T("SI6 IO Addrs"),m_Si6Res.nIOAddrs);
	m_Si6Res.nIORange = GetProfileInt(L"Cards",_T("SI6 IO Range"),m_Si6Res.nIORange);
	m_Si6Res.nMemAddrs = GetProfileInt(L"Cards",_T("SI6 Mem Addrs"),m_Si6Res.nMemAddrs);
	m_Si6Res.nMemRange = GetProfileInt(L"Cards",_T("SI6 Mem Range"),m_Si6Res.nMemRange);
	m_Si6Res.nIrq = GetProfileInt(L"Cards",_T("SI6 IRQ"),m_Si6Res.nIrq);

	m_UtRes.nIOAddrs = GetProfileInt(L"Cards",_T("Ultrasonics IO Addrs"),m_UtRes.nIOAddrs);
	m_UtRes.nIORange = GetProfileInt(L"Cards",_T("Ultrasonics IO Range"),m_UtRes.nIORange);
	m_UtRes.nMemAddrs = GetProfileInt(L"Cards",_T("Ultrasonics Mem Addrs"),m_UtRes.nMemAddrs);
	m_UtRes.nMemRange = GetProfileInt(L"Cards",_T("Ultrasonics Mem Range"),m_UtRes.nMemRange);
	m_UtRes.nIrq = GetProfileInt(L"Cards",_T("Ultrasonics IRQ"),m_UtRes.nIrq);

	m_PmacRes.nIOAddrs = GetProfileInt(L"Cards",_T("PMAC IO Addrs"),m_PmacRes.nIOAddrs);
	m_PmacRes.nIORange = GetProfileInt(L"Cards",_T("PMAC IO Range"),m_PmacRes.nIORange);
	m_PmacRes.nMemAddrs = GetProfileInt(L"Cards",_T("PMAC Mem Addrs"),m_PmacRes.nMemAddrs);
	m_PmacRes.nMemRange = GetProfileInt(L"Cards",_T("PMAC Mem Range"),m_PmacRes.nMemRange);
	m_PmacRes.nIrq = GetProfileInt(L"Cards",_T("PMAC IRQ"),m_PmacRes.nIrq);

}

void CUSLScannerApp::SaveCardEntries()
{
	WriteProfileInt(L"Cards",_T("SI6 IO Addrs"),m_Si6Res.nIOAddrs);
	WriteProfileInt(L"Cards",_T("SI6 IO Range"),m_Si6Res.nIORange);
	WriteProfileInt(L"Cards",_T("SI6 Mem Addrs"),m_Si6Res.nMemAddrs);
	WriteProfileInt(L"Cards",_T("SI6 Mem Range"),m_Si6Res.nMemRange);
	WriteProfileInt(L"Cards",_T("SI6 IRQ"),m_Si6Res.nIrq);

	WriteProfileInt(L"Cards",_T("Ultrasonics IO Addrs"),m_UtRes.nIOAddrs);
	WriteProfileInt(L"Cards",_T("Ultrasonics IO Range"),m_UtRes.nIORange);
	WriteProfileInt(L"Cards",_T("Ultrasonics Mem Addrs"),m_UtRes.nMemAddrs);
	WriteProfileInt(L"Cards",_T("Ultrasonics Mem Range"),m_UtRes.nMemRange);
	WriteProfileInt(L"Cards",_T("Ultrasonics IRQ"),m_UtRes.nIrq);

	WriteProfileInt(L"Cards",_T("PMAC IO Addrs"),m_PmacRes.nIOAddrs);
	WriteProfileInt(L"Cards",_T("PMAC IO Range"),m_PmacRes.nIORange);
	WriteProfileInt(L"Cards",_T("PMAC Mem Addrs"),m_PmacRes.nMemAddrs);
	WriteProfileInt(L"Cards",_T("PMAC Mem Range"),m_PmacRes.nMemRange);
	WriteProfileInt(L"Cards",_T("PMAC IRQ"),m_PmacRes.nIrq);

}

void CUSLScannerApp::SaveMaintenanceVariables()
{
	CString Buff;
	int nDistance;

	for(int nAxis = 0;nAxis<32;nAxis++) {
		Buff.Format(L"Axis %d Total Distance",nAxis);
		nDistance = (int)m_Axes[nAxis].fTotalDistance;
		WriteProfileInt(L"Maintenance",Buff,nDistance);

		
		Buff.Format(L"Axis %d Last Service Distance",nAxis);
		nDistance = (int)m_Axes[nAxis].fDistanceAtLastService;
		WriteProfileInt(L"Maintenance",Buff,nDistance);
	}
}

void CUSLScannerApp::RetrieveMaintenanceVariables()
{
	CString Buff;
	int nDistance = 0;

	for(int nAxis = 0;nAxis<32;nAxis++) {
		Buff.Format(L"Axis %d Total Distance",nAxis);
		nDistance = GetProfileInt(L"Maintenance",Buff,nDistance);
		m_Axes[nAxis].fTotalDistance = (float)nDistance;

		Buff.Format(L"Axis %d Last Service Distance",nAxis);
		nDistance = GetProfileInt(L"Maintenance",Buff,nDistance);
		m_Axes[nAxis].fDistanceAtLastService = (float)nDistance;
	}
}


void CUSLScannerApp::ConfineToScreen(CRect *pRect)
{
	int nHPixel = GetSystemMetrics(78);
	if (pRect->right > nHPixel) {
		pRect->left = nHPixel - pRect->Width();
		pRect->right = nHPixel;
	}

	int nVPixel = GetSystemMetrics(79);
	if (pRect->top < 0) {
		pRect->bottom += -pRect->top;
		pRect->top = 0;
	}
	if (pRect->bottom > nVPixel) {
		pRect->top = nVPixel - pRect->Height();
		pRect->bottom = nHPixel;
	}

}

void CUSLScannerApp::ConfineToScreen(RECT *pRect)
{
	int nHPixel = GetSystemMetrics(78);
	int nWidth = pRect->right - pRect->left;
	if(pRect->right > nHPixel) {
		pRect->left = nHPixel - nWidth;
		pRect->right = nHPixel;
	}
	int nVPixel = GetSystemMetrics(79);
	int nHeight = pRect->bottom - pRect->top;
	if (pRect->bottom > nVPixel) {
		pRect->top = nVPixel - nHeight;
		pRect->bottom = nHPixel;
	}

}


void CUSLScannerApp::AudioAlarm()
{
	bool bAlarm = false;
	int	nArray[4];
	int nValue;

	if(m_Tank.nAudioMaskSelect & 1) {		//check magnetic knock off
		if(m_Axes[1].nStatus & PMAC_MAX_SWITCH) bAlarm = true;
		if(m_Axes[6].nStatus & PMAC_MIN_SWITCH) bAlarm = true;

	}
	if(m_Tank.nAudioMaskSelect & 2) {		//check water velocity
		m_PmacUser.ReportPumpEncoderValues(nArray);

		if(m_PmacUser.ReportPumpStatus(m_Tank.nLeftPump) ) {
			if(abs(m_Axes[m_Tank.nLeftPump].nMaxSpeed - ((nArray[2]-m_nOldPumpArray[2]) * 3)) > MulDiv(m_Axes[m_Tank.nLeftPump].nMaxSpeed, 20, 100)) bAlarm = true; 
		}
		if(m_PmacUser.ReportPumpStatus(m_Tank.nRightPump) ) {
			if(abs(m_Axes[m_Tank.nRightPump].nMaxSpeed - ((nArray[3]-m_nOldPumpArray[3]) * 3)) > MulDiv(m_Axes[m_Tank.nRightPump].nMaxSpeed, 20, 100)) bAlarm = true; 
		}
		CopyMemory(m_nOldPumpArray,nArray,sizeof nArray);
	}

//	if(m_bAudioAlarm != bAlarm) {
		switch(m_Tank.nAudioPort) {
		case 0:
			nValue = m_PmacUser.GetGeneralOuput();
			bAlarm == false ? nValue &= ~m_Tank.nAudioBit : nValue |= m_Tank.nAudioBit;
//			m_PmacUser.WriteGeneralOuput(nValue);
			break;
		case 1:
			nValue = m_Si10User.GetGeneralOuput();
			bAlarm == true ? nValue &= ~m_Tank.nAudioBit : nValue |= m_Tank.nAudioBit;
			m_Si10User.WriteGeneralOuput(nValue);
			break;
		}
		m_bAudioAlarm = bAlarm;
//	}
}

bool CUSLScannerApp::FirstInstance(CString strVersion)
{
    CWnd *pWndPrev, *pWndChild;
	

	// Determine if another window with our class name exists...
    if (pWndPrev = CWnd::FindWindow(NULL,strVersion)) {
		// if so, does it have any popups?
        pWndChild = pWndPrev->GetLastActivePopup();

        // If iconic, restore the main window
        if (pWndPrev->IsIconic())
             pWndPrev->ShowWindow(SW_RESTORE);

        // Bring the main window or its popup to
        // the foreground
        pWndChild->SetForegroundWindow();

        return FALSE;
	}
    
    return TRUE;
          
}

void CUSLScannerApp::SetStringOnToClipboard(CString Buff)
{
	HGLOBAL hClipboardData;
	WCHAR * pchData;
	int ii;

	if ( OpenClipboard(NULL) )	{
		EmptyClipboard();
		hClipboardData = GlobalAlloc(GMEM_DDESHARE, (Buff.GetLength()+1) * sizeof WCHAR);
		pchData = (WCHAR*)GlobalLock(hClipboardData);
		for(ii=0;ii<Buff.GetLength();ii++) {
			pchData[ii] = Buff.GetAt(ii);
		}
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_UNICODETEXT,hClipboardData);
		CloseClipboard();
	}


}


void CUSLScannerApp::StartLogging()
{
	return;
	CString FileName = L"C:\\Logging.txt";
	CUSLFile File;
	CFileException e;

	if(m_pLoggingAr == NULL) {

		if(!m_FileLogging.Open(FileName, CFile::modeCreate|CFile::typeBinary|CFile::modeReadWrite|CFile::modeNoTruncate, &e)) {

		} else {
			m_FileLogging.SeekToEnd();
			m_pLoggingAr = new CArchive(&m_FileLogging,CArchive::store);
		}
	}
}

void CUSLScannerApp::StopLogging()
{
	return;
	if(m_pLoggingAr) {
		m_pLoggingAr->Close();
		SAFE_DELETE( m_pLoggingAr );
		m_FileLogging.Close();
	}
}

void CUSLScannerApp::AddToLogging(CString *pBuff)
{
	return;
	m_pLoggingAr->WriteString(pBuff->GetBuffer(256));
}

void CUSLScannerApp::LinearTracker()
{
	float fdB;

	if(m_bTracking == TRUE) {
		if(m_UtUser.m_TS[0].Gate.nAmplitude[0] > 10) {
			fdB = 20.0f *log10f(102.0f / (float)m_UtUser.m_TS[0].Gate.nAmplitude[0]);
			if(fdB>2.0f) fdB = 2.0f;
			if(fdB<-2.0f) fdB = -2.0f;
			if(fabsf(fdB) >= 0.4) {
				m_nSuspendProcessingCount=5;
				m_UtUser.m_TS[0].Pr30.fLinGain += fdB;
				if(m_UtUser.m_TS[0].Pr30.fLinGain<0.0f) m_UtUser.m_TS[0].Pr30.fLinGain = 0.0f;
				if(m_UtUser.m_TS[0].Pr30.fLinGain>30.0f) m_UtUser.m_TS[0].Pr30.fLinGain = 30.0f;
				m_CriticalSection.Lock();
				m_UtUser.Pr30LinFloatGain(&m_UtUser.m_TS[0].Pr30);
				m_CriticalSection.Unlock();
			}
		}
	}
}

void CUSLScannerApp::CheckQuaternionMaths()
{
    D3DXVECTOR3 vAxis = D3DXVECTOR3(0.7f, 0.6f, 0.2f);
	D3DXMATRIX mRotation, mM;
	float fAngle = PIf / 3.0f;

	float c = cosf(fAngle);
	float s = sinf(fAngle);
	float t = 1.0f - c;

	D3DXVec3Normalize(&vAxis, &vAxis);

	mM._11 = (t*vAxis.x*vAxis.x) + c;			mM._12 = (t*vAxis.x*vAxis.y) + (vAxis.z*s);	mM._13 = (t*vAxis.x*vAxis.z) - (vAxis.y*s);	mM._14 = 0.0f;
	mM._21 = (t*vAxis.x*vAxis.y) - (vAxis.z*s);	mM._22 = (t*vAxis.y*vAxis.y) + c;			mM._23 = (t*vAxis.y*vAxis.z) + (vAxis.x*s);	mM._24 = 0.0f;
	mM._31 = (t*vAxis.x*vAxis.z) + (vAxis.y*s);	mM._32 = (t*vAxis.y*vAxis.z) - (vAxis.x*s);	mM._33 = (t*vAxis.z*vAxis.z) + c;			mM._34 = 0.0f;
	mM._41 = 0.0f;								mM._42 = 0.0f;								mM._43 = 0.0f;								mM._44 = 1.0f;

	D3DXMatrixRotationAxis(&mRotation,&vAxis,fAngle);

	float fNewAngl = -atan2f(-mM._23, -mM._13);
//	exit(0);
}

void CUSLScannerApp::InvertNormScannerSpecific(CCoord* Cp, int nSide )
{

	switch(m_Tank.nScannerDescription) {
	default:
		Cp->Side[nSide].fI *= -1.0f;
		Cp->Side[nSide].fJ *= -1.0f;
		Cp->Side[nSide].fK *= -1.0f;
		break;
	case SPX_ROBOT:
		Cp->Side[nSide].fI *= -1.0f;
//		Cp->Side[nSide].fJ *= -1.0f;
//		Cp->Side[nSide].fK *= -1.0f;
		break;
	}
}

LARGE_INTEGER CUSLScannerApp::getHighResCount()
{
	LARGE_INTEGER CurrentCount;

	QueryPerformanceCounter(&CurrentCount);
	return CurrentCount;
}


//Return time in seconds
double CUSLScannerApp::getHighResTime()
{
	LARGE_INTEGER CurrentCount;

	if (m_CPUFrequency.QuadPart == 0) QueryPerformanceFrequency(&m_CPUFrequency);

	QueryPerformanceCounter(&CurrentCount);

	return ((double)CurrentCount.QuadPart / (double)m_CPUFrequency.QuadPart);
}

LARGE_INTEGER getHighResCount()
{
	LARGE_INTEGER CurrentCount;

	QueryPerformanceCounter(&CurrentCount);
	return CurrentCount;
}


int CUSLScannerApp::getElapsedTime_us(LARGE_INTEGER StartCount, LARGE_INTEGER *pStopCount)
{
	LARGE_INTEGER CurrentCount, ElapsedMicroseconds;

	if (m_CPUFrequency.QuadPart == 0) QueryPerformanceFrequency(&m_CPUFrequency);

	QueryPerformanceCounter(&CurrentCount);

	ElapsedMicroseconds.QuadPart = CurrentCount.QuadPart - StartCount.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	int nElapsedPeriod = (int)(ElapsedMicroseconds.QuadPart / m_CPUFrequency.QuadPart);

	if (pStopCount)
		*pStopCount = CurrentCount;

	return nElapsedPeriod;
}


bool CUSLScannerApp::StartThread(CString Name, AFX_THREADPROC pfnCallback, LPVOID pParam, int nRefreshRate_ms, int nPriority)
{
	for (int nIndex = 0; nIndex < MAX_THREAD_COUNT; nIndex++) {
		if (m_USLThread[nIndex].IsActive() == false) {
			m_USLThread[nIndex].Initialize(Name, pfnCallback, pParam, nRefreshRate_ms, nPriority);
			m_USLThread[nIndex].Start();
			return true;
		}
	}
	return false;
}

bool CUSLScannerApp::SuspendThread(LPVOID pParam)
{
	for (int nIndex = 0; nIndex < MAX_THREAD_COUNT; nIndex++) {
		if (m_USLThread[nIndex].IsActive() == true && pParam == m_USLThread[nIndex].getParam()) {
			m_USLThread[nIndex].Suspend();
			return true;
		}
	}
	return false;

}

bool CUSLScannerApp::IsAccessible(int nID)
{
	MinMax(&nID, 0, sizeof m_cAccess);
	if (m_nAccessLevelMask & m_cAccess[nID]) return true;
	return false;
}

bool CUSLScannerApp::IsAccessLevel(AccessLevel Level)
{
	if (m_nAccessLevelMask == Level) return true;
	return false;
}


void CUSLScannerApp::TestTurntableRollOver()
{
	int nEncoderPos = 0x81000000;
	double dStepsRev = 1032192;

	double dPos = (double)((unsigned int)nEncoderPos);

	theApp.m_Tank.nRLeft = 21;
	theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter = (int)(dPos / dStepsRev);
	theApp.m_Axes[theApp.m_Tank.nRLeft].nPos = (int)fmod(dPos, dStepsRev);

}

void CUSLScannerApp::FormatTime(CString &Buff, int nTimens, int nUnits, CString Units)
{
	switch (nUnits) {
	case 0: Buff.Format(_T("%d %s"), nTimens, Units);		//ns
		break;
	case 1: Buff.Format(_T("%.02f%s"), (float)nTimens / 1000.0f, Units);	//us
		break;
	case 2: Buff.Format(_T("%.02f %s"), (float)(nTimens * theApp.m_LastSettings.nWaterVelocity) / 2e6f, Units);	//mm Water
		break;
	case 3: Buff.Format(_T("%.02f %s"), (float)(nTimens * theApp.m_LastSettings.nMaterialVelocity) / 2e6f, Units);	//mm Material
		break;
	case 4: Buff.Format(_T("%.04f %s"), (float)(nTimens * theApp.m_LastSettings.nWaterVelocity) / 50.8e6f, Units);	// inch Water
		break;
	case 5: Buff.Format(_T("%.04f %s"), (float)(nTimens * theApp.m_LastSettings.nMaterialVelocity) / 50.8e6f, Units);	//inch Material
		break;
	}
}

int CUSLScannerApp::ScanfDistanceTimens(CString Buff, int nUnits)
{
	float fTemp;
	int nTimens;

	_WTOF(Buff, fTemp);
	switch (nUnits) {
	case 0:	nTimens = (int)fTemp;
		break;
	case 1:	nTimens = (int)(fTemp * 1000.0f);
		break;
	case 2: nTimens = (int)(fTemp * 2e6f) / theApp.m_LastSettings.nWaterVelocity;
		break;
	case 3:	nTimens = (int)(fTemp * 2e6f) / theApp.m_LastSettings.nMaterialVelocity;
		break;
	case 4:	nTimens = (int)(fTemp * 50.8e6f) / theApp.m_LastSettings.nWaterVelocity;
		break;
	case 5:	nTimens = (int)(fTemp * 50.8e6f) / theApp.m_LastSettings.nMaterialVelocity;
		break;
	};
	return nTimens;
}
