#!/usr/bin/python

__author__ = 'gdiaz'

# ROS INTERFACE

"""Provides a high level interface over ROS to control de CUBE(*) system.
CUBE: Project implemented on course EL5202. See Documentation.
"""
import sys
import math
import rospy

from threading import Thread

from std_msgs.msg import String

from bluetooth import *

class CubeInterface:
    def __init__(self):
        # Only argument stuff
        self.running = False
        self.client_socket_=BluetoothSocket(RFCOMM)
        self.data_file = open("data.txt", "wb+")

    def initialize(self):
        # Get params and allocate msgs
        self.state_update_rate = rospy.get_param('/rate', 2)
        #Buetooth
        self.client_socket_.connect(("00:13:04:03:00:02", 1)) #arduino
        print "Conection succed! starting comunication ..."

    def start(self):
        # Create subs, services, publishers, threads
        self.running = True
		#subscribers
        self.command_sub = rospy.Subscriber('/cube_command', String, self.process_command)
        #publishers
        self.data_pub = rospy.Publisher('/cube_data', String)
        Thread(target=self.update_state).start()

    def stop(self):
        self.running = False
        self.command_sub.unregister()
        self.data_pub.unregister()
        print "Conection Finish. Closing ports ..."
        self.client_socket_.close()

    def process_command(self, msg):
        print "msg to send:"+msg.data
        if (msg.data == "end") : self.running = False
        self.client_socket_.send(msg.data)

    def valid_data(self, data):
        return True
        # print len(data)
        cnt = 0
        for char in data:
            if char == ";": cnt+=1
        print cnt
        if cnt == 5:
            print "valid data"
            return True
        print "no valid data"
        return False

    def update_state(self):
        rate = rospy.Rate(self.state_update_rate)
        while self.running and not rospy.is_shutdown():
            # Receive data
            data = self.client_socket_.recv(1024)
            # data = self.client_socket_.recv(32)
            # print 'Received: ', data
            if self.valid_data(data):
                #write data to file
                self.data_file.write(data)
                #publish data
                self.data_pub.publish(data)
            rate.sleep()

if __name__ == '__main__':
    rospy.init_node('cube_interface')
    cube = CubeInterface()
    cube.initialize()
    cube.start()
    rospy.spin()
    cube.stop()