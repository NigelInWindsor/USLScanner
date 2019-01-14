function [ret] = mxEnableEventUpdate(bEnable)
% INPUT PARAMETER
%bEnable: 1 to enable the event update and 0 to disable it.
% OUTPUT
%return value (ret): 1 if no error and 0 if error.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~isnumeric(bEnable))
    error('input is not an integer');
end

ret = utCmd(-1,'EnableEventUpdate',bEnable);