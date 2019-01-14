// HWDlg.cpp : implementation file
//

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Resource.h"
#include "UTKernelLayoutFile.h"
#include "HWDlg.h"
#include "CustomizedDriverAPI.h"
#include "afxdialogex.h"
#include "TimerProcess.h"
#include "DlgHwLink.h"
#include "DlgCalibration.h"

#define WPARAM_RUN_TOOLBOX			123
#define LPARAM_RUN_TOOLBOX_OK		1234
#define LPARAM_RUN_TOOLBOX_ERROR	12345

typedef bool (WINAPI* pDemoCallbackInit)(int iSystem, int iCycleCount, int iPointCount);

const int g_iSystemCountMax=2;
extern pAllocDisplayCallback g_pAllocDisplayCallback;
extern CWnd *g_pMainFrame;
extern double g_dEncoder;
extern double g_dEncoderLocalAscan,g_dEncoderLocalCscan;
extern DWORD g_dwAscanSequenceLost;
extern int g_iAscanSequenceLostTimerMs;

//////////////////////////////////////////////////////////////////

extern HINSTANCE g_hInstance;
extern wchar_t g_wcFolderData[];
extern wchar_t g_wcFolderDataCfg[];

CRunToolbox g_RunToolbox;

//////////////////////////////////////////////////////////////////

wchar_t g_pDefaultString[]=L"&#@default@#&";

#ifdef _ENABLE_DISABLE_FILLING
bool g_bFillingBetweenCycle=true;
__declspec(dllexport) bool GetFillingBetweenCycle()
{
	return g_bFillingBetweenCycle;
}
bool g_bFillingLast=true;
__declspec(dllexport) bool GetFillingLast()
{
	return g_bFillingLast;
}
#endif _ENABLE_DISABLE_FILLING

long myround(double x)
{
	long nx = (long)x;
	if(x>0)
	{
		if((x-nx)>0.5)
			return nx+1;
		else
			return nx;
	}else{
		if((x-nx)<-0.5)
			return nx-1;
		else
			return nx;
	}
}

void NotifyErrorFile(wchar_t *pSection,wchar_t *pKey)
{
	wchar_t msg[MAX_PATH];

	swprintf(msg,MAX_PATH,L"Error in section '%s' for key '%s'!",pSection,pKey);
	AfxMessageBox(msg);
}

wchar_t *ReadArrayDouble(wchar_t *pValue,double &dValue)
{
	wchar_t *pStop;

	dValue = _wtof(pValue);
	pStop = wcsstr(pValue,L";");
	if(pStop)
		pStop++;
	return pStop;
}

bool AddArrayDouble(double dData,int iCountMax,double *pdData,int iCountExpected,int &iIndex)
{
	if(iIndex>=iCountMax)
		return false;
	if(iCountExpected && (iIndex>=iCountExpected))
		return false;
	pdData[iIndex++] = dData;
	return true;
}

bool FileReadInteger(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,int &iData)
{
	iData = GetPrivateProfileInt(pSection,pKey,-1,pFileName);
	if(iData==-1)
	{
		iData = GetPrivateProfileInt(pSection,pKey,0,pFileName);
		if(iData==0)
		{
			NotifyErrorFile(pSection,pKey);
			return false;
		}
	}
	return true;
}

bool FileReadDouble(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,double dUnitFactor,double &dData,bool bDisplayMessage=true);
bool FileReadDouble(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,double dUnitFactor,double &dData,bool bDisplayMessage)
{
	wchar_t data[MAX_PATH];

	GetPrivateProfileString(pSection,pKey,g_pDefaultString,data,MAX_PATH,pFileName);
	if(!wcscmp(data,g_pDefaultString))
	{
		if(bDisplayMessage)
			NotifyErrorFile(pSection,pKey);
		return false;
	}
	dData = _wtof(data);
	dData *= dUnitFactor;
	return true;
}

bool FileReadString(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,wchar_t *wcData,int iSize,bool bDisplayError=true);
bool FileReadString(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,wchar_t *wcData,int iSize,bool bDisplayError)
{
	wchar_t data[MAX_PATH];

	GetPrivateProfileString(pSection,pKey,g_pDefaultString,data,MAX_PATH,pFileName);
	if(!wcscmp(data,g_pDefaultString))
	{
		if(bDisplayError)
		{
			wsprintf(L"Error for the key \"%s\" section \"%s\"!",pKey,pSection);
			AfxMessageBox(data);
		}
		return false;
	}
	wcscpy_s(wcData,iSize,data);
	return true;
}

bool FileReadArrayDepth(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,int iCountMax,double dUnitFactor,double *pdData,int &iCountExpected)
{
	wchar_t pKeyBigData[MAX_PATH];
	wchar_t line[MAX_PATH];
	int iCount;
	wchar_t *pValue;
	double dData;

	iCount = 0;
	swprintf(pKeyBigData,MAX_PATH,L"%s",pKey);
	GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString,line,MAX_PATH,pFileName);
	if(!wcscmp(line,g_pDefaultString))
		return false;
	iCountExpected = 1;
	pValue = ReadArrayDouble(line,dData);
	dData *= dUnitFactor;
	if(pValue)
		iCountExpected++;
	while(AddArrayDouble(dData,iCountMax,pdData,iCountExpected,iCount) && pValue)
	{
		pValue = ReadArrayDouble(pValue,dData);
		dData *= dUnitFactor;
		if(pValue)
			iCountExpected++;
	}
	if(iCountExpected>iCountMax)
	{
		NotifyErrorFile(pSection,pKey);
		return false;
	}
	for(int iIndex=0;iIndex<iCountExpected-1;iIndex++)
	{
		if(pdData[iIndex]>=pdData[iIndex+1])
		{
			NotifyErrorFile(pSection,pKey);
			return false;
		}
	}
	return true;
}

wchar_t* GetEnumNext(wchar_t *pEnum)
{
	wchar_t *pValue;
	int iLength;

	pValue = pEnum;
	iLength = (int)wcslen(pValue);
	if(!pValue[iLength+1])
		return NULL;
	return &pValue[iLength+1];
}

bool FileReadEnum(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,wchar_t *pEnumList,int &iEnum,bool bDisplayErrorMsg=true);
bool FileReadEnum(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,wchar_t *pEnumList,int &iEnum,bool bDisplayErrorMsg)
{
	wchar_t *pValue;
	bool bFound=false;
	wchar_t wcAux[MAX_PATH];

	if(!FileReadString(pSection,pKey,pFileName,wcAux,MAX_PATH))
		return false;
	pValue = pEnumList;
	iEnum = 0;
	while(pValue)
	{
		if(!wcscmp(wcAux,pValue))
		{
			bFound = true;
			break;
		}
		iEnum++;
		pValue = GetEnumNext(pValue);
	}
	if(!bFound)
	{
		NotifyErrorFile(pSection,pKey);
		wcscpy_s(wcAux,MAX_PATH,L"");
		iEnum = 0;
		pValue = pEnumList;
		do{
			wcscat_s(wcAux,MAX_PATH,L"\'");
			wcscat_s(wcAux,MAX_PATH,pValue);
			wcscat_s(wcAux,MAX_PATH,L"\' ");
			wcslen(pValue);
			pValue += wcslen(pValue)+1;
			iEnum++;
		}while(pValue[0]);
		if(iEnum && bDisplayErrorMsg)
			AfxMessageBox(wcAux/*,L"Available values",MB_OK*/);
		return false;
	}
	return true;
}

void EditFile(wchar_t *pFile,bool bCloseWaiting)
{
	SHELLEXECUTEINFO execinfo;

	memset(&execinfo, 0, sizeof(execinfo));
	execinfo.lpFile = pFile;
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = L"open";
	execinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow = SW_SHOWDEFAULT;
	execinfo.lpParameters = 0;
	ShellExecuteEx(&execinfo);
	if(bCloseWaiting)
		WaitForSingleObject(execinfo.hProcess, INFINITE);
}

