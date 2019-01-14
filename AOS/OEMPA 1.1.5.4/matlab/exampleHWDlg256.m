function exampleHWDlg256()

    connected = 0;
    strFile128A = 'C:\ProgramData\AOS\OEMPA 1.1.5.4\Cfg\DefaultSetupOEMPA128A.txt';
    strFile128B = 'C:\ProgramData\AOS\OEMPA 1.1.5.4\Cfg\DefaultSetupOEMPA128B.txt';
    pulser = 0;
    deviceId0 = -1;
    deviceId1 = -1;
    lifoEnable = 1;
    %fifoEnable = 1;%if fifo is disabled only one sequence is saved by the driver otherwise many.
    if (strcmp(computer, 'PCWIN')>0)% && fifoEnable
        error('32 bits computer is required for the 256:256!');
        %fifoEnable = 0;%if fifo is disabled only one sequence is saved by the driver otherwise many.
    end
    %maxAscanCount = 2*128;%if fifo is enabled this is the max ascan count in the fifo, otherwise this is the max cycle count.
    %maxAscanPointCount = 4*1024;%maximum ascan point count (with or without fifo).
    display = 0;
    acquisition = 1;
    strIP0 = '192.168.0.11';
    strIP1 = '192.168.0.12';
    CycleCount0 = 0;
    CycleCount1 = 0;
    valueRange = 0.0;
    tic
    TotalTime = toc;
    valueTimerAcquisition = 0.25;
    valueTimerDisplay = 1.0/0.25;
    hwTotalByte = 0;%total input ascan coming from the HW.
    hwTotalByte0 = 0;%total input ascan coming from the HW.
    hwTotalByte1 = 0;%total input ascan coming from the HW.
    hwTotalAscan = 0;%total input ascan byte coming from the HW.
    acquisitionTotalAscan = 0;%total input ascan catch by matlab.
    %acqAscanCountLast = 0;%useful only if the fifo is disabled.
    hwTotalAscanStartAcq = 0;
    displayTotalCount = 0;%total display refresh count.
    displayTimeStart = toc;
    Cycles = [];
    global Ascan;
    Ascan = [];

    %utCmdInit('C:\Program Files\AOS\OEMPA 1.1.5.3\UTKernelMatlab.dll')%load matlab stub dll
    utCmdInit('C:\Users\Public\SW\UTKernel\x64\Release\UTKernelMatlab.dll')%load matlab stub dll
    
    hwDialog = dialog('Position',[300 300 260 300],...
           'Name','HW',...
           'WindowStyle','normal',...
           'CloseRequestFcn',@myCloseReq);
    
    textTimeAcquisitionUnit = uicontrol('Parent',hwDialog,...
           'Style','text',...
           'Position',[220 60+110-5 20 20],...
           'String','sec');
    
    textTimeDisplayUnit = uicontrol('Parent',hwDialog,...
           'Style','text',...
           'Position',[220 60+90-5 20 20],...
           'String','Hz');
    
    textRangeUnit = uicontrol('Parent',hwDialog,...
           'Style','text',...
           'Position',[220 60+70-5 20 20],...
           'String','us');
    
    textStatusAscan0 = uicontrol('Parent',hwDialog,...
           'Style','text',...
           'Position',[20 30+65 200 20],...
           'String','');
    
    textStatusAscan1 = uicontrol('Parent',hwDialog,...
           'Style','text',...
           'Position',[20 15+65 200 20],...
           'String','');
    
    textStatusAscan = uicontrol('Parent',hwDialog,...
           'Style','text',...
           'Position',[20 65 200 20],...
           'String','');
    
    textStatusCscan = uicontrol('Parent',hwDialog,...
           'Style','text',...
           'Position',[20 50 200 20],...
           'String','');
    
    textStatus = uicontrol('Parent',hwDialog,...
           'Style','text',...
           'Position',[10 35 220 20],...
           'String','');
    
    %textMaxCycle = uicontrol('Parent',hwDialog,...
    %       'Style','text',...
    %       'Position',[40 60+190-5 100 20],...
    %       'String','Max Cycle');
    
    %textMaxPoint = uicontrol('Parent',hwDialog,...
    %       'Style','text',...
    %       'Position',[40 60+170-5 100 20],...
    %       'String','Max Point');
    
    textRange = uicontrol('Parent',hwDialog,...
           'Style','text',...
           'Position',[110 60+65 60 20],...
           'String','Range');
    
    editTimerAcquisition = uicontrol('Parent',hwDialog,...
           'Style','edit',...
           'Position',[170 60+110 50 20],...
           'String',mat2str(valueTimerAcquisition),...
           'Callback',@editAcquisition_Callback);
    
    editTimerDisplay = uicontrol('Parent',hwDialog,...
           'Style','edit',...
           'Position',[170 60+90 50 20],...
           'String',mat2str(valueTimerDisplay),...
           'Callback',@editDisplay_Callback);
    
    editRange = uicontrol('Parent',hwDialog,...
           'Style','edit',...
           'Position',[170 60+70 50 20],...
           'String',mat2str(valueRange*1e6),...
           'Callback',@editRange_Callback);
    
    %editMaxCycle = uicontrol('Parent',hwDialog,...
    %       'Style','edit',...
    %       'Position',[120 60+190 100 20],...
    %       'String',int2str(maxAscanCount),...
    %       'Callback',@editMaxCycle_Callback);
       
    %editMaxPoint = uicontrol('Parent',hwDialog,...
    %       'Style','edit',...
    %       'Position',[120 60+170 100 20],...
    %       'String',int2str(maxAscanPointCount),...
    %       'Callback',@editMaxPoint_Callback);
       
    editIP0 = uicontrol('Parent',hwDialog,...
           'Style','edit',...
           'Position',[120 60+150 100 20],...
           'String',strIP0,...
           'Callback',@editIP0_Callback);

    editIP1 = uicontrol('Parent',hwDialog,...
           'Style','edit',...
           'Position',[120 60+170 100 20],...
           'String',strIP1,...
           'Callback',@editIP1_Callback);

    buttongroup = uibuttongroup('Parent',hwDialog,...
           'Visible','on',...
           'Position',[0 0 1 1],...
           'SelectionChangeFcn',@radiobutton_Callback);
       
    radiobuttonStandard = uicontrol(buttongroup,...
           'Style','radiobutton',...
           'Position',[160 60+210 40 20],...
           'String','Std');
       
    radiobuttonLifo = uicontrol(buttongroup,...
           'Style','radiobutton',...
           'Position',[200 60+210 40 20],...
           'String','Lifo');
       
    %checkboxFifo = uicontrol('Parent',hwDialog,...
    %       'Style','checkbox',...
    %       'Position',[120 60+210 40 20],...
    %       'String','Fifo',...
    %       'Callback',@checkboxFifo_Callback);
       
    %checkboxUSB3 = uicontrol('Parent',hwDialog,...
    %       'Style','checkbox',...
    %       'Position',[200 60+130 50 20],...
    %       'String','USB3',...
    %       'Callback',@checkboxUSB3_Callback);
       
    checkboxDevice1 = uicontrol('Parent',hwDialog,...
           'Style','checkbox',...
           'Position',[20 60+150 80 20],...
           'String','Device 1',...
           'Callback',@checkboxDevice1_Callback);
       
    checkboxDevice2 = uicontrol('Parent',hwDialog,...
           'Style','checkbox',...
           'Position',[20 60+170 80 20],...
           'String','Device 2',...
           'Callback',@checkboxDevice2_Callback);
       
    checkboxConnect = uicontrol('Parent',hwDialog,...
           'Style','checkbox',...
           'Position',[20 60+130 60 20],...
           'String','Connect',...
           'Callback',@checkboxConnect_Callback);
       
    checkboxPulser = uicontrol('Parent',hwDialog,...
           'Style','checkbox',...
           'Position',[20 60+110 90 20],...
           'String','Enable Pulser',...
           'Callback',@checkboxEnablePulser_Callback);
       
    checkboxAcquisition = uicontrol('Parent',hwDialog,...
           'Style','checkbox',...
           'Position',[110 60+110 60 20],...
           'String','Acq.',...
           'Callback',@checkboxAcquisition_Callback);
    
    checkboxDisplay = uicontrol('Parent',hwDialog,...
           'Style','checkbox',...
           'Position',[110 60+90 60 20],...
           'String','Display',...
           'Callback',@checkboxDisplay_Callback);
    
    buttonLoad = uicontrol('Parent',hwDialog,...
           'Position',[120 60+130 40 20],...
           'String','Load',...
           'Callback',@pushbuttonLoad_Callback);

    buttonSave = uicontrol('Parent',hwDialog,...
           'Position',[160 60+130 40 20],...
           'String','Save',...
           'Callback',@pushbuttonSave_Callback);
    
    buttonClose = uicontrol('Parent',hwDialog,...
           'Position',[100 15 70 20],...
           'String','Close',...
           'Callback',@pushbuttonClose_Callback);
    
    %if ~fifoEnable
    %    set(checkboxFifo, 'Value', 0);
    %else
    %    set(checkboxFifo, 'Value', 1);
    %end
    set(checkboxDevice1, 'Value', 0);
    set(checkboxDevice2, 'Value', 0);
    set(checkboxConnect, 'Value', connected);
    set(checkboxPulser, 'Value', pulser);
    set(checkboxAcquisition, 'Value', acquisition);
    set(checkboxDisplay, 'Value', display);
    if lifoEnable
        set(radiobuttonLifo, 'Value', 1);
    else
        set(radiobuttonStandard, 'Value', 1);
    end
    updateDialog
    
    myTimerStatus = timer('Period',1,...
        'StartDelay',0,...
        'TimerFcn',@my_timer_status,...
        'ExecutionMode','FixedRate');
    start(myTimerStatus);
    myTimerDisplay = timer('Period',1.0/valueTimerDisplay,...
        'StartDelay',0,...
        'TimerFcn',@my_timer_display,...
        'ExecutionMode','FixedRate');
    start(myTimerDisplay);
    if ~display
        stop(myTimerDisplay);
    end
    myTimerAcquisition = timer('Period',valueTimerAcquisition,...
        'StartDelay',0,...
        'TimerFcn',@my_timer_acquisition,...
        'ExecutionMode','FixedRate');
    start(myTimerAcquisition);
    if ~acquisition
        stop(myTimerAcquisition);
    end
    
    % Wait for hwDialog to close before running to completion
    %uiwait(hwDialog);

    function my_timer_status(obj, event, string_arg)
        if (acquisition>0) && (CycleCount0>0) && (pulser>0)
            if (deviceId0>=0) && ~mxIsConnected(deviceId0)
                set(checkboxConnect, 'Value', 0);
            end
            if (deviceId1>=0) && ~mxIsConnected(deviceId1)
                set(checkboxConnect, 'Value', 0);
            end
        end
        oldTotalByte = hwTotalByte;
        oldTotalByte0 = hwTotalByte0;
        oldTotalByte1 = hwTotalByte1;
        oldTotalTime = TotalTime;
        if (deviceId0>=0) && (CycleCount0>0)
            lostAscan0 = mxGetLostCountAscan(deviceId0);
            lostCscan0 = mxGetLostCountCscan(deviceId0);
            lostEncoder0 = mxGetLostCountEncoder(deviceId0);
            lostUSB30 = mxGetLostCountUSB3(deviceId0);
            TotalTime = toc;
            [fifoAscanCount, fifoAscanLost, hwTotalAscan, hwTotalByte] = mxGetAcquisitionAscanFifoStatus(deviceId0);
            speed = double(hwTotalByte-oldTotalByte)/(1024*1024*(TotalTime-oldTotalTime));
            set(textStatusAscan, 'String', sprintf('Ascan: %.3f MB/sec FifoLost %d',...
                speed,fifoAscanLost));
            if (acquisition>0) && (hwTotalByte-hwTotalAscanStartAcq>0)
                acq = 100.0*double(acquisitionTotalAscan)/double(hwTotalAscan-hwTotalAscanStartAcq);
            else
                acq = 0.0;
            end
            set(textStatus, 'String', sprintf('Acq. Process %.1f%% Refresh Rate %.1fHz',...
                acq,displayTotalCount/(toc-displayTimeStart)));

            if (deviceId1>=0) && (CycleCount0==CycleCount1)
                lostAscan1 = mxGetLostCountAscan(deviceId1);
                lostCscan1 = mxGetLostCountCscan(deviceId1);
                lostEncoder1 = mxGetLostCountEncoder(deviceId1);
                lostUSB31 = mxGetLostCountUSB3(deviceId1);
                
                [fifoAscanCount0, fifoAscanLost0, hwTotalAscan0, hwTotalByte0] = mxGetAcquisitionAscanFifoStatus256(deviceId0);
                speed0 = double(hwTotalByte0-oldTotalByte0)/(1024*1024*(TotalTime-oldTotalTime));
                set(textStatusAscan0, 'String', sprintf('Master: %.3f MB/sec Lost %d\t Fifo %d',...
                    speed0,lostAscan0,fifoAscanLost0));
                
                [fifoAscanCount1, fifoAscanLost1, hwTotalAscan1, hwTotalByte1] = mxGetAcquisitionAscanFifoStatus256(deviceId1);
                speed1 = double(hwTotalByte1-oldTotalByte1)/(1024*1024*(TotalTime-oldTotalTime));
                set(textStatusAscan1, 'String', sprintf('Slave: %.3f MB/sec Lost %d\t Fifo %d',...
                    speed1,lostAscan1,fifoAscanLost1));
                
                set(textStatusCscan, 'String', sprintf('LostM/S: Cscan %d/%d\t Enc %d/%d\t Usb %d',...
                    lostCscan0,lostCscan1,lostEncoder0,lostEncoder1,lostUSB30));
            else
            end
        else
            set(textStatusAscan, 'String', '');
            set(textStatusAscan0, 'String', '');
            set(textStatusAscan1, 'String', '');
        end
    end
    function my_timer_display(obj, event, string_arg)
        displayTotalCount = displayTotalCount+1;
        if (display>0) && (deviceId0>=0) && (CycleCount0>0)
            [isize,jsize] = size(Ascan);
            if jsize==CycleCount0
                figure(1);
                switch(class(Ascan))
                    case char('int8')
                        imagesc(Ascan,[-128 127]);
                    case char('uint8')
                        imagesc(Ascan,[0 255]);
                    case char('int16')
                        imagesc(Ascan,[-32768 32767]);
                    case char('uint16')
                        imagesc(Ascan,[0 65535]);
                end
                if 0
                    colorbar
                end
            end
        end
    end
    function my_timer_acquisition(obj, event, string_arg)
        if(acquisition>0) && (deviceId0>=0) && (CycleCount0>0)
            if length(Cycles)~=CycleCount0
                Cycles = linspace(0,CycleCount0-1,CycleCount0);
            end
            if lifoEnable>0
                %easier to use if you just want to display the last data.
                if lifoEnable
                    FifoIndex = mxGetAcquisitionAscanLifoIndex(deviceId0, Cycles);
                else
                    FifoIndex = mxGetAcquisitionAscanFifoIndex(deviceId0, Cycles);
                end
                if((length(FifoIndex)==CycleCount0) && (min(FifoIndex)>=0))
                    [Ascan, Cycle, Sequence, encRawVal, lEncoder] = mxGetAcquisitionAscanFifoData(deviceId0, FifoIndex);
                    [isize, jsize] = size(Ascan);
                    acquisitionTotalAscan = acquisitionTotalAscan + jsize;
                end
            else
                %we want to get all acquisition data.
                %it is required to read all ascan in the fifo otherwise the
                %fifo will be full. Once the fifo is full, the first sequence
                %is never a whole sequence and it is required to flush the
                %fifo.
                data = 1;
                myAscan = [];
                while(data)
                    FifoIndex = mxGetAcquisitionAscanFifoIndex(deviceId0, Cycles);
                    if(length(FifoIndex)==CycleCount0) && min(FifoIndex)>=0
                        [myAscan, Cycle, Sequence, encoderValue, lEncoder] = mxGetAcquisitionAscanFifoData(deviceId0, FifoIndex);
                        [isize, jsize] = size(myAscan);
                        acquisitionTotalAscan = acquisitionTotalAscan + jsize;
                    else
                        data = 0;
                    end
                end
                if length(myAscan)
                    Ascan = myAscan;
                end
            end
        end
    end
    function updateDialog
        if (deviceId1>=0)
            set(editIP1, 'Enable', 'off');
        else
            set(editIP1, 'Enable', 'on');
        end
        if (deviceId0>=0)
            %set(editMaxCycle, 'Enable', 'off');
            %set(editMaxPoint, 'Enable', 'off');
            %set(checkboxFifo, 'Enable', 'off');
            set(editIP0, 'Enable', 'off');
            set(checkboxConnect, 'Enable', 'on');
        else
            %set(editMaxCycle, 'Enable', 'on');
            %set(editMaxPoint, 'Enable', 'on');
            %set(checkboxFifo, 'Enable', 'on');
            set(editIP0, 'Enable', 'on');
            set(checkboxConnect, 'Enable', 'off');
            set(checkboxConnect, 'Value', 0);
            set(checkboxPulser, 'Value', 0);
        end
        connected = get(checkboxConnect, 'Value');
        pulser = get(checkboxPulser, 'Value');
        if (deviceId0>=0) && connected==1 && (CycleCount0>0)
            set(checkboxPulser, 'Enable', 'on');
            set(editRange, 'Enable', 'on');
        else
            set(checkboxPulser, 'Enable', 'off');
            set(editRange, 'Enable', 'off');
        end
        if (deviceId0>=0) && connected==1 && pulser==0
            set(buttonLoad, 'Enable', 'on');
            set(buttonSave, 'Enable', 'on');
            %set(checkboxUSB3, 'Enable', 'on');
        else
            set(buttonLoad, 'Enable', 'off');
            set(buttonSave, 'Enable', 'off');
            %set(checkboxUSB3, 'Enable', 'off');
        end
        if get(checkboxAcquisition, 'Value')
            set(editTimerAcquisition, 'Enable', 'off');
        else
            set(editTimerAcquisition, 'Enable', 'on');
        end
        if get(checkboxDisplay, 'Value')
            set(editTimerDisplay, 'Enable', 'off');
        else
            set(editTimerDisplay, 'Enable', 'on');
        end
        %if fifoEnable
            set(buttongroup, 'Visible', 'on');
        %else
        %    set(buttongroup, 'Visible', 'off');
        %end
    end
    function myClose()
        try
            stop(myTimerStatus);
            stop(myTimerDisplay);
            stop(myTimerAcquisition);
        catch err
        end
        try
            utCmdExit;
        catch err
            uiwait(msgbox('Error utCmdExit','modal'));
        end
        delete(gcf);
    end
    function myCloseReq(src,callbackdata)
        myClose;
    end
    function radiobutton_Callback(source,callbackdata)
       %callbackdata.OldValue.String
       data = callbackdata.NewValue;
       if radiobuttonStandard==data
           lifoEnable = 0;
       elseif radiobuttonLifo==data
           lifoEnable = 1;
       end
    end
    %function editMaxCycle_Callback(hObject, eventdata, handles)
        % hObject    handle to edit1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)
        
        % Hints: get(hObject,'String') returns contents of edit1 as text
        %        str2double(get(hObject,'String')) returns contents as double
    %    maxAscanCount = str2num( get(gcbo,'String') );
    %end
    %function editMaxPoint_Callback(hObject, eventdata, handles)
        % hObject    handle to edit1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)
        
        % Hints: get(hObject,'String') returns contents of edit1 as text
        %        str2double(get(hObject,'String')) returns contents as double
    %    maxAscanPointCount = str2num( get(gcbo,'String') );
    %end
    function editIP0_Callback(hObject, eventdata, handles)
        % hObject    handle to edit1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)
        
        % Hints: get(hObject,'String') returns contents of edit1 as text
        %        str2double(get(hObject,'String')) returns contents as double
        strIP0 = get(gcbo,'String');
    end
    function editIP1_Callback(hObject, eventdata, handles)
        % hObject    handle to edit1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)
        
        % Hints: get(hObject,'String') returns contents of edit1 as text
        %        str2double(get(hObject,'String')) returns contents as double
        strIP1 = get(gcbo,'String');
    end
    function editAcquisition_Callback(hObject, eventdata, handles)
        % hObject    handle to edit1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)
        
        % Hints: get(hObject,'String') returns contents of edit1 as text
        %        str2double(get(hObject,'String')) returns contents as double
        valueTimerAcquisition = str2double(get(gcbo,'String'));
        set(myTimerAcquisition ,'Period', valueTimerAcquisition);
    end
    function editDisplay_Callback(hObject, eventdata, handles)
        % hObject    handle to edit1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)
        
        % Hints: get(hObject,'String') returns contents of edit1 as text
        %        str2double(get(hObject,'String')) returns contents as double
        valueTimerDisplay = str2double(get(gcbo,'String'));
        set(myTimerDisplay ,'Period', 1.0/valueTimerDisplay);
    end
    function editRange_Callback(hObject, eventdata, handles)
        % hObject    handle to edit1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)
        
        % Hints: get(hObject,'String') returns contents of edit1 as text
        %        str2double(get(hObject,'String')) returns contents as double
        valueRange = str2double(get(gcbo,'String'))*1e-6;
        if pulser==0
            mxSetAscanRange256(deviceId0,deviceId1,CycleCount0,valueRange,0);
        else
            mxSetAscanRange256(deviceId0,deviceId1,CycleCount0,valueRange,2);
        end
    end
    function pushbuttonClose_Callback(hObject, eventdata, handles)
        % hObject    handle to pushbutton1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)
        %acquisition('Goodbye');
        myClose();
    end
    function pushbuttonLoad_Callback(hObject, eventdata, handles)
        % hObject    handle to pushbutton1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)
        CycleCount0 = 0;
        CycleCount1 = 0;
        try
            strFile128A = winqueryreg('HKEY_CURRENT_USER','Software\utKernel\matlab','SetupFile128A');
            strFile128B = winqueryreg('HKEY_CURRENT_USER','Software\utKernel\matlab','SetupFile128B');
        catch err
            strFile128A = 0;
            strFile128B = 0;
        end
        if(isnumeric(strFile128A) & ~strFile128A)
            [ret, folder] = uigetfile('*.txt');
        else
            [ret, folder] = uigetfile(strFile128A);
        end
        if ret==0
            return;
        end
        fullFileNameA = fullfile(folder, ret);
        if(isnumeric(strFile128B) & ~strFile128B)
            [ret, folder] = uigetfile('*.txt');
        else
            [ret, folder] = uigetfile(strFile128B);
        end
        if ret==0
            return;
        end
        fullFileNameB = fullfile(folder, ret);
        b = mxReadFileWriteHW256(deviceId0,deviceId1,fullFileNameA,fullFileNameB);
        if b
            strFile128A = fullFileNameA;
            strFile128B = fullFileNameB;
            %writereg('HKEY_CURRENT_USER','Software\utKernel\matlab','SetupFile',strFile);
            mxSaveRegistryString('Software\utKernel\matlab','SetupFile128A',strFile128A);
            mxSaveRegistryString('Software\utKernel\matlab','SetupFile128B',strFile128B);
            CycleCount0 = mxGetSWCycleCount(deviceId0);
            if deviceId1>=0
                CycleCount1 = mxGetSWCycleCount(deviceId1);
            else
                CycleCount1 = 0;
            end
        else
            uiwait(msgbox(strFile,'Error mxReadFileWriteHW256','modal'));
        end
        updateDialog
    end
    function pushbuttonSave_Callback(hObject, eventdata, handles)
        % hObject    handle to pushbutton1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)
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
        b = mxReadHWWriteFile(deviceId0,fullFileName);
    end
    %function checkboxFifo_Callback(hObject, eventdata, handles)
        % hObject    handle to checkbox1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)    
        % Hint: get(hObject,'Value') returns toggle state of checkbox1
    %    if (get(gcbo,'Value'))
    %        fifoEnable = 1;
    %    else
    %        fifoEnable = 0;
    %    end
    %    updateDialog();
    %end
    %function checkboxUSB3_Callback(hObject, eventdata, handles)
        % hObject    handle to checkbox1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)    
        % Hint: get(hObject,'Value') returns toggle state of checkbox1
    %    if (get(gcbo,'Value'))
    %        mxEnableUsb3(deviceId, 1);
    %    else
    %        mxEnableUsb3(deviceId, 0);
    %    end
    %end
    function checkboxDevice1_Callback(hObject, eventdata, handles)
        % hObject    handle to checkbox1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)    
        % Hint: get(hObject,'Value') returns toggle state of checkbox1
        
        if (get(gcbo,'Value'))
            if deviceId0==-1
                try
                    if (get(checkboxDevice2,'Value'))
                        deviceId0 = utCmdNewDevice(strIP0, [128 4096 0]);%new device (specify IP address and options: max 1024 ascan in the fifo and max ascan size is 16384).
                    else
                        deviceId0 = utCmdNewDevice(strIP0, [128 4096 1]);%new device (specify IP address and options: max 1024 ascan in the fifo and max ascan size is 16384).
                    end
                catch
                    deviceId0 = -1;
                end
                if deviceId0<0
                    set(checkboxDevice1, 'Value', 0);
                end
            end
