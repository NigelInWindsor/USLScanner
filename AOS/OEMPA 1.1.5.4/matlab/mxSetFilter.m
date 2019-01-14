function ret = mxSetFilter(deviceId,FilterIndex,Scale,FIRCoefficients)
% INPUT PARAMETER
%deviceId : identification number of the device.
%FilterIndex: Index of Filter to Set
%Scale: 
%FilterIndex : Index of Filter to be applied
% OUTPUT
%return value (ret): 1 if no error and 0 if error.

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
if length(FIRCoefficients)~=34
    error('error: you must provide 34 filter coefficients');
end
if FIRCoefficients(1) ~= 0
    error('error: invalid coefficient list; first value must be 0')
end
if ((FilterIndex < 1) || (FilterIndex > 15))
    error('error: invalid filter index')
end

ret = utCmd(deviceId,'mxSetFilter',[FilterIndex,Scale,FIRCoefficients]);