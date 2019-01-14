#pragma once
//HERE IS MACRO AND INLINE FUNCTION TO SHOW YOU HOW TO GET COMPATIBILITY WITH OLDER VERSION
//ALL MACROS HAVE NOT BEEN IMPLEMENTED BUT THERE IS A LOT OF EXAMPLES, SO YOU CAN IMPLEMENT NEW ONE BY YOURSELF
//But it is better to not use them directly and to rewrite your code, for that you can get inspiration from those macros.

//LOW LEVEL FUNCTIONS TO ACCESS THE HARDWARE
//functions to create/delete devices
//DRIVEROEMPA_API bool WINAPI OEMPA_Open(int &iDriverId);
	//To create a new "OEMPA" object.
	//input parameter "eCommunication" : communication link it can be "eEthernet" or "eUSB".
	//output parameter "iDriverId" : device identifier required for next operation.
	//return value : true if no error, false otherwise.
__inline bool WINAPI OEMPA_Open(int &iDriverId)
{
	CHWDeviceOEMPA *pDeviceOEMPA;

	pDeviceOEMPA = new CHWDeviceOEMPA;//instead to declare a local variable you can declare a global variable in your own code and do this new yourself!
	if(!pDeviceOEMPA)
		iDriverId = -1;
	else
		iDriverId = pDeviceOEMPA->GetDeviceId();
	return (iDriverId==-1?false:true);
}

//DRIVEROEMPA_API bool WINAPI OEMPA_Close(int iDriverId);
	//To destroy an "OEMPA" object.
	//input parameter "iDriverId" : device identifier (see "OEMPA_Open").
	//return value : true if no error, false otherwise.
__inline bool WINAPI OEMPA_Close(int iDriverId)
{
	CHWDevice *pDevice;
	CHWDeviceOEMPA *pDeviceOEMPA;

	pDevice = CHWDevice::GetHWDevice(iDriverId);
	if(!pDevice)
		return false;
	pDeviceOEMPA = dynamic_cast<CHWDeviceOEMPA*>(pDevice);
	if(!pDeviceOEMPA)
		return false;
	delete pDeviceOEMPA;
	return true;
}

//DRIVEROEMPA_API void WINAPI OEMPA_CloseAll();
	//Gateway to class API (other way to manage device with all features).
//DRIVEROEMPA_API CSWDevice* WINAPI OEMPA_GetDriver(int iDriverId);//high level access.
__inline CSWDevice* WINAPI OEMPA_GetDriver(int iDriverId)
{
	CHWDevice *pDevice;

	pDevice = CHWDevice::GetHWDevice(iDriverId);
	if(!pDevice)
		return false;
	return pDevice->GetSWDevice();
}

//DRIVEROEMPA_API CHWDevice* WINAPI OEMPA_GetKernelDriver(int iDriverId);//high level access.
#define OEMPA_GetKernelDriver(iDriverId)						CHWDevice::GetHWDevice(iDriverId)
//DRIVEROEMPA_API CHWDeviceOEMPA* WINAPI OEMPA_GetKernelDriverOEMPA(int iDriverId);//high level access.
#define OEMPA_GetKernelDriverOEMPA(iDriverId)					(CHWDevice::GetHWDevice(iDriverId) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)):NULL)
//DRIVEROEMPA_API void WINAPI OEMPA_LockDriver(bool bEnable);//high level access.
//DRIVEROEMPA_API int WINAPI OEMPA_GetCountCommunicationError(int iDriverId);
#define OEMPA_GetCountCommunicationError(iDriverId)				(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->GetSWDevice()->GetStreamError():0)

	//connection management
//DRIVEROEMPA_API bool WINAPI OEMPA_SetCommunication(int iDriverId,enumCommunication eCommunication);
#define OEMPA_SetCommunication(iDriverId,eCommunication)		(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->GetSWDevice()->SetCommunication(eCommunication):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetCommunication(int iDriverId,enumCommunication &eCommunication);
	//ethernet management
//DRIVEROEMPA_API bool WINAPI OEMPA_SetEthernet(int iDriverId,wchar_t *pIP,WORD wPort);
#define OEMPA_SetEthernet(iDriverId,pIP,wPort)					(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetSWDeviceOEMPA()->SetIPAddress(pIP) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetSWDeviceOEMPA()->SetPort(wPort):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEthernet(int iDriverId,int iIPSize,wchar_t *pIP,WORD &wPort);
	//USB management, see also "CUTUsbDevice".
