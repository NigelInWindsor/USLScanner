// RServerSocket.h: interface for the CRServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSERVERSOCKET_H__E40759D6_97E6_4183_AF77_9BD7FEFB293F__INCLUDED_)
#define AFX_RSERVERSOCKET_H__E40759D6_97E6_4183_AF77_9BD7FEFB293F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RClientSocket.h"
#include <memory>

class CRServerSocket : public CRSocket
{
public:
	CRServerSocket();
	virtual ~CRServerSocket();
	virtual void OnAccept(int nErrorCode);
	CRClientSocket* GetClientSocket(){return m_pRClientSocket;}
	virtual void Close();
	void OnClose(int iErrorCode);
private:
	CRClientSocket* m_pRClientSocket;
	CSingleLock* m_lock;
	CCriticalSection m_CritSection;
	void init();
	bool m_bConnected;
};

#endif // !defined(AFX_RSERVERSOCKET_H__E40759D6_97E6_4183_AF77_9BD7FEFB293F__INCLUDED_)
