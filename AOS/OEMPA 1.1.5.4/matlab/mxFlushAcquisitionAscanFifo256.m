function [ret] = mxFlushAcquisitionAscanFifo256(masterDeviceId,slaveDeviceId)
% INPUT PARAMETER
%deviceId : identification number of the device.
% OUTPUTs
%ret: return 1 if no error and 0 in case of error.

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

[ret] = utCmd([masterDeviceId,slaveDeviceId],'mxFlushAcquisitionAscanFifo256');