//DRIVEROEMPA_API bool WINAPI OEMPA_SetUSB(int iDriverId,const wchar_t *pDeviceName);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetUSB(int iDriverId,wchar_t *pDeviceName,int iSize);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetConnectionState(int iDriverId,enumConnectionState eConnection,bool bDisplayErrorMsg);
#define OEMPA_SetConnectionState(iDriverId,eConnection,bDisplayErrorMsg)		(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->GetSWDevice()->SetConnectionState(eConnection,bDisplayErrorMsg):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_IsConnected(int iDriverId);
#define OEMPA_IsConnected(iDriverId)								(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->GetSWDevice()->IsConnected():false)
//low level acquisition management
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAcquisitionAscan_0x00010103(int iDriverId,pTypeAcquisitionAscan_0x00010103 pProcess);
#define OEMPA_SetAcquisitionAscan_0x00010103(iDriverId,pProcess)	(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->SetAcquisitionAscan_0x00010103(pProcess):false)
//DRIVEROEMPA_API pTypeAcquisitionAscan_0x00010103 WINAPI OEMPA_GetAcquisitionAscan_0x00010103(int iDriverId);
#define OEMPA_GetAcquisitionAscan_0x00010103(iDriverId)				(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->GetAcquisitionAscan_0x00010103():NULL)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAcquisitionCscan_0x00010X02(int iDriverId,pTypeAcquisitionCscan_0x00010X02 pProcess);
#define OEMPA_SetAcquisitionCscan_0x00010X02(iDriverId,pProcess)	(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->SetAcquisitionCscan_0x00010X02(pProcess):false)
//DRIVEROEMPA_API pTypeAcquisitionCscan_0x00010X02 WINAPI OEMPA_GetAcquisitionCscan_0x00010X02(int iDriverId);
				//if bDigitalEdgesOnly=true to get only digital inputs updates (automatically sent by the FW when required).
				//if bDigitalEdgesOnly=false you can can get also encoder but please notice that encoder are already processed by the driver.
#define OEMPA_GetAcquisitionCscan_0x00010X02(iDriverId)				(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->GetAcquisitionCscan_0x00010X02():NULL)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAcquisitionIO_0x00010101(int iDriverId,pTypeAcquisitionIO_0x00010101 pProcess,bool bDigitalEdgesOnly=true);
#define OEMPA_SetAcquisitionIO_0x00010101(iDriverId,pProcess,bDigitalEdgesOnly)		(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->SetAcquisitionIO_0x00010101(pProcess,bDigitalEdgesOnly):false)
//DRIVEROEMPA_API pTypeAcquisitionIO_0x00010101 WINAPI OEMPA_GetAcquisitionIO_0x00010101(int iDriverId,bool &bDigitalEdgesOnly);
#define OEMPA_GetAcquisitionIO_0x00010101(iDriverId,bDigitalEdgesOnly)			(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->GetAcquisitionIO_0x00010101(bDigitalEdgesOnly):NULL)
//easy callback function management
//typedef LRESULT (WINAPI* pOEMPACallbackFunction)(const wchar_t *pMember,LONG lParam);//lParam is the value that have been used during registration (see "OEMPA_RegisterCallback").
//DRIVEROEMPA_API bool WINAPI OEMPA_RegisterCallback(int iDriverId,pOEMPACallbackFunction pFunc,LONG lParam,bool bUserInterface);
	//bUserInterface : true to call "pFunc" from an intermediate Windows procedure.
	//					false otherwise, in this case the call can occured from a worker thread,
	//					so in this case it will not possible to call special WIndows functions
	//					for example "UpdateData()".
