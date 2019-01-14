//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
/////////////////////////////////////////////////////////////////////////////
//	ioctl.cpp:		DeviceIoControl IOCTL codes supported by PHD Io drivers
/////////////////////////////////////////////////////////////////////////////

#define	USL_VERSION		"USL V1.00 November 27th 2006"


//Card Defines
#define	USL_PR20	1
#define	USL_PR50	2
#define	USL_PR15	3
#define	USL_PL15	4
#define	USL_MSC		5
#define	USL_ADC100	6
#define	USL_ADC200	7
#define	USL_DSP200	8
#define	USL_ATP_GPD0	9
#define	USL_ATP_GPD1	10
#define	USL_ATP_TIMS0	11
#define	USL_ATP_TIMS1	12
#define	USL_SI6			13
#define	USL_PR30		14
#define	USL_UP15		15
#define USL_SI10		16
#define	USL_PR40		17

#define	ACQIRIS_DP211	100
#define ALAZAR_9462		101
#define ALAZAR_860		102


#define	RFTYPELINEAR	0
#define	RFTYPERECTIFIED	1
#define	RFTYPELOG		2

#define	PR30_INTERNAL_TRIG_SRC	0
#define	PR30_EXTERNAL_TRIG_SRC	1

#define	POSITIVE_PHASE	0
#define	NEGATIVE_PHASE	1

struct	Pr30Data {
	int	nStructSize;
	int	nSrcAddrs;
	int	nDestAddrs;

	int	nPrfSrc;
	int	nPrf;
	int	nWriteTimeSlot;
	int	nTimeSlotSyncSrc;
	int nSyncPulseTimeSlot;

	int nPreampWriteSelect;
	int	nPreampGain;
	int	nPreampMode;
	int	nDamping;
	int	nPulseWidth;

	int	nMuxTx;
	int	nMuxRx;
	int nMuxRun;

	float fLogGain;
	int	nLogCourseGain;
	int	nLogFineGain;
	int	nLogPostDetectFilter;
	float fLinGain;
	int	nLinGain;
	int	nLinTimeConstant;
	int	nPulseVoltage;
	int	nHtOffOn;
	int	nPulserOffOn;
	int	nOutputSelect;
	int	nRectifierDetectionMode;
	int nRectifierDecayPercentage;
	int	nOutputOffsetEnable;

	int	nFilterType;
	int nFilterMode;
	int	nLowPassRange;
	int	nBandPassRange;
	int	nBandPassWidth;
	int	nBandPassCenterFrequency;

	int	nDacEnable;
	int	nDacTriggerMode;
	int	nDacTriggerThreshold;
	int nDacTriggerSlope;
	int	nDacBlanking;
	float fDacBlanking;
	int	nDacClkRate;
	int	nNumberDacPts;
	int	nDacBlankingUnits;
	int	nDacDelayUnits;
	int	nDacTable;
	float fDacDelay[4][64];
	float fDacGain[4][64];

	int	nSelfTestOffOn;
	int	nSelfTestFrequency;
	int	nSelfTestAmplitude;

	float fAuxLinGain;
	int	nAuxLinGain;
	int	nAuxHighPass;
	int	nAuxLowPass;
	int	nAuxGateNumber;

	int	nExtraLinGain;		//Used in the scanning routines
	int	nDACDeltaDelay;

	int	nI2CUmbilicalSelect;
	int	nRxRfUmbilicalSelect;

	int nSpare[87];
};

