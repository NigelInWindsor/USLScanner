#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OEMPAWizardExample.h"
#endif

#include <math.h>
#include "OEMPAWizardExampleDoc.h"
#include "OEMPAWizardExampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern wchar_t g_wcFolderData[MAX_PATH];
extern wchar_t g_wcFolderKernel[MAX_PATH];
extern wchar_t g_wcFolderEmuMon[MAX_PATH];

CHWDeviceOEMPA *g_pHWDeviceOEMPA=NULL;
CUTWizardSystem *g_pWizardSystem=NULL;
CUTChannels *g_pChannel=NULL;
wchar_t g_pDefault[]=L"&#@default@#&";
wchar_t g_pDefaultString2[]=L"&#@default@#&";

void NotifyErrorFile(wchar_t *pSection,wchar_t *pKey)
{
	wchar_t msg[MAX_PATH];

	swprintf(msg,MAX_PATH,L"Error in section '%s' for key '%s'!",pSection,pKey);
	MessageBox(NULL,msg,L"File Error",MB_OK);
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

bool FileReadString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *wcData,int iSize,bool bDisplayMsg=true);
bool FileReadString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *wcData,int iSize,bool bDisplayMsg)
{
	wchar_t data[MAX_PATH];

	GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	if(!wcscmp(data,g_pDefaultString2))
		return false;
	wcscpy_s(wcData,iSize,data);
	return true;
}

bool FileReadInteger(const wchar_t *pSection,const wchar_t *pKey,const wchar_t *pFileName,int &iData,bool bDisplayMsg,int iMin,int iMax)
{
	iData = GetPrivateProfileInt(pSection,pKey,-1,pFileName);
	if(iData==-1)
	{
		iData = GetPrivateProfileInt(pSection,pKey,0,pFileName);
		if(iData==0)
		{
			wchar_t pSection2[MAX_PATH];
			wchar_t pKey2[MAX_PATH];
			if(bDisplayMsg)
			{
				wcscpy_s(pSection2,MAX_PATH,pSection);
				wcscpy_s(pKey2,MAX_PATH,pKey);
				NotifyErrorFile(pSection2,pKey2);
			}
			return false;
		}
	}
	if((iData<iMin) || (iMax<iData))
	{
		if(bDisplayMsg)
		{
			wchar_t msg[MAX_PATH];
			swprintf(msg,MAX_PATH,L"Error in section '%s' for key '%s' out of range!",pSection,pKey);
			MessageBox(NULL,msg,L"File Error",MB_OK);
		}
	}
	return true;
}

bool FileReadEnum(const wchar_t *pSection,const wchar_t *pKey,const wchar_t *pFileName,wchar_t *pEnumList,bool &bExist,int &iEnum)
{
	wchar_t *pValue;
	bool bFound=false;
	wchar_t wcAux[MAX_PATH];

	bExist = true;
	GetPrivateProfileString(pSection,pKey,g_pDefault,wcAux,MAX_PATH,pFileName);
	if(!wcscmp(wcAux,g_pDefault))
	{
		bExist = false;
		return false;
	}
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
		return false;
	return true;
}

