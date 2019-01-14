// RemoteInterface.cpp: implementation of the CRemoteInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "RemoteInterface.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//bRemoteClientServer false = client; true = server
CRemoteInterface::CRemoteInterface()
{

	m_nPortNumber = 2049;

	m_nTimeSlot = 0;
}

CRemoteInterface::~CRemoteInterface()
{

}

bool CRemoteInterface::Start()
{
	

	switch(theApp.m_LastSettings.nRemoteInterfaceEthernetBluetooth) {
	case 1:	if(m_Ethernet.Connected() == false) {
			m_Ethernet.Connect(NULL,theApp.m_LastSettings.bEthernetRemoteClientServer,RemoteInterfaceEthernetCallback,theApp.m_LastSettings.nRemoteServerIPAddrs,m_nPortNumber);
			return true;
		}
		break;
	case 2:	if(m_Bluetooth.Connected() == false) {
			m_Bluetooth.Connect(NULL,theApp.m_LastSettings.bBluetoothRemoteClientServer,RemoteInterfaceEthernetCallback);
			return true;
		};
		break;
	}
	return false;
}

void CRemoteInterface::Close()
{

	m_Ethernet.Disconnect();
	m_Bluetooth.Disconnect();
}



bool CRemoteInterface::isEthernetConnected()
{
	return m_Ethernet.Connected();
}

CString CRemoteInterface::getPortNumber()
{
	CString Buff;

	Buff.Format(L"%d",m_nPortNumber);
	return Buff;
}

CString CRemoteInterface::getHostAddress()
{
	CString Buff;

	m_Ethernet.GetHostIPAddress(&Buff);
	return Buff;
}

void CRemoteInterface::SendMessage(CString Buff)
{

	

	switch(theApp.m_LastSettings.nRemoteInterfaceEthernetBluetooth) {
	case 1:	m_Ethernet.SendMessage(Buff);
		break;
	case 2:	m_Bluetooth.SendMessage(Buff);
		break;
	}

}

void CRemoteInterface::SendMessage(CString Buff,int nLength, char *pData, CString strEOL)
{
	

	switch(theApp.m_LastSettings.nRemoteInterfaceEthernetBluetooth) {
	case 1:	m_Ethernet.SendMessage(Buff,nLength,pData,strEOL);
		break;
	case 2:	m_Bluetooth.SendMessage(Buff,nLength,pData,strEOL);
		break;
	}
}

int CRemoteInterface::GetPktCount()
{
	
	switch(theApp.m_LastSettings.nRemoteInterfaceEthernetBluetooth) {
	case 1:	return m_Ethernet.GetPktCount();
		break;
	case 2:	return m_Bluetooth.GetPktCount();
		break;
	}
	return -1;
}

CString CRemoteInterface::GetPktTxRx(int nIndex)
{
	

	switch(theApp.m_LastSettings.nRemoteInterfaceEthernetBluetooth) {
	case 1:	return m_Ethernet.GetPktTxRx(nIndex);
		break;
	case 2:	return m_Bluetooth.GetPktTxRx(nIndex);
		break;
	}
	return L"";

	
}

CString CRemoteInterface::FormatPktTime(int nIndex)
{
	
	switch(theApp.m_LastSettings.nRemoteInterfaceEthernetBluetooth) {
	case 1:	return m_Ethernet.FormatPktTime(nIndex);
		break;
	case 2:	return m_Bluetooth.FormatPktTime(nIndex);
		break;
	}
	return L"";
}

CString CRemoteInterface::GetPktDataSize(int nIndex)
{
	

	switch(theApp.m_LastSettings.nRemoteInterfaceEthernetBluetooth) {
	case 1:	return m_Ethernet.GetPktDataSize(nIndex);
		break;
	case 2:	return m_Bluetooth.GetPktDataSize(nIndex);
		break;
	}
	return L"";
}

