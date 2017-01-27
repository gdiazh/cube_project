#!/usr/bin/python

__author__ = 'gdiaz'

import scipy as sp
import matplotlib.pyplot as plt
import serial
# Use Linear Model
from LinearModel import LinearModel
from cube_hw_interface import CubeHWInterface

from integral import Integral
from scope import Scope
import time

class CubeModel(object):
	def __init__(self, initialx1, initialx2, initialx3):
		self.x1 = initialx1
		self.x2 = initialx2
		self.x3 = initialx3
		self.dx1_dt = initialx2
		self.dx2_dt = 0
		self.dx3_dt = 0
		self.linear_model = LinearModel(initialx1, initialx2, initialx3)

	def updateStates(self, x1, x2, x3):
		self.x1 = x1
		self.x2 = x2
		self.x3 = x3

	def setInputs(self, u, x1, x2, x3):
		self.linear_model.setInputs(u, x1, x2, x3)
		self.dx1_dt = self.linear_model.dx1_dt
		self.dx2_dt = self.linear_model.dx2_dt
		self.dx3_dt = self.linear_model.dx3_dt

if __name__ == '__main__':
	# cube = CubeModel(0.01, 0, 0)#5*3.14/180
	cube = CubeModel(5*3.14/180, 0, 0)
	port = '/dev/ttyACM0'
	arduino_serial = serial.Serial(port, 230400, timeout=5)
	cube_hw = CubeHWInterface(arduino_serial)

	discrete_integral1 = Integral(0)
	discrete_integral2 = Integral(0)
	discrete_integral3 = Integral(0)
	# Initial values
	# u = 0
	# x1 = 1*180/3.14
	# x2 = 0
	# x3 = 0
	# Controller parameters for test
	K1 = -11561.15
	K2 = -1236.20
	K3 = -17.32
	# Sample parameters
	Tsignal = 0.1
	Ts = 0.001
	ti = 0
	tf = 2*Tsignal
	time_arr = []
	sampled_cube_angle = []
	sampled_dx1 = []
	sampled_u1 = []
	while True:
		# print "actual t = " +str(ti)
		# Store time and signals as vectors
		time_arr.append(ti)
		sampled_cube_angle.append(cube.x1*180/3.1415926)
		sampled_dx1.append(cube.dx1_dt)

		# send state to Arduino
		# cube_hw.sendState("x1="+str(cube.x1))
		# cube_hw.sendState("x2="+str(cube.x2))
		# cube_hw.sendState("x3="+str(cube.x3))
		# cube_hw.sendState("x1=0.12")
		# cube_hw.sendState("x2=0.93")
		# cube_hw.sendState("x3=0.57")
		# print "state send x: "+str(cube.x1)+","+str(cube.x2)+","+str(cube.x3)

		# Control Input
		# u = getInput()

		# u1_str = cube_hw.getState("u1")
		# cube_hw.updateInputs("x1=0", "x2=0", "x3=0", u1_str)
		# u = cube_hw.states[3]

		u = -(K1*cube.x1+K2*cube.x2+K3*cube.x3)*0.0251
		cube.setInputs(u, cube.x1, cube.x2, cube.x3)
		sampled_u1.append(u)

		# Update states, uncomment for prube whith estimated states
		cube.x1 = discrete_integral1.discreteIntegral(cube.dx1_dt*Ts)
		cube.x2 = discrete_integral2.discreteIntegral(cube.dx2_dt*Ts)
		cube.x3 = discrete_integral3.discreteIntegral(cube.dx3_dt*Ts)
		# cube.updateStates(x1, x2, x3)
		# # Update states, uncomment for prube whith Arduino mesured states
		# cube.x1 = 
		# cube.x2 = 
		# cube.x3 = 

		# update auxiliari variables
		ti+=Ts
		if ti>=tf: break
		# time.sleep(0.01)
	# Show  graph in a figure
	# scope1 = Scope('Linear System Simulation', time_arr, sampled_cube_angle, 'Cube Angle')
	# scope1.plot()
	# scope1 = Scope('states dinamics', time_arr, sampled_dx1, 'dx1')
	# scope1.plot()
	# scope3 = Scope('InputControlArduino', time_arr, sampled_u1, 'u')
	# scope3.plot()
	plt.figure()
	plt.plot(time_arr, sampled_cube_angle, 'b', label=r"$\theta_{b}$")
	plt.legend()
	plt.xlabel(r"Time$[s]$", family='serif', fontsize = 24, color = (0,0,1))
	plt.ylabel(r"Cube Angle,$\theta_{b}$", family='serif', fontsize = 24, color = 'blue')
	plt.grid(True)
	plt.grid(color = '0.5', linestyle = '--', linewidth = 1)
	# plt.axis('tight')
	plt.axis([0, 0.2, -5, 10])
	plt.title('Linear System Simulation with Arduino', family='serif', style = 'italic',fontsize = 20, color = 'black', verticalalignment = 'baseline', horizontalalignment = 'center')
	plt.savefig('linearSistemTestArduino.png')
	plt.show()