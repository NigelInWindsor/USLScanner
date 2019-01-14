
#include "stdafx.h"
#include "DlgHW.h"

bool FMCWriteHWFocalLaw(CHWDeviceOEMPA *pHWDeviceOEMPA,int iCycleCount,int iCycle,float fPulseWidth,float fGain)
{
	bool bRet=true;
	double dWedgeDelay;
	enumFocusing eFocusing=eStandard;
	DWORD adwHWAperture[g_iOEMPAApertureDWordCount]={0,0,0,0};
	float pfPrm[128];
	int iCount;

	if(!pHWDeviceOEMPA)
		return false;
	if(pHWDeviceOEMPA->IsMultiplexer())
		return false;
	switch(pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetElementCountMax())
	{
	default: return false;
	case 16: adwHWAperture[0] = 0x0000ffff;iCount = 16;break;
	case 32: adwHWAperture[0] = 0xffffffff;iCount = 32;break;
	case 64: adwHWAperture[0] = adwHWAperture[1] = 0xffffffff;iCount = 64;break;
	case 128:adwHWAperture[0] = adwHWAperture[1] = adwHWAperture[2] = adwHWAperture[3] = 0xffffffff;iCount = 128;break;
	}
	dWedgeDelay = 0.0;
	if(!pHWDeviceOEMPA->SetEmissionWedgeDelay(iCycle,iCycleCount,dWedgeDelay))
		return false;
	if(!pHWDeviceOEMPA->SetReceptionWedgeDelay(iCycle,iCycleCount,dWedgeDelay))
		return false;
	for(int iIndex=0;iIndex<iCount;iIndex++)
		pfPrm[iIndex] = fPulseWidth;
	if(!pHWDeviceOEMPA->SetEmissionWidths(iCycle,adwHWAperture,pfPrm))
		return false;
	for(int iIndex=0;iIndex<iCount;iIndex++)
		pfPrm[iIndex] = fGain;
	if(!pHWDeviceOEMPA->SetReceptionGains(iCycle,adwHWAperture,pfPrm))
		return false;

	for(int iIndex=0;iIndex<iCount;iIndex++)
		pfPrm[iIndex] = 0.0;
	if(!pHWDeviceOEMPA->SetEmissionDelays(iCycle,adwHWAperture,pfPrm))
		return false;
	int iFocalCount = 1;
	if(!pHWDeviceOEMPA->SetReceptionDelays(iCycle,iCount,iFocalCount,adwHWAperture,pfPrm))
		return false;

	if(!pHWDeviceOEMPA->SetEmissionEnable(iCycle,adwHWAperture))
		return false;
	if(!pHWDeviceOEMPA->SetReceptionEnable(iCycle,adwHWAperture))
		return false;
	if(!pHWDeviceOEMPA->EnableDDF(iCycle,eFocusing))
		return false;

	return bRet;
}

