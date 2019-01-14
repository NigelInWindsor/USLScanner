#include "stdafx.h"
#include "RBluetooth.h"
#include<sstream>
#include <wchar.h>
#include <intsafe.h>
#include <strsafe.h>


CRBluetooth::CRBluetooth() :	m_iCxnSuccess(0), m_iCxnError(1),  m_iCxnMaxInquiryRetry(3),
	m_iCxnDelayNextInquiry(15), m_iCxnDefaultListenBacklog(4), m_iMaxCxnCycles(1),
	m_sCxnTestDataString(L"abcdefghij0123456!£$%^")
{
	m_pszRemoteName = L"DESKTOP-GN0HPMT";
	//m_pszRemoteName = L"USL-PC-1164";
	SetSendMessageData(CXN_TEST_DATA_STRING);
	m_iMaxMessageLength = 10000;
	m_LocalSocket = INVALID_SOCKET;
	m_ulRetCode = m_iCxnSuccess;
    //m_sCxnInstanceString = L"Sample Bluetooth Server";

	//wchar_t* m_sCxnTestDataString = L"abcdefghij0123456!£$%^";
	
	
	m_bOpen = true;
	m_ulRetCode = m_iCxnSuccess;
	m_LocalSocket = INVALID_SOCKET;
	AllocateStringBuffer();
	m_bMessageReady = false;
	m_pszDataBufferIndex = NULL;
	m_pszDataBuffer = NULL;
	m_uiTotalLengthReceived = 0;
}


CRBluetooth::~CRBluetooth()
{
}

void CRBluetooth::InitialiseVariables() {
}

void CRBluetooth::ReinitialiseVariables() {
}

void CRBluetooth::Stop() {
	//Change the flag that keeps the accept thread or client connection open.
	m_bOpen = false;
}

ULONG CRBluetooth::StartWinsock() {
	ULONG       ulRetCode = m_iCxnSuccess;
	WSADATA     WSAData = { 0 };
	//todo	m_pRemoteBtAddr =  NULL ;

	if (m_iCxnSuccess == ulRetCode) {
		ulRetCode = WSAStartup(MAKEWORD(2, 2), &WSAData);
		if (m_iCxnSuccess != ulRetCode) {
			AfxMessageBox(L"-FATAL- | Unable to initialize Winsock version 2.2\n");
		}
	}
	return ulRetCode;
}

int CRBluetooth::MaxMessageSize()
{
	return MAX_LENGTH_BT_MESSAGE * sizeof(wchar_t);
}

