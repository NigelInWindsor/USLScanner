
#pragma once

#include "win_stub.h"
#include "UTBasicEnum.h"
#include "UTBasicType.h"
#include "UTUsbDevice.h"			//USB connection

#include "UTKernelStream.h"			//acquisition  management : definition of stream coming from FW to SW.
#include "UTKernelDriverExport.h"
#include "UTKernelDriverSystem.h"


typedef struct UTKERNEL_DRIVER_API_ALIGN structTFMParameters{
									structTFMParameters(){memset(this,0,sizeof(structTFMParameters));};
									int GetTFMAscanIndex(int iElement,int iCycle);
									enumTFMParameters eTFMParameters;
									enumTFMOptions eOption;
									unionTFMParameters	parameter;
									double dSamplingFrequency;//unit Hz
									double dVelocity;//unit m/s
									double dGain;//unit dB
									int iCycleCountIn;
									int iAscanPointCountIn;
									int iCycleCountOut;
									int iAscanPointCountOut;
									int iAscanBitSizeOut;
									int iModeOut;
								}structTFMParameters;
typedef void (*pCallbackHWMemory)(CHWDevice *pHWDevice, DWORD addr, DWORD data, int size);

class CHWDevice;

class UTKERNEL_DRIVER_API_ALIGN _CUTDeviceStream
{
private:
	ULONGLONG m_ullPrivateMemory[4];
public:
	_CUTDeviceStream();
	_CUTDeviceStream(DWORD *pdwData,int iSize);

};

//This class can be used to manage communication stream and sub stream.
class UTKERNEL_DRIVER_API_ALIGN CUTDeviceStream : public _CUTDeviceStream
{
public:
	CUTDeviceStream();
	CUTDeviceStream(char* data, int size);
	CUTDeviceStream(char* data, int size,int i);
	~CUTDeviceStream();

	void RemoveAll();
	bool IsBuilt() const;
	bool UpdateComId(WORD wComId,int iRetransmit,WORD wMarge=0);

	int GetSize() const;
	bool BuildSwap(int iIndexStart,int iIndexStop,int iIndexEnd);
	int BuildGrow(DWORD dwData);
	int BuildGrow(int iCount,DWORD *pdwData);
	bool BuildUpdate(int iIndex,DWORD dwData);
	bool BuildUpdate(int iIndex,int iCount,DWORD *pdwData);

	//build stream for simulation
	bool BuildAscan(int iCount,int iByteSize,const char *pBuf);//test purpose
	bool BuildText(const char *pText);
	bool Build(const char *pBuf,int iSize);

	//to find each sub stream inside the stream
	const DWORD *GetStream();
	const DWORD *GetSubStreamFirst(int &iSubStreamCount,DWORD &dwSettingId,int &iIndexSubStream,bool &bText,enumSubStreamId &byID,BYTE &byVersion,WORD &wByteSize);
	const DWORD *GetSubStreamNext(int &iIndexSubStream,enumSubStreamId &byID,BYTE &byVersion,WORD &wByteSize);
	const char* GetText() const;
	int GetSubStreamCount() const;

	//to send the stream by the driver.
	int Send(CHWDevice *pHWDevice);

};

class CSWDevice;
class CDrvDispatch;
class CUTSocket;
class CTimerProcess;

class UTKERNEL_DRIVER_API_ALIGN _CUTSocketMonitor
{
private:
	ULONGLONG m_ullPrivateMemory[44];
protected:

};

class UTKERNEL_DRIVER_API_ALIGN CUTSocketMonitor :
	public _CUTSocketMonitor
{
public:

	CUTSocketMonitor();
	~CUTSocketMonitor();

	bool IsConnected();
	int GetCountBadStopFlag();
	void EnableBadStopFlagCorrection(bool bEnable);
	bool IsBadStopFlagCorrectionEnabled();
	void SetBadStopFlagMsgCount(int iCount);
	int GetBadStopFlagMsgCount();

	bool IsIO();
	bool IsAscan();
	bool IsCscan();
	bool IsParametersSWToHW();//write
	bool IsParametersHWToSW();//read
	bool IsAcknowledgment();

	void EnableIO(bool bEnable);
	void EnableAscan(bool bEnable);
	void EnableCscan(bool bEnable);
	void EnableParametersSWToHW(bool bEnable);//write
	void EnableParametersHWToSW(bool bEnable);//read
	void EnableAcknowledgment(bool bEnable);
	void EnableParametersHWToSWAndNotifyApplication(bool bEnable);//read

	int Send_0x0104(bool bEnableFilter,bool bHWToSW,const char *data,int size);
	int Send_0x10(bool bEnableFilter,bool bHWToSW,const char *data,int size);
	bool OnTextStream(const char *pText);


};

