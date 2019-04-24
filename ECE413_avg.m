%Figure std deviation
%Created by Stephen Johnston
%For ECE413

Alpha = open('C:\Users\Stephen Johnston\Desktop\School Stuff\Quarter 10\ECE413\Alpha_boll_bands-60deg.fig');
D=get(gca,'Children');
Sig(1,:) = get(D(1),'YData') - get(D(3),'YData');
Mean(1,:) = get(D(3),'YData')
Bravo = open('C:\Users\Stephen Johnston\Desktop\School Stuff\Quarter 10\ECE413\Bravo_boll_bands-60deg.fig');
D=get(gca,'Children');
Sig(2,:) = get(D(1),'YData') - get(D(3),'YData');
Mean(2,:) = get(D(3),'YData')
Charlie = open('C:\Users\Stephen Johnston\Desktop\School Stuff\Quarter 10\ECE413\Charlie_boll_bands-60deg.fig');
D=get(gca,'Children');
Sig(3,:) = get(D(1),'YData') - get(D(3),'YData');
Mean(3,:) = get(D(3),'YData')
Delta = open('C:\Users\Stephen Johnston\Desktop\School Stuff\Quarter 10\ECE413\Delta_boll_bands-60deg.fig');
D=get(gca,'Children');
Sig(4,:) = get(D(1),'YData') - get(D(3),'YData');
Mean(4,:) = get(D(3),'YData')
Mean_total = 0;
for i = 1:4
Sigma = Sig(i,:).^2;
Mean_total = Mean_total + Mean(i,:)
end

frequency = get(D(3),'XData')
Mean_total = Mean_total/4;
Sigma_total = sqrt(Sigma)/3;
close all
plot(frequency,Mean_total, '-b')
hold on;
plot(frequency,Mean_total-Sigma_total, '-r')
hold on;
plot(frequency,Mean_total+Sigma_total, '-r')
title('S12 measurement total averaged from the 4 materials, 60 degree zenith')
legend('S12 mean','S12 mean - \sigma', 'S12 mean + \sigma')
xlabel('Frequency (GHz)')
ylabel('Magnitude dB')

