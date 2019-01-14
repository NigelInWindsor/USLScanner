
#include "stdafx.h"

bool g_bMultiHWChannelFWAscanRecoveryTime=false;
double MultiHWChannelFWAscanRecoveryTime()
{
	wchar_t data[MAX_PATH];
	wchar_t pFileName[MAX_PATH];
	double dData;
	int iData;

	wcscpy_s(pFileName,MAX_PATH,UTKernel_GetFolderDataCfg());
	wcscat_s(pFileName,MAX_PATH,L"CfgKernel.txt");
	wcscpy_s(pFileName,MAX_PATH,UTKernel_GetConfigurationFile());

	::GetPrivateProfileString(L"CustomizedAPI",L"FWAscanRecoveryTimeEnable",L"asdf1234",data,MAX_PATH,pFileName);
	if(!wcscmp(L"asdf1234",data))
	{
		WritePrivateProfileString(L"CustomizedAPI",L"FWAscanRecoveryTimeEnable",L"0",pFileName);
		iData = 0;
	}else{
		if(!swscanf_s(data,L"%d",&iData,sizeof(iData)))
			iData = 0;
	}
	if(!iData)
		g_bMultiHWChannelFWAscanRecoveryTime = false;
	else
		g_bMultiHWChannelFWAscanRecoveryTime = true;
	::GetPrivateProfileString(L"CustomizedAPI",L"FWAscanRecoveryTime",L"asdf1234",data,MAX_PATH,pFileName);
	if(!wcscmp(L"asdf1234",data))
	{
		WritePrivateProfileString(L"CustomizedAPI",L"FWAscanRecoveryTime",L"12.5",pFileName);
		return 12.5e-6;
	}else{
		if(!swscanf_s(data,L"%lf",&dData,sizeof(dData)))
			return 12.5e-6;
		dData *= 1.0e-6;
		return dData;
	}
}

bool g_bReferenceTimeOfFlightCorrection=true;

void GetCfgKernelReferenceTimeOfFlight(enumReferenceTimeOfFlight *pRef)
{
	wchar_t data[MAX_PATH/8];
	wchar_t pFileName[MAX_PATH];
	int iData;
	enumReferenceTimeOfFlight eReferenceTimeOfFlight;

	wcscpy_s(pFileName,MAX_PATH,UTKernel_GetConfigurationFile());
	::GetPrivateProfileString(L"CustomizedAPI",L"ReferenceTimeOfFlightCorrection",L"asdf1234",data,MAX_PATH,pFileName);
	if(!wcscmp(L"asdf1234",data))
	{
		g_bReferenceTimeOfFlightCorrection = true;
		WritePrivateProfileString(L"CustomizedAPI", L"ReferenceTimeOfFlightCorrection", L"1", pFileName);
	}else{
		if (swscanf_s(data, L"%d", &iData, sizeof(iData)) == 1)
		{
			if(!iData)
				g_bReferenceTimeOfFlightCorrection = false;
			else
				g_bReferenceTimeOfFlightCorrection = true;
		}
	}

	::GetPrivateProfileString(L"CustomizedAPI",L"ReferenceTimeOfFlightDefault",L"asdf1234",data,MAX_PATH/8,pFileName);
	if(!wcscmp(L"Interface",data))
		eReferenceTimeOfFlight = eInterface;
	else
	if(!wcscmp(L"Pulse",data))
		eReferenceTimeOfFlight = ePulse;
	else{
		eReferenceTimeOfFlight = ePulse;
		WritePrivateProfileString(L"CustomizedAPI",L"ReferenceTimeOfFlightDefault",L"Pulse",pFileName);
	}
	if(pRef)
		*pRef = eReferenceTimeOfFlight;
}

_CUST_DRV_DEF_ structRoot::structRoot()
{
	dwVersionLSB = OEMPA_GetVersionMinor();
	dwVersionMSB = OEMPA_GetVersionMajor();
	dwRootSize = sizeof(structRoot);
	ullSavedParameters = OEMPA_CYCLE_POINTCOUNT_AND_COMPRESSION;
	uiSavedFilterCount = 0;
	eDefaultHwLink = eUnlinked;
	GetSavedParameters(ullSavedParameters,uiSavedFilterCount);
	bEnableFMC = false;
	bEnableMultiHWChannel = false;
	bEnablePulserDuringReplay = false;
	bAscanEnable = true;
	bEnableCscanTof = true;
	eEnableTFM = eTFMOff;
	eAscanBitSize = e8Bits;
	eAscanRequest = eAscanAll;
	dAscanRequestFrequency = 5.0;
	eTriggerMode = eOEMPAInternal;
eEncoderDirection = eOEMPAUpDown;
cTemperatureWarning = 0;
cTemperatureAlarm = 0;
	dTriggerEncoderStep = 1.0e-3;
	eRequestIO = eOEMPANotRequired;
	iRequestIODigitalInputMaskRising = 0;
	iRequestIODigitalInputMaskFalling = 0;
	dDebouncerEncoder = 50.0e-6;
	dDebouncerDigital = 50.0e-6;
	for(int iIndex=0;iIndex<g_iOEMPAMappingDigitalOutputMax;iIndex++)
		eDigitalOuput[iIndex] = eOEMPALow;
	lSWEncoderResolution1 = 10;
	lSWEncoderResolution2 = 10;
	dwSWEncoderDivider1 = 1;
	dwSWEncoderDivider2 = 1;
	eEncoder1A = eDigitalInputOff;
	eEncoder1B = eDigitalInputOff;
	eEncoder2A = eDigitalInputOff;
	eEncoder2B = eDigitalInputOff;
	eExternalTriggerCycle = eDigitalInputOff;
	eExternalTriggerSequence = eDigitalInputOff;
	eEncoder1Type = eEncoderQuadrature;
	eEncoder2Type = eEncoderQuadrature;
	eKeepAlive = eKeepAliveHardwareAndComputer;
	for(int iFilterIndex=0;iFilterIndex<(int)uiSavedFilterCount;iFilterIndex++)
		OEMPA_ResetStructFilter(&aFilter[iFilterIndex]);
	iOEMPAProbeCountMax = g_iOEMPAFocalCountMax;//default value is for a standard OEMPA (for 16:16 ... 64:64 or 32:128 but not for 128:128).
	iOEMPAApertureCountMax = g_iOEMPAApertureElementCountMax;//default value is for a standard OEMPA (for 16:16 ... 64:64 or 32:128 but not for 128:128).
	iOEMPADDFCountMax = g_iOEMPAFocalCountMax;
	iFMCElementStart = 0;
	iFMCElementStop = 0;
	iFMCElementStep = 1;
	dSpecimenVelocity = 6300.0;
	dSpecimenRadius = 0.0;
	dSpecimenThickness = 0.0;
	dDigitizingPeriod = 10.0e-9;
	bUSB3Disable = 0;
	dMultiHWChannelRangeMax = 8192*2*10.0e-9;//The maximum HW memory size for OEM-MC channel is 8192. 10.0e-9 is the period of the digitizing frequency (100 MHz).
	dFWAscanRecoveryTime = 12.5e-6;//7.0 us + 5.0 us.
	dTriggerHighTime = 0.0;

	iSubSequenceEncoderCount = 0;
	iSubSequenceGateCount = 0;
	memset(aiSubSequenceCycleStart,0,g_iSubSequenceTableSize*sizeof(int));
	memset(aiSubSequenceCycleStop,0,g_iSubSequenceTableSize*sizeof(int));
	memset(afSubSequenceValue,0,g_iSubSequenceTableSize*sizeof(int));
	iSubSequenceAverage = 0;

	GetCfgKernelReferenceTimeOfFlight(&eReferenceTimeOfFlight);

	b256Master = false;
	b256Slave = false;

	pHWDeviceOEMPA = NULL;
	pLockUnlock = NULL;
	wcFileName[0] = 0;
	rParamList = NULL;//forbidden to set "new CRead_Write_ParametersList();" bceause of memcpy function to recopy the structure.
}
_CUST_DRV_DEF_ structRoot::~structRoot()
{
}

