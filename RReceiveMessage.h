// RReceiveMessage.h: interface for the CRReceiveMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RRECEIVEMESSAGE_H__7B7D1098_BFE4_483A_9364_BB422B1CD81C__INCLUDED_)
#define AFX_RRECEIVEMESSAGE_H__7B7D1098_BFE4_483A_9364_BB422B1CD81C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RMessage.h"

class CRReceiveMessage : public CRMessage  
{
public:
	static CRReceiveMessage* instance();
	virtual ~CRReceiveMessage();

private:
	CRReceiveMessage();
	static CRReceiveMessage* s_instance;
};

#endif // !defined(AFX_RRECEIVEMESSAGE_H__7B7D1098_BFE4_483A_9364_BB422B1CD81C__INCLUDED_)
