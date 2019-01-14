function [ret,setGain] = mxSetGainAnalog(deviceId,nCycle,gainValue)
% INPUT PARAMETER
%deviceId : identification number of the device.
%nCycle : cycle to changed
%gainValue : new Analog gain value
% OUTPUT
%return value (ret): 1 if no error and 0 if error.
%setGain: actual Digital Gain set by SetGainDigital

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

[ret,setGain] = utCmd(deviceId,'mxSetGainAnalog',[nCycle,gainValue]);