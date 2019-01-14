// SafetySerial.cpp: implementation of the CSafetySerial class.
//
//////////////////////////////////////////////////////////////////////

/*
Outputs

	#	| Complex		| Immersion
	1	| Servo Brakes	| X Brakes
	2	| Stepper Brakes| Z Brakes
	3	| Port Air		|
	4	| Starboard Air	|


Inputs

  ??????????????


*/
#include "stdafx.h"
#include "uslscanner.h"
#include "SafetySerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSafetySerial::CSafetySerial()
{
	m_bDevicePresent = false;
	m_bThreadEnabled = false;
	m_hSemaphore = false;
	m_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
	SetEvent(m_hSemaphore);

}

CSafetySerial::~CSafetySerial()
{
	SuspendThread();
}


bool CSafetySerial::FindDevice()
{
	WCHAR str[10];
	int nn;

	for(int ii=1;ii<3;ii++) {
		swprintf_s(str,10,_T("Com%d\0"),ii);
		if(Initialize(str)) {
			m_bDevicePresent=true;
			StartReadThread(); //Not needed, read on spec
			StartOuputWriteThread(); //Ouput requires writing to as least once per minute otherwise reverts o default
			GetOutputPort(nn);	//Initialize m_nOutputPort to reduce overhead on setting brakes
			return true;
		}

	}

	return false;
}


void CSafetySerial::ZBrakesEnable(int bOffOn, int nWhich)
{
	
	CString Buff;
/*
	if(!m_bDevicePresent)
		return;

	int	nn;

	while(!GetOutputPort(nn)){Sleep(500);}

	switch(nWhich) {
	case 1:
		switch(bOffOn) {
		case FALSE:
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);		theApp.m_PmacUser.SendStr(Buff);
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);	theApp.m_PmacUser.SendStr(Buff);
			nn |= theApp.m_Tank.nBrakeBit0;
			break;
		case TRUE:
			nn &= (theApp.m_Tank.nBrakeBit0 ^ 0xff);
			break;
		}
		break;
	case 0:
		switch(bOffOn) {
		case FALSE:
			nn |= theApp.m_Tank.nBrakeBit1;
			break;
		case TRUE:
			nn &= (theApp.m_Tank.nBrakeBit1 ^ 0xff);
			break;
		}
		break;
	}

	SetOutputPort(nn);

  

	return;
*/
	if(!m_bDevicePresent)
		return;

	int	nn;

//	while(!GetOutputPort(nn)){Sleep(500);}
	nn = m_nOutputPort;

	switch(nWhich) {
	case 0:
		switch(bOffOn) {
		case FALSE: 
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);		theApp.m_PmacUser.SendStr(Buff);
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);	theApp.m_PmacUser.SendStr(Buff);
			nn |= theApp.m_Tank.nBrakeBit0;
			break;
		case TRUE:
			nn &= ~theApp.m_Tank.nBrakeBit0;
			break;
		}
		break;
	case 1:
		switch(bOffOn) {
		case FALSE: 
			nn |= theApp.m_Tank.nBrakeBit1;
			break;
		case TRUE:
			nn &= ~theApp.m_Tank.nBrakeBit1;
			break;
		}
		break;
	}

	if(nn - m_nOutputPort) {
		SetOutputPort(nn);
		Sleep(200);
	};



}



int CSafetySerial::GetInputPort()
{
	if(!m_bDevicePresent)
		return -1;

	
	BYTE msg[10] = {0x00,0x00,0x45,0x44,0x76,0x00,0x00,0x36,0xFF,0x4F};
	BYTE rtnmsg[200];
	int nLen, nRetVal;

	if(WriteThenRead(msg, 10, rtnmsg, nLen))
		nRetVal = rtnmsg[34]&0xFF; //Data Byte 0 is the output Port (First 10 bytes make up header)


	//Update PMAC;
	theApp.m_PmacUser.UpdateSafetyUnitInputs(nRetVal);	
	theApp.m_PmacUser.m_nGeneralInput = nRetVal;

	return nRetVal;

}


BOOL CSafetySerial::GetOutputPort(int &nValue)
{
	if(!m_bDevicePresent)
		return -1;


	BYTE msg[10] = {0x00,0x00,0x45,0x44,0x76,0x00,0x00,0x36,0xFF,0x4F};
	BYTE rtnmsg[200];
	int nLen;


	if(WriteThenRead(msg, 10, rtnmsg, nLen)){
		nValue = rtnmsg[25]&0xFF; //Data Byte 15 is the output Port (First 10 bytes make up header)
		m_nOutputPort = nValue;
		return true;
	}

	return false;

}


