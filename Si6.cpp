// Si6.cpp: implementation of the CSi6 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Si6.h"
#include "MainFrm.h"
#include "winioctl.h"
#include <math.h>
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const BYTE	PARPORT_DATA	= 0;
const BYTE	PARPORT_STATUS	= 1;
const BYTE	PARPORT_CONTROL	= 2;



CSi6::CSi6()
{ 
	m_Si6.pAxis[0] = &theApp.m_Axes[theApp.m_Tank.nXStep];
	m_Si6.pAxis[1] = &theApp.m_Axes[theApp.m_Tank.nYStep];
	m_Si6.pAxis[2] = &theApp.m_Axes[theApp.m_Tank.nZStep];
	m_Si6.pAxis[3] = &theApp.m_Axes[theApp.m_Tank.nXtStep];
	m_Si6.pAxis[4] = &theApp.m_Axes[theApp.m_Tank.nYtStep];
	m_Si6.pAxis[5] = &theApp.m_Axes[23];



}

CSi6::~CSi6()
{
	DisableInterrupts();
	Sleep(20);
	FreeAndDeallocateDataMemory();
	FreeTimeSlotStruct();
	FreeUserSi6Struct();
	if( !CloseHandle(m_hSi6))	{
	//	MessageBox(NULL,_T("Failed to Deallocate SI6 Device Driver"),"Error"),NULL);
	};

}

bool CSi6::StartDriver()
{
	GetHandleToDevice(2000);
	if( m_hSi6==INVALID_HANDLE_VALUE) {
		MessageBox(NULL,_T("Failed to Allocate Hardware Resource\r\n SI6 Device Driver"),_T("Error"),NULL);
		return FALSE;
	} else {
		char	Version[40];
		GetVersion(Version,sizeof Version);
		if(strcmp(Version,SI6_VERSION)) {
			MessageBox(NULL,_T("Incorrect Version of SI6 driver"),_T("Error"),NULL);
			if( !CloseHandle(m_hSi6))	{
				MessageBox(NULL,_T("Failed to Deallocate  Device Driver"),_T("Error"),NULL);
			};
			m_hSi6=NULL;
			return FALSE;
		} else {
	//		MessageBox(NULL,Version,"Ok"),NULL);
			m_Si6.nAddrs=0x00;
			m_Si6.nJobNumber=theApp.m_nJobNumber;

			ShareUserSi6Struct();
			ShareTimeSlotStruct();
			EnableInterrupts(); //It is better to enable it after having loaded the AxisData from the Harddrive
			return TRUE;
		};
	}


}


void CSi6::Initialize()
{
	for(int ii=0;ii<AXISNUMBER;ii++) {
		EncoderResetMode(&theApp.m_Axes[ii]);
	};
//	EnableInterrupts();
}


