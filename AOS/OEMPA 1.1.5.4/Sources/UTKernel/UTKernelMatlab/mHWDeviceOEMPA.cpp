#include "stdafx.h"
#include "mHWDeviceOEMPA.h"

#define MAKE_i64(hi, lo)    (  (LONGLONG(DWORD(hi) & 0xffffffff) << 32 ) | LONGLONG(DWORD(lo) & 0xffffffff)  )

extern bool g_bToolbox;
DllImport void EnableIcmpCloseHandle(bool bEnable);


mCHWDeviceOEMPA::mCHWDeviceOEMPA(int iOEMPACycleCountMax, int iAscanPointCount, bool bFifo)
{
	EnableIcmpCloseHandle(false);//The call "pIcmpCloseHandle" can stuck (in case of bad processing from matlab for example)

	m_bToolbox = g_bToolbox;
	InitializeCriticalSection(&m_AcquisitionAscan);
	InitializeCriticalSection(&m_AcquisitionCscan);

	m_iCycleCount = -1;
	m_iHWCycleCount = -1;
	m_bEnableFMC = false;
	m_iFMCElementStart = 0;
	m_iFMCElementStop = 0;
	m_iFMCElementStep = 0;
	m_dAcquisitionStep = 0.0;

	m_ullAcquisitionTotalAscanCount = 0;
	m_ullAcquisitionTotalByteCount = 0;
	memset(m_iAcquisitionAscanSize,0,sizeof(m_iAcquisitionAscanSize));
	m_iOEMPACycleCountMax = iOEMPACycleCountMax;
	m_iAscanPointCount = iAscanPointCount;
	m_peBitSize = new enumBitSize[m_iOEMPACycleCountMax];
	m_pbSigned = new bool[m_iOEMPACycleCountMax];
	for(int iEncoderIndex=0;iEncoderIndex<g_iDriverEncoderCountMax;iEncoderIndex++)
	{
		m_lAcquisitionAscanRawEncoder[iEncoderIndex] = 0;
		m_dAcquisitionAscanEncoder[iEncoderIndex] = 0.0;
	}

	m_ullAcquisitionTotalCscanCount = 0;
	for(int iEncoderIndex=0;iEncoderIndex<g_iDriverEncoderCountMax;iEncoderIndex++)
	{
		m_lAcquisitionCscanRawEncoder[iEncoderIndex] = 0;
		m_dAcquisitionCscanEncoder[iEncoderIndex] = 0.0;
	}
	for(int iCycleIndex=0;iCycleIndex<m_iOEMPACycleCountMax;iCycleIndex++)
	{
		m_peBitSize[iCycleIndex] = e8Bits;
		m_pbSigned[iCycleIndex] = true;
	}

	m_pbyAcquisitionAscanBuffer = NULL;
	for(int iIndex=0;iIndex<4;iIndex++)
	{
		m_pbAcquisitionCscanEnable[iIndex] = NULL;
		m_pbyAcquisitionCscanAmplitude[iIndex] = NULL;
		m_psAcquisitionCscanTimeOfFlight[iIndex] = NULL;
	}

	m_pFifoAscan = m_hwDeviceOEMPA.GetAcquisitionFifo(eFifoAscan);
	m_pFifoCscan = m_hwDeviceOEMPA.GetAcquisitionFifo(eFifoCscan);
	m_hwDeviceOEMPA.SetAcquisitionParameter(this);
	UpdateFifo(bFifo);

	OEMPA_SetCallbackCustomizedDriverAPI(CallbackCustomizedAPI);
	m_hwDeviceOEMPA.SetDerivedClass(L"mCHWDeviceOEMPA",this);
}

mCHWDeviceOEMPA::~mCHWDeviceOEMPA()
{
	if(m_pbyAcquisitionAscanBuffer)
		delete m_pbyAcquisitionAscanBuffer;
	m_pbyAcquisitionAscanBuffer = NULL;
	for(int iIndex=0;iIndex<4;iIndex++)
	{
		if(m_pbAcquisitionCscanEnable)
			delete m_pbAcquisitionCscanEnable[iIndex];
		if(m_pbyAcquisitionCscanAmplitude)
			delete m_pbyAcquisitionCscanAmplitude[iIndex];
		if(m_psAcquisitionCscanTimeOfFlight)
			delete m_psAcquisitionCscanTimeOfFlight[iIndex];
		m_pbAcquisitionCscanEnable[iIndex] = NULL;
		m_pbyAcquisitionCscanAmplitude[iIndex] = NULL;
		m_psAcquisitionCscanTimeOfFlight[iIndex] = NULL;
	}
	if(m_peBitSize)
		delete m_peBitSize;
	if(m_pbSigned)
		delete m_pbSigned;
	DeleteCriticalSection(&m_AcquisitionAscan);
	DeleteCriticalSection(&m_AcquisitionCscan);
}

void mCHWDeviceOEMPA::Connected()
{
	bool bEnableFMC,bRet=true;
	int iCycleCount;

	if(m_bFifo)
	{
#ifdef _WIN64
		m_pFifoAscan->RemoveAll();
		m_pFifoCscan->RemoveAll();
#endif _WIN64
	}
	if(m_hwDeviceOEMPA.LockDevice())
	{
		if(!m_hwDeviceOEMPA.GetEnableFMC(&bEnableFMC))
			bRet = false;
		if(!m_hwDeviceOEMPA.GetCycleCount(&iCycleCount))
			bRet = false;
		m_hwDeviceOEMPA.UnlockDevice();
	}
	if(bRet && !bEnableFMC)
	{
		m_iCycleCount = iCycleCount;
		m_iHWCycleCount = iCycleCount;
	}
}

