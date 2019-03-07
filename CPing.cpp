#include "stdafx.h"
#include "CPing.h"


CPing::CPing()
{
}


CPing::~CPing()
{
}

int CPing::Ping(CString strIPAddrs, int nPort, CStringArray *pMessages)
{
	char cIPAddrs[100];
	CString Buff;

	WSAData wsaData;
	int nCode;
	if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
		Buff.Format(L"WSAStartup() returned error code %d", nCode);
		if (pMessages) pMessages->Add(Buff);
		return 255;
	}

	int ii;
	for (ii = 0; ii < strIPAddrs.GetLength(); ii++) cIPAddrs[ii] = (char)strIPAddrs.GetAt(ii);
	cIPAddrs[ii] = 0;

	int nRetVal = DoWinsock(cIPAddrs, nPort, 1, pMessages);

	WSACleanup();
	return nRetVal;

}


int CPing::DoWinsock(char* pcHost, int nPort, int nTimesToPing, CStringArray *pMessages)
{
	int have_valid_target = 1;
	ADDRINFO hint, *AddrInfo, *AI;
	char p[6];
	int r;
	int found;
	int nRet = 0;

	int ipv = 4;
	sprintf_s(p, sizeof(p), "%d", nPort);
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = PF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;

	// Find the server's address	
	r = getaddrinfo(pcHost, p, &hint, &AddrInfo);

	if (r != 0) {

		m_strLastError.Format(L"DNS: Could not find host - %s, aborting\n", pcHost);
		m_nLastError = NOT_FIND_HOST;
		if (pMessages) pMessages->Add(m_strLastError);
		return 3;
	}

	for (AI = AddrInfo; AI != NULL; AI = AI->ai_next) {
		if ((AI->ai_family == AF_UNSPEC && ipv == 0) ||
			(AI->ai_family == AF_INET && ipv != 6) ||
			(AI->ai_family == AF_INET6 && ipv != 4)) {
			found = 1;
			break;
		}
	}
	if (found == 0) {
		m_strLastError = L"DNS: No valid host found in AddrInfo for that type";
		m_nLastError = NO_VALID_HOST;
		if (pMessages) pMessages->Add(m_strLastError);
		return 3;
	}
	int nLoopCounter = 0;
	while (nLoopCounter < nTimesToPing) {
		int success_flag = 0;
		int ping_timeout = 2000;
		int	force_send_byte = 0;
		LARGE_INTEGER cpu_frequency;
		LARGE_INTEGER response_timer1;
		LARGE_INTEGER response_timer2;

		if (have_valid_target == 1) {

			SOCKET sd;

			// apparently... QueryPerformanceCounter isn't thread safe unless we do this
			SetThreadAffinityMask(GetCurrentThread(), 1);

			// start the timer right before we do the connection
			QueryPerformanceFrequency((LARGE_INTEGER *)&cpu_frequency);
			QueryPerformanceCounter((LARGE_INTEGER *)&response_timer1);

			sd = EstablishConnection(AI, ping_timeout, force_send_byte);
			QueryPerformanceCounter((LARGE_INTEGER *)&response_timer2);

			if (sd == INVALID_SOCKET) {
				nRet = 4;
				m_strLastError = L"Failed to ping socket";
				if (pMessages) pMessages->Add(m_strLastError);
			}

			if (ShutdownConnection(sd)) {
				nRet = 5;
			}
		}
		nLoopCounter++;
	}
	return nRet;
}



//// EstablishConnection ///////////////////////////////////////////////
// Connects to a given address, on a given port, both of which must be
// in network byte order.  Returns newly-connected socket if we succeed,
// or INVALID_SOCKET if we fail.

SOCKET CPing::EstablishConnection(ADDRINFO* address, int ping_timeout, int force_send_byte) {

	LARGE_INTEGER timer1;
	LARGE_INTEGER timer2;
	LARGE_INTEGER cpu_freq;

	double time_so_far;

	// Create a stream socket
	SOCKET sd = socket(address->ai_family, address->ai_socktype, address->ai_protocol);

	//-------------------------
	// Set the socket I/O mode: In this case FIONBIO
	// enables or disables the blocking mode for the
	// socket based on the numerical value of iMode.
	// If iMode = 0, blocking is enabled;
	// If iMode != 0, non-blocking mode is enabled.
	u_long iMode = 1;
	ioctlsocket(sd, FIONBIO, &iMode);

	QueryPerformanceFrequency((LARGE_INTEGER *)&cpu_freq);
	QueryPerformanceCounter((LARGE_INTEGER *)&timer1);
	connect(sd, address->ai_addr, (int)address->ai_addrlen);

	char *sendy = ".";
	int size = 1;
	int sendstatus = 1000;

	bool done = false;

	while (!done) {

		if (force_send_byte == 0) {
			sendstatus = send(sd, NULL, 0, 0);   // should return 0 below
		}
		else {
			sendstatus = send(sd, sendy, size, 0);   // should return sizeof(sendy) below
		}


		// one error code is if you send a send of size 0, the other is if you actually send data.
		if (sendstatus == size && force_send_byte == 1) {
			closesocket(sd);
			return sd;
		}

		if (sendstatus == 0 && force_send_byte == 0) {
			closesocket(sd);
			return sd;
		}

		QueryPerformanceCounter((LARGE_INTEGER *)&timer2);

		time_so_far = ((double)((timer2.QuadPart - timer1.QuadPart) * (double) 1000.0 / (double)cpu_freq.QuadPart));

		if (time_so_far >= ping_timeout) {
			done = true;
		}
		else {
			if (time_so_far < 200) {  // the idea here is to not grind the processor too hard if the precision gained isn't going to be useful.
				Sleep(0);
			}
			else {
				Sleep(1);
			}
		}
	}

	closesocket(sd);
	return INVALID_SOCKET;
}

const int kBufferSize = 1024;

bool CPing::ShutdownConnection(SOCKET sd) {
	// Disallow any further data sends.  This will tell the other side
	// that we want to go away now.  If we skip this step, we don't
	// shut the connection down nicely.
	if (shutdown(sd, SD_SEND) == SOCKET_ERROR) {
		closesocket(sd);
		return false;
	}

	// Receive any extra data still sitting on the socket.  After all
	// data is received, this call will block until the remote host
	// acknowledges the TCP control packet sent by the shutdown above.
	// Then we'll get a 0 back from recv, signalling that the remote
	// host has closed its side of the connection.
	char acReadBuffer[kBufferSize];
	while (1) {
		int nNewBytes = recv(sd, acReadBuffer, kBufferSize, 0);
		if (nNewBytes == SOCKET_ERROR) {
			closesocket(sd);
			return false;
		}
		else if (nNewBytes != 0) {
			//    cerr << endl << "FYI, received " << nNewBytes <<
			//            " unexpected bytes during shutdown." << acReadBuffer << endl;
			cout << " (" << nNewBytes << " bytes read)";
		}
		else {
			// Okay, we're done!
			break;
		}
	}

	// Close the socket.
	if (closesocket(sd) == SOCKET_ERROR) {
		return false;
	}

	return true;
}

