// WinVerUtils.cpp: implementation of the CWinVerUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "WinVerUtils.h"
#include <winnt.h>
#include <winbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinVerUtils::CWinVerUtils()
{

}

CWinVerUtils::~CWinVerUtils()
{

}

bool CWinVerUtils::IsWin2k()
{
	APP App = (APP) AfxGetApp();
    
	OSVERSIONINFOEX osvi;
    DWORDLONG dwlConditionMask = 0;

    // Initialize the OSVERSIONINFOEX structure.

    ::ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion = 5;
    osvi.dwMinorVersion = 0;

    // Initialize the condition mask.
    VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, VER_EQUAL );
    VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, VER_EQUAL );

    // Perform the test.
    return ::VerifyVersionInfo(
        &osvi,
        VER_MAJORVERSION | VER_MINORVERSION,
        dwlConditionMask);

}

bool CWinVerUtils::IsWin2k3OrLower()
{
	APP App = (APP) AfxGetApp();

    OSVERSIONINFOEX osvi;
    DWORDLONG dwlConditionMask = 0;

    // Initialize the OSVERSIONINFOEX structure.

    ::ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion = 5;
    osvi.dwMinorVersion = 2;

    // Initialize the condition mask.
    VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, VER_LESS_EQUAL );
    VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, VER_LESS_EQUAL );

    // Perform the test.
    return App->VerifyVersionInfo(
        &osvi,
        VER_MAJORVERSION | VER_MINORVERSION,
        dwlConditionMask);

}

bool CWinVerUtils::IsWin2008R2()
{
	APP App = (APP) AfxGetApp();

    OSVERSIONINFOEX osvi;
    DWORDLONG dwlConditionMask = 0;

    // Initialize the OSVERSIONINFOEX structure.

    ::ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion = 5;
    osvi.dwMinorVersion = 2;

    // Initialize the condition mask.
    VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, VER_LESS_EQUAL );
    VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, VER_LESS_EQUAL );

    // Perform the test.
    return App->VerifyVersionInfo(
        &osvi,
        VER_MAJORVERSION | VER_MINORVERSION,
        dwlConditionMask);

}