struct PR2050Data {
	int StructSize;
	int nAddrs;
	int nAttenuation;
	int nFilter;
	int nRectification;
	int nPulseVoltage;
	int nPrf;
	int nType;
	int nRfChannel;
	int	nSpare[10];
};
struct	Pr15Data {
	int	nStructSize;
	int	nAddrs;
	int	nTxRxMode;
	int	nPulserOffOn;
	int	nHtOffOn;
	int	nPulseWidth;
	int	nDamping;
	int	nPrfSrc;
	int	nPrf;
	int	nPreampGain;
	int	nMainGain;
	int	main_op;
	int	monitor_op;
	int	nFilterCenter;
	int	nFilterRange;
	int	rf_low_pass;
	int	rectify_filter;
	int	mux_state;
	int	nMuxMode;
	int	tvg_trig_source;
	int	tvg_enable;
	int	tvg_clk;
	int	tvg_length;
	int	tvg[64];
	int	tvg_trig_mode;
	int	tvg_slope;
	int	tvg_cursor;
	int	tvg_interface_threshold;
	int	tvg_blanking;
	int	tvg_cursor_position;
	double	dTvgOffset[16];
	double	dTvgValue[16];
	int	absolute_frequency;
	int	nMuxTimeslot;
	int	nMuxTx;
	int	nMuxRx;
	int	nMuxReset;
	int	nMuxSyncPulse1;
	int	nMuxSyncPulse2;
	int	nMuxPulseVoltage;
	int	mux_max_timeslot;
	int	sync_pulse_mode;
	int	tvg_power;
	int	tvg_total;
	int	tvg_shape;
	int	prf_out_polarity;
	int	ts0_reset_src;
	int	abs_preamp_gain;
	int	abs_main_gain;
	int	pulse_voltage;
	BOOLEAN bSelfTest;
	int	nSelfTestFrequency;
	int	nSelfTestAmplitude;
	char	cVersion;
	int	nSpare[25];
};

struct	Pl15Data {
	int	nStructSize;
	int	nAddrs;
	int	nTxRxMode;
	int	nPulserOffOn;
	int	nHtOffOn;
	int	nPulseWidth;
	int	nDamping;
	int	nPrfSrc;
	int	prf;
	int	nPreampGain;
	int	nMainGain;
	int	nMainOutput;
	int	nInternalExternalRfSrc;
	int	nFilterCenter;
	int	nFilterRange;
	int	rf_low_pass;
	int	detector;
	int	mux_state;
	int	nMuxMode;
	int	nMuxTimeslot;
	int	nMuxTx;
	int	nMuxRx;
	int	mux_reset;
	int	mux_sync_pulse_1;
	int	mux_sync_pulse_2;
	int	nMuxPulseVoltage;
	int	nMaxTimeslot;
	int	tvg_shape;
	int	prf_out_polarity;
	int	ts0_reset_src;
	int	nSpare[20];
};


struct	MuxData {
	int	nStructSize;
	int	nTxRxMode[8];
	int nMaxTimeslots;
	int	nAddrs;
	int	nOffset[8];
	int	nState;
	int	nIrq;
	int	nIrqAddrs;
	int	nIrqBit;
	};

struct LSAData{
	int nDOF;
	int nOldFocused; //0-false 1-true
	int nHighPassFilter;
	int nLowPassFilter;
	int nPulsarOnOff;
	int nOldElements; //Elements per focal law
	int nFLS;
	int nPreAmpGain;
	int nPulseVoltage;
	int nPulseWidth;
	float fOldScanPitch; //in multiples of fArrayPitch
	int  nOldElementsInArray;
	float fOldArrayPitch;
	int nOldStartElement;
	BOOLEAN bOldReverseArrayOrder;
};


struct	AtpGpdData {
	int	StructSize;
	int	nAddrs;
	int	MuxOffset;
	int	MuxAddrs;
	int	nControlReg0;
	int	nControlReg1;
	int	nBlanking;
	int nDelay;
	int nWidth;
	int	nMinThreshold;
	int	nMaxThreshold;
	int	nInterfaceThreshold;
	int	defect_threshold;
	int	rf_select;
	int	nTriggerMode;
	int	peak_sign;
	int	interface_sign;
	int	interface_slope;
	int	clk_rate;
	int	blank_marker;
	int	delay_marker;
	int	period_marker;
	int	rf_monitor;
	int	fir;
	int	amplitude;
	int	flags;
	int	defect_polarity;
	int	width_mode;
	int	scope_trigger_mode;
	int	max_width;
	char	spare[40];
};


