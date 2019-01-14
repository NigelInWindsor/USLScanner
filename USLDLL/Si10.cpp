/*********************************************************************************
   File name: Si10.cpp

   The header file, USLDLL.h, prototypes all of the DLL interface objects 
**********************************************************************************/
#include "Stdafx.h"
#include "USLDLL.h"
#include <iostream>


using namespace std;

#define PCI_DEVICE_ID 0x9030;
#define PCI_VENDOR_ID 0x10B5;




	SI10Data			m_Si10Data;
	AxisData			m_Axis[10];
	PLX_DEVICE_OBJECT	m_Device;
	bool				m_bDevicePresent;
	int					m_nCountersEnabled;

	char	m_Array[200000];


Si10class::Si10class() {};

Si10class::~Si10class() {};

int Si10class::DeviceOpen()
{

	PLX_PARAMS			IoBuffer;
	PLX_DEVICE_KEY		Key;
	unsigned long BytesReturned;
	int ReturnCode=0;
	int nResult;
	int nAxis;

	m_bDevicePresent = false;
	memset(&m_Device, 0, sizeof(PLX_DEVICE_OBJECT));

	ObjectInvalidate( &m_Device );

	m_Device.Key.ApiMode = PLX_API_MODE_PCI;

	m_Device.hDevice = CreateFile("\\\\.\\plx9030_v600-0",
										GENERIC_READ | GENERIC_WRITE,        // Desired access
										FILE_SHARE_READ | FILE_SHARE_WRITE,  // Shared mode
										NULL,                                // Security attributes
										OPEN_EXISTING,                       // Creation disposition
										FILE_ATTRIBUTE_NORMAL,               // File attributes
										NULL                                 // Template file
									);

	if(m_Device.hDevice == INVALID_HANDLE_VALUE) return SI10ApiFailedToAssignHandle;

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
		m_Device.hDevice = 0;
		return SI10ApiHardwareNotFound;
	}

	m_Device.Key = IoBuffer.Key;
	m_Device.Key.ApiIndex = 0;
	m_Device.Key.ApiMode = PLX_API_MODE_PCI;

	ObjectValidate( &m_Device.Key );
	ObjectValidate( &m_Device );


	m_Si10Data.bGenerateEvent = false;
	m_Si10Data.bGPIOEvent = false;
	m_Si10Data.nGPIOIrqCount = 0;

	for(nAxis=0;nAxis<10;nAxis++) {
		m_Si10Data.pAxis[nAxis] = &m_Axis[nAxis];
		m_Axis[nAxis].nDriverAxisNumber = nAxis;
	}

	nResult = DeviceIoControl( m_Device.hDevice, IOCTL_SI10_SHARE_USER_STRUCT,
							&m_Si10Data, 0,		// Input
							NULL, 0,		// Output
							&BytesReturned, NULL);
	if(nResult != 1) return SI10ApiFailedToShareStructures;

	nResult = LoadFPGAConfigurationData();
	if(nResult < 0) return nResult;
	
	return SI10ApiSuccess;

}

int Si10class::DeviceClose()
{

	if(m_bDevicePresent == true) {
		//Make sure no clocks are enabled
		int CounterEnabled = 0;
		SI10PciBarSpaceWrite(0x1A8,4,&CounterEnabled);

		//Turn off interrupts
		int Interrupt = 0x0;
		SI10PciBarSpaceWrite(0x1AC,4,&Interrupt);

		//Turn off GPIO interrupts
		SI10PciBarSpaceWrite(0x304,4,&Interrupt);

//		StopAllMotorsDead();

//		DeRegisterInterruptCallback();
		m_bDevicePresent = false;
	}

	if(m_Device.hDevice) 
		CloseHandle(m_Device.hDevice);

	m_Device.hDevice = 0;

	return SI10ApiSuccess;

}


