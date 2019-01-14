function [result] = mxReadFileWriteHW256(masterDeviceId,slaveDeviceId,filenameMaster,filenameSlave)
% INPUT PARAMETER
%deviceId : identification number of the device.
%filename : OEMPA file path.
% OUTPUT
%return value (result): 1 if the file have been loaded in the HW and 0 if error.

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
if(~ischar(filenameMaster))
    error('input should be a string');
end
if(~ischar(filenameSlave))
    error('input should be a string');
end

result = utCmd([masterDeviceId,slaveDeviceId],'mxReadFileWriteHW256',filenameMaster,filenameSlave);