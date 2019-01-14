// AcqirisDigitizer.cpp: implementation of the CAcqirisDigitizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "AcqirisDigitizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Pr30 origional jumper settings
//3 and 5 counting from i2c end

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	DC_OFFSET	0

CAcqirisDigitizer::CAcqirisDigitizer()
{
	float fDefaultRate[]= {2.0,2.5,4.0,5.0,10.0,20.0,25.0,40.0,50.0,100.0};

	m_bCardPresent=FALSE;
	m_bThreadEnabled=FALSE;
	m_nConversions=0;
	m_nSlotErrorCount=0;
	for(int ii=0;ii<9;ii++) 
		m_fRate[ii] = fDefaultRate[ii];

}

CAcqirisDigitizer::~CAcqirisDigitizer()
{

	SuspendThread();
	ViStatus status = Acqrs_closeAll(); 

}

int CAcqirisDigitizer::FindDevices()
{
	CString Buff;
	ViStatus status;
	ViChar cName[32];
	ViInt32	nSerialNumber,nBus,nSlot;
	ViAddr infoAddr;
//    ViSession instrID; // Instrument handle
    ViConstString rscStr; // Resource string
    ViConstString options; // Initialization options


	status = Acqrs_getNbrInstruments(&m_nNumInstruments);

	if(m_nNumInstruments == 0) return 0;

	switch(status) {
	case 0xbffa4c05: Buff = "Unsupported Acquiris Device";
		MessageBox(NULL,Buff,_T("Error"),MB_ICONERROR);
		return 0;
		break;
	}

	theApp.m_Scope.m_nBits = 8;

	if(theApp.m_Tank.nCalibrateAdc == 1) {
		options = "";
	} else {
		options = "cal=0"; // Skip self-calibration
	}

	// Initialize the digitizers
	for (int i = 0; i < m_nNumInstruments; i++)
	{
		char resourceName[20];
		sprintf_s(resourceName,20, "PCI::INSTR%d", i);
        rscStr = resourceName;

		status = Acqrs_InitWithOptions(resourceName, VI_FALSE, VI_FALSE, options, &(InstrumentID[i]));

		status = Acqrs_getInstrumentData(InstrumentID[i],cName,&nSerialNumber,&nBus,&nSlot);


		theApp.m_LastSettings.nAdcCardType = Acqiris_DP210;
		if(strcmp(cName,"DP1400") == 0)		theApp.m_LastSettings.nAdcCardType = Acqiris_DP1400;
		if(strcmp(cName,"U1071AHZ4")==0)	theApp.m_LastSettings.nAdcCardType = Acqiris_U1071AHZ4;
		if(strcmp(cName,"U1071AHZ2")==0) 	theApp.m_LastSettings.nAdcCardType = Acqiris_U1071AHZ2;

		SetConversionRates();

		m_bCardPresent = TRUE;

		options = "NbrExternalTriggers";
		status = Acqrs_getInstrumentInfo(InstrumentID[i],options,&infoAddr);
	
	}
	return m_nNumInstruments;

}

void CAcqirisDigitizer::SetConversionRates()
{
	float fDefaultRate[]= {2.0,2.5,4.0,5.0,10.0,20.0,25.0,40.0,50.0,100.0};
	float fDP1400Rate[]= {1.0,2.0,4.0,5.0,10.0,20.0,40.0,50.0,100.0};
	float fU1071AHZ4Rate[]= {1.0,2.0,4.0,5.0,10.0,20.0,40.0,50.0,100.0};
	float fU1071AHZ2Rate[]= {0.5,1.0,2.0,4.0,5.0,10.0,20.0,40.0,50.0,100.0};
	int nSrc,nDest;

	if(theApp.m_Tank.nMaxADCConversionRate < 10000000) theApp.m_Tank.nMaxADCConversionRate = 10000000;
	float fFastestSpeed = 1.0e9f / (float)theApp.m_Tank.nMaxADCConversionRate;

	for(nSrc=0;nSrc<10;nSrc++) m_fRate[nSrc] = 100.0f;

	switch(theApp.m_LastSettings.nAdcCardType ) {
	default:
		for(nSrc=nDest=0;nSrc<10;nSrc++) {
			if(fDefaultRate[nSrc] >= fFastestSpeed)	m_fRate[nDest++] = fDefaultRate[nSrc];
		}
		break;
	case Acqiris_DP1400:
		for(nSrc=nDest=0;nSrc<10;nSrc++) {
			if(fDP1400Rate[nSrc] >= fFastestSpeed)	m_fRate[nDest++] = fDP1400Rate[nSrc];
		}
		break;
	case Acqiris_U1071AHZ4:
		for(nSrc=nDest=0;nSrc<10;nSrc++) {
			if(fU1071AHZ4Rate[nSrc] >= fFastestSpeed)	m_fRate[nDest++] = fU1071AHZ4Rate[nSrc];
		}
		break;
	case Acqiris_U1071AHZ2:
		for(nSrc=nDest=0;nSrc<10;nSrc++) {
			if(fU1071AHZ2Rate[nSrc] >= fFastestSpeed)	m_fRate[nDest++] = fU1071AHZ2Rate[nSrc];
		}
		break;
	}

}

