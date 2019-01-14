function ret = mxSetFilterIndex(deviceId,nCycle,FilterIndex)
% INPUT PARAMETER
%deviceId : identification number of the device.
%nCycle : cycle to changed
%FilterIndex : Index of Filter to be applied
% OUTPUT
%return value (ret): 1 if no error and 0 if error.

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
if ((FilterIndex < 0) || (FilterIndex > 15))
    error('error: invalid filter index')
end

ret = utCmd(deviceId,'mxSetFilterIndex',[nCycle,FilterIndex]);