%             if deviceId1==-1
%                 try
%                     deviceId1 = utCmdNewDevice(strIP1, [128 4096 0]);%new device (specify IP address and options: max 1024 ascan in the fifo and max ascan size is 16384).
%                 catch
%                     deviceId1 = -1;
%                 end
%                 if deviceId1<0
%                     set(checkboxDevice2, 'Value', 0);
%                 end
%             end
            updateDialog();
        else
            if deviceId0>=0
                mxDeleteDevice(deviceId0);
                deviceId0 = -1;
            end
%             if deviceId1>=0
%                 mxDeleteDevice(deviceId1);
%                 deviceId1 = -1;
%             end
            updateDialog();
        end
    end
    function checkboxDevice2_Callback(hObject, eventdata, handles)
        % hObject    handle to checkbox2 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)    
        % Hint: get(hObject,'Value') returns toggle state of checkbox1
        
        if (get(gcbo,'Value'))
%             if deviceId0==-1
%                 try
%                     deviceId0 = utCmdNewDevice(strIP0, [128 4096 0]);%new device (specify IP address and options: max 1024 ascan in the fifo and max ascan size is 16384).
%                 catch
%                     deviceId0 = -1;
%                 end
%                 if deviceId0<0
%                     set(checkboxDevice1, 'Value', 0);
%                 end
%             end
            if deviceId1==-1
                try
                    deviceId1 = utCmdNewDevice(strIP1, [128 4096 0]);%new device (specify IP address and options: max 1024 ascan in the fifo and max ascan size is 16384).
                catch
                    deviceId1 = -1;
                end
                if deviceId1<0
                    set(checkboxDevice2, 'Value', 0);
                end
            end
            updateDialog();
        else
