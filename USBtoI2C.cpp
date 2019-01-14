// USBtoI2C.cpp: implementation of the CUSBtoI2C class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "uslscanner.h"
#include "USBtoI2C.h"
#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <comdef.h>
#include <winspool.h>
//#include <Wbemidl.h>




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


short _stdcall Inp32(short PortAddress);
void _stdcall Out32(short PortAddress, short data);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUSBtoI2C::CUSBtoI2C()
{
	nMsgL=1;
	Msg[0]=0x55;
	m_bDevicePresent=false;
	m_bMailboxFailed=false;

}

CUSBtoI2C::~CUSBtoI2C()
{


}


int CUSBtoI2C::FindDevice()
{
	//ToDo
	//Only Way I can find to do this is to install windows SDK
	//I don't believe its worth it.
	return 0;

}


void CUSBtoI2C::StartDriver()
{
	
	WCHAR str[20];

	if(Inp32(0x378)!=0xFF){
		Out32(0x37A,0x00);
		Out32(0x378,0xFF);
		Sleep(7000);
	}

	swprintf_s(str,_T("Com%d"),theApp.m_LastSettings.nUSBI2CComNum);
	SetupCommPort(str);

	m_bDevicePresent = true;

}


void CUSBtoI2C::SetupCommPort( LPTSTR ComPort)
{
	DCB dcb;
	COMMTIMEOUTS ct;

	CloseHandle(m_hCom);
	m_hCom = CreateFile(ComPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	GetCommState(m_hCom, &dcb);
	dcb.BaudRate = CBR_19200;
	dcb.fParity = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fAbortOnError = FALSE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = TWOSTOPBITS;
	SetCommState(m_hCom, &dcb);
  
	GetCommTimeouts(m_hCom, &ct);
	ct.ReadIntervalTimeout = 500;
    ct.ReadTotalTimeoutMultiplier =500; 
    ct.ReadTotalTimeoutConstant = 500; 
	SetCommTimeouts(m_hCom, &ct);

	SetCommMask(m_hCom, EV_RXCHAR);
}



int CUSBtoI2C::MailboxTx(BYTE nValue)
{
	DWORD n;
	int nLoc;
	BYTE bb[3];
	int nMailboxCount=0;

	if(m_bMailboxFailed)
		return -1;

	if(nValue==0xFF){
		//Send Message Buffer
		Msg[3]=nMsgL-3;  
		Msg[1]=Msg[1]<<1; 
		
		do {
			WriteFile(m_hCom, &Msg, nMsgL+1, &n, NULL);
			ReadFile(m_hCom, &bb, 1, &n, NULL);
			nMailboxCount++;
			if(nMailboxCount>10){
				MessageBox(NULL,_T("Mailbox has failed"),_T("Error"),NULL);
				m_bMailboxFailed=true;
				return -1;
			}
		} while(n<1);

		nMsgL=1;

	}else{
		//Add to MessageBuffer;
		if(nMsgL<3){
			nLoc=nMsgL;
		}else{
			nLoc=nMsgL+1;
		}

		Msg[nLoc]=nValue;
		nMsgL++;
	}

	return 1;

}

int CUSBtoI2C::ReadTS0()
{
	int ii;
	ii = Inp32(0x379);

	ii = ii>>7;
	ii = ii^0x01;

	return ii;
}

void CUSBtoI2C::Reset()
{

	Out32(0x378,0x00);//Set Reset Line to Low
	Sleep(200);
	Out32(0x378,0xFF);//Set Reset Line to Low
	Sleep(7000);

}