_CUST_DRV_DEF_ void OEMPA_DesallocCycle(structCycle* &pCycle)
{
	if(pCycle)
		delete[] pCycle;
	pCycle = NULL;
}
_CUST_DRV_DEF_ structCycle* OEMPA_AllocCycle(int iCycleCount)
{
	structCycle* pCycle;

	if(iCycleCount<=0)
		return NULL;
	pCycle = new structCycle[iCycleCount];
	if(!pCycle)
		return pCycle;
	memset(pCycle,0,sizeof(structCycle)*iCycleCount);
	for(int iCycle=0;iCycle<iCycleCount;iCycle++)
	{
		pCycle[iCycle].iFilterIndex = 1;
		//1.1.5.2n
		pCycle[iCycle].wAcqIdChannelProbe = 0xffff;
		pCycle[iCycle].wAcqIdChannelScan = 0xffff;
		pCycle[iCycle].wAcqIdChannelCycle = 0xffff;
		for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
		{
			pCycle[iCycle].gate[iGate].wAcqIDAmp = 0xffff;
			pCycle[iCycle].gate[iGate].wAcqIDTof = 0xffff;
		}
	}
	return pCycle;
}
_CUST_DRV_DEF_ bool WINAPI OEMPA_Free(structCycle* &pCycle,CFocalLaw* &pEmission,CFocalLaw* &pReception)
{
	if(!pCycle || !pEmission || !pReception)
		return false;
	if(pCycle)
		delete [] pCycle;
	pCycle = NULL;
	if(pEmission)
		delete [] pEmission;
	pEmission = NULL;
	if(pReception)
		delete [] pReception;
	pReception = NULL;
	return true;
}
_CUST_DRV_DEF_ void WINAPI OEMPA_Free(structCorrectionOEMPA &correction);
_CUST_DRV_DEF_ bool WINAPI OEMPA_New(structCorrectionOEMPA &correction)
{
	memset(&correction,0,sizeof(correction));
	correction.iAllocCount = g_iOEMPACycleCountMax;
	correction.iCount = 0;
	correction.pdAnglePulse = new double[g_iOEMPACycleCountMax];
	correction.pdSkewPulse = new double[g_iOEMPACycleCountMax];
	correction.pdXPulse = new double[g_iOEMPACycleCountMax];
	correction.pdYPulse = new double[g_iOEMPACycleCountMax];
	correction.pdZPulse = new double[g_iOEMPACycleCountMax];
	correction.pdXFocalPulse = new double[g_iOEMPACycleCountMax];
	correction.pdYFocalPulse = new double[g_iOEMPACycleCountMax];
	correction.pdZFocalPulse = new double[g_iOEMPACycleCountMax];
	correction.pdAngleRecv = new double[g_iOEMPACycleCountMax];
	correction.pdSkewRecv = new double[g_iOEMPACycleCountMax];
	correction.pdXRecv = new double[g_iOEMPACycleCountMax];
	correction.pdYRecv = new double[g_iOEMPACycleCountMax];
	correction.pdZRecv = new double[g_iOEMPACycleCountMax];
	correction.pdXFocalRecv = new double[g_iOEMPACycleCountMax];
	correction.pdYFocalRecv = new double[g_iOEMPACycleCountMax];
	correction.pdZFocalRecv = new double[g_iOEMPACycleCountMax];
	correction.piCountRecv = new int[g_iOEMPACycleCountMax];
	if(correction.pdAnglePulse)
		memset(correction.pdAnglePulse,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdSkewPulse)
		memset(correction.pdSkewPulse,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdXPulse)
		memset(correction.pdXPulse,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdYPulse)
		memset(correction.pdYPulse,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdZPulse)
		memset(correction.pdZPulse,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdXFocalPulse)
		memset(correction.pdXFocalPulse,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdYFocalPulse)
		memset(correction.pdYFocalPulse,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdZFocalPulse)
		memset(correction.pdZFocalPulse,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdAngleRecv)
		memset(correction.pdAngleRecv,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdSkewRecv)
		memset(correction.pdSkewRecv,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdXRecv)
		memset(correction.pdXRecv,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdYRecv)
		memset(correction.pdYRecv,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdZRecv)
		memset(correction.pdZRecv,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdXFocalRecv)
		memset(correction.pdXFocalRecv,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdYFocalRecv)
		memset(correction.pdYFocalRecv,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.pdZFocalRecv)
		memset(correction.pdZFocalRecv,0,sizeof(double)*g_iOEMPACycleCountMax);
	if(correction.piCountRecv)
		memset(correction.piCountRecv,0,sizeof(int)*g_iOEMPACycleCountMax);
	if(!correction.pdAnglePulse)
		goto return_false;
	if(!correction.pdSkewPulse)
		goto return_false;
	if(!correction.pdXPulse)
		goto return_false;
	if(!correction.pdYPulse)
		goto return_false;
	if(!correction.pdZPulse)
		goto return_false;
	if(!correction.pdXFocalPulse)
		goto return_false;
	if(!correction.pdYFocalPulse)
		goto return_false;
	if(!correction.pdZFocalPulse)
		goto return_false;
	if(!correction.pdAngleRecv)
		goto return_false;
	if(!correction.pdSkewRecv)
		goto return_false;
	if(!correction.pdXRecv)
		goto return_false;
	if(!correction.pdYRecv)
		goto return_false;
	if(!correction.pdZRecv)
		goto return_false;
	if(!correction.pdXFocalRecv)
		goto return_false;
	if(!correction.pdYFocalRecv)
		goto return_false;
	if(!correction.pdZFocalRecv)
		goto return_false;
	if(!correction.piCountRecv)
		goto return_false;
	return true;
return_false:
	OEMPA_Free(correction);
	return false;
}
_CUST_DRV_DEF_ void WINAPI OEMPA_Free(structCorrectionOEMPA &correction)
{
	correction.iAllocCount = 0;
	correction.iCount = 0;
	if(correction.pdAnglePulse)
		delete [] correction.pdAnglePulse;
	correction.pdAnglePulse = 0;
	if(correction.pdXPulse)
		delete [] correction.pdXPulse;
	correction.pdXPulse = 0;
	if(correction.pdYPulse)
		delete [] correction.pdYPulse;
	correction.pdYPulse = 0;
	if(correction.pdZPulse)
		delete [] correction.pdZPulse;
	correction.pdZPulse = 0;
	if(correction.pdXFocalPulse)
		delete [] correction.pdXFocalPulse;
	correction.pdXFocalPulse = 0;
	if(correction.pdYFocalPulse)
		delete [] correction.pdYFocalPulse;
	correction.pdYFocalPulse = 0;
	if(correction.pdZFocalPulse)
		delete [] correction.pdZFocalPulse;
	correction.pdZFocalPulse = 0;
	if(correction.pdAngleRecv)
		delete [] correction.pdAngleRecv;
	correction.pdAngleRecv = 0;
	if(correction.pdXRecv)
		delete [] correction.pdXRecv;
	correction.pdXRecv = 0;
	if(correction.pdYRecv)
		delete [] correction.pdYRecv;
	correction.pdYRecv = 0;
	if(correction.pdZRecv)
		delete [] correction.pdZRecv;
	correction.pdZRecv = 0;
	if(correction.pdXFocalRecv)
		delete [] correction.pdXFocalRecv;
	correction.pdXFocalRecv = 0;
	if(correction.pdYFocalRecv)
		delete [] correction.pdYFocalRecv;
	correction.pdYFocalRecv = 0;
	if(correction.pdZFocalRecv)
		delete [] correction.pdZFocalRecv;
	correction.pdZFocalRecv = 0;
	if(correction.piCountRecv)
		delete [] correction.piCountRecv;
	correction.piCountRecv = 0;
}

///////////////////////////////////////////////////////////////////////////////
_CUST_DRV_DEF_ void OEMPA_InitRoot(structRoot &root,CHWDeviceOEMPA *pHWDeviceOEMPA)
{
	if(!pHWDeviceOEMPA)
		return;
	root.iOEMPAProbeCountMax = pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetElementCountMax();
	root.iOEMPAApertureCountMax = pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetApertureCountMax();
	root.dMultiHWChannelRangeMax = pHWDeviceOEMPA->GetMultiHWChannelRangeMax();
	root.dFWAscanRecoveryTime = pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFWAscanRecoveryTime();//7.0 us + 5.0 us.
	root.dTriggerHighTime = pHWDeviceOEMPA->GetSignalTriggerHighTimeMin();
	root.pHWDeviceOEMPA = pHWDeviceOEMPA;
	root.iFMCElementStart = 0;
	root.iFMCElementStop = root.iOEMPAProbeCountMax-1;
	root.iFMCElementStep = 1;
	if(pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelSupported())
	{
		root.bEnableMultiHWChannel = pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsOEMMCEnabled();
		root.bEnableFMC = false;
	}
	if(!pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFlashUSB3Version())
		root.bUSB3Disable = 2;
	else
		root.bUSB3Disable = 0;//default value USB3 disabled!
}
_CUST_DRV_DEF_ void GetSavedParameters(ULONGLONG &ullSavedParameters,UINT &uiSavedFilterCount)
{
	LARGE_INTEGER liConfigurationParameters;
	
	liConfigurationParameters.QuadPart = ullSavedParameters;
	liConfigurationParameters.LowPart = GetPrivateProfileInt(L"OEMPA",L"SetupFileParametersLow",
														liConfigurationParameters.LowPart,
														UTKernel_GetConfigurationFile());
	liConfigurationParameters.HighPart = GetPrivateProfileInt(L"OEMPA",L"SetupFileParametersHigh",
														liConfigurationParameters.HighPart,
														UTKernel_GetConfigurationFile());
	ullSavedParameters = liConfigurationParameters.QuadPart;

	uiSavedFilterCount = GetPrivateProfileInt(L"OEMPA",L"SetupFileFilterCount",
														uiSavedFilterCount,
														UTKernel_GetConfigurationFile());
}

_CUST_DRV_DEF_ void OEMPA_ResetStructGate(structGate *pGate)
{
	pGate->bEnable = false;
	pGate->dStart = 0.0;
	pGate->dStop = 0.0;
	pGate->dThreshold = 0.0;
	pGate->eRectification = eSigned;
	pGate->eModeAmp = eAmpAbsolute;
	pGate->eModeTof = eTofAmplitudeDetection;
	pGate->wAcqIDAmp = 0;
	pGate->wAcqIDTof = 0;
	pGate->bTrackingStartEnable = false;
	pGate->iTrackingStartIndexCycle = 0;
	pGate->iTrackingStartIndexGate = 0;
	pGate->bTrackingStopEnable = false;
	pGate->iTrackingStopIndexCycle = 0;
	pGate->iTrackingStopIndexGate = 0;
}

_CUST_DRV_DEF_ void OEMPA_ResetStructCycle(structCycle *pCycle)
{
	pCycle->dGainDigital = 0.0;
	pCycle->fBeamCorrection = 0.0f;
	pCycle->dStart = 0.0;
	pCycle->dRange = 0.0;
	pCycle->dTimeSlot = 0.0;
	pCycle->dFMCSubTimeSlotAcq = -1.0;
	pCycle->dFMCSubTimeSlotReplay = -1.0;
	pCycle->lPointCount = 0;
	pCycle->lPointFactor = 0;
	pCycle->eCompressionType = eCompression;
	pCycle->eRectification = eSigned;
	for(int iIndex=0;iIndex<g_iOEMPADACCountMax;iIndex++)
	{
		pCycle->adDACTof[g_iOEMPADACCountMax] = 0.0;
		pCycle->afDACPrm[g_iOEMPADACCountMax] = 0.0f;
	}
	pCycle->iDACCount = 0;
	pCycle->bDACEnable = false;
	pCycle->bDACAutoStop = false;
	pCycle->bDACSlope = true;
	pCycle->bMaximum = true;
	pCycle->bMinimum = false;
	pCycle->bSaturation = false;
	pCycle->wAcqIdChannelProbe = 0xffff;
	pCycle->wAcqIdChannelScan = 0xffff;
	pCycle->wAcqIdChannelCycle = 0xffff;
	pCycle->fGainAnalog = 0.0f;
	pCycle->iFilterIndex = 1;
	pCycle->iGateCount = 0;
	pCycle->bTrackingAscanEnable = false;
	pCycle->iTrackingAscanIndexCycle = 0;
	pCycle->iTrackingAscanIndexGate = 0;
	pCycle->bTrackingDacEnable = false;
	pCycle->iTrackingDacIndexCycle = 0;
	pCycle->iTrackingDacIndexGate = 0;
	for(int iIndex=0;iIndex<g_iOEMPAGateCountMax;iIndex++)
		OEMPA_ResetStructGate(&pCycle->gate[iIndex]);
}

