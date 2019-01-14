
#pragma once

#define STRING_ARRAY_SIZE	L".count"

const int g_iOEMPADriverCountMax=8;
const int m_iNameSize=27+1;

const int g_iOEMPAApertureDWordCount=4;//4*32=64 elements (with a MUX only, without MUX it is 64 elements).

///////////////////////////////////////////////////////////////////////////////////////
// ENUMERATIONS
///////////////////////////////////////////////////////////////////////////////////////

typedef enum enumSharedEncoder{		eNoSharedEncoder,//encoder is not used.
									eNewSharedEncoder,//new free shared encoder (for example eSharedEncoder00 if it has not been used).
									eSharedEncoder00,
									eSharedEncoder01,
									eSharedEncoder02,
									eSharedEncoder03,
									eSharedEncoder04,
									eSharedEncoder05,
									eSharedEncoder06,
									eSharedEncoder07,
									eSharedEncoder08,
									eSharedEncoder09,
									eSharedEncoder10,
									eSharedEncoder11,
									eSharedEncoder12,
									eSharedEncoder13,
									eSharedEncoder14,
									eSharedEncoder15};

typedef enum enumAcquisitionState{	eAcqOff,	//pulse shot is disabled.
									eAcqDefault,//the configuration file is used to stop or start the pulse shot.
									eAcqOn		//pulse shot is enabled.

									};
typedef enum enumHardware{	eNoHW,eOEMPA };
typedef enum enumHardwareLink{eUnlinked,eMaster,eSlave};
typedef enum enumCommunication{	eNoCommunication,eEthernet,eUSB2 };
typedef enum enumConnectionState{	eDisconnected,eConnected };
typedef enum enumUpdateStatus{	eOutOfDate,eUpToDate };
typedef enum enumStreamPackage{ eSingle,eSort,eFifo };
typedef enum enumCommunicationOption{ eNoMemory,eRequestHW,eReadSW };
typedef enum enumAcquisitionFlush{	eAutomatic,ePooling };

typedef enum enumFieldStreamIndex {
		indexStreamFlag =	(int) 0x00000,
		indexStreamSize =	(int) 0x00001,
		indexStreamText =	(int) 0x00002
		}enumFieldStreamIndex;
typedef enum enumSubStreamId {
		idSubStreamIO =			(int) 0x01,
		idSubStreamCscan =		(int) 0x02,
		idSubStreamAscan =		(int) 0x03,
		idSubStreamParameters =	(int) 0x04
		}enumSubStreamId;



//those values are stored in file.
typedef enum enumClassID{	eCUTBaseClass=10,

							eCUTPulser=129,eCUTAcquisitionInfo=130,eCUTGate=131,
							//eCUTCscanGate=132,
							eCUTReceiver=133,eCUTPulserReceiver=134,eCUTScan=135,eCUTProbe=136,eCUTChannels=137,
							eCUTFocalLaw=138,eCUTDevice=139,eCUTDeviceMonitor=140,

							eCUTAxis=144,
eCUTAcquisitionData=145,
eCUTDisplayScreen=146,eCUTDisplayView=147,eCUTDisplayWindow=148,
eCUTDisplayCursor=149,eCUTDisplayZoom=150,
							eCUTDisplayLayout=151,
							eCUTEncoder=152,

							eCUTWizardProbe=160,eCUTWizardSpecimen=161,eCUTWizardWedge=162,
							eCUTWizardSystem=163,eCUTWizardAperture=164,eCUTWizardVector=165,
							eCUTWizardFocalLaw=166,eCUTWizardScan=167,eCUTWizardAcquisition=168,
							eCUTModule=169,
eCUTChannelsSimulator=170,
							eCUTPreference=171,
							//eCUTReservedForFuture=172,

							eCUTLinkId=180,eCUTKernelListLinkId=181,


							eCUTDeviceOEMPA=240,eCUTFilterOEMPA=241,

							//see also enum "enumRootType".
							eKernelAcquisitionBuffer2=256,
							eLastClassID=512,eMaxClassID=0xffff

							}enumClassID;

