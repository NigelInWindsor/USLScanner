function [module] = utCmdExit
% module (out) : many values are returned if one of them is null there is an error !

    %timerId = timerfind('Name', 'timerLoopWizardDisplay');
    %if(length(timerId))
    %    callback = utCmd(0,'mxSetWizardDisplayLoopCallback',0);
    %    stop(timerId);
    %    delete(timerId);
    %end

global g_Init;

g_Init = 0;
module = utCmd(-1,'FreeLibrary');

