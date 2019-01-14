// PmacUSL.cpp: implementation of the CPmacUSL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "PmacUSL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPmacUSL::CPmacUSL()
{
	m_bInterruptEnabled = FALSE;
	m_bIntCallbackThreadEnabled = FALSE;

	m_nEventsCount = 0;
	ZeroMemory((VOID*)&m_DeviceInterrupt, sizeof m_DeviceInterrupt);


}

CPmacUSL::~CPmacUSL()
{

	SuspendDriver();

}

bool CPmacUSL::SuspendDriver()
{
	int nResult;

	DisableInterrupt();
	if(m_Device.hDevice) {
		nResult = CloseHandle(m_Device.hDevice);
		m_Device.hDevice = NULL;
	}

	m_bInterruptEnabled = FALSE;
	m_bIntCallbackThreadEnabled = FALSE;
	m_nEventsCount = 0;
	ZeroMemory((VOID*)&m_DeviceInterrupt, sizeof m_DeviceInterrupt);

	return true;
}

bool CPmacUSL::StartDriver(CString &strPmacType,int *nPmacType)
{
	BSTR Date	= ::SysAllocString(L"");
	BSTR Version = ::SysAllocString(L"");
	int m_nPmacType;

	if(&m_Device == NULL)
        return false;

	if(IsObjectValid(&m_Device))
        return false;

	memset(&m_Device, 0, sizeof(PMAC_DEVICE_OBJECT));

	ObjectInvalidate( &m_Device );

	m_Device.hDevice = CreateFile(_T("\\\\.\\PmacPci0"),
										GENERIC_READ | GENERIC_WRITE,        // Desired access
										FILE_SHARE_READ | FILE_SHARE_WRITE,  // Shared mode
										NULL,                                // Security attributes
										OPEN_EXISTING,                       // Creation disposition
										FILE_ATTRIBUTE_NORMAL,               // File attributes
										NULL                                 // Template file
									);

	if(m_Device.hDevice == INVALID_HANDLE_VALUE)
		return false;

	if(Initialize() == FALSE) { 
		CloseHandle(m_Device.hDevice);
		m_Device.hDevice = NULL;
		return false;
	}

	SendStr(_T("I3=2 I4=0 I6=1 I63=1 I64=1"),false);

	GetPmacType(&m_nPmacType);
	*nPmacType = m_nPmacType;

	switch(m_nPmacType) {
	case PMAC1:	strPmacType.Format(_T("Pmac 1 %s %s"), Version ,Date);
		break;
	case PMAC2:	strPmacType.Format(_T("PMAC 2  %s %s"), Version ,Date);
		break;
	case PMAC_Ultralite:	strPmacType.Format(_T("PMAC Ultralite  %s %s"), Version ,Date);
		break;
	case Turbo_PMAC1:	strPmacType.Format(_T("Pmac Turbo 1 %s %s"), Version ,Date);
		break;
	case Turbo_PMAC2:	strPmacType.Format(_T("Pmac Turbo 2 %s %s"), Version ,Date);
		break;
	case Turbo_PMAC_Ultralite:	strPmacType.Format(_T("Turbo PMAC Ultralite %s %s"), Version ,Date);
		break;
	case Turbo_PMAC2_Ultralite:	strPmacType.Format(_T("Turbo PMAC 2 Ultralite %s %s"), Version ,Date);
		break;
	}


	EnableInterrupt();

	return true;
}


bool CPmacUSL::Initialize()
{
	if(m_Device.hDevice == NULL) return false;
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_Device.hDevice, IOCTL_MOTION_INIT,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);
	return result  & 0x01;
}

int CPmacUSL::SendStr(CString TxBuff, bool bWaitForResponce)
{
	if(m_Device.hDevice == NULL) return 0;

	DWORD BytesReturned;
	COMM_BUFFER CommBuffer;
	CString Buff;

	FlushStr();
	ZeroMemory((void*)&CommBuffer,sizeof CommBuffer);
	for(int ii=0;ii<TxBuff.GetLength();ii++) {
		CommBuffer.buffer[ii] = (char)TxBuff.GetAt(ii);
	}
	CommBuffer.maxChars = strlen(CommBuffer.buffer) + 1;
	BOOL result = DeviceIoControl( m_Device.hDevice, IOCTL_MOTION_WRITELN_PORT,
							(VOID*)&CommBuffer, sizeof CommBuffer,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);


	return 0;

}