void CAcqirisDigitizer::Configure()
{
	int	nAcqLength,nClkRate,nMaxClkRate,nTS,nMaxAcqLength;
	float fWidth;
	ViStatus status;
	ViInt32 nbrConvertersPerChannel;

	theApp.m_Scope.m_nEncodeing = SIGNED_BINARY;

	nMaxClkRate = 0;
	for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		fWidth = (float)theApp.m_UtUser.m_TS[nTS].Adc.nWidth;
		for(nClkRate=0;nClkRate<9;nClkRate++) {
			nAcqLength=(int)(fWidth / m_fRate[nClkRate]);
			if(nAcqLength<8192) goto finished;
		};
		finished:
		if(nClkRate > nMaxClkRate) nMaxClkRate = nClkRate;
	}

	nMaxAcqLength=0;
	for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod=m_fRate[nMaxClkRate];
		theApp.m_UtUser.m_TS[nTS].Adc.nConversionRate=nMaxClkRate;
		theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength=(int)((float)theApp.m_UtUser.m_TS[nTS].Adc.nWidth / m_fRate[nMaxClkRate]);
		nAcqLength = theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength + (int)((float)theApp.m_UtUser.m_TS[nTS].Adc.nDelay / m_fRate[nMaxClkRate]);
		if(nAcqLength > nMaxAcqLength) nMaxAcqLength = nAcqLength;
	}

	double sampInterval = (double)m_fRate[nMaxClkRate] * 1e-9;	//seconds
	double delayTime = 0.0 + theApp.m_Scope.m_fMainBangConstant;		//seconds
	long nbrSamples = nMaxAcqLength;
	long nbrSegments = 1;
	long coupling = 1;
	long bandwidth = 0;
	double fullScale = (double)theApp.m_Scope.m_fVoltageRange;
	double offset = theApp.m_Scope.m_fVoltageRange * (double)(theApp.m_Scope.m_fDCOffset / 100.0f);
	long trigCoupling = 0;
	long trigSlope = 0;
	if(theApp.m_Tank.fADCTriggerLevel <= 0.0f) theApp.m_Tank.fADCTriggerLevel = 2500.0f;
	double trigLevel = (double)theApp.m_Tank.fADCTriggerLevel;

	// Configure timebase

	nbrConvertersPerChannel = 1;
	if(theApp.m_LastSettings.nAdcCardType == Acqiris_U1071AHZ4) {
		nbrConvertersPerChannel = 2;
	} 
	if(theApp.m_LastSettings.nAdcCardType == Acqiris_U1071AHZ2) {
		nbrConvertersPerChannel = 1;
	} 
	if((theApp.m_LastSettings.nAdcCardType == Acqiris_DP1400) && (sampInterval == 1.0)) {
		nbrConvertersPerChannel = 1;
	} 

	ViInt32 usedChannels = 1;
	status = AcqrsD1_configChannelCombination(InstrumentID[0],nbrConvertersPerChannel,usedChannels);
	status = AcqrsD1_configHorizontal(InstrumentID[0], sampInterval, delayTime);
	AcqrsD1_configMemory(InstrumentID[0], nbrSamples, nbrSegments);

	// Configure vertical settings of channel 1
	AcqrsD1_configVertical(InstrumentID[0], 1, fullScale, offset, theApp.m_Scope.m_nCoupling, bandwidth);
	AcqrsD1_configVertical(InstrumentID[0], 2, fullScale, offset, theApp.m_Scope.m_nCoupling, bandwidth);

	
//	AcqrsD1_configTrigClass(InstrumentID[0], 0, 0x2, 0, 0, 0.0, 0.0);						// Configure edge trigger on channel 2
//	AcqrsD1_configTrigSource(InstrumentID[0], 1, trigCoupling, trigSlope, trigLevel, 0.0);		// Configure the trigger conditions of channel 1 internal trigger

	
	AcqrsD1_configTrigClass(InstrumentID[0], 0, 0x80000000, 0, 0, 0.0, 0.0);
	AcqrsD1_configTrigSource(InstrumentID[0], -1, trigCoupling, trigSlope, trigLevel, 0.0);

	double dSample,dDelay;
	AcqrsD1_getHorizontal(InstrumentID[0], &dSample, &dDelay);

	theApp.m_Scope.CalculateLogConversionTable();

}


