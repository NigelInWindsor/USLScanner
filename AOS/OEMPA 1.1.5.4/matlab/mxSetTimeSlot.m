function [result] = mxSetTimeSlot(deviceId, cycle, timeSlot)
% INPUT PARAMETER
%deviceId : identification number of the device.
%You have to call "LockDevice"/"UnlockDevice" first.
% OUTPUT
%return value (result): 1 if no error, 0 otherwise.

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

[result] = utCmd(deviceId,'mxSetTimeSlot', [cycle timeSlot]);
