// LaserRangefinder.cpp: implementation of the CLaserRangefinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "LaserRangefinder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CLaserRangefinder::CLaserRangefinder() : CSerial()
{
	m_pParam = new THREADSTRUCT;
	m_pParam->_this = this;
	m_bCR=false;
	m_bLF=false;
	m_sendBuff  = new char[100];
	ResetErrorCode();
	SetRunningContinuously(false);
}

CLaserRangefinder::~CLaserRangefinder()
{
	SuspendThread();
	delete[] m_sendBuff;
	delete m_pParam;
}

void CLaserRangefinder::ResetErrorCode(){
	m_Crit.Lock();
	m_errorCode = 0;
	m_Crit.Unlock();
}

int CLaserRangefinder::GetLastLaserError(){
	m_Crit.Lock();
	int retval = m_errorCode;
	m_Crit.Unlock();
	return retval;
}

char* CLaserRangefinder::GetBytesSent(int* nBytes){
	m_Crit.Lock();
	*nBytes = m_nBytesSent; 
	m_Crit.Unlock();
	return m_sendBuff;
}

CString CLaserRangefinder::GetReceived(){
	m_Crit.Lock();
	CString retval = m_receiveBuff;
	m_Crit.Unlock();
	return retval;
}

CString CLaserRangefinder::GetData(){
	m_Crit.Lock();
	CString retval = m_sData;
	m_Crit.Unlock();
	return retval;
}


void CLaserRangefinder::WriteBytes(char* bytes, int length){
	m_Crit.Lock();
	for(int i=0; i<length; i++){
		m_sendBuff[i] = bytes[i];
	}
	m_nBytesSent=length;
	CSerial::WriteBytes(m_sendBuff, m_nBytesSent);
	m_Crit.Unlock();
}

void CLaserRangefinder::WriteBytes(BYTE* bytes, int length){
	m_Crit.Lock();
	for(int i=0; i<length; i++){
		m_sendBuff[i] = bytes[i];
	}
	m_nBytesSent=length;
	CSerial::WriteBytes(m_sendBuff, m_nBytesSent);
	m_Crit.Unlock();
}

bool CLaserRangefinder::Read(){
	if(IStringReceived()){
		GetStr(&m_receiveBuff);
		HandleMessage(m_receiveBuff);
		return true;
	}
	return false;
}

bool CLaserRangefinder::Start(){
 	return CSerial::Start(m_sPort);
}

bool CLaserRangefinder::Close(){
	return CSerial::Close();
}

float CLaserRangefinder::GetLastDistance(){
	float retval;
	m_Crit.Lock();
	retval = m_fDistance;
	m_Crit.Unlock();
	return retval;
}

bool CLaserRangefinder::Initialize (CString strPortName)
{
  DWORD dwError;
//DWORD dwThreadID;
  DCB PortDCB;
  COMMTIMEOUTS CommTimeouts;
	m_bThreadEnabled = FALSE;

  wcscpy_s(m_PortName, strPortName);

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
  //PortDCB.fBinary = TRUE;               // Binary mode; no EOF check 
 // PortDCB.fParity = TRUE;               // Enable parity checking. 
//  PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control 
//  PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control 
//  PortDCB.fDtrControl = DTR_CONTROL_ENABLE; 
                                        // DTR flow control type 
 // PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity 
//  PortDCB.fTXContinueOnXoff = TRUE;     // XOFF continues Tx 
//  PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control 
//  PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control 
//  PortDCB.fErrorChar = FALSE;           // Disable error replacement. 
//  PortDCB.fNull = FALSE;                // Disable null stripping. 
//  PortDCB.fRtsControl = RTS_CONTROL_ENABLE; 
                                        // RTS flow control 
//  PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on 
                                        // error.
  PortDCB.ByteSize = m_iByteSize;                 // Number of bits/bytes, 4-8 
  PortDCB.Parity = EVENPARITY;            // 0-4=no,odd,even,mark,space 
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
//	m_pThread = AfxBeginThread(SerialPortOverlappedReadThread,this);
	m_pThread = AfxBeginThread(SerialPortReadThread, m_pParam);

  return TRUE;
}

#define READ_TIMEOUT 1

UINT CLaserRangefinder::SerialPortReadThread (LPVOID pParam)
{
	THREADSTRUCT* ts = (THREADSTRUCT*) pParam;
	DWORD dwBytesTransferred;

	CLaserRangefinder* pComs = ts->_this;
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

void CLaserRangefinder::SuspendThread()
{
	if (m_bThreadEnabled == TRUE) {
		m_bThreadEnabled = FALSE;
		while (m_bThreadFinished == FALSE) {
			Sleep(1);
		};
		WaitForSingleObject(m_pThread, INFINITE);
	};

}

void CLaserRangefinder::ByteSuccessfullyRead(BYTE* pArray,DWORD dwBytesTransfered)
{
	m_Crit.Lock();
	if(m_RxBuffer==NULL) return;

	for(int nn=0;nn<(int)dwBytesTransfered;nn++)  {
		m_RxBuffer[m_nRxPtr]=pArray[nn];
		TestEndOfLineByte(pArray[nn]);
		m_nRxPtr++;
		m_nRxPtr%=BUFFER_LENGTH;
	}
	Read();
	m_Crit.Unlock();
}

void CLaserRangefinder::TestEndOfLineByte(BYTE val){
		if(val == '\r') {
			m_bCR = true;
		}
		if(val == '\n'){ 
			m_bLF = true;
		}
}

void CLaserRangefinder::ResetEndOfLineByte(BYTE val){
		if(val == '\r') {
			m_bCR = false;
		}
		if(val == '\n'){ 
			m_bLF = false;
		}
}



bool CLaserRangefinder::IStringReceived()
{
	bool result=false;
	m_Crit.Lock();
	if(m_bCR && m_bLF) {
		result=true;
	}
	m_Crit.Unlock();
	return result;
}

bool CLaserRangefinder::GetStr(CString *pBuff)
{
	m_Crit.Lock();
	bool result = false;
	int nCount = m_nRxPtr - m_nLastReadPtr;
	if(nCount<0) {
		nCount += BUFFER_LENGTH;
	}
	if( nCount ) {

		pBuff->GetBufferSetLength(nCount);
		for(int ii=0;ii<nCount;ii++) {
			pBuff->SetAt(ii,m_RxBuffer[m_nLastReadPtr]);
			ResetEndOfLineByte(m_RxBuffer[m_nLastReadPtr]);
			m_nLastReadPtr++;
			m_nLastReadPtr %= BUFFER_LENGTH;
		}

		result=true;
	}

	result=false;
	m_Crit.Unlock();
	return result;
}

bool CLaserRangefinder::IsRunningContinuously(){
	m_Crit.Lock();
	bool retval = m_bRunningContinuously;
	m_Crit.Unlock();
	return retval;
}

void CLaserRangefinder::SetRunningContinuously(bool val){
	m_Crit.Lock();
	m_bRunningContinuously=val;
	m_Crit.Unlock();
}

void CLaserRangefinder::SetPort(CString sPort){
	m_sPort = sPort;
}