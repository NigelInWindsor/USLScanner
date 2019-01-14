
#pragma once

#include "CRead_Write_ParametersList.h"

#define OEMPA_ASCAN_ENABLE								0x0000000000000001
#define OEMPA_CSCAN_ENABLE_TOF							0x0000000000000002
#define OEMPA_ASCAN_BITSIZE								0x0000000000000004
#define OEMPA_TRIGGER_MODE								0x0000000000000008
#define OEMPA_TRIGGER_ENCODER_STEP						0x0000000000000010
#define OEMPA_REQUESTIO									0x0000000000000020
#define OEMPA_REQUESTIO_DIGITAL_INPUT_MASK				0x0000000000000040
#define OEMPA_KEEPALIVE									0x0000000000000080
#define OEMPA_DEBOUNCER_ENCODER							0x0000000000000100
#define OEMPA_DEBOUNCER_DIGITAL							0x0000000000000200
#define OEMPA_DIGITAL_OUTPUT_0							0x0000000000000400
#define OEMPA_DIGITAL_OUTPUT_1							0x0000000000000800
#define OEMPA_DIGITAL_OUTPUT_2							0x0000000000001000
#define OEMPA_DIGITAL_OUTPUT_3							0x0000000000002000
#define OEMPA_DIGITAL_OUTPUT_4							0x0000000000004000
#define OEMPA_DIGITAL_OUTPUT_5							0x0000000000008000
#define OEMPA_SW_ENCODER1_RESOLUTION					0x0000000000010000
#define OEMPA_SW_ENCODER1_DIVIDER						0x0000000000020000
#define OEMPA_ENCODER1A									0x0000000000040000
#define OEMPA_ENCODER1B									0x0000000000080000
#define OEMPA_ENCODER2A									0x0000000000100000
#define OEMPA_ENCODER2B									0x0000000000200000
#define OEMPA_EXTERNAL_TRIGGER_CYCLE					0x0000000000400000
#define OEMPA_EXTERNAL_TRIGGER_SEQUENCE					0x0000000000800000
#define OEMPA_SW_ENCODER2_RESOLUTION					0x0000000001000000
#define OEMPA_SW_ENCODER2_DIVIDER						0x0000000002000000
#define OEMPA_ENCODER1_TYPE								0x0000000004000000
#define OEMPA_ASCAN_REQUEST								0x0000000008000000
#define OEMPA_DEFAULT_HW_LINK							0x0000000010000000
#define OEMPA_TRACKING									0x0000000020000000
#define OEMPA_ENCODER2_TYPE								0x0000000040000000
#define OEMPA_ENABLE_FMC								0x0000000080000000
#define OEMPA_ENABLE_MULTI_CHANNEL						0x0000000100000000
#define OEMPA_DISABLE_USB3								0x0000000200000000
#define OEMMC_DECIMATION								0x0000000400000000
#define OEMPA_ENCODER_TRIG_DIRECTION					0x0000000800000000
#define OEMPA_TEMPERATURE_WARNING						0x0000001000000000
#define OEMPA_TEMPERATURE_ALARM							0x0000002000000000
#define OEMPA_TRIGGER_HIGH_TIME							0x0000004000000000
#define OEMPA_REFERENCE_TIME_OF_FLIGHT					0x0000008000000000

#define OEMPA_CYCLE_POINTCOUNT_AND_COMPRESSION			0x0000010000000000
#define OEMPA_CYCLE_POINTFACTOR_AND_COMPRESSION			0x0000020000000000
#define OEMPA_CYCLE_DAC_SLOPE							0x0000040000000000
#define OEMPA_CYCLE_DAC_GAIN							0x0000080000000000
#define OEMPA_CYCLE_ACQ_CHANNEL_ID						0x0000100000000000
#define OEMPA_CYCLE_ASCAN_BUFFER						0x0000200000000000

typedef enum enumReferenceTimeOfFlight{
	ePulse,//times of flight are defined from the pulser.
	eInterface//times of flight are defined from the interface.
} enumReferenceTimeOfFlight;


////////////////// CORRECTION //////////////////////

