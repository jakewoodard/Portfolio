#!/usr/bin/env python3
#
# Template for Problem 10 from Problem Set 3.2
#
# Insert a comment block describing the problem and curve fitting
# approach deployed.  Also, be sure your name and assignment
# number are included in your comments.
#
# Kenneth Woodard
# kwooda13
# 8/1/22
# Homework 6
# CS370

from numpy import array,zeros
from polyFit import *
import pylab

def evalPoly(c,x): # c stores coefficients for polynomial
    m = len(c) - 1 # (copied from polyFit module)
    p = c[m]
    for j in range(m):
        p = p*x + c[m-j-1]
    return p

xData = [1718, 1767, 1774, 1775, 1792, 1816, 1828, 1834, 1878, 1906] # xData array contains the years
yData = [0.5, 0.8, 1.4, 2.7, 4.5, 7.5, 12.0, 17.0, 17.2, 23.0] # yData array contains the efficiencies (as percentages)

minsdev=float("inf")
minpoly=0
n=len(xData)
print('Degree  Stdev   2000P')
for m in range(1,6):   # Try m=1,2,3,4,5 (degree of polynomial)
    ys=zeros((n),dtype='float') # initialize y-coordinates for polynomial curve

    coeff =  polyFit(xData,yData, m)    # get coefficients for n-th degree polynomial
    stdev = stdDev(coeff, xData, yData)  # get stdev of the error in the fit
    proj  = evalPoly(coeff, 2000) # evaluate the polynomial at year 2000
#
#   Year 2000 projections >= 100 or < 0 are meaniningless        
#
    if (stdev < minsdev) and proj < 100 and proj > 0 :

        print('{:3d}\t{:5.3f}\t{:5.3f}\t{:s}'.format(m,stdev,proj,'viable'))

        ys = neville(xData, yData, proj)  # get y-coordinates of polynomial using x-coordinates in xData array
#
#       Use Matplotlib to plot original data points with validated curve fitting
#
        pylab.figure()
        pylab.xlabel("x")
        pylab.ylabel("Thermal Efficiency")
        my_title= 'Fit with poly degree = ' + str(m) + '; green dot is 2000 projection'
        pylab.title(my_title)
        pylab.xlim(1710, 2015)                                 # x-axis values should range from 1710 to 2015
        pylab.plot(2000, proj, marker ='o', c = 'green')       # plot Year 2000 projection as a green dot
        pylab.plot(xData, yData, marker = 'o', c = 'red')      # plot the original data given in the xData and yData
                                                               # arrays as red dots
        pylab.plot(xData, ys, c = 'blue')                      # plot polynomial curve using xData and ys arrays
                                                      # and make it blue.
        pylab.grid()
        fname='degree' + str(m) + 'fit.png' # save figure to file rather than display
        pylab.savefig(fname)
    else :
        print('{:3d}\t{:5.3f}\t{:5.3f}\t{:s}'.format(m,stdev,proj,'not viable'))
#--------------------------------------------------------------------------------------
# Table to stdout should look similar to this...
#
# Degree Stdev   2000P
#   1 	 2.855	 34.986	    viable
#   2	 2.768	 45.419	    viable
#   3	 2.266	 -6.602	    not viable
#   4	 2.234	 112.391.   not viable
#   5	 2.496	 113.726.   not viable
#--------------------------------------------------------------------------------------
