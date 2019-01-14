// Serial.cpp: implementation of the CSerial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Serial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#undef BUFFER_LENGTH
#define BUFFER_LENGTH	128
extern	UINT SerialPortOverlappedReadThread (LPVOID lpvoid);

 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerial::CSerial()
{
	m_nComPort = 1;
	m_RxBuffer = NULL;
	m_pThread = NULL;
	m_bLF = false;
	m_bCR = false;
	m_iBaudRate=2400;
	m_iByteSize=8;
}

CSerial::~CSerial()
{

	Close();

}

bool CSerial::Start(int nComPort)
{
	CString Buff;
	m_nComPort = nComPort;

	Buff.Format(L"Com%d",nComPort);
	return Start(Buff);
}

bool CSerial::Start(CString sComPort){
	return Initialize(sComPort);
}

bool CSerial::Close ()
{
	DWORD dwError;
	CString Buff;

	if (m_bThreadEnabled==TRUE) {
		m_bThreadEnabled=FALSE;
		WaitForSingleObject(&m_pThread->m_hThread,INFINITE);
		SAFE_DELETE(m_RxBuffer);
		if(!CloseHandle (hPort)) {
			dwError = GetLastError();
			MessageBox(NULL,_T("Error closing handle to serial port"),_T("Serial Port"),MB_ICONERROR);
			return false;
		}
	}
	return true;

}

bool CSerial::Initialize (CString strPortName)
{
  DWORD dwError;
//DWORD dwThreadID;
  DCB PortDCB;
  COMMTIMEOUTS CommTimeouts;
	m_bThreadEnabled = FALSE;

  wcscpy_s(m_PortName,50, strPortName);

  // Open the serial port.
  hPort = CreateFile (m_PortName, // Pointer to the name of the port
                      GENERIC_READ | GENERIC_WRITE,
                                    // Access (read/write) mode
                      0,            // Share mode
                      NULL,         // Pointer to the security attribute
                      OPEN_EXISTING,// How to open the serial port
                      FILE_FLAG_OVERLAPPED,			//FILE_FLAG_OVERLAPPED,            // Port attributes
                      NULL);        // Handle to port with attribute
                                    // to copy


  // If it fails to open the port, return FALSE.
  if ( hPort == INVALID_HANDLE_VALUE ) 
  {
    // Could not open the port.
//    MessageBox (hMainWnd, TEXT("Unable to open the port"), 
//                TEXT("Error"), MB_OK);
//	char Buff[30];
//	sprintf(Buff,"Invalid Handle for port: %s"), m_PortName);
  // 	MessageBox(NULL, Buff, "Error"), MB_OK);
    dwError = GetLastError ();
    return FALSE;
  }

  PortDCB.DCBlength = sizeof (DCB);     

  // Get the default port setting information.
  GetCommState (hPort, &PortDCB);

  // Change the DCB structure settings.
  PortDCB.BaudRate = m_iBaudRate;              // Current baud 
  PortDCB.fBinary = TRUE;               // Binary mode; no EOF check 
  PortDCB.fParity = TRUE;               // Enable parity checking. 
  PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control 
  PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control 
  PortDCB.fDtrControl = DTR_CONTROL_ENABLE; 
                                        // DTR flow control type 
  PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity 
  PortDCB.fTXContinueOnXoff = TRUE;     // XOFF continues Tx 
  PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control 
  PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control 
  PortDCB.fErrorChar = FALSE;           // Disable error replacement. 
  PortDCB.fNull = FALSE;                // Disable null stripping. 
  PortDCB.fRtsControl = RTS_CONTROL_ENABLE; 
                                        // RTS flow control 
  PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on 
                                        // error.
  PortDCB.ByteSize = m_iByteSize;                 // Number of bits/bytes, 4-8 
  PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space 
  PortDCB.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2 

  // Configure the port according to the specifications of the DCB 
  // structure.
  if (!SetCommState (hPort, &PortDCB))
  {
    // Could not create the read thread.
//    MessageBox (hMainWnd, TEXT("Unable to configure the serial port"), 
//                TEXT("Error"), MB_OK);

	CString Buff;
	Buff.Format(_T("Unable to configure the serial port: %s"), m_PortName);
   	MessageBox( NULL, Buff, L"Error", MB_OK);
    dwError = GetLastError ();
    return FALSE;
  }

  // Retrieve the time-out parameters for all read and write operations
  // on the port. 
  GetCommTimeouts (hPort, &CommTimeouts);

  // Change the COMMTIMEOUTS structure settings.
  CommTimeouts.ReadIntervalTimeout = 1;  
  CommTimeouts.ReadTotalTimeoutMultiplier = 0;  
  CommTimeouts.ReadTotalTimeoutConstant = 0;    
  CommTimeouts.WriteTotalTimeoutMultiplier = 10;  
  CommTimeouts.WriteTotalTimeoutConstant = 1000;    

 
  // Set the time-out parameters for all read and write operations
  // on the port. 
  if (!SetCommTimeouts (hPort, &CommTimeouts))
  {
    // Could not create the read thread.
//    MessageBox (hMainWnd, TEXT("Unable to set the time-out parameters"), 
 //            
	

	CString Buff;
	Buff.Format(_T("Unable to set the time-out parameters: %s"), m_PortName);
//   	MessageBox(NULL, Buff, "Error"), MB_OK);
    dwError = GetLastError ();
    return FALSE;
  }

  // Direct the port to perform extended functions SETDTR and SETRTS.
  // SETDTR: Sends the DTR (data-terminal-ready) signal.
  // SETRTS: Sends the RTS (request-to-send) signal. 
  EscapeCommFunction (hPort, SETDTR);
  EscapeCommFunction (hPort, SETRTS);

  // Create a read thread for reading data from the communication port.

	SAFE_DELETE(m_RxBuffer);
	m_nLastReadPtr=m_nRxPtr=0;
	m_RxBuffer = new char[BUFFER_LENGTH];
	ZeroMemory(m_RxBuffer,BUFFER_LENGTH);

	m_bThreadEnabled = TRUE;
	m_pThread = AfxBeginThread(SerialPortOverlappedReadThread,this);


  return TRUE;
}