typedef struct structCorrectionOEMPA{
	int iCount;//count of useful item in the array "pdAngle".
	int iAllocCount;//allocation size.
	double dSpecimenVelocity;//m/s
	double dSpecimenRadius;//meter (if 0.0 it is a plane).
	double dSpecimenThickness;//meter (0.0 means not defined).
	double dDigitizingPeriod;//second
	double dSamplingFrequency;//hertz
	double *pdAnglePulse;
	double *pdSkewPulse;//see the member "bReadOEMPAFileNewWay".
	double *pdXPulse;//emergence point (pulser)
	double *pdYPulse;
	double *pdZPulse;
	double *pdXFocalPulse;//last focal point (pulser)
	double *pdYFocalPulse;
	double *pdZFocalPulse;
	double *pdAngleRecv;
	double *pdSkewRecv;
	double *pdXRecv;//emergence point (receiver)
	double *pdYRecv;
	double *pdZRecv;
	double *pdXFocalRecv;//last focal point (receiver)
	double *pdYFocalRecv;
	double *pdZFocalRecv;
	int *piCountRecv;
	bool bReadOEMPAFileNewWay;//definition of the skew: old way is absolute referential, new way is local referential (as it is for the attack angle "pdAnglePulse" and "pdAngleRecv").
}structCorrectionOEMPA;

typedef struct corrected_acq_data
{
    //X axis : scan axis definition
    int countX;        // point count.
    double stepX;    // resolution (unit=meter).
    double minX;    // minimum (unit=meter).
    double maxX;    // maximum : maxX = minX+countX*stepX (unit=meter).
   
    //Z axis : depth axis definition
    int countZ;        // point count of Y axis (depth) view.
    double stepZ;    // resolution (unit=meter).
    double minZ;    // minimum (unit=meter).
    double maxZ;    // maximum : maxZ = minZ+countZ*stepZ (unit=meter).
   
    //acquisition data for the corrected_view
    BYTE* acqData2;    // 2D array to store corrected acquisition data, examples:
                     // acqData2[0] => X=0 Z=0
                    // acqData2[1] => X=1 Z=0
                    // acqData2[2] => X=2 Z=0
                    // acqData2[iCountX+0] => X=0 Z=1
                    // acqData2[iCountX+1] => X=1 Z=1
                    // acqData2[iCountX+2] => X=2 Z=1
}corrected_acq_data;

typedef struct raw_acq_data
{
   double x;        // scan position of the beam on interface (emergence point, unit is meter).
   double angle;    // beam angle (unit=radian), example 0.0 : the beam is perpendicular to the interface.
                    // Orientation is counter clockwise.
   double step;        // acquisition step along acquisition beam (unit is meter).
   int count;        // data count
   BYTE* acqData1;    // acquisition data (one data = 1 BYTE).
}raw_acq_data;

////////////////////////// FOCAL LAW BASIC CLASSES ////////////////////////

class CFocalLaw;
class CRead_Write_ParametersList;

class _CUST_DRV_DECL_ CAperture
{
public:
	CAperture();
	~CAperture();
	int SizeOf();
	void SetFocalLaw(CFocalLaw *pFocalLaw);
	void Desalloc();
	void Reset();
	bool operator==(const CAperture& p);
	bool operator!=(const CAperture& p);
	CAperture& operator=(const CAperture& p);
	DWORD& operator[](int iIndex);
	operator PDWORD();
	bool Merge(CAperture *pLsb,CAperture *pMsb);
	bool Split(CAperture *pLsb,CAperture *pMsb);
	void SetFocalLawElementCount();
	int GetApertureElementCount();
	int GetFirstElementIndex();
	int GetLastElementIndex();
	bool GetMultiplexerBoardIndex(int iModulo/*16 or 32*/,int iBufferSize,int *aiBuffer,int &iElementCount);
	bool GetListElement(int iBufferSize,int *aiBuffer,int &iElementCount);
	bool SetAllElementEnable(bool bEnable);
	bool SetElementEnable(int iElement,bool bEnable);
	bool GetAperture256(bool bMaster,DWORD *padwAperture);

	int GetAllocatedSize() const;
	bool SetAllocatedSize(int iSize);
	bool Write(myCFile &file);
	bool Read(myCFile &file);

	//to manage the scale
	void AllocTotal(const CAperture &a);
	CAperture* GetTotal();
private:
	DWORD *m_pdwData;
	int m_iApertureAllocatedSize;
	CFocalLaw *m_pFocalLaw;
	CAperture *m_pApertureTotal;
};

