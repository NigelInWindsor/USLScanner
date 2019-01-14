function [ret] = mxToolboxLoadWizard(filename)
% INPUT PARAMETER
%filename : file to load.
% OUTPUT
%return value (ret): 1 if no error and 0 if error.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~ischar(filename))
    error('input should be a string');
end

ret = utCmd(-1,'mxToolboxLoadWizard',filename);