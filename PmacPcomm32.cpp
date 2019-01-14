// PmacPcomm32.cpp: implementation of the CPmacPcomm32 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "PmacPcomm32.h"
#include "runtime.h"
#include "HRTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPmacPcomm32::CPmacPcomm32()
{
	m_bDriverOpen = FALSE;
	m_hPmacLib=NULL;
	m_bThreadEnabled = FALSE;
	m_bThreadFinished = FALSE;

}

CPmacPcomm32::~CPmacPcomm32()
{
	if(m_bDriverOpen) {
		DisableInterrupt();
		m_bDriverOpen = !DeviceClose(m_dwDevice);
	}
	if(m_hPmacLib) {
		FreeLibrary(m_hPmacLib);
		m_hPmacLib = NULL;
	}

}


//Unicode
bool CPmacPcomm32::StartDriver(CString &m_strPmacType,int *nPmacType)
{
	char vs[30],ds[30];
	CString Buff;
	CString Version,Date;

	if(!m_hPmacLib)
		m_hPmacLib = OpenRuntimeLink();
  
	if(m_hPmacLib == NULL)
		return FALSE;

	if(m_bDriverOpen)
		return TRUE;

	m_dwDevice=0;

	if(m_dwDevice == NO_PMAC_DEVICE) {
		Buff.LoadString(IDS_No_PMAC_Not_Init);
		AfxMessageBox(Buff);
		return FALSE;
	}

	m_bDriverOpen = DeviceOpen(m_dwDevice);
//	ConfigureComms(NULL);
      
	if(m_bDriverOpen) {

		DeviceGetRomVersion(m_dwDevice,vs,30);
		DeviceGetRomDate(m_dwDevice,ds,30);

		*nPmacType = m_nPmacType = DeviceGetPmacType(m_dwDevice);

		Version.Format(_T("%s"),(CString)vs);
		Date.Format(_T("%s"),(CString)ds);

		switch(m_nPmacType) {
		case PMAC1:	m_strPmacType = _T("Pmac 1 ") + Version + Date;
			break;
		case PMAC2:	m_strPmacType.Format(_T("PMAC 2 %s %s"),vs,ds);
			break;
		case PMAC_Ultralite:	m_strPmacType.Format(_T("PMAC Ultralite %s %s"),vs,ds);
			break;
		case Turbo_PMAC1:	m_strPmacType = _T("Pmac Turbo 1 ") + Version + Date;
			break;
		case Turbo_PMAC2:	m_strPmacType = _T("Pmac Turbo 2 ") + Version + Date;
			break;
		case Turbo_PMAC_Ultralite:	m_strPmacType.Format(_T("Turbo PMAC Ultralite %s %s"),vs,ds);
			break;
		case Turbo_PMAC2_Ultralite:	m_strPmacType.Format(_T("Turbo PMAC 2 Ultralite %s %s"),vs,ds);
			break;
		}
		Sleep(200);

		EnableInterrupt();
		return TRUE;
	} else {
		Buff = "Could not initialize PMAC comms";
		AfxMessageBox(Buff);
		ConfigureComms(NULL);
		return FALSE;
	}

}


void CPmacPcomm32::ConfigureComms(HWND Wnd) 
{
  char vs[30],ds[30];

  // Close PMAC Device
  if(m_bDriverOpen) {
      m_bDriverOpen = !DeviceClose(m_dwDevice);
  }

  // Configure Device
	m_dwDevice = DeviceSelect(Wnd);
  if(m_dwDevice != NO_PMAC_DEVICE)
  {
  	m_bDriverOpen = DeviceOpen(m_dwDevice);	// Initialize Communication
  }
  else
    m_bDriverOpen = false;
 
  // Setup View to reflect device open status
  if(m_bDriverOpen) 
  {
     DeviceGetRomVersion(m_dwDevice,vs,30);
     DeviceGetRomDate(m_dwDevice,ds,30);
//     sprintf(str,TEXT("V%s %s"),vs,ds);
 //    SetTitle(str);
  }
  else 
  {
     AfxMessageBox(_T("Could not initialize PMAC Comm."));
  }
}


