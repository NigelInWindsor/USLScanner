// LazerSerialComms.h: interface for the CLazerSerialComms class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAZERSERIALCOMMS_H__CD1162B6_E889_4D41_9E00_B94C0E5D5D15__INCLUDED_)
#define AFX_LAZERSERIALCOMMS_H__CD1162B6_E889_4D41_9E00_B94C0E5D5D15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#ifndef SIO_UTIL_H
//#define SIO_UTIL_H

// hard coded maximum number of ports for device under Win32

#define MAXPORTS        4


// terminal size

#define MAXROWS         25
#define MAXCOLS         80

#define MAXBLOCK        1000

#define MAXLEN_TEMPSTR  81

#define RXQUEUE         4096
#define TXQUEUE         4096

// cursor states

#define CS_HIDE         0x00
#define CS_SHOW         0x01

// Flow control flags

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ascii definitions

#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13


// data structures

typedef struct
{
   HANDLE  idComDev ;
   DCB	   portDCB;
   BYTE    bPort, abScreen[ MAXROWS * MAXCOLS ] ;
   BOOL    fConnected, fXonXoff, fLocalEcho, fNewLine, fAutoWrap,
           fUseCNReceive, fDisplayErrors;
   BYTE    bByteSize, bFlowCtrl, bParity, bStopBits ;
   DWORD   dwBaudRate ;
   WORD    wCursorState ;
   HFONT   hTTYFont ;
   LOGFONT lfTTYFont ;
   DWORD   rgbFGColor ;
   int     xSize, ySize, xScroll, yScroll, xOffset, yOffset,
           nColumn, nRow, xChar, yChar ;
   HANDLE      hPostEvent, hWatchThread, hWatchEvent ;
   HWND        hTermWnd ;
   DWORD       dwThreadID ;
   OVERLAPPED  osWrite, osRead ;

} COMMINFO;


// macros ( for easier readability )

#define GWL_NPTTYINFO 0

#define GETHINST( x )  ((HINSTANCE) GetWindowLong( x, GWL_HINSTANCE ))
#define GETNPTTYINFO( x ) ((NPTTYINFO) GetWindowLong( x, GWL_NPTTYINFO ))
#define SETNPTTYINFO( x, y ) SetWindowLong( x, GWL_NPTTYINFO, (LONG) y )

#define COMDEV( x ) (x -> idComDev)
#define PORT( x )   (x -> bPort)
//#define SCREEN( x ) (x -> abScreen)
#define CONNECTED( x ) (x -> fConnected)
#define XONXOFF( x ) (x -> fXonXoff)
#define LOCALECHO( x ) (x -> fLocalEcho)
//#define NEWLINE( x ) (x -> fNewLine)
#define AUTOWRAP( x ) (x -> fAutoWrap)
#define BYTESIZE( x ) (x -> bByteSize)
#define FLOWCTRL( x ) (x -> bFlowCtrl)
#define PARITY( x ) (x -> bParity)
#define STOPBITS( x ) (x -> bStopBits)
#define BAUDRATE( x ) (x -> dwBaudRate)
#define CURSORSTATE( x ) (x -> wCursorState)
#define HTTYFONT( x ) (x -> hTTYFont)
#define LFTTYFONT( x ) (x -> lfTTYFont)
#define FGCOLOR( x ) (x -> rgbFGColor)
#define XSIZE( x ) (x -> xSize)
#define YSIZE( x ) (x -> ySize)
#define XSCROLL( x ) (x -> xScroll)
#define YSCROLL( x ) (x -> yScroll)
#define XOFFSET( x ) (x -> xOffset)
#define YOFFSET( x ) (x -> yOffset)
#define COLUMN( x ) (x -> nColumn)
#define ROW( x ) (x -> nRow)
#define XCHAR( x ) (x -> xChar)
#define YCHAR( x ) (x -> yChar )
#define USECNRECEIVE( x ) (x -> fUseCNReceive)
#define DISPLAYERRORS( x ) (x -> fDisplayErrors)

