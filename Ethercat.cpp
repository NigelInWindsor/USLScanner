// Ethercat.cpp: implementation of the CEthercat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Ethercat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PACKET_WAIT_TIMEOUT 20
#define IO_WAIT_TIMEOUT     15000
#define HOSTSTATE_TIMEOUT   5000

extern UINT EthercatThread(LPVOID pParam);
extern void _stdcall PendForCallBack(uint32_t ulNotification,uint32_t ulDataLen,void* pvData,void* pvUser);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

// Enabling interrupts on the Hilscher card
// Run the usual setup application to configure the NETx chip to ETHERCAT

// then run
// C:\Program Files\cifx Device Diver\cifXSetup.exe
// Highlight cifX0 in left pane
// Tick ON "Use Interrupt:"
// Press "Apply"

//////////////////////////////////////////////////////////////////////

CEthercat::CEthercat()
{
	m_bConnected = false;
	m_bFieldBusConnected = false;
	m_hDriver = NULL;
	m_hChannel = NULL;
	m_nCallBackCount = 0;
	m_nOldCallBackCount = 0;

	m_hEvent = NULL;
	m_pThread = NULL;
	m_bThreadEnabled = false;
	m_bThreadFinished = false;

}

CEthercat::~CEthercat()
{
	Close();
}

void CEthercat::Initialize(CEdit *pEdit)
{
	long lRet = CIFX_NO_ERROR;

	if (pEdit) pEdit->SetWindowText(L"Starting ethercat");
	lRet = xDriverOpen(&m_hDriver);
	if (CIFX_NO_ERROR != lRet) {
		if (pEdit) pEdit->SetWindowText(L"Failed to get handle");
		return;
	};
	m_nBusStatus |= CARD_CONNECTED;
	m_bConnected = true;

	EnumBoard();
}

//This is generally called from the FieldBus thread after the Ethercat bus has been switched on
void CEthercat::StartFieldBus()
{
	long lRet = CIFX_NO_ERROR;

	if (m_hDriver && IsConnected()) {
		lRet = xChannelOpen(m_hDriver, "cifX0", 0, &m_hChannel);

		if (CIFX_NO_ERROR != lRet) {
			return;
		};
		m_nBusStatus |= CHANNEL_OPEN;


		RegisterPLCPointers();
//		TestPlc();

		lRet = xChannelHostState(m_hChannel, CIFX_HOST_STATE_READY, &m_ulState, HOSTSTATE_TIMEOUT);
		if (CIFX_NO_ERROR != lRet) {
			return;
		}
		m_nBusStatus |= HOST_READY;

		lRet = xChannelBusState(m_hChannel, CIFX_BUS_STATE_ON, &m_ulState, IO_WAIT_TIMEOUT);
		if (CIFX_NO_ERROR != lRet) {
			return;
		}
		m_nBusStatus |= ETHERCAT_BUS_ON;
		m_bFieldBusConnected = true;

		RegisterInterruptCallback();
//		RegisterNotificationCallback();
	}
}

void CEthercat::Close(CEdit *pEdit)
{
	long lRet = CIFX_NO_ERROR;

	DeRegisterInterruptCallback();
//	DeRegisterNotificationCallback();
	DeRegisterPLCPointers();
/*
	if (m_hChannel) {
		lRet = xChannelBusState(m_hChannel, CIFX_BUS_STATE_OFF, &m_ulState, 0L);
		lRet = xChannelHostState(m_hChannel, CIFX_HOST_STATE_NOT_READY, &m_ulState, HOSTSTATE_TIMEOUT);
		lRet = xChannelClose(m_hChannel);
	}
*/
	if (m_hDriver != NULL) {
		Sleep(2);
		lRet = xDriverClose(m_hDriver);
		m_hDriver = NULL;
	};

	m_hChannel = NULL;
	m_bFieldBusConnected = false;
	m_bConnected = false;
}

bool CEthercat::IsConnected()
{

	return m_bConnected;
}

bool CEthercat::IsFieldBusConnected()
{
	return m_bFieldBusConnected;
}

bool CEthercat::IsAlive()
{
	int nDiff = m_nOldCallBackCount - m_nCallBackCount;

	if(IsConnected() == false) return false;
	if(IsFieldBusConnected() == false) return false;

//	if(nDiff == 0) return false;
	return true;
}

void CEthercat::EnumBoard()
{
	uint32_t          ulBoard    = 0;
	BOARD_INFORMATION tBoardInfo = {0};

	while(CIFX_NO_ERROR == xDriverEnumBoards(m_hDriver, ulBoard, sizeof(tBoardInfo), &tBoardInfo)) {
		uint32_t            ulChannel    = 0;
		CHANNEL_INFORMATION tChannelInfo = {0};

		while(CIFX_NO_ERROR == xDriverEnumChannels(m_hDriver, ulBoard, ulChannel, sizeof(tChannelInfo), &tChannelInfo)) {
			++ulChannel;
		}
		++ulBoard;
	}

}

bool CEthercat::IORead(void *pArray,int nLength)
{
 	long lRet = CIFX_NO_ERROR;
    unsigned long       ulCycle        = 0;
    uint32_t            ulState        = 0;

	if(m_bConnected == true) {
		if(CIFX_NO_ERROR == (lRet = xChannelIORead(m_hChannel, 0, 0, nLength, pArray, IO_WAIT_TIMEOUT))) {
			return true;
		}
	}
	return false;
}

bool CEthercat::IORead(int nLength)
{
 	long lRet = CIFX_NO_ERROR;
    unsigned long       ulCycle        = 0;
    uint32_t            ulState        = 0;

	if(m_bConnected == true) {
		if(CIFX_NO_ERROR == (lRet = xChannelIORead(m_hChannel, 0, 0, nLength, m_RxArray, IO_WAIT_TIMEOUT))) {
			return true;
		}
	}
	return false;
}

bool CEthercat::IOWrite(void *pArray,int nLength)
{
 	long lRet = CIFX_NO_ERROR;
    unsigned long       ulCycle        = 0;
    uint32_t            ulState        = 0;

	if(m_bConnected == true) {
		if(CIFX_NO_ERROR == (lRet = xChannelIOWrite(m_hChannel, 0, 0, nLength, pArray, IO_WAIT_TIMEOUT))) {
			return true;
		}
	}
	return false;
}

//uint32_t g_NotificationMode = CIFX_NOTIFY_RX_MBX_FULL;
//uint32_t g_NotificationMode = CIFX_NOTIFY_TX_MBX_EMPTY;
//uint32_t g_NotificationMode = CIFX_NOTIFY_PD0_IN;
//uint32_t g_NotificationMode = CIFX_NOTIFY_PD1_IN;
//uint32_t g_NotificationMode = CIFX_NOTIFY_PD0_OUT;
//uint32_t g_NotificationMode = CIFX_NOTIFY_PD1_OUT;
uint32_t g_NotificationMode = CIFX_NOTIFY_SYNC;
//uint32_t g_NotificationMode = CIFX_NOTIFY_COM;

void CEthercat::RegisterNotificationCallback()
{
	uint32_t ulErrorCount;

	long lRet = xChannelRegisterNotification(m_hChannel, g_NotificationMode, &PendForCallBack, this);

	SwitchToSyncmode();


	if(lRet) {
		MessageBox(NULL, L"Failed to register notification on field bus",L"Error",MB_ICONERROR);
	}
	lRet = xChannelSyncState(m_hChannel,CIFX_SYNC_SIGNAL_CMD, 100, &ulErrorCount);

	lRet = xChannelPLCActivateRead ( m_hChannel, 0);
}

void CEthercat::DeRegisterNotificationCallback()
{

	long lRet = xChannelUnregisterNotification(m_hChannel, g_NotificationMode);
}


int CEthercat::SwitchToSyncmode()
{
	RCX_SET_HANDSHAKE_CONFIG_REQ_T tSendPkt= {0};
	CIFX_PACKET tRecvPkt= {0};
	int32_t res;
	CString Buff;

	memset(&tSendPkt, 0, sizeof(tSendPkt));
	tSendPkt.tHead.ulDest=0x20;
	tSendPkt.tHead.ulSrc=0x00;
	tSendPkt.tHead.ulLen=sizeof(tSendPkt.tData);
	tSendPkt.tHead.ulCmd=RCX_SET_HANDSHAKE_CONFIG_REQ;
	tSendPkt.tData.bPDInHskMode=5;	// Syncmode 1
	tSendPkt.tData.bPDInSource=0;
	tSendPkt.tData.bPDOutHskMode=4;
	tSendPkt.tData.bPDOutSource=0;
	tSendPkt.tData.bSyncHskMode= RCX_SYNC_MODE_HST_CTRL;
	tSendPkt.tData.bSyncSource= 0;
    Buff.Format(L"switchToSyncmode() start\r\n");

	res=xChannelPutPacket(m_hChannel,(CIFX_PACKET*)&tSendPkt,20000);
	Buff.Format(L"switchToSyncmode() xChannelPutPacket() res=%x\r\n",res);

	while(CIFX_NO_ERROR == (res = xChannelGetPacket(m_hChannel, sizeof(tRecvPkt), &tRecvPkt, 20000)))
	{
		if(tRecvPkt.tHeader.ulCmd == (tSendPkt.tHead.ulCmd|1))
		{
			res = tRecvPkt.tHeader.ulState;
	        Buff.Format(L"switchToSyncmode() xChannelGetPacket() res=%x\r\n",res);
			break;
		}
		else
		{
			Buff.Format(L"switchToSyncmode() wrong pkt\r\n");
		}
	}
	return res;
}


void __stdcall PendForCallBack(uint32_t ulNotification,uint32_t ulDataLen,void* pvData,void* pvUser)
{
	CString Buff;
	long lRet;
	uint32_t ulReadState, ulErrorCount;
	CEthercat* pParent = (CEthercat *)pvUser;

	pParent->m_nCallBackCount++;
	lRet = xChannelPLCIsReadReady ( pParent->m_hChannel, 0, &ulReadState);
	if( CIFX_NO_ERROR != lRet)
	{
		Buff = L"Error";
	} else  {
		if( 1 == ulReadState) {
			memcpy( pParent->m_abBuffer, pParent->m_pabDPMMemoryIN, 4);
		}
	}
	lRet = xChannelPLCActivateRead ( pParent->m_hChannel, 0);
	lRet = xChannelSyncState(pParent->m_hChannel,CIFX_SYNC_ACKNOWLEDGE_CMD, 0, &ulErrorCount);
}


