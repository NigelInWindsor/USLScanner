
#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DRIVEROEMPA_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DRIVEROEMPA_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DRIVEROEMPA_EXPORTS
#define DRIVEROEMPA_API __declspec(dllexport)
#define DRIVEROEMPA_API_ALIGN __declspec(dllexport) __declspec(align(8))
#else
#define DRIVEROEMPA_API __declspec(dllimport)
#define DRIVEROEMPA_API_ALIGN __declspec(dllimport) __declspec(align(8))
#endif


class CSWDevice;

const int g_iAcqSizeMax=8;
const int g_iSubSequenceTableSize=256;
const int g_iFMCTimeSlotCorrectionOffsetUsMin=9;//2 us=>data lost, 5=> data lost, 8 us or 10 us is OK.

const float g_fOEMPADACSlopeStep=(0.1f/(1024.0f*10.0e-9f));//9765.625 dB/sec
const float g_fOEMPADACSlopeMin=-32768.0f*g_fOEMPADACSlopeStep;//-320000000 dB/sec
const float g_fOEMPADACSlopeMax=+32767.0f*g_fOEMPADACSlopeStep;//319990234.375 dB/sec

const int g_iOEMPACycleCountBitMax2=12;
const int g_iOEMPACycleCountMax=256*256;//FMC g_iOEMPACycleCountMax=((2 << (g_iOEMPACycleCountBitMax2-1)));//4096, it depend of the device, it could be 2017 with old devices.
const int g_iOEMPADACCountMax=64;
const int g_iOEMPAFocalCountMax=64;//1=first focal law.
const int g_iOEMPAGateCountMax=4;
const int g_iOEMPAApertureElementCountMaxPerBoard=16;
const int g_iOEMPAApertureElementCountMax=g_iOEMPAApertureElementCountMaxPerBoard*g_iOEMPABoardCountMax;



typedef bool (WINAPI* pCallbackSetSize)(struct structCallbackArrayFloat1D *pCallbackArray,int iSize);
typedef bool (WINAPI* pCallbackSetData)(struct structCallbackArrayFloat1D *pCallbackArray,int iIndex,float fData);
typedef bool (WINAPI* pCallbackGetSize)(struct structCallbackArrayFloat1D *pCallbackArray,int &iSize);
typedef bool (WINAPI* pCallbackGetData)(struct structCallbackArrayFloat1D *pCallbackArray,int iIndex,float &fData);
typedef struct structCallbackArrayFloat1D
{
	void *apParameter[g_iCallbackArrayParameterCountMax];
	pCallbackSetSize pSetSize;
	pCallbackSetData pSetData;
	pCallbackGetSize pGetSize;
	pCallbackGetData pGetData;
} structCallbackArrayFloat1D;

typedef bool (WINAPI* pCallbackSetSizeDouble)(struct structCallbackArrayDouble1D *pCallbackArray,int iSize);
typedef bool (WINAPI* pCallbackSetDataDouble)(struct structCallbackArrayDouble1D *pCallbackArray,int iIndex,double fData);
typedef bool (WINAPI* pCallbackGetSizeDouble)(struct structCallbackArrayDouble1D *pCallbackArray,int &iSize);
typedef bool (WINAPI* pCallbackGetDataDouble)(struct structCallbackArrayDouble1D *pCallbackArray,int iIndex,double &fData);
typedef struct structCallbackArrayDouble1D
{
	void *apParameter[g_iCallbackArrayParameterCountMax];
	pCallbackSetSizeDouble pSetSize;
	pCallbackSetDataDouble pSetData;
	pCallbackGetSizeDouble pGetSize;
	pCallbackGetDataDouble pGetData;
} structCallbackArrayDouble1D;

typedef bool (WINAPI* pCallbackSetSize2)(struct structCallbackArrayFloat2D *pCallbackArray,int iSize1,int iSize2);
typedef bool (WINAPI* pCallbackSetData2)(struct structCallbackArrayFloat2D *pCallbackArray,int iIndex1,int iIndex2,float fData);
typedef bool (WINAPI* pCallbackGetSize2)(struct structCallbackArrayFloat2D *pCallbackArray,int &iSize1,int &iSize2);
typedef bool (WINAPI* pCallbackGetData2)(struct structCallbackArrayFloat2D *pCallbackArray,int iIndex1,int iIndex2,float &fData);
typedef struct structCallbackArrayFloat2D
{
	void *apParameter[g_iCallbackArrayParameterCountMax];
	pCallbackSetSize2 pSetSize;
	pCallbackSetData2 pSetData;
	pCallbackGetSize2 pGetSize;
	pCallbackGetData2 pGetData;
} structCallbackArrayFloat2D;

typedef bool (WINAPI* pCallbackSetSizeDac)(struct structCallbackArrayFloatDac *pCallbackArray,int iSize);
typedef bool (WINAPI* pCallbackSetDataDac)(struct structCallbackArrayFloatDac *pCallbackArray,int iIndex,double dTime,float fSlope);
typedef bool (WINAPI* pCallbackGetSizeDac)(struct structCallbackArrayFloatDac *pCallbackArray,int &iSize);
typedef bool (WINAPI* pCallbackGetDataDac)(struct structCallbackArrayFloatDac *pCallbackArray,int iIndex,double &dTime,float &fSlope);
typedef struct structCallbackArrayFloatDac
{
	void *apParameter[g_iCallbackArrayParameterCountMax];
	pCallbackSetSizeDac	pSetSize;
	pCallbackSetDataDac pSetData;
	pCallbackGetSizeDac	pGetSize;
	pCallbackGetDataDac pGetData;
} structCallbackArrayFloatDac;

typedef bool (WINAPI* pCallbackSetSizeByte1D)(struct structCallbackArrayByte1D *pCallbackArray,int iSize);
typedef bool (WINAPI* pCallbackSetDataByte1D)(struct structCallbackArrayByte1D *pCallbackArray,int iIndex,BYTE byData);
typedef bool (WINAPI* pCallbackGetSizeByte1D)(struct structCallbackArrayByte1D *pCallbackArray,int &iSize);
typedef bool (WINAPI* pCallbackGetDataByte1D)(struct structCallbackArrayByte1D *pCallbackArray,int iIndex,BYTE &byData);
typedef struct structCallbackArrayByte1D
{
	void *apParameter[g_iCallbackArrayParameterCountMax];
	pCallbackSetSizeByte1D pSetSize;
	pCallbackSetDataByte1D pSetData;
	pCallbackGetSizeByte1D pGetSize;
	pCallbackGetDataByte1D pGetData;
} structCallbackArrayByte1D;

class CHWDeviceOempa;
class CHWDeviceOEMPA;

typedef bool (WINAPI *pOEMPA_ReadFileWriteHW)(CHWDeviceOEMPA *pOEMPA,const wchar_t *pFileName,enumAcquisitionState eAcqState);
typedef bool (WINAPI *pOEMPA_ReadFileWriteHW256)(CHWDeviceOEMPA *pMaster,CHWDeviceOEMPA *pSlave,wchar_t *pFileNameMaster,wchar_t *pFileNameSlave);
typedef bool (WINAPI *pOEMPA_CreateDefaultSetupFile)(CHWDeviceOEMPA *pHWDeviceOEMPA,bool bOEMPA,wchar_t *wcLoadDefaultSetup);
typedef bool (WINAPI *pOEMPA_LoadCalibrationSetup)(	CHWDeviceOEMPA *pHWDeviceOEMPA,bool bCheck,
													float fWidth,float fStart,float fRange,float fGainAnalog,double dGainDigital,
													const wchar_t *wcCalibrationFileInput,const wchar_t *wcCalibrationFileOutput,
													double dFMCTimeSlotFactor,int &iCalibrationPointCount,double &dAcquisitionTime);

DRIVEROEMPA_API void WINAPI OEMPA_SetReadFileWriteHW(pOEMPA_ReadFileWriteHW pFunc);
DRIVEROEMPA_API void WINAPI OEMPA_SetReadFileWriteHW256(pOEMPA_ReadFileWriteHW256 pFunc);
DRIVEROEMPA_API void WINAPI OEMPA_SetCreateDefaultSetupFile(pOEMPA_CreateDefaultSetupFile pFunc);
DRIVEROEMPA_API void WINAPI OEMPA_SetLoadCalibrationSetup(pOEMPA_LoadCalibrationSetup pFunc);
DRIVEROEMPA_API void WINAPI OEMPA_SetLoadCalibrationSetup256(pOEMPA_LoadCalibrationSetup pFunc);
DRIVEROEMPA_API const wchar_t* WINAPI OEMPA_GetVersion();
DRIVEROEMPA_API DWORD WINAPI OEMPA_GetVersionMajor();
DRIVEROEMPA_API DWORD WINAPI OEMPA_GetVersionMinor();
DRIVEROEMPA_API const char OEMPA_GetVersionLetter();


class DRIVEROEMPA_API_ALIGN _CSWFilterOEMPA
{
private:
	ULONGLONG m_ullPrivateMemory[75];
protected:

};

class DRIVEROEMPA_API_ALIGN CSWFilterOEMPA : public _CSWFilterOEMPA
{
public:

public:
	CSWFilterOEMPA();
	~CSWFilterOEMPA();
	int operator=(const CSWFilterOEMPA &a);
	bool operator==(const CSWFilterOEMPA &a) const;
	bool operator!=(const CSWFilterOEMPA &a) const;

	//easy way to use filter (predefined filters)
	bool SetFilter(enumOEMPAFilter eFilter);
	bool GetFilter(enumOEMPAFilter &eFilter) const;
	bool SetTitle(const wchar_t *pComment);//useful for custom filter.
	bool GetTitle(wchar_t *pComment,int iSize) const;

	//custom filter coefficient: functions used by the "CustomFilter" software.
	bool SetScale(WORD wScale);
	bool GetScale(WORD &wScale) const;
	bool SetCoefficientCount(int iCoefficientCount);
	bool GetCoefficientCount(int &iCoefficientCount) const;
	bool SetCoefficient(int iCoefficientIndex,short wValue);
	bool GetCoefficient(int iCoefficientIndex,short &wValue) const;
	bool SetFilter(WORD wScale,short wValue[g_iOEMPAFilterCoefficientMax],bool bUpdateHardware=true);//if you want to update all hardware filter, it is quicker to call "CSWDeviceOEMPA::UpdateAllFilter" at the end and before to call "SetFilter" with "bUpdateHardware=false" for all filters.
	bool GetFilter(WORD &wScale,short wValue[g_iOEMPAFilterCoefficientMax]) const;
	bool SetArrayCoefficient(int iCoefficientCount,short *pwValue);
	bool GetArrayCoefficient(int iCoefficientCount,short *pwValue) const;


};


typedef struct structOEMPACycleParameter{
	DWORD m_dwTimeSlot;
	DWORD m_dwPulserCount;
	float m_fPulseWidthMax;
}structOEMPACycleParameter;

class DRIVEROEMPA_API_ALIGN _CSWDeviceOEMPA
{
private:
	ULONGLONG m_ullPrivateMemory[1672];
protected:

};

class DRIVEROEMPA_API_ALIGN CSWDeviceOEMPA : public _CSWDeviceOEMPA
{
public:
	CSWDeviceOEMPA(CHWDeviceOempa *pHWDeviceOempa,CHWDeviceOEMPA *pHWDeviceOEMPA);
	~CSWDeviceOEMPA();
	int operator=(const CSWDeviceOEMPA &a);
	bool operator==(const CSWDeviceOEMPA &a) const;
	bool operator!=(const CSWDeviceOEMPA &a) const;
	CSWFilterOEMPA& operator[](int iFilterIndex);
	CSWFilterOEMPA& Filter(int iFilterIndex);

