function [result] = mxResetEncoder(deviceId)
% INPUT PARAMETER
%deviceId : integer returned by utCmdNewDevice
% OUTPUT
%return value (result): return 1 if no error otherwise 0.

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

result = utCmd(deviceId,'mxResetEncoder');