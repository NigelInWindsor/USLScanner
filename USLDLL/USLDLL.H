/********************************************************************************
   File name: DLLCode.h

   This file contains all the DLL interfacing object declarations, in this example:
   a class object, two global function object, and a global integer variable. 

   Notice: we use the same header file for compiling the .dll and the .exe (application).
   This header file defines a macro which export the target DLL objects if we are building
   a DLL, otherwise it import the DLL objects into an application which uses the DLL. If
   we define DLLDIR_EX (a preprocessor identifier), then the preprocessor define macro
   DLLDIR (a mnemonic for DLL, import/export, Direction) becomes an export instruction,
   otherwise its an import instruction by default. 
***********************************************************************************/

#ifdef Si10DIR_EX
   #define Si10DIR  __declspec(dllexport)     // export DLL information
#else
   #define Si10DIR  __declspec(dllimport)     // import DLL information
#endif

#define SI10ApiSuccess							512
#define SI10ApiFailure							-1
#define SI10ApiFailedToAssignHandle				-2
#define	SI10ApiHandleNotValid					-3
#define SI10ApiHardwareNotFound					-4
#define	SI10ApiFailedToShareStructures			-5
#define SI10ApiFailedToOpenConfigurationFile	-6
#define SI10ApiSelectedJoystickOutOfRange		-7
#define	SI10ApiSelectedAxisOutOfRange			-8
#define SI10ApiIrqMaskNotSet					-9
#define	SI10ApiAxisNotAvailable					-10
#define SI10ApiSelectedEncoderOutOfRange		-11


extern "C" { 
	bool Si10DIR isSi10AxisInPosition(int nAxis);

	int Si10DIR	getSi10EncoderValue(int nEncoderNumber, int *nValue);
	int	Si10DIR getSi10CurrentPositionReal(int nAxis, float *fPos);
	int	Si10DIR getSi10CurrentPositionCount(int nAxis, int *nPos);
	int Si10DIR	getSi10ReadJoystick(int nWhich, int *nValue);

	int Si10DIR setSi10EncoderZero(int nEncoderNumber);
	int	Si10DIR	setSi10AxisDefaults(int nAxis);
	int	Si10DIR	setSi10CardDefaults();

	int Si10DIR Si10MoveAxisToCount(int nAxis, int nPos);
	int Si10DIR Si10MoveAxisToReal(int nAxis, float fPos);
	int	Si10DIR	Si10DeviceClose();
	int	Si10DIR	Si10DeviceOpen();
};

class Si10DIR Si10class
{
	public:
	bool	isSi10AxisRunning(int nAxis);
	int		EncoderRead(int nEncoderNumber);
	void	EncoderZero(int nEncoderNumber);
	int		CounterStart(int nNumber,int nValue);
	int		DisableAxis(int nAxis);
	int		EnableAxis(int nAxis,int action);
	int		InpW(int nOffset);
	void	OutpW(int nOffset, int Value);
	bool	SI10PciBarSpaceWrite(int addrs, int ByteCount,int *pData);
	bool	SI10PciBarSpaceRead(int addrs, int ByteCount,int *pData);
	int		FindLatestBinaryFile(char* pStr);
	bool	isFPGALoaded();
	int		LoadFPGAConfigurationData();
	int		DeviceClose();
	int		DeviceOpen();
    Si10class();
    ~Si10class();
};

