function [dTimeSlotMin,dHWAcqSubTimeSlotMin,dReplayAcqSubTimeSlotMin,dReplayAcqSubTimeSlotOptimizedForThroughput] = mxGetFMCTimeLimitation(deviceId, fmcSubTCycleCount, ascanStart, ascanRange, timeSlot)
% INPUT PARAMETER
%deviceId : identification number of the device.
% OUTPUT
%return value (result): [ElementStart ElementStop] is FMC is enabled, dummy array otherwise.

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

[result] = utCmd(deviceId,'mxGetFMCTimeLimitation', [fmcSubTCycleCount ascanStart ascanRange timeSlot]);
dTimeSlotMin = result(1);
dHWAcqSubTimeSlotMin = result(2);
dReplayAcqSubTimeSlotMin = result(3);
dReplayAcqSubTimeSlotOptimizedForThroughput = result(4);