void mCHWDeviceOEMPA::UpdateFifo(bool bFifo)
{
	m_bFifo = bFifo;
	if(!m_bFifo)
	{
		if(!m_pbyAcquisitionAscanBuffer)
			m_pbyAcquisitionAscanBuffer = new BYTE[m_iOEMPACycleCountMax*m_iAscanPointCount];
		for(int iIndex=0;iIndex<4;iIndex++)
		{
			if(!m_pbAcquisitionCscanEnable[iIndex])
				m_pbAcquisitionCscanEnable[iIndex] = new bool[g_iOEMPACycleCountMax];
			if(!m_pbyAcquisitionCscanAmplitude[iIndex])
				m_pbyAcquisitionCscanAmplitude[iIndex] = new BYTE[g_iOEMPACycleCountMax];
			if(!m_psAcquisitionCscanTimeOfFlight[iIndex])
				m_psAcquisitionCscanTimeOfFlight[iIndex] = new short[g_iOEMPACycleCountMax];
			memset(m_pbAcquisitionCscanEnable[iIndex],0,g_iOEMPACycleCountMax*sizeof(bool));
			memset(m_pbyAcquisitionCscanAmplitude[iIndex],0,g_iOEMPACycleCountMax*sizeof(BYTE));
			memset(m_psAcquisitionCscanTimeOfFlight[iIndex],0,g_iOEMPACycleCountMax*sizeof(short));
		}
		memset(m_pbyAcquisitionAscanBuffer,0,sizeof(BYTE)*m_iOEMPACycleCountMax*m_iAscanPointCount);
	}else{
		m_pbyAcquisitionAscanBuffer = NULL;
		for(int iIndex=0;iIndex<4;iIndex++)
		{
			m_pbAcquisitionCscanEnable[iIndex] = NULL;
			m_pbyAcquisitionCscanAmplitude[iIndex] = NULL;
			m_psAcquisitionCscanTimeOfFlight[iIndex] = NULL;
		}
	}
	if(!m_bFifo)
	{
#ifdef _WIN64
		if(m_pFifoAscan)
			m_pFifoAscan->Desalloc();
		if(m_pFifoCscan)
			m_pFifoCscan->Desalloc();
#endif _WIN64
		m_hwDeviceOEMPA.SetAcquisitionAscan_0x00010103(ProcessAcquisitionAscan_0x00010103);
		m_hwDeviceOEMPA.SetAcquisitionCscan_0x00010X02(ProcessAcquisitionCscan_0x00010X02);
	}else{
		m_hwDeviceOEMPA.SetAcquisitionAscan_0x00010103(NULL);
		m_hwDeviceOEMPA.SetAcquisitionCscan_0x00010X02(NULL);
#ifdef _WIN64
		if(m_pFifoAscan)
			m_pFifoAscan->Alloc(m_iOEMPACycleCountMax, m_iOEMPACycleCountMax*m_iAscanPointCount);
		if(m_pFifoCscan)
			m_pFifoCscan->Alloc(m_iOEMPACycleCountMax, m_iOEMPACycleCountMax*g_iOEMPAGateCountMax*(sizeof(CSubStreamCscan_0x0X02)+sizeof(structCscanAmpTof_0x0202)));
#endif _WIN64
	}
}

bool mCHWDeviceOEMPA::IsFifoAscan()
{
	return m_bFifo;
}


UINT WINAPI mCHWDeviceOEMPA::ProcessAcquisitionAscan_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
{
	mCHWDeviceOEMPA *pmHWDevice=(mCHWDeviceOEMPA *)pAcquisitionParameter;
	int iCycleIndex=pAscanHeader->cycle,iFMCCount;

#ifdef _WIN64
	InterlockedIncrement64((volatile long long*)&pmHWDevice->m_ullAcquisitionTotalAscanCount);
	InterlockedAdd64((volatile long long*)&pmHWDevice->m_ullAcquisitionTotalByteCount,(pAscanHeader->size+sizeof(DWORD)));
#else _WIN64
	InterlockedIncrement((volatile long*)&pmHWDevice->m_ullAcquisitionTotalAscanCount);
	InterlockedExchange((volatile long*)&pmHWDevice->m_ullAcquisitionTotalByteCount,(DWORD)(pmHWDevice->m_ullAcquisitionTotalByteCount+(pAscanHeader->size+sizeof(DWORD))));
#endif _WIN64
#ifdef _DEVICE_OEMPA_256_256_
	if(pmHWDevice->m_csDevice256.m_bMaster || pmHWDevice->m_csDevice256.m_bSlave)
		return pmHWDevice->m_csDevice256.ProcessAcquisitionAscan_0x00010103(pAcquisitionParameter,acqInfo,pStreamHeader,pAscanHeader,pBufferMax,pBufferMin,pBufferSat);
#endif _DEVICE_OEMPA_256_256_
	EnterCriticalSection(&pmHWDevice->m_AcquisitionAscan);
	if(acqInfo.bFullMatrixCapture && pmHWDevice->m_bEnableFMC)
	{
		iFMCCount = (int)(pmHWDevice->m_iFMCElementStop-pmHWDevice->m_iFMCElementStart+1);
		if(pmHWDevice->m_iFMCElementStep!=1)
			iFMCCount = ((iFMCCount-1)/pmHWDevice->m_iFMCElementStep)+1;
		iCycleIndex = iCycleIndex*iFMCCount+(int)acqInfo.lFMCElementIndex-pmHWDevice->m_iFMCElementStart;
	}
	if(((int)pAscanHeader->dataCount<pmHWDevice->m_iAscanPointCount) && (iCycleIndex<pmHWDevice->m_iCycleCount))
	{
		for(int iEncoderIndex=0;iEncoderIndex<g_iDriverEncoderCountMax;iEncoderIndex++)
		{
			pmHWDevice->m_lAcquisitionAscanRawEncoder[iEncoderIndex] = acqInfo.lEncoder[iEncoderIndex];
			pmHWDevice->m_dAcquisitionAscanEncoder[iEncoderIndex] = acqInfo.dEncoder[iEncoderIndex];
		}
		//1.1.5.2n 12bits/16bits
		pmHWDevice->m_peBitSize[iCycleIndex] = (enumBitSize)pAscanHeader->bitSize;
		pmHWDevice->m_pbSigned[iCycleIndex] = (pAscanHeader->sign>0?true:false);
		pmHWDevice->m_iAcquisitionAscanSize[iCycleIndex] = (int)(pAscanHeader->dataCount*pAscanHeader->dataSize);
		if(pmHWDevice->m_pbyAcquisitionAscanBuffer)
			memcpy(&pmHWDevice->m_pbyAcquisitionAscanBuffer[iCycleIndex*pmHWDevice->m_iAscanPointCount],pBufferMax,pAscanHeader->dataCount*pAscanHeader->dataSize);
	}
	LeaveCriticalSection(&pmHWDevice->m_AcquisitionAscan);
	return 0;
}

