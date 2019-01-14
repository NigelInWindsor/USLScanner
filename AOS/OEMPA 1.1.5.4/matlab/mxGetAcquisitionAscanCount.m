function [count TotalByte] = mxGetAcquisitionAscanCount(deviceId)
% INPUT PARAMETER
%deviceId : identification number of the device.
% OUTPUT
%count: total ascan count that have been sent by the device after connection.

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

if nargout==2
    [count, TotalByte] = utCmd(deviceId,'mxGetAcquisitionAscanCount');
else
    [count] = utCmd(deviceId,'mxGetAcquisitionAscanCount');
    TotalByte = [];
end