// SpectrumDigitizer.cpp: implementation of the CSpectrumDigitizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "SpectrumDigitizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern UINT SpectrumThread(LPVOID pParam);


CSpectrumDigitizer::CSpectrumDigitizer()
{
	m_pThread = NULL;
	m_bThreadFinished = false;
	m_bThreadEnabled = false;
	m_bCardPresent = false;
	m_hDrv = NULL;
	m_nCardType = -1;
	m_CardType.Empty();

	m_bTimeBaseHW = false;
	m_bConfigureHW = false;

	m_fSamplingRate[0] = 5e9f;
	m_fSamplingRate[1] = 4e9f;
	m_fSamplingRate[2] = 3e9f;
	m_fSamplingRate[3] = 2e9f;
	m_fSamplingRate[4] = 1e9f;
	m_fSamplingRate[5] = 9e8f;
	m_fSamplingRate[6] = 8e8f;
	m_fSamplingRate[7] = 7e8f;
	m_fSamplingRate[8] = 6e8f;
	m_fSamplingRate[9] = 5e8f;
	m_fSamplingRate[10] = 4e8f;
	m_fSamplingRate[11] = 3e8f;
	m_fSamplingRate[12] = 2e8f;
	m_fSamplingRate[13] = 1.8e8f;
	m_fSamplingRate[14] = 1.5e8f;
	m_fSamplingRate[15] = 1.2e8f;
	m_fSamplingRate[16] = 1e8f;
	m_fSamplingRate[17] = 9e7f;
	m_fSamplingRate[18] = 8e7f;
	m_fSamplingRate[19] = 6e7f;
	m_fSamplingRate[20] = 5e7f;
	m_fSamplingRate[21] = 4e7f;
	m_fSamplingRate[22] = 3e7f;
	m_fSamplingRate[23] = 2.25e7f;

}

CSpectrumDigitizer::~CSpectrumDigitizer()
{
	SuspendThread();
	if(m_hDrv) spcm_vClose(m_hDrv);
}

int CSpectrumDigitizer::FindDevices()
{
	uint32 dwError = ERR_OK;

	m_hDrv = spcm_hOpen("/dev/spcm0");
	if(!m_hDrv) return 0;


	spcm_dwGetParam_i32(m_hDrv, SPC_PCITYP, &m_nCardType);
	m_CardType.Format(L"M3i.%04x",m_nCardType & TYP_VERSIONMASK);

	switch(m_nCardType) {
	case 0x52120: theApp.m_LastSettings.nAdcCardType = SpectrumM3i_2120;
	  break;
	case 0x52122: theApp.m_LastSettings.nAdcCardType = SpectrumM3i_2122;
	  break;
	case 0x52130: theApp.m_LastSettings.nAdcCardType = SpectrumM3i_2130;
	  break;
	case 0x52132: theApp.m_LastSettings.nAdcCardType = SpectrumM3i_2132;
	  break;
	default: m_CardType = L"Unsupported Spectrum";
		return 0;
		break;
	}

	spcm_dwGetParam_i32(m_hDrv, SPC_PCISAMPLERATE, &m_nMaxSupportedSamplingRate);
	spcm_dwGetParam_i32(m_hDrv, SPC_MIINST_BITSPERSAMPLE, &m_nBitsPerSample);

	theApp.m_Scope.m_nBits = m_nBitsPerSample;


	return 1;
}

void CSpectrumDigitizer::Configure()
{


	ConfigureHardware();
	ConfigureTimeBase();

}	
	
