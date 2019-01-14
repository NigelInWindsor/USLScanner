// Servo.cpp: implementation of the CServo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Servo.h"
#include "winioctl.h"
#include "math.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const BYTE	PARPORT_DATA	= 0;
const BYTE	PARPORT_STATUS	= 1;
const BYTE	PARPORT_CONTROL	= 2;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
Add link E61 EQU2 -> IR6 on PMAC
Add link E80 INT -> IRQ10
or  link E84 INT -> IRQ7
g_pmac->addrs = 0x210; default don't change links
g_pmac->irq = 10;
(long)g_pmac->mem_addrs = 0xd0000000l;
E24,25,26,27 position 3
E17 REMOVE AMPLIFIER ENABLE HIGH
E85,87,88 Ppower from Computer
SHORT OUT D7
*/



CServo::CServo()
{
	

	m_hServo=NULL;
	m_nJoystickMargin=3;
	m_nJoystickPercent=100;
	m_bWorking[0] = FALSE;
	m_bWorking[1] = FALSE;
	m_nCardNumber=0;
	m_nAxisOffset = 0x40;
	m_nTimeout = 100000;
	m_bThreadRun = FALSE;
	m_nThreadAction = 1;
	m_nJoystickMargin=4;
	m_nPmacAddrs=ADDRS0;
	m_bLookAheadFlag=FALSE;

	for(int ii=0;ii<16;ii++) {
		m_Pmac.pAxis[ii]=&theApp.m_Axes[ii];
	};

	m_nPlcAddrs = 0x10;

}

CServo::~CServo()
{
	if(m_hServo!=NULL)
		SuspendDriver();

}

bool CServo::StartDriver(CString &strPmacType,int *nPmacType)
{
	BSTR Date	= ::SysAllocString(L"");
	BSTR Version = ::SysAllocString(L"");
	bool retval;


	retval = StartDriver();

	*nPmacType = PMAC1;
	strPmacType.Format(_T("Pmac 1 %s %s"), Version ,Date);

	return retval;
}

bool CServo::StartDriver()
{
	CString	Buff;

	GetHandleToDevice(2000);
	if( m_hServo==INVALID_HANDLE_VALUE) {
		MessageBox(NULL,_T("Failed to Allocate Hardware Resources\r\nPMAC Device Driver"),_T("Error"),NULL);
		return FALSE;
	} else {
		char	Version[40];
		GetVersion(Version,sizeof Version);
		if(strcmp(Version,SERVO_VERSION)) {
			CloseHandle(m_hServo);
			Buff="Incorrect Version of SERVO driver\r\n";
			Buff+=Version;
			MessageBox(NULL,Buff,_T("Error"),NULL);
			m_hServo=NULL;
			return FALSE;
		} else {
//			MessageBox(NULL,Version,"Ok",NULL);
//			Reset();



			ShareUserServoStruct();
			ShareTimeSlotStruct();
			CommsInitialize();

			//Initialize 2nd pmac
			/*
			m_nPmacAddrs=ADDRS1;
			m_nCardNumber=1;
			CommsInitialize();
			m_nPmacAddrs=ADDRS0;
			m_nCardNumber=0;
			*/


			//Share
			EnableInterrupt();
			return TRUE;
		}
	}
//	WriteDpram(1,0xca);
//	int nValue=ReadDpram(0);
	return FALSE;
}

void CServo::SuspendDriver()
{
	//	SuspendJoystick(0);
	DisableInterrupt();
	Sleep(300);
	StopServoThread(); 
	FreeTimeSlotStruct();
	FreeUserServoStruct();
	if(m_hServo!=NULL) {
		if( !CloseHandle(m_hServo))	{
	//		MessageBox(NULL,"Failed to Deallocate Servo Device Driver","Error",NULL);
		};
	};

}

