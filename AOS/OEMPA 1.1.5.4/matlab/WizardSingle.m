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
%cd 'C:\Program Files\AOS\OEMPA 1.1.5.3\matlab';%path to find matlab process
%utCmdInit('C:\Program Files\AOS\OEMPA 1.1.5.3\UTKernelMatlab.dll')%load matlab stub dll

IPAddress = '127.0.0.1:5001';
SetupFileName = 'C:\Users\Public\Share\matrix\ExampleSingle.txt';
WizardFileName = 'C:\Users\Public\Share\matrix\WizardSingle.txt';
bVisible = 1;%it could be modified later with function "mxToolboxVisible(1 or 0)"
matrixFile = 'C:\Users\Public\Share\matrix\XYList11293.txt';
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
    if (0 && (exist(WizardFileName, 'file') == 2))
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
        v = mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardSpecimen\Specimen=Bar');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardSpecimen\Radius=254.0 mm');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardSpecimen\VelocityLongitudinal=5899.912 m/s');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardSpecimen\VelocityTransverse=3240.024 m/s');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Probe=Custom');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Frequency=5.0 MHz');

        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\Skew=90.0 deg');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\WizardWedge\Enable=1');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\WizardWedge\OffsetZ=19.5491 mm');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\WizardWedge\Velocity=2350.008 m/s');
        mxSetToolboxParameter('OEMPA_system\WizardSystem\WizardProbe:100\WizardWedge\Angle=36.1 m/s');
        mxToolboxLoadMatrix(matrixFile);

        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan.count=1');
        
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\Wave=Transverse');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\DepthMode=SoundPath');

        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStart\Aperture\Index.Count=128');
        for ii=0:127
            v = strcat('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStart\Aperture\Index:',int2str(ii),'=',int2str(ii));
            mxSetToolboxParameter(v);
        end
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStart\EmissionDepth=25.4 mm');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStart\Depth.count=1');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStart\Depth:0=25.4 mm');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStart\Angle=30.0 deg');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStart\Skew=90.0 deg');

        for ii=0:127
            v = strcat('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStop\Aperture\Index:',int2str(ii),'=',int2str(ii));
            mxSetToolboxParameter(v);
        end
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStop\Depth.count=1');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStop\Depth:0=25.4 mm');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStop\Angle=50.0 deg');
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStop\Skew=90.0 deg');
        
        for ii=0:127
            v = strcat('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStep\Aperture\Index:',int2str(ii),'=',int2str(ii));
            mxSetToolboxParameter(v);
        end
        mxSetToolboxParameter('OEMPA_channel_0\Channels\Probe:100\Scan:100\WizardScan\VectorStep\Angle=1.0 deg');
        %ability to change any other parameter...
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
    plot3(Element(1,:)*1000,Element(2,:)*1000,Element(3,:)*1000,'+r');
    iBeamCount = mxGetWizardBeamCount();
    for ii=1:iBeamCount
        Beam = mxGetWizardBeam(ii-1);
        plot3(Beam(1,:)*1000,Beam(2,:)*1000,Beam(3,:)*1000,'b');
    end
    %save setup.
    if(~mxToolboxIsDeviceConnected())%connect to the device.
        id = mxGetToolboxDeviceId();%get device id.
        if(~mxToolboxSaveSetup(id, SetupFileName))%connection with the device is required to save the OEMPA file.
            error('error to save OEMPA file');
        end
    end
    if(~mxToolboxSaveWizard(WizardFileName))%save only inputs (not outputs as delays etc...).
        error('error to save the wizard');
    end
    %if(~mxToolboxDeviceConnection(0))%disconnect.
    %    error('error: Toolbox OEMPA device bad connection')
    %end
end
%mxToolboxClose();%close the toolbox
%utCmdExit;%free matlab stub dll