class UTKERNEL_DRIVER_API_ALIGN _CUTServerSocketMonitor
{
private:
	ULONGLONG m_ullPrivateMemory[30];
protected:

};

class UTKERNEL_DRIVER_API_ALIGN CUTServerSocketMonitor :
	public _CUTServerSocketMonitor
{
private:

// Construction
public:

	CUTServerSocketMonitor(CUTSocketMonitor *pMonitorSocket);
	~CUTServerSocketMonitor();

	bool Start(int iPort);
	bool IsStarted() const;


// Attributes

// Overridable callbacks
protected:
	void OnClose(int nErrorCode);
	void CommunicationReport(wchar_t *string);
	void CommunicationReport(char *string);
	void OnListened(int nErrorCode);
	void OnAccept(int nErrorCode);

// Implementation
public:
};
/////////////////////////////////////////////////////////////////////////////////////////


class CSWFilterOEMPA;
class CDeviceMonitor;
class CSWDevice;
class CUTKernelDevice;
class CSWEncoder;

class UTKERNEL_DRIVER_API_ALIGN _CSWEncoder
{
private:
	ULONGLONG m_ullPrivateMemory[13];
protected:

};

class UTKERNEL_DRIVER_API_ALIGN CSWEncoder :
	public _CSWEncoder
{

public:
	CSWEncoder(CSWDevice *pSWDevice,enumSharedEncoder eSharedEncoder);
	~CSWEncoder();
	int operator=(const CSWEncoder &a);
	bool operator==(const CSWEncoder &a) const;
	bool operator!=(const CSWEncoder &a) const;

//setting part
	bool Enable(bool bEnabled);//value could be negative if encoder need to be inverted.
	bool IsEnabled() const;

	bool lSetResolution(long lResolution);//this function is only for integer type resolution (the resolution divider is equal to 1).
	bool lSetResolution(long lResolution,DWORD dwDivider);//divider is useful for a double type resolution encoder. DoubleResolution=Resolution/Divider.
	long lGetResolution() const;
	DWORD GetDivider() const;
	bool dSetResolution(double dResolution);//it is the same than to call "ConvertResolution(dResolution,lResolution,dwDivider);" followed by "SetResolution(lResolution,dwDivider)"
	double dGetResolution() const;
	static bool ConvertResolution(double dResolution,long &lResolution,DWORD &dwDivider);

	bool SetUnit(enumUnit eUnit);
	enumUnit GetUnit() const;

	bool SetType(enumEncoderType eEncoderType);
	enumEncoderType GetType() const;

	bool SetDigitalInput(enumIndexDigitalInput eEncoderInput,enumDigitalInput eDigitalInput);
	enumDigitalInput GetDigitalInput(enumIndexDigitalInput eEncoderInput) const;

//acquisition part
	bool SetSpeedDistance(double dValue);
	bool GetSpeedDistance(double &dValue) const;

	bool SetInspectionHWValue(int iValue);
	int GetInspectionHWValue() const;
	bool SetInspectionSWValue(double dValue);
	double GetInspectionSWValue() const;

	bool SetInspectionSpeed(double dValue);
	double GetInspectionSpeed() const;

	bool SetInspectionLength(double dValue);
	double GetInspectionLength() const;

	bool SetInspectionCount(int iValue);
	int GetInspectionCount() const;



	//Quick functions with no event notification (used by the acquisition process).
	//usefull functions for user who want to manage HW encoder process.
	bool GetEncoderIndex(int &iEncoderIndex) const;
	static bool SetInspectionHWValue(int iEncoderIndex,int iValue);
	static bool GetInspectionHWValue(int iEncoderIndex,int &iValue);
	static bool SetInspectionSWValue(int iEncoderIndex,double dValue);
	static bool GetInspectionSWValue(int iEncoderIndex,double &dValue);


};

class UTKERNEL_DRIVER_API_ALIGN _CDeviceMonitor
{
private:
	ULONGLONG m_ullPrivateMemory[4];
protected:

};

