#include "stdafx.h"
#include "RBluetoothServer.h"
#include <initguid.h>
#include <WinSock2.h>
#include <strsafe.h>

DEFINE_GUID(rbts_guidServiceClass, 0xb62c4e8d, 0x62cc, 0x404b, 0xbb, 0xbf, 0xbf, 0x3e, 0x3b, 0xbb, 0x13, 0x74);


CRBluetoothServer::CRBluetoothServer()
{
	
	CRBluetooth::CRBluetooth();
	m_lpCSAddrInfo = NULL;
    m_ClientSocket = INVALID_SOCKET;
    m_pszInstanceName = NULL;
	m_cbInstanceNameSize = 0;
	m_szThisComputerName[MAX_COMPUTERNAME_LENGTH + 1];
	m_strThisComputerName="";
}


CRBluetoothServer::~CRBluetoothServer()
{

}


CString CRBluetoothServer::GetMessageData() {
	m_bMessageReady = false;
	return  m_sMessageData; 
}

void CRBluetoothServer::Close()
{
}

void CRBluetoothServer::InitialiseVariables() {
	CRBluetooth::InitialiseVariables();
	m_ulRetCode = m_iCxnSuccess;
	
	m_bMessageReady = false;

	m_pszDataBuffer = NULL;
	m_pszDataBufferIndex = NULL;
	m_pszInstanceName = NULL;
	m_LocalSocket = INVALID_SOCKET;
	m_ClientSocket = INVALID_SOCKET;
	//The "send" socket for the server is the client socket.
	m_pSendReceiveSocket = &m_ClientSocket;;
	m_lpCSAddrInfo = NULL;
	m_cbInstanceNameSize = 0;
}

void CRBluetoothServer::ReinitialiseVariables() {
	CRBluetooth::ReinitialiseVariables();
	m_ulRetCode = m_iCxnSuccess;
	m_bMessageReady = false;
	m_LocalSocket = INVALID_SOCKET;
	m_ClientSocket = INVALID_SOCKET;
	//The "send" socket for the server is the client socket.
	m_pSendReceiveSocket = &m_ClientSocket;;
}

//Starts the server.
ULONG CRBluetoothServer::Start() {
	InitialiseVariables();
	if(StatusOK()) AllocateAddressInfoMemory();
	if(StatusOK()) ComputerName();	
	if(StatusOK()) OpenBluetoothSocket();
	if (StatusOK()) SetSocketReceiveBufferLength();
	if (StatusOK()) SetSocketSendBufferLength();
	if(StatusOK()) BindToBluetoothSocket();
	if(StatusOK()) GetSocketName();
	if(StatusOK()) AdvertiseSocket();
	if(StatusOK()) AllocateInstanceInfoMemory();
	if(StatusOK()) SetService();
	if (StatusOK()) Listen();
	return m_ulRetCode;
}

void CRBluetoothServer::Stop(){
	CRBluetooth::Stop();
}

ULONG CRBluetoothServer::Restart(){
	ReinitialiseVariables();
	if(StatusOK()) ComputerName();	
	if(StatusOK()) OpenBluetoothSocket();
	if (StatusOK()) SetSocketReceiveBufferLength();
	if (StatusOK()) SetSocketSendBufferLength();
	if(StatusOK()) BindToBluetoothSocket();
	if(StatusOK()) GetSocketName();
	if(StatusOK()) AdvertiseSocket();
	if(StatusOK()) AllocateInstanceInfoMemory();
	if(StatusOK()) SetService();
	if (StatusOK()) Listen();
	return m_ulRetCode;
}

void CRBluetoothServer::AllocateAddressInfoMemory(){
	m_heap1=GetProcessHeap();
	m_lpCSAddrInfo = (LPCSADDR_INFO)HeapAlloc(m_heap1,
		HEAP_ZERO_MEMORY,
		sizeof(CSADDR_INFO));
	if (NULL == m_lpCSAddrInfo) {
		CString errStr = L"CRBluetoothServer: !ERROR! | Unable to allocate memory for CSADDR_INFO";
		AfxMessageBox(errStr);
		SetError();
		return;
	}
	SetSuccess();
}