typedef union unionAmp{
	char charData;
	BYTE byteData;
}unionAmp;
UINT WINAPI mCHWDeviceOEMPA::ProcessAcquisitionCscan_0x00010X02(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof)
{
	mCHWDeviceOEMPA *pmHWDevice=(mCHWDeviceOEMPA *)pAcquisitionParameter;
	unsigned int uiGateId,uiCycle,uiData;
	unionAmp *pAmp;

	InterlockedIncrement64((volatile long long*)&pmHWDevice->m_ullAcquisitionTotalCscanCount);
	EnterCriticalSection(&pmHWDevice->m_AcquisitionCscan);
	if(pCscanHeader->Check() && pBufferAmp)
	{
		for(int iEncoderIndex=0;iEncoderIndex<g_iDriverEncoderCountMax;iEncoderIndex++)
		{
			pmHWDevice->m_lAcquisitionCscanRawEncoder[iEncoderIndex] = acqInfo.lEncoder[iEncoderIndex];
			pmHWDevice->m_dAcquisitionCscanEncoder[iEncoderIndex] = acqInfo.dEncoder[iEncoderIndex];
		}
		uiCycle = pCscanHeader->cycle;
		if(uiCycle<(unsigned int)pmHWDevice->m_hwDeviceOEMPA.GetSWDeviceOEMPA()->GetCycleCountMax())
		for(int iIndex=0;iIndex<(int)pCscanHeader->count;iIndex++)
		{
			uiGateId = pBufferAmp[iIndex].gateId;
			if(uiGateId>=g_iOEMPAGateCountMax)
				continue;
			uiData = pBufferAmp[iIndex].byAmp;
			pAmp = (unionAmp*)&uiData;
			if(pBufferAmp[iIndex].sign)
				pmHWDevice->m_pbyAcquisitionCscanAmplitude[uiGateId][uiCycle] = (BYTE)pAmp->charData;
			else
				pmHWDevice->m_pbyAcquisitionCscanAmplitude[uiGateId][uiCycle] = (BYTE)pAmp->byteData;
			pmHWDevice->m_pbAcquisitionCscanEnable[uiGateId][uiCycle] = true;
		}
	}
	if(pCscanHeader->Check() && pBufferAmpTof)
	{
		for(int iEncoderIndex=0;iEncoderIndex<g_iDriverEncoderCountMax;iEncoderIndex++)
		{
			pmHWDevice->m_lAcquisitionCscanRawEncoder[iEncoderIndex] = acqInfo.lEncoder[iEncoderIndex];
			pmHWDevice->m_dAcquisitionCscanEncoder[iEncoderIndex] = acqInfo.dEncoder[iEncoderIndex];
		}
		uiCycle = pCscanHeader->cycle;
		if(uiCycle<(unsigned int)pmHWDevice->m_hwDeviceOEMPA.GetSWDeviceOEMPA()->GetCycleCountMax())
		for(int iIndex=0;iIndex<(int)pCscanHeader->count;iIndex++)
		{
			uiGateId = pBufferAmpTof[iIndex].gateId;
			if(uiGateId>=g_iOEMPAGateCountMax)
				continue;
			uiData = pBufferAmpTof[iIndex].byAmp;
			pAmp = (unionAmp*)&uiData;
			if(pBufferAmpTof[iIndex].sign)
				pmHWDevice->m_pbyAcquisitionCscanAmplitude[uiGateId][uiCycle] = (BYTE)pAmp->charData;
			else
				pmHWDevice->m_pbyAcquisitionCscanAmplitude[uiGateId][uiCycle] = (BYTE)pAmp->byteData;
			pmHWDevice->m_pbyAcquisitionCscanAmplitude[uiGateId][uiCycle] = (BYTE)pAmp->byteData;
			pmHWDevice->m_psAcquisitionCscanTimeOfFlight[uiGateId][uiCycle] = pBufferAmpTof[iIndex].wTof;
			pmHWDevice->m_pbAcquisitionCscanEnable[uiGateId][uiCycle] = true;
		}
	}
	LeaveCriticalSection(&pmHWDevice->m_AcquisitionCscan);
	return 0;
}

structCorrectionOEMPA* WINAPI mCHWDeviceOEMPA::CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	CHWDeviceOEMPA *pHWDeviceOEMPA=(CHWDeviceOEMPA *)pAcquisitionParameter;
	mCHWDeviceOEMPA *pmHWDevice=NULL;
	int iFMCCount;
	wchar_t wcName[50];

	if(!pHWDeviceOEMPA)
		return NULL;
	pHWDeviceOEMPA->GetDerivedClass(50,wcName,(void*&)pmHWDevice);
	if(wcscmp(wcName,L"mCHWDeviceOEMPA") || !pmHWDevice)
		return NULL;
	switch(eStep)
	{
	default: break;
	case eWriteHW_Enter:
		EnterCriticalSection(&pmHWDevice->m_AcquisitionAscan);
		if(pRoot->iCycleCount>pmHWDevice->m_iOEMPACycleCountMax)
			pRoot->iCycleCount = pmHWDevice->m_iOEMPACycleCountMax;
		if(pRoot->iCycleCount>=0)
		{
			pmHWDevice->m_iCycleCount = pRoot->iCycleCount;
			pmHWDevice->m_iHWCycleCount = pRoot->iCycleCount;
		}
		pmHWDevice->m_bEnableFMC = pRoot->bEnableFMC;
		pmHWDevice->m_iFMCElementStart = pRoot->iFMCElementStart;
		pmHWDevice->m_iFMCElementStop = pRoot->iFMCElementStop;
		pmHWDevice->m_iFMCElementStep = pRoot->iFMCElementStep;
		switch(pRoot->eTriggerMode)
		{
		default:			pmHWDevice->m_dAcquisitionStep = 0.0;break;
		case eOEMPAInternal:
		case eOEMPAExternalCycle:
		case eOEMPAExternalSequence:
		case eOEMPAExternalCycleSequence:
		case eOEMPAEncoder:	pmHWDevice->m_dAcquisitionStep = pRoot->dTriggerEncoderStep;break;
		}
		switch(pRoot->eRequestIO)
		{
		case eOEMPAOnDigitalInputOnly:
		case eOEMPANotRequired:					pmHWDevice->m_dAcquisitionStep = 0.0;break;
		case eOEMPAOnCycleOnly:
		case eOEMPAOnSequenceOnly:
		case eOEMPAOnDigitalInputAndCycle:
		case eOEMPAOnDigitalInputAndSequence:	break;
		}
		if(pRoot->bEnableFMC)
		{
			iFMCCount = (pRoot->iFMCElementStop-pRoot->iFMCElementStart+1);
			if(pRoot->iFMCElementStep!=1)
				iFMCCount = ((iFMCCount-1)/pRoot->iFMCElementStep)+1;
			pmHWDevice->m_iCycleCount = pRoot->iCycleCount*iFMCCount;
		}
		for(int iCycleIndex=0;iCycleIndex<pRoot->iCycleCount;iCycleIndex++)
		{
			pmHWDevice->m_peBitSize[iCycleIndex] = (enumBitSize)pRoot->eAscanBitSize;
			pmHWDevice->m_pbSigned[iCycleIndex] = (pCycle[iCycleIndex].eRectification==eSigned?true:false);
		}
#ifdef _DEVICE_OEMPA_256_256_
		mCHWDeviceOEMPA256::CallbackCustomizedAPI(pAcquisitionParameter,pFileName,eStep,pRoot,pCycle,pEmission,pReception);
#endif _DEVICE_OEMPA_256_256_

		LeaveCriticalSection(&pmHWDevice->m_AcquisitionAscan);
		break;
	case eWriteHW_Leave:
		for(int iCycleIndex=0;iCycleIndex<pRoot->iCycleCount;iCycleIndex++)
		{
			pmHWDevice->m_iAcquisitionAscanSize[iCycleIndex] = (int)pCycle[iCycleIndex].lPointCount;
			switch(pRoot->eAscanBitSize)
			{
			case e8Bits:
			case eLog8Bits: break;
			case e12Bits:
			case e16Bits: pmHWDevice->m_iAcquisitionAscanSize[iCycleIndex] *= 2;break;
			}
			if(pmHWDevice->m_iAcquisitionAscanSize[iCycleIndex]!=pmHWDevice->m_iAcquisitionAscanSize[0])
			{
				wchar_t msg[MAX_PATH];
				wsprintf(msg,L"Warning Cycle %d: the PointCount is %d, this is different than the value of the first cycle %d!",iCycleIndex,pmHWDevice->m_iAcquisitionAscanSize[iCycleIndex],pmHWDevice->m_iAcquisitionAscanSize[0]);
				UTKernel_SystemMessageBoxList(msg);
			}
		}
		break;
	}
	return NULL;
}