int Si10class::LoadFPGAConfigurationData()
{

	CFile FileHandle;
	unsigned long BytesReturned;
	int nLength=0;
	char FileName[256];
	char ss[256];
	char *pss;
	pss=ss;

	if(isFPGALoaded())
		return SI10ApiSuccess;


	FindLatestBinaryFile(FileName);

	/* Now try to open file for Read, Binary */
	if (!FileHandle.Open(FileName,CFile::typeBinary|CFile::modeRead))
		return SI10ApiFailedToOpenConfigurationFile;

	nLength = FileHandle.GetLength();
	FileHandle.Read(m_Array,nLength);
	FileHandle.Close();

	m_Si10Data.pData = m_Array;
	m_Si10Data.nDataSize = nLength;

	if((m_Si10Data.pData != NULL) && (m_Si10Data.nDataSize>0)) {

	int result = DeviceIoControl( m_Device.hDevice, IOCTL_SI10_LOAD_FPGA_CODE,
							NULL, 0,   // Input
							NULL, 0,   // Output
							&BytesReturned, NULL);

	}


	Sleep(4000);

	return isFPGALoaded();


}

int Si10class::FindLatestBinaryFile(char* pStr)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = NULL;
	char FilePath[260];
	char Directory[256];
	int nHighest=0;

	GetSystemDirectory(Directory,256);

	sprintf(FilePath,"%s\\drivers\\si10v1_fpgava1*.bin",Directory);
	sprintf(pStr,"%s\\drivers\\si10v1_fpgava1.bin",Directory);

	hFind = FindFirstFile(FilePath, &ffd);
	if (hFind != INVALID_HANDLE_VALUE )
	{
		do{
			if( (atoi(ffd.cFileName+14)) > nHighest) {
				nHighest=atoi(ffd.cFileName+14);
				sprintf(pStr,"%s\\drivers\\%s",Directory,ffd.cFileName);
			}

		} while( FindNextFile(hFind, &ffd)!= 0 );

	}else{
		return SI10ApiFailedToOpenConfigurationFile;
	}

	return SI10ApiSuccess;


}

bool Si10class::isFPGALoaded()
{

	int TempBuffU32[256];
	U8  BitCtr;
	U8  TempUCh;

	SI10PciBarSpaceRead(0x3FC,0x4,TempBuffU32);

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
	// shift down for next 8 bits
	TempBuffU32[0] >>= 8;
	}

	return true;
}


bool Si10class::SI10PciBarSpaceRead(int addrs, int ByteCount,int *pData)
{
	PLX_PARAMS IoBuffer;
	bool bReturn = true;

	if (!IsObjectValid(&m_Device)) return false;

//	m_CriticalSection.Lock();

	IoBuffer.value[0]             = (U8) 2;
    IoBuffer.value[1]             = (PLX_ACCESS_TYPE) BitSize32;
    IoBuffer.value[2]             = (BOOLEAN) TRUE;

    IoBuffer.u.TxParams.LocalAddr = addrs;
    IoBuffer.u.TxParams.ByteCount = ByteCount;
    IoBuffer.u.TxParams.u.UserVa  = (PLX_UINT_PTR)pData;


	unsigned long BytesReturned;

	DeviceIoControl(m_Device.hDevice, PLX_IOCTL_PCI_BAR_SPACE_READ,
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Input
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Output
							&BytesReturned,NULL);

	if( IoBuffer.ReturnCode != SI10ApiSuccess) bReturn = false;

//	m_CriticalSection.Unlock();
	return bReturn;

}



bool Si10class::SI10PciBarSpaceWrite(int addrs, int ByteCount,int *pData)
{
	PLX_PARAMS IoBuffer;
	bool bReturn = true;

	if (!IsObjectValid(&m_Device)) return false;

//	m_CriticalSection.Lock();

	IoBuffer.value[0]             = (U8) 2;
    IoBuffer.value[1]             = (PLX_ACCESS_TYPE) BitSize32;
    IoBuffer.value[2]             = (BOOLEAN) TRUE;

    IoBuffer.u.TxParams.LocalAddr = addrs;
    IoBuffer.u.TxParams.ByteCount = ByteCount;
    IoBuffer.u.TxParams.u.UserVa  = (PLX_UINT_PTR)pData;


	unsigned long BytesReturned;

	DeviceIoControl(m_Device.hDevice, PLX_IOCTL_PCI_BAR_SPACE_WRITE,
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Input
							(VOID*) &IoBuffer, sizeof(PLX_PARAMS),			// Output
							&BytesReturned,NULL);

	if( IoBuffer.ReturnCode != SI10ApiSuccess) bReturn = false;

//	m_CriticalSection.Unlock();

	return bReturn;

}


