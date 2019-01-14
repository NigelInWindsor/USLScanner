
#pragma once

#if !defined(_CUST_WIZ_DEF_) && !defined(_CUST_WIZ_DECL_)
#define _CUST_WIZ_DEF_	__declspec(dllimport)
#define _CUST_WIZ_DECL_	__declspec(dllimport)
#pragma comment(lib,"UTCustomizedWizardAPI.lib")
#if (defined(__AFX_H__) || defined(__AFXWIN_H__))
	//MFC are used, no need to use classes defined in "UTCom.lib".
	#if !defined(myCFile)
		#define myCFile			CFile
	#endif
#else
	//MFC are not used, the classes defined in "UTCom.lib" are required.
	#include "myMFC.h"
	#pragma comment(lib,"UTCom.lib")
#endif
#endif

class CUTWizardSystem;
class CUTChannels;

//SINGLE-CHANNEL AND MULTI-CHANNEL
_CUST_WIZ_DECL_ bool WINAPI OEMPA_ReadWizardWriteFile(CHWDeviceOEMPA *pHWDeviceOEMPA,
						CUTWizardSystem *pSystem,CUTChannels *pChannels,const wchar_t *pFileNameIn,const wchar_t *pFileNameOut);//single-channel and multi-channel
_CUST_WIZ_DECL_ bool WINAPI OEMPA_ReadWizardWriteFile(CHWDeviceOEMPA *pHWDeviceOEMPA,
						CUTWizardSystem *pSystem,CUTChannels *pChannels,const wchar_t *pFileName);//single-channel and multi-channel
_CUST_WIZ_DECL_ bool WINAPI OEMPA_ReadWizardWriteFile(CHWDeviceOEMPA *pHWDeviceOEMPA,
						int iWizardSystemId,int iWizardChannelId,const wchar_t *pFileName);//single-channel and multi-channel
_CUST_WIZ_DECL_ bool WINAPI OEMPA_ReadWizard(CHWDeviceOEMPA *pHWDeviceOEMPA,CUTWizardSystem *pSystem,CUTChannels *pChannels,
						structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception);//single-channel and multi-channel.
_CUST_WIZ_DECL_ bool WINAPI OEMPA_ReadWizard(int iWizardSystemId,int iWizardChannelId,
						structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception);

//SINGLE-CHANNEL ONLY
_CUST_WIZ_DECL_ bool WINAPI OEMPA_ReadWizardWriteFile(	CHWDeviceOEMPA *pHWDeviceOEMPA,const wchar_t *pFileName,int iCycleCount,double dTimeSlot,int iSameAllCycles,
						int iEmissionElementCount,double dEmissionWedgeDelay,float fPulseWidth,
						int iReceptionElementCount,int iReceptionFocalCount,double dReceptionWedgeDelay,
						double dAscanStart,double dAscanRange,int iDACCount,
						int iGateCount,double dGateStart,double dGateStop,double dGateThreshold);//to download all HW parameters and save them in a file. Mono-channel only.
_CUST_WIZ_DECL_ bool WINAPI OEMPA_ReadWizard(	CUTWizardSystem *pSystem,CUTChannels *pChannels,//single-channel only
					bool &bLongitudinalWave,double &dSpecimenVelocity,
					int &iProbeElementCount,double &dProbePitch,double &dProbeFrequency,
					bool &bWedgeEnable,
					double &dWedgeHeight,double &dWedgeAngle,double &dWedgeVelocity,
					double &dScanDepthEmission,int iScanDepthReceptionMax,double *pScanDepthReception,int &iScanDepthReceptionCount,
					int &iScanElementCount,
					int &iScanElementStart,int &iScanElementStop,int &iScanElementStep,
					double &dScanAngleStart,double &dScanAngleStop,double &dScanAngleStep,
					double &dAscanStart,double &dAscanRange,double &dTimeSlot,
					int iCscanMax,double *pdCscanStart,double *pdCscanRange,double *pdCscanThreshold,
					enumRectification *peRectification,enumGateModeAmp *peModeAmp,enumGateModeTof *peModeTof,int &iCscanCount,
					enumDepthMode &eDepthMode
					);//single-channel only