void CheckCarriageReturnNotepad(const wchar_t *strFileNameIn,const wchar_t *strFileNameOut)
{
	//Sometimes in ASCII saved text file it is possible to find too much 0x0D character.
	//it is better to remove those characters, see the following example:
	//0x03FE4ED5  6e 74 3d 31 0d 0d 36 0d 0d 0a 0d 0d 0d 0a 5b 53  nt=1..6.......[S
	//0x03FE4EE5  63 61 6e 5c 4c 69 6e 65 61 72 5d 0d 0d 0d 0d 0a  can\Linear].....
	//0x03FE4EF5  23 22 43 6f 75 6e 74 22 20 69 73 20 74 68 65 20  #"Count" is the 
	//0x03FE4F05  61 70 65 72 74 75 72 65 20 65 6c 65 6d 65 6e 74  aperture element
	//0x03FE4F15  20 63 6f 75 6e 74 2e 0d 0d 0d 0d 0a 43 6f 75 6e   count......Coun
	//0x03FE4F25  74 3d 31 0d 0d 36 0d 0d 0a 23 22 44 65 70 74 68  t=1..6...#"Depth
	//0x03FE4F35  45 6d 69 73 73 69 6f 6e 22 20 69 73 20 74 68 65  Emission" is the
	//0x03FE4F45  20 65 6d 69 73 73 69 6f 6e 20 64 65 70 74 68 20   emission depth 
	//0x03FE4F55  6f 66 20 61 6c 6c 20 61 70 65 72 74 75 72 65 73  of all apertures
	int iLength=0,iCount=0,iIndex,iIndex2,iSize;
	unsigned int uiLength=0;
	char *pData;

	try{
		CFile fileIn(strFileNameIn,CFile::typeBinary | CFile::modeRead);
		iLength = (int)fileIn.GetLength();
		iSize = iLength+1;
		pData = new char[iSize];
		if(pData)
		{
			memset(pData,0,iLength+1);
			uiLength = fileIn.Read(pData,iLength);
		}
		fileIn.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	if((!iLength || !uiLength) || (iLength!=uiLength))
	{
		if(pData)
			delete pData;
		return;
	}
	//modifications
	iLength = (int)strlen(pData);
	iIndex = 0;
	while(iIndex<iLength)
	{
		if(pData[iIndex]==0x0D)
		{
			iIndex2 = iIndex+1;
			while((iIndex2<iLength) && (pData[iIndex2]==0x0D))
				iIndex2++;
			if(iIndex+1<iIndex2)
			if(iIndex2<iLength)
			{
				//remove 0x0D
				if(pData[iIndex2]==0x0A)
					//keep only one (0x0D 0x0A)
					iIndex++;
				strcpy_s(&pData[iIndex],iSize,&pData[iIndex2]);
				iLength -= iIndex2-iIndex;
			}else{
				pData[iIndex] = 0;
				break;
			}
		}
		iIndex++;
	}
	uiLength = (int)strlen(pData);
	if(iLength!=uiLength)
	{
		if(pData)
			delete pData;
		return;//no modification
	}
	try{
		CFile fileOut(strFileNameOut,CFile::typeBinary | CFile::modeCreate | CFile::modeWrite);
		fileOut.Write(pData,iLength);
		fileOut.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	if(pData)
		delete pData;
}

//////////////////////////////////////////////////////////////////

int g_iSystemCount=0;
structCorrectionOEMPA *g_pCorrection=NULL;

//////////////////////////////////////////////////////////////////

void WriteCfgSector();
//bool GetEncoderStep(double &dEncoderStep);
double g_dEncoderStep=1.0e-3;
double g_dEncoderLength=100.0e-3;
DWORD g_dwEncoderResolution=1;
bool g_bCscanTofEnable=false;//fw time of flight ON.
double g_dTofMax=0x7fff;//maximum time of flight (unit digitizing period).
bool g_bGateEnable[4];//gate start (unit digitizing period).
double g_dTofStart[4];//gate start (unit digitizing period).
double g_dTofRange[4];//gate range (unit digitizing period).
__declspec(dllexport) bool GetSystem(structSystem& system)
{
	bool bRet=true;

	if(!g_iSystemCount)
		return false;
	system.dDigitizingPeriod = 1.0/g_pCorrection[0].dSamplingFrequency;//g_pCorrection[0].dDigitizingPeriod
	//bRet = GetEncoderStep(system.dEncoderStep);
	system.dEncoderStep = CHWDlg::m_dEditStep*1.0e-3;//g_dEncoderStep;
	system.dEncoderLength = CHWDlg::m_dEditLength*1.0e-3;
	system.dVelocity = g_pCorrection[0].dSpecimenVelocity;
	if(CHWDlg::m_bCheckInternal)
		system.bInternal = true;
	else
		system.bInternal = false;
	system.bCscanTofEnable = g_bCscanTofEnable;
	system.dTofMax = g_dTofMax;
	for(int iGate=0;iGate<4;iGate++)
	{
		system.bGateEnable[iGate] = g_bGateEnable[iGate];
		system.dTofStart[iGate] = g_dTofStart[iGate];
		system.dTofRange[iGate] = g_dTofRange[iGate];
	}
	return bRet;
}

__declspec(dllexport) bool GetInfo(int iSystem, structInfo& Info)
{
	if(iSystem<0)
		return false;
	if(iSystem>=g_iSystemCount)
		return false;
	Info.iCycleCount = g_pCorrection[iSystem].iCount;
	Info.PointQuantity = g_pCorrection[iSystem].piCountRecv[0];
	for(int iCycle=0;iCycle<Info.iCycleCount;iCycle++)
	{
		if(Info.pdAngle)
			Info.pdAngle[iCycle] = g_pCorrection[iSystem].pdAngleRecv[iCycle];
		if(Info.pdX)
			Info.pdX[iCycle] = g_pCorrection[iSystem].pdXRecv[iCycle];
	}
	return true;
}

__declspec(dllexport) bool DemoSystem(structSystem& system)
{
	if(!g_iSystemCount)
		return false;
	system.dDigitizingPeriod = 1.0/g_pCorrection[0].dSamplingFrequency;//g_pCorrection[0].dDigitizingPeriod
	system.dEncoderStep = 1.0e-3;
	system.dVelocity = g_pCorrection[0].dSpecimenVelocity;
	return true;
}

__declspec(dllexport) bool DemoInfo(int iSystem, structInfo& Info)
{
	if(iSystem<0)
		return false;
	if(iSystem>=g_iSystemCount)
		return false;
	Info.iCycleCount = g_pCorrection[iSystem].iCount;
	Info.PointQuantity = g_pCorrection[iSystem].piCountRecv[0];
	for(int iCycle=0;iCycle<Info.iCycleCount;iCycle++)
	{
		Info.pdAngle[iCycle] = g_pCorrection[iSystem].pdAngleRecv[iCycle];
		Info.pdX[iCycle] = g_pCorrection[iSystem].pdXRecv[iCycle];
	}
	return true;
}

long HWGetEncoderResolution();
__declspec(dllexport) long GetEncoderResolution()
{
	return HWGetEncoderResolution();
}

//////////////////////////////////////////////////////////////////

UINT g_uiUTEventMessage=0;
CHWDeviceOEMPA *g_pHW=NULL;
CUTWizardSystem g_Wizard;
CUTChannels *g_pChannel=NULL;
LARGE_INTEGER *g_pllCallbackID=NULL;
bool m_bCfgDefaultAcquisitionUnlockOn=false;
DWORD g_dwSettingId=0;
bool g_bCriticalSectionSettingId=false;
CRITICAL_SECTION g_CriticalSectionSettingId;

UINT WINAPI AcquisitionIO1_0x00010101(void *pAcquisitionParameter,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader);
UINT WINAPI AcquisitionCscan1_0x00010X02(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp,const structCscanAmpTof_0x0202 *pBufferAmpTof);
bool HWEnableEncoder(CHWDeviceOEMPA &hw,bool bEnable,bool bInternal,double dAxisStep,double dAxisLength,long lResolution);

bool IsSettingId(DWORD dwSettingId)
{
	bool bRet;

	if(!g_bCriticalSectionSettingId)
		return false;
	EnterCriticalSection(&g_CriticalSectionSettingId);
	if(g_dwSettingId==dwSettingId)
		bRet = true;
	else
		bRet = false;
	LeaveCriticalSection(&g_CriticalSectionSettingId);
	return bRet;
}
bool WINAPI CallbackSettingId(DWORD &dwSettingId)
{//call from OEMPA customized API of the driver
	if(!g_bCriticalSectionSettingId)
		return false;
	EnterCriticalSection(&g_CriticalSectionSettingId);
	g_dwSettingId++;
	dwSettingId = g_dwSettingId;
	LeaveCriticalSection(&g_CriticalSectionSettingId);
	return true;
}

bool HWGetCycleCount(CHWDeviceOEMPA &hw,int &iCycleCount,double &dGain);
structCorrectionOEMPA* WINAPI CHWDlg::CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	CHWDeviceOEMPA *pHWDeviceOEMPA=NULL;
	int iFound=-1;
	double dRange;
	CHWDeviceOEMPA *pOEMPA=(CHWDeviceOEMPA*)pAcquisitionParameter;

	if(!g_pHW)
		return NULL;
	if(eStep==eWriteHW_Leave)
	{
		EnterCriticalSection(&m_CriticalSection);
		m_iAcquisitionCycleCount = pRoot->iCycleCount;
		LeaveCriticalSection(&m_CriticalSection);
	}
	for(int iSystemIndex=0;iSystemIndex<g_iSystemCount;iSystemIndex++)
	{
		pHWDeviceOEMPA = g_pHW;
		if(pHWDeviceOEMPA==pOEMPA)
		{
			iFound = iSystemIndex;
			break;
		}
	}
	if(iFound<0)
		return NULL;
	if((eStep==eReadFileWriteHW_Leave) && !iFound)
	{
		//setup and correction have just been loaded.
		if(HWGetCycleCount(g_pHW[iFound],CHWDlg::m_iCycleCount[iFound],CHWDlg::m_dEditGain))
			CHWDlg::m_dGain[iFound] = CHWDlg::m_dEditGain;
		if(g_pAllocDisplayCallback && UTKernel_IsUserInterfaceThread())
		{
			g_pAllocDisplayCallback(true);
		}
#ifdef _EVALUATION_
		if(g_pMainFrame)
			g_pMainFrame->PostMessage(g_uiUTEventMessage,0,1);
#endif _EVALUATION_
	}
	if((eStep==eWriteHW_Enter) && pRoot && pCycle)
	{
		//called just before sending data to the hardware, so it is possible to change them.
		//but here we are interesting only to get the maximum time of flight (no modification of the data).
		g_dTofMax = 0.0;
		for(int iGate=0;iGate<4;iGate++)
		{
			g_bGateEnable[iGate] = false;
			g_dTofStart[iGate] = 0;
			g_dTofRange[iGate] = 0;
		}
		for(int iCycle=0;iCycle<pRoot->iCycleCount;iCycle++)
		{
			if(g_dTofMax<pCycle[iCycle].dStart+pCycle[iCycle].dRange)
				g_dTofMax = pCycle[iCycle].dStart+pCycle[iCycle].dRange;
			for(int iGate=0;iGate<pCycle[iCycle].iGateCount;iGate++)
			{
				g_bGateEnable[iGate] = pCycle[iCycle].gate[iGate].bEnable;
				if(!g_bGateEnable[iGate])
					continue;
				if(g_dTofStart[iGate]<pCycle[iCycle].gate[iGate].dStart)
					g_dTofStart[iGate] = pCycle[iCycle].gate[iGate].dStart;
				dRange = pCycle[iCycle].gate[iGate].dStop-pCycle[iCycle].gate[iGate].dStart;
				if(g_dTofRange[iGate]<dRange)
					g_dTofRange[iGate] = dRange;
			}
		}
		g_dTofMax /= pOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod();
		for(int iGate=0;iGate<4;iGate++)
		{
			if(g_bGateEnable[iGate])
			{
				g_dTofStart[iGate] /= pOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod();
				g_dTofRange[iGate] /= pOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod();
			}
		}
	}
	return &g_pCorrection[iFound];
}

bool Register(int iSystemIndex,CHWDeviceOEMPA &hw,CUTWizardSystem &wizard,CUTChannels &channel)
{
	bool bError=false;
	int iId;
	//int iPort;
	//wchar_t pAux[MAX_PATH];
	wchar_t pName[40];
	wchar_t pMsg[2*MAX_PATH];
	wchar_t pFileName[MAX_PATH];
	CSWDeviceOEMPA *pDeviceOEMPA;
	bool bLoadFromSectorCfgFile=false;

	wcscpy_s(pFileName,MAX_PATH,g_wcFolderDataCfg);
	wcscat_s(pFileName,MAX_PATH,L"CfgOEMPASector.txt");
	if(!g_bCriticalSectionSettingId)
	{
		InitializeCriticalSection(&g_CriticalSectionSettingId);
		g_bCriticalSectionSettingId = true;
	}
	OEMPA_SetCallbackSettingId(CallbackSettingId);
	if(!wizard.GetID(iId))
	{
		swprintf(pName,40,L"OEMPA_system");//this line is valid for mono system (creation of the object) and multisystem  (copy of the created object).
		if(!wizard.SetName(pName))
			bError = true;
		if(!wizard.GetID(iId))
			bError = true;
	}
	if(!channel.GetID(iId))
	{
		swprintf(pName,40,L"OEMPA_channel_%d",hw.GetDeviceId());
		if(!channel.SetName(pName))
			bError = true;
		if(!channel.GetID(iId))
			bError = true;
		if(!channel.LinkWizardProbe(0,&wizard,iSystemIndex))
			bError = true;
	}
	if(!hw.GetSWDevice()->SetCommunication(eEthernet))
		bError = true;
	pDeviceOEMPA = hw.GetSWDeviceOEMPA();
	if(pDeviceOEMPA)
	{
		if(!bError)
		if(!iSystemIndex)
		{
			OEMPA_SetCallbackCustomizedDriverAPI(CHWDlg::CallbackCustomizedAPI);
			if(!hw.SetAcquisitionParameter(NULL))
				bError = true;
			if(!hw.SetAcquisitionAscan_0x00010103(CHWDlg::AcquisitionAscan1_0x00010103))
				bError = true;
			if(!hw.SetAcquisitionIO_0x00010101(AcquisitionIO1_0x00010101))
				bError = true;
			if(!hw.SetAcquisitionCscan_0x00010X02(AcquisitionCscan1_0x00010X02))
				bError = true;
		}else{
			if(!hw.SetAcquisitionAscan_0x00010103(CHWDlg::AcquisitionAscan2_0x00010103))
				bError = true;
		}
	}
	if(bError)
	{
		swprintf(pMsg,MAX_PATH,L"ERROR to register system #%d !!!",iSystemIndex);
		AfxMessageBox(pMsg);
	}
	return !bError;
}

bool Unregister(int iSystemIndex,CUTWizardSystem &wizard,CUTChannels &channel)
{
	int iId;
	bool bError=false;
	wchar_t pMsg[MAX_PATH];

	if(g_bCriticalSectionSettingId)
	{
		DeleteCriticalSection(&g_CriticalSectionSettingId);
		g_bCriticalSectionSettingId = false;
	}
	if(wizard.GetID(iId))
	{
		if(!wizard.FreeID())
			bError = true;
	}
	if(channel.GetID(iId))
	{
		if(!channel.FreeID())
			bError = true;
	}
	if(bError)
	{
		swprintf(pMsg,MAX_PATH,L"ERROR to unregister system #%d !!!",iSystemIndex);
		AfxMessageBox(pMsg);
	}
	return !bError;
}

void SetDefaultAcquisitionUnlockOn(CHWDeviceOEMPA &hw,bool bEnable)
{
	if(!m_bCfgDefaultAcquisitionUnlockOn)
		return;
	hw.GetSWDevice()->SetUnlockDefaultEnablePulser(bEnable);
}

long HWGetEncoderResolution()
{
	CHWDeviceOEMPA* pHWDeviceOEMPA;

	if(g_iSystemCount<0)
		return 1;//please use unregister first.
	if(!g_pHW)
		return 1;
	pHWDeviceOEMPA = g_pHW;
	if(!pHWDeviceOEMPA)
		return 1;
	return pHWDeviceOEMPA->GetSWEncoder(0)->lGetResolution();
}

bool UnregisterSystem();
bool RegisterSystem(int iSystemCount)
{
	CHWDeviceOEMPA* pHWDeviceOEMPA;
	bool bRet=true;

	if(g_iSystemCount>0)
		return false;//please use unregister first.
	if(g_Wizard.SetCount(iSystemCount))
		return false;
	//OEMPA_SetCallbackCustomizedDriverAPI(CallbackCustomizedAPI);
#ifdef _EVALUATION_
	CUTPreference *pPreference=CUTPreference::GetCurrentPreference();
	if(pPreference)
	{//to load automatically the setup at connection time.
		pPreference->SetDefaultSetupAuto(true);
	}
#endif _EVALUATION_
	g_pHW = new CHWDeviceOEMPA[iSystemCount];
	g_pChannel = new CUTChannels[iSystemCount];
	g_pCorrection = new structCorrectionOEMPA[iSystemCount];
	g_pllCallbackID = new LARGE_INTEGER[iSystemCount];
	for(int iSystemIndex=0;iSystemIndex<iSystemCount;iSystemIndex++)
	{
		if(!OEMPA_New(g_pCorrection[iSystemIndex]))
			bRet = false;
	}
	if(!g_pHW || !g_pChannel || !g_pCorrection || !g_pllCallbackID)
		bRet = false;
	if(!bRet)
	{
		if(g_pHW)
			delete [] g_pHW;
		g_pHW = NULL;
		if(g_pChannel)
			delete [] g_pChannel;
		g_pChannel = NULL;
		if(g_pCorrection)
			delete [] g_pCorrection;
		g_pCorrection = NULL;
		if(g_pllCallbackID)
			delete [] g_pllCallbackID;
		g_pllCallbackID = NULL;
		return false;
	}
	g_iSystemCount = iSystemCount;
	for(int iSystemIndex=0;iSystemIndex<iSystemCount;iSystemIndex++)
	{
		if(!Register(iSystemIndex,g_pHW[iSystemIndex],g_Wizard,g_pChannel[iSystemIndex]))
			bRet = false;
		g_pllCallbackID[iSystemIndex].QuadPart = 0xffffffffffffffff;
		if(bRet)
			g_pChannel[iSystemIndex].GetProbe(0)->Scan(0).UpdateScan();
		pHWDeviceOEMPA = &g_pHW[iSystemIndex];
		if(pHWDeviceOEMPA)
		{
			m_bCfgDefaultAcquisitionUnlockOn = pHWDeviceOEMPA->GetSWDevice()->GetUnlockDefaultEnablePulser();
			if(m_bCfgDefaultAcquisitionUnlockOn)
				SetDefaultAcquisitionUnlockOn(g_pHW[iSystemIndex],false);
		}else
			m_bCfgDefaultAcquisitionUnlockOn = false;
	}
	if(!bRet)
		UnregisterSystem();
	return bRet;
}

bool UnregisterSystem()
{
	bool bRet=true;

	if(!g_iSystemCount)
		return true;//please use unregister first.
	if(!g_pHW && !g_pChannel)
		return true;
	for(int iSystemIndex=0;iSystemIndex<g_iSystemCount;iSystemIndex++)
	{
		if(!Unregister(iSystemIndex,g_Wizard,g_pChannel[iSystemIndex]))
			bRet = false;
		OEMPA_Free(g_pCorrection[iSystemIndex]);
	}
	if(g_pHW)
		delete [] g_pHW;
	g_pHW = NULL;
	if(g_pChannel)
		delete [] g_pChannel;
	g_pChannel = NULL;
	if(g_pCorrection)
		delete [] g_pCorrection;
	g_pCorrection = NULL;
	if(g_pllCallbackID)
		delete [] g_pllCallbackID;
	g_pllCallbackID = NULL;
	g_iSystemCount = 0;
	return bRet;
}

bool HWUpdate(	bool bUpdateChannelWizard,CHWDeviceOEMPA &hw,CUTChannels &channel,structCorrectionOEMPA *pCorrection)
				//int iCscanCount,double dCscanStart[4],double dCscanRange[4])
{
	CHWDeviceOEMPA* pHWDeviceOEMPA;
	int iWizardSystemId,iWizardChannelId;
	bool bRet=true;
	structRoot root;
	structCycle *pCycle=NULL;
	CFocalLaw *pEmission=NULL;
	CFocalLaw *pReception=NULL;
	long lPointFactor=-2;
	enumCompressionType eCompressionType;

	if(!g_Wizard.GetID(iWizardSystemId))
		return false;
	if(!channel.GetID(iWizardChannelId))
		return false;
	if(bUpdateChannelWizard && !channel.Probe().Scan().UpdateScan())
		return false;
	pHWDeviceOEMPA = &hw;
	if(!pHWDeviceOEMPA)
		return false;
	CWaitCursor wait;

	if(!OEMPA_ReadWizard(pHWDeviceOEMPA,&g_Wizard,&channel,root,pCycle,pEmission,pReception))
	{
		AfxMessageBox(L"Error from the wizard!");
		bRet = false;goto end;
	}

	//for(int iGate=0;iGate<iCscanCount;iGate++)
	//{
	//	for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
	//	{
	//		pCycle[iCycle].gate[iGate].bEnable = true;
	//		pCycle[iCycle].gate[iGate].dStart;
	//		pCycle[iCycle].gate[iGate].dStop;
	//		pCycle[iCycle].gate[iGate].dThreshold;
	//		pCycle[iCycle].gate[iGate].dStart;
	//		pCycle[iCycle].gate[iGate].dStart;
	//	}
	//}
	
	pCorrection->dDigitizingPeriod = root.dDigitizingPeriod;
	pCorrection->dSpecimenVelocity = root.dSpecimenVelocity;
	pCorrection->iCount = 0;
	pCorrection->dSamplingFrequency = 1.0/root.dDigitizingPeriod;//1.1.5.0n required for button "wiz=>hw"

	if(pCorrection && (pCorrection->iAllocCount>0) &&
		(root.iCycleCount>=0))//no need to change correction if root.iCycleCount==-1
	{
		pCorrection->iCount = root.iCycleCount;
		for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
		{
			if(fabs(pEmission[iCycle].dSkew)<M_PI/4)
				pCorrection->pdAnglePulse[iCycle] = pEmission[iCycle].dAngle;
			else
				pCorrection->pdAnglePulse[iCycle] = -pEmission[iCycle].dAngle;
			if(fabs(pReception[iCycle].dSkew)<M_PI/4)
				pCorrection->pdAngleRecv[iCycle] = pReception[iCycle].dAngle;
			else
				pCorrection->pdAngleRecv[iCycle] = -pReception[iCycle].dAngle;
			pCorrection->piCountRecv[iCycle] = pCycle[iCycle].lPointCount;
			if(!pCorrection->piCountRecv[iCycle])
				pCorrection->piCountRecv[iCycle] = myround(pCycle[iCycle].dRange/pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod())+1;
		}
	}

	if(!OEMPA_WriteHW(pHWDeviceOEMPA,root,pCycle,pEmission,pReception))
	{
		AfxMessageBox(L"Error to write HW!");
		bRet = false;goto end;
	}

	if(pCorrection && (pCorrection->iAllocCount>0) &&
		(root.iCycleCount>=0))//no need to change correction if root.iCycleCount==-1
	{
		//1.1.5.0n required for button "wiz=>hw"
		for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
		{
			if((lPointFactor!=-2) && ((pCycle[iCycle].lPointFactor != lPointFactor) || (pCycle[iCycle].eCompressionType!=eCompressionType)))
			{
				//all cycle dont have same values.
				lPointFactor = -1;
				eCompressionType = eCompression;
			}else{
				lPointFactor = pCycle[iCycle].lPointFactor;
				eCompressionType = pCycle[iCycle].eCompressionType;
			}
		}
		if(lPointFactor != -2)
		{
			if(!pHWDeviceOEMPA->GetSamplingFrequency(eCompressionType,lPointFactor,pCorrection->dSamplingFrequency))
				pCorrection->dSamplingFrequency = 1.0/pCorrection->dDigitizingPeriod;
		}
	}
end:
	OEMPA_Free(pCycle,pEmission,pReception);

	return bRet;
}

bool HWGetCycleCount(CHWDeviceOEMPA &hw,int &iCycleCount,double &dGain)
{
	bool bRet=true;
	int iCycleCount2;
	CHWDeviceOEMPA *pHWDeviceOEMPA=&hw;

	if(!pHWDeviceOEMPA)
		return false;
	if(!pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	bRet = pHWDeviceOEMPA->GetCycleCount(&iCycleCount2);
	bRet = pHWDeviceOEMPA->GetGainDigital(0,&dGain) && bRet;
	pHWDeviceOEMPA->UnlockDevice();
	if(iCycleCount2>=0)
		iCycleCount = iCycleCount2;
	//here iCycleCount is updated.
	return bRet;
}

bool HWSetGain(CHWDeviceOEMPA &hw,double dGain)
{
	bool bRet=true;
	CHWDeviceOEMPA* pHWDeviceOEMPA;
	int iCycleCount;

	pHWDeviceOEMPA = &hw;
	if(!pHWDeviceOEMPA)
		return false;
	if(!pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	CallbackSettingId(g_dwSettingId);
	if(!pHWDeviceOEMPA->SetSettingId(g_dwSettingId))
		bRet = false;
	bRet = pHWDeviceOEMPA->GetCycleCount(&iCycleCount);
	if(!pHWDeviceOEMPA->Flush())
		bRet = false;
	for(int iCycle=0;iCycle<iCycleCount;iCycle++)
		bRet = pHWDeviceOEMPA->SetGainDigital(iCycle,dGain) && bRet;
	if(!pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	return bRet;
}

bool HWResetEncoder(int iSystemIndex,CHWDeviceOEMPA &hw)
{
	bool bRet=true;
	CHWDeviceOEMPA* pHWDeviceOEMPA;

	pHWDeviceOEMPA = &hw;
	if(!pHWDeviceOEMPA)
		return false;
	if(!pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	if(!pHWDeviceOEMPA->SetSettingId(g_dwSettingId))
		bRet = false;
	for(int iEncoderIndex=0;iEncoderIndex<g_iOEMPAEncoderCountMax;iEncoderIndex++)
	{
		if(!pHWDeviceOEMPA->ResetEncoder(iEncoderIndex))
			bRet = false;
	}
	if(!pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	return bRet;
}

bool HWGetTemperatureSensor(int iSystemIndex,CHWDeviceOEMPA &hw,int iBoardCount[g_iSystemCountMax],WORD wTemperature[1+g_iSystemCountMax][(1+g_iOEMPABoardCountMax)*2][g_iOEMPARXBoardTemperatureSensorCountMax])
{
	bool bRet=true;
	CHWDeviceOEMPA* pHWDeviceOEMPA;
	int iSensorCountMax;

	pHWDeviceOEMPA = &hw;
	if(!pHWDeviceOEMPA)
		return false;
	if(!pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	iBoardCount[iSystemIndex] = pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetRXBoardCount();
	for(int iIndexBoard=0;iIndexBoard<1+iBoardCount[iSystemIndex];iIndexBoard++)//1 for the ComBoard.
	{
		if(!iIndexBoard)
			iSensorCountMax = g_iOEMPAComBoardTemperatureSensorCountMax;
		else
			iSensorCountMax = g_iOEMPARXBoardTemperatureSensorCountMax;
		for(int iIndexSensor=0;iIndexSensor<iSensorCountMax;iIndexSensor++)
		{
			if(!pHWDeviceOEMPA->GetTemperatureSensor(iIndexBoard,iIndexSensor,&wTemperature[iSystemIndex][iIndexBoard][iIndexSensor]))
				bRet = false;
		}
	}
	if(!pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	return bRet;
}

bool HWEnableShot(CHWDeviceOEMPA &hw,bool bEnable)
{
	bool bRet=true;
	enumAcquisitionState eAcqState;
	CHWDeviceOEMPA* pHWDeviceOEMPA;

	pHWDeviceOEMPA = &hw;
	if(bEnable)
		eAcqState = eAcqOn;
	else
		eAcqState = eAcqOff;
	if(!pHWDeviceOEMPA->LockDevice(eAcqState))
		return false;
	pHWDeviceOEMPA->UnlockDevice(eAcqState);
	return bRet;
}

bool HWUpdateEnable(CHWDeviceOEMPA &hw,bool &bEnabled)
{
	CHWDeviceOEMPA* pHWDeviceOEMPA;

	pHWDeviceOEMPA = &hw;
	if(!pHWDeviceOEMPA)
		return false;
	bEnabled = pHWDeviceOEMPA->GetSWDevice()->IsPulserEnabled();//No need to call "LockDriver" and "UnlockDriver"
	return true;
}

bool HWEnableEncoder(CHWDeviceOEMPA &hw,bool bEnable,bool bInternal,double dAxisStep,double dAxisLength,long lResolution)
{
	bool bRet=true,bSlave=false;
	CHWDeviceOEMPA* pHWDeviceOEMPA;
	enumOEMPATrigger eTrig;
	enumOEMPARequestIO eReqIO;

	pHWDeviceOEMPA = &hw;
	if(!pHWDeviceOEMPA)
		return false;
	if(!pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsIOBoardEnabled() && !bInternal)
		bInternal = true;
	if(pHWDeviceOEMPA->GetHardwareLink()==eSlave)
		bSlave = true;
	if(!pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	if(bEnable && !bInternal && !bSlave)
	{
		if(!pHWDeviceOEMPA->GetSWEncoder(0)->lSetResolution(lResolution))
			bRet = false;
		if(!pHWDeviceOEMPA->SetEncoderWire1(0,eDigitalInput01))
			bRet = false;
		if(!pHWDeviceOEMPA->SetEncoderWire2(0,eDigitalInput02))
			bRet = false;
	}
	if(bEnable && !bInternal && !bSlave)
	{
		eReqIO = eOEMPAOnDigitalInputAndCycle;
		eTrig = eOEMPAEncoder;
	}else{
		eReqIO = eOEMPAOnDigitalInputOnly;
		eTrig = eOEMPAInternal;
		if(bSlave)
			eTrig = eOEMPAExternalCycleSequence;
	}
	if(!pHWDeviceOEMPA->SetRequestIO(eReqIO))
		bRet = false;
	if(!pHWDeviceOEMPA->SetTriggerMode(eTrig))
		bRet = false;
	if(bEnable && !bInternal && !bSlave && !pHWDeviceOEMPA->SetTriggerEncoderStep(dAxisStep))
		bRet = false;
	if(!pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	g_dEncoderStep = dAxisStep;
	g_dEncoderLength = dAxisLength;
	return bRet;
}

bool WizardLoad(CHWDeviceOEMPA &hw,CUTWizardSystem &wizard,CUTChannels &channel,CString &strFileName)
{
	int iRootWizardSystemId=-1,iRootChannelId=-1;
	bool bRet=true;
	CUTKernelFile file;
	wchar_t pRootName[MAX_PATH];
	wchar_t pRootNameExpected[MAX_PATH];
	enumRootType eRootID;

	if(!file.LoadPrepare(strFileName.GetBuffer(),false))
		return false;
	if(file.GetCount()!=2)
		return false;
	if(!file.GetObject(0,eRootID))
		return false;
	if((eRootID!=eRootTypeWizardSystem) && (eRootID!=eRootTypeChannels))
		return false;
	if(eRootID==eRootTypeWizardSystem)
		iRootWizardSystemId = 0;
	else
		iRootChannelId = 0;
	if(!file.GetObject(1,eRootID))
		return false;
	if(!iRootWizardSystemId && (eRootID!=eRootTypeChannels))
		return false;
	if(!iRootChannelId && (eRootID!=eRootTypeWizardSystem))
		return false;
	if(eRootID==eRootTypeWizardSystem)
		iRootWizardSystemId = 1;
	else
		iRootChannelId = 1;
	if(!file.GetObject(iRootWizardSystemId,MAX_PATH,pRootName))
		return false;
	if(wcsstr(pRootName,L"OEMPA_system")!=pRootName)
		return false;
	if(!file.GetObject(iRootChannelId,MAX_PATH,pRootName))
		return false;
	swprintf(pRootNameExpected,MAX_PATH,L"OEMPA_channel_%d",hw.GetDeviceId());
	//if((wcsstr(pRootName,L"OEMPA_channel")!=pRootName) && (wcsstr(pRootName,L"OEMPA_channel2")!=pRootName))
	if(wcsstr(pRootName,pRootNameExpected)!=pRootName)
		return false;
	if(!file.SetObject(iRootWizardSystemId,true))
		return false;
	if(!file.SetObject(iRootChannelId,true))
		return false;
	return file.LoadComplete();
}

bool WizardSave(CUTWizardSystem &wizard,CUTChannels &channel,CString strFile)
{
	CString strFileName;
	CUTKernelFile file;

	strFileName = strFile;
	if(!file.AddObject(&wizard,true))
		return false;
	if(!file.AddObject(&channel,true))
		return false;
	return file.Save(strFileName.GetBuffer());
}

void WizardWriteEnum(CStdioFile &file,wchar_t *pKeyName,int iIndex,wchar_t *pEnumList,int iEnum)
{
	wchar_t *pValue;
	bool bFound=false;
	CString string;

	pValue = pEnumList;
	while(iEnum && pValue)
	{
		iEnum--;
		pValue = GetEnumNext(pValue);
	}
	if(!pValue)
		return;
	string.Format(L"%s[%d]=%s\n",pKeyName,iIndex,pValue);
	file.WriteString(string);
}

void WizardWriteEnumList(CStdioFile &file,wchar_t *pEnumList)
{
	wchar_t *pValue;
	CString string;
	int iCount=0;

	string = L"#Available values: ";
	pValue = pEnumList;
	while(pValue)
	{
		if(iCount)
			string += L", \"";
		else
			string += L"\"";
		string += pValue;
		string += L"\"";
		iCount++;
		pValue = GetEnumNext(pValue);
	}
	string += L"\n";
	file.WriteString(string);
}

bool WizardSectorial(CUTWizardSystem *m_pWizardSystem,CUTChannels *m_pChannel,
					double &dAscanStart,double &dAscanRange,double &dTimeSlot,
					int &iCscanCount,double adCscanStart[4],double adCscanRange[4],double adCscanThreshold[4])
{
	bool bLinear=false;
	CString strFileName,string;
	wchar_t wcWave[50];
	int iWedgeEnable;
	bool bLongitudinalWave;double dSpecimenVelocity;
	int iProbeElementCount;double dProbePitch,dProbeFrequency;
	bool bWedgeEnable;
	double dWedgeHeight,dWedgeAngle,dWedgeVelocity;
	double dScanDepthEmission,adScanDepthReception[32];
	int iScanDepthReception,iScanElementCount;
	int iScanElementStart,iScanElementStop,iScanElementStep;
	double dScanAngleStart,dScanAngleStop,dScanAngleStep;
	double dSpecimenRadius,dProbeRadius,dAux;
	enumRectification aeRectification[4];
	enumGateModeAmp aeModeAmp[4];
	enumGateModeTof aeModeTof[4];
	enumDepthMode eDepthMode;
	int iEnum;
	bool bExist;
	wchar_t wcDepthMode[20];

	if(!OEMPA_ReadWizard(	m_pWizardSystem,m_pChannel,
					bLongitudinalWave,dSpecimenVelocity,
					iProbeElementCount,dProbePitch,dProbeFrequency,
					bWedgeEnable,
					dWedgeHeight,dWedgeAngle,dWedgeVelocity,
					dScanDepthEmission,32,adScanDepthReception,iScanDepthReception,
					iScanElementCount,
					iScanElementStart,iScanElementStop,iScanElementStep,
					dScanAngleStart,dScanAngleStop,dScanAngleStep,
					dAscanStart,dAscanRange,dTimeSlot,
					4,adCscanStart,adCscanRange,adCscanThreshold,
					aeRectification,aeModeAmp,aeModeTof,iCscanCount,
					eDepthMode))
		return false;
	//1.1.5.1r start
	switch(m_pWizardSystem->Specimen().GetSpecimen())
	{
	case ePlane: dSpecimenRadius = 0.0;break;
	case eDisk:
	case eBar:
	case eTube: m_pWizardSystem->Specimen().GetRadius(dSpecimenRadius);break;
	}
	if(!m_pWizardSystem->Probe(0).GetRadius(dProbeRadius))
		dProbeRadius = 0.0;
	//1.1.5.1r stop
	iScanElementStart++;
	iScanElementStop++;
	strFileName = g_wcFolderData;
	strFileName += L"Wizard.txt";
	bExist = false;
	if(bLinear && FileReadDouble(L"Scan\\Linear",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission,false))
		bExist = true;
	if(!bLinear && FileReadDouble(L"Scan\\Sectorial",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission,false))
		bExist = true;
	//1.1.5.1r start
	if(!FileReadDouble(L"Specimen",L"Radius",strFileName.GetBuffer(),1.0e-3,dAux,false))
		bExist = false;
	//1.1.5.1r stop
    if (!bExist)
	{
		try{
			CStdioFile file(strFileName,CFile::typeText | CFile::modeWrite | CFile::modeCreate);
			if(bLinear)
				file.WriteString(L"# TEMPLATE - SINGLE-CHANNEL - LINEAR\n");
			else
				file.WriteString(L"# TEMPLATE - SINGLE-CHANNEL - SECTORIAL\n");
			file.WriteString(L"#Fill in following input parameters, save the file and\n");
			file.WriteString(L"#close notepad to go on!\n");
			file.WriteString(L"\n");
			file.WriteString(L"[Specimen]\n");
			file.WriteString(L"#\"Velocity\" is the velocity in the specimen (unit=meter/sec).\n");
			string.Format(L"Velocity=%.1lf m/s\n",dSpecimenVelocity);
			file.WriteString(string);
			file.WriteString(L"#\"Wave\" is the wave ('L' for Longitudinal and 'T' for Transverse).\n");
			if(bLongitudinalWave)
				string.Format(L"Wave=L\n");
			else
				string.Format(L"Wave=T\n");
			file.WriteString(string);
			//1.1.5.1r start
			file.WriteString(L"#\"Radius\" is 0.0 for a plane (unit=millimeter).\n");
			string.Format(L"Radius=%.1lf mm\n",dSpecimenRadius*1000.0);
			file.WriteString(string);
			//1.1.5.1r stop
			file.WriteString(L"\n");
			file.WriteString(L"[Wedge]\n");
			file.WriteString(L"#\"Enable\" should be used if the wedge is used (0 to disable or 1 to enable).\n");
			if(bWedgeEnable)
				string.Format(L"Enable=1\n");
			else
				string.Format(L"Enable=0\n");
			file.WriteString(string);
			file.WriteString(L"#\"Velocity\" is the velocity in the wedge (unit=meter/sec).\n");
			string.Format(L"Velocity=%.1lf m/s\n",dWedgeVelocity);
			file.WriteString(string);
			file.WriteString(L"#\"Height\" is the probe height over the specimen for the middle of the probe (unit=milliter).\n");
			string.Format(L"Height=%.1lf mm\n",dWedgeHeight*1.0e3);
			file.WriteString(string);
			file.WriteString(L"#\"Angle\" is the probe angle over the specimen (unit=degree).\n");
			string.Format(L"Angle=%.1lf deg\n",dWedgeAngle*180.0/M_PI);
			file.WriteString(string);
			file.WriteString(L"\n");
			file.WriteString(L"[Probe]\n");
			file.WriteString(L"#\"ElementCount\" is the element count (linear phased array).\n");
			string.Format(L"ElementCount=%d\n",iProbeElementCount);
			file.WriteString(string);
			file.WriteString(L"#\"Pitch\" is the element pitch.\n");
			string.Format(L"Pitch=%.1lf mm\n",dProbePitch*1.0e3);
			file.WriteString(string);
			file.WriteString(L"#\"Frequency\" is the frequency (used to set the pulse width) unit is Megahertz.\n");
			string.Format(L"Frequency=%.2lf MHz\n",dProbeFrequency*1.0e-6);
			file.WriteString(string);
			//1.1.5.1r start
			file.WriteString(L"#\"Radius\" is 0.0 for a plane probe (unit=millimeter).\n");
			string.Format(L"Radius=%.1lf mm\n",dProbeRadius*1000.0);
			file.WriteString(string);
			//1.1.5.1r stop
			file.WriteString(L"\n");
			if(bLinear)
				file.WriteString(L"[Scan\\Linear]\n");
			else
				file.WriteString(L"[Scan\\Sectorial]\n");
			file.WriteString(L"#\"Count\" is the aperture element count.\n");
			string.Format(L"Count=%d\n",iScanElementCount);
			file.WriteString(string);
			file.WriteString(L"#\"DepthEmission\" is the emission depth of all apertures (unit=millimeter).\n");
			string.Format(L"DepthEmission=%.1lf mm\n",dScanDepthEmission*1.0e3);
			file.WriteString(string);
			file.WriteString(L"#\"DepthReception\" multiples depths is possible for DDF (unit=millimeter).\n");
			file.WriteString(L"#\"DepthReception=60.0;80.0;100.0 mm\" <= DDF example\n");
			file.WriteString(L"DepthReception=");
			for(int iIndex=0;iIndex<iScanDepthReception;iIndex++)
			{
				if(iIndex==iScanDepthReception-1)
					string.Format(L"%.1lf",adScanDepthReception[iIndex]*1.0e3);
				else
					string.Format(L"%.1lf;",adScanDepthReception[iIndex]*1.0e3);
				file.WriteString(string);
			}
			file.WriteString(L" mm\n");
			if(bLinear)
			{
				file.WriteString(L"#\"Angle\" is the angle of all apertures (unit=degree).\n");
				string.Format(L"Angle=%.1lf deg\n",dScanAngleStart*180.0/M_PI);
				file.WriteString(string);
				file.WriteString(L"#\"ElementStart\" is the first element of the first aperture.\n");
				string.Format(L"ElementStart=%d\n",iScanElementStart);
				file.WriteString(string);
				file.WriteString(L"#\"ElementStop\" is the first element of the last aperture.\n");
				string.Format(L"ElementStop=%d\n",iScanElementStop);
				file.WriteString(string);
				file.WriteString(L"#\"ElementStep\" is the element step.\n");
				string.Format(L"ElementStep=%d\n",iScanElementStep);
				file.WriteString(string);
				file.WriteString(L"#\"DepthMode\" could be \"TrueDepth\", \"SoundPath\" or \"TrueDepthBigBar\".\n");
				switch(eDepthMode)
				{
				case eStandardDepth:	string.Format(L"DepthMode=TrueDepth\n");break;
				case eSoundPath:		string.Format(L"DepthMode=SoundPath\n");break;
				case eDepthBigBar:		string.Format(L"DepthMode=TrueDepthBigBar\n");break;
				default: return false;
				}
				file.WriteString(string);
			}else{
				file.WriteString(L"#\"Element\" is the first element of all apertures.\n");
				string.Format(L"Element=%d\n",iScanElementStart);
				file.WriteString(string);
				file.WriteString(L"#\"AngleStart\" is the angle of the first aperture (unit=degree).\n");
				string.Format(L"AngleStart=%.1lf deg\n",dScanAngleStart*180.0/M_PI);
				file.WriteString(string);
				file.WriteString(L"#\"AngleStop\" is the angle of the last aperture (unit=degree).\n");
				string.Format(L"AngleStop=%.1lf deg\n",dScanAngleStop*180.0/M_PI);
				file.WriteString(string);
				file.WriteString(L"#\"AngleStep\" is the angle step (unit=degree).\n");
				string.Format(L"AngleStep=%.1lf deg\n",dScanAngleStep*180.0/M_PI);
				file.WriteString(string);
				file.WriteString(L"#\"DepthMode\" could be \"TrueDepth\", \"SoundPath\" or \"TrueDepthBigBar\".\n");
				switch(eDepthMode)
				{
				case eStandardDepth:	string.Format(L"DepthMode=TrueDepth\n");break;
				case eSoundPath:		string.Format(L"DepthMode=SoundPath\n");break;
				case eDepthBigBar:		string.Format(L"DepthMode=TrueDepthBigBar\n");break;
				default: return false;
				}
				file.WriteString(string);
			}
			file.WriteString(L"\n[Ascan]\n");
			file.WriteString(L"#\"Start\" is the digitizing start time in microsecond.\n");
			string.Format(L"Start=%.3lf 탎\n",dAscanStart*1.0e6);
			file.WriteString(string);
			file.WriteString(L"#\"Range\" is the digitizing range in microsecond.\n");
			string.Format(L"Range=%.3lf 탎\n",dAscanRange*1.0e6);
			file.WriteString(string);
			file.WriteString(L"#\"TimeSlot\" is the time between two pulse shot in microsecond.\n");
			string.Format(L"TimeSlot=%.3lf 탎\n",dTimeSlot*1.0e6);
			file.WriteString(string);

			file.WriteString(L"\n[Cscan]\n");
			file.WriteString(L"#\"Count\" is the Cscan gate count (range is 0 to 4).\n");
			string.Format(L"Count=%d\n",iCscanCount);
			file.WriteString(string);
			file.WriteString(L"#\"Start[0]\" is the start time in microsecond for the first Cscan gate.\n");
			for(int iGate=0;iGate<4;iGate++)
			{
				string.Format(L"Start[%d]=%.3lf 탎\n",iGate,adCscanStart[iGate]*1.0e6);
				file.WriteString(string);
			}
			file.WriteString(L"#\"Range[0]\" is the range in microsecond for the first Cscan gate.\n");
			for(int iGate=0;iGate<4;iGate++)
			{
				string.Format(L"Range[%d]=%.3lf 탎\n",iGate,adCscanRange[iGate]*1.0e6);
				file.WriteString(string);
			}
			file.WriteString(L"#\"Threshold[0]\" is the threshold in percent for the first Cscan gate.\n");
			for(int iGate=0;iGate<4;iGate++)
			{
				string.Format(L"Threshold[%d]=%.1lf %%\n",iGate,adCscanThreshold[iGate]);
				file.WriteString(string);
			}
			file.WriteString(L"#\"Rectification[0]\" is the Rectification mode for the first Cscan gate.\n");
			WizardWriteEnumList(file,L"Signed"L"\0"L"Unsigned"L"\0"L"UnsignedPositive"L"\0"L"UnsignedNegative"L"\0");
			for(int iGate=0;iGate<4;iGate++)
			{
				WizardWriteEnum(file,L"Rectification",iGate,L"Signed"L"\0"L"Unsigned"L"\0"L"UnsignedPositive"L"\0"L"UnsignedNegative"L"\0",(int)aeRectification[iGate]);
			}
			file.WriteString(L"#\"ModeAmp[0]\" is the Amplitude mode for the first Cscan gate.\n");
			WizardWriteEnumList(file,L"Absolute"L"\0"L"Maximum"L"\0"L"Minimum"L"\0"L"PeakToPeak"L"\0");
			for(int iGate=0;iGate<4;iGate++)
			{
				WizardWriteEnum(file,L"ModeAmp",iGate,L"Absolute"L"\0"L"Maximum"L"\0"L"Minimum"L"\0"L"PeakToPeak"L"\0",(int)aeModeAmp[iGate]);
			}
			file.WriteString(L"#\"ModeTof[0]\" is the TimeOfFlight mode for the first Cscan gate.\n");
			WizardWriteEnumList(file,L"AmplitudeDetection"L"\0"L"ThresholdCross"L"\0"L"ZeroFirstAfterThresholdCross"L"\0"L"ZeroLastBeforeThresholdCross"L"\0");
			for(int iGate=0;iGate<4;iGate++)
			{
				WizardWriteEnum(file,L"ModeTof",iGate,L"AmplitudeDetection"L"\0"L"ThresholdCross"L"\0"L"ZeroFirstAfterThresholdCross"L"\0"L"ZeroLastBeforeThresholdCross"L"\0",(int)aeModeTof[iGate]);
			}

			file.Close();
		}catch(CFileException *e){
			e->ReportError();
			e->Delete();
		}
	}
	EditFile(strFileName.GetBuffer(),true);
	CheckCarriageReturnNotepad(strFileName,strFileName);
	if(!FileReadDouble(L"Specimen",L"Velocity",strFileName.GetBuffer(),1.0,dSpecimenVelocity))
		return false;
	if(!FileReadString(L"Specimen",L"Wave",strFileName.GetBuffer(),wcWave,50))
		return false;
	if(wcWave[0]==L'L')
		bLongitudinalWave = true;
	else if(wcWave[0]==L'T')
		bLongitudinalWave = false;
	else
		return false;
	//1.1.5.1r start
	if(!FileReadDouble(L"Specimen",L"Radius",strFileName.GetBuffer(),1.0e-3,dSpecimenRadius))
		return false;
	//1.1.5.1r stop
	if(!FileReadInteger(L"Wedge",L"Enable",strFileName.GetBuffer(),iWedgeEnable))
		return false;
	if(iWedgeEnable)
		bWedgeEnable = true;
	else
		bWedgeEnable = false;
	if(!FileReadDouble(L"Wedge",L"Velocity",strFileName.GetBuffer(),1.0,dWedgeVelocity))
		return false;
	if(!FileReadDouble(L"Wedge",L"Height",strFileName.GetBuffer(),1.0e-3,dWedgeHeight))
		return false;
	if(!FileReadDouble(L"Wedge",L"Angle",strFileName.GetBuffer(),M_PI/180.0,dWedgeAngle))
		return false;
	if(!FileReadInteger(L"Probe",L"ElementCount",strFileName.GetBuffer(),iProbeElementCount))
		return false;
	if(!FileReadDouble(L"Probe",L"Pitch",strFileName.GetBuffer(),1.0e-3,dProbePitch))
		return false;
	if(!FileReadDouble(L"Probe",L"Frequency",strFileName.GetBuffer(),1.0e6,dProbeFrequency))
		return false;
	//1.1.5.1r start
	if(!FileReadDouble(L"Probe",L"Radius",strFileName.GetBuffer(),1.0e-3,dProbeRadius))
		return false;
	//1.1.5.1r stop
	if(bLinear)
	{
		if(!FileReadDouble(L"Scan\\Linear",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission))
			return false;
		if(!FileReadArrayDepth(L"Scan\\Linear",L"DepthReception",strFileName.GetBuffer(),32,1.0e-3,adScanDepthReception,iScanDepthReception))
			return false;
		if(!FileReadInteger(L"Scan\\Linear",L"Count",strFileName.GetBuffer(),iScanElementCount))
			return false;
		if(!FileReadDouble(L"Scan\\Linear",L"Angle",strFileName.GetBuffer(),M_PI/180.0,dScanAngleStart))
			return false;
		if(!FileReadInteger(L"Scan\\Linear",L"ElementStart",strFileName.GetBuffer(),iScanElementStart))
			return false;
		iScanElementStart--;
		if(!FileReadInteger(L"Scan\\Linear",L"ElementStop",strFileName.GetBuffer(),iScanElementStop))
			return false;
		iScanElementStop--;
		if(!FileReadInteger(L"Scan\\Linear",L"ElementStep",strFileName.GetBuffer(),iScanElementStep))
			return false;
		if(!iScanElementStep)
		{
			AfxMessageBox(L"Error element step canno't be null!");
			return false;
		}
		if(!FileReadString(L"Scan\\Linear",L"DepthMode",strFileName.GetBuffer(),wcDepthMode,20,false))
			wcscpy_s(wcDepthMode,L"TrueDepth");
	}else{
		if(!FileReadDouble(L"Scan\\Sectorial",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission))
			return false;
		if(!FileReadArrayDepth(L"Scan\\Sectorial",L"DepthReception",strFileName.GetBuffer(),32,1.0e-3,adScanDepthReception,iScanDepthReception))
			return false;
		if(!FileReadInteger(L"Scan\\Sectorial",L"Count",strFileName.GetBuffer(),iScanElementCount))
			return false;
		if(!FileReadInteger(L"Scan\\Sectorial",L"Element",strFileName.GetBuffer(),iScanElementStart))
			return false;
		iScanElementStart--;
		if(!FileReadDouble(L"Scan\\Sectorial",L"AngleStart",strFileName.GetBuffer(),M_PI/180.0,dScanAngleStart))
			return false;
		if(!FileReadDouble(L"Scan\\Sectorial",L"AngleStop",strFileName.GetBuffer(),M_PI/180.0,dScanAngleStop))
			return false;
		if(!FileReadDouble(L"Scan\\Sectorial",L"AngleStep",strFileName.GetBuffer(),M_PI/180.0,dScanAngleStep))
			return false;
		if(!FileReadString(L"Scan\\Sectorial",L"DepthMode",strFileName.GetBuffer(),wcDepthMode,20,false))
			wcscpy_s(wcDepthMode,L"TrueDepth");
	}
	if(!wcscmp(wcDepthMode,L"TrueDepth"))
		eDepthMode = eStandardDepth;
	else
	if(!wcscmp(wcDepthMode,L"SoundPath"))
		eDepthMode = eSoundPath;
	else
	if(!wcscmp(wcDepthMode,L"TrueDepthBigBar"))
	{
		if(dSpecimenRadius>0.0)
			eDepthMode = eDepthBigBar;
	}else
		return false;
	if(!FileReadDouble(L"Ascan",L"Start",strFileName.GetBuffer(),1.0e-6,dAscanStart))
		return false;
	if(!FileReadDouble(L"Ascan",L"Range",strFileName.GetBuffer(),1.0e-6,dAscanRange))
		return false;
	if(!FileReadDouble(L"Ascan",L"TimeSlot",strFileName.GetBuffer(),1.0e-6,dTimeSlot))
		return false;
	if(!FileReadInteger(L"Cscan",L"Count",strFileName.GetBuffer(),iCscanCount))
		return false;
	for(int iGate=0;iGate<iCscanCount;iGate++)
	{
		string.Format(L"Start[%d]",iGate);
		if(!FileReadDouble(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),1.0e-6,adCscanStart[iGate]))
			return false;
		string.Format(L"Range[%d]",iGate);
		if(!FileReadDouble(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),1.0e-6,adCscanRange[iGate]))
			return false;
		string.Format(L"Threshold[%d]",iGate);
		if(!FileReadDouble(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),1.0,adCscanThreshold[iGate]))
			return false;
		if(!FileReadDouble(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),1.0,adCscanThreshold[iGate]))
			return false;
		string.Format(L"Rectification[%d]",iGate);
		if(!FileReadEnum(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),L"Signed"L"\0"L"Unsigned"L"\0"L"UnsignedPositive"L"\0"L"UnsignedNegative"L"\0",iEnum))
			return false;
		aeRectification[iGate] = (enumRectification)iEnum;
		string.Format(L"ModeAmp[%d]",iGate);
		if(!FileReadEnum(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),L"Absolute"L"\0"L"Maximum"L"\0"L"Minimum"L"\0"L"PeakToPeak"L"\0",iEnum))
			return false;
		aeModeAmp[iGate] = (enumGateModeAmp)iEnum;
		string.Format(L"ModeTof[%d]",iGate);
		if(!FileReadEnum(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),L"AmplitudeDetection"L"\0"L"ThresholdCross"L"\0"L"ZeroFirstAfterThresholdCross"L"\0"L"ZeroLastBeforeThresholdCross"L"\0",iEnum))
			return false;
		aeModeTof[iGate] = (enumGateModeTof)iEnum;
	}
	if(AfxMessageBox(L"Do you want to complete ?",MB_YESNO)==IDNO)
		return false;
	//1.1.5.1r start
	if((!dSpecimenRadius && m_pWizardSystem->Specimen().SetSpecimen(ePlane)) ||
		((dSpecimenRadius>0.0) && m_pWizardSystem->Specimen().SetSpecimen(eDisk)))
	{
		AfxMessageBox(L"Error");
		return false;
	}
	if((dSpecimenRadius>0.0) && m_pWizardSystem->Specimen().SetRadius(dSpecimenRadius))
	{
		AfxMessageBox(L"Error");
		return false;
	}
	if(m_pWizardSystem->Probe(0).SetRadius(dProbeRadius))
	{
		AfxMessageBox(L"Error");
		return false;
	}
	//1.1.5.1r stop
	CWaitCursor wait;
	if(!OEMPA_WriteWizard(	m_pWizardSystem,m_pChannel,
						bLongitudinalWave,dSpecimenVelocity,
						iProbeElementCount,dProbePitch,dProbeFrequency,
						bWedgeEnable,
						dWedgeHeight,dWedgeAngle,dWedgeVelocity,
						dScanDepthEmission,adScanDepthReception,iScanDepthReception,
						iScanElementCount,
						bLinear,iScanElementStart,iScanElementStop,iScanElementStep,
						!bLinear,dScanAngleStart,dScanAngleStop,dScanAngleStep,
						dAscanStart,dAscanRange,dTimeSlot,//AscanStart, Range is written only for the first focal law
						iCscanCount,adCscanStart,adCscanRange,adCscanThreshold,
						aeRectification,aeModeAmp,aeModeTof,
						eDepthMode))
	{
		AfxMessageBox(L"Error");
		return false;
	}
	(*m_pChannel).GetProbe(0)->Scan(0).UpdateScan();
	return true;
}

bool SetWizardMatrix(CUTWizardSystem &wizard,int iCount,double *pdX,double *pdY,double *pdZ)
{
	CUTWizardProbe *pWizardProbe;
	CUTPoint point;

	pWizardProbe = &wizard.Probe(0);
	if(!pWizardProbe)
		return false;
	if(!pWizardProbe->SetWizardProbe(eCustom))
		return false;
	for(int iIndex=0;iIndex<iCount;iIndex++)
	{
		if(point.SetX(pdX[iIndex]))
			return false;
		if(point.SetX(pdX[iIndex]))
			return false;
		if(point.SetY(pdY[iIndex]))
			return false;
		if(point.SetZ(pdZ[iIndex]))
			return false;
		if(!pWizardProbe->SetCustomPoint(iIndex,point))
			return false;
	}
	return true;
}

bool GetWizardMatrix(CUTWizardSystem &wizard,int &iCount,double *pdX,double *pdY,double *pdZ)
{
	CUTWizardProbe *pWizardProbe;
	CUTPoint point;
	int iCount2;

	pWizardProbe = &wizard.Probe(0);
	if(!pWizardProbe)
		return false;
	if(pWizardProbe->GetWizardProbe()!=eCustom)
		return false;
	iCount2 = pWizardProbe->GetCount();
	if(!iCount)
		iCount = iCount2;
	else if(iCount>iCount2)
		return false;
	for(int iIndex=0;iIndex<iCount;iIndex++)
	{
		if(!pWizardProbe->GetPoint(iIndex,point))
			return false;
		pdX[iIndex] = point.GetX();
		pdY[iIndex] = point.GetY();
		pdZ[iIndex] = point.GetZ();
	}
	return true;
}

bool HWWriteFocalLaw(CUTChannels *pChannels,
					double dAscanStart,double dAscanRange,double dTimeSlot,
					int &iCscanCount,double dCscanStart[4],double dCscanRange[4],double dCscanThreshold[4])
{
	bool bRet=true;
	CUTScan *pScan;
	double dAscanStop;

	if(!pChannels)
		return false;
	if(!pChannels->IsMultiChannels())
		pScan = &pChannels->Probe().Scan();//single channel
	else
		pScan = &pChannels->Probe(0).Scan(0);//multi channel
	if(!pScan)
		return false;
	if(pScan->GetReceiverCycleCount())
	{
		if((*pScan)[0].Receiver().SetStart(dAscanStart))
			bRet = false;
		dAscanStop = dAscanRange+dAscanStart;
		if((*pScan)[0].Receiver().SetStop(dAscanStop))
			bRet = false;
		if((*pScan)[0].Pulser().SetTimer(dTimeSlot))
			bRet = false;
	}

	return bRet;
}

__declspec(dllexport) bool AutoResetAcq(double dEncoder)
{
	return false;
}

__declspec(dllexport) bool IsMechanicalManagement(bool Ascan)
{
	return Ascan;//ascan => true, cscan => false
}

//////////////////////////////////////////////////////////////////

CHWDlg *CHWDlg::m_pHWDlg=NULL;
bool g_bHWInit=false;
double CHWDlg::m_dEditGain=0.0;
double CHWDlg::m_dGain[2]={0.0,0.0};
int CHWDlg::m_iSystemIndex=0;
int CHWDlg::m_iCycleCount[2]={0,0};
int CHWDlg::m_iComboSystemCount=0;
int CHWDlg::m_iIndexLayoutFile=-1;
BOOL CHWDlg::m_bCheckToolboxInit=FALSE;
BOOL CHWDlg::m_bCheckConnect=FALSE;
BOOL CHWDlg::m_bCheckEnable=FALSE;
DWORD CHWDlg::m_dwTimerID=0;
bool CHWDlg::m_bUpdateDialog=false;
BOOL CHWDlg::m_bEnableEncoder=FALSE;
bool CHWDlg::m_bEnableInternal=false;
int CHWDlg::m_iCommunicationCountError=0;
long CHWDlg::m_lEditResolution=1;
double CHWDlg::m_dEditLength=100.0;
double CHWDlg::m_dEditThickness=10.0;
double CHWDlg::m_dEditStep=1.0;
BOOL CHWDlg::m_bCheckAutoResetAcq=FALSE;
BOOL CHWDlg::m_bCheckInternal=TRUE;
CString CHWDlg::m_strEditDataLostCount=L"";
DWORD CHWDlg::m_dwEditPort1=5001;
BYTE CHWDlg::m_cIP[4]={0,0,0,0};

int CHWDlg::m_iAcquisitionDataCount[g_iOEMPACycleCountMax]={0};
int CHWDlg::m_iAcquisitionDataSize[g_iOEMPACycleCountMax]={0};
enumBitSize CHWDlg::m_eAcquisitionDataBitSize[g_iOEMPACycleCountMax]={e8Bits};
bool CHWDlg::m_bAcquisitionDataSigned[g_iOEMPACycleCountMax]={0};
int CHWDlg::m_iAcquisitionCycleCount=0;
WORD *CHWDlg::m_pAcquisitionDataMax[g_iOEMPACycleCountMax]={0};
CRITICAL_SECTION CHWDlg::m_CriticalSection;
bool CHWDlg::m_bInitCriticalSection=false;

// CHWDlg dialog

IMPLEMENT_DYNAMIC(CHWDlg, CDialogEx)

CHWDlg::CHWDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHWDlg::IDD, pParent)
{
	HINSTANCE hInstance=AfxGetResourceHandle();
	
	CHWDeviceOEMPA::RegisterMultiProcess(L"OEMPASector");
	if(!m_bInitCriticalSection)
	{
		InitializeCriticalSection(&m_CriticalSection);
		m_bInitCriticalSection = true;
	}
	AfxSetResourceHandle(g_hInstance);
	g_uiUTEventMessage = UTKernel_RegisterWindowMessage();
	Create(IDD_DIALOG_HW,pParent);
	ShowWindow(SW_SHOW);
	AfxSetResourceHandle(hInstance);
	//OnEventMessage(CUTUsbDevice::wUpdateUSB,0);
	//m_hWndUsb = GetSafeHwnd();
	//CUTUsbDevice::RegisterHwnd(m_hWndUsb);
	m_pEditBkBrush = NULL;
	m_pEditBkBrush = new CBrush(RGB(0, 0, 0));
}

CHWDlg::~CHWDlg()
{
	//CUTUsbDevice::UnregisterHwnd(m_hWndUsb);
	m_iIndexLayoutFile = -1;
	CHWDeviceOEMPA::UnregisterMultiProcess();
}

CHWDlg *CHWDlg::GetHWDlg()
{
	return m_pHWDlg;
}

void CHWDlg::Display(CWnd *pWnd)
{
	if(!m_pHWDlg)
		m_pHWDlg = new CHWDlg(pWnd);
	if(!m_pHWDlg)
		return;
	//if(m_pHWDlg->IsIconic())
		m_pHWDlg->ShowWindow(SW_RESTORE);
}

BOOL CHWDlg::OnInitDialog()
{
	CString string;
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	wchar_t pTitle[MAX_PATH];
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	CDialogEx::OnInitDialog();

#ifndef _EVALUATION_
	m_cIP[0] = 192;
	m_cIP[1] = 168;
	m_cIP[2] = 1;
	m_cIP[3] = 11;
#else _EVALUATION_
	m_cIP[0] = 127;
	m_cIP[1] = 0;
	m_cIP[2] = 0;
	m_cIP[3] = 1;
#endif _EVALUATION_
	m_IPAddress1.SetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
	m_bUpdateDialog = true;
	UpdateData();
	m_SpinGain.SetRange32(0,800);
	m_SpinGain.SetBuddy(&m_EditGain);
	UpdateDialog();
	SetTimer(TRUE);

	//integration
	if(UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),MAX_PATH,pTitle,iResHorz,iResVert,wndPlacement))
	{
		if(UTKernel_LayoutFile()->RegisterWindow(pTitle,m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
		{
			if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,pTitle,sysTime,iResHorz,iResVert,wndPlacement,iData))
			if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}else
			UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,pTitle,iResHorz,iResVert,wndPlacement,0);
	}

	string.Format(L"HW (PID %d)",GetCurrentProcessId());
	SetWindowText(string);
