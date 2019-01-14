#include "stdafx.h"
#include "mHWDeviceOEMPA.h"
#ifdef _DEVICE_OEMPA_256_256_

#define MAKE_i64(hi, lo)    (  (LONGLONG(DWORD(hi) & 0xffffffff) << 32 ) | LONGLONG(DWORD(lo) & 0xffffffff)  )


BYTE g_pOutHeader[sizeof(CSubStreamAscan_0x0103)+g_iAscanPointCount256*2];//multiplied by 2 in case of 16 bits data.
BYTE *g_pOut=&g_pOutHeader[sizeof(CSubStreamAscan_0x0103)];
long long g_llSequence=-1;
int g_iCycleCount=-1;
const int g_iMergePreviousAscanCountMin=10;
int g_iMergePreviousAscanCountMin2=g_iMergePreviousAscanCountMin;
bool g_bStartFirstSequenceMaster=true;
bool g_bStartFirstSequenceSlave=true;
int g_iDeviceIdMaster=-1, g_iDeviceIdSlave=-1;
//int g_iDebug=0;
//extern "C"{
//__declspec(dllexport) bool mxEnableShot256(int iDeviceIdMaster,int iDeviceIdSlave, bool bValue);
//}

DWORD WINAPI mCHWDeviceOEMPA256::ThreadAcquisitionFifo(LPVOID pParam)
{
#ifdef _WIN64
	CFifo256 *pFifoOut=(CFifo256*)pParam;
	HANDLE hObject[2];
	DWORD dwReturnValue=WAIT_OBJECT_0;
	bool bError;

	do{
		hObject[0] = pFifoOut->m_pFifo->GetEvent();//FifoOutput.m_Fifo.GetEvent();
		//dwReturnValue = WaitForMultipleObjects(1,hObject,FALSE,100);
		pFifoOut->Check();//FifoOutput.Check();
		if(pFifoOut->m_pFifo->GetExit())
			break;
		switch(dwReturnValue)
		{
		case WAIT_ABANDONED_0:
		case WAIT_ABANDONED_0+1:
			bError = true;
			break;
		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0+1:
			bError = false;
			pFifoOut->MergePrevious();
			break;
		case WAIT_TIMEOUT:
			break;
		case WAIT_FAILED:
			bError = true;
			break;
		}
	}while(1);
	pFifoOut->m_pFifo->Exit();
#endif _WIN64
	return 0;
}
int CFifo256::MergePrevious()
{//add ascan from the 2 devices AND/OR SORT them before to fill the output fifo.
#ifdef _WIN64
	CStream_0x0001 *pStreamHeader1,*pStreamHeader2;
	structAcqInfoEx *pAcqInfo1,*pAcqInfo2;
	CSubStreamAscan_0x0103 AscanHeader1,AscanHeader2,*pAscanHeader1,*pAscanHeader2;
    int iItemCount1,iItemCount2,iItemTail1,iItemTail2,iItemHead1,iItemHead2;
    int _iItemCount1,_iItemCount2,_iItemTail1,_iItemTail2,_iItemHead1,_iItemHead2;
    LONGLONG sequence1,sequence2,sequence;
	LONGLONG _sequence1,_sequence2;
    int iCycle1,iCycle2,iCount=0;
    int _iCycle1,_iCycle2;
	int iItem1,iItem2,iSize;
	void *pBufferMax1,*pBufferMin1,*pBufferSat1;
	void *pBufferMax2,*pBufferMin2,*pBufferSat2;
	bool bData1,bData2,bOutput1,bOutput2;
	bool bRemove1,bRemove2;
	static bool g_bDump=false;//debugger
	CFifo256 *pFifoMaster,*pFifoSlave,*pFifoOutput;
	int g_iCycleMaxMaster,g_iCycleMaxSlave;
	int g_iCycleMinMaster,g_iCycleMinSlave;
	bool *g_abCycleSumMaster,*g_abCycleSumSlave;

	pFifoMaster = m_pmHWMaster->m_csDevice256.m_pFifoIn;
	pFifoSlave = m_pmHWSlave->m_csDevice256.m_pFifoIn;
	if(!pFifoMaster || !pFifoSlave)
		return 0;
	pFifoOutput = m_pmHWMaster->m_csDevice256.m_pFifoOut;
	g_iCycleMinMaster = m_pmHWMaster->m_csDevice256.g_iCycleMin;
	g_iCycleMaxMaster = m_pmHWMaster->m_csDevice256.g_iCycleMax;
	g_abCycleSumMaster = m_pmHWMaster->m_csDevice256.g_abCycleSum;
	g_iCycleMinSlave = m_pmHWSlave->m_csDevice256.g_iCycleMin;
	g_iCycleMaxSlave = m_pmHWSlave->m_csDevice256.g_iCycleMax;
	g_abCycleSumSlave = m_pmHWSlave->m_csDevice256.g_abCycleSum;
	if(g_bDump)
	{
		pFifoMaster->DumpFifo(L"c:\\Users\\Public\\DumpFifoMaster.txt");
		pFifoSlave->DumpFifo(L"c:\\Users\\Public\\DumpFifoSlave.txt");
		pFifoOutput->DumpFifo(L"c:\\Users\\Public\\DumpFifoOutput.txt");
	}
	iItemCount1 = pFifoMaster->m_pFifo->GetItemLimit(iItemTail1, iItemHead1);
	iItemCount2 = pFifoSlave->m_pFifo->GetItemLimit(iItemTail2, iItemHead2);
	if((iItemCount1<0) || (iItemCount2<0))
		return 0;
	if(((g_iCycleMinMaster>=0) && (iItemCount1<=g_iMergePreviousAscanCountMin2))
		|| ((g_iCycleMinSlave>=0) && (iItemCount2<=g_iMergePreviousAscanCountMin2)))
		return 0;//if one device has no item but should have => we have to wait for more data.
//if(g_iDebug>=2)
//{
//pFifoMaster->DumpFifo(L"c:\\Users\\Public\\DumpFifoMaster.txt");
//pFifoSlave->DumpFifo(L"c:\\Users\\Public\\DumpFifoSlave.txt");
//pFifoOutput->DumpFifo(L"c:\\Users\\Public\\DumpFifoOutput.txt");
//mxEnableShot256(0,1,false);
//g_iDebug = 1;
//}
	if(g_iCycleMinMaster>=0)
	{
		iItem1 = iItemHead1;
		pFifoMaster->m_pFifo->DecrementItemIndex(iItem1);//Head is the index of the next last item;
		if(!pFifoMaster->m_pFifo->OutAscan(iItem1,true,pAcqInfo1,pStreamHeader1,pAscanHeader1,pBufferMax1,pBufferMin1,pBufferSat1))
			return 0;
		sequence1 = MAKE_i64(pAscanHeader1->seqHigh, pAscanHeader1->seqLow);
	}
	if(g_iCycleMinSlave>=0)
	{
		iItem2 = iItemHead2;
		pFifoSlave->m_pFifo->DecrementItemIndex(iItem2);//Head is the index of the next last item;
		if(!pFifoSlave->m_pFifo->OutAscan(iItem2,true,pAcqInfo2,pStreamHeader2,pAscanHeader2,pBufferMax2,pBufferMin2,pBufferSat2))
			return 0;
		sequence2 = MAKE_i64(pAscanHeader2->seqHigh, pAscanHeader2->seqLow);
	}
	//the maximum of the last common sequence minus one.
	//no need to wait for full sequence because of data loss.
	if((g_iCycleMinMaster>=0) && (g_iCycleMinSlave>=0))
	{
		if(sequence1>sequence2)
			sequence = sequence2-1;
		else
			sequence = sequence1-1;
	}else{
		if(g_iCycleMinMaster>=0)
			sequence = sequence1-1;
		else
			sequence = sequence2-1;
	}
	if(sequence<=0)
		return 0;
	iItem1 = iItemTail1;
	iItem2 = iItemTail2;
	bOutput1 = true;
	bOutput2 = true;
	do{
		if(g_iCycleMinMaster>=0)
		{
			bData1 = pFifoMaster->m_pFifo->OutAscan(iItem1,true,pAcqInfo1,pStreamHeader1,pAscanHeader1,pBufferMax1,pBufferMin1,pBufferSat1);
			if(!bData1)
			{
				if(iCount>0)
					break;////Not an error, if "iCount>0" it is just that all data have been used.
				else
					break;//UTKernel_SystemMessageBoxList(L"Error FifoMaster 1!");
			}else{
				sequence1 = MAKE_i64(pAscanHeader1->seqHigh, pAscanHeader1->seqLow);
				iCycle1 = pAscanHeader1->cycle;
			}
		}else{
			bData1 = false;
			pFifoMaster->m_pFifo->RemoveTail();
		}
		if(g_iCycleMinSlave>=0)
		{
			bData2 = pFifoSlave->m_pFifo->OutAscan(iItem2,true,pAcqInfo2,pStreamHeader2,pAscanHeader2,pBufferMax2,pBufferMin2,pBufferSat2);
			if(!bData2)
			{
				if(iCount>0)
					break;//Not an error, if "iCount>0" it is just that all data have been used.
				else
					break;//UTKernel_SystemMessageBoxList(L"Error FifoSlave 1!");
			}else{
				sequence2 = MAKE_i64(pAscanHeader2->seqHigh, pAscanHeader2->seqLow);
				iCycle2 = pAscanHeader2->cycle;
			}
		}else{
			bData2 = false;
			pFifoSlave->m_pFifo->RemoveTail();
		}
		bOutput1 = false;
		bOutput2 = false;
		bRemove1 = true;
		bRemove2 = true;
		if(bData1 && bData2)
		{
			if(sequence1<sequence2)
				bOutput1 = true;//first master to output.
			else
			if(sequence1>sequence2)
				bOutput2 = true;//first slave to output.
			else{//sequence1==sequence2
				if(iCycle1<iCycle2)
					bOutput1 = true;//first master to output.
				else
				if(iCycle1>iCycle2)
					bOutput2 = true;//first slave to output.
				else
				{//iCycle1==iCycle2
					if(g_abCycleSumMaster[iCycle1])
						bOutput1 = true;
					else
						bRemove1 = true;//this item should be removed but only if the current data is processed.
					if(g_abCycleSumSlave[iCycle2])
						bOutput2 = true;
					else
						bRemove2 = true;//this item should be removed but only if the current data is processed.
				}
			}
		}else{
			bOutput1 = bData1;
			bOutput2 = bData2;
		}
		if((bOutput1 && (sequence1>sequence)) || (bOutput2 && (sequence2>sequence)))
			bOutput1 = bOutput2 = false;
		if(bOutput1 && !bOutput2)
		{
			iSize = (int)pAscanHeader1->size+sizeof(DWORD);
			pFifoOutput->m_pFifo->InFifo((BYTE*)pAscanHeader1, iSize, *pAcqInfo1, pStreamHeader1);
			if(pFifoMaster->m_pFifo->RemoveTail())
				pFifoMaster->m_pFifo->IncrementItemIndex(iItem1);
			else if(g_iCycleMinMaster>=0)
				UTKernel_SystemMessageBoxList(L"Error FifoMaster 3!");
			iCount++;
			if(bRemove2)
			{
				if(pFifoSlave->m_pFifo->RemoveTail())
					pFifoSlave->m_pFifo->IncrementItemIndex(iItem2);
				else if(g_iCycleMinSlave>=0)
					UTKernel_SystemMessageBoxList(L"Error FifoSlave 2!");
			}
		}else
		if(!bOutput1 && bOutput2)
		{
			iSize = (int)pAscanHeader2->size+sizeof(DWORD);
			pFifoOutput->m_pFifo->InFifo((BYTE*)pAscanHeader2, iSize, *pAcqInfo2, pStreamHeader2);
			if(pFifoSlave->m_pFifo->RemoveTail())
				pFifoSlave->m_pFifo->IncrementItemIndex(iItem2);
			else if(g_iCycleMinSlave>=0)
				UTKernel_SystemMessageBoxList(L"Error FifoSlave 3!");
			iCount++;
			if(bRemove1)
			{
				if(pFifoMaster->m_pFifo->RemoveTail())
					pFifoMaster->m_pFifo->IncrementItemIndex(iItem1);
				else if(g_iCycleMinMaster>=0)
					UTKernel_SystemMessageBoxList(L"Error FifoMaster 2!");
			}
		}else
		if(bOutput1 && bOutput2)
		{
			MergeAscans(pAscanHeader1, pAscanHeader2, (BYTE*)pBufferMax1, (BYTE*)pBufferMax2, *pAcqInfo1);
			iSize = pAscanHeader1->size+sizeof(DWORD);
			pFifoOutput->m_pFifo->InFifo((BYTE*)g_pOutHeader, iSize, *pAcqInfo1, pStreamHeader1);
			if(pFifoMaster->m_pFifo->RemoveTail())
				pFifoMaster->m_pFifo->IncrementItemIndex(iItem1);
			else if(g_iCycleMinMaster>=0)
				UTKernel_SystemMessageBoxList(L"Error FifoMaster 4!");
			if(pFifoSlave->m_pFifo->RemoveTail())
				pFifoSlave->m_pFifo->IncrementItemIndex(iItem2);
			else if(g_iCycleMinSlave>=0)
				UTKernel_SystemMessageBoxList(L"Error FifoSlave 4!");
			iCount++;
		}
	}while(bOutput1 || bOutput2);
	//check the fifo: should be the same sequence/cycle.
	_iItemCount1 = pFifoMaster->m_pFifo->GetItemLimit(_iItemTail1, _iItemHead1);
	_iItemCount2 = pFifoSlave->m_pFifo->GetItemLimit(_iItemTail2, _iItemHead2);
	if((g_iCycleMinMaster>=0) && (_iItemCount1>0) &&
		(g_iCycleMinSlave>=0) && (_iItemCount2>0) && (iCount>0))
	{
		if(	pFifoMaster->m_pFifo->OutAscan(_iItemTail1,true,pAcqInfo1,pStreamHeader1,pAscanHeader1,pBufferMax1,pBufferMin1,pBufferSat1) &&
			pFifoSlave->m_pFifo->OutAscan(_iItemTail2,true,pAcqInfo2,pStreamHeader2,pAscanHeader2,pBufferMax2,pBufferMin2,pBufferSat2))
		{
			_sequence1 = MAKE_i64(pAscanHeader1->seqHigh, pAscanHeader1->seqLow);
			_iCycle1 = pAscanHeader1->cycle;
			_sequence2 = MAKE_i64(pAscanHeader2->seqHigh, pAscanHeader2->seqLow);
			_iCycle2 = pAscanHeader2->cycle;
			if((_sequence1!=_sequence2) || (_iCycle1!=_iCycle2))
			{
				_iCycle1 = _iCycle1;//usually it is because of missing cycle for the slave.
				UTKernel_SystemMessageBoxList(L"ERROR bad synchronisation between the 128:128a and the 128:128b!");
			}
		}
	}
	if(g_bDump)
	{
		pFifoMaster->DumpFifo(L"c:\\Users\\Public\\DumpFifoMaster.txt");
		pFifoSlave->DumpFifo(L"c:\\Users\\Public\\DumpFifoSlave.txt");
		pFifoOutput->DumpFifo(L"c:\\Users\\Public\\DumpFifoOutput.txt");
	}
	if(iCount>0)
		return iCount;//breakpoint purpose.
#endif _WIN64
	return 0;
}
bool CFifo256::MergeAscans(const CSubStreamAscan_0x0103 *pAscanHeader1, const CSubStreamAscan_0x0103 *pAscanHeader2, BYTE *pbyAscan1, BYTE *pbyAscan2, structAcqInfoEx &acqInfo)
{//first input master, second input slave.
	int iCycle = pAscanHeader1->cycle;
	int iPointCount = pAscanHeader1->dataCount;
	
	if(iCycle != pAscanHeader2->cycle)
		return false;
	if(pAscanHeader1->seqLow != pAscanHeader2->seqLow)
		return false;
	if(pAscanHeader1->seqHigh != pAscanHeader2->seqHigh)
		return false;

	enumBitSize eBitSize = (enumBitSize)pAscanHeader1->bitSize;
	enumRectification eRectification = eSigned;//assumption signed otherwise summation is not possible.
	
	if(eBitSize == e8Bits)
	{
		char *pResult = (char*)&g_pOut[0];
		char *cAscan1 = (char*)pbyAscan1;
		char *cAscan2 = (char*)pbyAscan2;
		short sResult;
		for (int i=0; i<iPointCount; i++)
		{
			sResult = (short)cAscan1[i]+(short)cAscan2[i];
			if (sResult > 127)
				pResult[i] = 127;
			else if (sResult < -128)
				pResult[i] = -128;
			else
				pResult[i] = (char)sResult;
		}
	}
	else if (eBitSize == e12Bits)
	{
		short *pResult = (short*)&g_pOut[0];
		short *sAscan1 = (short*)pbyAscan1;
		short *sAscan2 = (short*)pbyAscan2;
		short sResult;
		for (int i=0; i<iPointCount; i++)
		{
			sResult = sAscan1[i]+sAscan2[i];
			if(sResult > 2047)
				pResult[i] = 2047;
			else if (sResult < -2048)
				pResult[i] = -2048;
			else
				pResult[i] = sResult;
		}
	}
	else if (eBitSize == e16Bits)
	{
		short *pResult = (short*)&g_pOut[0];
		short *sAscan1 = (short*)pbyAscan1;
		short *sAscan2 = (short*)pbyAscan2;
		long lResult;
		for (int i=0; i<iPointCount; i++)
		{
			lResult = (long)sAscan1[i]+(long)sAscan2[i];
			if(lResult > 32767)
				pResult[i] = 32767;
			else if(lResult < -32768)
				pResult[i] = -32768;
			else
				pResult[i] = (short)lResult;
		}
	}
	memcpy(g_pOutHeader, pAscanHeader1, sizeof(CSubStreamAscan_0x0103));
	return true;
}