int Si10class::InpW(int nOffset)
{
  	int nResult = 0;

	SI10PciBarSpaceRead(nOffset,0x4,&nResult);

	return nResult;

}


void Si10class::OutpW(int nOffset, int Value)
{

	SI10PciBarSpaceWrite(nOffset,0x4,&Value);

	return;
}

int Si10class::EnableAxis(int nAxis,int action)
{
	int nReturnCode;

	if((nAxis<0) || (nAxis>9)) return SI10ApiSelectedAxisOutOfRange;

	struct AxisData *axis = &m_Axis[nAxis];


	int	nSpeed;
	if(axis->nIrqMask==0)		return SI10ApiIrqMaskNotSet;
	if(axis->bAvailable==FALSE) return SI10ApiAxisNotAvailable;

	if((nReturnCode = DisableAxis(nAxis)) != SI10ApiSuccess) return nReturnCode;

	(axis->nPos > axis->nDesirePos)?
		(m_Si10Data.nDirReg|=axis->nBit):(m_Si10Data.nDirReg&=(~axis->nBit));

	axis->nCount=axis->nPos;
	switch(action) {
	case CONTINUOUS_ROTATE:
	case MOVETO:	axis->nCurrentSpeed = axis->nMinSpeed;
		if(axis->nDesirePos-axis->nPos) m_Si10Data.nProcessState|=axis->nIrqMask;
		break;
	case JOYSTICK:
		m_Si10Data.nJoystickPercent=100;
		axis->nCurrentSpeed = MulDiv(axis->nJoystickMinSpeed,m_Si10Data.nJoystickPercent,100);
		switch(m_Si10Data.nJoystickSource) {
		case WIRED_SI10: getSi10ReadJoystick(axis->nJoystickNumber,&axis->nJoystickZero);
			break;
		case WIRED_SERIAL:
		case WIRELESS_SERIAL: axis->nJoystickZero=m_Si10Data.nJoystickValue[axis->nJoystickNumber];
			break;
		}
		m_Si10Data.nProcessState&=(~axis->nIrqMask);
		break;
	case HOMETO:
		axis->nHomeAction = 0;
		axis->nCurrentSpeed = axis->nMinSpeed;
		(axis->nHomeSpeed<0)?(m_Si10Data.nDirReg|=axis->nBit):(m_Si10Data.nDirReg&=(~axis->nBit));
		m_Si10Data.nProcessState|=axis->nIrqMask;
		axis->bHomeFlag=FALSE;
		break;
	};

	m_Si10Data.nIrqAction[axis->nDriverAxisNumber]=action;

	OutpW(SI10_DIR_PORT,m_Si10Data.nDirReg ^ m_Si10Data.nDirEor);

	if(axis->nCurrentSpeed==0) axis->nCurrentSpeed=1;
	nSpeed=1000000/axis->nCurrentSpeed;
	if(nSpeed>0xffff) nSpeed=0xffff;

	CounterStart(axis->nCounter,nSpeed);

	m_nCountersEnabled |= (axis->nIrqMask);
	OutpW(0x1A8,m_nCountersEnabled);

	return SI10ApiSuccess;

}


int Si10class::DisableAxis(int nAxis)
{
	if((nAxis<0) || (nAxis>9)) return SI10ApiSelectedAxisOutOfRange;
	struct AxisData *axis = &m_Axis[nAxis];

	if(axis->nIrqMask==0)		return SI10ApiIrqMaskNotSet;
	if(axis->bAvailable==FALSE) return SI10ApiAxisNotAvailable;

	m_nCountersEnabled &= (~axis->nIrqMask);


	m_Si10Data.nIrqAction[nAxis]=0;

	OutpW(0x1A8,m_nCountersEnabled);

	m_Si10Data.nProcessState&=(~axis->nIrqMask);

	return SI10ApiSuccess;
}

