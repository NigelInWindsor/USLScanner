
#include "stdafx.h"
#include <ShellApi.h>
#include <tlhelp32.h> 
#include "mHWDeviceOEMPA.h"
#include "UTKernelBasic.h"

void TracePrintf(char *pTrace);

bool UTKernelMatlab_ToolboxRun(bool bShow);
DWORD UTKernelMatlab_GetToolboxProcessId();
bool UTKernelMatlab_ConnectToolbox(DWORD dwProcessId);
bool UTKernelMatlab_ToolboxClose();
DWORD UTKernelMatlab_ToolboxEcho(DWORD dwData);
bool UTKernelMatlab_ToolboxVisible(bool bEnable);
bool UTKernelMatlab_EnableEventUpdate(bool bEnable);
bool UTKernelMatlab_ToolboxDisplay(int iRootId);
bool UTKernelMatlab_ToolboxWizardNew();
bool UTKernelMatlab_ToolboxWizardDelete();
bool UTKernelMatlab_ToolboxWizardUpdateScan();
bool UTKernelMatlab_ToolboxIsWizardEnabled();
bool UTKernelMatlab_GetWizardId(int &iWizardSystemId,int &iWizardChannelId);
int UTKernelMatlab_GetWizardBeamCount();
bool UTKernelMatlab_GetWizardBeam(int iBeamIndex,double &dProbeX,double &dProbeY,double &dProbeZ,double &dEmergenceX,double &dEmergenceY,double &dEmergenceZ,double &dFocalX,double &dFocalY,double &dFocalZ);
bool UTKernelMatlab_ToolboxWizardReset();
int UTKernelMatlab_GetWizardElementCount();
bool UTKernelMatlab_GetWizardElement(int iBeamIndex,double &dProbeX,double &dProbeY,double &dProbeZ);
bool UTKernelMatlab_GetToolboxParameter(wchar_t *pIn,int iSize,wchar_t *pOut);
bool UTKernelMatlab_SetToolboxParameter(wchar_t *pCmdLine);
bool UTKernelMatlab_ToolboxLoadWizard(wchar_t *pWizardFile);
bool UTKernelMatlab_ToolboxSaveWizard(wchar_t *pWizardFile,bool bSaveWizardResults);
bool UTKernelMatlab_ToolboxLoadMatrix(wchar_t *pMatrixFile);
bool UTKernelMatlab_ToolboxSaveMatrix(wchar_t *pMatrixFile);
bool UTKernelMatlab_ToolboxSaveSetup(int iDeviceId, wchar_t *pSetupFile);
bool UTKernelMatlab_ToolboxSaveSetupEx(int iDeviceId, wchar_t *pSetupFileIn, wchar_t *pSetupFileOut);
int UTKernelMatlab_ToolboxGetDeviceId();
int UTKernelMatlab_ToolboxGetRootId(wchar_t *pName);
bool UTKernelMatlab_ToolboxDeviceAddress(wchar_t *pIPAddressAndPort);
bool UTKernelMatlab_ToolboxDeviceConnection(bool bConnect);
bool UTKernelMatlab_ToolboxIsDeviceConnected();
wchar_t *UTKernel_GetToolboxDeviceType();

DllImport bool OEMPA_SaveRegistryString(HKEY hRootKey,wchar_t *wcSection,wchar_t *wcKey,wchar_t *pValue);

