# First Python Ever here we go
# Kenneth Woodard
# COSC 423

import csv


def analyze_applicant1 ( app ) : 
  """
  Given the GPAs of a single applicant , return True if they are qualified
  Qualification : An applicant is qualified if ...
  − The average of all their grades is above 85
  @param applicant : a l i s t of GPAs ( integers )
  @return True if the applicant qualifies 
  """
  # Simulate an assessment
  # find average
  avg = 0
  avg = int(app[0]) + int(app[1]) + int(app[2]) + int(app[3]) + int(app[4]) + int(app[5])
  avg /= 6

  if (avg > 85):
    return True
  else:
    return False

def analyze_applicant2 ( app ) :
  if (int(app[0]) < 65):
    return False
  if (int(app[1]) < 65):
    return False
  if (int(app[2]) < 65):
    return False
  if (int(app[3]) < 65):
    return False
  if (int(app[4]) < 65):
    return False
  if (int(app[5]) < 65):
    return False
  else:
    return True

def analyze_applicant3 ( app ) :
  x = 0

  if (int(app[0]) < 85):
    x += 1
  if (int(app[1]) < 85):
    x += 1
  if (int(app[2]) < 85):
    x += 1
  if (int(app[3]) < 85):
    x += 1
  if (int(app[4]) < 85):
    x += 1
  if (int(app[5]) < 85):
    x += 1
  if (x > 2):
    return False
  else:
    return True

def analyze_applicant4 ( app ) :
  avg = 0
  avg = int(app[0]) + int(app[1]) + int(app[2]) + int(app[3]) + int(app[4])
  avg /= 5
  if (avg > 85):
    return True
  else:
    return False

# fold all functions together
def analyze_applicant (x) :
    if analyze_applicant1(x) and analyze_applicant2(x) and analyze_applicant3(x) and analyze_applicant4(x):
      out.append('ACCEPT')
    else:
      out.append('REJECT')

# open & read file
filename = "applicants.csv"
outfile = "results.csv"
fields = [] 
rows = []
out = []

# reading file
with open(filename, 'r') as csvfile:
  csvreader = csv.reader(csvfile)
  fields = next(csvreader)

  for row in csvreader:
    rows.append(row)

# analyze
for row in rows:
  analyze_applicant(row)

# writing file  
with open(outfile, 'w') as csvfile:
  csvwriter = csv.writer(csvfile) 
  csvwriter.writerows(out)

# testing for each function (complete)
"""
apple = [99, 99, 99, 99, 99, 99]
if analyze_applicant1(apple):
  print("1 pass")
if analyze_applicant2(apple):
  print("2 pass")
if analyze_applicant3(apple):
  print("3 pass")
if analyze_applicant4(apple):
  print("4 pass")
"""
