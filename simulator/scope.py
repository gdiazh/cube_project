#!/usr/bin/python

__author__ = 'gdiaz'

import math
import scipy as sp
import matplotlib.pyplot as plt

class Scope(object):
	def __init__(self, name, x, y, signalName):
		self.name = name
		self.x = x
		self.y = y
		self.signalName = signalName

	def plot(self):
		# Show  graph in a figure
		plt.figure()
		plt.plot(self.x, self.y, 'b', label=self.signalName)
		plt.legend()
		plt.grid(True)
		plt.grid(color = '0.5', linestyle = '--', linewidth = 1)
		plt.axis([0, 1, -1000, 1000])
		plt.title(self.name, family='serif', style = 'italic',fontsize = 20, color = 'black', verticalalignment = 'baseline', horizontalalignment = 'center')
		plt.savefig(self.name+'.png')
		plt.show()

if __name__ == '__main__':
	# Initial values
	n = 0
	f = 1
	dn = 0.01
	n_f = 5
	t_array = []
	y_array = []
	while True:
		u = math.sin(2*math.pi*f*n)
		t_array.append(n)
		y_array.append(u)
		n += dn
		if n>=n_f: break

	scope = Scope('Scope Test', t_array, y_array, 'sin(x)')
	scope.plot()