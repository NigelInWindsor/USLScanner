// ATS9462Digitizer.cpp: implementation of the CAlazarDigitizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "AlazarDigitizer.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SIGNED_OUTPUT 0
#define NUM_OF_CHANNELS 1
#define RECORDLENGTH 8192 //Must be divisable by 64!
#define	BUFFER_SIZE		0x10000



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlazarDigitizer::CAlazarDigitizer()
{
	U8 Major,Minor,Revision;
	bDevicesFound=FALSE;
		
	m_nNumOfChannels=2;
	
	bd.RecLength=RECORDLENGTH;	//132768
	m_bReEntrant = FALSE;

	AlazarGetSDKVersion(&Major,&Minor,&Revision);
	AlazarGetDriverVersion(&Major,&Minor,&Revision);
}

CAlazarDigitizer::~CAlazarDigitizer()
{

	SuspendThread();

	free(m_pUserData[0]);
	free(m_pUserData[1]);
	free(m_pUserData[2]);
	free(m_pUserData[3]);

}


/* returns number of devices registered on system*/
int CAlazarDigitizer::FindDevices()
{
	RETURN_CODE status=ApiSuccess;
	float fRate9626[] = {4.0f,8.0,10.0,20.0,50.0,100.0,200.0,500.0,1000.0,2000.0};
	float fRate9462[] = {5.56f,6.25f,8.0,10.0,20.0,50.0,100.0,200.0,500.0,1000.0,2000.0};
	float fRate860[] = {4.0,8.0,20.0,40.0,100.0,200.0,500.0,1000.0,2000.0,5000.0,10000.0};
	int sizePerChannel;
	int BuffersPerRecord=1;


	int ATS9626SampleRates[] = {SAMPLE_RATE_250MSPS,SAMPLE_RATE_125MSPS,SAMPLE_RATE_100MSPS,
								SAMPLE_RATE_50MSPS,SAMPLE_RATE_20MSPS,SAMPLE_RATE_10MSPS,SAMPLE_RATE_5MSPS,
								SAMPLE_RATE_2MSPS,SAMPLE_RATE_1MSPS,SAMPLE_RATE_500KSPS};

	int ATS9462SampleRates[] = {SAMPLE_RATE_180MSPS, SAMPLE_RATE_160MSPS, SAMPLE_RATE_125MSPS, SAMPLE_RATE_100MSPS,
								SAMPLE_RATE_50MSPS, SAMPLE_RATE_20MSPS, SAMPLE_RATE_10MSPS, SAMPLE_RATE_5MSPS,
								SAMPLE_RATE_2MSPS, SAMPLE_RATE_1MSPS, SAMPLE_RATE_500KSPS};

	int ATS860SampleRates[] = {SAMPLE_RATE_250MSPS,SAMPLE_RATE_125MSPS,SAMPLE_RATE_50MSPS,SAMPLE_RATE_25MSPS,
								SAMPLE_RATE_10MSPS,SAMPLE_RATE_5MSPS,SAMPLE_RATE_2MSPS,SAMPLE_RATE_1MSPS,
								SAMPLE_RATE_500KSPS,SAMPLE_RATE_200KSPS,SAMPLE_RATE_100KSPS};

	if(theApp.m_Tank.nMaxADCConversionRate < 10000000) theApp.m_Tank.nMaxADCConversionRate = 10000000;

	U32 systemCount = AlazarNumOfSystems();
	m_nBoards = AlazarBoardsInSystemBySystemID(1);

	if(m_nBoards==0) {
		theApp.m_LastSettings.nAdcCardType=Acqiris_DP210;
		return 0;
	}

	for(int i=0;i<m_nBoards;i++) {
		InstrumentID[i] = AlazarGetBoardBySystemID(1, 1+i);
	}

	theApp.m_nNumADCs = m_nBoards;

	m_Model = (ALAZAR_BOARDTYPES) AlazarGetBoardKind (InstrumentID[0]);

	
	switch(m_Model){
	case ATS9626:	theApp.m_LastSettings.nAdcCardType=Alazar_9626;
		CopyMemory(m_fSamplePeriod,fRate9626,sizeof fRate9626);
		CopyMemory(m_nSamplePeriodCodes,ATS9626SampleRates,sizeof ATS9626SampleRates);
		sizePerChannel = BuffersPerRecord*bd.RecLength*2; //*2 because 16bit resolution
		m_nNumOfChannels = 2;
		break;
	case ATS9462:	theApp.m_LastSettings.nAdcCardType=Alazar_9462;
		CopyMemory(m_fSamplePeriod,fRate9462,sizeof fRate9462);
		CopyMemory(m_nSamplePeriodCodes,ATS9462SampleRates,sizeof ATS9462SampleRates);
		sizePerChannel = BuffersPerRecord*bd.RecLength*2; //*2 because 16bit resolution
		m_nNumOfChannels = 1;
		break;
	case ATS860:	theApp.m_LastSettings.nAdcCardType=Alazar_860;
		CopyMemory(m_fSamplePeriod,fRate860,sizeof fRate860);
		CopyMemory(m_nSamplePeriodCodes,ATS860SampleRates,sizeof ATS860SampleRates);
		sizePerChannel = BuffersPerRecord*bd.RecLength;
		theApp.m_Scope.m_nBits = 8;
		break;
	default:
		if( (theApp.m_LastSettings.nAdcCardType==Alazar_9462) || (theApp.m_LastSettings.nAdcCardType==Alazar_860) || (theApp.m_LastSettings.nAdcCardType==Alazar_9626)){
			theApp.m_LastSettings.nAdcCardType=USL_ADC200;
		}
		return 0;
		break;
	}

	m_pUserData[0] = (U8 *) malloc((sizePerChannel * 2) * m_nNumOfChannels + 1024);
	m_pUserData[1] = (U8 *) malloc((sizePerChannel * 2) * m_nNumOfChannels + 1024);
	m_pUserData[2] = (U8 *) malloc((sizePerChannel * 2) * m_nNumOfChannels + 1024);
	m_pUserData[3] = (U8 *) malloc((sizePerChannel * 2) * m_nNumOfChannels + 1024);

	m_nBytesPerBuffer = sizePerChannel * m_nNumOfChannels;

	bDevicesFound=TRUE;
	m_bDelayWidthChanged=FALSE;

	return m_nBoards;

}