void CEthercat::Si10CallBack()
{
 	long lRet = CIFX_NO_ERROR;
	double dHRTime;
	int nLength;

	if(m_bFieldBusConnected == true) {

		switch(m_EthercatIO[0].cMachineDescription) {
		default: nLength = (12 + 1) * (sizeof (int));	//12 axes + 1 control
			break;
		case DUAL_ROBOT: nLength = sizeof TwinRobotIO + 20;
			break;
		case FB_11_AXIS: nLength = (11 + 1) * (sizeof (int)) + 11;	//1 control + 11 axes + 11 status
			break;
		case FB_TWIN_TOWER: nLength = sizeof HengshenIO;
			break;
		case DUAL_ROBOT_9_PLUS_9:
		case SPX_ROBOT: nLength = sizeof SPXRobotIO;
			break;
		case RAILWAY_AXLE: nLength = sizeof RailwayAxleIO;
			break;
		case QUICKSTEP_FLATBED:
		case TANK_5AXIS: nLength = sizeof Tank5AxisIO;
			break;
		case TANK_6AXIS: nLength = sizeof Tank6AxisIO;
			break;
		case TWIN_TOWER_KINEMATIC: nLength = sizeof Tank10AxisIO;
			break;
		}

		dHRTime = m_HRTimer.GetTime();

		if(CIFX_NO_ERROR == (lRet = xChannelIORead(m_hChannel, 0, 0, nLength, m_RxArray, IO_WAIT_TIMEOUT))) {
			if(m_EthercatIO[1].cCntrl[0] - m_RxArray[1]) {

				m_CriticalSection.Lock();

				CopyMemory(&m_EthercatIO[0],&m_EthercatIO[1],sizeof EthercatIO);
				CopyMemory(&m_EthercatIO[1],&m_RxArray,nLength);
				m_EthercatIO[1].dHRTime = dHRTime;

				m_CriticalSection.Unlock();

				theApp.m_Ethercat.m_nCallBackCount++;

				if(theApp.m_bCollectNormalizeData==TRUE) {
					theApp.m_Thread.CollectNormalizeData();
				}
				if ((theApp.m_bCollectComplexData == TRUE) && (theApp.m_pScanData != NULL)) {
		//			theApp.m_pScanData->CollectComplexData();
				}
			}
		}
	}
}