void CSpectrumDigitizer::ConfigureHardware()
{
	if(m_hDrv==0) return;

	//Analogue input
	int32 n50OhmTermination, nCoupling;
	switch(theApp.m_Scope.m_nCoupling) {
	case 0:
	case 1:	//DC 1MOhm
		n50OhmTermination = 0;
		nCoupling = 0;
		break;
	case 2:	//AC 1MOhm
		n50OhmTermination = 0;
		nCoupling = 1;
		break;
	case 3:	//DC 50Ohm
		n50OhmTermination = 1;
		nCoupling = 0;
		break;
	case 4:	//AC 50Ohm
		n50OhmTermination = 1;
		nCoupling = 1;
		break;
	}
	int32 nVoltageRange  = (int)((theApp.m_Scope.m_fVoltageRange * 1000.0f) / 2.0f);
	int32 nDCOffset = (int)theApp.m_Scope.m_fDCOffset;
	spcm_dwSetParam_i32(m_hDrv, SPC_CHENABLE, CHANNEL0);	//Only enable Channel0 not channel 1
	spcm_dwSetParam_i32(m_hDrv, SPC_PATH0, n50OhmTermination);				//0 = Buffered input, 1=  HF input 50 ohm termination
	spcm_dwSetParam_i32(m_hDrv, SPC_AMP0, nVoltageRange);	//+/- 2000 mV
	spcm_dwSetParam_i32(m_hDrv, SPC_OFFS0, nDCOffset);				//+/- offset in steps of 1%
	spcm_dwSetParam_i32(m_hDrv, SPC_ACDC0, nCoupling);				//0 DC couple, 1 AC coupled


	//Trigger
	int32 nTriggerVoltage = (int)theApp.m_Tank.fADCTriggerLevel;
	spcm_dwSetParam_i32(m_hDrv, SPC_TRIG_ORMASK, SPC_TMASK_EXT0);			//External trigger 0
	spcm_dwSetParam_i32(m_hDrv, SPC_TRIG_EXT0_LEVEL0, nTriggerVoltage);		//External trigger level 1800 mV
	spcm_dwSetParam_i32(m_hDrv, SPC_TRIG_EXT0_MODE, SPC_TM_POS);			//Positive edge triggered

	//Delay
	//Has to be here because it only increases in counts of 16 so cant be used at slow sapling rates
	spcm_dwSetParam_i32(m_hDrv, SPC_TRIG_DELAY, 0);					//Program delay / sampling rate, units are sample clocks

	CheckErrorMessage();

	theApp.m_Scope.CalculateLogConversionTable();

	m_bConfigureHW = false;

}

