function [ result ] = DumpHex( data )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

if (isa(data,'int8') | isa(data,'int16') | isa(data,'int32'))
    if isa(data,'int8')
        bSign = 256;
    else
        if isa(data,'int16')
            bSign = 256*256;
        else
            bSign = 256*256*256*256;
        end
    end
else
    bSign = 0;
end
result = [];
[row,col] = size(data);
for irow=1:row
    string = [];
    for icol=1:col
        mydata = double(data(irow,icol));
        if(bSign & (mydata<0))
            mydata = bSign+mydata;
        end
        s = sprintf('%.2X;',mydata);
        string = [string s];
    end
    result = strvcat(result,string);
end

end

