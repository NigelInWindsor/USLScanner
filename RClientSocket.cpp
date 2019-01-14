// RClientSocket.cpp: implementation of the CRClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "RClientSocket.h"
#include <algorithm>
#include "MainFrm.h"
#include "USLScanner.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRClientSocket::CRClientSocket()
{
	CSocket();
	init();

}

BOOL CRClientSocket::Attach(SOCKET hSocket){
	return CSocket::Attach(hSocket);
}


void CRClientSocket::init(){
	m_bNewMessage = false;
	m_lock = new CSingleLock(&m_CritSection);
	m_sendReady = CreateSemaphore(NULL,0,1,NULL);
}



CRClientSocket::~CRClientSocket()
{
	delete m_lock;
}


CString CRClientSocket::GetCString() {
	wchar_t wbuff[4000];
	char buff[4000];
	int iwbuf = Receive(buff, sizeof(wbuff));
	m_buffReceived = (CString)wbuff;
	return m_buffReceived;	
}

void CRClientSocket::GetBuffer(char* pBuf, unsigned int* iBytesReceived, int nBytes){
	//Copy bytes from socket to raw buffer
	if(!SetTimeOut(1000))ASSERT(false);
	(*iBytesReceived) = (unsigned int)Receive(pBuf, nBytes);
	if(!KillTimeOut())ASSERT(false);
}

void CRClientSocket::GetByte(char* pBuf){
	//Copy bytes from socket to raw buffer
	int tmp = (unsigned int)Receive(pBuf,1);
}


void CRClientSocket::OnSend(int nErrorCode){
	CRSocket::OnSend(nErrorCode);
	ReleaseSemaphore(CRemote::m_sendSemaphore,1,NULL);
	ReleaseSemaphore(m_sendReady,1,NULL);

}


void CRClientSocket::OnReceive(int nErrorCode)
{	
	CRSocket::OnReceive(nErrorCode);
//	AfxGetApp()->m_pMainWnd->SendMessage(REMOTE_RMESSAGE_RECEIVE);
	((CUSLScannerApp*)AfxGetApp())->m_pRemote->Receive();
}


int CRClientSocket::SendBuffer(char* pBuf, unsigned int nBytes){
	int nBytesSent=0;
	int timeout=100;
	
	int nBytesRemaining=nBytes;
	do{
		int retval=SOCKET_ERROR;
	//	while(retval == SOCKET_ERROR){
			if(!SetTimeOut(1000))ASSERT(false);
			retval = Send(&pBuf[nBytesSent], nBytesRemaining);
			if(!KillTimeOut()) ASSERT(false);
			//If there is an error, OnSend will be called when the socket is available to send again.
			//This will release the semaphore m_sendReady, which we wait for here.
			if(retval == SOCKET_ERROR) {
				WaitForSingleObject(m_sendReady, timeout);
				if(CRemote::WaitForKillEvent(0)) return 0;
			}
	//	}
		if (retval == SOCKET_ERROR){
		//	CString errStr;
		//	errStr.Format(L"CRClientSocket::SendBuffer: send error. WSAGetLastError[%d]", WSAGetLastError());
		//	AfxMessageBox(errStr);
		//	AfxThrowUserException();
		//	continue;
			return retval;
		} else {
			nBytesSent+=retval;
			nBytesRemaining-=retval;
		}

	} while (nBytesRemaining>0);
	return nBytesSent;
}	

void CRClientSocket::OnClose(int iErrorCode){
	CRSocket::OnClose(iErrorCode);
}