class UTKERNEL_DRIVER_API_ALIGN CDeviceMonitor : public _CDeviceMonitor
{
friend class CSWDevice;
	CDeviceMonitor(CSWDevice *pSWDevice);
public:
	~CDeviceMonitor();
	int operator=(const CDeviceMonitor &a);
	bool operator==(const CDeviceMonitor &a);
	bool operator!=(const CDeviceMonitor &a);

	bool SetPortServer(unsigned short usValue);
	unsigned short GetPortServer() const;

	int SetConnectionStateServer(enumConnectionState eConnection,bool bDisplayErrorMsg);
	enumConnectionState GetConnectionStateServer() const;
	bool IsServerConnected() const;

	bool SocketDisconnect();
	bool IsSocketConnected() const;

	bool EnableIO(bool bEnable);
	bool IsIOEnabled() const;

	bool EnableAscan(bool bEnable);
	bool IsAscanEnabled() const;

	bool EnableCscan(bool bEnable);
	bool IsCscanEnabled() const;

	bool EnableParametersSWToHW(bool bEnable);
	bool IsParametersSWToHWEnabled() const;

	bool EnableParametersHWToSW(bool bEnable);
	bool IsParametersHWToSWEnabled() const;

	bool EnableAcknoledgment(bool bEnable);
	bool IsAcknoledgmentEnabled() const;


};

class UTKERNEL_DRIVER_API_ALIGN _CSWDevice
{
private:
	ULONGLONG m_ullPrivateMemory[307];
protected:

};

class UTKERNEL_DRIVER_API_ALIGN CSWDevice : public _CSWDevice, public CDeviceMonitor
{

public:
	CSWDevice(CHWDevice *pHWDevice,enumHardware eDefaultHW,enumSharedEncoder eSharedEncoderX,enumSharedEncoder eSharedEncoderY);
	~CSWDevice();
	int operator=(const CSWDevice &a);
	bool operator==(const CSWDevice &a);
	bool operator!=(const CSWDevice &a);
	CSWEncoder *GetSWEncoder(int iEncoderIndex);
	const CSWEncoder *cGetSWEncoder(int iEncoderIndex) const;
	CDeviceMonitor& Monitor();
	CHWDevice *GetHWDevice();
	const CHWDevice *cGetHWDevice() const;

//root ID management.
	bool SetBoardName(const wchar_t *pName);//to register this new data in the kernel database.
	bool GetBoardName(int iSize,wchar_t *pName);


//Setting

	bool SetHardware(enumHardware eHW);
	enumHardware GetHardware() const;

	bool SetCommunication(enumCommunication eCommunication);
	enumCommunication GetCommunication() const;

	bool GetConfigurationFilePath(bool bFlashName,wchar_t *pPathName,int iSize);//to find the configuration file path.
	bool GetConfigurationFilePath(wchar_t *pPathName,int iSize);//to find the configuration file path.

	bool SetSetupFileDefault(bool bOEMPA,wchar_t *wcFile);
	bool GetSetupFileDefault(bool bOEMPA,int iSize,wchar_t *wcFile);
	bool SetSetupFileCurrent(const wchar_t *wcFile);
	bool GetSetupFileCurrent(int iSize,wchar_t *wcFile);
	bool GetElementFile(int iSize,wchar_t *wcFile);//Dead elements.

	bool SetAcquisitionFlush(enumAcquisitionFlush eAcquisitionFlush);
	enumAcquisitionFlush GetAcquisitionFlush() const;

	bool SetConnectionState(enumConnectionState eConnection,bool bDisplayErrorMsg);
	enumConnectionState GetConnectionState() const;//for MatchedDevices: this function called for the master takes also into account the slave.
	bool IsConnected() const;//for MatchedDevices: this function called for the master takes also into account the slave.

	bool CheckProcessId();//only one process have ownership of the driver.

	bool SetCfgStatus(enumUpdateStatus eUpdateStatus);
	enumUpdateStatus GetCfgStatus() const;

	int SetStreamCount(int iCount);
	int GetStreamCount() const;
	int SetStreamRetransmit(int iCount);
	int GetStreamRetransmit() const;
	int SetStreamError(int iCount);
	int GetStreamError() const;

