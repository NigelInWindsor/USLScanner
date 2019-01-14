function [ret,setRange,compression,pointCount,pointFactor] = mxSetAscanRange(deviceId,nCycle,rangeTime)
% INPUT PARAMETER
%deviceId : identification number of the device.
%nCycle : cycle to changed
%rangeTime : new range in sec
%
%OUTPUT
%return value (ret): 1 if no error and 0 if error.
%setRange : actual Range value set by SetAscanRange
%compression : type of compression being used:
%   0 - compression
%   1 - decimation
%pointCount : Number of points in A-scan
%pointFactor : (for compression) use every nth point where pointFactor = n

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

[ret,setRange,compression,pointCount,pointFactor] = utCmd(deviceId,'mxSetAscanRange',[nCycle,rangeTime]);