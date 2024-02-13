// Kenneth Woodard
// 9/3/19
// Dunn Section 3
// TA: Angel
// Lab 2

#include <iostream>
#include <string>

using namespace std;

int main() {

int streetNum;
string streetName;
string streetType;
int lastmValue;
int thismValue;
int differenceValue;
int estMortgage; 

   cout << "Enter the property street number, name, and type: ";
   cin >> streetNum >> streetName >> streetType;
   cout << "Last month's property value: $"; 
   cin >> lastmValue;
   cout << "This month's property value: $";
   cin >> thismValue;
   cout << '\n';
   cout << "This property is located at " << streetNum << " " << streetName << " " << streetType << ".\n";
   
//thismValue-lastmValue = differenceValue;

   cout << "Its value has changed by $" << (thismValue - lastmValue) << " since last month.\n";

//(thismValue*0.045)/12 = estMortgage;

   cout << "The estimated monthly mortgage is $" << ((thismValue*0.045)/12) << ".\n";

return 0;
}