	int GetLostCountAscan() const;
	int SetLostCountAscan(int iDataLostCount);
	int GetLostCountCscan() const;
	int SetLostCountCscan(int iDataLostCount);
	int GetLostCountEncoder() const;
	int SetLostCountEncoder(int iDataLostCount);
	int GetLostCountUSB3() const;
	int SetLostCountUSB3(int iDataLostCount);
	int GetLostCountFifo(enumAcquisitionFifo eFifo) const;//data (ascan, cscan or IO) lost because the output speed to get data is too low.
	int GetErrorCountUSB3() const;
	int SetErrorCountUSB3(int iDataErrorCount);
	void ResetCounters();


	//bool SetEncoderCount(int iCount);//0 1 or 2.
	int GetEncoderEnabledCount() const;
	void swProcessEncoder(structAcqInfoEx &acqInfo);

	bool EnablePulser(bool bEnable);
	bool IsPulserEnabled() const;


	//current (for this device only)
	bool SetLockDisableShot(bool bDisable);
	bool GetLockDisableShot() const;
	bool SetUnlockEnableShot(bool bEnable);
	bool GetUnlockEnableShot() const;

	//in software configuration file (used as default for all devices)
	static bool SetLockDefaultDisablePulser(bool bDisable);
	static bool GetLockDefaultDisablePulser();
	static bool SetUnlockDefaultEnablePulser(bool bEnable);
	static bool GetUnlockDefaultEnablePulser();

	bool SetAcqSpeedAscan(double dSpeed);
	double GetAcqSpeedAscan() const;
	bool SetAcqSpeedCscan(double dSpeed);
	double GetAcqSpeedCscan() const;
	bool SetAcqSpeedIO(double dSpeed);
	double GetAcqSpeedIO() const;



	void EnableAscanEncoder(bool bEnable);
	bool IsAscanEncoderEnabled();

	static void WriteCfgBool(const wchar_t *pSection,const wchar_t *pKey,bool bValue);
	static bool ReadCfgBool(const wchar_t *pSection,const wchar_t *pKey,bool bDefault,bool &bCreate);
	static int ReadCfgInt(const wchar_t *pSection,const wchar_t *pKey,int iDefault,bool &bCreate);
	static void WriteCfgInt(const wchar_t *pSection,const wchar_t *pKey,int iValue);
	static void WriteCfgString(const wchar_t *pSection,const wchar_t *pKey,wchar_t *pValue);
	static bool ReadCfgString(const wchar_t *pSection,const wchar_t *pKey,const wchar_t *pDefault,wchar_t *pValue,int iSize,bool &bCreate);

};


class CAcquisitionFifo;
class UTKERNEL_DRIVER_API_ALIGN _CHWDevice
{
private:
	ULONGLONG m_ullPrivateMemory[9725];

public:
	_CHWDevice(CHWDevice *pThis,enumHardware eDefaultHardware,enumSharedEncoder eSharedEncoderX,enumSharedEncoder eSharedEncoderY);
	~_CHWDevice();


};

class UTKERNEL_DRIVER_API_ALIGN CHWDevice : public _CHWDevice
{
private:

	CHWDevice();
	CHWDevice(const CHWDevice& a);
	~CHWDevice();

public:
	CSWDevice *GetSWDevice();
	const CSWDevice *cGetSWDevice() const;
	int GetDeviceId();
	static CHWDevice *GetHWDevice(int iDeviceId);
	virtual enumHardware CHWDevice::vf_GetHardware();
	void SetDerivedClass(wchar_t *pClassName,void *pDerivedClass);
	void GetDerivedClass(int iClassNameSize,wchar_t *pClassName,void* &pDerivedClass);

	CHWDevice *GetMatchedDevice();//link to the matched device (an OEMPA 64:64a could be matched with an OEMPA 64:64b to build an OEMPA 128:128).
	const CHWDevice *cGetMatchedDevice() const;//link to the matched device (an OEMPA 64:64a could be matched with an OEMPA 64:64b to build an OEMPA 128:128).
	enumHardwareLink GetMatchedDeviceHwLink() const;//is the matched device a slave or a master ?
	//1.1.5.4a to manage the 256:256 with four 64:64 (64:64A, 64:64B, 64:64C, 64:64D).
	CHWDevice *GetMatched256Device();
	void *GetMatched256DeviceOEMPA();
	const CHWDevice *cGetMatched256Device() const;
	enumHardwareLink GetMatched256DeviceHwLink() const;

//MultiSystem management start (shared functions between applications)
	enumHardwareLink GetHardwareLink() const;//link
	int GetMasterDeviceId() const;//to get the master device id, return -1 if the device is not a slave.

