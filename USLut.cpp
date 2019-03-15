// USLut.cpp: implementation of the CUSLut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "USLut.h"
#include "winioctl.h"
#include "math.h"
#include "MainFrm.h"
//#include "Version1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	NON_MUX			0
#define	MUX				1



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUSLut::CUSLut()
{
	m_hUSL=NULL;
//	m_file.Open("C:\\mailbox.txt", CFile::modeCreate|CFile::typeBinary|CFile::modeWrite);
	m_bAdcFifoWait=FALSE;
	m_bDspFifoWait=FALSE;
	m_nLastAdcTimeSlot=-1;
	m_nLastDspTimeSlot=-1;
	m_Pm30CommDevice=NOTHING_ATTACHED;
	m_nAveragePtr=0;
	m_pCollect[0] = NULL;
	m_pCollect[1] = NULL;
	m_nCollectPtr = 0;
	m_nCollectL[0] = 0;
	m_nCollectL[1] = 0;
	m_nCollectMode = 0;


	ZeroMemory(m_Cal, sizeof m_Cal);
	ZeroMemory(m_pnCalTrace, sizeof m_pnCalTrace);
	ZeroMemory(m_pnDefaultCalTrace, sizeof m_pnDefaultCalTrace);

	m_Units[0]="\x0b5s";
	m_Units[1]="mm";
	m_Units[2]="\x022";

//	m_pActiveTimeSlots = new CActiveTimeSlots();
}

CUSLut::~CUSLut()
{
//	m_file.Close();
	for(int nTS=0;nTS<256;nTS++) {
		SAFE_DELETE(m_pnCalTrace[nTS]);
		SAFE_DELETE(m_pnDefaultCalTrace[nTS]);
	}

	SAFE_DELETE( m_pCollect[0] );
	SAFE_DELETE( m_pCollect[1] );

	if((m_hUSL!=INVALID_HANDLE_VALUE) && (m_hUSL!=NULL)) {
		Dsp200StopProcessing(&m_TS[0].Dsp);
		DisableInterrupts();
		Sleep(200);
		FreeAndDeallocateBscanMemory();
		FreeTimeSlotStruct();
		if( !CloseHandle(m_hUSL))	{
			MessageBox(NULL,_T("Failed to Deallocate USL Device Driver"),_T("Error"),NULL);
		};
	}
//	SAFE_DELETE(m_pActiveTimeSlots);
}


bool CUSLut::StartDriver(bool bEnableUSLDriver,bool bEnableUSlDriverInterrupts)
{
	CString Buff;

	m_nPlcAddrs=0x58;
	MailboxAddrs=theApp.m_LastSettings.nMailBoxAddrs=0x00;
	MaxMailboxWait=0;
	m_nMailboxFailed=0;
	m_nMailboxRxFailed=0;
	m_bThreadEnabled = FALSE;
	m_bThreadFinished = FALSE;

	if(bEnableUSLDriver == true) {
		GetHandleToDevice(2000);
		if( m_hUSL==INVALID_HANDLE_VALUE) {
//			MessageBox(NULL,_T("Failed to Allocate Hardware Resource\r\n USL Device Driver"),_T("Error"),NULL);
			return FALSE;
		} else {
			char	Version[40];
			GetVersion(Version,sizeof Version);
			if(strcmp(Version,USL_VERSION)) {
				Buff="Incorrect Version of USL driver\r\n";
				Buff+=Version;
				MessageBox(NULL,Buff,_T("Error"),NULL);
				if( !CloseHandle(m_hUSL))	{
					MessageBox(NULL,_T("Failed to Deallocate USL Device Driver"),_T("Error"),NULL);
				};
				m_hUSL=NULL;
				return FALSE;
			}
		}
	}

	if(bEnableUSlDriverInterrupts == true) {
//		ShareTimeSlotStruct();
//		EnableInterrupts();
	}

	if (theApp.m_PM40User.IsConnected()) {
		m_Pm30CommDevice = PM40_I2C;
	}
	else {
		if (theApp.m_Si10User.IsConnected()) {
			m_Pm30CommDevice = SI10_I2C;
		}
		else {
			if (theApp.m_USBtoI2C.m_bDevicePresent) {
				m_Pm30CommDevice = USB_I2C;
			}
			else {
				if (m_hUSL != NULL) {
					m_Pm30CommDevice = USLDRIVER_I2C;
				}
			}
		}
	}

	
//	if(FindPM30s()){
//		AutoDetectPM30OutputDevice();
//	} else if(theApp.m_LastSettings.bPr30Present) { 		//User entered data not consistant with auto discover
		//User entered data overides
		m_nPM30Count=1;
//		m_nPM30Addresses[0]=theApp.m_LastSettings.nPr30DestAddrs;
//	}


	if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]==LSA) {
		//If using LSA we may have a m_nPM30Count>1
		m_nPM30Count=0;
		for(int ii=0;ii<4;ii++){
			if((theApp.m_LastSettings.nAttachedToPr30[ii]==LSA)&&(theApp.m_LastSettings.bPr30Present[ii])){
				m_nPM30Count++;
			}
		}
	}

	//if(m_nPM30Count==0)
		//ERROR


	Initialize(theApp.m_nJobNumber);
	//LiftInitialize();
	SetupOutputDeviceSettings();

	return TRUE;

}

int CUSLut::FindPM30s()
{
	PM30_RETURN_MSG Pm30Msg;

	return 0;

	m_nPM30Count=0;


	for(int ii=0;ii<4;ii++){//4

		//Turn on reply messages
		MailboxTx(0x8C + ii);
		MailboxTx(0x7D);
		MailboxTx(0x42);
		MailboxTx(0x20);
		MailboxTxCheckSum();
		MailboxTx(0xff);

		MailboxTx(0x8C + ii); //Ping
		MailboxTx(0x7D);
		MailboxTx(0x00);
		MailboxTx(0x01);
		MailboxTxCheckSum();
		MailboxTx(0xff);

		MailboxRx(&Pm30Msg);
		if(Pm30Msg.nBytes>1){
			m_nPM30Addresses[m_nPM30Count] = 0x8C + ii;
			m_nPM30Count++;
		}


		//Turn off reply messages
		MailboxTx(0x8C + ii);
		MailboxTx(0x7D);
		MailboxTx(0x42);
		MailboxTx(0x10);
		MailboxTxCheckSum();
		MailboxTx(0xff);

		Sleep(4000); //Allow for messages to missing PM30s to dissipate


	}

	return m_nPM30Count;
}

void CUSLut::AutoDetectPM30OutputDevice()
{
	PM30_RETURN_MSG Pm30Msg;
	PM30AttachedDevices nOutputDevice[4];

	for(int ii=0;ii<m_nPM30Count;ii++){

		//Check each PM30 for what is connected to it
		
		//Turn on reply messages
		MailboxTx(m_nPM30Addresses[ii]);
		MailboxTx(0x7D);
		MailboxTx(0x42);
		MailboxTx(0x20);
		MailboxTxCheckSum();
		MailboxTx(0xff);

		//Get Hardware Configuration
		MailboxTx(m_nPM30Addresses[ii]);
		MailboxTx(0x7D);
		MailboxTx(0x09);
		MailboxTx(0x0C | (m_nPM30Addresses[ii] & 0x03) );
		MailboxTx(0x10); //Harware Configuration
		MailboxTxCheckSum();
		MailboxTx(0xff);

		MailboxRx(&Pm30Msg);
		if(Pm30Msg.nBytes>1){
			
			if(Pm30Msg.data[7]>0){
				nOutputDevice[ii] = PPA15;
			}else if(Pm30Msg.data[8]>0){
				nOutputDevice[ii] = MUX8;
			}else {
				nOutputDevice[ii] = LSA;
			}

		}

		//Turn off reply messages
		MailboxTx(m_nPM30Addresses[ii]);
		MailboxTx(0x7D);
		MailboxTx(0x42);
		MailboxTx(0x10);
		MailboxTxCheckSum();
		MailboxTx(0xff);			
	}


	switch(m_nPM30Count){
	default:
	case 0: //You shouldn't have got here!!!!
		break;
	case 1:
		theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30] = nOutputDevice[0];
		break;
	case 2:
		if(nOutputDevice[0]!=nOutputDevice[1]) {
			//Multiple devices attached 
			//Provide Option
		}
		break;
	case 3:
		if((nOutputDevice[0]!=nOutputDevice[1]) | (nOutputDevice[0]!=nOutputDevice[2])){
			//Multiple devices attached 
			//Provide Option
		}
		break;
	case 4:
		if((nOutputDevice[0]!=nOutputDevice[1]) | (nOutputDevice[0]!=nOutputDevice[2]) | (nOutputDevice[0]!=nOutputDevice[3]) ){
			//Multiple devices attached 
			//Provide Option
		}
		break;
	}


	//if multiple devices issue prompt for which to use

}

void CUSLut::Initialize(int nJobNumber)
{
	if(m_Pm30CommDevice==NONE) return;

	MailboxPrfLowPeriod(); 
	m_TS[0].Dsp.bPlayBack=FALSE;
//	MscInitialize(); 
//	Adc100Initialize(&m_TS[0].Adc);
//	Dsp200Reset();
	Sleep(20);
//	Dsp200Initialize();
	for(int nTS=0;nTS<8;nTS++) { 
//		Pr30Defaults(&m_TS[nTS].Pr30,nTS);
	}
	RefreshAllVariables(nJobNumber);
//	Dsp200StartProcessing(&m_TS[0].Dsp);
//	MailboxHtReset();

}

void CUSLut::TimeSlotDefault()
{

	int	ii;

	for(ii=0;ii<8;ii++) {
		m_TS[ii].nStructSize=sizeof (struct TimeSlotData);
		m_TS[ii].nAmplifier=USL_PL15;
		Pl15Defaults(&m_TS[ii].Pl15);
		Pr15Defaults(&m_TS[ii].Pr15);
		Adc200Default(&m_TS[ii].Adc,ii);
		Dsp200Defaults(&m_TS[ii].Dsp,ii);

		m_TS[ii].Pr15.nMuxTimeslot=ii;
		m_TS[ii].Pr15.nPrfSrc=0;				//External
		m_TS[ii].Pr15.ts0_reset_src=1;		//External
		m_TS[ii].Pr15.mux_state=1;
		m_TS[ii].Pr15.nMuxMode=1;
		m_TS[ii].Pr15.nHtOffOn=1;
		m_TS[ii].Pr15.nPulserOffOn=1;

		m_TS[ii].Pl15.nMuxTimeslot=ii;
		m_TS[ii].Pl15.nPrfSrc=0;				//External
		m_TS[ii].Pl15.ts0_reset_src=0;		//Internal
		m_TS[ii].Pl15.mux_state=1;
		m_TS[ii].Pl15.nMuxMode=1;
		m_TS[ii].Pl15.nHtOffOn=1;
		m_TS[ii].Pl15.nPulserOffOn=1;

		m_TS[ii].Adc.mux_run_mode=1;				//run
		m_TS[ii].Dsp.nMuxMode=1;					//run
		m_TS[ii].Adc.nTimeslot=ii;

	};
	if(m_Global.Mux.nMaxTimeslots<=0) MuxDefaults();


}

BOOL CUSLut::GetHandleToDevice(int OS)
{
	WCHAR str[256];

	if(theApp.m_UtRes.nIOAddrs<0x100) {
		theApp.m_UtRes.nIOAddrs=0x110;
		theApp.m_UtRes.nIORange=0x40;
		theApp.m_UtRes.nMemAddrs=0xd4000;
		theApp.m_UtRes.nMemRange=0x1000;
		theApp.m_UtRes.nIrq=5;

	}

	swprintf_s(str,_T("\\\\.\\USL\\isa\\io%x,%x\\mem%x,%x\\irq%d\\override"),
		theApp.m_UtRes.nIOAddrs,theApp.m_UtRes.nIORange,
		theApp.m_UtRes.nMemAddrs,theApp.m_UtRes.nMemRange,
		theApp.m_UtRes.nIrq);

	m_hUSL=NULL;
	switch(OS) {
	case 95:
	case 98:
//		m_hUSL = GetDeviceViaInterface((LPGUID)&SI6_GUID,0);
		break;
	case 2000:
		m_hUSL = CreateFile(str, GENERIC_READ|GENERIC_WRITE, 0,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//		m_hUSL = CreateFile("\\\\.\\USL\\isa\\io110,60\\memd8000,1000\\irq5\\override", GENERIC_READ|GENERIC_WRITE, 0,
//			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	};
	if( m_hUSL==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	return TRUE;
}

WORD CUSLut::Inp(int nOffset,int nWhichInterface)
{

	if(m_hUSL!=NULL)
		nWhichInterface=USLDRIVER;

	if(nWhichInterface==USLDRIVER) {
		BYTE	ReadInput[3]={USL_READ, (BYTE)nOffset};
		DWORD BytesReturned;
		WORD rv[3]={0xffff,0xffff,0xffff};

		BOOL result=DeviceIoControl(m_hUSL, IOCTL_USL_RUN_CMDS,
									ReadInput, 2,			// Input
									rv, sizeof(rv),					// Output
									&BytesReturned, NULL);
		return rv[2] & 0x00ff;
	} else {
		return theApp.m_Si6User.Inp(nOffset);
	};
}

WORD CUSLut::Inpw(int nOffset,int nWhichInterface)
{

	if(nWhichInterface==USLDRIVER) {
		BYTE	ReadInput[3]={USL_READ, (BYTE)nOffset};
		DWORD BytesReturned;
		WORD rv[3]={0xffff,0xffff,0xffff};

		BOOL result=DeviceIoControl(m_hUSL, IOCTL_USL_RUN_CMDS,
									ReadInput, 2,			// Input
									rv, sizeof(rv),					// Output
									&BytesReturned, NULL);
		return rv[2];
	} else {
		return theApp.m_Si6User.Inpw(nOffset);
	};
}
void CUSLut::Outp(int nOffset, int nValue,int nWhichInterface)
{

	if(m_hUSL!=NULL)
		nWhichInterface=USLDRIVER;

	if(nWhichInterface==USLDRIVER) {
		BYTE	WriteOutput[3]={USL_WRITE, (BYTE)nOffset, (BYTE)(nValue & 0xff)};
		DWORD BytesReturned;
		WORD rv[3]={0xffff,0xffff,0xffff};

		BOOL result=DeviceIoControl(m_hUSL, IOCTL_USL_RUN_CMDS,
									WriteOutput, 3,			// Input
									rv, sizeof(rv),					// Output
									&BytesReturned, NULL);
	} else {
		theApp.m_Si6User.Outp(nOffset,nValue);
	};
}

void CUSLut::Outpw(int nOffset, int nValue,int nWhichInterface)
{
	Outp(nOffset,nValue&0xff,nWhichInterface);
	Outp(nOffset+1,(nValue>>8)&0xff,nWhichInterface);
}

void CUSLut::GetVersion(char *Version,int Length)
{
	DWORD BytesReturned;

	Version[0]=0;
	BOOL result = DeviceIoControl( m_hUSL, IOCTL_USL_GET_VERSION,
							NULL, 0,			// Input
							Version, Length,	// Output
							&BytesReturned, NULL);

}



void CUSLut::GetVersion(CString* Version)
{
	char str[256];

	GetVersion(str,sizeof str);
	Version->Empty();
	Version->GetBufferSetLength(strlen(str));
	for(UINT ii=0;ii<strlen(str);ii++) {
		Version->SetAt(ii,str[ii]);
	}
}

void CUSLut::EnableInterrupts()
{

	DWORD BytesReturned;
	WORD rv[8];


	BYTE ConnectToInterrupts[] ={
		USL_IRQ_CONNECT, 0x00, 0x00, 0x00,	// Connect to interrupt
	};

	if( int nn=DeviceIoControl( m_hUSL, IOCTL_USL_RUN_CMDS,
							ConnectToInterrupts, sizeof ConnectToInterrupts,
							rv, sizeof(rv),
							&BytesReturned, NULL)){
	}

}

void CUSLut::DisableInterrupts()
{
#define	PARPORT_CONTROL 0
	BYTE DisableInterrupts[] =
	{
		USL_WRITE, PARPORT_CONTROL, 0xCC,			// Disable interrupts
	};
	WORD rv[3];
	DWORD BytesReturned;


	BOOL result = DeviceIoControl( m_hUSL, IOCTL_USL_RUN_CMDS,
							DisableInterrupts, sizeof DisableInterrupts,	// Input
							rv, sizeof(rv),									// Output
							&BytesReturned, NULL);
}




BOOL CUSLut::ReadAdc200Memory(char* Buffer,int nLength,int nWhichInterface)
{
	BOOL result;

	if(nWhichInterface==USLDRIVER) {
		DWORD BytesReturned;

		result = DeviceIoControl( m_hUSL, IOCTL_USL_READ_ADC200_MEMORY,
							NULL, 0,			// Input
							Buffer, nLength,	// Output
							&BytesReturned, NULL);
	} else {
		DWORD BytesReturned;
		result = DeviceIoControl( 	theApp.m_Si6User.m_hSi6, IOCTL_SI6_READ_ADC200_MEMORY,	NULL, 0,			// Input
							Buffer, nLength,	// Output
							&BytesReturned, NULL);
	};
	return result;
}


BOOL CUSLut::ShareTimeSlotStruct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hUSL, IOCTL_USL_SHARE_TIMESLOT_STRUCT,
							&m_TS[0], 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

//This will be done automatically but it is good practice to call it if you have shared memory

BOOL CUSLut::FreeTimeSlotStruct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hUSL, IOCTL_USL_FREE_TIMESLOT_STRUCT,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}


////////////////////////////////////////////////////////////////////////////////
//The USLDriver fills the DSP structure with results every IRQ
//This Data is also copied to the SI6Driver so that the SI6Driver can
//Fill the scan array
//Call this routine after first initializing the SI6 scan data array
/////////////////////////////////////////////////////////////////////////////////
BOOL CUSLut::ShareSi6Struct(struct SI6Data *pSi6)
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hUSL, IOCTL_USL_SHARE_SI6_USER_STRUCT,
							pSi6, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

//This will be done automatically but it is good practice to call it if you have shared memory

BOOL CUSLut::FreeSi6Struct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hUSL, IOCTL_USL_FREE_SI6_USER_STRUCT,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

////////////////////////////////////////////////////////////////////////////////
//The USLDriver fills the DSP structure with results every IRQ
//This Data is also copied to the ServoDriver so that the ServoDriver can
//Fill the scan array
//Call this routine after first initializing the Servo scan data array
/////////////////////////////////////////////////////////////////////////////////
BOOL CUSLut::ShareServoStruct(struct SERVOData *pServo)
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hUSL, IOCTL_USL_SHARE_SERVO_USER_STRUCT,
							pServo, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

//This will be done automatically but it is good practice to call it if you have shared memory

BOOL CUSLut::FreeServoStruct()
{
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_hUSL, IOCTL_USL_FREE_SERVO_USER_STRUCT,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}

BOOL CUSLut::AllocateAndShareBscanMemory()
{
	DWORD BytesReturned;

	for(int ii=0;ii<8;ii++) {
		m_TS[ii].Adc.pBscanArray=m_BscanArray;
		m_TS[ii].Adc.nBscanArraySize=sizeof m_BscanArray;
		m_TS[ii].Adc.nBscanNumber=0;
	}
	BOOL result = DeviceIoControl( m_hUSL, IOCTL_USL_SHARE_BSCAN_ARRAY,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);

	if(result==1) {
		for(int ii=0;ii<8;ii++) {
			m_TS[ii].Adc.bBscanCollectData=TRUE;
		}
	}
	return result;
}

//This will be done automatically but it is good practice to call it if you have shared memory

BOOL CUSLut::FreeAndDeallocateBscanMemory()
{
	DWORD BytesReturned;

	for(int ii=0;ii<8;ii++) {
		m_TS[ii].Adc.bBscanCollectData=FALSE;
	}
	BOOL result = DeviceIoControl( m_hUSL, IOCTL_USL_FREE_BSCAN_ARRAY,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result;
}


void CUSLut::TimeslotDefaults()
{
	int	gg;


	m_Global.nPrf=2000;
	m_Global.nTimeSlot=0;
	m_Global.nTriggerSource=USL_PR30;

	MscDefaults();
	for(gg=0;gg<8;gg++) {
		Pr2050Default(&m_TS[gg].Pr20,0,USL_PR20);
		Adc200Default(&m_TS[gg].Adc,gg);
		Dsp200Defaults(&m_TS[gg].Dsp,gg);
		Pr15Defaults(&m_TS[gg].Pr15);
//		Pl15Defaults(&m_TS[gg].Pl15);
	};
}


/////////////////////////////////////////////////////////////////////////////
//Pr20 Pr50
	int	pr50_atten[]={0,2,4,6,1,3,5,7,
				0x80,0x82,0x84,0x86,0x81,0x83,0x85,0x87,
				0x08,0x0a,0x0c,0x0e,0x09,0x0b,0x0d,0x0f,
				0x88,0x8a,0x8c,0x8e,0x89,0x8b,0x8d,0x8f,
				0x48,0x4a,0x4c,0x4e,0x49,0x4b,0x4d,0x4f,
				0xc8,0xca,0xcc,0xce,0xc9,0xcb,0xcd,0xcf,
				0x68,0x6a,0x6c,0x6e,0x69,0x6b,0x6d,0x6f,
				0xe8,0xea,0xec,0xee,0xe9,0xeb,0xed,0xef };

	int	pr20_atten[]={0,2,4,6,1,3,5,7,
				0x80,0x82,0x84,0x86,0x81,0x83,0x85,0x87,
				0x20,0x22,0x24,0x26,0x21,0x23,0x25,0x27,
				0xA0,0xA2,0xA4,0xA6,0xA1,0xA3,0xA5,0xA7,
				0x60,0x62,0x64,0x66,0x61,0x63,0x65,0x67,
				0xe0,0xe2,0xe4,0xe6,0xe1,0xe3,0xe5,0xe7,
				0x68,0x6a,0x6c,0x6e,0x69,0x6b,0x6d,0x6f,
				0xe8,0xea,0xec,0xee,0xe9,0xeb,0xed,0xef };


void CUSLut::Pr2050Initialize(struct PR2050Data *pr)
{
	Pr2050PulseVoltage(pr);
	Pr2050Attenuation(pr);
	Pr2050FiltersRectifier(pr);
	Pr2050Prf(pr);
}


void CUSLut::Pr2050Default(struct PR2050Data *pr,int Type,int Channel)
{
	pr->StructSize=sizeof(struct PR2050Data);
	pr->nAddrs=0x10;
	pr->nAttenuation=40;
	pr->nPulseVoltage=200;
	pr->nFilter=0;
	pr->nPrf = 1000;
	pr->nRectification=0;
	pr->nType=Type;
	pr->nRfChannel=Channel;
}

BOOL CUSLut::Pr2050PulseVoltage(struct PR2050Data *pr, bool bOnOff)
{
	int	value;


	if(pr->nPulseVoltage > 600) return FALSE;
	if(pr->nPulseVoltage<0) return FALSE;

	if(bOnOff){
		value = MulDiv(pr->nPulseVoltage,255,600);
	}else{
		value = MulDiv(0,255,600);
	}

	Outp((pr->nAddrs&0xff0)+0x0c,value);

	return TRUE;
}

void CUSLut::Pr2050Attenuation(struct PR2050Data	*pr)
{
	int	value;

	switch(pr->nType) {
	case USL_PR20:
		value=pr20_atten[pr->nAttenuation];
		break;
	case USL_PR50:
		value=pr50_atten[pr->nAttenuation];
		break;
	};
	switch(pr->nRectification) {
	case 0:	value &= 0xef;
		break;
	case 1:	value &= 0xef;
		break;
	case 2:	value |= 0x10;
		break;
	};
	if(pr->nRfChannel==0) {
		Outp(pr->nAddrs+4,value);
	} else {
		Outp(pr->nAddrs+8,value);
	};
}

void CUSLut::Pr2050FiltersRectifier(struct PR2050Data *pr)
{
	int	filt[8]={0,0x10,0x80,0x90,0x40,0x50,0xC0,0xD0};
	int	rect[3]={0,0x20,0x20};
	int	value;

value=filt[pr->nFilter]|rect[pr->nRectification];
if(pr->nRfChannel==0) {
	Outp(pr->nAddrs+6,value);
} else {
	Outp(pr->nAddrs+10,value);
};
}


void CUSLut::Pr2050Prf(struct PR2050Data *pr)
{
int	period;

if(pr->nPrf<=10) pr->nPrf=10;
period=1000000/pr->nPrf;
if (period<=10) period=10;

Outp(m_Global.Msc.nAddrs+3,0x74,USLDRIVER);
Outp(m_Global.Msc.nAddrs+1,(period & 0xff),USLDRIVER);
Outp(m_Global.Msc.nAddrs+1,(period >> 8),USLDRIVER);
}
//////////////////////////////////////////////////////////////////////////
//MSC

void CUSLut::MscDefaults()
{
	MscDefaults(&m_Global.Msc);
}


void CUSLut::MscDefaults(struct MSCData *msc)
{
	msc->nAddrs=0x10;
	msc->nStructSize=sizeof (struct MSCData);
}

BOOL CUSLut::MscInitialize()
{
	return MscInitialize(&m_Global.Msc);
}


BOOL CUSLut::MscInitialize(struct MSCData *msc)
{

	MscPiaInitialize(msc);
	return TRUE;
}



int CUSLut::MscCounterRead(struct MSCData *msc,int counter)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return 0;

int	value;

/*=== latch counter then read it ===*/
if(counter>3) return FALSE;
Outp(msc->nAddrs+3,0x80);
value=Inp(msc->nAddrs+counter);
value+=(Inp(msc->nAddrs+counter)*256);
return(value);
}


void CUSLut::MscInterruptCounter(struct MSCData *msc,int count)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return;

/*set 8253 counter to mode 2*/
Outp(msc->nAddrs+3,0xb0);
Outp(msc->nAddrs+2,(count & 0xff));
Outp(msc->nAddrs+2,(count >> 8));
}


void CUSLut::MscPiaInitialize(struct MSCData *msc)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return;

	Outp(msc->nAddrs+7,0x80);
	Outp(msc->nAddrs+0xb,0x80);
}


void CUSLut::MscPiaOut(struct MSCData *msc,int channel,int value)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return;
	Outp(msc->nAddrs+4+channel,value);
}



void CUSLut::MscSwitchPiaOut(struct MSCData *msc,int channel,int value)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return;
	Outp(msc->nAddrs+0x8+channel,value);
}



int CUSLut::MscAdcRead(struct MSCData *msc,int channel)
{
unsigned int	value;
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return 0;

//value=(unsigned int)Inp(msc->Addrs+8+channel);
//_asm Jmp $+2
//value=(unsigned int)Inp(msc->Addrs+8+channel);

//msc->Addrs=0x140;
value=(unsigned int)Inp(msc->nAddrs+8+channel);
value=(unsigned int)Inp(msc->nAddrs+8+channel);

return(value & 0xff);
}

int CUSLut::MscAdcRead(int channel)
{
	unsigned int	value;
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return 0;

value=(unsigned int)Inp(0x10+8+channel);
value=(unsigned int)Inp(0x10+8+channel);

return(value & 0xff);
}


void CUSLut::MscEncoderReset(struct MSCData *msc,int number)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return;

Outp(msc->nAddrs+0x14+number,0);
}

int CUSLut::MscEncoderRead(struct MSCData *msc,int encoder)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return 0;
/*
int	temp;
int	value;

encoder += encoder;
temp = Inpw(msc->nAddrs+0x10+encoder);

value = temp & 0xff;
value <<= 8;
temp >>= 8;
temp &= 0xff;
value |= temp;
return(value);
*/
	int	nTemp;

	//Bid endian
	encoder += encoder;
	nTemp=((int)Inp(msc->nAddrs+0x10+encoder)<<8);
	nTemp|=Inp(msc->nAddrs+0x10+encoder+1);
//	if(nTemp&0x8000) nTemp |= 0xffff0000;

	return(nTemp);

}

void CUSLut::MscEncoderWrite(struct MSCData *msc,int encoder,int value)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return;

int	temp;

temp = value & 0xff;
temp <<= 8;
value >>= 8;
value &= 0xff;
value |= temp;
encoder += encoder;
Outpw(msc->nAddrs+0x10+encoder,value);
}


void CUSLut::MscDacWrite(struct MSCData *msc,int dac,int value)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return;

Outp(msc->nAddrs+0x0c+dac,value);
}

void CUSLut::MscSwitchHttoChannel(int nChannel)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return;

	switch(nChannel) {
	case 1: m_Global.Msc.nControlRegister |= 0x80;
		break;
	case 0: m_Global.Msc.nControlRegister &= 0x7f;
		break;
	}

	Outp(0x10+0x09,m_Global.Msc.nControlRegister);

}

////////////////////////////////////////////////////////////////////////////////////////////////
// General


void CUSLut::RfAndClockSource(struct MSCData *msc,int Tx,int Rx)
{
	if(theApp.m_LastSettings.bMscPresent!=TRUE) return;

	switch(Rx) {
	case 0:	msc->nControlRegister &= 0xfe;
		break;
	case 1: msc->nControlRegister |= 0x01;
		break;
	};

	switch(Tx) {
	case 0: msc->nControlRegister &= 0x7f;
		break;
	case 1: msc->nControlRegister |= 0x80;
		break;
	};

	Outp(msc->nAddrs+0x09,msc->nControlRegister);

}


