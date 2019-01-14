%you can complete each step below one after the other
%from the matlab command line

cd 'C:\Program Files\AOS\OEMPA 1.1.5.4\matlab';%path to find matlab process
utCmdInit('C:\Program Files\AOS\OEMPA 1.1.5.4\UTKernelMatlab.dll')%load matlab stub dll
if (strcmp(computer, 'PCWIN')>0)
    error('fifo is not available for 32 bits computer!');
end
maxAscanCount = 1024;%if fifo is enabled this is the max ascan count in the fifo, otherwise this is the max cycle count.
maxAscanPointCount = 16384;%maximum ascan point count (with or without fifo).
deviceId=utCmdNewDevice('192.168.1.11', [maxAscanCount maxAscanPointCount 1]);%new device (specify IP address and options: max 1024 ascan in the fifo and max ascan size is 16384).
b=mxConnect(deviceId,1);%connect
if ~b
    error('mxConnect');
end
b=mxReadFileWriteHW(deviceId,'C:\ProgramData\AOS\OEMPA 1.1.5.4\Cfg\DefaultSetupOEMPA.txt');%load config file
if ~b
    error('mxReadFileWriteHW');
end
a0 = mxGetAcquisitionAscanCount(deviceId);%get the total ascan count received by the computer after connection, should be 0 because pulser are still disabled.
[b0 fifoAscanLost0]=mxGetAcquisitionAscanFifoStatus(deviceId);%get the current ascan count saved in the fifo.
[c0 fifoCscanLost0]=mxGetAcquisitionCscanFifoStatus(deviceId);
cyclecount=mxGetSWCycleCount(deviceId);%get the cycle count (from setting, value in the OEMPA file).
cycles = linspace(0,cyclecount-1,cyclecount);
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
[b1 fifoAscanLost1]=mxGetAcquisitionAscanFifoStatus(deviceId);%get the current ascan count saved in the fifo.
[c1 fifoCscanLost0]=mxGetAcquisitionCscanFifoStatus(deviceId);
[aFifoIndex, Cycle, Sequence, PointCount, ByteSize, Signed] = mxGetAcquisitionAscanFifoIndex(deviceId, cycles);%get the fifo indexes of each cycle.
[Ascan, Cycle, Sequence, dEncoder, lEncoder] = mxGetAcquisitionAscanFifoData(deviceId,aFifoIndex);%get all cycles.
[cFifoIndex, Cycle, Sequence] = mxGetAcquisitionCscanFifoIndex(deviceId, cycles);%get the fifo indexes of each cycle.
[cscanAmp, cscanTof, dEncoder, lEncoder, sequence] = mxGetAcquisitionCscanFifoData(deviceId,cFifoIndex);%get cscan amplitude (all cycles).
plot(Ascan);%display ascan
b=mxConnect(deviceId,0);%disconnect
if ~b
    error('mxDisconnect');
end
mxDeleteDevice(deviceId);%delete device
utCmdExit;%free matlab stub dll

%mex utCmd.cpp;%to recompile the matlab process (REQUIRED ONLY ONE TIME), ADMINISTRATOR PRIVILEGE IS REQUIRED!
