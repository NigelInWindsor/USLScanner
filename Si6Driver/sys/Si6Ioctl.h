//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
/////////////////////////////////////////////////////////////////////////////
//	ioctl.cpp:		DeviceIoControl IOCTL codes supported by PHD Io drivers
/////////////////////////////////////////////////////////////////////////////
enum AmplifierType {
	AMPLIFIER_NOT_KNOWN = 0,
	Kollmorgen,
	Copley,
	Staubli,
	GeoBrick,
};

#define	SI6_VERSION		"SI6 V2.00 December 8th 2006"

#define bool BOOLEAN

//Common Structures
#define	SI6_CLK_PORT			0
#define	SI6_DIR_PORT			1
#define	SI6_CLK_ENABLE			2
#define	SI6_INT_CLR				3
#define	SI6_PIC					4
#define	SI6_COUNT_0				8
#define	SI6_COUNT_1				9
#define	SI6_COUNT_2				0x0a
#define	SI6_COUNT_3				0x0c
#define	SI6_COUNT_4				0x0d
#define	SI6_COUNT_5				0x0e
#define	SI6_CNTRL_012			0x0b
#define	SI6_CNTRL_345			0x0f
#define	SI6_JOYSTICK			0x10
#define	SI6_PIA					0x14
#define	SI6_ENCODER_SELECT		0x18
#define	SI6_ENCODER_MODE		0x19

#define	PIC_CNTRL				0x20
#define	PIC_CNTRL_BYTE			0x20
#define	PIC_MASK				0x21
#define	PIC2_CNTRL				0xa0
#define	PIC2_CNTRL_BYTE			0xa0
#define	PIC2_MASK				0xa1
#define	PIC_NOP					0x40
#define	PIC_EOI					0x20

#define	X0AXIS					0
#define	Y0AXIS					1
#define	Z0AXIS					2
#define	R0AXIS					3
#define	XT0AXIS					4
#define	YT0AXIS					5
#define	X1AXIS					6
#define	Y1AXIS					7
#define	Z1AXIS					8
#define	R1AXIS					9
#define	XT1AXIS					10
#define	YT1AXIS					11

#define	MOVETO					1
#define	HOMETO					2
#define	JOYSTICK				3
#define CONTINUOUS_ROTATE		4

#define	DISABLE					0
#define	ENABLE					1

#define	STEPPER					0
#define	SERVO					1
#define	SERVO_TURBO				2
#define	SI10					3
#define	FIELD_BUS				4
#define	SAFETY_UNIT				5


#define	MIN_TRAVEL				0x01
#define	MAX_TRAVEL				0x02
#define	MIN_SWITCH				0x04
#define	MAX_SWITCH				0x08
#define	HOME_SWITCH				0x10

#define AXISNAMELENGHT_CHAR			20
#define AXISNAMELENGHT				10
#define AXISUNITLENGHT				12
#define MAXIMUMNUMBERMOTORCONTROLED 10
#define MAXUNITNUMBER 3

struct	SI6Data {
	int		nStructSize;
	int		nAddrs;
	int		nClkEnable;
	int		nDirReg;
	int		nIrq;
	int		nIrqAction[8];
	struct	AxisData	*pAxis[8];
	int		nDirEor;
	int		nIrqMask;
	int		nEncoderMode;
	int		nIrqCount;
	int		nProcessState;
	int		nEcho;
	int		nJoystickPercent;
	int		nSwitchEor;
	struct	AxisData	*fAxis;
	struct	AxisData	*sAxis;
	int		nRevCounter;
	int		nLimitSwitch;				//Updated by ISR

	//Collect Data
	bool	bCollectData;
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
	int		nJoystickMode;
	int		nProcessCard;
	int		nJoystickSource;

	int		nCommsJoystickValue[5];		//Comms joystick
	int		nCommsSwitchValue;
	int		nNumberTimeSlots;

	int		nExternalInputNumber;
	int		nMinExternalValue;
	int		nMaxExternalValue;

	char	spare[64];
};


struct	AxisData 
{
	int		nStructSize;

	int		nMotorType;									//Stepper, Servo

	bool    bAvailable;								// Use Axis or not	
	WCHAR	cName[AXISNAMELENGHT];					//Axis Name
	bool	bDirectionReverse;						// Axis Direction

	//Units
	char	Units[MAXUNITNUMBER][AXISNAMELENGHT_CHAR];	// Axis Units Name
	int		nUnitSelected;							// Current Unit Selected
	float	fStepSize;								// Unit per Step (Encoder or Motor Pulses)
	int		nPrecision;								// Unit Display Precision
	int		nWidth;									// Unit Display Width

