function SystemDisplay(fig_nb,probe_number,probe_points,wedge_enable,specimen_type,specimen_radius,ProbeType,TRLRef)

if(strcmp(specimen_type,'Plane'))
    specimen_radius = 0;
end
%clf;hold on;
figure(fig_nb);
xlabel('X');
ylabel('Y');
zlabel('Z');
minX = min(probe_points(1,:));
maxX = max(probe_points(1,:));
minY = min(probe_points(2,:));
maxY = max(probe_points(2,:));
minZ = min(probe_points(3,:));
maxZ = max(probe_points(3,:));
[isize,jsize] = size(probe_points);
if((maxX>minX) & (maxY>minY))
    if(probe_number>0)
        for i=1:jsize
            stri = int2str(i);
            text(probe_points(1,i)*1000,probe_points(2,i)*1000,(probe_points(3,i)-specimen_radius)*1000,stri);
        end
    else
        for i=1:jsize
            plot3(probe_points(1,i)*1000,probe_points(2,i)*1000,(probe_points(3,i)-specimen_radius)*1000,'r.');
        end
        stri = int2str(1);
        text(probe_points(1,1)*1000,probe_points(2,1)*1000,(probe_points(3,1)-specimen_radius)*1000,stri);
        stri = int2str(jsize);
        text(probe_points(1,jsize)*1000,probe_points(2,jsize)*1000,(probe_points(3,jsize)-specimen_radius)*1000,stri);
    end
else
    for i=1:jsize
        plot3(probe_points(1,i)*1000,probe_points(2,i)*1000,(probe_points(3,i)-specimen_radius)*1000,'+');
    end
    stri = int2str(1);
    text(probe_points(1,1)*1000,probe_points(2,1)*1000,(probe_points(3,1)-specimen_radius)*1000,stri);
    stri = int2str(jsize);
    text(probe_points(1,jsize)*1000,probe_points(2,jsize)*1000,(probe_points(3,jsize)-specimen_radius)*1000,stri);
end
if(exist('ProbeType') && strcmp(ProbeType,'CustomTRL'))
   plot3(TRLRef(1,1)*1000,TRLRef(2,1)*1000,(TRLRef(3,1)-specimen_radius)*1000,'+g');
end
if(strcmp(specimen_type,'Plane'))
    specimenX=linspace(minX*1.5,maxX*1.5,4);
    specimenY=linspace(minY*1.5,maxY*1.5,4);
    specimenZ=[0 0 0 0;0 0 0 0;0 0 0 0;0 0 0 0];
    if(wedge_enable>0)
        mesh(specimenX*1000,specimenY*1000,specimenZ*1000);
    end
    if(maxX>minX)
        set(gca,'XLim',[minX maxX]*1.5*1000);
    end
    if(maxY>minY)
        set(gca,'YLim',[minY maxY]*1.5*1000);
    end
    if(abs(maxZ)>0)
        set(gca,'ZLim',[-abs(maxZ) abs(maxZ)]*1.5*1000);
    end
else
    for i=1:jsize
        alpha = atan2(probe_points(3,i),probe_points(2,i));
        if(i==1)
            alphaMin = alpha;
            alphaMax = alpha;
        else
            if(alphaMin>alpha)
                alphaMin = alpha;
            end
            if(alphaMax<alpha)
                alphaMax = alpha;
            end
        end
    end
    alpha = (alphaMax+alphaMin)/2;
    alphaDelta = (alphaMax-alphaMin);
    alphaMin2 = alpha-alphaDelta*1.5;
    alphaMax2 = alpha+alphaDelta*1.5;
    specimenX = linspace(minX*1.5,maxX*1.5,8);
    specimenY = specimen_radius*cos(linspace(alphaMin2,alphaMax2,8));
    specimenZ = specimen_radius*sin(linspace(alphaMin2,alphaMax2,8))'*[1 1 1 1 1 1 1 1];
    if(wedge_enable>0)
        mesh(specimenX*1000,specimenY*1000,(specimenZ-specimen_radius)*1000);
    end
    minX = min([minX min(specimenX)]);
    maxX = max([maxX max(specimenX)]);
    minY = min([minY min(specimenY)]);
    maxY = max([maxY max(specimenY)]);
    if(maxX>minX)
        set(gca,'XLim',[minX maxX]*1.5*1000);
    end
    if(maxY>minY)
        set(gca,'YLim',[minY maxY]*1.5*1000);
    end
    if(abs(maxZ)>0)
        set(gca,'ZLim',[min(min(specimenZ))-specimen_radius maxZ-specimen_radius]*1.5*1000);
    end
end

end