function [AscanRecoveryTime,FMCSubCycleRecoveryTime,FMCCycleRecoveryTime] = mxGetRecoveryTimes(deviceId)
% INPUT PARAMETER
%deviceId : identification number of the device.
% OUTPUTs
%returned values : unit is second.

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

[result] = utCmd(deviceId,'mxGetRecoveryTimes');
AscanRecoveryTime = result(1);
FMCSubCycleRecoveryTime = result(2);
FMCCycleRecoveryTime = result(3);