	//Motor Control
	int		nCounter;								// Si6 Clock Number
	int		nBit;									// Si6 Bit used to Pulse Motors
	int		nIrqMask;								// 
	int		nProcess;								// Monitor 
	int		nSoftErr;  //Folowing Error Soft
	int		nHardErr;  		
	int		nPositionError;
	int		nIrqCount;								//Diagnose: Number of Interupts received by the Si6

	//Motor Position
	int		nCount;	  								// Pulses Sent to Motor (For Position)
	int		nPos;									// Actual Position (Encoder or Motor Pulses)
	int		nDesirePos;								// Position Desired (Encoder or Motor Pulses)
	float 	fEncMotorRatio;							// To convert from Encoder to Pulses
	int		nStepsPerRev;							// If rotation table (Steps per Revolution)
	int		nStartPos;								// Start position of scan
	int		nFinishPos;								// Finish position of scan
	float	fDiameter;

	//Motor Speed and Acceleration
	int		nAcceleration;							// Motor Acceleration (2 or 3)
	int		nDeceleration;							// Motor Deceleration (2 or 3)
	int		nMinSpeed;								// Minimum Speed
	int		nMaxSpeed;								// Maximum Speed
	int		nCurrentSpeed;							// Current Speed
	int		nReqSpeed;								// Speed Requested
	int		nHomeSpeed;

	//Encoder Stuff
	int		nEncoderNumber;							// Encoder Number Used
	int		nEncoderMode;			//???  //Si6 
	bool	nEncoderResetMode;
	int		nEncoderValue;
	bool	bEncoderReverse;		//Direction of the Encode

	//Limit Travel
	int		nFlagMinMax;							// Bit Activated: 0: Min Soft Limit 1 : Max Soft Limit 2: Min Hardware Limit 3: Max Hardware Limit
	int		nMaxTravel;								// Software Max Limit
	int		nMinTravel;								// Software Min Limit
	int		nHomePos;								// Home state machine
	int		nMinSwitch;								// Si6 Number of the Min Limit Switch
	int		nMaxSwitch;								// Si6 Number of the Max Limit Switch							
	int		nHomeSwitch;							// Si6 Number of the Home Limit Switch
	int		nHomeMode;				//??
	bool 	bHomeFlag;				//Is the motor at home?

	//Joystick//////////////
	bool	bJoystickAvailable;						// Joystick Activated 
	int		nJoystickNumber;						// Si6 Joystick number
	int		nJoystickMaxSpeed;						// Joystick Max Speed
	int		nJoystickMinSpeed;						// Joystick Min Speed
	int		nJoystickZero;							// Joystick Zero Value
	int		nJoystickMinValue;						// Joystick Minimum Value
	int		nJoystickMaxValue;						// Joystick Maximum Value
	bool	bJoystickReverse;						// Joystick Direction
	int     nJoystickValueRead;						// Joystick Current Value Read (Between 0 and 255)
	int		nJoystickSwitchPos;						// Joystick position of toggle switch
	int		nJoystickGain;

	int		nEncMotorRatioNumerator;
	int		nEncMotorRatioDenominator;

	int		nHomeAction;							//Home state machine

	//Pmac Stuff
	int		nOffset;								
	int		nLimitSwitchOffset;						//Offset from limit switch
	float	fMaxJogSpeed;
	float	fAcceleration;
	float	fHomeSpeed;
	bool	bEnabled;
	int		nStatus;
	int		nPhysicalAxis;
	bool	bMustHome;
	float	fMaxMoveSpeed;

	int		nRLdiff;
	int		nHomeFoundAt;
	float	fMinKin;
	float	fMaxKin;
	int		nModuloMode;

	float	fParkPosition;
	int		nRevCounter;

	int		nDriverAxisNumber;

	float	fTotalDistance;
	float	fDistanceAtLastService;

	bool	bUseFiducial;
	int		nLatchContents;
	int		nFollowingError;

	int		nSlowIncrement;	//in thousands of a step;
	int		nDesireRevCount;

	float	fNotchFrequency;
	float	fNotchDamping;
	float	fBandPassFrequency;
	float	fBandPassDamping;

	int		nDAC;
	float	fJoystickAcceleration;

	int		nLastPosForTotalTravel;
	int		nMacroNode;
	AmplifierType		eAmplifierType;
	double	dStepsPerRev;
	int		nAxisType;	//Linear, Rotary, Roller
	float	fMinTravel;
	float	fMaxTravel;
	float	fFatalFollowingError;
	int		nSpare[9];

};

#undef bool

//	Return status codes