	bool IsPulserEnabled();

//ethernet
	bool SetIPAddress(const wchar_t *pValue);
	bool _SetIPAddress(const wchar_t *pValue);//same than "SetIPAddress".
	bool GetIPAddress(wchar_t *pValue,int iSize) const;
	const wchar_t* GetIPAddress() const;

	bool SetPort(unsigned short usValue);
	bool _SetPort(unsigned short usValue);//same than "SetPort", required because of the preprocessor definition in "WinSpool.h": "#define SetPort SetPortW"
	bool GetPort(unsigned short &usValue) const;


//USB3
	bool IsUSB3Connected();
	CUTUsb3Device *GetUSB3Device();

//status registers (digital inputs and temperatures)
	bool SetStatusRegister(enumUpdateStatus eCom);//set UpToDate to refresh status register.
	enumUpdateStatus GetStatusRegister() const;


	bool GetSerialNumber(wchar_t *pSN,int iSize) const;


	bool GetSystemType(wchar_t *pType,int iSize) const;

	int GetRXBoardCount() const;

	int GetApertureCountMax() const;//to get the maximum element count of an aperture.
	int GetElementCountMax() const;//to get the maximum element count of the system (in case of mux).

	const double dGetClockPeriod() const;//second
	const float fGetClockPeriod() const;//second
	const long lGetClockFrequency() const;//Hz

	const float fFocussingDelayStepEmission() const;//second
	const float fFocussingDelayStepReception() const;//second

	WORD GetFirmwareId() const;


	int GetCycleCountMax() const;
	bool IsFullMatrixCapture() const;//FMC could be enabled or disabled.
	bool IsFullMatrixCaptureReadWrite() const;//FMC bit could be Read/Write or Read only (for old FMC FW: FMC bit is not read only).
	bool GetFMCElement(int &iElementStart,int &iElementStop,int &iElementStep) const;
	bool IsMultiHWChannelSupported() const;
	bool IsTemperatureAlarmSupported() const;
	bool IsMultiHWChannelEnabled() const;
	bool IsMatrixAvailable() const;
	bool IsLabviewAvailable() const;
	bool IsTpacquisitionAvailable() const;
	bool IsWTSWAvailable() const;
	bool IsMatlabAvailable() const;
	bool IsTFMAvailable() const;
	bool IsEncoderDecimal() const;
	bool IsTFMProcessed(structTFMParameters *ptfmParameters=NULL) const;
	bool IsFMCElementStepSupported() const;
	bool IsFMCSubTimeSlotPerCycleSupported() const;


	bool SetKeepAlive(enumKeepAlive eKeepAlive);
	enumKeepAlive GetKeepAlive() const;

	bool EnableAscan(bool bEnable);
	bool IsAscanEnabled() const;

	bool SetAscanBitSize(enumBitSize eBitSize);
	enumBitSize GetAscanBitSize() const;

	bool SetAscanRequest(enumAscanRequest eAscanRequest);
	enumAscanRequest GetAscanRequest() const;

	bool SetAscanRequestFrequency(double dFreq);//Hz
	bool GetAscanRequestFrequency(double &dFreq) const;

	bool EnableCscanTof(bool bEnable);
	bool IsCscanTofEnabled() const;

	bool SetCycleCount(int iCount);
	int GetCycleCount() const;


	bool SetTriggerMode(enumOEMPATrigger eTrig);
	enumOEMPATrigger GetTriggerMode() const;
	bool EnableSubSequenceEncoder(bool bEnable);
	bool IsSubSequenceEncoderEnabled() const;
	bool EnableSubSequenceGate(bool bEnable);
	bool IsSubSequenceGateEnabled() const;

	bool SetTriggerEncoderStep(double dStep);
	bool GetTriggerEncoderStep(double &dStep) const;

	bool SetSignalTriggerHighTime(double dTime);
	double GetSignalTriggerHighTime() const;

	bool SetRequestIO(enumOEMPARequestIO eRequest);
	enumOEMPARequestIO GetRequestIO() const;

	bool SetRequestIODigitalInputMaskRising(int iMask);
	bool GetRequestIODigitalInputMaskRising(int &iMask) const;

	bool SetRequestIODigitalInputMaskFalling(int iEvent);
	bool GetRequestIODigitalInputMaskFalling(int &iEvent) const;

	bool SetExternalTriggerCycle(enumDigitalInput eDigitalInput);
	bool GetExternalTriggerCycle(enumDigitalInput &eDigitalInput) const;

	bool SetExternalTriggerSequence(enumDigitalInput eDigitalInput);
	bool GetExternalTriggerSequence(enumDigitalInput &eDigitalInput) const;

	bool SetMappingOutput(int iOutputIndex,enumOEMPAMappingDigitalOutput eMapping);
	bool GetMappingOutput(int iOutputIndex,enumOEMPAMappingDigitalOutput &eMapping) const;

	bool SetRequestCscan(enumOEMPARequestCscan eRequest);
	enumOEMPARequestCscan GetRequestCscan() const;

	bool SetEncoderDebouncer(double dTime);
	double GetEncoderDebouncer() const;

	bool SetDigitalInput(WORD usValue);
	WORD GetDigitalInput() const;//return the general purpose digital inputs state coming from the HW (binary value).

	bool SetDigitalDebouncer(double dTime);
	double GetDigitalDebouncer() const;

	bool SetMultiSystemDebouncer(double dTime);
	double GetMultiSystemDebouncer() const;

	bool ResetEncoder();

	bool GetTemperatureCount(int &iBoardCount,int &iSensorCountMax) const;//SensorCount is not the same for each board.
	bool GetTemperatureSensorCount(int iBoardIndex,int &iSensorCount) const;//SensorCount is not the same for each board.
	int GetTemperature(int iBoardIndex,int iSensorIndex,float &fValue) const;
	int SetArrayTemperature(int iBoardIndex,int iSensorCount,float *pfValue);
	int GetArrayTemperature(int iBoardIndex,int iSensorCount,float *pfValue) const;


	//SW member to enable the high level API update of acquisition status, see preferences member "AcqStatusEnable" and "AcqStatusTimer".
	bool EnableTimerUpdate(bool bEnable);


	bool IsIOBoardEnabled() const;
	bool IsOEMMCEnabled() const;//value in the flash (different than value in the register).
	double GetPulserPowerMax() const;//Unit Watt
	double GetPulserPowerCurrent() const;//Unit Watt
	BYTE GetFlashUSB3Version() const;
	DWORD GetFWUSB3Version() const;
	bool EnableUSB3(bool bEnable);
	bool IsUSB3Enabled() const;
	DWORD GetMBOptions() const;
	enumOEMPAAnalogBandwidth GetAnalogBandwidth() const;

	typedef struct structOptionsCom{
		unsigned uEthernetSpeed:16;
		unsigned uReserved:16;
	}structOptionsCom;
	typedef struct structOptionsTCP{
		unsigned uOption:8;
		unsigned uMss:8;
		unsigned uWndSize:8;
		unsigned uScale:8;
	}structOptionsTCP;
	typedef struct structOptionsFlash{
		unsigned uManufacturer:8;
		unsigned uMemoryType:8;
		unsigned uMemoryCapacity:8;
		unsigned uReserved:8;
	}structOptionsFlash;
	typedef struct structVersion{
		unsigned uMinorLSB:8;
		unsigned uMinorMSB:8;
		unsigned uMajorLSB:8;
		unsigned uMajorMSB:8;
	}structVersion;

	bool GetEmbeddedVersion(structVersion &version) const;
	bool GetOptionsCom(structOptionsCom &optionsCom) const;
	bool GetOptionsTCP(structOptionsTCP &optionsTCP) const;
	bool GetOptionsFlash(structOptionsFlash &optionsFlash) const;
	int GetPasscodeCount() const;
	bool GetPasscode(int iIndex,DWORD &dwPasscode) const;
	double GetMaximumThroughput() const;//Unit is Bytes/second.

	bool UpdateAllFilter();//to update all hardware filters at the same time.

	static void EnableMultiChannel(bool bEnable);//you need to restart the sw to take the new value in account. "CUTChannels::SetDefaultMultiChannels" is the same.
	static bool IsMultiChannelEnabled();
	static void EnableLoadDefaultSetup(bool bEnable);//you need to restart the sw to take the new value in account. "CUTPreference::SetDefaultSetupAuto" is the same.
	static bool IsLoadDefaultSetupEnabled();

//#ifdef _TFM_CPP_
	bool EnableTFM(bool bEnable,structTFMParameters *pTfmParameters);
	bool IsTFMEnabled(structTFMParameters *ptfmParameters);
	//call required to call directly the API.
	bool LoadTFMLibrary();
	bool FreeTFMLibrary();
//#endif _TFM_CPP_

	//calibration management
	bool IsSubSequenceAverageSupported() const;
	bool IsTimeOffsetSupported() const;
	bool EnableAlignment(bool bEnable);
	float GetCalibrationAlignment();//maximum correction.
	float GetCalibrationOffset();//required value after calibration has been performed.
	bool IsCalibrationPerformed();//same than "IsAlignmentPerformed"
	bool IsAlignmentPerformed();
	bool IsAlignmentEnabled();
	bool ResetAlignment();
	static void AlignmentCfgUpdated();
	void SetCalibrationParameters(float fWidth,float fStart,float fRange,float fGainAnalog,double dGainDigital);
	void GetCalibrationParameters(float &fWidth,float &fStart,float &fRange,float &fGainAnalog,double &dGainDigital);
	void SetCalibrationFileReport(const wchar_t *wcFileReport);
	bool GetCalibrationFileReport(int iSizeMax,wchar_t *wcFileReport);
	double GetPulserPower(int iCycleCount, structOEMPACycleParameter *aCycleParameter) const;//Unit Watt
	//The calibration time offset is internally added to RecoveryTimes.
	bool SetTimeOffset(float fTimeOffset);
	float GetTimeOffset();//current value used to correct time axis.

	void EnableEmissionDeadElement(bool bEnable);
	bool IsEmissionDeadElementEnabled();
	void ResetEmissionDeadElements();
	bool SetEmissionDeadElements(int iElementSize,bool *pbEnable);
	bool SetEmissionDeadElement(int iElementIndex,bool bEnable);
	void EnableReceptionDeadElement(bool bEnable);
	bool IsReceptionDeadElementEnabled();
	void ResetReceptionDeadElements();
	bool SetReceptionDeadElements(int iElementSize,bool *pbEnable);
	bool SetReceptionDeadElement(int iElementIndex,bool bEnable);
	void EnableNormalisationEmission(bool bEnable);
	void EnableNormalisationReception(bool bEnable);
	bool IsNormalisationEmissionEnabled();
	bool IsNormalisationReceptionEnabled();
	bool SetNormalisationEmissionDelay(int iElementIndex,float fDelay);
	bool SetNormalisationReceptionDelay(int iElementIndex,float fDelay);
	bool SetNormalisationEmissionDelays(int iElementSize,float *pfDelay);
	bool SetNormalisationReceptionDelays(int iElementSize,float *pfDelay);

	//For OEM-MC or OEM-PA (VF).
	void EnablePulserDuringReplay(bool bEnable);
	bool IsPulserDuringReplayEnabled();
	bool EnableCscanTimeOfFlightCorrection(bool bEnable);
	bool IsCscanTimeOfFlightCorrectionEnabled();
	bool GetCscanTimeOfFlightCorrection(int iCycle,BYTE &byDecimation,float &fAscanStart);//decimation range is 1..16.
	bool SetCscanTimeOfFlightCorrection(int iCycle,BYTE byDecimation,float fAscanStart);//decimation range is 1..16.

	double GetFWAscanRecoveryTime();
	double GetFMCSubCycleRecoveryTime();
	double GetFMCCycleRecoveryTime();

	bool Enable256(bool bEnable);
	bool Is256Enabled() const;

};