double mCHWDeviceOEMPA::GetAcquisitionStep()
{
	return m_dAcquisitionStep;
}
int mCHWDeviceOEMPA::GetSWCycleCount()
{
	return m_iCycleCount;
}
int mCHWDeviceOEMPA::GetHWCycleCount()
{
	return m_iHWCycleCount;
}
bool mCHWDeviceOEMPA::IsFullMatrixCapture(int &iElementStart,int &iElementStop,int &iElementStep)
{
	iElementStart = m_iFMCElementStart;
	iElementStop = m_iFMCElementStop;
	iElementStep = m_iFMCElementStep;
	return m_bEnableFMC;
}
void mCHWDeviceOEMPA::ResetCounters()
{
	if(m_hwDeviceOEMPA.GetSWDevice())
		m_hwDeviceOEMPA.GetSWDevice()->ResetCounters();
	InterlockedExchange64((volatile long long *)&m_ullAcquisitionTotalAscanCount,0);
	InterlockedExchange64((volatile long long *)&m_ullAcquisitionTotalByteCount,0);
	if(m_bFifo)
	{
#ifdef _WIN64
		if(m_pFifoAscan)
			m_pFifoAscan->ResetCounters();
		if(m_pFifoCscan)
			m_pFifoCscan->ResetCounters();
#endif _WIN64
	}
}
int mCHWDeviceOEMPA::GetLostAscan()
{
	return m_hwDeviceOEMPA.GetSWDevice()->GetLostCountAscan();
}
int mCHWDeviceOEMPA::GetLostCscan()
{
	return m_hwDeviceOEMPA.GetSWDevice()->GetLostCountCscan();
}
int mCHWDeviceOEMPA::GetLostEncoder()
{
	return m_hwDeviceOEMPA.GetSWDevice()->GetLostCountEncoder();
}
int mCHWDeviceOEMPA::GetLostUSB3()
{
	return m_hwDeviceOEMPA.GetSWDevice()->GetLostCountUSB3();
}
bool mCHWDeviceOEMPA::GetAcquisitionAscanFifoStatus(int &iAscanCount, int &iDataLost, __int64 &iTotalCount, __int64 &iTotalByteCount)
{
	if(m_bFifo)
	{
#ifdef _WIN64
		iAscanCount = m_pFifoAscan->GetCount();
		iDataLost = m_pFifoAscan->GetLost();
		iTotalCount = m_pFifoAscan->GetTotalCount();
		iTotalByteCount = m_pFifoAscan->GetTotalByteCount();
#endif _WIN64
	}else{
		iAscanCount = 0;
		iDataLost = 0;
		iTotalCount = 0;
		iTotalByteCount = 0;
	}
	return true;
}

bool mCHWDeviceOEMPA::GetAcquisitionAscanCount(__int64 &i64Count, __int64 &i64TotalByte)
{
#ifdef _WIN64
	DWORD dwCount;
#endif _WIN64

	if(!m_bFifo)
	{
		i64Count = m_ullAcquisitionTotalAscanCount;
		i64TotalByte = m_ullAcquisitionTotalByteCount;
	}else{
#ifdef _WIN64
		dwCount = (DWORD)m_pFifoAscan->GetTotalCount();
#endif _WIN64
	}
	return true;
}

bool mCHWDeviceOEMPA::GetAcquisitionAscanEncoder(int iEncoderIndex,double &dEncoder,long &lRawEncoder)
{
	if(iEncoderIndex<0)
		return false;
	if(iEncoderIndex>=g_iDriverEncoderCountMax)
		return false;
	if(!m_bFifo)
	{
		EnterCriticalSection(&m_AcquisitionAscan);
		lRawEncoder = m_lAcquisitionAscanRawEncoder[iEncoderIndex];
		dEncoder = m_dAcquisitionAscanEncoder[iEncoderIndex];
		LeaveCriticalSection(&m_AcquisitionAscan);
		return true;
	}else{
#ifdef _WIN64
		int iIndexTail, iIndexHead;
		structAcqInfoEx *pAcqInfo;
		CStream_0x0001 *pStreamHeader;
		CSubStreamAscan_0x0103 *pAscanHeader;
		void *pBufferMax,*pBufferMin,*pBufferSat;
		if(m_pFifoAscan->GetItemLimit(iIndexTail, iIndexHead)<2)
			return false;
		m_pFifoAscan->DecrementItemIndex(iIndexHead);
		if(!m_pFifoAscan->OutAscan(iIndexHead,true,pAcqInfo,pStreamHeader,pAscanHeader,pBufferMax,pBufferMin,pBufferSat))
			return false;
		if(!pAcqInfo)
			return false;
		lRawEncoder = pAcqInfo->lEncoder[iEncoderIndex];
		dEncoder = pAcqInfo->dEncoder[iEncoderIndex];
#endif _WIN64
	}
	return false;
}

DWORD mCHWDeviceOEMPA::GetAcquisitionAscanSize(int iCycle)
{
	DWORD dwSize;

	//if(m_bFifo)
	//	return 0;
	if(iCycle<0)
		return 0;
	if(iCycle>=m_iCycleCount)
		return 0;
	dwSize = (DWORD)m_iAcquisitionAscanSize[iCycle];
	return dwSize;
}

bool mCHWDeviceOEMPA::SetAcquisitionAscanSize(int iCycle,int iSize)
{
	if(iCycle<0)
		return false;
	if(iCycle>=m_iCycleCount)
		return false;
	m_iAcquisitionAscanSize[iCycle] = iSize;
	return true;
}

