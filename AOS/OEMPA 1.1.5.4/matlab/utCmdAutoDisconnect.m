function [result] = utCmdAutoDisconnect(enable)
% INPUT PARAMETER
%enable : 1 or 0.
% OUTPUT
%return value (result): 0 (error) or 1 (no error).
%   ...

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~isnumeric(enable))
    error('input should be a string');
end

result = utCmd(-1,'mxAutoDisconnect',enable);