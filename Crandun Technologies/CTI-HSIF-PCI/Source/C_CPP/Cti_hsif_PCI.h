#ifndef CTI_HSIF_PCI_H_INCLUDED
#define CTI_HSIF_PCI_H_INCLUDED

/*

  Copyright (c) 2006 Crandun Technologies Inc.  All rights reserved.

  This file MAY NOT BE REDISTRIBUTED, in whole or in part,
  without the express written permission of Crandun Technologies Inc.

  ------------------------------------------------------------------------------
  Programmer's Notes:

  (1) Include this header file in any program using the 
	  Crandun Technologies Inc. library

  (2) In order for this header file to correctly define the DLL entry points and
      import them into your program,
	  the symbol CTI_HSIF_DLL_EXPORTS must NOT be defined
*/

#ifdef CTI_HSIF_PCI_DLL_EXPORTS
#define CTI_HSIF_PCI_DLL_API __declspec(dllexport)
#else
#define CTI_HSIF_PCI_DLL_API __declspec(dllimport)
#endif


#include "CTI_HSIFDataPt.h"		/* definition of HSIF_DATA_PT */
#include "CTI_ErrCodes.h"			/* Error codes */
#include "CTI_AR4000dataPt.h"		/* definition of AR4000_DATA_PT */


#ifdef __cplusplus

namespace Crandun {				// everything below lives in the namespace "Crandun"

class HSIF_PCI;					// fwd declare implementation class

// This class is exported from CTI_HSIF_PCI.dll

class CTI_HSIF_PCI_DLL_API CTI_HSIF_PCI {
public:
	CTI_HSIF_PCI(void);				// constructor
	virtual ~CTI_HSIF_PCI(void);	// destructor

	//================= Library Setup and Configuration Functions =====================

	long setDriverOpen();

	long setCalibrationFile(const char *filename);
	long getCalibrationFile(char * pFileName, const long buflen) const;

	long setEncoderCountsPerRev(const long encoderNum, const long encoderCountsPerRev);
	long getEncoderCountsPerRev(const long encoderNum) const;

	long setCommOpen(const char * serialPortName, const long initialBaudRate );
	long setCommClosed();
	long getIsCommOpen();

	long setBaudRate(const long baudRate);
	long getBaudRate() const;

	long setBufferSize(const long nSamples);		// set driver internal buffer size (in samples)
	long getBufferSize() const;
	long setClearBuffer();							// purge all samples from driver's buffer
	long getDidBufferOverflow() const;
	long setResetBufferOverflow();

	long setCallbackFunction(long fn(const HSIF_DATA_PT * pD1, const long N1, const HSIF_DATA_PT * pD2, const long N2));
	long setCallbackThreshold(const long nSamples);
	long getCallbackThreshold() const;
	long getIsBufferAtThreshold() const;	// returns 1 if number of samples in buffer is past the callback threshold

	long setMotorMaxRPM(const long motorNum, const long maxRPM);
	long getMotorMaxRPM(const long motorNum) const;

	// ================ Sensor Configuration Functions ========================
	long setFactoryDefaults();						// set to factory defaults
	long setResetHSIFBoard();

	long setAnalogOutputCalibrated();			// turn on calibrated sensor output on current loop
	long setAnalogOutputUnCalibrated();			// turn on uncalibrated sensor output on current loop [AR4000 only]

	long getIsAnalogOutputCalibrated() const;	// return true if analog output is calibrated, false otherwise
	long setAnalogOutputOff();					// turn off current loop output
	long getIsAnalogOutputOn() const;			// return true if analog output is on, false otherwise

	long setAnalogZeroCurrent(const long microAmp);	// set analog output current delivered at sensor zero point
	long getAnalogZeroCurrent() const;				// get currently set zero point analog output current

	long getHSIFBufSizeBytes();		// Returns size of the buffer, in bytes, or -1 for error
	long getHSIFBufSizeSamples();

