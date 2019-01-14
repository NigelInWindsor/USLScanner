#include "stdafx.h"
#include "UTKernelBasic.h"//because of "UTKernel_GetFolderExe"

void EditFile(wchar_t *pFile,bool bCloseWaiting);
void CheckCarriageReturnNotepad(const wchar_t *strFileNameIn,const wchar_t *strFileNameOut);
bool FileReadInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int &iData,bool bDisplayMsg=true,int iMin=INT_MIN,int iMax=INT_MAX);

//those function are example how to use device functions.
//FW parameters are written to the FW and then they are readback
//the last operation check if the returned SW parameters are
//the same than the sent one.

void APISamplesReceptionFocusing(CHWDeviceOEMPA *pHWDeviceOEMPA);
void APISamplesEmissionFocusing(CHWDeviceOEMPA *pHWDeviceOEMPA);
void APISamplesEmissionWedgeDelay(CHWDeviceOEMPA *pHWDeviceOEMPA);
void APISamplesReceptionWedgeDelay(CHWDeviceOEMPA *pHWDeviceOEMPA);
void APISamplesReceptionBeam(CHWDeviceOEMPA *pHWDeviceOEMPA);
void APISamplesDACStandard(CHWDeviceOEMPA *pHWDeviceOEMPA);
void APISamplesDACCallback(CHWDeviceOEMPA *pHWDeviceOEMPA);
void APISamplesGate(CHWDeviceOEMPA *pHWDeviceOEMPA);


const int my_iReceptionCycleCountMax=10;
const int my_iReceptionFocalCountMax=4;
const int my_iReceptionElementCountMax=16;

const int my_iEmissionCycleCountMax=10;
const int my_iEmissionElementCountMax=16;

const int my_iDACCycleCountMax=10;
const int my_iDACPointCountMax=16;

const int my_iGateCycleCountMax=10;

bool DifferentFloat(float fData1,float fData2,float fDelta)
{
	if(fData1<fData2)
	{
		if(fData2-fData1>fDelta)
			return true;
	}else{
		if(fData1-fData2>fDelta)
			return true;
	}
	return false;
}
bool DifferentDouble(double dData1,double dData2,double fDelta)
{
	if(dData1<dData2)
	{
		if(dData2-dData1>fDelta)
			return true;
	}else{
		if(dData1-dData2>fDelta)
			return true;
	}
	return false;
}


////////////////////////////////////////////////////////////////////////////
bool WINAPI APISamplesCallbackSetSize2(structCallbackArrayFloat2D *pCallbackArray,int iSize1,int iSize2)
{
	float *pfDelay=(float*)pCallbackArray->apParameter[0];
	int *piElementCount=(int*)pCallbackArray->apParameter[1];
	int *piFocalCount=(int*)pCallbackArray->apParameter[2];

	if(piElementCount && (iSize1<=my_iReceptionElementCountMax))
		*piElementCount = iSize1;
	else
		return false;
	if(piFocalCount && (iSize2<=my_iReceptionFocalCountMax))
		*piFocalCount = iSize2;
	else
		return false;
	return true;
}
bool WINAPI APISamplesCallbackGetSize2(structCallbackArrayFloat2D *pCallbackArray,int &iSize1,int &iSize2)
{
	float *pfDelay=(float*)pCallbackArray->apParameter[0];
	int *piElementCount=(int*)pCallbackArray->apParameter[1];
	int *piFocalCount=(int*)pCallbackArray->apParameter[2];

	if(!piElementCount)
		return false;
	if(!piFocalCount)
		return false;
	iSize1 = *piElementCount;
	iSize2 = *piFocalCount;
	return true;
}
bool WINAPI APISamplesCallbackSetData2(structCallbackArrayFloat2D *pCallbackArray,int iIndex1,int iIndex2,float fData)
{
	float *pfDelay=(float*)pCallbackArray->apParameter[0];
	int *piElementCount=(int*)pCallbackArray->apParameter[1];
	int *piFocalCount=(int*)pCallbackArray->apParameter[2];

	if(!piElementCount)
		return false;
	if(!piFocalCount)
		return false;
	if(iIndex1<0)
		return false;
	if(iIndex1>=*piElementCount)
		return false;
	if(iIndex2<0)
		return false;
	if(iIndex2>=*piFocalCount)
		return false;
	pfDelay[iIndex2*my_iReceptionElementCountMax+iIndex1] = fData;
	return true;
}
bool WINAPI APISamplesCallbackGetData2(structCallbackArrayFloat2D *pCallbackArray,int iIndex1,int iIndex2,float &fData)
{
	float *pfDelay=(float*)pCallbackArray->apParameter[0];
	int *piElementCount=(int*)pCallbackArray->apParameter[1];
	int *piFocalCount=(int*)pCallbackArray->apParameter[2];

	if(!piElementCount)
		return false;
	if(!piFocalCount)
		return false;
	if(iIndex1<0)
		return false;
	if(iIndex1>=*piElementCount)
		return false;
	if(iIndex2<0)
		return false;
	if(iIndex2>=*piFocalCount)
		return false;
	fData = pfDelay[iIndex2*my_iReceptionElementCountMax+iIndex1];
	return true;
}

////////////////////////////////////////////////////////////////////////////
typedef struct structDacPoint
{
	double dTime;
	float fSlope;
} structDacPoint;
typedef struct structDac
{
	int iPointCount;
	structDacPoint point[my_iDACPointCountMax];
} structDac;