class _CUST_DRV_DECL_ CDDFTimeOfFlight
{
public:
	CDDFTimeOfFlight();
	~CDDFTimeOfFlight();
	int SizeOf();
	void SetFocalLaw(CFocalLaw *pFocalLaw);
	void Desalloc();
	void Reset();
	CDDFTimeOfFlight& operator=(const CDDFTimeOfFlight& p);
	double& operator[](int iIndex);
	operator double*();
	bool IsEqual(CDDFTimeOfFlight *p);

	int GetAllocatedSize() const;
	bool SetAllocatedSize(int iSize);
	bool Write(myCFile &file);
	bool Read(myCFile &file);
private:
	double *m_pdData;
	int m_iTofAllocatedSize;
	CFocalLaw *m_pFocalLaw;
};

class _CUST_DRV_DECL_ CApertureParameter
{
public:
	CApertureParameter();
	~CApertureParameter();
	int SizeOf();
	void SetFocalLaw(CFocalLaw *pFocalLaw);
	CFocalLaw *GetFocalLaw();
	void Desalloc();
	void Reset();
	CApertureParameter& operator=(const CApertureParameter& p);
	float& operator[](int iIndex);
	operator float*();
	bool Merge(CApertureParameter *pLsb,CApertureParameter *pMsb);
	bool Split(CAperture *pApRoot,CAperture *pApLsb,CAperture *pApMsb,CApertureParameter *pLsb,CApertureParameter *pMsb);
	bool RemoveElement(int iElement);//useful for dead element

	int GetAllocatedSize() const;
	bool SetAllocatedSize(int iSize);
	bool Write(myCFile &file);
	bool Read(myCFile &file);
private:
	float *m_pfData;
	int m_iParameterAllocatedSize;
	CFocalLaw *m_pFocalLaw;
};

class _CUST_DRV_DECL_ CDelayLaw
{
public:
	CDelayLaw();
	~CDelayLaw();
	int SizeOf();
	void SetFocalLaw(CFocalLaw *pFocalLaw);
	void Desalloc();
	void Reset();
	CDelayLaw& operator=(const CDelayLaw& p);
	float& operator[](int iIndex);
	float Get(int iIndex) const;
	bool Merge(CDelayLaw *pLsb,CDelayLaw *pMsb);
	bool Split(CAperture *pApRoot,CAperture *pApLsb,CAperture *pApMsb,CDelayLaw *pLsb,CDelayLaw *pMsb);
	bool RemoveElement(int iElement);//useful for dead element
	bool UpdateDelay(int iElement,float fDelay);
	bool ReorderDelay(int ElementCount,int *aiOrder);

	int GetAllocatedSize() const;
	bool SetAllocatedSize(int iSize);
	bool Write(myCFile &file);
	bool Read(myCFile &file);
private:
	float *m_pfDelayLaw;
	int m_iDelayAllocatedSize;
	CFocalLaw *m_pFocalLaw;
};

class _CUST_DRV_DECL_ CDDF
{
public:
	CDDF();//[g_iOEMPAFocalCountMax][g_iOEMPAApertureElementCountMax]
	~CDDF();
	int SizeOf();
	void SetFocalLaw(CFocalLaw *pFocalLaw);
	CFocalLaw *GetFocalLaw();
	void Desalloc();
	void Reset();
	CDDF& operator=(const CDDF& p);
	CDelayLaw& operator[](int iIndex);
	const CDelayLaw& Get(int iIndex) const;
	bool Merge(CDDF *pLsb,CDDF *pMsb);
	bool Split(CAperture *pApRoot,CAperture *pApLsb,CAperture *pApMsb,CDDF *pLsb,CDDF *pMsb);
	bool RemoveElement(int iElement);//useful for dead element
	bool UpdateDelay(int iElement,float fDelay);
	bool ReorderDelay(int ElementCount,int *piElement);

	void GetAllocatedSize(int &iDDF,int &iElement) const;
	bool SetAllocatedSize(int iDDF,int iElement);
	int GetAllocatedSize(int *piElement=NULL) const;
	bool SetAllocatedSize(int iSize);
	bool Write(myCFile &file);
	bool Read(myCFile &file);
private:
	CDelayLaw *m_pData;
	int m_iDDFAllocatedSize;
	CFocalLaw *m_pFocalLaw;
};

//////////////////////////////////////////////////////////////////

