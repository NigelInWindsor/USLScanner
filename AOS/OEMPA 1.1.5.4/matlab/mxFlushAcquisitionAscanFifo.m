function [ret] = mxFlushAcquisitionAscanFifo(deviceId)
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
if deviceId<0
    error('error: you have to create new device first');
end

[ret] = utCmd(deviceId,'mxFlushAcquisitionAscanFifo');