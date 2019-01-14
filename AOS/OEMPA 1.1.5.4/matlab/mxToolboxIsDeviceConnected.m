function [ret] = mxToolboxIsDeviceConnected()
% INPUT PARAMETER
%nonne.
% OUTPUT
%return value (running): 1 if ok and 0 if error.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end

ret = utCmd(-1,'mxToolboxIsDeviceConnected');