void CAlazarDigitizer::Configure(ADC200Data* pAdc)
{
	for(int ii=0; ii<m_nBoards; ii++){
		Configure(pAdc,ii);
	}

}




/* configure Device*/
void CAlazarDigitizer::Configure(ADC200Data* pAdc, int BoardNumber)
{
	RETURN_CODE status=ApiSuccess;
	bool bRestart=false;
	int	nClkRate,nSampleRate;
	U32 value;


	theApp.m_Scope.m_nEncodeing = UNSIGNED_BINARY;

	if(m_bThreadEnabled) {
		bRestart=true;
		SuspendThread();
	}


	pAdc->nHardwareDelay=pAdc->nDelay / 10;

	for(nClkRate=0;nClkRate<11;nClkRate++) {
		m_nAcqLength=(int)((float)pAdc->nWidth / m_fSamplePeriod[nClkRate]);
		if(m_nAcqLength<RECORDLENGTH) goto finished;
	};
	finished:

	nSampleRate = m_nSamplePeriodCodes[nClkRate];
	pAdc->nAcqLength=m_nAcqLength;
	pAdc->fSamplePeriod=m_fSamplePeriod[nClkRate];
	pAdc->nConversionRate=nClkRate;

	m_nDelay = (int)((float)pAdc->nDelay/m_fSamplePeriod[nClkRate]);

	if (m_nDelay==0)
		m_nDelay++;


	if(!bDevicesFound)
		return;

	bd.ClockSource		= INTERNAL_CLOCK;
	bd.ClockEdge		= CLOCK_EDGE_RISING;
	bd.SampleRate		= nSampleRate;

	switch(theApp.m_Scope.m_nCoupling) {
	case 0:
		break;
	case 1:
		bd.CouplingChanA	= DC_COUPLING;
		bd.InputImpedChanA	= IMPEDANCE_1M_OHM;
		break;
	case 2:
		bd.CouplingChanA	= AC_COUPLING;
		bd.InputImpedChanA	= IMPEDANCE_1M_OHM;
		break;
	case 3:
		bd.CouplingChanA	= DC_COUPLING;
		bd.InputImpedChanA	= IMPEDANCE_50_OHM;
		break;
	case 4:
		bd.CouplingChanA	= AC_COUPLING;
		bd.InputImpedChanA	= IMPEDANCE_50_OHM;
		break;
	}


	switch(m_Model){
	default:
		switch(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]) {
		default:
			bd.InputRangeChanA	= INPUT_RANGE_PM_4_V;
			if (theApp.m_Scope.m_fVoltageRange == 4.0f) {
				bd.InputRangeChanA = INPUT_RANGE_PM_4_V;
			} else {
				if (theApp.m_Scope.m_fVoltageRange == 2.0f) {
					bd.InputRangeChanA = INPUT_RANGE_PM_2_V;
				} else {
					if (theApp.m_Scope.m_fVoltageRange == 1.0f) {
						bd.InputRangeChanA = INPUT_RANGE_PM_1_V;
					} else {
						MessageBox(NULL,L"Invalid voltage range selected\ndefault value of 4V used",L"Error",MB_ICONERROR);
					}
				}
			}
			break;
		case LSA:
			bd.InputRangeChanA	= INPUT_RANGE_PM_1_V;
		break;
		}
		break;
	case ATS9626:
		bd.InputRangeChanA	= 33;		//INPUT_RANGE_1_V_25;
		bd.InputImpedChanA	= IMPEDANCE_50_OHM;
		break;
	}


	if(theApp.m_Tank.fADCTriggerLevel <= 0.0f) theApp.m_Tank.fADCTriggerLevel = 1500.0f;
	MinMax(&theApp.m_Tank.fADCTriggerLevel,0.0f,5000.0f);
	bd.TriEngOperation	= TRIG_ENGINE_OP_J;
    bd.TriggerEngine1	= TRIG_ENGINE_J;
    bd.TrigEngSource1	= TRIG_EXTERNAL;
    bd.TrigEngSlope1	= TRIGGER_SLOPE_POSITIVE;
    bd.TrigEngLevel1	= 128 + (int)((theApp.m_Tank.fADCTriggerLevel * 127.0f) / 5000.0f);  //145
    bd.TriggerEngine2	= TRIG_ENGINE_K;
    bd.TrigEngSource2	= TRIG_DISABLE;
    bd.TrigEngSlope2	= TRIGGER_SLOPE_POSITIVE;
    bd.TrigEngLevel2	= bd.TrigEngLevel1;

	status=AlazarAbortCapture(InstrumentID[BoardNumber]);
	status = AlazarAbortAsyncRead(InstrumentID[BoardNumber]);
	status=AlazarSetCaptureClock(InstrumentID[BoardNumber],bd.ClockSource,bd.SampleRate,bd.ClockEdge,0);

	status=AlazarInputControl(InstrumentID[BoardNumber],CHANNEL_A,bd.CouplingChanA,bd.InputRangeChanA,bd.InputImpedChanA);
	status=AlazarInputControl(InstrumentID[BoardNumber],CHANNEL_B,bd.CouplingChanA,bd.InputRangeChanA,bd.InputImpedChanA);

	status=AlazarSetTriggerOperation(InstrumentID[BoardNumber],bd.TriEngOperation
		,bd.TriggerEngine1,bd.TrigEngSource1,bd.TrigEngSlope1,bd.TrigEngLevel1
		,bd.TriggerEngine2,bd.TrigEngSource2,bd.TrigEngSlope2,bd.TrigEngLevel2);


	status=AlazarSetExternalTrigger(InstrumentID[BoardNumber],DC_COUPLING,ETR_5V);

	status=AlazarSetRecordSize(InstrumentID[BoardNumber],0,bd.RecLength);

	status=AlazarSetTriggerTimeOut(InstrumentID[BoardNumber],2000);

	status=AlazarSetTriggerDelay(InstrumentID[BoardNumber],m_nDelay=1);

	theApp.m_UtUser.SetMaxPRF(m_fSamplePeriod[nClkRate],m_nDelay);

	//Set to Signed output
	status = AlazarReadRegister(InstrumentID[BoardNumber], 13, &value, 0x32145876);
	value = value | 0x80000000;
	status = AlazarWriteRegister(InstrumentID[BoardNumber], 13, value, 0x32145876);

	if(bRestart==true ) {
		StartThread();
	}

}

