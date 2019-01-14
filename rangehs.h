#ifndef RANGEHS_H
#define RANGEHS_H

typedef DWORD HSIF_RESULT;
typedef DWORD HSIF_HANDLE;

#define HSIF_OVERFLOW	2
#define HSIF_SUCCESS	1
#define HSIF_FAIL		0


#define HSIF_BUFSIZE			4096	// number of samples the board can buffer
#define HSIF_INVALID_HANDLE		(-1)
#define HSIF_DATA_AVAILABLE		1
#define DEFAULT_MAX_RANGE		650		// 4000's default max range in inches 

#define ENCODER1	0x01
#define ENCODER2	0x02


// HS_RANGE_DATA, this structure is used internally for calibration
typedef struct
{
	DWORD	range;
	BYTE	amplitude;
	BYTE	ambient;
	BYTE	temperature;
	BYTE	pad;
} HS_RANGE_DATA;


//----------------------------------------------------------------------------------------------
//
//	Definitions of DLL interface functions
//
//----------------------------------------------------------------------------------------------
//
//	DLL General Information:
//
//	Initialization/Close and Multiple Cards
//
//	The DLL will find all instances of cards on Init and add them to its pool of resources.  
//	DLL will sense when a thread or process attaches to it and will make a note of it for 
//	assigning resources to processes or threads.  DLL will auto sense when a thread or process 
//	terminates, and will close resources allocated to it.  Resolution information will be kept 
//	internally by the DLL.  It will be updated from the default values when the user changes 
//	it through HsifSetSamplePeriod(); and HsifSetMaxRange(); 
//
//	Interrupts are set to go at half full. 
//
//	To reset, or initialize an interface, it is best to close then reopen the device.
//
//	Functions that require a comport handle should use the one returned by the utility 
//	OpenPort found in sio_util.c
//
//	The function HsifSetSamplePeriod(); should be used to set the AR4000 sample rate 
//	and max range. This will allow the calibrated value to be calculated by 
//	HsifProcessSamples(); properly.
//
//

//----------------------------------------------------------------------------------------------
//
//	MACROS
//
//----------------------------------------------------------------------------------------------
//
//	Macros for accessing data fields in interface data, other than straight
//	structure elements such as amplitude, ambient light. Encoder values may be
//	read directly, or the macros below, which update a higher order value,
//	may be used.
//

#define HS_AMP(d)	((d>>24)&0xff)
#define HS_AMB(d)	((d>>16)&0xff)
#define HS_TEMP(d)  ((d>>8)&0xff)
#define HS_STAT(d)	(d&0xff)


//	Raw sample directly from the Hsif card
typedef struct 
{
	DWORD	status;		// Fifo and encoder index latches, temp, amb, amp
	DWORD	encoder1;	// 
	DWORD	encoder2;	// 
	DWORD	range;
} HSIF_SAMPLE;


#define DATA_LOST(X) (((X).status) & 0x01)

#define INPUT1(X) (((X).status) & 0x4)
#define INPUT2(X) (((X).status) & 0x8)
#define INPUT3(X) (((X).status) & 0x2)

//	Processed, or calibrated, data sample
typedef struct
{
	USHORT	status;		// Fifo and encorer index latches, same as HSIF_SAMPLE
	double	angle1;		// In radians from offset entered by HsifSetEncoderCalibration
	double	angle2;		// In radians from offset entered by HsifSetEncoderCalibration
	double	distance;	// calbrated distance
	double	caltemp;		// calibrated temperature, degrees F.
	double	ambient;	// background illumination  same as HSIF_SAMPLE
	double	amplitude;	// reflected signal strength same as HSIF_SAMPLE
	BOOL	timeout;	// Will fold into Status on next rev
	DWORD	rawRange;	// raw range after conversion from CC and FC to an integer value
} HSIF_PROC_SAMPLE;


//----------------------------------------------------------------------------------------------
//	High Speed Interface PCI card API.
//
//	HSIF Acuity proprietary DLL routines.
//
//----------------------------------------------------------------------------------------------
//
// Function: HsifDllInit();
// 
// Description:
//
//	First time DLL is called, need to initialize internal structures
//	This will be called automatically by windows on startup.. but
//	for simulation when not part of a dll, we must call it explicitly
//	in the test program
//
// Returns:
//	
//	TRUE		- Successful initialization
//	FALSE		- Initialization failure
//

BOOL HsifDllInit();

