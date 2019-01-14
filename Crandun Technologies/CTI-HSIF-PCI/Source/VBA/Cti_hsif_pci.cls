VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "CTI_HSIF_PCI"
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False



Option Explicit
Option Base 1

'  Copyright (c) 2001 Crandun Technologies Inc.  All rights reserved.
'
'  Programmer 's Notes:
'  Include this file as a Class Module in any Visual Basic program using the
'  Crandun Technologies Inc. library
'

Private instanceIndex As Long
'======================== Driver Setup Functions =========================
Private Declare Function p_getNewCTIHSIF_PCI Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getNewCTIHSIF_PCI@0" () As Long
Private Declare Function p_setReleaseHandle Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setReleaseHandle@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setDriverOpen Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setDriverOpen@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setCalibrationFile Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setCalibrationFile@8" (ByVal sensorIndex As Long, ByVal filename As String) As Long
Private Declare Function p_getCalibrationFile Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getCalibrationFile@12" (ByVal sensorIndex As Long, ByVal filename As String, buflen As Long) As Long

Private Declare Function p_setEncoderCountsPerRev Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setEncoderCountsPerRev@12" (ByVal sensorIndex As Long, ByVal encoderNum As Long, ByVal countsPerRev As Long) As Long
Private Declare Function p_getEncoderCountsPerRev Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getEncoderCountsPerRev@8" (ByVal sensorIndex As Long, ByVal encoderNum As Long) As Long


Private Declare Function p_setCommOpen Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setCommOpen@12" (ByVal sensorIndex As Long, ByVal commPortName As String, ByVal baudRate As Long) As Long
Private Declare Function p_getIsCommOpen Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getIsCommOpen@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setCommClosed Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setCommClosed@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setBaudRate Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setBaudRate@8" (ByVal sensorIndex As Long, ByVal baudRate As Long) As Long
Private Declare Function p_getBaudRate Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getBaudRate@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setBufferSize Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setBufferSize@8" (ByVal sensorIndex As Long, ByVal nSamples As Long) As Long
Private Declare Function p_getBufferSize Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getBufferSize@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setClearBuffer Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setClearBuffer@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getDidBufferOverflow Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getDidBufferOverflow@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setResetBufferOverflow Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setResetBufferOverflow@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setMotorMaxRPM Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMotorMaxRPM@12" (ByVal sensorIndex As Long, ByVal motorNum As Long, ByVal maxRPM As Long) As Long
Private Declare Function p_getMotorMaxRPM Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMotorMaxRPM@8" (ByVal sensorIndex As Long, ByVal motorNum As Long) As Long

' ================ Sensor Configuration Functions ========================
Private Declare Function p_setFactoryDefaults Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setFactoryDefaults@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setResetHSIFBoard Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setResetHSIFBoard@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setAnalogOutputCalibrated Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setAnalogOutputCalibrated@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setAnalogOutputUnCalibrated Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setAnalogOutputUnCalibrated@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getIsAnalogOutputCalibrated Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getIsAnalogOutputCalibrated@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setAnalogOutputOff Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setAnalogOutputOff@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getIsAnalogOutputOn Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getIsAnalogOutputOn@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setAnalogZeroCurrent Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setAnalogZeroCurrent@8" (ByVal sensorIndex As Long, ByVal microAmp As Long) As Long
Private Declare Function p_getAnalogZeroCurrent Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getAnalogZeroCurrent@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_getHSIFBufSizeBytes Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getHSIFBufSizeBytes@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getHSIFBufSizeSamples Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getHSIFBufSizeSamples@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setMotorPower Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMotorPower@16" (ByVal sensorIndex As Long, ByVal motorNum As Long, ByVal motorPower As Long, ByVal dir As Long) As Long
Private Declare Function p_getMotorPower Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMotorPower@8" (ByVal sensorIndex As Long, ByVal motorNum As Long) As Long
Private Declare Function p_getMotorDirection Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMotorDirection@8" (ByVal sensorIndex As Long, ByVal motorNum As Long) As Long

