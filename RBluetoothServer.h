#ifndef _RBLUETOOTHSERVER_H_987263498612345861203566
#define _RBLUETOOTHSERVER_H_987263498612345861203566

#include "RBluetooth.h"

class CRBluetoothServer :
	public CRBluetooth
{
public:
	CRBluetoothServer();
	~CRBluetoothServer();

	//Starts the server.
	ULONG Start();
	virtual void Stop();

	ULONG Restart();

	CString GetMessageData();
	void Close();

	ULONG Listen();

	CString GetThisComputerName();

private:

	
	LPCSADDR_INFO   m_lpCSAddrInfo;
	SOCKADDR_BTH    m_SockAddrBthLocal = { 0 };
	WSAQUERYSET     m_wsaQuerySet;

	SOCKET          m_ClientSocket;
	wchar_t         m_szThisComputerName[MAX_COMPUTERNAME_LENGTH + 1];
	CString         m_strThisComputerName;
	wchar_t *       m_pszInstanceName;
	


	size_t          m_cbInstanceNameSize;


	HANDLE m_heap1;
	HANDLE m_heap2;
	HANDLE m_heap3;

	void AllocateAddressInfoMemory();
	void ComputerName();
	void OpenBluetoothSocket();
	void SetSocketBufferLength();
	void BindToBluetoothSocket();
	void GetSocketName();
	void AdvertiseSocket();
	void AllocateInstanceInfoMemory();
	void SetService();
	void UnsetService();
	void Accept();


	void InitialiseVariables();
	void ReinitialiseVariables();

	
}; 

#endif //#ifndef _RBLUETOOTHSERVER_H_987263498612345861203566