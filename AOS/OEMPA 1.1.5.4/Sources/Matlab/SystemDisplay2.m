function SystemDisplay2(fig_nb,probe_number,probe_points,wedge_enable,specimen_type,specimen_radius)

if(strcmp(specimen_type,'Plane'))
    specimen_radius = 0;
end
%clf;hold on;
figure(fig_nb);
xlabel('X');
ylabel('Y');
%zlabel('Z');
%minX = min(probe_points(1,:));
%maxX = max(probe_points(1,:));
minY = min(probe_points(2,:));
maxY = max(probe_points(2,:));
minZ = min(probe_points(3,:));
maxZ = max(probe_points(3,:));
[isize,jsize] = size(probe_points);
if(maxY>minY)
    if(probe_number>0)
        for i=1:jsize
            stri = int2str(i);
            text(probe_points(2,i)*1000,(probe_points(3,i))*1000,stri);
        end
    else
        for i=1:jsize
            plot(probe_points(2,i)*1000,(probe_points(3,i))*1000,'r.');
        end
        stri = int2str(1);
        text(probe_points(2,1)*1000,(probe_points(3,1))*1000,stri);
        stri = int2str(jsize);
        text(probe_points(2,jsize)*1000,(probe_points(3,jsize))*1000,stri);
    end
else
    for i=1:jsize
        plot(probe_points(2,i)*1000,(probe_points(3,i))*1000,'+');
    end
    stri = int2str(1);
    text(probe_points(2,1)*1000,(probe_points(3,1))*1000,stri);
    stri = int2str(jsize);
    text(probe_points(2,jsize)*1000,(probe_points(3,jsize))*1000,stri);
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
    specimenY = specimen_radius*cos(linspace(0,2*pi,100));
    specimenZ = specimen_radius*sin(linspace(0,2*pi,100))'*[1 1 1 1 1 1 1 1];
    if(wedge_enable>0)
        plot(specimenY*1000,specimenZ*1000,'LineWidth',3);
    end
end

end