///////////////////////////////////////////////////////////////////////
mCHWDeviceOEMPA256::mCHWDeviceOEMPA256()
{
	m_bMaster = false;
	m_bSlave = false;
	m_bMasterFifoOld = false;
	m_bMasterFifoOldEnable = false;
	m_bSlaveFifoOld = false;
	m_bSlaveFifoOldEnable = false;
	m_pmHWMaster = NULL;
	m_pmHWSlave = NULL;
	m_pFifoIn = NULL;
	m_pFifoOut = NULL;
	g_abCycleSum = NULL;
	g_iCycleMin = -1;
	g_iCycleMax = -1;
	g_iCycleCount = 0;
}
mCHWDeviceOEMPA256::~mCHWDeviceOEMPA256()
{
	if(m_pFifoIn)
		delete m_pFifoIn;
	if(m_pFifoOut)
		delete m_pFifoOut;
	if(g_abCycleSum)
		delete g_abCycleSum;
	m_pFifoIn = NULL;
	m_pFifoOut = NULL;
}
void mCHWDeviceOEMPA256::FlushFifos()
{
	g_llSequence = -1;
	if(m_pFifoIn)
		m_pFifoIn->Flush();
	if(m_pFifoOut)
		m_pFifoOut->Flush();
}
void mCHWDeviceOEMPA256::ResetCounters()
{
	if(m_pFifoIn)
		m_pFifoIn->ResetCounters();
	if(m_pFifoOut)
		m_pFifoOut->ResetCounters();
}
void mCHWDeviceOEMPA256::SetMaster(bool bEnable, mCHWDeviceOEMPA *pmCHWMaster, mCHWDeviceOEMPA *pmCHWSlave)
{
	m_bMaster = bEnable;
	g_iCycleMin = -1;
	g_iCycleMax = -1;
	g_iCycleCount = 0;
	if(bEnable)
	{
		if(!m_bMasterFifoOldEnable)
		{
			m_bMasterFifoOldEnable = true;
			m_bMasterFifoOld = pmCHWMaster->m_bFifo;
			if(pmCHWMaster->m_bFifo)
			{
				//UTKernel_SystemMessageBoxList(L"WARNING: The master device fifo should be disabled!");
				pmCHWMaster->UpdateFifo(false);
			}
		}
		m_pmHWMaster = pmCHWMaster;
		m_pmHWSlave = pmCHWSlave;
		if(!m_pFifoIn)
			m_pFifoIn = new CFifo256(1024, g_iAscanPointCount256);
		if(!m_pFifoOut)
			m_pFifoOut = new CFifo256(5000, g_iAscanPointCount256);
		if(!g_abCycleSum)
			g_abCycleSum = new bool[g_iCycleCountMax256];
		if(m_pFifoIn)
		{
			m_pFifoIn->m_pmHWMaster = pmCHWMaster;
			m_pFifoIn->m_pmHWSlave = pmCHWSlave;
		}
		if(m_pFifoOut)
		{
			m_pFifoOut->m_pmHWMaster = pmCHWMaster;
			m_pFifoOut->m_pmHWSlave = pmCHWSlave;
			pmCHWMaster->m_pFifoAscan = m_pFifoOut->m_pFifo;
			pmCHWMaster->m_bFifo = true;
		}
	}else{
		pmCHWMaster->m_pFifoAscan = pmCHWMaster->m_hwDeviceOEMPA.GetAcquisitionFifo(eFifoAscan);
		if(m_bMasterFifoOldEnable)
		{
			//UTKernel_SystemMessageBoxList(L"WARNING: The master device fifo should be enabled!");
			pmCHWMaster->UpdateFifo(m_bMasterFifoOld);
			m_bMasterFifoOldEnable = false;
		}
		m_pmHWMaster = NULL;
		m_pmHWSlave = NULL;
		g_iDeviceIdMaster = -1;
	}

	EnterCriticalSection(&pmCHWMaster->m_AcquisitionAscan);
	g_iCycleMin = -1;
	g_iCycleMax = -1;
	if(g_abCycleSum)
		memset(g_abCycleSum,0,g_iCycleCountMax256);
	LeaveCriticalSection(&pmCHWMaster->m_AcquisitionAscan);
#ifdef _WIN64
	if(bEnable)
	{
		if(m_pFifoOut && !m_pFifoOut->m_pFifo->GetEvent())
			m_pFifoOut->m_pFifo->CreateEvent(L":");
		if(m_pFifoOut && !m_pFifoOut->m_pFifo->IsRunning() && !m_pFifoOut->m_pFifo->Start(0,NULL,ThreadAcquisitionFifo, m_pFifoOut))
			UTKernel_SystemMessageBoxList(L"ERROR to run the : thread!");
	}else{
		if(m_pFifoOut)
			m_pFifoOut->m_pFifo->Stop();
	}
#endif _WIN64
}
void mCHWDeviceOEMPA256::SetSlave(bool bEnable, mCHWDeviceOEMPA *pmCHWMaster, mCHWDeviceOEMPA *pmCHWSlave)
{
	m_bSlave = bEnable;
	if(bEnable)
	{
		if(!m_bSlaveFifoOldEnable)
		{
			m_bSlaveFifoOldEnable = true;
			m_bSlaveFifoOld = pmCHWSlave->m_bFifo;
			if(pmCHWSlave->m_bFifo)
			{
				//UTKernel_SystemMessageBoxList(L"WARNING: The slave device fifo should be disabled!");
				pmCHWSlave->UpdateFifo(false);
			}
		}
		m_pmHWMaster = pmCHWMaster;
		m_pmHWSlave = pmCHWSlave;
		if(!m_pFifoIn)
			m_pFifoIn = new CFifo256(1024, g_iAscanPointCount256);
		if(!g_abCycleSum)
			g_abCycleSum = new bool[g_iCycleCountMax256];
		if(m_pFifoIn)
		{
			m_pFifoIn->m_pmHWMaster = pmCHWMaster;
			m_pFifoIn->m_pmHWSlave = pmCHWSlave;
		}
	}else{
		pmCHWSlave->m_pFifoAscan = pmCHWSlave->m_hwDeviceOEMPA.GetAcquisitionFifo(eFifoAscan);
		if(m_bSlaveFifoOldEnable)
		{
			//UTKernel_SystemMessageBoxList(L"WARNING: The slave device fifo should be enabled!");
			pmCHWSlave->UpdateFifo(m_bSlaveFifoOld);
			m_bSlaveFifoOldEnable = false;
		}
		m_pmHWMaster = NULL;
		m_pmHWSlave = NULL;
		g_iDeviceIdSlave = -1;
	}

#ifdef _WIN64
	EnterCriticalSection(&pmCHWSlave->m_AcquisitionAscan);
	g_iCycleMin = -1;
	g_iCycleMax = -1;
	if(g_abCycleSum)
		memset(g_abCycleSum,0,g_iCycleCountMax256);
	LeaveCriticalSection(&pmCHWSlave->m_AcquisitionAscan);
#endif _WIN64
}
bool mCHWDeviceOEMPA256::IsMaster()
{
	return m_bMaster;
}
bool mCHWDeviceOEMPA256::IsSlave()
{
	return m_bSlave;
}
bool mCHWDeviceOEMPA256::mxGetAcquisitionAscanFifoStatus(int &iAscanCount, int &iDataLost, int &iAscanTotal, __int64 &iByteTotal)
{
#ifdef _WIN64
	if(m_bMaster)
	{
		iAscanCount = m_pmHWMaster->m_csDevice256.m_pFifoIn->m_pFifo->GetCount();//FifoMaster.m_Fifo.GetCount();
		iDataLost = m_pmHWMaster->m_csDevice256.m_pFifoIn->m_pFifo->GetLost();//FifoMaster.m_Fifo.GetLost();
		iAscanTotal = (int)m_pmHWMaster->m_csDevice256.m_pFifoIn->m_pFifo->GetTotalCount();//FifoMaster.m_Fifo.GetTotalCount();
		iByteTotal = (int)m_pmHWMaster->m_csDevice256.m_pFifoIn->m_pFifo->GetTotalByteCount();//FifoMaster.m_Fifo.GetTotalCount();
		return true;
	}
	if(m_bSlave)
	{
		iAscanCount = m_pmHWSlave->m_csDevice256.m_pFifoIn->m_pFifo->GetCount();//FifoSlave.m_Fifo.GetCount();
		iDataLost = m_pmHWSlave->m_csDevice256.m_pFifoIn->m_pFifo->GetLost();//FifoSlave.m_Fifo.GetLost();
		iAscanTotal = (int)m_pmHWSlave->m_csDevice256.m_pFifoIn->m_pFifo->GetTotalCount();//FifoSlave.m_Fifo.GetTotalCount();
		iByteTotal = (int)m_pmHWSlave->m_csDevice256.m_pFifoIn->m_pFifo->GetTotalByteCount();//FifoSlave.m_Fifo.GetTotalCount();
		return true;
	}
#endif _WIN64
	return false;
}
structCorrectionOEMPA* WINAPI mCHWDeviceOEMPA256::CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	mCHWDeviceOEMPA *pmHWDevice=(mCHWDeviceOEMPA*)pAcquisitionParameter;
	int iCount;
	int *piCycleMin,*piCycleMax;
	bool *pbCycleSum;

	switch(eStep)
	{
	default: break;
	case eWriteHW_Enter:
		if((pmHWDevice->m_csDevice256.m_bMaster && pmHWDevice->m_csDevice256.m_bSlave) && (pCycle->eRectification!=eSigned))
			UTKernel_SystemMessageBoxList(L"Error signed ascan is expected!");
		if(pmHWDevice->m_csDevice256.m_bMaster || pmHWDevice->m_csDevice256.m_bSlave)
		{
			pmHWDevice->m_csDevice256.g_iCycleCount = pRoot->iCycleCount;
			piCycleMin = &pmHWDevice->m_csDevice256.g_iCycleMin;
			piCycleMax = &pmHWDevice->m_csDevice256.g_iCycleMax;
			pbCycleSum = pmHWDevice->m_csDevice256.g_abCycleSum;
			*piCycleMin = -1;
			*piCycleMax = -1;
			for(int iCycle=0;iCycle<g_iCycleCountMax256;iCycle++)
				pbCycleSum[iCycle] = false;
			for(int iCycle=0;iCycle<pRoot->iCycleCount;iCycle++)
			{
				iCount = pReception[iCycle].adwElement.GetApertureElementCount();
				if(iCount>0)
				{
					if((*piCycleMin<0) || (iCycle<*piCycleMin))
						*piCycleMin = iCycle;
					if((*piCycleMax<0) || (iCycle>*piCycleMax))
						*piCycleMax = iCycle;
					pbCycleSum[iCycle] = true;
				}
			}
		}
		break;
	}
	return NULL;
}
typedef union unionConst
{
	const void *pConstVoid;
	void *pVoid;
}unionConst;
UINT WINAPI mCHWDeviceOEMPA256::ProcessAcquisitionAscan_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
{
#ifdef _WIN64
	//structAcqInfoEx acqInfoOut;
	mCHWDeviceOEMPA *pmHWDevice=(mCHWDeviceOEMPA*)pAcquisitionParameter;
	int iCount=0;
	unionConst ascanHeader;
	CSubStreamAscan_0x0103 *_pAscanHeader;
	CFifo256 *pFifoMaster,*pFifoSlave,*pFifoOutput;

	pFifoMaster = pmHWDevice->m_csDevice256.m_pmHWMaster->m_csDevice256.m_pFifoIn;
	pFifoSlave = pmHWDevice->m_csDevice256.m_pmHWSlave->m_csDevice256.m_pFifoIn;
	pFifoOutput = pmHWDevice->m_csDevice256.m_pmHWMaster->m_csDevice256.m_pFifoOut;

	ascanHeader.pConstVoid = pAscanHeader;
	_pAscanHeader = (CSubStreamAscan_0x0103*)ascanHeader.pVoid;
	//if(pmHWDevice->m_bMasterDevice/* || pmHWDevice->m_bSlaveDevice*/)
	//	iCount = pmHWDevice->MergePrevious();//should be called only by the master otherwise mutual exclusion is required.
	if(pmHWDevice->m_csDevice256.m_bMaster)
	{
		CSubStreamAscan_0x0103 pAscanHeader2;
        BYTE *pAscan2=NULL;
        int iItem2=-1;
        LONGLONG sequence1=MAKE_i64(pAscanHeader->seqHigh, pAscanHeader->seqLow);
        int iCycle1=pAscanHeader->cycle;
		int iSize1=pAscanHeader->dataCount*pAscanHeader->dataSize;

		if(pmHWDevice->m_csDevice256.g_abCycleSum[iCycle1])
			_pAscanHeader->padding1 = 1;
		else
			_pAscanHeader->padding1 = 0;
		if(!g_bStartFirstSequenceMaster || (g_bStartFirstSequenceMaster && (sequence1==1)))
		{
			if(g_bStartFirstSequenceMaster)
				pFifoMaster->m_pFifo->RemoveAll();//because for the second EnablePulser the first sequence seems to be the last of the previous EnablePulser.
			g_bStartFirstSequenceMaster = false;
			iSize1 = HIWORD((DWORD*)pAscanHeader);
			iSize1 += sizeof(DWORD);
			if(pFifoMaster->m_llFWTimeStamp0 == -1)
				pFifoMaster->m_llFWTimeStamp0 = MAKE_i64(pAscanHeader->timeStampHigh,pAscanHeader->timeStampLow);
			pFifoMaster->m_pFifo->InFifo((BYTE*)pAscanHeader, iSize1, acqInfo, pStreamHeader);
			if(!iCycle1 && pFifoOutput->m_pFifo->GetEvent())
				SetEvent(pFifoOutput->m_pFifo->GetEvent());
pFifoMaster->Check();
		}
	}
	else if (pmHWDevice->m_csDevice256.m_bSlave)
	{
		CSubStreamAscan_0x0103 pAscanHeader1;
        BYTE *pAscan1=NULL;
        int iItem1=-1;
        LONGLONG sequence2=MAKE_i64(pAscanHeader->seqHigh, pAscanHeader->seqLow);
        int iCycle2=pAscanHeader->cycle;
		int iSize2=pAscanHeader->dataCount*pAscanHeader->dataSize;

 		if(pmHWDevice->m_csDevice256.g_abCycleSum[iCycle2])
			_pAscanHeader->padding1 = 1;
		else
			_pAscanHeader->padding1 = 0;
		if(!g_bStartFirstSequenceSlave || (g_bStartFirstSequenceSlave && (sequence2==1)))
		{
			if(g_bStartFirstSequenceSlave)
				pFifoSlave->m_pFifo->RemoveAll();//because for the second EnablePulser the first sequence seems to be the last of the previous EnablePulser.
			g_bStartFirstSequenceSlave = false;
			iSize2 = HIWORD((DWORD*)pAscanHeader);
			iSize2 += sizeof(DWORD);
			if(pFifoSlave->m_llFWTimeStamp0 == -1)
				pFifoSlave->m_llFWTimeStamp0 = MAKE_i64(pAscanHeader->timeStampHigh,pAscanHeader->timeStampLow);
			pFifoSlave->m_pFifo->InFifo((BYTE*)pAscanHeader, iSize2, acqInfo, pStreamHeader);
			if(!iCycle2 && pFifoOutput->m_pFifo->GetEvent())
				SetEvent(pFifoOutput->m_pFifo->GetEvent());
pFifoSlave->Check();
		}
	}
#endif _WIN64
	return 0;
}
///////////////////////////////////////////////////////////////////////
#endif _DEVICE_OEMPA_256_256_