void CUSLut::RfAndClockSource()
{
	MSCData* pMsc = &m_Global.Msc;

	switch(theApp.m_nJobNumber) {
	case VTT_HELSINKY:
		pMsc->nControlRegister=0;
		switch(m_TS[0].nTx) {
		case USL_PR20: pMsc->nControlRegister &= 0xfe;
			break;
		case USL_PR50: pMsc->nControlRegister |= 0x80;
			break;
		case USL_PL15: pMsc->nControlRegister |= 0x02;
			break;
		};
		switch(m_TS[0].nRx) {
		case USL_PR20: pMsc->nControlRegister &= 0xfe;
			break;
		case USL_PR50: pMsc->nControlRegister |= 0x01;
			break;
		case USL_PL15: pMsc->nControlRegister |= 0x02;
			break;
		};
		switch(theApp.m_LastSettings.nProcessCard) {
		case USL_ATP_GPD0:
		case USL_ATP_GPD1:
		case USL_ATP_TIMS0:
		case USL_ATP_TIMS1:
			pMsc->nControlRegister &= 0xfb;
			break;
		case USL_ADC100: pMsc->nControlRegister |= 0x04;
			break;
		};
		break;
	case BAE_VERTICAL:
	case BAE_TROLLEY:
	case BAE_FLAPERONS:
		pMsc->nControlRegister=0;
		switch(m_TS[0].nAmplifier) {
		case USL_PR15: pMsc->nControlRegister &= 0x00;
			break;
		case USL_PL15: pMsc->nControlRegister |= 0xff;
			break;
		};
		break;
	}

	Outp(pMsc->nAddrs+0x09,pMsc->nControlRegister);

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// DSP200

void CUSLut::Dsp200Defaults(struct	DSP200Data	*dsp,int nTimeSlot)
{
int	ii;

dsp->StructSize=sizeof(struct DSP200Data);
dsp->nAddrs = 0x38;
dsp->nIrq = 0x05;
dsp->IntGate = 0;
dsp->nCardType=0;					//0 Non Mux, 1 Mux
dsp->nTimeSlot=nTimeSlot;
for(ii=0;ii<8;ii++) {
	dsp->TriggerMode[ii]=0;
	dsp->Delay[ii]	= ii*40;
	dsp->Width[ii] = 20;
	dsp->InterfaceThreshold[ii]=0;
	dsp->InterfaceSlope[ii]=1;
	dsp->SignDetect[ii]=1;
};
for(ii=0;ii<2;ii++) {
	dsp->TimsTriggerMode[ii]=0;
	dsp->TimsStartSlope[ii]=1;
	dsp->TimsStopSlope[ii]=1;
};
dsp->TriggerMode[0]=1;
dsp->ClkRate=1250000;
dsp->Prf = 2000;
dsp->nPrfMode=0;
}

void CUSLut::Dsp200CheckVariables(DSP200Data *dsp,int nTimeSlot)
{

	dsp->StructSize=sizeof(struct DSP200Data);
	dsp->IntGate %= 3;
	dsp->nCardType %= 2;					//0 Non Mux, 1 Mux
	dsp->nTimeSlot=nTimeSlot;
	for(int ii=0;ii<8;ii++) {
		dsp->TriggerMode[ii] %= 2;
		dsp->InterfaceThreshold[ii] &=0xff;
		dsp->InterfaceSlope[ii] %= 2;
		dsp->SignDetect[ii] %= 2;
	};
	for(int ii=0;ii<2;ii++) {
//		dsp->TimsTriggerMode[ii] %= 2;
		dsp->TimsStartSlope[ii] %= 2;
		dsp->TimsStopSlope[ii] %= 2;
	};
	dsp->ClkRate=1250000;
	dsp->nPrfMode %= 2;

}

void CUSLut::Dsp200Initialize()
{
	int	nTs;

	for(nTs=0;nTs<8;nTs++) {
		Dsp200Initialize(&m_TS[nTs].Dsp,nTs);
	};
}

void CUSLut::Dsp200Initialize(struct	DSP200Data *dsp,int nTimeSlot)
{
	int	ii;

	Dsp200CheckVariables(dsp,nTimeSlot);

	for(ii=0;ii<8;ii++) {
		Dsp200GpdSetup(dsp,ii);
	};
	Dsp200GpdInterface(dsp);
	Dsp200Tims(dsp,0);
	Dsp200Tims(dsp,1);
}


void CUSLut::Dsp200WaitForFifo()
{
	while(m_bDspFifoWait) {
		ASSERT(1);
	}

}

void CUSLut::Dsp200GpdSetup(struct	DSP200Data *dsp,int which)
{
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;
int	sign[4]={0x05,0x06,0x07,0x07};
int	mode[4]={0,0x20,0x22,0x22};

	Dsp200CheckTimeSlot(dsp);
	Dsp200WaitForFifo();
	m_bDspFifoWait=TRUE;

Outp(dsp->nAddrs,0x81);
Outp(dsp->nAddrs,which);
Outp(dsp->nAddrs,mode[dsp->TriggerMode[which] % 4]);
Outp(dsp->nAddrs,sign[dsp->SignDetect[which] % 4]);
Outp(dsp->nAddrs,dsp->Delay[which]&0x3f);
Outp(dsp->nAddrs,(dsp->Delay[which]>>6)&0x3f);
Outp(dsp->nAddrs,dsp->Width[which]&0x3f);
Outp(dsp->nAddrs,(dsp->Width[which]>>6)&0x3f);
Outp(dsp->nAddrs,dsp->InterfaceThreshold[which]&0x3f);
Outp(dsp->nAddrs,(dsp->InterfaceThreshold[which]>>6)&0x03);
Outp(dsp->nAddrs,0xff);
m_bDspFifoWait=FALSE;
}



void CUSLut::Dsp200GpdInterface(struct	DSP200Data *dsp)
{
int	ii;
int	slope=0;
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;

for(ii=0;ii<dsp->IntGate;ii++) if(dsp->TriggerMode[ii]>1) slope = 1;
for(ii=dsp->IntGate+1;ii<8;ii++) if(dsp->TriggerMode[ii]>1) slope = 1;

	Dsp200CheckTimeSlot(dsp);
	Dsp200WaitForFifo();
	m_bDspFifoWait=TRUE;

Outp(dsp->nAddrs,0x83);
Outp(dsp->nAddrs,0x00);
if(slope==0) {
	Outp(dsp->nAddrs,0x00);
} else {
	switch(dsp->InterfaceSlope[dsp->IntGate]) {
	case 0:	Outp(dsp->nAddrs,0x38+dsp->IntGate);
		break;
	case 1:	Outp(dsp->nAddrs,0x30+dsp->IntGate);
		break;
	};
};
Outp(dsp->nAddrs,0xff);
m_bDspFifoWait=FALSE;
}




void CUSLut::Dsp200Tims(struct	DSP200Data *dsp,int which)
{
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;

	Dsp200CheckTimeSlot(dsp);
	Dsp200WaitForFifo();
	m_bDspFifoWait=TRUE;
	Outp(dsp->nAddrs,0x82);
	Outp(dsp->nAddrs,which);
	Outp(dsp->nAddrs,dsp->TimsTriggerMode[which]);
	Outp(dsp->nAddrs,0xff);
	m_bDspFifoWait=FALSE;
}




void CUSLut::Dsp200Prf(struct	DSP200Data *dsp)
{
int period;
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;

if(dsp->Prf==0) dsp->Prf=100;
dsp->ClkRate=1250000;
period=dsp->ClkRate/dsp->Prf;

	Dsp200WaitForFifo();
	m_bDspFifoWait=TRUE;

Outp(dsp->nAddrs,0x84);
Outp(dsp->nAddrs,period&0x3f);
Outp(dsp->nAddrs,(period>>6)&0x3f);
Outp(dsp->nAddrs,(period>>12)&0x3f);
Outp(dsp->nAddrs,0xff);
m_bDspFifoWait=FALSE;
}

BOOL CUSLut::Dsp200Present()
{
	return Dsp200Present(&m_TS[m_Global.nTimeSlot].Dsp);
}

BOOL CUSLut::Dsp200Present(struct DSP200Data *dsp)
{
//return FALSE if not present
//return TRUE if present


if((Inp(dsp->nAddrs+3) & 0x02)==0) return TRUE;
return FALSE;
}


void CUSLut::Dsp200ClearFifo(struct	DSP200Data *dsp)
{
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;

while((Inp(dsp->nAddrs+2)&0x02) && !(Inp(dsp->nAddrs+3)&0x03)) {
	Inp(dsp->nAddrs);
};
}



void CUSLut::Dsp200TriggerShot(struct	DSP200Data	*dsp)
{
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;
	Outp(dsp->nAddrs+1,0);		//Clear Interrupt
	Outp(dsp->nAddrs+3,0x02);	//Trigger One Shot
}

void CUSLut::Dsp200Reset()
{
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;
	Dsp200Reset(&m_TS[m_Global.nTimeSlot].Dsp);
}

void CUSLut::Dsp200Reset(struct DSP200Data	*dsp)
{
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;

	int	value;
	Outp(dsp->nAddrs+2,2);					//Hardware reset
	Sleep(100);

	Dsp200ClearFifo(dsp);

	value=0x02;
	if(dsp->nPrfMode) value|=0x04;
	if(dsp->nMuxMode) value|=0x08;
	Outp(dsp->nAddrs,0x80);
	Outp(dsp->nAddrs,value);
	Outp(dsp->nAddrs,0xff);
	Sleep(10);

	Dsp200CheckTimeSlot(dsp);
	m_bDspFifoWait=TRUE;

	Outp(dsp->nAddrs,0x85);					//reset max timeslot number
	Outp(dsp->nAddrs,dsp->nTimeSlot|0x04);
	Outp(dsp->nAddrs,0xff);
	m_bDspFifoWait=FALSE;
}

void CUSLut::Dsp200Restart()
{
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;

	struct	DSP200Data *dsp;

	dsp = &m_TS[m_Global.nTimeSlot].Dsp;

	Dsp200Control(dsp,0);

	Dsp200ClearFifo(dsp);					//remember to put 1 prf delay in
	Sleep(100);
	if(dsp->nCardType==1) {
		Outp(dsp->nAddrs+1,0);	//Clear Interrupt
	} else {
		Inp(dsp->nAddrs+1);	//Clear Interrupt
	}
	Dsp200Control(dsp,1);



}


void CUSLut::Dsp200Control(struct	DSP200Data	*dsp,int	process)
{
int	value;
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;

value=process&1;
if(dsp->nPrfMode) value|=0x04;
if(dsp->nMuxMode) value|=0x08;

	Dsp200CheckTimeSlot(dsp);
	Dsp200WaitForFifo();
	m_bDspFifoWait=TRUE;

Outp(dsp->nAddrs,0x80);
Outp(dsp->nAddrs,value);
Outp(dsp->nAddrs,0xff);
m_bDspFifoWait=FALSE;
}




void CUSLut::Dsp200Timeslot(struct DSP200Data *dsp)
{
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;

	Dsp200WaitForFifo();
m_bDspFifoWait=TRUE;

Outp(dsp->nAddrs,0x85);
Outp(dsp->nAddrs,dsp->nTimeSlot);
Outp(dsp->nAddrs,0xff);
m_nLastDspTimeSlot = dsp->nTimeSlot;

m_bDspFifoWait=FALSE;
}

void CUSLut::Dsp200StartProcessing(struct DSP200Data *dsp)
{
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;
	int	ii;

	switch(theApp.m_nJobNumber) {
	default:
		if(dsp->nCardType==1) {
			for(ii=0;ii<8;ii++) {
				m_TS[ii].Dsp.nMuxMode=1;
				m_TS[ii].Dsp.nPrfMode=0;
			};
		} else {
			dsp->nMuxMode=0;
		}
		EnableInterrupts();

		if(dsp->nCardType==1) {
			Outp(dsp->nAddrs+1,0);	//Clear Interrupt
		} else {
//			Inp(dsp->nAddrs+1);	//Clear Interrupt
			Outp(dsp->nAddrs+1,1);	//Clear Interrupt
		}
		Dsp200Control(dsp,1);
		break;
	case TWI:
	case 470:	//BAE SAMLESBURY 6 METER MECCASONICS RIG
//	case AWE_ALDERMARSTON:
	case AIM_COMPOSITES:
		if(dsp->nCardType==1) {
			for(ii=0;ii<8;ii++) {
				m_TS[ii].Dsp.nMuxMode=0;
				m_TS[ii].Dsp.nPrfMode=0;
			};
		} else {
			dsp->nMuxMode=0;
		}
		EnableInterrupts();

		if(dsp->nCardType==1) {
			Inp(dsp->nAddrs+1);	//Clear Interrupt
		} else {
			Outp(dsp->nAddrs+1,0);	//Clear Interrupt
		}
		Dsp200Control(dsp,1);
		break;
	}
}

void CUSLut::Dsp200StopProcessing(struct DSP200Data *dsp)
{
	if(theApp.m_LastSettings.bDspPresent!=TRUE) return;

	Dsp200CheckTimeSlot(dsp);
	Dsp200WaitForFifo();
	m_bDspFifoWait=TRUE;
	Outp(dsp->nAddrs,0x80);
	Outp(dsp->nAddrs,0x00);	//susspend processing
	Outp(dsp->nAddrs,0xff);

	Outp(dsp->nAddrs,0x80);
	Outp(dsp->nAddrs,0x02);	//purge fifos
	Outp(dsp->nAddrs,0xff);
m_bDspFifoWait=FALSE;
	DisableInterrupts();
	Sleep(20);

}

BOOL CUSLut::Dsp200CalculateHardware(int nTimeSlot)
{
	int	gg;
	static int nloop=0;
	nloop++;

	for(gg=0;gg<8;gg++) {
		Dsp200CalculateHardware(nTimeSlot,gg);
	};

	return TRUE;
}

BOOL CUSLut::Dsp200CalculateHardware(int nTimeSlot,int nWhich)
{
	int	nDelay,nWidth;
	struct DSP200Data* pDsp=&m_TS[nTimeSlot].Dsp;
	struct ADC200Data* pAdc=&m_TS[nTimeSlot].Adc;
	struct GatesData* pGate=&m_TS[nTimeSlot].Gate;

	if(pAdc->fSamplePeriod==0.0) return FALSE;
	pDsp->IntGate = 0;

	switch(pGate->nGateMode[nWhich]) {
	case 0:	//off;
		nDelay=0;
		nWidth=0;
		break;
	case 1:	//main bang
		pDsp->TriggerMode[nWhich]=1;
		nDelay=(int)((float)(pGate->nNsDelay[nWhich]-pAdc->nDelay)/pAdc->fSamplePeriod);
		nWidth=(int)((float)pGate->nNsWidth[nWhich]/pAdc->fSamplePeriod);
		break;
	case 2:	//interface
		pDsp->TriggerMode[nWhich]=2;
		nDelay=(int)((float)pGate->nNsDelay[nWhich]/pAdc->fSamplePeriod);
		nWidth=(int)((float)pGate->nNsWidth[nWhich]/pAdc->fSamplePeriod);
		break;
	default:
		pDsp->TriggerMode[nWhich]=3;
		nDelay=(int)((float)pGate->nNsDelay[nWhich]/pAdc->fSamplePeriod);
		nWidth=(int)((((float)(pGate->nTimeSample[1]-pGate->nTimeSample[0])*pAdc->fSamplePeriod) - (float)pGate->nNsFarDeadZone[nWhich])/pAdc->fSamplePeriod) - nDelay;
		break;
	};
	pGate->nSampleDelay[nWhich] = nDelay;
	pGate->nSampleWidth[nWhich] = nWidth;
	
	pDsp->Delay[nWhich]=nDelay;
	pDsp->Width[nWhich]=nWidth;
	pDsp->InterfaceThreshold[nWhich]=abs((pGate->nThreshold[nWhich]*127)/100);
	if(pGate->nThreshold[nWhich]>=0){
		pDsp->SignDetect[nWhich]=0;
		pDsp->InterfaceSlope[nWhich]=0;
	} else {
		pDsp->SignDetect[nWhich]=1;
		pDsp->InterfaceSlope[nWhich]=1;
	};
	Dsp200GpdSetup(pDsp,nWhich);
	Dsp200GpdInterface(pDsp);


	if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30] == LSA)
		theApp.m_LSA.MaintainUniformTSs(nTimeSlot);

	return TRUE;
}


bool CUSLut::CopyGatesAcrossTimeslots(int nTimeslot)
{

	/*
	This was intened for when using an LSA and we want to maintain gates across all timeslots
	However we don't wnat to copy the amplitudes of the gates
	*/


	/*
	struct	GatesData {
	int nStructSize; //4 bytes
	int	nGateMode[8];
	int	nNsDelay[8];
	int	nNsWidth[8];
	int	nSampleDelay[8];
	int	nSampleWidth[8];
	int	nThreshold[8];
	int	nDefectThreshold[8];
	int	nSignDetect[8];
	int	nTimsDetectionMode[8];
	int	nCollectAmp[8];
	int	nCollectTime[8];
	int	nCollectPhase[8];
	int	nTOFStartGate[8];
	int	nTOFStopGate[8]; //448 bytes
	WCHAR cName[8][60]; //960bytes
	//1412 bytes to here

	int	nAmplitude[8];
	int	nTimeSample[8];
	int	nPhase[8];

	int	nNsTimeBaseWidth;	//Used for B scan processing

	int nNsFarDeadZone[8]; //Elastic gate
	
	int	nSpare[33];*/

	for(int ii=0;ii<256;ii++){
		m_TS[ii].Gate.nStructSize = m_TS[nTimeslot].Gate.nStructSize;

		for(int jj=0;jj<8;jj++){
			m_TS[ii].Gate.nGateMode[jj] = m_TS[nTimeslot].Gate.nGateMode[jj];
			m_TS[ii].Gate.nNsDelay[jj] = m_TS[nTimeslot].Gate.nNsDelay[jj];
			m_TS[ii].Gate.nNsWidth[jj] = m_TS[nTimeslot].Gate.nNsWidth[jj];
			m_TS[ii].Gate.nSampleDelay[jj] = m_TS[nTimeslot].Gate.nSampleDelay[jj];
			m_TS[ii].Gate.nSampleWidth[jj] = m_TS[nTimeslot].Gate.nSampleWidth[jj];
			m_TS[ii].Gate.nThreshold[jj] = m_TS[nTimeslot].Gate.nThreshold[jj];
			m_TS[ii].Gate.nDefectThreshold[jj] = m_TS[nTimeslot].Gate.nDefectThreshold[jj];
			m_TS[ii].Gate.nSignDetect[jj] = m_TS[nTimeslot].Gate.nSignDetect[jj];
			m_TS[ii].Gate.nTimsDetectionMode[jj] = m_TS[nTimeslot].Gate.nTimsDetectionMode[jj];
			m_TS[ii].Gate.nCollectAmp[jj] = m_TS[nTimeslot].Gate.nCollectAmp[jj];
			m_TS[ii].Gate.nCollectTime[jj] = m_TS[nTimeslot].Gate.nCollectTime[jj];
			m_TS[ii].Gate.nCollectPhase[jj] = m_TS[nTimeslot].Gate.nCollectPhase[jj];
			m_TS[ii].Gate.nTOFStartGate[jj] = m_TS[nTimeslot].Gate.nTOFStartGate[jj];
			m_TS[ii].Gate.nTOFStopGate[jj] = m_TS[nTimeslot].Gate.nTOFStopGate[jj];
		}
		m_TS[ii].Gate.nNsPeakWidth = m_TS[nTimeslot].Gate.nNsPeakWidth;

		memcpy(&m_TS[ii].Gate.cName,&m_TS[nTimeslot].Gate.cName,960);
		m_TS[ii].Gate.nNsTimeBaseWidth = m_TS[nTimeslot].Gate.nNsTimeBaseWidth;
	}


	return true;
}

BOOL CUSLut::Dsp200CalculateTimsHardware(int nTimeSlot,int nWhich)
{
	struct DSP200Data* pDsp=&m_TS[nTimeSlot].Dsp;
	struct ADC200Data* pAdc=&m_TS[nTimeSlot].Adc;
	struct GatesData* pGate=&m_TS[nTimeSlot].Gate;

	switch(pGate->nTimsDetectionMode[nWhich]) {
	case 0:	pDsp->TimsTriggerMode[nWhich]=0;
		break;
	case 1:	pDsp->TimsTriggerMode[nWhich]=0x21;
		break;
	case 2:	pDsp->TimsTriggerMode[nWhich]=0x20;
		break;
	case 3:	pDsp->TimsTriggerMode[nWhich]=0x21;
		break;
	case 4:	pDsp->TimsTriggerMode[nWhich]=0x20;
		break;
	};
	switch(pGate->nTimsDetectionMode[nWhich]) {
	case 0:	pDsp->TimsTriggerMode[nWhich]&=0xef;
		break;
	case 1:	pDsp->TimsTriggerMode[nWhich]|=0x10;
		break;
	};
	switch(pGate->nTimsDetectionMode[nWhich]) {
	case 1:	pDsp->TimsTriggerMode[nWhich]&=0xfb;
		break;
	case 0:	pDsp->TimsTriggerMode[nWhich]|=0x04;
		break;
	};
	switch(pGate->nTimsDetectionMode[nWhich]) {
	case 1:	pDsp->TimsTriggerMode[nWhich]&=0xf7;
		break;
	case 0:	pDsp->TimsTriggerMode[nWhich]|=0x08;
		break;
	};


	Dsp200Tims(pDsp,nWhich);
	return TRUE;
}


void CUSLut::Dsp200GetStartStopTimeNs(int *nStart,int *nStop,bool *bPulseEcho,int *nTransmitter,int nTimeSlot)
{
	ADC200Data* pAdc = &m_TS[m_Global.nTimeSlot].Adc;
	GatesData* pGates = &m_TS[m_Global.nTimeSlot].Gate;
	DSP200Data* pDsp = &m_TS[m_Global.nTimeSlot].Dsp;

	*nStart = (int)((float)pDsp->TimsStartPos[0]*pAdc->fSamplePeriod) + pAdc->nDelay;
	*nStop = (int)((float)pDsp->TimsStopPos[0]*pAdc->fSamplePeriod) + pAdc->nDelay;

	if(bPulseEcho!=NULL) {
		if(m_TS[nTimeSlot].Pr15.nTxRxMode != 0) {
			*bPulseEcho=FALSE;
		} else {
			*bPulseEcho=TRUE;
		}
	}
	if(nTransmitter!=NULL)
	 	*nTransmitter = 0;

}

void CUSLut::Dsp200CheckTimeSlot(DSP200Data *pDsp)
{

	if(m_nLastDspTimeSlot - pDsp->nTimeSlot) {
		Dsp200Timeslot(pDsp);
	}


}

 /////////////////////////////////////////////////////////////////////////
// ADC200

void CUSLut::Adc200Default()
{
	Adc200Default(&m_TS[m_Global.nTimeSlot].Adc,m_Global.nTimeSlot);
}


void CUSLut::Adc200Default(struct ADC200Data *adc,int nTimeSlot)
{

	memset(adc,0,sizeof (struct ADC200Data));
	adc->nStructSize=sizeof (struct ADC200Data);
	adc->nTimeslot=nTimeSlot;
	adc->nConvertTimeslot=0;
	adc->nAddrs=0x30;
	adc->nMemoryAddrs=0xd4000l;
	adc->nMemoryWindowSize=4;
	adc->irq=5;
	adc->irq_enable=0;
	adc->convert_enable=1;
	adc->prf_source=1;			//0 internal 1 external
	adc->nTriggerSource=0;
	adc->rf_source=0;
	adc->nConversionRate=1;   // 100 MHz
	adc->blank_marker=0;
	adc->delay_marker=0;
	adc->gate_marker=1;
	adc->interface_marker=0;
	adc->pre_trigger=0;
	adc->nAcqLength=1024;
	adc->nGain=0x800;
	adc->nOffset=0x800;
	adc->interface_threshold=200;
	adc->prf=1000;
	adc->present=FALSE;

	adc->nDelay=0;
	adc->nWidth=20000;
	Adc200CalculateHardware(adc);

}

void CUSLut::Adc200Initialize()
{
	Adc200Initialize(&m_TS[m_Global.nTimeSlot].Adc);
}

void CUSLut::Adc200Initialize(struct ADC200Data *adc)
{

//	Adc200PurgeBothFifos(adc);
//	Sleep(50);
	Adc200Command0(adc);
	Adc200Command1(adc);
	Adc200Command2(adc);
	Adc200Command5(adc);
}

void CUSLut::Adc200Prf(struct ADC200Data *adc)
{
	Adc200Command2(adc);
}

void CUSLut::Adc200WriteTimeslot(struct ADC200Data *adc,int clear_flag)
{
	Adc200Command3(adc,clear_flag);
}

void CUSLut::Adc200ConvertTimeslot(struct ADC200Data *adc)
{
	Adc200Command4(adc);
}

void CUSLut::Adc200Delay(struct ADC200Data *adc)
{
	Adc200Command1(adc);
}
void CUSLut::Adc200Width(struct ADC200Data *adc)
{
	Adc200Command1(adc);
}

void CUSLut::Adc200RFSource(struct ADC200Data *adc)
{
	Adc200Command1(adc);
}

void CUSLut::Adc200MemoryAddrs(struct ADC200Data *adc)
{
	Adc200Command5(adc);
}


BOOL CUSLut::Adc200ReadParameters()
{
	return Adc200ReadParameters(&m_TS[m_Global.nTimeSlot].Adc);
}

BOOL CUSLut::Adc200ReadParameters(struct ADC200Data *adc)
{
	int	timeout=0,value;
	int	read_count=0;

	Adc200PurgeOutFifo(adc);
	Adc200Command11(adc);
	Sleep(50);

	memset(adc->serial_number,0,sizeof adc->serial_number);
	memset(&adc->type,0,sizeof adc->type);
	memset(&adc->nMemorySize,0,sizeof adc->nMemorySize);
	memset(adc->mod_record,0,sizeof adc->mod_record);
	memset(adc->tester,0,sizeof adc->tester);
	memset(adc->test_date,0,sizeof adc->test_date);

	while((timeout<10000)&&(read_count<27)) {
		if(Inp(adc->nAddrs+1)&0x04) {
			value=Inp(adc->nAddrs);
			switch(read_count) {
			case 0:
				if(value!=0x8b) {
					Adc200PurgeOutFifo(adc);
					adc->present=FALSE;
					return FALSE;
				};
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7: adc->serial_number[read_count-1]=value;
				break;
			case 8: adc->type=value;
				break;
			case 9: adc->nMemorySize=value;
				break;
			case 10:
			case 11:	adc->mod_record[read_count-10]=value;
				break;
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:	adc->tester[read_count-12]=value;
				break;
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:	adc->test_date[read_count-18]=value;
				break;
			case 26:
				break;
			};
			read_count++;
			timeout=0;
		};
		timeout++;
	};
	adc->present=TRUE;
	return TRUE;
}


void CUSLut::Adc200CalculateHardware()
{
	Adc200CalculateHardware(&m_TS[m_Global.nTimeSlot].Adc);
}

void CUSLut::Adc200CalculateHardware(struct ADC200Data *adc)
{

	int	nAcqLength,nSamplePeriod,nClkRate;

	adc->nHardwareDelay=adc->nDelay / 10;

	nSamplePeriod=10;
	for(nClkRate=3;nClkRate>=0;nClkRate--) {
		nAcqLength=adc->nWidth / nSamplePeriod;
		if(nAcqLength<4096) goto finished;
		nSamplePeriod*=2;
	};
	finished:
	adc->nAcqLength=nAcqLength;
	adc->fSamplePeriod=(float)nSamplePeriod;
	adc->nConversionRate=nClkRate;
}


int CUSLut::Adc200ReadArray(struct ADC200Data *adc,char *trace)
{
	int	timeout=0;
	int	flag=FALSE;
	if(trace==NULL) return FALSE;

	adc->control_reg&=0xf3;		//Request ADC access
	adc->control_reg|=0x02;
	Outp(adc->nAddrs+1,adc->control_reg);
	while(timeout<10000) {
		if((Inp(adc->nAddrs+1)  & 0x01) == 0 ) {
			timeout++;
		} else {
			int nAcqLength = adc->nAcqLength;
			if(nAcqLength>4095) nAcqLength=4095;
			ReadAdc200Memory(trace,nAcqLength);
			timeout=10000;
		};
	};

	adc->control_reg&=0xf5;	//Clear ADC access
	adc->control_reg|=0x04;
	Outp(adc->nAddrs+1,adc->control_reg);
	return flag;

}


void CUSLut::Adc200PurgeBothFifos(struct ADC200Data *adc)
{

	adc->nPurgeFifo=1;
	Adc200Command0(adc);
	adc->nPurgeFifo=0;
}


void CUSLut::Adc200PurgeOutFifo(struct ADC200Data *adc)
{
	int	value,loop=0;

	while((Inp(adc->nAddrs+1)&0x04)&&(loop<10000)) {
		value=Inp(adc->nAddrs);
		loop++;
	};
}

void CUSLut::Adc200ResetMaxTimeSlot(struct ADC200Data *adc)
{

	Adc200Command3(adc,1);
}

