function [ret] = mxToolboxSaveWizard(filename,bSaveWizardOutputs)
% INPUT PARAMETER
%filename : file to save.
%bSaveWizardOutputs: 1 to save the outputs of the wizard and 0 otherwise (save only inputs).
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
if nargin<2
    bSaveWizardOutputs = 0;
end
if(~isnumeric(bSaveWizardOutputs))
    error('input should be a string');
end

ret = utCmd(-1,'mxToolboxSaveWizard',bSaveWizardOutputs,filename);