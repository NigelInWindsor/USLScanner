function [timeOffset, delayCorrectionMax] = mxPerformCalibration(deviceId)
% INPUT PARAMETER
%deviceId : identification number of the device.
% OUTPUT
%timeOffset: time offset adviced by the calibration process.
%delayCorrectionMax: delay max to align all elements.
%in case of error both 'timeOffset' and 'delayCorrectionMax' are null.

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
	[timeOffset, delayCorrectionMax] = utCmd(deviceId,'mxPerformCalibration');
else
	delayCorrectionMax = [];
	timeOffset = utCmd(deviceId,'mxPerformCalibration');
end