void CUSLut::Adc200SelectRFSource(ADC200Data *pAdc,int nPr)
{
	switch(nPr) {
	default: pAdc->rf_source = 0;
		break;
	case USL_PR15: pAdc->rf_source = pAdc->nPR15ADCInput;
		break;
	case USL_PR30: pAdc->rf_source = pAdc->nPR30ADCInput;
		break;
	case USL_PL15: pAdc->rf_source = pAdc->nPL15ADCInput;
		break;
	}
	Adc200Command1(pAdc);
}


void CUSLut::Adc200Reset()
{
	Outp(m_TS[0].Adc.nAddrs,0x09);
}

void CUSLut::Adc200Command0(struct ADC200Data *adc)
{
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC200) return;

	int	value=0;

	Adc200CheckTimeSlot(adc);
	while(m_bAdcFifoWait) {
		Sleep(1);
	};
	m_bAdcFifoWait=TRUE;

	value=adc->prf_source;
	value|=((adc->irq_enable&1)<<1);
	value|=((adc->nPurgeFifo&1)<<2);
	value|=((adc->mux_run_mode&1)<<3);
	Outp(adc->nAddrs,0x80);
	Outp(adc->nAddrs,value);
	Outp(adc->nAddrs,0xff);
	m_bAdcFifoWait=FALSE;
}

void CUSLut::Adc200Command1(struct ADC200Data *adc)
{
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC200) return;
	int	value;

	Adc200CheckTimeSlot(adc);
	while(m_bAdcFifoWait) {};
	m_bAdcFifoWait=TRUE;
	value=adc->convert_enable&1;
	value|=((adc->nTriggerSource&1)<<1);
	value|=((adc->nTriggerSlope&1)<<2);
	value|=((adc->rf_source&3)<<3);
	Outp(adc->nAddrs,0x81);										//0
	Outp(adc->nAddrs,value);										//1
	Outp(adc->nAddrs,adc->nConversionRate&0x03);			//2
	value=adc->blank_marker;
	value|=((adc->delay_marker&1)<<1);
	value|=((adc->gate_marker&1)<<2);
	value|=((adc->interface_marker&1)<<3);
	Outp(adc->nAddrs,value);										//3
	Outp(adc->nAddrs,adc->pre_trigger&0x3f);				//4
	Outp(adc->nAddrs,(adc->pre_trigger>>6)&0x3f);			//5
	Outp(adc->nAddrs,(adc->pre_trigger>>12)&0x0f);		//6
	Outp(adc->nAddrs,adc->blanking&0x3f);					//7
	Outp(adc->nAddrs,(adc->blanking>>6)&0x3f);				//8
	Outp(adc->nAddrs,adc->nHardwareDelay&0x3f);						//9
	Outp(adc->nAddrs,(adc->nHardwareDelay>>6)&0x3f);					//10
	Outp(adc->nAddrs,(adc->nHardwareDelay>>12)&0x0f);				//11
	Outp(adc->nAddrs,adc->tes_delay&0x3f);					//12
	Outp(adc->nAddrs,(adc->tes_delay>>6)&0x03);			//13
	Outp(adc->nAddrs,adc->nAcqLength&0x3f);					//14
	Outp(adc->nAddrs,(adc->nAcqLength>>6)&0x3f);			//15
	Outp(adc->nAddrs,(adc->nAcqLength>>12)&0x07);			//16
	Outp(adc->nAddrs,adc->nGain&0x3f);							//17
	Outp(adc->nAddrs,(adc->nGain>>6)&0x3f);					//18
	Outp(adc->nAddrs,adc->nOffset&0x3f);						//19
	Outp(adc->nAddrs,(adc->nOffset>>6)&0x3f);				//20
	Outp(adc->nAddrs,adc->interface_threshold&0x3f);		//21
	Outp(adc->nAddrs,(adc->interface_threshold>>6)&0x0f);//22
	Outp(adc->nAddrs,0xff);										//23
	m_bAdcFifoWait=FALSE;
}


void CUSLut::Adc200Command2(struct ADC200Data *adc)
{
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC200) return;
	int	value;

	Adc200CheckTimeSlot(adc);
	while(m_bAdcFifoWait) {};
	m_bAdcFifoWait=TRUE;
	if(adc->prf<=0) adc->prf=100;
	value=(int)(2670000/(long)adc->prf);
	Outp(adc->nAddrs,0x82);
	Outp(adc->nAddrs,value&0x3f);
	Outp(adc->nAddrs,(value>>6)&0x3f);
	Outp(adc->nAddrs,(value>>12)&0x0f);
	Outp(adc->nAddrs,0xff);
	m_bAdcFifoWait=FALSE;
}

void CUSLut::Adc200Command3(struct ADC200Data *adc,int action)
{
	
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC200) return;

	while(m_bAdcFifoWait) {};
	m_bAdcFifoWait=TRUE;
	Outp(adc->nAddrs,0x83);
	Outp(adc->nAddrs,(adc->nTimeslot&0x07) | (action<<5));
	Outp(adc->nAddrs,0xff);
	m_nLastAdcTimeSlot = adc->nTimeslot;
	m_bAdcFifoWait=FALSE;
}


void CUSLut::Adc200Command4(struct ADC200Data *adc)
{
	
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC200) return;

	Adc200CheckTimeSlot(adc);
	while(m_bAdcFifoWait) {};
	m_bAdcFifoWait=TRUE;
	Outp(adc->nAddrs,0x84);
	Outp(adc->nAddrs,adc->nConvertTimeslot&0x07);
	Outp(adc->nAddrs,0xff);
	m_bAdcFifoWait=FALSE;
}

void CUSLut::Adc200Command5(struct ADC200Data *adc)
{
	
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC200) return;
char	lsn=0,msn=0;

	Adc200CheckTimeSlot(adc);
	while(m_bAdcFifoWait) {};
	m_bAdcFifoWait=TRUE;
	lsn=(char)(adc->nMemoryAddrs >> 12) & 0x0f;		//24
	msn=(char)(adc->nMemoryAddrs >> 16) & 0x0f;		//28
	Outp(adc->nAddrs,0x85);
	Outp(adc->nAddrs,lsn);
	Outp(adc->nAddrs,msn);
	Outp(adc->nAddrs,adc->nMemoryWindowSize);
	Outp(adc->nAddrs,0xff);
	m_bAdcFifoWait=FALSE;
}

void CUSLut::Adc200Command11(struct ADC200Data *adc)
{
	
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC200) return;

	Adc200CheckTimeSlot(adc);
	while(m_bAdcFifoWait) {};
	m_bAdcFifoWait=TRUE;
	Outp(adc->nAddrs,0x8b);
	Outp(adc->nAddrs,0xff);
	m_bAdcFifoWait=FALSE;
}

void CUSLut::Adc200CheckTimeSlot(ADC200Data *pAdc)
{

	if(m_nLastAdcTimeSlot - pAdc->nTimeslot) {
		Adc200Command3(pAdc,0);
	}
}

///////////////////////////////////////////////////////////////////////////////////
//PR15 Pulser Receiver
void CUSLut::Pr15Defaults(struct Pr15Data *pr15)
{
int	ii;

memset(pr15,0,sizeof (struct Pr15Data));
pr15->nMuxPulseVoltage = 60;
pr15->nAddrs = 0x89;						//1
pr15->nPreampGain = 4;					//0dB
pr15->nDamping = 0;						//0-3
pr15->nTxRxMode=0;						//pulse echo
pr15->nPulseWidth=20;						//10
pr15->nPulserOffOn=1;					//on
pr15->nHtOffOn=0;									//off
pr15->nPrfSrc=1;							//internal
pr15->ts0_reset_src=0;					//internal
pr15->mux_state=0;						//Halt
pr15->nMuxMode=0;							//non mux mode
pr15->nPrf=1000;							//1.2kHz
pr15->nMainGain=0;						//-27.5dB
pr15->main_op=0;							//rf
pr15->monitor_op=0;						//rf
pr15->nFilterRange=0x00;            //out		
pr15->nFilterCenter=15;					//63 - 15
pr15->rf_low_pass=3;						//15.0MHz
pr15->rectify_filter=3;					//10.0MHz 
pr15->tvg_trig_source=0;				//internal
pr15->tvg_enable=0;						//disabled
pr15->tvg_clk=0;							//1uS / point
for(ii=0;ii<64;ii++) pr15->tvg[ii]=0;
pr15->tvg_trig_mode=0;					//main bang trigger
pr15->tvg_slope=0;						//positive only
pr15->tvg_interface_threshold=0;		//100mV
pr15->tvg_cursor=0;						//off
pr15->tvg_blanking=1;					//1.33uS with 12MHz clock
pr15->tvg_cursor_position=0;
pr15->nMuxSyncPulse1=0;
pr15->nMuxSyncPulse2=0;
pr15->sync_pulse_mode=0x17;
//mux_pulse_voltage;        0-255 
}
void CUSLut::Pr15Initialize(struct Pr15Data *pr15)
{

Pr15Command18(pr15);
Pr15Command0(pr15);
Pr15Command1(pr15);
Pr15Command3(pr15);
Pr15Command2(pr15);
Pr15Command4(pr15);
Pr15Command6(pr15);
Pr15Command7(pr15);

Pr15Command8(pr15);
Pr15Command9(pr15);

Pr15Command10(pr15);

Pr15Command11(pr15);

Pr15Command14(pr15);
Pr15Command15(pr15);
Pr15Command19(pr15);
Pr15Command21(pr15);
Pr15Command4(pr15);

 }

void CUSLut::Pr15NonPreampInitialize(struct Pr15Data *pr15)
{

Pr15Command3(pr15);
Pr15Command4(pr15);
Pr15Command6(pr15);
Pr15Command7(pr15);
Pr15Command8(pr15);
Pr15Command9(pr15);
Pr15Command10(pr15);
Pr15Command11(pr15);
Pr15Command14(pr15);
Pr15Command15(pr15);
Pr15Command19(pr15);
Pr15Command21(pr15);
Pr15Command4(pr15);
}



void CUSLut::Pr15TxRxMode(struct	Pr15Data	*pr15)
{

Pr15Command0(pr15);
}


void CUSLut::Pr15Damping(struct	Pr15Data	*pr15)
{

Pr15Command0(pr15);
}


void CUSLut::Pr15PreampGain(struct	Pr15Data	*pr15)
{

Pr15Command0(pr15);
}

void CUSLut::Pr15PulseWidth(struct	Pr15Data	*pr15)
{

Pr15Command1(pr15);
}


void CUSLut::Pr15Prf(struct	Pr15Data	*pr15)
{

Pr15Command2(pr15);
}


void CUSLut::Pr15PrfSrc(struct	Pr15Data	*pr15)
{

Pr15Command3(pr15);
}


void CUSLut::Pr15PulseVoltage(struct	Pr15Data	*pr15)
{

Pr15Command3(pr15);
}

void CUSLut::Pr15Pulser(struct	Pr15Data	*pr15)
{

Pr15Command3(pr15);
}


void CUSLut::Pr15Gain(struct	Pr15Data	*pr15)
{

Pr15Command4(pr15);
}

void CUSLut::Pr15Output(struct Pr15Data *pr15)
{

Pr15Command4(pr15);
}


void CUSLut::Pr15Filters(struct Pr15Data *pr15)
{

Pr15Command6(pr15);
Pr15Command7(pr15);
}

/*
void CUSLut::ScopeTrigger(struct Pr15Data *pr15)
{
	pr15->sync_pulse_mode = 0x28;
	Pr15SyncPulseMode(pr15);
	Pr15Timeslot(pr15);
	Pr15SyncPulses(pr15);

}
*/







void CUSLut::Pr15LowPass(struct	Pr15Data	*pr15)
{

Pr15Command8(pr15);
}


void CUSLut::Pr15Rectifier(struct	Pr15Data	*pr15)
{

Pr15Command8(pr15);
}



void CUSLut::Pr15TvgCommands(struct	Pr15Data	*pr15)
{

Pr15Command3(pr15);
Pr15Command9(pr15);
//Pr15Command11(pr15);
Pr15Command14(pr15);
Pr15Command15(pr15);
}

void CUSLut::Pr15TvgData(struct	Pr15Data	*pr15)
{

Pr15Command10(pr15);
}


void CUSLut::Pr15Ts0ResetSource(struct Pr15Data *pr15)
{
Pr15Command11(pr15);
}



void CUSLut::Pr15TvgSinglePoint(struct	Pr15Data *pr15,int ptr)
{

MailboxTx(pr15->nAddrs);
MailboxTx(0x0a);
MailboxTx(0);
MailboxTx(ptr);
MailboxTx(pr15->tvg[ptr] & 0x3f);
MailboxTx((pr15->tvg[ptr] >> 6) & 0x03);
MailboxTx(0xff);
}




void CUSLut::Pr15MuxReset(struct Pr15Data *pr15)
{

pr15->mux_max_timeslot = pr15->nMuxTimeslot;
pr15->nMuxReset=1;
Pr15Command17(pr15);
pr15->nMuxReset=0;
}

void CUSLut::Pr15Timeslot(struct Pr15Data *pr15)
{
Pr15Command17(pr15);
}

void CUSLut::Pr15MuxTxRx(struct Pr15Data *pr15)
{
Pr15Command18(pr15);
}

void CUSLut::Pr15SyncPulses(struct	Pr15Data	*pr15)
{
Pr15Command19(pr15);
}


void CUSLut::Pr15SyncPulseMode(struct Pr15Data *pr15)
{
Pr15Command21(pr15);
}


void CUSLut::Pr15MuxHt(struct Pr15Data *pr15)
{
Pr15Command20(pr15);
}


void CUSLut::Pr15MuxState(struct Pr15Data *pr15)
{
Pr15Command3(pr15);
}

void CUSLut::Pr15PulseVoltageMSC(struct Pr15Data *pr15)
{
	//msc->nAddrs
	//nValue = 0-255 = Voltage = 0-600;
	int nValue = (pr15->pulse_voltage * 255) / 600;
	Outp(0x10+0x0c,nValue);

}

void CUSLut::Pr15SelfTestOnOff(Pr15Data *pPr15)
{

	Pr15Command11(pPr15);
}

void CUSLut::Pr15SelfTestFrequency(Pr15Data *pPr15)
{
	Pr15Command32(pPr15);

}

void CUSLut::Pr15SelfTestAmplitude(Pr15Data *pPr15)
{
	Pr15Command33(pPr15);

}


void CUSLut::Pr15Command0(struct	Pr15Data	*pr15)
{
int	value;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;


value = pr15->nTxRxMode & 1;
value |= ((pr15->nDamping & 0x03) << 1);
value |= ((pr15->nPreampGain & 0x07) << 3);
MailboxTx(pr15->nAddrs);
MailboxTx(0x00);
MailboxTx(value);
MailboxTx(0xff);
}

void CUSLut::Pr15Command1(struct	Pr15Data	*pr15)
{
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

MailboxTx(pr15->nAddrs);
MailboxTx(0x01);
MailboxTx(pr15->nPulseWidth & 0x3f);
MailboxTx((pr15->nPulseWidth >> 6) & 0x03);
MailboxTx(0xff);
}

void CUSLut::Pr15Command2(struct	Pr15Data	*pr15)
{
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

MailboxTx(pr15->nAddrs);
MailboxTx(0x02);
MailboxTx(pr15->nPrf & 0x3f);
MailboxTx((pr15->nPrf >> 6) & 0x3f);
MailboxTx((pr15->nPrf >> 12) & 0x0f);
MailboxTx(0xff);
}


void CUSLut::Pr15Command3(struct	Pr15Data	*pr15)
{
int	value;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

pr15->nPrfSrc=0;
value = pr15->nPulserOffOn & 1;
value |= ((pr15->nHtOffOn & 1) << 1);
value |= ((pr15->nPrfSrc & 1) << 2);
value |= ((pr15->mux_state & 1) << 3);
value |= ((pr15->tvg_cursor & 1) << 4);
value |= ((pr15->tvg_slope & 1) << 5);

MailboxTx(pr15->nAddrs);
MailboxTx(0x03);
MailboxTx(value);
MailboxTx(0xff);
}

void CUSLut::Pr15Command4(struct	Pr15Data	*pr15)
{
int	value;
int nGain = pr15->nMainGain ^ 0x00;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

value = (nGain >> 6) & 3;
value |= ((pr15->monitor_op & 1) << 2);
value |= ((pr15->main_op & 1) << 3);
MailboxTx(pr15->nAddrs);
MailboxTx(0x04);
MailboxTx(nGain & 0x3f);
MailboxTx(value);
MailboxTx(0xff);
}

void CUSLut::Pr15Command6(struct	Pr15Data	*pr15)
{
	int nValue[9]={0,0x01,0x03,0x05,0x07,0x09,0x0B,0x0D,0x0F};
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

	MailboxTx(pr15->nAddrs);
	MailboxTx(0x06);
	MailboxTx(nValue[pr15->nFilterRange]);
	MailboxTx(0xff);

}

void CUSLut::Pr15Command7(struct	Pr15Data	*pr15)
{
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

MailboxTx(pr15->nAddrs);
MailboxTx(0x07);
MailboxTx(pr15->nFilterCenter & 0x3f);
MailboxTx(0xff);
}


void CUSLut::Pr15Command8(struct	Pr15Data *pr15)
{
int	value;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

value = pr15->rf_low_pass & 0x03;
value |= ((pr15->rectify_filter & 0x03) << 3);
MailboxTx(pr15->nAddrs);
MailboxTx(0x08);
MailboxTx(value);
MailboxTx(0xff);
}




void CUSLut::Pr15Command9(struct	Pr15Data	*pr15)
{
int	value;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

value = pr15->tvg_clk & 0x07;
value |= ((pr15->tvg_enable & 1) << 3);

MailboxTx(pr15->nAddrs);
MailboxTx(9);
MailboxTx(value);
MailboxTx(0xff);
}



void CUSLut::Pr15Command10(struct	Pr15Data	*pr15)
{
int	loop,ii;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

for(loop=0;loop<64;loop+=8) {
	MailboxTx(pr15->nAddrs);
	MailboxTx(0x0a);
	MailboxTx(7);
	MailboxTx(loop);
	for(ii=loop;ii<(loop+8);ii++) {
		MailboxTx(pr15->tvg[ii] & 0x3f);
		MailboxTx((pr15->tvg[ii] >> 6) & 0x03);
	};
	MailboxTx(0xff);
};
}


void CUSLut::Pr15Command11(struct	Pr15Data	*pr15)
{
int	value=0;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

	if(theApp.m_LastSettings.nPr15Version<=0) return;

value = pr15->ts0_reset_src & 1;
value |= ((pr15->prf_out_polarity & 1) << 1);
value |= ((pr15->tvg_shape & 1) << 2);
if(pr15->bSelfTest==TRUE) value |= 8;
pr15->nMuxMode=0;
value |= (pr15->nMuxMode << 5);

MailboxTx(pr15->nAddrs);
MailboxTx(0x0b);
MailboxTx(value);
MailboxTx(0xff);
}



void CUSLut::Pr15Command14(struct	Pr15Data	*pr15)
{
int	value;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

value = ((pr15->tvg_interface_threshold >> 6) & 0x03);
value |= ((pr15->tvg_trig_mode & 1) << 2);
//value |= ((pr15->tvtheApp.m_slope & 1) << 3);
//value |= ((pr15->tvtheApp.m_cursor & 1) << 4);
MailboxTx(pr15->nAddrs);
MailboxTx(0x0e);
MailboxTx(pr15->tvg_interface_threshold & 0x3f);
MailboxTx(value);
MailboxTx(0xff);
}


void CUSLut::Pr15Command15(struct	Pr15Data	*pr15)
{
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;
MailboxTx(pr15->nAddrs);
MailboxTx(0x0f);
MailboxTx(pr15->tvg_blanking & 0x3f);
MailboxTx((pr15->tvg_blanking >> 6) & 0x3);
MailboxTx(0xff);
}

void CUSLut::Pr15Command17(struct	Pr15Data *pr15)
{
int	value;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

value = (pr15->nMuxReset&1) << 3;
value |= (pr15->nMuxTimeslot & 0x07);
MailboxTx(pr15->nAddrs);
MailboxTx(0x11);
MailboxTx(value);
MailboxTx(0xff);
}


void CUSLut::Pr15Command18(struct	Pr15Data *pr15)
{
int	value;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

value = (pr15->nMuxRx & 7) << 3;
value |= (pr15->nMuxTx & 0x07);
MailboxTx(pr15->nAddrs);
MailboxTx(0x12);
MailboxTx(value);
MailboxTx(0xff);
}

void CUSLut::Pr15Command19(struct	Pr15Data *pr15)
{
int	value;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

value = (pr15->nMuxSyncPulse2 & 7) << 3;
value |= (pr15->nMuxSyncPulse1 & 0x07);
MailboxTx(pr15->nAddrs);
MailboxTx(0x13);
MailboxTx(value);
MailboxTx(0xff);
}


void CUSLut::Pr15Command20(struct	Pr15Data *pr15)
{
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;
MailboxTx(pr15->nAddrs);
MailboxTx(0x14);
MailboxTx(pr15->nMuxPulseVoltage & 0x3f);
MailboxTx((pr15->nMuxPulseVoltage >>6 ) & 3);
MailboxTx(0xff);
}



void CUSLut::Pr15Command21(struct	Pr15Data *pr15)
{
int	value;
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

value = pr15->sync_pulse_mode;
MailboxTx(pr15->nAddrs);
MailboxTx(0x15);
MailboxTx(value);
MailboxTx(0xff);
}

void CUSLut::Pr15Command32(struct	Pr15Data *pr15)
{
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

MailboxTx(pr15->nAddrs);
MailboxTx(0x20);
MailboxTx(pr15->nSelfTestFrequency & 0x3f);
MailboxTx((pr15->nSelfTestFrequency >>6 ) & 0x3f);
MailboxTx(0xff);
}


void CUSLut::Pr15Command33(struct	Pr15Data *pr15)
{
	
	if(theApp.m_LastSettings.bPr15Present!= TRUE) return;

MailboxTx(pr15->nAddrs);
MailboxTx(0x21);
MailboxTx(pr15->nSelfTestAmplitude & 0x3f);
MailboxTx((pr15->nSelfTestAmplitude >>6 ) & 0x3);
MailboxTx(0xff);
}

void CUSLut::Pr15GetLowPassFilter(Pr15Data *pPr15, CString *pBuff,int nVerbose, int nLanguage)
{
	CString Code[7]={L"3.5MHz",L"5.0MHz",L"10 MHz",L"15 MHz"};

	if(nVerbose==0) {
		pBuff->Format(_T("%s"),Code[pPr15->rf_low_pass]);
	} else {
		pBuff->Format(_T("Low Pass %s"),Code[pPr15->rf_low_pass]);
	}
}
void CUSLut::Pr15GetPulseWidth(Pr15Data *pPr15, CString *pBuff,int nVerbose, int nLanguage)
{
	if(nVerbose==0) {
		pBuff->Format(_T("%d ns"),pPr15->nPulseWidth);
	} else {
		pBuff->Format(_T("Pulse Width %d ns"),pPr15->nPulseWidth);
	}
}

void CUSLut::Pr15GetPulseVoltage(Pr15Data *pPr15, CString *pBuff,int nVerbose, int nLanguage)
{

	if(nVerbose==0) {
		pBuff->Format(_T("%d V"),pPr15->pulse_voltage);
	} else {
		pBuff->Format(_T("Pulse voltage %d V"),pPr15->pulse_voltage);
	}
}

void CUSLut::Pr15GetDamping(Pr15Data *pPr15, CString *pBuff,int nVerbose, int nLanguage)
{
	int nCode[4]={200,100,50,33};

	if(nVerbose==0) {
		pBuff->Format(_T("%d ohms"),nCode[pPr15->nDamping]);
	}else {
		pBuff->Format(_T("Damping %d ohms"),nCode[pPr15->nDamping]);
	}
}

void CUSLut::Pr15GetPreampGain(Pr15Data *pPr15, CString *pBuff,int nVerbose, int nLanguage)
{
	int nCode[7]={-40,-30,-20,-10,0,10,20};

	if(nVerbose==0) {
		pBuff->Format(_T("%d dB"),nCode[pPr15->nPreampGain]);
	} else {
		pBuff->Format(_T("Preamp Gain %d dB"),nCode[pPr15->nPreampGain]);
	}
}

void CUSLut::Pr15GetMainGain(Pr15Data *pPr15, CString *pBuff,int nVerbose, int nLanguage)
{

	if(nVerbose==0) {
		pBuff->Format(_T("%.01fdB"),((double)pPr15->nMainGain-56)/2.0);
	} else {
		pBuff->Format(_T("Main Gain %.01fdB"),((double)pPr15->nMainGain-56)/2.0);
	}

}

void CUSLut::Pr15GetFilterRange(Pr15Data *pPr15, CString *pBuff,int nVerbose, int nLanguage)
{
	CString Code[7]={L"Wide Band",L"0.5-2MHz",L"1.0-4.0MHz",L"2.0-8.0MHz",L"4.0-15MHz"};

	if(nVerbose==0) {
		pBuff->Format(_T("%s"),Code[pPr15->nFilterRange]);
	}else {
		pBuff->Format(_T("Filter Range %s"),Code[pPr15->nFilterRange]);
	}
}

void CUSLut::Pr15GetCenterFrequency(Pr15Data *pPr15, CString *pBuff,int nVerbose, int nLanguage)
{

	if(nVerbose==0) {
		pBuff->Format(_T("%d"),pPr15->nFilterCenter);
	} else {
		pBuff->Format(_T("Center Frequency %d"),pPr15->nFilterCenter);
	}
}



//Conversion Functions/////////////////////////////////
int CUSLut::Pr15From_dB_To_Hardware(double GaindB)
{
	int Value = (int)((GaindB *2.0) + 56.0);
	if(Value<0) Value=0;
	if(Value > 255) Value=255;
	return Value;
}

double CUSLut::Pr15From_Hardware_To_dB(int GainHardware)
{
	double Value = (double)(GainHardware-56) / 2.0;
	return Value;
}


double CUSLut::Pl15From_Hardware_To_Volt(int VoltHardware)
{
	double Value = VoltHardware * 5;
	return Value;
}
int CUSLut::Pl15From_Volt_To_Hardware(double Volt)
{
	int Value = (int)(Volt / 5);
	return Value;
}


////////////////////////////////////////////////////////////////////////
//PL15 Log pulser receiver
int	log15_gain[51][2]={
{0,253},{0,228},{0,203},{0,178},{0,153},
{0,128},{0,103},{0,78},{0,53},{0,28},
{1,253},{1,228},{1,203},{1,178},{1,153},
{1,128},{1,103},{1,78},{1,53},{1,28},
{2,253},{2,228},{2,203},{2,178},{2,153},
{2,128},{2,103},{2,78},{2,53},{2,28},
{3,253},{3,228},{3,203},{3,178},{3,153},
{3,128},{3,103},{3,78},{3,53},{3,28},
{4,253},{4,228},{4,203},{4,178},{4,153},
{4,128},{4,103},{4,78},{4,53},{4,28},{4,3}};


void CUSLut::Pl15Defaults(struct Pl15Data *pl15)
{

pl15->nAddrs = 0x91;						//1
pl15->nPreampGain = 4;					//0dB
pl15->nDamping = 0;						//200R
pl15->nTxRxMode=0;						//pulse echo
pl15->nPulseWidth=100;							//10
pl15->nPulserOffOn=1;					//on
pl15->nHtOffOn=1;									//on
pl15->nPrfSrc=1;							//internal
pl15->mux_state=0;						//Halt
pl15->nMuxMode=0;							//non mux mode
pl15->prf=1000;							//1.2kHz
pl15->nMainGain=26;						//0dB
pl15->nMainOutput=0;							//rf
pl15->nInternalExternalRfSrc=0;						//rf
pl15->nFilterRange=0x00;            //out
pl15->nFilterCenter=15;					//63 - 15
pl15->rf_low_pass=3;						//15.0MHz
pl15->detector=3;							//10.0MHz
pl15->nMuxTimeslot=0;					//0
pl15->nMuxTx=0;
pl15->nMuxRx=0;
pl15->mux_reset = 0;						//no reset
pl15->nMuxPulseVoltage = 120;
pl15->ts0_reset_src = 0;				//internal
}


void CUSLut::Pl15Initialize(struct Pl15Data *pl15)
{

	Pl15Command18(pl15);
	Pl15Command0(pl15);
	Pl15Command1(pl15);
	Pl15Command3(pl15);
	Pl15Command2(pl15);
	Pl15Command4(pl15);
	Pl15Command5(pl15);
	Pl15Command6(pl15);
	Pl15Command7(pl15);
	Pl15Command8(pl15);
	Pl15Command11(pl15);
	Pl15Command17(pl15);
	Pl15Command20(pl15);

}

void CUSLut::Pl15NonPreampInitialize(struct Pl15Data *pl15)
{

	Pl15Command3(pl15);
	Pl15Command4(pl15);
	Pl15Command6(pl15);
	Pl15Command7(pl15);
	Pl15Command8(pl15);
	Pl15Command11(pl15);
	Pl15Command19(pl15);
	Pl15Command4(pl15);
}



void CUSLut::Pl15TxRxMode(struct	Pl15Data	*pl15)
{

	Pl15Command0(pl15);
}


void CUSLut::Pl15Damping(struct	Pl15Data	*pl15)
{

	Pl15Command0(pl15);
}