struct	AtpTimsData {
	int	StructSize;
	int	nAddrs;
	int	MuxOffset;
	int	MuxAddrs;
	int	control_reg0;
	int	control_reg1;
	int	nBlanking;
	int nDelay;
	int	nStartThreshold;
	int	nStopThreshold;
	int	averages;
	int	rf_select;
	int	average_mode;
	int	nTriggerMode;
	int	nStartSignalSign;
	int	nStartSignalSlope;
	int	nStopSignalSign;
	int	nStopSignalSlope;
	int	blank_marker;
	int	delay_marker;
	int	period_marker;
	int	time;
	int	flag;
	int	fir;
	int	divider;
	int	max_thickness;
	int	min_thickness;
	int	defect_thickness;
	int	nConstant;
	char	spare[40];
};

struct MSCData {
	int nStructSize;
	int nAddrs;
	int nControlRegister;
	char Spare[8];
};

struct	DSP200Data {
	int				StructSize;
	int				nAddrs;
	int				nIrq;
	int				IntGate;
	int				TriggerMode[8];
	int				Delay[8];
	int				Width[8];
	int				InterfaceThreshold[8];
	int				InterfaceSlope[8];
	int				SignDetect[8];
	int				InterfaceGate;
	int				InterfacePos;
	int				TimsTriggerMode[2];
	int				TimsStartSlope[2];
	int				TimsStopSlope[2];
	int				TimsStartPos[2];
	int				TimsStopPos[2];
	unsigned int	Prf;
	int				ClkRate;
	int				Enabled;
	BOOLEAN			bInterfaceStatus;
	int				TimsMeasureMode[2];
	int				nPrfMode;
	int				nMuxMode;
	int				nTimeSlot;
	int				nIrqCount;
	int				ErrorCount;
	unsigned char	Vv[30];
	void			(*IrqRoutine)();
	BOOLEAN			bPlayBack;
	int				nCardType;					//0 Non Mux, 1 Mux
	int				nPhase;
	char			spare[8];
};

struct	ADC200Data {
	int	nStructSize;

	int			nDelay;		//Delay from trig in ns
	int			nWidth;		//Width of trace in ns
	int			nGain;
	int			nOffset;

	int			nHardwareDelay;
	unsigned	nAcqLength;		//Number of samples in a trace
	float		fSamplePeriod;  //Time for each sample 10 20 40 80 ns
	int			nSamplePeriod;  //Time for each sample 10 20 40 80 ns
	int			nConversionRate;

	int	present;
	int	nAddrs;
	int	nMemoryAddrs;
	int	irq;
	int	control_reg;
	int	nTimeslot;
	int	nConvertTimeslot;
	int	prf_source;
	int	irq_enable;
	int	mux_run_mode;
	int	convert_enable;
	int	nTriggerSource;
	int	nTriggerSlope;
	int	rf_source;
	int	blank_marker;
	int	delay_marker;
	int	gate_marker;
	int	interface_marker;
	unsigned	pre_trigger;
	unsigned	blanking;
	unsigned	tes_delay;
	int	interface_threshold;
	int	prf;
	char	serial_number[8];
	int	type;
	int	config;
	char	mod_record[4];
	char	tester[8];
	char	test_date[10];
	int		nMemorySize;
	int		nMemoryWindowSize;
	void	*pBscanArray;
	BOOLEAN	bBscanCollectData;
	int		nBscanNumber;
	int		nBscanArraySize;
	int		nBscanMode;
	int		nBscanPreTrigger;
	int		nBscanPostTrigger;
	int		nBscanLength;
	int		nPurgeFifo;
	BOOLEAN	bGraticule;
	BOOLEAN	bText;
	BOOLEAN	bRectify;
	int		nCardType;
	int		nRfType;
	int		nPR15ADCInput;
	int		nPR20ADCInput;
	int		nPR50ADCInput;
	int		nPR30ADCInput;
	int		nPL15ADCInput;
	int		nAverages;

	int		nClkDivisor;	//Maximum frequency / divisor
	int		nSpare[9];
};

/*
struct GateStruct
{
	BOOLEAN GateOnOff;
	int GateType;
	//Measurement
	//0: Amplitude 1: Amplitude + time 2: Phase 3: Image Processed from others gates
	BOOLEAN FlagMainBangInterface;
	int GateDelayNanosec;
	int GateWidthNanosec;
	int GateAmplitudePourcent; //millivolt better?
	BOOLEAN GateSlope;
	int FlagPhase;
	BOOLEAN FlagAmplitude;
	BOOLEAN FlagTime;
	BYTE* ImageAcqBytePtr[3];  //0: Amplitude 1:Time 2:Phase
};
*/