CAperture::CAperture()
{
	m_pdwData = NULL;
	m_iApertureAllocatedSize = 0;
	m_pFocalLaw = NULL;
	m_pApertureTotal = NULL;
}
CAperture::~CAperture()
{
	Desalloc();
}
int CAperture::SizeOf()
{
	return sizeof(CAperture)+sizeof(DWORD)*m_iApertureAllocatedSize;
}
void CAperture::SetFocalLaw(CFocalLaw *pFocalLaw)
{
	m_pFocalLaw = pFocalLaw;
}
void CAperture::Desalloc()
{
	if(m_pdwData && (m_iApertureAllocatedSize>0))
	{
		//if(!AfxIsValidAddress(m_pdwData,m_iApertureAllocatedSize*sizeof(DWORD)))		
		//{
		//	m_iApertureAllocatedSize = m_iApertureAllocatedSize;
		//}
		delete m_pdwData;
	}
	m_pdwData = NULL;
	m_iApertureAllocatedSize = 0;
	if(m_pApertureTotal)
		delete m_pApertureTotal;
	m_pApertureTotal = NULL;
}
void CAperture::Reset()
{
	if(!m_pdwData || (m_iApertureAllocatedSize<=0))
		return;
	memset(m_pdwData,0,sizeof(float)*m_iApertureAllocatedSize);
}
bool CAperture::operator==(const CAperture& p)
{
	if(m_iApertureAllocatedSize!=p.m_iApertureAllocatedSize)
		return false;
	for(int iIndexAp=0;iIndexAp<m_iApertureAllocatedSize;iIndexAp++)
	{
		if(m_pdwData[iIndexAp]!=p.m_pdwData[iIndexAp])
			return false;
	}
	return true;
}
bool CAperture::operator!=(const CAperture& p)
{
	if(*this==p)
		return false;
	return true;
}
CAperture& CAperture::operator=(const CAperture& p)
{
	if(SetAllocatedSize(p.GetAllocatedSize()) && m_pdwData && p.m_pdwData)
	{
		for(int iIndex=0;iIndex<m_iApertureAllocatedSize;iIndex++)
		{
			m_pdwData[iIndex] = p.m_pdwData[iIndex];
		}
	}
	return *this;
}
DWORD& CAperture::operator[](int iIndex)
{
	static DWORD x;

	if(!m_pFocalLaw)
		return x;
	if(!m_iApertureAllocatedSize && !m_pdwData)
	{
		m_pFocalLaw->AllocDefault();
	}
	if((iIndex<0) || (m_iApertureAllocatedSize<=iIndex))
	{
		if(m_pFocalLaw)
		{
			//m_pFocalLaw->Error();
		}
		return x;
	}
	return m_pdwData[iIndex];
}
CAperture::operator PDWORD()
{
	return m_pdwData;
}
int CAperture::GetAllocatedSize() const
{
	return m_iApertureAllocatedSize;
}
bool CAperture::SetAllocatedSize(int iSize)
{
	if(iSize<g_iOEMPAApertureDWordCount)
		iSize = g_iOEMPAApertureDWordCount;//required to manage MUX (16:128 or 32:128)
	if(iSize==m_iApertureAllocatedSize)
		return true;
	if(iSize<0)
		return false;
	if(iSize>10000)
		return false;
	Desalloc();
	m_pdwData = new DWORD[iSize];
	if(!m_pdwData)
		return false;
	memset(m_pdwData,0,iSize*sizeof(DWORD));
	m_iApertureAllocatedSize = iSize;
	return true;
}
bool CAperture::Merge(CAperture *pLsb,CAperture *pMsb)
{
	int iCount=m_iApertureAllocatedSize/2;

	if(!m_iApertureAllocatedSize)
		return false;
	//check allocation
	for(int iIndex=iCount;iIndex<pLsb->m_iApertureAllocatedSize;iIndex++)
	{
		if(pLsb->m_pdwData[iIndex])
			return false;//impossible to merge (too many elements)
	}
	for(int iIndex=iCount;iIndex<pMsb->m_iApertureAllocatedSize;iIndex++)
	{
		if(pMsb->m_pdwData[iIndex])
			return false;//impossible to merge (too many elements)
	}
	for(int iIndex=0;iIndex<iCount;iIndex++)
	{
		m_pdwData[iIndex] = pLsb->m_pdwData[iIndex];
		m_pdwData[iIndex+iCount] = pMsb->m_pdwData[iIndex];
	}
	return true;
}
bool CAperture::Split(CAperture *pLsb,CAperture *pMsb)
{
	int iCount=m_iApertureAllocatedSize/2;

	if(!m_iApertureAllocatedSize)
		return false;
	//allocation check
	if(!pLsb || (pLsb->m_iApertureAllocatedSize<iCount))
		return false;
	if(!pMsb || (pMsb->m_iApertureAllocatedSize<iCount))
		return false;
	for(int iIndex=0;iIndex<iCount;iIndex++)
	{
		pLsb->m_pdwData[iIndex] = m_pdwData[iIndex];
		pMsb->m_pdwData[iIndex] = m_pdwData[iIndex+iCount];
		pLsb->m_pdwData[iIndex+iCount] = 0;
		pMsb->m_pdwData[iIndex+iCount] = 0;
	}
	pLsb->AllocTotal(*this);
	return true;
}
void CAperture::SetFocalLawElementCount()
{
	int iElementCount=0;
	DWORD dwBit;

	if(!m_pFocalLaw)
		return;
	for(int iIndex=0;iIndex<m_iApertureAllocatedSize;iIndex++)
	{
		dwBit = 1;
		for(int iIndex=0;iIndex<32;iIndex++)
		{
			if(m_pdwData[iIndex] & dwBit)
				iElementCount++;
			dwBit <<= 1;
		}
	}
	m_pFocalLaw->iElementCount = iElementCount;
}
int CAperture::GetApertureElementCount()
{
	int iElementCount;
	DWORD dwEnable;

	iElementCount = 0;
	for(int iIndexAp=0;iIndexAp<m_iApertureAllocatedSize;iIndexAp++)
	{
		dwEnable = 1;
		if(m_pdwData[iIndexAp])
		for(int iIndex=0;iIndex<32;iIndex++)
		{
			if(dwEnable & m_pdwData[iIndexAp])
				iElementCount++;
			dwEnable = dwEnable<<1;
		}
	}
	return iElementCount;
}
int CAperture::GetFirstElementIndex()
{
	int iElementCount;
	DWORD dwEnable;

	iElementCount = 0;
	for(int iIndexAp=0;iIndexAp<m_iApertureAllocatedSize;iIndexAp++)
	{
		dwEnable = 1;
		if(m_pdwData[iIndexAp])
		for(int iIndex=0;iIndex<32;iIndex++)
		{
			if(dwEnable & m_pdwData[iIndexAp])
			{
				iElementCount++;
				return iIndex+iIndexAp*32;
			}
			dwEnable = dwEnable<<1;
		}
	}
	return -1;
}
int CAperture::GetLastElementIndex()
{
	int iElementCount,iLast;
	DWORD dwEnable;

	iLast = -1;
	iElementCount = 0;
	for(int iIndexAp=0;iIndexAp<m_iApertureAllocatedSize;iIndexAp++)
	{
		dwEnable = 1;
		if(m_pdwData[iIndexAp])
		for(int iIndex=0;iIndex<32;iIndex++)
		{
			if(dwEnable & m_pdwData[iIndexAp])
			{
				iElementCount++;
				iLast = iIndex+iIndexAp*32;
			}
			dwEnable = dwEnable<<1;
		}
	}
	if(iLast>=0)
		return iLast;
	return -1;
}
bool CAperture::GetMultiplexerBoardIndex(int iModulo/*16 or 32*/,int iBufferSize,int *aiMuxBoardIndex,int &iElementCount)
{
	DWORD dwEnable;
	int iApertureAllocatedSize=m_iApertureAllocatedSize;
	WORD *pwData=(WORD*)m_pdwData;//1.1.5.2v
	bool bContinue;//1.1.5.2v

	if((iModulo!=16) && (iModulo!=32))
		return false;
	if(iModulo==16)
		iApertureAllocatedSize *= 2;
	for(int iIndex=0;iIndex<iBufferSize;iIndex++)
		aiMuxBoardIndex[iIndex] = -1;
	iElementCount = 0;
	for(int iIndexAp=0;iIndexAp<iApertureAllocatedSize;iIndexAp++)
	{
		dwEnable = 1;
		bContinue = false;
		if((iModulo==16) && pwData[iIndexAp])
			bContinue = true;
		if((iModulo==32) && m_pdwData[iIndexAp])
			bContinue = true;
		if(bContinue)
		for(int iIndex=0;iIndex<iModulo;iIndex++)
		{
			bContinue = false;
			if((iModulo==16) && (dwEnable & pwData[iIndexAp]))
				bContinue = true;
			if((iModulo==32) && (dwEnable & m_pdwData[iIndexAp]))
				bContinue = true;
			if(bContinue)
			{
				if(iIndex<iBufferSize)
				{
					if(aiMuxBoardIndex[iIndex]<0)
						aiMuxBoardIndex[iIndex] = iIndexAp;
					else
						return false;//index has been already connected.
				}else
					return false;
				iElementCount++;
			}
			dwEnable = dwEnable<<1;
		}
	}
	return true;
}
bool CAperture::GetListElement(int iBufferSize,int *aiBuffer,int &iElementCount)
{
	DWORD dwEnable;
	int iApertureAllocatedSize=m_iApertureAllocatedSize,iElementIndex;

	iElementCount = 0;
	iElementIndex = 0;
	for(int iIndexAp=0;iIndexAp<iApertureAllocatedSize;iIndexAp++)
	{
		dwEnable = 1;
		if(!m_pdwData[iIndexAp])
		{
			iElementIndex += 32;
			continue;
		}
		for(int iIndex=0;iIndex<32;iIndex++)
		{
			if(dwEnable & m_pdwData[iIndexAp])
			{
				if(iElementCount<iBufferSize)
					aiBuffer[iElementCount] = iElementIndex;
				else
					return false;
				iElementCount++;
			}
			dwEnable = dwEnable<<1;
			iElementIndex++;
		}
	}
	return true;
}
bool CAperture::SetAllElementEnable(bool bEnable)
{
	if(!m_pdwData)
		return false;
	if(bEnable)
	for(int iIndexAp=0;iIndexAp<m_iApertureAllocatedSize;iIndexAp++)
		m_pdwData[iIndexAp] = 0xffffffff;
	else
	for(int iIndexAp=0;iIndexAp<m_iApertureAllocatedSize;iIndexAp++)
		m_pdwData[iIndexAp] = 0;
	return true;
}
bool CAperture::SetElementEnable(int iElement,bool bEnable)
{
	int iBit=iElement%32;
	int iData=iElement/32;
	DWORD value;

	if(!m_pdwData)
		return false;
	if(iBit<0)
		return false;
	if(iBit>=32)
		return false;
	if(iData<0)
		return false;
	if(iData>=m_iApertureAllocatedSize)
		return false;
	value = 1<<iBit;
	if(bEnable)
		m_pdwData[iData] |= value;
	else{
		value = ~value;
		m_pdwData[iData] &= value;
	}
	return true;
}
bool CAperture::GetAperture256(bool bMaster,DWORD *padwAperture)
{
	int iOffset;

	if(m_iApertureAllocatedSize<g_iOEMPAApertureDWordCount)
		return false;
	if(bMaster)
		iOffset = 0;
	else
		iOffset = g_iOEMPAApertureDWordCount;
	for(int iIndex=0;iIndex<g_iOEMPAApertureDWordCount;iIndex++)
		padwAperture[iIndex+iOffset] = m_pdwData[iIndex];
	return true;
}
bool CAperture::Write(myCFile &file)
{
	DWORD dwData;

	dwData = sizeof(CAperture);
	file.Write(&dwData,sizeof(DWORD));
	dwData = m_iApertureAllocatedSize;
	file.Write(&dwData,sizeof(DWORD));
	file.Write(m_pdwData,m_iApertureAllocatedSize*sizeof(DWORD));
	return true;
}
bool CAperture::Read(myCFile &file)
{
	DWORD dwData;
	int iDWORDElementCount;

	file.Read(&dwData,sizeof(DWORD));
	if(dwData != sizeof(CFocalLaw))
		return false;
	file.Read(&dwData,sizeof(DWORD));
	iDWORDElementCount = (int)dwData;
	if(iDWORDElementCount!=m_iApertureAllocatedSize)
		return false;
	if(file.Read(m_pdwData,m_iApertureAllocatedSize*sizeof(DWORD))!=m_iApertureAllocatedSize*sizeof(DWORD))
		return false;
	return true;
}
void CAperture::AllocTotal(const CAperture &a)
{
	if(m_pApertureTotal)
		delete m_pApertureTotal;
	m_pApertureTotal = NULL;
	m_pApertureTotal = new CAperture();
	*m_pApertureTotal = a;
}
CAperture* CAperture::GetTotal()
{
	return m_pApertureTotal;
}

