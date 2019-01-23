// Pmac.cpp: implementation of the CPmac class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Pmac.h"
#include "runtime.h"
#include "PromptDlg.h"


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

inline void MulDiv(int *pV0,int nV1,int nV2)
{
	*pV0 = (*pV0 * nV1) / nV2;
}

CPmac::CPmac()
{
	m_bThreadRun = FALSE;
	m_nThreadAction = 0;
	m_nSubThreadAction = 0;
	nIrqCount=0;
	m_bLookAheadFlag=FALSE;
	m_nScanSide=2;
	ZeroMemory(m_nPumpPurgeStartTime,sizeof m_nPumpPurgeStartTime);
	m_bWriteReentrant = false;

	ZeroMemory(m_nDPRAMWriteAddrs,sizeof m_nDPRAMWriteAddrs);
	ZeroMemory(m_nDPRAMWriteValue,sizeof m_nDPRAMWriteValue);
	ZeroMemory(m_bLS_Disabled_Reenable_On_Exit, sizeof m_bLS_Disabled_Reenable_On_Exit);
	m_nDPRAMWritePtr = 0;
	m_nDPRAMWriteCurrent = 0;
	m_nDPRAMError = 0;
	m_fMaxStopTime = 0.0f;
	m_nLastGeneralInput = 0;
	m_bRetrieveVirtualAxes = false;
	m_bTakeShortestTurntableRoute = false;


	m_bDriverOpen = FALSE;
	m_nWhichDLL = COMMS32;
	m_nWhichDLL = COMMSSERVER;
	m_nWhichDLL = PMACCOMMSUSL;
//	m_nWhichDLL = PMACISAUSL;

}

CPmac::~CPmac()
{

}

bool CPmac::StartDriver()
{
	switch(m_nWhichDLL) {
	case COMMS32: m_bDriverOpen = m_Comms32.StartDriver(m_strPmacType,&m_nPmacType);
		break;
	case COMMSSERVER: m_bDriverOpen = m_CommsServer.StartDriver(m_strPmacType,&m_nPmacType);
		break;
	case PMACCOMMSUSL: m_bDriverOpen = m_CommsUSL.StartDriver(m_strPmacType,&m_nPmacType);
		break;
	case PMACISAUSL: m_bDriverOpen = m_ServoUser.StartDriver(m_strPmacType,&m_nPmacType);
		EnableInterrupt();
		break;
	}

	if(!m_bDriverOpen)
		return m_bDriverOpen;

	SetGeneralInputXor();
	SetPLCInterlocks();
	SetFeedRate(theApp.m_LastSettings.fFeedRate);

	return m_bDriverOpen;
}

bool CPmac::SuspendDriver()
{
	
	if(m_bDriverOpen==false)
		return false;


	switch(m_nWhichDLL) {
	case COMMS32:
		break;
	case COMMSSERVER: m_CommsServer.SuspendDriver();
		break;
	case PMACCOMMSUSL: m_CommsUSL.SuspendDriver();
		break;
	}
	return true;


}


int CPmac::SendStr(CString TxBuff,bool bWaitForResponce)
{
	
	if(m_bDriverOpen==false)
		return -1;

	switch(m_nWhichDLL) {
	case COMMS32: return m_Comms32.SendStr(TxBuff,bWaitForResponce);
		break;
	case COMMSSERVER: return m_CommsServer.SendStr(TxBuff,bWaitForResponce);
		break;
	case PMACCOMMSUSL: return m_CommsUSL.SendStr(TxBuff,bWaitForResponce);
		break;
	case PMACISAUSL: return m_ServoUser.CommsStr(TxBuff,bWaitForResponce);
		break;
	}
	return 0;
}

BOOL CPmac::GetStr(CString &RxBuff)
{
	
	if(m_bDriverOpen==false)
		return false;
	
	switch(m_nWhichDLL) {
	case COMMS32: return m_Comms32.GetStr(RxBuff);
		break;
	case COMMSSERVER: return m_CommsServer.GetStr(RxBuff);
		break;
	case PMACCOMMSUSL: return m_CommsUSL.GetStr(RxBuff);
		break;
	case PMACISAUSL: return	m_ServoUser.CommsGetStr(RxBuff);
		break;

	}
	return FALSE;
}

BOOL CPmac::RequestVariable(CString VariableName, int nOffset, CString *pRxBuff)
{
	
	if(m_bDriverOpen==false)
		return -1;

	switch(m_nWhichDLL) {
	case COMMS32: return m_Comms32.RequestVariable(VariableName,nOffset,pRxBuff);
		break;
	case COMMSSERVER: return m_CommsServer.RequestVariable(VariableName,nOffset,*pRxBuff);
		break;
	case PMACCOMMSUSL: return m_CommsUSL.RequestVariable(VariableName,nOffset,*pRxBuff);
		break;
	case PMACISAUSL: return m_ServoUser.RequestVariable(VariableName, nOffset,*pRxBuff);
		break;
	}
	return 0;
}

BOOL CPmac::RequestSlaveVariable(CString VariableName, int nNode, int nOffset, CString *pRxBuff)
{

	if(m_bDriverOpen==false)
		return -1;

	switch(m_nWhichDLL) {
	case COMMS32: return -1;
		break;
	case COMMSSERVER: return -1;
		break;
	case PMACCOMMSUSL: return m_CommsUSL.RequestSlaveVariable(VariableName,nNode,nOffset,*pRxBuff);
		break;
	case PMACISAUSL: return -1;
		break;
	}
	return 0;
}


void CPmac::SetVariable(CString VariableName, int nOffset, CString Value)
{
	CString Buff;
	CString RxBuff;

	Buff.Format(_T("%s%d=%s"),VariableName,nOffset,Value);
	SendStr(Buff);
}

void CPmac::SetVariable(CString VariableName, int nOffset, float fValue)
{
	CString Buff;
	CString RxBuff;

	Buff.Format(_T("%s%d=%f"),VariableName,nOffset,fValue);
	SendStr(Buff);
}

void CPmac::SetVariable(CString VariableName, int nOffset, int nValue)
{
	CString Buff;
	CString RxBuff;

	Buff.Format(_T("%s%d=%d"),VariableName,nOffset,nValue);
	SendStr(Buff);
}

int CPmac::RequestResponce(CString TxBuff, CString *pRxBuff)
{
	switch(m_nWhichDLL) {
	case COMMS32: return m_Comms32.RequestResponce(TxBuff, pRxBuff);
		break;
	case COMMSSERVER:
		break;
	}
	return 0;
}

int CPmac::RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, char *Result)
{


	switch(m_nWhichDLL) {
	case COMMS32: return m_Comms32.RequestMultipleVariables(VariableName, nStartOffset,nFinishOffset, Result);
		break;
	case COMMSSERVER:
		break;
	}
	return 0;
}

int CPmac::RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff)
{


	switch(m_nWhichDLL) {
	case COMMS32: return m_Comms32.RequestMultipleVariables(VariableName, nStartOffset,nFinishOffset, RxBuff);
		break;
	case COMMSSERVER: return m_CommsServer.RequestMultipleVariables(VariableName, nStartOffset,nFinishOffset, RxBuff);
		break;
	case PMACCOMMSUSL: return m_CommsUSL.RequestMultipleVariables(VariableName, nStartOffset,nFinishOffset, RxBuff);
		break;
	case PMACISAUSL: return m_ServoUser.RequestMultipleVariables(VariableName, nStartOffset,nFinishOffset, RxBuff);
		break;
	}
	return 0;
}

//Unicode
int CPmac::RequestMVariableDefinitions(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff)
{


	switch(m_nWhichDLL) {
	case COMMS32: return m_Comms32.RequestMVariableDefinitions(VariableName, nStartOffset,nFinishOffset, RxBuff);
		break;
	case COMMSSERVER: return m_CommsServer.RequestMVariableDefinitions(VariableName, nStartOffset,nFinishOffset, RxBuff);
		break;
	case PMACCOMMSUSL: return m_CommsUSL.RequestMVariableDefinitions(VariableName, nStartOffset,nFinishOffset, RxBuff);
		break;
	case PMACISAUSL: return m_ServoUser.RequestMVariableDefinitions(VariableName, nStartOffset,nFinishOffset, RxBuff);
		break;
	}
	return 0;
}

void CPmac::EnableInterrupt()
{
	switch(m_nWhichDLL) {
	case COMMS32: m_Comms32.EnableInterrupt();
		break;
	case COMMSSERVER:
		break;
	}

}

void CPmac::DisableInterrupt()
{
	switch(m_nWhichDLL) {
	case COMMS32: m_Comms32.DisableInterrupt();
		break;
	case COMMSSERVER:
		break;
	}

}

bool CPmac::DownLoad(CString FilePath)
{

	switch(m_nWhichDLL) {
	case COMMS32: return m_Comms32.DownLoad(FilePath);
		break;
	case COMMSSERVER: return m_CommsServer.DownLoad(FilePath);
		break;
	case PMACISAUSL: return m_ServoUser.DownloadProgram(FilePath);
		break;
	}
	return FALSE;
}

void CPmac::ZBrakesEnable(int bOffOn,int nWhich)
{
	int	nn;
	CString Buff;

	nn=m_nGeneralOutput;

	switch(nWhich) {
	case 0:
		switch(bOffOn) {
		case FALSE:
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);		SendStr(Buff);
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);	SendStr(Buff);
			nn |= theApp.m_Tank.nBrakeBit0;
			break;
		case TRUE:
			nn &= (theApp.m_Tank.nBrakeBit0 ^ 0xff);
			break;
		}
		break;
	case 1:
		switch(bOffOn) {
		case FALSE:
			nn |= theApp.m_Tank.nBrakeBit1;
			break;
		case TRUE:
			nn &= (theApp.m_Tank.nBrakeBit1 ^ 0xff);
			break;
		}
		break;
	}

	WriteDpram(PMAC_GENERAL_OUTPUTS,m_nGeneralOutput=nn);
}


int CPmac::GetBrakeStatus(int nWhich)
{
	int nFlag;
	int	nn;


	nn=m_nGeneralOutput;
	switch(nWhich) {
	case 0:
		(nn&theApp.m_Tank.nBrakeBit0) == theApp.m_Tank.nBrakeBit0 ? nFlag = 1 : nFlag = 0;
		break;
	case 1:
		(nn&theApp.m_Tank.nBrakeBit1) == theApp.m_Tank.nBrakeBit1 ? nFlag = 1 : nFlag = 0;
		break;
	}
	return nFlag;
}

int CPmac::GetInputPort()
{
	return m_nGeneralInput;
}




int CPmac::ReadDpram(int nOffset)
{

	switch(m_nWhichDLL) {
	case COMMS32: return m_Comms32.ReadDpram(nOffset);
		break;
	case COMMSSERVER: return m_CommsServer.ReadDpram(nOffset);
		break;
	case PMACCOMMSUSL: return m_CommsUSL.ReadDpram(nOffset);
		break;
	case PMACISAUSL: return m_ServoUser.ReadDpram(nOffset);
		break;
	}
	return 0;
}


void CPmac::WriteDpram(int nOffset, int nValue, bool bDoMemCheck)
{

	switch(m_nWhichDLL) {
	case COMMS32: m_Comms32.WriteDpram(nOffset,nValue);
		break;
	case COMMSSERVER:// m_CommsServer.WriteDpram(nOffset,nValue);
		m_CommsServer.DpramSetMem(nOffset,1,&nValue);
		break;
	case PMACCOMMSUSL: m_CommsUSL.WriteDpram(nOffset,nValue, bDoMemCheck);
		break;
	case PMACISAUSL: m_ServoUser.WriteDpram(nOffset,nValue);
		break;
	}
}

int CPmac::DpramGetMem(int nOffset,int nLength,PVOID pMem)
{

	switch(m_nWhichDLL) {
	case COMMS32: m_Comms32.DpramGetMem(nOffset,nLength,pMem);
		break;
	case COMMSSERVER: m_CommsServer.DpramGetMem(nOffset,nLength,pMem);
		break;
	case PMACCOMMSUSL: return m_CommsUSL.DpramGetMem(nOffset,nLength,pMem);
		break;
	case PMACISAUSL: return m_ServoUser.DpramGetMem(nOffset,nLength,pMem);
		break;
	}

	return 1;
}

int CPmac::DpramSetMem(int nOffset,int nLength,PVOID pMem,bool bDoMemCheck)
{

	switch(m_nWhichDLL) {
	case COMMS32: m_Comms32.DpramSetMem(nOffset,nLength,pMem,bDoMemCheck);
		break;
	case COMMSSERVER: m_CommsServer.DpramSetMem(nOffset,nLength,pMem);
		break;
	case PMACCOMMSUSL: return m_CommsUSL.DpramSetMem(nOffset,nLength,pMem,bDoMemCheck);
		break;
	case PMACISAUSL: return m_ServoUser.DpramSetMem(nOffset,nLength,pMem);
		break;
	}

	return 1;
}




bool CPmac::SuspendMotors()
{
//	m_bJoystickEnabled=FALSE;

	theApp.m_bRThetaJoystick=false;
	WriteDpram(PMAC_MOVE_FLAG_1,0);
	SendStr(L"&1 Q");
	SendStr(L"&1 A");
	SendStr(L"&2 Q");
	SendStr(L"&2 A");


	SendStr(L"UNDEFINE ALL");
	SendStr(L"DISABLE PLCC 2..31");
//	DisableLookAhead();
	WaitForFreeCoordinateSystems();
	for(int nAxis=0;nAxis<32;nAxis++) {
		if(theApp.m_Axes[nAxis].nMotorType == SERVO) {
			theApp.m_Axes[nAxis].bEnabled=FALSE;
		}
	}
	WriteDpram(PMAC_MOVE_FLAG_1,0);

	return TRUE;

}

bool CPmac::WaitForFreeCoordinateSystems()
{
	int nLoop = 1000;
	if(m_nWhichDLL==PMACISAUSL)
		return true;

	while(ReadDpram(PMAC_COORDINATE_STATUS)) {
		Sleep(1);
		nLoop--;
		if(nLoop<=0) return false;
	};
	return TRUE;
}


bool CPmac::OriginFindAxis(int nWhichAxis)
{
	CString Buff;
	int result;
	CCoord CpHead;
	int nMask,nFiducial;

	if(SuspendMotors()==FALSE) return FALSE;

	SendStr(L"M2=0");		//use proximity switch on Axis 4

	nFiducial = 0;
	for(int nAxis = 0;nAxis<16;nAxis++) {
		nMask = 1;
		if(theApp.m_Axes[nAxis].bUseFiducial == TRUE) {
			nMask <<= (theApp.m_Axes[nAxis].nPhysicalAxis-1);
			nFiducial |= nMask;
		}
	}
	Buff.Format(L"M199=%d",nFiducial);
	SendStr(Buff);


	if(theApp.m_Axes[nWhichAxis].bAvailable==TRUE) {

		switch(theApp.m_LastSettings.nManipulatorType[0]) {
		case ROTATE_DIF_THETA:
			if((nWhichAxis == theApp.m_Tank.nXtLeft) || (nWhichAxis == theApp.m_Tank.nYtLeft) || (nWhichAxis == theApp.m_Tank.nXtRight) || (nWhichAxis == theApp.m_Tank.nYtLeft)) {
				OriginRTheta(nWhichAxis);
				return TRUE;
			}
			//No break so that the other axes can be homed normally
		default:
			SendStr(L"&1");
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[nWhichAxis].nPhysicalAxis,theApp.m_Axes[nWhichAxis].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[nWhichAxis].bEnabled=theApp.m_Axes[nWhichAxis].bAvailable;

			SetVariable(_T("I"),theApp.m_Axes[nWhichAxis].nPhysicalAxis*100+13,0);
			SetVariable(_T("I"),theApp.m_Axes[nWhichAxis].nPhysicalAxis*100+14,0);
			SetVariable(_T("I"),theApp.m_Axes[nWhichAxis].nPhysicalAxis*100+26,theApp.m_Axes[nWhichAxis].nLimitSwitchOffset);
			SetVariable(_T("I"),theApp.m_Axes[nWhichAxis].nPhysicalAxis*100+23,theApp.m_Axes[nWhichAxis].fHomeSpeed);

			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			Buff.Format(_T("&1 B%d R"),theApp.m_Axes[nWhichAxis].nPhysicalAxis + 10);
			result = SendStr(Buff);

			return TRUE;
			break;
		}
	}
	return FALSE;
}

bool CPmac::OriginAll(int nAction)
{
	CString Buff;
	int nLimitSwitches = 0;
	int nNumberAxes = GetNumberAxes();

	if(SuspendMotors()==FALSE) return FALSE;

	for(int nAxis=0;nAxis<nNumberAxes;nAxis++) {
		if((theApp.m_Axes[nAxis].bAvailable) && (theApp.m_Axes[nAxis].nMotorType==SERVO)) {
			if(theApp.m_Axes[nAxis].nStatus & (PMAC_MAX_SWITCH | PMAC_MIN_SWITCH)) {
				nLimitSwitches++;
			}
		}
	}
	if(nLimitSwitches) {
//		Buff.Format(L"There are %d limit switches tripped\r\nPlease move the axes off their limit switches",nLimitSwitches);
//		MessageBox(NULL,Buff,L"Error",MB_ICONERROR);
//		return false;
	}
	EnableAxes(ORIGIN_ALL);

	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nXLeft].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nYLeft].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nZLeft].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nXtLeft].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nYtLeft].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nXRight].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nYRight].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nZRight].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nXtRight].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nYtRight].nLimitSwitchOffset);

	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis*100+23,theApp.m_Axes[theApp.m_Tank.nXLeft].fHomeSpeed);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis*100+23,theApp.m_Axes[theApp.m_Tank.nYLeft].fHomeSpeed);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis*100+23,theApp.m_Axes[theApp.m_Tank.nZLeft].fHomeSpeed);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis*100+23,theApp.m_Axes[theApp.m_Tank.nXtLeft].fHomeSpeed);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis*100+23,theApp.m_Axes[theApp.m_Tank.nYtLeft].fHomeSpeed);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis*100+23,theApp.m_Axes[theApp.m_Tank.nXRight].fHomeSpeed);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis*100+23,theApp.m_Axes[theApp.m_Tank.nYRight].fHomeSpeed);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis*100+23,theApp.m_Axes[theApp.m_Tank.nZRight].fHomeSpeed);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis*100+23,theApp.m_Axes[theApp.m_Tank.nXtRight].fHomeSpeed);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis*100+23,theApp.m_Axes[theApp.m_Tank.nYtRight].fHomeSpeed);

	switch(theApp.m_Tank.nScannerDescription) {
	case SINGLE_BRIDGE_DUAL_SIDED:
		SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nXBridge].nLimitSwitchOffset);
		SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nRLeft].nLimitSwitchOffset);
		break;
	}

	switch(nAction) {
	case ORIGIN_ALL_LEFT:
		switch(theApp.m_nJobNumber) {
		default:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B24 R");
			break;
		case RRFLATPACK:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B26 R");
			break;
		}
		break;
	case ORIGIN_ALL_RIGHT:
		SetFeedRate(theApp.m_LastSettings.fFeedRate);
		SendStr(L"&2 B25 R");
		break;
	case ORIGIN_ALL:
		switch(theApp.m_nJobNumber) {
		default:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B24 R");
			SendStr(L"&2 B25 R");
			break;
		case GKN:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B26 R");
			break;
		}
		break;
	case ORIGIN_XYZ_LEFT:
		SetFeedRate(theApp.m_LastSettings.fFeedRate);
		SendStr(L"&1 B27 R");
		break;

	}

	return TRUE;
}

void CPmac::UpdateHardwareDesirePos()
{
	int	ii;
	int nNumberAxes = GetNumberAxes();
	int nPosArray[16];
	int nPhysicalAxis;

	for(ii=0;ii<nNumberAxes;ii++) {
		nPhysicalAxis = theApp.m_Axes[ii].nPhysicalAxis-1;

		if((theApp.m_Axes[ii].bAvailable) && (theApp.m_Axes[ii].nMotorType==SERVO)) {

			if(theApp.m_Axes[ii].nStepsPerRev==0) {

				nPosArray[nPhysicalAxis]=theApp.m_Kinematics.m_LeadScrew[nPhysicalAxis].ActualToMachine(theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset,theApp.m_PmacUser.m_nMachinePosition[nPhysicalAxis]);

				if((ii==theApp.m_Tank.nYtLeft) && (theApp.m_LastSettings.nManipulatorType[PORTSIDE]==ROTATE_DIF_THETA)) {
					nPosArray[nPhysicalAxis]+=theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos;
				}
				if((ii==theApp.m_Tank.nYtRight) && (theApp.m_LastSettings.nManipulatorType[STARBOARD]==ROTATE_DIF_THETA)) {
					nPosArray[nPhysicalAxis]+=theApp.m_Axes[theApp.m_Tank.nXtRight].nDesirePos;
				}

			} else {
				nPosArray[nPhysicalAxis]=theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset+(theApp.m_Axes[ii].nRevCounter * theApp.m_Axes[ii].nStepsPerRev);
			}
		};
	};
	DpramSetMem(PMAC_DESIRE_POS,16,nPosArray);

}