bool WINAPI APISamplesCallbackSetSizeDac(structCallbackArrayFloatDac *pCallbackArray,int iSize)
{
	structDac *pDac=(structDac*)pCallbackArray->apParameter[0];

	if(!pDac)
		return false;
	if(iSize<=my_iDACPointCountMax)
		pDac->iPointCount = iSize;
	return true;
}
bool WINAPI APISamplesCallbackGetSizeDac(structCallbackArrayFloatDac *pCallbackArray,int &iSize)
{
	structDac *pDac=(structDac*)pCallbackArray->apParameter[0];

	if(!pDac)
		return false;
	iSize = pDac->iPointCount;
	return true;
}
bool WINAPI APISamplesCallbackSetDataDac(structCallbackArrayFloatDac *pCallbackArray,int iIndex,double dTime,float fSlope)
{
	structDac *pDac=(structDac*)pCallbackArray->apParameter[0];

	if(!pDac)
		return false;
	if(iIndex<0)
		return false;
	if(iIndex>=pDac->iPointCount)
		return false;
	pDac->point[iIndex].dTime = dTime;
	pDac->point[iIndex].fSlope = fSlope;
	return true;
}
bool WINAPI APISamplesCallbackGetDataDac(structCallbackArrayFloatDac *pCallbackArray,int iIndex,double &dTime,float &fSlope)
{
	structDac *pDac=(structDac*)pCallbackArray->apParameter[0];

	if(!pDac)
		return false;
	if(iIndex<0)
		return false;
	if(iIndex>=pDac->iPointCount)
		return false;
	dTime = pDac->point[iIndex].dTime;
	fSlope = pDac->point[iIndex].fSlope;
	return true;
}

