function [ret] = mxGetWizardBeam(iBeamIndex)
% INPUT PARAMETER
%iBeamIndex: index of the beam.
% OUTPUT
%return value (ret): 0 if error else the point:
%a =
%
%     1
%     2
%     3
%  (1,2,3) is Probe point (X,Y,Z)

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~isnumeric(iBeamIndex))
    error('input should be a integer');
end

ret = utCmd(-1,'mxGetWizardBeam',iBeamIndex);