#define POSTEVENT( x ) (x -> hPostEvent)
#define TERMWND( x ) (x -> hTermWnd)
#define HTHREAD( x ) (x -> hWatchThread)
#define THREADID( x ) (x -> dwThreadID)
#define WRITE_OS( x ) (x -> osWrite)
#define READ_OS( x ) (x -> osRead)

#define SET_PROP( x, y, z )  SetProp( x, MAKEINTATOM( y ), z )
#define GET_PROP( x, y )     GetProp( x, MAKEINTATOM( y ) )
#define REMOVE_PROP( x, y )  RemoveProp( x, MAKEINTATOM( y ) )

// purge control flags
#define PURGE_RX (PURGE_RXABORT | PURGE_RXCLEAR) 
#define PURGE_TX (PURGE_TXABORT | PURGE_TXCLEAR)

//Functions used internally in Serial I/O functions
LRESULT NEAR CreateCommInfo(COMMINFO *pCommInfo);
BOOL NEAR SetupConnection(COMMINFO *pCommInfo, int baudrate);
BOOL NEAR WriteCommBlock(COMMINFO *pCommInfo, LPCTSTR lpByte , DWORD dwBytesToWrite);

// user defined messages

#define CONTROLLER_RESPONSE (WM_USER+1)


// application constants

#define CTLLR_OUTPUT_EOL (0x0a)
#define SENSOR_OUTPUT_EOL (0x0a)

#define CTLLR_PROMPT_LEN 3
#define CTLLR_PROMPT_1 'Y'
#define CTLLR_PROMPT_2 '>'
#define CTLLR_PROMPT_3 ' '

#define ASCII_TIMEOUT	(0xffffffL)
#define BINARY_TIMEOUT	(0xffffffL)

#define ASCII_TIMEOUT_INTERVAL	10
#define BINARY_TIMEOUT_INTERVAL	10


class CLazerSerialComms  
{
public:

	HANDLE StageReady, SensorDataRcvd, DataRcvd, DataCopied;
	char LastSensorByte;
	char ControllerResponse[150], SensorResponse[150];

	char EncoderPosn[50];
	//FILE *SensorDataFileHandle;
	COMMINFO SensorInfo, StageInfo;

	BOOL OpenPort( COMMINFO *pCommInfo, int baudrate, int Portnum);
	unsigned int ReadPort( COMMINFO  *pCommInfo, LPSTR lpszBlock, unsigned int nMaxLength, BOOL wait );
	BOOL WritePort( COMMINFO *pCommInfo, LPCTSTR lpByte , DWORD dwBytesToWrite );
	BOOL ClosePort( COMMINFO *pCommInfo );
	BOOL PurgePort( COMMINFO *pCommInfo, int flags );
	int  sendstr( COMMINFO *pPort, TCHAR str[] );

	int  sendbinary( COMMINFO *pPort, unsigned char *str, int nbytes );
	int  ascii_read(COMMINFO *pPort, char *buf, int numBytes);				// read numBytes bytes
	unsigned int ascii_read_line(COMMINFO *pCommInfo, char *buf, unsigned int nMaxLength, BOOL crlf, int timeout);
	unsigned int binary_read_bytes(COMMINFO *pCommInfo, char *buf, unsigned int nMaxLength, int timeout);
	BOOL SendSensorCmd(COMMINFO *pPort, char * command);
	LRESULT CreateCommInfo(COMMINFO *pCommInfo);
	BOOL SetupConnection(COMMINFO *pCommInfo, int baudrate);

	CLazerSerialComms();
	virtual ~CLazerSerialComms();

};

extern void SensorApplicationAction(), ControllerApplicationAction();

#endif // !defined(AFX_LAZERSERIALCOMMS_H__CD1162B6_E889_4D41_9E00_B94C0E5D5D15__INCLUDED_)