void CRBluetoothServer::ComputerName() {
		DWORD           dwLenComputerName = MAX_COMPUTERNAME_LENGTH + 1;
		if (!GetComputerName(m_szThisComputerName, &dwLenComputerName)) {
			CString errStr;
			errStr.Format(L"CRBluetoothServer: | GetComputerName() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
			AfxMessageBox(errStr);
			SetError();
			return;
		}
		m_strThisComputerName = m_szThisComputerName;
		SetSuccess();
}

CString CRBluetoothServer::GetThisComputerName(){
	return m_strThisComputerName;;
}

void CRBluetoothServer::OpenBluetoothSocket() {
	//
	// Open a bluetooth socket using RFCOMM protocol
	//
	OutputDebugString(L"Opening socket...");
		m_LocalSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
		if (INVALID_SOCKET == m_LocalSocket) {
			OutputDebugString(L"...failed to open socket.");
			CString errStr;
			errStr.Format(L"CRBluetoothServer: | socket() call failed. WSAGetLastError = [%d]\n", WSAGetLastError());
			AfxMessageBox(errStr);
			SetError();
			return;
		}
		OutputDebugString(L"...success opening socket.");
		SetSuccess();
}


void CRBluetoothServer::BindToBluetoothSocket(){

	//
	// Setting address family to AF_BTH indicates winsock2 to use Bluetooth port
	//
	USHORT addrFam = AF_BTH;
	ULONG port = BT_PORT_ANY;
	GUID guid = GUID_NULL;

	m_SockAddrBthLocal.addressFamily = AF_BTH;
	m_SockAddrBthLocal.port = port;
	m_SockAddrBthLocal.serviceClassId = guid;
	m_SockAddrBthLocal.btAddr = 0;

	//
	// bind() associates a local address and port combination
	// with the socket just created. This is most useful when
	// the application is a server that has a well-known port
	// that clients know about in advance.
	//
	OutputDebugString(L"Binding local socket to blutooth socket...");
	if (SOCKET_ERROR == bind(m_LocalSocket,
		(struct sockaddr *) &m_SockAddrBthLocal,
		sizeof(SOCKADDR_BTH))) {
#ifdef BLUETOOTHDEBUG
		OutputDebugString(L"Failed to bind local socket to blutooth socket");
		CString errStr;
		errStr.Format(L"CRBluetoothServer: | bind() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket, WSAGetLastError());
		AfxMessageBox(errStr);
#endif
		SetError();
		return;
	}
		OutputDebugString(L"...success binding local socket to bluetooth socket.");
		SetSuccess();
}

void CRBluetoothServer::GetSocketName() {
	int iAddrLen = sizeof(SOCKADDR_BTH);
	ULONG m_ulRetCode = getsockname(m_LocalSocket,
		(struct sockaddr *)&m_SockAddrBthLocal,
		&iAddrLen);
	if (SOCKET_ERROR == m_ulRetCode) {
		CString errStr;
		errStr.Format(L"CRBluetoothServer | getsockname() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket, WSAGetLastError());
		AfxMessageBox(errStr);
		SetError();
		return;
	}
	SetSuccess();
}

void CRBluetoothServer::AdvertiseSocket() {
	//
	// CSADDR_INFO
	//
	m_lpCSAddrInfo[0].LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
	m_lpCSAddrInfo[0].LocalAddr.lpSockaddr = (LPSOCKADDR)&m_SockAddrBthLocal;
	m_lpCSAddrInfo[0].RemoteAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
	m_lpCSAddrInfo[0].RemoteAddr.lpSockaddr = (LPSOCKADDR)&m_SockAddrBthLocal;
	m_lpCSAddrInfo[0].iSocketType = SOCK_STREAM;
	m_lpCSAddrInfo[0].iProtocol = BTHPROTO_RFCOMM;

	//
	// If we got an address, go ahead and advertise it.
	//
	ZeroMemory(&m_wsaQuerySet, sizeof(m_wsaQuerySet));
	m_wsaQuerySet.dwSize = sizeof(m_wsaQuerySet);
	m_wsaQuerySet.lpServiceClassId = (LPGUID)&rbts_guidServiceClass;

	//
	// Adding a byte to the size to account for the space in the
	// format string in the swprintf call. This will have to change if converted
	// to UNICODE
	//
	HRESULT         res;

	res = StringCchLength(m_szThisComputerName, sizeof(m_szThisComputerName), &m_cbInstanceNameSize);
//	res = wcslen(m_szThisComputerName);//, sizeof(m_szThisComputerName), &m_cbInstanceNameSize);
	if (FAILED(res)) {
		CString errStr = L"CRBluetoothServer: -FATAL- | ComputerName specified is too large";
		AfxMessageBox(errStr);
		SetError();
		return;
	}
	SetSuccess();
}

void CRBluetoothServer::AllocateInstanceInfoMemory() {
	m_heap2=GetProcessHeap();
	m_cbInstanceNameSize += sizeof(CXN_INSTANCE_STRING) + 1;
	m_pszInstanceName = (LPWSTR)HeapAlloc(m_heap2,
		HEAP_ZERO_MEMORY,
		m_cbInstanceNameSize);
	if (NULL == m_pszInstanceName) {
		CString errStr;
		errStr.Format(L"CRBluetoothServer: -FATAL- | HeapAlloc failed | out of memory | gle = [%d] \n", GetLastError());
		AfxMessageBox(errStr);
		SetError();
		return;
	}
	SetSuccess();
}

void CRBluetoothServer::SetService() {
	StringCbPrintf(m_pszInstanceName, m_cbInstanceNameSize, L"%s %s", m_szThisComputerName, CXN_INSTANCE_STRING);
	//swprintf(m_pszInstanceName, L"%s %s", m_szThisComputerName, CXN_INSTANCE_STRING);
	
	
	m_wsaQuerySet.lpszServiceInstanceName = m_pszInstanceName;
	m_wsaQuerySet.lpszComment = L"Example Service instance registered in the directory service through RnR";
	m_wsaQuerySet.dwNameSpace = NS_BTH;
	m_wsaQuerySet.dwNumberOfCsAddrs = 1;      // Must be 1.
	m_wsaQuerySet.lpcsaBuffer = m_lpCSAddrInfo; // Req'd.

												//
												// As long as we use a blocking accept(), we will have a race
												// between advertising the service and actually being ready to
												// accept connections.  If we use non-blocking accept, advertise
												// the service after accept has been called.
												//
	int result = WSASetService(&m_wsaQuerySet, RNRSERVICE_REGISTER, 0);
	if (SOCKET_ERROR == result) {
		CString errStr;
		errStr.Format(L"CRBluetoothServer: | WSASetService() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
		AfxMessageBox(errStr);
		SetError();
		return;
	}
	SetSuccess();
}

//Deregister the service.
void CRBluetoothServer::UnsetService(){
	if (SOCKET_ERROR == WSASetService(&m_wsaQuerySet, RNRSERVICE_DELETE, 0)) {
		CString errStr;
		errStr.Format(L"CRBluetoothServer: | socket() call failed. WSAGetLastError = [%d]\n", WSAGetLastError());
		AfxMessageBox(errStr);
		SetError();
		return;
	}
	SetSuccess();
}



//
// listen() call indicates winsock2 to listen on a given socket for any incoming connection.
//
ULONG CRBluetoothServer::Listen() {
	OutputDebugString(L"Instructing winsock to listen...");
	if (SOCKET_ERROR == listen(m_LocalSocket, m_iCxnDefaultListenBacklog)) {
		OutputDebugString(L"...listen failed.");
		CString errStr;
		errStr.Format(L"CRBluetoothServer: | listen() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket, WSAGetLastError());
		AfxMessageBox(errStr);
		SetError();
		return m_ulRetCode;
	}
	OutputDebugString(L"...listen success.");
	SetSuccess();
	return m_ulRetCode;
}

void CRBluetoothServer::Accept() {
	//for (int iCxnCount = 0;
	//	(m_iCxnSuccess == m_ulRetCode) && ((iCxnCount < m_iMaxCxnCycles) || (m_iMaxCxnCycles == 0));
	//	iCxnCount++) {

	wprintf(L"\n");

	//
	// accept() call indicates winsock2 to wait for any
	// incoming connection request from a remote socket.
	// If there are already some connection requests on the queue,
	// returns the handle to this newly created socket. This newly created
	// socket represents the actual connection that connects the two sockets.
	//

	OutputDebugString(L"Accepting connection....");
	m_ClientSocket = accept(m_LocalSocket, NULL, NULL);
//	m_ClientSocket = accept(m_LocalSocket);
	OutputDebugString(L"Connected.");
	if (INVALID_SOCKET == m_ClientSocket) {
		CString errStr;
		errStr.Format(L"CRBluetoothServer | accept() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
		AfxMessageBox(errStr);

		SetError();
		return;
		//break; // Break out of the for loop
	}

	//if (!ReadData()) break;
//}
	SetSuccess();
	//}
}

  




