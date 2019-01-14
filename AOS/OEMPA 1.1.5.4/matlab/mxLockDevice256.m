function [result] = mxLockDevice256(masterDeviceId,slaveDeviceId,acqState)
% INPUT PARAMETER
%masterDeviceId : integer returned by utCmdNewDevice for Master Device
%slaveDeviceId : integer returned by utCmdNewDevice for Slave Device
%acqState : 0 = eAcqOff, 1 = eAcqDefault (Normal Usage), 2 = eAcqOn
% OUTPUT
%return value (result): return 1 if success and 0 if failed.

global g_Init;

if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if masterDeviceId<0
    error('error: you have to create new master device first');
end
if slaveDeviceId==masterDeviceId
    error('error: Master and Slave must be different devices');
end

result = utCmd([masterDeviceId,slaveDeviceId],'mxLockDevice256',acqState);