void CEthercat::InvalidateRobotJoints()
{
	EthercatIO	EthercatIO[2];
	int	ii,nPos, nCoord, nDest, nFingerAxis;
	int	*pnPos0;
	int	*pnPos1;
	unsigned char cStatus;
	int	nRollOverMode = 1;
	static int	nOldPos;
	static int nFilterCount = 500;
	static bool bFlag = FALSE;
	int nOldRevCounter, nStatus;
	HengshenIO *pHengshenIO[2];
	TwinRobotIO *pTwinRobotIO[2];
	SPXRobotIO *pSPXRobotIO[2];
	RailwayAxleIO *pRailwayAxleIO[2];
	Tank5AxisIO* pTank5AxisIO[2];
	Tank6AxisIO* pTank6AxisIO[2];
	Tank10AxisIO* pTank10AxisIO[2];
	int nRobotAxis[12] = { 3,4,5,6,7,8,12,13,14,15,16,17 };
	int nCartesianAxis[12] = { 0,1,2,9,10,11 };

	if(theApp.m_nMotorType != FIELD_BUS) return;
	if(theApp.m_LastSettings.nVirtualActualRobot != 1) return;


	m_CriticalSection.Lock();
	double dHRTime = m_HRTimer.GetTime();
	CopyMemory(EthercatIO, m_EthercatIO, sizeof m_EthercatIO);
	m_CriticalSection.Unlock();

	int nDenominator = (int)((EthercatIO[1].dHRTime - EthercatIO[0].dHRTime) * 1e6);
	int nNumerator = (int)((dHRTime - EthercatIO[0].dHRTime) * 1e6);
	int nAxisNumerator;


	switch(EthercatIO[1].cMachineDescription) {
	default:
		pTwinRobotIO[0] = (TwinRobotIO*)&EthercatIO[0];
		pTwinRobotIO[1] = (TwinRobotIO*)&EthercatIO[1];
		for(ii=0;ii<14;ii++) {
			nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
			nPos = MulDiv(pTwinRobotIO[1]->nEncoderPos[ii] - pTwinRobotIO[0]->nEncoderPos[ii], nAxisNumerator, nDenominator) + pTwinRobotIO[0]->nEncoderPos[ii];
			theApp.m_Robot[ ii/6 ].m_nJointPos[ ii%6 ] = nPos;
			theApp.m_Axes[ii].nPos = nPos;
		}

		for(ii=0;ii<6;ii++) {
			theApp.m_Robot[0].m_fJointAngle[ii] = (float)theApp.m_Robot[0].m_nJointPos[ii] * theApp.m_Robot[0].m_fStepSize[ii] * DEG_TO_RAD;
			theApp.m_Robot[1].m_fJointAngle[ii] = (float)theApp.m_Robot[1].m_nJointPos[ii] * theApp.m_Robot[1].m_fStepSize[ii] * DEG_TO_RAD;
		}
		for (nCoord = 0, nDest = 0; nCoord<3; nCoord++, nDest++) {
			pTwinRobotIO[0]->cCoordStatus[nCoord] & 0x01 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x100000000000 :	theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x100000000000;
			pTwinRobotIO[0]->cCoordStatus[nCoord] & 0x02 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x1000000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x1000000000000;
			pTwinRobotIO[0]->cCoordStatus[nCoord] & 0x04 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x10000000000 :	theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x10000000000;
			pTwinRobotIO[0]->cCoordStatus[nCoord] & 0x08 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x80000000000 :	theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x80000000000;
			pTwinRobotIO[0]->cCoordStatus[nCoord] & 0x10 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x400000000000 :	theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x400000000000;
			pTwinRobotIO[0]->cCoordStatus[nCoord] & 0x20 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x4000 :			theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x4000;
			pTwinRobotIO[0]->cCoordStatus[nCoord] & 0x80 ? theApp.m_FBCtrl.m_nProgActive[nDest]    |= 0x1 :				theApp.m_FBCtrl.m_nProgActive[nDest] &= ~0x1;
		}

		for(ii=0;ii<14;ii++) {
			cStatus = pTwinRobotIO[0]->cAxisStatus[ii];
			nStatus = 0;
			if(cStatus & FB_AMPLIFIER_ENABLED)	nStatus |= PMAC_AMPLIFIER_ENABLED;
			if(cStatus & FB_HOMED)				nStatus |= PMAC_HOMED;
			if(cStatus & FB_IN_POS)				nStatus |= PMAC_IN_POS;
			if(cStatus & FB_AMPLIFIER_FAULT)	nStatus |= PMAC_AMPLIFIER_FAULT;
			if(cStatus & FB_HARD_LIMIT)			nStatus |= PMAC_HARD_LIMIT;
			if(cStatus & FB_SOFT_LIMIT)			nStatus |= PMAC_SOFT_LIMIT;
			if(cStatus & FB_MIN_SWITCH)			nStatus |= PMAC_MIN_SWITCH;
			if(cStatus & FB_MAX_SWITCH)			nStatus |= PMAC_MAX_SWITCH;
			theApp.m_Axes[ii].nStatus = nStatus;
		}

		theApp.m_FBCtrl.m_nEStopStatus[0] = pTwinRobotIO[0]->cGeneralStatus & 1;
		theApp.m_FBCtrl.m_nEStopStatus[1] = pTwinRobotIO[0]->cGeneralStatus & 1;
		theApp.m_FBCtrl.m_nPumpStatus[0] = (pTwinRobotIO[0]->cGeneralStatus >> 1) & 1;
		theApp.m_FBCtrl.m_nPumpStatus[1] = (pTwinRobotIO[0]->cGeneralStatus >> 2) & 1;
		theApp.m_FBCtrl.m_nAirKnifeStatus[0] = (pTwinRobotIO[0]->cGeneralStatus >> 3) & 1;
		theApp.m_FBCtrl.m_nAirKnifeStatus[1] = (pTwinRobotIO[0]->cGeneralStatus >> 4) & 1;
		break;
	case FB_11_AXIS:
		pnPos0 =		(int*)&EthercatIO[0].cArray;
		pnPos1 =		(int*)&EthercatIO[1].cArray;
		theApp.m_Tank.fServoUpdateRate = 3072.0f;
		for(ii=0;ii<11;ii++) {
			theApp.m_PmacUser.m_nVelocity[ii] = (pnPos1[ii] - pnPos0[ii]) * 500;

			nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
			nPos = MulDiv(pnPos1[ii] - pnPos0[ii], nAxisNumerator, nDenominator) + pnPos0[ii] - theApp.m_Axes[ii].nOffset;

			theApp.m_PmacUser.m_nMachinePosition[ii]=nPos;
			if(nPos >= m_nOldPos[ii]) {
				theApp.m_PmacUser.m_nPosition[ii] = theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[ii].nPhysicalAxis - 1 ].MachineToActualPos(nPos);
			} else {
				theApp.m_PmacUser.m_nPosition[ii] = theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[ii].nPhysicalAxis - 1 ].MachineToActualNeg(nPos);
			}
			m_nOldPos[ii] = nPos;



			if(theApp.m_Axes[ii].dStepsPerRev==0.0) {
				theApp.m_Axes[ii].nPos = theApp.m_PmacUser.m_nPosition[ii];
			} else {

				if(nRollOverMode == 0) {
					nOldPos=theApp.m_Axes[ii].nPos;
					
					theApp.m_Axes[ii].nRevCounter = (int)((double)theApp.m_PmacUser.m_nPosition[ii] / theApp.m_Axes[ii].dStepsPerRev);

					if(bFlag==FALSE) theApp.m_PmacUser.m_nLastPos[ii]=theApp.m_PmacUser.m_nPosition[ii];
					theApp.m_PmacUser.m_nPosition[ii]=theApp.m_Axes[ii].nPos + (nPos-theApp.m_PmacUser.m_nLastPos[ii]);
					theApp.m_PmacUser.m_nPosition[ii]=(int)fmod((double)theApp.m_PmacUser.m_nPosition[ii],theApp.m_Axes[ii].dStepsPerRev);
					if(theApp.m_PmacUser.m_nPosition[ii]<0) {
						theApp.m_PmacUser.m_nPosition[ii]+=(int)theApp.m_Axes[ii].dStepsPerRev;
					}
					theApp.m_Axes[ii].nPos = theApp.m_PmacUser.m_nPosition[ii];
					theApp.m_PmacUser.m_nLastPos[ii]=theApp.m_PmacUser.m_nPosition[ii];
					if(abs(nOldPos - nPos)>(int)(theApp.m_Axes[ii].dStepsPerRev / 2.0f)) {
						theApp.m_Motors.m_nRevCounter++;
					}
				} else {
					nOldRevCounter = theApp.m_Axes[ii].nRevCounter;
					theApp.m_Axes[ii].nRevCounter = (int)((double)theApp.m_PmacUser.m_nPosition[ii] / theApp.m_Axes[ii].dStepsPerRev);
					theApp.m_PmacUser.m_nPosition[ii]=(int)fmod((double)theApp.m_PmacUser.m_nPosition[ii],theApp.m_Axes[ii].dStepsPerRev);
					theApp.m_Axes[ii].nPos = theApp.m_PmacUser.m_nPosition[ii];

					if(theApp.m_Axes[ii].nRevCounter > nOldRevCounter) {
						theApp.m_Motors.m_nRevCounter++;
					}
					if(theApp.m_Axes[ii].nRevCounter < nOldRevCounter) {
						theApp.m_Motors.m_nRevCounter--;
					}
				}
			};
		}

		for(ii=0;ii<11;ii++) {
			cStatus = EthercatIO[0].cArray[44+ii];
			cStatus & FB_AMPLIFIER_ENABLED	? theApp.m_Axes[ii].nStatus |= PMAC_AMPLIFIER_ENABLED :	theApp.m_Axes[ii].nStatus &= ~PMAC_AMPLIFIER_ENABLED;
			cStatus & FB_HOMED				? theApp.m_Axes[ii].nStatus |= PMAC_HOMED :				theApp.m_Axes[ii].nStatus &= ~PMAC_HOMED;
			cStatus & FB_IN_POS				? theApp.m_Axes[ii].nStatus |= PMAC_IN_POS :				theApp.m_Axes[ii].nStatus &= ~PMAC_IN_POS;
			cStatus & FB_AMPLIFIER_FAULT	? theApp.m_Axes[ii].nStatus |= PMAC_AMPLIFIER_FAULT :		theApp.m_Axes[ii].nStatus &= ~PMAC_AMPLIFIER_FAULT;
			cStatus & FB_HARD_LIMIT			? theApp.m_Axes[ii].nStatus |= PMAC_HARD_LIMIT :			theApp.m_Axes[ii].nStatus &= ~PMAC_HARD_LIMIT;
			cStatus & FB_SOFT_LIMIT			? theApp.m_Axes[ii].nStatus |= PMAC_SOFT_LIMIT :			theApp.m_Axes[ii].nStatus &= ~PMAC_SOFT_LIMIT;
			cStatus & FB_MIN_SWITCH			? theApp.m_Axes[ii].nStatus |= PMAC_MIN_SWITCH :			theApp.m_Axes[ii].nStatus &= ~PMAC_MIN_SWITCH;
			cStatus & FB_MAX_SWITCH			? theApp.m_Axes[ii].nStatus |= PMAC_MAX_SWITCH :			theApp.m_Axes[ii].nStatus &= ~PMAC_MAX_SWITCH;
		}
		break;
	case FB_TWIN_TOWER:
		pHengshenIO[0] = (HengshenIO*)&EthercatIO[0];
		pHengshenIO[1] = (HengshenIO*)&EthercatIO[1];
		for(ii=0;ii<11;ii++) {
			theApp.m_PmacUser.m_nVelocity[ii] = (pHengshenIO[1]->nEncoderPos[ii] - pHengshenIO[0]->nEncoderPos[ii]) * 500;

			cStatus = pHengshenIO[0]->cAxisStatus[ii];
			nStatus = 0;
			if(cStatus & FB_AMPLIFIER_ENABLED)	nStatus |= PMAC_AMPLIFIER_ENABLED;
			if(cStatus & FB_HOMED)				nStatus |= PMAC_HOMED;
			if(cStatus & FB_IN_POS)				nStatus |= PMAC_IN_POS;
			if(cStatus & FB_AMPLIFIER_FAULT)	nStatus |= PMAC_AMPLIFIER_FAULT;
			if(cStatus & FB_HARD_LIMIT)			nStatus |= PMAC_HARD_LIMIT;
			if(cStatus & FB_SOFT_LIMIT)			nStatus |= PMAC_SOFT_LIMIT;
			if(cStatus & FB_MIN_SWITCH)			nStatus |= PMAC_MIN_SWITCH;
			if(cStatus & FB_MAX_SWITCH)			nStatus |= PMAC_MAX_SWITCH;

			switch(ii) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
				nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
				nPos = MulDiv(pHengshenIO[1]->nEncoderPos[ii] - pHengshenIO[0]->nEncoderPos[ii], nAxisNumerator, nDenominator) + pHengshenIO[0]->nEncoderPos[ii];
				theApp.m_PmacUser.m_nMachinePosition[ii]=nPos;
				theApp.m_PmacUser.m_nPosition[ii] = theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[ii].nPhysicalAxis-1 ].MachineToActualPos(nPos);
				theApp.m_Axes[ii].nPos = theApp.m_PmacUser.m_nPosition[ii] - theApp.m_Axes[ii].nOffset;
				theApp.m_Axes[ii].nStatus = nStatus;
				break;
			case 5:
				nFingerAxis = theApp.m_Tank.nFingerLeft;
				nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
				nPos = MulDiv(pHengshenIO[1]->nEncoderPos[ii] - pHengshenIO[0]->nEncoderPos[ii], nAxisNumerator, nDenominator) + pHengshenIO[0]->nEncoderPos[ii];
				theApp.m_PmacUser.m_nMachinePosition[ii]=nPos;
				theApp.m_PmacUser.m_nPosition[ii] = theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nFingerAxis].nPhysicalAxis-1 ].MachineToActualPos(nPos);
				theApp.m_Axes[nFingerAxis].nPos = theApp.m_PmacUser.m_nPosition[ii] - theApp.m_Axes[nFingerAxis].nOffset;
				theApp.m_Axes[nFingerAxis].nStatus = nStatus;

				break;
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
				nPos = MulDiv(pHengshenIO[1]->nEncoderPos[ii] - pHengshenIO[0]->nEncoderPos[ii], nAxisNumerator, nDenominator) + pHengshenIO[0]->nEncoderPos[ii];
				theApp.m_PmacUser.m_nMachinePosition[ii]=nPos;
				theApp.m_PmacUser.m_nPosition[ii] = theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[ii-1].nPhysicalAxis-1 ].MachineToActualPos(nPos);
				theApp.m_Axes[ii-1].nPos = theApp.m_PmacUser.m_nPosition[ii] - theApp.m_Axes[ii-1].nOffset;
				theApp.m_Axes[ii-1].nStatus = nStatus;
				break;
			};

		}

		for(nCoord=0, nDest=0 ;nCoord<3;nCoord++, nDest++) {
			pHengshenIO[0]->cCoordStatus[nCoord] & 0x0001 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x100000000000 :	theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x100000000000;
			pHengshenIO[0]->cCoordStatus[nCoord] & 0x0002 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x1000000000000 :	theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x1000000000000;
			pHengshenIO[0]->cCoordStatus[nCoord] & 0x0004 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x10000000000 :	theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x10000000000;
			pHengshenIO[0]->cCoordStatus[nCoord] & 0x0008 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x80000000000 :	theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x80000000000;
			pHengshenIO[0]->cCoordStatus[nCoord] & 0x0010 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x400000000000 :	theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x400000000000;
			pHengshenIO[0]->cCoordStatus[nCoord] & 0x0100 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x4000 :			theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x4000;
			pHengshenIO[0]->cCoordStatus[nCoord] & 0x8000 ? theApp.m_FBCtrl.m_nProgActive[nDest] |= 0x1 :					theApp.m_FBCtrl.m_nProgActive[nDest] &= ~0x1;
		}

		theApp.m_FBCtrl.m_nAirKnifeStatus[0] = pHengshenIO[0]->cAirKnifeStatus & 1;
		theApp.m_FBCtrl.m_nAirKnifeStatus[1] = (pHengshenIO[0]->cAirKnifeStatus >> 1) & 1;
		theApp.m_FBCtrl.m_nPumpStatus[0] = pHengshenIO[0]->cPumpStatus & 1;
		theApp.m_FBCtrl.m_nPumpStatus[1] = (pHengshenIO[0]->cPumpStatus >> 1) & 1;
		theApp.m_FBCtrl.m_nTemperature[0] = pHengshenIO[0]->cTemperature[0];
		theApp.m_FBCtrl.m_nTemperature[1] = pHengshenIO[0]->cTemperature[1];
		theApp.m_FBCtrl.m_nToolHolderStatus[0] = pHengshenIO[0]->cToolIDClampStatus[0] & 3;
		theApp.m_FBCtrl.m_nToolHolderStatus[1] = pHengshenIO[0]->cToolIDClampStatus[1] & 3;
		theApp.m_FBCtrl.m_nToolID[0] = (pHengshenIO[0]->cToolIDClampStatus[0] >> 4) & 0x0f;
		theApp.m_FBCtrl.m_nToolID[1] = (pHengshenIO[0]->cToolIDClampStatus[1] >> 4) & 0x0f;
		theApp.m_FBCtrl.m_nDoorStatus[0] = pHengshenIO[0]->cRackDoors & 1;
		theApp.m_FBCtrl.m_nDoorStatus[1] = (pHengshenIO[0]->cRackDoors >> 1) & 1;
		theApp.m_FBCtrl.m_nCrashDetectStatus[0] = pHengshenIO[0]->cCrashDetect & 1;
		theApp.m_FBCtrl.m_nCrashDetectStatus[1] = (pHengshenIO[0]->cCrashDetect >> 1) & 1;
		theApp.m_FBCtrl.m_nGeneralEthercatInput = pHengshenIO[0]->cGeneralInput;
		
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:

		pSPXRobotIO[0] = (SPXRobotIO*)&EthercatIO[0];
		pSPXRobotIO[1] = (SPXRobotIO*)&EthercatIO[1];
		//Do the 12 Robot axes
		for (ii = 0; ii<12; ii++) {
			nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
			nPos = MulDiv(pSPXRobotIO[1]->nEncoderPos[nRobotAxis[ii]] - pSPXRobotIO[0]->nEncoderPos[nRobotAxis[ii]], nAxisNumerator, nDenominator) + pSPXRobotIO[0]->nEncoderPos[nRobotAxis[ii]];
			theApp.m_Robot[ii / 6].m_nJointPos[ii % 6] = nPos;
		}

		for (ii = 0; ii<6; ii++) {
			theApp.m_Robot[0].m_fJointAngle[ii] = (float)theApp.m_Robot[0].m_nJointPos[ii] * theApp.m_Robot[0].m_fStepSize[ii] * DEG_TO_RAD;
			theApp.m_Robot[1].m_fJointAngle[ii] = (float)theApp.m_Robot[1].m_nJointPos[ii] * theApp.m_Robot[1].m_fStepSize[ii] * DEG_TO_RAD;
		}

		// do the 6 cartesian axes
		for (ii = 0; ii<3; ii++) {
			nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
			nPos = MulDiv(pSPXRobotIO[1]->nEncoderPos[nCartesianAxis[ii]] - pSPXRobotIO[0]->nEncoderPos[nCartesianAxis[ii]], nAxisNumerator, nDenominator) + pSPXRobotIO[0]->nEncoderPos[nCartesianAxis[ii]];
			theApp.m_Axes[ii].nPos = nPos;
			nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii + 3].eAmplifierType] * (float)nDenominator) + nNumerator;
			nPos = MulDiv(pSPXRobotIO[1]->nEncoderPos[nCartesianAxis[ii+3]] - pSPXRobotIO[0]->nEncoderPos[nCartesianAxis[ii + 3]], nAxisNumerator, nDenominator) + pSPXRobotIO[0]->nEncoderPos[nCartesianAxis[ii + 3]];
			theApp.m_Axes[10 + ii].nPos = nPos;
		}
		for (nCoord = 0, nDest = 0; nCoord<3; nCoord++, nDest++) {
			pSPXRobotIO[0]->cCoordStatus[nCoord] & 0x01 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x100000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x100000000000;
			pSPXRobotIO[0]->cCoordStatus[nCoord] & 0x02 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x1000000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x1000000000000;
			pSPXRobotIO[0]->cCoordStatus[nCoord] & 0x04 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x10000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x10000000000;
			pSPXRobotIO[0]->cCoordStatus[nCoord] & 0x08 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x80000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x80000000000;
			pSPXRobotIO[0]->cCoordStatus[nCoord] & 0x10 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x400000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x400000000000;
			pSPXRobotIO[0]->cCoordStatus[nCoord] & 0x20 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x4000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x4000;
			pSPXRobotIO[0]->cCoordStatus[nCoord] & 0x80 ? theApp.m_FBCtrl.m_nProgActive[nDest] |= 0x1 : theApp.m_FBCtrl.m_nProgActive[nDest] &= ~0x1;
		}

		for (ii = 0; ii<20; ii++) {
			cStatus = pSPXRobotIO[0]->cAxisStatus[ii];
			nStatus = 0;
			if (cStatus & FB_AMPLIFIER_ENABLED)	nStatus |= PMAC_AMPLIFIER_ENABLED;
			if (cStatus & FB_HOMED)				nStatus |= PMAC_HOMED;
			if (cStatus & FB_IN_POS)			nStatus |= PMAC_IN_POS;
			if (cStatus & FB_AMPLIFIER_FAULT)	nStatus |= PMAC_AMPLIFIER_FAULT;
			if (cStatus & FB_HARD_LIMIT)		nStatus |= PMAC_HARD_LIMIT;
			if (cStatus & FB_SOFT_LIMIT)		nStatus |= PMAC_SOFT_LIMIT;
			if (cStatus & FB_MIN_SWITCH)		nStatus |= PMAC_MIN_SWITCH;
			if (cStatus & FB_MAX_SWITCH)		nStatus |= PMAC_MAX_SWITCH;
			switch (ii) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:	theApp.m_Axes[ii].nStatus = nStatus;
				break;
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17: theApp.m_Axes[ii+1].nStatus = nStatus;
				break;
			}
		}


		theApp.m_FBCtrl.m_nEStopStatus[0] = pSPXRobotIO[0]->cGeneralStatus & 1;
		theApp.m_FBCtrl.m_nEStopStatus[1] = pSPXRobotIO[0]->cGeneralStatus & 1;
		theApp.m_FBCtrl.m_nPumpStatus[0] = (pSPXRobotIO[0]->cGeneralStatus >> 1) & 1;
		theApp.m_FBCtrl.m_nPumpStatus[1] = (pSPXRobotIO[0]->cGeneralStatus >> 2) & 1;
		theApp.m_FBCtrl.m_nAirKnifeStatus[0] = (pSPXRobotIO[0]->cGeneralStatus >> 3) & 1;
		theApp.m_FBCtrl.m_nAirKnifeStatus[1] = (pSPXRobotIO[0]->cGeneralStatus >> 4) & 1;


		//R axis
		
		theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis = 21;
		theApp.m_Axes[theApp.m_Tank.nRLeft].dStepsPerRev = 360.0 / (double)theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;
		nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[theApp.m_Tank.nRLeft].eAmplifierType] * (float)nDenominator) + nNumerator;
		nPos = MulDiv(pSPXRobotIO[1]->nEncoderPos[20] - pSPXRobotIO[0]->nEncoderPos[20], nAxisNumerator, nDenominator) + pSPXRobotIO[0]->nEncoderPos[20];
		if (nRollOverMode == 0) {

		}
		else {
			nOldRevCounter = theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter;
			theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter = (int)((double)nPos / theApp.m_Axes[theApp.m_Tank.nRLeft].dStepsPerRev);
			theApp.m_Axes[theApp.m_Tank.nRLeft].nPos = (int)fmod((double)nPos, theApp.m_Axes[theApp.m_Tank.nRLeft].dStepsPerRev);

			if (theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter > nOldRevCounter) {
				theApp.m_Motors.m_nRevCounter++;
			}
			if (theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter < nOldRevCounter) {
				theApp.m_Motors.m_nRevCounter--;
			};

		}
		
		break;
	case RAILWAY_AXLE:
		pRailwayAxleIO[0] = (RailwayAxleIO*)&EthercatIO[0];
		pRailwayAxleIO[1] = (RailwayAxleIO*)&EthercatIO[1];
		// do the cartesian axes
		for (ii = 0; ii<7; ii++) {
			nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
			nPos = MulDiv(pRailwayAxleIO[1]->nEncoderPos[ii] - pRailwayAxleIO[0]->nEncoderPos[ii], nAxisNumerator, nDenominator) + pRailwayAxleIO[0]->nEncoderPos[ii];
			cStatus = pRailwayAxleIO[0]->cAxisStatus[ii];
			nStatus = 0;
			if (cStatus & FB_AMPLIFIER_ENABLED)	nStatus |= PMAC_AMPLIFIER_ENABLED;
			if (cStatus & FB_HOMED)				nStatus |= PMAC_HOMED;
			if (cStatus & FB_IN_POS)			nStatus |= PMAC_IN_POS;
			if (cStatus & FB_AMPLIFIER_FAULT)	nStatus |= PMAC_AMPLIFIER_FAULT;
			if (cStatus & FB_HARD_LIMIT)		nStatus |= PMAC_HARD_LIMIT;
			if (cStatus & FB_SOFT_LIMIT)		nStatus |= PMAC_SOFT_LIMIT;
			if (cStatus & FB_MIN_SWITCH)		nStatus |= PMAC_MIN_SWITCH;
			if (cStatus & FB_MAX_SWITCH)		nStatus |= PMAC_MAX_SWITCH;

			switch (ii) {
			case 0: theApp.m_Axes[theApp.m_Tank.nXLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nXLeft].nStatus = nStatus;
				break;
			case 1: theApp.m_Axes[theApp.m_Tank.nYLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYLeft].nStatus = nStatus;
				break;
			case 2: theApp.m_Axes[theApp.m_Tank.nZLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nZLeft].nStatus = nStatus;
				break;
			case 3: theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nStatus = nStatus;
				break;
			case 4: theApp.m_Axes[theApp.m_Tank.nYRight].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYRight].nStatus = nStatus;
				break;
			case 5: theApp.m_Axes[theApp.m_Tank.nZRight].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nZRight].nStatus = nStatus;
				break;
			case 6: theApp.m_Axes[theApp.m_Tank.nYtRight].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYtRight].nStatus = nStatus;
				break;
			}
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos = theApp.m_Axes[theApp.m_Tank.nXRight].nPos = theApp.m_Axes[theApp.m_Tank.nXtRight].nPos = 0;
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nStatus = theApp.m_Axes[theApp.m_Tank.nXRight].nStatus = theApp.m_Axes[theApp.m_Tank.nXtRight].nStatus = 0;
		}
		for (nCoord = 0, nDest = 0; nCoord<3; nCoord++, nDest++) {
			pRailwayAxleIO[0]->cCoordStatus[nCoord] & 0x0001 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x100000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x100000000000;
			pRailwayAxleIO[0]->cCoordStatus[nCoord] & 0x0002 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x1000000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x1000000000000;
			pRailwayAxleIO[0]->cCoordStatus[nCoord] & 0x0004 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x10000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x10000000000;
			pRailwayAxleIO[0]->cCoordStatus[nCoord] & 0x0008 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x80000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x80000000000;
			pRailwayAxleIO[0]->cCoordStatus[nCoord] & 0x0010 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x400000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x400000000000;
			pRailwayAxleIO[0]->cCoordStatus[nCoord] & 0x0100 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x4000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x4000;
			pRailwayAxleIO[0]->cCoordStatus[nCoord] & 0x8000 ? theApp.m_FBCtrl.m_nProgActive[nDest] |= 0x1 : theApp.m_FBCtrl.m_nProgActive[nDest] &= ~0x1;
		}

		break;
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		pTank5AxisIO[0] = (Tank5AxisIO*)&EthercatIO[0];
		pTank5AxisIO[1] = (Tank5AxisIO*)&EthercatIO[1];

		CopyMemory(&m_32AxisIO[0], &m_32AxisIO[1], sizeof(Global32AxisIO));
		CopyMemory(&m_32AxisIO[1], pTank5AxisIO[1], 8);
		CopyMemory(&m_32AxisIO[1].nEncoderPos, pTank5AxisIO[1]->nEncoderPos, sizeof(Tank5AxisIO::nEncoderPos));
		CopyMemory(&m_32AxisIO[1].cAxisStatus, pTank5AxisIO[1]->cAxisStatus, sizeof(Tank5AxisIO::cAxisStatus));

		Transfer32AxisIOCoordStatus();

		for (ii = 0; ii<6; ii++) {
			nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
			nPos = MulDiv(pTank5AxisIO[1]->nEncoderPos[ii] - pTank5AxisIO[0]->nEncoderPos[ii], nAxisNumerator, nDenominator) + pTank5AxisIO[0]->nEncoderPos[ii];

			nStatus = ConvertAxisStatusToPmacAxisStatus(m_32AxisIO[0].cAxisStatus[ii]);

			switch (ii) {
			case 0: theApp.m_Axes[theApp.m_Tank.nXLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nXLeft].nStatus = nStatus;
				break;
			case 1: theApp.m_Axes[theApp.m_Tank.nYLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYLeft].nStatus = nStatus;
				break;
			case 2: theApp.m_Axes[theApp.m_Tank.nZLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nZLeft].nStatus = nStatus;
				break;
			case 3: theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nXtLeft].nStatus = nStatus;
				break;
			case 4: theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nStatus = nStatus;
				break;
			}
			theApp.m_Axes[theApp.m_Tank.nXRight].nPos = theApp.m_Axes[theApp.m_Tank.nYRight].nPos = theApp.m_Axes[theApp.m_Tank.nZRight].nPos = 0;
			theApp.m_Axes[theApp.m_Tank.nXtRight].nPos = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos = 0;
			theApp.m_Axes[theApp.m_Tank.nXRight].nStatus = theApp.m_Axes[theApp.m_Tank.nYRight].nStatus = theApp.m_Axes[theApp.m_Tank.nZRight].nStatus = 0;
			theApp.m_Axes[theApp.m_Tank.nXtRight].nStatus = theApp.m_Axes[theApp.m_Tank.nYtRight].nStatus = 0;
		}

		break;
	case TANK_6AXIS:
		pTank6AxisIO[0] = (Tank6AxisIO*)&EthercatIO[0];
		pTank6AxisIO[1] = (Tank6AxisIO*)&EthercatIO[1];

		CopyMemory(&m_32AxisIO[0], &m_32AxisIO[1], sizeof(Global32AxisIO));
		CopyMemory(&m_32AxisIO[1], pTank6AxisIO[1], 8);
		CopyMemory(&m_32AxisIO[1].nEncoderPos, pTank6AxisIO[1]->nEncoderPos, sizeof(Tank6AxisIO::nEncoderPos));
		CopyMemory(&m_32AxisIO[1].cAxisStatus, pTank6AxisIO[1]->cAxisStatus, sizeof(Tank6AxisIO::cAxisStatus));

		Transfer32AxisIOCoordStatus();

		for (ii = 0; ii < 6; ii++) {
			nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
			nPos = MulDiv(pTank6AxisIO[1]->nEncoderPos[ii] - pTank6AxisIO[0]->nEncoderPos[ii], nAxisNumerator, nDenominator) + pTank6AxisIO[0]->nEncoderPos[ii];

			nStatus = ConvertAxisStatusToPmacAxisStatus(pTank6AxisIO[0]->cAxisStatus[ii]);

			switch (ii) {
			case 0: theApp.m_Axes[theApp.m_Tank.nXLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nXLeft].nStatus = nStatus;
				break;
			case 1: theApp.m_Axes[theApp.m_Tank.nYLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYLeft].nStatus = nStatus;
				break;
			case 2: theApp.m_Axes[theApp.m_Tank.nZLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nZLeft].nStatus = nStatus;
				break;
			case 3: theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nXtLeft].nStatus = nStatus;
				break;
			case 4: theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nStatus = nStatus;
				break;
			case 5: theApp.m_Axes[theApp.m_Tank.nRLeft].nPos = CheckForRollOver(theApp.m_Tank.nRLeft, nPos);
				theApp.m_Axes[theApp.m_Tank.nRLeft].nStatus = nStatus;
				break;
			}
			theApp.m_Axes[theApp.m_Tank.nXRight].nPos = theApp.m_Axes[theApp.m_Tank.nYRight].nPos = theApp.m_Axes[theApp.m_Tank.nZRight].nPos = 0;
			theApp.m_Axes[theApp.m_Tank.nXtRight].nPos = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos = 0;
			theApp.m_Axes[theApp.m_Tank.nXRight].nStatus = theApp.m_Axes[theApp.m_Tank.nYRight].nStatus = theApp.m_Axes[theApp.m_Tank.nZRight].nStatus = 0;
			theApp.m_Axes[theApp.m_Tank.nXtRight].nStatus = theApp.m_Axes[theApp.m_Tank.nYtRight].nStatus = 0;
		}

		break;
	case TWIN_TOWER_KINEMATIC:
		pTank10AxisIO[0] = (Tank10AxisIO*)&EthercatIO[0];
		pTank10AxisIO[1] = (Tank10AxisIO*)&EthercatIO[1];

		CopyMemory(&m_32AxisIO[0], &m_32AxisIO[1], sizeof(Global32AxisIO));
		CopyMemory(&m_32AxisIO[1], pTank10AxisIO[1], 8);
		CopyMemory(&m_32AxisIO[1].nEncoderPos, pTank10AxisIO[1]->nEncoderPos, sizeof(Tank10AxisIO::nEncoderPos));
		CopyMemory(&m_32AxisIO[1].cAxisStatus, pTank10AxisIO[1]->cAxisStatus, sizeof(Tank10AxisIO::cAxisStatus));

		Transfer32AxisIOCoordStatus();

		for (ii = 0; ii < 10; ii++) {
			nAxisNumerator = (int)(theApp.m_Tank.fEthercatSyncShift[theApp.m_Axes[ii].eAmplifierType] * (float)nDenominator) + nNumerator;
			nPos = MulDiv(pTank10AxisIO[1]->nEncoderPos[ii] - pTank10AxisIO[0]->nEncoderPos[ii], nAxisNumerator, nDenominator) + pTank10AxisIO[0]->nEncoderPos[ii];

			nStatus = ConvertAxisStatusToPmacAxisStatus(pTank10AxisIO[0]->cAxisStatus[ii]);

			switch (ii) {
			case 0: theApp.m_Axes[theApp.m_Tank.nXLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nXLeft].nStatus = nStatus;
				break;
			case 1: theApp.m_Axes[theApp.m_Tank.nYLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYLeft].nStatus = nStatus;
				break;
			case 2: theApp.m_Axes[theApp.m_Tank.nZLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nZLeft].nStatus = nStatus;
				break;
			case 3: theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nXtLeft].nStatus = nStatus;
				break;
			case 4: theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nStatus = nStatus;
				break;

			case 5: theApp.m_Axes[theApp.m_Tank.nXRight].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nXRight].nStatus = nStatus;
				break;
			case 6: theApp.m_Axes[theApp.m_Tank.nYRight].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYRight].nStatus = nStatus;
				break;
			case 7: theApp.m_Axes[theApp.m_Tank.nZRight].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nZRight].nStatus = nStatus;
				break;
			case 8: theApp.m_Axes[theApp.m_Tank.nXtRight].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nXtRight].nStatus = nStatus;
				break;
			case 9: theApp.m_Axes[theApp.m_Tank.nYtRight].nPos = nPos;
				theApp.m_Axes[theApp.m_Tank.nYtRight].nStatus = nStatus;
				break;
			}
		}
		theApp.m_FBCtrl.m_nEStopStatus[0] = pTank10AxisIO[0]->cEStopStatus & 1;
		theApp.m_FBCtrl.m_nEStopStatus[1] = pTank10AxisIO[0]->cEStopStatus & 1;
		theApp.m_FBCtrl.m_nPumpStatus[0] = (pTank10AxisIO[0]->cPumpStatus >> 0) & 1;
		theApp.m_FBCtrl.m_nPumpStatus[1] = (pTank10AxisIO[0]->cPumpStatus >> 1) & 1;
		theApp.m_FBCtrl.m_nAirKnifeStatus[0] = (pTank10AxisIO[0]->cAirKnifeStatus >> 0) & 1;
		theApp.m_FBCtrl.m_nAirKnifeStatus[1] = (pTank10AxisIO[0]->cAirKnifeStatus >> 1) & 1;

		break;
	};

}


