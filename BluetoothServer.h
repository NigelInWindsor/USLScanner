#pragma once
#include "Bluetooth.h"
class CBluetoothServer :
	public CBluetooth
{
public:
	CBluetoothServer();
	~CBluetoothServer();

	//Starts the server.
	ULONG Start();

	CString GetMessageData();
	void Close();

	


private:
	LPCSADDR_INFO   m_lpCSAddrInfo = NULL;
	SOCKADDR_BTH    m_SockAddrBthLocal = { 0 };
	WSAQUERYSET     m_wsaQuerySet = { 0 };

	SOCKET          m_ClientSocket = INVALID_SOCKET;
	wchar_t         m_szThisComputerName[MAX_COMPUTERNAME_LENGTH + 1];
	wchar_t *       m_pszInstanceName = NULL;
	
	size_t          m_cbInstanceNameSize = 0;


	
	

	ULONG AllocateAddressInfoMemory();
	ULONG ComputerName();
	ULONG OpenBluetoothSocket();
	ULONG SetSocketBufferLength();
	ULONG BindToBluetoothSocket();
	ULONG GetSocketName();
	ULONG AdvertiseSocket();
	ULONG AllocateInstanceInfoMemory();
	ULONG SetService();
	ULONG Listen();
	ULONG Accept();


	void InitialiseVariables();

	
}; 

