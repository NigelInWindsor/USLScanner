// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "win_stub.h"
#include <ShellApi.h>
#include <tlhelp32.h> 

#define WPARAM_RUN_TOOLBOX			123
#define LPARAM_RUN_TOOLBOX_OK		1234
#define LPARAM_RUN_TOOLBOX_ERROR	12345

bool g_bToolbox=false;
HWND g_DefaultHwnd=NULL;
HWND g_hWndToolbox=NULL;
HWND g_hWndToolbox2=NULL;
LPARAM g_lParamEcho=0;

extern "C"{
__declspec(dllexport) VS_FIXEDFILEINFO g_VS_FIXEDFILEINFO={	0xFEEF04BD,//DWORD dwSignature;
												0,//DWORD dwStrucVersion;
												0x00010001,//DWORD dwFileVersionMS;
												0x00050004,//DWORD dwFileVersionLS;
												0x00010001,//DWORD dwProductVersionMS;
												0x00050004,//DWORD dwProductVersionLS;
												VS_FF_DEBUG | VS_FF_PRERELEASE | VS_FF_PATCHED | VS_FF_PRIVATEBUILD | VS_FF_INFOINFERRED | VS_FF_SPECIALBUILD,//DWORD dwFileFlagsMask;
#ifdef _DEBUG
												VS_FF_DEBUG,//DWORD dwFileFlags;
#else _DEBUG
#ifdef _PRERELEASE
												VS_FF_PRERELEASE,//DWORD dwFileFlags;
#else _PRERELEASE
												0,//DWORD dwFileFlags;
#endif _PRERELEASE
#endif _DEBUG
												VOS__WINDOWS32,//DWORD dwFileOS;
												VFT_DLL,//DWORD dwFileType;
												0,//DWORD dwFileSubtype;
												0,//DWORD dwFileDateMS;
												0//DWORD dwFileDateLS;
											};
}

UINT g_uiUTEventToolboxMessage=0;
const wchar_t g_szClassName[] = L"UTKernelMatlabClass";