Private Declare Function p_setMotorRPM Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMotorRPM@16" (ByVal sensorIndex As Long, ByVal motorNum As Long, ByVal RPM As Long, ByVal dir As Long) As Long
Private Declare Function p_getMotorRPM Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMotorRPM@8" (ByVal sensorIndex As Long, ByVal motorNum As Long) As Long

Private Declare Function p_setSensorMaxRange Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setSensorMaxRange@8" (ByVal sensorIndex As Long, ByVal maxInches As Long) As Long
Private Declare Function p_getSensorMaxRange Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getSensorMaxRange@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setSpan Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setSpan@8" (ByVal sensorIndex As Long, ByVal dist As Long) As Long
Private Declare Function p_getSpan Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getSpan@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setTempHoldLevel Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setTempHoldLevel@8" (ByVal sensorIndex As Long, ByVal t As Long) As Long
Private Declare Function p_getTempHoldLevel Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getTempHoldLevel@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setZeroPt Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setZeroPt@8" (ByVal sensorIndex As Long, ByVal dist As Long) As Long
Private Declare Function p_getZeroPt Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getZeroPt@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setZeroPtUncalibrated Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setZeroPtUncalibrated@8" (ByVal sensorIndex As Long, ByVal dist As Long) As Long
Private Declare Function p_getZeroPtUncalibrated Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getZeroPtUncalibrated@4" (ByVal sensorIndex As Long) As Long

'============== Data Acquisition Functions ==================
Private Declare Function p_setLaserOn Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setLaserOn@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setLaserOff Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setLaserOff@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getIsLaserOn Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getIsLaserOn@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setSampleInterval Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setSampleInterval@8" (ByVal sensorIndex As Long, ByVal microSecPerSample As Long) As Long
Private Declare Function p_getSampleInterval Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getSampleInterval@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setSamplesPerSec Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setSamplesPerSec@8" (ByVal sensorIndex As Long, ByVal samplesPerSec As Long) As Long
Private Declare Function p_getSamplesPerSec Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getSamplesPerSec@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_getMaxSampleRate Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMaxSampleRate@8" (ByVal sensorIndex As Long, ByVal seconds As Long) As Long

Private Declare Function p_setContinuousHSIFOff Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setContinuousHSIFOff@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setContinuousHSIFOn Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setContinuousHSIFOn@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getIsContinuousHSIFOn Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getIsContinuousHSIFOn@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_getNumSamples Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getNumSamples@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_getSamples Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getSamples@20" (ByVal sensorIndex As Long, ByRef sampleArray As HSIF_DATA_PT, ByVal arraySize As Long, ByVal minSamplesToTake As Long, ByVal msWait As Long) As Long
Private Declare Function p_getSamples2 Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getSamples2@56" (ByVal sensorIndex As Long, _
                ByVal minSamples As Long, _
                ByVal msWait As Long, _
                ByVal bufSizeSamples As Long, _
                ByRef p_R_X As Single, _
                ByRef p_A1_Y As Single, _
                ByRef p_A2 As Single, _
                ByRef p_rawE1 As Long, _
                ByRef p_rawE2 As Long, _
                ByRef p_amplitude As Long, _
                ByRef p_ambient As Long, _
                ByRef p_temperature As Single, _
                ByRef p_rawRange As Long, _
                ByRef p_inputs As Long) As Long


Private Declare Function p_getSingleSample Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getSingleSample@8" (ByVal sensorIndex As Long, ByRef dataPt As Single) As Long
Private Declare Function p_getExtSingleSample Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getExtSingleSample@8" (ByVal sensorIndex As Long, ByRef extDataPt As HSIF_DATA_PT) As Long

Private Declare Function p_getNumDataLost Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getNumDataLost@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setResetDataLost Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setResetDataLost@4" (ByVal sensorIndex As Long) As Long

