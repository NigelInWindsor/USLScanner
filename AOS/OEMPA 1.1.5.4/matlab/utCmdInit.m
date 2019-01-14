function [module] = utCmdInit(utKernelDllPath)
% utCmdInit(utKernelDllPath)
% utKernelDllPath (in) : string (required only the first call time)
%                       this is the full path of the kernel,
%                       example : 'C:\Users\Public\SW\UTKernel\Debug\UTKernelMatlab.dll'.
%                       This parameter is stored in the registry,
%                       so you don't need to give it after the first call.
% module (out) : many values are returned if one of them is null there is an error !

if (~nargin)
    try
        utKernelDllPath = winqueryreg('HKEY_CURRENT_USER','Software\utKernel\matlab','KernelDllPath');
        [pathstr, name, ext] = fileparts(utKernelDllPath);
        if(length(name) & ~strcmpi(name,'UTKernelMatlab.dll'))
            utKernelDllPath = [pathstr '\UTKernelMatlab' ext];
        else
            utKernelDllPath = 0;
        end
    catch err
        utKernelDllPath = 0;
    end
    if(isnumeric(utKernelDllPath) & ~utKernelDllPath)
        try
            utKernelDllPath = winqueryreg('HKEY_CURRENT_USER','Software\utKernel','KernelDllPath');
            [pathstr, name, ext] = fileparts(utKernelDllPath);
            if(length(name))
                utKernelDllPath = [pathstr '\UTKernelMatlab' ext];
            else
                utKernelDllPath = 0;
            end
        catch err
            utKernelDllPath = 0;
            display('Any key have been defined yet in registry !');
            display('There are two different solutions:');
            display('- Either fill the path of the matlab kernel DLL (as the first input parameter).');
            display('- Either run first the kernel toolbox (to register default key in registry).');
        end
    end
end
if(~ischar(utKernelDllPath))
   return;
end

display(['DLL used: ''' utKernelDllPath '''!']);
string = ['utCmd(-1,''LoadLibrary'',''' utKernelDllPath ''');'];
try
    module = eval(string);
catch err
    module = 0;
    if(strcmp(computer, 'PCWIN')>0)
        display('Error : bad working path or bad installation ("utCmd.mexw32" missing) !');
    else
        display('Error : bad working path or bad installation ("utCmd.mexw64" missing) !');
    end
    display(string);
end
if ~module
else
    writereg('HKEY_CURRENT_USER','Software\utKernel\matlab','KernelDllPath',utKernelDllPath);
end

global g_Init;
global g_InitRet;
g_InitRet = module;
if length(find(module==0))>0
    g_Init = 0;
else
    g_Init = 1;
end