LRESULT CALLBACK UTKernelMatlab_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult=0;

	if(g_uiUTEventToolboxMessage && (msg==g_uiUTEventToolboxMessage))
	{
		if(wParam==WPARAM_TOOLBOX_HWND)
		{
			g_hWndToolbox2 = (HWND)lParam;
		}
		if(wParam==WPARAM_TOOLBOX_ECHO)
		{
			g_lParamEcho = lParam;
		}
		if((wParam==WPARAM_RUN_TOOLBOX) && (lParam==LPARAM_RUN_TOOLBOX_OK))
		{
			//AfxMessageBox(L"No error to run the toolbox, please try again!");
		}
		if((wParam==WPARAM_RUN_TOOLBOX) && (lParam==LPARAM_RUN_TOOLBOX_ERROR))
		{
			//AfxMessageBox(L"Error to run the toolbox, please try again!");
		}
		return lResult;//DefWindowProc(hwnd, msg, wParam, lParam);
	}else
	switch(msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

HWND WINAPI DefaultWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hWnd;

	//Step 1: Registering the Window Class
	if(!GetClassInfoEx(hInstance,g_szClassName,&wc))
	{
		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = 0;
		wc.lpfnWndProc   = UTKernelMatlab_WndProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = hInstance;
		wc.hIcon         = NULL;//LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor       = NULL;//LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = g_szClassName;
		wc.hIconSm       = NULL;//LoadIcon(NULL, IDI_APPLICATION);
		if(!RegisterClassEx(&wc))
		{
			//AfxMessageBox(L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}
	}

	// Step 2: Creating the Window
	hWnd = CreateWindowEx(	WS_EX_CLIENTEDGE,
							g_szClassName,
							L"UTKernelMatlab Default Window",
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, 250, 50,
							NULL, NULL, hInstance, NULL);

	if(hWnd == NULL)
	{
		//AfxMessageBox(L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Step 3: The Message Loop
	//while(GetMessage(&Msg, NULL, 0, 0) > 0)
	//{
	//	TranslateMessage(&Msg);
	//	DispatchMessage(&Msg);
	//}
	//return Msg.wParam;
	return hWnd;
}

char *g_pTrace=NULL;
int g_iTraceIndex=0;
const int g_iTraceSize=1024*1024*16;
void TraceAlloc()
{
g_pTrace=new char[g_iTraceSize];
memset(g_pTrace,0,g_iTraceSize);
g_iTraceIndex=0;
}
void TraceClose()
{
	FILE *stream=NULL;
	errno_t err;

	if(!g_pTrace)
		return;

	// Open for read (will fail if file "crt_fopen_s.c" does not exist)
	err  = fopen_s( &stream, "C:\\Users\\Public\\dump.txt", "w" );
	if( err != 0 )
		return;//The file was not opened

	for(int iIndex=0;iIndex<g_iTraceIndex;iIndex++)
		fputc(g_pTrace[iIndex],stream);

	// Close stream if it is not NULL 
	if( stream )
		err = fclose( stream );
}
void TracePrintf(char *pTrace)
{
return;
	int iSize;

	if(!g_pTrace)
		return;
	iSize = (int)strlen(pTrace);
	if(g_iTraceIndex+iSize >= g_iTraceSize)
		return;
	memcpy(&g_pTrace[g_iTraceIndex],pTrace,iSize);
	g_iTraceIndex += iSize;
}
wchar_t g_wcUTKernelFolderDll[MAX_PATH]={0};//win_stub
bool ListProcessModules(const wchar_t *pSearchFor,int iSizeFull,wchar_t *wcUTSockFullPath,int iSize,wchar_t *wcUTSockPath);
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	int iData=0;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
//TraceAlloc();
		g_uiUTEventToolboxMessage = RegisterWindowMessage(L"UTKernelToolboxAPI");
		g_DefaultHwnd = DefaultWindow(hModule, SW_HIDE);//SW_SHOW);
		ListProcessModules(L"win_stub.dll",0,NULL,MAX_PATH,g_wcUTKernelFolderDll);
		//example "C:\Users\Public\SW\UTKernel\Win32\Release\"
	case DLL_THREAD_ATTACH:
		iData = 1;//breakpoint purpose
		break;
	case DLL_THREAD_DETACH:
		iData = 2;//breakpoint purpose
		break;
	case DLL_PROCESS_DETACH:
		iData = 3;//breakpoint purpose
//TraceClose();
		break;
	}
	return TRUE;
}

bool ListProcessModules(const wchar_t *pSearchFor,int iSizeFull,wchar_t *wcUTSockFullPath,int iSize,wchar_t *wcUTSockPath)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;
	bool bRet=false;
	wchar_t *pFound;

	//  Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetCurrentProcessId() );
	if( hModuleSnap == INVALID_HANDLE_VALUE )
	{
		//printError( "CreateToolhelp32Snapshot (of modules)" );
		return( false );
	}

	//  Set the size of the structure before using it.
	me32.dwSize = sizeof( MODULEENTRY32 );

	//  Retrieve information about the first module,
	//  and exit if unsuccessful
	if( !Module32First( hModuleSnap, &me32 ) )
	{
		//printError( "Module32First" ); // Show cause of failure
		CloseHandle( hModuleSnap );    // Must clean up the snapshot object!
		return( false );
	}

	//  Now walk the module list of the process,
	//  and display information about each module
	do
	{
		//printf( "\n\n     MODULE NAME:     %s",			me32.szModule );
		//printf( "\n     executable     = %s",			me32.szExePath );
		//printf( "\n     process ID     = 0x%08X",		me32.th32ProcessID );
		//printf( "\n     ref count (g)  =     0x%04X",	me32.GlblcntUsage );
		//printf( "\n     ref count (p)  =     0x%04X",	me32.ProccntUsage );
		//printf( "\n     base address   = 0x%08X",		(DWORD) me32.modBaseAddr );
		//printf( "\n     base size      = %d",			me32.modBaseSize );
		pFound = wcsstr(me32.szModule,pSearchFor);//L".exe");
		if(pFound)
		{
			bRet = true;
			if(wcUTSockFullPath && (iSizeFull>0))
				wcscpy_s(wcUTSockFullPath,iSizeFull,me32.szExePath);
			if(wcUTSockPath && (iSize>0))
			{
				wcscpy_s(wcUTSockPath,iSize,me32.szExePath);
				pFound = wcsstr(wcUTSockPath,me32.szModule);
				*pFound = 0;
			}
			break;
		}

	} while( Module32Next( hModuleSnap, &me32 ) );

	//  Do not forget to clean up the snapshot object.
	CloseHandle( hModuleSnap );
	return( bRet );
}

