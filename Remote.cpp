// Remote.cpp: implementation of the CRemote class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Remote.h"
#include <windows.h>
#include "MainFrm.h"
#include "ScopeDlg.h"
#include "RMessageHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CRemote::CRemote()
{	
	m_pSendQueue = new CRMessageQueue();
	m_nSent=0;
	m_pParam = new THREADSTRUCT;
	m_pParam->_this = this;
	Defaults();
	//Initialise the message objects
	m_recMess = new CRMessage();
	m_sendMess = NULL;
	m_pMessageHandler = new CRMessageHandler();
//	for(int i=0; i<6; i++){
//		m_timers.push_back(new CLoopTimer());
//	}

	if(StartWinsock() != 0){
		CString errStr;
		errStr.Format(L"=CRITICAL= CRemote::CreateServerSocket() - unable to start winsock. WSAGetLastError[%d]", WSAGetLastError());
		AfxMessageBox(errStr);
	}
}

CRemote::~CRemote()
{	
	m_bRestart=false;
	Close();
	WSACleanup();
	DeleteSockets();
	DeleteServers();
//	for(std::vector<CLoopTimer*>::iterator iter = m_timers.begin();
//		iter!=m_timers.end();
//		iter++){
//			delete *iter;
//		}
//	m_timers.clear();
	delete m_pParam;
	delete m_recMess;
	SAFE_DELETE(m_sendMess);
	delete m_pMessageHandler;
	SAFE_DELETE(m_pBTServer);
	SAFE_DELETE(m_pSendQueue);
}

//	if(StartWinsock() != 0){
//		CString errStr;
//		errStr.Format(L"=CRITICAL= CRemote::CreateServerSocket() - unable to start winsock. WSAGetLastError[%d]", WSAGetLastError());
//		AfxMessageBox(errStr);
//	}

ULONG CRemote::StartWinsock() {
	ULONG       ulRetCode = 0;
	WSADATA     WSAData = { 0 };
	//todo	m_pRemoteBtAddr =  NULL ;

	if (0 == ulRetCode) {
		ulRetCode = WSAStartup(MAKEWORD(2, 2), &WSAData);
		if (0 != ulRetCode) {
			AfxMessageBox(L"CRemote::StartWinsock() -FATAL- | Unable to initialize Winsock version 2.2\n");
		}
	}
	InitSockets(&WSAData);
	return ulRetCode;
}

CWnd* CRemote::GetRemotePage(){
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(!pFrame) return NULL;
	if(!::IsWindow(pFrame->m_hWnd)) return NULL;
	CMaintenanceDiagnosticsSheet* mds = pFrame->m_pMaintenanceDiagnosticsSheet;
	if(!mds) return NULL;
	if(!::IsWindow(mds->m_hWnd)) return NULL;
	CWnd* rp = mds->m_pRemotePage;
	if(!rp) return NULL;
	if(!::IsWindow(rp->m_hWnd)) return NULL;
	return rp;	
}

	//Semaphores to indicate presence of message ready to send/receive
HANDLE CRemote::m_sendSemaphore = CreateSemaphore(NULL,0,1,NULL);
HANDLE CRemote::m_processSemaphore = CreateSemaphore(NULL,0,1,NULL);
HANDLE CRemote::m_updateDlgSemaphore = CreateSemaphore(NULL,1,1,NULL);
HANDLE CRemote::m_dataTransferSemaphore = CreateSemaphore(NULL,1,1,NULL);
HANDLE CRemote::m_hKillEvent = CreateSemaphore(NULL,0,1,NULL);
HANDLE CRemote::m_socketCloseSemaphore = CreateSemaphore(NULL,0,1,NULL);
HANDLE CRemote::m_sendClosed = CreateSemaphore(NULL,1,1,NULL);
//HANDLE CRemote::m_watcherClosed = CreateSemaphore(NULL,0,1,NULL);

void CRemote::Defaults(){
	m_iPort = 5000;
	SetOpen(false);
	SetConnected(false);
	SetInitialized(false);
	SetRestart(true);
	//Create the local CRSockets which is awrappers for a windows socket.
	//This socket also contains the client socket as a member variable.
	m_pLocalSocket = NULL;
	m_pConnectionSocket = NULL;
	//Create instances of sockets for WIFI or bluetooth and keep in memory. Use the above pointers to point to the required socket type.
	m_pBTLocalSocket = new CRServerSocket();
	m_pWIFILocalSocket = new CRServerSocket();
	m_pBTConnectionSocket = NULL;
	m_pWIFIConnectionSocket = NULL;
	m_pBTServer = NULL;
	m_conType = CT_BLUETOOTH;
	SetServerClosed();
}