void CUSLut::Pl15PreampGain(struct	Pl15Data	*pl15)
{

	Pl15Command0(pl15);
}

void CUSLut::Pl15PulseWidth(struct	Pl15Data	*pl15)
{

	Pl15Command1(pl15);
}


void CUSLut::Pl15prf(struct	Pl15Data	*pl15)
{

	Pl15Command2(pl15);
}


void CUSLut::Pl15PrfSrc(struct	Pl15Data	*pl15)
{

	Pl15Command3(pl15);
}


void CUSLut::Pl15PulseVoltage(struct	Pl15Data	*pl15)
{

	Pl15Command3(pl15);
}

void CUSLut::Pl15Pulser(struct	Pl15Data	*pl15)
{

	Pl15Command3(pl15);
}


void CUSLut::Pl15Gain(struct	Pl15Data	*pl15)
{

	Pl15Command4(pl15);
	Pl15Command5(pl15);
}

void CUSLut::Pl15Output(struct Pl15Data *pl15)
{

	Pl15Command4(pl15);
}


void CUSLut::Pl15Filters(struct Pl15Data *pl15)
{

	Pl15Command6(pl15);
	Pl15Command7(pl15);
}






void CUSLut::Pl15LowPass(struct	Pl15Data	*pl15)
{

	Pl15Command8(pl15);
}


void CUSLut::Pl15Detector(struct	Pl15Data	*pl15)
{

	Pl15Command8(pl15);
}

void CUSLut::Pl15Ts0ResetSource(struct Pl15Data *pl15)
{

	Pl15Command11(pl15);
}


void CUSLut::Pl15MuxReset(struct Pl15Data *pl15)
{
	pl15->nMaxTimeslot = pl15->nMuxTimeslot;
	pl15->mux_reset=1;
	Pl15Command17(pl15);
	pl15->mux_reset=0;
}

void CUSLut::Pl15Timeslot(struct Pl15Data *pl15)
{
	Pl15Command17(pl15);
}

void CUSLut::Pl15MuxTxRx(struct Pl15Data *pl15)
{

	Pl15Command18(pl15);
}

void CUSLut::Pl15SyncPulses(struct	Pl15Data	*pl15)
{

	Pl15Command19(pl15);
}


void CUSLut::Pl15MuxHt(struct Pl15Data *pl15)
{

	Pl15Command20(pl15);
}


void CUSLut::Pl15MuxState(struct Pl15Data *pl15)
{

	Pl15Command3(pl15);
}


void CUSLut::Pl15GetPulseWidth(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage)
{
	if(nVerbose==0) {
		pBuff->Format(_T("%d ns"),pPl15->nPulseWidth);
	} else {
		pBuff->Format(_T("Pulse Width %d ns"),pPl15->nPulseWidth);
	}
}

void CUSLut::Pl15GetPulseVoltage(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage)
{

	if(nVerbose==0) {
		pBuff->Format(_T("%d V"),pPl15->nMuxPulseVoltage);
	} else {
		pBuff->Format(_T("Pulse voltage %d V"),pPl15->nMuxPulseVoltage);
	}
}

void CUSLut::Pl15GetDamping(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage)
{
	int nCode[4]={200,100,50,33};

	if(nVerbose==0) {
		pBuff->Format(_T("%d ohms"),nCode[pPl15->nDamping]);
	}else {
		pBuff->Format(_T("Damping %d ohms"),nCode[pPl15->nDamping]);
	}
}

void CUSLut::Pl15GetPreampGain(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage)
{
	int nCode[7]={-40,-30,-20,-10,0,10,20};

	if(nVerbose==0) {
		pBuff->Format(_T("%d dB"),nCode[pPl15->nPreampGain]);
	} else {
		pBuff->Format(_T("Preamp Gain %d dB"),nCode[pPl15->nPreampGain]);
	}
}

void CUSLut::Pl15GetMainGain(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage)
{

	if(nVerbose==0) {
		pBuff->Format(_T("%.01fdB"),((double)pPl15->nMainGain-56)/2.0);
	} else {
		pBuff->Format(_T("Main Gain %.01fdB"),((double)pPl15->nMainGain-56)/2.0);
	}

}

void CUSLut::Pl15GetFilterRange(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage)
{
	CString Code[7]={L"Wide Band",L"0.5-2MHz",L"1.0-4.0MHz",L"2.0-8.0MHz",L"4.0-15MHz"};

	if(nVerbose==0) {
		pBuff->Format(_T("%s"),Code[pPl15->nFilterRange]);
	}else {
		pBuff->Format(_T("Filter Range %s"),Code[pPl15->nFilterRange]);
	}
}

void CUSLut::Pl15GetCenterFrequency(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage)
{

	if(nVerbose==0) {
		pBuff->Format(_T("%d"),pPl15->nFilterCenter);
	} else {
		pBuff->Format(_T("Center Frequency %d"),pPl15->nFilterCenter);
	}
}

void CUSLut::Pl15GetLowPassFilter(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage)
{
	CString Code[7]={L"3.5MHz",L"5.0MHz",L"10 MHz",L"15 MHz"};

	if(nVerbose==0) {
		pBuff->Format(_T("%s"),Code[pPl15->rf_low_pass]);
	} else {
		pBuff->Format(_T("Low Pass %s"),Code[pPl15->rf_low_pass]);
	}
}



void CUSLut::Pl15Command0(struct	Pl15Data	*pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	int	value;

	value = pl15->nTxRxMode;
	value |= ((pl15->nDamping & 0x03) << 1);
	value |= ((pl15->nPreampGain & 0x07) << 3);
	MailboxTx(pl15->nAddrs);
	MailboxTx(0x00);
	MailboxTx(value);
	MailboxTx(0xff);

}

void CUSLut::Pl15Command1(struct	Pl15Data	*pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;

	MailboxTx(pl15->nAddrs);
	MailboxTx(0x01);
	MailboxTx(pl15->nPulseWidth & 0x3f);
	MailboxTx((pl15->nPulseWidth >> 6) & 0x03);
	MailboxTx(0xff);
}

void CUSLut::Pl15Command2(struct	Pl15Data	*pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;

	MailboxTx(pl15->nAddrs);
	MailboxTx(0x02);
	MailboxTx(pl15->prf & 0x3f);
	MailboxTx((pl15->prf >> 6) & 0x3f);
	MailboxTx((pl15->prf >> 12) & 0x0f);
	MailboxTx(0xff);
}


void CUSLut::Pl15Command3(struct	Pl15Data	*pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	int	value;

	value = pl15->nPulserOffOn & 1;
	value |= ((pl15->nHtOffOn & 1) << 1);
	value |= ((pl15->nPrfSrc & 1) << 2);
	value |= ((pl15->mux_state & 1) << 3);
	MailboxTx(pl15->nAddrs);
	MailboxTx(0x03);
	MailboxTx(value);
	MailboxTx(0xff);
}

void CUSLut::Pl15Command4(struct	Pl15Data	*pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	int	value;

	pl15->nInternalExternalRfSrc=0;

	value = pl15->nMainGain;
	if(value<0) value=0;
	if(value>51) value=51;
	value = log15_gain[value][0] & 0x07;
	value |= ((pl15->nInternalExternalRfSrc & 1) << 3);
	value |= ((pl15->nMainOutput & 1) << 4);
	MailboxTx(pl15->nAddrs);
	MailboxTx(0x04);
	MailboxTx(value);
	MailboxTx(0xff);
}


void CUSLut::Pl15Command5(struct	Pl15Data	*pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	int	value;

	value = pl15->nMainGain;
	if(value<0) value=0;
	if(value>51) value=51;
	value = log15_gain[value][1];

	MailboxTx(pl15->nAddrs);
	MailboxTx(0x05);
	MailboxTx(value & 0x3f);
	MailboxTx((value>>6)&0x03);
	MailboxTx(0xff);
}


void CUSLut::Pl15Command6(struct	Pl15Data	*pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	int nValue[9]={0,0x01,0x03,0x05,0x07,0x09,0x0B,0x0D,0x0F};

	MailboxTx(pl15->nAddrs);
	MailboxTx(0x06);
	MailboxTx(nValue[pl15->nFilterRange]);
	MailboxTx(0xff);
}

void CUSLut::Pl15Command7(struct	Pl15Data	*pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;

	MailboxTx(pl15->nAddrs);
	MailboxTx(0x07);
	MailboxTx(pl15->nFilterCenter & 0x3f);
	MailboxTx(0xff);
}


void CUSLut::Pl15Command8(struct	Pl15Data *pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	int	value;

	value = pl15->rf_low_pass & 0x03;
	value |= ((pl15->detector & 0x03) << 3);
	MailboxTx(pl15->nAddrs);
	MailboxTx(0x08);
	MailboxTx(value);
	MailboxTx(0xff);
}


void CUSLut::Pl15Command11(struct	Pl15Data	*pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	int	value;

	if(theApp.m_LastSettings.nPl15Version<=0) return;

	value = pl15->ts0_reset_src & 1;
	value |= ((pl15->prf_out_polarity & 1) << 1);
	value |= ((pl15->tvg_shape & 1) << 2);
	value |= ((pl15->nMuxMode & 1) << 5);

	MailboxTx(pl15->nAddrs);
	MailboxTx(0x0b);
	MailboxTx(value);
	MailboxTx(0xff);
}


void CUSLut::Pl15Command17(struct	Pl15Data *pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	int	value;

	value = (pl15->mux_reset&1) << 3;
	value |= (pl15->nMaxTimeslot & 0x07);
	MailboxTx(pl15->nAddrs);
	MailboxTx(0x11);
	MailboxTx(value);
	MailboxTx(0xff);
}


void CUSLut::Pl15Command18(struct	Pl15Data *pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	int	value;

	value = (pl15->nMuxRx & 7) << 3;
	value |= (pl15->nMuxTx & 0x07);
	MailboxTx(pl15->nAddrs);
	MailboxTx(0x12);
	MailboxTx(value);
	MailboxTx(0xff);
}

void CUSLut::Pl15Command19(struct	Pl15Data *pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	int	value;

	value = (pl15->mux_sync_pulse_2 & 7) << 3;
	value |= (pl15->mux_sync_pulse_1 & 0x07);
	MailboxTx(pl15->nAddrs);
	MailboxTx(0x13);
	MailboxTx(value);
	MailboxTx(0xff);
}


void CUSLut::Pl15Command20(struct	Pl15Data *pl15)
{
	
	if(theApp.m_LastSettings.bPl15Present!= TRUE) return;
	MailboxTx(pl15->nAddrs);
	MailboxTx(0x14);
	MailboxTx(pl15->nMuxPulseVoltage & 0x3f);
	MailboxTx((pl15->nMuxPulseVoltage >>6 ) & 3);
	MailboxTx(0xff);
}

int CUSLut::Pl15From_dB_To_Hardware(double GaindB)
{
	int Value = (int)(GaindB + 25);
	if(Value<0) Value=0;
	if(Value>51) Value = 51;
	return Value;
	}

double CUSLut::Pl15From_Hardware_To_dB(int GainHardware)
{
	double Value = (double)GainHardware -25;
	return Value;
}
/////////////////////////////////////////////////////////////////////////
void CUSLut::MuxDefaults()
{
	m_Global.Mux.nMaxTimeslots=1;
}


////////////////////////////////////////////////////////////////////////
// MAILBOX
void CUSLut::ResetMailbox()
{
	

	switch(m_Pm30CommDevice) {
	case SI10_I2C:
		break;
	case USB_I2C:
		theApp.m_USBtoI2C.Reset();
		break;
	case USLDRIVER_I2C:
		Outp(MailboxAddrs+1,0x02);
		break;
	}
}



int CUSLut::MailboxTx(int	value)
{

	if (theApp.m_PM40User.IsConnected()) {
		return theApp.m_PM40User.MailboxTx(value, &m_nCheckSum);
	}


int timeout=32000,tt;

CString	Buff;
int MailboxFull;

if(m_Pm30CommDevice==NONE) return FALSE;


//ASSERT(value!=0x89);
//ASSERT(value!=0x91);
if(m_nMailboxFailed>0) {
	if(!theApp.m_LastSettings.bMscPresent) //If we have a MSc we may not have a mailbox
		MessageBox(NULL,_T("Mailbox has failed"),_T("Error"),NULL);
	m_nMailboxFailed = -1;
}
if(m_nMailboxFailed==-1) return FALSE;

tt=0;

switch(m_Pm30CommDevice) {
case SI10_I2C:
	MailboxFull = theApp.m_Si10User.InpW(0x03C4) & 0x03;
	break;
case USB_I2C:
	MailboxFull = 0;
	break;
case USLDRIVER_I2C:
	MailboxFull = Inp(MailboxAddrs+1)&0x02;
	break;
}//switch

while(MailboxFull) {
	tt++;
	if(tt>MaxMailboxWait) MaxMailboxWait=tt;
	if(tt>=timeout) {
		m_nMailboxFailed++;
		return FALSE;
	};

	if(theApp.m_Si10User.IsConnected()) {
		MailboxFull = theApp.m_Si10User.InpW(0x03C4) & 0x02;
	} else {
		MailboxFull = Inp(MailboxAddrs+1)&0x02;
	}
};
MailboxCount++;


switch(m_Pm30CommDevice) {
case SI10_I2C:
	theApp.m_Si10User.OutpW(0x03C0,value);
	break;
case USB_I2C:
	theApp.m_USBtoI2C.MailboxTx(value);
	break;
case USLDRIVER_I2C:
	Outp(MailboxAddrs,value);
	break;
}//sw

if(value & 0x80) {
	m_nCheckSum=value;
//	Buff.Format(_T(" \r\n");
//	sprintf_s(str,"%s",Buff);
//	m_file.Write(str,strlen(str));
} else {
	m_nCheckSum+=value;
//	Buff.Format(_T("%02x %03d\r\n",value,value);
//	sprintf_s(str,"%s",Buff);
//	m_file.Write(str,strlen(str));
}

//Sleep(5);
return TRUE;
}

unsigned int CUSLut::MailboxRx(PM30_RETURN_MSG *pPm30Msg)
{
	
	int timeout=100,tt=0;
	int MailboxFull;
	bool bRead=true;

	if(!(theApp.m_Si10User.IsConnected())) //only supported on Si10 at present
		return 0;

	if(m_nMailboxRxFailed>0) {
		MessageBox(NULL,_T("Mailbox receive has failed"),_T("Error"),NULL);
		m_nMailboxRxFailed = -1;
	}
	if(m_nMailboxRxFailed==-1) return FALSE;


	pPm30Msg->nBytes=0;

	while(bRead){

		MailboxFull = theApp.m_Si10User.InpW(0x03C4) & 0x01;

		if(MailboxFull==0) {//no byte
			tt++;
			Sleep(1);
			if(tt>=timeout) {
				bRead=false;
			};
		}else{
			tt=0;
		
			if(pPm30Msg->nBytes<50)
				pPm30Msg->data[pPm30Msg->nBytes] = theApp.m_Si10User.InpW(0x03C0) & 0xFF;
		
			if(pPm30Msg->data[pPm30Msg->nBytes] == 0xFF)
				return (pPm30Msg->nBytes+1);
		
			pPm30Msg->nBytes++;
		}
	
	}

	return pPm30Msg->nBytes;
}


int CUSLut::MailboxTxCheckSum()
{
	MailboxTx(m_nCheckSum&0x7f);
	return TRUE;
}


void CUSLut::MailboxHtReset()
{
	

	if(theApp.m_Si10User.IsConnected())
		return;

	MailboxTx(0x81);
	MailboxTx(0x0);
	MailboxTx(0x01);
	MailboxTx(0xff);

}

void CUSLut::MailboxHt(bool bEnable,int nVoltage)
{
	if(theApp.m_Si10User.IsConnected())
		return;

	MailboxTx(0x81);
	MailboxTx(0x3);
	(bEnable==FALSE) ? MailboxTx(0):MailboxTx(2);
	MailboxTx(0xff);

	//nValue 0 - 120 == nVoltage = 0 - 600
	int nValue = nVoltage / 5;
	MailboxTx(0x81);
	MailboxTx(0x14);
	MailboxTx(nValue & 0x3f);
	MailboxTx((nValue>>6) & 0x03);
	MailboxTx(0xff);
}

void CUSLut::MailboxPrf(int nValue)
{
	if(theApp.m_Si10User.IsConnected())
		return;

	m_TS[0].Up15.bIsrFlag=FALSE;

	MailboxTx(0x81);
	MailboxTx(0x02);
	MailboxTx(nValue & 0x3f);
	MailboxTx((nValue>>6) & 0x3f);
	MailboxTx((nValue>>12) & 0x0f);
	MailboxTx(0xff);

	Sleep(1);
	Outp(MailboxAddrs+3,1);
//	Outp(MailboxAddrs+3,0);
}

void CUSLut::MailboxPrfLowPeriod()
{
	
return;
	if(!(theApp.m_Si10User.IsConnected())){
		if(theApp.m_LastSettings.nMuxVersion < 2) {
			MailboxTx(0x81);
			MailboxTx(0x43);
			MailboxTx(0x20);
			MailboxTx(0xff);
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////
//General procedures

void CUSLut::SetPrf()
{
	
	int nPrf;
	int nMaxTS;

	if(m_Global.nPrf<=0) return;

	if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]==LSA){
		nMaxTS = m_Global.Mux.nMaxTimeslots;
		m_Global.Mux.nMaxTimeslots=1;
	}


	if((m_nMaxPRF<m_Global.nPrf* m_Global.Mux.nMaxTimeslots) && 
		((theApp.m_LastSettings.nAdcCardType==Alazar_9462) || (theApp.m_LastSettings.nAdcCardType==Alazar_9626) || (theApp.m_LastSettings.nAdcCardType==Alazar_860)) && 	(m_nMaxPRF>0) ){
			nPrf = m_nMaxPRF/m_Global.Mux.nMaxTimeslots;
	}else{
		nPrf = m_Global.nPrf;
	}

	switch(m_Global.nTriggerSource) {
	case 0:
		break;
	case USL_PR20:
		m_TS[m_Global.nTimeSlot].Pr20.nPrf=nPrf;
		Pr2050Prf(&m_TS[m_Global.nTimeSlot].Pr20);
		break;
	case USL_PR30:
		m_TS[m_Global.nTimeSlot].Pr30.nPrf=190000 / nPrf;
		Pr30Prf(&m_TS[m_Global.nTimeSlot].Pr30);
		break;
	case USL_ADC200:
		m_TS[m_Global.nTimeSlot].Adc.prf=nPrf;
		Adc200Prf(&m_TS[m_Global.nTimeSlot].Adc);
		break;
	case USL_DSP200:
		m_TS[m_Global.nTimeSlot].Dsp.Prf=nPrf * m_Global.Mux.nMaxTimeslots;
		Dsp200Prf(&m_TS[m_Global.nTimeSlot].Dsp);
		break;
	case USL_UP15:
		MailboxPrf(m_TS[0].Up15.nPrf=1300000 / (nPrf  * m_Global.Mux.nMaxTimeslots));
		break;
	case USL_SI10:
		theApp.m_Si10User.Si10SetPRF(nPrf * m_Global.Mux.nMaxTimeslots);
		break;
	case USL_PR40:
		m_TS[m_Global.nTimeSlot].Pr30.nPrf = 190000 / nPrf;
		Pr30Prf(&m_TS[m_Global.nTimeSlot].Pr30);
		break;
	};

	if(theApp.m_nJobNumber==VTT_HELSINKY) {
		m_TS[m_Global.nTimeSlot].Pl15.prf=1000000 / nPrf;
		Pl15prf(&m_TS[m_Global.nTimeSlot].Pl15);
	}

	if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]==LSA)
		m_Global.Mux.nMaxTimeslots = nMaxTS;

}


	//fRate = clocks per nanosecond
	//m_nDelay = Delay in Samples
void CUSLut::SetMaxPRF(float fRate , int nDelay)
{	//Only gets called when scope changed
	
	m_nMaxPRF = (int)(1.0f / ( ((fRate*1e-9f)*8192.0f) + ((1e-9f)*(float)nDelay) + (25e-6f) ));
	SetPrf();
}






int CUSLut::InterfacePos(int nTimeslot)
{
	int InterfacePos=(int)(m_TS[nTimeslot].Adc.fSamplePeriod*(float)m_TS[nTimeslot].Dsp.InterfacePos) + m_TS[nTimeslot].Adc.nDelay;
	return InterfacePos;
}


/*bool CUSLut::Adc200CalculateHardware()
{
	return Adc200CalculateHardware(&m_TS[m_Global.nTimeSlot].Adc);
}
*/

void CUSLut::ChangeMaxTimeSlots(int nJobNumber)
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	Dsp200StopProcessing(&m_TS[0].Dsp);
	if(theApp.m_LastSettings.bPl15Present==TRUE)	Pl15MuxReset(&m_TS[0].Pl15);
	if(theApp.m_LastSettings.bPr15Present==TRUE)	Pr15MuxReset(&m_TS[0].Pr15);
	if(theApp.m_LastSettings.bPr30Present[theApp.m_LastSettings.nUseWhichPR30]==TRUE)	Pr30MuxReset(&m_TS[0].Pr30);
	Adc200ResetMaxTimeSlot(&m_TS[0].Adc);
	RefreshAllVariables(nJobNumber);
	Dsp200StartProcessing(&m_TS[0].Dsp);

}


///////////////////////////////////////////////////////////////////////////////////////
void CUSLut::TimsDefaults(struct	AtpTimsData *tims)
{

tims->nAddrs=0x40;
tims->nBlanking=10;
tims->nDelay=20;
tims->nStartThreshold=1000;
tims->nStopThreshold=2000;
tims->rf_select=0;
tims->average_mode=0;					//OFF
tims->nTriggerMode=1;					//PRF CLOCK
tims->nStartSignalSign=0;				//POSITIVE
tims->nStartSignalSlope=0;			//POSITIVE
tims->nStopSignalSign=0;				//POSITIVE
tims->nStopSignalSlope=0;				//POSITIVE
tims->blank_marker=0;					//DISABLE
tims->delay_marker=0;					//DISABLE
tims->period_marker=1;					//ENABLE
tims->averages=1;
tims->divider=2;
}


void CUSLut::TimsControl(struct	AtpTimsData *tims)
{
tims->control_reg0 = 0;
tims->control_reg0 |= (tims->rf_select & 1);
tims->control_reg0 |= ((tims->average_mode & 1)<<1);
tims->control_reg0 |= ((tims->nTriggerMode & 1)<<2);
tims->control_reg0 |= ((tims->nStartSignalSign & 1)<<3);
tims->control_reg0 |= ((tims->nStartSignalSlope & 1)<<4);
tims->control_reg0 |= ((tims->nStopSignalSign & 1)<<5);
tims->control_reg0 |= ((tims->nStopSignalSlope & 1)<<6);
Outp(tims->nAddrs,tims->control_reg0);

tims->control_reg1 = 0;
tims->control_reg1 |= (tims->blank_marker & 1);
tims->control_reg1 |= ((tims->delay_marker & 1)<<1);
tims->control_reg1 |= ((tims->period_marker & 1)<<2);
Outp(tims->nAddrs+1,tims->control_reg1);
}





void CUSLut::TimsBlanking(struct	AtpTimsData *tims)
{

Outpw(tims->nAddrs+2,tims->nBlanking);
}

void CUSLut::TimsDelay(struct	AtpTimsData *tims)
{

Outpw(tims->nAddrs+4,tims->nDelay);
}


void CUSLut::TimsStartThreshold(struct	AtpTimsData *tims)
{

Outpw(tims->nAddrs+0x0a,tims->nStartThreshold);
}

void CUSLut::TimsStopThreshold(struct	AtpTimsData *tims)
{

Outpw(tims->nAddrs+0x08,tims->nStopThreshold);
}

void CUSLut::TimsAverages(struct	AtpTimsData *tims)
{

Outpw(tims->nAddrs+0x06,tims->averages);
}

int CUSLut::TimsTime()
{

	return TimsTime(&m_TS[m_Global.nTimeSlot].Tims[0]);
}

int  CUSLut::TimsTime(struct AtpTimsData *tims)
{
	int	vv;

	vv=0;
//	vv=Inpw(tims->nAddrs);
//	vv|=((int)Inpw(tims->nAddrs+2)<<16);
	vv=Inp(tims->nAddrs);
	vv|=((int)Inp(tims->nAddrs+1)<<8);
	vv|=((int)Inp(tims->nAddrs+2)<<16);
	tims->time=vv & 0x00ffffffl;
	tims->flag=(vv>>28)&0x000f;
	tims->time*=10;
	tims->time+=tims->nConstant;
	return tims->time;
}



void CUSLut::TimsInitialize(struct	AtpTimsData *tims)
{
	TimsControl(tims);
	TimsBlanking(tims);
	TimsDelay(tims);
	TimsStartThreshold(tims);
	TimsStopThreshold(tims);
	TimsAverages(tims);
}


void CUSLut::Retrieve(CString FilePath)
{
	CFileException e;
	CUSLFile file;
	int nGlobalOffset=0;
	int nTimeSlotOffset=sizeof m_Global + 0x200;

	if(!file.Open(FilePath,CFile::modeRead|CFile::typeBinary,&e)) {
//		Catch(&e);
	} else {
		Retrieve(&file);
		file.Close();
		m_FileName=FilePath;
		InitializeAddrs();
	}

}

void CUSLut::Save(CString FilePath)
{
	CUSLFile file;

	if(file.Open(FilePath,CFile::modeWrite|CFile::typeBinary|CFile::modeCreate)) {
		Save(&file);
		file.Close();
		m_FileName=FilePath;
	};
}

void CUSLut::Save(CUSLFile* pFile)
{
	int	nTS;

	pFile->WriteStruct(&m_Global, m_Global.nStructSize=sizeof m_Global);
	pFile->Seek(pFile->GetPosition()+0x200,CFile::begin);
	for(nTS=0;nTS<256;nTS++) {
		pFile->WriteStruct(&m_TS[nTS], m_TS[nTS].nStructSize=sizeof m_TS[0]);
		pFile->Seek(pFile->GetPosition()+0x200,CFile::begin);
	}
	for(nTS=0;nTS<256;nTS++) {
		pFile->WriteStruct(&m_Cal[nTS], m_Cal[nTS].nStructSize=sizeof m_Cal[0]);
		if((m_Cal[nTS].nArrayL>0) && (m_pnDefaultCalTrace[nTS]!=NULL)) {
			pFile->Seek(pFile->GetPosition()+0x20,CFile::begin);
			pFile->Write(m_pnDefaultCalTrace[nTS],m_Cal[nTS].nArrayL * sizeof (int));
		}
	}


}

void CUSLut::Retrieve(CUSLFile* pFile)
{
	int	nTS;

	if(pFile->GetLength()<70000) { //Version 1
//		RetrieveVersion1(pFile,&m_Global,m_TS);
	} else {
		pFile->ReadStruct(&m_Global, sizeof m_Global);
		pFile->Seek(pFile->GetPosition()+0x200,CFile::begin);

		int nMaxTS = 256;
		if(pFile->m_fVersion<4.22f) nMaxTS = 8;

		for(nTS=0;nTS<nMaxTS;nTS++) {
			pFile->ReadStruct(&m_TS[nTS], sizeof m_TS[0]);
			pFile->Seek(pFile->GetPosition()+0x200,CFile::begin);
			m_TS[nTS].Pr30.nWriteTimeSlot = nTS+1;
		}
		for(nTS=0;nTS<nMaxTS;nTS++) {
			SAFE_DELETE(m_pnCalTrace[nTS]);
			SAFE_DELETE(m_pnDefaultCalTrace[nTS]);
			pFile->ReadStruct(&m_Cal[nTS], sizeof m_Cal[0]);
			if((m_Cal[nTS].nArrayL>0) && (m_Cal[nTS].bDefaultTraceTrue==TRUE)) {
				m_pnDefaultCalTrace[nTS] = new int[m_Cal[nTS].nArrayL];
				pFile->Seek(pFile->GetPosition()+0x20,CFile::begin);
				pFile->Read(m_pnDefaultCalTrace[nTS],m_Cal[nTS].nArrayL * sizeof (int));
			}
			theApp.m_UtUser.m_TS[nTS].cName[59] = 0;
			for (int gg = 0; gg < 8; gg++) {
				theApp.m_UtUser.m_TS[nTS].Gate.cName[gg][59] = 0;
			}
		}
	}
	if (m_Global.Mux.nMaxTimeslots <= 0) m_Global.Mux.nMaxTimeslots = 1;

	for (int nIndex = 0; nIndex < 64; nIndex++) {
		m_Global.cChannelName[nIndex][59] = 0;
	}

}

