function varargout = exampleHWDlg(varargin)
% EXAMPLEHWDLG MATLAB code for exampleHWDlg.fig
%      EXAMPLEHWDLG, by itself, creates a new EXAMPLEHWDLG or raises the existing
%      singleton*.
%
%      H = EXAMPLEHWDLG returns the handle to a new EXAMPLEHWDLG or the handle to
%      the existing singleton*.
%
%      EXAMPLEHWDLG('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in EXAMPLEHWDLG.M with the given input arguments.
%
%      EXAMPLEHWDLG('Property','Value',...) creates a new EXAMPLEHWDLG or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before exampleHWDlg_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to exampleHWDlg_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help exampleHWDlg

% Last Modified by GUIDE v2.5 03-Nov-2016 16:51:38

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @exampleHWDlg_OpeningFcn, ...
                   'gui_OutputFcn',  @exampleHWDlg_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before exampleHWDlg is made visible.
function exampleHWDlg_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to exampleHWDlg (see VARARGIN)

% Choose default command line output for exampleHWDlg
handles.output = hObject;

global g_handles;
[handles(:).strFile] = 'C:\ProgramData\AOS\OEMPA 1.1.5.4\Cfg\DefaultSetupOEMPA.txt';
if (strcmp(computer, 'PCWIN')>0) && handles.fifoEnable
    warning('The fifo should be disabled for 32 bits computer!');
    handles.fifoEnable = 0;%if fifo is disabled only one sequence is saved by the driver otherwise many.
end
[handles(:).maxAscanCount] =  2*128*128;%if fifo is enabled this is the max ascan count in the fifo, otherwise this is the max cycle count.
[handles(:).maxAscanPointCount] =  4*1024;%maximum ascan point count (with or without fifo).
[handles(:).strIP] =  '192.168.1.11';
tic
[handles(:).valueTimerAcquisition] = 0.25;
[handles(:).valueTimerDisplay] = 1.0/0.25;
global displayTotalCount hwTotalAscan hwTotalByte CycleCount;
global Cycles TotalTime lifoEnable fifoEnable;
global acquisitionTotalAscan acqAscanCountLast hwTotalAscanStartAcq;
global pulser acquisition display connected deviceId displayTimeStart;
global Ascan;
global saveData AscanAll EncPosAll savedTotal zsize zstep X;
fifoEnable =  1;%if fifo is disabled only one sequence is saved by the driver otherwise many.
lifoEnable = 0;
saveData = 0; % EC %
savedTotal = 0;
zsize = 100;
zstep = 0.0;%meter, 0.0  means internal trigger.
X = 0.0;
deviceId =  -1;
connected = 0;%connected = 0;
pulser =  0;
acquisition =  1;
display =  0;
CycleCount =  0;
Cycles = [];
TotalTime = toc;
hwTotalAscanStartAcq = 0;
displayTimeStart = toc;
hwTotalByte = 0;%total input ascan coming from the HW.
hwTotalAscan = 0;%total input ascan byte coming from the HW.
displayTotalCount = 0;%total display refresh count.
acquisitionTotalAscan = 0;%total input ascan catch by matlab.
acqAscanCountLast = 0;%useful only if the fifo is disabled.
Ascan = [];

utCmdInit('C:\Program Files\AOS\OEMPA 1.1.5.4\UTKernelMatlab.dll')%load matlab stub dll
%utCmdInit('C:\Users\Public\SW\UTKernel\x64\Release\UTKernelMatlab.dll')%load matlab stub dll

if ~fifoEnable
    set(handles.checkboxFifo, 'Value', 0);
else
    set(handles.checkboxFifo, 'Value', 1);
end
set(handles.checkboxDevice, 'Value', 0);
set(handles.checkboxConnect, 'Value', connected);
set(handles.checkboxPulser, 'Value', pulser);
set(handles.checkboxAcquisition, 'Value', acquisition);
set(handles.checkboxDisplay, 'Value', display);
if lifoEnable
    set(handles.radiobuttonLifo, 'Value', 1);
else
    set(handles.radiobuttonStandard, 'Value', 1);
end


[handles(:).myTimerStatus] = timer('Period',1,...
    'StartDelay',0,...
    'TimerFcn',{@my_timer_status,hObject,handles},...
    'ExecutionMode','FixedRate');
start(handles.myTimerStatus);
[handles(:).myTimerDisplay] = timer('Period',1.0/handles.valueTimerDisplay,...
    'StartDelay',0,...
    'TimerFcn',{@my_timer_display,hObject,handles},...
    'ExecutionMode','FixedRate');
start(handles.myTimerDisplay);
if ~display
    stop(handles.myTimerDisplay);
end
[handles(:).myTimerAcquisition] = timer('Period',handles.valueTimerAcquisition,...
    'StartDelay',0,...
    'TimerFcn',{@my_timer_acquisition,hObject,handles},...
    'ExecutionMode','FixedRate');
start(handles.myTimerAcquisition);
if ~acquisition
    stop(handles.myTimerAcquisition);
end

set(handles.figure1,'CurrentAxes',handles.axes1);
g_handles = handles;
handles = updateDialog(hObject, handles);
    
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes exampleHWDlg wait for user response (see UIRESUME)
% uiwait(handles.figure1);

function my_timer_status(obj, event, hObject, handles)
global displayTotalCount hwTotalAscan hwTotalByte TotalTime CycleCount;
global pulser acquisition deviceId acquisitionTotalAscan hwTotalAscanStartAcq;
global displayTimeStart zstep X EncPosAll saveData fifoEnable;
if (acquisition>0) && (deviceId>=0) && (CycleCount>0) && (pulser>0)
    if ~mxIsConnected(deviceId)
        set(handles.checkboxConnect, 'Value', 0);
    end
end
oldTotalByte = hwTotalByte;
oldTotalTime = TotalTime;
if (deviceId>=0) && (CycleCount>0)
    if fifoEnable
        [fifoAscanCount, fifoAscanLost, hwTotalAscan, hwTotalByte] = mxGetAcquisitionAscanFifoStatus(deviceId);
    else
        [hwTotalAscan, hwTotalByte] = mxGetAcquisitionAscanCount(deviceId);
        fifoAscanLost = 0;
        fifoAscanCount = 0;
    end
    lostAscan = mxGetLostCountAscan(deviceId);
    lostCscan = mxGetLostCountCscan(deviceId);
    lostEncoder = mxGetLostCountEncoder(deviceId);
    lostUSB3 = mxGetLostCountUSB3(deviceId);
    TotalTime = toc;
    speed = double(hwTotalByte-oldTotalByte)/(1024*1024*(TotalTime-oldTotalTime));
    if saveData
        if pulser && length(EncPosAll)
            acq = 100.0*sum(EncPosAll(2,:))/length(EncPosAll(2,:));
        else
            acq = 0.0;
        end
    else
        if (acquisition>0) && (hwTotalAscan-hwTotalAscanStartAcq>0)
            acq = 100.0*double(acquisitionTotalAscan)/double(hwTotalAscan-hwTotalAscanStartAcq);
        else
            acq = 0.0;
        end
    end
    set(handles.textStatusAscan, 'String', sprintf('Ascan: %.3f MB/sec - Lost: hw %d fifo %d',...
        speed,lostAscan,fifoAscanLost));
    set(handles.textStatusCscan, 'String', sprintf('Lost: Cscan %d\t Encoder %d\t Usb %d',...
        lostCscan,lostEncoder,lostUSB3));
    if saveData
        if get(handles.checkboxDisplay, 'Value')
            if zstep>0.0
                set(handles.textStatus, 'String', sprintf('X %.1f mm Acq. %.1f%% Refresh %.1fHz',...
                    X*1e3,acq,displayTotalCount/(toc-displayTimeStart)));
            else
                set(handles.textStatus, 'String', sprintf('Acq. Progress %.1f%% Refresh Rate %.1fHz',...
                    acq,displayTotalCount/(toc-displayTimeStart)));
            end
        else
            if zstep>0.0
                set(handles.textStatus, 'String', sprintf('X %.1f mm Acq. Progress %.1f%%',X*1e3,acq));
            else
                set(handles.textStatus, 'String', sprintf('Acq. Progress %.1f%%',acq));
            end
        end
    else
        if get(handles.checkboxDisplay, 'Value')
            if zstep>0.0
                set(handles.textStatus, 'String', sprintf('X %.1f mm Acq. %.1f%% Refresh %.1fHz',...
                    X*1e3,acq,displayTotalCount/(toc-displayTimeStart)));
            else
                set(handles.textStatus, 'String', sprintf('Acq. Process %.1f%% Refresh Rate %.1fHz',...
                    acq,displayTotalCount/(toc-displayTimeStart)));
            end
        else
            if zstep>0.0
                set(handles.textStatus, 'String', sprintf('X %.1f mm Acq. %.1f%%',X*1e3,acq));
            else
                set(handles.textStatus, 'String', sprintf('Acq. Process %.1f%%',acq));
            end
        end
    end
else
    set(handles.textStatusAscan, 'String', '');
end
if zstep>0.0
    set(handles.textStatusSetup, 'String', sprintf('%d cycles, step %.1f mm', CycleCount, zstep*1e3));
else
    set(handles.textStatusSetup, 'String', sprintf('%d cycles, internal', CycleCount));
end

function my_timer_display(obj, event, hObject, handles)
global displayTotalCount CycleCount deviceId display Ascan;
displayTotalCount = displayTotalCount+1;
if (display>0) && (deviceId>=0) && (CycleCount>0)
    [isize,jsize] = size(Ascan);
    if jsize==CycleCount
        %figure(1);
        switch(class(Ascan))
            case char('int8')
                set(handles.axes1,'CLim',[-128 127]);
            case char('uint8')
                set(handles.axes1,'CLim',[0 255]);
            case char('int16')
                set(handles.axes1,'CLim',[-32768 32767]);
            case char('uint16')
                set(handles.axes1,'CLim',[0 65535]);
        end
        imagesc(Ascan, 'CDataMapping', 'scaled', 'Parent', handles.axes1);
        if 0
            colorbar
        end
    end
end

function my_timer_acquisition(obj, event, hObject, handles)
global Cycles lifoEnable AscanAll EncPosAll savedTotal zsize zstep;
global acquisitionTotalAscan acqAscanCountLast CycleCount fifoEnable;
global acquisition deviceId saveData pulser Ascan X iInternalIndex;
if(acquisition>0) && (deviceId>=0) && (CycleCount>0)
    if fifoEnable>0
		myAscan = [];
        if length(Cycles)~=CycleCount
            Cycles = linspace(0,CycleCount-1,CycleCount);
        end
        if lifoEnable
            %easier to use if you just want to display the last data.
            FifoIndex = mxGetAcquisitionAscanLifoIndex(deviceId, Cycles);
            if(length(FifoIndex)==CycleCount)
                [Ascan, Cycle, Sequence, encRawVal, lEncoder] = mxGetAcquisitionAscanFifoData(deviceId, FifoIndex);
                [isize, jsize] = size(Ascan);
                acquisitionTotalAscan = acquisitionTotalAscan + jsize;
            end
        else
            %we want to get all acquisition data.
            %it is required to read all ascan in the fifo otherwise the
            %fifo will be full. Once the fifo is full, the first sequence
            %is never a whole sequence and it is required to flush the
            %fifo.
            if saveData && pulser
                saved = 0;
                saveEnable = 1;
            else
                saveEnable = 0;
            end
            data = 1;
            myAscan = [];
            while(data)
                FifoIndex = mxGetAcquisitionAscanFifoIndex(deviceId, Cycles);
                if(length(FifoIndex)==CycleCount) && min(FifoIndex)>=0
                    [myAscan, Cycle, Sequence, encoderValue, lEncoder] = mxGetAcquisitionAscanFifoData(deviceId, FifoIndex);
                    [isize, jsize] = size(myAscan);
                    acquisitionTotalAscan = acquisitionTotalAscan + jsize;
					if (zstep>0.0) && length(encoderValue)>0
						X = encoderValue(1,1);
					else
						X = 0.0;
					end
                    if saveEnable
                        if length(AscanAll)==0
                            AscanAll = zeros(isize,jsize,zsize,'int8');
                            EncPosAll = zeros(2,zsize);
                            savedTotal = 0;
							iInternalIndex = 0;%useful if encoder is disabled.
                        end
						if zstep>0.0
							index = round(encoderValue(1)/zstep);
						else
							index = iInternalIndex;
							iInternalIndex = iInternalIndex + 1;
						end
                        if((0<=index) && (index<zsize))
                            AscanAll(:,:,index+1) = myAscan;
                            EncPosAll(1,index+1) = X;
                            EncPosAll(2,index+1) = 1;%data has been acquired.
                            saved = saved + 1;
                        end
                    end
                else
                    data = 0;
                end
            end
            if length(myAscan)
                Ascan = myAscan;
            end
            %if saveEnable && (saved>0)
            %    savedTotal = savedTotal + saved;
            %    fprintf('%d %d\n', savedTotal, saved);
            %end
        end
    else
        acqAscanCount = mxGetAcquisitionAscanCount(deviceId);
        if acqAscanCount>=acqAscanCountLast+CycleCount
            Ascan = [];
            for iCycle=0:CycleCount-1
                data = mxGetAcquisitionAscanData(deviceId, iCycle);
                Ascan = [Ascan data'];
            end
            acqAscanCountLast = acqAscanCount;
            acquisitionTotalAscan = acquisitionTotalAscan+CycleCount;
        end
    end
end
 
function handles = updateDialog(hObject, handles)
global CycleCount pulser connected deviceId fifoEnable;
if deviceId>=0
    set(handles.editMaxCycle, 'Enable', 'off');
    set(handles.editMaxPoint, 'Enable', 'off');
    set(handles.checkboxFifo, 'Enable', 'off');
    set(handles.editIP, 'Enable', 'off');
    set(handles.checkboxConnect, 'Enable', 'on');
else
    set(handles.editMaxCycle, 'Enable', 'on');
    set(handles.editMaxPoint, 'Enable', 'on');
    set(handles.checkboxFifo, 'Enable', 'on');
    set(handles.editIP, 'Enable', 'on');
    set(handles.checkboxConnect, 'Enable', 'off');
    set(handles.checkboxConnect, 'Value', 0);
    set(handles.checkboxPulser, 'Value', 0);
end
connected = get(handles.checkboxConnect, 'Value');
pulser = get(handles.checkboxPulser, 'Value');
if deviceId>=0 && connected==1 && CycleCount>0
    set(handles.checkboxPulser, 'Enable', 'on');
else
    set(handles.checkboxPulser, 'Enable', 'off');
end
if deviceId>=0 && connected==1 && pulser==0
    set(handles.buttonLoad, 'Enable', 'on');
    set(handles.buttonSave, 'Enable', 'on');
    set(handles.checkboxUSB3, 'Enable', 'on');
    set(handles.checkboxSaveData, 'Enable', 'on');
    set(handles.editGainDigital, 'Enable', 'on');
    if get(handles.checkboxSaveData,'Value')
        set(handles.editSaveData, 'Enable', 'off');
        set(handles.buttonAcqFile, 'Enable', 'on');
    else
        set(handles.editSaveData, 'Enable', 'on');
        set(handles.buttonAcqFile, 'Enable', 'off');
    end
else
    set(handles.buttonLoad, 'Enable', 'off');
    set(handles.buttonSave, 'Enable', 'off');
    set(handles.checkboxUSB3, 'Enable', 'off');
    set(handles.checkboxSaveData, 'Enable', 'off');
    set(handles.editSaveData, 'Enable', 'off');
    set(handles.editGainDigital, 'Enable', 'off');
    if get(handles.checkboxSaveData,'Value')
        set(handles.buttonAcqFile, 'Enable', 'on');
    else
        set(handles.buttonAcqFile, 'Enable', 'off');
    end
end
if get(handles.checkboxAcquisition, 'Value')
    set(handles.editTimerAcquisition, 'Enable', 'off');
else
    set(handles.editTimerAcquisition, 'Enable', 'on');
end
if get(handles.checkboxDisplay, 'Value')
    set(handles.editTimerDisplay, 'Enable', 'off');
else
    set(handles.editTimerDisplay, 'Enable', 'on');
end
if fifoEnable
    set(handles.buttongroup, 'Visible', 'on');
else
    set(handles.buttongroup, 'Visible', 'off');
end
    
function myClose()
global g_handles;
handles = g_handles;
try
    stop(handles.myTimerStatus);
    stop(handles.myTimerDisplay);
    stop(handles.myTimerAcquisition);
catch err
end
try
    utCmdExit;
catch err
    uiwait(msgbox('Error utCmdExit','modal'));
end
%delete(gcf);

% --- Executes on button press in buttonClose.
function buttonClose_Callback(hObject, eventdata, handles)
% hObject    handle to buttonClose (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
myClose
delete(handles.figure1);

% --- Executes when user attempts to close figure1.
function figure1_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: delete(hObject) closes the figure
myClose
delete(hObject);



% --- Executes on button press in checkboxDevice.
function checkboxDevice_Callback(hObject, eventdata, handles)
% hObject    handle to checkboxDevice (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkboxDevice
global deviceId fifoEnable;
if (get(hObject,'Value'))
    if deviceId==-1
        try
            deviceId = utCmdNewDevice(handles.strIP, [handles.maxAscanCount handles.maxAscanPointCount fifoEnable]);%new device (specify IP address and options: max 1024 ascan in the fifo and max ascan size is 16384).
        catch
            deviceId = -1;
        end
        if deviceId<0
            set(handles.checkboxDevice, 'Value', 0);
        end
        handles = updateDialog(hObject, handles);
    end
else
    if deviceId>=0
        mxDeleteDevice(deviceId);
        deviceId = -1;
        handles = updateDialog(hObject, handles);
    end
end
% Update handles structure
guidata(hObject, handles);



% --- Executes on button press in checkboxConnect.
function checkboxConnect_Callback(hObject, eventdata, handles)
% hObject    handle to checkboxConnect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkboxConnect
global connected;
global deviceId CycleCount;
if (get(hObject,'Value'))
    if connected==0
        b = mxConnect(deviceId,1);
        if ~b
            set(handles.checkboxConnect, 'Value', 0);
        else
            CycleCount = mxGetSWCycleCount(deviceId);
        end
        handles = updateDialog(hObject, handles);
        if mxIsUsb3Connected(deviceId)
            set(handles.checkboxUSB3, 'Visible', 'on');
            set(handles.checkboxUSB3, 'Enable', 'on');
            set(handles.checkboxUSB3, 'Value', 1);
            mxEnableUsb3(deviceId, 1);
        else
            set(handles.checkboxUSB3, 'Visible', 'off');
            set(handles.checkboxUSB3, 'Enable', 'off');
        end
    end
else
    if connected==1
        b = mxConnect(deviceId,0);
        handles = updateDialog(hObject, handles);
    end
end
% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in checkboxPulser.
function checkboxPulser_Callback(hObject, eventdata, handles)
% hObject    handle to checkboxPulser (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkboxPulser
global displayTotalCount hwTotalAscan hwTotalByte TotalTime;
global acquisitionTotalAscan acqAscanCountLast;
global pulser connected;
global deviceId hwTotalAscanStartAcq displayTimeStart;
if ~connected
    return;
end
if (get(hObject,'Value'))
    if pulser==0
        mxResetCounters(deviceId);
        hwTotalAscanStartAcq = 0;
        acquisitionTotalAscan = 0;
        acqAscanCountLast = 0;
        hwTotalAscan = 0;
        displayTotalCount = 0;
        %it is better to flush the fifo in case it was full.
        mxFlushAcquisitionAscanFifo(deviceId);
        b = mxEnableShot(deviceId,1);
        if ~b
            set(handles.checkboxPulser, 'Value', 0);
        else
            hwTotalByte = 0;
            tic
            TotalTime = toc;
            displayTimeStart = toc;
        end
    end
else
    if pulser==1
        b = mxEnableShot(deviceId,0);
    end
end
handles = updateDialog(hObject, handles);
% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in checkboxAcquisition.
function checkboxAcquisition_Callback(hObject, eventdata, handles)
% hObject    handle to checkboxAcquisition (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkboxAcquisition
global acquisitionTotalAscan acqAscanCountLast hwTotalAscan;
global acquisition hwTotalAscanStartAcq;
global deviceId;
if (get(hObject,'Value'))
    if acquisition==0
        acquisitionTotalAscan = 0;
        acqAscanCountLast = 0;
        hwTotalAscanStartAcq = hwTotalAscan;
        acquisition = 1;
        handles = updateDialog(hObject, handles);
        start(handles.myTimerAcquisition);
    end
else
    if acquisition==1
        stop(handles.myTimerAcquisition);
        acquisition = 0;
        handles = updateDialog(hObject, handles);
    end
end
% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in checkboxDisplay.
function checkboxDisplay_Callback(hObject, eventdata, handles)
% hObject    handle to checkboxDisplay (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkboxDisplay
global displayTotalCount display displayTimeStart;
global deviceId;
if (get(hObject,'Value'))
    if display==0
        display = 1;
        handles = updateDialog(hObject, handles);
        displayTotalCount = 0;%total display refresh count.
        displayTimeStart = toc;
        start(handles.myTimerDisplay);
    end
else
    if display==1
        stop(handles.myTimerDisplay);
        display = 0;
        handles = updateDialog(hObject, handles);
    end
end
% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in checkboxUSB3.
function checkboxUSB3_Callback(hObject, eventdata, handles)
% hObject    handle to checkboxUSB3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkboxUSB3
global deviceId;
if (get(hObject,'Value'))
    mxEnableUsb3(deviceId, 1);
else
    mxEnableUsb3(deviceId, 0);
end


% --- Executes on button press in buttonLoad.
function buttonLoad_Callback(hObject, eventdata, handles)
% hObject    handle to buttonLoad (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global CycleCount zstep;
global deviceId;
CycleCount = 0;
try
    handles.strFile = winqueryreg('HKEY_CURRENT_USER','Software\utKernel\matlab','SetupFile');
catch err
    handles.strFile = 0;
end
if(isnumeric(handles.strFile) & ~handles.strFile)
    [ret, folder] = uigetfile('*.txt');
else
    [ret, folder] = uigetfile(handles.strFile);
end
if ret==0
    return;
end
fullFileName = fullfile(folder, ret);
b = mxReadFileWriteHW(deviceId,fullFileName);
if b
    handles.strFile = fullFileName;
    %writereg('HKEY_CURRENT_USER','Software\utKernel\matlab','SetupFile',handles.strFile);
    mxSaveRegistryString('Software\utKernel\matlab','SetupFile',handles.strFile);
    CycleCount = mxGetSWCycleCount(deviceId);
    zstep = mxGetAcquisitionStep(deviceId);
else
    uiwait(msgbox(handles.strFile,'Error mxReadFileWriteHW','modal'));
end
handles = updateDialog(hObject, handles);
% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in buttonSave.
function buttonSave_Callback(hObject, eventdata, handles)
% hObject    handle to buttonSave (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global deviceId;
try
    file = winqueryreg('HKEY_CURRENT_USER','Software\utKernel\matlab','SetupFile');
catch err
    file = 0;
end
if(isnumeric(file) & ~file)
    [ret, folder] = uiputfile('*.txt');
else
    [ret, folder] = uiputfile(file);
end
if ret==0
    return;
end
fullFileName = fullfile(folder, ret);
b = mxReadHWWriteFile(deviceId,fullFileName);
% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in checkboxFifo.
function checkboxFifo_Callback(hObject, eventdata, handles)
% hObject    handle to checkboxFifo (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkboxFifo
global fifoEnable;
if (get(hObject,'Value'))
    fifoEnable = 1;
else
    fifoEnable = 0;
end
handles = updateDialog(hObject, handles);
% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in radiobuttonStandard.
function radiobuttonStandard_Callback(hObject, eventdata, handles)
% hObject    handle to radiobuttonStandard (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobuttonStandard


% --- Executes on button press in radiobuttonLifo.
function radiobuttonLifo_Callback(hObject, eventdata, handles)
% hObject    handle to radiobuttonLifo (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobuttonLifo



function editMaxCycle_Callback(hObject, eventdata, handles)
% hObject    handle to editMaxCycle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editMaxCycle as text
%        str2double(get(hObject,'String')) returns contents of editMaxCycle as a double
handles.maxAscanCount = str2num( get(hObject,'String') );
% Update handles structure
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function editMaxCycle_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editMaxCycle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editMaxPoint_Callback(hObject, eventdata, handles)
% hObject    handle to editMaxPoint (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editMaxPoint as text
%        str2double(get(hObject,'String')) returns contents of editMaxPoint as a double
handles.maxAscanPointCount = str2num( get(hObject,'String') );
% Update handles structure
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function editMaxPoint_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editMaxPoint (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editIP_Callback(hObject, eventdata, handles)
% hObject    handle to editIP (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editIP as text
%        str2double(get(hObject,'String')) returns contents of editIP as a double
handles.strIP = get(hObject,'String');
% Update handles structure
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function editIP_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editIP (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Outputs from this function are returned to the command line.
function varargout = exampleHWDlg_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

function editTimerAcquisition_Callback(hObject, eventdata, handles)
% hObject    handle to editTimerAcquisition (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editTimerAcquisition as text
%        str2double(get(hObject,'String')) returns contents of editTimerAcquisition as a double


% --- Executes during object creation, after setting all properties.
function editTimerAcquisition_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editTimerAcquisition (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editTimerDisplay_Callback(hObject, eventdata, handles)
% hObject    handle to editTimerDisplay (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editTimerDisplay as text
%        str2double(get(hObject,'String')) returns contents of editTimerDisplay as a double


% --- Executes during object creation, after setting all properties.
function editTimerDisplay_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editTimerDisplay (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in checkboxSaveData.
function checkboxSaveData_Callback(hObject, eventdata, handles)
% hObject    handle to checkboxSaveData (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkboxSaveData
global deviceId;
global saveData AscanAll EncPosAll zsize X;
if get(hObject,'Value')
    saveData = 1;
    AscanAll = [];
    EncPosAll = [] ;
    zsize = str2num( get(handles.editSaveData,'String') );
    X = 0.0;
    mxResetEncoder(deviceId);
else
    saveData = 0;
end
handles = updateDialog(hObject, handles);
% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in buttonAcqFile.
function buttonAcqFile_Callback(hObject, eventdata, handles)
% hObject    handle to buttonAcqFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global saveData AscanAll EncPosAll;
if saveData && ~isempty(AscanAll)
    [ret, folder] = uiputfile('*.mat');
    if ret==0
        return;
    end
    fullFileName = fullfile(folder, ret);
    if get(hObject,'Value')
        save(fullFileName,'AscanAll','EncPosAll')
        fprintf('MAT file saved.\n')
        %Example to load and display the datas (analysis mode)
        %load('Test.mat')    if 'Test.mat" was the name of the saved file.
        %imagesc(squeeze(max(abs(AscanAll(100:200,:,:)))))
    end
end


% --- Executes when selected object is changed in buttongroup.
function buttongroup_SelectionChangeFcn(hObject, eventdata, handles)
% hObject    handle to the selected object in buttongroup 
% eventdata  structure with the following fields (see UIBUTTONGROUP)
%	EventName: string 'SelectionChanged' (read only)
%	OldValue: handle of the previously selected object or empty if none was selected
%	NewValue: handle of the currently selected object
% handles    structure with handles and user data (see GUIDATA)

%eventdata.OldValue.String
global lifoEnable;
data = eventdata.NewValue;
if handles.radiobuttonStandard==data
    lifoEnable = 0;
elseif handles.radiobuttonLifo==data
    lifoEnable = 1;
end



function editSaveData_Callback(hObject, eventdata, handles)
% hObject    handle to editSaveData (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editSaveData as text
%        str2double(get(hObject,'String')) returns contents of editSaveData as a double
global AscanAll EncPosAll zsize;
zsize = get(hObject,'Value');
AscanAll = [];
EncPosAll = [];


% --- Executes during object creation, after setting all properties.
function editSaveData_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editSaveData (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editGainDigital_Callback(hObject, eventdata, handles)
% hObject    handle to editGainDigital (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editGainDigital as text
%        str2double(get(hObject,'String')) returns contents of editGainDigital as a double
global deviceId CycleCount;
GainDigital = str2double( get(hObject,'String') );
if(mxLockDevice(deviceId, 0))
    for iCycle=1:CycleCount
        mxSetGainDigital(deviceId, iCycle-1, GainDigital);
    end
    mxUnlockDevice(deviceId, 0);
end

% --- Executes during object creation, after setting all properties.
function editGainDigital_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editGainDigital (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
