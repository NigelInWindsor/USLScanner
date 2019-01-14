#pragma once

#ifdef _DEVICE_OEMPA_256_256_
const int g_iAscanPointCount256=16384;
const int g_iCycleCountMax256=1024;
class mCHWDeviceOEMPA;

class CFifo256
{
public:
    CFifo256(int iMaxAscanCount, int iMaxAscanSize);
    ~CFifo256();

	bool MergeAscans(const CSubStreamAscan_0x0103 *pAscanHeader1, const CSubStreamAscan_0x0103 *pAscanHeader2, BYTE *pbyAscan1, BYTE *pbyAscan2, structAcqInfoEx &acqInfo);
	int MergePrevious();
	bool DumpFifo(const wchar_t *wcFileName);
	void Flush();
	void ResetCounters();
	bool Check();
    int Out(int iItem, CSubStreamAscan_0x0103 &pAscanHeader, BYTE* &pAscan, structAcqInfoEx& acqInfo);//To get the ascan and to remove it from the fifo.
    int Peek(int iItem, CSubStreamAscan_0x0103 &pAscanHeader, BYTE* &pAscan, structAcqInfoEx& acqInfo);//At exit the data is NOT removed from the fifo.
	bool CheckFirstSequence(int iCycleCount, int &iAscanLost);
	CAcquisitionFifo m_Fifo;
	CAcquisitionFifo *m_pFifo;//so that it should be possible to use the fifo of the driver instead of the member "m_Fifo".
	long long m_llFWTimeStamp0;
	mCHWDeviceOEMPA *m_pmHWMaster;
	mCHWDeviceOEMPA *m_pmHWSlave;
};

class mCHWDeviceOEMPA256
{
public:
	friend class mCHWDeviceOEMPA;
	friend class CFifo256;
	mCHWDeviceOEMPA256();
	~mCHWDeviceOEMPA256();

	void SetMaster(bool bEnable, mCHWDeviceOEMPA *pmCHWMaster, mCHWDeviceOEMPA *pmCHWSlave);
	void SetSlave(bool bEnable, mCHWDeviceOEMPA *pmCHWMaster, mCHWDeviceOEMPA *pmCHWSlave);
	bool IsMaster();
	bool IsSlave();
	void FlushFifos();
	void ResetCounters();
	bool mxGetAcquisitionAscanFifoStatus(int &iAscanCount, int &iDataLost, int &iAscanTotal, __int64 &iByteTotal);
	static UINT WINAPI ProcessAcquisitionAscan_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
	static structCorrectionOEMPA* WINAPI CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
protected:
	static DWORD WINAPI ThreadAcquisitionFifo(LPVOID pParam);
	//:
	bool m_bMaster;
	bool m_bSlave;
	bool m_bMasterFifoOld;
	bool m_bMasterFifoOldEnable;
	bool m_bSlaveFifoOld;
	bool m_bSlaveFifoOldEnable;
	mCHWDeviceOEMPA *m_pmHWMaster;
	mCHWDeviceOEMPA *m_pmHWSlave;
	CFifo256 *m_pFifoIn;
	CFifo256 *m_pFifoOut;
	bool *g_abCycleSum;
	int g_iCycleMin;
	int g_iCycleMax;
	int g_iCycleCount;
};
#endif _DEVICE_OEMPA_256_256_

class mCHWDeviceOEMPA
{
public:
#ifdef _DEVICE_OEMPA_256_256_
	friend class mCHWDeviceOEMPA256;
#endif _DEVICE_OEMPA_256_256_
	mCHWDeviceOEMPA(int iOEMPACycleCountMax, int iAscanPointCount, bool bFifo);
	~mCHWDeviceOEMPA();

	static UINT WINAPI ProcessAcquisitionAscan_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
	static UINT WINAPI ProcessAcquisitionCscan_0x00010X02(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof);
	static structCorrectionOEMPA* WINAPI CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
	static mCHWDeviceOEMPA* mCHWDeviceOEMPA::mGetHWDevice(int iDeviceId);
	static CHWDeviceOEMPA* mCHWDeviceOEMPA::GetHWDevice(int iDeviceId);

	//setting
	void Connected();
	int GetSWCycleCount();
	int GetHWCycleCount();
	double GetAcquisitionStep();
	bool IsFullMatrixCapture(int &iElementStart,int &iElementStop,int &iElementStep);
	int GetLostAscan();
	int GetLostCscan();
	int GetLostEncoder();
	int GetLostUSB3();
	bool IsFifoAscan();
	void ResetCounters();
	void UpdateFifo(bool bFifo);
	
