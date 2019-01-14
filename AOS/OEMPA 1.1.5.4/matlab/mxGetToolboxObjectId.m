function [ret] = mxGetToolboxObjectId(name)
% INPUT PARAMETER
%neam: Kernel object name.
% OUTPUT
%return value (ret): 1 if no error and 0 if error.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~ischar(name))
    error('input is not a string');
end

ret = utCmd(-1,'mxGetToolboxObjectId',name);