bool CPmacUSL::IsStrReadyRead()
{
	if(m_Device.hDevice == NULL) return false;
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_Device.hDevice, IOCTL_MOTION_PORT_READREADY,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);

	if((result == 1) && (BytesReturned>0)) {
		return true;
	}
	return false;

}

void CPmacUSL::FlushStr()
{
	if(m_Device.hDevice == NULL) return;
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_Device.hDevice, IOCTL_MOTION_FLUSH_PORT,
							NULL, 0,			// Input
							NULL, 0,			// Output
							&BytesReturned, NULL);

	return;

}

bool CPmacUSL::GetStr(CString &RxBuff)
{
	if(m_Device.hDevice == NULL) return false;

	DWORD BytesReturned;
	COMM_BUFFER CommBuffer;
	CString Temp;
	int ii=0;
	BOOL result = 1;

	RxBuff.Empty();
	Temp.Empty();
	while(result)
	{
		ZeroMemory((void*)&CommBuffer,sizeof CommBuffer);
		CommBuffer.maxChars = sizeof CommBuffer.buffer;

		result = DeviceIoControl( m_Device.hDevice, IOCTL_MOTION_READLN_PORT,
								NULL, 0,			// Input
								(PVOID)&CommBuffer, sizeof CommBuffer,			// Output
								&BytesReturned, NULL);

		if(result==1) {
			Temp.GetBufferSetLength(CommBuffer.count);
			for(ii=0;ii<CommBuffer.count  && CommBuffer.buffer[ii] != 0;ii++) {
				Temp.SetAt(ii,CommBuffer.buffer[ii]);
			}
			Temp.GetBufferSetLength(ii);
			RxBuff += Temp;

			switch(CommBuffer.status) {
			case 0:
				break;
			case COMM_EOT:
				return true;
				break;
			case COMM_TIMEOUT:
				return false;
				break;
			case COMM_ERROR:
				break;
			case COMM_FAIL:
				break;
			}
		}
	}
	return false;
}

bool CPmacUSL::RequestVariable(CString VariableName, int nOffset,  CString &RxBuff)
{
	if(m_Device.hDevice == NULL) return 0;
	CString Buff;
	int nn;

	Buff.Format(_T("%s%d"),VariableName,nOffset);
	int nTimeout = 0;
	SendStr(Buff);

	RxBuff.Empty();
	while(RxBuff.GetLength()==0) {
		GetStr(RxBuff);
		nTimeout++;
		if(nTimeout>20) {
//			ASSERT( false );
			return false;
		}
	}
	if(RxBuff.GetLength()>0) {
		if(isascii(RxBuff.GetAt(0))) {
			nn=RxBuff.Find(_T("\r"),0);
			if((nn>0) && (nn<20)) {
				RxBuff.GetBufferSetLength(nn);
				return true;
			}
		}
	}
	return false;

}

bool CPmacUSL::RequestSlaveVariable(CString VariableName, int nNode, int nOffset, CString &RxBuff)
{

	if(m_Device.hDevice == NULL) return 0;
	CString Buff;
	int nn;

	Buff.Format(_T("MS%d,M%s%d"),nNode,VariableName,nOffset);
	int nTimeout = 0;
	SendStr(Buff);

	RxBuff.Empty();
	while(RxBuff.GetLength()==0) {
		GetStr(RxBuff);
		nTimeout++;
		if(nTimeout>10) return false;
	}
	if(RxBuff.GetLength()>0) {
		if(isascii(RxBuff.GetAt(0))) {
			nn=RxBuff.Find(_T("\r"),0);
			if((nn>0) && (nn<20)) {
				RxBuff.GetBufferSetLength(nn);
				return true;
			}
		}
	}
//	ASSERT(false);
	return false;
}


int CPmacUSL::RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff)
{
	if(m_Device.hDevice == INVALID_HANDLE_VALUE) return false;
	if(m_Device.hDevice == NULL) return 0;
	int nTimeout = 0;
	CString	Buff;

	Buff.Format(_T("%s%d..%d"),VariableName,nStartOffset,nFinishOffset);
	SendStr(Buff);

	RxBuff.Empty();
	while(RxBuff.GetLength()==0) {
		GetStr(RxBuff);
		nTimeout++;
	}
	return RxBuff.GetLength();

}


