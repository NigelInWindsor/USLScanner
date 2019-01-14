// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// TODO: reference additional headers your program requires here
//#include<Windows.h>

// TODO: reference additional headers your program requires here
#include <stdio.h>
#include <stdlib.h>

#include "windows.h"
#include "Common.h"

#define _USE_MFC_

class CFile
{
public:
	bool Write(void *pData,int);
	int Read(void *pData,int);
};