extern "C"{
	void ExitDll256();

#ifdef _DEBUG
	__declspec(dllexport) void mxDelete(BYTE *pBuffer, int iLine, char *file)
	{
		delete pBuffer;
	}
	__declspec(dllexport) BYTE* mxNew(int iSize, int iLine, char *file)
	{
		return new BYTE[iSize];
	}
#else _DEBUG
	__declspec(dllexport) void mxDelete(BYTE *pBuffer)
	{
		delete pBuffer;
	}
	__declspec(dllexport) BYTE* mxNew(int iSize)
	{
		return new BYTE[iSize];
	}
#endif _DEBUG

	__declspec(dllexport) void ExitDll()
	{
		ExitDll256();
	}
	__declspec(dllexport) void mxTraceMemory(char *string)
	{
		TracePrintf(string); 
	}

	bool g_bAutoDisconnect=false;
	__declspec(dllexport) bool mxAutoDisconnect(bool bEnable)
	{
		if(g_bAutoDisconnect != bEnable)
		{
			if(bEnable)
				CHWDeviceOEMPA::RegisterMultiProcess(L"MatlabStub");
			else
				CHWDeviceOEMPA::UnregisterMultiProcess();
		}
		g_bAutoDisconnect = bEnable;
		return true;
	}

//////////////////////////////////////////////////////////////
//DRIVER MANAGEMENT
//CUTKernelRoot functions

	__declspec(dllexport) int mxNewDevice(wchar_t *pIPAddressAndPort, int iOEMPACycleCountMax, int iAscanPointCount, int bEnableFifo)
	//input "pIPAddressAndPort" : example L"192.168.1.11", "192.168.1.11:5001"
	//output return the device identifier number if no error (-1 if error).
	{
		CHWDevice *pHWDevice;
		CHWDeviceOEMPA *pHWDeviceOEMPA;
		wchar_t *pPort;
		wchar_t pIPAddress[50];
		int iPort;
		wchar_t wcClassName[MAX_PATH];
		void *pDerivedClass;

TracePrintf("NewDevice\r\n");
#ifndef _WIN64
		if(bEnableFifo)
			return -1;
#endif _WIN64
		for(int iDeviceId=0;iDeviceId<256;iDeviceId++)
		{
			pHWDevice = CHWDevice::GetHWDevice(iDeviceId);
			if(pHWDevice)
			{
				wcClassName[0] = 0;
				pDerivedClass = NULL;
				pHWDevice->GetDerivedClass(MAX_PATH,wcClassName,pDerivedClass);
				if(!wcscmp(wcClassName,L"mCHWDeviceOEMPA"))
				{
					pHWDeviceOEMPA = (CHWDeviceOEMPA*)pHWDevice;
					if(pHWDeviceOEMPA &&
						!wcscmp(pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetIPAddress(),pIPAddressAndPort))
						return iDeviceId;
				}
			}
		}
		wcscpy_s(pIPAddress,50,pIPAddressAndPort);
		iPort = -1;
		pPort = wcsstr(pIPAddress,L":");
		if(pPort)
		{
			*pPort = 0;
			pPort++;
			iPort = _wtoi(pPort);
		}
		mCHWDeviceOEMPA *mpHWDeviceOEMPA = new mCHWDeviceOEMPA(iOEMPACycleCountMax, iAscanPointCount, bEnableFifo==0?false:true);
		if(mpHWDeviceOEMPA)
		{
			if(wcslen(pIPAddress)>3)
				mpHWDeviceOEMPA->m_hwDeviceOEMPA.GetSWDeviceOEMPA()->SetIPAddress(pIPAddress);
			if(iPort>=0)
				mpHWDeviceOEMPA->m_hwDeviceOEMPA.GetSWDeviceOEMPA()->SetPort(iPort);
			return mpHWDeviceOEMPA->m_hwDeviceOEMPA.GetDeviceId();
		}
		return -1;
	}

	__declspec(dllexport) bool mxDeleteDevice(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return true if no error and false in case of error.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		delete mpHWDeviceOEMPA;
		return true;
	}

	__declspec(dllexport) bool mxFindDevice(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return true if no error and false in case of error.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return true;
	}

	__declspec(dllexport) const wchar_t *mxGetFolderExe()
	{
		return UTKernel_GetFolderExe();
	}

	__declspec(dllexport) DWORD mxGetCfgKernelFile(wchar_t *pSection,wchar_t *pKey,DWORD dwDefault)
	{
		DWORD dwData = GetPrivateProfileInt(pSection,pKey,dwDefault,UTKernel_GetConfigurationFile());
		return dwData;
	}

	__declspec(dllexport) bool mxSetCfgKernelFile(wchar_t *pSection,wchar_t *pKey,DWORD dwValue)
	{
		wchar_t pValue[20];

		if(wcslen(pSection)>=20)
			return false;
		wsprintf(pValue,L"%ld",dwValue);
		WritePrivateProfileString(pSection,pKey,pValue,UTKernel_GetConfigurationFile());
		return true;
	}

	__declspec(dllexport) bool mxSaveRegistryString(wchar_t *wcSection,wchar_t *wcKey,wchar_t *pValue)
	{
		return OEMPA_SaveRegistryString(HKEY_CURRENT_USER,wcSection,wcKey,pValue);
	}

	__declspec(dllexport) bool mxGetSerialNumber(int iDeviceId, wchar_t *wcSerialNumber, int iSize)
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		return pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetSerialNumber(wcSerialNumber, iSize);
	}

	__declspec(dllexport) bool mxGetStatus(int iDeviceId, wchar_t *wcStatus, int iSize)
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		int iPort;
		wchar_t wcString[MAX_PATH];

		if(!pHWDeviceOEMPA)
			return false;
		iPort = CHWDevice::GetMonitorPort(iDeviceId);
		swprintf(wcString, MAX_PATH, L"monitor=%d", iPort);
		if((int)wcslen(wcString)>=iSize)
			return false;
		wcscpy_s(wcStatus, iSize, wcString);
		return true;
	}

	__declspec(dllexport) bool mxGetRecoveryTimes(int iDeviceId, double &dAscanRecoveryTime, double &dFMCSubCycleRecoveryTime, double &dFMCCycleRecoveryTime)
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		dAscanRecoveryTime = pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFWAscanRecoveryTime();
		dFMCSubCycleRecoveryTime = pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFMCSubCycleRecoveryTime();
		dFMCCycleRecoveryTime= pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFMCCycleRecoveryTime();
		return true;
	}

	__declspec(dllexport) bool mxReadFileWriteHW(int iDeviceId,wchar_t *pFileName)
	//input "iDeviceId": device identifier number.
	//input "pFileName": OEMPA file path.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
#ifdef _DEVICE_OEMPA_256_256_
		mpHWDeviceOEMPA->m_csDevice256.SetMaster(false,mpHWDeviceOEMPA,mpHWDeviceOEMPA);
		mpHWDeviceOEMPA->m_csDevice256.SetSlave(false,mpHWDeviceOEMPA,mpHWDeviceOEMPA);
#endif _DEVICE_OEMPA_256_256_
		return OEMPA_ReadFileWriteHW(pHWDeviceOEMPA,pFileName);
	}

	__declspec(dllexport) bool mxReadHWWriteFile(int iDeviceId,wchar_t *pFileName)
	//input "iDeviceId": device identifier number.
	//input "pFileName": OEMPA file path.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		return OEMPA_ReadHWWriteFile(pHWDeviceOEMPA,g_iOEMPACycleCountMax,g_iOEMPAApertureElementCountMax,g_iOEMPAFocalCountMax,g_iOEMPADACCountMax,pFileName);
	}

	__declspec(dllexport) bool mxConnect(int iDeviceId,bool bValue)
	//input "iDeviceId": device identifier number.
	//input "bValue": true to connect the device, false to disconnect.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		wchar_t pIPAddress[40];
		DWORD dwProcessId;
		bool bRet;

		if(!pHWDeviceOEMPA)
			return false;
		if(bValue)
		{
			if(g_bAutoDisconnect)
			{
				wcscpy_s(pIPAddress,40,pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetIPAddress());
				if(CHWDeviceOEMPA::IsMultiProcessConnected(pIPAddress,dwProcessId))
					CHWDeviceOEMPA::DisconnectMultiProcess(pIPAddress,dwProcessId);
			}
			bRet = pHWDeviceOEMPA->GetSWDevice()->SetConnectionState(eConnected,true);
			mpHWDeviceOEMPA->Connected();
		}else
			bRet = pHWDeviceOEMPA->GetSWDevice()->SetConnectionState(eDisconnected,true);
		return bRet;
	}

	__declspec(dllexport) bool mxIsConnected(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		return pHWDeviceOEMPA->GetSWDevice()->IsConnected();
	}

	__declspec(dllexport) bool mxIsFifo(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->IsFifoAscan();
	}

	__declspec(dllexport) bool mxIsFullMatrixCaptureReadWrite(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		return pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite();
	}

	__declspec(dllexport) bool mxResetCounters(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		mpHWDeviceOEMPA->ResetCounters();
		return true;
	}

	__declspec(dllexport) bool mxResetEncoder(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		return pHWDeviceOEMPA->GetSWDeviceOEMPA()->ResetEncoder();
	}

	__declspec(dllexport) bool mxIsUsb3Connected(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		return pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsUSB3Connected();
	}

	__declspec(dllexport) bool mxEnableShot(int iDeviceId,bool bValue)
	//input "iDeviceId": device identifier number.
	//input "bValue": true to enable pulser, false to disable pulser.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		bool bRet;

		if(!pHWDeviceOEMPA)
			return false;