void CAcqirisDigitizer::Vertical(ADC200Data* pAdc)
{
	long bandwidth = 0;
	double fullScale = (double)theApp.m_Scope.m_fVoltageRange;
	double offset = 1.0 * (double)(pAdc->nOffset-0x800) / (double)0x800;

//	AcqrsD1_configVertical(InstrumentID[0], 1, fullScale, offset, theApp.m_Scope.m_nCoupling, bandwidth);


}


void CAcqirisDigitizer::Horizontal(ADC200Data* pAdc)
{
	int	nAcqLength,nClkRate;

	pAdc->nHardwareDelay=pAdc->nDelay / 10;

	for(nClkRate=0;nClkRate<9;nClkRate++) {
		nAcqLength=(int)((float)pAdc->nWidth / m_fRate[nClkRate]);
		if(nAcqLength<8192) goto finished;
	};
	finished:
	pAdc->nAcqLength=nAcqLength;
	pAdc->fSamplePeriod=m_fRate[nClkRate];
	pAdc->nConversionRate=nClkRate;

	double sampInterval = (double)m_fRate[nClkRate] * 1e-9;	//seconds
	double delayTime = (double)pAdc->nDelay * 1e-9 + theApp.m_Scope.m_fMainBangConstant;		//seconds
	long nbrSamples = pAdc->nAcqLength;
	long nbrSegments = 1;

	// Configure timebase
	AcqrsD1_configHorizontal(InstrumentID[0], sampInterval, delayTime);
	AcqrsD1_configMemory(InstrumentID[0], nbrSamples, nbrSegments);


}

void CAcqirisDigitizer::Acquire(ADC200Data* pAdc)
{
	// ### Acquiring a waveform ###
	ViBoolean done = 0;
	long timeoutCounter = 100000; // Timeout waiting for a trigger
	long errTimeoutCounter = 500000; // Timeout for acquisition completion

	AcqrsD1_acquire(InstrumentID[0]); // Start the acquisition

	while (!done && errTimeoutCounter--)
	{
		AcqrsD1_acqDone(InstrumentID[0], &done); // Poll for the end of the acquisition
		if (!timeoutCounter--) // Trigger timeout occured
			AcqrsD1_forceTrig(InstrumentID[0]); // Force a 'manual' (or 'software') trigger
	}
	if (!done)
	{
		// Acquisition do not complete successfully
		AcqrsD1_stopAcquisition(InstrumentID[0]);
	}

}

void CAcqirisDigitizer::ReadArray(int nChannel, ADC200Data* pAdc, char *pArray, int nSlot)
{
	long segmentNumber = 0;
	long nbrSamples = pAdc->nAcqLength;
	
	AqReadParameters readPar;
	AqDataDescriptor descriptor;
	AqSegmentDescriptor segDesc;

	readPar.dataType = ReadInt8;
	readPar.readMode = ReadModeStdW;
	readPar.firstSegment = segmentNumber;
	readPar.nbrSegments = 1;
	readPar.firstSampleInSeg = (int)((float)pAdc->nDelay / pAdc->fSamplePeriod);
	readPar.nbrSamplesInSeg = pAdc->nAcqLength;
	readPar.segmentOffset = 0;
	readPar.dataArraySize = pAdc->nAcqLength + 32; // Buffer size + padding
	readPar.segDescArraySize = sizeof(AqSegmentDescriptor);

	readPar.flags		= 0;
	readPar.reserved	= 0;
	readPar.reserved2	= 0;
	readPar.reserved3	= 0;
	
	if(theApp.m_Scope.m_nAverages<=0) {
		ViStatus status = AcqrsD1_readData(InstrumentID[0], nChannel, &readPar, pArray, &descriptor, &segDesc);
		theApp.m_Scope.m_nAdcIndexFirstPoint[nSlot] = MinMax(&descriptor.indexFirstPoint,0,31);
	
	} else {
//		if(pAverageRFArrays == NULL) {
//			pAverageRFArrays = new char[8192 * 128 * 16];
//		}
	}
}

