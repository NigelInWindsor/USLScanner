// RSocket.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRSocket

CRSocket::CRSocket()
{
	m_bConnected = false;
}


CRSocket::~CRSocket()
{
}

void CRSocket::OnClose(int iErrorCode){
	CSocket::OnClose(iErrorCode);
	
	if(theApp.m_pRemote){
		theApp.m_pRemote->OnSocketClosed();
	}
//	ReleaseSemaphore(CRemote::m_socketCloseSemaphore,1,NULL); //Release the CRemote kill semaphore to close the threads and connection.
}

void CRSocket::Close(){
	m_bConnected=false;
	if(IsValidSocket()) CSocket::Close();
	m_hSocket = INVALID_SOCKET;
}

void CRSocket::OnConnect(int iErrorCode){
	if(iErrorCode == 0) {
		m_bConnected = true;
	} else {
		m_bConnected = false;
		CString errStr;
		errStr.Format(L"CRSocket - critical error: socket failed to connect. WSA error code = %d", iErrorCode);
		AfxMessageBox(errStr);
	}
}

bool CRSocket::SetTimeOut(UINT uTimeOut){
	m_nTimerID = SetTimer(NULL,0,uTimeOut,NULL);
	return m_nTimerID;
}

bool CRSocket::KillTimeOut(){
	return KillTimer(NULL, m_nTimerID);
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CRSocket, CSocket)
	//{{AFX_MSG_MAP(CRSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CRSocket member functions