////////////////////////////////////////////////////////////////////////////
void APISamples(CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	CString string;
	int iValue;

	string.Format(L"%sAPISamples.txt",UTKernel_GetFolderDataFiles(L"OEMPA"));
	try{
		CStdioFile file(string,CFile::typeText | CFile::modeWrite | CFile::modeCreate);
		file.WriteString(L"#API Samples : select one of the following items and set the value to 1 (single value)\n");
		file.WriteString(L"#To continue please save the file and close notepad.\n");
		file.WriteString(L"\n");
		file.WriteString(L"[APISamples]\n");
		file.WriteString(L"ReceptionFocusing=0\n");
		file.WriteString(L"EmissionFocusing=0\n");
		file.WriteString(L"EmissionWedgeDelay=0\n");
		file.WriteString(L"ReceptionWedgeDelay=0\n");
		file.WriteString(L"ReceptionBeam=0\t\t\t#Gain, AscanRange, GateMode.\n");
		file.WriteString(L"DAC_standard=0\t\t\t#set the DAC with two standard floatings arrays.\n");
		file.WriteString(L"DAC_callback=0\t\t\t#set the DAC with callback functions and any variable of your choice.\n");
		file.WriteString(L"ReceptionGate=0\n");
		file.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	EditFile(string.GetBuffer(),true);
	CheckCarriageReturnNotepad(string,string);
	UTKernel_SetFolderDataFiles(L"OEMPA",string.GetBuffer());
	if(FileReadInteger(L"APISamples",L"ReceptionFocusing",string.GetBuffer(),iValue) && (iValue==1))
	{
		APISamplesReceptionFocusing(pHWDeviceOEMPA);
		return;
	}
	if(FileReadInteger(L"APISamples",L"EmissionFocusing",string.GetBuffer(),iValue) && (iValue==1))
	{
		APISamplesEmissionFocusing(pHWDeviceOEMPA);
		return;
	}
	if(FileReadInteger(L"APISamples",L"EmissionWedgeDelay",string.GetBuffer(),iValue) && (iValue==1))
	{
		APISamplesEmissionWedgeDelay(pHWDeviceOEMPA);
		return;
	}
	if(FileReadInteger(L"APISamples",L"ReceptionWedgeDelay",string.GetBuffer(),iValue) && (iValue==1))
	{
		APISamplesReceptionWedgeDelay(pHWDeviceOEMPA);
		return;
	}
	if(FileReadInteger(L"APISamples",L"ReceptionBeam",string.GetBuffer(),iValue) && (iValue==1))
	{
		APISamplesReceptionBeam(pHWDeviceOEMPA);
		return;
	}
	if(FileReadInteger(L"APISamples",L"DAC_standard",string.GetBuffer(),iValue) && (iValue==1))
	{
		APISamplesDACStandard(pHWDeviceOEMPA);
		return;
	}
	if(FileReadInteger(L"APISamples",L"DAC_callback",string.GetBuffer(),iValue) && (iValue==1))
	{
		APISamplesDACCallback(pHWDeviceOEMPA);
		return;
	}
	if(FileReadInteger(L"APISamples",L"ReceptionGate",string.GetBuffer(),iValue) && (iValue==1))
	{
		APISamplesGate(pHWDeviceOEMPA);
		return;
	}
	AfxMessageBox(L"Failed : no item has been selected!");
}

void APISamplesReceptionFocusing(CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	DWORD adwApertureWr[g_iOEMPAApertureDWordCount];
	DWORD adwApertureReq[my_iReceptionCycleCountMax][g_iOEMPAApertureDWordCount];
	float pDelayWr[my_iReceptionCycleCountMax][my_iReceptionFocalCountMax][my_iReceptionElementCountMax];
	float pDelayReq[my_iReceptionCycleCountMax][my_iReceptionFocalCountMax][my_iReceptionElementCountMax];
	double pFocalTimeOfFlightWr[my_iReceptionCycleCountMax][my_iReceptionFocalCountMax];
	double pFocalTimeOfFlightReq[my_iReceptionCycleCountMax][my_iReceptionFocalCountMax];
	float pGainWr[my_iReceptionCycleCountMax][my_iReceptionElementCountMax];
	float pGainReq[my_iReceptionCycleCountMax][my_iReceptionElementCountMax];
	int aiFocalTimeOfFlightCountReq[my_iReceptionCycleCountMax];
	int aiGainCountReq[my_iReceptionCycleCountMax],iGainCountReq;
	long lCycleCountWr=10;
	long lCycleCountReq=0;
	int iError;
	int iCycleCount;
	int iFocalCountWr,iElementCountWr;
	int iFocalCountReq[my_iReceptionCycleCountMax],iElementCountReq[my_iReceptionCycleCountMax];
	int iAuxElementCountReq,iAuxFocalCountReq,iAuxFocalTimeOfFlightCountReq;
	structCallbackArrayFloat2D callbackArrayFloat2D;

	if(!pHWDeviceOEMPA)
		return;
	iError = 0;
	if(!pHWDeviceOEMPA->LockDevice())
		return;

	CHWDeviceOEMPA::SetDDFWaveTrackingCorrection(0);//because delays are ot ultrasound delays but only mathematical delay.
																	//with a real UT probe and delay do NOT call this function.
	//write data
	iCycleCount = my_iReceptionCycleCountMax;//you can change the value, range is 1 to "my_iReceptionCycleCountMax".
	iFocalCountWr = my_iReceptionFocalCountMax;//you can change the value, range is 1 to "my_iReceptionFocalCountMax".
	iElementCountWr = my_iReceptionElementCountMax;//you can change the value, range is 1 to "my_iReceptionElementCountMax".
	if(!CHWDeviceOEMPA::SetAllElementEnable(false,adwApertureWr))
		return;
	callbackArrayFloat2D.pSetSize = APISamplesCallbackSetSize2;
	callbackArrayFloat2D.pSetData = APISamplesCallbackSetData2;
	callbackArrayFloat2D.pGetSize = APISamplesCallbackGetSize2;
	callbackArrayFloat2D.pGetData = APISamplesCallbackGetData2;
	for(int iElementIndex=0;iElementIndex<iElementCountWr;iElementIndex++)
	{
		CHWDeviceOEMPA::SetElementEnable(iElementIndex,true,adwApertureWr);
	}
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		for(int iFocalIndex=0;iFocalIndex<iFocalCountWr;iFocalIndex++)
		{
			//iFocalIndex=0 : first focal law (used even if the DDF is not disabled).
			pFocalTimeOfFlightWr[iCycleIndex][iFocalIndex] = (float)(iCycleIndex*2.0e-6f+iFocalIndex*2.0e-6f);
			pFocalTimeOfFlightReq[iCycleIndex][iFocalIndex] = 0.0f;
			for(int iElementIndex=0;iElementIndex<iElementCountWr;iElementIndex++)
			{
				pDelayWr[iCycleIndex][iFocalIndex][iElementIndex] = (float)(iCycleIndex*1.0e-6f+iElementIndex*0.1e-6f+iFocalIndex*1.0e-6f);
				pDelayReq[iCycleIndex][iFocalIndex][iElementIndex] = 0.0f;
			}
		}
		for(int iElementIndex=0;iElementIndex<iElementCountWr;iElementIndex++)
		{
			pGainWr[iCycleIndex][iElementIndex] = (float)(iCycleIndex+iElementIndex*0.1e-6f);
			pGainReq[iCycleIndex][iElementIndex] = 0.0f;
		}

		if(!CHWDeviceOEMPA::SetAllElementEnable(false,adwApertureReq[iCycleIndex]))
			{iError = 1000;goto return_error;}
		if(!pHWDeviceOEMPA->IsMultiplexer())
		{
			if(!pHWDeviceOEMPA->SetReceptionEnable(iCycleIndex,adwApertureWr))
				{iError = 1000;goto return_error;}
		}else{
			if(!pHWDeviceOEMPA->SetMultiplexerEnable(iCycleIndex,adwApertureWr))
				{iError = 1000;goto return_error;}
		}
		if(!pHWDeviceOEMPA->SetReceptionGains(iCycleIndex,adwApertureWr,&pGainWr[iCycleIndex][0]))
			{iError = 1000;goto return_error;}
		if(!pHWDeviceOEMPA->SetReceptionFocusing(iCycleIndex,iFocalCountWr,&pFocalTimeOfFlightWr[iCycleIndex][0]))
			{iError = 1000;goto return_error;}

		callbackArrayFloat2D.apParameter[0] = &pDelayWr[iCycleIndex][0][0];
		callbackArrayFloat2D.apParameter[1] = &iElementCountWr;
		callbackArrayFloat2D.apParameter[2] = &iFocalCountWr;
		if(!pHWDeviceOEMPA->SetReceptionDelays(iCycleIndex,adwApertureWr,callbackArrayFloat2D))
			{iError = 1000;goto return_error;}
	}

	//request data
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		if(!pHWDeviceOEMPA->IsMultiplexer())
		{
			if(!pHWDeviceOEMPA->GetReceptionEnable(iCycleIndex,adwApertureReq[iCycleIndex]))
				{iError = 1000;goto return_error;}
		}else{
			if(!pHWDeviceOEMPA->GetMultiplexerEnable(iCycleIndex,adwApertureReq[iCycleIndex]))
				{iError = 1000;goto return_error;}
		}
		iGainCountReq = my_iReceptionElementCountMax;
		if(!pHWDeviceOEMPA->GetReceptionGains(iCycleIndex,iGainCountReq,&aiGainCountReq[iCycleIndex],&pGainReq[iCycleIndex][0]))
			{iError = 1000;goto return_error;}
		iAuxFocalTimeOfFlightCountReq = iFocalCountWr;//my_iReceptionFocalCountMax;
		if(!pHWDeviceOEMPA->GetReceptionFocusing(iCycleIndex,iAuxFocalTimeOfFlightCountReq,&aiFocalTimeOfFlightCountReq[iCycleIndex],&pFocalTimeOfFlightReq[iCycleIndex][0]))
			{iError = 1000;goto return_error;}

		callbackArrayFloat2D.apParameter[0] = &pDelayReq[iCycleIndex][0][0];
		callbackArrayFloat2D.apParameter[1] = &iElementCountReq[iCycleIndex];
		callbackArrayFloat2D.apParameter[2] = &iFocalCountReq[iCycleIndex];
		iAuxElementCountReq = iElementCountWr;//my_iReceptionElementCountMax;
		iAuxFocalCountReq = iFocalCountWr;//my_iReceptionFocalCountMax;
		if(!pHWDeviceOEMPA->GetReceptionDelays(iCycleIndex,iAuxElementCountReq,iAuxFocalCountReq,callbackArrayFloat2D))
			{iError = 1000;goto return_error;}
	}

	//check
