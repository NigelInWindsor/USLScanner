
#pragma once

const int g_iDriverEncoderCountMax=2;
const int g_iOEMPAEncoderCountMax=2;

const int g_iMaxNameLength=48;
const int g_iOEMPABoardCountMax=4;
const int g_iOEMPAComBoardTemperatureSensorCountMax=3;
const int g_iOEMPARXBoardTemperatureSensorCountMax=5;
const int g_iOEMPAMappingDigitalInputMax=6;
const int g_iOEMPAMappingDigitalOutputMax=6;
const int g_iOEMPAFilterCoefficientMax=34;
const int g_iCallbackArrayParameterCountMax=6;

class CStream_0x0001;
class CSubStreamAscan_0x0103;
class CSubStreamCscan_0x0X02;
class CSubStreamIO_0x0101;
class CUTChannels;

struct structCscanAmp_0x0102;
struct structCscanAmpTof_0x0202;

typedef struct structAcqInfoEx{
	long lEncoder[g_iDriverEncoderCountMax];//unit given by the encoder
	double dEncoder[g_iDriverEncoderCountMax];//unit : meter (encoder resolution is taken into account).
	DWORD dwDigitalInputs;
	long lMaxTemperature;//0 if not significant (for some FW version or in case IO stream are not sent by the HW).
	long lDeviceId;//device identifier
	long bMultiChannel;//1 if Multi-Channel is enabled, 0 otherwise.
	long bFullMatrixCapture;//1 if FullMatrixCapture is enabled, 0 otherwise.
	long lFMCElementIndex;//in case of Full Matrix Capture, this is the element index for the current ascan;
}structAcqInfoEx;