void RemoveDoublePoint(wchar_t *szExeFile)
{
	wchar_t *pFind,*pSlash,*pSlashOld;
	int iIndex;

	while(pFind = wcsstr(szExeFile,L"\\.."))
	{
		pSlash = wcsstr(szExeFile,L"\\");
		pSlashOld = pSlash;
		if((int)pSlash>(int)pFind)
			return;
		while((int)pSlash<(int)pFind)
		{
			pSlash = wcsstr(pSlashOld+1,L"\\");
			if((int)pSlash>=(int)pFind)
			{
				pSlash = pSlashOld;
				iIndex = 0;
				while(pFind[3+iIndex])
				{
					pSlash[iIndex] = pFind[3+iIndex];
					iIndex++;
				}
				pSlash[iIndex] = 0;
				break;
			}
			pSlashOld = pSlash;
		}
	}
}

bool FindProcessModule(const wchar_t *pExe,DWORD dwProcessId)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;
	bool bRet=false;
	wchar_t szExeFile[MAX_PATH];
	wchar_t szExeFile2[MAX_PATH];

	wcscpy_s(szExeFile,MAX_PATH,pExe);
	_wcslwr_s(szExeFile,MAX_PATH);
	//  Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwProcessId );
	if( hModuleSnap == INVALID_HANDLE_VALUE )
	{
		//printError( "CreateToolhelp32Snapshot (of modules)" );
		return( false );
	}

	//  Set the size of the structure before using it.
	me32.dwSize = sizeof( MODULEENTRY32 );

	//  Retrieve information about the first module,
	//  and exit if unsuccessful
	if( !Module32First( hModuleSnap, &me32 ) )
	{
		//printError( "Module32First" ); // Show cause of failure
		CloseHandle( hModuleSnap );    // Must clean up the snapshot object!
		return( false );
	}

	//  Now walk the module list of the process,
	//  and display information about each module
	do
	{
		//printf( "\n\n     MODULE NAME:     %s",			me32.szModule );
		//printf( "\n     executable     = %s",			me32.szExePath );
		//printf( "\n     process ID     = 0x%08X",		me32.th32ProcessID );
		//printf( "\n     ref count (g)  =     0x%04X",	me32.GlblcntUsage );
		//printf( "\n     ref count (p)  =     0x%04X",	me32.ProccntUsage );
		//printf( "\n     base address   = 0x%08X",		(DWORD) me32.modBaseAddr );
		//printf( "\n     base size      = %d",			me32.modBaseSize );
		wcscpy_s(szExeFile2,MAX_PATH,me32.szExePath);
		_wcslwr_s(szExeFile2,MAX_PATH);
		RemoveDoublePoint(szExeFile2);
		if(!wcscmp(szExeFile2,szExeFile))
		{
			bRet = true;
			break;
		}

	} while( Module32Next( hModuleSnap, &me32 ) );

	//  Do not forget to clean up the snapshot object.
	CloseHandle( hModuleSnap );
	return( bRet );
}