return_error:
	pHWDeviceOEMPA->UnlockDevice();
	CHWDeviceOEMPA::SetDDFWaveTrackingCorrection(1);

	//check the result
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		if(iElementCountWr!=iElementCountReq[iCycleIndex])
			iError++;
		for(int iApIndex=0;iApIndex<g_iOEMPAApertureDWordCount;iApIndex++)
		{
			if(adwApertureReq[iCycleIndex][iApIndex]!=adwApertureWr[iApIndex])
				iError++;
		}
		if(aiFocalTimeOfFlightCountReq[iCycleIndex]!=iFocalCountWr)
			iError++;
		if(aiGainCountReq[iCycleIndex]!=iElementCountWr)
			iError++;
		for(int iFocalIndex=0;iFocalIndex<iFocalCountWr;iFocalIndex++)
		{
			//iFocalIndex=0 : first focal law (used even if the DDF is not disabled).
			for(int iElementIndex=0;iElementIndex<iElementCountWr;iElementIndex++)
			{
				if(DifferentFloat(pDelayWr[iCycleIndex][iFocalIndex][iElementIndex],pDelayReq[iCycleIndex][iFocalIndex][iElementIndex],pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod()/10.0f))
					iError++;
			}
			if(DifferentDouble(pFocalTimeOfFlightWr[iCycleIndex][iFocalIndex],pFocalTimeOfFlightReq[iCycleIndex][iFocalIndex],pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod()/10.0f))
				iError++;
		}
		for(int iElementIndex=0;iElementIndex<iElementCountWr;iElementIndex++)
		{
			if(DifferentFloat(pGainWr[iCycleIndex][iElementIndex],pGainReq[iCycleIndex][iElementIndex],0.01f))
				iError++;
		}
	}
	if(iError)
		AfxMessageBox(L"Error to readback some parameters!");
	else
		AfxMessageBox(L"APISamplesReceptionFocusing successful!");
}


void APISamplesEmissionFocusing(CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	DWORD adwApertureWr[g_iOEMPAApertureDWordCount];
	DWORD adwApertureReq[my_iEmissionCycleCountMax][g_iOEMPAApertureDWordCount];
	float pDelayWr[my_iEmissionCycleCountMax][my_iEmissionElementCountMax];
	float pDelayReq[my_iEmissionCycleCountMax][my_iEmissionElementCountMax];
	float pWidthWr[my_iEmissionCycleCountMax][my_iEmissionElementCountMax];
	float pWidthReq[my_iEmissionCycleCountMax][my_iEmissionElementCountMax];
	int aiWidthCountReq[my_iEmissionCycleCountMax],iWidthCountReq;
	int aiDelayCountReq[my_iEmissionCycleCountMax];
	long lCycleCountWr=10;
	long lCycleCountReq=0;
	int iError;
	int iCycleCount;
	int iElementCountWr;
	int iAuxElementCountReq;

	if(!pHWDeviceOEMPA)
		return;
	iError = 0;
	if(!pHWDeviceOEMPA->LockDevice())
		return;

	//write data
	iCycleCount = my_iEmissionCycleCountMax;//you can change the value, range is 1 to "my_iEmissionCycleCountMax".
	iElementCountWr = my_iEmissionElementCountMax;//you can change the value, range is 1 to "my_iEmissionElementCountMax".
	if(!CHWDeviceOEMPA::SetAllElementEnable(false,adwApertureWr))
		return;
	for(int iElementIndex=0;iElementIndex<iElementCountWr;iElementIndex++)
	{
		CHWDeviceOEMPA::SetElementEnable(iElementIndex,true,adwApertureWr);
	}
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		for(int iElementIndex=0;iElementIndex<iElementCountWr;iElementIndex++)
		{
			pDelayWr[iCycleIndex][iElementIndex] = (float)(iCycleIndex*1.0e-6f+iElementIndex*0.1e-6f);
			pDelayReq[iCycleIndex][iElementIndex] = 0.0f;
			pWidthWr[iCycleIndex][iElementIndex] = (float)(iCycleIndex*0.25e-6f+iElementIndex*0.1e-6f);
			pWidthReq[iCycleIndex][iElementIndex] = 0.0f;
		}

		if(CHWDeviceOEMPA::SetAllElementEnable(false,adwApertureReq[iCycleIndex]))
			return;
		if(!pHWDeviceOEMPA->IsMultiplexer())
		{
			if(!pHWDeviceOEMPA->SetEmissionEnable(iCycleIndex,adwApertureWr))
				{iError = 1000;goto return_error;}
		}else{
			if(!pHWDeviceOEMPA->SetMultiplexerEnable(iCycleIndex,adwApertureWr))
				{iError = 1000;goto return_error;}
		}
		if(!pHWDeviceOEMPA->SetEmissionWidths(iCycleIndex,adwApertureWr,&pWidthWr[iCycleIndex][0]))
			{iError = 1000;goto return_error;}

		if(!pHWDeviceOEMPA->SetEmissionDelays(iCycleIndex,adwApertureWr,&pDelayWr[iCycleIndex][0]))
			{iError = 1000;goto return_error;}
	}

	//request data
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		if(!pHWDeviceOEMPA->IsMultiplexer())
		{
			if(!pHWDeviceOEMPA->GetEmissionEnable(iCycleIndex,adwApertureReq[iCycleIndex]))
				{iError = 1000;goto return_error;}
		}else{
			if(!pHWDeviceOEMPA->GetMultiplexerEnable(iCycleIndex,adwApertureWr))
				{iError = 1000;goto return_error;}
		}
		iWidthCountReq = my_iEmissionElementCountMax;
		if(!pHWDeviceOEMPA->GetEmissionWidths(iCycleIndex,iWidthCountReq,&aiWidthCountReq[iCycleIndex],&pWidthReq[iCycleIndex][0]))
			{iError = 1000;goto return_error;}

		iAuxElementCountReq = my_iEmissionElementCountMax;
		if(!pHWDeviceOEMPA->GetEmissionDelays(iCycleIndex,iAuxElementCountReq,&aiDelayCountReq[iCycleIndex],&pDelayReq[iCycleIndex][0]))
			{iError = 1000;goto return_error;}
	}

	//check