typedef enum enumRootType{	eRootTypeUnknown=0,

							
							eRootTypeAxis=eCUTAxis, eRootTypeAcquisitionData=eCUTAcquisitionData,
							eRootTypeDisplayCursor=eCUTDisplayCursor, eRootTypeDisplayZoom=eCUTDisplayZoom, eRootTypeDisplayLayout=eCUTDisplayLayout,
							eRootTypeChannels=eCUTChannels,eRootTypeWizardSystem=eCUTWizardSystem,//eKernelWizardScan=eCUTWizardScan,
							eRootTypeEncoder=eCUTEncoder,eRootTypePreference=eCUTPreference,
							eRootTypeDevice=eCUTDevice,
							//eRootTypeReservedForFuture=eCUTReservedForFuture,

							eRootTypeAcquisitionBuffer=256,
							eRootTypePath=384, eRootTypeExtendParameter=385
						} enumRootType;

typedef enum enumVersionType{eVersionFile,eVersionProduct};
typedef enum enumVersionWord{eMajorMSWord,eMajorLSWord,eMinorMSWord,eMinorLSWord};
typedef enum enumUTFormat { eNone, eFileBinary, eFileText, eSharedMemory };
typedef enum enumProperty{eGetComment,eGetPath,eGetRoot,eGetBaseLimit,eCheckLinkId,eSetLinkId,eUpdateFromObject};
typedef enum enumCallback{eCallbackCall,eCallbackWndMessage,eCallbackReplay,eCallbackDone};

typedef enum enumBitSize {e8Bits,e12Bits,e16Bits,eLog8Bits};
typedef enum enumAscanRequest {eAscanAll,eAscanSampled,eBscanOnLine};
typedef enum enumCompressionType {eCompression,eDecimation};
typedef enum enumGateModeAmp{eAmpAbsolute,eAmpMaximum,eAmpMinimum,eAmpPeakToPeak};
typedef enum enumGateModeTof{eTofAmplitudeDetection,//"AMP's TOF" : where the AMP result has been found, for Peak-Peak--> where Max has been found
							eTofThresholdCross,//"TH cross": first cross of the THRESHOLD  
							eTofZeroFirstAfterThresholdCross,//"ZrA": first time crossed 0 after crossing THRESHOLD
							eTofZeroLastBeforeThresholdCross//"ZrB": last time crossed 0 before crossing THRESHOLD
							};
typedef enum enumRectification{
						eSigned,			//Rectification: RF --> not rectified, signed value,
						eUnsigned,			//FW --> rectified, unsigned
						eUnsignedPositive,	//HWP --> only positive, unsigned,
						eUnsignedNegative	//HWN --> only negative
						};

typedef enum enumProbeMode{	ePulseEcho,//one probe is used to emit and receive.
							ePulseEchoSeparate,//two probes are on the same element : one to emit and another one to receive.
							ePulseOnly,
							eEchoOnly};
typedef enum enumFocusing{eStandard,eDDF};
typedef enum enumTrigger{
					eTimer,//wait timer before emitting the pulse.
					eEncoderXOneWay,//wait encoder moving (one way only) before emitting the pulse.
					eEncoderXBothWay,//wait mechanical moving (forward or backward) before emitting the pulse.
					eEncoderYOneWay,//wait encoder moving (one way only) before emitting the pulse.
					eEncoderYBothWay,//wait mechanical moving (forward or backward) before emitting the pulse.
					eExternal,//wait external pulse.
					eSoftware//the SW trig the shot.
						};
//typedef enum enumExternal{
//					eCycleOnly,//cycle is used also to change sequence.
//					eSequenceOnly,//cycles are still generated internally.
//					eCycleAndSequence//sequence is incremented with one digital input, cycle is incrmented with another digital input.
//						};
typedef enum enumIndexDigitalInput{
					eIndexDigitalInputBlanking=0,
					eIndexDigitalInputWire1=1,
					eIndexDigitalInputWire2=2,
					eIndexDigitalInputPresetRising=3,
					eIndexDigitalInputSpeed1=4,
					eIndexDigitalInputSpeed2=5,
					eIndexDigitalInputExternalCycle=6,
					eIndexDigitalInputExternalSequence=7,
					eIndexDigitalInputEncoder2Wire1=8,
					eIndexDigitalInputEncoder2Wire2=9,
					eIndexDigitalInputEncoder2PresetRising=10,
					eIndexDigitalInputLast
						};

