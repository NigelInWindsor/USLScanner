function [result] = mxEnableUsb3(deviceId, enable)
% INPUT PARAMETER
%deviceId : integer returned by utCmdNewDevice
%enable: 1 to enable it, 0 to disable it.
% OUTPUT
%return value (result): return 1 if connected and 0 if disconnected.

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

result = utCmd(deviceId,'mxEnableUsb3', enable);