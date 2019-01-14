function [running] = mxToolboxRun(bShow)
% INPUT PARAMETER
%1 to display the window and 0 to hide it.
% OUTPUT
%return value (running): boolean 1 if OK and 0 if error.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~isnumeric(bShow))
    error('input should be a number');
end
if(length(bShow)~=1)
    error('input length should be 1');
end

running = utCmd(-1,'mxToolboxRun',bShow);