// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ClientSocket.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
	m_bSocketConnected = false;
	m_bPPMACConnected = false;
	m_nCallBack=0;
	m_nBootupHandShakeMode = 0;
}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

bool CClientSocket::ConnectFromParent(long fn(CString *pBuff), int nRemoteIPAddrs, int nPort,CString User,CString Password)
{
	CString strHostName,Buff;
	int nErrorCode;
	unsigned char *pIpAddress = (unsigned char*)&nRemoteIPAddrs;

	if(m_bSocketConnected == false) {
		m_callback = fn;
		m_Password = Password;
		m_User = User;

		Create();
		AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT | FD_OOB);
		if (nRemoteIPAddrs != 0) {
			strHostName.Format(L"%d.%d.%d.%d", pIpAddress[3], pIpAddress[2], pIpAddress[1], pIpAddress[0]);
		}
		else {
			strHostName = "192.168.0.200";
			strHostName = "169.254.250.101";
		}
		if(Connect(strHostName,nPort) == 0) {
			switch (nErrorCode = GetLastError()) {
			case WSAEWOULDBLOCK: m_bSocketConnected = true;	//non critical error
				break;
			default: Buff.Format(L"Socket non-critial error %d", nErrorCode);
				MessageBox(NULL, Buff, L"Warning", MB_ICONWARNING);
				break;
			}
		} else {
			m_bSocketConnected = true;
		}

		Sleep(90);
	}

	return m_bSocketConnected;
}



bool CClientSocket::IsConnected()
{
	return m_bSocketConnected;
}

void CClientSocket::OnClose(int nErrorCode) 
{
//	AfxMessageBox("Connection Closed",MB_OK);
	
	CAsyncSocket::OnClose(nErrorCode);
	m_bSocketConnected = false;
	m_bPPMACConnected = false;

}

void CClientSocket::OnConnect(int nErrorCode) 
{
	CAsyncSocket::OnConnect(nErrorCode);
	if(nErrorCode == 0) m_bSocketConnected = true;
}