void CAlazarDigitizer::ConfigureWidthDelay(ADC200Data* pAdc)
{
	for(int ii=m_nBoards;ii>0;ii--)
	{
		ConfigureWidthDelay(pAdc,ii-1);
	}
}

void CAlazarDigitizer::ConfigureWidthDelay(ADC200Data* pAdc, int BoardNumber)
{
	bool bRestart=false;

	if((theApp.m_LastSettings.nAdcCardType!=Alazar_9462) && (theApp.m_LastSettings.nAdcCardType!=Alazar_860) && (theApp.m_LastSettings.nAdcCardType!=Alazar_9626))
		return;

	RETURN_CODE status=ApiSuccess;
	int	nMinDelay,nClkRate,nSampleRate,nMaxAcqLength,nMaxClkRate,nAcqLength,nTS;
	float fWidth;


	nMaxClkRate = 0;
	nMinDelay = 1000000;
	for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		if(nMinDelay > theApp.m_UtUser.m_TS[nTS].Adc.nDelay) nMinDelay = theApp.m_UtUser.m_TS[nTS].Adc.nDelay;
	}


	for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		fWidth = (float)((theApp.m_UtUser.m_TS[nTS].Adc.nDelay - nMinDelay) + theApp.m_UtUser.m_TS[nTS].Adc.nWidth);
		for(nClkRate=0;nClkRate<11;nClkRate++) {
			nAcqLength=(int)(fWidth / m_fSamplePeriod[nClkRate]);
			if(nAcqLength<RECORDLENGTH) goto finished;
		};
		finished:
		if(nClkRate > nMaxClkRate) nMaxClkRate = nClkRate;
	}

	nMaxAcqLength=0;
	for(nTS=0;nTS<256;nTS++) {
		theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod=m_fSamplePeriod[nMaxClkRate];
		theApp.m_UtUser.m_TS[nTS].Adc.nConversionRate=nMaxClkRate;
		theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength=(int)((float)theApp.m_UtUser.m_TS[nTS].Adc.nWidth / m_fSamplePeriod[nMaxClkRate]);
		nAcqLength = theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength + (int)((float)theApp.m_UtUser.m_TS[nTS].Adc.nDelay / m_fSamplePeriod[nMaxClkRate]);
		if(nAcqLength > nMaxAcqLength) nMaxAcqLength = nAcqLength;
		if(((float)theApp.m_UtUser.m_TS[nTS].Adc.nDelay-(float)nMinDelay)<0) {
			theApp.m_UtUser.m_TS[nTS].Adc.nHardwareDelay=0;
		}else{
			theApp.m_UtUser.m_TS[nTS].Adc.nHardwareDelay=(int)((float)(theApp.m_UtUser.m_TS[nTS].Adc.nDelay-nMinDelay) / m_fSamplePeriod[nMaxClkRate]);
		}
	}


	nSampleRate = m_nSamplePeriodCodes[nMaxClkRate];


	m_nDelay = (int)((float)pAdc->nDelay/m_fSamplePeriod[nMaxClkRate]);

	bd.ClockSource		= INTERNAL_CLOCK;
	bd.ClockEdge		= CLOCK_EDGE_RISING;
    bd.TrigEngLevel1	= 128 + (((int)theApp.m_Tank.fADCTriggerLevel * 127) / 5000);
	bd.TrigEngLevel2	= bd.TrigEngLevel1;


	if(nSampleRate!=m_nBoardSampleRate[BoardNumber]){
		m_nBoardSampleRate[BoardNumber] = nSampleRate;
		bd.SampleRate=nSampleRate;
		m_bReEntrant=TRUE;
		Sleep(100);
		status=AlazarSetCaptureClock(InstrumentID[BoardNumber],bd.ClockSource,bd.SampleRate,bd.ClockEdge,0);
		status=AlazarSetCaptureClock(InstrumentID[BoardNumber],bd.ClockSource,bd.SampleRate,bd.ClockEdge,0);
		m_bReEntrant=FALSE;
	}


	int nTemp;

	nMinDelay += (int)(theApp.m_Scope.m_fMainBangConstant / 1e-9);
	if(nMinDelay==0) nMinDelay=1;
	//This seems to have been changed in the newest alazar driver!!!!! this may fuck us up, will have to check driver revision
//	if(m_fSamplePeriod[nMaxClkRate]==4){ //250MSPS
//		nTemp = nMinDelay/8;
//		if(nTemp==0) nTemp=1;
//		status=AlazarSetTriggerDelay(InstrumentID[BoardNumber],nTemp);		//At 250MSPS the alazar delay clock is using 125MHz by design apprently
//	}else{
		nTemp = (int)((float)nMinDelay/m_fSamplePeriod[nMaxClkRate]);
		if(nTemp==0) nTemp=1;
		status=AlazarSetTriggerDelay(InstrumentID[BoardNumber],nTemp);