void CRemote::SetServerClosed(){
	m_CritSection.Lock();
	m_bServerOpen=false;
	m_CritSection.Unlock();
}

void CRemote::SetServerOpen(){
	m_CritSection.Lock();
	m_bServerOpen=true;
	m_CritSection.Unlock();
}


bool CRemote::GetServerOpen(){
	bool val;
	m_CritSection.Lock();
	val=m_bServerOpen;
	m_CritSection.Unlock();
	return val;
}


bool CRemote::StartServer(){
	CloseThreads();
	m_bRestart=true;
	if(CreateServerSocket()){
		return Listen();
	}
	return false;
}

bool CRemote::SuspendServer(){
	m_bRestart=false;
	CRMessage* msg = new CRMessage();
	msg->SetDestination(L"REMT");
	msg->SetCommand(L"CLSE");
	Send(msg);
	return true;
}

void CRemote::InitSockets(){
	WSADATA* dat = NULL;
	InitSockets(dat);
}

void CRemote::InitSockets(WSADATA* dat){
	if (AfxSocketInit(dat) == FALSE){
		AfxMessageBox(L"Failed to Initialize Sockets");
	}
	
}

bool CRemote::StartBluetoothServer(){
	if(m_pBTServer == NULL){ //Create and start the server if not already created.
		m_pBTServer = new CRBluetoothServer();
	
		if(m_pBTServer->Start() != m_pBTServer->iCxnSuccess()){
		//	CString errStr;
		//	errStr.Format(L"=CRITICAL= CRemote::StartBluetoothServer() - unable to start bluetooth server. WSAGetLastError[%d]", WSAGetLastError());
		//	AfxMessageBox(errStr);
			return false;
		}
		int iRet = m_pBTLocalSocket->Attach(m_pBTServer->GetLocalSocket());
		if(iRet == 0){
		//	CString errStr;
		//	errStr.Format(L"=CRITICAL= CRemote::StartBluetoothServer() - unable to attach local socket to bluetooth socket. WSAGetLastError[%d]", WSAGetLastError());
		//	AfxMessageBox(errStr);
			return false;
		}
	} else {
		if(m_pBTServer->Restart() != m_pBTServer->iCxnSuccess()){
		//	CString errStr;
		//	errStr.Format(L"=CRITICAL= CRemote::StartBluetoothServer() - unable to restart bluetooth server. WSAGetLastError[%d]", WSAGetLastError());
		//	AfxMessageBox(errStr);
			return false;	
		}
	}
	m_pLocalSocket = m_pBTLocalSocket; //Set pointers to point to the bluetooth sockets.
	m_pBTConnectionSocket = (CRClientSocket*)((CRServerSocket*)m_pBTLocalSocket)->GetClientSocket();
	m_pConnectionSocket = m_pBTConnectionSocket;
	return true;
}

bool CRemote::OpenWifiSockets(){
		SOCKET s = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (s==INVALID_SOCKET){
			AfxMessageBox(::WSAGetLastError());
			AfxThrowUserException();
			return false;
		}
	
		m_pWIFILocalSocket->Socket(s);
		m_pWIFIConnectionSocket = (CRClientSocket*)((CRServerSocket*)m_pWIFILocalSocket)->GetClientSocket();
		m_pWIFIConnectionSocket->Socket(s);
		if (m_pWIFILocalSocket->Create(m_iPort) == FALSE)
		{
			AfxMessageBox(L"Failed to create server socket");
			AfxThrowUserException();
			return false;
		}

		int optBufSize = 1000;
		setsockopt(m_pWIFIConnectionSocket->m_hSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&optBufSize, sizeof(optBufSize));
		setsockopt(m_pWIFIConnectionSocket->m_hSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&optBufSize, sizeof(optBufSize));
		setsockopt(m_pWIFILocalSocket->m_hSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&optBufSize, sizeof(optBufSize));
		setsockopt(m_pWIFILocalSocket->m_hSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&optBufSize, sizeof(optBufSize));


		bool optKeepAlive=false;
		setsockopt(m_pWIFIConnectionSocket->m_hSocket, SOL_SOCKET, SO_KEEPALIVE, (const char*)&optKeepAlive, sizeof(optKeepAlive));
		setsockopt(m_pWIFILocalSocket->m_hSocket, SOL_SOCKET, SO_KEEPALIVE, (const char*)&optKeepAlive, sizeof(optKeepAlive));

		bool optNoDelay = true;
		setsockopt(m_pWIFIConnectionSocket->m_hSocket, SOL_SOCKET, TCP_NODELAY, (const char*)&optNoDelay, sizeof(optNoDelay));
		setsockopt(m_pWIFILocalSocket->m_hSocket, SOL_SOCKET, TCP_NODELAY, (const char*)&optNoDelay, sizeof(optNoDelay));

		if (m_pWIFILocalSocket->Listen() == FALSE)
		{
			AfxMessageBox(L"Socket listen failed");
			return false;
		}

		m_pLocalSocket = m_pWIFILocalSocket;
		m_pConnectionSocket = m_pWIFIConnectionSocket;
		return true;
}

