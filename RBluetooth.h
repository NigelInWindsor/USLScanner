#ifndef _RBLUETOOTH_H_6219745162375124971249598612
#define _RBLUETOOTH_H_6219745162375124971249598612

//#include <stdio.h>
//#include <initguid.h>
//#include <winsock2.h>
//#include <ws2bth.h>
//#include <strsafe.h>
//#include <intsafe.h>


#include <stdio.h>
#include <winsock2.h>
#include <ws2bth.h>
//#include <strsafe.h>
#include <string.h>



#define CXN_TEST_DATA_STRING              (L"~!@#$%^&*()-_=+?<>1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define CXN_TRANSFER_DATA_LEN          (sizeof(CXN_TEST_DATA_STRING))
#define CXN_BDADDR_STR_LEN                17   // 6 two-digit hex values plus 5 colons
#define CXN_MAX_INQUIRY_RETRY             3
#define CXN_DELAY_NEXT_INQUIRY            15
#define CXN_SUCCESS                       0
#define CXN_ERROR                         1
#define CXN_DEFAULT_LISTEN_BACKLOG        4
#define MAX_LENGTH_BT_MESSAGE 1000
//#define CXN_TEST_DATA_STRING              (L"~!@#$%^&*()-_=+?<>1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
//#define CXN_TRANSFER_DATA_LEN          (sizeof(CXN_TEST_DATA_STRING))
#define US_MESSAGE_SIZE 100




// {B62C4E8D-62CC-404b-BBBF-BF3E3BBB1374}
//namespace RBluetooth{
//wchar_t g_szRemoteName[BTH_MAX_NAME_SIZE + 1] = { 0 };  // 1 extra for trailing NULL character
//wchar_t g_szRemoteAddr[CXN_BDADDR_STR_LEN + 1] = { 0 }; // 1 extra for trailing NULL character
//int  g_ulMaxCxnCycles = 1;
//ULONG NameToBthAddr(_In_ const LPWSTR pszRemoteName, _Out_ PSOCKADDR_BTH pRemoteBthAddr);
//ULONG RunClientMode(_In_ SOCKADDR_BTH ululRemoteBthAddr, _In_ int iMaxCxnCycles = 1);
//ULONG RunServerMode(_In_ int iMaxCxnCycles = 1);
//void  ShowCmdLineHelp(void);
//ULONG ParseCmdLine(_In_ int argc, _In_reads_(argc) wchar_t * argv[]);
//}

class CRBluetooth{
public:
	CRBluetooth();
	~CRBluetooth();

	//Get and set for the remote name (e.g. the name of the remote device).
	void SetRemoteName(const LPWSTR pszRemoteName) { m_pszRemoteName = pszRemoteName; }
	LPWSTR GetRemoteName() const { return m_pszRemoteName; }

	//Converts the remote name to a bluetooth address and stores it in the member variable.
	//Returns error/success code.
	ULONG NameToBthAddr();

	int iCxnSuccess() const { return m_iCxnSuccess; }
	
	ULONG StartWinsock();
	
	virtual void Stop();
	int MaxMessageSize();

	bool IsOpen() { return m_bOpen; }
	bool MessageReady() { return m_bMessageReady; }

	CString GetMessageData();

	//Set the received message data
	void SetMessageData(CString val);
	void SetMessageData(wchar_t* val);
	void SetMessageData(char * val);

	void SetMessageDataChar(char * val);

	//void SetSendMessage(USMessage* msg) { m_sendMessage = msg; m_bSendingByteArray = true;}

	//Set the message data to be sent
	void SetSendMessageData(CString val);
	
	void SendData();

	void SendWCharData();

	void SendCharData();

	void SendByteArrayData();

	void AllocateStringBuffer();

	BOOL ReadData();

	BOOL ReadWCharData();



	BOOL ReadCharData();

	SOCKET GetLocalSocket(){return m_LocalSocket;}
	SOCKET GetSendReceiveSocket(){return (*m_pSendReceiveSocket);}


	
protected:
//	GUID m_guidServiceClass;
	SOCKET          m_LocalSocket;
	//A pointer to the socket used to send data. 
	//For the server, this will be the client socket.
	//For the client, this will be the local socket.
	SOCKET*         m_pSendReceiveSocket;

	ULONG SetSocketSendBufferLength();

	ULONG SetSocketReceiveBufferLength();

	void PrintData();

	LPWSTR m_pszRemoteName;
	SOCKADDR_BTH m_pRemoteBtAddr;
	//Status an error codes
	const ULONG m_iCxnSuccess;
	ULONG           m_ulRetCode;
	const ULONG m_iCxnError;                         
	const int m_iCxnMaxInquiryRetry;
	const int m_iCxnDelayNextInquiry;
	#define CXN_INSTANCE_STRING L"Sample Bluetooth Server"
	//const wchar_t* m_sCxnInstanceString = L"Sample bluetooth server";
	const int m_iCxnDefaultListenBacklog;
	const CString m_sCxnTestDataString;
	const int m_iMaxCxnCycles;

	//virtual void InitialiseVariables();

	bool m_bOpen;
	bool m_bMessageReady;

	//The data string received
	CString m_sMessageData;
	//The size of the message string in bytes.
	int m_iMessageDataSize;

	//The message to be sent.
	CString m_sSendMessageData;
	ULONG m_iSendMessageDataSize;
	bool m_bSendingByteArray;

	//A pointer to the thread-safe messaqge object
//	USMessage* m_sendMessage;
	bool GetSuccess() {return (m_ulRetCode == m_iCxnSuccess);}
	void SetSuccess() { m_ulRetCode = m_iCxnSuccess; }
	void SetError() { m_ulRetCode = m_iCxnError; }
	BOOL StatusOK();

	virtual void CRBluetooth::InitialiseVariables();
	virtual void CRBluetooth::ReinitialiseVariables();
	char *          m_pszDataBufferIndex;
	char *          m_pszDataBuffer;
	UINT m_uiTotalLengthReceived;

	UINT m_iMaxMessageLength;
};

#endif //_RBLUETOOTH_H_6219745162375124971249598612