'============ Data Format Functions ====================
Private Declare Function p_setAngleOutputPolar Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setAngleOutputPolar@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setAngleOutputCartesian Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setAngleOutputCartesian@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getIsAngleOutputPolar Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getIsAngleOutputPolar@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setOutputFormatEnglish Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setOutputFormatEnglish@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setOutputFormatMetric Lib "CTI_HSIF_PCI_DLL.DLL" Alias "setOutputFormatMetric@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getIsOutputFormatEnglish Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getIsOutputFormatEnglish@4" (ByVal sensorIndex As Long) As Long

'================= Data Filtering Functions =======================
Private Declare Function p_setAngleOffset Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setAngleOffset@12" (ByVal sensorIndex As Long, ByVal encoderNum As Long, ByVal offset As Single) As Long
Private Declare Function p_getAngleOffset Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getAngleOffset@8" (ByVal sensorIndex As Long, ByVal encoderNum As Long) As Single


Private Declare Function p_setDiscardInvalidOn Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setDiscardInvalidOn@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_setDiscardInvalidOff Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setDiscardInvalidOff@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getIsDiscardInvalidOn Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getIsDiscardInvalidOn@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setMaxValidAmbient Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMaxValidAmbient@8" (ByVal sensorIndex As Long, ByVal amb As Long) As Long
Private Declare Function p_getMaxValidAmbient Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMaxValidAmbient@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setMinValidAmbient Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMinValidAmbient@8" (ByVal sensorIndex As Long, ByVal amb As Long) As Long
Private Declare Function p_getMinValidAmbient Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMinValidAmbient@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setMaxValidAmplitude Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMaxValidAmplitude@8" (ByVal sensorIndex As Long, ByVal amp As Long) As Long
Private Declare Function p_getMaxValidAmplitude Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMaxValidAmplitude@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_setMinValidAmplitude Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMinValidAmplitude@8" (ByVal sensorIndex As Long, ByVal amp As Long) As Long
Private Declare Function p_getMinValidAmplitude Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMinValidAmplitude@4" (ByVal sensorIndex As Long) As Long


Private Declare Function p_setMaxValidAngle Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMaxValidAngle@12" (ByVal sensorIndex As Long, ByVal encoderNum As Long, ByVal angle As Single) As Long
Private Declare Function p_getMaxValidAngle Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMaxValidAngle@4" (ByVal sensorIndex As Long, ByVal encoderNum As Long) As Single

Private Declare Function p_setMinValidAngle Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMinValidAngle@12" (ByVal sensorIndex As Long, ByVal encoderNum As Long, ByVal angle As Single) As Long
Private Declare Function p_getMinValidAngle Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMinValidAngle@4" (ByVal sensorIndex As Long, ByVal encoderNum As Long) As Single


Private Declare Function p_setMaxValidRange Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMaxValidRange@8" (ByVal sensorIndex As Long, ByVal rng As Single) As Long
Private Declare Function p_getMaxValidRange Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMaxValidRange@4" (ByVal sensorIndex As Long) As Single

Private Declare Function p_setMinValidRange Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMinValidRange@8" (ByVal sensorIndex As Long, ByVal rng As Single) As Long
Private Declare Function p_getMinValidRange Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMinValidRange@4" (ByVal sensorIndex As Long) As Single


Private Declare Function p_setRangeOffset Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setRangeOffset@8" (ByVal sensorIndex As Long, ByVal offset As Single) As Long
Private Declare Function p_getRangeOffset Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getRangeOffset@4" (ByVal sensorIndex As Long) As Single

Private Declare Function p_setRangeScaleFactor Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setRangeScaleFactor@8" (ByVal sensorIndex As Long, ByVal scaleFactor As Single) As Long
Private Declare Function p_getRangeScaleFactor Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getRangeScaleFactor@4" (ByVal sensorIndex As Long) As Single


Private Declare Function p_setMaxValidTemp Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMaxValidTemp@8" (ByVal sensorIndex As Long, ByVal Temp As Single) As Long
Private Declare Function p_getMaxValidTemp Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMaxValidTemp@4" (ByVal sensorIndex As Long) As Single

