function ScanReceptionDisplay(  fig_nb,ray_probe_points,ray_interface_points,...
                                ray_focal_points,wedge_enable,specimen_type,specimen_radius )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

figure(fig_nb);
[iprobe,jprobe] = size(ray_probe_points);
[iinterface,jinterface] = size(ray_interface_points);
[ifocal,jfocal] = size(ray_focal_points);

if((iprobe~=3) || (iinterface~=3) || (ifocal~=3))
    return;
end
if((jprobe~=jinterface) || (jfocal~=jinterface))
    return;
end

for j=1:jprobe
    plot3([ray_probe_points(1,j) ray_interface_points(1,j) ray_focal_points(1,j)]*1000,...
        [ray_probe_points(2,j) ray_interface_points(2,j) ray_focal_points(2,j)]*1000,...
        ([ray_probe_points(3,j) ray_interface_points(3,j) ray_focal_points(3,j)]-specimen_radius)*1000,'r');
    plot3([ray_probe_points(1,j)]*1000,...
        [ray_probe_points(2,j)]*1000,...
        ([ray_probe_points(3,j)]-specimen_radius)*1000,'b+');
    plot3([ray_interface_points(1,j)]*1000,...
        [ray_interface_points(2,j)]*1000,...
        ([ray_interface_points(3,j)]-specimen_radius)*1000,'b+');
    plot3([ray_focal_points(1,j)]*1000,...
        [ray_focal_points(2,j)]*1000,...
        ([ray_focal_points(3,j)]-specimen_radius)*1000,'b+');
end

XLim = get(gca,'XLim')/1000.0;
YLim = get(gca,'YLim')/1000.0;
ZLim = get(gca,'ZLim')/1000.0;
minX = min([XLim(1) ray_probe_points(1,:) ray_interface_points(1,:) ray_focal_points(1,:)]);
maxX = max([XLim(2) ray_probe_points(1,:) ray_interface_points(1,:) ray_focal_points(1,:)]);
minY = min([YLim(1) ray_probe_points(2,:) ray_interface_points(2,:) ray_focal_points(2,:)]);
maxY = max([YLim(2) ray_probe_points(2,:) ray_interface_points(2,:) ray_focal_points(2,:)]);
minZ = min([ZLim(1)+specimen_radius ray_probe_points(3,:) ray_interface_points(3,:) ray_focal_points(3,:)]);
maxZ = max([ZLim(2)+specimen_radius ray_probe_points(3,:) ray_interface_points(3,:) ray_focal_points(3,:)]);
set(gca,'XLim',[minX maxX]*1.5*1000);
set(gca,'YLim',[minY maxY]*1.5*1000);
set(gca,'ZLim',[minZ-specimen_radius maxZ-specimen_radius]*1.5*1000);

end

