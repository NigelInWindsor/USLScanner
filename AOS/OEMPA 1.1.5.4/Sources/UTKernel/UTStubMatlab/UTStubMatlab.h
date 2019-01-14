// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UTSTUBMATLAB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UTSTUBMATLAB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef UTSTUBMATLAB_EXPORTS
#define UTSTUBMATLAB_API __declspec(dllexport)
#define mxArray	void
//from "Matrix.h"
typedef enum
{
    mxUNKNOWN_CLASS = 0,
    mxCELL_CLASS,
    mxSTRUCT_CLASS,
    mxLOGICAL_CLASS,
    mxCHAR_CLASS,
    mxVOID_CLASS,
    mxDOUBLE_CLASS,
    mxSINGLE_CLASS,
    mxINT8_CLASS,
    mxUINT8_CLASS,
    mxINT16_CLASS,
    mxUINT16_CLASS,
    mxINT32_CLASS,
    mxUINT32_CLASS,
    mxINT64_CLASS,
    mxUINT64_CLASS,
    mxFUNCTION_CLASS,
    mxOPAQUE_CLASS,
    mxOBJECT_CLASS, /* keep the last real item in the list */
#if defined(_LP64) || defined(_WIN64)
    mxINDEX_CLASS = mxUINT64_CLASS,
#else
    mxINDEX_CLASS = mxUINT32_CLASS,
#endif
    /* TEMPORARY AND NASTY HACK UNTIL mxSPARSE_CLASS IS COMPLETELY ELIMINATED */
    mxSPARSE_CLASS = mxVOID_CLASS /* OBSOLETE! DO NOT USE */
}
mxClassID;
#else
#define UTSTUBMATLAB_API __declspec(dllimport)
#endif