BOOL CSi6::GetHandleToDevice(int OS)
{
	WCHAR str[256];

//	theApp.m_LastSettings.Si6Res.nIOAddrs=0;
	if(theApp.m_Si6Res.nIOAddrs<0x100) {
		switch(theApp.m_nJobNumber) {
		default:
			theApp.m_Si6Res.nIOAddrs=0x140;
			theApp.m_Si6Res.nIORange=0x20;
			theApp.m_Si6Res.nMemAddrs=0xd9000;
			theApp.m_Si6Res.nMemRange=0x100;
			theApp.m_Si6Res.nIrq=3;
			break;
		case CTG_COMPOSITES:
			theApp.m_Si6Res.nIOAddrs=0x180;
			theApp.m_Si6Res.nIORange=0x20;
			theApp.m_Si6Res.nMemAddrs=0xd0000;
			theApp.m_Si6Res.nMemRange=0x100;
			theApp.m_Si6Res.nIrq=7;
			break;
		}
	};
	swprintf_s(str,_T("\\\\.\\Si6\\isa\\io%x,%x\\mem%x,%x\\irq%d\\override"),
		theApp.m_Si6Res.nIOAddrs,theApp.m_Si6Res.nIORange,
		theApp.m_Si6Res.nMemAddrs,theApp.m_Si6Res.nMemRange,
		theApp.m_Si6Res.nIrq);


	m_hSi6=NULL;
	switch(OS) {
	case 95:
	case 98:
//		m_hSi6 = GetDeviceViaInterface((LPGUID)&SI6_GUID,0);
		break;
	case 2000:
		m_hSi6 = CreateFile(str, GENERIC_READ|GENERIC_WRITE, 0,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//		m_hSi6 = CreateFile("\\\\.\\Si6\\isa\\io180,20\\memd9000,100\\irq3\\override"), GENERIC_READ|GENERIC_WRITE, 0,
//			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	};
	if( m_hSi6==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	return TRUE;
}


///////////////////////////////////////////////////////////
//Windows 95 Windows 98

HANDLE CSi6::GetDeviceViaInterface( GUID* pGuid, DWORD instance)
{

/*	
	// Get handle to relevant device information set
	HDEVINFO info = SetupDiGetClassDevs(pGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
	if(info==INVALID_HANDLE_VALUE)
	{
		printf("No HDEVINFO available for this GUID\n");
		return NULL;
	}

	// Get interface data for the requested instance
	SP_INTERFACE_DEVICE_DATA ifdata;
	ifdata.cbSize = sizeof(ifdata);
	if(!SetupDiEnumDeviceInterfaces(info, NULL, pGuid, instance, &ifdata))
	{
		printf("No SP_INTERFACE_DEVICE_DATA available for this GUID instance\n");
		SetupDiDestroyDeviceInfoList(info);
		return NULL;
	}

	// Get size of symbolic link name
	DWORD ReqLen;
	SetupDiGetDeviceInterfaceDetail(info, &ifdata, NULL, 0, &ReqLen, NULL);
	PSP_INTERFACE_DEVICE_DETAIL_DATA ifDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA)(new char[ReqLen]);
	if( ifDetail==NULL)
	{
		SetupDiDestroyDeviceInfoList(info);
		return NULL;
	}

	// Get symbolic link name
	ifDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
	if( !SetupDiGetDeviceInterfaceDetail(info, &ifdata, ifDetail, ReqLen, NULL, NULL))
	{
		SetupDiDestroyDeviceInfoList(info);
		delete ifDetail;
		return NULL;
	}

//	printf("Symbolic link is %s\n"),ifDetail->DevicePath);
	// Open file
	HANDLE rv = CreateFile( ifDetail->DevicePath, 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( rv==INVALID_HANDLE_VALUE) rv = NULL;

	delete ifDetail;
	SetupDiDestroyDeviceInfoList(info);
	m_hSi6 = rv;
	return rv;
*/
	return 0;
}

WORD CSi6::Inp(int nOffset)
{
	BYTE	ReadInput[3]={SI6_READ, (BYTE)nOffset};
	DWORD BytesReturned;
	WORD rv[3]={0xffff,0xffff,0xffff};

	BOOL result=DeviceIoControl(m_hSi6, IOCTL_SI6_RUN_CMDS,
								ReadInput, 2,			// Input
								rv, sizeof(rv),					// Output
								&BytesReturned, NULL);
	if(result!=1) {
		ASSERT(TRUE);
	}
	return rv[2] & 0x00ff;

}

WORD CSi6::Inpw(int nOffset)
{
	BYTE	ReadInput[3]={SI6_READ, (BYTE)nOffset};
	DWORD BytesReturned;
	WORD rv[3]={0xffff,0xffff,0xffff};

	BOOL result=DeviceIoControl(m_hSi6, IOCTL_SI6_RUN_CMDS,
								ReadInput, 2,			// Input
								rv, sizeof(rv),					// Output
								&BytesReturned, NULL);
	return rv[2];

}
void CSi6::Outp(int nOffset, int nValue)
{
	BYTE	WriteOutput[3]={SI6_WRITE, (BYTE)nOffset, (BYTE)(nValue & 0xff)};
	DWORD BytesReturned;
	WORD rv[3]={0xffff,0xffff,0xffff};

	BOOL result=DeviceIoControl(m_hSi6, IOCTL_SI6_RUN_CMDS,
								WriteOutput, 3,			// Input
								rv, sizeof(rv),					// Output
								&BytesReturned, NULL);

}
void CSi6::GetVersion(char *Version,int Length)
{
	DWORD BytesReturned;

	Version[0]=0;
	BOOL result = DeviceIoControl( m_hSi6, IOCTL_SI6_GET_VERSION,
							NULL, 0,			// Input
							Version, Length,	// Output
							&BytesReturned, NULL);

}

void CSi6::GetVersion(CString* Version)
{
	char str[256];

	GetVersion(str,sizeof str);
	Version->Format(_T("%s"),str);
}


BOOL CSi6::UserSi6PtrToDriver()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hSi6, IOCTL_SI6_SEND_PTR_USER_SI6,
							&m_Si6, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}


void CSi6::EnableInterrupts()
{

	DWORD BytesReturned;
	WORD rv[8];

	m_Si6.nIrqMask=0xfe;
	BYTE ConnectToInterrupts[] ={
		(BYTE)SI6_WRITE, (BYTE)(SI6_CLK_ENABLE+m_Si6.nAddrs), (BYTE)0xff,
		(BYTE)SI6_WRITE, (BYTE)(SI6_PIA+3+m_Si6.nAddrs), (BYTE)0x9b,					// All ports are input
		(BYTE)SI6_WRITE, (BYTE)(SI6_PIC+1+m_Si6.nAddrs),(BYTE)m_Si6.nIrqMask,
		(BYTE)SI6_WRITE, (BYTE)(SI6_PIC+0+m_Si6.nAddrs),(BYTE)0x17,
		(BYTE)SI6_WRITE, (BYTE)(SI6_PIC+1+m_Si6.nAddrs),(BYTE)0x08,
		(BYTE)SI6_WRITE, (BYTE)(SI6_PIC+1+m_Si6.nAddrs),(BYTE)0x03,
		(BYTE)SI6_IRQ_CONNECT, (BYTE)0x00, (BYTE)0x00, (BYTE)0x00,	// Connect to interrupt
	};

	if( int nn=DeviceIoControl( m_hSi6, IOCTL_SI6_RUN_CMDS,
							ConnectToInterrupts, sizeof ConnectToInterrupts,
							rv, sizeof(rv),								
							&BytesReturned, NULL)){
	}

}

void CSi6::DisableInterrupts()
{
	BYTE DisableInterrupts[] =
	{
		SI6_WRITE, PARPORT_CONTROL, 0xCC,			// Disable interrupts
	};
	WORD rv[3];
	DWORD BytesReturned;

	Outp(m_Si6.nAddrs+SI6_PIC+1,m_Si6.nIrqMask=0xff);	//Mask all interrupts

	BOOL result = DeviceIoControl( m_hSi6, IOCTL_SI6_RUN_CMDS,
							DisableInterrupts, sizeof DisableInterrupts,	// Input
							rv, sizeof(rv),									// Output
							&BytesReturned, NULL);
							
}

BOOL CSi6::ShareUserSi6Struct()
{
	DWORD BytesReturned;

	m_Si6.pAxis[0] = &theApp.m_Axes[theApp.m_Tank.nXStep];
	m_Si6.pAxis[1] = &theApp.m_Axes[theApp.m_Tank.nYStep];
	m_Si6.pAxis[2] = &theApp.m_Axes[theApp.m_Tank.nZStep];
	m_Si6.pAxis[3] = &theApp.m_Axes[theApp.m_Tank.nXtStep];
	m_Si6.pAxis[4] = &theApp.m_Axes[theApp.m_Tank.nYtStep];
	m_Si6.pAxis[5] = &theApp.m_Axes[23];

	BOOL result = DeviceIoControl( m_hSi6, IOCTL_SI6_SHARE_USER_STRUCT,
							&m_Si6, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

//This will be done automatically but it is good practice to call it if you have shared memory

BOOL CSi6::FreeUserSi6Struct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hSi6, IOCTL_SI6_FREE_USER_STRUCT,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

BOOL CSi6::ShareTimeSlotStruct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hSi6, IOCTL_SI6_SHARE_TIMESLOT_STRUCT,
							&theApp.m_UtUser.m_TS[0], 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

//This will be done automatically but it is good practice to call it if you have shared memory

BOOL CSi6::FreeTimeSlotStruct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hSi6, IOCTL_SI6_FREE_TIMESLOT_STRUCT,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}
LONG CSi6::GetIrqCount()
{
	DWORD BytesReturned;
	LONG rv[4];

	FillMemory(rv, sizeof rv, 0xff);
	BOOL result = DeviceIoControl( m_hSi6, IOCTL_SI6_GET_IRQ_COUNT,
							NULL, 0,			// Input
							rv, sizeof rv,		// Output
							&BytesReturned, NULL);


	return rv[0];

}

void CSi6::EnableAxis(int nWhich,int action)
{
	struct AxisData *axis = &theApp.m_Axes[nWhich];
	int	nSpeed;
	if(axis->nBit==0) return;
	if(axis->bAvailable==FALSE) return;

	DisableAxis(nWhich);
	theApp.m_Motors.ZBrakesEnable(FALSE,1);

	(axis->nPos > axis->nDesirePos)?
		(m_Si6.nDirReg|=axis->nBit):(m_Si6.nDirReg&=(~axis->nBit));
	axis->nCount=axis->nPos;
	switch(action) {
	case MOVETO:	axis->nCurrentSpeed = axis->nMinSpeed;
		if(axis->nDesirePos-axis->nPos) m_Si6.nProcessState|=axis->nBit;
		break;
	case JOYSTICK:
 		m_Si6.nJoystickPercent=100;
		axis->nCurrentSpeed = MulDiv(axis->nJoystickMinSpeed,m_Si6.nJoystickPercent,100);
		if(m_Si6.nJoystickSource==0) {
			axis->nJoystickZero=ReadJoystick(axis->nJoystickNumber);
		} else {
			axis->nJoystickZero=m_Si6.nCommsJoystickValue[axis->nJoystickNumber];
		}
		m_Si6.nProcessState&=(~axis->nBit);
		break;
	case HOMETO:	axis->nCurrentSpeed = axis->nMinSpeed;
		(axis->nHomeSpeed<0)?(m_Si6.nDirReg|=axis->nBit):(m_Si6.nDirReg&=(~axis->nBit));
		m_Si6.nProcessState|=axis->nBit;
		axis->bHomeFlag=FALSE;
		break;
	};
	m_Si6.nIrqAction[axis->nCounter]=action;

	Outp(m_Si6.nAddrs+SI6_DIR_PORT,m_Si6.nDirReg ^ m_Si6.nDirEor);
	if(axis->nCurrentSpeed==0) axis->nCurrentSpeed=1;
	nSpeed=1000000/axis->nCurrentSpeed;
	if(nSpeed>0xffff) nSpeed=0xffff;
	CounterStart(axis->nCounter,nSpeed);
	CounterInterruptMask(axis->nCounter,ENABLE);

}


void CSi6::DisableAxis(int nWhich)
{
	struct AxisData *axis = &theApp.m_Axes[nWhich];

	if(axis->nBit == 0) return;
	if(axis->bAvailable==FALSE) return;

	m_Si6.nIrqAction[axis->nCounter]=0;
	CounterInterruptMask(axis->nCounter,DISABLE);
	m_Si6.nProcessState&=(~axis->nBit);
}


void CSi6::CounterInterruptMask(int irq,int action)
{
int	mask=1;

mask<<=irq;
switch(action) {
case ENABLE: m_Si6.nIrqMask &= (~mask);
	break;
case DISABLE: m_Si6.nIrqMask |= mask;
	break;
};
//m_Si6.nIrqMask &= 0xbf;	//This bit added to allow dsp to use the si6 PIC
m_Si6.nIrqMask |= 0x40;	//This bit added to allow dsp to use the si6 PIC
Outp(m_Si6.nAddrs+SI6_PIC+1,m_Si6.nIrqMask);
}



void CSi6::EnableJoystick()
{
	switch(theApp.m_Tank.nJoystickSource) {
	case WIRED_MSC:
	case WIRED_SI10:
	case WIRELESS_SERIAL:
	case WIRED_SERIAL:	m_Si6.nJoystickSource=1;		//Comms
		break;
	case WIRED_SI6:	m_Si6.nJoystickSource=0;		//Si6
		break;
	}

	m_Si6.nJoystickMode=0;
	EnableAxis(theApp.m_Tank.nXStep,JOYSTICK);
	EnableAxis(theApp.m_Tank.nYStep,JOYSTICK);
	EnableAxis(theApp.m_Tank.nZStep,JOYSTICK);
	EnableAxis(theApp.m_Tank.nXtStep,JOYSTICK);
	EnableAxis(theApp.m_Tank.nYtStep,JOYSTICK);
	EnableAxis(theApp.m_Tank.nRStep,JOYSTICK);

}



void CSi6::StopAllMotorsDead()
{
	DisableAxis(theApp.m_Tank.nXStep);
	DisableAxis(theApp.m_Tank.nYStep);
	DisableAxis(theApp.m_Tank.nZStep);
	DisableAxis(theApp.m_Tank.nXtStep);
	DisableAxis(theApp.m_Tank.nYtStep);
	DisableAxis(theApp.m_Tank.nRStep);

}



void CSi6::CounterStart(int number,int value)
{

switch(number) {
case 0:	Outp(m_Si6.nAddrs+SI6_CNTRL_012,0x34);
	Outp(m_Si6.nAddrs+SI6_COUNT_0,value&0xff);
	Outp(m_Si6.nAddrs+SI6_COUNT_0,value>>8);
	break;
case 1:	Outp(m_Si6.nAddrs+SI6_CNTRL_012,0x74);
	Outp(m_Si6.nAddrs+SI6_COUNT_1,value&0xff);
	Outp(m_Si6.nAddrs+SI6_COUNT_1,value>>8);
	break;
case 2:	Outp(m_Si6.nAddrs+SI6_CNTRL_012,0xb4);
	Outp(m_Si6.nAddrs+SI6_COUNT_2,value&0xff);
	Outp(m_Si6.nAddrs+SI6_COUNT_2,value>>8);
	break;
case 3:	Outp(m_Si6.nAddrs+SI6_CNTRL_345,0x34);
	Outp(m_Si6.nAddrs+SI6_COUNT_3,value&0xff);
	Outp(m_Si6.nAddrs+SI6_COUNT_3,value>>8);
	break;
case 4:	Outp(m_Si6.nAddrs+SI6_CNTRL_345,0x74);
	Outp(m_Si6.nAddrs+SI6_COUNT_4,value&0xff);
	Outp(m_Si6.nAddrs+SI6_COUNT_4,value>>8);
	break;
case 5:	Outp(m_Si6.nAddrs+SI6_CNTRL_345,0xb4);
	Outp(m_Si6.nAddrs+SI6_COUNT_5,value&0xff);
	Outp(m_Si6.nAddrs+SI6_COUNT_5,value>>8);
	break;
};

}


void CSi6::AxisDefaults(int nWhich)
{
	int	mask=1;

	memset(&theApp.m_Axes[nWhich],0,sizeof AxisData);
	theApp.m_Axes[nWhich].fStepSize = 0.1f;
	theApp.m_Axes[nWhich].nBit = mask<<nWhich;
	theApp.m_Axes[nWhich].nCounter=nWhich;
	theApp.m_Axes[nWhich].nMinSpeed = 200;
	theApp.m_Axes[nWhich].nMaxSpeed = 5000;
	theApp.m_Axes[nWhich].nAcceleration=3;
	theApp.m_Axes[nWhich].nDeceleration=4;
	theApp.m_Axes[nWhich].nHomeSpeed=-400;
	theApp.m_Axes[nWhich].nJoystickMinSpeed=200;
	theApp.m_Axes[nWhich].nJoystickMaxSpeed=7000;
	theApp.m_Axes[nWhich].nJoystickMinValue=93;
	theApp.m_Axes[nWhich].nJoystickMaxValue=155;
	theApp.m_Axes[nWhich].nJoystickNumber=nWhich;
	if(theApp.m_Axes[nWhich].nJoystickNumber>3) theApp.m_Axes[nWhich].nJoystickNumber=0;
	theApp.m_Axes[nWhich].bAvailable=TRUE;
	theApp.m_Axes[nWhich].bJoystickAvailable=TRUE;
	
}





int	CSi6::ReadEncoder(struct AxisData *axis)
{
int	temp;

Outp(m_Si6.nAddrs+SI6_ENCODER_SELECT,axis->nEncoderNumber);
temp = Inp(m_Si6.nAddrs+0x18);
temp |= ((int)Inp(m_Si6.nAddrs+0x19)<<8);
temp |= ((int)Inp(m_Si6.nAddrs+0x1a)<<16);
temp |= ((int)Inp(m_Si6.nAddrs+0x1b)<<24);
if(axis->nEncoderMode==2) temp ^= 0xffffffffl;
return temp;
}







int CSi6::ReadJoystick(int nWhich)
{
	return Inp(m_Si6.nAddrs+SI6_JOYSTICK+nWhich) & 0xff;
}

void CSi6::ZeroPosition(int nWhich)
{
	struct AxisData *pAxis = &theApp.m_Axes[nWhich];

	pAxis->nPos=pAxis->nCount=pAxis->nDesirePos=0;
	if(pAxis->nEncoderMode) EncoderZero(pAxis);
}





void CSi6::EncoderZero(struct AxisData	*pAxis)
{

	Outp(m_Si6.nAddrs+SI6_ENCODER_SELECT,pAxis->nEncoderNumber|0x20);
}


void CSi6::EncoderResetMode(struct AxisData *pAxis)
{
	int	mask=1;

	if(pAxis->nEncoderResetMode) {
		m_Si6.nEncoderMode |= (mask<<pAxis->nEncoderNumber);
	} else {
		m_Si6.nEncoderMode &= ~(mask<<pAxis->nEncoderNumber);
	};
	Outp(m_Si6.nAddrs+SI6_ENCODER_MODE,m_Si6.nEncoderMode);
}

int CSi6::ReadLimitSwitches()
{
	int	nLimitSwitch=Inp(m_Si6.nAddrs+SI6_PIA+0);
	nLimitSwitch|=((int)Inp(m_Si6.nAddrs+SI6_PIA+1)<<8);


	return nLimitSwitch ^ m_Si6.nSwitchEor;
}


//////////////////////////////////////////////////////////////////////////////////////////
/*
	Example of setting up variables for scan

	m_pSi6->StopAllMotorsDead();

	m_pSi6->m_Axes[XAXIS].nStartPos=0;
	m_pSi6->m_Axes[XAXIS].nFinishPos=1000;
	m_pSi6->m_Si6.nWhichAxis=XAXIS;
	m_pSi6->m_Si6.nSamplesLine=1024;
	m_pSi6->m_Si6.nNumberAmpGates=2;
	m_pSi6->m_Si6.nNumberTimeGates=1;
	if(m_pSi6->AllocateAndShareDataMemory() == TRUE) {

	while(lineNumber<maxLineNumber) {
		if(m_pSi6->nScanDirection==0)
			m_pSi6->m_Axes[XAXIS].nDesirePos=m_pSi6->m_Axes[XAXIS].nFinishPos;
		} else {
			m_pSi6->m_Axes[XAXIS].nDesirePos=m_pSi6->m_Axes[XAXIS].nStartPos;
		};
		m_pSi6->m_Axes[YAXIS].nDesirePos+=step;
		m_pSi6->m_Si6.bCollectData=TRUE;
		m_pSi6->EnableAxis(XAXIS,MOVETO);
		m_pSi6->EnableAxis(YAXIS,MOVETO);
		wait for end of line
		lineNumber++
		m_pSi6->nScanDirection^=1;

	}
	m_pSi6->FreeAndDeAllocateDataMemory();

*/

BOOL CSi6::AllocateAndShareDataMemory()
{
	DWORD BytesReturned;
	BOOL result;

//	if((m_Si6.nNumberAmpGates<=0)&&(m_Si6.nNumberTimeGates<=0)) return FALSE;
//	if(m_Si6.nSamplesLine<=0) return FALSE;

	m_Si6.pData = Array;
	m_Si6.nDataSize = sizeof Array;

	if((m_Si6.pData != NULL) && (m_Si6.nDataSize>0)) {

		result = DeviceIoControl( m_hSi6, IOCTL_SI6_SHARE_DATA_ARRAY,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
							
	} else {
		result=FALSE;
	}
	return result;
}


//This will be done automatically but it is good practice to call it if you have shared memory

BOOL CSi6::FreeAndDeallocateDataMemory()
{
	DWORD BytesReturned;
	BOOL result;

	m_Si6.bCollectData=FALSE;
	result = DeviceIoControl( m_hSi6, IOCTL_SI6_FREE_DATA_ARRAY,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);

	
	return result;
}

bool CSi6::TestAxisRunning(int nWhich)
{
	if(theApp.m_Axes[nWhich].bAvailable==TRUE) {
		if(theApp.m_Axes[nWhich].nBit & m_Si6.nProcessState) return TRUE;
	}
	return FALSE;

}


//Methods Added by Vince///////////////////////////////////////
	/////////////////////////////////////////////////

BOOL CSi6::IsAxisInPosition(int nWhich, int Pos)
{
	if (abs(theApp.m_Axes[nWhich].nPos  - Pos) <= theApp.m_Axes[nWhich].nPositionError) return TRUE;
	else return FALSE;
}



float CSi6::FromCurrentPosToUnit(int nWhich)
{
	float Value;
	Value =  FromPosToUnit(nWhich, theApp.m_Axes[nWhich].nPos);
	return Value;
}




//Convert From Pos to Unit : You can specify the UnitSelected to choose a one different 
//from the current selection in the axes structure
float CSi6::FromPosToUnit(int nWhich, int Pos, int UnitSelected)
{
	float Value;

	if (UnitSelected == -1) UnitSelected = theApp.m_Axes[nWhich].nUnitSelected;

	if (nWhich==AXIS_ROT&&UnitSelected==0)   
		//Special case for the rotation axis if unit is mm: Take into account the diameter
		{
		Value = (Pos  * theApp.m_Axes[nWhich].fStepSize * (float)PI*theApp.m_Axes[nWhich].fDiameter) / (float)(180.0 * 2);
		}
	else Value = Pos  * theApp.m_Axes[nWhich].fStepSize; //Normal Processing
	return Value;
}


//Convert From Pos to Unit : You can specify the UnitSelected to choose a one different 
//from the current selection in the axes structure
int	CSi6::FromUnitToPos(int nWhich, float UnitPos,int UnitSelected)
{
	float Value;
	ASSERT(theApp.m_Axes[nWhich].fStepSize!=0);
	if (UnitSelected == -1) UnitSelected = theApp.m_Axes[nWhich].nUnitSelected;
	if (nWhich==AXIS_ROT&&UnitSelected==0)   //Special case for the rotation axis if unit is mm
		{
		Value = (2*UnitPos * 180)  / (float) (theApp.m_Axes[nWhich].fStepSize * PI*theApp.m_Axes[nWhich].fDiameter) ;
		}
	else Value = UnitPos  / theApp.m_Axes[nWhich].fStepSize;

	return (int)Value;
}


bool CSi6::TestAllAxisRunning()
{
	bool Flag = FALSE;

	Flag |= TestAxisRunning(theApp.m_Tank.nXStep);
	Flag |= TestAxisRunning(theApp.m_Tank.nYStep);
	Flag |= TestAxisRunning(theApp.m_Tank.nZStep);
	Flag |= TestAxisRunning(theApp.m_Tank.nXtStep);
	Flag |= TestAxisRunning(theApp.m_Tank.nYtStep);
	Flag |= TestAxisRunning(theApp.m_Tank.nRStep);

	return Flag;

}


int CSi6::ReadJoystickPourcent(int nWhich)
{
	int Value = theApp.m_Axes[nWhich].nJoystickValueRead;
	int Buff;
	int ValuePourcent;
	if (Value >= theApp.m_Axes[nWhich].nJoystickZero) 
	{
		Buff = theApp.m_Axes[nWhich].nJoystickMaxValue - theApp.m_Axes[nWhich].nJoystickZero;
		if (Buff!=0)
		ValuePourcent = (100 * (Value - theApp.m_Axes[nWhich].nJoystickZero)) / Buff;
		else ValuePourcent = 0;
	}
	else
	{
		Buff = theApp.m_Axes[nWhich].nJoystickMaxValue - theApp.m_Axes[nWhich].nJoystickZero;
		if (Buff!=0)
		ValuePourcent = (-100 * (Value - theApp.m_Axes[nWhich].nJoystickZero)) / Buff;
		else ValuePourcent = 0;

	}

	return ValuePourcent;



}

int CSi6::FromVelocityUnitToMotor()
{

	return 0;


}


int CSi6::FromVelocityMotorToUnit()
{
	return 0;



}




void CSi6::EnableHomeFind(int nAxis)
{
	if(nAxis==-1) {
		EnableAxis(theApp.m_Tank.nXStep,HOMETO);
		EnableAxis(theApp.m_Tank.nYStep,HOMETO);
		EnableAxis(theApp.m_Tank.nZStep,HOMETO);
		EnableAxis(theApp.m_Tank.nXtStep,HOMETO);
		EnableAxis(theApp.m_Tank.nYtStep,HOMETO);
		EnableAxis(theApp.m_Tank.nRStep,HOMETO);
	} else {
		EnableAxis(nAxis,HOMETO);
	}
}


void CSi6::MoveToCoord(COORDDATA cp)
{
	theApp.m_Axes[theApp.m_Tank.nXStep].nDesirePos=(int)(cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYStep].nDesirePos=(int)(cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nZStep].nDesirePos=(int)(cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nXtStep].nDesirePos=(int)(cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYtStep].nDesirePos=(int)(cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nRStep].nDesirePos=(int)(cp.Side0.fR/theApp.m_Axes[theApp.m_Tank.nRStep].fStepSize);
	EnableAxis(theApp.m_Tank.nXStep,MOVETO);
	EnableAxis(theApp.m_Tank.nYStep,MOVETO);
	EnableAxis(theApp.m_Tank.nZStep,MOVETO);
	EnableAxis(theApp.m_Tank.nXtStep,MOVETO);
	EnableAxis(theApp.m_Tank.nYtStep,MOVETO);
	EnableAxis(theApp.m_Tank.nRStep,MOVETO);

}

void CSi6::MoveToCoord(HoldingFixtureCoord Cp)
{
	theApp.m_Axes[theApp.m_Tank.nXStep].nDesirePos=(int)(Cp.fXMoving/theApp.m_Axes[theApp.m_Tank.nXStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYStep].nDesirePos=(int)(Cp.fYMoving/theApp.m_Axes[theApp.m_Tank.nYStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nZStep].nDesirePos=(int)(Cp.fZMoving/theApp.m_Axes[theApp.m_Tank.nZStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nXtStep].nDesirePos=(int)(Cp.fYFixed/theApp.m_Axes[theApp.m_Tank.nXtStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYtStep].nDesirePos=(int)(Cp.fZFixed/theApp.m_Axes[theApp.m_Tank.nYtStep].fStepSize);
	EnableAxis(theApp.m_Tank.nXStep,MOVETO);
	EnableAxis(theApp.m_Tank.nYStep,MOVETO);
	EnableAxis(theApp.m_Tank.nZStep,MOVETO);
	EnableAxis(theApp.m_Tank.nXtStep,MOVETO);
	EnableAxis(theApp.m_Tank.nYtStep,MOVETO);

}


void CSi6::MoveToHomePos()
{
	theApp.m_Axes[theApp.m_Tank.nXStep].nDesirePos=theApp.m_Axes[theApp.m_Tank.nXStep].nHomePos;
	theApp.m_Axes[theApp.m_Tank.nYStep].nDesirePos=theApp.m_Axes[theApp.m_Tank.nYStep].nHomePos;
	theApp.m_Axes[theApp.m_Tank.nZStep].nDesirePos=theApp.m_Axes[theApp.m_Tank.nZStep].nHomePos;
	theApp.m_Axes[theApp.m_Tank.nXtStep].nDesirePos=theApp.m_Axes[theApp.m_Tank.nXtStep].nHomePos;
	theApp.m_Axes[theApp.m_Tank.nYtStep].nDesirePos=theApp.m_Axes[theApp.m_Tank.nYtStep].nHomePos;
	theApp.m_Axes[theApp.m_Tank.nRStep].nDesirePos=theApp.m_Axes[theApp.m_Tank.nRStep].nHomePos;
	EnableAxis(theApp.m_Tank.nXStep,MOVETO);
	EnableAxis(theApp.m_Tank.nYStep,MOVETO);
	EnableAxis(theApp.m_Tank.nZStep,MOVETO);
	EnableAxis(theApp.m_Tank.nXtStep,MOVETO);
	EnableAxis(theApp.m_Tank.nYtStep,MOVETO);
	EnableAxis(theApp.m_Tank.nRStep,MOVETO);

}


void CSi6::GetCoord(COORDDATA *Cp)
{
	Cp->Side0.fX = (float)theApp.m_Axes[theApp.m_Tank.nXStep].nPos * theApp.m_Axes[theApp.m_Tank.nXStep].fStepSize;
	Cp->Side0.fY = (float)theApp.m_Axes[theApp.m_Tank.nYStep].nPos * theApp.m_Axes[theApp.m_Tank.nYStep].fStepSize;
	Cp->Side0.fZ = (float)theApp.m_Axes[theApp.m_Tank.nZStep].nPos * theApp.m_Axes[theApp.m_Tank.nZStep].fStepSize;
	Cp->Side0.fR = (float)theApp.m_Axes[theApp.m_Tank.nRStep].nPos * theApp.m_Axes[theApp.m_Tank.nRStep].fStepSize;
	Cp->Side0.fXt = (float)theApp.m_Axes[theApp.m_Tank.nXtStep].nPos * theApp.m_Axes[theApp.m_Tank.nXtStep].fStepSize;
	Cp->Side0.fYt = (float)theApp.m_Axes[theApp.m_Tank.nYtStep].nPos * theApp.m_Axes[theApp.m_Tank.nYtStep].fStepSize;
	Cp->Side1.fX = (float)theApp.m_Axes[X1AXIS].nPos * theApp.m_Axes[X1AXIS].fStepSize;
}


void CSi6::CollectData(int nWhichAxis,float dStart,float dFinish,float dIncrement)
{
	float dTemp;

	if(dFinish<dStart) {
		dTemp=dFinish;
		dFinish=dStart;
		dStart=dTemp;
	};
	theApp.m_Axes[nWhichAxis].nStartPos = (int)(dStart / theApp.m_Axes[nWhichAxis].fStepSize);
	theApp.m_Axes[nWhichAxis].nFinishPos = (int)(dFinish / theApp.m_Axes[nWhichAxis].fStepSize);

	m_Si6.nWhichAxis=nWhichAxis;
	m_Si6.nSamplesLine=(int)((dFinish-dStart)/dIncrement);
	m_Si6.bCollectData=TRUE;
}

void CSi6::StopCollectingData()
{
	m_Si6.bCollectData=FALSE;

}



void CSi6::SetScanSpeed(float dScanSpeed,bool bArchive)
{
	if(bArchive==TRUE)
		m_nCorrectMaxSpeed=theApp.m_Axes[m_Si6.nWhichAxis].nMaxSpeed;

	int nSpeed = (int)(dScanSpeed/theApp.m_Axes[m_Si6.nWhichAxis].fStepSize);

	if(nSpeed<m_nCorrectMaxSpeed) {
		theApp.m_Axes[m_Si6.nWhichAxis].nMaxSpeed=nSpeed;
	}

}


void CSi6::ResetMaxSpeed()
{
	theApp.m_Axes[m_Si6.nWhichAxis].nMaxSpeed=m_nCorrectMaxSpeed;
}



void CSi6::GetTurntableCoord(COORDDATA *pCp)
{
	pCp->Side0.fR = (float)theApp.m_Axes[theApp.m_Tank.nRStep].nPos * theApp.m_Axes[theApp.m_Tank.nRStep].fStepSize;
}

void CSi6::GetRollerPos(COORDDATA *pCp)
{
	pCp->Side1.fX = (float)theApp.m_Axes[X1AXIS].nPos * theApp.m_Axes[X1AXIS].fStepSize;
}

void CSi6::StartContinuousRotate()
{
	theApp.m_Axes[theApp.m_Tank.nRStep].nDesirePos=0x7fffffff;
	EnableAxis(theApp.m_Tank.nRStep,MOVETO);
}

int CSi6::GetRevCounter()
{
	return m_Si6.nRevCounter;
}

void CSi6::SetRevCounter(int nValue)
{

	m_Si6.nRevCounter = nValue;

}

int CSi6::Save(CUSLFile *pFile, int nOffset)
{
	if(nOffset) pFile->Seek(nOffset,CFile::current);
	ULONGLONG nPos = pFile->GetPosition();
	pFile->WriteStruct(&m_Si6,m_Si6.nStructSize = sizeof m_Si6);

	return (int)nPos;
}

void CSi6::Retrieve(CUSLFile *pFile)
{
	pFile->ReadStruct(&m_Si6, sizeof m_Si6);
}
