function Display(probe_number)

TRLRef = [];
SystemScript
if(strcmp(specimen_type,'Plane'))
    specimen_radius = 0;
end
if(~exist('multiple_channel','var'))
    multiple_channel = 0;
end
if(~multiple_channel)
    figure(1);
    clf;hold on;
    fig_nb = 1;
    SystemDisplay(fig_nb,probe_number,probe_points,wedge_enable,specimen_type,specimen_radius,ProbeType,TRLRef);
    ScanReceptionScript
    ScanReceptionDisplay(fig_nb,ray_probe_points,ray_interface_points,...
        ray_focal_points,wedge_enable,specimen_type,specimen_radius );

else
    for ii=0:multiple_channel-1
        figure(1+ii);
        clf;hold on;
        fig_nb =1+ii;
        SystemDisplay(fig_nb,probe_number,probe_points,wedge_enable,specimen_type,specimen_radius,ProbeType,TRLRef);
        string = strcat('ScanReceptionScript_',int2str(ii));
        eval(string);
        ScanReceptionDisplay(fig_nb,ray_probe_points,ray_interface_points,...
            ray_focal_points,wedge_enable,specimen_type,specimen_radius );
    end
end
end