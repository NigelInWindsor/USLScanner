// Si10.cpp: implementation of the Si10 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Si10.h"
#include "winioctl.h"
#include "USLScanner.h"
#include "ViewSheet.h"



/**************

	Notes on Si10 Software

	See S627 Shanghai on server for more info on
		Collecting data routines Very Rough!
		Interfacing to Si10 from main code


***************/



/**************
	PUBLIC FUNCTIONS
		****************/

CSi10::CSi10()
{
	int ii;

	m_bConnected = false;
	m_bIntCallbackThreadEnabled = false;

	for(ii=0;ii<10;ii++) {
		m_Si10.pAxis[ii] = &theApp.m_Axes[ii];
	};

	m_bInternalPRF = theApp.m_LastSettings.bInternalPRF=1;
	m_bContinuousPRF = theApp.m_LastSettings.bContinuousPRF=0;
	m_bPRFCreateInt = theApp.m_LastSettings.bPRFCreateInt=0;

}

CSi10::~CSi10()
{

	CloseDevice();

}



bool CSi10::FindDevice()
{
	if (theApp.m_Si10Userx64.Initialize()) {
		m_bConnected = true;
	}
	else {
		if (!SI10DeviceOpen())
			return false;
	}
	m_bConnected = true;
	return true;

}

BOOL CSi10::IsConnected()
{
	return m_bConnected;
}


void CSi10::CloseDevice()
{
	if (theApp.m_Si10Userx64.IsConnected()) {
		theApp.m_Si10Userx64.CloseDevice();
		return;
	}
	if(IsConnected() == TRUE) {
		DeRegisterInterruptCallback();

		//Make sure no clocks are enabled
		U32 CounterEnabled = 0;
		OutpW(0x1A8,CounterEnabled);

		//Turn off interrupts
		U32 Interrupt = 0x0;
		OutpW(0x1AC,Interrupt);

		//Turn off GPIO interrupts
		OutpW(0x304,Interrupt);

		StopAllMotorsDead();

	}

	CloseHandle(m_Device.hDevice);
	m_bConnected = false;

}

bool CSi10::LoadFPGAConfigurationData()
{
	CFile FileHandle;
	CFileException ExceptionInstance;
	char pBuffer;
	unsigned long BytesReturned;
	int i=0;
	WCHAR FileName[256];
	char ss[256];
	char *pss;
	pss=ss;


	if(isFPGALoaded() == false || theApp.m_LastSettings.nForceSi10FirmwareLoad == TRUE) {


		FindLatestBinaryFile(FileName);

		/* Now try to open file for Read, Binary */
		if (!FileHandle.Open(FileName,CFile::typeBinary|CFile::modeRead, &ExceptionInstance))
			return false;


		CArchive archFile(&FileHandle,CArchive::load);

		while(archFile.Read(&pBuffer,1)) {
			m_Array[i] = pBuffer;
			i++;
		}

		archFile.Close();
		FileHandle.Close();

		m_Si10.pData = m_Array;
		m_Si10.nDataSize = i;

		if((m_Si10.pData != NULL) && (m_Si10.nDataSize>0)) {

		BOOL result = DeviceIoControl( m_Device.hDevice, IOCTL_SI10_LOAD_FPGA_CODE,
								NULL, 0,   // Input
								NULL, 0,   // Output
								&BytesReturned, NULL);

		}


		Sleep(4000);

	}

	return isFPGALoaded();


}

bool CSi10::FindLatestBinaryFile(WCHAR* pStr)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = NULL;
	WCHAR FilePath[260];
	WCHAR Directory[256];
	int nHighest=0;

	GetSystemDirectory(Directory,256);

	swprintf_s(FilePath,_T("%s\\drivers\\si10v1_fpgava1*.bin"),Directory);
	swprintf_s(pStr,100,_T("%s\\drivers\\si10v1_fpgava1.bin"),Directory);

	hFind = FindFirstFile(FilePath, &ffd);
	if (hFind != INVALID_HANDLE_VALUE )
	{
		do{
			if( (_ttoi(ffd.cFileName+14)) > nHighest) {
				nHighest=_ttoi(ffd.cFileName+14);
				swprintf_s(pStr,100,_T("%s\\drivers\\%s"),Directory,ffd.cFileName);
			}

		} while( FindNextFile(hFind, &ffd)!= 0 );

	}else{
		return false;
	}

	return true;


}

