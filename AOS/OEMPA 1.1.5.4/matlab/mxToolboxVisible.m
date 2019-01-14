function [ret] = mxToolboxVisible(bEnable)
% INPUT PARAMETER
%bEnable: 1 to show the window and 0 to hide it.
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

ret = utCmd(-1,'mxToolboxVisible',bEnable);