ULONG CRBluetooth::NameToBthAddr() {
	INT             iResult = m_iCxnSuccess;
	BOOL            bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
	ULONG           ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
	HANDLE          hLookup = NULL;
	PWSAQUERYSET    pWSAQuerySet = NULL;

	ZeroMemory(&m_pRemoteBtAddr, sizeof(m_pRemoteBtAddr));

	pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		ulPQSSize);
	if (NULL == pWSAQuerySet) {
		iResult = STATUS_NO_MEMORY;
		wprintf(L"!ERROR! | Unable to allocate memory for WSAQUERYSET\n");
	}

	
	// Find the device with the correct name
	if (m_iCxnSuccess == iResult) {

		for (INT iRetryCount = 0;
			!bRemoteDeviceFound && (iRetryCount < m_iCxnMaxInquiryRetry);
			iRetryCount++) {
			
			//LUP settings.
			ulFlags = LUP_CONTAINERS;
			ulFlags |= LUP_RETURN_NAME;
			ulFlags |= LUP_RETURN_ADDR;

			if (0 == iRetryCount) {
				wprintf(L"*INFO* | Inquiring device from cache...\n");
			}
			else {
		
				ulFlags |= LUP_FLUSHCACHE;
				AfxMessageBox(L"*INFO* | Unable to find device.  Waiting for %d seconds before re-inquiry...\n", m_iCxnDelayNextInquiry);
				Sleep(m_iCxnDelayNextInquiry * 1000);
				AfxMessageBox(L"*INFO* | Inquiring device ...\n");
			}

			// Start the lookup service
			iResult = m_iCxnSuccess;
			hLookup = 0;
			bContinueLookup = FALSE;
			ZeroMemory(pWSAQuerySet, ulPQSSize);
			pWSAQuerySet->dwNameSpace = NS_BTH;
			pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
			iResult = WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);

			// Even if we have an error, we want to continue until we
			// reach the m_iCxnMaxInquiryRetry
			if ((NO_ERROR == iResult) && (NULL != hLookup)) {
				bContinueLookup = TRUE;
			}
			else if (0 < iRetryCount) {
				AfxMessageBox(L"=CRITICAL= | WSALookupServiceBegin() failed with error code %d, WSAGetLastError = %d\n", iResult, WSAGetLastError());
				break;
			}

			while (bContinueLookup) {
				// Get info about next bluetooth device
				if (NO_ERROR == WSALookupServiceNext(hLookup,
					ulFlags,
					&ulPQSSize,
					pWSAQuerySet)) {
					// Compare names to see if this is the device we are looking for.
					if ((pWSAQuerySet->lpszServiceInstanceName != NULL) &&
						(m_iCxnSuccess == _wcsicmp(pWSAQuerySet->lpszServiceInstanceName, m_pszRemoteName))) {
						// Found a remote bluetooth device with matching name.
						// Get the address of the device and exit the lookup.
						CopyMemory(&m_pRemoteBtAddr,
							(PSOCKADDR_BTH)pWSAQuerySet->lpcsaBuffer->RemoteAddr.lpSockaddr,
							sizeof(m_pRemoteBtAddr));
						bRemoteDeviceFound = TRUE;
						bContinueLookup = FALSE;
					}
				}
				else {
					iResult = WSAGetLastError();
					if (WSA_E_NO_MORE == iResult) { //No more data
													// No more devices found.  Exit the lookup.
						bContinueLookup = FALSE;
					}
					else if (WSAEFAULT == iResult) {
						// The buffer for QUERYSET was insufficient.
						// In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
						// the required size.  So we can use this parameter to reallocate memory for QUERYSET.
						HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
						pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
							HEAP_ZERO_MEMORY,
							ulPQSSize);
						if (NULL == pWSAQuerySet) {
							AfxMessageBox(L"!ERROR! | Unable to allocate memory for WSAQERYSET\n");
							iResult = STATUS_NO_MEMORY;
							bContinueLookup = FALSE;
						}
					}
					else {
						AfxMessageBox(L"=CRITICAL= | WSALookupServiceNext() failed with error code %d\n", iResult);
						bContinueLookup = FALSE;
					}
				}
			}

			// End the lookup service
			WSALookupServiceEnd(hLookup);

			if (STATUS_NO_MEMORY == iResult) {
				break;
			}
		}
	}

	if (NULL != pWSAQuerySet) {
		HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
		pWSAQuerySet = NULL;
	}

	if (bRemoteDeviceFound) {
		SetSuccess();
	}
	else {
		SetError();
	}

	return iResult;
}



BOOL CRBluetooth::StatusOK() {
	return (m_ulRetCode == m_iCxnSuccess);
}

CString CRBluetooth::GetMessageData()
{
	m_bMessageReady = false;
	return  m_sMessageData; 
}

void CRBluetooth::SetMessageData(CString val)
{
	m_sMessageData = val;
	m_iMessageDataSize = (m_sMessageData.GetLength() + 1) * sizeof(m_sMessageData.GetAt(0));
	m_bMessageReady = true;
}

void CRBluetooth::SetMessageData(wchar_t * val)
{
	CString csVal = CString(val,CXN_TRANSFER_DATA_LEN);
	SetMessageData(csVal);
}

void CRBluetooth::SetMessageData(char * val)
{
	CString csVal = CString((wchar_t*)val, CXN_TRANSFER_DATA_LEN);
	SetMessageData(csVal);
}

void CRBluetooth::SetMessageDataChar(char * val)
{
	CString csVal = CString(val, CXN_TRANSFER_DATA_LEN);
	SetMessageData(csVal);
}


