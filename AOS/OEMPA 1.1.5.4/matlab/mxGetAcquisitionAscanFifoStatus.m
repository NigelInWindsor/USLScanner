function [AscanCount, AscanLost, TotalAscan, TotalByte] = mxGetAcquisitionAscanFifoStatus(deviceId)
% INPUT PARAMETER
%deviceId : identification number of the device.
% OUTPUTs
%AscanCount: ascan count that are still saved in the Fifo.
%AscanLost: ascan count that have been lost in the Fifo.
%TotalAscan: total ascan inputed.
%TotalByte: total byte inputed.

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

if nargout==4
    [AscanCount, AscanLost, TotalAscan, TotalByte] = utCmd(deviceId,'mxGetAcquisitionAscanFifoStatus');
else
if nargout==3
    [AscanCount, AscanLost, TotalAscan] = utCmd(deviceId,'mxGetAcquisitionAscanFifoStatus');
    TotalByte = [];
else
if nargout==2
    TotalAscan = [];
    TotalByte = [];
    [AscanCount, AscanLost] = utCmd(deviceId,'mxGetAcquisitionAscanFifoStatus');
else
    AscanLost = [];
    TotalAscan = [];
    TotalByte = [];
    [AscanCount] = utCmd(deviceId,'mxGetAcquisitionAscanFifoStatus');
end
end
end