//DRIVEROEMPA_API bool WINAPI OEMPA_UnregisterCallback(int iDriverId);
//functions to manage multiple devices.
//DRIVEROEMPA_API bool WINAPI OEMPA_LockDevices(enumAcquisitionState eAcqState=eAcqDefault);
//DRIVEROEMPA_API bool WINAPI OEMPA_UnlockDevices(enumAcquisitionState eAcqState=eAcqDefault);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetTimeStamp(int iDriverId);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetTimeStamps();
//DRIVEROEMPA_API int WINAPI OEMPA_GetTimeStamps(int iSize,ULONGLONG *pullTimeSW,ULONGLONG *pullTimeHW);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetTimeStamp(int iDriverId,ULONGLONG &ullTimeSW,ULONGLONG &ullTimeHW);
//functions to lock to the device.
//DRIVEROEMPA_API bool WINAPI OEMPA_LockDevice(int iDriverId,enumAcquisitionState eAcqState=eAcqDefault);
__inline bool OEMPA_LockDevice(int iDriverId,enumAcquisitionState eAcqState)
{
	return (CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->LockDevice(eAcqState):false);
}
__inline bool OEMPA_LockDevice(int iDriverId)
{
	return (CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->LockDevice():false);
}
//DRIVEROEMPA_API bool WINAPI OEMPA_UnlockDevice(int iDriverId,enumAcquisitionState eAcqState=eAcqDefault,DWORD *pdwSettingId=NULL);
__inline bool OEMPA_UnlockDevice(int iDriverId,enumAcquisitionState eAcqState)
{
	return (CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->UnlockDevice(eAcqState):false);
}
__inline bool OEMPA_UnlockDevice(int iDriverId)
{
	return (CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->UnlockDevice():false);
}
extern int *g_piDriverId;
__inline bool OEMPA_LockDevices()
{
	return OEMPA_LockDevice(*g_piDriverId);
}
__inline bool OEMPA_LockDevices(enumAcquisitionState eAcqState)
{
	return OEMPA_LockDevice(*g_piDriverId,eAcqState);
}
__inline bool OEMPA_UnlockDevices()
{
	return OEMPA_UnlockDevice(*g_piDriverId);
}
__inline bool OEMPA_UnlockDevices(enumAcquisitionState eAcqState)
{
	return OEMPA_UnlockDevice(*g_piDriverId,eAcqState);
}
//DRIVEROEMPA_API bool WINAPI OEMPA_GetSettingId(int iDriverId,DWORD &dwSettingId);
//management of the device (can be called after device is locked).
//DRIVEROEMPA_API bool WINAPI OEMPA_Flush(int iDriverId);
#define OEMPA_Flush(iDriverId)							(CHWDevice::GetHWDevice(iDriverId) ? CHWDevice::GetHWDevice(iDriverId)->Flush():false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetStreamPackage(int iDriverId,enumStreamPackage eStreamPackage);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetStreamPackage(int iDriverId,enumStreamPackage &eStreamPackage);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetCommunicationOption(int iDriverId,enumCommunicationOption eCommunicationOption);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetCommunicationOption(int iDriverId,enumCommunicationOption &eCommunicationOption);
//DRIVEROEMPA_API void WINAPI OEMPA_RemoveAll(int iDriverId);
//DRIVEROEMPA_API bool WINAPI OEMPA_ReLoadCfgFile(int iDriverId);

//PARAMETERS FUNCTIONS (SPECIAL FUNCTIONS)
//DRIVEROEMPA_API bool WINAPI OEMPA_EnableShot(int iDriverId,bool &bShot);
#define OEMPA_EnableShot(iDriverId,bShot)				(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->EnablePulser(bShot):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEnableShot(int iDriverId,bool *pbShot);
__inline bool OEMPA_GetEnableShot(int iDriverId,bool *pbShot)
{
	CHWDevice *pDevice;
	CHWDeviceOEMPA *pDeviceOEMPA;

	pDevice = CHWDevice::GetHWDevice(iDriverId);
	if(!pDevice)
		return false;
	pDeviceOEMPA = dynamic_cast<CHWDeviceOEMPA*>(pDevice);
	*pbShot = pDeviceOEMPA->GetSWDeviceOEMPA()->IsPulserEnabled();
	return true;
}