bool CSi10::StartDriver()
{
	m_Si10.bGenerateEvent = false;
	m_Si10.bGPIOEvent = false;
	m_Si10.nGPIOIrqCount = 0;
	
	
	//Share Si10 Struct
	if(ShareUserSi10Struct() == false)
		return false;

	if(ShareTimeSlotStruct() == false)
		return false;

	if(LoadFPGAConfigurationData()== false)
		return false;

	RegisterInterruptCallback();

	//Make sure no clocks are enabled
	U32 CounterEnabled = 0;
	OutpW(0x1A8,CounterEnabled);


	//generators to be software triggered, clk causes interrupt, ISR decides whether to issues step output
	U32 StepClk = 0x0000;
	OutpW(0x200,StepClk);


	//Turn on interrupts for all 10 stepper outputs
	U32 Interrupt = 0x03FF;
	OutpW(0x1AC,Interrupt);

	SetGPIOIRQMode();

	//Turn ON Enable Latch on Z register
	U32 LatchOnZ = 0x03FF;
	OutpW(0x068,LatchOnZ);

	//Turn OFF Enable Latch on Limit
	U32 LatchOnLimit = 0x0000;
	OutpW(0x06C,LatchOnLimit);

		m_Si10.nJoystickMode;
		m_Si10.nJoystickSource				= theApp.m_Tank.nJoystickSource;		//SERIAL, SI10
		m_Si10.nJoystickPercent				= 100;
		m_Si10.nJoystickLeftSideBit			= theApp.m_Tank.nLeftSideBit;
		m_Si10.nJoystickRightSideBit		= theApp.m_Tank.nRightSideBit;
		m_Si10.nJoystickDeadManBit0			= theApp.m_Tank.nDeadManBit0;
		m_Si10.nJoystickDeadManBit1			= theApp.m_Tank.nDeadManBit1;
		m_Si10.nJoystickMainManipBit		= theApp.m_Tank.nMainManipBit;
		m_Si10.nJoystickScannerFixtureBit	= theApp.m_Tank.nScannerFixtureBit;
		m_Si10.nJoystickSpeedBit			= theApp.m_Tank.nSpeedBit;
		m_Si10.nJoystickDeadBand			= theApp.m_Tank.nJoystickDeadBand;

	return true;
}

	//Turn on interrupt for all 8 GPIO bits Rising edge on all interrupts 0x5555
	//Turn on interrupt for all 8 GPIO bits Falling edge on all interrupts 0xffff
void CSi10::SetGPIOIRQMode()
{
	U32 nMask;

	switch(theApp.m_Tank.cSi10GPIOIrqMode[0]) {
	case 0:	//IRQ off
		nMask = 0x0000;
		OutpW(0x304,nMask);
		break;
	case 1: // Irq On rising edge
		nMask = 0x0001;
		OutpW(0x304,nMask);
		nMask = 0;
		OutpW(0x308,nMask);
		break;
	case 2: // Irq On falling edge
		nMask = 0x0003;
		OutpW(0x304,nMask);
		nMask = 0;
		OutpW(0x308,nMask);
		break;
	case 3: // Irq level change
		nMask = 0x0001;
		OutpW(0x304,nMask);
		nMask = 0x0002;							//Interrupt on both edges with SI10v1_fpga1170331.bin
		OutpW(0x308,nMask);
		break;
	}
}

void CSi10::EnableAxes(int nAction)
{
	for(int nAxis = 0;nAxis<24;nAxis++) {
		if((theApp.m_Axes[nAxis].bAvailable==TRUE) && (theApp.m_Axes[nAxis].nMotorType==STEPPER)) {
			EnableAxis(nAxis,nAction);
		}
	}
}

void CSi10::EnableAxis(int nWhich,int action, int nReqSpeed)
{
	struct AxisData *axis = &theApp.m_Axes[nWhich];

	axis->nReqSpeed = (int)((float)nReqSpeed/axis->fStepSize); //only used in continous rotate

	int	nSpeed;
	if(axis->nIrqMask==0) return;
	if(axis->bAvailable==FALSE) return;

	DisableAxis(nWhich);


	(axis->nPos > axis->nDesirePos)?
		(m_Si10.nDirReg|=axis->nBit):(m_Si10.nDirReg&=(~axis->nBit));

	axis->nCount=axis->nPos;
	switch(action) {
	case CONTINUOUS_ROTATE:
	case MOVETO:	axis->nCurrentSpeed = axis->nMinSpeed;
		if(axis->nDesirePos-axis->nPos) m_Si10.nProcessState|=axis->nIrqMask;
		break;
	case JOYSTICK:
		m_Si10.nJoystickPercent=100;
		axis->nCurrentSpeed = MulDiv(axis->nJoystickMinSpeed,m_Si10.nJoystickPercent,100);
		switch(theApp.m_Tank.nJoystickSource) {
		case WIRED_SI10: axis->nJoystickZero=ReadJoystick(axis->nJoystickNumber);
			break;
		case WIRED_SERIAL:
		case WIRELESS_SERIAL: axis->nJoystickZero=m_Si10.nJoystickValue[axis->nJoystickNumber];
			break;
		}
		m_Si10.nProcessState&=(~axis->nIrqMask);
		break;
	case HOMETO:
		axis->nHomeAction = 0;
		axis->nCurrentSpeed = axis->nMinSpeed;
		(axis->nHomeSpeed<0)?(m_Si10.nDirReg|=axis->nBit):(m_Si10.nDirReg&=(~axis->nBit));
		m_Si10.nProcessState|=axis->nIrqMask;
		axis->bHomeFlag=FALSE;
		break;
	};

	m_Si10.nIrqAction[axis->nDriverAxisNumber]=action;

	OutpW(SI10_DIR_PORT,m_Si10.nDirReg ^ m_Si10.nDirEor);

	if(axis->nCurrentSpeed==0) axis->nCurrentSpeed=1;
	nSpeed=1000000/axis->nCurrentSpeed;
	if(nSpeed>0xffff) nSpeed=0xffff;

	CounterStart(axis->nCounter,nSpeed);

	m_nCountersEnabled |= (axis->nIrqMask);
	OutpW(0x1A8,m_nCountersEnabled);

}


