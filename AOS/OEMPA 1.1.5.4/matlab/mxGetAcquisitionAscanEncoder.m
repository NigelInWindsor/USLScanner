function [encoder,raw] = mxGetAcquisitionAscanEncoder(deviceId,iEncoderIndex)
% INPUT PARAMETER
%deviceId : identification number of the device.
%iEncoderIndex : identification number of the encoder (0 or 1).
% OUTPUT
%return value (encoder): ascan encoder that have been sent by the device.
%return value (raw): ascan encoder (resolution is not taken into account).

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

[encoder,raw] = utCmd(deviceId,'mxGetAcquisitionAscanEncoder',iEncoderIndex);