#ifdef _DEVICE_OEMPA_256_256_
		if(mpHWDeviceOEMPA->m_csDevice256.IsMaster() || mpHWDeviceOEMPA->m_csDevice256.IsSlave())
		{
			UTKernel_SystemMessageBoxList(L"Error the device is a 256:256, please call mxEnableShot256() !");
			return false;
		}
#endif _DEVICE_OEMPA_256_256_
		if(bValue)
			bRet = pHWDeviceOEMPA->GetSWDevice()->EnablePulser(true);
		else
			bRet = pHWDeviceOEMPA->GetSWDevice()->EnablePulser(false);
		return bRet;
	}

	__declspec(dllexport) bool mxEnableUsb3(int iDeviceId,bool bValue)
	//input "iDeviceId": device identifier number.
	//input "bValue": true to enable pulser, false to disable pulser.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		bool bRet;

		if(!pHWDeviceOEMPA)
			return false;
		if(bValue)
			bRet = pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableUSB3(true);
		else
			bRet = pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableUSB3(false);
		return bRet;
	}

	__declspec(dllexport) double mxGetAcquisitionStep(int iDeviceId)
	//input "iDeviceId": device identifier number.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return 0.0;
		return mpHWDeviceOEMPA->GetAcquisitionStep();
	}

	__declspec(dllexport) int mxGetSWCycleCount(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return the cycle count.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return -1;
		return mpHWDeviceOEMPA->GetSWCycleCount();
	}

	__declspec(dllexport) int mxGetHWCycleCount(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return the cycle count.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return -1;
		return mpHWDeviceOEMPA->GetHWCycleCount();
	}

	__declspec(dllexport) bool mxPerformCalibration(int iDeviceId, double *pdTimeOffset, double *pdDelayCorrectionMax)
	//input "iDeviceId": device identifier number.
	//output return the cycle count.
	{
		float fDelayCorrectionMax,fTimeOffset;
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		if(!pHWDeviceOEMPA->PerformCalibration(fDelayCorrectionMax,fTimeOffset))
			return false;
		if(pdTimeOffset)
			*pdTimeOffset = (double)fTimeOffset;
		if(pdDelayCorrectionMax)
			*pdDelayCorrectionMax = (double)fDelayCorrectionMax;
		return true;
	}
	__declspec(dllexport) bool mxEnableAlignment(int iDeviceId, bool bEnable)
	//input "iDeviceId": device identifier number.
	//output return the cycle count.
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA || !pHWDeviceOEMPA->GetSWDeviceOEMPA())
			return false;
		return pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableAlignment(bEnable);
	}
	__declspec(dllexport) bool mxSetTimeOffset(int iDeviceId, double dTimeOffset)
	//input "iDeviceId": device identifier number.
	//output return the cycle count.
	{
		float fTimeOffset;
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		if(!pHWDeviceOEMPA->IsDeviceLocked())
			return false;
		fTimeOffset = (float)dTimeOffset;
		return pHWDeviceOEMPA->SetTimeOffset(fTimeOffset);
	}
	__declspec(dllexport) bool mxSetDelayOffset(int iDeviceId, double dDelayOffset)
	//input "iDeviceId": device identifier number.
	//output return the cycle count.
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		float pfDelay[g_iOEMPAApertureDWordCount*sizeof(DWORD)*8];//128 elements
		int iElementCount;
		bool bRet;

		if(!pHWDeviceOEMPA || !pHWDeviceOEMPA->GetSWDeviceOEMPA())
			return false;
		iElementCount = pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetElementCountMax();
		for(int iIndex=0;iIndex<iElementCount;iIndex++)
			pfDelay[iIndex] = (float)dDelayOffset;
		bRet = pHWDeviceOEMPA->GetSWDeviceOEMPA()->SetNormalisationEmissionDelays(iElementCount,pfDelay);
		bRet = pHWDeviceOEMPA->GetSWDeviceOEMPA()->SetNormalisationReceptionDelays(iElementCount,pfDelay) && bRet;
		pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableNormalisationEmission(true);
		pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableNormalisationReception(true);
		return bRet;
	}

	__declspec(dllexport) int mxGetFullMatrixCapture(int iDeviceId,int *piElementStart,int *piElementStop,int *piElementStep)
	//input "iDeviceId": device identifier number.
	//output return the cycle count.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return -1;
		return mpHWDeviceOEMPA->IsFullMatrixCapture(*piElementStart,*piElementStop,*piElementStep);
	}

	__declspec(dllexport) bool mxGetAcquisitionAscanCount(int iDeviceId, __int64 &i64Count, __int64 &i64TotalByte)
	//input "iDeviceId": device identifier number.
	//output return the acquisition ascan count.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionAscanCount(i64Count, i64TotalByte);
	}

	__declspec(dllexport) bool mxGetAcquisitionAscanEncoder(int iDeviceId,int iEncoderIndex,double *pdEncoder,long *plRawEncoder)
	//input "iDeviceId": device identifier number.
	//input "iEncoderIndex": 0 for X, 1 for Y.
	//output "pdEncoder" : the ascan encoder value in meter (resolution is taken into account).
	//output "plRawEncoder" : the ascan encoder value (resolution is not taken into account).
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionAscanEncoder(iEncoderIndex,*pdEncoder,*plRawEncoder);
	}

	__declspec(dllexport) DWORD mxGetAcquisitionAscanSize(int iDeviceId,int iCycle)
	//input "iDeviceId": device identifier number.
	//input "iCycle": cycle number.
	//output return the acquisition ascan size.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return -1;
		return mpHWDeviceOEMPA->GetAcquisitionAscanSize(iCycle);
	}

	__declspec(dllexport) enumBitSize mxGetAcquisitionAscanBitSize(int iDeviceId,int iCycle, bool *pbSigned)
	//input "iDeviceId": device identifier number.
	//input "iCycle": cycle number.
	//output return the acquisition ascan bitsize.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return e8Bits;
		return mpHWDeviceOEMPA->GetAcquisitionAscanBitSize(iCycle, *pbSigned);
	}

	__declspec(dllexport) bool mxGetAcquisitionAscanData(int iDeviceId,int iCycle,int iSizeMax,BYTE *pBuffer,int *piSize)
	//input "iDeviceId": device identifier number.
	//input "iCycle": cycle number.
	//input "iSizeMax": the "pBuffer" maximum size (BYTE size).
	//input "pBuffer": buffer where to get data.
	//input "piSize": data size that have been stored in "pBuffer" (BYTE size).
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionAscanData(iCycle,iSizeMax,pBuffer,*piSize);
	}

	__declspec(dllexport) DWORD mxDumpAcquisitionAscanFifo(int iDeviceId, wchar_t *wcFileName)
	//input "iDeviceId": device identifier number.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		bool bRet = false;

		if(!mpHWDeviceOEMPA)
			return false;
		if(mpHWDeviceOEMPA->DumpAcquisitionAscanFifo(wcFileName))
			bRet = true;
		return bRet;
	}

	__declspec(dllexport) DWORD mxGetAcquisitionAscanFifoStatus(int iDeviceId, long *plAscanCount, long *plDataLost, long long *pllTotalCount, long long *pllTotalByteCount)
	//input "iDeviceId": device identifier number.
	//output return the acquisition ascan count.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		int iDataLost, iAscanCount;
		__int64 iTotalCount,iTotalByteCount;
		bool bRet = false;

		if(!mpHWDeviceOEMPA)
			return false;
		if(mpHWDeviceOEMPA->GetAcquisitionAscanFifoStatus(iAscanCount, iDataLost, iTotalCount, iTotalByteCount))
			bRet = true;
		else{
			iDataLost = 0;
			iAscanCount = 0;
			iTotalCount = 0;
		}
		if(plAscanCount)
			*plAscanCount = (long)iAscanCount;
		if(plDataLost)
			*plDataLost = (long)iDataLost;
		if(pllTotalCount)
			*pllTotalCount = (long long)iTotalCount;
		if(pllTotalByteCount)
			*pllTotalByteCount = (long long)iTotalByteCount;
		return bRet;
	}

	__declspec(dllexport) bool mxGetAcquisitionAscanFifoInfo(int iDeviceId, int iItem, int *piCycle, long long *pllSequence, int *piPointCount, BYTE *pbyByteSize, bool *pbSigned)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionAscanFifoInfo(iItem, *piCycle, *pllSequence, *piPointCount, *pbyByteSize, *pbSigned);
	}
	__declspec(dllexport) bool mxGetAcquisitionAscanFifoItem(int iDeviceId, long long sequence, int iCycle, int *piItem)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionAscanFifoItem(sequence, iCycle, *piItem);
	}
	__declspec(dllexport) bool mxGetAcquisitionAscanLifoItem(int iDeviceId, long long sequence, int iCycle, int *piItem)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionAscanLifoItem(sequence, iCycle, *piItem);
	}
	__declspec(dllexport) bool mxGetAcquisitionAscanItemInc(int iDeviceId, int *piItem)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionAscanItemInc(*piItem);
	}
	__declspec(dllexport) bool mxGetAcquisitionAscanItemDec(int iDeviceId, int *piItem)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionAscanItemDec(*piItem);
	}

	__declspec(dllexport) int mxGetAcquisitionAscanFifoIndex(int iDeviceId, bool bFifo, int iCycleCount, int *piCycleIn, int *piIndex, int *piCycle, long long *pllSequence, int *piPointCount, BYTE *pbyByteSize, bool *pbSigned)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		int iRet;

		if(!mpHWDeviceOEMPA)
			return 0;
