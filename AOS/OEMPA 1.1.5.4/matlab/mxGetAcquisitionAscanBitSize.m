function [size signed] = mxGetAcquisitionAscanBitSize(deviceId,cycle)
% INPUT PARAMETER
%deviceId : identification number of the device.
%cycle : cycle index.
% OUTPUT
%size: string, the data bit size of the last ascan that has been sent by the device.
%signed: string, the data bit size of the last ascan that has been sent by the device.

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

if (nargout==2)
    [size signed] = utCmd(deviceId,'mxGetAcquisitionAscanBitSize',cycle);
else
    size = utCmd(deviceId,'mxGetAcquisitionAscanBitSize',cycle);
    signed = '';
end