extern "C"{
	__declspec(dllexport) DWORD mxGetAcquisitionAscanFifoStatus256(int iDeviceId, long *plAscanCount, long *plDataLost, long *plAscanTotal, long long *pllByteTotal)
	//input "iDeviceId": device identifier number.
	//output return the acquisition ascan count.
	{
#ifdef _DEVICE_OEMPA_256_256_
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		int iDataLost, iAscanCount, iAscanTotal;
		__int64 iByteTotal;
		bool bRet = false;

		if(!mpHWDeviceOEMPA)
			return false;
		if(mpHWDeviceOEMPA->m_csDevice256.mxGetAcquisitionAscanFifoStatus(iAscanCount, iDataLost, iAscanTotal, iByteTotal))
			bRet = true;
		else{
			iDataLost = 0;
			iAscanCount = 0;
			iAscanTotal = 0;
			iByteTotal = 0;
		}
		if(plAscanCount)
			*plAscanCount = (long)iAscanCount;
		if(plDataLost)
			*plDataLost = (long)iDataLost;
		if(plAscanTotal)
			*plAscanTotal = (long)iAscanTotal;
		if(pllByteTotal)
			*pllByteTotal = (long long)iByteTotal;
		return true;
#else _DEVICE_OEMPA_256_256_
		return false;
#endif _DEVICE_OEMPA_256_256_
	}
	__declspec(dllexport) bool mxLockDevice256(int iDeviceIdMaster, int iDeviceIdSlave, int eAcqState)
	//Lock OEM-PA Communication link to access FW parameters
	//input "iDeviceIdMaster": Master device identifier number.
	//input "iDeviceIdSlave": Slave device identifier number.
	//input "eAcqState": set acquisition state; 0 - eAcqOff (disabled), 1 - eAcqDefault (use config file), 2 - eAcqOn (enabled)
	//output return true if no error and false in case of error
	{
#ifdef _DEVICE_OEMPA_256_256_
		CHWDeviceOEMPA *pMaster=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdMaster),*pSlave;
		if(pMaster && (pSlave=(CHWDeviceOEMPA*)pMaster->GetMatched256DeviceOEMPA()))
		{//HW: 1 x 256:256 (HW summation)
			if(iDeviceIdSlave!=-1)
				return false;
			if(!pMaster->LockDevice((enumAcquisitionState)eAcqState))
				return false;
			if(!pSlave->LockDevice((enumAcquisitionState)eAcqState))
				return false;
			return true;
		}
		//HW: 2 x 128:128 with SW summation.
		CHWDeviceOEMPA *pHWMaster=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdMaster), *pHWSlave=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdSlave);

		if((!pHWMaster) || (!pHWSlave))
			return false;
		if(!pHWMaster->LockDevice((enumAcquisitionState)eAcqState))
			return false;
		if(!pHWSlave->LockDevice((enumAcquisitionState)eAcqState))
			return false;
		return true;
