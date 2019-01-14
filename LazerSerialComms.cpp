// LazerSerialComms.cpp: implementation of the CLazerSerialComms class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "LazerSerialComms.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLazerSerialComms::CLazerSerialComms()
{

}

CLazerSerialComms::~CLazerSerialComms()
{

}

BOOL CLazerSerialComms::OpenPort(COMMINFO *pCommInfo, int baudrate, int Portnum)
{            
   BOOL       fRetVal, Success ;
   COMMTIMEOUTS  CommTimeOuts ;
   TCHAR        szPort[ 10 ] ;

   wsprintf(szPort,_T("COM%d"),Portnum);


   // Open Comm Device

   if ((COMDEV( pCommInfo ) =
      CreateFile( szPort, GENERIC_READ | GENERIC_WRITE,
                  0,                    // exclusive access
                  NULL,                 // no security attrs
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL | 
                  FILE_FLAG_OVERLAPPED, // overlapped I/O
                  NULL )) == (HANDLE) -1 )
      return ( FALSE ) ;
   else
   {
      // get any early notifications

      Success = SetCommMask( COMDEV( pCommInfo ), EV_RXCHAR ) ;

      // setup device buffers

      Success = SetupComm( COMDEV( pCommInfo ), 4096, 4096 ) ;

      // purge any information in the buffer
	  Success = PurgePort(pCommInfo, PURGE_TX | PURGE_RX);
      
	  // set up for overlapped I/O

	  //modified to never return until operation is complete

//      CommTimeOuts.ReadIntervalTimeout = 3000;
//	  CommTimeOuts.ReadTotalTimeoutMultiplier = 3000 ;
//    CommTimeOuts.ReadTotalTimeoutConstant = 3000;

	  // Return immediately with chars that have been rcvd so far
	  // will not timeout

      CommTimeOuts.ReadIntervalTimeout		  = MAXDWORD;
	  CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
      CommTimeOuts.ReadTotalTimeoutConstant	  = 0;
      
	  CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
      CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;
      Success = SetCommTimeouts( COMDEV( pCommInfo ), &CommTimeOuts ) ;
   }

   fRetVal = SetupConnection(pCommInfo, baudrate) ;

   if (fRetVal)
   {
      CONNECTED( pCommInfo ) = TRUE ;

	  // assert DTR
   EscapeCommFunction( COMDEV( pCommInfo ), SETDTR ) ;
   }
   else
   {
      CONNECTED( pCommInfo ) = FALSE ;
      CloseHandle( COMDEV( pCommInfo ) ) ;
   }

         // purge any information in the buffer
   Success = PurgePort(pCommInfo, PURGE_TX | PURGE_RX);
      

   return ( fRetVal ) ;

} // end of OpenPort()


BYTE       DataBuf[ 2*MAXBLOCK + 1] ;
int		BytesAvail;


//---------------------------------------------------------------------------
//  int  ReadPort( COMMINFO pCommInfo, LPSTR lpszBlock, int nMaxLength )
//
//  Description:
//     Reads a block from the COM port and stuffs it into
//     the provided buffer.
//
//  Parameters:
//     
//     LPSTR lpszBlock
//        block used for storage
//
//     int nMaxLength
//        max length of block to read
//
//  Win-32 Porting Issues:
//     - ReadComm() has been replaced by ReadFile() in Win-32.
//     - Overlapped I/O has been implemented.
//
//---------------------------------------------------------------------------


unsigned int CLazerSerialComms::ReadPort(	COMMINFO  *pCommInfo, LPSTR lpszBlock, unsigned int nMaxLength, BOOL wait )
{
	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	unsigned int     dwLength = 0;
	DWORD	   bytesread = 0;
	unsigned int	totalbytesread = 0;
	TCHAR       szError[ 10 ] ;
	int timeouts = 0;

	// Max Sleep count (10ms)

	while(totalbytesread < nMaxLength) {
		ClearCommError( COMDEV( pCommInfo ), &dwErrorFlags, &ComStat ) ;

		// only try to read number of bytes in queue 
		dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;
		if(dwLength == 0)
		{
			Sleep(10);
			if(timeouts++ > 600)	// 6 Seconds worth between characters? exit
				return ASCII_TIMEOUT;
		}
		else
		{
			timeouts = 0;
		}
		if(!wait)
			nMaxLength = dwLength;

		if(dwLength != 0) {
			fReadStat = ReadFile( COMDEV( pCommInfo ), lpszBlock+totalbytesread,
								dwLength, &bytesread, &READ_OS( pCommInfo ) ) ;
			totalbytesread += bytesread;
			if (!fReadStat || totalbytesread != nMaxLength)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					Sleep(200); // Cant get ReadFile or GetOverlappedResult to wait
				}
				else
				{
					// some other error occurred
		
					dwLength = 0 ;
					ClearCommError( COMDEV( pCommInfo ), &dwErrorFlags, &ComStat ) ;
					if ((dwErrorFlags > 0) && DISPLAYERRORS( pCommInfo ))
					{
						wsprintf( szError, _T("<CE-%u>"), dwErrorFlags ) ;
					}
				}
			}
		}
	}   
   return ( dwLength ) ;

} // end of ReadPort()