typedef enum enumDigitalInput{
					eDigitalInputOff,
					eDigitalInput01,
					eDigitalInput02,
					eDigitalInput03,
					eDigitalInput04,
					eDigitalInput05,
					eDigitalInput06,
					eDigitalInput07,
					eDigitalInput08,
					eDigitalInput09,
					eDigitalInput10,
					eDigitalInput11,
					eDigitalInput12,
					eDigitalInput13,
					eDigitalInput14,
					eDigitalInput15,
					eDigitalInput16,
					eDigitalInput17,
					eDigitalInput18,
					eDigitalInput19,
					eDigitalInput20,
					eDigitalInput21,
					eDigitalInput22,
					eDigitalInput23,
					eDigitalInput24
						};
typedef enum enumEncoderType{
					eStaticScan,	//offset are used to store the data (no encoder).
					eEncoderQuadrature,	//encoder are used to store the data (quadrature).
					eEncoderQuadrature4Edges,//encoder are used to store the data (quadrature).
					eEncoderDirectionCount,	//encoder : one signal for the direction and one signal for the count.
					eEncoderForwardBackward,	//encoder : one signal for the forward and one signal for the back.
					eSpeedMeasurement //OnLine.
						};
typedef enum enumInternalMode{
					eInternalOff, //Off.
					eInternalStandAlone,//for one encoder that has not complementary encoder.
					eInternal1,//with two axis, first internal increment.
					eInternal2//with two axis, second internal increment.
						};
typedef enum enumArrayParamAccess{
					eNoParam,
					eParamInput,
					eParamOutput
						};
typedef enum enumGenericAccess{
					eOverwrite,
					eMaximum,
					eMinimum,
					eBitwiseOr,//same than eMaximum (bitwise version).
					eBitwiseAnd//same than eMinimum (bitwise version).
						};
typedef enum enumDataType{
					eBitwiseByte,//bit access, sizeof=1 BYTE (usefull for alarm data type).
					eBitwiseWord,//bit access, sizeof=2 WORD (usefull for alarm data type).
					eBitwiseDword,//bit access, sizeof=4 DWORD (usefull for alarm data type).
					eChar,//char, sizeof=1
					eUChar,//unsigned char, sizeof=1
					eShort,//short, sizeof=2
					eUShort,//unsigned short, sizeof=2
					eLong,//long, sizeof=4
					eULong//unsigned long, sizeof=4
						};
typedef enum enumAxis{
					eA,//amplitude axis for amplitude data or time of flight data.
					eX,//scan axis
					eY,//index axis
					eZ,//ultrasound axis
					eRho,//used instead of eY with polar coordinate (for cylindar specimen).
					eTheta//used instead of eZ with polar coordinate (for cylindar specimen).
					};

typedef enum enumHistoricIndex{	eIndexX,
									eIndexY,
									eIndexXY,
									eIndexError
									};

typedef enum enumUnit{	eNoUnit,eHexadecimal,ePercent,eMeter,eMilliMeter,eSecond,eMilliSecond,eMicroSecond,eNanoSecond,
						eMeterPerSecond,eRadian,eDegree,eDecibel,eHertz,eKiloHertz,eMegaHertz,eVolt,
						eBytePerSecond,eKiloBytePerSecond,eMegaBytePerSecond,eDegreeCelsius,eLinkRootId,
						eBinary,eStepPerMilliMeter,
						eLastUnit };

typedef enum enumDisplayType{
					eYZ,//Bscan
					eXY,//Cscan
					eXZ,//Dscan
					eProfileZ,//Ascan
					eProfileX,//Profile Cscan
					eProfileY//Profile Cscan
						};

typedef enum enumConfigurationSelection{
	eSave,
	eLoad,
	eDelete
};

typedef enum enumApply{
	eStandby,
	ePending,
	eDone
};

typedef enum enumKeepAlive{
	eKeepAliveOff,
	eKeepAliveHardwareAndComputer,
	eKeepAliveHardwareOnly,
	eKeepAliveComputerOnly
};

///////////////////////////////////////////////////////////////////////

typedef enum enumOEMPATrigger{
	eOEMPAInternal,//encoder and external signals are not used.
	eOEMPAEncoder,//encoder 1 is used for the sequence (cycle is internal).
	eOEMPAExternalCycle,//the next cycle after the last cycle is for the next sequence.
	eOEMPAExternalSequence,//sequence is coming from external pulse (cycle are internal).
	eOEMPAExternalCycleSequence//cycle and sequence are coming from external signals.

};