void CEthercat::Transfer32AxisIOCoordStatus()
{
	int nCoord, nDest;

	for (nCoord = 0, nDest = 0; nCoord < 3; nCoord++, nDest++) {
		m_32AxisIO[0].cCoordStatus[nCoord] & 0x0001 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x100000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x100000000000;
		m_32AxisIO[0].cCoordStatus[nCoord] & 0x0002 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x1000000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x1000000000000;
		m_32AxisIO[0].cCoordStatus[nCoord] & 0x0004 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x10000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x10000000000;
		m_32AxisIO[0].cCoordStatus[nCoord] & 0x0008 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x80000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x80000000000;
		m_32AxisIO[0].cCoordStatus[nCoord] & 0x0010 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x400000000000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x400000000000;
		m_32AxisIO[0].cCoordStatus[nCoord] & 0x0100 ? theApp.m_FBCtrl.m_n64CoordStatus[nDest] |= 0x4000 : theApp.m_FBCtrl.m_n64CoordStatus[nDest] &= ~0x4000;
		m_32AxisIO[0].cCoordStatus[nCoord] & 0x8000 ? theApp.m_FBCtrl.m_nProgActive[nDest] |= 0x1 : theApp.m_FBCtrl.m_nProgActive[nDest] &= ~0x1;
	}
}