//UNITS FOR FOLLOWING STRUCTURE : Unit of International System (second, dB, etc...)
const int g_iFilterTitleLength=64;
typedef struct structFilter
{
	wchar_t pTitle[g_iFilterTitleLength];
	WORD wScale;
	short aCoefficient[g_iOEMPAFilterCoefficientMax];
}structFilter;
_CUST_DRV_DECL_ void OEMPA_ResetStructFilter(structFilter *paFilter);

#pragma warning( disable : 4091 )
typedef struct structRoot;
#pragma warning( default : 4091 )
typedef bool (WINAPI* pCallbackLockUnlock)(structRoot *pRoot,bool bLock,enumAcquisitionState eAcqState);

_CUST_DRV_DECL_ void GetSavedParameters(ULONGLONG &ullSavedParameters,UINT &uiSavedFilterCount);

typedef struct structRoot
{
public:
	_CUST_DRV_DECL_ structRoot();
	_CUST_DRV_DECL_ ~structRoot();
	//Configuration parameters : begin
	DWORD dwVersionLSB;
	DWORD dwVersionMSB;
	DWORD dwRootSize;
	ULONGLONG ullSavedParameters;//each bit is for one parameter.
	UINT uiSavedFilterCount;
	enumHardwareLink eDefaultHwLink;
	bool bEnableFMC;//OEMPA_ENABLE_FMC
	bool bEnableMultiHWChannel;//OEMPA_ENABLE_MULTI_CHANNEL
	bool bEnablePulserDuringReplay;
	bool bAscanEnable;//OEMPA_ASCAN_ENABLE
	bool bEnableCscanTof;//OEMPA_CSCAN_ENABLE_TOF
	enumTFMParameters eEnableTFM;
	enumBitSize eAscanBitSize;//OEMPA_ASCAN_BITSIZE
	enumAscanRequest eAscanRequest;//OEMPA_ASCAN_REQUEST
	double dAscanRequestFrequency;//OEMPA_ASCAN_REQUEST
	enumOEMPATrigger eTriggerMode;//OEMPA_TRIGGER_MODE
	enumOEMPAEncoderDirection eEncoderDirection;//OEMPA_ENCODER_TRIG_DIRECTION
	char cTemperatureWarning;//OEMPA_TEMPERATURE_WARNING should be >=0.
	char cTemperatureAlarm;//OEMPA_TEMPERATURE_ALARM should be >=0.
	double dTriggerEncoderStep;//OEMPA_TRIGGER_ENCODER_STEP
	enumOEMPARequestIO eRequestIO;//OEMPA_REQUESTIO
	int iRequestIODigitalInputMaskRising;//OEMPA_REQUESTIO_DIGITAL_INPUT_MASK
	int iRequestIODigitalInputMaskFalling;//OEMPA_REQUESTIO_DIGITAL_INPUT_MASK
	double dDebouncerEncoder;//OEMPA_DEBOUNCER_ENCODER
	double dDebouncerDigital;//OEMPA_DEBOUNCER_DIGITAL
	enumOEMPAMappingDigitalOutput eDigitalOuput[g_iOEMPAMappingDigitalOutputMax];//OEMPA_DIGITAL_OUTPUT_0, OEMPA_DIGITAL_OUTPUT_1 ... OEMPA_DIGITAL_OUTPUT_5
	long lSWEncoderResolution1;//OEMPA_SW_ENCODER1_RESOLUTION SW parameter
	long lSWEncoderResolution2;//OEMPA_SW_ENCODER2_RESOLUTION SW parameter
	DWORD dwSWEncoderDivider1;//OEMPA_SW_ENCODER1_DIVIDER SW parameter
	DWORD dwSWEncoderDivider2;//OEMPA_SW_ENCODER2_DIVIDER SW parameter
	enumDigitalInput eEncoder1A;//OEMPA_ENCODER1A
	enumDigitalInput eEncoder1B;//OEMPA_ENCODER1B
	enumDigitalInput eEncoder2A;//OEMPA_ENCODER2A
	enumDigitalInput eEncoder2B;//OEMPA_ENCODER2B
	enumDigitalInput eExternalTriggerCycle;//OEMPA_EXTERNAL_TRIGGER_CYCLE
	enumDigitalInput eExternalTriggerSequence;//OEMPA_EXTERNAL_TRIGGER_SEQUENCE
	enumEncoderType eEncoder1Type;//OEMPA_ENCODER1_TYPE
	enumEncoderType eEncoder2Type;//OEMPA_ENCODER2_TYPE
	enumKeepAlive eKeepAlive;//OEMPA_KEEPALIVE
	//Configuration parameters : end

	int iCycleCount;//-1 means HW cycles are not modified, otherwise the HW cycles are updated.
					//0 the "SetCycleCount" function is called with value 0.
					//1 the "SetCycleCount" function is called with value 1.
					//...
	structFilter aFilter[g_iEnumOEMPAFilterIndexLast];
	int iFMCElementStart;//FullMatrixCapture
	int iFMCElementStop;//FullMatrixCapture
	int iFMCElementStep;//FullMatrixCapture

	double dSpecimenVelocity;//view correction purpose (m/s)
	double dSpecimenRadius;//meter (if 0.0 it is a plane).
	double dSpecimenThickness;//meter (0.0 means not defined).
	double dDigitizingPeriod;//view correction purpose (second)
	int iOEMPAProbeCountMax;//maximum element count for the probe.
	int iOEMPAApertureCountMax;//should be 16 or 32, this is the required modulo for any available multiplexer. 
	int iOEMPADDFCountMax;//maximum focal point (should be smaller or equal to "g_iOEMPAFocalCountMax").
	BYTE bUSB3Disable;
	double dMultiHWChannelRangeMax;//OEMMC max range
	double dFWAscanRecoveryTime;//phased array and OEM-MC.
	double dTriggerHighTime;
	
	//sub-sequence table management
	int iSubSequenceEncoderCount;//size of aiSubSequenceCycleStart and aiSubSequenceCycleStop in case Position is used (size of afSubSequenceValue is one more).
	int iSubSequenceGateCount;//size of aiSubSequenceCycleStart and aiSubSequenceCycleStop in case Gate is used (size of afSubSequenceValue is the same).
	int aiSubSequenceCycleStart[g_iSubSequenceTableSize];
	int aiSubSequenceCycleStop[g_iSubSequenceTableSize];
	float afSubSequenceValue[g_iSubSequenceTableSize];//unit is either millimeter (Position) either second (Gate).
	int iSubSequenceAverage;

	enumReferenceTimeOfFlight eReferenceTimeOfFlight;

	bool b256Master;
	bool b256Slave;

	CHWDeviceOEMPA *pHWDeviceOEMPA;
	pCallbackLockUnlock pLockUnlock;
	wchar_t wcFileName[MAX_PATH];
	CRead_Write_ParametersList *rParamList;
}structRoot;
_CUST_DRV_DECL_ void OEMPA_InitRoot(structRoot &root,CHWDeviceOEMPA *pHWDeviceOEMPA);