#ifdef _ENABLE_DISABLE_FILLING
	m_CheckFilingBetween.SetCheck(BST_CHECKED);
	m_CheckFilingLast.SetCheck(BST_CHECKED);
#else _ENABLE_DISABLE_FILLING
	m_CheckFilingBetween.EnableWindow(FALSE);
	m_CheckFilingLast.EnableWindow(FALSE);
#endif _ENABLE_DISABLE_FILLING

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHWDlg::PostNcDestroy()
{
	if(m_bCheckToolboxInit)
		UnregisterSystem();
	if(m_pHWDlg==this)
	{
		m_pHWDlg = NULL;
	}
	delete this;
}

void CHWDlg::Destroy()
{
	if(m_pHWDlg)
		m_pHWDlg->DestroyWindow();
}

void CHWDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// Free the space allocated for the background brush
	delete m_pEditBkBrush;
	m_pEditBkBrush = NULL;
}

HBRUSH CHWDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	DWORD dwTick;

	if(0 && (&m_EditDataLostCount==pWnd))
	{
		if(m_strEditDataLostCount.GetLength()>0)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkColor(RGB(255, 0, 0));
			return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
		}
	}
	if(&m_EditEncoder==pWnd)
	{
		dwTick = GetTickCount();
		if(g_iAscanSequenceLostTimerMs && (g_dwAscanSequenceLost+g_iAscanSequenceLostTimerMs>dwTick))
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkColor(RGB(255, 100, 0));
			return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
		}
		else
		if((g_dEncoder<0) || (CHWDlg::m_dEditLength<g_dEncoder*1e3))
		{
			if((g_dEncoder<-10*g_dEncoderStep) || (CHWDlg::m_dEditLength+10*g_dEncoderStep<g_dEncoder*1e3))
			{
				pDC->SetTextColor(RGB(0, 0, 0));
				pDC->SetBkColor(RGB(255, 0, 0));
			}else{
				pDC->SetTextColor(RGB(0, 0, 0));
				pDC->SetBkColor(RGB(255, 150, 0));
			}
			return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
		}
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//////////////////////////////////////////////////////////////////

void CHWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_WIZARD, m_CheckWizard);
	DDX_Check(pDX, IDC_CHECK_WIZARD, m_bCheckWizard);
	DDX_Control(pDX, IDC_COMBO_SYSTEM_COUNT, m_ComboSystemCount);
	DDX_CBIndex(pDX, IDC_COMBO_SYSTEM_COUNT, m_iComboSystemCount);
	DDX_Control(pDX, IDC_CHECK_TOOLBOX_INIT, m_CheckToolboxInit);
	DDX_Check(pDX, IDC_CHECK_TOOLBOX_INIT, m_bCheckToolboxInit);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dEditGain);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_EditGain);
	DDX_Control(pDX, IDC_SPIN_GAIN, m_SpinGain);
	DDX_Control(pDX, IDC_CHECK_CONNECT, m_ButtonConnect);
	DDX_Control(pDX, IDC_BUTTON_HW_UPDATE, m_ButtonHWUpdate);
	DDX_Check(pDX, IDC_CHECK_CONNECT, m_bCheckConnect);
	DDX_Control(pDX, IDC_BUTTON_HW_LOAD, m_ButtonLoad);
	DDX_Control(pDX, IDC_RADIO_PROBE1, m_RadioProbe1);
	DDX_Control(pDX, IDC_RADIO_PROBE2, m_RadioProbe2);
	DDX_Radio(pDX, IDC_RADIO_PROBE1, m_iSystemIndex);
	DDX_Control(pDX, IDC_BUTTON_RESET_TIMESTAMP, m_ButtonResetTime);
	DDX_Control(pDX, IDC_BUTTON_STATUS, m_ButtonStatus);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_CheckEnable);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bCheckEnable);
	DDX_Control(pDX, IDC_EDIT_ENCODER, m_EditEncoder);
	DDX_Control(pDX, IDC_BUTTON_HW_SAVE, m_ButtonHwSave);
	DDX_Control(pDX, IDC_BUTTON_SYSTEM_SAVE, m_ButtonWizardSave);
	DDX_Control(pDX, IDC_BUTTON_SYSTEM_LOAD, m_ButtonWizardLoad);
	DDX_Control(pDX, IDC_BUTTON_SCAN_SECTORIAL, m_ButtonWizardSectorial);
	DDX_Check(pDX, IDC_CHECK_ENCODER, m_bEnableEncoder);
	DDX_Control(pDX, IDC_CHECK_ENCODER, m_EnableEncoder);
	DDX_Control(pDX, IDC_BUTTON_CONFIG, m_ButtonConfig);
	DDX_Control(pDX, IDC_EDIT_RESOLUTION, m_EditResolution);
	DDX_Text(pDX, IDC_EDIT_RESOLUTION, m_lEditResolution);
	DDX_Control(pDX, IDC_EDIT_LENGTH, m_EditLength);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_dEditLength);
	DDX_Control(pDX, IDC_EDIT_THICKNESS, m_EditThickness);
	DDX_Text(pDX, IDC_EDIT_THICKNESS, m_dEditThickness);
	DDX_Control(pDX, IDC_EDIT_STEP, m_EditStep);
	DDX_Text(pDX, IDC_EDIT_STEP, m_dEditStep);
	DDX_Control(pDX, IDC_CHECK_AUTO_RESETACQ, m_CheckAutoResetAcq);
	DDX_Check(pDX, IDC_CHECK_AUTO_RESETACQ, m_bCheckAutoResetAcq);
	DDX_Control(pDX, IDC_CHECK_INTERNAL, m_CheckInternal);
	DDX_Check(pDX, IDC_CHECK_INTERNAL, m_bCheckInternal);
	DDX_Text(pDX, IDC_EDIT_DATA_LOST, m_strEditDataLostCount);
	DDX_Control(pDX, IDC_EDIT_DATA_LOST, m_EditDataLostCount);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPAddress1);
	DDX_Control(pDX, IDC_EDIT_PORT1, m_EditPort1);
	DDX_Text(pDX, IDC_EDIT_PORT1, m_dwEditPort1);
	DDX_Control(pDX, IDC_CHECK_HW_LINK, m_ButtonHwLink);
	DDX_Control(pDX, IDC_BUTTON_TOOLBOX, m_ButtonToolbox);