bool CEthercat::ReportPumpStatus(int nAxis, int nSide)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_Axes[nAxis].nPhysicalAxis) {
		case 13: if(m_EthercatIO[1].cCntrl[0] & 1) return true;
			break;
		case 14: if(m_EthercatIO[1].cCntrl[0] & 2) return true;
			break;
		case 10: if(m_EthercatIO[1].cCntrl[0] & 1) return true;
			break;
		case 20: if(m_EthercatIO[1].cCntrl[0] & 2) return true;
			break;
		};
		break;
	case FB_11_AXIS:
		switch(nAxis) {
		case 12: if(m_EthercatIO[1].cCntrl[0] & 1) return true;
			break;
		case 13: if(m_EthercatIO[1].cCntrl[0] & 2) return true;
			break;
		};
		break;
	case FB_TWIN_TOWER:
		switch(nAxis) {
		case 12: if(theApp.m_FBCtrl.m_nPumpStatus[0] & 1) return true;
			break;
		case 13: if(theApp.m_FBCtrl.m_nPumpStatus[1] & 1) return true;
			break;
		}
		break;
	case DUAL_ROBOT:
		switch(nAxis) {
		case 12: if(theApp.m_FBCtrl.m_nPumpStatus[0] & 1) return true;
			break;
		case 13: if(theApp.m_FBCtrl.m_nPumpStatus[1] & 1) return true;
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
	case TWIN_TOWER_KINEMATIC:
		if(theApp.m_FBCtrl.m_nPumpStatus[nSide] & 1) return true;
		break;
	}
	return false;
}

