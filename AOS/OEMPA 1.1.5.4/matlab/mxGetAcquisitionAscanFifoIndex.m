function [FifoIndex, Cycle, Sequence, PointCount, ByteSize, Signed] = mxGetAcquisitionAscanFifoIndex(deviceId, cycles)
% INPUT PARAMETER
%deviceId : identification number of the device.
%cycles: cycle indexes.
% OUTPUTs
%The output result is a vector of the following 6 datas:
%- FifoIndex: index in the Fifo (-2 means missing cycle).
%  to be sure that the sequence is a whole sequence the following if can be
%  used:
%   if length(FifoIndex) && min(FifoIndex)>=0
%       Ascan = mxGetAcquisitionAscanFifoData(deviceId, FifoIndex);;get the whole sequence of data
%   else
%       %have to wait more data in the fifo so that the first sequence will
%       %be a whole sequence.
%   end
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
	[FifoIndex, Cycle, Sequence, PointCount, ByteSize, Signed] = utCmd(deviceId,'mxGetAcquisitionAscanFifoIndex', cycles);
elseif nargout>=5
	[FifoIndex, Cycle, Sequence, PointCount, ByteSize] = utCmd(deviceId,'mxGetAcquisitionAscanFifoIndex', cycles);
	Signed = [];
elseif nargout>=4
	[FifoIndex, Cycle, Sequence, PointCount] = utCmd(deviceId,'mxGetAcquisitionAscanFifoIndex', cycles);
	ByteSize = [];
	Signed = [];
elseif nargout>=3
	[FifoIndex, Cycle, Sequence] = utCmd(deviceId,'mxGetAcquisitionAscanFifoIndex', cycles);
	PointCount = [];
	ByteSize = [];
	Signed = [];
elseif nargout>=2
	[FifoIndex, Cycle] = utCmd(deviceId,'mxGetAcquisitionAscanFifoIndex', cycles);
	Sequence = [];
	PointCount = [];
	ByteSize = [];
	Signed = [];
elseif nargout>=1
	[FifoIndex] = utCmd(deviceId,'mxGetAcquisitionAscanFifoIndex', cycles);
	Cycle = [];
	Sequence = [];
	PointCount = [];
	ByteSize = [];
	Signed = [];
end