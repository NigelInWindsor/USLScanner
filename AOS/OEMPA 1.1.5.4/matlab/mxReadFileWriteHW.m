function [result] = mxReadFileWriteHW(deviceId,filename)
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
if(~ischar(filename))
    error('input should be a string');
end

result = utCmd(deviceId,'mxReadFileWriteHW',filename);