//<<PARAMETERS MANAGEMENT FUNCTIONS : BEGIN>>
//those functions return true in case of no error and false in case of error.
//DRIVEROEMPA_API bool WINAPI OEMPA_GetFWId(int iDriverId,WORD *pwFWId);//to get the device version.
//DRIVEROEMPA_API bool WINAPI OEMPA_GetElementCountMax(int iDriverId,int *piCount);//to get the maximum element count of an aperture.
//DRIVEROEMPA_API bool WINAPI OEMPA_GetApertureCountMax(int iDriverId,int *piCount);//to get the maximum element count of the system.
//DRIVEROEMPA_API int WINAPI OEMPA_GetBoardCount(int iDriverId);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetTemperatureSensor(int iDriverId,int iIndexBoard,int iIndexSensor,WORD *pwTemperature);
//DRIVEROEMPA_API bool WINAPI OEMPA_ResetEncoder(int iDriverId,int iEncoderIndex);
#define OEMPA_ResetEncoder(iDriverId,iEncoderIndex)				(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->ResetEncoder(iEncoderIndex):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_EnableAscan(int iDriverId,bool &bEnable);//to enable ascan.
#define OEMPA_EnableAscan(iDriverId,bEnable)					(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->EnableAscan(bEnable):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEnableAscan(int iDriverId,bool *pbEnable);
#define OEMPA_GetEnableAscan(iDriverId,pbEnable)				(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetEnableAscan(pbEnable):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_EnableCscanTof(int iDriverId,bool &bEnable);
#define OEMPA_EnableCscanTof(iDriverId,bEnable)					(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->EnableCscanTof(bEnable):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEnableCscanTof(int iDriverId,bool *pbEnable);
#define OEMPA_GetEnableCscanTof(iDriverId,pbEnable)				(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetEnableCscanTof(pbEnable):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAscanBitSize(int iDriverId,enumBitSize &eBitSize);
#define OEMPA_SetAscanBitSize(iDriverId,eBitSize)					(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetAscanBitSize(eBitSize):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetAscanBitSize(int iDriverId,enumBitSize *peBitSize);
#define OEMPA_GetAscanBitSize(iDriverId,peBitSize)					(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetAscanBitSize(peBitSize):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetCycleCount(int iDriverId,int &iCycleCount);
#define OEMPA_SetCycleCount(iDriverId,iCycleCount)					(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetCycleCount(iCycleCount):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetCycleCount(int iDriverId,int *piCycleCount);
#define OEMPA_GetCycleCount(iDriverId,piCycleCount)					(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetCycleCount(piCycleCount):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetGainDigital(int iDriverId,int iCycle,double &dGain);
#define OEMPA_SetGainDigital(iDriverId,iCycle,dGain)				(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetGainDigital(iCycle,dGain):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetGainDigital(int iDriverId,int iCycle,double *pdGain);
#define OEMPA_GetGainDigital(iDriverId,iCycle,pdGain)				(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetGainDigital(iCycle,pdGain):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetBeamCorrection(int iDriverId,int iCycle,float &fGain);
#define OEMPA_SetBeamCorrection(iDriverId,iCycle,dGain)				(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetBeamCorrection(iCycle,dGain):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetBeamCorrection(int iDriverId,int iCycle,float *pfGain);
#define OEMPA_GetBeamCorrection(iDriverId,iCycle,pdGain)			(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetBeamCorrection(iCycle,pdGain):false)
//
//DRIVEROEMPA_API bool WINAPI OEMPA_SetDAC(int iDriverId,int iCycle,int &iCount,double *pdTime,float *pfSlope);
__inline bool OEMPA_SetDAC(int iDriverId,int iCycle,int &iCount,double *pdTime,float *pfSlope)
{
	return (CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetDACSlope(iCycle,iCount,pdTime,pfSlope):false);
}
//DRIVEROEMPA_API bool WINAPI OEMPA_SetDAC(int iDriverId,int iCycle,int &iCount,structCallbackArrayFloatDac &callbackArrayFloatDac);
#define OEMPA_SetDAC2(iDriverId,iCycle,iCountMax,callbackArrayFloatDac)		(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetDACSlope(iCycle,iCountMax,callbackArrayFloatDac):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetDAC(int iDriverId,int iCycle,int &iCountMax,int *piCount,double *pdTime,float *pfSlope);
__inline bool OEMPA_GetDAC(int iDriverId,int iCycle,int &iCountMax,int *piCount,double *pdTime,float *pfSlope)
{
	return (CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetDACSlope(iCycle,iCountMax,piCount,pdTime,pfSlope):false);
}
//DRIVEROEMPA_API bool WINAPI OEMPA_GetDAC(int iDriverId,int iCycle,int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac);
#define OEMPA_GetDAC2(iDriverId,iCycle,iCountMax,callbackArrayFloatDac)		(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetDACSlope(iCycle,iCountMax,callbackArrayFloatDac):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_EnableDAC(int iDriverId,int iCycle,bool &bEnable);
#define OEMPA_EnableDAC(iDriverId,iCycle,bEnable)						(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->EnableDAC(iCycle,bEnable):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEnableDAC(int iDriverId,int iCycle,bool *pbEnable);
#define OEMPA_GetEnableDAC(iDriverId,iCycle,pbEnable)					(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetEnableDAC(iCycle,pbEnable):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAscanRectification(int iDriverId,int iCycle,enumRectification &eRectification);
#define OEMPA_SetAscanRectification(iDriverId,iCycle,eRectification)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetAscanRectification(iCycle,eRectification):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetAscanRectification(int iDriverId,int iCycle,enumRectification *peRectification);
#define OEMPA_GetAscanRectification(iDriverId,iCycle,peRectification)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetAscanRectification(iCycle,peRectification):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAscanStart(int iDriverId,int iCycle,double &dAscanStart);
#define OEMPA_SetAscanStart(iDriverId,iCycle,dAscanStart)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetAscanStart(iCycle,dAscanStart):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetAscanStart(int iDriverId,int iCycle,double *pdAscanStart);
#define OEMPA_GetAscanStart(iDriverId,iCycle,pdAscanStart)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetAscanStart(iCycle,pdAscanStart):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAscanRangeWithFactor(int iDriverId,int iCycle,double &dRange,enumCompressionType &eCompressionType,long &lPointFactor);
#define OEMPA_SetAscanRangeWithFactor(iDriverId,iCycle,dRange,eCompressionType,lPointFactor)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetAscanRangeWithFactor(iCycle,dRange,eCompressionType,lPointFactor):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetAscanRangeWithFactor(int iDriverId,int iCycle,double *pdRange,enumCompressionType *peCompressionType,long *plPointFactor);
#define OEMPA_GetAscanRangeWithFactor(iDriverId,iCycle,pdRange,peCompressionType,plPointFactor)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetAscanRangeWithFactor(iCycle,pdRange,peCompressionType,plPointFactor):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAscanRangeWithCount(int iDriverId,int iCycle,double &dRange,enumCompressionType &eCompressionType,long &lPointCount);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetAscanRangeWithCount(int iDriverId,int iCycle,double *pdRange,enumCompressionType *peCompressionType,long *plPointCount);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetAscanPointCount(int iDriverId,int iCycle,long *pdAscanPointCount);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetTimeSlot(int iDriverId,int iCycle,double &dTime);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetTimeSlot(int iDriverId,int iCycle,double *pdTime);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAscanAcqIdChannelProbe(int iDriverId,int iCycle,WORD &wID);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetAscanAcqIdChannelProbe(int iDriverId,int iCycle,WORD *pwID);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAscanAcqIdChannelScan(int iDriverId,int iCycle,WORD &wID);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetAscanAcqIdChannelScan(int iDriverId,int iCycle,WORD *pwID);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAscanAcqIdChannelCycle(int iDriverId,int iCycle,WORD &wID);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetAscanAcqIdChannelCycle(int iDriverId,int iCycle,WORD *pwID);
//DRIVEROEMPA_API bool WINAPI OEMPA_EnableAscanMaximum(int iDriverId,int iCycle,bool &bEnable);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEnableAscanMaximum(int iDriverId,int iCycle,bool *pbEnable);
//DRIVEROEMPA_API bool WINAPI OEMPA_EnableAscanMinimum(int iDriverId,int iCycle,bool &bEnable);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEnableAscanMinimum(int iDriverId,int iCycle,bool *pbEnable);
//DRIVEROEMPA_API bool WINAPI OEMPA_EnableAscanSaturation(int iDriverId,int iCycle,bool &bEnable);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEnableAscanSaturation(int iDriverId,int iCycle,bool *pbEnable);
//
//DRIVEROEMPA_API bool WINAPI OEMPA_EnableGate(int iDriverId,bool &bEnable);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEnableGate(int iDriverId,bool *pbEnable);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetGateModeThreshold(int iDriverId,int iCycle,int iGate,bool &bEnable,enumGateModeAmp &eGateModeAmp,enumGateModeTof &eGateModeTof,enumRectification &eGateRectification,double &dThresholdPercent);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetGateModeThreshold(int iDriverId,int iCycle,int iGate,bool *pbEnable,enumGateModeAmp *peGateModeAmp,enumGateModeTof *peGateModeTof,enumRectification *peGateRectification,double *pdThresholdPercent);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetGateStart(int iDriverId,int iCycle,int iGate,double &dStart);
#define OEMPA_SetGateStart(iDriverId,iCycle,iGate,dStart)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetGateStart(iCycle,iGate,dStart):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetGateStart(int iDriverId,int iCycle,int iGate,double *pdStart);
#define OEMPA_GetGateStart(iDriverId,iCycle,iGate,pdStart)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetGateStart(iCycle,iGate,pdStart):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetGateStop(int iDriverId,int iCycle,int iGate,double &dStop);
#define OEMPA_SetGateStop(iDriverId,iCycle,iGate,dStop)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetGateStop(iCycle,iGate,dStop):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetGateStop(int iDriverId,int iCycle,int iGate,double *pdStop);
#define OEMPA_GetGateStop(iDriverId,iCycle,iGate,pdStop)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetGateStop(iCycle,iGate,pdStop):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetGateAcqIDAmp(int iDriverId,int iCycle,int iGate,WORD &wID);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetGateAcqIDAmp(int iDriverId,int iCycle,int iGate,WORD *pwID);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetGateAcqIDTof(int iDriverId,int iCycle,int iGate,WORD &wID);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetGateAcqIDTof(int iDriverId,int iCycle,int iGate,WORD *pwID);
//
//DRIVEROEMPA_API bool WINAPI OEMPA_SetHighVoltage(int iDriverId,int &iHighVoltage);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetHighVoltage(int iDriverId,int *piHighVoltage);
//
//DRIVEROEMPA_API bool WINAPI OEMPA_SetGateModeThreshold(int iDriverId,int iCycle,int iGate,bool &bEnable,enumGateModeAmp &eGateModeAmp,enumGateModeTof &eGateModeTof,enumRectification &eGateRectification,double &dThresholdPercent);
#define OEMPA_SetGateModeThreshold(iDriverId,iCycle,iGate,bEnable,eGateModeAmp,eGateModeTof,eGateRectification,dThresholdPercent)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetGateModeThreshold(iCycle,iGate,bEnable,eGateModeAmp,eGateModeTof,eGateRectification,dThresholdPercent):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetGateModeThreshold(int iDriverId,int iCycle,int iGate,bool *pbEnable,enumGateModeAmp *peGateModeAmp,enumGateModeTof *peGateModeTof,enumRectification *peGateRectification,double *pdThresholdPercent);
#define OEMPA_GetGateModeThreshold(iDriverId,iCycle,iGate,pbEnable,peGateModeAmp,peGateModeTof,peGateRectification,pdThresholdPercent)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetGateModeThreshold(iCycle,iGate,pbEnable,peGateModeAmp,peGateModeTof,peGateRectification,pdThresholdPercent):false)
//
//DRIVEROEMPA_API bool WINAPI OEMPA_SetGainAnalog(int iDriverId,int iCycle,float &fGain);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetGainAnalog(int iDriverId,int iCycle,float *pfGain);
//
//DRIVEROEMPA_API bool WINAPI OEMPA_SetEmissionWedgeDelay(int iDriverId,int iCycle,int iCycleCount,double &dWedgeDelay);//to set one wedge delay.
#define OEMPA_SetEmissionWedgeDelay(iDriverId,iCycle,iCycleCount,dWedgeDelay)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetEmissionWedgeDelay(iCycle,iCycleCount,dWedgeDelay):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEmissionWedgeDelay(int iDriverId,int iCycle,int iCountMax,double *pdWedgeDelay);//you have to call this function many times with same value for "iCountMax".
#define OEMPA_GetEmissionWedgeDelay(iDriverId,iCycle,iCycleCount,pdWedgeDelay)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetEmissionWedgeDelay(iCycle,iCycleCount,pdWedgeDelay):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetReceptionWedgeDelay(int iDriverId,int iCycle,int iCycleCount,double &dWedgeDelay);
#define OEMPA_SetReceptionWedgeDelay(iDriverId,iCycle,iCycleCount,dWedgeDelay)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetReceptionWedgeDelay(iCycle,iCycleCount,dWedgeDelay):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetReceptionWedgeDelay(int iDriverId,int iCycle,int iCountMax,double *pdWedgeDelay);//you have to call this function many times with same value for "iCountMax".
#define OEMPA_GetReceptionWedgeDelay(iDriverId,iCycle,iCycleCount,dWedgeDelay)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetReceptionWedgeDelay(iCycle,iCycleCount,dWedgeDelay):false)
//
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAllElementEnable(int iDriverId,bool bEnable,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
#define OEMPA_SetAllElementEnable(iDriverId,bEnable,adwHWAperture)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetAllElementEnable(bEnable,adwHWAperture):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetElementEnable(int iDriverId,int iElement,bool bEnable,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
#define OEMPA_SetElementEnable(iDriverId,iElement,bEnable,adwHWAperture)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetElementEnable(iElement,bEnable,adwHWAperture):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetElementEnable(int iDriverId,int iElement,const DWORD adwHWAperture[g_iOEMPAApertureDWordCount],bool &bEnable);
#define OEMPA_GetElementEnable(iDriverId,iElement,adwHWAperture,bEnable)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetElementEnable(iElement,adwHWAperture,bEnable):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_IsMultiplexer(int iDriverId);//does the HW include a multiplexer 16:128 or 32:128 ?
#define OEMPA_IsMultiplexer(iDriverId)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->IsMultiplexer():false)
//
//DRIVEROEMPA_API bool WINAPI OEMPA_SetMultiplexerEnable(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
#define OEMPA_SetMultiplexerEnable(iDriverId,iCycle,adwHWAperture)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetMultiplexerEnable(iCycle,adwHWAperture):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetMultiplexerEnable(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
#define OEMPA_GetMultiplexerEnable(iDriverId,iCycle,adwHWAperture)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetMultiplexerEnable(iCycle,adwHWAperture):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetEmissionEnable(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
#define OEMPA_SetEmissionEnable(iDriverId,iCycle,adwHWAperture)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetEmissionEnable(iCycle,adwHWAperture):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEmissionEnable(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
#define OEMPA_GetEmissionEnable(iDriverId,iCycle,adwHWAperture)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetEmissionEnable(iCycle,adwHWAperture):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetReceptionEnable(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
#define OEMPA_SetReceptionEnable(iDriverId,iCycle,adwHWAperture)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetReceptionEnable(iCycle,adwHWAperture):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetReceptionEnable(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount]);
#define OEMPA_GetReceptionEnable(iDriverId,iCycle,adwHWAperture)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetReceptionEnable(iCycle,adwHWAperture):false)
//
//DRIVEROEMPA_API bool WINAPI OEMPA_SetEmissionDelays(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfDelay);
#define OEMPA_SetEmissionDelays(iDriverId,iCycle,adwHWAperture,pfDelay)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetEmissionDelays(iCycle,adwHWAperture,pfDelay):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEmissionDelays(int iDriverId,int iCycle,int &iElementCountMax,int *piElementCount,float *pfDelay);
__inline bool OEMPA_GetEmissionDelays(int iDriverId,int iCycle,int &iElementCountMax,int *piElementCount,float *pfDelay)
{
	return (CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetEmissionDelays(iCycle,iElementCountMax,piElementCount,pfDelay):false);
}
//DRIVEROEMPA_API bool WINAPI OEMPA_SetEmissionDelays(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat1D &callbackArrayFloat1D);
#define OEMPA_SetEmissionDelays2(iDriverId,iCycle,adwHWAperture,callbackArrayFloat1D)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetEmissionDelays(iCycle,adwHWAperture,callbackArrayFloat1D):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEmissionDelays(int iDriverId,int iCycle,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D);//Size = element size.
#define OEMPA_GetEmissionDelays2(iDriverId,iCycle,iElementCountMax,callbackArrayFloat1D)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetEmissionDelays(iCycle,iElementCountMax,callbackArrayFloat1D):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetReceptionDelays(int iDriverId,int iCycle,int iLineSize,int &iFocalCount,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfDelay);
__inline bool OEMPA_SetReceptionDelays(int iDriverId,int iCycle,int iLineSize,int &iFocalCount,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfDelay)
{
	return (CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetReceptionDelays(iCycle,iLineSize,iFocalCount,adwHWAperture,pfDelay):false);
}
//DRIVEROEMPA_API bool WINAPI OEMPA_GetReceptionDelays(int iDriverId,int iCycle,int iLineSize,int &iElementCountMax,int &iFocalCountMax,int *piElementCount,int *piFocalCount,float *pfDelay);
__inline bool OEMPA_GetReceptionDelays(int iDriverId,int iCycle,int iLineSize,int &iElementCountMax,int &iFocalCountMax,int *piElementCount,int *piFocalCount,float *pfDelay)
{
	return (CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetReceptionDelays(iCycle,iLineSize,iElementCountMax,iFocalCountMax,piElementCount,piFocalCount,pfDelay):false);
}
//DRIVEROEMPA_API bool WINAPI OEMPA_SetReceptionDelays(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat2D &callbackArrayFloat2D);//Size1 = element size, Size2 = DDF size.
#define OEMPA_SetReceptionDelays(iDriverId,iCycle,adwHWAperture,callbackArrayFloat1D)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetReceptionDelays(iCycle,adwHWAperture,callbackArrayFloat1D):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetReceptionDelays(int iDriverId,int iCycle,int &iElementCountMax,int &iFocalCountMax,structCallbackArrayFloat2D &callbackArrayFloat2D);//Size1 = element size, Size2 = DDF size.
#define OEMPA_GetReceptionDelays(iDriverId,iCycle,iElementCountMax,iFocalCountMax,callbackArrayFloat1D)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetReceptionDelays(iCycle,iElementCountMax,iFocalCountMax,callbackArrayFloat1D):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetReceptionFocusing(int iDriverId,int iCycle,int &iFocalCount,double *pdFocalTof);
#define OEMPA_SetReceptionFocusing(iDriverId,iCycle,iFocalCount,pdFocalTof)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetReceptionFocusing(iCycle,iFocalCount,pdFocalTof):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetReceptionFocusing(int iDriverId,int iCycle,int &iFocalCountMax,int *piFocalCount,double *pdFocalTof);
#define OEMPA_GetReceptionFocusing(iDriverId,iCycle,iFocalCountMax,piFocalCount,pdFocalTof)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetReceptionFocusing(iCycle,iFocalCountMax,piFocalCount,pdFocalTof):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_EnableDDF(int iDriverId,int iCycle,bool &bEnable);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEnableDDF(int iDriverId,int iCycle,bool *pbEnable);
//DRIVEROEMPA_API bool WINAPI OEMPA_EnableDDF(int iDriverId,int iCycle,enumFocusing &eFocusing);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEnableDDF(int iDriverId,int iCycle,enumFocusing *peFocusing);
//
//DRIVEROEMPA_API bool WINAPI OEMPA_SetEmissionWidth(int iDriverId,int iCycle,int iElementIndex,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float &fWidth);
#define OEMPA_SetEmissionWidth(iDriverId,iCycle,iElementIndex,adwHWAperture,fWidth)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetEmissionWidth(iCycle,iElementIndex,adwHWAperture,fWidth):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetEmissionWidths(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfWidth);
#define OEMPA_SetEmissionWidths(iDriverId,iCycle,adwHWAperture,pfWidth)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetEmissionWidths(iCycle,adwHWAperture,pfWidth):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEmissionWidths(int iDriverId,int iCycle,int &iElementCountMax,int *piElementCount,float *pfWidth);
#define OEMPA_GetEmissionWidths(iDriverId,iCycle,iElementCountMax,piElementCount,pfWidth)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetEmissionWidths(iCycle,iElementCountMax,piElementCount,pfWidth):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetEmissionWidths(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat1D &callbackArrayFloat1D);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetEmissionWidths(int iDriverId,int iCycle,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D);//Size = element size.
//
//DRIVEROEMPA_API bool WINAPI OEMPA_SetReceptionGain(int iDriverId,int iCycle,int iElementIndex,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float &fGain);
//DRIVEROEMPA_API bool WINAPI OEMPA_SetReceptionGains(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],float *pfGain);
#define OEMPA_SetReceptionGains(iDriverId,iCycle,adwHWAperture,pfGain)	(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->SetReceptionGains(iCycle,adwHWAperture,pfGain):false)
//DRIVEROEMPA_API bool WINAPI OEMPA_GetReceptionGains(int iDriverId,int iCycle,int &iElementCountMax,int *piElementCount,float *pfGain);
__inline bool OEMPA_GetReceptionGains(int iDriverId,int iCycle,int &iElementCountMax,int *piElementCount,float *pfGain)
{
	return (CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetReceptionGains(iCycle,iElementCountMax,piElementCount,pfGain):false);
}
//DRIVEROEMPA_API bool WINAPI OEMPA_SetReceptionGains(int iDriverId,int iCycle,DWORD adwHWAperture[g_iOEMPAApertureDWordCount],structCallbackArrayFloat1D &callbackArrayFloat1D);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetReceptionGains(int iDriverId,int iCycle,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D);//Size = element size.

//<<PARAMETERS MANAGEMENT FUNCTIONS : END>>
//very low level hardware access (normally you don't need to use those functions)
//DRIVEROEMPA_API bool WINAPI OEMPA_SetAcquisitionFlush(int iDriverId,enumAcquisitionFlush eAcquisitionFlush);
//DRIVEROEMPA_API enumAcquisitionFlush WINAPI OEMPA_GetAcquisitionFlush(int iDriverId);
//DRIVEROEMPA_API bool WINAPI OEMPA_WriteHW(int iDriverId,DWORD dwAddress,DWORD dwData,int iSize);
//DRIVEROEMPA_API bool WINAPI OEMPA_RequestSW(int iDriverId,DWORD dwAddress,void *pSWData,bool bPtrList,pCallbackRequestTranslation pFunc,int iSize);
//DRIVEROEMPA_API bool WINAPI OEMPA_ReadHW(int iDriverId,DWORD dwAddress,void *pSWData,pCallbackRequestTranslation pFunc,int iSize);
//DRIVEROEMPA_API bool WINAPI OEMPA_ReadSW(int iDriverId,DWORD dwAddress,DWORD &dwData,int iSize);
//DRIVEROEMPA_API bool WINAPI OEMPA_ReadSW(int iDriverId,DWORD dwAddress,void *pSWData,bool bPtrListDesalloc,pCallbackRequestTranslation pFunc,int iSize);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetFirstWriteHW(int iDriverId,bool bClear,DWORD &dwAddress,DWORD &dwData,int &iSize,CDrvPosition &drvPos);
//DRIVEROEMPA_API bool WINAPI OEMPA_GetNextWriteHW(int iDriverId,bool bClear,DWORD &dwAddress,DWORD &dwData,int &iSize,CDrvPosition &drvPos);

