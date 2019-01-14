// PmacPcommServer.cpp: implementation of the CPmacPcommServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "PmacPcommServer.h"
#include "CPmacDevice.h"
#include "CPmacNC.h"
#include "Delta Tau\PcommServer\Include\PmacServer.h"    // Accsee from Additional directory include of Project setting.
#include "PmacNCEvents.h"
#include "PmacDeviceEvents.h"
#include "afxctl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPmacDevice			m_pmacDevice;
CPmacNC				m_pmacNC;
CPmacNCEvents		*m_NCEvents;
CPmacDeviceEvents	*m_PmacDeviceEvents;

CString				m_RxBuff;
BOOL				m_bDriverOpen;
long				m_dwDevice;
BOOL				m_bInterrupt;
bool				m_bCoIinitialized;

CProgressCtrl m_ProgressBar;
CString *MyString;
HWND MainhWnd;


CPmacPcommServer::CPmacPcommServer()
{
	m_bDriverOpen = FALSE;
	m_bThreadEnabled = FALSE;
	m_bThreadFinished = FALSE;
	m_bCoIinitialized = FALSE;

}

CPmacPcommServer::~CPmacPcommServer()
{

	SuspendDriver();
	if(m_bCoIinitialized==true) {
//		CoUninitialize();
		m_bCoIinitialized = false;
	}

}

bool CPmacPcommServer::SuspendDriver()
{
	int retval;

	if(m_bDriverOpen) {
		DisableInterrupt();
		m_pmacDevice.Close(m_dwDevice);
		m_pUnkSink = m_NCEvents->GetIDispatch(FALSE);
		retval = AfxConnectionUnadvise(m_pDisp, DIID__IPmacNCEvents, m_pUnkSink, FALSE,m_dwCookie);
		m_pUnkSink1 = m_PmacDeviceEvents->GetIDispatch(FALSE);
		retval = AfxConnectionUnadvise(m_pDisp1, DIID__IPmacDeviceEvents, m_pUnkSink1, FALSE,m_dwCookie1);
		m_bDriverOpen = FALSE;
		SAFE_DELETE( m_PmacDeviceEvents );
		SAFE_DELETE( m_NCEvents );
		SAFE_DELETE( MyString );

	}
	m_bDriverOpen = FALSE;
	m_bThreadEnabled = FALSE;
	m_bThreadFinished = FALSE;

	return true;
}

bool CPmacPcommServer::StartDriver(CString &strPmacType,int *nPmacType)
{
	CString Buff;
	BSTR Date	= ::SysAllocString(L"");
	BSTR Version = ::SysAllocString(L"");
	int retVal;
	long test,dwDevice;
	BOOL pbSuccess; 


	if(m_bCoIinitialized!=true) {
		CoInitialize(NULL);
		m_bCoIinitialized=true;

		test = m_pmacDevice.CreateDispatch(_T("PcommServer.PmacDevice.1"));
	    if (!test) {
			AfxMessageBox(_T("Can Not Connect to PcommServer Interface "));
			m_pmacDevice.SelectDevice(NULL,&dwDevice,&pbSuccess);
		}
		test = m_pmacNC.CreateDispatch(_T("PcommServer.PmacNC.1"));
	}

    m_NCEvents = new CPmacNCEvents();
    m_PmacDeviceEvents = new CPmacDeviceEvents();  
	m_pUnkSink = m_NCEvents->GetIDispatch(FALSE);
	m_pDisp = m_pmacNC.m_lpDispatch;
	// Connect NC events.
	retVal = AfxConnectionAdvise(m_pDisp,DIID__IPmacNCEvents,m_pUnkSink,FALSE,&m_dwCookie);
    m_pUnkSink1 = m_PmacDeviceEvents->GetIDispatch(FALSE);
	m_pDisp1 = m_pmacDevice.m_lpDispatch;
	//Connect PMAC Device events. 
	retVal = AfxConnectionAdvise(m_pDisp1,DIID__IPmacDeviceEvents,m_pUnkSink1,FALSE,&m_dwCookie1);
	MyString = new CString();
//	MainhWnd = this->m_hWnd;

	m_dwDevice=0;
	m_pmacDevice.Open(m_dwDevice,&m_bDriverOpen);


	if((m_dwDevice == NO_PMAC_DEVICE) || (m_bDriverOpen == false)) {
		Buff.LoadString(IDS_No_PMAC_Not_Init);
		AfxMessageBox(Buff);
		m_pmacDevice.SelectDevice(NULL,&dwDevice,&pbSuccess);
		return FALSE;
	}

      
	if(m_bDriverOpen) {
		m_pmacDevice.GetRomVersion(m_dwDevice,&Version);
		m_pmacDevice.GetRomDate(m_dwDevice,&Date);
		m_pmacDevice.GetPmacType(m_dwDevice,&m_nPmacType);
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
		return TRUE;
	}
	return FALSE;


}