int	Si10class::CounterStart(int nNumber,int nValue)
{

	switch(nNumber) {
	case 0:
		OutpW(SI10_COUNT_0,nValue&0xffff);
		break;
	case 1:
		OutpW(SI10_COUNT_1,nValue&0xffff);
		break;
	case 2:
		OutpW(SI10_COUNT_2,nValue&0xffff);
		break;
	case 3:
		OutpW(SI10_COUNT_3,nValue&0xffff);
		break;
	case 4:
		OutpW(SI10_COUNT_4,nValue&0xffff);
		break;
	case 5:
		OutpW(SI10_COUNT_5,nValue&0xffff);
		break;
	case 6:
		OutpW(SI10_COUNT_6,nValue&0xffff);
		break;
	case 7:
		OutpW(SI10_COUNT_7,nValue&0xffff);
		break;
	case 8:
		OutpW(SI10_COUNT_8,nValue&0xffff);
		break;
	case 9:
		OutpW(SI10_COUNT_9,nValue&0xffff);
		break;
	};
	return SI10ApiSuccess;

}

bool Si10class::isSi10AxisRunning(int nAxis)
{

	if(m_Axis[nAxis].bAvailable==TRUE) {
		if(m_Axis[nAxis].nIrqMask & m_Si10Data.nProcessState) return TRUE;
	}
	return FALSE;

}


int	Si10class::EncoderRead(int nEncoderNumber)
{
	int nResult;

	SI10PciBarSpaceRead(SI10_ENCODER_0 + nEncoderNumber * 4,0x4,&nResult);
	
	return nResult;
}

void Si10class::EncoderZero(int nEncoderNumber)
{
	int nValue = 0x0;

	SI10PciBarSpaceWrite(SI10_ENCODER_0 + nEncoderNumber * 4,0x4,&nValue);

}

///////////////////////////////////////////////////////////////////////////////////////
//  GLOBAL FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////

int	Si10DeviceOpen()
{
	Si10class Si10;

	return Si10.DeviceOpen();
}

int	Si10DeviceClose()
{
	Si10class Si10;

	return Si10.DeviceClose();
}


int	getSi10ReadJoystick(int nWhich, int *nValue)
{
	Si10class Si10;

	if((nWhich<0) || (nWhich>3)) return SI10ApiSelectedJoystickOutOfRange;

	int nReturnValue = Si10.InpW(SI10_JOYSTICK);

	*nValue = (nReturnValue>>(nWhich*8)) & 0x000000FF;

	return SI10ApiSuccess;
}

int	setSi10CardDefaults()
{
	Si10class Si10;

	int CounterEnabled = 0;
	Si10.SI10PciBarSpaceWrite(0x1A8,4,&CounterEnabled);

	//generators to be software triggered, clk causes interrupt, ISR decides whether to issues step output
	int StepClk = 0x0000;
	Si10.SI10PciBarSpaceWrite(0x200,4,&StepClk);

	//Turn on interrupts for all 10 stepper outputs
	int Interrupt = 0x03FF;
	Si10.SI10PciBarSpaceWrite(0x1AC,4,&Interrupt);

	//Turn on interrupt for all 8 GPIO bits Rising edge on all interrupts 0x5555
	//Turn on interrupt for all 8 GPIO bits Falling edge on all interrupts 0xffff
	Interrupt = 0xffff;
	Si10.SI10PciBarSpaceWrite(0x304,4,&Interrupt);

	//Turn ON Enable Latch on Z register
	int LatchOnZ = 0x03FF;
	Si10.SI10PciBarSpaceWrite(0x068,4,&LatchOnZ);

	//Turn OFF Enable Latch on Limit
	int LatchOnLimit = 0x0000;
	Si10.SI10PciBarSpaceWrite(0x06C,4,&LatchOnLimit);

	m_Si10Data.nJoystickMode				= 0;
	m_Si10Data.nJoystickSource				= 4;		//Wired SI10
	m_Si10Data.nJoystickPercent				= 100;
	m_Si10Data.nJoystickLeftSideBit			= 0;
	m_Si10Data.nJoystickRightSideBit		= 0;
	m_Si10Data.nJoystickDeadManBit0			= 0;
	m_Si10Data.nJoystickDeadManBit1			= 0;
	m_Si10Data.nJoystickMainManipBit		= 0;
	m_Si10Data.nJoystickScannerFixtureBit	= 0;
	m_Si10Data.nJoystickSpeedBit			= 0;

	return SI10ApiSuccess;

}

