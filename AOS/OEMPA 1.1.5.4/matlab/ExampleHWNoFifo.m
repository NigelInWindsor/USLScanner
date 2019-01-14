%you can complete each step below one after the other
%from the matlab command line

cd 'C:\Program Files\AOS\OEMPA 1.1.5.4\matlab';%path to find matlab process
utCmdInit('C:\Program Files\AOS\OEMPA 1.1.5.4\UTKernelMatlab.dll')%load matlab stub dll
maxAscanCount = 1024;%if fifo is enabled this is the max ascan count in the fifo, otherwise this is the max cycle count.
maxAscanPointCount = 16384;%maximum ascan point count (with or without fifo).
deviceId=utCmdNewDevice('192.168.1.13', [maxAscanCount maxAscanPointCount 0]);%new device (specify IP address and options: max 1024 ascan in the fifo and max ascan size is 16384).
b=mxConnect(deviceId,1);%connect
if ~b
    error('mxConnect');
end
b=mxReadFileWriteHW(deviceId,'C:\ProgramData\AOS\OEMPA 1.1.5.4\Cfg\DefaultSetupOEMPA.txt');%load config file
if ~b
    error('mxReadFileWriteHW');
end
a0 = mxGetAcquisitionAscanCount(deviceId);%get the total ascan count received by the computer after connection, should be 0 because pulser are still disabled.
c0 = mxGetAcquisitionCscanCount(deviceId);%get cscan count, should be 0 because pulser are still disabled.
cyclecount=mxGetSWCycleCount(deviceId);%get the cycle count (from setting, value in the OEMPA file).
b=mxEnableShot(deviceId,1);%start pulse
if ~b
    error('mxEnableShot');
end
%wait a little
pause(1);
b=mxEnableShot(deviceId,0);%stop pulse
if ~b
    error('mxDisableShot');
end
a1 = mxGetAcquisitionAscanCount(deviceId);%get the total ascan count received by the computer after connection.
c1 = mxGetAcquisitionCscanCount(deviceId);%get cscan count
cycle = 0;%first cycle
Ascan = mxGetAcquisitionAscanData(deviceId,cycle);%get the first cycle ascan.
gateid = 0;%first gate
cscanamp = mxGetAcquisitionCscanAmplitude(deviceId,gateid);%get the first gate cscan amplitude (all cycles).
cscantof = mxGetAcquisitionCscanTimeOfFlight(deviceId,gateid);%get the first gate cscan time of fligth (all cycles).
plot(Ascan);%display ascan
b=mxConnect(deviceId,0);%disconnect
if ~b
    error('mxDisconnect');
end
mxDeleteDevice(deviceId);%delete device
utCmdExit;%free matlab stub dll

%mex utCmd.cpp;%to recompile the matlab process (REQUIRED ONLY ONE TIME), ADMINISTRATOR PRIVILEGE IS REQUIRED!
