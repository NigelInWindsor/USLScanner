// Ethernet.cpp: implementation of the CEthernet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Ethernet.h"
#include <afxsock.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define WINSOCK_DEPRECATED_NO_WARNINGS

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEthernet::CEthernet()
{
	m_connection=INVALID_SOCKET;
	m_pParent = NULL;
	m_PktArray = NULL;
	m_nPktL = 0;
}

CEthernet::~CEthernet()
{
	Disconnect();
	closesocket(m_connection);
	for(int ii=0;ii<m_nPktL;ii++) {
		m_PktArray[ii].Zero();
	}
	SAFE_DELETE_ARRAY( m_PktArray );
}

void CEthernet::Connect(CWnd* pParent,bool bServer, long fn(CWnd* pParent,int nDataSize, char* Data), int nRemoteIPAddrs, int nPort)
{
	m_bServer = bServer;
	m_nPort = nPort;
	m_callback = fn;
	m_nRemoteIP = nRemoteIPAddrs;
	m_pParent = pParent; 

	m_bRecThreadEnabled = TRUE;
	pRecThread = AfxBeginThread(ReceiveThread,this,THREAD_PRIORITY_NORMAL,0,NULL);
}

bool CEthernet::SendMessage(int nDataSize, char *Data)
{

	//if connection not established return
	if(Connected()==false)
		return false;


	send(m_connection,Data,nDataSize,0);
	AddPkt(PKT_TX, nDataSize, Data);

	return true;

}

void CEthernet::SendMessage(CString Buff)
{
	if(Buff.GetLength() > 0) {
		char *cArray = new char[Buff.GetLength() + 1];

		int ii;
		for(ii=0;ii<Buff.GetLength();ii++) {
			cArray[ii] = (char)Buff.GetAt(ii);
		}
		cArray[ii++]=0;

		SendMessage(ii,cArray);

		delete cArray;
	}
}
void CEthernet::SendMessage(CString Buff,int nLength,char *pData, CString strEOL)
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




UINT ReceiveThread (LPVOID lpvoid)
{
	CString Buff;
	CEthernet* pParent = (CEthernet*) lpvoid;
	char temp[PACKET_SIZE];

start:

	SOCKET server;
	WSADATA wsaData;
	sockaddr_in local;


	int wsaret=WSAStartup(0x101,&wsaData);

	if(wsaret!=0)
		 return 0;


	if(pParent->Connected()==false) {
		if(pParent->m_bServer) { 	//if Server, Start listening for connection

    
			local.sin_family=AF_INET; //Address family
			local.sin_addr.s_addr=INADDR_ANY; //Wild card IP address
			local.sin_port=htons((u_short)pParent->m_nPort); //port to use

			server=socket(AF_INET,SOCK_STREAM,0);

			if(server==INVALID_SOCKET)
				return 0;
    
			if(bind(server,(sockaddr*)&local,sizeof(local))!=0)
				return 0;
    
			if(listen(server,10)!=0)
				return 0;
    
			sockaddr_in from;
			int fromlen=sizeof(from);


			pParent->m_connection=accept(server,(struct sockaddr*)&from,&fromlen);
			closesocket(server);
				
		}else{ 	//if client, Start polling ip address

			pParent->m_connection=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); //TCP
			if(pParent->m_connection==INVALID_SOCKET)
				return 0;


			int j = sizeof(bool);
			setsockopt(pParent->m_connection,IPPROTO_TCP,TCP_NODELAY,(char*)&j,j); //dont concatenate packets

			sockaddr_in Server;
    
			Server.sin_family=AF_INET; 
			Server.sin_addr.s_addr=ntohl(pParent->m_nRemoteIP);
			Server.sin_port=htons((u_short)pParent->m_nPort);

			if(connect(pParent->m_connection,(sockaddr*)&Server,sizeof(Server))!=0) {
			   Sleep(1000);
			   goto start;
			}

		}
	}
  


	while(pParent->m_bRecThreadEnabled){
		int n=recv(pParent->m_connection,temp,PACKET_SIZE,0);

		
//		if(n!=PACKET_SIZE)
//			recv(pParent->m_connection,&temp[n],PACKET_SIZE-n,0);
			
		if(!(n==SOCKET_ERROR || n==0))	{
			pParent->AddPkt(PKT_RX, n, temp);
			pParent->m_callback(pParent->m_pParent,n,temp);
		}

		if(n==SOCKET_ERROR) { //Socket Error
			closesocket(pParent->m_connection);
			pParent->m_connection = INVALID_SOCKET;
			WSACleanup();
			goto start;
		}

		if(n==0) { //Socet closed Gracefully
		    closesocket(pParent->m_connection);
			pParent->m_connection = INVALID_SOCKET;
			WSACleanup();
			goto start;
		}
	}


		
    closesocket(pParent->m_connection);


    WSACleanup();
	pParent->m_connection=INVALID_SOCKET;

    return 0;

}