CString CRemoteInterface::FormatPktData(int nIndex)
{
	

	switch(theApp.m_LastSettings.nRemoteInterfaceEthernetBluetooth) {
	case 1:	return m_Ethernet.FormatPktData(nIndex);
		break;
	case 2:	return m_Bluetooth.FormatPktData(nIndex);
		break;
	}
	return L"";
	
}

long RemoteInterfaceEthernetCallback(CWnd* pParent, int nDataSize, char* Data)
{
	
	CString Buff;
	int nSrc,nDest,nFlag=MSG_NOT_RECOGNISED;

	Buff.GetBufferSetLength(nDataSize);
	for(int ii=0;ii<nDataSize;ii++) {
		Buff.SetAt(ii,Data[ii]);
	}

	//Ultrasonics
	if(tolower(Data[0]) == 'u' && tolower(Data[1]) == 't') {

		Buff.GetBufferSetLength(nDataSize-2);
		for(nSrc=2,nDest=0;nSrc<nDataSize;nSrc++) {
			if(isalnum(Data[nSrc]) ) {
				Buff.SetAt(nDest++,Data[nSrc]);
			}
		}

		nFlag = theApp.m_RemoteInterface.RespondToUltrasonicMsg(&Buff);
	}

	switch(nFlag) {
	case MSG_OK:
		break;
	case INVALID_VALUE: 	theApp.m_RemoteInterface.SendMessage(L"Invalid Value\r\n");
		break;
	case MSG_NOT_RECOGNISED:	theApp.m_RemoteInterface.SendMessage(L"Message not recognised\r\n");
		break;
	}



	return 0;
}

int CRemoteInterface::RespondToUltrasonicMsg(CString *pMessage)
{
	
	int nTemp;
	CString Buff;

	if(pMessage->Find(L"setTimeSlot") == 0) {
		swscanf_s(pMessage->GetBuffer(100),L"setTimeSlot%d",&nTemp);
		if(nTemp>=1 && nTemp<=256) {
			m_nTimeSlot = nTemp - 1;
			return MSG_OK;
		}
		return INVALID_VALUE;
	}

	if(pMessage->Find(L"getLogGain") == 0) {
		theApp.m_RemoteInterface.SendMessage(theApp.m_UtUser.strGetLogGain(m_nTimeSlot) + L"\r\n");
		return MSG_OK;
	}
	if(pMessage->Find(L"getPreAmpGain") == 0) {
		theApp.m_RemoteInterface.SendMessage(theApp.m_UtUser.strGetPreAmpGain(m_nTimeSlot) + L"\r\n");
		return MSG_OK;
	}
	if(pMessage->Find(L"getTimeSlot") == 0) {
		Buff.Format(L"%d",m_nTimeSlot + 1);
		theApp.m_RemoteInterface.SendMessage(Buff + L"\r\n");
		return MSG_OK;
	}
	if(pMessage->Find(L"getAmplifier") == 0) {
		theApp.m_RemoteInterface.SendMessage(theApp.m_UtUser.strGetAmplifier(m_nTimeSlot) + L"\r\n");
		return MSG_OK;
	}
	if(pMessage->Find(L"getTrace") == 0) {
		theApp.m_RemoteInterface.SendMessage(L"Trace", 8192, theApp.m_Scope.m_RFTrace[m_nTimeSlot],  L"\r\n");
		return MSG_OK;
	}

	return MSG_NOT_RECOGNISED;

}


bool CRemoteInterface::isBluetoothConnected()
{

	return m_Bluetooth.Connected();
}


void CRemoteInterface::BTRefreshAvailableDevices()
{
	m_Bluetooth.RefreshAvailableDevices();
}

int CRemoteInterface::getBTAvailableDevicesSize()
{

	return m_Bluetooth.getAvailableDevicesSize();
}

CString CRemoteInterface::getBTDeviceName(int nIndex)
{

	return m_Bluetooth.getDeviceName(nIndex);
}