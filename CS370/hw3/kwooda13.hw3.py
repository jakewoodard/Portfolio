#!/usr/bin/env python3
#######################################################
# probs2_2.py
# 
# Scripts corresponding to problems 12, 17, and 18 on
# pages 79-82.
######################################################

########################
## Kenneth Woodard
## CS370
## Homework 3
## July 21, 2022
########################

import numpy
from numpy import array,zeros
from choleski import *
from gaussElimin import *

## problem2_2_12
######################################################
# 1. Populate k, W, and a below with the values from
#    problem 12 on page 79.
#
# Correct Output: 
# [ 1.66666667  2.66666667  2.66666667]
#
######################################################

#k = array(??,float)
#W = array(??,float)
#a = zeros((3,3))
#a[0,0] = ??
#a[0,1] = ??
#a[1,0] = ??
#a[0,2] = ??
#a[2,0] = ??
#a[1,1] = ??
#a[1,2] = ??
#a[2,1] = ??
#a[2,2] = ??
#L = choleski(a)
#x = choleskiSol(L,W)
#print("Displacements are (in units of W/k):\n\n",x)
#print("--------------------------------------------")

k = 1
W = 1

k1 = k3 = k4 = k
k2 = k5 = 2 * k
W1 = W3 = 2 * W
W2 = W

A = [[k1 + k2 + k3 + k5, -k3, -k5], [-k3, k3 + k4, -k4], [-k5, -k4, k4 + k5]]
B = [W1, W2, W3]
inverse = numpy.linalg.inv(A)
X = numpy.dot(inverse, B)
print("Displacements are (in units of W/k):\n\n", X)
print("--------------------------------------------")

## problem2_2_17
######################################################
# 1. Populate a and b below with the values from
#    problem 17 on page 82.
#
# Correct Outputs:
# R = 5.0 ohms
# The currents are (in amps):
# [ 2.82926829 1.26829268 4.97560976]
# R = 10.0 ohms
# The currents are (in amps):
# [ 2.66666667 1.33333333 4.88888889]
# R = 20.0 ohms
# The currents are (in amps):
# [ 2.4516129 1.41935484 4.77419355]
#
######################################################

b = [0,0,0]
R = [5.0, 10.0, 20.0]
for r in R:
   a = zeros([3,3])
   r_0 = (50 + r, -r, -30)
   a[0,:] = r_0
   r_0 = (-r, 65 + r, -15)
   a[1,:] = r_0
   r_0 = (-30, -15, 45)
   a[2,:] = r_0
   b[0] = 0
   b[1] = 0
   b[2] = 120
   print("\nR =", r, "ohms")
   print("The currents are (in amps):\n", gaussElimin(a, b))
print("--------------------------------------------")

## problem2_2_18
######################################################
# 1. Populate a and b below with the values from
#    problem 18 on page 82.
#
# Correct Output:
# The loop currents are (in amps):
# [-4.18239492 -2.66455194 -2.71213323 -1.20856463]
######################################################

a = zeros([4,4])
a[0,:] = (80, -50, -30, 0)
a[1,:] = (-50, 100, -10, -25)
a[2,:] = (-30, -10, 65, -20)
a[3,:] = (0, -25, -20, 100)
b = [-240, 0, 0, 0]
print("The currents are (in amps):\n",gaussElimin(a,b))

input("Press return to exit")