void CAcqirisDigitizer::StartThread()
{
	theApp.m_LastSettings.nUseWhichPR30 %= 8;

	if((m_bThreadEnabled==FALSE) && (m_bCardPresent==TRUE)) {
		m_bThreadEnabled=TRUE;
		switch(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]) {
		default:
		case 0: //PPA
		case 1: //MUX
			m_pThread=AfxBeginThread(&AcqirisThread,this,THREAD_PRIORITY_TIME_CRITICAL ,0,NULL);
			break;
		case 2:
			m_pThread=AfxBeginThread(&AcqirisLSAThread,this,THREAD_PRIORITY_TIME_CRITICAL ,0,NULL);
			break;
		}
	};

}

void CAcqirisDigitizer::SuspendThread()
{
	if(m_bThreadEnabled==TRUE) {
		m_bThreadEnabled=FALSE;
		while(m_bThreadFinished==FALSE) {
			Sleep(1);
		};
		WaitForSingleObject(m_pThread,INFINITE);
	};

}

UINT AcqirisThread(LPVOID pParam)
{
	CAcqirisDigitizer* pParent = (CAcqirisDigitizer *)pParam;
	int nBit;
	static int m_nSlot=0;
	char RFTrace[0x8000];
	int	ii,nTS;
	pParent->m_bThreadFinished=FALSE;
	ViStatus status;
	long timeout = 100;
	int nMainChannel,nAAChannel; //Main Signal, Aux Amp signal (No code provision below yet for this)
	bool bLogFlag;

	for(ii=1;ii>=0;ii--){
		switch(theApp.m_LastSettings.ADCInputDevice[ii]) {
		case PM30_PPA15:
		case PM30_MUX8:
		case PR2050:
			nMainChannel=ii+1;
			break;
		case PM30_AA:
			nAAChannel=ii+1;
		}
	}

	
	if((theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>8)||(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots<0))
		theApp.m_UtUser.m_Global.Mux.nMaxTimeslots=8;

	AcqrsD1_acquire(pParent->InstrumentID[0]);
	

	theApp.m_UtUser.ArmPrf();

	
	theApp.m_UtUser.m_TS[0].Up15.bIsrFlag=FALSE;

	while(pParent->m_bThreadEnabled==TRUE) {
		status = AcqrsD1_waitForEndOfAcquisition(pParent->InstrumentID[0], timeout);

		if(status == ACQIRIS_ERROR_ACQ_TIMEOUT) {
			theApp.m_UtUser.ArmPrf();
		} else {
			
			nBit = 	theApp.m_UtUser.ReadTS0();
		

			if(nBit) {
				if(m_nSlot != (theApp.m_UtUser.m_Global.Mux.nMaxTimeslots-1)) {
					pParent->m_nSlotErrorCount++;
				}
				m_nSlot = theApp.m_UtUser.m_Global.Mux.nMaxTimeslots-1;
			}
			
			switch(theApp.m_Scope.m_nPlayRecording[m_nSlot]) {
			case 0:
				pParent->ReadArray(nMainChannel, &theApp.m_UtUser.m_TS[m_nSlot].Adc, RFTrace, m_nSlot);
				theApp.m_UtUser.m_TS[m_nSlot].Pr30.nOutputSelect == 2 ? bLogFlag = true : bLogFlag = false;
				theApp.m_Scope.FillScopeArray(m_nSlot, (char*)RFTrace, theApp.m_UtUser.m_TS[m_nSlot].Adc.nAcqLength, bLogFlag, theApp.m_UtUser.m_TS[m_nSlot].Adc.nAverages);
/*
				pParent->ReadArray(nMainChannel,&theApp.m_UtUser.m_TS[m_nSlot].Adc,RFTrace,m_nSlot);
				if(theApp.m_UtUser.m_TS[m_nSlot].Pr30.nOutputSelect==2) {
					pParent->CalculateLogConversionTable();
					for(ii=0;ii<(int)theApp.m_UtUser.m_TS[m_nSlot].Adc.nAcqLength;ii++) {
						theApp.m_Scope.m_RFTrace[m_nSlot][ii]=pParent->cLogConversionTable[RFTrace[ii]+128];
					}
				} else {
					pParent->CalculateLinConversionTable();
					for(ii=0;ii<(int)theApp.m_UtUser.m_TS[m_nSlot].Adc.nAcqLength;ii++) {
						theApp.m_Scope.m_RFTrace[m_nSlot][ii]=pParent->cLinConversionTable[RFTrace[ii]+128];
					}
				}
				*/
				break;
			case 1:
			case 2:
			case 3:
				break;
			}
			

			nTS = m_nSlot + 1;
			nTS %= theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;
			if(theApp.m_UtUser.m_TS[nTS].Pr30.nOutputSelect==2) {
				pParent->Vertical(&theApp.m_UtUser.m_TS[nTS].Adc);
			} else {
				pParent->Vertical(&theApp.m_UtUser.m_TS[nTS].Adc);
			}

			pParent->m_nConversions++;
			AcqrsD1_acquire(pParent->InstrumentID[0]);

			if(m_nSlot == 0) {
				theApp.m_Thread.PrfEvent();
			}
			
			theApp.m_UtUser.ArmPrf();

			theApp.m_UtUser.m_TS[0].Up15.bIsrFlag=FALSE;
			
			theApp.m_UtUser.ProcessTrace(theApp.m_Scope.m_RFTrace[m_nSlot],m_nSlot);
			m_nSlot++;
			m_nSlot %= theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;

			if(theApp.m_PmacUser.m_nWhichDLL == PMACISAUSL) {
				if(theApp.m_bCollectComplexData==TRUE) {
					theApp.m_pScanData->CollectComplexData();
				}
			}
		}

	};
	pParent->m_bThreadFinished=TRUE;
	return 0;
}