bool FindProcess(const wchar_t *pExe,DWORD &dwProcessId)
{
	HANDLE hProcessSnap;
	//HANDLE hProcess;
	PROCESSENTRY32 pe32;
	wchar_t szExeFile[MAX_PATH];
	wchar_t szExeFile2[MAX_PATH];
	wchar_t *pSlash;

	wcscpy_s(szExeFile,MAX_PATH,pExe);
	_wcslwr_s(szExeFile,MAX_PATH);
	pSlash = wcsstr(szExeFile,L"\\");
	if(pSlash)
	{
		while(wcsstr(pSlash+1,L"\\"))
			pSlash = wcsstr(pSlash+1,L"\\");
		wcscpy_s(szExeFile,MAX_PATH,pSlash+1);
	}
	dwProcessId = 0;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
		return( false );

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		CloseHandle( hProcessSnap );    // Must clean up the snapshot object!
		return( false );
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do{
		//printf( "\nPROCESS NAME:  %s", pe32.szExeFile );
		//printf( "\n  process ID        = 0x%08X", pe32.th32ProcessID );
		//printf( "\n  thread count      = %d",   pe32.cntThreads );
		//printf( "\n  parent process ID = 0x%08X",
		//pe32.th32ParentProcessID );
		//printf( "\n  Priority Base     = %d", pe32.pcPriClassBase );
		//if( dwPriorityClass )
		//printf( "\n  Priority Class    = %d", dwPriorityClass );
		// List the modules and threads associated with this process
		//ListProcessModules( pe32.th32ProcessID );
		//ListProcessThreads( pe32.th32ProcessID );
		wcscpy_s(szExeFile2,MAX_PATH,pe32.szExeFile);
		_wcslwr_s(szExeFile2,MAX_PATH);
		if(!wcscmp(szExeFile2,szExeFile) && FindProcessModule(pExe,pe32.th32ProcessID))
		{
			dwProcessId = pe32.th32ProcessID;
			break;
		}

	}while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );
	if(dwProcessId)
		return( true );
	else
		return( false );
}

typedef struct FINDWINDOWHANDLESTRUCT
{
	DWORD dwProcessId;
	HWND hWndFound;
}FINDWINDOWHANDLESTRUCT;
BOOL CALLBACK EnumWindowCallBack(HWND hwnd, LPARAM lParam) 
{ 
	FINDWINDOWHANDLESTRUCT * pfwhs = (FINDWINDOWHANDLESTRUCT * )lParam; 
	DWORD ProcessId; 
	wchar_t display[128] = L"";
	GetWindowThreadProcessId ( hwnd, &ProcessId ); 
	LONG lResult;

	// note: In order to make sure we have the MainFrame, verify that the title 
	// has Zero-Length. Under Windows 98, sometimes the Client Window ( which doesn't 
	// have a title ) is enumerated before the MainFrame 

	lResult = (LONG)SendMessage(hwnd, WM_GETTEXT, 100, (LPARAM)display);//return the number of character copied.
	if ( ProcessId  == pfwhs->dwProcessId && lResult && wcslen(display) != 0)
	{ 
		if(wcsstr(display,L"Toolbox")) 
		{
			pfwhs->hWndFound = hwnd;
			return false; 
		}
		if(wcsstr(display,L"ToolBox")) 
		{
			pfwhs->hWndFound = hwnd;
			return false; 
		}
	} 
	//else 
	{ 
		// Keep enumerating 
		return true; 
	} 
}
bool GetMainWndFromProcessId(DWORD dwProcessId,HWND &hWnd)
{
	FINDWINDOWHANDLESTRUCT fwhs;

	fwhs.dwProcessId = dwProcessId;
	fwhs.hWndFound  = NULL;

	EnumWindows ( EnumWindowCallBack, (LPARAM)&fwhs ) ;
	if(fwhs.hWndFound)
	{
		hWnd = fwhs.hWndFound;
		return true;
	}
	return false;
}