return_error:
	pHWDeviceOEMPA->UnlockDevice();
	//check the result
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		for(int iApIndex=0;iApIndex<g_iOEMPAApertureDWordCount;iApIndex++)
		{
			if(adwApertureReq[iCycleIndex][iApIndex]!=adwApertureWr[iApIndex])
				iError++;
		}
		if(aiDelayCountReq[iCycleIndex]!=iElementCountWr)
			iError++;
		if(aiWidthCountReq[iCycleIndex]!=iElementCountWr)
			iError++;
		for(int iElementIndex=0;iElementIndex<iElementCountWr;iElementIndex++)
		{
			if(DifferentFloat(pDelayWr[iCycleIndex][iElementIndex],pDelayReq[iCycleIndex][iElementIndex],pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod()/10.0f))
				iError++;
		}
		for(int iElementIndex=0;iElementIndex<iElementCountWr;iElementIndex++)
		{
			if(DifferentFloat(pWidthWr[iCycleIndex][iElementIndex],pWidthReq[iCycleIndex][iElementIndex],0.01f))
				iError++;
		}
	}
	if(iError)
		AfxMessageBox(L"Error to readback some parameters!");
	else
		AfxMessageBox(L"APISamplesEmissionFocusing successful!");
}

void APISamplesEmissionWedgeDelay(CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	double dWedgeDelayWr[10];
	double dWedgeDelayReq[10];
	int iCycleCountWr=10;
	int iCycleCountReq=0;
	int iError;

	iError = 0;
	if(!pHWDeviceOEMPA->LockDevice())
		return;

	//write data
	for(int iCycle=0;iCycle<10;iCycle++)
	{
		dWedgeDelayWr[iCycle] = (double)(iCycle+1)*1.0e-6;
		if(!pHWDeviceOEMPA->SetEmissionWedgeDelay(iCycle,10,dWedgeDelayWr[iCycle]))
			{iError = 1000;goto return_error;}
	}
	if(!pHWDeviceOEMPA->SetCycleCount(iCycleCountWr))
		{iError = 1000;goto return_error;}

	//request data
	if(!pHWDeviceOEMPA->GetCycleCount(&iCycleCountReq))
		{iError = 1000;goto return_error;}
	for(int iCycle=0;iCycle<10;iCycle++)
	{
		dWedgeDelayReq[iCycle] = 0.0;
		if(!pHWDeviceOEMPA->GetEmissionWedgeDelay(iCycle,10,&dWedgeDelayReq[iCycle]))
			{iError = 1000;goto return_error;}
	}

	//check
return_error:
	pHWDeviceOEMPA->UnlockDevice();
	//check the result
	if(iCycleCountWr!=iCycleCountReq)
		iError++;
	for(int iCycle=0;iCycle<10;iCycle++)
	{
		if(dWedgeDelayWr[iCycle]!=dWedgeDelayReq[iCycle])
			iError++;
	}
	if(iError)
		AfxMessageBox(L"Error to readback some parameters!");
	else
		AfxMessageBox(L"APISamplesEmissionWedgeDelay successful!");
}

void APISamplesReceptionWedgeDelay(CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	double dWedgeDelayWr[10];
	double dWedgeDelayReq[10];
	int iCycleCountWr=10;
	int iCycleCountReq=0;
	int iError;

	iError = 0;
	if(!pHWDeviceOEMPA->LockDevice())
		return;

	//write data
	for(int iCycle=0;iCycle<10;iCycle++)
	{
		dWedgeDelayWr[iCycle] = (double)(iCycle+1)*1.0e-6;
		if(!pHWDeviceOEMPA->SetReceptionWedgeDelay(iCycle,10,dWedgeDelayWr[iCycle]))
			{iError = 1000;goto return_error;}
	}
	if(!pHWDeviceOEMPA->SetCycleCount(iCycleCountWr))
		{iError = 1000;goto return_error;}

	//request data
	if(!pHWDeviceOEMPA->GetCycleCount(&iCycleCountReq))
		{iError = 1000;goto return_error;}
	for(int iCycle=0;iCycle<10;iCycle++)
	{
		dWedgeDelayReq[iCycle] = 0.0;
		if(!pHWDeviceOEMPA->GetReceptionWedgeDelay(iCycle,10,&dWedgeDelayReq[iCycle]))
			{iError = 1000;goto return_error;}
	}

	//check
return_error:
	pHWDeviceOEMPA->UnlockDevice();
	//check the result
	if(iCycleCountWr!=iCycleCountReq)
		iError++;
	for(int iCycle=0;iCycle<10;iCycle++)
	{
		if(dWedgeDelayWr[iCycle]!=dWedgeDelayReq[iCycle])
			iError++;
	}
	if(iError)
		AfxMessageBox(L"Error to readback some parameters!");
	else
		AfxMessageBox(L"APISamplesReceptionWedgeDelay successful!");
}

