
#pragma once
#if !defined(_CUST_DRV_DEF_) && !defined(_CUST_DRV_DECL_)
#define _CUST_DRV_DEF_	__declspec(dllimport)
#define _CUST_DRV_DECL_	__declspec(dllimport)
#pragma comment(lib,"UTCustomizedDriverAPI.lib")
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

#include "CustomizedKernelAPI.h"

/////////////////////////////////////////////////////////////////////
//those function can be used to exchange structures data with HW.
_CUST_DRV_DECL_ bool WINAPI OEMPA_ReadHWWriteFileCycle(CHWDeviceOEMPA *pOEMPA,int iCycleIndex,int iDACCountMax,structRoot *pRoot,structCycle *pCycle);
_CUST_DRV_DECL_ bool WINAPI OEMPA_WriteHW(CHWDeviceOEMPA *pOEMPA,structRoot &root,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception,enumAcquisitionState eAcq=eAcqDefault);
_CUST_DRV_DECL_ bool WINAPI OEMPA_ReadHW(CHWDeviceOEMPA *pOEMPA,int iCycleCountMax,int iElementCountMaxIn,int iFocalCountMax,int iDACCountMax,
							structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception);
_CUST_DRV_DECL_ bool OEMPA_WriteCorrectedFileText(const wchar_t *pFileCorrected,structRoot *pRoot,structCorrectionOEMPA &beam);

/////////////////////////////////////////////////////////////////////
//those function can be used to exchange structures data with OEMPA file.
//see example to read in function "OEMPA_ReadFileWriteHW" of OEMPATool application sofwtare.
_CUST_DRV_DECL_ bool WINAPI OEMPA_ReadFileText(	const wchar_t *pFileName,
							structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception);
_CUST_DRV_DECL_ bool WINAPI OEMPA_ReadFileBinary(const wchar_t *pFileName,
							structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception);
_CUST_DRV_DECL_ bool WINAPI OEMPA_WriteFileText(	const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
_CUST_DRV_DECL_ bool WINAPI OEMPA_WriteFileBinary(	const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);

_CUST_DRV_DECL_ bool WINAPI OEMPA_ConvertFileFormat(CHWDeviceOEMPA *pHWDeviceOEMPA,const wchar_t *pFileName,const wchar_t *pFileNew);//bin file to txt file OR txt file to bin file.
_CUST_DRV_DECL_ bool WriteCorrectedFileText(	const wchar_t *pFileCorrected,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
_CUST_DRV_DECL_ bool IsValidFileExtension(const wchar_t *pFileName,bool &bTextFile);
/////////////////////////////////////////////////////////////////////

class CHWDeviceOEMPA;

_CUST_DRV_DECL_ bool WINAPI OEMPA_ReadFileWriteHW(CHWDeviceOEMPA *pOEMPA,const wchar_t *pFileName,enumAcquisitionState eAcqState=eAcqDefault);//to upload all parameters from a file to the HW, see also "OEMPA_SetCallbackCustomizedDriverAPI".
_CUST_DRV_DECL_ bool WINAPI OEMPA_ReadHWWriteFile(CHWDeviceOEMPA *pOEMPA,int iCycleCountMax,int iElementCountMax,int iFocalCountMax,int iDACCountMax,wchar_t *pFileName);//to download all HW parameters and save them in a file.
_CUST_DRV_DECL_ bool WINAPI OEMPA_Free(structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception);
_CUST_DRV_DECL_ bool WINAPI OEMPA_New(structCorrectionOEMPA &correction);
_CUST_DRV_DECL_ void WINAPI OEMPA_Free(structCorrectionOEMPA &correction);
_CUST_DRV_DECL_ bool WINAPI OEMPA_ReadFileWriteHW256(CHWDeviceOEMPA *pHWMaster, CHWDeviceOEMPA *pHWSlave, wchar_t *pFileNameMaster, wchar_t *pFileNameSlave);

typedef bool (WINAPI* pCallbackSettingId)(DWORD &dwSettingId);
_CUST_DRV_DECL_ void WINAPI OEMPA_SetCallbackSettingId(pCallbackSettingId pFunc);
_CUST_DRV_DECL_ pCallbackSettingId WINAPI OEMPA_GetCallbackSettingId();

_CUST_DRV_DECL_ void NotifyErrorFocalLaw(int iCycle);

//This callback function is used for many purposes:
//- to get correction pointer that should be filled from loading OEMPA file.
//- to check setting read from the file before sending it to the hardware.
//- to notify the end of loading OEMPA file.
//- to change or add items just before saving OEMPA file.
//It is called from "OEMPA_ReadFileWriteHW" please see each function to understand each step.
typedef enum enumStepCustomizedAPI
{
    eReadFileWriteHW_Enter,		//Just after calling  ReadFileWriteHW but
								//before loading the OEMPA file, to get the correction pointer.
    eWriteHW_Enter,				//Just before sending the configuration data to the device
								//so it is possible to read/change it.
    eWriteHW_Leave,				//Just after sending the configuration data to the device
								//so it is possible to see if the driver have updated any of the parameters.
    eReadFileWriteHW_Leave,		//Just before returning from ReadFileWriteHW
								//after the correction has been updated.
    eWriteFile_Enter,			//Just before saving any OEMPA file
								//so it is possible to add any special option that is required.
								//For example the 1/n AscanRequest.
    eWriteFile_Leave,			//Just after the OEMPA file has been saved.
    eReadHW_Enter,				//Just before reading the configuration data from the device
    eReadHW_Leave				//Just after reading the configuration data from the device
};
typedef structCorrectionOEMPA* (WINAPI* pCallbackCustomizedDriverAPI)(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
_CUST_DRV_DECL_ bool WINAPI OEMPA_SetCallbackCustomizedDriverAPI(pCallbackCustomizedDriverAPI pFunc);
_CUST_DRV_DECL_ pCallbackCustomizedDriverAPI WINAPI OEMPA_GetCallbackCustomizedDriverAPI();