#define CHECKHANDLE(h) if(h < 0 || h > MAX4000BOARDS || !(handles[h].alloc) || !(handles[h].hsifhandle)) return(HSIF_FAIL)
#define CHECKIOCTL if (!fSuccess) return(HSIF_FAIL); else return(HSIF_SUCCESS)


// Function: HsifOpen
// 
// Description:
//
//	Open communication with PCI HSIF card.  HSIF cards are enumerated 1,2,3, 4 as 
//	they are found when the DLL is first loaded.  If hsifNum is 0, 
//	the first available HSIF device HANDLE is returned.
//	
//	If a serial port is dedicated to the AR4000, a handle to the opened com device can be included.
//	The sample period and max range are set to their default values. 
//
//	If DLL calls that communicate with an AR4000 are to be used, a serial port should be opened with 
//	OpenPort (see sio_util.h) prior to HsifOpen, and the handle returned passed in.
//
// Returns:
//	
//	HANDLE				- handle to hsifCard resource
//	HSIF_INVALID_HANDLE	- invalid handle, could
//
HSIF_HANDLE HsifOpen( DWORD hsifNum, HANDLE comHandle );


// Function: HsifClose
//
// Description:  
//
//  Close process's access to a HSIF card.  
//  Calling with an invalid handle is not destructive
//
// Returns:
//
//	HSIF_SUCCESS	- Card closed successfully, resources freed.
//	HSIF_FAIL		- Card failed to close.
//
HSIF_RESULT HsifClose( HSIF_HANDLE hsifCard );


// Function: HsifSamplingModeInit
//
// Description:
//  
//  Set HSIF card to sampling mode.  This must be done before samples can be captured.
//
// Returns
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifSamplingModeInit( HSIF_HANDLE hsifCard );







// 
// Function: HsifResetBoard
//
//
// Description:
//  
//  Resets the board to its power up state: Buffer empty, motors off, sampling disabled.
//  
//
// Returns:
//
//	HSIF_SUCCESS	- Card successfully reset
//	HSIF_FAIL		- Card reset failure
//



HSIF_RESULT HsifResetBoard( HSIF_HANDLE hsifCard);


// 
// Function: HsifProcessSamples
//
//
// Description:
//  
//  Converts raw sample data to calibrated range data.  
//	SampleBuf and procSampleBuf must contain numSamples number of elements.  
//	Converts distance to range in inches.
//	Converts temperature value to calibrate degrees F.
//
// Notes:
//
//	If a timeout occurred during a sample (no range signal for 13ms)
//	the timeout flag will be set to TRUE for that sample.  This
//	can be used to detect whether the AR-4000 range cable is unplugged.
//
// Returns:
//
//	HSIF_OVERFLOW	- Set if a hardware overflow occurred during one of the samples
//	HSIF_SUCCESS	- Read up to numSamples successfully
//	HSIF_FAIL		- Read failure, possible bad HANDLE, or card not responding.
//

HSIF_RESULT HsifProcessSamples( HSIF_HANDLE hsifCard, HSIF_SAMPLE * sampleBuf, 
							   HSIF_PROC_SAMPLE * procSampleBuf, DWORD numSamples );

// 
// Function: HsifLoadCalibrationData
//
//
// Description:
//  
//	Loads calibration data file for a particular AR4000 sensor to be used with ProcessSamples 
//	for generating true distance readings.
//  
//
// Returns:
//
//	HSIF_SUCCESS	- Read up to numSamples successfully
//	HSIF_FAIL	- Read failure, possible bad HANDLE, or card not responding.
//

BOOL HsifLoadCalibrationData( HSIF_HANDLE hsifCard, LPCSTR filename );


// 
// Function: HsifSetSamplePeriod
//
//
// Description:
//  
//	Set sample period and max range. This command writes to the AR4000 via the COM port.  
//	If a valid comHandle is specified, the function will send the appropriate commands 
//	to the AR4000 to set the sample period over the serial port. 
//	If comHandle is set to HSIF_INVALID_HANDLE, the function assumes that the user has set up the 
//	AR4000 already and no attempts to access it over a serial port will be made.
//
//
// Returns:
//
//	HSIF_WARN_COM	- Sample period updated internally, but an invalid com handle was specified 
//					  so no communication with the AR4000 through the serial port was attempted.
//	HSIF_SUCCESS	- Set sample period and range successfully
//	HSIF_FAIL		- Failed to set sample and range
//

