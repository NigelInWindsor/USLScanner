function [result] = mxExample1(deviceId)
% INPUT PARAMETER
%deviceId : identification number of the device.
% OUTPUT
%return value (result): the last ascan that have been sent by the device,
%this is an array (vector) that you can disply to see the ascan.

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

result = utCmd(deviceId,'mxExample1');