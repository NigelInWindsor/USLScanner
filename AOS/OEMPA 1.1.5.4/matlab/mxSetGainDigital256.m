function ret=mxSetGainDigital256(masterDeviceId,slaveDeviceId,setup_Ncycle1, gainValue,state)
if nargin<5
    state = 0;
end
    mxLockDevice256(masterDeviceId,slaveDeviceId,0);
    for nCycle = 0:setup_Ncycle1-1,
        [ret,setGain] = mxSetGainDigital(masterDeviceId,nCycle,gainValue);
        if slaveDeviceId>=0
            [ret,setGain] = mxSetGainDigital(slaveDeviceId,nCycle,gainValue);
        end
    end
    mxUnlockDevice256(masterDeviceId,slaveDeviceId,state);
