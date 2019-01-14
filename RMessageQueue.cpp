// RMessageQueue.cpp: implementation of the CRMessageQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "RMessageQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRMessageQueue::CRMessageQueue()
{
}

CRMessageQueue::~CRMessageQueue()
{
	CRMessage* mess = poll((long)0);
	while(mess != NULL){
		delete mess;
		mess = poll((long)0);
	}	
}