int CPmacPcomm32::SendStr(CString TxBuff,bool bWaitForResponce)
{
	int	ii=0;
	int	vv=0;
	int	nFlag = 0;
	int nn;
	char str[256];
	CString Buff;
	CString RxBuff;
	CString ErrorCode[20];
	ErrorCode[0]=" ";
	ErrorCode[1].LoadString(IDS_Command_Not_Allowed_During_Execution);
	ErrorCode[2]=" ";
	ErrorCode[3].LoadString(IDS_Data_Error_Unrecognised_Command);
	ErrorCode[4]=" ";
	ErrorCode[5].LoadString(IDS_Command_Not_Allowed_Unless_Buffer_Open);
	ErrorCode[6].LoadString(IDS_No_Room_In_Buffer);
	ErrorCode[7].LoadString(IDS_Buffer_In_Use);
	ErrorCode[8]=" ";
	ErrorCode[9].LoadString(IDS_Program_Structural_Error);
	ErrorCode[10].LoadString(IDS_Both_Overtravel_Limits_Set);
	ErrorCode[11].LoadString(IDS_Previous_Move_Not_Complete);
	ErrorCode[12].LoadString(IDS_Motor_Is_Open_Loop);
	ErrorCode[13].LoadString(IDS_Motor_In_CS_Not_Activated);
	ErrorCode[14].LoadString(IDS_No_Motors_In_CS);
	ErrorCode[15].LoadString(IDS_Not_Pointing_To_Valid_Program);
	ErrorCode[16].LoadString(IDS_Running_Improperly_Structured_Program);

	if(m_bDriverOpen) {
		if(GetStr(RxBuff)) {
//			TRACE0(RxBuff +"\n");
		}
//	    DeviceSendLine(m_dwDevice,(WCHAR*)LPCTSTR(TxBuff));
		for(nn=0;nn<TxBuff.GetLength();nn++) {
			str[nn]=(char)TxBuff.GetAt(nn);
		}
		str[nn]=0;
		DeviceSendLine(m_dwDevice,(WCHAR*)str);
		Sleep(1);
		if(bWaitForResponce==TRUE) {
			while(DeviceGetLine(m_dwDevice,(WCHAR*)str,255) > 0) {
				RxBuff += str;
				RxBuff += '\n';
			}
			return (RxBuff.GetLength() > 0);
		}
	}

	return nFlag;
}

BOOL CPmacPcomm32::GetStr(CString &RxBuff)
{
	char str[256];

	RxBuff.Empty();
	if(m_bDriverOpen){
		if(DeviceReadReady(0)==TRUE) {
			while(DeviceGetLine(m_dwDevice,(WCHAR*)str,255) > 0) {
			  RxBuff += str;
//			  RxBuff += '\n';
			}
			return (RxBuff.GetLength() > 0);
		  }
	}
    return FALSE;
}

int CPmacPcomm32::RequestResponce(CString TxBuff, CString *pRxBuff)
{

	SendStr(TxBuff);
	pRxBuff->Empty();
	if(m_bDriverOpen) {
		while(DeviceReadReady(0)!=TRUE) {};
		GetStr(*pRxBuff);
	} else {
		pRxBuff->Format(_T("No PMAC"));
	}
	return pRxBuff->GetLength();

}

int CPmacPcomm32::RequestVariable(CString VariableName, int nOffset, char *Result)
{
	CString Buff;
	CString RxBuff;
	TCHAR str[256];

	if(m_bDriverOpen){
		Buff.Format(_T("%s%d"),VariableName,nOffset);
		SendStr(Buff);
		while(DeviceGetLine(m_dwDevice,str,255) > 0) {
		  RxBuff += str;
		}
		strcpy_s(Result, 100,(char *)LPCTSTR(RxBuff));
	}
	return strlen(Result);

}

int CPmacPcomm32::RequestVariable(CString VariableName, int nOffset, CString *pRxBuff)
{
	CString Buff;

	Buff.Format(_T("%s%d"),VariableName,nOffset);
	SendStr(Buff);
	pRxBuff->Empty();
	if(m_bDriverOpen) {
		while(DeviceReadReady(0)!=TRUE) {
			Sleep(1);
		};
		GetStr(*pRxBuff);
		pRxBuff->Remove('\r');
	} else {
		pRxBuff->Format(_T("No PMAC"));
	}
	return pRxBuff->GetLength();

}

int CPmacPcomm32::RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, char *Result)
{
	CString	RxBuff,Buff;
	char str[256];
	int nn;

	Buff.Format(_T("%s%d..%d"),VariableName,nStartOffset,nFinishOffset);
	SendStr(Buff);

	Result[0]=0;
	if(m_bDriverOpen) {
		while(DeviceReadReady(0)!=TRUE) {};

		while(DeviceGetLine(m_dwDevice,(WCHAR*)str,255) > 0) {
		  RxBuff += str;
		}
		for(nn=0;nn<RxBuff.GetLength();nn++) {
			Result[nn]=(char)RxBuff.GetAt(nn);
		}
		Result[nn]=0;
	}
	return strlen(Result);

}
//Unicode
int CPmacPcomm32::RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff)
{
	CString	Buff;
	char str[256];

	Buff.Format(_T("%s%d..%d"),VariableName,nStartOffset,nFinishOffset);
	SendStr(Buff);
	RxBuff.Empty();
	while(DeviceGetLine(m_dwDevice,(WCHAR*)str,255) > 0) {
	  RxBuff += str;
	}
	return RxBuff.GetLength();

}