bool UTKernelMatlab_ToolboxKeepAlive()
{
	if(!g_hWndToolbox)
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_KEEPALIVE, (LPARAM)0)==0x12345678)
		return true;
	g_hWndToolbox = NULL;
	g_hWndToolbox2 = NULL;
	g_lParamEcho = 0;
	return false;
}

DWORD UTKernelMatlab_ToolboxEcho(DWORD dwData)
{
	if(!g_hWndToolbox)
		return false;
	//g_lParamEcho = 0;
	PostMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_ECHO, (LPARAM)dwData);
	//Sleep(dwSleepTime);
	//if(g_lParamEcho == 1)
	//	return true;
	return (DWORD)g_lParamEcho;
}

bool UTKernelMatlab_ConnectToolbox(DWORD dwProcessId)
{
	int iCount=0;

	if(g_hWndToolbox)
	{
		if(UTKernelMatlab_ToolboxKeepAlive())
			return true;
	}
	if(!g_hWndToolbox && !g_hWndToolbox2)
	{
		Sleep(2500);//1.1.5.3l MatlabWizard otherwise the function below doesnt work each time.
		while(!GetMainWndFromProcessId(dwProcessId,g_hWndToolbox))
		{
			Sleep(100);
			iCount++;
			if(iCount>100)
				return false;
		}
		//::PostMessage(g_DefaultHwnd,g_uiUTEventToolboxMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
		::SendMessage(g_DefaultHwnd,g_uiUTEventToolboxMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
		PostMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_HWND, (LPARAM)g_DefaultHwnd);
		//Following lines => deadlock (because running on the main thread)
		//while(!g_hWndToolbox2)
		//	Sleep(100);
		//1.1.5.3l MatlabWizard DEADLOCK if called a second time
		//1.1.5.3l MatlabWizard PostMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_HWND, (LPARAM)g_DefaultHwnd);
		return true;
	}
	return false;
}
bool UTKernelMatlab_RunExe(bool bWaitClose,bool bShow,const wchar_t* pExeName)
{
	SHELLEXECUTEINFO execinfo;
	bool bRet;

	memset(&execinfo, 0, sizeof(execinfo));
	execinfo.lpFile = pExeName;
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = L"open";
	execinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	if(bShow)
		execinfo.nShow = SW_SHOWDEFAULT;
	else
		execinfo.nShow = SW_HIDE;
	if(ShellExecuteEx(&execinfo))//When DLLs are loaded into your process, you acquire a lock known as a loader lock. The DllMain function always executes under the loader lock. It is important that you do not call ShellExecuteEx while you hold a loader lock. Because ShellExecuteEx is extensible, you could load code that does not function properly in the presence of a loader lock, risking a deadlock and therefore an unresponsive thread.
		bRet = true;
	else
		bRet = false;
	if(bWaitClose)
		WaitForSingleObject(execinfo.hProcess, INFINITE);
	else{
		//Sleep(4000);//tempo to be sure that it has been launched correctly "UTKernel_IsToolboxRunning" will be calleable just after returning this function.
		g_hWndToolbox = NULL;
		g_hWndToolbox2 = NULL;
		if(!UTKernelMatlab_ConnectToolbox(GetProcessId(execinfo.hProcess)))
			bRet = false;
	}
	return bRet;
}
bool UTKernelMatlab_ToolboxRun(bool bShow)
{
	wchar_t pToolbox[MAX_PATH];

	swprintf(pToolbox, MAX_PATH, L"%sUTToolbox.exe", g_wcUTKernelFolderDll);
	return UTKernelMatlab_RunExe(false,bShow,pToolbox);
}
DWORD UTKernelMatlab_GetToolboxProcessId()
{
	wchar_t pToolbox[MAX_PATH];
	DWORD dwProcessId;

	swprintf(pToolbox, MAX_PATH, L"%sUTToolbox.exe", g_wcUTKernelFolderDll);
	if(!FindProcess(pToolbox,dwProcessId))
		return 0;
	return dwProcessId;
}