typedef enum enumOEMPAEncoderDirection{
	eOEMPAUpDown,//pulser is triggered for both directions.
	eOEMPAUpOnly,//pulser is triggered only if encoder goes up.
	eOEMPADownOnly//pulser is triggered only if encoder goes down.

};

typedef enum enumOEMPARequestIO{
	eOEMPANotRequired=0,
	eOEMPAOnCycleOnly,//use this value if you want Ascan with encoder.
	eOEMPAOnSequenceOnly,
	eOEMPAOnDigitalInputOnly,
	eOEMPAOnDigitalInputAndCycle,//use this value if you want Ascan with encoder and digital inputs.
	eOEMPAOnDigitalInputAndSequence

};

typedef enum enumOEMPAFilter{
	eOEMPAFilterCustom,
	eOEMPAHighPassOrder64Cut1Mhz,
	eOEMPAHighPassOrder64Cut2Mhz,
	eOEMPAHighPassOrder64Cut3Mhz,
	eOEMPAHighPassOrder64Cut4Mhz,
	eOEMPAHighPassOrder64Cut5Mhz,
	eOEMPAHighPassOrder64Cut6Mhz,
	eOEMPAHighPassOrder64Cut7Mhz,
	eOEMPAHighPassOrder64Cut8Mhz,
	eOEMPAHighPassOrder64Cut9Mhz,
	eOEMPAHighPassOrder64Cut10Mhz

};

typedef enum enumOEMPAFilterIndex{
	eOEMPAFilterOff,
	eOEMPAFilter1,
	eOEMPAFilter2,
	eOEMPAFilter3,
	eOEMPAFilter4,
	eOEMPAFilter5,
	eOEMPAFilter6,
	eOEMPAFilter7,
	eOEMPAFilter8,
	eOEMPAFilter9,
	eOEMPAFilter10,
	eOEMPAFilter11,
	eOEMPAFilter12,
	eOEMPAFilter13,
	eOEMPAFilter14,
	eOEMPAFilter15

};
const int g_iEnumOEMPAFilterIndexLast=eOEMPAFilter15+1;

typedef enum enumOEMPAMappingDigitalInput{
	eOEMPANotUsed,
	eOEMPAEncoder1A,
	eOEMPAEncoder1B,
	eOEMPAEncoder2A,
	eOEMPAEncoder2B,
	eOEMPAExternalTriggerCycle,
	eOEMPAExternalTriggerSequence,
	eOEMPADigitalInput0,
	eOEMPADigitalInput1,
	eOEMPADigitalInput2,
	eOEMPADigitalInput3,
	eOEMPADigitalInput4,
	eOEMPADigitalInput5,
	eOEMPAResetTrackingTable

};

typedef enum enumOEMPAMappingDigitalOutput{
	eOEMPALow,
	eOEMPAHigh,
	eOEMPASignalCycle,
	eOEMPASignalSequence,
	eOEMPATemperatureAlarm

};

typedef enum enumOEMPARequestCscan{
	eOEMPACycle,
	eOEMPASequence

};

typedef enum enumOEMPAAnalogBandwidth{
	eLow,
	eHigh
};

///////////////////////////////////////////////////////////////////////

typedef enum enumEvent{
					eEventNew=0,//new identifier has been allocated (shared data with single name).
					eEventPostNew=1,//new identifier has been allocated (shared data with single name).
					eEventPreDelete=2,//identifier is going to be desallocated (shared data removed).
					eEventDelete=3,//shared data has been removed.
					eEventReName=4,//an instance has been renamed (identifier has been allocated for shared data).
					eEventAlloc=5,//acquisition buffer : alloc (notification after allocating).
					eEventFree=6,//acquisition buffer : free (notification before releasing).
					eEventTrace=7,//system and user information (debug purpose).
					eEventPreUpdate=8,//a shared data is going to be updated.
					eEventUpdate=9,//a shared data has been updated.
					eEventSave=10,//used to filter "Save()" in shared memory only one is required at the end of all operations.
					eEventLoad=11,
					eEventPostLoad=12,
					eEventLanguage=13,//language has been changed.
					eEventAll=32,//last item, for previous one (check of the path is required for callback registration).
					eEventParametersFWAcqId=33,//update "CUTAcquisitionInfo".
					eEventBufferReqFree=34,//used to dispatch event process to close.
					eEventBufferReqAlloc=35,//used to dispatch event process to open.
					eEventBufferAllocated=36,//notification (acquisition buffer data has been opened). eEventFree 
					eEventBufferFreed=37,//notification (acquisition buffer data has been closed).
					eEventBufferPosition=38,//you will be able to set the acquisition buffer position where to store acquisition data.
					eEventRefreshUpdate=39,//acquisition refresh occured.
					eEventDisplayRootId=40,//display kernel object (RootId is given by lParam).
					eEventDeviceConnected=41,//device connection.
					eEventDeviceDisconnected=42,//device disconnection.
					eEventUser0=256 //user have to use value over this one.
						};

