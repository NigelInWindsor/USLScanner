function [ret] = mxToolboxDisplay(iObjectRootId)
% INPUT PARAMETER
%iObjectRootId: id of the Kernel object.
% OUTPUT
%return value (ret): 1 if no error and 0 if error.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~isnumeric(iObjectRootId))
    error('input is not an integer');
end

ret = utCmd(-1,'mxToolboxDisplay',iObjectRootId);