// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

extern "C"{
__declspec(dllexport) VS_FIXEDFILEINFO g_VS_FIXEDFILEINFO={	0xFEEF04BD,//DWORD dwSignature;
												0,//DWORD dwStrucVersion;
												0x00010001,//DWORD dwFileVersionMS;
												0x00050004,//DWORD dwFileVersionLS;
												0x00010001,//DWORD dwProductVersionMS;
												0x00050004,//DWORD dwProductVersionLS;
												VS_FF_DEBUG | VS_FF_PRERELEASE | VS_FF_PATCHED | VS_FF_PRIVATEBUILD | VS_FF_INFOINFERRED | VS_FF_SPECIALBUILD,//DWORD dwFileFlagsMask;
#ifdef _DEBUG
												VS_FF_DEBUG,//DWORD dwFileFlags;
#else _DEBUG
#ifdef _PRERELEASE
												VS_FF_PRERELEASE,//DWORD dwFileFlags;
#else _PRERELEASE
												0,//DWORD dwFileFlags;
#endif _PRERELEASE
#endif _DEBUG
												VOS__WINDOWS32,//DWORD dwFileOS;
												VFT_DLL,//DWORD dwFileType;
												0,//DWORD dwFileSubtype;
												0,//DWORD dwFileDateMS;
												0//DWORD dwFileDateLS;
											};
}

pCallbackCustomizedDriverAPI g_pCallbackCustomizedDriverAPI=NULL;
_CUST_DRV_DEF_ bool WINAPI OEMPA_SetCallbackCustomizedDriverAPI(pCallbackCustomizedDriverAPI pFunc)
{
	g_pCallbackCustomizedDriverAPI = pFunc;
	return true;
}
_CUST_DRV_DEF_ pCallbackCustomizedDriverAPI WINAPI OEMPA_GetCallbackCustomizedDriverAPI()
{
	return g_pCallbackCustomizedDriverAPI;
}

bool WINAPI OEMPA_CreateDefaultSetupFile(CHWDeviceOEMPA *pHWDeviceOEMPA,bool bOEMPA,wchar_t *wcLoadDefaultSetup);
bool WINAPI OEMPA_CreateDefaultSetupFilePA(CHWDeviceOEMPA *pHWDeviceOEMPA,wchar_t *wcLoadDefaultSetup);//PhasedArray
bool WINAPI OEMPA_CreateDefaultSetupFileMC(CHWDeviceOEMPA *pHWDeviceOEMPA,wchar_t *wcLoadDefaultSetup);//MultiChannel
bool WINAPI OEMPA_CreateDefaultSetupFilePA256(CHWDeviceOEMPA *pHWDeviceOEMPA,wchar_t *wcLoadDefaultSetup);