bool CRemote::CreateServerSocket(){
	bool bRet=false;
	if(m_conType==CT_TCPIP){
		bRet = OpenWifiSockets();
	} else if(m_conType == CT_BLUETOOTH){
		bRet = StartBluetoothServer();//If the bluetooth server already exists, this will put the server in "Listen" mode (waiting for client connection).
	} else {
		CString errstr = L"CRemote::CreateServerSocket(): unknown connection type.";
		AfxMessageBox(errstr);
		bRet = false;
	}
	if(bRet){
		SetInitialized(true);
	}
	return bRet;
}


bool CRemote::WaitForKillEvent(int time_ms){
	if(WaitForSingleObject(m_hKillEvent, time_ms) == WAIT_OBJECT_0) {
			ReleaseSemaphore(m_hKillEvent,1,NULL);
			return true;
	}
	return false;
}

bool CRemote::Listen(){
	SetOpen(true);

	CloseHandle(m_sendClosed);
	CloseHandle(m_hKillEvent);
	m_sendClosed = CreateSemaphore(NULL,1,1,NULL);
	m_hKillEvent = CreateSemaphore(NULL,0,1,NULL);

	m_hSendQueueListenThread = AfxBeginThread(SendQueueListenThread, m_pParam);
	if(m_hSendQueueListenThread == NULL) return false;
	SetThreadPriority(m_hSendQueueListenThread, THREAD_PRIORITY_NORMAL);

	SetServerOpen();

	return true;
}

//Add multiple messaged to queue
void CRemote::Send(std::list<CRMessage*> mlist){
	m_CritSection.Lock();
	for(std::list<CRMessage*>::iterator it = mlist.begin(); it!=mlist.end(); it++){
			Send(*it);
		}
	m_CritSection.Unlock();
}

//Adds  message to the queue

void CRemote::Send(CRMessage* msg){
	m_CritSection.Lock();
	m_pSendQueue->offer(msg);  //m_pSendQueue inherits from CConcurrentQueue and is therefore thread-safe, so no locking is required.
	m_CritSection.Unlock();
}


//Sends message to socket
//void CRemote::Send(CRMessage* msg){
//	m_CritSection.Lock();
//	if(msg != NULL){
//		msg->Pack();
//		msg->Send(m_pConnectionSocket);
//		delete msg;
//	}
//	(CMainFrame*)AfxGetApp()->m_pMainWnd->PostMessage(REMOTE_RMESSAGE_SEND);
//	m_CritSection.Unlock();
//}


/*
//Write the queued messages to the socket
void CRemote::Send(){
	m_CritSection.Lock();
	int poll_timeout_ms=0;
	CRMessage* pMess;
	do{
		pMess=m_pSendQueue->poll(poll_timeout_ms); //Get the next message in the queue	
		if(WaitForKillEvent(0)) break; 
		if(pMess){
			pMess->Pack(); 
			pMess->Send(m_pConnectionSocket);
			delete pMess;
		}
	}while(pMess);
	ReleaseSemaphore(CRemote::m_sendSemaphore,1,NULL); //Ready to send again.
	m_CritSection.Unlock();
}
*/

