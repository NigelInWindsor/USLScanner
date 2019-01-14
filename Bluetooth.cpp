// Bluetooth.cpp: implementation of the CBluetooth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Bluetooth.h"
#include <ws2bth.h>
#include <winsock2.h>
#include <Ws2tcpip.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DEFINE_GUID(g_guidServiceClass, 0xb62c4e8d, 0x62cc, 0x404b, 0xbb, 0xbf, 0xbf, 0x3e, 0x3b, 0xbb, 0x13, 0x74);

extern ULONG NameToBthAddr(const wchar_t *pszRemoteName, BTH_ADDR * pRemoteBtAddr);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
#include <winsock2.h>
SOCKADDR_BTH sockAddr;
SOCKET btSocket;
int error;

   btSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
   memset (&sockAddr, 0, sizeof(sockAddr));
   sockAddr.addressFamily = AF_BTH;
   sockAddr.serviceClassId = RFCOMM_PROTOCOL_UUID;
   sockAddr.port = BT_PORT_ANY;
   sockAddr.btAddr = <your bluetooth address>
   error = connect(btSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

  unsigned char *p;
ULONGLONG ullAddr;

   p = (unsigned char *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr; // point to addr
   memcpy(&ullAddr, &p[2], 8); // copy Bluetooth address of device we found
*/

CBluetooth::CBluetooth()
{
	m_bRecThreadEnabled = false;
	m_socket=INVALID_SOCKET;
	m_nAvailableDevicesSize = 0;
	RefreshAvailableDevices();

	m_PktArray = NULL;
	m_nPktL = 0;

}

CBluetooth::~CBluetooth()
{
	m_bRecThreadEnabled = false;
	closesocket(m_socket);

	for(int ii=0;ii<m_nPktL;ii++) {
		m_PktArray[ii].Zero();
	}
	SAFE_DELETE_ARRAY( m_PktArray );

}

bool CBluetooth::Connect(CWnd* pParent,bool bServer, long fn(CWnd* pParent, int nDataSize, char* Data))
{
	m_bServer = bServer;
	m_bRecThreadEnabled = TRUE;
	m_callback = fn;
	m_pParent = pParent;

	pRecThread = AfxBeginThread(BluetoothReceiveThread,this,THREAD_PRIORITY_NORMAL,0,NULL);

	return true;
}

bool CBluetooth::Connected()
{
	if(m_socket==INVALID_SOCKET)
		return false;
	
	return true;
}

bool CBluetooth::Disconnect()
{
	if(Connected() == false) return true;

	if(m_bRecThreadEnabled == true) {
		m_bRecThreadEnabled = false;
		WaitForSingleObject(pRecThread,INFINITE);
	}
	return true;

}

bool CBluetooth::SendMessage(int nDataSize, char *Data)
{

	//if connection not established return
	if(Connected()==false)
		return false;


	send(m_socket,Data,nDataSize,0);
	AddPkt(PKT_TX, nDataSize, Data);

	return true;

}

void CBluetooth::SendMessage(CString Buff)
{
	int ii;

	if(Buff.GetLength() > 0) {
		char *cArray = new char[Buff.GetLength() + 1];

		for(ii=0;ii<Buff.GetLength();ii++) {
			cArray[ii] = (char)Buff.GetAt(ii);
		}
		cArray[ii++]=0;

		SendMessage(ii,cArray);

		delete cArray;
	}
}
void CBluetooth::SendMessage(CString Buff,int nLength,char *pData, CString strEOL)
{
	int	ii,jj;
	CString strLength;

	if(Buff.GetLength() > 0) {
		strLength.Format(L"<%d>",nLength);

		char *cArray = new char[Buff.GetLength() + strLength.GetLength() + strEOL.GetLength() + nLength + 1];

		for(ii=0;ii<Buff.GetLength();ii++) 	cArray[ii] = (char)Buff.GetAt(ii);
		for(ii,jj=0;jj<strLength.GetLength();ii++,jj++) cArray[ii] = (char)strLength.GetAt(jj);

		CopyMemory(&cArray[ii],pData,nLength);
		ii+=nLength;

		for(ii,jj=0;jj<strEOL.GetLength();ii++,jj++) cArray[ii] = (char)strEOL.GetAt(jj);
		
		SendMessage(ii,cArray);

		delete cArray;
	}

}


#define DATA_BUFSIZE 4096


UINT BluetoothReceiveThread (LPVOID lpvoid)
{
	CString Buff;
	CBluetooth* pParent = (CBluetooth*) lpvoid;
	char temp[PACKET_SIZE];
	wchar_t m_szRemoteName[BTH_MAX_NAME_SIZE + 1] = {_T("Nigel's 3rd Kindle")};

    struct addrinfo *result = NULL, *ptr = NULL;
    WSAOVERLAPPED RecvOverlapped;
    SOCKET ConnSocket = INVALID_SOCKET;
    WSABUF DataBuf;
    DWORD RecvBytes, Flags;
    char buffer[DATA_BUFSIZE];

    int err = 0;
    int rc;

	GUID UUID;
	UUID.Data1 = 0x3606f360;
	UUID.Data2 = 0xe4df;
	UUID.Data3 = 0x11e0;
	UUID.Data4[0] = 0x95;
	UUID.Data4[1] = 0x72;
	UUID.Data4[2] = 0x08;
	UUID.Data4[3] = 0x00;
	UUID.Data4[4] = 0x20;
	UUID.Data4[5] = 0x0c;
	UUID.Data4[6] = 0x9a;
	UUID.Data4[7] = 0x66;

start:

	SOCKET server,client;
	WSADATA wsaData;

	SOCKADDR_BTH sockAddr;
	int error;
    WSAQUERYSET     wsaQuerySet = {0};
    wchar_t         szThisComputerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD           dwLenComputerName = MAX_COMPUTERNAME_LENGTH + 1;
    size_t          cbInstanceNameSize = 0;


//	ZeroMemory(m_szRemoteName, sizeof m_szRemoteName);
//	for(ii=0;ii<pParent->m_BTDeviceNames[theApp.m_LastSettings.nBluetoothDeviceNumber % 32].GetLength();ii++) {
//		m_szRemoteName[ii]=pParent->m_BTDeviceNames[theApp.m_LastSettings.nBluetoothDeviceNumber % 32].GetAt(ii);
//	};

	int wsaret=WSAStartup(0x202,&wsaData);

	if(wsaret!=0)
		 return 0;


	if(pParent->Connected()==false) {
		if(pParent->m_bServer) { 	//if Server, Start listening for connection

	        if ( !GetComputerName(szThisComputerName, &dwLenComputerName) ) {
				return 0;
	        }


			server = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
			if(server==INVALID_SOCKET)
				return 0;

			WSAPROTOCOL_INFO protocolInfo;
			int nProtocolInfoSize = sizeof(protocolInfo);
			if(getsockopt(server, SOL_SOCKET, SO_PROTOCOL_INFO, (char*)&protocolInfo, &nProtocolInfoSize) != 0) {
				closesocket(server);
				goto start;
			}


			ZeroMemory(&sockAddr,sizeof(sockAddr));
			sockAddr.addressFamily = AF_BTH;
			sockAddr.port = BT_PORT_ANY;
			sockAddr.serviceClassId = GUID_NULL;
			sockAddr.serviceClassId = UUID;

			if(bind(server,(sockaddr*)&sockAddr,sizeof(sockAddr))!=0) {
				closesocket(server);
				goto start;
			}

			int length = sizeof(SOCKADDR_BTH) ;
			if(0 != getsockname(server,(sockaddr*)&sockAddr,&length)) {
				closesocket(server);
				return 0;
			}
    

			if(listen(server,2)!=0) {
				closesocket(server);
				goto start;
			}


			SOCKADDR_BTH sa2;
			int nSize=sizeof(sa2);

			pParent->m_socket=accept(server,(SOCKADDR*)&sa2, &nSize);
			closesocket(server);
				
		}else{ 	//if client, Start polling ip address


			ZeroMemory(&sockAddr,sizeof(sockAddr));
			if(NameToBthAddr(m_szRemoteName, &sockAddr.btAddr) ) {
				sockAddr.addressFamily = AF_BTH;
	//			sockAddr.serviceClassId = RFCOMM_PROTOCOL_UUID;
				sockAddr.serviceClassId = UUID;
				sockAddr.port = BT_PORT_ANY;
				sockAddr.port = 0;

				client = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
				if(client==INVALID_SOCKET)
					return 0;

				if(error = connect(client, (SOCKADDR*)&sockAddr, sizeof(sockAddr))) {
					closesocket(client);
					pParent->m_socket=INVALID_SOCKET;
					Sleep(1000);
					goto start;
				} else {
					pParent->m_socket = client;
				}
			} else {
				goto start;
			}
		}
	}
  

	bool bOverlapped = false;

	if(bOverlapped == true) {

		ZeroMemory((PVOID) & RecvOverlapped, sizeof (WSAOVERLAPPED));

	    RecvOverlapped.hEvent = WSACreateEvent();
		if (RecvOverlapped.hEvent == NULL) {
			closesocket(pParent->m_socket);
			pParent->m_socket = INVALID_SOCKET;
			goto start;
		}

	    DataBuf.len = DATA_BUFSIZE;
	    DataBuf.buf = buffer;

	    while (pParent->m_bRecThreadEnabled) {

			Flags = 0;
			rc = WSARecv(pParent->m_socket, &DataBuf, 1, &RecvBytes, &Flags, &RecvOverlapped, NULL);
			if ((rc == SOCKET_ERROR) && (WSA_IO_PENDING != (err = WSAGetLastError()))) {
				TRACE1("WSARecv failed with error: %d\n", err);
			} else {

				rc = WSAWaitForMultipleEvents(1, &RecvOverlapped.hEvent, TRUE, INFINITE, TRUE);
				if (rc == WSA_WAIT_FAILED) {
					TRACE1("WSAWaitForMultipleEvents failed with error: %d\n", WSAGetLastError());
				} else {

					rc = WSAGetOverlappedResult(ConnSocket, &RecvOverlapped, &RecvBytes, FALSE, &Flags);
					if (rc == FALSE) {
						TRACE1("WSARecv operation failed with error: %d\n", WSAGetLastError());
					} else {

						TRACE1("Read %d bytes\n", RecvBytes);

						WSAResetEvent(RecvOverlapped.hEvent);
					}
				}
			}
		}

		WSACloseEvent(RecvOverlapped.hEvent);
		closesocket(pParent->m_socket);
		pParent->m_socket=INVALID_SOCKET;
		WSACleanup();

	} else {
		while(pParent->m_bRecThreadEnabled){
			int n=recv(pParent->m_socket,temp,PACKET_SIZE,0);

				
			if(!(n==SOCKET_ERROR || n==0))	{
				pParent->AddPkt(PKT_RX, n, temp);
				pParent->m_callback(pParent->m_pParent,n,temp);
			}

			if(n==SOCKET_ERROR) { //Socket Error
				closesocket(pParent->m_socket);
				pParent->m_socket = INVALID_SOCKET;
				WSACleanup();
				goto start;
			}

			if(n==0) { //Socet closed Gracefully
				closesocket(pParent->m_socket);
				pParent->m_socket = INVALID_SOCKET;
				WSACleanup();
				goto start;
			}
		}

		closesocket(pParent->m_socket);
		pParent->m_socket=INVALID_SOCKET;
		WSACleanup();
	}


	return err;

}


ULONG NameToBthAddr(const wchar_t *pszRemoteName, BTH_ADDR * pRemoteBtAddr)
{
	INT          iResult = 0, iRetryCount = 0;
	BOOL         bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
    ULONG        ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
    HANDLE       hLookup = 0;
    PWSAQUERYSET pWSAQuerySet = NULL;
	CString Buff;

    if ( ( pWSAQuerySet = (PWSAQUERYSET) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulPQSSize) )  == NULL) {
//        printf("!ERROR! | Unable to allocate memory for WSAQUERYSET!\n");
        goto CleanupAndExit;
    }
 

    for (iRetryCount = 0; !bRemoteDeviceFound && (iRetryCount < CXN_MAX_INQUIRY_RETRY); iRetryCount++)  {
        ulFlags = LUP_CONTAINERS;
        ulFlags |= LUP_RETURN_NAME;
        ulFlags |= LUP_RETURN_ADDR;
        if ( iRetryCount == 0)  {
//            printf("*INFO* | Inquiring device from cache...\n");
        }  else {
            ulFlags |= LUP_FLUSHCACHE;
//            printf("*INFO* | Unable to find device.  Waiting for %d seconds before re-inquiry...\n", CXN_DELAY_NEXT_INQUIRY);
  //          printf("I am sleeping for a while...\n");
            Sleep(CXN_DELAY_NEXT_INQUIRY * 1000);
    //        printf("*INFO* | Inquiring device ...\n");
        }
 
        iResult = 0;
        hLookup = 0;
        bContinueLookup = FALSE;
        ZeroMemory(pWSAQuerySet, ulPQSSize);
        pWSAQuerySet->dwNameSpace = NS_BTH;
        pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
        iResult = WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);
        if ( ( iResult == NO_ERROR) && (hLookup != NULL) ) {
//            printf("WSALookupServiceBegin() is fine!\n");
            bContinueLookup = TRUE;
        }
        else if ( 0 < iRetryCount )
        {
//            printf("=CRITICAL= | WSALookupServiceBegin() failed with error code %d, Error = %d\n", iResult, WSAGetLastError());
            goto CleanupAndExit;
		}
 

        while ( bContinueLookup )  {

            if ( WSALookupServiceNext(hLookup, ulFlags, &ulPQSSize, pWSAQuerySet) == NO_ERROR) {
//                printf("WSALookupServiceNext() is OK lol!\n");

                if ( ( pWSAQuerySet->lpszServiceInstanceName != NULL ))  {
					  if(wcsncmp(pWSAQuerySet->lpszServiceInstanceName, pszRemoteName, wcslen(pWSAQuerySet->lpszServiceInstanceName)) == 0) {
	                    CopyMemory(pRemoteBtAddr, &((PSOCKADDR_BTH) pWSAQuerySet->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr, sizeof(*pRemoteBtAddr));
	                    bRemoteDeviceFound = TRUE;
	                    bContinueLookup = FALSE;
					  }
                }
            } else {
                if ( ( iResult = WSAGetLastError() ) == WSA_E_NO_MORE) {
//                    printf("No more device found...\n");
                    bContinueLookup = FALSE;
                }
                else if ( iResult == WSAEFAULT)
                {
                    HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
                    pWSAQuerySet = NULL;
                    if ( NULL == ( pWSAQuerySet = (PWSAQUERYSET) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulPQSSize) ) ) {
//                        printf("!ERROR! | Unable to allocate memory for WSAQERYSET...\n");
                        bContinueLookup = FALSE;
                    }
                    else
                        Buff.Format(L"HeapAlloc() for WSAQERYSET is OK!\n");
                }
                else
                {
                    Buff.Format(Buff,L"=CRITICAL= | WSALookupServiceNext() failed with error code %d\n", iResult);
                    bContinueLookup = FALSE;

                }

            }

        }
        WSALookupServiceEnd(hLookup);

    }

 

CleanupAndExit:

    if ( NULL != pWSAQuerySet ) {
        HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
        pWSAQuerySet = NULL;
    }

    if ( bRemoteDeviceFound ) {
        return(1);
    } else {
        return(0);
    }

}

