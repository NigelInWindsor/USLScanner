// RClientSocket.h: interface for the CRClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCLIENTSOCKET_H__17511DAC_074B_4ECB_A697_7DFF1DEC592A__INCLUDED_)
#define AFX_RCLIENTSOCKET_H__17511DAC_074B_4ECB_A697_7DFF1DEC592A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RSocket.h"

class CRClientSocket : public CRSocket 
{
public:
	CRClientSocket();
	virtual BOOL Attach(SOCKET hSocket); 
	virtual ~CRClientSocket();
	CString GetCString();
	void GetBuffer(char* pBuf, unsigned int* iBytesReceived, int nBytesToRead);
	void GetByte(char* pBuf);
	int SendBuffer(char* pBuf, unsigned int nBytes);
	virtual void OnSend(int iErrorCode);
	virtual void OnClose(int iErrorCode);
	virtual void OnReceive(int iErrorCode);

private: 
	bool m_bNewMessage;
	CString m_buffReceived;
	CSingleLock* m_lock;
	CCriticalSection m_CritSection;
	CRITICAL_SECTION mutex;
	void init();
	HANDLE m_sendReady; //Send ready semaphore.
};
#endif // !defined(AFX_RCLIENTSOCKET_H__17511DAC_074B_4ECB_A697_7DFF1DEC592A__INCLUDED_)
