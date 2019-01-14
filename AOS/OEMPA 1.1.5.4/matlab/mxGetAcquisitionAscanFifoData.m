function [Ascan, Cycle, Sequence, dEncoder, lEncoder] = mxGetAcquisitionAscanFifoData(deviceId, FifoIndex)
% INPUT PARAMETER
%deviceId : identification number of the device.
%FifoIndex: index of each data in the fifo.
% OUTPUTs
%Ascan: ascan result. Assumption: all cycles should have the same point count otherwise no data will be returned.
%Cycle index.
%Sequence index.
%dEncoder: encoder (unit meter).
%lEncoder: raw encoder.

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

if nargout==5
    [Ascan, Cycle, Sequence, dEncoder, lEncoder] = utCmd(deviceId,'mxGetAcquisitionAscanFifoData', FifoIndex);
elseif nargout==4
    [Ascan, Cycle, Sequence, dEncoder] = utCmd(deviceId,'mxGetAcquisitionAscanFifoData', FifoIndex);
    lEncoder = [];
elseif nargout==3
    [Ascan, Cycle, Sequence] = utCmd(deviceId,'mxGetAcquisitionAscanFifoData', FifoIndex);
    dEncoder = [];
    lEncoder = [];
elseif nargout==2
    [Ascan, Cycle] = utCmd(deviceId,'mxGetAcquisitionAscanFifoData', FifoIndex);
    Sequence = [];
    dEncoder = [];
    lEncoder = [];
elseif nargout==1
    [Ascan] = utCmd(deviceId,'mxGetAcquisitionAscanFifoData', FifoIndex);
    Cycle = [];
    Sequence = [];
    dEncoder = [];
    lEncoder = [];
elseif nargout==0
    utCmd(deviceId,'mxGetAcquisitionAscanFifoData', FifoIndex);
    Ascan = [];
    Cycle = [];
    Sequence = [];
    dEncoder = [];
    lEncoder = [];
end