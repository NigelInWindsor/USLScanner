function [ret] = mxToolboxEcho(dwData)
%This function post a value to the toolbox which is posted back to matlab (echo).
% INPUT PARAMETER
%dwData: new value to post to the toolbox.
% OUTPUT
%return value (ret): The returned value of this function is the last value posted back by the Toolbox.
%Example:
%mxToolboxEcho(4)
%   return 0    (this is the init value)
%mxToolboxEcho(78)
%   return 4    (previous)
%mxToolboxEcho(12)
%   return 78    (previous)
%mxToolboxEcho(92)
%   return 12    (previous)

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~isnumeric(dwData))
    error('input is not an integer');
end

ret = utCmd(-1,'mxToolboxEcho',dwData);