void CRBluetooth::PrintData() {

	if (CXN_TRANSFER_DATA_LEN != m_uiTotalLengthReceived) {
		wprintf(L"+WARNING+ | Data transfer aborted mid-stream. Expected Length = [%I64u], Actual Length = [%d]\n", (ULONG64)sizeof(m_sCxnTestDataString), m_uiTotalLengthReceived);
	}
	wprintf(L"*INFO* | Received following data string from remote device:\n%s\n", (wchar_t *)m_pszDataBuffer);
	OutputDebugString(L"\n");
	OutputDebugString(L"Received the following data: \n");
	OutputDebugString((wchar_t*)m_pszDataBuffer);
	OutputDebugString(L"\n");
}

void CRBluetooth::SetSendMessageData(CString val)
{
	m_sSendMessageData = val;
	m_iSendMessageDataSize = (m_sSendMessageData.GetLength() + 1) * sizeof(m_sMessageData.GetAt(0));
	m_bSendingByteArray = false;
}

void CRBluetooth::SendData() {
	if (m_bSendingByteArray) {
		SendByteArrayData();
	}
	else {
		SendCharData();
	}
}

void CRBluetooth::SendWCharData() {
	//Create a data string.
	wchar_t* pszData = NULL;
	pszData = (wchar_t *)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		CXN_TRANSFER_DATA_LEN);
	if (NULL == pszData) {
		m_ulRetCode = STATUS_NO_MEMORY;
		wprintf(L"=CRITICAL= | HeapAlloc failed | out of memory, gle = [%d] \n", GetLastError());
	}
	//Copy the message string to the data string.
	HRESULT res = StringCbCopyN(pszData, m_iSendMessageDataSize, m_sSendMessageData.GetBuffer(m_sSendMessageData.GetLength()), m_iSendMessageDataSize);
	//HRESULT res =(wcsncpy(pszData, m_sSendMessageData.GetBuffer(m_sSendMessageData.GetLength()), m_iSendMessageDataSize) !=NULL);
	if (FAILED(res)) {
		wprintf(L"=CRITICAL= | Creating a static data string failed\n");
		SetError();
		return;
	}
	//Null terminate the string.
	pszData[(m_iSendMessageDataSize / sizeof(wchar_t)) - 1] = 0;
	CString infoStr = CString(L"*INFO* | Sending following data string: ") + CString((wchar_t*)pszData) + CString(L"\n");
	OutputDebugString(infoStr);
	if (SOCKET_ERROR == send(*m_pSendReceiveSocket,
		(char *)pszData,
		//(int)CXN_TRANSFER_DATA_LEN,
		(int)m_iSendMessageDataSize,
		//sizeOfTestDataString,
		0)) {
		ULONG WSALastErr = WSAGetLastError();
		std::wstringstream wss;
		//todo wss << L"=CRITICAL= | send() call failed w/socket = [0x" << std::hex << (ULONG64)m_LocalSocket << std::dec << "], szData = [" << pszData << "], dataLen = [" << m_iSendMessageDataSize << "]. WSAGetLastError=[" << WSAGetLastError() << "]" << std::endl;
		CString errStr = CString(wss.str().c_str());
		AfxMessageBox(errStr);
		SetError();
	}
}

void CRBluetooth::SendCharData() {
	//Create a data string.
	char* pszData = NULL;
	pszData = (char*)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		CXN_TRANSFER_DATA_LEN);
	if (NULL == pszData) {
		m_ulRetCode = STATUS_NO_MEMORY;
		wprintf(L"=CRITICAL= | HeapAlloc failed | out of memory, gle = [%d] \n", GetLastError());
	}
	//Copy the message string to the data string.
//	HRESULT res = StringCbCopyN(pszData, m_iSendMessageDataSize, m_sSendMessageData.GetBuffer(m_sSendMessageData.GetLength()), m_iSendMessageDataSize);
	int i = 0;
	int nchars = m_iSendMessageDataSize / sizeof(m_sSendMessageData.GetAt(0));
	for (; i < nchars-1; i++) {
		pszData[i] = (char)m_sSendMessageData.GetBuffer(m_sSendMessageData.GetLength())[i];
	}
	//Null terminate the string.
	pszData[i] = '\n';
	pszData[i+1] = '\0';
	//if (FAILED(res)) {
	//	wprintf(L"=CRITICAL= | Creating a static data string failed\n");
	//	SetError();
	//	return;
	//}
	//Null terminate the string.
