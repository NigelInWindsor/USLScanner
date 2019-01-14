struct	Pr30DataV1 {
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

	int nSpare[91];
};

struct PR2050DataV1 {
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
struct	Pr15DataV1 {
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
	bool bSelfTest;
	int	nSelfTestFrequency;
	int	nSelfTestAmplitude;
	char	cVersion;
	int	nSpare[25];
};

struct	Pl15DataV1 {
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


struct	MuxDataV1 {
	int	nStructSize;
	int	nTxRxMode[8];
	int nMaxTimeslots;
	int	nAddrs;
	int	nOffset[8];
	int	nState;
	int	nIrq;
	int	nIrqAddrs;
	int	nIrqBit;
	int	nSpare[10];
	};

struct	AtpGpdDataV1 {
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


struct	AtpTimsDataV1 {
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

struct MSCDataV1 {
	int nStructSize;
	int nAddrs;
	int nControlRegister;
	char Spare[8];
};

struct	DSP200DataV1 {
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
	bool			bInterfaceStatus;
	int				TimsMeasureMode[2];
	int				nPrfMode;
	int				nMuxMode;
	int				nTimeSlot;
	int				nIrqCount;
	int				ErrorCount;
	unsigned char	Vv[30];
	void			(*IrqRoutine)();
	bool			bPlayBack;
	int				nCardType;					//0 Non Mux, 1 Mux
	int				nPhase;
	char			spare[8];
};

struct	ADC200DataV1 {
	int	nStructSize;

	unsigned	nDelay;		//Delay from trig in ns
	unsigned	nWidth;		//Width of trace in ns
	int			nGain;
	int			nOffset;

	int			nHardwareDelay;
	unsigned	nAcqLength;		//Number of samples in a trace
	float		fSamplePeriod;  //Time for each sample 10 20 40 80 ns
	int			nSamplePeriod;  //Time for each sample 10 20 40 80 ns
	int			nConversionRate;//12.5 25.0 50.0 100.0 Mhz

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
	bool	bBscanCollectData;
	int		nBscanNumber;
	int		nBscanArraySize;
	int		nBscanMode;
	int		nBscanPreTrigger;
	int		nBscanPostTrigger;
	int		nBscanLength;
	int		nPurgeFifo;
	bool	bGraticule;
	bool	bText;
	bool	bRectify;
	int		nCardType;
	int		nRfType;
	int		nPR15ADCInput;
	int		nPR20ADCInput;
	int		nPR50ADCInput;
	int		nPR30ADCInput;
	int		nPL15ADCInput;
	float	fFullScaleRange;
	float	fTriggerLevel;
	int		nSpare[9];
};

/*
struct GateStruct
{
	bool GateOnOff;
	int GateType;
	//Measurement
	//0: Amplitude 1: Amplitude + time 2: Phase 3: Image Processed from others gates
	bool FlagMainBangInterface;
	int GateDelayNanosec;
	int GateWidthNanosec;
	int GateAmplitudePourcent; //millivolt better?
	bool GateSlope;
	int FlagPhase;
	bool FlagAmplitude;
	bool FlagTime;
	BYTE* ImageAcqBytePtr[3];  //0: Amplitude 1:Time 2:Phase
};
*/

/*
struct GateStruct
{
	bool GateOnOff;
	int GateType;
	//Measurement
	//0: Amplitude 1: Amplitude + time 2: Phase 3: Image Processed from others gates
	bool FlagMainBangInterface;
	int GateDelayNanosec;
	int GateWidthNanosec;
	int GateAmplitudePourcent; //millivolt better?
	bool GateSlope;
	int FlagPhase;
	bool FlagAmplitude;
	bool FlagTime;
	BYTE* ImageAcqBytePtr[3];  //0: Amplitude 1:Time 2:Phase
}
*/

struct	GatesDataV1 {
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
	char cName[8][60];

	int	nAmplitude[8];
	int	nTimeSample[8];
	int	nPhase[8];
	int	nSpare[42];
};

struct Up15DataV1 {
	int nStructSize;
	int	nPrf;
	int nCount;
	bool bIsrFlag;
	int	nSpare[6];
};

struct	UtGlobalDataV1 {
	int	nStructSize;
	int	nPrf;
	int	nTimeSlot;
	int	nTriggerSource;
	char cChannelName[64][60];
	struct	MSCDataV1	Msc;
	struct	MuxDataV1	Mux;
	
};


struct	TimeSlotDataV1 {
	int		nStructSize;
	int		nAmplifier;
	int		nIrqCount;
	int		nTx;
	int		nRx;
	char	cName[60];
	struct	PR2050DataV1	Pr20;
	struct	PR2050DataV1	Pr50;
	struct	Pr15DataV1	Pr15;
	struct	Pl15DataV1	Pl15;
	struct	ADC200DataV1	Adc;
	struct	DSP200DataV1	Dsp;
	struct	GatesDataV1	Gate;
	struct	AtpGpdDataV1	Gpd[2];
	struct	AtpTimsDataV1	Tims[2];
	struct	Pr30DataV1	Pr30;
	struct	Up15DataV1	Up15;

