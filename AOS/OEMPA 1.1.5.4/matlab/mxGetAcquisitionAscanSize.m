function [result] = mxGetAcquisitionAscanSize(deviceId,cycle)
% INPUT PARAMETER
%deviceId : identification number of the device.
%cycle : cycle index.
% OUTPUT
%return value (result): before enabling pulser this is the theoritical size of ascan
%			after enabling pulser the size of the last ascan that sent by the device.
%			size in bytes.

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

result = utCmd(deviceId,'mxGetAcquisitionAscanSize',cycle);