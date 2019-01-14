
#include "stdafx.h"
#define _USE_MATH_DEFINES 
#include <tlhelp32.h> 
#include <stdio.h> 
#include <math.h> 

#define _USE_CALLBACK_ARRAY_

//#define MICRO_SECOND			L"µs"
#define MICRO_SECOND			L"us"

extern bool g_bReferenceTimeOfFlightCorrection;
void GetCfgKernelReferenceTimeOfFlight(enumReferenceTimeOfFlight *pRef);
bool ReadHWMultiHWChannelUpdate(int iCycle,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pFocalLaw);
bool ReadFileStandardCheck(int iCycle,const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pFocalLaw);
bool ReadFileMultiHWChannelUpdate(int iCycle,const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pReception);

/////////////////////////////////////////////////////////////////////////////

_CUST_DRV_DEF_ bool WINAPI OEMPA_DeviceMerge(	CFocalLaw *pEmissionLsb,CFocalLaw *pReceptionLsb,
												CFocalLaw *pEmissionMsb,CFocalLaw *pReceptionMsb,
												structRoot &root,CFocalLaw* &pEmission,CFocalLaw* &pReception)
{
	bool bRet=true;

	if(!pEmissionLsb || !pReceptionLsb)
		return false;
	if(!pEmissionMsb || !pReceptionMsb)
		return false;
	if(pEmission || pReception)
		return false;
	pEmission = NULL;
	pReception = NULL;
	if(root.iCycleCount<=0)
		return false;

	pEmission = new CFocalLaw[root.iCycleCount];
	pReception = new CFocalLaw[root.iCycleCount];
	if(!pEmission || !pReception)
		{bRet = false;goto end;}

	for(int iCycleIndex=0;iCycleIndex<root.iCycleCount;iCycleIndex++)
	{
		pEmission[iCycleIndex].SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
		if(!pEmission[iCycleIndex].Merge(&pEmissionLsb[iCycleIndex],&pEmissionMsb[iCycleIndex]))
			{bRet = false;goto end;}
		pReception[iCycleIndex].SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
		if(!pReception[iCycleIndex].Merge(&pReceptionLsb[iCycleIndex],&pReceptionMsb[iCycleIndex]))
			{bRet = false;goto end;}
	}

end:

	if(!bRet)
	{
		if(pEmission)
			delete [] pEmission;
		pEmission = NULL;
		if(pReception)
			delete [] pReception;
		pReception = NULL;
	}

	return bRet;
}

_CUST_DRV_DEF_ bool WINAPI OEMPA_DeviceSplit(	structRoot &root,CFocalLaw* &pEmission,CFocalLaw* &pReception,
												structRoot &rootLsb,CFocalLaw* &pEmissionLsb,CFocalLaw* &pReceptionLsb,
												structRoot &rootMsb,CFocalLaw* &pEmissionMsb,CFocalLaw* &pReceptionMsb
												)
{
	bool bRet=true;
	int iOEMPAProbeCountMax,iOEMPAApertureCountMax;
	
	if(!pEmission || !pReception)
		return false;
	if(root.iCycleCount<=0)
		return false;
	if(pEmissionLsb || pReceptionLsb)
		return false;
	if(pEmissionMsb || pReceptionMsb)
		return false;

	iOEMPAProbeCountMax = rootLsb.iOEMPAProbeCountMax;
	iOEMPAApertureCountMax = rootLsb.iOEMPAApertureCountMax;
	memcpy(&rootLsb,&root,sizeof(root));
	rootLsb.iOEMPAProbeCountMax = iOEMPAProbeCountMax;
	rootLsb.iOEMPAApertureCountMax = iOEMPAApertureCountMax;

	iOEMPAProbeCountMax = rootMsb.iOEMPAProbeCountMax;
	iOEMPAApertureCountMax = rootMsb.iOEMPAApertureCountMax;
	memcpy(&rootMsb,&root,sizeof(root));
	rootMsb.iOEMPAProbeCountMax = iOEMPAProbeCountMax;
	rootMsb.iOEMPAApertureCountMax = iOEMPAApertureCountMax;

	pEmissionLsb = new CFocalLaw[root.iCycleCount];
	pReceptionLsb = new CFocalLaw[root.iCycleCount];
	pEmissionMsb = new CFocalLaw[root.iCycleCount];
	pReceptionMsb = new CFocalLaw[root.iCycleCount];
	if(!pEmissionLsb || !pReceptionLsb || !pEmissionMsb || !pReceptionMsb)
		{bRet = false;goto end;}

	for(int iCycleIndex=0;iCycleIndex<root.iCycleCount;iCycleIndex++)
	{
		pEmissionLsb[iCycleIndex].SetAllocatedSize(rootLsb.iOEMPADDFCountMax,rootLsb.iOEMPAProbeCountMax);
		pEmissionMsb[iCycleIndex].SetAllocatedSize(rootMsb.iOEMPADDFCountMax,rootMsb.iOEMPAProbeCountMax);
		if(!pEmission[iCycleIndex].Split(&pEmissionLsb[iCycleIndex],&pEmissionMsb[iCycleIndex]))
			{bRet = false;goto end;}
		pReceptionLsb[iCycleIndex].SetAllocatedSize(rootLsb.iOEMPADDFCountMax,rootLsb.iOEMPAProbeCountMax);
		pReceptionMsb[iCycleIndex].SetAllocatedSize(rootMsb.iOEMPADDFCountMax,rootMsb.iOEMPAProbeCountMax);
		if(!pReception[iCycleIndex].Split(&pReceptionLsb[iCycleIndex],&pReceptionMsb[iCycleIndex]))
			{bRet = false;goto end;}
	}

end:

	if(!bRet)
	{
		if(pEmissionLsb)
			delete [] pEmissionLsb;
		pEmissionLsb = NULL;
		if(pReceptionLsb)
			delete [] pReceptionLsb;
		pReceptionLsb = NULL;
		if(pEmissionMsb)
			delete [] pEmissionMsb;
		pEmissionMsb = NULL;
		if(pReceptionMsb)
			delete [] pReceptionMsb;
		pReceptionMsb = NULL;
	}

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////
void NewLine(const wchar_t *pFileName)
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

void FillNewLine(const wchar_t *pFileName)
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

_CUST_DRV_DEF_ bool IsValidFileAnsi(const wchar_t *pFileName)
{
	unsigned char start[3]={0,0,0};
	bool bRet=true;

#ifdef _USE_MFC_
	try{
		CFile file(pFileName,CFile::typeBinary | CFile::modeRead);
#else _USE_MFC_
		myCFile file(pFileName,myCFile::typeBinary | myCFile::modeRead);
#endif _USE_MFC_
		if(!file.Read(start,3))
			bRet = false;
		file.Close();
#ifdef _USE_MFC_
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
#endif _USE_MFC_
	if(bRet)
	{
		if((start[0]==0xEF) && (start[1]==0xBB) && (start[2]==0xBF))
			return false;//UTF-8
		if((start[0]==0xFE) && (start[1]==0xFF))
			return false;//UTF-16 big endian
		if((start[0]==0xFF) && (start[1]==0xFE))
			return false;//UTF-16 little endian
	}
	return true;
}

_CUST_DRV_DEF_ bool IsValidFileExtension(const wchar_t *pFileName,bool &bTextFile)
{
	wchar_t drive[_MAX_DRIVE],dir[_MAX_DIR],fname[MAX_PATH],ext[_MAX_EXT];

	_wsplitpath_s(pFileName,drive,_MAX_DRIVE,dir,_MAX_DIR,fname,MAX_PATH,ext,_MAX_EXT);
	_wcslwr_s(ext,_MAX_EXT);
	if(!wcscmp(ext,L".bin"))
	{
		bTextFile = false;
		return true;
	}
	if(!wcscmp(ext,L".txt"))
	{
		bTextFile = true;
		return true;
	}
	return false;
}

bool IsBinaryFile(const wchar_t *pFileName,int iSize,wchar_t *pDiffFileName)
{
	bool bTextFile;
	wchar_t drive[_MAX_DRIVE],dir[_MAX_DIR],fname[MAX_PATH],ext[_MAX_EXT];

	_wsplitpath_s(pFileName,drive,_MAX_DRIVE,dir,_MAX_DIR,fname,MAX_PATH,ext,_MAX_EXT);
	if(!wcscmp(ext,L".bin"))
	{
		bTextFile = false;
		swprintf(pDiffFileName,iSize,L"%s%s%s_diff.txt",drive,dir,fname);
		return true;
	}
	if(!wcscmp(ext,L".txt"))
	{
		bTextFile = true;
		return false;
	}
	return false;
}

_CUST_DRV_DEF_ void NotifyErrorFocalLaw(int iCycle)
{
	wchar_t msg[MAX_PATH];

	swprintf(msg,MAX_PATH,L"No reception element for cycle %d!",iCycle);
	OempaApiMessageBox(NULL,msg,L"File Error",MB_OK);
}

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

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
bool ReadCorrectedFileTextFocalLaw(int iCycle,bool bPulse,const wchar_t *pFileName,structRoot *pRoot,CFocalLaw *pFocalLaw)
{
	wchar_t cycle[MAX_PATH];
	bool bRet=true;
	int iCount,iCountExpected;

	if(bPulse)
		swprintf(cycle,MAX_PATH,L"Correction\\Cycle:%d\\Pulser",iCycle);
	else
		swprintf(cycle,MAX_PATH,L"Correction\\Cycle:%d\\Receiver",iCycle);

	if(!pRoot->rParamList->FileReadDouble(cycle,L"WedgeDelay",pFileName,1.0e-6,pFocalLaw->dWedgeDelay))
		{bRet = false;goto end;}
	if(!pRoot->rParamList->FileReadDouble(cycle,L"Skew",pFileName,M_PI/180.0,pFocalLaw->dSkew))
		{bRet = false;goto end;}
	if(!pRoot->rParamList->FileReadDouble(cycle,L"Angle",pFileName,M_PI/180.0,pFocalLaw->dAngle))
		{bRet = false;goto end;}
	if(!pRoot->rParamList->FileReadDouble(cycle,L"X",pFileName,1.0e-3,pFocalLaw->dX))
		{bRet = false;goto end;}
	if(!pRoot->rParamList->FileReadDouble(cycle,L"Y",pFileName,1.0e-3,pFocalLaw->dY))
		{bRet = false;goto end;}
	if(!pRoot->rParamList->FileReadDouble(cycle,L"Z",pFileName,1.0e-3,pFocalLaw->dZ))
		{bRet = false;goto end;}
	if(!pRoot->rParamList->FileReadDouble(cycle,L"FocalX",pFileName,1.0e-3,pFocalLaw->dFocalX,false))
		pFocalLaw->dFocalX = 0.0;
	if(!pRoot->rParamList->FileReadDouble(cycle,L"FocalY",pFileName,1.0e-3,pFocalLaw->dFocalY,false))
		pFocalLaw->dFocalY = 0.0;
	if(!pRoot->rParamList->FileReadDouble(cycle,L"FocalZ",pFileName,1.0e-3,pFocalLaw->dFocalZ,false))
		pFocalLaw->dFocalZ = 0.0;

	if(!pRoot->rParamList->FileReadInteger(cycle,L"TRLWedgeDelayReal.count",pFileName,iCount,false))
		iCount = 0;
	pFocalLaw->aTRLWedgeDelayReal.SetAllocatedSize(iCount);
	iCountExpected = iCount;
	if(iCount && !pRoot->rParamList->FileReadArrayFloat(cycle,L"TRLWedgeDelayReal",pFileName,iCount,1.0e-6f,&pFocalLaw->aTRLWedgeDelayReal[0],iCountExpected))
	{
		OempaApiMessageBox(NULL,L"TRLWedgeDelayReal",L"File Error",MB_OK);
		{bRet = false;goto end;}
	}
	if(!pRoot->rParamList->FileReadInteger(cycle,L"TRLWedgeDelayRef.count",pFileName,iCount,false))
		iCount = 0;
	pFocalLaw->aTRLWedgeDelayRef.SetAllocatedSize(iCount);
	iCountExpected = iCount;
	if(iCount && !pRoot->rParamList->FileReadArrayFloat(cycle,L"TRLWedgeDelayRef",pFileName,iCount,1.0e-6f,&pFocalLaw->aTRLWedgeDelayRef[0],iCountExpected))
	{
		OempaApiMessageBox(NULL,L"TRLWedgeDelayRef",L"File Error",MB_OK);
		{bRet = false;goto end;}
	}

end:
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////

bool ReadCorrectedFileText(	const wchar_t *pFileName,
							structRoot &root,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	bool bRet=true;
	wchar_t wcAux[MAX_PATH];
	structCycle *pCycleAux;
	CFocalLaw *pFocalLaw;
	int iCycleCount;

	if(root.iCycleCount<=0)
		return true;
	if(!pCycle || !pEmission || !pReception)
	{
		bRet = false;goto end;
	}
	if(!root.rParamList->FileReadInteger(L"Correction\\Root",L"CycleCount",pFileName,iCycleCount,false))
		return true;
	if(iCycleCount!=root.iCycleCount)
	{
		OempaApiMessageBox(NULL,L"CycleCount differents values in correction section and root section!",L"Error",MB_OK);
		bRet = false;goto end;
	}
	if(iCycleCount!=root.iCycleCount)
	{
		OempaApiMessageBox(NULL,L"Different cycles counts!",L"Error",MB_OK);
		bRet = false;goto end;
	}
	if(!root.rParamList->FileReadDouble(L"Correction\\Root",L"SpecimenVelocity",pFileName,1.0,root.dSpecimenVelocity))
		{bRet = false;goto end;}
	if(!root.rParamList->FileReadDouble(L"Correction\\Root",L"SpecimenRadius",pFileName,1.0e-3,root.dSpecimenRadius,false))
		root.dSpecimenRadius = 0.0;
	if(!root.rParamList->FileReadDouble(L"Correction\\Root",L"SpecimenThickness",pFileName,1.0e-3,root.dSpecimenThickness,false))
		root.dSpecimenThickness = 0.0;
	if(!root.rParamList->FileReadDouble(L"Correction\\Root",L"DigitizingPeriod",pFileName,1.0e-9,root.dDigitizingPeriod))
		{bRet = false;goto end;}

	for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
	{
		pCycleAux = &pCycle[iCycle];

		pFocalLaw = &pEmission[iCycle];
		if(!ReadCorrectedFileTextFocalLaw(iCycle,true,pFileName,&root,pFocalLaw))
		{
			swprintf(wcAux,MAX_PATH,L"Emission Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;goto end;
		}
		pFocalLaw = &pReception[iCycle];
		if(!ReadCorrectedFileTextFocalLaw(iCycle,false,pFileName,&root,pFocalLaw))
		{
			swprintf(wcAux,MAX_PATH,L"Reception Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;goto end;
		}
	}

end:
	return bRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////

bool WriteCorrectedFileTextFocalLaw(int iCycle,bool bPulse,const wchar_t *pFileCorrected,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pFocalLaw)
{
	wchar_t cycle[MAX_PATH];
	bool bRet=true;
	int iCount;

	if(bPulse)
	{
		swprintf(cycle,MAX_PATH,L"Correction\\Cycle:%d\\Pulser",iCycle);
		pFocalLaw->iFocalCount = 1;
	}else
		swprintf(cycle,MAX_PATH,L"Correction\\Cycle:%d\\Receiver",iCycle);

	if(!pRoot->rParamList->FileWriteDouble(cycle,L"WedgeDelay",pFileCorrected,1.0e6,pFocalLaw->dWedgeDelay,MICRO_SECOND))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"Angle",pFileCorrected,180.0/M_PI,pFocalLaw->dAngle,L"deg"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"Skew",pFileCorrected,180.0/M_PI,pFocalLaw->dSkew,L"deg"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"X",pFileCorrected,1.0e+3,pFocalLaw->dX,L"mm"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"Y",pFileCorrected,1.0e+3,pFocalLaw->dY,L"mm"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"Z",pFileCorrected,1.0e+3,pFocalLaw->dZ,L"mm"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"FocalX",pFileCorrected,1.0e+3,pFocalLaw->dFocalX,L"mm"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"FocalY",pFileCorrected,1.0e+3,pFocalLaw->dFocalY,L"mm"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"FocalZ",pFileCorrected,1.0e+3,pFocalLaw->dFocalZ,L"mm"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteInteger(cycle,L"Count",pFileCorrected,(int)pCycle->lPointCount,L""))
		bRet = false;

	iCount = pFocalLaw->aTRLWedgeDelayReal.GetAllocatedSize();
	if(bRet && iCount)
	{
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"TRLWedgeDelayReal.count",pFileCorrected,iCount,L""))
			iCount = 0;
		if(iCount && !pRoot->rParamList->FileWriteArrayFloat(cycle,L"TRLWedgeDelayReal",pFileCorrected,1.0e6f,&pFocalLaw->aTRLWedgeDelayReal[0],iCount,L"%.3f"MICRO_SECOND,L"%.3f;"))
		{
			OempaApiMessageBox(NULL,L"TRLWedgeDelayReal",L"File Error",MB_OK);
			bRet = false;
		}
	}
	iCount = pFocalLaw->aTRLWedgeDelayRef.GetAllocatedSize();
	if(bRet && iCount)
	{
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"TRLWedgeDelayRef.count",pFileCorrected,iCount,L""))
			iCount = 0;
		if(iCount && !pRoot->rParamList->FileWriteArrayFloat(cycle,L"TRLWedgeDelayRef",pFileCorrected,1.0e6f,&pFocalLaw->aTRLWedgeDelayRef[0],iCount,L"%.3f"MICRO_SECOND,L"%.3f;"))
		{
			OempaApiMessageBox(NULL,L"TRLWedgeDelayRef",L"File Error",MB_OK);
			bRet = false;
		}
	}

	//if(bRet)
	//	NewLine(pFileCorrected);
	return bRet;
}

bool WriteCorrectedFileTextFocalLaw(int iCycle,bool bPulse,const wchar_t *pFileCorrected,structRoot *pRoot,structCorrectionOEMPA &beam)
{
	wchar_t cycle[MAX_PATH];
	bool bRet=true;

	if(bPulse)
	{
		swprintf(cycle,MAX_PATH,L"Correction\\Cycle:%d\\Pulser",iCycle);
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"WedgeDelay",pFileCorrected,1.0e6f,0.0/*pFocalLaw->dWedgeDelay*/,MICRO_SECOND))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"CenterDelay",pFileCorrected,1.0e6f,0.0/*pFocalLaw->dCenterDelay*/,MICRO_SECOND))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"Angle",pFileCorrected,180.0/M_PI,beam.pdAnglePulse[iCycle]/*pFocalLaw->dAngle*/,L"deg"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"Skew",pFileCorrected,180.0/M_PI,beam.pdSkewPulse[iCycle]/*pFocalLaw->dSkew*/,L"deg"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"X",pFileCorrected,1.0e+3,beam.pdXPulse[iCycle]/*pFocalLaw->dX*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"Y",pFileCorrected,1.0e+3,beam.pdYPulse[iCycle]/*pFocalLaw->dY*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"Z",pFileCorrected,1.0e+3,beam.pdZPulse[iCycle]/*pFocalLaw->dZ*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"FocalX",pFileCorrected,1.0e+3,beam.pdXFocalPulse[iCycle]/*pFocalLaw->dFocalX*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"FocalY",pFileCorrected,1.0e+3,beam.pdYFocalPulse[iCycle]/*pFocalLaw->dFocalY*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"FocalZ",pFileCorrected,1.0e+3,beam.pdZFocalPulse[iCycle]/*pFocalLaw->dFocalZ*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"Count",pFileCorrected,0/*(int)pCycle->lPointCount*/,L""))
			bRet = false;
	}else{
		swprintf(cycle,MAX_PATH,L"Correction\\Cycle:%d\\Receiver",iCycle);
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"WedgeDelay",pFileCorrected,1.0e6f,0.0/*pFocalLaw->dWedgeDelay*/,MICRO_SECOND))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"CenterDelay",pFileCorrected,1.0e6f,0.0/*pFocalLaw->dCenterDelay*/,MICRO_SECOND))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"Angle",pFileCorrected,180.0/M_PI,beam.pdAngleRecv[iCycle]/*pFocalLaw->dAngle*/,L"deg"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"Skew",pFileCorrected,180.0/M_PI,beam.pdSkewRecv[iCycle]/*pFocalLaw->dSkew*/,L"deg"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"X",pFileCorrected,1.0e+3,beam.pdXRecv[iCycle]/*pFocalLaw->dX*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"Y",pFileCorrected,1.0e+3,beam.pdYRecv[iCycle]/*pFocalLaw->dY*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"Z",pFileCorrected,1.0e+3,beam.pdZRecv[iCycle]/*pFocalLaw->dZ*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"FocalX",pFileCorrected,1.0e+3,beam.pdXFocalRecv[iCycle]/*pFocalLaw->dFocalX*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"FocalY",pFileCorrected,1.0e+3,beam.pdYFocalRecv[iCycle]/*pFocalLaw->dFocalY*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"FocalZ",pFileCorrected,1.0e+3,beam.pdZFocalRecv[iCycle]/*pFocalLaw->dFocalZ*/,L"mm"))
			bRet = false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"Count",pFileCorrected,beam.piCountRecv[iCycle]/*(int)pCycle->lPointCount*/,L""))
			bRet = false;
	}

	//if(bRet)
	//	NewLine(pFileCorrected);
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////

_CUST_DRV_DEF_ bool WriteCorrectedFileText(	const wchar_t *pFileCorrected,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	bool bRet=true;
	wchar_t wcAux[MAX_PATH];
	structCycle *pCycleAux;

	//DeleteFile(pFileCorrected);
	if(!pRoot->rParamList)
		return false;//1.1.5.2v
	if(!pRoot->rParamList->FileWriteInteger(L"Correction\\Root",L"CycleCount",pFileCorrected,pRoot->iCycleCount,NULL))
		return false;
	if(!pRoot->rParamList->FileWriteDouble(L"Correction\\Root",L"SpecimenVelocity",pFileCorrected,1.0,pRoot->dSpecimenVelocity,L"m/s"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(L"Correction\\Root",L"SpecimenRadius",pFileCorrected,1.0e3,pRoot->dSpecimenRadius,L"mm"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(L"Correction\\Root",L"SpecimenThickness",pFileCorrected,1.0e3,pRoot->dSpecimenThickness,L"mm"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(L"Correction\\Root",L"DigitizingPeriod",pFileCorrected,1.0e+9,pRoot->dDigitizingPeriod,L"ns"))
		bRet = false;

	//if(bRet)
	//	NewLine(pFileCorrected);

	for(int iCycle=0;iCycle<pRoot->iCycleCount;iCycle++)
	{
		pCycleAux = &pCycle[iCycle];

		if(!WriteCorrectedFileTextFocalLaw(iCycle,true,pFileCorrected,pRoot,pCycleAux,&pEmission[iCycle]))
		{
			swprintf(wcAux,MAX_PATH,L"Correction Emission Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;
		}
		if(!WriteCorrectedFileTextFocalLaw(iCycle,false,pFileCorrected,pRoot,pCycleAux,&pReception[iCycle]))
		{
			swprintf(wcAux,MAX_PATH,L"Correction Reception Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;
		}
	}

	return bRet;
}

_CUST_DRV_DEF_ bool OEMPA_WriteCorrectedFileText(	const wchar_t *pFileCorrected,structRoot *pRoot,structCorrectionOEMPA &beam)
{
	bool bRet=true;
	bool bAllocParamList=false;
	wchar_t wcAux[MAX_PATH];

	if(!pRoot->rParamList)
	{
		pRoot->rParamList = new CRead_Write_ParametersList();
		bAllocParamList = true;
	}
	if(!pRoot->rParamList)
	{
		OempaApiMessageBox(NULL,L"pRoot->rParamList is NULL!",L"Error",MB_OK);
		return false;
	}

	//DeleteFile(pFileCorrected);
	if(!pRoot->rParamList->FileWriteInteger(L"Correction\\Root",L"CycleCount",pFileCorrected,beam.iCount/*pRoot->iCycleCount*/,NULL))
		return false;
	if(!pRoot->rParamList->FileWriteDouble(L"Correction\\Root",L"SpecimenVelocity",pFileCorrected,1.0,beam.dSpecimenVelocity/*pRoot->dSpecimenVelocity*/,L"m/s"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(L"Correction\\Root",L"SpecimenRadius",pFileCorrected,1.0e3,beam.dSpecimenRadius/*pRoot->dSpecimenRadius*/,L"mm"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(L"Correction\\Root",L"SpecimenThickness",pFileCorrected,1.0e3,beam.dSpecimenThickness/*pRoot->dSpecimenThickness*/,L"mm"))
		bRet = false;
	if(!pRoot->rParamList->FileWriteDouble(L"Correction\\Root",L"DigitizingPeriod",pFileCorrected,1.0e+9,beam.dDigitizingPeriod/*pRoot->dDigitizingPeriod*/,L"ns"))
		bRet = false;

	//if(bRet)
	//	NewLine(pFileCorrected);

	for(int iCycle=0;iCycle<beam.iCount;iCycle++)
	{
		if(!WriteCorrectedFileTextFocalLaw(iCycle,true,pFileCorrected,pRoot,beam))
		{
			swprintf(wcAux,MAX_PATH,L"Correction Emission Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;
		}
		if(!WriteCorrectedFileTextFocalLaw(iCycle,false,pFileCorrected,pRoot,beam))
		{
			swprintf(wcAux,MAX_PATH,L"Correction Reception Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;
		}
	}

	if(bAllocParamList && pRoot->rParamList)
	{
		delete pRoot->rParamList;
		pRoot->rParamList = NULL;
	}

	return bRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool WriteFileTextFilter(int iFilterIndex,const wchar_t *pFileName,structRoot *pRoot)
{
	wchar_t filter[MAX_PATH];
	bool bRet=true;

	swprintf(filter,MAX_PATH,L"Filter:%d",iFilterIndex);

	if(!pRoot->rParamList->FileWriteString(filter,L"Title",pFileName,pRoot->aFilter[iFilterIndex].pTitle))
		return false;
	if(!pRoot->rParamList->FileWriteWord(filter,L"Scale",pFileName,pRoot->aFilter[iFilterIndex].wScale,NULL))
		return false;
	if(!pRoot->rParamList->FileWriteInteger(filter,L"Coefficient.count",pFileName,g_iOEMPAFilterCoefficientMax,NULL))
		bRet = false;
	if(!pRoot->rParamList->FileWriteArrayShort(filter,L"Coefficient",pFileName,&pRoot->aFilter[iFilterIndex].aCoefficient[0],g_iOEMPAFilterCoefficientMax,NULL))
	{
		OempaApiMessageBox(NULL,L"Coefficient",L"File Error",MB_OK);
		bRet = false;
	}

	//if(bRet)
	//	NewLine(pFileName);
	return bRet;
}

bool WriteFileTextCycleGate(int iCycle,int iGate,const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle)
{
	wchar_t cycle[MAX_PATH];
	bool bRet=true;
	structGate *pGate=&pCycle->gate[iGate];

	swprintf(cycle,MAX_PATH,L"Cycle:%d\\Gate:%d",iCycle,iGate);

	if(!pRoot->rParamList->FileWriteBoolean(cycle,L"Enable",pFileName,pGate->bEnable))
		return false;
	if(!pGate->bEnable)
		return true;//1.1.5.0u
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"Start",pFileName,1.0e6,pGate->dStart,MICRO_SECOND))
		return false;
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"Stop",pFileName,1.0e6,pGate->dStop,MICRO_SECOND))
		return false;
	
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"Threshold",pFileName,1.0,pGate->dThreshold,L"%"))
		return false;
	
	if(!pRoot->rParamList->FileWriteEnum(cycle,L"Rectification",pFileName,L"Signed"L"\0"L"Unsigned"L"\0"L"UnsignedPositive"L"\0"L"UnsignedNegative"L"\0",(int)pGate->eRectification))
		bRet = false;
	if(!pRoot->rParamList->FileWriteEnum(cycle,L"ModeAmp",pFileName,L"Absolute"L"\0"L"Maximum"L"\0"L"Minimum"L"\0"L"PeakToPeak"L"\0",(int)pGate->eModeAmp))
		bRet = false;
	if(!pRoot->rParamList->FileWriteEnum(cycle,L"ModeTof",pFileName,L"AmplitudeDetection"L"\0"L"ThresholdCross"L"\0"L"ZeroFirstAfterThresholdCross"L"\0"L"ZeroLastBeforeThresholdCross"L"\0",(int)pGate->eModeTof))
		bRet = false;

	if((pRoot->ullSavedParameters & OEMPA_CYCLE_ACQ_CHANNEL_ID) || (pGate->wAcqIDAmp!=0xffff) || (pGate->wAcqIDTof!=0xffff))
	{
		if(!pRoot->rParamList->FileWriteWord(cycle,L"AcqIDAmp",pFileName,pGate->wAcqIDAmp,NULL))
			return false;
		if(!pRoot->rParamList->FileWriteWord(cycle,L"AcqIDTof",pFileName,pGate->wAcqIDTof,NULL))
			return false;
	}

	if((pRoot->ullSavedParameters & OEMPA_TRACKING) || pGate->bTrackingStartEnable)
	{
		if(!pRoot->rParamList->FileWriteBoolean(cycle,L"TrackingStartEnable",pFileName,pGate->bTrackingStartEnable))
			return false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"TrackingStartIndexCycle",pFileName,pGate->iTrackingStartIndexCycle,L""))
			bRet = false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"TrackingStartIndexGate",pFileName,pGate->iTrackingStartIndexGate,L""))
			bRet = false;
	}
	if((pRoot->ullSavedParameters & OEMPA_TRACKING) || pGate->bTrackingStopEnable)
	{
		if(!pRoot->rParamList->FileWriteBoolean(cycle,L"TrackingStopEnable",pFileName,pGate->bTrackingStopEnable))
			return false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"TrackingStopIndexCycle",pFileName,pGate->iTrackingStopIndexCycle,L""))
			bRet = false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"TrackingStopIndexGate",pFileName,pGate->iTrackingStopIndexGate,L""))
			bRet = false;
	}

	//if(bRet)
	//	NewLine(pFileName);
	return bRet;
}