void CUSLut::RetrieveVersion1(CUSLFile *pFile,UtGlobalData *pGlobal,TimeSlotData *pTS,int nGlobalOffset,int nTimeSlotOffset,int nOffset)
{
	int	nTS,gg;
	UtGlobalDataV1 Global;
	TimeSlotDataV1 TS;
	CString Buff;

	if(nGlobalOffset!=0) pFile->Seek(nGlobalOffset,CFile::begin);
	pFile->ReadStruct(&Global, sizeof m_Global);
	pGlobal->nPrf = Global.nPrf;
	pGlobal->nTimeSlot = Global.nTimeSlot;
	pGlobal->nTriggerSource = Global.nTriggerSource;
	CopyMemory(&pGlobal->Msc,&Global.Msc,sizeof Global.Msc);
	CopyMemory(&pGlobal->Mux,&Global.Mux,sizeof Global.Mux);
	for(nTS=0;nTS<64;nTS++) {
		Global.cChannelName[nTS][59]=0;
		Buff.Format(_T("%s"),(CString)Global.cChannelName[nTS]);
		swscanf_s(Buff,_T("%s"),&pGlobal->cChannelName[nTS],60);
		wcscpy_s(pGlobal->cChannelName[nTS],(WCHAR*)LPCTSTR(Buff));
	}

	if(nTimeSlotOffset!=0) {
		pFile->Seek(nTimeSlotOffset,CFile::begin);
	} else {
		pFile->Seek(pFile->GetPosition()+0x200,CFile::begin);
	}
	for(nTS=0;nTS<8;nTS++) {
		pFile->ReadStruct(&TS, sizeof TS);
		pTS[nTS].nAmplifier = TS.nAmplifier;
		pTS[nTS].nIrqCount = TS.nIrqCount;
		pTS[nTS].nTx = TS.nTx;
		pTS[nTS].nRx = TS.nRx;
		CopyMemory(&pTS[nTS].Pr20,&TS.Pr20,sizeof TS.Pr20);
		CopyMemory(&pTS[nTS].Pr50,&TS.Pr50,sizeof TS.Pr50);
		CopyMemory(&pTS[nTS].Pr15,&TS.Pr15,sizeof TS.Pr15);
		CopyMemory(&pTS[nTS].Pl15,&TS.Pl15,sizeof TS.Pl15);
		CopyMemory(&pTS[nTS].Dsp,&TS.Dsp,sizeof TS.Dsp);
		CopyMemory(&pTS[nTS].Gate,&TS.Gate,sizeof TS.Gate);
		CopyMemory(&pTS[nTS].Adc,&TS.Adc,sizeof TS.Adc);
		for(gg=0;gg<8;gg++) {
			TS.Gate.cName[gg][59]=0;
			Buff.Format(_T("%s"),(CString)TS.Gate.cName[gg]);
			wcscpy_s(pTS[nTS].Gate.cName[gg],(WCHAR*)LPCTSTR(Buff));
		}
		CopyMemory(pTS[nTS].Gpd,TS.Gpd,sizeof TS.Gpd);
		CopyMemory(pTS[nTS].Tims,TS.Tims,sizeof TS.Tims);
		CopyMemory(&pTS[nTS].Pr30,&TS.Pr30,sizeof TS.Pr30);
		CopyMemory(&pTS[nTS].Up15,&TS.Up15,sizeof TS.Up15);
		TS.cName[59]=0;
		Buff.Format(_T("%s"),(CString)TS.cName);
		wcscpy_s(pTS[nTS].cName,(WCHAR*)LPCTSTR(Buff));


		pFile->Seek(pFile->GetPosition()+nOffset,CFile::begin);
	}

}


//S455	BAE Samlesbury Foreplane Scanner
//Arcom PCI08	8 in 8 out
//Address	Function		R/W		Description
//00		Outputs			Write	outputs	0=0V 1=+24V	Asume Pull uP on output
//00		Outputs			Read	Read Output Control Register
//01		Inputs			Read	Inputs
//02		Output enable	Write	Enable(1) Disable(0)

void CUSLut::LiftInitialize()
{
	Outp(m_nPlcAddrs+0,0x00,USLDRIVER);
	Outp(m_nPlcAddrs+2,0x07,USLDRIVER);	//enable output
	if(IsLiftUp() == true) {
		LiftCommandUp();
	} else {
		if(IsLiftDown() == true) {
			LiftCommandDown();
		}
	}

}


bool CUSLut::LiftCommandUp()
{

	Outp(m_nPlcAddrs+2,0x07,USLDRIVER);	//enable output
	Outp(m_nPlcAddrs+0,0x01,USLDRIVER);
	if(Inp(m_nPlcAddrs+1,USLDRIVER) & 1) return FALSE;
	return TRUE;
}

bool CUSLut::LiftCommandDown()
{
	Outp(m_nPlcAddrs+2,0x07,USLDRIVER);	//enable output
	Outp(m_nPlcAddrs+0,0x02,USLDRIVER);
	if(Inp(m_nPlcAddrs+1,USLDRIVER) & 2) return FALSE;
	return TRUE;

}

bool CUSLut::LiftCommandStop()
{
	Outp(m_nPlcAddrs+2,0x07,USLDRIVER);	//enable output
	Outp(m_nPlcAddrs+0,0x00,USLDRIVER);
	return TRUE;

}


bool CUSLut::IsLiftUp()
{

	if(Inp(m_nPlcAddrs+1,USLDRIVER) & 1) return FALSE;
	return TRUE;
}

bool CUSLut::IsLiftDown()
{

	if(Inp(m_nPlcAddrs+1,USLDRIVER) & 2) return FALSE;
	return TRUE;
}

int CUSLut::WhereIsLift()
{

	int nStatus = Inp(m_nPlcAddrs+1,USLDRIVER) & 3;

	if(nStatus == 2) return 1;		//Up
	if(nStatus == 1) return 2;		//Down
	return 3;
}

void CUSLut::Buzzer(bool bAction)
{
	Outp(m_nPlcAddrs+2,0x07);
	int nn=Inp(m_nPlcAddrs+0,USLDRIVER);
	if(bAction==TRUE) {
		Outp(m_nPlcAddrs+0,nn+4,USLDRIVER);
	} else {
		Outp(m_nPlcAddrs+0,nn&0xfb,USLDRIVER);
	}

}



void CUSLut::Adc100Default(ADC200Data *pAdc)
{
	pAdc->nAddrs = 0x30;
	pAdc->nMemoryAddrs = 0xd4000l;
	pAdc->nHardwareDelay = 0;
	pAdc->control_reg = 0x34;
	pAdc->nConversionRate = 3;
	pAdc->fSamplePeriod=10.0;
	pAdc->nAcqLength=1024;
}

void CUSLut::Adc100Initialize(ADC200Data *pAdc)
{

	Adc100ConvTypeFreq(pAdc);
	Adc100Delay(pAdc);
	Adc100Priority(pAdc);
}

void CUSLut::Adc100ConvTypeFreq(ADC200Data *pAdc)
{
	int	temp,frequency;
	int nPolarity=1;
	int	nConvertMode=2;

	switch(pAdc->nConversionRate) {
	case 3:	frequency = 8;		/*100*/
			break;
	case 2:	frequency = 4;		/*50*/
			break;
	case 1:	frequency = 0x2;	/*25*/
			break;
	case 0:	frequency = 0x1;	/*12.5*/
			break;
	};
	temp = (nConvertMode << 4) + frequency;
	if(nPolarity>0) temp|=0x40;

	
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC100) return;
	Outp(pAdc->nAddrs+3,temp);
}




void CUSLut::Adc100Delay(ADC200Data *pAdc)
{
	int	nTime;

	pAdc->nHardwareDelay &= 0x7fff;
	switch(pAdc->convert_enable) {
	case 1: nTime = 0x8000 | pAdc->nHardwareDelay;
		break;
	case 0: nTime = pAdc->nHardwareDelay;
		break;
	};

	
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC100) return;
	Outp(pAdc->nAddrs,(nTime & 0xff));
	Outp(pAdc->nAddrs+1,(nTime >> 8));
}

void CUSLut::Adc100Priority(ADC200Data *pAdc)
{
	int	nPriority=1;

	pAdc->control_reg &= 0xdf;
	pAdc->control_reg |= ((nPriority & 0x01) << 5);

	
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC100) return;
	Outp(pAdc->nAddrs+2,pAdc->control_reg);
}


void CUSLut::Adc100CalculateHardware(ADC200Data *pAdc)
{
	

	int	nAcqLength,nSamplePeriod,nClkRate;

	if(theApp.m_LastSettings.nAdcDelayClkPeriod>0) {
		pAdc->nHardwareDelay=pAdc->nDelay / theApp.m_LastSettings.nAdcDelayClkPeriod;
	}

	nSamplePeriod=10;
	for(nClkRate=3;nClkRate>=0;nClkRate--) {
		nAcqLength=pAdc->nWidth / nSamplePeriod;
		if(nAcqLength<1024) goto finished;
		nSamplePeriod*=2;
	};
	finished:
	pAdc->nAcqLength=nAcqLength;
	pAdc->fSamplePeriod=(float)nSamplePeriod;
	pAdc->nConversionRate=nClkRate;
}





int CUSLut::Adc100ReadArray(ADC200Data *pAdc, char *pArray)
{
	
	if(theApp.m_LastSettings.nAdcCardType!=USL_ADC100) return FALSE;
	int	ii;
	int	nTemp,nFlag;

	if(pAdc->bBscanCollectData!=TRUE) {
		nTemp = pAdc->control_reg;
		/* stop adc and request access to ram */
		nTemp &= 0xef;	/* set bit 4 low */
		nTemp|=0x20;
		Outp(pAdc->nAddrs+2,nTemp);
		for(ii=0;ii<1000;ii++) {
			if((Inp(pAdc->nAddrs) & 0x10) == 0){	/* wait for adcacgrant*/
				ii=3000;
			};
		};
		if(ii<3000) {
			nFlag=Inp(pAdc->nAddrs);
			pAdc->control_reg |= 0x10;		/* relinquish adc */
			Outp(pAdc->nAddrs+2,pAdc->control_reg);
			return FALSE;
		};

		ReadAdc200Memory(pArray,pAdc->nAcqLength);

		pAdc->control_reg |= 0x10;		/* relinquish adc */
		Outp(pAdc->nAddrs+2,pAdc->control_reg);
	} else {
		ReadAdc200Memory(pArray,pAdc->nAcqLength);
	}
	return FALSE;
}



void CUSLut::GpdDefault(struct	AtpGpdData *gpd)
{

	gpd->nAddrs=0x40;
	gpd->nBlanking=10;
	gpd->nDelay=20;
	gpd->nWidth=30;
	gpd->nMinThreshold=100;
	gpd->nMaxThreshold=2000;
	gpd->nInterfaceThreshold=2000;
	gpd->rf_select=0;
	gpd->peak_sign=0;							//POSITIVE
	gpd->interface_sign=0;					//POSITIVE
	gpd->interface_slope=0;					//POSITIVE
	gpd->blank_marker=0;						//DISABLE
	gpd->delay_marker=0;						//DISABLE
	gpd->period_marker=1;					//ENABLE
	gpd->rf_monitor=0;						//THIS
	gpd->scope_trigger_mode=0;
	gpd->clk_rate=0;
}


void CUSLut::GpdControl(struct	AtpGpdData *gpd)
{
	gpd->nControlReg0 = 0;
	gpd->nControlReg0 |= (gpd->rf_select & 1);
	gpd->nControlReg0 |= ((gpd->nTriggerMode & 1)<<1);
	gpd->nControlReg0 |= ((gpd->peak_sign & 1)<<2);
	gpd->nControlReg0 |= ((gpd->interface_sign & 1)<<3);
	gpd->nControlReg0 |= ((gpd->interface_slope & 1)<<4);
	Outp(gpd->nAddrs,gpd->nControlReg0);

	gpd->nControlReg1 = 0;
	gpd->nControlReg1 |= (gpd->blank_marker & 1);
	gpd->nControlReg1 |= ((gpd->delay_marker & 1)<<1);
	gpd->nControlReg1 |= ((gpd->period_marker & 1)<<2);
	gpd->nControlReg1 |= ((gpd->clk_rate & 1)<<3);
	gpd->nControlReg1 |= ((gpd->scope_trigger_mode & 3)<<4);
	gpd->nControlReg1 |= ((gpd->rf_monitor & 1)<<7);
	Outp(gpd->nAddrs+1,gpd->nControlReg1);
}





void CUSLut::GpdBlanking(struct	AtpGpdData *gpd)
{

	Outpw(gpd->nAddrs+2,gpd->nBlanking);
}

void CUSLut::GpdDelay(struct	AtpGpdData *gpd)
{

	Outpw(gpd->nAddrs+4,gpd->nDelay);
}

void CUSLut::GpdWidth(struct	AtpGpdData *gpd)
{

	Outpw(gpd->nAddrs+6,gpd->nWidth);
}

void CUSLut::GpdMaxThreshold(struct	AtpGpdData *gpd)
{

	Outpw(gpd->nAddrs+0x08,gpd->nMaxThreshold);
}

void CUSLut::GpdMinThreshold(struct	AtpGpdData *gpd)
{

	Outpw(gpd->nAddrs+0x0a,gpd->nMinThreshold);
}

void CUSLut::GpdInterfaceThreshold(struct	AtpGpdData *gpd)
{

	Outpw(gpd->nAddrs+0x0c,gpd->nInterfaceThreshold);
}

int CUSLut::GpdAmplitude(struct AtpGpdData *gpd)
{
	int vv;

	vv=Inp(gpd->nAddrs);
	gpd->amplitude=vv&0x0f;
	gpd->flags=(vv>>4)&0x0f;
	return vv&0x0f;
}



void CUSLut::GpdInitialize(struct	AtpGpdData *gpd)
{
	GpdControl(gpd);
	GpdBlanking(gpd);
	GpdDelay(gpd);
	GpdWidth(gpd);
	GpdMaxThreshold(gpd);
	GpdMinThreshold(gpd);
	GpdInterfaceThreshold(gpd);
}


void CUSLut::InitializeAddrs()
{
	
	int	ii;

	m_Global.Msc.nAddrs=theApp.m_LastSettings.nMscAddrs;
	for(ii=0;ii<8;ii++) {
		m_TS[ii].Pr20.nAddrs=theApp.m_LastSettings.nPr20Addrs;
		m_TS[ii].Pr50.nAddrs=theApp.m_LastSettings.nPr50Addrs;
		m_TS[ii].Pr15.nAddrs=theApp.m_LastSettings.nPr15Addrs=0x89;
		m_TS[ii].Pl15.nAddrs=theApp.m_LastSettings.nPl15Addrs=0x91;
		m_TS[ii].Pr20.nType=USL_PR20;
		m_TS[ii].Pr50.nType=USL_PR50;
		m_TS[ii].Adc.nAddrs=theApp.m_LastSettings.nAdcAddrs;
		m_TS[ii].Adc.nMemoryAddrs=theApp.m_LastSettings.nAdcMemoryAddrs;
		m_TS[ii].Dsp.nAddrs=theApp.m_LastSettings.nDspAddrs;
		m_TS[ii].Gpd[0].nAddrs=theApp.m_LastSettings.nAtpGpd0Addrs;
		m_TS[ii].Gpd[1].nAddrs=theApp.m_LastSettings.nAtpGpd1Addrs;
		m_TS[ii].Tims[0].nAddrs=theApp.m_LastSettings.nAtpTims0Addrs;
		m_TS[ii].Tims[1].nAddrs=theApp.m_LastSettings.nAtpTims1Addrs;
		m_TS[ii].Dsp.nCardType=theApp.m_LastSettings.nDspCardType;
		m_TS[ii].Adc.nCardType=theApp.m_LastSettings.nAdcCardType;
		m_TS[ii].Pr30.nDestAddrs=theApp.m_LastSettings.nPr30DestAddrs[theApp.m_LastSettings.nUseWhichPR30];//=0x8d;
		m_TS[ii].Pr30.nSrcAddrs=theApp.m_LastSettings.nPr30SrcAddrs=0x7d;

	}

}

//************************************************
// Pr30
//*************************************************