void CDlgHW::TestOEMMC1()
{
	bool bRet=true;
	int iCycleCount,iCycle;
	enumOEMPATrigger eTrig=eOEMPAInternal;
	enumOEMPARequestIO eReq=eOEMPAOnDigitalInputAndCycle;
	double dRange;
	enumCompressionType eComp=eCompression;
	long lPointCount, lPointFactor;
	double dTime;
	double dGainDigital;
	float fGainAnalog,fBeamCorrection;
	WORD wID=65535;
	DWORD adwHWAperture[4]={0};
	float fWidth,fGain;
	bool bEnable;
	enumGateModeAmp amp;
	enumGateModeTof tof;
	enumRectification eRect;
	double dTh;
	int iAcqElem,iCount;
	float fStart;
	double adTime[g_iOEMPADACCountMax]={0.0};
	float afGain[g_iOEMPADACCountMax]={0.0f};

	if(m_pHWDeviceOEMPA->LockDevice())
	{
		//SetCycleCount(2)
		iCycleCount = 2;
		if(!m_pHWDeviceOEMPA->SetCycleCount(iCycleCount))
			bRet = false;
		//EnableMultiHWChannel(True) = True
		if(!m_pHWDeviceOEMPA->EnableMultiHWChannel(true))
			bRet = false;
		//EnableAscan(True)
		if(!m_pHWDeviceOEMPA->EnableAscan(true))
			bRet = false;
		//EnableCscanTof(True)
		if(!m_pHWDeviceOEMPA->EnableCscanTof(true))
			bRet = false;
		//SetAscanBitSize(e8Bits)
		if(!m_pHWDeviceOEMPA->SetAscanBitSize(e8Bits))
			bRet = false;
		//SetExternalTriggerSequence(eDigitalInputOff)
		if(!m_pHWDeviceOEMPA->SetExternalTriggerSequence(eDigitalInputOff))
			bRet = false;
		//SetExternalTriggerCycle(eDigitalInputOff)
		if(!m_pHWDeviceOEMPA->SetExternalTriggerCycle(eDigitalInputOff))
			bRet = false;
		//SetTriggerMode(eOEMPAInternal)
		if(!m_pHWDeviceOEMPA->SetTriggerMode(eTrig))
			bRet = false;
		//SetRequestIO(eOEMPAOnDigitalInputAndCycle) Succeeded
		if(!m_pHWDeviceOEMPA->SetRequestIO(eReq))
			bRet = false;
		//SetAscanRequest(eAscanAll)
		if(!m_pHWDeviceOEMPA->SetAscanRequest(eAscanAll))
			bRet = false;
		//Flush() Succeeded
		if(!m_pHWDeviceOEMPA->Flush())
			bRet = false;
//cycle 0
iCycle = 0;
		//SetAscanRangeWithCount( 0, 50.000000 us, eDecimation, 5000, 1) Succeeded
		dRange = 50.0e-6;
		lPointCount = 5001;//error 5001 instead of 5000
		lPointFactor = 1;
		if(!m_pHWDeviceOEMPA->SetAscanRangeWithCount(iCycle, dRange, eComp, lPointCount, lPointFactor))
			bRet = false;
		//SetTimeSlot( 0, 0.001000) Succeeded
		dTime = 0.001;
		if(!m_pHWDeviceOEMPA->SetTimeSlot(iCycle, dTime))
			bRet = false;
		//EnableMultiHWChannelAcquisition(0,1,True) = True
		if(!m_pHWDeviceOEMPA->EnableMultiHWChannelAcquisition(iCycle,iCycleCount,true))//error 2 instead of 1
			bRet = false;
		//EnableAscanMaximum(0, True) Succeeded
		if(!m_pHWDeviceOEMPA->EnableAscanMaximum(iCycle,true))
			bRet = false;
		//EnableAscanMinimum(0, False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableAscanMinimum(iCycle, false))
			bRet = false;
		//EnableAscanSaturation(0, False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableAscanSaturation(iCycle, false))
			bRet = false;
		//SetAscanRectification(0, eSigned) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanRectification(iCycle, eSigned))
			bRet = false;
		//SetGainDigital(0, 0.000000) Succeeded
		dGainDigital = 0.0;
		if(!m_pHWDeviceOEMPA->SetGainDigital(iCycle, dGainDigital))
			bRet = false;
		//SetGainAnalog(0,30.000000) Succeeded
		fGainAnalog = 30.0f;
		if(!m_pHWDeviceOEMPA->SetGainAnalog(iCycle,fGainAnalog))
			bRet = false;
		//EnableDAC(0, False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableDAC(iCycle, false))
			bRet = false;
		//SetFilterIndex(cycle 0, eOEMPAFilterOff) Succeeded
		if(!m_pHWDeviceOEMPA->SetFilterIndex(iCycle, eOEMPAFilterOff))
			bRet = false;
		//SetBeamCorrection(0,0.000000) Succeeded
		fBeamCorrection = 0.0;
		if(!m_pHWDeviceOEMPA->SetBeamCorrection(iCycle,fBeamCorrection))
			bRet = false;
		//SetAscanAcqIdChannelProbe(0, 65535) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanAcqIdChannelProbe(iCycle, wID))
			bRet = false;
		//SetAscanAcqIdChannelScan(0, 65535) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanAcqIdChannelScan(iCycle, wID))
			bRet = false;
		//SetAscanAcqIdChannelCycle(0, 65535) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanAcqIdChannelCycle(iCycle, wID))
			bRet = false;
		//SetEmissionWidths(0, 0x1, 0x0, 0x0, 0x0, 0.030000 us) Succeeded
		adwHWAperture[0] = 1;
		fWidth = 0.030e-6f;
		if(!m_pHWDeviceOEMPA->SetEmissionWidths(iCycle, adwHWAperture, &fWidth))//modif
			bRet = false;
		//EnableDDF(0,False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableDDF(iCycle,false))
			bRet = false;
		//SetReceptionGains(0, 0x1, 0x0, 0x0, 0x0, 0.000000) Succeeded
		adwHWAperture[0] = 1;
		fGain = 0.0;
		if(!m_pHWDeviceOEMPA->SetReceptionGains(iCycle, adwHWAperture, &fGain))
			bRet = false;
		//SetEmissionEnable(0, 0x1, 0x0, 0x0, 0x0) Succeeded
		adwHWAperture[0] = 1;
		if(!m_pHWDeviceOEMPA->SetEmissionEnable(iCycle, adwHWAperture))
			bRet = false;
		//SetReceptionEnable(0, 0x1, 0x0, 0x0, 0x0) Succeeded
		adwHWAperture[0] = 1;
		if(!m_pHWDeviceOEMPA->SetReceptionEnable(iCycle, adwHWAperture))
			bRet = false;
		//SetMultiHWChannelAcqWriteStart(0,0,1,0.000000) = Succeeded
		iAcqElem = 0;//should be same than adwHWAperture
		fStart = 0.0;
		if(!m_pHWDeviceOEMPA->SetMultiHWChannelAcqWriteStart(iCycle,iAcqElem,1,&fStart))
			bRet = false;
		//SetGateModeThreshold(0,0,False,eAmpMaximum,eTofAmplitudeDetection,eSigned,0.000000) Succeeded
		//SetGateModeThreshold(0,1,False,eAmpMaximum,eTofAmplitudeDetection,eSigned,0.000000) Succeeded
		//SetGateModeThreshold(0,2,False,eAmpMaximum,eTofAmplitudeDetection,eSigned,0.000000) Succeeded
		//SetGateModeThreshold(0,3,False,eAmpMaximum,eTofAmplitudeDetection,eSigned,0.000000) Succeeded
		bEnable = false;
		amp = eAmpMaximum;
		tof = eTofAmplitudeDetection;
		eRect = eSigned;
		dTh = 0.0;
		for(int iGate=0;iGate<4;iGate++)
		{
			if(!m_pHWDeviceOEMPA->SetGateModeThreshold(iCycle,iGate,bEnable,amp,tof,eRect,dTh))
				bRet = false;
		}
//cycle 1
iCycle = 1;
		//SetAscanRangeWithCount( 1, 50.000000 us, eDecimation, 5000, 1) Succeeded
		dRange = 50.0e-6;
		lPointCount = 5001;//error instead of 5000.
		lPointFactor = 1;
		if(!m_pHWDeviceOEMPA->SetAscanRangeWithCount(iCycle, dRange, eComp, lPointCount, lPointFactor))
			bRet = false;
		//SetTimeSlot( 1, 0.001000) Succeeded
		dTime = 0.001;
		if(!m_pHWDeviceOEMPA->SetTimeSlot(iCycle, dTime))
			bRet = false;
		//EnableMultiHWChannelAcquisition(1,1,True) = True
		if(!m_pHWDeviceOEMPA->EnableMultiHWChannelAcquisition(iCycle,iCycleCount,true))//error 2 instead of 1
			bRet = false;
		//EnableAscanMaximum(1, True) Succeeded
		if(!m_pHWDeviceOEMPA->EnableAscanMaximum(iCycle,true))
			bRet = false;
		//EnableAscanMinimum(1, False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableAscanMinimum(iCycle, false))
			bRet = false;
		//EnableAscanSaturation(1, False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableAscanSaturation(iCycle, false))
			bRet = false;
		//SetAscanRectification(1, eSigned) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanRectification(iCycle, eSigned))
			bRet = false;
		//SetGainDigital(1, 0.000000) Succeeded
		dGainDigital = 0.0;
		if(!m_pHWDeviceOEMPA->SetGainDigital(iCycle, dGainDigital))
			bRet = false;
		//SetGainAnalog(1,0.000000) Succeeded
		fGainAnalog = 0.0f;
		if(!m_pHWDeviceOEMPA->SetGainAnalog(iCycle,fGainAnalog))
			bRet = false;
		//EnableDAC(1, False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableDAC(iCycle, false))
			bRet = false;
		//SetFilterIndex(cycle 1, eOEMPAFilterOff) Succeeded
		if(!m_pHWDeviceOEMPA->SetFilterIndex(iCycle, eOEMPAFilterOff))
			bRet = false;
		//SetBeamCorrection(1,0.000000) Succeeded
		fBeamCorrection = 0.0;
		if(!m_pHWDeviceOEMPA->SetBeamCorrection(iCycle,fBeamCorrection))
			bRet = false;
		//SetAscanAcqIdChannelProbe(1, 65535) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanAcqIdChannelProbe(iCycle, wID))
			bRet = false;
		//SetAscanAcqIdChannelScan(1, 65535) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanAcqIdChannelScan(iCycle, wID))
			bRet = false;
		//SetAscanAcqIdChannelCycle(1, 65535) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanAcqIdChannelCycle(iCycle, wID))
			bRet = false;
		//SetEmissionWidths(1, 0x2, 0x0, 0x0, 0x0, 0.030000 us) Succeeded
		adwHWAperture[0] = 2;
		fWidth = 0.030e-6f;
		if(!m_pHWDeviceOEMPA->SetEmissionWidths(iCycle, adwHWAperture, &fWidth))//modif
			bRet = false;
		//EnableDDF(1,False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableDDF(iCycle,false))
			bRet = false;
		//SetReceptionGains(1, 0x2, 0x0, 0x0, 0x0, 0.000000) Succeeded
		adwHWAperture[0] = 2;
		fGain = 0.0;
		if(!m_pHWDeviceOEMPA->SetReceptionGains(iCycle, adwHWAperture, &fGain))
			bRet = false;
		//SetEmissionEnable(1, 0x2, 0x0, 0x0, 0x0) Succeeded
		adwHWAperture[0] = 2;
		if(!m_pHWDeviceOEMPA->SetEmissionEnable(iCycle, adwHWAperture))
			bRet = false;
		//SetReceptionEnable(1, 0x2, 0x0, 0x0, 0x0) Succeeded
		adwHWAperture[0] = 2;
		if(!m_pHWDeviceOEMPA->SetReceptionEnable(iCycle, adwHWAperture))
			bRet = false;
		//SetMultiHWChannelAcqWriteStart(1,1,1,0.000000) = Succeeded
		iAcqElem = 1;//should be same than adwHWAperture
		fStart = 0.0;
		if(!m_pHWDeviceOEMPA->SetMultiHWChannelAcqWriteStart(iCycle,iAcqElem,1,&fStart))
			bRet = false;
		//SetGateModeThreshold(1,0,False,eAmpMaximum,eTofAmplitudeDetection,eSigned,0.000000) Succeeded
		//SetGateModeThreshold(1,1,False,eAmpMaximum,eTofAmplitudeDetection,eSigned,0.000000) Succeeded
		//SetGateModeThreshold(1,2,False,eAmpMaximum,eTofAmplitudeDetection,eSigned,0.000000) Succeeded
		//SetGateModeThreshold(1,3,False,eAmpMaximum,eTofAmplitudeDetection,eSigned,0.000000) Succeeded
		bEnable = false;
		amp = eAmpMaximum;
		tof = eTofAmplitudeDetection;
		eRect = eSigned;
		dTh = 0.0;
		for(int iGate=0;iGate<4;iGate++)
		{
			if(!m_pHWDeviceOEMPA->SetGateModeThreshold(iCycle,iGate,bEnable,amp,tof,eRect,dTh))
				bRet = false;
		}
//EXTRA to initialize DAC memory of the 2d cycle (the DAC of the first cycle has alreday been initialized at connection time when the default OEMMC file has been loaded).
		iCount = 0;
		if(!m_pHWDeviceOEMPA->SetDACGain(false,iCycle,iCount,adTime,afGain))
			bRet = false;
		//Flush() Succeeded
		if(!m_pHWDeviceOEMPA->UnlockDevice())
			bRet = false;
	}else
		bRet = false;
	if(bRet)
		AfxMessageBox(L"Succeeded");
	else
		AfxMessageBox(L"Error");
}