bool CSafetySerial::SetOutputPort(int nValue)
{
	if(!m_bDevicePresent)
		return false;

	BYTE msg[22] = {0x00,0x00,0x41,0x44,0x42,0x00,0x00,0x06,0xFF,0x4F,0x42,0x00,0x00,0x06,0xFF,0x4F,0xDD,0xDD,0xDD,0xDD,0xCC,0xCC}; //DD=Data CC=Checksum
	BYTE checksum[2];
	BYTE rtnmsg[200];
	int nLen;


	msg[16] = (nValue&0xFF);
	msg[17] = (nValue&0xFF00)>>8;
	msg[18] = (nValue&0xFF0000)>>16;
	msg[19] = (nValue&0xFF000000)>>24;

	GetChecksum(msg,20,checksum);

	msg[20] = checksum[0];
	msg[21] = checksum[1];

	WriteThenRead(msg, 22, rtnmsg, nLen);

	m_nOutputPort = nValue;

	//evaluate replay here

	return true;
}


int CSafetySerial::GetBrakeStatus(int nWhich)
{
	
	int nFlag=false;

	switch(nWhich) {
	case 0:
		if(m_nOutputPort&theApp.m_Tank.nBrakeBit0) 
			nFlag = 1;  //disengaged
		break;
	case 1:
		if(m_nOutputPort&theApp.m_Tank.nBrakeBit1)
			nFlag = 1;
		break;
	}

	return nFlag;

}

bool CSafetySerial::ReportAirStatus(int nMask)
{
	bool bFlag;

	(m_nOutputPort & nMask) ? bFlag = TRUE : bFlag = FALSE;

	return bFlag;

}

void CSafetySerial::AirStart(int nMask)
{
	int nn;
	while(!GetOutputPort(nn)){Sleep(500);}
	nn |= nMask;
	SetOutputPort(nn);

}

void CSafetySerial::AirStop(int nMask)
{
	int nn;
	while(!GetOutputPort(nn)){Sleep(500);}
	nn &= ~nMask;
	SetOutputPort(nn);
}

bool CSafetySerial::StartReadThread()
{

	if(m_bThreadEnabled==false) {
		m_bThreadEnabled=true;
		m_pThread=AfxBeginThread(&SafetySerialReadThread,this,THREAD_PRIORITY_NORMAL ,0,NULL);
	};

	return true;
}

bool CSafetySerial::StartOuputWriteThread()
{

	if(m_bOutputThreadEnabled==false) {
		m_bOutputThreadEnabled=true;
		m_pOutputThread=AfxBeginThread(&SafetySerialOutputThread,this,THREAD_PRIORITY_NORMAL ,0,NULL);
	};

	return true;
}

void CSafetySerial::SuspendThread()
{
	if(m_bThreadEnabled==TRUE) {
		int nRet = WaitForSingleObject(m_hSemaphore, 1000);
		m_bThreadEnabled = FALSE;
		WaitForSingleObject(&SafetySerialReadThread,INFINITE);
	};

	if(m_bOutputThreadEnabled==TRUE) {
		int nRet = WaitForSingleObject(m_hSemaphore, 1000);
		m_bOutputThreadEnabled=FALSE;
		WaitForSingleObject(&SafetySerialOutputThread,INFINITE);
	};
}

bool CSafetySerial::Initialize (LPTSTR lpszPortName)
{
	DCB PortDCB;
	COMMTIMEOUTS CommTimeouts;

	wcscpy_s(m_PortName, 10, lpszPortName);

	// Open the serial port.
	hPort = CreateFile (lpszPortName, // Pointer to the name of the port
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
		return FALSE;
	}

	PortDCB.DCBlength = sizeof (DCB);     

	// Get the default port setting information.
	GetCommState (hPort, &PortDCB);

	// Change the DCB structure settings.
	PortDCB.BaudRate = CBR_115200;					//DONE
	PortDCB.fBinary = TRUE;               
	PortDCB.fParity = FALSE;			///CHANGED				               
	PortDCB.fOutxCtsFlow = FALSE;          
	PortDCB.fOutxDsrFlow = FALSE;          
	PortDCB.fDtrControl = DTR_CONTROL_ENABLE; 
										
	PortDCB.fDsrSensitivity = FALSE;      
	PortDCB.fTXContinueOnXoff = TRUE;     
	PortDCB.fOutX = FALSE;                
	PortDCB.fInX = FALSE;                
	PortDCB.fErrorChar = FALSE;          
	PortDCB.fNull = FALSE;               
	PortDCB.fRtsControl = RTS_CONTROL_ENABLE; 
										
	PortDCB.fAbortOnError = FALSE;        
										
	PortDCB.ByteSize = 8;						//DONE
	PortDCB.Parity = NOPARITY;					//DONE
	PortDCB.StopBits = ONESTOPBIT;				//DONE 

	// Configure the port according to the specifications of the DCB 
	// structure.
	if (!SetCommState (hPort, &PortDCB))
	{
		CString Buff;
		Buff.Format(_T("Unable to configure the serial port: %s"), m_PortName);
		return FALSE;
	}

	// Retrieve the time-out parameters for all read and write operations
	// on the port. 
	GetCommTimeouts (hPort, &CommTimeouts);

	// Change the COMMTIMEOUTS structure settings.
	CommTimeouts.ReadIntervalTimeout = -1;  
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;  
	CommTimeouts.ReadTotalTimeoutConstant = 0;    
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;  
	CommTimeouts.WriteTotalTimeoutConstant = 0;    


	// Set the time-out parameters for all read and write operations
	// on the port. 
	if (!SetCommTimeouts (hPort, &CommTimeouts))
	{
		CString Buff;
		Buff.Format(_T("Unable to set the time-out parameters: %s"), m_PortName);
		return FALSE;
	}

	SetupComm(hPort,4000,4000);
	PurgeComm(hPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);


	return ReadDeviceID(lpszPortName);

}

