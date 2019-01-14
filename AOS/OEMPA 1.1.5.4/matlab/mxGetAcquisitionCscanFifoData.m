function [Amp, Tof, Cycle, Sequence, dEncoder, lEncoder] = mxGetAcquisitionCscanFifoData(deviceId, FifoIndex)
% INPUT PARAMETER
%deviceId : identification number of the device.
%FifoIndex: index of each data in the fifo.
% OUTPUTs
%Amp: amplitude (4 gates for each cycle).
%Tof: time of flight (4 gates for each cycle).
%Cycle: cycle index.
%Sequence: sequence index.
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

if nargout==6
    [Amp, Tof, Cycle, Sequence, dEncoder, lEncoder] = utCmd(deviceId,'mxGetAcquisitionCscanFifoData', FifoIndex);
elseif nargout==5
    [Amp, Tof, Cycle, Sequence, dEncoder] = utCmd(deviceId,'mxGetAcquisitionCscanFifoData', FifoIndex);
    lEncoder = [];
elseif nargout==4
    [Amp, Tof, Cycle, Sequence] = utCmd(deviceId,'mxGetAcquisitionCscanFifoData', FifoIndex);
    lEncoder = [];
    dEncoder = [];
elseif nargout==3
    [Amp, Tof, Cycle] = utCmd(deviceId,'mxGetAcquisitionCscanFifoData', FifoIndex);
    dEncoder = [];
    lEncoder = [];
    Sequence = [];
elseif nargout==2
    [Amp, Tof] = utCmd(deviceId,'mxGetAcquisitionCscanFifoData', FifoIndex);
    Cycle = [];
    dEncoder = [];
    lEncoder = [];
    Sequence = [];
elseif nargout==1
    [Amp] = utCmd(deviceId,'mxGetAcquisitionCscanFifoData', FifoIndex);
    Tof = [];
    Cycle = [];
    dEncoder = [];
    lEncoder = [];
    Sequence = [];
end