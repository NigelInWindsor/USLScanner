// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here
#include "Common.h"
#include "win_stub.h"

//Options for "CustomizedWizardAPI.h"
#define _CUST_WIZ_DECL_ __declspec(dllexport)
#define	_CUST_WIZ_DEF_	__declspec(dllexport)
#include "myMFC.h"
#pragma comment(lib,"UTCom.lib")

#define DllExport		__declspec(dllexport)
#define DllImport		__declspec(dllimport)

#include "UTKernelDriver.h"
#include "UTKernelDriverOEMPA.h"
#include "UTKernelAPI.h"
#include "CustomizedDriverAPI.h"
#include "CustomizedWizardAPI.h"
