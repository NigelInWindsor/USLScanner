// RReceiveMessage.cpp: implementation of the CRReceiveMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "RReceiveMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRReceiveMessage* CRReceiveMessage::s_instance = NULL;

CRReceiveMessage* CRReceiveMessage::instance(){
	if(!s_instance){
		s_instance = new CRReceiveMessage();
	}
	return s_instance;
}


CRReceiveMessage::CRReceiveMessage()
{

}

CRReceiveMessage::~CRReceiveMessage()
{

}