_CUST_WIZ_DECL_ bool WINAPI OEMPA_WriteWizard(	CUTWizardSystem *pSystem,CUTChannels *pChannels,//single-channel only
					bool bLongitudinalWave,double dSpecimenVelocity,
					int iProbeElementCount,double dProbePitch,double dProbeFrequency,
					bool bWedgeEnable,
					double dWedgeHeight,double dWedgeAngle,double dWedgeVelocity,
					double dScanDepthEmission,double *pScanDepthReception,int iScanDepthReceptionCount,
					int iScanElementCount,
					bool bLinear,int iScanElementStart,int iScanElementStop,int iScanElementStep,
					bool bSectorial,double dScanAngleStart,double dScanAngleStop,double dScanAngleStep,
					double dAscanStart,double dAscanRange,double dTimeSlot,
					int iCscanCount,double *pdCscanStart,double *pdCscanRange,double *pdCscanThreshold,
					enumRectification *peRectification,enumGateModeAmp *peModeAmp,enumGateModeTof *peModeTof,
					enumDepthMode eDepthMode
					);//single-channel only

_CUST_WIZ_DECL_ bool WINAPI OEMPA_ReadWizardMultipleChannel(	int iProbeIndex,int iScanIndex,
					CUTWizardSystem *pSystem,CUTChannels *pChannels,
					bool &bLongitudinalWave,double &dSpecimenVelocity,
					int &iProbeElementCount,double &dProbePitch,double &dProbeFrequency,
					bool &bWedgeEnable,
					double &dWedgeHeight,double &dWedgeAngle,double &dWedgeVelocity,
					double &dScanDepthEmission,int iScanDepthReceptionMax,double *pScanDepthReception,int &iScanDepthReceptionCount,
					int &iScanElementCount,
					int &iScanElementStart,int &iScanElementStop,int &iScanElementStep,
					double &dScanAngleStart,double &dScanAngleStop,double &dScanAngleStep,
					double &dAscanStart,double &dAscanRange,double &dTimeSlot,
					int iCscanMax,double *pdCscanStart,double *pdCscanRange,double *pdCscanThreshold,
					enumRectification *peRectification,enumGateModeAmp *peModeAmp,enumGateModeTof *peModeTof,int &iCscanCount,
					enumDepthMode &eDepthMode
					);
_CUST_WIZ_DECL_ bool WINAPI OEMPA_WriteWizardMultipleChannel(	int iProbeIndex,int iScanIndex,
					CUTWizardSystem *pSystem,CUTChannels *pChannels,
					bool bLongitudinalWave,double dSpecimenVelocity,
					int iProbeElementCount,double dProbePitch,double dProbeFrequency,
					bool bWedgeEnable,
					double dWedgeHeight,double dWedgeAngle,double dWedgeVelocity,
					double dScanDepthEmission,double *pScanDepthReception,int iScanDepthReceptionCount,
					int iScanElementCount,
					bool bLinear,int iScanElementStart,int iScanElementStop,int iScanElementStep,
					bool bSectorial,double dScanAngleStart,double dScanAngleStop,double dScanAngleStep,
					double dAscanStart,double dAscanRange,double dTimeSlot,
					int iCscanCount,double *pdCscanStart,double *pdCscanRange,double *pdCscanThreshold,
					enumRectification *peRectification,enumGateModeAmp *peModeAmp,enumGateModeTof *peModeTof,
					enumDepthMode eDepthMode
					);

//This callback function is used for many purposes:
//It is called from "OEMPA_ReadWizard", please see each function to understand each step.
typedef enum enumStepCustomizedWizardAPI
{
	eLoadWizard_Enter,
	eLoadWizard_Leave
};
typedef structCorrectionOEMPA* (WINAPI* pCallbackCustomizedWizardAPI)(CHWDeviceOEMPA *pOEMPA,const wchar_t *pFileName,enumStepCustomizedWizardAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
_CUST_WIZ_DECL_ bool WINAPI OEMPA_SetCallbackCustomizedWizardAPI(pCallbackCustomizedWizardAPI pFunc);
_CUST_WIZ_DECL_ pCallbackCustomizedWizardAPI WINAPI OEMPA_GetCallbackCustomizedWizardAPI();
