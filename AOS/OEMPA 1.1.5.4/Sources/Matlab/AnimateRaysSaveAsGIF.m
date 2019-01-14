%%
probe_number = 0;
SystemScript
if(strcmp(specimen_type,'Plane'))
    specimen_radius = 0;
end
figure(1);
clf;hold on;
fig_nb = 1;
%view(90,0);
SystemDisplay2(fig_nb,probe_number,probe_points,wedge_enable,specimen_type,specimen_radius);
ScanReceptionScript
if(~exist('BarInspectionScanCount','var'))
    ScanReceptionScript
else
    g_ray_probe_points=[];
    g_ray_interface_points=[];
    g_ray_focal_points=[];
    g_left_probe_points=[];
    g_left_interface_points=[];
    g_left_focal_points=[];
    g_right_probe_points=[];
    g_right_interface_points=[];
    g_right_focal_points=[];
    for ii=0:BarInspectionScanCount-1
        if(BarInspectionScanCount==1)
            string = strcat('ScanReceptionScript');
        else
            string = strcat('ScanReceptionScript_',int2str(ii));
        end
        eval(string);
        g_ray_probe_points = [g_ray_probe_points ray_probe_points];
        g_ray_interface_points = [g_ray_interface_points ray_interface_points];
        g_ray_focal_points = [g_ray_focal_points ray_focal_points];
        g_left_probe_points = [g_left_probe_points left_probe_points];
        g_left_interface_points = [g_left_interface_points left_interface_points];
        g_left_focal_points = [g_left_focal_points left_focal_points];
        g_right_probe_points = [g_right_probe_points right_probe_points];
        g_right_interface_points = [g_right_interface_points right_interface_points];
        g_right_focal_points = [g_right_focal_points right_focal_points];
    end
    ray_probe_points = Reorder(BarInspectionReorder,g_ray_probe_points);
    ray_interface_points = Reorder(BarInspectionReorder,g_ray_interface_points);
    ray_focal_points = Reorder(BarInspectionReorder,g_ray_focal_points);
    left_probe_points = Reorder(BarInspectionReorder,g_left_probe_points);
    left_interface_points = Reorder(BarInspectionReorder,g_left_interface_points);
    left_focal_points = Reorder(BarInspectionReorder,g_left_focal_points);
    right_probe_points = Reorder(BarInspectionReorder,g_right_probe_points);
    right_interface_points = Reorder(BarInspectionReorder,g_right_interface_points);
    right_focal_points = Reorder(BarInspectionReorder,g_right_focal_points);
end
%%
filename = 'testnew58.gif'; %specify save name and save in present working directory
%filename = '/Volumes/Music Partition/Dropbox (AOS)/Gruber-AOS/ThunderbirdDownloads/Matlab/testnew57.gif'; %specify save location
TimeBetweenFrames = 0.1;    %set the speed of the .gif (seconds per frame)



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

%view(-270, 0);   %change viewing angle of plot

%call plot once to get figure handles
for j=1
    filli = fill([left_probe_points(2,j) left_interface_points(2,j) left_focal_points(2,j) right_interface_points(2,j) right_probe_points(2,j)]*1000,...
        ([left_probe_points(3,j) left_interface_points(3,j) left_focal_points(3,j) right_interface_points(3,j) right_probe_points(3,j)])*1000,'g');
    rayi = plot([ray_probe_points(2,j) ray_interface_points(2,j) ray_focal_points(2,j)]*1000,...
        ([ray_probe_points(3,j) ray_interface_points(3,j) ray_focal_points(3,j)])*1000,'r');
    %lefti = plot([left_probe_points(2,j) left_interface_points(2,j) left_focal_points(2,j)]*1000,...
    %    ([left_probe_points(3,j) left_interface_points(3,j) left_focal_points(3,j)])*1000,'r');
    %righti = plot([right_probe_points(2,j) right_interface_points(2,j) right_focal_points(2,j)]*1000,...
    %    ([right_probe_points(3,j) right_interface_points(3,j) right_focal_points(3,j)])*1000,'r');

    ray_probep = plot([ray_probe_points(2,j)]*1000,...
        ([ray_probe_points(3,j)])*1000,'b.');
    ray_focalp = plot([ray_focal_points(2,j)]*1000,...
        ([ray_focal_points(3,j)])*1000,'b.');

    left_probep = plot([left_probe_points(2,j)]*1000,...
        ([left_probe_points(3,j)])*1000,'b.');
    left_focalp = plot([left_focal_points(2,j)]*1000,...
        ([left_focal_points(3,j)])*1000,'b.');

    right_probep = plot([right_probe_points(2,j)]*1000,...
        ([right_probe_points(3,j)])*1000,'b.');
    right_focalp = plot([right_focal_points(2,j)]*1000,...
        ([right_focal_points(3,j)])*1000,'b.');

 %xlim([-80 80])   %set plot limits
 %ylim([-80 80])
 %zlim([-80 80])
 axis equal;