//	}


	theApp.m_UtUser.SetMaxPRF(m_fSamplePeriod[nMaxClkRate],nMinDelay);



}




void CAlazarDigitizer::StartThread()
{
	if(m_bThreadEnabled==FALSE) {
		m_bThreadEnabled=TRUE;
		switch(m_Model){
		case ATS9626:
			m_pThread=AfxBeginThread(&ATS9626Thread,this,THREAD_PRIORITY_TIME_CRITICAL ,0,NULL);
			break;
		case ATS9462:
			m_pThread=AfxBeginThread(&ATS9462Thread,this,THREAD_PRIORITY_TIME_CRITICAL ,0,NULL);
			break;
		case ATS860:
			switch(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]) {
			default:
			case 0: //PPA
			case 1: //MUX
				m_pThread=AfxBeginThread(&ATS860Thread,this,THREAD_PRIORITY_TIME_CRITICAL ,0,NULL);
				break;
			case 2: //LSA
				m_pThread=AfxBeginThread(&ATS860LSAThread,this,THREAD_PRIORITY_TIME_CRITICAL ,0,NULL);
			break;
			}//LSA? Switch
		}//Model? switch
	};

}

void CAlazarDigitizer::SuspendThread()
{
	if(m_bThreadEnabled==TRUE) {
		m_bThreadEnabled=FALSE;
		while(m_bThreadFinished==FALSE) {
			Sleep(1);
		};
		WaitForSingleObject(m_pThread,INFINITE);
	};

}


UINT ATS860Thread(LPVOID pParam)
{
	CAlazarDigitizer* pParent = (CAlazarDigitizer *)pParam;
	pParent->m_bThreadFinished=FALSE;
	RETURN_CODE status=ApiSuccess;
	U8 *pBuffer;
	static int nSlot,nPrSlot;
	int ii;
	int nSampleRate;
	unsigned char *pSrc = NULL, *pAASrc = NULL, *pDest = NULL;
	int nActiveLaw=1,nOldLaw=1, nOlderLaw=1;
	int BuffersPosted, BuffersCompleted, TransfersCompleted, BufferIndex;
	bool bLogFlag;


	U32	CFLAGS = 0;

	CFLAGS = ADMA_NPT | ADMA_EXTERNAL_STARTCAPTURE ;
	

//If repost error go to here
restart:


	nSampleRate=pParent->m_nBoardSampleRate[1];

	if((theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>8)||(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots<0))
		theApp.m_UtUser.m_Global.Mux.nMaxTimeslots=8;

	pParent->m_bReEntrant=TRUE;
	if(pParent->m_nNumOfChannels==1)
		status = AlazarBeforeAsyncRead(pParent->InstrumentID[0], CHANNEL_A, 0, pParent->bd.RecLength, 1, 0x7FFFFFFF, CFLAGS);
	else if(pParent->m_nNumOfChannels==2)
		status = AlazarBeforeAsyncRead(pParent->InstrumentID[0], CHANNEL_A | CHANNEL_B, 0, pParent->bd.RecLength, 1, 0x7FFFFFFF, CFLAGS);


	status = AlazarPostAsyncBuffer(pParent->InstrumentID[0], pParent->m_pUserData[0], pParent->m_nBytesPerBuffer);
	status = AlazarPostAsyncBuffer(pParent->InstrumentID[0], pParent->m_pUserData[1], pParent->m_nBytesPerBuffer);
	pParent->m_bReEntrant=FALSE;

	BuffersPosted = 2;
	BuffersCompleted = 0;
	TransfersCompleted = 0;
	BufferIndex = 0;
	nSlot=0;

	status = AlazarStartCapture(pParent->InstrumentID[0]);

	theApp.m_UtUser.ArmPrf();

	theApp.m_Scope.CalculateLogConversionTable();

	while(pParent->m_bThreadEnabled==TRUE) {
		
		if(pParent->m_bReEntrant==FALSE) {

			status = ApiSuccess;
			BufferIndex = BuffersCompleted % BuffersPosted;
			pBuffer = pParent->m_pUserData[BufferIndex];

			theApp.m_UtUser.ArmPrf();

			status = AlazarWaitAsyncBufferComplete(pParent->InstrumentID[0], pBuffer, 1000);


			if(status == ApiSuccess) {
				if(	theApp.m_LastSettings.nDesiredDevice==NOT_PHASED_ARRAY) {
					if(theApp.m_Si10User.IsConnected() == TRUE) {
						if(theApp.m_Si10User.Si10ReadTS0()) {
							if(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>1) {
								nSlot = theApp.m_UtUser.m_Global.Mux.nMaxTimeslots-2;
							} else {
								nSlot = 0;
							}
						}
					} else {
						if(theApp.m_UtUser.Inp(theApp.m_UtUser.MailboxAddrs+2) & 1) {
							nSlot = theApp.m_UtUser.ReadTS0()-1;
						}
					}
					nPrSlot = nSlot;
				} else {
					nSlot = nActiveLaw - 2;
					if(nSlot<0) nSlot += theApp.m_LSA.nFocalLawsL;
					nActiveLaw++;
					if(nActiveLaw>=theApp.m_LSA.nFocalLawsL) {
						nActiveLaw=0;
					}

					theApp.m_LSA.CmdSetActiveFL(nActiveLaw);
					nPrSlot = 0;
				}

				for(ii=0;ii<2;ii++){ //max 2 inputs
					switch(theApp.m_LastSettings.ADCInputDevice[ii]) {
					case UNUSED:
						break;
					default:
						switch(ii){
						case 0:
							pSrc = pBuffer+theApp.m_UtUser.m_TS[nPrSlot].Adc.nHardwareDelay;
							break;
						case 1:
							pSrc = pBuffer+theApp.m_UtUser.m_TS[nPrSlot].Adc.nHardwareDelay+RECORDLENGTH;
							break;
						}
						break;
					case PM30_AA:
						switch(ii){
						case 0:
							pAASrc = pBuffer+theApp.m_UtUser.m_TS[nPrSlot].Adc.nHardwareDelay;
							break;
						case 1:
							pAASrc = pBuffer+theApp.m_UtUser.m_TS[nPrSlot].Adc.nHardwareDelay+RECORDLENGTH;
							break;
						}
					}
				}
				if(theApp.m_LSA.IsConnected() && theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
					theApp.m_LSA.StoreAScan(nSlot, pSrc, theApp.m_UtUser.m_TS[nPrSlot].Adc.nAcqLength);
				}


				if(theApp.m_Scope.m_nPlayRecording[nPrSlot]==0) {
					theApp.m_UtUser.m_TS[nPrSlot].Pr30.nOutputSelect == 2 ? bLogFlag = true : bLogFlag = false;
					theApp.m_Scope.FillScopeArray(nPrSlot, (char*)pSrc, theApp.m_UtUser.m_TS[nPrSlot].Adc.nAcqLength, bLogFlag, theApp.m_UtUser.m_TS[nPrSlot].Adc.nAverages);
				}
				theApp.m_UtUser.m_TS[nSlot].nIrqCount++;

				BuffersCompleted++;
				theApp.m_UtUser.ProcessTrace(theApp.m_Scope.m_RFTrace[nSlot],nSlot);
				nSlot++;
				nSlot %= theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;

				status = AlazarPostAsyncBuffer(pParent->InstrumentID[0],  pParent->m_pUserData[BufferIndex], pParent->m_nBytesPerBuffer);
				if(status != ApiSuccess) 
				{
					//ApiBufferOverflow? PRF may be too high see note above!
					status = AlazarAbortAsyncRead(pParent->InstrumentID[0]);
					goto restart;
				}

				theApp.m_Acqiris.m_nConversions++;

			} else {

				status = AlazarAbortAsyncRead(pParent->InstrumentID[0]);
				goto restart;
			}
		}

	}

	AlazarAbortAsyncRead(pParent->InstrumentID[0]);
	pParent->m_bThreadFinished=TRUE;

	return 0;
}