	// Sets the motor power (0-63) & direction (0/1)
	long setMotorPower(const long motorNum, const long power, const long dir);
	long getMotorPower(const long motorNum) const;
	long getMotorDirection(const long motorNum) const;
	
	long setMotorRPM(const long motorNum, const long RPM, const long direction);
	long getMotorRPM(const long motorNum) const;

	long setSensorMaxRange(const long maxInches);
	long getSensorMaxRange() const;

	long setSpan(const long dist);					// set point at which current loop output is at maximum value
	long getSpan() const;							// get currently configured span setting

	long setTempHoldLevel(const long t);			// set/get AR4000 temperature hold level
	long getTempHoldLevel() const;

	long setZeroPt(const long dist);				// set zero point for (calibrated) output values
	long getZeroPt() const;							// get currently set (calibrated) zero point

	long setZeroPtUncalibrated(const long dist);	// set zero point for uncalibrated output values
	long getZeroPtUncalibrated() const;				// currently set uncalibrated zero point

	//============== Data Acquisition Functions ==================
	long setLaserOn();
	long setLaserOff();
	long getIsLaserOn() const;

	long setSampleInterval(const long microSecPerSample);	// set/get sampling interval in microseconds
	long getSampleInterval() const;

	long setSamplesPerSec(const long samplesPerSec);
	long getSamplesPerSec() const;

	long setContinuousHSIFOff();				// turn off continuous samples from HSIF card
	long setContinuousHSIFOn();					// turn on continuous samples from HSIF card
	long getIsContinuousHSIFOn() const;

	long getNumSamples() const;			// returns number of samples currently available in buffer

	long getSamples(HSIF_DATA_PT* pBuf, const long bufSizeSamples, const long minSamplesToTake, const long msWait);
	long getSamples2(const long minSamplesToTake, 
		const long msWait, 
		const long bufSizeSamples, 
		float* p_R_X, 
		float* p_A1_Y, 
		float* p_A2, 
		long*  p_rawE1, 
		long*  p_rawE2, 
		long*  p_amplitude, 
		long*  p_ambient, 
		float* p_temperature, 
		unsigned long* p_rawRange, 
		long* p_inputs); 

	long getSingleSample(float * dp);			// turn laser on, read one sample, then turn laser off
	long getExtSingleSample(AR4000_DATA_PT* dp);

	long getNumDataLost() const;	// number of samples skipped in HSIF card input stream
	long setResetDataLost();		// reset samples skipped counter

	//============ Data Format Functions ====================
	long setAngleOutputPolar();
	long setAngleOutputCartesian();
	long getIsAngleOutputPolar() const;

	long setOutputFormatEnglish();				// set output format to english units (inches)
	long setOutputFormatMetric();				// set output format to metric units (millimeters)
	long getIsOutputFormatEnglish() const;		// 1 if current output format is english

	//================= Data Filtering Functions =======================
	long setAngleOffset(const long encoderNum, const float offset); 
	float getAngleOffset(const long encoderNum) const;

	long setDiscardInvalidOn();
	long setDiscardInvalidOff();
	long getIsDiscardInvalidOn() const;

	long setMaxValidAmbient(const long amb);	// set ambient light level above which sample will be discarded
	long getMaxValidAmbient() const;			// get currently set maximum valid ambient light level

	long setMinValidAmbient(const long amb);	// set ambient light level below which sample will be discarded
	long getMinValidAmbient() const;			// get currently set minimum valid ambient light level

	long setMaxValidAmplitude(const long amp);	// set sample amplitude above which sample will be discarded
	long getMaxValidAmplitude() const;			// get currently set maximum valid amplitude

	long setMinValidAmplitude(const long amp);	// set sample amplitude below which sample will be discarded
	long getMinValidAmplitude() const;			// get currently set minimum valid amplitude

	long setMaxValidAngle(const long encoderNum, const float angle);
	float getMaxValidAngle(const long encoderNum) const;

	long setMinValidAngle(const long encoderNum, const float angle);
	float getMinValidAngle(const long encoderNum) const;