bool CEthercat::ReportWaterSpray()
{

	if(m_EthercatIO[1].cCntrl[0] & 1) return true;
	return false;
}

bool CEthercat::ReportCoordStatus(int nCoord)
{

	switch(nCoord) {
	case 1: if(m_EthercatIO[1].cCntrl[1] & 1)	return true;
		break;
	case 2: if(m_EthercatIO[1].cCntrl[1] & 2)	return true;
		break;
	case 3: if(m_EthercatIO[1].cCntrl[1] & 4)	return true;
		break;
	};
	return false;
}



int	CEthercat::GetInputPort()
{
	switch(theApp.m_Tank.nScannerDescription) {
	default: return 0x80;
		break;
	case FB_11_AXIS:
		return 0x80;
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:

		return theApp.m_FBCtrl.m_nEStopStatus[0] & 1;
		break;
	}
}

void CEthercat::RegisterInterruptCallback()
{
	if(theApp.m_LastSettings.nEthercatCommunicationMode > 0 && theApp.m_LastSettings.nEthercatCommunicationMode<5) {

		if (m_bThreadEnabled == false) {
			m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			m_bThreadEnabled = true;
			m_pThread = AfxBeginThread(&EthercatThread, this, THREAD_PRIORITY_TIME_CRITICAL, 0, NULL);
		}
	}

}

void CEthercat::DeRegisterInterruptCallback()
{
	if (m_bThreadEnabled) {
		SetEvent(m_hEvent);
		m_bThreadEnabled = false;
		WaitForSingleObject(m_pThread, 1000);
		CloseHandle(m_hEvent);
	}
}

UINT EthercatThread(LPVOID pParam)
{
	CEthercat* pParent = (CEthercat *)pParam;
	pParent->m_bThreadFinished = FALSE;
	int nEventPeriod[6] = {0,0,1,2,5,5};
	int nPeriod;

	while(pParent->m_bThreadEnabled) {
		nPeriod = nEventPeriod[theApp.m_LastSettings.nEthercatCommunicationMode % 5];
		if (WaitForSingleObject(pParent->m_hEvent, nPeriod) == WAIT_OBJECT_0) {

			ResetEvent(pParent->m_hEvent);
		}
		pParent->Si10CallBack();
	}

	pParent->m_bThreadEnabled = false;
	pParent->m_bThreadFinished = TRUE;

	return 0;
}

void CEthercat::TestPlc()
{
	CString Buff;
	unsigned char* pabDPMMemory = NULL;
	uint32_t ulAreaStartOffset = 0;
	uint32_t ulAreaSize = 0;
	long lRet = CIFX_NO_ERROR;
	long lRetIN = CIFX_NO_ERROR;
	long lRetOUT = CIFX_NO_ERROR;
	unsigned char abBuffer[1000] = {0};
	uint32_t ulState = 0;

	/* Define the memory structures for Input data */
	PLC_MEMORY_INFORMATION tMemory = {0};
	tMemory.pvMemoryID = NULL; // Identification of the memory area
	tMemory.ppvMemoryPtr = (void**)&pabDPMMemory; // Memory pointer
	tMemory.ulAreaDefinition = CIFX_IO_INPUT_AREA; // Input/output area
	tMemory.ulAreaNumber = 0; // Area number
	tMemory.pulIOAreaStartOffset = &ulAreaStartOffset; // Start offset of the requested channel
	tMemory.pulIOAreaSize = &ulAreaSize; // Memory size of the requested channel
	
	/* Define the memory structures for Output data */
	unsigned char* pabDPMMemory_OUT = NULL;
	uint32_t ulAreaStartOffset_OUT = 0;
	uint32_t ulAreaSize_OUT = 0;
	PLC_MEMORY_INFORMATION tMemory_OUT = {0};
	tMemory_OUT.pvMemoryID = NULL; // Identification of the memory area
	tMemory_OUT.ppvMemoryPtr = (void**)&pabDPMMemory_OUT; // Memory pointer
	tMemory_OUT.ulAreaDefinition = CIFX_IO_OUTPUT_AREA; // Input/output area
	tMemory_OUT.ulAreaNumber = 0; // Area number
	tMemory_OUT.pulIOAreaStartOffset = &ulAreaStartOffset_OUT; // Start offset of the requested channel
	tMemory_OUT.pulIOAreaSize = &ulAreaSize_OUT; // Memory size of the requested channel
	/* Open a DPM memory pointer */
	if ( (CIFX_NO_ERROR != (lRetIN = xChannelPLCMemoryPtr( m_hChannel, CIFX_MEM_PTR_OPEN, &tMemory)) )
		|| (CIFX_NO_ERROR != (lRetOUT = xChannelPLCMemoryPtr( m_hChannel, CIFX_MEM_PTR_OPEN,&tMemory_OUT))) )
	{
		// Failed to get the memory mapping
		Buff.Format(L"InError: 0x%08lx  InError: 0x%08lx", lRetIN, lRetOUT); 
	} else
	{
		uint32_t ulWaitBusCount = 100;
		/* Signal application is ready */
		lRet = xChannelHostState( m_hChannel, CIFX_HOST_STATE_READY, &ulState, 100);
		if( CIFX_NO_ERROR != lRet)
		{
			Buff.Format(L"Error: 0x%08lx", lRet); 
		}
			/* Wait until BUS is up and running */
		Buff.Format(L"\r\nWait until BUS communication is available!\r\n");
		do
		{
			lRet = xChannelBusState( m_hChannel, CIFX_BUS_STATE_ON, &ulState, 100);
			if( CIFX_NO_ERROR != lRet)
			{
				if( CIFX_DEV_NO_COM_FLAG != lRet)
				{
					Buff.Format(L"Error: 0x%08lx", lRet); 
					break;
				}
			} else if( 1 == ulState)
			{
				/* Bus is ON */
				Buff.Format(L"\r\nBUS is ON!\r\n");
				break;
			}
		} while ( --ulWaitBusCount > 0);
		if( 0 == ulWaitBusCount)
		{
			Buff.Format(L"Error: 0x%08lx", lRet); 
		}
		/*----------------------*/
		/* Start cyclic data IO */
		/*----------------------*/
		if( CIFX_NO_ERROR == lRet)
		{			
			Buff.Format(L"\n Press any key to stop \r\n");
			int nLoopContinue = 1;
			while (nLoopContinue == 1)
			{
				// We have a memory mapping, check if access to the DPM is allowed
				uint32_t ulReadState = 0;
				uint32_t ulWriteState = 0;
				/*----------------------------------------*/
				/* Check if we can access the INPUT image */
				/*----------------------------------------*/
				lRet = xChannelPLCIsReadReady ( m_hChannel, 0, &ulReadState);
				if( CIFX_NO_ERROR != lRet)
				{
					Buff.Format(L"Error: 0x%08lx", lRet); 
				} else if( 1 == ulReadState)
				{
					/* It is allowed to read the image */
					/* Read 100 Bytes */
					memcpy( abBuffer, pabDPMMemory, sizeof(abBuffer));
					/* Activate transfer */
					lRet = xChannelPLCActivateRead ( m_hChannel, 0);
					if( CIFX_NO_ERROR != lRet)
						Buff.Format(L"Error: 0x%08lx", lRet); 
					}
					/*-----------------------------------------*/
					/* Check if we can access the OUTPUT image */
					/*-----------------------------------------*/
					lRet = xChannelPLCIsWriteReady ( m_hChannel, 0, &ulWriteState);
					if( CIFX_NO_ERROR != lRet)
					{
						Buff.Format(L"Error: 0x%08lx", lRet); 
					} else if( 1 == ulWriteState)
					{
							/* It is allowed to write the image */
							pabDPMMemory_OUT[0]++;
							pabDPMMemory_OUT[1] = abBuffer[1];
							lRet = xChannelPLCActivateWrite ( m_hChannel, 0);
							if( CIFX_NO_ERROR != lRet)
							Buff.Format(L"Error: 0x%08lx", lRet); 
					}
				}
			}
	}
	lRet = xChannelBusState( m_hChannel, CIFX_BUS_STATE_OFF, &ulState, 100);
	if(CIFX_NO_ERROR != lRet)
	{
		Buff.Format(L"Error: 0x%08lx", lRet); 
	}
	lRet = xChannelHostState( m_hChannel, CIFX_HOST_STATE_NOT_READY, &ulState, 100);
	if(CIFX_NO_ERROR != lRet)
	{
		Buff.Format(L"Error: 0x%08lx", lRet); 
	}
	/* Return the DPM memory pointer */
	if ( NULL != pabDPMMemory)
	{
		lRet = xChannelPLCMemoryPtr( m_hChannel, CIFX_MEM_PTR_CLOSE, &tMemory);
		if(lRet != CIFX_NO_ERROR)
			/* Failed to return memory pointer */
			Buff.Format(L"Error: 0x%08lx", lRet); 
	}
	/* Return the DPM memory pointer */
	if ( NULL != pabDPMMemory_OUT)
	{
		lRet = xChannelPLCMemoryPtr( m_hChannel, CIFX_MEM_PTR_CLOSE, &tMemory_OUT);
		if(lRet != CIFX_NO_ERROR)
		/* Failed to return memory pointer */
		Buff.Format(L"Error: 0x%08lx", lRet); 
	}
	
		
	Buff.Format(L"\n Test PLC functions done\r\n");

}