//---------------------------------------------------------------------------
//  BOOL WritePort()
//
//  Description:
//     Writes a block of data to the COM port specified in the associated
//     TTY info structure.
//
//  Parameters:
//   
//
//     BYTE *pByte
//        pointer to data to write to port
//
//  Win-32 Porting Issues:
//     - WriteComm() has been replaced by WriteFile() in Win-32.
//     - Overlapped I/O has been implemented.
//
//---------------------------------------------------------------------------

BOOL CLazerSerialComms::WritePort( COMMINFO *pCommInfo, LPCTSTR lpByte , DWORD dwBytesToWrite)
{

	BOOL        fWriteStat ;
	DWORD       dwBytesWritten ;
	DWORD       dwErrorFlags;
	DWORD   	dwError;
	COMSTAT     ComStat;
	TCHAR        szError[ 10 ] ;

	fWriteStat = WriteFile( COMDEV( pCommInfo ), lpByte, dwBytesToWrite,
	                       &dwBytesWritten, &WRITE_OS( pCommInfo ) ) ;

	// Note that normally the code will not execute the following
	// because the driver caches write operations. Small I/O requests 
	// (up to several thousand bytes) will normally be accepted 
	// immediately and WriteFile will return true even though an
	// overlapped operation was specified

	if (!fWriteStat) 
	{
		char *lpMessageBuffer;
		if(dwError = GetLastError()) 
			FormatMessage(
			  FORMAT_MESSAGE_ALLOCATE_BUFFER |
			  FORMAT_MESSAGE_FROM_SYSTEM,
			  NULL,
			  dwError,
			  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //The user default language
			  (LPTSTR) &lpMessageBuffer,
			  0,
			  NULL );

		if(dwError == ERROR_IO_PENDING)
		{
			// We should wait for the completion of the write operation
			// so we know if it worked or not

			// This is only one way to do this. It might be beneficial to 
			// the to place the writing operation in a separate thread 
			// so that blocking on completion will not negatively 
			// affect the responsiveness of the UI

			// If the write takes long enough to complete, this 
			// function will timeout according to the
			// CommTimeOuts.WriteTotalTimeoutConstant variable.
			// At that time we can check for errors and then wait 
			// some more.

			while(!GetOverlappedResult( COMDEV( pCommInfo ),
				&WRITE_OS( pCommInfo ), &dwBytesWritten, TRUE ))
			{
				dwError = GetLastError();
				if(dwError == ERROR_IO_INCOMPLETE)
					// normal result if not finished
					continue;
				else
				{
					// an error occurred, try to recover
					wsprintf( szError, _T("<CE-%u>"), dwError ) ;
					ClearCommError( COMDEV( pCommInfo ), &dwErrorFlags, &ComStat ) ;
					if ((dwErrorFlags > 0) && DISPLAYERRORS( pCommInfo ))
					{
						wsprintf( szError, _T("<CE-%u>"), dwErrorFlags ) ;
					}
					break;
				}
			}
		}
		else
		{						 	
			// some other error occurred

			ClearCommError( COMDEV( pCommInfo ), &dwErrorFlags, &ComStat ) ;
			if ((dwErrorFlags > 0) && DISPLAYERRORS( pCommInfo ))
			{
				wsprintf( szError, _T("<CE-%u>"), dwErrorFlags ) ;
			}
			return ( FALSE );
		}
	}
	return ( TRUE ) ;

} // end of WritePort()