//#ifdef _ENABLE_DISABLE_FILLING
	DDX_Control(pDX, IDC_CHECK_FILLING_BETWEEN, m_CheckFilingBetween);
	DDX_Control(pDX, IDC_CHECK_FILLING_LAST, m_CheckFilingLast);
//#endif _ENABLE_DISABLE_FILLING
}

BEGIN_MESSAGE_MAP(CHWDlg, CDialogEx)
	ON_REGISTERED_MESSAGE(g_uiUTEventMessage, &CHWDlg::OnEventMessage)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GAIN, &CHWDlg::OnDeltaposSpinGain)
	ON_BN_CLICKED(IDCANCEL, &CHWDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_HW_UPDATE, &CHWDlg::OnBnClickedButtonHwUpdate)
	ON_BN_CLICKED(IDC_CHECK_TOOLBOX_INIT, &CHWDlg::OnBnClickedCheckToolboxInit)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CHWDlg::OnBnClickedCheckConnect)
	ON_BN_CLICKED(IDC_BUTTON_HW_LOAD, &CHWDlg::OnBnClickedButtonHwLoad)
	ON_BN_CLICKED(IDOK, &CHWDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, &CHWDlg::OnEnKillfocusEditGain)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_HW_SAVE, &CHWDlg::OnBnClickedButtonHwSave)
	ON_BN_CLICKED(IDC_RADIO_PROBE1, &CHWDlg::OnBnClickedRadioProbe1)
	ON_BN_CLICKED(IDC_RADIO_PROBE2, &CHWDlg::OnBnClickedRadioProbe2)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TIMESTAMP, &CHWDlg::OnBnClickedButtonResetTimestamp)
	ON_BN_CLICKED(IDC_BUTTON_STATUS, &CHWDlg::OnBnClickedButtonStatus)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, &CHWDlg::OnBnClickedCheckEnable)
	ON_BN_CLICKED(IDC_BUTTON_TOOLBOX, &CHWDlg::OnBnClickedButtonToolbox)
	ON_BN_CLICKED(IDC_BUTTON_SCAN_SECTORIAL, &CHWDlg::OnBnClickedButtonScanSectorial)
	ON_BN_CLICKED(IDC_BUTTON_SYSTEM_SAVE, &CHWDlg::OnBnClickedButtonSystemSave)
	ON_BN_CLICKED(IDC_BUTTON_SYSTEM_LOAD, &CHWDlg::OnBnClickedButtonSystemLoad)
	ON_BN_CLICKED(IDC_CHECK_ENCODER, &CHWDlg::OnBnClickedCheckEncoder)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CHWDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CHWDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CHWDlg::OnBnClickedButtonConfig)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_AUTO_RESETACQ, &CHWDlg::OnBnClickedCheckAutoResetacq)
	ON_BN_CLICKED(IDC_CHECK_INTERNAL, &CHWDlg::OnBnClickedCheckInternal)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION, &CHWDlg::OnBnClickedButtonCalibration)
	ON_BN_CLICKED(IDC_CHECK_HW_LINK, &CHWDlg::OnBnClickedCheckHwLink)
	ON_BN_CLICKED(IDC_CHECK_FILLING_BETWEEN, &CHWDlg::OnBnClickedCheckFillingBetween)
	ON_BN_CLICKED(IDC_CHECK_FILLING_LAST, &CHWDlg::OnBnClickedCheckFillingLast)
