#!/usr/bin/python

__author__ = 'gdiaz'

import math
import scipy as sp
import matplotlib.pyplot as plt

class Integral(object):
	def __init__(self, initialValue):
		self.prviousValue = initialValue
		self.actualValue = 0

	def discreteIntegral(self, input):
		self.actualValue = self.prviousValue + input;
		self.prviousValue = self.actualValue;
		return self.actualValue

if __name__ == '__main__':
	discrete_integral = Integral(0)
	# Initial values
	n = 0
	f = 1
	dn = 0.01
	n_f = 5
	t_array = []
	y_array = []
	y2_array = []
	Ki = dn
	while True:
		u = math.sin(2*math.pi*f*n)
		x1 = Ki*discrete_integral.discreteIntegral(u)
		t_array.append(n)
		y_array.append(u)
		y2_array.append(x1)
		n += dn
		if n>=n_f: break

	# Show  graph in a figure
	plt.figure()
	plt.plot(t_array, y_array, 'r', label=r"$sin(2\pi t)$")
	plt.hold(True)
	plt.plot(t_array, y2_array, 'b', label=r"$\int \; sin(2\pi t)$")
	plt.legend()
	plt.xlabel(r"Time$[s]$", family='serif', fontsize = 24, color = (0,0,1))
	plt.ylabel(r"Amplitude,$y(t)$", family='serif', fontsize = 24, color = 'blue')
	plt.grid(True)
	plt.grid(color = '0.5', linestyle = '--', linewidth = 1)
	# plt.axis('tight')
	# plt.axis([0, 7, -3, 3])
	plt.title('Signal test for integer module', family='serif', style = 'italic',fontsize = 20, color = 'black', verticalalignment = 'baseline', horizontalalignment = 'center')
	plt.savefig('integerTest.png')
	plt.show()