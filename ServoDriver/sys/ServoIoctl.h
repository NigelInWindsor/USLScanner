//////////////////////////////////////////////////////////////////////////////
//	Copyright Ultrasonic Sciences Ltd 2001
//
/////////////////////////////////////////////////////////////////////////////
//	ioctl.cpp:		DeviceIoControl IOCTL codes supported by PHD Io drivers
/////////////////////////////////////////////////////////////////////////////


#define	SERVO_VERSION		"Servo V1.25 August 16th 2006"

//Common Structures

#define	PMAC_IPOS			1
#define	PMAC_BREQ			2
#define	PMAC_ERROR			4
#define	PMAC_FERROR			8
#define	PMAC_HREQ			0x10
#define	PMAC_IR5			0x20
#define	PMAC_IR6			0x40
#define	PMAC_IR7			0x80
#define	PMAC_NOP			0x40
#define	PMAC_EOI			0x20
#define	PMAC_ICR			0
#define	PMAC_STATUS			2
#define	PMAC_DATA			7
#define	PMAC_PIC0			8
#define	PMAC_PIC1			9
#define	PMAC_PIC2			10
#define	PMAC_RX				2
#define	PMAC_TX				1

#define	PMAC_AXIS_OFFSET		8
#define	PMAC_POS				0x00
#define	PMAC_DESIRE_POS			0x10
#define	PMAC_JOY_DEV			0x20
#define	PMAC_MOTOR_STATUS		0x30
#define	PMAC_FOLLOWING_ERROR	0x40
#define	PMAC_VELOCITY			0x3B0
#define PMAC_PUMP_ENCODERS		0x3BC

#define	PMAC_MIN_SWITCH			0x400000
#define	PMAC_MAX_SWITCH			0x200000
#define	PMAC_IN_POS				0x1
#define	PMAC_SOFT_LIMIT			0x2
#define	PMAC_HARD_LIMIT			0x4
#define	PMAC_AMPLIFIER_FAULT	0x8
#define	PMAC_HOMED				0x400
#define	PMAC_AMPLIFIER_ENABLED	0x80000
#define	PMAC_ENCODER_FAILURE	0x10

#define	GIMBAL					0
#define	GONIO					1

#define	DONT_WAIT				0
#define	WAIT					1
#define	IN_POS					1

#define	MOVETO					1
#define	HOMETO					2
#define	JOYSTICK				3

#define	DISABLE					0
#define	ENABLE					1

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

#define	DISABLE					0
#define	ENABLE					1


#define AXISNAMELENGHT 10
#define AXISUNITLENGHT 12
#define MAXIMUMNUMBERMOTORCONTROLED 10
#define MAXUNITNUMBER 3
#define	MAXAXISNUMBER	16

struct	SERVOData {
	int		nStructSize;
	int		nAddrs;
	int		nClkEnable;
	int		nDirReg;
	int		nIrq;
	int		nIrqAction[MAXAXISNUMBER];
	struct	AxisData	*pAxis[MAXAXISNUMBER];
	int		nDirEor;
	int		nIrqMask;
	int		nEncoderMode;
	int		nIrqCount;
	int		nProcessState;
	int		nEcho;
	int		nJoystickPercent;
	bool	bJoystickEnabled;
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
	int		nNumberTimeSlots;
	int		nNumberAmpGates[8];
	int		nNumberTimeGates[8];
	int		nNumberPhaseGates[8];
	char	Amp[8][8];
	int		Time[8][2];
	int		Phase;
	bool	bContinuousRotate;

	char	*pMyself;
	int		nXJoyValue;
	int		nYJoyValue;
	int		nZJoyValue;
	int		nSJoyValue;
	int		nTJoyValue;
	int		nSwitchJoyValue;
	int		nTogglePos;
	char	spare[72];
};

