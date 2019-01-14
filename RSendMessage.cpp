// RSendMessage.cpp: implementation of the CRSendMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "RSendMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRSendMessage* CRSendMessage::s_instance = NULL;

CRSendMessage* CRSendMessage::instance(){
	if(!s_instance){
		s_instance = new CRSendMessage();
	}
	return s_instance;
}

CRSendMessage::CRSendMessage()
{

}

CRSendMessage::~CRSendMessage()
{

}


void CRSendMessage::Copy(const CRMessage &obj){
	SetDestination(obj.GetDestination());
	SetCommand(obj.GetCommand());;
	SetData(obj.GetData(),obj.GetDataSendSize());
}