int CPmacPcommServer::SendStr(CString TxBuff,bool bWaitForResponce)
{
	long nStatus;
	BSTR m_RxString = ::SysAllocString(L"");;

	if(m_bDriverOpen) {

		m_pmacDevice.GetResponseEx(m_dwDevice,TxBuff,0,&m_RxString,&nStatus);
		if(::SysStringLen(m_RxString)) {
			m_RxBuff = m_RxString;
		}

	}
 	
	return 0;
}

BOOL CPmacPcommServer::GetStr(CString &RxBuff)
{

	if(m_bDriverOpen) {
		if(m_RxBuff.GetLength()) {
			RxBuff = m_RxBuff;
			m_RxBuff.Empty();
			return TRUE;
		}
	}
	return FALSE;
	
}

int CPmacPcommServer::RequestResponce(CString TxBuff, CString *pRxBuff)
{

	return 0;
/*
	SendStr(TxBuff);
	pRxBuff->Empty();
	if(m_bDriverOpen) {
		while(DeviceReadReady(0)!=TRUE) {};
		GetStr(*pRxBuff);
	} else {
		pRxBuff->Format(_T("No PMAC"));
	}
	return pRxBuff->GetLength();
*/
}


int CPmacPcommServer::RequestVariable(CString VariableName, int nOffset, CString &RxBuff)
{
	CString Buff;

	Buff.Format(_T("%s%d"),VariableName,nOffset);
	SendStr(Buff);
	GetStr(RxBuff);

	return RxBuff.GetLength();

}

void CPmacPcommServer::SetVariable(CString VariableName, int nOffset, CString Value)
{
	return;
	CString Buff;
	CString RxBuff;

	Buff.Format(_T("%s%d=%s"),VariableName,nOffset,Value);
	SendStr(Buff);
}

void CPmacPcommServer::SetVariable(CString VariableName, int nOffset, float fValue)
{
	return;
	CString Buff;
	CString RxBuff;

	Buff.Format(_T("%s%d=%f"),VariableName,nOffset,fValue);
	SendStr(Buff);
}

void CPmacPcommServer::SetVariable(CString VariableName, int nOffset, int nValue)
{
	CString Buff;
	CString RxBuff;

	Buff.Format(_T("%s%d=%d"),VariableName,nOffset,nValue);
	SendStr(Buff);
}

//Unicode
int CPmacPcommServer::RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff)
{
	CString	Buff;

	Buff.Format(_T("%s%d..%d"),VariableName,nStartOffset,nFinishOffset);
	SendStr(Buff);
	GetStr(RxBuff);
	return RxBuff.GetLength();

}

//Unicode
int CPmacPcommServer::RequestMVariableDefinitions(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff)
{
	CString	Buff;

	Buff.Format(_T("%s%d..%d->"),VariableName,nStartOffset,nFinishOffset);
	SendStr(Buff);
	GetStr(RxBuff);
	return RxBuff.GetLength();

}

//Unicode

void CPmacPcommServer::EnableInterrupt()
{
	if(m_bDriverOpen){


		SendStr(L"DISABLE PLCC 0..31");
		SendStr(L"DISABLE PLC 0..31");
		SendStr(L"I5=0");
//		nn=DeviceINTRFuncCallInit(m_dwDevice,InterruptServiceRoutine, 0xabcd, 0xffffffbf );	//enable IR6

		SendStr(L"ENABLE PLCC 1");
		SendStr(L"ENABLE PLCC 0");
		SendStr(L"I5=3");
		SendStr(L"I8=0");

		m_pmacDevice.put_InterruptMask(m_dwDevice,0xbf); // Mask all interrupts
		m_pmacDevice.put_InterruptsEnabled(m_dwDevice,true); // Enable Interrupts
	}
}



