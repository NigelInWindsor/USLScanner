function [ret] = mxToolboxWizardUpdateScan()
% INPUT PARAMETER
%non.
% OUTPUT
%return value (ret): 0 if error else 1.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end

ret = utCmd(-1,'mxToolboxWizardUpdateScan');