	//FUNCTION to call before connection (this information is registered in the default setup OEMPA file)
	bool SetDefaultHwLink(enumHardwareLink eHardwareLink,bool &bPreviousMasterUnregistered);//define the default link for this device (default link before connection).
													//Only one master could be defined between all devices.
													//output "bPreviousMasterUnregistered" : return false if no error, return true to notify that previou master have been unregistered (only one master is possible).
	enumHardwareLink GetDefaultHwLink() const;
	bool IsDefaultHwLinkEnabled() const;//return false if device connection and initialisation has been done and true otherwise.

	//FUNCTION to call after connection
	//DO NOT CALL LockDevice/UnlockDevice to call following:
	bool SlaveConnect(int iMasterDeviceId);//connect current unlinked device to the specified master or unlinked device (that should be conncted).
	bool SlaveDisconnect();//disconnect current slave device.

	//static information functions : called from any application on any device.
	static const int GetDeviceCountMax();//return the maximum count of device that can be managed.
	static bool IsConnected(int iDeviceId);//for MatchedDevices: master and slave are managed independently.
	static bool GetDefaultHwLink(int iDeviceId,enumHardwareLink &eHardwareLink);//read the default link (used before connection with the device).
	static enumHardwareLink GetHardwareLink(int iDeviceId);//read the current link state (after connection with the device).
	static int GetMasterDeviceId(int iSlaveDeviceId);//to get the master device id, return -1 if the device is not a slave.
	static int GetSlaveCount(int iMasterDeviceId);//return -1 if the device is not a master.
	static int GetSlaveDeviceId(int iMasterDeviceId,int iListSize,int *piDeviceIdList);//return the list of slaves of the master.
	static bool ReadBoardName(int iDeviceId,wchar_t *stringId,int iSize);
	static bool ReadCommunicationName(int iDeviceId,wchar_t *stringId,int iSize);
	static int GetMonitorPort(int iDeviceId);

	static bool GetMatchedDeviceHwLink(int iDeviceId,enumHardwareLink &eMatchedDeviceHwLink,int &iMatchedDeviceId);
	static void EnableMatchedDeviceHwLink(bool bEnable);//enable matched device (at connection time).
	static bool IsMatchedDeviceHwLinkEnabled();
	//1.1.5.4a to manage the 256:256 with four 64:64 (64:64a, 64:64b, 64:64c, 64:64d).
	static bool GetMatched256DeviceHwLink(int iDeviceId,enumHardwareLink &eMatchedDeviceHwLink,int &iMatchedDeviceId);

	static void EnableCommonKeepAlive(bool bEnable);
	static bool IsCommonKeepAliveEnabled();
	static void SetCommonLockDisableShot(bool bDisable);
	static bool GetCommonLockDisableShot();
	static void SetCommonUnlockEnableShot(bool bEnable);
	static bool GetCommonUnlockEnableShot();

	//LockDevice/UnlockDevice is required to call following:
	bool SetTimeStamp(bool bReset,bool bStopNextStandby);//to synchronize time stamp of device chain.
									//bReset=true: reset the time stamp.
									//bReset=false: set the hardware time stamp equal to the computer time stamp.
									//bStopNextStandby=true: to be able to read time stamp on the next standby (good precision).
									//bStopNextStandby=false: it willnot be possible to read time stamp on the next standby (bad precision).
	bool GetTimeStamp(/*in/out*/int &iSystemCount,ULONGLONG &ullTimeSW,ULONGLONG *pullTimeHW/*[iSystemCount]*/);
//MultiSystem management end

public:
//--------------------------------------------------------------------------------
//Device low level management
	//Those function can be used to receive acquisition data.
	bool SetAcquisitionParameter(void *pParameter);//first parameter of the callback acquisition function.
	void* GetAcquisitionParameter();
	bool SetAcquisitionAscan_0x00010103(pTypeAcquisitionAscan_0x00010103 pProcess);
	pTypeAcquisitionAscan_0x00010103 GetAcquisitionAscan_0x00010103();
	bool SetAcquisitionCscan_0x00010X02(pTypeAcquisitionCscan_0x00010X02 pProcess);
	pTypeAcquisitionCscan_0x00010X02 GetAcquisitionCscan_0x00010X02();
	bool SetAcquisitionIO_0x00010101(pTypeAcquisitionIO_0x00010101 pProcess,bool bDigitalEdgesOnly=true);
	pTypeAcquisitionIO_0x00010101 GetAcquisitionIO_0x00010101(bool &bDigitalEdgesOnly);
	bool SetAcquisitionIO_1x00010101(pTypeAcquisitionIO_1x00010101 pProcess,bool bDigitalEdgesOnly=true);
	pTypeAcquisitionIO_1x00010101 GetAcquisitionIO_1x00010101(bool &bDigitalEdgesOnly);
	bool SetAcquisitionStream(pTypeAcquisitionStream pProcess);
	pTypeAcquisitionStream GetAcquisitionStream();