	long setMaxValidRange(const float rng);		// set range value above which sample will be discarded
	float getMaxValidRange() const;				// currently set maximum valid range value

	long setMinValidRange(const float rng);		// value below which sample will be discarded
	float getMinValidRange() const;				// currently set minimum valid range value

	long setRangeOffset(const float offset);	// offset to be added to all subsequent range measurements
	float getRangeOffset() const;				// offset to be added to all subsequent range measurements

	long setRangeScaleFactor(const float scale); 
	float getRangeScaleFactor() const;

	long setMaxValidTemp(const float temp);	// temperature above which sample will be discarded
	float getMaxValidTemp() const;			// currently set maximum valid temperature value	

	long setMinValidTemp(const float temp);	// temperature below which sample will be discarded
	float getMinValidTemp() const;			// currently set minimum valid temperature value	

	// ================ Error Handling and Miscellaneous Functions =====================
	long getIsError() const;
	long getErrorMessage(char * pErrMsg, const long strLen) const;
	long setClearError();

	long getFirmwareVersion(char * version, const long versionStringLen) const;	// returns sensor firmware version as string
	long getLibraryName(char * libName, const long stringLen) const;			// returns this software library version as string
	long getLibraryVersion() const;					// returns software library version as numeric
	
	long setDebugFileName(const char * szDebugFileName);
	void closeDebugFile();
	void printDebugSamples();

	long setClearHSIFBuffer();


private:
	CTI_HSIF_PCI (const CTI_HSIF_PCI &);				// don't permit copying
	CTI_HSIF_PCI& operator=(const CTI_HSIF_PCI &);		// or assignment
	HSIF_PCI * pSensor;
};

}	/* end of namespace Crandun */

#endif	/* end of ifdef cplusplus */