/*
struct	ServoAxisData 
{
	int		nStructSize;

	int		nMemOffset;								//Offset from base addrs of M Variables
	int		nOffset;								//Offset from limit switch
	double	dMaxSpeed;
	double	dAcceleration;
	double	dDeceleration;
	bool	bEnabled;

	bool    bAvailable;								// Use Axis or not	
	char	Name[AXISNAMELENGHT];					//Axis Name
	bool	bDirectionReverse;						// Axis Direction

	//Units
	char	Units[MAXUNITNUMBER][AXISNAMELENGHT];	// Axis Units Name
	int		nUnitSelected;							// Current Unit Selected
	double	dStepSize;								// Unit per Step (Encoder or Motor Pulses)
	int		nPrecision;								// Unit Display Precision
	int		nWidth;									// Unit Display Width

	//Motor Control
	int		nCounter;								// Si6 Clock Number
	int		nBit;									// Si6 Bit used to Pulse Motors
	int		nControlMotor;							// Which Functon to use (0: No 1:Relative 2:Absolute)
	int		nProcess;								// Monitor 
	int		nSoftErr;  //Folowing Error Soft
	int		nHardErr;  		
	int		nPositionError;
	int		nIrqCount;								//Diagnose: Number of Interupts received by the Si6

	//Motor Position
	int		nCount;	  								// Pulses Sent to Motor (For Position)
	int		nPos;									// Actual Position (Encoder or Motor Pulses)
	int		nDesirePos;								// Position Desired (Encoder or Motor Pulses)
	double 	dEncmotor;								// To convert from Encoder to Pulses
	int		nStepsPerRev;							// If rotation table (Steps per Revolution)
	int		nStartPos;								// Start position of scan
	int		nFinishPos;								// Finish position of scan
	int		nRLdiff;								// Difference in position between left and right
	double	dDiameter;

	//Motor Speed and Acceleration
	int		nAcceleration;							// Motor Acceleration (2 or 3)
	int		nDeceleration;							// Motor Deceleration (2 or 3)
	int		nMinSpeed;								// Minimum Speed
	int		nMaxSpeed;								// Maximum Speed
	int		nCurrentSpeed;							// Current Speed
	int		nReqSpeed;				//Speed Requested
	int		nHomeSpeed;

	//Encoder Stuff
	int		nEncoderNumber;							// Encoder Number Used
	int		nEncoderMode;			//???  //Si6 
	bool	nEncoderResetMode;
	int		nEncoderValue;
	bool	bEncoderReverse;		//Direction of the Encode

	//Limit Travel
	int		nStatus;								// Read from PMAC Status registers
	int		nFlagMinMax;							// Bit Activated: 0: Min Soft Limit 1 : Max Soft Limit 2: Min Hardware Limit 3: Max Hardware Limit
	int		nMaxTravel;								// Software Max Limit
	int		nMinTravel;								// Software Min Limit
	int		nHomePos;								// Origin Position
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
	int		nJoystickGain;

	int		nScratch;
	char	nSpare[100];
};
*/
//	Return status codes

enum SERVO_ERRORS
{
	SERVO_OK = 0,
	SERVO_UNRECOGNISED_CMD,				// Unrecognised command
	SERVO_NO_CMD_PARAMS,				// Command does not have required number of parameters
	SERVO_NO_OUTPUT_ROOM,				// No room in output buffer
	SERVO_NO_INTERRUPT,					// IRQ_CONNECT: No interrupt resource given
	SERVO_ALREADY_CONNECTED_TO_INT,		// IRQ_CONNECT: Already connected to interrupt
	SERVO_NOT_IN_RANGE,					// IRQ_CONNECT: Interrupt register not in range
	SERVO_BAD_INTERRUPT_VALUE,			// IRQ_CONNECT: Impossible to get interrupt value with specified mask
	SERVO_CANNOT_CONNECT_TO_INTERRUPT,	// IRQ_CONNECT: cannot connect to the given interrupt
	SERVO_CANNOT_RW_NEXT,				// SERVO_WRITE_NEXT or SERVO_READ_NEXT: Cannot use in IOCTL_SERVO_RUN_CMDS call
	SERVO_NO_DATA_LEFT_TO_TRANSFER,		// SERVO_WRITE_NEXT or SERVO_READ_NEXT: No data left to transfer
	SERVO_DELAY_TOO_LONG,				// Delay must be 60us or smaller
	SERVO_CANCELLED,					// Command processing stopped as IRP cancelled
	SERVO_BYTE_CMDS_ONLY,				// Only BYTE/UCHAR size commands are currently supported
};