	CAcquisitionFifo *GetAcquisitionFifo(enumAcquisitionFifo eFifo);

	bool SetAcquisitionInfo(pTypeAcquisitionInfo pProcess);
	pTypeAcquisitionInfo GetAcquisitionInfo();

	static bool SetCallbackNotification(pTypeCallbackNotification pProcess);
	static pTypeCallbackNotification GetCallbackNotification();
	static int BrowseCallbackNotification();//the CallbackNotification function is called for each CHWDevice instance.

	bool IsDriverEncoderManagementEnabled();
	void EnableDriverEncoderManagement(bool bEnable);

//lock/unlock the device
//you have to lock the device before to use its functions (for example "SetStandby" function).
//(the communication link with the HW is shared between all threads, only one at a time
//can lock it)
	bool LockDevice(enumAcquisitionState eAcqState=eAcqDefault);//use "eAcqOff" to disable shot pulse.
					//eAcqDefault is used to read the value from the configuration file.
					//return true if no error and false in case of communication error.
	bool UnlockDevice(enumAcquisitionState eAcqState=eAcqDefault);//use "eAcqOn" to enable shot pulse again.
					//eAcqDefault is used to read the value from the configuration file.
					//return true if no error and false in case of communication error.
	bool IsDeviceLocked();//in case of multiple systems, check that all are locked.
	bool IsDeviceUnlocked();//in case of multiple systems, check that all are unlocked.
	int GetDeviceLockCount();
	DWORD GetSettingId();

//management of the device (can be called after device has been locked).
	bool Flush();//return true if no error and false in case of communication error.

//Don't use those very low level functions, they are reserved for advanced user.
	bool WriteHW(DWORD dwAddress,DWORD dwData,int iSize);
	bool ReadHW(DWORD dwAddress,DWORD *pdwData,int iSize);
	static bool SetCallbackHWMemory(pCallbackHWMemory _pCallbackHWMemory);
	static pCallbackHWMemory GetCallbackHWMemory();

public:
	bool IsCallbackStandbyEnabled();
	void EnableCallbackStandby(bool bEnable);
	bool IsCallbackEnabled();
	void EnableCallback(bool bEnable);
	void _EnableCallback(bool bEnable);
	bool IsCallbackEnabled(bool bFeedbackDriver);
	bool IsFeedbackDriverEnabled();
	void EnableFeedbackDriver(bool bFeedbackDriver);

public://don't use following functions
	bool GetCommunicationName(wchar_t *stringId,int iSize);
	virtual int CHWDevice::vf_GetReservedData();
	virtual bool CHWDevice::vf_SetAcquisitionFlush(enumAcquisitionFlush eAcquisitionFlush);
	virtual enumAcquisitionFlush CHWDevice::vf_GetAcquisitionFlush() const;
	virtual LRESULT CHWDevice::vf_CallbackKernelDriver(const wchar_t *param_pPathDefaultLanguage,CHWDevice *pHWDevice,WORD wCallbackEncoder,WORD wCallbackParam);
	virtual bool CHWDevice::vf_SetCommunication(enumCommunication eCommunication);
	virtual bool CHWDevice::vf_LoadCfgFile();
	virtual bool CHWDevice::vf_ReLoadCfgFile(){return false;};//=0;
	virtual bool CHWDevice::vf_ReadStatusRegister(){return false;};//=0;
	virtual bool CHWDevice::vf_SetStandby(bool bLock,enumAcquisitionState &eAcqState);

};