BOOL CServo::GetHandleToDevice(int OS)
{
	
	WCHAR str[256];

	if(theApp.m_PmacRes.nIOAddrs<0x100) {
		theApp.m_PmacRes.nIOAddrs=0x210;
		theApp.m_PmacRes.nIORange=0x20;
		theApp.m_PmacRes.nMemAddrs=0xd0000;
		theApp.m_PmacRes.nMemRange=0x1000;
		theApp.m_PmacRes.nIrq=10;
	};
	swprintf_s(str,_T("\\\\.\\USLSERVO\\isa\\io%x,%x\\mem%x,%x\\irq%d\\override"),
		theApp.m_PmacRes.nIOAddrs,theApp.m_PmacRes.nIORange,
		theApp.m_PmacRes.nMemAddrs,theApp.m_PmacRes.nMemRange,
		theApp.m_PmacRes.nIrq);

	m_hServo=0;
	switch(OS) {
	case 95:
	case 98:
//		m_hSERVO = GetDeviceViaInterface((LPGUID)&SI6_GUID,0);
		break;
	case 2000:
		m_hServo=CreateFile(str, GENERIC_READ|GENERIC_WRITE, 0,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//		m_hServo=CreateFile("\\\\.\\USLSERVO\\isa\\io210,10\\memd0000,1000\\irq7\\override", GENERIC_READ|GENERIC_WRITE, 0,
//			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	};
	if( m_hServo==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	return TRUE;
}

WORD CServo::Inp(int nOffset)
{
	BYTE	ReadInput[3]={SERVO_READ, (BYTE)nOffset};
	DWORD BytesReturned;
	WORD rv[3]={0xffff,0xffff,0xffff};

	BOOL result=DeviceIoControl(m_hServo, IOCTL_SERVO_RUN_CMDS,
								ReadInput, 2,			// Input
								rv, sizeof(rv),					// Output
								&BytesReturned, NULL);
	return rv[2] & 0x00ff;

}

WORD CServo::Inpw(int nOffset)
{
	BYTE	ReadInput[3]={SERVO_READ, (BYTE)nOffset};
	DWORD BytesReturned;
	WORD rv[3]={0xffff,0xffff,0xffff};

	BOOL result=DeviceIoControl(m_hServo, IOCTL_SERVO_RUN_CMDS,
								ReadInput, 2,			// Input
								rv, sizeof(rv),					// Output
								&BytesReturned, NULL);
	return rv[2];

}
void CServo::Outp(int nOffset, int nValue)
{
	BYTE	WriteOutput[3]={SERVO_WRITE, (BYTE)nOffset, (BYTE)(nValue & 0xff)};
	DWORD BytesReturned;
	WORD rv[3]={0xffff,0xffff,0xffff};

	BOOL result=DeviceIoControl(m_hServo, IOCTL_SERVO_RUN_CMDS,
								WriteOutput, 3,			// Input
								rv, sizeof(rv),					// Output
								&BytesReturned, NULL);

}
void CServo::WriteDpram(int nOffset, int nValue)
{
	int	WriteOutput[3]={SERVO_WRITE_DPRAM, nOffset,nValue};
	DWORD BytesReturned;
	WORD rv[3]={0xffff,0xffff,0xffff};
	int	nn=sizeof WriteOutput;

	BOOL result=DeviceIoControl(m_hServo, IOCTL_SERVO_RUN_CMDS,
								WriteOutput, sizeof WriteOutput, // Input
								rv, sizeof(rv),					// Output
								&BytesReturned, NULL);

}

LONG CServo::ReadDpram(int nOffset)
{
	int	WriteOutput[2]={SERVO_READ_DPRAM, nOffset};
	DWORD BytesReturned;
	LONG rv[2];
	
	FillMemory(rv, sizeof rv, 0xff);
	BOOL result=DeviceIoControl(m_hServo, IOCTL_SERVO_RUN_CMDS,
								WriteOutput,sizeof WriteOutput,	// Input
								rv, sizeof(rv),					// Output
								&BytesReturned, NULL);

	return rv[1];
}


void CServo::GetVersion(char *Version,int Length)
{
	DWORD BytesReturned;

	Version[0]=0;
	BOOL result = DeviceIoControl( m_hServo, IOCTL_SERVO_GET_VERSION,
							NULL, 0,			// Input
							Version, Length,	// Output
							&BytesReturned, NULL);

}


void CServo::GetVersion(CString* Version)
{
	char str[256];

	GetVersion(str,sizeof str);
	Version->Format(_T("%s"),str);
}

BOOL CServo::ShareUserServoStruct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hServo, IOCTL_SERVO_SHARE_USER_STRUCT,
							&m_Pmac, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

BOOL CServo::FreeUserServoStruct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hServo, IOCTL_SERVO_FREE_USER_STRUCT,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

BOOL CServo::ShareTimeSlotStruct()
{
	DWORD BytesReturned;
	

	BOOL result = DeviceIoControl( m_hServo, IOCTL_SERVO_SHARE_TIMESLOT_STRUCT,
							&theApp.m_UtUser.m_TS[0], 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

//This will be done automatically but it is good practice to call it if you have shared memory

BOOL CServo::FreeTimeSlotStruct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hServo, IOCTL_SERVO_FREE_TIMESLOT_STRUCT,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}
//////////////////////////////////////////////////////////////////////////////////////////
/*
	Example of setting up variables for scan

	m_pSERVO->StopAllMotorsDead();

	m_pServo->m_Axes[XAXIS].nStartPos=0;
	m_pServo->m_Axes[XAXIS].nFinishPos=1000;
	m_pServo->m_Pmac.nWhichAxis=XAXIS;
	m_pServo->m_Pmac.nSamplesLine=1024;
	m_pServo->m_Pmac.nNumberAmpGates=2;
	m_pServo->m_Pmac.nNumberTimeGates=1;
	m_pServo->m_Pmac.nNumberTimeSlots=3;
	if(m_pServo->AllocateAndShareDataMemory() == TRUE) {

	while(lineNumber<maxLineNumber) {
		if(m_pSERVO->nScanDirection==0)
			m_pSERVO->m_Axes[XAXIS].nDesirePos=m_pSERVO->m_Axes[XAXIS].nFinishPos;
		} else {
			m_pSERVO->m_Axes[XAXIS].nDesirePos=m_pSERVO->m_Axes[XAXIS].nStartPos;
		};
		m_pSERVO->m_Axes[YAXIS].nDesirePos+=step;
		m_pSERVO->m_SERVO.bCollectData=TRUE;
		m_pSERVO->EnableAxis(XAXIS,MOVETO);
		m_pSERVO->EnableAxis(YAXIS,MOVETO);
		wait for end of line
		lineNumber++
		m_pSERVO->nScanDirection^=1;

	}
	m_pSERVO->FreeAndDeAllocateDataMemory();

*/

bool CServo::CollectData(int nAxis, int nStart, int nFinish, int nSamples, int nNumberAmpGates,int nNumberTimeGates,bool bContinuousRotate)
{
	

	theApp.m_Axes[nAxis].nStartPos=nStart;
	theApp.m_Axes[nAxis].nFinishPos=nFinish;
	m_Pmac.nWhichAxis=nAxis;
	m_Pmac.nSamplesLine=nSamples;
	m_Pmac.nNumberAmpGates[0]=nNumberAmpGates;
	m_Pmac.nNumberTimeGates[0]=nNumberTimeGates;
	m_Pmac.bContinuousRotate=bContinuousRotate;
	m_Pmac.nNumberTimeSlots=1;
	if(AllocateAndShareDataMemory()==TRUE) {
		m_Pmac.bCollectData = TRUE;
		return TRUE;
	}
	return FALSE;
}


bool CServo::StopCollectingData()
{
	m_Pmac.bCollectData = FALSE;
	Sleep(20);
	FreeAndDeallocateDataMemory();

	return TRUE;

}

BOOL CServo::AllocateAndShareDataMemory()
{
	DWORD BytesReturned;
	BOOL result;

	if((m_Pmac.nNumberAmpGates<=0)&&(m_Pmac.nNumberTimeGates<=0)) return FALSE;
	if(m_Pmac.nSamplesLine<=0) return FALSE;

	m_Pmac.pData = Array;
	m_Pmac.nDataSize = sizeof Array;

	if((m_Pmac.pData != NULL) && (m_Pmac.nDataSize>0)) {

		result = DeviceIoControl( m_hServo, IOCTL_SERVO_SHARE_DATA_ARRAY,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
							
	} else {
		result=FALSE;
	}
	return result;
}


//This will be done automatically but it is good practice to call it if you have shared memory

BOOL CServo::FreeAndDeallocateDataMemory()
{
	DWORD BytesReturned;
	BOOL result;

	m_Pmac.bCollectData=FALSE;
	result = DeviceIoControl( m_hServo, IOCTL_SERVO_FREE_DATA_ARRAY,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);

	
	return result;
}

/*
BOOL CServo::ShareDriverStruct()
{
	DWORD BytesReturned;
	PVOID rv[6];

	BOOL result = DeviceIoControl( m_hPmac, IOCTL_PMAC_SHARE_DRIVER_PMAC,
							NULL, 0,			// Input
							&rv, sizeof (rv),			// Output
							&BytesReturned, NULL);
	
	PMDL mdl = IoAllocateMdl(rv[0],sizeof (struct PMACData),FALSE,FALSE,NULL);
	if(mdl) {
//		MmProbeAndLockPages(mdl,UserMode,IoModifyAccess);
//		struct PMACData *buffer=(struct PMACData *)MmGetSystemAddressForMdl(mdl);
//		buffer->nIrqCount++;
		
//		MmUnlockPages(mdl);
		IoFreeMdl(mdl);
	}


	return result;
}

BOOL CServo::FreeDriverStruct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hPmac, IOCTL_PMAC_FREE_DRIVER_PMAC,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}
*/
LONG CServo::GetIrqCount()
{
	DWORD BytesReturned;
	LONG rv[4];

	FillMemory(rv, sizeof rv, 0xff);
	BOOL result = DeviceIoControl( m_hServo, IOCTL_SERVO_GET_IRQ_COUNT,
							NULL, 0,			// Input
							rv, sizeof rv,		// Output
							&BytesReturned, NULL);


	return rv[0];

}
void CServo::EnableInterrupt()
{
	


	BYTE ConnectToInterrupts[] =
	{
		SERVO_IRQ_CONNECT, PARPORT_STATUS, 0x00, 0x00,	// Connect to interrupt
	};
	DWORD BytesReturned;
	WORD rv[8];

	Outp(PMAC_PIC1+m_nPmacAddrs,0xff);
	Outp(5+m_nPmacAddrs,0);
	Outp(6+m_nPmacAddrs,0);
	Outp(PMAC_ICR+m_nPmacAddrs,0x00);
	Outp(PMAC_PIC0+m_nPmacAddrs,0x17);
	Outp(PMAC_PIC1+m_nPmacAddrs,0x08);
	Outp(PMAC_PIC1+m_nPmacAddrs,0x03);
	Outp(PMAC_PIC1+m_nPmacAddrs,0xbf);		//unmask lower five interrupts on pmac
	Outp(PMAC_ICR+m_nPmacAddrs,0x01);

	CommsClear();

	CommsStr("DISABLE PLC 2..31");
	CommsStr("ENABLE PLC 0");
	CommsStr("ENABLE PLC 1");
	CommsStr("I5=3");


	BOOL result = DeviceIoControl( m_hServo, IOCTL_SERVO_RUN_CMDS,
						ConnectToInterrupts, sizeof ConnectToInterrupts,	// Input
						rv, sizeof(rv),										// Output
						&BytesReturned, NULL);

	


}



void CServo::DisableInterrupt()
{
	BYTE DisableInterrupts[] =
	{
		SERVO_WRITE, PARPORT_CONTROL, 0xCC,			// Disable interrupts
	};
	WORD rv[3];
	DWORD BytesReturned;

	Outp(PMAC_PIC1+m_nPmacAddrs,0xff);
	BOOL result = DeviceIoControl( m_hServo, IOCTL_SERVO_RUN_CMDS,
							DisableInterrupts, sizeof DisableInterrupts,	// Input
							rv, sizeof(rv),									// Output
							&BytesReturned, NULL);

}

BOOL CServo::CommsInitialize()
{

	while(Inp(PMAC_STATUS+m_nPmacAddrs)==0xff) {
		m_bWorking[m_nCardNumber]=FALSE;
		if(MessageBox(NULL,_T("Failed to initialize PMAC Comms\r\nTry hardware reset"),_T("Error"),MB_YESNO)==IDYES) {
			Reset();
		} else {
			return FALSE;
		}
	}
	m_nMaxTimeoutUsed = 0;
	m_bWorking[m_nCardNumber]=TRUE;
	CommsClear();
	CommsStr("I3=2");
	CommsStr("I6=1");


	return m_bWorking[m_nCardNumber];
}

void CServo::CommsClear()
{
	WCHAR Str[100];
	int	ii;
	CString Temp1, Temp2;

	if(m_bWorking[m_nCardNumber]==FALSE) return;
	m_nUnSolicitedPtr=0;
	while(Inp(PMAC_STATUS+m_nPmacAddrs) & PMAC_TX) {
		m_UnSolicitedData[m_nUnSolicitedPtr++]=(char)Inp(PMAC_DATA+m_nPmacAddrs);
		m_UnSolicitedData[m_nUnSolicitedPtr++]=0;
		m_nUnSolicitedPtr%=sizeof m_UnSolicitedData;
		Sleep(1);
	};
	Outp(PMAC_DATA-2+m_nPmacAddrs,0);
	Outp(PMAC_DATA-1+m_nPmacAddrs,0);
	if(m_nUnSolicitedPtr) {
		for(ii=0;ii<m_nUnSolicitedPtr;ii++) {
			if(isprint(m_UnSolicitedData[ii])) {
				CharToCString(m_LastCommsStr,Temp1);
				CharToCString(m_UnSolicitedData,Temp2);
				swprintf_s(Str,_T("%s\r\n%s %d"),Temp1.GetBuffer(),Temp2.GetBuffer(),m_nUnSolicitedPtr);
				MessageBox(NULL,Str,_T("Error"),NULL);
				ii=m_nUnSolicitedPtr;
			};
		};
	};

}

int CServo::CommsHit()
{

	Sleep(5);
	return(Inp(PMAC_STATUS+m_nPmacAddrs) & 1);
}


int CServo::CommsGetch()
{
	int	ii=0;
	int	vv=0;

	if(m_bWorking[m_nCardNumber]==FALSE) return 0;
	while(ii++<m_nTimeout&&!(Inp(PMAC_STATUS+m_nPmacAddrs)&1)) {}; //Loop until ready to rx
	if(ii<m_nTimeout) {
		vv=Inp(PMAC_DATA+m_nPmacAddrs);
	} else {
		MessageBox(NULL,_T("Pmac Failed On Timeout"),_T("Error"),NULL);
	};
	if(ii>m_nMaxTimeoutUsed) m_nMaxTimeoutUsed =ii;
	return vv;
}

void CServo::CommsPutch(char vv)
{
	int	ii=0;

	if(m_bWorking[m_nCardNumber]==FALSE) return;
	while(ii++<m_nTimeout&&!(Inp(PMAC_STATUS+m_nPmacAddrs)&2)) {}; //Loop until ready to send
	if(ii<m_nTimeout) {
		Outp(PMAC_DATA+m_nPmacAddrs,vv);
	} else {
		MessageBox(NULL,_T("Pmac Failed On Timeout"),_T("Error"),NULL);
	};
	if(ii>m_nMaxTimeoutUsed) m_nMaxTimeoutUsed =ii;
}

int CServo::CommsGetStr(CString &str)
{
	char chr[9000];

	CommsGetStr(chr);

	CharToCString(chr,str);	

	return 1;	

}

void CServo::CharToCString(char *chr, CString &str)
{
	unsigned int i=0;
	CString cstr;

	for(i=0;i<strlen(chr);i++)
	{
		str += (WCHAR)chr[i];
	}

	return;

}


int CServo::CommsGetStr(char *string)
{
	int	nn=0;
	char	vv=0;

	if(m_bWorking[m_nCardNumber]==FALSE) return 0xff;

	strcpy_s(string,strlen(m_LastStrRx) + 1, m_LastStrRx);
	return strlen(m_LastStrRx);

	for(nn=0;(nn<256)&&(vv!=ACK)&&(vv!=LF); ) {
		if(CommsHit()) {
			switch(vv=CommsGetch()) {
			default:
				string[nn++]=vv;
				break;
			case CR://	string[nn++]=0;
				break;
			case ACK:
				break;
			case LF:
				break;
			};
		};
	};
	string[nn]=0;
	CommsClear();
	return strlen(string);
}

int CServo::CommsStr(CString str, bool bWaitForResponce)
{
	char chr[255];
	
	CStringToChar(str,chr);

	return CommsStr(chr);

}

int CServo::CommsStr(char *string)
{
	int	ii=0;
	int	vv=0;
	int	nFlag = 0;
	CString Buff;
	CString Temp1, Temp2;
	CString ErrorCode[20]={
		L" ",
		L"Command not allowed during program execution",
		L" ",
		L"Data error or unrecognised command",
		L" ",
		L"Command not allowed unless buffer open",
		L"No Room in buffer for command",
		L"Buffer already in use",
		L" ",
		L"Program Structural error",
		L"Both overtravel limits set",
		L"Previous move not completed",
		L"A Motor is open loop",
		L"A motor in the C.S is not activated",
		L"No Motors in the C.S",
		L"Not pointing to valid program",
		L"Running improperly structured program" };


	if(m_bWorking[m_nCardNumber]==FALSE) return 0;
	CommsClear();
	strcpy_s(m_LastCommsStr,string);
	while(string[ii]) CommsPutch(string[ii++]);
	CommsPutch(CR);
	strcpy_s(m_LastStrTx,string);

	if(strchr(string,'%')) {
		ii=0;
	}
	ii=0;
	while(1) {
		vv=CommsGetch();
		switch(vv) {
		case LF:
		case ACK: m_LastStrRx[ii]=0;
			return nFlag;
			break;
		case CR:
			if(nFlag==1) {
				int nErrNumber = atoi(&m_LastStrRx[3]);
				CharToCString(m_LastStrTx,Temp1);
				CharToCString(m_LastStrRx,Temp2);
				Buff.Format(_T("Error writing %s to PMAC %s\r\n "),Temp1,Temp2);
				Buff+=ErrorCode[nErrNumber];
				MessageBox(NULL,Buff,_T("Error"),NULL);
				return nFlag;
			}
			m_LastStrRx[ii++]=vv;
			m_LastStrRx[ii]=0;
			break;
		case BELL:
			nFlag=1;
			break;
		default:
			m_LastStrRx[ii++]=vv;
			m_LastStrRx[ii]=0;
		}
	};
	
	return nFlag;
}


void CServo::CommsStrNoCr(char *string)
{
	int	ii=0;

	if(m_bWorking[m_nCardNumber]==FALSE) return;
	while(string[ii]) CommsPutch(string[ii++]);
	strcpy_s(m_LastStrTx,strlen(string) + 1, string);

}

bool CServo::DownloadProgram(CString FilePath)
{
	CFileException e;
	CString str;
	CStdioFile File;
	char chr[50000];
	CString Buff;

	if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeText,&e)) {
		return false;
	} else {

		CommsStr("UNDEFINE ALL");
		CommsStr("A");
		CommsStr("I5=0");
		CommsStr("DISABLE PLC 0..31");
		CommsStr("CLOSE");
		Sleep(100);
		CommsStr("DELETE GATHER");
		Sleep(100);
		CommsStr("DELETE TRACE");

		while(File.ReadString(Buff)){
			CStringToChar(Buff,chr);
			CommsStr(chr);
		}

		CommsStr("I5=3");
		CommsStr("ENABLE PLC 0..31");

	}

	return true;

}


void CServo::DownLoadProgram(char *String, int nn,int nm)
{
	int ii;

	CommsStr("UNDEFINE ALL");
	CommsStr("A");
	CommsStr("I5=0");
	CommsStr("DISABLE PLC 0..31");
	CommsStr("CLOSE");
	Sleep(100);
	CommsStr("DELETE GATHER");
	Sleep(100);
	CommsStr("DELETE TRACE");
	
	for(ii=0;ii<nn;ii++) {
		CommsStr(&(String[ii*nm]));
	};

//	enable_pmac_interrupt();
}

void CServo::PositionPlcProgram()
{

	char String[][20]= {
	{"OPEN PLC 0 CLEAR"},
	{"M14=M10 / 3072"},
	{"M24=M20 / 3072"},
	{"M34=M30 / 3072"},
	{"M44=M40 / 3072"},
	{"M54=M50 / 3072"},
	{"M64=M60 / 3072"},
	{"M74=M70 / 3072"},
	{"M0=0"},
	{"M0=1"},	
	{"P11=M7"},
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);

}

void CServo::StatusPlcProgram()
{
/*
	{"IF(M7=P11)"},				//Watchdog
		{"M16=0"},
		{"CMD\x022#1j/\x022"},
		{"M26=0"},
		{"CMD\x022#2j/\x022"},
		{"M36=0"},
		{"CMD\x022#3j/\x022"},
		{"M46=0"},
		{"CMD\x022#4j/\x022"},
		{"M56=0"},
		{"CMD\x022#5j/\x022"},
		{"M66=0"},
		{"CMD\x022#6j/\x022"},
	{"ENDIF"},
*/

	char String[][50] = {
	{"OPEN PLC 1 CLEAR"},
	{"M17=(M11&15728640)|(M12&434176)|(M13&19471)"},
	{"M27=(M21&15728640)|(M22&434176)|(M23&19471)"},
	{"M37=(M31&15728640)|(M32&434176)|(M33&19471)"},
	{"M47=(M41&15728640)|(M42&434176)|(M43&19471)"},
	{"M57=(M51&15728640)|(M52&434176)|(M53&19471)"},
	{"M67=(M61&15728640)|(M62&434176)|(M63&19471)"},
	{"M77=(M71&15728640)|(M72&434176)|(M73&19471)"},
	{"M8=M130 + (M131*2) + (M139*4)"},
	{"M140=M142"},								//8 outputs
	{"M143=M141"},								//8 Inputs
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);

}

void CServo::PolyMoveProgram()
{

	char String[][70]={
	{"OPEN PROG 20 CLEAR"},
	{"M6=20"},
	{"M1=1"},
	{"FRAX(X,Y,Z,A,B)"},
	{"ABS"},
	{"LINEAR"},
	{"TA(M132)"},
	{"TS(M9)"},
	{"TM(M2)"},
	{"X(M200)Y(M201)Z(M202)A(M203)B(M204)"},
	{"M5=1"},
	{"Q0=1"},
	{"WHILE(Q0<M3 AND M1=1)"},
		{"Q1=M5*5"},
		{"X(M(200+Q1))Y(M(201+Q1))Z(M(202+Q1))A(M(203+Q1))B(M(204+Q1))"},
		{"Q0 = Q0 + 1"},
		{"M5 = M5 + 1"},
		{"IF(M5>80)"},
			{"M5=0"},
		{"ENDIF"},
	{"ENDWHILE"},
	{"DWELL1"},
	{"M1=1"},
	{"M6=0"},
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
}


int CServo::DownLoadPolyMoveCoordinates(CPolyCoord *ToolPath, int nDir)
{
	

	int	ii,jj;
	int	nX0,nY0,nZ0,nXt0,nYt0;
	int	nOffset;
	CCoord	Cp;

	nOffset=PMAC_DPRAM_DATA_START;

	if(nDir==0) {
		for(ii=0;(ii<ToolPath->m_nCoordL)&&(ii<60);ii++) {
			Cp=ToolPath->m_pCp[ii];
			nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
			WriteDpram(nOffset+ii*12+0,nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset);
			WriteDpram(nOffset+ii*12+1,nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset);
			WriteDpram(nOffset+ii*12+2,nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset);
			WriteDpram(nOffset+ii*12+3,nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset);
			WriteDpram(nOffset+ii*12+4,nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset);
			nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
			nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
			nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
			WriteDpram(nOffset+ii*12+5,nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
			WriteDpram(nOffset+ii*12+6,nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
			WriteDpram(nOffset+ii*12+7,nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
			WriteDpram(nOffset+ii*12+8,nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset);
			WriteDpram(nOffset+ii*12+9,nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset);
		};
	} else {
		for(ii=0,jj=ToolPath->m_nCoordL-1;(ii<ToolPath->m_nCoordL)&&(ii<60);ii++,jj--) {
			Cp=ToolPath->m_pCp[jj];
			nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
			WriteDpram(nOffset+ii*12+0,nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset);
			WriteDpram(nOffset+ii*12+1,nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset);
			WriteDpram(nOffset+ii*12+2,nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset);
			WriteDpram(nOffset+ii*12+3,nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset);
			WriteDpram(nOffset+ii*12+4,nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset);
			nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
			nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
			nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
			WriteDpram(nOffset+ii*12+5,nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
			WriteDpram(nOffset+ii*12+6,nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
			WriteDpram(nOffset+ii*12+7,nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
			WriteDpram(nOffset+ii*12+8,nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset);
			WriteDpram(nOffset+ii*12+9,nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset);
		};
	};

	WriteDpram(PMAC_LOOP_COUNT_1, 0);		//M5
	WriteDpram(PMAC_NUMBER_COORDS, ii);		//M3
	m_nWritePtr=ii%60;
	return ii;
}

int CServo::DownLoadNextPolyMoveCoordinate(int nNextCoordinate,CPolyCoord *ToolPath, int nDir)
{
	

	int	ii;
	int	nX0,nY0,nZ0,nXt0,nYt0;
	int	nOffset=PMAC_DPRAM_DATA_START;
	CCoord	Cp;
	int nLoopCount=GetLoopCounter();

	if(nNextCoordinate>=ToolPath->m_nCoordL) return nNextCoordinate;
	if((nNextCoordinate-nLoopCount) >= 50) return nNextCoordinate;

	ii=m_nWritePtr;
	m_nWritePtr++;
	m_nWritePtr=m_nWritePtr%60;

	if(nDir==0) {
		Cp=ToolPath->m_pCp[nNextCoordinate];
		nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
		nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
		nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
		nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
		nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
		WriteDpram(nOffset+ii*12+0,nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset);
		WriteDpram(nOffset+ii*12+1,nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset);
		WriteDpram(nOffset+ii*12+2,nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset);
		WriteDpram(nOffset+ii*12+3,nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset);
		WriteDpram(nOffset+ii*12+4,nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset);
		nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
		nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
		nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
		WriteDpram(nOffset+ii*12+5,nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
		WriteDpram(nOffset+ii*12+6,nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
		WriteDpram(nOffset+ii*12+7,nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
		WriteDpram(nOffset+ii*12+8,nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset);
		WriteDpram(nOffset+ii*12+9,nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset);
	} else {
		Cp=ToolPath->m_pCp[ToolPath->m_nCoordL-nNextCoordinate-1];
		nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
		nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
		nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
		nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
		nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
		WriteDpram(nOffset+ii*12+0,nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset);
		WriteDpram(nOffset+ii*12+1,nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset);
		WriteDpram(nOffset+ii*12+2,nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset);
		WriteDpram(nOffset+ii*12+3,nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset);
		WriteDpram(nOffset+ii*12+4,nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset);
		nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
		nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
		nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
		WriteDpram(nOffset+ii*12+5,nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
		WriteDpram(nOffset+ii*12+6,nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
		WriteDpram(nOffset+ii*12+7,nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
		WriteDpram(nOffset+ii*12+8,nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset);
		WriteDpram(nOffset+ii*12+9,nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset);
	};
	nNextCoordinate++;
	WriteDpram(PMAC_NUMBER_COORDS, nNextCoordinate);		//M3

	return nNextCoordinate;
}


void CServo::StartPolyMove(int nn)
{
	int	nTm=2;
	int	nTs=1;
	int	nTa=0;

	WriteDpram(PMAC_MOVE_FLAG_1, 1);
	WriteDpram(PMAC_LOOP_COUNT_1, 0);		//M5
	WriteDpram(PMAC_NUMBER_COORDS, nn);		//M3
	WriteDpram(PMAC_JOYSTICK_TIP_MODE, 1);		//M3
	WriteDpram(PMAC_TM, nTm);
	WriteDpram(PMAC_TS, nTs);
	WriteDpram(PMAC_TA, nTa);

	CommsStr("&1 B30 R");
	CommsStr("&3 B33 R");
	Sleep(100);

}

void CServo::StartDualPolyMove(int nn)
{
	int	nTm=5;
	int	nTs=1;
	int	nTa=0;

	WriteDpram(PMAC_MOVE_FLAG_1, 1);
	WriteDpram(PMAC_LOOP_COUNT_1, 0);		//M5
	WriteDpram(PMAC_LOOP_COUNT_2, 0);		//M133
	WriteDpram(PMAC_NUMBER_COORDS, nn);		//M3
	WriteDpram(PMAC_TM, nTm);
	WriteDpram(PMAC_TS, nTs);
	WriteDpram(PMAC_TA, nTa);

	CommsStr("&1 B31 R");
	CommsStr("&3 B32 R");
	Sleep(100);

}

void CServo::StartPolyMoveWithLookAhead(int nn)
{
	
	int	nTm=2;
	int	nTs=0;
	int	nTa=1;

	WriteDpram(PMAC_MOVE_FLAG_1, 1);
	WriteDpram(PMAC_JOYSTICK_TIP_MODE, 1);		//M3
	WriteDpram(PMAC_FEEDFORWARD, theApp.m_Tank.nTenthAxisFeedForward);		//M3
	WriteDpram(PMAC_TM, nTm);
	WriteDpram(PMAC_TS, nTs);
	WriteDpram(PMAC_TA, nTa);

	switch(PROFILE->m_nScanSide) {
	case 0:
		CommsStr("&1 B35 R");
		break;
	case 1:
		CommsStr("&1 B36 R");
		break;
	case 2:
		CommsStr("&1 B30 R");
		CommsStr("&3 B33 R");
		break;
	}
	Sleep(100);

}
void CServo::StartDualPolyMoveWithLookAhead(int nn)
{
	int	nTm=2;
	int	nTs=1;
	int	nTa=3;

	WriteDpram(PMAC_MOVE_FLAG_1, 1);
	WriteDpram(PMAC_LOOP_COUNT_1, 0);		//M5
	WriteDpram(PMAC_NUMBER_COORDS, nn);		//M3
	WriteDpram(PMAC_TM, nTm);
	WriteDpram(PMAC_TS, nTs);
	WriteDpram(PMAC_TA, nTa);

	CommsStr("&1 B30 R &3 B33 R");
	Sleep(100);

}


int CServo::GetLoopCounter()
{
	int	nn=ReadDpram(PMAC_LOOP_COUNT_1);
	return nn;

}

void CServo::GeneralMVariables()
{
	
	int ii,nAddrs;
	char str[30];
	char StringTurbo[][30]={
	{"M0..1023->*"},
	{"M0->X:$078004,13,1"},			//IRQ BIT EQU 2
	{"M1->DP:$060060"},				//MOVE FLAG
	{"M2->DP:$060061"},				//TM
	{"M3->DP:$060062"},				//NUMBER COORDINATES
	{"M4->DP:$060063"},				//MANIPULATOR MODE
	{"M5->DP:$060064"},				//LOOP COUNTER
	{"M6->DP:$060065"},				//CURRENT PROGRAM
	{"M7->DP:$060066"},				//TRACKING MODE
	{"M8->DP:$060067"},				//COORDINATE_STATUS
	{"M9->DP:$060068"},				//TS
	{"M10->D:$00008B"},				//X Right Axis
	{"M11->X:$078000,24,U"},
	{"M12->X:$0000B0,24,U"},
	{"M13->Y:$0000C0,24,U"},
	{"M14->DP:$060000"},				//PC location of axis position
	{"M15->DP:$060001"},				//PC location of axis desire_position
	{"M16->DP:$060002"},				//PC location of axis joystick deviation
	{"M17->DP:$060003"},				//PC location MOTOR STATUS
	{"M18->DP:$060004"},				//PC location AVERAGED VELOCITY
	{"M19->D:$000091"},
	{"M20->D:$00010B"},				//Y Right Axis
	{"M21->X:$078004,24,U"},
	{"M22->X:$000130,24,U"},
	{"M23->Y:$000140,24,U"},
	{"M24->DP:$060008"},
	{"M25->DP:$060009"},
	{"M26->DP:$06000A"},
	{"M27->DP:$06000B"},
	{"M28->DP:$06000C"},
	{"M29->D:$000111"},
	{"M30->D:$00018B"},				//Z Right Axis
	{"M31->X:$078008,24,U"},
	{"M32->X:$0001B0,24,U"},
	{"M33->Y:$0001C0,24,U"},
	{"M34->DP:$060010"},
	{"M35->DP:$060011"},
	{"M36->DP:$060012"},
	{"M37->DP:$060013"},
	{"M38->DP:$060014"},
	{"M39->D:$000191"},
	{"M40->D:$00020B"},				//Xt Right Axis
	{"M41->X:$07800C,24,U"},
	{"M42->X:$000230,24,U"},
	{"M43->Y:$000240,24,U"},
	{"M44->DP:$060018"},
	{"M45->DP:$060019"},
	{"M46->DP:$06001A"},
	{"M47->DP:$06001B"},
	{"M48->DP:$06001C"},
	{"M49->D:$000211"},
	{"M50->D:$00028B"},				//Yt Right Axis
	{"M51->X:$078100,24,U"},
	{"M52->X:$0002B0,24,U"},
	{"M53->Y:$0002C0,24,U"},
	{"M54->DP:$060020"},
	{"M55->DP:$060021"},
	{"M56->DP:$060022"},
	{"M57->DP:$060023"},
	{"M58->DP:$060024"},
	{"M59->D:$000291"},
	{"M60->D:$00030B"},				//Pump Right
	{"M61->X:$078104,24,U"},
	{"M62->X:$000330,24,U"},		
	{"M63->Y:$000340,24,U"},		
	{"M64->DP:$060028"},
	{"M65->DP:$060029"},
	{"M66->DP:$06002A"},
	{"M67->DP:$06002B"},
	{"M68->DP:$06002C"},
	{"M69->D:$000311"},
	{"M70->D:$00038B"},				// X Left Axis
	{"M71->X:$078108,24,U"},
	{"M72->X:$0003B0,24,U"},
	{"M73->Y:$0003C0,24,U"},
	{"M74->DP:$060030"},
	{"M75->DP:$060031"},
	{"M76->DP:$060032"},
	{"M77->DP:$060033"},
	{"M78->DP:$060034"},
	{"M79->D:$000391"},
	{"M80->D:$00040B"},				//Y Left Axis
	{"M81->X:$07820C,24,U"},
	{"M82->X:$000530,24,U"},
	{"M83->Y:$000540,24,U"},
	{"M84->DP:$060038"},
	{"M85->DP:$060039"},
	{"M86->DP:$06003A"},
	{"M87->DP:$06003B"},
	{"M88->DP:$06003C"},
	{"M89->D:$000511"},
	{"M90->D:$00058B"},			// Z Left axis
	{"M91->X:$078208,24,U"},
	{"M92->X:$0005B0,24,U"},
	{"M93->Y:$0005C0,24,U"},
	{"M94->DP:$060040"},
	{"M95->DP:$060041"},
	{"M96->DP:$060042"},
	{"M97->DP:$060043"},
	{"M98->DP:$060044"},
	{"M99->D:$000591"},
	{"M100->D:$00060B"},			//Xt Left axis
	{"M101->X:$078300,24,U"},
	{"M102->X:$000630,24,U"},
	{"M103->Y:$000640,24,U"},
	{"M104->DP:$060048"},
	{"M105->DP:$060049"},
	{"M106->DP:$06004A"},
	{"M107->DP:$06004B"},
	{"M108->DP:$06004C"},
	{"M109->D:$000611"},
	{"M110->D:$00068B"},			//Yt Left axis
	{"M111->X:$078304,24,U"},
	{"M112->X:$0006B0,24,U"},
	{"M113->Y:$0006C0,24,U"},
	{"M114->DP:$060050"},
	{"M115->DP:$060051"},
	{"M116->DP:$060052"},
	{"M117->DP:$060053"},
	{"M118->DP:$060054"},
	{"M119->D:$000691"},
	{"M120->D:$00070B"},			//Pump Left axis
	{"M121->X:$078308,24,U"},
	{"M122->X:$000730,24,U"},
	{"M123->Y:$000740,24,U"},
	{"M124->DP:$060058"},
	{"M125->DP:$060059"},
	{"M126->DP:$06005A"},
	{"M127->DP:$06005B"},
	{"M128->DP:$06005C"},
	{"M129->D:$000711"},
	{"M130->X:$002040,0,1"},			//COORDINATE SYSTEM 1
	{"M131->X:$002140,0,1"},			//COORDINATE_SYSTEM 2
	{"M132->DP:$060069"},				//TA
	{"M139->X:$002240,0,1"},			//COORDINATE_SYSTEM 3
	{"M140->Y:$078802,8,8,U"},			//8 Outputs
	{"M141->Y:$078802,0,8,U"},			//8 Inputs
	{"M142->DP:$06005D"},				//DPRAM 8 Outputs;
	{"M143->DP:$06005E"},				//DPRAM 8 Inputs
	{"M150->D:$0000EF"},				//Averagedvelocity	
	{"M151->D:$00016F"},				//Averagedvelocity		
	{"M152->D:$0001EF"},				//Averagedvelocity		
	{"M153->D:$00026F"},				//Averagedvelocity		
	{"M154->D:$0002EF"},				//Averagedvelocity		
	{"M155->D:$00036F"},				//Averagedvelocity		
	{"M156->D:$0003EF"},				//Averagedvelocity		
	{"M157->D:$00046F"}};				//Averagedvelocity		

	char StringNormal[][30]={
	{"M0..1023->*"},
	{"M0->X:$0C004,13,1"},			//IRQ BIT EQU 2
	{"M1->DP:$00D060"},				//MOVE FLAG
	{"M2->DP:$00D061"},
	{"M3->DP:$00D062"},
	{"M4->DP:$00D063"},
	{"M5->DP:$00D064"},
	{"M6->DP:$00D065"},
	{"M7->DP:$00D066"},
	{"M8->DP:$00D067"},
	{"M9->DP:$00D068"},

	{"M10->D:$00002B"},				//Actual Position X Right Axis
	{"M11->X:$00C000,24,U"},		//Limit switches
	{"M12->X:$00003D,24,U"},		//desired velocity 0
	{"M13->Y:$000814,24,U"},		//fatal warning error
	{"M14->DP:$00D000"},				//PC location of axis position
	{"M15->DP:$00D001"},				//PC location of axis desire_position
	{"M16->DP:$00D002"},				//PC location of axis joystick deviation
	{"M17->DP:$00D003"},				//PC location MOTOR STATUS
	{"M18->DP:$00D004"},				//PC location FOLLOWING ERROR
	{"M19->D:$000091"},				//Jog to position

	{"M20->D:$000067"},				//Y Right Axis
	{"M21->X:$00C004,24,U"},
	{"M22->X:$000079,24,U"},
	{"M23->Y:$0008D4,24,U"},
	{"M24->DP:$00D008"},
	{"M25->DP:$00D009"},
	{"M26->DP:$00D00A"},
	{"M27->DP:$00D00B"},
	{"M28->DP:$00D00C"},
	{"M29->D:$000111"},

	{"M30->D:$0000A3"},				//Z Right Axis
	{"M31->X:$00C008,24,U"},
	{"M32->X:$0000B5,24,U"},
	{"M33->Y:$000994,24,U"},
	{"M34->DP:$00D010"},
	{"M35->DP:$00D011"},
	{"M36->DP:$00D012"},
	{"M37->DP:$00D013"},
	{"M38->DP:$00D014"},
	{"M39->D:$000191"},

	{"M40->D:$0000DF"},				//Xt Right Axis
	{"M41->X:$00C00C,24,U"},
	{"M42->X:$0000F1,24,U"},
	{"M43->Y:$000A54,24,U"},
	{"M44->DP:$00D018"},
	{"M45->DP:$00D019"},
	{"M46->DP:$00D01A"},
	{"M47->DP:$00D01B"},
	{"M48->DP:$00D01C"},
	{"M49->D:$000211"}
	,
	{"M50->D:$00011B"},				//Yt Right Axis
	{"M51->X:$00C010,24,U"},
	{"M52->X:$00012D,24,U"},
	{"M53->Y:$000B14,24,U"},
	{"M54->DP:$00D020"},
	{"M55->DP:$00D021"},
	{"M56->DP:$00D022"},
	{"M57->DP:$00D023"},
	{"M58->DP:$00D024"},
	{"M59->D:$000291"},

	{"M130->X:$000818,0,1"},			//COORDINATE SYSTEM 1
	{"M131->X:$0008D8,0,1"},			//COORDINATE_SYSTEM 2
	{"M132->DP:$00D069"},				//TA
	{"M139->X:$0009D8,0,1"},			//COORDINATE_SYSTEM 3
	{"M140->Y:$FFC2,8,8,U"},			//8 Outputs
	{"M141->Y:$FFC2,0,8,U"},			//8 Inputs
	{"M142->DP:$D05D"},					//DPRAM 8 Outputs;
	{"M143->DP:$D05E"}};					//DPRAM 8 Inputs



	switch(theApp.m_nMotorType) {
	default: MessageBox(NULL,_T("Incorrest motor type"),_T("Error"),MB_OK);
		break;
	case PMAC_TURBO:
		DownLoadProgram((char *)StringTurbo,(sizeof StringTurbo)/(sizeof StringTurbo[0]),sizeof StringTurbo[0]);
		for(ii=200,nAddrs=0x60070;ii<1024;ii++,nAddrs++) {
			sprintf_s(str,"M%d->DP:$%06lx",ii,nAddrs);
			CommsStr(str);
		};
		CommsStr("WX:$7009,$D,$05");		//configure for dual port ram
		CommsStr("I58=0");			//DISABLE DPRAM ASCII COMMS
		CommsStr("I65=1");			//ENABLE ACC-24
		CommsStr("I93=$D");			//DPRAM BASE ADDRS 0xd0000
		CommsStr("I94=$5");			//ENABLE DPRAM
		break;
	case PMAC_NORMAL:
		DownLoadProgram((char *)StringNormal,(sizeof StringNormal)/(sizeof StringNormal[0]),sizeof StringNormal[0]);
		for(ii=200,nAddrs=0xD070;ii<1024;ii++,nAddrs++) {
			sprintf_s(str,"M%d->DP:$%06lx",ii,nAddrs);
			CommsStr(str);
		};
		CommsStr("WX:$E009,$D,$05");		//configure for dual port ram
		CommsStr("WX:$786,$D,$05");		//configure for dual port ram
		break;

	}
}

void CServo::GeneralIVariables()
{
	
	char StringTurbo[][20]={
	{"I3=2"},
	{"I4=0"},
	{"I9=2"},
	{"I8=0"},
	{"I58=0"},			//DISABLE DPRAM ASCII COMMS
	{"I65=1"},			//ENABLE ACC-24
	{"I93=$D"},			//DPRAM BASE ADDRS 0xd0000
	{"I94=$5"},			//ENABLE DPRAM
	{"I102=$078003"},	//DAC 1
	{"I202=$07800A"},	//DAC 4
	{"I303=$07800B"},	//DAC 3
	{"I402=$078103"},	//DAC 5
	{"I502=$078102"},	//DAC 6
	{"I602=$07810B"},	//DAC 7
	{"I902=$078203"},	//DAC 9
	{"I1002=$07820A"},	//DAC 12
	{"I1103=$07820B"},	//DAC 11
	{"I1202=$078303"},	//DAC 13
	{"I1302=$078302"},	//DAC 14
	{"I1402=$07830B"},	//DAC 15
	{"I103=$003502"},	//MOTOR ENCODER FEEDBACK
	{"I203=$003504"},
	{"I303=$003503"},
	{"I403=$003505"},
	{"I503=$003506"},
	{"I603=$003508"},
	{"I903=$00350A"},
	{"I1003=$00350C"},
	{"I1103=$00350B"},
	{"I1203=$00350D"},
	{"I1303=$00350E"},
	{"I1403=$003510"},
	{"I104=$003502"},	//LOAD ENCODER FEEDBACK
	{"I204=$003504"},
	{"I304=$003503"},
	{"I404=$003505"},
	{"I504=$003506"},
	{"I604=$003507"},
	{"I904=$00350A"},
	{"I1004=$00350C"},
	{"I1104=$00350B"},
	{"I1204=$00350D"},
	{"I1304=$00350E"},
	{"I1404=$00350F"},
	{"I125=$078004"},	//FLAG FEEDBACK
	{"I225=$07800C"},
	{"I325=$078008"},
	{"I425=$078100"},
	{"I525=$078104"},
	{"I625=$078108"},
	{"I925=$078204"},
	{"I1025=$07820C"},
	{"I1125=$078208"},
	{"I1225=$078300"},
	{"I1325=$078304"},
	{"I1425=$078308"},
	{"I7010=7"},			//Encoder Decoder
	{"I7020=7"},
	{"I7030=7"},
	{"I7040=3"},
	{"I7110=7"},
	{"I7120=3"},
	{"I7130=1"},
	{"I7140=0"},
	{"I7210=7"},
	{"I7220=7"},
	{"I7230=7"},
	{"I7240=3"},
	{"I7310=7"},
	{"I7320=3"},
	{"I7330=1"},
	{"I7340=0"},
	{"I909=2"},				//Pump LT Velocity Multiplier
	{"I1009=2"},			//Pump LB Velocity Multiplier
	{"I1109=2"},			//Pump RT Velocity Multiplier
	{"I1209=2"}};			//Pump RB Velocity Multiplier

	switch(theApp.m_nMotorType) {
	default: MessageBox(NULL,_T("Incorrest motor type"),_T("Error"),MB_OK);
		break;
	case PMAC_TURBO:
		DownLoadProgram((char *)StringTurbo,(sizeof StringTurbo)/(sizeof StringTurbo[0]),sizeof StringTurbo[0]);
		break;
	}
}

void CServo::MovetoProgram()
{

	char String[][60]={
		{"OPEN PROG 6 CLEAR"},
		{"M6=6"},
		{"LINEAR"},
		{"FRAX(X,Y,Z,A,B,C,U,V)"},
		{"ABS"},
		{"TM(M2)"},
		{"TA(M132)"},
		{"TS(M9)"},
		{"M1=1"},
		{"X(M15)Y(M25)Z(M35)A(M45)B(M55)C(M65)U(M75)V(M85)"},
		{"DWELL1"},
		{"M1=0"},
		{"M6=0"},
		{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
}

void CServo::ScanProgram()
{

	char String[][60]={
		{"OPEN PROG 4 CLEAR"},
		{"M6=4"},
		{"LINEAR"},
		{"FRAX(X,Y,Z,A,B,C,U,V)"},
		{"ABS"},
		{"TM(M2)"},
		{"TA(M132)"},
		{"TS(M9)"},
		{"WHILE(M1=1)"},
			{"X(M15)Y(M25)Z(M35)A(M45)B(M55)C(M65)U(M75)V(M85)"},
		{"ENDWHILE"},
		{"M6=0"},
		{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
}

void CServo::OriginAllProgram()
{
	

	char String[][30]={
		{"OPEN PROG 9 CLEAR"},
		{"M1=1"},
		{"M6=9"},
		{"IF(I123>0)"},
			{"I123=I123 * -1"},
		{"ENDIF"},
		{"IF(I323<0)"},
			{"I323=I323 * -1"},
		{"ENDIF"},
		{"IF(I423>0)"},
			{"I423=I423 * -1"},
		{"ENDIF"},
		{"IF(I523>0)"},
			{"I523=I523 * -1"},
		{"ENDIF"},
		{"IF(I623>0)"},
			{"I623=I623 * -1"},
		{"ENDIF"},
		{"I124=$120000"},						//Disable limit switches
		{"I126=0"},								//Home Offset = 0
		{"I113=0"},								//disable +ve software limit
		{"I114=0"},								//disable -ve software limit
		{"I7013=2"},								//Flag==-ve limit
		{"I324=$120000"},						//disable limit switches
		{"I326=0"},								//Home Offset = 0
		{"I313=0"},								//disable +ve software limit
		{"I314=0"},								//disable -ve software limit
		{"I7033=1"},								//Flag==+ve limit
		{"I424=$120000"},						//disable limit switches
		{"I426=0"},								//Home Offset = 0
		{"I413=0"},								//disable +ve software limit
		{"I414=0"},								//disable -ve software limit
		{"I7043=2"},								//Flag==-ve limit
		{"I524=$120000"},						//Disable limit switches
		{"I526=0"},								//Home Offset = 0
		{"I513=0"},								//disable +ve software limit
		{"I514=0"},								//disable -ve software limit
		{"I7113=2"},								//Flag==-ve limit
		{"I624=$120000"},						//Disable limit switches
		{"I626=0"},								//Home Offset = 0
		{"I613=0"},								//disable +ve software limit
		{"I614=0"},								//disable -ve software limit
		{"I7123=2"},								//Flag==-ve limit
		{"IF(M17&$400000=$400000)"},		//if already on the -ve limit
			{"I7012=10"},						//Look for falling edge of flag
			{"IF(I123>0)"},						//Reverse Home Direction
				{"I123=I123 * -1"},
			{"ENDIF"},
			{"HOME1"},
			{"I123=I123 * -1"},				//Reverse Home direction
		{"ENDIF"},
		{"IF(M37&$200000=$200000)"},		//if already on the +ve limit
			{"I7032=10"},						//Look for falling edge of flag
			{"IF(I323<0)"},						//Reverse Home Direction
				{"I323=I323 * -1"},
			{"ENDIF"},
			{"HOME3"},
			{"I323=I323 * -1"},				//Reverse Home direction
		{"ENDIF"},
		{"IF(M47&$400000=$400000)"},		//if already on the -ve limit
			{"I7042=10"},						//Look for falling edge of flag
			{"I423=I423 * -1"},					//Reverse Home Direction
			{"HOME4"},
			{"I423=I423 * -1"},				//Reverse Home direction
		{"ENDIF"},
		{"IF(M57&$400000=$400000)"},		//if already on the -ve limit
			{"I7112=10"},						//Look for falling edge of flag
			{"IF(I523>0)"},						//Reverse Home Direction
				{"I523=I523 * -1"},
			{"ENDIF"},
			{"HOME5"},
			{"I523=I523 * -1"},				//Reverse Home direction
		{"ENDIF"},
		{"IF(M67&$400000=$400000)"},		//if already on the -ve limit
			{"I7122=10"},						//Look for falling edge of flag
			{"IF(I623>0)"},						//Reverse Home Direction
				{"I623=I623 * -1"},
			{"ENDIF"},
			{"HOME6"},
			{"I623=I623 * -1"},				//Reverse Home direction
		{"ENDIF"},
		{"I7012=6"},								//Look for rising edge of flag
		{"I7032=6"},								//Look for rising edge of flag
		{"I7042=6"},								//Look for rising edge of flag
		{"I7112=6"},								//Look for rising edge of flag
		{"I7122=6"},								//Look for rising edge of flag
		{"HOME1,3,4,5,6"},
		{"I126=1000"},
		{"I326=-10000"},
		{"I426=10000"},
		{"I526=1000"},
		{"I626=1000"},
		{"I7012=1"},								//Look for rising CHz
		{"I7032=1"},								//Look for rising CHz
		{"I7042=1"},								//Look for rising CHz
		{"I7112=1"},								//Look for rising CHz
		{"I7122=1"},								//Look for rising CHz
		{"I123=I123 * -1"},					//Reverse Home direction
		{"I323=I323 * -1"},					//Reverse Home direction
		{"I423=I423 * -1"},					//Reverse Home direction
		{"I523=I523 * -1"},					//Reverse Home direction
		{"I623=I623 * -1"},					//Reverse Home direction
		{"HOME1,3,4,5,6"},
		{"I124=$100000"},						//reset limit switches
		{"I324=$100000"},						//reset limit switches
		{"I424=$100000"},						//reset limit switches
		{"I524=$100000"},						//reset limit switches
		{"I624=$100000"},						//reset limit switches
		{"I123=I123 * -1"},					//Reverse Home direction
		{"I323=I323 * -1"},					//Reverse Home direction
		{"I423=I423 * -1"},					//Reverse Home direction
		{"I523=I523 * -1"},					//Reverse Home direction
		{"I623=I623 * -1"},					//Reverse Home direction
		{"M1=0"},
		{"M6=0"},
		{"CLOSE"}
	};
	char StringAim[][60]={
	{"OPEN PROG 9 CLEAR"},
	{"M6=9"},
	{"M1=1"},
//Z Axis
	{"P3=I326"},
	{"I326=0"},
	{"I325=$2C008"},
	{"I913=2"},
	{"IF(I323>0)"},
		{"I323=I323*-1"},
	{"ENDIF"},
	{"IF(M35&$400000=$400000)"},
		{"I912=10"},
		{"I323=I323 * -1"},
		{"HOME3"},
		{"I912=5"},
		{"I326=P3"},
		{"HOME3"},
		{"I323=I323 * -1"},
	{"ELSE"},
		{"I912=2"},	//Look for rising edge of limit switch
		{"HOME3"},
		{"I912=10"},
		{"I323=I323 * -1"},
		{"HOME3"},
		{"I912=5"},
		{"I326=P3"},
		{"HOME3"},
		{"I323=I323 * -1"},
	{"ENDIF"},
	{"I325=$C008"},
//Y axis	
	{"P2=I226"},
	{"I226=0"},
	{"I225=$2C004"},
	{"I908=2"},
	{"IF(I223>0)"},
		{"I223=I223*-1"},
	{"ENDIF"},
	{"IF(M25&$400000=$400000)"},
		{"I907=10"},
		{"I223=I223 * -1"},
		{"HOME2"},
		{"I907=5"},
		{"I226=P2"},
		{"HOME2"},
		{"I223=I223 * -1"},
	{"ELSE"},
		{"I907=2"},	//Look for rising edge of limit switch
		{"HOME2"},
		{"I907=10"},
		{"I223=I223 * -1"},
		{"HOME2"},
		{"I907=5"},
		{"I226=P2"},
		{"HOME2"},
		{"I223=I223 * -1"},
	{"ENDIF"},
	{"I225=$C004"},
//X Axis
	{"I125=$C000"},
	{"I903=2"},			//+VE LIMIT SWITCH
	{"IF(I123>0)"},
		{"I123=I123*-1"},
	{"ENDIF"},
	{"IF(M15&$400000=$400000)"},
		{"I902=10"},
		{"I123=I123 * -1"},
		{"HOME1"},
		{"I123=I123 * -1"},
	{"ENDIF"},
	{"I125=$2C000"},
	{"I902=2"},				//rising edge of flag only
	{"HOME1"},
	{"I125=$C000"},

	{"M1=0"},
	{"M6=0"},
	{"CLOSE"}};

	switch(theApp.m_nJobNumber) {
	default:	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
		break;
	case AIM_COMPOSITES:
		DownLoadProgram((char *)StringAim,(sizeof StringAim)/(sizeof StringAim[0]),sizeof StringAim[0]);
		break;
	}
}

void CServo::OriginAllS455Program()
{

	char String[][30]={
	{"OPEN PROG 10 CLEAR"},
	{"M1=1"},
	{"M6=10"},
	{"IF(I323>0)"},					// Z AXIS FIRST
		{"I323=I323 * -1"},
	{"ENDIF"},
	{"I324=$120000"},				//disable limit switches
	{"P103=I326"},
	{"I326=0"},						//Home Offset = 0
	{"I313=0"},						//disable +ve software limit
	{"I314=0"},						//disable -ve software limit
	{"I7033=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M37&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7032=10"},				//Look for falling edge of flag
		{"IF(I323<0)"},				//Reverse Home Direction
			{"I323=I323 * -1"},
		{"ENDIF"},
		{"HOME3"},
		{"I323=I323 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7032=6"},					//Look for rising edge of flag
	{"HOME3"},
	{"I323=I323 * -1"},				//Reverse Home direction
	{"I7032=1"},					//Look for rising CHz
	{"I326=P103"},
	{"HOME3"},
	{"I324=$100000"},				//reset limit switches
	{"I323=I323 * -1"},				//Reverse Home direction

	{"IF(I123>0)"},					//X AXIS NEXT
		{"I123=I123 * -1"},
	{"ENDIF"},
	{"I124=$120000"},				//disable limit switches
	{"P101=I226"},
	{"I126=0"},						//Home Offset = 0
	{"I113=0"},						//disable +ve software limit
	{"I114=0"},						//disable -ve software limit
	{"I7013=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M17&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7012=10"},				//Look for falling edge of flag
		{"IF(I123<0)"},				//Reverse Home Direction
			{"I123=I123 * -1"},
		{"ENDIF"},
		{"HOME1"},
		{"I123=I123 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7012=6"},					//Look for rising edge of flag
	{"HOME1"},
	{"I123=I123 * -1"},				//Reverse Home direction
	{"I7012=1"},					//Look for rising CHz
	{"I126=P101"},
	{"HOME1"},
	{"I124=$100000"},				//reset limit switches
	{"I123=I123 * -1"},				//Reverse Home direction

	{"IF(I223>0)"},					//NOW Y AXIS
		{"I223=I223 * -1"},
	{"ENDIF"},
	{"I224=$120000"},				//disable limit switches
	{"P102=I226"},
	{"I226=0"},						//Home Offset = 0
	{"I213=0"},						//disable +ve software limit
	{"I214=0"},						//disable -ve software limit
	{"I7023=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M27&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7022=10"},				//Look for falling edge of flag
		{"IF(I223<0)"},				//Reverse Home Direction
			{"I223=I223 * -1"},
		{"ENDIF"},
		{"HOME2"},
		{"I223=I223 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7022=6"},					//Look for rising edge of flag
	{"HOME2"},
	{"I223=I223 * -1"},				//Reverse Home direction
	{"I7022=1"},					//Look for rising CHz
	{"I226=P102"},
	{"HOME2"},
	{"I224=$100000"},				//reset limit switches
	{"I223=I223 * -1"},				//Reverse Home direction
	
	
	{"IF(I423>0)"},					//XTIP AXIS
		{"I423=I423 * -1"},
	{"ENDIF"},
	{"I424=$120000"},				//disable limit switches
	{"P104=I426"},
	{"I426=0"},						//Home Offset = 0
	{"I413=0"},						//disable +ve software limit
	{"I414=0"},						//disable -ve software limit
	{"I7043=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M47&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7042=10"},				//Look for falling edge of flag
		{"IF(I423<0)"},				//Reverse Home Direction
			{"I423=I423 * -1"},
		{"ENDIF"},
		{"HOME4"},
		{"I423=I423 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7042=6"},					//Look for rising edge of flag
	{"HOME4"},
	{"I423=I423 * -1"},				//Reverse Home direction
	{"I7042=1"},					//Look for rising CHz
	{"I426=P104"},
	{"HOME4"},
	{"I424=$120000"},				//reset limit switches
	{"I423=I423 * -1"},				//Reverse Home direction

	{"IF(I523>0)"},					//YTIP AXIS
		{"I523=I523 * -1"},
	{"ENDIF"},
	{"I524=$120000"},				//disable limit switches
	{"P105=I526"},
	{"I526=0"},						//Home Offset = 0
	{"I513=0"},						//disable +ve software limit
	{"I514=0"},						//disable -ve software limit
	{"I7113=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M57&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7112=10"},				//Look for falling edge of flag
		{"IF(I523<0)"},				//Reverse Home Direction
			{"I523=I523 * -1"},
		{"ENDIF"},
		{"HOME5"},
		{"I523=I523 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7112=6"},					//Look for rising edge of flag
	{"HOME5"},
	{"I523=I523 * -1"},				//Reverse Home direction
	{"I7112=1"},					//Look for rising CHz
	{"I526=P105"},
	{"HOME5"},
	{"I524=$120000"},				//reset limit switches
	{"I523=I523 * -1"},				//Reverse Home direction

	
	{"M1=0"},
	{"M6=0"},
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);


}



void CServo::OriginX0Program()
{
	

	char String[][30]={
	{"OPEN PROG 11 CLEAR"},
	{"M1=1"},
	{"M6=11"},
	{"IF(I123>0)"},
		{"I123=I123 * -1"},
	{"ENDIF"},
	{"I124=$120000"},				//disable limit switches
	{"P101=I226"},
	{"I126=0"},						//Home Offset = 0
	{"I113=0"},						//disable +ve software limit
	{"I114=0"},						//disable -ve software limit
	{"I7013=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M17&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7012=10"},				//Look for falling edge of flag
		{"IF(I123<0)"},				//Reverse Home Direction
			{"I123=I123 * -1"},
		{"ENDIF"},
		{"HOME1"},
		{"I123=I123 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7012=6"},					//Look for rising edge of flag
	{"HOME1"},
	{"I123=I123 * -1"},				//Reverse Home direction
	{"I7012=1"},					//Look for rising CHz
	{"I126=P101"},
	{"HOME1"},
	{"I124=$100000"},				//reset limit switches
	{"I123=I123 * -1"},				//Reverse Home direction
	{"M1=0"},
	{"M6=0"},
	{"CLOSE"}};

	char StringAim[][30]={
	{"OPEN PROG 11 CLEAR"},
	{"M1=1"},
	{"M6=11"},
	{"I125=$C000"},
	{"I903=2"},			//+VE LIMIT SWITCH
	{"IF(I123>0)"},
		{"I123=I123*-1"},
	{"ENDIF"},
	{"IF(M17&$400000=$400000)"},
		{"I902=10"},
		{"I123=I123 * -1"},
		{"HOME1"},
		{"I123=I123 * -1"},
	{"ENDIF"},
	{"I125=$2C000"},
	{"I902=2"},				//rising edge of flag only
	{"HOME1"},
	{"I125=$C000"},
	{"M1=0"},
	{"M6=0"},
	{"CLOSE"}};

	switch(theApp.m_nJobNumber) {
	default:	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
		break;
	case AIM_COMPOSITES: DownLoadProgram((char *)StringAim,(sizeof StringAim)/(sizeof StringAim[0]),sizeof StringAim[0]);
		break;
	}
}
void CServo::OriginY0Program()
{
	
	char String[][30]={
	{"OPEN PROG 12 CLEAR"},
	{"M1=1"},
	{"M6=12"},
	{"IF(I223>0)"},
		{"I223=I223 * -1"},
	{"ENDIF"},
	{"I224=$120000"},				//disable limit switches
	{"P102=I226"},
	{"I226=0"},						//Home Offset = 0
	{"I213=0"},						//disable +ve software limit
	{"I214=0"},						//disable -ve software limit
	{"I7023=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M27&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7022=10"},				//Look for falling edge of flag
		{"IF(I223<0)"},				//Reverse Home Direction
			{"I223=I223 * -1"},
		{"ENDIF"},
		{"HOME2"},
		{"I223=I223 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7022=6"},					//Look for rising edge of flag
	{"HOME2"},
	{"I223=I223 * -1"},				//Reverse Home direction
	{"I7022=1"},					//Look for rising CHz
	{"I226=P102"},
	{"HOME2"},
	{"I224=$100000"},				//reset limit switches
	{"I223=I223 * -1"},				//Reverse Home direction
	{"M1=0"},
	{"M6=0"},
	{"CLOSE"}};

	char StringAim[][30]={
	{"OPEN PROG 12 CLEAR"},
	{"M1=1"},
	{"M6=12"},
	{"I225=$C004"},
	{"I908=2"},			//+VE LIMIT SWITCH
	{"IF(I223>0)"},
		{"I223=I223*-1"},
	{"ENDIF"},
	{"IF(M27&$400000=$400000)"},
		{"I907=10"},
		{"I223=I223 * -1"},
		{"HOME2"},
		{"I223=I223 * -1"},
	{"ENDIF"},
	{"I225=$2C004"},
	{"I907=2"},				//rising edge of flag only
	{"HOME2"},
	{"I225=$C004"},
	{"M1=0"},
	{"M6=0"},
	{"CLOSE"}};

	switch(theApp.m_nJobNumber) {
	default:	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
		break;
	case AIM_COMPOSITES: DownLoadProgram((char *)StringAim,(sizeof StringAim)/(sizeof StringAim[0]),sizeof StringAim[0]);
		break;
	}
}

void CServo::OriginZ0Program()
{

	char String[][30]={
	{"OPEN PROG 13 CLEAR"},
	{"M1=1"},
	{"M6=13"},
	{"IF(I323>0)"},
		{"I323=I323 * -1"},
	{"ENDIF"},
	{"I324=$120000"},				//disable limit switches
	{"P103=I326"},
	{"I326=0"},						//Home Offset = 0
	{"I313=0"},						//disable +ve software limit
	{"I314=0"},						//disable -ve software limit
	{"I7033=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M37&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7032=10"},				//Look for falling edge of flag
		{"IF(I323<0)"},				//Reverse Home Direction
			{"I323=I323 * -1"},
		{"ENDIF"},
		{"HOME3"},
		{"I323=I323 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7032=6"},					//Look for rising edge of flag
	{"HOME3"},
	{"I323=I323 * -1"},				//Reverse Home direction
	{"I7032=1"},					//Look for rising CHz
	{"I326=P103"},
	{"HOME3"},
	{"I324=$100000"},				//reset limit switches
	{"I323=I323 * -1"},				//Reverse Home direction
	{"M1=0"},
	{"M6=0"},
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
}

void CServo::OriginXt0Program()
{

	char String[][30]={
	{"OPEN PROG 14 CLEAR"},
	{"M1=1"},
	{"M6=14"},
	{"IF(I423>0)"},
		{"I423=I423 * -1"},
	{"ENDIF"},
	{"I424=$120000"},				//disable limit switches
	{"P104=I426"},
	{"I426=0"},						//Home Offset = 0
	{"I413=0"},						//disable +ve software limit
	{"I414=0"},						//disable -ve software limit
	{"I7043=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M47&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7042=10"},				//Look for falling edge of flag
		{"IF(I423<0)"},				//Reverse Home Direction
			{"I423=I423 * -1"},
		{"ENDIF"},
		{"HOME4"},
		{"I423=I423 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7042=6"},					//Look for rising edge of flag
	{"HOME4"},
	{"I423=I423 * -1"},				//Reverse Home direction
	{"I7042=1"},					//Look for rising CHz
	{"I426=P104"},
	{"HOME4"},
	{"I424=$120000"},				//reset limit switches
	{"I423=I423 * -1"},				//Reverse Home direction
	{"M1=0"},
	{"M6=0"},
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
}

void CServo::OriginYt0Program()
{

	char String[][30]={
	{"OPEN PROG 15 CLEAR"},
	{"M1=1"},
	{"M6=15"},
	{"IF(I523>0)"},
		{"I523=I523 * -1"},
	{"ENDIF"},
	{"I524=$120000"},				//disable limit switches
	{"P105=I526"},
	{"I526=0"},						//Home Offset = 0
	{"I513=0"},						//disable +ve software limit
	{"I514=0"},						//disable -ve software limit
	{"I7113=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M57&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7112=10"},				//Look for falling edge of flag
		{"IF(I523<0)"},				//Reverse Home Direction
			{"I523=I523 * -1"},
		{"ENDIF"},
		{"HOME5"},
		{"I523=I523 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7112=6"},					//Look for rising edge of flag
	{"HOME5"},
	{"I523=I523 * -1"},				//Reverse Home direction
	{"I7112=1"},					//Look for rising CHz
	{"I526=P105"},
	{"HOME5"},
	{"I524=$120000"},				//reset limit switches
	{"I523=I523 * -1"},				//Reverse Home direction
	{"M1=0"},
	{"M6=0"},
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
}

void CServo::OriginR0Program()
{

	char String[][30]={
	{"OPEN PROG 16 CLEAR"},
	{"M1=1"},
	{"M6=16"},
	{"IF(I623<0)"},
		{"I623=I623 * -1"},
	{"ENDIF"},
	{"I624=$120000"},				//disable limit switches
	{"P106=I626"},
	{"I626=0"},						//Home Offset = 0
	{"I613=0"},						//disable +ve software limit
	{"I614=0"},						//disable -ve software limit
	{"I7123=2"},					//Flag==1:+ve limit 2:-ve limit
	{"IF(M67&$400000=$400000)"},	//if already on the 200000:+ve limit 400000:-ve limit
		{"I7122=10"},				//Look for falling edge of flag
		{"IF(I623>0)"},				//Reverse Home Direction
			{"I623=I623 * -1"},
		{"ENDIF"},
		{"HOME6"},
		{"I623=I623 * -1"},			//Reverse Home direction
	{"ENDIF"},
	{"I7122=6"},					//Look for rising edge of flag
	{"HOME6"},
	{"I623=I623 * -1"},				//Reverse Home direction
	{"I7122=1"},					//Look for rising CHz
	{"I626=P106"},
	{"HOME6"},
	{"I624=$120000"},				//reset limit switches
	{"I623=I623 * -1"},				//Reverse Home direction
	{"M1=0"},
	{"M6=0"},
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
}
void CServo::JoystickProgram()
{
	char String[][50]={
	{"OPEN PROG 7 CLEAR"},
	{"M6=7"},
	{"SPLINE1"},
	{"FRAX(X,Y,Z,A,B,C,U,V)"},
	{"ABS"},
	{"TM10"},
	{"TA500"},
	{"TS500"},
	{"WHILE(M1=1)"},
		{"X(M15)Y(M25)Z(M35)A(M45)B(M55)C(M65)"},
	{"ENDWHILE"},
	{"M6=0"},
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
}

void CServo::JoystickPlc()
{
	char String[][50]={
	{"OPEN PLC 2 CLEAR"},
	{"P100=P100+1"},		//only happens once every 100 ms
	{"IF(P100>100)"},
		{"P100=0"},

		{"IF(M7=0)"},
			{"IF(M16!=P10)"},
				{"P10=M16"},
				{"IF(P10=0)"},
					{"CMD\x022#1j/\x022"},
				{"ENDIF"},
				{"IF(P10>0)"},
					{"I122=P10/100"},
					{"CMD\x022#1j+\x022"},
				{"ENDIF"},
				{"IF(P10<0)"},
					{"I122=P10/-100"},
					{"CMD\x022#1j-\x022"},
				{"ENDIF"},
			{"ENDIF"},
		{"ELSE"},
			{"I122=I116"},
			{"M19=M15"},
			{"CMD\x022#1j=*\x022"},
		{"ENDIF"},

		{"IF(M7=0)"},
			{"IF(M26 != P20)"},
				{"P20=M26"},
				{"IF(P20=0)"},
					{"CMD\x022#2j/\x022"},
				{"ENDIF"},
				{"IF(P20>0)"},
					{"I222=P20/100"},
					{"CMD\x022#2j+\x022"},
				{"ENDIF"},
				{"IF(P20<0)"},
					{"I222=P20/-100"},
					{"CMD\x022#2j-\x022"},
				{"ENDIF"},
			{"ENDIF"},
		{"ELSE"},
			{"I222=I216"},
			{"M29=M25"},
			{"CMD\x022#2j=*\x022"},
		{"ENDIF"},

		{"IF(M7=0)"},
			{"IF(M36 != P30)"},
				{"P30=M36"},
				{"IF(P30=0)"},
					{"CMD\x022#3j/\x022"},
				{"ENDIF"},
					{"IF(P30>0)"},
					{"I322=P30/100"},
					{"CMD\x022#3j+\x022"},
				{"ENDIF"},
				{"IF(P30<0)"},
					{"I322=P30/-100"},
					{"CMD\x022#3j-\x022"},
				{"ENDIF"},
			{"ENDIF"},
		{"ELSE"},
			{"I322=I316"},
			{"M39=M35"},
			{"CMD\x022#3j=*\x022"},
		{"ENDIF"},

		{"IF(M46 != P40)"},
			{"P40=M46"},
			{"IF(P40=0)"},
				{"CMD\x022#4j/\x022"},
			{"ENDIF"},
			{"IF(P40>0)"},
				{"I422=P40/100"},
				{"CMD\x022#4j+\x022"},
			{"ENDIF"},
			{"IF(P40<0)"},
				{"I422=P40/-100"},
				{"CMD\x022#4j-\x022"},
			{"ENDIF"},
		{"ENDIF"},
		{"IF(M56 != P50)"},
			{"P50=M56"},
			{"IF(P50=0)"},
				{"CMD\x022#5j/\x022"},
			{"ENDIF"},
			{"IF(P50>0)"},
				{"I522=P50/100"},
				{"CMD\x022#5j+\x022"},
			{"ENDIF"},
			{"IF(P50<0)"},
				{"I522=P50/-100"},
				{"CMD\x022#5j-\x022"},
			{"ENDIF"},
		{"ENDIF"},
		{"IF(M66 != P60)"},
			{"P60=M66"},
			{"IF(P60=0)"},
				{"CMD\x022#6j/\x022"},
			{"ENDIF"},
			{"IF(P60>0)"},
				{"I622=P60/100"},
				{"CMD\x022#6j+\x022"},
			{"ENDIF"},
			{"IF(P60<0)"},
				{"I622=P60/-100"},
				{"CMD\x022#6j-\x022"},
			{"ENDIF"},
		{"ENDIF"},
		{"IF(M76 != P70)"},
			{"P70=M76"},
			{"IF(P70=0)"},
				{"CMD\x022#7j/\x022"},
			{"ENDIF"},
			{"IF(P70>0)"},
				{"I722=P70/100"},
				{"CMD\x022#7j+\x022"},
			{"ENDIF"},
			{"IF(P70<0)"},
				{"I722=P70/-100"},
				{"CMD\x022#7j-\x022"},
			{"ENDIF"},
		{"ENDIF"},
	{"ENDIF"},
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);

}





void CServo::DesirePosEqualsCoord(COORDDATA *Cp)
{
	

	theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)(Cp->Side0.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos=(int)(Cp->Side0.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos=(int)(Cp->Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
	theApp.m_Axes[R0AXIS].nDesirePos=(int)(Cp->Side0.fR/theApp.m_Axes[R0AXIS].fStepSize);
	theApp.m_Axes[XT0AXIS].nDesirePos=(int)(Cp->Side0.fXt/theApp.m_Axes[XT0AXIS].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos=(int)(Cp->Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)(Cp->Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
	UpdatePmacDesirePos();
	
}


void CServo::UpdatePmacDesirePos()
{
	
	int	ii;


	for(ii=0;ii<8;ii++) {
		if(theApp.m_Axes[ii].bAvailable) {
			WriteDpram(theApp.m_Axes[ii].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset);
		};
	};

}


void CServo::StartScan()
{
	

	int	ii=0;
	int	nMax,nTemp;

	nMax=0;
	for(ii=0;ii<8;ii++) {
		if(theApp.m_Axes[ii].bAvailable) {
			WriteDpram(theApp.m_Axes[ii].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset);
			nTemp=CalcMoveTime(&theApp.m_Axes[ii],labs(theApp.m_Axes[ii].nDesirePos-theApp.m_Axes[ii].nPos));
			if(nTemp>nMax) nMax=nTemp;
		};
	};

	nMax=100;
	WriteDpram(PMAC_MOVE_FLAG_1,1);
	WriteDpram(PMAC_TM,nMax);
	WriteDpram(PMAC_TA,0);
	WriteDpram(PMAC_TS,300);
	CommsStr("&1 B4 R");
	Sleep(100);
}


void CServo::MoveTo(BOOL bAction)

{
	

	int	ii=0;
	int	nMax,nTemp;

	nMax=0;
	for(ii=0;ii<8;ii++) {
		if(theApp.m_Axes[ii].bEnabled) {
			WriteDpram(theApp.m_Axes[ii].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset);
			nTemp=CalcMoveTime(&theApp.m_Axes[ii],labs(theApp.m_Axes[ii].nDesirePos-theApp.m_Axes[ii].nPos));
			if(nTemp>nMax) nMax=nTemp;
		};
	};

	WriteDpram(PMAC_MOVE_FLAG_1,1);
	WriteDpram(PMAC_TM,nMax);
	WriteDpram(PMAC_TA,0);
	WriteDpram(PMAC_TS,300);
	CommsStr("&1 B6 R");
	Sleep(100);
	while(!IsMotionFinished() && (bAction==WAIT)) {};
}


int CServo::CalcMoveTime(struct AxisData *pAxis,int	nTotalS)
{
	float	vel,accel;
	float	s0,s1,s4,ds;
	float	t0,t1,t4,dt;
	int	nTemp;


	s4=(float)nTotalS;

	vel=pAxis->fMaxMoveSpeed/pAxis->fStepSize/1000.0f;//this assumes mm/sec
	vel=pAxis->fMaxMoveSpeed;
	accel=pAxis->fAcceleration;

	if(accel==0.0) accel=1.0;

	t0=s0=0;
	t1=vel/accel;
	s1=0.5f * accel * (t1 * t1);
	if((s1 * 2) > s4) {
		s1 = s4 / 2;
		t1=(float)sqrt((2.0 * s1) / accel);
		t4= t1 * 2;
	} else {
		ds = (s4 - (s1 * 2));
		dt = ds / vel;
		t4 = t1 * 2 + dt;
	};

	nTemp=(int)t4;
	return nTemp;
}

BOOL CServo::IsAxisInPosition(int nWhich)
{
	
	if(!(theApp.m_Axes[nWhich].nStatus&PMAC_IN_POS)) return FALSE;
	else return TRUE;
}




BOOL CServo::IsMotionFinished()
{
	
	int	ii;

	for(ii=0;ii<10;ii++) {
		if(theApp.m_Axes[ii].bAvailable) {
			if(!(theApp.m_Axes[ii].nStatus&PMAC_IN_POS)) return FALSE; //something has not finished
		}
	}
	return TRUE;	// All motion has finished
}
BOOL CServo::IsPolyMotionFinished()
{
	
	int	ii;

	for(ii=0;ii<9;ii++) {	//Note only 9 Axes
		if(theApp.m_Axes[ii].bAvailable) {
			if(!(theApp.m_Axes[ii].nStatus&PMAC_IN_POS)) return FALSE; //something has not finished
		}
	}
	return TRUE;	// All motion has finished
}

bool CServo::IsOriginFinished(int nAxis)
{

	

	if(theApp.m_Axes[nAxis].bAvailable) {
		if(!(theApp.m_Axes[nAxis].nStatus&PMAC_IN_POS)) return FALSE; //something has not finished
		if(!(theApp.m_Axes[nAxis].nStatus&PMAC_HOMED)) return FALSE; //something has not finished
		if(GetProgramRunning()==1) return FALSE;
	}
	return TRUE;	// All motion has finished
}

int CServo::GetProgramRunning()
{
	int	nn=ReadDpram(PMAC_MOVE_FLAG_1);
	return nn;

}


//RETURNS ZERO IF ALL AXES HAVE ZERO VELOCITY && WITHIN ERROR BAND
/*
pmac_move_finished(int action)
{
#ifdef	_HWP
int	ii;
int	bit=1;
long	flag;
unsigned int	finish;

finish=0;
for(ii=0;ii<5;ii++,bit<<=1) {
	if(axes_struct[ii]->enabled==TRUE) {
		flag=*(axes_struct[ii]->pmac_addrs+PMAC_MOTOR_STATUS);
		switch(action) {
		case -1:
			break;
		case 0:	if(labs(axes_struct[ii]->pos-axes_struct[ii]->desire_pos) >
												axes_struct[ii]->err) finish|=bit;
			break;
		default:	if(!(flag & action)) finish |= bit;
			break;
		};
	};
};
for(ii=6;ii<11;ii++,bit<<=1) {
	if(axes_struct[ii]->enabled==TRUE) {
		flag=*(axes_struct[ii]->pmac_addrs+PMAC_MOTOR_STATUS);
		switch(action) {
		case -1:
			break;
		case 0:	if(labs(axes_struct[ii]->pos-axes_struct[ii]->desire_pos) >
												axes_struct[ii]->err) finish|=bit;
			break;
		default:	if(!(flag & action)) finish |= bit;
			break;
		};
	};
};
if(!(*(pmac.mem_addrs + PMAC_MOVE_FLAG))) finish |= 1;

return finish;
#endif
}
*/


bool CServo::WaitForFreeCoordinateSystems()
{
	while(ReadDpram(PMAC_COORDINATE_STATUS)) {

	};
	return TRUE;
}



bool CServo::SuspendJoystick(int nCoordinateMode)
{
	
	char Str[40];
	int nAxis;
	if(m_bWorking[m_nCardNumber]==FALSE) return FALSE;

	m_Pmac.bJoystickEnabled=FALSE;
	m_nThreadAction=0;

	WriteDpram(PMAC_MOVE_FLAG_1,0);
	CommsStr("&1 Q");
	CommsStr("&1 A");
	CommsStr("&2 Q");
	CommsStr("&2 A");

	CommsStr("UNDEFINE ALL");
	CommsStr("DISABLE PLC 2");
	DisableLookAhead();
	WaitForFreeCoordinateSystems();
	theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=FALSE;
	theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=FALSE;
	theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=FALSE;
	theApp.m_Axes[XT0AXIS].bEnabled=FALSE;
	theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=FALSE;
	theApp.m_Axes[R0AXIS].bEnabled=FALSE;
	theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=FALSE;
	theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=FALSE;
	theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=FALSE;
	theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=FALSE;
	theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=FALSE;
	theApp.m_Axes[R1AXIS].bEnabled=FALSE;
	Sleep(100);
//	while(IsMotionFinished()) {};

	switch(nCoordinateMode) {
	case 0:
		if(AreMotorsPowerd()==FALSE) return FALSE;
		if(IsMovementAllowed()==FALSE) return FALSE;
		CommsStr("&1");
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			sprintf_s(Str,"#%d->X#%dj/",theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};		
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			sprintf_s(Str,"#%d->Y#%dj/",theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};		
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			sprintf_s(Str,"#%d->Z#%dj/",theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};		
		if((theApp.m_Axes[XT0AXIS].bAvailable==TRUE) && (DoesAxisNeedHoming(XT0AXIS)==FALSE)) {
			sprintf_s(Str,"#%d->A#%dj/",theApp.m_Axes[XT0AXIS].nPhysicalAxis,theApp.m_Axes[XT0AXIS].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[XT0AXIS].bEnabled=theApp.m_Axes[XT0AXIS].bAvailable;
		};		
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			sprintf_s(Str,"#%d->B#%dj/",theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};		
		if((theApp.m_Axes[R0AXIS].bAvailable==TRUE) && (DoesAxisNeedHoming(R0AXIS)==FALSE)) {
			sprintf_s(Str,"#%d->C#%dj/",theApp.m_Axes[R0AXIS].nPhysicalAxis,theApp.m_Axes[R0AXIS].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[R0AXIS].bEnabled=theApp.m_Axes[R0AXIS].bAvailable;
		};		
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			sprintf_s(Str,"#%d->U#%dj/",theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		break;
	case 1:
		//No coordinate System but enable the amplifiers
		//Joystick Plc
		if(AreMotorsPowerd()==FALSE) return FALSE;
		if(IsMovementAllowed()==FALSE) return FALSE;
		for(nAxis=0;nAxis<7;nAxis++) {
			if((theApp.m_Axes[nAxis].bAvailable==TRUE) && (DoesAxisNeedHoming(nAxis)==FALSE)) {
				sprintf_s(Str,"#%dj/",theApp.m_Axes[nAxis].nPhysicalAxis);
				CommsStr(Str);
				theApp.m_Axes[nAxis].bEnabled=theApp.m_Axes[nAxis].bAvailable;
			};
		};
		break;
	case 2:
		//Nome Find enable
		//No home find check done
		if(AreMotorsPowerd()==FALSE) return FALSE;
		CommsStr("&1");
		if(theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) {
			sprintf_s(Str,"#%d->X#%dj/",theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};		
		if(theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) {
			sprintf_s(Str,"#%d->Y#%dj/",theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};		
		if(theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) {
			sprintf_s(Str,"#%d->Z#%dj/",theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};		
		if(theApp.m_Axes[XT0AXIS].bAvailable==TRUE) {
			sprintf_s(Str,"#%d->A#%dj/",theApp.m_Axes[XT0AXIS].nPhysicalAxis,theApp.m_Axes[XT0AXIS].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[XT0AXIS].bEnabled=theApp.m_Axes[XT0AXIS].bAvailable;
		};		
		if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) {
			sprintf_s(Str,"#%d->B#%dj/",theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};		
		if(theApp.m_Axes[R0AXIS].bAvailable==TRUE) {
			sprintf_s(Str,"#%d->C#%dj/",theApp.m_Axes[R0AXIS].nPhysicalAxis,theApp.m_Axes[R0AXIS].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[R0AXIS].bEnabled=theApp.m_Axes[R0AXIS].bAvailable;
		};
		break;
	case 3:
		//Turntable Only
		//No home find check done
		if(AreMotorsPowerd()==FALSE) return FALSE;
		CommsStr("&1");
		if(theApp.m_Axes[R0AXIS].bAvailable==TRUE) {
			sprintf_s(Str,"#%d->C#%dj/",theApp.m_Axes[R0AXIS].nPhysicalAxis,theApp.m_Axes[R0AXIS].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[R0AXIS].bEnabled=theApp.m_Axes[R0AXIS].bAvailable;
		};
		break;
	case 4:
		//Rotate axis not include for turntable or roller continuous
		if(AreMotorsPowerd()==FALSE) return FALSE;
		if(IsMovementAllowed()==FALSE) return FALSE;
		CommsStr("&1");
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			sprintf_s(Str,"#%d->X#%dj/",theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};		
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			sprintf_s(Str,"#%d->Y#%dj/",theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};		
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			sprintf_s(Str,"#%d->Z#%dj/",theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};		
		if((theApp.m_Axes[XT0AXIS].bAvailable==TRUE) && (DoesAxisNeedHoming(XT0AXIS)==FALSE)) {
			sprintf_s(Str,"#%d->A#%dj/",theApp.m_Axes[XT0AXIS].nPhysicalAxis,theApp.m_Axes[XT0AXIS].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[XT0AXIS].bEnabled=theApp.m_Axes[XT0AXIS].bAvailable;
		};		
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			sprintf_s(Str,"#%d->B#%dj/",theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			CommsStr(Str);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};		
		break;

	}
	return TRUE;

}


int CServo::RequestVariable(CString VariableName, int nOffset, CString &Result)
{
	char chrName[5000];
	char chrResult[5000];

	Result.Format(_T(""));

	CStringToChar(VariableName,chrName);
	RequestVariable(chrName, nOffset, chrResult);
	CharToCString(chrResult,Result);

	return 1;

}



int CServo::RequestVariable(char *VariableName, int nOffset, char *Result)
{
	char str[5000];

	sprintf_s(str,"%s%d",VariableName,nOffset);
	CommsStr(str);
	Sleep(50);

	Result[0]=0;
	if(CommsGetStr(Result)==0) MessageBox(NULL,_T("Failed to RX str"),_T("Error"),NULL);
	return strlen(Result);

}

int CServo::RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, CString &Result)
{

	char chrName[5000];
	char chrResult[5000];

	CStringToChar(VariableName,chrName);
	RequestMultipleVariable(chrName, nStartOffset, nFinishOffset, chrResult);
	CharToCString(chrResult,Result);

	return 1;
}


int CServo::RequestMultipleVariable(char *VariableName, int nStartOffset,int nFinishOffset, char *Result)
{
	char str[5000];

	sprintf_s(str,"%s%d..%d",VariableName,nStartOffset,nFinishOffset);
	CommsStr(str);

	Result[0]=0;
	if(CommsGetStr(Result)==0) MessageBox(NULL,_T("Failed to RX str"),_T("Error"),NULL);
	return strlen(Result);

}

int CServo::RequestMVariableDefinitions(CString VariableName, int nStartOffset,int nFinishOffset, CString &Result)
{

	char chrName[5000];
	char chrResult[5000];

	CStringToChar(VariableName,chrName);
	RequestMVariableDefinitions(chrName, nStartOffset, nFinishOffset, chrResult);
	CharToCString(chrResult,Result);

	return 1;
}

int CServo::RequestMVariableDefinitions(char *VariableName, int nStartOffset,int nFinishOffset, char *Result)
{
	char str[5000];
	
	sprintf_s(str,"%s%d..%d->",VariableName,nStartOffset,nFinishOffset);
	CommsStr(str);

	Result[0]=0;
	if(CommsGetStr(Result)==0) MessageBox(NULL,_T("Failed to RX str"),_T("Error"),NULL);
	return strlen(Result);

}

void CServo::SetVariable(char *VariableName,int nOffset,int nValue)
{
	char str[30];

	sprintf_s(str,"%s%d=%d",VariableName,nOffset,nValue);
	CommsStr(str);

}

void CServo::SetVariable(char *VariableName,int nOffset,float fValue)
{
	char str[30];

	sprintf_s(str,"%s%d=%.03f",VariableName,nOffset,fValue);
	CommsStr(str);

}

void CServo::SetVariable(char *VariableName,int nOffset,double dValue)
{
	char str[30];


	sprintf_s(str,"%s%d=%.03f",VariableName,nOffset,dValue);
	CommsStr(str);

}

void CServo::SetVariable(char *VariableName,int nOffset,char *pStr)
{
	char str[30];


	sprintf_s(str,"%s%d=%s",VariableName,nOffset,pStr);
	CommsStr(str);

}



void CServo::Reset()
{
	//I94 should be on

	Outp(12+m_nPmacAddrs,0x40);
	Outp(10+m_nPmacAddrs,0x40);
	Sleep(20);
	//You should now reintialze comms and any interrupts
}


void CServo::StartJoystick()
{
	
	int nJoyValue[4];


	SuspendJoystick(1);

	switch(theApp.m_Tank.nJoystickSource) {
	case WIRED_MSC:
		m_Pmac.nXJoyValue=theApp.m_UtUser.MscAdcRead(0);
		m_Pmac.nYJoyValue=theApp.m_UtUser.MscAdcRead(1);
		m_Pmac.nZJoyValue=theApp.m_UtUser.MscAdcRead(2);
		m_Pmac.nSJoyValue=theApp.m_UtUser.MscAdcRead(3);
		break;
	}
	nJoyValue[0]=m_Pmac.nXJoyValue;
	nJoyValue[1]=m_Pmac.nYJoyValue;
	nJoyValue[2]=m_Pmac.nZJoyValue;
	nJoyValue[3]=m_Pmac.nSJoyValue;

	for(int nAxis=0;nAxis<8;nAxis++) {
		theApp.m_Axes[nAxis].nJoystickZero = nJoyValue[theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickNumber % 4];
		theApp.m_Axes[nAxis].nDesirePos=theApp.m_Axes[nAxis].nPos;
		if(theApp.m_Axes[nAxis].bAvailable==TRUE)
			WriteDpram(theApp.m_Axes[nAxis].nPhysicalAxis-1+PMAC_JOY_DEV,0);
	}


	theApp.m_Thread.m_nThreadAction = 10;

	//Motion control joystick
//	WriteDpram(PMAC_MOVE_FLAG_1,1);
//	CommsStr("&1 B7 R");
//	m_Pmac.bJoystickEnabled=TRUE;

	//PLC controlled joystick
	WriteDpram(PMAC_JOYSTICK_TIP_MODE, 0);
	CommsStr("ENABLE PLC 2");


/*
	theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=theApp.joystickDlg.m_nXAxis;
	theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=theApp.joystickDlg.m_nXAxis;
	theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=theApp.joystickDlg.m_nYAxis;
	theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=theApp.joystickDlg.m_nYAxis;
	theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=theApp.joystickDlg.m_nZAxis;
	theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=theApp.joystickDlg.m_nZAxis;
	theApp.m_Axes[R0AXIS].nJoystickZero=theApp.joystickDlg.m_nSAxis;
	theApp.m_Axes[R1AXIS].nJoystickZero=theApp.joystickDlg.m_nSAxis;
	theApp.m_Axes[theApp.m_Tank.nXRight].nRLdiff=theApp.m_Axes[theApp.m_Tank.nXRight].nRLdiff=theApp.m_Axes[theApp.m_Tank.nXRight].nPos-theApp.m_Axes[theApp.m_Tank.nXRight].nPos;	
	theApp.m_Axes[theApp.m_Tank.nYRight].nRLdiff=theApp.m_Axes[theApp.m_Tank.nYRight].nRLdiff=theApp.m_Axes[theApp.m_Tank.nYRight].nPos-theApp.m_Axes[theApp.m_Tank.nYRight].nPos;	
	theApp.m_Axes[theApp.m_Tank.nZRight].nRLdiff=theApp.m_Axes[theApp.m_Tank.nZRight].nRLdiff=theApp.m_Axes[theApp.m_Tank.nZRight].nPos-theApp.m_Axes[theApp.m_Tank.nZRight].nPos;	
	theApp.m_Axes[R0AXIS].nRLdiff=theApp.m_Axes[R1AXIS].nRLdiff=theApp.m_Axes[R0AXIS].nPos-theApp.m_Axes[R1AXIS].nPos;
	theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nXRight].nPos;
	theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nXRight].nPos;
	theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nYRight].nPos;
	theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nYRight].nPos;
	theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nZRight].nPos;
	theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nZRight].nPos;
	theApp.m_Axes[R0AXIS].nDesirePos=theApp.m_Axes[R0AXIS].nPos;
	theApp.m_Axes[R1AXIS].nDesirePos=theApp.m_Axes[R1AXIS].nPos;
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nXRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nXRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nYRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nYRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nZRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nZRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
	WriteDpram(theApp.m_Axes[R0AXIS].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[R0AXIS].nDesirePos+theApp.m_Axes[R0AXIS].nOffset);
	WriteDpram(theApp.m_Axes[R1AXIS].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[R1AXIS].nDesirePos+theApp.m_Axes[R1AXIS].nOffset);

	WriteDpram(PMAC_MOVE_FLAG_1,1);
	CommsStr("&1 B7 R");
	m_Pmac.bJoystickEnabled=TRUE;
*/
}

void CServo::MoveToZero()
{
	COORDDATA	Cp;

	SuspendJoystick(0);
	Cp.Side0.fX=0;
	Cp.Side0.fX=0;
	Cp.Side0.fY=0;
	Cp.Side0.fY=0;
	Cp.Side0.fZ=0;
	Cp.Side0.fZ=0;
	Cp.Side0.fR=0;
	Cp.Side0.fR=0;
	DesirePosEqualsCoord(&Cp);
	MoveTo(WAIT);

}

void CServo::StartServoThread()
{
	if(m_bThreadRun==FALSE) {
		m_bThreadRun = TRUE;
		m_pThread = AfxBeginThread(&ServoThread,this,THREAD_PRIORITY_NORMAL,0,NULL);
	};
}
void CServo::StopServoThread()
{
	if(m_bThreadRun==TRUE) {
		m_bThreadRun = FALSE;
		WaitForSingleObject(&m_pThread->m_hThread,INFINITE);
	};
}



/**************************************************************************************/

UINT ServoThread (LPVOID lpvoid)
{

	CServo* pServo = (CServo *) lpvoid;

	while (pServo->m_bThreadRun != FALSE) 
	{

		switch(pServo->m_nThreadAction) {
		case 0:
			break;
		case 1:
//			pServo->CalculateJoystickPositions();
			pServo->GoniometricJoystickServiceRoutine();
			break;
		};
  
	}

	return 0;
}






void CServo::HomeAllAxesWithoutMoving()
{

	SuspendJoystick(0);

	CommsStr("#1HMZ");
	CommsStr("#2HMZ");
	CommsStr("#3HMZ");
	CommsStr("#4HMZ");
	CommsStr("#5HMZ");
	CommsStr("#6HMZ");
	CommsStr("#7HMZ");
	CommsStr("#8HMZ");

}

void CServo::HomeAxisWithoutMoving(int nWhichAxis)
{
	char str[40];

	sprintf_s(str,"#%dHMZ",nWhichAxis);
	CommsStr(str);
}






void CServo::CoordEqualsCurrentPos(COORDDATA *pCp)
{
	

	pCp->Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nPos*theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize;
	pCp->Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nPos*theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize;
	pCp->Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nPos*theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize;
	pCp->Side1.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nPos*theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize;
	pCp->Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nPos*theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize;
	pCp->Side1.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nPos*theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize;
	pCp->Side0.fR=(float)theApp.m_Axes[R0AXIS].nPos*theApp.m_Axes[R0AXIS].fStepSize;
	pCp->Side1.fR=(float)theApp.m_Axes[R1AXIS].nPos*theApp.m_Axes[R1AXIS].fStepSize;

}

void CServo::GetHeadPos(COORDDATA *pCp)
{
	
	pCp->Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nPos*theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize;
	pCp->Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nPos*theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize;
	pCp->Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nPos*theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize;
	pCp->Side0.fR=(float)theApp.m_Axes[R0AXIS].nPos*theApp.m_Axes[R0AXIS].fStepSize;
	pCp->Side0.fXt=(float)theApp.m_Axes[XT0AXIS].nPos*theApp.m_Axes[XT0AXIS].fStepSize;
	pCp->Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos*theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize;
	pCp->Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nPos*theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize;
}


void CServo::StopAllMotorsDead()
{

	CommsStr("#1k#2k#3k#4k#5k#6k#7k#8k");
}

//////////////////////////////////////////////////////////////////////////



void CServo::SetPumpSpeed(int nWhichAxis)
{
	char	Str[40];


	
	sprintf_s(Str,"I%d=%.03f",theApp.m_Axes[nWhichAxis].nPhysicalAxis*100+22,(double)theApp.m_Axes[nWhichAxis].nMaxSpeed/1000.0);
	CommsStr(Str);
	if(ReportPumpStatus(nWhichAxis) == TRUE) {
		sprintf_s(Str,"#%d",theApp.m_Axes[nWhichAxis].nPhysicalAxis);
		CommsStr(Str);
		CommsStr("J+");
	}

}


float CServo::GetPumpSpeed(int nWhich)
{
	char	Str[20];
	float	fSpeed;

	if(nWhich<0) return -1.0;
	if(nWhich>3) return -1.0;

	m_nPmacAddrs=ADDRS1;
	m_nCardNumber=1;

	RequestVariable("I",(nWhich+1)*100+22,Str);
	fSpeed=(float)atof(Str);

	m_nPmacAddrs=ADDRS0;
	m_nCardNumber=0;

	return fSpeed;

}

bool CServo::ReportPumpStatus(int nWhichAxis)
{
	
	char Str[30];
	int	status;


	m_nPmacAddrs=ADDRS1;
	m_nCardNumber=1;

	sprintf_s(Str,"#%d?",theApp.m_Axes[nWhichAxis].nPhysicalAxis);
	CommsStr(Str);
	Sleep(50);

	Str[0]=0;
	if(CommsGetStr(Str)==0) MessageBox(NULL,_T("Failed to Get Pump Status"),_T("Error"),NULL);

	m_nPmacAddrs=ADDRS0;
	m_nCardNumber=0;
	sscanf_s(Str,"%x",&status);
	if(status==0x4000) return TRUE;
	return FALSE;

}

void CServo::PumpStopStart(int nWhich,bool bStopStart)
{

	switch(bStopStart) {
	case FALSE:	PumpStop(nWhich);
		break;
	case TRUE:	PumpStart(nWhich);
		break;
	}
}

void CServo::PumpStart(int nWhich)
{
	char	Str[30];
	

	sprintf_s(Str,"#%dJ+",theApp.m_Axes[nWhich].nPhysicalAxis);
	CommsStr(Str);

}

void CServo::PumpStartAndSpeed(int nWhich, float fSpeed)
{
	char	Str[30];
	

	sprintf_s(Str,"#%d",theApp.m_Axes[nWhich].nPhysicalAxis);
	CommsStr(Str);
	sprintf_s(Str,"I%d=%.02f",theApp.m_Axes[nWhich].nPhysicalAxis*100+22,fSpeed);
	CommsStr(Str);
	CommsStr("J+");

}


void CServo::PumpStop(int nWhich)
{
	char Str[20];
	
	sprintf_s(Str,"#%dk",theApp.m_Axes[nWhich].nPhysicalAxis);
	CommsStr(Str);


}

void CServo::RecoverFromLimitSwitches()
{
	
	COORDDATA	cp;
	int	ii;

	SuspendLimitSwitches();
	CoordEqualsCurrentPos(&cp);

	for(ii=0;ii<8;ii++) {
		if(theApp.m_Axes[ii].bAvailable==TRUE) {
			if(theApp.m_Axes[ii].nStatus&PMAC_MIN_SWITCH) {			//-ve limit hit
				switch(ii) {
				case 0: cp.Side0.fX+=10.0;
					break;
				case 2: cp.Side0.fY+=10.0;
					break;
				case 3: cp.Side1.fY+=10.0;
					break;
				case 4: cp.Side0.fZ+=10.0;
					break;
				case 5: cp.Side1.fZ+=10.0;
					break;
				};
			};
			if(theApp.m_Axes[ii].nStatus&PMAC_MAX_SWITCH) {			//+ve limit hit
				switch(ii) {
				case 0: cp.Side0.fX-=10.0;
					break;
				case 2: cp.Side0.fY-=10.0;
					break;
				case 3: cp.Side1.fY-=10.0;
					break;
				case 4: cp.Side0.fZ-=10.0;
					break;
				case 5: cp.Side1.fZ-=10.0;
					break;
				};
			};
		};
	};


	SuspendJoystick(0);
	DesirePosEqualsCoord(&cp);
	MoveTo(WAIT);
	RestoreLimitSwitches();
	StartJoystick();
}


void CServo::SuspendLimitSwitches()
{
	CommsStr("I124=$120000");
	CommsStr("I224=$120000");
	CommsStr("I324=$120000");
	CommsStr("I424=$120000");
	CommsStr("I524=$120000");
	CommsStr("I624=$120000");
	CommsStr("I724=$120000");
	CommsStr("I824=$120000");

}


void CServo::RestoreLimitSwitches()
{

	CommsStr("I124=$100000");
	CommsStr("I224=$100000");
	CommsStr("I324=$100000");
	CommsStr("I424=$100000");
	CommsStr("I524=$100000");
	CommsStr("I624=$100000");
	CommsStr("I724=$100000");
	CommsStr("I824=$100000");

}




int CServo::GetCurrentProgram()
{
	int	nn=ReadDpram(PMAC_CURRENT_PROGRAM);
	return nn;
}


void CServo::CalculateJoystickPositions()
{
	
	
	int	nXTemp,nYTemp,nZTemp,nXtTemp,nYtTemp,nSTemp,nAction,nTemp,nAxis,nSign,nReqPos;
	static	int	nWhichAxisLoop=0,nOldSide=0;

	nXTemp=m_Pmac.nXJoyValue;
	nYTemp=m_Pmac.nYJoyValue;
	nZTemp=m_Pmac.nZJoyValue;
	nXtTemp=m_Pmac.nXJoyValue;
	nYtTemp=m_Pmac.nYJoyValue;
	nSTemp=m_Pmac.nSJoyValue=0;

	nWhichAxisLoop++;
	nWhichAxisLoop%=4;

	if(nSTemp<30) nAction=0;	//X Y Z
	if(nSTemp>240) nAction=1;	//Manipulator

	switch(nWhichAxisLoop) {
	case 0:	nTemp=nXTemp;
		(nAction==0)?(nAxis=theApp.m_Tank.nXRight):(nAxis=XT0AXIS);
		break;
	case 1:	nTemp=nYTemp;
		(nAction==0)?(nAxis=theApp.m_Tank.nYRight):(nAxis=theApp.m_Tank.nYtLeft);
		break;
	case 2:	nTemp=nZTemp;
		(nAction==0)?(nAxis=theApp.m_Tank.nZRight):(nAxis=R0AXIS);
		break;
	};

	
	nTemp -= theApp.m_Axes[nAxis].nJoystickZero;
	(theApp.m_Axes[nAxis].bJoystickReverse)?(nTemp *= -1):(nTemp);
	if(theApp.m_Axes[nAxis].nJoystickGain==0)	theApp.m_Axes[nAxis].nJoystickGain = 100;
	if((abs(nTemp) > 35) || (abs(nTemp)<=m_nJoystickMargin) || (theApp.m_Axes[nAxis].bAvailable==FALSE)) {
		nTemp = 0;
		nReqPos=0;
	} else {
		nSign = nTemp;
		nReqPos=(abs(nTemp)-m_nJoystickMargin)*theApp.m_Axes[nAxis].nJoystickGain;
		if(nSign<0) nReqPos*= -1;
	};
	theApp.m_Axes[nAxis].nDesirePos=theApp.m_Axes[nAxis].nPos+nReqPos;
	WriteDpram(theApp.m_Axes[nAxis].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[nAxis].nDesirePos+theApp.m_Axes[nAxis].nOffset);

}

void CServo::GoniometricJoystickServiceRoutine()
{
	

	int	nXTemp,nYTemp,nZTemp,nSTemp,nXtTemp,nTemp,nSign;
	int	nReqPos,nJoystickAction=0;
	static	int		nWhichAxis=0;
	AxisData *pGAxis = NULL;
	#define	MARGIN	3

	nXTemp=m_Pmac.nXJoyValue;
	nYTemp=m_Pmac.nYJoyValue;
	nZTemp=m_Pmac.nZJoyValue;
	nXtTemp=m_Pmac.nXJoyValue;
	nSTemp=m_Pmac.nXJoyValue;

//(s_temp<15) ? (joystick_action=1):(joystick_action=0);
//joystick_action^=button_mode;
	nWhichAxis++;
	nWhichAxis%=3;

	switch(nWhichAxis) {
	case 0:	nTemp=nXTemp;
		break;
	case 1:	nTemp=nYTemp;
		break;
	case 2:	nTemp=nZTemp;
		break;
	};

	switch(nJoystickAction) {
	case 0:
		switch(nWhichAxis) {
		case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXRight];
			break;
		case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYRight];
			break;
		case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZRight];
			break;
		};
		break;
	case 1:
		switch(nWhichAxis) {
		case 0:pGAxis=&theApp.m_Axes[XT0AXIS];
			break;
		case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
			break;
		case 2:pGAxis=&theApp.m_Axes[R0AXIS];
			break;
		};
		break;
	};

	if(pGAxis==NULL) return;


	nTemp -= pGAxis->nJoystickZero;
	(pGAxis->bJoystickReverse==TRUE)?(nTemp *= -1):(nTemp);
	if(pGAxis->nJoystickGain==0)	pGAxis->nJoystickGain = 100;
	if((abs(nTemp) > 45) || (abs(nTemp)<=MARGIN)) {
		nTemp = 0;
		if(abs(nXTemp-theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero)<=MARGIN) {
			if(abs(nYTemp-theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero)<=MARGIN) {
				if(abs(nZTemp-theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero)<=MARGIN) {
					GetHeadPos(&CpJoystickHead);
				};
			};
		};
	} else {
		nSign = nTemp;
		nReqPos=((abs(nTemp)-MARGIN))*pGAxis->nJoystickGain;
		if(nSign<0) nReqPos*= -1;

		pGAxis->nDesirePos+=nReqPos;
		if((pGAxis->nDesirePos-pGAxis->nPos)>2000l)	pGAxis->nDesirePos=pGAxis->nPos+2000l;
		if((pGAxis->nDesirePos-pGAxis->nPos)<-2000l)	pGAxis->nDesirePos=pGAxis->nPos-2000l;
	};
//	if(tank.manipulator_mode) check_goniometer(joystick_action,which_axis);
//	*(pmac.mem_addrs + PMAC_MANIPULATOR_MODE) = (long)joystick_action;

	WriteDpram(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
}



void CServo::PLCJoystickServiceRoutine()
{
	


	switch(theApp.m_nJobNumber){
	default:
		break;
	case TWI:
		TWIJoystickServiceRoutine();
		return;
		break;
	}


	int	nXTemp,nYTemp,nZTemp,nSTemp,nXtTemp,nYtTemp,nTemp,nSign;
	int	nReqPos,nJoystickAction=0;
	static	int		nWhichAxis=0;
	static	int	nDeadMan=10;
	static bool bDeadMan=TRUE;
	AxisData *pGAxis = NULL;

	#define	MARGIN	3

	nXTemp=m_Pmac.nXJoyValue;
	nYTemp=m_Pmac.nYJoyValue;
	nZTemp=m_Pmac.nZJoyValue;
	nXtTemp=m_Pmac.nXJoyValue;
	nYtTemp=m_Pmac.nYJoyValue;
	nSTemp=m_Pmac.nSJoyValue;

	if(nXTemp<10) {
		nDeadMan=10;
		WriteDpram(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1+PMAC_JOY_DEV,0);
		WriteDpram(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1+PMAC_JOY_DEV,0);
		WriteDpram(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1+PMAC_JOY_DEV,0);
		WriteDpram(theApp.m_Axes[XT0AXIS].nPhysicalAxis-1+PMAC_JOY_DEV,0);
		WriteDpram(theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis-1+PMAC_JOY_DEV,0);
		bDeadMan=TRUE;
		return;
	};
	if(nDeadMan>0) {
		nDeadMan--;
		if(m_Pmac.nXJoyValue>10) theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=m_Pmac.nXJoyValue;
		if(m_Pmac.nXJoyValue>10) theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=m_Pmac.nYJoyValue;
		if(m_Pmac.nZJoyValue>10) theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=m_Pmac.nZJoyValue;
		if(m_Pmac.nXJoyValue>10) theApp.m_Axes[XT0AXIS].nJoystickZero=m_Pmac.nXJoyValue;
		if(m_Pmac.nYJoyValue>10) theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=m_Pmac.nYJoyValue;
		bDeadMan=FALSE;
	};

//(s_temp<15) ? (joystick_action=1):(joystick_action=0);
//joystick_action^=button_mode;
	nWhichAxis++;
	nWhichAxis%=3;

	switch(nWhichAxis) {
	case 0:	nTemp=nXTemp;
		break;
	case 1:	nTemp=nYTemp;
		break;
	case 2:	nTemp=nZTemp;
		break;
	};

	switch(theApp.m_nJobNumber){
	default:
		break;
	case AIM_COMPOSITES:
		nSTemp=128;
		break;
	}



	if(nSTemp>10) {
		switch(nWhichAxis) {
		case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXRight];
			break;
		case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYRight];
			break;
		case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZRight];
			break;
		};
	} else {
		switch(nWhichAxis) {
		case 0:pGAxis=&theApp.m_Axes[XT0AXIS];
			break;
		case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
			break;
		case 2:pGAxis=&theApp.m_Axes[R0AXIS];
			break;
		};
	};

	if(pGAxis==NULL) return;


	nReqPos=0;
	nTemp -= pGAxis->nJoystickZero;
	(pGAxis->bJoystickReverse==TRUE)?(nTemp *= -1):(nTemp);
	if(pGAxis->nJoystickGain==0)	pGAxis->nJoystickGain = 100;
	if((abs(nTemp) > 45) || (abs(nTemp)<=MARGIN)) {
/*
		nTemp = 0;
		if(abs(nXTemp-theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero)<=MARGIN) {
			if(abs(nYTemp-theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero)<=MARGIN) {
				if(abs(nZTemp-theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero)<=MARGIN) {
					GetHeadPos(&CpJoystickHead);
				};
			};
		};
		*/
	} else {
		nSign = nTemp;
		nReqPos=((abs(nTemp)-MARGIN))*pGAxis->nJoystickGain;
		if(nSign<0) nReqPos*= -1;

	};
	WriteDpram(pGAxis->nPhysicalAxis-1+PMAC_JOY_DEV,nReqPos);
	if(nSTemp>10) {
		WriteDpram(PMAC_JOYSTICK_TIP_MODE, 0);
		GetHeadPos(&m_HeadCp);
//???		theApp.m_Manip.SurfaceFromHead(m_HeadCp, &m_SurfaceCp,TRUE);
	} else {
		WriteDpram(PMAC_JOYSTICK_TIP_MODE, 1);
		GetHeadPos(&m_HeadCp);
		m_SurfaceCp.Side0.fXt=m_HeadCp.Side0.fXt;
		m_SurfaceCp.Side0.fYt=m_HeadCp.Side0.fYt;
//???		theApp.m_Manip.HeadFromSurface(m_SurfaceCp, &m_HeadCp, TRUE);
	};
	theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos = (int)(m_HeadCp.Side0.fX / theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos = (int)(m_HeadCp.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos = (int)(m_HeadCp.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
	WriteDpram(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);

}


/*

extern	float xt0_angle,yt0_angle,angle;
extern	float cosA0,sinA0,cosB0,sinB0;


check_goniometer(int joystick_action,int which_axis)
{
float	angle,pos;

if(joystick_action==0) {							// moving x,y,z
	coord_equals_desire_pos(&cph);
	switch(which_axis) {
	case 0:
		break;
	case 1:
		if(yt0_angle<=45.0) {
			pos=point.zp-((y0_axis.desire_pos*y0_axis.stepsize)-point.yp)*sinB0;
			z0_axis.desire_pos=(long)(pos/z0_axis.stepsize);
		};
		break;
	case 2:
		if(yt0_angle>=45.0) {
			pos=point.yp+((z0_axis.desire_pos*z0_axis.stepsize)-point.zp)*cosB0;
			y0_axis.desire_pos=(long)(pos/y0_axis.stepsize);
		};
		break;
	};
	surface_from_head(&cph,&cps,0);
} else {													// moving xt,yt
	cps.xtp=(float)xt0_axis.desire_pos*xt0_axis.stepsize;
	cps.ytp=(float)yt0_axis.desire_pos*yt0_axis.stepsize;
	head_from_surface(&cps,&cph);
	x0_axis.desire_pos=cph.xp/x0_axis.stepsize;
	y0_axis.desire_pos=cph.yp/y0_axis.stepsize;
	z0_axis.desire_pos=cph.zp/z0_axis.stepsize;
	point=cph;
};
return 0;
}


  */

void CServo::FindPeakAmplitude(int nSamples, int *nPos)
{
	int	ii;
	unsigned char cMax=0;

	for(ii=0;ii<nSamples;ii++) {
		if((Array[ii]-128)>cMax) {
			cMax=Array[ii]-128;
			*nPos=ii;
		};
	};

}

void CServo::FindMeanAmplitude(int nSamples, int *nPos)
{
	int	ii;
	unsigned char cMax=0;
	int	nSum=0,nTotal=0;

	for(ii=0;ii<nSamples;ii++) {
		nSum+=((int)(Array[ii]-128) * ii);
		nTotal+=(int)(Array[ii]-128);
	};
	if(nTotal>0) {
		*nPos=nSum/nTotal;
	};

}


void CServo::SetupLookAhead()
{
float	velocity,acceleration,stop_time,max_stop_time;
int	ii,segments_needed,temp;
char	str[200];

	if(m_bLookAheadFlag==FALSE) {
		max_stop_time=0.01f;
		for(ii=100;ii<600;ii+=100) {
			sprintf_s(str,"I%d=0 I%d=0",ii+13,ii+14);
			CommsStr(str);
	
			RequestVariable("I",ii+16,str);
			velocity=(float)atof(str);
			RequestVariable("I",ii+17,str);
			acceleration=(float)atof(str);
			stop_time=velocity/acceleration;
			if(stop_time>max_stop_time) max_stop_time=stop_time;
		};
	
		CommsStr("I5113=2");
		segments_needed=(int)(max_stop_time / 4.0);
		temp=(segments_needed*4)/3 + 10;
		sprintf_s(str,"I5120=%d",temp);
		CommsStr(str);

		sprintf_s(str,"&1 DEF LOOK %d",temp);
		CommsStr(str);

		m_bLookAheadFlag=TRUE;
	}
}




void CServo::DisableLookAhead()
{
	if(m_bLookAheadFlag==TRUE) {
		CommsStr("I5213=0");
		//CommsStr("I5223=0");
		CommsStr("&1 A");
		CommsStr("DEL LOOK");
		m_bLookAheadFlag=FALSE;
	}

}



bool CServo::LiftCommandUp()
{

	Outp(m_nPlcAddrs+2,0xfe);	//enable output
	Outp(m_nPlcAddrs+0,0xfe);
	if(Inp(m_nPlcAddrs+1) & 1) {
		return TRUE;
	};
	return FALSE;
}

bool CServo::LiftCommandDown()
{
	Outp(m_nPlcAddrs+2,0xfd);	//enable output
	Outp(m_nPlcAddrs+0,0x02);
	if(Inp(m_nPlcAddrs+0) & 2) {
		return TRUE;
	};
	return FALSE;

}

bool CServo::IsSystemParked()
{
	

	if(theApp.m_Axes[theApp.m_Tank.nXRight].nPos > (int)(3.0 / theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize)) return FALSE;
	if(theApp.m_Axes[theApp.m_Tank.nYRight].nPos > (int)(3.0 / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize)) return FALSE;
	if(theApp.m_Axes[theApp.m_Tank.nZRight].nPos > (int)(3.0 / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize)) return FALSE;

	return TRUE;
}

void CServo::TurntableTo(int nSide)
{
	
	int	nMax,nTemp;

	if(nSide==PORTSIDE) {
		theApp.m_Axes[R0AXIS].nDesirePos = 0;
	} else {
		theApp.m_Axes[R0AXIS].nDesirePos = (int)(180.0 / theApp.m_Axes[R0AXIS].fStepSize);
	};
	nMax=0;
	if(theApp.m_Axes[R0AXIS].bAvailable) {
		WriteDpram(theApp.m_Axes[R0AXIS].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[R0AXIS].nDesirePos+theApp.m_Axes[R0AXIS].nOffset);
		nTemp=CalcMoveTime(&theApp.m_Axes[R0AXIS],labs(theApp.m_Axes[R0AXIS].nDesirePos-theApp.m_Axes[R0AXIS].nPos));
		if(nTemp>nMax) nMax=nTemp;
	};

	WriteDpram(PMAC_MOVE_FLAG_1,1);
	WriteDpram(PMAC_TM,nMax);
	WriteDpram(PMAC_TA,0);
	WriteDpram(PMAC_TS,300);
	CommsStr("&1 B6 R");
	Sleep(200);
}


bool CServo::IsHomeRequired()
{
	
	int	ii,flag,mask,nNumberAxes=0;

	m_bAllMotorsHomed=FALSE;

	flag=0;
	mask=1;
	for(ii=0;ii<8;ii++) {
		if(theApp.m_Axes[ii].bAvailable==TRUE) {
			if(DoesAxisNeedHoming(ii)==TRUE) {
				flag |= mask;
				nNumberAxes++;
			};
			mask<<=1;
		};
	};
	if(flag==0) {
		m_bAllMotorsHomed=TRUE;
		return TRUE;
	};
	CString Buff;
	Buff.Format(_T("There are %d Axes that need to be homed"),nNumberAxes);
	MessageBox(NULL,Buff,_T("Error"),NULL);

	return FALSE;
}

bool CServo::DoesAxisNeedHoming(int nWhichAxis)
{
	

	if(theApp.m_Axes[nWhichAxis].bMustHome==TRUE) {
		if(!(theApp.m_Axes[nWhichAxis].nStatus&0x400l)) return TRUE;
	};
	return FALSE;
}


bool CServo::IsMovementAllowed()
{
	

	return TRUE;
	if(theApp.m_nJobNumber==455) {
		if(theApp.m_UtUser.IsLiftDown()==FALSE) {
			MessageBox(NULL,_T("Lift Is Up!! No Movement is allowed"),_T("Error"),NULL);
			return FALSE;
		};
	};
	return TRUE;

}

bool CServo::IsThisSide(int nSide)
{
	
	int	nPos,nDeltaPos;

	nDeltaPos=(int)(10.0/theApp.m_Axes[R0AXIS].fStepSize);
	if(nSide==0) {
		nPos = 0;
	} else {
		nPos = (int)(180.0/theApp.m_Axes[R0AXIS].fStepSize);
	};
	if(abs(nPos-theApp.m_Axes[R0AXIS].nPos) < nDeltaPos) {
		if(theApp.m_UtUser.IsLiftDown()==TRUE) {
			return TRUE;
		};
	}
	return FALSE;
}

/*
This is checked on Min switch channel 7
0 == Not Powered
1 == Powered
*/


bool CServo::AreMotorsPowerd()
{
	


	switch(theApp.m_nJobNumber) {
	case 455:
		int	nn=ReadDpram(PMAC_MOTOR_STATUS + 6 * PMAC_AXIS_OFFSET);
		theApp.m_Axes[6].nPhysicalAxis=7;
		nn=theApp.m_Axes[6].nStatus;
		if(nn & PMAC_MIN_SWITCH) return TRUE;
		MessageBox(NULL,_T("Motors are not powerd up"),_T("Error"),NULL);
		return FALSE;
		break;
	}

	return TRUE;

}

bool CServo::HomeFoundOnXYZXtYt()
{
	
	int	nA[5]={theApp.m_Tank.nXRight, theApp.m_Tank.nYRight, theApp.m_Tank.nZRight, XT0AXIS, theApp.m_Tank.nYtLeft};
	int	ii;

	for(ii=0;ii<5;ii++) {
		if(theApp.m_Axes[nA[ii]].bAvailable) {
			if(!(theApp.m_Axes[nA[ii]].nStatus&PMAC_HOMED)) return FALSE; //something has not finished
		}
	}
	return TRUE;

}

bool CServo::HomeFoundOnR()
{
	

	if(theApp.m_Axes[R0AXIS].bAvailable) {
		if(!(theApp.m_Axes[R0AXIS].nStatus&PMAC_HOMED)) return FALSE; //something has not finished
	}
	return TRUE;

}


void CServo::SimpleBackwardForwardProgram()
{
	char String[][50]={
	{"OPEN PROG 30 CLEAR"},
	{"M6=7"},
	{"Linear"},
	{"FRAX(X,Y,Z,A,B,C,U,V)"},
	{"ABS"},
	{"TM7000"},
	{"TA1000"},
	{"TS1"},
	{"WHILE(M1=30)"},
		{"Y20000"},
		{"Y100000"},
	{"ENDWHILE"},
	{"M6=0"},
	{"CLOSE"}};

	DownLoadProgram((char *)String,(sizeof String)/(sizeof String[0]),sizeof String[0]);
}

bool CServo::IsHomeFindFinishedOnAxis(int nAxis)
{
	

	if(theApp.m_Axes[nAxis].bAvailable) {
		if(!(theApp.m_Axes[nAxis].nStatus&PMAC_HOMED)) return FALSE; //something has not finished
		if(!(theApp.m_Axes[nAxis].nStatus&PMAC_IN_POS)) return FALSE; //something has not finished
	}
	return TRUE;

}

int CServo::GetESwitchStatus()
{
	int nFlag;
	int	nn;


	nn=ReadDpram(PMAC_GENERAL_INPUTS);
	nn&1 ? nFlag = 0 : nFlag =1;

	return nFlag;
}

void CServo::ZBrakesEnable(int bOffOn)
{
	int	nn,nTemp;

	nTemp=nn=ReadDpram(PMAC_GENERAL_OUTPUTS);

	switch(bOffOn) {
	case FALSE: nn |= 0x01;
		break;
	case TRUE:
		if(GetESwitchStatus()==1) {
			nn &= 0xfe;
		}
		break;
	}

	WriteDpram(PMAC_GENERAL_OUTPUTS,nn);

}

void CServo::ZBrakesOn()
{
	int	nn;

	nn=ReadDpram(PMAC_GENERAL_OUTPUTS);
	nn &= 0xfe;

	WriteDpram(PMAC_GENERAL_OUTPUTS,nn);
}

int CServo::GetBrakeStatus()
{
	int nFlag;
	int	nn;


	nn=ReadDpram(PMAC_GENERAL_OUTPUTS);
	nn&1 ? nFlag = 0 : nFlag =1;

	return nFlag;
}

void CServo::TWIJoystickServiceRoutine()
{
	

	int	nTemp,nToggleTemp,nSign,nJoyValue[4],nToggleTempPos;
	int	nReqPos,nJoystickAction=0;
	int	nJoystickGain;
	static	int		nWhichAxis=0;
	static	bool	bDeadMan=TRUE;
	AxisData *pGAxis=NULL;
	AxisData *pHAxis=NULL;

	#define	MARGIN	3
	#define	LEFT_JOYSTICK	1
	#define	RIGHT_JOYSTICK	2
	#define	BOTH_JOYSTICK	3
	#define	MAIN_AXES	0
	#define	TIP_AXES	1
	#define ROTATE_AXES	2
	
	nJoyValue[0]=m_Pmac.nXJoyValue;
	nJoyValue[1]=m_Pmac.nYJoyValue;
	nJoyValue[2]=m_Pmac.nZJoyValue;
	nJoyValue[3]=m_Pmac.nSJoyValue;
	nToggleTemp=m_Pmac.nTJoyValue;
	nToggleTempPos=m_Pmac.nTogglePos;

	if(m_Pmac.nSwitchJoyValue & DEADMAN_SWITCH) {
		for(int nAxis=0;nAxis<8;nAxis++) {
			theApp.m_Axes[nAxis].nJoystickZero = nJoyValue[theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickNumber % 4];
		}
	};


	nWhichAxis++;
	nWhichAxis%=8;

	nTemp = nJoyValue[theApp.m_Axes[nWhichAxis].nJoystickNumber % 4];

	pGAxis=&theApp.m_Axes[nWhichAxis];

	if(pGAxis==NULL) return;
	if(pGAxis->bAvailable==FALSE) return;

	nReqPos=0;
	nTemp -= pGAxis->nJoystickZero;
	(pGAxis->bJoystickReverse==TRUE)?(nTemp *= -1):(nTemp);
	if(pGAxis->nJoystickSwitchPos != m_Pmac.nTogglePos) nTemp=0;
	if(pGAxis->nJoystickGain==0)	pGAxis->nJoystickGain = 100;
	if(m_Pmac.nSwitchJoyValue & FAST_JOYSTICK) {
		nJoystickGain = pGAxis->nJoystickGain;
	} else {
		nJoystickGain = (pGAxis->nJoystickGain / 10);
	}

	if((abs(nTemp) > 50) || (abs(nTemp)<=MARGIN)) {
		nTemp = 0;
		GetHeadPos(&CpJoystickHead);
	} else {
		nSign = nTemp;
		nReqPos=((abs(nTemp)-MARGIN))*nJoystickGain;
		if(nSign<0) nReqPos*= -1;

	};


	WriteDpram(pGAxis->nPhysicalAxis-1+PMAC_JOY_DEV,nReqPos);
/*

  	switch(m_Pmac.nTogglePos) {
	case 0: nJoystickAction=MAIN_AXES;
		break;
	case 1: nJoystickAction=TIP_AXES;
		break;
	case 2: nJoystickAction=ROTATE_AXES;
		break;
	}

	
	if(nJoystickAction==MAIN_AXES) {
		WriteDpram(PMAC_JOYSTICK_TRACKING_MODE, 0);
		GetHeadPos(&m_HeadCp);
		theApp.m_Kinematics.SurfaceFromHead(&m_HeadCp, &m_SurfaceCp,PORTSIDE);
		theApp.m_Kinematics.SurfaceFromHead(&m_HeadCp, &m_SurfaceCp,STARBOARD);
	} else {
		if(theApp.m_LastSettings.nJoystickTipMode==1) {
			GetHeadPos(&m_HeadCp);
			m_SurfaceCp.Side0.fXt=m_HeadCp.Side0.fXt;
			m_SurfaceCp.Side0.fYt=m_HeadCp.Side0.fYt;
			m_SurfaceCp.Side1.fXt=m_HeadCp.Side1.fXt;
			m_SurfaceCp.Side1.fYt=m_HeadCp.Side1.fYt;
			theApp.m_Kinematics.HeadFromSurface(&m_SurfaceCp, &m_HeadCp, PORTSIDE);
			theApp.m_Kinematics.HeadFromSurface(&m_SurfaceCp, &m_HeadCp, STARBOARD);
			theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos = (int)(m_HeadCp.Side0.fX / theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos = (int)(m_HeadCp.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos = (int)(m_HeadCp.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos = (int)(m_HeadCp.Side1.fX / theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos = (int)(m_HeadCp.Side1.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos = (int)(m_HeadCp.Side1.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nXRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nYRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nZRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nXRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nYRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nZRight].nMemOffset+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
			WriteDpram(PMAC_JOYSTICK_TRACKING_MODE, 1);
		}
	}
*/



}

void CServo::GetTurntablePos(COORDDATA *pCp)
{
	pCp->Side0.fR = (float)theApp.m_Axes[R0AXIS].nPos * theApp.m_Axes[R0AXIS].fStepSize;
}


void CServo::StartContinuousRotate(int nAxis,float fRotateSpeed)
{
	
	char	str[30];
	float fSpeed;

	fSpeed = fRotateSpeed / 1000.0f / theApp.m_Axes[nAxis].fStepSize;
	if(fSpeed>theApp.m_Axes[nAxis].fMaxJogSpeed) fSpeed=theApp.m_Axes[nAxis].fMaxJogSpeed;

	sprintf_s(str,"I%d=%.02f",theApp.m_Axes[nAxis].nPhysicalAxis*100+22,fSpeed);
	CommsStr(str);
	
	sprintf_s(str,"#%dj+",theApp.m_Axes[nAxis].nPhysicalAxis);
	CommsStr(str);
}

void CServo::ZeroAxis(int nAxis)
{

}

void CServo::GetRollerPos(COORDDATA *pCp)
{
	pCp->Side1.fX = (float)theApp.m_Axes[theApp.m_Tank.nXRight].nPos * theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize;
}


void CServo::ResetAllMaxSpeeds()
{
	for(int nAxis=0;nAxis<8;nAxis++) {
		if(theApp.m_Axes[nAxis].bAvailable==TRUE) {
			SetVariable("I",theApp.m_Axes[nAxis].nPhysicalAxis*100+22,theApp.m_Axes[nAxis].fMaxJogSpeed);
			SetVariable("I",theApp.m_Axes[nAxis].nPhysicalAxis*100+16,theApp.m_Axes[nAxis].fMaxMoveSpeed);
		}
	}
}


void CServo::SetRevCounter(int nCount)
{
	m_Pmac.nRevCounter = nCount;

}

int	CServo::GetRevCounter()
{
	return m_Pmac.nRevCounter;
}

bool CServo::GetStallStatus()
{
	
	int	ii;

	for(ii=0;ii<8;ii++) {
		if(theApp.m_Axes[ii].nStatus & PMAC_HARD_LIMIT) return TRUE;
	}

	return FALSE;
}


void CServo::CStringToChar(CString str, char* chr)
{
	int i=0;

	for(i=0;i<str.GetLength();i++)
	{
		chr[i]= (char)str.GetAt(i);
	}

	chr[i] = '\0';
}

int CServo::DpramSetMem(int nOffset,int nLength,PVOID pMem)
{
	int i=0;
	int *pInt;
	int val;

	pInt = (int*)pMem;

	for(i=0;i<nLength;i++){
		val = *pInt ;
		WriteDpram(nOffset+i,val);
		pInt++;
	}

	return 1;
}

int CServo::DpramGetMem(int nOffset,int nLength,PVOID pMem)
{
	int i=0;
	int *pInt;
	
	pInt = (int*)pMem;

	for(i=0;i<nLength;i++){
		*pInt = ReadDpram(nOffset+i);
		pInt++;
	}

	return 1;
}

