
#define	SI10_CLK_PORT			0x0204 //Cause trigger
#define	SI10_DIR_PORT			0x0208 //This is the stepper motor direction register for all channels
#define	SI10_CLK_ENABLE			0x01A8 //This is the clock enable register for all clocks
//#define	SI10_INT_CLR			3
//#define	SI10_PIC				4
#define	SI10_COUNT_0			0x0180
#define	SI10_COUNT_1			0x0184
#define	SI10_COUNT_2			0x0188
#define	SI10_COUNT_3			0x018C
#define	SI10_COUNT_4			0x0190
#define	SI10_COUNT_5			0x0194
#define	SI10_COUNT_6			0x0198
#define	SI10_COUNT_7			0x019C
#define	SI10_COUNT_8			0x01A0
#define	SI10_COUNT_9			0x01A4

#define SI10_ENCODER_0			0x0000
#define SI10_ENCODER_1			0x0004
#define SI10_ENCODER_2			0x0008
#define SI10_ENCODER_3			0x000A
#define SI10_ENCODER_4			0x000C
#define SI10_ENCODER_5			0x0010
#define SI10_ENCODER_6			0x0014
#define SI10_ENCODER_7			0x0018
#define SI10_ENCODER_8			0x001A
#define SI10_ENCODER_9			0x001C

#define SI10_COUNTER_LATCH_0	0x0040
#define SI10_COUNTER_LATCH_1	0x0044
#define SI10_COUNTER_LATCH_2	0x0048
#define SI10_COUNTER_LATCH_3	0x004A
#define SI10_COUNTER_LATCH_4	0x004C
#define SI10_COUNTER_LATCH_5	0x0050
#define SI10_COUNTER_LATCH_6	0x0054
#define SI10_COUNTER_LATCH_7	0x0058
#define SI10_COUNTER_LATCH_8	0x005A
#define SI10_COUNTER_LATCH_9	0x005C


//#define	SI10_CNTRL_012			0x0b
//#define	SI10_CNTRL_345			0x0f
#define	SI10_JOYSTICK			0x0380

#define	SI10_LIMIT_SWITCHES		0x0080

//#define	SI10_PIA					0x14
//#define	SI10_ENCODER_SELECT		0x18
//#define	SI10_ENCODER_MODE		0x19

//#define	PIC_CNTRL				0x20
//#define	PIC_CNTRL_BYTE			0x20
//#define	PIC_MASK				0x21
//#define	PIC2_CNTRL				0xa0
//#define	PIC2_CNTRL_BYTE			0xa0
//#define	PIC2_MASK				0xa1
//#define	PIC_NOP					0x40
//#define	PIC_EOI					0x20

//#define	X0AXIS					0
//#define	Y0AXIS					1
//#define	Z0AXIS					2
//#define	R0AXIS					3
//#define	XT0AXIS					4
//#define	YT0AXIS					5
//#define	X1AXIS					6
//#define	Y1AXIS					7
//#define	Z1AXIS					8
//#define	R1AXIS					9
//#define	XT1AXIS					10
//#define	YT1AXIS					11

//#define	MOVETO					1
//#define	HOMETO					2
//#define	JOYSTICK				3

//#define	DISABLE					0
//#define	ENABLE					1


//#define AXISNAMELENGHT 20
//#define AXISUNITLENGHT 12
//#define MAXIMUMNUMBERMOTORCONTROLED 10
//#define MAXUNITNUMBER 3
enum JoystickSource {
		WIRED_MSC=0,
		WIRED_SI6,
		WIRED_SERIAL,
		WIRELESS_SERIAL,
		WIRED_SI10,
	};


unsigned long	nTable[10001];

struct	SI10Data {
	int		nStructSize;
	int		nAddrs;
	int		nClkEnable;
	int		nDirReg;
	int		nIrq;
	int		nIrqAction[10];
	struct	AxisData	*pAxis[10];			//From Si6Ioctl
	int		nDirEor;
	int		nIrqMask;
	int		nEncoderMode;
	int		nIrqCount;
	int		nIrqPRFCount;
	int		nProcessState;
	int		nEcho;
	int		nSwitchEor;
	struct	AxisData	*fAxis;				//From Si6Ioctl
	struct	AxisData	*sAxis;				//From Si6Ioctl
	int		nRevCounter;
	int		nLimitSwitch;					//Updated by ISR

	//Collect Data
	BOOLEAN	bCollectData;
	int		nScanDirection;
	void	*pData;
	int		nDataSize;
	int		nDataPtr;
	int		nSamplesLine;
	int		nWhichAxis;
	int		nNumberAmpGates;
	int		nNumberTimeGates;
	char	Amp[8];
	int		Time[2];
	int		Phase;

	char	*pMyself;
	int		nJobNumber;
	int		nProcessCard;

	int		nNumberTimeSlots;

	int		nExternalInputNumber;
	int		nMinExternalValue;
	int		nMaxExternalValue;

	BOOLEAN	bGenerateEvent;

	int		nJoystickValue[5];		//Comms joystick
	int		nJoystickSwitchValue;
	int		nJoystickMode;
	int		nJoystickSpare;
	int		nJoystickSource;		//SERIAL, SI10
	int		nJoystickPercent;
	int		nJoystickLeftSideBit;
	int		nJoystickRightSideBit;
	int		nJoystickDeadManBit0;
	int		nJoystickDeadManBit1;
	int		nJoystickMainManipBit;
	int		nJoystickScannerFixtureBit;
	int		nJoystickSpeedBit;

	int		nSpare[10];
};


struct	SI10UTData {
	int		nStructSize;
	
	BOOLEAN	bCollectData;
	
	int		nSamplesLine;
	int		nNumberLines;

	int		nStartPos; //um
	int		nFinishPos; //um

	int		nScanLine;

	void	*m_cAmpBuffer[100]; //NUMBER_BUFFERS = 100
	void 	*m_nTimeBuffer[100]; //NUMBER_BUFFERS = 100

	int		nStepSize;
	int		nSamplePeriod;

	int		nAmpImages;
	int		nTimeImages;

	char	spare[100];
};