//---------------------------------------------------------------------------
//  BOOL ClosePort()
//
//  Description:
//     Closes the connection to the port.  Resets the connect flag
//     in the TTYINFO struct.
//
//  Parameters:
//    
//
//  Win-32 Porting Issues:
//     - Needed to stop secondary thread.  SetCommMask() will signal the
//       WaitCommEvent() event and the thread will halt when the
//       CONNECTED() flag is clear.
//     - Use new PurgeComm() API to clear communications driver before
//       closing device.
//
//---------------------------------------------------------------------------


BOOL CLazerSerialComms::ClosePort( COMMINFO *pCommInfo )
{
//   HMENU      hMenu ;
 
   // set connected flag to FALSE

   CONNECTED( pCommInfo ) = FALSE ;

   // disable event notification and wait for thread
   // to halt

   SetCommMask( COMDEV( pCommInfo ), 0 ) ;

   // block until thread has been halted

	// separate threads not used in general utilities   while(THREADID(pCommInfo) != 0);

   // drop DTR
   EscapeCommFunction( COMDEV( pCommInfo ), CLRDTR ) ;

   // purge any outstanding reads/writes and close device handle

   PurgeComm( COMDEV( pCommInfo ), PURGE_TXABORT | PURGE_RXABORT |
                                   PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
   // Close the comm port.
   CloseHandle( COMDEV( pCommInfo ) ) ;

   return ( TRUE ) ;

} // end of ClosePort()


BOOL PurgePort(COMMINFO *pCommInfo, int flags)
{
	BOOL Success;

    Success = PurgeComm( COMDEV( pCommInfo ), flags) ;
	return(Success);
}


/////////////////////////////////////////////////////////////////////////////////////////
// Specialized functions for sending and receiving ascii strings and binary data with 
// a checksum
//
// Read one line of text from the serial port up to the first newline character or
// CR/LF pair if enabled
//
// Return number of bytes read, or ASCII_TIMEOUT
unsigned int CLazerSerialComms::ascii_read_line(COMMINFO *pCommInfo, char *buf, unsigned int nMaxLength, BOOL crlf, int timeout)
{
	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	unsigned int     dwLength = 0;
	DWORD	   bytesread = 0;
	unsigned int	totalbytesread = 0;
	TCHAR       szError[ 10 ] ;
	int		timeout_count = 0;
	char * pstr;
	int return_read = 0;
	

	// wait in intervals of 


	while(totalbytesread < nMaxLength) {
		ClearCommError( COMDEV( pCommInfo ), &dwErrorFlags, &ComStat ) ;

		// only try to read number of bytes in queue 
		dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;
		if(dwLength == 0)
		{
			Sleep(ASCII_TIMEOUT_INTERVAL);
			timeout_count += ASCII_TIMEOUT_INTERVAL;
			if(timeout_count > timeout)
				return ASCII_TIMEOUT;
		}
		else
		{
			timeout_count = 0;
		}
//		if(!wait)
//			nMaxLength = dwLength;

		if(dwLength != 0) {
			pstr = buf+totalbytesread;
			fReadStat = ReadFile( COMDEV( pCommInfo ), pstr,
								1, &bytesread, &READ_OS( pCommInfo ) ) ;
			totalbytesread += bytesread;
			return_read += bytesread;
			if (!fReadStat || (bytesread != 1))
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					Sleep(200); // Cant get ReadFile or GetOverlappedResult to wait
				}
				else
				{
					// some other error occurred
					dwLength = 0;
					ClearCommError( COMDEV( pCommInfo ), &dwErrorFlags, &ComStat ) ;
					if ((dwErrorFlags > 0) && DISPLAYERRORS( pCommInfo ))
					{
						wsprintf( szError, _T("<CE-%u>"), dwErrorFlags ) ;
					}
				}
			}
			else // error didn't occur, check for newline
			{
				if( *pstr == 0x0a)
				{
					*pstr = 0;
					return_read--;
					return return_read;
				}
				if( crlf && *pstr == 0x0d)
				{
					*pstr = 0;
					return_read--;
				}

			}

		}
	}   
   return ( dwLength ) ;
}



/////////////////////////////////////////////////////////////////////////////////////////
// Specialized functions for sending and receiving ascii strings and binary data 
// with a checksum
//
// Read up to nMaxLength binary characters from the serial port, or until timeout
//
// Return number of bytes read, or BINARY_TIMEOUT