//	pszData[(m_iSendMessageDataSize / sizeof(char)) - 1] = 0;
	CString infoStr = CString(L"*INFO* | Sending following data string: ") + CString((char*)pszData) + CString(L"\n");
	OutputDebugString(infoStr);
	if (SOCKET_ERROR == send(*m_pSendReceiveSocket,
		(char *)pszData,
		//(int)CXN_TRANSFER_DATA_LEN,
		(int)m_iSendMessageDataSize,
		//sizeOfTestDataString,
		0)) {
		ULONG WSALastErr = WSAGetLastError();
		std::wstringstream wss;
//todo		wss << L"=CRITICAL= | send() call failed w/socket = [0x" << std::hex << (ULONG64)m_LocalSocket << std::dec << "], szData = [" << pszData << "], dataLen = [" << m_iSendMessageDataSize << "]. WSAGetLastError=[" << WSAGetLastError() << "]" << std::endl;
		CString errStr = CString(wss.str().c_str());
		AfxMessageBox(errStr);
		SetError();
	}
}

void CRBluetooth::SendByteArrayData()
{
	//Create a data string.
	char* pszData = NULL;
	//Allocate enough memory for the bytes.
	pszData = (char*)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		US_MESSAGE_SIZE);
	if (NULL == pszData) {
		m_ulRetCode = STATUS_NO_MEMORY;
		wprintf(L"=CRITICAL= | HeapAlloc failed | out of memory, gle = [%d] \n", GetLastError());
	}
	//Copy the message string to the data string.
//	HRESULT res = StringCbCopyN(pszData, m_iSendMessageDataSize, m_sSendMessageData.GetBuffer(), m_iSendMessageDataSize);
	int i = 0;
	int nchars = US_MESSAGE_SIZE;
	std::wstringstream wss;
	wss.clear();

//	memcpy(pszData, m_baSendMessageData, m_ibaSize);
	//Use the thread safe copy function provided my the USMessage class.
//todo	m_sendMessage->GetData(pszData);

	//For debugging------------------
	//	for (; i < nchars; i++) {

	//	wss << (int)m_baSendMessageData[i];
	//		wss << ",";
	//	}
	//CString messageString = CString(wss.str().c_str());
	//CString infoStr = CString(L"*INFO* | Sending following integers as bytes: ") + messageString + CString(L"\n");
//	OutputDebugString(infoStr);
	//===============================

	if (SOCKET_ERROR == send(*m_pSendReceiveSocket,
		reinterpret_cast<char*>(pszData),
		//(int)CXN_TRANSFER_DATA_LEN,
		US_MESSAGE_SIZE,
		//sizeOfTestDataString,
		0)) {
		ULONG WSALastErr = WSAGetLastError();
		
		//todo wss << L"=CRITICAL= | send() call failed w/socket = [0x" << std::hex << (ULONG64)m_LocalSocket << std::dec << "], szData = [" << pszData << "], dataLen = [" << US_MESSAGE_SIZE*sizeof(char) << "]. WSAGetLastError=[" << WSAGetLastError() << "]" << std::endl;
		CString errStr = CString(wss.str().c_str());
		AfxMessageBox(errStr);
		SetError();
	}

}

void CRBluetooth::AllocateStringBuffer() {
	m_pszDataBuffer = (char *)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		CXN_TRANSFER_DATA_LEN);
	if (NULL == m_pszDataBuffer) {
		std::wstringstream wss;
		wss << L"-FATAL- | HeapAlloc failed | out of memory | gle = [" << GetLastError() << "]" << std::endl;
		AfxMessageBox(wss.str().c_str());
		SetError();
	}
}