#ifdef _WIN64
class DRIVEROEMPA_API_ALIGN _CAcquisitionFifo
{
private:
	ULONGLONG m_ullPrivateMemory[1];
protected:

};
class DRIVEROEMPA_API_ALIGN CAcquisitionFifo : public _CAcquisitionFifo
{
public:
	CAcquisitionFifo(enumAcquisitionFifo eFifo,CHWDeviceOEMPA *pHWDeviceOEMPA);
	~CAcquisitionFifo();

	enumAcquisitionFifo GetFifo();//return the type of the fifo.
	bool IsEnabled();//Fifo has been allocated.
	bool Alloc(int iCycleCountMax, __int64 iBufferByteSize);//to enable the fifo, should be called just after the constructor of "CHWDeviceOEMPA".
		//"iCycleCountMax": maximum cycle count in the fifo (in case of AscanFifo maximum ascan count).
		//"iBufferByteSize": maximum buffer size to store all datas (ascan or cscan or IO).
		//Example:	iCycleCountMax=8    iBufferByteSize=1024+8*4=1056 (ascan header is 8 DWORDs)
		//			you can store 8 ascan of 128 BYTES each, OR 4 ascan of 256 BYTES each.
	bool GetAlloc(int &iDataCountMax, __int64 &iBufferByteSize);//get allocation sizes.
	bool Desalloc();

	//count of data in the fifo.
	int GetCount() const;//count of data in the fifo.
	int GetLost() const;//count of lost data (new input data but the fifo was full so old data has been lost to save new data).
	__int64 GetTotalCount() const;//total data count that have been inputed of the fifo.
	__int64 GetTotalByteCount() const;//total byte count that have been inputed of the fifo.
	void ResetCounters();
	bool RemoveAll();//remove all data, can fail in the case the fifo is accessed at the same time than the priority input thread.
	bool RemoveTail();//remove the oldest data in the fifo, can fail in the case the data is accessed at the same time than the priority input thread.
	bool RemoveItem(int iItem);//remove the specified item and previous ones, -1 means the oldest data in the fifo (same than "RemoveTail"). Can fail in the case the data is accessed at the same time than the priority input thread.
	bool DumpFifo(const wchar_t *wcFileName);//debug purpose.

	//Output data from the fifo. The input parameter is the index of the data in the Fifo. This input parameter "iItem" is retrieved by others functions.
	//"bPeek": false to remove the data from the fifo or true to keep the data inside the fifo.
	//			if "bPeek=true" then the function "RemoveTail" or "RemoveItem" should be called.
	//"iItem": index of the item in the fifo, -1 means the last data.
	bool OutAscan(int iItem,bool bPeek,structAcqInfoEx* &pAcqInfo,CStream_0x0001* &pStreamHeader,CSubStreamAscan_0x0103* &pAscanHeader,void* &pBufferMax,void* &pBufferMin,void* &pBufferSat);
	bool OutCscan(int iItem,bool bPeek,structAcqInfoEx* &pAcqInfo,CStream_0x0001* &pStreamHeader,const CSubStreamCscan_0x0X02* &pCscanHeader,const structCscanAmp_0x0102* &pBufferAmp, const structCscanAmpTof_0x0202* &pBufferAmpTof);
	bool OutIO(int iItem,bool bPeek,CStream_0x0001* &pStreamHeader,const CSubStreamIO_0x0101* &pIOHeader);
	bool OutIO(int iItem,bool bPeek,structAcqInfoEx* &pAcqInfo,CStream_0x0001* &pStreamHeader,const CSubStreamIO_0x0101* &pIOHeader);

	//Functions to retrieve index of the data in the fifo for the specified cycle or the specified sequence/cycle.
	//For the following functions: "iFMCElement" and "iStartItem" should be -1 by default.
	//"iFMCElement" is used only for FMC mode, this is the pulser element, -1 means the parameter is not used.
	//"iStartItem" is the last item from which the search is done, -1 means the parameter is not used.
	//"iCycle" if -1 then the first cycle of the sequence is returned.
	//FIFO means FirstInFirstOut.
	//LIFO means LastInFirstOut.
	int GetFifoItem(LONGLONG sequence, int iCycle, int iFMCElement, int iStartItem);//return -1 if not found, otherwise the index.
	int GetFifoItem(int iCycle, int iFMCElement, int iStartItem);//(for the first sequence in the fifo) return -1 if not found, otherwise the index.
	int GetLifoItem(LONGLONG sequence, int iCycle, int iFMCElement, int iStartItem);//return -1 if not found, otherwise the index.
	int GetLifoItem(int iCycle, int iFMCElement, int iStartItem);//(for the last sequence in the fifo) return -1 if not found, otherwise the index.
	//Functions to retrieve index of any data in the fifo.
	int GetItemLimit(int &iIndexTail, int &iIndexHead);//the return value is the same than "GetCount", output parameter "iIndexTail" is the index of the oldest data in the fifo, output parameter "iIndexHead" is the index of the next input data that will be saved in the fifo.
	void IncrementItemIndex(int &iIndex);//function to increment the index from the Tail (oldest input in the fifo) to the Head (last data in the fifo).
	void DecrementItemIndex(int &iIndex);//function to decrement the index from the Head to the Tail.

	//To input new data in the fifo, this function is called by the driver.
	bool InFifo(BYTE *pSubStreamData,int iSubStreamDataSize,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader);//"iSubStreamDataSize" is the total size of the sub stream (header + data).
	BYTE *GetSubStreamItem(int iItem,int &iSubStreamDataSize,structAcqInfoEx* &pAcqInfo,CStream_0x0001* &pStreamHeader);

	//The integrated thread is launched automatically by the driver, so the user dont have to call the following functions:
	bool IsRunning();//Is the integrated thread running.
	bool Start(int iType, void* pCallback, LPTHREAD_START_ROUTINE lpStartAddress, PVOID ThreadParameter);
	bool Stop();
	void ResetProcess();
	bool SetThreadPriority(int nPriority);//input parameter is explained explanation see documentation of "SetThreadPriority".
	bool CreateEvent(int iDeviceId);
	bool CreateEvent(wchar_t *pName);
	HANDLE GetEvent();

	int _CheckCycleFMC(int iCycle,structAcqInfoEx &acqInfo);//if FMC is disabled it returns the input parameter "iCycle" otherwise it returns the absolute cycle index taking into account the pulser element (via "acqInfo.lFMCElementIndex").
	void AddFifoLost(int iLostCount);
	DWORD GetExit();
	void Exit();

};
#else _WIN64

class DRIVEROEMPA_API_ALIGN _CAcquisitionFifo
{
private:
	ULONGLONG m_ullPrivateMemory[1];
private:

};
class DRIVEROEMPA_API_ALIGN CAcquisitionFifo : public _CAcquisitionFifo
{
public:
	CAcquisitionFifo();
	~CAcquisitionFifo();
};
#endif _WIN64

typedef struct structEncoderStepFloat{
	WORD wStep;//unit=raw encoder step
	unsigned uDecimal:9;//decimal part
}structEncoderStepFloat;
typedef struct structDecimation{
	unsigned lsb:4;
	unsigned msb:4;
	//comment about the values:
	//	0 => decimation of 1 = no decimation.
	//	1 => decimation of 2 (digitizing frequency is divided by 2).
	//	...
	//	15 => decimation of 16 (digitizing frequency is divided by 16).
}structDecimation;
typedef union unionDecimation{
	structDecimation data;
	BYTE byte;
}unionDecimation;
typedef union unionOEMMCDecimation{
	unsigned char byte[g_iOEMPAApertureElementCountMax/2];
	unsigned long dword[g_iOEMPAApertureElementCountMax/(2*sizeof(unsigned long))];
	unsigned __int64 ulonglong[g_iOEMPAApertureElementCountMax/(2*sizeof(unsigned __int64))];
}unionOEMMCDecimation;

class DRIVEROEMPA_API_ALIGN _CHWDeviceOempa
{
private:
	ULONGLONG m_ullPrivateMemory[1991];
protected:

};

class CSWDeviceOEMPA;
class CUTKernelRoot;