bool WINAPI OEMPA_CreateDefaultSetupFile(CHWDeviceOEMPA *pHWDeviceOEMPA,bool bOEMPA,wchar_t *wcLoadDefaultSetup)
{//ToDo impossible now to create le setup from the driver
	enumHardwareLink eHWLink;

	if(bOEMPA)
	{
		eHWLink = pHWDeviceOEMPA->GetMatched256DeviceHwLink();
		if(eHWLink!=eUnlinked)
			return OEMPA_CreateDefaultSetupFilePA256(pHWDeviceOEMPA,wcLoadDefaultSetup);
		else
			return OEMPA_CreateDefaultSetupFilePA(pHWDeviceOEMPA,wcLoadDefaultSetup);
	}else
		return OEMPA_CreateDefaultSetupFileMC(pHWDeviceOEMPA,wcLoadDefaultSetup);
}
bool WINAPI OEMPA_CreateDefaultSetupFilePA(CHWDeviceOEMPA *pHWDeviceOEMPA,wchar_t *wcLoadDefaultSetup)
{
	DWORD dwAttributes;
	structRoot root;
	structCycle cycle;
	CFocalLaw focalLaw;

	OEMPA_InitRoot(root,pHWDeviceOEMPA);
	if(!wcLoadDefaultSetup || !wcslen(wcLoadDefaultSetup))
		return true;//no file
	dwAttributes = GetFileAttributes(wcLoadDefaultSetup);
	if(dwAttributes!=INVALID_FILE_ATTRIBUTES)
		return true;

	OEMPA_ResetStructCycle(&cycle);
	OEMPA_ResetStructFocalLaw(&focalLaw);
	if(!focalLaw.GetDWORDElementSize())
		focalLaw.SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
	OEMPA_ResetStructFocalLaw(&focalLaw);

	cycle.dRange = 30.0e-6;
	cycle.bMaximum = true;
	cycle.dTimeSlot = 2000.0e-6;
	cycle.fGainAnalog = 20.0f;
	
	focalLaw.iElementCount = 1;
	focalLaw.iFocalCount = 1;
	focalLaw.adwElement[0] = 1;
	focalLaw.iDelayCount = focalLaw.iFocalCount*focalLaw.iElementCount;
	focalLaw.afPrm[0] = 0.5e-6f;

	root.iCycleCount = 1;
	return OEMPA_WriteFileText(wcLoadDefaultSetup,&root,&cycle,&focalLaw,&focalLaw);
}
bool WINAPI OEMPA_CreateDefaultSetupFileMC(CHWDeviceOEMPA *pHWDeviceOEMPA,wchar_t *wcLoadDefaultSetup)
{
	DWORD dwAttributes;
	structRoot root;
	structCycle cycle;
	CFocalLaw focalLawE;
	CFocalLaw focalLawR;

	OEMPA_InitRoot(root,pHWDeviceOEMPA);
	root.bEnableMultiHWChannel = true;//because Options has not been initialized yet.
	if(!wcLoadDefaultSetup || !wcslen(wcLoadDefaultSetup))
		return true;//no file
	dwAttributes = GetFileAttributes(wcLoadDefaultSetup);
	if(dwAttributes!=INVALID_FILE_ATTRIBUTES)
		return true;

	OEMPA_ResetStructCycle(&cycle);
	OEMPA_ResetStructFocalLaw(&focalLawE);
	OEMPA_ResetStructFocalLaw(&focalLawR);
	if(!focalLawE.GetDWORDElementSize())
		focalLawE.SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
	OEMPA_ResetStructFocalLaw(&focalLawE);
	if(!focalLawR.GetDWORDElementSize())
		focalLawR.SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
	OEMPA_ResetStructFocalLaw(&focalLawR);

	cycle.dRange = 30.0e-6;
	cycle.bMaximum = true;
	cycle.dTimeSlot = 2000.0e-6;
	cycle.fGainAnalog = 20.0f;
	cycle.bHWAcquisition = true;
	
	focalLawE.iElementCount = 1;
	focalLawE.iFocalCount = 1;
	focalLawE.adwElement[0] = 1;
	focalLawE.iDelayCount = focalLawE.iElementCount;
	focalLawE.afPrm[0] = 0.5e-6f;

	focalLawR.iElementCount = 1;
	focalLawR.iFocalCount = 1;
	focalLawR.adwElement[0] = 1;
	focalLawR.iDelayCount = focalLawR.iElementCount;
	focalLawR.afPrm[0] = 0.5e-6f;

	root.iCycleCount = 1;
	return OEMPA_WriteFileText(wcLoadDefaultSetup,&root,&cycle,&focalLawE,&focalLawR);
}