int CPmacUSL::RequestMVariableDefinitions(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff)
{
	if(m_Device.hDevice == INVALID_HANDLE_VALUE) return false;
	if(m_Device.hDevice == NULL) return 0;
	int nTimeout = 0;
	CString	Buff;

	Buff.Format(_T("%s%d..%d->"),VariableName,nStartOffset,nFinishOffset);
	SendStr(Buff);

	RxBuff.Empty();
	while(RxBuff.GetLength()==0) {
		GetStr(RxBuff);
		nTimeout++;
	}
	return RxBuff.GetLength();

}


int CPmacUSL::ReadDpram(int nOffset)
{
	long nValue;

	DpramGetMem(nOffset,1,(PVOID)&nValue);

	return nValue;
}

void CPmacUSL::WriteDpram(int nOffset, int nValue, bool bDoMemCheck)
{
	int nTemp = nValue;

	DpramSetMem(nOffset,1,(PVOID)&nTemp,bDoMemCheck);

}

int CPmacUSL::DpramGetMem(int nOffset,int nLength,PVOID pMem)
{
	if(m_Device.hDevice == NULL) return 0;


	DWORD BytesReturned;
	int nByteOffset = nOffset * 4; 

	BOOL result = DeviceIoControl( m_Device.hDevice, IOCTL_MOTION_GET_MEM,
							(PVOID)&nByteOffset, 4,			// Input
							pMem, nLength * 4,			// Output
							&BytesReturned, NULL);


	return 1;
}

int CPmacUSL::DpramSetMem(int nOffset,int nLength,PVOID pMem, bool bDoMemCheck)
{
	if(m_Device.hDevice == NULL) return 0;

	DWORD BytesReturned;
	DWORD *pOffset = (DWORD*) pMem;
	int nReadArray[1024];
	int *pWriteArray = (int*)pMem;
	int ii;

	theApp.m_LastSettings.bDPRamWriteMemCheck = TRUE;
again:
	int nByteOffset = nOffset * 4; 
	BOOL result = DeviceIoControl( m_Device.hDevice, IOCTL_MOTION_SET_MEM,
							pMem, nLength * 4,			// Input
							(PVOID)&nByteOffset, 4,				// Output
							&BytesReturned, NULL);

	if(bDoMemCheck == TRUE) {
		if(theApp.m_LastSettings.bDPRamWriteMemCheck == TRUE) {
			DpramGetMem(nOffset,nLength,nReadArray);

			for(ii=0;ii<nLength;ii++) {
				if(nReadArray[ii] != pWriteArray[ii]) {
					theApp.m_PmacUser.m_nDPRAMError++;
					goto again;
				}
			}
		}
	}


	return 1;
}

void CPmacUSL::EnableInterrupt()
{
	DWORD BytesReturned;
	DWORD nIntMask;

	if(m_Device.hDevice == NULL) return;

	if(m_bInterruptEnabled == FALSE) {

		m_bIntCallbackThreadEnabled = true;
		m_pInterruptThread=AfxBeginThread(&PendForPmacInterrupt,this,THREAD_PRIORITY_TIME_CRITICAL ,0,NULL);


		SendStr(L"DISABLE PLCC 0..31");
		SendStr(L"DISABLE PLC 0..31");
		SendStr(L"I5=0");
		
		for(int ii = 0;ii<MAX_INT_LEVELS;ii++) {
			m_DeviceInterrupt.hEvent[ii] = CreateEvent(NULL, FALSE, FALSE, NULL);
		}

		DeviceIoControl( m_Device.hDevice, IOCTL_REF_INTR_EVENTS,
								&m_DeviceInterrupt, sizeof(m_DeviceInterrupt),		// Input
								NULL, 0,		// Output
								&BytesReturned, NULL);

		nIntMask = 0xffffffbf;
		DeviceIoControl(  m_Device.hDevice, IOCTL_MOTION_SET_INTR_MASK,
								&nIntMask, sizeof(nIntMask),		// Input
								NULL, 0,		// Output
								&BytesReturned, NULL);


		if(theApp.m_nJobNumber==DOWTY_BSCAN) {
			//For Some Reason the NON-turbo PCI PMAC won't allow me to use the PLCC's
			SendStr(L"ENABLE PLC 1");
			SendStr(L"ENABLE PLC 0");
		}else{
			SendStr(L"ENABLE PLCC 1");
			SendStr(L"ENABLE PLCC 0");
		}

		SendStr(L"I5=3");
		SendStr(L"I8=0");


		m_bInterruptEnabled = TRUE;
	}
}