void CSpectrumDigitizer::ConfigureTimeBase()
{
	if(m_hDrv==0) return;
	if(theApp.m_Tank.nMaxADCConversionRate < 10000000) theApp.m_Tank.nMaxADCConversionRate = 1000000000;

	int nTS;
    uint32 dwError = ERR_OK;
	float fMaxTime = 0.0f;
	float fMaxWidth = 0.0f;
	float fMinDelay = 1e20f;
	static int nOldNumberSamples = -1;
	static int64 nOld64ClkRate = -1;

	for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		if(fMaxTime < (float)(theApp.m_UtUser.m_TS[nTS].Adc.nWidth + theApp.m_UtUser.m_TS[nTS].Adc.nDelay)) fMaxTime = (float)(theApp.m_UtUser.m_TS[nTS].Adc.nWidth + theApp.m_UtUser.m_TS[nTS].Adc.nDelay);
		if(fMaxWidth < (float)theApp.m_UtUser.m_TS[nTS].Adc.nWidth) fMaxWidth = (float)theApp.m_UtUser.m_TS[nTS].Adc.nWidth;
		if(fMinDelay > (float)theApp.m_UtUser.m_TS[nTS].Adc.nDelay) fMinDelay = (float)theApp.m_UtUser.m_TS[nTS].Adc.nDelay;
	}
	fMaxWidth += (theApp.m_Scope.m_fMainBangConstant * 1e9f);
	fMaxTime += (theApp.m_Scope.m_fMainBangConstant * 1e9f);

	float fSamplingRate = 1.0f / ((fMaxWidth * 1e-9f) / 8192.0f);
	if(fSamplingRate > (float)m_nMaxSupportedSamplingRate)			fSamplingRate = (float)m_nMaxSupportedSamplingRate;
	if(fSamplingRate > (float)theApp.m_Tank.nMaxADCConversionRate)	fSamplingRate = (float)theApp.m_Tank.nMaxADCConversionRate;
	int64 n64ClkRate = (int64)fSamplingRate;
	for(int ii=23;ii>=0;ii--) {
		if(m_fSamplingRate[ii] <= fSamplingRate) n64ClkRate = (LONGLONG)m_fSamplingRate[ii];
	}
	if(n64ClkRate < 22500000) n64ClkRate = 22500000;
	float fSamplePeriod = 1e9f / (float)n64ClkRate;

	int nNumberSamples = ((int)(fMaxTime / fSamplePeriod) & 0xfffffff0) + 32;
	if(nNumberSamples - nOldNumberSamples) {

		if (!dwError) dwError = spcm_dwSetParam_i32(m_hDrv, SPC_MEMSIZE, nNumberSamples);					//Set number of samples to be recorded
		if (!dwError) dwError = spcm_dwSetParam_i32(m_hDrv, SPC_POSTTRIGGER, nNumberSamples);					//Set number of samples to be recorded
		CheckErrorMessage();

		nOldNumberSamples = nNumberSamples;
	}

		
	if(	theApp.m_UtUser.m_TS[0].Adc.nConversionRate != (int)n64ClkRate  || nOld64ClkRate - n64ClkRate) {
		
		if (!dwError) dwError = spcm_dwSetParam_i64 (m_hDrv, SPC_SAMPLERATE, n64ClkRate);
		if (!dwError) dwError = spcm_dwGetParam_i64 (m_hDrv, SPC_SAMPLERATE, &n64ClkRate);


		CheckErrorMessage();
		nOld64ClkRate = n64ClkRate;
	}

	int nMaxAcqLength=0;
	for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod= fSamplePeriod;
		theApp.m_UtUser.m_TS[nTS].Adc.nConversionRate=(int)n64ClkRate;
		theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength=(int)((float)theApp.m_UtUser.m_TS[nTS].Adc.nWidth / fSamplePeriod);
		if(theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength < 1) theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength = 1;
		if(theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength >8192) theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength = 8192;

		theApp.m_UtUser.m_TS[nTS].Adc.nHardwareDelay=(int)(((float)theApp.m_UtUser.m_TS[nTS].Adc.nDelay + (theApp.m_Scope.m_fMainBangConstant * 1e9)) / fSamplePeriod);
	}
	m_bTimeBaseHW = false;
}

void CSpectrumDigitizer::CheckErrorMessage()
{
	char szErrorText[ERRORTEXTLEN];
	CString Message;
	uint32 dwErrorReg, dwErrorCode;
	int32 lErrorValue;

	if(spcm_dwGetErrorInfo_i32(m_hDrv, NULL, NULL, szErrorText) != ERR_OK) {
		dwErrorCode = spcm_dwGetErrorInfo_i32(m_hDrv, &dwErrorReg, &lErrorValue, NULL);
		Message.Format(L"Error code: %d in register %d at value %d", dwErrorCode, dwErrorReg, lErrorValue);
	}

}

CString &CSpectrumDigitizer::getName()
{
	return m_CardType;
}

void CSpectrumDigitizer::StartThread()
{
	if(m_hDrv==0) return;

	if(m_bThreadEnabled==FALSE) {
		m_bThreadEnabled=TRUE;
		m_pThread=AfxBeginThread(&SpectrumThread,this,THREAD_PRIORITY_TIME_CRITICAL ,0,NULL);
	};
}

void CSpectrumDigitizer::SuspendThread()
{
	if(m_bThreadEnabled==TRUE) {
		m_bThreadEnabled=FALSE;
		while(m_bThreadFinished==FALSE) {
			Sleep(1);
		};
	};

}

