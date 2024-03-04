# Kenneth Woodard
# kwooda13 
# July 15, 2022
# August 2013 High Temps for Knoxville,TN
# Homework 1
# COSC 370

from numpy import arange
import matplotlib.pyplot as plt
xData = arange(1,32)    # Ranges for x and y axes must match
tData = [86,87,84,86,86,86,84,83,90,89,88,85,86,79,83,81, \
         75,80,81,85,81,88,89,87,84,85,86,88,88,90,90]
avg = []  # First value for montly avg high temp is just Day 1 temp

## print given table
#print("tData given table")
#print(tData)
#print("\n")

a = 0
total = float(0)
av = float(0)

temp = []

## calculate averages
## outer for loop goes through
## days in the month
## ---- inner for loop finds the
## ---- sum of the temps b4 up to
## ---- that day
## outer loop then divides by
## number of days and rounds to
## two digits

for x in xData:
    a = x
    total = 0
    while a > 0:
        total += tData[a-1]
        a = a - 1
    av = total / x
    av = float(av)
    av = round(av, 2)
    avg.append(av)

## print average values to stdout
print(" --- running monthly avg table --- ")
print(avg)
print("\n")

## plot graph for both high temp
plt.plot(xData, tData, color = 'blue', marker='o', markerfacecolor='red', markersize=5, linewidth = '1')
plt.plot(xData, avg, color = 'green', linestyle = 'dashed', linewidth = '1')
plt.text(15, 86, "Monthly Avg", color = "green")

## set graph limits
plt.xlim(0,32) 
plt.ylim(70,95)
## set gridlines
plt.grid(color = 'black', linestyle = ':', linewidth = 0.8)
## set labels
plt.xlabel('Days')
plt.ylabel('High Temp (F)')

## set graph title
plt.title('High Temperatures for Knoxville, TN - August 2013')
plt.show()

## Dr. Berry's advice
##
##
## 1) CALCULATE A RUNNING MONTHLY AVERAGE AND PRINT IT OUT IN A TABLE
##    IT DOES NOT MATTER HOW THE TABLE IS FORMATTED
##
## 2) CREATE A GRAPH FOR THE DATA USING MATPLOTLIB
##	- PLOT RED POINTS WITH BLUE LINE FOR ORIGINAL DATA
##	- SHOW CHANGE IN AVERAGE HIGH WITH GREEN DASHED LINE
##	- SET DISPLAY RANGES FOR X AND Y AXES
##		- X SHOULD RANGE FROM 0 TO 32
##		- Y SHOULD RANGE FROM 70 TO 95
##	- ENABLE GRID DISPLAY
##	- LABEL AXES AND SET TITLE
##	- USE MATPLOTLIB.PYPLOT.TEXT() TO LABEL THE MONTHLY AVERAGE LINE