///////////////////////////////////////////////////////////////////////////////////////////////

typedef enum enumSpecimen{
			ePlane,	//the referential point (0,0,0) is on the front surface. X is the mechanical scan axis.
			eDisk, //same than 2D bar
			eBar, //the referential point (0,0,0) is on the axis of the part, the X axis is the main axis of the bar.
			eTube //tube in the future. //same referential than the bar.
			};

typedef enum enumCoordinate{
			eCartesian,//In case of plane specimen cartesian coordinates are used.
							//X : scan axis.
							//Y : index axis (0 is the reference point of the probe).
							//Z : ultrasound path, the probe have a positive value.
			ePolar	//In case of bar or tube specimen polar coordinates are used.
							//X : scan axis.
							//θ : angle (0 is the reference point of the probe).
							//Z : ultrasound path (0 on the bar axis).
			};

typedef enum enumOffsetType{
			eDistance,//In case of plane specimen cartesian coordinates are used.
			eArcLength,//In case of bar or tube the OffsetY could be measured on the bar radius.
			eAngle//In case of bar or tube the OffsetY could be defined with an angle.
			};

typedef enum enumWizardProbe{
			eMono,
			eLinear1D,
			eLinear2D,
			eCustomTRL,
			eCustom,
			eLinear1D_NextAfterLastIsFirst
			};

typedef enum enumAcquisitionWizardMode{
										eCreateAcquisitionDataAndAxis,
										eUseExistingAcquisitionData
								};

typedef enum enumAcquisitionDataType{
										eAscan,
										eCscanAmplitude,
										eCscanTimeOfFlight,
								};

typedef enum enumAcquisitionMode{	eOffset,//use the distance offset of the focal law.
									eIndex//use the offset as an index.
									};

typedef enum enumAcquisitionDataMode{
										eMerge,		//=> eEncoderAndFocalPoint for X AND/OR Y; eScanOrder for X OR Y.
										eSeparate	//=> eEncoderOnly for X AND/OR Y; 
								};

typedef enum enumAcquisitionEncoderMode {eStatic,
										eEncoderOnly,
										eEncoderAndFocalPoint,
										eScanOrder
								};
typedef enum enumDepthMode {eStandardDepth,eSoundPath,eDepthBeforeBackwall,eDepthAfterBackwall,ePathBeforeProbe,eDepthBigBar};
typedef enum enumWave {eLongitudinal,eTransverse};
typedef enum enumScan {	eScanFirst,eScanDepth,eScanAngle,eScanSkew,eScanOffset1,eScanOffset2,
						eScanLast};
				//"eScanApertureCallback" is used to get index list for "eCustom" probe.

//////other private kernel enumerations///////

typedef enum enumReferential{
			eSpecimen=0,//absolute referential (inspection referential).
			eWedge=1,//usefull for bar and tube (tangent on the interface).
			eProbe=2,//referential of the probe, used to define it.
			eReferentialCount
			};

typedef enum enumUserMode{
			eUser,
			eAdvanced,
			eSupervisor
			};

typedef enum enumTFMOptions{ eCPU=0,eGPU=1,eCPU_1Thread=2 };
typedef enum enumTFMParameters{ eTFMOff=0,eFlatContact_Linear1D=1 };
typedef enum enumAcquisitionFifo{ eFifoAscan, eFifoCscan, eFifoIO };

/////////////////////////////////////////////////////////////////