	int		nSpare[90];
};

//////////////////////////////////////////////////////////////////////////////
//
// Complex Geometry structures
//
/////////////////////////////////////////////////////////////////////////////
struct PR2050DataCG {
	int StructSize;
	int nAddrs;
	int nAttenuation;
	int nFilter;
	int nRectification;
	int nPulseVoltage;
	int nPrf;
	int nType;
	int nRfChannel;
};
struct	Pr15DataCG {
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
	int	nTvgEnable;
	int	tvg_clk;
	int	tvg_length;
	int	tvg[64];
	int	nTvgTriggerMode;
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
	bool bSelfTest;
	int	nSelfTestFrequency;
	int	nSelfTestAmplitude;
	char	cVersion;
	char	spare[25];
};

struct	Pl15DataCG {
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
	int	nMuxReset;
	int	mux_sync_pulse_1;
	int	mux_sync_pulse_2;
	int	nMuxPulseVoltage;
	int	mux_max_timeslot;
	int	tvg_shape;
	int	prf_out_polarity;
	int	ts0_reset_src;
	bool bSelfTest;
	int	nSelfTestFrequency;
	int	nSelfTestAmplitude;
	char cVersion;
	char cSpare[4];
};


struct	MuxDataCG {
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

struct	AtpGpdDataCG {
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


struct	AtpTimsDataCG {
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

struct MSCDataCG {
	int nStructSize;
	int nAddrs;
	int nControlRegister;
	char Spare[8];
};

struct	DSP200DataCG {
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
	bool			bInterfaceStatus;
	int				TimsMeasureMode[2];
	int				nPrfMode;
	int				nMuxMode;
	int				nTimeSlot;
	int				nIrqCount;
	int				ErrorCount;
	unsigned char	Vv[30];
	void			(*IrqRoutine)();
	int				nScratch;
	int				nCardType;					//0 Non Mux, 1 Mux
	char			spare[12];
};

struct	ADC200DataCG {
	int	nStructSize;

	unsigned	nDelay;		//Delay from trig in ns
	unsigned	nWidth;		//Width of trace in ns
	int			nGain;
	int			nOffset;

	int			nHardwareDelay;
	unsigned	nAcqLength;		//Number of samples in a trace
	int			nSamplePeriod;  //Time for each sample 10 20 40 80 ns
	int			nConversionRate;//12.5 25.0 50.0 100.0 Mhz

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
	int	trigger_source;
	int	trigger_slope;
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
	bool	bBscanCollectData;
	int		nBscanNumber;
	int		nBscanArraySize;
	int		nBscanMode;
	int		nBscanPreTrigger;
	int		nBscanPostTrigger;
	int		nBscanLength;
	int		nPurgeFifo;
	bool	bGraticule;
	bool	bText;
	bool	bRectify;
	int		nCardType;
	char	spare[16];
};


struct	GatesDataCG {
	int nStructSize;
	int	nTimeBaseDelay;
	int	nTimeBaseWidth;
	int	nMaxTimeBase;
	int	nTriggerMode[8];
	int	nDelay[8];
	int	nWidth[8];
	int	nThreshold[8];
	int	nSignDetect[8];
	int	nAmplitude[8];
	int	nInterfacePosition;
	int	nTimsMode[2];
	int	nTimsDetectionMode[2];
	int nTimsStartSlope[2];
	int nTimsStopSlope[2];
	int nCollectAmp[8];
	int	nCollectTime[2];
};


struct	UtGlobalDataCG {
	int	nStructSize;
	int	nPrf;
	int	nTimeSlot;
	int	nTriggerSource;
	struct	MSCData	Msc;
	struct	MuxData	Mux;
	int	nSpare[10];
};

struct	TimeSlotDataCG {
	int		nStructSize;
	int		nAmplifier;
	int		nTransmitter;
	int		nReceiver;
	int		nIrqCount;
	struct	PR2050DataCG	Pr20;
	struct	PR2050DataCG	Pr50;
	struct	Pr15DataCG		Pr15;
	struct	Pl15DataCG		Pl15;
	struct	ADC200DataCG	Adc;
	struct	DSP200DataCG	Dsp;
	struct	GatesDataCG		Gate;
	struct	AtpGpdDataCG	Gpd[2];
	struct	AtpTimsDataCG	Tims[2];
	int		nSpare[10];
};
//////////////////////////////////////////////////////////////////////////////
//
// End of Complex Geometry structures
//
/////////////////////////////////////////////////////////////////////////////
