function [result] = mxResetCounters256(masterDeviceId,slaveDeviceId)
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
if masterDeviceId<0
    error('error: you have to create new master device first');
end
if slaveDeviceId<0
    error('error: you have to create new slave device first');
end
if slaveDeviceId==masterDeviceId
    error('error: Master and Slave must be different devices');
end

result = utCmd([masterDeviceId,slaveDeviceId],'mxResetCounters256');