void APISamplesReceptionBeam(CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	double dGainWr;
	double dGainReq;
	double dRangeWr=12.3e-6;
	double dRangeReq;
	enumCompressionType eCompressionTypeWr=eDecimation;
	enumCompressionType eCompressionTypeReq;
	long lFactorWr=10,lPointWr=0;
	long lFactorReq,lPointReq;
	bool bEnableGateWr,bEnableGateReq;
	enumGateModeAmp eGateModeAmpWr,eGateModeAmpReq;
	enumGateModeTof eGateModeTofWr,eGateModeTofReq;
	enumRectification eGateRectificationWr,eGateRectificationReq;
	double dThresholdPercentWr,dThresholdPercentReq;
	int iError;

	iError = 0;
	if(!pHWDeviceOEMPA->LockDevice())
		return;

	//write data
	bEnableGateWr = true;
	eGateModeAmpWr = eAmpPeakToPeak;
	eGateModeTofWr = eTofZeroFirstAfterThresholdCross;
	eGateRectificationWr = eUnsigned;
	dThresholdPercentWr = 50.0;
	dGainWr = 11.1;
	if(!pHWDeviceOEMPA->SetGainDigital(0,dGainWr))
		{iError = 1000;goto return_error;}
	if(!pHWDeviceOEMPA->SetAscanRangeWithFactor(0,dRangeWr,eCompressionTypeWr,lFactorWr,lPointWr))
		{iError = 1000;goto return_error;}
	if(!pHWDeviceOEMPA->SetGateModeThreshold(0,0,bEnableGateWr,eGateModeAmpWr,eGateModeTofWr,eGateRectificationWr,dThresholdPercentWr))
		{iError = 1000;goto return_error;}

	//request data
	if(!pHWDeviceOEMPA->GetGainDigital(0,&dGainReq))
		{iError = 1000;goto return_error;}
	if(!pHWDeviceOEMPA->GetAscanRange(0,&dRangeReq,&eCompressionTypeReq,&lFactorReq,&lPointReq))
		{iError = 1000;goto return_error;}
	if(!pHWDeviceOEMPA->GetGateModeThreshold(0,0,&bEnableGateReq,&eGateModeAmpReq,&eGateModeTofReq,&eGateRectificationReq,&dThresholdPercentReq))
		{iError = 1000;goto return_error;}

	//check
return_error:
	pHWDeviceOEMPA->UnlockDevice();
	//check the result
	if(dGainWr!=dGainReq)
		iError++;
	if(dRangeWr!=dRangeReq)
		iError++;
	if(eCompressionTypeWr!=eCompressionTypeReq)
		iError++;
	if(lFactorWr!=lFactorReq)
		iError++;
	if(bEnableGateWr!=bEnableGateReq)
		iError++;
	if(eGateModeAmpWr!=eGateModeAmpReq)
		iError++;
	if(eGateModeTofWr!=eGateModeTofReq)
		iError++;
	if(eGateRectificationWr!=eGateRectificationReq)
		iError++;
	if(iError)
		AfxMessageBox(L"Error to readback some parameters!");
	else
		AfxMessageBox(L"APISamplesReceptionBeam successful!");
}

void APISamplesDACStandard(CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	int iError;
	int iCycleCount,iPointCountMax;
	double pdTimeWr[my_iDACCycleCountMax][my_iDACPointCountMax];
	float pfSlopeWr[my_iDACCycleCountMax][my_iDACPointCountMax];
	double pdTimeReq[my_iDACCycleCountMax][my_iDACPointCountMax];
	float pfSlopeReq[my_iDACCycleCountMax][my_iDACPointCountMax];
	int aiDACCountReq[my_iDACCycleCountMax];
	int iDACCountWr;

	if(!pHWDeviceOEMPA)
		return;
	iError = 0;
	if(!pHWDeviceOEMPA->LockDevice())
		return;

	//write data
	iCycleCount = my_iDACCycleCountMax;
	iDACCountWr = my_iDACPointCountMax;
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		for(int iDACIndex=0;iDACIndex<iDACCountWr;iDACIndex++)
		{
			pdTimeWr[iCycleIndex][iDACIndex] = (double)(iCycleIndex+2*iDACIndex)*pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod()*50;
			pfSlopeWr[iCycleIndex][iDACIndex] = (float)(iCycleIndex+iDACIndex)*g_fOEMPADACSlopeStep;
			pdTimeReq[iCycleIndex][iDACIndex] = 0.0;
			pfSlopeReq[iCycleIndex][iDACIndex] = 0.0f;
		}
		if(!pHWDeviceOEMPA->SetDACSlope(iCycleIndex,iDACCountWr,&pdTimeWr[iCycleIndex][0],&pfSlopeWr[iCycleIndex][0]))
			{iError = 1000;goto return_error;}
	}

	//request data
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		iPointCountMax = my_iDACPointCountMax;
		if(!pHWDeviceOEMPA->GetDACSlope(iCycleIndex,iPointCountMax,&aiDACCountReq[iCycleIndex],&pdTimeReq[iCycleIndex][0],&pfSlopeReq[iCycleIndex][0]))
			{iError = 1000;goto return_error;}
	}

	//check
