#!/usr/bin/python

__author__ = 'gdiaz'

import serial
import syslog
import time

#The following line is for serial over GPIO
port = '/dev/ttyACM0'

class CubeHWInterface(object):
    def __init__(self, serial):
        self.arduino = serial
        self.states = [0, 0, 0, 0]

    def getState(self, state):  #state = {"x1", "x2", "x3"}
        stateReceived = self.arduino.readline()
        # if (len(stateReceived))
        # while (stateReceived[0:2] != state):
        #     stateReceived = self.arduino.readline()
        # print ("Message from arduino: ")
        print (stateReceived)
        # print type(msg)
        # print "length(msg)="+str(len(msg))
        return stateReceived
    def sendState(self, state): #state = "xi=777.777"
        received_OK = False
        # while (not(received_OK)):
        # print ("Python value sent: ")
        # print (state)
        self.arduino.write(state)
        self.arduino.write('\n')
        self.arduino.flush()
        # time.sleep(0.05)
        # received_OK = self.getState(state[0:2]) != state[0:2]
        # print ("Write Succesful")
    def checkStateKey(self, str_state, key):
        if str_state[0:2] == key: return True
        return False
    def updateInputs(self, x1_str, x2_str, x3_str, u1_str):
        if (self.checkStateKey(x1_str, "x1")):
            try:
                x1 = float(x1_str[3:])
                self.states[0] = x1
            except Exception as e:
                print 'Exception thrown while converting %s to float' % (x1_str)
                # raise e
        if (self.checkStateKey(x2_str, "x2")):
            try:
                x2 = float(x2_str[3:])
                self.states[1] = x2
            except Exception as e:
                print 'Exception thrown while converting %s to float' % (x2_str)
                # raise e
        if (self.checkStateKey(x3_str, "x3")):
            try:
                x3 = float(x3_str[3:])
                self.states[2] = x3
            except Exception as e:
                print 'Exception thrown while converting %s to float' % (x3_str)
                # raise e
        if (self.checkStateKey(u1_str, "u1")):
            try:
                u1 = float(u1_str[3:])
                self.states[3] = u1
            except Exception as e:
                print 'Exception thrown while converting %s to float' % (u1_str)
                # raise e

if __name__ == '__main__':
    arduino_serial = serial.Serial(port, 115200, timeout=5)
    cube_hw = CubeHWInterface(arduino_serial)
    i = 0
    while (i < 400):
        # print "iter = "+str(i)
        # cube_hw.sendState("x1=111.111")
        # x1_str = cube_hw.getState("x1")
        # cube_hw.sendState("x2=222.222")
        # x2_str = cube_hw.getState("x2")
        # cube_hw.sendState("x3=333.333")
        # x3_str = cube_hw.getState("x3")
        # cube_hw.updateInputs(x1_str, x2_str, x3_str, u1_str)
        cube_hw.sendState("x1=1450.15")
        cube_hw.sendState("x2=234.93")
        cube_hw.sendState("x3=41.57")
        u1_str = cube_hw.getState("u1")
        cube_hw.updateInputs("x1=0", "x2=0", "x3=0", u1_str)
        print "Python Values:"+str(cube_hw.states)
        i = i + 1
    arduino_serial.close()