//char string[MAX_PATH];
//sprintf_s(string,MAX_PATH,"LifoIndex %d start\r\n",iCycleCount);
//TracePrintf(string);
		iRet = mpHWDeviceOEMPA->GetAcquisitionIndex(true, bFifo, iCycleCount, piCycleIn, piIndex, piCycle, pllSequence, piPointCount, pbyByteSize, pbSigned);
//sprintf_s(string,MAX_PATH,"LifoIndex %d %d\r\n",iCycleCount,iRet);
//TracePrintf(string);
		return iRet;
	}

	__declspec(dllexport) int mxGetAcquisitionAscanLifoIndex(int iDeviceId, bool bFifo, int iCycleCount, int *piCycleIn, int *piIndex, int *piCycle, long long *pllSequence, int *piPointCount, BYTE *pbyByteSize, bool *pbSigned)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		int iRet;

		if(!mpHWDeviceOEMPA)
			return 0;
		iRet = mpHWDeviceOEMPA->GetAcquisitionIndex(true, bFifo, iCycleCount, piCycleIn, piIndex, piCycle, pllSequence, piPointCount, pbyByteSize, pbSigned);
		return iRet;
	}

	__declspec(dllexport) bool mxGetAcquisitionAscanFifoData(int iDeviceId, bool bPeek, int iItemCount, int *pItem, int iSizeMax, BYTE *pBuffer, int *piSize, int *piCycle, long long *pSequence, int iSizeEncoder, double *pdEncoder, long *plRawEncoder)
	//input "iDeviceId": device identifier number.
	//input "iCycle": cycle number.
	//input "iSizeMax": the "pBuffer" maximum size (BYTE size).
	//input "pBuffer": buffer where to get data.
	//input "piSize": data size that have been stored in "pBuffer" (BYTE size).
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		bool bRet;