int	CBluetooth::getAvailableDevicesSize()
{

	return m_nAvailableDevicesSize;
}


CString CBluetooth::getDeviceName(int nIndex)
{

	return m_BTDeviceNames[nIndex];
}


void CBluetooth::RefreshAvailableDevices()
{
	INT         iResult = 0, iRetryCount = 0;
	bool        bContinueLookup = false, bRemoteDeviceFound = false;
    ULONG       ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
    HANDLE      hLookup = 0;


	ulFlags = LUP_CONTAINERS | LUP_RETURN_NAME | LUP_RETURN_ADDR;
	iResult = 0;
	hLookup = 0;

	ZeroMemory(m_WSAQuerySet, sizeof m_WSAQuerySet);
	m_WSAQuerySet[0].dwNameSpace = NS_BTH;
	m_WSAQuerySet[0].dwSize = sizeof(WSAQUERYSET);

	iResult = WSALookupServiceBegin(&m_WSAQuerySet[0], ulFlags, &hLookup);
	if ( ( iResult == NO_ERROR) && (hLookup != NULL) ) {

		m_nAvailableDevicesSize = 0;
		bContinueLookup = true;
		while(bContinueLookup == true) {
			ZeroMemory(m_WSAQuerySet, sizeof m_WSAQuerySet);
			m_WSAQuerySet[0].dwNameSpace = NS_BTH;
			m_WSAQuerySet[0].dwSize = sizeof(WSAQUERYSET);
			iResult = WSALookupServiceNext(hLookup, ulFlags, &ulPQSSize, &m_WSAQuerySet[0]);

			if(iResult == NO_ERROR) {
				if(wcslen(m_WSAQuerySet[0].lpszServiceInstanceName) > 0) {
					m_BTDeviceNames[m_nAvailableDevicesSize++].Format(L"%s",m_WSAQuerySet[0].lpszServiceInstanceName);
				}

			} else {
                if ( ( iResult = WSAGetLastError() ) == (WSA_E_NO_MORE | WSAEFAULT)) {
                    bContinueLookup = false;
                }
            }

        }
        WSALookupServiceEnd(hLookup);

    }

}

