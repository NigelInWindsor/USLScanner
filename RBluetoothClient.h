#ifndef _RBLUETOOTHCLIENT_H_863492134061234096123046
#define _RBLUETOOTHCLIENT_H_863492134061234096123046

#include "RBluetooth.h"
#define SIZE_OF_MESSAGE 1000
class CRBluetoothClient :
	public CRBluetooth
{
public:
	CRBluetoothClient();
	~CRBluetoothClient();

	ULONG ConnectToServer();
	void Stop();


	//wchar_t         *pszData = NULL;
	SOCKADDR_BTH    m_SockAddrBthServer;


private:
	

	virtual void InitialiseVariables();
	void SetAddressFamily();
	void OpenSocket();
	void ConnectSocketToRemote();
	void CloseConnection();
};

#endif //#ifndef _RBLUETOOTHCLIENT_H_863492134061234096123046