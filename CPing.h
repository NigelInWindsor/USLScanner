#pragma once

#define NOT_FIND_HOST	2
#define NO_VALID_HOST	3


class CPing
{
public:
	CString m_strLastError;
	int	m_nLastError;

	CPing();
	~CPing();
	int Ping(CString strIPAddrs, int nPort, CStringArray *pMessages = NULL);
	int DoWinsock(char * pcHost, int nPort, int nTimesToPing = 0, CStringArray *pMessages = NULL);
	SOCKET EstablishConnection(ADDRINFO * address, int ping_timeout, int force_send_byte);
	bool ShutdownConnection(SOCKET sd);
};

