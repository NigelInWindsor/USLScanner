function ret=mxSetAscanStart256(masterDeviceId,slaveDeviceId,setup_Ncycle1, startValue,state)

if nargin<5
    state = 0;
end
    mxLockDevice256(masterDeviceId,slaveDeviceId,0);
    for nCycle = 0:setup_Ncycle1-1,
        [ret,setGain] = mxSetAscanStart(masterDeviceId,nCycle,startValue);
        if slaveDeviceId>=0
            [ret,setGain] = mxSetAscanStart(slaveDeviceId,nCycle,startValue);
        end
    end
    mxUnlockDevice256(masterDeviceId,slaveDeviceId,state);