HSIF_RESULT HsifSetSamplePeriod( HSIF_HANDLE hsifCard, HANDLE comHandle, DWORD range, DWORD samplePeriod );

// 
// Function: HsifLaserOn 
//
// Description:
//  
//	Turn laser on.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//

HSIF_RESULT HsifLaserOn( HSIF_HANDLE hsifCard );


// 
// Function: HsifLaserOff
//
// Description:
//  
//	Turn laser off.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//

HSIF_RESULT HsifLaserOff( HSIF_HANDLE hsifCard );


// 
// Function: HsifSamplingEnable
//
// Description:
//  
//	Enable Sampling.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifSamplingEnable( HSIF_HANDLE hsifCard );


// 
// Function: HsifSamplingDisable
//
// Description:
//  
//	Disable Sampling.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifSamplingDisable( HSIF_HANDLE hsifCard );


// 
// Function: HsifClearOverflow
//
// Description:
//  
//	Clears the hardware and software buffer overflow flags.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//

HSIF_RESULT HsifClearOverflow( HSIF_HANDLE hsifCard );


// 
// Function: HsifClearSampleBuffer
//
// Description:
//  
//	Clears all buffered samples that have been taken but not yet read by the application.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifClearSampleBuffer( HSIF_HANDLE hsifCard );


// 
// Function: HsifDataAvailable
//
// Description:
//  
//	Tests the board and buffered samples and returns the number of samples currently
//	available.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifDataAvailable( HSIF_HANDLE hsifCard, DWORD * numSamplesAvailable );


// 
// Function: HsifGetOverflowStatus
//
// Description:
//  
//	Gets the buffer overflow status for samples.  If overflowStatus is TRUE, 
//	the application isn't reading samples out of the buffer fast enough.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifGetOverflowStatus( HSIF_HANDLE hsifCard, BOOL * overflowStatus);

// 
// Function: HsifSetMotorPower
//
// Description:
//  
//	Sets motor 1 or 2 power to a value between 0 (off) and 255 (full power).
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifSetMotorPower( HSIF_HANDLE hsifCard, DWORD motorNum, DWORD power );


// 
// Function: HsifClearEncoder
//
// Description:
//  
//	Clear either one or both encoders.  The encoderMask can be set to ENCODER1 to clear encoder 1,
//	ENCODER2 to clear encoder 2, or (ENCODER1 | ENCODER2) to clear both encoders.  The clear 
//	will take place either when the index pulse occurs if withIndex is TRUE, or immediately 
//	if withIndex is FALSE.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifClearEncoder( HSIF_HANDLE hsifCard , DWORD encoderMask, BOOL withIndex);

// 
// Function: HsifCalibrateEncoder
//
// Description:
//  
//	Set up a count offset and counts per revolution to define the basis for calculating motor angles
//	in HsifProcessSamples(). 
//
//		Motor angle = (encodercount - offset)/countsPerRev * 2 * pi
//
//	Motor angle has the units of radians.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifCalibrateEncoder( HSIF_HANDLE hsifCard, DWORD encoder, DWORD offset, DWORD countsPerRev);



// 
// Function: HsifCalibrate 
//
//
// Description:
//  
//	Perform an internal HSIF card calibration.  
//	This must be run before the card begins taking samples.
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifCalibrate( HSIF_HANDLE hsifCard);


// 
// Function: HsifGetBufferedSamples
//
//
// Description:
//  
//	Get up to numSamples raw samples from an HSIF card.  
//  
//	If wait is TRUE, the function will block until numSamples are read.
//	If wait is FALSE, the function will return immediately with as many samples that are available 
//	up to numSamples.  The number actually read is set in numRead.
//
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
//	numRead = number of samples actually read
//
HSIF_RESULT HsifGetBufferedSamples( HSIF_HANDLE hsifCard, HSIF_SAMPLE * sampleBuf, DWORD numSamples, DWORD *numRead, BOOL wait );

// 
// Function: HsifSetPollMode
//
//
// Description:
//  
//	Enable Poll mode for slow sample speeds if mode = TRUE.  
//	In general, the user should always set this to TRUE.  If set
//	to FALSE, there will be a latency of 2048 samples between
//	transfers from the HSIF card buffer to the PC's local memory.
//
//
// Returns:
//
//	HSIF_SUCCESS	- Success
//	HSIF_FAIL		- Failure
//
HSIF_RESULT HsifSetPollMode( HSIF_HANDLE hsifCard , BOOL mode);



#endif // RANGEHS_H