typedef UINT (WINAPI *pTypeAcquisitionAscan_0x00010103)(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
	//input parameters
	//- "pAcquisitionParameter" : given by the user (see "CHWDevice::SetAcquisitionParameter").
	//- "pAcqInfo" : encoder values and extra data ("pAcqInfo::pChannels" is reserved for kernel API).
	//		"structAcqInfoEx::lEncoder1" first encoder associated to the other inputs parameters (according that IO stream are sent, it depends of the HW and SW configuration).
	//		"structAcqInfoEx::lEncoder2" second encoder associated to the other inputs parameters (according that IO stream are sent, it depends of the HW and SW configuration).
	//- "pStreamHeader" is a pointer on stream description, useful member is:
	//		"CStream_0x0001::settingId" identifier of the FW parameter set.
	//- "pAscanHeader" is a pointer on data description, useful members are:
	//		"CSubStreamAscan_0x0103::cycle" cycle number (for single acquisition channel), for multiple acquisition channel refer to "pAcqInfo".
	//		"CSubStreamAscan_0x0103::dataCount" data count.
	//		"CSubStreamAscan_0x0103::dataSize" size of one data (in bytes, for max, min and sat buffer).
	//		"CSubStreamAscan_0x0103::max" maximum buffer is valid (pBufferMax is not null).
	//		"CSubStreamAscan_0x0103::min" minimum buffer is valid (pBufferMin is not null).
	//		"CSubStreamAscan_0x0103::sat" saturation buffer is valid (pBufferSat is not null).
	//		"CSubStreamAscan_0x0103::seqLow" sequence number (LOW DWORD).
	//		"CSubStreamAscan_0x0103::seqHigh" sequence number (HIGH DWORD).
	//		"CSubStreamAscan_0x0103::FWAcqIdChannelProbe" probe index of the logical channel (acquisition identifier information), see "OEMPA_SetAscanAcqIdChannelProbe",
	//		"CSubStreamAscan_0x0103::FWAcqIdChannelScan" scan index of the logical channel (acquisition identifier information), see "OEMPA_SetAscanAcqIdChannelScan".
	//		"CSubStreamAscan_0x0103::FWAcqIdChannelCycle" cycle index of the logical channel (acquisition identifier information), see "OEMPA_SetAscanAcqIdChannelCycle".
	//		see comments in "CSubStreamAscan_0x0103" for more information.
	//- "pBufferMax" is the buffer of maximum acquisition data, array of data ("dataSize" is the size of one data).
	//		("CSubStreamAscan_0x0103::dataCount" x "CSubStreamAscan_0x0103::dataSize" is the total size of this buffer .
	//- "pBufferMin" is the buffer of minimum acquisition data, array of data ("dataSize" is the size of one data).
	//		("CSubStreamAscan_0x0103::dataCount" x "CSubStreamAscan_0x0103::dataSize" is the total size of this buffer .
	//- "pBufferSat" is the buffer of saturation acquisition data, array of data ("dataSize" is the size of one data but
	//		only the LSB bit is significant other bits are 0).
	//		("CSubStreamAscan_0x0103::dataCount" x "CSubStreamAscan_0x0103::dataSize" is the total size of this buffer .

typedef UINT (WINAPI *pTypeAcquisitionCscan_0x00010X02)(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof);
	//input parameters
	//- "pAcquisitionParameter" : given by the user (see "CHWDevice::SetAcquisitionParameter").
	//- "pAcqInfo" : encoder values and extra data ("pAcqInfo::pChannels" is reserved for kernel API).
	//		"structAcqInfoEx::lEncoder1" first encoder associated to the other inputs parameters (according that IO stream are sent, it depends of the HW and SW configuration).
	//		"structAcqInfoEx::lEncoder2" second encoder associated to the other inputs parameters (according that IO stream are sent, it depends of the HW and SW configuration).
	//- "pStreamHeader" is a pointer on stream description, useful member is:
	//		"CStream_0x0001::settingId" identifier of the FW parameter set.
	//- "pCscanHeader" is a pointer on data description, useful members are:
	//		"CSubStreamCscan_0x0X02::version" sub stream type (1: amplitude only, 2 amplitude and time of flight).
	//		"CSubStreamCscan_0x0X02::cycle" cycle number.
	//		"CSubStreamCscan_0x0X02::count" data count.
	//		"CSubStreamCscan_0x0X02::seqLow" sequence number (LOW DWORD).
	//		"CSubStreamCscan_0x0X02::seqHigh" sequence number (HIGH DWORD).
	//		see comments in "CSubStreamCscan_0x0X02" for more information.
	//- "pBufferAmp" is the buffer of cscan amplitude (sub stream version.
	//		this is an array which size is "CSubStreamCscan_0x0X02::count".
	//		NULL in case "CSubStreamCscan_0x0X02::version" is 2 (amplitude and time of flight).
	//- "pBufferAmpTof" is the buffer of cscan amplitude and time of flight.
	//		this is an array which size is "CSubStreamCscan_0x0X02::count".
	//		NULL in case "CSubStreamCscan_0x0X02::version" is 1 (amplitude only).

typedef UINT (WINAPI *pTypeAcquisitionIO_0x00010101)(void *pAcquisitionParameter,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader);
	//Useful members are (see class "CSubStreamIO_0x0101"):
	//CSubStreamIO_0x0101::encoder1
	//CSubStreamIO_0x0101::encoder2
	//CSubStreamIO_0x0101::inputs	: digital inputs
	//CSubStreamIO_0x0101::edges	: which digital inputs has been updated ?

typedef UINT (WINAPI *pTypeAcquisitionIO_1x00010101)(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader);
	//Useful members are (see class "CSubStreamIO_0x0101"):
	//CSubStreamIO_0x0101::encoder1
	//CSubStreamIO_0x0101::encoder2
	//CSubStreamIO_0x0101::inputs	: digital inputs
	//CSubStreamIO_0x0101::edges	: which digital inputs has been updated ?

typedef UINT (WINAPI *pTypeAcquisitionInfo)(void *pAcquisitionParameter,const wchar_t *pInfo);
	//example of input parameter: "HW Error : ..." error reporting by the hardware.

typedef UINT (WINAPI *pTypeAcquisitionStream)(void *pAcquisitionParameter,void *pData,int iSize);
	//This function can be used to receive the whole FW stream (including all ascan, cscan, IO...).
	//OEM-PA1: whole stream from USB3.

class CHWDevice;
typedef struct structCallbackNotification{
	void *pAcquisitionParameter;
	pTypeAcquisitionAscan_0x00010103 pAscan_0x00010103;
	pTypeAcquisitionCscan_0x00010X02 pCscan_0x00010X02;
	pTypeAcquisitionIO_0x00010101 pIO_0x00010101;
	pTypeAcquisitionIO_1x00010101 pIO_1x00010101;
	bool bIODigitalEdgesOnly;
	pTypeAcquisitionInfo pInfo;
	pTypeAcquisitionStream pStream;
}structCallbackNotification;
typedef UINT (WINAPI *pTypeCallbackNotification)(CHWDevice *pHWDevice,structCallbackNotification &data);
	//input parameters
	//- "pHWDevice" : driver pontier.
	//- "data" : list of all acquisition callback functions.


typedef struct structTFM_FlatContact_Linear1D{
									double dProbePitchIn;
									double dXFirstElement;//unit meter
									double dXMin;//unit meter
									double dXMax;//unit meter
									double dZMin;//unit meter
									double dZMax;//unit meter
									int iApertureElementCount;
								}structTFM_FlatContact_Linear1D;
typedef union unionTFMParameters{
									structTFM_FlatContact_Linear1D	flatContactLinear1D;
								}unionTFMParameters;