/*
struct GateStruct
{
	BOOLEAN GateOnOff;
	int GateType;
	//Measurement
	//0: Amplitude 1: Amplitude + time 2: Phase 3: Image Processed from others gates
	BOOLEAN FlagMainBangInterface;
	int GateDelayNanosec;
	int GateWidthNanosec;
	int GateAmplitudePourcent; //millivolt better?
	BOOLEAN GateSlope;
	int FlagPhase;
	BOOLEAN FlagAmplitude;
	BOOLEAN FlagTime;
	BYTE* ImageAcqBytePtr[3];  //0: Amplitude 1:Time 2:Phase
}
*/

struct	GatesData {
	int nStructSize;
	int	nGateMode[8];
	int	nNsDelay[8];
	int	nNsWidth[8];
	int	nSampleDelay[8];
	int	nSampleWidth[8];
	int	nThreshold[8];
	int	nDefectThreshold[8];
	int	nSignDetect[8];
	int	nTimsDetectionMode[8];
	int	nCollectAmp[8];
	int	nCollectTime[8];
	int	nCollectPhase[8];
	int	nTOFStartGate[8];
	int	nTOFStopGate[8];
	WCHAR cName[8][60];

	int	nAmplitude[8];
	int	nTimeSample[8];
	int	nPhase[8];

	int	nNsTimeBaseWidth;	//Used for B scan processing

	int nNsFarDeadZone[8]; //Elastic gate
	int	nTimeStatus[8];

	int	nNsPeakWidth;		//Yes only 1. Why would you want more
	
	int	nSpare[24];
};

struct Up15Data {
	int nStructSize;
	int	nPrf;
	int nCount;
	BOOLEAN bIsrFlag;
	int	nSpare[6];
};

struct	ExtSource {
	int nStructSize;

	BOOLEAN bCollect[8];
	unsigned char cGain[8];
};



struct	UtGlobalData {
	int	nStructSize;
	int	nPrf;
	int	nTimeSlot;
	int	nTriggerSource;
	WCHAR cChannelName[64][60];
	struct	MSCData	Msc;
	struct	MuxData	Mux;
	struct	LSAData LSA;
	struct	ExtSource	Ext;
};


struct	TimeSlotData {
	int		nStructSize;
	int		nAmplifier;
	int		nIrqCount;
	int		nTx;
	int		nRx;
	WCHAR	cName[60];
	struct	PR2050Data	Pr20;
	struct	PR2050Data	Pr50;
	struct	Pr15Data	Pr15;
	struct	Pl15Data	Pl15;
	struct	ADC200Data	Adc;
	struct	DSP200Data	Dsp;
	struct	GatesData	Gate;
	struct	AtpGpdData	Gpd[2];
	struct	AtpTimsData	Tims[2];
	struct	Pr30Data	Pr30;
	struct	Up15Data	Up15;

	int		nScanLine;
	int		nSpare[89];
};

//	Return status codes

enum USL_ERRORS
{
	USL_OK = 0,
	USL_UNRECOGNISED_CMD,				// Unrecognised command
	USL_NO_CMD_PARAMS,				// Command does not have required number of parameters
	USL_NO_OUTPUT_ROOM,				// No room in output buffer
	USL_NO_INTERRUPT,					// IRQ_CONNECT: No interrupt resource given
	USL_ALREADY_CONNECTED_TO_INT,		// IRQ_CONNECT: Already connected to interrupt
	USL_NOT_IN_RANGE,					// IRQ_CONNECT: Interrupt register not in range
	USL_BAD_INTERRUPT_VALUE,			// IRQ_CONNECT: Impossible to get interrupt value with specified mask
	USL_CANNOT_CONNECT_TO_INTERRUPT,	// IRQ_CONNECT: cannot connect to the given interrupt
	USL_CANNOT_RW_NEXT,				// USL_WRITE_NEXT or USL_READ_NEXT: Cannot use in IOCTL_USL_RUN_CMDS call
	USL_NO_DATA_LEFT_TO_TRANSFER,		// USL_WRITE_NEXT or USL_READ_NEXT: No data left to transfer
	USL_DELAY_TOO_LONG,				// Delay must be 60us or smaller
	USL_CANCELLED,					// Command processing stopped as IRP cancelled
	USL_BYTE_CMDS_ONLY,				// Only BYTE/UCHAR size commands are currently supported
};