Private Declare Function p_setMinValidTemp Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setMinValidTemp@8" (ByVal sensorIndex As Long, ByVal Temp As Single) As Long
Private Declare Function p_getMinValidTemp Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getMinValidTemp@4" (ByVal sensorIndex As Long) As Single

' ================ Error Handling and Miscellaneous Functions =====================
Private Declare Function p_getIsError Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getIsError@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getErrorMessage Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getErrorMessage@12" (ByVal sensorIndex As Long, ByVal errMsg As String, ByVal strLen As Long) As Long
Private Declare Function p_setClearError Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_setClearError@4" (ByVal sensorIndex As Long) As Long

Private Declare Function p_getFirmwareVersion Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getFirmwareVersion@12" (ByVal sensorIndex As Long, ByVal firmwareVersion As String, ByVal strLen As Long) As Long
Private Declare Function p_getLibraryVersion Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getLibraryVersion@4" (ByVal sensorIndex As Long) As Long
Private Declare Function p_getLibraryName Lib "CTI_HSIF_PCI_DLL.DLL" Alias "_getLibraryName@12" (ByVal sensorIndex As Long, ByVal libName As String, ByVal strLen As Long) As Long

Private Sub Class_Initialize()
   instanceIndex = -1
   instanceIndex = p_getNewCTIHSIF_PCI()
End Sub

Private Sub Class_Terminate()
   p_setReleaseHandle (instanceIndex)
End Sub

'======================== Driver Setup Functions =========================
Public Function setDriverOpen() As Long
   setDriverOpen = p_setDriverOpen(instanceIndex)
End Function


Public Function setCalibrationFile(ByVal filename As String) As Long
   setCalibrationFile = p_setCalibrationFile(instanceIndex, filename)
End Function
Public Function getCalibrationFile(ByRef filename As String) As Long
   filename = "                                                  "
   '         12345678901234567890123456789012345678901234567890  '50 chars long
   filename = filename + filename   '100 chars long
   filename = filename + filename   '200 chars long
   filename = filename + filename   '400 chars long
   getCalibrationFile = p_getCalibrationFile(instanceIndex, filename, Len(filename))
End Function

Public Function setEncoderCountsPerRev(ByVal encoderNum As Long, ByVal countsPerRev As Long) As Long
   setEncoderCountsPerRev = p_setEncoderCountsPerRev(instanceIndex, encoderNum, countsPerRev)
End Function
Public Function getEncoderCountsPerRev(ByVal encoderNum As Long) As Long
   getEncoderCountsPerRev = p_getEncoderCountsPerRev(instanceIndex, encoderNum)
End Function



Public Function setCommOpen(ByVal serialPortName As String, ByVal initialBaudRate As Long) As Long
   setCommOpen = p_setCommOpen(instanceIndex, serialPortName, initialBaudRate)
End Function

Public Function getIsCommOpen() As Long
   getIsCommOpen = p_getIsCommOpen(instanceIndex)
End Function

Public Function setCommClosed() As Long
   setCommClosed = p_setCommClosed(instanceIndex)
End Function


Public Function setBaudRate(ByVal baudRate As Long) As Long
   setBaudRate = p_setBaudRate(instanceIndex, baudRate)
End Function
Public Function getBaudRate() As Long
   getBaudRate = p_getBaudRate(instanceIndex)
End Function


Public Function setBufferSize(ByVal nSamples As Long) As Long
   setBufferSize = p_setBufferSize(instanceIndex, nSamples)
End Function
Public Function getBufferSize() As Long
   getBufferSize = p_getBufferSize(instanceIndex)
End Function
Public Function setClearBuffer() As Long
   setClearBuffer = p_setClearBuffer(instanceIndex)
End Function
Public Function getDidBufferOverflow() As Long
   getDidBufferOverflow = p_getDidBufferOverflow(instanceIndex)
End Function
Public Function setResetBufferOverflow() As Long
   setResetBufferOverflow = p_setResetBufferOverflow(instanceIndex)
End Function