bool CSafetySerial::WriteThenRead(BYTE *lpWriteBuff, DWORD dwToWrite, BYTE *lpReadBuf, int &nLen)
{

	bool retval=false;
	BYTE rtnmsg[300];

	ResetEvent(m_hSemaphore);

	try{
		m_CriticalSection.Lock();
	}catch(...){
		return false;
	}


	if (WriteABuffer(lpWriteBuff, dwToWrite)) {
		Sleep(200);
		if (ReadABuffer(rtnmsg, nLen)) {
			if (nLen < 300) {
				if (ValidateMsg(rtnmsg, nLen)) {
					retval = true;
				}
			}
		}
	}

	if (retval == true) {
		memcpy(lpReadBuf, rtnmsg, nLen);
	}

	m_CriticalSection.Unlock();

	SetEvent(m_hSemaphore);


	return retval;

}


bool CSafetySerial::ValidateMsg(BYTE *lpBuf, int nLen)
{

	if((nLen<9)&&(nLen!=4)) //Send reply always 4 bytes fetch reply always greater than 9
		return false;

	if(*lpBuf!=0)			//telegram identifier, always 0
		return false;

	
	if(*(lpBuf+1)!=0)		//telegram identifier, always 0
		return false;

	
	if(*(lpBuf+2)!=0)		//telegram type, always 0
		return false;


	if((*(lpBuf+3))!=0)		//Error, non 0 indicates error
		return false;


	return true;

}


bool CSafetySerial::ReadDeviceID(LPTSTR lpszPortName) 
{
	BYTE msg[10] = {0x00,0x00,0x45,0x44,0x07,0x00,0x00,0x0D,0xFF,0x4F};
	BYTE rtnmsg[50];
	int nLen;

	WriteThenRead(msg, 10, rtnmsg, nLen);

	if(nLen!=30)
		return false;

	//Process rtn messgae
	if((nLen>3)&&(rtnmsg[3]!=0)) { 
		MessageBox(NULL,_T("Safety Unit Comms Error: 0x01"),_T("Error"),NULL);
		return false;
	}

	//Actual device address = rtnmsg[9]

	//data is rtnmsg[10] to rtnmsg[nLen-2]
	m_nSafetyLevel = (rtnmsg[10]&0xF0)>>4;
	m_nModuleType = (rtnmsg[11]&0xFF);
	m_nModuleDiagnosisID = (rtnmsg[17]&0xFF);
	m_nSerialNumberYear = (rtnmsg[24]&0xFF);
	m_nSerialNumberWeek = (rtnmsg[25]&0xFF);
	m_nSerialNumberWithinWeek = ((rtnmsg[26]&0xFF) | ((rtnmsg[27]&0xFF)<<8));
	swscanf_s(m_PortName,_T("Com%d"),&m_nPortNumber);


	m_DeviceID.Format(_T("%s CPU%d Year%d Week%d #%d"),lpszPortName,m_nModuleType,m_nSerialNumberYear,m_nSerialNumberWeek,m_nSerialNumberWithinWeek);

	return true;
}



bool CSafetySerial::GetChecksum(BYTE *pMsg,int nLen, BYTE *pChecksum)
{

	WORD CRC = -1;
	int ii=0;


	for(ii=10;ii<20;ii++) {
		CRC = (CRC<<8) ^ (crc_table[ (CRC>>8) ^ (*(pMsg+ii))]);
	}

	*pChecksum = CRC&0xFF;
	*(pChecksum+1) = (CRC>>8)&0xFF;

	return true;
}



