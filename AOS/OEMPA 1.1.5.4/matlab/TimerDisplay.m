function [lastOut display] = TimerDisplay(tstart, lastIn);
%timer of 4 secondes for the display of the acquisition status.
%outputs
%display: 0 not time for the display, 1 display should be done.
%lastOut: new value for the input lastIn

if length(lastIn)==0
    display = 1;
    lastOut = round(toc(tstart));
    return;
end

next = round( toc(tstart) );
if (rem( next, 5)==4) && (next>lastIn)
    display = 1;
else
    display = 0;
end
lastOut = next;