return_error:
	pHWDeviceOEMPA->UnlockDevice();
	//check the result
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		if(aiDACCountReq[iCycleIndex]!=iDACCountWr)
			iError++;
		for(int iDACIndex=0;iDACIndex<iDACCountWr;iDACIndex++)
		{
			if(DifferentDouble(pdTimeWr[iCycleIndex][iDACIndex],pdTimeReq[iCycleIndex][iDACIndex],pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod()/10.0f))
				iError++;
			if(DifferentFloat(pfSlopeWr[iCycleIndex][iDACIndex],pfSlopeReq[iCycleIndex][iDACIndex],g_fOEMPADACSlopeStep/10.0f))
				iError++;
		}
	}
	if(iError)
		AfxMessageBox(L"Error to readback some parameters!");
	else
		AfxMessageBox(L"APISamplesDACStandard successful!");
}

void APISamplesDACCallback(CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	int iError;
	int iCycleCount,iPointCountMax;
	structDac dacWr[my_iDACCycleCountMax],dacReq[my_iDACCycleCountMax];
	int iDACCountWr;
	structCallbackArrayFloatDac callbackArrayFloatDac;

	if(!pHWDeviceOEMPA)
		return;
	iError = 0;
	if(!pHWDeviceOEMPA->LockDevice())
		return;

	callbackArrayFloatDac.pSetSize = APISamplesCallbackSetSizeDac;
	callbackArrayFloatDac.pSetData = APISamplesCallbackSetDataDac;
	callbackArrayFloatDac.pGetSize = APISamplesCallbackGetSizeDac;
	callbackArrayFloatDac.pGetData = APISamplesCallbackGetDataDac;

	//write data
	iCycleCount = my_iDACCycleCountMax;
	iDACCountWr = my_iDACPointCountMax;
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		dacWr[iCycleIndex].iPointCount = iDACCountWr;
		dacReq[iCycleIndex].iPointCount = 0;
		for(int iDACIndex=0;iDACIndex<iDACCountWr;iDACIndex++)
		{
			dacWr[iCycleIndex].point[iDACIndex].dTime = (float)(iCycleIndex+2*iDACIndex)*pHWDeviceOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod()*50;
			dacWr[iCycleIndex].point[iDACIndex].fSlope = (float)(iCycleIndex+iDACIndex)*g_fOEMPADACSlopeStep;
			dacReq[iCycleIndex].point[iDACIndex].dTime = 0.0f;
			dacReq[iCycleIndex].point[iDACIndex].fSlope = 0.0f;
		}
		callbackArrayFloatDac.apParameter[0] = &dacWr[iCycleIndex];
		if(!pHWDeviceOEMPA->SetDACSlope(iCycleIndex,iDACCountWr,callbackArrayFloatDac))
			{iError = 1000;goto return_error;}
	}

	//request data
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		iPointCountMax = my_iDACPointCountMax;
		callbackArrayFloatDac.apParameter[0] = &dacReq[iCycleIndex];
		if(!pHWDeviceOEMPA->GetDACSlope(iCycleIndex,iPointCountMax,callbackArrayFloatDac))
			{iError = 1000;goto return_error;}
	}

	//check