#else _DEVICE_OEMPA_256_256_
		return false;
#endif _DEVICE_OEMPA_256_256_
	}
	__declspec(dllexport) bool mxUnlockDevice256(int iDeviceIdMaster, int iDeviceIdSlave, int eAcqState)
	//Unlock OEM-PA Communication link to access FW parameters
	//input "iDeviceIdMaster": Master device identifier number.
	//input "iDeviceIdSlave": Slave device identifier number.
	//input "eAcqState": set acquisition state; 0 - eAcqOff (disabled), 1 - eAcqDefault (use config file), 2 - eAcqOn (enabled)
	//output return true if no error and false in case of error
	{
#ifdef _DEVICE_OEMPA_256_256_
		CHWDeviceOEMPA *pMaster=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdMaster),*pSlave;
		if(pMaster && (pSlave=(CHWDeviceOEMPA*)pMaster->GetMatched256DeviceOEMPA()))
		{//HW: 1 x 256:256 (HW summation)
			if(iDeviceIdSlave!=-1)
				return false;
			if(!pMaster->UnlockDevice((enumAcquisitionState)eAcqState))
				return false;
			if(!pSlave->UnlockDevice((enumAcquisitionState)eAcqState))
				return false;
			return true;
		}
		//HW: 2 x 128:128 with SW summation.
		CHWDeviceOEMPA *pHWMaster=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdMaster), *pHWSlave=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdSlave);

		if((!pHWMaster) || (!pHWSlave))
			return false;
		g_bStartFirstSequenceMaster = true;
		g_bStartFirstSequenceSlave = true;
		g_iDeviceIdMaster = iDeviceIdMaster;
		g_iDeviceIdSlave = iDeviceIdSlave;
		if(!pHWSlave->UnlockDevice((enumAcquisitionState)eAcqState))
			return false;
		if(!pHWMaster->UnlockDevice((enumAcquisitionState)eAcqState))
			return false;
		return true;