Public Function setMotorMaxRPM(ByVal motorNum As Long, ByVal maxRPM As Long) As Long
   setMotorMaxRPM = p_setMotorMaxRPM(instanceIndex, motorNum, maxRPM)
End Function
Public Function getMotorMaxRPM(ByVal motorNum As Long) As Long
   getMotorMaxRPM = p_getMotorMaxRPM(instanceIndex, motorNum)
End Function


' ================ Sensor Configuration Functions ========================
Public Function setFactoryDefaults() As Long
   setFactoryDefaults = p_setFactoryDefaults(instanceIndex)
End Function
Public Function setResetHSIFBoard() As Long
   setResetHSIFBoard = p_setResetHSIFBoard(instanceIndex)
End Function


Public Function setAnalogOutputCalibrated() As Long
   setAnalogOutputCalibrated = p_setAnalogOutputCalibrated(instanceIndex)
End Function

Public Function setAnalogOutputUnCalibrated() As Long
   setAnalogOutputUnCalibrated = p_setAnalogOutputUnCalibrated(instanceIndex)
End Function

Public Function getIsAnalogOutputCalibrated() As Long
   getIsAnalogOutputCalibrated = p_getIsAnalogOutputCalibrated(instanceIndex)
End Function

Public Function setAnalogOutputOff() As Long
   setAnalogOutputOff = p_setAnalogOutputOff(instanceIndex)
End Function

Public Function getIsAnalogOutputOn() As Long
   getIsAnalogOutputOn = p_getIsAnalogOutputOn(instanceIndex)
End Function


Public Function setAnalogZeroCurrent(ByVal microAmp As Long) As Long
   setAnalogZeroCurrent = p_setAnalogZeroCurrent(instanceIndex, microAmp)
End Function
Public Function getAnalogZeroCurrent() As Long
   getAnalogZeroCurrent = p_getAnalogZeroCurrent(instanceIndex)
End Function

Public Function getHSIFBufSizeBytes() As Long
   getHSIFBufSizeBytes = p_getHSIFBufSizeBytes(instanceIndex)
End Function
Public Function getHSIFBufSizeSamples() As Long
   getHSIFBufSizeSamples = p_getHSIFBufSizeSamples(instanceIndex)
End Function

Public Function setMotorPower(ByVal motorNum As Long, ByVal motorPower As Long, ByVal dir As Long) As Long
   setMotorPower = p_setMotorPower(instanceIndex, motorNum, motorPower, dir)
End Function
Public Function getMotorPower(ByVal motorNum As Long) As Long
   getMotorPower = p_getMotorPower(instanceIndex, motorNum)
End Function
Public Function getMotorDirection(ByVal motorNum As Long) As Long
   getMotorDirection = p_getMotorDirection(instanceIndex, motorNum)
End Function


Public Function setMotorRPM(ByVal motorNum As Long, ByVal RPM As Long, ByVal dir As Long) As Long
   setMotorRPM = p_setMotorRPM(instanceIndex, motorNum, RPM, dir)
End Function
Public Function getMotorRPM(ByVal motorNum As Long) As Long
   getMotorRPM = p_getMotorRPM(instanceIndex, motorNum)
End Function


Public Function setSensorMaxRange(ByVal maxInches As Long) As Long
   setSensorMaxRange = p_setSensorMaxRange(instanceIndex, maxInches)
End Function
Public Function getSensorMaxRange() As Long
   getSensorMaxRange = p_getSensorMaxRange(instanceIndex)
End Function


Public Function setSpan(ByVal dist As Long) As Long
   setSpan = p_setSpan(instanceIndex, dist)
End Function
Public Function getSpan() As Long
   getSpan = p_getSpan(instanceIndex)
End Function


Public Function setTempHoldLevel(ByVal temperature As Long) As Long
   setTempHoldLevel = p_setTempHoldLevel(instanceIndex, temperature)
