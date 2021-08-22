import matplotlib.pyplot as plt
import csv, sys

# verify we have 1 extra argument, hope it's the file name.
if (len(sys.argv) != 2):
    print("Incorrect number of args - the only argument should be the steering data path")
    exit()
    
# assign arg to filepath var
filepath = sys.argv[1]

# setup some lists to hold wheel data.  X dim is time, y is angle
wheel_00_timestamp = []
wheel_00_angle = []
wheel_01_timestamp = []
wheel_01_angle = []
wheel_10_timestamp = []
wheel_10_angle = []
wheel_11_timestamp = []
wheel_11_angle = []

# super nasty
with open(filepath, newline='\n') as steeringDataFile:
    fileReader = csv.reader(steeringDataFile, delimiter=',')
    next(fileReader)
    for i in range(4):
        wheelCount = 0
        for row in fileReader:
            if (int(row[0]) == 0 and int(row[1]) == 0):
                wheel_00_timestamp.append(float(row[2]))
                wheel_00_angle.append(float(row[3]))
            if (int(row[0]) == 0 and int(row[1]) == 1):
                wheel_01_timestamp.append(float(row[2]))
                wheel_01_angle.append(float(row[3]))
            if (int(row[0]) == 1 and int(row[1]) == 0):
                wheel_10_timestamp.append(float(row[2]))
                wheel_10_angle.append(float(row[3]))
            if (int(row[0]) == 1 and int(row[1]) == 1):
                wheel_11_timestamp.append(float(row[2]))
                wheel_11_angle.append(float(row[3]))

# plot it all out
plt.ylabel("Angle")
plt.xlabel("Time")
plt.plot(wheel_00_timestamp,wheel_00_angle,'r',label="Axle 0, Wheel 0")
plt.plot(wheel_01_timestamp,wheel_01_angle,'g',label="Axle 0, Wheel 1")
plt.plot(wheel_10_timestamp,wheel_10_angle,'b',label="Axle 1, Wheel0")
plt.plot(wheel_11_timestamp,wheel_11_angle,'y',label="Axle 1, Wheel1")
plt.legend()
plt.plot
plt.show()