BOOL CRBluetooth::ReadCharData() {
	AllocateStringBuffer();
	bool bContinue = true;
	m_pszDataBufferIndex = m_pszDataBuffer;
	m_uiTotalLengthReceived = 0;

	//Read data from the socket up to the maximum length, or until a null character is received.
	while (bContinue && (m_uiTotalLengthReceived < m_iMaxMessageLength)) {
		UINT iLengthReceived = 0;
		while (iLengthReceived =
			recv(*m_pSendReceiveSocket,
			(char *)m_pszDataBufferIndex,
				(CXN_TRANSFER_DATA_LEN - m_uiTotalLengthReceived),
				0)
			) {
			if (iLengthReceived > CXN_TRANSFER_DATA_LEN) {
				break;
				bContinue = false;
			}
			if (iLengthReceived > 0 ) {
				//Check for the null character
				UINT iterm = iLengthReceived / sizeof(char);
				char charTerm = (char)m_pszDataBuffer[iterm];
				if (charTerm == (char)0) {
					bContinue = false;
					break;
				}
			}

		}
		switch (iLengthReceived) {
		case 0: // socket connection has been closed gracefully
			bContinue = FALSE;

		case SOCKET_ERROR:
			wprintf(L"=CRITICAL= | recv() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
			bContinue = FALSE;
			SetError();
			return FALSE;

		default:

			//
			// Make sure we have enough room
			//
			if (iLengthReceived > (CXN_TRANSFER_DATA_LEN - m_uiTotalLengthReceived)) {
				wprintf(L"=CRITICAL= | received too much data\n");
				bContinue = FALSE;
				SetError();
				return FALSE;
			}

			m_pszDataBufferIndex += iLengthReceived;
			m_uiTotalLengthReceived += iLengthReceived;
		}
	}
	//Copy the received data to the CRBluetooth.
	SetMessageDataChar(m_pszDataBuffer);
	return TRUE;
}

BOOL CRBluetooth::ReadData() {
	return ReadCharData();
}

BOOL CRBluetooth::ReadWCharData() {
	bool bContinue = true;
	m_pszDataBufferIndex = m_pszDataBuffer;
	m_uiTotalLengthReceived = 0;
	int iLengthReceived = 0;


	//Read data from the socket up to the maximum length, or until a null character is received.
	while (bContinue && (m_uiTotalLengthReceived < m_iMaxMessageLength)) {
		UINT            iLengthReceived = 0;
		while (iLengthReceived =
			recv(*m_pSendReceiveSocket,
			(char *)m_pszDataBufferIndex,
				(CXN_TRANSFER_DATA_LEN - m_uiTotalLengthReceived),
				0)
			) {
			if (iLengthReceived > 0) {
				//Check for the null character
				UINT iterm = iLengthReceived / sizeof(wchar_t);
				wchar_t charTerm = (wchar_t)m_pszDataBuffer[iterm];
				if (charTerm == (wchar_t)0) {
					bContinue = false;
					break;
				}
			}

		}



		switch (iLengthReceived) {
		case 0: // socket connection has been closed gracefully
			bContinue = FALSE;

		case SOCKET_ERROR:
			wprintf(L"=CRITICAL= | recv() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
			bContinue = FALSE;
			SetError();
			return FALSE;

		default:

			//
			// Make sure we have enough room
			//
			if (iLengthReceived > (CXN_TRANSFER_DATA_LEN - m_uiTotalLengthReceived)) {
				wprintf(L"=CRITICAL= | received too much data\n");
				bContinue = FALSE;
				SetError();
				return FALSE;
			}

			m_pszDataBufferIndex += iLengthReceived;
			m_uiTotalLengthReceived += iLengthReceived;
		}
	}
	//Copy the received data to the CRBluetooth.
	SetMessageData(m_pszDataBuffer);
	return TRUE;
}

ULONG CRBluetooth::SetSocketSendBufferLength() {
	//Set the buffer length to the message size.
	int optVal = US_MESSAGE_SIZE;
	int optLen = sizeof(int);
	int iResult = setsockopt(m_LocalSocket, SOL_SOCKET, SO_SNDBUF,
		(char*)&optVal, optLen);
	if (iResult == SOCKET_ERROR) {
		SetError();
	}
	else {
		SetSuccess();
	}
	return iResult;
}

ULONG CRBluetooth::SetSocketReceiveBufferLength() {
	//Set the buffer length to the message size.
	int optVal = US_MESSAGE_SIZE;
	int optLen = sizeof(int);
	int iResult = setsockopt(m_LocalSocket, SOL_SOCKET, SO_RCVBUF,
		(char*)&optVal, optLen);
	if (iResult == SOCKET_ERROR) {
		SetError();
	}
	else {
		SetSuccess();
	}
	return iResult;
}