End Function
Public Function getTempHoldLevel() As Long
   getTempHoldLevel = p_getTempHoldLevel(instanceIndex)
End Function


Public Function setZeroPt(ByVal dist As Long) As Long
   setZeroPt = p_setZeroPt(instanceIndex, dist)
End Function
Public Function getZeroPt() As Long
   getZeroPt = p_getZeroPt(instanceIndex)
End Function



Public Function setZeroPtUncalibrated(ByVal dist As Long) As Long
   setZeroPtUncalibrated = p_setZeroPtUncalibrated(instanceIndex, dist)
End Function
Public Function getZeroPtUncalibrated() As Long
   getZeroPtUncalibrated = p_getZeroPtUncalibrated(instanceIndex)
End Function


'============== Data Acquisition Functions ==================
Public Function setLaserOn() As Long
   setLaserOn = p_setLaserOn(instanceIndex)
End Function
Public Function setLaserOff() As Long
   setLaserOff = p_setLaserOff(instanceIndex)
End Function
Public Function getIsLaserOn() As Long
   getIsLaserOn = p_getIsLaserOn(instanceIndex)
End Function

Public Function setSampleInterval(ByVal microSecPerSample As Long) As Long
   setSampleInterval = p_setSampleInterval(instanceIndex, microSecPerSample)
End Function
Public Function getSampleInterval() As Long
   getSampleInterval = p_getSampleInterval(instanceIndex)
End Function


Public Function setSamplesPerSec(ByVal samplesPerSec As Long) As Long
   setSamplesPerSec = p_setSamplesPerSec(instanceIndex, samplesPerSec)
End Function
Public Function getSamplesPerSec() As Long
   getSamplesPerSec = p_getSamplesPerSec(instanceIndex)
End Function


Public Function getMaxSampleRate(ByVal seconds As Long) As Long
   getMaxSampleRate = p_getMaxSampleRate(instanceIndex, seconds)
End Function



Public Function setContinuousHSIFOff() As Long
   setContinuousHSIFOff = p_setContinuousHSIFOff(instanceIndex)
End Function
Public Function setContinuousHSIFOn() As Long
   setContinuousHSIFOn = p_setContinuousHSIFOn(instanceIndex)
End Function
Public Function getIsContinuousHSIFOn() As Long
   getIsContinuousHSIFOn = p_getIsContinuousHSIFOn(instanceIndex)
End Function



Public Function getNumSamples() As Long
   getNumSamples = p_getNumSamples(instanceIndex)
End Function

Public Function getSamples(result() As HSIF_DATA_PT, ByVal minSamples As Long, ByVal msWait As Long) As Long
   Dim numElements As Long
   Dim firstElement As Long

   firstElement = LBound(result)
   numElements = (UBound(result) - LBound(result) + 1)

   getSamples = p_getSamples(instanceIndex, result(firstElement), numElements, minSamples, msWait)
End Function

Public Function getSamples2(ByVal minSamples As Long, ByVal msWait As Long, _
                p_R_X() As Single, _
                p_A1_Y() As Single, _
                p_A2() As Single, _
                p_rawE1() As Long, _
                p_rawE2() As Long, _
                p_amplitude() As Long, _
                p_ambient() As Long, _
                p_temperature() As Single, _
                p_rawRange() As Long, _
                p_inputs() As Long) As Long
   Dim numElements As Long
   Dim firstElement As Long

   firstElement = LBound(p_R_X)
   numElements = (UBound(p_R_X) - LBound(p_R_X) + 1)

   getSamples2 = p_getSamples2(instanceIndex, minSamples, msWait, numElements, _
                    p_R_X(LBound(p_R_X)), _
                    p_A1_Y(LBound(p_A1_Y)), _
                    p_A2(LBound(p_A2)), _
                    p_rawE1(LBound(p_rawE1)), _
                    p_rawE2(LBound(p_rawE2)), _
                    p_amplitude(LBound(p_amplitude)), _
                    p_ambient(LBound(p_ambient)), _
                    p_temperature(LBound(p_temperature)), _
                    p_rawRange(LBound(p_rawRange)), _
                    p_inputs(LBound(p_inputs)))