#else _DEVICE_OEMPA_256_256_
		return false;
#endif _DEVICE_OEMPA_256_256_
	}
	__declspec(dllexport) bool mxEnableShot256(int iDeviceIdMaster,int iDeviceIdSlave, bool bValue)
	//input "iDeviceIdMaster": Master device identifier number.
	//input "iDeviceIdSlave": Slave device identifier number.
	//input "bValue": true to enable pulser, false to disable pulser.
	//output return true if no error and false in case of error
	{
#ifdef _DEVICE_OEMPA_256_256_
		CHWDeviceOEMPA *pMaster=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdMaster),*pSlave;
		if(pMaster && (pSlave=(CHWDeviceOEMPA*)pMaster->GetMatched256DeviceOEMPA()))
		{//HW: 1 x 256:256 (HW summation)
			if(iDeviceIdSlave!=-1)
				return false;
			if(bValue)
			{
				if(!mxLockDevice256(iDeviceIdMaster, -1, eAcqOff))
					return false;
				if(!mxUnlockDevice256(iDeviceIdMaster, -1, eAcqOn))
					return false;
			}
			else
			{
				if(!mxLockDevice256(iDeviceIdMaster, -1, eAcqOff))
					return false;
				if(!mxUnlockDevice256(iDeviceIdMaster, -1, eAcqOff))
					return false;
			}
			return true;
		}
		//HW: 2 x 128:128 with SW summation.
		mCHWDeviceOEMPA *mpHWMaster=mCHWDeviceOEMPA::mGetHWDevice(iDeviceIdMaster), *mpHWSlave=mCHWDeviceOEMPA::mGetHWDevice(iDeviceIdSlave);
		if(bValue)
		{
			g_bStartFirstSequenceMaster = true;
			g_bStartFirstSequenceSlave = true;
			g_iDeviceIdMaster = iDeviceIdMaster;
			g_iDeviceIdSlave = iDeviceIdSlave;
			if(!mxLockDevice256(iDeviceIdMaster, iDeviceIdSlave, eAcqOff))
				return false;
			if(!mxUnlockDevice256(iDeviceIdMaster, iDeviceIdSlave, eAcqOn))
				return false;
		}
		else
		{
			g_iDeviceIdMaster = -1;
			g_iDeviceIdSlave = -1;
			if(!mxLockDevice256(iDeviceIdMaster, iDeviceIdSlave, eAcqOff))
				return false;
			if(!mxUnlockDevice256(iDeviceIdMaster, iDeviceIdSlave, eAcqOff))
				return false;
			return true;//breakpoint
		}
		return true;
