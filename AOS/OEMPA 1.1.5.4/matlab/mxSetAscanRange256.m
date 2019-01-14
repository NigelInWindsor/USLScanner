function ret=mxSetAscanRange256(masterDeviceId,slaveDeviceId,setup_Ncycle1,rangeValue,state)

if nargin<5
    state = 0;
end
    mxLockDevice256(masterDeviceId,slaveDeviceId,0);
    for nCycle = 0:setup_Ncycle1-1,
        [ret,setGain] = mxSetAscanRange(masterDeviceId,nCycle,rangeValue);
        if slaveDeviceId>=0
            [ret,setGain] = mxSetAscanRange(slaveDeviceId,nCycle,rangeValue);
        end
    end
    mxUnlockDevice256(masterDeviceId,slaveDeviceId,state);
