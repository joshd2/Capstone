# -*- coding: utf-8 -*-
"""
Created on Sat Apr 20 10:09:12 2019

@author: Emil McDowell
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os

def main():
    
    alldata = pd.DataFrame(columns = ["frequency", "S11 (Mag)",
            "S11 (Phase)", "S21M", "S21 (Phase)", "S12 (Mag)", "S12 (Phase)", "S22 (Mag)", "S22 (Phase)", "angle"])
    for filename in os.listdir(os.getcwd()):
        if (filename != "Sample.py" and filename != "export_dataframe.csv"):
            print(filename)
            
            #Parsing the file name to get the angle value
            #Assumes the file name will be of the form N#-#.S2P
            #Where - stands for the decimal point, and N indicates a minus sign
            #Some files may have to be changed to meet this format
            string1, string2 = filename.split("-")
            string2, string3 = string2.split(".")
            if (string1[0] == 'N'):
                string1 = '-' + string1[1:] + '.'
            else:
                string1 = string1 + '.'
            angle = float(string1 + string2)
            
            #Reads the data into a data frame
            freqdata = pd.read_csv(filename, delim_whitespace = True, header = None, names = ["frequency", "S11 (Mag)",
            "S11 (Phase)", "S21M", "S21 (Phase)", "S12 (Mag)", "S12 (Phase)", "S22 (Mag)", "S22 (Phase)"],
            skiprows = 8, dtype = float)
            
            freqdata = freqdata.assign(angle = angle) 
            alldata = pd.concat([alldata, freqdata], ignore_index=True)
    
    #grouping the data by it's angle value and finding the index of the minimums    
    anglegroup = alldata.groupby(['angle'])
    minimumids = anglegroup.S21M.idxmin().values
    angles = anglegroup.S21M.idxmin().index
    
    #Minimum Data point plotting
    minimumdata = pd.DataFrame(columns = ["angle", "frequency", "S21M"])
    for ids in minimumids:
        minfreq = alldata.at[ids, "frequency"]
        minangle = alldata.at[ids, "angle"]
        minS21= alldata.at[ids, "S21M"]
        minimumdatainc = pd.DataFrame(data = [[minangle, minfreq, minS21]], columns = ["angle", "frequency", "S21M"])
        minimumdata = pd.concat([minimumdata , minimumdatainc], ignore_index=True)
    minimumdata.plot.scatter(x = "angle", y = "S21M")
    minimumdata.plot.scatter(x = "angle", y = "frequency")
    
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot(minimumdata["angle"].values, minimumdata["S21M"].values, minimumdata["frequency"].values)
    plt.show()
    minimumdata.to_csv(r'export_dataframe.csv', index=False)
    
    #Plotting Across All Angles
    plt.figure()
    for points in angles:
        subdata = anglegroup.apply(lambda x: x[x['angle'] == points])
        x = (subdata["frequency"].values)*10**-9
        plt.plot(x, subdata["S21M"].values)
    #plt.legend(angles)
    axes = plt.gca()
    #axes.set_ylim([-0.001, 0.0025])
    #axes.set_xlim([160,280])
    plt.xlabel('Frequency (GHz)')
    plt.ylabel('Intensity (dB)')
    plt.title("Intensity vs Frequency")
    plt.show()
        


        
main()