#else _DEVICE_OEMPA_256_256_
		return false;
#endif _DEVICE_OEMPA_256_256_
	}
	__declspec(dllexport) bool mxReadFileWriteHW256(int iDeviceIdMaster, int iDeviceIdSlave, wchar_t *pFileNameMaster, wchar_t *pFileNameSlave, int *piCyclesMaster, int *piCyclesSlave)
	//input "iDeviceId": device identifier number.
	//input "pFileName": OEMPA file path.
	//output return true if no error and false in case of error
	{
#ifdef _DEVICE_OEMPA_256_256_
		CHWDeviceOEMPA *pMaster=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdMaster),*pSlave;
		if(pMaster && (pSlave=(CHWDeviceOEMPA*)pMaster->GetMatched256DeviceOEMPA()))
		{//HW: 1 x 256:256 (HW summation)
			if(iDeviceIdSlave!=-1)
				return false;
			return OEMPA_ReadFileWriteHW256(pMaster,pSlave,pFileNameMaster,pFileNameSlave);
		}
		//HW: 2 x 128:128 with SW summation.
		CHWDeviceOEMPA *pHWMaster=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdMaster), *pHWSlave=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdSlave);
		mCHWDeviceOEMPA *mpHWMaster=mCHWDeviceOEMPA::mGetHWDevice(iDeviceIdMaster), *mpHWSlave=mCHWDeviceOEMPA::mGetHWDevice(iDeviceIdSlave);

		if((!pHWMaster) || (!pHWSlave))
			return false;
		mpHWMaster->m_csDevice256.SetMaster(true,mpHWMaster,mpHWSlave);
		mpHWSlave->m_csDevice256.SetSlave(true,mpHWMaster,mpHWSlave);

		return OEMPA_ReadFileWriteHW256(pHWMaster,pHWSlave,pFileNameMaster,pFileNameSlave);