UINT ATS860LSAThread(LPVOID pParam)
{
	CAlazarDigitizer* pParent = (CAlazarDigitizer *)pParam;
	pParent->m_bThreadFinished=FALSE;
	RETURN_CODE status=ApiSuccess;
	U8 *pBuffer = NULL, *pBuffer2 = NULL;
	int ii,jj,nRFChannel,nOffset;
	int nSampleRate;
	unsigned char *pSrc[4];
	unsigned char *pDest;
	int nActiveLaw=1,nOldLaw=1, nOlderLaw=1;
	int BuffersPosted, BuffersCompleted, TransfersCompleted, BufferIndex;

	U32	CFLAGS = 0;

	CFLAGS = ADMA_NPT | ADMA_EXTERNAL_STARTCAPTURE;	

//If repost error go to here
restart:
	nSampleRate=pParent->m_nBoardSampleRate[1];

	pParent->m_bReEntrant=TRUE;
	if(pParent->m_nNumOfChannels==1) {
		status = AlazarBeforeAsyncRead(pParent->InstrumentID[0], CHANNEL_A, 0, pParent->bd.RecLength, 1, 0x7FFFFFFF, CFLAGS);
		if(pParent->m_nBoards>1) status = AlazarBeforeAsyncRead(pParent->InstrumentID[1], CHANNEL_A, 0, pParent->bd.RecLength, 1, 0x7FFFFFFF, CFLAGS);
	}else if(pParent->m_nNumOfChannels==2){
		status = AlazarBeforeAsyncRead(pParent->InstrumentID[0], CHANNEL_A | CHANNEL_B, 0, pParent->bd.RecLength, 1, 0x7FFFFFFF, CFLAGS);
		if(pParent->m_nBoards>1) status = AlazarBeforeAsyncRead(pParent->InstrumentID[1], CHANNEL_A | CHANNEL_B, 0, pParent->bd.RecLength, 1, 0x7FFFFFFF, CFLAGS);
	}


	status = AlazarPostAsyncBuffer(pParent->InstrumentID[0], pParent->m_pUserData[0], pParent->m_nBytesPerBuffer);
	status = AlazarPostAsyncBuffer(pParent->InstrumentID[0], pParent->m_pUserData[1], pParent->m_nBytesPerBuffer);
	if(pParent->m_nBoards>1){
		status = AlazarPostAsyncBuffer(pParent->InstrumentID[1], pParent->m_pUserData[2], pParent->m_nBytesPerBuffer);
		status = AlazarPostAsyncBuffer(pParent->InstrumentID[1], pParent->m_pUserData[3], pParent->m_nBytesPerBuffer);
	}

	pParent->m_bReEntrant=FALSE;

	BuffersPosted = 2;
	BuffersCompleted = 0;
	TransfersCompleted = 0;
	BufferIndex = 0;

	status = AlazarStartCapture(pParent->InstrumentID[0]);
	if(pParent->m_nBoards>1) status = AlazarStartCapture(pParent->InstrumentID[1]);

	theApp.m_UtUser.ArmPrf();

	pParent->CalculateLogConversionTable();

	switch(theApp.m_UtUser.m_nPM30Count) {
		default:
			return 0;
		case 1:
			nOffset=0; //Shouldn't be needed
		break;
		case 2:
			nOffset=64;
		break;
		case 4:
			nOffset=32;
		break;
	}

	while(pParent->m_bThreadEnabled==TRUE) {
		
		if(pParent->m_bReEntrant==FALSE) {

			status = ApiSuccess;
			BufferIndex = BuffersCompleted % BuffersPosted;
			pBuffer = pParent->m_pUserData[BufferIndex];
			
			theApp.m_UtUser.ArmPrf();

			status = AlazarWaitAsyncBufferComplete(pParent->InstrumentID[0], pBuffer, 1000);
			if (pParent->m_nBoards > 1) {
				pBuffer2 = pParent->m_pUserData[BufferIndex + 2];
				status = AlazarWaitAsyncBufferComplete(pParent->InstrumentID[1], pBuffer2, 20);
			}
	
			if(status == ApiSuccess) {
		
				nOlderLaw = nOldLaw;
				nOldLaw=nActiveLaw;
				nActiveLaw++;
				if(nActiveLaw>=theApp.m_LSA.m_nActualLawsL)
					nActiveLaw=0;

				theApp.m_LSA.CmdSetActiveFL(nActiveLaw+1); //+1

				pSrc[0]=pBuffer;
				for(ii=0,jj=0;ii<4;ii++){ //max 4 inputs
					if(theApp.m_LastSettings.ADCInputDevice[ii]==PM30_LSA) {
						switch(ii){
						case 0:
							pSrc[jj]=pBuffer;
							break;
						case 1:
							pSrc[jj]=pBuffer+RECORDLENGTH;
							break;
						case 2:
							if(pBuffer2) pSrc[jj]=pBuffer2;
							break;
						case 3:
							if(pBuffer2) pSrc[jj]=pBuffer2+RECORDLENGTH;
							break;
						}//switch
						jj++;
					}
				}		

				if(theApp.m_Scope.m_nPlayRecording[nActiveLaw]==0) {
			
					nRFChannel = 0;
					int nLaw = nActiveLaw - 2;
					if(nLaw<0) nLaw += theApp.m_LSA.nFocalLawsL;
					if(nLaw<0) nLaw = 0;

					if(nLaw>=0) {
						theApp.m_LSA.StoreAScan(nLaw, pSrc[nRFChannel], theApp.m_UtUser.m_TS[0].Adc.nAcqLength);
						theApp.m_UtUser.ProcessTrace((char *)pSrc[nRFChannel],nLaw);
					
						pDest = (unsigned char *)theApp.m_Scope.m_RFTrace[nLaw];

						if(theApp.m_UtUser.m_TS[0].Pr30.nOutputSelect==2) {
							pParent->CalculateLogConversionTable();
							for(unsigned int nS =0; nS<theApp.m_UtUser.m_TS[0].Adc.nAcqLength; nS++) {
								pDest[nS]=pParent->cLogConversionTable[(char)pSrc[nRFChannel][nS]+128];
							}
						} else {
							memcpy(pDest,pSrc[nRFChannel],theApp.m_UtUser.m_TS[0].Adc.nAcqLength);
						}
					}
				}

		 		BuffersCompleted++;
				
				status = AlazarPostAsyncBuffer(pParent->InstrumentID[0],  pParent->m_pUserData[BufferIndex], pParent->m_nBytesPerBuffer);
				if(pParent->m_nBoards>1) status = AlazarPostAsyncBuffer(pParent->InstrumentID[1],  pParent->m_pUserData[BufferIndex+2], pParent->m_nBytesPerBuffer);

				if(status != ApiSuccess) 
				{
					//ApiBufferOverflow? PRF may be too high see note above!
					status = AlazarAbortAsyncRead(pParent->InstrumentID[0]);
					if(pParent->m_nBoards>1) status = AlazarAbortAsyncRead(pParent->InstrumentID[1]);
					goto restart;
				}

				theApp.m_Acqiris.m_nConversions++;

			} else {

				status = AlazarAbortAsyncRead(pParent->InstrumentID[0]);
				theApp.m_UtUser.ArmPrf();
				if(pParent->m_nBoards>1) status = AlazarAbortAsyncRead(pParent->InstrumentID[1]);
				Sleep(1000);
				goto restart;
			}
		}

	}

	AlazarAbortAsyncRead(pParent->InstrumentID[0]);
	if(pParent->m_nBoards>1) AlazarAbortAsyncRead(pParent->InstrumentID[1]);

	
	pParent->m_bThreadFinished=TRUE;

	return 0;
}


