function [result] = mxGetAcquisitionCscanTimeOfFlight(deviceId,gateId)
% INPUT PARAMETER
%deviceId : identification number of the device.
%gateId : identification number of the gate (range 0..3).
% OUTPUT
%return value (result): TimeOfFlight cscan of all cycles sent by the
%device.

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

result = utCmd(deviceId,'mxGetAcquisitionCscanTimeOfFlight',gateId);