bool CPmac::MoveTo(bool bAction)
{
	int	ii,nMax,nTemp;
	int nNumberAxes = GetNumberAxes();
	int nPosArray[16];
	int	nDesirePos;
	int nPhysicalAxis;

	nMax=0;
	for(ii=0;ii<12;ii++) {
		if((theApp.m_Axes[ii].bAvailable) && (theApp.m_Axes[ii].nMotorType==SERVO)) {
			nPhysicalAxis = theApp.m_Axes[ii].nPhysicalAxis-1;

			if(theApp.m_Axes[ii].nStepsPerRev==0) {
				nPosArray[nPhysicalAxis]=theApp.m_Kinematics.m_LeadScrew[nPhysicalAxis].ActualToMachine(theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset,theApp.m_PmacUser.m_nMachinePosition[nPhysicalAxis]);

				if((ii==theApp.m_Tank.nYtLeft) && (theApp.m_LastSettings.nManipulatorType[PORTSIDE]==ROTATE_DIF_THETA)) {
					nPosArray[nPhysicalAxis]+=theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos;
				}
				if((ii==theApp.m_Tank.nYtRight) && (theApp.m_LastSettings.nManipulatorType[STARBOARD]==ROTATE_DIF_THETA)) {
					nPosArray[nPhysicalAxis]+=theApp.m_Axes[theApp.m_Tank.nXtRight].nDesirePos;
				}

				nTemp=CalcMoveTime(&theApp.m_Axes[ii],labs(theApp.m_Axes[ii].nDesirePos-theApp.m_Axes[ii].nPos));
			} else {
				if(abs(theApp.m_Axes[ii].nDesirePos - theApp.m_Axes[ii].nPos) < (theApp.m_Axes[ii].nStepsPerRev / 2) || m_bTakeShortestTurntableRoute == false) {
					nDesirePos = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset+(theApp.m_Axes[ii].nRevCounter * theApp.m_Axes[ii].nStepsPerRev);
				} else {

					if(theApp.m_Axes[ii].nDesirePos < theApp.m_Axes[ii].nPos) {
						nDesirePos = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset+((theApp.m_Axes[ii].nRevCounter+1) * theApp.m_Axes[ii].nStepsPerRev);
					} else {
						nDesirePos = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset+((theApp.m_Axes[ii].nRevCounter-1) * theApp.m_Axes[ii].nStepsPerRev);
					}

				}
				nPosArray[nPhysicalAxis] = nDesirePos;
				nTemp=CalcMoveTime(&theApp.m_Axes[ii],labs(theApp.m_Axes[ii].nDesirePos-theApp.m_Axes[ii].nPos));
			}
			if(nTemp>nMax) nMax=nTemp;
		};
	};
	DpramSetMem(PMAC_DESIRE_POS,16,nPosArray);

//	nMax = MulDiv(nMax,theApp.m_LastSettings.fFeedRate,100);


	WriteDpram(PMAC_TM,nMax);
	WriteDpram(PMAC_TA,theApp.m_Tank.nTA);
	WriteDpram(PMAC_TS,theApp.m_Tank.nTS);
	SetFeedRate(theApp.m_LastSettings.fFeedRate);

	if(theApp.m_Thread.m_bCollectPmacTrajectory==true) {
		theApp.m_Thread.m_CpTrajectory.Zero();
		theApp.m_Thread.m_CpTrajectory.AllocateCoords(60000);
		theApp.m_Thread.m_bCollectTrajectory=true;
	}

	SendStr(L"&1 B6 R");
	if(nNumberAxes>4)
		SendStr(L"&2 B7 R");

	if(bAction==WAIT) {
		Sleep(200);
		while(!IsMotionFinished()) {
			Sleep(1);
		};
	}

	return TRUE;
}

bool CPmac::MoveJustR(bool bAction)
{
	int	nMax,nTemp;
	int nNumberAxes = GetNumberAxes();
	int nDesirePos;
	int nPosArray[16];

	nMax=0;
	if((theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nRLeft].nMotorType==SERVO)) {
		if(theApp.m_Axes[theApp.m_Tank.nRLeft].nStepsPerRev==0) {
			nDesirePos=theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset;
		} else {
			nDesirePos = (theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos + theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset) % theApp.m_Axes[theApp.m_Tank.nRLeft].nStepsPerRev;
			nDesirePos += (theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter * theApp.m_Axes[theApp.m_Tank.nRLeft].nStepsPerRev);
		}
		nPosArray[theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis-1] = nDesirePos;
		DpramSetMem(PMAC_DESIRE_POS,12,nPosArray);

		nTemp=(int)((float)labs(theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos-theApp.m_Axes[theApp.m_Tank.nRLeft].nPos) / theApp.m_Axes[theApp.m_Tank.nRLeft].fMaxMoveSpeed);
		if(nTemp>nMax) nMax=nTemp;
	};

	nMax = MulDiv(nMax,(int)theApp.m_LastSettings.fFeedRate,100);

//	WriteDpram(PMAC_MOVE_FLAG,1);
	WriteDpram(PMAC_TM,nMax);
	WriteDpram(PMAC_TA,theApp.m_Tank.nTA);
	WriteDpram(PMAC_TS,theApp.m_Tank.nTS);
	SetFeedRate(theApp.m_LastSettings.fFeedRate);
	SendStr(L"&1 B8 R");
	if(bAction==WAIT) {
		Sleep(200);
		while(!IsMotionFinished()) {
			Sleep(1);
		};
	}

	return TRUE;
}

void CPmac::EnableAxes(int nAction)
{
	SetPLCInterlocks();
	ClearGeoBrickFaults();

	switch(theApp.m_Tank.nScannerDescription) {
	case VERTICAL_BORE_SCANNER:	EnableVerticalBoreScanner(nAction);
		break;
	case SINGLE_BRIDGE_TANK_WITH_R: Enable6Axes(nAction);
		break;
	case DUAL_SIDED_7_AXES:	Enable7Axes(nAction);
		break;
	case SINGLE_BRIDGE_DUAL_SIDED:  Enable12Axes(nAction);
		break;
	case DUAL_SIDED_SQUIRTER_2:		Enable10AxesVersion2(nAction);
		break;
	case SINGLE_BRIDGE_TT_ROLLERS:	EnableSingleBridgeTTRollers(nAction);
		break;
	default: Enable10Axes(nAction);
		 break;
	}
}

void CPmac::EnableAxis(int nAxisStruct,int nPhysicalAxisOffset, char cCoordAxis)
{
	CString Buff;

	if((theApp.m_Axes[nAxisStruct].bAvailable==TRUE) && (DoesAxisNeedHoming(nAxisStruct)==FALSE)) {
		switch(theApp.m_Axes[nAxisStruct].eAmplifierType) {
		default:
			Buff.Format(_T("#%d->%c#%dj/"),theApp.m_Axes[nAxisStruct].nPhysicalAxis + nPhysicalAxisOffset,cCoordAxis ,theApp.m_Axes[nAxisStruct].nPhysicalAxis + nPhysicalAxisOffset);
			break;
		case GeoBrick:
			Buff.Format(_T("#%d->%c#%d$"),theApp.m_Axes[nAxisStruct].nPhysicalAxis + nPhysicalAxisOffset,cCoordAxis ,theApp.m_Axes[nAxisStruct].nPhysicalAxis + nPhysicalAxisOffset);
			break;
		}
		SendStr(Buff);
		theApp.m_Axes[nAxisStruct].bEnabled=theApp.m_Axes[nAxisStruct].bAvailable;
	}
}

void CPmac::EnableAxis(int nAxisStruct)
{
	CString Buff;

	if((theApp.m_Axes[nAxisStruct].bAvailable==TRUE) && (DoesAxisNeedHoming(nAxisStruct)==FALSE)) {
		switch(theApp.m_Axes[nAxisStruct].eAmplifierType) {
		default:
			Buff.Format(_T("#%dj/"),theApp.m_Axes[nAxisStruct].nPhysicalAxis);
			break;
		case GeoBrick:
			Buff.Format(_T("#%d$"),theApp.m_Axes[nAxisStruct].nPhysicalAxis);
			break;
		}
		SendStr(Buff);
		theApp.m_Axes[nAxisStruct].bEnabled=theApp.m_Axes[nAxisStruct].bAvailable;
	}
}

void CPmac::Enable10AxesVersion2(int nAction)
{
	CString Buff;

	DisableLookAhead();
	SetPLCInterlocks();

	switch(nAction) {
	case BOTH_MOVETO:
	case BOTH_POLY_MOVE:
	case BOTH_MOVETO_WITHOUT_R:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		EnableAxis(theApp.m_Tank.nXRight,0,'C');
		EnableAxis(theApp.m_Tank.nYRight,0,'U');
		EnableAxis(theApp.m_Tank.nZRight,0,'V');
		EnableAxis(theApp.m_Tank.nXtRight,0,'W');
		SendStr(L"&2");
		EnableAxis(theApp.m_Tank.nXLeft,20,'X');
		EnableAxis(theApp.m_Tank.nYLeft,20,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,20,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,20,'A');
		EnableAxis(theApp.m_Tank.nYtRight,0,'B');
		EnableAxis(theApp.m_Tank.nXRight,20,'C');
		EnableAxis(theApp.m_Tank.nYRight,20,'U');
		EnableAxis(theApp.m_Tank.nZRight,20,'V');
		EnableAxis(theApp.m_Tank.nXtRight,20,'W');
		break;
	case LEFT_MOVETO:
	case LEFT_POLY_MOVE:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		break;
	case RIGHT_MOVETO:
	case RIGHT_POLY_MOVE:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXRight,0,'X');
		EnableAxis(theApp.m_Tank.nYRight,0,'Y');
		EnableAxis(theApp.m_Tank.nZRight,0,'Z');
		EnableAxis(theApp.m_Tank.nXtRight,0,'A');
		EnableAxis(theApp.m_Tank.nYtRight,0,'B');
		break;
	case ENABLE_JOYSTICK:
		EnableAxis(theApp.m_Tank.nXLeft);
		EnableAxis(theApp.m_Tank.nYLeft);
		EnableAxis(theApp.m_Tank.nZLeft);
		EnableAxis(theApp.m_Tank.nXtLeft);
		EnableAxis(theApp.m_Tank.nYtLeft);
		EnableAxis(theApp.m_Tank.nXRight);
		EnableAxis(theApp.m_Tank.nYRight);
		EnableAxis(theApp.m_Tank.nZRight);
		EnableAxis(theApp.m_Tank.nXtRight);
		EnableAxis(theApp.m_Tank.nYtRight);
		SetFeedRate(theApp.m_LastSettings.fFeedRate);
		DownloadSoftwareOverTravelLimits();
		break;
	case ORIGIN_ALL:
		DisableSoftwareOverTravelLimits();
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		SendStr(L"&2");
		EnableAxis(theApp.m_Tank.nXRight,0,'X');
		EnableAxis(theApp.m_Tank.nYRight,0,'Y');
		EnableAxis(theApp.m_Tank.nZRight,0,'Z');
		EnableAxis(theApp.m_Tank.nXtRight,0,'A');
		EnableAxis(theApp.m_Tank.nYtRight,0,'B');
		break;
	case TUNNING:
		SendStr(L"&1");
		EnableAxis(m_nTunningAxis,0,'X');
		break;

	}
	if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
}

void CPmac::EnableVerticalBoreScanner(int nAction)
{
	CString Buff;

	DisableLookAhead();
	SetPLCInterlocks();

	switch(nAction) {
	case BOTH_MOVETO:
	case BOTH_POLY_MOVE:
	case BOTH_MOVETO_WITHOUT_R:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		break;
	case BOTH_MOVETO_WITH_R:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nRLeft,0,'C');
		break;
	case LEFT_MOVETO:
	case LEFT_MOVETO_WITH_R:
	case LEFT_POLY_MOVE:
		SendStr(L"&1");
//		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
//		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
//		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
//		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
//		EnableAxis(theApp.m_Tank.nRLeft,0,'C');
		break;
	case RIGHT_MOVETO:
	case RIGHT_POLY_MOVE:
		break;
	case ENABLE_JOYSTICK:
		EnableAxis(theApp.m_Tank.nXLeft);
		EnableAxis(theApp.m_Tank.nYLeft);
		EnableAxis(theApp.m_Tank.nZLeft);
		EnableAxis(theApp.m_Tank.nXtLeft);
		EnableAxis(theApp.m_Tank.nYtLeft);
		EnableAxis(theApp.m_Tank.nRLeft);
		SetFeedRate(theApp.m_LastSettings.fFeedRate);
		DownloadSoftwareOverTravelLimits();
		break;
	case ORIGIN_ALL:
		DisableSoftwareOverTravelLimits();
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		EnableAxis(theApp.m_Tank.nRLeft,0,'C');
		break;
	case TUNNING:
		SendStr(L"&1");
		EnableAxis(m_nTunningAxis,0,'X');
		break;

	}
	if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);

}

void CPmac::Enable6Axes(int nAction)
{
	CString Buff;

	DisableLookAhead();
	SetPLCInterlocks();

	switch(nAction) {
	case BOTH_MOVETO:
	case BOTH_POLY_MOVE:
	case BOTH_MOVETO_WITH_R:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		EnableAxis(theApp.m_Tank.nRLeft,0,'C');
		break;
	case BOTH_MOVETO_WITHOUT_R:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		break;
	case LEFT_MOVETO:
	case LEFT_MOVETO_WITH_R:
	case LEFT_POLY_MOVE:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		EnableAxis(theApp.m_Tank.nRLeft,0,'C');
		break;
	case RIGHT_MOVETO:
	case RIGHT_POLY_MOVE:
		break;
	case ENABLE_JOYSTICK:
		EnableAxis(theApp.m_Tank.nXLeft);
		EnableAxis(theApp.m_Tank.nYLeft);
		EnableAxis(theApp.m_Tank.nZLeft);
		EnableAxis(theApp.m_Tank.nXtLeft);
		EnableAxis(theApp.m_Tank.nYtLeft);
		EnableAxis(theApp.m_Tank.nRLeft);
		SetFeedRate(theApp.m_LastSettings.fFeedRate);
		DownloadSoftwareOverTravelLimits();
		break;
	case ORIGIN_ALL:
		DisableSoftwareOverTravelLimits();
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		EnableAxis(theApp.m_Tank.nRLeft,0,'C');
		break;
	case TUNNING:
		SendStr(L"&1");
		EnableAxis(m_nTunningAxis,0,'X');
		break;

	}
	if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
}


void CPmac::Enable7Axes(int nAction)
{
	CString Buff;

	DisableLookAhead();
	SetPLCInterlocks();

	switch(nAction) {
	case BOTH_MOVETO:
	case BOTH_POLY_MOVE:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYRight,0,'B');
		EnableAxis(theApp.m_Tank.nZRight,0,'C');
		EnableAxis(theApp.m_Tank.nYtRight,0,'U');
		break;
	case BOTH_MOVETO_WITHOUT_R:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYRight,0,'B');
		break;
	case LEFT_MOVETO:
	case LEFT_POLY_MOVE:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'A');
		break;
	case RIGHT_MOVETO:
	case RIGHT_POLY_MOVE:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXRight,0,'X');
		EnableAxis(theApp.m_Tank.nYRight,0,'Y');
		EnableAxis(theApp.m_Tank.nZRight,0,'Z');
		EnableAxis(theApp.m_Tank.nYtRight,0,'A');
		break;
	case ENABLE_JOYSTICK:
		EnableAxis(theApp.m_Tank.nXLeft);
		EnableAxis(theApp.m_Tank.nYLeft);
		EnableAxis(theApp.m_Tank.nZLeft);
		EnableAxis(theApp.m_Tank.nXtLeft);
		EnableAxis(theApp.m_Tank.nYtLeft);
		EnableAxis(theApp.m_Tank.nXRight);
		EnableAxis(theApp.m_Tank.nYRight);
		EnableAxis(theApp.m_Tank.nZRight);
		EnableAxis(theApp.m_Tank.nXtRight);
		EnableAxis(theApp.m_Tank.nYtRight);
		SetFeedRate(theApp.m_LastSettings.fFeedRate);
		break;
	case ORIGIN_ALL:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'B');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'A');
		SendStr(L"&2");
		EnableAxis(theApp.m_Tank.nYRight,0,'Y');
		EnableAxis(theApp.m_Tank.nZRight,0,'Z');
		EnableAxis(theApp.m_Tank.nYtRight,0,'A');
		break;
	case TUNNING:
		SendStr(L"&1");
		EnableAxis(m_nTunningAxis,0,'X');
		break;

	}
	if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
}

void CPmac::EnableSingleBridgeTTRollers(int nAction)
{
	CString Buff;

	DisableLookAhead();
	SetPLCInterlocks();

	switch(nAction) {
	case BOTH_MOVETO:
	case BOTH_POLY_MOVE:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		EnableAxis(theApp.m_Tank.nRLeft,0,'C');
		EnableAxis(theApp.m_Tank.nRRight,0,'U');
		break;
	case LEFT_MOVETO:
	case LEFT_POLY_MOVE:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nRLeft,0,'C');
		EnableAxis(theApp.m_Tank.nRRight,0,'U');
		break;
	case RIGHT_MOVETO:
	case RIGHT_POLY_MOVE:
		break;
	case ENABLE_JOYSTICK:
		EnableAxis(theApp.m_Tank.nXLeft);
		EnableAxis(theApp.m_Tank.nYLeft);
		EnableAxis(theApp.m_Tank.nZLeft);
		EnableAxis(theApp.m_Tank.nXtLeft);
		EnableAxis(theApp.m_Tank.nYtLeft);
		EnableAxis(theApp.m_Tank.nXRight);
		EnableAxis(theApp.m_Tank.nYRight);
		EnableAxis(theApp.m_Tank.nZRight);
		EnableAxis(theApp.m_Tank.nXtRight);
		EnableAxis(theApp.m_Tank.nYtRight);
		EnableAxis(theApp.m_Tank.nRLeft);
		EnableAxis(theApp.m_Tank.nRRight);
		SetFeedRate(theApp.m_LastSettings.fFeedRate);
		break;
	case ORIGIN_ALL:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		EnableAxis(theApp.m_Tank.nRLeft,0,'C');
		break;
	case TUNNING:
		SendStr(L"&1");
		EnableAxis(m_nTunningAxis,0,'X');
		break;

	}
	if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
}

void CPmac::Enable10Axes(int nAction)
{
	CString Buff;

	DisableLookAhead();
//	DownloadPIDForManipulators();
	ResetAllMaxSpeeds();
	SetPLCInterlocks();

	for(int ii=0;ii<10;ii++) {
		Buff.Format(_T("P%d=0"),ii+220);	//clear encoder failure flag
		SendStr(Buff);
	}

	switch(nAction) {
	case BOTH_MOVETO:
	case ENABLE_JOYSTICK_R_THETA:
	case BOTH_MOVETO_WITHOUT_R:
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXLeft)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYLeft)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZLeft)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtLeft)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};

		SendStr(L"&2");
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
		};
		
		if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
		break;
	case BOTH_MOVETO_WITH_R:
		//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXLeft)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYLeft)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZLeft)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtLeft)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nRLeft)==FALSE)) {
			Buff.Format(_T("#%d->C#%dj/"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nRLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable;
		};

		SendStr(L"&2");
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
		};
		
		if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
		break;
	case MOVE_JUST_R:
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nRLeft)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nRLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable;
		};
		break;
	case ENABLE_JOYSTICK:
		//No coordinate System but enable the amplifiers
		//Joystick Plc
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		DownloadSoftwareOverTravelLimits();
		EnableAxis(theApp.m_Tank.nXLeft);
		EnableAxis(theApp.m_Tank.nYLeft);
		EnableAxis(theApp.m_Tank.nZLeft);
		EnableAxis(theApp.m_Tank.nXtLeft);
		EnableAxis(theApp.m_Tank.nYtLeft);
		EnableAxis(theApp.m_Tank.nXRight);
		EnableAxis(theApp.m_Tank.nYRight);
		EnableAxis(theApp.m_Tank.nZRight);
		EnableAxis(theApp.m_Tank.nXtRight);
		EnableAxis(theApp.m_Tank.nYtRight);
		EnableAxis(theApp.m_Tank.nRLeft);
		EnableAxis(theApp.m_Tank.nRRight);
/*
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nRLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nRLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nRRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nRRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nRRight].bEnabled=TRUE;
		}
		*/
		if(theApp.m_LastSettings.nBrakeScannerMode==1) {
			Sleep(300);
			theApp.m_Motors.ZBrakesEnable(FALSE,0);
		}
		break;
	case ORIGIN_ALL:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		DisableSoftwareOverTravelLimits();
		SendStr(L"&1");
		if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->C#%dj/"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nRLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable;
		};

		SendStr(L"&2");
		if(theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
		};
		if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
		break;
	case BOTH_POLY_MOVE:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXLeft)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYLeft)==FALSE)) {
			Buff.Format(_T("#%d->C#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZLeft)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtLeft)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%d->U#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%d->V#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};

		switch(theApp.m_Tank.nTrackingTenthAxis) {
		case 0:
			if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
				Buff.Format(_T("#%d->W#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
				SendStr(Buff);
				theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
			};
			SendStr(L"&2");
			if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
				Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
				SendStr(Buff);
				theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
			};
			break;
		default:
			if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
				Buff.Format(_T("#%d->W#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
				SendStr(Buff);
				theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
			};
			SendStr(L"&2");
			if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
				Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
				SendStr(Buff);
				theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
			};
			break;
		}
/*		switch(theApp.m_LastSettings.nManipulatorType[PORTSIDE]) {
		case 0:
			if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
				Buff.Format(_T("#%d->W#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
				SendStr(Buff);
				theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
			};
			SendStr(L"&2");
			if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
				Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
				SendStr(Buff);
				theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
			};
			break;
		case 1:
			if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
				Buff.Format(_T("#%d->W#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
				SendStr(Buff);
				theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
			};
			SendStr(L"&2");
			if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
				Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
				SendStr(Buff);
				theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
			};
			break;
		}
		*/
		break;
	case LEFT_MOVETO:
	case LEFT_POLY_MOVE:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		break;
	case LEFT_MOVETO_WITH_R:
		SendStr(L"&1");
		EnableAxis(theApp.m_Tank.nXLeft,0,'X');
		EnableAxis(theApp.m_Tank.nYLeft,0,'Y');
		EnableAxis(theApp.m_Tank.nZLeft,0,'Z');
		EnableAxis(theApp.m_Tank.nXtLeft,0,'A');
		EnableAxis(theApp.m_Tank.nYtLeft,0,'B');
		EnableAxis(theApp.m_Tank.nRLeft,0,'C');
		break;
	case RIGHT_MOVETO:
	case RIGHT_POLY_MOVE:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
		};
		break;
	case TUNNING:
		SendStr(L"&1");
		if(theApp.m_Axes[m_nTunningAxis].bAvailable==TRUE) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[m_nTunningAxis].nPhysicalAxis,theApp.m_Axes[m_nTunningAxis].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[m_nTunningAxis].bEnabled=theApp.m_Axes[m_nTunningAxis].bAvailable;
		};
		break;

	}
	if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
}