bool WINAPI OEMPA_CreateDefaultSetupFilePA256(CHWDeviceOEMPA *pHWDeviceOEMPA,wchar_t *wcLoadDefaultSetup)
{
	DWORD dwAttributes128A,dwAttributes128B;
	structRoot root;
	structCycle cycle;
	CFocalLaw focalLaw;
	bool bRet=true;
	wchar_t drive[_MAX_DRIVE],dir[_MAX_DIR],fname[MAX_PATH],ext[_MAX_EXT];
	wchar_t wcLoadDefaultSetup128A[MAX_PATH],wcLoadDefaultSetup128B[MAX_PATH];

	OEMPA_InitRoot(root,pHWDeviceOEMPA);
	if(!wcLoadDefaultSetup || !wcslen(wcLoadDefaultSetup))
		return true;//no file
	_wsplitpath_s(wcLoadDefaultSetup,drive,_MAX_DRIVE,dir,_MAX_DIR,fname,MAX_PATH,ext,_MAX_EXT);

	wcscpy_s(wcLoadDefaultSetup128A,MAX_PATH,drive);
	wcscat_s(wcLoadDefaultSetup128A,MAX_PATH,dir);
	wcscat_s(wcLoadDefaultSetup128A,MAX_PATH,fname);
	wcscat_s(wcLoadDefaultSetup128A,MAX_PATH,L"128A");
	wcscat_s(wcLoadDefaultSetup128A,MAX_PATH,ext);
	wcscpy_s(wcLoadDefaultSetup128B,MAX_PATH,drive);
	wcscat_s(wcLoadDefaultSetup128B,MAX_PATH,dir);
	wcscat_s(wcLoadDefaultSetup128B,MAX_PATH,fname);
	wcscat_s(wcLoadDefaultSetup128B,MAX_PATH,L"128B");
	wcscat_s(wcLoadDefaultSetup128B,MAX_PATH,ext);
	dwAttributes128A = GetFileAttributes(wcLoadDefaultSetup128A);
	dwAttributes128B = GetFileAttributes(wcLoadDefaultSetup128B);
	if((dwAttributes128A!=INVALID_FILE_ATTRIBUTES) && (dwAttributes128B!=INVALID_FILE_ATTRIBUTES))
		return true;

	OEMPA_ResetStructCycle(&cycle);
	OEMPA_ResetStructFocalLaw(&focalLaw);
	if(!focalLaw.GetDWORDElementSize())
		focalLaw.SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
	OEMPA_ResetStructFocalLaw(&focalLaw);

	root.iCycleCount = 1;
	cycle.dRange = 30.0e-6;
	cycle.bMaximum = true;
	cycle.dTimeSlot = 2000.0e-6;
	cycle.dGainDigital = 20.0;//1.1.5.4h
	cycle.fGainAnalog = 20.0f;

	if(dwAttributes128B==INVALID_FILE_ATTRIBUTES)
	{
		bRet = OEMPA_WriteFileText(wcLoadDefaultSetup128B,&root,&cycle,&focalLaw,&focalLaw) && bRet;
		WritePrivateProfileString(L"Root",L"Enable256",L"1",wcLoadDefaultSetup128B);
	}

	focalLaw.iElementCount = 1;
	focalLaw.iFocalCount = 1;
	focalLaw.adwElement[0] = 1;
	focalLaw.iDelayCount = focalLaw.iFocalCount*focalLaw.iElementCount;
	focalLaw.afPrm[0] = 0.5e-6f;

	if(dwAttributes128A==INVALID_FILE_ATTRIBUTES)
	{
		bRet = OEMPA_WriteFileText(wcLoadDefaultSetup128A,&root,&cycle,&focalLaw,&focalLaw) && bRet;
		WritePrivateProfileString(L"Root",L"Enable256",L"1",wcLoadDefaultSetup128A);
	}

	return bRet;
}
bool WINAPI OEMPA_LoadCalibrationSetup256(	CHWDeviceOEMPA *pHWDeviceOEMPA,bool bCheck,
										float fWidth,float fStart,float fRange,float fGainAnalog,double dGainDigital,
										const wchar_t *wcCalibrationFileInput,const wchar_t *wcCalibrationFileOutput,
										double dFMCTimeSlotFactor,int &iCalibrationPointCount,double &dAcquisitionTime)
{
	bool bRet=true;
	structRoot rootMaster,rootSlave,root;
	structCycle *pCycleAux=NULL;
	CFocalLaw *pAux=NULL;
	structCycle *pCycleRef=NULL;
	CFocalLaw *pEmissionRef=NULL,*pReceptionRef=NULL;
	structCycle *pCycleMaster=NULL;
	CFocalLaw *pEmissionMaster=NULL,*pReceptionMaster=NULL;
	structCycle *pCycleSlave=NULL;
	CFocalLaw *pEmissionSlave=NULL,*pReceptionSlave=NULL;
	bool bCalibrationFileInput,bCalibrationFileOutput;
	bool bFullMatrixCapture;
	int iOEMPAApertureElementCountMax;
	float fCalibrationOffset=0.0;
	CHWDeviceOEMPA *pHWMaster,*pHWSlave;
	wchar_t wcCalibrationFileOutputMaster[MAX_PATH];
	wchar_t wcCalibrationFileOutputSlave[MAX_PATH];
	wchar_t drive[_MAX_DRIVE],dir[_MAX_DIR],fname[MAX_PATH],ext[_MAX_EXT];
		
	iCalibrationPointCount = 0;
	if(!pHWDeviceOEMPA || !pHWDeviceOEMPA->GetSWDeviceOEMPA() || !pHWDeviceOEMPA->GetMatched256DeviceOEMPA())
		return false;
	pHWMaster = pHWDeviceOEMPA;
	pHWSlave = (CHWDeviceOEMPA*)pHWDeviceOEMPA->GetMatched256DeviceOEMPA();
	bFullMatrixCapture = pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture();
	if(bFullMatrixCapture)
		return false;
	iOEMPAApertureElementCountMax = pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetApertureCountMax();
	if(!iOEMPAApertureElementCountMax)
		return false;
	if(wcslen(wcCalibrationFileInput))
		bCalibrationFileInput = true;
	else
		bCalibrationFileInput = false;
	if(wcslen(wcCalibrationFileOutput))
	{
		bCalibrationFileOutput = true;
		_wsplitpath_s(wcCalibrationFileOutput,drive,_MAX_DRIVE,dir,_MAX_DIR,fname,MAX_PATH,ext,_MAX_EXT);
	}else
		bCalibrationFileOutput = false;

	OEMPA_InitRoot(root,pHWDeviceOEMPA);
	if(bCalibrationFileInput && !OEMPA_ReadFileText(wcCalibrationFileInput,root,pCycleRef,pEmissionRef,pReceptionRef))
		bRet = false;

	//Master
	if(bCalibrationFileOutput)
	{
		wcscpy_s(wcCalibrationFileOutputMaster,MAX_PATH,drive);
		wcscat_s(wcCalibrationFileOutputMaster,MAX_PATH,dir);
		wcscat_s(wcCalibrationFileOutputMaster,MAX_PATH,fname);
		wcscat_s(wcCalibrationFileOutputMaster,MAX_PATH,L"128A");
		wcscat_s(wcCalibrationFileOutputMaster,MAX_PATH,ext);
	}
	OEMPA_InitRoot(rootMaster,pHWDeviceOEMPA);
	rootMaster.b256Master = true;
	rootMaster.ullSavedParameters |= OEMPA_TRIGGER_MODE | OEMPA_ASCAN_ENABLE | OEMPA_ASCAN_BITSIZE;//1.1.5.3l
	pCycleMaster = OEMPA_AllocCycle(iOEMPAApertureElementCountMax);
	pEmissionMaster = new CFocalLaw[iOEMPAApertureElementCountMax];
	pReceptionMaster = new CFocalLaw[iOEMPAApertureElementCountMax];
	if(pCycleMaster && pEmissionMaster && pReceptionMaster)
	{
		rootMaster.iCycleCount = iOEMPAApertureElementCountMax;
		OEMPA_ResetArrayFocalLaw(rootMaster.iCycleCount,pEmissionMaster);
		OEMPA_ResetArrayFocalLaw(rootMaster.iCycleCount,pReceptionMaster);
		for(int i=0;(i<iOEMPAApertureElementCountMax) && bRet;i++)
		{
			pCycleAux = &pCycleMaster[i];
			OEMPA_ResetStructCycle(pCycleAux);
			if(pCycleRef)
			{
				memcpy(pCycleAux,pCycleRef,sizeof(structCycle));
			}else{
				pCycleAux->eCompressionType = eCompression;
				pCycleAux->dStart = fStart;//0.0;
				pCycleAux->dRange = fRange;//2.0e-6;
				pCycleAux->dTimeSlot = 2000.0e-6;
				pCycleAux->bMaximum = true;
				pCycleAux->dGainDigital = dGainDigital;//20.0;
				pCycleAux->iFilterIndex = 1;
			}

			pAux = &pEmissionMaster[i];
			if(pEmissionRef && (pEmissionRef->iFocalCount==1) && (pEmissionRef->iElementCount==1))
			{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
				pAux->afPrm[0] = pEmissionRef->afPrm[0];
				pAux->afDelay[0][0] = pEmissionRef->afDelay[0][0];
			}else{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
				pAux->afPrm[0] = fWidth;
				pAux->afDelay[0][0] = 0.0f;
			}
			pAux->adFocalTimeOfFlight[0] = 0.0;
			pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
			pAux->adwElement.SetAllElementEnable(false);
			//Master emission
			if(i<iOEMPAApertureElementCountMax/2)
			{
				pAux->adwElement.SetElementEnable(i,true);
				pAux->iElementCount = 1;
				pAux->iPrmCount = 1;
			}else{
				pAux->iElementCount = 0;
				pAux->iPrmCount = 0;
			}
			pAux->iDelayCount = 1;
			pAux->iFocalCount = 1;

			pAux = &pReceptionMaster[i];
			if(pReceptionRef && (pReceptionRef->iFocalCount==1) && (pReceptionRef->iDelayCount<=1) && (pReceptionRef->iElementCount==1))
			{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax/2);
				pAux->afPrm[0] = pReceptionRef->afPrm[0];
				pAux->afDelay[0][0] = pReceptionRef->afDelay[0][0];
				pAux->adFocalTimeOfFlight[0] = pReceptionRef->adFocalTimeOfFlight[0];
			}else{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax/2);
				pAux->afPrm[0] = fGainAnalog;
				pAux->afDelay[0][0] = 0.0f;
				pAux->adFocalTimeOfFlight[0] = 0.0;
			}
			pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax/2);
			pAux->adwElement.SetAllElementEnable(false);
			//Master reception
			if(i<iOEMPAApertureElementCountMax/2)
			{
				pAux->adwElement.SetElementEnable(i,true);
				pAux->iElementCount = 1;
				pAux->iPrmCount = 1;
			}else{
				pAux->iElementCount = 0;
				pAux->iPrmCount = 0;
			}
			pAux->iDelayCount = 1;
			pAux->iFocalCount = 1;
		}
	}else
		bRet = false;

	if(bCalibrationFileOutput)
	{
		wcscpy_s(wcCalibrationFileOutputSlave,MAX_PATH,drive);
		wcscat_s(wcCalibrationFileOutputSlave,MAX_PATH,dir);
		wcscat_s(wcCalibrationFileOutputSlave,MAX_PATH,fname);
		wcscat_s(wcCalibrationFileOutputSlave,MAX_PATH,L"128B");
		wcscat_s(wcCalibrationFileOutputSlave,MAX_PATH,ext);
	}
	OEMPA_InitRoot(rootSlave,pHWDeviceOEMPA);
	rootSlave.b256Slave = true;
	rootSlave.ullSavedParameters |= OEMPA_TRIGGER_MODE | OEMPA_ASCAN_ENABLE | OEMPA_ASCAN_BITSIZE;//1.1.5.3l
	pCycleSlave = OEMPA_AllocCycle(iOEMPAApertureElementCountMax);
	pEmissionSlave = new CFocalLaw[iOEMPAApertureElementCountMax];
	pReceptionSlave = new CFocalLaw[iOEMPAApertureElementCountMax];
	if(pCycleSlave && pEmissionSlave && pReceptionSlave)
	{
		rootSlave.iCycleCount = iOEMPAApertureElementCountMax;
		OEMPA_ResetArrayFocalLaw(rootSlave.iCycleCount,pEmissionSlave);
		OEMPA_ResetArrayFocalLaw(rootSlave.iCycleCount,pReceptionSlave);
		for(int i=0;(i<iOEMPAApertureElementCountMax) && bRet;i++)
		{
			pCycleAux = &pCycleSlave[i];
			OEMPA_ResetStructCycle(pCycleAux);
			if(pCycleRef)
			{
				memcpy(pCycleAux,pCycleRef,sizeof(structCycle));
			}else{
				pCycleAux->eCompressionType = eCompression;
				pCycleAux->dStart = fStart;//0.0;
				pCycleAux->dRange = fRange;//2.0e-6;
				pCycleAux->dTimeSlot = 2000.0e-6;
				pCycleAux->bMaximum = true;
				pCycleAux->dGainDigital = dGainDigital;//20.0;
				pCycleAux->iFilterIndex = 1;
			}

			pAux = &pEmissionSlave[i];
			if(pEmissionRef && (pEmissionRef->iFocalCount==1) && (pEmissionRef->iElementCount==1))
			{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
				pAux->afPrm[0] = pEmissionRef->afPrm[0];
				pAux->afDelay[0][0] = pEmissionRef->afDelay[0][0];
			}else{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
				pAux->afPrm[0] = fWidth;
				pAux->afDelay[0][0] = 0.0f;
			}
			pAux->adFocalTimeOfFlight[0] = 0.0;
			pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
			pAux->adwElement.SetAllElementEnable(false);
			//Slave emission
			if(i>=iOEMPAApertureElementCountMax/2)
			{
				pAux->adwElement.SetElementEnable(i-iOEMPAApertureElementCountMax/2,true);
				pAux->iElementCount = 1;
				pAux->iPrmCount = 1;
				pAux->iDelayCount = 1;
				pAux->iFocalCount = 1;
			}else{
				pAux->iElementCount = 0;
				pAux->iPrmCount = 0;
				pAux->iDelayCount = 0;
				pAux->iFocalCount = 0;
			}

			pAux = &pReceptionSlave[i];
			if(pReceptionRef && (pReceptionRef->iFocalCount==1) && (pReceptionRef->iDelayCount<=1) && (pReceptionRef->iElementCount==1))
			{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax/2);
				pAux->afPrm[0] = pReceptionRef->afPrm[0];
				pAux->afDelay[0][0] = pReceptionRef->afDelay[0][0];
				pAux->adFocalTimeOfFlight[0] = pReceptionRef->adFocalTimeOfFlight[0];
			}else{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax/2);
				pAux->afPrm[0] = fGainAnalog;
				pAux->afDelay[0][0] = 0.0f;
				pAux->adFocalTimeOfFlight[0] = 0.0;
			}
			pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax/2);
			pAux->adwElement.SetAllElementEnable(false);
			//Slave reception
			if(i>=iOEMPAApertureElementCountMax/2)
			{
				pAux->adwElement.SetElementEnable(i-iOEMPAApertureElementCountMax/2,true);
				pAux->iElementCount = 1;
				pAux->iPrmCount = 1;
				pAux->iDelayCount = 1;
				pAux->iFocalCount = 1;
			}else{
				pAux->iElementCount = 0;
				pAux->iPrmCount = 0;
				pAux->iDelayCount = 0;
				pAux->iFocalCount = 0;
			}
		}
	}else
		bRet = false;

	for(int iCycleIndex=0;iCycleIndex<rootMaster.iCycleCount;iCycleIndex++)
	{
		if(	(pReceptionMaster[iCycleIndex].iFocalCount>0) &&
			(pReceptionSlave[iCycleIndex].iFocalCount>0) &&
			(pReceptionMaster[iCycleIndex].iFocalCount != pReceptionSlave[iCycleIndex].iFocalCount))
			{bRet = false;goto return_false;}
		pReceptionMaster[iCycleIndex].m_p256MatchedDevice = &pReceptionSlave[iCycleIndex];
		pReceptionSlave[iCycleIndex].m_p256MatchedDevice = &pReceptionMaster[iCycleIndex];
	}

	dAcquisitionTime = rootMaster.iCycleCount*pCycleMaster[0].dTimeSlot;
	if(bCalibrationFileOutput && !OEMPA_WriteFileText(wcCalibrationFileOutputMaster,&rootMaster,pCycleMaster,pEmissionMaster,pReceptionMaster))
		bRet = false;
	if(bCalibrationFileOutput && !OEMPA_WriteFileText(wcCalibrationFileOutputSlave,&rootSlave,pCycleSlave,pEmissionSlave,pReceptionSlave))
		bRet = false;
	if(pHWMaster->LockDevice() && pHWSlave->LockDevice())
	{
		if(pHWMaster->GetSWDeviceOEMPA()->IsTimeOffsetSupported() && pHWSlave->GetSWDeviceOEMPA()->IsTimeOffsetSupported())
		{
			if(!bCheck && !pHWMaster->SetTimeOffset(fCalibrationOffset))
				bRet = false;
			//if(!bCheck && !pHWSlave->SetTimeOffset(fCalibrationOffset))
			//	bRet = false;
		}
		if(bRet && !OEMPA_WriteHW(pHWMaster, rootMaster, pCycleMaster, pEmissionMaster, pReceptionMaster,eAcqOff))
			bRet = false;
		if(bRet && !OEMPA_WriteHW(pHWSlave,rootSlave, pCycleSlave, pEmissionSlave, pReceptionSlave, eAcqOff))
			bRet = false;
		if(bRet)
		{
			if(bCalibrationFileOutput && !OEMPA_WriteFileText(wcCalibrationFileOutputMaster,&rootMaster,pCycleMaster,pEmissionMaster,pReceptionMaster))
				bRet = false;
			if(bCalibrationFileOutput && !OEMPA_WriteFileText(wcCalibrationFileOutputSlave,&rootSlave,pCycleSlave,pEmissionSlave,pReceptionSlave))
				bRet = false;
			iCalibrationPointCount = (int)pCycleMaster->lPointCount;
		}
		Sleep(250);//required for USB3.
		if(!pHWSlave->UnlockDevice(eAcqOn) || !pHWMaster->UnlockDevice(eAcqOn))
			bRet = false;
	}else
		bRet = false;