unsigned int CLazerSerialComms::binary_read_bytes(COMMINFO *pCommInfo, char *buf, unsigned int nMaxLength, int timeout)
{
	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	unsigned int     dwLength = 0;
	DWORD	   bytesread = 0;
	unsigned int	totalbytesread = 0;
	TCHAR       szError[ 10 ] ;
	int		timeout_count = 0;
	char * pstr;
	

	// wait in intervals of 


	while(totalbytesread < nMaxLength) {
		ClearCommError( COMDEV( pCommInfo ), &dwErrorFlags, &ComStat ) ;

		// only try to read number of bytes in queue 
		dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;
		if(dwLength == 0)
		{
			Sleep(BINARY_TIMEOUT_INTERVAL);
			timeout_count += BINARY_TIMEOUT_INTERVAL;
			if(timeout_count > timeout)
				return BINARY_TIMEOUT;
		}
		else
		{
			timeout_count = 0;
		}
//		if(!wait)
//			nMaxLength = dwLength;

		if(dwLength != 0) {
			pstr = buf+totalbytesread;
			fReadStat = ReadFile( COMDEV( pCommInfo ), pstr,
								1, &bytesread, &READ_OS( pCommInfo ) ) ;
			totalbytesread += bytesread;
			if (!fReadStat || (bytesread != 1))
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					Sleep(200); // Cant get ReadFile or GetOverlappedResult to wait
				}
				else
				{
					// some other error occurred
					dwLength = 0;
					ClearCommError( COMDEV( pCommInfo ), &dwErrorFlags, &ComStat ) ;
					if ((dwErrorFlags > 0) && DISPLAYERRORS( pCommInfo ))
					{
						wsprintf( szError, _T("<CE-%u>"), dwErrorFlags ) ;
					}
				}
			}
			else // error didn't occur, check for newline
			{
				if(totalbytesread == nMaxLength)
				{
					return totalbytesread;
				}
			}

		}
	}   
   return ( dwLength ) ;
}


int CLazerSerialComms::ascii_read(COMMINFO *pPort, char *buf, int numBytes)	/* read until the next LF */
{
	int read_bytes = 0;

	read_bytes = ReadPort(pPort, buf, numBytes, TRUE );
	return read_bytes;
}

int CLazerSerialComms::sendstr(COMMINFO *pPort, TCHAR str[])
{
	int len;

	len = wcslen(str);
	return(WritePort(pPort, str, len));
	
}

int CLazerSerialComms::sendbinary(COMMINFO *pPort, unsigned char *str, int nbytes)
{
	int i;
	unsigned int checksum = 0;

	for(i=0;i < nbytes; i++)
		checksum += str[i];
	WritePort(pPort, (const unsigned short *)str, nbytes);
	
	return(checksum);

}




BOOL CLazerSerialComms::SendSensorCmd(COMMINFO *pPort, char * command)
{

	if(! WritePort(&SensorInfo, (LPCTSTR)command, strlen(command)))
		return(FALSE);

	return(TRUE);
}




//////////////////////////////////////////////////////////////////////////////////////
// Internal functions from here down: Interface Functions above.

// CreateCommInfo
// Called by OpenPort to initialize all the arcane aspects of the port