//Write the message to the socket
void CRemote::Send(){
	m_CritSection.Lock();
	bool bClose=false;
	int retval;
	if(m_sendMess){
		m_sendMess->Pack();
	if(m_sendMess->CompareCommand("CLSE")){
			if(m_sendMess->CompareDestination("REMT")){
				bClose=true;
			}
		}
	retval =  m_sendMess->Send(m_pConnectionSocket);
	
		delete m_sendMess;
		m_sendMess = NULL;
	}
	if(bClose || (retval==SOCKET_ERROR)){
		OnSocketClosed();
	}
	ReleaseSemaphore(CRemote::m_sendSemaphore,1,NULL); //Release the send lock.
	m_CritSection.Unlock();
}

void CRemote::Receive(){	
	m_CritSection.Lock();
	//Pass the kill event handle to the message so it can interrupt reading at any time.
	m_recMess->Read(m_pConnectionSocket, m_hKillEvent);
	std::list<CRMessage*> mlist = m_pMessageHandler->HandleMessage(m_recMess);//Append to the list of send messages.
	Send(mlist);
	m_CritSection.Unlock();
}


//A thread to listen to the message handler and send a new message when it becomes available.
UINT CRemote::SendQueueListenThread(LPVOID pParam) {	
	THREADSTRUCT* ts = (THREADSTRUCT*)pParam;
	WaitForSingleObject(m_sendClosed, 1000);
    
	//Pointer to this CRemote obejct.
	CRemote* pThis = ts->_this;

	CRMessage* pMess=NULL;
	int poll_timeout_ms=100;
	while(true){
		pMess = pThis->m_pSendQueue->poll(poll_timeout_ms);
		if(pMess){
		//	pMess->Pack();
			while(WaitForSingleObject(pThis->m_sendSemaphore, poll_timeout_ms) != WAIT_OBJECT_0){ //Wait for the previous send to complete.
				if(pThis->WaitForKillEvent()) break;
			}
			pThis->m_sendMess = pMess; //Pass the message pointer to Remote.
			CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			if(pFrame){
				pFrame->PostMessage(REMOTE_RMESSAGE_SEND); //Send the message in the main thread.
			} 
			
		} else {
			if(pThis->WaitForKillEvent()) break;
		}
	}
	ReleaseSemaphore(CRemote::m_sendClosed,1,NULL);
	return 0;
}

bool CRemote::Restart(){
	if(CloseThreads()){
		return StartServer();
	} else {
		return false;
	}
}

bool CRemote::OnSocketClosed(){
	m_CritSection.Lock();
	bool result=false;
	if(GetServerOpen()){
		SetServerClosed();
		if(m_bRestart){
			result=Restart();
		} else {
			result=CloseThreads();
		}
	} else{
		result=false;
	}
	clearSendQueue(10);
	UpdateDialog();
	m_CritSection.Unlock();
	return result;
}


bool CRemote::clearSendQueue(int iTimeout_ms){
	m_CritSection.Lock();
	bool retval = false;
	CRMessage* pMess = NULL;
	while(true){
		pMess=m_pSendQueue->poll(iTimeout_ms);
		if(pMess) {
			delete pMess;
			pMess=NULL;
		} else {
			break;
		}
	}
	if(m_pSendQueue->size()>0) {
		retval=false;
	} else {
		retval=true;
	}
	m_CritSection.Unlock();
	return retval;
}

bool CRemote::RestartThreads(){
	if(m_conType == CT_BLUETOOTH){
		if(m_pBTServer->Listen() != m_pBTServer->iCxnSuccess()){
			return false;
		}
		m_pConnectionSocket = (CRClientSocket*)((CRServerSocket*)m_pLocalSocket)->GetClientSocket();
	} else { //TODO test wifi reconnection.
		//DeleteSockets();
		if(!CreateServerSocket()) return false;
	}
	return Listen();
}


bool CRemote::Close(){
	if(CloseThreads()){
		return CloseSockets();
	} else {
		return false;
	}
}

bool CRemote::GetClosing(){
	m_CritSection.Lock();
	return m_bClosing;
	m_CritSection.Unlock();
}

void CRemote::SetClosing(bool val){
	m_CritSection.Lock();
	m_bClosing=val;
	m_CritSection.Unlock();
}


bool CRemote::CloseClientSocket(){
	if(m_pConnectionSocket) m_pConnectionSocket->Close();
	return true;
}

