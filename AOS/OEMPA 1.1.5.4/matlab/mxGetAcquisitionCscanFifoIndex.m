function [FifoIndex, Cycle, Sequence] = mxGetAcquisitionCscanFifoIndex(deviceId, cycles)
% INPUT PARAMETER
%deviceId : identification number of the device.
%cycles: cycle indexes.
% OUTPUTs
%The output result is a vector of the following 3 datas:
%- FifoIndex: index in the Fifo (-2 means missing cycle).
%- Cycle: cycle index.
%- Sequence: sequence index.

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

if nargin<2
    cycleCount = mxGetSWCycleCount(deviceId);
	cycles = linspace(0,cycleCount-1,cycleCount);
end

if nargout>=3
	[FifoIndex, Cycle, Sequence] = utCmd(deviceId,'mxGetAcquisitionCscanFifoIndex', cycles);
elseif nargout>=2
	[FifoIndex, Cycle] = utCmd(deviceId,'mxGetAcquisitionCscanFifoIndex', cycles);
	Sequence = [];
elseif nargout>=1
	[FifoIndex] = utCmd(deviceId,'mxGetAcquisitionAscanFifoIndex', cycles);
	Cycle = [];
	Sequence = [];
end