enumBitSize mCHWDeviceOEMPA::GetAcquisitionAscanBitSize(int iCycle, bool &bSigned)
{
	enumBitSize eBitSize;

	if(iCycle<0)
		return e8Bits;
	if(iCycle>=m_iCycleCount)
		return e8Bits;
	eBitSize = m_peBitSize[iCycle];
	bSigned = m_pbSigned[iCycle];
	return eBitSize;
}

bool mCHWDeviceOEMPA::GetAcquisitionAscanData(int iCycle,int iSizeMax,BYTE *pBuffer,int &iSize)
{
	bool bRet=true;

	if(m_bFifo)
		return false;
	if(iCycle<0)
		return false;
	if(iCycle>=m_iCycleCount)
		return false;
	iSize = 0;
	EnterCriticalSection(&m_AcquisitionAscan);
	if(m_iAcquisitionAscanSize)
	{
		iSize = m_iAcquisitionAscanSize[iCycle];
		if(iSize>iSizeMax)
		{
			iSize = iSizeMax;
			bRet = false;
		}
		memcpy(pBuffer,&m_pbyAcquisitionAscanBuffer[iCycle*m_iAscanPointCount],iSize);
	}
	LeaveCriticalSection(&m_AcquisitionAscan);
	return bRet;
}

bool mCHWDeviceOEMPA::GetAcquisitionAscanFifoItem(long long sequence, int iCycle, int &iItem)
{
#ifdef _WIN64
	if(!m_bFifo)
		return false;
	if(sequence==-1)
		iItem = m_pFifoAscan->GetFifoItem(sequence, iCycle, -1, -1);
	else
		iItem = m_pFifoAscan->GetFifoItem(iCycle, -1, -1);
	if(iItem<0)
		return false;
	return true;
#else _WIN64
	return false;
#endif _WIN64
}
bool mCHWDeviceOEMPA::GetAcquisitionAscanLifoItem(long long sequence, int iCycle, int &iItem)
{
#ifdef _WIN64
	if(!m_bFifo)
		return false;
	if(sequence==-1)
		iItem = m_pFifoAscan->GetLifoItem(sequence, iCycle, -1, -1);
	else
		iItem = m_pFifoAscan->GetLifoItem(iCycle, -1, -1);
	if(iItem<0)
		return false;
	return true;
#else _WIN64
	return false;
#endif _WIN64
}
bool mCHWDeviceOEMPA::GetAcquisitionAscanItemInc(int &iItem)
{
#ifdef _WIN64
	if(!m_bFifo)
		return false;
	m_pFifoAscan->IncrementItemIndex(iItem);
	return true;
#else _WIN64
	return false;
#endif _WIN64
}
bool mCHWDeviceOEMPA::GetAcquisitionAscanItemDec(int &iItem)
{
#ifdef _WIN64
	if(!m_bFifo)
		return false;
	m_pFifoAscan->DecrementItemIndex(iItem);
	return true;
#else _WIN64
	return false;
#endif _WIN64
}
bool mCHWDeviceOEMPA::GetAcquisitionAscanFifoInfo(int iItem, int &iCycle, long long &llSequence, int &iPointCount, BYTE &byByteSize, bool &bSigned)
{
#ifdef _WIN64
	structAcqInfoEx *pAcqInfo;
	CStream_0x0001 *pStreamHeader;
	CSubStreamAscan_0x0103 *pAscanHeader;
	void *pBufferMax,*pBufferMin,*pBufferSat;

	if(!m_pFifoAscan->OutAscan(iItem,true,pAcqInfo,pStreamHeader,pAscanHeader,pBufferMax,pBufferMin,pBufferSat))
		return false;
	iCycle = pAscanHeader->cycle;
	llSequence = MAKE_i64(pAscanHeader->seqHigh, pAscanHeader->seqLow);
	iPointCount = (int)pAscanHeader->dataCount;
	byByteSize = pAscanHeader->dataSize;
	if(pAscanHeader->sign)
		bSigned = true;
	else
		bSigned = false;
	return true;
#else _WIN64
	return false;
#endif _WIN64
}
int mCHWDeviceOEMPA::GetAcquisitionIndex(bool bAscan, bool bFifo, int iCycleCount, int *piCycleIn, int *piItem, int *piCycle, long long *pllSequence, int *piPointCount, BYTE *pbyByteSize, bool *pbSigned)
{
#ifdef _WIN64
	bool bRet=true;
	structAcqInfoEx *pAcqInfo;
	CStream_0x0001 *pStreamHeader;
	CSubStreamAscan_0x0103 *pAscanHeader;
	const CSubStreamCscan_0x0X02 *pCscanHeader;
	const structCscanAmp_0x0102 *pBufferAmp;
	const structCscanAmpTof_0x0202 *pBufferAmpTof;
	void *pBufferMax,*pBufferMin,*pBufferSat;
	int iIndexTail, iIndexHead, iItem, iSequenceCount, iCycle, iCycleIn, iCycleIndex, iCycleIndex2;
	long long llSequence;
	enumAcquisitionFifo eFifo;
	CAcquisitionFifo *pFifo;

	if(!m_bFifo)
		return 0;
	if(bAscan)
	{
		eFifo = eFifoAscan;
		pFifo = m_pFifoAscan;
	}else{
		eFifo = eFifoCscan;
		pFifo = m_pFifoCscan;
	}
	if(!pFifo)
		return false;
if(false)
pFifo->DumpFifo(L"C:\\Users\\Public\\dump.txt");
	if(!piItem)
		return 0;
	if(!piCycle)
		return 0;
	if(!pllSequence)
		return 0;
	if(bAscan)
	{
		if(!piPointCount)
			return 0;
		if(!pbyByteSize)
			return 0;
		if(!pbSigned)
			return 0;
	}
	if(iCycleCount<=0)
		return 0;
	if(pFifo->GetItemLimit(iIndexTail, iIndexHead)<=0)
		return 0;
	iSequenceCount = 0;
	if(bFifo)
		iItem = iIndexTail;
	else{
		iItem = iIndexHead;//Lifo
		pFifo->DecrementItemIndex(iItem);
	}
	iCycleIndex2 = 0;
	while(iCycleIndex2<iCycleCount)
	{
		if(bFifo)
			iCycleIndex = iCycleIndex2;
		else
			iCycleIndex = iCycleCount-iCycleIndex2-1;//Lifo
		iCycleIn = piCycleIn[iCycleIndex];
		if(bAscan)
		{
			if(!pFifo->OutAscan(iItem,true,pAcqInfo,pStreamHeader,pAscanHeader,pBufferMax,pBufferMin,pBufferSat))
				break;
			iCycle = pFifo->_CheckCycleFMC((int)pAscanHeader->cycle,*pAcqInfo);
			llSequence = MAKE_i64(pAscanHeader->seqHigh, pAscanHeader->seqLow);
		}else{
			if(!pFifo->OutCscan(iItem,true,pAcqInfo,pStreamHeader,pCscanHeader,pBufferAmp,pBufferAmpTof))
				break;
			iCycle = pFifo->_CheckCycleFMC((int)pCscanHeader->cycle,*pAcqInfo);
			llSequence = MAKE_i64(pCscanHeader->seqHigh, pCscanHeader->seqLow);
		}
		if(!iCycleIndex2 && !bFifo && (iCycle!=iCycleIn))
			goto next_item;//look for the last cycle.
		if((bFifo && (iCycle>iCycleIn)) || (!bFifo && (iCycle<iCycleIn)))
		{//filling
			piItem[iCycleIndex] = -2;//means missing data.
			piCycle[iCycleIndex] = iCycleIn;
			pllSequence[iCycleIndex] = 0;
			if(bAscan)
			{
				piPointCount[iCycleIndex] = 0;
				pbyByteSize[iCycleIndex] = 0;
				pbSigned[iCycleIndex] = 0;
			}
			iCycleIndex2++;
			continue;
		}else
		if(iCycle==iCycleIn)
		{
			piItem[iCycleIndex] = iItem;
			piCycle[iCycleIndex] = iCycle;
			pllSequence[iCycleIndex] = llSequence;
			if(bAscan)
			{
				piPointCount[iCycleIndex] = (int)pAscanHeader->dataCount;
				pbyByteSize[iCycleIndex] = pAscanHeader->dataSize;
				if(pAscanHeader->sign)
					pbSigned[iCycleIndex] = true;
				else
					pbSigned[iCycleIndex] = false;
			}
			iCycleIndex2++;
		}//else next_item
next_item:
		if(bFifo)
		{
			pFifo->IncrementItemIndex(iItem);
			if(iItem==iIndexHead)
				break;
		}else{
			if(iItem==iIndexTail)
				break;
			pFifo->DecrementItemIndex(iItem);
		}
	}
	return iCycleIndex2;
#else _WIN64
	return 0;
#endif _WIN64
}
bool mCHWDeviceOEMPA::GetAcquisitionAscanFifoData(bool bPeek, int iItemCount, int *pItem, int iSizeMax, BYTE *pBuffer, int &iSize, int *pCycle, LONGLONG *pSequence, int iSizeEncoder, double *pdEncoder, long *plRawEncoder)
{
#ifdef _WIN64
	bool bRet=true,bFirst=false;
	structAcqInfoEx *pAcqInfo;
	CStream_0x0001 *pStreamHeader;
	CSubStreamAscan_0x0103 *pAscanHeader;
	void *pBufferMax,*pBufferMin,*pBufferSat;
	BYTE *pBuffer2=pBuffer;
	int iItem,iCycle,iPointCount,iByteSize,iSigned;
	long long sequence;
	CAcquisitionFifo *pFifo;

	if(!m_bFifo)
		return false;
	pFifo = m_pFifoAscan;
	if(!pFifo)
		return false;
	if(iItemCount<0)
		return false;
	for(int iIndex=0;iIndex<iItemCount;iIndex++)
	{
		iItem = pItem[iIndex];
		if(iItem>=-1)
		{
			if(!pFifo->OutAscan(iItem,bPeek,pAcqInfo,pStreamHeader,pAscanHeader,pBufferMax,pBufferMin,pBufferSat))
				return false;//error
			if(!pAscanHeader)
				return false;
			iCycle = pFifo->_CheckCycleFMC((int)pAscanHeader->cycle,*pAcqInfo);
			sequence = MAKE_i64(pAscanHeader->seqHigh,pAscanHeader->seqLow);
			pCycle[iIndex] = iCycle;
			pSequence[iIndex] = sequence;
			iSize = (int)(pAscanHeader->dataCount*pAscanHeader->dataSize);
			if(!bFirst)
			{
				bFirst = true;
				iPointCount = (int)(pAscanHeader->dataCount);
				iByteSize = (int)(pAscanHeader->dataSize);
				iSigned = (int)pAscanHeader->sign;
			}else{
				if(iPointCount != (int)(pAscanHeader->dataCount))
					{bRet = false;break;}
				if(iByteSize != (int)(pAscanHeader->dataSize))
					{bRet = false;break;}
				if(iSigned != (int)pAscanHeader->sign)
					{bRet = false;break;}
			}
			if(iSize>iSizeMax/iItemCount)
			{
				iSize = iSizeMax/iItemCount;
				bRet = false;
			}
			if(pBuffer2 && pBufferMax)
				memcpy(pBuffer2,pBufferMax,iSize);
			if(pBuffer2 && !pBufferMax)
				bRet = false;
			for(int iIndex2=0;iIndex2<iSizeEncoder;iIndex2++)
			{
				if(plRawEncoder)
					plRawEncoder[iIndex*iSizeEncoder+iIndex2] = pAcqInfo->lEncoder[iIndex2];
				else
					plRawEncoder[iIndex*iSizeEncoder+iIndex2] = 0;
				if(pdEncoder)
					pdEncoder[iIndex*iSizeEncoder+iIndex2] = pAcqInfo->dEncoder[iIndex2];
				else
					pdEncoder[iIndex*iSizeEncoder+iIndex2] = 0.0;
			}
		}else
		if(iItem==-2)
		{
			iSize = iSizeMax/iItemCount;
			pCycle[iIndex] = -1;
			pSequence[iIndex] = -1;
			memset(pBuffer2,0,iSize);
			for(int iIndex2=0;iIndex2<iSizeEncoder;iIndex2++)
			{
				if(plRawEncoder)
					plRawEncoder[iIndex*iSizeEncoder+iIndex2] = plRawEncoder[iIndex2];
				else
					plRawEncoder[iIndex*iSizeEncoder+iIndex2] = 0;
				if(pdEncoder)
					pdEncoder[iIndex*iSizeEncoder+iIndex2] = pdEncoder[iIndex2];
				else
					pdEncoder[iIndex*iSizeEncoder+iIndex2] = 0.0;
			}
		}else
			return false;
		pBuffer2 += iSize;
	}
	return bRet;
#else _WIN64
	return false;
#endif _WIN64
}
bool mCHWDeviceOEMPA::GetAcquisitionCscanData(bool bFifo, bool bPeek, int iItemCount, int *pItem, BYTE *pAmp, short *psTof, int *pCycle, LONGLONG *pSequence, int iSizeEncoder, double *pdEncoder, long *plRawEncoder)
{
#ifdef _WIN64
	bool bRet=true;
	structAcqInfoEx *pAcqInfo;
	CStream_0x0001 *pStreamHeader;
	const CSubStreamCscan_0x0X02 *pCscanHeader;
	const structCscanAmp_0x0102 *pBufferAmp;
	const structCscanAmpTof_0x0202 *pBufferAmpTof;
	int iItem,iCycle;
	long long sequence;
	unsigned int uiGateId;
	CAcquisitionFifo *pFifo;

	if(!m_bFifo)
		return false;
	pFifo = m_pFifoCscan;
	if(!pFifo)
		return false;
	if(iItemCount<0)
		return false;
	for(int iIndex=0;iIndex<iItemCount;iIndex++)
	{
		iItem = pItem[iIndex];
		if(iItem>=-1)
		{
			if(!pFifo->OutCscan(iItem,bPeek,pAcqInfo,pStreamHeader,pCscanHeader,pBufferAmp,pBufferAmpTof))
				return false;//error
			if(!pCscanHeader)
				return false;
			iCycle = pFifo->_CheckCycleFMC((int)pCscanHeader->cycle,*pAcqInfo);
			sequence = MAKE_i64(pCscanHeader->seqHigh,pCscanHeader->seqLow);
			pCycle[iIndex] = iCycle;
			pSequence[iIndex] = sequence;
			if(pCscanHeader->Check() && pBufferAmp)
			{
				for(int iIndex2=0;iIndex2<(int)pCscanHeader->count;iIndex2++)
				{
					uiGateId = pBufferAmp[iIndex2].gateId;
					if(uiGateId>=g_iOEMPAGateCountMax)
						continue;
					if(pAmp)
						pAmp[iIndex*g_iOEMPAGateCountMax+uiGateId] = pBufferAmp[iIndex2].byAmp;
				}
			}
			if(pCscanHeader->Check() && pBufferAmpTof)
			{
				for(int iIndex2=0;iIndex2<(int)pCscanHeader->count;iIndex2++)
				{
					uiGateId = pBufferAmpTof[iIndex2].gateId;
					if(uiGateId>=g_iOEMPAGateCountMax)
						continue;
					if(pAmp)
						pAmp[iIndex*g_iOEMPAGateCountMax+uiGateId] = pBufferAmpTof[iIndex2].byAmp;
					if(psTof)
						psTof[iIndex*g_iOEMPAGateCountMax+uiGateId] = pBufferAmpTof[iIndex2].wTof;
				}
			}
			for(int iIndex2=0;iIndex2<iSizeEncoder;iIndex2++)
			{
				if(plRawEncoder)
					plRawEncoder[iIndex*iSizeEncoder+iIndex2] = pAcqInfo->lEncoder[iIndex2];
				else
					plRawEncoder[iIndex*iSizeEncoder+iIndex2] = 0;
				if(pdEncoder)
					pdEncoder[iIndex*iSizeEncoder+iIndex2] = pAcqInfo->dEncoder[iIndex2];
				else
					pdEncoder[iIndex*iSizeEncoder+iIndex2] = 0.0;
			}
		}else
		if(iItem==-2)
		{
			pCycle[iIndex] = -1;
			pSequence[iIndex] = -1;
			for(int iGateId=0;iGateId<g_iOEMPAGateCountMax;iGateId++)
			{
				if(pAmp)
					pAmp[iIndex*g_iOEMPAGateCountMax+iGateId] = 0;
				if(psTof)
					psTof[iIndex*g_iOEMPAGateCountMax+iGateId] = 0;
			}
			for(int iIndex2=0;iIndex2<iSizeEncoder;iIndex2++)
			{
				if(plRawEncoder)
					plRawEncoder[iIndex*iSizeEncoder+iIndex2] = plRawEncoder[iIndex2];
				else
					plRawEncoder[iIndex*iSizeEncoder+iIndex2] = 0;
				if(pdEncoder)
					pdEncoder[iIndex*iSizeEncoder+iIndex2] = pdEncoder[iIndex2];
				else
					pdEncoder[iIndex*iSizeEncoder+iIndex2] = 0.0;
			}
		}else
			return false;
	}
	return bRet;
#else _WIN64
	return false;
#endif _WIN64
}
bool mCHWDeviceOEMPA::FlushAcquisitionAscanFifo()
{
#ifdef _WIN64
	return m_pFifoAscan->RemoveAll();
#else _WIN64
	return false;
#endif _WIN64
}
bool mCHWDeviceOEMPA::FlushAcquisitionCscanFifo()
{
#ifdef _WIN64
	return m_pFifoCscan->RemoveAll();
#else _WIN64
	return false;
#endif _WIN64
}
bool mCHWDeviceOEMPA::DumpAcquisitionAscanFifo(const wchar_t *wcFileName)
{
#ifdef _WIN64
	return m_pFifoAscan->DumpFifo(wcFileName);
#else _WIN64
	return false;
#endif _WIN64
}

