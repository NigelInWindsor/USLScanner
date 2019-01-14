// Remote.h: interface for the CRemote class.
// This lass listens to the socket for messages, then adds received messages to the CRMessageReceivedQueue.
// It also listens to the CRMessageSendQueue and sends messages from there to the socket.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REMOTE_H__F15056A1_C7AC_40F5_86E3_F4C70EC05C90__INCLUDED_)
#define AFX_REMOTE_H__F15056A1_C7AC_40F5_86E3_F4C70EC05C90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RServerSocket.h"
#include "RClientSocket.h"
#include "RMessage.h"
#include "RMessageHandler.h"
#include "RMessageDefs.h"
#include "LoopTimer.h"
#include "RBluetoothServer.h"
#include "RReceiveMessage.h"
#include "RSendMessage.h"
#include "RMessageQueue.h"
#include "Mmsystem.h"

typedef enum CONNECTION_TYPES {CT_TCPIP, CT_BLUETOOTH} CON_TYPE;

class CRemote  {
public:
	CRemote();
	virtual ~CRemote();
	ULONG StartWinsock();
	bool Close();
	bool CloseThreads();
	bool RestartThreads();
	bool CloseSockets();
	bool CloseClientSocket();
	bool StartServer();
	bool SuspendServer();
	void Send(std::list<CRMessage*> mlist);
	void Send(CRMessage* msg);
	void Send();
	void Receive();
	int GetNSent();
	void SetConType(CON_TYPE type) {m_conType = type;}
	void SetPort(int iPort){ m_iPort=iPort; }
	int GetPort(){return m_iPort;}
	CON_TYPE GetConType(){return m_conType;}

	//Query if the connection is active.
	bool GetOpen();
	bool GetConnected();

	//Semaphores to indicate presence of message ready to send/receive
	static HANDLE m_processSemaphore; //Process lock
	static HANDLE m_sendSemaphore; //Permission to send
	static HANDLE m_updateDlgSemaphore; //Signal to update dialog
	static HANDLE m_dataTransferSemaphore; //Data transfer lock
	static HANDLE m_socketCloseSemaphore; //Semaphore from the socket to indicate closure e.g. when client socket disconnects.
	static HANDLE m_hKillEvent;

	static HANDLE m_listenClosed;
	static HANDLE m_sendClosed;


	vector<CLoopTimer*> GetTimers(){return m_timers;}
	CRMessage* GetSendMessage(){return m_sendMess;}
	CRMessage* GetReceiveMessage(){return m_recMess;}

	CRMessageHandler* GetMessageHandler(){return m_pMessageHandler;}

	bool OnSocketClosed();
	bool Restart();
	bool GetRestart(){return m_bRestart;}
	void SetRestart(bool val){m_bRestart = val;}

	void SetConnected(bool val);
	void SetOpen(bool val);

	void SetInitialized(bool val){m_bInitialized=val;}
	bool GetInitialized(){return m_bInitialized;}

	//Waits for the kill event for a set length of time. If it occurs, returns true, else false.
	static bool WaitForKillEvent(int time_ms = 0);

	CString GetThisComputerName();

private:

	//The sockets.
	//The socket to read from and write to (remote socket for server, local socket for client),
	CRClientSocket* m_pConnectionSocket;
	//Local socket.
	CRSocket* m_pLocalSocket;
	//Pointers to bluetooth and WIFI versions of the above.
	CRClientSocket* m_pBTConnectionSocket;
	CRSocket* m_pBTLocalSocket;
	CRClientSocket* m_pWIFIConnectionSocket;
	CRSocket* m_pWIFILocalSocket;

	bool GetServerOpen();
	void SetServerOpen();
	void SetServerClosed();

	bool m_bServerOpen; //To keep track of server state.

	//The port number.
	int m_iPort;
	
	//Set the default values
	void Defaults();
	void InitSockets();
	void InitSockets(WSADATA* dat);
	bool CreateServerSocket();
	bool Listen();

	//Threads
	typedef struct THREADSTRUCT
	{
		CRemote* _this;
	}THREADSTRUCT;
	static UINT SendQueueListenThread(LPVOID pParam);
	CWinThread* m_hSendQueueListenThread;
	bool HasThreadExited(CWinThread* thread);

	THREADSTRUCT *m_pParam;
//	LPVOID m_pParam;
	void DeleteSockets();
	void DeleteServers();
	//Connection open flag.
	bool m_bOpen;
	bool m_bClosing;
	void SetClosing(bool val);
	bool GetClosing();
	//Connected flag.
	bool m_bConnected;
	bool m_bInitialized;
	//Wether to restart on socket close.
	bool m_bRestart;
	//For thread synchronisation
	CCriticalSection m_CritSection;	
	void SetNSent(int);
	int m_nSent;

	//Loop timers
	vector<CLoopTimer*> m_timers; 

	//The type of connection (see enum CON_TYPE)
	CON_TYPE m_conType;

	//For establishing a bluetooth connection.
	CRBluetoothServer* m_pBTServer;

	//A class for routing RMessages to their message handlers.
	CRMessageHandler* m_pMessageHandler;

	CWnd* GetRemotePage();

	//Pointers to the send and receive messages.
	CRMessage* m_recMess;
	CRMessage* m_sendMess;


	//The send message queue
	CRMessageQueue* m_pSendQueue = NULL;

	bool clearSendQueue(int iTimeOut_ms);

	bool WaitForThreadsToClose(int timeout);

	bool StartBluetoothServer();
	bool OpenWifiSockets();

	void UpdateDialog();
};

#endif // !defined(AFX_REMOTE_H__F15056A1_C7AC_40F5_86E3_F4C70EC05C90__INCLUDED_)
