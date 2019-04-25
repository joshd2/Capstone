%Graph of angles
%Created by Stephen Johnston
%For ECE413
%clear all
files = dir('C:\Users\Stephen Johnston\Desktop\School Stuff\Quarter 10\ECE413\AllTheSmallData\Smallsamples\Alpha\15 deg down\*.S2P');
xlsfiles={files.name}
xlsfiles=sort(xlsfiles)
for i = 1:length(files)
    S2P = importdata(['C:\Users\Stephen Johnston\Desktop\School Stuff\Quarter 10\ECE413\AllTheSmallData\Smallsamples\Alpha\15 deg down\' xlsfiles{i}]);
    S_param{i} = S2P.data(:,2:end);
    end
   figure;
   x = S2P.data(:,1)/10^9;
   y = S_param{1}(:,3);
   plot(x,y);
   gif('Animation.gif','frame',gcf) 
    while 1 > 0
    for i = 1:length(files)
    drawnow;
    y = S_param{i}(:,3);
    plot(x,y);    
    ylim([-50 -10])
    xlabel('Frequency (GHz)')
    ylabel('Magnitude (dB)')
    title('Animated plot of angular response')
    pause(.01);
    gif
    end
    end