void CPmacUSL::DisableInterrupt()
{
	DWORD nIntMask;
	DWORD BytesReturned;
	if(m_Device.hDevice == NULL) return;

	if(m_bInterruptEnabled == TRUE) {
		SendStr(L"DISABLE PLCC 0..31");
		SendStr(L"DISABLE PLC 0..31");
		SendStr(L"I5=0");

		nIntMask = 0xffffffff;
		DeviceIoControl(  m_Device.hDevice, IOCTL_MOTION_SET_INTR_MASK,
								&nIntMask, sizeof(nIntMask),		// Input
								NULL, 0,		// Output
								&BytesReturned, NULL);


		if(m_bIntCallbackThreadEnabled) {
			m_bIntCallbackThreadEnabled = false;
			WaitForSingleObject(m_pInterruptThread,INFINITE);
		}
		m_bInterruptEnabled = FALSE;
	}

}

UINT PendForPmacInterrupt(LPVOID pParam)
{
	CPmacUSL* pParent = (CPmacUSL *)pParam;

	pParent->m_bIntCallbackThreadFinished = FALSE;
	while(pParent->m_bIntCallbackThreadEnabled == TRUE) {
		if(WaitForSingleObject(pParent->m_DeviceInterrupt.hEvent[6], 100) == WAIT_OBJECT_0) {
			pParent->m_nEventsCount++;
//			theApp.m_PmacUser.nIrqCount++;

			pParent->InterruptServiceRoutine();


			ResetEvent(pParent->m_DeviceInterrupt.hEvent[0]);
		}

	}
	pParent->m_bIntCallbackThreadFinished = TRUE;
	TRACE0("SI10 Interrupt callback finished\n");

	return 0;


}