void CPmacPcommServer::InterruptServiceRoutine()
{
	int	nAxis;
	int nOldPos,nPos;
	static	bool bFlag=FALSE;
	static int nStatusLoop=0;
	int	nRollOverMode = 1;

//	theApp.m_Si10User.TestPatternOutput(1);
	theApp.m_PmacUser.nIrqCount++;
	DpramGetMem(0,12,(PVOID)theApp.m_PmacUser.m_nPosition);
	if(nStatusLoop>100) {
		DpramGetMem(PMAC_MOTOR_STATUS,12,(PVOID)theApp.m_PmacUser.m_nStatus);
		nStatusLoop = 0;
	}
	if(theApp.m_bCollectTunning==TRUE) {
		DpramGetMem(PMAC_FOLLOWING_ERROR,12,(PVOID)theApp.m_PmacUser.m_nFollowingError);
	}

	for(nAxis=0;nAxis<32;nAxis++) {
		if((theApp.m_Axes[nAxis].nMotorType == SERVO) && (theApp.m_Axes[nAxis].bAvailable==TRUE)) {
			
			nPos = theApp.m_PmacUser.m_nPosition[theApp.m_Axes[nAxis].nPhysicalAxis-1]-theApp.m_Axes[nAxis].nOffset;

			if(theApp.m_Axes[nAxis].dStepsPerRev==0.0f) {
				theApp.m_Axes[nAxis].nPos = nPos;
			} else {

				if(nRollOverMode == 0) {
					nOldPos=theApp.m_Axes[nAxis].nPos;
					
					theApp.m_Axes[nAxis].nRevCounter = nPos / (int)theApp.m_Axes[nAxis].dStepsPerRev;

					if(bFlag==FALSE) theApp.m_PmacUser.m_nLastPos[nAxis]=nPos;
					nPos=theApp.m_Axes[nAxis].nPos + (nPos-theApp.m_PmacUser.m_nLastPos[nAxis]);
					nPos%=(int)theApp.m_Axes[nAxis].dStepsPerRev;
					if(nPos<0) nPos+=(int)theApp.m_Axes[nAxis].dStepsPerRev;
					theApp.m_Axes[nAxis].nPos = nPos;
					theApp.m_PmacUser.m_nLastPos[nAxis]=nPos;
					if(abs(nOldPos - nPos)>1000) theApp.m_Motors.m_nRevCounter++;
				} else {
					theApp.m_Axes[nAxis].nRevCounter = nPos / (int)theApp.m_Axes[nAxis].dStepsPerRev;
					nPos%=(int)theApp.m_Axes[nAxis].dStepsPerRev;
					nOldPos=theApp.m_Axes[nAxis].nPos;
					theApp.m_Axes[nAxis].nPos = nPos;
					if(abs(nPos - nOldPos) > (int)(theApp.m_Axes[nAxis].dStepsPerRev / 2.0f)) {
						theApp.m_Motors.m_nRevCounter++;
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


void CPmacPcommServer::DisableInterrupt()
{
	if(m_bDriverOpen){
		m_pmacDevice.put_InterruptMask(m_dwDevice,0xff); // Mask all interrupts
		m_pmacDevice.put_InterruptsEnabled(m_dwDevice,false); // Disable Interrupts
	}
}

bool CPmacPcommServer::DownLoad(CString FilePath)
{
	int	nn=0;
	int bSuccess;


	m_pmacDevice.Download(m_dwDevice,FilePath,false,false,true,true,&bSuccess);
	if(bSuccess != 65535) {
		MessageBox(NULL,_T("Failed to Download file ") + FilePath,_T("Error"),MB_ICONERROR);
		return false;
	} else {
		MessageBox(NULL,_T("File successfully downloaded ") + FilePath,_T("Message"),MB_OK);
		return true;
	}


	return nn  & 0x01;
}

int CPmacPcommServer::ReadDpram(int nOffset)
{
	long nValue = 0;
	int	nSuccess;

	if(m_bDriverOpen) {
		m_pmacDevice.DPRGetLong(m_dwDevice, (UINT) nOffset * 4,&nValue, &nSuccess);
	}

	return nValue;
}

void CPmacPcommServer::WriteDpram(int nOffset, int nValue)
{
	int	nSuccess;

	if(m_bDriverOpen) {
		m_pmacDevice.DPRSetLong(m_dwDevice, (UINT) nOffset * 4, nValue, &nSuccess);
	}

}

int CPmacPcommServer::DpramGetMem(int nOffset,int nLength,PVOID pMem)
{
	if(m_bDriverOpen) {

		char            *ptr;
		VARIANT         pVarray;
		BOOL			bSuccess;
		long            lcount;
		long			count = nLength * 4;

		VariantInit(&pVarray);

		m_pmacDevice.DPRGetMem(m_dwDevice, (UINT) nOffset * 4,count, &pVarray,&bSuccess);
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = count;
		if(SafeArrayGetDim(pVarray.parray) == 1) {
			SafeArrayGetUBound(pVarray.parray, 1, &lcount);
			if(lcount == (count-1))
			{
				SafeArrayAccessData(pVarray.parray, (void HUGEP**)&ptr);
				memcpy(pMem,ptr,count) ;
				SafeArrayUnaccessData(pVarray.parray);
			}
		}
		SafeArrayDestroy(pVarray.parray);
		VariantClear(&pVarray);

		return bSuccess;
			
	}

	return 0;
}

int CPmacPcommServer::DpramSetMem(int nOffset,int nLength,PVOID pMem)
{

	if(m_bDriverOpen) {



		SAFEARRAY       *psa;
		char            *ptr1;
		VARIANT         pVarray;
		BOOL			bSuccess;
		long			count = nLength * 4;
		bSuccess = 0;

		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = count;
		psa = SafeArrayCreate(VT_UI1,1,rgsabound);
		ptr1 =(char *)memcpy(psa->pvData,pMem,count);
		if (ptr1) {
			VariantInit (&pVarray);
			pVarray.vt = (VT_ARRAY | VT_UI1);
			pVarray.parray = psa;
				m_pmacDevice.DPRSetMem(m_dwDevice, nOffset * 4, count, pVarray,&bSuccess);
			VariantClear(&pVarray);
		}
		return bSuccess;
	}
	return 0;
}


void CPmacPcommServer::Save(CUSLFile *pFile)
{
	CString Buff,RxBuff;

	GetStr(RxBuff);		//called just to ensure buff is empty;

	SendStr(L"I5=0");
	SendStr(L"I9=3");

	if(theApp.m_LastSettings.nCheckPmacIVariables==TRUE) {

		switch(m_nPmacType) {
		case PMAC1:
		case PMAC2:
		case PMAC_Ultralite:
			Buff.Format(_T("I%d..%d"),0,999);
			SendStr(Buff);
			GetStr(RxBuff);
			pFile->WriteUnicodeStringAsAscii(&RxBuff);
			break;
		case Turbo_PMAC1:
		case Turbo_PMAC2:
		case Turbo_PMAC_Ultralite:
		case Turbo_PMAC2_Ultralite:
			Buff.Format(_T("I%d..%d"),0,1599);
			SendStr(Buff);
			GetStr(RxBuff);
			pFile->WriteUnicodeStringAsAscii(&RxBuff);
			Buff.Format(_T("I%d..%d"),6000,7499);
			SendStr(Buff);
			GetStr(RxBuff);
			pFile->WriteUnicodeStringAsAscii(&RxBuff);
			break;
		}

	}
	if(theApp.m_LastSettings.nCheckPmacMVariables==TRUE) {
		SendStr(L"M0..1200->");
		GetStr(RxBuff);
		pFile->WriteUnicodeStringAsAscii(&RxBuff);
	}
	

	for(int i=0;i<32;i++) {
		if(theApp.m_LastSettings.nCheckPmacPrograms) {
			Buff.Format(_T("list prog %d"),i);
			SendStr(Buff);
			GetStr(RxBuff);
			if(RxBuff.GetLength()>10){ //10 to acount for error msg
				Buff.Format(_T("\r\n\r\nOPEN PROG %d CLEAR\r\n"),i);
				pFile->WriteUnicodeStringAsAscii(&Buff);
				pFile->WriteUnicodeStringAsAscii(&RxBuff);
				Buff.Format(_T("CLOSE"));
				pFile->WriteUnicodeStringAsAscii(&Buff);
			}
		}
	
		if(theApp.m_LastSettings.nCheckPmacPLCs) {
			Buff.Format(_T("list plc %d"),i);
			SendStr(Buff);
			GetStr(RxBuff);
			if(RxBuff.GetLength()>10){ //10 to acount for error msg
				Buff.Format(_T("\n\nOPEN PLC %d CLEAR\n"),i);
				pFile->WriteUnicodeStringAsAscii(&Buff);
				pFile->WriteUnicodeStringAsAscii(&RxBuff);
				Buff.Format(_T("CLOSE"));
				pFile->WriteUnicodeStringAsAscii(&Buff);
			}
		}
	}


	SendStr(_T("I5=3"));
	SendStr(_T("I9=2"));

}
