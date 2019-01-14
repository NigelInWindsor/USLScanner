function [result] = mxResetCounters(deviceId)
% INPUT PARAMETER
%deviceId : integer returned by utCmdNewDevice
% OUTPUT
%return value (result): return 1 if the fifo is enabled or 0 if disabled.

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

result = utCmd(deviceId,'mxResetCounters');