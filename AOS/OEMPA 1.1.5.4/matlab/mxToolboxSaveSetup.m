function [ret] = mxToolboxSaveSetup(iDeviceId, filenameOut, filenameIn)
% INPUT PARAMETER
%deviceId : identification number of the device.
%filenameOut : output OEMPA file path.
%filenameIn : input OEMPA file path (to get GainDigital etc...).
% OUTPUT
%return value (ret): 1 if no error and 0 if error.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if nargin<2
    error('two input arguments required!');
end
if(~isnumeric(iDeviceId))
    error('input should be a string');
end
if(~ischar(filenameOut))
    error('input should be a string');
end

if(nargin<3)
    ret = utCmd(iDeviceId,'mxToolboxSaveSetup',filenameOut);
else
    if(~ischar(filenameIn))
        error('input should be a string');
    end
    ret = utCmd(iDeviceId,'mxToolboxSaveSetupEx',filenameIn,filenameOut);
end