return_error:
	pHWDeviceOEMPA->UnlockDevice();
	//check the result
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		if(dacReq[iCycleIndex].iPointCount!=dacWr[iCycleIndex].iPointCount)
			iError++;
		for(int iDACIndex=0;iDACIndex<iDACCountWr;iDACIndex++)
		{
			if(DifferentDouble(dacReq[iCycleIndex].point[iDACIndex].dTime,dacWr[iCycleIndex].point[iDACIndex].dTime,pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod()/10.0f))
				iError++;
			if(DifferentFloat(dacReq[iCycleIndex].point[iDACIndex].fSlope,dacWr[iCycleIndex].point[iDACIndex].fSlope,g_fOEMPADACSlopeStep/10.0f))
				iError++;
		}
	}
	if(iError)
		AfxMessageBox(L"Error to readback some parameters!");
	else
		AfxMessageBox(L"APISamplesDACCallback successful!");
}
void TestAPISamplesDACGain(CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	int iError=0;
	int iCountMax=8,iCount=0;
	double pdTime[8]={0,0,0,0,0,0,0,0};
	float pfGain[8]={0,0,0,0,0,0,0,0};

	if(!pHWDeviceOEMPA->LockDevice())
		return;
	if(!pHWDeviceOEMPA->GetDACGain(0,iCountMax,&iCount,pdTime,pfGain))
		iError = 1;
	//check
	pHWDeviceOEMPA->UnlockDevice();
	if(!_CrtCheckMemory())
		AfxMessageBox(L"_CrtCheckMemory ko!");
	if(iError)
		AfxMessageBox(L"Error to readback some parameters!");
	else
		AfxMessageBox(L"APISamplesReceptionBeam successful!");
}
void APISamplesGate(CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	double dStartWr[my_iGateCycleCountMax][g_iOEMPAGateCountMax],dStopWr[my_iGateCycleCountMax][g_iOEMPAGateCountMax];
	double dStartReq[my_iGateCycleCountMax][g_iOEMPAGateCountMax],dStopReq[my_iGateCycleCountMax][g_iOEMPAGateCountMax];
	bool bEnableGateWr[my_iGateCycleCountMax][g_iOEMPAGateCountMax],bEnableGateReq[my_iGateCycleCountMax][g_iOEMPAGateCountMax];
	enumGateModeAmp eGateModeAmpWr[my_iGateCycleCountMax][g_iOEMPAGateCountMax],eGateModeAmpReq[my_iGateCycleCountMax][g_iOEMPAGateCountMax];
	enumGateModeTof eGateModeTofWr[my_iGateCycleCountMax][g_iOEMPAGateCountMax],eGateModeTofReq[my_iGateCycleCountMax][g_iOEMPAGateCountMax];
	enumRectification eGateRectificationWr[my_iGateCycleCountMax][g_iOEMPAGateCountMax],eGateRectificationReq[my_iGateCycleCountMax][g_iOEMPAGateCountMax];
	double dThresholdPercentWr[my_iGateCycleCountMax][g_iOEMPAGateCountMax],dThresholdPercentReq[my_iGateCycleCountMax][g_iOEMPAGateCountMax];
	int iError,iMode,iCycleCount;

	if(!pHWDeviceOEMPA)
		return;
	iError = 0;
	if(!pHWDeviceOEMPA->LockDevice())
		return;

	//write data
	iCycleCount = my_iGateCycleCountMax;
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		for(int iGateIndex=0;iGateIndex<g_iOEMPAGateCountMax;iGateIndex++)
		{
			bEnableGateWr[iCycleIndex][iGateIndex] = true;
			bEnableGateReq[iCycleIndex][iGateIndex] = false;
			dStartWr[iCycleIndex][iGateIndex] = (iCycleIndex*1.0e-6f+iGateIndex*2.0e-6f);
			dStartReq[iCycleIndex][iGateIndex] = 0.0f;
			dStopWr[iCycleIndex][iGateIndex] = (4.0e-6f+iCycleIndex*1.0e-6f+iGateIndex*2.0e-6f);
			dStopReq[iCycleIndex][iGateIndex] = 0.0f;
			iMode = (iCycleIndex+iGateIndex) & 3;
			eGateModeAmpWr[iCycleIndex][iGateIndex] = (enumGateModeAmp)iMode;
			eGateModeAmpReq[iCycleIndex][iGateIndex] = eAmpPeakToPeak;
			iMode = (1+iCycleIndex+iGateIndex) & 3;
			eGateModeTofWr[iCycleIndex][iGateIndex] = (enumGateModeTof)iMode;
			eGateModeTofReq[iCycleIndex][iGateIndex] = eTofZeroFirstAfterThresholdCross;
			iMode = (2+iCycleIndex+iGateIndex) & 3;
			eGateRectificationWr[iCycleIndex][iGateIndex] = (enumRectification)iMode;
			eGateRectificationReq[iCycleIndex][iGateIndex] = (enumRectification)0;
			dThresholdPercentWr[iCycleIndex][iGateIndex] = iCycleIndex*40.0/iCycleCount+iGateIndex*40.0/g_iOEMPAGateCountMax;

			if(!pHWDeviceOEMPA->SetGateModeThreshold(iCycleIndex,iGateIndex,bEnableGateWr[iCycleIndex][iGateIndex],eGateModeAmpWr[iCycleIndex][iGateIndex],eGateModeTofWr[iCycleIndex][iGateIndex],eGateRectificationWr[iCycleIndex][iGateIndex],dThresholdPercentWr[iCycleIndex][iGateIndex]))
				{iError = 1000;goto return_error;}
			if(!pHWDeviceOEMPA->SetGateStart(iCycleIndex,iGateIndex,dStartWr[iCycleIndex][iGateIndex]))
				{iError = 1000;goto return_error;}
			if(!pHWDeviceOEMPA->SetGateStop(iCycleIndex,iGateIndex,dStopWr[iCycleIndex][iGateIndex]))
				{iError = 1000;goto return_error;}
		}
	}

	//request data
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		for(int iGateIndex=0;iGateIndex<4;iGateIndex++)
		{
			if(!pHWDeviceOEMPA->GetGateModeThreshold(iCycleIndex,iGateIndex,&bEnableGateReq[iCycleIndex][iGateIndex],&eGateModeAmpReq[iCycleIndex][iGateIndex],&eGateModeTofReq[iCycleIndex][iGateIndex],&eGateRectificationReq[iCycleIndex][iGateIndex],&dThresholdPercentReq[iCycleIndex][iGateIndex]))
				{iError = 1000;goto return_error;}
			if(!pHWDeviceOEMPA->GetGateStart(iCycleIndex,iGateIndex,&dStartReq[iCycleIndex][iGateIndex]))
				{iError = 1000;goto return_error;}
			if(!pHWDeviceOEMPA->GetGateStop(iCycleIndex,iGateIndex,&dStopReq[iCycleIndex][iGateIndex]))
				{iError = 1000;goto return_error;}
		}
	}

	//check
return_error:
	pHWDeviceOEMPA->UnlockDevice();
	//check the result
	for(int iCycleIndex=0;iCycleIndex<iCycleCount;iCycleIndex++)
	{
		for(int iGateIndex=0;iGateIndex<4;iGateIndex++)
		{
			if(bEnableGateWr[iCycleIndex][iGateIndex]!=bEnableGateReq[iCycleIndex][iGateIndex])
				iError++;
			if(eGateModeAmpWr[iCycleIndex][iGateIndex]!=eGateModeAmpReq[iCycleIndex][iGateIndex])
				iError++;
			if(eGateModeTofWr[iCycleIndex][iGateIndex]!=eGateModeTofReq[iCycleIndex][iGateIndex])
				iError++;
			if(eGateRectificationWr[iCycleIndex][iGateIndex]!=eGateRectificationReq[iCycleIndex][iGateIndex])
				iError++;
			if(DifferentDouble(dThresholdPercentWr[iCycleIndex][iGateIndex],dThresholdPercentReq[iCycleIndex][iGateIndex],0.1))
				iError++;
			if(DifferentDouble(dStartWr[iCycleIndex][iGateIndex],dStartReq[iCycleIndex][iGateIndex],pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod()/10.0))
				iError++;
			if(DifferentDouble(dStopWr[iCycleIndex][iGateIndex],dStopReq[iCycleIndex][iGateIndex],pHWDeviceOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod()/10.0))
				iError++;
		}
	}
	if(iError)
		AfxMessageBox(L"Error to readback some parameters!");
	else
		AfxMessageBox(L"APISamplesReceptionBeam successful!");
}