/* =====================================================================================*/
/* C interface declarations */
/* =====================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

	//======================== Driver Setup Functions =========================
	CTI_HSIF_PCI_DLL_API long __stdcall getNewCTIHSIF_PCI();
	CTI_HSIF_PCI_DLL_API long __stdcall setReleaseHandle(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setDriverOpen(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setCalibrationFile(const long sensorIndex, const char *filename);
	CTI_HSIF_PCI_DLL_API long __stdcall getCalibrationFile(const long sensorIndex, char * pFileName, const long buflen);

	CTI_HSIF_PCI_DLL_API long __stdcall setEncoderCountsPerRev(const long sensorIndex, const long encoderNum, const long encoderCountsPerRev);
	CTI_HSIF_PCI_DLL_API long __stdcall getEncoderCountsPerRev(const long sensorIndex, const long encoderNum);

	CTI_HSIF_PCI_DLL_API long __stdcall setCommOpen(const long sensorIndex, const char * comport, long initialBaudRate);
	CTI_HSIF_PCI_DLL_API long __stdcall setCommClosed(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall getIsCommOpen(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setBaudRate(const long sensorIndex, const long baudRate);
	CTI_HSIF_PCI_DLL_API long __stdcall getBaudRate(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setBufferSize(const long sensorIndex, const long nSamples);		// set driver internal buffer size (in samples)
	CTI_HSIF_PCI_DLL_API long __stdcall getBufferSize(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall setClearBuffer(const long sensorIndex);	// purge all samples from driver's buffer
	CTI_HSIF_PCI_DLL_API long __stdcall getDidBufferOverflow(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall setResetBufferOverflow(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setCallbackFunction(const long sensorIndex, long fn(const HSIF_DATA_PT * pD1, const long N1, const HSIF_DATA_PT * pD2, const long N2));
	CTI_HSIF_PCI_DLL_API long __stdcall setCallbackThreshold(const long sensorIndex, const long nSamples);
	CTI_HSIF_PCI_DLL_API long __stdcall getCallbackThreshold(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall getIsBufferAtThreshold(const long sensorIndex);	// returns 1 if number of samples in buffer is past the callback threshold

	CTI_HSIF_PCI_DLL_API long __stdcall setMotorMaxRPM(const long sensorIndex, const long motorNum, const long maxRPM);
	CTI_HSIF_PCI_DLL_API long __stdcall getMotorMaxRPM(const long sensorIndex, const long motorNum);

	// ================ Sensor Configuration Functions ========================

	CTI_HSIF_PCI_DLL_API long __stdcall setFactoryDefaults(const long sensorIndex);		// set to factory defaults
	CTI_HSIF_PCI_DLL_API long __stdcall setResetHSIFBoard(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setAnalogOutputCalibrated(const long sensorIndex);	// turn on calibrated sensor output on current loop
	CTI_HSIF_PCI_DLL_API long __stdcall setAnalogOutputUnCalibrated(const long sensorIndex);	// turn on uncalibrated sensor output on current loop [AR4000 only]

	CTI_HSIF_PCI_DLL_API long __stdcall getIsAnalogOutputCalibrated(const long sensorIndex); // return true if analog output is calibrated, false otherwise
	CTI_HSIF_PCI_DLL_API long __stdcall setAnalogOutputOff(const long sensorIndex);			 // turn off current loop output
	CTI_HSIF_PCI_DLL_API long __stdcall getIsAnalogOutputOn(const long sensorIndex);		 // return true if analog output is on, false otherwise

	CTI_HSIF_PCI_DLL_API long __stdcall setAnalogZeroCurrent(const long sensorIndex, const long microAmp);	// set analog output current delivered at sensor zero point
	CTI_HSIF_PCI_DLL_API long __stdcall getAnalogZeroCurrent(const long sensorIndex);		// get currently set zero point analog output current

	CTI_HSIF_PCI_DLL_API long __stdcall getHSIFBufSizeBytes(const long sensorIndex);		// Returns size of the buffer, in bytes, or -1 for error
	CTI_HSIF_PCI_DLL_API long __stdcall getHSIFBufSizeSamples(const long sensorIndex);

	// Sets the motor power (0-63) & direction (0/1)
	CTI_HSIF_PCI_DLL_API long __stdcall setMotorPower(const long sensorIndex, const long motorNum, const long power, const long dir);
	CTI_HSIF_PCI_DLL_API long __stdcall getMotorPower(const long sensorIndex, const long motorNum);
	CTI_HSIF_PCI_DLL_API long __stdcall getMotorDirection(const long sensorIndex, const long motorNum);

	CTI_HSIF_PCI_DLL_API long __stdcall setMotorRPM(const long sensorIndex, const long motorNum, const long RPM, const long direction);
	CTI_HSIF_PCI_DLL_API long __stdcall getMotorRPM(const long sensorIndex, const long motorNum);

	CTI_HSIF_PCI_DLL_API long __stdcall setSensorMaxRange(const long sensorIndex, const long maxInches);
	CTI_HSIF_PCI_DLL_API long __stdcall getSensorMaxRange(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setSpan(const long sensorIndex, const long dist);	// set point at which current loop output is at maximum value
	CTI_HSIF_PCI_DLL_API long __stdcall getSpan(const long sensorIndex);					// get currently configured span setting

	CTI_HSIF_PCI_DLL_API long __stdcall setTempHoldLevel(const long sensorIndex, const long t);			// set/get AR4000 temperature hold level
	CTI_HSIF_PCI_DLL_API long __stdcall getTempHoldLevel(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setZeroPt(const long sensorIndex, const long dist);	// set zero point for (calibrated) output values
	CTI_HSIF_PCI_DLL_API long __stdcall getZeroPt(const long sensorIndex);					// get currently set (calibrated) zero point

	CTI_HSIF_PCI_DLL_API long __stdcall setZeroPtUncalibrated(const long sensorIndex, const long dist);	// set zero point for uncalibrated output values
	CTI_HSIF_PCI_DLL_API long __stdcall getZeroPtUncalibrated(const long sensorIndex);		// currently set uncalibrated zero point


	//============== Data Acquisition Functions ==================

	CTI_HSIF_PCI_DLL_API long __stdcall setLaserOn(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall setLaserOff(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall getIsLaserOn(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setSampleInterval(const long sensorIndex, const long microSecPerSample);	// set/get sampling interval in microseconds
	CTI_HSIF_PCI_DLL_API long __stdcall getSampleInterval(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setSamplesPerSec(const long sensorIndex, const long samplesPerSec);
	CTI_HSIF_PCI_DLL_API long __stdcall getSamplesPerSec(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setContinuousHSIFOff(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall setContinuousHSIFOn(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall getIsContinuousHSIFOn(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall getNumSamples(const long sensorIndex);			// returns number of samples currently available in buffer

	CTI_HSIF_PCI_DLL_API long __stdcall getSamples(const long sensorIndex, HSIF_DATA_PT* pBuf, const long bufSizeSamples, const long minSamplesToTake, const long msWait);
	CTI_HSIF_PCI_DLL_API long __stdcall getSamples2(const long sensorIndex, 
			const long minSamplesToTake, 
			const long msWait, 
			const long bufSizeSamples, 
			float* p_R_X, 
			float* p_A1_Y, 
			float* p_A2, 
			long*  p_rawE1, 
			long*  p_rawE2, 
			long*  p_amplitude, 
			long*  p_ambient, 
			float* p_temperature, 
			unsigned long* p_rawRange, 
			long* p_inputs); 


	CTI_HSIF_PCI_DLL_API long __stdcall getSingleSample(const long sensorIndex, float * result);	// turn laser on, read one sample, then turn laser off
	CTI_HSIF_PCI_DLL_API long __stdcall getExtSingleSample(const long sensorIndex, AR4000_DATA_PT* result);

	CTI_HSIF_PCI_DLL_API long __stdcall getNumDataLost(const long sensorIndex);  // number of samples with 'data lost' flag set
	CTI_HSIF_PCI_DLL_API long __stdcall setResetDataLost(const long sensorIndex);

	//=============== Data Format Functions ====================
	CTI_HSIF_PCI_DLL_API long __stdcall setAngleOutputPolar(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall setAngleOutputCartesian(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall getIsAngleOutputPolar(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setOutputFormatEnglish(const long sensorIndex);		// set output format to english units (inches)
	CTI_HSIF_PCI_DLL_API long __stdcall setOutputFormatMetric(const long sensorIndex);		// set output format to metric units (millimeters)
	CTI_HSIF_PCI_DLL_API long __stdcall getIsOutputFormatEnglish(const long sensorIndex);		// 1 if current output format is english

	//================= Data Filtering Functions =======================
	CTI_HSIF_PCI_DLL_API long __stdcall  setAngleOffset(const long sensorIndex, const long encoderNum, const float offset); 
	CTI_HSIF_PCI_DLL_API float __stdcall getAngleOffset(const long sensorIndex, const long encoderNum);

	CTI_HSIF_PCI_DLL_API long __stdcall setDiscardInvalidOn(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall setDiscardInvalidOff(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall getIsDiscardInvalidOn(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall setMaxValidAmbient(const long sensorIndex, const long amb);	// set ambient light level above which sample will be discarded
	CTI_HSIF_PCI_DLL_API long __stdcall getMaxValidAmbient(const long sensorIndex);					// get currently set maximum valid ambient light level

	CTI_HSIF_PCI_DLL_API long __stdcall setMinValidAmbient(const long sensorIndex, const long amb);	// set ambient light level below which sample will be discarded
	CTI_HSIF_PCI_DLL_API long __stdcall getMinValidAmbient(const long sensorIndex);					// get currently set minimum valid ambient light level

	CTI_HSIF_PCI_DLL_API long __stdcall setMaxValidAmplitude(const long sensorIndex, const long amp);	// set sample amplitude above which sample will be discarded
	CTI_HSIF_PCI_DLL_API long __stdcall getMaxValidAmplitude(const long sensorIndex);					// get currently set maximum valid amplitude

	CTI_HSIF_PCI_DLL_API long __stdcall setMinValidAmplitude(const long sensorIndex, const long amp);	// set sample amplitude below which sample will be discarded
	CTI_HSIF_PCI_DLL_API long __stdcall getMinValidAmplitude(const long sensorIndex);					// get currently set minimum valid amplitude

	CTI_HSIF_PCI_DLL_API long  __stdcall setMaxValidAngle(const long sensorIndex, const long encoderNum, const float angle);
	CTI_HSIF_PCI_DLL_API float __stdcall getMaxValidAngle(const long sensorIndex, const long encoderNum);

	CTI_HSIF_PCI_DLL_API long  __stdcall setMinValidAngle(const long sensorIndex, const long encoderNum, const float angle);
	CTI_HSIF_PCI_DLL_API float __stdcall getMinValidAngle(const long sensorIndex, const long encoderNum);

	CTI_HSIF_PCI_DLL_API long  __stdcall setMaxValidRange(const long sensorIndex, const float rng);// set range value above which sample will be discarded
	CTI_HSIF_PCI_DLL_API float __stdcall getMaxValidRange(const long sensorIndex);				// currently set maximum valid range value

	CTI_HSIF_PCI_DLL_API long  __stdcall setMinValidRange(const long sensorIndex, const float rng);// value below which sample will be discarded
	CTI_HSIF_PCI_DLL_API float __stdcall getMinValidRange(const long sensorIndex);				// currently set minimum valid range value

	CTI_HSIF_PCI_DLL_API long  __stdcall setRangeOffset(const long sensorIndex, const float offset);	// offset to be added to all subsequent range measurements
	CTI_HSIF_PCI_DLL_API float __stdcall getRangeOffset(const long sensorIndex);						// offset to be added to all subsequent range measurements

	CTI_HSIF_PCI_DLL_API long  __stdcall setRangeScaleFactor(const long sensorIndex, const float scale); 
	CTI_HSIF_PCI_DLL_API float __stdcall getRangeScaleFactor(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long  __stdcall setMaxValidTemp(const long sensorIndex, const float temp);	// temperature above which sample will be discarded
	CTI_HSIF_PCI_DLL_API float __stdcall getMaxValidTemp(const long sensorIndex);					// currently set maximum valid temperature value	

	CTI_HSIF_PCI_DLL_API long  __stdcall setMinValidTemp(const long sensorIndex, const float temp);	// temperature below which sample will be discarded
	CTI_HSIF_PCI_DLL_API float __stdcall getMinValidTemp(const long sensorIndex);					// currently set minimum valid temperature value	

	// ================ Error Handling and Miscellaneous Functions =====================
	CTI_HSIF_PCI_DLL_API long __stdcall getIsError(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall getErrorMessage(const long sensorIndex, char * pErrMsg, const long strLen);
	CTI_HSIF_PCI_DLL_API long __stdcall setClearError(const long sensorIndex);

	CTI_HSIF_PCI_DLL_API long __stdcall  getFirmwareVersion(const long sensorIndex, char * version, const long versionStringLen);	// returns sensor firmware version as string
	CTI_HSIF_PCI_DLL_API long __stdcall  getLibraryName(const long sensorIndex, char * libName, const long stringLen);			// returns this software library version as string
	CTI_HSIF_PCI_DLL_API long __stdcall  getLibraryVersion(const long sensorIndex);						// returns software library version as numeric
	
	// Undocumented functions for logging debugging data
	CTI_HSIF_PCI_DLL_API long __stdcall  setDebugFileName(const long sensorIndex, const char * szDebugFileName);
	CTI_HSIF_PCI_DLL_API long __stdcall  closeDebugFile(const long sensorIndex);
	CTI_HSIF_PCI_DLL_API long __stdcall  printDebugSamples(const long sensorIndex);

	//============= Added for PCI ========================= 
	CTI_HSIF_PCI_DLL_API long __stdcall  setClearHSIFBuffer(const long sensorIndex);		// Reads & discard data in HW buffer & resets buffer overflow flag


#ifdef __cplusplus
}
#endif

#endif // CTI_HSIF_PCI_H_INCLUDED