bool CEthernet::Connected()
{
/*	char buf;

	int err = recv(m_connection,&buf,1,MSG_PEEK);
	
	if(err == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			return false;
	}
*/

	if(m_connection==INVALID_SOCKET)
		return false;
	
	return true;

}


bool CEthernet::Disconnect()
{
	
	m_bRecThreadEnabled = false;
	TRACE0("Ethernet Wait to disconnect\n");
	WaitForSingleObject(pRecThread,INFINITE);
	TRACE0("Ethernet wait succeded\n");

	return true;
}


bool CEthernet::GetAllIPAddress()
{
	CString strTemp;
	struct hostent *host;
	char str[256];

	struct in_addr *ptr; // To retrieve the IP Address 

	DWORD dwScope = RESOURCE_CONTEXT;
	NETRESOURCE *NetResource = NULL;
	HANDLE hEnum;
	WNetOpenEnum( dwScope, NULL, NULL, 
				 NULL, &hEnum );

	WSADATA wsaData;
	WSAStartup(MAKEWORD(1,1),&wsaData);

	if ( hEnum )
	{
		DWORD Count = 0xFFFFFFFF;
		DWORD BufferSize = 2048;
		LPVOID Buffer = new char[2048];
		WNetEnumResource( hEnum, &Count, 
			Buffer, &BufferSize );
		NetResource = (NETRESOURCE*)Buffer;

		char szHostName[200];
		unsigned int i;
		int nLoop = 0;
		for ( i = 0; 
			i < BufferSize/sizeof(NETRESOURCE); 
			i++, NetResource++ ,nLoop++)
		{
			if ( NetResource->dwUsage == 
				RESOURCEUSAGE_CONTAINER && 
				NetResource->dwType == 
				RESOURCETYPE_ANY )
			{
				if ( NetResource->lpRemoteName )
				{
					CString strFullName = 
						NetResource->lpRemoteName;
					if ( 0 == 
						strFullName.Left(2).Compare(L"\\\\") )   
						strFullName = 
							strFullName.Right(
								strFullName.GetLength()-2);

					gethostname( szHostName, strlen( szHostName ) );
					int ii;
					for(ii=0;ii<strFullName.GetLength();ii++) {
						str[ii] = (char)strFullName.GetAt(ii);
					}
					str[ii]=0;

					host = gethostbyname(str);

					if(host == NULL) continue; 
					ptr = (struct in_addr *) 
						host->h_addr_list[0];                    

					// Eg. 211.40.35.76 split up like this.             
					int a = ptr->S_un.S_un_b.s_b1;  // 211           
					int b = ptr->S_un.S_un_b.s_b2;  // 40
					int c = ptr->S_un.S_un_b.s_b3;  // 35
					int d = ptr->S_un.S_un_b.s_b4;  // 76

					strTemp.Format(L"%s -->  %d.%d.%d.%d",
						strFullName,a,b,c,d);
					AfxMessageBox(strTemp);
					
				}
			}
		}

		delete Buffer;
		WNetCloseEnum( hEnum ); 
	}
	
	WSACleanup();

	return true;
}

//#include <winsock2.h>
//#include <ws2tcpip.h>
#include "iphlpapi.h"

//#pragma comment(lib, "iphlpapi.lib")
//#pragma comment(lib, "ws2_32.lib")


