function [ret] = mxToolboxDeviceConnection(bConnect)
% INPUT PARAMETER
%none.
% OUTPUT
%return value (running): 1 if ok and 0 if error.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~isnumeric(bConnect))
    error('input should be a string');
end

ret = utCmd(-1,'mxToolboxDeviceConnection',bConnect);