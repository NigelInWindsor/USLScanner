function [result] = mxSetCfgKernelFile(strKey,dwValue)

if ischar(strKey)==0
    error('error: bad input#1 parameter');
end
if ischar(dwValue)==1
    error('error: bad input#2 parameter');
end

[result] = utCmd(-1,'mxSetCfgKernelFile',dwValue,strKey);