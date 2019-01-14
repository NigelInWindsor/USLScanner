function ret=mxSetAscanRangeWithFactor256(masterDeviceId,slaveDeviceId,setup_Ncycle1, range, factor,state)

if nargin<6
    state = 0;
end
mxLockDevice256(masterDeviceId,slaveDeviceId,0);
    for nCycle = 0:setup_Ncycle1-1,
        [ret,setGain] = mxSetAscanRangeWithFactor(masterDeviceId,nCycle,range,1,factor);
        if slaveDeviceId>=0
            [ret,setGain] = mxSetAscanRangeWithFactor(slaveDeviceId,nCycle,range,1,factor);
        end
    end
    mxUnlockDevice256(masterDeviceId,slaveDeviceId,state);