void CPmacUSL::InterruptServiceRoutine()
{
	int	nAxis,nVAxis;
	int nOldPos,nPos;
	static	bool bFlag=FALSE;
	static int nStatusLoop=0;
	int	nRollOverMode = 1;
	int nTemp;
	CCoord CpSurface;

	theApp.m_PmacUser.nIrqCount++;
	DpramGetMem(0,0x68,(PVOID)m_nLowerBlock);
	DpramGetMem(0x3B0,0x20,(PVOID)m_nUpperBlock);

	for(nAxis=0;nAxis<12;nAxis++) {
		theApp.m_PmacUser.m_nMachinePosition[nAxis]=m_nLowerBlock[nAxis];
		if(m_nLowerBlock[nAxis] >= m_nOldPos[nAxis]) {
			theApp.m_PmacUser.m_nPosition[nAxis] = theApp.m_Kinematics.m_LeadScrew[nAxis].MachineToActualPos(m_nLowerBlock[nAxis]);
		} else {
			theApp.m_PmacUser.m_nPosition[nAxis] = theApp.m_Kinematics.m_LeadScrew[nAxis].MachineToActualNeg(m_nLowerBlock[nAxis]);
		}
		m_nOldPos[nAxis] = m_nLowerBlock[nAxis];
	}

	if(nStatusLoop>100) {
		theApp.m_PmacUser.m_nGeneralInput = theApp.m_PmacUser.ReadDpram(PMAC_GENERAL_INPUTS);
		theApp.m_PmacUser.m_nGeneralOutput = theApp.m_PmacUser.ReadDpram(PMAC_GENERAL_OUTPUTS);
		nStatusLoop = 0;
	}
	CopyMemory(theApp.m_PmacUser.m_nStatus,			&m_nLowerBlock[0x30],14 * sizeof (int));
	CopyMemory(theApp.m_PmacUser.m_nFollowingError,	&m_nLowerBlock[0x40],12 * sizeof (int));
	CopyMemory(theApp.m_PmacUser.m_nVelocity,			&m_nUpperBlock[0x00],32 * sizeof (int));
	CopyMemory(theApp.m_PmacUser.m_nDAC,				&m_nUpperBlock[0x10],32 * sizeof (int));
	CopyMemory(theApp.m_PmacUser.m_nPumpsVanes,		&m_nUpperBlock[0x0C],4 * sizeof (int));
	theApp.m_PmacUser.m_nProgramStatus = m_nLowerBlock[0x60];
	theApp.m_PmacUser.m_nCoordinateStatus = m_nLowerBlock[0x67];
	theApp.m_PmacUser.m_nProgramNumber[0] = m_nUpperBlock[0x1E];
	theApp.m_PmacUser.m_nProgramNumber[1] = m_nUpperBlock[0x1F];

	//Virtual axes
	if(theApp.m_PmacUser.m_bRetrieveVirtualAxes == true) {
		DpramGetMem(0x400,0x50,(PVOID)m_nVirtualBlock);
		CopyMemory(&theApp.m_PmacUser.m_nStatus[20],			&m_nVirtualBlock[0x30],12 * sizeof (int));

		for(nAxis=0,nVAxis=20;nAxis<12;nAxis++,nVAxis++) {
			theApp.m_PmacUser.m_nMachinePosition[nVAxis]=m_nVirtualBlock[nAxis];
			if(m_nVirtualBlock[nAxis] >= m_nOldPos[nVAxis]) {
				theApp.m_PmacUser.m_nPosition[nVAxis] = theApp.m_Kinematics.m_LeadScrew[nAxis].MachineToActualPos(m_nVirtualBlock[nAxis]);
			} else {
				theApp.m_PmacUser.m_nPosition[nVAxis] = theApp.m_Kinematics.m_LeadScrew[nAxis].MachineToActualNeg(m_nVirtualBlock[nAxis]);
			}
			m_nOldPos[nVAxis] = m_nVirtualBlock[nAxis];
			nPos = theApp.m_PmacUser.m_nPosition[nVAxis]-theApp.m_Axes[nVAxis].nOffset;
			if(theApp.m_Axes[nAxis].nStepsPerRev==0) {
				theApp.m_Axes[nVAxis].nPos = nPos;
			} else {
				theApp.m_Axes[nVAxis].nRevCounter = nPos / theApp.m_Axes[nAxis].nStepsPerRev;
				nPos%=theApp.m_Axes[nAxis].nStepsPerRev;
				theApp.m_Axes[nVAxis].nPos = nPos;
			}
			theApp.m_Axes[nVAxis].nStatus =			theApp.m_PmacUser.m_nStatus[nVAxis];
		}

	}

	for(nAxis=0;nAxis<14;nAxis++) {
		if((theApp.m_Axes[nAxis].nMotorType == SERVO) && (theApp.m_Axes[nAxis].bAvailable==TRUE)) {
			
			nPos = theApp.m_PmacUser.m_nPosition[theApp.m_Axes[nAxis].nPhysicalAxis-1]-theApp.m_Axes[nAxis].nOffset;

			switch(theApp.m_LastSettings.nManipulatorType[PORTSIDE]) {
			case ROTATE_DIF_THETA:
				if(nAxis==theApp.m_Tank.nXtLeft) theApp.m_fRotateThetaRevNumber[PORTSIDE] = (float)nPos/65536.0f;
				if(nAxis==theApp.m_Tank.nYtLeft) nPos -= theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos;	//This part due to the differential
				break;
			case ROTATE_THETA_HORIZONTAL:
			case ROTATE_THETA_VERTICAL:
				if(nAxis==theApp.m_Tank.nXtLeft) theApp.m_fRotateThetaRevNumber[PORTSIDE] = (float)nPos/65536.0f;
				break;
			}
			switch(theApp.m_LastSettings.nManipulatorType[STARBOARD]) {
			case ROTATE_DIF_THETA:
				if(nAxis==theApp.m_Tank.nXtRight) theApp.m_fRotateThetaRevNumber[STARBOARD] = (float)nPos/65536.0f;
				if(nAxis==theApp.m_Tank.nYtRight) nPos -= theApp.m_Axes[theApp.m_Tank.nXtRight].nPos;	//This part due to the differential
				break;
			case ROTATE_THETA_HORIZONTAL:
			case ROTATE_THETA_VERTICAL:
				if(nAxis==theApp.m_Tank.nXtRight) theApp.m_fRotateThetaRevNumber[STARBOARD] = (float)nPos/65536.0f;
				break;
			}

			if(theApp.m_Axes[nAxis].nStepsPerRev==0) {
				theApp.m_Axes[nAxis].nPos = nPos;
			} else {

				if(nRollOverMode == 0) {
					nOldPos=theApp.m_Axes[nAxis].nPos;
					
					theApp.m_Axes[nAxis].nRevCounter = nPos / theApp.m_Axes[nAxis].nStepsPerRev;

					if(bFlag==FALSE) theApp.m_PmacUser.m_nLastPos[nAxis]=nPos;
					nPos=theApp.m_Axes[nAxis].nPos + (nPos-theApp.m_PmacUser.m_nLastPos[nAxis]);
					nPos%=theApp.m_Axes[nAxis].nStepsPerRev;
					if(nPos<0) {
						nPos+=theApp.m_Axes[nAxis].nStepsPerRev;
					}
					theApp.m_Axes[nAxis].nPos = nPos;
					theApp.m_PmacUser.m_nLastPos[nAxis]=nPos;
					if(abs(nOldPos - nPos)>(theApp.m_Axes[nAxis].nStepsPerRev / 2)) {
						theApp.m_Motors.m_nRevCounter++;
					}
				} else {
					theApp.m_Axes[nAxis].nRevCounter = nPos / theApp.m_Axes[nAxis].nStepsPerRev;
					nPos%=theApp.m_Axes[nAxis].nStepsPerRev;
					nOldPos=theApp.m_Axes[nAxis].nPos;
					theApp.m_Axes[nAxis].nPos = nPos;
					if(abs(nPos - nOldPos) > (theApp.m_Axes[nAxis].nStepsPerRev / 2)) {
						theApp.m_Motors.m_nRevCounter++;
					}
				}
			};
			theApp.m_Axes[nAxis].nStatus =			theApp.m_PmacUser.m_nStatus[theApp.m_Axes[nAxis].nPhysicalAxis-1];
			theApp.m_Axes[nAxis].nCurrentSpeed =		theApp.m_PmacUser.m_nVelocity[theApp.m_Axes[nAxis].nPhysicalAxis-1];
			theApp.m_Axes[nAxis].nFollowingError =	theApp.m_PmacUser.m_nFollowingError[theApp.m_Axes[nAxis].nPhysicalAxis-1];
			theApp.m_Axes[nAxis].nDAC =				theApp.m_PmacUser.m_nDAC[theApp.m_Axes[nAxis].nPhysicalAxis-1];
		}
	};
	
	bFlag = TRUE;
	nStatusLoop++;
	


	if(theApp.m_bCollectComplexData==TRUE) {
		theApp.m_pScanData->CollectComplexData();

		nTemp=theApp.m_LastSettings.nPositionDisplayMode;
		theApp.m_LastSettings.nPositionDisplayMode=2;
		theApp.m_Motors.GetSurfacePos(&CpSurface);
//		PROFILE->m_ScanLine[theApp.m_Thread.m_nScanLine].GetElectronicValues(&CpSurface,0);
		theApp.m_LastSettings.nPositionDisplayMode=nTemp;
		theApp.m_UtUser.SetAllUt(CpSurface,PROFILE->m_nHeader);
	}
	if(theApp.m_bCollectNormalizeData==TRUE) {
		theApp.m_Thread.CollectNormalizeData();
	}
	if(theApp.m_bCollectBladeData==TRUE) {
		theApp.m_pScanData->CollectBladeData();
	}
	if(theApp.m_bCollectTunning==TRUE) {
		theApp.m_Thread.CollectTunning();
	}
	if(theApp.m_bCollectEccentricData==TRUE) {
		theApp.m_Thread.CollectEccentricData();
	}
	if(theApp.m_bRThetaJoystick==true) {
		theApp.m_PmacUser.RotateThetaJoystick(0);
	}
	if(theApp.m_bCollectWaterPath==TRUE) {
		theApp.m_Thread.CollectWaterPathData();
	}
	if(	theApp.m_bLaserCollectTaughtLines==TRUE) {
		theApp.m_Thread.CollectLaserTaughtLineData();
	}
	if(theApp.m_Thread.m_bCollectTrajectory==TRUE) {
		theApp.m_Thread.CollectTrajectory();
	}

	theApp.m_DataTranslationHW.ReadAllADC();
}

bool CPmacUSL::GetPmacType(PVOID pMem)
{
	if(m_Device.hDevice == NULL) return false;
	DWORD BytesReturned;

	BOOL result = DeviceIoControl( m_Device.hDevice, IOCTL_MOTION_DEV_TYPE,
							NULL, 0,			// Input
							pMem, 4,			// Output
							&BytesReturned, NULL);

	if((result == 1) && (BytesReturned>0)) {
		return true;
	}
	return false;

}