CDDFTimeOfFlight::CDDFTimeOfFlight()
{
	m_pdData = NULL;
	m_iTofAllocatedSize = 0;
	m_pFocalLaw = NULL;
}
CDDFTimeOfFlight::~CDDFTimeOfFlight()
{
	Desalloc();
}
int CDDFTimeOfFlight::SizeOf()
{
	return sizeof(CDDFTimeOfFlight)+sizeof(double)*m_iTofAllocatedSize;
}
void CDDFTimeOfFlight::SetFocalLaw(CFocalLaw *pFocalLaw)
{
	m_pFocalLaw = pFocalLaw;
}
void CDDFTimeOfFlight::Desalloc()
{
	if(m_pdData && (m_iTofAllocatedSize>0))
		delete m_pdData;
	m_pdData = NULL;
	m_iTofAllocatedSize = 0;
}
void CDDFTimeOfFlight::Reset()
{
	if(!m_pdData || (m_iTofAllocatedSize<=0))
		return;
	memset(m_pdData,0,sizeof(float)*m_iTofAllocatedSize);
}
CDDFTimeOfFlight& CDDFTimeOfFlight::operator=(const CDDFTimeOfFlight& p)
{
	if(SetAllocatedSize(p.GetAllocatedSize()))
	{
		for(int iIndex=0;iIndex<m_iTofAllocatedSize;iIndex++)
		{
			m_pdData[iIndex] = p.m_pdData[iIndex];
		}
	}
	return *this;
}
double& CDDFTimeOfFlight::operator[](int iIndex)
{
	static double x;

	if(!m_pFocalLaw)
		return x;
	if(!m_iTofAllocatedSize && !m_pdData)
	{
		m_pFocalLaw->AllocDefault();
	}
	if((iIndex<0) || (m_iTofAllocatedSize<=iIndex))
	{
		if(m_pFocalLaw)
		{
			//m_pFocalLaw->Error();
		}
		return x;
	}
	return m_pdData[iIndex];
}
CDDFTimeOfFlight::operator double*()
{
	return m_pdData;
}
int CDDFTimeOfFlight::GetAllocatedSize() const
{
	return m_iTofAllocatedSize;
}
bool CDDFTimeOfFlight::SetAllocatedSize(int iSize)
{
	if(iSize==m_iTofAllocatedSize)
		return true;
	if(iSize<0)
		return false;
	if(iSize>10000)
		return false;
	Desalloc();
	m_pdData = new double[iSize];
	if(!m_pdData)
		return false;
	m_iTofAllocatedSize = iSize;
	return true;
}
bool CDDFTimeOfFlight::IsEqual(CDDFTimeOfFlight *p)
{
	if(m_iTofAllocatedSize!=p->m_iTofAllocatedSize)
		return false;
	for(int iIndex=0;iIndex<m_iTofAllocatedSize;iIndex++)
	{
		if(m_pdData[iIndex]!=p->m_pdData[iIndex])
			return false;
	}
	return true;
}
bool CDDFTimeOfFlight::Write(myCFile &file)
{
	DWORD dwData;

	dwData = sizeof(CDDFTimeOfFlight);
	file.Write(&dwData,sizeof(DWORD));
	dwData = m_iTofAllocatedSize;
	file.Write(&dwData,sizeof(DWORD));
	file.Write(m_pdData,m_iTofAllocatedSize*sizeof(double));
	return true;
}
bool CDDFTimeOfFlight::Read(myCFile &file)
{
	DWORD dwData;
	int iDDFCount;

	file.Read(&dwData,sizeof(DWORD));
	if(dwData != sizeof(CDDFTimeOfFlight))
		return false;
	file.Read(&dwData,sizeof(DWORD));
	iDDFCount = (int)dwData;
	if(iDDFCount!=m_iTofAllocatedSize)
		return false;
	if(file.Read(m_pdData,m_iTofAllocatedSize*sizeof(double))!=m_iTofAllocatedSize*sizeof(double))
		return false;
	return true;
}

CApertureParameter::CApertureParameter()
{
	m_pfData = NULL;
	m_iParameterAllocatedSize = 0;
	m_pFocalLaw = NULL;
}
CApertureParameter::~CApertureParameter()
{
	Desalloc();
}
int CApertureParameter::SizeOf()
{
	return sizeof(CApertureParameter)+sizeof(float)*m_iParameterAllocatedSize;
}
void CApertureParameter::SetFocalLaw(CFocalLaw *pFocalLaw)
{
	m_pFocalLaw = pFocalLaw;
}
CFocalLaw *CApertureParameter::GetFocalLaw()
{
	return m_pFocalLaw;
}
void CApertureParameter::Desalloc()
{
	if(m_pfData && (m_iParameterAllocatedSize>0))
		delete m_pfData;
	m_pfData = NULL;
	m_iParameterAllocatedSize = 0;
}
void CApertureParameter::Reset()
{
	if(!m_pfData || (m_iParameterAllocatedSize<=0))
		return;
	memset(m_pfData,0,sizeof(float)*m_iParameterAllocatedSize);
}
CApertureParameter& CApertureParameter::operator=(const CApertureParameter& p)
{
	if(SetAllocatedSize(p.GetAllocatedSize()))
	{
		for(int iIndex=0;iIndex<m_iParameterAllocatedSize;iIndex++)
		{
			m_pfData[iIndex] = p.m_pfData[iIndex];
		}
	}
	return *this;
}
float& CApertureParameter::operator[](int iIndex)
{
	static float x;

	if(!m_pFocalLaw)
		return x;
	if(!m_iParameterAllocatedSize && !m_pfData)
	{
		m_pFocalLaw->AllocDefault();
	}
	if((iIndex<0) || (m_iParameterAllocatedSize<=iIndex))
	{
		if(m_pFocalLaw)
		{
			//m_pFocalLaw->Error();
		}
		return x;
	}
	return m_pfData[iIndex];
}
CApertureParameter::operator float*()
{
	return m_pfData;
}
bool CApertureParameter::Merge(CApertureParameter *pLsb,CApertureParameter *pMsb)
{
	int iCount=m_iParameterAllocatedSize/2;

	if(!m_iParameterAllocatedSize)
		return false;
	//allocation check
	if(!pLsb || (pLsb->m_iParameterAllocatedSize<iCount))
		return false;
	if(!pMsb || (pMsb->m_iParameterAllocatedSize<iCount))
		return false;
	//element check
	iCount = pLsb->m_pFocalLaw->iPrmCount+pMsb->m_pFocalLaw->iPrmCount;
	if(iCount!=m_pFocalLaw->iPrmCount)
		return false;
	iCount = pLsb->m_pFocalLaw->iPrmCount;
	for(int iIndex=0;iIndex<iCount;iIndex++)
	{
		m_pfData[iIndex] = pLsb->m_pfData[iIndex];
	}
	for(int iIndex=0;iIndex<pMsb->m_pFocalLaw->iPrmCount;iIndex++)
	{
		m_pfData[iIndex+iCount] = pMsb->m_pfData[iIndex];
	}
	return true;
}
bool CApertureParameter::Split(CAperture *pApRoot,CAperture *pApLsb,CAperture *pApMsb,CApertureParameter *pLsb,CApertureParameter *pMsb)
{
	int iCount=m_iParameterAllocatedSize/2;

	if(!m_iParameterAllocatedSize)
		return false;
	//allocation check
	if(!pLsb || (pLsb->m_iParameterAllocatedSize<iCount))
		return false;
	if(!pMsb || (pMsb->m_iParameterAllocatedSize<iCount))
		return false;
	//element check
	if(m_pFocalLaw->iPrmCount!=pLsb->m_pFocalLaw->iPrmCount+pMsb->m_pFocalLaw->iPrmCount)
		return false;
	iCount = pLsb->m_pFocalLaw->iPrmCount;
	for(int iIndex=0;iIndex<iCount;iIndex++)
	{
		pLsb->m_pfData[iIndex] = m_pfData[iIndex];
	}
	for(int iIndex=0;iIndex<pMsb->m_pFocalLaw->iPrmCount;iIndex++)
	{
		pMsb->m_pfData[iIndex] = m_pfData[iIndex+iCount];
	}
	return true;
}
bool CApertureParameter::RemoveElement(int iElement)
{
	if(!m_iParameterAllocatedSize)
		return false;
	if(m_pFocalLaw->iPrmCount<=0)
		return false;
	for(int iIndex=iElement;iIndex<m_pFocalLaw->iPrmCount-1;iIndex++)
	{
		m_pfData[iIndex] = m_pfData[iIndex+1];
	}
	m_pFocalLaw->iPrmCount--;
	return true;
}
int CApertureParameter::GetAllocatedSize() const
{
	return m_iParameterAllocatedSize;
}
bool CApertureParameter::SetAllocatedSize(int iSize)
{
	if(iSize==m_iParameterAllocatedSize)
		return true;
	if(iSize<0)
		return false;
	if(iSize>10000)
		return false;
	Desalloc();
	m_pfData = new float[iSize];
	if(!m_pfData)
		return false;
	m_iParameterAllocatedSize = iSize;
	return true;
}
bool CApertureParameter::Write(myCFile &file)
{
	DWORD dwData;

	dwData = sizeof(CApertureParameter);
	file.Write(&dwData,sizeof(DWORD));
	dwData = m_iParameterAllocatedSize;
	file.Write(&dwData,sizeof(DWORD));
	file.Write(m_pfData,m_iParameterAllocatedSize*sizeof(float));
	return true;
}
bool CApertureParameter::Read(myCFile &file)
{
	DWORD dwData;
	int iElementCount;

	file.Read(&dwData,sizeof(DWORD));
	if(dwData != sizeof(CApertureParameter))
		return false;
	file.Read(&dwData,sizeof(DWORD));
	iElementCount = (int)dwData;
	if(iElementCount!=m_iParameterAllocatedSize)
		return false;
	if(file.Read(m_pfData,m_iParameterAllocatedSize*sizeof(float))!=m_iParameterAllocatedSize*sizeof(float))
		return false;
	return true;
}