bool WriteFileTextCycle(int iCycle,const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycleIn)
{
	wchar_t cycle[MAX_PATH];
	bool bRet=true;
	structCycle *pCycle=&pCycleIn[iCycle];

	swprintf(cycle,MAX_PATH,L"Cycle:%d",iCycle);

	if(!pRoot->rParamList->FileWriteDouble(cycle,L"GainDigital",pFileName,1.0,pCycle->dGainDigital,L"dB"))//dB
		return false;

	if((pRoot->ullSavedParameters & OEMPA_ENABLE_MULTI_CHANNEL) || pRoot->bEnableMultiHWChannel)
	{
		if(!pRoot->rParamList->FileWriteBoolean(cycle,L"HWAcquisition",pFileName,pCycle->bHWAcquisition))
		{
			if(!pRoot->rParamList->FileWriteBoolean(cycle,L"MultiChannelAcquisition",pFileName,pCycle->bHWAcquisition))
				return false;
		}
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"BeamCorrection",pFileName,1.0,pCycle->fBeamCorrection,L"dB"))//dB
			return false;
	}else{
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"BeamCorrection",pFileName,1.0,pCycle->fBeamCorrection,L"dB"))//dB
			return false;
	}
	if(!pRoot->bEnableMultiHWChannel && !pRoot->rParamList->FileWriteDouble(cycle,L"Start",pFileName,1.0e6,pCycle->dStart,MICRO_SECOND))
		return false;
	if(!pRoot->rParamList->FileWriteDouble(cycle,L"Range",pFileName,1.0e6,pCycle->dRange,MICRO_SECOND))
		return false;

	if(!pRoot->bEnableMultiHWChannel)
	{
		if(!pRoot->rParamList->FileWriteDouble(cycle,L"TimeSlot",pFileName,1.0e6,pCycle->dTimeSlot,MICRO_SECOND))
			return false;
		if((pCycle->dFMCSubTimeSlotAcq>0.0) && !pRoot->rParamList->FileWriteDouble(cycle,L"FMCSubTimeSlotAcq",pFileName,1.0e6,pCycle->dFMCSubTimeSlotAcq,MICRO_SECOND))
			return false;
		if((pCycle->dFMCSubTimeSlotReplay>0.0) && !pRoot->rParamList->FileWriteDouble(cycle,L"FMCSubTimeSlotReplay",pFileName,1.0e6,pCycle->dFMCSubTimeSlotReplay,MICRO_SECOND))
			return false;
	}else{
		if(pCycle->bHWAcquisition)
		{
			if(!pRoot->rParamList->FileWriteDouble(cycle,L"HWAcquisitionTime",pFileName,1.0e6,pCycle->dTimeSlot,MICRO_SECOND))
				return false;
		}else{
			if(!pRoot->rParamList->FileWriteDouble(cycle,L"HWReplayTime",pFileName,1.0e6,pCycle->dTimeSlot,MICRO_SECOND))
				return false;
		}
	}
	if(pCycle->eCompressionType==eCompression)
	{
		if(!pRoot->rParamList->FileWriteLong(cycle,L"PointCount",pFileName,pCycle->lPointCount,NULL))
			return false;
		if(!pRoot->rParamList->FileWriteEnum(cycle,L"CompressionType",pFileName,L"Compression"L"\0"L"Decimation"L"\0",(int)pCycle->eCompressionType))
			bRet = false;
	}else
	if(pCycle->eCompressionType==eDecimation)
	{
		if(!pRoot->rParamList->FileWriteLong(cycle,L"PointFactor",pFileName,pCycle->lPointFactor,NULL))
			return false;
		if(!pRoot->rParamList->FileWriteEnum(cycle,L"CompressionType",pFileName,L"Compression"L"\0"L"Decimation"L"\0",(int)pCycle->eCompressionType))
			bRet = false;
	}else{//old code
		if((pRoot->ullSavedParameters & OEMPA_CYCLE_POINTCOUNT_AND_COMPRESSION) && !pRoot->rParamList->FileWriteLong(cycle,L"PointCount",pFileName,pCycle->lPointCount,NULL))
			return false;
		if((pRoot->ullSavedParameters & OEMPA_CYCLE_POINTFACTOR_AND_COMPRESSION) && !pRoot->rParamList->FileWriteLong(cycle,L"PointFactor",pFileName,pCycle->lPointFactor,NULL))
			return false;
		if((pRoot->ullSavedParameters & (OEMPA_CYCLE_POINTCOUNT_AND_COMPRESSION | OEMPA_CYCLE_POINTFACTOR_AND_COMPRESSION)) &&
			!pRoot->rParamList->FileWriteEnum(cycle,L"CompressionType",pFileName,L"Compression"L"\0"L"Decimation"L"\0",(int)pCycle->eCompressionType))
			bRet = false;
	}
	if(!pRoot->rParamList->FileWriteEnum(cycle,L"Rectification",pFileName,L"Signed"L"\0"L"Unsigned"L"\0"L"UnsignedPositive"L"\0"L"UnsignedNegative"L"\0",(int)pCycle->eRectification))
		bRet = false;

	if(pCycle->bDACEnable || pCycle->iDACCount || (pRoot->ullSavedParameters & (OEMPA_CYCLE_DAC_SLOPE | OEMPA_CYCLE_DAC_GAIN)))
	{
		if(!pRoot->rParamList->FileWriteBoolean(cycle,L"DACEnable",pFileName,pCycle->bDACEnable))
			return false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"DACTof.count",pFileName,pCycle->iDACCount,NULL))
			bRet = false;
		if(!pRoot->rParamList->FileWriteArrayDouble(cycle,L"DACTof",pFileName,1.0e6,pCycle->adDACTof,pCycle->iDACCount,MICRO_SECOND))
		{
			OempaApiMessageBox(NULL,L"DACTof",L"File Error",MB_OK);
			bRet = false;
		}
		if(pCycle->bDACSlope)
		{
			if(!pRoot->rParamList->FileWriteInteger(cycle,L"DACSlope.count",pFileName,pCycle->iDACCount,NULL))
				bRet = false;
			if(!pRoot->rParamList->FileWriteArrayFloat(cycle,L"DACSlope",pFileName,1.0e-6f,pCycle->afDACPrm,pCycle->iDACCount,L"%.8f dB/"MICRO_SECOND,L"%.8f;"))
			{
				OempaApiMessageBox(NULL,L"DACSlope",L"File Error",MB_OK);
				bRet = false;
			}
		}else{
			if(!pRoot->rParamList->FileWriteInteger(cycle,L"DACGain.count",pFileName,pCycle->iDACCount,NULL))
				bRet = false;
			if(!pRoot->rParamList->FileWriteArrayFloat(cycle,L"DACGain",pFileName,1.0f,pCycle->afDACPrm,pCycle->iDACCount,L"%.8f dB",L"%.8f;"))
			{
				OempaApiMessageBox(NULL,L"DACGain",L"File Error",MB_OK);
				bRet = false;
			}
			if(!pRoot->rParamList->FileWriteBoolean(cycle,L"DACAutoStop",pFileName,false))
				return false;
		}
	}

	if((pRoot->ullSavedParameters & OEMPA_CYCLE_ASCAN_BUFFER) || !pCycle->bMaximum)
	if(!pRoot->rParamList->FileWriteBoolean(cycle,L"Maximum",pFileName,pCycle->bMaximum))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_CYCLE_ASCAN_BUFFER) || pCycle->bMinimum)
	if(!pRoot->rParamList->FileWriteBoolean(cycle,L"Minimum",pFileName,pCycle->bMinimum))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_CYCLE_ASCAN_BUFFER) || pCycle->bSaturation)
	if(!pRoot->rParamList->FileWriteBoolean(cycle,L"Saturation",pFileName,pCycle->bSaturation))
		return false;

	if((pRoot->ullSavedParameters & OEMPA_CYCLE_ACQ_CHANNEL_ID) ||
		((pCycle->wAcqIdChannelProbe!=0xFFFF) || (pCycle->wAcqIdChannelScan!=0xFFFF) || (pCycle->wAcqIdChannelCycle!=0xFFFF)))
	{
		if(!pRoot->rParamList->FileWriteWord(cycle,L"AcqIdChannelProbe",pFileName,pCycle->wAcqIdChannelProbe,NULL))
			return false;
		if(!pRoot->rParamList->FileWriteWord(cycle,L"AcqIdChannelScan",pFileName,pCycle->wAcqIdChannelScan,NULL))
			return false;
		if(!pRoot->rParamList->FileWriteWord(cycle,L"AcqIdChannelCycle",pFileName,pCycle->wAcqIdChannelCycle,NULL))
			return false;
	}

	if(!pRoot->rParamList->FileWriteInteger(cycle,L"FilterIndex",pFileName,pCycle->iFilterIndex,NULL))
		bRet = false;
	if(!pRoot->bEnableMultiHWChannel || (pRoot->bEnableMultiHWChannel && pCycle->bHWAcquisition))
	if(!pRoot->rParamList->FileWriteFloat(cycle,L"GainAnalog",pFileName,1.0f,pCycle->fGainAnalog,L"dB"))//dB
		return false;

	if(!pRoot->rParamList->FileWriteInteger(cycle,L"GateCount",pFileName,pCycle->iGateCount,NULL))
		bRet = false;

	if((pRoot->ullSavedParameters & OEMPA_TRACKING) || pCycle->bTrackingAscanEnable)
	{
		if(!pRoot->rParamList->FileWriteBoolean(cycle,L"TrackingAscanEnable",pFileName,pCycle->bTrackingAscanEnable))
			return false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"TrackingAscanIndexCycle",pFileName,pCycle->iTrackingAscanIndexCycle,L""))
			bRet = false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"TrackingAscanIndexGate",pFileName,pCycle->iTrackingAscanIndexGate,L""))
			bRet = false;
	}
	if((pRoot->ullSavedParameters & OEMPA_TRACKING) || pCycle->bTrackingDacEnable)
	{
		if(!pRoot->rParamList->FileWriteBoolean(cycle,L"TrackingDacEnable",pFileName,pCycle->bTrackingDacEnable))
			return false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"TrackingDacIndexCycle",pFileName,pCycle->iTrackingDacIndexCycle,L""))
			bRet = false;
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"TrackingDacIndexGate",pFileName,pCycle->iTrackingDacIndexGate,L""))
			bRet = false;
	}

	//if(bRet)
	//	NewLine(pFileName);

	for(int iGateIndex=0;iGateIndex<g_iOEMPAGateCountMax;iGateIndex++)
	{
		//1.1.5.0u if(!pCycle->gate[iGateIndex].bEnable)
		//1.1.5.0u	continue;
		if(iGateIndex>=pCycle->iGateCount)
			break;//1.1.5.0u
		if(!WriteFileTextCycleGate(iCycle,iGateIndex,pFileName,pRoot,pCycle))
		{
			OempaApiMessageBox(NULL,L"Gate",L"File Error",MB_OK);
			bRet = false;
		}
	}

	return bRet;
}

bool WriteFileTextMultiHWChannel(int iCycle,bool bPulse,bool bHWAcquisition,const wchar_t *pFileName,structRoot *pRoot,CFocalLaw *pFocalLaw);
bool WriteFileTextFocalLaw(int iCycle,bool bPulse,bool bHWAcquisition,const wchar_t *pFileName,structRoot *pRoot,CFocalLaw *pFocalLaw)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t cycle[MAX_PATH];
	bool bRet=true;
	enumFocusing eFocusing;
	int aiElement[2*g_iOEMPAApertureElementCountMax];
	DWORD dwElement,dwCurrent;
	int iIndex;

	if(pRoot->bEnableMultiHWChannel)
		return WriteFileTextMultiHWChannel(iCycle,bPulse,bHWAcquisition,pFileName,pRoot,pFocalLaw);

	if(bPulse)
	{
		swprintf(cycle,MAX_PATH,L"Cycle:%d\\Pulser",iCycle);
		pFocalLaw->iFocalCount = 1;
	}else
		swprintf(cycle,MAX_PATH,L"Cycle:%d\\Receiver",iCycle);

	if(!pRoot->rParamList->FileWriteDouble(cycle,L"WedgeDelay",pFileName,1.0e6,pFocalLaw->dWedgeDelay,MICRO_SECOND))
		bRet = false;
	if(!pRoot->rParamList->FileWriteFloat(cycle,L"CenterDelay",pFileName,1.0e6f,pFocalLaw->fCenterDelay,MICRO_SECOND))
		bRet = false;
	if(!pRoot->rParamList->FileWriteInteger(cycle,L"Element.count",pFileName,pFocalLaw->iElementCount,NULL))
		bRet = false;
	iIndex = 0;
	for(int iAp=0;iAp<pFocalLaw->adwElement.GetAllocatedSize();iAp++)
	{
		dwElement = pFocalLaw->adwElement[iAp];
		dwCurrent = 1;
		for(int iElem=0;iElem<32;iElem++)
		{
			if(dwElement & dwCurrent)
				aiElement[iIndex++] = iElem+32*iAp;
			dwCurrent = dwCurrent<<1;
		}
	}
	if(iIndex!=pFocalLaw->iElementCount)
	{
		OempaApiMessageBox(NULL,L"Element count error!",L"File Error",MB_OK);
		bRet = false;
	}
	if(!pRoot->rParamList->FileWriteArrayInteger(cycle,L"Element",pFileName,aiElement,pFocalLaw->iElementCount,NULL))
	{
		OempaApiMessageBox(NULL,L"Element",L"File Error",MB_OK);
		bRet = false;
	}

	eFocusing = eStandard;
	if(!bPulse)
	{
		if(pFocalLaw->iFocalCount>1)
			eFocusing = eDDF;
		if(!pRoot->rParamList->FileWriteEnum(cycle,L"Focusing",pFileName,L"Standard"L"\0"L"DDF"L"\0",(int)eFocusing))
			bRet = false;
	}

	if(!bPulse && (eFocusing==eDDF))
	{
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"FocalTimeOfFlight.count",pFileName,pFocalLaw->iFocalCount,NULL))
			bRet = false;
		if(pFocalLaw->iElementCount && !pRoot->rParamList->FileWriteArrayDouble(cycle,L"FocalTimeOfFlight",pFileName,1.0e6,pFocalLaw->adFocalTimeOfFlight,pFocalLaw->iFocalCount,MICRO_SECOND))
		{
			OempaApiMessageBox(NULL,L"FocalTimeOfFlight",L"File Error",MB_OK);
			bRet = false;
		}
	}

	swprintf(wcAux,MAX_PATH,L"%d;%d",pFocalLaw->iElementCount,pFocalLaw->iFocalCount);
	if(!pRoot->rParamList->FileWriteString(cycle,L"Delay.count",pFileName,wcAux))
		bRet = false;

	if(bPulse)
	{
		if(!pRoot->rParamList->FileWriteArrayFloat(cycle,L"Delay",pFileName,1.0e6f,&pFocalLaw->afDelay[0][0],pFocalLaw->iElementCount,MICRO_SECOND))
		{
			OempaApiMessageBox(NULL,L"Delay",L"File Error",MB_OK);
			bRet = false;
		}
		swprintf(wcAux,MAX_PATH,L"%d",pFocalLaw->iElementCount);
		if(!pRoot->rParamList->FileWriteString(cycle,L"Width.count",pFileName,wcAux))
			bRet = false;
		if(!pRoot->rParamList->FileWriteArrayFloat(cycle,L"Width",pFileName,1.0e6,pFocalLaw->afPrm,pFocalLaw->iElementCount,MICRO_SECOND))
		{
			OempaApiMessageBox(NULL,L"Width",L"File Error",MB_OK);
			bRet = false;
		}
	}else{
		if(!pRoot->rParamList->FileWriteArrayFloat2D(cycle,L"Delay",pFileName,pFocalLaw->iElementCount,pFocalLaw->iFocalCount,1.0e6f,pFocalLaw->afDelay,MICRO_SECOND))
		{
			OempaApiMessageBox(NULL,L"Delay",L"File Error",MB_OK);
			bRet = false;
		}
		swprintf(wcAux,MAX_PATH,L"%d",pFocalLaw->iElementCount);
		if(!pRoot->rParamList->FileWriteString(cycle,L"Gain.count",pFileName,wcAux))
			bRet = false;
		if(!pRoot->rParamList->FileWriteArrayFloat(cycle,L"Gain",pFileName,1.0,pFocalLaw->afPrm,pFocalLaw->iElementCount,L"dB"))
		{
			OempaApiMessageBox(NULL,L"Gain",L"File Error",MB_OK);
			bRet = false;
		}
		if(!pRoot->bEnableFMC)
		{//1.1.5.1 FMC
			swprintf(wcAux,MAX_PATH,L"%d",pFocalLaw->iFocalCount);
			if(!pRoot->rParamList->FileWriteString(cycle,L"FocalTimeOfFlight.count",pFileName,wcAux))
				bRet = false;
			if(!pRoot->rParamList->FileWriteArrayDouble(cycle,L"FocalTimeOfFlight",pFileName,1.0e6,pFocalLaw->adFocalTimeOfFlight,pFocalLaw->iFocalCount,MICRO_SECOND))
			{
				OempaApiMessageBox(NULL,L"FocalTimeOfFlight",L"File Error",MB_OK);
				bRet = false;
			}
		}
	}

	//if(bRet)
	//	NewLine(pFileName);
	return bRet;
}

bool WriteFileTextMultiHWChannel(int iCycle,bool bPulse,bool bHWAcquisition,const wchar_t *pFileName,structRoot *pRoot,CFocalLaw *pFocalLaw)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t cycle[MAX_PATH];
	bool bRet=true;
	int aiElement[2*g_iOEMPAApertureElementCountMax];
	DWORD dwElement,dwCurrent;
	int iIndex;
	int aiDecimation[g_iOEMPAApertureElementCountMax],iHWChDecimationCount,iHWChDecimationFirst;
	int iStartFirst,iStartCount;

	if(!pRoot->bEnableMultiHWChannel)
		return false;

	//1.1.5.3u if(bPulse && !bHWAcquisition)
	//1.1.5.3u	return true;

	if(bPulse)
	{
		swprintf(cycle,MAX_PATH,L"Cycle:%d\\Pulser",iCycle);
		pFocalLaw->iFocalCount = 1;
	}else
		swprintf(cycle,MAX_PATH,L"Cycle:%d\\Receiver",iCycle);

	//1.1.5.3u if((bPulse && bHWAcquisition) || !bPulse)
	if(pFocalLaw->iElementCount>0)
	{
		iIndex = 0;
		for(int iAp=0;iAp<pFocalLaw->adwElement.GetAllocatedSize();iAp++)
		{
			dwElement = pFocalLaw->adwElement[iAp];
			dwCurrent = 1;
			for(int iElem=0;iElem<32;iElem++)
			{
				if(dwElement & dwCurrent)
					aiElement[iIndex++] = iElem+32*iAp;
				dwCurrent = dwCurrent<<1;
			}
		}
		//1.1.5.3u pFocalLaw->iElementCount = iIndex;//because ReadHWWriteFileMultiHWChannel dont set this value.
		if(!pRoot->rParamList->FileWriteInteger(cycle,L"Element.count",pFileName,pFocalLaw->iElementCount,NULL))
			bRet = false;
		if(!pRoot->rParamList->FileWriteArrayInteger(cycle,L"Element",pFileName,aiElement,pFocalLaw->iElementCount,NULL))
		{
			OempaApiMessageBox(NULL,L"Element",L"File Error",MB_OK);
			bRet = false;
		}
	}

	pFocalLaw->eFocusing = eStandard;
	pFocalLaw->iFocalCount = 1;

	if(bHWAcquisition && !bPulse)
	{
		if(!pFocalLaw->GetMultiChannel(iStartFirst,iStartCount))
		{
			iStartFirst = 0;
			iStartCount = pFocalLaw->iDelayCount;
		}
		swprintf(wcAux,MAX_PATH,L"%d",iStartFirst);
		if(!pRoot->rParamList->FileWriteString(cycle,L"StartFirst",pFileName,wcAux))
			bRet = false;
		swprintf(wcAux,MAX_PATH,L"%d",iStartCount);
		if(!pRoot->rParamList->FileWriteString(cycle,L"Start.count",pFileName,wcAux))
			bRet = false;
		if(!pRoot->rParamList->FileWriteArrayFloat(cycle,L"Start",pFileName,1.0e6f,&pFocalLaw->afDelay[0][iStartFirst],iStartCount,MICRO_SECOND))
		{
			OempaApiMessageBox(NULL,L"Start",L"File Error",MB_OK);
			bRet = false;
		}
		pFocalLaw->GetMultiChannelDecimation(iHWChDecimationFirst,iHWChDecimationCount,aiDecimation);
		if((pFocalLaw->iDelayCount>0) || (pRoot->ullSavedParameters & OEMMC_DECIMATION))
		{
			if(!pRoot->rParamList->FileWriteArrayInteger(cycle,L"Decimation",pFileName,&aiDecimation[iStartFirst],iStartCount,NULL))
			{
				OempaApiMessageBox(NULL,L"Decimation",L"File Error",MB_OK);
				bRet = false;
			}
		}
	}

	if(bPulse)
	{//1.1.5.3u
		//1.1.5.3u if(bHWAcquisition && bPulse)
		if(pFocalLaw->iElementCount>0)
		{
			swprintf(wcAux,MAX_PATH,L"%d",pFocalLaw->iElementCount);
			if(!pRoot->rParamList->FileWriteString(cycle,L"Width.count",pFileName,wcAux))
				bRet = false;
			if(!pRoot->rParamList->FileWriteArrayFloat(cycle,L"Width",pFileName,1.0e6,pFocalLaw->afPrm,pFocalLaw->iElementCount,MICRO_SECOND))
			{
				OempaApiMessageBox(NULL,L"Width",L"File Error",MB_OK);
				bRet = false;
			}
		}
		for(int iElement=0;iElement<pFocalLaw->iElementCount;iElement++)
		{
			if(!pFocalLaw->afDelay[0][iElement])
				continue;
			swprintf(wcAux,MAX_PATH,L"%d;1",pFocalLaw->iElementCount);
			if(!pRoot->rParamList->FileWriteString(cycle,L"Delay.count",pFileName,wcAux))
				bRet = false;
			if(!pRoot->rParamList->FileWriteArrayFloat(cycle,L"Delay",pFileName,1.0e6f,&pFocalLaw->afDelay[0][0],pFocalLaw->iElementCount,MICRO_SECOND))
			{
				OempaApiMessageBox(NULL,L"Delay",L"File Error",MB_OK);
				bRet = false;
			}
			break;
		}
	}

	return bRet;
}