#define READ_TIMEOUT      1      // milliseconds

UINT SerialPortOverlappedReadThread (LPVOID lpvoid)
{
	DWORD dwBytesTransferred;

	CSerial* pComs = (CSerial *) lpvoid;
	BOOL bWaitingOnRead = FALSE;
	OVERLAPPED osReader = {0};
	DWORD dwRes;
	BYTE	Array[100];

	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL)
		return -1;	// Error creating overlapped event; abort.
		
	SetCommMask (pComs->hPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_RLSD | EV_RING);

	pComs->m_bThreadFinished=FALSE;
	while (pComs->m_bThreadEnabled == TRUE) {
		if (!bWaitingOnRead) {
			if (!ReadFile(pComs->hPort, Array, 1, &dwBytesTransferred, &osReader)) {
				if (GetLastError() != ERROR_IO_PENDING) {    // read not delayed?
				 // Error in communications; report it.
				} else {
					bWaitingOnRead = TRUE;
				}
			} else {
				pComs->ByteSuccessfullyRead( Array,dwBytesTransferred);
			}
		}
		if (bWaitingOnRead) {
			dwRes = WaitForSingleObject(osReader.hEvent, READ_TIMEOUT);
			switch(dwRes)  {
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(pComs->hPort, &osReader, &dwBytesTransferred, FALSE)) {
					 // Error in communications; report it.
				} else {
					if(dwBytesTransferred>0) pComs->ByteSuccessfullyRead( Array, dwBytesTransferred);
						bWaitingOnRead = FALSE;
				}
				break;
				case WAIT_TIMEOUT:
				  // Operation isn't complete yet. fWaitingOnRead flag isn't
				  // changed since I'll loop back around, and I don't want
				  // to issue another read until the first one finishes.
				  //
				  // This is a good time to do some background work.
				break;                       
				default:
				MessageBox(NULL,_T("ERROR"),_T("ERROR"),NULL);
				  // Error in the WaitForSingleObject; abort.
				  // This indicates a problem with the OVERLAPPED structure's
				  // event handle.
				break;
			}
		}
	}

	CloseHandle(osReader.hEvent);
	pComs->m_bThreadFinished=TRUE;

	return 0;
}

void CSerial::WriteByte(char Byte)
{
	WriteBytes(&Byte,1);
}


