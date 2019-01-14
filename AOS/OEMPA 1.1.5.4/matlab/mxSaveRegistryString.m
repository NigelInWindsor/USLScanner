function [result] = mxSaveRegistryString(section, key, value)

if(nargin<3)
    error('not enough input parameters');
end
if ischar(section)==0
    error('error: bad input parameter');
end
if ischar(key)==0
    error('error: bad input parameter');
end
if ischar(value)==0
    error('error: bad input parameter');
end
result = utCmd(-1, 'mxSaveRegistryString', section, key, value);
