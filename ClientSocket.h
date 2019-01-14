#if !defined(AFX_CLIENTSOCKET_H__42C5C9C7_3102_11D2_9A30_00C04FB78B23__INCLUDED_)
#define AFX_CLIENTSOCKET_H__42C5C9C7_3102_11D2_9A30_00C04FB78B23__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ClientSocket.h : header file
//

#include <afxsock.h>

/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

const	unsigned char IAC		= 255;
const	unsigned char DO		= 253;
const	unsigned char DONT		= 254;
const	unsigned char WILL		= 251;
const	unsigned char WONT		= 252;
const	unsigned char SB		= 250;
const	unsigned char SE		= 240;
const	unsigned char IS		= '0';
const	unsigned char SEND		= '1';
const	unsigned char INFO		= '2';
const	unsigned char VAR		= '0';
const	unsigned char VALUE		= '1';
const	unsigned char ESC		= '2';
const	unsigned char USERVAR	= '3';

#define dtX 8
#define dtY 13

#define IOBUFFSIZE 0x10000
#define	NUMBEROFLINES 100

class CClientSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	bool SendStr(CString Buff);
	void ArrangeReply(CString strOption);
	CString m_strNormalText;
	void RespondToOptions();
	void ProcessOptions();
	int TempCounter;
	CString m_strOptions;
	CStringList m_ListOptions;
	BOOL bNegotiating;
	BOOL bOptionsSent;
	CString m_strResp;
	CString m_strLine;

	BOOL GetLine( unsigned char * bytes, int nBytes, int& ndx );

	char cText[80][NUMBEROFLINES];
	long cCursX;
	unsigned char m_bBuf[IOBUFFSIZE];
	void ProcessMessage(CClientSocket *pSock);
	
	bool IsConnected();
	bool m_bSocketConnected;
	bool m_bPPMACConnected;
	int  m_nBootupHandShakeMode;
	CString m_User;
	CString m_Password;
	long (*m_callback) (CString *pBuff);
	bool ConnectFromParent(long fn(CString *pBuff), int nRemoteIPAddrs=0 ,int nPort = 20347,CString User = L"root", CString Password = L"deltatau");

	CClientSocket();
	virtual ~CClientSocket();

// Overrides
public:
	int m_nCallBack;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__42C5C9C7_3102_11D2_9A30_00C04FB78B23__INCLUDED_)