#else _DEVICE_OEMPA_256_256_
		return false;
#endif _DEVICE_OEMPA_256_256_
	}
	__declspec(dllexport) bool mxResetCounters256(int iDeviceIdMaster, int iDeviceIdSlave)
	{
#ifdef _DEVICE_OEMPA_256_256_
		CHWDeviceOEMPA *pMaster=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdMaster),*pSlave;
		if(pMaster && (pSlave=(CHWDeviceOEMPA*)pMaster->GetMatched256DeviceOEMPA()))
		{//HW: 1 x 256:256 (HW summation)
			return false;
		}
		//HW: 2 x 128:128 with SW summation.
		mCHWDeviceOEMPA *mpHWMaster=mCHWDeviceOEMPA::mGetHWDevice(iDeviceIdMaster), *mpHWSlave=mCHWDeviceOEMPA::mGetHWDevice(iDeviceIdSlave);

		if((!mpHWMaster) || (!mpHWSlave))
			return false;
		mpHWMaster->m_csDevice256.ResetCounters();
		mpHWSlave->m_csDevice256.ResetCounters();

		return true;
		return true;
#else _DEVICE_OEMPA_256_256_
		return false;
#endif _DEVICE_OEMPA_256_256_
	}
	__declspec(dllexport) bool mxFlushAcquisitionAscanFifo256(int iDeviceIdMaster, int iDeviceIdSlave)
	{
#ifdef _DEVICE_OEMPA_256_256_
		CHWDeviceOEMPA *pMaster=mCHWDeviceOEMPA::GetHWDevice(iDeviceIdMaster),*pSlave;
		if(pMaster && (pSlave=(CHWDeviceOEMPA*)pMaster->GetMatched256DeviceOEMPA()))
		{//HW: 1 x 256:256 (HW summation)
			return false;
		}
		//HW: 2 x 128:128 with SW summation.
		mCHWDeviceOEMPA *mpHWMaster=mCHWDeviceOEMPA::mGetHWDevice(iDeviceIdMaster), *mpHWSlave=mCHWDeviceOEMPA::mGetHWDevice(iDeviceIdSlave);

		if((!mpHWMaster) || (!mpHWSlave))
			return false;
		mpHWMaster->m_csDevice256.FlushFifos();
		mpHWSlave->m_csDevice256.FlushFifos();
		return true;
#else _DEVICE_OEMPA_256_256_
		return false;
#endif _DEVICE_OEMPA_256_256_
	}
	void ExitDll256()
	{
#ifdef _DEVICE_OEMPA_256_256_
		if((g_iDeviceIdMaster>=0) && (g_iDeviceIdSlave>=0))
			mxEnableShot256(g_iDeviceIdMaster, g_iDeviceIdSlave, false);
#endif _DEVICE_OEMPA_256_256_
	}

//////////////////////////////////////////////////////////////
}
#ifdef _DEVICE_OEMPA_256_256_


///////////////////////////////////

CFifo256::CFifo256(int iMaxAscanCount, int iMaxAscanSize)
#ifdef _WIN64
	: m_Fifo(eFifoAscan, NULL)