void CPmac::Enable12Axes(int nAction)
{
	CString Buff;

	DisableLookAhead();
//	DownloadPIDForManipulators();
	ResetAllMaxSpeeds();
	SetPLCInterlocks();

	switch(nAction) {
	case BOTH_MOVETO:
	case BOTH_MOVETO_WITHOUT_R:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXBridge)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXBridge].bEnabled=theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXLeft)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYLeft)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZLeft)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtLeft)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			Buff.Format(_T("#%d->C#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nRLeft)==FALSE)) {
			Buff.Format(_T("#%d->U#%dj/"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nRLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable;
		};


		SendStr(L"&2");
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
		};
		if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
		break;
	case BOTH_MOVETO_WITH_R:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXBridge)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXBridge].bEnabled=theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXLeft)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYLeft)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZLeft)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtLeft)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			Buff.Format(_T("#%d->C#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};

		if((theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nRLeft)==FALSE)) {
			Buff.Format(_T("#%d->U#%dj/"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nRLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable;
		};

		SendStr(L"&2");
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
		};
		if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
		break;
	case ENABLE_JOYSTICK:
		//No coordinate System but enable the amplifiers
		//Joystick Plc
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		if((theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXBridge)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXBridge].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=TRUE;
		}
		if((theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nRLeft)==FALSE)) {
			Buff.Format(_T("#%d->C#%dj/"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nRLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable;
		};
		if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
		break;
	case ORIGIN_ALL:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};

		SendStr(L"&2");
		if(theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
		};
		if(theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
		};

		SendStr(L"&3");
		if(theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable==TRUE) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXBridge].bEnabled=theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable;
		};
		break;
	case LEFT_POLY_MOVE:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXBridge)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXBridge].bEnabled=theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXLeft)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYLeft)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZLeft)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtLeft)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			Buff.Format(_T("#%d->C#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};
		break;
	case RIGHT_POLY_MOVE:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXBridge)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXBridge].bEnabled=theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
			Buff.Format(_T("#%d->C#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
		};
		break;
	case BOTH_POLY_MOVE:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXBridge)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXBridge].bEnabled=theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXLeft)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYLeft)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZLeft)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtLeft)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			Buff.Format(_T("#%d->C#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%d->U#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%d->V#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%d->W#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};


		SendStr(L"&2");
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
		};
		break;
	case LEFT_MOVETO:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXLeft)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYLeft)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZLeft)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtLeft)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtLeft)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXBridge)==FALSE)) {
			Buff.Format(_T("#%d->C#%dj/"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXBridge].bEnabled=theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable;
		};
		break;
	case RIGHT_MOVETO:
//		if(AreMotorsPowerd()==FALSE) return FALSE;
//		if(IsMovementAllowed()==FALSE) return FALSE;
		SendStr(L"&1");
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXRight)==FALSE)) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYRight)==FALSE)) {
			Buff.Format(_T("#%d->Y#%dj/"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nZRight)==FALSE)) {
			Buff.Format(_T("#%d->Z#%dj/"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nZRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nXtRight)==FALSE)) {
			Buff.Format(_T("#%d->A#%dj/"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nXtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable;
		};
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) && (DoesAxisNeedHoming(theApp.m_Tank.nYtRight)==FALSE)) {
			Buff.Format(_T("#%d->B#%dj/"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[theApp.m_Tank.nYtRight].bEnabled=theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable;
		};
		break;
	case TUNNING:
		SendStr(L"&1");
		if(theApp.m_Axes[m_nTunningAxis].bAvailable==TRUE) {
			Buff.Format(_T("#%d->X#%dj/"),theApp.m_Axes[m_nTunningAxis].nPhysicalAxis,theApp.m_Axes[m_nTunningAxis].nPhysicalAxis);
			SendStr(Buff);
			theApp.m_Axes[m_nTunningAxis].bEnabled=theApp.m_Axes[m_nTunningAxis].bAvailable;
		};
		break;
	}
	if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);
}

int CPmac::CalcMoveTime(struct AxisData *pAxis,int	nTotalS)
{
	float	vel,accel;
	float	s0,s1,s4,ds;
	float	t0,t1,t4,dt;
	int	nTemp;


	s4=(float)nTotalS * pAxis->fStepSize;

	vel=pAxis->fMaxMoveSpeed*pAxis->fStepSize*1000.0f;
	accel=pAxis->fAcceleration*pAxis->fStepSize*1000.0f;

	if(accel==0.0) accel=1.0;

	t0=s0=0;
	t1=vel/accel;
	s1=0.5f * accel * (t1 * t1) /1000.0f;
	if((s1 * 2) > s4) {
		s1 = s4 / 2;
		t1=sqrtf((2.0f * s1) / accel * 1000.0f);
		t1 = sqrtf((float)nTotalS / pAxis->fAcceleration);
		t4= t1 * 2;
	} else {
		ds = (s4 - (s1 * 2));
		dt = ds / vel;
		t4 = t1 * 2 + dt;
	};

	nTemp=(int)t4;
	return nTemp;
}

BOOL CPmac::IsAxisInPosition(int nWhich)
{
	if(theApp.m_Axes[nWhich].nStatus&PMAC_IN_POS) return TRUE;
	return FALSE;
}

BOOL CPmac::IsPhysicalAxisInPosition(int nWhich)
{
	if(m_nStatus[nWhich-1]&PMAC_IN_POS) return TRUE;
	return FALSE;
}


BOOL CPmac::IsMotionFinished()
{
	int	ii;
	int nAxes=GetNumberAxes();

	if(theApp.m_nJobNumber==GKN){
		nAxes=12;
	}
//	if(theApp.m_PmacUser.m_nCoordinateStatus != 0 ) return false;

	if(ReadDpram(PMAC_MOVE_FLAG_1) != 0) 
		return FALSE;
	if(ReadDpram(PMAC_COORDINATE_STATUS) != 0) 
		return FALSE;

	for(ii=0;ii<nAxes;ii++) {
		if((theApp.m_Axes[ii].bAvailable)&&(theApp.m_Axes[ii].nMotorType==SERVO)&&(theApp.m_Axes[ii].bEnabled)) {
			if(!(theApp.m_Axes[ii].nStatus&PMAC_IN_POS)) return FALSE; //something has not finished
		}
	}
	return TRUE;	// All motion has finished
}

bool CPmac::IsMachineZero()
{
	int	ii;
	int nAxes=GetNumberAxes();

	if(theApp.m_nJobNumber==GKN){
		nAxes=12;
	}

	for(ii=0;ii<nAxes;ii++) {
		if((theApp.m_Axes[ii].bAvailable)&&(theApp.m_Axes[ii].nMotorType==SERVO)&&(theApp.m_Axes[ii].bEnabled)) {
			if(abs(theApp.m_Axes[ii].nPos) < 100) return FALSE;
		}
	}
	return TRUE;	// All relevent axes are at zero

}

BOOL CPmac::IsMoveRFinished()
{
	if(ReadDpram(PMAC_MOVE_FLAG_1) == 1) return FALSE;
	if((theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable)&&(theApp.m_Axes[theApp.m_Tank.nRLeft].nMotorType==SERVO)) {
		if(!(theApp.m_Axes[theApp.m_Tank.nRLeft].nStatus&PMAC_IN_POS)) return FALSE; //something has not finished
	}
	return TRUE;	// All motion has finished

}


BOOL CPmac::IsPolyMotionFinished()
{
	int	ii;

	if(theApp.m_PmacUser.m_nCoordinateStatus == 0) return true;

	if(ReadDpram(PMAC_MOVE_FLAG_1) != 0) 
		return FALSE;
	if(ReadDpram(PMAC_COORDINATE_STATUS) != 0) 
		return FALSE;

	for(ii=0;ii<10;ii++) {	//Note only 9 Axes
		if(theApp.m_Axes[ii].bAvailable) {
			if(!(theApp.m_Axes[ii].nStatus&PMAC_IN_POS)) return FALSE; //something has not finished
		}
	}
	return TRUE;	// All motion has finished
}

bool CPmac::DoesAxisNeedHoming(int nWhichAxis)
{
	if(theApp.m_Axes[nWhichAxis].bMustHome==TRUE) {
		if(!(theApp.m_Axes[nWhichAxis].nStatus&0x400l)) return TRUE;
	};
	return FALSE;
}






void CPmac::StartJoystick()
{
	int nArray[16];
	CMotors* pM = &theApp.m_Motors;

	SuspendMotors();
	SetLimitSwitchesToDefault();
	theApp.m_Motors.GetSurfacePos(&m_CpJoystickSurface);

	switch(theApp.m_LastSettings.nManipulatorType[PORTSIDE]) {
	default: EnableAxes(ENABLE_JOYSTICK);
		break;
	case ROTATE_DIF_THETA: EnableAxes(ENABLE_JOYSTICK_R_THETA);
		break;
	}
	int nXLeft = theApp.m_Tank.nXLeft;
	int nYLeft = theApp.m_Tank.nYLeft;
	int nZLeft = theApp.m_Tank.nZLeft;
	int nXtLeft = theApp.m_Tank.nXtLeft;
	int nYtLeft = theApp.m_Tank.nYtLeft;
	int nRLeft = theApp.m_Tank.nRLeft;
	int nXRight = theApp.m_Tank.nXRight;
	int nYRight = theApp.m_Tank.nYRight;
	int nZRight = theApp.m_Tank.nZRight;
	int nXtRight = theApp.m_Tank.nXtRight;
	int nYtRight = theApp.m_Tank.nYtRight;
	int nRRight = theApp.m_Tank.nRRight;

	m_bDriveAwayRequired[0] = false;
	m_bDriveAwayRequired[1] = false;

	theApp.m_Motors.EnsureJoystickValuesAreCurrent();

	if(pM->m_nJoyValue[theApp.m_Axes[nXLeft].nJoystickNumber]>10) theApp.m_Axes[nXLeft].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nXLeft].nJoystickNumber];
	if(pM->m_nJoyValue[theApp.m_Axes[nYLeft].nJoystickNumber]>10) theApp.m_Axes[nYLeft].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nYLeft].nJoystickNumber];
	if(pM->m_nJoyValue[theApp.m_Axes[nZLeft].nJoystickNumber]>10) theApp.m_Axes[nZLeft].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nZLeft].nJoystickNumber];
	if(pM->m_nJoyValue[theApp.m_Axes[nXtLeft].nJoystickNumber]>10) theApp.m_Axes[nXtLeft].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nXtLeft].nJoystickNumber];
	if(pM->m_nJoyValue[theApp.m_Axes[nYtLeft].nJoystickNumber]>10) theApp.m_Axes[nYtLeft].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nYtLeft].nJoystickNumber];
	if(pM->m_nJoyValue[theApp.m_Axes[nRLeft].nJoystickNumber]>10) theApp.m_Axes[nRLeft].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nRLeft].nJoystickNumber];

	if(pM->m_nJoyValue[theApp.m_Axes[nXRight].nJoystickNumber]>10) theApp.m_Axes[nXRight].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nXRight].nJoystickNumber];
	if(pM->m_nJoyValue[theApp.m_Axes[nYRight].nJoystickNumber]>10) theApp.m_Axes[nYRight].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nYRight].nJoystickNumber];
	if(pM->m_nJoyValue[theApp.m_Axes[nZRight].nJoystickNumber]>10) theApp.m_Axes[nZRight].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nZRight].nJoystickNumber];
	if(pM->m_nJoyValue[theApp.m_Axes[nXtRight].nJoystickNumber]>10) theApp.m_Axes[nXtRight].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nXtRight].nJoystickNumber];
	if(pM->m_nJoyValue[theApp.m_Axes[nYtRight].nJoystickNumber]>10) theApp.m_Axes[nYtRight].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nYtRight].nJoystickNumber];
	if(pM->m_nJoyValue[theApp.m_Axes[nRRight].nJoystickNumber]>10) theApp.m_Axes[nRRight].nJoystickZero=pM->m_nJoyValue[theApp.m_Axes[nRRight].nJoystickNumber];


	for(int ii=0;ii<16;ii++) {
		if((theApp.m_Axes[ii].bAvailable) && (theApp.m_Axes[ii].nMotorType==SERVO)) {
			theApp.m_Axes[ii].nDesirePos=theApp.m_Axes[ii].nPos;
			nArray[theApp.m_Axes[ii].nPhysicalAxis-1]=theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset;
		};
	};
	switch(theApp.m_LastSettings.nJoystickTipMode) {
	case 0:
	case 1:
		nArray[theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis-1] += theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos;
		break;
	}
	DpramSetMem(PMAC_DESIRE_POS,16,nArray);
	for(int ii=0;ii<10;ii++) {
//		DpramSetMem(PMAC_DPRAM_DATA_START+ii*12,12,nArray);
	}

	ZeroMemory(nArray,sizeof nArray);
	DpramSetMem(PMAC_JOY_DEV,12,nArray);
	WriteDpram(PMAC_JOYSTICK_TIP_MODE, 0);


	switch(theApp.m_LastSettings.nManipulatorType[PORTSIDE]) {
	default: 
		if(theApp.m_nJobNumber==DOWTY_BSCAN) {
			SendStr(L"ENABLE PLC 2");
		 }else{
			SendStr(L"ENABLE PLCC 2");
		 }
		break;
	case ROTATE_DIF_THETA:
		//??????
		WriteDpram(PMAC_MOVE_FLAG_1,1);
		WriteDpram(PMAC_LOOP_COUNT_1, m_nJoystickLoopCount=0);		//M5
		WriteDpram(PMAC_NUMBER_COORDS, m_nJoystickLoopCount);		//M3
		WriteDpram(PMAC_TM,theApp.m_Tank.nTM=1);
		WriteDpram(PMAC_TA,theApp.m_Tank.nTA=100);
		WriteDpram(PMAC_TS,theApp.m_Tank.nTS=5);
		SendStr(L"&1 B40 R");
		theApp.m_bRThetaJoystick = true;
		break;
	}

}

void CPmac::PLCJoystickServiceRoutine(int nNotSafe)
{
	if(theApp.m_LastSettings.nManipulatorType[PORTSIDE] == ROTATE_DIF_THETA) return;

	switch(theApp.m_Tank.nScannerDescription){
	default: PLCJoystickServiceRoutine10Axes(nNotSafe);
		break;
	case SINGLE_BRIDGE_TT_ROLLERS:
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TANK: PLCJoystickServiceRoutingSBT(nNotSafe);
		break;
	case SINGLE_BRIDGE_DUAL_SIDED: PLCJoystickServiceRoutine12Axes(nNotSafe);
		break;
	}
}


void CPmac::AbortRotateThetaJoystick()
{
	if(theApp.m_LastSettings.nManipulatorType[PORTSIDE] == ROTATE_DIF_THETA) {
		SuspendMotors();
	}
}