bool CRemote::CloseThreads(){
		bool retval = true;
		ReleaseSemaphore(m_hKillEvent,1,NULL);
		SetOpen(false);
		SetConnected(false);
		if(!WaitForThreadsToClose(1000)){
			retval=false;		
		}
		if(m_pConnectionSocket){m_pConnectionSocket->Close();}
		if(m_conType == CT_BLUETOOTH){
			if(m_pBTServer) m_pBTServer->Close();
		}
		return retval;
}


bool CRemote::HasThreadExited(CWinThread* thread){
	DWORD exitCode=NULL;
	GetExitCodeThread(thread->m_hThread, &exitCode);
	if(exitCode == STILL_ACTIVE){
		return false;
	}
	return true;
}

bool CRemote::WaitForThreadsToClose(int timeout){
	bool result = true;
	CString errStr;
	
	if(WaitForSingleObject(m_sendClosed, timeout) != WAIT_OBJECT_0){
	errStr.Format(L"=CRITICAL= CRemote::WaitForThreadsToClose(int timeout) - unable to close SendQueueListenThread - timeout = %d ms. WSAGetLastError[%d]", timeout, WSAGetLastError());
		AfxMessageBox(errStr);
		result = false;
	} 
	ReleaseSemaphore(m_sendClosed, 1, NULL);
	
	return result;
}


bool CRemote::CloseSockets(){
	
		if(m_pConnectionSocket){
			m_pConnectionSocket->Close();
		}
		if(m_pLocalSocket) {
			m_pLocalSocket->Close();
		}
		
	SetOpen(false);

	if(WaitForSingleObject(m_updateDlgSemaphore,1000) == WAIT_OBJECT_0){
		UpdateDialog();
	}

	SetInitialized(false);
	return true;
}

void CRemote::UpdateDialog(){
	CWnd* rp = GetRemotePage();
	if(rp!=NULL){ 
		rp->PostMessage(REMOTE_WM_UPDATECONNECTIONSTATUS,0,0);
		ReleaseSemaphore(m_updateDlgSemaphore,1,NULL);	
	}
}

void CRemote::DeleteServers(){
	if(m_pBTServer){
			m_pBTServer->Close();
			delete m_pBTServer;
			m_pBTServer=NULL;
			}
}

void CRemote::DeleteSockets(){
	delete m_pBTLocalSocket; 
	delete m_pWIFILocalSocket;
	//The following are shared pointers and already deleted by deleting BT and WIFI local sockets.
	m_pLocalSocket=NULL;
	m_pConnectionSocket=NULL;
	m_pBTConnectionSocket = NULL;
	m_pWIFIConnectionSocket = NULL;
}

void CRemote::SetNSent(int val){
	m_nSent=val;
	//A pointer to the main frame window.
	CMainFrame*  pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(WaitForSingleObject(m_updateDlgSemaphore,0) == WAIT_OBJECT_0){
		CWnd* rp = GetRemotePage();
		if(rp){
			rp->PostMessage(RMESSAGE_WM_REMOTEUPDATE,0,0);
			ReleaseSemaphore(m_updateDlgSemaphore,1,NULL);
		}
	}
}

int CRemote::GetNSent(){return m_nSent;}

//Query if connected to remote socket.
bool CRemote::GetConnected(){
	CSingleLock lock(&m_CritSection);
	lock.Lock();
	if(!m_pLocalSocket) return false;
	bool isCon = m_pLocalSocket->IsConnected();
	lock.Unlock();
	return isCon;
}

void CRemote::SetConnected(bool val){
	CSingleLock lock(&m_CritSection);
	lock.Lock();
	m_bConnected=val;
	lock.Unlock();
}

bool CRemote::GetOpen(){
	bool result;
	CSingleLock lock(&m_CritSection);
	lock.Lock();
	result=m_bOpen;
	lock.Unlock();
	return result;
}

void CRemote::SetOpen(bool val){
	CSingleLock lock(&m_CritSection);
	lock.Lock();
	m_bOpen=val;
	lock.Unlock();
	if (AfxGetApp()->m_pMainWnd) {
		AfxGetApp()->m_pMainWnd->PostMessage(REMOTE_PAGE_UPDATE_CONTROLS);
	}
}

CString CRemote::GetThisComputerName(){
	CString retval;
	if(m_pBTServer){
		retval = m_pBTServer->GetThisComputerName();
	} else {
		retval = "";
	}
	return retval;
}