	//acquisition Ascan functions for both FIFO enabled and disabled:
	bool GetAcquisitionAscanCount(__int64 &i64Count, __int64 &i64TotalByte);//Total ascan count that have been received by the computer.
	//functions for NO FIFO:
	bool GetAcquisitionAscanEncoder(int iEncoderIndex,double &dEncoder,long &lRawEncoder);
	DWORD GetAcquisitionAscanSize(int iCycle);
	bool SetAcquisitionAscanSize(int iCycle,int iSize);
	enumBitSize GetAcquisitionAscanBitSize(int iCycle, bool &bSigned);//1.1.5.2n
	bool GetAcquisitionAscanData(int iCycle,int iSizeMax,BYTE *pBuffer,int &iSize);
	//functions for FIFO (ENABLED):
	bool GetAcquisitionAscanFifoItem(long long sequence, int iCycle, int &iItem);
	bool GetAcquisitionAscanLifoItem(long long sequence, int iCycle, int &iItem);
	bool GetAcquisitionAscanItemInc(int &iItem);
	bool GetAcquisitionAscanItemDec(int &iItem);
	bool GetAcquisitionAscanFifoData(bool bPeek, int iItemCount, int *pItem, int iSizeMax, BYTE *pBuffer, int &iSize, int *piCycle, LONGLONG *pSequence, int iSizeEncoder, double *pdEncoder, long *plRawEncoder);
	bool GetAcquisitionAscanFifoStatus(int &iAscanCount, int &iDataLost, __int64 &iTotalCount, __int64 &iTotalByteCount);
	bool FlushAcquisitionAscanFifo();
	bool DumpAcquisitionAscanFifo(const wchar_t *wcFileName);
	bool GetAcquisitionAscanFifoInfo(int iItem, int &iCycle, long long &llSequence, int &iPointCount, BYTE &byByteSize, bool &bSigned);
	int GetAcquisitionIndex(bool bAscan, bool bFifo, int iCycleCount, int *piCycleIn, int *piItem, int *piCycle, long long *pllSequence, int *piPointCount, BYTE *pbyByteSize, bool *pbSigned);

	//acquisition Cscan functions for both FIFO enabled and disabled:
	bool GetAcquisitionCscanCount(__int64 &i64Count);
	//functions for NO FIFO:
	bool GetAcquisitionCscanEncoder(int iEncoderIndex,double &dEncoder,long &lRawEncoder);
	bool GetAcquisitionCscanEnable(int iGate,int iSizeMax,BYTE *pBuffer,int &iSize);
	bool GetAcquisitionCscanAmplitude(int iGate,int iSizeMax,BYTE *pBuffer,int &iSize);
	bool GetAcquisitionCscanTimeOfFlight(int iGate,int iSizeMax,short *pBuffer,int &iSize);
	//functions for FIFO (ENABLED):
	bool GetAcquisitionCscanData(bool bFifo, bool bPeek, int iItemCount, int *pItem, BYTE *pAmp, short *psTof, int *piCycle, long long *pllSequence, int iSizeEncoder, double *pdEncoder, long *plRawEncoder);
	bool GetAcquisitionCscanFifoStatus(int &iAscanCount, int &iDataLost);//current ascan count in the Fifo.
	bool FlushAcquisitionCscanFifo();

	//example of functions linked with matlab
	bool Example1();
	bool Example2(int iSizeIn, double *pDataIn,int *piSizeOut, double *pDataOut);

	CHWDeviceOEMPA m_hwDeviceOEMPA;
#ifdef _DEVICE_OEMPA_256_256_
	mCHWDeviceOEMPA256 m_csDevice256;
#endif _DEVICE_OEMPA_256_256_
private:
	CRITICAL_SECTION m_AcquisitionAscan;
	CRITICAL_SECTION m_AcquisitionCscan;
	bool m_bToolbox;
	//setting
	int m_iCycleCount;
	int m_iHWCycleCount;
	bool m_bEnableFMC;
	int m_iFMCElementStart;
	int m_iFMCElementStop;
	int m_iFMCElementStep;
	double m_dAcquisitionStep;//0.0 in case of internal trigger.
	//acquisition Ascan
	bool m_bFifo;
	enumBitSize *m_peBitSize;
	bool *m_pbSigned;
	unsigned long long m_ullAcquisitionTotalAscanCount;
	unsigned long long m_ullAcquisitionTotalByteCount;
	int m_iAcquisitionAscanSize[g_iOEMPACycleCountMax];
	int m_iOEMPACycleCountMax, m_iAscanPointCount;
	BYTE *m_pbyAcquisitionAscanBuffer;
	long m_lAcquisitionAscanRawEncoder[g_iDriverEncoderCountMax];//resolution is not taken into account.
	double m_dAcquisitionAscanEncoder[g_iDriverEncoderCountMax];//resolution is taken into account.
	CAcquisitionFifo *m_pFifoAscan;
	//acquisition Cscan
	unsigned long long m_ullAcquisitionTotalCscanCount;
	bool *m_pbAcquisitionCscanEnable[4];//m_bAcquisitionCscanEnable[4][g_iOEMPACycleCountMax];
	BYTE *m_pbyAcquisitionCscanAmplitude[4];//m_byAcquisitionCscanAmplitude[4][g_iOEMPACycleCountMax];
	short *m_psAcquisitionCscanTimeOfFlight[4];//m_sAcquisitionCscanTimeOfFlight[4][g_iOEMPACycleCountMax];
	long m_lAcquisitionCscanRawEncoder[g_iDriverEncoderCountMax];//resolution is not taken into account.
	double m_dAcquisitionCscanEncoder[g_iDriverEncoderCountMax];//resolution is taken into account.
	CAcquisitionFifo *m_pFifoCscan;
};