End Function




Public Function getSingleSample(ByRef dataPt As Single) As Long
   getSingleSample = p_getSingleSample(instanceIndex, dataPt)
End Function

Public Function getExtSingleSample(ByRef dataPt() As HSIF_DATA_PT) As Long
   Dim firstElement As Long

   firstElement = LBound(dataPt)
   getExtSingleSample = p_getExtSingleSample(instanceIndex, dataPt(firstElement))
End Function


Public Function getNumDataLost() As Long
   getNumDataLost = p_getNumDataLost(instanceIndex)
End Function
Public Function setResetDataLost() As Long
   setResetDataLost = p_setResetDataLost(instanceIndex)
End Function


'============ Data Format Functions ====================
Public Function setAngleOutputPolar() As Long
   setAngleOutputPolar = p_setAngleOutputPolar(instanceIndex)
End Function
Public Function setAngleOutputCartesian() As Long
   setAngleOutputCartesian = p_setAngleOutputCartesian(instanceIndex)
End Function
Public Function getIsAngleOutputPolar() As Long
   getIsAngleOutputPolar = p_getIsAngleOutputPolar(instanceIndex)
End Function


Public Function setOutputFormatEnglish() As Long
   setOutputFormatEnglish = p_setOutputFormatEnglish(instanceIndex)
End Function
Public Function setOutputFormatMetric() As Long
   setOutputFormatMetric = p_setOutputFormatMetric(instanceIndex)
End Function
Public Function getIsOutputFormatEnglish() As Long
   getIsOutputFormatEnglish = p_getIsOutputFormatEnglish(instanceIndex)
End Function


'================= Data Filtering Functions =======================
Public Function setAngleOffset(ByVal encoderNum As Long, ByVal offset As Single) As Long
   setAngleOffset = p_setAngleOffset(instanceIndex, encoderNum, offset)
End Function

Public Function getAngleOffset(ByVal encoderNum As Long) As Single
   getAngleOffset = p_getAngleOffset(instanceIndex, encoderNum)
End Function

Public Function setDiscardInvalidOn() As Long
   setDiscardInvalidOn = p_setDiscardInvalidOn(instanceIndex)
End Function
Public Function setDiscardInvalidOff() As Long
   setDiscardInvalidOff = p_setDiscardInvalidOff(instanceIndex)
End Function
Public Function getIsDiscardInvalidOn() As Long
   getIsDiscardInvalidOn = p_getIsDiscardInvalidOn(instanceIndex)
End Function



Public Function setMaxValidAmbient(ByVal amb As Long) As Long
   setMaxValidAmbient = p_setMaxValidAmbient(instanceIndex, amb)
End Function
Public Function getMaxValidAmbient() As Long
   getMaxValidAmbient = p_getMaxValidAmbient(instanceIndex)
End Function
Public Function setMinValidAmbient(ByVal amb As Long) As Long
   setMinValidAmbient = p_setMinValidAmbient(instanceIndex, amb)
End Function
Public Function getMinValidAmbient() As Long
   getMinValidAmbient = p_getMinValidAmbient(instanceIndex)
End Function


Public Function setMaxValidAmplitude(ByVal amp As Long) As Long
   setMaxValidAmplitude = p_setMaxValidAmplitude(instanceIndex, amp)
End Function
Public Function getMaxValidAmplitude() As Long
   getMaxValidAmplitude = p_getMaxValidAmplitude(instanceIndex)
End Function
Public Function setMinValidAmplitude(ByVal amp As Long) As Long
   setMinValidAmplitude = p_setMinValidAmplitude(instanceIndex, amp)
End Function
Public Function getMinValidAmplitude() As Long
   getMinValidAmplitude = p_getMinValidAmplitude(instanceIndex)
End Function