UINT ATS9462Thread(LPVOID pParam)
{
	CAlazarDigitizer* pParent = (CAlazarDigitizer *)pParam;
	pParent->m_bThreadFinished=FALSE;
	RETURN_CODE status=ApiSuccess;
	U8 *pBuffer;
	int nBit;
	WORD wValue;
	unsigned char cValue;
	unsigned __int16 uwValue;
	unsigned __int16 *pSrc,*pwDest;
	unsigned char *pDest;
	static int nSlot;
	int BuffersPosted, BuffersCompleted, TransfersCompleted, BufferIndex;
	U32 nTriggerAddrs,nTimeStampUpper,nTimeStampLower;

	U32	CFLAGS = 0;

	CFLAGS = ADMA_NPT | ADMA_FIFO_ONLY_STREAMING | ADMA_EXTERNAL_STARTCAPTURE ;

	pParent->CalculateLogConversionTable();


//If repost error go to here
restart:

	if(pParent->m_nNumOfChannels==1)
		status = AlazarBeforeAsyncRead(pParent->InstrumentID[0], CHANNEL_A, 0, pParent->bd.RecLength, 1, 0x7FFFFFFF, CFLAGS);
	else if(pParent->m_nNumOfChannels==2)
		status = AlazarBeforeAsyncRead(pParent->InstrumentID[0], CHANNEL_A | CHANNEL_B, 0, pParent->bd.RecLength, 1, 0x7FFFFFFF, CFLAGS);

	status = AlazarPostAsyncBuffer(pParent->InstrumentID[0], pParent->m_pUserData[0], pParent->m_nBytesPerBuffer);
	status = AlazarPostAsyncBuffer(pParent->InstrumentID[0], pParent->m_pUserData[1], pParent->m_nBytesPerBuffer);

	BuffersPosted = 2;
	BuffersCompleted = 0;
	TransfersCompleted = 0;
	BufferIndex = 0;
	nSlot=0;

	status = AlazarStartCapture(pParent->InstrumentID[0]);

	theApp.m_Si10User.Si10ArmPRF();
	Sleep(1);

	while(pParent->m_bThreadEnabled==TRUE) {

		status = ApiSuccess;
		BufferIndex = BuffersCompleted % BuffersPosted;
		pBuffer = pParent->m_pUserData[BufferIndex];

		theApp.m_Si10User.Si10ArmPRF();
	
		status = AlazarWaitAsyncBufferComplete(pParent->InstrumentID[0], pBuffer, 100);

		if(status == ApiSuccess) {
			nBit = theApp.m_UtUser.Inp(theApp.m_UtUser.MailboxAddrs+2) & 1;
			if(nBit) {
				nSlot = theApp.m_UtUser.m_Global.Mux.nMaxTimeslots-1;
			}

			status = AlazarGetTriggerAddress (pParent->InstrumentID[0],0,&nTriggerAddrs,&nTimeStampUpper,&nTimeStampLower);

			if(theApp.m_Scope.m_nPlayRecording[nSlot]==0) {
				pSrc = (unsigned __int16*)pBuffer+theApp.m_UtUser.m_TS[nSlot].Adc.nHardwareDelay;
				pDest = (unsigned char *)theApp.m_Scope.m_RFTrace[nSlot];
				pwDest = (unsigned __int16*)theApp.m_Scope.m_wRFTrace[nSlot];

				if(theApp.m_UtUser.m_TS[nSlot].Pr30.nOutputSelect==2) {
					for(unsigned int nS=0; nS<theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength; nS++) {
						uwValue = pSrc[nS] + 0x8000;
						cValue = (unsigned char)(uwValue >> 8);
						pDest[nS]=pParent->cLogConversionTable[cValue];

						pwDest[nS]=pParent->wLogConversionTable[uwValue];
					}
				} else {
					for(unsigned int nS = 1; nS<theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength; nS++) {
						wValue = pSrc[nS];
						pDest[nS] = wValue >> 8;
					}
					CopyMemory(pwDest, pSrc, theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength * sizeof (__int16));
				}
			}


			BuffersCompleted++;
			theApp.m_UtUser.ProcessTrace(theApp.m_Scope.m_RFTrace[nSlot],nSlot);
			nSlot++;
			nSlot %= theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;

			status = AlazarPostAsyncBuffer(pParent->InstrumentID[0],  pParent->m_pUserData[BufferIndex], pParent->m_nBytesPerBuffer);
			if(status != ApiSuccess) 
			{
				AlazarForceTrigger (pParent->InstrumentID[0]);
		//		Sleep(1);
				status = AlazarAbortAsyncRead(pParent->InstrumentID[0]);
				goto restart;
			}

			theApp.m_Acqiris.m_nConversions++;

		} else {

			AlazarForceTrigger (pParent->InstrumentID[0]);
			Sleep(1);
			status = AlazarAbortAsyncRead(pParent->InstrumentID[0]);
			goto restart;
		}

	}

	AlazarForceTrigger (pParent->InstrumentID[0]);
	Sleep(2);
	AlazarAbortAsyncRead(pParent->InstrumentID[0]);
	pParent->m_bThreadFinished=TRUE;

	return 0;

}


