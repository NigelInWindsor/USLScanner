#include "stdafx.h"
#include <gcroot.h>

#pragma managed(push, off)
#include "UTKernelDriver.h"
#include "UTKernelDriverOEMPA.h"
#include "UTKernelAPI.h"
#include "CustomizedDriverAPI.h"
typedef union constVoid{
	const void *pcVoid;
	void *pVoid;
}constVoid;
#pragma managed(push, on)

DllImport bool UTDriver_IsUserInterfaceThread();
void init(structAcqInfoEx &acqInfo);
void init(CStream_0x0001 &StreamHeader);
void init(CSubStreamIO_0x0101 &ioHeader);
void init(structCscanAmp_0x0102 &bufferAmp);
void init(structCscanAmpTof_0x0202 &bufferAmpTof);
void init(CSubStreamCscan_0x0X02 &cscanHeader);
void init(CSubStreamAscan_0x0103 &AscanHeader);
__declspec(dllimport) void debug_EnableHeapEx(bool bEnable,char *pstrModuleName);
__declspec(dllexport) bool debug_DumpHeap(char *pFileName,bool bStatistics);

UINT WINAPI gAcquisitionAscan_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
UINT WINAPI gAcquisitionCscan_0x00010X02(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof);
UINT WINAPI gAcquisitionIO_0x00010101(void *pAcquisitionParameter,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader);
UINT WINAPI gAcquisitionIO_1x00010101(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader);
UINT WINAPI gAcquisitionInfo(void *pAcquisitionParameter,const wchar_t *pInfo);

structCorrectionOEMPA* WINAPI gCallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
void WINAPI gCallbackSystemMessageBox(const wchar_t *pMsg);
void WINAPI gCallbackSystemMessageBoxList(const wchar_t *pMsg);
UINT WINAPI gCallbackSystemMessageBoxButtons(const wchar_t *pMsg,const wchar_t *pTitle,UINT nType);//should return IDOK IDYES IDNO...depending of the button pressed by the user.
int WINAPI gCallbackOempaApiMessageBox(HWND hWnd,const wchar_t *lpszText,const wchar_t *lpszCaption,UINT nType);

bool WINAPI gCallbackSetSizeDouble(struct structCallbackArrayDouble1D *pCallbackArray,int iSize);
bool WINAPI gCallbackSetDataDouble(struct structCallbackArrayDouble1D *pCallbackArray,int iIndex,double fData);
bool WINAPI gCallbackGetSizeDouble(struct structCallbackArrayDouble1D *pCallbackArray,int &iSize);
bool WINAPI gCallbackGetDataDouble(struct structCallbackArrayDouble1D *pCallbackArray,int iIndex,double &fData);

bool WINAPI gCallbackSetSizeFloat(struct structCallbackArrayFloat1D *pCallbackArray,int iSize);
bool WINAPI gCallbackSetDataFloat(struct structCallbackArrayFloat1D *pCallbackArray,int iIndex,float fData);
bool WINAPI gCallbackGetSizeFloat(struct structCallbackArrayFloat1D *pCallbackArray,int &iSize);
bool WINAPI gCallbackGetDataFloat(struct structCallbackArrayFloat1D *pCallbackArray,int iIndex,float &fData);

bool WINAPI gCallbackSetSizeDelay2(struct structCallbackArrayFloat2D *pCallbackArray,int iSize1,int iSize2);
bool WINAPI gCallbackSetDataDelay2(struct structCallbackArrayFloat2D *pCallbackArray,int iIndex1,int iIndex2,float fData);
bool WINAPI gCallbackGetSizeDelay2(struct structCallbackArrayFloat2D *pCallbackArray,int &iSize1,int &iSize2);
bool WINAPI gCallbackGetDataDelay2(struct structCallbackArrayFloat2D *pCallbackArray,int iIndex1,int iIndex2,float &fData);

bool WINAPI gCallbackSetSizeDac(struct structCallbackArrayFloatDac *pCallbackArray,int iSize);
bool WINAPI gCallbackSetDataDac(struct structCallbackArrayFloatDac *pCallbackArray,int iIndex,double dTime,float fSlope);
bool WINAPI gCallbackGetSizeDac(struct structCallbackArrayFloatDac *pCallbackArray,int &iSize);
bool WINAPI gCallbackGetDataDac(struct structCallbackArrayFloatDac *pCallbackArray,int iIndex,double &dTime,float &fSlope);

bool WINAPI gCallbackSetSizeByte1D(struct structCallbackArrayByte1D *pCallbackArray,int iSize);
bool WINAPI gCallbackSetDataByte1D(struct structCallbackArrayByte1D *pCallbackArray,int iIndex,BYTE byData);
bool WINAPI gCallbackGetSizeByte1D(struct structCallbackArrayByte1D *pCallbackArray,int &iSize);
bool WINAPI gCallbackGetDataByte1D(struct structCallbackArrayByte1D *pCallbackArray,int iIndex,BYTE &byData);

void gCallbackHWMemory(CHWDevice *pHWDevice, DWORD addr, DWORD data, int size);

using System::String;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;


namespace csDriverOEMPA
{

#pragma region enumerations
	public enum class csEnumHardwareLink{
									csUnlinked=eUnlinked,
									csMaster=eMaster,
									csSlave=eSlave
									};
	public enum class csEnumMsgBoxButtons{
									csOK												= 0x00000000,
									csOKCANCEL											= 0x00000001,
									csABORTRETRYIGNORE									= 0x00000002,
									csYESNOCANCEL										= 0x00000003,
									csYESNO												= 0x00000004,
									csRETRYCANCEL										= 0x00000005
									};
	public enum class csEnumMsgBoxReturn{
									csOK												= 1,
									csCANCEL											= 2,
									csABORT												= 3,
									csRETRY												= 4,
									csIGNORE											= 5,
									csYES												= 6,
									csNO												= 7
									};
	public enum class csEnumCustomizedFlags{
									csOEMPA_ASCAN_ENABLE								= 0x0000000000000001,
									csOEMPA_CSCAN_ENABLE_TOF							= 0x0000000000000002,
									csOEMPA_ASCAN_BITSIZE								= 0x0000000000000004,
									csOEMPA_TRIGGER_MODE								= 0x0000000000000008,
									csOEMPA_TRIGGER_ENCODER_STEP						= 0x0000000000000010,
									csOEMPA_REQUESTIO									= 0x0000000000000020,
									csOEMPA_REQUESTIO_DIGITAL_INPUT_MASK				= 0x0000000000000040,
									csOEMPA_KEEPALIVE									= 0x0000000000000080,
									csOEMPA_DEBOUNCER_ENCODER							= 0x0000000000000100,
									csOEMPA_DEBOUNCER_DIGITAL							= 0x0000000000000200,
									csOEMPA_DIGITAL_OUTPUT_0							= 0x0000000000000400,
									csOEMPA_DIGITAL_OUTPUT_1							= 0x0000000000000800,
									csOEMPA_DIGITAL_OUTPUT_2							= 0x0000000000001000,
									csOEMPA_DIGITAL_OUTPUT_3							= 0x0000000000002000,
									csOEMPA_DIGITAL_OUTPUT_4							= 0x0000000000004000,
									csOEMPA_DIGITAL_OUTPUT_5							= 0x0000000000008000,
									csOEMPA_SW_ENCODER1_RESOLUTION					= 0x0000000000010000,
									csOEMPA_SW_ENCODER1_DIVIDER						= 0x0000000000020000,
									csOEMPA_ENCODER1A									= 0x0000000000040000,
									csOEMPA_ENCODER1B									= 0x0000000000080000,
									csOEMPA_ENCODER2A									= 0x0000000000100000,
									csOEMPA_ENCODER2B									= 0x0000000000200000,
									csOEMPA_EXTERNAL_TRIGGER_CYCLE					= 0x0000000000400000,
									csOEMPA_EXTERNAL_TRIGGER_SEQUENCE					= 0x0000000000800000,
									csOEMPA_SW_ENCODER2_RESOLUTION					= 0x0000000001000000,
									csOEMPA_SW_ENCODER2_DIVIDER						= 0x0000000002000000,
									csOEMPA_ENCODER1_TYPE								= 0x0000000004000000,
									csOEMPA_ASCAN_REQUEST								= 0x0000000008000000,
									csOEMPA_DEFAULT_HW_LINK							= 0x0000000010000000,
									csOEMPA_TRACKING									= 0x0000000020000000,
									csOEMPA_ENCODER2_TYPE								= 0x0000000040000000
									};
	public enum class csEnumAcquisitionState{	csAcqOff=eAcqOff,	//pulse shot is disabled.
									csAcqDefault=eAcqDefault,//the configuration file is used to stop or start the pulse shot.
									csAcqOn=eAcqOn			//pulse shot is enabled.
									};
	public enum class csEnumConnectionState{	csDisconnected=eDisconnected,csConnected=eConnected };
	public enum class csEnumUpdateStatus{	csOutOfDate=eOutOfDate,csUpToDate=eUpToDate };
	public enum class csEnumUnit{
		csNoUnit=eNoUnit,csHexadecimal=eHexadecimal,csPercent=ePercent,csMeter=eMeter,csMilliMeter=eMilliMeter,csSecond=eSecond,csMilliSecond=eMilliSecond,csMicroSecond=eMicroSecond,csNanoSecond=eNanoSecond,
		csMeterPerSecond=eMeterPerSecond,csRadian=eRadian,csDegree=eDegree,csDecibel=eDecibel,csHertz=eHertz,csKiloHertz=eKiloHertz,csMegaHertz=eMegaHertz,csVolt=eVolt,
		csBytePerSecond=eBytePerSecond,csKiloBytePerSecond=eKiloBytePerSecond,csMegaBytePerSecond=eMegaBytePerSecond,csDegreeCelsius=eDegreeCelsius,csLinkRootId=eLinkRootId,
		csBinary=eBinary,csStepPerMilliMeter=eStepPerMilliMeter,
		csLastUnit=eLastUnit
	};
	public enum class csEnumEncoderType{
		csStaticScan=eStaticScan,	//offset are used to store the data (no encoder).
		csEncoderQuadrature=eEncoderQuadrature,	//encoder are used to store the data (quadrature).
		csEncoderQuadrature4Edges=eEncoderQuadrature4Edges,//encoder are used to store the data (quadrature).
		csEncoderDirectionCount=eEncoderDirectionCount,	//encoder : one signal for the direction and one signal for the count.
		csEncoderForwardBackward=eEncoderForwardBackward,	//encoder : one signal for the forward and one signal for the back.
		csSpeedMeasurement=eSpeedMeasurement //OnLine.
	};

	public enum class csEnumOEMPAFilter{
		csOEMPAFilterCustom=eOEMPAFilterCustom,
		csOEMPAHighPassOrder64Cut1Mhz=eOEMPAHighPassOrder64Cut1Mhz,
		csOEMPAHighPassOrder64Cut2Mhz=eOEMPAHighPassOrder64Cut2Mhz,
		csOEMPAHighPassOrder64Cut3Mhz=eOEMPAHighPassOrder64Cut3Mhz,
		csOEMPAHighPassOrder64Cut4Mhz=eOEMPAHighPassOrder64Cut4Mhz,
		csOEMPAHighPassOrder64Cut5Mhz=eOEMPAHighPassOrder64Cut5Mhz,
		csOEMPAHighPassOrder64Cut6Mhz=eOEMPAHighPassOrder64Cut6Mhz,
		csOEMPAHighPassOrder64Cut7Mhz=eOEMPAHighPassOrder64Cut7Mhz,
		csOEMPAHighPassOrder64Cut8Mhz=eOEMPAHighPassOrder64Cut8Mhz,
		csOEMPAHighPassOrder64Cut9Mhz=eOEMPAHighPassOrder64Cut9Mhz,
		csOEMPAHighPassOrder64Cut10Mhz=eOEMPAHighPassOrder64Cut10Mhz
	};
	public enum class csEnumIndexDigitalInput{
		csIndexDigitalInputBlanking=eIndexDigitalInputBlanking,
		csIndexDigitalInputWire1=eIndexDigitalInputWire1,
		csIndexDigitalInputWire2=eIndexDigitalInputWire2,
		csIndexDigitalInputPresetRising=eIndexDigitalInputPresetRising,
		csIndexDigitalInputSpeed1=eIndexDigitalInputSpeed1,
		csIndexDigitalInputSpeed2=eIndexDigitalInputSpeed2,
		csIndexDigitalInputExternalCycle=eIndexDigitalInputExternalCycle,
		csIndexDigitalInputExternalSequence=eIndexDigitalInputExternalSequence,
		csIndexDigitalInputEncoder2Wire1=eIndexDigitalInputEncoder2Wire1,
		csIndexDigitalInputEncoder2Wire2=eIndexDigitalInputEncoder2Wire2,
		csIndexDigitalInputEncoder2PresetRising=eIndexDigitalInputEncoder2PresetRising,
		csIndexDigitalInputLast=eIndexDigitalInputLast
	};
	public enum class csEnumDigitalInput{
		csDigitalInputOff=eDigitalInputOff,
		csDigitalInput01=eDigitalInput01,
		csDigitalInput02=eDigitalInput02,
		csDigitalInput03=eDigitalInput03,
		csDigitalInput04=eDigitalInput04,
		csDigitalInput05=eDigitalInput05,
		csDigitalInput06=eDigitalInput06,
		csDigitalInput07=eDigitalInput07,
		csDigitalInput08=eDigitalInput08,
		csDigitalInput09=eDigitalInput09,
		csDigitalInput10=eDigitalInput10,
		csDigitalInput11=eDigitalInput11,
		csDigitalInput12=eDigitalInput12,
		csDigitalInput13=eDigitalInput13,
		csDigitalInput14=eDigitalInput14,
		csDigitalInput15=eDigitalInput15,
		csDigitalInput16=eDigitalInput16,
		csDigitalInput17=eDigitalInput17,
		csDigitalInput18=eDigitalInput18,
		csDigitalInput19=eDigitalInput19,
		csDigitalInput20=eDigitalInput20,
		csDigitalInput21=eDigitalInput21,
		csDigitalInput22=eDigitalInput22,
		csDigitalInput23=eDigitalInput23,
		csDigitalInput24=eDigitalInput24
	};
	public enum class csEnumKeepAlive{
		eKeepAliveOff,
		eKeepAliveHardwareAndComputer,
		eKeepAliveHardwareOnly,
		eKeepAliveComputerOnly
	};
	public enum class csEnumFocusing{csStandard=eStandard,csDDF=eDDF};
	public enum class csEnumAscanRequest{
		eAscanAll,
		eAscanSampled,
		eBscanOnLine
	};
	public enum class csEnumBitSize {cs8Bits=e8Bits,cs12Bits=e12Bits,cs16Bits=e16Bits,csLog8Bits=eLog8Bits};
	public enum class csEnumCompressionType {csCompression=eCompression,csDecimation=eDecimation};
	public enum class csEnumGateModeAmp{csAmpAbsolute=eAmpAbsolute,csAmpMaximum=eAmpMaximum,csAmpMinimum=eAmpMinimum,csAmpPeakToPeak=eAmpPeakToPeak};
	public enum class csEnumGateModeTof{csTofAmplitudeDetection=eTofAmplitudeDetection,//"AMP's TOF" : where the AMP result has been found, for Peak-Peak--> where Max has been found
								csTofThresholdCross=eTofThresholdCross,//"TH cross": first cross of the THRESHOLD  
								csTofZeroFirstAfterThresholdCross=eTofZeroFirstAfterThresholdCross,//"ZrA": first time crossed 0 after crossing THRESHOLD
								csTofZeroLastBeforeThresholdCross=eTofZeroLastBeforeThresholdCross//"ZrB": last time crossed 0 before crossing THRESHOLD
								};
	public enum class csEnumRectification{
							csSigned=eSigned,			//Rectification: RF --> not rectified, signed value,
							csUnsigned=eUnsigned,			//FW --> rectified, unsigned
							csUnsignedPositive=eUnsignedPositive,	//HWP --> only positive, unsigned,
							csUnsignedNegative=eUnsignedNegative	//HWN --> only negative
							};

	public enum class csEnumOEMPATrigger{
		csOEMPAInternal=eOEMPAInternal,//encoder and external signals are not used.
		csOEMPAEncoder=eOEMPAEncoder,//encoder 1 is used for the sequence (cycle is internal).
		csOEMPAExternalCycle=eOEMPAExternalCycle,//the next cycle after the last cycle is for the next sequence.
		csOEMPAExternalSequence=eOEMPAExternalSequence,//sequence is coming from external pulse (cycle are internal).
		csOEMPAExternalCycleSequence=eOEMPAExternalCycleSequence//cycle and sequence are coming from external signals.
	};
	public enum class csEnumOEMPAEncoderDirection{
		csOEMPAUpDown=eOEMPAUpDown,//pulser is triggered for both directions.
		csOEMPAUp=eOEMPAUpOnly,//pulser is triggered only if encoder goes up.
		csOEMPADown=eOEMPADownOnly//pulser is triggered only if encoder goes down.
	};
	public enum class csEnumOEMPARequestIO{
		csOEMPANotRequired=eOEMPANotRequired,
		csOEMPAOnCycleOnly=eOEMPAOnCycleOnly,//use this value if you want Ascan with encoder.
		csOEMPAOnSequenceOnly=eOEMPAOnSequenceOnly,
		csOEMPAOnDigitalInputOnly=eOEMPAOnDigitalInputOnly,
		csOEMPAOnDigitalInputAndCycle=eOEMPAOnDigitalInputAndCycle,//use this value if you want Ascan with encoder and digital inputs.
		csOEMPAOnDigitalInputAndSequence=eOEMPAOnDigitalInputAndSequence

	};
	public enum class csEnumOEMPAFilterIndex{
		csOEMPAFilterOff=eOEMPAFilterOff,
		csOEMPAFilter1=eOEMPAFilter1,
		csOEMPAFilter2=eOEMPAFilter2,
		csOEMPAFilter3=eOEMPAFilter3,
		csOEMPAFilter4=eOEMPAFilter4,
		csOEMPAFilter5=eOEMPAFilter5,
		csOEMPAFilter6=eOEMPAFilter6,
		csOEMPAFilter7=eOEMPAFilter7,
		csOEMPAFilter8=eOEMPAFilter8,
		csOEMPAFilter9=eOEMPAFilter9,
		csOEMPAFilter10=eOEMPAFilter10,
		csOEMPAFilter11=eOEMPAFilter11,
		csOEMPAFilter12=eOEMPAFilter12,
		csOEMPAFilter13=eOEMPAFilter13,
		csOEMPAFilter14=eOEMPAFilter14,
		csOEMPAFilter15=eOEMPAFilter15

	};
	public enum class csEnumOEMPAMappingDigitalInput{
		csOEMPANotUsed=eOEMPANotUsed,
		csOEMPAEncoder1A=eOEMPAEncoder1A,
		csOEMPAEncoder1B=eOEMPAEncoder1B,
		csOEMPAEncoder2A=eOEMPAEncoder2A,
		csOEMPAEncoder2B=eOEMPAEncoder2B,
		csOEMPAExternalTriggerCycle=eOEMPAExternalTriggerCycle,
		csOEMPAExternalTriggerSequence=eOEMPAExternalTriggerSequence,
		csOEMPADigitalInput0=eOEMPADigitalInput0,
		csOEMPADigitalInput1=eOEMPADigitalInput1,
		csOEMPADigitalInput2=eOEMPADigitalInput2,
		csOEMPADigitalInput3=eOEMPADigitalInput3,
		csOEMPADigitalInput4=eOEMPADigitalInput4,
		csOEMPADigitalInput5=eOEMPADigitalInput5,
		csOEMPAResetTrackingTable=eOEMPAResetTrackingTable
	};
	public enum class csEnumOEMPAMappingDigitalOutput{
		csOEMPALow=eOEMPALow,
		csOEMPAHigh=eOEMPAHigh,
		csOEMPASignalCycle=eOEMPASignalCycle,
		csOEMPASignalSequence=eOEMPASignalSequence
	};
	public enum class csEnumOEMPARequestCscan{
		csOEMPACycle=eOEMPACycle,
		csOEMPASequence=eOEMPASequence
	};
	public enum class csEnumStepCustomizedAPI
	{
		csReadFileWriteHW_Enter=eReadFileWriteHW_Enter,
		csWriteHW_Enter=eWriteHW_Enter,
		csWriteHW_Leave=eWriteHW_Leave,
		csReadFileWriteHW_Leave=eReadFileWriteHW_Leave,
		csWriteFile_Enter=eWriteFile_Enter,
		csWriteFile_Leave=eWriteFile_Leave
	};
	public enum class csEnumTFMParameters
	{csTFMOff=eTFMOff,csFlatContact_Linear1D=eFlatContact_Linear1D};
	public enum class csEnumAcquisitionFifo
	{csFifoAscan=eFifoAscan,csFifoCscan=eFifoCscan,csFifoIO=eFifoIO};
	public enum class csEnumReferenceTimeOfFlight
	{csPulse=ePulse,csInterface=eInterface};
#pragma endregion enumerations
///////////////////////////////////////////////////////////////////////////////////////
#pragma region structures
	[StructLayout(LayoutKind::Sequential)]
	public ref class csAcqInfoEx
	{
	public:
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iDriverEncoderCountMax)]
		array<long>^ lEncoder;//unit given by the encoder
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iDriverEncoderCountMax)]
		array<double>^ dEncoder;//unit : meter (encoder resolution is taken into account).
		DWORD dwDigitalInputs;
		long lMaxTemperature;//0 if not significant (for some FW version or in case IO stream are not sent by the HW).
		long lDeviceId;//device identifier
		long bMultiChannel;//1 if Multi-Channel is enabled, 0 otherwise.
		long bFullMatrixCapture;//1 if FullMatrixCapture is enabled, 0 otherwise.
		long lFMCElementIndex;//in case of Full Matrix Capture, this is the element index for the current ascan;
	};

	[StructLayout(LayoutKind::Sequential)]
	public ref class csHeaderStream_0x0001
	{
	public:
		unsigned long start;
		unsigned long size;
		unsigned long frameId;
		unsigned long settingId;
		unsigned short subStreamCount;
		unsigned short version;
	};

	[StructLayout(LayoutKind::Sequential)]
	public ref class csHeaderIO_0x0001
	{
	public:
		BYTE id;
		BYTE version;
		WORD size;
		DWORD timeStampLow;
		DWORD timeStampHigh;
		WORD cycle;
		WORD padding0;
		DWORD seqLow;
		DWORD seqHigh;
		DWORD inputs;//digital inputs value.
		DWORD edges;//edges : which digital input has been updated ?
						//if bit 16 is set to 1 then this is initialisation stream (sent on any updates from "SetRequestIO").
		DWORD encoder1;
		DWORD encoder2;
	};

	[StructLayout(LayoutKind::Sequential,Size=sizeof(structCscanAmp_0x0102))]
	public ref class csCscanAmp_0x0102
	{
	public:
		BYTE byAmp;//cscan amplitude. Could be signed value, it depends of the GateModeThreshold setting data.
		BYTE gateType;//please use properties
							//unsigned gateId:2;//gate identifier (could be: 0=first gate, 1=second gate, 2=third gate, 3=fourth gate).
							//unsigned sign:1;
							//unsigned reserved:5;
		WORD wAcqId;//acquisition identifier (could be used to link the unique gate/cycle cscan data with the setting). But it is not required to use it.

		property int gateId{int get(){return gateType & 0x3;};}
		property bool sign {bool get(){return (gateType & 4?true:false);};}
		property bool AmpOverThreshold {bool get(){return (gateType & 8?true:false);};}
		property bool IFOldReference {bool get(){return (gateType & 0x20?true:false);};}
		property bool IFNotInitialized {bool get(){return (gateType & 0x40?true:false);};}
	};

	[StructLayout(LayoutKind::Sequential,Size=sizeof(structCscanAmpTof_0x0202))]
	public ref class csCscanAmpTof_0x0202
	{
	public:
		BYTE byAmp;//cscan amplitude. Could be signed value, it depends of the GateModeThreshold setting data.
		BYTE gateType;//please use properties
					//unsigned gateId:2;//gate identifier (could be: 0=first gate, 1=second gate, 2=third gate, 3=fourth gate).
					//unsigned sign:1;
					//unsigned reserved:5;
		WORD wAcqIdAmp;//acquisition identifier (could be used to link the unique gate/cycle cscan data with the setting). But it is not required to use it.
		WORD wTof;//cscan time of flight for the same gate.
		WORD wAcqIdTof;//time of flight acquisition identifier (could be used to link the unique gate/cycle cscan data with the setting). But it is not required to use it.

		property int gateId{int get(){return gateType & 0x3;};}
		property bool sign {bool get(){return (gateType & 4?true:false);};}
		property bool AmpOverThreshold {bool get(){return (gateType & 8?true:false);};}
		property bool TofValidity {bool get(){return (gateType & 0x10?true:false);};}
		property bool IFOldReference {bool get(){return (gateType & 0x20?true:false);};}
		property bool IFNotInitialized {bool get(){return (gateType & 0x40?true:false);};}
	};

	[StructLayout(LayoutKind::Sequential)]
	public ref class csSubStreamCscan_0x0X02
	{
	public:
		BYTE id;
		BYTE version;	//could be 1 or 2 (it depends of the setting, parameter RequestCscan with enumeration enumOEMPARequestCscan):
							//1: all cscan gate (amplitude only), packed together in the current sub stream, are for the same cycle number.
							//	 in this case the member "cylce" could used.
							//2: all cscan gate (amplitude + time of flight) of all cycles of the sequence are packed together in the same substream.
							//	 in this case the member "cylce" has no significance.
		WORD size;
		DWORD timeStampLow;
		DWORD timeStampHigh;
		WORD cycle;//cycle value in case "version" member value is 1, cycle=0xffff in case "version" member value is 2 (not significant in this case).
		WORD exCount;//please use property count
					//unsigned count:15;//count of gates (amplitude only in case of "version" is 1, or amplitude + time of flight in case of "version" is 2).
					//unsigned reserved:1;
		DWORD seqLow;//sequence number (LOW WORD).
		DWORD seqHigh;//sequence number (HIGH WORD).

		property int count{int get(){return exCount & 0x7fff;};}
	};

	[StructLayout(LayoutKind::Sequential)]
	public ref class csSubStreamAscan_0x0103
	{
	//ONLY USEFUL MEMBERS FOR THE USER HAVE COMMENT.
	public:
		BYTE id;
		BYTE version;
		WORD size;
		DWORD timeStampLow;
		DWORD timeStampHigh;
		WORD cycle;//cycle number. this value is the cycle index (range 0 to max CycleCount-1, where CycleCount is the cycle count in your OEMPA file.
		WORD dataCount;	//data count (dataCount x dataSize = total data size). This is the sample count.
						//For example if you have AscanRange=30 us in your OEMPA file without compression:
						//	3000 ns/10 ns=3000 plus one point (10 ns= digitizing clock) that is dataCount=3001.
						//If you increase the AscanRange this value will be increased.
						//The buffer size of pBufferMax is (dataCount x dataSize).
		BYTE ascan_byte1;//please use properties
							//unsigned src:1;
							//unsigned dst:1;
							//unsigned type:1;
							//unsigned error:1;
							//unsigned dataSize:4;//size of one data (in bytes), take a look also to member "bitSize".
		BYTE ascan_byte2;//please use properties
							//unsigned align:4;
							//unsigned max:1;//maximum buffer is valid.
							//unsigned min:1;//minimum buffer is valid.
							//unsigned sat:1;//saturation buffer is valid.
							//unsigned sign:1;//sign of maximum and minimum buffer data.
		WORD ascan_word1;//please use properties
							//unsigned bitSize:2;//see enumBitSize
							//unsigned seqLost:1;//encoder speed too high.
							//unsigned padding0:13;
		WORD FWAcqIdChannelCycle;//acquisition Id for saturation data, see "OEMPA_SetAscanAcqIdChannelCycle".
		WORD FWAcqIdChannelScan;//acquisition Id for minimum data, see "OEMPA_SetAscanAcqIdChannelScan".
		WORD FWAcqIdChannelProbe;//acquisition Id for maximum data, see "OEMPA_SetAscanAcqIdChannelProbe".
		WORD padding1;
		DWORD seqLow;//sequence number (LOW DWORD)
		DWORD seqHigh;//sequence number (HIGH DWORD)

		property bool src {bool get(){return (ascan_byte1 & 1?true:false);};}
		property bool dst {bool get(){return (ascan_byte1 & 2?true:false);};}
		property bool type {bool get(){return (ascan_byte1 & 4?true:false);};}
		property bool error {bool get(){return (ascan_byte1 & 8?true:false);};}
		property int dataSize {int get(){return (ascan_byte1 & 0xf0)/16;};}

		property int align {int get(){return (ascan_byte2 & 0xf);};}
		property bool max {bool get(){return (ascan_byte2 & 0x10?true:false);};}//true if pBufferMax is not NULL.
		property bool min {bool get(){return (ascan_byte2 & 0x20?true:false);};}//true if pBufferMin is not NULL.
		property bool sat {bool get(){return (ascan_byte2 & 0x40?true:false);};}//true if pBufferSat is not NULL.
		property bool sign {bool get(){return (ascan_byte2 & 0x80?true:false);};}//1 if ascan (pBufferMax) is signed data and 0 if it is unsigned.

		property csEnumBitSize bitSize {csEnumBitSize get(){return (csEnumBitSize)(ascan_word1 & 0x3);};}//This is the size of one sample. This size can be set by the OEMPA file. For example:
																										 //cs8Bits -> bitSize=1
																										 //cs12Bits -> bitSize=2
																										 //cs16Bits -> bitSize=2
		//property bool seqLost {bool get(){return (ascan_word1 & 0x4?true:false);};}
		property bool dacIFOldReference {bool get(){return (ascan_word1 & 0x4?true:false);};}
		property bool dacIFNotInitialized {bool get(){return (ascan_word1 & 0x8?true:false);};}
		property bool IFOldReference {bool get(){return (ascan_word1 & 0x10?true:false);};}
		property bool IFNotInitialized {bool get(){return (ascan_word1 & 0x20?true:false);};}
	};
//////////////////////////////////////////////////////////////////////////
	[StructLayout(LayoutKind::Sequential,CharSet = CharSet::Unicode)]
	public ref class csFilter
	{
	public:
		[MarshalAs(UnmanagedType::ByValTStr, SizeConst = g_iFilterTitleLength)]
		String^ pTitle;
		WORD wScale;
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iOEMPAFilterCoefficientMax)]
		array<short>^ aCoefficient;
	};

	[StructLayout(LayoutKind::Sequential)]
	public ref class csRoot
	{
	public:
		bool vCopyTo(void *pRoot){return CopyTo((structRoot*)pRoot);};
		bool vCopyFrom(void *pRoot){return CopyFrom((structRoot*)pRoot);};
		bool CopyTo(structRoot *pRoot);
		bool CopyFrom(structRoot *pRoot);

		DWORD dwVersionLSB;
		DWORD dwVersionMSB;
		DWORD dwRootSize;
		ULONGLONG ullSavedParameters;//each bit is for one parameter.
		UINT uiSavedFilterCount;
	csEnumHardwareLink csDefaultHwLink;
	bool bEnableFMC;//OEMPA_ENABLE_FMC
	bool bEnableMultiHWChannel;//OEMPA_ENABLE_MULTI_CHANNEL
		bool bAscanEnable;//OEMPA_ASCAN_ENABLE
		bool bEnableCscanTof;//OEMPA_CSCAN_ENABLE_TOF
	csEnumTFMParameters csEnableTFM;
		csEnumBitSize csAscanBitSize;//OEMPA_ASCAN_BITSIZE
		csEnumAscanRequest csAscanRequest;//OEMPA_ASCAN_REQUEST
		double dAscanRequestFrequency;//OEMPA_ASCAN_REQUEST
		csEnumOEMPATrigger csTriggerMode;//OEMPA_TRIGGER_MODE
		csEnumOEMPAEncoderDirection csEncoderDirection;//OEMPA_ENCODER_TRIG_DIRECTION
		char cTemperatureWarning;//OEMPA_TEMPERATURE_WARNING should be >=0.
		char cTemperatureAlarm;//OEMPA_TEMPERATURE_ALARM should be >=0.
		double dTriggerEncoderStep;//OEMPA_TRIGGER_ENCODER_STEP
		csEnumOEMPARequestIO csRequestIO;//OEMPA_REQUESTIO
		int iRequestIODigitalInputMaskRising;//OEMPA_REQUESTIO_DIGITAL_INPUT_MASK
		int iRequestIODigitalInputMaskFalling;//OEMPA_REQUESTIO_DIGITAL_INPUT_MASK
		double dDebouncerEncoder;//OEMPA_DEBOUNCER_ENCODER
		double dDebouncerDigital;//OEMPA_DEBOUNCER_DIGITAL
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iOEMPAMappingDigitalOutputMax)]
		array<csEnumOEMPAMappingDigitalOutput>^ csDigitalOuput;//OEMPA_DIGITAL_OUTPUT_0, OEMPA_DIGITAL_OUTPUT_1 ... OEMPA_DIGITAL_OUTPUT_5
		long lSWEncoderResolution1;//OEMPA_SW_ENCODER1_RESOLUTION SW parameter
		long lSWEncoderResolution2;//OEMPA_SW_ENCODER2_RESOLUTION SW parameter
		DWORD dwSWEncoderDivider1;//OEMPA_SW_ENCODER1_DIVIDER SW parameter
		DWORD dwSWEncoderDivider2;//OEMPA_SW_ENCODER2_DIVIDER SW parameter
		csEnumDigitalInput csEncoder1A;//OEMPA_ENCODER1A
		csEnumDigitalInput csEncoder1B;//OEMPA_ENCODER1B
		csEnumDigitalInput csEncoder2A;//OEMPA_ENCODER2A
		csEnumDigitalInput csEncoder2B;//OEMPA_ENCODER2B
		csEnumDigitalInput csExternalTriggerCycle;//OEMPA_EXTERNAL_TRIGGER_CYCLE
		csEnumDigitalInput csExternalTriggerSequence;//OEMPA_EXTERNAL_TRIGGER_SEQUENCE
		csEnumEncoderType csEncoder1Type;//OEMPA_ENCODER1_TYPE
		csEnumEncoderType csEncoder2Type;//OEMPA_ENCODER2_TYPE
		csEnumKeepAlive csKeepAlive;//OEMPA_KEEPALIVE
		//Configuration parameters : end

		int iCycleCount;//-1 means HW cycles are not modified, otherwise the HW cycles are updated.
						//0 the "SetCycleCount" function is called with value 0.
						//1 the "SetCycleCount" function is called with value 1.
						//...
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iEnumOEMPAFilterIndexLast)]
		array<csFilter^>^ aFilter;//OEMPA_DIGITAL_OUTPUT_0, OEMPA_DIGITAL_OUTPUT_1 ... OEMPA_DIGITAL_OUTPUT_5
		int iFMCElementStart;//FullMatrixCapture
		int iFMCElementStop;//FullMatrixCapture
		int iFMCElementStep;//FullMatrixCapture

		double dSpecimenVelocity;//view correction purpose (m/s)
	double dSpecimenRadius;//meter (if 0.0 it is a plane).
	double dSpecimenThickness;//meter (0.0 means not defined).
		double dDigitizingPeriod;//view correction purpose (second)
		int iOEMPAProbeCountMax;
		int iOEMPAApertureCountMax;
	int iOEMPADDFCountMax;
	BYTE bUSB3Disable;
	double dMultiHWChannelRangeMax;//OEMMC max range
	double dFWAscanRecoveryTime;//phased array or OEM-MC
	double dTriggerHighTime;
	
	//sub-sequence table management
	int iSubSequenceEncoderCount;//size of aiSubSequenceCycleStart and aiSubSequenceCycleStop in case Position is used (size of afSubSequenceValue is one more).
	int iSubSequenceGateCount;//size of aiSubSequenceCycleStart and aiSubSequenceCycleStop in case Gate is used (size of afSubSequenceValue is the same).
	[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iSubSequenceTableSize)]
	array<int>^ aiSubSequenceCycleStart;
	[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iSubSequenceTableSize)]
	array<int>^ aiSubSequenceCycleStop;
	[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iSubSequenceTableSize)]
	array<float>^ afSubSequenceValue;//unit is either millimeter (Position) either second (Gate).
	int iSubSequenceAverage;

	csEnumReferenceTimeOfFlight eReferenceTimeOfFlight;

	bool b256Master;
	bool b256Slave;

		void/*CHWDeviceOEMPA*/ *pHWDeviceOEMPA;//1.1.5.4g void is better.
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = MAX_PATH)]
		array<wchar_t>^ wcFileName;//wchar_t wcFileName[MAX_PATH];
	};
	[StructLayout(LayoutKind::Sequential)]
	public ref class csGate
	{
	public:
		//ToDo bool CopyTo(structRoot *pRoot);
		//ToDo bool CopyFrom(structRoot *pRoot);

		bool bEnable;
		double dStart;//unit = second
		double dStop;//unit = second
		double dThreshold;//unit = percent
		csEnumRectification eRectification;
		csEnumGateModeAmp eModeAmp;
		csEnumGateModeTof eModeTof;
		WORD wAcqIDAmp;
		WORD wAcqIDTof;
		//tracking
		bool bTrackingStartEnable;
		int iTrackingStartIndexCycle;
		int iTrackingStartIndexGate;
		bool bTrackingStopEnable;
		int iTrackingStopIndexCycle;
		int iTrackingStopIndexGate;
	};

	[StructLayout(LayoutKind::Sequential)]
	public ref class csCycle
	{
	public:
		bool vCopyTo(void *pCycle){return CopyTo((structCycle*)pCycle);};
		bool vCopyFrom(void *pCycle){return CopyFrom((structCycle*)pCycle);};
		bool CopyTo(structCycle *pCycle);
		bool CopyFrom(structCycle *pCycle);

		double dGainDigital;
		float fBeamCorrection;
		double dStart;//unit = second
		double dRange;//unit = second
		double dTimeSlot;//unit = second
	double dFMCSubTimeSlotAcq;//unit = second
	double dFMCSubTimeSlotReplay;//unit = second
		long lPointCount;
		long lPointFactor;//1.1.5.4g
		csEnumCompressionType eCompressionType;
		csEnumRectification eRectification;
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iOEMPADACCountMax)]
		array<double>^ adDACTof;//unit = second
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iOEMPADACCountMax)]
		array<float>^ afDACPrm;//unit = dB/second (in case of slope) and dB (in case of gain), see "bDACSlope".
		int iDACCount;
		bool bDACSlope;
		bool bDACEnable;
		bool bDACAutoStop;
		bool bMaximum;
		bool bMinimum;
		bool bSaturation;
		WORD wAcqIdChannelProbe;
		WORD wAcqIdChannelScan;
		WORD wAcqIdChannelCycle;
		float fGainAnalog;//unit = dB
		int iFilterIndex;//0=no filter, and range 1 to 15 to select one filter.
		//tracking: ascan and DAC 1.1.5.4i
		bool bTrackingAscanEnable;
		int iTrackingAscanIndexCycle;
		int iTrackingAscanIndexGate;
		bool bTrackingDacEnable;
		int iTrackingDacIndexCycle;
		int iTrackingDacIndexGate;

		int iGateCount;
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iOEMPAGateCountMax)]
		array<csGate^>^ gate;
	};

	[StructLayout(LayoutKind::Sequential)]
	public ref class csFocalLaw
	{
	public:
		bool vCopyTo(void *pFocalLaw){return CopyTo((CFocalLaw*)pFocalLaw);};
		bool vCopyFrom(void *pFocalLaw){return CopyFrom((CFocalLaw*)pFocalLaw);};
		bool CopyTo(CFocalLaw *pFocalLaw);
		bool CopyFrom(CFocalLaw *pFocalLaw);

		double dWedgeDelay;//unit = second.
		int iElementCount;//count of element in the aperture.
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iOEMPAApertureDWordCount)]
		array<DWORD>^ adwElement;//bit i : 0 if element is disabled, 1 if element is enabled. 2x32 = 64 elements.
		csEnumFocusing csFocusing;//to enable DDF.
		int iDelayCount;//count of useful delays = "iFocalCount" x "iElementCount".
		int iFocalCount;//count of focal law.
		//[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iOEMPAFocalCountMax*g_iOEMPAApertureElementCountMax)]
		array<float,2>^ afDelay;//unit = second, first index = focal law index, 2d index = element.
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iOEMPAFocalCountMax)]
		array<double>^ adFocalTimeOfFlight;//unit = second.
		float fCenterDelay;//delay for the aperture centroid.
		int iPrmCount;//useful item count in array "afPrm". Copy of "iElementCount".
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iOEMPAApertureElementCountMax)]
		array<float>^ afPrm;//gain (dB) or width (second)
		double dSkew;//view correction purpose
		double dAngle;//view correction purpose
		double dX,dY,dZ;//view correction purpose
		double dFocalX,dFocalY,dFocalZ;//last focal point (absolute referential)
		[MarshalAs(UnmanagedType::ByValArray, SizeConst = g_iOEMPAApertureElementCountMax/2)]
		array<BYTE>^ hwAcqDecimation;//one BYTE is for 2 channels (that is 1 decimation 4 bits).

		float GetDelay(int iFocalIndex,int iElementIndex);
		bool SetDelay(int iFocalIndex,int iElementIndex,float fDelay);
		int GetHWAcquisitionDecimation(int iChannelIndex);
	};

	[StructLayout(LayoutKind::Sequential)]
	public ref class csOptionsCom
	{
	public:
		WORD uEthernetSpeed;
		WORD uReserved;
	};
	[StructLayout(LayoutKind::Sequential)]
	public ref class csOptionsTCP
	{
	public:
		BYTE uScale;
		BYTE uWndSize;
		BYTE uMss;
		BYTE uOption;
	};
	[StructLayout(LayoutKind::Sequential)]
	public ref class csOptionsFlash
	{
	public:
		BYTE uManufacturer;
		BYTE uMemoryType;
		BYTE uMemoryCapacity;
		BYTE uReserved;
	};
	[StructLayout(LayoutKind::Sequential)]
	public ref class csVersion
	{
	public:
		BYTE uMinorLSB;
		BYTE uMinorMSB;
		BYTE uMajorLSB;
		BYTE uMajorMSB;
	};

#pragma endregion structures
//////////////////////////////////////////////////////////////////////////
#pragma region delegates
	public delegate int TypeAcquisitionAscan_0x00010103(Object ^pAcquisitionParameter,csAcqInfoEx^ %acqInfo,csHeaderStream_0x0001^ %StreamHeader,csSubStreamAscan_0x0103^ %ascanHeader,const void* pBufferMax,const void* pBufferMin,const void* pBufferSat);
	public delegate int TypeAcquisitionCscan_0x00010X02(Object ^pAcquisitionParameter,csAcqInfoEx^ %acqInfo,csHeaderStream_0x0001^ %streamHeader,csSubStreamCscan_0x0X02^ %cscanHeader,array<csCscanAmp_0x0102^>^ %bufferAmp,array<csCscanAmpTof_0x0202^>^ %bufferAmpTof);
	public delegate int TypeAcquisitionIO_0x00010101(Object ^pAcquisitionParameter,csHeaderStream_0x0001^ %streamHeader,csHeaderIO_0x0001^ %ioHeader);
	public delegate int TypeAcquisitionIO_1x00010101(Object ^pAcquisitionParameter,csAcqInfoEx^ %acqInfo,csHeaderStream_0x0001^ %streamHeader,csHeaderIO_0x0001^ %ioHeader);
	public delegate int TypeAcquisitionInfo(Object ^pAcquisitionParameter,String ^pInfo);

	public delegate int TypeCallbackCustomizedDriverAPI(Object ^pAcquisitionParameter,csEnumStepCustomizedAPI %eStepCustomizedAPI,String ^pInfo,int %iCycleCount);

	public delegate void TypeCallbackSystemMessageBox(const String ^pMsg);
	public delegate void TypeCallbackSystemMessageBoxList(const String ^pMsg);
	public delegate csEnumMsgBoxReturn TypeCallbackSystemMessageBoxButtons(const String ^pMsg,const String ^pTitle,csEnumMsgBoxButtons nType);//should return IDOK IDYES IDNO...depending of the button pressed by the user.
	public delegate csEnumMsgBoxReturn TypeCallbackOempaApiMessageBox(/*HWND hWnd,*/const String ^lpszText,const String ^lpszCaption,csEnumMsgBoxButtons nType);
	ref class csHWDevice;
	public delegate void TypeCallbackHWMemory(csHWDevice ^hwDevice, bool bMaster, unsigned long addr, unsigned long data, unsigned long size);


#pragma endregion delegates
//////////////////////////////////////////////////////////////////////////
#pragma region pin_list
	public ref class csPinListItem
    {
	public:
		csPinListItem(Object^ object_){
			pin_object = object_;
			pin_handle = GCHandle::Alloc(object_, GCHandleType::Pinned);
			pin_pointer = pin_handle.AddrOfPinnedObject();
		};
		~csPinListItem(){Free();};
		void Free()
		{
			if((pin_object!=nullptr) && (pin_pointer!=(IntPtr)NULL))
			{
				pin_handle.Free();
				pin_pointer = (IntPtr)NULL;
			}
		}
		void *Pointer(){
			if(pin_pointer==(IntPtr)NULL)
				return NULL;
			return (void*)pin_pointer.ToPointer();
		}
	private:
		Object^ pin_object;
		GCHandle pin_handle;
		IntPtr pin_pointer;
	protected:
		!csPinListItem(){Free();};
	};
	public ref class csPinList
    {
	private:
		List<csPinListItem^> list;
	public:
		csPinList(){};
		~csPinList(){Free();};
		void Free()
		{
			for each(csPinListItem^ st in list)
			{
				st->Free();
				delete st;
			}
			list.Clear();
		};
		void* Add(Object^ object){
			csPinListItem^ x=gcnew csPinListItem(object);
			list.Add(x);
			return x->Pointer();
		};

	protected:
		!csPinList(){Free();};
	};

	public ref class csDac
	{
	public:
		csDac(double time,float slope)
		{dTime=time;fSlope=slope;};
		double dTime;
		float fSlope;
	};

	public ref class acsDac
	{
	public:
		array<csDac^>^ list;
		acsDac()
		{
			m_pointer=new gcroot<acsDac^>(this);
		};
		~acsDac()
		{
			delete m_pointer;
			m_pointer = NULL;
		};
		csDac^ operator[](int a)
		{
			return list[a];
		};
		gcroot<acsDac^>* GetGcroot(){return m_pointer;}
	private:
		gcroot<acsDac^>* m_pointer;//Array type can't be inherited. Array can't be extended.
	};

	public ref class acsDouble
	{
	public:
		array<double>^ list;
		acsDouble()
		{
			m_pointer=new gcroot<acsDouble^>(this);
		};
		~acsDouble()
		{
			delete m_pointer;
			m_pointer = NULL;
		};
		gcroot<acsDouble^>* GetGcroot(){return m_pointer;}
	private:
		gcroot<acsDouble^>* m_pointer;//Array type can't be inherited. Array can't be extended.
	};

	public ref class acsFloat
	{
	public:
		array<float>^ list;
		acsFloat()
		{
			m_pointer=new gcroot<acsFloat^>(this);
		};
		~acsFloat()
		{
			delete m_pointer;
			m_pointer = NULL;
		};
		gcroot<acsFloat^>* GetGcroot(){return m_pointer;}
	private:
		gcroot<acsFloat^>* m_pointer;//Array type can't be inherited. Array can't be extended.
	};

	public ref class acsDelayReception
	{
	public:
		array<float,2>^ list;
		acsDelayReception()
		{
			m_pointer=new gcroot<acsDelayReception^>(this);
		};
		~acsDelayReception()
		{
			delete m_pointer;
			m_pointer = NULL;
		};
		gcroot<acsDelayReception^>* GetGcroot(){return m_pointer;}
	private:
		gcroot<acsDelayReception^>* m_pointer;//Array type can't be inherited. Array can't be extended.
	};

	public ref class acsByte
	{
	public:
		array<BYTE>^ list;
		acsByte()
		{
			m_pointer=new gcroot<acsByte^>(this);
		};
		~acsByte()
		{
			delete m_pointer;
			m_pointer = NULL;
		};
		gcroot<acsByte^>* GetGcroot(){return m_pointer;}
	private:
		gcroot<acsByte^>* m_pointer;//Array type can't be inherited. Array can't be extended.
	};
public ref class csCProcessingTime
{//ljr 2016
public:
	csCProcessingTime()
	{
		LARGE_INTEGER x;
		QueryPerformanceFrequency(&x);
		m_llFrequency = x.QuadPart;
		Start();
	};
	void Start()
	{
		LARGE_INTEGER x;
		QueryPerformanceCounter(&x);
		m_llTimeStart = x.QuadPart;
		m_llTimeStop = m_llTimeStart;
	};
	void Stop()
	{
		LARGE_INTEGER x;
		QueryPerformanceCounter(&x);
		m_llTimeStop = x.QuadPart;
	};
	double GetProcessingTime()
	{
		double dProcessingTime = (double)(m_llTimeStop-m_llTimeStart)/(double)(m_llFrequency);
		return dProcessingTime;
	};
private:
	__int64 m_llFrequency;
	__int64 m_llTimeStart;
	__int64 m_llTimeStop;//LARGE_INTEGER
};

#pragma endregion pin_list
//////////////////////////////////////////////////////////////////////////
	public ref class csKernelDriver
    {
	public:
		static void GetVersion([Out] String^ %pMsg);
		static char GetVersionLetter();
		static bool CrtCheckMemory();
		static bool CrtSetDbgFlag(bool bEnable);
		//static void debug_EnableHeapEx(bool bEnable, String ^pFileName);
		//static bool debug_DumpHeap(String ^pFileName,bool bStatistics);
	};
	public ref class csMsgBox
    {
	private:
		static TypeCallbackSystemMessageBox ^g_CallbackSystemMessageBox;
		static TypeCallbackSystemMessageBoxList ^g_CallbackSystemMessageBoxList;
		static TypeCallbackSystemMessageBoxButtons ^g_CallbackSystemMessageBoxButtons;
		static TypeCallbackOempaApiMessageBox ^g_CallbackOempaApiMessageBox;
		static csMsgBox();
	public:
		//To call the functions used by the driver to display the popup.
		static void SystemMessageBox(String ^pMsg);
		static void SystemMessageBoxList(String ^pMsg);
		static csEnumMsgBoxReturn SystemMessageBoxButtons(String ^pMsg,String ^pTitle,csEnumMsgBoxButtons nType);
		static csEnumMsgBoxReturn OempaApiMessageBox(String ^pMsg,String ^pTitle,csEnumMsgBoxButtons nType);

		//function to register callback to avoid the popup.
		static void SetCallbackSystemMessageBox(TypeCallbackSystemMessageBox ^pCallback);
		static void SetCallbackSystemMessageBoxList(TypeCallbackSystemMessageBoxList ^pCallback);
		static void SetCallbackSystemMessageBoxButtons(TypeCallbackSystemMessageBoxButtons ^pCallback);
		static void SetCallbackOempaApiMessageBox(TypeCallbackOempaApiMessageBox ^pCallback);

		static TypeCallbackSystemMessageBox ^GetCallbackSystemMessageBox();
		static TypeCallbackSystemMessageBoxList ^GetCallbackSystemMessageBoxList();
		static TypeCallbackSystemMessageBoxButtons ^GetCallbackSystemMessageBoxButtons();
		static TypeCallbackOempaApiMessageBox ^GetCallbackOempaApiMessageBox();

		static void CallbackSystemMessageBox(const wchar_t *pMsg);
		static void CallbackSystemMessageBoxList(const wchar_t *pMsg);
		static unsigned int CallbackSystemMessageBoxButtons(const wchar_t *pMsg,const wchar_t *pTitle,UINT nType);
		static int CallbackOempaApiMessageBox(HWND hWnd,LPCTSTR lpszText,LPCTSTR lpszCaption,UINT nType);

		static bool IsUserInterfaceThread();//this function could be called to know if the current thread is attached to the management of window.
	};
	ref class csHWDeviceOEMPA;
	public ref class csCustomizedAPI
    {
	private:
		CHWDeviceOEMPA *m_pHWDeviceOEMPA;
		Object ^m_csAcquisitionParameter;
		structRoot *m_pRoot;
		structCycle *m_pCycle;
		CFocalLaw *m_pEmission,*m_pReception;
		int m_iCycleCount;
	public:
		csCustomizedAPI(CHWDeviceOEMPA *pHWDeviceOEMPA);
		~csCustomizedAPI();
		void Free();

		bool ReadFileWriteHW(String ^pValue);
		bool ReadHWWriteFile(String ^pValue);
		bool WriteHW(csHWDeviceOEMPA^ %pOEMPA,csRoot^ %root,array<csCycle^>^ %cycle,array<csFocalLaw^>^ %emission,array<csFocalLaw^>^ %reception,csEnumAcquisitionState eAcqState);

	//callback functions
		bool GetRoot([Out] csRoot^ %root);
		bool SetRoot([In] csRoot^ %root);
		bool GetCycle(int iCycle,[Out] csCycle^ %cycle);
		bool SetCycle(int iCycle,[In] csCycle^ cycle);
		bool GetFocalLaw(bool bEmission,int iCycle,[Out] csFocalLaw^ %focalLaw);
		bool SetFocalLaw(bool bEmission,int iCycle,[In] csFocalLaw^ focalLaw);

	//callback management
		bool SetCallbackCustomizedDriverAPI(TypeCallbackCustomizedDriverAPI ^pProcess);
		TypeCallbackCustomizedDriverAPI ^GetCallbackCustomizedDriverAPI();
		TypeCallbackCustomizedDriverAPI ^m_csCallback;
		bool SetAcquisitionParameter(Object ^pParameter);//first parameter of the callback acquisition function.
		Object ^GetAcquisitionParameter();
		void CallbackCustomizedAPI(const wchar_t *pFileName,enumStepCustomizedAPI eStepCustomizedAPI,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);

	protected:
		!csCustomizedAPI();
	};

//////////////////////////////////////////////////////////////////////////
#pragma region csSWEncoder
    public ref class csSWEncoder
    {
	private:
		//CHWDevice *m_pHWDevice;
		CSWEncoder *m_pSWEncoder;
		void Constructor(CSWEncoder *pSWEncoder);
	public:
		csSWEncoder(CSWEncoder *pSWEncoder);
		~csSWEncoder();
		void Free();

	//setting part
		bool Enable(bool bEnabled);//value could be negative if encoder need to be inverted.
		bool IsEnabled();

		bool lSetResolution(long lResolution);//this function is only for integer type resolution (the resolution divider is equal to 1).
		bool lSetResolution(long lResolution,DWORD dwDivider);//divider is useful for a double type resolution encoder. DoubleResolution=Resolution/Divider.
		long lGetResolution();
		DWORD GetDivider();
		bool dSetResolution(double dResolution);//it is the same than to call "ConvertResolution(dResolution,lResolution,dwDivider);" followed by "SetResolution(lResolution,dwDivider)"
		double dGetResolution();

		bool SetUnit(csEnumUnit eUnit);
		csEnumUnit GetUnit();

		bool SetType(csEnumEncoderType eEncoderType);
		csEnumEncoderType GetType();

		bool SetDigitalInput(csEnumIndexDigitalInput eEncoderInput,csEnumDigitalInput eDigitalInput);
		csEnumDigitalInput GetDigitalInput(csEnumIndexDigitalInput eEncoderInput);

	//acquisition part
		bool SetSpeedDistance(double dValue);
		bool GetSpeedDistance([Out] double %dValue);

		bool SetInspectionHWValue(int iValue);
		int GetInspectionHWValue();
		bool SetInspectionSWValue(double dValue);
		double GetInspectionSWValue();

		bool SetInspectionSpeed(double dValue);
		double GetInspectionSpeed();

		bool SetInspectionLength(double dValue);
		double GetInspectionLength();

		bool SetInspectionCount(int iValue);
		int GetInspectionCount();

	protected:
		!csSWEncoder();
	};
#pragma endregion csSWEncoder

#pragma region csSWDevice
	public ref class csSWDevice
    {
	private:
		csSWEncoder ^m_csSWEncoder1;
		csSWEncoder ^m_csSWEncoder2;
		CHWDeviceOEMPA *m_pHWDeviceOEMPA;
		CHWDevice *m_pHWDevice;
		CSWDevice *m_pSWDevice;
	public:
		csSWDevice();
		~csSWDevice();
		void Constructor(CHWDeviceOEMPA *pHWDeviceOEMPA,CHWDevice *pHWDevice);
		void Free();
		csSWEncoder ^GetSWEncoder(int iEncoderIndex);

	//root ID management.
		bool SetBoardName(String ^pName);//to register this new data in the kernel database.
		bool GetBoardName([Out] String^ %pName);

	//Setting
		bool GetConfigurationFilePath(bool bFlashName,String ^pPathName);//to find the configuration file path.

		bool SetSetupFileDefault(bool bOEMPA,String ^wcFile);
		bool GetSetupFileDefault(bool bOEMPA,[Out] String^ %wcFile);
		bool SetSetupFileCurrent(String ^wcFile);
		bool GetSetupFileCurrent([Out] String^ %wcFile);

		//bool SetAcquisitionFlush(enumAcquisitionFlush eAcquisitionFlush);
		//enumAcquisitionFlush GetAcquisitionFlush();

		bool SetConnectionState(csEnumConnectionState eConnection,bool bDisplayErrorMsg);
		csEnumConnectionState GetConnectionState();
		bool IsConnected();

		bool CheckProcessId();//only one process have ownership of the driver.

		bool SetCfgStatus(csEnumUpdateStatus eUpdateStatus);
		csEnumUpdateStatus GetCfgStatus();

		int SetStreamCount(int iCount);
		int GetStreamCount();
		int SetStreamRetransmit(int iCount);
		int GetStreamRetransmit();
		int SetStreamError(int iCount);
		int GetStreamError();

		int GetLostCountAscan();
		int SetLostCountAscan(int iDataLostCount);
		int GetLostCountCscan();
		int SetLostCountCscan(int iDataLostCount);
		int GetLostCountEncoder();
		int SetLostCountEncoder(int iDataLostCount);
		int GetLostCountUSB3();
		int SetLostCountUSB3(int iDataLostCount);
		int GetErrorCountUSB3();
		int GetLostCountFifo(csEnumAcquisitionFifo csFifo);//data (ascan, cscan or IO) lost because the output speed to get data is too low.
		void ResetCounters();

		//bool SetEncoderCount(int iCount);//0 1 or 2.
		int GetEncoderEnabledCount();
		void swProcessEncoder(structAcqInfoEx &acqInfo);

		bool EnablePulser(bool bEnable);
		bool IsPulserEnabled();

		bool SetLockDefaultDisablePulser(bool bDisable);
		bool GetLockDefaultDisablePulser();
		bool SetUnlockDefaultEnablePulser(bool bEnable);
		bool GetUnlockDefaultEnablePulser();

		bool SetAcqSpeedAscan(double dSpeed);
		double GetAcqSpeedAscan();
		bool SetAcqSpeedCscan(double dSpeed);
		double GetAcqSpeedCscan();
		bool SetAcqSpeedIO(double dSpeed);
		double GetAcqSpeedIO();

	protected:
		!csSWDevice();
	};
#pragma endregion csSWDevice

#pragma region csHWDevice
	ref class csAcquisitionFifo;
	public ref class csHWDevice
    {
	private:
		csPinList ^m_List;
		csSWDevice ^m_csSWDevice;
		CHWDevice *m_pHWDevice;
		CSWDevice *m_pSWDevice;
		Object ^m_csAcquisitionParameter;
		csCustomizedAPI ^m_csCustomizedAPI;
		bool m_bDigitalEdgesOnly;
	public:
		csHWDevice();
		~csHWDevice();
		void Constructor(CHWDeviceOEMPA *pHWDeviceOEMPA,CHWDevice *pHWDevice,csCustomizedAPI ^_csCustomizedAPI);
		void Free();
		csSWDevice ^GetSWDevice();
		int GetDeviceId();
		static int GetMonitorPort(int iDeviceId);

//MultiSystem management start (shared functions between applications)
	csEnumHardwareLink GetHardwareLink();
	int GetMasterDeviceId();

	//FUNCTION to call before connection (this information is registered in the default setup OEMPA file)
	bool SetDefaultHwLink(csEnumHardwareLink csHardwareLink,[Out] bool %bPreviousMasterUnregistered);
	csEnumHardwareLink GetDefaultHwLink();
	bool IsDefaultHwLinkEnabled();

	//FUNCTION to call after connection
	//DO NOT CALL LockDevice/UnlockDevice to call following:
	bool SlaveConnect(int iMasterDeviceId);
	bool SlaveDisconnect();

	//Device low level management
		//Those function can be used to receive acquisition data.
		bool SetAcquisitionParameter(Object ^pParameter);//first parameter of the callback acquisition function.
		Object ^GetAcquisitionParameter();
		bool SetAcquisitionAscan_0x00010103(TypeAcquisitionAscan_0x00010103 ^pProcess);
		TypeAcquisitionAscan_0x00010103 ^GetAcquisitionAscan_0x00010103();
		bool SetAcquisitionCscan_0x00010X02(TypeAcquisitionCscan_0x00010X02 ^pProcess);
		TypeAcquisitionCscan_0x00010X02 ^GetAcquisitionCscan_0x00010X02();
		bool SetAcquisitionIO_0x00010101(TypeAcquisitionIO_0x00010101 ^pProcess,bool bDigitalEdgesOnly);
		TypeAcquisitionIO_0x00010101 ^GetAcquisitionIO_0x00010101([Out] bool^ %bDigitalEdgesOnly);
		bool SetAcquisitionIO_1x00010101(TypeAcquisitionIO_1x00010101 ^pProcess,bool bDigitalEdgesOnly);
		TypeAcquisitionIO_1x00010101 ^GetAcquisitionIO_1x00010101([Out] bool^ %bDigitalEdgesOnly);

		virtual csAcquisitionFifo ^GetAcquisitionFifo(csEnumAcquisitionFifo csFifo);

		bool SetAcquisitionInfo(TypeAcquisitionInfo ^pProcess);
		TypeAcquisitionInfo ^GetAcquisitionInfo();

		bool IsDriverEncoderManagementEnabled();
		void EnableDriverEncoderManagement(bool bEnable);

	//lock/unlock the device
	//you have to lock the device before to use its functions (for example "SetStandby" function).
	//(the communication link with the HW is shared between all threads, only one at a time
	//can lock it)
		bool LockDevice();
		bool LockDevice(csEnumAcquisitionState eAcqState);//use "eAcqOff" to disable shot pulse.
						//eAcqDefault is used to read the value from the configuration file.
						//return true if no error and false in case of communication error.
		bool UnlockDevice();
		bool UnlockDevice(csEnumAcquisitionState eAcqState);//use "eAcqOn" to enable shot pulse again.
						//eAcqDefault is used to read the value from the configuration file.
						//return true if no error and false in case of communication error.
		bool IsDeviceLocked();
		DWORD GetSettingId();

	//management of the device (can be called after device has been locked).
		bool Flush();//return true if no error and false in case of communication error.

	//Don't use those very low level functions, they are reserved for advanced user.
		bool WriteHW(DWORD dwAddress,DWORD dwData,int iSize);
		/*unsafe*/bool ReadHW(DWORD dwAddress,[Out] DWORD *pdwData,int iSize);
		bool SetCallbackHWMemory(TypeCallbackHWMemory ^callbackHWMemory);
		TypeCallbackHWMemory ^GetCallbackHWMemory();

	//callback management
		gcroot<csHWDevice^>* m_pointer;

		TypeAcquisitionAscan_0x00010103 ^m_csAscan;
		TypeAcquisitionCscan_0x00010X02 ^m_csCscan;
		TypeAcquisitionIO_0x00010101 ^m_csIo0;
		TypeAcquisitionIO_1x00010101 ^m_csIo1;
		TypeAcquisitionInfo ^m_csInfo;
		TypeCallbackHWMemory ^g_CallbackHWMemory;

		int AcquisitionAscan_0x00010103(structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
		int AcquisitionCscan_0x00010X02(structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof);
		int AcquisitionIO_0x00010101(const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader);
		int AcquisitionIO_1x00010101(structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader);
		int AcquisitionInfo(const wchar_t *pInfo);
		void CallbackCustomizedAPI(const wchar_t *pFileName,enumStepCustomizedAPI eStepCustomizedAPI,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
		void CallbackHWMemory(bool bMaster, unsigned long addr, unsigned long data, int size);
		void* ListAddObject(Object^ object);
		void test();
	protected:
		!csHWDevice();
	};
#pragma endregion hwDriver

//////////////////////////////////////////////////////////////////////////
#pragma region csSWFilterOEMPA
   public ref class csSWFilterOEMPA
    {
	private:
		CHWDeviceOEMPA *m_pHWDeviceOEMPA;
		CSWDeviceOEMPA *m_pSWDeviceOEMPA;
		CSWFilterOEMPA *m_pSWFilterOEMPA;
	public:
		csSWFilterOEMPA();
		~csSWFilterOEMPA();
		void Constructor(CHWDeviceOEMPA *pHWDeviceOEMPA,CSWDeviceOEMPA *pSWDeviceOEMPA,CSWFilterOEMPA *pSWFilterOEMPA);
		void Free();

		bool SetFilter(csEnumOEMPAFilter eFilter);
		bool GetFilter([Out] csEnumOEMPAFilter %eFilter);
		bool SetTitle(String^ pValue);//useful for custom filter.
		bool GetTitle([Out] String^ %pValue);

		//custom filter coefficient: functions used by the "CustomFilter" software.
		bool SetScale(WORD wScale);
		bool GetScale([Out] WORD %wScale);
		bool SetCoefficientCount(int iCoefficientCount);
		bool GetCoefficientCount([Out] int %iCoefficientCount);
		bool SetCoefficient(int iCoefficientIndex,short wValue);
		bool GetCoefficient(int iCoefficientIndex,[Out] short %wValue);
		bool SetFilter(WORD wScale,array<short>^ wValue,bool bUpdateHardware);//if you want to update all hardware filter, it is quicker to call "CSWDeviceOEMPA::UpdateAllFilter" at the end and before to call "SetFilter" with "bUpdateHardware=false" for all filters.
		bool GetFilter([Out] WORD %wScale,[Out] array<short>^ %wValue);
	protected:
		!csSWFilterOEMPA();
	};
#pragma endregion csSWFilterOEMPA

#pragma region csSWDeviceOEMPA
    public ref class csSWDeviceOEMPA
    {
	private:
		array<csSWFilterOEMPA^> ^m_acsSWFilterOEMPA;
		CHWDeviceOEMPA *m_pHWDeviceOEMPA;
		CSWDeviceOEMPA *m_pSWDeviceOEMPA;
	public:
		csSWDeviceOEMPA();
		~csSWDeviceOEMPA();
		void Constructor(CHWDeviceOEMPA *pHWDeviceOEMPA,CSWDeviceOEMPA *pSWDeviceOEMPA);
		void Free();
		csSWFilterOEMPA^ Filter(int iFilterIndex);

		bool IsPulserEnabled();

	//ethernet
		bool SetIPAddress(String ^pValue);
		bool GetIPAddress([Out] String^ %pValue);

		bool SetPort(unsigned short usValue);
		bool _SetPort(unsigned short usValue);//same than "SetPort", required because of the preprocessor definition in "WinSpool.h": "#define SetPort SetPortW".
		bool GetPort(unsigned short %usValue);

	//USB3
		bool IsUSB3Connected();

	//status registers (digital inputs and temperatures)
		bool SetStatusRegister(csEnumUpdateStatus eCom);//set UpToDate to refresh status register.
		csEnumUpdateStatus GetStatusRegister();

		bool GetSerialNumber([Out] String^ %pSN);

		bool GetSystemType([Out] String^ %pType);

		int GetRXBoardCount();
		int GetApertureCountMax();//to get the maximum element count of an aperture.
		int GetElementCountMax();//to get the maximum element count of the system (in case of mux).

		const double dGetClockPeriod();//ns
		const float fGetClockPeriod();//ns
		const long lGetClockFrequency();//Hz

		WORD GetFirmwareId();

		int GetCycleCountMax();
		bool IsFullMatrixCapture();//FMC could be enabled or disabled.
		bool IsFullMatrixCaptureReadWrite();//FMC bit could be Read/Write or Read only (for old FMC FW: FMC bit is not read only).
		bool GetFMCElement([Out] int %iElementStart, [Out] int %iElementStop, [Out] int %iElementStep);
		bool IsMultiHWChannelSupported();
		bool IsTemperatureAlarmSupported();
		bool IsMultiHWChannelEnabled();
		bool IsMatrixAvailable();
		bool IsLabviewAvailable();
		bool IsTpacquisitionAvailable();
		bool IsWTSWAvailable();
		bool IsEncoderDecimal();
		bool IsFMCElementStepSupported();

		bool SetKeepAlive(csEnumKeepAlive eKeepAlive);
		csEnumKeepAlive GetKeepAlive();

		bool EnableAscan(bool bEnable);
		bool IsAscanEnabled();

		bool SetAscanBitSize(csEnumBitSize eBitSize);
		csEnumBitSize GetAscanBitSize();

		bool SetAscanRequest(csEnumAscanRequest eAscanRequest);
		csEnumAscanRequest GetAscanRequest();

		bool SetAscanRequestFrequency(double dFreq);//Hz
		bool GetAscanRequestFrequency([Out] double %dFreq);

		bool EnableCscanTof(bool bEnable);
		bool IsCscanTofEnabled();

		bool SetCycleCount(int iCount);
		int GetCycleCount();

		bool SetTriggerMode(csEnumOEMPATrigger eTrig);
		csEnumOEMPATrigger GetTriggerMode();

		bool SetTriggerEncoderStep(double dStep);
		bool GetTriggerEncoderStep([Out] double %dStep);

		bool SetSignalTriggerHighTime(double dTime);
		double GetSignalTriggerHighTime();

		bool SetRequestIO(csEnumOEMPARequestIO eRequest);
		csEnumOEMPARequestIO GetRequestIO();

		bool SetRequestIODigitalInputMaskRising(int iMask);
		bool GetRequestIODigitalInputMaskRising([Out] int %iMask);

		bool SetRequestIODigitalInputMaskFalling(int iEvent);
		bool GetRequestIODigitalInputMaskFalling([Out] int %iEvent);

		bool SetExternalTriggerCycle(csEnumDigitalInput eDigitalInput);
		bool GetExternalTriggerCycle([Out] csEnumDigitalInput %eDigitalInput);

		bool SetExternalTriggerSequence(csEnumDigitalInput eDigitalInput);
		bool GetExternalTriggerSequence([Out] csEnumDigitalInput %eDigitalInput);

		bool SetMappingOutput(int iOutputIndex,csEnumOEMPAMappingDigitalOutput eMapping);
		bool GetMappingOutput(int iOutputIndex,[Out] csEnumOEMPAMappingDigitalOutput %eMapping);

		bool SetRequestCscan(csEnumOEMPARequestCscan eRequest);
		csEnumOEMPARequestCscan GetRequestCscan();

		bool SetEncoderDebouncer(double dTime);
		double GetEncoderDebouncer();

		bool SetDigitalInput(WORD usValue);
		WORD GetDigitalInput();

		bool SetDigitalDebouncer(double dTime);
		double GetDigitalDebouncer();

		bool ResetEncoder();

		bool GetTemperatureCount([Out] int %iBoardCount,[Out] int %iSensorCountMax);
		bool GetTemperatureSensorCount(int iBoardIndex,[Out] int %iSensorCount);
		bool GetTemperature(int iBoardIndex,int iSensorIndex,[Out] float %fValue);

		//SW member to enable the high level API update of acquisition status, see preferences member "AcqStatusEnable" and "AcqStatusTimer".
		bool EnableTimerUpdate(bool bEnable);

		bool IsIOBoardEnabled();
		bool IsOEMMCEnabled();
		double GetPulserPowerMax();
		double GetPulserPowerCurrent();
		BYTE GetFlashUSB3Version();
		DWORD GetFWUSB3Version();
		bool EnableUSB3(bool bEnable);
		bool IsUSB3Enabled();
		DWORD GetMBOptions();

		bool GetEmbeddedVersion([Out] csVersion^ %version);
		bool GetOptionsCom([Out] csOptionsCom^ %optionsCom);
		bool GetOptionsTCP([Out] csOptionsTCP^ %optionsTCP);
		bool GetOptionsFlash([Out] csOptionsFlash^ %optionsFlash);
		int GetPasscodeCount();
		bool GetPasscode(int iIndex,[Out] DWORD %dwPasscode);
		double GetMaximumThroughput();//Unit is Bytes/second.

		bool UpdateAllFilter();//to update all hardware filters at the same time.

		static void EnableMultiChannel(bool bEnable);//you need to restart the sw to take the new value in account. "CUTChannels::SetDefaultMultiChannels" is the same.
		static bool IsMultiChannelEnabled();
		static void EnableLoadDefaultSetup(bool bEnable);//you need to restart the sw to take the new value in account. "CUTPreference::SetDefaultSetupAuto" is the same.
		static bool IsLoadDefaultSetupEnabled();

		bool IsTimeOffsetSupported();
		bool EnableAlignment(bool bEnable);
		float GetCalibrationAlignment();//maximum correction.
		float GetCalibrationOffset();//required value after calibration has been performed.
		bool IsCalibrationPerformed();//same than "IsAlignmentPerformed"
		bool IsAlignmentPerformed();
		bool IsAlignmentEnabled();
		bool ResetAlignment();
		static void AlignmentCfgUpdated();
		void SetCalibrationParameters(float fWidth,float fStart,float fRange,float fGainAnalog,double dGainDigital);
		void GetCalibrationParameters(float &fWidth,float &fStart,float &fRange,float &fGainAnalog,double &dGainDigital);
		bool SetCalibrationFileReport(String ^pFileReport);
		bool GetCalibrationFileReport([Out] String^ %pFileReport);

		//The calibration time offset is internally added to RecoveryTimes.
		bool SetTimeOffset(float fTimeOffset);
		float GetTimeOffset();//current value used to correct time axis.

		//For OEM-MC or OEM-PA (VF).
		void EnablePulserDuringReplay(bool bEnable);
		bool IsPulserDuringReplayEnabled();
		bool EnableCscanTimeOfFlightCorrection(bool bEnable);
		bool IsCscanTimeOfFlightCorrectionEnabled();
		bool GetCscanTimeOfFlightCorrection(int iCycle,BYTE %byDecimation,float %fAscanStart);//decimation range is 1..16.
		bool SetCscanTimeOfFlightCorrection(int iCycle,BYTE byDecimation,float fAscanStart);//decimation range is 1..16.

		double GetFWAscanRecoveryTime();
		double GetFMCSubCycleRecoveryTime();
		double GetFMCCycleRecoveryTime();
	protected:
		!csSWDeviceOEMPA();
	};
#pragma endregion csSWDeviceOEMPA

#pragma region csHWDeviceOEMPA
	public ref class csHWDeviceOEMPA : public csHWDevice
    {
	private:
		csCustomizedAPI ^m_csCustomizedAPI;
		csSWDeviceOEMPA ^m_csSWDeviceOEMPA;
		CHWDeviceOEMPA *m_pHWDeviceOEMPA;
		CHWDevice *m_pHWDevice;
		CSWDeviceOEMPA *m_pSWDeviceOEMPA;
		csAcquisitionFifo ^m_FifoAscan;
		csAcquisitionFifo ^m_FifoCscan;
		csAcquisitionFifo ^m_FifoIO;
	public:
		csHWDeviceOEMPA();
		~csHWDeviceOEMPA();
		void Free();
		csSWDeviceOEMPA^ GetSWDeviceOEMPA();
		csHWDevice ^GetHWDevice();
		csCustomizedAPI ^GetCustomizedAPI();
		virtual csAcquisitionFifo ^GetAcquisitionFifo(csEnumAcquisitionFifo csFifo) override;

		/*unsafe*/bool GetDigitalInput(DWORD *pdwData);//reading the current state of digital inputs

		bool DisableUSB3(bool bDisable);
		/*unsafe*/bool GetUSB3Disabled(/*fixed*/[Out] bool *pbDisable);

		/*unsafe*/bool GetApertureCountMax(int *piCount);//to get the maximum element count of an aperture.
		/*unsafe*/bool GetElementCountMax(int *piCount);//to get the maximum element count of the system (in case of mux).
			//Output parameters
			//	piCount : maximum aperture size.

		/*unsafe*/bool GetTemperatureSensor(int iIndexBoard,int iIndexSensor,WORD *pwTemperature);//FW parameter
								//iIndexBoard: 0=ComBoard, 1=first RX board, 2=second RX board, 3=third RX board, 4=four RX board.
		bool SetTemperatureAlarm(BYTE &byWarning,BYTE &byAlarm);
		/*unsafe*/bool GetTemperatureAlarm(BYTE *pbyWarning,BYTE *pbyAlarm);

		//MultiProcess management begin
		static bool IsMultiProcessRegistered();
		static bool RegisterMultiProcess(String ^wcProcessName);//call this function before instantiating any device.
		static bool UnregisterMultiProcess();
		static bool IsMultiProcessConnected(String ^wcIPAddress,[Out] DWORD %dwProcessId);//call this function to know if another process has been already connected with the device.
				//Input "wcIPAddress": IP address of the device.
				//Output "dwProcessId": process ID of the process that has already been connected with the device.
				//return true if any other process has already been connected with the device, false otherwise.
		static bool DisconnectMultiProcess(String ^wcIPAddress,DWORD dwProcessId);//call this function to disconnect the device of another process (.
				//Input "wcIPAddress": IP address of the device.
				//Input "dwProcessId": process ID of the process that has already been connected with the device.
				//return true if the device has been disconnected, false otherwise.
		static int GetMultiProcessCount();//to get the count of other process that has been registered with function "RegisterMultiProcess".
		static bool GetMultiProcessInfo(int iIndex,[Out] DWORD %dwProcessId,[Out] String ^wcProcessName);//to get information of other process that has been registered with function "RegisterMultiProcess".
		//MultiProcess management end

		bool EnableFMC(bool bEnable);//return false if the FW does not support this feature (old FMC FW).
		/*unsafe*/bool GetEnableFMC(bool *pbEnable);//return false if the FW does not support this feature (old FMC FW).
		//you can use the following functions in any case (step equal or different to 1):
		bool SetFMCElement(int %iElementStart,int %iElementStop,int %iElementStep);
		/*unsafe*/bool GetFMCElement(int *piElementStart,int *piElementStop,int *piElementStep);
		//you can use the following functions only if the step is 1:
		bool SetFMCElementStart(int %iElementIndex);
		/*unsafe*/bool GetFMCElementStart(int *piElementIndex);
		bool SetFMCElementStop(int %iElementIndex);
		/*unsafe*/bool GetFMCElementStop(int *piElementIndex);
	
		bool EnableMultiHWChannel(bool bEnable);//return false if the FW does not support the MultiChannel feature.
		/*unsafe*/bool GetEnableMultiHWChannel(bool *pbEnable);//return false if the FW does not support the MultiChannel feature.

		bool ResetTimeStamp();

		bool ResetEncoder(int iEncoderIndex);
		bool SetEncoder(int iEncoderIndex,double %dValue);//unit=meter
		bool SetEncoder(int iEncoderIndex,DWORD %dwValue);//unit=encoder step
		bool SetEncoderType(csEnumEncoderType %eEncoder1Type,csEnumEncoderType %eEncoder2Type);
		/*unsafe*/bool GetEncoderType(csEnumEncoderType *peEncoder1Type,csEnumEncoderType *peEncoder2Type);

		bool EnableAscan(bool bAscan);//to enable Ascan acquisition that will be sent by the FW to the SW.
		/*unsafe*/bool GetEnableAscan(bool *pbAscan);

		//Cscan time of flight : you have frist to define gates (see "SetGateXXX").
		bool EnableCscanTof(bool bCscanTof);//to enable Cscan time of flight acquisition that will be sent by the FW to the SW.
		/*unsafe*/bool GetEnableCscanTof(bool *pbCscanTof);

		bool SetAscanBitSize(csEnumBitSize %eBitSize);//Ascan data size.
		/*unsafe*/bool GetAscanBitSize(csEnumBitSize *peBitSize);

		bool SetAscanRequest(csEnumAscanRequest %eAscanRequest);
		/*unsafe*/bool GetAscanRequest(csEnumAscanRequest *peAscanRequest);

		bool SetAscanRequestFrequency(double %dValue);
		/*unsafe*/bool GetAscanRequestFrequency(double *pdValue);
		bool CheckAscanRequestFrequency(double %dValue);

//<<PARAMETERS MANAGEMENT FUNCTIONS : BEGIN>>
	//low level API
		/*unsafe*/bool GetFWId(WORD *pwFWId);//to get the device version.

		bool SetCycleCount(int %lCycleCount);//to write the cycle count.
		/*unsafe*/bool GetCycleCount(/*fixed*/[Out] int *piCycleCount);//to read the cycle count.
		bool CheckCycleCount(int %iCycleCount);//to check the cycle count (minimum value, maximum value).

		bool SetTriggerMode(csEnumOEMPATrigger %eTrig);
		/*unsafe*/bool GetTriggerMode(/*fixed*/[Out] csEnumOEMPATrigger *peTrig);

		bool SetEncoderDirection(csEnumOEMPAEncoderDirection &eEncoderDirection);
		/*unsafe*/bool GetEncoderDirection(/*fixed*/[Out] csEnumOEMPAEncoderDirection *peEncoderDirection);

		//It is better to use following function (unit is meter), previously you have to set the resolution ("GetSWEncoder(int iEncoder)->lSetResolution()").
		bool SetTriggerEncoderStep(double %dStep);//unit=meter
		/*unsafe*/bool GetTriggerEncoderStep(/*fixed*/[Out] double *pdStep);//unit=meter

		bool SetSignalTriggerHighTime(double %dTime);
		/*unsafe*/bool GetSignalTriggerHighTime(/*fixed*/[Out] double *pdTime);

		bool SetRequestIO(csEnumOEMPARequestIO %eRequest);
		/*unsafe*/bool GetRequestIO(/*fixed*/[Out] csEnumOEMPARequestIO *peRequest);

		bool SetRequestIODigitalInputMask(int %iMaskFalling,int %iMaskRising);
		/*unsafe*/bool GetRequestIODigitalInputMask(/*fixed*/[Out] int *piMaskFalling,/*fixed*/[Out] int *piMaskRising);
		bool CheckRequestDigitalInputMask(int %iMask);

		/*unsafe*/bool GetFilterCoefficient(csEnumOEMPAFilter eFilter,/*fixed*/WORD *pwScale,/*fixed*/[Out] array<short>^ %wValue/*g_iOEMPAFilterCoefficientMax*/);
		bool FindFilterCoefficient(WORD %wScale,array<short>^ %wValue/*g_iOEMPAFilterCoefficientMax*/,[Out] csEnumOEMPAFilter^ %eFilter);
		bool SetFilter(csEnumOEMPAFilterIndex eFilterIndex,WORD %wScale,array<short>^ %wValue/*g_iOEMPAFilterCoefficientMax*/);
		/*unsafe*/bool GetFilter(csEnumOEMPAFilterIndex eFilterIndex,/*fixed*/WORD *pwScale,/*fixed*/[Out] array<short>^ %wValue/*g_iOEMPAFilterCoefficientMax*/);

		bool SetEncoderWire1(int iEncoderIndex,csEnumDigitalInput %eDigitalInput);//int iEncoderIndex : 0 for first encoder, 1 for second encoder.
		bool SetEncoderWire2(int iEncoderIndex,csEnumDigitalInput %eDigitalInput);
		bool SetExternalTriggerCycle(csEnumDigitalInput %eDigitalInput);
		bool SetExternalTriggerSequence(csEnumDigitalInput %eDigitalInput);
		/*unsafe*/bool GetEncoderWire1(int iEncoderIndex,/*fixed*/[Out] csEnumDigitalInput *peDigitalInput);
		/*unsafe*/bool GetEncoderWire2(int iEncoderIndex,/*fixed*/[Out] csEnumDigitalInput *peDigitalInput);
		/*unsafe*/bool GetExternalTriggerCycle(/*fixed*/[Out] csEnumDigitalInput *peDigitalInput);
		/*unsafe*/bool GetExternalTriggerSequence(/*fixed*/[Out] csEnumDigitalInput *peDigitalInput);

		bool SetDigitalOutput(int iIndex,csEnumOEMPAMappingDigitalOutput eMappingDigitalOutput);
		/*unsafe*/bool GetDigitalOutput(int iIndex,/*fixed*/[Out] csEnumOEMPAMappingDigitalOutput *peMappingDigitalOutput);

		bool SetEncoderDebouncer(double %dValue);
		/*unsafe*/bool GetEncoderDebouncer(/*fixed*/[Out] double *pdValue);
		bool CheckEncoderDebouncer(double %dValue);

		bool SetDigitalDebouncer(double %dValue);
		/*unsafe*/bool GetDigitalDebouncer(/*fixed*/[Out] double *pdValue);
		bool CheckDigitalDebouncer(double %dValue);

		bool SetGainDigital(int iCycle,double %dGain);//to write the amplification.
		/*unsafe*/bool GetGainDigital(int iCycle,/*fixed*/[Out] double *pdGain);//to read the amplification.
		bool CheckGainDigital(double %dGain);//to check the amplification value (minimum, maximum, step).

		bool SetBeamCorrection(int iCycle,float %fGain);
		/*unsafe*/bool GetBeamCorrection(int iCycle,float *pfGain);
		bool CheckBeamCorrection(float %fGain);

		bool SetDACSlope(int iCycle,acsDac^ %dac/*int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac*/);
		bool GetDACSlope(int iCycle,[Out] acsDac^ %dac/*int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac*/);
		bool SetDACGain(bool bAutoStop,int iCycle,acsDac^ %dac/*int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac*/);
		bool GetDACGain(int iCycle,[Out] acsDac^ %dac/*int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac*/);
		bool EnableDAC(int iCycle,bool %bEnable);
		/*unsafe*/bool GetEnableDAC(int iCycle,bool *pbEnable);
		bool CheckDACSlope(double %dTime,float %fSlope);
		bool CheckDACCount(int %iCount);

		bool SetAscanRectification(int iCycle,csEnumRectification %eRectification);
		/*unsafe*/bool GetAscanRectification(int iCycle,csEnumRectification *peRectification);

		bool SetAscanStart(int iCycle,double %dStart);
		/*unsafe*/bool GetAscanStart(int iCycle,double *pdStart);
		bool CheckAscanStart(double %dStart);
	
		bool SetAscanRange(int iCycle,double %dRange,/*output only*/[Out] csEnumCompressionType %eCompressionType,/*output only*/[Out] long %lPointCount,/*output only*/[Out] long %lPointFactor);//list count is automatically calculated from the digitizing clock, compression could be used.
		bool SetAscanRangeWithFactor(int iCycle,double %dRange,csEnumCompressionType %eCompressionType,/*in/out*/long %lPointFactor,/*output only*/[Out] long %lPointCount);
		bool SetAscanRangeWithCount(int iCycle,double %dRange,/*in/out (check)*/csEnumCompressionType %eCompressionType,/*in/out (check)*/long %lPointCount,/*output only*/[Out] long %lPointFactor);
		/*unsafe*/bool GetAscanRange(int iCycle,double *pdRange,csEnumCompressionType *peCompressionType,long *plPointCount,long *plPointFactor);
		bool CheckAscanRange(double %dRange);
		bool CheckAscanRangeWithCount(double %dRange,csEnumCompressionType %eCompressionType,long %lPointCount);
		bool GetSamplingFrequency(csEnumCompressionType %eCompressionType,long %lPointFactor,[Out] double %dSamplingFrequency/*Hertz*/);
		
		bool SetFilterIndex(int iCycle,csEnumOEMPAFilterIndex %eFilterIndex);
		/*unsafe*/bool GetFilterIndex(int iCycle,csEnumOEMPAFilterIndex *peFilterIndex);
		
		bool SetTimeSlot(int iCycle,double %dTime);
		/*unsafe*/bool GetTimeSlot(int iCycle,double *pdTime);
		bool CheckTimeSlot(double %dTime);
	double GetTimeSlotMinimum(long lAscanPointCount, enumBitSize eAscanBitSize);//Minimum timeSlot for maximum useful data throughput (not LAN throughput, data header is not taken into account).
	double GetAscanThroughput(double dTimeSlot, long lAscanPointCount, enumBitSize eAscanBitSize);//useful data throughput not LAN throughput (data header is not taken into account).
	//FMC SubTimeSlot management
	bool SetFMCSubTimeSlotAcqReplay(int iCycle,double dAscanStart,double dAscanRange,double %dTimeSlot);//easy function to use
	bool GetFMCTimeLimitation(double dAscanStart,double dAscanRange,double dTimeSlot,double %dTimeSlotMin,double %dHWAcqSubTimeSlotMin,double %dReplaySubTimeSlotMin,double %dReplaySubTimeSlotOptimizedForThroughput);
	int GetFMCSubTimeSlotCount();
	//instead to call "SetFMCSubTimeSlotAcq" and "SetFMCSubTimeSlotReplay" you can call "SetFMCSubTimeSlot" which is easier to use.
	bool SetFMCSubTimeSlotAcq(int iCycle,double %dSubTimeSlot);
	bool SetFMCSubTimeSlotReplay(int iCycle,double %dSubTimeSlot);
	/*unsafe*/bool GetFMCSubTimeSlotAcq(int iCycle,double *pdSubTimeSlot);
	/*unsafe*/bool GetFMCSubTimeSlotReplay(int iCycle,double *pdSubTimeSlot);
		
		bool SetAscanAcqIdChannelProbe(int iCycle,WORD %wID);
		/*unsafe*/bool GetAscanAcqIdChannelProbe(int iCycle,WORD *pwID);
		
		bool SetAscanAcqIdChannelScan(int iCycle,WORD %wID);
		/*unsafe*/bool GetAscanAcqIdChannelScan(int iCycle,WORD *pwID);
		
		bool SetAscanAcqIdChannelCycle(int iCycle,WORD %wID);
		/*unsafe*/bool GetAscanAcqIdChannelCycle(int iCycle,WORD *pwID);
		
		bool EnableAscanMaximum(int iCycle,bool %bEnable);
		/*unsafe*/bool GetEnableAscanMaximum(int iCycle,bool *pbEnable);
		
		bool EnableAscanMinimum(int iCycle,bool %bEnable);
		/*unsafe*/bool GetEnableAscanMinimum(int iCycle,bool *pbEnable);
		
		bool EnableAscanSaturation(int iCycle,bool %bEnable);
		/*unsafe*/bool GetEnableAscanSaturation(int iCycle,bool *pbEnable);
		
		bool SetGateModeThreshold(int iCycle,int iGate,bool %bEnable,csEnumGateModeAmp %eGateModeAmp,csEnumGateModeTof %eGateModeTof,csEnumRectification %eGateRectification,double %dThresholdPercent);
		/*unsafe*/bool GetGateModeThreshold(int iCycle,int iGate,bool *pbEnable,csEnumGateModeAmp *peGateModeAmp,csEnumGateModeTof *peGateModeTof,csEnumRectification *peGateRectification,double *pdThresholdPercent);
		bool CheckGateModeThreshold(bool %bEnable,csEnumGateModeAmp %eGateModeAmp,csEnumGateModeTof %eGateModeTof,csEnumRectification %eGateRectification,double %dThresholdPercent);
		
		bool SetGateStart(int iCycle,int iGate,double %dStart);
		/*unsafe*/bool GetGateStart(int iCycle,int iGate,double *pdStart);
		bool CheckGateStart(double %dStart);
		
		bool CheckGateStartStop(double %dStart,double %dStop);
		bool SetGateStop(int iCycle,int iGate,double %dStop);
		/*unsafe*/bool GetGateStop(int iCycle,int iGate,double *pdStop);
		bool CheckGateStop(double %dStop);
		
		bool SetGateAcqIDAmp(int iDriverId,int iCycle,WORD %wID);
		/*unsafe*/bool GetGateAcqIDAmp(int iDriverId,int iCycle,WORD *pwID);
		bool SetGateAcqIDTof(int iDriverId,int iCycle,WORD %wID);
		/*unsafe*/bool GetGateAcqIDTof(int iDriverId,int iCycle,WORD *pwID);
		
		bool SetTrackingGateStart(int iCycle,int iGate,bool bEnable,int %iTrackingCycleIndex,int %iTrackingGateIndex);
		/*unsafe*/bool GetTrackingGateStart(int iCycle,int iGate,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
		bool SetTrackingGateStop(int iCycle,int iGate,bool bEnable,int %iTrackingCycleIndex,int %iTrackingGateIndex);
		/*unsafe*/bool GetTrackingGateStop(int iCycle,int iGate,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
		bool SetTrackingAscan(int iCycle,bool bEnable,int %iTrackingCycleIndex,int %iTrackingGateIndex);
		/*unsafe*/bool GetTrackingAscan(int iCycle,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
		bool SetTrackingDac(int iCycle,bool bEnable,int %iTrackingCycleIndex,int %iTrackingGateIndex);
		/*unsafe*/bool GetTrackingDac(int iCycle,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex);
		bool CheckTracking(bool bEnable,int %iTrackingCycleIndex,int %iTrackingGateIndex);
		bool ResetTrackingTable();//this is called automatically when the IF tracking of one gate is updated.

		bool SetGainAnalog(int iCycle,float %fGain);
		/*unsafe*/bool GetGainAnalog(int iCycle,float *pfGain);
		bool CheckGainAnalog(float %fGain);
		
		//For some of the following functions the CycleCount is required
		//(it should be the total cycle count of the sequence).
		bool SetEmissionWedgeDelay(int iCycle,int iCycleCount,double %dWedgeDelay);//"iCycleCount"=cycle count.
		/*unsafe*/bool GetEmissionWedgeDelay(int iCycle,int iCountMax,double *pdWedgeDelay);
		bool SetReceptionWedgeDelay(int iCycle,int iCycleCount,double %dWedgeDelay);
		/*unsafe*/bool GetReceptionWedgeDelay(int iCycle,int iCountMax,double *pdWedgeDelay);
		bool CheckWedgeDelay(double %dWedgeDelay);
		
		static bool SetAllElementEnable(bool bEnable,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/);
		static bool SetElementEnable(int iElement,bool bEnable,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/);
		static bool GetElementEnable(int iElement,/*const*/ array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/,bool %bEnable);
		
		//APERTURE DEFINITION : 2 cases.
		bool IsMultiplexer();//this function can be used to know if a multiplexer (16:128 or 32:128) is included in the system.
		//Case of a system with a multiplexer (16:128 or 32:128), please use following functions:
		//	multiplexer case: if emission and reception have same aperture:
		bool SetMultiplexerEnable(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/);
		bool GetMultiplexerEnable(int iCycle,/*fixed*/[Out] array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/);
		//	multiplexer case: if emission and reception aperture are different:
		//	(you cannot have more than 32 elements in case of a 32/128 and 16 elements in case of a 16/128).
		bool SetMultiplexerEnable(int iCycle,array<DWORD>^ %adwHWApertureEmission/*[g_iOEMPAApertureDWordCount]*/,array<DWORD>^ %adwHWApertureReception/*[g_iOEMPAApertureDWordCount]*/);
		bool GetMultiplexerEnable(int iCycle,/*fixed*/[Out] array<DWORD>^ %adwHWApertureEmission/*[g_iOEMPAApertureDWordCount]*/,/*fixed*/[Out] array<DWORD>^ %adwHWApertureReception/*[g_iOEMPAApertureDWordCount]*/);
		bool CheckMultiplexerAperture(array<DWORD>^ %adwHWApertureEmission/*[g_iOEMPAApertureDWordCount]*/,array<DWORD>^ % adwHWApertureReception/*[g_iOEMPAApertureDWordCount]*/);//to check that emission and reception apertures are coherent.
		//Case of a system (16:16, 32:32, 64:64 etc...) without a multiplexer, then please use the following functions:
		bool SetEmissionEnable(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/);
		bool GetEmissionEnable(int iCycle,/*fixed*/[Out] array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/);
		bool SetReceptionEnable(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/);
		bool GetReceptionEnable(int iCycle,/*fixed*/[Out] array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/);

		bool SetEmissionDelays(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/,acsFloat^ %afDelay/*,structCallbackArrayFloat1D &callbackArrayFloat1D*/);//Size = element size.
		bool GetEmissionDelays(int iCycle,/*fixed*/[Out] acsFloat^ %afDelay/*,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D*/);//Size = element size.
		bool SetEmissionWidths(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/,acsFloat^ %afWidth/*,structCallbackArrayFloat1D &callbackArrayFloat1D*/);
		bool GetEmissionWidths(int iCycle,/*fixed*/[Out] acsFloat^ %afWidth/*,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D*/);
		bool SetReceptionGains(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/,acsFloat^ %afGain/*,structCallbackArrayFloat1D &callbackArrayFloat1D*/);
		bool GetReceptionGains(int iCycle,/*fixed*/[Out] acsFloat^ %afGain/*,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D*/);
		bool SetReceptionDelays(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/,acsDelayReception^ %afDelay/*,structCallbackArrayFloat2D &callbackArrayFloat2D*/);//Size1 = element size, Size2 = DDF size.
		bool GetReceptionDelays(int iCycle,/*fixed*/[Out] acsDelayReception^ %afDelay/*,int &iElementCountMax,int &iFocalCountMax,structCallbackArrayFloat2D &callbackArrayFloat2D*/);//Size1 = element size, Size2 = DDF size.
		bool SetReceptionFocusing(int iCycle,acsDouble^ %dFocalTof);
		/*unsafe*/bool GetReceptionFocusing(int iCycle,/*fixed*/[Out] acsDouble^ %dFocalTof);
		bool SetReceptionFocusing(int iCycle,acsDouble^ %dFocalTof,float %fCenterDelayE,float %fCenterDelayR);
		/*unsafe*/bool GetReceptionFocusing(int iCycle,/*fixed*/[Out] acsDouble^ %dFocalTof,[Out] float *pfCenterDelayE,[Out] float *pfCenterDelayR);
		bool EnableDDF(int iCycle,bool %bEnable);
		/*unsafe*/bool GetEnableDDF(int iCycle,bool *pbEnable);
		bool EnableDDF(int iCycle,csEnumFocusing %eFocusing);
		/*unsafe*/bool GetEnableDDF(int iCycle,csEnumFocusing *peFocusing);
		static void SetDDFTimeOfFlightMiddle(bool bEnable);
		static bool IsDDFTimeOfFlightMiddle();
		static void SetDDFWaveTrackingCorrection(int iEnable);
		static int GetDDFWaveTrackingCorrection();
		static void SetFMCReceptionSimplified(bool bEnable);
		static bool IsFMCReceptionSimplified();
		bool CheckFocalTimeOfFlight(double %dDelay);
		bool CheckEmissionWidth(float %fWidth);
		bool CheckReceptionGain(float %fGain);
		bool CheckEmissionDelay(float %fDelay);
		bool CheckReceptionDelay(float %fDelay);
		DWORD GetSWBaseAddress();

		bool EnableMultiHWChannelAcquisition(int iCycle,int iCycleCount,bool bEnable);
		/*unsafe*/bool GetEnableMultiHWChannelAcquisition(int iCycle,bool *pbEnable);
		bool SetMultiHWChannelAcqDecimation(int iCycle,acsByte^ %abyData);
		/*unsafe*/ bool GetMultiHWChannelAcqDecimation(int iCycle,[Out] acsByte^ %abyData);
		//A-scan Start is specified for consecutive elements from the first single element #1 (no gap).
		bool SetMultiHWChannelAcqWriteStart(int iCycle,int iAcqElement,int iStartCount,acsFloat^ %afStart/*float *pfStart*/);
		bool GetMultiHWChannelAcqWriteStart(int iCycle,/*fixed*/[Out] acsFloat^ %afStart/*int &iStartCountMax,int *piStartCount,float *pfStart*/);
		double GetMultiHWChannelRangeMax();
		double GetFWAscanRecoveryTime();

		bool SetSettingId(DWORD dwSettingId);
		/*unsafe*/bool GetSettingId(DWORD *pdwSettingId);//ask hw as for others functions.
		DWORD swGetSettingId();//ask sw (boolean memory image resident in computer) instead of hw.
		
		//calibration management
		bool PerformCalibration(float &fDelayMax,float &fCorrectionOffset);//In case of calibration error you can display the reporting file (see "GetCalibrationFileReport").
				//output parameter "fDelayMax" maximum correction delay in second
				//output parameter "fCalibrationOffset" offset time of all elements in second, call "SetTimeOffset" with this value to correct it.
		bool UpdateCalibration();
		bool SetTimeOffset(float &fTime);
		bool GetTimeOffset(float *pfTime);
////in case of Ethernet, KeepAlive is useful to prevent communication deadlock
////but if the developper stay a long time inside a breakpoint it will break the socket.
//bool SetKeepAlive(enumKeepAlive eKeepAlive);
//bool GetKeepAlive(enumKeepAlive *peKeepAlive);//ask hw as for others functions.

//<<PARAMETERS MANAGEMENT FUNCTIONS : END>>

		void test();
		void/*CHWDeviceOEMPA*/ *GetHWDeviceOEMPA();
		static int Test();
	protected:
		!csHWDeviceOEMPA();
	};
#ifdef _WIN64
	public ref class csAcquisitionFifo
    {
	private:
		csHWDeviceOEMPA ^m_csHWDeviceOEMPA;
		csEnumAcquisitionFifo m_csFifo;
		CAcquisitionFifo *m_pAcquisitionFifo;
		bool m_bNewFifo;
	public:
		csAcquisitionFifo(csEnumAcquisitionFifo csFifo,csHWDeviceOEMPA ^csHWDeviceOEMPA);
		csAcquisitionFifo(csEnumAcquisitionFifo csFifo);
		~csAcquisitionFifo();
		void Free();

		csEnumAcquisitionFifo GetFifo();//return the type of the fifo.
		bool IsEnabled();//Fifo has been allocated.
		bool Alloc(int iCycleCountMax, __int64 iBufferByteSize);//to enable the fifo, should be called just after the constructor of "CHWDeviceOEMPA".
			//"iCycleCountMax": maximum cycle count in the fifo (in case of AscanFifo maximum ascan count).
			//"iBufferByteSize": maximum buffer size to store all datas (ascan or cscan or IO).
			//Example:	iCycleCountMax=8    iBufferByteSize=1024+8*4=1056 (ascan header is 8 DWORDs)
			//			you can store 8 ascan of 128 BYTES each, OR 4 ascan of 256 BYTES each.
		bool GetAlloc([Out] int %iDataCountMax, [Out] __int64 %iBufferByteSize);//get allocation sizes.
		bool Desalloc();

		//count of data in the fifo.
		int GetCount();//count of data in the fifo.
		int GetLost();//count of lost data (new input data but the fifo was full so old data has been lost to save new data).
		__int64 GetTotalCount();//total data count that have been inputed of the fifo.
		__int64 GetTotalByteCount();//total byte count that have been inputed of the fifo.
		void ResetCounters();
		bool RemoveAll();//remove all data, can fail in the case the fifo is accessed at the same time than the priority input thread.
		bool RemoveTail();//remove the oldest data in the fifo, can fail in the case the data is accessed at the same time than the priority input thread.
		bool RemoveItem(int iItem);//remove the specified item and previous ones, -1 means the oldest data in the fifo (same than "RemoveTail"). Can fail in the case the data is accessed at the same time than the priority input thread.
		bool DumpFifo(String ^pFileName);//debug purpose.

		//Output data from the fifo. The input parameter is the index of the data in the Fifo. This input parameter "iItem" is retrieved by others functions.
		//"bPeek": false to remove the data from the fifo or true to keep the data inside the fifo.
		//			if "bPeek=true" then the function "RemoveTail" or "RemoveItem" should be called.
		//"iItem": index of the item in the fifo, -1 means the last data.
		/*unsafe*/bool OutAscan(int iItem,bool bPeek,[Out] csAcqInfoEx^ %acqInfo,[Out] csHeaderStream_0x0001^ %streamHeader,[Out] csSubStreamAscan_0x0103^ %ascanHeader,[Out] const void* %pBufferMax,[Out] const void* %pBufferMin,[Out] const void* %pBufferSat);
		bool OutCscan(int iItem,bool bPeek,[Out] csAcqInfoEx^ %acqInfo,[Out] csHeaderStream_0x0001^ %streamHeader,[Out] csSubStreamCscan_0x0X02^ %cscanHeader,[Out] array<csCscanAmp_0x0102^>^ %bufferAmp,[Out] array<csCscanAmpTof_0x0202^>^ %bufferAmpTof);
		bool _OutIO(int iItem,bool bPeek,[Out] csHeaderStream_0x0001^ %streamHeader,[Out] csHeaderIO_0x0001^ %ioHeader);
		bool OutIO(int iItem,bool bPeek,[Out] csAcqInfoEx^ %acqInfo,[Out] csHeaderStream_0x0001^ %streamHeader,[Out] csHeaderIO_0x0001^ %ioHeader);

		//Functions to retrieve index of the data in the fifo for the specified cycle or the specified sequence/cycle.
		//For the following functions: "iFMCElement" and "iStartItem" should be -1 by default.
		//"iFMCElement" is used only for FMC mode, this is the pulser element, -1 means the parameter is not used.
		//"iStartItem" is the last item from which the search is done, -1 means the parameter is not used.
		//"iCycle" if -1 then the first cycle of the sequence is returned.
		//FIFO means FirstInFirstOut.
		//LIFO means LastInFirstOut.
		int GetFifoItem(LONGLONG sequence, int iCycle, int iFMCElement, int iStartItem);//return -1 if not found, otherwise the index.
		int GetFifoItem(int iCycle, int iFMCElement, int iStartItem);//(for the first sequence in the fifo) return -1 if not found, otherwise the index.
		int GetLifoItem(LONGLONG sequence, int iCycle, int iFMCElement, int iStartItem);//return -1 if not found, otherwise the index.
		int GetLifoItem(int iCycle, int iFMCElement, int iStartItem);//(for the last sequence in the fifo) return -1 if not found, otherwise the index.
		//Functions to retrieve index of any data in the fifo.
		int GetItemLimit([Out] int %iIndexTail, [Out] int %iIndexHead);//the return value is the same than "GetFifoCount", output parameter "iIndexTail" is the index of the oldest data in the fifo, output parameter "iIndexHead" is the index of the next input data that will be saved in the fifo.
		void IncrementItemIndex(int %iIndex);//function to increment the index from the Tail (oldest input in the fifo) to the Head (last data in the fifo).
		void DecrementItemIndex(int %iIndex);//function to decrement the index from the Head to the Tail.

		//To input new data in the fifo, this function is called by the driver.
		/*unsafe*/bool InFifo(BYTE *pSubStreamData,int iSubStreamDataSize,csAcqInfoEx^ %acqInfo,csHeaderStream_0x0001^ %streamHeader);
		BYTE *GetSubStreamItem(int iItem,[Out] int %iSubStreamDataSize,[Out] csAcqInfoEx^ %acqInfo,[Out] csHeaderStream_0x0001^ %streamHeader);

		////The integrated thread is launched automatically by the driver, so the user dont have to call the following functions:
		bool IsRunning();//Is the integrated thread running.
		//bool Start(int iType, void* pCallback, LPTHREAD_START_ROUTINE lpStartAddress, PVOID ThreadParameter);
		//bool Stop();
		//bool SetThreadPriority(int nPriority);//input parameter is explained explanation see documentation of "SetThreadPriority".
		//bool CreateEvent(int iDeviceId);
		//bool CreateEvent(wchar_t *pName);
		//HANDLE GetEvent();

		void AddFifoLost(int iLostCount);
		DWORD GetExit();
		void Exit();
	protected:
		!csAcquisitionFifo();
	};
#else _WIN64
	public ref class csAcquisitionFifo
    {
	public:
		csAcquisitionFifo(csEnumAcquisitionFifo csFifo,csHWDeviceOEMPA ^csHWDeviceOEMPA);
		csAcquisitionFifo(csEnumAcquisitionFifo csFifo);

		bool Alloc(int iDataCountMax, __int64 iBufferByteSize){return false;};
	};
#endif _WIN64
#pragma endregion hwDriverOEMPA
//////////////////////////////////////////////////////////////////////////

};

namespace csDriverOEMPA
{
	float csFocalLaw::GetDelay(int iFocalIndex,int iElementIndex)
	{
		if(iFocalIndex<0)
			return false;
		if(iFocalIndex>=afDelay->GetLength(0))
			return false;
		if(iElementIndex<0)
			return false;
		if(iElementIndex>=afDelay->GetLength(1))
			return false;
		//fDelay = afDelay[iFocalIndex*g_iOEMPAApertureElementCountMax+iElementIndex];
		return afDelay[iFocalIndex,iElementIndex];
	}
	bool csFocalLaw::SetDelay(int iFocalIndex,int iElementIndex,float fDelay)
	{
		if(iFocalIndex<0)
			return false;
		if(iFocalIndex>=afDelay->GetLength(0))
			return false;
		if(iElementIndex<0)
			return false;
		if(iElementIndex>=afDelay->GetLength(1))
			return false;
		//afDelay[iFocalIndex*g_iOEMPAApertureElementCountMax+iElementIndex] = fDelay;
		afDelay[iFocalIndex,iElementIndex] = fDelay;
		return true;
	}
	int csFocalLaw::GetHWAcquisitionDecimation(int iChannelIndex)
	{
		if(hwAcqDecimation==nullptr)
			return 0;
		if(iChannelIndex<0)
			return 0;
		if(iChannelIndex>=hwAcqDecimation->GetLength(0))
			return 0;
		if(!(iChannelIndex%2))
			return (hwAcqDecimation[iChannelIndex/2] & 0xf);
		else
			return ((hwAcqDecimation[iChannelIndex/2]>>4) & 0xf);
	}
////////////////////////////////////////////////////////
#pragma region csSWEncoder
	csSWEncoder::csSWEncoder(CSWEncoder *pSWEncoder)
	{
		Free();
		Constructor(pSWEncoder);
	}
	csSWEncoder::~csSWEncoder()
	{
		this->!csSWEncoder();
	}
	csSWEncoder::!csSWEncoder()
	{
		Free();
	}
	void csSWEncoder::Constructor(CSWEncoder *pSWEncoder)
	{
		m_pSWEncoder = pSWEncoder;
	}
	void csSWEncoder::Free()
	{
		//m_pHWDevice = NULL;
		m_pSWEncoder = NULL;
	}

//setting part
	bool csSWEncoder::Enable(bool bEnabled)//value could be negative if encoder need to be inverted.
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->Enable(bEnabled);
	}
	bool csSWEncoder::IsEnabled()
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->IsEnabled();
	}

	bool csSWEncoder::lSetResolution(long lResolution)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->lSetResolution(lResolution);
	}
	long csSWEncoder::lGetResolution()
	{
		if(!m_pSWEncoder)
			return 0;
		return m_pSWEncoder->lGetResolution();
	}
	DWORD csSWEncoder::GetDivider()
	{
		if(!m_pSWEncoder)
			return 0;
		return m_pSWEncoder->GetDivider();
	}
	bool csSWEncoder::dSetResolution(double dResolution)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->dSetResolution(dResolution);
	}
	double csSWEncoder::dGetResolution()
	{
		if(!m_pSWEncoder)
			return 0;
		return m_pSWEncoder->dGetResolution();
	}
	bool csSWEncoder::lSetResolution(long lResolution,DWORD dwDivider)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->lSetResolution(lResolution,dwDivider);
	}

	bool csSWEncoder::SetUnit(csEnumUnit eUnit)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->SetUnit((enumUnit)eUnit);
	}
	csEnumUnit csSWEncoder::GetUnit()
	{
		if(!m_pSWEncoder)
			return (csEnumUnit)0;
		return (csEnumUnit)m_pSWEncoder->GetUnit();
	}

	bool csSWEncoder::SetType(csEnumEncoderType eEncoderType)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->SetType((enumEncoderType)eEncoderType);
	}
	csEnumEncoderType csSWEncoder::GetType()
	{
		if(!m_pSWEncoder)
			return (csEnumEncoderType)0;
		return (csEnumEncoderType)m_pSWEncoder->GetType();
	}

	bool csSWEncoder::SetDigitalInput(csEnumIndexDigitalInput eEncoderInput,csEnumDigitalInput eDigitalInput)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->SetDigitalInput((enumIndexDigitalInput)eEncoderInput,(enumDigitalInput)eDigitalInput);
	}
	csEnumDigitalInput csSWEncoder::GetDigitalInput(csEnumIndexDigitalInput eEncoderInput)
	{
		enumDigitalInput eDigitalInput;

		if(!m_pSWEncoder)
			return (csEnumDigitalInput)0;
		eDigitalInput = m_pSWEncoder->GetDigitalInput((enumIndexDigitalInput)eEncoderInput);
		return (csEnumDigitalInput)eDigitalInput;
	}

//acquisition part
	bool csSWEncoder::SetSpeedDistance(double dValue)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->SetSpeedDistance(dValue);
	}
	bool csSWEncoder::GetSpeedDistance(double %dValue)
	{
		double dValue2;
		bool bRet;
		
		if(!m_pSWEncoder)
			return false;
		bRet = m_pSWEncoder->GetSpeedDistance(dValue2);
		dValue = dValue2;
		return bRet;
	}

	bool csSWEncoder::SetInspectionHWValue(int iValue)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->SetInspectionHWValue(iValue);
	}
	int csSWEncoder::GetInspectionHWValue()
	{
		if(!m_pSWEncoder)
			return 0;
		return m_pSWEncoder->GetInspectionHWValue();
	}
	bool csSWEncoder::SetInspectionSWValue(double dValue)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->SetInspectionSWValue(dValue);
	}
	double csSWEncoder::GetInspectionSWValue()
	{
		if(!m_pSWEncoder)
			return 0.0;
		return m_pSWEncoder->GetInspectionSWValue();
	}

	bool csSWEncoder::SetInspectionSpeed(double dValue)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->SetInspectionSpeed(dValue);
	}
	double csSWEncoder::GetInspectionSpeed()
	{
		if(!m_pSWEncoder)
			return 0.0;
		return m_pSWEncoder->GetInspectionSpeed();
	}

	bool csSWEncoder::SetInspectionLength(double dValue)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->SetInspectionLength(dValue);
	}
	double csSWEncoder::GetInspectionLength()
	{
		if(!m_pSWEncoder)
			return 0.0;
		return m_pSWEncoder->GetInspectionLength();
	}

	bool csSWEncoder::SetInspectionCount(int iValue)
	{
		if(!m_pSWEncoder)
			return false;
		return m_pSWEncoder->SetInspectionCount(iValue);
	}
	int csSWEncoder::GetInspectionCount()
	{
		if(!m_pSWEncoder)
			return 0;
		return m_pSWEncoder->GetInspectionCount();
	}
#pragma endregion csSWEncoder
////////////////////////////////////////////////////////
#pragma region csSWDevice
	csSWDevice::csSWDevice()
	{
		Free();
	}
	csSWDevice::~csSWDevice()
	{
		this->!csSWDevice();
	}
	csSWDevice::!csSWDevice()
	{
		Free();
	}
	csSWEncoder ^csSWDevice::GetSWEncoder(int iEncoderIndex)
	{
		if(iEncoderIndex<0)
			return nullptr;
		if(iEncoderIndex>=g_iDriverEncoderCountMax)
			return nullptr;
		if(!iEncoderIndex)
			return m_csSWEncoder1;
		return m_csSWEncoder2;
	}
	void csSWDevice::Constructor(CHWDeviceOEMPA *pHWDeviceOEMPA,CHWDevice *pHWDevice)
	{
		CSWEncoder *pSWEncoder;

		pSWEncoder = pHWDeviceOEMPA->GetSWEncoder(0);
		m_csSWEncoder1 = gcnew csSWEncoder(pSWEncoder);
		pSWEncoder = pHWDeviceOEMPA->GetSWEncoder(1);
		m_csSWEncoder2 = gcnew csSWEncoder(pSWEncoder);
		m_pHWDeviceOEMPA = pHWDeviceOEMPA;
		m_pHWDevice = pHWDevice;
		m_pSWDevice = m_pHWDevice->GetSWDevice();
	}
	void csSWDevice::Free()
	{
		m_csSWEncoder1 = nullptr;
		m_csSWEncoder2 = nullptr;
		m_pHWDeviceOEMPA = NULL;
		m_pHWDevice = NULL;
		m_pSWDevice = NULL;
	}

	bool csSWDevice::SetBoardName(String ^pName)//to register this new data in the kernel database.
	{
		wchar_t* y;
		bool bRet;

		if(!m_pSWDevice)
			return false;
		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pName);
		bRet = m_pSWDevice->SetBoardName(y);
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}
	bool csSWDevice::GetBoardName([Out] String^ %pName)
	{
		wchar_t pAux[MAX_PATH];

		if(!m_pSWDevice)
			return false;
		if(!m_pSWDevice->GetBoardName(MAX_PATH,pAux))
			return false;
		pName = Marshal::PtrToStringUni((IntPtr)pAux);
		return true;
	}

	bool csSWDevice::GetConfigurationFilePath(bool bFlashName,String ^pPathName)//to find the configuration file path.
	{
		wchar_t pAux[MAX_PATH];

		if(!m_pSWDevice)
			return false;
		if(!m_pSWDevice->GetConfigurationFilePath(bFlashName,pAux,MAX_PATH))
			return false;
		pPathName = Marshal::PtrToStringUni((IntPtr)pAux);
		return true;
	}

	bool csSWDevice::SetSetupFileDefault(bool bOEMPA,String^ wcFile)
	{
		wchar_t* y;
		bool bRet;

		if(!m_pSWDevice)
			return false;
		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(wcFile);
		bRet = m_pSWDevice->SetSetupFileDefault(bOEMPA,y);
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}
	bool csSWDevice::GetSetupFileDefault(bool bOEMPA,[Out] String^ %wcFile)
	{
		wchar_t pAux[MAX_PATH];

		if(!m_pSWDevice)
			return false;
		if(!m_pSWDevice->GetSetupFileDefault(bOEMPA,MAX_PATH,pAux))
			return false;
		wcFile = Marshal::PtrToStringUni((IntPtr)pAux);
		return true;
	}
	bool csSWDevice::SetSetupFileCurrent(String^ wcFile)
	{
		wchar_t* y;
		bool bRet;

		if(!m_pSWDevice)
			return false;
		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(wcFile);
		bRet = m_pSWDevice->SetSetupFileCurrent(y);
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}
	bool csSWDevice::GetSetupFileCurrent([Out] String^ %wcFile)
	{
		wchar_t pAux[MAX_PATH];

		if(!m_pSWDevice)
			return false;
		if(!m_pSWDevice->GetSetupFileCurrent(MAX_PATH,pAux))
			return false;
		wcFile = Marshal::PtrToStringUni((IntPtr)pAux);
		return true;
	}

	bool csSWDevice::SetConnectionState(csEnumConnectionState eConnection,bool bDisplayErrorMsg)
	{
		if(!m_pSWDevice)
			return 0.0;
		return m_pSWDevice->SetConnectionState((enumConnectionState)eConnection,bDisplayErrorMsg);
	}
	csEnumConnectionState csSWDevice::GetConnectionState()
	{
		if(!m_pSWDevice)
			return csEnumConnectionState::csDisconnected;
		return (csEnumConnectionState)m_pSWDevice->GetConnectionState();
	}
	bool csSWDevice::IsConnected()
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->IsConnected();
	}

	bool csSWDevice::CheckProcessId()//only one process have ownership of the driver.
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->CheckProcessId();
	}

	bool csSWDevice::SetCfgStatus(csEnumUpdateStatus eUpdateStatus)
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->SetCfgStatus((enumUpdateStatus)eUpdateStatus);
	}
	csEnumUpdateStatus csSWDevice::GetCfgStatus()
	{
		if(!m_pSWDevice)
			return csEnumUpdateStatus::csOutOfDate;
		return (csEnumUpdateStatus)m_pSWDevice->GetCfgStatus();
	}

	int csSWDevice::SetStreamCount(int iCount)
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->SetStreamCount(iCount);
	}
	int csSWDevice::GetStreamCount()
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->GetStreamCount();
	}
	int csSWDevice::SetStreamRetransmit(int iCount)
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->SetStreamRetransmit(iCount);
	}
	int csSWDevice::GetStreamRetransmit()
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->GetStreamRetransmit();
	}
	int csSWDevice::SetStreamError(int iCount)
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->SetStreamError(iCount);
	}
	int csSWDevice::GetStreamError()
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->GetStreamError();
	}

	int csSWDevice::GetLostCountAscan()
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->GetLostCountAscan();
	}
	int csSWDevice::SetLostCountAscan(int iDataLostCount)
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->SetLostCountAscan(iDataLostCount);
	}
	int csSWDevice::GetLostCountCscan()
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->GetLostCountCscan();
	}
	int csSWDevice::SetLostCountCscan(int iDataLostCount)
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->SetLostCountCscan(iDataLostCount);
	}
	int csSWDevice::GetLostCountEncoder()
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->GetLostCountEncoder();
	}
	int csSWDevice::SetLostCountEncoder(int iDataLostCount)
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->SetLostCountEncoder(iDataLostCount);
	}
	int csSWDevice::GetLostCountUSB3()
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->GetLostCountUSB3();
	}
	int csSWDevice::SetLostCountUSB3(int iDataLostCount)
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->SetLostCountUSB3(iDataLostCount);
	}
	int csSWDevice::GetErrorCountUSB3()
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->GetErrorCountUSB3();
	}
	int csSWDevice::GetLostCountFifo(csEnumAcquisitionFifo csFifo)
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->GetLostCountFifo((enumAcquisitionFifo)csFifo);
	}
	void csSWDevice::ResetCounters()
	{
		if(!m_pSWDevice)
			return;
		m_pSWDevice->ResetCounters();
	}

	//bool SetEncoderCount(int iCount)//0 1 or 2.
	int csSWDevice::GetEncoderEnabledCount()
	{
		if(!m_pSWDevice)
			return 0;
		return m_pSWDevice->GetEncoderEnabledCount();
	}
	void csSWDevice::swProcessEncoder(structAcqInfoEx &acqInfo)
	{
		if(!m_pSWDevice)
			return;
		m_pSWDevice->swProcessEncoder(acqInfo);
	}

	bool csSWDevice::EnablePulser(bool bEnable)
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->EnablePulser(bEnable);
	}
	bool csSWDevice::IsPulserEnabled()
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->IsPulserEnabled();
	}

	bool csSWDevice::SetLockDefaultDisablePulser(bool bDisable)
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->SetLockDefaultDisablePulser(bDisable);
	}
	bool csSWDevice::GetLockDefaultDisablePulser()
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->GetLockDefaultDisablePulser();
	}
	bool csSWDevice::SetUnlockDefaultEnablePulser(bool bEnable)
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->SetUnlockDefaultEnablePulser(bEnable);
	}
	bool csSWDevice::GetUnlockDefaultEnablePulser()
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->GetUnlockDefaultEnablePulser();
	}

	bool csSWDevice::SetAcqSpeedAscan(double dSpeed)
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->SetAcqSpeedAscan(dSpeed);
	}
	double csSWDevice::GetAcqSpeedAscan()
	{
		if(!m_pSWDevice)
			return 0.0;
		return m_pSWDevice->GetAcqSpeedAscan();
	}
	bool csSWDevice::SetAcqSpeedCscan(double dSpeed)
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->SetAcqSpeedCscan(dSpeed);
	}
	double csSWDevice::GetAcqSpeedCscan()
	{
		if(!m_pSWDevice)
			return 0.0;
		return m_pSWDevice->GetAcqSpeedCscan();
	}
	bool csSWDevice::SetAcqSpeedIO(double dSpeed)
	{
		if(!m_pSWDevice)
			return false;
		return m_pSWDevice->SetAcqSpeedIO(dSpeed);
	}
	double csSWDevice::GetAcqSpeedIO()
	{
		if(!m_pSWDevice)
			return 0.0;
		return m_pSWDevice->GetAcqSpeedIO();
	}
#pragma endregion csSWDevice
////////////////////////////////////////////////////////
#pragma region csHWDevice
#ifdef _WIN64
	csAcquisitionFifo::csAcquisitionFifo(csEnumAcquisitionFifo csFifo,csHWDeviceOEMPA ^csHWDeviceOEMPA)
	{
		CHWDeviceOEMPA* pHWDeviceOEMPA;
		enumAcquisitionFifo eFifo=(enumAcquisitionFifo)csFifo;

		m_csHWDeviceOEMPA = csHWDeviceOEMPA;
		m_csFifo = csFifo;
		m_bNewFifo = false;
		pHWDeviceOEMPA = (CHWDeviceOEMPA*)m_csHWDeviceOEMPA->GetHWDeviceOEMPA();
		if(pHWDeviceOEMPA)
			m_pAcquisitionFifo = (CAcquisitionFifo*)pHWDeviceOEMPA->GetAcquisitionFifo(eFifo);
		else
			m_pAcquisitionFifo = NULL;
	}
	csAcquisitionFifo::csAcquisitionFifo(csEnumAcquisitionFifo csFifo)
	{
		m_csHWDeviceOEMPA = nullptr;
		m_csFifo = csFifo;
		m_bNewFifo = true;
		m_pAcquisitionFifo = new CAcquisitionFifo((enumAcquisitionFifo)csFifo,NULL);
	}
	csAcquisitionFifo::~csAcquisitionFifo()
	{
		this->!csAcquisitionFifo();
	}
	csAcquisitionFifo::!csAcquisitionFifo()
	{
		Free();
	}
	void csAcquisitionFifo::Free()
	{
		if(m_bNewFifo)
			delete m_pAcquisitionFifo;
		m_pAcquisitionFifo = NULL;
	}

	csEnumAcquisitionFifo csAcquisitionFifo::GetFifo()
	{
		return m_csFifo;
	}
	bool csAcquisitionFifo::IsEnabled()
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->IsEnabled();
	}
	bool csAcquisitionFifo::IsRunning()
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->IsRunning();
	}
	bool csAcquisitionFifo::Alloc(int iDataCountMax, __int64 iBufferByteSize)
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->Alloc(iDataCountMax, iBufferByteSize);
	}
	bool csAcquisitionFifo::GetAlloc([Out] int %iDataCountMax, [Out] __int64 %iBufferByteSize)
	{
		bool bRet;
		int _iDataCountMax;
		__int64 _iBufferByteSize;

		if(!m_pAcquisitionFifo)
			return false;
		bRet = m_pAcquisitionFifo->GetAlloc(_iDataCountMax,_iBufferByteSize);
		iDataCountMax = _iDataCountMax;
		iBufferByteSize = _iBufferByteSize;
		return bRet;
	}
	bool csAcquisitionFifo::Desalloc()
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->Desalloc();
	}

	//count of data in the fifo.
	int csAcquisitionFifo::GetCount()
	{
		if(!m_pAcquisitionFifo)
			return 0;
		return m_pAcquisitionFifo->GetCount();
	}
	int csAcquisitionFifo::GetLost()
	{
		if(!m_pAcquisitionFifo)
			return 0;
		return m_pAcquisitionFifo->GetLost();
	}
	__int64 csAcquisitionFifo::GetTotalCount()
	{
		if(!m_pAcquisitionFifo)
			return 0;
		return m_pAcquisitionFifo->GetTotalCount();
	}
	__int64 csAcquisitionFifo::GetTotalByteCount()
	{
		if(!m_pAcquisitionFifo)
			return 0;
		return m_pAcquisitionFifo->GetTotalByteCount();
	}
	void csAcquisitionFifo::ResetCounters()
	{
		if(!m_pAcquisitionFifo)
			return;
		m_pAcquisitionFifo->ResetCounters();
	}
	bool csAcquisitionFifo::RemoveAll()
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->RemoveAll();
	}
	bool csAcquisitionFifo::RemoveTail()
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->RemoveTail();
	}
	bool csAcquisitionFifo::RemoveItem(int iItem)
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->RemoveItem(iItem);
	}
	bool csAcquisitionFifo::DumpFifo(String ^pFileName)
	{
		wchar_t* y;
		bool bRet;

		if(!m_pAcquisitionFifo)
			return false;
		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pFileName);
		bRet = m_pAcquisitionFifo->DumpFifo(y);
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}

	/*unsafe*/bool csAcquisitionFifo::OutAscan(int iItem,bool bPeek,[Out] csAcqInfoEx^ %acqInfo,[Out] csHeaderStream_0x0001^ %streamHeader,[Out] csSubStreamAscan_0x0103^ %ascanHeader,[Out] const void* %pBufferMax,[Out] const void* %pBufferMin,[Out] const void* %pBufferSat)
	{
		structAcqInfoEx* _acqInfo;
		CStream_0x0001* _streamHeader;
		CSubStreamAscan_0x0103* _ascanHeader;
		void *_pBufferMax,*_pBufferMin,*_pBufferSat;
		constVoid p;

		if(!m_pAcquisitionFifo)
			return false;
		if(!m_pAcquisitionFifo->OutAscan(iItem,bPeek,_acqInfo,_streamHeader,_ascanHeader,_pBufferMax,_pBufferMin,_pBufferSat))
			return false;
		p.pcVoid = _acqInfo;
		acqInfo = safe_cast<csAcqInfoEx^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csAcqInfoEx::typeid));
		p.pcVoid = _streamHeader;
		streamHeader = safe_cast<csHeaderStream_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderStream_0x0001::typeid));
		p.pcVoid = _ascanHeader;
		ascanHeader = safe_cast<csSubStreamAscan_0x0103^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csSubStreamAscan_0x0103::typeid));
		pBufferMax = _pBufferMax;
		pBufferMin = _pBufferMin;
		pBufferSat = _pBufferSat;
		return true;
	}
	bool csAcquisitionFifo::OutCscan(int iItem,bool bPeek,[Out] csAcqInfoEx^ %acqInfo,[Out] csHeaderStream_0x0001^ %streamHeader,[Out] csSubStreamCscan_0x0X02^ %cscanHeader,[Out] array<csCscanAmp_0x0102^>^ %bufferAmp,[Out] array<csCscanAmpTof_0x0202^>^ %bufferAmpTof)
	{
		structAcqInfoEx* _acqInfo;
		CStream_0x0001* _streamHeader;
		const CSubStreamCscan_0x0X02* _cscanHeader;
		const structCscanAmp_0x0102* _pBufferAmp;
		const structCscanAmpTof_0x0202* _pBufferAmpTof;
		structCscanAmp_0x0102 *pAmp;
		structCscanAmpTof_0x0202 *pAmpTof;
		constVoid p;

		if(!m_pAcquisitionFifo)
			return false;
		if(!m_pAcquisitionFifo->OutCscan(iItem,bPeek,_acqInfo,_streamHeader,_cscanHeader,_pBufferAmp,_pBufferAmpTof))
			return false;
		p.pcVoid = _acqInfo;
		acqInfo = safe_cast<csAcqInfoEx^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csAcqInfoEx::typeid));
		p.pcVoid = _streamHeader;
		streamHeader = safe_cast<csHeaderStream_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderStream_0x0001::typeid));
		p.pcVoid = _cscanHeader;
		cscanHeader = safe_cast<csSubStreamCscan_0x0X02^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csSubStreamCscan_0x0X02::typeid));
		if(_pBufferAmp)
		{
			p.pcVoid = _pBufferAmp;
			pAmp = (structCscanAmp_0x0102*)p.pVoid;
			bufferAmp = gcnew array<csCscanAmp_0x0102^>(_cscanHeader->count);
			for(int i=0;i<(int)_cscanHeader->count;i++)
				bufferAmp[i] = safe_cast<csCscanAmp_0x0102^>(Marshal::PtrToStructure((IntPtr)&pAmp[i],csCscanAmp_0x0102::typeid));
		}
		if(_pBufferAmpTof)
		{
			p.pcVoid = _pBufferAmpTof;
			pAmpTof = (structCscanAmpTof_0x0202*)p.pVoid;
			bufferAmpTof = gcnew array<csCscanAmpTof_0x0202^>(_cscanHeader->count);
			for(int i=0;i<(int)_cscanHeader->count;i++)
				bufferAmpTof[i] = safe_cast<csCscanAmpTof_0x0202^>(Marshal::PtrToStructure((IntPtr)&pAmpTof[i],csCscanAmpTof_0x0202::typeid));
		}
		return true;
	}
	bool csAcquisitionFifo::_OutIO(int iItem,bool bPeek,[Out] csHeaderStream_0x0001^ %streamHeader,[Out] csHeaderIO_0x0001^ %ioHeader)
	{
		CStream_0x0001* _streamHeader;
		const CSubStreamIO_0x0101* _ioHeader;
		constVoid p;

		if(!m_pAcquisitionFifo)
			return false;
		if(!m_pAcquisitionFifo->OutIO(iItem,bPeek,_streamHeader,_ioHeader))
			return false;
		p.pcVoid = _streamHeader;
		streamHeader = safe_cast<csHeaderStream_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderStream_0x0001::typeid));
		p.pcVoid = _ioHeader;
		ioHeader = safe_cast<csHeaderIO_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderIO_0x0001::typeid));
		return true;
	}
	bool csAcquisitionFifo::OutIO(int iItem,bool bPeek,[Out] csAcqInfoEx^ %acqInfo,[Out] csHeaderStream_0x0001^ %streamHeader,[Out] csHeaderIO_0x0001^ %ioHeader)
	{
		structAcqInfoEx* _acqInfo;
		CStream_0x0001* _streamHeader;
		const CSubStreamIO_0x0101* _ioHeader;
		constVoid p;

		if(!m_pAcquisitionFifo)
			return false;
		if(!m_pAcquisitionFifo->OutIO(iItem,bPeek,_acqInfo,_streamHeader,_ioHeader))
			return false;
		p.pcVoid = _acqInfo;
		acqInfo = safe_cast<csAcqInfoEx^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csAcqInfoEx::typeid));
		p.pcVoid = _streamHeader;
		streamHeader = safe_cast<csHeaderStream_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderStream_0x0001::typeid));
		p.pcVoid = _ioHeader;
		ioHeader = safe_cast<csHeaderIO_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderIO_0x0001::typeid));
		return true;
	}

	int csAcquisitionFifo::GetFifoItem(LONGLONG sequence, int iCycle, int iFMCElement, int iStartItem)
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->GetFifoItem(sequence, iCycle, iFMCElement, iStartItem);
	}
	int csAcquisitionFifo::GetFifoItem(int iCycle, int iFMCElement, int iStartItem)
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->GetFifoItem(iCycle, iFMCElement, iStartItem);
	}
	int csAcquisitionFifo::GetLifoItem(LONGLONG sequence, int iCycle, int iFMCElement, int iStartItem)
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->GetLifoItem(sequence, iCycle, iFMCElement, iStartItem);
	}
	int csAcquisitionFifo::GetLifoItem(int iCycle, int iFMCElement, int iStartItem)
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->GetLifoItem(iCycle, iFMCElement, iStartItem);
	}
	int csAcquisitionFifo::GetItemLimit([Out] int %iIndexTail, [Out] int %iIndexHead)
	{
		int _iIndexTail, _iIndexHead;
		int iRet;

		if(!m_pAcquisitionFifo)
			return false;
		iRet = m_pAcquisitionFifo->GetItemLimit(_iIndexTail, _iIndexHead);
		iIndexTail = _iIndexTail;
		iIndexHead = _iIndexHead;
		return iRet;
	}
	void csAcquisitionFifo::IncrementItemIndex(int %iIndex)
	{
		int _iIndex;

		if(!m_pAcquisitionFifo)
			return;
		_iIndex = iIndex;
		m_pAcquisitionFifo->IncrementItemIndex(_iIndex);
		iIndex = _iIndex;
	}
	void csAcquisitionFifo::DecrementItemIndex(int %iIndex)
	{
		int _iIndex;

		if(!m_pAcquisitionFifo)
			return;
		_iIndex = iIndex;
		m_pAcquisitionFifo->DecrementItemIndex(_iIndex);
		iIndex = _iIndex;
	}

	//To input new data in the fifo, this function is called by the driver.
	/*unsafe*/bool csAcquisitionFifo::InFifo(BYTE *pSubStreamData,int iSubStreamDataSize,csAcqInfoEx^ %acqInfo,csHeaderStream_0x0001^ %streamHeader)
	{
		structAcqInfoEx* _acqInfo;
		CStream_0x0001* _streamHeader;
		CSubStreamAscan_0x0103* _ascanHeader;
		bool bRet;
		int struct_size_acq_info = Marshal::SizeOf(acqInfo);
		int struct_size_stream = Marshal::SizeOf(streamHeader);
		System::IntPtr ptr_acq_info = Marshal::AllocHGlobal(struct_size_acq_info);
		System::IntPtr ptr_stream = Marshal::AllocHGlobal(struct_size_stream);

		if(!m_pAcquisitionFifo)
			return false;
		Marshal::StructureToPtr(acqInfo, ptr_acq_info, true);
		Marshal::StructureToPtr(streamHeader, ptr_stream, true);
		_acqInfo = (structAcqInfoEx*)ptr_acq_info.ToPointer();
		_streamHeader = (CStream_0x0001*)ptr_stream.ToPointer();
		_ascanHeader = (CSubStreamAscan_0x0103*)pSubStreamData;
		bRet = m_pAcquisitionFifo->InFifo(pSubStreamData,iSubStreamDataSize,*_acqInfo,_streamHeader);
		Marshal::FreeHGlobal(ptr_acq_info);
		Marshal::FreeHGlobal(ptr_stream);
		ptr_acq_info = IntPtr::Zero;
		ptr_stream = IntPtr::Zero;
		return bRet;
	}
	BYTE *csAcquisitionFifo::GetSubStreamItem(int iItem,[Out] int %iSubStreamDataSize,[Out] csAcqInfoEx^ %acqInfo,[Out] csHeaderStream_0x0001^ %streamHeader)
	{
		BYTE *pSubStream;
		structAcqInfoEx* _acqInfo;
		CStream_0x0001* _streamHeader;
		constVoid p;
		int _iSubStreamDataSize;

		if(!m_pAcquisitionFifo)
			return NULL;
		pSubStream = m_pAcquisitionFifo->GetSubStreamItem(iItem,_iSubStreamDataSize,_acqInfo,_streamHeader);
		p.pcVoid = _acqInfo;
		acqInfo = safe_cast<csAcqInfoEx^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csAcqInfoEx::typeid));
		p.pcVoid = _streamHeader;
		streamHeader = safe_cast<csHeaderStream_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderStream_0x0001::typeid));
		return pSubStream;
	}

	////The integrated thread is launched automatically by the driver, so the user dont have to call the following functions:
	//bool Start(int iType, void* pCallback, LPTHREAD_START_ROUTINE lpStartAddress, PVOID ThreadParameter);
	//bool Stop();
	//bool SetThreadPriority(int nPriority);//input parameter is explained explanation see documentation of "SetThreadPriority".
	//bool CreateEvent(int iDeviceId);
	//bool CreateEvent(wchar_t *pName);
	//HANDLE GetEvent();

	void csAcquisitionFifo::AddFifoLost(int iLostCount)
	{
		if(!m_pAcquisitionFifo)
			return;
		return m_pAcquisitionFifo->AddFifoLost(iLostCount);
	}
	DWORD csAcquisitionFifo::GetExit()
	{
		if(!m_pAcquisitionFifo)
			return false;
		return m_pAcquisitionFifo->GetExit();
	}
	void csAcquisitionFifo::Exit()
	{
		if(!m_pAcquisitionFifo)
			return;
		return m_pAcquisitionFifo->Exit();
	}
#else _WIN64
	csAcquisitionFifo::csAcquisitionFifo(csEnumAcquisitionFifo csFifo,csHWDeviceOEMPA ^csHWDeviceOEMPA)
	{
	}
	csAcquisitionFifo::csAcquisitionFifo(csEnumAcquisitionFifo csFifo)
	{
	}
#endif _WIN64

	csHWDevice::csHWDevice()
	{
		m_csSWDevice = nullptr;
		m_pHWDevice = NULL;
		m_pSWDevice = NULL;
		m_csAcquisitionParameter = nullptr;
		m_csAscan = nullptr;
		m_csCscan = nullptr;
		m_csIo0 = nullptr;
		m_csIo1 = nullptr;
		m_csInfo = nullptr;
		m_bDigitalEdgesOnly = false;
		m_pointer = NULL;
		g_CallbackHWMemory = nullptr;
	}
	csHWDevice::~csHWDevice()
	{
		this->!csHWDevice();
	}
	csHWDevice::!csHWDevice()
	{
		Free();
	}
	void csHWDevice::Constructor(CHWDeviceOEMPA *pHWDeviceOEMPA,CHWDevice *pHWDevice,csCustomizedAPI ^_csCustomizedAPI)
	{
		m_csCustomizedAPI = _csCustomizedAPI;
		m_csSWDevice = gcnew csSWDevice();
		m_List = gcnew csPinList();

		if(m_csSWDevice!=nullptr)
			m_csSWDevice->Constructor(pHWDeviceOEMPA,pHWDevice);
		m_pHWDevice = pHWDevice;
		m_pSWDevice = m_pHWDevice->GetSWDevice();
		m_pointer = new gcroot<csHWDevice^>(this);
	}
	void csHWDevice::Free()
	{
		m_csSWDevice = nullptr;
		m_List = nullptr;
		m_pHWDevice = NULL;
		m_pSWDevice = NULL;
		if(m_pointer)
			delete m_pointer;
		m_pointer = NULL;
	}
	csAcquisitionFifo ^csHWDevice::GetAcquisitionFifo(csEnumAcquisitionFifo csFifo)
	{
		return nullptr;
	}
	csSWDevice ^csHWDevice::GetSWDevice()
	{
		return m_csSWDevice;
	}
	int csHWDevice::GetDeviceId()
	{
		if(!m_pHWDevice)
			return -1;
		return m_pHWDevice->GetDeviceId();
	}
	int csHWDevice::GetMonitorPort(int iDeviceId)
	{
		return CHWDevice::GetMonitorPort(iDeviceId);
	}
	csEnumHardwareLink csHWDevice::GetHardwareLink()
	{
		if(!m_pHWDevice)
			return csEnumHardwareLink::csUnlinked;
		return (csEnumHardwareLink)m_pHWDevice->GetHardwareLink();
	}
	int csHWDevice::GetMasterDeviceId()
	{
		if(!m_pHWDevice)
			return -1;
		return m_pHWDevice->GetMasterDeviceId();
	}
	bool csHWDevice::SetDefaultHwLink(csEnumHardwareLink csHardwareLink,[Out] bool %bPreviousMasterUnregistered)
	{
		bool bRet,bPreviousMasterUnregistered2;

		bPreviousMasterUnregistered = false;
		if(!m_pHWDevice)
			return false;
		bRet = m_pHWDevice->SetDefaultHwLink((enumHardwareLink)csHardwareLink,bPreviousMasterUnregistered2);
		bPreviousMasterUnregistered = bPreviousMasterUnregistered2;
		return bRet;
	}
	csEnumHardwareLink csHWDevice::GetDefaultHwLink()
	{
		if(!m_pHWDevice)
			return csEnumHardwareLink::csUnlinked;
		return (csEnumHardwareLink)m_pHWDevice->GetDefaultHwLink();
	}
	bool csHWDevice::IsDefaultHwLinkEnabled()
	{
		if(!m_pHWDevice)
			return false;
		return m_pHWDevice->IsDefaultHwLinkEnabled();
	}
	bool csHWDevice::SlaveConnect(int iMasterDeviceId)
	{
		if(!m_pHWDevice)
			return false;
		return m_pHWDevice->SlaveConnect(iMasterDeviceId);
	}
	bool csHWDevice::SlaveDisconnect()
	{
		if(!m_pHWDevice)
			return false;
		return m_pHWDevice->SlaveDisconnect();
	}

	bool csHWDevice::SetAcquisitionParameter(Object ^pParameter)//first parameter of the callback acquisition function.
	{
		m_csAcquisitionParameter = pParameter;
		if(m_csCustomizedAPI!=nullptr)
			m_csCustomizedAPI->SetAcquisitionParameter(pParameter);
		if(!m_pHWDevice)
			return false;
		return true;
	}
	Object ^csHWDevice::GetAcquisitionParameter()
	{
		return m_csAcquisitionParameter;
	}
	bool csHWDevice::SetAcquisitionAscan_0x00010103(TypeAcquisitionAscan_0x00010103 ^pProcess)
	{
		//IntPtr ip;

		m_csAscan = pProcess;
		if(!m_pHWDevice)
			return false;
		//ip = Marshal::GetFunctionPointerForDelegate(pProcess);
		//pAcquisitionAscan_0x00010103 = ip.ToPointer();
		m_pHWDevice->SetAcquisitionParameter(m_pointer);
		return m_pHWDevice->SetAcquisitionAscan_0x00010103(gAcquisitionAscan_0x00010103);
	}
	TypeAcquisitionAscan_0x00010103 ^csHWDevice::GetAcquisitionAscan_0x00010103()
	{
		return m_csAscan;
	}
	bool csHWDevice::SetAcquisitionCscan_0x00010X02(TypeAcquisitionCscan_0x00010X02 ^pProcess)
	{
		m_csCscan = pProcess;
		if(!m_pHWDevice)
			return false;
		m_pHWDevice->SetAcquisitionParameter(m_pointer);
		return m_pHWDevice->SetAcquisitionCscan_0x00010X02(gAcquisitionCscan_0x00010X02);
	}
	TypeAcquisitionCscan_0x00010X02 ^csHWDevice::GetAcquisitionCscan_0x00010X02()
	{
		return m_csCscan;
	}
	bool csHWDevice::SetAcquisitionIO_0x00010101(TypeAcquisitionIO_0x00010101 ^pProcess,bool bDigitalEdgesOnly)
	{
		m_bDigitalEdgesOnly = bDigitalEdgesOnly;
		m_csIo0 = pProcess;
		if(!m_pHWDevice)
			return false;
		m_pHWDevice->SetAcquisitionParameter(m_pointer);
		return m_pHWDevice->SetAcquisitionIO_0x00010101(gAcquisitionIO_0x00010101,bDigitalEdgesOnly);
	}
	TypeAcquisitionIO_0x00010101 ^csHWDevice::GetAcquisitionIO_0x00010101([Out] bool^ %bDigitalEdgesOnly)
	{
		bDigitalEdgesOnly = m_bDigitalEdgesOnly;
		return m_csIo0;
	}
	bool csHWDevice::SetAcquisitionIO_1x00010101(TypeAcquisitionIO_1x00010101 ^pProcess,bool bDigitalEdgesOnly)
	{
		m_bDigitalEdgesOnly = bDigitalEdgesOnly;
		m_csIo1 = pProcess;
		if(!m_pHWDevice)
			return false;
		m_pHWDevice->SetAcquisitionParameter(m_pointer);
		return m_pHWDevice->SetAcquisitionIO_1x00010101(gAcquisitionIO_1x00010101,bDigitalEdgesOnly);
	}
	TypeAcquisitionIO_1x00010101 ^csHWDevice::GetAcquisitionIO_1x00010101([Out] bool^ %bDigitalEdgesOnly)
	{
		bDigitalEdgesOnly = m_bDigitalEdgesOnly;
		return m_csIo1;
	}
	bool csHWDevice::SetAcquisitionInfo(TypeAcquisitionInfo ^pProcess)
	{
		m_csInfo = pProcess;
		if(!m_pHWDevice)
			return false;
		return m_pHWDevice->SetAcquisitionInfo(gAcquisitionInfo);
	}
	TypeAcquisitionInfo ^csHWDevice::GetAcquisitionInfo()
	{
		return m_csInfo;
	}

	bool csHWDevice::IsDriverEncoderManagementEnabled()
	{
		if(!m_pHWDevice)
			return false;
		return m_pHWDevice->IsDriverEncoderManagementEnabled();
	}
	void csHWDevice::EnableDriverEncoderManagement(bool bEnable)
	{
		if(!m_pHWDevice)
			return;
		m_pHWDevice->EnableDriverEncoderManagement(bEnable);
	}

	bool csHWDevice::LockDevice()
	{
		if(!m_pHWDevice)
			return false;
		m_List->Free();
		return m_pHWDevice->LockDevice();
	}
	bool csHWDevice::LockDevice(csEnumAcquisitionState eAcqState)
	{
		if(!m_pHWDevice)
			return false;
		m_List->Free();
		return m_pHWDevice->LockDevice((enumAcquisitionState)eAcqState);
	}
	bool csHWDevice::UnlockDevice()
	{
		bool bRet;

		if(!m_pHWDevice)
			return false;
		bRet = m_pHWDevice->UnlockDevice();
		m_List->Free();
		return bRet;
	}
	bool csHWDevice::UnlockDevice(csEnumAcquisitionState eAcqState)
	{
		bool bRet;

		if(!m_pHWDevice)
			return false;
		bRet = m_pHWDevice->UnlockDevice((enumAcquisitionState)eAcqState);
		m_List->Free();
		return bRet;
	}
	bool csHWDevice::IsDeviceLocked()
	{
		if(!m_pHWDevice)
			return false;
		return m_pHWDevice->IsDeviceLocked();
	}
	DWORD csHWDevice::GetSettingId()
	{
		if(!m_pHWDevice)
			return 0;
		return m_pHWDevice->GetSettingId();
	}

	bool csHWDevice::Flush()
	{
		if(!m_pHWDevice)
			return false;
		return m_pHWDevice->Flush();
	}
	bool csHWDevice::WriteHW(DWORD dwAddress,DWORD dwData,int iSize)
	{
		if(!m_pHWDevice)
			return false;
		return m_pHWDevice->WriteHW(dwAddress,dwData,iSize);
	}
	/*unsafe*/bool csHWDevice::ReadHW(DWORD dwAddress,[Out] DWORD *pdwData,int iSize)
	{
		if(!m_pHWDevice)
			return false;
		return m_pHWDevice->ReadHW(dwAddress,pdwData,iSize);
	}
	bool csHWDevice::SetCallbackHWMemory(TypeCallbackHWMemory ^callbackHWMemory)
	{
		if(m_pHWDevice->GetAcquisitionParameter() && (m_pHWDevice->GetAcquisitionParameter()!=m_pointer))
			return false;
		if(CHWDevice::GetCallbackHWMemory() && (CHWDevice::GetCallbackHWMemory()!=gCallbackHWMemory))
			return false;
		if(!CHWDevice::SetCallbackHWMemory(gCallbackHWMemory))
			return false;
		g_CallbackHWMemory = callbackHWMemory;
		return true;
	}
	TypeCallbackHWMemory ^csHWDevice::GetCallbackHWMemory()
	{
		if(CHWDevice::GetCallbackHWMemory()==gCallbackHWMemory)
			return g_CallbackHWMemory;
		return nullptr;
	}
	void csHWDevice::CallbackHWMemory(bool bMaster, DWORD addr, DWORD data, int size)
	{
		if(g_CallbackHWMemory==nullptr)
			return;
		g_CallbackHWMemory->Invoke(this, bMaster, addr, data, (unsigned long)size);
	}
	int csHWDevice::AcquisitionAscan_0x00010103(structAcqInfoEx &acqInfo_,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
	{
		csAcqInfoEx^ acqInfo;
		csHeaderStream_0x0001^ headerStream;
		csSubStreamAscan_0x0103^ headerAscan;
		constVoid p;

		if(m_csAscan==nullptr)
			return 1;
		p.pcVoid = &acqInfo_;
		acqInfo = safe_cast<csAcqInfoEx^>(Marshal::PtrToStructure((IntPtr)&acqInfo_,csAcqInfoEx::typeid));
		p.pcVoid = pStreamHeader;
		headerStream = safe_cast<csHeaderStream_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderStream_0x0001::typeid));
		p.pcVoid = pAscanHeader;
		headerAscan = safe_cast<csSubStreamAscan_0x0103^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csSubStreamAscan_0x0103::typeid));
		return m_csAscan->Invoke(m_csAcquisitionParameter,acqInfo,headerStream,headerAscan,pBufferMax,pBufferMin,pBufferSat);
	}
	int csHWDevice::AcquisitionCscan_0x00010X02(structAcqInfoEx &acqInfo_,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof)
	{
		csAcqInfoEx^ acqInfo;
		csHeaderStream_0x0001^ headerStream;
		csSubStreamCscan_0x0X02^ cscanHeader;
		array<csCscanAmp_0x0102^>^ bufferAmp=nullptr;
		array<csCscanAmpTof_0x0202^>^ bufferAmpTof=nullptr;
		constVoid p;
		structCscanAmp_0x0102 *pAmp;
		structCscanAmpTof_0x0202 *pAmpTof;

		if(m_csCscan==nullptr)
			return 1;
		p.pcVoid = &acqInfo_;
		acqInfo = safe_cast<csAcqInfoEx^>(Marshal::PtrToStructure((IntPtr)&acqInfo_,csAcqInfoEx::typeid));
		p.pcVoid = pStreamHeader;
		headerStream = safe_cast<csHeaderStream_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderStream_0x0001::typeid));
		p.pcVoid = pCscanHeader;
		cscanHeader = safe_cast<csSubStreamCscan_0x0X02^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csSubStreamCscan_0x0X02::typeid));
		if(p.pcVoid=pBufferAmp)//and not ==
		{
			p.pcVoid = pBufferAmp;
			pAmp = (structCscanAmp_0x0102*)p.pVoid;
			bufferAmp = gcnew array<csCscanAmp_0x0102^>(pCscanHeader->count);
			for(int i=0;i<(int)pCscanHeader->count;i++)
				bufferAmp[i] = safe_cast<csCscanAmp_0x0102^>(Marshal::PtrToStructure((IntPtr)&pAmp[i],csCscanAmp_0x0102::typeid));
		}
		if(p.pcVoid=pBufferAmpTof)//and not ==
		{
			p.pcVoid = pBufferAmpTof;
			pAmpTof = (structCscanAmpTof_0x0202*)p.pVoid;
			bufferAmpTof = gcnew array<csCscanAmpTof_0x0202^>(pCscanHeader->count);
			for(int i=0;i<(int)pCscanHeader->count;i++)
				bufferAmpTof[i] = safe_cast<csCscanAmpTof_0x0202^>(Marshal::PtrToStructure((IntPtr)&pAmpTof[i],csCscanAmpTof_0x0202::typeid));
		}
		return m_csCscan->Invoke(m_csAcquisitionParameter,acqInfo,headerStream,cscanHeader,bufferAmp,bufferAmpTof);
	}
	int csHWDevice::AcquisitionIO_0x00010101(const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader)
	{
		csHeaderStream_0x0001^ headerStream;
		csHeaderIO_0x0001^ ioHeader;
		constVoid p;

		if(m_csIo0==nullptr)
			return 1;
		p.pcVoid = pStreamHeader;
		headerStream = safe_cast<csHeaderStream_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderStream_0x0001::typeid));
		p.pcVoid = pIOHeader;
		ioHeader = safe_cast<csHeaderIO_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderIO_0x0001::typeid));
		return m_csIo0->Invoke(m_csAcquisitionParameter,headerStream,ioHeader);
	}
	int csHWDevice::AcquisitionIO_1x00010101(structAcqInfoEx &acqInfo_,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader)
	{
		csAcqInfoEx^ acqInfo;
		csHeaderStream_0x0001^ headerStream;
		csHeaderIO_0x0001^ ioHeader;
		constVoid p;

		if(m_csIo1==nullptr)
			return 1;
		p.pcVoid = &acqInfo_;
		acqInfo = safe_cast<csAcqInfoEx^>(Marshal::PtrToStructure((IntPtr)&acqInfo_,csAcqInfoEx::typeid));
		p.pcVoid = pStreamHeader;
		headerStream = safe_cast<csHeaderStream_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderStream_0x0001::typeid));
		p.pcVoid = pIOHeader;
		ioHeader = safe_cast<csHeaderIO_0x0001^>(Marshal::PtrToStructure((IntPtr)p.pVoid,csHeaderIO_0x0001::typeid));
		return m_csIo1->Invoke(m_csAcquisitionParameter,acqInfo,headerStream,ioHeader);
	}
	int csHWDevice::AcquisitionInfo(const wchar_t *pInfo)
	{
		String^ pValue;
		constVoid p;

		if(m_csInfo==nullptr)
			return 1;
		p.pcVoid = pInfo;
		pValue = Marshal::PtrToStringUni((IntPtr)p.pVoid);
		return m_csInfo->Invoke(m_csAcquisitionParameter,pValue);
	}
	void csHWDevice::CallbackCustomizedAPI(const wchar_t *pFileName,enumStepCustomizedAPI eStepCustomizedAPI,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
	{
		if(m_csCustomizedAPI==nullptr)
			return;
		m_csCustomizedAPI->CallbackCustomizedAPI(pFileName,eStepCustomizedAPI,pRoot,pCycle,pEmission,pReception);
	}
	void* csHWDevice::ListAddObject(Object^ object)
	{
		if(m_List==nullptr)
			return NULL;
		return m_List->Add(object);
	}
	void csHWDevice::test()
	{
		structAcqInfoEx acqInfo;
		CStream_0x0001 StreamHeader;
		CSubStreamAscan_0x0103 AscanHeader;
		CSubStreamCscan_0x0X02 cscanHeader;
		structCscanAmp_0x0102 bufferAmp[4];
		structCscanAmpTof_0x0202 bufferAmpTof[4];
		CSubStreamIO_0x0101 ioHeader;

		init(acqInfo);
		init(StreamHeader);
		init(AscanHeader);
		init(cscanHeader);
		cscanHeader.count = 4;
		for(int i=0;i<4;i++)
		{
			init(bufferAmp[i]);
			init(bufferAmpTof[i]);
		}
		init(ioHeader);
		gAcquisitionAscan_0x00010103(m_pointer,acqInfo,&StreamHeader,&AscanHeader,NULL,NULL,NULL);
		gAcquisitionInfo(m_pointer,L"Hello world!");
		gAcquisitionIO_0x00010101(m_pointer,&StreamHeader,&ioHeader);
		gAcquisitionCscan_0x00010X02(m_pointer,acqInfo,&StreamHeader,&cscanHeader,bufferAmp,bufferAmpTof);
	}
#pragma endregion csHWDevice
////////////////////////////////////////////////////////
	void csKernelDriver::GetVersion([Out] String^ %pMsg)
	{
		wchar_t pAux[MAX_PATH/4];
		wcscpy_s(pAux,MAX_PATH/4,OEMPA_GetVersion());
		pMsg = Marshal::PtrToStringUni((IntPtr)pAux);
	}
	char csKernelDriver::GetVersionLetter()
	{
		return KIT_VERSION_LETTER;
	}
	bool csKernelDriver::CrtCheckMemory()
	{
#ifdef _DEBUG
		if(::_CrtCheckMemory())
			return true;
		else
			return false;
#else _DEBUG
		return true;
#endif _DEBUG
	}
	bool csKernelDriver::CrtSetDbgFlag(bool bEnable)
	{
#ifdef _DEBUG
		if (bEnable)
			::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF & _CRTDBG_CHECK_ALWAYS_DF);
		else
			::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
		return true;
#else _DEBUG
		return false;
#endif _DEBUG
	}
	//void csKernelDriver::debug_EnableHeapEx(bool bEnable, String ^pFileName)
	//{
	//	wchar_t* y;
	//	char cData[MAX_PATH];
	//	DWORD dwConvert;

	//	y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pFileName);
	//	dwConvert = (DWORD)WideCharToMultiByte(CP_ACP, 0, y, -1, cData, MAX_PATH, NULL, NULL);
	//	Marshal::FreeHGlobal((IntPtr)y);
	//	if(dwConvert)
	//		::debug_EnableHeapEx(bEnable, cData);
	//}
	//bool csKernelDriver::debug_DumpHeap(String ^pFileName,bool bStatistics)
	//{
	//	wchar_t* y;
	//	char cData[MAX_PATH];
	//	DWORD dwConvert;

	//	y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pFileName);
	//	dwConvert = (DWORD)WideCharToMultiByte(CP_ACP, 0, y, -1, cData, MAX_PATH, NULL, NULL);
	//	Marshal::FreeHGlobal((IntPtr)y);
	//	if(!dwConvert)
	//		return false;
	//	return ::debug_DumpHeap(cData, bStatistics);
	//}
	static csMsgBox::csMsgBox()
	{
		g_CallbackSystemMessageBox = nullptr;
		g_CallbackSystemMessageBoxList = nullptr;
		g_CallbackSystemMessageBoxButtons = nullptr;
		g_CallbackOempaApiMessageBox = nullptr;
	}

	void csMsgBox::SetCallbackSystemMessageBox(TypeCallbackSystemMessageBox ^pProcess)
	{
		g_CallbackSystemMessageBox = pProcess;
		::SetCallbackSystemMessageBox(gCallbackSystemMessageBox);
	}
	void csMsgBox::SetCallbackSystemMessageBoxList(TypeCallbackSystemMessageBoxList ^pProcess)
	{
		g_CallbackSystemMessageBoxList = pProcess;
		::SetCallbackSystemMessageBoxList(gCallbackSystemMessageBoxList);
	}
	void csMsgBox::SetCallbackSystemMessageBoxButtons(TypeCallbackSystemMessageBoxButtons ^pProcess)
	{
		g_CallbackSystemMessageBoxButtons = pProcess;
		::SetCallbackSystemMessageBoxButtons(gCallbackSystemMessageBoxButtons);
	}
	void csMsgBox::SetCallbackOempaApiMessageBox(TypeCallbackOempaApiMessageBox ^pProcess)
	{
		g_CallbackOempaApiMessageBox = pProcess;
		::SetCallbackMessageBox(gCallbackOempaApiMessageBox);
	}

	TypeCallbackSystemMessageBox ^csMsgBox::GetCallbackSystemMessageBox()
	{
		return g_CallbackSystemMessageBox;
	}
	TypeCallbackSystemMessageBoxList ^csMsgBox::GetCallbackSystemMessageBoxList()
	{
		return g_CallbackSystemMessageBoxList;
	}
	TypeCallbackSystemMessageBoxButtons ^csMsgBox::GetCallbackSystemMessageBoxButtons()
	{
		return g_CallbackSystemMessageBoxButtons;
	}
	TypeCallbackOempaApiMessageBox ^csMsgBox::GetCallbackOempaApiMessageBox()
	{
		return g_CallbackOempaApiMessageBox;
	}

	void csMsgBox::CallbackSystemMessageBox(const wchar_t *pMsg)
	{
		String^ pValue;
		constVoid p;

		if(g_CallbackSystemMessageBox==nullptr)
			return;
		p.pcVoid = pMsg;
		pValue = Marshal::PtrToStringUni((IntPtr)p.pVoid);
		g_CallbackSystemMessageBox->Invoke(pValue);
	}
	void csMsgBox::CallbackSystemMessageBoxList(const wchar_t *pMsg)
	{
		String^ pValue;
		constVoid p;

		if(g_CallbackSystemMessageBoxList==nullptr)
			return;
		p.pcVoid = pMsg;
		pValue = Marshal::PtrToStringUni((IntPtr)p.pVoid);
		g_CallbackSystemMessageBoxList->Invoke(pValue);
	}
	unsigned int csMsgBox::CallbackSystemMessageBoxButtons(const wchar_t *pMsg,const wchar_t *pTitle,UINT nType)
	{
		String^ pValue1;
		String^ pValue2;
		constVoid p1,p2;

		if(g_CallbackSystemMessageBoxButtons==nullptr)
			return 0;
		p1.pcVoid = pMsg;
		pValue1 = Marshal::PtrToStringUni((IntPtr)p1.pVoid);
		p2.pcVoid = pTitle;
		pValue2 = Marshal::PtrToStringUni((IntPtr)p2.pVoid);
		return (unsigned int)g_CallbackSystemMessageBoxButtons->Invoke(pValue1,pValue2,(csEnumMsgBoxButtons)nType);
	}
	int csMsgBox::CallbackOempaApiMessageBox(HWND hWnd,const wchar_t* lpszText,const wchar_t* lpszCaption,UINT nType)
	{
		String^ pValue1;
		String^ pValue2;
		constVoid p1,p2;

		if(g_CallbackOempaApiMessageBox==nullptr)
			return 0;
		p1.pcVoid = lpszText;
		pValue1 = Marshal::PtrToStringUni((IntPtr)p1.pVoid);
		p2.pcVoid = lpszCaption;
		pValue2 = Marshal::PtrToStringUni((IntPtr)p2.pVoid);
		return (int)g_CallbackOempaApiMessageBox->Invoke(/*hWnd,*/pValue1,pValue2,(csEnumMsgBoxButtons)nType);
	}

	void csMsgBox::SystemMessageBox(String ^pMsg)
	{
		wchar_t* y;

		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pMsg);
		UTKernel_SystemMessageBox(y);
		Marshal::FreeHGlobal((IntPtr)y);
	}
	void csMsgBox::SystemMessageBoxList(String ^pMsg)
	{
		wchar_t* y;

		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pMsg);
		UTKernel_SystemMessageBoxList(y);
		Marshal::FreeHGlobal((IntPtr)y);
	}
	csEnumMsgBoxReturn csMsgBox::SystemMessageBoxButtons(String ^pMsg,String ^pTitle,csEnumMsgBoxButtons nType)
	{
		wchar_t *y,*z;
		UINT ret;

		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pMsg);
		z = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pTitle);
		ret = UTKernel_SystemMessageBox(y,z,(UINT)nType);
		Marshal::FreeHGlobal((IntPtr)y);
		Marshal::FreeHGlobal((IntPtr)z);
		return (csEnumMsgBoxReturn)ret;
	}
	csEnumMsgBoxReturn csMsgBox::OempaApiMessageBox(String ^lpszText,String ^lpszCaption,csEnumMsgBoxButtons nType)
	{
		wchar_t *y,*z;
		int ret;

		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(lpszText);
		z = (wchar_t*)(void*)Marshal::StringToHGlobalUni(lpszCaption);
		ret = ::OempaApiMessageBox(NULL,y,z,(UINT)nType);
		Marshal::FreeHGlobal((IntPtr)y);
		Marshal::FreeHGlobal((IntPtr)z);
		return (csEnumMsgBoxReturn)ret;
	}
	bool csMsgBox::IsUserInterfaceThread()
	{
		return UTDriver_IsUserInterfaceThread();//this function could be called to know if the current thread is attached to the management of window.
	}
////////////////////////////////////////////////////////
#pragma region csCustomizedAPI
	csCustomizedAPI::csCustomizedAPI(CHWDeviceOEMPA *pHWDeviceOEMPA)
	{
		m_pHWDeviceOEMPA = pHWDeviceOEMPA;
		m_csCallback = nullptr;
		m_csAcquisitionParameter = nullptr;
		m_pRoot = NULL;
		m_pCycle = NULL;
		m_pEmission = NULL;
		m_pReception = NULL;
		m_iCycleCount = -2;
	}
	csCustomizedAPI::~csCustomizedAPI()
	{
		this->!csCustomizedAPI();
	}
	csCustomizedAPI::!csCustomizedAPI()
	{
		Free();
	}
	void csCustomizedAPI::Free()
	{
		m_pHWDeviceOEMPA = NULL;
	}

	bool csCustomizedAPI::SetAcquisitionParameter(Object ^pParameter)//first parameter of the callback acquisition function.
	{
		m_csAcquisitionParameter = pParameter;
		if(!m_pHWDeviceOEMPA)
			return false;
		return true;
	}
	Object ^csCustomizedAPI::GetAcquisitionParameter()
	{
		return m_csAcquisitionParameter;
	}
	bool csCustomizedAPI::SetCallbackCustomizedDriverAPI(TypeCallbackCustomizedDriverAPI ^pProcess)
	{
		m_csCallback = pProcess;
		if(!m_pHWDeviceOEMPA)
			return false;
		return OEMPA_SetCallbackCustomizedDriverAPI(gCallbackCustomizedAPI);
	}
	TypeCallbackCustomizedDriverAPI ^csCustomizedAPI::GetCallbackCustomizedDriverAPI()
	{
		return m_csCallback;
	}
	void csCustomizedAPI::CallbackCustomizedAPI(const wchar_t *pFileName,enumStepCustomizedAPI eStepCustomizedAPI,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
	{
		String^ pValue;
		constVoid p;

		if(m_csCallback==nullptr)
			return;
		p.pcVoid = pFileName;
		pValue = Marshal::PtrToStringUni((IntPtr)p.pVoid);
		m_iCycleCount = pRoot->iCycleCount;
		m_pRoot = pRoot;
		m_pCycle = pCycle;
		m_pEmission = pEmission;
		m_pReception = pReception;
		m_csCallback->Invoke(m_csAcquisitionParameter,(csEnumStepCustomizedAPI)eStepCustomizedAPI,pValue,m_iCycleCount);
		m_pRoot = NULL;
		m_pCycle = NULL;
		m_pEmission = NULL;
		m_pReception = NULL;
		m_iCycleCount = -2;
	}

	bool csRoot::CopyFrom(structRoot *pRoot)
	{
		if(!pRoot)
			return false;
		//root = safe_cast<csRoot^>(Marshal::PtrToStructure((IntPtr)pRoot,csRoot::typeid));
		this->dwVersionLSB = pRoot->dwVersionLSB;
		this->dwVersionMSB = pRoot->dwVersionMSB;
		this->dwRootSize = pRoot->dwRootSize;
		this->ullSavedParameters = pRoot->ullSavedParameters;
		this->uiSavedFilterCount = pRoot->uiSavedFilterCount;
		this->csDefaultHwLink = (csEnumHardwareLink)pRoot->eDefaultHwLink;
		this->bEnableFMC = pRoot->bEnableFMC;
		this->bEnableMultiHWChannel = pRoot->bEnableMultiHWChannel;
		this->bAscanEnable = pRoot->bAscanEnable;
		this->bEnableCscanTof = pRoot->bEnableCscanTof;
		this->csEnableTFM = (csEnumTFMParameters)pRoot->eEnableTFM;
		this->csAscanBitSize = (csEnumBitSize)(pRoot->eAscanBitSize);
		this->csAscanRequest = (csEnumAscanRequest)pRoot->eAscanRequest;
		this->dAscanRequestFrequency = pRoot->dAscanRequestFrequency;
		this->csTriggerMode = (csEnumOEMPATrigger)pRoot->eTriggerMode;
		this->csEncoderDirection = (csEnumOEMPAEncoderDirection)pRoot->eEncoderDirection;
		this->cTemperatureAlarm = pRoot->cTemperatureAlarm;
		this->cTemperatureWarning = pRoot->cTemperatureWarning;
		this->dTriggerEncoderStep = pRoot->dTriggerEncoderStep;
		this->csRequestIO = (csEnumOEMPARequestIO)pRoot->eRequestIO;
		this->iRequestIODigitalInputMaskRising = pRoot->iRequestIODigitalInputMaskRising;
		this->iRequestIODigitalInputMaskFalling = pRoot->iRequestIODigitalInputMaskFalling;
		this->dDebouncerEncoder = pRoot->dDebouncerEncoder;
		this->dDebouncerDigital = pRoot->dDebouncerDigital;
		this->csDigitalOuput = gcnew array<csEnumOEMPAMappingDigitalOutput>(6);
		for(int i=0;i<6;i++)
			this->csDigitalOuput[i] = (csEnumOEMPAMappingDigitalOutput)pRoot->eDigitalOuput[i];
		this->lSWEncoderResolution1 = pRoot->lSWEncoderResolution1;
		this->lSWEncoderResolution2 = pRoot->lSWEncoderResolution2;
		this->dwSWEncoderDivider1 = pRoot->dwSWEncoderDivider1;
		this->dwSWEncoderDivider2 = pRoot->dwSWEncoderDivider2;
		this->csEncoder1A = (csEnumDigitalInput)pRoot->eEncoder1A;
		this->csEncoder1B = (csEnumDigitalInput)pRoot->eEncoder1B;
		this->csEncoder2A = (csEnumDigitalInput)pRoot->eEncoder2A;
		this->csEncoder2B = (csEnumDigitalInput)pRoot->eEncoder2B;
		this->csExternalTriggerCycle = (csEnumDigitalInput)pRoot->eExternalTriggerCycle;
		this->csExternalTriggerSequence = (csEnumDigitalInput)pRoot->eExternalTriggerSequence;
		this->csEncoder1Type = (csEnumEncoderType)pRoot->eEncoder1Type;
		this->csEncoder2Type = (csEnumEncoderType)pRoot->eEncoder2Type;
		this->csKeepAlive = (csEnumKeepAlive)pRoot->eKeepAlive;

		this->iCycleCount = pRoot->iCycleCount;
		this->aFilter = gcnew array<csFilter^>(g_iEnumOEMPAFilterIndexLast);
		for(int i=0;i<g_iEnumOEMPAFilterIndexLast;i++)
		{
			this->aFilter[i] = gcnew csFilter;
			this->aFilter[i]->aCoefficient = gcnew array<short>(g_iOEMPAFilterCoefficientMax);
			for(int j=0;j<g_iOEMPAFilterCoefficientMax;j++)
				this->aFilter[i]->aCoefficient[j] = pRoot->aFilter[i].aCoefficient[j];
			this->aFilter[i]->wScale = pRoot->aFilter[i].wScale;
			//this->aFilter[i]->pTitle = gcnew String(pRoot->aFilter[i].pTitle);
			if(wcslen(pRoot->aFilter[i].pTitle)<g_iFilterTitleLength)
				this->aFilter[i]->pTitle = Marshal::PtrToStringUni((IntPtr)pRoot->aFilter[i].pTitle);
			else
				this->aFilter[i]->pTitle = gcnew String(L"");
		}
		this->iFMCElementStart = pRoot->iFMCElementStart;
		this->iFMCElementStop = pRoot->iFMCElementStop;
		this->iFMCElementStep = pRoot->iFMCElementStep;

		this->dSpecimenVelocity = pRoot->dSpecimenVelocity;
		this->dDigitizingPeriod = pRoot->dDigitizingPeriod;
		this->dSpecimenRadius = pRoot->dSpecimenRadius;
		this->dSpecimenThickness = pRoot->dSpecimenThickness;
		this->iOEMPAProbeCountMax = pRoot->iOEMPAProbeCountMax;
		this->iOEMPAApertureCountMax = pRoot->iOEMPAApertureCountMax;
		this->iOEMPADDFCountMax = pRoot->iOEMPADDFCountMax;
		this->bUSB3Disable = pRoot->bUSB3Disable;
		this->dMultiHWChannelRangeMax = pRoot->dMultiHWChannelRangeMax;
		this->dFWAscanRecoveryTime = pRoot->dFWAscanRecoveryTime;
		this->dTriggerHighTime = pRoot->dTriggerHighTime;

		this->iSubSequenceEncoderCount = pRoot->iSubSequenceEncoderCount;
		this->iSubSequenceGateCount = pRoot->iSubSequenceGateCount;
		this->aiSubSequenceCycleStart = gcnew array<int>(g_iSubSequenceTableSize);
		this->aiSubSequenceCycleStop = gcnew array<int>(g_iSubSequenceTableSize);
		this->afSubSequenceValue = gcnew array<float>(g_iSubSequenceTableSize);
		for(int iIndex=0;iIndex<g_iSubSequenceTableSize;iIndex++)
		{
			this->aiSubSequenceCycleStart[iIndex] = pRoot->aiSubSequenceCycleStart[iIndex];
			this->aiSubSequenceCycleStop[iIndex] = pRoot->aiSubSequenceCycleStop[iIndex];
			this->afSubSequenceValue[iIndex] = pRoot->afSubSequenceValue[iIndex];
		}
		this->iSubSequenceAverage = pRoot->iSubSequenceAverage;

		this->eReferenceTimeOfFlight = (csEnumReferenceTimeOfFlight)pRoot->eReferenceTimeOfFlight;

		this->b256Master = pRoot->b256Master;
		this->b256Slave = pRoot->b256Slave;

		this->pHWDeviceOEMPA = pRoot->pHWDeviceOEMPA;
		this->wcFileName = gcnew array<wchar_t>(MAX_PATH);
		for(int i=0;i<MAX_PATH;i++)
			this->wcFileName[i] = pRoot->wcFileName[i];
		if(wcslen(pRoot->wcFileName)>=MAX_PATH)
			this->wcFileName[MAX_PATH-1] = 0;
		return true;
	}
	bool csRoot::CopyTo(structRoot *pRoot)
	{
		wchar_t* y;

		if(!pRoot)
			return false;
		pRoot->dwVersionLSB = this->dwVersionLSB;
		pRoot->dwVersionMSB = this->dwVersionMSB;
		pRoot->dwRootSize = this->dwRootSize;
		pRoot->ullSavedParameters  = this->ullSavedParameters;
		pRoot->uiSavedFilterCount = this->uiSavedFilterCount;
		pRoot->eDefaultHwLink = (enumHardwareLink)this->csDefaultHwLink;
		pRoot->bEnableFMC = this->bEnableFMC;
		pRoot->bEnableMultiHWChannel = this->bEnableMultiHWChannel;
		pRoot->bAscanEnable = this->bAscanEnable;
		pRoot->bEnableCscanTof = this->bEnableCscanTof;
		pRoot->eEnableTFM = (enumTFMParameters)this->csEnableTFM;
		pRoot->eAscanBitSize = (enumBitSize)(this->csAscanBitSize);
		pRoot->eAscanRequest = (enumAscanRequest)this->csAscanRequest;
		pRoot->dAscanRequestFrequency = this->dAscanRequestFrequency;
		pRoot->eTriggerMode = (enumOEMPATrigger)this->csTriggerMode;
		pRoot->eEncoderDirection = (enumOEMPAEncoderDirection)this->csEncoderDirection;
		pRoot->cTemperatureAlarm = this->cTemperatureAlarm;
		pRoot->cTemperatureWarning = this->cTemperatureWarning;
		pRoot->dTriggerEncoderStep = this->dTriggerEncoderStep;
		pRoot->eRequestIO = (enumOEMPARequestIO)this->csRequestIO;
		pRoot->iRequestIODigitalInputMaskRising = this->iRequestIODigitalInputMaskRising;
		pRoot->iRequestIODigitalInputMaskFalling = this->iRequestIODigitalInputMaskFalling;
		pRoot->dDebouncerEncoder = this->dDebouncerEncoder;
		pRoot->dDebouncerDigital = this->dDebouncerDigital;
		if(this->csDigitalOuput!=nullptr)
		for(int i=0;i<6;i++)
			pRoot->eDigitalOuput[i] = (enumOEMPAMappingDigitalOutput)this->csDigitalOuput[i];
		pRoot->lSWEncoderResolution1 = this->lSWEncoderResolution1;
		pRoot->lSWEncoderResolution2 = this->lSWEncoderResolution2;
		pRoot->dwSWEncoderDivider1 = this->dwSWEncoderDivider1;
		pRoot->dwSWEncoderDivider2 = this->dwSWEncoderDivider2;
		pRoot->eEncoder1A = (enumDigitalInput)this->csEncoder1A;
		pRoot->eEncoder1B = (enumDigitalInput)this->csEncoder1B;
		pRoot->eEncoder2A = (enumDigitalInput)this->csEncoder2A;
		pRoot->eEncoder2B = (enumDigitalInput)this->csEncoder2B;
		pRoot->eExternalTriggerCycle = (enumDigitalInput)this->csExternalTriggerCycle;
		pRoot->eExternalTriggerSequence = (enumDigitalInput)this->csExternalTriggerSequence;
		pRoot->eEncoder1Type = (enumEncoderType)this->csEncoder1Type;
		pRoot->eEncoder2Type = (enumEncoderType)this->csEncoder2Type;
		pRoot->eKeepAlive = (enumKeepAlive)this->csKeepAlive;

		pRoot->iCycleCount = this->iCycleCount;
		if(this->aFilter!=nullptr)
		for(int i=0;i<g_iEnumOEMPAFilterIndexLast;i++)
		{
			if(this->aFilter[i]==nullptr)
				continue;
			if(this->aFilter[i]->aCoefficient!=nullptr)
			for(int j=0;j<g_iOEMPAFilterCoefficientMax;j++)
				pRoot->aFilter[i].aCoefficient[j]  = this->aFilter[i]->aCoefficient[j];
			pRoot->aFilter[i].wScale = this->aFilter[i]->wScale;
			if(this->aFilter[i]->pTitle==nullptr)
				continue;
			y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(this->aFilter[i]->pTitle);
			wcscpy_s(pRoot->aFilter[i].pTitle,MAX_PATH,y);
			Marshal::FreeHGlobal((IntPtr)y);
		}
		pRoot->iFMCElementStart = this->iFMCElementStart;
		pRoot->iFMCElementStop = this->iFMCElementStop;
		pRoot->iFMCElementStep = this->iFMCElementStep;

		pRoot->dSpecimenVelocity  = this->dSpecimenVelocity;
		pRoot->dSpecimenRadius = this->dSpecimenRadius;
		pRoot->dSpecimenThickness = this->dSpecimenThickness;
		pRoot->dDigitizingPeriod  = this->dDigitizingPeriod;
		pRoot->iOEMPAProbeCountMax = this->iOEMPAProbeCountMax;
		pRoot->iOEMPAApertureCountMax = this->iOEMPAApertureCountMax;
		pRoot->iOEMPADDFCountMax = this->iOEMPADDFCountMax;
		pRoot->bUSB3Disable = this->bUSB3Disable;
		pRoot->dMultiHWChannelRangeMax = this->dMultiHWChannelRangeMax;
		pRoot->dFWAscanRecoveryTime = this->dFWAscanRecoveryTime;
		pRoot->dTriggerHighTime = this->dTriggerHighTime;

		pRoot->iSubSequenceEncoderCount = this->iSubSequenceEncoderCount;
		pRoot->iSubSequenceGateCount = this->iSubSequenceGateCount;
		for(int iIndex=0;iIndex<g_iSubSequenceTableSize;iIndex++)
		{
			if(this->aiSubSequenceCycleStart!=nullptr)
				pRoot->aiSubSequenceCycleStart[iIndex] = this->aiSubSequenceCycleStart[iIndex];
			if(this->aiSubSequenceCycleStop!=nullptr)
				pRoot->aiSubSequenceCycleStop[iIndex] = this->aiSubSequenceCycleStop[iIndex];
			if(this->afSubSequenceValue!=nullptr)
				pRoot->afSubSequenceValue[iIndex] = this->afSubSequenceValue[iIndex];
		}
		pRoot->iSubSequenceAverage = this->iSubSequenceAverage;

		pRoot->eReferenceTimeOfFlight = (enumReferenceTimeOfFlight)this->eReferenceTimeOfFlight;

		pRoot->b256Master = this->b256Master;
		pRoot->b256Slave = this->b256Slave;

		pRoot->pHWDeviceOEMPA  = (CHWDeviceOEMPA*)this->pHWDeviceOEMPA;
		if(this->wcFileName!=nullptr)
		{
			//y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(this->wcDebugFile1);
			//wcscpy_s(pRoot->wcDebugFile1,MAX_PATH,y);
			//Marshal::FreeHGlobal((IntPtr)y);
			for (int i=0;i<MAX_PATH;i++)
				pRoot->wcFileName[i] = this->wcFileName[i];
		}
		return true;
	}
	bool csCycle::CopyFrom(structCycle *pCycle)
	{
		if(!pCycle)
			return false;
		//1.1.3.2c if gate ON following code crash
		//cycle = safe_cast<csCycle^>(Marshal::PtrToStructure((IntPtr)&m_pCycle[iCycle],csCycle::typeid));
		this->dGainDigital = pCycle->dGainDigital;
		this->fBeamCorrection = pCycle->fBeamCorrection;
		this->dStart = pCycle->dStart;
		this->dRange = pCycle->dRange;
		this->dTimeSlot = pCycle->dTimeSlot;
		this->dFMCSubTimeSlotAcq = pCycle->dFMCSubTimeSlotAcq;
		this->dFMCSubTimeSlotReplay = pCycle->dFMCSubTimeSlotReplay;
		this->lPointCount = pCycle->lPointCount;
		this->lPointFactor = pCycle->lPointFactor;
		this->eCompressionType = (csEnumCompressionType)pCycle->eCompressionType;
		this->eRectification = (csEnumRectification)pCycle->eRectification;
		this->bDACAutoStop = pCycle->bDACAutoStop;
		this->iDACCount = pCycle->iDACCount;
		if(this->iDACCount>0)
		{
			this->adDACTof = gcnew array<double>(g_iOEMPADACCountMax);
			this->afDACPrm = gcnew array<float>(g_iOEMPADACCountMax);
			if(this->adDACTof!=nullptr)
			{
				for(int iIndex=0;iIndex<g_iOEMPADACCountMax;iIndex++)
					this->adDACTof[iIndex] = pCycle->adDACTof[iIndex];
			}
			if(this->afDACPrm!=nullptr)
			{
				for(int iIndex=0;iIndex<g_iOEMPADACCountMax;iIndex++)
					this->afDACPrm[iIndex] = pCycle->afDACPrm[iIndex];
			}
		}
		this->bDACSlope = pCycle->bDACSlope;
		this->bDACEnable = pCycle->bDACEnable;
		this->bMaximum = pCycle->bMaximum;
		this->bMinimum = pCycle->bMinimum;
		this->bSaturation = pCycle->bSaturation;
		this->wAcqIdChannelProbe = pCycle->wAcqIdChannelProbe;
		this->wAcqIdChannelScan = pCycle->wAcqIdChannelScan;
		this->wAcqIdChannelCycle = pCycle->wAcqIdChannelCycle;
		this->fGainAnalog = pCycle->fGainAnalog;
		this->iFilterIndex = pCycle->iFilterIndex;
		this->bTrackingAscanEnable = pCycle->bTrackingAscanEnable;
		this->iTrackingAscanIndexCycle = pCycle->iTrackingAscanIndexCycle;
		this->iTrackingAscanIndexGate = pCycle->iTrackingAscanIndexGate;
		this->bTrackingDacEnable = pCycle->bTrackingDacEnable;
		this->iTrackingDacIndexCycle = pCycle->iTrackingDacIndexCycle;
		this->iTrackingDacIndexGate = pCycle->iTrackingDacIndexGate;
		this->iGateCount = pCycle->iGateCount;
		this->gate = gcnew array<csGate^>(g_iOEMPAGateCountMax);
		for(int iIndex=0;iIndex<g_iOEMPAGateCountMax;iIndex++)
		{
			//this->gate[iIndex] = safe_cast<csGate^>(Marshal::PtrToStructure((IntPtr)&pCycle->gate[iIndex],csGate::typeid));
			this->gate[iIndex] = gcnew csGate;
			this->gate[iIndex]->bEnable = pCycle->gate[iIndex].bEnable;
			this->gate[iIndex]->dStart = pCycle->gate[iIndex].dStart;
			this->gate[iIndex]->dStop = pCycle->gate[iIndex].dStop;
			this->gate[iIndex]->dThreshold = pCycle->gate[iIndex].dThreshold;
			this->gate[iIndex]->eRectification = (csEnumRectification)pCycle->gate[iIndex].eRectification;
			this->gate[iIndex]->eModeAmp = (csEnumGateModeAmp)pCycle->gate[iIndex].eModeAmp;
			this->gate[iIndex]->eModeTof = (csEnumGateModeTof)pCycle->gate[iIndex].eModeTof;
			this->gate[iIndex]->wAcqIDAmp = pCycle->gate[iIndex].wAcqIDAmp;
			this->gate[iIndex]->wAcqIDTof = pCycle->gate[iIndex].wAcqIDTof;
			this->gate[iIndex]->bTrackingStartEnable = pCycle->gate[iIndex].bTrackingStartEnable;
			this->gate[iIndex]->iTrackingStartIndexCycle = pCycle->gate[iIndex].iTrackingStartIndexCycle;
			this->gate[iIndex]->iTrackingStartIndexGate = pCycle->gate[iIndex].iTrackingStartIndexGate;
			this->gate[iIndex]->bTrackingStopEnable = pCycle->gate[iIndex].bTrackingStopEnable;
			this->gate[iIndex]->iTrackingStopIndexCycle = pCycle->gate[iIndex].iTrackingStopIndexCycle;
			this->gate[iIndex]->iTrackingStopIndexGate = pCycle->gate[iIndex].iTrackingStopIndexGate;
		}
		return true;
	}
	bool csCycle::CopyTo(structCycle *pCycle)
	{
		if(!pCycle)
			return false;
		pCycle->dGainDigital = this->dGainDigital;
		pCycle->fBeamCorrection = this->fBeamCorrection;
		pCycle->dStart = this->dStart;
		pCycle->dRange = this->dRange;
		pCycle->dTimeSlot = this->dTimeSlot;
		pCycle->dFMCSubTimeSlotAcq = this->dFMCSubTimeSlotAcq;
		pCycle->dFMCSubTimeSlotReplay = this->dFMCSubTimeSlotReplay;
		pCycle->lPointCount = this->lPointCount;
		pCycle->lPointFactor = this->lPointFactor;
		pCycle->eCompressionType = (enumCompressionType)this->eCompressionType;
		pCycle->eRectification = (enumRectification)this->eRectification;
		pCycle->bDACAutoStop = this->bDACAutoStop;
		pCycle->iDACCount = this->iDACCount;
		if((this->iDACCount>0) && (this->adDACTof!=nullptr) && (this->afDACPrm!=nullptr)
			&& (this->adDACTof->Rank==1) && (this->afDACPrm->Rank==1)
			&& (this->adDACTof->GetLength(0)==g_iOEMPADACCountMax) && (this->afDACPrm->GetLength(0)==g_iOEMPADACCountMax))
		{
			for(int iIndex=0;iIndex<this->iDACCount;iIndex++)
			{
				pCycle->adDACTof[iIndex] = this->adDACTof[iIndex];
				pCycle->afDACPrm[iIndex] = this->afDACPrm[iIndex];
			}
		}
		pCycle->bDACSlope = this->bDACSlope;
		pCycle->bDACEnable = this->bDACEnable;
		pCycle->bMaximum = this->bMaximum;
		pCycle->bMinimum = this->bMinimum;
		pCycle->bSaturation = this->bSaturation;
		pCycle->wAcqIdChannelProbe = this->wAcqIdChannelProbe;
		pCycle->wAcqIdChannelScan = this->wAcqIdChannelScan;
		pCycle->wAcqIdChannelCycle = this->wAcqIdChannelCycle;
		pCycle->fGainAnalog = this->fGainAnalog;
		pCycle->iFilterIndex = this->iFilterIndex;
		pCycle->bTrackingAscanEnable = this->bTrackingAscanEnable;
		pCycle->iTrackingAscanIndexCycle = this->iTrackingAscanIndexCycle;
		pCycle->iTrackingAscanIndexGate = this->iTrackingAscanIndexGate;
		pCycle->bTrackingDacEnable = this->bTrackingDacEnable;
		pCycle->iTrackingDacIndexCycle = this->iTrackingDacIndexCycle;
		pCycle->iTrackingDacIndexGate = this->iTrackingDacIndexGate;
		pCycle->iGateCount = this->iGateCount;
		if((this->gate != nullptr) && (this->gate->Length==g_iOEMPAGateCountMax))
		{
			for(int iIndex=0;iIndex<g_iOEMPAGateCountMax;iIndex++)
			{
				//this->gate[iIndex] = safe_cast<csGate^>(Marshal::PtrToStructure((IntPtr)&pCycle->gate[iIndex],csGate::typeid));
				pCycle->gate[iIndex].bEnable = this->gate[iIndex]->bEnable;
				pCycle->gate[iIndex].dStart = this->gate[iIndex]->dStart;
				pCycle->gate[iIndex].dStop = this->gate[iIndex]->dStop;
				pCycle->gate[iIndex].dThreshold = this->gate[iIndex]->dThreshold;
				pCycle->gate[iIndex].eRectification = (enumRectification)this->gate[iIndex]->eRectification;
				pCycle->gate[iIndex].eModeAmp = (enumGateModeAmp)this->gate[iIndex]->eModeAmp;
				pCycle->gate[iIndex].eModeTof = (enumGateModeTof)this->gate[iIndex]->eModeTof;
				pCycle->gate[iIndex].wAcqIDAmp = this->gate[iIndex]->wAcqIDAmp;
				pCycle->gate[iIndex].wAcqIDTof = this->gate[iIndex]->wAcqIDTof;
				pCycle->gate[iIndex].bTrackingStartEnable = this->gate[iIndex]->bTrackingStartEnable;
				pCycle->gate[iIndex].iTrackingStartIndexCycle = this->gate[iIndex]->iTrackingStartIndexCycle;
				pCycle->gate[iIndex].iTrackingStartIndexGate = this->gate[iIndex]->iTrackingStartIndexGate;
				pCycle->gate[iIndex].bTrackingStopEnable = this->gate[iIndex]->bTrackingStopEnable;
				pCycle->gate[iIndex].iTrackingStopIndexCycle = this->gate[iIndex]->iTrackingStopIndexCycle;
				pCycle->gate[iIndex].iTrackingStopIndexGate = this->gate[iIndex]->iTrackingStopIndexGate;
			}
		}
		return true;
	}
	bool csFocalLaw::CopyFrom(CFocalLaw *pFocalLaw)
	{
		if(!pFocalLaw)
			return false;
		//1.1.5.3k
		//if(bEmission)
		//	this = safe_cast<csFocalLaw^>(Marshal::PtrToStructure((IntPtr)&m_pEmission[iCycle],csFocalLaw::typeid));
		//else
		//	this = safe_cast<csFocalLaw^>(Marshal::PtrToStructure((IntPtr)&m_pReception[iCycle],csFocalLaw::typeid));
		this->dWedgeDelay = pFocalLaw->dWedgeDelay;
		this->iElementCount = pFocalLaw->iElementCount;
		this->adwElement = gcnew array<DWORD>(g_iOEMPAApertureDWordCount);
		for(int iIndex=0; iIndex<g_iOEMPAApertureDWordCount; iIndex++)
			this->adwElement[iIndex] = pFocalLaw->adwElement[iIndex];
		this->csFocusing = (csEnumFocusing)pFocalLaw->eFocusing;
		this->iDelayCount = pFocalLaw->iDelayCount;
		this->iFocalCount = pFocalLaw->iFocalCount;
		this->afDelay = gcnew array<float, 2>(pFocalLaw->iFocalCount, pFocalLaw->iElementCount);
		for (int iFocalIndex = 0; iFocalIndex<pFocalLaw->iFocalCount; iFocalIndex++)
		{
			for(int iElementIndex=0; iElementIndex<pFocalLaw->iElementCount; iElementIndex++)
				this->afDelay[iFocalIndex,iElementIndex] = pFocalLaw->afDelay[iFocalIndex][iElementIndex];
		}
		this->adFocalTimeOfFlight = gcnew array<double>(g_iOEMPAFocalCountMax);
		this->fCenterDelay = pFocalLaw->fCenterDelay;
		for(int iIndex=0; iIndex<pFocalLaw->iFocalCount; iIndex++)
			this->adFocalTimeOfFlight[iIndex] = pFocalLaw->adFocalTimeOfFlight[iIndex];
		this->iPrmCount = pFocalLaw->iPrmCount;
		this->afPrm = gcnew array<float>(g_iOEMPAApertureElementCountMax);
		for(int iIndex=0; iIndex<pFocalLaw->iElementCount; iIndex++)
			this->afPrm[iIndex] = pFocalLaw->afPrm[iIndex];
		this->dSkew = pFocalLaw->dSkew;
		this->dAngle = pFocalLaw->dAngle;
		this->dX = pFocalLaw->dX;
		this->dY = pFocalLaw->dY;
		this->dZ = pFocalLaw->dZ;
		this->dFocalX = pFocalLaw->dFocalX;
		this->dFocalY = pFocalLaw->dFocalY;
		this->dFocalZ = pFocalLaw->dFocalZ;
		this->hwAcqDecimation = gcnew array<BYTE>(g_iOEMPAApertureElementCountMax/2);
		for(int iIndex=0; iIndex<g_iOEMPAApertureElementCountMax/2; iIndex++)
			this->hwAcqDecimation[iIndex] = pFocalLaw->hwAcqDecimation.byte[iIndex];
		return true;
	}
	bool csFocalLaw::CopyTo(CFocalLaw *pFocalLaw)
	{
		pFocalLaw->dWedgeDelay = this->dWedgeDelay;
		pFocalLaw->iElementCount = this->iElementCount;
		pFocalLaw->adwElement.SetAllElementEnable(false);
		for(int iIndex=0; iIndex<this->adwElement->Length; iIndex++)
			pFocalLaw->adwElement[iIndex] = this->adwElement[iIndex];
		pFocalLaw->eFocusing = (enumFocusing)this->csFocusing;
		pFocalLaw->iDelayCount = this->iDelayCount;
		pFocalLaw->iFocalCount = this->iFocalCount;
		for (int iFocalIndex = 0; iFocalIndex<pFocalLaw->iFocalCount; iFocalIndex++)
		{
			for(int iElementIndex=0; iElementIndex<pFocalLaw->iElementCount; iElementIndex++)
				pFocalLaw->afDelay[iFocalIndex][iElementIndex] = this->afDelay[iFocalIndex,iElementIndex];
		}
		pFocalLaw->fCenterDelay = this->fCenterDelay;
		if(this->adFocalTimeOfFlight!=nullptr)
		for(int iIndex=0; iIndex<pFocalLaw->iFocalCount; iIndex++)
			pFocalLaw->adFocalTimeOfFlight[iIndex] = this->adFocalTimeOfFlight[iIndex];
		pFocalLaw->iPrmCount = this->iPrmCount;
		if(this->afPrm!=nullptr)
		for(int iIndex=0; iIndex<pFocalLaw->iElementCount; iIndex++)
			pFocalLaw->afPrm[iIndex] = this->afPrm[iIndex];
		pFocalLaw->dSkew = this->dSkew;
		pFocalLaw->dAngle = this->dAngle;
		pFocalLaw->dX = this->dX;
		pFocalLaw->dY = this->dY;
		pFocalLaw->dZ = this->dZ;
		pFocalLaw->dFocalX = this->dFocalX;
		pFocalLaw->dFocalY = this->dFocalY;
		pFocalLaw->dFocalZ = this->dFocalZ;
		if(this && (this->hwAcqDecimation!=nullptr))
		{
			for(int iIndex=0; iIndex<this->hwAcqDecimation->GetLength(0); iIndex++)
				pFocalLaw->hwAcqDecimation.byte[iIndex] = this->hwAcqDecimation[iIndex];
		}
		return true;
	}
	bool csCustomizedAPI::GetRoot([Out] csRoot^ %root)
	{
		if(!m_pRoot)
			return false;
		if(root==nullptr)
			root = gcnew csRoot;
		if(root==nullptr)
			return false;
		//root = safe_cast<csRoot^>(Marshal::PtrToStructure((IntPtr)m_pRoot,csRoot::typeid));
		return root->CopyFrom(m_pRoot);
	}
	bool csCustomizedAPI::SetRoot([In] csRoot^ %root)
	{
		if(!m_pRoot)
			return false;
		return root->CopyTo(m_pRoot);
	}
	bool csCustomizedAPI::GetCycle(int iCycle,[Out] csCycle^ %cycle)
	{
		if(!m_pCycle)
			return false;
		if(iCycle<0)
			return false;
		if(iCycle>=m_iCycleCount)
			return false;
		if(cycle==nullptr)
			cycle = gcnew csCycle;
		if(cycle==nullptr)
			return false;
		return cycle->CopyFrom(&m_pCycle[iCycle]);
	}
	bool csCustomizedAPI::SetCycle(int iCycle,[In] csCycle^ cycle)
	{
		if(cycle==nullptr)
			return false;
		if(!m_pCycle)
			return false;
		if(iCycle<0)
			return false;
		if(iCycle>=m_iCycleCount)
			return false;
		return cycle->CopyTo(&m_pCycle[iCycle]);
	}
	bool csCustomizedAPI::GetFocalLaw(bool bEmission,int iCycle,[Out] csFocalLaw^ %focalLaw)
	{
		CFocalLaw *pFocalLaw;

		if(bEmission && !m_pEmission)
			return false;
		if(!bEmission && !m_pReception)
			return false;
		if(iCycle<0)
			return false;
		if(iCycle>=m_iCycleCount)
			return false;
		if(focalLaw==nullptr)
			focalLaw = gcnew csFocalLaw;
		if(focalLaw==nullptr)
			return false;
		//1.1.5.3k
		//if(bEmission)
		//	focalLaw = safe_cast<csFocalLaw^>(Marshal::PtrToStructure((IntPtr)&m_pEmission[iCycle],csFocalLaw::typeid));
		//else
		//	focalLaw = safe_cast<csFocalLaw^>(Marshal::PtrToStructure((IntPtr)&m_pReception[iCycle],csFocalLaw::typeid));
		if(bEmission)
			pFocalLaw = &m_pEmission[iCycle];
		else
			pFocalLaw = &m_pReception[iCycle];
		return focalLaw->CopyFrom(pFocalLaw);
	}
	bool csCustomizedAPI::SetFocalLaw(bool bEmission,int iCycle,[In] csFocalLaw^ focalLaw)
	{
		CFocalLaw *pFocalLaw;

		if(focalLaw==nullptr)
			return false;
		if(focalLaw->iFocalCount>focalLaw->afDelay->GetLength(0))
			return false;
		if(focalLaw->iElementCount>focalLaw->afDelay->GetLength(1))
			return false;
		if(iCycle<0)
			return false;
		if(iCycle>=m_iCycleCount)
			return false;
		if(bEmission)
		{
			if(!m_pEmission)
				return false;
			pFocalLaw = &m_pEmission[iCycle];
		}else{
			if(!m_pReception)
				return false;
			pFocalLaw = &m_pReception[iCycle];
		}
		return focalLaw->CopyTo(pFocalLaw);
	}
	bool csCustomizedAPI::ReadFileWriteHW(String ^pValue)
	{
		wchar_t* y;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pValue);
		bRet = OEMPA_ReadFileWriteHW(m_pHWDeviceOEMPA,y);
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}
	bool csCustomizedAPI::ReadHWWriteFile(String ^pValue)
	{
		wchar_t* y;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pValue);
		bRet = OEMPA_ReadHWWriteFile(m_pHWDeviceOEMPA,g_iOEMPACycleCountMax,g_iOEMPAApertureElementCountMax,g_iOEMPAFocalCountMax,g_iOEMPADACCountMax,y);
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}
	bool csCustomizedAPI::WriteHW(csHWDeviceOEMPA^ %pOEMPA,csRoot^ %root,array<csCycle^>^ %cycle,array<csFocalLaw^>^ %emission,array<csFocalLaw^>^ %reception,csEnumAcquisitionState acqState)
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA;
		structRoot Root;
		csCycle^ cycle2;
		csFocalLaw ^emission2,^reception2;
		bool bRet;

		pHWDeviceOEMPA = (CHWDeviceOEMPA*)pOEMPA->GetHWDeviceOEMPA();
		if(pOEMPA==nullptr)
			return false;
		if(root==nullptr)
			return false;
		m_pRoot = &Root;
		if(!SetRoot(root))
			{bRet = false;goto end;}
		m_iCycleCount = Root.iCycleCount;
		if(m_iCycleCount>0)
		{
			m_pCycle = OEMPA_AllocCycle(m_iCycleCount);
			m_pEmission = new CFocalLaw[m_iCycleCount];
			m_pReception = new CFocalLaw[m_iCycleCount];
			if(!m_pCycle || !m_pEmission || !m_pReception)
				{bRet = false;goto end;}
			OEMPA_ResetArrayFocalLaw(m_iCycleCount,m_pEmission);
			OEMPA_ResetArrayFocalLaw(m_iCycleCount,m_pReception);
			for(int iCycle=0;iCycle<m_iCycleCount;iCycle++)
			{
				cycle2 = cycle[iCycle];
				emission2 = emission[iCycle];
				reception2 = reception[iCycle];
				if(!SetCycle(iCycle,cycle2))
					{bRet = false;goto end;}
				if(!SetFocalLaw(true,iCycle,emission2))
					{bRet = false;goto end;}
				if(!SetFocalLaw(false,iCycle,reception2))
					{bRet = false;goto end;}
			}
		}

		OEMPA_InitRoot(*m_pRoot,pHWDeviceOEMPA);
		bRet = OEMPA_WriteHW(pHWDeviceOEMPA,*m_pRoot,m_pCycle,m_pEmission,m_pReception,(enumAcquisitionState)acqState);
	end:
		m_pRoot = NULL;
		if(m_pCycle)
			delete [] m_pCycle;
		m_pCycle = NULL;
		if(m_pEmission)
			delete [] m_pEmission;
		m_pEmission = NULL;
		if(m_pReception)
			delete [] m_pReception;
		m_pReception = NULL;
		m_iCycleCount = -2;
		return bRet;
	}
#pragma endregion csCustomizedAPI
////////////////////////////////////////////////////////
#pragma region csSWFilterOEMPA
	csSWFilterOEMPA::csSWFilterOEMPA()
	{
		Free();
	}
	csSWFilterOEMPA::~csSWFilterOEMPA()
	{
		this->!csSWFilterOEMPA();
	}
	csSWFilterOEMPA::!csSWFilterOEMPA()
	{
		Free();
	}
	void csSWFilterOEMPA::Constructor(CHWDeviceOEMPA *pHWDeviceOEMPA,CSWDeviceOEMPA *pSWDeviceOEMPA,CSWFilterOEMPA *pSWFilterOEMPA)
	{
		m_pHWDeviceOEMPA = pHWDeviceOEMPA;
		m_pSWDeviceOEMPA = pSWDeviceOEMPA;
		m_pSWFilterOEMPA = pSWFilterOEMPA;
	}
	void csSWFilterOEMPA::Free()
	{
		m_pHWDeviceOEMPA = NULL;
		m_pSWDeviceOEMPA = NULL;
		m_pSWFilterOEMPA = NULL;
	}

	bool csSWFilterOEMPA::SetFilter(csEnumOEMPAFilter eFilter)
	{
		if(!m_pSWFilterOEMPA)
			return false;
		return m_pSWFilterOEMPA->SetFilter((enumOEMPAFilter)eFilter);
	}
	bool csSWFilterOEMPA::GetFilter([Out] csEnumOEMPAFilter %eFilter)
	{
		enumOEMPAFilter eFilter2;
		bool bRet;

		if(!m_pSWFilterOEMPA)
			return false;
		bRet = m_pSWFilterOEMPA->GetFilter(eFilter2);
		eFilter = (csEnumOEMPAFilter)eFilter2;
		return bRet;
	}
	bool csSWFilterOEMPA::SetTitle(String^ pValue)//useful for custom filter.
	{
		wchar_t* y;
		bool bRet;

		if(!m_pSWFilterOEMPA)
			return false;
		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pValue);
		bRet = m_pSWFilterOEMPA->SetTitle(y);
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}
	bool csSWFilterOEMPA::GetTitle([Out] String^ %pValue)
	{
		wchar_t pAux[MAX_PATH];

		if(!m_pSWFilterOEMPA)
			return false;
		if(!m_pSWFilterOEMPA->GetTitle(pAux,MAX_PATH))
			return false;
		pValue = Marshal::PtrToStringUni((IntPtr)pAux);
		return true;
	}

	//custom filter coefficient: functions used by the "CustomFilter" software.
	bool csSWFilterOEMPA::SetScale(WORD wScale)
	{
		if(!m_pSWFilterOEMPA)
			return false;
		return m_pSWFilterOEMPA->SetScale(wScale);
	}
	bool csSWFilterOEMPA::GetScale([Out] WORD %wScale)
	{
		WORD wScale2;
		bool bRet;

		if(!m_pSWFilterOEMPA)
			return false;
		bRet = m_pSWFilterOEMPA->GetScale(wScale2);
		wScale = wScale2;
		return bRet;
	}
	bool csSWFilterOEMPA::SetCoefficientCount(int iCoefficientCount)
	{
		if(!m_pSWFilterOEMPA)
			return false;
		return m_pSWFilterOEMPA->SetCoefficientCount(iCoefficientCount);
	}
	bool csSWFilterOEMPA::GetCoefficientCount([Out] int %iCoefficientCount)
	{
		int iCoefficientCount2;
		bool bRet;

		if(!m_pSWFilterOEMPA)
			return false;
		bRet = m_pSWFilterOEMPA->GetCoefficientCount(iCoefficientCount2);
		iCoefficientCount = iCoefficientCount2;
		return bRet;
	}
	bool csSWFilterOEMPA::SetCoefficient(int iCoefficientIndex,short wValue)
	{
		if(!m_pSWFilterOEMPA)
			return false;
		return m_pSWFilterOEMPA->SetCoefficient(iCoefficientIndex,wValue);
	}
	bool csSWFilterOEMPA::GetCoefficient(int iCoefficientIndex,[Out] short %wValue)
	{
		short wValue2;
		bool bRet;

		if(!m_pSWFilterOEMPA)
			return false;
		bRet = m_pSWFilterOEMPA->GetCoefficient(iCoefficientIndex,wValue2);
		wValue = wValue2;
		return bRet;
	}
	bool csSWFilterOEMPA::SetFilter(WORD wScale,array<short>^ wValue,bool bUpdateHardware)//if you want to update all hardware filter, it is quicker to call "CSWDeviceOEMPA::UpdateAllFilter" at the end and before to call "SetFilter" with "bUpdateHardware=false" for all filters.
	{
		short wValue2[g_iOEMPAFilterCoefficientMax];

		if(!m_pSWFilterOEMPA)
			return false;
		if(wValue->GetLength(0)!=g_iOEMPAFilterCoefficientMax)
			return false;
		for(int iIndex=0;iIndex<g_iOEMPAFilterCoefficientMax;iIndex++)
			wValue2[iIndex] = wValue[iIndex];
		return m_pSWFilterOEMPA->SetFilter(wScale,wValue2,bUpdateHardware);
	}
	bool csSWFilterOEMPA::GetFilter([Out] WORD %wScale,array<short>^ %wValue)
	{
		WORD wScale2;
		short wValue2[g_iOEMPAFilterCoefficientMax];
		bool bRet;

		if(!m_pSWFilterOEMPA)
			return false;
		bRet = m_pSWFilterOEMPA->GetFilter(wScale2,wValue2);
		wScale = wScale2;
		wValue = gcnew array<short>(g_iOEMPAFilterCoefficientMax);
		for(int iIndex=0;iIndex<g_iOEMPAFilterCoefficientMax;iIndex++)
			wValue[iIndex] = wValue2[iIndex];
		return bRet;
	}

#pragma endregion csSWFilterOEMPA
////////////////////////////////////////////////////////
#pragma region csSWDeviceOEMPA
	csSWDeviceOEMPA::csSWDeviceOEMPA()
	{
		Free();
	}
	csSWDeviceOEMPA::~csSWDeviceOEMPA()
	{
		this->!csSWDeviceOEMPA();
	}
	csSWDeviceOEMPA::!csSWDeviceOEMPA()
	{
		Free();
	}
	void csSWDeviceOEMPA::Constructor(CHWDeviceOEMPA *pHWDeviceOEMPA,CSWDeviceOEMPA *pSWDeviceOEMPA)
	{
		CSWFilterOEMPA *pSWFilterOEMPA;

		//array<int>^ strarray = gcnew array<int>(x);
		m_acsSWFilterOEMPA = gcnew array<csSWFilterOEMPA^>(eOEMPAFilter15+1);
		m_pHWDeviceOEMPA = pHWDeviceOEMPA;
		m_pSWDeviceOEMPA = pSWDeviceOEMPA;
		for(int iIndex=0;iIndex<eOEMPAFilter15;iIndex++)
		{
			pSWFilterOEMPA = &pSWDeviceOEMPA->Filter(iIndex);
			m_acsSWFilterOEMPA[iIndex] = gcnew csSWFilterOEMPA();
			m_acsSWFilterOEMPA[iIndex]->Constructor(pHWDeviceOEMPA,pSWDeviceOEMPA,pSWFilterOEMPA);
		}
	}
	void csSWDeviceOEMPA::Free()
	{
		if(m_acsSWFilterOEMPA!=nullptr)
		for(int iIndex=0;iIndex<eOEMPAFilter15;iIndex++)
		{
			m_acsSWFilterOEMPA[iIndex] = nullptr;
		}
		m_acsSWFilterOEMPA = nullptr;
		m_pHWDeviceOEMPA = NULL;
		m_pSWDeviceOEMPA = NULL;
	}
	csSWFilterOEMPA^ csSWDeviceOEMPA::Filter(int iFilterIndex)
	{
		if(iFilterIndex<0)
			return nullptr;
		if(iFilterIndex>=eOEMPAFilter15)
			return nullptr;
		return m_acsSWFilterOEMPA[iFilterIndex];
	}

	bool csSWDeviceOEMPA::IsPulserEnabled()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsPulserEnabled();
	}
//ethernet
	bool csSWDeviceOEMPA::SetIPAddress(String ^pValue)
	{
		wchar_t* y;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pValue);
		bRet = m_pSWDeviceOEMPA->SetIPAddress(y);
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}
	bool csSWDeviceOEMPA::GetIPAddress([Out] String^ %pValue)
	{
		wchar_t pAux[MAX_PATH];

		if(!m_pSWDeviceOEMPA)
			return false;
		if(!m_pSWDeviceOEMPA->GetIPAddress(pAux,MAX_PATH))
			return false;
		pValue = Marshal::PtrToStringUni((IntPtr)pAux);
		return true;
	}

	bool csSWDeviceOEMPA::SetPort(unsigned short usValue)
	{
		return _SetPort(usValue);
	}
	bool csSWDeviceOEMPA::_SetPort(unsigned short usValue)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->_SetPort(usValue);//same than "SetPort", required because of the preprocessor definition in "WinSpool.h": "#define SetPort SetPortW".
	}
	bool csSWDeviceOEMPA::GetPort(unsigned short %usValue)
	{
		unsigned short usValue2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetPort(usValue2);
		usValue = usValue2;
		return bRet;
	}

//USB3
	bool csSWDeviceOEMPA::IsUSB3Connected()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsUSB3Connected();
	}

//status registers (digital inputs and temperatures)
	bool csSWDeviceOEMPA::SetStatusRegister(csEnumUpdateStatus eCom)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetStatusRegister((enumUpdateStatus)eCom);
	}
	csEnumUpdateStatus csSWDeviceOEMPA::GetStatusRegister()
	{
		if(!m_pSWDeviceOEMPA)
			return (csEnumUpdateStatus)0;
		return (csEnumUpdateStatus)m_pSWDeviceOEMPA->GetStatusRegister();
	}

	bool csSWDeviceOEMPA::GetSerialNumber([Out] String^ %pSN)
	{
		wchar_t pAux[MAX_PATH];

		if(!m_pSWDeviceOEMPA)
			return false;
		if(!m_pSWDeviceOEMPA->GetSerialNumber(pAux,MAX_PATH))
			return false;
		pSN = Marshal::PtrToStringUni((IntPtr)pAux);
		return true;
	}

	bool csSWDeviceOEMPA::GetSystemType([Out] String^ %pType)
	{
		wchar_t pAux[MAX_PATH];

		if(!m_pSWDeviceOEMPA)
			return false;
		if(!m_pSWDeviceOEMPA->GetSystemType(pAux,MAX_PATH))
			return false;
		pType = Marshal::PtrToStringUni((IntPtr)pAux);
		return true;
	}

	int csSWDeviceOEMPA::GetRXBoardCount()
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->GetRXBoardCount();
	}
	int csSWDeviceOEMPA::GetApertureCountMax()//to get the maximum element count of an aperture.
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->GetApertureCountMax();
	}
	int csSWDeviceOEMPA::GetElementCountMax()//to get the maximum element count of the system (in case of mux).
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->GetElementCountMax();
	}

	const double csSWDeviceOEMPA::dGetClockPeriod()//ns
	{
		if(!m_pSWDeviceOEMPA)
			return 10.0;
		return m_pSWDeviceOEMPA->dGetClockPeriod();
	}
	const float csSWDeviceOEMPA::fGetClockPeriod()//ns
	{
		if(!m_pSWDeviceOEMPA)
			return 10.0f;
		return m_pSWDeviceOEMPA->fGetClockPeriod();
	}
	const long csSWDeviceOEMPA::lGetClockFrequency()//Hz
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->lGetClockFrequency();
	}

	WORD csSWDeviceOEMPA::GetFirmwareId()
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->GetFirmwareId();
	}

	int csSWDeviceOEMPA::GetCycleCountMax()
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->GetCycleCountMax();
	}
	bool csSWDeviceOEMPA::IsFullMatrixCapture()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsFullMatrixCapture();
	}
	bool csSWDeviceOEMPA::IsFullMatrixCaptureReadWrite()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsFullMatrixCaptureReadWrite();
	}
	bool csSWDeviceOEMPA::GetFMCElement([Out] int %iElementStart, [Out] int %iElementStop, [Out] int %iElementStep)
	{
		int iElementStart2,iElementStop2,iElementStep2;

		if(!m_pSWDeviceOEMPA)
			return false;
		if(!m_pSWDeviceOEMPA->GetFMCElement(iElementStart2,iElementStop2,iElementStep2))
			return false;
		iElementStart = iElementStart2;
		iElementStop = iElementStop2;
		iElementStep = iElementStep2;
		return true;
	}
	bool csSWDeviceOEMPA::IsMultiHWChannelSupported()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsMultiHWChannelSupported();
	}
	bool csSWDeviceOEMPA::IsTemperatureAlarmSupported()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsTemperatureAlarmSupported();
	}
	bool csSWDeviceOEMPA::IsMultiHWChannelEnabled()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsMultiHWChannelEnabled();
	}
	bool csSWDeviceOEMPA::IsMatrixAvailable()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsMatrixAvailable();
	}
	bool csSWDeviceOEMPA::IsLabviewAvailable()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsLabviewAvailable();
	}
	bool csSWDeviceOEMPA::IsTpacquisitionAvailable()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsTpacquisitionAvailable();
	}
	bool csSWDeviceOEMPA::IsWTSWAvailable()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsWTSWAvailable();
	}
	bool csSWDeviceOEMPA::IsEncoderDecimal()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsEncoderDecimal();
	}
	bool csSWDeviceOEMPA::IsFMCElementStepSupported()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsFMCElementStepSupported();
	}

	bool csSWDeviceOEMPA::SetKeepAlive(csEnumKeepAlive eKeepAlive)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetKeepAlive((enumKeepAlive)eKeepAlive);
	}
	csEnumKeepAlive csSWDeviceOEMPA::GetKeepAlive()
	{
		if(!m_pSWDeviceOEMPA)
			return (csEnumKeepAlive)0;
		return (csEnumKeepAlive)m_pSWDeviceOEMPA->GetKeepAlive();
	}

	bool csSWDeviceOEMPA::EnableAscan(bool bEnable)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->EnableAscan(bEnable);
	}
	bool csSWDeviceOEMPA::IsAscanEnabled()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsAscanEnabled();
	}

	bool csSWDeviceOEMPA::SetAscanBitSize(csEnumBitSize eBitSize)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetAscanBitSize((enumBitSize)eBitSize);
	}
	csEnumBitSize csSWDeviceOEMPA::GetAscanBitSize()
	{
		if(!m_pSWDeviceOEMPA)
			return (csEnumBitSize)0;
		return (csEnumBitSize)m_pSWDeviceOEMPA->GetAscanBitSize();
	}

	bool csSWDeviceOEMPA::SetAscanRequest(csEnumAscanRequest eAscanRequest)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetAscanRequest((enumAscanRequest)eAscanRequest);
	}
	csEnumAscanRequest csSWDeviceOEMPA::GetAscanRequest()
	{
		if(!m_pSWDeviceOEMPA)
			return (csEnumAscanRequest)0;
		return (csEnumAscanRequest)m_pSWDeviceOEMPA->GetAscanRequest();
	}

	bool csSWDeviceOEMPA::SetAscanRequestFrequency(double dFreq)//Hz
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetAscanRequestFrequency(dFreq);
	}
	bool csSWDeviceOEMPA::GetAscanRequestFrequency([Out] double %dFreq)
	{
		double dFreq2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetAscanRequestFrequency(dFreq2);
		dFreq = dFreq2;
		return bRet;
	}

	bool csSWDeviceOEMPA::EnableCscanTof(bool bEnable)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->EnableCscanTof(bEnable);
	}
	bool csSWDeviceOEMPA::IsCscanTofEnabled()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsCscanTofEnabled();
	}

	bool csSWDeviceOEMPA::SetCycleCount(int iCount)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetCycleCount(iCount);
	}
	int csSWDeviceOEMPA::GetCycleCount()
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->GetCycleCount();
	}

	bool csSWDeviceOEMPA::SetTriggerMode(csEnumOEMPATrigger eTrig)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetTriggerMode((enumOEMPATrigger)eTrig);
	}
	csEnumOEMPATrigger csSWDeviceOEMPA::GetTriggerMode()
	{
		if(!m_pSWDeviceOEMPA)
			return (csEnumOEMPATrigger)0;
		return (csEnumOEMPATrigger)m_pSWDeviceOEMPA->GetTriggerMode();
	}

	bool csSWDeviceOEMPA::SetTriggerEncoderStep(double dStep)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetTriggerEncoderStep(dStep);
	}
	bool csSWDeviceOEMPA::GetTriggerEncoderStep([Out] double %dStep)
	{
		double dStep2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetTriggerEncoderStep(dStep2);
		dStep = dStep2;
		return bRet;
	}

	bool csSWDeviceOEMPA::SetSignalTriggerHighTime(double dTime)
	{
		double dTime2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		dTime2 = dTime;
		bRet = m_pSWDeviceOEMPA->SetSignalTriggerHighTime(dTime2);
		dTime = dTime2;
		return bRet;
	}
	double csSWDeviceOEMPA::GetSignalTriggerHighTime()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->GetSignalTriggerHighTime();
	}

	bool csSWDeviceOEMPA::SetRequestIO(csEnumOEMPARequestIO eRequest)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetRequestIO((enumOEMPARequestIO)eRequest);
	}
	csEnumOEMPARequestIO csSWDeviceOEMPA::GetRequestIO()
	{
		if(!m_pSWDeviceOEMPA)
			return (csEnumOEMPARequestIO)0;
		return (csEnumOEMPARequestIO)m_pSWDeviceOEMPA->GetRequestIO();
	}

	bool csSWDeviceOEMPA::SetRequestIODigitalInputMaskRising(int iMask)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetRequestIODigitalInputMaskRising(iMask);
	}
	bool csSWDeviceOEMPA::GetRequestIODigitalInputMaskRising([Out] int %iMask)
	{
		int iMask2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetRequestIODigitalInputMaskRising(iMask2);
		iMask = iMask2;
		return bRet;
	}

	bool csSWDeviceOEMPA::SetRequestIODigitalInputMaskFalling(int iEvent)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetRequestIODigitalInputMaskFalling(iEvent);
	}
	bool csSWDeviceOEMPA::GetRequestIODigitalInputMaskFalling([Out] int %iEvent)
	{
		int iEvent2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetRequestIODigitalInputMaskFalling(iEvent2);
		iEvent = iEvent2;
		return bRet;
	}

	bool csSWDeviceOEMPA::SetExternalTriggerCycle(csEnumDigitalInput eDigitalInput)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetExternalTriggerCycle((enumDigitalInput)eDigitalInput);
	}
	bool csSWDeviceOEMPA::GetExternalTriggerCycle([Out] csEnumDigitalInput %eDigitalInput)
	{
		enumDigitalInput eDigitalInput2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetExternalTriggerCycle(eDigitalInput2);
		eDigitalInput = (csEnumDigitalInput)eDigitalInput2;
		return bRet;
	}

	bool csSWDeviceOEMPA::SetExternalTriggerSequence(csEnumDigitalInput eDigitalInput)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetExternalTriggerSequence((enumDigitalInput)eDigitalInput);
	}
	bool csSWDeviceOEMPA::GetExternalTriggerSequence([Out] csEnumDigitalInput %eDigitalInput)
	{
		enumDigitalInput eDigitalInput2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetExternalTriggerSequence(eDigitalInput2);
		eDigitalInput = (csEnumDigitalInput)eDigitalInput2;
		return bRet;
	}

	bool csSWDeviceOEMPA::SetMappingOutput(int iOutputIndex,csEnumOEMPAMappingDigitalOutput eMapping)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetMappingOutput(iOutputIndex,(enumOEMPAMappingDigitalOutput)eMapping);
	}
	bool csSWDeviceOEMPA::GetMappingOutput(int iOutputIndex,[Out] csEnumOEMPAMappingDigitalOutput %eMapping)
	{
		enumOEMPAMappingDigitalOutput eMapping2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetMappingOutput(iOutputIndex,eMapping2);
		eMapping = (csEnumOEMPAMappingDigitalOutput)eMapping2;
		return bRet;
	}

	bool csSWDeviceOEMPA::SetRequestCscan(csEnumOEMPARequestCscan eRequest)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetRequestCscan((enumOEMPARequestCscan)eRequest);
	}
	csEnumOEMPARequestCscan csSWDeviceOEMPA::GetRequestCscan()
	{
		if(!m_pSWDeviceOEMPA)
			return (csEnumOEMPARequestCscan)0;
		return (csEnumOEMPARequestCscan)m_pSWDeviceOEMPA->GetRequestCscan();
	}

	bool csSWDeviceOEMPA::SetEncoderDebouncer(double dTime)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetEncoderDebouncer(dTime);
	}
	double csSWDeviceOEMPA::GetEncoderDebouncer()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->GetEncoderDebouncer();
	}

	bool csSWDeviceOEMPA::SetDigitalInput(WORD usValue)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetDigitalInput(usValue);
	}
	WORD csSWDeviceOEMPA::GetDigitalInput()
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->GetDigitalInput();
	}

	bool csSWDeviceOEMPA::SetDigitalDebouncer(double dTime)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetDigitalDebouncer(dTime);
	}
	double csSWDeviceOEMPA::GetDigitalDebouncer()
	{
		if(!m_pSWDeviceOEMPA)
			return 0.0;
		return m_pSWDeviceOEMPA->GetDigitalDebouncer();
	}

	bool csSWDeviceOEMPA::ResetEncoder()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->ResetEncoder();
	}

	bool csSWDeviceOEMPA::GetTemperatureCount([Out] int %iBoardCount,[Out] int %iSensorCount)
	{
		int iBoardCount2,iSensorCount2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetTemperatureCount(iBoardCount2,iSensorCount2);
		iBoardCount = iBoardCount2;
		iSensorCount = iSensorCount2;
		return bRet;
	}
	bool csSWDeviceOEMPA::GetTemperatureSensorCount(int iBoardIndex,[Out] int %iSensorCount)
	{
		int iSensorCount2;
		bool bRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetTemperatureSensorCount(iBoardIndex,iSensorCount2);
		iSensorCount = iSensorCount2;
		return bRet;
	}
	bool csSWDeviceOEMPA::GetTemperature(int iBoardIndex,int iSensorIndex,[Out] float %fValue)
	{
		float fValue2;
		int iRet;

		if(!m_pSWDeviceOEMPA)
			return false;
		iRet = m_pSWDeviceOEMPA->GetTemperature(iBoardIndex,iSensorIndex,fValue2);
		fValue = fValue2;
		return (iRet?false:true);
	}

	//SW member to enable the high level API update of acquisition status, see preferences member "AcqStatusEnable" and "AcqStatusTimer".
	bool csSWDeviceOEMPA::EnableTimerUpdate(bool bEnable)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->EnableTimerUpdate(bEnable);
	}

	bool csSWDeviceOEMPA::IsIOBoardEnabled()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsIOBoardEnabled();
	}
	bool csSWDeviceOEMPA::IsOEMMCEnabled()
	{
		if(!m_pSWDeviceOEMPA)
			return 0.0;
		return m_pSWDeviceOEMPA->IsIOBoardEnabled();
	}
	double csSWDeviceOEMPA::GetPulserPowerMax()
	{
		if(!m_pSWDeviceOEMPA)
			return 0.0;
		return m_pSWDeviceOEMPA->GetPulserPowerMax();
	}
	double csSWDeviceOEMPA::GetPulserPowerCurrent()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->GetPulserPowerCurrent();
	}
	BYTE csSWDeviceOEMPA::GetFlashUSB3Version()
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->GetFlashUSB3Version();
	}
	DWORD csSWDeviceOEMPA::GetFWUSB3Version()
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->GetFWUSB3Version();
	}
	bool csSWDeviceOEMPA::EnableUSB3(bool bEnable)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->EnableUSB3(bEnable);
	}
	bool csSWDeviceOEMPA::IsUSB3Enabled()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsUSB3Enabled();
	}
	DWORD csSWDeviceOEMPA::GetMBOptions()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->GetMBOptions();
	}

	bool csSWDeviceOEMPA::GetEmbeddedVersion([Out] csVersion^ %version)
	{
		bool bRet;
		CSWDeviceOEMPA::structVersion _version;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetEmbeddedVersion(_version);
		version = safe_cast<csVersion^>(Marshal::PtrToStructure((IntPtr)&_version,csVersion::typeid));
		return bRet;
	}
	bool csSWDeviceOEMPA::GetOptionsCom([Out] csOptionsCom^ %optionsCom)
	{
		bool bRet;
		CSWDeviceOEMPA::structOptionsCom _optionsCom;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetOptionsCom(_optionsCom);
		optionsCom = safe_cast<csOptionsCom^>(Marshal::PtrToStructure((IntPtr)&_optionsCom,csOptionsCom::typeid));
		return bRet;
	}
	bool csSWDeviceOEMPA::GetOptionsTCP([Out] csOptionsTCP^ %optionsTCP)
	{
		bool bRet;
		CSWDeviceOEMPA::structOptionsTCP _optionsTCP;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetOptionsTCP(_optionsTCP);
		optionsTCP = safe_cast<csOptionsTCP^>(Marshal::PtrToStructure((IntPtr)&_optionsTCP,csOptionsTCP::typeid));
		return bRet;
	}
	bool csSWDeviceOEMPA::GetOptionsFlash([Out] csOptionsFlash^ %optionsFlash)
	{
		bool bRet;
		CSWDeviceOEMPA::structOptionsFlash _optionsFlash;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetOptionsFlash(_optionsFlash);
		optionsFlash = safe_cast<csOptionsFlash^>(Marshal::PtrToStructure((IntPtr)&_optionsFlash,csOptionsFlash::typeid));
		return bRet;
	}
	int csSWDeviceOEMPA::GetPasscodeCount()
	{
		if(!m_pSWDeviceOEMPA)
			return 0;
		return m_pSWDeviceOEMPA->GetPasscodeCount();
	}
	bool csSWDeviceOEMPA::GetPasscode(int iIndex,[Out] DWORD %dwPasscode)
	{
		bool bRet;
		DWORD _dwPasscode;

		dwPasscode = 0;
		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetPasscode(iIndex,_dwPasscode);
		dwPasscode = _dwPasscode;
		return bRet;
	}
	double csSWDeviceOEMPA::GetMaximumThroughput()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->GetMaximumThroughput();
	}

	bool csSWDeviceOEMPA::UpdateAllFilter()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->UpdateAllFilter();
	}

	void csSWDeviceOEMPA::EnableMultiChannel(bool bEnable)
	{
		CSWDeviceOEMPA::EnableMultiChannel(bEnable);
	}
	bool csSWDeviceOEMPA::IsMultiChannelEnabled()
	{
		return CSWDeviceOEMPA::IsMultiChannelEnabled();
	}
	void csSWDeviceOEMPA::EnableLoadDefaultSetup(bool bEnable)
	{
		CSWDeviceOEMPA::EnableLoadDefaultSetup(bEnable);
	}
	bool csSWDeviceOEMPA::IsLoadDefaultSetupEnabled()
	{
		return CSWDeviceOEMPA::IsLoadDefaultSetupEnabled();
	}

	bool csSWDeviceOEMPA::IsTimeOffsetSupported()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsTimeOffsetSupported();
	}
	void csSWDeviceOEMPA::AlignmentCfgUpdated()
	{
		CSWDeviceOEMPA::AlignmentCfgUpdated();
	}
	void csSWDeviceOEMPA::SetCalibrationParameters(float fWidth,float fStart,float fRange,float fGainAnalog,double dGainDigital)
	{
		if(!m_pSWDeviceOEMPA)
			return;
		return m_pSWDeviceOEMPA->SetCalibrationParameters(fWidth,fStart,fRange,fGainAnalog,dGainDigital);
	}
	void csSWDeviceOEMPA::GetCalibrationParameters(float &fWidth,float &fStart,float &fRange,float &fGainAnalog,double &dGainDigital)
	{
		if(!m_pSWDeviceOEMPA)
			return;
		return m_pSWDeviceOEMPA->GetCalibrationParameters(fWidth,fStart,fRange,fGainAnalog,dGainDigital);
	}
	bool csSWDeviceOEMPA::IsCalibrationPerformed()
	{
		return IsAlignmentPerformed();
	}
	bool csSWDeviceOEMPA::IsAlignmentPerformed()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsAlignmentPerformed();
	}
	bool csSWDeviceOEMPA::EnableAlignment(bool bEnable)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->EnableAlignment(bEnable);
	}
	float csSWDeviceOEMPA::GetCalibrationAlignment()
	{
		if(!m_pSWDeviceOEMPA)
			return 0.0f;
		return m_pSWDeviceOEMPA->GetCalibrationAlignment();
	}
	float csSWDeviceOEMPA::GetCalibrationOffset()
	{
		if(!m_pSWDeviceOEMPA)
			return 0.0f;
		return m_pSWDeviceOEMPA->GetCalibrationOffset();
	}
	bool csSWDeviceOEMPA::IsAlignmentEnabled()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsAlignmentEnabled();
	}
	bool csSWDeviceOEMPA::ResetAlignment()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->ResetAlignment();
	}
	bool csSWDeviceOEMPA::SetCalibrationFileReport(String ^pFileReport)
	{
		wchar_t* y;

		if(!m_pSWDeviceOEMPA)
			return false;
		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(pFileReport);
		m_pSWDeviceOEMPA->SetCalibrationFileReport(y);
		Marshal::FreeHGlobal((IntPtr)y);
		return true;
	}
	bool csSWDeviceOEMPA::GetCalibrationFileReport([Out] String^ %pFileReport)
	{
		wchar_t pAux[MAX_PATH];

		if(!m_pSWDeviceOEMPA)
			return false;
		if(!m_pSWDeviceOEMPA->GetCalibrationFileReport(MAX_PATH,pAux))
			return false;
		pFileReport = Marshal::PtrToStringUni((IntPtr)pAux);
		return true;
	}
	bool csSWDeviceOEMPA::SetTimeOffset(float fTimeOffset)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->SetTimeOffset(fTimeOffset);
	}
	float csSWDeviceOEMPA::GetTimeOffset()
	{
		if(!m_pSWDeviceOEMPA)
			return 0.0f;
		return m_pSWDeviceOEMPA->GetTimeOffset();
	}
	void csSWDeviceOEMPA::EnablePulserDuringReplay(bool bEnable)
	{
		if(!m_pSWDeviceOEMPA)
			return;
		m_pSWDeviceOEMPA->EnablePulserDuringReplay(bEnable);
	}
	bool csSWDeviceOEMPA::IsPulserDuringReplayEnabled()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsPulserDuringReplayEnabled();
	}
	bool csSWDeviceOEMPA::EnableCscanTimeOfFlightCorrection(bool bEnable)
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->EnableCscanTimeOfFlightCorrection(bEnable);
	}
	bool csSWDeviceOEMPA::IsCscanTimeOfFlightCorrectionEnabled()
	{
		if(!m_pSWDeviceOEMPA)
			return false;
		return m_pSWDeviceOEMPA->IsCscanTimeOfFlightCorrectionEnabled();
	}
	bool csSWDeviceOEMPA::GetCscanTimeOfFlightCorrection(int iCycle,BYTE %byDecimation,float %fAscanStart)
	{
		bool bRet;
		BYTE byDecimation2;
		float fAscanStart2;

		if(!m_pSWDeviceOEMPA)
			return false;
		bRet = m_pSWDeviceOEMPA->GetCscanTimeOfFlightCorrection(iCycle,byDecimation2,fAscanStart2);
		byDecimation = byDecimation2;
		fAscanStart = fAscanStart2;
		return bRet;
	}
	bool csSWDeviceOEMPA::SetCscanTimeOfFlightCorrection(int iCycle,BYTE byDecimation,float fAscanStart)
	{
		BYTE byDecimation2;
		float fAscanStart2;

		if(!m_pSWDeviceOEMPA)
			return false;
		byDecimation2 = byDecimation;
		fAscanStart2 = fAscanStart;
		return m_pSWDeviceOEMPA->SetCscanTimeOfFlightCorrection(iCycle,byDecimation2,fAscanStart2);
	}
	double csSWDeviceOEMPA::GetFWAscanRecoveryTime()
	{
		if(!m_pSWDeviceOEMPA)
			return 0.0;
		return m_pSWDeviceOEMPA->GetFWAscanRecoveryTime();
	}
	double csSWDeviceOEMPA::GetFMCSubCycleRecoveryTime()
	{
		if(!m_pSWDeviceOEMPA)
			return 0.0;
		return m_pSWDeviceOEMPA->GetFMCSubCycleRecoveryTime();
	}
	double csSWDeviceOEMPA::GetFMCCycleRecoveryTime()
	{
		if(!m_pSWDeviceOEMPA)
			return 0.0;
		return m_pSWDeviceOEMPA->GetFMCCycleRecoveryTime();
	}
#pragma endregion csSWDeviceOEMPA
////////////////////////////////////////////////////////
#pragma region csHWDeviceOEMPA
	csHWDeviceOEMPA::csHWDeviceOEMPA() : csHWDevice()
	{
		bool bCreate;
		int iValue = CSWDevice::ReadCfgInt(L"UTKernel\\Device",L"csDriverOEMPACount",0,bCreate);

		iValue++;
		CSWDevice::WriteCfgInt(L"UTKernel\\Device",L"csDriverOEMPACount",iValue);
		m_pHWDeviceOEMPA = new CHWDeviceOEMPA();
		m_csCustomizedAPI = gcnew csCustomizedAPI(m_pHWDeviceOEMPA);
		m_pHWDevice = dynamic_cast<CHWDevice*>(m_pHWDeviceOEMPA);
		m_pSWDeviceOEMPA = m_pHWDeviceOEMPA->GetSWDeviceOEMPA();
		csHWDevice::Constructor(m_pHWDeviceOEMPA,m_pHWDevice,m_csCustomizedAPI);
		m_csSWDeviceOEMPA = gcnew csSWDeviceOEMPA();
		m_csSWDeviceOEMPA->Constructor(m_pHWDeviceOEMPA,m_pSWDeviceOEMPA);
		m_FifoAscan = gcnew csAcquisitionFifo(csEnumAcquisitionFifo::csFifoAscan, this);
		m_FifoCscan = gcnew csAcquisitionFifo(csEnumAcquisitionFifo::csFifoCscan, this);
		m_FifoIO = gcnew csAcquisitionFifo(csEnumAcquisitionFifo::csFifoIO, this);
		if(m_pHWDevice)
			m_pHWDevice->SetDerivedClass(L"csHWDeviceOEMPA",NULL);
	}
	csHWDeviceOEMPA::~csHWDeviceOEMPA()
	{
		this->!csHWDeviceOEMPA();
	}
	csHWDeviceOEMPA::!csHWDeviceOEMPA()
	{
		Free();
	}
	void csHWDeviceOEMPA::Free()
	{
		m_pSWDeviceOEMPA = NULL;
		if(m_pHWDeviceOEMPA)
			delete m_pHWDeviceOEMPA;
		m_pHWDeviceOEMPA = NULL;
	}
	csAcquisitionFifo ^csHWDeviceOEMPA::GetAcquisitionFifo(csEnumAcquisitionFifo csFifo)
	{
		switch(csFifo)
		{
		case csEnumAcquisitionFifo::csFifoAscan: return m_FifoAscan;break;
		case csEnumAcquisitionFifo::csFifoCscan: return m_FifoCscan;break;
		case csEnumAcquisitionFifo::csFifoIO: return m_FifoIO;break;
		}
		return nullptr;
	}
	csSWDeviceOEMPA^ csHWDeviceOEMPA::GetSWDeviceOEMPA()
	{
		return m_csSWDeviceOEMPA;
	}
	csHWDevice ^csHWDeviceOEMPA::GetHWDevice()
	{
		return this;
	}
	csCustomizedAPI ^csHWDeviceOEMPA::GetCustomizedAPI()
	{
		return m_csCustomizedAPI;
	}

	/*unsafe*/bool csHWDeviceOEMPA::GetDigitalInput(DWORD *pdwData)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetDigitalInput(pdwData);
	}
	bool csHWDeviceOEMPA::DisableUSB3(bool bDisable)
	{
		bool bRet,bDisable2;

		if(!m_pHWDeviceOEMPA)
			return false;
		bDisable2 = bDisable;
		bRet = m_pHWDeviceOEMPA->DisableUSB3(bDisable2);
		bDisable = bDisable2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetUSB3Disabled(/*fixed*/[Out] bool *pbDisable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetUSB3Disabled(pbDisable);
	}

	/*unsafe*/bool csHWDeviceOEMPA::GetApertureCountMax(int *piCount)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetApertureCountMax(piCount);
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetElementCountMax(int *piCount)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetElementCountMax(piCount);
	}

	/*unsafe*/bool csHWDeviceOEMPA::GetTemperatureSensor(int iIndexBoard,int iIndexSensor,WORD *pwTemperature)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetTemperatureSensor(iIndexBoard,iIndexSensor,pwTemperature);
	}
	bool csHWDeviceOEMPA::SetTemperatureAlarm(BYTE &byWarning,BYTE &byAlarm)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->SetTemperatureAlarm(byWarning,byAlarm);
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetTemperatureAlarm(BYTE *pbyWarning,BYTE *pbyAlarm)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetTemperatureAlarm(pbyWarning,pbyAlarm);
	}

	//MultiProcess management begin
	bool csHWDeviceOEMPA::IsMultiProcessRegistered()
	{
		return CHWDeviceOEMPA::IsMultiProcessRegistered();
	}
	bool csHWDeviceOEMPA::RegisterMultiProcess(String ^wcProcessName)
	{
		wchar_t* y;
		bool bRet;

		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(wcProcessName);
		bRet = CHWDeviceOEMPA::RegisterMultiProcess(y);
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}
	bool csHWDeviceOEMPA::UnregisterMultiProcess()
	{
		return CHWDeviceOEMPA::UnregisterMultiProcess();
	}
	bool csHWDeviceOEMPA::IsMultiProcessConnected(String ^wcIPAddress,[Out] DWORD %dwProcessId)
	{
		wchar_t* y;
		bool bRet;
		DWORD dwProcessId2;

		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(wcIPAddress);
		bRet = CHWDeviceOEMPA::IsMultiProcessConnected(y,dwProcessId2);
		dwProcessId = dwProcessId2;
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}
	bool csHWDeviceOEMPA::DisconnectMultiProcess(String ^wcIPAddress,DWORD dwProcessId)
	{
		wchar_t* y;
		bool bRet;
		DWORD dwProcessId2;

		y = (wchar_t*)(void*)Marshal::StringToHGlobalUni(wcIPAddress);
		dwProcessId2 = dwProcessId;
		bRet = CHWDeviceOEMPA::DisconnectMultiProcess(y,dwProcessId2);
		Marshal::FreeHGlobal((IntPtr)y);
		return bRet;
	}
	int csHWDeviceOEMPA::GetMultiProcessCount()
	{
		return CHWDeviceOEMPA::GetMultiProcessCount();
	}
	bool csHWDeviceOEMPA::GetMultiProcessInfo(int iIndex,[Out] DWORD %dwProcessId,[Out] String ^wcProcessName)
	{
		bool bRet;
		DWORD dwProcessId2;
		wchar_t wcProcessName2[MAX_PATH/2];

		bRet = CHWDeviceOEMPA::GetMultiProcessInfo(iIndex,dwProcessId2,MAX_PATH/2,wcProcessName2);
		dwProcessId = dwProcessId2;
		wcProcessName = Marshal::PtrToStringUni((IntPtr)wcProcessName2);
		return bRet;
	}
	//MultiProcess management end

	bool csHWDeviceOEMPA::EnableFMC(bool bEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->EnableFMC(bEnable);
	}
	bool csHWDeviceOEMPA::GetEnableFMC(bool *pbEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableFMC(pbEnable);
	}
	bool csHWDeviceOEMPA::SetFMCElement(int %iElementStart,int %iElementStop,int %iElementStep)
	{
		int iElementStart2,iElementStop2,iElementStep2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iElementStart2 = iElementStart;
		iElementStop2 = iElementStop;
		iElementStep2 = iElementStep;
		bRet = m_pHWDeviceOEMPA->SetFMCElement(iElementStart2,iElementStop2,iElementStep2);
		iElementStart = iElementStart2;
		iElementStop = iElementStop2;
		iElementStep = iElementStep2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetFMCElement(int *piElementStart,int *piElementStop,int *piElementStep)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetFMCElement(piElementStart,piElementStop,piElementStep);
	}
	bool csHWDeviceOEMPA::SetFMCElementStart(int %iElementIndex)
	{
		int iElementIndex2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iElementIndex2 = iElementIndex;
		bRet = m_pHWDeviceOEMPA->SetFMCElementStart(iElementIndex2);
		iElementIndex = iElementIndex2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetFMCElementStart(int *piElementIndex)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetFMCElementStart(piElementIndex);
	}
	bool csHWDeviceOEMPA::SetFMCElementStop(int %iElementIndex)
	{
		int iElementIndex2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iElementIndex2 = iElementIndex;
		bRet = m_pHWDeviceOEMPA->SetFMCElementStop(iElementIndex2);
		iElementIndex = iElementIndex2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetFMCElementStop(int *piElementIndex)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetFMCElementStop(piElementIndex);
	}
	bool csHWDeviceOEMPA::EnableMultiHWChannel(bool bEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->EnableMultiHWChannel(bEnable);
	}
	bool csHWDeviceOEMPA::GetEnableMultiHWChannel(bool *pbEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableMultiHWChannel(pbEnable);
	}

	bool csHWDeviceOEMPA::ResetTimeStamp()
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->ResetTimeStamp();
	}

	bool csHWDeviceOEMPA::ResetEncoder(int iEncoderIndex)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->ResetEncoder(iEncoderIndex);
	}
	bool csHWDeviceOEMPA::SetEncoder(int iEncoderIndex,double %dValue)
	{
		double dValue2=dValue;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bRet = m_pHWDeviceOEMPA->SetEncoder(iEncoderIndex,dValue2);
		dValue = dValue2;
		return bRet;
	}
	bool csHWDeviceOEMPA::SetEncoder(int iEncoderIndex,DWORD %dwValue)
	{
		DWORD dwValue2=dwValue;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bRet = m_pHWDeviceOEMPA->SetEncoder(iEncoderIndex,dwValue2);
		dwValue = dwValue2;
		return bRet;
	}
	bool csHWDeviceOEMPA::SetEncoderType(csEnumEncoderType %eEncoder1Type,csEnumEncoderType %eEncoder2Type)
	{
		enumEncoderType eEncoder1Type2,eEncoder2Type2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eEncoder1Type2 = (enumEncoderType)eEncoder1Type;
		eEncoder2Type2 = (enumEncoderType)eEncoder2Type;
		bRet = m_pHWDeviceOEMPA->SetEncoderType(eEncoder1Type2,eEncoder2Type2);
		eEncoder1Type = (csEnumEncoderType)eEncoder1Type2;
		eEncoder2Type = (csEnumEncoderType)eEncoder2Type2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEncoderType(csEnumEncoderType *peEncoder1Type,csEnumEncoderType *peEncoder2Type)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEncoderType((enumEncoderType*)peEncoder1Type,(enumEncoderType*)peEncoder2Type);
	}

	bool csHWDeviceOEMPA::EnableAscan(bool bAscan)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->EnableAscan(bAscan);
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEnableAscan(bool *pbAscan)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableAscan(pbAscan);
	}

	bool csHWDeviceOEMPA::EnableCscanTof(bool bCscanTof)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->EnableCscanTof(bCscanTof);
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEnableCscanTof(bool *pbCscanTof)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableCscanTof(pbCscanTof);
	}

	bool csHWDeviceOEMPA::SetAscanBitSize(csEnumBitSize %eBitSize)
	{
		enumBitSize eBitSize2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eBitSize2 = (enumBitSize)eBitSize;
		bRet = m_pHWDeviceOEMPA->SetAscanBitSize(eBitSize2);
		eBitSize = (csEnumBitSize)eBitSize2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetAscanBitSize(csEnumBitSize *peBitSize)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetAscanBitSize((enumBitSize*)peBitSize);
	}

	bool csHWDeviceOEMPA::SetAscanRequest(csEnumAscanRequest %eAscanRequest)
	{
		enumAscanRequest eAscanRequest2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eAscanRequest2 = (enumAscanRequest)eAscanRequest;
		bRet = m_pHWDeviceOEMPA->SetAscanRequest(eAscanRequest2);
		eAscanRequest = (csEnumAscanRequest)eAscanRequest2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetAscanRequest(csEnumAscanRequest *peAscanRequest)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetAscanRequest((enumAscanRequest*)peAscanRequest);
	}

	bool csHWDeviceOEMPA::SetAscanRequestFrequency(double %dValue)
	{
		double dValue2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dValue2 = dValue;
		bRet = m_pHWDeviceOEMPA->SetAscanRequestFrequency(dValue2);
		dValue = dValue2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetAscanRequestFrequency(double *pdValue)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetAscanRequestFrequency(pdValue);
	}
	bool csHWDeviceOEMPA::CheckAscanRequestFrequency(double %dValue)
	{
		double dValue2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dValue2 = dValue;
		bRet = m_pHWDeviceOEMPA->CheckAscanRequestFrequency(dValue2);
		dValue = dValue2;
		return bRet;
	}

	/*unsafe*/bool csHWDeviceOEMPA::GetFWId(WORD *pwFWId)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetFWId(pwFWId);
	}

	bool csHWDeviceOEMPA::SetCycleCount(int %lCycleCount)
	{
		int lCycleCount2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		lCycleCount2 = lCycleCount;
		bRet = m_pHWDeviceOEMPA->SetCycleCount(lCycleCount2);
		lCycleCount = lCycleCount2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetCycleCount(/*fixed*/[Out] int *piCycleCount)//handle de l objet pointe
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetCycleCount(piCycleCount);
	}
	bool csHWDeviceOEMPA::CheckCycleCount(int %iCycleCount)
	{
		int iCycleCount2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iCycleCount2 = iCycleCount;
		bRet = m_pHWDeviceOEMPA->CheckCycleCount(iCycleCount2);
		iCycleCount = iCycleCount2;
		return bRet;
	}

	bool csHWDeviceOEMPA::SetTriggerMode(csEnumOEMPATrigger %eTrig)
	{
		enumOEMPATrigger eTrig2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eTrig2 = (enumOEMPATrigger)eTrig;
		bRet = m_pHWDeviceOEMPA->SetTriggerMode(eTrig2);
		eTrig = (csEnumOEMPATrigger)eTrig2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetTriggerMode(/*fixed*/[Out] csEnumOEMPATrigger *peTrig)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetTriggerMode((enumOEMPATrigger*)peTrig);
	}

	bool csHWDeviceOEMPA::SetEncoderDirection(csEnumOEMPAEncoderDirection &eEncoderDirection)
	{
		enumOEMPAEncoderDirection eEncoderDirection2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eEncoderDirection2 = (enumOEMPAEncoderDirection)eEncoderDirection;
		bRet = m_pHWDeviceOEMPA->SetEncoderDirection(eEncoderDirection2);
		eEncoderDirection = (csEnumOEMPAEncoderDirection)eEncoderDirection2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEncoderDirection(/*fixed*/[Out] csEnumOEMPAEncoderDirection *peEncoderDirection)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEncoderDirection((enumOEMPAEncoderDirection*)peEncoderDirection);
	}

	bool csHWDeviceOEMPA::SetTriggerEncoderStep(double %dStep)
	{
		double dStep2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dStep2 = dStep;
		bRet = m_pHWDeviceOEMPA->SetTriggerEncoderStep(dStep2);
		dStep = dStep2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetTriggerEncoderStep(/*fixed*/[Out] double *pdStep)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetTriggerEncoderStep(pdStep);
	}

	bool csHWDeviceOEMPA::SetSignalTriggerHighTime(double %dTime)
	{
		double dTime2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dTime2 = dTime;
		bRet = m_pHWDeviceOEMPA->SetSignalTriggerHighTime(dTime2);
		dTime = dTime2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetSignalTriggerHighTime(/*fixed*/[Out] double *pdTime)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetSignalTriggerHighTime(pdTime);
	}

	bool csHWDeviceOEMPA::SetRequestIO(csEnumOEMPARequestIO %eRequest)
	{
		enumOEMPARequestIO eRequest2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eRequest2 = (enumOEMPARequestIO)eRequest;
		bRet = m_pHWDeviceOEMPA->SetRequestIO(eRequest2);
		eRequest = (csEnumOEMPARequestIO)eRequest2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetRequestIO(/*fixed*/ [Out] csEnumOEMPARequestIO *peRequest)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetRequestIO((enumOEMPARequestIO*)peRequest);
	}

	bool csHWDeviceOEMPA::SetRequestIODigitalInputMask(int %iMaskFalling,int %iMaskRising)
	{
		int iMaskFalling2,iMaskRising2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iMaskFalling2 = iMaskFalling;
		iMaskRising2 = iMaskRising;
		bRet = m_pHWDeviceOEMPA->SetRequestIODigitalInputMask(iMaskFalling2,iMaskRising2);
		iMaskFalling = iMaskFalling2;
		iMaskRising = iMaskRising2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetRequestIODigitalInputMask(/*fixed*/[Out] int *piMaskFalling,/*fixed*/[Out] int *piMaskRising)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetRequestIODigitalInputMask(piMaskFalling,piMaskRising);
	}
	bool csHWDeviceOEMPA::CheckRequestDigitalInputMask(int %iMask)
	{
		int iMask2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bRet = m_pHWDeviceOEMPA->CheckRequestDigitalInputMask(iMask2);
		iMask = iMask2;
		return bRet;
	}

	/*unsafe*/bool csHWDeviceOEMPA::GetFilterCoefficient(csEnumOEMPAFilter eFilter,/*fixed*/WORD *pwScale,/*fixed*/[Out] array<short>^ %wValue/*g_iOEMPAFilterCoefficientMax*/)
	{
		WORD wValue2[g_iOEMPAFilterCoefficientMax];
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		wValue = gcnew array<short>(g_iOEMPAFilterCoefficientMax);
		bRet = m_pHWDeviceOEMPA->GetFilterCoefficient((enumOEMPAFilter)eFilter,*pwScale,wValue2);
		for(int i=0;i<g_iOEMPAFilterCoefficientMax;i++)
			wValue[i] = wValue2[i];
		return bRet;
	}
	bool csHWDeviceOEMPA::FindFilterCoefficient(WORD %wScale,array<short>^ %wValue/*g_iOEMPAFilterCoefficientMax*/,[Out] csEnumOEMPAFilter^ %eFilter)
	{
		short pwValue2[g_iOEMPAFilterCoefficientMax];
		enumOEMPAFilter eFilter2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(wValue->Length!=g_iOEMPAFilterCoefficientMax)
			return false;
		for(int i=0;i<g_iOEMPAFilterCoefficientMax;i++)
			pwValue2[i] = wValue[i];
		bRet = m_pHWDeviceOEMPA->FindFilterCoefficient(wScale,pwValue2,eFilter2);
		eFilter = (csEnumOEMPAFilter)eFilter2;
		return bRet;
	}
	bool csHWDeviceOEMPA::SetFilter(csEnumOEMPAFilterIndex eFilterIndex,WORD %wScale,array<short>^ %wValue/*g_iOEMPAFilterCoefficientMax*/)
	{
		WORD wScale2;
		short pwValue2[g_iOEMPAFilterCoefficientMax];
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(wValue->Length!=g_iOEMPAFilterCoefficientMax)
			return false;
		wScale2 = wScale;
		for(int i=0;i<g_iOEMPAFilterCoefficientMax;i++)
			pwValue2[i] = wValue[i];
		bRet = m_pHWDeviceOEMPA->SetFilter((enumOEMPAFilterIndex)eFilterIndex,wScale2,pwValue2);
		wScale = wScale2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetFilter(csEnumOEMPAFilterIndex eFilterIndex,/*fixed*/WORD *pwScale,/*fixed*/[Out] array<short>^ %wValue/*g_iOEMPAFilterCoefficientMax*/)
	{
		short *pwValue;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		wValue = gcnew array<short>(g_iOEMPAFilterCoefficientMax);
		pwValue = (short*)(void*)ListAddObject(wValue);
		bRet = m_pHWDeviceOEMPA->GetFilter((enumOEMPAFilterIndex)eFilterIndex,pwScale,pwValue);
		return bRet;
	}

	bool csHWDeviceOEMPA::SetEncoderWire1(int iEncoderIndex,csEnumDigitalInput %eDigitalInput)//int iEncoderIndex : 0 for first encoder, 1 for second encoder.
	{
		enumDigitalInput eDigitalInput2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eDigitalInput2 = (enumDigitalInput)eDigitalInput;
		bRet = m_pHWDeviceOEMPA->SetEncoderWire1(iEncoderIndex,eDigitalInput2);
		eDigitalInput = (csEnumDigitalInput)eDigitalInput2;
		return bRet;
	}
	bool csHWDeviceOEMPA::SetEncoderWire2(int iEncoderIndex,csEnumDigitalInput %eDigitalInput)
	{
		enumDigitalInput eDigitalInput2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eDigitalInput2 = (enumDigitalInput)eDigitalInput;
		bRet = m_pHWDeviceOEMPA->SetEncoderWire2(iEncoderIndex,eDigitalInput2);
		eDigitalInput = (csEnumDigitalInput)eDigitalInput2;
		return bRet;
	}
	bool csHWDeviceOEMPA::SetExternalTriggerCycle(csEnumDigitalInput %eDigitalInput)
	{
		enumDigitalInput eDigitalInput2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eDigitalInput2 = (enumDigitalInput)eDigitalInput;
		bRet = m_pHWDeviceOEMPA->SetExternalTriggerCycle(eDigitalInput2);
		eDigitalInput = (csEnumDigitalInput)eDigitalInput2;
		return bRet;
	}
	bool csHWDeviceOEMPA::SetExternalTriggerSequence(csEnumDigitalInput %eDigitalInput)
	{
		enumDigitalInput eDigitalInput2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eDigitalInput2 = (enumDigitalInput)eDigitalInput;
		bRet = m_pHWDeviceOEMPA->SetExternalTriggerSequence(eDigitalInput2);
		eDigitalInput = (csEnumDigitalInput)eDigitalInput2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEncoderWire1(int iEncoderIndex,/*fixed*/[Out] csEnumDigitalInput *peDigitalInput)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEncoderWire1(iEncoderIndex,(enumDigitalInput*)peDigitalInput);
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEncoderWire2(int iEncoderIndex,/*fixed*/[Out] csEnumDigitalInput *peDigitalInput)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEncoderWire2(iEncoderIndex,(enumDigitalInput*)peDigitalInput);
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetExternalTriggerCycle(/*fixed*/[Out] csEnumDigitalInput *peDigitalInput)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetExternalTriggerCycle((enumDigitalInput*)peDigitalInput);
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetExternalTriggerSequence(/*fixed*/[Out] csEnumDigitalInput *peDigitalInput)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetExternalTriggerSequence((enumDigitalInput*)peDigitalInput);
	}

	bool csHWDeviceOEMPA::SetDigitalOutput(int iIndex,csEnumOEMPAMappingDigitalOutput eMappingDigitalOutput)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->SetDigitalOutput(iIndex,(enumOEMPAMappingDigitalOutput)eMappingDigitalOutput);
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetDigitalOutput(int iIndex,/*fixed*/[Out] csEnumOEMPAMappingDigitalOutput *peMappingDigitalOutput)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetDigitalOutput(iIndex,(enumOEMPAMappingDigitalOutput*)peMappingDigitalOutput);
	}

	bool csHWDeviceOEMPA::SetEncoderDebouncer(double %dValue)
	{
		double dValue2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dValue2 = dValue;
		bRet = m_pHWDeviceOEMPA->SetEncoderDebouncer(dValue2);
		dValue = dValue2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEncoderDebouncer(/*fixed*/[Out] double *pdValue)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEncoderDebouncer(pdValue);
	}
	bool csHWDeviceOEMPA::CheckEncoderDebouncer(double %dValue)
	{
		double dValue2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dValue2 = dValue;
		bRet = m_pHWDeviceOEMPA->CheckEncoderDebouncer(dValue2);
		dValue = dValue2;
		return bRet;
	}

	bool csHWDeviceOEMPA::SetDigitalDebouncer(double %dValue)
	{
		double dValue2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bRet = m_pHWDeviceOEMPA->SetDigitalDebouncer(dValue2);
		dValue = dValue2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetDigitalDebouncer(/*fixed*/[Out] double *pdValue)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetDigitalDebouncer(pdValue);
	}
	bool csHWDeviceOEMPA::CheckDigitalDebouncer(double %dValue)
	{
		double dValue2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dValue2 = dValue;
		bRet = m_pHWDeviceOEMPA->CheckDigitalDebouncer(dValue2);
		dValue = dValue2;
		return bRet;
	}

	bool csHWDeviceOEMPA::SetGainDigital(int iCycle,double %dGain)
	{
		double dGain2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dGain2 = dGain;
		bRet = m_pHWDeviceOEMPA->SetGainDigital(iCycle,dGain2);
		dGain = dGain2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetGainDigital(int iCycle,/*fixed*/[Out] double *pdGain)
	{
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bRet = m_pHWDeviceOEMPA->GetGainDigital(iCycle,pdGain);
		return bRet;
	}
	bool csHWDeviceOEMPA::CheckGainDigital(double %dGain)
	{
		double dGain2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dGain2 = dGain;
		bRet = m_pHWDeviceOEMPA->CheckGainDigital(dGain2);
		dGain = dGain2;
		return bRet;
	}
	void/*CHWDeviceOEMPA*/ *csHWDeviceOEMPA::GetHWDeviceOEMPA()
	{
		return m_pHWDeviceOEMPA;
	}
	void csHWDeviceOEMPA::test()
	{
		structRoot root;
		structCycle cycle;

		OEMPA_ResetStructCycle(&cycle);
		root.iCycleCount = 1;
		cycle.dGainDigital = 12.0;
		gCallbackCustomizedAPI(m_pHWDeviceOEMPA,L"Hello world!",eWriteHW_Enter,&root,&cycle,NULL,NULL);
		gCallbackSystemMessageBoxList(L"Hello world!!");
		csHWDevice::test();
	}
	int csHWDeviceOEMPA::Test()
	{
		return 0;
	}
	bool csHWDeviceOEMPA::SetBeamCorrection(int iCycle,float %fGain)
	{
		float fGain2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		fGain2 = fGain;
		bRet = m_pHWDeviceOEMPA->SetBeamCorrection(iCycle,fGain2);
		fGain = fGain2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetBeamCorrection(int iCycle,float *pfGain)
	{
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bRet = m_pHWDeviceOEMPA->GetBeamCorrection(iCycle,pfGain);
		return bRet;
	}
	bool csHWDeviceOEMPA::CheckBeamCorrection(float %fGain)
	{
		float fGain2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		fGain2 = fGain;
		bRet = m_pHWDeviceOEMPA->CheckBeamCorrection(fGain2);
		fGain = fGain2;
		return bRet;
	}

	bool csHWDeviceOEMPA::SetDACSlope(int iCycle,acsDac^ %dac/*int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac*/)
	{
		structCallbackArrayFloatDac callback;
		int iMax=g_iOEMPADACCountMax;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(dac==nullptr)
			return false;
		if(dac->list==nullptr)
			return false;
		if(!dac->list->Length)
			return false;
		if(dac->list->Length>=iMax)
			return false;
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)dac->GetGcroot();//pdac;
		callback.apParameter[2] = NULL;
		callback.pSetSize = gCallbackSetSizeDac;
		callback.pSetData = gCallbackSetDataDac;
		callback.pGetSize = gCallbackGetSizeDac;
		callback.pGetData = gCallbackGetDataDac;
		iMax = dac->list->Length;
		bRet = m_pHWDeviceOEMPA->SetDACSlope(iCycle,iMax,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::GetDACSlope(int iCycle,[Out] acsDac^ %dac/*int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac*/)
	{
		structCallbackArrayFloatDac callback;
		int iMax=g_iOEMPADACCountMax;
		bool bRet;
		gcroot<acsDac^> *pdac;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(dac==nullptr)
			dac = gcnew acsDac;
		pdac = dac->GetGcroot();
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)pdac;
		callback.apParameter[2] = NULL;
		callback.pSetSize = gCallbackSetSizeDac;
		callback.pSetData = gCallbackSetDataDac;
		callback.pGetSize = gCallbackGetSizeDac;
		callback.pGetData = gCallbackGetDataDac;
		bRet = m_pHWDeviceOEMPA->GetDACSlope(iCycle,iMax,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::SetDACGain(bool bAutoStop,int iCycle,acsDac^ %dac/*int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac*/)
	{
		structCallbackArrayFloatDac callback;
		int iMax=g_iOEMPADACCountMax;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(dac==nullptr)
			return false;
		if(dac->list==nullptr)
			return false;
		if(!dac->list->Length)
			return false;
		if(dac->list->Length>=iMax)
			return false;
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)dac->GetGcroot();//pdac;
		callback.apParameter[2] = (void*)1;
		callback.pSetSize = gCallbackSetSizeDac;
		callback.pSetData = gCallbackSetDataDac;
		callback.pGetSize = gCallbackGetSizeDac;
		callback.pGetData = gCallbackGetDataDac;
		iMax = dac->list->Length;
		bRet = m_pHWDeviceOEMPA->SetDACGain(bAutoStop,iCycle,iMax,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::GetDACGain(int iCycle,[Out] acsDac^ %dac/*int &iCountMax,structCallbackArrayFloatDac &callbackArrayFloatDac*/)
	{
		structCallbackArrayFloatDac callback;
		int iMax=g_iOEMPADACCountMax;
		bool bRet;
		gcroot<acsDac^> *pdac;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(dac==nullptr)
			dac = gcnew acsDac;
		pdac = dac->GetGcroot();
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)pdac;
		callback.apParameter[2] = (void*)1;
		callback.pSetSize = gCallbackSetSizeDac;
		callback.pSetData = gCallbackSetDataDac;
		callback.pGetSize = gCallbackGetSizeDac;
		callback.pGetData = gCallbackGetDataDac;
		bRet = m_pHWDeviceOEMPA->GetDACGain(iCycle,iMax,callback);
		return bRet;
	}
////////////
	bool csHWDeviceOEMPA::SetEmissionDelays(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/,acsFloat^ %afDelay/*,structCallbackArrayFloat1D &callbackArrayFloat1D*/)
	{
		DWORD adwHWAperture2[g_iOEMPAApertureDWordCount];
		structCallbackArrayFloat1D callback;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adwHWAperture==nullptr) || (adwHWAperture->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture2[i] = adwHWAperture[i];
		if(afDelay==nullptr)
			return false;
		if(afDelay->list==nullptr)
			return false;
		if(afDelay->list->Length>g_iOEMPAApertureElementCountMax)
			return false;
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)afDelay->GetGcroot();//pdac;
		callback.pSetSize = gCallbackSetSizeFloat;
		callback.pSetData = gCallbackSetDataFloat;
		callback.pGetSize = gCallbackGetSizeFloat;
		callback.pGetData = gCallbackGetDataFloat;
		bRet = m_pHWDeviceOEMPA->SetEmissionDelays(iCycle,adwHWAperture2,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::GetEmissionDelays(int iCycle,/*fixed*/[Out] acsFloat^ %afDelay/*,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D*/)
	{
		structCallbackArrayFloat1D callback;
		int iMax=g_iOEMPAApertureElementCountMax;
		bool bRet;
		gcroot<acsFloat^> *pDelay;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(afDelay==nullptr)
			afDelay = gcnew acsFloat;
		pDelay = afDelay->GetGcroot();
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)pDelay;
		callback.pSetSize = gCallbackSetSizeFloat;
		callback.pSetData = gCallbackSetDataFloat;
		callback.pGetSize = gCallbackGetSizeFloat;
		callback.pGetData = gCallbackGetDataFloat;
		bRet = m_pHWDeviceOEMPA->GetEmissionDelays(iCycle,iMax,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::SetEmissionWidths(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/,acsFloat^ %afWidth/*,structCallbackArrayFloat1D &callbackArrayFloat1D*/)
	{
		DWORD adwHWAperture2[g_iOEMPAApertureDWordCount];
		structCallbackArrayFloat1D callback;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adwHWAperture==nullptr) || (adwHWAperture->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture2[i] = adwHWAperture[i];
		if(afWidth==nullptr)
			return false;
		if(afWidth->list==nullptr)
			return false;
		if(afWidth->list->Length>g_iOEMPAApertureElementCountMax)
			return false;
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)afWidth->GetGcroot();//pdac;
		callback.pSetSize = gCallbackSetSizeFloat;
		callback.pSetData = gCallbackSetDataFloat;
		callback.pGetSize = gCallbackGetSizeFloat;
		callback.pGetData = gCallbackGetDataFloat;
		bRet = m_pHWDeviceOEMPA->SetEmissionWidths(iCycle,adwHWAperture2,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::GetEmissionWidths(int iCycle,/*fixed*/[Out] acsFloat^ %afWidth/*,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D*/)
	{
		structCallbackArrayFloat1D callback;
		int iMax=g_iOEMPAApertureElementCountMax;
		bool bRet;
		gcroot<acsFloat^> *pWidth;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(afWidth==nullptr)
			afWidth = gcnew acsFloat;
		pWidth = afWidth->GetGcroot();
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)pWidth;
		callback.pSetSize = gCallbackSetSizeFloat;
		callback.pSetData = gCallbackSetDataFloat;
		callback.pGetSize = gCallbackGetSizeFloat;
		callback.pGetData = gCallbackGetDataFloat;
		bRet = m_pHWDeviceOEMPA->GetEmissionWidths(iCycle,iMax,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::SetReceptionGains(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/,acsFloat^ %afGain/*,structCallbackArrayFloat1D &callbackArrayFloat1D*/)
	{
		DWORD adwHWAperture2[g_iOEMPAApertureDWordCount];
		structCallbackArrayFloat1D callback;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adwHWAperture==nullptr) || (adwHWAperture->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture2[i] = adwHWAperture[i];
		if(afGain==nullptr)
			return false;
		if(afGain->list==nullptr)
			return false;
		if(afGain->list->Length>g_iOEMPAApertureElementCountMax)
			return false;
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)afGain->GetGcroot();
		callback.pSetSize = gCallbackSetSizeFloat;
		callback.pSetData = gCallbackSetDataFloat;
		callback.pGetSize = gCallbackGetSizeFloat;
		callback.pGetData = gCallbackGetDataFloat;
		bRet = m_pHWDeviceOEMPA->SetReceptionGains(iCycle,adwHWAperture2,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::GetReceptionGains(int iCycle,/*fixed*/[Out] acsFloat^ %afGain/*,int &iElementCountMax,structCallbackArrayFloat1D &callbackArrayFloat1D*/)
	{
		structCallbackArrayFloat1D callback;
		int iMax=g_iOEMPAApertureElementCountMax;
		bool bRet;
		gcroot<acsFloat^> *pGain;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(afGain==nullptr)
			afGain = gcnew acsFloat;
		pGain = afGain->GetGcroot();
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)pGain;
		callback.pSetSize = gCallbackSetSizeFloat;
		callback.pSetData = gCallbackSetDataFloat;
		callback.pGetSize = gCallbackGetSizeFloat;
		callback.pGetData = gCallbackGetDataFloat;
		bRet = m_pHWDeviceOEMPA->GetReceptionGains(iCycle,iMax,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::SetReceptionDelays(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/,acsDelayReception^ %afDelay/*,structCallbackArrayFloat2D &callbackArrayFloat2D*/)
	{
		DWORD adwHWAperture2[g_iOEMPAApertureDWordCount];
		structCallbackArrayFloat2D callback;
		int iMax=g_iOEMPAFocalCountMax;//g_iOEMPADACCountMax;
		int iSize1,iSize2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adwHWAperture==nullptr) || (adwHWAperture->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture2[i] = adwHWAperture[i];
		if(afDelay==nullptr)
			return false;
		if(afDelay->list==nullptr)
			return false;
		iSize1 = afDelay->list->GetLength(0);//element count
		iSize2 = afDelay->list->GetLength(1);//focal count
		if(iSize2>iMax)
			return false;
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)afDelay->GetGcroot();//pdac;
		callback.pSetSize = gCallbackSetSizeDelay2;
		callback.pSetData = gCallbackSetDataDelay2;
		callback.pGetSize = gCallbackGetSizeDelay2;
		callback.pGetData = gCallbackGetDataDelay2;
		bRet = m_pHWDeviceOEMPA->SetReceptionDelays(iCycle,adwHWAperture2,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::GetReceptionDelays(int iCycle,/*fixed*/[Out] acsDelayReception^ %afDelay/*,int &iElementCountMax,int &iFocalCountMax,structCallbackArrayFloat2D &callbackArrayFloat2D*/)
	{
		structCallbackArrayFloat2D callback;
		int iMax1=g_iOEMPAApertureElementCountMax;
		int iMax2=g_iOEMPAFocalCountMax;
		bool bRet;
		gcroot<acsDelayReception^> *pDelay2;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(afDelay==nullptr)
			afDelay = gcnew acsDelayReception;
		pDelay2 = afDelay->GetGcroot();
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)pDelay2;
		callback.pSetSize = gCallbackSetSizeDelay2;
		callback.pSetData = gCallbackSetDataDelay2;
		callback.pGetSize = gCallbackGetSizeDelay2;
		callback.pGetData = gCallbackGetDataDelay2;
		bRet = m_pHWDeviceOEMPA->GetReceptionDelays(iCycle,iMax1,iMax2,callback);
		return bRet;
	}
////////////
	bool csHWDeviceOEMPA::EnableDAC(int iCycle,bool %bEnable)
	{
		bool bEnable2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bEnable2 = bEnable;
		bRet = m_pHWDeviceOEMPA->EnableDAC(iCycle,bEnable2);
		bEnable = bEnable2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEnableDAC(int iCycle,bool *pbEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableDAC(iCycle,pbEnable);
	}
	bool csHWDeviceOEMPA::CheckDACSlope(double %dTime,float %fSlope)
	{
		double dTime2;
		float fSlope2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dTime2 = dTime;
		fSlope2 = fSlope;
		bRet = m_pHWDeviceOEMPA->CheckDACSlope(dTime2,fSlope2);
		dTime = dTime2;
		fSlope = fSlope2;
		return bRet;
	}
	bool csHWDeviceOEMPA::CheckDACCount(int %iCount)
	{
		int iCount2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iCount2 = iCount;
		bRet = m_pHWDeviceOEMPA->CheckDACCount(iCount2);
		iCount = iCount2;
		return bRet;
	}

	bool csHWDeviceOEMPA::SetAscanRectification(int iCycle,csEnumRectification %eRectification)
	{
		enumRectification eRectification2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eRectification2 = (enumRectification)eRectification;
		bRet = m_pHWDeviceOEMPA->SetAscanRectification(iCycle,eRectification2);
		eRectification = (csEnumRectification)eRectification2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetAscanRectification(int iCycle,csEnumRectification *peRectification)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetAscanRectification(iCycle,(enumRectification*)peRectification);
	}

	bool csHWDeviceOEMPA::SetAscanStart(int iCycle,double %dStart)
	{
		double dStart2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dStart2 = dStart;
		bRet = m_pHWDeviceOEMPA->SetAscanStart(iCycle,dStart2);
		dStart = dStart2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetAscanStart(int iCycle,double *pdStart)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetAscanStart(iCycle,pdStart);
	}
	bool csHWDeviceOEMPA::CheckAscanStart(double %dStart)
	{
		double dStart2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dStart2 = dStart;
		bRet = m_pHWDeviceOEMPA->CheckAscanStart(dStart2);
		dStart = dStart2;
		return bRet;
	}
	
	bool csHWDeviceOEMPA::SetAscanRange(int iCycle,double %dRange,/*output only*/[Out] csEnumCompressionType %eCompressionType,/*output only*/[Out] long %lPointCount,/*output only*/[Out] long %lPointFactor)
	{
		double dRange2;
		enumCompressionType eCompressionType2;
		long lPointCount2,lPointFactor2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dRange2 = dRange;
		eCompressionType2 = (enumCompressionType)eCompressionType;
		bRet = m_pHWDeviceOEMPA->SetAscanRange(iCycle,dRange2,eCompressionType2,lPointCount2,lPointFactor2);
		dRange = dRange2;
		eCompressionType = (csEnumCompressionType)eCompressionType2;
		lPointCount = lPointCount2;
		lPointFactor = lPointFactor2;
		return bRet;
	}
	bool csHWDeviceOEMPA::SetAscanRangeWithFactor(int iCycle,double %dRange,csEnumCompressionType %eCompressionType,/*in/out*/long %lPointFactor,/*output only*/[Out] long %lPointCount)
	{
		double dRange2;
		enumCompressionType eCompressionType2;
		long lPointCount2,lPointFactor2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dRange2 = dRange;
		eCompressionType2 = (enumCompressionType)eCompressionType;
		lPointFactor2 = lPointFactor;
		bRet = m_pHWDeviceOEMPA->SetAscanRangeWithFactor(iCycle,dRange2,eCompressionType2,lPointFactor2,lPointCount2);
		dRange = dRange2;
		eCompressionType = (csEnumCompressionType)eCompressionType2;
		lPointFactor = lPointFactor2;
		lPointCount = lPointCount2;
		return bRet;
	}
	bool csHWDeviceOEMPA::SetAscanRangeWithCount(int iCycle,double %dRange,/*in/out (check)*/csEnumCompressionType %eCompressionType,/*in/out (check)*/long %lPointCount,/*output only*/[Out] long %lPointFactor)
	{
		double dRange2=dRange;
		enumCompressionType eCompressionType2=(enumCompressionType)eCompressionType;
		long lPointCount2=lPointCount,lPointFactor2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eCompressionType2 = (enumCompressionType)eCompressionType;
		bRet = m_pHWDeviceOEMPA->SetAscanRangeWithCount(iCycle,dRange2,eCompressionType2,lPointCount2,lPointFactor2);
		dRange = dRange2;
		eCompressionType = (csEnumCompressionType)eCompressionType2;
		lPointCount = lPointCount2;
		lPointFactor = lPointFactor2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetAscanRange(int iCycle,double *pdRange,csEnumCompressionType *peCompressionType,long *plPointCount,long *plPointFactor)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetAscanRange(iCycle,pdRange,(enumCompressionType*)peCompressionType,plPointCount,plPointFactor);
	}
	bool csHWDeviceOEMPA::CheckAscanRange(double %dRange)
	{
		double dRange2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dRange2 = dRange;
		bRet = m_pHWDeviceOEMPA->CheckAscanRange(dRange2);
		dRange = dRange2;
		return bRet;
	}
	bool csHWDeviceOEMPA::CheckAscanRangeWithCount(double %dRange,csEnumCompressionType %eCompressionType,long %lPointCount)
	{
		double dRange2;
		enumCompressionType eCompressionType2;
		long lPointCount2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eCompressionType2 = (enumCompressionType)eCompressionType;
		bRet = m_pHWDeviceOEMPA->CheckAscanRangeWithCount(dRange2,eCompressionType2,lPointCount2);
		dRange = dRange2;
		eCompressionType = (csEnumCompressionType)eCompressionType2;
		lPointCount = lPointCount2;
		return bRet;
	}
	bool csHWDeviceOEMPA::GetSamplingFrequency(csEnumCompressionType %eCompressionType,long %lPointFactor,[Out] double %dSamplingFrequency/*Hertz*/)
	{
		double dSamplingFrequency2;
		enumCompressionType eCompressionType2;
		long lPointFactor2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eCompressionType2 = (enumCompressionType)eCompressionType;
		lPointFactor2 = (long)lPointFactor;
		bRet = m_pHWDeviceOEMPA->GetSamplingFrequency(eCompressionType2,lPointFactor2,dSamplingFrequency2);
		dSamplingFrequency = dSamplingFrequency2;
		return bRet;
	}

	bool csHWDeviceOEMPA::SetFilterIndex(int iCycle,csEnumOEMPAFilterIndex %eFilterIndex)
	{
		enumOEMPAFilterIndex eFilterIndex2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		eFilterIndex2 = (enumOEMPAFilterIndex)eFilterIndex;
		bRet = m_pHWDeviceOEMPA->SetFilterIndex(iCycle,eFilterIndex2);
		eFilterIndex = (csEnumOEMPAFilterIndex)eFilterIndex2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetFilterIndex(int iCycle,csEnumOEMPAFilterIndex *peFilterIndex)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetFilterIndex(iCycle,(enumOEMPAFilterIndex*)peFilterIndex);
	}
		
	bool csHWDeviceOEMPA::SetTimeSlot(int iCycle,double %dTime)
	{
		double dTime2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dTime2 = dTime;
		bRet = m_pHWDeviceOEMPA->SetTimeSlot(iCycle,dTime2);
		dTime = dTime2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetTimeSlot(int iCycle,double *pdTime)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetTimeSlot(iCycle,pdTime);
	}
	bool csHWDeviceOEMPA::CheckTimeSlot(double %dTime)
	{
		double dTime2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dTime2 = dTime;
		bRet = m_pHWDeviceOEMPA->CheckTimeSlot(dTime2);
		dTime = dTime2;
		return bRet;
	}
	double csHWDeviceOEMPA::GetTimeSlotMinimum(long lAscanPointCount, enumBitSize eAscanBitSize)
	{
		double dRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dRet = m_pHWDeviceOEMPA->GetTimeSlotMinimum(lAscanPointCount, eAscanBitSize);
		return dRet;
	}
	double csHWDeviceOEMPA::GetAscanThroughput(double dTimeSlot, long lPointCount, enumBitSize eAscanBitSize)
	{
		double dRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dRet = m_pHWDeviceOEMPA->GetAscanThroughput(dTimeSlot, lPointCount, eAscanBitSize);
		return dRet;
	}
	//FMC SubTimeSlot management
	bool csHWDeviceOEMPA::SetFMCSubTimeSlotAcqReplay(int iCycle,double dAscanStart,double dAscanRange,double %dTimeSlot)
	{
		double dTimeSlot2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dTimeSlot2 = dTimeSlot;
		bRet = m_pHWDeviceOEMPA->SetFMCSubTimeSlotAcqReplay(iCycle,dAscanStart,dAscanRange,dTimeSlot2);
		dTimeSlot = dTimeSlot2;
		return bRet;
	}
	bool csHWDeviceOEMPA::GetFMCTimeLimitation(double dAscanStart,double dAscanRange,double dTimeSlot,double %dTimeSlotMin,double %dHWAcqSubTimeSlotMin,double %dReplaySubTimeSlotMin,double %dReplaySubTimeSlotOptimizedForThroughput)
	{
		double _dTimeSlotMin,_dHWAcqSubTimeSlotMin,_dReplayAcqSubTimeSlotMin,_dReplayAcqSubTimeSlotOptimizedForThroughput;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bRet = m_pHWDeviceOEMPA->GetFMCTimeLimitation(dAscanStart,dAscanRange,dTimeSlot,_dTimeSlotMin,_dHWAcqSubTimeSlotMin,_dReplayAcqSubTimeSlotMin,_dReplayAcqSubTimeSlotOptimizedForThroughput);
		dTimeSlotMin = _dTimeSlotMin;
		dHWAcqSubTimeSlotMin = _dHWAcqSubTimeSlotMin;
		dReplaySubTimeSlotMin = _dReplayAcqSubTimeSlotMin;
		dReplaySubTimeSlotOptimizedForThroughput = _dReplayAcqSubTimeSlotOptimizedForThroughput;
		return bRet;
	}
	int csHWDeviceOEMPA::GetFMCSubTimeSlotCount()
	{
		if(!m_pHWDeviceOEMPA)
			return 0;
		return m_pHWDeviceOEMPA->GetFMCSubTimeSlotCount();
	}
	bool csHWDeviceOEMPA::SetFMCSubTimeSlotAcq(int iCycle,double %dSubTimeSlot)
	{
		double dSubTimeSlot2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dSubTimeSlot2 = dSubTimeSlot;
		bRet = m_pHWDeviceOEMPA->SetFMCSubTimeSlotAcq(iCycle,dSubTimeSlot2);
		dSubTimeSlot = dSubTimeSlot2;
		return bRet;
	}
	bool csHWDeviceOEMPA::SetFMCSubTimeSlotReplay(int iCycle,double %dSubTimeSlot)
	{
		double dSubTimeSlot2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dSubTimeSlot2 = dSubTimeSlot;
		bRet = m_pHWDeviceOEMPA->SetFMCSubTimeSlotReplay(iCycle,dSubTimeSlot2);
		dSubTimeSlot = dSubTimeSlot2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetFMCSubTimeSlotAcq(int iCycle,double *pdSubTimeSlot)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetFMCSubTimeSlotAcq(iCycle,pdSubTimeSlot);
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetFMCSubTimeSlotReplay(int iCycle,double *pdSubTimeSlot)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetFMCSubTimeSlotReplay(iCycle,pdSubTimeSlot);
	}

	bool csHWDeviceOEMPA::SetAscanAcqIdChannelProbe(int iCycle,WORD %wID)
	{
		WORD wID2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		wID2 = wID;
		bRet = m_pHWDeviceOEMPA->SetAscanAcqIdChannelProbe(iCycle,wID2);
		wID = wID2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetAscanAcqIdChannelProbe(int iCycle,WORD *pwID)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetAscanAcqIdChannelProbe(iCycle,pwID);
	}
		
	bool csHWDeviceOEMPA::SetAscanAcqIdChannelScan(int iCycle,WORD %wID)
	{
		WORD wID2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		wID2 = wID;
		bRet = m_pHWDeviceOEMPA->SetAscanAcqIdChannelScan(iCycle,wID2);
		wID = wID2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetAscanAcqIdChannelScan(int iCycle,WORD *pwID)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetAscanAcqIdChannelScan(iCycle,pwID);
	}
		
	bool csHWDeviceOEMPA::SetAscanAcqIdChannelCycle(int iCycle,WORD %wID)
	{
		WORD wID2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		wID2 = wID;
		bRet = m_pHWDeviceOEMPA->SetAscanAcqIdChannelCycle(iCycle,wID2);
		wID = wID2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetAscanAcqIdChannelCycle(int iCycle,WORD *pwID)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetAscanAcqIdChannelCycle(iCycle,pwID);
	}
		
	bool csHWDeviceOEMPA::EnableAscanMaximum(int iCycle,bool %bEnable)
	{
		bool bEnable2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bEnable2 = bEnable;
		bRet = m_pHWDeviceOEMPA->EnableAscanMaximum(iCycle,bEnable2);
		bEnable = bEnable2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEnableAscanMaximum(int iCycle,bool *pbEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableAscanMaximum(iCycle,pbEnable);
	}
		
	bool csHWDeviceOEMPA::EnableAscanMinimum(int iCycle,bool %bEnable)
	{
		bool bEnable2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bEnable2 = bEnable;
		bRet = m_pHWDeviceOEMPA->EnableAscanMinimum(iCycle,bEnable2);
		bEnable = bEnable2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEnableAscanMinimum(int iCycle,bool *pbEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableAscanMinimum(iCycle,pbEnable);
	}
		
	bool csHWDeviceOEMPA::EnableAscanSaturation(int iCycle,bool %bEnable)
	{
		bool bEnable2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bEnable2 = bEnable;
		bRet = m_pHWDeviceOEMPA->EnableAscanSaturation(iCycle,bEnable2);
		bEnable = bEnable2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEnableAscanSaturation(int iCycle,bool *pbEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableAscanSaturation(iCycle,pbEnable);
	}
		
	bool csHWDeviceOEMPA::SetGateModeThreshold(int iCycle,int iGate,bool %bEnable,csEnumGateModeAmp %eGateModeAmp,csEnumGateModeTof %eGateModeTof,csEnumRectification %eGateRectification,double %dThresholdPercent)
	{
		bool bEnable2;
		double dThresholdPercent2;
		enumGateModeAmp eGateModeAmp2;
		enumGateModeTof eGateModeTof2;
		enumRectification eGateRectification2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bEnable2 = bEnable;
		dThresholdPercent2 = dThresholdPercent;
		eGateModeAmp2 = (enumGateModeAmp)eGateModeAmp;
		eGateModeTof2 = (enumGateModeTof)eGateModeTof;
		eGateRectification2 = (enumRectification)eGateRectification;
		bRet = m_pHWDeviceOEMPA->SetGateModeThreshold(iCycle,iGate,bEnable2,eGateModeAmp2,eGateModeTof2,eGateRectification2,dThresholdPercent2);
		bEnable = bEnable2;
		dThresholdPercent = dThresholdPercent2;
		eGateModeAmp = (csEnumGateModeAmp)eGateModeAmp2;
		eGateModeTof = (csEnumGateModeTof)eGateModeTof2;
		eGateRectification = (csEnumRectification)eGateRectification2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetGateModeThreshold(int iCycle,int iGate,bool *pbEnable,csEnumGateModeAmp *peGateModeAmp,csEnumGateModeTof *peGateModeTof,csEnumRectification *peGateRectification,double *pdThresholdPercent)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetGateModeThreshold(iCycle,iGate,pbEnable,(enumGateModeAmp*)peGateModeAmp,(enumGateModeTof*)peGateModeTof,(enumRectification*)peGateRectification,pdThresholdPercent);
	}
	bool csHWDeviceOEMPA::CheckGateModeThreshold(bool %bEnable,csEnumGateModeAmp %eGateModeAmp,csEnumGateModeTof %eGateModeTof,csEnumRectification %eGateRectification,double %dThresholdPercent)
	{
		bool bEnable2;
		double dThresholdPercent2;
		enumGateModeAmp eGateModeAmp2;
		enumGateModeTof eGateModeTof2;
		enumRectification eGateRectification2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		bEnable2 = bEnable;
		dThresholdPercent2 = dThresholdPercent;
		eGateModeAmp2 = (enumGateModeAmp)eGateModeAmp;
		eGateModeTof2 = (enumGateModeTof)eGateModeTof;
		eGateRectification2 = (enumRectification)eGateRectification;
		bRet = m_pHWDeviceOEMPA->CheckGateModeThreshold(bEnable2,eGateModeAmp2,eGateModeTof2,eGateRectification2,dThresholdPercent2);
		bEnable = bEnable2;
		dThresholdPercent = dThresholdPercent2;
		eGateModeAmp = (csEnumGateModeAmp)eGateModeAmp2;
		eGateModeTof = (csEnumGateModeTof)eGateModeTof2;
		eGateRectification = (csEnumRectification)eGateRectification2;
		return bRet;
	}
		
	bool csHWDeviceOEMPA::SetGateStart(int iCycle,int iGate,double %dStart)
	{
		double dStart2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dStart2 = dStart;
		bRet = m_pHWDeviceOEMPA->SetGateStart(iCycle,iGate,dStart2);
		dStart = dStart2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetGateStart(int iCycle,int iGate,double *pdStart)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetGateStart(iCycle,iGate,pdStart);
	}
	bool csHWDeviceOEMPA::CheckGateStart(double %dStart)
	{
		double dStart2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dStart2 = dStart;
		bRet = m_pHWDeviceOEMPA->CheckGateStart(dStart2);
		dStart = dStart2;
		return bRet;
	}
		
	bool csHWDeviceOEMPA::CheckGateStartStop(double %dStart,double %dStop)
	{
		double dStart2;
		double dStop2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dStop2 = dStop;
		dStart2 = dStart;
		bRet = m_pHWDeviceOEMPA->CheckGateStartStop(dStart2,dStop2);
		dStart = dStart2;
		dStop = dStop2;
		return bRet;
	}
	bool csHWDeviceOEMPA::SetGateStop(int iCycle,int iGate,double %dStop)
	{
		double dStop2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dStop2 = dStop;
		bRet = m_pHWDeviceOEMPA->SetGateStop(iCycle,iGate,dStop2);
		dStop = dStop2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetGateStop(int iCycle,int iGate,double *pdStop)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetGateStop(iCycle,iGate,pdStop);
	}
	bool csHWDeviceOEMPA::CheckGateStop(double %dStop)
	{
		double dStop2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dStop2 = dStop;
		bRet = m_pHWDeviceOEMPA->CheckGateStop(dStop2);
		dStop = dStop2;
		return bRet;
	}
		
	bool csHWDeviceOEMPA::SetGateAcqIDAmp(int iDriverId,int iCycle,WORD %wID)
	{
		WORD wID2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		wID2 = wID;
		bRet = m_pHWDeviceOEMPA->SetGateAcqIDAmp(iDriverId,iCycle,wID2);
		wID = wID2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetGateAcqIDAmp(int iDriverId,int iCycle,WORD *pwID)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetGateAcqIDAmp(iDriverId,iCycle,pwID);
	}
	bool csHWDeviceOEMPA::SetGateAcqIDTof(int iDriverId,int iCycle,WORD %wID)
	{
		WORD wID2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		wID2 = wID;
		bRet = m_pHWDeviceOEMPA->SetGateAcqIDTof(iDriverId,iCycle,wID2);
		wID = wID2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetGateAcqIDTof(int iDriverId,int iCycle,WORD *pwID)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetGateAcqIDTof(iDriverId,iCycle,pwID);
	}

	bool csHWDeviceOEMPA::SetTrackingGateStart(int iCycle,int iGate,bool bEnable,int %iTrackingCycleIndex,int %iTrackingGateIndex)
	{
		int iTrackingCycleIndex2,iTrackingGateIndex2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iTrackingCycleIndex2 = iTrackingCycleIndex;
		iTrackingGateIndex2 = iTrackingGateIndex;
		bRet = m_pHWDeviceOEMPA->SetTrackingGateStart(iCycle,iGate,bEnable,iTrackingCycleIndex2,iTrackingGateIndex2);
		iTrackingCycleIndex = iTrackingCycleIndex2;
		iTrackingGateIndex = iTrackingGateIndex2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetTrackingGateStart(int iCycle,int iGate,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetTrackingGateStart(iCycle,iGate,pbEnable,piTrackingCycleIndex,piTrackingGateIndex);
	}
	bool csHWDeviceOEMPA::SetTrackingGateStop(int iCycle,int iGate,bool bEnable,int %iTrackingCycleIndex,int %iTrackingGateIndex)
	{
		int iTrackingCycleIndex2,iTrackingGateIndex2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iTrackingCycleIndex2 = iTrackingCycleIndex;
		iTrackingGateIndex2 = iTrackingGateIndex;
		bRet = m_pHWDeviceOEMPA->SetTrackingGateStop(iCycle,iGate,bEnable,iTrackingCycleIndex2,iTrackingGateIndex2);
		iTrackingCycleIndex = iTrackingCycleIndex2;
		iTrackingGateIndex = iTrackingGateIndex2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetTrackingGateStop(int iCycle,int iGate,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetTrackingGateStop(iCycle,iGate,pbEnable,piTrackingCycleIndex,piTrackingGateIndex);
	}
	bool csHWDeviceOEMPA::SetTrackingAscan(int iCycle,bool bEnable,int %iTrackingCycleIndex,int %iTrackingGateIndex)
	{
		int iTrackingCycleIndex2,iTrackingGateIndex2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iTrackingCycleIndex2 = iTrackingCycleIndex;
		iTrackingGateIndex2 = iTrackingGateIndex;
		bRet = m_pHWDeviceOEMPA->SetTrackingAscan(iCycle,bEnable,iTrackingCycleIndex2,iTrackingGateIndex2);
		iTrackingCycleIndex = iTrackingCycleIndex2;
		iTrackingGateIndex = iTrackingGateIndex2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetTrackingAscan(int iCycle,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetTrackingAscan(iCycle,pbEnable,piTrackingCycleIndex,piTrackingGateIndex);
	}
	bool csHWDeviceOEMPA::SetTrackingDac(int iCycle,bool bEnable,int %iTrackingCycleIndex,int %iTrackingGateIndex)
	{
		int iTrackingCycleIndex2,iTrackingGateIndex2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iTrackingCycleIndex2 = iTrackingCycleIndex;
		iTrackingGateIndex2 = iTrackingGateIndex;
		bRet = m_pHWDeviceOEMPA->SetTrackingDac(iCycle,bEnable,iTrackingCycleIndex2,iTrackingGateIndex2);
		iTrackingCycleIndex = iTrackingCycleIndex2;
		iTrackingGateIndex = iTrackingGateIndex2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetTrackingDac(int iCycle,bool *pbEnable,int *piTrackingCycleIndex,int *piTrackingGateIndex)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetTrackingDac(iCycle,pbEnable,piTrackingCycleIndex,piTrackingGateIndex);
	}
	bool csHWDeviceOEMPA::CheckTracking(bool bEnable,int %iTrackingCycleIndex,int %iTrackingGateIndex)
	{
		int iTrackingCycleIndex2,iTrackingGateIndex2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		iTrackingCycleIndex2 = iTrackingCycleIndex;
		iTrackingGateIndex2 = iTrackingGateIndex;
		bRet = m_pHWDeviceOEMPA->CheckTracking(bEnable,iTrackingCycleIndex2,iTrackingGateIndex2);
		iTrackingCycleIndex = iTrackingCycleIndex2;
		iTrackingGateIndex = iTrackingGateIndex2;
		return bRet;
	}
	bool csHWDeviceOEMPA::ResetTrackingTable()//this is called automatically when the IF tracking of one gate is updated.
	{
		return m_pHWDeviceOEMPA->ResetTrackingTable();
	}

	bool csHWDeviceOEMPA::SetGainAnalog(int iCycle,float %fGain)
	{
		float fGain2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		fGain2 = fGain;
		bRet = m_pHWDeviceOEMPA->SetGainAnalog(iCycle,fGain2);
		fGain = fGain2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetGainAnalog(int iCycle,float *pfGain)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetGainAnalog(iCycle,pfGain);
	}
	bool csHWDeviceOEMPA::CheckGainAnalog(float %fGain)
	{
		float fGain2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		fGain2 = fGain;
		bRet = m_pHWDeviceOEMPA->CheckGainAnalog(fGain2);
		fGain = fGain2;
		return bRet;
	}

	bool csHWDeviceOEMPA::SetEmissionWedgeDelay(int iCycle,int iCycleCount,double %dWedgeDelay)
	{
		double dWedgeDelay2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dWedgeDelay2 = dWedgeDelay;
		bRet = m_pHWDeviceOEMPA->SetEmissionWedgeDelay(iCycle,iCycleCount,dWedgeDelay2);
		dWedgeDelay = dWedgeDelay2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEmissionWedgeDelay(int iCycle,int iCountMax,double *pdWedgeDelay)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEmissionWedgeDelay(iCycle,iCountMax,pdWedgeDelay);
	}
	bool csHWDeviceOEMPA::SetReceptionWedgeDelay(int iCycle,int iCycleCount,double %dWedgeDelay)
	{
		double dWedgeDelay2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dWedgeDelay2 = dWedgeDelay;
		bRet = m_pHWDeviceOEMPA->SetReceptionWedgeDelay(iCycle,iCycleCount,dWedgeDelay2);
		dWedgeDelay = dWedgeDelay2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetReceptionWedgeDelay(int iCycle,int iCountMax,double *pdWedgeDelay)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetReceptionWedgeDelay(iCycle,iCountMax,pdWedgeDelay);
	}
	bool csHWDeviceOEMPA::CheckWedgeDelay(double %dWedgeDelay)
	{
		double dWedgeDelay2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dWedgeDelay2 = dWedgeDelay;
		bRet = m_pHWDeviceOEMPA->CheckWedgeDelay(dWedgeDelay2);
		dWedgeDelay = dWedgeDelay2;
		return bRet;
	}

	bool csHWDeviceOEMPA::SetAllElementEnable(bool bEnable,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD adwHWAperture2[g_iOEMPAApertureDWordCount];
		bool bRet;

		if((adwHWAperture==nullptr) || (adwHWAperture->Length!=g_iOEMPAApertureDWordCount))
			adwHWAperture = gcnew array<DWORD>(g_iOEMPAApertureDWordCount);
		bRet = CHWDeviceOEMPA::SetAllElementEnable(bEnable,adwHWAperture2);
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture[i] = adwHWAperture2[i];
		return bRet;
	}
	bool csHWDeviceOEMPA::SetElementEnable(int iElement,bool bEnable,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD adwHWAperture2[g_iOEMPAApertureDWordCount];
		bool bRet;

		if((adwHWAperture==nullptr) || (adwHWAperture->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture2[i] = adwHWAperture[i];
		bRet = CHWDeviceOEMPA::SetElementEnable(iElement,bEnable,adwHWAperture2);
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture[i] = adwHWAperture2[i];
		return bRet;
	}
	bool csHWDeviceOEMPA::GetElementEnable(int iElement,/*const*/ array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/,bool %bEnable)
	{
		DWORD adwHWAperture2[g_iOEMPAApertureDWordCount];
		bool bEnable2;
		bool bRet;

		if((adwHWAperture==nullptr) || (adwHWAperture->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture2[i] = adwHWAperture[i];
		bRet = CHWDeviceOEMPA::GetElementEnable(iElement,adwHWAperture2,bEnable2);
		bEnable = bEnable2;
		return bRet;
	}

	bool csHWDeviceOEMPA::IsMultiplexer()//this function can be used to know if a multiplexer (16:128 or 32:128) is included in the system.
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->IsMultiplexer();
	}
	//Case of a system with a multiplexer (16:128 or 32:128), please use following functions:
	//	in this case same aperture is used for emission and reception
	bool csHWDeviceOEMPA::SetMultiplexerEnable(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD adwHWAperture2[g_iOEMPAApertureDWordCount];
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adwHWAperture==nullptr) || (adwHWAperture->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture2[i] = adwHWAperture[i];
		bRet = m_pHWDeviceOEMPA->SetMultiplexerEnable(iCycle,adwHWAperture2);
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture[i] = adwHWAperture2[i];
		return bRet;
	}
	bool csHWDeviceOEMPA::GetMultiplexerEnable(int iCycle,/*fixed*/[Out] array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD *pwValue;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		adwHWAperture = gcnew array<DWORD>(g_iOEMPAApertureDWordCount);
		pwValue = (DWORD*)(void*)ListAddObject(adwHWAperture);
		bRet = m_pHWDeviceOEMPA->GetMultiplexerEnable(iCycle,pwValue);
		return bRet;
	}
	bool csHWDeviceOEMPA::SetMultiplexerEnable(int iCycle,array<DWORD>^ %adwHWApertureEmission/*[g_iOEMPAApertureDWordCount]*/,array<DWORD>^ %adwHWApertureReception/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD adwHWApertureE2[g_iOEMPAApertureDWordCount];
		DWORD adwHWApertureR2[g_iOEMPAApertureDWordCount];
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adwHWApertureEmission==nullptr) || (adwHWApertureEmission->Length!=g_iOEMPAApertureDWordCount))
			return false;
		if((adwHWApertureReception==nullptr) || (adwHWApertureReception->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
		{
			adwHWApertureE2[i] = adwHWApertureEmission[i];
			adwHWApertureR2[i] = adwHWApertureReception[i];
		}
		bRet = m_pHWDeviceOEMPA->SetMultiplexerEnable(iCycle,adwHWApertureE2,adwHWApertureR2);
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
		{
			adwHWApertureEmission[i] = adwHWApertureE2[i];
			adwHWApertureReception[i] = adwHWApertureR2[i];
		}
		return bRet;
	}
	bool csHWDeviceOEMPA::GetMultiplexerEnable(int iCycle,/*fixed*/[Out] array<DWORD>^ %adwHWApertureEmission/*[g_iOEMPAApertureDWordCount]*/,/*fixed*/[Out] array<DWORD>^ %adwHWApertureReception/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD *pwValueE;
		DWORD *pwValueR;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		adwHWApertureEmission = gcnew array<DWORD>(g_iOEMPAApertureDWordCount);
		if((adwHWApertureEmission==nullptr) || (adwHWApertureEmission->Length!=g_iOEMPAApertureDWordCount))
			return false;
		adwHWApertureReception = gcnew array<DWORD>(g_iOEMPAApertureDWordCount);
		if((adwHWApertureReception==nullptr) || (adwHWApertureReception->Length!=g_iOEMPAApertureDWordCount))
			return false;
		pwValueE = (DWORD*)(void*)ListAddObject(adwHWApertureEmission);
		pwValueR = (DWORD*)(void*)ListAddObject(adwHWApertureReception);
		bRet = m_pHWDeviceOEMPA->GetMultiplexerEnable(iCycle,pwValueE,pwValueR);
		return bRet;
	}
	bool csHWDeviceOEMPA::CheckMultiplexerAperture(array<DWORD>^ %adwHWApertureEmission/*[g_iOEMPAApertureDWordCount]*/,array<DWORD>^ % adwHWApertureReception/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD adwHWApertureE2[g_iOEMPAApertureDWordCount];
		DWORD adwHWApertureR2[g_iOEMPAApertureDWordCount];
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adwHWApertureEmission==nullptr) || (adwHWApertureEmission->Length!=g_iOEMPAApertureDWordCount))
			return false;
		if((adwHWApertureReception==nullptr) || (adwHWApertureReception->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
		{
			adwHWApertureE2[i] = adwHWApertureEmission[i];
			adwHWApertureR2[i] = adwHWApertureReception[i];
		}
		bRet = m_pHWDeviceOEMPA->CheckMultiplexerAperture(adwHWApertureE2,adwHWApertureR2);
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
		{
			adwHWApertureEmission[i] = adwHWApertureE2[i];
			adwHWApertureReception[i] = adwHWApertureR2[i];
		}
		return bRet;
	}
	//Case of a system without a multiplexer (16:128 or 32:128), please use following functions:
	bool csHWDeviceOEMPA::SetEmissionEnable(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD adwHWAperture2[g_iOEMPAApertureDWordCount];
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adwHWAperture==nullptr) || (adwHWAperture->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture2[i] = adwHWAperture[i];
		bRet = m_pHWDeviceOEMPA->SetEmissionEnable(iCycle,adwHWAperture2);
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture[i] = adwHWAperture2[i];
		return bRet;
	}
	bool csHWDeviceOEMPA::GetEmissionEnable(int iCycle,/*fixed*/[Out] array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD *pwValue;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		adwHWAperture = gcnew array<DWORD>(g_iOEMPAApertureDWordCount);
		pwValue = (DWORD*)(void*)ListAddObject(adwHWAperture);
		bRet = m_pHWDeviceOEMPA->GetEmissionEnable(iCycle,pwValue);
		return bRet;
	}
	bool csHWDeviceOEMPA::SetReceptionEnable(int iCycle,array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD adwHWAperture2[g_iOEMPAApertureDWordCount];
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adwHWAperture==nullptr) || (adwHWAperture->Length!=g_iOEMPAApertureDWordCount))
			return false;
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture2[i] = adwHWAperture[i];
		bRet = m_pHWDeviceOEMPA->SetReceptionEnable(iCycle,adwHWAperture2);
		for(int i=0;i<g_iOEMPAApertureDWordCount;i++)
			adwHWAperture[i] = adwHWAperture2[i];
		return bRet;
	}
	bool csHWDeviceOEMPA::GetReceptionEnable(int iCycle,/*fixed*/[Out] array<DWORD>^ %adwHWAperture/*[g_iOEMPAApertureDWordCount]*/)
	{
		DWORD *pwValue;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		adwHWAperture = gcnew array<DWORD>(g_iOEMPAApertureDWordCount);
		pwValue = (DWORD*)(void*)ListAddObject(adwHWAperture);
		bRet = m_pHWDeviceOEMPA->GetReceptionEnable(iCycle,pwValue);
		return bRet;
	}

	bool csHWDeviceOEMPA::SetReceptionFocusing(int iCycle,acsDouble^ %adFocalTof)
	{
		structCallbackArrayDouble1D callback;
		bool bRet;
		int iFocalCount;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adFocalTof!=nullptr) && (adFocalTof->list!=nullptr) && (adFocalTof->list->Length>g_iOEMPAFocalCountMax))
			return false;
		if((adFocalTof==nullptr) || (adFocalTof->list==nullptr))
		{
			iFocalCount = 0;
			bRet = m_pHWDeviceOEMPA->SetReceptionFocusing(iCycle,iFocalCount,NULL);
			return bRet;
		}
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)adFocalTof->GetGcroot();//pdac;
		callback.pSetSize = gCallbackSetSizeDouble;
		callback.pSetData = gCallbackSetDataDouble;
		callback.pGetSize = gCallbackGetSizeDouble;
		callback.pGetData = gCallbackGetDataDouble;
		bRet = m_pHWDeviceOEMPA->SetReceptionFocusing(iCycle,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::GetReceptionFocusing(int iCycle,/*fixed*/[Out] acsDouble^ %adFocalTof)
	{
		structCallbackArrayDouble1D callback;
		int iMax=g_iOEMPAApertureElementCountMax;
		bool bRet;
		gcroot<acsDouble^> *pTof;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(adFocalTof==nullptr)
			adFocalTof = gcnew acsDouble;
		pTof = adFocalTof->GetGcroot();
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)pTof;
		callback.pSetSize = gCallbackSetSizeDouble;
		callback.pSetData = gCallbackSetDataDouble;
		callback.pGetSize = gCallbackGetSizeDouble;
		callback.pGetData = gCallbackGetDataDouble;
		bRet = m_pHWDeviceOEMPA->GetReceptionFocusing(iCycle,iMax,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::SetReceptionFocusing(int iCycle,acsDouble^ %adFocalTof,float %fCenterDelayE,float %fCenterDelayR)
	{
		structCallbackArrayDouble1D callback;
		bool bRet;
		int iFocalCount;
		float _fCenterDelayE,_fCenterDelayR;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((adFocalTof!=nullptr) && (adFocalTof->list!=nullptr) && (adFocalTof->list->Length>g_iOEMPAFocalCountMax))
			return false;
		if((adFocalTof==nullptr) || (adFocalTof->list==nullptr))
		{
			iFocalCount = 0;
			bRet = m_pHWDeviceOEMPA->SetReceptionFocusing(iCycle,iFocalCount,NULL);
			return bRet;
		}
		_fCenterDelayE = fCenterDelayE;
		_fCenterDelayR = fCenterDelayR;
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)adFocalTof->GetGcroot();//pdac;
		callback.pSetSize = gCallbackSetSizeDouble;
		callback.pSetData = gCallbackSetDataDouble;
		callback.pGetSize = gCallbackGetSizeDouble;
		callback.pGetData = gCallbackGetDataDouble;
		bRet = m_pHWDeviceOEMPA->SetReceptionFocusing(iCycle,callback,_fCenterDelayE,_fCenterDelayR);
		fCenterDelayE = _fCenterDelayE;
		fCenterDelayR = _fCenterDelayR;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetReceptionFocusing(int iCycle,/*fixed*/[Out] acsDouble^ %adFocalTof,[Out] float *pfCenterDelayE,[Out] float *pfCenterDelayR)
	{
		structCallbackArrayDouble1D callback;
		int iMax=g_iOEMPAApertureElementCountMax;
		bool bRet;
		gcroot<acsDouble^> *pTof;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(adFocalTof==nullptr)
			adFocalTof = gcnew acsDouble;
		pTof = adFocalTof->GetGcroot();
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)pTof;
		callback.pSetSize = gCallbackSetSizeDouble;
		callback.pSetData = gCallbackSetDataDouble;
		callback.pGetSize = gCallbackGetSizeDouble;
		callback.pGetData = gCallbackGetDataDouble;
		bRet = m_pHWDeviceOEMPA->GetReceptionFocusing(iCycle,iMax,callback,pfCenterDelayE,pfCenterDelayR);
		return bRet;
	}
	bool csHWDeviceOEMPA::EnableDDF(int iCycle,bool %bEnable)
	{
		bool bEnable2;
		bool bRet;
		//int %iFocalCount,double *pdFocalTof

		if(!m_pHWDeviceOEMPA)
			return false;
		bEnable2 = bEnable;
		bRet = m_pHWDeviceOEMPA->EnableDDF(iCycle,bEnable2);
		bEnable = bEnable2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetEnableDDF(int iCycle,bool *pbEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableDDF(iCycle,pbEnable);
	}
	bool csHWDeviceOEMPA::EnableDDF(int iCycle,csEnumFocusing %eFocusing)
	{
		enumFocusing eFocusing2;
		bool bRet;
		//int %iFocalCount,double *pdFocalTof

		if(!m_pHWDeviceOEMPA)
			return false;
		eFocusing2 = (enumFocusing)eFocusing;
		bRet = m_pHWDeviceOEMPA->EnableDDF(iCycle,eFocusing2);
		eFocusing = (csEnumFocusing)eFocusing2;
		return bRet;
	}
	bool csHWDeviceOEMPA::GetEnableDDF(int iCycle,csEnumFocusing *peFocusing)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableDDF(iCycle,(enumFocusing*)peFocusing);
	}
	void csHWDeviceOEMPA::SetDDFTimeOfFlightMiddle(bool bEnable)
	{
		CHWDeviceOEMPA::SetDDFTimeOfFlightMiddle(bEnable);
	}
	bool csHWDeviceOEMPA::IsDDFTimeOfFlightMiddle()
	{
		return CHWDeviceOEMPA::IsDDFTimeOfFlightMiddle();
	}
	void csHWDeviceOEMPA::SetDDFWaveTrackingCorrection(int iEnable)
	{
		CHWDeviceOEMPA::SetDDFWaveTrackingCorrection(iEnable);
	}
	int csHWDeviceOEMPA::GetDDFWaveTrackingCorrection()
	{
		return CHWDeviceOEMPA::GetDDFWaveTrackingCorrection();
	}
	void csHWDeviceOEMPA::SetFMCReceptionSimplified(bool bEnable)
	{
		CHWDeviceOEMPA::SetFMCReceptionSimplified(bEnable);
	}
	bool csHWDeviceOEMPA::IsFMCReceptionSimplified()
	{
		return CHWDeviceOEMPA::IsFMCReceptionSimplified();
	}
	bool csHWDeviceOEMPA::CheckFocalTimeOfFlight(double %dDelay)
	{
		double dDelay2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dDelay2 = dDelay;
		bRet = m_pHWDeviceOEMPA->CheckFocalTimeOfFlight(dDelay2);
		dDelay = dDelay2;
		return bRet;
	}
	bool csHWDeviceOEMPA::CheckEmissionWidth(float %fWidth)
	{
		float fWidth2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		fWidth2 = fWidth;
		bRet = m_pHWDeviceOEMPA->CheckEmissionWidth(fWidth2);
		fWidth = fWidth2;
		return bRet;
	}
	bool csHWDeviceOEMPA::CheckReceptionGain(float %fGain)
	{
		float fGain2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		fGain2 = fGain;
		bRet = m_pHWDeviceOEMPA->CheckReceptionGain(fGain2);
		fGain = fGain2;
		return bRet;
	}
	bool csHWDeviceOEMPA::CheckEmissionDelay(float %fDelay)
	{
		float fDelay2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		fDelay2 = fDelay;
		bRet = m_pHWDeviceOEMPA->CheckEmissionDelay(fDelay2);
		fDelay = fDelay2;
		return bRet;
	}
	bool csHWDeviceOEMPA::CheckReceptionDelay(float %fDelay)
	{
		float fDelay2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		fDelay2 = fDelay;
		bRet = m_pHWDeviceOEMPA->CheckReceptionDelay(fDelay2);
		fDelay = fDelay2;
		return bRet;
	}
	DWORD csHWDeviceOEMPA::GetSWBaseAddress()
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetSWBaseAddress();
	}

	bool csHWDeviceOEMPA::EnableMultiHWChannelAcquisition(int iCycle,int iCycleCount,bool bEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->EnableMultiHWChannelAcquisition(iCycle,iCycleCount,bEnable);
	}
	bool csHWDeviceOEMPA::GetEnableMultiHWChannelAcquisition(int iCycle,bool *pbEnable)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetEnableMultiHWChannelAcquisition(iCycle,pbEnable);
	}
	bool csHWDeviceOEMPA::SetMultiHWChannelAcqDecimation(int iCycle,acsByte^ %abyData)
	{
		unionOEMMCDecimation decimation;
		unionDecimation *pX;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if((abyData==nullptr) || (abyData->list==nullptr) || (abyData->list->Length>g_iOEMPAApertureElementCountMax))
			return false;
		memset(&decimation,0,sizeof(decimation));
		for(int iIndex=0;iIndex<abyData->list->Length;iIndex++)
		{
			pX = (unionDecimation*)&decimation.byte[iIndex/2];
			if(!(iIndex%2))
				pX->data.lsb = abyData->list[iIndex];
			else
				pX->data.msb = abyData->list[iIndex];
		}
		bRet = m_pHWDeviceOEMPA->SetMultiHWChannelAcqDecimation(iCycle,decimation);
		return bRet;
	}
	bool csHWDeviceOEMPA::GetMultiHWChannelAcqDecimation(int iCycle,[Out] acsByte^ %abyData)
	{
		structCallbackArrayByte1D callback;
		gcroot<acsByte^> *pByte;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(abyData==nullptr)
			abyData = gcnew acsByte;
		pByte = abyData->GetGcroot();
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)pByte;
		callback.pSetSize = gCallbackSetSizeByte1D;
		callback.pSetData = gCallbackSetDataByte1D;
		callback.pGetSize = gCallbackGetSizeByte1D;
		callback.pGetData = gCallbackGetDataByte1D;
		return m_pHWDeviceOEMPA->GetMultiHWChannelAcqDecimation(iCycle,callback);
	}
	bool csHWDeviceOEMPA::SetMultiHWChannelAcqWriteStart(int iCycle,int iAcqElement,int iStartCount,acsFloat^ %afStart/*float *pfStart*/)
	{
		structCallbackArrayFloat1D callback;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(afStart==nullptr)
			return false;
		if(afStart->list==nullptr)
			return false;
		if(afStart->list->Length>g_iOEMPAApertureElementCountMax)
			return false;
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)afStart->GetGcroot();
		callback.pSetSize = gCallbackSetSizeFloat;
		callback.pSetData = gCallbackSetDataFloat;
		callback.pGetSize = gCallbackGetSizeFloat;
		callback.pGetData = gCallbackGetDataFloat;
		bRet = m_pHWDeviceOEMPA->SetMultiHWChannelAcqWriteStart(iCycle,iAcqElement,iStartCount,callback);
		return bRet;
	}
	bool csHWDeviceOEMPA::GetMultiHWChannelAcqWriteStart(int iCycle,/*fixed*/[Out] acsFloat^ %afStart/*int &iStartCountMax,int *piStartCount,float *pfStart*/)
	{
		structCallbackArrayFloat1D callback;
		int iMax=g_iOEMPAApertureElementCountMax;
		bool bRet;
		gcroot<acsFloat^> *pStart;

		if(!m_pHWDeviceOEMPA)
			return false;
		if(afStart==nullptr)
			afStart = gcnew acsFloat;
		iMax = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetElementCountMax();//1.1.5.2h
		pStart = afStart->GetGcroot();
		callback.apParameter[0] = m_pointer;
		callback.apParameter[1] = (void*)pStart;
		callback.pSetSize = gCallbackSetSizeFloat;
		callback.pSetData = gCallbackSetDataFloat;
		callback.pGetSize = gCallbackGetSizeFloat;
		callback.pGetData = gCallbackGetDataFloat;
		bRet = m_pHWDeviceOEMPA->GetMultiHWChannelAcqWriteStart(iCycle,iMax,callback);
		return bRet;
	}
	double csHWDeviceOEMPA::GetMultiHWChannelRangeMax()
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetMultiHWChannelRangeMax();
	}
	double csHWDeviceOEMPA::GetFWAscanRecoveryTime()
	{
		if(!m_pHWDeviceOEMPA || !m_pHWDeviceOEMPA->GetSWDeviceOEMPA())
			return false;
		return m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFWAscanRecoveryTime();
	}

	bool csHWDeviceOEMPA::SetSettingId(DWORD dwSettingId)
	{
		DWORD dwSettingId2;
		bool bRet;

		if(!m_pHWDeviceOEMPA)
			return false;
		dwSettingId2 = dwSettingId;
		bRet = m_pHWDeviceOEMPA->SetSettingId(dwSettingId2);
		dwSettingId = dwSettingId2;
		return bRet;
	}
	/*unsafe*/bool csHWDeviceOEMPA::GetSettingId(DWORD *pdwSettingId)
	{
		if(!m_pHWDeviceOEMPA)
			return 0;
		return m_pHWDeviceOEMPA->GetSettingId(pdwSettingId);
	}
	DWORD csHWDeviceOEMPA::swGetSettingId()
	{
		if(!m_pHWDeviceOEMPA)
			return 0;
		return m_pHWDeviceOEMPA->swGetSettingId();
	}
	bool csHWDeviceOEMPA::PerformCalibration(float &fDelayMax,float &fCorrectionOffset)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->PerformCalibration(fDelayMax,fCorrectionOffset);
	}
	bool csHWDeviceOEMPA::UpdateCalibration()
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->UpdateCalibration();
	}
	bool csHWDeviceOEMPA::SetTimeOffset(float &fTime)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->SetTimeOffset(fTime);
	}
	bool csHWDeviceOEMPA::GetTimeOffset(float *pfTime)
	{
		if(!m_pHWDeviceOEMPA)
			return false;
		return m_pHWDeviceOEMPA->GetTimeOffset(pfTime);
	}

#pragma endregion csHWDeviceOEMPA
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

}


#pragma region callback
	UINT WINAPI gAcquisitionAscan_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pAcquisitionParameter);
		if(!pointer)
			return 1;
		return (*pointer)->AcquisitionAscan_0x00010103(acqInfo,pStreamHeader,pAscanHeader,pBufferMax,pBufferMin,pBufferSat);
	}
	UINT WINAPI gAcquisitionCscan_0x00010X02(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pAcquisitionParameter);
		if(!pointer)
			return 1;
		return (*pointer)->AcquisitionCscan_0x00010X02(acqInfo,pStreamHeader,pCscanHeader,pBufferAmp,pBufferAmpTof);
	}
	UINT WINAPI gAcquisitionIO_0x00010101(void *pAcquisitionParameter,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pAcquisitionParameter);
		if(!pointer)
			return 1;
		return (*pointer)->AcquisitionIO_0x00010101(pStreamHeader,pIOHeader);
	}
	UINT WINAPI gAcquisitionIO_1x00010101(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pAcquisitionParameter);
		if(!pointer)
			return 1;
		return (*pointer)->AcquisitionIO_1x00010101(acqInfo,pStreamHeader,pIOHeader);
	}
	UINT WINAPI gAcquisitionInfo(void *pAcquisitionParameter,const wchar_t *pInfo)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pAcquisitionParameter);
		if(!pointer)
			return 1;
		return (*pointer)->AcquisitionInfo(pInfo);
	}

	structCorrectionOEMPA* WINAPI gCallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
	{
		CHWDeviceOEMPA *pOEMPA=(CHWDeviceOEMPA*)pAcquisitionParameter;
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		void *pAcquisitionParameter2;

		pAcquisitionParameter2 = pOEMPA->GetAcquisitionParameter();
		if(!pAcquisitionParameter2)
			return NULL;
		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pAcquisitionParameter2);
		(*pointer)->CallbackCustomizedAPI(pFileName,eStep,pRoot,pCycle,pEmission,pReception);
		return NULL;//we dont need correction in the current example.
	}

	static gcroot<csDriverOEMPA::csMsgBox^> g_pStaticKernelAPI;
	void WINAPI gCallbackSystemMessageBox(const wchar_t *pMsg)
	{
		g_pStaticKernelAPI->CallbackSystemMessageBox(pMsg);
	}
	void WINAPI gCallbackSystemMessageBoxList(const wchar_t *pMsg)
	{
		g_pStaticKernelAPI->CallbackSystemMessageBoxList(pMsg);
	}
	UINT WINAPI gCallbackSystemMessageBoxButtons(const wchar_t *pMsg,const wchar_t *pTitle,UINT nType)
	{
		return g_pStaticKernelAPI->CallbackSystemMessageBoxButtons(pMsg,pTitle,nType);
	}
	int WINAPI gCallbackOempaApiMessageBox(HWND hWnd,LPCTSTR lpszText,LPCTSTR lpszCaption,UINT nType)
	{
		return g_pStaticKernelAPI->CallbackOempaApiMessageBox(hWnd,lpszText,lpszCaption,nType);
	}

	bool WINAPI gCallbackSetSizeDouble(struct structCallbackArrayDouble1D *pCallbackArray,int iSize)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDouble^>* csDouble;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csDouble = (gcroot<csDriverOEMPA::acsDouble^>*)(pCallbackArray->apParameter[1]);
		if(!csDouble)
			return false;
		(*csDouble)->list = gcnew array<double>(iSize);
		return true;
	}
	bool WINAPI gCallbackSetDataDouble(struct structCallbackArrayDouble1D *pCallbackArray,int iIndex,double fData)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDouble^>* csDouble;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csDouble = (gcroot<csDriverOEMPA::acsDouble^>*)(pCallbackArray->apParameter[1]);
		if(!csDouble || ((*csDouble)->list==nullptr))
			return false;
		if(iIndex<0)
			return false;
		if(iIndex>=(*csDouble)->list->Length)
			return true;
		(*csDouble)->list[iIndex] = fData;
		return true;
	}
	bool WINAPI gCallbackGetSizeDouble(struct structCallbackArrayDouble1D *pCallbackArray,int &iSize)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDouble^>* csDouble;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csDouble = (gcroot<csDriverOEMPA::acsDouble^>*)(pCallbackArray->apParameter[1]);
		if(!csDouble || ((*csDouble)->list==nullptr))
			return false;
		iSize = (*csDouble)->list->Length;
		return true;
	}
	bool WINAPI gCallbackGetDataDouble(struct structCallbackArrayDouble1D *pCallbackArray,int iIndex,double &fData)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDouble^>* csDouble;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csDouble = (gcroot<csDriverOEMPA::acsDouble^>*)(pCallbackArray->apParameter[1]);
		if(!csDouble || ((*csDouble)->list==nullptr))
			return false;
		if(iIndex<0)
			return false;
		if(iIndex>=(*csDouble)->list->Length)
			return true;
		fData = (*csDouble)->list[iIndex];
		return true;
	}

	bool WINAPI gCallbackSetSizeFloat(struct structCallbackArrayFloat1D *pCallbackArray,int iSize)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsFloat^>* csFloat;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csFloat = (gcroot<csDriverOEMPA::acsFloat^>*)(pCallbackArray->apParameter[1]);
		if(!csFloat)
			return false;
		(*csFloat)->list = gcnew array<float>(iSize);
		return true;
	}
	bool WINAPI gCallbackSetDataFloat(struct structCallbackArrayFloat1D *pCallbackArray,int iIndex,float fData)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsFloat^>* csFloat;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csFloat = (gcroot<csDriverOEMPA::acsFloat^>*)(pCallbackArray->apParameter[1]);
		if(!csFloat || ((*csFloat)->list==nullptr))
			return false;
		if(iIndex<0)
			return false;
		if(iIndex>=(*csFloat)->list->Length)
			return true;
		(*csFloat)->list[iIndex] = fData;
		return true;
	}
	bool WINAPI gCallbackGetSizeFloat(struct structCallbackArrayFloat1D *pCallbackArray,int &iSize)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsFloat^>* csFloat;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csFloat = (gcroot<csDriverOEMPA::acsFloat^>*)(pCallbackArray->apParameter[1]);
		if(!csFloat || ((*csFloat)->list==nullptr))
			return false;
		iSize = (*csFloat)->list->Length;
		return true;
	}
	bool WINAPI gCallbackGetDataFloat(struct structCallbackArrayFloat1D *pCallbackArray,int iIndex,float &fData)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsFloat^>* csFloat;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csFloat = (gcroot<csDriverOEMPA::acsFloat^>*)(pCallbackArray->apParameter[1]);
		if(!csFloat || ((*csFloat)->list==nullptr))
			return false;
		if(iIndex<0)
			return false;
		if(iIndex>=(*csFloat)->list->Length)
			return true;
		fData = (*csFloat)->list[iIndex];
		return true;
	}

	bool WINAPI gCallbackSetSizeDelay2(struct structCallbackArrayFloat2D *pCallbackArray,int iSize1,int iSize2)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDelayReception^>* csDelay;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csDelay = (gcroot<csDriverOEMPA::acsDelayReception^>*)(pCallbackArray->apParameter[1]);
		if(!csDelay)
			return false;
		(*csDelay)->list = gcnew array<float,2>(iSize1,iSize2);
		return true;
	}
	bool WINAPI gCallbackSetDataDelay2(struct structCallbackArrayFloat2D *pCallbackArray,int iIndex1,int iIndex2,float fData)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDelayReception^>* csDelay;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csDelay = (gcroot<csDriverOEMPA::acsDelayReception^>*)(pCallbackArray->apParameter[1]);
		if(!csDelay || ((*csDelay)->list==nullptr))
			return false;
		if((*csDelay)->list->Rank!=2)
			return false;
		if(iIndex1<0)
			return false;
		if(iIndex1>=(*csDelay)->list->GetLength(0))
			return true;
		if(iIndex2<0)
			return false;
		if(iIndex2>=(*csDelay)->list->GetLength(1))
			return true;
		(*csDelay)->list[iIndex1,iIndex2] = fData;
		return true;
	}
	bool WINAPI gCallbackGetSizeDelay2(struct structCallbackArrayFloat2D *pCallbackArray,int &iSize1,int &iSize2)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDelayReception^>* csDelay;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csDelay = (gcroot<csDriverOEMPA::acsDelayReception^>*)(pCallbackArray->apParameter[1]);
		if(!csDelay || ((*csDelay)->list==nullptr))
			return false;
		iSize1 = (*csDelay)->list->GetLength(0);
		iSize2 = (*csDelay)->list->GetLength(1);
		return true;
	}
	bool WINAPI gCallbackGetDataDelay2(struct structCallbackArrayFloat2D *pCallbackArray,int iIndex1,int iIndex2,float &fData)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDelayReception^>* csDelay;
		int iSize1,iSize2;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csDelay = (gcroot<csDriverOEMPA::acsDelayReception^>*)(pCallbackArray->apParameter[1]);
		if(!csDelay || ((*csDelay)->list==nullptr))
			return false;
		if((*csDelay)->list->Rank!=2)
			return false;
		iSize1 = (*csDelay)->list->GetLength(0);
		iSize2 = (*csDelay)->list->GetLength(1);
		if(iIndex1<0)
			return false;
		if(iIndex1>=iSize1)
			return true;
		if(iIndex2<0)
			return false;
		if(iIndex2>=iSize2)
			return true;
		fData = (*csDelay)->list[iIndex1,iIndex2];
		return true;
	}

	bool WINAPI gCallbackSetSizeDac(struct structCallbackArrayFloatDac *pCallbackArray,int iSize)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDac^>* dac;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		dac = (gcroot<csDriverOEMPA::acsDac^>*)(pCallbackArray->apParameter[1]);
		if(!dac || !iSize)
			return false;
		(*dac)->list = gcnew array<csDriverOEMPA::csDac^>(iSize);
		for(int i=0;i<iSize;i++)
			(*dac)->list[i] = gcnew csDriverOEMPA::csDac(0.0,0.0f);
		return true;
	}
	bool WINAPI gCallbackSetDataDac(struct structCallbackArrayFloatDac *pCallbackArray,int iIndex,double dTime,float fSlope)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDac^>* dac;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		dac = (gcroot<csDriverOEMPA::acsDac^>*)(pCallbackArray->apParameter[1]);
		if(!dac)
			return false;
		if(iIndex<0)
			return false;
		if(iIndex>=(*dac)->list->Length)
			return true;
		((*dac)->list[iIndex])->dTime = dTime;
		((*dac)->list[iIndex])->fSlope = fSlope;
		return true;
	}
	bool WINAPI gCallbackGetSizeDac(struct structCallbackArrayFloatDac *pCallbackArray,int &iSize)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDac^>* dac;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		dac = (gcroot<csDriverOEMPA::acsDac^>*)(pCallbackArray->apParameter[1]);
		if(!dac)
			return false;
		iSize = (*dac)->list->Length;
		return true;
	}
	bool WINAPI gCallbackGetDataDac(struct structCallbackArrayFloatDac *pCallbackArray,int iIndex,double &dTime,float &fSlope)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsDac^>* dac;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		dac = (gcroot<csDriverOEMPA::acsDac^>*)(pCallbackArray->apParameter[1]);
		if(!dac)
			return false;
		if(iIndex<0)
			return false;
		if(iIndex>=(*dac)->list->Length)
			return false;
		dTime = ((*dac)->list[iIndex])->dTime;
		fSlope = ((*dac)->list[iIndex])->fSlope;
		return true;
	}

	bool WINAPI gCallbackSetSizeByte1D(struct structCallbackArrayByte1D *pCallbackArray,int iSize)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsByte^>* csByte;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csByte = (gcroot<csDriverOEMPA::acsByte^>*)(pCallbackArray->apParameter[1]);
		if(!csByte)
			return false;
		(*csByte)->list = gcnew array<BYTE>(iSize);
		return true;
	}
	bool WINAPI gCallbackSetDataByte1D(struct structCallbackArrayByte1D *pCallbackArray,int iIndex,BYTE byData)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsByte^>* csByte;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csByte = (gcroot<csDriverOEMPA::acsByte^>*)(pCallbackArray->apParameter[1]);
		if(!csByte || ((*csByte)->list==nullptr))
			return false;
		if(iIndex<0)
			return false;
		if(iIndex>=(*csByte)->list->Length)
			return true;
		(*csByte)->list[iIndex] = byData;
		return true;
	}
	bool WINAPI gCallbackGetSizeByte1D(struct structCallbackArrayByte1D *pCallbackArray,int &iSize)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsByte^>* csByte;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csByte = (gcroot<csDriverOEMPA::acsByte^>*)(pCallbackArray->apParameter[1]);
		if(!csByte || ((*csByte)->list==nullptr))
			return false;
		iSize = (*csByte)->list->Length;
		return true;
	}
	bool WINAPI gCallbackGetDataByte1D(struct structCallbackArrayByte1D *pCallbackArray,int iIndex,BYTE &byData)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		gcroot<csDriverOEMPA::acsByte^>* csByte;

		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pCallbackArray->apParameter[0]);
		csByte = (gcroot<csDriverOEMPA::acsByte^>*)(pCallbackArray->apParameter[1]);
		if(!csByte || ((*csByte)->list==nullptr))
			return false;
		if(iIndex<0)
			return false;
		if(iIndex>=(*csByte)->list->Length)
			return true;
		byData = (*csByte)->list[iIndex];
		return true;
	}
	void gCallbackHWMemory(CHWDevice *pHWDevice, DWORD addr, DWORD data, int size)
	{
		gcroot<csDriverOEMPA::csHWDevice^>* pointer;
		void *pAcquisitionParameter2;
		bool bMaster=true;

restart:
		if(!pHWDevice)
			return;
		switch(pHWDevice->GetMatchedDeviceHwLink())
		{
		case eUnlinked:	break;
		case eMaster:	break;
		case eSlave:	if(!bMaster)
							return;//error
						bMaster = false;
						pHWDevice = pHWDevice->GetMatchedDevice();
						goto restart;
						break;
		}
		pAcquisitionParameter2 = pHWDevice->GetAcquisitionParameter();
		if(!pAcquisitionParameter2)
			return;
		pointer = (gcroot<csDriverOEMPA::csHWDevice^>*)(pAcquisitionParameter2);
		(*pointer)->CallbackHWMemory(bMaster, addr, data, size);
	}

#pragma endregion callback

#pragma region init
	void init(structAcqInfoEx &acqInfo)
	{
		acqInfo.lEncoder[0] = 0x12345678;
		acqInfo.lEncoder[1] = 0x89ABCDEF;
		acqInfo.dEncoder[0] = 1.2345678;
		acqInfo.dEncoder[1] = 1.2345678;
		acqInfo.dwDigitalInputs = 0x12345678;
		acqInfo.lDeviceId = 0x89ABCDEF;
		acqInfo.bMultiChannel = 0x12345678;
		acqInfo.bFullMatrixCapture = 0;
		acqInfo.lFMCElementIndex = 0x12345678;
	};

	void init(CStream_0x0001 &StreamHeader)
	{
		StreamHeader.start = 0x12345678;
		StreamHeader.size = 0x89ABCDEF;
		StreamHeader.frameId = 0x12345678;
		StreamHeader.settingId = 0x89ABCDEF;
		StreamHeader.subStreamCount = 0x1234;
		StreamHeader.version = 0x89AB;
	};

	void init(CSubStreamIO_0x0101 &ioHeader)
	{
		ioHeader.id = 0x12;
		ioHeader.version = 0x89;
		ioHeader.size = 0x1234;
		ioHeader.timeStampLow = 0x89ABCDEF;
		ioHeader.timeStampHigh = 0x12345678;
		ioHeader.cycle = 0x89AB;
		ioHeader.padding0 = 0x1234;
		ioHeader.seqLow = 0x89ABCDEF;
		ioHeader.seqHigh = 0x12345678;
		ioHeader.inputs = 0x89ABCDEF;
		ioHeader.edges = 0x12345678;
		ioHeader.encoder1 = 0x89ABCDEF;
		ioHeader.encoder2 = 0x12345678;
	};

	void init(structCscanAmp_0x0102 &bufferAmp)
	{
		bufferAmp.byAmp = 0x12;
		bufferAmp.gateId = 0x89;
		bufferAmp.wAcqId = 0x1234;
	};

	void init(structCscanAmpTof_0x0202 &bufferAmpTof)
	{
		bufferAmpTof.byAmp = 0x12;
		bufferAmpTof.gateId = 0x89;
		bufferAmpTof.wAcqIdAmp = 0x1234;
		bufferAmpTof.wTof = 0x89AB;
		bufferAmpTof.wAcqIdTof = 0x1234;
	};

	void init(CSubStreamCscan_0x0X02 &cscanHeader)
	{
		cscanHeader.id = 0x12;
		cscanHeader.version = 0x89;
		cscanHeader.size = 0x1234;
		cscanHeader.timeStampLow = 0x89ABCDEF;
		cscanHeader.timeStampHigh = 0x12345678;
		cscanHeader.cycle = 0x89AB;
		cscanHeader.count = 0x1234;
		cscanHeader.seqLow = 0x89ABCDEF;
		cscanHeader.seqHigh = 0x12345678;
	};

	void init(CSubStreamAscan_0x0103 &AscanHeader)
	{
		AscanHeader.id = 0x12;
		AscanHeader.version = 0x89;
		AscanHeader.size = 0x1234;
		AscanHeader.timeStampLow = 0x89ABCDEF;
		AscanHeader.timeStampHigh = 0x12345678;
		AscanHeader.cycle = 0x89AB;
		AscanHeader.dataCount = 0x1234;
		//AscanHeader.ascan_byte1 = 0x89;
		AscanHeader.src = 1;
		AscanHeader.dst = 0;
		AscanHeader.type = 0;
		AscanHeader.error = 1;
		AscanHeader.dataSize = 0x8;//size of one data (in bytes), take a look also to member "bitSize".
		//AscanHeader.ascan_byte2 = 0x12;
		AscanHeader.align = 2;
		AscanHeader.max = 1;//maximum buffer is valid.
		AscanHeader.min = 0;//minimum buffer is valid.
		AscanHeader.sat = 0;//saturation buffer is valid.
		AscanHeader.sign = 0;//sign of maximum and minimum buffer data.
		//AscanHeader.ascan_word1 = 0x89AB;
		AscanHeader.bitSize = 3;//see enumBitSize
		//AscanHeader.seqLost = 0;//encoder speed too high.
		AscanHeader.dacIFOldReference = 0;
		AscanHeader.dacIFNotInitialized = 0;
		AscanHeader.IFOldReference = 0;
		AscanHeader.IFNotInitialized = 0;
		AscanHeader.padding0 = 0x1135;
		AscanHeader.FWAcqIdChannelCycle = 0x1234;
		AscanHeader.FWAcqIdChannelScan = 0x89AB;
		AscanHeader.FWAcqIdChannelProbe = 0x1234;
		AscanHeader.padding1 = 0x89AB;
		AscanHeader.seqLow = 0x12345678;
		AscanHeader.seqHigh = 0x89ABCDEF;
	};

#pragma endregion init
//////////////////////////////////////////////////////////////////////////////