UINT AcqirisLSAThread(LPVOID pParam)
{
	CAcqirisDigitizer* pParent = (CAcqirisDigitizer *)pParam;
	char RFTrace[8192];
	int ii,jj;
	char *pSrc;
	char *pDest;
	pParent->m_bThreadFinished=FALSE;
	ViStatus status;
	long timeout = 100;
	int nBeamChannel[4]; //4 beams each connects to an ADC input
	struct LSAFocalLaw LocalFL;

	int nActiveLaw=1,nOldLaw=1;

	AcqrsD1_acquire(pParent->InstrumentID[0]);
	theApp.m_UtUser.ArmPrf();	


	for(ii=0,jj=0;ii<2;ii++){ //max 2 inputs
		if(theApp.m_LastSettings.ADCInputDevice[ii]==PM30_LSA) {
			nBeamChannel[jj]=ii+1;
		}
	}



	while(pParent->m_bThreadEnabled==TRUE) {
		status = AcqrsD1_waitForEndOfAcquisition(pParent->InstrumentID[0], timeout);

		if(status == ACQIRIS_ERROR_ACQ_TIMEOUT) {
			AcqrsD1_acquire(pParent->InstrumentID[0]);
			theApp.m_UtUser.ArmPrf();
		} else {
			
			nOldLaw=nActiveLaw;
			nActiveLaw++;
			if(nActiveLaw>=theApp.m_LSA.nFocalLawsL) {
				nActiveLaw=0;
			}

			theApp.m_LSA.CmdSetActiveFL(nActiveLaw+1);
			theApp.m_LSA.GetSetFL(nOldLaw, &LocalFL,true);
	
			if(theApp.m_Scope.m_nPlayRecording[nActiveLaw]==0) {
				
				pParent->ReadArray(nBeamChannel[0],&theApp.m_UtUser.m_TS[0].Adc,RFTrace,nActiveLaw);
				pSrc = RFTrace;


				theApp.m_LSA.StoreAScan(nOldLaw, RFTrace, theApp.m_UtUser.m_TS[0].Adc.nAcqLength);					
				theApp.m_UtUser.ProcessTrace((char *)pSrc,nOldLaw);
					

				pDest = (char *)theApp.m_Scope.m_RFTrace[nOldLaw];
				if(theApp.m_UtUser.m_TS[0].Pr30.nOutputSelect==2) {
					for(ii=0;ii<(int)theApp.m_UtUser.m_TS[0].Adc.nAcqLength;ii++) {
						pDest[ii]=theApp.m_Scope.cLogConversionTable[pSrc[ii]];
					}
				} else {
					memcpy(pDest,pSrc,theApp.m_UtUser.m_TS[0].Adc.nAcqLength);
				}
			}

			theApp.m_UtUser.ArmPrf();	
			pParent->m_nConversions++;
			AcqrsD1_acquire(pParent->InstrumentID[0]);


			if(theApp.m_PmacUser.m_nWhichDLL == PMACISAUSL) {
				if(theApp.m_bCollectComplexData==TRUE) {
					theApp.m_pScanData->CollectComplexData();
				}
			}
		}

	};
	pParent->m_bThreadFinished=TRUE;
	return 0;
}



void CAcqirisDigitizer::CalculateLinConversionTable()
{
	int	ii,nn;

	for(ii=0;ii<256;ii++) {
		nn=(ii-128)+DC_OFFSET;
		if(nn>127) nn=127;
		if(nn<-128) nn=-128;
		cLinConversionTable[ii] = nn;
	}

}