UINT ATS9626Thread(LPVOID pParam)
{
	CAlazarDigitizer* pParent = (CAlazarDigitizer *)pParam;
	pParent->m_bThreadFinished=FALSE;
	RETURN_CODE status=ApiSuccess;
	U8 *pBuffer;
	static int nSlot;
	int nSampleRate;
	int nActiveLaw=1,nOldLaw=1;
	int BuffersPosted, BuffersCompleted, TransfersCompleted, BufferIndex;

	WORD wValue;
	unsigned char cValue;
	unsigned __int16 uwValue;
	unsigned __int16 *pSrc,*pwDest;
	unsigned char *pDest;


	U32	CFLAGS = 0;

	CFLAGS = ADMA_NPT | ADMA_EXTERNAL_STARTCAPTURE ;
	

//If repost error go to here
restart:


	nSampleRate=pParent->m_nBoardSampleRate[1];

	if((theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>8)||(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots<0))
		theApp.m_UtUser.m_Global.Mux.nMaxTimeslots=8;

	pParent->m_bReEntrant=TRUE;
	if(pParent->m_nNumOfChannels==1)
		status = AlazarBeforeAsyncRead(pParent->InstrumentID[0], CHANNEL_A, 0, pParent->bd.RecLength, 1, 0x7FFFFFFF, CFLAGS);
	else if(pParent->m_nNumOfChannels==2)
		status = AlazarBeforeAsyncRead(pParent->InstrumentID[0], CHANNEL_A | CHANNEL_B, 0, pParent->bd.RecLength, 1, 0x7FFFFFFF, CFLAGS);


	status = AlazarPostAsyncBuffer(pParent->InstrumentID[0], pParent->m_pUserData[0], pParent->m_nBytesPerBuffer);
	status = AlazarPostAsyncBuffer(pParent->InstrumentID[0], pParent->m_pUserData[1], pParent->m_nBytesPerBuffer);
	pParent->m_bReEntrant=FALSE;

	BuffersPosted = 2;
	BuffersCompleted = 0;
	TransfersCompleted = 0;
	BufferIndex = 0;
	nSlot=0;

	status = AlazarStartCapture(pParent->InstrumentID[0]);

	theApp.m_UtUser.ArmPrf();

	pParent->CalculateLogConversionTable();

	while(pParent->m_bThreadEnabled==TRUE) {
		
		if(pParent->m_bReEntrant==FALSE) {

			status = ApiSuccess;
			BufferIndex = BuffersCompleted % BuffersPosted;
			pBuffer = pParent->m_pUserData[BufferIndex];

			theApp.m_UtUser.ArmPrf();

			status = AlazarWaitAsyncBufferComplete(pParent->InstrumentID[0], pBuffer, 1000);


			if(status == ApiSuccess) {
				if(theApp.m_Si10User.IsConnected() == TRUE) {
					if(theApp.m_Si10User.Si10ReadTS0()) {
						if(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>1) {
							nSlot = theApp.m_UtUser.m_Global.Mux.nMaxTimeslots-2;
						} else {
							nSlot = 0;
						}
					}
				} else {
					if(theApp.m_UtUser.Inp(theApp.m_UtUser.MailboxAddrs+2) & 1) {
						nSlot = theApp.m_UtUser.ReadTS0()-1;
					}
				}

				if(theApp.m_Scope.m_nPlayRecording[nSlot]==0) {


					pSrc = (unsigned __int16*)pBuffer+theApp.m_UtUser.m_TS[nSlot].Adc.nHardwareDelay;
					pDest = (unsigned char *)theApp.m_Scope.m_RFTrace[nSlot];
					pwDest = (unsigned __int16*)theApp.m_Scope.m_wRFTrace[nSlot];

					if(theApp.m_UtUser.m_TS[nSlot].Pr30.nOutputSelect==2) {
						for(unsigned int nS=0; nS<theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength; nS++) {
							uwValue = pSrc[nS] + 0x7fff;
							cValue = (unsigned char)(uwValue >> 8);
							pDest[nS]=pParent->cLogConversionTable[cValue];

							pwDest[nS]=pParent->wLogConversionTable[uwValue];
						}
					} else {
						for(unsigned int nS = 1; nS<theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength; nS++) {
							wValue = pSrc[nS];
							pDest[nS] = wValue >> 8;
						}
						CopyMemory(pwDest, pSrc, theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength * sizeof (__int16));
					}

/*
					pDest = (unsigned char *)theApp.m_Scope.m_RFTrace[nSlot];
					if(theApp.m_UtUser.m_TS[nSlot].Pr30.nOutputSelect==2) {
						for(ii=0;ii<theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength;ii++) {
							nn = (char)pSrc[ii]+128;
							if(nn<0) nn=0;
						//	if(nn>255) nn=255;
							pDest[ii]=pParent->cLogConversionTable[(char)pSrc[ii]+128];
						}
					} else {
						CopyMemory(pDest,pSrc,theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength);
					}
					*/
				}
				theApp.m_UtUser.m_TS[nSlot].nIrqCount++;

				BuffersCompleted++;
				theApp.m_UtUser.ProcessTrace(theApp.m_Scope.m_RFTrace[nSlot],nSlot);
				nSlot++;
				nSlot %= theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;

				status = AlazarPostAsyncBuffer(pParent->InstrumentID[0],  pParent->m_pUserData[BufferIndex], pParent->m_nBytesPerBuffer);
				if(status != ApiSuccess) 
				{
					//ApiBufferOverflow? PRF may be too high see note above!
					status = AlazarAbortAsyncRead(pParent->InstrumentID[0]);
					goto restart;
				}

				theApp.m_Acqiris.m_nConversions++;

			} else {

				status = AlazarAbortAsyncRead(pParent->InstrumentID[0]);
				goto restart;
			}
		}

	}

	AlazarAbortAsyncRead(pParent->InstrumentID[0]);
	pParent->m_bThreadFinished=TRUE;

	return 0;
}

