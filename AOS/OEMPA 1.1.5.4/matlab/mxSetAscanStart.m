function [ret,setGain] = mxSetAscanStart(deviceId,nCycle,startValue)
% INPUT PARAMETER
%deviceId : identification number of the device.
%nCycle : cycle to changed
%startValue : new start in seconds
% OUTPUT
%return value (ret): 1 if no error and 0 if error.
%setStart: actual Ascan start in seconds.

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

[ret,setGain] = utCmd(deviceId,'mxSetAscanStart',[nCycle,startValue]);