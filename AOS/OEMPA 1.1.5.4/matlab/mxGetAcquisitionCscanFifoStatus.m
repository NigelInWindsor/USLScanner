function [AscanCount, AscanLost, perf] = mxGetAcquisitionCscanFifoStatus(deviceId)
% INPUT PARAMETER
%deviceId : identification number of the device.
% OUTPUTs
%AscanCount: cscan count that are still saved in the Fifo.
%AscanLost: cscan count that have been lost in the Fifo.

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

if nargout==2
    [AscanCount, AscanLost] = utCmd(deviceId,'mxGetAcquisitionCscanFifoStatus');
else
    AscanLost = [];
    [AscanCount] = utCmd(deviceId,'mxGetAcquisitionCscanFifoStatus');
end