bool UTKernelMatlab_ToolboxClose()
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_CLOSE, (LPARAM)0)==0x12345678)
		return true;
	else
		return false;
}

bool UTKernelMatlab_ToolboxVisible(bool bEnable)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_VISIBLE, (LPARAM)(bEnable?true:false))==0x12345678)
		return true;
	else
		return false;
}
bool UTKernelMatlab_EnableEventUpdate(bool bEnable)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_ENABLE_EVENT_UPDATE, (LPARAM)(bEnable?true:false))==0x12345678)
		return true;
	else
		return false;
}

bool UTKernelMatlab_ToolboxDisplay(int iRootId)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_DISPLAY, (LPARAM)iRootId)==0x12345678)
		return true;
	else
		return false;
}

bool UTKernelMatlab_ToolboxWizardNew()
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_WIZARD_NEW, (LPARAM)0)==0x12345678)
		return true;
	return false;
}

bool UTKernelMatlab_ToolboxWizardDelete()
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_WIZARD_DEL, (LPARAM)0)==0x12345678)
		return true;
	return false;
}

bool UTKernelMatlab_ToolboxWizardUpdateScan()
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_WIZARD_UPDATE, (LPARAM)0)==0x12345678)
		return true;
	return false;
}

bool UTKernelMatlab_ToolboxIsWizardEnabled()
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_WIZARD_ISENABLED, (LPARAM)0)==0x12345678)
		return true;
	return false;
}

bool UTKernelMatlab_ToolboxWizardReset()
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_WIZARD_RESET, (LPARAM)0)==0x12345678)
		return true;
	return false;
}

int UTKernelMatlab_GetWizardBeamCount()
{
	int iCount=0;

	if(!UTKernelMatlab_ToolboxKeepAlive())
		return 0;
	memset(&g_ToolboxWizardParameter,0,sizeof(g_ToolboxWizardParameter));
	g_ToolboxWizardParameter.iCount = 0;
	g_ToolboxWizardParameter.iIndex = -1;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_WIZARD_BEAM_COUNT, (LPARAM)0)==0x12345678)
		return g_ToolboxWizardParameter.iCount;
	return 0;
}

bool UTKernelMatlab_GetWizardBeam(int iBeamIndex,double &dProbeX,double &dProbeY,double &dProbeZ,double &dEmergenceX,double &dEmergenceY,double &dEmergenceZ,double &dFocalX,double &dFocalY,double &dFocalZ)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	memset(&g_ToolboxWizardParameter,0,sizeof(g_ToolboxWizardParameter));
	g_ToolboxWizardParameter.iIndex = iBeamIndex;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_WIZARD_BEAM, (LPARAM)0)==0x12345678)
	{
		dProbeX = g_ToolboxWizardParameter.dProbeX;
		dProbeY = g_ToolboxWizardParameter.dProbeY;
		dProbeZ = g_ToolboxWizardParameter.dProbeZ;
		dEmergenceX = g_ToolboxWizardParameter.dEmergenceX;
		dEmergenceY = g_ToolboxWizardParameter.dEmergenceY;
		dEmergenceZ = g_ToolboxWizardParameter.dEmergenceZ;
		dFocalX = g_ToolboxWizardParameter.dFocalX;
		dFocalY = g_ToolboxWizardParameter.dFocalY;
		dFocalZ = g_ToolboxWizardParameter.dFocalZ;
		return true;
	}
	return false;
}