//char string[MAX_PATH];
//sprintf_s(string,MAX_PATH,"FifoData %d start\r\n",iItemCount);
//TracePrintf(string);
		if(!mpHWDeviceOEMPA)
			return false;
		bRet = mpHWDeviceOEMPA->GetAcquisitionAscanFifoData(bPeek,iItemCount,pItem,iSizeMax,pBuffer,*piSize,piCycle,pSequence,iSizeEncoder,pdEncoder,plRawEncoder);
//sprintf_s(string,MAX_PATH,"FifoData %d %d\r\n",iItemCount,bRet?1:0);
//TracePrintf(string);
		return bRet;
	}

	__declspec(dllexport) bool mxFlushAcquisitionAscanFifo(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->FlushAcquisitionAscanFifo();
	}
	__declspec(dllexport) bool mxFlushAcquisitionCscanFifo(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->FlushAcquisitionCscanFifo();
	}

	__declspec(dllexport) bool mxGetAcquisitionCscanCount(int iDeviceId, __int64 &i64Count)
	//input "iDeviceId": device identifier number.
	//output return the acquisition cscan count.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionCscanCount(i64Count);
	}

	__declspec(dllexport) bool mxGetAcquisitionCscanEncoder(int iDeviceId,int iEncoderIndex,double *pdEncoder,long *plRawEncoder)
	//input "iDeviceId": device identifier number.
	//input "iEncoderIndex": 0 for X, 1 for Y.
	//output "pdEncoder" : the cscan encoder value in meter (resolution is taken into account).
	//output "plRawEncoder" : the cscan encoder value (resolution is not taken into account).
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionCscanEncoder(iEncoderIndex,*pdEncoder,*plRawEncoder);
	}

	__declspec(dllexport) bool mxGetAcquisitionCscanEnable(int iDeviceId,int iGate,int iSizeMax,BYTE *pBuffer,int *piSize)
	//Return the boolean array of each cycle,
	//data is 0 if no acquisition data have been acquired for the cycle
	//data is 1 if acquisition data have been acquired.
	//input "iDeviceId": device identifier number.
	//input "iGate": gate number (range is 0 to 3).
	//input "iSizeMax": the "pBuffer" maximum size (index is the cycle number).
	//input "pBuffer": buffer where to get data.
	//input "piSize": data size that have been stored in "pBuffer".
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionCscanEnable(iGate,iSizeMax,pBuffer,*piSize);
	}

	__declspec(dllexport) bool mxGetAcquisitionCscanAmplitude(int iDeviceId,int iGate,int iSizeMax,BYTE *pBuffer,int *piSize)
	//Return the amplitude data array of each cycle.
	//input "iDeviceId": device identifier number.
	//input "iGate": gate number (range is 0 to 3).
	//input "iSizeMax": the "pBuffer" maximum size.
	//input "pBuffer": buffer where to get data (index is the cycle number).
	//input "piSize": data size that have been stored in "pBuffer".
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionCscanAmplitude(iGate,iSizeMax,pBuffer,*piSize);
	}

	__declspec(dllexport) bool mxGetAcquisitionCscanTimeOfFlight(int iDeviceId,int iGate,int iSizeMax,short *pBuffer,int *piSize)
	//Return the time of flight data array of each cycle.
	//input "iDeviceId": device identifier number.
	//input "iGate": gate number (range is 0 to 3).
	//input "iSizeMax": the "pBuffer" maximum size.
	//input "pBuffer": buffer where to get data (index is the cycle number).
	//input "piSize": data size that have been stored in "pBuffer".
	//output return true if no error and false in case of error
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->GetAcquisitionCscanTimeOfFlight(iGate,iSizeMax,pBuffer,*piSize);
	}

	__declspec(dllexport) DWORD mxGetAcquisitionCscanFifoStatus(int iDeviceId, long *plCscanCount, long *plDataLost)
	//input "iDeviceId": device identifier number.
	//output return the acquisition ascan count.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		int iDataLost, iCscanCount;
		bool bRet = false;

		if(!mpHWDeviceOEMPA)
			return false;
		if(mpHWDeviceOEMPA->GetAcquisitionCscanFifoStatus(iCscanCount, iDataLost))
			bRet = true;
		else{
			iDataLost = 0;
			iCscanCount = 0;
		}
		if(plCscanCount)
			*plCscanCount = (long)iCscanCount;
		if(plDataLost)
			*plDataLost = (long)iDataLost;
		return bRet;
	}

	__declspec(dllexport) int mxGetAcquisitionCscanLifoIndex(int iDeviceId, bool bFifo, int iCycleCount, int *piCycleIn, int *piIndex, int *piCycle, long long *pllSequence)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		int iRet;

		if(!mpHWDeviceOEMPA)
			return 0;
		iRet = mpHWDeviceOEMPA->GetAcquisitionIndex(false, bFifo, iCycleCount, piCycleIn, piIndex, piCycle, pllSequence, NULL, NULL, NULL);
		return iRet;
	}

	__declspec(dllexport) int mxGetAcquisitionCscanFifoIndex(int iDeviceId, bool bFifo, int iCycleCount, int *piCycleIn, int *piIndex, int *piCycle, long long *pllSequence)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		int iRet;

		if(!mpHWDeviceOEMPA)
			return 0;
		iRet = mpHWDeviceOEMPA->GetAcquisitionIndex(false, bFifo, iCycleCount, piCycleIn, piIndex, piCycle, pllSequence, NULL, NULL, NULL);
		return iRet;
	}

	__declspec(dllexport) bool mxGetAcquisitionCscanFifoData(int iDeviceId, bool bFifo, bool bPeek, int iItemCount, int *pItem, BYTE *pAmp, short *psTof, int *piCycle, LONGLONG *pSequence, int iSizeEncoder, double *pdEncoder, long *plRawEncoder)
	//input "iDeviceId": device identifier number.
	//output return the acquisition ascan count.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		bool bRet = false;

		if(!mpHWDeviceOEMPA)
			return false;
		bRet = mpHWDeviceOEMPA->GetAcquisitionCscanData(bFifo, bPeek, iItemCount, pItem, pAmp, psTof, piCycle, pSequence, iSizeEncoder, pdEncoder, plRawEncoder);
		return bRet;
	}

	__declspec(dllexport) bool mxGetAcquisitionCscanLifoData(int iDeviceId, bool bFifo, bool bPeek, int iItemCount, int *pItem, BYTE *pAmp, short *psTof, int *piCycle, LONGLONG *pSequence, int iSizeEncoder, double *pdEncoder, long *plRawEncoder)
	//input "iDeviceId": device identifier number.
	//output return the acquisition ascan count.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		bool bRet = false;

		if(!mpHWDeviceOEMPA)
			return false;
		bRet = mpHWDeviceOEMPA->GetAcquisitionCscanData(bFifo, bPeek, iItemCount, pItem, pAmp, psTof, piCycle, pSequence, iSizeEncoder, pdEncoder, plRawEncoder);
		return bRet;
	}

	__declspec(dllexport) bool mxLockDevice(int iDeviceId, int eAcqState)
	//Lock OEM-PA Communication link to access FW parameters
	//input "iDeviceId": Master device identifier number.
	//input "eAcqState": set acquisition state; 0 - eAcqOff (disabled), 1 - eAcqDefault (use config file), 2 - eAcqOn (enabled)
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if((!pHWDeviceOEMPA) || (!pHWDeviceOEMPA->LockDevice((enumAcquisitionState)eAcqState)))
			return false;

		return true;
	}

	__declspec(dllexport) bool mxUnlockDevice(int iDeviceId, int eAcqState)
	//Unlock OEM-PA Communication link to access FW parameters
	//input "iDeviceId": Master device identifier number.
	//input "eAcqState": set acquisition state; 0 - eAcqOff (disabled), 1 - eAcqDefault (use config file), 2 - eAcqOn (enabled)
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if((!pHWDeviceOEMPA) || (!pHWDeviceOEMPA->UnlockDevice((enumAcquisitionState)eAcqState)))
			return false;

		return true;
	}

	__declspec(dllexport) bool mxSetGainDigital(int iDeviceId, int iCycle, double *pdGain)
	//Set Digital Gain of iCycle to dGain
	//input "iDeviceId": device identifier number.
	//input "iCycle": cycle being changed.
	//input/output "*pdGain": Gain value.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		return pHWDeviceOEMPA->SetGainDigital(iCycle, *pdGain);
	}

	__declspec(dllexport) bool mxSetGainAnalog(int iDeviceId, int iCycle, float *pfGain)
	//Set Analog Gain of iCycle to fGain
	//input "iDeviceId": device identifier number.
	//input "iCycle": cycle being changed.
	//input "*pfGain": Gain value.
	//output return true if no error and false in case of error
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		bool bRet;

		if(!pHWDeviceOEMPA)
			return false;
		bRet = pHWDeviceOEMPA->SetGainAnalog(iCycle, *pfGain);
		if(pHWDeviceOEMPA->GetHWMatched256DeviceOEMPA())
		{//1.1.5.4i
			if(!pHWDeviceOEMPA->GetHWMatched256DeviceOEMPA()->SetGainAnalog(iCycle,*pfGain))
				bRet = false;
		}
		return bRet;
	}

	__declspec(dllexport) bool mxSetAscanRange(int iDeviceId, int iCycle, double *pdRange, BYTE *pbCompression, long *lPointCount, long *lPointFactor)
	//Set range of iCycle to dRange
	//input "iDeviceId": device identifier number.
	//input "iCycle": cycle being changed.
	//output return true if no error and false in case of error
	//input/output "*pdRange": Range value.
	//output "piCompressionType": 0 Compression, 1 Decimation
	//output "iPointCount": Number of Points in Ascan
	//output "iPointFactor": Compression Factor of Ascan
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		enumCompressionType eComp = (enumCompressionType)*pbCompression;
		bool bRet = false;

		if(!mpHWDeviceOEMPA)
			return false;
		if(!pHWDeviceOEMPA)
			return false;
		bRet = pHWDeviceOEMPA->SetAscanRange(iCycle, *pdRange, eComp, *lPointCount, *lPointFactor);
		mpHWDeviceOEMPA->SetAcquisitionAscanSize(iCycle, *lPointCount);
		*pbCompression = (BYTE)eComp;
		return bRet;
	}

	__declspec(dllexport) bool mxSetAscanRangeWithFactor(int iDeviceId, int iCycle, double *pdRange, BYTE *pbCompression, long *lPointFactor, long *lPointCount)
	//Set range of iCycle to dRange
	//input "iDeviceId": device identifier number.
	//input "iCycle": cycle being changed.
	//output return true if no error and false in case of error
	//input/output "*pdRange": Range value.
	//input/output "piCompressionType": 0 Compression, 1 Decimation
	//output "iPointCount": Number of Points in Ascan
	//intput/output "iPointFactor": Compression Factor of Ascan
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		enumCompressionType eComp = (enumCompressionType)*pbCompression;
		bool bRet;

		if(!mpHWDeviceOEMPA)
			return false;
		if(!pHWDeviceOEMPA)
			return false;
		bRet = pHWDeviceOEMPA->SetAscanRangeWithFactor(iCycle, *pdRange, eComp, *lPointFactor, *lPointCount);
		mpHWDeviceOEMPA->SetAcquisitionAscanSize(iCycle, *lPointCount);
		*pbCompression = (BYTE)eComp;
		return bRet;
	}

	__declspec(dllexport) bool mxSetAscanStart(int iDeviceId, int iCycle, double *pdStart)
	//Set start of iCycle to dStart
	//input "iDeviceId": device identifier number.
	//input "iCycle": cycle being changed.
	//output return true if no error and false in case of error
	//input/output "*pdStart": Ascan start in seconds
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		bool bRet;

		if(!pHWDeviceOEMPA)
			return false;
		bRet = pHWDeviceOEMPA->SetAscanStart(iCycle, *pdStart);
		if(pHWDeviceOEMPA->GetHWMatched256DeviceOEMPA())
		{//1.1.5.4i
			if(!pHWDeviceOEMPA->GetHWMatched256DeviceOEMPA()->SetAscanStart(iCycle,*pdStart))
				bRet = false;
		}
		return bRet;
	}

	__declspec(dllexport) bool mxSetFilterIndex(int iDeviceId, int iCycle, int iFilterIndex)
	//Set range of iCycle to dRange
	//input "iDeviceId": device identifier number.
	//input "iCycle": cycle being changed.
	//input "iFilterIndex": Filter Index to apply.
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		enumOEMPAFilterIndex eFilterIndex=(enumOEMPAFilterIndex)iFilterIndex;
		bool bRet;

		if(!pHWDeviceOEMPA)
			return false;
		bRet = pHWDeviceOEMPA->SetFilterIndex(iCycle, eFilterIndex);
		return bRet;
	}

	__declspec(dllexport) bool mxSetFilter(int iDeviceId, int iFilterIndex, WORD *wScale, short wValue[g_iOEMPAFilterCoefficientMax])
	//Set range of iCycle to dRange
	//input "iDeviceId": device identifier number.
	//input "wScale": Summation divider.
	//input "wValue": FIR Coefficients
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		enumOEMPAFilterIndex eFilterIndex=(enumOEMPAFilterIndex)iFilterIndex;
		bool bRet;

		if(!pHWDeviceOEMPA)
			return false;
		bRet = pHWDeviceOEMPA->SetFilter(eFilterIndex, *wScale, wValue);
		return bRet;
	}

	__declspec(dllexport) int mxGetLostCountAscan(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output the number of Ascans lost.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return -1;
		return mpHWDeviceOEMPA->GetLostAscan();
	}

	__declspec(dllexport) int mxGetLostCountEncoder(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output the number of encoder counts missed.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);
		if(!mpHWDeviceOEMPA)
			return -1;
		return mpHWDeviceOEMPA->GetLostEncoder();
	}

	__declspec(dllexport) int mxGetLostCountCscan(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output the number of Cscans counts missed.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return -1;
		return mpHWDeviceOEMPA->GetLostCscan();
	}

	__declspec(dllexport) int mxGetLostCountUSB3(int iDeviceId)
	//input "iDeviceId": device identifier number.
	//output the number of Cscans counts missed.
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return -1;
		return mpHWDeviceOEMPA->GetLostUSB3();
	}

	__declspec(dllexport) bool mxGetFMCTimeLimitation(int iDeviceId,int iFMCSubCycleCount,double dAscanStart,double dAscanRange,double dTimeSlot,double *pdTimeSlotMin,double *pdHWAcqSubTimeSlotMin,double *pdReplayAcqSubTimeSlotMin,double *pdReplayAcqSubTimeSlotOptimizedForThroughput)
	//input "iDeviceId": device identifier number.
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		return pHWDeviceOEMPA->GetFMCTimeLimitation( iFMCSubCycleCount, dAscanStart, dAscanRange, dTimeSlot, *pdTimeSlotMin, *pdHWAcqSubTimeSlotMin, *pdReplayAcqSubTimeSlotMin, *pdReplayAcqSubTimeSlotOptimizedForThroughput);
	}
	__declspec(dllexport) bool mxSetFMCSubTimeSlot(int iDeviceId,int iCycle,double dFMCSubTimeSlotAcq,double dFMCSubTimeSlotReplay)
	//input "iDeviceId": device identifier number.
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);
		bool bRet=true;

		if(!pHWDeviceOEMPA)
			return false;
		if((dFMCSubTimeSlotAcq>=0.0) && !pHWDeviceOEMPA->SetFMCSubTimeSlotAcq( iCycle, dFMCSubTimeSlotAcq))
			bRet = false;
		if((dFMCSubTimeSlotReplay>=0.0) && !pHWDeviceOEMPA->SetFMCSubTimeSlotReplay( iCycle, dFMCSubTimeSlotReplay))
			bRet = false;
		return bRet;
	}
	__declspec(dllexport) bool mxSetTimeSlot(int iDeviceId,int iCycle,double dTimeSlot)
	//input "iDeviceId": device identifier number.
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA=mCHWDeviceOEMPA::GetHWDevice(iDeviceId);

		if(!pHWDeviceOEMPA)
			return false;
		return pHWDeviceOEMPA->SetTimeSlot( iCycle, dTimeSlot);
	}

	__declspec(dllexport) double *mxAllocArrayDouble(int iSize,double *pBuffer)
	{
		double *pBuffer2;
		if(iSize)
		{
			pBuffer2 = new double[iSize];
			if(pBuffer2)
			{
				for(int i=0;i<iSize;i++)
					pBuffer2[i] = 0.0;
			}
			return pBuffer2;
		}else if(pBuffer)
			delete pBuffer;
		return NULL;
	}

	__declspec(dllexport) bool mxGetTraceGlobalFileName(int iSize,wchar_t *pFilePath)
	{
		return UTKernel_GetTraceGlobalFileName(iSize,pFilePath);
	}

	__declspec(dllexport) bool mxEnableTraceGlobal(bool bEnable)
	{
		return UTKernel_EnableTraceGlobal(bEnable);
	}

	__declspec(dllexport) bool mxIsTraceGlobalEnabled()
	{
		return UTKernel_IsTraceGlobalEnabled();
	}

	__declspec(dllexport) void mxTrace(const wchar_t *pString)
	{
		UTKernel_Trace(pString);
	}

	//////////////////////////////////////////////////////////////////

	__declspec(dllexport) bool mxExample1(int iDeviceId)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->Example1();
	}

	__declspec(dllexport) bool mxExample2(int iDeviceId,int iSizeIn, double *pDataIn,int *piSizeOut, double *pDataOut)
	{
		mCHWDeviceOEMPA *mpHWDeviceOEMPA=mCHWDeviceOEMPA::mGetHWDevice(iDeviceId);

		if(!mpHWDeviceOEMPA)
			return false;
		return mpHWDeviceOEMPA->Example2(iSizeIn, pDataIn, piSizeOut, pDataOut);
	}

