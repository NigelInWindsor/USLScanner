function [deviceId] = utCmdNewDevice(IPAddress, option)
% INPUT PARAMETER
%IPAddress : string example '192.168.1.11'
%option : vector of 3 doubles, the first is the maximum cycle count value without Fifo
%		OR the maximum ascan count in the Fifo,
%       the second is the maximum ascan point count (with or without Fifo).
%       Be carefull in case the fifo is disabled a buffer is allocated
%       The size of this buffer is "maximum cycle count value" multiplied
%       by "maximum ascan point count".
%		The third double is 0.0 (false) or 1.0 (true) to enable or disable the output fifo.
%       By default the fifo is enabled, its buffer size is 64 MBytes, that
%       is for example:
%           ascan count max 4096 and max point count is 16384
%           ascan count max 1024 and max point count is 65536
% OUTPUT
%return value (deviceId): identification number of the device
%that should be used for further access to the device
%   deviceId=0  => first device
%   deviceId=1  => second device
%   ...

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~ischar(IPAddress))
    error('input should be a string');
end
if nargin>1
    [isize, jsize] = size(option);
    if (isize==1) && (jsize==3)
        fifoEnable = option(3);
        if(strcmp(computer, 'PCWIN')>0) && fifoEnable
            error('The fifo should be disabled for 32 bits computer!');
            fifoEnable = 0;%if fifo is disabled only one sequence is saved by the driver otherwise many.
        end
    end
end
if nargin<2
    deviceId = utCmd(-1,'mxNewDevice',IPAddress);
else
    if length(option)>3
        error('input size should be a vector of 2 doubles');
    end
    deviceId = utCmd(-1, 'mxNewDevice', option, IPAddress);
end