//LOW LEVEL CLASS TO ACCESS THE HARDWARE
//it is not possible to instantiate this class directly,
//you need to instantiate "CSWDevice" first and use its 
//"CSWDevice::GetHWDevice()" function member.
//The "CSWDevice::GetHWDevice()" will return a "CHWDeviceOempa" object
//and not a "CHWDevice" in the case "CSWDevice::GetHardware()" is equal to "eOEMPA".
class DRIVEROEMPA_API_ALIGN CHWDeviceOempa :
	public CHWDevice,
	public _CHWDeviceOempa
{
public:

	CHWDeviceOempa(CHWDeviceOEMPA *pHWDeviceOEMPA,enumSharedEncoder eSharedEncoderX,enumSharedEncoder eSharedEncoderY);
	CHWDeviceOempa(CHWDeviceOEMPA *pHWDeviceOEMPA,const wchar_t *pFlashFile);//to emulate a specific HW system.
	~CHWDeviceOempa();

	CHWDeviceOempa *GetMatchedDeviceOempa();//link to the matched device (an OEMPA 64:64a could be matched with an OEMPA 64:64b to build an OEMPA 128:128).
	CHWDeviceOempa *GetMatched256DeviceOempa(int iModuleIndex);
	int _GetMatchedDevicesApertureCountMax();//to get the whole maximum element count of an aperture.
	int _GetMatchedDevicesElementCountMax();//to get the whole maximum element count of the system (in case of mux).
	int GetMatchedDevicesApertureCountMax();//to get the whole maximum element count of an aperture.
	int GetMatchedDevicesElementCountMax();//to get the whole maximum element count of the system (in case of mux).

	CHWDeviceOEMPA *GetHWDeviceOEMPA();
	CSWDeviceOEMPA *GetSWDeviceOEMPA();
	CSWEncoder *GetSWEncoder(int iEncoder);//same than "CHWDevice::GetSWDevice()->CSWDevice::GetSWEncoder(int iEncoder)", but quicker.
	virtual enumHardware CHWDeviceOempa::vf_GetHardware();

	bool GetDigitalInput(DWORD *pdwData);//reading the current state of digital inputs

	//ToDo move this function to CSWDeviceOEMPA
	bool IsFlashSWSection();//to check that the flash SW section has been reading from the hardware without error.

	//flash management
	static bool GetFlashItem(const wchar_t *strFlashFile,const wchar_t *pItemName,int iValueSize,wchar_t *pValue);
	static bool GetFlashItemBasic(const wchar_t *strFlashFile,const wchar_t *pItemName,int iValueSize,wchar_t *pValue);
	static bool SetFlashItem(const wchar_t *strFlashFile,const wchar_t *pItemName,const wchar_t *pValue);

	//Valid only for the 64:64A and the 64:64B (not for 64:64C and 64:64D).
	bool Enable256(bool bEnable);
	bool GetEnable256(bool *pbEnable);


//FW members (those parameters are inside the HW board)
//before using them you need to lock the device : see member functions of "CHWDevice"
//(the communication link with the HW is shared between all threads, only one at a time can lock it)



//<<FLASH MANAGEMENT FUNCTIONS : BEGIN>>
	//bool GetFlashTextFile(wchar_t *pFilePathName);//overwrite the text file.

//<<FLASH MANAGEMENT FUNCTIONS : END>>

//<<PARAMETERS MANAGEMENT FUNCTIONS : BEGIN>>
//All those functions return true in case of no error and false in case of error.

	bool GetFWId(WORD *pwFWId);//to get the device version.
	bool dwGetFWId(DWORD *pdwFWId);//to get the device version.
	bool GetFWOptions(DWORD *pdwFWOptions);
	bool GetOptionsCom(CSWDeviceOEMPA::structOptionsCom *pOptionsCom);
	bool GetOptionsTCP(CSWDeviceOEMPA::structOptionsTCP *pOptionsTCP);
	bool GetOptionsFlash(CSWDeviceOEMPA::structOptionsFlash *pOptionsFlash);
	bool GetEmbeddedVersion(CSWDeviceOEMPA::structVersion *pVersion);

	bool DisableUSB3(bool bDisable);
	bool GetUSB3Disabled(bool *pbDisable);


	bool GetApertureCountMax(int *piCount);//to get the maximum element count of an aperture (request to the HW).
	bool GetElementCountMax(int *piCount);//to get the maximum element count of the system (in case of mux, request to the HW).
		//Output parameters
		//	piCount : maximum aperture size.

	bool GetTemperatureSensor(int iIndexBoard,int iIndexSensor,WORD *pwTemperature);//FW parameter
							//iIndexBoard: 0=ComBoard, 1=first RX board, 2=second RX board, 3=third RX board, 4=four RX board.
	bool SetTemperatureAlarm(BYTE &byWarning,BYTE &byAlarm);
	bool GetTemperatureAlarm(BYTE *pbyWarning,BYTE *pbyAlarm);



	bool EnableFMC(bool bEnable);//return false if the FW does not support the FMC feature (old FMC FW and FW for which FMC is not supported).
	bool GetEnableFMC(bool *pbEnable);//return false if the FW does not support the FMC feature (old FMC FW and FW for which FMC is not supported).
	//you can use the following functions in any case (step equal or different to 1):
	bool SetFMCElement(int &iElementStart,int &iElementStop,int &iElementStep);
	bool GetFMCElement(int *piElementStart,int *piElementStop,int *piElementStep);
	//you can use the following functions only if the step is 1:
	bool SetFMCElementStart(int &iElementIndex);
	bool GetFMCElementStart(int *piElementIndex);
	bool SetFMCElementStop(int &iElementIndex);
	bool GetFMCElementStop(int *piElementIndex);
	bool SetReceptionGainsFMC256(int iCycle,DWORD adwHWAperture[2*g_iOEMPAApertureDWordCount],float *pfGain);


	bool EnableMultiHWChannel(bool bEnable);//return false if the FW does not support the MultiChannel feature.
	bool GetEnableMultiHWChannel(bool *pbEnable);//return false if the FW does not support the MultiChannel feature.


	bool ResetTimeStamp();

	bool ResetEncoder(int iEncoderIndex);//same than "SetEncoder(iEncoderIndex,0)".
	bool SetEncoder(int iEncoderIndex,double &dValue);//unit=meter
	bool SetEncoder(int iEncoderIndex,DWORD &dwValue);//unit=encoder step
	bool SetEncoderType(enumEncoderType &eEncoder1Type,enumEncoderType &eEncoder2Type);
	bool GetEncoderType(enumEncoderType *peEncoder1Type,enumEncoderType *peEncoder2Type);


	bool EnableAscan(bool bAscan);//to enable Ascan acquisition that will be sent by the FW to the SW.
	bool GetEnableAscan(bool *pbAscan);


	//Cscan time of flight : you have frist to define gates (see "SetGateXXX").
	bool EnableCscanTof(bool bCscanTof);//to enable Cscan time of flight acquisition that will be sent by the FW to the SW.
	bool GetEnableCscanTof(bool *pbCscanTof);


	bool SetAscanBitSize(enumBitSize eBitSize);//Ascan data size.
	bool GetAscanBitSize(enumBitSize *peBitSize);


	bool SetAscanRequest(enumAscanRequest eAscanRequest);
	bool GetAscanRequest(enumAscanRequest *peAscanRequest);


	//This function "SetAscanRequestFrequency" can be used when you dont want to receive all Ascan
	//(to decrease the throughput) see also the function "SetAscanRequest".
	//Be careful if you want to set the Pulse Repetition Rate, please refer to function "SetTimeSlot".
	bool SetAscanRequestFrequency(double &dValue);
	bool GetAscanRequestFrequency(double *pdValue);
	bool CheckAscanRequestFrequency(double &dValue);


	bool SetCycleCount(int &lCycleCount);//to write the cycle count.
	bool GetCycleCount(int *plCycleCount);//to read the cycle count.
	bool CheckCycleCount(int &iCycleCount);//to check the cycle count (minimum value, maximum value).


	bool SetTriggerMode(enumOEMPATrigger &eTrig);
	bool GetTriggerMode(enumOEMPATrigger *peTrig);
	//In case of the mode "eEncoder" an additional mode is required:
	bool SetEncoderDirection(enumOEMPAEncoderDirection &eEncoderDirection);
	bool GetEncoderDirection(enumOEMPAEncoderDirection *peEncoderDirection);


	//It is better to use following function (unit is meter), previously you have to set the resolution ("GetSWEncoder(int iEncoder)->SetResolution()").
	bool SetTriggerEncoderStep(double &dStep);//unit=meter
	bool GetTriggerEncoderStep(double *pdStep);//unit=meter
	//than those functions (if you want to manage by yourself the encoder resolution).
	bool SetTriggerEncoderStep(DWORD &dwStep);//unit=encoder step
	bool GetTriggerEncoderStep(DWORD *pdwStep);//unit=encoder step
	//for some of the FW version a floating point value is available (see function "CSWDeviceOEMPA::IsEncoderDecimal"):
	bool SetTriggerEncoderStepFloat(structEncoderStepFloat &stStep);
	bool GetTriggerEncoderStepFloat(structEncoderStepFloat *pstStep);//unit=encoder step


	bool SetSignalTriggerHighTime(double &dTime);
	bool GetSignalTriggerHighTime(double *pdTime);
	double GetSignalTriggerHighTimeMin();


	bool SetRequestIO(enumOEMPARequestIO eRequest);
	bool GetRequestIO(enumOEMPARequestIO *peRequest);


	bool SetRequestIODigitalInputMask(int &iMaskFalling,int &iMaskRising);
	bool GetRequestIODigitalInputMask(int *piMaskFalling,int *piMaskRising);
	bool CheckRequestDigitalInputMask(int &iMask);



	//bool SetEnableFilter(bool bEnable);
	//bool GetEnableFilter(bool *pbEnable);
	bool GetFilterCoefficient(enumOEMPAFilter eFilter,WORD &wScale,WORD wValue[g_iOEMPAFilterCoefficientMax]);
	bool FindFilterCoefficient(WORD wScale,const short wValue[g_iOEMPAFilterCoefficientMax],enumOEMPAFilter &eFilter);
	bool SetFilter(enumOEMPAFilterIndex eFilterIndex,WORD &wScale,short wValue[g_iOEMPAFilterCoefficientMax]);
	bool GetFilter(enumOEMPAFilterIndex eFilterIndex,WORD *pwScale,short wValue[g_iOEMPAFilterCoefficientMax]);


	bool SetEncoderWire1(int iEncoderIndex,enumDigitalInput eDigitalInput);//int iEncoderIndex : 0 for first encoder, 1 for second encoder.
	bool SetEncoderWire2(int iEncoderIndex,enumDigitalInput eDigitalInput);
	bool SetExternalTriggerCycle(enumDigitalInput eDigitalInput);
	bool SetExternalTriggerSequence(enumDigitalInput eDigitalInput);
	bool GetEncoderWire1(int iEncoderIndex,enumDigitalInput *peDigitalInput);
	bool GetEncoderWire2(int iEncoderIndex,enumDigitalInput *peDigitalInput);
	bool GetExternalTriggerCycle(enumDigitalInput *peDigitalInput);
	bool GetExternalTriggerSequence(enumDigitalInput *peDigitalInput);


	bool SetDigitalOutput(int iIndex,enumOEMPAMappingDigitalOutput eMappingDigitalOutput);
	bool GetDigitalOutput(int iIndex,enumOEMPAMappingDigitalOutput *peMappingDigitalOutput);




	bool SetEncoderDebouncer(double &dValue);
	bool GetEncoderDebouncer(double *pdValue);
	bool CheckEncoderDebouncer(double &dValue);


	bool SetDigitalDebouncer(double &dValue);
	bool GetDigitalDebouncer(double *pdValue);
	bool CheckDigitalDebouncer(double &dValue);


	bool SetMultiSystemDebouncer(double &dValue);
	bool GetMultiSystemDebouncer(double *pdValue);
	bool CheckMultiSystemDebouncer(double &dValue);


	bool SetAnalogBandwidth(enumOEMPAAnalogBandwidth &eAnalogBandwidth);
	bool GetAnalogBandwidth(enumOEMPAAnalogBandwidth *peAnalogBandwidth);


	//static bool WINAPI CheckGain(double &dGain,float &fBeamCorrection,int &iDACCountMax,int &iDACCount,float *pfDACSlope,float *pfDACTime);
	bool SetGainDigital(int iCycle,double &dGain);//to write the amplification.
	bool GetGainDigital(int iCycle,double *pdGain);//to read the amplification.
	bool CheckGainDigital(double &dGain);//to check the amplification value (minimum, maximum, step).


	//SubSequence
	bool DisableSubSequenceTable();
	bool SetSubSequenceTableEncoder(int iSubSequenceCycleCount,int *aiCycleStart,int *aiCycleStop,int iSubSequenceStepCount,DWORD *adwStep);
	bool GetSubSequenceTableEncoder(int iSubSequenceCycleMaxCount,int *aiCycleStart,int *aiCycleStop,int *piSubSequenceCycleCount,int iSubSequenceStepMaxCount,DWORD *adwStep,int *piSubSequenceStepCount);
	bool SetSubSequenceTableEncoder(int iSubSequenceCycleCount,int *aiCycleStart,int *aiCycleStop,int iSubSequenceStepCount,float *afStep);
	bool GetSubSequenceTableEncoder(int iSubSequenceCycleMaxCount,int *aiCycleStart,int *aiCycleStop,int *piSubSequenceCycleCount,int iSubSequenceStepMaxCount,float *afStep,int *piSubSequenceStepCount);
	bool SetSubSequenceTableGate(int iSubSequenceCycleCount,int *aiCycleStart,int *aiCycleStop,int iSubSequenceTofCount,float *afTof);
	bool GetSubSequenceTableGate(int iSubSequenceCycleMaxCount,int *aiCycleStart,int *aiCycleStop,int *piSubSequenceCycleCount,int iSubSequenceTofMaxCount,float *afTof,int *piSubSequenceTofCount);
	bool SetSubSequenceAverage(int &iAverage);
	bool GetSubSequenceAverage(int *piAverage);


	bool SetBeamCorrection(int iCycle,float &fGain);
	bool GetBeamCorrection(int iCycle,float *pfGain);
	bool CheckBeamCorrection(float &fGain);


	bool SetDACSlope(int iCycle,int &iCount,double *pdTime,float *pfSlope);
	bool SetDACSlope(int iCycle,int &iCount,double *pdTime,float *pfSlope,int iStringErrorSize,wchar_t *pStringError);//this function give more information about the error.
	bool GetDACSlope(int iCycle,int &iCountMax,int *piCount,double *pdTime,float *pfSlope);
	bool SetDACSlope(int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac);
	bool SetDACSlope(int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac,int iStringErrorSize,wchar_t *pStringError);//this function give more information about the error.
	bool GetDACSlope(int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac);

	bool SetDACGain(bool bAutoStop,int iCycle,int &iCount,double *pdTime,float *pfGain);//bAutoStop : true to automatically add a point to return the gain to 0dB.
	bool GetDACGain(int iCycle,int &iCountMax,int *piCount,double *pdTime,float *pfGain);
	bool SetDACGain(bool bAutoStop,int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac);//bAutoStop : true to automatically add a point to return the gain to 0dB.
	bool GetDACGain(int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac);

	bool EnableDAC(int iCycle,bool bEnable);
	bool GetEnableDAC(int iCycle,bool *pbEnable);
	bool CheckDACSlope(double &fTime,float &fSlope);
	bool CheckDACCount(int &iCount);


	bool SetAscanRectification(int iCycle,enumRectification eRectification);
	bool GetAscanRectification(int iCycle,enumRectification *peRectification);


	bool SetAscanStart(int iCycle,double &dStart);
	bool GetAscanStart(int iCycle,double *pdStart);
	bool CheckAscanStart(double &dStart);


	bool SetAscanRange(int iCycle,double &dRange,/*output only*/enumCompressionType &eCompressionType,/*output only*/long &lPointCount,/*output only*/long &lPointFactor);//point count is automatically calculated from the digitizing clock.
			//Input is the range, this function determines the point count, the digitizing frequency is the default HW clock (normally 100 MHz).
	bool SetAscanRangeWithFactor(int iCycle,double &dRange,enumCompressionType &eCompressionType,/*in/out*/long &lPointFactor,/*output only*/long &lPointCount);
			//Inputs are range + factor, this function determines the point count.
			//two compressions are available:
			//	"eCompression": compression with minimum and maximum.
			//	"eDecimation": compression fixed digitizing frequency (no minimum and maximum).
			//DigitizingFrequency = clock x lPointFactor / 256
			//normally clock=100 MHz
			//"lPointFactor" range is 1 to 256.
	bool SetAscanRangeWithCount(int iCycle,double &dRange,/*in/out (check)*/enumCompressionType &eCompressionType,/*in/out (check)*/long &lPointCount,/*output only*/long &lPointFactor);
	bool GetAscanRange(int iCycle,double *pdRange,enumCompressionType *peCompressionType,long *plPointCount,long *plPointFactor);
	bool CheckAscanRange(double &dRange);
	bool CheckAscanRangeWithCount(double &dRange,enumCompressionType &eCompressionType,long &lPointCount);
	bool GetSamplingFrequency(enumCompressionType eCompressionType,long lPointFactor,double &dSamplingFrequency/*Hertz*/);


	bool SetFilterIndex(int iCycle,enumOEMPAFilterIndex eFilterIndex);
	bool GetFilterIndex(int iCycle,enumOEMPAFilterIndex *peFilterIndex);


	bool SetTimeSlot(int iCycle,double &dTimeSlot);
	bool GetTimeSlot(int iCycle,double *pdTimeSlot);
	bool CheckTimeSlot(double &dTimeSlot);
	double GetTimeSlotMinimum(long lAscanPointCount, enumBitSize eAscanBitSize);//Minimum timeSlot for maximum useful data throughput (not LAN throughput, data header is not taken into account).
	double GetAscanThroughput(double dTimeSlot, long lAscanPointCount, enumBitSize eAscanBitSize);//useful data throughput not LAN throughput (data header is not taken into account).
	//FMC SubTimeSlot management
	bool SetFMCSubTimeSlotAcqReplay(int iCycle,double dAscanStart,double dAscanRange,double &dTimeSlot);//easy function to use
	bool GetFMCTimeLimitation(double dAscanStart,double dAscanRange,double dTimeSlot,double &dTimeSlotMin,double &dHWAcqSubTimeSlotMin,double &dReplaySubTimeSlotMin,double &dReplaySubTimeSlotOptimizedForThroughput);
	bool GetFMCTimeLimitation(int iFMCSubCycleCount,double dAscanStart,double dAscanRange,double dTimeSlot,double &dTimeSlotMin,double &dHWAcqSubTimeSlotMin,double &dReplaySubTimeSlotMin,double &dReplaySubTimeSlotOptimizedForThroughput);
	int GetFMCSubTimeSlotCount();
	//instead to call "SetFMCSubTimeSlotAcq" and "SetFMCSubTimeSlotReplay" you can call "SetFMCSubTimeSlotAcqReplay" which is easier to use.
	bool SetFMCSubTimeSlotAcq(int iCycle,double &dSubTimeSlot);
	bool SetFMCSubTimeSlotReplay(int iCycle,double &dSubTimeSlot);
	bool GetFMCSubTimeSlotAcq(int iCycle,double *pdSubTimeSlot);
	bool GetFMCSubTimeSlotReplay(int iCycle,double *pdSubTimeSlot);


	bool SetAscanAcqIdChannelProbe(int iCycle,WORD &wID);
	bool GetAscanAcqIdChannelProbe(int iCycle,WORD *pwID);


	bool SetAscanAcqIdChannelScan(int iCycle,WORD &wID);
	bool GetAscanAcqIdChannelScan(int iCycle,WORD *pwID);


	bool SetAscanAcqIdChannelCycle(int iCycle,WORD &wID);
	bool GetAscanAcqIdChannelCycle(int iCycle,WORD *pwID);


	bool EnableAscanMaximum(int iCycle,bool bEnable);
	bool GetEnableAscanMaximum(int iCycle,bool *pbEnable);


	bool EnableAscanMinimum(int iCycle,bool bEnable);
	bool GetEnableAscanMinimum(int iCycle,bool *pbEnable);


	bool EnableAscanSaturation(int iCycle,bool bEnable);
	bool GetEnableAscanSaturation(int iCycle,bool *pbEnable);


	bool SetGateModeThreshold(int iCycle,int iGate,bool bEnable,double dThresholdPercent,enumGateModeAmp eGateModeAmp,enumGateModeTof eGateModeTof,enumRectification eGateRectification);
	bool SetGateModeThreshold(int iCycle,int iGate,bool &bEnable,enumGateModeAmp &eGateModeAmp,enumGateModeTof &eGateModeTof,enumRectification &eGateRectification,double &dThresholdPercent);
	bool GetGateModeThreshold(int iCycle,int iGate,bool *pbEnable,enumGateModeAmp *peGateModeAmp,enumGateModeTof *peGateModeTof,enumRectification *peGateRectification,double *pdThresholdPercent);
	bool CheckGateModeThreshold(bool &bEnable,enumGateModeAmp &eGateModeAmp,enumGateModeTof &eGateModeTof,enumRectification &eGateRectification,double &dThresholdPercent);


	bool SetGateStart(int iCycle,int iGate,double &dStart);
	bool GetGateStart(int iCycle,int iGate,double *pdStart);
	bool CheckGateStart(double &dStart);


	bool CheckGateStartStop(double &dStart,double &dStop);
	bool SetGateStop(int iCycle,int iGate,double &dStop);
	bool GetGateStop(int iCycle,int iGate,double *pdStop);
	bool CheckGateStop(double &dStop);


	bool SetGateAcqIDAmp(int iCycle,int iGate,WORD &wID);
	bool GetGateAcqIDAmp(int iCycle,int iGate,WORD *pwID);
	bool SetGateAcqIDTof(int iCycle,int iGate,WORD &wID);
	bool GetGateAcqIDTof(int iCycle,int iGate,WORD *pwID);


	bool SetTrackingGateStart(int iCycle,int iGate,bool bEnable,int &iTrackingCycleIndex,int &iTrackingGateIndex);
	bool GetTrackingGateStart(int iCycle,int iGate,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
	bool SetTrackingGateStop(int iCycle,int iGate,bool bEnable,int &iTrackingCycleIndex,int &iTrackingGateIndex);
	bool GetTrackingGateStop(int iCycle,int iGate,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
	bool SetTrackingAscan(int iCycle,bool bEnable,int &iTrackingCycleIndex,int &iTrackingGateIndex);
	bool GetTrackingAscan(int iCycle,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
	bool SetTrackingDac(int iCycle,bool bEnable,int &iTrackingCycleIndex,int &iTrackingGateIndex);
	bool GetTrackingDac(int iCycle,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
	bool CheckTracking(bool bEnable,int &iTrackingCycleIndex,int &iTrackingGateIndex);
	bool ResetTrackingTable();//this is called automatically when the IF tracking of one gate is updated.




	bool SetGainAnalog(int iCycle,float &fGain);
	bool GetGainAnalog(int iCycle,float *pfGain);
	bool CheckGainAnalog(float &fGain);




	//For some of the following functions the CycleCount is required
	//(it should be the total cycle count of the sequence).
	bool SetEmissionWedgeDelay(int iCycle,int iCycleCount,double &dWedgeDelay);//"iCycleCount"=cycle count.
	bool GetEmissionWedgeDelay(int iCycle,int iCountMax,double *pdWedgeDelay);
	bool SetReceptionWedgeDelay(int iCycle,int iCycleCount,double &dWedgeDelay);
	bool GetReceptionWedgeDelay(int iCycle,int iCountMax,double *pdWedgeDelay);
	bool CheckWedgeDelay(double &dWedgeDelay);


	static bool SetAllElementEnable(bool bEnable,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	static bool SetElementEnable(int iElement,bool bEnable,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	static bool GetElementEnable(int iElement,const DWORD adwHWAperture[g_iOEMPAApertureDWordCount],bool &bEnable);

	//APERTURE DEFINITION : 2 cases.
	bool IsMultiplexer();//this function can be used to know if a multiplexer (16:128 or 32:128) is included in the system.
	//Case of a system with a multiplexer (16:128 or 32:128), please use following functions:
	//	multiplexer case: if emission and reception have same aperture:
	bool SetMultiplexerEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	bool GetMultiplexerEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	//	multiplexer case: if emission and reception aperture are different:
	//	(you cannot have more than 32 elements in case of a 32/128 and 16 elements in case of a 16/128).
	bool SetMultiplexerEnable(int iCycle,DWORD adwHWApertureEmission[g_iOEMPAApertureDWordCount],DWORD adwHWApertureReception[g_iOEMPAApertureDWordCount]);
	bool GetMultiplexerEnable(int iCycle,DWORD adwHWApertureEmission[g_iOEMPAApertureDWordCount],DWORD adwHWApertureReception[g_iOEMPAApertureDWordCount]);
	bool CheckMultiplexerAperture(DWORD adwHWApertureEmission[g_iOEMPAApertureDWordCount],DWORD adwHWApertureReception[g_iOEMPAApertureDWordCount]);//to check that emission and reception apertures are coherent.
	//Case of a system (16:16, 32:32, 64:64 etc...) without a multiplexer, then please use the following functions:
	bool SetEmissionEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	bool GetEmissionEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	bool SetReceptionEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	bool GetReceptionEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	bool SetReceptionMatchedDevicesEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);//usefull for matched devices only.
	bool SetReceptionEnable256(bool bMaster,int iCycle,DWORD adwHWAperture[2*g_iOEMPAApertureDWordCount]);


	bool SetEmissionDelays(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfDelay);
	bool GetEmissionDelays(int iCycle,int &iElementCountMax,int *piElementCount,float *pfDelay);
	bool SetEmissionDelays(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat1D &callbackArrayFloat1D);//Size = element size.
	bool GetEmissionDelays(int iCycle,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D);//Size = element size.
	bool SetEmissionWidth(int iCycle,int iElementIndex,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float &fWidth);
	bool SetEmissionWidths(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfWidth);
	bool GetEmissionWidths(int iCycle,int &iElementCountMax,int *piElementCount,float *pfWidth);
	bool SetEmissionWidths(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat1D &callbackArrayFloat1D);
	bool GetEmissionWidths(int iCycle,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D);
	bool SetReceptionGain(int iCycle,int iElementIndex,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float &fGain);
	bool SetReceptionGains(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfGain);
	bool GetReceptionGains(int iCycle,int &iElementCountMax,int *piElementCount,float *pfGain);
	bool SetReceptionGains(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat1D &callbackArrayFloat1D);
	bool GetReceptionGains(int iCycle,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D);
	bool SetReceptionDelays(int iCycle,int iLineSize,int &iFocalCount,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfDelay);
	bool GetReceptionDelays(int iCycle,int iLineSize,int &iElementCountMax,int &iFocalCountMax,int *piElementCount,int *piFocalCount,float *pfDelay);
	bool SetReceptionDelays(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat2D &callbackArrayFloat2D);//Size1 = element size, Size2 = DDF size.
	bool GetReceptionDelays(int iCycle,int &iElementCountMax,int &iFocalCountMax,structCallbackArrayFloat2D &callbackArrayFloat2D);//Size1 = element size, Size2 = DDF size.
	bool SetReceptionFocusing(int iCycle,int &iFocalCount,double *pdFocalTof);
	bool GetReceptionFocusing(int iCycle,int &iFocalCountMax,int *piFocalCount,double *pdFocalTof);
	bool SetReceptionFocusing(int iCycle,structCallbackArrayDouble1D &callbackArrayDouble1D);
	bool GetReceptionFocusing(int iCycle,int &iFocalCountMax,structCallbackArrayDouble1D &callbackArrayDouble1D);
	bool SetReceptionFocusing(int iCycle,int &iFocalCount,double *pdFocalTof,float &fCenterDelayE,float &fCenterDelayR);
	bool GetReceptionFocusing(int iCycle,int &iFocalCountMax,int *piFocalCount,double *pdFocalTof,float *pfCenterDelayE,float *pfCenterDelayR);
	bool SetReceptionFocusing(int iCycle,structCallbackArrayDouble1D &callbackArrayDouble1D,float &fCenterDelayE,float &fCenterDelayR);
	bool GetReceptionFocusing(int iCycle,int &iFocalCountMax,structCallbackArrayDouble1D &callbackArrayDouble1D,float *pfCenterDelayE,float *pfCenterDelayR);
	bool GetReceptionNormalisationDelayAv(bool bEmission,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float &fDelay);
	bool EnableDDF(int iCycle,bool bEnable);
	bool GetEnableDDF(int iCycle,bool *pbEnable);
	bool EnableDDF(int iCycle,enumFocusing &eFocusing);
	bool GetEnableDDF(int iCycle,enumFocusing *peFocusing);
	static void SetDDFTimeOfFlightMiddle(bool bEnable);
	static bool IsDDFTimeOfFlightMiddle();
	static void SetDDFWaveTrackingCorrection(int iEnable);
	static int GetDDFWaveTrackingCorrection();
	static void SetFMCReceptionSimplified(bool bEnable);
	static bool IsFMCReceptionSimplified();
	bool CheckFocalTimeOfFlight(double &dDelay);
	bool CheckEmissionWidth(float &fWidth);
	bool CheckReceptionGain(float &fGain);
	bool CheckEmissionDelay(float &fDelay);
	bool CheckReceptionDelay(float &fDelay);
	DWORD GetSWBaseAddress();


	bool EnableMultiHWChannelAcquisition(int iCycle,int iCycleCount,bool bEnable);
	bool GetEnableMultiHWChannelAcquisition(int iCycle,bool *pbEnable);
	bool SetMultiHWChannelAcqDecimation(int iCycle,unionOEMMCDecimation &hwAcqDecimation);
	bool GetMultiHWChannelAcqDecimation(int iCycle,unionOEMMCDecimation *hwAcqDecimation);
	bool GetMultiHWChannelAcqDecimation(int iCycle,structCallbackArrayByte1D &callbackArrayByte1D);
	//A-scan Start is specified for consecutive elements from the first single element #1 (index base 1, no gap between elements).
	bool SetMultiHWChannelAcqWriteStart(int iCycle,int iAcqElement,int iStartCount,float *pfStart);
			//input "iAcqElement": element for which ascan is sent to the computer.
			//input "iStartCount": element count of the array "pfStart".
			//input "pfStart": array of ascan start from the first element probe (element #1, 1 base index).
	bool GetMultiHWChannelAcqWriteStart(int iCycle,int &iStartCountMax,int *piStartCount,float *pfStart);
	bool SetMultiHWChannelAcqWriteStart(int iCycle,int iAcqElement,int iStartCount,structCallbackArrayFloat1D &callbackArrayFloat1D);//Size1 = element size.
	bool GetMultiHWChannelAcqWriteStart(int iCycle,int &iStartCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D);//Size1 = element size.
	double GetMultiHWChannelRangeMax();//unit us.


	bool SetSettingId(DWORD dwSettingId);
	bool GetSettingId(DWORD *pdwSettingId);//ask hw as for others functions.
	DWORD swGetSettingId();//ask sw (boolean memory image resident in computer) instead of hw.


	//in case of Ethernet, KeepAlive is useful to prevent communication deadlock
	//but if the developper stay a long time inside a breakpoint it will break the socket.
	bool SetKeepAlive(enumKeepAlive eKeepAlive);
	bool GetKeepAlive(enumKeepAlive *peKeepAlive);//ask hw as for others functions.


	//calibration management
	bool PerformCalibration(float &fDelayCorrectionMax,float &fTimeOffset);
	bool SetTimeOffset(float &fTime);
	bool GetTimeOffset(float *pfTime);


	bool TestFWMemoryCycle(int iCycleCount,DWORD dwData);
	bool TestFWMemoryFilter(int iFilterCount,DWORD dwData);
//<<PARAMETERS MANAGEMENT FUNCTIONS : END>>


public://don't use following functions
	virtual bool CHWDeviceOempa::vf_GetStringId(wchar_t *stringId,int iSize);
	virtual LRESULT CHWDeviceOempa::vf_CallbackKernelDriver(const wchar_t *param_pPathDefaultLanguage,CHWDevice *pHWDevice,WORD wCallbackEncoder,WORD wCallbackParam);
	virtual int CHWDeviceOempa::vf_GetReservedData();
	virtual bool CHWDeviceOempa::vf_SetCommunication(enumCommunication eCommunication);
	virtual bool CHWDeviceOempa::vf_LoadCfgFile();
	virtual bool CHWDeviceOempa::vf_ReLoadCfgFile();
	virtual bool CHWDeviceOempa::vf_SetStandby(bool bLock,enumAcquisitionState &eAcqState);
	virtual bool CHWDeviceOempa::vf_ReadStatusRegister();


public:

private:

};

class DRIVEROEMPA_API_ALIGN _CHWDeviceOEMPA
{
private:
	ULONGLONG m_ullPrivateMemory[33];
protected:

};

class DRIVEROEMPA_API_ALIGN CHWDeviceOEMPA :
	public CHWDeviceOempa,
	public _CHWDeviceOEMPA
{
private:
	//TO DO LATER: integrated thread to dispatch ascan to the callback
	//static DWORD WINAPI ThreadFifoAscan(LPVOID pParam);

public:
	CHWDeviceOEMPA();
	CHWDeviceOEMPA(const wchar_t *pFlashFile);//to emulate a specific HW system.
	~CHWDeviceOEMPA();

	CSWDeviceOEMPA *GetSWDeviceOEMPA();
	CSWEncoder *GetSWEncoder(int iEncoder);//same than "CHWDevice::GetSWDevice()->CSWDevice::GetSWEncoder(int iEncoder)", but quicker.
	virtual enumHardware CHWDeviceOEMPA::vf_GetHardware();
	CHWDeviceOEMPA* GetHWMatched256DeviceOEMPA();

	bool GetDigitalInput(DWORD *pdwData);//reading the current state of digital inputs

	//ToDo move this function to CSWDeviceOEMPA
	bool IsFlashSWSection();//to check that the flash SW section has been reading from the hardware without error.

	//flash management
	static bool GetFlashItem(const wchar_t *strFlashFile,const wchar_t *pItemName,int iValueSize,wchar_t *pValue);
	static bool SetFlashItem(const wchar_t *strFlashFile,const wchar_t *pItemName,const wchar_t *pValue);

	//MultiProcess management begin
	static bool IsMultiProcessRegistered();
	static bool RegisterMultiProcess(const wchar_t *wcProcessName);//call this function before instantiating any device.
	static bool UnregisterMultiProcess();
	static bool IsMultiProcessConnected(wchar_t *wcIPAddress,DWORD &dwProcessId);//call this function to know if another process has been already connected with the device.
				//Input "wcIPAddress": IP address of the device.
				//Output "dwProcessId": process ID of the process that has already been connected with the device.
				//return true if any other process has already been connected with the device, false otherwise.
	static bool DisconnectMultiProcess(wchar_t *wcIPAddress,DWORD dwProcessId);//call this function to disconnect the device of another process (.
				//Input "wcIPAddress": IP address of the device.
				//Input "dwProcessId": process ID of the process that has already been connected with the device.
				//return true if the device has been disconnected, false otherwise.
	static int GetMultiProcessCount();//to get the count of other process that has been registered with function "RegisterMultiProcess".
	static bool GetMultiProcessInfo(int iIndex,DWORD &dwProcessId,int iSize,wchar_t *wcProcessName);//to get information of other process that has been registered with function "RegisterMultiProcess".
	//MultiProcess management end

	//Valid only for the 64:64A and the 64:64B (not for 64:64C and 64:64D).
	bool Enable256(bool bEnable);
	bool GetEnable256(bool *pbEnable);

//FW members (those parameters are inside the HW board)
//before using them you need to lock the device : see member functions of "CHWDevice"
//(the communication link with the HW is shared between all threads, only one at a time can lock it)

//<<FLASH MANAGEMENT FUNCTIONS : BEGIN>>
	//bool GetFlashTextFile(wchar_t *pFilePathName);//overwrite the text file.
//<<FLASH MANAGEMENT FUNCTIONS : END>>

//<<PARAMETERS MANAGEMENT FUNCTIONS : BEGIN>>
//All those functions return true in case of no error and false in case of error.

	bool GetFWId(WORD *pwFWId);//to get the device version.
	bool dwGetFWId(DWORD *pdwFWId);//to get the device version.
		//Output parameters
		//	pwFWId : FW version.
	bool GetFWOptions(WORD *pwFWOptions);
	bool DisableUSB3(bool bDisable);
	bool GetUSB3Disabled(bool *pbDisable);


	bool GetApertureCountMax(int *piCount);//to get the maximum element count of an aperture (request to the HW).
	bool GetElementCountMax(int *piCount);//to get the maximum element count of the system (in case of mux, request to the HW).
		//Output parameters
		//	piCount : maximum aperture size.

	bool GetTemperatureSensor(int iIndexBoard,int iIndexSensor,WORD *pwTemperature);//FW parameter
							//iIndexBoard: 0=ComBoard, 1=first RX board, 2=second RX board, 3=third RX board, 4=four RX board.
	bool SetTemperatureAlarm(BYTE &byWarning,BYTE &byAlarm);
	bool GetTemperatureAlarm(BYTE *pbyWarning,BYTE *pbyAlarm);

	bool EnableFMC(bool bEnable);//return false if the FW does not support the FMC feature (old FMC FW and FW for which FMC is not supported).
	bool GetEnableFMC(bool *pbEnable);//return false if the FW does not support the FMC feature (old FMC FW and FW for which FMC is not supported).
	//you can use the following functions in case "CSWDeviceOEMPA::Is" return true:
	bool SetFMCElement(int &iElementStart,int &iElementStop,int &iElementStep);
	bool GetFMCElement(int *piElementStart,int *piElementStop,int *piElementStep);
	//in case it return false you have to use the following functions (step is 1):
	bool SetFMCElementStart(int &iElementIndex);
	bool GetFMCElementStart(int *piElementIndex);
	bool SetFMCElementStop(int &iElementIndex);
	bool GetFMCElementStop(int *piElementIndex);
	bool SetReceptionGainsFMC256(int iCycle,DWORD adwHWAperture[2*g_iOEMPAApertureDWordCount],float *pfGain);

	bool EnableMultiHWChannel(bool bEnable);//return false if the FW does not support the MultiChannel feature.
	bool GetEnableMultiHWChannel(bool *pbEnable);//return false if the FW does not support the MultiChannel feature.

	bool ResetTimeStamp();

	bool ResetEncoder(int iEncoderIndex);//same than "SetEncoder(iEncoderIndex,0)".
	bool SetEncoder(int iEncoderIndex,double &dValue);//unit=meter
	bool SetEncoder(int iEncoderIndex,DWORD &dwValue);//unit=encoder step
	bool SetEncoderType(enumEncoderType &eEncoder1Type,enumEncoderType &eEncoder2Type);
	bool GetEncoderType(enumEncoderType *peEncoder1Type,enumEncoderType *peEncoder2Type);

	bool EnableAscan(bool bAscan);//to enable Ascan acquisition that will be sent by the FW to the SW.
	bool GetEnableAscan(bool *pbAscan);

	//Cscan time of flight : you have frist to define gates (see "SetGateXXX").
	bool EnableCscanTof(bool bCscanTof);//to enable Cscan time of flight acquisition that will be sent by the FW to the SW.
	bool GetEnableCscanTof(bool *pbCscanTof);

	bool SetAscanBitSize(enumBitSize eBitSize);//Ascan data size.
	bool GetAscanBitSize(enumBitSize *peBitSize);

	bool SetAscanRequest(enumAscanRequest eAscanRequest);
	bool GetAscanRequest(enumAscanRequest *peAscanRequest);

	//This function "SetAscanRequestFrequency" can be used when you dont want to receive all Ascan
	//(to decrease the throughput) see also the function "SetAscanRequest".
	//Be careful if you want to set the Pulse Repetition Rate, please refer to function "SetTimeSlot".
	bool SetAscanRequestFrequency(double &dValue);
	bool GetAscanRequestFrequency(double *pdValue);
	bool CheckAscanRequestFrequency(double &dValue);

	bool SetCycleCount(int &lCycleCount);//to write the cycle count.
	bool GetCycleCount(int *plCycleCount);//to read the cycle count.
	bool CheckCycleCount(int &iCycleCount);//to check the cycle count (minimum value, maximum value).

	bool SetTriggerMode(enumOEMPATrigger &eTrig);
	bool GetTriggerMode(enumOEMPATrigger *peTrig);

	//It is better to use following function (unit is meter), previously you have to set the resolution ("GetSWEncoder(int iEncoder)->SetResolution()").
	bool SetTriggerEncoderStep(double &dStep);//unit=meter
	bool GetTriggerEncoderStep(double *pdStep);//unit=meter
	//than those functions (if you want to manage by yourself the encoder resolution).
	bool SetTriggerEncoderStep(DWORD &dwStep);//unit=encoder step
	bool GetTriggerEncoderStep(DWORD *pdwStep);//unit=encoder step
	//for some of the FW version a floating point value is available (see function "CSWDeviceOEMPA::IsEncoderDecimal"):
	bool SetTriggerEncoderStepFloat(structEncoderStepFloat &stStep);
	bool GetTriggerEncoderStepFloat(structEncoderStepFloat *pstStep);//unit=encoder step

	bool SetSignalTriggerHighTime(double &dTime);
	bool GetSignalTriggerHighTime(double *pdTime);
	double GetSignalTriggerHighTimeMin();

	bool SetRequestIO(enumOEMPARequestIO eRequest);
	bool GetRequestIO(enumOEMPARequestIO *peRequest);

	bool SetRequestIODigitalInputMask(int &iMaskFalling,int &iMaskRising);
	bool GetRequestIODigitalInputMask(int *piMaskFalling,int *piMaskRising);
	bool CheckRequestDigitalInputMask(int &iMask);

	//bool SetEnableFilter(bool bEnable);
	//bool GetEnableFilter(bool *pbEnable);
	bool GetFilterCoefficient(enumOEMPAFilter eFilter,WORD &wScale,WORD wValue[g_iOEMPAFilterCoefficientMax]);
	bool FindFilterCoefficient(WORD wScale,const short wValue[g_iOEMPAFilterCoefficientMax],enumOEMPAFilter &eFilter);
	bool SetFilter(enumOEMPAFilterIndex eFilterIndex,WORD &wScale,short wValue[g_iOEMPAFilterCoefficientMax]);
	bool GetFilter(enumOEMPAFilterIndex eFilterIndex,WORD *pwScale,short wValue[g_iOEMPAFilterCoefficientMax]);

	bool SetEncoderWire1(int iEncoderIndex,enumDigitalInput eDigitalInput);//int iEncoderIndex : 0 for first encoder, 1 for second encoder.
	bool SetEncoderWire2(int iEncoderIndex,enumDigitalInput eDigitalInput);
	bool SetExternalTriggerCycle(enumDigitalInput eDigitalInput);
	bool SetExternalTriggerSequence(enumDigitalInput eDigitalInput);
	bool GetEncoderWire1(int iEncoderIndex,enumDigitalInput *peDigitalInput);
	bool GetEncoderWire2(int iEncoderIndex,enumDigitalInput *peDigitalInput);
	bool GetExternalTriggerCycle(enumDigitalInput *peDigitalInput);
	bool GetExternalTriggerSequence(enumDigitalInput *peDigitalInput);

	bool SetDigitalOutput(int iIndex,enumOEMPAMappingDigitalOutput eMappingDigitalOutput);
	bool GetDigitalOutput(int iIndex,enumOEMPAMappingDigitalOutput *peMappingDigitalOutput);

	bool SetEncoderDebouncer(double &dValue);
	bool GetEncoderDebouncer(double *pdValue);
	bool CheckEncoderDebouncer(double &dValue);

	bool SetDigitalDebouncer(double &dValue);
	bool GetDigitalDebouncer(double *pdValue);
	bool CheckDigitalDebouncer(double &dValue);

	bool SetMultiSystemDebouncer(double &dValue);
	bool GetMultiSystemDebouncer(double *pdValue);
	bool CheckMultiSystemDebouncer(double &dValue);

	//static bool WINAPI CheckGain(double &dGain,float &fBeamCorrection,int &iDACCountMax,int &iDACCount,float *pfDACSlope,float *pfDACTime);
	bool SetGainDigital(int iCycle,double &dGain);//to write the amplification.
	bool GetGainDigital(int iCycle,double *pdGain);//to read the amplification.
	bool CheckGainDigital(double &dGain);//to check the amplification value (minimum, maximum, step).

	//SubSequence
	bool DisableSubSequenceTable();
	bool SetSubSequenceTableEncoder(int iSubSequenceCycleCount,int *aiCycleStart,int *aiCycleStop,int iSubSequenceStepCount,DWORD *adwStep);
	bool GetSubSequenceTableEncoder(int iSubSequenceCycleMaxCount,int *aiCycleStart,int *aiCycleStop,int *piSubSequenceCycleCount,int iSubSequenceStepMaxCount,DWORD *adwStep,int *piSubSequenceStepCount);
	bool SetSubSequenceTableEncoder(int iSubSequenceCycleCount,int *aiCycleStart,int *aiCycleStop,int iSubSequenceStepCount,float *afStep);
	bool GetSubSequenceTableEncoder(int iSubSequenceCycleMaxCount,int *aiCycleStart,int *aiCycleStop,int *piSubSequenceCycleCount,int iSubSequenceStepMaxCount,float *afStep,int *piSubSequenceStepCount);
	bool SetSubSequenceTableGate(int iSubSequenceCycleCount,int *aiCycleStart,int *aiCycleStop,int iSubSequenceTofCount,float *afTof);
	bool GetSubSequenceTableGate(int iSubSequenceCycleMaxCount,int *aiCycleStart,int *aiCycleStop,int *piSubSequenceCycleCount,int iSubSequenceTofMaxCount,float *afTof,int *piSubSequenceTofCount);
	bool SetSubSequenceAverage(int &iAverage);
	bool GetSubSequenceAverage(int *piAverage);

	bool SetBeamCorrection(int iCycle,float &fGain);
	bool GetBeamCorrection(int iCycle,float *pfGain);
	bool CheckBeamCorrection(float &fGain);

	bool SetDACSlope(int iCycle,int &iCount,double *pdTime,float *pfSlope);
	bool SetDACSlope(int iCycle,int &iCount,double *pdTime,float *pfSlope,int iStringErrorSize,wchar_t *pStringError);//this function give more information about the error.
	bool GetDACSlope(int iCycle,int &iCountMax,int *piCount,double *pdTime,float *pfSlope);
	bool SetDACSlope(int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac);
	bool SetDACSlope(int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac,int iStringErrorSize,wchar_t *pStringError);//this function give more information about the error.
	bool GetDACSlope(int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac);

	bool SetDACGain(bool bAutoStop,int iCycle,int &iCount,double *pdTime,float *pfGain);//bAutoStop : true to automatically add a point to return the gain to 0dB.
	bool GetDACGain(int iCycle,int &iCountMax,int *piCount,double *pdTime,float *pfGain);
	bool SetDACGain(bool bAutoStop,int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac);//bAutoStop : true to automatically add a point to return the gain to 0dB.
	bool GetDACGain(int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac);

	bool EnableDAC(int iCycle,bool bEnable);
	bool GetEnableDAC(int iCycle,bool *pbEnable);
	bool CheckDACSlope(double &fTime,float &fSlope);
	bool CheckDACCount(int &iCount);

	bool SetAscanRectification(int iCycle,enumRectification eRectification);
	bool GetAscanRectification(int iCycle,enumRectification *peRectification);

	bool SetAscanStart(int iCycle,double &dStart);
	bool GetAscanStart(int iCycle,double *pdStart);
	bool CheckAscanStart(double &dStart);

	bool SetAscanRange(int iCycle,double &dRange,/*output only*/enumCompressionType &eCompressionType,/*output only*/long &lPointCount,/*output only*/long &lPointFactor);//point count is automatically calculated from the digitizing clock.
			//Input is the range, this function determines the point count, the digitizing frequency is the default HW clock (normally 100 MHz).
	bool SetAscanRangeWithFactor(int iCycle,double &dRange,enumCompressionType &eCompressionType,/*in/out*/long &lPointFactor,/*output only*/long &lPointCount);
			//Inputs are range + factor, this function determines the point count.
			//two compressions are available:
			//	"eCompression": compression with minimum and maximum.
			//	"eDecimation": compression fixed digitizing frequency (no minimum and maximum).
			//DigitizingFrequency = clock x lPointFactor / 256
			//normally clock=100 MHz
			//"lPointFactor" range is 1 to 256.
	bool SetAscanRangeWithCount(int iCycle,double &dRange,/*in/out (check)*/enumCompressionType &eCompressionType,/*in/out (check)*/long &lPointCount,/*output only*/long &lPointFactor);
	bool GetAscanRange(int iCycle,double *pdRange,enumCompressionType *peCompressionType,long *plPointCount,long *plPointFactor);
	bool CheckAscanRange(double &dRange);
	bool CheckAscanRangeWithCount(double &dRange,enumCompressionType &eCompressionType,long &lPointCount);
	bool GetSamplingFrequency(enumCompressionType eCompressionType,long lPointFactor,double &dSamplingFrequency/*Hertz*/);

	bool SetFilterIndex(int iCycle,enumOEMPAFilterIndex eFilterIndex);
	bool GetFilterIndex(int iCycle,enumOEMPAFilterIndex *peFilterIndex);

	bool SetTimeSlot(int iCycle,double &dTime);
	bool GetTimeSlot(int iCycle,double *pdTime);
	bool CheckTimeSlot(double &dTime);

	bool SetAscanAcqIdChannelProbe(int iCycle,WORD &wID);
	bool GetAscanAcqIdChannelProbe(int iCycle,WORD *pwID);

	bool SetAscanAcqIdChannelScan(int iCycle,WORD &wID);
	bool GetAscanAcqIdChannelScan(int iCycle,WORD *pwID);

	bool SetAscanAcqIdChannelCycle(int iCycle,WORD &wID);
	bool GetAscanAcqIdChannelCycle(int iCycle,WORD *pwID);

	bool EnableAscanMaximum(int iCycle,bool bEnable);
	bool GetEnableAscanMaximum(int iCycle,bool *pbEnable);

	bool EnableAscanMinimum(int iCycle,bool bEnable);
	bool GetEnableAscanMinimum(int iCycle,bool *pbEnable);

	bool EnableAscanSaturation(int iCycle,bool bEnable);
	bool GetEnableAscanSaturation(int iCycle,bool *pbEnable);

	bool SetGateModeThreshold(int iCycle,int iGate,bool bEnable,double dThresholdPercent,enumGateModeAmp eGateModeAmp,enumGateModeTof eGateModeTof,enumRectification eGateRectification);
	bool SetGateModeThreshold(int iCycle,int iGate,bool &bEnable,enumGateModeAmp &eGateModeAmp,enumGateModeTof &eGateModeTof,enumRectification &eGateRectification,double &dThresholdPercent);
	bool GetGateModeThreshold(int iCycle,int iGate,bool *pbEnable,enumGateModeAmp *peGateModeAmp,enumGateModeTof *peGateModeTof,enumRectification *peGateRectification,double *pdThresholdPercent);
	bool CheckGateModeThreshold(bool &bEnable,enumGateModeAmp &eGateModeAmp,enumGateModeTof &eGateModeTof,enumRectification &eGateRectification,double &dThresholdPercent);

	bool SetGateStart(int iCycle,int iGate,double &dStart);
	bool GetGateStart(int iCycle,int iGate,double *pdStart);
	bool CheckGateStart(double &dStart);

	bool CheckGateStartStop(double &dStart,double &dStop);
	bool SetGateStop(int iCycle,int iGate,double &dStop);
	bool GetGateStop(int iCycle,int iGate,double *pdStop);
	bool CheckGateStop(double &dStop);

	bool SetGateAcqIDAmp(int iCycle,int iGate,WORD &wID);
	bool GetGateAcqIDAmp(int iCycle,int iGate,WORD *pwID);
	bool SetGateAcqIDTof(int iCycle,int iGate,WORD &wID);
	bool GetGateAcqIDTof(int iCycle,int iGate,WORD *pwID);

	bool SetTrackingGateStart(int iCycle,int iGate,bool bEnable,int &iTrackingCycleIndex,int &iTrackingGateIndex);
	bool GetTrackingGateStart(int iCycle,int iGate,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
	bool SetTrackingGateStop(int iCycle,int iGate,bool bEnable,int &iTrackingCycleIndex,int &iTrackingGateIndex);
	bool GetTrackingGateStop(int iCycle,int iGate,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
	bool SetTrackingAscan(int iCycle,bool bEnable,int &iTrackingCycleIndex,int &iTrackingGateIndex);
	bool GetTrackingAscan(int iCycle,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
	bool SetTrackingDac(int iCycle,bool bEnable,int &iTrackingCycleIndex,int &iTrackingGateIndex);
	bool GetTrackingDac(int iCycle,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
	bool CheckTracking(bool bEnable,int &iTrackingCycleIndex,int &iTrackingGateIndex);
	bool ResetTrackingTable();//this is called automatically when the IF tracking of one gate is updated.

	bool SetGainAnalog(int iCycle,float &fGain);
	bool GetGainAnalog(int iCycle,float *pfGain);
	bool CheckGainAnalog(float &fGain);

	bool SetEmissionWedgeDelay(int iCycle,int iCycleCount,double &dWedgeDelay);//"iCycleCount"=cycle count.
	bool GetEmissionWedgeDelay(int iCycle,int iCountMax,double *pdWedgeDelay);
	bool SetReceptionWedgeDelay(int iCycle,int iCycleCount,double &dWedgeDelay);
	bool GetReceptionWedgeDelay(int iCycle,int iCountMax,double *pdWedgeDelay);
	bool CheckWedgeDelay(double &dWedgeDelay);

	static bool SetAllElementEnable(bool bEnable,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	static bool SetElementEnable(int iElement,bool bEnable,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	static bool GetElementEnable(int iElement,const DWORD adwHWAperture[g_iOEMPAApertureDWordCount],bool &bEnable);

	//APERTURE DEFINITION : 2 cases.
	bool IsMultiplexer();//this function can be used to know if a multiplexer (16:128 or 32:128) is included in the system.
	//Case of a system with a multiplexer (16:128 or 32:128), please use following functions:
	//	multiplexer case: if emission and reception have same aperture:
	bool SetMultiplexerEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	bool GetMultiplexerEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	//	multiplexer case: if emission and reception aperture are different:
	//	(you cannot have more than 32 elements in case of a 32/128 and 16 elements in case of a 16/128).
	bool SetMultiplexerEnable(int iCycle,DWORD adwHWApertureEmission[g_iOEMPAApertureDWordCount],DWORD adwHWApertureReception[g_iOEMPAApertureDWordCount]);
	bool GetMultiplexerEnable(int iCycle,DWORD adwHWApertureEmission[g_iOEMPAApertureDWordCount],DWORD adwHWApertureReception[g_iOEMPAApertureDWordCount]);
	bool CheckMultiplexerAperture(DWORD adwHWApertureEmission[g_iOEMPAApertureDWordCount],DWORD adwHWApertureReception[g_iOEMPAApertureDWordCount]);//to check that emission and reception apertures are coherent.
	//Case of a system (16:16, 32:32, 64:64 etc...) without a multiplexer, then please use the following functions:
	bool SetEmissionEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	bool GetEmissionEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	bool SetReceptionEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	bool GetReceptionEnable(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
	bool SetReceptionEnable256(bool bMaster,int iCycle,DWORD adwHWAperture[2*g_iOEMPAApertureDWordCount]);

	bool SetEmissionDelays(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfDelay);
	bool GetEmissionDelays(int iCycle,int &iElementCountMax,int *piElementCount,float *pfDelay);
	bool SetEmissionDelays(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat1D &callbackArrayFloat1D);//Size = element size.
	bool GetEmissionDelays(int iCycle,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D);//Size = element size.
	bool SetEmissionWidth(int iCycle,int iElementIndex,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float &fWidth);
	bool SetEmissionWidths(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfWidth);
	bool GetEmissionWidths(int iCycle,int &iElementCountMax,int *piElementCount,float *pfWidth);
	bool SetEmissionWidths(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat1D &callbackArrayFloat1D);
	bool GetEmissionWidths(int iCycle,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D);
	bool SetReceptionGain(int iCycle,int iElementIndex,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float &fGain);
	bool SetReceptionGains(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfGain);
	bool GetReceptionGains(int iCycle,int &iElementCountMax,int *piElementCount,float *pfGain);
	bool SetReceptionGains(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat1D &callbackArrayFloat1D);
	bool GetReceptionGains(int iCycle,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D);
	bool SetReceptionDelays(int iCycle,int iLineSize,int &iFocalCount,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfDelay);
	bool GetReceptionDelays(int iCycle,int iLineSize,int &iElementCountMax,int &iFocalCountMax,int *piElementCount,int *piFocalCount,float *pfDelay);
	bool SetReceptionDelays(int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat2D &callbackArrayFloat2D);//Size1 = element size, Size2 = DDF size.
	bool GetReceptionDelays(int iCycle,int &iElementCountMax,int &iFocalCountMax,structCallbackArrayFloat2D &callbackArrayFloat2D);//Size1 = element size, Size2 = DDF size.
	bool SetReceptionDelays256(bool bMaster,int iCycle,DWORD adwHWApertureMaster[g_iOEMPAApertureDWordCount],DWORD adwHWApertureSlave[g_iOEMPAApertureDWordCount],structCallbackArrayFloat2D &masterArrayFloat2D,structCallbackArrayFloat2D &slaveArrayFloat2D);
	bool SetReceptionFocusing(int iCycle,int &iFocalCount,double *pdFocalTof);
	bool GetReceptionFocusing(int iCycle,int &iFocalCountMax,int *piFocalCount,double *pdFocalTof);
	bool SetReceptionFocusing(int iCycle,structCallbackArrayDouble1D &callbackArrayDouble1D);
	bool GetReceptionFocusing(int iCycle,int &iFocalCountMax,structCallbackArrayDouble1D &callbackArrayDouble1D);
	bool SetReceptionFocusing(int iCycle,int &iFocalCount,double *pdFocalTof,float &fCenterDelayE,float &fCenterDelayR);
	bool GetReceptionFocusing(int iCycle,int &iFocalCountMax,int *piFocalCount,double *pdFocalTof,float *pfCenterDelayE,float *pfCenterDelayR);
	bool SetReceptionFocusing(int iCycle,structCallbackArrayDouble1D &callbackArrayDouble1D,float &fCenterDelayE,float &fCenterDelayR);
	bool GetReceptionFocusing(int iCycle,int &iFocalCountMax,structCallbackArrayDouble1D &callbackArrayDouble1D,float *pfCenterDelayE,float *pfCenterDelayR);
	bool EnableDDF(int iCycle,bool bEnable);
	bool GetEnableDDF(int iCycle,bool *pbEnable);
	bool EnableDDF(int iCycle,enumFocusing &eFocusing);
	bool GetEnableDDF(int iCycle,enumFocusing *peFocusing);
	static void SetDDFTimeOfFlightMiddle(bool bEnable);
	static bool IsDDFTimeOfFlightMiddle();
	static void SetDDFWaveTrackingCorrection(int iEnable);
	static int GetDDFWaveTrackingCorrection();
	static void SetFMCReceptionSimplified(bool bEnable);
	static bool IsFMCReceptionSimplified();
	bool CheckFocalTimeOfFlight(double &dDelay);
	bool CheckEmissionWidth(float &fWidth);
	bool CheckReceptionGain(float &fGain);
	bool CheckEmissionDelay(float &fDelay);
	bool CheckReceptionDelay(float &fDelay);
	DWORD GetSWBaseAddress();

	bool SetSettingId(DWORD dwSettingId);
	bool GetSettingId(DWORD *pdwSettingId);//ask hw as for others functions.
	DWORD swGetSettingId();//ask sw (boolean memory image resident in computer) instead of hw.

	//in case of Ethernet, KeepAlive is useful to prevent communication deadlock
	//but if the developper stay a long time inside a breakpoint it will break the socket.
	bool SetKeepAlive(enumKeepAlive eKeepAlive);
	bool GetKeepAlive(enumKeepAlive *peKeepAlive);//ask hw as for others functions.

	//calibration management
	bool PerformCalibration(float &fDelayCorrectionMax,float &fTimeOffset);//In case of calibration error you can display the reporting file (see "CSWDeviceOEMPA::GetCalibrationFileReport").
				//output parameter "fDelayCorrectionMax" maximum correction delay in second
				//output parameter "fTimeOffset" call "SetTimeOffset" with this value to correct it.
	bool UpdateCalibration();//check only with the display.
	bool SetTimeOffset(float &fTime);
	bool GetTimeOffset(float *pfTime);


	bool TestFWMemoryCycle(int iCycleCount,DWORD dwData);
	bool TestFWMemoryFilter(int iFilterCount,DWORD dwData);
	static void DisableDisplayFatalErrorCustomizedAPI(bool bDisable);
	static bool IsCustomizedAPIDisabled();
	static void EnableConnectionShortcut(bool bConnectionShortcut);
	static BYTE IsConnectionShortcutEnabled();
	static void EnableConnectionShortcutWarningDisplay(bool bWarningDisplay);
	static bool IsConnectionShortcutWarningDisplayEnabled();
//<<PARAMETERS MANAGEMENT FUNCTIONS : END>>
	virtual int CHWDeviceOEMPA::vf_GetReservedData();

#ifdef _WIN64
	CAcquisitionFifo *GetFifo(enumAcquisitionFifo eFifo);
#endif _WIN64
};
