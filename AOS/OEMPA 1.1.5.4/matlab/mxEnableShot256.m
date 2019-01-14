function [result] = mxEnableShot256(masterDeviceId,slaveDeviceId,enable)
% INPUT PARAMETER
%deviceId : identification number of the device.
%enable : 1 toenable pulser and 0 to disable them.
% OUTPUT
%return value (result): 1 if success and 0 if error.

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
if slaveDeviceId==masterDeviceId
    error('error: Master and Slave must be different devices');
end

result = utCmd([masterDeviceId,slaveDeviceId],'mxEnableShot256',enable);