CDelayLaw::CDelayLaw()
{
	m_pfDelayLaw = NULL;
	m_iDelayAllocatedSize = 0;
	m_pFocalLaw = NULL;
}
CDelayLaw::~CDelayLaw()
{
	Desalloc();
}
int CDelayLaw::SizeOf()
{
	return sizeof(CDelayLaw)+sizeof(float)*m_iDelayAllocatedSize;
}
void CDelayLaw::SetFocalLaw(CFocalLaw *pFocalLaw)
{
	m_pFocalLaw = pFocalLaw;
}
void CDelayLaw::Desalloc()
{
	if(m_pfDelayLaw && (m_iDelayAllocatedSize>0))
		delete m_pfDelayLaw;
	m_pfDelayLaw = NULL;
	m_iDelayAllocatedSize = 0;
}
void CDelayLaw::Reset()
{
	if(!m_pfDelayLaw || (m_iDelayAllocatedSize<=0))
		return;
	memset(m_pfDelayLaw,0,sizeof(float)*m_iDelayAllocatedSize);
}
CDelayLaw& CDelayLaw::operator=(const CDelayLaw& p)
{
	if(SetAllocatedSize(p.GetAllocatedSize()))
	{
		for(int iIndex=0;iIndex<m_iDelayAllocatedSize;iIndex++)
		{
			m_pfDelayLaw[iIndex] = p.m_pfDelayLaw[iIndex];
		}
	}
	return *this;
}
float& CDelayLaw::operator[](int iIndex)
{
	static float x;

	if(!m_iDelayAllocatedSize)
		return x;
	if((iIndex<0) || (m_iDelayAllocatedSize<=iIndex))
	{
		if(m_pFocalLaw)
		{
			//m_pFocalLaw->Error();
		}
		return x;
	}
	return m_pfDelayLaw[iIndex];
}
float CDelayLaw::Get(int iIndex) const
{
	static float x;

	if(!m_iDelayAllocatedSize)
		return x;
	if((iIndex<0) || (m_iDelayAllocatedSize<=iIndex))
	{
		if(m_pFocalLaw)
		{
			//m_pFocalLaw->Error();
		}
		return x;
	}
	return m_pfDelayLaw[iIndex];
}
bool CDelayLaw::Merge(CDelayLaw *pLsb,CDelayLaw *pMsb)
{
	int iCount=m_iDelayAllocatedSize;

	if(!m_iDelayAllocatedSize)
		return false;
	//allocation check
	if(!pLsb || (pLsb->m_iDelayAllocatedSize>iCount))
		return false;
	if(!pMsb || (pMsb->m_iDelayAllocatedSize>iCount))
		return false;
	//element check
	iCount = pLsb->m_pFocalLaw->iElementCount+pMsb->m_pFocalLaw->iElementCount;
	if(iCount!=m_pFocalLaw->iElementCount)
		return false;
	iCount = pLsb->m_pFocalLaw->iElementCount;
	for(int iIndex=0;iIndex<iCount;iIndex++)
	{
		m_pfDelayLaw[iIndex] = pLsb->m_pfDelayLaw[iIndex];
	}
	for(int iIndex=0;iIndex<pMsb->m_pFocalLaw->iElementCount;iIndex++)
	{
		m_pfDelayLaw[iIndex+iCount] = pMsb->m_pfDelayLaw[iIndex];
	}
	return true;
}
bool CDelayLaw::Split(CAperture *pApRoot,CAperture *pApLsb,CAperture *pApMsb,CDelayLaw *pLsb,CDelayLaw *pMsb)
{
	int iCount=m_iDelayAllocatedSize/2;

	if(!m_iDelayAllocatedSize)
		return false;
	//allocation check
	if(!pLsb || (pLsb->m_iDelayAllocatedSize<iCount))
		return false;
	if(!pMsb || (pMsb->m_iDelayAllocatedSize<iCount))
		return false;
	//element check
	if(m_pFocalLaw->iElementCount!=pLsb->m_pFocalLaw->iElementCount+pMsb->m_pFocalLaw->iElementCount)
		return false;
	iCount = pLsb->m_pFocalLaw->iElementCount;
	for(int iIndex=0;iIndex<iCount;iIndex++)
	{
		pLsb->m_pfDelayLaw[iIndex] = m_pfDelayLaw[iIndex];
	}
	for(int iIndex=0;iIndex<pMsb->m_pFocalLaw->iElementCount;iIndex++)
	{
		pMsb->m_pfDelayLaw[iIndex] = m_pfDelayLaw[iIndex+iCount];
	}
	return true;
}
bool CDelayLaw::RemoveElement(int iElement)
{
	if(!m_iDelayAllocatedSize)
		return false;
	if(m_pFocalLaw->iDelayCount<=0)
		return false;
	for(int iIndex=iElement;iIndex<m_pFocalLaw->iElementCount-1;iIndex++)
	{
		m_pfDelayLaw[iIndex] = m_pfDelayLaw[iIndex+1];
	}
	return true;
}
bool CDelayLaw::UpdateDelay(int iElement,float fDelay)
{
	if(!m_iDelayAllocatedSize)
		return false;
	if(iElement<0)
		return false;
	if(m_pFocalLaw->iDelayCount<=iElement)
		return false;
	m_pfDelayLaw[iElement] += fDelay;
	return true;
}
bool CDelayLaw::ReorderDelay(int ElementCount,int *aiOrder)
{
	float afDelay[g_iOEMPAApertureElementCountMax*2];
	int iElement;

	for(int iIndex=0;iIndex<ElementCount;iIndex++)
		afDelay[iIndex] = m_pfDelayLaw[iIndex];
	for(int iIndex=0;iIndex<ElementCount;iIndex++)
	{
		iElement = aiOrder[iIndex];
		m_pfDelayLaw[iIndex] = afDelay[iElement];
	}
	return true;
}
int CDelayLaw::GetAllocatedSize() const
{
	return m_iDelayAllocatedSize;
}
bool CDelayLaw::SetAllocatedSize(int iSize)
{
	if(iSize==m_iDelayAllocatedSize)
		return true;
	if(iSize<0)
		return false;
	if(iSize>10000)
		return false;
	Desalloc();
	m_pfDelayLaw = new float[iSize];
	if(!m_pfDelayLaw)
		return false;
	m_iDelayAllocatedSize = iSize;
	return true;
}
bool CDelayLaw::Write(myCFile &file)
{
	DWORD dwData;

	dwData = sizeof(CDelayLaw);
	file.Write(&dwData,sizeof(DWORD));
	dwData = m_iDelayAllocatedSize;
	file.Write(&dwData,sizeof(DWORD));
	file.Write(m_pfDelayLaw,m_iDelayAllocatedSize*sizeof(float));
	return true;
}
bool CDelayLaw::Read(myCFile &file)
{
	DWORD dwData;
	int iElementCount;

	file.Read(&dwData,sizeof(DWORD));
	if(dwData != sizeof(CDelayLaw))
		return false;
	file.Read(&dwData,sizeof(DWORD));
	iElementCount = (int)dwData;
	if(iElementCount!=m_iDelayAllocatedSize)
		return false;
	if(file.Read(m_pfDelayLaw,m_iDelayAllocatedSize*sizeof(DWORD))!=m_iDelayAllocatedSize*sizeof(DWORD))
		return false;
	return true;
}

