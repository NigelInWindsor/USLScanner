#pragma once
//HERE IS MACRO AND INLINE FUNCTION TO SHOW YOU HOW TO GET COMPATIBILITY WITH OLDER VERSION
//ALL MACROS HAVE NOT BEEN IMPLEMENTED BUT THERE IS A LOT OF EXAMPLES, SO YOU CAN IMPLEMENT NEW ONE BY YOURSELF
//But it is better to not use them directly and to rewrite your code, for that you can get inspiration from those macros.

#define CUTKernelDriverOEMPA			CHWDeviceOEMPA
#define CUTKernelDriver					CHWDevice
#define CUTDevice						CSWDevice
#define CUTDeviceOEMPA					CSWDeviceOEMPA

#define OEMPA_SetCallbackReadFileWriteHW	OEMPA_SetCallbackCustomizedAPI
#define pCallbackReadFileWriteHW			pCallbackCustomizedDriverAPI
#define OEMPA_WriteFileExample				OEMPA_ReadWizardWriteFile
#define OEMPA_WriteWizardExample			OEMPA_ReadWizardWriteFile

#define g_iOEMPABoardTemperatureSensorCountMax	g_iOEMPARXBoardTemperatureSensorCountMax

#define swIsUnlockDefaultEnableShot		GetSWDevice()->GetUnlockDefaultEnablePulser
#define swSetUnlockDefaultEnableShot	GetSWDevice()->SetUnlockDefaultEnablePulser
#define IsShotEnabled					IsPulserEnabled

#define OEMPA_GetBoardCount(iDriverId)				(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetSWDeviceOEMPA()->GetRXBoardCount():false)
#define OEMPA_GetTemperatureSensor(iDriverId,iIndexBoard,iIndexSensor,wTemperature)		(CHWDevice::GetHWDevice(iDriverId) && dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId)) ? dynamic_cast<CHWDeviceOEMPA*>(CHWDevice::GetHWDevice(iDriverId))->GetTemperatureSensor(iIndexBoard,iIndexSensor,wTemperature):false)
__inline bool OEMPA_GetUSB(int iDriverId,wchar_t *pValue,int iSize)
{
	return false;
}

#define eTofZrA							eTofZeroFirstAfterThresholdCross

#include "CompatibilityFunctionAPI_1_0_5_X.h"