//Unicode
int CPmacPcomm32::RequestMVariableDefinitions(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff)
{
	CString	Buff;

	Buff.Format(_T("%s%d..%d->"),VariableName,nStartOffset,nFinishOffset);
	SendStr(Buff);
	if(m_bDriverOpen) {
		while(DeviceReadReady(0)!=TRUE) {};
		GetStr(RxBuff);
	} else {
		RxBuff.Format(_T("No PMAC"));
	}
	return RxBuff.GetLength();
}

//Unicode

bool CPmacPcomm32::DownLoad(CString FilePath)
{
	int	ii,nn;
	char cFileName[256];
	for(ii=0;ii<FilePath.GetLength();ii++) {
		cFileName[ii] = (char)FilePath.GetAt(ii);
	}
	cFileName[ii]=0;

	if((nn=DeviceDownload(m_dwDevice,NULL,NULL,NULL,cFileName,NULL,NULL,NULL,TRUE))==0) {
		MessageBox(NULL,_T("Failed to Download file ") + FilePath,_T("Error"),MB_ICONERROR);
		return FALSE;
	} else {
		MessageBox(NULL,_T("File successfully downloaded ") + FilePath,_T("Message"),MB_OK);
		return TRUE;
	}
}

int CPmacPcomm32::ReadDpram(int nOffset)
{
	int nValue = 0;
	if(m_bDriverOpen) {
		nValue = DeviceDPRGetDWord(m_dwDevice, (UINT) nOffset * 4);
	}

	return nValue;
}



void CPmacPcomm32::WriteDpram(int nOffset, int nValue)
{

	if(m_bDriverOpen) {
		DeviceDPRSetDWord(m_dwDevice, (UINT) nOffset * 4, nValue);
	}

}

int CPmacPcomm32::DpramGetMem(int nOffset,int nLength,PVOID pMem)
{

	if(m_bDriverOpen) {
		DeviceDPRGetMem(m_dwDevice, (UINT) nOffset * 4,nLength * 4,pMem);
	}


	return 1;
}