void CDlgHW::TestOEMMC2()
{
	bool bRet=true;
	int iCycleCount,iCycle;
	enumOEMPATrigger eTrig=eOEMPAInternal;
	enumOEMPARequestIO eReq=eOEMPAOnDigitalInputAndCycle;
	double dRange;
	enumCompressionType eComp=eCompression;
	long lPointCount, lPointFactor;
	double dTime;
	double dGainDigital;
	float fGainAnalog,fBeamCorrection;
	WORD wID=65535;
	DWORD adwHWAperture[4]={0};
	float fWidth,fGain;
	int iAcqElem;
	float fStart;
	double adTime[g_iOEMPADACCountMax]={0.0};
	float afGain[g_iOEMPADACCountMax]={0.0f};
	float *pfStart;
	unionOEMMCDecimation hwAcqDecimation;

	if(m_pHWDeviceOEMPA->LockDevice())
	{
		iCycle = 0;
		//SetCycleCount(1)  Succeeded
		iCycleCount = 1;
		if(!m_pHWDeviceOEMPA->SetCycleCount(iCycleCount))
			bRet = false;
		//EnableMultiHWChannel(True) = Succeeded
		if(!m_pHWDeviceOEMPA->EnableMultiHWChannel(true))
			bRet = false;
		//EnableAscan(True) Succeeded
		if(!m_pHWDeviceOEMPA->EnableAscan(true))
			bRet = false;
		//EnableCscanTof(True) Succeeded
		if(!m_pHWDeviceOEMPA->EnableCscanTof(true))
			bRet = false;
		//SetAscanBitSize(e8Bits) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanBitSize(e8Bits))
			bRet = false;
		//SetExternalTriggerSequence(eDigitalInputOff) Succeeded
		if(!m_pHWDeviceOEMPA->SetExternalTriggerSequence(eDigitalInputOff))
			bRet = false;
		//SetExternalTriggerCycle(eDigitalInputOff) Succeeded
		if(!m_pHWDeviceOEMPA->SetExternalTriggerCycle(eDigitalInputOff))
			bRet = false;
		//SetTriggerMode(eOEMPAInternal) Succeeded
		eTrig = eOEMPAInternal;
		if(!m_pHWDeviceOEMPA->SetTriggerMode(eTrig))
			bRet = false;
		//SetRequestIO(eOEMPAOnDigitalInputAndCycle) Succeeded
		eReq = eOEMPAOnDigitalInputAndCycle;
		if(!m_pHWDeviceOEMPA->SetRequestIO(eReq))
			bRet = false;
		//SetAscanRequest(eAscanAll) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanRequest(eAscanAll))
			bRet = false;
		//SetDigitalOutput(1, eOEMPASignalSequence) Succeeded
		//SetFilter(eOEMPAFilter5, scale 15, 34 wValues: 0,157,259,331,359,337,264,148, ......) Succeeded
		//SetAscanRangeWithCount( 0, 50.000000 us, eDecimation, 5000, 1) Succeeded
		dRange = 50.0e-6;
		lPointCount = 5001;//error 5001 instead of 5000
		lPointFactor = 1;
		eComp = eDecimation;
		if(!m_pHWDeviceOEMPA->SetAscanRangeWithCount(iCycle, dRange, eComp, lPointCount, lPointFactor))
			bRet = false;
		//SetTimeSlot( 0, 0.033333) Succeeded
		dTime = 0.033333;
		if(!m_pHWDeviceOEMPA->SetTimeSlot(iCycle, dTime))
			bRet = false;
		//EnableMultiHWChannelAcquisition(0,1,True) = True
		if(!m_pHWDeviceOEMPA->EnableMultiHWChannelAcquisition(iCycle,iCycleCount,true))//error 2 instead of 1
			bRet = false;
		//EnableAscanMaximum(0, True) Succeeded
		if(!m_pHWDeviceOEMPA->EnableAscanMaximum(iCycle,true))
			bRet = false;
		//EnableAscanMinimum(0, False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableAscanMinimum(iCycle, false))
			bRet = false;
		//EnableAscanSaturation(0, False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableAscanSaturation(iCycle, false))
			bRet = false;
		//SetAscanRectification(0, eSigned) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanRectification(iCycle, eSigned))
			bRet = false;
		//SetGainDigital(0, 12.000000) Succeeded
		dGainDigital = 0.0;//instead of 12.0
		if(!m_pHWDeviceOEMPA->SetGainDigital(iCycle, dGainDigital))
			bRet = false;
		//SetGainAnalog(0,28.000000) Succeeded
		fGainAnalog = 10.0f;//instead of 28.0f;
		if(!m_pHWDeviceOEMPA->SetGainAnalog(iCycle,fGainAnalog))
			bRet = false;
		//SetDACGain(False,0,0,,) Succeeded
		//EnableDAC(0, False) Succeeded
		if(!m_pHWDeviceOEMPA->EnableDAC(iCycle, false))
			bRet = false;
		//SetFilterIndex(cycle 0, eOEMPAFilterOff) Succeeded
		if(!m_pHWDeviceOEMPA->SetFilterIndex(iCycle, eOEMPAFilterOff))
			bRet = false;
		//SetBeamCorrection(0,0.000000) Succeeded
		fBeamCorrection = 0.0;
		if(!m_pHWDeviceOEMPA->SetBeamCorrection(iCycle,fBeamCorrection))
			bRet = false;
		//SetAscanAcqIdChannelProbe(0, 65535) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanAcqIdChannelProbe(iCycle, wID))
			bRet = false;
		//SetAscanAcqIdChannelScan(0, 65535) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanAcqIdChannelScan(iCycle, wID))
			bRet = false;
		//SetAscanAcqIdChannelCycle(0, 65535) Succeeded
		if(!m_pHWDeviceOEMPA->SetAscanAcqIdChannelCycle(iCycle, wID))
			bRet = false;
		//SetEmissionWidths(0, 0x1, 0x0, 0x0, 0x0, 0.060000 us) Succeeded
		adwHWAperture[0] = 1;
		fWidth = 0.060e-6f;
		if(!m_pHWDeviceOEMPA->SetEmissionWidths(iCycle, adwHWAperture, &fWidth))//modif
			bRet = false;
//EnableDDF(0,False) Succeeded
//if(!m_pHWDeviceOEMPA->EnableDDF(iCycle,false))
//	bRet = false;
		//SetReceptionGains(0, 0x1, 0x0, 0x0, 0x0, 0.000000) Succeeded
		adwHWAperture[0] = 1;
		fGain = 0.0;
		if(!m_pHWDeviceOEMPA->SetReceptionGains(iCycle, adwHWAperture, &fGain))
			bRet = false;
		//SetEmissionEnable(0, 0x1, 0x0, 0x0, 0x0) Succeeded
		adwHWAperture[0] = 1;
		if(!m_pHWDeviceOEMPA->SetEmissionEnable(iCycle, adwHWAperture))
			bRet = false;
		//SetReceptionEnable(0, 0x1, 0x0, 0x0, 0x0) Succeeded
		adwHWAperture[0] = 1;
		if(!m_pHWDeviceOEMPA->SetReceptionEnable(iCycle, adwHWAperture))
			bRet = false;
		//SetMultiHWChannelAcqWriteStart(0,1,1,0.000010) = Succeeded
		iAcqElem = 0;//should be same than adwHWAperture
		fStart = 0.000010f;
		pfStart = &fStart;
		if(!m_pHWDeviceOEMPA->SetMultiHWChannelAcqWriteStart(iCycle,iAcqElem,1,pfStart))
			bRet = false;
		//SetMultiHWChannelAcqDecimation(0,1226912) = Succeeded
		hwAcqDecimation.dword[0] = 1226912;
		if(!m_pHWDeviceOEMPA->SetMultiHWChannelAcqDecimation(iCycle,hwAcqDecimation))
			bRet = false;
		if(!m_pHWDeviceOEMPA->UnlockDevice())
			bRet = false;
	}else
		bRet = false;
	if(bRet)
		AfxMessageBox(L"Succeeded");
	else
		AfxMessageBox(L"Error");
}