void CSi10::DisableAxis(int nWhich)
{
	struct AxisData *axis = &theApp.m_Axes[nWhich];

	if(axis->nIrqMask == 0) return;
	if(axis->bAvailable==FALSE) return;

	m_nCountersEnabled &= (~axis->nIrqMask);


	m_Si10.nIrqAction[nWhich]=0;

	OutpW(0x1A8,m_nCountersEnabled);

	m_Si10.nProcessState&=(~axis->nIrqMask);
}

bool CSi10::IsMotionFinished()
{
	if(IsAxisInPosition(theApp.m_Tank.nXLeft) == false) return false;
	if(IsAxisInPosition(theApp.m_Tank.nYLeft) == false) return false;
	if(IsAxisInPosition(theApp.m_Tank.nZLeft) == false) return false;
	if(IsAxisInPosition(theApp.m_Tank.nXtLeft) == false) return false;
	if(IsAxisInPosition(theApp.m_Tank.nYtLeft) == false) return false;
//	if(IsAxisInPosition(theApp.m_Tank.nRLeft) == false) return false;
	return true;
}

bool CSi10::IsAxisInPosition(int nAxis)
{
	if(theApp.m_Axes[nAxis].bAvailable==TRUE) {
		if(abs(theApp.m_Axes[nAxis].nDesirePos - theApp.m_Axes[nAxis].nPos) > theApp.m_Axes[nAxis].nPositionError) return false;
	}

	return true;
}

bool CSi10::TestAllAxisRunning()
{
	bool Flag = FALSE;

	Flag |= TestAxisRunning(theApp.m_Tank.nXLeft);
	Flag |= TestAxisRunning(theApp.m_Tank.nYLeft);
	Flag |= TestAxisRunning(theApp.m_Tank.nZLeft);
	Flag |= TestAxisRunning(theApp.m_Tank.nXtLeft);
	Flag |= TestAxisRunning(theApp.m_Tank.nYtLeft);
	Flag |= TestAxisRunning(theApp.m_Tank.nRLeft);

	return Flag;
}

bool CSi10::TestHoldingFixtureAllAxisRunning()
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

bool CSi10::TestAxisRunning(int nWhich)
{
	if(theApp.m_Axes[nWhich].bAvailable==TRUE) {
		if(theApp.m_Axes[nWhich].nIrqMask & m_Si10.nProcessState) return TRUE;
	}
	return FALSE;

}

int CSi10::ReadJoystick(int nWhich)
{
	U32 Res = InpW(SI10_JOYSTICK);
	int Result = (int)Res;

	return ( (Result>>(nWhich*8)) & 0x000000FF );
}



U32 CSi10::InpW(int nOffset)
{
  	U32 Result = 0;

	if (theApp.m_Si10Userx64.IsConnected()) {
		return theApp.m_Si10Userx64.InpW(nOffset);
	}
	else {
		if (IsConnected()) {
			SI10PciBarSpaceRead(nOffset, 0x4, &Result);
		}
	}
	return Result;
}


void CSi10::OutpW(int nOffset, U32 Value)
{
	if (theApp.m_Si10Userx64.IsConnected()) {
		theApp.m_Si10Userx64.OutpW(nOffset, (int)Value);
		return;
	}

	SI10PciBarSpaceWrite(nOffset,0x04,&Value);

	return;
}

bool CSi10::ShareUTStruct()
{
	unsigned long BytesReturned;

	int result = DeviceIoControl( m_Device.hDevice, IOCTL_SI10_SHARE_UT_USER_STRUCT,
							&m_Si10UTData, 0,		// Input
							NULL, 0,		// Output
							&BytesReturned, NULL);

	return result  & 0x01;

}

bool CSi10::FreeUTStruct()
{
	unsigned long BytesReturned;


	BOOL result = DeviceIoControl( m_Device.hDevice, IOCTL_SI10_FREE_UT_USER_STRUCT,
							NULL, 0,		// Input
							NULL, 0,		// Output
							&BytesReturned, NULL);

	return result & 0x01;

}

//Extract scan values from ViewSheet
//To add: some sort of nWhichAxis variable;
bool CSi10::CollectData(void* ptr, int nScanLine, int nStepSize, int nSamplePeriod)
{
	if( (ptr == NULL) ||
		(nStepSize==0) ||
		(nSamplePeriod==0) )
		return false;

	CViewSheet *pViewSheet =  (CViewSheet*) ptr;

	m_Si10UTData.bCollectData = TRUE;

	m_Si10UTData.nSamplesLine = pViewSheet->m_Data.m_nSamplesLine;
	m_Si10UTData.nNumberLines = pViewSheet->m_Data.m_nNumberLines;
	m_Si10UTData.nStartPos = (int)(pViewSheet->m_Data.m_CpStart.Side0.fX * 1000.0f); //is start pos in scanning axis ASSUMES X Now hardcoded changin in scan size or stop start pos will have no effect
	m_Si10UTData.nFinishPos = (int)(pViewSheet->m_Data.m_CpFinish.Side0.fX * 1000.0f);
	m_Si10UTData.nAmpImages = pViewSheet->m_Data.m_nNumberAmpImages[0];
	m_Si10UTData.nTimeImages = pViewSheet->m_Data.m_nNumberTimeImages[0];

	for(int i=0; i<100; i++) {
		m_Si10UTData.m_cAmpBuffer[i] = pViewSheet->m_Data.m_cAmpBuffer[i];
		m_Si10UTData.m_nTimeBuffer[i] = pViewSheet->m_Data.m_nTimeBuffer[i];
	}

	m_Si10UTData.nScanLine = nScanLine;
	m_Si10UTData.nStepSize = nStepSize;
	m_Si10UTData.nSamplePeriod = nSamplePeriod;

	//Share it here
	ShareUTStruct();

	return true;
}