bool mCHWDeviceOEMPA::GetAcquisitionCscanCount(__int64 &i64Count)
{
	if(!m_bFifo)
	{
		i64Count = m_ullAcquisitionTotalCscanCount;
	}else{
#ifdef _WIN64
		i64Count = m_pFifoCscan->GetTotalCount();
#else _WIN64
		i64Count = 0;
#endif _WIN64
	}
	return true;
}

bool mCHWDeviceOEMPA::GetAcquisitionCscanEncoder(int iEncoderIndex,double &dEncoder,long &lRawEncoder)
{
	if(m_bFifo)
		return false;
	if(iEncoderIndex<0)
		return false;
	if(iEncoderIndex>=g_iDriverEncoderCountMax)
		return false;
	EnterCriticalSection(&m_AcquisitionCscan);
	lRawEncoder = m_lAcquisitionCscanRawEncoder[iEncoderIndex];
	dEncoder = m_dAcquisitionCscanEncoder[iEncoderIndex];
	LeaveCriticalSection(&m_AcquisitionCscan);
	return true;
}

bool mCHWDeviceOEMPA::GetAcquisitionCscanEnable(int iGate,int iSizeMax,BYTE *pBuffer,int &iSize)
{
	if(m_bFifo)
		return false;
	if(m_iCycleCount<=0)
		return false;
	iSize = m_iCycleCount;
	if(iSize>iSizeMax)
		iSize = iSizeMax;
	EnterCriticalSection(&m_AcquisitionCscan);
	memcpy(pBuffer,&m_pbAcquisitionCscanEnable[iGate][0],iSize*sizeof(BYTE));
	LeaveCriticalSection(&m_AcquisitionCscan);
	return true;
}

