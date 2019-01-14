// USLut.h: interface for the CUSLut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USLUT_H__3939471F_0E0A_4981_8EE0_5794EF79250F__INCLUDED_)
#define AFX_USLUT_H__3939471F_0E0A_4981_8EE0_5794EF79250F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "C:\DriverProjects\USLDriver\sys\USLIoctl.h"
//#include "USLIoctl.h"


#include "USLFile.h"
#include "Ethernet.h"
#include "USLScanner.h"	// Added by ClassView
#include "ActiveTimeSlots.h"

#define	COLLECT_DISABLE		0
#define	COLLECT_UNTIL_FULL	1
#define	COLLECT_RING_FILL	2

#define ADC_NORMAL_OPERATION	0
#define ADC_MID_SCALE			1
#define ADC_POSITIVE_FULLSCALE	2
#define ADC_NEGATIVE_FULLSCALE	3
#define ADC_CHECKERBOARD		4
#define ADC_RAMP				0x0f


struct	PrCalibrationResultsData {
	int	nStructSize;
	float fMinFrequency;
	float fMaxFrequency;
	float fIncFrequency;
	float fPrGain;
	float fMaxDeviation;
	float fMinDeviation;
	bool bDefaultTraceTrue;
	int nArrayL;
	int	nDefaultSignalAmplitude;
	int	m_osTestDateTime;
	int nSpare[18];
};

enum Pm30CommDevice { //In Order of Prefere3nces
	NOTHING_ATTACHED,
	SI10_I2C,
	USB_I2C,
	USLDRIVER_I2C,
	PM40_I2C,
};

struct PM30_RETURN_MSG{
	int nBytes;
	BYTE data[50];
};


struct UT_ETHERNET_MSG {
	union {
		char AllData[PACKET_SIZE];
		struct {
			int Instruction;
			int nDataSize;
			char Data[PACKET_SIZE-8];
		};
	};
};
struct UT_RETURN_ETHERNET_MSG {
	union {
		char AllData[PACKET_SIZE];
		struct {
			int Instruction;
			int	nStatus;
			int nDataSize;
			char Data[PACKET_SIZE-12];
		};
	};
};

struct UT_ETHERNET_MSG_ASCII {
	union {
		char AllData[PACKET_SIZE];
		struct {
			int Instruction;
			int nDataSize;
			char Data[PACKET_SIZE-8];
		};
	};
};
struct UT_RETURN_ETHERNET_MSG_ASCII {
	union {
		char AllData[PACKET_SIZE];
		struct {
			char cStr[PACKET_SIZE];
		};
	};
};


extern	long USLutEthernetCallback(CWnd* pParent, int nDataSize, char* Data);

class CUSLut  
{
public:
	void GetCollect(int nDir,int nIndex,struct TimeSlotDataCollectionStruct *pTSCollect);
	int CollectGetSize(int nDir);
	int GetCollectPtr();
	int m_nCollectMode;
	void Collect(int nCollectMode);
	bool GetNearestCollectValue(int nDir,int nFieldBusCount,struct TimeSlotDataCollectionStruct *TSCollect,int *nPtr = NULL);
	void CollectData(int nSlot, int nFieldBusInputReg, int nFieldBusCount);
	int m_nCollectPtr;
	int	m_nCollectL[2];
	struct TimeSlotDataCollectionStruct *m_pCollect[2];
	void AllocateDataCollection();
	CString strGetAmplifier(int nTimeSlot);
	CString strGetPreAmpGain(int nTimeSlot);
	CString strGetLogGain(int nTimeSlot);
	bool EthernetConnect();
	CEthernet m_Ethernet;

	void TurntableGainChange(CCoord Cp);
	void SetAllUt(CCoord Cp,int *pHeader);
	void SetGatesFromCoordinate(COORDDATA Cp);
	void GetAllUt(CCoord *pCp);
	void GetGatePos(COORDDATA *pCp);

	int GetGateMaterialTimeNs(int nTS, int nTOF);
	int GetVerboseGateMaterialTimeNs(int nTS, int nTOF,int *pnInterface,int *pnBackWall,int *pnConfidence);
	int GetGateWaterTimeNs(int nTS);
	float GetVerboseGateThickness(int nTS, int nTOF,int *pnInterface,int *pnBackWall,int *pnConfidence);
	float GetGateThickness(int nTS,int nTOF);
	float GetGateWaterPath(int nTS);