typedef struct structGate
{
	bool bEnable;
	double dStart;//unit = second
	double dStop;//unit = second
	double dThreshold;//unit = percent
	enumRectification eRectification;
	enumGateModeAmp eModeAmp;
	enumGateModeTof eModeTof;
	WORD wAcqIDAmp;
	WORD wAcqIDTof;
	//tracking
	bool bTrackingStartEnable;
	int iTrackingStartIndexCycle;
	int iTrackingStartIndexGate;
	bool bTrackingStopEnable;
	int iTrackingStopIndexCycle;
	int iTrackingStopIndexGate;
} structGate;
_CUST_DRV_DECL_ void OEMPA_ResetStructGate(structGate *pGate);

typedef struct structCycle
{
	double dGainDigital;
	float fBeamCorrection;
	double dStart;//unit = second
	double dRange;//unit = second
	double dTimeSlot;//unit = second
	double dFMCSubTimeSlotAcq;//unit = second
	double dFMCSubTimeSlotReplay;//unit = second
	long lPointCount;
	long lPointFactor;//compression factor OR decimation factor (it depends of "eCompressionType").
	enumCompressionType eCompressionType;
	enumRectification eRectification;
	double adDACTof[g_iOEMPADACCountMax];//unit = second
	float afDACPrm[g_iOEMPADACCountMax];//unit = dB/second (in case of slope) and dB (in case of gain), see "bDACSlope" to know if it is slope or gain.
	int iDACCount;
	bool bDACSlope;
	bool bDACEnable;
	bool bDACAutoStop;
	bool bMaximum;
	bool bMinimum;
	bool bSaturation;
	bool bHWAcquisition;//true for ultrasound cycle and false for HW replay cycle.
	WORD wAcqIdChannelProbe;
	WORD wAcqIdChannelScan;
	WORD wAcqIdChannelCycle;
	float fGainAnalog;//unit = dB
	int iFilterIndex;//0=no filter, and range 1 to 15 to select one filter.
	//tracking: ascan and DAC
	bool bTrackingAscanEnable;
	int iTrackingAscanIndexCycle;
	int iTrackingAscanIndexGate;
	bool bTrackingDacEnable;
	int iTrackingDacIndexCycle;
	int iTrackingDacIndexGate;
	//gates
	int iGateCount;
	structGate gate[g_iOEMPAGateCountMax];
} structCycle;
_CUST_DRV_DECL_ structCycle* OEMPA_AllocCycle(int iCycleCount);
_CUST_DRV_DECL_ void OEMPA_ResetStructCycle(structCycle *pCycle);
_CUST_DRV_DECL_ void OEMPA_DesallocCycle(structCycle* &pCycle);