//CRASH if "mex_MALLOC" is not defined or if "mex_MALLOC" and "mex_MALLOC_PERSISTENT" are both defined.
//#define mex_MALLOC
//#define mex_MALLOC_PERSISTENT
#define g_iOEMPAFilterCoefficientMax	34
#define g_iBufferSize					16384
#define	VALUE_OUT						plhs[0]
#define DefineGlobalVariable(type) p##type g_p##type;
#define DefineProcAddress(hModule,var_type,error) \
	if(!hModule)\
	{\
		error++;\
		mxMain.g_pmexPrintf("error function \"%s\" unavailabled!\n",#var_type);\
	}else{\
		mxMain.g_p##var_type = (p##var_type)GetProcAddress(hModule,#var_type);\
		if(!mxMain.g_p##var_type)\
		{\
			error++;\
			mxMain.g_pmexPrintf("error function \"%s\" unavailabled!\n",#var_type);\
		}\
		/*else mexPrintf("function \"%s\" ok!\n",#var_type);*/\
	}

typedef struct structDimension{
	int number_of_dimensions;
	int dims[4];
}structDimension;

typedef bool (*pmxStubLibrary)(bool bLoad, const char *pStubMatlab, char *pThisFileVersion, char *pThisProductVersion);
typedef void (*pmxEnter)();
typedef void (*pmxExit)();
typedef void (*pmxStackTest)(BYTE *pBuffer,int iSize);
typedef bool (*pmxCheckCompilation)(bool bDebug);
typedef bool (*pmxGeneralProcess)(void *pThis);
typedef bool (*pmxDeviceProcess)(void *pThis);
typedef bool (*pmxDevice256Process)(void *pThis);

typedef void (*pmexErrMsgTxt)(const char *errormsg);
typedef int (*pmexPrintf)(const char *message, ...);
typedef mxArray* (*pmxCreateDummy)();
typedef mxArray* (*pmxCreateString2)(const wchar_t *pIn);
typedef mxArray* (*pmxCreateByteResult0)(BYTE byData2);
typedef mxArray* (*pmxCreateByteResult1)(int iSize,BYTE *pbyData2);
typedef mxArray* (*pmxCreateAscanResult2)(int iRow, int iCol, int iByteSize, bool bSigned, BYTE** ppbyData2);
typedef mxArray* (*pmxCreateCharResult1)(int iSize,char *pbyData2);
typedef mxArray* (*pmxCreateCharResult2)(int iRow,int iCol,char *pData2);
typedef mxArray* (*pmxCreateWordResult1)(int iSize,WORD *pwData2);
typedef mxArray* (*pmxCreateShortResult1)(int iSize,short *psData2);
typedef mxArray* (*pmxCreateShortResult2)(int iRow,int iCol,short *psData2);
typedef mxArray* (*pmxCreateLongResult1)(int iSize,long *pdwData2);
typedef mxArray* (*pmxCreateLongResult2)(int iRow,int iCol,long *pdwData2);
typedef mxArray* (*pmxCreateLongLongResult1)(int iSize,long long *pdwData2);
typedef mxArray* (*pmxCreateLongLongResult2)(int iRow,int iCol,long long *pdwData2);
typedef mxArray* (*pmxCreateDWORDResult1)(int iSize,unsigned long *pdwData2);
typedef mxArray* (*pmxCreateIntegerResult2d)(int iRow,int iCol,double *pdData2);
typedef mxArray* (*pmxCreateIntegerResult2)(int iRow,int iCol,int *piData2);
typedef mxArray* (*pmxCreateDoubleResult0)(double dData);
typedef mxArray* (*pmxCreateDoubleResult1)(int iSize,double *pData2);
typedef mxArray* (*pmxCreateDoubleResult2)(int iRow,int iCol,double *pdData2);

//DRIVER MANAGEMENT
typedef mxClassID (*pmxGetClassID)(const mxArray *pm);
typedef bool (*pmxIsSparse)(const mxArray *pm);
typedef double *(*pmxGetPr)(const mxArray *pm);
typedef void (*pmxSetPr)(mxArray *pm, double *pr);
typedef void *(*pmxGetData)(const mxArray *pm);
typedef void (*pmxSetData)(mxArray *pm, void *pr);
typedef void (*pmxGetDimension)(const mxArray *pm, structDimension *dim);
typedef void* (*pmxMalloc2)(size_t size);
typedef void *(*pmxCalloc2)(size_t n, size_t size);
typedef void (*pmxFree)(void *ptr);
typedef void (*pmexMakeMemoryPersistent)(void *ptr);
typedef void (*pmexMakeArrayPersistent)(mxArray *pm);
typedef const char *(*pmexFunctionName)(void);
typedef mxArray *(*pmexGetVariable)(const char *workspace, const char *varname);
typedef int (*pmexPutVariable)(const char *workspace, const char *varname, const mxArray *pm);
typedef void (*pmxDestroyArray)(mxArray *pm);
#if defined (_DEBUG)
typedef void (*pmxDelete)(BYTE *pBuffer, int iLine, char *file);
typedef BYTE* (*pmxNew)(int iSize, int iLine, char *file);
#else
typedef void (*pmxDelete)(BYTE *pBuffer);
typedef BYTE* (*pmxNew)(int iSize);
#endif
typedef bool (*pmxAutoDisconnect)(bool bEnable);
typedef int (*pmxNewDevice)(wchar_t *pIPAddress, int iOEMPACycleCountMax, int iAscanPointCount, int bEnableFifo);
typedef bool (*pmxDeleteDevice)(int iDeviceId);
typedef bool (*pmxFindDevice)(int iDeviceId);
typedef DWORD (*pmxGetCfgKernelFile)(wchar_t *pSection,wchar_t *pKey,DWORD dwDefault);
typedef bool (*pmxSetCfgKernelFile)(wchar_t *pSection,wchar_t *pKey,DWORD dwValue);
typedef bool (*pmxSaveRegistryString)(wchar_t *wcSection,wchar_t *wcKey,wchar_t *pValue);
typedef const wchar_t *(*pmxGetFolderExe)();
typedef bool (*pmxReadFileWriteHW)(int iDeviceId,wchar_t *pFileName);
typedef bool (*pmxReadHWWriteFile)(int iDeviceId,wchar_t *pFileName);
typedef bool (*pmxConnect)(int iDeviceId,bool bValue);
typedef bool (*pmxIsConnected)(int iDeviceId);
typedef bool (*pmxIsUsb3Connected)(int iDeviceId);
typedef bool (*pmxIsFifo)(int iDeviceId);
typedef bool (*pmxIsFullMatrixCaptureReadWrite)(int iDeviceId);
typedef bool (*pmxResetCounters)(int iDeviceId);
typedef bool (*pmxResetEncoder)(int iDeviceId);
typedef bool (*pmxGetSerialNumber)(int iDeviceId, wchar_t *wcSerialNumber, int iSize);
typedef bool (*pmxGetStatus)(int iDeviceId, wchar_t *wcStatus, int iSize);
typedef bool (*pmxEnableShot)(int iDeviceId,bool bValue);
typedef bool (*pmxEnableUsb3)(int iDeviceId,bool bValue);
typedef double (*pmxGetAcquisitionStep)(int iDeviceId);
typedef int (*pmxGetSWCycleCount)(int iDeviceId);
typedef int (*pmxGetHWCycleCount)(int iDeviceId);
typedef bool (*pmxPerformCalibration)(int iDeviceId, double *pdTimeOffset, double *pdDelayCorrectionMax);
typedef bool (*pmxEnableAlignment)(int iDeviceId, bool bEnable);
typedef bool (*pmxSetTimeOffset)(int iDeviceId, double dTimeOffset);
typedef bool (*pmxSetDelayOffset)(int iDeviceId, double dDelayOffset);
typedef bool (*pmxGetFullMatrixCapture)(int iDeviceId,int *piElementStart,int *piElementStop,int *piElementStep);
typedef bool (*pmxGetAcquisitionAscanCount)(int iDeviceId, __int64 &i64Count, __int64 &i64TotalByte);
typedef bool (*pmxGetAcquisitionAscanEncoder)(int iDeviceId,int iEncoderIndex,double *pdEncoder,long *plRawEncoder);
typedef DWORD (*pmxGetAcquisitionAscanSize)(int iDeviceId,int iCycle);
typedef enum enumBitSize (*pmxGetAcquisitionAscanBitSize)(int iDeviceId,int iCycle,bool *pbSigned);
typedef bool (*pmxGetAcquisitionAscanData)(int iDeviceId,int iCycle,int iSizeMax,BYTE *pBuffer,int *piSize);
typedef bool (*pmxDumpAcquisitionAscanFifo)(int iDeviceId, wchar_t *wcFileName);
typedef bool (*pmxGetAcquisitionAscanFifoStatus)(int iDeviceId, long *plAscanCount, long *plDataLost, long long *pllTotalCount, long long *pllTotalByteCount);
typedef int (*pmxGetAcquisitionAscanFifoIndex)(int iDeviceId, bool bFifo, int iCycleCount, int *piCycleIn, int *piItem, int *piCycle, long long *pllSequence, int *piPointCount, BYTE *pbyByteSize, bool *pbSigned);
typedef int (*pmxGetAcquisitionAscanLifoIndex)(int iDeviceId, bool bFifo, int iCycleCount, int *piCycleIn, int *piItem, int *piCycle, long long *pllSequence, int *piPointCount, BYTE *pbyByteSize, bool *pbSigned);
typedef bool (*pmxGetAcquisitionAscanFifoInfo)(int iDeviceId, int iItem, int *piCycle, long long *pllSequence, int *piPointCount, BYTE *pbyByteSize, bool *pbSigned);
typedef bool (*pmxGetAcquisitionAscanFifoData)(int iDeviceId, bool bPeek, int iItemCount, int *pItem, int iSizeMax, BYTE *pBuffer, int *piSize, int *piCycle, long long *pSequence, int iSizeEncoder, double *pdEncoder, long *plRawEncoder);
typedef bool (*pmxFlushAcquisitionAscanFifo)(int iDeviceId);
typedef bool (*pmxFlushAcquisitionCscanFifo)(int iDeviceId);
typedef bool (*pmxGetAcquisitionCscanFifoStatus)(int iDeviceId, long *plAscanCount, long *plDataLost);
typedef int (*pmxGetAcquisitionCscanFifoIndex)(int iDeviceId, bool bFifo, int iCycleCount, int *piCycleIn, int *piItem, int *piCycle, long long *pllSequence);
typedef int (*pmxGetAcquisitionCscanLifoIndex)(int iDeviceId, bool bFifo, int iCycleCount, int *piCycleIn, int *piItem, int *piCycle, long long *pllSequence);
typedef bool (*pmxGetAcquisitionCscanFifoData)(int iDeviceId, bool bPeek, int iItemCount, int *pItem, BYTE *pAmp, short *psTof, int *piCycle, LONGLONG *pSequence, int iSizeEncoder, double *pdEncoder, long *plRawEncoder);
typedef bool (*pmxGetAcquisitionCscanCount)(int iDeviceId, __int64 &i64Count);
typedef bool (*pmxGetAcquisitionCscanEncoder)(int iDeviceId,int iEncoderIndex,double *pdEncoder,long *plRawEncoder);
typedef bool (*pmxGetAcquisitionCscanEnable)(int iDeviceId,int iGate,int iSizeMax,BYTE *pBuffer,int *piSize);
typedef bool (*pmxGetAcquisitionCscanAmplitude)(int iDeviceId,int iGate,int iSizeMax,BYTE *pBuffer,int *piSize);
typedef bool (*pmxGetAcquisitionCscanTimeOfFlight)(int iDeviceId,int iGate,int iSizeMax,short *pBuffer,int *piSize);
typedef bool (*pmxSetGainAnalog)(int iDeviceId,int iCycle,float *pfGain);
typedef bool (*pmxSetGainDigital)(int iDeviceId,int iCycle,double *pdGain);
typedef bool (*pmxSetAscanRange)(int iDeviceId,int iCycle,double *pdRange, BYTE *pbCompression, long *lPointCount, long *lPointFactor);///*output only*/enumCompressionType &eCompressionType,/*output only*/long &lPointCount,/*output only*/long &lPointFactor);
typedef bool (*pmxSetAscanRangeWithFactor)(int iDeviceId,int iCycle,double *pdRange, BYTE *pbCompression, long *lPointFactor, long *lPointCount);///*output only*/enumCompressionType &eCompressionType,/*output only*/long &lPointCount,/*output only*/long &lPointFactor);
typedef bool (*pmxSetAscanStart)(int iDeviceId,int iCycle,double *pdStart);
typedef bool (*pmxSetFilterIndex)(int iDeviceId,int iCycle,int iFilterIndex);
typedef bool (*pmxSetFilter)(int iDeviceId,int iFilterIndex, WORD *wScale, short wValue[g_iOEMPAFilterCoefficientMax]);
typedef bool (*pmxLockDevice)(int iDeviceId, int eAcqState);
typedef bool (*pmxUnlockDevice)(int iDeviceId, int eAcqState);
typedef int (*pmxGetLostCountAscan)(int iDeviceId);
typedef int (*pmxGetLostCountCscan)(int iDeviceId);
typedef int (*pmxGetLostCountEncoder)(int iDeviceId);
typedef int (*pmxGetLostCountUSB3)(int iDeviceId);
typedef bool (*pmxGetRecoveryTimes)(int iDeviceId, double &dAscanRecoveryTime, double &dFMCSubCycleRecoveryTime, double &dFMCCycleRecoveryTime);
typedef bool (*pmxGetFMCTimeLimitation)(int iDeviceId,int iFMCSubCycleCount,double dAscanStart,double dAscanRange,double dTimeSlot,double *pdTimeSlotMin,double *pdHWAcqSubTimeSlotMin,double *pdReplayAcqSubTimeSlotMin,double *pdReplayAcqSubTimeSlotOptimizedForThroughput);
typedef bool (*pmxSetFMCSubTimeSlot)(int iDeviceId,int iCycle,double dFMCSubTimeSlotAcq,double dFMCSubTimeSlotReplay);
typedef bool (*pmxSetTimeSlot)(int iDeviceId,int iCycle,double dTimeSlot);
typedef bool (*pmxExample1)(int iDeviceId);
typedef bool (*pmxExample2)(int iDeviceId,int iSizeIn, double *pDataIn,int *piSizeOut, double *pDataOut);
typedef bool (*pmxGetTraceGlobalFileName)(int iSize,wchar_t *pFilePath);
typedef bool (*pmxEnableTraceGlobal)(bool bEnable);
typedef bool (*pmxIsTraceGlobalEnabled)();
typedef void (*pmxTrace)(const wchar_t *pString);
typedef double* (*pmxAllocArrayDouble)(int iSize,double *pBuffer);

//256:256
typedef bool (*pmxReadFileWriteHW256)(int iDeviceIdMaster, int iDeviceIdSlave, wchar_t *pFileNameMaster, wchar_t *pFileNameSlave, int *piCyclesMaster, int *piCyclesSlave);
typedef bool (*pmxEnableShot256)(int iDeviceIdMaster,int iDeviceIdSlave, bool bValue);
typedef bool (*pmxGetAcquisitionAscanFifoStatus256)(int iDeviceId, long *plAscanCount, long *plDataLost, long *plAscanTotal, long long *plByteTotal);
typedef bool (*pmxLockDevice256)(int iDeviceIdMaster, int iDeviceIdSlave, int eAcqState);
typedef bool (*pmxUnlockDevice256)(int iDeviceIdMaster, int iDeviceIdSlave, int eAcqState);
typedef bool (*pmxResetCounters256)(int iDeviceIdMaster, int iDeviceIdSlave);
typedef bool (*pmxFlushAcquisitionAscanFifo256)(int iDeviceIdMaster, int iDeviceIdSlave);

//TOOLBOX MANAGEMENT
typedef bool (*pmxToolboxRun)(bool bShowWindow);
typedef bool (*pmxIsToolboxRunning)();
typedef bool (*pmxToolboxClose)();
typedef DWORD (*pmxToolboxEcho)(DWORD dwData);
typedef bool (*pmxToolboxVisible)(bool bEnable);
typedef bool (*pmxEnableEventUpdate)(bool bEnable);
typedef bool (*pmxToolboxDisplay)(int iRootId);
typedef bool (*pmxToolboxWizardNew)();
typedef bool (*pmxToolboxWizardDelete)();
typedef bool (*pmxToolboxWizardUpdateScan)();
typedef bool (*pmxToolboxIsWizardEnabled)();
typedef bool (*pmxToolboxWizardReset)();
typedef int (*pmxGetWizardBeamCount)();
typedef bool (*pmxGetWizardBeam)(int iBeamIndex,double *pdProbeX,double *pdProbeY,double *pdProbeZ,double *pdEmergenceX,double *pdEmergenceY,double *pdEmergenceZ,double *pdFocalX,double *pdFocalY,double *pdFocalZ);
typedef int (*pmxGetWizardElementCount)();
typedef bool (*pmxGetWizardElement)(int iProbeIndex,double *pdProbeX,double *pdProbeY,double *pdProbeZ);
typedef bool (*pmxGetToolboxParameter)(wchar_t *pIn,int iSize,wchar_t *pOut);
typedef bool (*pmxSetToolboxParameter)(wchar_t *pIn);
typedef bool (*pmxToolboxSaveSetup)(int iDeviceId,wchar_t *pSetupFile);
typedef bool (*pmxToolboxSaveSetupEx)(int iDeviceId,wchar_t *pSetupFileIn,wchar_t *pSetupFileOut);
typedef bool (*pmxToolboxLoadWizard)(wchar_t *pWizardFile);
typedef bool (*pmxToolboxSaveWizard)(wchar_t *pWizardFile,bool bSaveWizardResults);
typedef bool (*pmxToolboxLoadMatrix)(wchar_t *pMatrixFile);
typedef bool (*pmxToolboxSaveMatrix)(wchar_t *pMatrixFile);
typedef int (*pmxGetToolboxDeviceId)();
typedef int (*pmxGetToolboxObjectId)(wchar_t *pRootName);
typedef bool (*pmxSetToolboxDeviceAddress)(wchar_t *pIPAddressAndPort);
typedef bool (*pmxToolboxDeviceConnection)(bool bConnect);
typedef bool (*pmxToolboxIsDeviceConnected)();
typedef const wchar_t *(*pmxGetToolboxDeviceType)();

typedef void (*pmxTraceMemory)(char *message);


class CStubMatlab
{
public:
#ifdef _DEBUG
	void *mxNew(int iSize, int iLine, char *file);
	void mxDelete(void *pBuffer, int iLine, char *file);
#else _DEBUG
	void *mxNew(int iSize);
	void mxDelete(void *pBuffer);
#endif _DEBUG

	bool mxGeneralProcess();
	bool mxDeviceProcess();
	bool mxDevice256Process();
	bool mxAscanFifoIndex();
	bool mxAscanFifoData();
	void mexErrMsgTxt(const char *errormsg);
	mxClassID mxGetClassID(const mxArray *pm);
	bool mxIsSparse(const mxArray *pm);
	double *mxGetPr(const mxArray *pm);
	void mxSetPr(mxArray *pm, double *pr);
	void *mxGetData(const mxArray *pm);
	void mxSetData(mxArray *pm, void *pr);
	void mxGetDimension(const mxArray *pm, structDimension *dim);
	void *mxMalloc2(size_t size);
	void *mxCalloc2(size_t n, size_t size);
	void mxFree(void *ptr);
	void mexMakeMemoryPersistent(void *ptr);
	void mexMakeArrayPersistent(mxArray *pm);
	const char *mexFunctionName(void);
	mxArray *mexGetVariable(const char *workspace, const char *varname);
	int mexPutVariable(const char *workspace, const char *varname, const mxArray *pm);
	void mxDestroyArray(mxArray *pm);
	mxArray* mxCreateDummy();
	mxArray* mxCreateString2(const wchar_t *pIn);
	mxArray* mxCreateByteResult0(BYTE byData2);
	mxArray* mxCreateByteResult1(int iSize,BYTE *pbyData2);
	mxArray* mxCreateAscanResult2(int iRow, int iCol, int iByteSize, bool bSigned, BYTE** ppbyData2);
	mxArray* mxCreateCharResult1(int iSize,char *pbyData2);
	mxArray* mxCreateCharResult2(int iRow,int iCol,char *pData2);
	mxArray* mxCreateWordResult1(int iSize,WORD *pwData2);
	mxArray* mxCreateShortResult1(int iSize,short *psData2);
	mxArray* mxCreateShortResult2(int iRow,int iCol,short *psData2);
	mxArray* mxCreateLongResult1(int iSize,long *pdwData2);
	mxArray* mxCreateLongResult2(int iRow,int iCol,long *pdwData2);
	mxArray* mxCreateLongLongResult1(int iSize,long long *pdwData2);
	mxArray* mxCreateLongLongResult2(int iRow,int iCol,long long *pdwData2);
	mxArray* mxCreateDWORDResult1(int iSize,unsigned long *pdwData2);
	mxArray* mxCreateIntegerResult2d(int iRow,int iCol,double *pdData2);
	mxArray* mxCreateIntegerResult2(int iRow,int iCol,int *piData2);
	mxArray* mxCreateDoubleResult0(double dData);
	mxArray* mxCreateDoubleResult1(int iSize,double *pData2);
	mxArray* mxCreateDoubleResult2(int iRow,int iCol,double *pdData2);

	int nlhs;
	int nrhs;
	mxArray **plhs;
	const mxArray **prhs;
	int number_of_dimensions;
	int dims[2];
	int iId,iId2;
	wchar_t *wcFunc;
	wchar_t *wcString;
	wchar_t *wcName;
	wchar_t *wcName5;
	void *pMatrix;
	double *pdMatrix;
	int iMatrixClass;

	pmxStubLibrary g_pmxStubLibrary;
	pmxEnter g_pmxEnter;
	pmxExit g_pmxExit;
	pmxStackTest g_pmxStackTest;
	pmxCheckCompilation g_pmxCheckCompilation;
	pmxGeneralProcess g_pmxGeneralProcess;
	pmxDeviceProcess g_pmxDeviceProcess;
	pmxDevice256Process g_pmxDevice256Process;
	pmxExit g_pExitDll;

	//utCmd.c
	//pmxPrintf g_pmexPrintf;
	DefineGlobalVariable(mexPrintf)
	DefineGlobalVariable(mexErrMsgTxt)
	DefineGlobalVariable(mxGetClassID)
	DefineGlobalVariable(mxIsSparse)
	DefineGlobalVariable(mxGetPr)
	DefineGlobalVariable(mxSetPr)
	DefineGlobalVariable(mxGetData)
	DefineGlobalVariable(mxSetData)
	DefineGlobalVariable(mxGetDimension)
	DefineGlobalVariable(mxMalloc2)
	DefineGlobalVariable(mxCalloc2)
	DefineGlobalVariable(mxFree)
	DefineGlobalVariable(mexMakeMemoryPersistent);
	DefineGlobalVariable(mexMakeArrayPersistent);
	DefineGlobalVariable(mexFunctionName);
	DefineGlobalVariable(mexGetVariable);
	DefineGlobalVariable(mexPutVariable);
	DefineGlobalVariable(mxDestroyArray)
	DefineGlobalVariable(mxCreateDummy)
	DefineGlobalVariable(mxCreateString2)
	DefineGlobalVariable(mxCreateByteResult0)
	DefineGlobalVariable(mxCreateByteResult1)
	DefineGlobalVariable(mxCreateAscanResult2)
	DefineGlobalVariable(mxCreateCharResult1)
	DefineGlobalVariable(mxCreateCharResult2)
	DefineGlobalVariable(mxCreateWordResult1)
	DefineGlobalVariable(mxCreateShortResult1)
	DefineGlobalVariable(mxCreateShortResult2)
	DefineGlobalVariable(mxCreateLongResult1)
	DefineGlobalVariable(mxCreateLongResult2)
	DefineGlobalVariable(mxCreateLongLongResult1)
	DefineGlobalVariable(mxCreateLongLongResult2)
	DefineGlobalVariable(mxCreateDWORDResult1)
	DefineGlobalVariable(mxCreateIntegerResult2d)
	DefineGlobalVariable(mxCreateIntegerResult2)
	DefineGlobalVariable(mxCreateDoubleResult0)
	DefineGlobalVariable(mxCreateDoubleResult1)
	DefineGlobalVariable(mxCreateDoubleResult2)

	//DRIVER MANAGEMENT
	DefineGlobalVariable(mxDelete)
	DefineGlobalVariable(mxNew)
	DefineGlobalVariable(mxAutoDisconnect)
	DefineGlobalVariable(mxNewDevice)
	DefineGlobalVariable(mxDeleteDevice)
	DefineGlobalVariable(mxFindDevice)
	DefineGlobalVariable(mxGetCfgKernelFile)
	DefineGlobalVariable(mxSetCfgKernelFile)
	DefineGlobalVariable(mxSaveRegistryString)
	DefineGlobalVariable(mxGetFolderExe)
	DefineGlobalVariable(mxReadFileWriteHW)
	DefineGlobalVariable(mxReadHWWriteFile)
	DefineGlobalVariable(mxConnect)
	DefineGlobalVariable(mxIsConnected)
	DefineGlobalVariable(mxIsUsb3Connected)
	DefineGlobalVariable(mxIsFifo)
	DefineGlobalVariable(mxIsFullMatrixCaptureReadWrite)
	DefineGlobalVariable(mxResetCounters)
	DefineGlobalVariable(mxResetEncoder)
	DefineGlobalVariable(mxGetSerialNumber)
	DefineGlobalVariable(mxGetStatus)
	DefineGlobalVariable(mxEnableShot)
	DefineGlobalVariable(mxEnableUsb3)
	DefineGlobalVariable(mxGetAcquisitionStep)
	DefineGlobalVariable(mxGetSWCycleCount)
	DefineGlobalVariable(mxGetHWCycleCount)
	DefineGlobalVariable(mxPerformCalibration)
	DefineGlobalVariable(mxEnableAlignment)
	DefineGlobalVariable(mxSetTimeOffset)
	DefineGlobalVariable(mxSetDelayOffset)
	DefineGlobalVariable(mxGetFullMatrixCapture)
	DefineGlobalVariable(mxGetAcquisitionAscanCount)
	DefineGlobalVariable(mxGetAcquisitionAscanEncoder)
	DefineGlobalVariable(mxGetAcquisitionAscanSize)
	DefineGlobalVariable(mxGetAcquisitionAscanBitSize)
	DefineGlobalVariable(mxGetAcquisitionAscanData)
	DefineGlobalVariable(mxDumpAcquisitionAscanFifo)
	DefineGlobalVariable(mxGetAcquisitionAscanFifoStatus)
	DefineGlobalVariable(mxGetAcquisitionAscanFifoIndex)
	DefineGlobalVariable(mxGetAcquisitionAscanLifoIndex)
	DefineGlobalVariable(mxGetAcquisitionAscanFifoInfo)
	DefineGlobalVariable(mxGetAcquisitionAscanFifoData)
	DefineGlobalVariable(mxFlushAcquisitionAscanFifo)
	DefineGlobalVariable(mxFlushAcquisitionCscanFifo)
	DefineGlobalVariable(mxGetAcquisitionCscanFifoStatus)
	DefineGlobalVariable(mxGetAcquisitionCscanFifoIndex)
	DefineGlobalVariable(mxGetAcquisitionCscanLifoIndex)
	DefineGlobalVariable(mxGetAcquisitionCscanFifoData)
	DefineGlobalVariable(mxGetAcquisitionCscanCount)
	DefineGlobalVariable(mxGetAcquisitionCscanEncoder)
	DefineGlobalVariable(mxGetAcquisitionCscanEnable)
	DefineGlobalVariable(mxGetAcquisitionCscanAmplitude)
	DefineGlobalVariable(mxGetAcquisitionCscanTimeOfFlight)
	DefineGlobalVariable(mxSetGainAnalog)
	DefineGlobalVariable(mxSetGainDigital)
	DefineGlobalVariable(mxSetAscanRange)
	DefineGlobalVariable(mxSetAscanRangeWithFactor)
	DefineGlobalVariable(mxSetAscanStart)
	DefineGlobalVariable(mxSetFilterIndex)
	DefineGlobalVariable(mxSetFilter)
	DefineGlobalVariable(mxLockDevice)
	DefineGlobalVariable(mxUnlockDevice)
	DefineGlobalVariable(mxGetLostCountAscan)
	DefineGlobalVariable(mxGetLostCountCscan)
	DefineGlobalVariable(mxGetLostCountEncoder)
	DefineGlobalVariable(mxGetLostCountUSB3)
	DefineGlobalVariable(mxGetRecoveryTimes)
	DefineGlobalVariable(mxGetFMCTimeLimitation)
	DefineGlobalVariable(mxSetFMCSubTimeSlot)
	DefineGlobalVariable(mxSetTimeSlot)
	DefineGlobalVariable(mxExample1)
	DefineGlobalVariable(mxExample2)
	DefineGlobalVariable(mxGetTraceGlobalFileName)
	DefineGlobalVariable(mxIsTraceGlobalEnabled)
	DefineGlobalVariable(mxEnableTraceGlobal)
	DefineGlobalVariable(mxTrace)
	DefineGlobalVariable(mxAllocArrayDouble)

	//256:256
	DefineGlobalVariable(mxReadFileWriteHW256)
	DefineGlobalVariable(mxEnableShot256)
	DefineGlobalVariable(mxGetAcquisitionAscanFifoStatus256)
	DefineGlobalVariable(mxLockDevice256)
	DefineGlobalVariable(mxUnlockDevice256)
	DefineGlobalVariable(mxResetCounters256)
	DefineGlobalVariable(mxFlushAcquisitionAscanFifo256)

	//TOOLBOX MANAGEMENT
	DefineGlobalVariable(mxToolboxRun)
	DefineGlobalVariable(mxIsToolboxRunning)
	DefineGlobalVariable(mxToolboxVisible)
	DefineGlobalVariable(mxEnableEventUpdate)
	DefineGlobalVariable(mxToolboxClose)
	DefineGlobalVariable(mxToolboxEcho)
	DefineGlobalVariable(mxToolboxDisplay)
	DefineGlobalVariable(mxToolboxWizardNew)
	DefineGlobalVariable(mxToolboxWizardDelete)
	DefineGlobalVariable(mxToolboxWizardUpdateScan)
	DefineGlobalVariable(mxToolboxIsWizardEnabled)
	DefineGlobalVariable(mxGetWizardBeamCount)
	DefineGlobalVariable(mxGetWizardBeam)
	DefineGlobalVariable(mxToolboxWizardReset)
	DefineGlobalVariable(mxGetWizardElementCount)
	DefineGlobalVariable(mxGetWizardElement)
	DefineGlobalVariable(mxGetToolboxParameter)
	DefineGlobalVariable(mxSetToolboxParameter)
	DefineGlobalVariable(mxToolboxSaveSetup)
	DefineGlobalVariable(mxToolboxSaveSetupEx)
	DefineGlobalVariable(mxToolboxLoadWizard)
	DefineGlobalVariable(mxToolboxSaveWizard)
	DefineGlobalVariable(mxToolboxLoadMatrix)
	DefineGlobalVariable(mxToolboxSaveMatrix)
	DefineGlobalVariable(mxGetToolboxDeviceId)
	DefineGlobalVariable(mxGetToolboxObjectId)
	DefineGlobalVariable(mxSetToolboxDeviceAddress)
	DefineGlobalVariable(mxToolboxDeviceConnection)
	DefineGlobalVariable(mxToolboxIsDeviceConnected)
	DefineGlobalVariable(mxGetToolboxDeviceType)

	DefineGlobalVariable(mxTraceMemory)

};

