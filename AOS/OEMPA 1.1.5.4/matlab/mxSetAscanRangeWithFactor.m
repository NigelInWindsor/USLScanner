function [ret,setRange,compressionOut,pointCountOut,pointFactorOut] = mxSetAscanRangeWithFactor(deviceId,nCycle,rangeTime,compressionIn,pointFactorIn)
% INPUT PARAMETER
%deviceId : identification number of the device.
%nCycle : cycle to changed
%rangeTime : new range value in sec
%compressionType : 0 = compression, 1 = decimation
%pointFactor : sampling frequency is 100 MHz / pointFactor
%
%OUTPUT
%return value (ret): 1 if no error and 0 if error.
%setRange : actual Range value set by SetAscanRangeWithFactor
%compressionOut : type of compression being used:
%   0 - compression
%   1 - decimation
%pointCountOut : Number of points in A-scan
%pointFactorOut : (for compression) use every nth point where pointFactor = n

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if deviceId<0
    error('error: you have to create new device first');
end

[ret,setRange,compressionOut,pointCountOut,pointFactorOut] = utCmd(deviceId,'mxSetAscanRangeWithFactor',[nCycle,rangeTime,compressionIn,pointFactorIn]);