CDDF::CDDF()
{
	//no default allocation
	m_pData = NULL;
	m_iDDFAllocatedSize = 0;
	m_pFocalLaw = NULL;
}
CDDF::~CDDF()
{
	Desalloc();
}
int CDDF::SizeOf()
{
	int iSizeOf=sizeof(CDDF);
	for(int iIndex=0;iIndex<m_iDDFAllocatedSize;iIndex++)
		iSizeOf += m_pData[iIndex].SizeOf();
	return iSizeOf;
}
void CDDF::SetFocalLaw(CFocalLaw *pFocalLaw)
{
	m_pFocalLaw = pFocalLaw;
}
CFocalLaw *CDDF::GetFocalLaw()
{
	return m_pFocalLaw;
}
void CDDF::Desalloc()
{
	if(m_pData)
	for(int iIndex=0;iIndex<m_iDDFAllocatedSize;iIndex++)
	{
		m_pData[iIndex].Desalloc();
	}
	if(m_pData && (m_iDDFAllocatedSize>0))
		delete[] m_pData;
	m_pData = NULL;
	m_iDDFAllocatedSize = 0;
}
void CDDF::Reset()
{
	for(int iIndex=0;iIndex<m_iDDFAllocatedSize;iIndex++)
	{
		m_pData[iIndex].Reset();
	}
}
CDDF& CDDF::operator=(const CDDF& p)
{
	if(SetAllocatedSize(p.GetAllocatedSize()))
	{
		for(int iIndex=0;iIndex<m_iDDFAllocatedSize;iIndex++)
		{
			m_pData[iIndex] = p.m_pData[iIndex];
		}
	}
	return *this;
}
CDelayLaw& CDDF::operator[](int iIndex)
{
	static CDelayLaw x;

	if(!m_pFocalLaw)
		return x;
	if(!m_iDDFAllocatedSize && !m_pData)
	{
		m_pFocalLaw->AllocDefault();
	}
	if((iIndex<0) || (m_iDDFAllocatedSize<=iIndex))
	{
		if(m_pFocalLaw)
		{
			//m_pFocalLaw->Error();
		}
		return x;
	}
	return m_pData[iIndex];
}
const CDelayLaw& CDDF::Get(int iIndex) const
{
	static CDelayLaw x;

	if(!m_pFocalLaw)
		return x;
	if(!m_iDDFAllocatedSize && !m_pData)
	{
		m_pFocalLaw->AllocDefault();
	}
	if((iIndex<0) || (m_iDDFAllocatedSize<=iIndex))
	{
		if(m_pFocalLaw)
		{
			//m_pFocalLaw->Error();
		}
		return x;
	}
	return m_pData[iIndex];
}
bool CDDF::Merge(CDDF *pLsb,CDDF *pMsb)
{
	int iCount=m_iDDFAllocatedSize;

	if(!m_iDDFAllocatedSize)
		return false;
	//allocation check
	if(!pLsb || (pLsb->m_iDDFAllocatedSize>iCount))
		return false;
	if(!pMsb || (pMsb->m_iDDFAllocatedSize>iCount))
		return false;
	//ddf check
	if(pLsb->m_pFocalLaw->iFocalCount && (pLsb->m_pFocalLaw->iFocalCount!=m_pFocalLaw->iFocalCount))
		return false;
	if(pMsb->m_pFocalLaw->iFocalCount && (pMsb->m_pFocalLaw->iFocalCount!=m_pFocalLaw->iFocalCount))
		return false;
	for(int iIndex=0;iIndex<m_pFocalLaw->iFocalCount;iIndex++)
	{
		if(!m_pData[iIndex].Merge(&pLsb->m_pData[iIndex],&pMsb->m_pData[iIndex]))
			return false;
	}
	return true;
}
bool CDDF::Split(CAperture *pApRoot,CAperture *pApLsb,CAperture *pApMsb,CDDF *pLsb,CDDF *pMsb)
{
	int iCount=m_iDDFAllocatedSize;

	if(!m_iDDFAllocatedSize)
		return false;
	//allocation check
	if(!pLsb || (pLsb->m_iDDFAllocatedSize<iCount))
		return false;
	if(!pMsb || (pMsb->m_iDDFAllocatedSize<iCount))
		return false;
	//ddf check
	if(pLsb->m_pFocalLaw->iElementCount && (pLsb->m_pFocalLaw->iFocalCount != m_pFocalLaw->iFocalCount))
		return false;
	if(pMsb->m_pFocalLaw->iElementCount && (pMsb->m_pFocalLaw->iFocalCount != m_pFocalLaw->iFocalCount))
		return false;
	for(int iIndex=0;iIndex<m_pFocalLaw->iFocalCount;iIndex++)
	{
		if(!m_pData[iIndex].Split(pApRoot,pApLsb,pApMsb,&pLsb->m_pData[iIndex],&pMsb->m_pData[iIndex]))
			return false;
	}
	return true;
}
bool CDDF::RemoveElement(int iElement)
{
	if(!m_iDDFAllocatedSize)
		return false;
	for(int iIndex=0;iIndex<m_pFocalLaw->iFocalCount;iIndex++)
	{
		if(!m_pData[iIndex].RemoveElement(iElement))
			return false;
		m_pFocalLaw->iDelayCount--;
	}
	return true;
}
bool CDDF::UpdateDelay(int iElement,float fDelay)
{
	if(!m_iDDFAllocatedSize)
		return false;
	for(int iIndex=0;iIndex<m_pFocalLaw->iFocalCount;iIndex++)
	{
		if(!m_pData[iIndex].UpdateDelay(iElement,fDelay))
			return false;
	}
	return true;
}
bool CDDF::ReorderDelay(int ElementCount,int *piElement)
{
	int aiOrder[g_iOEMPAApertureElementCountMax*2];
	int aiElement[g_iOEMPAApertureElementCountMax*2];
	int iMin,iFound;
	bool bUpdate=false;

	if(ElementCount>g_iOEMPAApertureElementCountMax*2)
	{
		UTKernel_SystemMessageBoxList(L"Error CDDF::ReorderDelay()");
		return false;
	}
	for(int iIndex=0;iIndex<ElementCount;iIndex++)
	{
		aiElement[iIndex] = piElement[iIndex];
		if((iIndex>0) && (piElement[iIndex]<piElement[iIndex-1]))
			bUpdate = true;
	}
	if(!bUpdate)
		return true;
	for(int iIndex=0;iIndex<ElementCount;iIndex++)
	{
		iMin = 0x7fffffff;
		iFound = -1;
		for(int iIndex2=0;iIndex2<ElementCount;iIndex2++)
		{
			if(iMin>piElement[iIndex2])
			{
				iMin = piElement[iIndex2];
				iFound = iIndex2;
			}
		}
		if(iFound<0)
			return false;
		piElement[iFound] = 0x7fffffff;
		if(iIndex!=iFound)
			bUpdate = true;
		aiOrder[iIndex] = iFound;
	}
	for(int iIndex=0;iIndex<m_pFocalLaw->iFocalCount;iIndex++)
	{
		if(!m_pData[iIndex].ReorderDelay(ElementCount,aiOrder))
			return false;
	}
	return true;
}
void CDDF::GetAllocatedSize(int &iDDF,int &iElement) const
{
	iDDF = m_iDDFAllocatedSize;
	iElement = 0;
	if(m_iDDFAllocatedSize)
	{
		if(m_pFocalLaw)
			iElement = m_pFocalLaw->GetCountElement();
		else
			iElement = m_pData[0].GetAllocatedSize();
	}
}
bool CDDF::SetAllocatedSize(int iDDF,int iElement)
{
	if(!SetAllocatedSize(iDDF))
		return false;
	for(int iIndex=0;iIndex<m_iDDFAllocatedSize;iIndex++)
	{
		if(!m_pData[iIndex].SetAllocatedSize(iElement))
			return false;
		m_pData[iIndex].SetFocalLaw(m_pFocalLaw);
	}
	return true;
}
int CDDF::GetAllocatedSize(int *piElement) const
{
	if(piElement)
	{
		if(!m_iDDFAllocatedSize)
			*piElement = 0;
		else
			*piElement = m_pFocalLaw->GetCountElement();
	}
	return m_iDDFAllocatedSize;
}
bool CDDF::SetAllocatedSize(int iSize)
{
	if(iSize==m_iDDFAllocatedSize)
		return true;
	if(iSize<0)
		return false;
	if(iSize>10000)
		return false;
	Desalloc();
	m_pData = new CDelayLaw[iSize];
	if(!m_pData)
		return false;
	m_iDDFAllocatedSize = iSize;
	return true;
}
bool CDDF::Write(myCFile &file)
{
	DWORD dwData;

	dwData = sizeof(CDDF);
	file.Write(&dwData,sizeof(DWORD));
	dwData = m_iDDFAllocatedSize;
	file.Write(&dwData,sizeof(DWORD));
	for(int iIndex=0;iIndex<m_iDDFAllocatedSize;iIndex++)
	{
		if(!m_pData[iIndex].Write(file))
			return false;
	}
	return true;
}
bool CDDF::Read(myCFile &file)
{
	DWORD dwData;
	int iDDFCount;

	file.Read(&dwData,sizeof(DWORD));
	if(dwData != sizeof(CDDF))
		return false;
	file.Read(&dwData,sizeof(DWORD));
	iDDFCount = (int)dwData;
	if(iDDFCount!=m_iDDFAllocatedSize)
		return false;
	for(int iIndex=0;iIndex<m_iDDFAllocatedSize;iIndex++)
	{
		if(!m_pData[iIndex].Read(file))
			return false;
	}
	return true;
}