/**********************
	PRIVATE FUNCTIONS
		********************/


void CSi10::AxisDefaults(int nWhich)
{
	int	nMask=1;

	memset(&theApp.m_Axes[nWhich],0,sizeof AxisData);
//	theApp.m_Axes[nWhich].dStepSize = 0.1;
	theApp.m_Axes[nWhich].nBit = nMask<<nWhich;
	theApp.m_Axes[nWhich].nCounter=nWhich;
	nMask = 1;
	theApp.m_Axes[nWhich].nIrqMask=nMask << theApp.m_Axes[nWhich].nCounter;
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


bool CSi10::ShareUserSi10Struct()
{
	unsigned long BytesReturned;
	int nAxis,nTotal;

	for(nAxis=0,nTotal=0;(nAxis<24) && (nTotal < 10);nAxis++) {
		if((theApp.m_Axes[nAxis].bAvailable==TRUE) && (theApp.m_Axes[nAxis].nMotorType==STEPPER)) {
			m_Si10.pAxis[nTotal] = &theApp.m_Axes[nAxis];
			theApp.m_Axes[nAxis].nDriverAxisNumber = nTotal++;
		}
	}

	if (theApp.m_Si10Userx64.IsConnected()) {
		return true;
	}
	else {
		if (IsConnected()) {
			int nResult = DeviceIoControl(m_Device.hDevice, IOCTL_SI10_SHARE_USER_STRUCT,
				&m_Si10, 0,		// Input
				NULL, 0,		// Output
				&BytesReturned, NULL);

			if (nResult == 1) return true;
		}
	}
	return false;

}







bool CSi10::SI10PciBarSpaceRead(U32 addrs, U32 ByteCount,U32 *pData)
{
	PLX_PARAMS IoBuffer;
	bool bReturn = true;


	if (!IsObjectValid(&m_Device)) return false;

	m_CriticalSection.Lock();

	IoBuffer.value[0]             = (U8) 2;
    IoBuffer.value[1]             = (PLX_ACCESS_TYPE) BitSize32;
    IoBuffer.value[2]             = (BOOLEAN) TRUE;

    IoBuffer.u.TxParams.LocalAddr = addrs;
    IoBuffer.u.TxParams.ByteCount = ByteCount;
    IoBuffer.u.TxParams.u.UserVa  = (PLX_UINT_PTR)pData;


	unsigned long BytesReturned;

	int nResult = DeviceIoControl(m_Device.hDevice, PLX_IOCTL_PCI_BAR_SPACE_READ,
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Input
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Output
							&BytesReturned,NULL);

	m_CriticalSection.Unlock();
	if( IoBuffer.ReturnCode != SI10ApiSuccess) bReturn = false;

	return bReturn;

}



bool CSi10::SI10PciBarSpaceWrite(U32 addrs, U32 ByteCount,U32 *pData)
{
	PLX_PARAMS IoBuffer;
	bool bReturn = true;

	if (!IsObjectValid(&m_Device)) return false;

	m_CriticalSection.Lock();

	IoBuffer.value[0]             = (U8) 2;
    IoBuffer.value[1]             = (PLX_ACCESS_TYPE) BitSize32;
    IoBuffer.value[2]             = (BOOLEAN) TRUE;

    IoBuffer.u.TxParams.LocalAddr = addrs;
    IoBuffer.u.TxParams.ByteCount = ByteCount;
    IoBuffer.u.TxParams.u.UserVa  = (PLX_UINT_PTR)pData;


	unsigned long BytesReturned;

	int nResult = DeviceIoControl(m_Device.hDevice, PLX_IOCTL_PCI_BAR_SPACE_WRITE,
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Input
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Output
							&BytesReturned,NULL);

	m_CriticalSection.Unlock();
	if( IoBuffer.ReturnCode != SI10ApiSuccess) bReturn = false;


	return bReturn;

}

bool CSi10::SI10DeviceOpen()
{

	PLX_PARAMS			IoBuffer;
	PLX_DEVICE_KEY		Key;
	unsigned long BytesReturned;
	int ReturnCode=0;

	if(&m_Device == NULL)    return false;

	if(IsObjectValid(&m_Device))
        return false;

	memset(&m_Device, 0, sizeof(PLX_DEVICE_OBJECT));

	ObjectInvalidate( &m_Device );

	m_Device.Key.ApiMode = PLX_API_MODE_PCI;

	m_Device.hDevice = CreateFile(_T("\\\\.\\plx9030_v600-0"),
										GENERIC_READ | GENERIC_WRITE,        // Desired access
										FILE_SHARE_READ | FILE_SHARE_WRITE,  // Shared mode
										NULL,                                // Security attributes
										OPEN_EXISTING,                       // Creation disposition
										FILE_ATTRIBUTE_NORMAL,               // File attributes
										NULL                                 // Template file
									);

	if(m_Device.hDevice == INVALID_HANDLE_VALUE)
		return false;

	memset(&Key,PCI_FIELD_IGNORE,sizeof(PLX_DEVICE_KEY));

	 // Set up fields to be used in search
	Key.DeviceId    = PCI_DEVICE_ID;
	Key.VendorId    = PCI_VENDOR_ID;

	IoBuffer.Key	= Key;
	IoBuffer.value[0] = 0;


	DeviceIoControl(m_Device.hDevice, PLX_IOCTL_PCI_DEVICE_FIND,
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Input
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Output
							&BytesReturned,NULL);

	if(IoBuffer.ReturnCode != SI10ApiSuccess) {
		CloseHandle(m_Device.hDevice);
		return false;
	}

	m_Device.Key = IoBuffer.Key;
	m_Device.Key.ApiIndex = 0;
	m_Device.Key.ApiMode = PLX_API_MODE_PCI;

	ObjectValidate( &m_Device.Key );

	ObjectValidate( &m_Device );

	return true;

}


bool CSi10::GetDriverVersion(CString *pBuff)
{
	PLX_PARAMS IoBuffer;
	bool bReturn = true;

	if (theApp.m_Si10Userx64.IsConnected()) {
		*pBuff = theApp.m_Si10Userx64.getDriverVersion();
		return true;
	}

	if (!IsObjectValid(&m_Device)) return false;

	m_CriticalSection.Lock();

	ZeroMemory(&IoBuffer, sizeof IoBuffer);


	unsigned long BytesReturned;

	DeviceIoControl(m_Device.hDevice, PLX_IOCTL_DRIVER_VERSION,
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Input
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Output
							&BytesReturned,NULL);

	m_CriticalSection.Unlock();
	if( IoBuffer.ReturnCode != SI10ApiSuccess) bReturn = false;


	int nMajorVersion = (IoBuffer.value[0] >> 16) & 0xff;
	int nMinorVersion = (IoBuffer.value[0] >> 8) & 0xff;
	int nRevision =		(IoBuffer.value[0] >> 0) & 0xff;

	pBuff->Format(_T("%d.%02d.%d"),nMajorVersion, nMinorVersion, nRevision);

	return bReturn;

}

void CSi10::GetFirmwareVersion(CString *pBuff)
{
  	U32 Result = 0;

	if (theApp.m_Si10Userx64.IsConnected()) {
		*pBuff = theApp.m_Si10Userx64.getFPGABinaryDate();
		return;
	}

	SI10PciBarSpaceRead(0x3FC,0x4,&Result);

	pBuff->Format(_T("%06x"),Result);

}

void CSi10::CounterStart(int number,int value)
{

	switch(number) {
	case 0:
		OutpW(SI10_COUNT_0,value&0xffff);
		break;
	case 1:
		OutpW(SI10_COUNT_1,value&0xffff);
		break;
	case 2:
		OutpW(SI10_COUNT_2,value&0xffff);
		break;
	case 3:
		OutpW(SI10_COUNT_3,value&0xffff);
		break;
	case 4:
		OutpW(SI10_COUNT_4,value&0xffff);
		break;
	case 5:
		OutpW(SI10_COUNT_5,value&0xffff);
		break;
	case 6:
		OutpW(SI10_COUNT_6,value&0xffff);
		break;
	case 7:
		OutpW(SI10_COUNT_7,value&0xffff);
		break;
	case 8:
		OutpW(SI10_COUNT_8,value&0xffff);
		break;
	case 9:
		OutpW(SI10_COUNT_9,value&0xffff);
		break;
	};

}



bool CSi10::isFPGALoaded(CString* pBuff)
{

	U32 TempBuffU32[256];
	U8  BitCtr;
	U8  TempUCh;
	CString Temp, Buff;

	SI10PciBarSpaceRead(0x3FC,0x4,TempBuffU32);

	Buff.Empty();
	for(BitCtr=0;BitCtr <= 3;BitCtr++)
	{
		TempUCh = (U8)(TempBuffU32[0] & 0x000000FF);

		switch (BitCtr)
		{
		case 0:
			if((TempUCh<'A') || (TempUCh>'Z'))
			return false;
			break;
		case 1:
			if((TempUCh<'0') || (TempUCh>'9'))
			return false;
		break;
		case 2:
			if((TempUCh<'0') || (TempUCh>'9'))
			return false;
		case 3:
			if((TempUCh<' ') || (TempUCh>'9'))
			return false;
			break;
		};
		Temp.Format(L"%c", TempUCh);
		Buff += Temp;

		TempBuffU32[0] >>= 8;
	}
	if(pBuff != NULL) {
		pBuff->Format(L"%s", Temp);
	}

	return true;
}


int CSi10::Si10ReadTS0()
{
	U32 Result = 0;

	SI10PciBarSpaceRead(0x3C4,0x4,&Result);

	if(Result&0x00000008) //bit 3
		return 1;
	else
		return 0;

}

void CSi10::Si10SetPRF(int nPrf)
{
	U32 nValue = 0x00;

	/* Mailbox &  Syncs Generator Control Register*/
	if(m_bInternalPRF)
		nValue |= 0x01;

	if(m_bContinuousPRF)
		nValue |= 0x08;

	//create Interrupt
	if(m_bPRFCreateInt)
		nValue |= 0x20;

	if(theApp.m_LastSettings.nDesiredDevice==NOT_PHASED_ARRAY) {
		if(theApp.m_LastSettings.nMuxVersion < 2) {
			nValue |= 0x40;		//25 useconds
		}
	} else {
		nValue |= 0x44;			//delayed prf
	}

	OutpW(0x3C4,nValue);

	if(nPrf<=0)
		nPrf=100;

	/* PRF Trigger rate */
	nValue = 0xf4240/nPrf; //1 unit = 1us

	if(nValue<20)
		nValue=20;

	if(nValue>0x3b2f)
		nValue=0x3b2f;

	OutpW(0x3CC,nValue);

	return;
}

void CSi10::Reset()
{
//	U32 nValue;

//???	OutpW(0x3C4, 0x200, &nValue);
}

void CSi10::Si10ArmPRF()
{
	U32 nValue = 0x10; //cause trigger

	/* Mailbox &  Syncs Generator Control Register*/
	if(m_bInternalPRF)
		nValue |= 0x01;


	if(m_bContinuousPRF)
		nValue |= 0x08;

	//create Interrupt
	if(m_bPRFCreateInt)
		nValue |= 0x20;

	if(theApp.m_LastSettings.nDesiredDevice==NOT_PHASED_ARRAY) {
		if(theApp.m_LastSettings.nMuxVersion < 2) {
			nValue |= 0x40;		//25 useconds
		}
	} else {
		nValue |= 0x44;			//delayed prf
	}

	OutpW(0x3C4,nValue);

	return;
}



void CSi10::RegisterInterruptCallback()
{
	if (theApp.m_Si10Userx64.IsConnected()) {
		theApp.m_Si10Userx64.RegisterInterruptCallback();
	}
	else {
		m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		unsigned long BytesReturned;
		DeviceIoControl(m_Device.hDevice, IOCTL_SI10_ENABLE_INTERRUPT_CALLBACK,
			&m_hEvent, sizeof(m_hEvent),		// Input
			NULL, 0,		// Output
			&BytesReturned, NULL);

		m_bIntCallbackThreadEnabled = true;

		m_pThread = AfxBeginThread(&PendForInterrupt, this, THREAD_PRIORITY_TIME_CRITICAL, 0, NULL);
	}
}

void CSi10::DeRegisterInterruptCallback()
{

	if (theApp.m_Si10Userx64.IsConnected()) {
		theApp.m_Si10Userx64.DeRegisterInterruptCallback();
	}
	else {
		if (m_bIntCallbackThreadEnabled) {
			TRACE0("Wait for Si10 Thread to finish\n");
			m_bIntCallbackThreadEnabled = false;
			WaitForSingleObject(m_pThread, INFINITE);
			TRACE0("SI10 Object finished\n");
		}
	}
}



UINT PendForInterrupt(LPVOID pParam)
{
	CSi10* pParent = (CSi10 *)pParam;

	pParent->m_bIntCallbackThreadFinished = FALSE;
	while(pParent->m_bIntCallbackThreadEnabled == TRUE) {
		if(WaitForSingleObject(pParent->m_hEvent, 100) == WAIT_OBJECT_0) {
			pParent->m_nEventsCount++;

			theApp.m_Ethercat.Si10CallBack();

			if((theApp.m_bCollectComplexData==TRUE) && (theApp.m_pScanData!=NULL)) {
//				theApp.m_pScanData->CollectComplexData();
			}
			if(theApp.m_bCollectNormalizeData==TRUE) {
				theApp.m_Thread.CollectNormalizeData();
			}

			if(theApp.m_bCollectEccentricData==TRUE) {
				theApp.m_Thread.CollectEccentricData();
			}

			if(	theApp.m_bLaserCollectTaughtLines==TRUE) {
				theApp.m_Thread.CollectLaserTaughtLineData();
			}


			pParent->m_Si10.bGPIOEvent = FALSE;

			ResetEvent(pParent->m_hEvent);
		}
	}
	pParent->m_bIntCallbackThreadFinished = TRUE;
	TRACE0("SI10 Interrupt callback finished\n");

	return 0;


}


bool CSi10::ShareTimeSlotStruct()
{
	unsigned long BytesReturned;

	int nResult = DeviceIoControl( m_Device.hDevice, IOCTL_SI10_SHARE_TIMESLOT_STRUCT,
							&theApp.m_UtUser.m_TS[0], 0,		// Input
							NULL, 0,		// Output
							&BytesReturned, NULL);

	if(nResult == 1) return true;
	return false;

}



void CSi10::EnableJoystick()
{
	m_Si10.nJoystickSource=theApp.m_Tank.nJoystickSource;
	m_Si10.nJoystickMode=0;

	for(int nAxis = 0;nAxis<24;nAxis++) {
		if((theApp.m_Axes[nAxis].bAvailable==TRUE) && (theApp.m_Axes[nAxis].nMotorType==STEPPER)) {
			EnableAxis(nAxis,JOYSTICK);
		}
	}

}


void CSi10::StopAllMotorsDead()
{
	for(int nAxis = 0;nAxis<24;nAxis++) {
		if((theApp.m_Axes[nAxis].bAvailable==TRUE) && (theApp.m_Axes[nAxis].nMotorType==STEPPER)) {
			DisableAxis(nAxis);
		}
	}

}

void CSi10::MoveTo(int nAction)
{
	//nDesirePos alread initialized
	for(int nAxis = 0;nAxis<24;nAxis++) {
		if((theApp.m_Axes[nAxis].bAvailable==TRUE) && (theApp.m_Axes[nAxis].nMotorType==STEPPER)) {
			EnableAxis(nAxis,MOVETO);
		}
	}
}

void CSi10::MoveJustR(int nAction)
{
	int nAxis = theApp.m_Tank.nRLeft;

	//nDesirePos alread initialized
	if((theApp.m_Axes[nAxis].bAvailable==TRUE) && (theApp.m_Axes[nAxis].nMotorType==STEPPER)) {
		EnableAxis(nAxis,MOVETO);
	}
}

int CSi10::DownLoadPolyMoveCoordinates(CPolyCoord *ToolPath, int nDir)
{
	CCoord	Cp;

	if(nDir==0) {
		Cp=ToolPath->m_pCp[ToolPath->m_nCoordL - 1];
	} else {
		Cp=ToolPath->m_pCp[0];
	}

	DownLoadCoord(Cp);

	return ToolPath->m_nCoordL;
}

void CSi10::DownLoadCoord(CCoord Cp)
{
	int nX0,nY0,nZ0,nXt0,nYt0;

	nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
	nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
	nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
	nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
	nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);

	theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos = nX0;
	theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos = nY0;
//	TRACE1("%d\n",nY0);
	theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos = nZ0;
	theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos = nXt0;
	theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos = nYt0;

}

