// Kenneth Woodard
// October 9th, 2019
// Dunn - Section 3
// TA: Angel
// Lab 6
// This program calculates the total of a speeding ticket based on the location of the incident and the recorded speed relative to the speed limit.

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <iomanip>
using namespace std;

int main () {

// create all the variables for file input info
string citationNumber;
int month;
int year;
int day;
int clockedSpeed;
int speedLimit;
char road;
double interstate = 5.2252;
double highway = 9.4412;
double residential = 17.1525;
double none = 12.152;
double fine; 
int value1;
int value2;
int value;

// create array to put months into 3 letter abbr
string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// get file name from user and open file
  string filename;
  cout << "Enter a ticket file: ";
  cin >> filename;
  ifstream fin;
  fin.open(filename);

// error check to make sure file can be opened
  if (!fin) {
    cout << "Unable to open " << filename << ".\n";
    return 1;
    }

// ask user for report file and open it
  string report;
  cout << "Enter a report file: ";
  cin >> report;
  ofstream fout;
  fout.open(report);

// error check to make output file can be opened
  if (!fout) {
    cout << "Unable to open " << report << ".\n";
    return 1;
    }


// ask user for start and end date
  int MM1, MM2, DD1, DD2, YYYY1, YYYY2;

  cout << "Enter report start date (mm dd yyyy): ";
  cin >> MM1 >> DD1 >> YYYY1;
  cout << "Enter report end date   (mm dd yyyy): ";
  cin >> MM2 >> DD2 >> YYYY2;

// create while loop to input info from file
while (fin >> citationNumber >> month >> day >> year >> clockedSpeed >> speedLimit >> road) {

// if user types year as two digits assume 21st century
if (year < 100) {
year = year + 2000;
}
if (YYYY1 < 100) {
YYYY1 = YYYY1 + 2000;
}
if (YYYY2 < 100) {
YYYY2 = YYYY2 + 2000;
}

value1 = (YYYY1 * 10000) + (MM1 * 100) + DD1;
value2 = (YYYY2 * 10000) + (MM2 * 100) + DD2;
value = (year * 10000) + (month * 100) + day;

// switch statement for calculating fine  
switch (road) {
  case 'I':
  case 'i': fine = (clockedSpeed - speedLimit) * interstate;
  break;
  case 'R':
  case 'r': fine = (clockedSpeed - speedLimit) * residential;
  break;
  case 'H':
  case 'h': fine = (clockedSpeed - speedLimit) * highway;
  break;
  default: fine = (clockedSpeed - speedLimit) * none;
  break;
}

// correct negative fine
if (fine < 0) {
fine = 0;
}

// filter out dates that are not within range
if (value1 <= value && value2 >= value) {
   fout << setfill('0') << setw(2) << right << day << left << "-" 
        << setfill('0') << setw(2) << months[month - 1] << "-"
        << year << setw(10) << setfill(' ') << citationNumber << "$" << setw(9) 
        << setfill(' ') << right << setprecision(2) << fixed << fine << endl;
}

}

// close both files
fin.close();
fout.close();

return 0;
}