	void GateGetTriggerMode(GatesData* pGate, int nGate,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void GateGetDelay(GatesData* pGate, int nGate,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void GateGetWidth(GatesData* pGate, int nGate,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void GateGetThreshold(GatesData* pGate, int nGate,CString* pBuff,int nVerbose=0, int nLanguage=0);

	void ProcessSignedTraceOld(char * trace, int nSlot);
	void ProcessSignedTrace(char *trace, int nSlot);

	bool HasInterfaceFailed();
	int m_nInterfaceFailureCount;
	void RetrieveVersion1(CUSLFile* pFile,UtGlobalData *pGlobal,TimeSlotData *pTS,int nGlobalOffset=0,int nTimeSlotOffset=0,int nOffset=0x200);
	void EnableMailBoxTrigger(bool bFlag);
	void ProcessUnSignedTrace(unsigned char *trace,int nSlot);
	void ProcessTrace(char *trace,int nSlot);
	void Dsp200WaitForFifo();
	int m_nLastAdcTimeSlot;
	int m_nLastDspTimeSlot;
	bool m_bDspFifoWait;
	bool m_bAdcFifoWait;
	int	m_nMaxPRF;
	int	m_nCurrentPRF;
	int m_nPM30Count;
	int m_nPM30Addresses[4];
	int m_nMailboxRxFailed;

	CUSLFile m_file;
	void AllPrPlTxRxModes();

	void Pr15GetFilterRange(Pr15Data *pPr15, CString *pBuff,int nVerbose=0, int nLanguage=0);
	void Pr15GetCenterFrequency(Pr15Data *pPr15, CString *pBuff,int nVerbose=0, int nLanguage=0);
	void Pr15GetLowPassFilter(Pr15Data *pPr15, CString *pBuff,int nVerbose=0, int nLanguage=0);
	void Pr15GetMainGain(Pr15Data* pPr15,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void Pr15GetPreampGain(Pr15Data* pPr15,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void Pr15GetDamping(Pr15Data* pPr15,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void Pr15GetPulseVoltage(Pr15Data* pPr15,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void Pr15GetPulseWidth(Pr15Data* pPr15,CString* pBuff,int nVerbose=0, int nLanguage=0);

	void Pr30Timeslot(Pr30Data* pPr30);
	void Pr30MuxStart(Pr30Data* pPr30);
	void Pr30MuxHalt(Pr30Data* pPr30);
	void Pr30MuxReset(Pr30Data* pPr30);
	void Pr30SetRx(Pr30Data* pPr30);
	void Pr30SetTx(Pr30Data* pPr30);
	void Pr30GetFilterType(Pr30Data *pPr30, CString *pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetFilterRange(Pr30Data *pPr30, CString *pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetLowPassFilter(Pr30Data *pPr30, CString *pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetLinearTimeConstant(Pr30Data *pPr30, CString *pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetMainGain(Pr30Data* pPr30,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetPreampGain(Pr30Data* pPr30,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetDamping(Pr30Data* pPr30,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetPulseVoltage(Pr30Data* pPr30,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetPulseWidth(Pr30Data* pPr30,CString* pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetOutputModeAndGain(Pr30Data *pPr30, CString *pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetOutputMode(Pr30Data *pPr30, CString *pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetTxRx(Pr30Data *pPr30, CString *pBuff,int nVerbose=0, int nLanguage=0);
	void Pr30GetDACMode(Pr30Data *pPr30, CString *pBuff,int nVerbose=0, int nLanguage=0);

	void Pr30AuxFloatGain(Pr30Data *pPr30);
	void Pr30LogPostDetectFilter(Pr30Data* pPr30);
	void Pr30PulseVoltageMSC(Pr30Data *pPr30);
	void Pr30CenterFrequency(Pr30Data *pPr30);
	void Pr30FilterRange(Pr30Data *pPr30);
	void Pr30LinearTimeConstant(Pr30Data* pPr30);
	void Pr30ResetMaxDacDataNumber(Pr30Data* pPr30);
	void Pr30CalculateDacData(Pr30Data* pPr30);
	void Pr30DacEnable(Pr30Data* pPr30);
	void Pr30DacClkRate(Pr30Data* pPr30);
	void Pr30Initialize(Pr30Data *pPr30);
	void Pr30Defaults(Pr30Data *pPr30,int nTS=0);
	void Pr30ZeroCurrentDacDelaysAndGains(Pr30Data* pPr30);
	void Pr30ZeroCurrentDacGains(Pr30Data* pPr30);
	void Pr30DacTriggerMode(Pr30Data* pPr30);
	void Pr30DacThreshold(Pr30Data *pPr30);
	void Pr30DacBlankingFromString(Pr30Data* pPr30,CString Buff);
	void Pr30DacBlankingString(Pr30Data* pPr30,CString* pBuff);
	void Pr30BlankingIntToFloat(Pr30Data *pPr30);
	void Pr30DacBlanking(Pr30Data *pPr30);
	void Pr30RectifierDecay(Pr30Data* pPr30);
	void Pr30RectifierMode(Pr30Data* pPr30);
	float Pr30CalculateRealWorldCentreFrequency(Pr30Data *pPr30, CString *pBuff);
	void Pr30FilterMode(Pr30Data* pPr30);
	void Pr30BandPassWidth(Pr30Data* pPr30);
	void Pr30FilterType(Pr30Data* pPr30);
	void Pr30LinFloatGain(Pr30Data* pPr30);
	void Pr30LogFloatGain(Pr30Data* pPr30);
	void Pr30PreampGain(Pr30Data *pPr30);
	void Pr30PreampMode(Pr30Data *pPr30);
	void Pr30Damping(Pr30Data *pPr30);
	void Pr30PulseWidth(Pr30Data *pPr30);
	void Pr30PulserOffOn(Pr30Data *pPr30);
	void Pr30PulseVoltage(Pr30Data *pPr30);
	void Pr30OutputSelect(Pr30Data *pPr30);
	void Pr30OutputOffsetEnable(Pr30Data *pPr30);
	void Pr30HtOffOn(Pr30Data *pPr30);
	void Pr30ResetMaxTimeSlots(Pr30Data *pPr30, bool bClearAllData = true);
	void Pr30Prf(Pr30Data *pPr30);
	void Pr30SyncPulseTimeSlot(Pr30Data *pPr30);
	void Pr30SelfTestOnOff(Pr30Data *pPr);
	void Pr30SelfTestFrequency(Pr30Data *pPr);
	void Pr30SelfTestAmplitude(Pr30Data *pPr);
	void Pr30UmbilicalSelect(Pr30Data *pPr, int nChannel);
	void Pr30UmbilicalSelect(int nChannel);

	void Pr30Command1(Pr30Data	*pr30);
	void Pr30Command2(Pr30Data	*pr30);
	void Pr30Command3(Pr30Data	*pr30);
	void Pr30Command4(Pr30Data	*pr30);
	void Pr30Command5(Pr30Data	*pr30);
	void Pr30Command6(Pr30Data	*pr30);
	void Pr30Command7(Pr30Data	*pr30);
	void Pr30Command8(Pr30Data	*pr30,int nAction = 0);
	void Pr30Command9(Pr30Data	*pr30);
	void Pr30Command10(Pr30Data	*pr30);
	void Pr30Command11(Pr30Data	*pr30);
	void Pr30Command12(Pr30Data	*pr30);
	void Pr30Command13(Pr30Data	*pr30);
	void Pr30Command14(Pr30Data	*pr30);
	void Pr30Command15(Pr30Data	*pr30);
	void Pr30Command16(Pr30Data	*pr30);
	void Pr30Command17(Pr30Data	*pr30);
	void Pr30Command18(Pr30Data	*pr30,bool bReset = FALSE);
	void Pr30Command19(Pr30Data	*pr30);
	void Pr30Command20(Pr30Data	*pr30);
	void Pr30Command21(Pr30Data	*pr30);
	void Pr30Command22(Pr30Data	*pr30);
	void Pr30Command23(Pr30Data	*pr30);
	void Pr30Command24(Pr30Data	*pr30);
	void Pr30Command25(Pr30Data	*pr30);
	void Pr30Command26(Pr30Data	*pr30);
	void Pr30Command27(Pr30Data	*pr30);
	void Pr30Command28(Pr30Data	*pr30);
	void Pr30Command30(Pr30Data	*pr30);
	void Pr30Command31(Pr30Data	*pr30);
	void Pr30Command32(Pr30Data	*pr30);
	void Pr30Command33(Pr30Data	*pr30);
	void Pr30Command34(Pr30Data	*pr30);
	void Pr30Command35(Pr30Data	*pr30);
	void Pr30Command36(Pr30Data	*pr30);
	void Pr30Command37(Pr30Data	*pr30);
	void Pr30Command38(Pr30Data	*pr30);
	void Pr30Command39(Pr30Data	*pr30);
	void Pr30Command40(Pr30Data	*pr30,int nStartAddrs);
	void Pr30Command41(Pr30Data	*pr30,int *pnValue);
	void Pr30Command42(Pr30Data	*pr30);
	void Pr30Command43(Pr30Data	*pr30);
	void Pr30Command45(Pr30Data	*pr30);
	void Pr30Command46(Pr30Data	*pr30);
	void Pr30Command47(Pr30Data	*pr30);
	void Pr30Command48(Pr30Data	*pr30);
	void Pr30Command49(Pr30Data	*pr30);
	void Pr30Command50(Pr30Data	*pr30);
	void Pr30Command51(Pr30Data	*pr30);
	void Pr30Command52(Pr30Data	*pr30);
	void Pr30Command53(Pr30Data	*pr30);
	void Pr30Command54(Pr30Data	*pr30);
	void Pr30Command55(Pr30Data	*pr30);
	void Pr30Command56(Pr30Data	*pr30);
	void Pr30Command64(Pr30Data	*pr30);
	void Pr30Command67(Pr30Data	*pr30);
	void Pr30Command68(Pr30Data	*pr30);
	void Pr30Command69(Pr30Data	*pr30);
	void Pr30Command4Ah(TimeSlotData * pTS);
	void Pr30Command4Bh(TimeSlotData * pTS);
	void Pr30Command4Dh(TimeSlotData * pTS);
	void Pr30Command69h(TimeSlotData * pTS, int nMode);

	///////////////////////////////////////////////////////////////
	int	m_nGateAmp[1024][256][8];
	int	m_nAveragePtr;
	int	GetAverageAmplitude(int nTS, int nGate);

	char	m_BscanArray[10000000];
	void InitializeAddrs();
	int Adc100ReadArray(ADC200Data *pData,char *pArray);
	void Adc100CalculateHardware(ADC200Data *pAdc);
	void Adc100Priority(ADC200Data *pAdc);
	void Adc100Delay(ADC200Data *pAdc);
	void Adc100ConvTypeFreq(ADC200Data *pAdc);
	void Adc100Initialize(ADC200Data *pAdc);
	void Adc100Default(ADC200Data *pAdc);

	void MuxRun();
	void MuxHalt();
	void MuxReset();
	void ResetMaxTimeSlots();
	void MailboxPrfLowPeriod();
	void MailboxPrf(int nValue);
	void MailboxHtReset();
	int  MailboxTxCheckSum();
	void Buzzer(bool bAction);

	CString	m_Units[3];
	int	m_nPlcAddrs;
	int	m_nCheckSum;
	CString	m_FileName;
	HANDLE m_hUSL;
	struct	TimeSlotData	m_TS[256];
	struct	UtGlobalData	m_Global;
	struct	PrCalibrationResultsData m_Cal[256];
	int* m_pnDefaultCalTrace[256];
	int* m_pnCalTrace[256];
	int	MailboxAddrs;
	int MailboxCount;
	int m_nMailboxFailed;
	int MaxMailboxWait;
	bool StartDriver(bool EnableUSLDriver = false,bool bEnableUSlDriverInterrupts = false);
	int MinMax(int *pnV, int nMin, int nMax);
	long MinMax(long *pnV, int nMin, int nMax);
	CUSLut();
	~CUSLut();
	void Initialize(int nJobNumber);
	void TimeSlotDefault();
	BOOL GetHandleToDevice(int OS);

	WORD Inp(int nOffset,int nWhichInterface = SI6DRIVER);
	WORD Inpw(int nOffset,int nWhichInterface = SI6DRIVER);
	void Outp(int nOffset, int nValue,int nWhichInterface = SI6DRIVER);
	void Outpw(int nOffset, int nValue,int nWhichInterface = SI6DRIVER);

	void GetVersion(CString* Version);
	void GetVersion(char *Version,int Length);
	void EnableInterrupts();
	void DisableInterrupts();
	BOOL ShareTimeSlotStruct();
	BOOL FreeTimeSlotStruct();
	BOOL ShareSi6Struct(struct SI6Data *pSi6);
	BOOL FreeSi6Struct();
	BOOL ShareServoStruct(struct SERVOData *pServo);
	BOOL FreeServoStruct();
	BOOL ReadAdc200Memory(char* Buffer,int nLength,int nWhichInterface = SI6DRIVER);
	BOOL AllocateAndShareBscanMemory();
	BOOL FreeAndDeallocateBscanMemory();

	void	*m_pBscanData;
	void TimeslotDefaults();

	///////////////////////////////////////////////////////////////////
	void Pr2050Initialize(struct PR2050Data *pr);
	void Pr2050Default(struct PR2050Data *pr,int type,int channel);
	BOOL Pr2050PulseVoltage(struct PR2050Data *pr, bool bOnOff = true);
	void Pr2050Attenuation(struct PR2050Data	*pr);
	void Pr2050FiltersRectifier(struct PR2050Data *pr);
	void Pr2050Prf(struct PR2050Data *pr);

	///////////////////////////////////////////////////////////////////
	void MscDefaults();
	void MscDefaults(struct MSCData *msc);
	BOOL MscInitialize();
	BOOL MscInitialize(struct MSCData *msc);
	int MscCounterRead(struct MSCData *msc,int counter);
	void MscInterruptCounter(struct MSCData *msc,int count);
	void MscPiaInitialize(struct MSCData *msc);
	void MscPiaOut(struct MSCData *msc,int channel,int value);
	void MscSwitchPiaOut(struct MSCData *msc,int channel,int value);
	int MscAdcRead(int channel);
	int MscAdcRead(struct MSCData *msc,int channel);
	void MscEncoderReset(struct MSCData *msc,int number);
	int MscEncoderRead(struct MSCData *msc,int encoder);
	void MscEncoderWrite(struct MSCData *msc,int encoder,int value);
	void MscDacWrite(struct MSCData *msc,int dac,int value);
	void MscSwitchHttoChannel(int nChannel);
	/////////////////////////////////////////////////////////////
	void RfAndClockSource(struct MSCData *msc,int Tx,int Rx);
	void RfAndClockSource();

	///////////////////////////////////////////////////////////////
	void Dsp200Defaults(struct	DSP200Data	*dsp,int nTimeSlot);
	void Dsp200CheckVariables(DSP200Data *dsp,int nTimeSlot);
	void Dsp200Initialize();
	void Dsp200Initialize(struct	DSP200Data *dsp,int nTimeSlot);
	void Dsp200GpdSetup(struct	DSP200Data *dsp,int which);
	void Dsp200GpdInterface(struct	DSP200Data *dsp);
	void Dsp200Tims(struct	DSP200Data *dsp,int which);
	void Dsp200Prf(struct	DSP200Data *dsp);
	BOOL Dsp200Present();
	BOOL Dsp200Present(struct DSP200Data *dsp);
	void Dsp200ClearFifo(struct	DSP200Data *dsp);
	void Dsp200TriggerShot(struct	DSP200Data	*dsp);
	void Dsp200Reset();
	void Dsp200Reset(struct	DSP200Data	*dsp);
	void Dsp200Restart();
	void Dsp200Timeslot(struct DSP200Data *dsp);
	void Dsp200Control(struct	DSP200Data	*dsp,int	process);
	void Dsp200StartProcessing(struct DSP200Data *dsp);
	void Dsp200StopProcessing(struct DSP200Data *dsp);
	BOOL Dsp200CalculateHardware(int nTimeSlot);
	BOOL Dsp200CalculateHardware(int nTimeSlot,int nWhich);
	BOOL Dsp200CalculateTimsHardware(int nTimeSlot,int nWhich);
	void Dsp200GetStartStopTimeNs(int *nStart,int *nStop,bool *bPulseEcho=NULL,int *nTransmitter=NULL,int nTimeSlot=0);
	void Dsp200CheckTimeSlot(DSP200Data *pDsp);

	/////////////////////////////////////////////////////////////
	void Adc200Default();
	void Adc200Initialize();
	void Adc200Default(struct ADC200Data *adc,int nTimeSlot);
	void Adc200Initialize(struct ADC200Data *adc);
	void Adc200Prf(struct ADC200Data *adc);
	void Adc200CalculateHardware();
	void Adc200CalculateHardware(struct ADC200Data *adc);
	void Adc200WriteTimeslot(struct ADC200Data *adc,int clear_flag);
	void Adc200ConvertTimeslot(struct ADC200Data *adc);
	void Adc200Delay(struct ADC200Data *adc);
	void Adc200Width(struct ADC200Data *adc);
	void Adc200RFSource(struct ADC200Data *adc);
	void Adc200MemoryAddrs(struct ADC200Data *adc);
	BOOL Adc200ReadParameters();
	BOOL Adc200ReadParameters(struct ADC200Data *adc);
	int Adc200ReadArray(struct ADC200Data *adc,char *trace);
	void Adc200PurgeBothFifos(struct ADC200Data *adc);
	void Adc200PurgeOutFifo(struct ADC200Data *adc);
	void Adc200ResetMaxTimeSlot(struct ADC200Data *adc);
	void Adc200MuxStart(ADC200Data *adc);
	void Adc200MuxHalt(ADC200Data *adc);
	void Adc200CheckTimeSlot(ADC200Data *pAdc);
	void Adc200SelectRFSource(ADC200Data *pAdc,int nPr);
	void Adc200Reset();
	void Adc200Command0(struct ADC200Data *adc);
	void Adc200Command1(struct ADC200Data *adc);
	void Adc200Command2(struct ADC200Data *adc);
	void Adc200Command3(struct ADC200Data *adc,int action);
	void Adc200Command4(struct ADC200Data *adc);
	void Adc200Command5(struct ADC200Data *adc);
	void Adc200Command11(struct ADC200Data *adc);
	/////////////////////////////////////////////////////////////
	void Pr15Defaults(struct Pr15Data *pr15);
	void Pr15Initialize(struct Pr15Data *pr15);
	void Pr15NonPreampInitialize(struct Pr15Data *pr15);
	void Pr15TxRxMode(struct	Pr15Data	*pr15);
	void Pr15Damping(struct	Pr15Data	*pr15);
	void Pr15PreampGain(struct	Pr15Data	*pr15);
	void Pr15PulseWidth(struct	Pr15Data	*pr15);
	void Pr15Prf(struct	Pr15Data	*pr15);
	void Pr15PrfSrc(struct	Pr15Data	*pr15);
	void Pr15PulseVoltage(struct	Pr15Data	*pr15);
	void Pr15Pulser(struct	Pr15Data	*pr15);
	void Pr15Gain(struct	Pr15Data	*pr15);
	void Pr15Output(struct Pr15Data *pr15);
	void Pr15Filters(struct Pr15Data *pr15);
	void Pr15LowPass(struct	Pr15Data	*pr15);
	void Pr15Rectifier(struct	Pr15Data	*pr15);
	void Pr15TvgCommands(struct	Pr15Data	*pr15);
	void Pr15TvgData(struct	Pr15Data	*pr15);
	void Pr15Ts0ResetSource(struct Pr15Data *pr15);
	void Pr15TvgSinglePoint(struct	Pr15Data *pr15,int ptr);
	void Pr15MuxReset(struct Pr15Data *pr15);
	void Pr15MuxTxRx(struct Pr15Data *pr15);
	void Pr15Timeslot(struct Pr15Data *pr15);
	void Pr15SyncPulses(struct	Pr15Data	*pr15);
	void Pr15SyncPulseMode(struct Pr15Data *pr15);
	void Pr15MuxHt(struct Pr15Data *pr15);
	void Pr15MuxState(struct Pr15Data *pr15);
	void Pr15PulseVoltageMSC(struct Pr15Data *pr15);
	void Pr15SelfTestAmplitude(Pr15Data* pPr15);
	void Pr15SelfTestFrequency(Pr15Data* pPr15);
	void Pr15SelfTestOnOff(Pr15Data* pPr15);
	void Pr15Command0(struct	Pr15Data	*pr15);
	void Pr15Command1(struct	Pr15Data	*pr15);
	void Pr15Command2(struct	Pr15Data	*pr15);
	void Pr15Command3(struct	Pr15Data	*pr15);
	void Pr15Command4(struct	Pr15Data	*pr15);
	void Pr15Command5(struct	Pr15Data	*pr15);
	void Pr15Command6(struct	Pr15Data	*pr15);
	void Pr15Command7(struct	Pr15Data	*pr15);
	void Pr15Command8(struct	Pr15Data	*pr15);
	void Pr15Command9(struct	Pr15Data	*pr15);
	void Pr15Command10(struct	Pr15Data	*pr15);
	void Pr15Command11(struct	Pr15Data	*pr15);
	void Pr15Command12(struct	Pr15Data	*pr15);
	void Pr15Command13(struct	Pr15Data	*pr15);
	void Pr15Command14(struct	Pr15Data	*pr15);
	void Pr15Command15(struct	Pr15Data	*pr15);
	void Pr15Command16(struct	Pr15Data	*pr15);
	void Pr15Command17(struct	Pr15Data	*pr15);
	void Pr15Command18(struct	Pr15Data	*pr15);
	void Pr15Command19(struct	Pr15Data	*pr15);
	void Pr15Command20(struct	Pr15Data	*pr15);
	void Pr15Command21(struct	Pr15Data	*pr15);
	void Pr15Command32(struct	Pr15Data	*pr15);
	void Pr15Command33(struct	Pr15Data	*pr15);
	//Conversion routine
	int Pr15From_dB_To_Hardware(double GaindB);
	double Pr15From_Hardware_To_dB(int GainHardware);
	//////////
	void Pl15Defaults(struct Pl15Data *pl15);
	void Pl15Initialize(struct Pl15Data *pl15);
	void Pl15NonPreampInitialize(struct Pl15Data *pl15);
	void Pl15TxRxMode(struct	Pl15Data	*pl15);
	void Pl15Damping(struct	Pl15Data	*pl15);
	void Pl15PreampGain(struct	Pl15Data	*pl15);
	void Pl15PulseWidth(struct	Pl15Data	*pl15);
	void Pl15prf(struct	Pl15Data	*pl15);
	void Pl15PrfSrc(struct	Pl15Data	*pl15);
	void Pl15PulseVoltage(struct	Pl15Data	*pl15);
	void Pl15Pulser(struct	Pl15Data	*pl15);
	void Pl15Gain(struct	Pl15Data	*pl15);
	void Pl15Output(struct Pl15Data *pl15);
	void Pl15Filters(struct Pl15Data *pl15);
	void Pl15LowPass(struct	Pl15Data	*pl15);
	void Pl15Detector(struct	Pl15Data	*pl15);
	void Pl15Ts0ResetSource(struct Pl15Data *pl15);
	void Pl15MuxReset(struct Pl15Data *pl15);
	void Pl15Timeslot(struct Pl15Data *pl15);
	void Pl15MuxTxRx(struct Pl15Data *pl15);
	void Pl15SyncPulses(struct	Pl15Data	*pl15);
	void Pl15MuxHt(struct Pl15Data *pl15);
	void Pl15MuxState(struct Pl15Data *pl15);
	void Pl15GetPulseWidth(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage);
	void Pl15GetPulseVoltage(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage);
	void Pl15GetDamping(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage);
	void Pl15GetPreampGain(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage);
	void Pl15GetMainGain(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage);
	void Pl15GetFilterRange(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage);
	void Pl15GetCenterFrequency(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage);
	void Pl15GetLowPassFilter(Pl15Data *pPl15, CString *pBuff,int nVerbose, int nLanguage);
	void Pl15Command0(struct	Pl15Data	*pl15);
	void Pl15Command1(struct	Pl15Data	*pl15);
	void Pl15Command2(struct	Pl15Data	*pl15);
	void Pl15Command3(struct	Pl15Data	*pl15);
	void Pl15Command4(struct	Pl15Data	*pl15);
	void Pl15Command5(struct	Pl15Data	*pl15);
	void Pl15Command6(struct	Pl15Data	*pl15);
	void Pl15Command7(struct	Pl15Data	*pl15);
	void Pl15Command8(struct	Pl15Data *pl15);
	void Pl15Command11(struct	Pl15Data	*pl15);
	void Pl15Command17(struct	Pl15Data *pl15);
	void Pl15Command18(struct	Pl15Data *pl15);
	void Pl15Command19(struct	Pl15Data *pl15);
	void Pl15Command20(struct	Pl15Data *pl15);
	///////////
	void TimsDefaults(struct	AtpTimsData *tims);
	void TimsControl(struct	AtpTimsData *tims);
	void TimsBlanking(struct	AtpTimsData *tims);
	void TimsDelay(struct	AtpTimsData *tims);
	void TimsStartThreshold(struct	AtpTimsData *tims);
	void TimsStopThreshold(struct	AtpTimsData *tims);
	void TimsAverages(struct	AtpTimsData *tims);
	int TimsTime();
	int TimsTime(struct AtpTimsData *tims);
	void TimsInitialize(struct	AtpTimsData *tims);
	////////////////////
	void GpdDefault(struct	AtpGpdData *gpd);
	void GpdControl(struct	AtpGpdData *gpd);
	void GpdBlanking(struct	AtpGpdData *gpd);
	void GpdDelay(struct	AtpGpdData *gpd);
	void GpdWidth(struct	AtpGpdData *gpd);
	void GpdMaxThreshold(struct	AtpGpdData *gpd);
	void GpdMinThreshold(struct	AtpGpdData *gpd);
	void GpdInterfaceThreshold(struct	AtpGpdData *gpd);
	int GpdAmplitude(struct AtpGpdData *gpd);
	void GpdInitialize(struct	AtpGpdData *gpd);



	//Conversion/////////////////////
	int Pl15From_dB_To_Hardware(double GaindB);
	double Pl15From_Hardware_To_dB(int GainHardware);

	double Pl15From_Hardware_To_Volt(int VoltHardware);
	int Pl15From_Volt_To_Hardware(double Volt);
	/////////
	void MuxDefaults();
	/////////
	void ResetMailbox();
	int MailboxTx(int	value);
	unsigned int MailboxRx(PM30_RETURN_MSG *pPm30Msg);
	void MailboxHt(bool bEnable,int nVoltage);

	///////////
	void SetPrf();
	void SetMaxPRF(float fRate , int nDelay);
	void SetMaxPRF();
	void RefreshAllVariables(int nJobNumber);
	void SelectWriteTimeslot();
	void SelectMaxTimeSlots();
	void ChangeMaxTimeSlots(int nJobNumber);

	int InterfacePos(int nTimeslot);

	void Retrieve(CString FilePath);
	void Save(CString FilePath);
	void Save(CUSLFile* pFile);
	void Retrieve(CUSLFile* pFile);

	void LiftInitialize();
	bool LiftCommandUp();
	bool LiftCommandDown();
	bool LiftCommandStop();
	bool IsLiftUp();
	bool IsLiftDown();
	int	WhereIsLift();
	void CopyTS(int nTSDest, int nTSSrc);
	int FindPM30s();
	void AutoDetectPM30OutputDevice();

	CWinThread* m_pThread;
	bool m_bThreadEnabled;
	bool m_bThreadFinished;
	void ArmPrf();
	int ReadTS0();
	bool SetupOutputDeviceSettings();
	bool CopyGatesAcrossTimeslots(int nTimeslot);

	int m_nArray[1000];


	Pm30CommDevice m_Pm30CommDevice;

	CActiveTimeSlots* GetActiveTimeSlots(){return m_pActiveTimeSlots;}

	private:
		CActiveTimeSlots* m_pActiveTimeSlots = NULL;
};


#endif // !defined(AFX_USLUT_H__3939471F_0E0A_4981_8EE0_5794EF79250F__INCLUDED_)
