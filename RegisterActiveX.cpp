// RegisterActiveX.cpp: implementation of the CRegisterActiveX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "RegisterActiveX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegisterActiveX::CRegisterActiveX()
{

}


void CRegisterActiveX::Register(CString DllPathName)
{
	HINSTANCE hLib = LoadLibrary(DllPathName);

	if (hLib < (HINSTANCE)HINSTANCE_ERROR)
	{
 	MessageBox(NULL,_T("Unable to load dll ") + DllPathName, _T("Warning!"), MB_OK|MB_SYSTEMMODAL );

	}

	FARPROC lpDllEntryPoint;
// Find the entry point.
/*(FARPROC&)*/ lpDllEntryPoint = GetProcAddress(hLib,"DllRegisterServer");
	if (lpDllEntryPoint != NULL)
	(lpDllEntryPoint)();
	else
	{
	MessageBox(NULL,_T("Unable to retreive the dllRegisterServer address  for the library ") + DllPathName, _T("Warning!"), MB_OK|MB_SYSTEMMODAL );
	}

}

CRegisterActiveX::~CRegisterActiveX()
{

}