bool CEthernet::GetHostIPAddress(CString *pBuff)
{
	CString sAddr;
	char data[sizeof(MIB_IPADDRTABLE) + 32 * sizeof(MIB_IPADDRROW)];
	ULONG len = sizeof(data);
//	struct in_addr  inAddr;
//	PMIB_IPADDRTABLE ipAddr;
//	char szBuf[256];
//	int ind;

//	The GetIpAddrTable function retrieves the interface-to-IP-address mapping table.
//	This is the structure of the data it returns:
//
//	typedef struct _MIB_IPADDRROW
//	{
//	    DWORD        dwAddr;
//	    DWORD        dwIndex;
//	    DWORD        dwMask;
//	    DWORD        dwBCastAddr;
//	    DWORD        dwReasmSize;
//	    unsigned short    unused1;
//	    unsigned short    wType;
//	} MIB_IPADDRROW, *PMIB_IPADDRROW;
//	
//	typedef struct _MIB_IPADDRTABLE
//	{
//	    DWORD         dwNumEntries;
//	    MIB_IPADDRROW table[ANY_SIZE];
//	} MIB_IPADDRTABLE, *PMIB_IPADDRTABLE;
//	
/*
	if(GetIpAddrTable((PMIB_IPADDRTABLE)data, &len, true ) == NO_ERROR) {
		ipAddr = (PMIB_IPADDRTABLE)data;
		for(ind = 0; ind < (int)ipAddr->dwNumEntries; ind++) {
			if(ipAddr->table[ind].dwAddr != 0) {
				memmove (&inAddr, &(ipAddr->table[ind].dwAddr), 4);
				sAddr = inet_ntoa (inAddr);
				sprintf_s(szBuf, 256, "Local address: \t%s\r\n", (LPCWSTR)sAddr); 
				szTotResult += szBuf;

				pBuff->Format(L"%s", sAddr);
			}
		}
	}
	*/
	return true;
}

CString CEthernet::getPortNumber()
{
	CString Buff;

	Buff.Format(L"%d",m_nPort);

	return Buff;
}




void CEthernet::AddPkt(bool bDirection, int nDataSize, char *pData)
{
	int nn=m_nPktL;
	m_nPktL++;

	if(m_nPktL==1) {
		m_PktArray = new ETHERNETPKT[m_nPktL];
	} else {
		m_PktArray = (ETHERNETPKT *)realloc(m_PktArray,m_nPktL * (sizeof ETHERNETPKT));
	};
	m_PktArray[nn].Add(bDirection, nDataSize, pData);

}

int CEthernet::GetPktCount()
{
	return m_nPktL;
}

CString CEthernet::GetPktTxRx(int nIndex)
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

CString CEthernet::FormatPktTime(int nIndex)
{
	CString Buff;

	if(nIndex<m_nPktL) {
		m_PktArray[nIndex].FormatTime(&Buff);
	}
	return Buff;

}

CString CEthernet::GetPktDataSize(int nIndex)
{
	CString Buff;

	if(nIndex<m_nPktL) {
		Buff.Format(L"%d",m_PktArray[nIndex].GetDataSize());
	}
	return Buff;
}

CString CEthernet::FormatPktData(int nIndex)
{
	CString Buff;

	if(nIndex<m_nPktL) {
		m_PktArray[nIndex].FormatData(&Buff);
	}
	return Buff;
}

bool CEthernet::Ping()
{
	WSADATA wsaData;
	// Initialize Winsock
	int wret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wret != 0) {
		printf("WSAStartup failed: %d\n", wret);
		return 1;
	}
	if (AfxSocketInit(&wsaData) == FALSE) {
		AfxMessageBox(L"Failed to Initialize Sockets");
	}

	DWORD dwError;
	struct hostent *remoteHost;
	char host_addr[] = "128.128.128.100"; //or any other IP
	struct in_addr addr = { 0 };

	addr.s_addr = inet_addr(host_addr);
	if (addr.s_addr == INADDR_NONE) {
		printf("The IPv4 address entered must be a legal address\n");
		return 1;
	}
	else
		remoteHost = gethostbyaddr((char *)&addr, 4, AF_INET);


	if (remoteHost == NULL) {
		dwError = WSAGetLastError();
		if (dwError != 0)
			printf("error: %d", dwError);
	}
	else {
		printf("Hostname: %s\n", remoteHost->h_name);
	}

//	WSACleanup();

	return true;
}
