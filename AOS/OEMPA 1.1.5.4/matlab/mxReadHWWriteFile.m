function [result] = mxReadHWWriteFile(deviceId,filename)
% INPUT PARAMETER
%deviceId : identification number of the device.
%filename : OEMPA file path.
% OUTPUT
%return value (result): 1 if the file have been loaded in the HW and 0 if error.

global g_Init;
if ~exist('g_Init','var')
    error('error: you have to load the UTKernelMatlab.dll');
end
if ~g_Init
    error('error: you have to load the UTKernelMatlab.dll');
end
if(~ischar(filename))
    error('input should be a string');
end

if exist(filename, 'file')
  % File does not exist.
  warningMessage = sprintf('Error: file does exist:\n%s', filename);
  uiwait(msgbox(warningMessage));
  result = 0;
  return;
end

result = utCmd(deviceId,'mxReadHWWriteFile',filename);