
#include "stdafx.h"
#define _USE_MATH_DEFINES 
#include <tlhelp32.h> 
#include <stdio.h> 
#include <math.h> 

#define _USE_CALLBACK_ARRAY_

//#define MICRO_SECOND			L"µs"
#define MICRO_SECOND			L"us"

void NewLineWizard(const wchar_t *pFileName)
{
#ifdef _USE_MFC_
	try{
		CStdioFile file(pFileName,CFile::typeText | CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		file.SeekToEnd();
		file.WriteString(L"\n");
#else _USE_MFC_
		myCFile file(pFileName,myCFile::typeText | myCFile::modeCreate | myCFile::modeNoTruncate | myCFile::modeWrite);
		file.SeekToEnd();
		file.Write(L"\r\n",2);
#endif _USE_MFC_
		file.Close();
#ifdef _USE_MFC_
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
#endif _USE_MFC_
}

void FillNewLineWizard(const wchar_t *pFileName)
{
	char *pData=NULL;
	DWORD dwLength=0;
	bool bError=false;

#ifdef _USE_MFC_
	try{
		CStdioFile file(pFileName,CFile::typeBinary | CFile::modeRead);
#else _USE_MFC_
		myCFile file(pFileName,myCFile::typeBinary | myCFile::modeRead);
#endif _USE_MFC_
		dwLength = (DWORD)file.GetLength();
		pData = new char[dwLength];
		if(!pData || (file.Read(pData,dwLength)!=dwLength))
			bError = true;
		file.Close();
#ifdef _USE_MFC_
	}catch(CFileException *e){
		bError = true;
		e->ReportError();
		e->Delete();
	}
#endif _USE_MFC_
	if(bError)
	{
		if(pData)
			delete pData;
		return;
	}
#ifdef _USE_MFC_
	try{
		CStdioFile file2(pFileName,CFile::typeBinary | CFile::modeCreate | CFile::modeWrite);
#else _USE_MFC_
		myCFile file2(pFileName,myCFile::typeBinary | myCFile::modeCreate | myCFile::modeWrite);
#endif _USE_MFC_
		for(int iIndex=0;iIndex<(int)dwLength;iIndex++)
		{
			if(pData[iIndex]=='[')
			{
				if(iIndex>=4)
				{
					if(!((pData[iIndex-4]=='\r') && (pData[iIndex-3]=='\n') && (pData[iIndex-2]=='\r') && (pData[iIndex-1]=='\n')))
						file2.Write("\r\n",2);
				}
			}
			file2.Write(&pData[iIndex],1);
		}
		file2.Close();
#ifdef _USE_MFC_
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
#endif _USE_MFC_
	if(pData)
		delete pData;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

bool GetObjectWizard(	int iWizardSystemId,int iWizardChannelId,
						bool &bCreateSystem,bool &bCreateChannel,
						CUTWizardSystem* &pSystem,CUTChannels* &pChannels)
{
	CUTKernelRoot *ptr;

	ptr = CUTKernelRoot::NewIfRequired(eRootTypeWizardSystem,iWizardSystemId,bCreateSystem);
	pSystem = dynamic_cast<CUTWizardSystem*>(ptr);
	if(!pSystem)
	{
		OempaApiMessageBox(NULL,L"Bad object in the kernel!",L"Kernel Error",MB_OK);
		return false;
	}
	ptr = CUTKernelRoot::NewIfRequired(eRootTypeChannels,iWizardChannelId,bCreateChannel);
	pChannels = dynamic_cast<CUTChannels*>(ptr);
	if(!pChannels)
	{
		ptr = dynamic_cast<CUTKernelRoot*>(pSystem);
		CUTKernelRoot::DeleteIfRequired(eRootTypeWizardSystem,bCreateSystem,ptr);
		OempaApiMessageBox(NULL,L"Bad object in the kernel!",L"Kernel Error",MB_OK);
		return false;
	}
	return true;
}

void ReleaseObjectWizard(	bool bCreateSystem,bool bCreateChannel,
							CUTWizardSystem *pSystem,CUTChannels *pChannels)
{
	CUTKernelRoot *ptr;

	if(pChannels)
	{
		ptr = dynamic_cast<CUTKernelRoot*>(pChannels);
		CUTKernelRoot::DeleteIfRequired(eRootTypeChannels,bCreateChannel,ptr);
	}
	if(pSystem)
	{
		ptr = dynamic_cast<CUTKernelRoot*>(pSystem);
		CUTKernelRoot::DeleteIfRequired(eRootTypeWizardSystem,bCreateSystem,ptr);
	}
}

bool ElementOrder(int iElementCount,int aiOrder[2*g_iOEMPAApertureDWordCount*16])
{
	int iMin,iIndex;
	int aiOrder2[2*g_iOEMPAApertureDWordCount*16];

	for(int iElem1=0;iElem1<iElementCount;iElem1++)
		aiOrder2[iElem1] = -1;
	for(int iElem1=0;iElem1<iElementCount;iElem1++)
	{
		//find minimum
		iMin = 10000;
		iIndex = -1;
		for(int iElem2=0;iElem2<iElementCount;iElem2++)
		{
			if((aiOrder2[iElem2]==-1) && (iMin>aiOrder[iElem2]))
			{
				iMin = aiOrder[iElem2];
				iIndex = iElem2;
			}
		}
		if(iIndex>=0)
			aiOrder2[iIndex] = iElem1;
	}
	for(int iElem1=0;iElem1<iElementCount;iElem1++)
		aiOrder[iElem1] = aiOrder2[iElem1];
	return true;
}

_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizardWriteFile(CHWDeviceOEMPA *pHWDeviceOEMPA,
						CUTWizardSystem *pSystem,CUTChannels *pChannels,
						const wchar_t *pFileNameIn,const wchar_t *pFileNameOut)
{
	const wchar_t *pFileCorrected=pFileNameOut;
	bool bRet=true,bAllocParamList=false;
	structRoot root;
	structCycle *pCycle=NULL;
	CFocalLaw *pEmission=NULL;
	CFocalLaw *pReception=NULL;
	structRoot root2;
	structCycle *pCycle2=NULL;
	CFocalLaw *pEmission2=NULL;
	CFocalLaw *pReception2=NULL;
	bool bTextFile,bTextFile2;

	if(!IsValidFileExtension(pFileNameOut,bTextFile))
	{
		OempaApiMessageBox(NULL,L"Bad file extension!",L"File Error",MB_OK);
		return false;
	}
	if(!root.rParamList)
	{
		root.rParamList = new CRead_Write_ParametersList();
		bAllocParamList = true;
	}
	if(!root.rParamList)
	{
		OempaApiMessageBox(NULL,L"root.rParamList is NULL!",L"Error",MB_OK);
		return false;
	}

	root.ullSavedParameters = 0;//with the example those parameters are not significant.

	if(!OEMPA_ReadWizard(	pHWDeviceOEMPA,pSystem,pChannels,root,pCycle,pEmission,pReception))
	{
		bRet = false;goto end;
	}

	if(pFileNameIn && wcslen(pFileNameIn))
	{
		if(!IsValidFileExtension(pFileNameOut,bTextFile2))
		{
			OempaApiMessageBox(NULL,L"Bad file extension!",L"File Error",MB_OK);
			return false;
		}
		if(bTextFile2 && !OEMPA_ReadFileText(pFileNameIn,root2,pCycle2,pEmission2,pReception2))
		{bRet = false;goto end;}
		if(!bTextFile2 && !OEMPA_ReadFileBinary(pFileNameIn,root2,pCycle2,pEmission2,pReception2))
		{bRet = false;goto end;}
		if(root2.iCycleCount!=1)
		{bRet = false;goto end;}
		for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
		{
			memcpy(&pCycle[iCycle],pCycle2,sizeof(structCycle));
		}
	}

	if(bTextFile && !OEMPA_WriteFileText(pFileNameOut,&root,pCycle,pEmission,pReception))
	{
		OempaApiMessageBox(NULL,L"Write data error!",L"File Error",MB_OK);
		bRet = false;goto end;
	}
	if(!bTextFile && !OEMPA_WriteFileBinary(pFileNameOut,&root,pCycle,pEmission,pReception))
	{
		OempaApiMessageBox(NULL,L"Write data error!",L"File Error",MB_OK);
		bRet = false;goto end;
	}
	//correction
	if(bTextFile)
	{
		//FileWriteString(L"Root",L"Corrected",pFileNameOut,pFileCorrected);
		if(!WriteCorrectedFileText(pFileCorrected,&root,pCycle,pEmission,pReception))
		{
			OempaApiMessageBox(NULL,L"Write data error!",L"File Error",MB_OK);
			//bRet = false;goto end;
		}else
		if(bRet)
			FillNewLineWizard(pFileNameOut);
	}

end:
	if(pCycle)
		delete [] pCycle;
	pCycle = NULL;
	if(pEmission)
		delete [] pEmission;
	pEmission = NULL;
	if(pReception)
		delete [] pReception;
	pReception = NULL;
	if(pCycle2)
		delete [] pCycle2;
	pCycle2 = NULL;
	if(pEmission2)
		delete [] pEmission2;
	pEmission2 = NULL;
	if(pReception2)
		delete [] pReception2;
	pReception2 = NULL;

	return bRet;
}

_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizardWriteFile(CHWDeviceOEMPA *pHWDeviceOEMPA,
						CUTWizardSystem *pSystem,CUTChannels *pChannels,const wchar_t *pFileName)
{
	return OEMPA_ReadWizardWriteFile(pHWDeviceOEMPA, pSystem, pChannels, NULL, pFileName);
}

_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizardWriteFile(CHWDeviceOEMPA *pHWDeviceOEMPA,
						int iWizardSystemId,int iWizardChannelId,const wchar_t *pFileName)
{
	const wchar_t *pFileCorrected=pFileName;
	bool bRet=true,bAllocParamList=false;
	structRoot root;
	structCycle *pCycle=NULL;
	CFocalLaw *pEmission=NULL;
	CFocalLaw *pReception=NULL;
	bool bTextFile;
	bool bCreateSystem,bCreateChannel;
	CUTWizardSystem *pSystem=NULL;
	CUTChannels *pChannels=NULL;

	if(!IsValidFileExtension(pFileName,bTextFile))
	{
		OempaApiMessageBox(NULL,L"Bad file extension!",L"File Error",MB_OK);
		return false;
	}
	if(!root.rParamList)
	{
		root.rParamList = new CRead_Write_ParametersList();
		bAllocParamList = true;
	}
	if(!root.rParamList)
	{
		OempaApiMessageBox(NULL,L"root.rParamList is NULL!",L"Error",MB_OK);
		return false;
	}

	if(!GetObjectWizard(iWizardSystemId,iWizardChannelId,
						bCreateSystem,bCreateChannel,pSystem,pChannels))
	{
		OempaApiMessageBox(NULL,L"Bad object creation!",L"Kernel Error",MB_OK);
		return false;
	}
	if(!OEMPA_ReadWizard(	pHWDeviceOEMPA,pSystem,pChannels,root,pCycle,pEmission,pReception))
	{
		bRet = false;goto end;
	}

	if(bTextFile && !OEMPA_WriteFileText(pFileName,&root,pCycle,pEmission,pReception))
	{
		OempaApiMessageBox(NULL,L"Write data error!",L"File Error",MB_OK);
		bRet = false;goto end;
	}
	if(!bTextFile && !OEMPA_WriteFileBinary(pFileName,&root,pCycle,pEmission,pReception))
	{
		OempaApiMessageBox(NULL,L"Write data error!",L"File Error",MB_OK);
		bRet = false;goto end;
	}
	//correction
	if(bTextFile)
	{
		//FileWriteString(L"Root",L"Corrected",pFileName,pFileCorrected);
		if(!WriteCorrectedFileText(pFileCorrected,&root,pCycle,pEmission,pReception))
		{
			OempaApiMessageBox(NULL,L"Write data error!",L"File Error",MB_OK);
			//bRet = false;goto end;
		}else
		if(bRet)
			FillNewLineWizard(pFileName);
	}

end:
	if(pCycle)
		delete [] pCycle;
	pCycle = NULL;
	if(pEmission)
		delete [] pEmission;
	pEmission = NULL;
	if(pReception)
		delete [] pReception;
	pReception = NULL;

	ReleaseObjectWizard(bCreateSystem,bCreateChannel,pSystem,pChannels);

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void ReadWizardProbeOffset(CUTWizardSystem *pSystem,CUTChannels *pChannels,
						CUTProbe *pProbe,bool bPulse,double &dProbeOffsetX,double &dProbeOffsetY)
{
	CUTWizardWedge *pWizardWedge;
	int iWizardProbeIndex;
	CUTPoint point;

	iWizardProbeIndex = pProbe->GetWizardProbeIndex(bPulse);
	pWizardWedge = &pSystem->Wedge(iWizardProbeIndex);
	if(pWizardWedge && pWizardWedge->GetWedgeOffset(point))
	{
		dProbeOffsetX = point.GetX();
		dProbeOffsetY = point.GetY();
	}else{
		dProbeOffsetX = 0.0;
		dProbeOffsetY = 0.0;
	}
}

_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizardScan(CHWDeviceOEMPA *pHWDeviceOEMPA,CUTWizardSystem *pSystem,CUTChannels *pChannels,
				CUTProbe *pProbe,CUTScan *pScan,
				int iIndexDeviceProbe,int iIndexScan,
				structRoot &root,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception,
				bool bMonoChannel,int &iLogicalChannel,int &iCycleOffset
				)
{
	bool bRet=true;
	structCycle *pCycleAux;
	CFocalLaw *pFocalLaw;
	int iCycleCount;
	int iProbeElement;
	float fDelay,fGain,fBeamCorrection,fCenterDelayE,fCenterDelayR;
	double dTof;
	CUTWizardProbe *pWizardProbe;
	CUTPulserReceiver *pPulserReceiver;
	CUTFocalLaw *pChannelFocalLaw;
	CUTGate *pGate;
	double dTimeSlot=10.0e-3;
	float fPulseWidth=100.0e-9f;
	double dAscanStart=0.0,dAscanRange=10.0e-6,dGateStart=0.0,dGateStop=1.0e-6,dGateThreshold=0.0;
	int iDACCount=0,iGateCount=0,iConnectorProbeFirst;
	structCorrectionOEMPA *pCorrection=NULL;
	int aiOrder[2*g_iOEMPAApertureDWordCount*16];
	double dProbeOffsetPulseX,dProbeOffsetPulseY,dProbeOffsetEchoX,dProbeOffsetEchoY,dTime;
	wchar_t wcError[MAX_PATH];
	bool bFullMatrixCapture;

	if(!pScan)
		return false;
	if(!pCycle || !pEmission || !pReception)
		return false;
	iCycleCount = pScan->GetPulserReceiverCount();
	if(!iCycleCount)
		return true;
	if(iCycleCount>root.iCycleCount)
		return false;
	ReadWizardProbeOffset(pSystem,pChannels,pProbe,true,dProbeOffsetPulseX,dProbeOffsetPulseY);
	ReadWizardProbeOffset(pSystem,pChannels,pProbe,false,dProbeOffsetEchoX,dProbeOffsetEchoY);
	bFullMatrixCapture = pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture();

	pWizardProbe = &pSystem->Probe(0);
	if(!pWizardProbe)
	{
		OempaApiMessageBox(NULL,L"Bad pointer!",L"Wizard Error",MB_OK);
		bRet = false;goto end;
	}
	fPulseWidth = (float)(0.5/pWizardProbe->GetFrequency());//1.1.5.3m
	pHWDeviceOEMPA->CheckEmissionWidth(fPulseWidth);//1.1.5.3m
	fBeamCorrection = 0.0f;
	pHWDeviceOEMPA->CheckBeamCorrection(fBeamCorrection);
	for(int iCycle=0;iCycle<iCycleCount;iCycle++)
	{
		if(iCycle+iCycleOffset>=root.iCycleCount)
		{
			bRet = false;goto end;
		}

		pPulserReceiver = &(*pScan)[iCycle];
		pCycleAux = &pCycle[iCycle+iCycleOffset];

		pCycleAux->dGainDigital = pPulserReceiver->Receiver().GetGainDigital();
		pHWDeviceOEMPA->CheckGainDigital(pCycleAux->dGainDigital);
		pCycleAux->fGainAnalog = (float)pPulserReceiver->Receiver().GetGainAnalog();
		pHWDeviceOEMPA->CheckGainAnalog(pCycleAux->fGainAnalog);
		pCycleAux->iFilterIndex = 1;
 		pCycleAux->fBeamCorrection = fBeamCorrection;
		dAscanStart = pPulserReceiver->Receiver().GetStart();
		dAscanRange = pPulserReceiver->Receiver().GetStop()-dAscanStart;
		pHWDeviceOEMPA->CheckAscanStart(dAscanStart);
		pCycleAux->dStart = dAscanStart;
		pHWDeviceOEMPA->CheckAscanRange(dAscanRange);
		pCycleAux->dRange = dAscanRange;
		if(pPulserReceiver->GetTrigger()==eTimer)
			dTimeSlot = pPulserReceiver->GetTimer();//member of "CUTPulser"
		else
			dTimeSlot = 100.0e-6;
		pHWDeviceOEMPA->CheckTimeSlot(dTimeSlot);
		pCycleAux->dTimeSlot = dTimeSlot;
		pCycleAux->lPointCount = 0;//myround(dAscanRange/pHWDeviceOEMPA->dGetClockPeriod())+1;
		pCycleAux->lPointFactor = 0;
		pCycleAux->eCompressionType = eCompression;
		pHWDeviceOEMPA->CheckAscanRangeWithCount(pCycleAux->dRange,pCycleAux->eCompressionType,pCycleAux->lPointCount);
		pCycleAux->eRectification = eSigned;
		pCycleAux->bDACAutoStop = false;
		pCycleAux->bDACSlope = false;
		pCycleAux->iDACCount = 0;
		for(int iDACIndex=0;iDACIndex<iDACCount;iDACIndex++)
		{
			pCycleAux->adDACTof[iDACIndex] = pCycleAux->dStart+10.0*iDACIndex*pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod();//unit = second
			pCycleAux->afDACPrm[iDACIndex] = iDACIndex*g_fOEMPADACSlopeStep/pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod();//unit = dB/second
		}
		pCycleAux->bDACEnable = false;
		pCycleAux->bMaximum = true;
		pCycleAux->bMinimum = false;
		pCycleAux->bSaturation = false;
		if(bMonoChannel)
		{
			pCycleAux->wAcqIdChannelProbe = 0xffff;
			pCycleAux->wAcqIdChannelScan = 0xffff;
			pCycleAux->wAcqIdChannelCycle = 0xffff;
		}else{
			pCycleAux->wAcqIdChannelProbe = iIndexDeviceProbe;
			pCycleAux->wAcqIdChannelScan = iIndexScan;
			pCycleAux->wAcqIdChannelCycle = iCycle;
		}
		iGateCount = pPulserReceiver->Receiver().GetCscanCount();
		pCycleAux->iGateCount = iGateCount;
		for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
		{
			if(iGate<iGateCount)
				pCycleAux->gate[iGate].bEnable = true;
			else
				pCycleAux->gate[iGate].bEnable = false;
			pGate = &pPulserReceiver->Receiver().CscanGate(iGate);
			if(pGate)
			{
				dGateStart = pGate->GetStart();
				pCycleAux->gate[iGate].dStart = dGateStart;
				dGateStop = pGate->GetStop();
				pCycleAux->gate[iGate].dStop = dGateStop;
				dGateThreshold = pGate->GetThreshold();
				pCycleAux->gate[iGate].dThreshold = dGateThreshold;
				pCycleAux->gate[iGate].eRectification = pGate->GetRectification();
				pCycleAux->gate[iGate].eModeAmp = pGate->GetModeAmplitude();
				pCycleAux->gate[iGate].eModeTof = pGate->GetModeTimeOfFlight();
				pCycleAux->gate[iGate].wAcqIDAmp = 0xffff;
				pCycleAux->gate[iGate].wAcqIDTof = 0xffff;
			}
			pHWDeviceOEMPA->CheckGateStart(pCycleAux->gate[iGate].dStart);
			pHWDeviceOEMPA->CheckGateStop(pCycleAux->gate[iGate].dStop);
			pHWDeviceOEMPA->CheckGateModeThreshold(pCycleAux->gate[iGate].bEnable,pCycleAux->gate[iGate].eModeAmp,pCycleAux->gate[iGate].eModeTof,pCycleAux->gate[iGate].eRectification,pCycleAux->gate[iGate].dThreshold);
		}

		//those two following lines give the same result
		pChannelFocalLaw = dynamic_cast<CUTFocalLaw*>(&pPulserReceiver->Pulser());
		pChannelFocalLaw = dynamic_cast<CUTFocalLaw*>(pPulserReceiver);
		pFocalLaw = &pEmission[iCycle+iCycleOffset];
		if(!pFocalLaw->GetDWORDElementSize())
			pFocalLaw->SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
		pFocalLaw->dAngle = pChannelFocalLaw->GetAngle();
		pFocalLaw->dSkew = pChannelFocalLaw->GetSkew();
		pChannelFocalLaw->GetEmergence(pFocalLaw->dX,pFocalLaw->dY,pFocalLaw->dZ);
		if(pChannelFocalLaw->IsTRLEnabled())
			pChannelFocalLaw->GetInterfaceCenterTRL(pFocalLaw->dX,pFocalLaw->dY,pFocalLaw->dZ);
		pChannelFocalLaw->GetFocal(pFocalLaw->dFocalX,pFocalLaw->dFocalY,pFocalLaw->dFocalZ);
		pFocalLaw->dX += dProbeOffsetPulseX;
		pFocalLaw->dY += dProbeOffsetPulseY;
		pFocalLaw->dFocalX += dProbeOffsetPulseX;
		pFocalLaw->dFocalY += dProbeOffsetPulseY;
		pFocalLaw->eFocusing = eStandard;
		pFocalLaw->fCenterDelay = 0.0;
		if(bFullMatrixCapture)
			pFocalLaw->dWedgeDelay = 0.0;
		else{
			if(!pChannelFocalLaw->GetCenterDelay(fCenterDelayE))
			{
				pFocalLaw->dWedgeDelay = pChannelFocalLaw->GetWedgeDelay()+fCenterDelayE;
				pFocalLaw->fCenterDelay = fCenterDelayE;
			}else
				pFocalLaw->dWedgeDelay = pChannelFocalLaw->GetWedgeDelay();
		}
		pHWDeviceOEMPA->CheckWedgeDelay(pFocalLaw->dWedgeDelay);
		pFocalLaw->iElementCount = pChannelFocalLaw->GetCount();
		if(pFocalLaw->iElementCount>root.iOEMPAApertureCountMax)
		{
			OempaApiMessageBox(NULL,L"Too many elements in your aperture!",L"Wizard Error",MB_OK);
			bRet = false;goto end;
		}
		pFocalLaw->iPrmCount = pFocalLaw->iElementCount;
		if(pFocalLaw->iPrmCount>0)
			pFocalLaw->iFocalCount = 1;
		else
			pFocalLaw->iFocalCount = 0;
		pFocalLaw->iDelayCount = pFocalLaw->iElementCount;
		//1.1.5.3m fPulseWidth = (float)pPulserReceiver->GetWidth();//member of "CUTPulser"
		//1.1.5.3m pHWDeviceOEMPA->CheckEmissionWidth(fPulseWidth);
		iConnectorProbeFirst = pProbe->GetFirst();
		if(!pHWDeviceOEMPA->SetAllElementEnable(false,pFocalLaw->adwElement))
			{bRet = false;goto end;}
		for(int iElem=0;iElem<pFocalLaw->iElementCount;iElem++)
		{
			if(pChannelFocalLaw->GetElementIndex(iElem,iProbeElement))
				{bRet = false;goto end;}
			iProbeElement += iConnectorProbeFirst;
			if(iProbeElement>=root.iOEMPAProbeCountMax)
			{
				wsprintf(wcError,L"The probe has only %d elements, it is impossible to use the element index %d!",root.iOEMPAProbeCountMax,iProbeElement);
				OempaApiMessageBox(NULL,wcError,L"Wizard Error",MB_OK);
				bRet = false;goto end;
			}
			pHWDeviceOEMPA->SetElementEnable(iProbeElement,true,pFocalLaw->adwElement);
			/*following code is the same than to call "SetElementEnable"
			if(iElem>=32)
			{
				dwElem = 1<<(iProbeElement-32);
				pFocalLaw->adwElement[0] |= dwElem;
			}else{
				dwElem = 1<<iProbeElement;
				pFocalLaw->adwElement[1] |= dwElem;
			}*/
			aiOrder[iElem] = iProbeElement;
		}
		ElementOrder(pFocalLaw->iElementCount,aiOrder);
		for(int iElem=0;iElem<pFocalLaw->iElementCount;iElem++)
		{
			if(pChannelFocalLaw->GetDelay(iElem,fDelay))
				{bRet = false;goto end;}
			pHWDeviceOEMPA->CheckEmissionDelay(fDelay);
			pFocalLaw->afDelay[0][aiOrder[iElem]] = fDelay;
			pFocalLaw->afPrm[aiOrder[iElem]] = fPulseWidth;//width : 100 ns
		}
		if(pChannelFocalLaw->GetTRLWedgeDelaySize())
		{
			pFocalLaw->aTRLWedgeDelayRef.SetAllocatedSize(pChannelFocalLaw->GetTRLWedgeDelaySize());
			pFocalLaw->aTRLWedgeDelayReal.SetAllocatedSize(pChannelFocalLaw->GetTRLWedgeDelaySize());
			for(int iIndex=0;iIndex<pChannelFocalLaw->GetTRLWedgeDelaySize();iIndex++)
			{
				if(pChannelFocalLaw->GetTRLWedgeDelayRef(iIndex,dTime))
					pFocalLaw->aTRLWedgeDelayRef[iIndex] = (float)(dTime);
				if(pChannelFocalLaw->GetTRLWedgeDelayReal(iIndex,dTime))
					pFocalLaw->aTRLWedgeDelayReal[iIndex] = (float)(dTime);
			}
		}

		pChannelFocalLaw = &pPulserReceiver->Receiver();
		pFocalLaw = &pReception[iCycle+iCycleOffset];
		if(!pFocalLaw->GetDWORDElementSize())
			pFocalLaw->SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
		if(bFullMatrixCapture)
		{
			pFocalLaw->iFocalCount = 1;
			pFocalLaw->dAngle = 0.0;
			pFocalLaw->dSkew = 0.0;
			pFocalLaw->dX = 0.0;
			pFocalLaw->dY = 0.0;
			pFocalLaw->dZ = 0.0;
			pFocalLaw->dFocalX = 0.0;
			pFocalLaw->dFocalY = 0.0;
			pFocalLaw->dFocalZ = 0.0;
			pFocalLaw->eFocusing = eStandard;
			pFocalLaw->dWedgeDelay = 0.0;
			pFocalLaw->fCenterDelay = 0.0;
			pFocalLaw->iElementCount = root.iFMCElementStop-root.iFMCElementStart+1;
			pFocalLaw->iFocalCount = 1;
			pFocalLaw->iDelayCount = root.iFMCElementStop-root.iFMCElementStart+1;
			for(int iElem=0;iElem<pFocalLaw->iElementCount;iElem++)
			{
				pFocalLaw->afDelay[0][iElem] = 0.0f;
				pFocalLaw->adFocalTimeOfFlight[0] = 0.0;
				if(pChannelFocalLaw->GetGain(0,fGain))
					{bRet = false;goto end;}
				pHWDeviceOEMPA->CheckReceptionGain(fGain);
				pFocalLaw->afPrm[iElem] = fGain;//gain (dB)
			}
			if(!pHWDeviceOEMPA->SetAllElementEnable(false,pFocalLaw->adwElement))
				{bRet = false;goto end;}
			for(int iElem=root.iFMCElementStart;iElem<=root.iFMCElementStop;iElem++)
				pHWDeviceOEMPA->SetElementEnable(iElem,true,pFocalLaw->adwElement);
		}else{
			pFocalLaw->dAngle = pChannelFocalLaw->GetAngle();
			pFocalLaw->dSkew = pChannelFocalLaw->GetSkew();
			pChannelFocalLaw->GetEmergence(pFocalLaw->dX,pFocalLaw->dY,pFocalLaw->dZ);
			if(pChannelFocalLaw->IsTRLEnabled())
				pChannelFocalLaw->GetInterfaceCenterTRL(pFocalLaw->dX,pFocalLaw->dY,pFocalLaw->dZ);
			pChannelFocalLaw->GetFocal(pFocalLaw->dFocalX,pFocalLaw->dFocalY,pFocalLaw->dFocalZ);
			pFocalLaw->dX += dProbeOffsetEchoX;
			pFocalLaw->dY += dProbeOffsetEchoY;
			pFocalLaw->dFocalX += dProbeOffsetEchoX;
			pFocalLaw->dFocalY += dProbeOffsetEchoY;
			pFocalLaw->eFocusing = pChannelFocalLaw->GetFocusing();
			if(!pChannelFocalLaw->GetCenterDelay(fCenterDelayR))
			{
				pFocalLaw->dWedgeDelay = pChannelFocalLaw->GetWedgeDelay()+fCenterDelayR;
				pFocalLaw->fCenterDelay = fCenterDelayR;
			}else
				pFocalLaw->dWedgeDelay = pChannelFocalLaw->GetWedgeDelay();
			pHWDeviceOEMPA->CheckWedgeDelay(pFocalLaw->dWedgeDelay);
			pFocalLaw->iElementCount = pChannelFocalLaw->GetCount();
			if(pFocalLaw->iElementCount>root.iOEMPAApertureCountMax)
			{
				OempaApiMessageBox(NULL,L"Too many elements in your aperture!",L"Wizard Error",MB_OK);
				bRet = false;goto end;
			}
			pFocalLaw->iPrmCount = pFocalLaw->iElementCount;//gain
			pFocalLaw->iFocalCount = pChannelFocalLaw->GetFocalLawCount();
			pFocalLaw->iDelayCount = pFocalLaw->iElementCount*pFocalLaw->iFocalCount;
			iConnectorProbeFirst = pProbe->GetFirst();
			if(!pHWDeviceOEMPA->SetAllElementEnable(false,pFocalLaw->adwElement))
				{bRet = false;goto end;}
			if(!pFocalLaw->iElementCount)
			{
				NotifyErrorFocalLaw(iCycle);
				bRet = false;goto end;
			}
			for(int iElem=0;iElem<pFocalLaw->iElementCount;iElem++)
			{
				if(pChannelFocalLaw->GetElementIndex(iElem,iProbeElement))
					{bRet = false;goto end;}
				iProbeElement += iConnectorProbeFirst;
				if(iProbeElement>=root.iOEMPAProbeCountMax)
				{
					wsprintf(wcError,L"The probe has only %d elements, it is impossible to use the element index %d!",root.iOEMPAProbeCountMax,iProbeElement);
					OempaApiMessageBox(NULL,L"Aperture bad index element!",L"Wizard Error",MB_OK);
					bRet = false;goto end;
				}
				pHWDeviceOEMPA->SetElementEnable(iProbeElement,true,pFocalLaw->adwElement);
				/*following code is the same than to call "SetElementEnable"
				if(iElem>=32)
				{
					dwElem = 1<<(iProbeElement-32);
					pFocalLaw->adwElement[0] |= dwElem;
				}else{
					dwElem = 1<<iProbeElement;
					pFocalLaw->adwElement[1] |= dwElem;
				}*/
				aiOrder[iElem] = iProbeElement;
			}
			ElementOrder(pFocalLaw->iElementCount,aiOrder);
			for(int iElem=0;iElem<pFocalLaw->iElementCount;iElem++)
			{
				for(int iFocalIndex=0;iFocalIndex<pFocalLaw->iFocalCount;iFocalIndex++)
				{
					if(pChannelFocalLaw->GetDelay(iElem,iFocalIndex,fDelay))
						{bRet = false;goto end;}
					pHWDeviceOEMPA->CheckReceptionDelay(fDelay);
					pFocalLaw->afDelay[iFocalIndex][aiOrder[iElem]] = fDelay;
					if(!iElem)
					{
						dTof = 0.0;
						if((pFocalLaw->eFocusing==eDDF) && pChannelFocalLaw->GetFocalLawTof(iFocalIndex,dTof))
							{bRet = false;goto end;}
						dTof *= 2;
						//dTof += fCenterDelayE+fCenterDelayR;//1.1.5.1f
						pHWDeviceOEMPA->CheckFocalTimeOfFlight(dTof);
						pFocalLaw->adFocalTimeOfFlight[iFocalIndex] = dTof;
					}
				}
				if(pChannelFocalLaw->GetGain(aiOrder[iElem],fGain))
					{bRet = false;goto end;}
				pHWDeviceOEMPA->CheckReceptionGain(fGain);
				pFocalLaw->afPrm[iElem] = fGain;//gain (dB)
			}
			if(root.eReferenceTimeOfFlight==eInterface)
			{
				for(int iFocalIndex=0;iFocalIndex<pFocalLaw->iFocalCount;iFocalIndex++)
				{
					pFocalLaw->adFocalTimeOfFlight[iFocalIndex] -= pFocalLaw->dWedgeDelay+pEmission[iCycle+iCycleOffset].dWedgeDelay;
				}
			}
		}
		if(pChannelFocalLaw->GetTRLWedgeDelaySize())
		{
			pFocalLaw->aTRLWedgeDelayRef.SetAllocatedSize(pChannelFocalLaw->GetTRLWedgeDelaySize());
			pFocalLaw->aTRLWedgeDelayReal.SetAllocatedSize(pChannelFocalLaw->GetTRLWedgeDelaySize());
			for(int iIndex=0;iIndex<pChannelFocalLaw->GetTRLWedgeDelaySize();iIndex++)
			{
				if(pChannelFocalLaw->GetTRLWedgeDelayRef(iIndex,dTime))
					pFocalLaw->aTRLWedgeDelayRef[iIndex] = (float)(dTime);
				if(pChannelFocalLaw->GetTRLWedgeDelayReal(iIndex,dTime))
					pFocalLaw->aTRLWedgeDelayReal[iIndex] = (float)(dTime);
			}
		}
	}
	iCycleOffset += iCycleCount;
	if(!bMonoChannel)
		iLogicalChannel++;
end:
	return bRet;
}

_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizardCorrection(
				structRoot &root,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception,
				structCorrectionOEMPA *pCorrection,bool &bRet)
{
	structCycle *pCycleAux;
	CFocalLaw *pFocalLaw;

	if(!pCorrection)
		return true;
	if(pCorrection && (!pCycle || !pEmission || !pReception))
		return false;

	if(pCorrection)
	{
		//additional correction information placed in text file only.
		pCorrection->iCount = root.iCycleCount;
		pCorrection->dSpecimenVelocity = root.dSpecimenVelocity;
		pCorrection->dDigitizingPeriod = root.dDigitizingPeriod;
		for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
		{
			pFocalLaw = &pEmission[iCycle];
			if(pCorrection->bReadOEMPAFileNewWay)
			{
				pCorrection->pdAnglePulse[iCycle] = pFocalLaw->dAngle;
				pCorrection->pdSkewPulse[iCycle] = pFocalLaw->dSkew;
			}else{
				if(fabs(pFocalLaw->dSkew)>M_PI*0.9)
					pCorrection->pdAnglePulse[iCycle] = -pFocalLaw->dAngle;
				else
					pCorrection->pdAnglePulse[iCycle] = pFocalLaw->dAngle;
				pCorrection->pdSkewPulse[iCycle] = 0.0;
			}
			pCorrection->pdXPulse[iCycle] = pFocalLaw->dX;
			pCorrection->pdYPulse[iCycle] = pFocalLaw->dY;
			pCorrection->pdZPulse[iCycle] = pFocalLaw->dZ;
			pCorrection->pdXFocalPulse[iCycle] = pFocalLaw->dFocalX;
			pCorrection->pdYFocalPulse[iCycle] = pFocalLaw->dFocalY;
			pCorrection->pdZFocalPulse[iCycle] = pFocalLaw->dFocalZ;
			pFocalLaw = &pReception[iCycle];
			if(pCorrection->bReadOEMPAFileNewWay)
			{
				pCorrection->pdAngleRecv[iCycle] = pFocalLaw->dAngle;
				pCorrection->pdSkewRecv[iCycle] = pFocalLaw->dSkew;
			}else{
				if(fabs(pFocalLaw->dSkew)>M_PI*0.9)
					pCorrection->pdAngleRecv[iCycle] = -pFocalLaw->dAngle;
				else
					pCorrection->pdAngleRecv[iCycle] = pFocalLaw->dAngle;
				pCorrection->pdSkewRecv[iCycle] = 0.0;
			}
			pCorrection->pdXRecv[iCycle] = pFocalLaw->dX;
			pCorrection->pdYRecv[iCycle] = pFocalLaw->dY;
			pCorrection->pdZRecv[iCycle] = pFocalLaw->dZ;
			pCorrection->pdXFocalRecv[iCycle] = pFocalLaw->dFocalX;
			pCorrection->pdYFocalRecv[iCycle] = pFocalLaw->dFocalY;
			pCorrection->pdZFocalRecv[iCycle] = pFocalLaw->dFocalZ;
			pCorrection->piCountRecv[iCycle] = (int)pCycle[iCycle].lPointCount;
		}
		pCorrection->dSpecimenRadius = root.dSpecimenRadius;
		pCorrection->dSpecimenThickness = root.dSpecimenThickness;
	}

	if(bRet)
	{
		for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
		{
			pCycleAux = &pCycle[iCycle];
			pCycleAux->iGateCount = 0;
			for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
			{
				if(pCycleAux->gate[iGate].bEnable)
					pCycleAux->iGateCount++;
			}
		}
	}

	return bRet;
}

//#include "hwbrk.h"
//http://www.codeproject.com/Articles/28071/Toggle-hardware-data-read-execute-breakpoints-prog
_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizardSingleChannel(CHWDeviceOEMPA *pHWDeviceOEMPA,CUTWizardSystem *pSystem,CUTChannels *pChannels,
				structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception)
{
	bool bRet=true;
	CUTProbe *pProbe;
	CUTScan *pScan;
	enumWave eWave;
	int iCycleCount;
	int iLogicalChannel,iCycleOffset;
	structCorrectionOEMPA *pCorrection=NULL;
	pCallbackCustomizedWizardAPI pCallbackCustomizedAPI=OEMPA_GetCallbackCustomizedWizardAPI();

	if(pCycle || pEmission || pReception)
		return false;

	if(pChannels->IsMultiChannels())
		return false;

	OEMPA_InitRoot(root,pHWDeviceOEMPA);
	pProbe = &pChannels->Probe();//single channel
	pScan = &pChannels->Probe().Scan();//single channel
	iCycleCount = pScan->GetPulserReceiverCount();
	if(!iCycleCount)
	{
		OempaApiMessageBox(NULL,L"Dummy scan!",L"Kernel Error",MB_OK);
		return false;
	}

	if(pCallbackCustomizedAPI)
		pCorrection = (*pCallbackCustomizedAPI)(pHWDeviceOEMPA,NULL,eLoadWizard_Enter,&root,pCycle,pEmission,pReception);

	root.iCycleCount = iCycleCount;
	root.dDigitizingPeriod = pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod();
	eWave = pScan->GetWave();
	root.dSpecimenVelocity = pSystem->Specimen().GetVelocity(eWave);
	switch(pSystem->Specimen().GetSpecimen())
	{
	default:
	case ePlane:
		root.dSpecimenRadius = 0.0;
		root.dSpecimenThickness = 0.0;
		break;
	case eDisk:
	case eBar:
		if(!pSystem->Specimen().GetRadius(root.dSpecimenRadius))
			root.dSpecimenRadius = 0.0;
		root.dSpecimenThickness = 0.0;
		break;
	case eTube:
		if(!pSystem->Specimen().GetRadius(root.dSpecimenRadius))
			root.dSpecimenRadius = 0.0;
		if(!pSystem->Specimen().GetThickness(root.dSpecimenThickness))
			root.dSpecimenThickness = 0.0;
		break;
	}

	pCycle = OEMPA_AllocCycle(root.iCycleCount);
	pEmission = new CFocalLaw[root.iCycleCount];
	pReception = new CFocalLaw[root.iCycleCount];
	if(!pCycle || !pEmission || !pReception)
	{
		bRet = false;goto end;
	}
	//int *pData=&pCycle[121].iGateCount;
	OEMPA_ResetArrayFocalLaw(root.iCycleCount,pEmission);
	OEMPA_ResetArrayFocalLaw(root.iCycleCount,pReception);

	iLogicalChannel = 0;
	iCycleOffset = 0;
	bRet = OEMPA_ReadWizardScan(pHWDeviceOEMPA,pSystem,pChannels,pProbe,pScan,0,0,root,pCycle,pEmission,pReception,true,iLogicalChannel,iCycleOffset);

end:
	if(!OEMPA_ReadWizardCorrection(root,pCycle,pEmission,pReception,pCorrection,bRet))
		bRet = false;

	if(!bRet)
	{
		if(pCycle)
			delete [] pCycle;
		pCycle = NULL;
		if(pEmission)
			delete [] pEmission;
		pEmission = NULL;
		if(pReception)
			delete [] pReception;
		pReception = NULL;
	}

	if(bRet && pCallbackCustomizedAPI)
		(*pCallbackCustomizedAPI)(pHWDeviceOEMPA,NULL,eLoadWizard_Leave,&root,pCycle,pEmission,pReception);

	return bRet;
}

_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizardMultiChannel(CHWDeviceOEMPA *pHWDeviceOEMPA,CUTWizardSystem *pSystem,CUTChannels *pChannels,
				structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception)
{
	bool bRet=true;
	int iCycleCount;
	CUTProbe *pProbe;
	CUTScan *pScan;
	enumWave eWave;
	structCorrectionOEMPA *pCorrection=NULL;
	int iProbeCount,iScanCount;
	int iLogicalChannel,iCycleOffset;
	pCallbackCustomizedWizardAPI pCallbackCustomizedAPI=OEMPA_GetCallbackCustomizedWizardAPI();

	if(pCycle || pEmission || pReception)
		return false;

	if(!pChannels->IsMultiChannels())
		return false;

	iCycleCount = 0;
	iProbeCount = pChannels->GetProbeCount();
	for(int iIndexProbe=0;iIndexProbe<iProbeCount;iIndexProbe++)
	{
		pProbe = &pChannels->Probe(iIndexProbe);
		iScanCount = pProbe->GetScanCount();
		for(int iIndexScan=0;iIndexScan<iScanCount;iIndexScan++)
		{
			pScan = &pProbe->Scan(iIndexScan);
			iCycleCount += pScan->GetPulserReceiverCount();
		}
	}
	if(!iCycleCount)
	{
		OempaApiMessageBox(NULL,L"Dummy scan!",L"Kernel Error",MB_OK);
		return false;
	}

	if(pCallbackCustomizedAPI)
		pCorrection = (*pCallbackCustomizedAPI)(pHWDeviceOEMPA,NULL,eLoadWizard_Enter,&root,pCycle,pEmission,pReception);

	OEMPA_InitRoot(root,pHWDeviceOEMPA);
	root.iCycleCount = iCycleCount;
	root.dDigitizingPeriod = pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod();
	eWave = pScan->GetWave();
	root.dSpecimenVelocity = pSystem->Specimen().GetVelocity(eWave);
	switch(pSystem->Specimen().GetSpecimen())
	{
	default:
	case ePlane:
		root.dSpecimenRadius = 0.0;
		root.dSpecimenThickness = 0.0;
		break;
	case eDisk:
	case eBar:
		if(!pSystem->Specimen().GetRadius(root.dSpecimenRadius))
			root.dSpecimenRadius = 0.0;
		root.dSpecimenThickness = 0.0;
		break;
	case eTube:
		if(!pSystem->Specimen().GetRadius(root.dSpecimenRadius))
			root.dSpecimenRadius = 0.0;
		if(!pSystem->Specimen().GetThickness(root.dSpecimenThickness))
			root.dSpecimenThickness = 0.0;
		break;
	}

	pCycle = OEMPA_AllocCycle(root.iCycleCount);
	pEmission = new CFocalLaw[root.iCycleCount];
	pReception = new CFocalLaw[root.iCycleCount];
	if(!pCycle || !pEmission || !pReception)
	{
		bRet = false;goto end;
	}
	OEMPA_ResetArrayFocalLaw(root.iCycleCount,pEmission);
	OEMPA_ResetArrayFocalLaw(root.iCycleCount,pReception);

	iLogicalChannel = 0;
	iCycleOffset = 0;
	iProbeCount = pChannels->GetProbeCount();
	for(int iIndexProbe=0;iIndexProbe<iProbeCount;iIndexProbe++)
	{
		pProbe = &pChannels->Probe(iIndexProbe);
		iScanCount = pProbe->GetScanCount();
		for(int iIndexScan=0;iIndexScan<iScanCount;iIndexScan++)
		{
			pScan = &pProbe->Scan(iIndexScan);
			if(!OEMPA_ReadWizardScan(pHWDeviceOEMPA,pSystem,pChannels,pProbe,pScan,
						iIndexProbe,iIndexScan,
						root,pCycle,pEmission,pReception,false,iLogicalChannel,iCycleOffset))
				{bRet = false;goto end;}
		}
	}

end:
	if(!OEMPA_ReadWizardCorrection(root,pCycle,pEmission,pReception,pCorrection,bRet))
		bRet = false;

	if(!bRet)
	{
		if(pCycle)
			delete [] pCycle;
		pCycle = NULL;
		if(pEmission)
			delete [] pEmission;
		pEmission = NULL;
		if(pReception)
			delete [] pReception;
		pReception = NULL;
	}

	if(bRet && pCallbackCustomizedAPI)
		(*pCallbackCustomizedAPI)(pHWDeviceOEMPA,NULL,eLoadWizard_Leave,&root,pCycle,pEmission,pReception);

	return bRet;
}

_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizard(CHWDeviceOEMPA *pHWDeviceOEMPA,CUTWizardSystem *pSystem,CUTChannels *pChannels,
				structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception)
{
	if(!pChannels->IsMultiChannels())
		return OEMPA_ReadWizardSingleChannel(pHWDeviceOEMPA,pSystem,pChannels,root,pCycle,pEmission,pReception);
	else
		return OEMPA_ReadWizardMultiChannel(pHWDeviceOEMPA,pSystem,pChannels,root,pCycle,pEmission,pReception);
}

_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizard(CHWDeviceOEMPA *pHWDeviceOEMPA,int iWizardSystemId,int iWizardChannelId,
						structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception)
{
	CUTChannels *pChannels=NULL;
	CUTWizardSystem *pSystem=NULL;
	bool bRet=true;
	bool bCreateSystem,bCreateChannel;

	if(pCycle || pEmission || pReception)
		return false;

	if(!GetObjectWizard(iWizardSystemId,iWizardChannelId,
						bCreateSystem,bCreateChannel,pSystem,pChannels))
	{
		OempaApiMessageBox(NULL,L"Bad object creation!",L"Kernel Error",MB_OK);
		return false;
	}

	bRet = OEMPA_ReadWizard(pHWDeviceOEMPA,pSystem,pChannels,root,pCycle,pEmission,pReception);

	ReleaseObjectWizard(bCreateSystem,bCreateChannel,pSystem,pChannels);

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizard(	CUTWizardSystem *pSystem,CUTChannels *pChannels,
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
					)
{
	return OEMPA_ReadWizardMultipleChannel(0,0,
					pSystem,pChannels,
					bLongitudinalWave,dSpecimenVelocity,
					iProbeElementCount,dProbePitch,dProbeFrequency,
					bWedgeEnable,
					dWedgeHeight,dWedgeAngle,dWedgeVelocity,
					dScanDepthEmission,iScanDepthReceptionMax,pScanDepthReception,iScanDepthReceptionCount,
					iScanElementCount,
					iScanElementStart,iScanElementStop,iScanElementStep,
					dScanAngleStart,dScanAngleStop,dScanAngleStep,
					dAscanStart,dAscanRange,dTimeSlot,
					iCscanMax,pdCscanStart,pdCscanRange,pdCscanThreshold,
					peRectification,peModeAmp,peModeTof,iCscanCount,
					eDepthMode
					);
}
_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizardMultipleChannel(	int iProbeIndex,int iScanIndex,
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
					)
{
	bool bRet=true;
	CUTWizardProbe *pWizardProbe;
	CUTWizardWedge *pWizardWedge;
	CUTScan *pScan;
	double dWedgeSkew,dWedgeTilt;
	CUTGate *pGate;
	int iWizardProbeIndex;

	if(!pSystem || !pChannels)
		return false;

	if(!pChannels->IsMultiChannels())
	{
		pScan = &pChannels->Probe().Scan();//single channel
		pWizardProbe = &pSystem->Probe(0);
	}else{
		if(iProbeIndex<0)
			return false;
		if(iProbeIndex>=pChannels->GetProbeCount())
			return false;
		if(iScanIndex<0)
			return false;
		if(iScanIndex>=pChannels->Probe(iProbeIndex).GetScanCount())
			return false;
		pScan = &pChannels->Probe(iProbeIndex).Scan(iScanIndex);//multi channel
		iWizardProbeIndex = pChannels->Probe(iProbeIndex).GetWizardProbeIndex(false);
		pWizardProbe = &pSystem->Probe(iWizardProbeIndex);
	}
	if(!pScan)
		return false;
	if(!pWizardProbe)
		return false;
	pWizardWedge = dynamic_cast<CUTWizardWedge*>(pWizardProbe);
	if(!pWizardWedge)
		return false;

	if(pWizardProbe->GetWizardProbe()!=eLinear1D)
		return false;//1.1.5.2i
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

	eDepthMode = pScan->GetDepthMode();
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

_CUST_WIZ_DEF_ bool WINAPI OEMPA_WriteWizard(	CUTWizardSystem *pSystem,CUTChannels *pChannels,
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
					enumRectification *peRectification,enumGateModeAmp *peModeAmp,enumGateModeTof *peModeTof,
					enumDepthMode eDepthMode
					)
{
	return OEMPA_WriteWizardMultipleChannel(0,0,
					pSystem,pChannels,
					bLongitudinalWave,dSpecimenVelocity,
					iProbeElementCount,dProbePitch,dProbeFrequency,
					bWedgeEnable,
					dWedgeHeight,dWedgeAngle,dWedgeVelocity,
					dScanDepthEmission,pScanDepthReception,iScanDepthReceptionCount,
					iScanElementCount,
					bScanLinear,
					iScanElementStart,iScanElementStop,iScanElementStep,
					bScanAngle,
					dScanAngleStart,dScanAngleStop,dScanAngleStep,
					dAscanStart,dAscanRange,dTimeSlot,
					iCscanCount,pdCscanStart,pdCscanRange,pdCscanThreshold,
					peRectification,peModeAmp,peModeTof,
					eDepthMode
					);
}
_CUST_WIZ_DEF_ bool WINAPI OEMPA_WriteWizardMultipleChannel(	int iProbeIndex,int iScanIndex,
					CUTWizardSystem *pSystem,CUTChannels *pChannels,
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
					enumRectification *peRectification,enumGateModeAmp *peModeAmp,enumGateModeTof *peModeTof,
					enumDepthMode eDepthMode
					)
{
	bool bRet=true;
	CUTWizardProbe *pWizardProbe;
	CUTWizardWedge *pWizardWedge;
	CUTScan *pScan;
	CUTGate *pGate;
	double dAscanStop;
	int iCount;
	int iWizardProbeIndex;

	if(!pSystem || !pChannels)
		return false;

	if(!pChannels->IsMultiChannels())
	{
		pScan = &pChannels->Probe().Scan();//single channel
		pWizardProbe = &pSystem->Probe(0);
	}else{
		if(iProbeIndex<0)
			return false;
		if(iProbeIndex>=pChannels->GetProbeCount())
			return false;
		if(iScanIndex<0)
			return false;
		if(iScanIndex>=pChannels->Probe(iProbeIndex).GetScanCount())
			return false;
		pScan = &pChannels->Probe(iProbeIndex).Scan(iScanIndex);//multi channel
		iWizardProbeIndex = pChannels->Probe(iProbeIndex).GetWizardProbeIndex(false);
		pWizardProbe = &pSystem->Probe(iWizardProbeIndex);
	}
	if(!pScan)
		return false;
	if(!pWizardProbe)
		return false;
	pWizardWedge = dynamic_cast<CUTWizardWedge*>(pWizardProbe);
	if(!pWizardWedge)
		return false;

	if(pWizardProbe->SetWizardProbe(eLinear1D))
		return false;//1.1.5.2i
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

	if(pScan->SetDepthMode(eDepthMode))
		bRet = false;
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
		{
			pScan->VectorStop().GetFirst(iScanElementStop);
			//bRet = false;
		}
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

	if(!pScan->GetReceiverCycleCount())
	{
		//1.1.3.2c Set at least one focal law otherwise it is not possible to save Ascan parameters.
		pScan->SetPulserReceiverCount(1);
	}

	if(pScan->GetReceiverCycleCount())
	{
		//1:1:4:1 can fail with big values
		//if((*pScan)[0].Receiver().SetStartStop(dAscanStart,dAscanStop))
		//	bRet = false;
		//dAscanStop = dAscanRange+dAscanStart;
		//if((*pScan)[0].Receiver().SetStop(dAscanStop))
		//	bRet = false;
		dAscanStop = dAscanRange+dAscanStart;
		if((*pScan)[0].Receiver().SetStartStop(dAscanStart,dAscanStop))
			bRet = false;
		if((*pScan)[0].Pulser().SetTimer(dTimeSlot))
			bRet = false;
		iCount = pScan->GetReceiverCycleCount();
		for(int iIndex=0;iIndex<iCount;iIndex++)
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

_CUST_WIZ_DEF_ bool WINAPI OEMPA_ReadWizard(	int iWizardSystemId,int iWizardChannelId,
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
					)
{
	bool bRet=true;
	CUTChannels *pChannels=NULL;
	CUTWizardSystem *pSystem=NULL;
	bool bCreateSystem,bCreateChannel;

	if(!GetObjectWizard(iWizardSystemId,iWizardChannelId,
						bCreateSystem,bCreateChannel,pSystem,pChannels))
	{
		OempaApiMessageBox(NULL,L"Bad object creation!",L"Kernel Error",MB_OK);
		return false;
	}

	bRet = OEMPA_ReadWizard(	pSystem,pChannels,
						bLongitudinalWave,dSpecimenVelocity,
						iProbeElementCount,dProbePitch,dProbeFrequency,
						bWedgeEnable,
						dWedgeHeight,dWedgeAngle,dWedgeVelocity,
						dScanDepthEmission,iScanDepthReceptionMax,pScanDepthReception,iScanDepthReceptionCount,
						iScanElementCount,
						iScanElementStart,iScanElementStop,iScanElementStep,
						dScanAngleStart,dScanAngleStop,dScanAngleStep,
						dAscanStart,dAscanRange,dTimeSlot,
						iCscanMax,pdCscanStart,pdCscanRange,pdCscanThreshold,
						peRectification,peModeAmp,peModeTof,iCscanCount,
						eDepthMode
						);
	ReleaseObjectWizard(bCreateSystem,bCreateChannel,pSystem,pChannels);
	return bRet;
}

_CUST_WIZ_DEF_ bool WINAPI OEMPA_WriteWizard(	int iWizardSystemId,int iWizardChannelId,
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
					)
{
	bool bRet=true;
	CUTChannels *pChannels=NULL;
	CUTWizardSystem *pSystem=NULL;
	bool bCreateSystem,bCreateChannel;

	if(!bLinear && !bSectorial)
		return false;
	if(bLinear && bSectorial)
		return false;
	if(!GetObjectWizard(iWizardSystemId,iWizardChannelId,
						bCreateSystem,bCreateChannel,pSystem,pChannels))
	{
		OempaApiMessageBox(NULL,L"Bad object creation!",L"Kernel Error",MB_OK);
		return false;
	}

	bRet = OEMPA_WriteWizard(	pSystem,pChannels,
						bLongitudinalWave,dSpecimenVelocity,
						iProbeElementCount,dProbePitch,dProbeFrequency,
						bWedgeEnable,
						dWedgeHeight,dWedgeAngle,dWedgeVelocity,
						dScanDepthEmission,pScanDepthReception,iScanDepthReceptionCount,
						iScanElementCount,
						bLinear,iScanElementStart,iScanElementStop,iScanElementStep,
						bSectorial,dScanAngleStart,dScanAngleStart,1.0,
						dAscanStart,dAscanRange,dTimeSlot,
						iCscanCount,pdCscanStart,pdCscanRange,pdCscanThreshold,
						peRectification,peModeAmp,peModeTof,
						eDepthMode
						);
	ReleaseObjectWizard(bCreateSystem,bCreateChannel,pSystem,pChannels);
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

_CUST_WIZ_DEF_ bool WINAPI OEMPA_WriteHWWizard(	CHWDeviceOEMPA *pHWDeviceOEMPA,
									int iWizardSystemId,int iWizardChannelId)
{
	bool bRet=true;
	structRoot root;
	structCycle *pCycle=NULL;
	CFocalLaw *pEmission=NULL;
	CFocalLaw *pReception=NULL;

	if(!OEMPA_ReadWizard(	pHWDeviceOEMPA,iWizardSystemId,iWizardChannelId,root,pCycle,pEmission,pReception))
	{
		bRet = false;goto end;
	}
	if(!OEMPA_WriteHW(pHWDeviceOEMPA,root,pCycle,pEmission,pReception))
	{
		OempaApiMessageBox(NULL,L"Write data error!",L"File Error",MB_OK);
		bRet = false;goto end;
	}

end:
	if(pCycle)
		delete [] pCycle;
	pCycle = NULL;
	if(pEmission)
		delete [] pEmission;
	pEmission = NULL;
	if(pReception)
		delete [] pReception;
	pReception = NULL;

	return bRet;
}