void CSerial::WriteBytes(char* bytes, int length)
{
   OVERLAPPED osWrite = {0};
  DWORD dwNumBytesWritten;
   DWORD dwRes;
  bool bResult;

     // Create this write operation's OVERLAPPED structure's hEvent.
   osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (osWrite.hEvent == NULL)
      // error creating overlapped event handle
      return ;

  if (!WriteFile (hPort, bytes, length, &dwNumBytesWritten, &osWrite))    {
       if (GetLastError() != ERROR_IO_PENDING) { 
         // WriteFile failed, but isn't delayed. Report error and abort.
         bResult = FALSE;
      } else {
         // Write is pending.
         dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
         switch(dwRes)
         {
            // OVERLAPPED structure's event has been signaled. 
            case WAIT_OBJECT_0:
                 if (!GetOverlappedResult(hPort, &osWrite, &dwNumBytesWritten, FALSE))
                       bResult = FALSE;
                 else
                  // Write operation completed successfully.
                  bResult = TRUE;
                 break;
            
            default:
                 // An error has occurred in WaitForSingleObject.
                 // This usually indicates a problem with the
                // OVERLAPPED structure's event handle.
                 bResult = FALSE;
                 break;
         }
      }
   } else {
      // WriteFile completed immediately.
      bResult = TRUE;
   }

  CloseHandle(osWrite.hEvent);

}

void CSerial::WriteBytes(unsigned char* bytes, int length)
{
   OVERLAPPED osWrite = {0};
  DWORD dwNumBytesWritten;
   DWORD dwRes;
  bool bResult;

     // Create this write operation's OVERLAPPED structure's hEvent.
   osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (osWrite.hEvent == NULL)
      // error creating overlapped event handle
      return ;

  if (!WriteFile (hPort, bytes, length, &dwNumBytesWritten, &osWrite))    {
       if (GetLastError() != ERROR_IO_PENDING) { 
         // WriteFile failed, but isn't delayed. Report error and abort.
         bResult = FALSE;
      } else {
         // Write is pending.
         dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
         switch(dwRes)
         {
            // OVERLAPPED structure's event has been signaled. 
            case WAIT_OBJECT_0:
                 if (!GetOverlappedResult(hPort, &osWrite, &dwNumBytesWritten, FALSE))
                       bResult = FALSE;
                 else
                  // Write operation completed successfully.
                  bResult = TRUE;
                 break;
            
            default:
                 // An error has occurred in WaitForSingleObject.
                 // This usually indicates a problem with the
                // OVERLAPPED structure's event handle.
                 bResult = FALSE;
                 break;
         }
      }
   } else {
      // WriteFile completed immediately.
      bResult = TRUE;
   }

  CloseHandle(osWrite.hEvent);

}




void CSerial::WriteStr(char *string)
{
	unsigned int	ii;

	for(ii=0;ii<strlen(string);ii++) {
		WriteByte(string[ii]);
	};
}


void CSerial::ByteSuccessfullyRead(BYTE* pArray,DWORD dwBytesTransfered)
{
	if(m_RxBuffer==NULL) return;

	for(int nn=0;nn<(int)dwBytesTransfered;nn++)  {
		m_RxBuffer[m_nRxPtr]=pArray[nn];
		m_nRxPtr++;
		m_nRxPtr%=BUFFER_LENGTH;
		if(pArray[nn] == '\r') m_bLF = true;
		if(pArray[nn] == '\n') m_bCR = true;
	}
}

bool CSerial::IStringReceived()
{
	if(m_bLF || m_bCR) return true;
	return false;
}

bool CSerial::Hit()
{

	if(m_bThreadEnabled == false) return false;
	if((m_nRxPtr - m_nLastReadPtr) == 0) return false;

	return true;
}

bool CSerial::GetStr(CString *pBuff)
{

	int nCount = m_nRxPtr - m_nLastReadPtr;
	if(nCount<0) {
		nCount += BUFFER_LENGTH;
	}
	if( nCount ) {

		pBuff->GetBufferSetLength(nCount);
		for(int ii=0;ii<nCount;ii++) {
			pBuff->SetAt(ii,m_RxBuffer[m_nLastReadPtr]);
			if(m_RxBuffer[m_nLastReadPtr] == '\r') m_bLF = false;
			if(m_RxBuffer[m_nLastReadPtr] == '\n') m_bCR = false;

			m_nLastReadPtr++;
			m_nLastReadPtr %= BUFFER_LENGTH;
		}

		return true;
	}

	return false;


}

bool CSerial::SendStr(CString *pBuff)
{
	char str[256];
	int nn;

	for(nn=0;nn<pBuff->GetLength();nn++) {
		str[nn]=(char)pBuff->GetAt(nn);
	}
//	str[nn++]='\r';
//	str[nn++]='\n';
	str[nn]=0;
	WriteStr(str);

	return true;
}
