#include "stdafx.h"
#include "RBluetoothClient.h"
#include <sstream>
#include <wchar.h>
#include <initguid.h>

DEFINE_GUID(rbtc_guidServiceClass, 0xb62c4e8d, 0x62cc, 0x404b, 0xbb, 0xbf, 0xbf, 0x3e, 0x3b, 0xbb, 0x13, 0x74);

CRBluetoothClient::CRBluetoothClient()
{
	CRBluetooth::CRBluetooth();
	m_SockAddrBthServer = m_pRemoteBtAddr;
}


CRBluetoothClient::~CRBluetoothClient()
{
}


ULONG CRBluetoothClient::ConnectToServer() {
	InitialiseVariables();
	if (StatusOK()) SetAddressFamily();
	if (StatusOK()) OpenSocket();
	if (StatusOK()) SetSocketReceiveBufferLength();
	if (StatusOK()) SetSocketSendBufferLength();
	if (StatusOK()) ConnectSocketToRemote();
	while (m_bOpen) {
		if (m_bMessageReady == false) {
			if (StatusOK()) ReadData();
		}		
		//if (StatusOK()) PrintData();
	}
//	if (StatusOK()) SendData();
//	if (StatusOK()) CloseConnection();
	
	return(m_ulRetCode);
}

void CRBluetoothClient::Stop()
{
	CloseConnection();
}


void CRBluetoothClient::InitialiseVariables()
{
	CRBluetooth::InitialiseVariables();
	m_bOpen = false;
	//CRBluetooth::InitialiseVariables();
	NameToBthAddr();
	ULONG ulRetCode = m_iCxnSuccess;
	//wchar_t         *m_pszData = NULL;
	//wcsncpy_s(m_pszData, m_sCxnTestDataString, SIZE_OF_MESSAGE);
	//Null terminate the string
	//m_pszData[sizeof(m_sCxnTestDataString)] = 0;
	m_LocalSocket = INVALID_SOCKET;
	m_pSendReceiveSocket = &m_LocalSocket;
	m_SockAddrBthServer = m_pRemoteBtAddr;
}

void CRBluetoothClient::SetAddressFamily()
{
		//
		// Setting address family to AF_BTH indicates winsock2 to use Bluetooth sockets
		// Port should be set to 0 if ServiceClassId is spesified.
		//
		m_SockAddrBthServer.addressFamily = AF_BTH;
		m_SockAddrBthServer.serviceClassId = rbtc_guidServiceClass;
		m_SockAddrBthServer.port = 0;

		//
		// Create a static data-string, which will be transferred to the remote
		// Bluetooth device
		//
		/*
		HRESULT         res;

		res = StringCbCopyN(m_pszData, sizeof(m_sCxnTestDataString), m_sCxnTestDataString, sizeof(m_sCxnTestDataString));
		if (FAILED(res)) {
		wprintf(L"=CRITICAL= | Creating a static data string failed\n");
		m_ulRetCode = m_iCxnError;
		}
		*/
		
}

void CRBluetoothClient::OpenSocket()
{
	//
	// Open a bluetooth socket using RFCOMM protocol
	//
	m_LocalSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (INVALID_SOCKET == m_LocalSocket) {
		wprintf(L"=CRITICAL= | socket() call failed. WSAGetLastError = [%d]\n", WSAGetLastError());
		m_ulRetCode = m_iCxnError;
		SetError();
	}
	SetSuccess();
}

void CRBluetoothClient::ConnectSocketToRemote()
{
	//
	// Connect the socket (pSocket) to a given remote socket represented by address (pServerAddr)
	//

	if (SOCKET_ERROR == connect(m_LocalSocket,
		(struct sockaddr *) &m_SockAddrBthServer,
		sizeof(SOCKADDR_BTH))) {
		std::wstringstream ss;
		ss << WSAGetLastError() << std::endl;
		AfxMessageBox(ss.str().c_str());
		wprintf(L"=CRITICAL= | connect() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
		m_ulRetCode = m_iCxnError;
		//break;
		SetError();
	}
	m_bOpen = true;
}


void CRBluetoothClient::CloseConnection()
{
	//
	// Close the socket
	//
	if (m_bOpen) {
		if (SOCKET_ERROR == closesocket(m_LocalSocket)) {
			wprintf(L"=CRITICAL= | closesocket() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)m_LocalSocket, WSAGetLastError());
			SetError();
			//	break;
		}
	}
	m_LocalSocket = INVALID_SOCKET;
	m_bOpen = false;

}