//Should use WriteThenRead Function as require a lock
bool CSafetySerial::WriteABuffer(BYTE * lpBuf, DWORD dwToWrite)
{
   OVERLAPPED osWrite = {0};
   DWORD dwWritten;
   DWORD dwRes;
   bool fRes;

   // Create this write operation's OVERLAPPED structure's hEvent.
   osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (osWrite.hEvent == NULL)
      // error creating overlapped event handle
      return FALSE;

   // Issue write.
   if (!WriteFile(hPort, lpBuf, dwToWrite, &dwWritten, &osWrite)) {
      if (GetLastError() != ERROR_IO_PENDING) { 
         // WriteFile failed, but isn't delayed. Report error and abort.
         fRes = FALSE;
      } else {
         // Write is pending.
         dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
         switch(dwRes)
         {
            // OVERLAPPED structure's event has been signaled. 
            case WAIT_OBJECT_0:
                 if (!GetOverlappedResult(hPort, &osWrite, &dwWritten, FALSE))
                       fRes = FALSE;
                 else
                  // Write operation completed successfully.
                  fRes = TRUE;
                 break;
            
            default:
                 // An error has occurred in WaitForSingleObject.
                 // This usually indicates a problem with the
                // OVERLAPPED structure's event handle.
                 fRes = FALSE;
                 break;
         }
      }
   } else {
      // WriteFile completed immediately.
      fRes = TRUE;
   }

   
   CloseHandle(osWrite.hEvent);

   return fRes;
}

//Should use WriteThenRead Function as require a lock
#define READ_TIMEOUT      1      // milliseconds
bool CSafetySerial::ReadABuffer(BYTE *lpBuf, int &nLen)
{

	COMSTAT ComState;
	DWORD dwErrors, dwBytesTransferred;
	OVERLAPPED osReader = {0};
	nLen=0;
	int nTimeoutPeriod = 1200; //ms
	clock_t StartTime = clock();
	clock_t CurrentTime = clock();
	bool bReturn = false;
	bool bWaitingOnRead=false;
	DWORD dwRes;

	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL)
		return false;	// Error creating overlapped event; abort.

	SetCommMask (hPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_RLSD | EV_RING);


	//Attempt to listen for data for timeout period
	while((((CurrentTime - StartTime)/CLOCKS_PER_SEC)*1000) < nTimeoutPeriod) {

		int gg= CLOCKS_PER_SEC;
		ClearCommError(hPort,&dwErrors,&ComState);
  
	//	if(Errors)
	//		do something

		//We have data to be read
		while(ComState.cbInQue>0) {
			if(!bWaitingOnRead) {
				if(ReadFile(hPort, lpBuf+nLen, ComState.cbInQue, &dwBytesTransferred, &osReader)) {
					Sleep(10); //allow following extra data to be transferred into buffer
					nLen+=dwBytesTransferred;
					ClearCommError(hPort,&dwErrors,&ComState);
					bReturn=true;
				}else{
					if (GetLastError() != ERROR_IO_PENDING) {    // read not delayed?
					 // Error in communications; report it.
					} else {
						bWaitingOnRead = TRUE;
					}
				}
			}else{
				dwRes = WaitForSingleObject(osReader.hEvent, READ_TIMEOUT);
				switch(dwRes)  {
				case WAIT_OBJECT_0:
					if (!GetOverlappedResult(hPort, &osReader, &dwBytesTransferred, FALSE)) {
						 // Error in communications; report it.
					} else {
						Sleep(10); //allow following extra data to be transferred into buffer
						nLen+=dwBytesTransferred;
						ClearCommError(hPort,&dwErrors,&ComState);
						bReturn=true;
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

		if(bReturn) {
			CloseHandle(osReader.hEvent);
			return true;
		}

		CurrentTime = clock();
	}


	CloseHandle(osReader.hEvent);
	return bReturn;

}


/* This is not needed as the saefty unit omn responds to our messages */


UINT SafetySerialReadThread (LPVOID lpvoid)
{

	CSafetySerial* pComs = (CSafetySerial *) lpvoid;

	pComs->m_bThreadFinished=false;
	while (pComs->m_bThreadEnabled == true) {
		Sleep(2000);
		pComs->m_GeneralInputs = pComs->GetInputPort();
	}

	pComs->m_bThreadFinished=true;

	return 0;
}

/* We need to continually write to safety unit otherwise it will go to defaults */

UINT SafetySerialOutputThread (LPVOID lpvoid)
{

	CSafetySerial* pComs = (CSafetySerial *) lpvoid;
	int nValue;

	pComs->m_bOutputThreadFinished=false;
	while (pComs->m_bOutputThreadEnabled == true) {
		Sleep(10000);
		while(!pComs->GetOutputPort(nValue)){Sleep(500);}
		pComs->SetOutputPort(nValue);
	}

	pComs->m_bOutputThreadFinished=true;

	return 0;
}