bool ReadHWWriteTextSubSequence(const wchar_t *pFileName,structRoot &root)
{
	bool bRet=true;
	int iSubSequenceCount;

	iSubSequenceCount = root.iSubSequenceEncoderCount;
	if(!iSubSequenceCount)
		iSubSequenceCount = root.iSubSequenceGateCount;
	if(iSubSequenceCount<=1)
		return true;
	iSubSequenceCount--;

	if(!root.rParamList->FileWriteInteger(L"SubSequence",L"CycleStart.count",pFileName,iSubSequenceCount,NULL))
		return true;
	if(!root.rParamList->FileWriteArrayInteger(L"SubSequence",L"CycleStart",pFileName,root.aiSubSequenceCycleStart,iSubSequenceCount,NULL))
	{
		OempaApiMessageBox(NULL,L"SubSequence/CycleStart",L"File Error",MB_OK);
		bRet = false;
	}

	if(!root.rParamList->FileWriteInteger(L"SubSequence",L"CycleStop.count",pFileName,iSubSequenceCount,NULL))
		return true;
	if(!root.rParamList->FileWriteArrayInteger(L"SubSequence",L"CycleStop",pFileName,root.aiSubSequenceCycleStop,iSubSequenceCount,NULL))
	{
		OempaApiMessageBox(NULL,L"SubSequence/CycleStop",L"File Error",MB_OK);
		bRet = false;
	}

	if(root.iSubSequenceEncoderCount>0)
	{
		if(!root.rParamList->FileWriteInteger(L"SubSequence",L"Encoder.count",pFileName,iSubSequenceCount+1,NULL))
			return true;
		if(!root.rParamList->FileWriteArrayFloat(L"SubSequence",L"Encoder",pFileName,1.0e3f,root.afSubSequenceValue,iSubSequenceCount+1,L"mm"))
		{
			OempaApiMessageBox(NULL,L"SubSequence/Encoder",L"File Error",MB_OK);
			{bRet = false;goto end;}
		}
	}
	if(root.iSubSequenceGateCount>0)
	{
		if(!root.rParamList->FileWriteInteger(L"SubSequence",L"Gate.count",pFileName,iSubSequenceCount+1,NULL))
			return true;
		if(!root.rParamList->FileWriteArrayFloat(L"SubSequence",L"Gate",pFileName,1.0e6f,root.afSubSequenceValue,iSubSequenceCount+1,L"us"))
		{
			OempaApiMessageBox(NULL,L"SubSequence/Gate",L"File Error",MB_OK);
			{bRet = false;goto end;}
		}
	}

	root.rParamList->FileWriteInteger(L"SubSequence",L"Average",pFileName,root.iSubSequenceAverage,L"");

end:
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////

_CUST_DRV_DEF_ bool WINAPI OEMPA_WriteFileText(	const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	bool bRet=true,bAllocParamList=false;
	wchar_t wcAux[MAX_PATH];
	pCallbackCustomizedDriverAPI pCallbackCustomizedAPI=OEMPA_GetCallbackCustomizedDriverAPI();

	if(pCallbackCustomizedAPI && pRoot && pRoot->pHWDeviceOEMPA)
		(*pCallbackCustomizedAPI)(pRoot->pHWDeviceOEMPA,pRoot->wcFileName,eWriteFile_Enter,pRoot,pCycle,pEmission,pReception);
	//DeleteFile(pFileName);
	if(!pRoot->rParamList)
	{
		pRoot->rParamList = new CRead_Write_ParametersList();
		bAllocParamList = true;
	}
	if(!pRoot->rParamList)
	{
		OempaApiMessageBox(NULL,L"pRoot->rParamList is NULL!",L"Error",MB_OK);
		return false;
	}
	if(!pRoot->rParamList->FileWriteString(L"Root",L"VersionDriverOEMPA",pFileName,OEMPA_GetVersion()))
		return false;
	if(((pRoot->ullSavedParameters & OEMPA_DISABLE_USB3) || (pRoot->bUSB3Disable==1)) && !pRoot->rParamList->FileWriteBoolean(L"Root",L"USB3Disable",pFileName,pRoot->bUSB3Disable==1?true:false))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_DEFAULT_HW_LINK) && !pRoot->rParamList->FileWriteEnum(L"Root",L"DefaultHwLink",pFileName,
		L"Unlinked"L"\0"L"Master"L"\0"L"Slave"L"\0",(int)pRoot->eDefaultHwLink))
		return false;
	if(pRoot->b256Master || pRoot->b256Slave)
		pRoot->rParamList->FileWriteString(L"Root",L"Enable256",pFileName,L"1");//1.1.4.5e
	if(pRoot->pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelSupported() || pRoot->bEnableMultiHWChannel)
	{// "|| pRoot->bEnableMultiHWChannel" is required because of the "DefaultSetupOEMMC.txt" (this file can be created at connection time with an OEMPA device.
		if(((pRoot->ullSavedParameters & OEMPA_ENABLE_MULTI_CHANNEL) || pRoot->bEnableMultiHWChannel) &&
			!pRoot->rParamList->FileWriteBoolean(L"Root",L"EnableMultiChannel",pFileName,pRoot->bEnableMultiHWChannel))
			return false;
	}
	if(pRoot->pHWDeviceOEMPA && (
			//FW FMC only (old version)
			(pRoot->pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture() &&
			!pRoot->pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite())
		||
			//FW FMC Read/Write
			(pRoot->pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite() &&
			(pRoot->bEnableFMC || (pRoot->ullSavedParameters & OEMPA_ENABLE_FMC)))
		))
	{
		if(!pRoot->rParamList->FileWriteBoolean(L"Root",L"EnableFMC",pFileName,pRoot->bEnableFMC))
			return false;
		if(pRoot->bEnableFMC)
		{
			if(!pRoot->rParamList->FileWriteInteger(L"Root",L"FMCElementStart",pFileName,pRoot->iFMCElementStart,NULL))
				return false;
			if(!pRoot->rParamList->FileWriteInteger(L"Root",L"FMCElementStop",pFileName,pRoot->iFMCElementStop,NULL))
				return false;
			if(!pRoot->rParamList->FileWriteInteger(L"Root",L"FMCElementStep",pFileName,pRoot->iFMCElementStep,NULL))
				return false;
		}
	}
	if((pRoot->ullSavedParameters & OEMPA_ASCAN_ENABLE) && !pRoot->rParamList->FileWriteBoolean(L"Root",L"AscanEnable",pFileName,pRoot->bAscanEnable))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_CSCAN_ENABLE_TOF) && !pRoot->rParamList->FileWriteBoolean(L"Root",L"EnableCscanTof",pFileName,pRoot->bEnableCscanTof))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_ASCAN_BITSIZE) && !pRoot->rParamList->FileWriteEnum(L"Root",L"AscanBitSize",pFileName,
									L"8Bits"L"\0"L"12Bits"L"\0"L"16Bits"L"\0"L"Log8Bits"L"\0",(int)pRoot->eAscanBitSize))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_TRIGGER_MODE) && !pRoot->rParamList->FileWriteEnum(L"Root",L"TriggerMode",pFileName,L"Internal"L"\0"L"Encoder"L"\0"L"ExternalCycle"L"\0"
									L"ExternalSequence"L"\0"L"ExternalCycleSequence"L"\0",(int)pRoot->eTriggerMode))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_ENCODER_TRIG_DIRECTION) && !pRoot->rParamList->FileWriteEnum(L"Root",L"EncoderDirection",pFileName,L"UpDown"L"\0"L"UpOnly"L"\0"L"DownOnly"L"\0",(int)pRoot->eEncoderDirection))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_TRIGGER_ENCODER_STEP) && !pRoot->rParamList->FileWriteDouble(L"Root",L"TriggerEncoderStep",pFileName,1.0e3,pRoot->dTriggerEncoderStep,L"mm"))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_REQUESTIO) && !pRoot->rParamList->FileWriteEnum(L"Root",L"RequestIO",pFileName,L"NotRequired"L"\0"L"OnCycleOnly"L"\0"L"OnSequenceOnly"L"\0"
									L"OnDigitalInputOnly"L"\0"L"OnDigitalInputAndCycle"L"\0"L"OnDigitalInputAndSequence"L"\0",(int)pRoot->eRequestIO))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_REQUESTIO_DIGITAL_INPUT_MASK) && !pRoot->rParamList->FileWriteLong(L"Root",L"RequestIODigitalInputMaskRising",pFileName,pRoot->iRequestIODigitalInputMaskRising,L""))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_REQUESTIO_DIGITAL_INPUT_MASK) && !pRoot->rParamList->FileWriteLong(L"Root",L"RequestIODigitalInputMaskFalling",pFileName,pRoot->iRequestIODigitalInputMaskFalling,L""))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_DEBOUNCER_ENCODER) && !pRoot->rParamList->FileWriteDouble(L"Root",L"DebouncerEncoder",pFileName,1.0e6f,pRoot->dDebouncerEncoder,MICRO_SECOND))
		bRet = false;
	if((pRoot->ullSavedParameters & OEMPA_DEBOUNCER_DIGITAL) && !pRoot->rParamList->FileWriteDouble(L"Root",L"DebouncerDigital",pFileName,1.0e6f,pRoot->dDebouncerDigital,MICRO_SECOND))
		bRet = false;
	if((pRoot->ullSavedParameters & OEMPA_DIGITAL_OUTPUT_0) && !pRoot->rParamList->FileWriteEnum(L"Root",L"DigitalOutput0",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",(int)pRoot->eDigitalOuput[0]))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_DIGITAL_OUTPUT_1) && !pRoot->rParamList->FileWriteEnum(L"Root",L"DigitalOutput1",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",(int)pRoot->eDigitalOuput[1]))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_DIGITAL_OUTPUT_2) && !pRoot->rParamList->FileWriteEnum(L"Root",L"DigitalOutput2",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",(int)pRoot->eDigitalOuput[2]))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_DIGITAL_OUTPUT_3) && !pRoot->rParamList->FileWriteEnum(L"Root",L"DigitalOutput3",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",(int)pRoot->eDigitalOuput[3]))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_DIGITAL_OUTPUT_4) && !pRoot->rParamList->FileWriteEnum(L"Root",L"DigitalOutput4",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",(int)pRoot->eDigitalOuput[4]))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_DIGITAL_OUTPUT_5) && !pRoot->rParamList->FileWriteEnum(L"Root",L"DigitalOutput5",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",(int)pRoot->eDigitalOuput[5]))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_SW_ENCODER1_RESOLUTION) && !pRoot->rParamList->FileWriteLong(L"Root",L"SWEncoder1Resolution",pFileName,pRoot->lSWEncoderResolution1,L""))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_SW_ENCODER1_DIVIDER) && !pRoot->rParamList->FileWriteLong(L"Root",L"SWEncoder1Divider",pFileName,pRoot->dwSWEncoderDivider1,L""))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_SW_ENCODER2_RESOLUTION) && !pRoot->rParamList->FileWriteLong(L"Root",L"SWEncoder2Resolution",pFileName,pRoot->lSWEncoderResolution2,L""))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_SW_ENCODER2_DIVIDER) && !pRoot->rParamList->FileWriteLong(L"Root",L"SWEncoder2Divider",pFileName,pRoot->dwSWEncoderDivider2,L""))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_ENCODER1A) && !pRoot->rParamList->FileWriteEnum(L"Root",L"Encoder1A",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",(int)pRoot->eEncoder1A))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_ENCODER1B) && !pRoot->rParamList->FileWriteEnum(L"Root",L"Encoder1B",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",(int)pRoot->eEncoder1B))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_ENCODER2A) && !pRoot->rParamList->FileWriteEnum(L"Root",L"Encoder2A",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",(int)pRoot->eEncoder2A))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_ENCODER2B) && !pRoot->rParamList->FileWriteEnum(L"Root",L"Encoder2B",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",(int)pRoot->eEncoder2B))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_EXTERNAL_TRIGGER_CYCLE) && !pRoot->rParamList->FileWriteEnum(L"Root",L"ExternalTriggerCycle",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",(int)pRoot->eExternalTriggerCycle))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_EXTERNAL_TRIGGER_SEQUENCE) && !pRoot->rParamList->FileWriteEnum(L"Root",L"ExternalTriggerSequence",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",(int)pRoot->eExternalTriggerSequence))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_KEEPALIVE) && !pRoot->rParamList->FileWriteEnum(L"Root",L"KeepAlive",pFileName,L"Off"L"\0"L"HardwareAndComputer"L"\0"L"HardwareOnly"L"\0"
								L"ComputerOnly"L"\0",(int)pRoot->eKeepAlive))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_ENCODER1_TYPE) && !pRoot->rParamList->FileWriteEnum(L"Root",L"Encoder1Type",pFileName,L"StaticScan"L"\0"L"Quadrature"L"\0"L"Quadrature4Edges"L"\0"L"DirectionCount"L"\0"
									L"ForwardBackward"L"\0",(int)pRoot->eEncoder1Type))
		return false;
	if((pRoot->ullSavedParameters & OEMPA_ENCODER2_TYPE) && !pRoot->rParamList->FileWriteEnum(L"Root",L"Encoder2Type",pFileName,L"StaticScan"L"\0"L"Quadrature"L"\0"L"Quadrature4Edges"L"\0"L"DirectionCount"L"\0"
									L"ForwardBackward"L"\0",(int)pRoot->eEncoder2Type))
		return false;
	if(pRoot->ullSavedParameters & OEMPA_ASCAN_REQUEST)
	{
		if(!pRoot->rParamList->FileWriteEnum(L"Root",L"AscanRequest",pFileName,L"AscanAll"L"\0"L"AscanSampled"L"\0"
									L"BscanOnLine"L"\0",(int)pRoot->eAscanRequest))
			return false;
		if(!pRoot->rParamList->FileWriteDouble(L"Root",L"AscanRequestFrequency",pFileName,1.0,pRoot->dAscanRequestFrequency,L"Hz"))
			return false;
	}
	if(pRoot->ullSavedParameters & OEMPA_TRIGGER_HIGH_TIME)
	{
		if(!pRoot->rParamList->FileWriteDouble(L"Root",L"TriggerHighTime",pFileName,1.0e6,pRoot->dTriggerHighTime,L"us"))
			return false;
	}

	if(!pRoot->rParamList->FileWriteInteger(L"Root",L"CycleCount",pFileName,pRoot->iCycleCount,NULL))
		return false;

	if((pRoot->ullSavedParameters & OEMPA_DISABLE_USB3) || (pRoot->bUSB3Disable==1))
	{
		if(!pRoot->rParamList->FileWriteInteger(L"Root",L"DisableUSB3",pFileName,pRoot->bUSB3Disable,NULL))
			return false;
	}
	if((pRoot->ullSavedParameters & OEMPA_TEMPERATURE_WARNING) || (pRoot->cTemperatureWarning>0))
	{
		if(!pRoot->rParamList->FileWriteInteger(L"Root",L"TemperatureWarning",pFileName,pRoot->cTemperatureWarning,NULL))
			return false;
	}
	if((pRoot->ullSavedParameters & OEMPA_TEMPERATURE_ALARM) || (pRoot->cTemperatureWarning>0))
	{
		if(!pRoot->rParamList->FileWriteInteger(L"Root",L"TemperatureAlarm",pFileName,pRoot->cTemperatureAlarm,NULL))
			return false;
	}
	if((pRoot->ullSavedParameters & OEMPA_REFERENCE_TIME_OF_FLIGHT) || (pRoot->eReferenceTimeOfFlight!=ePulse))
	{
		switch(pRoot->eReferenceTimeOfFlight)
		{
		case ePulse:
			if(!pRoot->rParamList->FileWriteString(L"Root",L"ReferenceTimeOfFlight",pFileName,L"Pulse"))
				return false;
			break;
		case eInterface:
			if(!pRoot->rParamList->FileWriteString(L"Root",L"ReferenceTimeOfFlight",pFileName,L"Interface"))
				return false;
			break;
		}
	}

	if(pRoot->uiSavedFilterCount && !pRoot->rParamList->FileWriteInteger(L"Root",L"FilterCount",pFileName,(int)pRoot->uiSavedFilterCount,NULL))
		return false;

	for(int iFilterIndex=0;iFilterIndex<(int)pRoot->uiSavedFilterCount;iFilterIndex++)
	{
		if(!WriteFileTextFilter(iFilterIndex,pFileName,pRoot))
		{
			swprintf(wcAux,MAX_PATH,L"Filter %d",iFilterIndex);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;
		}
	}

	if(!ReadHWWriteTextSubSequence(pFileName,*pRoot))
		bRet = false;

	//if(bRet)
	//	NewLine(pFileName);

	for(int iCycle=0;iCycle<pRoot->iCycleCount;iCycle++)
	{
		if(!WriteFileTextCycle(iCycle,pFileName,pRoot,pCycle))
		{
			swprintf(wcAux,MAX_PATH,L"Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;
		}
		if(!WriteFileTextFocalLaw(iCycle,true,pCycle[iCycle].bHWAcquisition,pFileName,pRoot,&pEmission[iCycle]))
		{
			swprintf(wcAux,MAX_PATH,L"Emission Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;
		}
		if(!WriteFileTextFocalLaw(iCycle,false,pCycle[iCycle].bHWAcquisition,pFileName,pRoot,&pReception[iCycle]))
		{
			swprintf(wcAux,MAX_PATH,L"Reception Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;
		}
	}
	if(pCallbackCustomizedAPI && pRoot && pRoot->pHWDeviceOEMPA)
		(*pCallbackCustomizedAPI)(pRoot->pHWDeviceOEMPA,pRoot->wcFileName,eWriteFile_Leave,pRoot,pCycle,pEmission,pReception);

	if(pRoot->rParamList->fastWriting)//GP
		pRoot->rParamList->ReadListWriteFile(pFileName);//GP

	if(bRet)
		FillNewLine(pFileName);

	if(bAllocParamList && pRoot->rParamList)
	{
		delete pRoot->rParamList;
		pRoot->rParamList = NULL;
	}
	return bRet;
}

_CUST_DRV_DEF_ bool WINAPI OEMPA_WriteFileBinary(	const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	bool bRet=true;
	DWORD dwData;
	pCallbackCustomizedDriverAPI pCallbackCustomizedAPI=OEMPA_GetCallbackCustomizedDriverAPI();

	if(pCallbackCustomizedAPI && pRoot && pRoot->pHWDeviceOEMPA)
		(*pCallbackCustomizedAPI)(pRoot->pHWDeviceOEMPA,pRoot->wcFileName,eWriteFile_Enter,pRoot,pCycle,pEmission,pReception);
#ifdef _USE_MFC_
	try{
		CFile file(pFileName,CFile::typeBinary | CFile::modeCreate | CFile::modeWrite);
#else _USE_MFC_
		myCFile file(pFileName,myCFile::typeBinary | myCFile::modeCreate | myCFile::modeWrite);
#endif _USE_MFC_
		//root
		dwData = 1;//one item (one root)
		file.Write(&dwData,sizeof(DWORD));
		dwData = sizeof(structRoot);//size of this item.
		file.Write(&dwData,sizeof(DWORD));
		file.Write(pRoot,sizeof(structRoot));
		//cycles
		dwData = pRoot->iCycleCount;//N item.
		file.Write(&dwData,sizeof(DWORD));
		dwData = sizeof(structCycle);//size of those items.
		file.Write(&dwData,sizeof(DWORD));
		file.Write(pCycle,pRoot->iCycleCount*sizeof(structCycle));
		//emission focal laws
		dwData = pRoot->iCycleCount;//N item.
		file.Write(&dwData,sizeof(DWORD));
		//dwData = sizeof(CFocalLaw);//size of those items.
		//file.Write(&dwData,sizeof(DWORD));
		//file.Write(pEmission,pRoot->iCycleCount*sizeof(CFocalLaw));
		for(int iCycleIndex=0;iCycleIndex<iCycleIndex;iCycleIndex++)
		{
			if(!pEmission[iCycleIndex].Write(file))
				bRet = false;
		}
		//reception focal laws
		dwData = pRoot->iCycleCount;//N item.
		file.Write(&dwData,sizeof(DWORD));
		//dwData = sizeof(CFocalLaw);//size of those items.
		//file.Write(&dwData,sizeof(DWORD));
		//file.Write(pReception,pRoot->iCycleCount*sizeof(CFocalLaw));
		for(int iCycleIndex=0;iCycleIndex<iCycleIndex;iCycleIndex++)
		{
			if(!pReception[iCycleIndex].Write(file))
				bRet = false;
		}
		file.Close();
#ifdef _USE_MFC_
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
#endif _USE_MFC_
	if(pCallbackCustomizedAPI && pRoot && pRoot->pHWDeviceOEMPA)
		(*pCallbackCustomizedAPI)(pRoot->pHWDeviceOEMPA,pRoot->wcFileName,eWriteFile_Leave,pRoot,pCycle,pEmission,pReception);
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
_CUST_DRV_DEF_ bool WINAPI OEMPA_ReadWizardWriteFile(	CHWDeviceOEMPA *pHWDeviceOEMPA,const wchar_t *pFileName,int iCycleCount,double dTimeSlot,int iSameAllCycles,
						int iEmissionElementCount,double dEmissionWedgeDelay,float fPulseWidth,
						int iReceptionElementCount,int iReceptionFocalCount,double dReceptionWedgeDelay,
						double dAscanStart,double dAscanRange,int iDACCount,
						int iGateCount,double dGateStart,double dGateStop,double dGateThreshold)
{
	bool bRet=true;
	structRoot root;
	structCycle *pCycle=NULL,*pCycleAux;
	CFocalLaw *pEmission=NULL;
	CFocalLaw *pReception=NULL;
	CFocalLaw *pFocalLaw;
	bool bTextFile;

	OEMPA_InitRoot(root,pHWDeviceOEMPA);
	if(!IsValidFileExtension(pFileName,bTextFile))
	{
		OempaApiMessageBox(NULL,L"Bad file extension!",L"File Error",MB_OK);
		return false;
	}

	root.ullSavedParameters = 0;//with the example those parameters are not significant.

	root.iCycleCount = iCycleCount;

	pCycle = OEMPA_AllocCycle(root.iCycleCount);
	pEmission = new CFocalLaw[root.iCycleCount];
	pReception = new CFocalLaw[root.iCycleCount];
	if(!pCycle || !pEmission || !pReception)
	{
		bRet = false;goto end;
	}
	OEMPA_ResetArrayFocalLaw(root.iCycleCount,pEmission);
	OEMPA_ResetArrayFocalLaw(root.iCycleCount,pReception);

	if(iSameAllCycles)
	{
		pCycleAux = &pCycle[0];

		pCycleAux->dGainDigital = 10.0;
		pHWDeviceOEMPA->CheckGainDigital(pCycleAux->dGainDigital);
		pCycleAux->fBeamCorrection = 0.0f;
		pHWDeviceOEMPA->CheckBeamCorrection(pCycleAux->fBeamCorrection);
		pCycleAux->dStart = dAscanStart;
		pHWDeviceOEMPA->CheckAscanStart(pCycleAux->dStart);
		pCycleAux->dRange = dAscanRange;
		pHWDeviceOEMPA->CheckAscanRange(pCycleAux->dRange);
		pCycleAux->dTimeSlot = dTimeSlot;
		pHWDeviceOEMPA->CheckTimeSlot(pCycleAux->dTimeSlot);
		pCycleAux->lPointCount = 0;//myround(dAscanRange/pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod())+1;
		pCycleAux->lPointFactor = 0;
		pCycleAux->eCompressionType = eCompression;
		pHWDeviceOEMPA->CheckAscanRangeWithCount(pCycleAux->dRange,pCycleAux->eCompressionType,pCycleAux->lPointCount);
		pCycleAux->eRectification = eSigned;
		pCycleAux->bDACAutoStop = false;
		pCycleAux->bDACSlope = true;
		pCycleAux->iDACCount = iDACCount;
		for(int iDACIndex=0;iDACIndex<iDACCount;iDACIndex++)
		{
			pCycleAux->adDACTof[iDACIndex] = pCycleAux->dStart+10.0*iDACIndex*pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod();//unit = second
			pCycleAux->afDACPrm[iDACIndex] = iDACIndex*g_fOEMPADACSlopeStep/pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod();//unit = dB/second
		}
		pCycleAux->bDACEnable = false;
		pCycleAux->bMaximum = true;
		pCycleAux->bMinimum = false;
		pCycleAux->bSaturation = false;
		pCycleAux->wAcqIdChannelProbe = 0xffff;
		pCycleAux->wAcqIdChannelScan = 0xffff;
		pCycleAux->wAcqIdChannelCycle = 0xffff;
		pCycleAux->fGainAnalog = 0.0f;//float in the future//unit = dB
		pCycleAux->iFilterIndex = 1;
		pCycleAux->bTrackingAscanEnable = false;
		pCycleAux->bTrackingDacEnable = false;
		pCycleAux->iGateCount = iGateCount;
		for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
		{
			if(iGate<iGateCount)
				pCycleAux->gate[iGate].bEnable = true;
			else
				pCycleAux->gate[iGate].bEnable = false;
			pCycleAux->gate[iGate].dStart = dGateStart+iGate*0.1e-6;
			pCycleAux->gate[iGate].dStop = dGateStop+iGate*0.1e-6;
			pCycleAux->gate[iGate].dThreshold = dGateThreshold;
			pCycleAux->gate[iGate].eRectification = eSigned;
			pCycleAux->gate[iGate].eModeAmp = eAmpMaximum;
			pCycleAux->gate[iGate].eModeTof = eTofAmplitudeDetection;
			pCycleAux->gate[iGate].wAcqIDAmp = 0xffff;
			pCycleAux->gate[iGate].wAcqIDTof = 0xffff;
			pCycleAux->gate[iGate].wAcqIDTof = 0xffff;
			pCycleAux->gate[iGate].bTrackingStartEnable = false;
			pCycleAux->gate[iGate].bTrackingStopEnable = false;
			pHWDeviceOEMPA->CheckGateStart(pCycleAux->gate[iGate].dStart);
			pHWDeviceOEMPA->CheckGateStop(pCycleAux->gate[iGate].dStop);
			pHWDeviceOEMPA->CheckGateModeThreshold(pCycleAux->gate[iGate].bEnable,pCycleAux->gate[iGate].eModeAmp,pCycleAux->gate[iGate].eModeTof,pCycleAux->gate[iGate].eRectification,pCycleAux->gate[iGate].dThreshold);
		}

		pFocalLaw = &pEmission[0];
		if(!pFocalLaw->GetDWORDElementSize())
			pFocalLaw->SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
		pFocalLaw->eFocusing = eStandard;
		pFocalLaw->dWedgeDelay = dEmissionWedgeDelay;
		pFocalLaw->dAngle = 0.0;
		pFocalLaw->dX = 0.0;
		pFocalLaw->dY = 0.0;
		pFocalLaw->dZ = 0.0;
		pHWDeviceOEMPA->CheckWedgeDelay(pFocalLaw->dWedgeDelay);
		pFocalLaw->iElementCount = iEmissionElementCount;
		pFocalLaw->iPrmCount = iEmissionElementCount;//width
		pFocalLaw->iFocalCount = 0;
		pFocalLaw->iDelayCount = pFocalLaw->iElementCount;//*pFocalLaw->iFocalCount;
		if(!pHWDeviceOEMPA->SetAllElementEnable(false,pFocalLaw->adwElement))
			{bRet = false;goto end;}
		for(int iElem=0;iElem<iEmissionElementCount;iElem++)
		{
			pHWDeviceOEMPA->SetElementEnable(iElem,true,pFocalLaw->adwElement);
			/*following code is the same 
			if(iElem>=32)
			{
				dwElem = 1<<(iElem-32);
				pFocalLaw->adwElement[0] |= dwElem;
			}else{
				dwElem = 1<<iElem;
				pFocalLaw->adwElement[1] |= dwElem;
			}*/
			pFocalLaw->afDelay[0][iElem] = (float)iElem*pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod();
			pHWDeviceOEMPA->CheckEmissionDelay(pFocalLaw->afDelay[0][iElem]);
			pFocalLaw->afPrm[iElem] = fPulseWidth;
			pHWDeviceOEMPA->CheckEmissionWidth(pFocalLaw->afPrm[iElem]);
		}

		pFocalLaw = &pReception[0];
		if(!pFocalLaw->GetDWORDElementSize())
			pFocalLaw->SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
		if(iReceptionFocalCount>1)
			pFocalLaw->eFocusing = eDDF;
		else
			pFocalLaw->eFocusing = eStandard;
		pFocalLaw->dWedgeDelay = dReceptionWedgeDelay;
		pFocalLaw->dAngle = 0.0;
		pFocalLaw->dX = 0.0;
		pFocalLaw->dY = 0.0;
		pFocalLaw->dZ = 0.0;
		pHWDeviceOEMPA->CheckWedgeDelay(pFocalLaw->dWedgeDelay);
		pFocalLaw->iElementCount = iReceptionElementCount;
		pFocalLaw->iPrmCount = iReceptionElementCount;//gain
		pFocalLaw->iFocalCount = iReceptionFocalCount;
		pFocalLaw->iDelayCount = pFocalLaw->iElementCount*pFocalLaw->iFocalCount;
		if(pHWDeviceOEMPA->SetAllElementEnable(false,pFocalLaw->adwElement))
			{bRet = false;goto end;}
		if(!pFocalLaw->iElementCount)
		{
			NotifyErrorFocalLaw(0);
			bRet = false;goto end;
		}
		for(int iElem=0;iElem<iEmissionElementCount;iElem++)
		{
			pHWDeviceOEMPA->SetElementEnable(iElem,true,pFocalLaw->adwElement);
			/*following code is the same 
			if(iElem>=32)
			{
				dwElem = 1<<(iElem-32);
				pFocalLaw->adwElement[0] |= dwElem;
			}else{
				dwElem = 1<<iElem;
				pFocalLaw->adwElement[1] |= dwElem;
			}*/
			for(int iFocalIndex=0;iFocalIndex<pFocalLaw->iFocalCount;iFocalIndex++)
			{
				pFocalLaw->afDelay[iFocalIndex][iElem] = (float)iElem*pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod()+(float)iFocalIndex*1.0e-6f;
				pHWDeviceOEMPA->CheckReceptionDelay(pFocalLaw->afDelay[iFocalIndex][iElem]);
				if(!iElem)
				{
					pFocalLaw->adFocalTimeOfFlight[iFocalIndex] = (double)(iFocalIndex+1)*1.0e-6f;
					pHWDeviceOEMPA->CheckFocalTimeOfFlight(pFocalLaw->adFocalTimeOfFlight[iFocalIndex]);
				}
			}
			pFocalLaw->afPrm[iElem] = 0.0f;
			pHWDeviceOEMPA->CheckReceptionGain(pFocalLaw->afPrm[iElem]);
		}
		for(int iCycle=1;iCycle<root.iCycleCount;iCycle++)
		{
			pCycleAux = &pCycle[iCycle];
			memcpy(pCycleAux,&pCycle[0],sizeof(structCycle));
			pFocalLaw = &pEmission[iCycle];
			*pFocalLaw = pEmission[0];
			pFocalLaw = &pReception[iCycle];
			*pFocalLaw = pReception[0];
		}
	}else{
		for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
		{
			pCycleAux = &pCycle[iCycle];

			pCycleAux->dGainDigital = 10.0+iCycle/10.0;
			pHWDeviceOEMPA->CheckGainDigital(pCycleAux->dGainDigital);
			pCycleAux->fBeamCorrection = iCycle/10.0f;
			pHWDeviceOEMPA->CheckBeamCorrection(pCycleAux->fBeamCorrection);
			pCycleAux->dStart = dAscanStart+iCycle*10.0e-6/root.iCycleCount;
			pHWDeviceOEMPA->CheckAscanStart(pCycleAux->dStart);
			pCycleAux->dRange = dAscanRange+iCycle*10.0e-6/root.iCycleCount;
			pHWDeviceOEMPA->CheckAscanRange(pCycleAux->dRange);
			pCycleAux->dTimeSlot = 100.0e-6;
			pHWDeviceOEMPA->CheckTimeSlot(pCycleAux->dTimeSlot);
			pCycleAux->lPointCount = 0;//myround(dAscanRange/pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod())+1;
			pCycleAux->lPointFactor = 0;
			pCycleAux->eCompressionType = eCompression;
			pHWDeviceOEMPA->CheckAscanRangeWithCount(pCycleAux->dRange,pCycleAux->eCompressionType,pCycleAux->lPointCount);
			pCycleAux->eRectification = eSigned;
			pCycleAux->bDACAutoStop = false;
			pCycleAux->bDACSlope = true;
			pCycleAux->iDACCount = iDACCount;
			for(int iDACIndex=0;iDACIndex<iDACCount;iDACIndex++)
			{
				pCycleAux->adDACTof[iDACIndex] = pCycleAux->dStart+10.0*iDACIndex*pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod();//unit = second
				pCycleAux->afDACPrm[iDACIndex] = iDACIndex*g_fOEMPADACSlopeStep/pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod();//unit = dB/second
			}
			pCycleAux->bDACEnable = false;
			pCycleAux->bMaximum = true;
			pCycleAux->bMinimum = false;
			pCycleAux->bSaturation = false;
			pCycleAux->wAcqIdChannelProbe = 0xffff;
			pCycleAux->wAcqIdChannelScan = 0xffff;
			pCycleAux->wAcqIdChannelCycle = 0xffff;
			pCycleAux->fGainAnalog = 0.0f;//float in the future//unit = dB
			pCycleAux->iFilterIndex = 1;
			pCycleAux->bTrackingAscanEnable = false;
			pCycleAux->bTrackingDacEnable = false;
			pCycleAux->iGateCount = iGateCount;
			for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
			{
				if(iGate<iGateCount)
					pCycleAux->gate[iGate].bEnable = true;
				else
					pCycleAux->gate[iGate].bEnable = false;
				pCycleAux->gate[iGate].dStart = dGateStart+iGate*0.1e-6+iCycle*10.0e-6/root.iCycleCount;
				pCycleAux->gate[iGate].dStop = dGateStop+iGate*0.1e-6+iCycle*10.0e-6/root.iCycleCount;
				pCycleAux->gate[iGate].dThreshold = dGateThreshold;
				pCycleAux->gate[iGate].eRectification = eSigned;
				pCycleAux->gate[iGate].eModeAmp = eAmpMaximum;
				pCycleAux->gate[iGate].eModeTof = eTofAmplitudeDetection;
				pCycleAux->gate[iGate].wAcqIDAmp = 0xffff;
				pCycleAux->gate[iGate].wAcqIDTof = 0xffff;
				pCycleAux->gate[iGate].bTrackingStartEnable = false;
				pCycleAux->gate[iGate].bTrackingStopEnable = false;
				pHWDeviceOEMPA->CheckGateStart(pCycleAux->gate[iGate].dStart);
				pHWDeviceOEMPA->CheckGateStop(pCycleAux->gate[iGate].dStop);
				pHWDeviceOEMPA->CheckGateModeThreshold(pCycleAux->gate[iGate].bEnable,pCycleAux->gate[iGate].eModeAmp,pCycleAux->gate[iGate].eModeTof,pCycleAux->gate[iGate].eRectification,pCycleAux->gate[iGate].dThreshold);
			}

			pFocalLaw = &pEmission[iCycle];
			if(!pFocalLaw->GetDWORDElementSize())
				pFocalLaw->SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
			pFocalLaw->eFocusing = eStandard;
			pFocalLaw->dWedgeDelay = dEmissionWedgeDelay+iCycle*100.0e-6/root.iCycleCount;
			pHWDeviceOEMPA->CheckWedgeDelay(pFocalLaw->dWedgeDelay);
			pFocalLaw->iElementCount = iEmissionElementCount;
			pFocalLaw->iPrmCount = iEmissionElementCount;//width
			pFocalLaw->iFocalCount = 0;
			pFocalLaw->iDelayCount = pFocalLaw->iElementCount;//*pFocalLaw->iFocalCount;
			if(!pHWDeviceOEMPA->SetAllElementEnable(false,pFocalLaw->adwElement))
				{bRet = false;goto end;}
			for(int iElem=0;iElem<iEmissionElementCount;iElem++)
			{
				pHWDeviceOEMPA->SetElementEnable(iElem,true,pFocalLaw->adwElement);
				/*following code is the same 
				if(iElem>=32)
				{
					dwElem = 1<<(iElem-32);
					pFocalLaw->adwElement[0] |= dwElem;
				}else{
					dwElem = 1<<iElem;
					pFocalLaw->adwElement[1] |= dwElem;
				}*/
				pFocalLaw->afDelay[0][iElem] = (float)iElem*pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod()+(float)iCycle*10.0e-6f/(float)root.iCycleCount;
				pHWDeviceOEMPA->CheckEmissionDelay(pFocalLaw->afDelay[0][iElem]);
				pFocalLaw->afPrm[iElem] = 100.0e-9f+(float)iElem*pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod()+(float)iCycle*2.5e-6f/(float)root.iCycleCount;//width : 100 ns
				pHWDeviceOEMPA->CheckEmissionWidth(pFocalLaw->afPrm[iElem]);
			}

			pFocalLaw = &pReception[iCycle];
			if(!pFocalLaw->GetDWORDElementSize())
				pFocalLaw->SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
			if(iReceptionFocalCount>1)
				pFocalLaw->eFocusing = eDDF;
			else
				pFocalLaw->eFocusing = eStandard;
			pFocalLaw->dWedgeDelay = dReceptionWedgeDelay+iCycle*100.0e-6/root.iCycleCount;
			pHWDeviceOEMPA->CheckWedgeDelay(pFocalLaw->dWedgeDelay);
			pFocalLaw->iElementCount = iReceptionElementCount;
			pFocalLaw->iPrmCount = iReceptionElementCount;//gain
			pFocalLaw->iFocalCount = iReceptionFocalCount;
			pFocalLaw->iDelayCount = pFocalLaw->iElementCount*pFocalLaw->iFocalCount;
			if(!pHWDeviceOEMPA->SetAllElementEnable(false,pFocalLaw->adwElement))
				{bRet = false;goto end;}
			if(!pFocalLaw->iElementCount)
			{
				NotifyErrorFocalLaw(iCycle);
				bRet = false;goto end;
			}
			for(int iElem=0;iElem<iEmissionElementCount;iElem++)
			{
				pHWDeviceOEMPA->SetElementEnable(iElem,true,pFocalLaw->adwElement);
				/*following code is the same 
				if(iElem>=32)
				{
					dwElem = 1<<(iElem-32);
					pFocalLaw->adwElement[0] |= dwElem;
				}else{
					dwElem = 1<<iElem;
					pFocalLaw->adwElement[1] |= dwElem;
				}*/
				for(int iFocalIndex=0;iFocalIndex<pFocalLaw->iFocalCount;iFocalIndex++)
				{
					pFocalLaw->afDelay[iFocalIndex][iElem] = (float)iElem*pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod()+(float)iFocalIndex*1.0e-6f+(float)iCycle*10.0e-6f/(float)root.iCycleCount;
					pHWDeviceOEMPA->CheckReceptionDelay(pFocalLaw->afDelay[iFocalIndex][iElem]);
					if(!iElem)
					{
						pFocalLaw->adFocalTimeOfFlight[iFocalIndex] = (double)(iFocalIndex+1)*1.0e-6f+(double)iCycle*10.0e-6f/(double)root.iCycleCount;
						pHWDeviceOEMPA->CheckFocalTimeOfFlight(pFocalLaw->adFocalTimeOfFlight[iFocalIndex]);
					}
				}
				pFocalLaw->afPrm[iElem] = (float)iElem/10.0f+(float)iCycle*10.0f/(float)root.iCycleCount;//gain (dB)
				pHWDeviceOEMPA->CheckReceptionGain(pFocalLaw->afPrm[iElem]);
			}
		}
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

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
bool WINAPI CallbackSetData1(structCallbackArrayFloat1D *pCallbackArray,int iIndex1,float fData);
bool WINAPI CallbackSetSize1(structCallbackArrayFloat1D *pCallbackArray,int iSize1)
{
	CFocalLaw *pFocalLaw=(CFocalLaw*)pCallbackArray->apParameter[0];
	structRoot *pRoot=(structRoot*)pCallbackArray->apParameter[1];

	pFocalLaw->iFocalCount = 1;
	pFocalLaw->iDelayCount = iSize1;
	if(pRoot && pRoot->bEnableMultiHWChannel)
	{//OEMMC 1.1.5.2b
		for(int iIndex=0;iIndex<iSize1;iIndex++)
			CallbackSetData1(pCallbackArray,iIndex,-1.0f);
	}
	return true;
}

bool WINAPI CallbackGetSize1(structCallbackArrayFloat1D *pCallbackArray,int &iSize1)
{
	CFocalLaw *pFocalLaw=(CFocalLaw*)pCallbackArray->apParameter[0];

	iSize1 = pFocalLaw->iDelayCount;
	return true;
}

bool WINAPI CallbackSetData1(structCallbackArrayFloat1D *pCallbackArray,int iIndex1,float fData)
{
	CFocalLaw *pFocalLaw=(CFocalLaw*)pCallbackArray->apParameter[0];

	if(iIndex1<0)
		return false;
	if(iIndex1>=pFocalLaw->iDelayCount)
		return false;
	pFocalLaw->afDelay[0][iIndex1] = fData;
	return true;
}

bool WINAPI CallbackGetData1(structCallbackArrayFloat1D *pCallbackArray,int iIndex1,float &fData)
{
	CFocalLaw *pFocalLaw=(CFocalLaw*)pCallbackArray->apParameter[0];

	if(iIndex1<0)
		return false;
	if(iIndex1>=pFocalLaw->iElementCount)
		return false;
	fData = pFocalLaw->afDelay[0][iIndex1];
	return true;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
bool WINAPI CallbackSetSize2(structCallbackArrayFloat2D *pCallbackArray,int iSize1,int iSize2)
{
	CFocalLaw *pFocalLaw=(CFocalLaw*)pCallbackArray->apParameter[0];

	pFocalLaw->iElementCount = iSize1;
	pFocalLaw->iFocalCount = iSize2;
	pFocalLaw->iDelayCount = iSize1*iSize2;
	return true;
}

bool WINAPI CallbackGetSize2(structCallbackArrayFloat2D *pCallbackArray,int &iSize1,int &iSize2)
{
	CFocalLaw *pFocalLaw=(CFocalLaw*)pCallbackArray->apParameter[0];

	iSize1 = pFocalLaw->iElementCount;
	iSize2 = pFocalLaw->iFocalCount;
	return true;
}

bool WINAPI CallbackSetData2(structCallbackArrayFloat2D *pCallbackArray,int iIndex1,int iIndex2,float fData)
{
	CFocalLaw *pFocalLaw=(CFocalLaw*)pCallbackArray->apParameter[0];

	if(iIndex1<0)
		return false;
	if(iIndex1>=pFocalLaw->iElementCount)
		return false;
	if(iIndex2<0)
		return false;
	if(iIndex2>=pFocalLaw->iFocalCount)
		return false;
	pFocalLaw->afDelay[iIndex2][iIndex1] = fData;
	return true;
}

bool WINAPI CallbackGetData2(structCallbackArrayFloat2D *pCallbackArray,int iIndex1,int iIndex2,float &fData)
{
	CFocalLaw *pFocalLaw=(CFocalLaw*)pCallbackArray->apParameter[0];

	if(iIndex1<0)
		return false;
	if(iIndex1>=pFocalLaw->iElementCount)
		return false;
	if(iIndex2<0)
		return false;
	if(iIndex2>=pFocalLaw->iFocalCount)
		return false;
	fData = pFocalLaw->afDelay[iIndex2][iIndex1];
	return true;
}

/////////////////////////////////////////////////////////////////////////////

bool ReadHWWriteFileCycleGate(CHWDeviceOEMPA *pOEMPA,int iCycle,int iGate,structRoot *pRoot,structCycle *pCycle)
{
	structGate *pGate=&pCycle->gate[iGate];
	bool bRet=true;

	pGate->bEnable = false;
	pGate->eModeAmp = (enumGateModeAmp)0;
	pGate->eModeTof = (enumGateModeTof)0;
	pGate->eRectification = (enumRectification)0;
	pGate->dThreshold = 0.0;
	if(!pOEMPA->GetGateModeThreshold(iCycle,iGate,&pGate->bEnable,&pGate->eModeAmp,&pGate->eModeTof,&pGate->eRectification,&pGate->dThreshold))
		return false;

	pGate->dStart = 0.0;
	if(!pOEMPA->GetGateStart(iCycle,iGate,&pGate->dStart))
		return false;
	pGate->dStop = 0;
	if(!pOEMPA->GetGateStop(iCycle,iGate,&pGate->dStop))
		return false;
	pGate->wAcqIDAmp = 0xffff;
	if(!pOEMPA->GetGateAcqIDAmp(iCycle,iGate,&pGate->wAcqIDAmp))
		return false;
	pGate->wAcqIDTof = 0xffff;
	if(!pOEMPA->GetGateAcqIDTof(iCycle,iGate,&pGate->wAcqIDTof))
		return false;

	if(!pOEMPA->GetTrackingGateStart(iCycle,iGate,&pGate->bTrackingStartEnable,&pGate->iTrackingStartIndexCycle,&pGate->iTrackingStartIndexGate))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetTrackingGateStart",L"HW error",MB_OK);
		bRet = false;
	}
	if(!pOEMPA->GetTrackingGateStop(iCycle,iGate,&pGate->bTrackingStopEnable,&pGate->iTrackingStopIndexCycle,&pGate->iTrackingStopIndexGate))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetTrackingGateStop",L"HW error",MB_OK);
		bRet = false;
	}

	return bRet;
}

_CUST_DRV_DEF_ bool WINAPI OEMPA_ReadHWWriteFileCycle(CHWDeviceOEMPA *pOEMPA,int iCycle,int iDACCountMax,structRoot *pRoot,structCycle *pCycle)
{
	bool bRet=true;

	pCycle->dGainDigital = 0.0;
	if(!pOEMPA->GetGainDigital(iCycle,&pCycle->dGainDigital))
		return false;
	pCycle->fBeamCorrection = 0.0f;
	if(!pOEMPA->GetBeamCorrection(iCycle,&pCycle->fBeamCorrection))
		return false;
	pCycle->iFilterIndex = 1;
	if(!pOEMPA->GetFilterIndex(iCycle,(enumOEMPAFilterIndex*)&pCycle->iFilterIndex))
		return false;
	pCycle->fGainAnalog = 0.0f;
	if(!pOEMPA->GetGainAnalog(iCycle,&pCycle->fGainAnalog))
		return false;
	
	if(!pRoot->bEnableMultiHWChannel)
	{
		if(!pOEMPA->GetAscanStart(iCycle,&pCycle->dStart))
			return false;
	}else{
		pCycle->dStart = 0.0;
		if(!pOEMPA->GetEnableMultiHWChannelAcquisition(iCycle,&pCycle->bHWAcquisition))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetEnableMultiHWChannelAcquisition",L"HW error",MB_OK);
			return false;
		}
	}
	pCycle->dRange = 0.0;
	pCycle->eCompressionType = (enumCompressionType)0;
	pCycle->lPointCount = 0;
	pCycle->lPointFactor = 0;
	if(!pOEMPA->GetAscanRange(iCycle,&pCycle->dRange,&pCycle->eCompressionType,&pCycle->lPointCount,&pCycle->lPointFactor))
		return false;
	pCycle->dTimeSlot = 0.0;
	if(!pOEMPA->GetTimeSlot(iCycle,&pCycle->dTimeSlot))
		return false;
	if (pOEMPA->GetSWDeviceOEMPA()->IsFMCSubTimeSlotPerCycleSupported())
	{
		pCycle->dFMCSubTimeSlotAcq = -1.0;
		if(!pOEMPA->GetFMCSubTimeSlotAcq(iCycle,&pCycle->dFMCSubTimeSlotAcq))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetFMCSubTimeSlotAcq",L"HW error",MB_OK);
			return false;
		}
		pCycle->dFMCSubTimeSlotReplay = -1.0;
		if(!pOEMPA->GetFMCSubTimeSlotReplay(iCycle,&pCycle->dFMCSubTimeSlotReplay))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetFMCSubTimeSlotReplay",L"HW error",MB_OK);
			return false;
		}
	}
	pCycle->eRectification = (enumRectification)0;
	if(!pOEMPA->GetAscanRectification(iCycle,&pCycle->eRectification))
		return false;
	
	pCycle->iDACCount = 0;
	if(pRoot->ullSavedParameters & (OEMPA_CYCLE_DAC_SLOPE | OEMPA_CYCLE_DAC_GAIN))
	{
		if(pRoot->ullSavedParameters & OEMPA_CYCLE_DAC_SLOPE)
			pCycle->bDACSlope = true;
		else
			pCycle->bDACSlope = false;
	}
	if(pCycle->bDACSlope)
	{
		if(!pOEMPA->GetDACSlope(iCycle,iDACCountMax,&pCycle->iDACCount,pCycle->adDACTof,pCycle->afDACPrm))
			return false;
	}else{
		if(!pOEMPA->GetDACGain(iCycle,iDACCountMax,&pCycle->iDACCount,pCycle->adDACTof,pCycle->afDACPrm))
			return false;
	}
	if(!pOEMPA->GetEnableDAC(iCycle,&pCycle->bDACEnable))
		return false;
	
	pCycle->bMaximum = false;
	if(!pOEMPA->GetEnableAscanMaximum(iCycle,&pCycle->bMaximum))
		return false;
	pCycle->bMinimum = false;
	if(!pOEMPA->GetEnableAscanMinimum(iCycle,&pCycle->bMinimum))
		return false;
	pCycle->bSaturation = false;
	if(!pOEMPA->GetEnableAscanSaturation(iCycle,&pCycle->bSaturation))
		return false;

	pCycle->wAcqIdChannelProbe = 0xffff;
	if(!pOEMPA->GetAscanAcqIdChannelProbe(iCycle,&pCycle->wAcqIdChannelProbe))
		return false;
	pCycle->wAcqIdChannelScan = 0xffff;
	if(!pOEMPA->GetAscanAcqIdChannelScan(iCycle,&pCycle->wAcqIdChannelScan))
		return false;
	pCycle->wAcqIdChannelCycle = 0xffff;
	if(!pOEMPA->GetAscanAcqIdChannelCycle(iCycle,&pCycle->wAcqIdChannelCycle))
		return false;

	if(!pOEMPA->GetTrackingAscan(iCycle,&pCycle->bTrackingAscanEnable,&pCycle->iTrackingAscanIndexCycle,&pCycle->iTrackingAscanIndexGate))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetTrackingAscan",L"HW error",MB_OK);
		return false;
	}
	if(!pOEMPA->GetTrackingDac(iCycle,&pCycle->bTrackingDacEnable,&pCycle->iTrackingDacIndexCycle,&pCycle->iTrackingDacIndexGate))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetTrackingDac",L"HW error",MB_OK);
		return false;
	}

	for(int iGateIndex=0;iGateIndex<g_iOEMPAGateCountMax;iGateIndex++)
	{
		pCycle->iGateCount = 0;
		if(!ReadHWWriteFileCycleGate(pOEMPA,iCycle,iGateIndex,pRoot,pCycle))
			return false;
	}

	return true;
}

