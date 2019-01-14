function [FifoIndex, Cycle, Sequence, PointCount, ByteSize, Signed] = mxGetAcquisitionAscanLifoIndex(deviceId, cycles)
% INPUT PARAMETER
%deviceId : identification number of the device.
%cycles: cycle indexes.
% OUTPUTs
%The output result is a vector of the following 3 datas:
%- FifoIndex: index in the Fifo (-2 means missing cycle).
%- Cycle: cycle index.
%- Sequence: sequence index.
%- PointCount: ascan point count.
%- ByteSize: byte size of one ascan data.
%- Signed: 1 in case of signed data and 0 for unsigned data.

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

if nargout>=6
	[FifoIndex, Cycle, Sequence, PointCount, ByteSize, Signed] = utCmd(deviceId,'mxGetAcquisitionAscanLifoIndex', cycles);
elseif nargout>=5
	[FifoIndex, Cycle, Sequence, PointCount, ByteSize] = utCmd(deviceId,'mxGetAcquisitionAscanLifoIndex', cycles);
	Signed = [];
elseif nargout>=4
	[FifoIndex, Cycle, Sequence, PointCount] = utCmd(deviceId,'mxGetAcquisitionAscanLifoIndex', cycles);
	ByteSize = [];
	Signed = [];
elseif nargout>=3
	[FifoIndex, Cycle, Sequence] = utCmd(deviceId,'mxGetAcquisitionAscanLifoIndex', cycles);
	PointCount = [];
	ByteSize = [];
	Signed = [];
elseif nargout>=2
	[FifoIndex, Cycle] = utCmd(deviceId,'mxGetAcquisitionAscanLifoIndex', cycles);
	Sequence = [];
	PointCount = [];
	ByteSize = [];
	Signed = [];
elseif nargout>=1
	[FifoIndex] = utCmd(deviceId,'mxGetAcquisitionAscanLifoIndex', cycles);
	Cycle = [];
	Sequence = [];
	PointCount = [];
	ByteSize = [];
	Signed = [];
end