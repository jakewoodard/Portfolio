#!/usr/bin/env python3
# prob2-3-19 (hwk #4)
#
#  Problem 2.3.19 (pg. 101)
#
#  1) Implement the Ax(v) function below so that it will initialize
#     and return the matrix-vector product Ax, where A is given at
#     the top of page 101 in the textbook.
#  
#  2) Use the provided conjGrad.py module to solve the system described
#     in problem #19 (uncomment and fill in the '??' lines and provide
#     the requested loop and final error difference).
#
# The correct solution is:
#
# [ 21.42857143 38.39285714  57.14285714   47.32142857   75.
#   90.17857143 92.85714286 124.10714286 128.57142857]
#
########################################################################

# Kenneth Woodard
# CS 370
# Homework 4
# netID: kwooda13
# 7/26/22

import numpy
from numpy import zeros,array,linalg
from conjGrad import *

# Refer to pg. 101 for what the matrix Ax should be

def Ax(v):
    Ax = zeros((9))*1.0
    # fill in the 9 rows of Ax
    n = len(v)
    Ax[0] = 4*v[0] - v[1] + v[n-1]
    Ax[n-1] = -v[n-2] + 4*v[n-1] + v[0]

    for i in range(1,n-1):
        Ax[i] = -v[i-1]+4*v[i]-v[i+1]
    return Ax

b = array([0,0,100,0,0,100,200,200,300])*(-1.0)
x = zeros((9))*1.0
tol = 1e-06
s1,numIter = conjGrad(Ax, x, b)

print("\nThe solution is:\n",s1)
print("\nNumber of iterations =",numIter, "using Tol: ", 1e-06)

print("\n CG Convergence Test")
print("Iterations   Tolerance")

TOL = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
a = 1.0
for u in range(0, 8):
    a /= 100.0
    TOL[u] = a

for r in range(0, 8):
    s1, numIter = conjGrad(Ax, x, b, TOL[r])
    print("  ", end = "   ")
    print(numIter, end = "       ")
    print(TOL[r])

s1, numIter = conjGrad(Ax, x, b, 1e-06)
s2, numIter = conjGrad(Ax, x, b, 1e-16)

err = numpy.abs(s1 - s2)

    
# Create a loop here to call conjGrad with these tolerances and obtain/print the iteration
# count with each tolerance used:  1.e-02, 1.e-04, 1.e-06, 1.e-08, 1.e-10, 1.e-12, 1.e-14, 1.e-16
#
#
# Finally, print the error between the solutions using the 1.e-06 and 1.e-16 tolerances
print("\nError between vectors obtained with tol=1e-06 and tol=1e-16: ",err,"\n")