int setSi10AxisDefaults(int nAxis)
{
	int	nMask=1;

	memset(&m_Axis[nAxis],0,sizeof AxisData);
	m_Axis[nAxis].fStepSize = 0.1f;
	m_Axis[nAxis].nBit = nMask<<nAxis;
	m_Axis[nAxis].nCounter=nAxis;
	nMask = 1;
	m_Axis[nAxis].nIrqMask=nMask << m_Axis[nAxis].nCounter;
	m_Axis[nAxis].nMinSpeed = 200;
	m_Axis[nAxis].nMaxSpeed = 5000;
	m_Axis[nAxis].nAcceleration=3;
	m_Axis[nAxis].nDeceleration=4;
	m_Axis[nAxis].nHomeSpeed=-400;
	m_Axis[nAxis].nJoystickMinSpeed=200;
	m_Axis[nAxis].nJoystickMaxSpeed=7000;
	m_Axis[nAxis].nJoystickMinValue=93;
	m_Axis[nAxis].nJoystickMaxValue=155;
	m_Axis[nAxis].nJoystickNumber=nAxis%4;
	m_Axis[nAxis].bAvailable=TRUE;
	m_Axis[nAxis].bJoystickAvailable=TRUE;


	return SI10ApiSuccess;
}

int	getSi10CurrentPositionReal(int nAxis, float *fPos)
{
	if((nAxis<0) || (nAxis>9)) return SI10ApiSelectedAxisOutOfRange;


	*fPos = (float)m_Axis[nAxis].nPos * m_Axis[nAxis].fStepSize;

	return SI10ApiSuccess;
}

int	getSi10CurrentPositionCount(int nAxis, int *nPos)
{
	if((nAxis<0) || (nAxis>9)) return SI10ApiSelectedAxisOutOfRange;


	*nPos = m_Axis[nAxis].nPos;

	return SI10ApiSuccess;
}

int Si10MoveAxisToReal(int nAxis, float fPos)
{
	Si10class Si10;

	if((nAxis<0) || (nAxis>9)) return SI10ApiSelectedAxisOutOfRange;

	m_Axis[nAxis].nDesirePos = (int)(fPos / m_Axis[nAxis].fStepSize);

	return Si10.EnableAxis(nAxis,MOVETO);

}

int Si10MoveAxisToCount(int nAxis, int nPos)
{
	Si10class Si10;

	if((nAxis<0) || (nAxis>9)) return SI10ApiSelectedAxisOutOfRange;

	m_Axis[nAxis].nDesirePos = nPos;

	return Si10.EnableAxis(nAxis,MOVETO);

}

bool isSi10AxisInPosition(int nAxis)
{
	Si10class Si10;
	return !Si10.isSi10AxisRunning(nAxis);
}

int getSi10EncoderValue(int nEncoderNumber, int *nValue)
{

	Si10class Si10;

	if((nEncoderNumber<0) || (nEncoderNumber>9)) return SI10ApiSelectedEncoderOutOfRange;

	*nValue = Si10.EncoderRead(nEncoderNumber);

	return SI10ApiSuccess;
}


int setSi10EncoderZero(int nEncoderNumber)
{

	Si10class Si10;

	if((nEncoderNumber<0) || (nEncoderNumber>9)) return SI10ApiSelectedEncoderOutOfRange;

	Si10.EncoderZero(nEncoderNumber);

	return SI10ApiSuccess;
}