//////////////////////////////////////////////////////////////
//TOLLBOX MANAGEMENT

	__declspec(dllexport) bool mxIsToolboxRunning()
	{
		DWORD dwProcessId=UTKernelMatlab_GetToolboxProcessId();

		if(!dwProcessId)
			return false;
		return UTKernelMatlab_ConnectToolbox(dwProcessId);
	}
	__declspec(dllexport) bool mxToolboxRun(bool bShowWindow)
	{
		if(mxIsToolboxRunning())
		{
			if(UTKernelMatlab_ConnectToolbox(UTKernelMatlab_GetToolboxProcessId()))
				return true;
			return false;//please close all other instances.
		}
		bool bToolboxRunning = UTKernelMatlab_ToolboxRun(bShowWindow);
		return bToolboxRunning;
	}
	__declspec(dllexport) bool mxToolboxClose()
	{
		bool bRet = UTKernelMatlab_ToolboxClose();
		return bRet;
	}
	__declspec(dllexport) DWORD mxToolboxEcho(DWORD dwData)
	{
		DWORD dwRet = UTKernelMatlab_ToolboxEcho(dwData);
		return dwRet;
	}
	__declspec(dllexport) bool mxToolboxVisible(bool bEnable)
	{
		return UTKernelMatlab_ToolboxVisible(bEnable);
	}
	__declspec(dllexport) bool mxEnableEventUpdate(bool bEnable)
	{
		return UTKernelMatlab_EnableEventUpdate(bEnable);
	}
	__declspec(dllexport) bool mxToolboxDisplay(int iRootId)
	{
		return UTKernelMatlab_ToolboxDisplay(iRootId);
	}
	__declspec(dllexport) bool mxToolboxWizardNew()
	{
		return UTKernelMatlab_ToolboxWizardNew();
	}
	__declspec(dllexport) bool mxToolboxWizardDelete()
	{
		return UTKernelMatlab_ToolboxWizardDelete();
	}
	__declspec(dllexport) bool mxToolboxWizardUpdateScan()
	{
		return UTKernelMatlab_ToolboxWizardUpdateScan();
	}
	__declspec(dllexport) bool mxToolboxIsWizardEnabled()
	{
		return UTKernelMatlab_ToolboxIsWizardEnabled();
	}
	__declspec(dllexport) int mxGetWizardBeamCount()
	{
		return UTKernelMatlab_GetWizardBeamCount();
	}
	__declspec(dllexport) bool mxGetWizardBeam(int iBeamIndex,double *pdProbeX,double *pdProbeY,double *pdProbeZ,double *pdEmergenceX,double *pdEmergenceY,double *pdEmergenceZ,double *pdFocalX,double *pdFocalY,double *pdFocalZ)
	{
		return UTKernelMatlab_GetWizardBeam(iBeamIndex,*pdProbeX,*pdProbeY,*pdProbeZ,*pdEmergenceX,*pdEmergenceY,*pdEmergenceZ,*pdFocalX,*pdFocalY,*pdFocalZ);
	}
	__declspec(dllexport) int mxToolboxWizardReset()
	{
		return UTKernelMatlab_ToolboxWizardReset();
	}
	__declspec(dllexport) int mxGetWizardElementCount()
	{
		return UTKernelMatlab_GetWizardElementCount();
	}
	__declspec(dllexport) bool mxGetWizardElement(int iProbeIndex,double *pdProbeX,double *pdProbeY,double *pdProbeZ)
	{
		return UTKernelMatlab_GetWizardElement(iProbeIndex,*pdProbeX,*pdProbeY,*pdProbeZ);
	}
	__declspec(dllexport) bool mxGetToolboxParameter(wchar_t *pIn,int iSize,wchar_t *pOut)
	{
		return UTKernelMatlab_GetToolboxParameter(pIn,iSize,pOut);
	}
	__declspec(dllexport) bool mxSetToolboxParameter(wchar_t *pCmdLine)
	{
		return UTKernelMatlab_SetToolboxParameter(pCmdLine);
	}
	__declspec(dllexport) bool mxToolboxLoadWizard(wchar_t *pWizardFile)
	{
		return UTKernelMatlab_ToolboxLoadWizard(pWizardFile);
	}
	__declspec(dllexport) bool mxToolboxSaveWizard(wchar_t *pWizardFile,bool bSaveWizardResults)
	{
		return UTKernelMatlab_ToolboxSaveWizard(pWizardFile,bSaveWizardResults);
	}
	__declspec(dllexport) bool mxToolboxLoadMatrix(wchar_t *pMatrixFile)
	{
		return UTKernelMatlab_ToolboxLoadMatrix(pMatrixFile);
	}
	__declspec(dllexport) bool mxToolboxSaveMatrix(wchar_t *pMatrixFile)
	{
		return UTKernelMatlab_ToolboxSaveMatrix(pMatrixFile);
	}
	__declspec(dllexport) bool mxToolboxSaveSetup(int iDeviceId, wchar_t *pSetupFile)
	{
		return UTKernelMatlab_ToolboxSaveSetup(iDeviceId, pSetupFile);
	}
	__declspec(dllexport) bool mxToolboxSaveSetupEx(int iDeviceId, wchar_t *pSetupFileIn, wchar_t *pSetupFileOut)
	{
		return UTKernelMatlab_ToolboxSaveSetupEx(iDeviceId, pSetupFileIn, pSetupFileOut);
	}
	__declspec(dllexport) int mxGetToolboxDeviceId()
	{
		return UTKernelMatlab_ToolboxGetDeviceId();
	}
	__declspec(dllexport) int mxGetToolboxObjectId(wchar_t *pName)
	{
		return UTKernelMatlab_ToolboxGetRootId(pName);
	}
	__declspec(dllexport) bool mxSetToolboxDeviceAddress(wchar_t *pIPAddressAndPort)
	{
		return UTKernelMatlab_ToolboxDeviceAddress(pIPAddressAndPort);
	}
	__declspec(dllexport) bool mxToolboxDeviceConnection(bool bConnect)
	{
		if(bConnect && UTKernelMatlab_ToolboxIsDeviceConnected())
			return true;
		return UTKernelMatlab_ToolboxDeviceConnection(bConnect);
	}
	__declspec(dllexport) bool mxToolboxIsDeviceConnected()
	{
		return UTKernelMatlab_ToolboxIsDeviceConnected();
	}
	__declspec(dllexport) const wchar_t *mxGetToolboxDeviceType()
	{
		return UTKernel_GetToolboxDeviceType();
	}

//////////////////////////////////////////////////////////////
}

