function [ret] = mxSetToolboxDeviceAddress(strIPAddressAndPort)
% INPUT PARAMETER
%IPAddress and port.
% example1: '192.168.1.11'
% example2: '192.168.1.11:5001'
% OUTPUT
%return value (running): 1 if ok and 0 if error.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~ischar(strIPAddressAndPort))
    error('input should be a string');
end

ret = utCmd(-1,'mxSetToolboxDeviceAddress',strIPAddressAndPort);