bool ReadHWWriteFileMultiHWChannel(CHWDeviceOEMPA *pOEMPA,int iCycle,bool bPulse,int iCycleCountMax,int iElementCountMax,structRoot *pRoot,CFocalLaw *pFocalLaw);
bool ReadHWWriteFileFocalLaw(CHWDeviceOEMPA *pOEMPA,int iCycle,bool bPulse,int iCycleCountMax,int iElementCountMax,int iFocalCountMax,structRoot *pRoot,CFocalLaw *pFocalLaw,CFocalLaw *pFocalLaw2)
{
	bool bRet=true;
	structCallbackArrayFloat2D callbackArrayFloat2D;

	if(pRoot->bEnableMultiHWChannel)
		return ReadHWWriteFileMultiHWChannel(pOEMPA,iCycle,bPulse,iCycleCountMax,iElementCountMax,pRoot,pFocalLaw);

	if(bPulse)
	{
		pFocalLaw->eFocusing = eStandard;
		iFocalCountMax = 1;
		pFocalLaw->dWedgeDelay = 0.0;
		if(!pOEMPA->GetEmissionWedgeDelay(iCycle,iCycleCountMax,&pFocalLaw->dWedgeDelay))
			return false;
		pFocalLaw->iPrmCount = 0;
		if(!pOEMPA->GetEmissionWidths(iCycle,iElementCountMax,&pFocalLaw->iPrmCount,pFocalLaw->afPrm))
			return false;
		pFocalLaw->iElementCount = 0;
		if(!pOEMPA->GetEmissionDelays(iCycle,iElementCountMax,&pFocalLaw->iElementCount,&pFocalLaw->afDelay[0][0]))
			return false;
		pFocalLaw->adwElement[0]=pFocalLaw->adwElement[1]=0;
		if(!pOEMPA->IsMultiplexer() && !pOEMPA->GetEmissionEnable(iCycle,pFocalLaw->adwElement))
			return false;
	}else{
		if(iFocalCountMax>g_iOEMPAFocalCountMax)
			return false;
		pFocalLaw->dWedgeDelay = 0;
		if(!pOEMPA->GetReceptionWedgeDelay(iCycle,iCycleCountMax,&pFocalLaw->dWedgeDelay))
			return false;
		pFocalLaw->iPrmCount = 0;
		if(!pOEMPA->GetReceptionGains(iCycle,iElementCountMax,&pFocalLaw->iPrmCount,pFocalLaw->afPrm))
			return false;
		callbackArrayFloat2D.apParameter[0] = pFocalLaw;
		callbackArrayFloat2D.pSetSize = CallbackSetSize2;
		callbackArrayFloat2D.pSetData = CallbackSetData2;
		callbackArrayFloat2D.pGetSize = CallbackGetSize2;
		callbackArrayFloat2D.pGetData = CallbackGetData2;
		callbackArrayFloat2D.pSetSize(&callbackArrayFloat2D,0,0);//default init
		if(!pOEMPA->GetReceptionDelays(iCycle,iElementCountMax,iFocalCountMax,callbackArrayFloat2D))
			return false;
		pFocalLaw->adwElement[0]=pFocalLaw->adwElement[1]=0;
		if(!pOEMPA->IsMultiplexer() && !pOEMPA->GetReceptionEnable(iCycle,pFocalLaw->adwElement))
			return false;
		if(!pRoot->bEnableFMC)
		{//1.1.5.1 FMC
			pFocalLaw->iFocalCount = 0;
			if(!pOEMPA->GetReceptionFocusing(iCycle,iFocalCountMax,&pFocalLaw->iFocalCount,pFocalLaw->adFocalTimeOfFlight,&pFocalLaw2->fCenterDelay,&pFocalLaw->fCenterDelay))
				return false;
			if(!pOEMPA->GetEnableDDF(iCycle,&pFocalLaw->eFocusing))
				return false;
		}else{
			pFocalLaw->iFocalCount = 1;
			pFocalLaw->eFocusing = eStandard;
		}
	}
	
	return bRet;
}

bool ReadHWWriteFileMultiHWChannel(CHWDeviceOEMPA *pOEMPA,int iCycle,bool bPulse,int iCycleCountMax,int iElementCountMax,structRoot *pRoot,CFocalLaw *pFocalLaw)
{
	bool bRet=true;
	structCallbackArrayFloat1D callbackArrayFloat1D;

	if(!pRoot->bEnableMultiHWChannel)
		return false;

	pFocalLaw->eFocusing = eStandard;
	pFocalLaw->dWedgeDelay = 0.0;
	pFocalLaw->iElementCount = 0;
	pFocalLaw->iPrmCount = 0;
	pFocalLaw->iDelayCount = 0;

	if(bPulse)
	{
		if(!pOEMPA->GetEmissionWidths(iCycle,iElementCountMax,&pFocalLaw->iPrmCount,pFocalLaw->afPrm))
			return false;
		if(!pOEMPA->GetEmissionDelays(iCycle,iElementCountMax,&pFocalLaw->iElementCount,&pFocalLaw->afDelay[0][0]))//1.1.5.3u
			return false;
		pFocalLaw->adwElement[0]=pFocalLaw->adwElement[1]=0;
		if(!pOEMPA->GetEmissionEnable(iCycle,pFocalLaw->adwElement))
			return false;
	}else{
		callbackArrayFloat1D.apParameter[0] = pFocalLaw;
		callbackArrayFloat1D.apParameter[1] = pRoot;
		callbackArrayFloat1D.pSetSize = CallbackSetSize1;
		callbackArrayFloat1D.pSetData = CallbackSetData1;
		callbackArrayFloat1D.pGetSize = CallbackGetSize1;
		callbackArrayFloat1D.pGetData = CallbackGetData1;
		callbackArrayFloat1D.pSetSize(&callbackArrayFloat1D,0);//default init
		if(!pOEMPA->GetMultiHWChannelAcqWriteStart(iCycle,iElementCountMax,callbackArrayFloat1D))
			return false;
		pFocalLaw->adwElement[0]=pFocalLaw->adwElement[1]=0;
		if(!pOEMPA->GetReceptionEnable(iCycle,pFocalLaw->adwElement))
			return false;
		memset(&pFocalLaw->hwAcqDecimation,0,sizeof(unionOEMMCDecimation));
		if(!pOEMPA->GetMultiHWChannelAcqDecimation(iCycle,&pFocalLaw->hwAcqDecimation))
			return false;
	}
	
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
pCallbackSettingId g_pCallbackSettingId=NULL;
_CUST_DRV_DEF_ void WINAPI OEMPA_SetCallbackSettingId(pCallbackSettingId pFunc)
{
	g_pCallbackSettingId = pFunc;
}
_CUST_DRV_DEF_ pCallbackSettingId WINAPI OEMPA_GetCallbackSettingId()
{
	return g_pCallbackSettingId;
}

/////////////////////////////////////////////////////////////////////////////
_CUST_DRV_DEF_ bool WINAPI OEMPA_ReadHW(CHWDeviceOEMPA *pOEMPA,int iCycleCountMax,int iElementCountMaxIn,int iFocalCountMax,int iDACCountMax,
							structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception)
{
	wchar_t wcSection[MAX_PATH];
	wchar_t wcAux[MAX_PATH];
	bool bRet=true;
	int iElementCountMax;
	structCycle *pCycleAux;
	DWORD dwSettingId;
	bool bMux=false,bIOBoard,bCycleNew;
	pCallbackCustomizedDriverAPI pCallbackCustomizedAPI=OEMPA_GetCallbackCustomizedDriverAPI();

	if(pEmission || pReception)
		return false;
	if(pCycle)
		bCycleNew = false;
	else
		bCycleNew = true;

	if(pCallbackCustomizedAPI)
		(*pCallbackCustomizedAPI)(pOEMPA,root.wcFileName,eReadHW_Enter,&root,pCycle,pEmission,pReception);
	if(!pOEMPA->GetSWDevice()->IsConnected())
	{
		OempaApiMessageBox(NULL,L"Device is disconnected!",L"HW error",MB_OK);
		return false;
	}
	if(!root.pLockUnlock)
	{
		if(!pOEMPA->LockDevice(eAcqDefault))
			return false;
	}else{
		if(!(*root.pLockUnlock)(&root,true,eAcqDefault))
			return false;
	}
	if(g_pCallbackSettingId)
	{
		dwSettingId = pOEMPA->swGetSettingId();//current value
		if(g_pCallbackSettingId(dwSettingId) && !pOEMPA->SetSettingId(dwSettingId))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetSettingId",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	bIOBoard = pOEMPA->GetSWDeviceOEMPA()->IsIOBoardEnabled();
	//first read the cycle count
	if(!pOEMPA->GetApertureCountMax(&iElementCountMax))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetApertureCountMax",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(!pOEMPA->GetCycleCount(&root.iCycleCount))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetCycleCount",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(root.pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelSupported() &&
		!pOEMPA->GetEnableMultiHWChannel(&root.bEnableMultiHWChannel))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetEnableMultiHWChannel",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(root.pHWDeviceOEMPA && (root.pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture() ||
		root.pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite()))
	{//1.1.5.1 FMC
		if(!root.pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite())
			root.bEnableFMC = true;
		else if(!pOEMPA->GetEnableFMC(&root.bEnableFMC))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetEnableFMC",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	if(!pOEMPA->Flush())//to get right now the cycle count in order to alloc memory.
	{
		OempaApiMessageBox(NULL,L"pOEMPA->Flush",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(!root.iCycleCount)
		{bRet = false;goto end;}
	if(iElementCountMaxIn<iElementCountMax)
		iElementCountMax = iElementCountMaxIn;

	if(bCycleNew)
	{
		pCycle = OEMPA_AllocCycle(root.iCycleCount);
		if(!pCycle)
			{bRet = false;goto end;}
	}
	pEmission = new CFocalLaw[root.iCycleCount];
	pReception = new CFocalLaw[root.iCycleCount];
	if(!pEmission || !pReception)
		{bRet = false;goto end;}
	OEMPA_ResetArrayFocalLaw(root.iCycleCount,pEmission);
	OEMPA_ResetArrayFocalLaw(root.iCycleCount,pReception);

	if(root.ullSavedParameters & OEMPA_DEFAULT_HW_LINK)
	{
		root.eDefaultHwLink = pOEMPA->GetDefaultHwLink();
	}
	if((root.ullSavedParameters & OEMPA_DISABLE_USB3) && pOEMPA->GetSWDeviceOEMPA()->GetFlashUSB3Version() && !pOEMPA->GetUSB3Disabled((bool*)&root.bUSB3Disable))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetUSB3Disabled",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(((root.pHWDeviceOEMPA && root.pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelSupported())
		|| (root.ullSavedParameters & OEMPA_ENABLE_MULTI_CHANNEL))
		&& !pOEMPA->GetEnableMultiHWChannel(&root.bEnableMultiHWChannel))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetEnableMultiHWChannel",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if((root.ullSavedParameters & OEMPA_ASCAN_ENABLE) && !pOEMPA->GetEnableAscan(&root.bAscanEnable))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetEnableAscan",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if((root.ullSavedParameters & OEMPA_CSCAN_ENABLE_TOF) && !pOEMPA->GetEnableCscanTof(&root.bEnableCscanTof))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetEnableCscanTof",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if((root.ullSavedParameters & OEMPA_ASCAN_BITSIZE) && !pOEMPA->GetAscanBitSize(&root.eAscanBitSize))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetAscanBitSize",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if((root.ullSavedParameters & OEMPA_TRIGGER_MODE) && !pOEMPA->GetTriggerMode(&root.eTriggerMode))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetTriggerMode",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if((root.ullSavedParameters & OEMPA_ENCODER_TRIG_DIRECTION) && !pOEMPA->GetEncoderDirection(&root.eEncoderDirection))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetEncoderDirection",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_TRIGGER_ENCODER_STEP) && !pOEMPA->GetTriggerEncoderStep(&root.dTriggerEncoderStep))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetTriggerEncoderStep",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_REQUESTIO) && !pOEMPA->GetRequestIO(&root.eRequestIO))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetRequestIO",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_REQUESTIO_DIGITAL_INPUT_MASK) && !pOEMPA->GetRequestIODigitalInputMask(&root.iRequestIODigitalInputMaskFalling,&root.iRequestIODigitalInputMaskRising))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetRequestIODigitalInputMask",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DEBOUNCER_ENCODER) && !pOEMPA->GetEncoderDebouncer(&root.dDebouncerEncoder))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetEncoderDebouncer",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DEBOUNCER_DIGITAL) && !pOEMPA->GetDigitalDebouncer(&root.dDebouncerDigital))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetDigitalDebouncer",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_0) && !pOEMPA->GetDigitalOutput(0,&root.eDigitalOuput[0]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetDigitalOutput",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_1) && !pOEMPA->GetDigitalOutput(1,&root.eDigitalOuput[1]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetDigitalOutput",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_2) && !pOEMPA->GetDigitalOutput(2,&root.eDigitalOuput[2]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetDigitalOutput",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_3) && !pOEMPA->GetDigitalOutput(3,&root.eDigitalOuput[3]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetDigitalOutput",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_4) && !pOEMPA->GetDigitalOutput(4,&root.eDigitalOuput[4]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetDigitalOutput",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_5) && !pOEMPA->GetDigitalOutput(5,&root.eDigitalOuput[5]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetDigitalOutput",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_SW_ENCODER1_RESOLUTION))
		root.lSWEncoderResolution1 = pOEMPA->GetSWDevice()->GetSWEncoder(0)->lGetResolution();
	if(bIOBoard && (root.ullSavedParameters & OEMPA_SW_ENCODER1_DIVIDER))
		root.dwSWEncoderDivider1 = pOEMPA->GetSWDevice()->GetSWEncoder(0)->GetDivider();
	if(bIOBoard && (root.ullSavedParameters & OEMPA_SW_ENCODER2_RESOLUTION))
		root.lSWEncoderResolution2 = pOEMPA->GetSWDevice()->GetSWEncoder(1)->lGetResolution();
	if(bIOBoard && (root.ullSavedParameters & OEMPA_SW_ENCODER2_DIVIDER))
		root.dwSWEncoderDivider2 = pOEMPA->GetSWDevice()->GetSWEncoder(1)->GetDivider();
	if(bIOBoard && (root.ullSavedParameters & OEMPA_ENCODER1A) && !pOEMPA->GetEncoderWire1(0,&root.eEncoder1A))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetEncoderWire1",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_ENCODER1B) && !pOEMPA->GetEncoderWire2(0,&root.eEncoder1B))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetEncoderWire2",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_ENCODER2A) && !pOEMPA->GetEncoderWire1(1,&root.eEncoder2A))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetEncoderWire1",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_ENCODER2B) && !pOEMPA->GetEncoderWire2(1,&root.eEncoder2B))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetEncoderWire2",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_EXTERNAL_TRIGGER_CYCLE) && !pOEMPA->GetExternalTriggerCycle(&root.eExternalTriggerCycle))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetExternalTriggerCycle",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_EXTERNAL_TRIGGER_SEQUENCE) && !pOEMPA->GetExternalTriggerSequence(&root.eExternalTriggerSequence))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetExternalTriggerSequence",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if((root.ullSavedParameters & OEMPA_KEEPALIVE) && !pOEMPA->GetKeepAlive(&root.eKeepAlive))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->GetKeepAlive",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard)
	{
		if((root.ullSavedParameters & OEMPA_ENCODER1_TYPE) && (root.ullSavedParameters & OEMPA_ENCODER2_TYPE) &&
			!pOEMPA->GetEncoderType(&root.eEncoder1Type,&root.eEncoder2Type))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetEncoder12Type",L"HW error",MB_OK);
			bRet = false;goto end;
		}
		if((root.ullSavedParameters & OEMPA_ENCODER1_TYPE) && !(root.ullSavedParameters & OEMPA_ENCODER2_TYPE) &&
			!pOEMPA->GetEncoderType(&root.eEncoder1Type,NULL))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetEncoder1Type",L"HW error",MB_OK);
			bRet = false;goto end;
		}
		if(!(root.ullSavedParameters & OEMPA_ENCODER1_TYPE) && (root.ullSavedParameters & OEMPA_ENCODER2_TYPE) &&
			!pOEMPA->GetEncoderType(NULL,&root.eEncoder2Type))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetEncoder2Type",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	if(root.ullSavedParameters & OEMPA_ASCAN_REQUEST)
	{
		if(!pOEMPA->GetAscanRequest(&root.eAscanRequest))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetAscanRequest",L"HW error",MB_OK);
			bRet = false;goto end;
		}
		if(!pOEMPA->GetAscanRequestFrequency(&root.dAscanRequestFrequency))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetAscanRequestFrequency",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	if(root.ullSavedParameters & OEMPA_TRIGGER_HIGH_TIME)
	{
		if(!pOEMPA->GetSignalTriggerHighTime(&root.dTriggerHighTime))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetSignalTriggerHighTime",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	if(pOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture())
	{
		if(pOEMPA->GetSWDeviceOEMPA()->IsFMCElementStepSupported())
		{
			if(!pOEMPA->GetFMCElement(&root.iFMCElementStart,&root.iFMCElementStop,&root.iFMCElementStep))
				{bRet = false;goto end;}
		}else{
			if(root.iFMCElementStep!=1)
				{bRet = false;goto end;}				
			if(!pOEMPA->GetFMCElementStart(&root.iFMCElementStart))
				{bRet = false;goto end;}
			if(!pOEMPA->GetFMCElementStop(&root.iFMCElementStop))
				{bRet = false;goto end;}
		}
	}
	if(root.ullSavedParameters & OEMPA_TEMPERATURE_WARNING)
	{
		if(!pOEMPA->GetTemperatureAlarm((BYTE*)&root.cTemperatureWarning,NULL))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetTemperatureAlarm",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	if(root.ullSavedParameters & OEMPA_TEMPERATURE_ALARM)
	{
		if(!pOEMPA->GetTemperatureAlarm(NULL,(BYTE*)&root.cTemperatureAlarm))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetTemperatureAlarm",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}

	for(int iFilterIndex=0;iFilterIndex<(int)root.uiSavedFilterCount;iFilterIndex++)
	{
		if(!pOEMPA->GetFilter((enumOEMPAFilterIndex)(eOEMPAFilter1+iFilterIndex),
						&root.aFilter[iFilterIndex].wScale,root.aFilter[iFilterIndex].aCoefficient))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetFilter",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}

	if(!pOEMPA->GetSubSequenceTableEncoder(g_iSubSequenceTableSize,root.aiSubSequenceCycleStart,root.aiSubSequenceCycleStop,NULL,g_iSubSequenceTableSize,root.afSubSequenceValue,&root.iSubSequenceEncoderCount))
		{bRet = false;goto end;}
	if(!pOEMPA->GetSubSequenceTableGate(g_iSubSequenceTableSize,root.aiSubSequenceCycleStart,root.aiSubSequenceCycleStop,NULL,g_iSubSequenceTableSize,root.afSubSequenceValue,&root.iSubSequenceGateCount))
		{bRet = false;goto end;}
	if(!pOEMPA->GetSubSequenceAverage(&root.iSubSequenceAverage))
		{bRet = false;goto end;}

	for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
	{
		swprintf(wcSection,MAX_PATH,L"Cycle:%d",iCycle);
		if(bCycleNew && !OEMPA_ReadHWWriteFileCycle(pOEMPA,iCycle,iDACCountMax,&root,&pCycle[iCycle]))
			{bRet = false;goto end;}

		if(!pEmission[iCycle].GetDWORDElementSize())
			pEmission[iCycle].SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
		if(!ReadHWWriteFileFocalLaw(pOEMPA,iCycle,true,iCycleCountMax,iElementCountMax,iFocalCountMax,&root,&pEmission[iCycle],&pReception[iCycle]))
		{
			swprintf(wcAux,MAX_PATH,L"Emission Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;goto end;
		}
		if(!pReception[iCycle].GetDWORDElementSize())
			pReception[iCycle].SetAllocatedSize(root.iOEMPADDFCountMax,root.iOEMPAProbeCountMax);
		if(!ReadHWWriteFileFocalLaw(pOEMPA,iCycle,false,iCycleCountMax,iElementCountMax,iFocalCountMax,&root,&pReception[iCycle],&pEmission[iCycle]))
		{
			swprintf(wcAux,MAX_PATH,L"Reception Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;goto end;
		}
		if(pOEMPA->IsMultiplexer())
		{
			bMux = true;//there is only one mux for emission and reception
			if(!pOEMPA->GetMultiplexerEnable(iCycle,pEmission[iCycle].adwElement,pReception[iCycle].adwElement))
			{
				bRet = false;goto end;
			}
			//Impossible to read reception at the same time
			//if(!pOEMPA->GetMultiplexerEnable(iCycle,pReception[iCycle].adwElement))
			//	return false;
		}
	}

end:
	if(!root.pLockUnlock)
	{
		if(!pOEMPA->UnlockDevice())
		{
			OempaApiMessageBox(NULL,L"Communication Error!",L"Error",MB_OK);
			bRet = false;
		}
	}else{
		if(!(*root.pLockUnlock)(&root,false,eAcqDefault))
		{
			OempaApiMessageBox(NULL,L"Communication Error!",L"Error",MB_OK);
			bRet = false;
		}
	}

	if(root.bEnableMultiHWChannel)
	{
		for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
		{
			if(pReception[iCycle].adwElement.GetFirstElementIndex()>=0)
				pReception[iCycle].iElementCount = 1;
			if(!ReadHWMultiHWChannelUpdate(iCycle,&root,pCycle,pReception))
				{bRet = false;break;}
		}
	}
	if(bRet)
	{
		for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
		{
			if(bCycleNew)
			{
				pCycleAux = &pCycle[iCycle];
				pCycleAux->iGateCount = 0;
				for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
				{
					if(pCycleAux->gate[iGate].bEnable)
						pCycleAux->iGateCount = iGate+1;//1.1.5.0u pCycleAux->iGateCount++;
				}
			}
			if(g_bReferenceTimeOfFlightCorrection && (root.eReferenceTimeOfFlight==ePulse) && (pReception[iCycle].iFocalCount>1))
			{
				for(int iFocalIndex=0;iFocalIndex<pReception[iCycle].iFocalCount;iFocalIndex++)
				{
					pReception[iCycle].adFocalTimeOfFlight[iFocalIndex] += pReception[iCycle].dWedgeDelay+pEmission[iCycle].dWedgeDelay;
					pReception[iCycle].adFocalTimeOfFlight[iFocalIndex] -= pReception[iCycle].fCenterDelay + pEmission[iCycle].fCenterDelay;//1.1.5.4n
				}
			}
		}
	}else{
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

	if(pCallbackCustomizedAPI)
		(*pCallbackCustomizedAPI)(pOEMPA,root.wcFileName,eReadHW_Leave,&root,pCycle,pEmission,pReception);

	return bRet;
}

_CUST_DRV_DEF_ bool WINAPI OEMPA_ReadHWWriteFile(CHWDeviceOEMPA *pOEMPA,int iCycleCountMax,int iElementCountMaxIn,int iDDFCountMax,int iDACCountMax,wchar_t *pFileName)
{
	wchar_t wcAux[MAX_PATH];
	structRoot root;
	structCycle *pCycle=NULL;
	CFocalLaw *pEmission=NULL;
	CFocalLaw *pReception=NULL;
	bool bRet=true,bAllocParamList=false;
	bool bTextFile;

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
	OEMPA_InitRoot(root,pOEMPA);
	if(!IsValidFileExtension(pFileName,bTextFile))
	{
		OempaApiMessageBox(NULL,L"Bad file extension!",L"File Error",MB_OK);
		return false;
	}
	OEMPA_InitRoot(root,pOEMPA);
	wcscpy_s(root.wcFileName,MAX_PATH,pFileName);

	if(!OEMPA_ReadHW(pOEMPA,iCycleCountMax,iElementCountMaxIn,iDDFCountMax,iDACCountMax,
				root,pCycle,pEmission,pReception))
	{
		swprintf(wcAux,MAX_PATH,L"HW read back error");
		OempaApiMessageBox(NULL,wcAux,L"HW error",MB_OK);
		bRet = false;goto end;
	}

	if(bTextFile && !OEMPA_WriteFileText(	pFileName,&root,pCycle,pEmission,pReception))
	{
		swprintf(wcAux,MAX_PATH,L"HW => File error");
		OempaApiMessageBox(NULL,wcAux,L"File Error",MB_OK);
		bRet = false;goto end;
	}
	if(!bTextFile && !OEMPA_WriteFileBinary(	pFileName,&root,pCycle,pEmission,pReception))
	{
		swprintf(wcAux,MAX_PATH,L"HW => File error");
		OempaApiMessageBox(NULL,wcAux,L"File Error",MB_OK);
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

	if(bAllocParamList && root.rParamList)
	{
		delete root.rParamList;
		root.rParamList = NULL;
	}

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
bool WriteHWCycleGate(CHWDeviceOEMPA *pOEMPA,int iCycle,int iGate,structRoot *pRoot,structCycle *pCycle)
{
	structGate *pGate=&pCycle->gate[iGate];
	bool bRet=true;

	if(!pOEMPA->SetGateModeThreshold(iCycle,iGate,pGate->bEnable,pGate->eModeAmp,pGate->eModeTof,pGate->eRectification,pGate->dThreshold))
		return false;
	if(pRoot->ullSavedParameters & OEMPA_TRACKING)
	{
		if(!pOEMPA->SetTrackingGateStart(iCycle,iGate,pGate->bTrackingStartEnable,pGate->iTrackingStartIndexCycle,pGate->iTrackingStartIndexGate))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetTrackingGateStart",L"HW error",MB_OK);
			return false;
		}
		if(!pOEMPA->SetTrackingGateStop(iCycle,iGate,pGate->bTrackingStopEnable,pGate->iTrackingStopIndexCycle,pGate->iTrackingStopIndexGate))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetTrackingGateStop",L"HW error",MB_OK);
			return false;
		}
	}else{
		if(!pOEMPA->SetTrackingGateStart(iCycle,iGate,false,pGate->iTrackingStartIndexCycle,pGate->iTrackingStartIndexGate))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetTrackingGateStart",L"HW error",MB_OK);
			return false;
		}
		if(!pOEMPA->SetTrackingGateStop(iCycle,iGate,false,pGate->iTrackingStopIndexCycle,pGate->iTrackingStopIndexGate))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetTrackingGateStop",L"HW error",MB_OK);
			return false;
		}
	}
	if(!pGate->bEnable)
		return true;
	if(!pOEMPA->SetGateStart(iCycle,iGate,pGate->dStart))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetGateStart",L"HW error",MB_OK);
		return false;
	}
	if(!pOEMPA->SetGateStop(iCycle,iGate,pGate->dStop))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetGateStop",L"HW error",MB_OK);
		return false;
	}
	if(!pOEMPA->SetGateAcqIDAmp(iCycle,iGate,pGate->wAcqIDAmp))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetGateAcqIDAmp",L"HW error",MB_OK);
		return false;
	}
	if(!pOEMPA->SetGateAcqIDTof(iCycle,iGate,pGate->wAcqIDTof))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetGateAcqIDTof",L"HW error",MB_OK);
		return false;
	}
	return bRet;
}

bool WriteHWCycle(CHWDeviceOEMPA *pOEMPA,int iCycle,structRoot *pRoot,structCycle *pCycle,int iMultiChannelReceptionElementIndex)
{
	bool bRet=true;
	wchar_t pStringError[MAX_PATH/2];
	wchar_t pStringError2[MAX_PATH/2];
	bool bMultiHWChannelSupported;
	double dThroughput;

	if(!pOEMPA)
		return false;
	if(!pCycle)
		return false;
	bMultiHWChannelSupported = pOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelSupported();
	if(bMultiHWChannelSupported && pRoot->bEnableMultiHWChannel)
	{
		if(!pOEMPA->EnableMultiHWChannelAcquisition(iCycle,pRoot->iCycleCount,pCycle->bHWAcquisition))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->EnableMultiHWChannelAcquisition",L"HW error",MB_OK);
			return false;
		}
#ifdef _DECIMATION_TIME_CORRECTION_
		if(!pOEMPA->swSetMultiHWChannelDecimationElement(iCycle,iMultiChannelReceptionElementIndex))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->swSetMultiHWChannelDecimationElement",L"HW error",MB_OK);
			return false;
		}
