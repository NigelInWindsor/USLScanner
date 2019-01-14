function [result] = mxGetStatus(deviceId)
% INPUT PARAMETER
%deviceId : integer returned by utCmdNewDevice
% OUTPUT
%return value (result): string, the SerialNumber of the device.

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

result = utCmd(deviceId,'mxGetStatus');