int CPmacPcomm32::DpramSetMem(int nOffset,int nLength,PVOID pMem, bool bDoMemCheck)
{
	int nReadArray[1024];
	int ii;
	int *pWriteArray = (int*)pMem;

	if(m_bDriverOpen) {
again:
		DeviceDPRSetMem(m_dwDevice, (UINT) nOffset * 4,nLength * 4,pMem);

		if(bDoMemCheck == TRUE) {
			DeviceDPRGetMem(m_dwDevice, (UINT) nOffset * 4,nLength * 4,nReadArray);

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

void CPmacPcomm32::EnableInterrupt()
{
	if(m_bDriverOpen){


		SendStr(L"DISABLE PLCC 0..31");
		SendStr(L"DISABLE PLC 0..31");
		SendStr(L"I5=0");
		
		if(theApp.m_LastSettings.bPMACUsesHardwareIRQ)
			long nn=DeviceINTRFuncCallInit(m_dwDevice,InterruptServiceRoutine, 0xabcd, 0xffffffbf );	//enable IR6

		SendStr(L"ENABLE PLCC 1");
		SendStr(L"ENABLE PLCC 0");
		SendStr(L"I5=3");
		SendStr(L"I8=0");
		
		if(!theApp.m_LastSettings.bPMACUsesHardwareIRQ)
			StartThread();

	}
}

void CPmacPcomm32::StartThread()
{
	if(m_bThreadEnabled==FALSE) {
		m_bThreadEnabled = TRUE;
//		m_pThread = AfxBeginThread(&PmacThread,this,THREAD_PRIORITY_TIME_CRITICAL,0,NULL);
		m_pThread = AfxBeginThread(&PmacThread,this,THREAD_PRIORITY_NORMAL,0,NULL);
	};
}
void CPmacPcomm32::StopThread()
{
	if(m_bThreadEnabled==TRUE) {
		m_bThreadEnabled = FALSE;
		WaitForSingleObject(&m_pThread->m_hThread,INFINITE);
	};
}


UINT PmacThread(LPVOID pParam)
{
	CPmacPcomm32* pThread = (CPmacPcomm32 *)pParam;
	static int loop=0;
	CHRTimer HRTimer;
	

	while(pThread->m_bThreadEnabled==TRUE) {
		HRTimer.StartTimer();
		while(HRTimer.StopTimer() < 0.5e-3) {};
		InterruptServiceRoutine(0,NULL);
	};
	pThread->m_bThreadFinished=TRUE;
	return 0;
}


void __stdcall InterruptServiceRoutine(DWORD dwUser,PINTRBUFFER pBuff)
{
	int	nAxis;
	int nOldPos,nPos;
	static	bool bFlag=FALSE;
	static int nStatusLoop=0;
	int	nRollOverMode = 1;

	theApp.m_PmacUser.nIrqCount++;
//	theApp.m_Si10User.TestPatternOutput(1);

	theApp.m_PmacUser.DpramGetMem(0,12,(PVOID)theApp.m_PmacUser.m_nPosition);
	if(nStatusLoop>100) {
		theApp.m_PmacUser.DpramGetMem(PMAC_MOTOR_STATUS,12,(PVOID)theApp.m_PmacUser.m_nStatus);
		theApp.m_PmacUser.m_nGeneralInput = theApp.m_PmacUser.ReadDpram(PMAC_GENERAL_INPUTS);
		theApp.m_PmacUser.m_nGeneralOutput= theApp.m_PmacUser.ReadDpram(PMAC_GENERAL_OUTPUTS);

		if((theApp.m_Thread.m_nThreadAction == JOYSTICK_START) && (theApp.m_Thread.m_nSubThreadAction == 10)) {
			theApp.m_PmacUser.PLCJoystickServiceRoutine(FALSE);
		}

		nStatusLoop = 0;
	}
	if(theApp.m_bCollectTunning==TRUE) {
		theApp.m_PmacUser.DpramGetMem(PMAC_FOLLOWING_ERROR,12,(PVOID)theApp.m_PmacUser.m_nFollowingError);
	}

	for(nAxis=0;nAxis<32;nAxis++) {
		if((theApp.m_Axes[nAxis].nMotorType == SERVO) && (theApp.m_Axes[nAxis].bAvailable==TRUE)) {
			
			nPos = theApp.m_PmacUser.m_nPosition[theApp.m_Axes[nAxis].nPhysicalAxis-1]-theApp.m_Axes[nAxis].nOffset;

			if(theApp.m_Axes[nAxis].nStepsPerRev==0) {
				theApp.m_Axes[nAxis].nPos = nPos;
			} else {

				if(nRollOverMode == 0) {
					nOldPos=theApp.m_Axes[nAxis].nPos;
					
					theApp.m_Axes[nAxis].nRevCounter = nPos / theApp.m_Axes[nAxis].nStepsPerRev;

					if(bFlag==FALSE) theApp.m_PmacUser.m_nLastPos[nAxis]=nPos;
					nPos=theApp.m_Axes[nAxis].nPos + (nPos-theApp.m_PmacUser.m_nLastPos[nAxis]);
					nPos%=theApp.m_Axes[nAxis].nStepsPerRev;
					if(nPos<0) nPos+=theApp.m_Axes[nAxis].nStepsPerRev;
					theApp.m_Axes[nAxis].nPos = nPos;
					theApp.m_PmacUser.m_nLastPos[nAxis]=nPos;
					if(abs(nOldPos - nPos)>1000) {
						if(nPos>nOldPos) {
							theApp.m_Motors.m_nRevCounter--;
						}else{
							theApp.m_Motors.m_nRevCounter++;
						}
					}
				} else {
					theApp.m_Axes[nAxis].nRevCounter = nPos / theApp.m_Axes[nAxis].nStepsPerRev;
					nPos%=theApp.m_Axes[nAxis].nStepsPerRev;
					if(nPos<0) nPos+=theApp.m_Axes[nAxis].nStepsPerRev;
					nOldPos=theApp.m_Axes[nAxis].nPos;
					theApp.m_Axes[nAxis].nPos = nPos;
					if(abs(nPos - nOldPos) > (theApp.m_Axes[nAxis].nStepsPerRev / 2)) {
						if(nPos>nOldPos) {
							theApp.m_Motors.m_nRevCounter--;
						}else{
							theApp.m_Motors.m_nRevCounter++;
						}
					}
				}
			};
			if(nStatusLoop==0) theApp.m_Axes[nAxis].nStatus = theApp.m_PmacUser.m_nStatus[theApp.m_Axes[nAxis].nPhysicalAxis-1];
			theApp.m_Axes[nAxis].nCurrentSpeed = theApp.m_PmacUser.m_nFollowingError[theApp.m_Axes[nAxis].nPhysicalAxis-1];

		}
	};
	bFlag = TRUE;
	nStatusLoop++;


	if(theApp.m_bCollectComplexData==TRUE) {
		theApp.m_pScanData->CollectComplexData();
	}
	if(theApp.m_bCollectNormalizeData==TRUE) {
		theApp.m_Thread.CollectNormalizeData();
	}
	if(theApp.m_bCollectTunning==TRUE) {
		theApp.m_Thread.CollectTunning();
	}
	/*
	if(theApp.m_ServoUser.theApp.m_Motors.m_bCollectData==TRUE) {
		theApp.m_ServoUser.CollectGateData();
	};
	if(theApp.m_ServoUser.theApp.m_Motors.m_bCollectPeakData==TRUE) {
		theApp.m_ServoUser.CollectPeakGateData();
	};
	if((theApp.m_Axes[theApp.m_Tank.nZLeft].nStatus & PMAC_MAX_SWITCH) && (theApp.m_LastSettings.m_bProtectionRingEnabled==TRUE)) {
		theApp.m_bProtectionRingTripped = TRUE;
	}
	*/
//	theApp.m_Si10User.TestPatternOutput(0);

}


void CPmacPcomm32::DisableInterrupt()
{
	if(m_bDriverOpen){
		
		if(theApp.m_LastSettings.bPMACUsesHardwareIRQ)
			BOOL result = DeviceINTRTerminate(m_dwDevice);

		SendStr(L"DISABLE PLCC 0..31");
		SendStr(L"DISABLE PLC 0..31");
		SendStr(L"I5=0");

		if(!theApp.m_LastSettings.bPMACUsesHardwareIRQ)
			StopThread();
	}
}


void CPmacPcomm32::Save(CUSLFile *pFile)
{
	CString Buff;
	char str[4096];

	SendStr(L"I5=0");
	SendStr(L"I9=3");

	if(theApp.m_LastSettings.nCheckPmacIVariables==TRUE) {

		switch(m_nPmacType) {
		case PMAC1:
		case PMAC2:
		case PMAC_Ultralite:
			Buff.Format(_T("I%d..%d"),0,999);
			SendStr(Buff);
			while(DeviceGetLine(m_dwDevice,(WCHAR*)str,255) > 0) {
				strcat_s(str,"\n");
				pFile->Write(str,strlen(str));
			}
			break;
		case Turbo_PMAC1:
		case Turbo_PMAC2:
			Buff.Format(_T("I%d..%d"),0,1599);
			SendStr(Buff);
			while(DeviceGetLine(m_dwDevice,(WCHAR*)str,255) > 0) {
				strcat_s(str,"\n");
				pFile->Write(str,strlen(str));
			}
			Buff.Format(_T("I%d..%d"),7000,7499);
			SendStr(Buff);
			while(DeviceGetLine(m_dwDevice,(WCHAR*)str,255) > 0) {
				strcat_s(str,"\n");
				pFile->Write(str,strlen(str));
			}
			break;
		case Turbo_PMAC_Ultralite:
		case Turbo_PMAC2_Ultralite:
			Buff.Format(_T("I%d..%d"),0,1599);
			SendStr(Buff);
			while(DeviceGetLine(m_dwDevice,(WCHAR*)str,255) > 0) {
				strcat_s(str,"\n");
				pFile->Write(str,strlen(str));
			}
			Buff.Format(_T("I%d..%d"),6000,7499);
			SendStr(Buff);
			while(DeviceGetLine(m_dwDevice,(WCHAR*)str,255) > 0) {
				strcat_s(str,"\n");
				pFile->Write(str,strlen(str));
			}
			break;
		}

	}
	if(theApp.m_LastSettings.nCheckPmacMVariables==TRUE) {
		SendStr(L"M0..1023->");
		while(DeviceGetLine(m_dwDevice,(WCHAR*)str,255) > 0) {
			strcat_s(str,"\n");
			pFile->Write(str,strlen(str));
		}
	}
	
	SendStr(_T("I5=3"));
	SendStr(_T("I9=2"));

}

void CPmacPcomm32::Retrieve(CString FilePath)
{
	CString Buff;

	SendStr(L"I5=0");
	SendStr(L"I9=3");

	DownLoad(FilePath);

	SendStr(L"I5=3");
	SendStr(L"I9=2");

}