_CUST_DRV_DEF_ CFocalLaw* OEMPA_AllocFocalLaw(int iCycleCount)
{
	CFocalLaw* pFocalLaw;

	if (iCycleCount<=0)
		return NULL;
	pFocalLaw = new CFocalLaw[iCycleCount];
	return pFocalLaw;
}
_CUST_DRV_DEF_ void OEMPA_DesallocFocalLaw(CFocalLaw* &pFocalLaw)
{
	if(pFocalLaw)
		delete[] pFocalLaw;
	pFocalLaw = NULL;
}
CFocalLaw::CFocalLaw()
{
	//no default allocation
	m_iLawDDFAllocatedSize = 0;
	m_iLawElementAllocatedElementSize = 0;
	m_iLawApertureAllocatedSize = 0;
	adwElement.SetFocalLaw(this);
	adFocalTimeOfFlight.SetFocalLaw(this);
	afPrm.SetFocalLaw(this);
	afDelay.SetFocalLaw(this);
	aTRLWedgeDelayRef.SetFocalLaw(this);
	aTRLWedgeDelayReal.SetFocalLaw(this);
	for(int iIndex=0;iIndex<g_iOEMPAApertureElementCountMax/(2*sizeof(unsigned long long));iIndex++)
		hwAcqDecimation.ulonglong[iIndex] = 0;
	m_p256MatchedDevice = NULL;
	fCenterDelay = 0.0f;
}
CFocalLaw::CFocalLaw(int &iDDFCount,int &iElementCount)
{
	m_iLawDDFAllocatedSize = 0;
	m_iLawElementAllocatedElementSize = 0;
	m_iLawApertureAllocatedSize = 0;
	adwElement.SetFocalLaw(this);
	adFocalTimeOfFlight.SetFocalLaw(this);
	afPrm.SetFocalLaw(this);
	afDelay.SetFocalLaw(this);
	aTRLWedgeDelayRef.SetFocalLaw(this);
	aTRLWedgeDelayReal.SetFocalLaw(this);
	SetAllocatedSize(iDDFCount,iElementCount);
	for(int iIndex=0;iIndex<g_iOEMPAApertureElementCountMax/(2*sizeof(unsigned long long));iIndex++)
		hwAcqDecimation.ulonglong[iIndex] = 0;
}
CFocalLaw::~CFocalLaw()
{
	Desalloc();
}
int CFocalLaw::SizeOf()
{
	return sizeof(CFocalLaw)+adwElement.SizeOf()+adFocalTimeOfFlight.SizeOf()+afPrm.SizeOf()+afDelay.SizeOf()+aTRLWedgeDelayRef.SizeOf()+aTRLWedgeDelayReal.SizeOf();
}
CFocalLaw& CFocalLaw::operator=(const CFocalLaw& p)
{
	if(SetAllocatedSize(p.GetCountDDF(),p.GetCountElement()))
	{
		dWedgeDelay = p.dWedgeDelay;
		iElementCount = p.iElementCount;
		adwElement = p.adwElement;
		eFocusing = p.eFocusing;
		iDelayCount = p.iDelayCount;
		iFocalCount = p.iFocalCount;
		afDelay = p.afDelay;
		aTRLWedgeDelayRef = p.aTRLWedgeDelayRef;
		aTRLWedgeDelayReal = p.aTRLWedgeDelayReal;
		adFocalTimeOfFlight = p.adFocalTimeOfFlight;
		iPrmCount = p.iPrmCount;
		afPrm = p.afPrm;
		dSkew = p.dSkew;
		dAngle = p.dAngle;
		dX = p.dX;
		dY = p.dY;
		dZ = p.dZ;
		dFocalX = p.dFocalX;
		dFocalY = p.dFocalY;
		dFocalZ = p.dFocalZ;
		for(int iIndex=0;iIndex<g_iOEMPAApertureElementCountMax/(2*sizeof(unsigned long long));iIndex++)
			hwAcqDecimation.ulonglong[iIndex] = p.hwAcqDecimation.ulonglong[iIndex];
	}
	return *this;
}
void CFocalLaw::AllocDefault()
{
	SetAllocatedSize(g_iOEMPAFocalCountMax,g_iOEMPAApertureElementCountMax);
}
void CFocalLaw::Desalloc()
{
	adwElement.Desalloc();
	adFocalTimeOfFlight.Desalloc();
	afPrm.Desalloc();
	afDelay.Desalloc();
	aTRLWedgeDelayRef.Desalloc();
	aTRLWedgeDelayReal.Desalloc();
	m_iLawDDFAllocatedSize = 0;
	m_iLawElementAllocatedElementSize = 0;
	m_iLawApertureAllocatedSize = 0;
}
void CFocalLaw::Reset()
{
	dWedgeDelay = 0.0;
	iElementCount = 0;
	for(int iIndex=0;iIndex<m_iLawApertureAllocatedSize;iIndex++)
		adwElement[iIndex] = 0;
	eFocusing = eStandard;
	iFocalCount = 0;
	iDelayCount = iFocalCount * iElementCount;
	afDelay.Reset();
	aTRLWedgeDelayRef.Reset();
	aTRLWedgeDelayReal.Reset();
	for(int iIndex=0;iIndex<m_iLawDDFAllocatedSize;iIndex++)
		adFocalTimeOfFlight[iIndex] = 0.0;
	iPrmCount = iElementCount;
	for(int iElem=0;iElem<m_iLawElementAllocatedElementSize;iElem++)
		afPrm[iElem] = 0.0f;
	dSkew = 0.0;
	dAngle = 0.0;
	dX = 0.0;
	dY = 0.0;
	dZ = 0.0;
	dFocalX = 0.0;
	dFocalY = 0.0;
	dFocalZ = 0.0;
	for(int iIndex=0;iIndex<g_iOEMPAApertureElementCountMax/(2*sizeof(unsigned long long));iIndex++)
		hwAcqDecimation.ulonglong[iIndex] = 0;
}
bool CFocalLaw::SetAllocatedSize(int iDDFCount,int iElementCount)
{
	int iDWORDElementSize;

	if((iDDFCount==m_iLawDDFAllocatedSize) && (iElementCount==m_iLawElementAllocatedElementSize))
		return true;
	Desalloc();
	if(iDDFCount<0)
		return false;
	if(iElementCount<0)
		return false;
	iDWORDElementSize = iElementCount/32;
	if(iElementCount%32)
		iDWORDElementSize++;
	if(iDWORDElementSize<g_iOEMPAApertureDWordCount)
		iDWORDElementSize = g_iOEMPAApertureDWordCount;//required to manage MUX (16:128 or 32:128)
	adwElement.SetAllocatedSize(iDWORDElementSize);
	if(!(DWORD*)adwElement)
	{
		Desalloc();
		return false;
	}
	adFocalTimeOfFlight.SetAllocatedSize(iDDFCount);
	if(!(double*)adFocalTimeOfFlight)
	{
		Desalloc();
		return false;
	}
	afPrm.SetAllocatedSize(iElementCount);
	if(!(float*)afPrm)
	{
		Desalloc();
		return false;
	}
	if(!afDelay.SetAllocatedSize(iDDFCount,iElementCount))
	{
		Desalloc();
		return false;
	}
	m_iLawDDFAllocatedSize = iDDFCount;
	m_iLawElementAllocatedElementSize = iElementCount;
	m_iLawApertureAllocatedSize = iDWORDElementSize;
	return true;
}
int CFocalLaw::GetCountDDF() const
{
	return m_iLawDDFAllocatedSize;
}
int CFocalLaw::GetCountElement() const
{
	return m_iLawElementAllocatedElementSize;
}
int CFocalLaw::GetDWORDElementSize() const
{
	return m_iLawApertureAllocatedSize;
}
void CFocalLaw::GetMultiChannelDecimation(int &iFirst,int &iCount,int aiDecimation[g_iOEMPAApertureElementCountMax]) const
{
	unionDecimation *pX;
	int iHWChDecimationCount = 0;
	int iHWChDecimationFirst = -1;
	int iHWChDecimationLast = -1;
	for(int iIndex=0;iIndex<g_iOEMPAApertureElementCountMax/2;iIndex++)
	{
		pX = (unionDecimation *)&hwAcqDecimation.byte[iIndex];
		if(aiDecimation)
		{
			aiDecimation[iIndex*2+0] = pX->data.lsb;
			aiDecimation[iIndex*2+1] = pX->data.msb;
		}
		if((iHWChDecimationFirst<0) && (pX->data.lsb>0))
			iHWChDecimationFirst = iIndex*2+0;
		if((iHWChDecimationFirst<0) && (pX->data.msb>0))
			iHWChDecimationFirst = iIndex*2+1;
		if(pX->data.lsb>0)
			iHWChDecimationLast = iIndex*2+0;
		if(pX->data.msb>0)
			iHWChDecimationLast = iIndex*2+1;
	}
	if(iHWChDecimationFirst>=0)
		iHWChDecimationCount = iHWChDecimationLast-iHWChDecimationFirst+1;
	else
		iHWChDecimationFirst = 0;
	iCount = iHWChDecimationCount;
	iFirst = iHWChDecimationFirst;
}
bool CFocalLaw::GetMultiChannel(int &iStartFirst,int &iStartCount) const
{
	int iDecimationFirst,iDecimationLast,iDecimationCount;
	int iDelayFirst,iDelayLast,iDelayCount2;
	int iDDF,iElement,iStartLast;

	GetMultiChannelDecimation(iDecimationFirst,iDecimationCount,NULL);
	iDecimationLast = iDecimationFirst+iDecimationCount-1;
	afDelay.GetAllocatedSize(iDDF,iElement);
	if(iDDF<1)
	{
		iStartFirst = 0;
		iStartCount = iDelayCount;
		return false;
	}
	iDelayFirst = -1;
	iDelayLast = -1;
	iDelayCount2 = 0;
	for(int iIndex=0;iIndex<iDelayCount;iIndex++)
	{
		//OEMMC 1.1.5.2b if(afDelay.Get(0).Get(iIndex)>0.0f)
		if(afDelay.Get(0).Get(iIndex)>=0.0f)
		{
			if(iDelayFirst == -1)
				iDelayFirst = iIndex;
			iDelayLast = iIndex;
		}
	}
	if((iDelayFirst>=0) && (iDelayLast>=0))
		iDelayCount2 = iDelayLast-iDelayFirst+1;
	else
		iDelayFirst = 0;
	if(!iDecimationCount && !iDelayCount2)
	{//bug
		iStartFirst = 0;
		iStartCount = 0;
		return true;
	}
	if(!iDelayCount2)
	{
		iStartFirst = iDecimationFirst;
		iStartCount = iDecimationCount;
		return true;
	}
	if(iDecimationFirst<iDelayFirst)
		iStartFirst = iDecimationFirst;
	else
		iStartFirst = iDelayFirst;
	if(iDecimationLast>iDelayLast)
		iStartLast = iDecimationLast;
	else
		iStartLast = iDelayLast;
	iStartCount = iStartLast-iStartFirst+1;
	return true;
}
bool CFocalLaw::Merge(CFocalLaw *pLsb,CFocalLaw *pMsb)
{
	if(!pLsb || !pMsb)
		return false;
	iElementCount = pLsb->iElementCount+pMsb->iElementCount;
	iDelayCount = pLsb->iDelayCount+pMsb->iDelayCount;
	if(pLsb->iFocalCount<pMsb->iFocalCount)
	{
		iFocalCount = pMsb->iFocalCount;
		adFocalTimeOfFlight = pMsb->adFocalTimeOfFlight;
	}else{
		iFocalCount = pLsb->iFocalCount;
		adFocalTimeOfFlight = pLsb->adFocalTimeOfFlight;
	}
	iPrmCount = pLsb->iPrmCount+pMsb->iPrmCount;
	if(!adwElement.Merge(&pLsb->adwElement,&pMsb->adwElement))
		return false;
	if(!afPrm.Merge(&pLsb->afPrm,&pMsb->afPrm))
		return false;
	if(!afDelay.Merge(&pLsb->afDelay,&pMsb->afDelay))
		return false;
	dWedgeDelay = pLsb->dWedgeDelay;
	eFocusing = pLsb->eFocusing;
	dSkew = pLsb->dSkew;
	dAngle = pLsb->dAngle;
	dX = pLsb->dX;
	dY = pLsb->dY;
	dZ = pLsb->dZ;
	dFocalX = pLsb->dFocalX;
	dFocalY = pLsb->dFocalY;
	dFocalZ = pLsb->dFocalZ;
	return true;
}
bool CFocalLaw::Split(CFocalLaw *pLsb,CFocalLaw *pMsb)
{
	if(!pLsb || !pMsb)
		return false;
	if(iElementCount!=iPrmCount)
		return false;
	pLsb->iElementCount = pMsb->iElementCount = 0;
	pLsb->iDelayCount = pMsb->iDelayCount = 0;
	pLsb->iPrmCount = pMsb->iPrmCount = 0;
	pLsb->iFocalCount = pMsb->iFocalCount = iFocalCount;
	if(!adwElement.Split(&pLsb->adwElement,&pMsb->adwElement))
		return false;
	pLsb->iElementCount = pLsb->adwElement.GetApertureElementCount();
	pMsb->iElementCount = pMsb->adwElement.GetApertureElementCount();
	if(!pLsb->iElementCount)
		pLsb->iFocalCount = 0;
	if(!pMsb->iElementCount)
		pMsb->iFocalCount = 0;
	pLsb->iPrmCount = pLsb->iElementCount;
	pMsb->iPrmCount = pMsb->iElementCount;
	pLsb->iDelayCount = pLsb->iElementCount;
	pMsb->iDelayCount = pMsb->iElementCount;
	for(int iIndex=0;iIndex<iFocalCount;iIndex++)
	{
		pLsb->adFocalTimeOfFlight[iIndex] = adFocalTimeOfFlight[iIndex];
		pMsb->adFocalTimeOfFlight[iIndex] = adFocalTimeOfFlight[iIndex];
	}
	if(!afPrm.Split(&adwElement,&pLsb->adwElement,&pMsb->adwElement,&pLsb->afPrm,&pMsb->afPrm))
		return false;
	if(!afDelay.Split(&adwElement,&pLsb->adwElement,&pMsb->adwElement,&pLsb->afDelay,&pMsb->afDelay))
		return false;
	pLsb->dWedgeDelay = pMsb->dWedgeDelay = dWedgeDelay;
	pLsb->eFocusing = pMsb->eFocusing = eFocusing;
	pLsb->dSkew = pMsb->dSkew = dSkew;
	pLsb->dAngle = pMsb->dAngle = dAngle;
	pLsb->dX = pMsb->dX = dX;
	pLsb->dY = pMsb->dY = dY;
	pLsb->dZ = pMsb->dZ = dZ;
	pLsb->dFocalX = pMsb->dFocalX = dFocalX;
	pLsb->dFocalY = pMsb->dFocalY = dFocalY;
	pLsb->dFocalZ = pMsb->dFocalZ = dFocalZ;
	return true;
}
bool CFocalLaw::RemoveElement(int iElement)
{
	DWORD dwElement,dwElem;
	int iCount;
	int iFound=-1;

	//check that the element is inside the aperture.
	iCount = 0;
	for(int iIndex=0;iIndex<m_iLawApertureAllocatedSize;iIndex++)
	{
		dwElem = 1;
		if((iElement<32*(iIndex+1)) && (iElement>=32*iIndex))
			dwElement = 1<<(iElement-32*iIndex);
		else
			dwElement = 0;
		for(int iIndex2=0;iIndex2<32;iIndex2++)
		{
			if(adwElement[iIndex] & dwElement)
			{
				iFound = iIndex;
				if(dwElem==dwElement)
					break;
			}
			if(adwElement[iIndex] & dwElem)
				iCount++;
			dwElem <<= 1;
		}
		if(iFound!=-1)
			break;
	}
	if(iFound==-1)
		return false;
	if(iElementCount<=0)
		return false;
	if(iDelayCount<=0)
		return false;
	if(iPrmCount<=0)
		return false;
	if(!adwElement.SetElementEnable(iElement,false))
		return false;
	afDelay.RemoveElement(iCount);
	afPrm.RemoveElement(iCount);
	iElementCount--;
	return true;
}
bool CFocalLaw::Write(myCFile &file)
{
	DWORD dwData;
	int iAux;

	dwData = sizeof(CFocalLaw);
	file.Write(&dwData,sizeof(DWORD));
	dwData = m_iLawDDFAllocatedSize;
	file.Write(&dwData,sizeof(DWORD));
	dwData = m_iLawElementAllocatedElementSize;
	file.Write(&dwData,sizeof(DWORD));
	dwData = m_iLawApertureAllocatedSize;
	file.Write(&dwData,sizeof(DWORD));

	file.Write(&dWedgeDelay,sizeof(dWedgeDelay));
	iAux = iElementCount;
	file.Write(&iAux,sizeof(iAux));
	if(!adwElement.Write(file))
		return false;
	file.Write(&eFocusing,sizeof(eFocusing));
	iAux = iDelayCount;
	file.Write(&iAux,sizeof(iAux));
	iAux = iFocalCount;
	file.Write(&iAux,sizeof(iAux));
	if(!afDelay.Write(file))
		return false;
	if(!adFocalTimeOfFlight.Write(file))
		return false;
	iAux = iPrmCount;
	file.Write(&iAux,sizeof(iAux));
	if(!afPrm.Write(file))
		return false;
	file.Write(&dSkew,sizeof(dSkew));
	file.Write(&dAngle,sizeof(dAngle));
	file.Write(&dX,sizeof(dX));
	file.Write(&dY,sizeof(dY));
	file.Write(&dZ,sizeof(dZ));
	file.Write(&dFocalX,sizeof(dFocalX));
	file.Write(&dFocalY,sizeof(dFocalY));
	file.Write(&dFocalZ,sizeof(dFocalZ));
	if(!aTRLWedgeDelayRef.Write(file))
		return false;
	if(!aTRLWedgeDelayReal.Write(file))
		return false;
	return true;
}
bool CFocalLaw::Read(myCFile &file)
{
	DWORD dwData;
	int iCountDDF,iCountElement,iAux;

	file.Read(&dwData,sizeof(DWORD));
	if(dwData != sizeof(CFocalLaw))
		return false;
	file.Read(&dwData,sizeof(DWORD));
	iCountDDF = (int)dwData;
	file.Read(&dwData,sizeof(DWORD));
	dwData = m_iLawElementAllocatedElementSize;
	file.Read(&dwData,sizeof(DWORD));
	iCountElement = (int)dwData;
	if(!SetAllocatedSize(iCountElement,iCountElement))
		return false;

	file.Read(&dWedgeDelay,sizeof(dWedgeDelay));
	file.Read(&iAux,sizeof(iAux));
	iElementCount = iAux;
	if(!adwElement.Read(file))
		return false;
	file.Read(&eFocusing,sizeof(eFocusing));
	file.Read(&iAux,sizeof(iAux));
	iDelayCount = iAux;
	file.Read(&iAux,sizeof(iAux));
	iFocalCount = iAux;
	if(!afDelay.Read(file))
		return false;
	if(!adFocalTimeOfFlight.Read(file))
		return false;
	file.Read(&iAux,sizeof(iAux));
	iPrmCount = iAux;
	if(!afPrm.Read(file))
		return false;
	file.Read(&dSkew,sizeof(dSkew));
	file.Read(&dAngle,sizeof(dAngle));
	file.Read(&dX,sizeof(dX));
	file.Read(&dY,sizeof(dY));
	file.Read(&dZ,sizeof(dZ));
	file.Read(&dFocalX,sizeof(dFocalX));
	file.Read(&dFocalY,sizeof(dFocalY));
	file.Read(&dFocalZ,sizeof(dFocalZ));
	if(!aTRLWedgeDelayRef.Read(file))
		return false;
	if(!aTRLWedgeDelayReal.Read(file))
		return false;
	return true;
}