int UTKernelMatlab_GetWizardElementCount()
{
	int iCount=0;

	if(!UTKernelMatlab_ToolboxKeepAlive())
		return 0;
	memset(&g_ToolboxWizardParameter,0,sizeof(g_ToolboxWizardParameter));
	g_ToolboxWizardParameter.iCount = 0;
	g_ToolboxWizardParameter.iIndex = -1;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_WIZARD_ELEMENT_COUNT, (LPARAM)0)==0x12345678)
		return g_ToolboxWizardParameter.iCount;
	return 0;
}

bool UTKernelMatlab_GetWizardElement(int iBeamIndex,double &dProbeX,double &dProbeY,double &dProbeZ)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	memset(&g_ToolboxWizardParameter,0,sizeof(g_ToolboxWizardParameter));
	g_ToolboxWizardParameter.iIndex = iBeamIndex;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_WIZARD_ELEMENT, (LPARAM)0)==0x12345678)
	{
		dProbeX = g_ToolboxWizardParameter.dProbeX;
		dProbeY = g_ToolboxWizardParameter.dProbeY;
		dProbeZ = g_ToolboxWizardParameter.dProbeZ;
		return true;
	}
	return false;
}

bool UTKernelMatlab_GetWizardId(int &iWizardSystemId,int &iWizardChannelId)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	g_ToolboxWizardParameter.iCount = -1;
	g_ToolboxWizardParameter.iIndex = -1;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_WIZARD_GETID, (LPARAM)0)==0x12345678)
	{
		iWizardSystemId = g_ToolboxWizardParameter.iCount;
		iWizardChannelId = g_ToolboxWizardParameter.iIndex;
		return true;
	}
	return false;
}

bool UTKernelMatlab_GetToolboxParameter(wchar_t *pIn,int iSize,wchar_t *pOut)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	memset(&g_ToolboxWizardParameter,0,sizeof(g_ToolboxWizardParameter));
	wcscpy_s(g_ToolboxWizardParameter.pInput,MAX_PATH,pIn);
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_TOOLBOX_GET, (LPARAM)0)==0x12345678)
	{
		wcscpy_s(pOut,iSize,g_ToolboxWizardParameter.pOutput);
		return true;
	}
	return false;
}

bool UTKernelMatlab_SetToolboxParameter(wchar_t *pCmdLine)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	memset(&g_ToolboxWizardParameter,0,sizeof(g_ToolboxWizardParameter));
	wcscpy_s(g_ToolboxWizardParameter.pInput,MAX_PATH,pCmdLine);
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_TOOLBOX_SET, (LPARAM)0)==0x12345678)
		return true;
	return false;
}

bool UTKernelMatlab_ToolboxLoadWizard(wchar_t *pFileName)
{
	bool bRet=false;
	ATOM hAtom=NULL;
	DWORD dwAttributes;

	dwAttributes = GetFileAttributes(pFileName);
	if(dwAttributes==INVALID_FILE_ATTRIBUTES)
		return false;
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(!StringToAtom(pFileName,hAtom))
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_LOAD_WIZARD, (LPARAM)hAtom)==0x12345678)
		bRet = true;
	GlobalDeleteAtom(hAtom);
	return bRet;
}

bool UTKernelMatlab_ToolboxSaveWizard(wchar_t *pWizardFile,bool bSaveWizardResults)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	wcscpy_s(g_ToolboxWizardParameter.pInput,MAX_PATH,pWizardFile);
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_SAVE_WIZARD, (LPARAM)(bSaveWizardResults?1:0))==0x12345678)
		return true;
	return false;
}

bool UTKernelMatlab_ToolboxLoadMatrix(wchar_t *pFileName)
{
	bool bRet=false;
	ATOM hAtom=NULL;
	DWORD dwAttributes;

	dwAttributes = GetFileAttributes(pFileName);
	if(dwAttributes==INVALID_FILE_ATTRIBUTES)
		return false;
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(!StringToAtom(pFileName,hAtom))
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_LOAD_MATRIX, (LPARAM)hAtom)==0x12345678)
		bRet = true;
	GlobalDeleteAtom(hAtom);
	return bRet;
}