bool FileReadDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double &dData,bool bDisplayMsg)
{
	wchar_t data[MAX_PATH];

	GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	if(!wcscmp(data,g_pDefaultString2))
	{
		if(bDisplayMsg)
			NotifyErrorFile(pSection,pKey);
		return false;
	}
	dData = _wtof(data);
	dData *= dUnitFactor;
	return true;
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

bool FileReadArrayDepth(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,double dUnitFactor,double *pdData,int &iCountExpected)
{
	wchar_t pKeyBigData[MAX_PATH];
	wchar_t line[MAX_PATH];
	int iCount;
	wchar_t *pValue;
	double dData;

	iCount = 0;
	swprintf(pKeyBigData,MAX_PATH,L"%s",pKey);
	GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
	if(!wcscmp(line,g_pDefaultString2))
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
	if(iIndex>=0)
		string.Format(L"%s[%d]=%s\n",pKeyName,iIndex,pValue);
	else
		string.Format(L"%s=%s\n",pKeyName,pValue);
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

bool WizardDelete()
{
	bool bRet=true;

	if(g_pChannel && !CUTKernelFile::DeleteObject(g_pChannel,eRootTypeChannels))
		bRet = false;
	else
		g_pChannel = NULL;
	if(g_pWizardSystem && !CUTKernelFile::DeleteObject(g_pWizardSystem,eRootTypeWizardSystem))
		bRet = false;
	else
		g_pWizardSystem = NULL;
	Sleep(250);//to avoid crash due to internal latency of the kernel (in case of quick successive new/delete).
	return true;
}

bool WizardNew()
{
	wchar_t pAux[MAX_PATH];
	CUTKernelRoot *ptr;
	int iWizardProbeIndex=0;

	swprintf(pAux,MAX_PATH,L"OEMPA_system");
	ptr = CUTKernelFile::NewObject(eRootTypeWizardSystem,pAux);
	g_pWizardSystem = dynamic_cast<CUTWizardSystem*>(ptr);
	if(!g_pWizardSystem)
	{
		WizardDelete();
		return false;
	}
	swprintf(pAux,MAX_PATH,L"OEMPA_channel");
	ptr = CUTKernelFile::NewObject(eRootTypeChannels,pAux);
	g_pChannel = dynamic_cast<CUTChannels*>(ptr);
	if(!g_pChannel)
	{
		WizardDelete();
		return false;
	}
	Sleep(250);//to avoid crash due to internal latency of the kernel (in case of quick successive new/delete).
	return true;
}

bool DeviceOEMPADelete()
{
	bool bRet=true;
	if(g_pHWDeviceOEMPA)
		delete g_pHWDeviceOEMPA;
	else
		bRet = false;
	g_pHWDeviceOEMPA = NULL;
	return bRet;
}

bool DeviceOEMPANew(bool bHW)
{
	CString strFileName;

	strFileName = g_wcFolderEmuMon;
	strFileName += L"WizardTemplateFlashFile.txt";
	//WritePrivateProfileString(L"",L"",L"",strFileName);
	if(!bHW)
		g_pHWDeviceOEMPA = new CHWDeviceOEMPA(strFileName);
	else
		g_pHWDeviceOEMPA = new CHWDeviceOEMPA();
	return true;
}

bool WizardLink()
{
	if(!g_pWizardSystem)
		return false;
	if(!g_pChannel)
		return false;
	if(!g_pChannel->LinkWizardProbe(0,g_pWizardSystem,100))
		WizardDelete();
	return true;
}

bool WINAPI OEMPA_WriteWizard(
					bool bLongitudinalWave,double dSpecimenVelocity,
					int iProbeElementCount,double dProbePitch,double dProbeFrequency,
					bool bWedgeEnable,
					double dWedgeHeight,double dWedgeAngle,double dWedgeVelocity,
					double dScanDepthEmission,double *pScanDepthReception,int iScanDepthReceptionCount,
					int iScanElementCount,
					bool bScanLinear,
					int iScanElementStart,int iScanElementStop,int iScanElementStep,
					bool bScanAngle,
					double dScanAngleStart,double dScanAngleStop,double dScanAngleStep,
					double dAscanStart,double dAscanRange,double dTimeSlot,
					int iCscanCount,double *pdCscanStart,double *pdCscanRange,double *pdCscanThreshold,
					enumRectification *peRectification,enumGateModeAmp *peModeAmp,enumGateModeTof *peModeTof)
{
	bool bRet=true;
	CUTWizardProbe *pWizardProbe;
	CUTWizardWedge *pWizardWedge;
	CUTScan *pScan;
	CUTGate *pGate;
	double dAscanStop;
	CUTWizardSystem *pSystem=g_pWizardSystem;
	CUTChannels *pChannels=g_pChannel;

	if(!pSystem || !pChannels)
		return false;

	if(pChannels->IsMultiChannels())
		return false;
	
	pScan = &pChannels->Probe().Scan();//single channel
	if(!pScan)
		return false;
	pWizardProbe = &pSystem->Probe(0);
	if(!pWizardProbe)
		return false;
	pWizardWedge = dynamic_cast<CUTWizardWedge*>(pWizardProbe);
	if(!pWizardWedge)
		return false;

	if(bLongitudinalWave)
	{
		if(pScan->SetWave(eLongitudinal))
			bRet = false;
		if(pSystem->Specimen().SetVelocity(eLongitudinal,dSpecimenVelocity))
			bRet = false;
	}else{
		if(pScan->SetWave(eTransverse))
			bRet = false;
		if(pSystem->Specimen().SetVelocity(eTransverse,dSpecimenVelocity))
			bRet = false;
	}

	if(pWizardProbe->SetCount(iProbeElementCount))
		bRet = false;
	if(pWizardProbe->SetPitch(dProbePitch))
		bRet = false;
	if(pWizardProbe->SetFrequency(dProbeFrequency))
		bRet = false;
	if(bWedgeEnable)
	{
		if(pWizardWedge->SetEnable(true))
			bRet = false;
		if(pWizardWedge->SetVelocity(dWedgeVelocity))
			bRet = false;
		if(pWizardWedge->SetOffset(eZ,dWedgeHeight))
			bRet = false;
		if(pWizardWedge->SetAngles(0.0,dWedgeAngle,0.0))
			bRet = false;
	}else{
		if(pWizardWedge->SetEnable(false))
			bRet = false;
	}

	if(pScan->VectorStart().SetDepthEmission(dScanDepthEmission))
		bRet = false;
	if(pScan->VectorStart().SetFocalLawCount(iScanDepthReceptionCount))
		bRet = false;
	for(int iIndex=0;iIndex<iScanDepthReceptionCount;iIndex++)
	{
		if(pScan->VectorStart().SetDepth(iIndex,pScanDepthReception[iIndex]))
			bRet = false;
	}
	if(pScan->VectorStart().SetCount(iScanElementCount))
		bRet = false;
	if(bScanLinear)
	{
		if(pScan->VectorStart().SetFirst(iScanElementStart))
			bRet = false;
		if(pScan->VectorStop().SetFirst(iScanElementStop))
			bRet = false;
		if(pScan->VectorStep().SetFirst(iScanElementStep))
			bRet = false;
	}else{
		if(pScan->VectorStart().SetFirst(iScanElementStart))
			bRet = false;
		if(pScan->VectorStop().SetFirst(iScanElementStart))
			bRet = false;
		if(pScan->VectorStep().SetFirst(0))
			bRet = false;
	}
	if(bScanAngle)
	{
		if(pScan->VectorStart().SetAngle(dScanAngleStart))
			bRet = false;
		if(pScan->VectorStop().SetAngle(dScanAngleStop))
			bRet = false;
		if(pScan->VectorStep().SetAngle(dScanAngleStep))
			bRet = false;
	}else{
		if(pScan->VectorStart().SetAngle(dScanAngleStart))
			bRet = false;
		if(pScan->VectorStop().SetAngle(dScanAngleStart))
			bRet = false;
		if(pScan->VectorStep().SetAngle(M_PI/180.0))
			bRet = false;
	}

	if(pScan->GetReceiverCycleCount())
	{
		dAscanStop = dAscanRange+dAscanStart;
		//1:1:4:1
		//if((*pScan)[0].Receiver().SetStart(dAscanStart))
		//	bRet = false;
		//if((*pScan)[0].Receiver().SetStop(dAscanStop))
		//	bRet = false;
		if((*pScan)[0].Receiver().SetStartStop(dAscanStart,dAscanStop))
			bRet = false;
		if((*pScan)[0].Pulser().SetTimer(dTimeSlot))
			bRet = false;
		for(int iIndex=0;iIndex<pScan->GetReceiverCycleCount();iIndex++)
		{
			(*pScan)[iIndex].Receiver().SetCscanCount(iCscanCount);
			for(int iGate=0;iGate<iCscanCount;iGate++)
			{
				pGate = &(*pScan)[iIndex].Receiver().CscanGate(iGate);
				if(pGate->SetGate(pdCscanStart[iGate],pdCscanRange[iGate]))
					bRet = false;
				if(pGate->SetThreshold(pdCscanThreshold[iGate]))
					bRet = false;
				if(pGate->SetModeAmplitude(peModeAmp[iGate]))
					bRet = false;
				if(pGate->SetModeTimeOfFlight(peModeTof[iGate]))
					bRet = false;
				if(pGate->SetRectification(peRectification[iGate]))
					bRet = false;
			}
		}
	}

	return bRet;
}

bool WINAPI OEMPA_ReadWizard(
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
					enumRectification *peRectification,enumGateModeAmp *peModeAmp,enumGateModeTof *peModeTof,int &iCscanCount)
{
	bool bRet=true;
	CUTWizardProbe *pWizardProbe;
	CUTWizardWedge *pWizardWedge;
	CUTScan *pScan;
	double dWedgeSkew,dWedgeTilt;
	CUTGate *pGate;
	CUTWizardSystem *pSystem=g_pWizardSystem;
	CUTChannels *pChannels=g_pChannel;

	if(!pSystem || !pChannels)
		return false;

	if(pChannels->IsMultiChannels())
		return false;
	
	pScan = &pChannels->Probe().Scan();//single channel
	if(!pScan)
		return false;
	pWizardProbe = &pSystem->Probe(0);
	if(!pWizardProbe)
		return false;
	pWizardWedge = dynamic_cast<CUTWizardWedge*>(pWizardProbe);
	if(!pWizardWedge)
		return false;

	if(pScan->GetWave()==eLongitudinal)
	{
		bLongitudinalWave = true;
		dSpecimenVelocity = pSystem->Specimen().GetVelocity(eLongitudinal);
	}else{
		bLongitudinalWave = false;
		dSpecimenVelocity = pSystem->Specimen().GetVelocity(eTransverse);
	}

	iProbeElementCount = pWizardProbe->GetCount();
	if(!pWizardProbe->GetPitch(dProbePitch))
		bRet = false;
	dProbeFrequency = pWizardProbe->GetFrequency();
	bWedgeEnable = pWizardWedge->IsEnabled();
	if(bWedgeEnable)
	{
		dWedgeVelocity = pWizardWedge->GetVelocity();
		if(!pWizardWedge->GetOffset(eZ,dWedgeHeight))
			bRet = false;
		if(!pWizardWedge->GetAngles(dWedgeSkew,dWedgeAngle,dWedgeTilt))
			bRet = false;
	}else{
		dWedgeVelocity = 1480.0;
		dWedgeHeight = 0.0;
		dWedgeAngle = 0.0;
	}

	dScanDepthEmission = pScan->VectorStart().GetDepthEmission();
	iScanDepthReceptionCount = pScan->VectorStart().GetFocalLawCount();
	if(iScanDepthReceptionCount>iScanDepthReceptionMax)
		iScanDepthReceptionCount = iScanDepthReceptionMax;
	for(int iIndex=0;iIndex<iScanDepthReceptionCount;iIndex++)
	{
		pScanDepthReception[iIndex] = pScan->VectorStart().GetDepth(iIndex);
	}
	iScanElementCount = pScan->VectorStart().GetCount();
	if(!pScan->VectorStart().GetFirst(iScanElementStart))
		bRet = false;
	if(!pScan->VectorStop().GetFirst(iScanElementStop))
		bRet = false;
	if(!pScan->VectorStep().GetFirst(iScanElementStep))
		bRet = false;
	dScanAngleStart = pScan->VectorStart().GetAngle();
	dScanAngleStop = pScan->VectorStop().GetAngle();
	dScanAngleStep = pScan->VectorStep().GetAngle();

	for(int iGate=0;iGate<iCscanMax;iGate++)
	{
		pdCscanStart[iGate] = 0.0;
		pdCscanRange[iGate] = 1.0e-6;
		pdCscanThreshold[iGate] = 50.0;
		peRectification[iGate] = eSigned;
		peModeAmp[iGate] = eAmpAbsolute;
		peModeTof[iGate] = eTofAmplitudeDetection;
	}
	if(pScan->GetReceiverCycleCount())
	{
		dAscanStart = (*pScan)[0].Receiver().GetStart();
		dAscanRange = (*pScan)[0].Receiver().GetStop();
		dAscanRange -= dAscanStart;
		dTimeSlot = (*pScan)[0].Pulser().GetTimer();
		iCscanCount = (*pScan)[0].Receiver().GetCscanCount();
		for(int iGate=0;iGate<iCscanCount;iGate++)
		{
			pGate = &(*pScan)[0].Receiver().CscanGate(iGate);
			pdCscanStart[iGate] = pGate->GetStart();
			pdCscanRange[iGate] = pGate->GetStop()-pGate->GetStart();
			pdCscanThreshold[iGate] = pGate->GetThreshold();
			peRectification[iGate] = pGate->GetRectification();
			peModeAmp[iGate] = pGate->GetModeAmplitude();
			peModeTof[iGate] = pGate->GetModeTimeOfFlight();
		}
	}else{
		dAscanStart = 0.0e-6;
		dAscanRange = 30.0e-6;
		dTimeSlot = 2500.0e-6;
		iCscanCount = 0;
	}

	return bRet;
}

bool WINAPI OEMPA_WriteWizard(int iWizardProbeCount,bool *pbWizardProbeWedgeEnable,int iProbeCount,int *piProbeWizardIndex,int *piProbeChannelCount)
{
	bool bRet=true;
	CUTWizardWedge *pWizardWedge;
	CUTProbe *pProbe;
	CUTWizardSystem *pSystem=g_pWizardSystem;
	CUTChannels *pChannels=g_pChannel;
	int iProbeCount2,iWizardSystemRootId;

	if(!pSystem || !pChannels || !pSystem->GetID(iWizardSystemRootId))
		return false;

	if(!pChannels->IsMultiChannels())
		return false;

	if(pSystem->SetCount(iWizardProbeCount))
		return false;
	for(int iWizardProbeIndex=0;iWizardProbeIndex<iWizardProbeCount;iWizardProbeIndex++)
	{
		pWizardWedge = &pSystem->Wedge(iWizardProbeIndex);
		if(pbWizardProbeWedgeEnable)
			pWizardWedge->SetEnable(pbWizardProbeWedgeEnable[iWizardProbeIndex]);
	}

	do{
		iProbeCount2 = pChannels->GetProbeCount();
		if(iProbeCount2<iProbeCount)
		{
			if(pChannels->ProbeCountIncrement())
				return false;
		}
		if(iProbeCount2>iProbeCount)
		{
			if(pChannels->ProbeCountDecrement())
				return false;
		}
	}while(iProbeCount2!=iProbeCount);

	for(int iProbeIndex=0;iProbeIndex<iProbeCount;iProbeIndex++)
	{
		pProbe = &pChannels->Probe(iProbeIndex);
		if(piProbeWizardIndex)
		{
			if(piProbeWizardIndex[iProbeIndex]!=-1)
			{
				if(!pProbe->SetWizardSystemId(false,pSystem,piProbeWizardIndex[iProbeIndex]))
					return false;
			}else{
				if(!pProbe->SetWizardSystemId(false,L""))
					return false;
			}
		}
		if(piProbeChannelCount)
		{
			if(pProbe->SetScanCount(piProbeChannelCount[iProbeIndex]))
				return false;
		}
	}

	return bRet;
}

bool WINAPI OEMPA_ReadWizard(int iWizardProbeMax,int &iWizardProbeCount,bool *pbWizardProbeWedgeEnable,int iProbeMax,int &iProbeCount,int *piProbeWizardIndex,int *piProbeChannelCount)
{
	bool bRet=true;
	CUTWizardWedge *pWizardWedge;
	CUTProbe *pProbe;
	CUTWizardSystem *pSystem=g_pWizardSystem;
	CUTChannels *pChannels=g_pChannel;
	int iWizardSystemRootId;

	if(!pSystem || !pChannels || !pSystem->GetID(iWizardSystemRootId))
		return false;

	if(!pChannels->IsMultiChannels())
		return false;

	iWizardProbeCount = pSystem->GetCount();
	for(int iWizardProbeIndex=0;iWizardProbeIndex<iWizardProbeCount;iWizardProbeIndex++)
	{
		pWizardWedge = &pSystem->Wedge(iWizardProbeIndex);
		if(pbWizardProbeWedgeEnable && (iWizardProbeIndex<iWizardProbeMax))
			pbWizardProbeWedgeEnable[iWizardProbeIndex] = pWizardWedge->IsEnabled();
	}

	iProbeCount = pChannels->GetProbeCount();
	for(int iProbeIndex=0;iProbeIndex<iProbeCount;iProbeIndex++)
	{
		pProbe = &pChannels->Probe(iProbeIndex);
		if(iProbeIndex<iProbeMax)
		{
			if(piProbeWizardIndex)
				piProbeWizardIndex[iProbeIndex] = pProbe->GetWizardProbeIndex(false);
			if(piProbeChannelCount)
				piProbeChannelCount[iProbeIndex] = pProbe->GetScanCount();
		}
	}

	return bRet;
}

bool WizardUpdateScan(int *piErrorChannelProbe,int *piErrorChannelScan)
{
	CUTProbe *pProbe;
	CUTScan *pScan;
	int iProbeCount,iScanCount;
	bool bRet=true;

	if(piErrorChannelProbe)
		*piErrorChannelProbe = -1;
	if(piErrorChannelScan)
		*piErrorChannelScan = -1;
	if(!g_pChannel)
		return false;
	if(!CUTChannels::IsDefaultMultiChannels())
	{
		if(!(*g_pChannel).Probe().Scan().SetScanStatusOutOfDate())
			return false;
		return (*g_pChannel).Probe().Scan().UpdateScan();//UpdateScan
	}else{
		for(int iStep=0;iStep<2;iStep++)
		{
			iProbeCount = g_pChannel->GetProbeCount();
			for(int iProbeIndex=0;iProbeIndex<iProbeCount;iProbeIndex++)
			{
				pProbe = &g_pChannel->Probe(iProbeIndex);
				iScanCount = pProbe->GetScanCount();
				for(int iScanIndex=0;iScanIndex<iScanCount;iScanIndex++)
				{
					pScan = &pProbe->Scan(iScanIndex);
					if((iStep==0) && !pScan->SetScanStatusOutOfDate())
						bRet = false;
					if((iStep==1) && !pScan->UpdateScan())
					{
						bRet = false;
						if(piErrorChannelProbe && (*piErrorChannelProbe==-1))
							*piErrorChannelProbe = iProbeIndex;
						if(piErrorChannelScan && (*piErrorChannelScan==-1))
							*piErrorChannelScan = iScanIndex;
					}
				}
			}
		}
	}
	return bRet;
}

bool IsWizardScanUpToDate()
{
	CUTProbe *pProbe;
	CUTScan *pScan;
	int iProbeCount,iScanCount;
	bool bRet=true;

	if(!g_pChannel)
		return false;
	if(!CUTChannels::IsDefaultMultiChannels())
		return (*g_pChannel).Probe().Scan().IsScanStatusUpToDate();
	else{
		iProbeCount = g_pChannel->GetProbeCount();
		for(int iProbeIndex=0;iProbeIndex<iProbeCount;iProbeIndex++)
		{
			pProbe = &g_pChannel->Probe(iProbeIndex);
			iScanCount = pProbe->GetScanCount();
			for(int iScanIndex=0;iScanIndex<iScanCount;iScanIndex++)
			{
				pScan = &pProbe->Scan(iScanIndex);
				if(!pScan->IsScanStatusUpToDate())
				{
					bRet = false;
					break;
				}
			}
		}
	}
	return bRet;
}

structCorrectionOEMPA *g_pCorrection=NULL;
structCorrectionOEMPA* WINAPI CallbackCustomizedWizardAPI(CHWDeviceOEMPA *pOEMPA,const wchar_t *pFileName,enumStepCustomizedWizardAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	return g_pCorrection;
}

bool OEMPA_ReadWizardWriteFile(const wchar_t *pFileName)
{
	return OEMPA_ReadWizardWriteFile(g_pHWDeviceOEMPA,g_pWizardSystem,g_pChannel,pFileName);
}

bool WizardToHw()
{
	bool bRet=true;
	structRoot root;
	structCycle *pCycle=NULL;
	CFocalLaw *pEmission=NULL;
	CFocalLaw *pReception=NULL;

	if(!OEMPA_ReadWizard(g_pHWDeviceOEMPA,g_pWizardSystem,g_pChannel,root,pCycle,pEmission,pReception))
	{
		AfxMessageBox(L"Error from the wizard!");
		bRet = false;goto end;
	}

	if(!OEMPA_WriteHW(g_pHWDeviceOEMPA,root,pCycle,pEmission,pReception))
	{
		AfxMessageBox(L"Error to write HW!");
		bRet = false;goto end;
	}

end:
	OEMPA_Free(pCycle,pEmission,pReception);

	return bRet;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//bool FileReadString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *wcData,int iSize,bool bDisplayMsg=true);
bool FileReadInteger(const wchar_t *pSection,const wchar_t *pKey,const wchar_t *pFileName,int &iData,bool bDisplayMsg=true,int iMin=INT_MIN,int iMax=INT_MAX);
bool FileReadEnum(const wchar_t *pSection,const wchar_t *pKey,const wchar_t *pFileName,wchar_t *pEnumList,bool &bExist,int &iEnum);
bool FileReadDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double &dData,bool bDisplayMsg=true);
bool FileReadArrayDepth(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,double dUnitFactor,double *pdData,int &iCountExpected);

void WizardWriteEnum(CStdioFile &file,wchar_t *pKeyName,int iIndex,wchar_t *pEnumList,int iEnum);
void WizardWriteEnumList(CStdioFile &file,wchar_t *pEnumList);

const wchar_t *GetOEMPAWizardExamplePath()
{
	if(CUTChannels::IsDefaultMultiChannels())
		return L"OEMPAWizardExampleMultiChannel.txt";
	else
		return L"OEMPAWizardExampleSingleChannel.txt";
}

bool ReadWizardWriteTemplateSingleChannel(bool bOverwrite)
{
	CString strFileName,string;
	bool bLongitudinalWave;double dSpecimenVelocity;
	int iProbeElementCount;double dProbePitch,dProbeFrequency;
	bool bWedgeEnable;
	double dWedgeHeight,dWedgeAngle,dWedgeVelocity;
	double dScanDepthEmission,adScanDepthReception[32];
	int iScanDepthReception,iScanElementCount;
	int iScanElementStart,iScanElementStop,iScanElementStep;
	double dScanAngleStart,dScanAngleStop,dScanAngleStep;
	double dAscanStart,dAscanRange,dTimeSlot;
	double adCscanStart[4],adCscanRange[4],adCscanThreshold[4];
	enumRectification aeRectification[4];
	enumGateModeAmp aeModeAmp[4];
	enumGateModeTof aeModeTof[4];
	int iCscanCount;
	bool bLinear=true;
	bool bExist;

	if(!OEMPA_ReadWizard(
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
					aeRectification,aeModeAmp,aeModeTof,iCscanCount))
		return false;
	iProbeElementCount = 64;
	iScanElementStop = iProbeElementCount-1;
	iScanElementStart++;
	iScanElementStop++;
	strFileName = g_wcFolderKernel;
	strFileName += GetOEMPAWizardExamplePath();
	try{
		CStdioFile file(strFileName,CFile::typeText | CFile::modeRead);
		file.Close();
		bExist = true;
	}catch(CFileException *e){
		e->Delete();
		bExist = false;
	}
	if(bExist && !bOverwrite)
		return true;
	try{
		CStdioFile file(strFileName,CFile::typeText | CFile::modeWrite | CFile::modeCreate);
		file.WriteString(L"# TEMPLATE - SINGLE-CHANNEL - LINEAR\n");
		file.WriteString(L"#Step 1: Fill in following input parameters.\n");
		file.WriteString(L"#Step 2: use the menu \"Save As... (OEMPA file format)\" to apply those parameters and to save the scanning.\n");
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
			string.Format(L" mm\n");
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
	//EditFile(strFileName.GetBuffer(),true);
	return true;
}

bool ReadTemplateWriteWizardSingleChannel()
{
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
	double dAscanStart,dAscanRange,dTimeSlot;
	double adCscanStart[4],adCscanRange[4],adCscanThreshold[4];
	enumRectification aeRectification[4];
	enumGateModeAmp aeModeAmp[4];
	enumGateModeTof aeModeTof[4];
	int iCscanCount,iEnum;
	bool bLinear=true;
	bool bExist;

	strFileName = g_wcFolderKernel;
	strFileName += GetOEMPAWizardExamplePath();
	dSpecimenVelocity = 6400.0;
	iCscanCount = 0;
	wcscpy_s(wcWave,50,L"L");
	iWedgeEnable = 0;
	dWedgeVelocity = 1500.0;
	dWedgeAngle = 0.0;
	dWedgeHeight = 0.0;
	bWedgeEnable = false;
	iProbeElementCount = 64;
	dScanAngleStart = 0.0;
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
	}
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
		string.Format(L"Rectification[%d]",iGate);
		if(!FileReadEnum(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),L"Signed"L"\0"L"Unsigned"L"\0"L"UnsignedPositive"L"\0"L"UnsignedNegative"L"\0",bExist,iEnum))
			return false;
		aeRectification[iGate] = (enumRectification)iEnum;
		string.Format(L"ModeAmp[%d]",iGate);
		if(!FileReadEnum(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),L"Absolute"L"\0"L"Maximum"L"\0"L"Minimum"L"\0"L"PeakToPeak"L"\0",bExist,iEnum))
			return false;
		aeModeAmp[iGate] = (enumGateModeAmp)iEnum;
		string.Format(L"ModeTof[%d]",iGate);
		if(!FileReadEnum(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),L"AmplitudeDetection"L"\0"L"ThresholdCross"L"\0"L"ZeroFirstAfterThresholdCross"L"\0"L"ZeroLastBeforeThresholdCross"L"\0",bExist,iEnum))
			return false;
		aeModeTof[iGate] = (enumGateModeTof)iEnum;
	}
	//if(AfxMessageBox(L"Do you want to complete ?",MB_YESNO)==IDNO)
	//	return false;
	CWaitCursor wait;
	if(!OEMPA_WriteWizard(
						bLongitudinalWave,dSpecimenVelocity,
						iProbeElementCount,dProbePitch,dProbeFrequency,
						bWedgeEnable,
						dWedgeHeight,dWedgeAngle,dWedgeVelocity,
						dScanDepthEmission,adScanDepthReception,iScanDepthReception,
						iScanElementCount,
						bLinear,iScanElementStart,iScanElementStop,iScanElementStep,
						!bLinear,dScanAngleStart,dScanAngleStop,dScanAngleStep,
						dAscanStart,dAscanRange,dTimeSlot,
						iCscanCount,adCscanStart,adCscanRange,adCscanThreshold,
						aeRectification,aeModeAmp,aeModeTof))
	{
		AfxMessageBox(L"Error");
		return false;
	}
	return true;
	//return true;
}

