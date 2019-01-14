// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#define _USE_MFC_

class CFile
{
public:
	bool Write(void *pData,int);
	int Read(void *pData,int);
};

// TODO: reference additional headers your program requires here
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "UTKernelDriver.h"
#include "UTKernelDriverOEMPA.h"
#include "CustomizedDriverAPI.h"
#include "CustomizedWizardAPI.h"

#define DllExport		__declspec(dllexport)
#define DllImport		__declspec(dllimport)