class _CUST_DRV_DECL_ CFocalLaw
{
public:
	CFocalLaw();
	CFocalLaw(int &iDDFCount,int &iElementCount);
	~CFocalLaw();
	int SizeOf();
	CFocalLaw& operator=(const CFocalLaw& p);

	double dWedgeDelay;//unit = second.
	int iElementCount;//count of element in the aperture.
	CAperture adwElement;//bit i : 0 if element is disabled, 1 if element is enabled. 2x32 = 64 elements. DWORD [g_iOEMPAApertureDWordCount]
	enumFocusing eFocusing;//to enable DDF.
	int iDelayCount;//count of useful delays = "iFocalCount" x "iElementCount".
	int iFocalCount;//count of focal law.
	CDDF afDelay;//unit = second								float [g_iOEMPAFocalCountMax][g_iOEMPAApertureElementCountMax]
	CDDFTimeOfFlight adFocalTimeOfFlight;//unit = second		double [g_iOEMPAFocalCountMax]
	float fCenterDelay;//delay for the aperture centroid.
	int iPrmCount;//useful item count in array "afPrm". Copy of "iElementCount".
	CApertureParameter afPrm;//gain (dB) or width (second)		float [g_iOEMPAApertureElementCountMax]
	double dSkew;//skew angle (local referential)
	double dAngle;//attach angle (local referential)
	double dX,dY,dZ;//emergence point (absolute referential)
	double dFocalX,dFocalY,dFocalZ;//last focal point (absolute referential)
	unionOEMMCDecimation hwAcqDecimation;

	CDelayLaw aTRLWedgeDelayRef;
	CDelayLaw aTRLWedgeDelayReal;

	void Desalloc();
	void Reset();
	void AllocDefault();
	bool SetAllocatedSize(int iDDFCount,int iElementCount);
	int GetCountDDF() const;
	int GetCountElement() const;
	int GetDWORDElementSize() const;
	void GetMultiChannelDecimation(int &iFirst,int &iCount,int aiDecimation[g_iOEMPAApertureElementCountMax]) const;
	bool GetMultiChannel(int &iStartFirst,int &iStartCount) const;
	bool Merge(CFocalLaw *pLsb,CFocalLaw *pMsb);
	bool Split(CFocalLaw *pLsb,CFocalLaw *pMsb);
	bool RemoveElement(int iElement);//useful for dead element
#ifdef _USE_MFC_
	bool Write(CFile &file);
	bool Read(CFile &file);
#else _USE_MFC_
	bool Write(myCFile &file);
	bool Read(myCFile &file);
#endif _USE_MFC_
	CFocalLaw *m_p256MatchedDevice;
private:
	int m_iLawDDFAllocatedSize;
	int m_iLawElementAllocatedElementSize;
	int m_iLawApertureAllocatedSize;
};
_CUST_DRV_DECL_ void OEMPA_ResetStructFocalLaw(CFocalLaw *pFocalLaw);
_CUST_DRV_DECL_ void OEMPA_ResetArrayFocalLaw(int iSize,CFocalLaw *pFocalLaw);
_CUST_DRV_DECL_ void OEMPA_DesallocFocalLaw(CFocalLaw* &pFocalLaw);
_CUST_DRV_DECL_ CFocalLaw* OEMPA_AllocFocalLaw(int iCycleCount);

