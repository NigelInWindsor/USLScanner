function [result] = mxGetLostCountUSB3(deviceId)
% INPUT PARAMETER
%deviceId : identification number of the device.
% OUTPUT
%return value (result): Number of Ascans lost.

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

[result] = utCmd(deviceId,'mxGetLostCountUSB3');