void CBluetooth::AddPkt(bool bDirection, int nDataSize, char *pData)
{

	int nn=m_nPktL;
	m_nPktL++;

	if(m_nPktL==1) {
		m_PktArray = new BLUETOOTHPKT[m_nPktL];
	} else {
		m_PktArray = (BLUETOOTHPKT *)realloc(m_PktArray,m_nPktL * (sizeof BLUETOOTHPKT));
	};
	m_PktArray[nn].Add(bDirection, nDataSize, pData);

}

int CBluetooth::GetPktCount()
{
	return m_nPktL;
}

CString CBluetooth::GetPktTxRx(int nIndex)
{
	CString Buff;

	if(nIndex<m_nPktL) {
		if(m_PktArray[nIndex].GetTxRx() == PKT_RX) {
			Buff.LoadString(IDS_Rx);
		} else {
			Buff.LoadString(IDS_Tx);
		}
	}
	return Buff;
}

CString CBluetooth::FormatPktTime(int nIndex)
{
	CString Buff;

	if(nIndex<m_nPktL) {
		m_PktArray[nIndex].FormatTime(&Buff);
	}
	return Buff;

}

CString CBluetooth::GetPktDataSize(int nIndex)
{
	CString Buff;

	if(nIndex<m_nPktL) {
		Buff.Format(L"%d",m_PktArray[nIndex].GetDataSize());
	}
	return Buff;
}

CString CBluetooth::FormatPktData(int nIndex)
{
	CString Buff;

	if(nIndex<m_nPktL) {
		m_PktArray[nIndex].FormatData(&Buff);
	}
	return Buff;
}