void CPmac::RotateThetaJoystick(int nNotSafe)
{
	int	nXTemp,nYTemp,nZTemp,nSTemp,nXtTemp,nYtTemp,nTemp,nSign;
	bool bDeadManSZ = false;
	bool bDeadManXY = false;
	bool bDesirePosChanged = false;
	int	nReqPos,nJoystickAction=0;
	int	nJoystickGain,nWhichAxis,nSide,nMaxDeviation,ii,nPtr;
	AxisData *pGAxis=NULL;
	AxisData *pHAxis=NULL;
	int nArray[16];
	CCoord CpHead;
	static int nOldJoystickTipMode = 0;
	int nGenInputs = theApp.m_Motors.GetInputPort();


	#define	LEFT_JOYSTICK	1
	#define	RIGHT_JOYSTICK	2
	#define	BOTH_JOYSTICK	3
	#define	MAIN_AXES	0
	#define	TIP_AXES	1



	theApp.m_Motors.EnsureJoystickValuesAreCurrent();

	nXTemp=theApp.m_Motors.m_nJoyValue[0];
	nYTemp=theApp.m_Motors.m_nJoyValue[1];
	nZTemp=theApp.m_Motors.m_nJoyValue[2];
	nXtTemp=theApp.m_Motors.m_nJoyValue[0];
	nYtTemp=theApp.m_Motors.m_nJoyValue[1];
	nSTemp=theApp.m_Motors.m_nJoyValue[3];

	switch(theApp.m_Tank.nJoystickSource) {
	case WIRED_SI10:
	case WIRED_SERIAL:							nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nJoyValue[4]<15)		nSide = RIGHT_JOYSTICK;
		if(theApp.m_Motors.m_nJoyValue[4] > 220)	nSide = LEFT_JOYSTICK;
		nMaxDeviation = 50;
		bDeadManXY = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		bDeadManXY = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		bDeadManSZ = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	case WIRELESS_SERIAL:												nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nLeftSideBit)	nSide = LEFT_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nRightSideBit) nSide = RIGHT_JOYSTICK;
		nMaxDeviation = 138;
		bDeadManXY = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	}

	if(nGenInputs & theApp.m_Tank.nDockedBit) {
		bDeadManXY = true;
		bDeadManSZ = true;
	}


	if((!bDeadManXY) && !(theApp.m_Thread.m_nInPort & 1)){
		theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
	}

	if((!bDeadManSZ) && !(theApp.m_Thread.m_nInPort & 1)){
		theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
	}

	if(nJoystickAction==MAIN_AXES) {
		theApp.m_Motors.GetSurfacePos(&m_CpJoystickSurface);
	}
	if(nOldJoystickTipMode - theApp.m_LastSettings.nJoystickTipMode) {
		switch(theApp.m_LastSettings.nJoystickTipMode) {
		case 0:
		case 1:
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos = theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos;
			break;
		case 2:
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos = theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos += theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos;
			break;
		}
	}
	nOldJoystickTipMode = theApp.m_LastSettings.nJoystickTipMode;

	int M5 = ReadDpram(PMAC_LOOP_COUNT_1 );
	if((m_nJoystickLoopCount - M5) > 1  ) return;
	m_nJoystickLoopCount++;


	for(nWhichAxis=0;nWhichAxis<3;nWhichAxis++) {
		switch(nWhichAxis) {
		case 0:	nTemp=nXTemp;
			break;
		case 1:	nTemp=nYTemp;
			break;
		case 2:	nTemp=nZTemp;
			break;
		};

		if(nJoystickAction==MAIN_AXES) {
			switch(nWhichAxis) {
			case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXLeft];
				break;
			case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYLeft];
				break;
			case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZLeft];
				break;
			};
		} else {
			switch(nWhichAxis) {
			case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtLeft];
				break;
			case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
				break;
			case 2:pGAxis=NULL;
				break;
			};
		};

		if(pGAxis==NULL) {
			goto increment_loop_count;
		}

		nReqPos=0;
		nTemp -= pGAxis->nJoystickZero;
		(pGAxis->bJoystickReverse==TRUE)?(nTemp *= -1):(nTemp);
		if(pGAxis->nJoystickGain==0)	pGAxis->nJoystickGain = 100;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nSpeedBit) {
			nJoystickGain = pGAxis->nJoystickGain / 10;
		} else {
			nJoystickGain = pGAxis->nJoystickGain;
		}

		if((abs(nTemp) > 300) || (abs(nTemp)<=theApp.m_Tank.nJoystickDeadBand) || (nNotSafe == 1)) {

		} else {
			nSign = nTemp;
			nReqPos = ((abs(nTemp)-theApp.m_Tank.nJoystickDeadBand) * (int)(pGAxis->fMaxMoveSpeed * (float)theApp.m_Tank.nTA)) / 128;
			nReqPos = MulDiv(nJoystickGain, nReqPos, 100);
			if(nSign<0) nReqPos*= -1;

			int nMaxDeviation = (int)pGAxis->fMaxMoveSpeed * theApp.m_Tank.nTA * 4;
			int nDesirePos = pGAxis->nDesirePos + nReqPos;
			if((nDesirePos - pGAxis->nPos) > nMaxDeviation) {
				nDesirePos = pGAxis->nPos + nMaxDeviation;
			}
			if((pGAxis->nPos - nDesirePos) > nMaxDeviation) {
				nDesirePos = pGAxis->nPos - nMaxDeviation;
			}
			if(nDesirePos - pGAxis->nDesirePos) {
				pGAxis->nDesirePos = nDesirePos;
				if(pGAxis->nStatus & PMAC_HOMED) {
					if( (pGAxis->nDesirePos + pGAxis->nOffset)  < (int)(pGAxis->fMinKin / pGAxis->fStepSize)) pGAxis->nDesirePos = (int)(pGAxis->fMinKin / pGAxis->fStepSize)-pGAxis->nOffset;
					if( (pGAxis->nDesirePos + pGAxis->nOffset) > (int)(pGAxis->fMaxKin / pGAxis->fStepSize)) pGAxis->nDesirePos = (int)(pGAxis->fMaxKin / pGAxis->fStepSize)-pGAxis->nOffset;
				}
				if((nJoystickAction!=MAIN_AXES) && (theApp.m_LastSettings.nJoystickTipMode == 1)) {	//Goniometric

					m_CpJoystickSurface.Side[PORTSIDE].fXt = (float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos * theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize;
					m_CpJoystickSurface.Side[PORTSIDE].fYt = (float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos * theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize;
					theApp.m_Kinematics.NormalsFromTips(&m_CpJoystickSurface,PORTSIDE);
					theApp.m_Kinematics.HeadFromSurface(&m_CpJoystickSurface,&CpHead,PORTSIDE,TRUE,TRUE);
					theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos = (int)(CpHead.Side0.fX / theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos = (int)(CpHead.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos = (int)(CpHead.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
				}

			}
		};

	}
increment_loop_count:
	for(ii=0;ii<16;ii++) {
		if((theApp.m_Axes[ii].bAvailable) && (theApp.m_Axes[ii].nMotorType==SERVO)) {
			nArray[theApp.m_Axes[ii].nPhysicalAxis-1]=theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset;
		};
	};
	switch(theApp.m_LastSettings.nJoystickTipMode) {
	case 0:
	case 1:
		nArray[theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis-1] += theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos;
		break;
	}
	nPtr = m_nJoystickLoopCount % 2;
	DpramSetMem(PMAC_DPRAM_DATA_START+nPtr*12,12,nArray,true);
	WriteDpram(PMAC_NUMBER_COORDS, m_nJoystickLoopCount);		//M3



}




void CPmac::PLCJoystickServiceRoutingSBT(int nNotSafe)
{
	int	nXTemp,nYTemp,nZTemp,nSTemp,nXtTemp,nYtTemp,nTemp,nSign;
	bool bDeadManSZ = false;
	bool bDeadManXY = false;
	int	nReqPos,nJoystickAction=0;
	int	nWhichAxis,nSide,nMaxDeviation;
	float fJoystickGain;
	AxisData *pGAxis=NULL;
	AxisData *pHAxis=NULL;

	#define	LEFT_JOYSTICK	1
	#define	RIGHT_JOYSTICK	2
	#define	BOTH_JOYSTICK	3
	#define	MAIN_AXES	0
	#define	TIP_AXES	1
	#define	AUX_AXES	2

	ZeroMemory(m_nJoyDev,sizeof m_nJoyDev);
	theApp.m_Motors.EnsureJoystickValuesAreCurrent();

	nXTemp=theApp.m_Motors.m_nJoyValue[0];
	nYTemp=theApp.m_Motors.m_nJoyValue[1];
	nZTemp=theApp.m_Motors.m_nJoyValue[2];
	nXtTemp=theApp.m_Motors.m_nJoyValue[0];
	nYtTemp=theApp.m_Motors.m_nJoyValue[1];
	nSTemp=theApp.m_Motors.m_nJoyValue[3];

	switch(theApp.m_Tank.nJoystickSource) {
	case WIRED_SI10:
	case WIRED_SERIAL:
		switch(theApp.m_nJobNumber) {
		default:
			nSide = BOTH_JOYSTICK;
			if(theApp.m_Motors.m_nJoyValue[4]<15)		nSide = RIGHT_JOYSTICK;
			if(theApp.m_Motors.m_nJoyValue[4] > 220)	nSide = LEFT_JOYSTICK;
			nMaxDeviation = 50;
			bDeadManXY = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
			bDeadManSZ = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
			bDeadManXY = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
			bDeadManSZ = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
			if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
			break;
		case TWI:
			nSide = LEFT_JOYSTICK;
			nJoystickAction = MAIN_AXES;
			if((theApp.m_Motors.m_nJoyValue[4] > 15) && (theApp.m_Motors.m_nJoyValue[4] <220))	{
				nJoystickAction = TIP_AXES;
			}
			if(theApp.m_Motors.m_nJoyValue[4] > 220) {
				nJoystickAction = AUX_AXES;
			};

			nMaxDeviation = 50;
			bDeadManXY = true;
			bDeadManSZ = true;
			break;
		}
		break;
	case WIRELESS_SERIAL:												nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nLeftSideBit)	nSide = LEFT_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nRightSideBit) nSide = RIGHT_JOYSTICK;
		nMaxDeviation = 138;
		bDeadManXY = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	case WIRED_MSC:
		bDeadManXY = true;
		bDeadManSZ = true;
		if(theApp.m_Motors.m_nJoyValue[3]>160) nJoystickAction = TIP_AXES;
		break;
	}

	if(m_nGeneralInput & theApp.m_Tank.nDockedBit) {
		bDeadManXY = true;
		bDeadManSZ = true;
	}


	if((!bDeadManXY) && !(theApp.m_Thread.m_nInPort & 1)){
		theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickNumber];
		m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1]=0;
		m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1]=0;
		m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis-1]=0;
		m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis-1]=0;
	}

	if((!bDeadManSZ) && !(theApp.m_Thread.m_nInPort & 1)){
		theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickNumber];
		m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1]=0;
		m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis-1]=0;
	}


	for(nWhichAxis=0;nWhichAxis<3;nWhichAxis++) {
		pGAxis=NULL;
		switch(nJoystickAction) {
		case MAIN_AXES:
			switch(nWhichAxis) {
			case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXLeft];
				nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickNumber];
				break;
			case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYLeft];
				nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickNumber];
				break;
			case 2:	pGAxis=&theApp.m_Axes[theApp.m_Tank.nZLeft];
				nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickNumber];
				break;
			};
			break;
		case TIP_AXES:
			switch(nWhichAxis) {
			case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtLeft];
				nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickNumber];
				break;
			case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
				nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickNumber];
				break;
			case 2:	
				if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) {
					pGAxis=&theApp.m_Axes[theApp.m_Tank.nRLeft];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickNumber];
				}else{
					pGAxis=NULL;
				}
				break;
			};
			break;
		case AUX_AXES:
			switch(nWhichAxis) {
			case 0:
			case 1: pGAxis=NULL;
				break;
			case 2:	
				pGAxis=NULL;
				if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) {
					pGAxis=&theApp.m_Axes[theApp.m_Tank.nRLeft];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickNumber];
				}
				if(theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable) {
					pGAxis=&theApp.m_Axes[theApp.m_Tank.nRRight];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nRRight].nJoystickNumber];
				}
				break;
			};
			break;
		};

		if(pGAxis) {

			nReqPos=0;
			nTemp -= pGAxis->nJoystickZero;
			(pGAxis->bJoystickReverse==TRUE)?(nTemp *= -1):(nTemp);
			if(pGAxis->nJoystickGain==0)	pGAxis->nJoystickGain = 10;

			fJoystickGain = (float)pGAxis->nJoystickGain;
			switch(theApp.m_nJobNumber) {
			default:
				if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nSpeedBit) {
					fJoystickGain = (float)pGAxis->nJoystickGain / 10;
				}
				break;
			case TWI:
				if(!(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nSpeedBit)) {
					fJoystickGain = (float)pGAxis->nJoystickGain / 10;
				}
				break;
			}
			if(theApp.m_Tank.nJoystickDeadBand < 1) theApp.m_Tank.nJoystickDeadBand = 1;

			if((abs(nTemp) > 300) || (abs(nTemp)<=theApp.m_Tank.nJoystickDeadBand) || (nNotSafe == 1)) {
				nTemp = 0;
				if(abs(nXTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
					if(abs(nYTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
						if(abs(nZTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
							theApp.m_Motors.GetHeadPos(&m_CpJoystickHead);		//I don't know what this is here for
						};
					};
				};
			} else {
				nSign = nTemp;
				nReqPos=(int)((float)((abs(nTemp)-theApp.m_Tank.nJoystickDeadBand))*fJoystickGain);
				if(nSign<0) nReqPos*= -1;

			};

			m_nJoyDev[pGAxis->nPhysicalAxis-1]=nReqPos;

			if(nJoystickAction==MAIN_AXES) {
				WriteDpram(PMAC_JOYSTICK_TIP_MODE, 0);
				theApp.m_Motors.GetHeadPos(&m_CpHead);
				theApp.m_Kinematics.SurfaceFromHead(&m_CpHead, &m_CpSurface,PORTSIDE);
				theApp.m_Kinematics.SurfaceFromHead(&m_CpHead, &m_CpSurface,STARBOARD);
			} else {
				if((theApp.m_LastSettings.nJoystickTipMode==1) && (bDeadManXY == TRUE)){
					theApp.m_Motors.GetHeadPos(&m_CpHead);
					m_CpSurface.Side0.fXt=m_CpHead.Side0.fXt;
					m_CpSurface.Side0.fYt=m_CpHead.Side0.fYt;
					m_CpSurface.Side1.fXt=m_CpHead.Side1.fXt;
					m_CpSurface.Side1.fYt=m_CpHead.Side1.fYt;
					theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, PORTSIDE);
					theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, STARBOARD);
					theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos = (int)(m_CpHead.Side0.fX / theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos = (int)(m_CpHead.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos = (int)(m_CpHead.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos = (int)(m_CpHead.Side1.fX / theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos = (int)(m_CpHead.Side1.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos = (int)(m_CpHead.Side1.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
					WriteDpram(theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset);
					WriteDpram(theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset);
					WriteDpram(theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset);
					WriteDpram(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
					WriteDpram(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
					WriteDpram(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
					WriteDpram(PMAC_JOYSTICK_TIP_MODE, 1);
				}
			}
		}
	}
	DpramSetMem(PMAC_JOY_DEV,12,m_nJoyDev);

}

void CPmac::PLCJoystickServiceRoutine10Axes(int nNotSafe)
{
	int	nXTemp,nYTemp,nZTemp,nSTemp,nXtTemp,nYtTemp,nTemp,nSign;
	int	nReqPos,nJoystickAction=0,nSide;
	float	fJoystickGain;
	int		nWhichAxis,nMaxDeviation;
	bool bDeadManSZ = false;
	bool bDeadManXY = false;
	AxisData *pGAxis=NULL;
	AxisData *pHAxis=NULL;
	float fXBridge;
	int nDesirePos[100];
	int nGenInputs = theApp.m_Motors.GetInputPort();

	#define	LEFT_JOYSTICK	1
	#define	RIGHT_JOYSTICK	2
	#define	BOTH_JOYSTICK	3
	#define	MAIN_AXES	0
	#define	TIP_AXES	1

	ZeroMemory(m_nJoyDev,sizeof m_nJoyDev);

	theApp.m_Motors.EnsureJoystickValuesAreCurrent();

	nXTemp=theApp.m_Motors.m_nJoyValue[0];
	nYTemp=theApp.m_Motors.m_nJoyValue[1];
	nZTemp=theApp.m_Motors.m_nJoyValue[2];
	nXtTemp=theApp.m_Motors.m_nJoyValue[0];
	nYtTemp=theApp.m_Motors.m_nJoyValue[1];
	nSTemp=theApp.m_Motors.m_nJoyValue[3];
	switch(theApp.m_Tank.nJoystickSource) {
	case WIRED_SERIAL:							nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nJoyValue[4]<15)		nSide = RIGHT_JOYSTICK;
		if(theApp.m_Motors.m_nJoyValue[4] > 220)	nSide = LEFT_JOYSTICK;
		nMaxDeviation = 50;
		bDeadManXY = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	case WIRELESS_SERIAL:												nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nLeftSideBit)	nSide = LEFT_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nRightSideBit) nSide = RIGHT_JOYSTICK;
		nMaxDeviation = 138;
		bDeadManXY = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	}

	if(nGenInputs & theApp.m_Tank.nDockedBit) {
		bDeadManXY = true;
		bDeadManSZ = true;
	}


	if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nScannerFixtureBit) {
		theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickNumber];
		theApp.m_Axes[theApp.m_Tank.nRRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nRRight].nJoystickNumber];
	} else {
		if(bDeadManXY == false) {
			theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickNumber];
			theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickNumber];
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickNumber];
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickNumber];
			theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickNumber];
			theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickNumber];
			theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickNumber];
			theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickNumber];
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis-1]=0;
		}
		if(bDeadManSZ == false) {
			theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickNumber];
			theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickNumber];
			theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickNumber];
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1]=0;
			m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis-1]=0;
		}
	}


	for(nWhichAxis=0; nWhichAxis<4; nWhichAxis++) {
		pGAxis=NULL;
		pHAxis=NULL;
		switch(nWhichAxis) {
		case 0:	nTemp=nXTemp;
			break;
		case 1:	nTemp=nYTemp;
			break;
		case 2:	nTemp=nZTemp;
			break;
		case 3:	nTemp=nSTemp;
			break;
		};

		if((nSide == LEFT_JOYSTICK) || (nSide == BOTH_JOYSTICK)) {
			if(nJoystickAction==MAIN_AXES) {
				switch(nWhichAxis) {
				case 0:
					if((nSide==BOTH_JOYSTICK) && (theApp.m_nJobNumber==GKN) ){
						pGAxis=&theApp.m_Axes[theApp.m_Tank.nXBridge];
					}else {
						pGAxis=&theApp.m_Axes[theApp.m_Tank.nXLeft];
						pHAxis=&theApp.m_Axes[theApp.m_Tank.nXRight];
					}
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickNumber];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYLeft];
					pHAxis=&theApp.m_Axes[theApp.m_Tank.nYRight];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickNumber];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZLeft];
					pHAxis=&theApp.m_Axes[theApp.m_Tank.nZRight];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickNumber];
					break;
				case 3:pGAxis=&theApp.m_Axes[theApp.m_Tank.nRLeft];
					pHAxis=NULL;
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickNumber];
					break;
				};
			} else {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtLeft];
					pHAxis=&theApp.m_Axes[theApp.m_Tank.nXtRight];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickNumber];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
					pHAxis=&theApp.m_Axes[theApp.m_Tank.nYtRight];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickNumber];
					break;
				case 2:pGAxis=NULL;
					pHAxis=NULL;
					break;
				case 3:pGAxis=&theApp.m_Axes[theApp.m_Tank.nRLeft];
					pHAxis=NULL;
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickNumber];
					break;
				};
			};
		}
		if(nSide == RIGHT_JOYSTICK) {
			if(nJoystickAction==MAIN_AXES) {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXRight];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickNumber];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYRight];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickNumber];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZRight];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickNumber];
					break;
				case 3:pGAxis=NULL;
					break;
				};
			} else {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtRight];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickNumber];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtRight];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickNumber];
					break;
				case 2:pGAxis=NULL;
					break;
				case 3:pGAxis=&theApp.m_Axes[theApp.m_Tank.nRLeft];
					nTemp = theApp.m_Motors.m_nJoyValue[theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickNumber];
					break;
				};
			};
		}

		if(pGAxis ) {


			nReqPos=0;
			nTemp -= pGAxis->nJoystickZero;
			(pGAxis->bJoystickReverse==TRUE)?(nTemp *= -1):(nTemp);
			if(pGAxis->nJoystickGain==0)	pGAxis->nJoystickGain = 10;
			if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nSpeedBit) {
				fJoystickGain = (float)pGAxis->nJoystickGain / 10.0f;
			} else {
				fJoystickGain = (float)pGAxis->nJoystickGain;
			}

			if(theApp.m_Tank.nJoystickDeadBand < 1) theApp.m_Tank.nJoystickDeadBand = 1;

			if((abs(nTemp) > nMaxDeviation) || (abs(nTemp)<=theApp.m_Tank.nJoystickDeadBand) || (nNotSafe == 1)) {
				nTemp = 0;
				if(abs(nXTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
					if(abs(nYTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
						if(abs(nZTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
							theApp.m_Motors.GetHeadPos(&m_CpJoystickHead);		//I don't know what this is here for
						};
					};
				};
			} else {
				nSign = nTemp;
				nReqPos=(int)((float)((abs(nTemp)-theApp.m_Tank.nJoystickDeadBand))*fJoystickGain);
				if(nSign<0) nReqPos*= -1;

			};


			if(nSide != BOTH_JOYSTICK) {
				theApp.m_Axes[theApp.m_Tank.nXLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nXRight].nPos - theApp.m_Axes[theApp.m_Tank.nXLeft].nPos;
				theApp.m_Axes[theApp.m_Tank.nYLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nYRight].nPos - theApp.m_Axes[theApp.m_Tank.nYLeft].nPos;
				theApp.m_Axes[theApp.m_Tank.nZLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nZRight].nPos - theApp.m_Axes[theApp.m_Tank.nZLeft].nPos;
				theApp.m_Axes[theApp.m_Tank.nXtLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nXtRight].nPos - theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos;
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos - theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos;
			} else {
				if(pHAxis != NULL) {
					m_nJoyDev[pHAxis->nPhysicalAxis-1]=(int)((float)nReqPos * pGAxis->fStepSize / pHAxis->fStepSize);
				}
			}
			m_nJoyDev[pGAxis->nPhysicalAxis-1]=nReqPos;

			if(nJoystickAction==MAIN_AXES) {
				WriteDpram(PMAC_JOYSTICK_TIP_MODE, 0);
				theApp.m_Motors.GetHeadPos(&m_CpHead);
				theApp.m_Kinematics.SurfaceFromHead(&m_CpHead, &m_CpSurface,PORTSIDE);
				theApp.m_Kinematics.SurfaceFromHead(&m_CpHead, &m_CpSurface,STARBOARD);
			} else {
/*10Axes*/		if(theApp.m_LastSettings.nJoystickTipMode==1) {
					if(theApp.m_nJobNumber==GKN){
						theApp.m_Motors.GetHeadPos(&m_CpHead);
						m_CpSurface.Side0.fXt=m_CpHead.Side0.fXt;
						m_CpSurface.Side0.fYt=m_CpHead.Side0.fYt;
						m_CpSurface.Side1.fXt=m_CpHead.Side1.fXt;
						m_CpSurface.Side1.fYt=m_CpHead.Side1.fYt;
						theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, PORTSIDE);
						theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, STARBOARD);

						if(theApp.m_LastSettings.bEnableSide[0] == theApp.m_LastSettings.bEnableSide[1] == 1) {
							fXBridge = ((m_CpHead.Side0.fX + m_CpHead.Side1.fX) / 2.0f) + theApp.m_Kinematics.m_fXOffset[PORTSIDE];
							theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(fXBridge/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
							theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)((m_CpHead.Side0.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
							theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)((m_CpHead.Side1.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
						} else {
							if(theApp.m_LastSettings.bEnableSide[0] == 1) {
								theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(m_CpHead.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
								theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(0.0/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
							}
							if(theApp.m_LastSettings.bEnableSide[1] == 1) {
								theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(m_CpHead.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
								theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(0.0/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
							}
						}
						theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos = (int)(m_CpHead.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos = (int)(m_CpHead.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos = (int)(m_CpHead.Side1.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos = (int)(m_CpHead.Side1.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset;
						DpramSetMem(PMAC_DESIRE_POS,12,nDesirePos);
						WriteDpram(PMAC_JOYSTICK_TIP_MODE, 1);
					}else{
						theApp.m_Motors.GetHeadPos(&m_CpHead);
						m_CpSurface.Side0.fXt=m_CpHead.Side0.fXt;
						m_CpSurface.Side0.fYt=m_CpHead.Side0.fYt;
						m_CpSurface.Side1.fXt=m_CpHead.Side1.fXt;
						m_CpSurface.Side1.fYt=m_CpHead.Side1.fYt;
						theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, PORTSIDE);
						theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, STARBOARD);
						theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos = (int)(m_CpHead.Side0.fX / theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos = (int)(m_CpHead.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos = (int)(m_CpHead.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos = (int)(m_CpHead.Side1.fX / theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos = (int)(m_CpHead.Side1.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos = (int)(m_CpHead.Side1.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset;
						nDesirePos[theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1] = theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset;
						DpramSetMem(PMAC_DESIRE_POS,10,nDesirePos);

						WriteDpram(PMAC_JOYSTICK_TIP_MODE, 1);
					}//GKN
				}
			}
		}
	}
	// try to avoid colliding
	CCoord Cp,CpSurface;
	/*
	theApp.m_Motors.GetHeadPos(&Cp);
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	if(m_bDriveAwayRequired[PORTSIDE] == true) {
		if(CCoord::Length(Cp,m_CpCollisionPoint,PORTSIDE) > 30.0f) m_bDriveAwayRequired[PORTSIDE] = false;
	}
	if(m_bDriveAwayRequired[STARBOARD] == true) {
		if(CCoord::Length(Cp,m_CpCollisionPoint,STARBOARD) > 30.0f) m_bDriveAwayRequired[STARBOARD] = false;
	}
	
	theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
	theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);

	if(theApp.m_nSide0Orientation == 0) {

		if(((theApp.m_Kinematics.m_fWaterPath[PORTSIDE] < (theApp.m_LastSettings.fMinimumWaterPathJog+20.0f)) && (theApp.m_Kinematics.m_fWaterPath[PORTSIDE] > 0.0f)) || (m_bDriveAwayRequired[PORTSIDE]==true)) {
			nTemp = (int)(theApp.m_Kinematics.m_fWaterPath[PORTSIDE] - theApp.m_LastSettings.fMinimumWaterPathJog);
			if(nTemp < 0) nTemp = 0;
			if((nTemp == 0) && (m_bDriveAwayRequired[PORTSIDE] == false)) {
				m_bDriveAwayRequired[PORTSIDE] = true;
				m_CpCollisionPoint = Cp;
			}
			if(m_bDriveAwayRequired[PORTSIDE] == true) nTemp = 0;
			if((Signf(CpSurface.Side[PORTSIDE].fI) == NEGATIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1] > 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1],nTemp,20);
			if((Signf(CpSurface.Side[PORTSIDE].fJ) == NEGATIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1] > 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1],nTemp,20);
			if((Signf(CpSurface.Side[PORTSIDE].fK) == NEGATIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1] > 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1],nTemp,20);
			if((Signf(CpSurface.Side[PORTSIDE].fI) == POSITIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1] < 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1],nTemp,20);
			if((Signf(CpSurface.Side[PORTSIDE].fJ) == POSITIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1] < 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1],nTemp,20);
			if((Signf(CpSurface.Side[PORTSIDE].fK) == POSITIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1] < 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1],nTemp,20);
		}

		if(((theApp.m_Kinematics.m_fWaterPath[STARBOARD] < (theApp.m_LastSettings.fMinimumWaterPathJog+20.0f)) && (theApp.m_Kinematics.m_fWaterPath[STARBOARD] > 0.0f)) || (m_bDriveAwayRequired[STARBOARD]==true)) {
			nTemp = (int)(theApp.m_Kinematics.m_fWaterPath[STARBOARD] - theApp.m_LastSettings.fMinimumWaterPathJog);
			if(nTemp < 0) nTemp = 0;
			if((nTemp == 0) && (m_bDriveAwayRequired[STARBOARD] == false)) {
				m_bDriveAwayRequired[STARBOARD] = true;
				m_CpCollisionPoint = Cp;
			}
			if(m_bDriveAwayRequired[STARBOARD] == true) nTemp = 0;
			if((Signf(CpSurface.Side[STARBOARD].fI) == NEGATIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1] < 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1],nTemp,20);
			if((Signf(CpSurface.Side[STARBOARD].fJ) == NEGATIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1] < 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1],nTemp,20);
			if((Signf(CpSurface.Side[STARBOARD].fK) == NEGATIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1] < 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1],nTemp,20);
			if((Signf(CpSurface.Side[STARBOARD].fI) == POSITIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1] > 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1],nTemp,20);
			if((Signf(CpSurface.Side[STARBOARD].fJ) == POSITIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1] > 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1],nTemp,20);
			if((Signf(CpSurface.Side[STARBOARD].fK) == POSITIVE) && (m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1] > 0)) MulDiv(&m_nJoyDev[theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1],nTemp,20);
		}

	} else {

	}
*/

	DpramSetMem(PMAC_JOY_DEV,12,m_nJoyDev);
}

void CPmac::PLCJoystickServiceRoutine12Axes(int nNotSafe)
{
	int	nXTemp,nYTemp,nZTemp,nSTemp,nXtTemp,nYtTemp,nTemp,nSign;
	int	nReqPos,nJoystickAction=0,nSide;
	int	nJoystickGain,nMaxDeviation;
	bool bDeadManSZ = false;
	bool bDeadManXY = false;
	AxisData *pGAxis=NULL;
	AxisData *pHAxis=NULL;
	float fXBridge;

	#define	LEFT_JOYSTICK	1
	#define	RIGHT_JOYSTICK	2
	#define	BOTH_JOYSTICK	3
	#define	MAIN_AXES	0
	#define	TIP_AXES	1


	theApp.m_Motors.EnsureJoystickValuesAreCurrent();

	nXTemp=theApp.m_Motors.m_nJoyValue[0];
	nYTemp=theApp.m_Motors.m_nJoyValue[1];
	nZTemp=theApp.m_Motors.m_nJoyValue[2];
	nXtTemp=theApp.m_Motors.m_nJoyValue[0];
	nYtTemp=theApp.m_Motors.m_nJoyValue[1];
	nSTemp=theApp.m_Motors.m_nJoyValue[3];
	switch(theApp.m_Tank.nJoystickSource) {
	case WIRED_SERIAL:							nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nJoyValue[4]<15)		nSide = RIGHT_JOYSTICK;
		if(theApp.m_Motors.m_nJoyValue[4] > 220)	nSide = LEFT_JOYSTICK;
		nMaxDeviation = 50;
		bDeadManXY = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	case WIRELESS_SERIAL:												nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nLeftSideBit)	nSide = LEFT_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nRightSideBit) nSide = RIGHT_JOYSTICK;
		nMaxDeviation = 138;
		bDeadManXY = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	}

	if(m_nGeneralInput & theApp.m_Tank.nDockedBit) {
		bDeadManXY = true;
		bDeadManSZ = true;
	}

	if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nScannerFixtureBit) {
		theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
		theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nXBridge].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
	} else {
		if(bDeadManXY == false) {
			theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nXBridge].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];

			WriteDpram(theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1+PMAC_JOY_DEV,0);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1+PMAC_JOY_DEV,0);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis-1+PMAC_JOY_DEV,0);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis-1+PMAC_JOY_DEV,0);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1+PMAC_JOY_DEV,0);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1+PMAC_JOY_DEV,0);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis-1+PMAC_JOY_DEV,0);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis-1+PMAC_JOY_DEV,0);
		
		}
		if(bDeadManSZ == false) {
			theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
			theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
			theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[3];
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1+PMAC_JOY_DEV,0);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1+PMAC_JOY_DEV,0);
			WriteDpram(theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis-1+PMAC_JOY_DEV,0);
		}
	}

	for(int nWhichAxis=0; nWhichAxis<3; nWhichAxis++) {
		pGAxis=NULL;
		pHAxis=NULL;
		switch(nWhichAxis) {
		case 0:	nTemp=nXTemp;
			break;
		case 1:	nTemp=nYTemp;
			break;
		case 2:	nTemp=nZTemp;
			break;
		};

		switch(nSide) {
		case LEFT_JOYSTICK:
			if(nJoystickAction==MAIN_AXES) {
				switch(nWhichAxis) {
				case 0:
					if(theApp.m_LastSettings.bEnableSide[1]) {
						pGAxis=&theApp.m_Axes[theApp.m_Tank.nXLeft];
					} else {
						pGAxis=&theApp.m_Axes[theApp.m_Tank.nXBridge];
					}
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYLeft];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZLeft];
					break;
				};
			} else {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtLeft];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nRLeft];
					break;
				};
			};
			break;
		case RIGHT_JOYSTICK:
			if(nJoystickAction==MAIN_AXES) {
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
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtRight];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtRight];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nRLeft];
					break;
				};
			};
			break;
		case BOTH_JOYSTICK:
			if(nJoystickAction==MAIN_AXES) {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXBridge];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nYRight];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nZRight];
					break;
				};
			} else {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nXtRight];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nYtRight];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nRLeft];
					break;
				};
			};
			break;
		}

		if(pGAxis==NULL) return;


		nReqPos=0;
		nTemp -= pGAxis->nJoystickZero;
		(pGAxis->bJoystickReverse==TRUE)?(nTemp *= -1):(nTemp);
		if(pGAxis->nJoystickGain==0)	pGAxis->nJoystickGain = 100;

		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nSpeedBit) {
			nJoystickGain = pGAxis->nJoystickGain / 10;
		} else {
			nJoystickGain = pGAxis->nJoystickGain;
		}

		if((abs(nTemp) > nMaxDeviation) || (abs(nTemp)<=theApp.m_Tank.nJoystickDeadBand) || (nNotSafe == 1)) {
			nTemp = 0;
			if(abs(nXTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
				if(abs(nYTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
					if(abs(nZTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
						theApp.m_Motors.GetHeadPos(&m_CpJoystickHead);		//I don't know what this is here for
					};
				};
			};
		} else {
			nSign = nTemp;
			nReqPos=((abs(nTemp)-theApp.m_Tank.nJoystickDeadBand))*nJoystickGain;
			if(nSign<0) nReqPos*= -1;

		};


		if(nSide != BOTH_JOYSTICK) {
			theApp.m_Axes[theApp.m_Tank.nXLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nXRight].nPos - theApp.m_Axes[theApp.m_Tank.nXLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nYLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nYRight].nPos - theApp.m_Axes[theApp.m_Tank.nYLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nZLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nZRight].nPos - theApp.m_Axes[theApp.m_Tank.nZLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nXtRight].nPos - theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos - theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos;
		} else {
			if(pHAxis != NULL) {
				WriteDpram(pHAxis->nPhysicalAxis-1+PMAC_JOY_DEV,m_nJoyDev[pHAxis->nPhysicalAxis-1]=nReqPos);
			}
		}
		WriteDpram(pGAxis->nPhysicalAxis-1+PMAC_JOY_DEV,m_nJoyDev[pGAxis->nPhysicalAxis-1]=nReqPos,1);

		if(nJoystickAction==MAIN_AXES) {
			WriteDpram(PMAC_JOYSTICK_TIP_MODE, 0);
			theApp.m_Motors.GetHeadPos(&m_CpHead);
			theApp.m_Kinematics.SurfaceFromHead(&m_CpHead, &m_CpSurface,PORTSIDE);
			theApp.m_Kinematics.SurfaceFromHead(&m_CpHead, &m_CpSurface,STARBOARD);
		} else {
			if(theApp.m_LastSettings.nJoystickTipMode==1) {
				theApp.m_Motors.GetHeadPos(&m_CpHead);
				m_CpSurface.Side0.fXt=m_CpHead.Side0.fXt;
				m_CpSurface.Side0.fYt=m_CpHead.Side0.fYt;
				m_CpSurface.Side1.fXt=m_CpHead.Side1.fXt;
				m_CpSurface.Side1.fYt=m_CpHead.Side1.fYt;
				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, PORTSIDE);
				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, STARBOARD);

				if(theApp.m_LastSettings.bEnableSide[0] == theApp.m_LastSettings.bEnableSide[1] == 1) {
					fXBridge = ((m_CpHead.Side0.fX + m_CpHead.Side1.fX) / 2.0f) + theApp.m_Kinematics.m_fXOffset[PORTSIDE];
					theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(fXBridge/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)((m_CpHead.Side0.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)((m_CpHead.Side1.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
				} else {
					if(theApp.m_LastSettings.bEnableSide[0] == 1) {
						theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(m_CpHead.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(0.0/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					}
					if(theApp.m_LastSettings.bEnableSide[1] == 1) {
						theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(m_CpHead.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(0.0/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					}
				}
				//This ought to be changed to write a block of memory
				theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos = (int)(m_CpHead.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos = (int)(m_CpHead.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos = (int)(m_CpHead.Side1.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos = (int)(m_CpHead.Side1.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
				WriteDpram(theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset);
				WriteDpram(theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset);
				WriteDpram(theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset);
				WriteDpram(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
				WriteDpram(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
				WriteDpram(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
				WriteDpram(theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset);
				WriteDpram(PMAC_JOYSTICK_TIP_MODE, 1);
			}
		}
	}


}


//Unicode


#include "Include\gather.h"

void CPmac::DoAStep(int nAxis, int nDistance, int nTime)
{
/*
	int nn = DeviceDownload(m_dwDevice,NULL,NULL,NULL,"C:\\WindowsProjects\\TimetTanks\\PLCC.txt",NULL,NULL,NULL,TRUE);

	WTG_EX mask;

	mask.ENC_TO_G = GATHER_ENC1;

	nn = DeviceSetQuickGatherEx(m_dwDevice,&mask,TRUE);
	nn = DeviceStartGather(m_dwDevice);

	SendStr(L"I5=3");
	SendStr(L"ENABLE PLCC 30");

	Sleep(2);
	nn = DeviceStopGather(m_dwDevice);
	nn = DeviceGetNumGatherSamples(m_dwDevice);

	char *str;
	str = new char[nn*20];
	nn = DeviceGetGather(m_dwDevice,nn,str,0xffffffff);
	delete str;
	*/
}


int CPmac::DownLoadPolyMoveCoordinates(CPolyCoord *ToolPath, int nDir)
{
	int	ii,jj;
	int	nX0,nY0,nZ0,nXt0,nYt0,nR0;
	int	nOffset;
	CCoord	Cp;
	int nArray[12];
	ZeroMemory(nArray,sizeof nArray);

	switch(theApp.m_Tank.nScannerDescription) {
	case SINGLE_BRIDGE_DUAL_SIDED:
		return DownLoadPolyMoveCoordinatesSingleBridgeDualSided(ToolPath,nDir);
	}


	nOffset=PMAC_DPRAM_DATA_START;

	if(nDir==0) {
		for(ii=0;(ii<ToolPath->m_nCoordL)&&(ii<60);ii++) {
			Cp=ToolPath->m_pCp[ii];
			nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
			if(theApp.m_LastSettings.nManipulatorType[PORTSIDE]==ROTATE_DIF_THETA) nYt0 += nXt0;
			nArray[0]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
			nArray[1]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
			nArray[2]=theApp.m_Kinematics.m_LeadScrew[2].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
			nArray[3]=theApp.m_Kinematics.m_LeadScrew[3].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
			nArray[4]=theApp.m_Kinematics.m_LeadScrew[4].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
			nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
			nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
			nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
			if(theApp.m_LastSettings.nManipulatorType[STARBOARD]==ROTATE_DIF_THETA) nYt0 += nXt0;
			nArray[5]=theApp.m_Kinematics.m_LeadScrew[5].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
			nArray[6]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
			nArray[7]=theApp.m_Kinematics.m_LeadScrew[7].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
			nArray[8]=theApp.m_Kinematics.m_LeadScrew[8].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
			nArray[9]=theApp.m_Kinematics.m_LeadScrew[9].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);

			nR0=(int)(Cp.Side0.fR/theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);
			nArray[10]=theApp.m_Kinematics.m_LeadScrew[10].ActualToMachine(nR0+theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[10]);
			nR0=(int)(Cp.Side1.fR/theApp.m_Axes[theApp.m_Tank.nRRight].fStepSize);
			nArray[11]=theApp.m_Kinematics.m_LeadScrew[11].ActualToMachine(nR0+theApp.m_Axes[theApp.m_Tank.nRRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[11]);
			DpramSetMem(PMAC_DPRAM_DATA_START+ii*12,12,nArray,true);
		};
	} else {
		for(ii=0,jj=ToolPath->m_nCoordL-1;(ii<ToolPath->m_nCoordL)&&(ii<60);ii++,jj--) {
			Cp=ToolPath->m_pCp[jj];
			nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
			if(theApp.m_LastSettings.nManipulatorType[PORTSIDE]==ROTATE_DIF_THETA) nYt0 += nXt0;
			nArray[0]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
			nArray[1]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
			nArray[2]=theApp.m_Kinematics.m_LeadScrew[2].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
			nArray[3]=theApp.m_Kinematics.m_LeadScrew[3].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
			nArray[4]=theApp.m_Kinematics.m_LeadScrew[4].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
			nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
			nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
			nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
			if(theApp.m_LastSettings.nManipulatorType[STARBOARD]==ROTATE_DIF_THETA) nYt0 += nXt0;
			nArray[5]=theApp.m_Kinematics.m_LeadScrew[5].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
			nArray[6]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
			nArray[7]=theApp.m_Kinematics.m_LeadScrew[7].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
			nArray[8]=theApp.m_Kinematics.m_LeadScrew[8].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
			nArray[9]=theApp.m_Kinematics.m_LeadScrew[9].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);

			nR0=(int)(Cp.Side0.fR/theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);
			nArray[10]=theApp.m_Kinematics.m_LeadScrew[10].ActualToMachine(nR0+theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[10]);
			nR0=(int)(Cp.Side1.fR/theApp.m_Axes[theApp.m_Tank.nRRight].fStepSize);
			nArray[11]=theApp.m_Kinematics.m_LeadScrew[11].ActualToMachine(nR0+theApp.m_Axes[theApp.m_Tank.nRRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[11]);

			DpramSetMem(PMAC_DPRAM_DATA_START+ii*12,12,nArray,true);
		};
	};
	ZeroMemory(nArray,sizeof nArray);
	DpramSetMem(PMAC_DPRAM_DATA_START+ii*12,12,nArray,true);

	WriteDpram(PMAC_LOOP_COUNT_1, 0);		//M5
	WriteDpram(PMAC_NUMBER_COORDS, ii);		//M3
	m_nWritePtr=ii%60;
	return ii;
}

int CPmac::DownLoadNextPolyMoveCoordinate(int nNextCoordinate,CPolyCoord *ToolPath, int nDir)
{
	switch(theApp.m_Tank.nScannerDescription) {
	case SINGLE_BRIDGE_DUAL_SIDED:
		return DownLoadNextPolyMoveCoordinateSingleBridgeDualSided(nNextCoordinate,ToolPath,nDir);
	}

	int	ii;
	int	nX0,nY0,nZ0,nXt0,nYt0;
	int	nOffset=PMAC_DPRAM_DATA_START;
	CCoord	Cp;
	int nLoopCount=GetLoopCounter();

	if(nNextCoordinate>=ToolPath->m_nCoordL) return nNextCoordinate;
	if((nNextCoordinate-nLoopCount) >= 58) return nNextCoordinate;

	int nArray[12];
	ZeroMemory(nArray,sizeof nArray);


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
		if(theApp.m_LastSettings.nManipulatorType[PORTSIDE]==ROTATE_DIF_THETA) nYt0 += nXt0;
		nArray[0]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
		nArray[1]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
		nArray[2]=theApp.m_Kinematics.m_LeadScrew[2].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
		nArray[3]=theApp.m_Kinematics.m_LeadScrew[3].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
		nArray[4]=theApp.m_Kinematics.m_LeadScrew[4].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
		nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
		nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
		nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
		if(theApp.m_LastSettings.nManipulatorType[STARBOARD]==ROTATE_DIF_THETA) nYt0 += nXt0;
		nArray[5]=theApp.m_Kinematics.m_LeadScrew[5].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
		nArray[6]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
		nArray[7]=theApp.m_Kinematics.m_LeadScrew[7].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
		nArray[8]=theApp.m_Kinematics.m_LeadScrew[8].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
		nArray[9]=theApp.m_Kinematics.m_LeadScrew[9].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);
	} else {
		Cp=ToolPath->m_pCp[ToolPath->m_nCoordL-nNextCoordinate-1];
		nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
		nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
		nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
		nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
		nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
		if(theApp.m_LastSettings.nManipulatorType[PORTSIDE]==ROTATE_DIF_THETA) nYt0 += nXt0;
		nArray[0]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
		nArray[1]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
		nArray[2]=theApp.m_Kinematics.m_LeadScrew[2].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
		nArray[3]=theApp.m_Kinematics.m_LeadScrew[3].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
		nArray[4]=theApp.m_Kinematics.m_LeadScrew[4].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
		nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
		nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
		nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
		if(theApp.m_LastSettings.nManipulatorType[STARBOARD]==ROTATE_DIF_THETA) nYt0 += nXt0;
		nArray[5]=theApp.m_Kinematics.m_LeadScrew[5].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
		nArray[6]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
		nArray[7]=theApp.m_Kinematics.m_LeadScrew[7].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
		nArray[8]=theApp.m_Kinematics.m_LeadScrew[8].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
		nArray[9]=theApp.m_Kinematics.m_LeadScrew[9].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);
	};
	DpramSetMem(PMAC_DPRAM_DATA_START+ii*12,12,nArray,true);
	nNextCoordinate++;
	WriteDpram(PMAC_NUMBER_COORDS, nNextCoordinate,true);		//M3

	return nNextCoordinate;
}



void CPmac::StartPolyMoveWithLookAhead(int nn)
{
	int	nTm=2;		//2
	int	nTs=5;		//5
	int	nTa=1;		//1
	CString Buff;


	WriteDpram(PMAC_JOYSTICK_TIP_MODE, 0);		//M3 //1
	WriteDpram(PMAC_FEEDFORWARD, theApp.m_Tank.nTenthAxisFeedForward);		//M3
	theApp.m_Tank.nTrackingTenthAxis &=1;
	Buff.Format(_T("M168=%d"),theApp.m_Tank.nTrackingTenthAxis);
	SendStr(Buff);

	WriteDpram(PMAC_TM, nTm);
	WriteDpram(PMAC_TS, nTs);
	WriteDpram(PMAC_TA, nTa);

//	WriteDpram(PMAC_TA,theApp.m_Tank.nTA);
//	WriteDpram(PMAC_TS,theApp.m_Tank.nTS);

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(m_nScanSide) {
		case 0:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B35 R");
			break;
		case 1:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B36 R");
			break;
		case 2:
			Buff.Format(_T("M1900=%d"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset);
			SendStr(Buff);
			Buff.Format(_T("M1901=%d"),theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset);
			SendStr(Buff);
			Buff.Format(_T("M1902=%d"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset);
			SendStr(Buff);
			Buff.Format(_T("M1903=%d"),theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset);
			SendStr(Buff);
			SendStr(L"&1 B30 R &2 B33 R");
			break;
		}
		break;
	case DUAL_SIDED_7_AXES:
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TT_ROLLERS:
		switch(m_nScanSide) {
		case 0:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B35 R");
			break;
		case 1:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B35 R");
			break;
		case 2:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B35 R");
			break;
		}
		break;
	case DUAL_SIDED_SQUIRTER_2:
		switch(m_nScanSide) {
		case 0:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B35 R");
			break;
		case 1:
			SetFeedRate(theApp.m_LastSettings.fFeedRate);
			SendStr(L"&1 B36 R");
			break;
		case 2:
//			SendStr(_T("&1"));
//			Buff.Format(_T("TA%d"),nTa);	SendStr(Buff);
//			Buff.Format(_T("TM%d"),nTm);	SendStr(Buff);
//			Buff.Format(_T("TS%d"),nTs);	SendStr(Buff);
//			SendStr(_T("&2"));
//			Buff.Format(_T("TA%d"),nTa);	SendStr(Buff);
//			Buff.Format(_T("TM%d"),nTm);	SendStr(Buff);
//			Buff.Format(_T("TS%d"),nTs);	SendStr(Buff);
//			SetFeedRate(0);
//			Sleep(100);
//			SendStr(L"&1 B40 R"); //SendStr(L"&1 B35 R &2 B36 R");
//			SendStr(L"&2 B41 R"); //SendStr(L"&1 B35 R &2 B36 R");
			Sleep(500);
			SendStr(L"&1 B35 R");
			SendStr(L"&2 B36 R");
			break;
		}
		break;
	}

}


int CPmac::GetLoopCounter()
{
	int	nn=ReadDpram(PMAC_LOOP_COUNT_1);
	return nn;

}





void CPmac::DownloadPIDForManipulators()
{
	AxisData* pAxis;

	return;
	pAxis = &theApp.m_Axes[theApp.m_Tank.nXtLeft];
	SetVariable(_T("I"),pAxis->nPhysicalAxis*100+22,pAxis->fMaxMoveSpeed);
	SetVariable(_T("I"),pAxis->nPhysicalAxis*100+16,pAxis->fMaxMoveSpeed);
	SetVariable(_T("I"),pAxis->nPhysicalAxis*100+23,pAxis->fHomeSpeed);
	SetVariable(_T("I"),pAxis->nPhysicalAxis*100+19,pAxis->fAcceleration);
	SetVariable(_T("I"),pAxis->nPhysicalAxis*100+17,pAxis->fAcceleration);
	SetVariable(_T("I"),pAxis->nPhysicalAxis*100+26,pAxis->nLimitSwitchOffset);
	SetVariable(_T("I"),pAxis->nPhysicalAxis*100+28,pAxis->nPositionError*16);



}

void CPmac::HomeAxisWithoutMoving(int nWhichAxis)
{
	CString	 Buff;

	if(nWhichAxis < 20) {
		Buff.Format(_T("#%dHMZ"),nWhichAxis);
		SendStr(Buff);
	} else {
		Buff.Format(_T("#%do0"),nWhichAxis);
		SendStr(Buff);
		Buff.Format(_T("HMZ"));
		SendStr(Buff);
	}
}



void CPmac::TuningMove()
{
	int	ii,nMoveTime;
	CString Buff;
	int nArray[12];
	int nPhysicalAxis = m_nTunningAxis + 1;

	nMoveTime=0;
	for(ii=0;ii<12;ii++) {
		if((theApp.m_Axes[ii].bAvailable) && (theApp.m_Axes[ii].nMotorType==SERVO)) {
			nArray[theApp.m_Axes[ii].nPhysicalAxis-1] = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset;

			if(theApp.m_Axes[ii].nPhysicalAxis == nPhysicalAxis) {
				theApp.m_Axes[ii].nDesirePos = theApp.m_Axes[ii].nPos + m_nTunningDistance;
				nMoveTime=CalcMoveTime(&theApp.m_Axes[ii],labs(theApp.m_Axes[ii].nDesirePos-theApp.m_Axes[ii].nPos));
				nArray[theApp.m_Axes[ii].nPhysicalAxis-1] = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset;
			} else {
				nArray[theApp.m_Axes[ii].nPhysicalAxis-1] = theApp.m_Axes[ii].nPos+theApp.m_Axes[ii].nOffset;
			}
		};
	};
	DpramSetMem(PMAC_DESIRE_POS,12,nArray);

	WriteDpram(PMAC_MOVE_FLAG_1,1);
	WriteDpram(PMAC_TM,nMoveTime);
	WriteDpram(PMAC_TA,theApp.m_Tank.nTA);
	WriteDpram(PMAC_TS,theApp.m_Tank.nTS);
	Buff.Format(_T("M2=%d"),m_nTunningAxis+1);
	SendStr(Buff);

	theApp.m_bCollectTunning=TRUE;
	SetFeedRate(theApp.m_LastSettings.fFeedRate);
	SendStr(L"&1 B50 R");


}

void CPmac::MoveToHighGain()
{
	int	ii,nMax,nTemp;
	CString Buff;

	nMax=0;
	for(ii=0;ii<10;ii++) {
		if((theApp.m_Axes[ii].bAvailable) && (theApp.m_Axes[ii].nMotorType==SERVO)) {
			WriteDpram(theApp.m_Axes[ii].nPhysicalAxis-1+PMAC_DESIRE_POS,theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset);
			nTemp=CalcMoveTime(&theApp.m_Axes[ii],labs(theApp.m_Axes[ii].nDesirePos-theApp.m_Axes[ii].nPos));
			if(nTemp>nMax) nMax=nTemp;
		};
	};

	WriteDpram(PMAC_TM,nMax);
	WriteDpram(PMAC_TA,theApp.m_Tank.nTA);
	WriteDpram(PMAC_TS,theApp.m_Tank.nTS);
	Buff.Format(_T("M2=%d"),m_nTunningAxis);
	SendStr(Buff);

	SetFeedRate(theApp.m_LastSettings.fFeedRate);
	SendStr(L"&1 B51 R");


}


void CPmac::SetScanSpeed(float fReqSpeed)
{
	switch(theApp.m_Tank.nScannerDescription) {
	case SINGLE_BRIDGE_DUAL_SIDED:
		SetScanSpeedSingleBridgeDualSided(fReqSpeed);
		return;
	}

	float fRate;
	AxisData *pnAxis[6] = { &theApp.m_Axes[theApp.m_Tank.nXLeft], &theApp.m_Axes[theApp.m_Tank.nYLeft], &theApp.m_Axes[theApp.m_Tank.nZLeft],
							  &theApp.m_Axes[theApp.m_Tank.nXRight], &theApp.m_Axes[theApp.m_Tank.nYRight], &theApp.m_Axes[theApp.m_Tank.nZRight] };

	for(int nAxis=0;nAxis<6;nAxis++) {
		if(fReqSpeed < ((pnAxis[nAxis]->fMaxMoveSpeed * 1000.0f) * pnAxis[nAxis]->fStepSize)) {
			fRate = (fReqSpeed / pnAxis[nAxis]->fStepSize) / 1000.0f;
		} else {
			fRate = pnAxis[nAxis]->fMaxMoveSpeed;
		}

		if(pnAxis[nAxis]->bAvailable==TRUE) {
			SetVariable(_T("I"),pnAxis[nAxis]->nPhysicalAxis*100+16,fRate);
			SetVariable(_T("I"),pnAxis[nAxis]->nPhysicalAxis*100+2016,fRate);
		}
	}
	m_fMaxStopTime = 0.0f;
}


void CPmac::SetScanSpeedSingleBridgeDualSided(float fReqSpeed)
{
	float fRate;
	AxisData *pnAxis[7] = { &theApp.m_Axes[theApp.m_Tank.nXBridge], &theApp.m_Axes[theApp.m_Tank.nXLeft], &theApp.m_Axes[theApp.m_Tank.nYLeft], &theApp.m_Axes[theApp.m_Tank.nZLeft],
							  &theApp.m_Axes[theApp.m_Tank.nXRight], &theApp.m_Axes[theApp.m_Tank.nYRight], &theApp.m_Axes[theApp.m_Tank.nZRight] };

	for(int nAxis=0;nAxis<7;nAxis++) {
		if(fReqSpeed < ((pnAxis[nAxis]->fMaxMoveSpeed * 1000.0f) * pnAxis[nAxis]->fStepSize)) {
			fRate = (fReqSpeed / pnAxis[nAxis]->fStepSize) / 1000.0f;
		} else {
			fRate = pnAxis[nAxis]->fMaxMoveSpeed;
		}
		SetVariable(_T("I"),pnAxis[nAxis]->nPhysicalAxis*100+16,fRate);
		SetVariable(_T("I"),pnAxis[nAxis]->nPhysicalAxis*100+2016,fRate);
	}
	m_fMaxStopTime = 0.0f;
}


void CPmac::ResetAllMaxSpeeds()
{
	int nNumberAxes = GetNumberAxes();

	for(int nAxis=0;nAxis<nNumberAxes;nAxis++) {
		if(theApp.m_Axes[nAxis].bAvailable==TRUE) {
			SetVariable(_T("I"),theApp.m_Axes[nAxis].nPhysicalAxis*100+22,theApp.m_Axes[nAxis].fMaxJogSpeed);
			SetVariable(_T("I"),theApp.m_Axes[nAxis].nPhysicalAxis*100+16,theApp.m_Axes[nAxis].fMaxMoveSpeed);
			SetVariable(_T("I"),theApp.m_Axes[nAxis].nPhysicalAxis*100+2022,theApp.m_Axes[nAxis].fMaxJogSpeed);
			SetVariable(_T("I"),theApp.m_Axes[nAxis].nPhysicalAxis*100+2016,theApp.m_Axes[nAxis].fMaxMoveSpeed);
		}
	}
}


bool CPmac::ReportAirStatus(int nMask)
{
	bool bFlag;


	(m_nGeneralOutput & nMask) ? bFlag = TRUE : bFlag = FALSE;

	return bFlag;

}

void CPmac::AirStart(int nMask)
{

	m_nGeneralOutput |= nMask;
	WriteDpram(PMAC_GENERAL_OUTPUTS,m_nGeneralOutput);

}

void CPmac::AirStop(int nMask)
{

	m_nGeneralOutput &= ~nMask;
	WriteDpram(PMAC_GENERAL_OUTPUTS,m_nGeneralOutput);


}


int CPmac::GetNumberAxes()
{
	int nNumberAxes=0;

	switch(theApp.m_Tank.nScannerDescription) {
	case SINGLE_BRIDGE_DUAL_SIDED:
	case SINGLE_BRIDGE_TANK_WITH_R:
		nNumberAxes = 12;
		break;
	case SINGLE_BRIDGE_TT_ROLLERS:
		nNumberAxes = 7;
		break;
	default:
		if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable==TRUE) nNumberAxes++;
		break;
	}
	switch(theApp.m_nJobNumber) {
	case GOODRICH_MACHINE_4:
		nNumberAxes = 12;
		break;
	}

	return nNumberAxes;
}

int CPmac::DownLoadPolyMoveCoordinatesSingleBridgeDualSided(CPolyCoord *ToolPath, int nDir)
{
	int	ii,jj;
	int	nX0,nY0,nZ0,nXt0,nYt0;
	int nX1,nY1,nZ1,nXt1,nYt1,nXB;
	float fXBridge;
	int	nOffset;
	CCoord	Cp;
	int nArray[12];
	ZeroMemory(nArray,sizeof nArray);

	nOffset=PMAC_DPRAM_DATA_START;

	if(nDir==0) {
		for(ii=0;(ii<ToolPath->m_nCoordL)&&(ii<60);ii++) {
			Cp=ToolPath->m_pCp[ii];
			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:
				nX0=(int)(-theApp.m_Kinematics.m_fXOffset[PORTSIDE]/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
				nXB=(int)((Cp.Side0.fX+theApp.m_Kinematics.m_fXOffset[PORTSIDE])/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
				nArray[0]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
				nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
				break;
			case 1:
				nX1=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
				nArray[5]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
				nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nX1+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
				break;
			case 2:
				if((theApp.m_LastSettings.nManipulatorType[0] == INDEX_FINGER) || (theApp.m_LastSettings.nManipulatorType[1] == INDEX_FINGER)) {
					fXBridge = (((Cp.Side0.fX + Cp.Side1.fX) / 2.0f) + (theApp.m_Kinematics.m_fXOffset[PORTSIDE]/3.0f));
				} else {
					fXBridge = (((Cp.Side0.fX + Cp.Side1.fX) / 2.0f) + theApp.m_Kinematics.m_fXOffset[PORTSIDE]);
				}
				nXB=(int)(fXBridge/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
				nX0=(int)((Cp.Side0.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
				nX1=(int)((Cp.Side1.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
				nArray[0]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
				nArray[5]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(nX1+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
				nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
				break;
			}
			nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
			nArray[1]=theApp.m_Kinematics.m_LeadScrew[2].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
			nArray[2]=theApp.m_Kinematics.m_LeadScrew[3].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
			nArray[3]=theApp.m_Kinematics.m_LeadScrew[4].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
			nArray[4]=theApp.m_Kinematics.m_LeadScrew[5].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
			nY1=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			nZ1=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			nXt1=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
			nYt1=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
			nArray[6]=theApp.m_Kinematics.m_LeadScrew[7].ActualToMachine(nY1+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
			nArray[7]=theApp.m_Kinematics.m_LeadScrew[8].ActualToMachine(nZ1+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
			nArray[8]=theApp.m_Kinematics.m_LeadScrew[9].ActualToMachine(nXt1+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);
			nArray[9]=theApp.m_Kinematics.m_LeadScrew[10].ActualToMachine(nYt1+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[10]);
			DpramSetMem(PMAC_DPRAM_DATA_START+ii*12,12,nArray,true);
		};
	} else {
		for(ii=0,jj=ToolPath->m_nCoordL-1;(ii<ToolPath->m_nCoordL)&&(ii<60);ii++,jj--) {
			Cp=ToolPath->m_pCp[jj];
			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:
				nX0=(int)(-theApp.m_Kinematics.m_fXOffset[PORTSIDE]/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
				nXB=(int)((Cp.Side0.fX+theApp.m_Kinematics.m_fXOffset[PORTSIDE])/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
				nArray[0]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
				nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
				break;
			case 1:
				nX1=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
				nArray[5]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
				nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nX1+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
				break;
			case 2:
				if((theApp.m_LastSettings.nManipulatorType[0] == INDEX_FINGER) || (theApp.m_LastSettings.nManipulatorType[1] == INDEX_FINGER)) {
					fXBridge = (((Cp.Side0.fX + Cp.Side1.fX) / 2.0f) + (theApp.m_Kinematics.m_fXOffset[PORTSIDE]/3.0f));
				} else {
					fXBridge = (((Cp.Side0.fX + Cp.Side1.fX) / 2.0f) + theApp.m_Kinematics.m_fXOffset[PORTSIDE]);
				}
				nXB=(int)(fXBridge/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
				nX0=(int)((Cp.Side0.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
				nX1=(int)((Cp.Side1.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
				nArray[0]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
				nArray[5]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(nX1+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
				nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
				break;
			}
			nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
			nArray[1]=theApp.m_Kinematics.m_LeadScrew[2].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
			nArray[2]=theApp.m_Kinematics.m_LeadScrew[3].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
			nArray[3]=theApp.m_Kinematics.m_LeadScrew[4].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
			nArray[4]=theApp.m_Kinematics.m_LeadScrew[5].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
			nY1=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			nZ1=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			nXt1=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
			nYt1=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
			nArray[6]=theApp.m_Kinematics.m_LeadScrew[7].ActualToMachine(nY1+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
			nArray[7]=theApp.m_Kinematics.m_LeadScrew[8].ActualToMachine(nZ1+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
			nArray[8]=theApp.m_Kinematics.m_LeadScrew[9].ActualToMachine(nXt1+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);
			nArray[9]=theApp.m_Kinematics.m_LeadScrew[10].ActualToMachine(nYt1+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[10]);
			DpramSetMem(PMAC_DPRAM_DATA_START+ii*12,12,nArray,true);
		};
	};

	WriteDpram(PMAC_LOOP_COUNT_1, 0);		//M5
	WriteDpram(PMAC_NUMBER_COORDS, ii);		//M3
	m_nWritePtr=ii%60;
	return ii;
}

int CPmac::DownLoadNextPolyMoveCoordinateSingleBridgeDualSided(int nNextCoordinate,CPolyCoord *ToolPath, int nDir)
{
	int	ii;
	int	nX0,nY0,nZ0,nXt0,nYt0,nXB;
	int	nX1,nY1,nZ1,nXt1,nYt1;
	float fXBridge;
	int	nOffset=PMAC_DPRAM_DATA_START;
	CCoord	Cp;

	int nLoopCount=GetLoopCounter();

	if(nNextCoordinate>=ToolPath->m_nCoordL) return nNextCoordinate;
	if((nNextCoordinate-nLoopCount) >= 58) return nNextCoordinate;

	int nArray[12];
	ZeroMemory(nArray,sizeof nArray);


	ii=m_nWritePtr;
	m_nWritePtr++;
	m_nWritePtr=m_nWritePtr%60;

	if(nDir==0) {
		Cp=ToolPath->m_pCp[nNextCoordinate];
		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0:
			nX0=(int)(-theApp.m_Kinematics.m_fXOffset[PORTSIDE]/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			nXB=(int)((Cp.Side0.fX+theApp.m_Kinematics.m_fXOffset[PORTSIDE])/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
			//nArray[0]=nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset;
			//nArray[10]=nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset;
			nArray[0]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
			nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
			break;
		case 1:
			nX1=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
			//nArray[5]=0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset;
			//nArray[10]=nX1+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset;
			nArray[5]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
			nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nX1+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);			
			break;
		case 2:
			if((theApp.m_LastSettings.nManipulatorType[0] == INDEX_FINGER) || (theApp.m_LastSettings.nManipulatorType[1] == INDEX_FINGER)) {
				fXBridge = (((Cp.Side0.fX + Cp.Side1.fX) / 2.0f) + (theApp.m_Kinematics.m_fXOffset[PORTSIDE]/3.0f));
			} else {
				fXBridge = (((Cp.Side0.fX + Cp.Side1.fX) / 2.0f) + theApp.m_Kinematics.m_fXOffset[PORTSIDE]);
			}
			nXB=(int)(fXBridge/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
			nX0=(int)((Cp.Side0.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			nX1=(int)((Cp.Side1.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
		//	nArray[0]=nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset;
		//	nArray[5]=nX1+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset;
		//	nArray[10]=nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset;
			nArray[0]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
			nArray[5]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(nX1+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
			nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
			break;
		}
		nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
		nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
		nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
		nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
	//	nArray[1]=nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset;
	//	nArray[2]=nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset;
	//	nArray[3]=nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset;
	//	nArray[4]=nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset;
		nArray[1]=theApp.m_Kinematics.m_LeadScrew[2].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
		nArray[2]=theApp.m_Kinematics.m_LeadScrew[3].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
		nArray[3]=theApp.m_Kinematics.m_LeadScrew[4].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
		nArray[4]=theApp.m_Kinematics.m_LeadScrew[5].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
		nY1=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
		nZ1=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		nXt1=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		nYt1=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
	//	nArray[6]=nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset;
	//	nArray[7]=nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset;
	//	nArray[8]=nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset;
	//	nArray[9]=nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset;
		nArray[6]=theApp.m_Kinematics.m_LeadScrew[7].ActualToMachine(nY1+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
		nArray[7]=theApp.m_Kinematics.m_LeadScrew[8].ActualToMachine(nZ1+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
		nArray[8]=theApp.m_Kinematics.m_LeadScrew[9].ActualToMachine(nXt1+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);
		nArray[9]=theApp.m_Kinematics.m_LeadScrew[10].ActualToMachine(nYt1+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[10]);	
	} else {
		Cp=ToolPath->m_pCp[ToolPath->m_nCoordL-nNextCoordinate-1];
		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0:
			nX0=(int)(-theApp.m_Kinematics.m_fXOffset[PORTSIDE]/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			nXB=(int)((Cp.Side0.fX+theApp.m_Kinematics.m_fXOffset[PORTSIDE])/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
		//	nArray[0]=nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset;
		//	nArray[10]=nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset;
			nArray[0]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
			nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
			break;
		case 1:
			nX1=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
		//	nArray[5]=0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset;
		//	nArray[10]=nX1+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset;
			nArray[5]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
			nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nX1+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
			break;
		case 2:
			if((theApp.m_LastSettings.nManipulatorType[0] == INDEX_FINGER) || (theApp.m_LastSettings.nManipulatorType[1] == INDEX_FINGER)) {
				fXBridge = (((Cp.Side0.fX + Cp.Side1.fX) / 2.0f) + (theApp.m_Kinematics.m_fXOffset[PORTSIDE]/3.0f));
			} else {
				fXBridge = (((Cp.Side0.fX + Cp.Side1.fX) / 2.0f) + theApp.m_Kinematics.m_fXOffset[PORTSIDE]);
			}
			nXB=(int)(fXBridge/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
			nX0=(int)((Cp.Side0.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			nX1=(int)((Cp.Side1.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
		//	nArray[0]=nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset;
		//	nArray[5]=nX1+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset;
		//	nArray[10]=nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset;
			nArray[0]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
			nArray[5]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(nX1+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
			nArray[10]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nXB+theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
			break;
		}
		nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
		nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
		nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
		nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
	//	nArray[1]=nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset;
	//	nArray[2]=nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset;
	//	nArray[3]=nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset;
	//	nArray[4]=nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset;
		nArray[1]=theApp.m_Kinematics.m_LeadScrew[2].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
		nArray[2]=theApp.m_Kinematics.m_LeadScrew[3].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
		nArray[3]=theApp.m_Kinematics.m_LeadScrew[4].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
		nArray[4]=theApp.m_Kinematics.m_LeadScrew[5].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
		nY1=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
		nZ1=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		nXt1=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		nYt1=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
//		nArray[6]=nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset;
//		nArray[7]=nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset;
//		nArray[8]=nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset;
//		nArray[9]=nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset;
		nArray[6]=theApp.m_Kinematics.m_LeadScrew[7].ActualToMachine(nY1+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
		nArray[7]=theApp.m_Kinematics.m_LeadScrew[8].ActualToMachine(nZ1+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
		nArray[8]=theApp.m_Kinematics.m_LeadScrew[9].ActualToMachine(nXt1+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);
		nArray[9]=theApp.m_Kinematics.m_LeadScrew[10].ActualToMachine(nYt1+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[10]);
	};
	DpramSetMem(PMAC_DPRAM_DATA_START+ii*12,12,nArray,true);
	nNextCoordinate++;
	WriteDpram(PMAC_NUMBER_COORDS, nNextCoordinate,true);		//M3

	return nNextCoordinate;
}

void CPmac::GetTurntablePos(COORDDATA *pCp)
{
	pCp->Side0.fR = (float)theApp.m_Axes[theApp.m_Tank.nRLeft].nPos * theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;
}

void CPmac::GetTurntableTruePos(int nPos)
{
	nPos = theApp.m_Axes[theApp.m_Tank.nRLeft].nPos + theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter * theApp.m_Axes[theApp.m_Tank.nRLeft].nStepsPerRev;
}


void CPmac::StartContinuousRotate(int nAxis,float fRotateSpeed, int nCW_CCW)
{
	CString Buff;
	float fSpeed;

	fSpeed = fRotateSpeed / 1000.0f / theApp.m_Axes[nAxis].fStepSize;
//	if(theApp.m_nJobNumber == GOODRICH_MACHINE_4) {
//		fSpeed /= 16.0f;
//	}

	Buff.Format(_T("I%d=%.02f"),theApp.m_Axes[nAxis].nPhysicalAxis*100+22,fSpeed);
	SendStr(Buff);

	if(nCW_CCW == 0) {
		Buff.Format(_T("#%dj+"),theApp.m_Axes[nAxis].nPhysicalAxis);
	} else {
		Buff.Format(_T("#%dj-"),theApp.m_Axes[nAxis].nPhysicalAxis);
	}
	SendStr(Buff);

}


void CPmac::StopContinuousRotate(int nAxis)
{
	CString Buff;

	Buff.Format(_T("#%dj/"),theApp.m_Axes[nAxis].nPhysicalAxis);
	SendStr(Buff);

}

bool CPmac::IsRotateRunning()
{
	if(abs(theApp.m_Axes[theApp.m_Tank.nRLeft].nCurrentSpeed) > 30) return true;
	return false;
}

void CPmac::StartXNumberOfRevs(float fNumber,float fSpeed)
{
	int nDesirePos;
	CString Buff;
	float fEncPerMs;

	fEncPerMs = fSpeed / 1000.0f / theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;

	Buff.Format(_T("I%d=%.02f"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis*100+22,fEncPerMs);
	SendStr(Buff);

	nDesirePos = (int)(fNumber * (float)theApp.m_Axes[theApp.m_Tank.nRLeft].nStepsPerRev);
	Buff.Format(_T("#%dj:%d"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis,nDesirePos);
	SendStr(Buff);
}


int CPmac::GetAxisEncoderPos(int nAxis)
{
	int	nPosition[1024];

	DpramGetMem(0,12,(PVOID)nPosition);

	return nPosition[nAxis-1];
}

void CPmac::JogTo(int nAxis, int nPos)
{
	CString Buff;

	Buff.Format(_T("I%d=I%d"),nAxis*100+22,nAxis*100+16);
	SendStr(Buff);

	Buff.Format(_T("#%dJ=%d"),nAxis,nPos);
	SendStr(Buff);
}


void CPmac::RelativeMove(AxisData* pAxis, int nRelMove)
{
	CString Buff;

	SuspendMotors();

	Buff.Format(_T("I%d=%.02f"),pAxis->nPhysicalAxis*100+22,pAxis->fMaxMoveSpeed);
	SendStr(Buff);

	Buff.Format(_T("#%dJ:%d"),pAxis->nPhysicalAxis,nRelMove);
	SendStr(Buff);
}

void CPmac::RelativeMove(AxisData* pAxis, float fRelMove)
{
	CString Buff;

	Buff.Format(_T("I%d=%.02f"),pAxis->nPhysicalAxis*100+22,pAxis->fMaxMoveSpeed);
	SendStr(Buff);

	Buff.Format(_T("#%dJ:%d"),pAxis->nPhysicalAxis,(int)(fRelMove / pAxis->fStepSize));
	SendStr(Buff);
}


bool CPmac::ReportPumpStatus(int nWhichAxis)
{
	CString Buff;
	int	status;
	int nMask;

	if(m_bDriverOpen) {


		switch(theApp.m_Axes[nWhichAxis].nMotorType) {
		case 0:
			break;
		case 1:
			switch(m_nPmacType) {
			case PMAC1:	nMask = 0x4000;
				swscanf_s(Buff,_T("%x"),&status);
				break;
			case PMAC2:
				break;
			case PMAC_Ultralite:
				break;
			case Turbo_PMAC1:
			case Turbo_PMAC2:
			case Turbo_PMAC2_Ultralite:
				if(theApp.m_Axes[nWhichAxis].nStatus & PMAC_AMPLIFIER_ENABLED) {
					return true;
				} else {
					return false;
				}
				Buff.Format(_T("#%d?"),theApp.m_Axes[nWhichAxis].nPhysicalAxis);
				SendStr(Buff);
				Buff.Empty();
				GetStr(Buff);
				nMask = 0x800;
				Buff.GetBufferSetLength(4);
				swscanf_s(Buff,_T("%x"),&status);
				break;
			case Turbo_PMAC_Ultralite:
				if(theApp.m_Axes[nWhichAxis].nStatus & PMAC_AMPLIFIER_ENABLED) {
					return TRUE;
				} else {
					return FALSE;
				}
				break;
			}
			if(status&nMask) return TRUE;
			break;
		case 2:
			if(nWhichAxis == theApp.m_Tank.nLeftPump) {
				if(m_nGeneralOutput & theApp.m_Tank.nPumpBit[0]) return TRUE;
			}
			if(nWhichAxis == theApp.m_Tank.nRightPump) {
				if(m_nGeneralOutput & theApp.m_Tank.nPumpBit[1]) return TRUE;
			}
			break;
		}

	}
	return FALSE;

}

void CPmac::PumpStopStart(int nWhich,bool bStopStart)
{

	switch(bStopStart) {
	case FALSE:	PumpStop(nWhich);
		break;
	case TRUE:	PumpStart(nWhich);
		break;
	}
}

void CPmac::PumpStart(int nWhich)
{
	CString Buff;
	int nn;

	switch(theApp.m_Axes[nWhich].nMotorType) {
	case 0:
		break;
	case 1:
		Buff.Format(_T("#%dJ+"),theApp.m_Axes[nWhich].nPhysicalAxis);
		SendStr(Buff);
		break;
	case 2:
		nn = m_nGeneralOutput;
		if(nWhich == theApp.m_Tank.nLeftPump) {
			nn |= theApp.m_Tank.nPumpBit[0];
		}
		if(nWhich == theApp.m_Tank.nRightPump) {
			nn |= theApp.m_Tank.nPumpBit[1];
		}
		WriteDpram(PMAC_GENERAL_OUTPUTS,m_nGeneralOutput=nn);
		break;
	}


}

void CPmac::PumpStop(int nWhich)
{
	CString Buff;

	int nn;

	switch(theApp.m_Axes[nWhich].nMotorType) {
	case 0:
		break;
	case 1:
		Buff.Format(_T("#%dk"),theApp.m_Axes[nWhich].nPhysicalAxis);
		SendStr(Buff);
		break;
	case 2:
		nn = m_nGeneralOutput;
		if(nWhich == theApp.m_Tank.nLeftPump) {
			nn &= ~theApp.m_Tank.nPumpBit[0];
		}
		if(nWhich == theApp.m_Tank.nRightPump) {
			nn &= ~theApp.m_Tank.nPumpBit[1];
		}
		WriteDpram(PMAC_GENERAL_OUTPUTS,m_nGeneralOutput=nn);
		break;
	}

}

void CPmac::SetPumpSpeed(int nWhichAxis)
{
	CString Buff;
	int nScaleFactor = 1;
	CCoord Cp;
	int nSide = 0;

		
	switch(nWhichAxis) {
	case 12: nScaleFactor = (int)theApp.m_Tank.fPumpScaleFactor[0];
		if(theApp.m_Tank.fPumpVerticalMultiplier[0]) {
			theApp.m_Motors.GetSurfacePos(&Cp);
			theApp.m_nPrimarySide == 0 ? nSide = 0 : nSide = 1;
			nScaleFactor = (int)((float)nScaleFactor * (1.0f + (theApp.m_Tank.fPumpVerticalMultiplier[0] * Cp.Side[nSide].fZ / 1000.0f)));
		}
		break;
	case 13: nScaleFactor = (int)theApp.m_Tank.fPumpScaleFactor[1];
		if(theApp.m_Tank.fPumpVerticalMultiplier[0]) {
			theApp.m_Motors.GetSurfacePos(&Cp);
			theApp.m_nPrimarySide == 0 ? nSide = 1 : nSide = 0;
			nScaleFactor = (int)((float)nScaleFactor * (1.0f + (theApp.m_Tank.fPumpVerticalMultiplier[1] * Cp.Side[nSide].fZ / 1000.0f)));
		}
		break;
	}
	if(nScaleFactor<=0) nScaleFactor = 0;
	Buff.Format(_T("I%d=%.03f"),theApp.m_Axes[nWhichAxis].nPhysicalAxis*100+22,(double)theApp.m_Axes[nWhichAxis].nMaxSpeed/1000.0f*(float)nScaleFactor);
	SendStr(Buff);
	if(ReportPumpStatus(nWhichAxis) == TRUE) {
		Buff.Format(_T("#%d"),theApp.m_Axes[nWhichAxis].nPhysicalAxis);
		SendStr(Buff);
		SendStr(L"J+");
	}

}


void CPmac::PurgePump(int nWhich,bool bStart)
{
	CString Buff;
	CTime CurrentTime = CTime::GetCurrentTime();
	int nTime;
	int nScaleFactor = 1;
	CCoord Cp;
	int nSide = 0;

	switch(nWhich) {
	case 12: nScaleFactor = (int)theApp.m_Tank.fPumpScaleFactor[0];
		if(theApp.m_Tank.fPumpVerticalMultiplier[0]) {
			theApp.m_Motors.GetSurfacePos(&Cp);
			theApp.m_nPrimarySide == 0 ? nSide = 0 : nSide = 1;
			nScaleFactor = (int)((float)nScaleFactor * (1.0f + (theApp.m_Tank.fPumpVerticalMultiplier[0] * Cp.Side[nSide].fZ / 1000.0f)));
		}
		break;
	case 13: nScaleFactor = (int)theApp.m_Tank.fPumpScaleFactor[1];
		if(theApp.m_Tank.fPumpVerticalMultiplier[0]) {
			theApp.m_Motors.GetSurfacePos(&Cp);
			theApp.m_nPrimarySide == 0 ? nSide = 0 : nSide = 1;
			nScaleFactor = (int)((float)nScaleFactor * (1.0f + (theApp.m_Tank.fPumpVerticalMultiplier[1] * Cp.Side[nSide].fZ / 1000.0f)));
		}
		break;
	}
	if(nScaleFactor<1) nScaleFactor = 1;
	if(bStart==TRUE) {
		Buff.Format(_T("I%d=%.03f"),theApp.m_Axes[nWhich].nPhysicalAxis*100+22,100.0f/1000.0f*(float)nScaleFactor);
		SendStr(Buff);
		Buff.Format(_T("#%dJ+"),theApp.m_Axes[nWhich].nPhysicalAxis);
		SendStr(Buff);
		m_nPumpPurgeStartTime[nWhich] = (int)CurrentTime.GetTime();
		return;
	}
	if(m_nPumpPurgeStartTime[nWhich]) {
		nTime = (int)CurrentTime.GetTime();
		if((nTime - m_nPumpPurgeStartTime[nWhich]) > 30) {			// 10 seconds
			SetPumpSpeed(nWhich);
			m_nPumpPurgeStartTime[nWhich]=0;
		}
	}

}


void CPmac::ChangePumpSpeedDueToHeight()
{
	CCoord Cp;

	theApp.m_Motors.GetSurfacePos(&Cp);
	switch(theApp.m_nPrimarySide) {
	case 0:
		if(fabsf(Cp.Side0.fZ - m_CpSurfacePump.Side0.fZ) >= 10.0f) {
			SetPumpSpeed(theApp.m_Tank.nLeftPump);
			m_CpSurfacePump.Side0.fZ = Cp.Side0.fZ;
		}
		if(fabsf(Cp.Side1.fZ - m_CpSurfacePump.Side1.fZ) >= 10.0f) {
			SetPumpSpeed(theApp.m_Tank.nRightPump);
			m_CpSurfacePump.Side1.fZ = Cp.Side1.fZ;
		}
		break;
	case 1:
		if(fabsf(Cp.Side1.fZ - m_CpSurfacePump.Side1.fZ) >= 10.0f) {
			SetPumpSpeed(theApp.m_Tank.nLeftPump);
			m_CpSurfacePump.Side1.fZ = Cp.Side1.fZ;
		}
		if(fabsf(Cp.Side0.fZ - m_CpSurfacePump.Side0.fZ) >= 10.0f) {
			SetPumpSpeed(theApp.m_Tank.nRightPump);
			m_CpSurfacePump.Side0.fZ = Cp.Side0.fZ;
		}
		break;
	};
}



void CPmac::KillAllMotors()
{
	int	ii;
	int nAxes=GetNumberAxes();
	CString Buff;

	for(ii=0;ii<nAxes;ii++) {
		if((theApp.m_Axes[ii].bAvailable)&&(theApp.m_Axes[ii].nMotorType==SERVO)) {
			Buff.Format(_T("#%dk"),theApp.m_Axes[ii].nPhysicalAxis);
			SendStr(Buff);
		}
	}

}

//Kills them if the e switch is pressed
bool CPmac::CheckPumps()
{
	bool bFlagKillPumps = false;

	if(m_nGeneralInput != m_nLastGeneralInput) {
		if(m_nGeneralInput & theApp.m_Tank.nEmergencyBit) bFlagKillPumps = true;
		if((m_nGeneralInput & theApp.m_Tank.nDoorBit) && (m_nGeneralInput & theApp.m_Tank.nAutomaticBit)) bFlagKillPumps = true;

		if(bFlagKillPumps == true) {
			PumpStop(theApp.m_Tank.nLeftPump);
			PumpStop(theApp.m_Tank.nRightPump);
			Sleep(200);
			m_nLastGeneralInput = m_nGeneralInput;
			return true;
		}
	}
	m_nLastGeneralInput = m_nGeneralInput;
	return false;
}

//pArray must be four integers
void CPmac::ReportPumpEncoderValues(int *pArray)
{

//	DpramGetMem(PMAC_PUMP_ENCODERS,4,(PVOID)pArray);
	CopyMemory((PVOID)pArray,m_nPumpsVanes,4 * sizeof(int));

}

void CPmac::SetGeneralInputXor()
{
	WriteDpram(PMAC_GENERAL_INPUTS_EOR,theApp.m_Tank.nGeneralInputXOR);
}

void CPmac::EnableLights(int nFlag)
{
	int nn = m_nGeneralOutput;

	int nMask = ~(theApp.m_Tank.nAndonOkay | theApp.m_Tank.nAndonCycleRunning | theApp.m_Tank.nAndonWarning | theApp.m_Tank.nAndonFault);
	nn&=nMask;
	nn|= nFlag;

	if(nn != m_nGeneralOutput) {
		WriteDpram(PMAC_GENERAL_OUTPUTS,m_nGeneralOutput = nn);
	}

}

void CPmac::DisableLimitSwitch(AxisData *pAxis)
{
	CString Buff;
	int nValue = 0;

	if(RequestVariable(_T("I"),100*pAxis->nPhysicalAxis+24,&Buff) == TRUE) {
		swscanf_s(Buff,_T("$%x"),&nValue);
	}

	nValue |= 0x20000;
	Buff.Format(_T("I%d=$%x"),pAxis->nPhysicalAxis*100 + 24,nValue);
	SendStr(Buff);

}

void CPmac::EnableLimitSwitch(AxisData *pAxis)
{
	CString Buff;
	int nValue = 0;

	if(RequestVariable(_T("I"),100*pAxis->nPhysicalAxis+24,&Buff) == TRUE) {
		swscanf_s(Buff,_T("$%x"),&nValue);
	}

	nValue &= 0xfffdffff;

	Buff.Format(_T("I%d=$%x"),pAxis->nPhysicalAxis*100 + 24,nValue);
	SendStr(Buff);

}

void CPmac::ReEnableAllLimitSwitches()
{
	CString Buff;

	for(int nAxis=1;nAxis<32;nAxis++) {
		if(m_bLS_Disabled_Reenable_On_Exit[nAxis] == true) {
			Buff.Format(_T("I%d=$0"),nAxis*100 + 24);
			SendStr(Buff);
		}
		m_bLS_Disabled_Reenable_On_Exit[nAxis] = false;
	}

}





void CPmac::StartTurntableCartesianScanningAxes(CCoord Cp)
{
	int	nTm=100;
	int	nTs=5;
	int	nTa=1;
	CString Buff;

	UpdateTurntableCartesianScanningAxes(Cp);

	WriteDpram(PMAC_JOYSTICK_TIP_MODE, 0);		//M3 //1

	WriteDpram(PMAC_TM, nTm);
	WriteDpram(PMAC_TS, nTs);
	WriteDpram(PMAC_TA, nTa);


	switch(m_nScanSide) {
	case 0:
		SetFeedRate(theApp.m_LastSettings.fFeedRate);
		SendStr(L"&1 B40 R");
		break;
	case 1:
		SetFeedRate(theApp.m_LastSettings.fFeedRate);
		SendStr(L"&1 B41 R");
		break;
	case 2:
		SetFeedRate(theApp.m_LastSettings.fFeedRate);
		SendStr(L"&1 B40 R &2 B41 R");
		break;
	}

}

void CPmac::UpdateTurntableCartesianScanningAxes(CCoord Cp)
{
	int nX0,nY0,nZ0,nXt0,nYt0;
	int	 nArray[16];
	int	ii,nMax,nTemp;
	int nNumberAxes = GetNumberAxes();
	int nPosArray[16];
	int	nDesirePos;
	int nPhysicalAxis;

	switch(theApp.m_nJobNumber) {
	default:

		nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
		nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
		nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
		nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
		nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
		nArray[0]=theApp.m_Kinematics.m_LeadScrew[0].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
		nArray[1]=theApp.m_Kinematics.m_LeadScrew[1].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
		nArray[2]=theApp.m_Kinematics.m_LeadScrew[2].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
		nArray[3]=theApp.m_Kinematics.m_LeadScrew[3].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
		nArray[4]=theApp.m_Kinematics.m_LeadScrew[4].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
		nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
		nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
		nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
		nArray[5]=theApp.m_Kinematics.m_LeadScrew[5].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
		nArray[6]=theApp.m_Kinematics.m_LeadScrew[6].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
		nArray[7]=theApp.m_Kinematics.m_LeadScrew[7].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
		nArray[8]=theApp.m_Kinematics.m_LeadScrew[8].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
		nArray[9]=theApp.m_Kinematics.m_LeadScrew[9].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);

		DpramSetMem(PMAC_DESIRE_POS,12,nArray);
		break;
	case NLR:
		nMax=0;
		for(ii=0;ii<16;ii++) {
			if((theApp.m_Axes[ii].bAvailable) && (theApp.m_Axes[ii].nMotorType==SERVO)) {
				nPhysicalAxis = theApp.m_Axes[ii].nPhysicalAxis-1;

				if(theApp.m_Axes[ii].nStepsPerRev==0) {
					nPosArray[nPhysicalAxis]=theApp.m_Kinematics.m_LeadScrew[nPhysicalAxis].ActualToMachine(theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset,theApp.m_PmacUser.m_nMachinePosition[nPhysicalAxis]);

					if((ii==theApp.m_Tank.nYtLeft) && (theApp.m_LastSettings.nManipulatorType[PORTSIDE]==ROTATE_DIF_THETA)) {
						nPosArray[nPhysicalAxis]+=theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos;
					}
					if((ii==theApp.m_Tank.nYtRight) && (theApp.m_LastSettings.nManipulatorType[STARBOARD]==ROTATE_DIF_THETA)) {
						nPosArray[nPhysicalAxis]+=theApp.m_Axes[theApp.m_Tank.nXtRight].nDesirePos;
					}

				} else {
					if(abs(theApp.m_Axes[ii].nDesirePos - theApp.m_Axes[ii].nPos) < (theApp.m_Axes[ii].nStepsPerRev / 2)) {
						nDesirePos = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset+(theApp.m_Axes[ii].nRevCounter * theApp.m_Axes[ii].nStepsPerRev);
					} else {

						if(theApp.m_Axes[ii].nDesirePos < theApp.m_Axes[ii].nPos) {
							nDesirePos = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset+((theApp.m_Axes[ii].nRevCounter+1) * theApp.m_Axes[ii].nStepsPerRev);
						} else {
							nDesirePos = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset+((theApp.m_Axes[ii].nRevCounter-1) * theApp.m_Axes[ii].nStepsPerRev);
						}

					}
					nPosArray[nPhysicalAxis] = nDesirePos;
				}
				nTemp=CalcMoveTime(&theApp.m_Axes[ii],labs(theApp.m_Axes[ii].nDesirePos-theApp.m_Axes[ii].nPos));
				if(nTemp>nMax) nMax=nTemp;
			};
		};
		DpramSetMem(PMAC_DESIRE_POS,16,nPosArray);
		break;
	}

}


void CPmac::Save(CUSLFile *pFile)
{
	CWaitCursor Wait;

	switch(m_nWhichDLL) {
	case COMMS32:		m_Comms32.Save(pFile);
		break;
	case COMMSSERVER:	m_CommsServer.Save(pFile);
		break;
	case PMACCOMMSUSL:
		SuspendDriver();
		m_nWhichDLL = COMMSSERVER;
		StartDriver();

		m_CommsServer.Save(pFile);

		SuspendDriver();
		m_nWhichDLL = PMACCOMMSUSL;
		StartDriver();
		break;
	}
	Wait.Restore();
	MessageBox(NULL,_T("Pmac variables saved to file"),_T("Message"),MB_OK);
}

void CPmac::Retrieve(CString FilePath)
{
	CWaitCursor Wait;

	switch(m_nWhichDLL) {
	case COMMS32: m_Comms32.Retrieve(FilePath);
		break;
	case COMMSSERVER: m_CommsServer.DownLoad(FilePath);
		break;
	case PMACCOMMSUSL:
		SuspendDriver();
		m_nWhichDLL = COMMSSERVER;
		StartDriver();

		m_CommsServer.DownLoad(FilePath);
		m_CommsServer.SendStr(L"I9=2");

		SuspendDriver();
		m_nWhichDLL = PMACCOMMSUSL;
		StartDriver();
		break;
	}
	Wait.Restore();
	MessageBox(NULL,_T("Pmac has been updated from file"),_T("Message"),MB_OK);
}

inline int CPmac::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}


void CPmac::LiftInitialize()
{

	if(IsLiftUp() == true) {
		LiftCommandUp();
	} else {
		if(IsLiftDown() == true) {
			LiftCommandDown();
		}
	}

}


bool CPmac::LiftCommandUp()
{
	m_nGeneralOutput |= theApp.m_Tank.nCommandLiftUp;
	m_nGeneralOutput &= ~theApp.m_Tank.nCommandLiftDown;
	WriteDpram(PMAC_GENERAL_OUTPUTS,m_nGeneralOutput);

	return TRUE;
}

bool CPmac::LiftCommandDown()
{
	m_nGeneralOutput |= theApp.m_Tank.nCommandLiftDown;
	m_nGeneralOutput &= ~theApp.m_Tank.nCommandLiftUp;
	WriteDpram(PMAC_GENERAL_OUTPUTS,m_nGeneralOutput);
	return TRUE;

}

bool CPmac::LiftCommandStop()
{
	m_nGeneralOutput &= ~theApp.m_Tank.nCommandLiftDown;
	m_nGeneralOutput &= ~theApp.m_Tank.nCommandLiftUp;
	WriteDpram(PMAC_GENERAL_OUTPUTS,m_nGeneralOutput);
	return TRUE;

}


bool CPmac::IsLiftUp()
{
	if(m_nGeneralInput & theApp.m_Tank.nLiftUpLimitSwitch) return true;
	return false;
}

bool CPmac::IsLiftDown()
{
	if(m_nGeneralInput & theApp.m_Tank.nLiftDownLimitSwitch) return true;
	return false;
}

int CPmac::WhereIsLift()
{

	if(IsLiftUp()) return 1;		//Up
	if(IsLiftDown()) return 2;		//Down
	return 3;
}

void CPmac::OriginRTheta(int nWhichAxis)
{
	int	nYtMoveDistance;
	CCoord CpHead;

	//Must do Yt first because any change in Xtoffset will affect Yt offset
	if((nWhichAxis == theApp.m_Tank.nXtLeft) || (nWhichAxis == theApp.m_Tank.nYtLeft)) {
		HomeAxisWithoutMoving(theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
		HomeAxisWithoutMoving(theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = 0;
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset = 0;
		Sleep(100);
		if(abs((nYtMoveDistance=theApp.m_Axes[theApp.m_Tank.nYtLeft].nLimitSwitchOffset - theApp.m_Si10User.ReadAbsEncoder(1))) < 32767) {
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = (nYtMoveDistance + (theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos + theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos)) % 0x10000;
		} else {
			if(nYtMoveDistance<0) {
				nYtMoveDistance += 0x10000;
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = (nYtMoveDistance + (theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos + theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos)) % 0x10000;
			} else {
				ASSERT(FALSE); // this bit needs doing
			}
		}

		theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset = ((theApp.m_Axes[theApp.m_Tank.nXtLeft].nLimitSwitchOffset - theApp.m_Si10User.ReadAbsEncoder(0)) + theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos) % 0x10000;
		Sleep(100);
	}

	if((nWhichAxis == theApp.m_Tank.nXtRight) || (nWhichAxis == theApp.m_Tank.nYtRight)) {
		HomeAxisWithoutMoving(theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis);
		HomeAxisWithoutMoving(theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis);
		theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = 0;
		theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset = 0;
		Sleep(100);
		if(abs((nYtMoveDistance=theApp.m_Axes[theApp.m_Tank.nYtRight].nLimitSwitchOffset - theApp.m_Si10User.ReadAbsEncoder(2))) < 32767) {
			theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = (nYtMoveDistance + (theApp.m_Axes[theApp.m_Tank.nYtRight].nPos + theApp.m_Axes[theApp.m_Tank.nXtRight].nPos)) % 0x10000;
		} else {
			if(nYtMoveDistance<0) {
				nYtMoveDistance += 0x10000;
				theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = (nYtMoveDistance + (theApp.m_Axes[theApp.m_Tank.nYtRight].nPos + theApp.m_Axes[theApp.m_Tank.nXtRight].nPos)) % 0x10000;
			} else {
				ASSERT(FALSE); // this bit needs doing
			}
		}

		theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset = ((theApp.m_Axes[theApp.m_Tank.nXtRight].nLimitSwitchOffset - theApp.m_Si10User.ReadAbsEncoder(3)) + theApp.m_Axes[theApp.m_Tank.nXtRight].nPos) % 0x10000;
		Sleep(100);
	}

	theApp.m_Motors.GetHeadPos(&CpHead);

	if(nWhichAxis == theApp.m_Tank.nXtLeft) CpHead.Side0.fXt=0.0f;
	if(nWhichAxis == theApp.m_Tank.nYtLeft) CpHead.Side0.fYt=0.0f;
	if(nWhichAxis == theApp.m_Tank.nXtRight) CpHead.Side1.fXt=0.0f;
	if(nWhichAxis == theApp.m_Tank.nYtRight) CpHead.Side1.fYt=0.0f;
	EnableAxes(BOTH_MOVETO);
	theApp.m_Motors.DesirePosEqualsCoord(CpHead);
	MoveTo(DONT_WAIT);

}

void CPmac::DownloadLimitSwitchOffsets()
{
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nXLeft].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nYLeft].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nZLeft].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nXtLeft].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nYtLeft].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nXRight].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nYRight].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nZRight].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nXtRight].nLimitSwitchOffset);
	SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis*100+26,theApp.m_Axes[theApp.m_Tank.nYtRight].nLimitSwitchOffset);

}


void CPmac::DownloadSoftwareOverTravelLimits()
{
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis*100+13,theApp.m_Axes[theApp.m_Tank.nXLeft].nMaxTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis*100+13,theApp.m_Axes[theApp.m_Tank.nYLeft].nMaxTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis*100+13,theApp.m_Axes[theApp.m_Tank.nZLeft].nMaxTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis*100+13,theApp.m_Axes[theApp.m_Tank.nXtLeft].nMaxTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis*100+13,theApp.m_Axes[theApp.m_Tank.nYtLeft].nMaxTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis*100+13,theApp.m_Axes[theApp.m_Tank.nXRight].nMaxTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis*100+13,theApp.m_Axes[theApp.m_Tank.nYRight].nMaxTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis*100+13,theApp.m_Axes[theApp.m_Tank.nZRight].nMaxTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis*100+13,theApp.m_Axes[theApp.m_Tank.nXtRight].nMaxTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis*100+13,theApp.m_Axes[theApp.m_Tank.nYtRight].nMaxTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis*100+14,theApp.m_Axes[theApp.m_Tank.nXLeft].nMinTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis*100+14,theApp.m_Axes[theApp.m_Tank.nYLeft].nMinTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis*100+14,theApp.m_Axes[theApp.m_Tank.nZLeft].nMinTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis*100+14,theApp.m_Axes[theApp.m_Tank.nXtLeft].nMinTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis*100+14,theApp.m_Axes[theApp.m_Tank.nYtLeft].nMinTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis*100+14,theApp.m_Axes[theApp.m_Tank.nXRight].nMinTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis*100+14,theApp.m_Axes[theApp.m_Tank.nYRight].nMinTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis*100+14,theApp.m_Axes[theApp.m_Tank.nZRight].nMinTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis*100+14,theApp.m_Axes[theApp.m_Tank.nXtRight].nMinTravel);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis*100+14,theApp.m_Axes[theApp.m_Tank.nYtRight].nMinTravel);
}

void CPmac::DisableSoftwareOverTravelLimits()
{
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis*100+13,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis*100+13,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis*100+13,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis*100+13,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis*100+13,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis*100+13,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis*100+13,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis*100+13,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis*100+13,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis*100+13,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis*100+14,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis*100+14,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis*100+14,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis*100+14,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis*100+14,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis*100+14,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis*100+14,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis*100+14,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis*100+14,0);
	theApp.m_PmacUser.SetVariable(_T("I"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis*100+14,0);
}
/*
void CPmac::EnableAxes(int nAction)
{
	switch(theApp.m_Tank.nScannerDescription) {
	case SINGLE_BRIDGE_DUAL_SIDED:  Enable12Axes(nAction);
		break;
	default: Enable10Axes(nAction);
		 break;
	}
}
*/

bool CPmac::UpdateSafetyUnitInputs(int nValue)
{
	int nInterlock = 0;

	if(theApp.m_Tank.nBrakeInterlockPLCEnable) nInterlock |= 1;
	if(theApp.m_Tank.nManipulatorInterlockPLCEnable) nInterlock |= 2;

	if(m_nWhichDLL != COMMSSERVER) {
		nValue = ((nValue&0xFF)<<8) | 4 | nInterlock;
		WriteDpram(PMAC_SAFETY_UNIT_INPUTS,nValue); //DPRAM 5C - M179
	}

	return true;
}

void CPmac::SetPLCInterlocks()
{
	int nInterlock = 0;

	if(theApp.m_Tank.nBrakeInterlockPLCEnable) nInterlock |= 1;
	if(theApp.m_Tank.nManipulatorInterlockPLCEnable) nInterlock |= 2;

//	SetVariable('M',179,nInterlock);
	UpdateSafetyUnitInputs(0);
}

void CPmac::SetFeedRate(float fPercentageRate)
{
	int nArray[4];
	int nValue;
	CString Buff;

	return;
	fPercentageRate = 100.0f;

	if(RequestVariable(L"I",10,&Buff) == TRUE) {
		swscanf_s(Buff,L"%d",&nValue);

		for(int ii=0;ii<4;ii++) {
			nArray[ii] = (int)((float)nValue * fPercentageRate/ 100.0f);
		}
		DpramSetMem(PMAC_FEEDRATE,4,nArray,true);
		theApp.m_Tank.fServoUpdateRate = (8388608.0f / (float)nValue) * 1000.0f;
	}
}

void CPmac::UpdateVirtualAxes()
{

	SendStr(L"Enable PLCC 3");
	m_fMaxStopTime = 0.0f;
}

void CPmac::JogMove(AxisData *pAxis, float fDistance, int nMode)
{
	CString Buff;

	if(fDistance != 0.0f) {
		Buff.Format(_T("I%d=%.02f"),pAxis->nPhysicalAxis*100+22,pAxis->fMaxMoveSpeed);
		SendStr(Buff);

		if(nMode == 0) {
			Buff.Format(_T("#%dJ:%d"),pAxis->nPhysicalAxis,(int)(fDistance / pAxis->fStepSize));
		} else {
			if(fDistance>0.0f) {
				Buff.Format(_T("#%dJ+"),pAxis->nPhysicalAxis);
			} else {
				Buff.Format(_T("#%dJ-"),pAxis->nPhysicalAxis);
			}
		}
	} else {
		Buff.Format(_T("#%dJ/"),pAxis->nPhysicalAxis);
	};

	SendStr(Buff);

}

void CPmac::RequestAllAccelerationsAndVelocities()
{
	int nNumberAxes = GetNumberAxes();
	CString Buff;
	
	for(int ii=0;ii<12;ii++) {
		if(RequestVariable(_T("I"),100*theApp.m_Axes[ii].nPhysicalAxis+16,&Buff) == TRUE) {
			_WTOF(Buff,theApp.m_Axes[ii].fMaxMoveSpeed);
		}
		if(RequestVariable(_T("I"),100*theApp.m_Axes[ii].nPhysicalAxis+17,&Buff) == TRUE) {
			_WTOF(Buff,theApp.m_Axes[ii].fAcceleration);
		}
	}
};


void CPmac::SetupLookAhead()
{
	int ii,nSegmentTime,nSegmentsNeeded;
	CString Buff;
	int nNumberAxes = GetNumberAxes();

	switch(m_nWhichDLL) {
	default:
		if(m_bLookAheadFlag==FALSE) {
			if(m_fMaxStopTime == 0.0f) {
				m_fMaxStopTime=0.01f;

				for(ii=0;ii<nNumberAxes;ii++) {
					m_fStopTime=theApp.m_Axes[ii].fMaxMoveSpeed/theApp.m_Axes[ii].fAcceleration;
					if(m_fStopTime>m_fMaxStopTime) m_fMaxStopTime=m_fStopTime;
				};
			}
			
			nSegmentTime = 4;	//Tm=2

			nSegmentsNeeded=(int)(m_fMaxStopTime / ((float)nSegmentTime * 2.0f));
			nSegmentsNeeded=(nSegmentsNeeded*4)/3 + 10;


			Buff.Format(_T("I5213=%d"),nSegmentTime);	//CS 2
			SendStr(Buff);
			Buff.Format(_T("I5220=%d"),nSegmentsNeeded);
			SendStr(Buff);
			Buff.Format(_T("&2 DEF LOOK %d 1000"),nSegmentsNeeded*2);
			SendStr(Buff);

			Buff.Format(_T("I5113=%d"),nSegmentTime);	//CS 1
			SendStr(Buff);
			Buff.Format(_T("I5120=%d"),nSegmentsNeeded);
			SendStr(Buff);
			Buff.Format(_T("&1 DEF LOOK %d 1000"),nSegmentsNeeded*2);
			SendStr(Buff);

			m_bLookAheadFlag=TRUE;
		}
		break;
	case PMACISAUSL:
		break;
	}//switch

}


void CPmac::DisableLookAhead()
{

	if(m_nWhichDLL==PMACISAUSL)
		return;

//	if(m_bLookAheadFlag==TRUE) {
		SendStr(L"I5113=0");			//Low coordinate system first
		SendStr(L"&1 A DEL LOOK");
		SendStr(L"I5213=0");
		SendStr(L"&2 A DEL LOOK");
		m_bLookAheadFlag=FALSE;
//	}

}


void CPmac::WriteGeneralOuput(int nValue)
{
	m_nGeneralOutput = nValue;

	WriteDpram(PMAC_GENERAL_OUTPUTS,m_nGeneralOutput);
}

int CPmac::GetGeneralOuput()
{
	return m_nGeneralOutput;
}


void CPmac::VerticalBoreScannerMoveTurntable(int nDesirePos)
{
	int nPosArray[16];

	int nAxis = theApp.m_Tank.nZRight;

	theApp.m_Axes[nAxis].nDesirePos = nDesirePos;

	DisableLookAhead();
	SetPLCInterlocks();

	SendStr(L"&3");
	EnableAxis(theApp.m_Tank.nZRight,0,'Z');


	DpramGetMem(PMAC_DESIRE_POS,16,nPosArray);

	int nPhysicalAxis = theApp.m_Axes[nAxis].nPhysicalAxis-1;

	nPosArray[nPhysicalAxis]=theApp.m_Kinematics.m_LeadScrew[nPhysicalAxis].ActualToMachine(theApp.m_Axes[nAxis].nDesirePos+theApp.m_Axes[nAxis].nOffset,theApp.m_PmacUser.m_nMachinePosition[nPhysicalAxis]);

	int nTime=CalcMoveTime(&theApp.m_Axes[nAxis],labs(theApp.m_Axes[nAxis].nDesirePos-theApp.m_Axes[nAxis].nPos));

	DpramSetMem(PMAC_DESIRE_POS,16,nPosArray);


	WriteDpram(PMAC_TM,nTime);
	WriteDpram(PMAC_TA,theApp.m_Tank.nTA);
	WriteDpram(PMAC_TS,theApp.m_Tank.nTS);
	SetFeedRate(theApp.m_LastSettings.fFeedRate);

	SendStr(L"&3 B3 R");

}

void CPmac::VerticalBoreScannerMoveProbes(float fPos)
{
	int nPosArray[16];

	int nAxis = theApp.m_Tank.nZLeft;

	DisableLookAhead();
	SetPLCInterlocks();

	if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);

	SendStr(L"&2");
	EnableAxis(theApp.m_Tank.nZLeft,0,'Y');


	DpramGetMem(PMAC_DESIRE_POS,16,nPosArray);

	int nPhysicalAxis = theApp.m_Axes[nAxis].nPhysicalAxis-1;

	theApp.m_Axes[nAxis].nDesirePos = (int)(fPos / theApp.m_Axes[nAxis].fStepSize);

	nPosArray[nPhysicalAxis]=theApp.m_Kinematics.m_LeadScrew[nPhysicalAxis].ActualToMachine(theApp.m_Axes[nAxis].nDesirePos+theApp.m_Axes[nAxis].nOffset,theApp.m_PmacUser.m_nMachinePosition[nPhysicalAxis]);

	int nTime=CalcMoveTime(&theApp.m_Axes[nAxis],labs(theApp.m_Axes[nAxis].nDesirePos-theApp.m_Axes[nAxis].nPos));

	DpramSetMem(PMAC_DESIRE_POS,16,nPosArray);


	WriteDpram(PMAC_TM,nTime);
	WriteDpram(PMAC_TA,theApp.m_Tank.nTA);
	WriteDpram(PMAC_TS,theApp.m_Tank.nTS);
	SetFeedRate(theApp.m_LastSettings.fFeedRate);

	SendStr(L"&2 B2 R");

}

void CPmac::SetLimitSwitchesToDefault()
{
	if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable==TRUE) theApp.m_PmacUser.EnableLimitSwitch(&theApp.m_Axes[theApp.m_Tank.nXLeft]);
	if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable==TRUE) theApp.m_PmacUser.EnableLimitSwitch(&theApp.m_Axes[theApp.m_Tank.nYLeft]);
	if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable==TRUE) theApp.m_PmacUser.EnableLimitSwitch(&theApp.m_Axes[theApp.m_Tank.nZLeft]);

	if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable==TRUE) theApp.m_PmacUser.DisableLimitSwitch(&theApp.m_Axes[theApp.m_Tank.nXtLeft]);
	if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable==TRUE) theApp.m_PmacUser.DisableLimitSwitch(&theApp.m_Axes[theApp.m_Tank.nYtLeft]);

	if(theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable==TRUE) theApp.m_PmacUser.EnableLimitSwitch(&theApp.m_Axes[theApp.m_Tank.nXRight]);
	if(theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable==TRUE) theApp.m_PmacUser.EnableLimitSwitch(&theApp.m_Axes[theApp.m_Tank.nYRight]);
	if(theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable==TRUE) theApp.m_PmacUser.EnableLimitSwitch(&theApp.m_Axes[theApp.m_Tank.nZRight]);

	if(theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable==TRUE) theApp.m_PmacUser.DisableLimitSwitch(&theApp.m_Axes[theApp.m_Tank.nXtRight]);
	if(theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable==TRUE) theApp.m_PmacUser.DisableLimitSwitch(&theApp.m_Axes[theApp.m_Tank.nYtRight]);

}



void CPmac::ClearGeoBrickFaults()
{
	CString Buff;

	for(int nAxis=0;nAxis<12;nAxis++) {
		if((theApp.m_Axes[nAxis].bAvailable==TRUE) && (theApp.m_Axes[nAxis].eAmplifierType==1)) {

			if(theApp.m_Axes[nAxis].nStatus & PMAC_AMPLIFIER_FAULT) {
				Buff.Format(L"MSCLRF%d",theApp.m_Axes[nAxis].nMacroNode);
				SendStr(Buff);
			}
		};
	}


}

