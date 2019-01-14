// RemoteInterface.h: interface for the CRemoteInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REMOTEINTERFACE_H__C027BF6B_C835_4E73_B39D_AD509D412F80__INCLUDED_)
#define AFX_REMOTEINTERFACE_H__C027BF6B_C835_4E73_B39D_AD509D412F80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Ethernet.h"
#include "Bluetooth.h"

#define MSG_OK				1
#define INVALID_VALUE		-1
#define MSG_NOT_RECOGNISED	-2

extern	long RemoteInterfaceEthernetCallback(CWnd* pParent, int nDataSize, char* Data);

class CRemoteInterface  
{
private:
	CBluetooth	m_Bluetooth;
	CEthernet	m_Ethernet;
public:
	CString getBTDeviceName(int nIndex);
	int	getBTAvailableDevicesSize();
	void BTRefreshAvailableDevices();
	bool isBluetoothConnected();
	int m_nTimeSlot;
	int RespondToUltrasonicMsg(CString *pMessage);
	CString FormatPktData(int nIndex);
	CString GetPktDataSize(int nIndex);
	CString FormatPktTime(int nIndex);
	CString GetPktTxRx(int nIndex);
	int GetPktCount();
	bool m_bServer;
	void SendMessage(CString Buff,int nLength,char *pData, CString strEOL);
	void SendMessage(CString Buff);
	CString getPortNumber();
	CString getHostAddress();
	int m_nPortNumber;
	bool isEthernetConnected();
	void Close();
	bool Start();
	CRemoteInterface();
	virtual ~CRemoteInterface();

};

#endif // !defined(AFX_REMOTEINTERFACE_H__C027BF6B_C835_4E73_B39D_AD509D412F80__INCLUDED_)