%             if deviceId0>=0
%                 mxDeleteDevice(deviceId0);
%                 deviceId0 = -1;
%             end
            if deviceId1>=0
                mxDeleteDevice(deviceId1);
                deviceId1 = -1;
            end
            updateDialog();
        end
    end
    function checkboxConnect_Callback(hObject, eventdata, handles)
        % hObject    handle to checkbox1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)    
        % Hint: get(hObject,'Value') returns toggle state of checkbox1
        
        if (get(gcbo,'Value'))
            if connected==0
                b0 = mxConnect(deviceId0,1);
                if ~b0
                    set(checkboxConnect, 'Value', 0);
                end
                if deviceId1>=0
                    b1 = mxConnect(deviceId1,1);
                    if ~b1
                        set(checkboxConnect, 'Value', 0);
                    end
                end
                updateDialog();
                %if mxIsUsb3Connected(deviceId)
                    %set(checkboxUSB3, 'Visible', 'on');
                    %set(checkboxUSB3, 'Enable', 'on');
                    %set(checkboxUSB3, 'Value', 1);
                    %mxEnableUsb3(deviceId, 1);
                %else
                    %set(checkboxUSB3, 'Visible', 'off');
                    %set(checkboxUSB3, 'Enable', 'off');
                %end
            end
        else
            if connected==1
                b0 = mxConnect(deviceId0,0);
                if deviceId1>=0
                    b1 = mxConnect(deviceId1,0);
                end
                updateDialog();
            end
        end
    end
    function checkboxEnablePulser_Callback(hObject, eventdata, handles)
        % hObject    handle to checkbox1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)    
        % Hint: get(hObject,'Value') returns toggle state of checkbox1
        
        if ~connected
            return;
        end
        if (get(gcbo,'Value'))
            if pulser==0
                if deviceId1>=0
                    mxFlushAcquisitionAscanFifo256(deviceId0,deviceId1);
                    mxResetCounters256(deviceId0,deviceId1);
                else
                    mxFlushAcquisitionAscanFifo(deviceId0);
                    mxResetCounters(deviceId0);
                end
                acquisitionTotalAscan = 0;
                acqAscanCountLast = 0;
                hwTotalAscanStartAcq = 0;
                hwTotalAscan = 0;
                displayTotalCount = 0;
                b = mxEnableShot256(deviceId0,deviceId1,1);
                if ~b
                    set(checkboxPulser, 'Value', 0);
                else
                    hwTotalByte = 0;
                    hwTotalByte0 = 0;
                    hwTotalByte1 = 0;
                    tic
                    TotalTime = toc;
                    displayTimeStart = toc;
                end
            end
        else
            if pulser==1
                b = mxEnableShot256(deviceId0,deviceId1,0);
            end
        end
        updateDialog();
    end
    function checkboxAcquisition_Callback(hObject, eventdata, handles)
        % hObject    handle to checkbox1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)    
        % Hint: get(hObject,'Value') returns toggle state of checkbox1
        
        if (get(gcbo,'Value'))
            if acquisition==0
                acquisitionTotalAscan = 0;
                acqAscanCountLast = 0;
                hwTotalAscanStartAcq = hwTotalAscan;
                acquisition = 1;
                updateDialog();
                start(myTimerAcquisition);
            end
        else
            if acquisition==1
                stop(myTimerAcquisition);
                acquisition = 0;
                updateDialog();
            end
        end
    end
    function checkboxDisplay_Callback(hObject, eventdata, handles)
        % hObject    handle to checkbox1 (see GCBO)
        % eventdata  reserved - to be defined in a future version of MATLAB
        % handles    structure with handles and user data (see GUIDATA)    
        % Hint: get(hObject,'Value') returns toggle state of checkbox1
        
        if (get(gcbo,'Value'))
            if display==0
                display = 1;
                updateDialog();
                displayTotalCount = 0;%total display refresh count.
                displayTimeStart = toc;
                start(myTimerDisplay);
            end
        else
            if display==1
                stop(myTimerDisplay);
                display = 0;
                updateDialog();
            end
        end
    end
end