void CClientSocket::OnOutOfBandData(int nErrorCode) 
{
	ASSERT(FALSE); //Telnet should not have OOB data
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CClientSocket::OnSend(int nErrorCode) 
{
	CAsyncSocket::OnSend(nErrorCode);
}

void CClientSocket::OnReceive(int nErrorCode) 
{
	bool bFlag;

	int nBytes = Receive(m_bBuf, IOBUFFSIZE);

	if(nBytes != SOCKET_ERROR) {
		int ndx = 0;
		while(ndx < nBytes) {
			bFlag=GetLine(m_bBuf, nBytes, ndx) & 0x01;
			ProcessOptions();
			if(m_bPPMACConnected == false) {
				m_callback(&m_strNormalText);
				if(m_strNormalText.Find(L"login:")>=0) {
					Send("root\r\n",6);
				}
				if(m_strNormalText.Find(L"Password:")>=0) {
					Send("deltatau\r\n",10);
				}

				switch (m_nBootupHandShakeMode) {
				default:
					if (m_strNormalText.Find(L"ppmac#")>0) {
						Send("gpascii -2\r\n", 12);
						m_bPPMACConnected = true;
					}
					if (m_strNormalText.Find(L"SSH-2.0") >= 0) {
						Send("Debian-4\r\n", 6);
					}
					break;
				case 3:
					if (m_strNormalText.Find(L"ppmac#")>0 && m_strNormalText.GetLength() < 42) {
						Send("gpascii -2\r\n", 12);
					}
					if (m_strNormalText.Find(L"SSH-2.0") >= 0) {
						Send("Debian-4\r\n", 6);
					}
					if (m_strNormalText.Find(L"STDIN") >= 0) {
						m_bPPMACConnected = true;
					}
					break;
				}
				m_strLine.Empty();
			}
			if(m_bPPMACConnected == true) {
				if(bFlag == true) {
					m_callback(&m_strNormalText);
					m_nCallBack++;
					m_strLine.Empty();
				}
			}
		}
	}
	m_strResp.Empty();
}


void CClientSocket::ProcessOptions()
{
	CString m_strTemp;
	CString m_strOption;
	unsigned char ch;
	int ndx;
	int ldx;
	BOOL bScanDone = FALSE;

	m_strTemp = m_strLine;

	while(!m_strTemp.IsEmpty() && bScanDone != TRUE)
	{
		ndx = m_strTemp.Find(IAC);
		if(ndx != -1)
		{
			m_strNormalText += m_strTemp.Left(ndx);
			ch = (unsigned char)m_strTemp.GetAt(ndx + 1);
			switch(ch)
			{
			case DO:
			case DONT:
			case WILL:
			case WONT:
				m_strOption		= m_strTemp.Mid(ndx, 3);
				m_strTemp		= m_strTemp.Mid(ndx + 3);
				m_strNormalText	= m_strTemp.Left(ndx);
				m_ListOptions.AddTail(m_strOption);
				break;
			case IAC:
				m_strNormalText	= m_strTemp.Left(ndx);
				m_strTemp		= m_strTemp.Mid(ndx + 1);
				break;
			case SB:
				m_strNormalText = m_strTemp.Left(ndx);
				ldx = m_strTemp.Find(SE);
				m_strOption		= m_strTemp.Mid(ndx, ldx);
				m_ListOptions.AddTail(m_strOption);
				m_strTemp		= m_strTemp.Mid(ldx);
				AfxMessageBox(m_strOption,MB_OK);
				break;
			}
		}

		else
		{
			m_strNormalText = m_strTemp;
			bScanDone = TRUE;
		}
	} 

	RespondToOptions();
}


void CClientSocket::RespondToOptions()
{
	CString strOption;
	char str[512];
	
	while(!m_ListOptions.IsEmpty())
	{
		strOption = m_ListOptions.RemoveHead();

		ArrangeReply(strOption);
	}


	ZeroMemory(str,sizeof str);
	for(int ii=0;ii<m_strResp.GetLength();ii++) {
		str[ii] = (char)m_strResp.GetAt(ii);
	}
	Send(str,strlen(str));
	m_strResp.Empty();
}

void CClientSocket::ArrangeReply(CString strOption)
{

	unsigned char Verb;
	unsigned char Option;
	unsigned char Modifier;
	unsigned char ch;
	BOOL bDefined = FALSE;

	if(strOption.GetLength() < 3) return;

	Verb = (char)strOption.GetAt(1);
	Option = (char)strOption.GetAt(2);

	switch(Option)
	{
	case 1:	// Echo
	case 3: // Suppress Go-Ahead
		bDefined = TRUE;
		break;
	}

	m_strResp += (WCHAR)IAC;

	if(bDefined == TRUE)
	{
		switch(Verb)
		{
		case DO:
			ch = WILL;
			m_strResp += (WCHAR)ch;
			m_strResp += (WCHAR)Option;
			break;
		case DONT:
			ch = WONT;
			m_strResp += (WCHAR)ch;
			m_strResp += (WCHAR)Option;
			break;
		case WILL:
			ch = DO;
			m_strResp += (WCHAR)ch;
			m_strResp += (WCHAR)Option;
			break;
		case WONT:
			ch = DONT;
			m_strResp += (WCHAR)ch;
			m_strResp += (WCHAR)Option;
			break;
		case SB:
			Modifier = (unsigned char)strOption.GetAt(3);
			if(Modifier == SEND)
			{
				ch = SB;
				m_strResp += (WCHAR)ch;
				m_strResp += (WCHAR)Option;
				m_strResp += (WCHAR)IS;
				m_strResp += (WCHAR)IAC;
				m_strResp += (WCHAR)SE;
			}
			break;
		}
	}

	else
	{
		switch(Verb)
		{
		case DO:
			ch = WONT;
			m_strResp += (WCHAR)ch;
			m_strResp += (WCHAR)Option;
			break;
		case DONT:
			ch = WONT;
			m_strResp += (WCHAR)ch;
			m_strResp += (WCHAR)Option;
			break;
		case WILL:
			ch = DONT;
			m_strResp += (WCHAR)ch;
			m_strResp += (WCHAR)Option;
			break;
		case WONT:
			ch = DONT;
			m_strResp += (WCHAR)ch;
			m_strResp += (WCHAR)Option;
			break;
		}
	}
}


BOOL CClientSocket::GetLine( unsigned char * bytes, int nBytes, int& ndx )
{
	BOOL bLine = FALSE;
	while ( bLine == FALSE && ndx < nBytes )
	{
		unsigned char ch = bytes[ndx];
		
		switch( ch )
		{
		case '\r': // ignore
			m_strLine += "\r"; //"CR";
			break;
		case '\n': // end-of-line
			m_strLine += "\n"; //"LF";
			bLine = TRUE;
			break;
		default:   // other....
			m_strLine += (WCHAR)ch;
			break;
		} 

		ndx ++;

	}
	return bLine;
}

bool CClientSocket::SendStr(CString Buff)
{
	char str[512];
	int ii;

	if(m_bPPMACConnected == true) {

		for(ii=0;ii<Buff.GetLength();ii++) {
			str[ii] = (char)Buff.GetAt(ii);
		}
		str[ii++]='\r';
		str[ii++]='\n';
		str[ii++]=0;
		Send(str,strlen(str));

	}

	return false;
}


