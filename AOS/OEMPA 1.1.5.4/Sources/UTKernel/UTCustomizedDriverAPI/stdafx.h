// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// TODO: reference additional headers your program requires here
#include "Common.h"
#include "win_stub.h"

//Options for "CustomizedDriverAPI.h"
#define _CUST_DRV_DECL_ __declspec(dllexport)
#define	_CUST_DRV_DEF_	__declspec(dllexport)
#include "myMFC.h"
#pragma comment(lib,"UTCom.lib")

#define DllExport		__declspec(dllexport)
#define DllImport		__declspec(dllimport)

#include "UTKernelDriver.h"
#include "UTKernelDriverOEMPA.h"
#include "CustomizedDriverAPI.h"
