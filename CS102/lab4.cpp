// Kenneth Woodard
// Dunn: Section 3
// 09/11/2019
// TA: Angel
// lab 4
// This code asks the user about their cars and trucks and outputs gas mileage information about their vehicles for them. 

#include <iostream>
#include <limits>
using namespace std;

int main () {
// variables
  string command;
  double carMiles;
  double carGallons;
  double carMPG;
  double truckMiles;
  double truckGallons;
  double truckMPG;
  double totalcarMiles;
  double totalcarGallons;
  double totaltruckMiles;
  double totaltruckGallons;
  int totalCars;
  int totalTrucks;
  
  // assign 0 to total cars and trucks initially to allow 2nd if-else statement below to execute properly
  totalCars = 0;
  totalTrucks = 0;
do {
// do-while loop to ensure command is asked for at least once
  cout << "Enter a command\n";
  cin >> command;
// if-else statements set up for the 3 different options
if (command == "car") {

  // recieve data
  cout << "Enter car's miles: ";
  cin >> carMiles;
  cin.clear();
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  cout << "Enter car's gallons: ";
  cin >> carGallons;
  cin.clear();
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  // algebra 
  totalCars = ++totalCars;
  totalcarMiles = totalcarMiles + carMiles;
  totalcarGallons = totalcarGallons + carGallons; 
}
else if (command == "truck") {
 
  // recieve data
  cout << "Enter truck's miles: ";
  cin >> truckMiles;
  cin.clear();
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  cout << "Enter truck's gallons: ";
  cin >> truckGallons;
  cin.clear();
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  // algebra
  totalTrucks = ++totalTrucks;
  totaltruckMiles = totaltruckMiles + truckMiles;
  totaltruckGallons = totaltruckGallons + truckGallons;
} 
else if (command == "done") {
   break;
}
else {
   cout << "Unknown command\n";
} 
}  while (command != "done");
   
   // if-else to output data on MPG
if (totalCars != 0 && totalTrucks != 0) {
   carMPG = totalcarMiles / totalcarGallons;
   cout << "Average car MPG: " << carMPG << endl;
   truckMPG = totaltruckMiles / totaltruckGallons;
   cout << "Average truck MPG: " << truckMPG << endl;
   
}
else if (totalCars == 0 && totalTrucks != 0) {
   truckMPG = totaltruckMiles / totaltruckGallons;
   cout << "Average truck MPG: " << truckMPG << endl;
   cout << "Fleet has no cars.\n";
}
else if (totalCars != 0 && totalTrucks == 0) {
   carMPG = totalcarMiles / totalcarGallons;
   cout << "Average car MPG: " << carMPG << endl;
   cout << "Fleet has no trucks.\n";
}
else if (totalCars == 0 && totalTrucks == 0) {
   cout << "Fleet has no cars.\n" << "Fleet has no trucks.\n";
}

return 0;
}