void CUSLut::Pr30Command1(Pr30Data	*pr30)
{
	int	nCode[2] = {0x10,0x20};

	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {
			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(1);
			MailboxTx(nCode[pr30->nPrfSrc&1]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command2(Pr30Data	*pr30)
{
	
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {
			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(2);
			MailboxTx((pr30->nPrf>>7) & 0x7f);
			MailboxTx(pr30->nPrf & 0x7f);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command3(Pr30Data	*pr30)
{
	int	nCode[2] = {0x10,0x20};
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(3);
			MailboxTx(nCode[pr30->nTimeSlotSyncSrc&1]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command4(Pr30Data	*pr30)
{
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(4);
			MailboxTx(pr30->nWriteTimeSlot);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command5(Pr30Data	*pr30)			//Reset Max Timeslot
{
	
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(5);
			MailboxTx(0x5A);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command6(Pr30Data	*pr30)
{
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(6);
			MailboxTx(pr30->nSyncPulseTimeSlot);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command7(Pr30Data	*pr30)
{
	int	nCode[2] = {0x10,0x20};
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(7);
			MailboxTx(nCode[pr30->nMuxRun&1]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command8(Pr30Data	*pr30,int nAction)			//Clear Currently selected timeslot
{
	int nCode[2] = {0x10,0x20};				//0 Clear All current TS Data
											//1 Clear only DAC Data for current TS
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(8);
			MailboxTx(nCode[nAction&1]);			//0=Timeslot, 1=Max Data pt
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}


void CUSLut::Pr30Command16(Pr30Data	*pr30)
{
	int	nCode[6] = {0x2c,0x36,0x40,0x4a,0x54,0x5e};

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(16);
			MailboxTx(nCode[pr30->nPreampGain%6]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command17(Pr30Data	*pr30)
{
	int	nCode[6] = {0x10,0x20};

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {
		MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
		MailboxTx(pr30->nSrcAddrs);
		MailboxTx(17);
		MailboxTx(nCode[pr30->nPreampMode&1]);
		MailboxTxCheckSum();
		MailboxTx(0xff);
		}
	}
}
void CUSLut::Pr30Command18(Pr30Data	*pr30,bool bReset)		//reset max PPA selected
{
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			int nValue = pr30->nPreampWriteSelect&0x1f;
			if(bReset==TRUE) nValue |= 0x40;

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(18);
			MailboxTx(nValue);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command19(Pr30Data	*pr30)
{
	int	nCode[7] = {0x04,0x08,0x10,0x14};
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(19);
			MailboxTx(nCode[pr30->nDamping%7]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}


void CUSLut::Pr30Command20(Pr30Data	*pr30)
{
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(20);
			MailboxTx((pr30->nPulseWidth>>7) & 0x1f);
			MailboxTx(pr30->nPulseWidth & 0x7f);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command21(Pr30Data	*pr30)
{
	int	nCode[7] = {0x10,0x20};

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {


		MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
		MailboxTx(pr30->nSrcAddrs);
		MailboxTx(21);
		MailboxTx(nCode[pr30->nPulserOffOn]);
		MailboxTxCheckSum();
		MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command22(Pr30Data	*pr30)
{
	
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

		MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
		MailboxTx(pr30->nSrcAddrs);
		MailboxTx(22);
		MailboxTx((pr30->nPulseVoltage>>7) & 0x03);
		MailboxTx(pr30->nPulseVoltage & 0x7f);
		MailboxTxCheckSum();
		MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command23(Pr30Data	*pr30)
{
	int	nCode[7] = {0x10,0x20};
	
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

		MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
		MailboxTx(pr30->nSrcAddrs);
		MailboxTx(23);
		MailboxTx(nCode[pr30->nHtOffOn]);
		MailboxTxCheckSum();
		MailboxTx(0xff);
		}
	}


}

void CUSLut::Pr30Command24(Pr30Data	*pr30)
{
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(24);
			MailboxTx(pr30->nMuxTx);	//1 - nn
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command25(Pr30Data	*pr30)
{

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

		MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
		MailboxTx(pr30->nSrcAddrs);
		MailboxTx(25);
		MailboxTx(pr30->nMuxRx);	//11 - nn
		MailboxTxCheckSum();
		MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command26(Pr30Data	*pr30)
{
}
void CUSLut::Pr30Command27(Pr30Data	*pr30)
{
}
void CUSLut::Pr30Command28(Pr30Data	*pr30)
{
}

void CUSLut::Pr30Command30(Pr30Data	*pr30)
{
	int nCode[3] = {0x10,0x20,0x30};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {


		MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
		MailboxTx(pr30->nSrcAddrs);
		MailboxTx(30);
		MailboxTx(nCode[pr30->nLogCourseGain %  3]);
		MailboxTxCheckSum();
		MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command31(Pr30Data	*pr30)
{
	int nCode[3] = {0x10,0x20,0x30};
	


	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			if(theApp.m_LastSettings.nPr30Version[ii] == 0) {
				MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
				MailboxTx(pr30->nSrcAddrs);
				MailboxTx(31);
				MailboxTx(pr30->nLogFineGain % 0x64);
				MailboxTxCheckSum();
				MailboxTx(0xff);
			}
		}
	}
}

void CUSLut::Pr30Command32(Pr30Data	*pr30)
{
	int nCode[6]= {8,0x10,0x20,0x30,0x40,0x70};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

		MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
		MailboxTx(pr30->nSrcAddrs);
		MailboxTx(32);
		MailboxTx(nCode[pr30->nLogPostDetectFilter%6]);
		MailboxTxCheckSum();
		MailboxTx(0xff);
		}
	}

}


void CUSLut::Pr30Command33(Pr30Data	*pr30)
{
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			if(pr30->nOutputSelect<=1) {
				MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
				MailboxTx(pr30->nSrcAddrs);
				MailboxTx(33);
				MailboxTx((pr30->nLinGain>>7) & 0x07);
				MailboxTx(pr30->nLinGain & 0x7f);
				MailboxTxCheckSum();
				MailboxTx(0xff);
			}
			if((pr30->nOutputSelect==2) && (theApp.m_LastSettings.nPr30Version[ii]>=1)) {
				MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
				MailboxTx(pr30->nSrcAddrs);
				MailboxTx(33);
				MailboxTx((pr30->nLogFineGain>>7) & 0x07);
				MailboxTx(pr30->nLogFineGain & 0x7f);
				MailboxTxCheckSum();
				MailboxTx(0xff);
			}
		}
	}
}

void CUSLut::Pr30Command34(Pr30Data	*pr30)
{
	int	nCode[7] = {0x10,0x20};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {


		MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
		MailboxTx(pr30->nSrcAddrs);
		MailboxTx(34);
		MailboxTx(nCode[pr30->nLinTimeConstant&1]);
		MailboxTxCheckSum();
		MailboxTx(0xff);
		}
	}

}


void CUSLut::Pr30Command35(Pr30Data	*pr30)
{
	int nCode[2] = {0x10,0x20};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {
			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(35);
			MailboxTx(nCode[pr30->nDacEnable&0x01]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command36(Pr30Data	*pr30)
{
	int nCode[2] = {0x10,0x20};
	

	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

		MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
		MailboxTx(pr30->nSrcAddrs);
		MailboxTx(36);
		MailboxTx(nCode[pr30->nDacTriggerMode&1]);
		MailboxTxCheckSum();
		MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command37(Pr30Data	*pr30)
{
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

		MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
		MailboxTx(pr30->nSrcAddrs);
		MailboxTx(37);
		MailboxTx((pr30->nDacTriggerThreshold>>7) & 1);
		MailboxTx(pr30->nDacTriggerThreshold & 0x7f);
		MailboxTxCheckSum();
		MailboxTx(0xff);
		}
	}
}


void CUSLut::Pr30Command38(Pr30Data	*pr30)
{
	int nCode[2]={0x10,0x20};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(38);
			MailboxTx(nCode[pr30->nDacTriggerSlope&1]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command39(Pr30Data	*pr30)
{
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			switch(theApp.m_LastSettings.nDacBlankingUnits%3) {
			case 0:
				pr30->nDacBlanking = (int)(pr30->fDacBlanking * 10.0f);
				break;
			case 1:
				pr30->nDacBlanking = (int)(pr30->fDacBlanking / (float)theApp.m_LastSettings.nWaterVelocity * 2e4f);
				break;
			case 2:
				pr30->nDacBlanking = (int)(pr30->fDacBlanking / (float)theApp.m_LastSettings.nMaterialVelocity *2e4f );
				break;
			}

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(39);
			MailboxTx((pr30->nDacBlanking>>14) & 3);
			MailboxTx((pr30->nDacBlanking>>7) & 0x7f);
			MailboxTx( pr30->nDacBlanking & 0x7f);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command40(Pr30Data	*pr30,int nStartAddrs)
{

	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(40);
			MailboxTx((nStartAddrs>>7) & 7);
			MailboxTx(nStartAddrs & 0x7f);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

//what to do here with multiple PM30s
void CUSLut::Pr30Command41(Pr30Data	*pr30,int *pnValue)
{

	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {
			int nNumberPts,nLoop;
			int nValue;


			if(pr30->nNumberDacPts==0) pr30->nNumberDacPts=1;
			for(nLoop=0; nLoop<pr30->nNumberDacPts; nLoop+=nNumberPts) {

				nNumberPts=pr30->nNumberDacPts - nLoop;
				if(nNumberPts>100) nNumberPts=100;

				Pr30Command40(pr30,nLoop);


				MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
				MailboxTx(pr30->nSrcAddrs);
				MailboxTx(41);
				MailboxTx(nNumberPts);
				for(int ii=0;ii<nNumberPts;ii++) {
					nValue = abs(pnValue[ii+nLoop]);

					if(pnValue[ii+nLoop]>=0) {
						MailboxTx((nValue>>7) & 7);
					} else {
						MailboxTx(((nValue>>7) & 7) | 0x40);
					}
					MailboxTx(nValue & 0x7f);
				}
				MailboxTxCheckSum();
				MailboxTx(0xff);
			}
		}
	}


}
//0x2A
void CUSLut::Pr30Command42(Pr30Data	*pr30)
{
	int nCode[8] = {0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x3C};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(42);
			MailboxTx(nCode[pr30->nDacClkRate%8]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command43(Pr30Data	*pr30)
{
	
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(43);
			MailboxTx((pr30->nAuxLinGain>>7) & 0x01);
			MailboxTx(pr30->nAuxLinGain & 0x7f);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command45(Pr30Data	*pr30)
{
	int nCode[3] = {0x10,0x20,0x30};
	
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {


			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(45);
			MailboxTx(nCode[pr30->nRectifierDetectionMode %  3]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command46(Pr30Data	*pr30)
{
	
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {
	
			int nValue = ((pr30->nRectifierDecayPercentage * 250) / 100) + 5;

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(46);
			MailboxTx((nValue >> 7) & 1);
			MailboxTx(nValue & 0x7f);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}
void CUSLut::Pr30Command47(Pr30Data	*pr30)
{
	int nCode[2]={0x10,0x20};
	
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {


			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(47);
			MailboxTx(nCode[pr30->nFilterMode & 1]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command48(Pr30Data	*pr30)
{
	int nCode[5]={0x10,0x20,0x30,0x40,0x50};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(48);
			MailboxTx(nCode[pr30->nLowPassRange%5]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command49(Pr30Data	*pr30)
{
	int nCode[5]={0x10,0x20,0x30,0x40,0x50};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(49);
			MailboxTx(nCode[pr30->nBandPassRange%5]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

//0x32
void CUSLut::Pr30Command50(Pr30Data	*pr30)
{
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			int nValue = pr30->nBandPassCenterFrequency;
			if(nValue<3) nValue = 3;
			if(nValue>63) nValue = 63;

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(50);
			MailboxTx(pr30->nBandPassCenterFrequency);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}


void CUSLut::Pr30Command51(Pr30Data	*pr30)
{
	int nCode[2]={0x10,0x20};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(51);
			MailboxTx(nCode[pr30->nBandPassWidth&1]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command52(Pr30Data	*pr30)
{
	int nCode[4]= {0x20,0x30,0x40,0x50};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {


			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(52);
			MailboxTx(nCode[pr30->nOutputSelect]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command53(Pr30Data	*pr30)
{
	int nCode[4]= {0x10,0x20};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(53);
			MailboxTx(nCode[pr30->nSelfTestOffOn]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command54(Pr30Data	*pr30)
{
	
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(54);
			MailboxTx((pr30->nSelfTestFrequency>>7)&0x1f);
			MailboxTx(pr30->nSelfTestFrequency&0x3f);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command55(Pr30Data	*pr30)
{
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(55);
			MailboxTx(pr30->nSelfTestAmplitude&0x3f);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command56(Pr30Data	*pr30)
{
	
	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {
	
			if(theApp.m_LastSettings.nPr30Version[ii] < 2) return;

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(56);
			MailboxTx(pr30->nDACDeltaDelay);	//range 0 - 15
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30Command64(Pr30Data	*pr30)
{
	int nCode[2]= {0x10,0x20};
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(64);
			MailboxTx(nCode[pr30->nOutputOffsetEnable&1]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}

}

void CUSLut::Pr30Command67(Pr30Data	*pr30)
{
	int nCode[3]= {0x20,0x30,0x10}; 
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			if(theApp.m_LastSettings.nMuxVersion < 3) {
				MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
				MailboxTx(pr30->nSrcAddrs);
				MailboxTx(67);
				MailboxTx(nCode[theApp.m_LastSettings.nMuxVersion]);
				MailboxTxCheckSum();
				MailboxTx(0xff);
			}
		}
	}
}

//I2C Umbillical select
void CUSLut::Pr30Command68(Pr30Data	*pr30)
{
	int nCode[]= {0x10,0x20}; 
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(68);
			MailboxTx(nCode[pr30->nI2CUmbilicalSelect & 1]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

//RxRF umbilical select
void CUSLut::Pr30Command69(Pr30Data	*pr30)
{
	int nCode[]= {0x10,0x20}; 
	

	for(int ii=0;ii<4;ii++) {
		if((theApp.m_LastSettings.bUseWhichPM30[ii])&&(theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pr30->nSrcAddrs);
			MailboxTx(69);
			MailboxTx(nCode[pr30->nRxRfUmbilicalSelect & 1]);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

//Enable ADC
void CUSLut::Pr30Command4Ah(TimeSlotData *pTS)
{

	for (int ii = 0; ii < 4; ii++) {
		if ((theApp.m_LastSettings.bUseWhichPM30[ii]) && (theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pTS->Pr30.nSrcAddrs);
			MailboxTx(0x4A);
			MailboxTx(0x00);
			MailboxTx(0x06);		//D0:0 Trigger = PRF, D1:1 Acquistion enable = true, D2:0 IRQ = false
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

//Sampling rate
void CUSLut::Pr30Command4Bh(TimeSlotData *pTS)
{
	int nDivisor = pTS->Adc.nClkDivisor;
	for (int ii = 0; ii < 4; ii++) {
		if ((theApp.m_LastSettings.bUseWhichPM30[ii]) && (theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pTS->Pr30.nSrcAddrs);
			MailboxTx(0x4B);
			MailboxTx(0x00);		//Both ADC
			MailboxTx((nDivisor >> 7) & 0x01);		//Divisor = 1
			MailboxTx(nDivisor & 0x7f);		//Divisor = 1
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

//Number of samples after trigger
void CUSLut::Pr30Command4Dh(TimeSlotData *pTS)
{
	float fDelayns = (float)pTS->Adc.nDelay + theApp.m_Scope.m_fMainBangConstant * 1e9f;
	int nSamples = pTS->Adc.nAcqLength + (int)(fDelayns / pTS->Adc.fSamplePeriod);
	for (int ii = 0; ii < 4; ii++) {
		if ((theApp.m_LastSettings.bUseWhichPM30[ii]) && (theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pTS->Pr30.nSrcAddrs);
			MailboxTx(0x4D);
			MailboxTx(0x00);		//Both ADC
			MailboxTx((nSamples >> 14) & 0x3f);
			MailboxTx((nSamples >> 7) & 0x7f);
			MailboxTx((nSamples >> 0) & 0x7f);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}
//ADC test modes control
void CUSLut::Pr30Command69h(TimeSlotData *pTS, int nMode)
{

	for (int ii = 0; ii < 4; ii++) {
		if ((theApp.m_LastSettings.bUseWhichPM30[ii]) && (theApp.m_LastSettings.bPr30Present[ii])) {

			MailboxTx(theApp.m_LastSettings.nPr30DestAddrs[ii]);
			MailboxTx(pTS->Pr30.nSrcAddrs);
			MailboxTx(0x69);
			MailboxTx(nMode);
			MailboxTxCheckSum();
			MailboxTx(0xff);
		}
	}
}

void CUSLut::Pr30ResetMaxTimeSlots(Pr30Data *pPr30, bool bClearAllData)
{
	Pr30MuxHalt(pPr30);
	Pr30Command5(pPr30);
	Pr30Command18(pPr30,bClearAllData);
	Sleep(100);
}


void CUSLut::Pr30Prf(Pr30Data *pPr30)
{
	Pr30Command2(pPr30);
}

void CUSLut::Pr30SyncPulseTimeSlot(Pr30Data *pPr30)
{
	Pr30Command6(pPr30);
}

void CUSLut::Pr30PreampGain(Pr30Data *pPr30)
{
	pPr30->nPreampWriteSelect = pPr30->nMuxRx;
	Pr30Command18(pPr30);
	Pr30Command16(pPr30);
}

void CUSLut::Pr30PreampMode(Pr30Data *pPr30)
{
	pPr30->nPreampWriteSelect = pPr30->nMuxTx;
	Pr30Command18(pPr30);
	Pr30Command17(pPr30);
}

void CUSLut::Pr30Damping(Pr30Data *pPr30)
{
	pPr30->nPreampWriteSelect = pPr30->nMuxTx;
	Pr30Command18(pPr30);
	Pr30Command19(pPr30);
}

void CUSLut::Pr30PulseWidth(Pr30Data *pPr30)
{
	pPr30->nPreampWriteSelect = pPr30->nMuxTx;
	Pr30Command18(pPr30);
	Pr30Command20(pPr30);
}

void CUSLut::Pr30PulserOffOn(Pr30Data *pPr30)
{
	pPr30->nPreampWriteSelect = pPr30->nMuxTx;
	Pr30Command18(pPr30);
	Pr30Command21(pPr30);
}

void CUSLut::Pr30HtOffOn(Pr30Data *pPr30)
{
	pPr30->nPreampWriteSelect = pPr30->nMuxTx;
	Pr30Command18(pPr30);
	Pr30Command23(pPr30);
}

void CUSLut::Pr30PulseVoltage(Pr30Data *pPr30)
{
	pPr30->nPreampWriteSelect = pPr30->nMuxTx;
	Pr30Command18(pPr30);
	Pr30Command22(pPr30);
}

void CUSLut::Pr30OutputSelect(Pr30Data *pPr30)
{

	Pr30Command52(pPr30);
}

void CUSLut::Pr30OutputOffsetEnable(Pr30Data *pPr30)
{

	Pr30Command64(pPr30);
}

void CUSLut::Pr30LogFloatGain(Pr30Data *pPr30)
{

	pPr30->nLogCourseGain = (int)(pPr30->fLogGain/10.0);
	pPr30->nLogFineGain = (int)fmod(pPr30->fLogGain*10.0,100.0);
	Pr30Command30(pPr30);
	Pr30Command33(pPr30);
}

void CUSLut::Pr30LinFloatGain(Pr30Data *pPr30)
{
	pPr30->nLinGain = (int)(pPr30->fLinGain*10.0);
	Pr30Command33(pPr30);
}



void CUSLut::Pr30AuxFloatGain(Pr30Data *pPr30)
{
	pPr30->nAuxLinGain = (int)(pPr30->fAuxLinGain*5.0);
	Pr30Command43(pPr30);
}

void CUSLut::Pr30FilterType(Pr30Data *pPr30)
{
	switch(pPr30->nFilterType) {
	case 0: pPr30->nFilterMode=0;
		break;
	case 1: pPr30->nFilterMode=0;
		pPr30->nBandPassWidth=0;
		break;
	case 2: pPr30->nFilterMode=1;
		pPr30->nBandPassWidth=1;
		break;
	}
	Pr30Command47(pPr30);
	Pr30Command51(pPr30);

}

void CUSLut::Pr30BandPassWidth(Pr30Data *pPr30)
{
	Pr30Command51(pPr30);

}

void CUSLut::Pr30FilterMode(Pr30Data *pPr30)
{
	Pr30Command47(pPr30);

}

float CUSLut::Pr30CalculateRealWorldCentreFrequency(Pr30Data *pPr30,CString *pBuff)
{

	float fMin[5] = {0.5,1.0,2.0,4.0,8.0};
	float fMax[5] = {2.0,4.0,8.0,16.0,32.0};
	float fOffset,fValue;
	int nScale;

	pPr30->nBandPassRange %= 5;

	switch(nScale=3) {
	case 0: fOffset = (float)(63-pPr30->nBandPassCenterFrequency) / 60.0f;		//0 - 1.0
		fOffset = (float)exp(fOffset) - 1.0f;
		fValue = ((fMax[pPr30->nBandPassRange]-fMin[pPr30->nBandPassRange]) * fOffset) / 1.71828f;
		break;
	case 1: fOffset = (float)(63-pPr30->nBandPassCenterFrequency) / 30.0f;		//0 - 1.0
		fOffset = (float)exp(fOffset) - 1.0f;
		fValue = ((fMax[pPr30->nBandPassRange]-fMin[pPr30->nBandPassRange]) * fOffset) / 6.389056f;
		break;
	case 2: fOffset = (float)(63-pPr30->nBandPassCenterFrequency) / 15.0f;		//0 - 1.0
		fOffset = (float)exp(fOffset) - 1.0f;
		fValue = ((fMax[pPr30->nBandPassRange]-fMin[pPr30->nBandPassRange]) * fOffset) / 53.59815f;
		break;
	case 3: fOffset = (float)(63-pPr30->nBandPassCenterFrequency) / 12.0f;		//0 - 1.0
		fOffset = (float)exp(fOffset) - 1.0f;
		fValue = ((fMax[pPr30->nBandPassRange]-fMin[pPr30->nBandPassRange]) * fOffset) / 147.413159f;
		break;
	case 4: fOffset = (float)(63-pPr30->nBandPassCenterFrequency) / 10.0f;		//0 - 1.0
		fOffset = (float)exp(fOffset) - 1.0f;
		fValue = ((fMax[pPr30->nBandPassRange]-fMin[pPr30->nBandPassRange]) * fOffset) / 402.42879f;
		break;
	}

	fValue += fMin[pPr30->nBandPassRange];

	pBuff->Format(_T("%.02f MHz"),fValue);

	return fValue;
}

void CUSLut::Pr30RectifierMode(Pr30Data *pPr30)
{
	Pr30Command45(pPr30);
}

void CUSLut::Pr30RectifierDecay(Pr30Data *pPr30)
{
	Pr30Command46(pPr30);
}


void CUSLut::Pr30BlankingIntToFloat(Pr30Data *pPr30)
{
	

	switch(theApp.m_LastSettings.nDacBlankingUnits%3) {
	case 0:
		pPr30->fDacBlanking = (float)pPr30->nDacBlanking / 10.0f;
		break;
	case 1:
		pPr30->fDacBlanking = (float)pPr30->nDacBlanking * (float)theApp.m_LastSettings.nWaterVelocity / 2e4f;
		break;
	case 2:
		pPr30->fDacBlanking = (float)pPr30->nDacBlanking * (float)theApp.m_LastSettings.nMaterialVelocity /2e4f;
		break;
	}

}

void CUSLut::Pr30DacBlankingString(Pr30Data *pPr30, CString *pBuff)
{
	
	CString Micro = L"\x0b5";

	switch(theApp.m_LastSettings.nDacBlankingUnits%3) {
	case 0:
		pBuff->Format(_T("%.01f %ss"),pPr30->fDacBlanking,Micro);
		break;
	case 1:
		pBuff->Format(_T("%.01f mm w"),pPr30->fDacBlanking);
		break;
	case 2:
		pBuff->Format(_T("%.01f mm w"),pPr30->fDacBlanking);
		break;
	}
}


void CUSLut::Pr30DacBlankingFromString(Pr30Data *pPr30, CString Buff)
{
	
	_WTOF(Buff,pPr30->fDacBlanking);

	Pr30Command4(pPr30);
	Pr30Command39(pPr30);

}

void CUSLut::Pr30DacBlanking(Pr30Data *pPr30)
{

	Pr30Command4(pPr30);
	Pr30Command39(pPr30);

}

void CUSLut::Pr30DacTriggerMode(Pr30Data *pPr30)
{
	Pr30Command36(pPr30);
}

void CUSLut::Pr30DacClkRate(Pr30Data *pPr30)
{
	Pr30Command42(pPr30);
	Pr30Command56(pPr30);
}

void CUSLut::Pr30DacEnable(Pr30Data *pPr30)
{
	Pr30Command4(pPr30);
	Pr30Command35(pPr30);
}


void CUSLut::Pr30DacThreshold(Pr30Data *pPr30)
{

	Pr30Command37(pPr30);
	Pr30Command38(pPr30);

}

void CUSLut::Pr30ZeroCurrentDacGains(Pr30Data *pPr30)
{

	for(int ii=0;ii<64;ii++) {
		pPr30->fDacGain[pPr30->nDacTable][ii]=0.0f;
	}
}

void CUSLut::Pr30ZeroCurrentDacDelaysAndGains(Pr30Data *pPr30)
{
	for(int ii=0;ii<64;ii++) {
		pPr30->fDacDelay[pPr30->nDacTable][ii]=0.0f;
		pPr30->fDacGain[pPr30->nDacTable][ii]=0.0f;
	}

}


void CUSLut::Pr30Defaults(Pr30Data *pPr30,int nTS)
{
	
	memset(pPr30,0,sizeof (Pr30Data));

	theApp.m_LastSettings.nPr30DestAddrs[theApp.m_LastSettings.nUseWhichPR30]=0x8d;
	pPr30->nSrcAddrs=0x7d;

	pPr30->nPrfSrc = 1;				//0 Internal, 1 External
	pPr30->nPrf = 1000;
	pPr30->nTimeSlotSyncSrc = 0;	//0 Internal, 1 External
	pPr30->nWriteTimeSlot = nTS+1;		//All timeslots will be written to
	pPr30->nSyncPulseTimeSlot = 1;
	pPr30->nMuxRun = 0;
	pPr30->nPreampWriteSelect = 1;
	pPr30->nPreampGain = 4;
	pPr30->nPreampMode = 0;
	pPr30->nDamping = 0;
	pPr30->nPulseWidth = 800;		//100ns
	pPr30->nPulserOffOn = 1;
	pPr30->nPulseVoltage = 200;		//200V
	pPr30->nHtOffOn = 1;
	pPr30->nMuxTx =nTS+1;
	pPr30->nMuxRx =nTS+1;


	pPr30->nLogCourseGain=2;		//20dB
	pPr30->nLogFineGain=0x64;		//10dB
	pPr30->nLogPostDetectFilter=5;	//WideBand

	pPr30->fLinGain=0.0;			//0dB
	pPr30->nLinGain = (int)(pPr30->fLinGain*10.0);

	pPr30->fAuxLinGain=0.0;			//0dB
	pPr30->nAuxLinGain = (int)(pPr30->fAuxLinGain*5.0);

	pPr30->nLinTimeConstant = 0;

	pPr30->nDacEnable=0;			//Off
	pPr30->nDacTriggerSlope=0;
	pPr30->nDacTriggerThreshold=128;

	pPr30->nI2CUmbilicalSelect = 0;
	pPr30->nRxRfUmbilicalSelect = 0;


}

void CUSLut::Pr30Initialize(Pr30Data *pPr30)
{

	Pr30Command67(pPr30);	//Mux version
	Pr30Command4(pPr30);	//Write timeslot start at zero
	Pr30Command68(pPr30);
	Pr30Command69(pPr30);

	Pr30Command24(pPr30);	//Mux transmitter start at 1
	Pr30Command25(pPr30);	//Mux receiver star at 11

	Pr30Command1(pPr30);
	Pr30Command2(pPr30);
	Pr30Command3(pPr30);
	Pr30Command6(pPr30);

	pPr30->nPreampWriteSelect = pPr30->nMuxTx;
	Pr30Command18(pPr30);	//Choose preamp to write to
	Pr30PreampMode(pPr30);
	Pr30Damping(pPr30);
	Pr30PulseWidth(pPr30);
	Pr30PulserOffOn(pPr30);
	Pr30HtOffOn(pPr30);

	pPr30->nPreampWriteSelect = pPr30->nMuxRx;
	Pr30Command18(pPr30);	//Choose preamp to write to
	Pr30PreampGain(pPr30);

	Pr30Command26(pPr30);
	Pr30Command27(pPr30);

	Pr30Command28(pPr30);
	Pr30Command30(pPr30);
	Pr30Command31(pPr30);

	Pr30Command32(pPr30);
	Pr30Command33(pPr30);
	Pr30Command34(pPr30);
	Pr30Command35(pPr30);

	Pr30Command36(pPr30);
	Pr30Command37(pPr30);

	Pr30Command38(pPr30);
	Pr30Command39(pPr30);

	Pr30Command42(pPr30);
	Pr30Command43(pPr30);
	Pr30Command45(pPr30);
	Pr30Command46(pPr30);
	Pr30Command47(pPr30);
	Pr30Command56(pPr30);

	Pr30Command48(pPr30);
	Pr30Command49(pPr30);

	Pr30Command50(pPr30);
	Pr30Command51(pPr30);
	Pr30Command52(pPr30);
	Pr30Command64(pPr30);

	Pr30CalculateDacData(pPr30);

	Pr30PulseVoltage(pPr30);


}




void CUSLut::Pr30CalculateDacData(Pr30Data* pPr30)
{
	
	float fMaxTime = 0.0f;
	int	nT = pPr30->nDacTable = 0;
	int nArray[1000];
	memset(nArray,0,1000 * sizeof (int));
	float fLatency = 0.2f;	//200ns
	float fPos;
	CPolyCoord Line;
	CCoord Cp;

	if(theApp.m_LastSettings.nPr30Version[theApp.m_LastSettings.nUseWhichPR30]==2) {
		fLatency = 0.0f;
		pPr30->nDACDeltaDelay = 0;
		Pr30Command56(pPr30);
	};

	Cp.Zero();
	Line.Zero();
	for(int ii=0;ii<64;ii++) {
		switch(theApp.m_LastSettings.nDacDelayUnits) {
		case 0:	Cp.fPos[0] = pPr30->fDacDelay[nT][ii];		//us
			break;
		case 1:	Cp.fPos[0] = pPr30->fDacDelay[nT][ii] / (float)theApp.m_LastSettings.nWaterVelocity * 2000.0f;		//mm
			break;
		case 2:	Cp.fPos[0] = pPr30->fDacDelay[nT][ii] / (float)theApp.m_LastSettings.nMaterialVelocity * 2000.0f;		//mm
			break;
		}
		if(Cp.fPos[0]>fMaxTime) fMaxTime = Cp.fPos[0];
		Cp.fPos[1]=pPr30->fDacGain[nT][ii];
		if((ii==0) || ((ii>0) && (Cp.fPos[0]!=0.0f))) {
			if(Cp.fPos[0] > fLatency) Cp.fPos[0] -= fLatency;
			Line.Add(Cp);
		}
	}

	pPr30->nDacClkRate=6;
	float fClkPeriod=0.1f;
	if(theApp.m_LastSettings.nPr30Version[theApp.m_LastSettings.nUseWhichPR30]==2) {
		pPr30->nDacClkRate=6;
		fClkPeriod=0.06666f;
		if(fMaxTime>66.666) {
			pPr30->nDacClkRate=6;
			fClkPeriod=0.1f;
		}
	}


	if(fMaxTime>100.0) {
		pPr30->nDacClkRate=5;
		fClkPeriod=0.2f;
	}
	if(fMaxTime>200.0) {
		pPr30->nDacClkRate=4;
		fClkPeriod=0.5f;
	}
	if(fMaxTime>500.0) {
		pPr30->nDacClkRate=3;
		fClkPeriod=1.0f;
	}
	if(fMaxTime>1000.0) {
		pPr30->nDacClkRate=2;
		fClkPeriod=2.0f;
	}
	if(fMaxTime>2000.0) {
		pPr30->nDacClkRate=1;
		fClkPeriod=5.0f;
	}
	if(fMaxTime>5000.0) {
		pPr30->nDacClkRate=0;
		fClkPeriod=10.0f;
	}

	pPr30->nNumberDacPts = (int)(fMaxTime / fClkPeriod);


	for(int ii=0;ii<pPr30->nNumberDacPts;ii++) {
		fPos = (float)ii * fClkPeriod;
		Line.CalculateCoord(fPos,Cp);
		nArray[ii] = (int)(Cp.fPos[1]*10.0f);
	}
	//?????
	Pr30DacClkRate(pPr30);
	Pr30ResetMaxDacDataNumber(pPr30);
	Pr30Command41(pPr30,nArray);

}
void CUSLut::Pr30ResetMaxDacDataNumber(Pr30Data *pPr30)
{
	Pr30Command8(pPr30, 1);
}

void CUSLut::Pr30LinearTimeConstant(Pr30Data *pPr30)
{
	Pr30Command34(pPr30);

}

void CUSLut::Pr30FilterRange(Pr30Data *pPr30)
{
	Pr30Command47(pPr30);
	Pr30Command48(pPr30);
	Pr30Command49(pPr30);

}

void CUSLut::Pr30CenterFrequency(Pr30Data *pPr30)
{
	Pr30Command50(pPr30);

}

void CUSLut::Pr30PulseVoltageMSC(Pr30Data *pPr30)
{
	//msc->nAddrs
	Outp(0x10+0x0c,pPr30->nPulseVoltage);

}


void CUSLut::Pr30LogPostDetectFilter(Pr30Data *pPr30)
{
	Pr30Command32(pPr30);

}

void CUSLut::Pr30SetTx(Pr30Data *pPr30)
{
	Pr30Command4(pPr30);	//Select timeslot
	Pr30Command24(pPr30);	//Select Mux Tx
	Pr30Command25(pPr30);	//Select Mux Rx
	pPr30->nPreampWriteSelect = pPr30->nMuxTx;
	Pr30Command18(pPr30);	//Choose preamp to write to

	Pr30Command21(pPr30);
	Pr30PulseWidth(pPr30);
	Pr30Damping(pPr30);
	Pr30PreampMode(pPr30);
}

void CUSLut::Pr30SetRx(Pr30Data *pPr30)
{
	Pr30Command4(pPr30);	//Select timeslot
	Pr30Command24(pPr30);	//Select Mux Tx
	Pr30Command25(pPr30);	//Select Mux Rx

	pPr30->nPreampWriteSelect = pPr30->nMuxRx;
	Pr30Command18(pPr30);	//Choose Preamp to write to

	Pr30Command21(pPr30);
	Pr30PreampGain(pPr30);
}


void CUSLut::Pr30MuxReset(Pr30Data *pPr30)
{
	Pr30Command5(pPr30);

}

void CUSLut::Pr30MuxStart(Pr30Data *pPr30)
{

	pPr30->nMuxRun=1;
	Pr30Command7(pPr30);

}

void CUSLut::Pr30MuxHalt(Pr30Data *pPr30)
{

	pPr30->nMuxRun=0;
	Pr30Command7(pPr30);

}


void CUSLut::Pr30Timeslot(Pr30Data *pPr30)
{

	Pr30Command4(pPr30);
	Pr30Command18(pPr30);

}

void CUSLut::Pr30GetTxRx(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{

	if(nVerbose==0) {
		pBuff->Format(_T("Tx:%s Rx:%s"),m_Global.cChannelName[pPr30->nMuxTx-1],m_Global.cChannelName[pPr30->nMuxRx-1]);
	} else {
		pBuff->Format(_T("Tx Rx = Tx:%s Rx:%s"),m_Global.cChannelName[pPr30->nMuxTx-1],m_Global.cChannelName[pPr30->nMuxRx-1]);
	}
}

void CUSLut::Pr30GetPulseWidth(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{
	CString strVerbose;

	if(nVerbose==0) {
		pBuff->Format(_T("%.01f ns"),(float)pPr30->nPulseWidth/10.0);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Pulse Width";
			break;
		case 1: strVerbose.LoadString(IDS_Pulse_Width);
			break;
		}
		pBuff->Format(L"%s %.01f ns",strVerbose, (float)pPr30->nPulseWidth/10.0);
	}
}

void CUSLut::Pr30GetPulseVoltage(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{
	CString strVerbose;

	if(nVerbose==0) {
		pBuff->Format(_T("%d V"),pPr30->nPulseVoltage);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Pulse Voltage";
			break;
		case 1: strVerbose.LoadString(IDS_Pulse_Voltage);
			break;
		}
		pBuff->Format(L"%s %d V",strVerbose, pPr30->nPulseVoltage);
	}
}

void CUSLut::Pr30GetDamping(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{
	int nCode[7]={33,50,68,100,200,300,400};
	CString strVerbose;

	if(nVerbose==0) {
		pBuff->Format(_T("%d ohms"),nCode[pPr30->nDamping%7]);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Damping";
			break;
		case 1: strVerbose.LoadString(IDS_Damping);
			break;
		}
		pBuff->Format(L"%s %d ohms",strVerbose, nCode[pPr30->nDamping%7]);
	}
}

void CUSLut::Pr30GetPreampGain(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{
	int nCode[7]={-20,-10,0,10,20,30};
	CString strVerbose;

	if(nVerbose==0) {
		pBuff->Format(_T("%d dB"),nCode[pPr30->nPreampGain%6]);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Preamp Gain";
			break;
		case 1: strVerbose.LoadString(IDS_Preamp_Gain);
			break;
		}
		pBuff->Format(L"%s %d dB",strVerbose, nCode[pPr30->nPreampGain%6]);

	}
}

void CUSLut::Pr30GetMainGain(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{
	CString strVerbose;

	if(nVerbose==0) {
		pBuff->Format(_T("%.01fdB"),pPr30->fLinGain);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Main Gain";
			break;
		case 1: strVerbose.LoadString(IDS_Main_Gain);
			break;
		}
		pBuff->Format(L"%s %.01fdB",strVerbose, pPr30->fLinGain);
	}

}

void CUSLut::Pr30GetOutputModeAndGain(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{

	if(nVerbose==0) {
		switch(pPr30->nOutputSelect) {
		case 0:	pBuff->Format(_T("RF Lin %.01fdB"),pPr30->fLinGain);
			break;
		case 1:	pBuff->Format(_T("Rectified %.01fdB"),pPr30->fLinGain);
			break;
		case 2:	pBuff->Format(_T("log %.01fdB"),pPr30->fLogGain);
			break;
		}
	} else {
		switch(pPr30->nOutputSelect) {
		case 0:	pBuff->Format(_T("Main Gain RF Lin %.01fdB"),pPr30->fLinGain);
			break;
		case 1:	pBuff->Format(_T("Main Gain Rectified %.01fdB"),pPr30->fLinGain);
			break;
		case 2:	pBuff->Format(_T("Main Gain log %.01fdB"),pPr30->fLogGain);
			break;
		}
	}

}

void CUSLut::Pr30GetOutputMode(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{

	if(nVerbose==0) {
		switch(pPr30->nOutputSelect) {
		case 0:	pBuff->Format(_T("RF Linear"));
			break;
		case 1:	pBuff->Format(_T("Rectified"));
			break;
		case 2:	pBuff->Format(_T("Logorithmic"));
			break;
		}
	} else {
		switch(pPr30->nOutputSelect) {
		case 0:	pBuff->Format(_T("Main Gain RF Linear"));
			break;
		case 1:	pBuff->Format(_T("Main Gain Rectified"));
			break;
		case 2:	pBuff->Format(_T("Main Gain Logorithmic"));
			break;
		}
	}

}

void CUSLut::Pr30GetFilterRange(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{
	CString strVerbose;
	CString Code[7]={L"Wide Band",L"0.5-2MHz",L"1.0-4.0MHz",L"2.0-8.0MHz",L"4.0-16MHz",L"8-32MHz"};

	if(nVerbose==0) {
		pBuff->Format(_T("%s"),Code[pPr30->nBandPassRange]);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Filter Range";
			break;
		case 1: strVerbose.LoadString(IDS_Range);
			break;
		}
		pBuff->Format(L"%s %s",strVerbose,Code[pPr30->nBandPassRange]);
	}
}

void CUSLut::Pr30GetFilterType(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{
	CString strVerbose;
	CString Code[3];
	Code[0].LoadString(IDS_Low_Pass);
	Code[1].LoadString(IDS_Band_Pass_Narrow);
	Code[2].LoadString(IDS_Band_Pass_Wide);

	if(nVerbose==0) {
		pBuff->Format(_T("%s"),Code[pPr30->nFilterType]);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Filter Type";
			break;
		case 1: strVerbose.LoadString(IDS_Filter_Type);
			break;
		}
		pBuff->Format(L"%s %s",strVerbose, Code[pPr30->nBandPassRange]);
	}
}


void CUSLut::Pr30GetLowPassFilter(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{
	CString Code[7]={L"2.5MHz",L"5.0MHz",L"10 MHz",L"15 MHz",L"Full"};
	CString strVerbose;

	if(nVerbose==0) {
		pBuff->Format(_T("%s"),Code[pPr30->nLowPassRange%5]);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Low Pass";
			break;
		case 1: strVerbose.LoadString(IDS_Low_Pass);
			break;
		}
		pBuff->Format(L"%s %s",strVerbose, Code[pPr30->nLowPassRange%5]);
	}
}

void CUSLut::Pr30GetLinearTimeConstant(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{
	CString Code[2]={L"0.5MHz",L"1.5MHz"};
	CString strVerbose;

	if(nVerbose==0) {
		pBuff->Format(_T("%s"),Code[pPr30->nLinTimeConstant%2]);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Filter Cut Off";
			break;
		case 1: strVerbose.LoadString(IDS_Filter_Cut_Off);
			break;
		}
		pBuff->Format(L"%s %s",strVerbose, Code[pPr30->nLinTimeConstant%2]);
	}

}

void CUSLut::Pr30GetDACMode(Pr30Data *pPr30, CString *pBuff,int nVerbose, int nLanguage)
{
	CString Code[3]={L"Off",L"Main Bang",L"Interface"};
	CString strVerbose;
	int nCode = 0;

	if(nLanguage!=0) {
		Code[0].LoadString(IDS_Off);
		Code[1].LoadString(IDS_Main_Bang);
		Code[2].LoadString(IDS_Interface);
	}

	if(pPr30->nDacEnable == 0) {
		nCode = 0;
	} else {
		if(pPr30->nDacTriggerMode == 0) {
			nCode = 1;
		} else {
			nCode = 2;
		}
	}

	if(nVerbose==0) {
		pBuff->Format(_T("%s"),Code[nCode%3]);
	} else {
		switch(nLanguage) {
		default: strVerbose = "DAC Mode";
			break;
		case 1: strVerbose.LoadString(IDS_DAC_Mode);
			break;
		}
		pBuff->Format(L"%s %s",strVerbose, Code[nCode%3]);
	}

}

void CUSLut::Pr30SelfTestOnOff(Pr30Data *pPr)
{
	

//	pPr->nSelfTestOffOn = theApp.m_LastSettings.nPr30SelfTestOffOn;
	Pr30Command53(pPr);
}

void CUSLut::Pr30SelfTestFrequency(Pr30Data *pPr)
{
	Pr30Command54(pPr);
}

void CUSLut::Pr30SelfTestAmplitude(Pr30Data *pPr)
{
	Pr30Command55(pPr);
}


void CUSLut::Pr30UmbilicalSelect(Pr30Data *pPr, int nChannel)
{
	pPr->nI2CUmbilicalSelect = nChannel;
	pPr->nRxRfUmbilicalSelect = nChannel;

	Pr30Command4(pPr);	//Write timeslot start at zero
	Pr30Command68(pPr);
	Pr30Command69(pPr);
}

void CUSLut::Pr30UmbilicalSelect(int nChannel)
{
	for(int nTS=0;nTS<256;nTS++) {
		m_TS[nTS].Pr30.nI2CUmbilicalSelect = nChannel;
		m_TS[nTS].Pr30.nRxRfUmbilicalSelect = nChannel;
	}

	Pr30Command4(&m_TS[0].Pr30);	//Write timeslot start at zero
	Pr30Command68(&m_TS[0].Pr30);
	Pr30Command69(&m_TS[0].Pr30);
}

///////////////////////////////////////////////////////////////////////
//PR30 finished
/////////////////////////////////////////////////////////////////////


void CUSLut::Adc200MuxStart(ADC200Data *adc)
{

	adc->mux_run_mode=1;
	Adc200Command0(adc);
}

void CUSLut::Adc200MuxHalt(ADC200Data *adc)
{

	adc->mux_run_mode=0;
	Adc200Command0(adc);
}




void CUSLut::SelectWriteTimeslot()
{
	

//	m_TS[m_Global.nTimeSlot].Adc.nConvertTimeslot=m_Global.nTimeSlot;
//	m_TS[m_Global.nTimeSlot].Adc.nTimeslot=m_Global.nTimeSlot;

	if(theApp.m_LastSettings.bPr15Present) Pr15Timeslot(&m_TS[m_Global.nTimeSlot].Pr15);
	if(theApp.m_LastSettings.bPl15Present) Pl15Timeslot(&m_TS[m_Global.nTimeSlot].Pl15);
	if(theApp.m_LastSettings.bPr30Present) Pr30Timeslot(&m_TS[m_Global.nTimeSlot].Pr30);

//	Adc200ConvertTimeslot(&m_TS[m_Global.nTimeSlot].Adc);
//	Adc200WriteTimeslot(&m_TS[m_Global.nTimeSlot].Adc,0);
//	Dsp200Timeslot(&m_TS[m_Global.nTimeSlot].Dsp);
}

void CUSLut::RefreshAllVariables(int nJobNumber)
{
	
	int	nTS;
	InitializeAddrs();
	if(m_Pm30CommDevice==NONE) return;

	if(m_Global.Mux.nMaxTimeslots<=0)m_Global.Mux.nMaxTimeslots=1;
	if(m_Global.Mux.nMaxTimeslots>8)m_Global.Mux.nMaxTimeslots=8;
	MinMax(&m_Global.nTimeSlot, 0, 7);
	int	nOldTimeSlot = m_Global.nTimeSlot;

	switch(m_Global.nTriggerSource) {
	case USL_PR20:
	case USL_PR30:
	case USL_ADC100:
	case USL_DSP200:
	case USL_UP15:
	case USL_SI10:
	case USL_PR40:
		break;
	default:m_Global.nTriggerSource = USL_PR30;
		break;
	}

	Pr30ResetMaxTimeSlots(&m_TS[0].Pr30);

	if(m_pActiveTimeSlots) m_pActiveTimeSlots->Clear(); //Clear the time slot data wrapper
	for(nTS=0;nTS<m_Global.Mux.nMaxTimeslots;nTS++) {
		if(m_Global.nTriggerSource==USL_PR30 || m_Global.nTriggerSource == USL_PR40)
			m_TS[nTS].Pr30.nPrfSrc=0;	
		else
			m_TS[nTS].Pr30.nPrfSrc=1;
	}


	for(nTS=0;nTS<m_Global.Mux.nMaxTimeslots;nTS++) {
		m_TS[nTS].Pr30.nTimeSlotSyncSrc=0;
		//PM30 isn't aware of LSA Timeslots
		if((m_Global.Mux.nMaxTimeslots==1) || (theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30] == LSA) ){
			m_TS[nTS].Pr30.nWriteTimeSlot=1;
			m_TS[nTS].Pr30.nPreampWriteSelect=1;
			m_TS[nTS].Pr30.nMuxRun=0;
			if(theApp.m_LastSettings.bPr30Present[theApp.m_LastSettings.nUseWhichPR30]==TRUE) {
				if (m_pActiveTimeSlots) m_pActiveTimeSlots->addPr30(); //Append the pr30 time slot
				Pr30Initialize(&m_TS[0].Pr30);
				Pr30CalculateDacData(&m_TS[0].Pr30);
			}
		} else {
			m_TS[nTS].Pr30.nWriteTimeSlot=nTS+1;
			m_TS[nTS].Pr30.nPreampWriteSelect=nTS+1;
			m_TS[nTS].Pr30.nMuxRun=1;
			if(theApp.m_LastSettings.bPr30Present[theApp.m_LastSettings.nUseWhichPR30]==TRUE) {
				if(m_pActiveTimeSlots) m_pActiveTimeSlots->addPr30(); //Append the pr30 time slot
				Pr30Initialize(&m_TS[nTS].Pr30);
				Pr30CalculateDacData(&m_TS[nTS].Pr30);
			}
		}
	
	};
	m_Global.nTimeSlot=nOldTimeSlot;

	SelectWriteTimeslot();
	MuxRun();
	SetPrf();
	if(m_Global.Mux.nMaxTimeslots>1) {
		Pr30MuxStart(&m_TS[0].Pr30);
	}


}

void CUSLut::MuxRun()
{
	

	if(m_Global.Mux.nMaxTimeslots<=0) {
		MuxDefaults();
	};
	m_Global.Mux.nState = MUX;
}


void CUSLut::MuxHalt()
{
	

//	DisableMuxInterrupt();
//	Outp(theApp.m_LastSettings.nMailBoxAddrs+1,0x00);
	m_Global.Mux.nState = NON_MUX;
}


void CUSLut::MuxReset()
{

//	Pr15MuxReset(&m_TS[0].Pr15);
}


void CUSLut::ProcessTrace(char *trace,int nSlot)
{
	

	ProcessSignedTrace(trace,nSlot);

//	if((theApp.m_bCollectComplexData==TRUE) && (theApp.m_pScanData!=NULL)) {
//		theApp.m_pScanData->CollectComplexData();
//	}
/*
	CollectData(nSlot, theApp.m_Si10User.GetFieldBusRegister(), theApp.m_Si10User.m_Si10.nGPIOIrqCount);
	*/
}


void CUSLut::ProcessUnSignedTrace(unsigned char *trace,int nSlot)
{
	
	DSP200Data* pDsp = &m_TS[nSlot].Dsp;
	GatesData* pGates = &m_TS[nSlot].Gate;

	int nInterfacePos = -1;
	int nStopPos=-1;
	int	ii,jj,gg;
	unsigned char cThreshold;
	unsigned char cMax[8];
	int	nDelay,nFinish;
	ZeroMemory(cMax,sizeof cMax);


	cThreshold=(unsigned char)pDsp->InterfaceThreshold[0] + 128;
	if(pDsp->SignDetect[0]&1) cThreshold*=-1;
	nDelay=pDsp->Delay[0];
	if(nDelay<0) nDelay=0;
	if(nDelay>8192) nDelay = 8191;
	for(ii=nDelay,jj=0;jj<pDsp->Width[0];ii++,jj++) {
		if(nInterfacePos==-1) {	//Find interface pos
			if(!pDsp->SignDetect[0]) {
				if(trace[ii]>=cThreshold) nInterfacePos = ii;
			} else {
				if(trace[ii]<=cThreshold) nInterfacePos = ii;
			}
		}
		if(!pDsp->SignDetect) {
			if(trace[ii]<=cMax[0]) cMax[0] = trace[ii];
		} else {
			if(trace[ii]>=cMax[0]) cMax[0] = trace[ii];
		}
	}
	pDsp->InterfacePos=nInterfacePos;
	pDsp->TimsStartPos[0]=nInterfacePos;
	pGates->nTimeSample[0]=nInterfacePos;
	pGates->nAmplitude[0]=cMax[0]-128;


	for(gg=1;gg<8;gg++) {
		nStopPos=-1;
		switch(pDsp->TriggerMode[gg]) {
		case 0:
			break;
		case 1:
			nDelay=pDsp->Delay[gg];
			if(nDelay<0) nDelay=0;
			if(nDelay>8191) nDelay = 8191;
			break;
		case 2:
			nDelay=pDsp->Delay[gg] + nInterfacePos;
			if(nDelay<0) nDelay=0;
			if(nDelay>8191) nDelay = 8191;
			break;
		}
		if(pDsp->TriggerMode[gg]>0) {
			nFinish = pDsp->Width[gg] + nDelay;
			if(nFinish>8191) nFinish = 8191;
			cThreshold=(char)pDsp->InterfaceThreshold[gg] +128;
			if(pDsp->SignDetect[gg]&1) cThreshold*=-1;
			if(!pDsp->SignDetect[gg]) {
				for(ii=nDelay;ii<=nFinish;ii++) {
					if(trace[ii]>=cMax[gg]) cMax[gg] = trace[ii];
				}
			} else {
				for(ii=nDelay;ii<=nFinish&&(nStopPos==-1);ii++) {
					if(trace[ii]<=cMax[gg]) cMax[gg] = trace[ii];
				}
			}
			//Tims
			if(gg>=1) {
				switch(pGates->nTimsDetectionMode[gg]) {
				case 1:
				case 2:
					for(ii=nDelay;ii<=nFinish;ii++) {
						switch(pGates->nTimsDetectionMode[gg]) {
						case 1:	if((nStopPos==-1) && (trace[ii]>=cThreshold)) nStopPos = ii;
							break;
						case 2:	if(trace[ii]>=cMax[gg]) nStopPos = ii;
							break;
						}
					}
					break;
				case 3:
				case 4:
					for(ii=nFinish;ii>=nDelay;ii--) {
						switch(pGates->nTimsDetectionMode[gg]) {
						case 3:	if((nStopPos==-1) && (trace[ii]>=cThreshold)) nStopPos = ii;
							break;
						case 4:	if(trace[ii]>=cMax[gg]) nStopPos = ii;
							break;
						}
					}
					break;
				}
				pDsp->TimsStopPos[0]=nStopPos;
			}
		}
		pGates->nTimeSample[gg]=nStopPos;
		pGates->nAmplitude[gg]=cMax[gg]-128;
	}

}

int CUSLut::GetAverageAmplitude(int nTS, int nGate)
{

	int nTotal = 0;
	int ii;
	for(ii=0;ii<1024;ii++) {
		nTotal += m_nGateAmp[ii][nTS][nGate];
	}
	nTotal /= 1024;
	return nTotal;
}

void CUSLut::GetAllUt(CCoord *pCp)
{
	int	ii;

	for(ii=0;ii<2;ii++) {
		pCp->nGain[ii] = m_TS[ii].Pr30.nLinGain;
	}

	for(ii=0;ii<6;ii++) {
		pCp->nDelay[ii] = m_TS[0].Gate.nNsDelay[ii];
		pCp->nWidth[ii] = m_TS[0].Gate.nNsWidth[ii];
	}
}

void CUSLut::GetGatePos(COORDDATA *pCp)
{
	int	ii;

	for(ii=0;ii<6;ii++) {
		pCp->nDelay[ii] = m_TS[0].Gate.nNsDelay[ii];
		pCp->nWidth[ii] = m_TS[0].Gate.nNsWidth[ii];
	}
}

void CUSLut::SetGatesFromCoordinate(COORDDATA Cp)
{
	int	ii;

	for(ii=0;ii<6;ii++) {
		m_TS[m_Global.nTimeSlot].Gate.nNsDelay[ii] = Cp.nDelay[ii];
		m_TS[m_Global.nTimeSlot].Gate.nNsWidth[ii] = Cp.nWidth[ii];
		Dsp200CalculateHardware(0,ii);
	}
}

void CUSLut::SetAllUt(CCoord Cp, int *pHeader)
{
	int ii;

	for(ii=0;ii<2;ii++) {
		if(pHeader[H_GAIN0 + ii] == 1) {
			if(m_TS[ii].Pr30.nLinGain - Cp.nGain[ii]) {
				m_TS[ii].Pr30.nLinGain = Cp.nGain[ii];
				m_TS[ii].Pr30.fLinGain = (float)m_TS[ii].Pr30.nLinGain / 10.0f;
//				Pr30Timeslot(&m_TS[ii].Pr30);
//				Pr30LinFloatGain(&m_TS[ii].Pr30);
			}
		}
	}
	for(ii=0;ii<6;ii++) {
		if(pHeader[H_DELAY0 + ii] == 1) m_TS[0].Gate.nNsDelay[ii] = Cp.nDelay[ii];
		if(pHeader[H_WIDTH0 + ii] == 1) m_TS[0].Gate.nNsWidth[ii] = Cp.nWidth[ii];
		Dsp200CalculateHardware(0,ii);
	}

}

void CUSLut::EnableMailBoxTrigger(bool bFlag)
{

	if(bFlag==FALSE) {
		Outp(MailboxAddrs+2,0);
	} else {
		Outp(MailboxAddrs+2,0xff);
	}
}

void CUSLut::ResetMaxTimeSlots()
{
	

	Dsp200StopProcessing(&m_TS[0].Dsp);
	if(theApp.m_LastSettings.bPl15Present==TRUE)	Pl15MuxReset(&m_TS[0].Pl15);
	if(theApp.m_LastSettings.bPr15Present==TRUE)	Pr15MuxReset(&m_TS[0].Pr15);
	if(theApp.m_LastSettings.bPr30Present[theApp.m_LastSettings.nUseWhichPR30]==TRUE)	Pr30ResetMaxTimeSlots(&m_TS[0].Pr30);
	if(theApp.m_LastSettings.bDspPresent==TRUE) Dsp200Reset(&m_TS[0].Dsp);
	Adc200ResetMaxTimeSlot(&m_TS[0].Adc);

}

float CUSLut::GetGateWaterPath(int nTS)
{
	
	float fMul;

	if(m_TS[nTS].Pr30.nMuxTx == m_TS[nTS].Pr30.nMuxRx) {
		if(m_TS[nTS].Pr30.nPreampMode == 0) {
			fMul = 0.5e-6f;
		} else {
			fMul = 1.0e-6f;
		}
	} else {
		fMul = 1e-6f;
	}

	nTS%=256;
	m_TS[nTS].Gate.nTOFStopGate[0]%=8;
	float fPath = (((float)m_TS[nTS].Gate.nTimeSample[m_TS[nTS].Gate.nTOFStopGate[0]] * m_TS[nTS].Adc.fSamplePeriod) + m_TS[nTS].Adc.nDelay) * (float)theApp.m_LastSettings.nWaterVelocity * fMul;

	if(m_TS[nTS].Gate.nTimeStatus[0] == 0) {
		fPath = 0.0f;
	}
	return fPath;

}

int CUSLut::GetGateWaterTimeNs(int nTS)
{
	

	nTS%=256;
	m_TS[nTS].Gate.nTOFStopGate[0]%=8;
	int nTime = (int)((float)m_TS[nTS].Gate.nTimeSample[m_TS[nTS].Gate.nTOFStopGate[0]] * m_TS[nTS].Adc.fSamplePeriod + m_TS[nTS].Adc.nDelay);

	return nTime;

}

float CUSLut::GetGateThickness(int nTS, int nTOF)
{
	
	float fMul,fPath;
	GatesData *pGate = &m_TS[nTS].Gate;

	if(m_TS[nTS].Pr30.nMuxTx == m_TS[nTS].Pr30.nMuxRx) {
		if(m_TS[nTS].Pr30.nPreampMode == 0) {
			fMul = 0.5e-6f;
		} else {
			fMul = 1.0e-6f;
		}
	} else {
		fMul = 1.0f;
	}

	int nGStop = pGate->nTOFStopGate[nTOF];
	int nGStart = pGate->nTOFStartGate[nTOF];
	if(nGStart<0) nGStart=0;
	if(nGStart>7) nGStart=7;
	if(nGStop<0) nGStop=0;
	if(nGStop>8) nGStop=0;
	if(nGStop==0) {
		fPath = theApp.TimeToThickness((int)((float)pGate->nTimeSample[nGStop] * m_TS[nTS].Adc.fSamplePeriod)) * fMul;
	} else {
		fPath = theApp.TimeToThickness((int)((float)(pGate->nTimeSample[nGStop] - pGate->nTimeSample[nGStart-1]) * m_TS[nTS].Adc.fSamplePeriod)) * fMul;
	}
	return fPath;

}

float CUSLut::GetVerboseGateThickness(int nTS, int nTOF,int *pnInterface,int *pnBackWall,int *pnConfidence)
{
	
	float fMul,fPath;
	GatesData *pGate = &m_TS[nTS].Gate;

	if(m_TS[nTS].Pr30.nMuxTx == m_TS[nTS].Pr30.nMuxRx) {
		if(m_TS[nTS].Pr30.nPreampMode == 0) {
			fMul = 0.5e-6f;
		} else {
			fMul = 1.0e-6f;
		}
	} else {
		fMul = 1.0f;
	}

	int nGStop = pGate->nTOFStopGate[nTOF];
	int nGStart = pGate->nTOFStartGate[nTOF];
	if(nGStart<0) nGStart=0;
	if(nGStart>7) nGStart=7;
	if(nGStop<0) nGStop=0;
	if(nGStop>8) nGStop=0;
	if(nGStop==0) {
		fPath = theApp.TimeToThickness((int)((float)pGate->nTimeSample[nGStop] * m_TS[nTS].Adc.fSamplePeriod)) * fMul;
	} else {
		fPath = theApp.TimeToThickness((int)((float)(pGate->nTimeSample[nGStop] - pGate->nTimeSample[nGStart-1]) * m_TS[nTS].Adc.fSamplePeriod)) * fMul;
	}
	*pnConfidence = 100;
	*pnInterface = 	pGate->nTimeStatus[nGStart-1];
	*pnBackWall =  	pGate->nTimeStatus[nGStop];

	if(*pnInterface == 0) {
		*pnConfidence = 0;
	}
	if(*pnBackWall == 0) {
		*pnConfidence = 0;
	}

	
	return fPath;
}


int CUSLut::GetGateMaterialTimeNs(int nTS, int nTOF)
{
	
	int nTimeNs;
	GatesData *pGate = &m_TS[nTS].Gate;

	int nGStop = pGate->nTOFStopGate[nTOF];
	int nGStart = pGate->nTOFStartGate[nTOF];
	if(nGStart<0) nGStart=0;
	if(nGStart>7) nGStart=7;
	if(nGStop<0) nGStop=0;
	if(nGStop>8) nGStop=0;
	if(nGStop==0) {
		nTimeNs = (int)((float)pGate->nTimeSample[nGStop] * m_TS[nTS].Adc.fSamplePeriod);
	} else {
		nTimeNs = (int)((float)(pGate->nTimeSample[nGStop] - pGate->nTimeSample[nGStart-1]) * m_TS[nTS].Adc.fSamplePeriod);
	}
	return nTimeNs;

}

int CUSLut::GetVerboseGateMaterialTimeNs(int nTS, int nTOF,int *pnInterface,int *pnBackWall,int *pnConfidence)
{
	
	int nTimeNs;
	GatesData *pGate = &m_TS[nTS].Gate;

	int nGStop = pGate->nTOFStopGate[nTOF];
	int nGStart = pGate->nTOFStartGate[nTOF];
	if(nGStart<0) nGStart=0;
	if(nGStart>7) nGStart=7;
	if(nGStop<0) nGStop=0;
	if(nGStop>8) nGStop=0;
	if(nGStop==0) {
		nTimeNs = (int)((float)pGate->nTimeSample[nGStop] * m_TS[nTS].Adc.fSamplePeriod);
	} else {
		nTimeNs = (int)((float)(pGate->nTimeSample[nGStop] - pGate->nTimeSample[nGStart-1]) * m_TS[nTS].Adc.fSamplePeriod);
	}
	*pnConfidence = 100;
	*pnInterface = 	pGate->nTimeStatus[nGStart-1];
	*pnBackWall =  	pGate->nTimeStatus[nGStop];

	if(*pnInterface == 0) {
		*pnConfidence = 0;
	}
	if(*pnBackWall == 0) {
		*pnConfidence = 0;
	}

	return nTimeNs;

}

void CUSLut::CopyTS(int nTSDest, int nTSSrc)
{
	m_TS[nTSDest] = m_TS[nTSSrc];

}


bool CUSLut::HasInterfaceFailed()
{
	if(m_Global.nPrf < m_nInterfaceFailureCount) {
		return TRUE;
	} else {
		return FALSE;
	}
}

int CUSLut::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

long CUSLut::MinMax(long *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}


void CUSLut::ArmPrf()
{
	

	switch(m_Pm30CommDevice) {
	case SI10_I2C:
		theApp.m_Si10User.Si10ArmPRF();
		break;
	case USLDRIVER_I2C:
		Outp(theApp.m_UtUser.MailboxAddrs+3,1);
		break;
	}
}



int CUSLut::ReadTS0()
{
	

	switch(m_Pm30CommDevice) {
	case SI10_I2C:
		return theApp.m_Si10User.Si10ReadTS0();
		break;
	case USB_I2C:
		return theApp.m_USBtoI2C.ReadTS0();
		break;
	case USLDRIVER_I2C:
		return Inp(theApp.m_UtUser.MailboxAddrs+2)&1;
		break;
	}
	return 0;

}


void CUSLut::TurntableGainChange(CCoord Cp)
{

	Cp.nTS = MinMax(&Cp.nTS,0,7);
	if(m_TS[Cp.nTS].Pr30.nExtraLinGain != Cp.nGain[0]) {

		m_TS[Cp.nTS].Pr30.nLinGain = (int)(m_TS[Cp.nTS].Pr30.fLinGain*10.0f) + Cp.nGain[0];
		if(m_TS[Cp.nTS].Pr30.nLinGain<0) m_TS[Cp.nTS].Pr30.nLinGain=0;

		Pr30Timeslot(&m_TS[Cp.nTS].Pr30);
		Pr30Command33(&m_TS[Cp.nTS].Pr30);

		m_TS[Cp.nTS].Pr30.nExtraLinGain = Cp.nGain[0];
	}

}


bool CUSLut::EthernetConnect()
{

	if(m_Ethernet.Connected() == false) {
		m_Ethernet.Connect((CWnd*)this,true,USLutEthernetCallback);
	}

	return true;
}

bool CUSLut::SetupOutputDeviceSettings()
{	
	
	int ii;
	WCHAR str[4];

	switch(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]) {
	default:
	case PPA15:
	case MUX8:
		//This is here so when we change from LSA back to MUX we aren't left with a large number of Probes per image
		//I realise that this may be overwriting correct data.
		//Needs to be resolved TODO
		for(ii=0;ii<256;ii++){
			//ScanLine
			m_TS[ii].nScanLine = 0;
			PROFILE->m_nNumberProbesPerImage = 1;	
		}
		break;
	case LSA:
		for(ii=0;ii<256;ii++){
			//TS name
			swprintf_s(str,_T("%d\0"),ii+1);
			wcscpy_s(m_TS[ii].cName,str);
			//ScanLine
			m_TS[ii].nScanLine = ii;
			PROFILE->m_nNumberProbesPerImage = m_Global.LSA.nFLS;	
		}
	break;
	}

	return true;
}

struct UT_VERBOSE_THICKNESS {
	float fThickness;
	bool bInterfaceStatus;
	bool bBackWallStatus;
	float fReliablity;
};


long USLutEthernetCallback(CWnd* pParent, int nDataSize, char* Data)
{
	
	UT_ETHERNET_MSG			rx_msg;
	UT_RETURN_ETHERNET_MSG	tx_msg;
	UT_RETURN_ETHERNET_MSG_ASCII	tx_msg_ascii;
	float *pfThickness;
	int	*pnNumber;
	UT_VERBOSE_THICKNESS *pVerbose;
	CCoord Cp;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int	nCommsMode = 0;
	int	nInterfaceStatus = 1;
	int	nBackWallStatus = 1;
	int nConfidence = 68;
	int nInstruction;
	int	nFBH,nStatus;
	char strBlockName[100];
	char strTemp[100];
	CString Buff,Temp;
	int ii,nn,mm;
	int nPulserOffOn;
	float fFlowRate = 1.23f;

	float fThickness = theApp.m_UtUser.GetVerboseGateThickness(0,1,&nInterfaceStatus,&nBackWallStatus,&nConfidence);



	memcpy(rx_msg.AllData,Data,nDataSize);
	ZeroMemory((void*)&tx_msg,sizeof tx_msg);

	if(nCommsMode==0) {	//Ascii
		nStatus = 0;
		ZeroMemory(&tx_msg_ascii,sizeof tx_msg_ascii);

		sscanf_s(Data,"%d",&nInstruction);

		switch(nInstruction) {
		case 0: //handshake
			break;
		default:	//Unrecognised command
			sprintf_s(tx_msg_ascii.cStr,"299 -1");
			break;
		case 100:	//Get thickness
			sprintf_s(tx_msg_ascii.cStr,"200 %d %.03f",nStatus, fThickness);
			break;
		case 101:	//Get verbose thickness
			sprintf_s(tx_msg_ascii.cStr,"201 %d %.03f %d %d %d",nStatus ,fThickness,nInterfaceStatus,nBackWallStatus,nConfidence);
			break;
		case 102:
			theApp.m_ThickCal.Zero();
			Buff.Empty();
			Buff.GetBufferSetLength(strlen(Data));
			for(ii=0;ii<(int)strlen(Data);ii++) {
				Buff.SetAt(ii,Data[ii]);
			}
			nn=Buff.Find(_T(" "),0)+1;
			mm=Buff.Find(_T(" "),nn);
			Temp = Buff.Mid(nn,mm-nn);
			nFBH = _ttoi(Temp);
			nn=mm+1;

			Cp.Zero();
			for(ii=0;ii<nFBH-1;ii++) {
				mm=Buff.Find(L" ",nn);
				Temp = Buff.Mid(nn,mm-nn);
				_WTOF(Temp,Cp.fNomThickness);
				nn=mm+1;
				theApp.m_ThickCal.Add(Cp);
			}
			Temp = Buff.Right(Buff.GetLength()-nn);
			_WTOF(Temp,Cp.fNomThickness);
			theApp.m_ThickCal.Add(Cp);

			theApp.ComputeVelocityTable();
			if(pFrame->m_pMotionToolsSheet) {
				pFrame->m_pMotionToolsSheet->UpdateAllPages();
			}
			sprintf_s(tx_msg_ascii.cStr,"202 %d",nStatus);
			break;
		case 103:
			sscanf_s(Data,"%d %d",&nInstruction,&nFBH);
			if(nFBH>=0 && nFBH < theApp.m_ThickCal.m_nCoordL) {
				theApp.m_ThickCal.m_pCp[nFBH].nTimens = theApp.m_UtUser.GetVerboseGateMaterialTimeNs(0,1,&nInterfaceStatus,&nBackWallStatus,&nConfidence);
				if(theApp.m_ThickCal.m_pCp[nFBH].nTimens<0) theApp.m_ThickCal.m_pCp[nFBH].nTimens=0;
				theApp.ComputeVelocityTable();
				if(pFrame->m_pMotionToolsSheet) {
					pFrame->m_pMotionToolsSheet->UpdateAllPages();
				}
			} else {
				nStatus=-1;
			}
			sprintf_s(tx_msg_ascii.cStr,"203 %d",nStatus);
			break;
		case 104: // report calibration values
			nn=theApp.m_ThicknessCalibrationPathName.ReverseFind('\\');
			Buff = theApp.m_ThicknessCalibrationPathName.Right(theApp.m_ThicknessCalibrationPathName.GetLength()-nn-1);
			if((nn=Buff.Find(L"."))>0) {
				Buff.GetBufferSetLength(nn);
			};
			for(ii=0;ii<Buff.GetLength();ii++) {
				strBlockName[ii] = (char)Buff.GetAt(ii);
			};
			strBlockName[ii++]=0;

			nStatus = 0;
			for(ii=0;ii<theApp.m_ThickCal.GetSize();ii++) {
				theApp.m_ThickCal.GetCoord(ii,&Cp);
				if((Cp.nMinTimens<=Cp.nTimens) && (Cp.nTimens<=Cp.nMaxTimens)) {

				} else {
					nStatus = -1;
				}
			}

			sprintf_s(tx_msg_ascii.cStr,"204 %d %s %d",nStatus , strBlockName, theApp.m_ThickCal.GetSize());
			for(ii=0;ii<theApp.m_ThickCal.GetSize();ii++) {
				theApp.m_ThickCal.GetCoord(ii,&Cp);
				sprintf_s(strTemp," %.03f",(float)Cp.nTimens / 1000.0f);
				strcat_s(tx_msg_ascii.cStr, strTemp);
			}
			break;
		case 105:
			sscanf_s(Data,"%d %d",&nInstruction,&nPulserOffOn);
			pFrame->HTOffOn(nPulserOffOn);
			sprintf_s(tx_msg_ascii.cStr,"205 %d",nStatus);
			break;

		case 106:
			sprintf_s(tx_msg_ascii.cStr,"206 %d %.03f",nStatus, fFlowRate);
			break;
		}
		theApp.m_UtUser.m_Ethernet.SendMessage(strlen(tx_msg_ascii.cStr)+1,tx_msg_ascii.cStr);

	} else {

		switch(rx_msg.Instruction) {
		case 0: //handshake
			break;
		default:
			tx_msg.Instruction=299;
			tx_msg.nStatus=-1;
			tx_msg.nDataSize=0;
			theApp.m_UtUser.m_Ethernet.SendMessage(tx_msg.nDataSize+12,tx_msg.AllData);
			break;
		case 100:
			tx_msg.Instruction=200;
			tx_msg.nStatus=0;
			tx_msg.nDataSize=4;
			pfThickness = (float *) &tx_msg.Data;
			*pfThickness = 1.245f;
			theApp.m_UtUser.m_Ethernet.SendMessage(tx_msg.nDataSize+12,tx_msg.AllData);
			break;
		case 101:
			tx_msg.Instruction=201;
			tx_msg.nStatus=0;
			tx_msg.nDataSize=sizeof UT_VERBOSE_THICKNESS;
			pVerbose = (UT_VERBOSE_THICKNESS *) &tx_msg.Data;
			pVerbose->fThickness = 2.362f;
			pVerbose->bInterfaceStatus=true;
			pVerbose->bBackWallStatus=false;
			pVerbose->fReliablity=68.2f;
			theApp.m_UtUser.m_Ethernet.SendMessage(tx_msg.nDataSize+12,tx_msg.AllData);
			break;
		case 102:
			pnNumber = (int *)&rx_msg.Data;
			pfThickness = (float *)&rx_msg.Data[4];

			theApp.m_ThickCal.Zero();
			Cp.Zero();
			for(ii=0;ii<*pnNumber;ii++) {
				Cp.fNomThickness = pfThickness[ii];
				theApp.m_ThickCal.Add(Cp);
			};
			if(pFrame->m_pMotionToolsSheet) {
				pFrame->m_pMotionToolsSheet->UpdateAllPages();
			}

			tx_msg.Instruction=202;
			tx_msg.nStatus=0;
			tx_msg.nDataSize=0;
			theApp.m_UtUser.m_Ethernet.SendMessage(tx_msg.nDataSize+12,tx_msg.AllData);
			break;
		case 103:
			pnNumber = (int *)&rx_msg.Data;
			if(*pnNumber>=0 && *pnNumber < theApp.m_ThickCal.m_nCoordL) {
				tx_msg.nStatus=0;
				theApp.m_ThickCal.m_pCp[*pnNumber].nTimens = theApp.m_UtUser.GetGateMaterialTimeNs(0,1);
				if(pFrame->m_pMotionToolsSheet) {
					pFrame->m_pMotionToolsSheet->UpdateAllPages();
				}
			} else {
				tx_msg.nStatus=-1;
			}

			tx_msg.Instruction=203;
			tx_msg.nDataSize=0;
			theApp.m_UtUser.m_Ethernet.SendMessage(tx_msg.nDataSize+12,tx_msg.AllData);
			break;
		}
	}
	return 0;

}

//********************************************************
//Remote Interface
CString CUSLut::strGetLogGain(int nTimeSlot)
{
	CString Buff;

	Buff.Format(L"%.01f",m_TS[nTimeSlot].Pr30.fLogGain);

	return Buff;
}

CString CUSLut::strGetPreAmpGain(int nTimeSlot)
{
	CString Buff;

	switch(m_TS[nTimeSlot].Pr30.nPreampGain) {
	case 0: Buff = "-20";
		break;
	case 1: Buff = "-10";
		break;
	case 2: Buff = "0";
		break;
	case 3: Buff = "10";
		break;
	case 4: Buff = "20";
		break;
	}

	return Buff;
}

CString CUSLut::strGetAmplifier(int nTimeSlot)
{
	CString Buff;

	switch(m_TS[nTimeSlot].Pr30.nOutputSelect) {
	case 0: Buff = "RF";
		break;
	case 1: Buff = "Rectified";
		break;
	case 2: Buff = "Logorithmic";
		break;
	}
	return Buff;
}


////////////////////////////////////////////////////////////////////////////////
// Collection stuff
//////////////////////////////////////////////////////////////////////////////////

void CUSLut::AllocateDataCollection()
{
	m_nCollectPtr = 0;
	m_nCollectL[0] = 0;
	m_nCollectL[1] = 0;
	SAFE_DELETE( m_pCollect[0] );
	SAFE_DELETE( m_pCollect[1] );

	m_pCollect[0] = new TimeSlotDataCollectionStruct[ COLLECT_SIZE ];
	m_pCollect[1] = new TimeSlotDataCollectionStruct[ COLLECT_SIZE ];
	

}


void CUSLut::CollectData(int nSlot, int nFieldBusInputReg, int nFieldBusCount)
{
	int nGStop,nGStart,tt,nNsPath;
	


	switch(m_nCollectMode) {
	default:	//Dont Collect
		break;
	case 1:		//Collect until full
		if(nFieldBusInputReg & theApp.m_Tank.nRobotInCollectDataBit) {
			if(m_nCollectPtr < COLLECT_SIZE) {

				m_pCollect[theApp.m_Thread.m_nDir][m_nCollectPtr].nFieldBusInputReg = nFieldBusInputReg;
				m_pCollect[theApp.m_Thread.m_nDir][m_nCollectPtr].nFieldBusCount = nFieldBusCount;

				CopyMemory(m_pCollect[theApp.m_Thread.m_nDir][m_nCollectPtr].TS[nSlot].nAmplitude, m_TS[nSlot].Gate.nAmplitude, sizeof m_TS[nSlot].Gate.nAmplitude);

				for(tt=0;tt<8;tt++) {
					if(m_TS[nSlot].Gate.nCollectTime[tt]>0) {
						nGStop = m_TS[nSlot].Gate.nTOFStopGate[tt];
						nGStart = m_TS[nSlot].Gate.nTOFStartGate[tt];
						nNsPath = (int)((float)(m_TS[nSlot].Gate.nTimeSample[nGStop] - m_TS[nSlot].Gate.nTimeSample[nGStart-1]) * m_TS[nSlot].Adc.fSamplePeriod);
		
						m_pCollect[theApp.m_Thread.m_nDir][m_nCollectPtr].TS[nSlot].nNsPath[tt] = nNsPath;
					}
				}
			}
			if((nSlot + 1) >= theApp.m_UtUser.m_Global.Mux.nMaxTimeslots) {
				if(m_nCollectPtr < COLLECT_SIZE) m_nCollectPtr++;
			}
			if(m_nCollectL[theApp.m_Thread.m_nDir]< m_nCollectPtr) m_nCollectL[theApp.m_Thread.m_nDir] = m_nCollectPtr;
		}
		break;
	}

}

bool CUSLut::GetNearestCollectValue(int nDir, int nFieldBusCount,struct TimeSlotDataCollectionStruct *TSCollect,int *nPtr)
{
	if(m_pCollect[nDir] == NULL) return false;
	
	int nStartFieldBusCount = m_pCollect[nDir][0].nFieldBusCount;
	int k,kLo,kHi;

	k = kLo = 0;
	kHi = m_nCollectL[nDir]-1;

	while((kHi-kLo)>1) {
		k=(kHi+kLo)/2;
		((m_pCollect[nDir][k].nFieldBusCount - nStartFieldBusCount)>nFieldBusCount)?(kHi=k):(kLo=k);
	}
	*TSCollect = m_pCollect[nDir][k];
	if(nPtr) *nPtr = k;

	return true;
}

void CUSLut::Collect(int nCollectMode)
{
	

	if(m_pCollect[theApp.m_Thread.m_nDir] == NULL) AllocateDataCollection();

	switch(nCollectMode) {
	default:
		theApp.m_bCollectComplexData = false;
		break;
	case COLLECT_UNTIL_FULL:
		m_nCollectPtr = 0;
		m_nCollectL[theApp.m_Thread.m_nDir] = 0;
		theApp.m_bCollectComplexData = true;
		break;
	}
	m_nCollectMode = nCollectMode;
}

int CUSLut::GetCollectPtr()
{
	return m_nCollectPtr;
}

int CUSLut::CollectGetSize(int nDir)
{
	return m_nCollectL[nDir];
}

void CUSLut::GetCollect(int nDir,int nIndex,struct TimeSlotDataCollectionStruct *pTSCollect)
{

	if((nIndex>=0) && (nIndex<m_nCollectL[nDir])) {
		*pTSCollect = m_pCollect[nDir][nIndex];
	}
}


void CUSLut::GateGetTriggerMode(GatesData* pGate, int nGate,CString* pBuff,int nVerbose, int nLanguage)
{
	CString strVerbose;
	CString Code[3] = {L"Off", L"Main Bang", L"Interface"};

	if(nLanguage != 0) {
		Code[0].LoadString(IDS_Off);
		Code[1].LoadString(IDS_Main_Bang);
		Code[2].LoadString(IDS_Interface);
	}

	if(nVerbose==0) {
		pBuff->Format(L"%s",Code[pGate->nGateMode[nGate]%3]);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Trigger Mode";
			break;
		case 1: strVerbose.LoadString(IDS_Trigger_mode);
			break;
		}
		pBuff->Format(L"%s %s",strVerbose, Code[pGate->nGateMode[nGate]%3]);
	}

}

void CUSLut::GateGetDelay(GatesData* pGate, int nGate,CString* pBuff,int nVerbose, int nLanguage)
{
	
	CString strVerbose, strValue;

	switch(theApp.m_Scope.m_nGateDelayUnits) {
	case 0: strValue.Format(L"%.02f",(float)pGate->nNsDelay[nGate]/1000);
		break;
	case 1: strValue.Format(L"%.02f",(float)(pGate->nNsDelay[nGate]*theApp.m_LastSettings.nMaterialVelocity)/2e6);
		break;
	case 2: strValue.Format(L"%.04f",(float)(pGate->nNsDelay[nGate]*theApp.m_LastSettings.nMaterialVelocity)/50.8e6);
		break;
	}
	strValue += m_Units[theApp.m_Scope.m_nGateDelayUnits%3];

	if(nVerbose==0) {
		pBuff->Format(L"%s",strValue);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Delay";
			break;
		case 1: strVerbose.LoadString(IDS_Delay);
			break;
		}
		pBuff->Format(L"%s %s",strVerbose, strValue);
	}

}

void CUSLut::GateGetWidth(GatesData* pGate, int nGate,CString* pBuff,int nVerbose, int nLanguage)
{
	
	CString strVerbose, strValue;

	switch(theApp.m_Scope.m_nGateWidthUnits) {
	case 0: strValue.Format(L"%.02f",(float)pGate->nNsWidth[nGate]/1000);
		break;
	case 1: strValue.Format(L"%.02f",(float)(pGate->nNsWidth[nGate]*theApp.m_LastSettings.nMaterialVelocity)/2e6);
		break;
	case 2: strValue.Format(L"%.04f",(float)(pGate->nNsWidth[nGate]*theApp.m_LastSettings.nMaterialVelocity)/50.8e6);
		break;
	}
	strValue += m_Units[theApp.m_Scope.m_nGateWidthUnits%3];

	if(nVerbose==0) {
		pBuff->Format(L"%s",strValue);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Width";
			break;
		case 1: strVerbose.LoadString(IDS_Width);
			break;
		}
		pBuff->Format(L"%s %s",strVerbose, strValue);
	}

}

void CUSLut::GateGetThreshold(GatesData* pGate, int nGate,CString* pBuff,int nVerbose, int nLanguage)
{
	
	CString strVerbose, strValue;

	strValue.Format(L"%d %%",pGate->nThreshold[nGate]);

	if(nVerbose==0) {
		pBuff->Format(L"%s",strValue);
	} else {
		switch(nLanguage) {
		default: strVerbose = "Threshold";
			break;
		case 1: strVerbose.LoadString(IDS_Threshold);
			break;
		}
		pBuff->Format(L"%s %s",strVerbose, strValue);
	}

}


void CUSLut::ProcessSignedTraceOld(char *trace, int nSlot)
{
	DSP200Data* pDsp = &m_TS[nSlot].Dsp;
	GatesData* pGates = &m_TS[nSlot].Gate;
	ADC200Data* pAdc = &m_TS[nSlot].Adc;

	if (theApp.m_nSuspendProcessingCount>0) {
		theApp.m_nSuspendProcessingCount--;
		return;
	}

	int nInterfacePos = -1;
	int nStopPos = -1, nPhase = -1;
	int	ii, jj, gg;
	char cThreshold, cPhasePositive, cPhaseNegative;
	char cMax[8];
	char cMin[8];
	char cTempMax;
	char cTempMin;
	int	nDelay, nFinish, nTimeStatus;
	int	nPeakSampleFinishPos;
	bool bPeakStarted;
	ZeroMemory(cMax, sizeof cMax);
	FillMemory(cMin, sizeof cMin, 127);

	int nAcqLength = pAdc->nAcqLength;
	MinMax(&nAcqLength, 1, 8192);

	if (nSlot == 0) m_nAveragePtr++;
	m_nAveragePtr %= 1024;

	cThreshold = (char)pDsp->InterfaceThreshold[0];
	nPhase = -1;
	if (pDsp->SignDetect[0] & 1) cThreshold *= -1;
	cPhasePositive = abs(cThreshold);
	cPhaseNegative = cPhasePositive * -1;

	nDelay = pDsp->Delay[0];
	MinMax(&nDelay, 10, nAcqLength);
	if ((pDsp->Width[0] + nDelay)>nAcqLength) pDsp->Width[0] = nAcqLength - nDelay;
	for (ii = nDelay, jj = 0; jj<pDsp->Width[0]; ii++, jj++) {
		if (nInterfacePos == -1) {	//Find interface pos
			if (!pDsp->SignDetect[0]) {
				if (trace[ii] >= cThreshold)
					nInterfacePos = ii;
			}
			else {
				if (trace[ii] <= cThreshold)
					nInterfacePos = ii;
			}
		}
		if (!pDsp->SignDetect) {
			if (trace[ii] <= cMax[0]) cMax[0] = trace[ii];
		}
		else {
			if (trace[ii] >= cMax[0]) cMax[0] = trace[ii];
		}

		//phase
		if (pGates->nCollectPhase[0] && nPhase == -1) {
			if (trace[ii] >= cPhasePositive) {
				pGates->nPhase[0] = nPhase = 0;
			}
			if (trace[ii] <= cPhaseNegative) {
				pGates->nPhase[0] = nPhase = 1;
			}
		}
	}
	if (nInterfacePos == -1) {
		m_nInterfaceFailureCount++;
		pGates->nTimeStatus[0] = 0;
	}
	else {
		m_nInterfaceFailureCount = 0;
		pGates->nTimeStatus[0] = 1;
	}
	nInterfacePos -= theApp.m_Scope.m_nAdcIndexFirstPoint[nSlot];
	pDsp->InterfacePos = nInterfacePos;
	pDsp->TimsStartPos[0] = nInterfacePos;
	pGates->nTimeSample[0] = nInterfacePos;
	pGates->nAmplitude[0] = cMax[0];
	m_nGateAmp[m_nAveragePtr][nSlot][0] = pGates->nAmplitude[0];

	for (gg = 1; gg<8; gg++) {
		nTimeStatus = 0;

		switch (pGates->nGateMode[gg]) {
		case 0:								//Off
			break;
		case 1:								//Main Bang triggered
			nDelay = pGates->nSampleDelay[gg];
			nFinish = pGates->nSampleWidth[gg] + nDelay;
			break;
		case 2:								//interface triggered
			if (nInterfacePos != -1) {
				nDelay = pGates->nSampleDelay[gg] + nInterfacePos;
				nFinish = pGates->nSampleWidth[gg] + nDelay;
			}
			else {
				nDelay = 0;
				nFinish = 0;
			}
			break;
		default:								//interface triggered stopping before a previous gate
			nDelay = pGates->nSampleDelay[gg] + nInterfacePos;
			if (pGates->nTimeSample[pGates->nGateMode[gg] - 2] >= 0) {		//if the gate had a stop value use it
				nFinish = pGates->nTimeSample[pGates->nGateMode[gg] - 2] - (int)((float)pGates->nNsFarDeadZone[gg] / pAdc->fSamplePeriod);
				pGates->nSampleWidth[gg] = nFinish - nDelay;
			}
			else {													//else use max width
				pGates->nSampleWidth[gg] = (int)((float)pGates->nNsWidth[gg] / pAdc->fSamplePeriod);
				nFinish = pGates->nSampleWidth[gg] + nDelay;
			}
			break;
		}
		MinMax(&nDelay, 0, nAcqLength);
		MinMax(&nFinish, 0, nAcqLength);
		nStopPos = -1;
		nPhase = -1;
		cTempMax = -128;
		cTempMin = 127;

		if (pGates->nGateMode[gg]>0 && nDelay - nFinish) {

			cThreshold = MulDiv(pGates->nThreshold[gg], 127, 100);
			bPeakStarted = false;
			nPeakSampleFinishPos = 200000;
			cPhasePositive = abs(cThreshold);
			cPhaseNegative = cPhasePositive * -1;


			for (ii = nDelay, jj = nFinish; ii <= nFinish; ii++, jj--) {
				//Amplitude
				if (trace[ii] >= cMax[gg]) cMax[gg] = trace[ii];
				if (trace[ii] <= cMin[gg]) cMin[gg] = trace[ii];

				//Tims
				if (pGates->nTimsDetectionMode[gg]) {
					if (cThreshold >= 0) {
						switch (pGates->nTimsDetectionMode[gg]) {
						case 1:
							if ((trace[ii] >= cThreshold) && (nStopPos == -1)) {
								nStopPos = ii;
								nTimeStatus = 1;
							}
							break;
						case 2:
							if (trace[ii]>cTempMax) {
								nStopPos = ii;
								cTempMax = trace[ii];
								nTimeStatus = 1;
							}
							break;
						case 3:
							if ((trace[jj] >= cThreshold) && (nStopPos == -1)) {
								nStopPos = jj;
								nTimeStatus = 1;
							}
							break;
						case 4:
							if ((trace[ii]>cTempMax) && (ii<nPeakSampleFinishPos)) {
								nStopPos = ii;
								cTempMax = trace[ii];
								nTimeStatus = 1;
								if ((trace[ii] >= cThreshold) && (bPeakStarted == false)) {
									bPeakStarted = true;
									nPeakSampleFinishPos = ii + (int)((float)pGates->nNsPeakWidth / pAdc->fSamplePeriod);
								}
							}
							break;
						case 5:
							if ((trace[ii] >= cThreshold)) {
								if (trace[ii]>cTempMax) {
									nStopPos = ii;
									cTempMax = trace[ii];
									nTimeStatus = 1;
								}
							}
							break;
						}
					}
					else {
						switch (pGates->nTimsDetectionMode[gg]) {
						case 1:
							if ((trace[ii] <= cThreshold) && (nStopPos == -1)) {
								nStopPos = ii;
								nTimeStatus = 1;
							}
							break;
						case 2:
							if (trace[ii]<cTempMin) {
								nStopPos = ii;
								cTempMin = trace[ii];
								nTimeStatus = 1;
							}
							break;
						case 3:
							if ((trace[jj] <= cThreshold) && (nStopPos == -1)) {
								nStopPos = jj;
								nTimeStatus = 1;
							}
							break;
						case 4:
							if ((trace[ii]<cTempMin) && (ii<nPeakSampleFinishPos)) {
								nStopPos = ii;
								cTempMin = trace[ii];
								nTimeStatus = 1;
								if ((trace[ii]<cThreshold) && (bPeakStarted == false)) {
									bPeakStarted = true;
									nPeakSampleFinishPos = ii + (int)((float)pGates->nNsPeakWidth / pAdc->fSamplePeriod);
								}
							}
							break;
						case 5:
							if ((trace[ii] <= cThreshold)) {
								if (trace[ii]<cTempMin) {
									nStopPos = ii;
									cTempMin = trace[ii];
									nTimeStatus = 1;
								}
							}
						}
					}
				}

				//phase
				if ((pGates->nCollectPhase[gg]) && (nPhase == -1)) {
					if (trace[ii] >= cPhasePositive) {
						pGates->nPhase[gg] = nPhase = 0;
					}
					if (trace[ii] <= cPhaseNegative) {
						pGates->nPhase[gg] = nPhase = 1;
					}
				}

				pDsp->TimsStopPos[0] = nStopPos - theApp.m_Scope.m_nAdcIndexFirstPoint[nSlot];
			}
		}
		pGates->nTimeStatus[gg] = nTimeStatus;
		pGates->nTimeSample[gg] = nStopPos - theApp.m_Scope.m_nAdcIndexFirstPoint[nSlot];
		if (pGates->nThreshold[gg] > 0) {
			pGates->nAmplitude[gg] = cMax[gg];
		}
		else {
			pGates->nAmplitude[gg] = cMin[gg];
		}
		pGates->nPhase[gg] = nPhase;
		m_nGateAmp[m_nAveragePtr][nSlot][gg] = pGates->nAmplitude[gg];
	}
	if ((theApp.m_bCollectComplexData == TRUE) && (theApp.m_pScanData != NULL)) {
		theApp.m_pScanData->CollectComplexData();
	}
}




void CUSLut::ProcessSignedTrace(char *trace, int nSlot)
{
	DSP200Data* pDsp = &m_TS[nSlot].Dsp;
	GatesData* pGates = &m_TS[nSlot].Gate;
	ADC200Data* pAdc = &m_TS[nSlot].Adc;

	if (theApp.m_nSuspendProcessingCount>0) {
		theApp.m_nSuspendProcessingCount--;
		return;
	}

	int nInterfacePos = -1;
	int nStopPos = -1, nPhase = -1;
	int	ii, jj, gg;
	char cThreshold, cPhasePositive, cPhaseNegative;
	char cMax[8];
	char cMin[8];
	char cTempMax;
	char cTempMin;
	int	nDelay, nFinish, nTimeStatus;
	int	nPeakSampleFinishPos;
	bool bPeakStarted;
	ZeroMemory(cMax, sizeof cMax);
	FillMemory(cMin, sizeof cMin, 127);

	int nAcqLength = pAdc->nAcqLength;
	MinMax(&nAcqLength, 1, 8192);

	if (nSlot == 0) m_nAveragePtr++;
	m_nAveragePtr %= 1024;

	cThreshold = (char)pDsp->InterfaceThreshold[0];
	nPhase = -1;
	if (pDsp->SignDetect[0] & 1) cThreshold *= -1;
	cPhasePositive = abs(cThreshold);
	cPhaseNegative = cPhasePositive * -1;

	for (gg = 0; gg<8; gg++) {
		nTimeStatus = 0;

		switch (pGates->nGateMode[gg]) {
		case 0:								//Off
			break;
		case 1:								//Main Bang triggered
			nDelay = pGates->nSampleDelay[gg];
			nFinish = pGates->nSampleWidth[gg] + nDelay;
			break;
		case 2:								//interface triggered
			if (nInterfacePos != -1) {
				nDelay = pGates->nSampleDelay[gg] + nInterfacePos;
				nFinish = pGates->nSampleWidth[gg] + nDelay;
			}
			else {
				nDelay = 0;
				nFinish = 0;
			}
			break;
		default:								//interface triggered stopping before a previous gate
			nDelay = pGates->nSampleDelay[gg] + nInterfacePos;
			if (pGates->nTimeSample[pGates->nGateMode[gg] - 2] >= 0) {		//if the gate had a stop value use it
				nFinish = pGates->nTimeSample[pGates->nGateMode[gg] - 2] - (int)((float)pGates->nNsFarDeadZone[gg] / pAdc->fSamplePeriod);
				pGates->nSampleWidth[gg] = nFinish - nDelay;
			}
			else {													//else use max width
				pGates->nSampleWidth[gg] = (int)((float)pGates->nNsWidth[gg] / pAdc->fSamplePeriod);
				nFinish = pGates->nSampleWidth[gg] + nDelay;
			}
			break;
		}
		MinMax(&nDelay, 0, nAcqLength);
		MinMax(&nFinish, 0, nAcqLength);
		nStopPos = -1;
		nPhase = -1;
		cTempMax = -128;
		cTempMin = 127;

		if (pGates->nGateMode[gg]>0 && nDelay - nFinish) {

			cThreshold = MulDiv(pGates->nThreshold[gg], 127, 100);
			bPeakStarted = false;
			nPeakSampleFinishPos = 200000;
			cPhasePositive = abs(cThreshold);
			cPhaseNegative = cPhasePositive * -1;


			for (ii = nDelay, jj = nFinish; ii <= nFinish; ii++, jj--) {
				//Amplitude
				if (trace[ii] >= cMax[gg]) cMax[gg] = trace[ii];
				if (trace[ii] <= cMin[gg]) cMin[gg] = trace[ii];

				//Tims
				if (pGates->nTimsDetectionMode[gg]) {
					if (cThreshold >= 0) {
						switch (pGates->nTimsDetectionMode[gg]) {
						case 1:
							if ((trace[ii] >= cThreshold) && (nStopPos == -1)) {
								nStopPos = ii;
								nTimeStatus = 1;
							}
							break;
						case 2:
							if (trace[ii]>cTempMax) {
								nStopPos = ii;
								cTempMax = trace[ii];
								nTimeStatus = 1;
							}
							break;
						case 3:
							if ((trace[jj] >= cThreshold) && (nStopPos == -1)) {
								nStopPos = jj;
								nTimeStatus = 1;
							}
							break;
						case 4:
							if ((trace[ii]>cTempMax) && (ii<nPeakSampleFinishPos)) {
								nStopPos = ii;
								cTempMax = trace[ii];
								nTimeStatus = 1;
								if ((trace[ii] >= cThreshold) && (bPeakStarted == false)) {
									bPeakStarted = true;
									nPeakSampleFinishPos = ii + (int)((float)pGates->nNsPeakWidth / pAdc->fSamplePeriod);
								}
							}
							break;
						case 5:
							if ((trace[ii] >= cThreshold)) {
								if (trace[ii]>cTempMax) {
									nStopPos = ii;
									cTempMax = trace[ii];
									nTimeStatus = 1;
								}
							}
							break;
						}
					}
					else {
						switch (pGates->nTimsDetectionMode[gg]) {
						case 1:
							if ((trace[ii] <= cThreshold) && (nStopPos == -1)) {
								nStopPos = ii;
								nTimeStatus = 1;
							}
							break;
						case 2:
							if (trace[ii]<cTempMin) {
								nStopPos = ii;
								cTempMin = trace[ii];
								nTimeStatus = 1;
							}
							break;
						case 3:
							if ((trace[jj] <= cThreshold) && (nStopPos == -1)) {
								nStopPos = jj;
								nTimeStatus = 1;
							}
							break;
						case 4:
							if ((trace[ii]<cTempMin) && (ii<nPeakSampleFinishPos)) {
								nStopPos = ii;
								cTempMin = trace[ii];
								nTimeStatus = 1;
								if ((trace[ii]<cThreshold) && (bPeakStarted == false)) {
									bPeakStarted = true;
									nPeakSampleFinishPos = ii + (int)((float)pGates->nNsPeakWidth / pAdc->fSamplePeriod);
								}
							}
							break;
						case 5:
							if ((trace[ii] <= cThreshold)) {
								if (trace[ii]<cTempMin) {
									nStopPos = ii;
									cTempMin = trace[ii];
									nTimeStatus = 1;
								}
							}
						}
					}
				}

				//phase
				if ((pGates->nCollectPhase[gg]) && (nPhase == -1)) {
					if (trace[ii] >= cPhasePositive) {
						pGates->nPhase[gg] = nPhase = 0;
					}
					if (trace[ii] <= cPhaseNegative) {
						pGates->nPhase[gg] = nPhase = 1;
					}
				}

				pDsp->TimsStopPos[0] = nStopPos - theApp.m_Scope.m_nAdcIndexFirstPoint[nSlot];
			}
		}

		if (gg == 0) {
			if (nTimeStatus == 0) {
				m_nInterfaceFailureCount++;
			}
			else {
				m_nInterfaceFailureCount = 0;
				nInterfacePos = nStopPos - theApp.m_Scope.m_nAdcIndexFirstPoint[nSlot];
			}
			pDsp->InterfacePos = nInterfacePos;
			pDsp->TimsStartPos[0] = nInterfacePos;
		}
		pGates->nTimeStatus[gg] = nTimeStatus;
		pGates->nTimeSample[gg] = nStopPos - theApp.m_Scope.m_nAdcIndexFirstPoint[nSlot];
		if (pGates->nThreshold[gg] > 0) {
			pGates->nAmplitude[gg] = cMax[gg];
		}
		else {
			pGates->nAmplitude[gg] = cMin[gg];
		}
		pGates->nPhase[gg] = nPhase;
		m_nGateAmp[m_nAveragePtr][nSlot][gg] = pGates->nAmplitude[gg];
	}
	if ((theApp.m_bCollectComplexData == TRUE) && (theApp.m_pScanData != NULL)) {
		theApp.m_pScanData->CollectComplexData();
	}
}