/////////////////////////////////////////////////////////////////////////////
//	Command codes	reg:	1 byte Offset into address space
//					Value	1/2/4 byte value according to top 2 bits
//					count:	1 byte count
//					seconds:IRQ based read or write timeout

//const UCHAR USL_UCHAR = 0x00;
//const UCHAR USL_UWORD = 0x40;	// Not implemented yet
//const UCHAR USL_ULONG = 0x80;	// Not implemented yet

enum USL_CMDS
{
	USL_OR = 0,		// reg,Value				Use to set bit(s)
	USL_AND,			// reg,Value				Use to clear bit(s)
	USL_XOR,			// reg,Value				Use to toggle bit(s)

	USL_WRITE,		// reg,Value
	USL_READ,			// reg				Value

	USL_DELAY,		// delay					1us units, delay<=60us

	USL_WRITES,		// reg,count,Values,delay	Write values to same reg with delay
	USL_READS,		// reg,count,delay	Values	Read values from same reg with delay

	USL_IRQ_CONNECT,	// reg,mask,Value			Connect to interrupt
						//							on interrupt:	reg read
						//											anded with mask
						//											if equals value then it's ours
						// Usually last cmd in a buffer to make next cmds synchronised

	USL_TIMEOUT,		// seconds					Specify timeout for reads and writes
	USL_WRITE_NEXT,	// reg						Write next value from write buffer
	USL_READ_NEXT,	// reg						Read next value into read buffer
};

#define IOCTL_USL_RUN_CMDS CTL_CODE(                FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_USL_CMDS_FOR_READ CTL_CODE(           FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_USL_CMDS_FOR_READ_START CTL_CODE(     FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_USL_CMDS_FOR_WRITE CTL_CODE(          FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_USL_GET_RW_RESULTS CTL_CODE(          FILE_DEVICE_UNKNOWN, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_USL_GET_VERSION CTL_CODE(             FILE_DEVICE_UNKNOWN, 0x806, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_USL_SHARE_TIMESLOT_STRUCT CTL_CODE(   FILE_DEVICE_UNKNOWN, 0x807,	METHOD_NEITHER,	 FILE_ANY_ACCESS)

#define IOCTL_USL_FREE_TIMESLOT_STRUCT CTL_CODE(    FILE_DEVICE_UNKNOWN, 0x808,	METHOD_NEITHER,	 FILE_ANY_ACCESS)

#define IOCTL_USL_READ_ADC200_MEMORY CTL_CODE(      FILE_DEVICE_UNKNOWN, 0x809, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_USL_SHARE_SI6_USER_STRUCT CTL_CODE(   FILE_DEVICE_UNKNOWN, 0x80A,	METHOD_NEITHER,	 FILE_ANY_ACCESS)

#define IOCTL_USL_FREE_SI6_USER_STRUCT CTL_CODE(    FILE_DEVICE_UNKNOWN, 0x80B,	METHOD_NEITHER,	 FILE_ANY_ACCESS)

#define IOCTL_USL_SHARE_SERVO_USER_STRUCT CTL_CODE( FILE_DEVICE_UNKNOWN, 0x80C,	METHOD_NEITHER,	 FILE_ANY_ACCESS)

#define IOCTL_USL_FREE_SERVO_USER_STRUCT CTL_CODE(  FILE_DEVICE_UNKNOWN, 0x80D,	METHOD_NEITHER,	 FILE_ANY_ACCESS)

#define	IOCTL_USL_SHARE_BSCAN_ARRAY CTL_CODE(		FILE_DEVICE_UNKNOWN, 0x80E,	METHOD_NEITHER,	 FILE_ANY_ACCESS)

#define	IOCTL_USL_FREE_BSCAN_ARRAY CTL_CODE(		FILE_DEVICE_UNKNOWN, 0x80F,	METHOD_NEITHER,	 FILE_ANY_ACCESS)