bool mCHWDeviceOEMPA::GetAcquisitionCscanAmplitude(int iGate,int iSizeMax,BYTE *pBuffer,int &iSize)
{
	bool bRet=true;

	if(m_bFifo)
		return false;
	if(m_iCycleCount<=0)
		return false;
	iSize = m_iCycleCount;
	if(iSize>iSizeMax)
		iSize = iSizeMax;
	EnterCriticalSection(&m_AcquisitionCscan);
	memcpy(pBuffer,&m_pbyAcquisitionCscanAmplitude[iGate][0],iSize*sizeof(BYTE));
	LeaveCriticalSection(&m_AcquisitionCscan);
	return bRet;
}

bool mCHWDeviceOEMPA::GetAcquisitionCscanTimeOfFlight(int iGate,int iSizeMax,short *pBuffer,int &iSize)
{
	bool bRet=true;

	if(m_bFifo)
		return false;
	if(m_iCycleCount<=0)
		return false;
	iSize = m_iCycleCount;
	if(iSize>iSizeMax)
		iSize = iSizeMax;
	EnterCriticalSection(&m_AcquisitionCscan);
	memcpy(pBuffer,&m_psAcquisitionCscanTimeOfFlight[iGate][0],iSize*sizeof(short));
	LeaveCriticalSection(&m_AcquisitionCscan);
	return bRet;
}

