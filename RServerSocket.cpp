// RServerSocket.cpp: implementation of the CRServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "RServerSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRServerSocket::CRServerSocket()
{
	init();
}

void CRServerSocket::Close(){
	if(m_pRClientSocket != NULL) m_pRClientSocket->Close();
	CRSocket::Close();
}

void CRServerSocket::init(){
	m_pRClientSocket = new CRClientSocket();
	m_pRClientSocket->m_hSocket = INVALID_SOCKET;
	//m_lock = new CSingleLock(&m_CritSection);
}

CRServerSocket::~CRServerSocket()
{
	SAFE_DELETE(m_pRClientSocket);
}

void CRServerSocket::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	Accept(*m_pRClientSocket);
	if (INVALID_SOCKET == m_pRClientSocket->m_hSocket) {
		CString errStr;
		errStr.Format(L"=CRITICAL= | accept() failed. WSAGetLastError=[%d]\n", WSAGetLastError());
		AfxMessageBox(errStr);
	}
}

void CRServerSocket::OnClose(int iErrorCode){
	CRSocket::OnClose(iErrorCode);
}