END_MESSAGE_MAP()


// CHWDlg message handlers


LRESULT CHWDlg::OnEventMessage(WPARAM wParam,LPARAM lParam)
{
#ifdef _USB2_
	if(wParam==CUTUsbDevice::wUpdateUSB)
	{//case of USB connection
		UpdateData();
	}
#endif _USB2_
	if((wParam==WPARAM_RUN_TOOLBOX) && (lParam==LPARAM_RUN_TOOLBOX_OK))
		g_RunToolbox.SignalPostFeedback();//No error
	if((wParam==WPARAM_RUN_TOOLBOX) && (lParam==LPARAM_RUN_TOOLBOX_ERROR))
		AfxMessageBox(L"Error to run the toolbox, please try again!");
	return 0;
}

DWORD WINAPI CHWDlg::CallbackThreadToolbox(HWND hWnd,CRunToolbox *pRunToolbox)
{
	int iRootId;
	CWaitCursor wait;

	if(!UTKernel_IsToolboxRunning())
	{
		pRunToolbox->ResetPostFeedback();
		UTKernel_ToolboxRun(false,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
		if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
			return 1;
		pRunToolbox->ResetPostFeedback();
		UTKernel_ToolboxDisplayCloseAll(hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
		if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
			return 2;
	}
	if(m_bCheckToolboxInit)
	{
		if(g_Wizard.GetID(iRootId))
		{
			pRunToolbox->ResetPostFeedback();
			UTKernel_ToolboxDisplay(iRootId,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
			if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
				return 3;
		}
		if(g_pChannel && g_pChannel[m_iSystemIndex].GetID(iRootId))
		{
			pRunToolbox->ResetPostFeedback();
			UTKernel_ToolboxDisplay(iRootId,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
			if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
				return 4;
		}
	}
	return 0;//no error
}

//////////////////////////////////////////////////////////////////

void CHWDlg::UpdateDialog()
{
	int iPos;

	if(!GetSafeHwnd() || !m_bUpdateDialog)
		return;
	m_IPAddress1.ShowWindow(m_bCheckToolboxInit?SW_SHOW:SW_HIDE);
	m_EditPort1.ShowWindow(m_bCheckToolboxInit?SW_SHOW:SW_HIDE);
	m_ButtonHwLink.ShowWindow(m_bCheckToolboxInit?SW_SHOW:SW_HIDE);
	m_ButtonConnect.ShowWindow(m_bCheckToolboxInit?SW_SHOW:SW_HIDE);

	m_IPAddress1.EnableWindow(m_bCheckToolboxInit && !m_bCheckConnect);
	m_EditPort1.EnableWindow(m_bCheckToolboxInit && !m_bCheckConnect);
	m_ButtonHwLink.EnableWindow(m_bCheckToolboxInit);
	m_ComboSystemCount.EnableWindow(!m_bCheckToolboxInit);
	m_ButtonConfig.EnableWindow(m_bCheckToolboxInit);
	m_ButtonConnect.EnableWindow(m_bCheckToolboxInit);
	m_ButtonHwSave.EnableWindow(m_bCheckToolboxInit);
	m_ButtonWizardSave.EnableWindow(m_bCheckToolboxInit);
	m_ButtonWizardLoad.EnableWindow(m_bCheckToolboxInit);
	m_ButtonWizardSectorial.EnableWindow(m_bCheckToolboxInit);
	if(!m_bCheckToolboxInit)
		m_bCheckConnect = FALSE;
	m_ButtonHWUpdate.EnableWindow(m_bCheckConnect);
	m_CheckWizard.EnableWindow(m_bCheckConnect);
	m_ButtonLoad.EnableWindow(m_bCheckConnect);
	m_ButtonResetTime.EnableWindow(m_bCheckConnect);
	m_ButtonStatus.EnableWindow(m_bCheckConnect);
	m_CheckEnable.EnableWindow(m_bCheckConnect);
	m_EnableEncoder.EnableWindow(m_bCheckConnect);
	m_EditEncoder.ShowWindow(m_bCheckConnect && m_bEnableEncoder);
	m_CheckInternal.ShowWindow(m_bCheckConnect && m_bEnableEncoder);
	if(!(m_bCheckConnect && m_bEnableEncoder))
		m_bCheckInternal = FALSE;
	m_EditGain.EnableWindow(m_iCycleCount[m_iSystemIndex]?TRUE:FALSE);
	m_SpinGain.EnableWindow(m_iCycleCount[m_iSystemIndex]?TRUE:FALSE);
	iPos = (int)(m_dEditGain*10.0);
	m_SpinGain.SetPos32(iPos);
	m_RadioProbe1.ShowWindow(g_iSystemCount==2?SW_SHOW:SW_HIDE);
	m_RadioProbe2.ShowWindow(g_iSystemCount==2?SW_SHOW:SW_HIDE);
	m_EditResolution.EnableWindow(!m_bEnableEncoder);
	m_EditLength.EnableWindow(!m_bEnableEncoder);
	m_EditThickness.EnableWindow(!m_bEnableEncoder);
	m_EditStep.EnableWindow(!m_bEnableEncoder);
	m_CheckInternal.EnableWindow(m_bEnableInternal);
	m_ButtonToolbox.EnableWindow(IsWow64()?FALSE:TRUE);
	UpdateData(FALSE);
}

bool CHWDlg::SetTimer(BOOL bEnable)
{
	if(bEnable && !m_dwTimerID)
	{
		m_dwTimerID = (DWORD)CWnd::SetTimer(g_kTimerID, g_kTimerDelay, NULL);
		if(m_dwTimerID)
			return true;
		return false;
	}
	if(!bEnable && m_dwTimerID)
	{
		KillTimer(m_dwTimerID);
		m_dwTimerID = 0;
		return true;
	}
	return false;
}

void CHWDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString string;
	bool bUpdate=false;
	int iDataLostAscan,iDataLostCscan,iDataLostEncoder,iDataLostUSB3;
	CString strDataLost,strAux;
	CHWDeviceOEMPA *pHWDeviceOEMPA=&g_pHW[0];

	if(nIDEvent != g_kTimerID)
		return;
	if(!g_iSystemCount)
		return;
	string.Format(L"%.1lf mm",g_dEncoder*1.0e3);
	m_EditEncoder.SetWindowText(string);

	if(pHWDeviceOEMPA)
	{
		if(m_bCheckConnect && !pHWDeviceOEMPA->GetSWDevice()->IsConnected())
		{
			bUpdate = true;
			m_bCheckConnect = FALSE;
		}
		if(!pHWDeviceOEMPA->GetSWDevice()->IsPulserEnabled())
		{
			if(m_bCheckEnable)
			{
				bUpdate = true;
				m_bCheckEnable = FALSE;
			}
		}else{
			if(!m_bCheckEnable)
			{
				bUpdate = true;
				m_bCheckEnable = TRUE;
			}
		}
		iDataLostAscan = pHWDeviceOEMPA->GetSWDevice()->GetLostCountAscan();
		iDataLostCscan = pHWDeviceOEMPA->GetSWDevice()->GetLostCountCscan();
		iDataLostEncoder = pHWDeviceOEMPA->GetSWDevice()->GetLostCountEncoder();
		iDataLostUSB3 = pHWDeviceOEMPA->GetSWDevice()->GetLostCountUSB3();
		strDataLost = L"";
		if(iDataLostAscan)
		{
			strAux.Format(L"A=%d ",iDataLostAscan);
			strDataLost += strAux;
		}
		if(iDataLostCscan)
		{
			strAux.Format(L"C=%d ",iDataLostCscan);
			strDataLost += strAux;
		}
		if(iDataLostEncoder)
		{
			strAux.Format(L"E=%d ",iDataLostEncoder);
			strDataLost += strAux;
		}
		if(iDataLostUSB3)
		{
			strAux.Format(L"U=%d ",iDataLostUSB3);
			strDataLost += strAux;
		}
		if(m_strEditDataLostCount!=strDataLost)
		{
			bUpdate = true;
			m_strEditDataLostCount = strDataLost;
		}
		if(m_bEnableEncoder && (m_lEditResolution != pHWDeviceOEMPA->GetSWDevice()->GetSWEncoder(0)->lGetResolution()))
			m_lEditResolution = pHWDeviceOEMPA->GetSWDevice()->GetSWEncoder(0)->lGetResolution();
		if(bUpdate)
			UpdateDialog();
	}
}

void CHWDlg::OnBnClickedCancel()
{
	DestroyWindow();
}

void CHWDlg::OnClose()
{
	CHWDlg::Destroy();
}

void CHWDlg::RefreshConnect()
{
	bool bConnected,bEnableEncoder;
	int iComError;
	CString string;
	CSWDeviceOEMPA *pDeviceOEMPA;
	CHWDeviceOEMPA* pHWDeviceOEMPA;
	enumOEMPATrigger eOEMPATrigger;
	bool bSlave=false;

	for(int iSystemIndex=0;iSystemIndex<g_iSystemCount;iSystemIndex++)
	{
		if(!iSystemIndex)
		{
			bConnected = g_pHW[iSystemIndex].GetSWDevice()->IsConnected();
			pDeviceOEMPA = g_pHW[iSystemIndex].GetSWDeviceOEMPA();
			bEnableEncoder = false;
			if((g_pHW[iSystemIndex].GetSWDevice()->GetEncoderEnabledCount()>0) && pDeviceOEMPA)
			{
				eOEMPATrigger = pDeviceOEMPA->GetTriggerMode();
				if((eOEMPATrigger==eOEMPAEncoder) || (eOEMPATrigger==eOEMPAInternal))
					bEnableEncoder = true;
				if((eOEMPATrigger==eOEMPAEncoder) && pDeviceOEMPA->GetTriggerEncoderStep(m_dEditStep))
					m_dEditStep *= 1.0e3;
			}
			g_bCscanTofEnable = pDeviceOEMPA->IsCscanTofEnabled();
			iComError = g_pHW[iSystemIndex].GetSWDevice()->GetStreamError();
			if(bConnected)
			{
				m_bCheckEnable = TRUE;
				if(g_pHW[iSystemIndex].GetSWDevice()->IsPulserEnabled())
					m_bCheckEnable = TRUE;
				else
					m_bCheckEnable = FALSE;
			}else
				m_bCheckEnable = FALSE;
		}else{
			if(!g_pHW[iSystemIndex].GetSWDevice()->IsConnected())
				bConnected = false;
			iComError += g_pHW[iSystemIndex].GetSWDevice()->GetStreamError();
		}
	}
	if(bConnected)
	{
		if(!m_bCheckConnect && !CHWDlg::m_iCycleCount[0])
		{
			//first connection (and no OEMPA file has not yet been loaded)
			if(HWGetCycleCount(g_pHW[0],CHWDlg::m_iCycleCount[0],CHWDlg::m_dEditGain))
				CHWDlg::m_dGain[0] = CHWDlg::m_dEditGain;
			if(g_pAllocDisplayCallback)
				g_pAllocDisplayCallback(true);
		}
		m_bCheckConnect = TRUE;
	}else{
		//m_iCommunicationCountError = 0;
		m_bCheckConnect = FALSE;
	}
	pHWDeviceOEMPA = &g_pHW[m_iSystemIndex];
	if(pHWDeviceOEMPA && (pHWDeviceOEMPA->GetHardwareLink()==eSlave))
		bSlave = true;
	if(pHWDeviceOEMPA && !pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsIOBoardEnabled())
	{
		if(bSlave)
			m_bCheckInternal = FALSE;
		else
		m_bCheckInternal = TRUE;
		m_bEnableInternal = false;
	}else{
		if(bSlave)
			m_bEnableInternal = false;
		else
		m_bEnableInternal = true;
	}
	if(pHWDeviceOEMPA && pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsIOBoardEnabled())
	if(bEnableEncoder)
		m_bEnableEncoder = TRUE;
	else
		m_bEnableEncoder = FALSE;
	UpdateDialog();
	if(bConnected && (m_iCommunicationCountError<iComError))
	{
		m_iCommunicationCountError = iComError;
		string.Format(L"Communication errors (%d) have been notified!",m_iCommunicationCountError);
		AfxMessageBox(string);
	}
}

void CHWDlg::OnBnClickedCheckToolboxInit()
{
	CWaitCursor wait;

	UpdateData();
	if(m_bCheckToolboxInit)
	{
		if(!m_iComboSystemCount)
			m_iSystemIndex = 0;
		if(!RegisterSystem(m_iComboSystemCount+1))
			m_bCheckToolboxInit = FALSE;
		m_iCommunicationCountError = 0;
	}else{
		if(!UnregisterSystem())
			m_bCheckToolboxInit = TRUE;
	}
	UpdateDialog();
}

void CHWDlg::OnBnClickedCheckConnect()
{
	enumConnectionState eConnection;
	bool bRet=true;
	bool bConnected=false;
	BOOL bCheckConnect=m_bCheckConnect;
	CWaitCursor wait;
	wchar_t pValue[40];
	DWORD dwProcessId;

	UpdateData();
	m_IPAddress1.GetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
	swprintf(pValue,40,L"%d.%d.%d.%d",m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
	if(m_bCheckConnect)
	{
		eConnection = eConnected;
		if(CHWDeviceOEMPA::IsMultiProcessConnected(pValue,dwProcessId))
			CHWDeviceOEMPA::DisconnectMultiProcess(pValue,dwProcessId);
	}else
		eConnection = eDisconnected;
	if(g_pHW)
	{
		if(bCheckConnect && !m_bCheckConnect)
			m_strEditDataLostCount = L"";
		if(!bCheckConnect && m_bCheckConnect)
		{
			if(!g_pHW[0].GetSWDeviceOEMPA()->SetIPAddress(pValue) ||
				!g_pHW[0].GetSWDeviceOEMPA()->SetPort((unsigned short)m_dwEditPort1))
				bRet = false;
		}
		for(int iSystemIndex=0;iSystemIndex<g_iSystemCount;iSystemIndex++)
		{
			if(!g_pHW[iSystemIndex].GetSWDevice()->SetConnectionState(eConnection,true))
				bRet = false;
		}
		for(int iSystemIndex=0;iSystemIndex<g_iSystemCount;iSystemIndex++)
		{
			if(!iSystemIndex)
				bConnected = g_pHW[iSystemIndex].GetSWDevice()->IsConnected();
			else{
				if(!g_pHW[iSystemIndex].GetSWDevice()->IsConnected())
					bConnected = false;
			}
		}
		if(bConnected)
			m_bCheckConnect = TRUE;
		else
			m_bCheckConnect = FALSE;
	}
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"Communication error!");
	else
		RefreshConnect();
}

void CHWDlg::OnBnClickedRadioProbe1()
{
	UpdateData();
	m_dEditGain = m_dGain[m_iSystemIndex];
	UpdateDialog();
}

void CHWDlg::OnBnClickedRadioProbe2()
{
	UpdateData();
	m_dEditGain = m_dGain[m_iSystemIndex];
	UpdateDialog();
}

void CHWDlg::OnBnClickedButtonHwUpdate()
{
	bool bUpdateChannelWizard;
	bool bRet=true;

	UpdateData();
	m_iCycleCount[m_iSystemIndex] = 0;
	m_dEditGain = 0.0;
	if(m_bCheckWizard)
		bUpdateChannelWizard = true;
	else
		bUpdateChannelWizard = false;
	if(g_pAllocDisplayCallback)
		g_pAllocDisplayCallback(false);
	if(g_pHW && g_pChannel)
	{
		for(int iSystemIndex=0;iSystemIndex<g_iSystemCount;iSystemIndex++)
		{
			bRet = HWUpdate(bUpdateChannelWizard,g_pHW[iSystemIndex],g_pChannel[iSystemIndex],&g_pCorrection[iSystemIndex]) && bRet;
		}
	}
	if(!HWGetCycleCount(g_pHW[m_iSystemIndex],m_iCycleCount[m_iSystemIndex],m_dGain[m_iSystemIndex]))
		AfxMessageBox(L"Error to readback HW!");
	m_dEditGain = m_dGain[m_iSystemIndex];
	if(!bRet)
		AfxMessageBox(L"Error");
	UpdateDialog();
	if(g_pAllocDisplayCallback)
		g_pAllocDisplayCallback(true);
}

void CHWDlg::OnBnClickedButtonHwLoad()
{
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPA");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(TRUE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string;
	CHWDeviceOEMPA* pHWDeviceOEMPA;

	UpdateData();
	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetFileExt();
	if((string!=L"txt") && (string!=L"bin"))
		return;
	CWaitCursor wait;
	string = filedialog.GetPathName();
	pHWDeviceOEMPA = &g_pHW[m_iSystemIndex];
	m_dGain[m_iSystemIndex] = 0.0;
	m_iCycleCount[m_iSystemIndex] = 0;
	m_dEditGain = 0.0;
	if(g_pAllocDisplayCallback)
		g_pAllocDisplayCallback(false);
	if(pHWDeviceOEMPA && !OEMPA_ReadFileWriteHW(pHWDeviceOEMPA,string.GetBuffer()))
		AfxMessageBox(L"Error to update HW!");
	UpdateDialog();
	UTKernel_SetFolderDataFiles(L"OEMPA",string.GetBuffer());
}

void CHWDlg::OnDeltaposSpinGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	*pResult = 0;
	BOOL bResult;
	int iPos=m_SpinGain.GetPos32(&bResult);
	iPos = pNMUpDown->iPos+pNMUpDown->iDelta;
	m_dEditGain = iPos/10.0;
	UpdateData(FALSE);
	if(!HWSetGain(g_pHW[m_iSystemIndex],m_dEditGain))
		AfxMessageBox(L"Error to write HW amplification!");
}

void CHWDlg::OnBnClickedOk()
{
	UpdateGain();
}

void CHWDlg::OnEnKillfocusEditGain()
{
	double dGain=m_dEditGain;

	UpdateData();
	if(dGain==m_dEditGain)
		return;
	UpdateGain();
}

void CHWDlg::UpdateGain()
{
	int iPos;
	double dGain=m_dEditGain;

	UpdateData();
	iPos = (int)(m_dEditGain*10.0);
	m_SpinGain.SetPos32(iPos);
	if(!HWSetGain(g_pHW[m_iSystemIndex],m_dEditGain))
		AfxMessageBox(L"Error to write HW amplification!");
	m_dGain[m_iSystemIndex] = m_dEditGain;
	UpdateDialog();
}

void CHWDlg::OnBnClickedButtonHwSave()
{
	//IN THE FUTURE
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPA");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",this,0);
	CString string,strAux;
	CHWDeviceOEMPA* pHWDeviceOEMPA;

	if(!g_pHW)
		return;
	pHWDeviceOEMPA = &g_pHW[m_iSystemIndex];
	if(!pHWDeviceOEMPA)
		return;
	if(filedialog.DoModal()!=IDOK)
		return;
	CWaitCursor wait;
	string = filedialog.GetPathName();
	try{
		CFile::Remove(string);
	}catch(CFileException *e){
		e->Delete();
	}
	if(!OEMPA_ReadWizardWriteFile(pHWDeviceOEMPA,&g_Wizard,&g_pChannel[m_iSystemIndex],string.GetBuffer()))
		AfxMessageBox(L"Error to save the file!");
	else{
		try{
			strAux = g_wcFolderData;
			strAux += L"Wizard.txt";
			CStdioFile fileIn(strAux,CFile::typeText | CFile::modeRead);
			CStdioFile fileOut(string,CFile::typeText | CFile::modeWrite);
			fileOut.SeekToEnd();
			fileOut.WriteString(L"\n");
			while(fileIn.ReadString(strAux))
			{
				fileOut.WriteString(L"#");
				fileOut.WriteString(strAux);
				fileOut.WriteString(L"\n");
			}
		}catch(CFileException *e){
			e->ReportError();
			e->Delete();
		}
		EditFile(string.GetBuffer(),false);
	}
	UTKernel_SetFolderDataFiles(L"OEMPA",string.GetBuffer());
}

void CHWDlg::OnBnClickedButtonResetTimestamp()
{
	CHWDeviceOEMPA* pHWDeviceOEMPA;
	bool bRet = true;

	pHWDeviceOEMPA = &g_pHW[m_iSystemIndex];
	if(!pHWDeviceOEMPA->LockDevice(eAcqOff))
		return;
	if(g_pAllocDisplayCallback)
		g_pAllocDisplayCallback(false);
	CallbackSettingId(g_dwSettingId);
	if(!g_pHW[0].ResetTrackingTable())
		bRet = false;
	//ToDo
	//CHWDevice::SetTimeStamps();
	if(!g_pHW[0].ResetTimeStamp())//STOP_NEXT_STANDBY
		bRet = false;
	pHWDeviceOEMPA->UnlockDevice();
	for(int iSystemIndex=0;iSystemIndex<g_iSystemCount;iSystemIndex++)
	{
		if(!HWResetEncoder(iSystemIndex,g_pHW[iSystemIndex]))
			AfxMessageBox(L"Error to reset encoders!");
	}
	if(g_pAllocDisplayCallback)
		g_pAllocDisplayCallback(true);
	g_dEncoder = 0.0;
	g_dEncoderLocalAscan = 0.0;
	g_dEncoderLocalCscan = 0.0;
}

void CHWDlg::OnBnClickedButtonStatus()
{
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"Status");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string,strFile;
	int iBoardCount[g_iSystemCountMax];
	WORD awTemperature[g_iSystemCountMax][(1+g_iOEMPABoardCountMax)*2][g_iOEMPARXBoardTemperatureSensorCountMax];
	ULONGLONG aullTimeStampSW[g_iSystemCountMax],aullTimeStampHW[g_iSystemCountMax];
	int iRet=0,iSensorCountMax;
	bool bRet=true,bDigitalInput=false;
	DWORD dwDigitalInput[2];
	CHWDeviceOEMPA* pHWDeviceOEMPA;
	int iLength;

	pHWDeviceOEMPA = &g_pHW[m_iSystemIndex];
	if(filedialog.DoModal()!=IDOK)
		return;
	strFile = filedialog.GetPathName();
	for(int iSystemIndex=0;iSystemIndex<g_iSystemCount;iSystemIndex++)
	{
		if(!HWGetTemperatureSensor(iSystemIndex,g_pHW[iSystemIndex],iBoardCount,awTemperature))
		{
			bRet = false;
			AfxMessageBox(L"Error to get temperature sensors!");
		}
	}
	//ToDo
	//if(!pHWDeviceOEMPA->LockDevice(eAcqOff))
	//	return;
	//iRet = CHWDevice::GetTimeStamps(g_iSystemCountMax,aullTimeStampSW,aullTimeStampHW);
	//for(int iSystemIndex=0;iSystemIndex<g_iSystemCount;iSystemIndex++)
	//{
	//	pHWDeviceOEMPA = &g_pHW[iSystemIndex];
	//	if(pHWDeviceOEMPA && pHWDeviceOEMPA->GetDigitalInput(&dwDigitalInput[iSystemIndex]))
	//		bDigitalInput = true;
	//	else
	//		bDigitalInput = false;
	//}
	//if(!pHWDeviceOEMPA->UnlockDevice())
	//	return;
	//if(iRet!=g_iSystemCount)
	//	AfxMessageBox(L"Error to get time stamps!");
	//if(bRet && (iRet==g_iSystemCount))
	if(bRet)
	{
		try{
			CStdioFile file(strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			file.SeekToEnd();
			string.Format(L"System\tBoard\tSensor\tTemperature\n");
			file.WriteString(string);
			for(int iSystemIndex=0;iSystemIndex<g_iSystemCount;iSystemIndex++)
			{
				for(int iIndexBoard=0;iIndexBoard<1+iBoardCount[iSystemIndex];iIndexBoard++)
				{
					if(!iIndexBoard)
						iSensorCountMax = g_iOEMPAComBoardTemperatureSensorCountMax;
					else
						iSensorCountMax = g_iOEMPARXBoardTemperatureSensorCountMax;
					for(int iIndexSensor=0;iIndexSensor<iSensorCountMax;iIndexSensor++)
					{
						string.Format(L"%d\t%d\t%d\t%d 캜\n",iSystemIndex,iIndexBoard,iIndexSensor,(int)awTemperature[iSystemIndex][iIndexBoard][iIndexSensor]);
						file.WriteString(string);
					}
				}
				string.Format(L"\n");
				file.WriteString(string);
				string.Format(L"%d\tTimeStamp SW=0x%.16I64X HW=0x%.16I64X\n",iSystemIndex,aullTimeStampSW[iSystemIndex],aullTimeStampHW[iSystemIndex]);
				file.WriteString(string);
				if(aullTimeStampSW[iSystemIndex]>aullTimeStampHW[iSystemIndex])
					string.Format(L"%d\tTimeStamp Delta=+0x%.16I64X\n",iSystemIndex,aullTimeStampSW[iSystemIndex]-aullTimeStampHW[iSystemIndex]);
				else
					string.Format(L"%d\tTimeStamp Delta=-0x%.16I64X\n",iSystemIndex,aullTimeStampHW[iSystemIndex]-aullTimeStampSW[iSystemIndex]);
				if(bDigitalInput)
					string.Format(L"%d\tDigitalInputs = 0x%.2lX\n",iSystemIndex,dwDigitalInput[iSystemIndex]);
				file.WriteString(string);
				file.WriteString(string);
			}
			DumpFileData(file);
			file.Close();
		}catch(CFileException *e){
			e->ReportError();
			e->Delete();
		}
		try{
			CStdioFile file(strFile, CFile::modeRead | CFile::typeText);
			iLength = (int)file.GetLength();
			file.Close();
		}catch(CFileException *e){
			iLength = 0;
			e->ReportError();
			e->Delete();
		}
		if(iLength<1024*1024)
			EditFile(strFile.GetBuffer(),false);
		UTKernel_SetFolderDataFiles(L"Status",strFile.GetBuffer());
		if(iLength>=1024*1024)
			AfxMessageBox(L"The file has been created!");
	}
}

void CHWDlg::OnBnClickedCheckEnable()
{
	UpdateData();
	if(m_bCfgDefaultAcquisitionUnlockOn)
	{
		if(m_bCheckEnable)
			SetDefaultAcquisitionUnlockOn(g_pHW[m_iSystemIndex],true);
		else
			SetDefaultAcquisitionUnlockOn(g_pHW[m_iSystemIndex],false);
	}
	if(m_bCheckEnable)
		HWEnableShot(g_pHW[m_iSystemIndex],true);
	else
		HWEnableShot(g_pHW[m_iSystemIndex],false);
}

void CHWDlg::OnBnClickedButtonToolbox()
{
	g_RunToolbox.Run(GetSafeHwnd(),CHWDlg::CallbackThreadToolbox);
}

void CHWDlg::OnBnClickedButtonScanSectorial()
{
	double dAscanStart,dAscanRange,dTimeSlot;
	int iCscanCount=0;
	double adCscanStart[4]={0.0,0.0,0.0,0.0};
	double adCscanRange[4]={1.0e-6,1.0e-6,1.0e-6,1.0e-6};
	double adCscanThreshold[4]={50.0,50.0,50.0,50.0};

	if(WizardSectorial(&g_Wizard,&g_pChannel[m_iSystemIndex],dAscanStart,dAscanRange,dTimeSlot,iCscanCount,adCscanStart,adCscanRange,adCscanThreshold))
	{
		CWaitCursor wait;
		g_pChannel[m_iSystemIndex].GetProbe(0)->Scan(0).UpdateScan();
		HWWriteFocalLaw(&g_pChannel[m_iSystemIndex],
					dAscanStart,dAscanRange,dTimeSlot,iCscanCount,adCscanStart,adCscanRange,adCscanThreshold);
	}
}

void CHWDlg::OnBnClickedButtonSystemSave()
{
	CString strFileName;
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"Kernel");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);

	if(filedialog.DoModal()!=IDOK)
		return;
	CWaitCursor wait;
	strFileName = filedialog.GetPathName();
	UpdateData();
	if(!WizardSave(g_Wizard,g_pChannel[m_iSystemIndex],strFileName))
		AfxMessageBox(L"Error to save this file!");
	UTKernel_SetFolderDataFiles(L"Kernel",strFileName.GetBuffer());
}