enum SI6_ERRORS
{
	SI6_OK = 0,
	SI6_UNRECOGNISED_CMD,				// Unrecognised command
	SI6_NO_CMD_PARAMS,				// Command does not have required number of parameters
	SI6_NO_OUTPUT_ROOM,				// No room in output buffer
	SI6_NO_INTERRUPT,					// IRQ_CONNECT: No interrupt resource given
	SI6_ALREADY_CONNECTED_TO_INT,		// IRQ_CONNECT: Already connected to interrupt
	SI6_NOT_IN_RANGE,					// IRQ_CONNECT: Interrupt register not in range
	SI6_BAD_INTERRUPT_VALUE,			// IRQ_CONNECT: Impossible to get interrupt value with specified mask
	SI6_CANNOT_CONNECT_TO_INTERRUPT,	// IRQ_CONNECT: cannot connect to the given interrupt
	SI6_CANNOT_RW_NEXT,				// SI6_WRITE_NEXT or SI6_READ_NEXT: Cannot use in IOCTL_SI6_RUN_CMDS call
	SI6_NO_DATA_LEFT_TO_TRANSFER,		// SI6_WRITE_NEXT or SI6_READ_NEXT: No data left to transfer
	SI6_DELAY_TOO_LONG,				// Delay must be 60us or smaller
	SI6_CANCELLED,					// Command processing stopped as IRP cancelled
	SI6_BYTE_CMDS_ONLY,				// Only BYTE/UCHAR size commands are currently supported
};

/////////////////////////////////////////////////////////////////////////////
//	Command codes	reg:	1 byte Offset into address space
//					Value	1/2/4 byte value according to top 2 bits
//					count:	1 byte count
//					seconds:IRQ based read or write timeout

//const UCHAR SI6_UCHAR = 0x00;
//const UCHAR SI6_UWORD = 0x40;	// Not implemented yet
//const UCHAR SI6_ULONG = 0x80;	// Not implemented yet

enum SI6_CMDS
{
	SI6_OR = 0,		// reg,Value				Use to set bit(s)
	SI6_AND,			// reg,Value				Use to clear bit(s)
	SI6_XOR,			// reg,Value				Use to toggle bit(s)

	SI6_WRITE,		// reg,Value
	SI6_READ,			// reg				Value

	SI6_DELAY,		// delay					1us units, delay<=60us

	SI6_WRITES,		// reg,count,Values,delay	Write values to same reg with delay
	SI6_READS,		// reg,count,delay	Values	Read values from same reg with delay

	SI6_IRQ_CONNECT,	// reg,mask,Value			Connect to interrupt
						//							on interrupt:	reg read
						//											anded with mask
						//											if equals value then it's ours
						// Usually last cmd in a buffer to make next cmds synchronised

	SI6_TIMEOUT,		// seconds					Specify timeout for reads and writes
	SI6_WRITE_NEXT,	// reg						Write next value from write buffer
	SI6_READ_NEXT,	// reg						Read next value into read buffer
};

#define IOCTL_SI6_RUN_CMDS CTL_CODE(                FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_CMDS_FOR_READ CTL_CODE(           FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_CMDS_FOR_READ_START CTL_CODE(     FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_CMDS_FOR_WRITE CTL_CODE(          FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_GET_RW_RESULTS CTL_CODE(          FILE_DEVICE_UNKNOWN, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_GET_VERSION CTL_CODE(             FILE_DEVICE_UNKNOWN, 0x806, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_GET_PTR_SI6 CTL_CODE(             FILE_DEVICE_UNKNOWN, 0x807, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_GET_SI6 CTL_CODE(                 FILE_DEVICE_UNKNOWN, 0x808, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_SEND_PTR_USER_SI6 CTL_CODE(       FILE_DEVICE_UNKNOWN, 0x809, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SI6_DRIVER_SI6STRUCT_TO_USER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80A, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_USER_SI6STRUCT_TO_DRIVER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80B, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_SHARE_USER_STRUCT CTL_CODE(       FILE_DEVICE_UNKNOWN, 0x80C,	METHOD_NEITHER,	 FILE_ANY_ACCESS)

#define IOCTL_SI6_FREE_USER_STRUCT CTL_CODE(		FILE_DEVICE_UNKNOWN, 0x80D, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SI6_GET_IRQ_COUNT CTL_CODE(			FILE_DEVICE_UNKNOWN, 0x80E,	METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SI6_SHARE_DATA_ARRAY CTL_CODE(		FILE_DEVICE_UNKNOWN, 0x80F, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SI6_FREE_DATA_ARRAY CTL_CODE(			FILE_DEVICE_UNKNOWN, 0x810, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SI6_SHARE_TIMESLOT_STRUCT CTL_CODE(	FILE_DEVICE_UNKNOWN, 0x811, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SI6_FREE_TIMESLOT_STRUCT CTL_CODE(	FILE_DEVICE_UNKNOWN, 0x812, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SI6_READ_ADC200_MEMORY CTL_CODE(      FILE_DEVICE_UNKNOWN, 0x813, METHOD_BUFFERED, FILE_ANY_ACCESS)