#endif _DECIMATION_TIME_CORRECTION_
	}else{
		if(!pOEMPA->SetAscanStart(iCycle,pCycle->dStart))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetAscanStart",L"HW error",MB_OK);
			return false;
		}
	}

	if(!pOEMPA->SetGainDigital(iCycle,pCycle->dGainDigital))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetGainDigital",L"HW error",MB_OK);
		return false;
	}
	if(!pOEMPA->SetBeamCorrection(iCycle,pCycle->fBeamCorrection))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetBeamCorrection",L"HW error",MB_OK);
		return false;
	}
	if(!pOEMPA->SetFilterIndex(iCycle,(enumOEMPAFilterIndex)pCycle->iFilterIndex))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetFilterIndex",L"HW error",MB_OK);
		return false;
	}
	//1.1.5.4g to avoid spike
	//if((!pRoot->bEnableMultiHWChannel || (pRoot->bEnableMultiHWChannel && pCycle->bHWAcquisition)) &&
	//	(!pOEMPA->SetGainAnalog(iCycle,pCycle->fGainAnalog))
	if(!pOEMPA->SetGainAnalog(iCycle,pCycle->fGainAnalog))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetGainAnalog",L"HW error",MB_OK);
		return false;
	}
	
	if(pCycle->lPointCount)
	{
		if(!pOEMPA->SetAscanRangeWithCount(iCycle,pCycle->dRange,pCycle->eCompressionType,pCycle->lPointCount,pCycle->lPointFactor))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetAscanRangeWithCount",L"HW error",MB_OK);
			return false;
		}
	}else
	if(pCycle->lPointFactor)
	{
		if(!pOEMPA->SetAscanRangeWithFactor(iCycle,pCycle->dRange,pCycle->eCompressionType,pCycle->lPointFactor,pCycle->lPointCount))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetAscanRangeWithFactor",L"HW error",MB_OK);
			return false;
		}
	}else{
		pCycle->eCompressionType = eCompression;
		if(!pOEMPA->SetAscanRange(iCycle,pCycle->dRange,pCycle->eCompressionType,pCycle->lPointCount,pCycle->lPointFactor))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetAscanRange",L"HW error",MB_OK);
			return false;
		}
	}
	if(pRoot->bEnableFMC)
	{
		double dTimeSlotMin,dHWAcqSubTimeSlotMin,dReplaySubTimeSlotMin,dReplaySubTimeSlotOptimizedForThroughput,dTimeSlot;
		if(!pOEMPA->GetFMCTimeLimitation(pCycle->dStart,pCycle->dRange,pCycle->dTimeSlot,dTimeSlotMin,dHWAcqSubTimeSlotMin,dReplaySubTimeSlotMin,dReplaySubTimeSlotOptimizedForThroughput))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->GetFMCTimeLimitation",L"HW error",MB_OK);
			return false;
		}
		int iCount=pOEMPA->GetFMCSubTimeSlotCount();
		if(!pCycle->dTimeSlot)
		{//get the minimum value.
			if((pCycle->dFMCSubTimeSlotAcq<0.0) || (pCycle->dFMCSubTimeSlotReplay<0.0))
			{
				pCycle->dFMCSubTimeSlotAcq = dHWAcqSubTimeSlotMin;
				pCycle->dFMCSubTimeSlotReplay = dReplaySubTimeSlotOptimizedForThroughput;//to smooth the throughput.
				pCycle->dTimeSlot = dTimeSlotMin;
			}else{
				double dFMCMargin;
				if(!iCount)
				{
					OempaApiMessageBox(NULL,L"pOEMPA->GetFMCSubTimeSlotCount",L"HW error",MB_OK);
					return false;
				}
				dFMCMargin = dTimeSlotMin-(dHWAcqSubTimeSlotMin+dReplaySubTimeSlotMin*(iCount-1));
				pCycle->dTimeSlot = dFMCMargin+pCycle->dFMCSubTimeSlotAcq+pCycle->dFMCSubTimeSlotReplay*(iCount-1);
			}
		}
		dTimeSlot = pCycle->dTimeSlot;
		if (pOEMPA->GetSWDeviceOEMPA()->IsFMCSubTimeSlotPerCycleSupported())
		{
			if((pCycle->dFMCSubTimeSlotAcq<0.0) || (pCycle->dFMCSubTimeSlotReplay<0.0))
			{
				if(!pCycle->bTrackingAscanEnable)
				{
					if(!pOEMPA->SetFMCSubTimeSlotAcqReplay(iCycle,pCycle->dStart,pCycle->dRange,dTimeSlot))
					{
						OempaApiMessageBox(NULL,L"pOEMPA->SetFMCSubTimeSlotAcqReplay",L"HW error",MB_OK);
						return false;
					}
				}else{
					OempaApiMessageBox(NULL,L"Tracking and FMC not compatible!",L"HW error",MB_OK);
					return false;
				}
			}else{
				if(!pOEMPA->SetFMCSubTimeSlotAcq(iCycle,pCycle->dFMCSubTimeSlotAcq))
				{
					OempaApiMessageBox(NULL,L"pOEMPA->SetFMCSubTimeSlotAcq",L"HW error",MB_OK);
					return false;
				}
				if(!pOEMPA->SetFMCSubTimeSlotReplay(iCycle,pCycle->dFMCSubTimeSlotReplay))
				{
					OempaApiMessageBox(NULL,L"pOEMPA->SetFMCSubTimeSlotReplay",L"HW error",MB_OK);
					return false;
				}
			}
		}else{
			if(!pCycle->bTrackingAscanEnable && (dTimeSlot<dTimeSlotMin))
			{
				OempaApiMessageBox(NULL,L"TimeSlotMin",L"HW error",MB_OK);
				return false;
			}
		}
	}
	if(!pOEMPA->SetTimeSlot(iCycle,pCycle->dTimeSlot))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetTimeSlot",L"HW error",MB_OK);
		return false;
	}
	dThroughput = pOEMPA->GetAscanThroughput(pCycle->dTimeSlot, pCycle->lPointCount, pRoot->eAscanBitSize);
	if(pRoot->bEnableFMC && (pRoot->iCycleCount>0))
	{
		int iCount=pOEMPA->GetFMCSubTimeSlotCount();
		dThroughput *= (double)iCount;
	}
	int iDataCount=0;
	if(pCycle->bMaximum)
		iDataCount++;
	if(pCycle->bMinimum)
		iDataCount++;
	if(pCycle->bSaturation)
		iDataCount++;
	dThroughput *= (double)iDataCount;
	if(dThroughput > pOEMPA->GetSWDeviceOEMPA()->GetMaximumThroughput())
	{
		bool bCreate;
		if(CSWDevice::ReadCfgBool(L"CustomizedAPI",L"WarningThroughput",true,bCreate))
		{
			wchar_t msg[MAX_PATH];
			swprintf(msg,MAX_PATH,L"Cycle %d: ascan throughput is %.1lf MB/sec but maximum value is %.1lf MB/sec", iCycle, dThroughput/(1024.0*1024.0), pOEMPA->GetSWDeviceOEMPA()->GetMaximumThroughput()/(1024.0*1024.0));
			UTKernel_SystemMessageBoxList(msg);
		}
	}
	if(!pOEMPA->SetAscanRectification(iCycle,pCycle->eRectification))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetAscanRectification",L"HW error",MB_OK);
		return false;
	}
	
	if(pCycle->bDACSlope)
	{
		if(!pOEMPA->SetDACSlope(iCycle,pCycle->iDACCount,pCycle->adDACTof,pCycle->afDACPrm,MAX_PATH/2,pStringError))
		{
			if(!wcslen(pStringError))
				OempaApiMessageBox(NULL,L"pOEMPA->SetDACSlope",L"HW error",MB_OK);
			else{
				wcscpy_s(pStringError2,MAX_PATH/2,L"pOEMPA->SetDACSlope ");
				wcscat_s(pStringError2,MAX_PATH/2,pStringError);
				OempaApiMessageBox(NULL,pStringError2,L"HW error",MB_OK);
			}
			return false;
		}
	}else{
		if(!pOEMPA->SetDACGain(pCycle->bDACAutoStop,iCycle,pCycle->iDACCount,pCycle->adDACTof,pCycle->afDACPrm))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetDACGain",L"HW error",MB_OK);
			return false;
		}
	}
	if(!pOEMPA->EnableDAC(iCycle,pCycle->bDACEnable))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->EnableDAC",L"HW error",MB_OK);
		return false;
	}
	
	if(!pOEMPA->EnableAscanMaximum(iCycle,pCycle->bMaximum))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->EnableAscanMaximum",L"HW error",MB_OK);
		return false;
	}
	if(!pOEMPA->EnableAscanMinimum(iCycle,pCycle->bMinimum))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->EnableAscanMinimum",L"HW error",MB_OK);
		return false;
	}
	if(pCycle->bSaturation)
	{
		OempaApiMessageBox(NULL,L"EnableAscanSaturation is forbidden!",L"HW error",MB_OK);
		pCycle->bSaturation = false;
	}
	if(!pOEMPA->EnableAscanSaturation(iCycle,pCycle->bSaturation))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->EnableAscanSaturation",L"HW error",MB_OK);
		return false;
	}

	if(!pOEMPA->SetAscanAcqIdChannelProbe(iCycle,pCycle->wAcqIdChannelProbe))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetAscanAcqIdChannelProbe",L"HW error",MB_OK);
		return false;
	}
	if(!pOEMPA->SetAscanAcqIdChannelScan(iCycle,pCycle->wAcqIdChannelScan))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetAscanAcqIdChannelScan",L"HW error",MB_OK);
		return false;
	}
	if(!pOEMPA->SetAscanAcqIdChannelCycle(iCycle,pCycle->wAcqIdChannelCycle))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetAscanAcqIdChannelCycle",L"HW error",MB_OK);
		return false;
	}

	if(pRoot->ullSavedParameters & OEMPA_TRACKING)
	{
		if(!pOEMPA->SetTrackingAscan(iCycle,pCycle->bTrackingAscanEnable,pCycle->iTrackingAscanIndexCycle,pCycle->iTrackingAscanIndexGate))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetTrackingAscan",L"HW error",MB_OK);
			return false;
		}
		if(!pOEMPA->SetTrackingDac(iCycle,pCycle->bTrackingDacEnable,pCycle->iTrackingDacIndexCycle,pCycle->iTrackingDacIndexGate))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetTrackingDac",L"HW error",MB_OK);
			return false;
		}
	}else{
		if(!pRoot->bEnableMultiHWChannel && !pOEMPA->SetTrackingAscan(iCycle,false,pCycle->iTrackingAscanIndexCycle,pCycle->iTrackingAscanIndexGate))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetTrackingAscan",L"HW error",MB_OK);
			return false;
		}
		if(!pOEMPA->SetTrackingDac(iCycle,false,pCycle->iTrackingDacIndexCycle,pCycle->iTrackingDacIndexGate))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetTrackingDac",L"HW error",MB_OK);
			return false;
		}
	}

	for(int iGateIndex=0;iGateIndex<g_iOEMPAGateCountMax;iGateIndex++)
	{
		if(!WriteHWCycleGate(pOEMPA,iCycle,iGateIndex,pRoot,pCycle))
			return false;
	}

	return true;
}

bool CheckMultiplexerAperture(CHWDeviceOEMPA *pOEMPA,int iCycle,structRoot *pRoot,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	//check that the merge of both aperture has not more than 32 element (in case of a 32/128)
	int aiEmissionMuxBoardIndex[32];
	int aiReceptionMuxBoardIndex[32];
	int iElementCount;
	wchar_t wcAux[MAX_PATH];

	if(!pEmission || !pReception)
		return false;
	if(pEmission->adwElement.GetAllocatedSize()!=pReception->adwElement.GetAllocatedSize())
		return false;
	if(!pEmission->adwElement.GetMultiplexerBoardIndex(pRoot->iOEMPAApertureCountMax,32,aiEmissionMuxBoardIndex,iElementCount))
		return false;
	if(iElementCount>pRoot->iOEMPAApertureCountMax)
	{
		swprintf(wcAux,MAX_PATH,L"Multiplexer too many element for emission cycle #%d",iCycle);
		OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
		return false;//too many elements
	}
	if(!pReception->adwElement.GetMultiplexerBoardIndex(pRoot->iOEMPAApertureCountMax,32,aiReceptionMuxBoardIndex,iElementCount))
		return false;
	if(iElementCount>pRoot->iOEMPAApertureCountMax)
	{
		swprintf(wcAux,MAX_PATH,L"Multiplexer too many element for reception cycle #%d",iCycle);
		OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
		return false;//too many elements
	}
	for(int i=0;i<pRoot->iOEMPAApertureCountMax;i++)
	{
		if(	(aiEmissionMuxBoardIndex[i]>=0) && (aiReceptionMuxBoardIndex[i]>=0) &&
			(aiEmissionMuxBoardIndex[i] != aiReceptionMuxBoardIndex[i]))
		{
			swprintf(wcAux,MAX_PATH,L"Multiplexer pitch/catch error for cycle #%d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			return false;
		}
	}
	return true;
}
bool WriteHWMultiHWChannel(CHWDeviceOEMPA *pOEMPA,int iCycle,bool bPulse,bool bMultiHWChannelAcqEnable,structRoot *pRoot,CFocalLaw *pFocalLaw);
bool WriteHWFocalLaw(CHWDeviceOEMPA *pOEMPA,int iCycle,bool bPulse,bool bMultiHWChannelAcqEnable,structRoot *pRoot,CFocalLaw *pFocalLaw,CFocalLaw *pFocalLaw2)
{
	bool bRet=true;
	structCallbackArrayFloat2D callbackArrayFloat2D,_callbackArrayFloat2D;
	DWORD adwHWAperture256[2*g_iOEMPAApertureDWordCount];
	bool bFullMatrixCapture;
	bool bMultiHWChannelSupported;
	double pdFocalTof[g_iOEMPAFocalCountMax];

	if(!pOEMPA)
		return false;
	bMultiHWChannelSupported = pOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelSupported();
	if(bMultiHWChannelSupported && pRoot->bEnableMultiHWChannel)
		return WriteHWMultiHWChannel(pOEMPA,iCycle,bPulse,bMultiHWChannelAcqEnable,pRoot,pFocalLaw);
	bFullMatrixCapture = pOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture();
	if(bPulse)
	{
		if(!pOEMPA->SetEmissionWedgeDelay(iCycle,pRoot->iCycleCount,pFocalLaw->dWedgeDelay))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetEmissionWedgeDelay",L"HW error",MB_OK);
			return false;
		}
		if(!pOEMPA->SetEmissionWidths(iCycle,pFocalLaw->adwElement,pFocalLaw->afPrm))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetEmissionWidths",L"HW error",MB_OK);
			return false;
		}
		if(!pOEMPA->SetEmissionDelays(iCycle,pFocalLaw->adwElement,&pFocalLaw->afDelay[0][0]))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetEmissionDelays",L"HW error",MB_OK);
			return false;
		}
		if(!pOEMPA->IsMultiplexer() && !pOEMPA->SetEmissionEnable(iCycle,pFocalLaw->adwElement))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetEmissionEnable",L"HW error",MB_OK);
			return false;
		}
	}else{
		if(!pOEMPA->SetReceptionWedgeDelay(iCycle,pRoot->iCycleCount,pFocalLaw->dWedgeDelay))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionWedgeDelay",L"HW error",MB_OK);
			return false;
		}
		if(pRoot->bEnableFMC && (pRoot->b256Master || pRoot->b256Slave))
		{
			if(!pOEMPA->SetReceptionGainsFMC256(iCycle,pFocalLaw->adwElement,pFocalLaw->afPrm))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionGainsFMC256",L"HW error",MB_OK);
				return false;
			}
		}else{
			if(!pOEMPA->SetReceptionGains(iCycle,pFocalLaw->adwElement,pFocalLaw->afPrm))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionGains",L"HW error",MB_OK);
				return false;
			}
		}
		callbackArrayFloat2D.apParameter[0] = pFocalLaw;
		callbackArrayFloat2D.pSetSize = CallbackSetSize2;
		callbackArrayFloat2D.pSetData = CallbackSetData2;
		callbackArrayFloat2D.pGetSize = CallbackGetSize2;
		callbackArrayFloat2D.pGetData = CallbackGetData2;
		if(pRoot->b256Master && pRoot->b256Slave)
		{
			OempaApiMessageBox(NULL,L"Error for 256:256",L"HW error",MB_OK);
			return false;
		}
		if(!pRoot->b256Master && !pRoot->b256Slave)
		{
			if(!pOEMPA->SetReceptionDelays(iCycle,pFocalLaw->adwElement,callbackArrayFloat2D))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionDelays",L"HW error",MB_OK);
				return false;
			}
		}else
		if(pRoot->b256Master)
		{
			pFocalLaw->adwElement.GetAperture256(true,adwHWAperture256);
			pFocalLaw->m_p256MatchedDevice->adwElement.GetAperture256(false,adwHWAperture256);
			_callbackArrayFloat2D.apParameter[0] = pFocalLaw->m_p256MatchedDevice;
			_callbackArrayFloat2D.pSetSize = CallbackSetSize2;
			_callbackArrayFloat2D.pSetData = CallbackSetData2;
			_callbackArrayFloat2D.pGetSize = CallbackGetSize2;
			_callbackArrayFloat2D.pGetData = CallbackGetData2;
			if(!pOEMPA->SetReceptionDelays256(true,iCycle,pFocalLaw->adwElement,pFocalLaw->m_p256MatchedDevice->adwElement,callbackArrayFloat2D,_callbackArrayFloat2D))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionDelays256",L"HW error",MB_OK);
				return false;
			}
		}else
		if(pRoot->b256Slave)
		{
			pFocalLaw->m_p256MatchedDevice->adwElement.GetAperture256(true,adwHWAperture256);
			pFocalLaw->adwElement.GetAperture256(false,adwHWAperture256);
			_callbackArrayFloat2D.apParameter[0] = pFocalLaw->m_p256MatchedDevice;
			_callbackArrayFloat2D.pSetSize = CallbackSetSize2;
			_callbackArrayFloat2D.pSetData = CallbackSetData2;
			_callbackArrayFloat2D.pGetSize = CallbackGetSize2;
			_callbackArrayFloat2D.pGetData = CallbackGetData2;
			if(!pOEMPA->SetReceptionDelays256(false,iCycle,pFocalLaw->m_p256MatchedDevice->adwElement,pFocalLaw->adwElement,_callbackArrayFloat2D,callbackArrayFloat2D))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionDelays256",L"HW error",MB_OK);
				return false;
			}
		}
		if(!pOEMPA->IsMultiplexer())
		{
			if(!bFullMatrixCapture)
			{
				if(!pRoot->b256Master && !pRoot->b256Slave)
				{
					if(!pOEMPA->SetReceptionEnable(iCycle,pFocalLaw->adwElement))
					{
						OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionEnable",L"HW error",MB_OK);
						return false;
					}
				}else
				if(pRoot->b256Master)
				{
					if(!pOEMPA->SetReceptionEnable256(true,iCycle,adwHWAperture256))
					{
						OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionEnable256",L"HW error",MB_OK);
						return false;
					}
				}else
				if(pRoot->b256Slave)
				{
					if(!pOEMPA->SetReceptionEnable256(false,iCycle,adwHWAperture256))
					{
						OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionEnable256",L"HW error",MB_OK);
						return false;
					}
				}
			}
		}
		if(!bFullMatrixCapture)
		{
			if(g_bReferenceTimeOfFlightCorrection && (pRoot->eReferenceTimeOfFlight==ePulse) && (pFocalLaw->iFocalCount>1))
			{
				for(int iFocalIndex=0;iFocalIndex<pFocalLaw->iFocalCount;iFocalIndex++)
				{
					pdFocalTof[iFocalIndex] = pFocalLaw->adFocalTimeOfFlight[iFocalIndex] - pFocalLaw->dWedgeDelay - pFocalLaw2->dWedgeDelay;
					pdFocalTof[iFocalIndex] += pFocalLaw->fCenterDelay + pFocalLaw2->fCenterDelay;//1.1.5.4n
				}
				if(!pOEMPA->SetReceptionFocusing(iCycle,pFocalLaw->iFocalCount,pdFocalTof,pFocalLaw2->fCenterDelay,pFocalLaw->fCenterDelay))
				{
					OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionFocusing",L"HW error",MB_OK);
					return false;
				}
			}else{
				if(!pOEMPA->SetReceptionFocusing(iCycle,pFocalLaw->iFocalCount,pFocalLaw->adFocalTimeOfFlight,pFocalLaw2->fCenterDelay,pFocalLaw->fCenterDelay))
				{
					OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionFocusing",L"HW error",MB_OK);
					return false;
				}
			}
			if(!pOEMPA->EnableDDF(iCycle,pFocalLaw->eFocusing))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->EnableDDF",L"HW error",MB_OK);
				return false;
			}
		}
	}

	return bRet;
}

bool WriteHWMultiHWChannel(CHWDeviceOEMPA *pOEMPA,int iCycle,bool bPulse,bool bHWAcquisition,structRoot *pRoot,CFocalLaw *pFocalLaw)
{
	bool bRet=true;
	bool bFullMatrixCapture;
	bool bMultiHWChannelSupported;
	double dWedgeDelay=0.0;
	DWORD adwHWAperture[g_iOEMPAApertureDWordCount]={0};
	static CFocalLaw *pFocalLawAcquisition=NULL;

	if(!pOEMPA)
		return false;
	bFullMatrixCapture = pOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture();
	bMultiHWChannelSupported = pOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelSupported();
	if(!bMultiHWChannelSupported)
		return false;
	if(!iCycle)
	{
		pFocalLawAcquisition = NULL;
		if(!bHWAcquisition)
		{
			OempaApiMessageBox(NULL,L"MultiChannel Error: the first cycle should be an acquisition!",L"File Error",MB_OK);
			return false;
		}
	}

	if(bPulse)
	{
		//1.1.5.3u if(bHWAcquisition)
		if((bHWAcquisition && !pRoot->bEnablePulserDuringReplay) || pRoot->bEnablePulserDuringReplay)
		{
			if(!pOEMPA->SetEmissionEnable(iCycle,pFocalLaw->adwElement))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetEmissionEnable",L"HW error",MB_OK);
				return false;
			}
			if(!pOEMPA->SetEmissionWidths(iCycle,pFocalLaw->adwElement,pFocalLaw->afPrm))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetEmissionWidths",L"HW error",MB_OK);
				return false;
			}
			if(!pOEMPA->SetEmissionDelays(iCycle,pFocalLaw->adwElement,&pFocalLaw->afDelay[0][0]))//1.1.5.3u
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetEmissionDelays",L"HW error",MB_OK);
				return false;
			}
		}else{
			if(!pOEMPA->SetEmissionEnable(iCycle,adwHWAperture))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetEmissionEnable",L"HW error",MB_OK);
				return false;
			}
		}
	}else{
		if(pFocalLaw->adwElement.GetApertureElementCount()>1)
		{
			OempaApiMessageBox(NULL,L"element count of MultiChannel aperture should be one!",L"HW error",MB_OK);
			return false;
		}
		if(!pOEMPA->SetReceptionEnable(iCycle,pFocalLaw->adwElement))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetReceptionEnable",L"HW error",MB_OK);
			return false;
		}
		if(bHWAcquisition)
		{
			pFocalLawAcquisition = pFocalLaw;
			float *pf=&pFocalLaw->afDelay[0][0];
			if(!pOEMPA->SetMultiHWChannelAcqWriteStart(iCycle,pFocalLaw->adwElement.GetFirstElementIndex(),pFocalLaw->iDelayCount,pf))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetMultiHWChannelAcqWriteStart",L"HW error",MB_OK);
				return false;
			}
			if(!pOEMPA->SetMultiHWChannelAcqDecimation(iCycle,pFocalLaw->hwAcqDecimation))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetMultiHWChannelAcqDecimation",L"HW error",MB_OK);
				return false;
			}
		}
	}
	if(!bHWAcquisition && !bPulse && pFocalLawAcquisition)
	{
		//Help to the HW to manage the gates (in the future).
		if(!pOEMPA->SetMultiHWChannelAcqWriteStart(iCycle,0,pFocalLawAcquisition->iDelayCount,&pFocalLawAcquisition->afDelay[0][0]))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetMultiHWChannelAcqWriteStart",L"HW error",MB_OK);
			return false;
		}
	}

	return bRet;
}
/////////////////////////////////////////////////////////////////////////////

