// RSendMessage.h: interface for the CRSendMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSENDMESSAGE_H__E0BB5894_7A38_4B72_9B6D_6B84095E0207__INCLUDED_)
#define AFX_RSENDMESSAGE_H__E0BB5894_7A38_4B72_9B6D_6B84095E0207__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RMessage.h"

class CRSendMessage : public CRMessage  
{
public:
	static CRSendMessage* instance();
	virtual ~CRSendMessage();
	void Copy(const CRMessage &obj);

private:
	static CRSendMessage* s_instance;
	CRSendMessage();
};

#endif // !defined(AFX_RSENDMESSAGE_H__E0BB5894_7A38_4B72_9B6D_6B84095E0207__INCLUDED_)