void CHWDlg::OnBnClickedButtonSystemLoad()
{
	CString strFileName;
	CString strFileFullPathName=_T("*.txt");
	_TCHAR BASED_CODE szFilter[] = _T("Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||");
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"Kernel");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(TRUE,_T("*.txt"),strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,this,0);

	if(filedialog.DoModal()!=IDOK)
		return;
	CWaitCursor wait;
	strFileName = filedialog.GetPathName();
	if(!WizardLoad(g_pHW[m_iSystemIndex],g_Wizard,g_pChannel[m_iSystemIndex],strFileName))
		AfxMessageBox(L"Error to load this file!");
	UTKernel_SetFolderDataFiles(L"Kernel",strFileName.GetBuffer());
}

void CHWDlg::OnMove(int x, int y)//integration
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;
	wchar_t pTitle[MAX_PATH];
	CDialog::OnMove(x, y);

	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),MAX_PATH,pTitle,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"HW",iResHorz,iResVert,wndPlacement,0);
}

extern UINT g_uiUTEventMessage;
void CHWDlg::OnBnClickedCheckEncoder()
{
	bool bRet;
	BOOL bEnableEncoderNoCallback;
	CHWDeviceOEMPA* pHWDeviceOEMPA;
	bool bSlave=false;

	UpdateData();
	bEnableEncoderNoCallback = m_bEnableEncoder;//side effect on "m_bEnableEncoder" because of "RefreshConnect".
	pHWDeviceOEMPA = &g_pHW[m_iSystemIndex];
	if(pHWDeviceOEMPA && (pHWDeviceOEMPA->GetHardwareLink()==eSlave))
	{
		bSlave = true;
		m_bCheckInternal = FALSE;
	}
	if(m_bEnableEncoder)
	{
		WriteCfgSector();
		bRet = HWEnableEncoder(g_pHW[m_iSystemIndex],true,m_bCheckInternal?true:false,m_dEditStep*1.0e-3,m_dEditLength*1.0e-3,m_lEditResolution);
		if(!bSlave && pHWDeviceOEMPA && !pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsIOBoardEnabled())
			m_bCheckInternal = TRUE;
	}else
		bRet = HWEnableEncoder(g_pHW[m_iSystemIndex],false,m_bCheckInternal?true:false,m_dEditStep*1.0e-3,m_dEditLength*1.0e-3,m_lEditResolution);
	if(!bRet)
		AfxMessageBox(L"Communication error!");
	m_bEnableEncoder = bEnableEncoderNoCallback;
	UpdateDialog();
	if(GetParent())
		GetParent()->PostMessage(g_uiUTEventMessage);
}

