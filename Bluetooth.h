// Bluetooth.h: interface for the CBluetooth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLUETOOTH_H__272FD4CC_1C98_4F68_85EF_34A6A015D487__INCLUDED_)
#define AFX_BLUETOOTH_H__272FD4CC_1C98_4F68_85EF_34A6A015D487__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


extern	UINT BluetoothReceiveThread (LPVOID lpvoid);
extern	UINT BluetoothListenForConnectionThread (LPVOID lpvoid);

#define CXN_BDADDR_STR_LEN                17   // 6 two-digit hex values plus 5 colons
#define CXN_TRANSFER_DATA_LENGTH          100  // length of the data to be transferred
#define CXN_MAX_INQUIRY_RETRY             3
#define CXN_DELAY_NEXT_INQUIRY            15
#define PACKET_SIZE 10000
#define	PKT_RX	TRUE
#define	PKT_TX	FALSE


typedef struct tagBluetoothPkt
{
private:
	char *m_pData;
	int	m_nDataSize;
	SYSTEMTIME m_st;
	bool	m_bTxRx;

public:
	void	Zero()
	{
		if( m_pData != NULL) {
			delete m_pData;
			m_pData = NULL;
		};
		m_nDataSize = 0;
	};

	void FormatData(CString *pBuff)
	{
		int nLength = m_nDataSize;
		if(nLength>40) nLength = 40;
		pBuff->GetBufferSetLength(nLength);
		for(int ii=0;ii<nLength;ii++) {
			pBuff->SetAt(ii,m_pData[ii]);
		}
	}

	int GetDataSize()
	{
		return m_nDataSize;
	};

	bool GetTxRx()
	{
		return m_bTxRx;
	};

	void SetTime()
	{
		GetLocalTime( &m_st );
	};

	void FormatTime(CString *pBuff)
	{
		pBuff->Format(L"%d-%d %d:%02d:%02d.%03d", m_st.wDay,m_st.wMonth,m_st.wHour,m_st.wMinute,m_st.wSecond,m_st.wMilliseconds);
	};

	void Add(bool bTxRx,int nDataSize,char *pData)
	{
		SetTime();
		m_bTxRx = bTxRx;
		m_nDataSize = nDataSize;
		if(m_nDataSize>0) {
			m_pData = new char[nDataSize];
			CopyMemory(m_pData, pData, nDataSize);
		}
	}


} BLUETOOTHPKT;



class CBluetooth  
{
public:
	CString FormatPktData(int nIndex);
	CString GetPktDataSize(int nIndex);
	CString FormatPktTime(int nIndex);
	CString GetPktTxRx(int nIndex);
	int GetPktCount();
	void AddPkt(bool bDirection, int nDataSize, char *pData);

	BLUETOOTHPKT *m_PktArray;
	int m_nPktL;

	void SendMessage(CString Buff,int nLength,char *pData, CString strEOL);
	void SendMessage(CString Buff);
	bool SendMessage(int nDataSize, char *Data);

	CString getDeviceName(int nIndex);
	int	getAvailableDevicesSize();
	int m_nAvailableDevicesSize;
	void RefreshAvailableDevices();
	CString m_BTDeviceNames[32];
	WSAQUERYSET	m_WSAQuerySet[32];
	CWinThread	*pRecThread;
	bool m_bRecThreadEnabled;
	bool m_bServer;
	long (*m_callback) (CWnd*, int, char*);
	SOCKET m_socket; 
	CWnd* m_pParent;
	bool Disconnect();
	bool Connected();
	bool Connect(CWnd* pParent,bool Server, long fn(CWnd* pParent, int nDataSize, char* Data));
	CBluetooth();
	virtual ~CBluetooth();

};

#endif // !defined(AFX_BLUETOOTH_H__272FD4CC_1C98_4F68_85EF_34A6A015D487__INCLUDED_)