#endif _WIN64
{
#ifdef _WIN64
	m_pFifo = &m_Fifo;
	m_pFifo->Alloc(iMaxAscanCount, iMaxAscanCount*iMaxAscanSize);
#endif _WIN64
	m_llFWTimeStamp0 = -1;
	m_pmHWMaster = NULL;
	m_pmHWSlave = NULL;
}
CFifo256:: ~CFifo256()
{
}
void CFifo256::Flush()
{
	m_llFWTimeStamp0 = -1;
	//m_pFifo->ResetData();
#ifdef _WIN64
	m_pFifo->RemoveAll();
#endif _WIN64
}
void CFifo256::ResetCounters()
{
#ifdef _WIN64
	m_pFifo->ResetCounters();
#endif _WIN64
}
int CFifo256::Out(int iItem, CSubStreamAscan_0x0103 &pAscanHeader, BYTE* &pAscan, structAcqInfoEx& acqInfo)//To get the ascan and to remove it from the fifo.
{
#ifdef _WIN64
	structAcqInfoEx *_pAcqInfo;
	CStream_0x0001 *_pStreamHeader;
	CSubStreamAscan_0x0103 *_pAscanHeader;
	void *_pBufferMax, *_pBufferMin, *_pBufferSat;

	if(m_pFifo->OutAscan(iItem, false, _pAcqInfo, _pStreamHeader, _pAscanHeader, _pBufferMax, _pBufferMin, _pBufferSat))
	{
		memcpy(&pAscanHeader, _pAscanHeader, sizeof(CSubStreamAscan_0x0103));
		memcpy(&acqInfo, _pAcqInfo, sizeof(acqInfo));
		pAscan = (BYTE*)_pBufferMax;
		return 0;
	}
#endif _WIN64
	return -1;//error
}
int CFifo256::Peek(int iItem, CSubStreamAscan_0x0103 &pAscanHeader, BYTE* &pAscan, structAcqInfoEx& acqInfo)//At exit the data is NOT removed from the fifo.
{
#ifdef _WIN64
	structAcqInfoEx *_pAcqInfo;
	CStream_0x0001 *_pStreamHeader;
	CSubStreamAscan_0x0103 *_pAscanHeader;
	void *_pBufferMax, *_pBufferMin, *_pBufferSat;

	if(m_pFifo->OutAscan(iItem, true, _pAcqInfo, _pStreamHeader, _pAscanHeader, _pBufferMax, _pBufferMin, _pBufferSat))
	{
		memcpy(&pAscanHeader, _pAscanHeader, sizeof(CSubStreamAscan_0x0103));
		memcpy(&acqInfo, _pAcqInfo, sizeof(acqInfo));
		pAscan = (BYTE*)_pBufferMax;
		return 0;
	}
#endif _WIN64
	return -1;//error
}
bool CFifo256::CheckFirstSequence(int iCycleCount, int &iAscanLost)
{//the purpose is to wait a whole sequence (no missing cycle).
#ifdef _WIN64
	int iItem, iItemCount, iIndexCycle, _iItemTail, iItemTail, iItemHead, iCount, iCycle;
	//vector<CSubStreamAscan_0x0103>::pointer ptrHeader;
	CStream_0x0001 *pStreamHeader;
	structAcqInfoEx *pAcqInfo;
	CSubStreamAscan_0x0103 *ptrHeader;
	void *pBufferMax,*pBufferMin,*pBufferSat;
	static bool g_bDump=false;

	iAscanLost = 0;
	if(!iCycleCount)
		return true;
restart:
	iItemCount = m_pFifo->GetItemLimit(iItemTail,iItemHead);
    if(iItemCount<2*iCycleCount)
    {//wait for 2 sequences then we will eventually remove the first one if this sequence have missing cycle.
        return false;
    }
	iCount = 0;
	_iItemTail = -1;
	iItem = iItemTail;
	do{
		if(!m_pFifo->OutAscan(iItem,true,pAcqInfo,pStreamHeader,ptrHeader,pBufferMax,pBufferMin,pBufferSat))
			return false;//error
		iIndexCycle = ptrHeader->cycle;
		if(iIndexCycle==0)
			break;
		_iItemTail = iItem;
		m_pFifo->IncrementItemIndex(iItem);
		iCount++;
		if(iCount>iCycleCount)
			break;
	}while(true);
	if((iIndexCycle>0) && (_iItemTail>=0))
	{
		iAscanLost += iCount;
		m_pFifo->RemoveItem(_iItemTail);
if(g_bDump)
{
	m_pmHWMaster->m_csDevice256.m_pFifoIn->DumpFifo(L"c:\\Users\\Public\\DumpFifoMaster.txt");
	m_pmHWSlave->m_csDevice256.m_pFifoIn->DumpFifo(L"c:\\Users\\Public\\DumpFifoSlave.txt");
	m_pmHWMaster->m_csDevice256.m_pFifoOut->DumpFifo(L"c:\\Users\\Public\\DumpFifoOutput.txt");
}
		return false;
	}
	_iItemTail = iItem;
	iCount = 1;
	iCycle = 0;
	while(iCount<=iCycleCount)
	{//the cycle index is not 0 for the first item => remove bad cycles.
		iCount++;
		iCycle++;
		_iItemTail = iItem;
		m_pFifo->IncrementItemIndex(iItem);
		if(!m_pFifo->OutAscan(iItem,true,pAcqInfo,pStreamHeader,ptrHeader,pBufferMax,pBufferMin,pBufferSat))
			return false;//error
		iIndexCycle = ptrHeader->cycle;
		if((iIndexCycle==iCycle) && (iCount==iCycleCount))
			return true;
	}
	iAscanLost += 1;
	m_pFifo->RemoveTail();//remove at least the first item (cycle 0).
	goto restart;
#endif _WIN64
	return false;
}
bool CFifo256::Check()
{
return true;
#ifdef _WIN64
	if(g_iCycleCount==-1)
		return true;
	int iItem, iItemCount, iItemTail, iItemHead, iCount;
	long long sequence,seqOld=-1,seqCycle;
	CStream_0x0001 *pStreamHeader;
	structAcqInfoEx *pAcqInfo;
	CSubStreamAscan_0x0103 *ptrHeader;
	void *pBufferMax,*pBufferMin,*pBufferSat;
	static bool g_bDump=false;

	iItemCount = m_pFifo->GetItemLimit(iItemTail,iItemHead);
    if(iItemCount<=0)
        return true;
	iCount = 0;
	iItem = iItemTail;
	do{
		if(iItem==iItemHead)
			break;
		if(!m_pFifo->OutAscan(iItem,true,pAcqInfo,pStreamHeader,ptrHeader,pBufferMax,pBufferMin,pBufferSat))
			return false;//error
		sequence = MAKE_i64(ptrHeader->seqHigh, ptrHeader->seqLow);
		seqCycle = sequence * g_iCycleCount;
		seqCycle += ptrHeader->cycle;
		m_pFifo->IncrementItemIndex(iItem);
		if((seqOld!=-1) && (seqOld+1!=seqCycle))
		{
if(g_bDump)
{
	m_pmHWMaster->m_csDevice256.m_pFifoIn->DumpFifo(L"c:\\Users\\Public\\DumpFifoMaster.txt");
	m_pmHWSlave->m_csDevice256.m_pFifoIn->DumpFifo(L"c:\\Users\\Public\\DumpFifoSlave.txt");
	m_pmHWMaster->m_csDevice256.m_pFifoOut->DumpFifo(L"c:\\Users\\Public\\DumpFifoOutput.txt");
}
if(this==m_pmHWMaster->m_csDevice256.m_pFifoIn)//&FifoMaster)
	UTKernel_SystemMessageBoxList(L"ERROR Master missing data!");
if(this==m_pmHWSlave->m_csDevice256.m_pFifoIn)//&FifoSlave)
	UTKernel_SystemMessageBoxList(L"ERROR Slave missing data!");
if(this==m_pmHWMaster->m_csDevice256.m_pFifoOut)//&FifoOutput)
	UTKernel_SystemMessageBoxList(L"ERROR OutputFifo missing data!");
return false;//probably data loss => have to resynchronize the master and the slave.
		}
		iCount++;
		seqOld = seqCycle;
	}while(iCount<=iItemCount);
if(g_bDump)
{
	m_pmHWMaster->m_csDevice256.m_pFifoIn->DumpFifo(L"c:\\Users\\Public\\DumpFifoMaster.txt");
	m_pmHWSlave->m_csDevice256.m_pFifoIn->DumpFifo(L"c:\\Users\\Public\\DumpFifoSlave.txt");
	m_pmHWMaster->m_csDevice256.m_pFifoOut->DumpFifo(L"c:\\Users\\Public\\DumpFifoOutput.txt");
}
	return true;
#endif _WIN64
}
bool CFifo256::DumpFifo(const wchar_t *wcFileName)
{
	bool bRet=true;
#ifdef _WIN64
	LARGE_INTEGER li;
	wchar_t string[MAX_PATH];

	bRet=m_pFifo->DumpFifo(wcFileName);
	myCStdioFile file(wcFileName,myCFile::typeText|myCFile::modeWrite);
	file.SeekToEnd();
	li.QuadPart = m_llFWTimeStamp0;
	wsprintf(string,L"0x%.8lX\t0x%.8lX\n",
		(long)li.HighPart,(long)li.LowPart);
	file.WriteString(string);
	file.Close();
#endif _WIN64
	return bRet;
}
#endif _DEVICE_OEMPA_256_256_