void CHWDlg::OnBnClickedButtonLoad()
{
	CString string,strAux;
	CString strFileName;
	CString strFileFullPathName=_T("*.txt");
	_TCHAR BASED_CODE szFilter[] = _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"Matrix");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(TRUE,_T("*.txt"),strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,this,0);
	double *pdX=NULL,*pdY=NULL,*pdZ=NULL,*pD;
	wchar_t wc;
	int iIndex,iIndexAxis,iCount1,iCount2;
	bool bSeparator;

	if(filedialog.DoModal()!=IDOK)
		return;
	CWaitCursor wait;
	strFileName = filedialog.GetPathName();
	try{
		CStdioFile file(strFileName,CFile::typeText | CFile::modeRead);
		iCount1 = 0;
		iCount2 = 0;
		file.SeekToBegin();
		while(file.ReadString(string))
			iCount1++;
		pdX = new double[iCount1];
		pdY = new double[iCount1];
		pdZ = new double[iCount1];
		if(pdX && pdY && pdZ)
		{
			file.SeekToBegin();
			while(file.ReadString(string))
			{
				iIndexAxis = 0;
				for(int iAxis=0;iAxis<3;iAxis++)
				{
					iIndex = 0;
					do{
						wc = string[iIndex++];
						if((wc==L'.') || ((L'0'<=wc) && (wc<=L'9')))
							bSeparator = false;
						else
							bSeparator = true;
					}while(!bSeparator);
					strAux = string.Mid(iIndexAxis,iIndex);
					switch(iAxis)
					{
					case 0: pD = pdX;break;
					case 1: pD = pdY;break;
					case 2: pD = pdZ;break;
					}
					if(swscanf_s(strAux,L"%lf",pD,sizeof(double))==1)
						iCount2++;
					do{
						wc = string[iIndex++];
						if((wc==L'.') || ((L'0'<=wc) && (wc<=L'9')))
							bSeparator = false;
						else
							bSeparator = true;
					}while(bSeparator);
					iIndexAxis += iIndex;
				}
			}
		}
		file.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	if(iCount2!=iCount1)
		AfxMessageBox(L"Error to load this file!");
	else if(!SetWizardMatrix(g_Wizard,iCount1,pdX,pdY,pdZ))
		AfxMessageBox(L"Error to load this file!");
	if(pdX)
		delete pdX;
	pdX = NULL;
	if(pdY)
		delete pdY;
	pdY = NULL;
	if(pdZ)
		delete pdZ;
	pdZ = NULL;
	UTKernel_SetFolderDataFiles(L"Matrix",strFileName.GetBuffer());
}

void CHWDlg::OnBnClickedButtonSave()
{
	CString string,strAux;
	CString strFileName;
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"Kernel");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	double *pdX=NULL,*pdY=NULL,*pdZ=NULL;
	int iCount1;

	if(filedialog.DoModal()!=IDOK)
		return;
	CWaitCursor wait;
	strFileName = filedialog.GetPathName();
	if(g_Wizard.Probe(0).GetWizardProbe()!=eCustom)
	{
		AfxMessageBox(L"Please use first the Toolbox or the button 'Load' to use matrix!");
		return;
	}
	iCount1 = 0;
	if(!GetWizardMatrix(g_Wizard,iCount1,NULL,NULL,NULL))
	{
		AfxMessageBox(L"Error to save this matrix!");
		return;
	}
	pdX = new double[iCount1];
	pdY = new double[iCount1];
	pdZ = new double[iCount1];
	if(!GetWizardMatrix(g_Wizard,iCount1,pdX,pdY,pdZ))
		AfxMessageBox(L"Error to save this matrix!");
	else
	try{
		CStdioFile file(strFileName,CFile::typeText | CFile::modeWrite | CFile::modeCreate);
		for(int iIndex=0;iIndex<iCount1;iIndex++)
		{
			string.Format(L"%lf\t%lf\t%lf\n",pdX[iIndex],pdY[iIndex],pdZ[iIndex]);
			file.WriteString(string);
		}
		file.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	if(pdX)
		delete pdX;
	pdX = NULL;
	if(pdY)
		delete pdY;
	pdY = NULL;
	if(pdZ)
		delete pdZ;
	pdZ = NULL;
	UTKernel_SetFolderDataFiles(L"Matrix",strFileName.GetBuffer());
}

void CHWDlg::OnBnClickedButtonConfig()
{
	g_RunToolbox.Run(GetSafeHwnd(),CHWDlg::CallbackThreadToolbox);
}

void CHWDlg::OnBnClickedCheckAutoResetacq()
{
	UpdateData();
}

void CHWDlg::OnBnClickedCheckInternal()
{
	UpdateData();
	if(m_bCheckInternal)
	{
	}
	OnBnClickedCheckEncoder();
}

void CHWDlg::OnBnClickedButtonCalibration()
{
	CDlgCalibration dlg(&g_pHW[0],this);

	dlg.DoModal();
}


void CHWDlg::OnBnClickedCheckHwLink()
{
	CHWDeviceOEMPA* pHWDeviceOEMPA=&g_pHW[m_iSystemIndex];
	CDlgHwLink dlg(pHWDeviceOEMPA,this);

	dlg.DoModal();
}

#ifdef _ENABLE_DISABLE_FILLING
void CHWDlg::OnBnClickedCheckFillingBetween()
{
	if(m_CheckFilingBetween.GetCheck()==BST_CHECKED)
		g_bFillingBetweenCycle = true;
	else
		g_bFillingBetweenCycle = false;
}
void CHWDlg::OnBnClickedCheckFillingLast()
{
	if(m_CheckFilingLast.GetCheck()==BST_CHECKED)
		g_bFillingLast = true;
	else
		g_bFillingLast = false;
}
#endif _ENABLE_DISABLE_FILLING
