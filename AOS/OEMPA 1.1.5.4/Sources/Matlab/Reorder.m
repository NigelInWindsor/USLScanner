function [ out_ray ] = Reorder(BarInspectionReorder, ray)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

if(BarInspectionReorder==0)
    out_ray = ray;
    return;
end

[isize,jsize] = size(ray);
tab = zeros(1,jsize);

out_ray = [];
index = 1;
for jj=1:jsize
    while(tab(index)>0.5)
        index = index+1;
    end
    if(tab(index)<0.5)
        out_ray = [out_ray ray(:,index)];
        tab(index) = 1;
    else
        error('error');
    end;
    index = index+BarInspectionReorder+1;
    if(index>jsize)
        index = 1;
    end
end