UINT SpectrumThread(LPVOID pParam)
{
	CSpectrumDigitizer* pParent = (CSpectrumDigitizer *)pParam;
	int nSlot = 0;
	int nBit, nPrSlot;
	int nActiveLaw=1;
	char *pcBuffer = new char[8192];
	bool bLogFlag;

	pParent->m_bThreadFinished = false;

	spcm_dwSetParam_i32(pParent->m_hDrv, SPC_M2CMD, M2CMD_CARD_START | M2CMD_CARD_ENABLETRIGGER);
	theApp.m_UtUser.ArmPrf();

	while(pParent->m_bThreadEnabled) {
		spcm_dwSetParam_i32(pParent->m_hDrv, SPC_TIMEOUT, 100);

		if(spcm_dwSetParam_i32(pParent->m_hDrv, SPC_M2CMD, M2CMD_CARD_WAITREADY) != ERR_TIMEOUT) {

			if(	theApp.m_LastSettings.nDesiredDevice==NOT_PHASED_ARRAY) {
				nBit = 	theApp.m_UtUser.ReadTS0();
				if(nBit) nSlot = theApp.m_UtUser.m_Global.Mux.nMaxTimeslots-1;
				nPrSlot = nSlot;

			} else {
				nSlot = theApp.m_PhasedArray[PORTSIDE].m_nFlOrder[nActiveLaw];
				nActiveLaw++;
				if(nActiveLaw>theApp.m_LSA.nFocalLawsL) {
					nActiveLaw=0;
				}

				theApp.m_LSA.CmdSetActiveFL(theApp.m_PhasedArray[PORTSIDE].m_nFlOrder[nActiveLaw]);
				nPrSlot = 0;
			}


			spcm_dwDefTransfer_i64(pParent->m_hDrv, SPCM_BUF_DATA, SPCM_DIR_CARDTOPC, 0, pcBuffer, theApp.m_UtUser.m_TS[nPrSlot].Adc.nHardwareDelay, theApp.m_UtUser.m_TS[nPrSlot].Adc.nAcqLength);
			spcm_dwSetParam_i32(pParent->m_hDrv, SPC_M2CMD, M2CMD_DATA_STARTDMA | M2CMD_DATA_WAITDMA);
			theApp.m_UtUser.m_TS[nPrSlot].Pr30.nOutputSelect == 2 ? bLogFlag = true : bLogFlag = false;
			theApp.m_Scope.FillScopeArray(nPrSlot, (char*)pcBuffer, theApp.m_UtUser.m_TS[nPrSlot].Adc.nAcqLength, bLogFlag, theApp.m_UtUser.m_TS[nPrSlot].Adc.nAverages);

			if(theApp.m_LSA.IsConnected() && theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
				char* pDest = theApp.m_Scope.m_RFTrace[nSlot];
				theApp.m_LSA.StoreAScan(nSlot, pDest, theApp.m_UtUser.m_TS[nPrSlot].Adc.nAcqLength);
			}

		} else {
			pParent->m_bConfigureHW = true;
			spcm_dwSetParam_i32 (pParent->m_hDrv, SPC_M2CMD, M2CMD_CARD_FORCETRIGGER);
		}

		if(pParent->m_bTimeBaseHW == true) pParent->ConfigureTimeBase();
		if(pParent->m_bConfigureHW == true) pParent->ConfigureHardware();

		spcm_dwSetParam_i32(pParent->m_hDrv, SPC_M2CMD, M2CMD_CARD_START | M2CMD_CARD_ENABLETRIGGER);
		theApp.m_UtUser.ArmPrf();

		theApp.m_UtUser.m_TS[nSlot].nIrqCount++;
		theApp.m_UtUser.ProcessTrace(theApp.m_Scope.m_RFTrace[nSlot],nSlot);
		nSlot++;
		nSlot %= theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;
		theApp.m_Acqiris.m_nConversions++;

	}

	delete pcBuffer;
	pParent->m_bThreadFinished = true;
	return 0;
}