void CEthercat::RegisterPLCPointers()
{
	long lRetIN, lRetOUT;
	CString Buff;

	m_pabDPMMemoryIN = NULL;
	m_ulAreaStartOffsetIN = 0;
	m_ulAreaSizeIN = 0;

	ZeroMemory(&m_tMemoryIN, sizeof m_tMemoryIN);
	m_tMemoryIN.pvMemoryID = NULL; // Identification of the memory area
	m_tMemoryIN.ppvMemoryPtr = (void**)&m_pabDPMMemoryIN; // Memory pointer
	m_tMemoryIN.ulAreaDefinition = CIFX_IO_INPUT_AREA; // Input/output area
	m_tMemoryIN.ulAreaNumber = 0; // Area number
	m_tMemoryIN.pulIOAreaStartOffset = &m_ulAreaStartOffsetIN; // Start offset of the requested channel
	m_tMemoryIN.pulIOAreaSize = &m_ulAreaSizeIN; // Memory size of the requested channel
	
	/* Define the memory structures for Output data */
	m_pabDPMMemoryOUT = NULL;
	m_ulAreaStartOffsetOUT = 0;
	m_ulAreaSizeOUT = 0;

	ZeroMemory(&m_tMemoryOUT, sizeof m_tMemoryOUT);
	m_tMemoryOUT.pvMemoryID = NULL; // Identification of the memory area
	m_tMemoryOUT.ppvMemoryPtr = (void**)&m_pabDPMMemoryOUT; // Memory pointer
	m_tMemoryOUT.ulAreaDefinition = CIFX_IO_OUTPUT_AREA; // Input/output area
	m_tMemoryOUT.ulAreaNumber = 0; // Area number
	m_tMemoryOUT.pulIOAreaStartOffset = &m_ulAreaStartOffsetOUT; // Start offset of the requested channel
	m_tMemoryOUT.pulIOAreaSize = &m_ulAreaSizeOUT; // Memory size of the requested channel

	if ( (CIFX_NO_ERROR != (lRetIN = xChannelPLCMemoryPtr( m_hChannel, CIFX_MEM_PTR_OPEN, &m_tMemoryIN)) )
		|| (CIFX_NO_ERROR != (lRetOUT = xChannelPLCMemoryPtr( m_hChannel, CIFX_MEM_PTR_OPEN,&m_tMemoryOUT))) )
	{
		// Failed to get the memory mapping
		Buff.Format(L"InError: 0x%08lx  InError: 0x%08lx", lRetIN, lRetOUT); 
		return;
	}

}

void CEthercat::DeRegisterPLCPointers()
{
	long lRet;
	CString Buff;

	if ( NULL != m_pabDPMMemoryIN)
	{
		lRet = xChannelPLCMemoryPtr( m_hChannel, CIFX_MEM_PTR_CLOSE, &m_tMemoryIN);
		if(lRet != CIFX_NO_ERROR)
			Buff.Format(L"Error: 0x%08lx", lRet); 
	}
	if ( NULL != m_pabDPMMemoryOUT)
	{
		lRet = xChannelPLCMemoryPtr( m_hChannel, CIFX_MEM_PTR_CLOSE, &m_tMemoryOUT);
		if(lRet != CIFX_NO_ERROR)
			Buff.Format(L"Error: 0x%08lx", lRet); 
	}

}

CString &CEthercat::getCallbackCount()
{
	static CString strCallbackCount;

	strCallbackCount.Format(L"%d",m_nCallBackCount);
	return strCallbackCount;
}

CString &CEthercat::getCallbackPeriod()
{
	static CString strCallbackPeriod;

	strCallbackPeriod.Format(L"%f",m_EthercatIO[1].dHRTime - m_EthercatIO[0].dHRTime);
	return strCallbackPeriod;
}


CString &CEthercat::getStrDataFormat()
{
	static CString strDataFormat;

	strDataFormat.Format(L"Data format: %d",m_EthercatIO[0].cMachineDescription);

	return strDataFormat;
}

int CEthercat::getDataFormat(int *nDataFormat)
{
	if (nDataFormat != NULL) *nDataFormat = (int)m_EthercatIO[0].cMachineDescription;
	return (int)m_EthercatIO[0].cMachineDescription;
}

CString &CEthercat::getLifeCount()
{
	static CString strLifeCount;

	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: strLifeCount.Empty();
		break;
	case FB_TWIN_TOWER:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_6AXIS:
	case QUICKSTEP_FLATBED:
	case TWIN_TOWER_KINEMATIC:

		strLifeCount.Format(L"Life count: %d",pHengshenIO->cLife);
		break;
	}


	return strLifeCount;
}

CString &CEthercat::getCoordStatus(int nCoord)
{
	static CString strCoordStatus;
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
	TwinRobotIO *pTwinRobotIO = (TwinRobotIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: strCoordStatus.Empty();
		break;
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
		strCoordStatus.Format(L"Coord status %d: %04x", nCoord, pTwinRobotIO->cCoordStatus[nCoord]);
		break;
	case SPX_ROBOT:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_6AXIS:
	case QUICKSTEP_FLATBED:
	case FB_TWIN_TOWER:
	case TWIN_TOWER_KINEMATIC:
		strCoordStatus.Format(L"Coord status %d: %04x",nCoord, pHengshenIO->cCoordStatus[nCoord]);
		break;
	}

	return strCoordStatus;
}

CString &CEthercat::getEncoderPos(int nAxis)
{
	static CString strEncoderPos;
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
	TwinRobotIO *pTwinRobotIO = (TwinRobotIO*)&m_EthercatIO[0];
	SPXRobotIO *pSPXRobotIO = (SPXRobotIO*)&m_EthercatIO[0];
	RailwayAxleIO *pRailwayAxleIO = (RailwayAxleIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: strEncoderPos.Empty();
		break;
	case DUAL_ROBOT:
		strEncoderPos.Format(L"Encoder pos[%d]: %d",nAxis + 1,pTwinRobotIO->nEncoderPos[nAxis]);
		break;
	case FB_TWIN_TOWER:
		strEncoderPos.Format(L"Encoder pos[%d]: %d",nAxis + 1,pHengshenIO->nEncoderPos[nAxis]);
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		strEncoderPos.Format(L"Encoder pos[%d]: %d", nAxis + 1, pSPXRobotIO->nEncoderPos[nAxis]);
		break;
	case RAILWAY_AXLE:
		strEncoderPos.Format(L"Encoder pos[%d]: %d", nAxis + 1, pRailwayAxleIO->nEncoderPos[nAxis]);
		break;
	case TANK_5AXIS:
	case TANK_6AXIS:
	case QUICKSTEP_FLATBED:
	case TWIN_TOWER_KINEMATIC:
		strEncoderPos.Format(L"Encoder pos[%d]: %d", nAxis + 1, m_32AxisIO[0].nEncoderPos[nAxis]);
		break;
	}

	return strEncoderPos;
}

CString &CEthercat::getStrAxisStatus(int nAxis)
{
	static CString strAxisStatus;
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
	TwinRobotIO *pTwinRobotIO = (TwinRobotIO*)&m_EthercatIO[0];
	SPXRobotIO *pSPXRobotIO = (SPXRobotIO*)&m_EthercatIO[0];
	RailwayAxleIO *pRailwayAxleIO = (RailwayAxleIO*)&m_EthercatIO[0];
	Tank5AxisIO *pTank5AxisIO = (Tank5AxisIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: strAxisStatus.Empty();
		break;
	case DUAL_ROBOT:
		strAxisStatus.Format(L"0x%02x",pTwinRobotIO->cAxisStatus[nAxis]);
		break;
	case FB_TWIN_TOWER:
		strAxisStatus.Format(L"0x%02x",pHengshenIO->cAxisStatus[nAxis]);
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		strAxisStatus.Format(L"0x%02x", pSPXRobotIO->cAxisStatus[nAxis]);
		break;
	case RAILWAY_AXLE:
		strAxisStatus.Format(L"0x%02x", pRailwayAxleIO->cAxisStatus[nAxis]);
		break;
	case TANK_5AXIS:
	case TANK_6AXIS:
	case QUICKSTEP_FLATBED:
	case TWIN_TOWER_KINEMATIC:
		strAxisStatus.Format(L"0x%02x", m_32AxisIO[0].cAxisStatus[nAxis]);
		break;
	}

	return strAxisStatus;
}

int CEthercat::getAxisStatus(int nAxis)
{
	static CString strAxisStatus;
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
	TwinRobotIO *pTwinRobotIO = (TwinRobotIO*)&m_EthercatIO[0];
	SPXRobotIO *pSPXRobotIO = (SPXRobotIO*)&m_EthercatIO[0];
	RailwayAxleIO *pRailwayAxleIO = (RailwayAxleIO*)&m_EthercatIO[0];
	Tank5AxisIO *pTank5AxisIO = (Tank5AxisIO*)&m_EthercatIO[0];
	Tank6AxisIO *pTank6AxisIO = (Tank6AxisIO*)&m_EthercatIO[0];
	Tank10AxisIO *pTank10AxisIO = (Tank10AxisIO*)&m_EthercatIO[0];

	switch (getDataFormat()) {
	case DUAL_ROBOT:
		return ConvertAxisStatusToPmacAxisStatus(pTwinRobotIO->cAxisStatus[nAxis]);
		break;
	case FB_TWIN_TOWER:
		return ConvertAxisStatusToPmacAxisStatus(pHengshenIO->cAxisStatus[nAxis]);
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		return ConvertAxisStatusToPmacAxisStatus(pSPXRobotIO->cAxisStatus[nAxis]);
		break;
	case RAILWAY_AXLE:
		return ConvertAxisStatusToPmacAxisStatus(pRailwayAxleIO->cAxisStatus[nAxis]);
		break;
	case TANK_5AXIS:
	case TANK_6AXIS:
	case QUICKSTEP_FLATBED:
		return ConvertAxisStatusToPmacAxisStatus(pTank6AxisIO->cAxisStatus[nAxis]);
		break;
	case TWIN_TOWER_KINEMATIC:
		return ConvertAxisStatusToPmacAxisStatus(pTank10AxisIO->cAxisStatus[nAxis]);
		break;
	}

	return 0;
}