//bool mCHWDeviceOEMPA::GetAcquisitionCscanFifoData(bool bPeek, int iCycle, int iSizeGateCount, BYTE *pAmp, int &iAmp, short *psTof, int &iTof, int iSizeEncoder, double *pdEncoder, long *plRawEncoder, int &iEncoder, LONGLONG &sequence)
//{
//#ifdef _WIN64
//	structAcqInfoEx *pAcqInfo;
//	CStream_0x0001 *pStreamHeader;
//	const CSubStreamCscan_0x0X02 *pCscanHeader;
//	const structCscanAmp_0x0102 *pBufferAmp;
//	const structCscanAmpTof_0x0202 *pBufferAmpTof;
//	unsigned int uiGateId;
//	int iItem;
//
//	iAmp = iTof = iEncoder = 0;
//	sequence = 0;
//	if(!m_bFifo)
//		return false;
//	if(iSizeGateCount<0)
//		return false;
//	if(iSizeGateCount>4)
//		return false;
//	if(iSizeEncoder<0)
//		return false;
//	if(iSizeEncoder>g_iDriverEncoderCountMax)
//		return false;
//	iItem = m_pFifoCscan->GetFifoItem(iCycle,-1,-1);
//	if(iItem<0)
//		return false;//error
//	if(!m_pFifoCscan->OutCscan(iItem,bPeek,pAcqInfo,pStreamHeader,pCscanHeader,pBufferAmp,pBufferAmpTof))
//		return false;//error
//	if(!pAcqInfo)
//		return false;//error
//	sequence = MAKE_i64(pCscanHeader->seqHigh,pCscanHeader->seqLow);
//	iEncoder = iSizeEncoder;
//	for(int iIndex=0;iIndex<iSizeEncoder;iIndex++)
//	{
//		if(plRawEncoder)
//			plRawEncoder[iIndex] = pAcqInfo->lEncoder[iIndex];
//		if(pdEncoder)
//			pdEncoder[iIndex] = pAcqInfo->dEncoder[iIndex];
//	}
//	if(pCscanHeader->Check() && pBufferAmp)
//	{
//		iAmp = (int)pCscanHeader->count;
//		for(int iIndex=0;iIndex<(int)pCscanHeader->count;iIndex++)
//		{
//			uiGateId = pBufferAmp[iIndex].gateId;
//			if(uiGateId>=g_iOEMPAGateCountMax)
//				continue;
//			if(((int)uiGateId<iSizeGateCount) && pAmp)
//				pAmp[uiGateId] = pBufferAmp[iIndex].byAmp;
//		}
//	}
//	if(pCscanHeader->Check() && pBufferAmpTof)
//	{
//		iAmp = iTof = (int)pCscanHeader->count;
//		for(int iIndex=0;iIndex<(int)pCscanHeader->count;iIndex++)
//		{
//			uiGateId = pBufferAmpTof[iIndex].gateId;
//			if(uiGateId>=g_iOEMPAGateCountMax)
//				continue;
//			if(((int)uiGateId<iSizeGateCount) && pAmp)
//				pAmp[uiGateId] = pBufferAmpTof[iIndex].byAmp;
//			if(((int)uiGateId<iSizeGateCount) && psTof)
//				psTof[uiGateId] = pBufferAmpTof[iIndex].wTof;
//		}
//	}
//	return true;
//#else _WIN64
//	return false;
//#endif _WIN64
//}
bool mCHWDeviceOEMPA::GetAcquisitionCscanFifoStatus(int &iCscanCount, int &iDataLost)
{
#ifdef _WIN64
	if(m_bFifo)
	{
		iCscanCount = m_pFifoCscan->GetCount();
		iDataLost = m_pFifoCscan->GetLost();
	}else{
		iCscanCount = 0;
		iDataLost = 0;
	}
	return true;
#else _WIN64
	return false;
#endif _WIN64
}

mCHWDeviceOEMPA* mCHWDeviceOEMPA::mGetHWDevice(int iDeviceId)
{
	CHWDevice *pHWDevice=CHWDevice::GetHWDevice(iDeviceId);
	mCHWDeviceOEMPA *mpHWDeviceOEMPA;

	if(!pHWDevice)
		return NULL;
	mpHWDeviceOEMPA = static_cast<mCHWDeviceOEMPA*>(pHWDevice->GetAcquisitionParameter());
	if(!mpHWDeviceOEMPA)
		return NULL;
	return mpHWDeviceOEMPA;
}

CHWDeviceOEMPA* mCHWDeviceOEMPA::GetHWDevice(int iDeviceId)
{
	CHWDevice *pHWDevice=CHWDevice::GetHWDevice(iDeviceId);
	CHWDeviceOEMPA *pHWDeviceOEMPA;

	if(!pHWDevice)
		return NULL;
	pHWDeviceOEMPA = dynamic_cast<CHWDeviceOEMPA*>(pHWDevice);
	if(!pHWDeviceOEMPA)
		return NULL;
	return pHWDeviceOEMPA;
}

bool mCHWDeviceOEMPA::Example1()
{
	return true;
}

bool mCHWDeviceOEMPA::Example2(int iSizeIn, double *pDataIn,int *piSizeOut, double *pDataOut)
{
	if(!piSizeOut)
		return false;
	if(*piSizeOut>iSizeIn)
		*piSizeOut = iSizeIn;
	for(int i=0;i<*piSizeOut;i++)
		pDataOut[i] = (pDataIn[i]+1)*2;
	return true;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

