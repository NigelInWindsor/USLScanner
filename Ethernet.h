// Ethernet.h: interface for the CEthernet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETHERNET_H__B4F46556_1AFB_4F12_938D_9BBECDEEF6CE__INCLUDED_)
#define AFX_ETHERNET_H__B4F46556_1AFB_4F12_938D_9BBECDEEF6CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include <sys/timeb.h>
//#include <sys/types.h>
//#include <time.h>
#include <Afxtempl.h>

#define PACKET_SIZE 10000
#define	PKT_RX	TRUE
#define	PKT_TX	FALSE


extern	UINT ReceiveThread (LPVOID lpvoid);
extern	UINT ListenForConnectionThread (LPVOID lpvoid);


typedef struct tagEthernetPkt
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


} ETHERNETPKT;



class CEthernet  
{
public:
	CString FormatPktData(int nIndex);
	CString GetPktDataSize(int nIndex);
	CString FormatPktTime(int nIndex);
	CString GetPktTxRx(int nIndex);
	int GetPktCount();
	void AddPkt(bool bDirection, int nDataSize, char *pData);

	ETHERNETPKT *m_PktArray;
	int m_nPktL;

	CString szTotResult;
	byte char_to_base64 [128];
	char base64_to_char [64];

	void SendMessage(CString Buff,int nLength,char *pData, CString strEOL);
	void SendMessage(CString Buff);
	bool SendMessage(int nDataSize, char *Data);

	CString getPortNumber();
	bool GetAllIPAddress();
	bool GetHostIPAddress(CString *pBuff);
	bool Disconnect();
	CEthernet();
	virtual ~CEthernet();

	void Connect(CWnd* pParent,bool Server, long fn(CWnd* pParent, int nDataSize, char* Data), int nRemoteIPAddrs=0 ,int nPort = 20347);
	bool Connected();


	CWinThread	*pRecThread;
	int m_nPort;
	bool m_bServer;
	SOCKET m_connection; 
	bool m_bRecThreadEnabled;
	long (*m_callback) (CWnd*, int, char*);
	int m_nRemoteIP;
	int m_nPacketSize;
	CWnd* m_pParent;

	//CALLBACK m_Callback;


private:

};

#endif // !defined(AFX_ETHERNET_H__B4F46556_1AFB_4F12_938D_9BBECDEEF6CE__INCLUDED_)