end

%%
% Cycle through elements, replacing starting and ending values for red
% lines
if 1
for j=1:jprobe

    set(filli,'XData',[left_probe_points(2,j) left_interface_points(2,j) left_focal_points(2,j) right_interface_points(2,j) right_probe_points(2,j)]*1000);
    set(filli,'YData',[left_probe_points(3,j) left_interface_points(3,j) left_focal_points(3,j) right_interface_points(3,j) right_probe_points(3,j)]*1000);
    
    %replace ray coordinates
    set(rayi,'XData',[ray_probe_points(2,j) ray_interface_points(2,j) ray_focal_points(2,j)]*1000);
    set(rayi,'YData',[ray_probe_points(3,j) ray_interface_points(3,j) ray_focal_points(3,j)]*1000);

    %set(lefti,'XData',[left_probe_points(2,j) left_interface_points(2,j) left_focal_points(2,j)]*1000);
    %set(lefti,'YData',[left_probe_points(3,j) left_interface_points(3,j) left_focal_points(3,j)]*1000);

    %set(righti,'XData',[right_probe_points(2,j) right_interface_points(2,j) right_focal_points(2,j)]*1000);
    %set(righti,'YData',[right_probe_points(3,j) right_interface_points(3,j) right_focal_points(3,j)]*1000);

    %replace probe point coordinates
    set(ray_probep,'XData',[ray_probe_points(2,j)]*1000);
    set(ray_probep,'YData',[ray_probe_points(3,j)]*1000);
    %replace interface point coordinates
    %set(ray_interp,'XData',[ray_interface_points(2,j)]*1000);
    %set(ray_interp,'YData',[ray_interface_points(3,j)]*1000);
    %replace focal point coordinates
    set(ray_focalp,'XData',[ray_focal_points(2,j)]*1000);
    set(ray_focalp,'YData',[ray_focal_points(3,j)]*1000);
    
    %replace probe point coordinates
    set(left_probep,'XData',[left_probe_points(2,j)]*1000);
    set(left_probep,'YData',[left_probe_points(3,j)]*1000);
    %replace interface point coordinates
    %set(left_interp,'XData',[left_interface_points(2,j)]*1000);
    %set(left_interp,'YData',[left_interface_points(3,j)]*1000);
    %replace focal point coordinates
    set(left_focalp,'XData',[left_focal_points(2,j)]*1000);
    set(left_focalp,'YData',[left_focal_points(3,j)]*1000);
    
    %replace probe point coordinates
    set(right_probep,'XData',[right_probe_points(2,j)]*1000);
    set(right_probep,'YData',[right_probe_points(3,j)]*1000);
    %replace interface point coordinates
    %set(right_interp,'XData',[right_interface_points(2,j)]*1000);
    %set(right_interp,'YData',[right_interface_points(3,j)]*1000);
    %replace focal point coordinates
    set(right_focalp,'XData',[right_focal_points(2,j)]*1000);
    set(right_focalp,'YData',[right_focal_points(3,j)]*1000);
    
    drawnow; 
    pause(0.1);
    %----------------------------------------------------
    %Convert to .gif section
    
    im = frame2im(getframe);            %get figure as frame, then convert to image
    [imind,cm] = rgb2ind(im,256);       %convert image to indexed image
    if j == 1;
        imwrite(imind,cm,filename,'gif', 'Loopcount',inf,'DelayTime',TimeBetweenFrames);  %write first frame with .gif options
    else
        imwrite(imind,cm,filename,'gif','WriteMode','append','DelayTime',TimeBetweenFrames); %append all other frames
    end
    
    %----------------------------------------------------
end
end
hold off