LRESULT CLazerSerialComms::CreateCommInfo(COMMINFO *pCommInfo)
{

   // initialize TTY info structure

//   COMDEV( pCommInfo )        = 0;
   CONNECTED( pCommInfo )     = TRUE ;
   CURSORSTATE( pCommInfo )   = CS_HIDE ;
   LOCALECHO( pCommInfo )     = FALSE ;
   AUTOWRAP( pCommInfo )      = TRUE ;
   PORT( pCommInfo )          = 1 ;
   BAUDRATE( pCommInfo )      = CBR_9600 ;
   BYTESIZE( pCommInfo )      = 8 ;
   FLOWCTRL( pCommInfo )      = FC_RTSCTS ;
   FLOWCTRL( pCommInfo )      = FC_XONXOFF ;
   PARITY( pCommInfo )        = NOPARITY ;
   STOPBITS( pCommInfo )      = ONESTOPBIT ;
   XONXOFF( pCommInfo )       = FALSE ;
   XSIZE( pCommInfo )         = 0 ;
   YSIZE( pCommInfo )         = 0 ;
   XSCROLL( pCommInfo )       = 0 ;
   YSCROLL( pCommInfo )       = 0 ;
   XOFFSET( pCommInfo )       = 0 ;
   YOFFSET( pCommInfo )       = 0 ;
   COLUMN( pCommInfo )        = 0 ;
   ROW( pCommInfo )           = 0 ;
   HTTYFONT( pCommInfo )      = NULL ;
   FGCOLOR( pCommInfo )       = RGB( 0, 0, 0 ) ;
   USECNRECEIVE( pCommInfo )  = TRUE ;
   DISPLAYERRORS( pCommInfo ) = TRUE ;
   WRITE_OS( pCommInfo ).Offset = 0 ;
   WRITE_OS( pCommInfo ).OffsetHigh = 0 ;
   READ_OS( pCommInfo ).Offset = 0 ;
   READ_OS( pCommInfo ).OffsetHigh = 0 ;

   // create I/O event used for overlapped reads / writes

   READ_OS( pCommInfo ).hEvent = CreateEvent( NULL,    // no security
                                              TRUE,    // explicit reset req
                                              FALSE,   // initial event reset
                                              NULL ) ; // no name
   if (READ_OS( pCommInfo ).hEvent == NULL)
   {
      LocalFree( pCommInfo ) ;
      return ( -1 ) ;
   }
   WRITE_OS( pCommInfo ).hEvent = CreateEvent( NULL,    // no security
                                               TRUE,    // explicit reset req
                                               FALSE,   // initial event reset
                                               NULL ) ; // no name
   if (NULL == WRITE_OS( pCommInfo ).hEvent)
   {
      CloseHandle( READ_OS( pCommInfo ).hEvent ) ;
      LocalFree( pCommInfo ) ;
      return ( -1 ) ;
   }

   return ( (LRESULT) TRUE ) ;

} // end of CreateCommInfo()


//---------------------------------------------------------------------------
//  BOOL NEAR SetupConnection()
//
//  Description:
//     This routines sets up the DCB based on settings in the
//     TTY info structure and performs a SetCommState().
//
//  Parameters:
//     HWND hWnd
//        handle to TTY window
//
//  Win-32 Porting Issues:
//     - Win-32 requires a slightly different processing of the DCB.
//       Changes were made for configuration of the hardware handshaking
//       lines.
//
//---------------------------------------------------------------------------


BOOL CLazerSerialComms::SetupConnection(COMMINFO *pCommInfo, int baudrate)
{
   BOOL       fRetVal ;
	char *lpMessageBuffer;
	DCB *pDCB = &(pCommInfo->portDCB);

   CreateCommInfo(pCommInfo);
   fRetVal= GetCommState( COMDEV( pCommInfo ), pDCB ) ;

   		pDCB->BaudRate = baudrate;
		pDCB->fBinary = 1;
		pDCB->Parity = FALSE;
		pDCB->fOutxCtsFlow = pCommInfo->bFlowCtrl;
		pDCB->fOutxDsrFlow = pCommInfo->bFlowCtrl;
		pDCB->fDtrControl = DTR_CONTROL_DISABLE;
		pDCB->fDsrSensitivity = FALSE;
		pDCB->fTXContinueOnXoff = FALSE;
		pDCB->fOutX = !pCommInfo->bFlowCtrl; // if hardware fctl in operation do not use
		pDCB->fInX = !pCommInfo->bFlowCtrl;
		pDCB->fErrorChar = FALSE;
		pDCB->fNull = FALSE;
		pDCB->fRtsControl = RTS_CONTROL_DISABLE;
		pDCB->fAbortOnError = FALSE;
		pDCB->XonLim = 10;	
		pDCB->XoffLim = 10;
		pDCB->ByteSize = 8;
		pDCB->Parity = NOPARITY;
		pDCB->StopBits = ONESTOPBIT;
		pDCB->XonChar = 17; // ^Q = XON;
		pDCB->XoffChar = 19; // ^S = XOFF;
  

   fRetVal = SetCommState( COMDEV( pCommInfo ), pDCB ) ;
   	if(!fRetVal) {
 
			FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //The user default language
			(LPTSTR) &lpMessageBuffer,
			0,
			NULL );
			LocalFree(lpMessageBuffer);
	}

   return ( fRetVal ) ;

} // end of SetupConnection()