//m_nLogMin m_nLogMax 0 <--> 0x7ff

void CAlazarDigitizer::CalculateLogConversionTable()
{
	int	nMin = theApp.m_Scope.m_nLogMin / 8;
	int	nMax = theApp.m_Scope.m_nLogMax / 8;
	int	ii;
	static int nDelta =0;

	theApp.m_Scope.CalculateLogConversionTable();

	if(theApp.m_nTrackingAmp[theApp.m_nTrackingTS] < theApp.m_UtUser.m_TS[theApp.m_nTrackingTS].Gate.nAmplitude[0]) {
		nDelta--;
	}
	if(theApp.m_nTrackingAmp[theApp.m_nTrackingTS] > theApp.m_UtUser.m_TS[theApp.m_nTrackingTS].Gate.nAmplitude[0]) {
		nDelta++;
	}
	if(theApp.m_bTracking!=TRUE) nDelta = 0;
	if(nDelta>10) nDelta=10;
	if(nDelta<-10) nDelta = -10;

	//8 bit
	if(nMax == nMin) nMax = nMin + 2;

	for(ii=0;ii<nMin;ii++) 
		cLogConversionTable[ii]=0;
	for(ii;ii<nMax;ii++) {
		cLogConversionTable[ii]=(char)(((ii-nMin) * 127) / (nMax-nMin)) + nDelta;

	}
	for(ii;ii<256;ii++) {
		cLogConversionTable[ii]=127;
	}

	//16 bit
	
	nMin = theApp.m_Scope.m_nLogMin << 5;
	nMax = theApp.m_Scope.m_nLogMax << 5;
	if(nMax == nMin) nMax = nMin + 0x20;
	int nValue;

	for(ii=0;ii<nMin;ii++) 
		wLogConversionTable[ii]=0;
	for(ii;ii<nMax;ii++) {
		nValue = (((ii-nMin) * 0x7fff) / (nMax-nMin)) + (nDelta << 6);
		if(nValue < 0) nValue = 0;
		if(nValue > 0x7fff) nValue = 0x7fff;
		wLogConversionTable[ii]=(__int16)nValue;
	}
	for(ii;ii<0x10000;ii++) {
		wLogConversionTable[ii]=0x7fff;
	}
	
}


BOOL AbortAsyncRead()
{
	RETURN_CODE retCode = AlazarAbortAsyncRead(theApp.m_Alazar.InstrumentID[0]);
	if(retCode != ApiSuccess) {
		return FALSE;
	}
	return TRUE;
}