int CSi10::ReadLimitSwitches()
{
	U32 Res = InpW(SI10_LIMIT_SWITCHES);
	int nResult = (int)Res ^ m_Si10.nSwitchEor;

	return nResult;
}



void CSi10::ZeroPosition(int nWhich)
{
	struct AxisData *pAxis = &theApp.m_Axes[nWhich];

	pAxis->nPos=pAxis->nCount=pAxis->nDesirePos=0;
	if(pAxis->nEncoderMode)
		EncoderZero(pAxis);

	if(pAxis->nStepsPerRev){
		pAxis->nRevCounter=0;
		pAxis->nDesireRevCount=0;
	}
}



void CSi10::EncoderZero(struct AxisData	*pAxis)
{
	U32 nValue = 0x0;

	OutpW(SI10_ENCODER_0 + pAxis->nEncoderNumber * 4,nValue);

}

void CSi10::MoveToCoord(HoldingFixtureCoord Cp)
{
	theApp.m_Axes[theApp.m_Tank.nXStep].nDesirePos=(int)(Cp.fXMoving/theApp.m_Axes[theApp.m_Tank.nXStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYStep].nDesirePos=(int)(Cp.fYMoving/theApp.m_Axes[theApp.m_Tank.nYStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nZStep].nDesirePos=(int)(Cp.fZMoving/theApp.m_Axes[theApp.m_Tank.nZStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nXtStep].nDesirePos=(int)(Cp.fYFixed/theApp.m_Axes[theApp.m_Tank.nXtStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYtStep].nDesirePos=(int)(Cp.fZFixed/theApp.m_Axes[theApp.m_Tank.nYtStep].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nRStep].nDesirePos=(int)(Cp.fXFixed/theApp.m_Axes[theApp.m_Tank.nRStep].fStepSize);
	EnableAxis(theApp.m_Tank.nXStep,MOVETO);
	EnableAxis(theApp.m_Tank.nYStep,MOVETO);
	EnableAxis(theApp.m_Tank.nZStep,MOVETO);
	EnableAxis(theApp.m_Tank.nXtStep,MOVETO);
	EnableAxis(theApp.m_Tank.nYtStep,MOVETO);
	EnableAxis(theApp.m_Tank.nRStep,MOVETO);

}

void CSi10::EnableHomeFind(int nAxis)
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

void CSi10::TestPatternOutput(int nPattern)
{
	OutpW(SI10_DIR_PORT,nPattern);
}

int CSi10::GetInputPort()
{
	theApp.m_Tank.nGeneralInputXORSi10 = 0xff;

	return ( m_nGeneralInput = (InpW(0x300) ^ theApp.m_Tank.nGeneralInputXORSi10)); //GP IO
}

void CSi10::WriteGeneralOuput(int nValue)
{
	m_nGeneralOutput = nValue;

	OutpW(0x300,m_nGeneralOutput);		

}

int CSi10::GetGeneralOuput()
{
	return m_nGeneralOutput;
}

void CSi10::CheckHoldingBrake()
{
	if(theApp.m_LastSettings.nBrakeHoldingMode == 1) {		//Automatic
		if(m_Si10.nProcessState - m_nOldProcessState) {

			if((m_Si10.nProcessState & theApp.m_Axes[theApp.m_Tank.nZStep].nIrqMask) || (m_Si10.nProcessState & theApp.m_Axes[theApp.m_Tank.nYtStep].nIrqMask)) {
				theApp.m_Motors.ZBrakesEnable(FALSE,1);
			} else {
				//At Fokker holding fixture brakes and scanner brakes on same output
				if(theApp.m_Tank.nBrakeBit1!=theApp.m_Tank.nBrakeBit0)
					theApp.m_Motors.ZBrakesEnable(TRUE,1);
			}

		}
	}
	m_nOldProcessState = m_Si10.nProcessState;
}

int CSi10::ReadAbsEncoder(int nWhich)
{
	OutpW(0x100,0);
	return (InpW(0x104 + nWhich * 4) ^0xffff) & 0xffff;
}



int CSi10::ReadAbsStatus(int nWhich, CString &Buff)
{
	int nStatus = InpW(0x104 + nWhich * 4) & 0xffff0000;
	CString Temp;

	Buff.Empty();
	if(nStatus == 0) Buff.LoadString(IDS_Ok);

	if(nStatus & 0x10000) {
		Temp.LoadString(IDS_Warning);
		Buff += (Temp + _T(" "));
	}
	if(nStatus & 0x20000) {
		Temp.LoadString(IDS_Error);
		Buff += (Temp + _T(" "));
	}
	if(nStatus & 0x40000) {
		Temp.LoadString(IDS_Overflow);
		Buff += (Temp + _T(" "));
	}
	if(nStatus & 0x80000) {
		Temp.LoadString(IDS_Format);
		Buff += (Temp + _T(" "));
	}
	if(nStatus & 0x100000) {
		Temp.LoadString(IDS_CRC);
		Buff += (Temp + _T(" "));
	}


	return nStatus;

}

void CSi10::StartContinuousRotate(int nAxis, float Speed, int nDir)
{
	theApp.m_Axes[nAxis].nDesireRevCount = theApp.m_Axes[nAxis].nRevCounter;

	if(nDir==0) {
		theApp.m_Axes[nAxis].nDesirePos=0x7fffffff;
	}else{
		theApp.m_Axes[nAxis].nDesirePos=0x80000000;
	}

	EnableAxis(nAxis,CONTINUOUS_ROTATE,(int)Speed);

}


void CSi10::StartContinuousRotate(int nAxis, float fIncrement, float fStartPos, float fFinishPos)
{
	theApp.m_Axes[nAxis].nStartPos = (int)(fStartPos/theApp.m_Axes[nAxis].fStepSize);
	theApp.m_Axes[nAxis].nFinishPos = (int)(fFinishPos/theApp.m_Axes[nAxis].fStepSize);
	theApp.m_Axes[nAxis].nSlowIncrement = (int)((fIncrement*1000)/theApp.m_Axes[nAxis].fStepSize); //in thousands of a step


	EnableAxis(nAxis,CONTINUOUS_ROTATE);

}


void CSi10::StopContinuousRotate(int nAxis)
{
	DisableAxis(nAxis);
}

int CSi10::GetRevCounter(int nAxis)
{
	return theApp.m_Axes[nAxis].nRevCounter;
}

void CSi10::SetRevCounter(int nAxis, int nValue)
{
	theApp.m_Axes[nAxis].nRevCounter = nValue;
}

bool CSi10::IsRotateRunning()
{
	for(int nAxis = 0;nAxis<24;nAxis++) {
		if((theApp.m_Axes[nAxis].bAvailable==TRUE)
			&& (theApp.m_Axes[nAxis].nMotorType==STEPPER)
			&& (m_Si10.nIrqAction[theApp.m_Axes[nAxis].nDriverAxisNumber]==CONTINUOUS_ROTATE) ) {
			return true;
		}
	}

	return false;
}

void CSi10::StartXNumberOfRevs(float fNumber,float fSpeed)
{
	int nDesirePos;

	theApp.m_Axes[theApp.m_Tank.nRLeft].nDesireRevCount = theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter + (int)fNumber;

	nDesirePos = (int)(fNumber * (float)theApp.m_Axes[theApp.m_Tank.nRLeft].nStepsPerRev) + theApp.m_Axes[theApp.m_Tank.nRLeft].nPos;
	nDesirePos %= theApp.m_Axes[theApp.m_Tank.nRLeft].nStepsPerRev;

	theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos=nDesirePos;

	EnableAxis(theApp.m_Tank.nRLeft,MOVETO,(int)fSpeed);
}


void CSi10::MoveTurntableCartesianScanningAxes(CCoord Cp)
{
	DownLoadCoord(Cp);

	//MoveTo Cartesian Scanning Axes
	//nDesirePos alread initialized
	for(int nAxis = 0;nAxis<24;nAxis++) {
		if((theApp.m_Axes[nAxis].bAvailable==TRUE) && (theApp.m_Axes[nAxis].nMotorType==STEPPER) && (nAxis!=theApp.m_Tank.nRLeft) ) {
			EnableAxis(nAxis,MOVETO);
		}
	}

}

void CSi10::EnableLights(int nFlag)
{
	int nn = m_nGeneralOutput;

	int nMask = ~(theApp.m_Tank.nAndonOkay | theApp.m_Tank.nAndonCycleRunning | theApp.m_Tank.nAndonWarning | theApp.m_Tank.nAndonFault);
	nn&=nMask;
	nn|= nFlag;

	if(nn != m_nGeneralOutput) {
		WriteGeneralOuput(m_nGeneralOutput = nn);
	}
}

void CSi10::OpenLaserDoor()
{
	WriteGeneralOuput(m_nGeneralOutput &= 0xfe);
}

void CSi10::CloseLaserDoor()
{
	WriteGeneralOuput(m_nGeneralOutput |= 1);
}

void CSi10::SetScanSpeed(float fSpeed)
{
	theApp.m_Axes[theApp.m_Tank.nXLeft].nMaxSpeed = (int)(fSpeed / theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
}

int CSi10::GetFieldBusRegister()
{
	return ( m_Si10.nGeneralInput ^ 0xff);
}

void CSi10::UpdateTurntableCartesianScanningAxes(CCoord Cp)
{
	theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
	theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
}