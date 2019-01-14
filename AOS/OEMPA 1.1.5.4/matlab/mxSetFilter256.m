function ret=mxSetFilter256(masterDeviceId,slaveDeviceId,FilterIndex,Scale,FIRCoefficients,state)
% INPUT PARAMETER
%FilterIndex: Index of Filter to Set
%Scale: 
%FIRCoefficients: array of coefficients (34).
% OUTPUT
%return value (ret): 1 if no error and 0 if error.

if nargin<6
    state = 0;
end
    mxLockDevice256(masterDeviceId,slaveDeviceId,0);
    [ret] = mxSetFilter(masterDeviceId,FilterIndex,Scale,FIRCoefficients);
    if slaveDeviceId>=0
        [ret] = mxSetFilter(slaveDeviceId,FilterIndex,Scale,FIRCoefficients);
    end
    mxUnlockDevice256(masterDeviceId,slaveDeviceId,state);