_CUST_DRV_DEF_ void OEMPA_ResetStructFocalLaw(CFocalLaw *pFocalLaw)
{
	pFocalLaw->Reset();
}
_CUST_DRV_DEF_ void OEMPA_ResetArrayFocalLaw(int iSize,CFocalLaw *pFocalLaw)
{
	for(int iIndex=0;iIndex<iSize;iIndex++)
		pFocalLaw[iIndex].Reset();
}

WORD g_wFilterHighPassOrder64Cut0p5MHz_[]={
0,
-44,//b0
-55,
-68,
-82,
-97,
-115,
-133,
-153,
-174,
-197,
-221,
-246,
-271,
-298,
-324,
-352,
-379,
-406,
-433,
-459,
-485,
-509,
-532,
-554,
-574,
-592,
-609,
-623,
-634,
-643,
-650,
-654,//b31
32113//b32
};
_CUST_DRV_DEF_ void OEMPA_ResetStructFilter(structFilter *paFilter)
{
	memset(paFilter->pTitle,0,sizeof(wchar_t)*g_iFilterTitleLength);
	wcscpy_s(paFilter->pTitle,g_iFilterTitleLength,L"HighPassCut0.5MHzOrder64");
	paFilter->wScale = 15;
	for(int iCoefficientIndex=0;iCoefficientIndex<g_iOEMPAFilterCoefficientMax;iCoefficientIndex++)
		paFilter->aCoefficient[iCoefficientIndex] = g_wFilterHighPassOrder64Cut0p5MHz_[iCoefficientIndex];
}