/////////////////////////////////////////////////////////////////////////////
//	Command codes	reg:	1 byte Offset into address space
//					Value	1/2/4 byte value according to top 2 bits
//					count:	1 byte count
//					seconds:IRQ based read or write timeout

const UCHAR SERVO_UCHAR = 0x00;
const UCHAR SERVO_UWORD = 0x40;	// Not implemented yet
const UCHAR SERVO_ULONG = 0x80;	// Not implemented yet

enum SERVO_CMDS
{
	SERVO_OR = 0,		// reg,Value				Use to set bit(s)
	SERVO_AND,			// reg,Value				Use to clear bit(s)
	SERVO_XOR,			// reg,Value				Use to toggle bit(s)

	SERVO_WRITE,		// reg,Value
	SERVO_READ,			// reg				Value

	SERVO_DELAY,		// delay					1us units, delay<=60us

	SERVO_WRITES,		// reg,count,Values,delay	Write values to same reg with delay
	SERVO_READS,		// reg,count,delay	Values	Read values from same reg with delay

	SERVO_IRQ_CONNECT,	// reg,mask,Value			Connect to interrupt
						//							on interrupt:	reg read
						//											anded with mask
						//											if equals value then it's ours
						// Usually last cmd in a buffer to make next cmds synchronised

	SERVO_TIMEOUT,		// seconds					Specify timeout for reads and writes
	SERVO_WRITE_NEXT,	// reg						Write next value from write buffer
	SERVO_READ_NEXT,	// reg						Read next value into read buffer

	SERVO_WRITE_DPRAM,	// reg,Value
	SERVO_READ_DPRAM,	// reg				Value
};

#define IOCTL_SERVO_RUN_CMDS CTL_CODE(                FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_CMDS_FOR_READ CTL_CODE(           FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_CMDS_FOR_READ_START CTL_CODE(     FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_CMDS_FOR_WRITE CTL_CODE(          FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_GET_RW_RESULTS CTL_CODE(          FILE_DEVICE_UNKNOWN, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_GET_VERSION CTL_CODE(             FILE_DEVICE_UNKNOWN, 0x806, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_GET_PTR_SERVO CTL_CODE(             FILE_DEVICE_UNKNOWN, 0x807, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_GET_SERVO CTL_CODE(                 FILE_DEVICE_UNKNOWN, 0x808, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_SEND_PTR_USER_SERVO CTL_CODE(       FILE_DEVICE_UNKNOWN, 0x809, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SERVO_DRIVER_SERVOSTRUCT_TO_USER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80A, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_USER_SERVOSTRUCT_TO_DRIVER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80B, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_SHARE_USER_STRUCT CTL_CODE(       FILE_DEVICE_UNKNOWN, 0x80C,	METHOD_NEITHER,	 FILE_ANY_ACCESS)

#define IOCTL_SERVO_FREE_USER_STRUCT CTL_CODE(		FILE_DEVICE_UNKNOWN, 0x80D, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SERVO_GET_IRQ_COUNT CTL_CODE(			FILE_DEVICE_UNKNOWN, 0x80E,	METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SERVO_SHARE_DATA_ARRAY CTL_CODE(		FILE_DEVICE_UNKNOWN, 0x80F, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SERVO_FREE_DATA_ARRAY CTL_CODE(			FILE_DEVICE_UNKNOWN, 0x810, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SERVO_SHARE_TIMESLOT_STRUCT CTL_CODE(	FILE_DEVICE_UNKNOWN, 0x811, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SERVO_FREE_TIMESLOT_STRUCT CTL_CODE(	FILE_DEVICE_UNKNOWN, 0x812, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define IOCTL_SERVO_ENABLE_INTERRUPT_CALLBACK	CTL_CODE(       FILE_DEVICE_UNKNOWN, 0x813,	METHOD_NEITHER,	 FILE_ANY_ACCESS)

