function ExampleWizard(password)
%PLEASE READ CAREFULLY THE COMMENTS
%Before running this function compile the mex file:
%mex utCmd.c;%to recompile the matlab process (REQUIRED ONLY ONE TIME), ADMINISTRATOR PRIVILEGE IS REQUIRED!
%
%The password is required for special purpose, you dont need to know it.
%You can complete each step below one after the other
%from the matlab command line

%In case of Kit32 with a 64 bits computer
%cd 'C:\Program Files (x86)\AOS\OEMPA 1.1.5.3\matlab';%path to find matlab process
%utCmdInit('C:\Program Files (x86)\AOS\OEMPA 1.1.5.3\UTKernelMatlab.dll')%load matlab stub dll in case of Kit32 with 64 bits computer
%In case of Kit64 or Kit32 with 32 bits computer
cd 'C:\Program Files\AOS\OEMPA 1.1.5.3\matlab';%path to find matlab process
utCmdInit('C:\Program Files\AOS\OEMPA 1.1.5.3\UTKernelMatlab.dll')%load matlab stub dll

IPAddress = '192.168.1.11:5001';
SetupFileName = 'C:\ProgramData\AOS\OEMPA 1.1.5.3\Files\OEMPA\Example.txt';
WizardFileName = 'C:\ProgramData\AOS\OEMPA 1.1.5.3\Files\Kernel\Wizard.txt';
bVisible = 1;%it could be modified later with function "mxToolboxVisible(1 or 0)"
bMatrix = 0;%0 to disable the matrix code and 1 to enable it.
if(~mxToolboxIsRunning())
    mxToolboxRun(bVisible);%run the toolbox
    %"UTKernelMatlab.dll" and "UTToolbox.exe" should be in the same folder.
end
if(mxToolboxIsRunning())
    %device connection is required for two purposes:
    %- to save the OEMPA file.
    %- to be able to use matrix.
    if(~mxSetToolboxDeviceAddress(IPAddress))%set IP address of the device.
        error('error to set the IP address');
    else
        if(~mxToolboxDeviceConnection(1))%connect to the device.
            error('error: Toolbox OEMPA device bad connection')
        end
    end
    if(nargin>=1)
        mxSetToolboxParameter(strcat('CurrentPref\Preference\Password=',password));
    end
    if (exist(WizardFileName, 'file') == 2)
		%first time you run this script the file doesnt exist.
		%second time the file has been created.
        if(~mxToolboxLoadWizard(WizardFileName))
            error('error to load the setup');
        end
    else%the setup file doesnt exist => create default wizard with default values.
        if(~mxToolboxIsWizardEnabled())
            if(~mxToolboxWizardNew())%default values => need to set all wanted values.
                error('error to create the wizard');
            end
        else
            %the wizard has been already created, the value could be
            %different than the defaults.
        end
        if(bVisible>0)
            if(~mxToolboxDisplay(mxGetToolboxObjectId('OEMPA_system')))
                sprintf('warning: WizardSystem not displayed')
            end
            if(~mxToolboxDisplay(mxGetToolboxObjectId('OEMPA_channel_0')))
                sprintf('warning: Channel not displayed');
            end
        end
        %fill all input parameters
        v = mxGetToolboxParameter('OEMPA_system\WizardSystem\WizardSpecimen\VelocityLongitudinal');%before updating the 'VelocityLongitudinal'.
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardSpecimen\VelocityLongitudinal=5950.0 m/s');
        v = mxGetToolboxParameter('OEMPA_system\WizardSystem\WizardSpecimen\VelocityLongitudinal');%after updating the 'VelocityLongitudinal'.
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardSpecimen\VelocityTransverse=2950.0 m/s');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Frequency=5.0 MHz');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Count=32');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Pitch=0.5 mm');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\WizardWedge\Enable=1');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\WizardWedge\OffsetZ=20.0 mm');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\WizardWedge\Velocity=2450.0 m/s');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\Wave=Transverse');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStart\Aperture\Count1=32');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStart\EmissionDepth=40.0 mm');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStart\Depth.count=4');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStart\Depth:0=10.0 mm');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStart\Depth:1=20.0 mm');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStart\Depth:2=30.0 mm');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStart\Depth:3=40.0 mm');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStart\Angle=-10.0 deg');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStop\Angle=+10.0 deg');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStep\Angle=1.0 deg');
        %ability to change any other parameter...
		%it is also possible to use matrix:
		if(bMatrix)
			mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Probe=Custom');
            %before loading the file
			X = mxGetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Probe:0\X');
			Y = mxGetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Probe:0\Y');
			Z = mxGetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Probe:0\Z');
			mxToolboxLoadMatrix('C:\ProgramData\AOS\OEMPA 1.1.5.3\Files\OEMPA\MatrixBar\XYList11292.txt');
            %after loading the file
			X = [X ' ' mxGetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Probe:0\X')];
			Y = [Y ' ' mxGetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Probe:0\Y')];
			Z = [Z ' ' mxGetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Probe:0\Z')];
			mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStart\Aperture\Index.count=128');
			mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe\Scan\WizardScan\VectorStart\Aperture\Index:0=0:127');
		end
    end
    if(~mxToolboxWizardUpdateScan())%compute focal laws (delays, ...)
        error('error to update the scan');
    end
    %display results
    figure(1);
    clf;
    hold on;
    iProbeElementCount = mxGetWizardElementCount();
    Element = [];
    for ii=1:iProbeElementCount
        Element = [Element mxGetWizardElement(ii-1)];
    end
    plot(Element(1,:)*1000,Element(3,:)*1000,'+r');
    iBeamCount = mxGetWizardBeamCount();
    for ii=1:iBeamCount
        Beam = mxGetWizardBeam(ii-1);
        plot(Beam(1,:)*1000,Beam(3,:)*1000,'b');
    end
    %save setup.
    if(mxToolboxIsDeviceConnected())%connect to the device.
        id = mxGetToolboxDeviceId();%get device id.
        if(~mxToolboxSaveSetup(id, SetupFileName))%connection with the device is required to save the OEMPA file.
            error('error to save OEMPA file');
        end
    end
    if(~mxToolboxSaveWizard(WizardFileName))%save only inputs (not outputs as delays etc...).
        error('error to save the wizard');
    end
    if(~mxToolboxDeviceConnection(0))%disconnect.
        error('error: Toolbox OEMPA device bad connection')
    end
end
%mxToolboxClose();%close the toolbox
%utCmdExit;%free matlab stub dll
