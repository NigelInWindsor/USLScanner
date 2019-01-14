// dllmain.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HINSTANCE g_hInstance=NULL;
wchar_t g_wcFolderData[MAX_PATH]=L"";
wchar_t g_wcFolderDataCfg[MAX_PATH]=L"";
bool g_bExit=false;

static AFX_EXTENSION_MODULE testDemoSettingDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("testDemoSetting.DLL Initializing!\n");
		g_hInstance = hInstance;
		wcscpy_s(g_wcFolderData,MAX_PATH,UTKernel_GetFolderData());
		wcscpy_s(g_wcFolderDataCfg,MAX_PATH,UTKernel_GetFolderDataCfg());
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(testDemoSettingDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(testDemoSettingDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("testDemoSetting.DLL Terminating!\n");
		g_bExit = true;
		UTKernel_Close();//clean the kernel

		// Terminate the library before destructors are called
		AfxTermExtensionModule(testDemoSettingDLL);
	}
	return 1;   // ok
}
