%%%%%ECE413 S2P file renderer
%%%%%Created by Stephen Johnston on 4-23
clear all
files = dir('C:\Users\Stephen Johnston\Desktop\School Stuff\Quarter 10\ECE413\AllTheSmallData\Smallsamples\Delta\45 deg down\*.S2P');
    for i = 1:length(files)
    S2P = importdata(['C:\Users\Stephen Johnston\Desktop\School Stuff\Quarter 10\ECE413\AllTheSmallData\Smallsamples\Delta\45 deg down\' files(i).name]);
    S_param{i} = S2P.data(:,2:end);
    end
%Getting the S2P measurements    
    S11_mean = 0;
    S12_mean = 0;
    for i = 1:length(files)
   S11_mean = S11_mean + S_param{i}(:,1);
   S12_mean = S12_mean + S_param{i}(:,3);
    end
S11_mean = S11_mean/length(files);
S12_mean = S12_mean/length(files);
%Takes Smean


Sigma = 0;

        for j = 1:length(files)
        Sigma = Sigma + (S_param{j}(:,3) - S12_mean).^2;
        end
    Sigma = sqrt(Sigma/(length(files)-1));
%Creates standard deviation
plot(S2P.data(:,1)/10^9,S12_mean, '-b')
hold on;
plot(S2P.data(:,1)/10^9,S12_mean-Sigma, '-r')
hold on;
plot(S2P.data(:,1)/10^9,S12_mean+Sigma, '-r')
title('Delta S12 measurement over all angles, 60 degree zenith')
legend('S12 mean','S12 mean - \sigma', 'S12_mean + \sigma')
xlabel('Frequency (GHz)')
ylabel('Magnitude dB')
%Plots all bollanger bands