Public Function setMaxValidAngle(ByVal encoderNum As Long, ByVal angle As Long) As Long
   setMaxValidAngle = p_setMaxValidAngle(instanceIndex, encoderNum, angle)
End Function
Public Function getMaxValidAngle(ByVal encoderNum As Long) As Single
   getMaxValidAngle = p_getMaxValidAngle(instanceIndex, encoderNum)
End Function
Public Function setMinValidAngle(ByVal encoderNum As Long, ByVal angle As Long) As Long
   setMinValidAngle = p_setMinValidAngle(instanceIndex, encoderNum, angle)
End Function
Public Function getMinValidAngle(ByVal encoderNum As Long) As Single
   getMinValidAngle = p_getMinValidAngle(instanceIndex, encoderNum)
End Function


Public Function setMaxValidRange(ByVal rng As Single) As Long
   setMaxValidRange = p_setMaxValidRange(instanceIndex, rng)
End Function
Public Function getMaxValidRange() As Single
   getMaxValidRange = p_getMaxValidRange(instanceIndex)
End Function
Public Function setMinValidRange(ByVal rng As Single) As Long
   setMinValidRange = p_setMinValidRange(instanceIndex, rng)
End Function
Public Function getMinValidRange() As Single
   getMinValidRange = p_getMinValidRange(instanceIndex)
End Function


Public Function setRangeOffset(ByVal offset As Single) As Long
   setRangeOffset = p_setRangeOffset(instanceIndex, offset)
End Function
Public Function getRangeOffset() As Single
   getRangeOffset = p_getRangeOffset(instanceIndex)
End Function
Public Function setRangeScaleFactor(ByVal scaleFactor As Single) As Long
   setRangeScaleFactor = p_setRangeScaleFactor(instanceIndex, scaleFactor)
End Function
Public Function getRangeScaleFactor() As Single
   getRangeScaleFactor = p_getRangeScaleFactor(instanceIndex)
End Function


Public Function setMaxValidTemp(ByVal Temp As Single) As Long
   setMaxValidTemp = p_setMaxValidTemp(instanceIndex, Temp)
End Function
Public Function getMaxValidTemp() As Single
   getMaxValidTemp = p_getMaxValidTemp(instanceIndex)
End Function
Public Function setMinValidTemp(ByVal Temp As Single) As Long
   setMinValidTemp = p_setMinValidTemp(instanceIndex, Temp)
End Function
Public Function getMinValidTemp() As Single
   getMinValidTemp = p_getMinValidTemp(instanceIndex)
End Function


' ================ Error Handling and Miscellaneous Functions =====================
Public Function getIsError() As Long
   getIsError = p_getIsError(instanceIndex)
End Function
Public Function getErrorMessage(ByRef errMsg As String) As Long
   Dim strLength As Long
   errMsg = "                                                  "
   '         12345678901234567890123456789012345678901234567890  '50 chars long
   errMsg = errMsg + errMsg   '100 chars long
   errMsg = errMsg + errMsg   '200 chars long
   errMsg = errMsg + errMsg   '400 chars long
   strLength = Len(errMsg)
   
   getErrorMessage = p_getErrorMessage(instanceIndex, errMsg, strLength)
End Function
Public Function setClearError() As Long
   setClearError = p_setClearError(instanceIndex)
End Function
Public Function getFirmwareVersion(ByRef verString As String) As Long
   verString = "                                                  "
   '            12345678901234567890123456789012345678901234567890  '50 chars long
   getFirmwareVersion = p_getFirmwareVersion(instanceIndex, verString, Len(verString))
End Function
Public Function getLibraryVersion() As Long
   getLibraryVersion = p_getLibraryVersion(instanceIndex)
End Function
Public Function getLibraryName(ByRef libName As String) As Long
   libName = "                                        "
   '          1234567890123456789012345678901234567890
   getLibraryName = p_getLibraryName(instanceIndex, libName, Len(libName))
End Function