bool ReadWizardWriteTemplateMultiChannel(bool bOverwrite)
{
	CString strFileName,string;
	int iWizardProbeCount;
	int iProbeCount;
	bool pbWizardProbeWedgeEnable[8];
	int piProbeWizardIndex[32],piProbeChannelCount[32];
	bool bExist;

	if(!OEMPA_ReadWizard(8,iWizardProbeCount,pbWizardProbeWedgeEnable,32,iProbeCount,piProbeWizardIndex,piProbeChannelCount))
		return false;
	strFileName = g_wcFolderKernel;
	strFileName += GetOEMPAWizardExamplePath();
	try{
		CStdioFile file(strFileName,CFile::typeText | CFile::modeRead);
		file.Close();
		bExist = true;
	}catch(CFileException *e){
		e->Delete();
		bExist = false;
	}
	if(bExist && !bOverwrite)
		return true;
	try{
		CStdioFile file(strFileName,CFile::typeText | CFile::modeWrite | CFile::modeCreate);
		file.WriteString(L"# TEMPLATE - MULTI-CHANNEL\n");
		file.WriteString(L"#Step 1: Fill in the following main input parameters.\n");
		file.WriteString(L"#Step 2: To apply those parameters: use the menu \"Save\" OR press the button \"Write\" of the dialog \"HW & Kernel\".\n");
		file.WriteString(L"#Step 3: press the button \"Edit\" of the dialog \"HW & Kernel\" to edit each scanning.\n");
		file.WriteString(L"#Step 5: use the menu \"Save As... (OEMPA file format)\" to save the final multi-channel scanning.\n");
		file.WriteString(L"#Step 6: Run \"OEMPATool\" to load it and to display the result.\n");
		file.WriteString(L"\n");
		file.WriteString(L"[WizardSystem]\n");
		file.WriteString(L"#\"WizardProbeCount\" is probe count for the wizard.\n");
		string.Format(L"WizardProbeCount=%d\n",iWizardProbeCount);
		file.WriteString(string);
		file.WriteString(L"#\"WizardWedgeEnable_X=Y\" is to enable each wedge:\n");
		file.WriteString(L"#  - X (index) range is 0 to WizardProbeCount-1.\n");
		file.WriteString(L"#  - Y (value) is 0 (no wedge) or 1 (wedge enabled).\n");
		file.WriteString(L"#  - This item is NOT REQUIRED, you can skip it.\n");
		for(int iWizardWedgeIndex=0;iWizardWedgeIndex<iWizardProbeCount;iWizardWedgeIndex++)
		{
			string.Format(L"WizardWedgeEnable_%d=%s\n",iWizardWedgeIndex,pbWizardProbeWedgeEnable[iWizardWedgeIndex]?L"1":L"0");
			file.WriteString(string);
		}
		file.WriteString(L"\n");
		file.WriteString(L"[Channels]\n");
		file.WriteString(L"#\"ProbeCount\" is the probe count on the connector.\n");
		string.Format(L"ProbeCount=%d\n",iProbeCount);
		file.WriteString(string);
		file.WriteString(L"#\"WizardProbeIndex_X=Y\" is the index of the wizard probe:\n");
		file.WriteString(L"#  - X (index) range is 0 to ProbeCount-1.\n");
		file.WriteString(L"#  - Y (value) range is 0 to WizardProbeCount-1 (range 100 to WizardProbeCount+100-1 is the same).\n");
		file.WriteString(L"#  - This item is NOT REQUIRED, you can skip it.\n");
		for(int iProbeIndex=0;iProbeIndex<iProbeCount;iProbeIndex++)
		{
			string.Format(L"WizardProbeIndex_%d=%d\n",iProbeIndex,piProbeWizardIndex[iProbeIndex]);
			file.WriteString(string);
		}
		file.WriteString(L"#\"ScanCount_X=Y\" is the count of scanning for each probe.\n");
		file.WriteString(L"#  - X (index) range is 0 to ProbeCount-1.\n");
		file.WriteString(L"#  - Y (value) is the logical channel count (scanning count).\n");
		for(int iProbeIndex=0;iProbeIndex<iProbeCount;iProbeIndex++)
		{
			string.Format(L"ScanCount_%d=%d\n",iProbeIndex,piProbeChannelCount[iProbeIndex]);
			file.WriteString(string);
		}

		file.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	//EditFile(strFileName.GetBuffer(),true);
	return true;
}

bool ReadTemplateWriteWizardMultiChannel()
{
	CString strFileName,string;
	int iWizardProbeCount,iProbeCount,iAux;
	bool pbWizardProbeWedgeEnable[8];
	int piProbeWizardIndex[32],piProbeChannelCount[32];

	strFileName = g_wcFolderKernel;
	strFileName += GetOEMPAWizardExamplePath();
	if(!FileReadInteger(L"WizardSystem",L"WizardProbeCount",strFileName.GetBuffer(),iWizardProbeCount,true,1,8))
		return false;
	for(int iWizardWedgeIndex=0;iWizardWedgeIndex<iWizardProbeCount;iWizardWedgeIndex++)
	{
		string.Format(L"WizardWedgeEnable_%d",iWizardWedgeIndex);
		if(FileReadInteger(L"WizardSystem",string,strFileName.GetBuffer(),iAux,false))
			pbWizardProbeWedgeEnable[iWizardWedgeIndex] = iAux?true:false;
		else
			pbWizardProbeWedgeEnable[iWizardWedgeIndex] = false;
	}
	if(!FileReadInteger(L"Channels",L"ProbeCount",strFileName.GetBuffer(),iProbeCount,true))
		return false;
	for(int iProbeIndex=0;iProbeIndex<iProbeCount;iProbeIndex++)
	{
		string.Format(L"WizardProbeIndex_%d",iProbeIndex);
		if(FileReadInteger(L"Channels",string,strFileName.GetBuffer(),iAux,false))
		{
			if(iAux>=0)
				piProbeWizardIndex[iProbeIndex] = iAux;
			else
				piProbeWizardIndex[iProbeIndex] = 100+iProbeIndex;
		}else
			piProbeWizardIndex[iProbeIndex] = 100+iProbeIndex;
		string.Format(L"ScanCount_%d",iProbeIndex);
		if(!FileReadInteger(L"Channels",string,strFileName.GetBuffer(),iAux,true,1,32))
			return false;
		piProbeChannelCount[iProbeIndex] = iAux;
	}
	CWaitCursor wait;
	if(!OEMPA_WriteWizard(iWizardProbeCount,pbWizardProbeWedgeEnable,iProbeCount,piProbeWizardIndex,piProbeChannelCount))
	{
		AfxMessageBox(L"Error");
		return false;
	}
	return true;
}

bool ReadWizardWriteTemplate(bool bOverwrite)
{
	if(CUTChannels::IsDefaultMultiChannels())
		return ReadWizardWriteTemplateMultiChannel(bOverwrite);
	else
		return ReadWizardWriteTemplateSingleChannel(bOverwrite);
}

bool ReadTemplateWriteWizard()
{
	bool bRet;

	if(CUTChannels::IsDefaultMultiChannels())
	{
		bRet = ReadTemplateWriteWizardMultiChannel();
		if(!bRet)
			ReadWizardWriteTemplateMultiChannel(true);
	}else{
		WizardLink();//link between the single CUTChannels with the single CUTWizardProbe, for MultiChannel the link is performed in "ReadTemplateWriteWizardMultiChannel".
		bRet = ReadTemplateWriteWizardSingleChannel();
		if(!bRet)
			ReadWizardWriteTemplateSingleChannel(true);
	}
	return bRet;
}

