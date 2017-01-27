#!/usr/bin/python

__author__ = 'gdiaz'

import numpy as np

class LinearModel(object):
	def __init__(self, initialx1, initialx2, initialx3):
		self.A = np.array([[0, 1, 0], [84.49, -0.58, 0.02], [-84.49, 0.58, -1.02]])
		self.B = np.array([0, -14.48, 516.48])
		self.stateX1 = initialx1
		self.stateX2 = initialx1
		self.stateX3 = initialx1
		self.dx1_dt = initialx2
		self.dx2_dt = 0
		self.dx3_dt = 0

	def setInputs(self, u, x1, x2, x3):
		x = np.array([x1, x2, x3])
		[self.dx1_dt, self.dx2_dt, self.dx3_dt] = np.dot(self.A, x) +  self.B*u