bool UTKernelMatlab_ToolboxSaveMatrix(wchar_t *pFileName)
{
	bool bRet=false;
	ATOM hAtom=NULL;
	
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(!StringToAtom(pFileName,hAtom))
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_SAVE_MATRIX, (LPARAM)hAtom)==0x12345678)
		bRet = true;
	GlobalDeleteAtom(hAtom);
	return bRet;
}

bool UTKernelMatlab_ToolboxSaveSetup(int iDeviceId, wchar_t *pSetupFile)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	wcscpy_s(g_ToolboxWizardParameter.pInput,MAX_PATH,L"");
	wcscpy_s(g_ToolboxWizardParameter.pOutput,MAX_PATH,pSetupFile);
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_SAVE_SETUP_DEFAULT, (LPARAM)iDeviceId)==0x12345678)
		return true;
	return false;
}
bool UTKernelMatlab_ToolboxSaveSetupEx(int iDeviceId, wchar_t *pSetupFileIn, wchar_t *pSetupFileOut)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	wcscpy_s(g_ToolboxWizardParameter.pInput,MAX_PATH,pSetupFileIn);
	wcscpy_s(g_ToolboxWizardParameter.pOutput,MAX_PATH,pSetupFileOut);
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_SAVE_SETUP, (LPARAM)iDeviceId)==0x12345678)
		return true;
	return false;
}

int UTKernelMatlab_ToolboxGetDeviceId()
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return -1;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_DEVICE_GETID, (LPARAM)0)==0x12345678)
		return g_ToolboxWizardParameter.iDeviceId;
	return -1;
}

int UTKernelMatlab_ToolboxGetRootId(wchar_t *pName)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return -1;
	wcscpy_s(g_ToolboxWizardParameter.pInput,MAX_PATH,pName);
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_GETROOTID, (LPARAM)0)==0x12345678)
		return g_ToolboxWizardParameter.iDeviceId;
	return -1;
}

bool UTKernelMatlab_ToolboxDeviceAddress(wchar_t *pIPAddressAndPort)
{
	wchar_t *pPort;
	
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	wcscpy_s(g_ToolboxWizardParameter.pIPAddress,50,pIPAddressAndPort);
	g_ToolboxWizardParameter.iPort = -1;
	pPort = wcsstr(g_ToolboxWizardParameter.pIPAddress,L":");
	if(pPort)
	{
		*pPort = 0;
		pPort++;
		g_ToolboxWizardParameter.iPort = _wtoi(pPort);
	}
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_DEVICE_ADDRESS, (LPARAM)0)==0x12345678)
		return true;
	return false;
}

bool UTKernelMatlab_ToolboxDeviceConnection(bool bConnect)
{
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_DEVICE_CONNECTION, (LPARAM)(bConnect?1:0))==0x12345678)
		return true;
	return false;
}

bool UTKernelMatlab_ToolboxIsDeviceConnected()
{
	bool bRet=false;
	
	if(!UTKernelMatlab_ToolboxKeepAlive())
		return false;
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_DEVICE_ISCONNECTED, (LPARAM)0)==0x12345678)
		bRet = true;
	return bRet;
}

wchar_t *UTKernel_GetToolboxDeviceType()
{
	static wchar_t type[MAX_PATH];
	
	if(!UTKernelMatlab_ToolboxKeepAlive())
	{
		wcscpy_s(type,MAX_PATH,L"Error");
		return type;
	}
	if(SendMessage(g_hWndToolbox, g_uiUTEventToolboxMessage, WPARAM_TOOLBOX_DEVICE_TYPE, (LPARAM)0)==0x12345678)
	{
		wcscpy_s(type,MAX_PATH,g_ToolboxWizardParameter.pOutput);
		return type;
	}
	wcscpy_s(type,MAX_PATH,L"Error");
	return type;
}