return_false:
	if(pCycleMaster)
		delete[] pCycleMaster;
	if(pEmissionMaster)
		delete[] pEmissionMaster;
	if(pReceptionMaster)
		delete[] pReceptionMaster;

	if(pCycleSlave)
		delete[] pCycleSlave;
	if(pEmissionSlave)
		delete[] pEmissionSlave;
	if(pReceptionSlave)
		delete[] pReceptionSlave;

	if(pCycleRef)
		delete[] pCycleRef;
	if(pEmissionRef)
		delete[] pEmissionRef;
	if(pReceptionRef)
		delete[] pReceptionRef;
	return bRet;
}

bool WINAPI OEMPA_LoadCalibrationSetup(	CHWDeviceOEMPA *pHWDeviceOEMPA,bool bCheck,
										float fWidth,float fStart,float fRange,float fGainAnalog,double dGainDigital,
										const wchar_t *wcCalibrationFileInput,const wchar_t *wcCalibrationFileOutput,
										double dFMCTimeSlotFactor,int &iCalibrationPointCount,double &dAcquisitionTime)
{
	bool bRet=true;
	structRoot root;
	structCycle *pCycle=NULL,*pCycleAux=NULL;
	CFocalLaw *pEmission=NULL,*pReception=NULL,*pAux=NULL;
	structCycle *pCycleRef=NULL;
	CFocalLaw *pEmissionRef=NULL,*pReceptionRef=NULL;
	bool bCalibrationFileInput,bCalibrationFileOutput;
	bool bFullMatrixCapture;
	int iOEMPAApertureElementCountMax,iFMCCycleCount;
	float fCalibrationOffset=0.0;
		
	iCalibrationPointCount = 0;
	if(!pHWDeviceOEMPA || !pHWDeviceOEMPA->GetSWDeviceOEMPA() || pHWDeviceOEMPA->GetMatched256DeviceOEMPA())
		return false;
	bFullMatrixCapture = pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture();
	iOEMPAApertureElementCountMax = pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetApertureCountMax();
	if(!iOEMPAApertureElementCountMax)
		return false;
	if(wcslen(wcCalibrationFileInput))
		bCalibrationFileInput = true;
	else
		bCalibrationFileInput = false;
	if(wcslen(wcCalibrationFileOutput))
		bCalibrationFileOutput = true;
	else
		bCalibrationFileOutput = false;
	OEMPA_InitRoot(root,pHWDeviceOEMPA);
	root.ullSavedParameters |= OEMPA_TRIGGER_MODE | OEMPA_ASCAN_ENABLE | OEMPA_ASCAN_BITSIZE;//1.1.5.3l
	pCycle = OEMPA_AllocCycle(iOEMPAApertureElementCountMax);
	pEmission = new CFocalLaw[iOEMPAApertureElementCountMax];
	pReception = new CFocalLaw[iOEMPAApertureElementCountMax];
	if(pCycle && pEmission && pReception)
	{
		if(bCalibrationFileInput && !OEMPA_ReadFileText(wcCalibrationFileInput,root,pCycleRef,pEmissionRef,pReceptionRef))
			bRet = false;
		root.iCycleCount = iOEMPAApertureElementCountMax;
		OEMPA_ResetArrayFocalLaw(root.iCycleCount,pEmission);
		OEMPA_ResetArrayFocalLaw(root.iCycleCount,pReception);
		for(int i=0;(i<iOEMPAApertureElementCountMax) && bRet;i++)
		{
			pCycleAux = &pCycle[i];
			OEMPA_ResetStructCycle(pCycleAux);
			if(pCycleRef)
			{
				memcpy(pCycleAux,pCycleRef,sizeof(structCycle));
			}else{
				pCycleAux->eCompressionType = eCompression;
				pCycleAux->dStart = fStart;//0.0;
				pCycleAux->dRange = fRange;//2.0e-6;
				pCycleAux->dTimeSlot = 2000.0e-6;
				pCycleAux->bMaximum = true;
				pCycleAux->dGainDigital = dGainDigital;//20.0;
				pCycleAux->iFilterIndex = 1;
			}

			pAux = &pEmission[i];
			if(pEmissionRef && (pEmissionRef->iFocalCount==1) && (pEmissionRef->iElementCount==1))
			{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
				pAux->afPrm[0] = pEmissionRef->afPrm[0];
				pAux->afDelay[0][0] = pEmissionRef->afDelay[0][0];
			}else{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
				pAux->afPrm[0] = fWidth;
				pAux->afDelay[0][0] = 0.0f;
			}
			pAux->adFocalTimeOfFlight[0] = 0.0;
			pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
			pAux->adwElement.SetAllElementEnable(false);
			pAux->adwElement.SetElementEnable(i,true);
			pAux->iElementCount = 1;
			pAux->iDelayCount = 1;
			pAux->iFocalCount = 1;
			pAux->iPrmCount = 1;

			pAux = &pReception[i];
			if(pReceptionRef && (pReceptionRef->iFocalCount==1) && (pReceptionRef->iDelayCount<=1) && (pReceptionRef->iElementCount==1))
			{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
				pAux->afPrm[0] = pReceptionRef->afPrm[0];
				pAux->afDelay[0][0] = pReceptionRef->afDelay[0][0];
				pAux->adFocalTimeOfFlight[0] = pReceptionRef->adFocalTimeOfFlight[0];
			}else{
				pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
				pAux->afPrm[0] = fGainAnalog;
				pAux->afDelay[0][0] = 0.0f;
				pAux->adFocalTimeOfFlight[0] = 0.0;
			}
			pAux->SetAllocatedSize(1,iOEMPAApertureElementCountMax);
			pAux->adwElement.SetAllElementEnable(false);
			pAux->adwElement.SetElementEnable(i,true);
			pAux->iElementCount = 1;
			pAux->iDelayCount = 1;
			pAux->iFocalCount = 1;
			pAux->iPrmCount = 1;
		}
		if(bFullMatrixCapture)
		{
			for(int i=0;(i<root.iCycleCount) && bRet;i++)
			{
				pCycleAux = &pCycle[i];
				//1.1.5.3l pCycleAux->dTimeSlot *= (root.iFMCElementStop-root.iFMCElementStart+1)/dFMCTimeSlotFactor;
				iFMCCycleCount = root.iFMCElementStop-root.iFMCElementStart+1;
				if(iFMCCycleCount%root.iFMCElementStep>0)
					iFMCCycleCount = 1+iFMCCycleCount/root.iFMCElementStep;
				else
					iFMCCycleCount = iFMCCycleCount/root.iFMCElementStep;
				pCycleAux->dTimeSlot *= iFMCCycleCount/dFMCTimeSlotFactor;
			}
		}
		dAcquisitionTime = root.iCycleCount*pCycle[0].dTimeSlot;
		if(bCalibrationFileOutput && !OEMPA_WriteFileText(wcCalibrationFileOutput,&root,pCycle,pEmission,pReception))
			bRet = false;
		if(pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsTimeOffsetSupported())
		{
			if(pHWDeviceOEMPA->LockDevice())
			{
				if(!bCheck && !pHWDeviceOEMPA->SetTimeOffset(fCalibrationOffset))
					bRet = false;
				if(bRet && !OEMPA_WriteHW(pHWDeviceOEMPA,root,pCycle,pEmission,pReception,eAcqOff))
					bRet = false;
				else{
					if(bCalibrationFileOutput && !OEMPA_WriteFileText(wcCalibrationFileOutput,&root,pCycle,pEmission,pReception))
						bRet = false;
					iCalibrationPointCount = (int)pCycle->lPointCount;
				}
				Sleep(250);//required for USB3.
				if(!pHWDeviceOEMPA->UnlockDevice(eAcqOn))
					bRet = false;
			}else
				bRet = false;
		}else{
			if(bRet && !OEMPA_WriteHW(pHWDeviceOEMPA,root,pCycle,pEmission,pReception,eAcqOn))
				bRet = false;
			else{
				if(bCalibrationFileOutput && !OEMPA_WriteFileText(wcCalibrationFileOutput,&root,pCycle,pEmission,pReception))
					bRet = false;
				iCalibrationPointCount = (int)pCycle->lPointCount;
			}
		}
	}else
		bRet = false;
	if(pCycle)
		delete[] pCycle;
	if(pEmission)
		delete[] pEmission;
	if(pReception)
		delete[] pReception;
	if(pCycleRef)
		delete[] pCycleRef;
	if(pEmissionRef)
		delete[] pEmissionRef;
	if(pReceptionRef)
		delete[] pReceptionRef;
	return bRet;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		OEMPA_SetReadFileWriteHW(OEMPA_ReadFileWriteHW);
		OEMPA_SetReadFileWriteHW256(OEMPA_ReadFileWriteHW256);
		OEMPA_SetCreateDefaultSetupFile(OEMPA_CreateDefaultSetupFile);
		OEMPA_SetLoadCalibrationSetup(OEMPA_LoadCalibrationSetup);
		OEMPA_SetLoadCalibrationSetup256(OEMPA_LoadCalibrationSetup256);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