CString &CEthercat::getAirKnifeStatus()
{
	static CString strAirKnifeStatus;
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
	SPXRobotIO *pSPXRobotIO = (SPXRobotIO*)&m_EthercatIO[0];
	TwinRobotIO *pTwinRobotIO = (TwinRobotIO*)&m_EthercatIO[0];


	switch(getDataFormat()) {
	default: strAirKnifeStatus.Empty();
		break;
	case FB_TWIN_TOWER:
		strAirKnifeStatus.Format(L"Air Knife[1:2]: %d",pHengshenIO->cAirKnifeStatus);
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case DUAL_ROBOT:
		strAirKnifeStatus.Format(L"Air Knife[1:2]: %d",(pSPXRobotIO->cGeneralStatus >> 3) & 0x03);
		break;
	}

	return strAirKnifeStatus;
}

CString &CEthercat::getEStopStatus()
{
	static CString strEStopStatus;
	//HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
	SPXRobotIO *pSPXRobotIO = (SPXRobotIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: strEStopStatus.Empty();
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		strEStopStatus.Format(L"E Stop: %d",(pSPXRobotIO->cGeneralStatus >> 0) & 0x01);
		break;
	}

	return strEStopStatus;
}

CString &CEthercat::getPumpStatus()
{
	static CString strPumpStatus;
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
	TwinRobotIO *pTwinRobotIO = (TwinRobotIO*)&m_EthercatIO[0];
	SPXRobotIO *pSPXRobotIO = (SPXRobotIO*)&m_EthercatIO[0];
	Tank10AxisIO *pTank10AxisIO = (Tank10AxisIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: strPumpStatus.Empty();
		break;
	case FB_TWIN_TOWER:
		strPumpStatus.Format(L"Pump[1:2]: %d",pHengshenIO->cPumpStatus);
		break;
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		strPumpStatus.Format(L"Pump[1:2]: %d",(pSPXRobotIO->cGeneralStatus >> 1) & 0x03);
		break;
	case TWIN_TOWER_KINEMATIC:
		strPumpStatus.Format(L"Pump[1:2]: %d", pTank10AxisIO->cPumpStatus & 0x03);
		break;
	}

	return strPumpStatus;
}

CString &CEthercat::getRackDoorStatus()
{
	static CString strRackDoorStatus;
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
//	TwinRobotIO *pTwinRobotIO = (TwinRobotIO*)&m_EthercatIO[0];
//	SPXRobotIO *pSPXRobotIO = (SPXRobotIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: strRackDoorStatus.Empty();
		break;
	case FB_TWIN_TOWER:
		strRackDoorStatus.Format(L"Rack door[1:2]: %d",pHengshenIO->cRackDoors);
		break;
	}

	return strRackDoorStatus;
}

CString &CEthercat::getToolClampStatusAndID(int nSide)
{
	static CString strStatusID;
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: strStatusID.Empty();
		break;
	case FB_TWIN_TOWER:
		strStatusID.Format(L"Clamp %d: %d,  ID: %d",nSide, pHengshenIO->cToolIDClampStatus[nSide] & 1, (pHengshenIO->cToolIDClampStatus[nSide] >> 4) & 0x0f);
		break;
	}

	return strStatusID;
}

CString &CEthercat::getTemperature(int nWhich)
{
	static CString strTemperature;
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: strTemperature.Empty();
		break;
	case FB_TWIN_TOWER:
		strTemperature.Format(L"Temperature %d: %dC ",nWhich,pHengshenIO->cTemperature[nWhich]);
		break;
	}

	return strTemperature;
}


int CEthercat::EncoderPos(int nAxis)
{
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
	TwinRobotIO *pTwinRobotIO = (TwinRobotIO*)&m_EthercatIO[0];
	SPXRobotIO *pSPXRobotIO = (SPXRobotIO*)&m_EthercatIO[0];
	Tank5AxisIO* pTank5AxisIO = (Tank5AxisIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: return 0;
		break;
	case DUAL_ROBOT:
		return pTwinRobotIO->nEncoderPos[nAxis];
		break;
	case FB_TWIN_TOWER:
		return pHengshenIO->nEncoderPos[nAxis];
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		return pSPXRobotIO->nEncoderPos[nAxis];
		break;
	case TANK_5AXIS:
	case TANK_6AXIS:
	case QUICKSTEP_FLATBED:
	case TWIN_TOWER_KINEMATIC:
		return m_32AxisIO[0].nEncoderPos[nAxis];
		break;
	}

	return 0;
}


bool CEthercat::IsRotateRunning()
{
	bool bFlag = false;
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
	TwinRobotIO *pTwinRobotIO = (TwinRobotIO*)&m_EthercatIO[0];
	SPXRobotIO *pSPXRobotIO = (SPXRobotIO*)&m_EthercatIO[0];

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		if((pSPXRobotIO->cAxisStatus[20] & 0x01) == 0x01 && (pSPXRobotIO->cAxisStatus[20] & 0x02) == 0x00) { //Powered and not in position
			bFlag = true;																					 //in other words moving
		}
		break;
	}

	return bFlag;

}


int CEthercat::getMotorStatus(int nMotor)
{
	HengshenIO *pHengshenIO = (HengshenIO*)&m_EthercatIO[0];
	TwinRobotIO *pTwinRobotIO = (TwinRobotIO*)&m_EthercatIO[0];
	SPXRobotIO *pSPXRobotIO = (SPXRobotIO*)&m_EthercatIO[0];
	Tank5AxisIO* pTank5AxisIO = (Tank5AxisIO*)&m_EthercatIO[0];

	switch(getDataFormat()) {
	default: return 1;
		break;
	case DUAL_ROBOT:
		return pTwinRobotIO->cAxisStatus[nMotor];
		break;
	case FB_TWIN_TOWER:
		return pHengshenIO->cAxisStatus[nMotor];
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		return pSPXRobotIO->cAxisStatus[nMotor];
		break;
	case TANK_5AXIS:
	case TANK_6AXIS:
	case QUICKSTEP_FLATBED:
	case TWIN_TOWER_KINEMATIC:
		return m_32AxisIO[0].cAxisStatus[nMotor];
		break;
	}

}

int CEthercat::ConvertAxisStatusToPmacAxisStatus(int cStatus)
{
	int nStatus = 0;
	if (cStatus & FB_AMPLIFIER_ENABLED)	nStatus |= PMAC_AMPLIFIER_ENABLED;
	if (cStatus & FB_HOMED)				nStatus |= PMAC_HOMED;
	if (cStatus & FB_IN_POS)			nStatus |= PMAC_IN_POS;
	if (cStatus & FB_AMPLIFIER_FAULT)	nStatus |= PMAC_AMPLIFIER_FAULT;
	if (cStatus & FB_HARD_LIMIT)		nStatus |= PMAC_HARD_LIMIT;
	if (cStatus & FB_SOFT_LIMIT)		nStatus |= PMAC_SOFT_LIMIT;
	if (cStatus & FB_MIN_SWITCH)		nStatus |= PMAC_MIN_SWITCH;
	if (cStatus & FB_MAX_SWITCH)		nStatus |= PMAC_MAX_SWITCH;
	return nStatus;
}

int CEthercat::CheckForRollOver(int nAxis, int nPos)
{
	int nRollOverMode = 1;
	static int nOldPos = -1;
	bool bFlag = false;
	static int nOldRevCounter = -1;


	if (theApp.m_Axes[nAxis].dStepsPerRev == 0.0) {
		theApp.m_Axes[nAxis].nPos = nPos;
	}
	else {

		if (nRollOverMode == 0) {
			nOldPos = theApp.m_Axes[nAxis].nPos;

			theApp.m_Axes[nAxis].nRevCounter = (int)((double)theApp.m_PmacUser.m_nPosition[nAxis] / theApp.m_Axes[nAxis].dStepsPerRev);

			if (bFlag == FALSE) theApp.m_PmacUser.m_nLastPos[nAxis] = theApp.m_PmacUser.m_nPosition[nAxis];
			theApp.m_PmacUser.m_nPosition[nAxis] = theApp.m_Axes[nAxis].nPos + (nPos - theApp.m_PmacUser.m_nLastPos[nAxis]);
			theApp.m_PmacUser.m_nPosition[nAxis] = (int)fmod((double)theApp.m_PmacUser.m_nPosition[nAxis], theApp.m_Axes[nAxis].dStepsPerRev);
			if (theApp.m_PmacUser.m_nPosition[nAxis]<0) {
				theApp.m_PmacUser.m_nPosition[nAxis] += (int)theApp.m_Axes[nAxis].dStepsPerRev;
			}
			theApp.m_Axes[nAxis].nPos = theApp.m_PmacUser.m_nPosition[nAxis];
			theApp.m_PmacUser.m_nLastPos[nAxis] = theApp.m_PmacUser.m_nPosition[nAxis];
			if (abs(nOldPos - nPos)>(int)(theApp.m_Axes[nAxis].dStepsPerRev / 2.0f)) {
				theApp.m_Motors.m_nRevCounter++;
			}
		}
		else {
			nOldRevCounter = theApp.m_Axes[nAxis].nRevCounter;
			theApp.m_PmacUser.m_nPosition[nAxis] = theApp.m_Axes[nAxis].nPos + (nPos - theApp.m_PmacUser.m_nLastPos[nAxis]);
			theApp.m_Axes[nAxis].nRevCounter = (int)((double)theApp.m_PmacUser.m_nPosition[nAxis] / theApp.m_Axes[nAxis].dStepsPerRev);
			theApp.m_PmacUser.m_nPosition[nAxis] = (int)fmod((double)theApp.m_PmacUser.m_nPosition[nAxis], theApp.m_Axes[nAxis].dStepsPerRev);
			theApp.m_Axes[nAxis].nPos = theApp.m_PmacUser.m_nPosition[nAxis];
			theApp.m_PmacUser.m_nLastPos[nAxis] = theApp.m_PmacUser.m_nPosition[nAxis];

			if (theApp.m_Axes[nAxis].nRevCounter > nOldRevCounter) {
				theApp.m_Motors.m_nRevCounter++;
			}
			if (theApp.m_Axes[nAxis].nRevCounter < nOldRevCounter) {
				theApp.m_Motors.m_nRevCounter--;
			}
		}
	};

	return theApp.m_Axes[nAxis].nPos;
}
