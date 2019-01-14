#if !defined(AFX_RSOCKET_H__6131A403_3828_4594_B7BA_18622376AA2C__INCLUDED_)
#define AFX_RSOCKET_H__6131A403_3828_4594_B7BA_18622376AA2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RSocket.h : header file
//

#include <afxsock.h>


/////////////////////////////////////////////////////////////////////////////
// CRSocket command target

class CRSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CRSocket();
	virtual ~CRSocket();
	virtual void Close();
	virtual void OnClose(int iErrorCode);
	virtual void OnSend(int iErrorCode){CSocket::OnSend(iErrorCode);}
	virtual void OnReceive(int iErrorCode){CSocket::OnReceive(iErrorCode);}
	virtual void OnConnect(int iErrorCode);
	bool IsConnected(){ return m_bConnected; }
	bool SetTimeOut(UINT uTimeOut);
	bool KillTimeOut();
	CRSocket FromHandle(SOCKET hSocket);
	
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CRSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
	bool IsValidSocket(){return m_hSocket == INVALID_SOCKET;}
protected:
	bool m_bConnected;
private:
	CCriticalSection m_CritSection;
	int m_nTimerID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSOCKET_H__6131A403_3828_4594_B7BA_18622376AA2C__INCLUDED_)