bool TestFunction()
{
        const int nNumUnits = 5;
        const LPWSTR szIPs[] = { L"192.168.1.25"
                , L"192.168.1.24"
                , L"192.168.1.13"
                , L"192.168.1.14"
                , L"192.168.1.11"
        };
		CHWDeviceOEMPA vecUnits[nNumUnits],*pUnit;

        auto link = eMaster;
        for ( int i = 0; i < nNumUnits; i++)
        {
                //vecUnits.push_back(std::make_shared<CFPHWDeviceOEMPA>());
                /*auto*/ pUnit = &vecUnits[i];

                if ( !pUnit->SetAcquisitionParameter(nullptr) ||
                        !pUnit->SetAcquisitionAscan_0x00010103(CDlgHW::ProcessAcquisitionAscan_0x00010103) ||
                        !pUnit->SetAcquisitionCscan_0x00010X02(CDlgHW::ProcessAcquisitionCscan_0x00010X02) ||
                        //!pUnit->SetAcquisitionInfo(CAOSUnit::CallbackAcquisitionInfo) ||
                        !OEMPA_SetCallbackCustomizedDriverAPI(CDlgHW::CallbackCustomizedAPI) )
                {
                        return false;
                }

                pUnit->EnableCommonKeepAlive(false);
                bool bPreviousMasterUnregistered;
                if ( ! pUnit->SetDefaultHwLink(link, bPreviousMasterUnregistered) )
                        return false;

                link = eSlave;

                if ( !pUnit->GetSWDeviceOEMPA()->SetIPAddress(szIPs[i]) )
                {
                        return false ;
                }
               
                if ( !pUnit->GetSWDevice()->IsConnected() )
                {
                        if ( !pUnit->GetSWDevice()->SetConnectionState(eConnected, false) ||
                                !pUnit->GetSWDevice()->IsConnected() )
                        {
                                return false;
                        }
                }

                if ( ! pUnit->LockDevice(eAcqOff) )
                {
                        return false;
                }

                if ( !pUnit->ResetTimeStamp() )
                {
                        return false;
                }

                pUnit->SetSettingId(0);//s_dwSettingId);

                if ( !pUnit->GetSWDeviceOEMPA()->IsOEMMCEnabled() )
                {
                        float fMaxCorrection, fTimeOffset;
                        pUnit->PerformCalibration(fMaxCorrection, fTimeOffset);
                }

                if ( ! pUnit->UnlockDevice(eAcqOff) )
                {
                        return false;
                }
        }


        // disconnect
       
        enumHardwareLink eLink;

        //for ( auto& pUnit : vecUnits )
        //{
        /*for ( int i = 0; i < nNumUnits; i++)
        {
                //vecUnits.push_back(std::make_shared<CFPHWDeviceOEMPA>());
                pUnit = &vecUnits[i];
                bool bError = false;
                eLink = pUnit->GetDefaultHwLink();
                if ( eLink == eSlave )
                {
                        if ( !pUnit->SlaveDisconnect() )
                        {
                                bError = true;
                        }
                }
        }*/

        //for ( auto& pUnit : vecUnits )
        //{
        /*for ( int i = 0; i < nNumUnits; i++)
        {
                //vecUnits.push_back(std::make_shared<CFPHWDeviceOEMPA>());
                pUnit = &vecUnits[i];
                eLink = pUnit->GetDefaultHwLink();
                if ( eLink == eSlave )
                {
                        if ( !pUnit->GetSWDevice()->SetConnectionState(eDisconnected, false) )
                        {
                                return false;
                        }
                }
        }*/

        //for ( auto& pUnit : vecUnits )
        //{
        for ( int i = 0; i < nNumUnits; i++)
        {
                //vecUnits.push_back(std::make_shared<CFPHWDeviceOEMPA>());
                /*auto*/ pUnit = &vecUnits[i];
                eLink = pUnit->GetDefaultHwLink();
                //if ( eLink == eMaster )
                {
                        if ( !pUnit->GetSWDevice()->SetConnectionState(eDisconnected, false) )
                        {
                                return false;
                        }
                }
        }
		return true;

}