_CUST_DRV_DEF_ bool WINAPI OEMPA_WriteHW(CHWDeviceOEMPA *pOEMPA,structRoot &root,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception,enumAcquisitionState eAcqState)
{
	wchar_t wcAux[MAX_PATH];
	bool bRet=true;
	DWORD dwSettingId;
	CSWFilterOEMPA *pFilter;
	bool bPreviousMasterUnregistered=false,bIOBoard,bFullMatrixCaptureReadWrite,bCheck;
	pCallbackCustomizedDriverAPI pCallbackCustomizedAPI=OEMPA_GetCallbackCustomizedDriverAPI();
	int iMultiChannelReceptionElementIndex;
#ifdef _WIN64
	CAcquisitionFifo *pFifo;
	int iDataCountMax;
	__int64 iBufferByteSize;
#endif _WIN64

	if(!pOEMPA)
		return false;
	if((root.iCycleCount>0) && (!pCycle || !pEmission || !pReception))
	{
		OempaApiMessageBox(NULL,L"bad inputs",L"HW error",MB_OK);
		return false;
	}
	if(pCallbackCustomizedAPI)
		(*pCallbackCustomizedAPI)(pOEMPA,root.wcFileName,eWriteHW_Enter,&root,pCycle,pEmission,pReception);

	if(!pOEMPA->GetSWDevice()->IsConnected())
	{
		OempaApiMessageBox(NULL,L"Device is disconnected!",L"HW error",MB_OK);
		return false;
	}
	GetCfgKernelReferenceTimeOfFlight(NULL);
	if(!root.pLockUnlock)
	{
		if(!pOEMPA->LockDevice(eAcqDefault))
		{
			OempaApiMessageBox(NULL,L"LockDevice",L"HW error",MB_OK);
			return false;
		}
	}else{
		if(!(*root.pLockUnlock)(&root,true,eAcqDefault))
		{
			OempaApiMessageBox(NULL,L"LockDevice",L"HW error",MB_OK);
			return false;
		}
	}
	if(g_pCallbackSettingId)
	{
		dwSettingId = pOEMPA->swGetSettingId();//current value
		if(g_pCallbackSettingId(dwSettingId) && !pOEMPA->SetSettingId(dwSettingId))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetSettingId",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	if(pOEMPA->GetMatched256DeviceOEMPA())
	{
		if(root.b256Master && !pOEMPA->Enable256(true))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->Enable256",L"HW error",MB_OK);
			bRet = false;goto end;
		}
		if(!root.b256Master && !root.b256Slave && !pOEMPA->Enable256(false))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->Enable256",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	bIOBoard = pOEMPA->GetSWDeviceOEMPA()->IsIOBoardEnabled();
	//first write the cycle count
	if((root.iCycleCount>=0) && !pOEMPA->SetCycleCount(root.iCycleCount))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetCycleCount",L"HW error",MB_OK);
		bRet = false;goto end;
	}

	if((root.ullSavedParameters & OEMPA_DEFAULT_HW_LINK) && pOEMPA->IsDefaultHwLinkEnabled() && !pOEMPA->SetDefaultHwLink(root.eDefaultHwLink,bPreviousMasterUnregistered))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetDefaultHwLink",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bPreviousMasterUnregistered)
	{
		OempaApiMessageBox(NULL,L"Only one master is possible!",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(((root.ullSavedParameters & OEMPA_DISABLE_USB3) || (root.bUSB3Disable==1)) && pOEMPA->GetSWDeviceOEMPA()->GetFlashUSB3Version() && !pOEMPA->DisableUSB3(root.bUSB3Disable==1?true:false))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->DisableUSB3",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(((root.ullSavedParameters & OEMPA_ASCAN_ENABLE) || !root.bAscanEnable) && !pOEMPA->EnableAscan(root.bAscanEnable))//1.1.5.0v
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetEnableAscan",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(((root.ullSavedParameters & OEMPA_CSCAN_ENABLE_TOF) || root.bEnableCscanTof) && !pOEMPA->EnableCscanTof(root.bEnableCscanTof))//1.1.5.0v
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetEnableCscanTof",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(((root.ullSavedParameters & OEMPA_ASCAN_BITSIZE) || (root.eAscanBitSize!=e8Bits)) && !pOEMPA->SetAscanBitSize(root.eAscanBitSize))//1.1.5.0v
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetAscanBitSize",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_SW_ENCODER1_RESOLUTION))
	{
		if(root.ullSavedParameters & OEMPA_SW_ENCODER1_DIVIDER)
		{
			if(!pOEMPA->GetSWDevice()->GetSWEncoder(0)->lSetResolution(root.lSWEncoderResolution1,root.dwSWEncoderDivider1))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->lSetResolution divider A",L"HW error",MB_OK);
				bRet = false;goto end;
			}
		}else{
			if(!pOEMPA->GetSWDevice()->GetSWEncoder(0)->lSetResolution(root.lSWEncoderResolution1))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->lSetResolution A",L"HW error",MB_OK);
				bRet = false;goto end;
			}
		}
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_SW_ENCODER2_RESOLUTION))
	{
		if(root.ullSavedParameters & OEMPA_SW_ENCODER2_DIVIDER)
		{
			if(!pOEMPA->GetSWDevice()->GetSWEncoder(1)->lSetResolution(root.lSWEncoderResolution2,root.dwSWEncoderDivider2))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->lSetResolution divider B",L"HW error",MB_OK);
				bRet = false;goto end;
			}
		}else{
			if(!pOEMPA->GetSWDevice()->GetSWEncoder(1)->lSetResolution(root.lSWEncoderResolution2))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->lSetResolution B",L"HW error",MB_OK);
				bRet = false;goto end;
			}
		}
	}
	if(((root.ullSavedParameters & OEMPA_TRIGGER_MODE) || (root.eTriggerMode!=eOEMPAInternal)) && !pOEMPA->SetTriggerMode(root.eTriggerMode))//1.1.5.0v
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetTriggerMode",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(((root.ullSavedParameters & OEMPA_ENCODER_TRIG_DIRECTION) || (root.eEncoderDirection!=eOEMPAUpDown)) && !pOEMPA->SetEncoderDirection(root.eEncoderDirection))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetEncoderDirection",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_TRIGGER_ENCODER_STEP) && !pOEMPA->SetTriggerEncoderStep(root.dTriggerEncoderStep))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetTriggerEncoderStep",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && ((root.ullSavedParameters & OEMPA_REQUESTIO) || (root.eRequestIO!=eOEMPAOnCycleOnly)) && !pOEMPA->SetRequestIO(root.eRequestIO))//1.1.5.0v
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetRequestIO",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_REQUESTIO_DIGITAL_INPUT_MASK) && !pOEMPA->SetRequestIODigitalInputMask(root.iRequestIODigitalInputMaskFalling,root.iRequestIODigitalInputMaskRising))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetRequestIODigitalInputMask",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DEBOUNCER_ENCODER) && !pOEMPA->SetEncoderDebouncer(root.dDebouncerEncoder))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetEncoderDebouncer",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DEBOUNCER_DIGITAL) && !pOEMPA->SetDigitalDebouncer(root.dDebouncerDigital))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetDigitalDebouncer",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_0) && !pOEMPA->SetDigitalOutput(0,root.eDigitalOuput[0]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetDigitalOutput 0",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_1) && !pOEMPA->SetDigitalOutput(1,root.eDigitalOuput[1]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetDigitalOutput 1",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_2) && !pOEMPA->SetDigitalOutput(2,root.eDigitalOuput[2]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetDigitalOutput 2",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_3) && !pOEMPA->SetDigitalOutput(3,root.eDigitalOuput[3]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetDigitalOutput 3",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_4) && !pOEMPA->SetDigitalOutput(4,root.eDigitalOuput[4]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetDigitalOutput 4",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_DIGITAL_OUTPUT_5) && !pOEMPA->SetDigitalOutput(5,root.eDigitalOuput[5]))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetDigitalOutput 5",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_ENCODER1A) && !pOEMPA->SetEncoderWire1(0,root.eEncoder1A))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetEncoderWire1 A",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_ENCODER1B) && !pOEMPA->SetEncoderWire2(0,root.eEncoder1B))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetEncoderWire2 A",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_ENCODER2A) && !pOEMPA->SetEncoderWire1(1,root.eEncoder2A))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetEncoderWire1 B",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_ENCODER2B) && !pOEMPA->SetEncoderWire2(1,root.eEncoder2B))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetEncoderWire2 B",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_EXTERNAL_TRIGGER_CYCLE) && !pOEMPA->SetExternalTriggerCycle(root.eExternalTriggerCycle))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetExternalTriggerCycle",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard && (root.ullSavedParameters & OEMPA_EXTERNAL_TRIGGER_SEQUENCE) && !pOEMPA->SetExternalTriggerSequence(root.eExternalTriggerSequence))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetExternalTriggerSequence",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if((root.ullSavedParameters & OEMPA_KEEPALIVE) && !pOEMPA->SetKeepAlive(root.eKeepAlive))
	{
		OempaApiMessageBox(NULL,L"pOEMPA->SetKeepAlive",L"HW error",MB_OK);
		bRet = false;goto end;
	}
	if(bIOBoard)
	{
		if((root.ullSavedParameters & OEMPA_ENCODER1_TYPE) && (root.ullSavedParameters & OEMPA_ENCODER2_TYPE) &&
			!pOEMPA->SetEncoderType(root.eEncoder1Type,root.eEncoder2Type))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetEncoder12Type",L"HW error",MB_OK);
			bRet = false;goto end;
		}
		if((root.ullSavedParameters & OEMPA_ENCODER1_TYPE) && !(root.ullSavedParameters & OEMPA_ENCODER2_TYPE) &&
			!pOEMPA->SetEncoderType(root.eEncoder1Type,root.eEncoder1Type))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetEncoder1Type",L"HW error",MB_OK);
			bRet = false;goto end;
		}
		if(!(root.ullSavedParameters & OEMPA_ENCODER1_TYPE) && (root.ullSavedParameters & OEMPA_ENCODER2_TYPE) &&
			!pOEMPA->SetEncoderType(root.eEncoder2Type,root.eEncoder2Type))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetEncoder2Type",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	if((root.ullSavedParameters & OEMPA_ASCAN_REQUEST) || (root.eAscanRequest!=eAscanAll))//1.1.5.0v
	{
		if(!pOEMPA->SetAscanRequest(root.eAscanRequest))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetAscanRequest",L"HW error",MB_OK);
			bRet = false;goto end;
		}
		if(!pOEMPA->SetAscanRequestFrequency(root.dAscanRequestFrequency))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetAscanRequestFrequency",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	if(root.ullSavedParameters & OEMPA_TRIGGER_HIGH_TIME)
	{
		if(!pOEMPA->SetSignalTriggerHighTime(root.dTriggerHighTime))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetSignalTriggerHighTime",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	bFullMatrixCaptureReadWrite = pOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite();
	//1.1.5.4i if(bFullMatrixCaptureReadWrite && !root.bEnableMultiHWChannel && !pOEMPA->EnableFMC(root.bEnableFMC))
	if(root.ullSavedParameters & OEMPA_ENABLE_FMC)
	{
		if(bFullMatrixCaptureReadWrite && !pOEMPA->EnableFMC(root.bEnableFMC))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->EnableFMC",L"HW error",MB_OK);
			bRet = false;goto end;
		}
		if(pOEMPA->GetSWDeviceOEMPA()->IsFMCElementStepSupported())
		{
			if(!pOEMPA->SetFMCElement(root.iFMCElementStart,root.iFMCElementStop,root.iFMCElementStep))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetFMCElement",L"HW error",MB_OK);
				bRet = false;goto end;
			}
		}else{
			if(root.iFMCElementStep!=1)
			{
				OempaApiMessageBox(NULL,L"FMCElementStep should be 1 (not supported by the FW)!",L"Error",MB_OK);
				bRet = false;goto end;
			}
			if(!pOEMPA->SetFMCElementStart(root.iFMCElementStart))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetFMCElementStart",L"HW error",MB_OK);
				bRet = false;goto end;
			}
			if(!pOEMPA->SetFMCElementStop(root.iFMCElementStop))
			{
				OempaApiMessageBox(NULL,L"pOEMPA->SetFMCElementStop",L"HW error",MB_OK);
				bRet = false;goto end;
			}
		}
	}
	//1.1.5.4i if(pOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelSupported() && !root.bEnableFMC)
	if(root.ullSavedParameters & OEMPA_ENABLE_MULTI_CHANNEL)
	{
		pOEMPA->GetSWDeviceOEMPA()->EnablePulserDuringReplay(root.bEnablePulserDuringReplay);
		if(!pOEMPA->EnableMultiHWChannel(root.bEnableMultiHWChannel))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->EnableMultiHWChannel",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}
	if((root.ullSavedParameters & OEMPA_TEMPERATURE_WARNING) && (root.ullSavedParameters & OEMPA_TEMPERATURE_ALARM))
	{
		if(!pOEMPA->SetTemperatureAlarm((BYTE&)root.cTemperatureWarning,(BYTE&)root.cTemperatureAlarm))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetTemperatureAlarm",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}

	for(int iFilterIndex=0;iFilterIndex<(int)root.uiSavedFilterCount;iFilterIndex++)
	{
		if(!pOEMPA->SetFilter((enumOEMPAFilterIndex)(eOEMPAFilter1+iFilterIndex),
						root.aFilter[iFilterIndex].wScale,root.aFilter[iFilterIndex].aCoefficient))
		{
			OempaApiMessageBox(NULL,L"pOEMPA->SetFilter",L"HW error",MB_OK);
			bRet = false;goto end;
		}
		pFilter = NULL;
		if(iFilterIndex<(int)eOEMPAFilter15)
			pFilter = &pOEMPA->GetSWDeviceOEMPA()->Filter(iFilterIndex);
		if(pFilter && !pFilter->SetTitle(root.aFilter[iFilterIndex].pTitle))
		{
			OempaApiMessageBox(NULL,L"pFilter->SetTitle",L"HW error",MB_OK);
			bRet = false;goto end;
		}
	}

	if(	(root.iSubSequenceEncoderCount>=0) && !root.iSubSequenceGateCount &&
		!pOEMPA->SetSubSequenceTableEncoder(root.iSubSequenceEncoderCount,root.aiSubSequenceCycleStart,root.aiSubSequenceCycleStop,root.iSubSequenceEncoderCount+1,root.afSubSequenceValue))
		{bRet = false;goto end;}
	else if(	!root.iSubSequenceEncoderCount && (root.iSubSequenceGateCount>0) &&
		!pOEMPA->SetSubSequenceTableGate(root.iSubSequenceGateCount,root.aiSubSequenceCycleStart,root.aiSubSequenceCycleStop,root.iSubSequenceGateCount+1,root.afSubSequenceValue))
		{bRet = false;goto end;}
	else if(!root.iSubSequenceEncoderCount && !root.iSubSequenceGateCount && !pOEMPA->DisableSubSequenceTable())
		{bRet = false;goto end;}
	if(!pOEMPA->SetSubSequenceAverage(root.iSubSequenceAverage))
		{bRet = false;goto end;}

	for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
	{
		iMultiChannelReceptionElementIndex = pReception[iCycle].adwElement.GetFirstElementIndex();
		if(!pCycle[iCycle].dTimeSlot)
			bCheck = true;
		else
			bCheck = false;
		if(!WriteHWCycle(pOEMPA,iCycle,&root,&pCycle[iCycle],iMultiChannelReceptionElementIndex))
		{
			swprintf(wcAux,MAX_PATH,L"Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;goto end;
		}
		if(bCheck && pCycle[iCycle].dTimeSlot)
		{//1.1.5.4i
			if(!root.bEnableMultiHWChannel)
			{
				if(!ReadFileStandardCheck(iCycle,root.wcFileName,&root,pCycle,pReception))
					{bRet = false;goto end;}
			}else{
				if(!ReadFileMultiHWChannelUpdate(iCycle,root.wcFileName,&root,pCycle,pReception))
					{bRet = false;goto end;}
			}
		}

		if(pOEMPA->IsMultiplexer())
		{//1.1.5.0u
			if(!CheckMultiplexerAperture(pOEMPA,iCycle,&root,&pEmission[iCycle],&pReception[iCycle]))
			{
				bRet = false;goto end;
			}
		}
		if(	(!pEmission[iCycle].adwElement.GetAllocatedSize() && !root.dAscanRequestFrequency) ||
			!WriteHWFocalLaw(pOEMPA,iCycle,true,pCycle[iCycle].bHWAcquisition,&root,&pEmission[iCycle],&pReception[iCycle]))
		{
			swprintf(wcAux,MAX_PATH,L"Emission Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;goto end;
		}
		if(	!pReception[iCycle].adwElement.GetAllocatedSize() ||
			!WriteHWFocalLaw(pOEMPA,iCycle,false,pCycle[iCycle].bHWAcquisition,&root,&pReception[iCycle],&pEmission[iCycle]))
		{
			swprintf(wcAux,MAX_PATH,L"Reception Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;goto end;
		}
		if(pOEMPA->IsMultiplexer())
		{
			//emission and reception should have the same aperture
			//write one time only the aperture
			if(pEmission[iCycle].adwElement!=pReception[iCycle].adwElement)
			{
				//1.1.5.0u
				//swprintf(wcAux,MAX_PATH,L"With a multiplexer emission and reception aperture should the same (cycle %d)",iCycle);
				//OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
				//bRet = false;goto end;
				if(!pOEMPA->SetMultiplexerEnable(iCycle,pEmission[iCycle].adwElement,pReception[iCycle].adwElement))
				{
					swprintf(wcAux,MAX_PATH,L"Multiplexer Emission/Reception Cycle %d",iCycle);
					OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
					bRet = false;goto end;
				}
			}else
			if(!pOEMPA->SetMultiplexerEnable(iCycle,pEmission[iCycle].adwElement))
			{
				swprintf(wcAux,MAX_PATH,L"Multiplexer Emission Cycle %d",iCycle);
				OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
				bRet = false;goto end;
			}
		}
	}

#ifdef _WIN64
	pFifo = pOEMPA->GetAcquisitionFifo(eFifoAscan);
	if(pFifo && pFifo->IsEnabled() && pFifo->GetAlloc(iDataCountMax, iBufferByteSize) && (root.iCycleCount>0))
	{
		bool bWarning=false,bCreate;
		if(!root.bEnableFMC)
		{
			if(root.iCycleCount>iDataCountMax)
				bWarning = true;
		}else{
			if((root.iFMCElementStep>0) && (root.iCycleCount*(root.iFMCElementStop-root.iFMCElementStart+1)/root.iFMCElementStep>iDataCountMax))
				bWarning = true;
			if((root.iFMCElementStep==0) && (root.iCycleCount*(root.iFMCElementStop-root.iFMCElementStart+1)>iDataCountMax))
				bWarning = true;
		}
		if(bWarning && CSWDevice::ReadCfgBool(L"CustomizedAPI",L"WarningFMCFifo",true,bCreate))
			UTKernel_SystemMessageBoxList(L"Warning: the ascan Fifo is not able to store a whole sequence!");
	}
#endif _WIN64
end:
	if(!root.pLockUnlock)
	{
		if(!pOEMPA->UnlockDevice(eAcqState))
		{
			OempaApiMessageBox(NULL,L"Communication Error!",L"Error",MB_OK);
			bRet = false;
		}
	}else{
		if(!(*root.pLockUnlock)(&root,false,eAcqState))
		{
			OempaApiMessageBox(NULL,L"Communication Error!",L"Error",MB_OK);
			bRet = false;
		}
	}

	if(bRet && pCallbackCustomizedAPI)
		(*pCallbackCustomizedAPI)(pOEMPA,root.wcFileName,eWriteHW_Leave,&root,pCycle,pEmission,pReception);

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
void ReportFileError(wchar_t *wcSection, wchar_t *wcKey)
{
	wchar_t msg[MAX_PATH];

	swprintf(msg,MAX_PATH,L"Error the key \"%s\" in section \"%s\" is wrong!",wcKey,wcSection);
	OempaApiMessageBox(NULL,msg,L"File Error",MB_OK);
}

bool ReadFileTextFilter(int iFilterIndex,const wchar_t *pFileName,structRoot *pRoot)
{
	wchar_t filter[MAX_PATH];
	bool bRet=true;
	int iCount;

	if(!pRoot->rParamList)
		return false;
	swprintf(filter,MAX_PATH,L"Filter:%d",iFilterIndex);

	if(!pRoot->rParamList->FileReadString(filter,L"Title",pFileName,pRoot->aFilter[iFilterIndex].pTitle,g_iFilterTitleLength))
	{
		ReportFileError(filter, L"Title");
		return false;
	}
	if(!pRoot->rParamList->FileReadWord(filter,L"Scale",pFileName,pRoot->aFilter[iFilterIndex].wScale))
	{
		ReportFileError(filter, L"Scale");
		return false;
	}
	if(!pRoot->rParamList->FileReadInteger(filter,L"Coefficient.count",pFileName,iCount))
	{
		ReportFileError(filter, L"Coefficient.count");
		bRet = false;
	}
	if(iCount != g_iOEMPAFilterCoefficientMax)
		bRet = false;
	if(!pRoot->rParamList->FileReadArrayShort(filter,L"Coefficient",pFileName,g_iOEMPAFilterCoefficientMax,&pRoot->aFilter[iFilterIndex].aCoefficient[0],iCount))
	{
		ReportFileError(filter, L"Coefficient");
		bRet = false;
	}

	if(bRet)
		pRoot->uiSavedFilterCount++;

	return bRet;
}

bool ReadFileTextCycleGate(int iCycle,int iGate,const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle)
{
	wchar_t cycle[MAX_PATH];
	structGate *pGate=&pCycle->gate[iGate];
	bool bRet=true;
	int iEnum;

	if(!pRoot->rParamList)
		return false;
	swprintf(cycle,MAX_PATH,L"Cycle:%d\\Gate:%d",iCycle,iGate);

	pGate->bEnable = true;
	if(!pRoot->rParamList->FileReadBoolean(cycle,L"Enable",pFileName,pGate->bEnable,false))
	{
		//by default gates are disabled but after one first setup
		//they can be enabled!
		pGate->bEnable = false;
		pGate->eModeAmp = (enumGateModeAmp)0;
		pGate->eModeTof = (enumGateModeTof)0;
		pGate->eRectification = (enumRectification)0;
		pGate->dThreshold = 0.0;
		pGate->bTrackingStartEnable = false;
		pGate->iTrackingStartIndexCycle = 0;
		pGate->iTrackingStartIndexGate = 0;
		pGate->bTrackingStopEnable = false;
		pGate->iTrackingStopIndexCycle = 0;
		pGate->iTrackingStopIndexGate = 0;
		return bRet;
	}
	if(!pGate->bEnable)
		return true;

	if(!pRoot->rParamList->FileReadDouble(cycle,L"Start",pFileName,1.0e-6,pGate->dStart))
	{
		ReportFileError(cycle, L"Start");
		return false;
	}
	if(!pRoot->rParamList->FileReadDouble(cycle,L"Stop",pFileName,1.0e-6,pGate->dStop))
	{
		ReportFileError(cycle, L"Stop");
		return false;
	}
	if(!pRoot->rParamList->FileReadDouble(cycle,L"Threshold",pFileName,1.0,pGate->dThreshold))
	{
		ReportFileError(cycle, L"Threshold");
		return false;
	}
	if(!pRoot->rParamList->FileReadEnum(cycle,L"Rectification",pFileName,L"Signed"L"\0"L"Unsigned"L"\0"L"UnsignedPositive"L"\0"L"UnsignedNegative"L"\0",iEnum))
	{
		ReportFileError(cycle, L"Rectification");
		bRet = false;goto end;
	}
	pGate->eRectification = (enumRectification)iEnum;
	if(!pRoot->rParamList->FileReadEnum(cycle,L"ModeAmp",pFileName,L"Absolute"L"\0"L"Maximum"L"\0"L"Minimum"L"\0"L"PeakToPeak"L"\0",iEnum))
	{
		ReportFileError(cycle, L"ModeAmp");
		bRet = false;goto end;
	}
	pGate->eModeAmp = (enumGateModeAmp)iEnum;
	if(!pRoot->rParamList->FileReadEnum(cycle,L"ModeTof",pFileName,L"AmplitudeDetection"L"\0"L"ThresholdCross"L"\0"L"ZeroFirstAfterThresholdCross"L"\0"L"ZeroLastBeforeThresholdCross"L"\0",iEnum))
	{
		ReportFileError(cycle, L"ModeTof");
		bRet = false;goto end;
	}
	pGate->eModeTof = (enumGateModeTof)iEnum;

	if(!pRoot->rParamList->FileReadWord(cycle,L"AcqIDAmp",pFileName,pGate->wAcqIDAmp,false))
		pGate->wAcqIDAmp = 0xFFFF;
	if(!pRoot->rParamList->FileReadWord(cycle,L"AcqIDTof",pFileName,pGate->wAcqIDTof,false))
		pGate->wAcqIDTof = 0xFFFF;

	if(pRoot->rParamList->FileReadBoolean(cycle,L"TrackingStartEnable",pFileName,pGate->bTrackingStartEnable,false))
	{
		pRoot->ullSavedParameters |= OEMPA_TRACKING;
		if(!pRoot->rParamList->FileReadInteger(cycle,L"TrackingStartIndexCycle",pFileName,pGate->iTrackingStartIndexCycle))
		{
			ReportFileError(cycle, L"TrackingStartIndexCycle");
			bRet = false;
		}
		if(!pRoot->rParamList->FileReadInteger(cycle,L"TrackingStartIndexGate",pFileName,pGate->iTrackingStartIndexGate))
		{
			ReportFileError(cycle, L"TrackingStartIndexGate");
			bRet = false;
		}
	}else{
		pGate->bTrackingStartEnable = false;
		pGate->iTrackingStartIndexCycle = 0;
		pGate->iTrackingStartIndexGate = 0;
	}
	if(pRoot->rParamList->FileReadBoolean(cycle,L"TrackingStopEnable",pFileName,pGate->bTrackingStopEnable,false))
	{
		pRoot->ullSavedParameters |= OEMPA_TRACKING;
		if(!pRoot->rParamList->FileReadInteger(cycle,L"TrackingStopIndexCycle",pFileName,pGate->iTrackingStopIndexCycle))
		{
			ReportFileError(cycle, L"TrackingStopIndexCycle");
			bRet = false;
		}
		if(!pRoot->rParamList->FileReadInteger(cycle,L"TrackingStopIndexGate",pFileName,pGate->iTrackingStopIndexGate))
		{
			ReportFileError(cycle, L"TrackingStopIndexGate");
			bRet = false;
		}
	}else{
		pGate->bTrackingStopEnable = false;
		pGate->iTrackingStopIndexCycle = 0;
		pGate->iTrackingStopIndexGate = 0;
	}

end:
	return bRet;
}

bool ReadFileTextCycle(int iCycle,const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle)
{
	wchar_t cycle[MAX_PATH];
	bool bRet=true;
	int iEnum,iDACCountSlope,iDACCountGain,iDACCountAux;

	if(!pRoot->rParamList)
		return false;
	swprintf(cycle,MAX_PATH,L"Cycle:%d",iCycle);

	if(!pRoot->rParamList->FileReadDouble(cycle,L"GainDigital",pFileName,1.0,pCycle->dGainDigital))//dB
	{
		ReportFileError(cycle, L"GainDigital");
		bRet = false;
	}
	
	pCycle->bHWAcquisition = true;
	pCycle->fBeamCorrection = 0.0f;
	if(pRoot->bEnableMultiHWChannel)
	{
		if(!pRoot->rParamList->FileReadBoolean(cycle,L"HWAcquisition",pFileName,pCycle->bHWAcquisition,false))
			pRoot->rParamList->FileReadBoolean(cycle,L"MultiChannelAcquisition",pFileName,pCycle->bHWAcquisition,false);
	}
	if(!pRoot->rParamList->FileReadFloat(cycle,L"BeamCorrection",pFileName,1.0f,pCycle->fBeamCorrection))//dB
	{
		ReportFileError(cycle, L"BeamCorrection");
		return false;
	}
	
	pCycle->dStart = 0.0;
	if(!pRoot->bEnableMultiHWChannel && !pRoot->rParamList->FileReadDouble(cycle,L"Start",pFileName,1.0e-6,pCycle->dStart))
	{
		ReportFileError(cycle, L"Start");
		return false;
	}
	
	if(!pRoot->rParamList->FileReadDouble(cycle,L"Range",pFileName,1.0e-6,pCycle->dRange))
	{
		ReportFileError(cycle, L"Range");
		return false;
	}
	if(pRoot->rParamList->FileReadEnum(cycle,L"CompressionType",pFileName,L"Compression"L"\0"L"Decimation"L"\0",iEnum,false))
	{
		pRoot->ullSavedParameters |= OEMPA_CYCLE_POINTCOUNT_AND_COMPRESSION;
		pCycle->eCompressionType = (enumCompressionType)iEnum;
	}
	if(pRoot->rParamList->FileReadLong(cycle,L"PointCount",pFileName,pCycle->lPointCount,false))
	{
		pRoot->ullSavedParameters |= OEMPA_CYCLE_POINTCOUNT_AND_COMPRESSION;
	}else{
		pCycle->lPointCount = 0;
		if(pRoot->rParamList->FileReadLong(cycle,L"PointFactor",pFileName,pCycle->lPointFactor,false))
		{
			pRoot->ullSavedParameters |= OEMPA_CYCLE_POINTFACTOR_AND_COMPRESSION;
		}else
			pCycle->lPointFactor = 0;
	}
	
	if(!pRoot->bEnableMultiHWChannel)
	{
		if(!pRoot->rParamList->FileReadDouble(cycle,L"TimeSlot",pFileName,1.0e-6,pCycle->dTimeSlot))
		{
			ReportFileError(cycle, L"TimeSlot");
			return false;
		}
		if(pRoot->bEnableFMC)
		{
			if(!pRoot->rParamList->FileReadDouble(cycle,L"FMCSubTimeSlotAcq",pFileName,1.0e-6,pCycle->dFMCSubTimeSlotAcq,false))
				pCycle->dFMCSubTimeSlotAcq = -1.0;
			if(!pRoot->rParamList->FileReadDouble(cycle,L"FMCSubTimeSlotReplay",pFileName,1.0e-6,pCycle->dFMCSubTimeSlotReplay,false))
				pCycle->dFMCSubTimeSlotReplay = -1.0;
		}
	}else{
		if(pCycle->bHWAcquisition)
		{
			if(!pRoot->rParamList->FileReadDouble(cycle,L"HWAcquisitionTime",pFileName,1.0e-6,pCycle->dTimeSlot,false))
			{
				if(!pRoot->rParamList->FileReadDouble(cycle,L"TimeSlot",pFileName,1.0e-6,pCycle->dTimeSlot))
				{
					ReportFileError(cycle, L"TimeSlot");
					return false;
				}
			}
		}else{
			if(!pRoot->rParamList->FileReadDouble(cycle,L"HWReplayTime",pFileName,1.0e-6,pCycle->dTimeSlot,false))
			{
				if(!pRoot->rParamList->FileReadDouble(cycle,L"TimeSlot",pFileName,1.0e-6,pCycle->dTimeSlot))
				{
					ReportFileError(cycle, L"TimeSlot");
					return false;
				}
			}
		}
	}
	
	if(!pRoot->rParamList->FileReadEnum(cycle,L"Rectification",pFileName,L"Signed"L"\0"L"Unsigned"L"\0"L"UnsignedPositive"L"\0"L"UnsignedNegative"L"\0",iEnum))
	{
		ReportFileError(cycle, L"Rectification");
		bRet = false;goto end;
	}
	pCycle->eRectification = (enumRectification)iEnum;

	if(!pRoot->rParamList->FileReadBoolean(cycle,L"Maximum",pFileName,pCycle->bMaximum,false))
		pCycle->bMaximum = true;
	if(!pRoot->rParamList->FileReadBoolean(cycle,L"Minimum",pFileName,pCycle->bMinimum,false))
		pCycle->bMinimum = false;
	if(!pRoot->rParamList->FileReadBoolean(cycle,L"Saturation",pFileName,pCycle->bSaturation,false))
		pCycle->bSaturation = false;

	if(!pRoot->rParamList->FileReadWord(cycle,L"AcqIdChannelProbe",pFileName,pCycle->wAcqIdChannelProbe,false))
		pCycle->wAcqIdChannelProbe = 0xffff;
	if(!pRoot->rParamList->FileReadWord(cycle,L"AcqIdChannelScan",pFileName,pCycle->wAcqIdChannelScan,false))
		pCycle->wAcqIdChannelScan = 0xffff;
	if(!pRoot->rParamList->FileReadWord(cycle,L"AcqIdChannelCycle",pFileName,pCycle->wAcqIdChannelCycle,false))
		pCycle->wAcqIdChannelCycle = 0xffff;

	if(!pRoot->rParamList->FileReadInteger(cycle,L"DACTof.count",pFileName,pCycle->iDACCount,false))
		pCycle->iDACCount = 0;
	if(!pRoot->rParamList->FileReadInteger(cycle,L"DACSlope.count",pFileName,iDACCountSlope,false))
		iDACCountSlope = 0;
	if(!pRoot->rParamList->FileReadInteger(cycle,L"DACGain.count",pFileName,iDACCountGain,false))
		iDACCountGain = 0;
	if(pCycle->iDACCount && ((iDACCountSlope!=pCycle->iDACCount) && (iDACCountGain!=pCycle->iDACCount)))
		{bRet = false;goto end;}

	iDACCountAux = pCycle->iDACCount;
	if(pCycle->iDACCount && !pRoot->rParamList->FileReadArrayDouble(cycle,L"DACTof",pFileName,pCycle->iDACCount,1.0e-6,pCycle->adDACTof,iDACCountAux))
	{
		ReportFileError(cycle, L"DACTof");
		bRet = false;goto end;
	}
	if(pCycle->iDACCount==iDACCountSlope)
	{
		pCycle->bDACSlope = true;
		if(pCycle->iDACCount && !pRoot->rParamList->FileReadArrayFloat(cycle,L"DACSlope",pFileName,pCycle->iDACCount,1.0e6f,pCycle->afDACPrm,iDACCountSlope))
		{
			ReportFileError(cycle, L"DACSlope");
			bRet = false;goto end;
		}
	}else
	if(pCycle->iDACCount==iDACCountGain)
	{
		pCycle->bDACSlope = false;
		if(pCycle->iDACCount && !pRoot->rParamList->FileReadArrayFloat(cycle,L"DACGain",pFileName,pCycle->iDACCount,1.0f,pCycle->afDACPrm,iDACCountGain))
		{
			ReportFileError(cycle, L"DACGain");
			bRet = false;goto end;
		}
		pRoot->rParamList->FileReadBoolean(cycle,L"DACAutoStop",pFileName,pCycle->bDACAutoStop,false);
	}
	if(!pRoot->rParamList->FileReadBoolean(cycle,L"DACEnable",pFileName,pCycle->bDACEnable,false))
		pCycle->bDACEnable = false;

	if(!pRoot->rParamList->FileReadInteger(cycle,L"FilterIndex",pFileName,pCycle->iFilterIndex,true,0,15))
		bRet = false;
	if(!pRoot->bEnableMultiHWChannel || (pRoot->bEnableMultiHWChannel && pCycle->bHWAcquisition))
	{
		if(!pRoot->rParamList->FileReadFloat(cycle,L"GainAnalog",pFileName,1.0f,pCycle->fGainAnalog))//dB
			return false;
	}else if(pRoot->bEnableMultiHWChannel && (iCycle>0))
		pCycle->fGainAnalog = pCycle[-1].fGainAnalog;//1.1.5.4g same than the previous value to avoid spike.

	if(!pRoot->rParamList->FileReadInteger(cycle,L"GateCount",pFileName,pCycle->iGateCount,true,0,4))
	{
		ReportFileError(cycle, L"GateCount");
		bRet = false;goto end;
	}
	for(int iGateIndex=0;iGateIndex<g_iOEMPAGateCountMax;iGateIndex++)
	{//set all gates even if one of them is not used because the previous SW could used it, so it is required to disable it.
		if(!ReadFileTextCycleGate(iCycle,iGateIndex,pFileName,pRoot,pCycle))
		{
			OempaApiMessageBox(NULL,L"Gate",L"File Error",MB_OK);
			bRet = false;goto end;
		}
		if(iGateIndex>=pCycle->iGateCount)
			pCycle->gate[iGateIndex].bEnable = false;
	}

	if(pRoot->rParamList->FileReadBoolean(cycle,L"TrackingAscanEnable",pFileName,pCycle->bTrackingAscanEnable,false))
	{
		pRoot->ullSavedParameters |= OEMPA_TRACKING;
		if(!pRoot->rParamList->FileReadInteger(cycle,L"TrackingAscanIndexCycle",pFileName,pCycle->iTrackingAscanIndexCycle))
		{
			ReportFileError(cycle, L"TrackingAscanIndexCycle");
			bRet = false;
		}
		if(!pRoot->rParamList->FileReadInteger(cycle,L"TrackingAscanIndexGate",pFileName,pCycle->iTrackingAscanIndexGate))
		{
			ReportFileError(cycle, L"TrackingAscanIndexGate");
			bRet = false;
		}
	}else{
		pCycle->bTrackingAscanEnable = false;
		pCycle->iTrackingAscanIndexCycle = 0;
		pCycle->iTrackingAscanIndexGate = 0;
	}
	if(pRoot->rParamList->FileReadBoolean(cycle,L"TrackingDacEnable",pFileName,pCycle->bTrackingDacEnable,false))
	{
		pRoot->ullSavedParameters |= OEMPA_TRACKING;
		if(!pRoot->rParamList->FileReadInteger(cycle,L"TrackingDacIndexCycle",pFileName,pCycle->iTrackingDacIndexCycle))
		{
			ReportFileError(cycle, L"TrackingDacIndexCycle");
			bRet = false;
		}
		if(!pRoot->rParamList->FileReadInteger(cycle,L"TrackingDacIndexGate",pFileName,pCycle->iTrackingDacIndexGate))
		{
			ReportFileError(cycle, L"TrackingDacIndexGate");
			bRet = false;
		}
	}else{
		pCycle->bTrackingDacEnable = false;
		pCycle->iTrackingDacIndexCycle = 0;
		pCycle->iTrackingDacIndexGate = 0;
	}

end:
	return bRet;
}

bool ReadFileTextMultiHWChannel(int iCycle,bool bPulse,bool bHWAcquisition,const wchar_t *pFileName,structRoot *pRoot,CFocalLaw *pFocalLaw);
bool ReadFileTextFocalLaw(int iCycle,bool bPulse,bool bHWAcquisition,const wchar_t *pFileName,structRoot *pRoot,CFocalLaw *pFocalLaw)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t wcAux2[MAX_PATH];
	wchar_t cycle[MAX_PATH];
	bool bRet=true;
	int aiElement[2*g_iOEMPAApertureElementCountMax];
	int iEnum,iElementCount;
	int iElem,iDelayCount;

	if(!pRoot->rParamList)
		return false;
	if(pRoot->bEnableMultiHWChannel)
		return ReadFileTextMultiHWChannel(iCycle,bPulse,bHWAcquisition,pFileName,pRoot,pFocalLaw);

	if(bPulse)
		swprintf(cycle,MAX_PATH,L"Cycle:%d\\Pulser",iCycle);
	else
		swprintf(cycle,MAX_PATH,L"Cycle:%d\\Receiver",iCycle);

	if(!pRoot->rParamList->FileReadDouble(cycle,L"WedgeDelay",pFileName,1.0e-6,pFocalLaw->dWedgeDelay))
	{
		ReportFileError(cycle, L"WedgeDelay");
		bRet = false;goto end;
	}
	if(!pRoot->rParamList->FileReadFloat(cycle,L"CenterDelay",pFileName,1.0e-6f,pFocalLaw->fCenterDelay,false))
		pFocalLaw->fCenterDelay = 0.0f;
	
	if(!pRoot->rParamList->FileReadInteger(cycle,L"Element.count",pFileName,pFocalLaw->iElementCount))
	{
		ReportFileError(cycle, L"Element.count");
		bRet = false;goto end;
	}
	
	if(!pFocalLaw->iElementCount)
	{
		if(!pFocalLaw->GetDWORDElementSize())
			pFocalLaw->SetAllocatedSize(pRoot->iOEMPADDFCountMax,pRoot->iOEMPAProbeCountMax);
		if(!CHWDeviceOEMPA::SetAllElementEnable(false,pFocalLaw->adwElement))
			return false;
		pFocalLaw->iFocalCount = 0;
		pFocalLaw->eFocusing = eStandard;
		pFocalLaw->iPrmCount = 0;
		return bRet;
	}

	iElementCount = (int)pFocalLaw->iElementCount;
	if(iElementCount && !pRoot->rParamList->FileReadArrayInteger(cycle,L"Element",pFileName,pFocalLaw->iElementCount,aiElement,iElementCount))
	{
		//OempaApiMessageBox(NULL,L"Element",L"File Error",MB_OK);
		bRet = false;goto end;
	}

	if(!pFocalLaw->GetDWORDElementSize())
		pFocalLaw->SetAllocatedSize(pRoot->iOEMPADDFCountMax,pRoot->iOEMPAProbeCountMax);
	if(!CHWDeviceOEMPA::SetAllElementEnable(false,pFocalLaw->adwElement))
		{bRet = false;goto end;}
	for(int i=0;i<iElementCount;i++)
	{
		iElem = aiElement[i];
		CHWDeviceOEMPA::SetElementEnable(iElem,true,pFocalLaw->adwElement);
		/*following code is the same 
		if(iElem>=32)
		{
			dwElem = 1<<(iElem-32);
			adwHWAperture[0] |= dwElem;
		}else{
			dwElem = 1<<iElem;
			adwHWAperture[1] |= dwElem;
		}*/
	}

	if(bPulse)
		pFocalLaw->eFocusing = eStandard;
	else{
		if(!pRoot->rParamList->FileReadEnum(cycle,L"Focusing",pFileName,L"Standard"L"\0"L"DDF"L"\0",iEnum))
		{
			ReportFileError(cycle, L"Focusing");
			bRet = false;goto end;
		}
		pFocalLaw->eFocusing = (enumFocusing)iEnum;
	}

	pFocalLaw->iFocalCount = 1;
	if(!bPulse && (pFocalLaw->eFocusing==eDDF))
	{
		if(!pRoot->rParamList->FileReadInteger(cycle,L"FocalTimeOfFlight.count",pFileName,pFocalLaw->iFocalCount))
		{
			ReportFileError(cycle, L"FocalTimeOfFlight.count");
			bRet = false;goto end;
		}
	}

	swprintf(wcAux,MAX_PATH,L"%d;%d",iElementCount,pFocalLaw->iFocalCount);
	if(!pRoot->rParamList->FileReadString(cycle,L"Delay.count",pFileName,wcAux2,MAX_PATH))
	{
		ReportFileError(cycle, L"Delay.count");
		bRet = false;goto end;
	}
	if(wcscmp(wcAux,wcAux2))
	{
		OempaApiMessageBox(NULL,L"Delay.count",L"File Error",MB_OK);
		bRet = false;goto end;
	}

	iDelayCount = iElementCount*pFocalLaw->iFocalCount;
	if(!pRoot->rParamList->FileReadArrayFloat2D(cycle,L"Delay",pFileName,pFocalLaw->iElementCount,pFocalLaw->iFocalCount,1.0e-6f,pFocalLaw->afDelay,iDelayCount))
	{
		//OempaApiMessageBox(NULL,L"Delay",L"File Error",MB_OK);
		bRet = false;goto end;
	}
	if(!pFocalLaw->afDelay.ReorderDelay(iElementCount,aiElement))
		{bRet = false;goto end;}
	pFocalLaw->iDelayCount = iElementCount*pFocalLaw->iFocalCount;
	if(iElementCount>pFocalLaw->GetCountElement())
	{
		OempaApiMessageBox(NULL,L"Aperture: too many elements!",L"File Error",MB_OK);
		bRet = false;goto end;
	}
	pFocalLaw->iPrmCount = iElementCount;
	if(bPulse)
	{
		swprintf(wcAux,MAX_PATH,L"%d",iElementCount);
		if(!pRoot->rParamList->FileReadString(cycle,L"Width.count",pFileName,wcAux2,MAX_PATH))
		{
			ReportFileError(cycle, L"Width.count");
			bRet = false;goto end;
		}
		if(wcscmp(wcAux,wcAux2))
		{
			OempaApiMessageBox(NULL,L"Width.count",L"File Error",MB_OK);
			bRet = false;goto end;
		}
		if(!pRoot->rParamList->FileReadArrayFloat(cycle,L"Width",pFileName,pFocalLaw->iPrmCount,1.0e-6f,pFocalLaw->afPrm,pFocalLaw->iPrmCount))
		{
			//OempaApiMessageBox(NULL,L"Width",L"File Error",MB_OK);
			bRet = false;goto end;
		}
	}else{
		if(pRoot->bEnableFMC)
			pFocalLaw->iFocalCount = 1;
		else{//1.1.5.1 FMC
			swprintf(wcAux,MAX_PATH,L"%d",pFocalLaw->iFocalCount);
			if(!pRoot->rParamList->FileReadString(cycle,L"FocalTimeOfFlight.count",pFileName,wcAux2,MAX_PATH))
			{
				ReportFileError(cycle,L"FocalTimeOfFlight.count");
				bRet = false;goto end;
			}
			if(wcscmp(wcAux,wcAux2))
			{
				OempaApiMessageBox(NULL,L"FocalTimeOfFlight.count",L"File Error",MB_OK);
				bRet = false;goto end;
			}
			if(!pRoot->rParamList->FileReadArrayDouble(cycle,L"FocalTimeOfFlight",pFileName,pFocalLaw->iFocalCount,1.0e-6,pFocalLaw->adFocalTimeOfFlight,pFocalLaw->iFocalCount))
			{
				//OempaApiMessageBox(NULL,L"FocalTimeOfFlight",L"File Error",MB_OK);
				bRet = false;goto end;
			}
		}
		swprintf(wcAux,MAX_PATH,L"%d",iElementCount);
		if(!pRoot->rParamList->FileReadString(cycle,L"Gain.count",pFileName,wcAux2,MAX_PATH))
		{
			ReportFileError(cycle, L"Gain.count");
			bRet = false;goto end;
		}
		if(wcscmp(wcAux,wcAux2))
		{
			OempaApiMessageBox(NULL,L"Gain.count",L"File Error",MB_OK);
			{bRet = false;goto end;}
		}
		if(!pRoot->rParamList->FileReadArrayFloat(cycle,L"Gain",pFileName,pFocalLaw->iPrmCount,1.0f,pFocalLaw->afPrm,pFocalLaw->iPrmCount))
		{
			//OempaApiMessageBox(NULL,L"Gain",L"File Error",MB_OK);
			bRet = false;goto end;
		}
	}

end:
	return bRet;
}

bool ReadFileTextMultiHWChannel(int iCycle,bool bPulse,bool bHWAcquisition,const wchar_t *pFileName,structRoot *pRoot,CFocalLaw *pFocalLaw)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t wcAux2[MAX_PATH];
	wchar_t cycle[MAX_PATH];
	bool bRet=true;
	int aiElement[2*g_iOEMPAApertureElementCountMax];
	int iElementCount;
	int iElem,iDelayCount,iStartFirst;
	int aiDecimation[g_iOEMPAApertureElementCountMax],iDecimationCount;
	int iIndexElement,iDecimation,iIndex2;
	unionDecimation *pX;

	if(!pRoot->rParamList)
		return false;
	if(!pRoot->bEnableMultiHWChannel)
		return false;

	pFocalLaw->eFocusing = eStandard;
	pFocalLaw->iFocalCount = 1;
	pFocalLaw->iPrmCount = 0;

	if(bPulse)
		swprintf(cycle,MAX_PATH,L"Cycle:%d\\Pulser",iCycle);
	else
		swprintf(cycle,MAX_PATH,L"Cycle:%d\\Receiver",iCycle);

	if(!pRoot->rParamList->FileReadInteger(cycle,L"Element.count",pFileName,pFocalLaw->iElementCount,false))
		pFocalLaw->iElementCount = 0;

	iElementCount = (int)pFocalLaw->iElementCount;
	if(iElementCount && !pRoot->rParamList->FileReadArrayInteger(cycle,L"Element",pFileName,pFocalLaw->iElementCount,aiElement,iElementCount))
	{
		//OempaApiMessageBox(NULL,L"Element",L"File Error",MB_OK);
		bRet = false;goto end;
	}

	if(!pFocalLaw->GetDWORDElementSize())
		pFocalLaw->SetAllocatedSize(pRoot->iOEMPADDFCountMax,pRoot->iOEMPAProbeCountMax);
	if(!CHWDeviceOEMPA::SetAllElementEnable(false,pFocalLaw->adwElement))
		{bRet = false;goto end;}
	for(int i=0;i<iElementCount;i++)
	{
		iElem = aiElement[i];
		iIndexElement = iElem;
		CHWDeviceOEMPA::SetElementEnable(iElem,true,pFocalLaw->adwElement);
	}

	if(!bPulse && bHWAcquisition)
	{
		if(pFocalLaw->iElementCount>1)
		{
			OempaApiMessageBox(NULL,L"Too many reception elements!",L"File Error",MB_OK);
			bRet = false;goto end;
		}
		if(!pRoot->rParamList->FileReadInteger(cycle,L"StartFirst",pFileName,iStartFirst,false))
			iStartFirst = 0;
		if(!pRoot->rParamList->FileReadInteger(cycle,L"Start.count",pFileName,iDelayCount))
		{
			ReportFileError(cycle, L"Start.count");
			bRet = false;goto end;
		}
		pFocalLaw->iDelayCount = iDelayCount;
		if(pFocalLaw->iDelayCount+iStartFirst>pRoot->iOEMPAApertureCountMax)
		{
			OempaApiMessageBox(NULL,L"Start too many elements!",L"File Error",MB_OK);
			bRet = false;goto end;
		}
		if(!pRoot->rParamList->FileReadArrayFloat2D(cycle,L"Start",pFileName,pFocalLaw->iDelayCount,pFocalLaw->iFocalCount,1.0e-6f,pFocalLaw->afDelay,iDelayCount))
		{
			//OempaApiMessageBox(NULL,L"Start",L"File Error",MB_OK);
			bRet = false;goto end;
		}
		if(iStartFirst>0)
		{
			for(int iIndex=pFocalLaw->iDelayCount-1;iIndex>=0;iIndex--)
				pFocalLaw->afDelay[0][iIndex+iStartFirst] = pFocalLaw->afDelay[0][iIndex];
			for(int iIndex=0;iIndex<iStartFirst;iIndex++)
				pFocalLaw->afDelay[0][iIndex] = 0.0f;
			pFocalLaw->iDelayCount += iStartFirst;
		}
		memset(&pFocalLaw->hwAcqDecimation,0,sizeof(unionOEMMCDecimation));
		iDecimationCount = iDelayCount;
		if(iDelayCount>0)
		{
			if(!pRoot->rParamList->FileReadArrayInteger(cycle,L"Decimation",pFileName,iDelayCount,aiDecimation,iDecimationCount))
			{
				//swprintf(wcAux,MAX_PATH,L"\"Decimation\" count should be %d (same than \"Start.count\") not %d.",iDelayCount,iDecimationCount);
				//OempaApiMessageBox(NULL,wcAux,L"File Error",MB_OK);
				if(iDecimationCount)
					{bRet = false;goto end;}
			}
			pRoot->ullSavedParameters |= OEMMC_DECIMATION;
			for(int iIndex=0;iIndex<iDecimationCount;iIndex++)
			{
				if((aiDecimation[iIndex]<0) || (15<aiDecimation[iIndex]))
				{
					swprintf(wcAux,MAX_PATH,L"Decimation value %d out of range 0 to 15.",aiDecimation[iIndex]);
					OempaApiMessageBox(NULL,wcAux,L"File Error",MB_OK);
					bRet = false;goto end;
				}
				if((0<=aiDecimation[iIndex]) && (aiDecimation[iIndex]<16))
				{
					iIndex2 = iIndex+iStartFirst;
					pX = (unionDecimation*)&pFocalLaw->hwAcqDecimation.byte[iIndex2/2];
					if(!(iIndex2%2))
						pX->data.lsb = aiDecimation[iIndex];
					else
						pX->data.msb = aiDecimation[iIndex];
				}
			}
		}
	}
	if(iElementCount>pFocalLaw->GetCountElement())
	{
		OempaApiMessageBox(NULL,L"Aperture: too many elements!",L"File Error",MB_OK);
		bRet = false;goto end;
	}
	if(!bPulse && bHWAcquisition)
	{
		//check that the acquisition element has no decimation.
		pX = (unionDecimation*)&pFocalLaw->hwAcqDecimation.byte[iIndexElement/2];
		if(!(iIndexElement%2))
			iDecimation = pX->data.lsb;
		else
			iDecimation = pX->data.msb;
		if(iDecimation>0)
		{
			swprintf(wcAux,MAX_PATH,L"Decimation of the acquisition cycle element should be 0!");
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;goto end;
		}
	}
	if(bPulse)
	{//1.1.5.3u Start=>Delay for reception, still Delays are pulser are free.
		if((!bHWAcquisition && (iElementCount>0)) || (bHWAcquisition && !iElementCount))
			pRoot->bEnablePulserDuringReplay = true;//1.1.5.3u
		if(iElementCount>0)//1.1.5.3u if(bHWAcquisition && bPulse)
		{
			pFocalLaw->iPrmCount = iElementCount;
			swprintf(wcAux,MAX_PATH,L"%d",iElementCount);
			if(!pRoot->rParamList->FileReadString(cycle,L"Width.count",pFileName,wcAux2,MAX_PATH))
			{
				ReportFileError(cycle, L"Width.count");
				bRet = false;goto end;
			}
			if(wcscmp(wcAux,wcAux2))
			{
				OempaApiMessageBox(NULL,L"Width.count",L"File Error",MB_OK);
				bRet = false;goto end;
			}
			if(!pRoot->rParamList->FileReadArrayFloat(cycle,L"Width",pFileName,pFocalLaw->iPrmCount,1.0e-6f,pFocalLaw->afPrm,pFocalLaw->iPrmCount))
			{
				//OempaApiMessageBox(NULL,L"Width",L"File Error",MB_OK);
				bRet = false;goto end;
			}
		}
		if(pRoot->rParamList->FileReadString(cycle,L"Delay.count",pFileName,wcAux2,MAX_PATH,false))
		{
			swprintf(wcAux,MAX_PATH,L"%d;1",iElementCount);
			if(wcscmp(wcAux,wcAux2))
			{
				OempaApiMessageBox(NULL,L"Delay.count",L"File Error",MB_OK);
				bRet = false;goto end;
			}
			iDelayCount = iElementCount;
			if(!pRoot->rParamList->FileReadArrayFloat2D(cycle,L"Delay",pFileName,pFocalLaw->iElementCount,pFocalLaw->iFocalCount,1.0e-6f,pFocalLaw->afDelay,iDelayCount))
			{
				ReportFileError(cycle, L"Delay");
				bRet = false;goto end;
			}else{
				if(!pFocalLaw->afDelay.ReorderDelay(iElementCount,aiElement))
					{bRet = false;goto end;}
				pFocalLaw->iDelayCount = iElementCount;
			}
			if(bHWAcquisition)
				pRoot->bEnablePulserDuringReplay = true;//otherwsie the delay is not taken into account.
		}else{
			for(int iElementIndex=0;iElementIndex<iElementCount;iElementIndex++)
				pFocalLaw->afDelay[0][iElementIndex] = 0.0f;
		}
	}

end:
	return bRet;
}

bool ReadFileStandardCheck(int iCycle,const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pFocalLaw)
{
	double dRange,dStart,dTimeSlot,dStop;
	wchar_t wcAux[MAX_PATH];

	dRange = pCycle[iCycle].dRange;
	if(!pFocalLaw[iCycle].iElementCount)
		return true;
	if(pCycle[iCycle].bTrackingAscanEnable)
		return true;//Because of the tracking, it is impossible to check the range!
	dStart = pCycle[iCycle].dStart;
	dTimeSlot = pCycle[iCycle].dTimeSlot;
	//limitation of the FW recovery time
	dStop = dStart+pCycle[iCycle].dRange+pRoot->dFWAscanRecoveryTime;
	if(dTimeSlot && (dStop>dTimeSlot))//1.1.5.4i if(dStop>dTimeSlot)
	{
		swprintf(wcAux,MAX_PATH,L"Bad TimeSlot/Range for the cycle %d",iCycle);
		OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
		return false;//inconsistent range.
	}
	return true;
}

bool GetMultiHWChannelCheck(int iCycle,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pFocalLaw, CFocalLaw* &pFocalLawAcquisition, int &iCycleAcquisition, int &iIndexAp2, int &iIndexElem2)
{
	//int iElementCount;
	//int aiElement[g_iOEMPAApertureElementCountMax]={0};
	DWORD dwEnable;

	iIndexAp2 = iIndexElem2 = -1;
	if(!iCycle)
	{
		pFocalLawAcquisition = NULL;
		iCycleAcquisition = -1;
		if(!pCycle[iCycle].bHWAcquisition && !pRoot->bEnablePulserDuringReplay)
		{
			OempaApiMessageBox(NULL,L"MultiChannel Error: the first cycle should be an acquisition!",L"File Error",MB_OK);
			return false;
		}
	}
	if(pCycle[iCycle].bHWAcquisition)
	{
		pFocalLawAcquisition = &pFocalLaw[iCycle];
		iCycleAcquisition = iCycle;
	}

	if(!pCycle[iCycle].bHWAcquisition && (pFocalLaw[iCycle].iElementCount>1))
		return false;//too many elements
	if(!pFocalLaw[iCycle].iElementCount)
		return true;
	//for(int iIndexAp=0;iIndexAp<g_iOEMPAApertureDWordCount;iIndexAp++)
	//{
	//	dwEnable = 1;
	//	if(pFocalLawAcquisition->adwElement[iIndexAp])
	//	for(int iIndex=0;iIndex<32;iIndex++)
	//	{
	//		if(dwEnable & pFocalLawAcquisition->adwElement[iIndexAp])
	//			aiElement[iElementCount++] = iIndexAp*32+iIndex;
	//		dwEnable = dwEnable<<1;
	//	}
	//}
	//OEMMC only one element is used at a time. Need to know his number.
	for(int iIndexAp=0;iIndexAp<g_iOEMPAApertureDWordCount;iIndexAp++)
	{
		dwEnable = 1;
		if(pFocalLaw[iCycle].adwElement[iIndexAp])
		for(int iIndex=0;iIndex<32;iIndex++)
		{
			if(dwEnable & pFocalLaw[iCycle].adwElement[iIndexAp])
			{
				dwEnable = dwEnable<<1;
				iIndexAp2 = iIndexAp;
				iIndexElem2 = iIndex;
				break;
			}
			dwEnable = dwEnable<<1;
		}
		if(iIndexAp2>=0)
			break;
	}
	if(iIndexAp2<0)
		return false;
	return true;
}
bool ReadFileMultiHWChannelUpdate(int iCycle,const wchar_t *pFileName,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pReception)
{
	int iIndexAp2,iIndexElem2,iDecimation;
	double dRangeMax,dStart,dStop;
	wchar_t wcAux[MAX_PATH];
	static CFocalLaw *pReceptionAcquisition=NULL;
	static int iCycleAcquisition=-1;

	if(!GetMultiHWChannelCheck(iCycle,pRoot,pCycle,pReception,pReceptionAcquisition,iCycleAcquisition,iIndexAp2,iIndexElem2))
		return false;
	if(!pReception[iCycle].iElementCount)
		return true;
	if((iIndexAp2<0) || (iIndexElem2<0))
		return true;
	for(int iIndexElement=0;iIndexElement<pReceptionAcquisition->iDelayCount;iIndexElement++)
	{
		//if(aiElement[iIndexElement]==iIndexAp2*32+iIndexElem2)
		if(iIndexElement==iIndexAp2*32+iIndexElem2)
		{
			dStart = (double)pReceptionAcquisition->afDelay[0][iIndexElement];
			if(pCycle[iCycle].bHWAcquisition)
			{
				iDecimation = 1;
			}else{
				iDecimation = pReceptionAcquisition->hwAcqDecimation.byte[iIndexElement/2];
				if(iIndexElement%2)
					iDecimation = (iDecimation>>4)+1;
				else
					iDecimation = (iDecimation & 0xf)+1;
			}
			pCycle[iCycle].dRange /= (double)iDecimation;
			if(pCycle[iCycle].bDACEnable)
			{
				for(int iIndex=0;iIndex<pCycle[iCycle].iDACCount;iIndex++)
				{
					if((iCycleAcquisition!=iCycle) && !pCycle[iCycle].bTrackingDacEnable)
					{
						if(pCycle[iCycle].adDACTof[iIndex]<dStart)
						{
							swprintf(wcAux,MAX_PATH,L"MultiChannel bad DAC time for the cycle %d",iCycle);
							OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
							return false;//inconsistent range.
						}
						pCycle[iCycle].adDACTof[iIndex] -= dStart;
					}
					pCycle[iCycle].adDACTof[iIndex] /= iDecimation;
					//pCycle[iCycle].afDACPrm[iIndex] /= iDecimation;//slope should be the same.
				}
			}
			for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
			{
				if(!pCycle[iCycle].gate[iGate].bEnable)
					continue;
				if((iCycleAcquisition!=iCycle) && !pCycle[iCycle].gate[iGate].bTrackingStartEnable)
				{
					if(pCycle[iCycle].gate[iGate].dStart<dStart)
					{
						swprintf(wcAux,MAX_PATH,L"MultiChannel bad gate start time for the cycle %d",iCycle);
						OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
						return false;//inconsistent range.
					}
					pCycle[iCycle].gate[iGate].dStart -= dStart;
				}
				pCycle[iCycle].gate[iGate].dStart /= (double)iDecimation;
				if((iCycleAcquisition!=iCycle) && !pCycle[iCycle].gate[iGate].bTrackingStopEnable)
				{
					if(pCycle[iCycle].gate[iGate].dStop<dStart)
					{
						swprintf(wcAux,MAX_PATH,L"MultiChannel bad gate stop time for the cycle %d",iCycle);
						OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
						return false;//inconsistent range.
					}
					pCycle[iCycle].gate[iGate].dStop -= dStart;
				}
				pCycle[iCycle].gate[iGate].dStop /= (double)iDecimation;
			}
			if(pCycle[iCycle].bTrackingAscanEnable)
				return true;//Because of the tracking, it is impossible to check the range!
			//check the Range compared to the TimeSlot of the acquisition cycle and the maximum available range.
			dRangeMax = pRoot->dMultiHWChannelRangeMax;//8192 or 10240 points clock 50 MHz => 163.84 us or 204.8 us.
			if(dRangeMax+dStart>pCycle[iCycleAcquisition].dTimeSlot)
				dRangeMax = pCycle[iCycleAcquisition].dTimeSlot-dStart;//range limitation because of the acquisition cycle time slot.
			if(pCycle[iCycle].dRange>dRangeMax)
			{
				swprintf(wcAux,MAX_PATH,L"MultiChannel bad range for the cycle %d",iCycle);
				OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
				return false;//inconsistent range.
			}
			//check the range compared to the TimeSlot of the same cycle "iCycle".
			//limitation of the FW recovery time
			if(!pCycle[iCycle].bHWAcquisition)
				dStart = 0.0;
			dStop = dStart + pCycle[iCycle].dRange + pRoot->dFWAscanRecoveryTime;
			dStop /= iDecimation;//Sam
			if(dStop > pCycle[iCycle].dTimeSlot)
			{
				swprintf(wcAux,MAX_PATH,L"MultiChannel bad TimeSlot/Range for the cycle %d",iCycle);
				OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
				return false;//inconsistent range.
			}
			return true;
		}
	}
	swprintf(wcAux,MAX_PATH,L"MultiChannel bad receiver channel number for the cycle %d",iCycle);
	OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
	return false;//the element is not part of the acquisition aperture!
}
bool ReadHWMultiHWChannelUpdate(int iCycle,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pReception)
{
	int iIndexAp2,iIndexElem2,iDecimation;
	double dStart;
	static CFocalLaw *pReceptionAcquisition=NULL;
	static int iCycleAcquisition=-1;

	if(!GetMultiHWChannelCheck(iCycle,pRoot,pCycle,pReception,pReceptionAcquisition,iCycleAcquisition,iIndexAp2,iIndexElem2))
		return false;
	if((iIndexAp2<0) || (iIndexElem2<0))
		return false;
	for(int iIndexElement=0;iIndexElement<pReceptionAcquisition->iDelayCount;iIndexElement++)
	{
		//if(aiElement[iIndexElement]==iIndexAp2*32+iIndexElem2)
		if(iIndexElement==iIndexAp2*32+iIndexElem2)
		{
			dStart = (double)pReceptionAcquisition->afDelay[0][iIndexElement];
			if(pCycle[iCycle].bHWAcquisition)
			{
				iDecimation = 1;
			}else{
				iDecimation = pReceptionAcquisition->hwAcqDecimation.byte[iIndexElement/2];
				if(iIndexElement%2)
					iDecimation = (iDecimation>>4)+1;
				else
					iDecimation = (iDecimation & 0xf)+1;
			}
			pCycle[iCycle].dRange *= (double)iDecimation;
			if(pCycle[iCycle].bDACEnable)
			{
				for(int iIndex=0;iIndex<pCycle[iCycle].iDACCount;iIndex++)
				{
					pCycle[iCycle].adDACTof[iIndex] *= iDecimation;
					pCycle[iCycle].afDACPrm[iIndex] *= iDecimation;
					if((iCycleAcquisition!=iCycle) && !pCycle[iCycle].bTrackingDacEnable)
						pCycle[iCycle].adDACTof[iIndex] += dStart;
				}
			}
			for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
			{
				if(!pCycle[iCycle].gate[iGate].bEnable)
					continue;
				pCycle[iCycle].gate[iGate].dStart *= (double)iDecimation;
				if((iCycleAcquisition!=iCycle) && !pCycle[iCycle].gate[iGate].bTrackingStartEnable)
					pCycle[iCycle].gate[iGate].dStart += dStart;
				pCycle[iCycle].gate[iGate].dStop *= (double)iDecimation;
				if((iCycleAcquisition!=iCycle) && !pCycle[iCycle].gate[iGate].bTrackingStopEnable)
					pCycle[iCycle].gate[iGate].dStop += dStart;
			}
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

//#ifdef _TFM_CPP_
bool ReadFileTextTFMFlatContactLinear1D(const wchar_t *pFileName,structRoot &root,structCycle* pCycle,CFocalLaw* pEmission,CFocalLaw* pReception,structCorrectionOEMPA* pCorrection)
{
	structTFMParameters tfm;
	long lData;
	int iEnum;
	bool bRet=true;

	if(!root.rParamList)
	{
		OempaApiMessageBox(NULL,L"root.rParamList is NULL!",L"Error",MB_OK);
		return false;
	}
	if(root.iCycleCount<=1)
	{
		OempaApiMessageBox(NULL,L"For TFM CycleCount should be greater than one!",L"Error",MB_OK);
		bRet = false;goto end;
	}
	tfm.eTFMParameters = eFlatContact_Linear1D;
	tfm.parameter.flatContactLinear1D.dXMin = -(root.iCycleCount-1)*tfm.parameter.flatContactLinear1D.dProbePitchIn/2;
	tfm.parameter.flatContactLinear1D.dXMax = (root.iCycleCount-1)*tfm.parameter.flatContactLinear1D.dProbePitchIn/2;
	tfm.parameter.flatContactLinear1D.dZMin = 0.0;
	tfm.parameter.flatContactLinear1D.iApertureElementCount = 32;
	tfm.iCycleCountOut = root.iCycleCount;
	tfm.iAscanPointCountOut = 128;
	tfm.iCycleCountIn = root.iCycleCount;
	tfm.iAscanBitSizeOut = -1;//same than input
	tfm.iModeOut = 0;
	if(tfm.iCycleCountIn>0)
		tfm.iAscanPointCountIn = pCycle->lPointCount;
	else
		tfm.iAscanPointCountIn = 0;
	if(root.dDigitizingPeriod)
		tfm.dSamplingFrequency = 1/root.dDigitizingPeriod;
	tfm.dGain = 0.0;
	for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
	{
		if(pCycle[iCycle].dStart>0.0)
		{
			OempaApiMessageBox(NULL,L"For TFM AscanStart should be 0.0 us!",L"Error",MB_OK);
			bRet = false;goto end;
		}
		if(iCycle && (pCycle[iCycle].dRange!=pCycle[0].dRange))
		{
			OempaApiMessageBox(NULL,L"For TFM AscanRange should be the same for all cycles!",L"Error",MB_OK);
			bRet = false;goto end;
		}
		if((pCycle[iCycle].eCompressionType==eCompression) && (pCycle[iCycle].lPointFactor<256))
		{
			OempaApiMessageBox(NULL,L"For TFM compression is not available!",L"Error",MB_OK);
			bRet = false;goto end;
		}else{
			if((pCycle[iCycle].eCompressionType==eDecimation) && (pCycle[iCycle].lPointFactor!=pCycle[0].lPointFactor))
			{
				OempaApiMessageBox(NULL,L"For TFM decimation should be the same for all cycles!",L"Error",MB_OK);
				bRet = false;goto end;
			}
		}
		if(pCycle[iCycle].lPointCount!=tfm.iAscanPointCountIn)
		{
			OempaApiMessageBox(NULL,L"For TFM AscanPointCount should be same for all cycles!",L"Error",MB_OK);
			bRet = false;goto end;
		}
	}

	if(pCorrection)
	{
		tfm.parameter.flatContactLinear1D.dZMax = pCorrection->dSpecimenVelocity*pCycle[0].dRange/2.0;
		tfm.dSamplingFrequency = pCorrection->dSamplingFrequency;
		tfm.dVelocity = pCorrection->dSpecimenVelocity;
	}
	if(!root.rParamList->FileReadDouble(L"TFM_FlatContactLinear1D",L"ZMax",pFileName,1.0e-3,tfm.parameter.flatContactLinear1D.dZMax,pCorrection?false:true)
		&& !pCorrection)
	{
		OempaApiMessageBox(NULL,L"ZMax",L"Error",MB_OK);
		bRet = false;goto end;
	}
	if(!root.rParamList->FileReadDouble(L"TFM_FlatContactLinear1D",L"SamplingFrequency",pFileName,1.0e6,tfm.dSamplingFrequency,pCorrection?false:true)
		&& !pCorrection)
	{
		OempaApiMessageBox(NULL,L"SamplingFrequency",L"Error",MB_OK);
		bRet = false;goto end;
	}
	if(!root.rParamList->FileReadDouble(L"TFM_FlatContactLinear1D",L"Velocity",pFileName,1.0,tfm.dVelocity,pCorrection?false:true)
		&& !pCorrection)
	{
		OempaApiMessageBox(NULL,L"Velocity",L"Error",MB_OK);
		bRet = false;goto end;
	}
	if(!root.rParamList->FileReadDouble(L"TFM_FlatContactLinear1D",L"Gain",pFileName,1.0,tfm.dGain,false))
		tfm.dGain = 0.0;

	if(root.rParamList->FileReadEnum(L"TFM_FlatContactLinear1D",L"Option",pFileName,
		L"CPU"L"\0"L"GPU"L"\0"L"CPU_1Thread"L"\0",iEnum,false))
		tfm.eOption = (enumTFMOptions)iEnum;
	if(!root.rParamList->FileReadDouble(L"TFM_FlatContactLinear1D",L"ProbePitch",pFileName,1.0e-3,tfm.parameter.flatContactLinear1D.dProbePitchIn,true))
	{
		OempaApiMessageBox(NULL,L"ProbePitch",L"Error",MB_OK);
		bRet = false;goto end;
	}
	if(!root.rParamList->FileReadDouble(L"TFM_FlatContactLinear1D",L"XFirstElement",pFileName,1.0e-3,tfm.parameter.flatContactLinear1D.dXFirstElement,false))
	{
		OempaApiMessageBox(NULL,L"XFirstElement",L"Error",MB_OK);
		bRet = false;goto end;
	}
	if(!root.rParamList->FileReadDouble(L"TFM_FlatContactLinear1D",L"XMin",pFileName,1.0e-3,tfm.parameter.flatContactLinear1D.dXMin,false))
	{
		OempaApiMessageBox(NULL,L"XMin",L"Error",MB_OK);
		bRet = false;goto end;
	}
	if(!root.rParamList->FileReadDouble(L"TFM_FlatContactLinear1D",L"XMax",pFileName,1.0e-3,tfm.parameter.flatContactLinear1D.dXMax,false))
	{
		OempaApiMessageBox(NULL,L"XMax",L"Error",MB_OK);
		bRet = false;goto end;
	}
	if(!root.rParamList->FileReadDouble(L"TFM_FlatContactLinear1D",L"ZMin",pFileName,1.0e-3,tfm.parameter.flatContactLinear1D.dZMin,pCorrection?false:true)
		&& !pCorrection)
	{
		OempaApiMessageBox(NULL,L"ZMin",L"Error",MB_OK);
		bRet = false;goto end;
	}
	lData = tfm.iCycleCountOut;
	if(!root.rParamList->FileReadLong(L"TFM_FlatContactLinear1D",L"CycleCountOut",pFileName,lData,false))
	{
		OempaApiMessageBox(NULL,L"CycleCountOut",L"Error",MB_OK);
		bRet = false;goto end;
	}else
		tfm.iCycleCountOut = (int)lData;
	lData = tfm.iAscanPointCountOut;
	if(!root.rParamList->FileReadLong(L"TFM_FlatContactLinear1D",L"AscanPointCountOut",pFileName,lData,false))
	{
		OempaApiMessageBox(NULL,L"AscanPointCountOut",L"Error",MB_OK);
		bRet = false;goto end;
	}else
		tfm.iAscanPointCountOut = (int)lData;
	lData = tfm.parameter.flatContactLinear1D.iApertureElementCount;
	if(!root.rParamList->FileReadLong(L"TFM_FlatContactLinear1D",L"ApertureElementCount",pFileName,lData,false))
	{
		OempaApiMessageBox(NULL,L"ApertureElementCount",L"Error",MB_OK);
		bRet = false;goto end;
	}else
		tfm.parameter.flatContactLinear1D.iApertureElementCount = (int)lData;
	if(!root.rParamList->FileReadEnum(L"TFM_FlatContactLinear1D",L"AscanBitSizeOut",pFileName,
		L"8Bits"L"\0"L"12Bits"L"\0"L"16Bits"L"\0",tfm.iAscanBitSizeOut,false))
		tfm.iAscanBitSizeOut = -1;
	lData = tfm.iModeOut;
	if(root.rParamList->FileReadLong(L"TFM_FlatContactLinear1D",L"ModeOut",pFileName,lData,false))
		tfm.iModeOut = (int)lData;
	if(root.pHWDeviceOEMPA)
		bRet = root.pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableTFM(true,&tfm);
end:
	return bRet;
}
//#endif _TFM_CPP_
bool ReadFileTextSubSequence(const wchar_t *pFileName,structRoot &root)
{
	int iCycleStopCount,iAux,iEncoderCount,iSubSequenceCount;
	bool bRet=true;

	if(!root.rParamList)
	{
		OempaApiMessageBox(NULL,L"root.rParamList is NULL!",L"Error",MB_OK);
		return false;
	}
	if(!root.rParamList->FileReadInteger(L"SubSequence",L"CycleStart.count",pFileName,iSubSequenceCount,false))
		return true;
	if(iSubSequenceCount<0)
		{bRet = false;goto end;}
	if(iSubSequenceCount>=g_iSubSequenceTableSize)
		{bRet = false;goto end;}
	iAux = iSubSequenceCount;
	if(iAux && !root.rParamList->FileReadArrayInteger(L"SubSequence",L"CycleStart",pFileName,iSubSequenceCount,root.aiSubSequenceCycleStart,iAux))
	{
		OempaApiMessageBox(NULL,L"CycleStart",L"File Error",MB_OK);
		{bRet = false;goto end;}
	}
	root.iSubSequenceEncoderCount=root.iSubSequenceGateCount=0;

	if(!root.rParamList->FileReadInteger(L"SubSequence",L"CycleStop.count",pFileName,iCycleStopCount))
		{bRet = false;goto end;}
	if(iCycleStopCount!=iSubSequenceCount)
		{bRet = false;goto end;}
	iAux = iCycleStopCount;
	if(iAux && !root.rParamList->FileReadArrayInteger(L"SubSequence",L"CycleStop",pFileName,iSubSequenceCount,root.aiSubSequenceCycleStop,iAux))
	{
		OempaApiMessageBox(NULL,L"CycleStop",L"File Error",MB_OK);
		{bRet = false;goto end;}
	}

	if(root.rParamList->FileReadInteger(L"SubSequence",L"Encoder.count",pFileName,iEncoderCount,false))
	{
		if((iSubSequenceCount>0) && (iEncoderCount!=iSubSequenceCount+1))
			{bRet = false;goto end;}
		iAux = iEncoderCount+1;
		if(iAux && !root.rParamList->FileReadArrayFloat(L"SubSequence",L"Encoder",pFileName,iSubSequenceCount+1,1.0e-3f,root.afSubSequenceValue,iEncoderCount))
		{
			OempaApiMessageBox(NULL,L"SubSequence/Encoder",L"File Error",MB_OK);
			{bRet = false;goto end;}
		}
		root.iSubSequenceEncoderCount = iSubSequenceCount;
	}
	if(root.rParamList->FileReadInteger(L"SubSequence",L"Gate.count",pFileName,iEncoderCount,false))
	{
		if((iSubSequenceCount>0) && (iEncoderCount!=iSubSequenceCount+1))
			{bRet = false;goto end;}
		iAux = iEncoderCount+1;
		if(iAux && !root.rParamList->FileReadArrayFloat(L"SubSequence",L"Gate",pFileName,iSubSequenceCount+1,1.0e-6f,root.afSubSequenceValue,iEncoderCount))
		{
			OempaApiMessageBox(NULL,L"SubSequence/Gate",L"File Error",MB_OK);
			{bRet = false;goto end;}
		}
		root.iSubSequenceGateCount = iSubSequenceCount;
	}
	if(!root.iSubSequenceEncoderCount && !root.iSubSequenceGateCount)
	{
		OempaApiMessageBox(NULL,L"SubSequence: no Encoder and no Gate!",L"File Error",MB_OK);
		bRet = false;goto end;
	}
	root.rParamList->FileReadInteger(L"SubSequence",L"Average",pFileName,root.iSubSequenceAverage,false,0,80,0);

end:
	return bRet;
}

_CUST_DRV_DEF_ bool WINAPI OEMPA_ReadFileText(	const wchar_t *pFileName,
							structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception)
{
	bool bRet=true,bAllocParamList=false;
	int iEnum,iFilterCount,iDisableUSB3;
	long lData,lData2;
	wchar_t wcAux[MAX_PATH];
	structCycle *pCycleAux;
	CFocalLaw *pFocalLaw;
	bool bUSB3Disable;

	if(pCycle || pEmission || pReception)
		return false;
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
	if(root.rParamList->fastReading)//GP
		root.rParamList->ReadTxtCreateList(pFileName); //GP

	root.ullSavedParameters = 0;//read from the file itself

	if(root.rParamList->FileReadEnum(L"Root",L"DefaultHwLink",pFileName,
		L"Unlinked"L"\0"L"Master"L"\0"L"Slave"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_DEFAULT_HW_LINK;
		root.eDefaultHwLink = (enumHardwareLink)iEnum;
	}
	if(root.rParamList->FileReadBoolean(L"Root",L"USB3Disable",pFileName,bUSB3Disable,false))
	{
		root.ullSavedParameters |= OEMPA_DISABLE_USB3;
		root.bUSB3Disable = bUSB3Disable?1:0;
	}
	root.bEnableMultiHWChannel = false;
	if(root.rParamList->FileReadBoolean(L"Root",L"EnableMultiChannel",pFileName,root.bEnableMultiHWChannel,false))
	{
		root.ullSavedParameters |= OEMPA_ENABLE_MULTI_CHANNEL;
	}
	if(root.rParamList->FileReadBoolean(L"Root",L"AscanEnable",pFileName,root.bAscanEnable,false))
	{
		root.ullSavedParameters |= OEMPA_ASCAN_ENABLE;
	}
	if(root.rParamList->FileReadBoolean(L"Root",L"EnableCscanTof",pFileName,root.bEnableCscanTof,false))
	{
		root.ullSavedParameters |= OEMPA_CSCAN_ENABLE_TOF;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"AscanBitSize",pFileName,L"8Bits"L"\0"L"12Bits"L"\0"L"16Bits"L"\0"L"Log8Bits"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_ASCAN_BITSIZE;
		root.eAscanBitSize = (enumBitSize)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"TriggerMode",pFileName,L"Internal"L"\0"L"Encoder"L"\0"L"ExternalCycle"L"\0"
									L"ExternalSequence"L"\0"L"ExternalCycleSequence"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_TRIGGER_MODE;
		root.eTriggerMode = (enumOEMPATrigger)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"EncoderDirection",pFileName,L"UpDown"L"\0"L"UpOnly"L"\0"L"DownOnly"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_ENCODER_TRIG_DIRECTION;
		root.eEncoderDirection = (enumOEMPAEncoderDirection)iEnum;
	}
	if(root.rParamList->FileReadDouble(L"Root",L"TriggerEncoderStep",pFileName,1.0e-3,root.dTriggerEncoderStep,false))
	{
		root.ullSavedParameters |= OEMPA_TRIGGER_ENCODER_STEP;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"RequestIO",pFileName,L"NotRequired"L"\0"L"OnCycleOnly"L"\0"L"OnSequenceOnly"L"\0"
									L"OnDigitalInputOnly"L"\0"L"OnDigitalInputAndCycle"L"\0"L"OnDigitalInputAndSequence"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_REQUESTIO;
		root.eRequestIO = (enumOEMPARequestIO)iEnum;
	}
	if(root.rParamList->FileReadLong(L"Root",L"RequestIODigitalInputMaskRising",pFileName,lData,false) &&
		root.rParamList->FileReadLong(L"Root",L"RequestIODigitalInputMaskFalling",pFileName,lData2,true))
	{
		root.ullSavedParameters |= OEMPA_REQUESTIO_DIGITAL_INPUT_MASK;
		root.iRequestIODigitalInputMaskRising = (int)lData;
		root.iRequestIODigitalInputMaskFalling = (int)lData2;
	}
	if(root.rParamList->FileReadDouble(L"Root",L"DebouncerEncoder",pFileName,1.0e-6f,root.dDebouncerEncoder,false))
		root.ullSavedParameters |= OEMPA_DEBOUNCER_ENCODER;
	if(root.rParamList->FileReadDouble(L"Root",L"DebouncerDigital",pFileName,1.0e-6f,root.dDebouncerDigital,false))
		root.ullSavedParameters |= OEMPA_DEBOUNCER_DIGITAL;
	if(root.rParamList->FileReadEnum(L"Root",L"DigitalOutput0",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_DIGITAL_OUTPUT_0;
		root.eDigitalOuput[0] = (enumOEMPAMappingDigitalOutput)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"DigitalOutput1",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_DIGITAL_OUTPUT_1;
		root.eDigitalOuput[1] = (enumOEMPAMappingDigitalOutput)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"DigitalOutput2",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_DIGITAL_OUTPUT_2;
		root.eDigitalOuput[2] = (enumOEMPAMappingDigitalOutput)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"DigitalOutput3",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_DIGITAL_OUTPUT_3;
		root.eDigitalOuput[3] = (enumOEMPAMappingDigitalOutput)iEnum,false;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"DigitalOutput4",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",iEnum))
	{
		root.ullSavedParameters |= OEMPA_DIGITAL_OUTPUT_4;
		root.eDigitalOuput[4] = (enumOEMPAMappingDigitalOutput)iEnum,false;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"DigitalOutput5",pFileName,L"Low"L"\0"L"High"L"\0"L"SignalCycle"L"\0"
									L"SignalSequence"L"\0"L"TemperatureAlarm"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_DIGITAL_OUTPUT_5;
		root.eDigitalOuput[5] = (enumOEMPAMappingDigitalOutput)iEnum;
	}
	if(root.rParamList->FileReadLong(L"Root",L"SWEncoder1Resolution",pFileName,lData,false))
	{
		root.ullSavedParameters |= OEMPA_SW_ENCODER1_RESOLUTION;
		root.lSWEncoderResolution1 = (int)lData;
		if(root.rParamList->FileReadLong(L"Root",L"SWEncoder1Invert",pFileName,lData,false) && (lData==1) && (root.lSWEncoderResolution1>0))
			root.lSWEncoderResolution1 = -root.lSWEncoderResolution1;//compatibility with old OEMPA file.
	}
	if(root.rParamList->FileReadULong(L"Root",L"SWEncoder1Divider",pFileName,root.dwSWEncoderDivider1,false))
	{
		if(!root.dwSWEncoderDivider1)
		{
			ReportFileError(L"Root",L"SWEncoder1Divider");
			bRet = false;goto end;
		}
		root.ullSavedParameters |= OEMPA_SW_ENCODER1_DIVIDER;
	}
	if(root.rParamList->FileReadLong(L"Root",L"SWEncoder2Resolution",pFileName,lData,false))
	{
		root.ullSavedParameters |= OEMPA_SW_ENCODER2_RESOLUTION;
		root.lSWEncoderResolution2 = (int)lData;
		if(root.rParamList->FileReadLong(L"Root",L"SWEncoder2Invert",pFileName,lData,false) && (lData==1) && (root.lSWEncoderResolution2>0))
			root.lSWEncoderResolution2 = -root.lSWEncoderResolution2;//compatibility with old OEMPA file.
	}
	if(root.rParamList->FileReadULong(L"Root",L"SWEncoder2Divider",pFileName,root.dwSWEncoderDivider2,false))
	{
		if(!root.dwSWEncoderDivider2)
		{
			ReportFileError(L"Root",L"SWEncoder2Divider");
			bRet = false;goto end;
		}
		root.ullSavedParameters |= OEMPA_SW_ENCODER2_DIVIDER;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"Encoder1A",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_ENCODER1A;
		root.eEncoder1A = (enumDigitalInput)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"Encoder1B",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_ENCODER1B;
		root.eEncoder1B = (enumDigitalInput)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"Encoder2A",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_ENCODER2A;
		root.eEncoder2A = (enumDigitalInput)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"Encoder2B",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_ENCODER2B;
		root.eEncoder2B = (enumDigitalInput)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"ExternalTriggerCycle",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_EXTERNAL_TRIGGER_CYCLE;
		root.eExternalTriggerCycle = (enumDigitalInput)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"ExternalTriggerSequence",pFileName,L"DigitalInputOff"L"\0"L"DigitalInput01"L"\0"L"DigitalInput02"L"\0"
									L"DigitalInput03"L"\0"L"DigitalInput04"L"\0"L"DigitalInput05"L"\0"L"DigitalInput06"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_EXTERNAL_TRIGGER_SEQUENCE;
		root.eExternalTriggerSequence = (enumDigitalInput)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"KeepAlive",pFileName,L"Off"L"\0"L"HardwareAndComputer"L"\0"L"HardwareOnly"L"\0"
									L"ComputerOnly"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_KEEPALIVE;
		root.eKeepAlive = (enumKeepAlive)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"EncoderType",pFileName,L"StaticScan"L"\0"L"Quadrature"L"\0"L"Quadrature4Edges"L"\0"L"DirectionCount"L"\0"
									L"ForwardBackward"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_ENCODER1_TYPE | OEMPA_ENCODER2_TYPE;
		root.eEncoder1Type = (enumEncoderType)iEnum;
		root.eEncoder2Type = (enumEncoderType)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"Encoder1Type",pFileName,L"StaticScan"L"\0"L"Quadrature"L"\0"L"Quadrature4Edges"L"\0"L"DirectionCount"L"\0"
									L"ForwardBackward"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_ENCODER1_TYPE;
		root.eEncoder1Type = (enumEncoderType)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"Encoder2Type",pFileName,L"StaticScan"L"\0"L"Quadrature"L"\0"L"Quadrature4Edges"L"\0"L"DirectionCount"L"\0"
									L"ForwardBackward"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_ENCODER2_TYPE;
		root.eEncoder2Type = (enumEncoderType)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"AscanRequest",pFileName,L"AscanAll"L"\0"L"AscanSampled"L"\0"
									L"BscanOnLine"L"\0",iEnum,false))
	{
		if(root.ullSavedParameters & OEMPA_ASCAN_REQUEST)
		{
			ReportFileError(L"Root",L"AscanRequest");
			bRet = false;goto end;
		}
		root.ullSavedParameters |= OEMPA_ASCAN_REQUEST;
		root.eAscanRequest = (enumAscanRequest)iEnum;
	}
	if(root.ullSavedParameters & OEMPA_ASCAN_REQUEST)
	if(!root.rParamList->FileReadDouble(L"Root",L"AscanRequestFrequency",pFileName,1.0,root.dAscanRequestFrequency,false))
	{
		ReportFileError(L"Root",L"AscanRequestFrequency");
		bRet = false;goto end;
	}
	if(root.rParamList->FileReadDouble(L"Root",L"TriggerHighTime",pFileName,1.0e-6,root.dTriggerHighTime,false))
		root.ullSavedParameters |= OEMPA_TRIGGER_HIGH_TIME;

	if(!root.rParamList->FileReadInteger(L"Root",L"CycleCount",pFileName,root.iCycleCount))
	{
		ReportFileError(L"Root",L"CycleCount");
		bRet = false;goto end;
	}
	if(root.rParamList->FileReadInteger(L"Root",L"DisableUSB3",pFileName,iDisableUSB3,false))
	{
		root.ullSavedParameters |= OEMPA_DISABLE_USB3;
		root.bUSB3Disable = (BYTE)iDisableUSB3;
	}
	bool bFullMatrixCaptureReadWrite = root.pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite();
	if(root.pHWDeviceOEMPA &&
		(//FW FMC only (old version)
			(root.pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture() && !bFullMatrixCaptureReadWrite)
			||
			//FW FMC Read/Write
			bFullMatrixCaptureReadWrite)
		)
	{
		if(bFullMatrixCaptureReadWrite && root.rParamList->FileReadBoolean(L"Root",L"EnableFMC",pFileName,root.bEnableFMC,false))
		{//1.1.5.4i
			root.ullSavedParameters |= OEMPA_ENABLE_FMC;
			if(!root.rParamList->FileReadInteger(L"Root",L"FMCElementStart",pFileName,root.iFMCElementStart,false,0,root.iOEMPAProbeCountMax-1,-123456))
			{
				if(root.iFMCElementStart == -123456)
					root.iFMCElementStart = 0;
				else{
					ReportFileError(L"Root",L"FMCElementStart");
					bRet = false;goto end;
				}
			}
			if(!root.rParamList->FileReadInteger(L"Root",L"FMCElementStop",pFileName,root.iFMCElementStop,false,0,root.iOEMPAProbeCountMax-1,-123456))
			{
				if(root.iFMCElementStop == -123456)
					root.iFMCElementStop = root.iOEMPAProbeCountMax-1;
				else{
					ReportFileError(L"Root",L"FMCElementStop");
					bRet = false;goto end;
				}
			}
			if(!root.rParamList->FileReadInteger(L"Root",L"FMCElementStep",pFileName,root.iFMCElementStep,false,0,root.iOEMPAProbeCountMax-1,-123456))
			{
				if(root.iFMCElementStep == -123456)
					root.iFMCElementStep = 1;
				else{
					ReportFileError(L"Root",L"FMCElementStep");
					bRet = false;goto end;
				}
			}
		}
	}
	if(root.rParamList->FileReadEnum(L"Root",L"EnableTFM",pFileName,L"Off"L"\0"L"FlatContactLinear1D"L"\0"L"\0",iEnum,false))
		root.eEnableTFM = (enumTFMParameters)iEnum;
	else
		root.eEnableTFM = eTFMOff;
	if(root.rParamList->FileReadInteger(L"Root",L"TemperatureWarning",pFileName,iEnum,false,0,127))
	{
		root.ullSavedParameters |= OEMPA_TEMPERATURE_WARNING;
		root.cTemperatureWarning = (char)iEnum;
	}
	if(root.rParamList->FileReadInteger(L"Root",L"TemperatureAlarm",pFileName,iEnum,false,0,127))
	{
		root.ullSavedParameters |= OEMPA_TEMPERATURE_ALARM;
		root.cTemperatureAlarm = (char)iEnum;
	}
	if(root.rParamList->FileReadEnum(L"Root",L"ReferenceTimeOfFlight",pFileName,L"Pulse"L"\0"L"Interface"L"\0"L"0"L"\0"L"1"L"\0",iEnum,false))
	{
		root.ullSavedParameters |= OEMPA_REFERENCE_TIME_OF_FLIGHT;
		switch(iEnum)
		{
		default: break;
		case 0:
		case 2: root.eReferenceTimeOfFlight = ePulse;break;
		case 1:
		case 3: root.eReferenceTimeOfFlight = eInterface;break;
		}
	}

	if(!root.rParamList->FileReadInteger(L"Root",L"FilterCount",pFileName,iFilterCount,false))
		iFilterCount = 0;

	root.uiSavedFilterCount = 0;
	for(int iFilterIndex=0;iFilterIndex<iFilterCount;iFilterIndex++)
	{
		if(!ReadFileTextFilter(iFilterIndex,pFileName,&root))
		{
			swprintf(wcAux,MAX_PATH,L"Filter %d",iFilterIndex);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;
		}
	}

	if(!ReadFileTextSubSequence(pFileName,root))
		bRet = false;

	if(root.iCycleCount>0)
	{
		pCycle = OEMPA_AllocCycle(root.iCycleCount);
		pEmission = new CFocalLaw[root.iCycleCount];
		pReception = new CFocalLaw[root.iCycleCount];
		if(!pCycle || !pEmission || !pReception)
		{
			bRet = false;goto end;
		}
		OEMPA_ResetArrayFocalLaw(root.iCycleCount,pEmission);
		OEMPA_ResetArrayFocalLaw(root.iCycleCount,pReception);
	}

	for(int iCycle=0;iCycle<root.iCycleCount;iCycle++)
	{
		pCycleAux = &pCycle[iCycle];
		if(!ReadFileTextCycle(iCycle,pFileName,&root,pCycleAux))
			{bRet = false;goto end;}

		pFocalLaw = &pEmission[iCycle];
		if(!ReadFileTextFocalLaw(iCycle,true,pCycleAux->bHWAcquisition,pFileName,&root,pFocalLaw))
		{
			swprintf(wcAux,MAX_PATH,L"Emission Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;goto end;
		}
		pFocalLaw = &pReception[iCycle];
		if(!ReadFileTextFocalLaw(iCycle,false,pCycleAux->bHWAcquisition,pFileName,&root,pFocalLaw))
		{
			swprintf(wcAux,MAX_PATH,L"Reception Cycle %d",iCycle);
			OempaApiMessageBox(NULL,wcAux,L"Error",MB_OK);
			bRet = false;goto end;
		}

		if(!root.bEnableMultiHWChannel)
		{
			if(!ReadFileStandardCheck(iCycle,pFileName,&root,pCycle,pReception))
				{bRet = false;goto end;}
		}else{
			if(!ReadFileMultiHWChannelUpdate(iCycle,pFileName,&root,pCycle,pReception))
				{bRet = false;goto end;}
		}
	}

end:
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

	if(bAllocParamList && root.rParamList)
	{
		delete root.rParamList;
		root.rParamList = NULL;
	}

	return bRet;
}

_CUST_DRV_DEF_ bool WINAPI OEMPA_ReadFileBinary(const wchar_t *pFileName,
							structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception)
{
	bool bRet=true;
	DWORD dwData;
	UINT uiRes;

	if(pCycle || pEmission || pReception)
		return false;
#ifdef _USE_MFC_
	try{
		CFile file(pFileName,CFile::typeBinary | CFile::modeRead);
#else _USE_MFC_
		myCFile file(pFileName,myCFile::typeBinary | myCFile::modeRead);
#endif _USE_MFC_
		//root
		if(bRet && (uiRes=file.Read(&dwData,sizeof(DWORD))!=sizeof(DWORD)))
			bRet = false;
		if(bRet && (dwData!=1))//one item (one root)
			bRet = false;
		if(bRet && (uiRes=file.Read(&dwData,sizeof(DWORD))!=sizeof(DWORD)))
			bRet = false;
		if(dwData!=sizeof(structRoot))//size of this item.
			bRet = false;
		if(bRet && (uiRes=file.Read(&root,sizeof(structRoot))!=sizeof(structRoot)))
			bRet = false;
		if(bRet && (root.iCycleCount>0))
		{
			pCycle = OEMPA_AllocCycle(root.iCycleCount);
			pEmission = new CFocalLaw[root.iCycleCount];
			pReception = new CFocalLaw[root.iCycleCount];
			if(!pCycle || !pEmission || !pReception)
				bRet = false;
			OEMPA_ResetArrayFocalLaw(root.iCycleCount,pEmission);
			OEMPA_ResetArrayFocalLaw(root.iCycleCount,pReception);
		}
		//cycles
		if(bRet && (uiRes=file.Read(&dwData,sizeof(DWORD))!=sizeof(DWORD)))
			bRet = false;
		if(bRet && (dwData!=root.iCycleCount))
			bRet = false;
		if(bRet && (uiRes=file.Read(&dwData,sizeof(DWORD))!=sizeof(DWORD)))
			bRet = false;
		if(bRet && (dwData!=sizeof(structCycle)))
			bRet = false;
		if(bRet && (uiRes=file.Read(pCycle,root.iCycleCount*sizeof(structCycle))!=root.iCycleCount*sizeof(structCycle)))
			bRet = false;
		//emission focal laws
		if(bRet && (uiRes=file.Read(&dwData,sizeof(DWORD))!=sizeof(DWORD)))
			bRet = false;
		if(bRet && (dwData!=root.iCycleCount))
			bRet = false;
		//if(bRet && (uiRes=file.Read(&dwData,sizeof(DWORD))!=sizeof(DWORD)))
		//	bRet = false;
		//if(bRet && (dwData!=sizeof(CFocalLaw)))
		//	bRet = false;
		//if(bRet && (uiRes=file.Read(pEmission,root.iCycleCount*sizeof(CFocalLaw))!=root.iCycleCount*sizeof(CFocalLaw)))
		//	bRet = false;
		for(int iCycleIndex=0;iCycleIndex<iCycleIndex;iCycleIndex++)
		{
			if(!pEmission[iCycleIndex].Read(file))
				bRet = false;
		}
		//reception focal laws
		if(bRet && (uiRes=file.Read(&dwData,sizeof(DWORD))!=sizeof(DWORD)))
			bRet = false;
		if(bRet && (dwData!=root.iCycleCount))
			bRet = false;
		//if(bRet && (uiRes=file.Read(&dwData,sizeof(DWORD))!=sizeof(DWORD)))
		//	bRet = false;
		//if(bRet && (dwData!=sizeof(CFocalLaw)))
		//	bRet = false;
		//if(bRet && (uiRes=file.Read(pReception,root.iCycleCount*sizeof(CFocalLaw))!=root.iCycleCount*sizeof(CFocalLaw)))
		//	bRet = false;
		for(int iCycleIndex=0;iCycleIndex<iCycleIndex;iCycleIndex++)
		{
			if(!pReception[iCycleIndex].Write(file))
				bRet = false;
		}
		file.Close();
#ifdef _USE_MFC_
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
		bRet = false;
	}
#endif _USE_MFC_

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

	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//#ifdef _TFM_CPP_
bool ReadFileTFM(const wchar_t *pFileName,structRoot &root,structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception,structCorrectionOEMPA *pCorrection)
{
	structCorrectionOEMPA *pCorrection2=NULL;
	bool bRet=true;
	int iCycleCount;

	if(root.eEnableTFM==eTFMOff)
	{
		if(!root.pHWDeviceOEMPA)
			return false;
		bRet = root.pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableTFM(false,NULL);
		return bRet;
	}
	if(root.rParamList->FileReadInteger(L"Correction\\Root",L"CycleCount",pFileName,iCycleCount,false))
		pCorrection2 = pCorrection;
	switch(root.eEnableTFM)
	{
	default:
	case eTFMOff:
		if(root.pHWDeviceOEMPA)
			bRet = root.pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableTFM(false,NULL);
		break;
	case eFlatContact_Linear1D:
		if(!ReadFileTextTFMFlatContactLinear1D(pFileName,root,pCycle,pEmission,pReception,pCorrection2))
		{
			OempaApiMessageBox(NULL,L"TFM FlatContactLinear1D",L"Error",MB_OK);
			bRet = false;
		}
		break;
	}
	return bRet;
}
//#endif _TFM_CPP_
_CUST_DRV_DEF_ bool WINAPI OEMPA_ReadFileWriteHW(CHWDeviceOEMPA *pHWDeviceOEMPA,const wchar_t *pFileName,enumAcquisitionState eAcqState)
{
	bool bTextFile;
	bool bRet=true,bAllocParamList=false;
	structRoot root;
	structCycle *pCycle=NULL;
	CFocalLaw *pEmission=NULL;
	CFocalLaw *pReception=NULL;
	const wchar_t *wcCorrected=pFileName;
	CFocalLaw *pFocalLaw;
	structCorrectionOEMPA *pCorrection=NULL;
	CSWDevice *pSWDevice;
	long lPointFactor=-2;
	enumCompressionType eCompressionType;
	pCallbackCustomizedDriverAPI pCallbackCustomizedAPI=OEMPA_GetCallbackCustomizedDriverAPI();
	DWORD dwAttributes;

	dwAttributes = GetFileAttributes(pFileName);
	if(dwAttributes==INVALID_FILE_ATTRIBUTES)
		return false;
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
	OEMPA_InitRoot(root,pHWDeviceOEMPA);
	if(!IsValidFileAnsi(pFileName))
	{
		OempaApiMessageBox(NULL,L"Unicode file prohibited!",L"File Error",MB_OK);
		return false;
	}
	if(!IsValidFileExtension(pFileName,bTextFile))
	{
		OempaApiMessageBox(NULL,L"Bad file extension!",L"File Error",MB_OK);
		return false;
	}
	wcscpy_s(root.wcFileName,MAX_PATH,pFileName);

	if(bTextFile && pCallbackCustomizedAPI)
		pCorrection = (*pCallbackCustomizedAPI)(pHWDeviceOEMPA,root.wcFileName,eReadFileWriteHW_Enter,&root,pCycle,pEmission,pReception);

	if(bTextFile && !OEMPA_ReadFileText(pFileName,root,pCycle,pEmission,pReception))
	{
		OempaApiMessageBox(NULL,L"Read data error!",L"File Error",MB_OK);
		bRet = false;
	}
	if(!bTextFile && !OEMPA_ReadFileBinary(pFileName,root,pCycle,pEmission,pReception))
	{
		OempaApiMessageBox(NULL,L"Read data error!",L"File Error",MB_OK);
		bRet = false;
	}

	if(bRet && !OEMPA_WriteHW(pHWDeviceOEMPA,root,pCycle,pEmission,pReception,eAcqState))
	{
		OempaApiMessageBox(NULL,L"Write data error!",L"HW error",MB_OK);
		bRet = false;
	}

	if(bTextFile && pCorrection)
	{
		//additional correction information placed in text file only.
		if(ReadCorrectedFileText(wcCorrected,root,pCycle,pEmission,pReception))
		{
			pCorrection->iCount = root.iCycleCount;
			pCorrection->dSpecimenVelocity = root.dSpecimenVelocity;
			pCorrection->dDigitizingPeriod = root.dDigitizingPeriod;
			pCorrection->dSamplingFrequency = 1.0/pCorrection->dDigitizingPeriod;
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
				//1.1.5.0n if(pCycle[iCycle].lPointFactor != 256)
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
			}
			pCorrection->dSpecimenRadius = root.dSpecimenRadius;
			pCorrection->dSpecimenThickness = root.dSpecimenThickness;
			//1.1.5.0n if((lPointFactor != 256) && (lPointFactor != -2))
			if(lPointFactor != -2)
			{
				if(!pHWDeviceOEMPA->GetSamplingFrequency(eCompressionType,lPointFactor,pCorrection->dSamplingFrequency))
					pCorrection->dSamplingFrequency = 1.0/pCorrection->dDigitizingPeriod;
			}
		}else
			pCorrection->iCount = 0;
	}

//#ifdef _TFM_CPP_
	if(!ReadFileTFM(pFileName,root,pCycle,pEmission,pReception,pCorrection))
		bRet = false;
//#endif _TFM_CPP_

	if(pCycle)
		delete [] pCycle;
	pCycle = NULL;
	if(pEmission)
		delete [] pEmission;
	pEmission = NULL;
	if(pReception)
		delete [] pReception;
	pReception = NULL;

	if(bRet && bTextFile && pCallbackCustomizedAPI)
		(*pCallbackCustomizedAPI)(pHWDeviceOEMPA,root.wcFileName,eReadFileWriteHW_Leave,&root,pCycle,pEmission,pReception);

	if(pHWDeviceOEMPA && bRet)
	{
		pSWDevice = pHWDeviceOEMPA->GetSWDevice();
		if(pSWDevice)
			pSWDevice->SetSetupFileCurrent(pFileName);
	}

	if(bAllocParamList && root.rParamList)
	{
		delete root.rParamList;
		root.rParamList = NULL;
	}

	return bRet;
}

bool OEMPA_InitRoot256(bool bMaster,bool &bAllocParamList, wchar_t *pFileName, CHWDeviceOEMPA *pHWDeviceOEMPA, structRoot &root, structCycle* &pCycle, CFocalLaw* &pEmission, CFocalLaw* &pReception)
{
	bool bTextFile;
	int iEnable256;

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
	if(!IsValidFileAnsi(pFileName))
	{
		OempaApiMessageBox(NULL,L"Unicode file prohibited!",L"File Error",MB_OK);
		return false;
	}
	if(!IsValidFileExtension(pFileName,bTextFile) || !bTextFile)
	{
		OempaApiMessageBox(NULL,L"Bad file extension!",L"File Error",MB_OK);
		return false;
	}
	wcscpy_s(root.wcFileName,MAX_PATH,pFileName);
	OEMPA_InitRoot(root, pHWDeviceOEMPA);
	if(!OEMPA_ReadFileText(pFileName,root,pCycle,pEmission,pReception))
	{
		OempaApiMessageBox(NULL,L"Read data error!",L"File Error",MB_OK);
		return false;
	}
	if(!root.rParamList->FileReadInteger(L"Root",L"Enable256",pFileName,iEnable256,false))
		iEnable256 = 0;
	if(iEnable256)
	{
		if(bMaster)
			root.b256Master = true;
		else
			root.b256Slave = true;
	}else{
		root.b256Master = false;
		root.b256Slave = false;
	}
	return true;
}

__declspec(dllexport) bool WINAPI OEMPA_ReadFileWriteHW256(CHWDeviceOEMPA *pHWMaster, CHWDeviceOEMPA *pHWSlave, wchar_t *pFileNameMaster, wchar_t *pFileNameSlave)
//output return true if no error and false in case of error
{
	pCallbackCustomizedDriverAPI pCallbackCustomizedAPI=OEMPA_GetCallbackCustomizedDriverAPI();
	structCorrectionOEMPA *pCorrection=NULL;
	structRoot rootMaster;
	structCycle *pCycleMaster=NULL;
	CFocalLaw *pEmissionMaster=NULL;
	CFocalLaw *pReceptionMaster=NULL;
	structRoot rootSlave;
	structCycle *pCycleSlave=NULL;
	CFocalLaw *pEmissionSlave=NULL;
	CFocalLaw *pReceptionSlave=NULL;
	bool bAllocParamListMaster=false,bAllocParamListSlave=false;
	bool bRet=true;
	const wchar_t *wcCorrected=pFileNameMaster;
	long lPointFactor=-2;
	enumCompressionType eCompressionType;
	CFocalLaw *pFocalLaw;

	if((!pHWMaster) || (!pHWSlave))
		return false;
	if(!OEMPA_InitRoot256(true, bAllocParamListMaster, pFileNameMaster, pHWMaster, rootMaster, pCycleMaster, pEmissionMaster, pReceptionMaster))
		goto return_false;
	if(!OEMPA_InitRoot256(false, bAllocParamListSlave, pFileNameSlave, pHWSlave, rootSlave, pCycleSlave, pEmissionSlave, pReceptionSlave))
		goto return_false;

	if(rootMaster.iCycleCount!=rootSlave.iCycleCount)
		goto return_false;
	if(!pReceptionMaster || !pReceptionSlave)
		goto return_false;
	if(pCallbackCustomizedAPI)
		pCorrection = (*pCallbackCustomizedAPI)(pHWMaster,rootMaster.wcFileName,eReadFileWriteHW_Enter, &rootMaster, pCycleMaster, pEmissionMaster, pReceptionMaster);

	if(	(rootMaster.b256Master && !rootSlave.b256Slave) ||
		(!rootMaster.b256Master && rootSlave.b256Slave))
		goto return_false;
	if(rootMaster.b256Master && rootSlave.b256Slave)
	{
		for(int iCycleIndex=0;iCycleIndex<rootMaster.iCycleCount;iCycleIndex++)
		{
			if(	(pReceptionMaster[iCycleIndex].iFocalCount>0) &&
				(pReceptionSlave[iCycleIndex].iFocalCount>0) &&
				(pReceptionMaster[iCycleIndex].iFocalCount != pReceptionSlave[iCycleIndex].iFocalCount))
				goto return_false;
			pReceptionMaster[iCycleIndex].m_p256MatchedDevice = &pReceptionSlave[iCycleIndex];
			pReceptionSlave[iCycleIndex].m_p256MatchedDevice = &pReceptionMaster[iCycleIndex];
		}
	}

	if(bRet && !OEMPA_WriteHW(pHWMaster, rootMaster, pCycleMaster, pEmissionMaster, pReceptionMaster,eAcqOff))
	{
		OempaApiMessageBox(NULL,L"Write data error!",L"HW error",MB_OK);
		goto return_false;
	}
	if(bRet && !OEMPA_WriteHW(pHWSlave,rootSlave, pCycleSlave, pEmissionSlave, pReceptionSlave, eAcqOff))
	{
		OempaApiMessageBox(NULL,L"Write data error!",L"HW error",MB_OK);
		goto return_false;
	}

	if(pCorrection)
	{
		//additional correction information placed in text file only.
		if(ReadCorrectedFileText(wcCorrected, rootMaster, pCycleMaster, pEmissionMaster, pReceptionMaster))
		{
			pCorrection->iCount = rootMaster.iCycleCount;
			pCorrection->dSpecimenVelocity = rootMaster.dSpecimenVelocity;
			pCorrection->dDigitizingPeriod = rootMaster.dDigitizingPeriod;
			pCorrection->dSamplingFrequency = 1.0/pCorrection->dDigitizingPeriod;
			for(int iCycle=0;iCycle<rootMaster.iCycleCount;iCycle++)
			{
				pFocalLaw = &pEmissionMaster[iCycle];
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
				pFocalLaw = &pReceptionMaster[iCycle];
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
				pCorrection->piCountRecv[iCycle] = (int)pCycleMaster[iCycle].lPointCount;
				//1.1.5.0n if(pCycle[iCycle].lPointFactor != 256)
				{
					if((lPointFactor!=-2) && ((pCycleMaster[iCycle].lPointFactor != lPointFactor) || (pCycleMaster[iCycle].eCompressionType!=eCompressionType)))
					{
						//all cycle dont have same values.
						lPointFactor = -1;
						eCompressionType = eCompression;
					}else{
						lPointFactor = pCycleMaster[iCycle].lPointFactor;
						eCompressionType = pCycleMaster[iCycle].eCompressionType;
					}
				}
			}
			pCorrection->dSpecimenRadius = rootMaster.dSpecimenRadius;
			pCorrection->dSpecimenThickness = rootMaster.dSpecimenThickness;
			//1.1.5.0n if((lPointFactor != 256) && (lPointFactor != -2))
			if(lPointFactor != -2)
			{
				if(!pHWMaster->GetSamplingFrequency(eCompressionType,lPointFactor,pCorrection->dSamplingFrequency))
					pCorrection->dSamplingFrequency = 1.0/pCorrection->dDigitizingPeriod;
			}
		}else
			pCorrection->iCount = 0;
	}
	bRet = true;

return_false:
	if(pCycleMaster)
		delete [] pCycleMaster;
	pCycleMaster = NULL;
	if(pEmissionMaster)
		delete [] pEmissionMaster;
	pEmissionMaster = NULL;
	if(pReceptionMaster)
		delete [] pReceptionMaster;
	pReceptionMaster = NULL;
	if(pCycleSlave)
		delete [] pCycleSlave;
	pCycleSlave = NULL;
	if(pEmissionSlave)
		delete [] pEmissionSlave;
	pEmissionSlave = NULL;
	if(pReceptionSlave)
		delete [] pReceptionSlave;
	pReceptionSlave = NULL;
	return bRet;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

_CUST_DRV_DEF_ bool WINAPI OEMPA_ConvertFileFormat(CHWDeviceOEMPA *pHWDeviceOEMPA,const wchar_t *pFileName,const wchar_t *pFileNew)
{
	bool bTextFile;
	bool bRet=true;
	structRoot root;
	structCycle *pCycle=NULL;
	CFocalLaw *pEmission=NULL;
	CFocalLaw *pReception=NULL;
 
	OEMPA_InitRoot(root,pHWDeviceOEMPA);
	if(!IsValidFileAnsi(pFileName))
	{
		OempaApiMessageBox(NULL,L"Unicode file prohibited!",L"File Error",MB_OK);
		return false;
	}
	if(!IsValidFileExtension(pFileNew,bTextFile))
	{
		OempaApiMessageBox(NULL,L"Bad file extension!",L"File Error",MB_OK);
		return false;
	}
	if(!IsValidFileExtension(pFileName,bTextFile))
	{
		OempaApiMessageBox(NULL,L"Bad file extension!",L"File Error",MB_OK);
		return false;
	}

	if(bTextFile)
	{
		if(!OEMPA_ReadFileText(pFileName,root,pCycle,pEmission,pReception))
		{
			OempaApiMessageBox(NULL,L"Read data error!",L"File Error",MB_OK);
			bRet = false;
		}else{
			if(!OEMPA_WriteFileBinary(pFileNew,&root,pCycle,pEmission,pReception))
			{
				OempaApiMessageBox(NULL,L"Write data error!",L"File Error",MB_OK);
				bRet = false;
			}
		}
	}
	if(!bTextFile)
	{
		if(!OEMPA_ReadFileBinary(pFileName,root,pCycle,pEmission,pReception))
		{
			OempaApiMessageBox(NULL,L"Read data error!",L"File Error",MB_OK);
			bRet = false;
		}else{
			if(!OEMPA_WriteFileText(pFileNew,&root,pCycle,pEmission,pReception))
			{
				OempaApiMessageBox(NULL,L"Write data error!",L"File Error",MB_OK);
				bRet = false;
			}
		}
	}

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

/////////////////////////////////////////////////////////////////////////////
