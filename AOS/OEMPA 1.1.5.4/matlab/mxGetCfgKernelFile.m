function [result] = mxGetCfgKernelFile(strKey)

if ischar(strKey)==0
    error('error: bad input parameter');
end

[result] = utCmd(-1,'mxGetCfgKernelFile',strKey);